// SPDX-License-Identifier: GPL-2.0
/*
 * visualizefx_drm — HP VISUALIZE fx5/fx10 ("Lego") DRM/KMS driver scaffold.
 *
 * Phase 1 target: bind the PCI device, map its BARs, register a DRM device
 * with one CRTC/encoder/connector and the known mode list, and expose a dumb
 * framebuffer. Mode programming itself is a GAP (see visfx_crtc_mode_set).
 *
 * This file is a scaffold: it is structured like a real modern-DRM driver so
 * the GAP functions can be filled in one at a time. It builds cleanly as an
 * out-of-tree module against Linux 6.12 LTS (see ../driver/README.md), but it
 * will not light a display until the GAP functions are done.
 */
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/aperture.h>

#include <drm/drm_drv.h>
#include <drm/drm_fbdev_shmem.h>
#include <drm/drm_gem_shmem_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_managed.h>
#include <drm/drm_modeset_helper_vtables.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_framebuffer.h>	/* struct drm_framebuffer (fb->width/height) */

#include <linux/bitfield.h>
#include <linux/iopoll.h>
#include <drm/drm_edid.h>

#include "visualizefx_drv.h"

/* ======================================================================== *
 *  Low-level helpers (register fields KNOWN from HP lego_hw.h)
 * ======================================================================== */

/* Wait until the write pipe has drained (STATUS.WPNE == 0). HP's X server
 * used an equivalent WAIT_WPNE helper. */
int visfx_wait_idle(struct visfx_device *v)
{
	u32 st;

	return readl_poll_timeout(v->mmio + VISFX_UB_STATUS, st,
				  !(st & VISFX_STATUS_WPNE), 1, 100000);
}

/* Wait for vertical sync using the VS state field (no interrupt needed). */
static int visfx_wait_vsync(struct visfx_device *v)
{
	u32 vs;

	return readl_poll_timeout(v->mmio + VISFX_VS, vs,
				  FIELD_GET(VISFX_VS_STATE, vs) ==
					VISFX_VS_STATE_SYNC,
				  10, 100000);
}

/* DDC: the VDP DDC register exposes SCL/SDA as two read pins and two drive
 * bits, which is exactly what i2c-algo-bit needs. Open-drain behaviour
 * (writing 1 releases the line) is INFER — confirm on hardware. */
static void visfx_ddc_set(struct visfx_device *v, u32 bit, int state)
{
	u32 val = visfx_rd(v, VISFX_DDC);

	val = state ? (val | bit) : (val & ~bit);
	visfx_wr(v, VISFX_DDC, val);
	visfx_wait_idle(v);
}

static void visfx_ddc_setscl(void *data, int state)
{
	visfx_ddc_set(data, VISFX_DDC_SCL_REG, state);
}

static void visfx_ddc_setsda(void *data, int state)
{
	visfx_ddc_set(data, VISFX_DDC_SDA_REG, state);
}

static int visfx_ddc_getscl(void *data)
{
	return !!(visfx_rd(data, VISFX_DDC) & VISFX_DDC_SCL_PIN);
}

static int visfx_ddc_getsda(void *data)
{
	return !!(visfx_rd(data, VISFX_DDC) & VISFX_DDC_SDA_PIN);
}

static int visfx_ddc_init(struct visfx_device *v)
{
	struct i2c_algo_bit_data *b = &v->ddc_bit;

	b->data = v;
	b->setsda = visfx_ddc_setsda;
	b->setscl = visfx_ddc_setscl;
	b->getsda = visfx_ddc_getsda;
	b->getscl = visfx_ddc_getscl;
	b->udelay = 10;		/* ~50 kHz, standard for DDC */
	b->timeout = usecs_to_jiffies(2200);

	v->ddc.owner = THIS_MODULE;
	v->ddc.dev.parent = &v->pdev->dev;
	v->ddc.algo_data = b;
	snprintf(v->ddc.name, sizeof(v->ddc.name), "visualizefx DDC");

	return i2c_bit_add_bus(&v->ddc);
}

/* ======================================================================== *
 *  CRTC / encoder / connector
 * ======================================================================== */

/* Reject modes whose values do not fit the KNOWN register field widths:
 * VHAL active counts are 12 bits, VTG porch/sync fields are 8 bits. */
