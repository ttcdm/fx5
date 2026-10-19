/* SPDX-License-Identifier: GPL-2.0 */
/*
 * visualizefx_drm — modern Linux DRM/KMS driver scaffold for the
 * HP VISUALIZE fx5/fx10 ("Lego") workstation graphics accelerator.
 *
 * STATUS: scaffold. Written against the DRM API shape of Linux 6.x but it is
 * NOT build-verified here and will NOT drive hardware until the GAP items are
 * filled. Every value taken from evidence is marked KNOWN; every placeholder
 * is marked GAP with where the real value must come from.
 *
 * Evidence sources (all either the owner's files or public references):
 *   - HP VISUALIZE fx driver packages (owner-supplied)
 *   - Linux on PA-RISC "HP Visualize FX" wiki (HP lego_address.h derived)
 *   - NetBSD summitfb / summitreg.h (sibling "Summit" chip, BSD-licensed)
 */
#ifndef __VISUALIZEFX_DRV_H__
#define __VISUALIZEFX_DRV_H__

#include <linux/bits.h>
#include <linux/io.h>
#include <linux/pci.h>
#include <linux/types.h>
#include <drm/drm_device.h>
#include <drm/drm_crtc.h>
#include <drm/drm_encoder.h>
#include <drm/drm_connector.h>

/* ---- PCI identity (KNOWN: HP INF + openpa) ------------------------------ */
#define PCIID_HP			0x103c
#define PCIID_HP_VISUALIZE_FX		0x100a	/* fx5 / fx10 "Lego" engine */
/* Companion 2D/VGA core on the same board (KNOWN: HP README, pci.ids).
 * Already driven by the mainline mgag200 driver; not claimed by this driver. */
#define PCIID_MATROX			0x102b
#define PCIID_MATROX_G200_AGP		0x0521

/* Full register map (offsets, STATUS/VDP/DDC fields, CD packet header) from
 * HP's GPL hpgraphics_kernel source — see visualizefx_regs.h. */
#include "visualizefx_regs.h"
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>

/* ---- driver-private device ---------------------------------------------- */
struct visfx_device {
	struct drm_device	drm;
	struct pci_dev		*pdev;

	/* Card window: control space at +0, frame buffer at +16 MB (KNOWN from
	 * lego_address.h). That BAR0 exposes this window is still to be
	 * confirmed with lspci on the real board. */
	void __iomem		*mmio;		/* control space (16 MB) */
	resource_size_t		mmio_base;
	resource_size_t		mmio_size;
	void __iomem		*vram;		/* frame buffer window   */
	resource_size_t		vram_base;
	resource_size_t		vram_size;
	u32			board_id;	/* R_ID readback         */

	/* DDC: bit-banged I2C over the VDP DDC register (KNOWN fields). */
	struct i2c_adapter	ddc;
	struct i2c_algo_bit_data ddc_bit;

	/* KMS objects (single head for Phase 1). */
	struct drm_crtc		crtc;
	struct drm_encoder	encoder;
	struct drm_connector	connector;
};

static inline struct visfx_device *to_visfx(struct drm_device *ddev)
{
	return container_of(ddev, struct visfx_device, drm);
}

/* Register accessors. VDP registers are only reachable through the 2D
 * buffered region, so writes queue in the write pipe; callers that need the
 * write to have landed call visfx_wait_idle() (STATUS.WPNE clear). */
static inline u32 visfx_rd(struct visfx_device *v, unsigned long off)
{
	return readl(v->mmio + off);
}
static inline void visfx_wr(struct visfx_device *v, unsigned long off, u32 val)
{
	writel(val, v->mmio + off);
}

/* visualizefx_drv.c */
int visfx_wait_idle(struct visfx_device *v);

/* visualizefx_modes.c */
int visfx_connector_get_modes(struct drm_connector *connector);

#endif /* __VISUALIZEFX_DRV_H__ */
