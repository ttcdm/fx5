/*
 * QEMU model of the HP VISUALIZE fx5/fx10 ("Lego") graphics engine.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * THIS IS A PROBE/MODESET TRACE HARNESS, NOT A GPU EMULATOR.
 *
 * It exists so the out-of-tree Linux DRM driver in fx5/driver/ can probe,
 * bind, read back the board id, and run its mode-set register writes inside a
 * VM — with no physical 103c:100a card, for which no emulator exists. It does
 * NOT render pixels, does NOT implement the 2D/3D command engine, and does NOT
 * drive a display. Reads return just-plausible values for exactly the registers
 * the driver's probe/modeset path touches; writes are stored and traced.
 *
 * Register offsets and field positions mirror fx5/driver/visualizefx_regs.h,
 * which was derived from HP's own GPL source (hpgraphics_kernel). Only the
 * handful of registers the driver actually reads during probe/modeset are
 * modelled; everything else reads as 0 and writes are logged generically.
 */
#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/log.h"
#include "hw/pci/pci_device.h"
#include "hw/qdev-properties.h"
#include "migration/vmstate.h"
#include "qom/object.h"
#include "trace.h"

#define TYPE_VISUALIZEFX "visualizefx"
OBJECT_DECLARE_SIMPLE_TYPE(VisualizeFXState, VISUALIZEFX)

/* PCI identity (KNOWN: HP INF + openpa; matches driver's PCIID_* macros). */
#define VISFX_PCI_VENDOR    0x103c      /* Hewlett-Packard                  */
#define VISFX_PCI_DEVICE    0x100a      /* VISUALIZE fx5/fx10 "Lego" engine */
#define VISFX_PCI_REVISION  0x80
#define VISFX_PCI_SUBSYS    0x10d4      /* one of the fx5 board subsystem ids */

/* BAR0 geometry (KNOWN: lego_address.h). One 32MB window: 16MB control space
 * at +0, frame-buffer window at +16MB. The driver maps the two halves
 * separately and requires len >= VISFX_FB_BASE + 1MB. */
#define VISFX_BAR_SIZE      (32 * MiB)
#define VISFX_FB_BASE       (16 * MiB)
#define VISFX_CTL_SIZE      (16 * MiB)
#define VISFX_FB_SIZE       (16 * MiB)

/* Control-space register offsets the driver touches (see visualizefx_regs.h). */
#define R_UB_STATUS         0x00641400u
#define R_CFG               0x00800044u
#define R_VHAL              0x0080005Cu
#define R_VTG               0x00800068u
#define R_VS                0x00800070u
#define R_DDC               0x008000D4u
#define R_ID                0x0082022Cu

/* STATUS bits (MSB-first, per HP's big-endian headers). */
#define STATUS_FAULT        (1u << 27)  /* must read 0: no fault at probe     */
#define STATUS_WPNE         (1u << 24)  /* must read 0: write pipe drained    */
#define STATUS_FIFO_MASK    0x00000fffu /* report the command FIFO as empty   */

/* VS state field = bits 13:12; 0 == VISFX_VS_STATE_SYNC. */
#define VS_STATE_SHIFT      12
#define VS_STATE_MASK       (0x3u << VS_STATE_SHIFT)
#define VS_COUNT_MASK       0x00000fffu

/* DDC pin/drive bits. */
#define DDC_SCL_PIN         (1u << 3)
#define DDC_SDA_PIN         (1u << 2)
#define DDC_SCL_REG         (1u << 1)
#define DDC_SDA_REG         (1u << 0)

/* VHAL field extraction (for trace readability). */
#define VHAL_ALC(v)         (((v) >> 16) & 0xfff)   /* active lines  */
#define VHAL_APC(v)         ((v) & 0xfff)           /* active pixels */

/* Synthetic board id returned for R_ID. The real value is a GAP — HP's GPL
 * source never documents it — so this is a clearly-non-hardware placeholder
 * the driver only logs. */
#define VISFX_BOARD_ID      0x0000000au

struct VisualizeFXState {
    PCIDevice parent_obj;

    MemoryRegion bar;       /* 32MB container registered as BAR0 */
    MemoryRegion ctl;       /* 16MB MMIO control space (I/O ops) */
    MemoryRegion fb;        /* 16MB frame-buffer window (plain RAM) */

    /* Modelled register state. */
    uint32_t cfg;
    uint32_t vhal;
    uint32_t vtg;
    uint32_t ddc;
    uint32_t vs_tick;       /* advanced each VS read so SYNC is always reached */
};

/*
 * Control-space reads. Only the registers the driver's probe/modeset path
 * reads are modelled; anything else reads back as 0.
 */