static enum drm_mode_status visfx_crtc_mode_valid(struct drm_crtc *crtc,
				const struct drm_display_mode *m)
{
	if (m->hdisplay > 4095 || m->vdisplay > 4095)
		return MODE_BAD;
	if (m->vsync_start - m->vdisplay > 255 ||
	    m->vsync_end - m->vsync_start > 255 ||
	    m->vtotal - m->vsync_end > 255)
		return MODE_V_ILLEGAL;
	if (m->flags & DRM_MODE_FLAG_INTERLACE)
		return MODE_NO_INTERLACE;
	return MODE_OK;
}

static void visfx_crtc_mode_set_nofb(struct drm_crtc *crtc)
{
	struct visfx_device *v = to_visfx(crtc->dev);
	struct drm_display_mode *m = &crtc->state->adjusted_mode;
	u32 cfg;

	/* Active size (VHAL: KNOWN offset and field widths; INFER that ALC is
	 * lines and APC is pixels, from HP's register name). */
	visfx_wr(v, VISFX_VHAL,
		 FIELD_PREP(VISFX_VHAL_ALC, m->vdisplay) |
		 FIELD_PREP(VISFX_VHAL_APC, m->hdisplay));

	/* Vertical timing (VTG: KNOWN fields, in lines). */
	visfx_wr(v, VISFX_VTG,
		 FIELD_PREP(VISFX_VTG_VFP, m->vsync_start - m->vdisplay) |
		 FIELD_PREP(VISFX_VTG_VSYNC, m->vsync_end - m->vsync_start) |
		 FIELD_PREP(VISFX_VTG_VBP, m->vtotal - m->vsync_end));

	/* Sync polarity (CFG: KNOWN bits). */
	cfg = visfx_rd(v, VISFX_CFG);
	cfg &= ~(VISFX_CFG_POS_HSYNC | VISFX_CFG_POS_VSYNC | VISFX_CFG_DAC_DISABLE);
	if (m->flags & DRM_MODE_FLAG_PHSYNC)
		cfg |= VISFX_CFG_POS_HSYNC;
	if (m->flags & DRM_MODE_FLAG_PVSYNC)
		cfg |= VISFX_CFG_POS_VSYNC;
	visfx_wr(v, VISFX_CFG, cfg);

	/*
	 * GAP — still missing, so the display will not run yet:
	 *  - HTG (horizontal front porch / sync / back porch): offset KNOWN,
	 *    field layout absent from HP's GPL source. Horizontal values exceed
	 *    255 pixels at 1600x1200, so the units differ from VTG.
	 *  - Dot-clock PLL (PLL_DOT_CTL/CNT): offsets KNOWN, field layout and the
	 *    reference frequency absent from HP's GPL source.
	 *  - ETG / STG timing generators and scanout base (IBMAP*).
	 * HP's mode setting lived in the closed X server, not in the GPL kernel
	 * module, so these need tracing on real hardware.
	 */
	drm_dbg_kms(&v->drm,
		    "mode_set %dx%d@%d (pclk %d kHz): VHAL/VTG/CFG written; HTG and PLL are a GAP\n",
		    m->hdisplay, m->vdisplay, drm_mode_vrefresh(m), m->clock);

	visfx_wait_idle(v);
}

static void visfx_crtc_atomic_enable(struct drm_crtc *crtc,
				     struct drm_atomic_state *state)
{
	struct visfx_device *v = to_visfx(crtc->dev);

	/* Turn the DAC on (CFG.DAC_DISABLE: KNOWN bit). Monitor power (MPC)
	 * fields are a GAP, so DPMS states beyond on/off are not handled. */
	visfx_wr(v, VISFX_CFG, visfx_rd(v, VISFX_CFG) & ~VISFX_CFG_DAC_DISABLE);
	visfx_wait_idle(v);
}

static void visfx_crtc_atomic_disable(struct drm_crtc *crtc,
				      struct drm_atomic_state *state)
{
	struct visfx_device *v = to_visfx(crtc->dev);

	/* Change outputs during vertical sync to avoid a visible glitch. */
	visfx_wait_vsync(v);
	visfx_wr(v, VISFX_CFG, visfx_rd(v, VISFX_CFG) | VISFX_CFG_DAC_DISABLE);
	visfx_wait_idle(v);
}

