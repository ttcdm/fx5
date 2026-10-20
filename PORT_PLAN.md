# Porting the HP VISUALIZE fx5/fx10 to modern Linux — plan and status

This is the engineering plan plus an honest status of each piece. It is paired
with a driver scaffold under `visualizefx_drm/`. Nothing here has run on the
real hardware; items are marked **KNOWN** (established from the files or public
source), **STUB** (scaffolded, not real yet), or **GAP** (not recovered; needs
hardware or HP's GPL source).

## 0. What "port" honestly means here

There is no way to recompile the Windows or Linux 2002 drivers for a modern
kernel. A port means writing two new pieces:

1. A **DRM/KMS kernel driver** — mode setting, memory management, command
   submission.
2. A **Mesa Gallium driver** — OpenGL, translating GL state into the card's
   command packets.

Phase 1 (a working 2D/modeset display) is achievable by a determined person
with the card. Phase 2 (accelerated OpenGL) is a multi-person-year effort
comparable to early Nouveau/etnaviv. This plan front-loads the achievable part.

## 1. Target hardware (KNOWN)

- HP 3D engine: PCI `103C:100A`, subsystems `103C:10D4`–`10D8`, rev `0x80`.
  HP internal family name "Lego" (fx5/fx10). Sibling "Summit" = fx2/4/6.
- On-board Matrox G200 (`102B:0521`) provides VGA/2D and the physical output.
  "The Matrox device is part of the VisualizeFX card. It provides VGA
  functionality and cannot be used at the same time while using VisualizeFX
  graphics." (HP README)
- The 3D engine raises no PCI interrupt. It signals by DMA-writing marker
  values to host memory that software polls, and (per the PA-RISC wiki) can
  interrupt by writing to a CPU address. The Windows miniport registers no ISR;
  the GPL `hpgfx.o` registers none either. (KNOWN, cross-checked)
- Framebuffer/engine BAR is large (32 MB region 0 on fx5 per openpa lspci);
  expansion ROM present. Exact BAR map for `103C:100A` on x86: **GAP**.

## 2. Register and command knowledge

HP's kernel source package `hpgraphics_kernel-1.5.13-1.src.rpm` was recovered
from the Internet Archive (see §6). Updated status:

| Area | Status | Source |
|---|---|---|
| Card address map (control, 2D/3D buffered, FB at +16 MB) | KNOWN | HP `lego_address.h` |
| All 4,060 register offsets, by hardware block | KNOWN | HP `lego_address.h` |
| `STATUS` 0x641400, every bit position | KNOWN | HP `lego_hw.h` (explicit `<31:30>` comments) |
| Bit order of HP's field structures (MSB first) | KNOWN | confirmed by the `STATUS` and CD-buffer comments |
| Command-data (CD) packet header; CD buffer control | KNOWN | wiki + HP `lego_hw.h` |
| Display registers: cursor, LUT, CFG, VHAL, VTG, VS, DDC, overlay, image plane, PLL offsets | KNOWN offsets | HP `lego_address.h` |
| Field layouts: CFG (sync polarity, DAC off), DDC (I²C pins), VS (line count + state), VTG, VHAL, SCR | KNOWN | HP `lego_hw.h` |
| DMA engine, CD buffers, marker completion, MTRR | KNOWN (GPL code) | HP `hpgfx_dma.c`, `hpgfx_mtrr.c` |
| Board config: Durango/Silverton/Super, RT and B7 counts, SDR/DDR, FB size | KNOWN fields | HP `hp_config.h` (how it's read: GAP) |
| HTG (horizontal timing) field layout | GAP | not in HP's source |
| PLL field layout and reference clock | GAP | not in HP's source |
| MPC (DPMS), ETG/STG fields, scanout base (IBMAP) meaning | GAP | names known, fields not |
| Mode-set / bring-up sequence | GAP | lived in HP's closed X server, not the GPL module |
| 3D state | KNOWN offsets, semantics GAP | `lego_address.h` + `lego_hw.h` structs |

What's left for a first picture is now narrow: the HTG layout, the dot-clock PLL
encoding, the scanout base, and the bring-up order. All need tracing on the
real card (log what HP's X server writes to `0x0080005C`–`0x008000B8` during a
mode switch).

## 3. Display modes (KNOWN — full data, in the scaffold)

HP's Linux `X0screens` ships the exact timings the card is known to drive.
These are transcribed verbatim into `visualizefx_drm/visualizefx_modes.c`.
Format: `hdisp,hss,hse,htotal, vdisp,vss,vse,vtotal, pixclock(kHz)`.

Standard VESA: 640x480@75/85, 800x600@60/75/85, 1024x768@60/75/85,
1280x1024@60/75/85, 1600x1200@60/75/85.
Non-standard: 1600x1024@76, 1920x1200@50/60/72.
Stereo: 800x600@120, 1024x768@120, 1280x1024@100/110.

## 4. Phased plan

### Phase 0 — zero-RE fallback (do this first)
Boot current Linux (kernel ≥5.10) with the card installed. The Matrox G200 is
already supported by the mainline `mgag200` DRM driver. If the board routes its
monitor output through the G200 at the console, you get a 2D desktop with
software GL (llvmpipe) and **no driver work at all**. Verify with `lspci -nnk`.
This is the realistic "use it on modern Linux today" answer. (GAP: whether the
output is wired that way — needs the hardware.)

### Phase 1 — native KMS modeset driver (the real port, achievable)
Goal: the HP engine lights the monitor at the modes in §3 under KMS.
1. PCI driver binds `103C:100A`; map BARs; read PCI config. (scaffold: STUB map)
2. Bring-up sequence mirroring HP `LegoScreen`: `openAndMapDevice`,
   `setupHardware`, `initMiscHardware`, `initPrecomputedRegisters`,
   `resetDevice`, `initScreenDimensions`. (GAP: register writes)
3. One `drm_crtc`/`encoder`/`connector`; feed the §3 mode table.
4. A dumb framebuffer in VRAM + `fbdev` emulation → Linux console.
5. Fill in CRTC/PLL programming from HP GPL source or hardware tracing. (GAP)

Exit criteria: `modetest` sets 1280x1024 and shows a test pattern.

### Phase 2 — memory management + command submission
1. GEM + TTM (or GEM-VRAM) buffer objects over the framebuffer aperture.
2. A command-submit ioctl that validates and DMAs CD buffers, reusing the
   marker/poll completion from `hpgfx.o`. (PARTIAL: GPL source completes it)
3. Port the `fbm`/`_hpLegoFbm_` VRAM manager design for textures/surfaces.

Exit criteria: userspace can allocate a BO, submit a clear packet, see it land.

### Phase 3 — Mesa Gallium driver (the long pole)
1. New Gallium driver `visfx`. Map Gallium state to the `hpDSM_*`/`_hpLegoDSM_`
   command packets (state groups are enumerated in the findings).
2. Start with clears, solid tris, then textures, then the HP extensions
   (occlusion/visibility query → `GL_ARB_occlusion_query`).
Exit criteria: `glxgears`, then `piglit` subsets.

### Phase 4 — polish
Cursor, gamma LUT, overlay plane, stereo, DPMS, multi-head (SLS).

## 5. Risk/ROI

- Phase 0: hours. Phase 1: weeks–months with hardware + GPL source. Phase 2:
  months. Phase 3: the multi-year item. Phase 4: optional.
- Highest ROI: Phase 0 (maybe already works), then getting HP's GPL source,
  then Phase 1.
- Hard dependency throughout: **the physical card**. No GPU driver is finishable
  without it; every register write must be tried on real silicon.

## 6. HP's kernel source package (recovered)

- **Where:** Internet Archive capture of
  `hyper-linux.org/HP-HOWTO/mirror/Software/hpgraphics_kernel-1.5.13-1.src.rpm`,
  captured 2003-09-10 (9 identical captures 2003–2013; the live site is gone).
- **Integrity:** 169,856 bytes. SHA-1 (base32) `VBNX7WC2QYYX3ZMBW5WGHR27SVSRV4XE`
  matches the archive's recorded digest. SHA-256
  `b5a35295d448a722cd94b9a7a13f9e88d6716924fb05871d857264ee9a1a31cc`.
- **Contents:** `hpgraphics_kernel-1.5.13-1.tgz` (27 Jun 2002) and the build
  spec. The tarball holds `hpgfx_drv.c`, `hpgfx_dma.c`, `hpgfx_mtrr.c`, the
  XFree86 4.x DRM core, and `headers/`: `lego_address.h` (413 KB, 4,060
  registers), `lego_hw.h`, `lego_hw_macros.h`, `hp_config.h`, `bitfield.h`,
  `hpgfx_shared_dma.h`, `hpgfx_utils.c/h`, `sys/framebuf.h`.
- **Licences as stated in the files:**
  - `hpgfx_drv.c`, `hpgfx_dma.c`, `hpgfx_mtrr.c`: GPL v2, © 2000 Hewlett-Packard.
  - DRM core and `hpgfx_utils.c`: Precision Insight X-style licence.
  - `lego_address.h`, `lego_hw.h`: "(c) Hewlett-Packard … All rights reserved",
    with no GPL grant in the file, though shipped as the GPL module's source.
    The scaffold therefore defines its own names for the register offsets
    (facts) rather than copying these headers. If you hold the rights, as you
    say, you can decide to include them directly.
- **What it did not contain:** any mode-setting code. The GPL kernel module
  never touches the display registers; HP's mode setting was in the closed X
  server. So the PLL encoding and horizontal-timing layout are still open.
