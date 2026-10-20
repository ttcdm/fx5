# visualizefx_drm — scaffold

A starting-point modern-Linux DRM/KMS driver for the HP VISUALIZE fx5/fx10
("Lego") workstation graphics card (PCI `103c:100a`). Read `../PORT_PLAN.md`
first.

## Be clear on what this is

This is a **scaffold**, not a working driver. It is honest about that in every
file. It gives a real port a correct skeleton and all the facts that could be
established without the hardware, so the remaining work is "fill the marked
gaps" rather than "start from nothing".

- **Builds cleanly on modern Linux.** Verified as an out-of-tree module against
  **Linux 6.12 LTS** (Debian trixie kernel headers `6.12.113+deb13-amd64`,
  x86_64, `make W=1` with no warnings or errors). See "Building" below. A
  different kernel version may need small API fixups.
- **No output.** Even though it compiles and would bind the PCI device, the
  CRTC programming is a stub, so nothing appears on screen.

## Building

```
make KDIR=/lib/modules/$(uname -r)/build
```

`KDIR` defaults to the running kernel's build tree; point it at any 6.x
kernel-headers tree. The port was build-verified in a container:

```
# Debian trixie = kernel 6.12 LTS
apt-get install -y build-essential linux-headers-amd64 bc libelf-dev
make -C /usr/src/linux-headers-*-amd64 M=$PWD modules
```

Output: `visualizefx.ko`, module alias `pci:v0000103Cd0000100A...`
(HP VISUALIZE fx engine), depends on `drm_kms_helper drm drm_shmem_helper
i2c-algo-bit`.

## What is real (KNOWN, from evidence)

- PCI match `103c:100a`, companion Matrox G200 `102b:0521` identified.
- **The full register map from HP's own source** (`visualizefx_regs.h`):
  address-space layout (control at +0, frame buffer at +16 MB), every
  `STATUS` bit, and the display registers (cursor, LUT, `CFG`, `VHAL`,
  `VTG`, `VS`, `DDC`, overlay, image plane, PLL offsets, board ID).
- Real register use in the driver:
  - `visfx_wait_idle()` polls `STATUS.WPNE`; `visfx_wait_vsync()` polls the
    `VS` state field (no interrupt needed).
  - **DDC/EDID** over the `DDC` register with Linux `i2c-algo-bit`; HP's fixed
    timing table is the fallback.
  - Mode set writes `VHAL` (active size), `VTG` (vertical timing) and `CFG`
    (sync polarity); enable/disable toggles `CFG.DAC_DISABLE`.
  - `mode_valid` rejects modes that don't fit the registers' field widths.
  - Probe maps control and frame buffer separately and logs the `R_ID`
    board ID and any `STATUS` fault.
- The complete **mode/timing table** from HP's Linux X config
  (`visualizefx_modes.c`): 18 standard + 4 stereo timings.

## What is missing (GAP — needs the real card)

HP's GPL source did not contain mode setting (that lived in HP's closed X
server), so these remain:
- `HTG` horizontal timing field layout.
- Dot-clock PLL field layout and reference frequency.
- Scanout base (`IBMAP*` meaning) and stride; plane `atomic_update`.
- `MPC` (DPMS), `ETG`/`STG` fields, cursor image format.
- Bring-up/reset sequence and how to read the installed memory size.
- Confirming BAR0 is the card window on the x86 board (`lspci -vv`).
- Everything 3D (the Mesa driver, Phase 3).

## How to make progress (cheapest first)

1. **Try `mgag200` first.** Boot a current Linux on the box; if the monitor
   output runs through the on-card Matrox G200, you already have a 2D desktop.
   `lspci -nnk` shows what bound.
2. **Trace the hardware.** With the card in a machine running HP's 2002 Linux
   stack or Windows driver, log the writes to `0x0080005C`–`0x008000B8` (`VHAL`
   through the PLLs) and `0x0080032x` (`IBMAP`) during a mode switch. That
   gives the `HTG`, PLL and scanout encodings to finish
   `visfx_crtc_mode_set_nofb`.

## Files

- `visualizefx_regs.h`  — register map from HP's source (offsets + fields)
- `visualizefx_drv.h`   — PCI identity, device struct, accessors
- `visualizefx_drv.c`   — PCI probe, DDC, DRM/KMS setup, CRTC (remaining GAPs marked)
- `visualizefx_modes.c` — EDID-first modes with HP's timing table as fallback
- `Kconfig`, `Makefile` — build glue

## Licence

GPL-2.0, to match the Linux DRM subsystem and HP's own GPL `hpgfx` module.