static uint64_t visfx_ctl_read(void *opaque, hwaddr addr, unsigned size)
{
    VisualizeFXState *s = opaque;
    uint32_t val = 0;

    switch (addr) {
    case R_ID:
        /* Board id readback — synthetic placeholder (see VISFX_BOARD_ID). */
        val = VISFX_BOARD_ID;
        break;

    case R_UB_STATUS:
        /* WPNE (bit24) clear => visfx_wait_idle() returns immediately.
         * FAULT (bit27) clear => probe does not warn. FIFO reported empty. */
        val = STATUS_FIFO_MASK;
        break;

    case R_VS:
        /* Report the vertical state cycling SYNC->BACK_PORCH->ACTIVE->FRONT
         * so visfx_wait_vsync() (waits for SYNC==0) always completes, with a
         * line counter that advances so the field looks alive. */
        val = ((s->vs_tick & 0x3u) << VS_STATE_SHIFT) |
              ((s->vs_tick >> 2) & VS_COUNT_MASK);
        s->vs_tick++;
        break;

    case R_DDC:
        /* Open-drain loopback with no I2C slave attached: each line sits at the
         * level the driver last drove it to (pull-ups make a released line 1).
         * i2c-algo-bit then runs cleanly but gets no ACK, so EDID read fails and
         * the driver falls back to its fixed mode table — an honest "no
         * monitor". SCL_PIN(bit3)<-SCL_REG(bit1), SDA_PIN(bit2)<-SDA_REG(bit0). */
        val = s->ddc & ~(DDC_SCL_PIN | DDC_SDA_PIN);
        if (s->ddc & DDC_SCL_REG) {
            val |= DDC_SCL_PIN;
        }
        if (s->ddc & DDC_SDA_REG) {
            val |= DDC_SDA_PIN;
        }
        break;

    case R_CFG:
        val = s->cfg;
        break;
    case R_VHAL:
        val = s->vhal;
        break;
    case R_VTG:
        val = s->vtg;
        break;
    default:
        val = 0;
        break;
    }

    trace_visualizefx_ctl_read(addr, val, size);
    return val;
}

/*
 * Control-space writes. The mode-set registers are stored and traced with a
 * decoded line; everything else is stored nowhere and logged generically.
 */
static void visfx_ctl_write(void *opaque, hwaddr addr, uint64_t val,
                            unsigned size)
{
    VisualizeFXState *s = opaque;

    switch (addr) {
    case R_CFG:
        s->cfg = val;
        trace_visualizefx_cfg_write(val);
        break;
    case R_VHAL:
        s->vhal = val;
        trace_visualizefx_vhal_write(val, VHAL_ALC(val), VHAL_APC(val));
        break;
    case R_VTG:
        s->vtg = val;
        trace_visualizefx_vtg_write(val);
        break;
    case R_DDC:
        s->ddc = val;
        trace_visualizefx_ddc_write(val);
        break;
    default:
        trace_visualizefx_ctl_write(addr, val, size);
        break;
    }
}

static const MemoryRegionOps visfx_ctl_ops = {
    .read = visfx_ctl_read,
    .write = visfx_ctl_write,
    .endianness = DEVICE_LITTLE_ENDIAN, /* the x86 103c:100a board is LE */
    .valid.min_access_size = 1,
    .valid.max_access_size = 4,
    .impl.min_access_size = 4,
    .impl.max_access_size = 4,
};

static void visfx_realize(PCIDevice *pci, Error **errp)
{
    VisualizeFXState *s = VISUALIZEFX(pci);

    /* Subsystem id: the driver accepts any subsystem, but a real fx5 id makes
     * lspci -nnvv look authentic. */
    pci_set_word(pci->config + PCI_SUBSYSTEM_VENDOR_ID, VISFX_PCI_VENDOR);
    pci_set_word(pci->config + PCI_SUBSYSTEM_ID, VISFX_PCI_SUBSYS);

    /* BAR0 = 32MB container: control MMIO at +0, frame-buffer RAM at +16MB. */
    memory_region_init(&s->bar, OBJECT(s), "visualizefx-bar", VISFX_BAR_SIZE);
    memory_region_init_io(&s->ctl, OBJECT(s), &visfx_ctl_ops, s,
                          "visualizefx-ctl", VISFX_CTL_SIZE);
    memory_region_init_ram(&s->fb, OBJECT(s), "visualizefx-fb",
                           VISFX_FB_SIZE, &error_fatal);
    memory_region_add_subregion(&s->bar, 0, &s->ctl);
    memory_region_add_subregion(&s->bar, VISFX_FB_BASE, &s->fb);

    /* Non-prefetchable: the control half of this BAR has read side effects
     * (e.g. the VS register advances its state field on every read), which a
     * prefetchable region must not have. The driver maps the control space
     * uncached regardless; the frame-buffer half it maps write-combining. */
    pci_register_bar(pci, 0, PCI_BASE_ADDRESS_MEM_TYPE_32, &s->bar);

    trace_visualizefx_realize(VISFX_PCI_VENDOR, VISFX_PCI_DEVICE);
}

static const VMStateDescription vmstate_visualizefx = {
    .name = TYPE_VISUALIZEFX,
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (VMStateField[]) {
        VMSTATE_PCI_DEVICE(parent_obj, VisualizeFXState),
        VMSTATE_UINT32(cfg, VisualizeFXState),
        VMSTATE_UINT32(vhal, VisualizeFXState),
        VMSTATE_UINT32(vtg, VisualizeFXState),
        VMSTATE_UINT32(ddc, VisualizeFXState),
        VMSTATE_UINT32(vs_tick, VisualizeFXState),
        VMSTATE_END_OF_LIST()
    }
};

static void visualizefx_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);

    k->realize = visfx_realize;
    k->vendor_id = VISFX_PCI_VENDOR;
    k->device_id = VISFX_PCI_DEVICE;
    k->revision = VISFX_PCI_REVISION;
    k->class_id = PCI_CLASS_DISPLAY_OTHER;

    dc->desc = "HP VISUALIZE fx5/fx10 probe/modeset harness (not a GPU)";
    dc->vmsd = &vmstate_visualizefx;
    set_bit(DEVICE_CATEGORY_DISPLAY, dc->categories);
}

static const TypeInfo visualizefx_info = {
    .name = TYPE_VISUALIZEFX,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(VisualizeFXState),
    .class_init = visualizefx_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { },
    },
};

static void visualizefx_register_types(void)
{
    type_register_static(&visualizefx_info);
}

type_init(visualizefx_register_types)