static const struct drm_crtc_helper_funcs visfx_crtc_helper_funcs = {
	.mode_valid	= visfx_crtc_mode_valid,
	.mode_set_nofb	= visfx_crtc_mode_set_nofb,
	.atomic_enable	= visfx_crtc_atomic_enable,
	.atomic_disable	= visfx_crtc_atomic_disable,
};

static const struct drm_crtc_funcs visfx_crtc_funcs = {
	.reset			= drm_atomic_helper_crtc_reset,
	.destroy		= drm_crtc_cleanup,
	.set_config		= drm_atomic_helper_set_config,
	.page_flip		= drm_atomic_helper_page_flip,
	.atomic_duplicate_state	= drm_atomic_helper_crtc_duplicate_state,
	.atomic_destroy_state	= drm_atomic_helper_crtc_destroy_state,
};

static const struct drm_encoder_funcs visfx_encoder_funcs = {
	.destroy = drm_encoder_cleanup,
};

static const struct drm_connector_helper_funcs visfx_connector_helper_funcs = {
	.get_modes = visfx_connector_get_modes,
};

static const struct drm_connector_funcs visfx_connector_funcs = {
	.fill_modes		= drm_helper_probe_single_connector_modes,
	.destroy		= drm_connector_cleanup,
	.reset			= drm_atomic_helper_connector_reset,
	.atomic_duplicate_state	= drm_atomic_helper_connector_duplicate_state,
	.atomic_destroy_state	= drm_atomic_helper_connector_destroy_state,
};

/* ======================================================================== *
 *  Simple plane over the dumb framebuffer (SHMEM GEM for the scaffold).
 *  On real hardware the scanout buffer must live in VRAM; swap the GEM
 *  backend for GEM-VRAM/TTM in Phase 2. (STUB)
 * ======================================================================== */

static const u32 visfx_formats[] = {
	DRM_FORMAT_XRGB8888,	/* 24bpp TrueColor — the card's native depth */
};

/* A full-screen primary plane: no scaling, must cover the CRTC, pinned on. */
static int visfx_plane_atomic_check(struct drm_plane *plane,
				    struct drm_atomic_state *state)
{
	struct drm_plane_state *new_state =
		drm_atomic_get_new_plane_state(state, plane);
	struct drm_crtc_state *crtc_state;

	if (!new_state->crtc)
		return 0;

	crtc_state = drm_atomic_get_new_crtc_state(state, new_state->crtc);
	return drm_atomic_helper_check_plane_state(new_state, crtc_state,
						   DRM_PLANE_NO_SCALING,
						   DRM_PLANE_NO_SCALING,
						   false, false);
}

/*
 * The atomic helper calls funcs->atomic_update() unconditionally for an
 * enabled plane in drm_atomic_helper_commit_planes(); a NULL pointer there
 * oopses during the very first (fbdev) modeset. Programming the real scanout
 * base/stride is still a GAP: the scaffold backs the framebuffer with a SHMEM
 * GEM in system RAM (not card VRAM) and the IBMAP* scanout-register semantics
 * are unknown (see visualizefx_regs.h). So this is a logged near-no-op that
 * lets the commit complete; wire in the IBMAP/stride writes in Phase 2 once
 * VRAM scanout exists.
 */
static void visfx_plane_atomic_update(struct drm_plane *plane,
				      struct drm_atomic_state *state)
{
	struct visfx_device *v = to_visfx(plane->dev);
	struct drm_plane_state *new_state =
		drm_atomic_get_new_plane_state(state, plane);

	if (new_state && new_state->fb)
		drm_dbg_kms(&v->drm,
			    "plane update %ux%u (scanout base/stride: GAP)\n",
			    new_state->fb->width, new_state->fb->height);
}

static const struct drm_plane_helper_funcs visfx_plane_helper_funcs = {
	.atomic_check	= visfx_plane_atomic_check,
	.atomic_update	= visfx_plane_atomic_update,
};

