// visfx-kms-demo.c — freestanding KMS "live usage" demo for the emulated
// HP VISUALIZE fx card. No libdrm: raw DRM ioctls only, so it static-links and
// drops straight into the busybox initramfs.
//
// It opens the DRM device, enumerates the connector + its mode list, allocates
// and draws into a dumb framebuffer, then drives a SEQUENCE of real modesets
// (640x480 -> 1600x1200). Each modeset makes the driver program the card's
// VHAL/VTG/CFG timing registers, which land in QEMU's device trace — that is
// the demo: the emulated card being driven through several resolutions.
//
// HONEST LIMITATION: the drawn pixels are NEVER scanned out. The QEMU model has
// no display surface and the driver's plane update is a no-op (a documented
// GAP), so nothing appears on a screen. This exercises the control/modeset
// path, not pixel output.
//
// Build: gcc -static -O2 -o visfx-demo visfx-kms-demo.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <drm/drm.h>
#include <drm/drm_mode.h>

/* Resolutions to cycle through, matched by name prefix against the connector's
 * advertised modes. Each drives one modeset -> one VHAL/VTG/CFG register group
 * in the trace. */
static const char *kTargets[] = {
	"640x480", "800x600", "1024x768", "1280x1024", "1600x1200",
};
#define NTARGETS (int)(sizeof(kTargets) / sizeof(kTargets[0]))

static int drm_ioctl(int fd, unsigned long req, void *arg)
{
	int r;
	do { r = ioctl(fd, req, arg); } while (r < 0 && (errno == EINTR || errno == EAGAIN));
	return r;
}

