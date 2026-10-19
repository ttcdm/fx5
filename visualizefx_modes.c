// SPDX-License-Identifier: GPL-2.0
/*
 * visualizefx_modes.c — the display modes the HP VISUALIZE fx5/fx10 is known
 * to drive.
 *
 * STATUS: KNOWN DATA. Every timing below is transcribed verbatim from the
 * MonitorTimingData lines in HP's own Linux X server config file
 * (usr/X11R6/hp/default/X0screens, hpgraphics-1.5.13-1). HP's format is:
 *
 *   hdisplay, hsync_start, hsync_end, htotal,
 *   vdisplay, vsync_start, vsync_end, vtotal,
 *   pixel_clock_in_kHz
 *
 * which maps one-to-one onto struct drm_display_mode. Sync polarities are not
 * given by HP; VESA DMT conventions are applied (GAP where a mode is not a
 * standard DMT mode — marked below). Feeding these as the connector's mode
 * list is correct regardless of whether CRTC programming is finished.
 */
#include <drm/drm_modes.h>
#include <drm/drm_connector.h>
#include <drm/drm_edid.h>

#include "visualizefx_drv.h"

struct visfx_timing {
	const char *name;
	int clock;			/* kHz */
	int hdisplay, hss, hse, htotal;
	int vdisplay, vss, vse, vtotal;
	bool stereo;			/* HP "3D Stereoscopic" group */
};

/* KNOWN: X0screens "FX 5/10 graphic cards support the following ... modes". */
static const struct visfx_timing visfx_timings[] = {
	/* name            clk     hd   hss  hse  ht    vd   vss  vse  vt   stereo */
	{ "640x480@75",   31500,  640, 656, 720, 840,  480, 481, 484, 500, false },
	{ "640x480@85",   36000,  640, 696, 752, 832,  480, 481, 484, 509, false },
	{ "800x600@60",   40000,  800, 840, 968,1056,  600, 601, 605, 627, false },
	{ "800x600@75",   49500,  800, 816, 896,1056,  600, 601, 604, 625, false },
	{ "800x600@85",   56250,  800, 832, 896,1048,  600, 601, 604, 631, false },
	{ "1024x768@60",  65000, 1024,1048,1184,1344,  768, 771, 777, 806, false },
	{ "1024x768@75",  78750, 1024,1040,1136,1312,  768, 769, 772, 800, false },
	{ "1024x768@85",  94500, 1024,1072,1168,1376,  768, 769, 772, 808, false },
	{ "1280x1024@60",108000, 1280,1328,1440,1640, 1024,1025,1028,1066, false },
	{ "1280x1024@75",135000, 1280,1296,1440,1688, 1024,1025,1028,1066, false },
	{ "1280x1024@85",157500, 1280,1344,1504,1728, 1024,1025,1028,1072, false },
	{ "1600x1200@60",162000, 1600,1664,1856,2160, 1200,1201,1204,1250, false },
	{ "1600x1200@75",202500, 1600,1664,1856,2160, 1200,1201,1204,1250, false },
	{ "1600x1200@85",229500, 1600,1664,1856,2160, 1200,1201,1204,1250, false },
	/* Common non-standard VESA modes (HP-provided; polarity: GAP). */
	{ "1600x1024@76",170000, 1600,1632,1792,2096, 1024,1027,1030,1070, false },
	{ "1920x1200@50",152892, 1920,1928,2200,2480, 1200,1201,1204,1233, false },
	{ "1920x1200@60",185553, 1920,1936,2208,2496, 1200,1201,1204,1239, false },
	{ "1920x1200@72",226974, 1920,1944,2224,2528, 1200,1201,1204,1247, false },
	/* 3D stereoscopic timings (enabled only with EnableStereoVisuals). */
	{ "800x600@120s",  83950,  800, 856, 944,1088,  600, 601, 604, 643, true  },
	{ "1024x768@120s",139054, 1024,1104,1216,1408,  768, 769, 772, 823, true  },
	{ "1280x1024@100s",190960,1280,1376,1520,1760, 1024,1025,1028,1085, true  },
	{ "1280x1024@110s",211218,1280,1376,1520,1760, 1024,1025,1028,1091, true  },
};

int visfx_connector_get_modes(struct drm_connector *connector)
{
	struct drm_device *dev = connector->dev;
	struct visfx_device *v = to_visfx(dev);
	const struct drm_edid *edid = NULL;
	int i, count = 0;

	/* Prefer the monitor's own EDID over DDC (VDP DDC register: KNOWN). */
	if (v->ddc.algo_data)
		edid = drm_edid_read_ddc(connector, &v->ddc);
	drm_edid_connector_update(connector, edid);
	if (edid) {
		count = drm_edid_connector_add_modes(connector);
		drm_edid_free(edid);
		if (count)
			return count;
	}

	/* Fallback: the fixed timings HP shipped for these boards. */
	for (i = 0; i < ARRAY_SIZE(visfx_timings); i++) {
		const struct visfx_timing *t = &visfx_timings[i];
		struct drm_display_mode *m;

		/*
		 * Skip stereo timings by default: HP gates them behind an opt-in
		 * ("EnableStereoVisuals") and warns they can overdrive a monitor.
		 * Expose them later behind a property. (design choice)
		 */
		if (t->stereo)
			continue;

		m = drm_mode_create(dev);
		if (!m)
			break;

		m->type = DRM_MODE_TYPE_DRIVER;
		m->clock = t->clock;
		m->hdisplay = t->hdisplay;
		m->hsync_start = t->hss;
		m->hsync_end = t->hse;
		m->htotal = t->htotal;
		m->vdisplay = t->vdisplay;
		m->vsync_start = t->vss;
		m->vsync_end = t->vse;
		m->vtotal = t->vtotal;
		/* GAP: real sync polarities. VESA DMT is +hsync +vsync for most
		 * of these; HP's config does not record them. */
		m->flags = DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC;

		drm_mode_set_name(m);
		if (t->clock == 135000 && t->hdisplay == 1280)
			m->type |= DRM_MODE_TYPE_PREFERRED;  /* HP default */

		drm_mode_probed_add(connector, m);
		count++;
	}

	/* The fixed list is the fallback HP itself used via MonitorTimingData
	 * when no EDID is available. */
	return count;
}