static const struct drm_plane_funcs visfx_plane_funcs = {
	.update_plane		= drm_atomic_helper_update_plane,
	.disable_plane		= drm_atomic_helper_disable_plane,
	/* No .destroy: the plane is allocated with the devres-managed
	 * drmm_universal_plane_alloc(), which cleans it up automatically.
	 * Supplying .destroy trips a WARN in __drmm_universal_plane_alloc()
	 * (!funcs || funcs->destroy) and fails probe with -EINVAL. */
	.reset			= drm_atomic_helper_plane_reset,
	.atomic_duplicate_state	= drm_atomic_helper_plane_duplicate_state,
	.atomic_destroy_state	= drm_atomic_helper_plane_destroy_state,
};

static const struct drm_mode_config_funcs visfx_mode_config_funcs = {
	.fb_create	= drm_gem_fb_create,
	.atomic_check	= drm_atomic_helper_check,
	.atomic_commit	= drm_atomic_helper_commit,
};

/* ======================================================================== *
 *  DRM driver
 * ======================================================================== */

DEFINE_DRM_GEM_FOPS(visfx_fops);

static const struct drm_driver visfx_drm_driver = {
	.driver_features	= DRIVER_MODESET | DRIVER_ATOMIC | DRIVER_GEM,
	.fops			= &visfx_fops,
	.name			= "visualizefx",
	.desc			= "HP VISUALIZE fx5/fx10 (scaffold)",
	.major			= 0,
	.minor			= 0,	/* 0.0: scaffold, not functional */
	DRM_GEM_SHMEM_DRIVER_OPS,
};

static int visfx_kms_init(struct visfx_device *v)
{
	struct drm_device *drm = &v->drm;
	struct drm_plane *primary;
	int ret;

	ret = drmm_mode_config_init(drm);
	if (ret)
		return ret;

	drm->mode_config.funcs = &visfx_mode_config_funcs;
	drm->mode_config.min_width = 640;
	drm->mode_config.min_height = 480;
	drm->mode_config.max_width = 1920;	/* KNOWN: max listed timing */
	drm->mode_config.max_height = 1200;
	drm->mode_config.preferred_depth = 24;	/* card's native TrueColor depth */

	primary = drmm_universal_plane_alloc(drm, struct drm_plane, dev,
					     0, &visfx_plane_funcs,
					     visfx_formats,
					     ARRAY_SIZE(visfx_formats),
					     NULL, DRM_PLANE_TYPE_PRIMARY, NULL);
	if (IS_ERR(primary))
		return PTR_ERR(primary);
	drm_plane_helper_add(primary, &visfx_plane_helper_funcs);

	ret = drm_crtc_init_with_planes(drm, &v->crtc, primary, NULL,
					&visfx_crtc_funcs, NULL);
	if (ret)
		return ret;
	drm_crtc_helper_add(&v->crtc, &visfx_crtc_helper_funcs);

	v->encoder.possible_crtcs = drm_crtc_mask(&v->crtc);
	ret = drm_encoder_init(drm, &v->encoder, &visfx_encoder_funcs,
			       DRM_MODE_ENCODER_DAC, NULL);
	if (ret)
		return ret;

	ret = drm_connector_init(drm, &v->connector, &visfx_connector_funcs,
				 DRM_MODE_CONNECTOR_VGA);
	if (ret)
		return ret;
	drm_connector_helper_add(&v->connector, &visfx_connector_helper_funcs);

	ret = drm_connector_attach_encoder(&v->connector, &v->encoder);
	if (ret)
		return ret;

	drm_mode_config_reset(drm);
	return 0;
}

/* ======================================================================== *
 *  PCI probe / remove
 * ======================================================================== */