int main(void)
{
	const char *dev = "/dev/dri/card0";
	int fd = open(dev, O_RDWR | O_CLOEXEC);
	if (fd < 0) { fprintf(stderr, "open %s: %s\n", dev, strerror(errno)); return 1; }

	printf("=================== visfx KMS demo ===================\n");
	printf("opened %s\n", dev);

	if (drm_ioctl(fd, DRM_IOCTL_SET_MASTER, 0) < 0)
		printf("note: SET_MASTER failed (%s) — continuing\n", strerror(errno));

	/* ---- resources (two-pass) ---- */
	struct drm_mode_card_res res; memset(&res, 0, sizeof(res));
	if (drm_ioctl(fd, DRM_IOCTL_MODE_GETRESOURCES, &res) < 0) {
		fprintf(stderr, "GETRESOURCES: %s\n", strerror(errno)); return 1;
	}
	uint64_t fb_ids[16], crtc_ids[16], conn_ids[16], enc_ids[16];
	res.fb_id_ptr        = (uint64_t)(uintptr_t)fb_ids;
	res.crtc_id_ptr      = (uint64_t)(uintptr_t)crtc_ids;
	res.connector_id_ptr = (uint64_t)(uintptr_t)conn_ids;
	res.encoder_id_ptr   = (uint64_t)(uintptr_t)enc_ids;
	if (res.count_crtcs > 16) res.count_crtcs = 16;
	if (res.count_connectors > 16) res.count_connectors = 16;
	if (res.count_fbs > 16) res.count_fbs = 16;
	if (res.count_encoders > 16) res.count_encoders = 16;
	if (drm_ioctl(fd, DRM_IOCTL_MODE_GETRESOURCES, &res) < 0) {
		fprintf(stderr, "GETRESOURCES(2): %s\n", strerror(errno)); return 1;
	}
	printf("resources: %u crtc(s), %u connector(s), %ux%u..%ux%u\n",
	       res.count_crtcs, res.count_connectors,
	       res.min_width, res.min_height, res.max_width, res.max_height);
	if (res.count_crtcs < 1 || res.count_connectors < 1) {
		fprintf(stderr, "no crtc/connector\n"); return 1;
	}
	uint32_t crtc_id = (uint32_t)crtc_ids[0];

	/* ---- pick a connector that has modes (two-pass each) ---- */
	struct drm_mode_modeinfo modes[64];
	struct drm_mode_get_connector conn;
	uint32_t conn_id = 0, nmodes = 0;
	for (uint32_t i = 0; i < res.count_connectors; i++) {
		memset(&conn, 0, sizeof(conn));
		conn.connector_id = (uint32_t)conn_ids[i];
		if (drm_ioctl(fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn) < 0) continue;
		if (conn.count_modes == 0) continue;
		uint32_t want = conn.count_modes > 64 ? 64 : conn.count_modes;
		memset(&conn, 0, sizeof(conn));
		conn.connector_id = (uint32_t)conn_ids[i];
		conn.modes_ptr = (uint64_t)(uintptr_t)modes;
		conn.count_modes = want;
		if (drm_ioctl(fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn) < 0) continue;
		conn_id = (uint32_t)conn_ids[i];
		nmodes = conn.count_modes > want ? want : conn.count_modes;
		break;
	}
	if (!conn_id) { fprintf(stderr, "no connector with modes\n"); return 1; }

	printf("connector %u: connection=%s, %u mode(s):\n", conn_id,
	       conn.connection == 1 ? "connected" :
	       conn.connection == 2 ? "disconnected" : "unknown", nmodes);
	for (uint32_t i = 0; i < nmodes; i++)
		printf("   mode[%2u] %-14s %u kHz %ux%u\n", i, modes[i].name,
		       modes[i].clock, modes[i].hdisplay, modes[i].vdisplay);

	/* ---- dumb buffer at the max advertised size; draw a test pattern ---- */
	struct drm_mode_create_dumb creq; memset(&creq, 0, sizeof(creq));
	creq.width = res.max_width; creq.height = res.max_height; creq.bpp = 32;
	if (drm_ioctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &creq) < 0) {
		fprintf(stderr, "CREATE_DUMB: %s\n", strerror(errno)); return 1;
	}
	struct drm_mode_map_dumb mreq; memset(&mreq, 0, sizeof(mreq));
	mreq.handle = creq.handle;
	if (drm_ioctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq) < 0) {
		fprintf(stderr, "MAP_DUMB: %s\n", strerror(errno)); return 1;
	}
	uint8_t *fb = mmap(0, creq.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mreq.offset);
	if (fb == MAP_FAILED) { fprintf(stderr, "mmap: %s\n", strerror(errno)); return 1; }

	/* vertical gradient + 8 vertical colour bars (in guest RAM; not scanned out) */
	static const uint32_t bars[8] = {
		0xffffff, 0xffff00, 0x00ffff, 0x00ff00,
		0xff00ff, 0xff0000, 0x0000ff, 0x000000,
	};
	for (uint32_t y = 0; y < creq.height; y++) {
		uint32_t shade = (y * 255) / creq.height;
		uint32_t *row = (uint32_t *)(fb + (uint64_t)y * creq.pitch);
		for (uint32_t x = 0; x < creq.width; x++) {
			uint32_t bar = bars[(x * 8) / creq.width];
			uint32_t r = (((bar >> 16) & 0xff) * shade) / 255;
			uint32_t g = (((bar >>  8) & 0xff) * shade) / 255;
			uint32_t b = (((bar      ) & 0xff) * shade) / 255;
			row[x] = (r << 16) | (g << 8) | b;
		}
	}
	printf("drew test pattern into %ux%u dumb buffer (pitch %u, %llu bytes)\n",
	       creq.width, creq.height, creq.pitch, (unsigned long long)creq.size);

	struct drm_mode_fb_cmd fbc; memset(&fbc, 0, sizeof(fbc));
	fbc.width = creq.width; fbc.height = creq.height; fbc.pitch = creq.pitch;
	fbc.bpp = 32; fbc.depth = 24; fbc.handle = creq.handle;
	if (drm_ioctl(fd, DRM_IOCTL_MODE_ADDFB, &fbc) < 0) {
		fprintf(stderr, "ADDFB: %s\n", strerror(errno)); return 1;
	}

	/* ---- drive a modeset for each target resolution ---- */
	printf("--- driving %d modesets (each writes VHAL/VTG/CFG) ---\n", NTARGETS);
	int done = 0;
	for (int t = 0; t < NTARGETS; t++) {
		int mi = -1;
		for (uint32_t i = 0; i < nmodes; i++)
			if (strncmp(modes[i].name, kTargets[t], strlen(kTargets[t])) == 0) { mi = (int)i; break; }
		if (mi < 0) { printf(">>> %-10s : not advertised, skipping\n", kTargets[t]); continue; }

		struct drm_mode_crtc set; memset(&set, 0, sizeof(set));
		set.crtc_id = crtc_id;
		set.fb_id = fbc.fb_id;
		set.set_connectors_ptr = (uint64_t)(uintptr_t)&conn_id;
		set.count_connectors = 1;
		set.mode_valid = 1;
		set.mode = modes[mi];
		printf(">>> SETCRTC %-14s (%u kHz, %ux%u)\n", modes[mi].name,
		       modes[mi].clock, modes[mi].hdisplay, modes[mi].vdisplay);
		if (drm_ioctl(fd, DRM_IOCTL_MODE_SETCRTC, &set) < 0) {
			printf("    SETCRTC failed: %s\n", strerror(errno));
			continue;
		}
		done++;
		usleep(150 * 1000); /* separate the register groups in time */
	}

	printf("=================== demo done: %d/%d modesets ===================\n",
	       done, NTARGETS);
	munmap(fb, creq.size);
	close(fd);
	return 0;
}