static int visfx_pci_probe(struct pci_dev *pdev,
			   const struct pci_device_id *ent)
{
	struct visfx_device *v;
	struct drm_device *drm;
	int ret;

	ret = pcim_enable_device(pdev);
	if (ret)
		return ret;

	v = devm_drm_dev_alloc(&pdev->dev, &visfx_drm_driver,
			       struct visfx_device, drm);
	if (IS_ERR(v))
		return PTR_ERR(v);
	drm = &v->drm;
	v->pdev = pdev;
	pci_set_drvdata(pdev, v);

	/*
	 * BAR mapping. HP's lego_address.h puts control space at +0 (16 MB) and
	 * the frame buffer window at +16 MB within one 32 MB-aligned window
	 * (KNOWN). openpa's lspci shows a 32 MB region 0 on the PA-RISC fx5.
	 * That BAR0 is this window on the x86 103c:100a board is the remaining
	 * assumption; check it with `lspci -vv` before trusting this.
	 */
	if (pci_resource_len(pdev, 0) < VISFX_FB_BASE + SZ_1M) {
		drm_err(drm, "BAR0 is %llu bytes, smaller than the expected window\n",
			(unsigned long long)pci_resource_len(pdev, 0));
		return -ENODEV;
	}
	v->mmio_base = pci_resource_start(pdev, 0);
	v->mmio_size = VISFX_FB_BASE;
	v->mmio = devm_ioremap(&pdev->dev, v->mmio_base, v->mmio_size);
	if (!v->mmio)
		return -ENOMEM;

	v->vram_base = v->mmio_base + VISFX_FB_BASE;
	/* Window size available in the BAR. The real installed memory size
	 * (HP's HpConfigInfo.fbSize) still has to be read from the board. */
	v->vram_size = min_t(resource_size_t,
			     pci_resource_len(pdev, 0) - VISFX_FB_BASE, SZ_16M);
	v->vram = devm_ioremap_wc(&pdev->dev, v->vram_base, v->vram_size);
	if (!v->vram)
		return -ENOMEM;

	pci_set_master(pdev);

	v->board_id = visfx_rd(v, VISFX_R_ID);
	drm_info(drm, "board id readback (R_ID) = 0x%08x\n", v->board_id);

	if (visfx_rd(v, VISFX_UB_STATUS) & VISFX_STATUS_FAULT)
		drm_warn(drm, "STATUS reports a fault at probe\n");

	/*
	 * GAP: hardware bring-up, mirroring HP LegoScreen order:
	 *   openAndMapDevice -> setupHardware -> initMiscHardware ->
	 *   initPrecomputedRegisters -> resetDevice -> initScreenDimensions.
	 * These sequences were in HP's closed X server, not the GPL module.
	 */

	ret = visfx_ddc_init(v);
	if (ret)
		drm_warn(drm, "DDC bus not registered (%d); using fixed modes\n", ret);

	ret = visfx_kms_init(v);
	if (ret)
		return ret;

	ret = drm_dev_register(drm, 0);
	if (ret)
		return ret;

	/*
	 * Set up the in-kernel fbdev client (Linux console). The 6.11 fbdev
	 * refactor removed drm_fbdev_generic_setup(); a SHMEM-backed driver now
	 * calls the memory-manager-specific helper directly.
	 *
	 * The argument is the preferred *bpp*, not the colour depth: the card's
	 * native TrueColor is 24-bit colour carried in a 32-bit pixel, which is
	 * DRM_FORMAT_XRGB8888 — the one format the primary plane advertises. Ask
	 * for 32 bpp so the fbdev client picks XRGB8888; passing 24 makes it pick
	 * the 3-byte RGB888 the plane does not support ("No compatible format
	 * found"). mode_config.preferred_depth stays 24 (that is the colour depth).
	 */
	drm_fbdev_shmem_setup(drm, 32);

	drm_warn(drm,
		 "SCAFFOLD: bound %04x:%04x but modeset is a stub; no output yet\n",
		 pdev->vendor, pdev->device);
	return 0;
}

static void visfx_pci_remove(struct pci_dev *pdev)
{
	struct visfx_device *v = pci_get_drvdata(pdev);

	drm_dev_unplug(&v->drm);
	drm_atomic_helper_shutdown(&v->drm);
	if (v->ddc.algo_data)
		i2c_del_adapter(&v->ddc);
}

/* KNOWN: the five fx5/fx10 board subsystem IDs are 103c:10d4..10d8; match on
 * the base device id and accept any subsystem. */
static const struct pci_device_id visfx_pci_ids[] = {
	{ PCI_DEVICE(PCIID_HP, PCIID_HP_VISUALIZE_FX) },
	{ }
};
MODULE_DEVICE_TABLE(pci, visfx_pci_ids);

static struct pci_driver visfx_pci_driver = {
	.name		= "visualizefx",
	.id_table	= visfx_pci_ids,
	.probe		= visfx_pci_probe,
	.remove		= visfx_pci_remove,
};
module_pci_driver(visfx_pci_driver);

MODULE_DESCRIPTION("HP VISUALIZE fx5/fx10 DRM scaffold");
MODULE_LICENSE("GPL");
