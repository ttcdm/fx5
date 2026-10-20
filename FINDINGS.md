# HP VISUALIZE fx5/fx10 — Windows 2000 driver package: findings and Linux port roadmap

Status as of 2026-10-19. Each point is marked **Observed** (seen directly in the files or in Linux source), **Inferred**, or **Unknown**.

## 1. What the file is

| Item | Value |
|---|---|
| File | `fx_w2k_118b.exe` |
| SHA-256 | `53cb7916e5f70c8f1a099edb01988780be90ef9bf8266209a6e851aebe12d67f` |
| Format | WinZip self-extracting archive (Win16 NE stub + ZIP) — **Observed** |
| Contents | 21 files: HP VISUALIZE fx5/fx10 OpenGL driver for Windows 2000, v1.18 (March 2002) — **Observed** (README, WHATSNEW, INF) |

It is a set of display drivers, not an application. Nothing in it can be recompiled or run on Linux.

## 2. Hardware (from `HPFXLW2K.INF` and `README.TXT`)

- **HP 3D accelerator:** PCI `103C:100A`, subsystems `103C:10D4`–`10D8`, revision `0x80` (five board variants) — **Observed**
- **Matrox Millennium G200 on the same board:** PCI `102B:0521` (G200 AGP), subsystem `103C:100A`. The README says it "provides VGA functionality and cannot be used at the same time while using VisualizeFX graphics" — **Observed**
- **3D feature set** (`HPFXLRLS.W2K`): fixed-function OpenGL with 3D textures, depth textures/shadow, hardware occlusion and visibility queries (`GL_HP_occlusion`, `GL_HP_visibility_test`, `GL_HP_visibility_stats`), supersample anti-aliasing, overlays and stereo — **Observed**
- **Newest Windows it supports:** XP (added in 1.16). There is no later driver model — **Observed** (WHATSNEW)

## 3. Driver components (from the INF)

| Installed as | Source file | Role |
|---|---|---|
| `hpfxlmp.sys` | `HPFXLMP.W2K` | Kernel video miniport (PCI probe, mode setting, memory mapping) |
| `hpfxlgdi.dll` | `HPFXLGDI.W2K` | 2D GDI display driver |
| `hpfxlut.dll` | `HPFXLUT.W2K` | HP utilities interface |
| `hpfxldd.dll` | `HPFXLDD.W2K` | DirectDraw |
| `hpfxld3d.dll` | `HPFXLD3D.DLL` | Direct3D |
| `hpfxlicd.dll` | `HPFXLICD.W2K` | OpenGL client driver (948 KB, the largest component) |
| `hpfxlkds.dll` / `hpfxluds.dll` | `HPFXLKDS.W2K` / `HPFXLUDS.W2K` | OpenGL support libraries (entry point `fp14_init`) |
| `hpfxltab.dll` | `HPFXLTB.W2K` | Display Properties tab |
| `fx5diag.exe` | `FX5DIAG.W2K` | Board diagnostics (works through OpenGL and DAC tests) |
| `HpVisIni.exe` | `HPVISINI.EXE` | Startup helper that manages the per-application settings |
| — | `HPOGLGFX.DRV` | 3ds Max 3 viewport plug-in (not installed by the INF) |

The many `*_config` INF sections hold per-application OpenGL settings for Pro/E, CATIA, Maya, 3ds Max and others. They are compatibility switches only, not hardware programming.

## 4. Miniport (`HPFXLMP.W2K`, SHA-256 `e2bc9dad…7b3d`) — static analysis with GNU objdump

- **Observed:** Native NT driver, image base `0x10000`. Imports from `VIDEOPRT.SYS`, `HAL.dll` and `ntoskrnl.exe` only.
- **Observed:** DriverEntry registers find-adapter, initialize, request handler, reset, power, and child-descriptor (monitor) callbacks. **It registers no interrupt handler.**
- **Observed:** The PCI probe checks for vendor `103C` and device `100A`, reads the full PCI configuration space, and asks Windows for up to 6 address ranges. It also writes some PCI configuration registers.
- **Observed:** It allocates contiguous physical memory and maps a physical memory section, so buffers are shared with the hardware.
- **Unknown:** The register-by-register meaning of the hardware programming. This was not completed, and the deep decompiler (Hopper) is unusable on this Mac because Spotlight is disabled.

## 5. What Linux supports today

- **Observed:** Current mainline Linux `drivers/gpu/drm/mgag200/mgag200_drv.c` lists `PCI_VDEVICE(MATROX, 0x0521)` as `G200_AGP` and handles it with `mgag200_g200_device_create()`. The Linux kernel driver database (LKDDb) dates this support to kernel 5.10 and later.
- **Observed:** No Linux driver claims `103C:100A`. The pci.ids entry for it ("Visualize FX2") is flagged as reported invalid.
- **Unknown:** Whether the board's Matrox output actually reaches the monitor connector under Linux when the HP engine is not set up. Only a test on the real hardware can answer this.

## 6. Options, cheapest first

**A. Basic desktop display through the on-board Matrox G200** (likely low effort; untested)
1. Boot any current live Linux (kernel 5.10 or newer) on the machine with the card installed.
2. Run `lspci -nnk`. Look for `102b:0521` with `Kernel driver in use: mgag200`, and for `103c:100a` with no driver.
3. Run `dmesg | grep -i mgag200` and check for a working display.

What you get is 2D only: modesetting plus a software-rendered desktop (Mesa llvmpipe for OpenGL). The HP 3D engine stays unused.

**B. Keep the OpenGL hardware working**
The only route that exists today is Windows 2000/XP on the original hardware. Running Windows XP in a VM with the card passed through (VFIO) might work in principle — **Unknown**, untested.

**C. A real Linux 3D driver for the HP engine** (very large effort)
This needs a new kernel driver for display and memory management plus a Mesa userspace driver for OpenGL. Building them means documenting the hardware programming from these Windows drivers and testing every step on the real board. There is no datasheet and no existing community, so expect person-years of work, comparable in kind to the early Nouveau or etnaviv projects.

## 7. Open questions

- Does the Matrox G200 drive the board's monitor output on Linux (option A)?
- Which of the five subsystem IDs (`10D4`–`10D8`) is your board? `lspci -nn -v` shows it.
- Is this the x86 board, or the PA-RISC version that HP also shipped? The Windows driver implies x86.
- For option C: the hardware register map and command format are not recovered.

## 8. Chip family and public documentation (added later on 2026-10-19; internet sources + the file only)

### The driver targets HP's "Lego" chip family
- **Observed:** `HPFXLUT.W2K` version info: *"HP Lego Windows2000 Display Driver Utilities"*. The DirectDraw driver `HPFXLDD.W2K` also contains the string `Lego`.
- **Observed (PA-RISC docs):** FX5 and FX10 are the **Lego** family. FX2/FX4/FX6 are **Summit**. FX5/FX10 merge the texture and raster units into one ("RT"), and the Lego command processor is called "Heathrow".
- **Inferred:** the x86 fx5/fx10 in this package use the same Lego chip set as the PA-RISC FX5/FX10.

### Version info (all files 1.18 F1, 03/02/2002)
| File | HP's description |
|---|---|
| `HPFXLMP.W2K` | Windows 2000 Video Miniport Driver |
| `HPFXLGDI.W2K` | Windows2000 Display Driver |
| `HPFXLICD.W2K` | Display Driver and OGL Support |
| `HPFXLKDS.W2K` / `HPFXLUDS.W2K` | "KDSM" / "UDSM" (inferred: kernel-/user-mode device-specific module; matches the `_hpDsm_` exports) |
| `HPFXLDD.W2K` / `HPFXLD3D.DLL` | DirectDraw HAL / D3D HAL |
| `HPFXLUT.W2K` | **HP Lego** Display Driver Utilities |
| `FX5DIAG.W2K` | "GraphDiag" MFC application |
| `HPVISINI.EXE` | built as the NT4 version (1.18 F1 NT4) |

REA evidence for the miniport resource tree: `ev_ae1994c4…ecb59`.

### Interfaces each component exposes (exports)
- `HPFXLICD`: the standard Windows OpenGL client-driver set (`DrvCreateContext`, `DrvSetContext`, `DrvGetProcAddress`, `DrvValidateVersion`, layer-plane/overlay functions), plus HP-internal contexts `_hpOgl_*`, `_hpDlm_*` (display lists), `_hpDsm_*`, `_hpSum_context`, `_hpOcm_`, `_hpPcm_`, `_hpIgs_`, and the `EXT_texture_object` entry points.
- `HPFXLKDS` → `fp14_init`, `HPFXLDD` → `fp12_init_if`, `HPFXLD3D` → `d3dInitInterface`, `HPFXLUT` → `HpUtilsInitInterface`. All four import only `WIN32K.SYS`, so they run in kernel mode inside the display driver. The INF wires them up by these entry-point names.
- `HPFXLUDS` exports `_hpfxludsDSM_info` and imports `HPFXLICD`, so it is a user-mode plug-in for the OpenGL driver.

A Linux port replaces every one of these Windows interfaces: the miniport and kernel modules with a DRM/KMS kernel driver, and the OpenGL client driver with a Mesa driver.

### Public hardware documentation that already exists
1. **Linux PA-RISC wiki, "HP Visualize FX"**: card/codename table, STI IDs, BAR sizes, the **card address map taken from HP's `lego_address.h`** (control, privileged, 2D/3D buffered regions, framebuffers), the command-data (CD) packet header format, key status/control/interrupt registers, and the interrupt method: FX2/4/5/6/10 interrupt by **writing to a CPU address, not through a PCI INT# line**. That fits the Windows miniport registering no interrupt handler.
2. **NetBSD `summitfb` + `sys/dev/ic/summitreg.h` + `xf86-video-ngle`** (Michael Lorenz, 2024–2026, BSD licence): a working open-source framebuffer and 2D-acceleration driver for the **Summit** FX4. Values were recovered from the card ROM. Its `VISFX_STATUS` at `0x641400` matches the wiki's `UB_STATUS` `0x0064'1400`, so the two sources agree.
3. **HP's own Linux driver, `hpgraphics`** (2000–2002; for example `hpgraphics-1.1.0-2.i386.rpm`, Red Hat 6.1–7.2): closed X server and OpenGL plus a **GPL kernel part for the x86 AGP card**. Its source RPM shipped `lego_address.h`, `lego_hw.h` and `lego_hw_macros.h`. **Unknown** whether a copy survives; I could not reach archive.org or the HPE forums (HTTP 403).

### Caveats
- **PCI ID:** the PA-RISC docs say Lego and Summit cards report `103c:1008`. This package's INF uses `103c:100a`. **Unknown** whether the x86 board's BAR layout or address map differs from the PA-RISC one.
- The wiki says outright that "no documentation is available for the card". Most of its register meanings are reverse-engineered, and many bits are still marked unknown.
- `summitfb` targets Summit (FX4), not Lego. The 2D path may be similar, but the 3D/texture units differ.

### What this means for a Linux port
- **Inferred:** a Linux 2D/KMS driver for the HP engine could start from NetBSD `summitfb` plus the wiki's Lego address map. That is much less work than starting from these Windows binaries alone.
- **Highest-value missing item:** HP's `hpgraphics` GPL source with the Lego headers. To look for it yourself, search archive.org's Wayback Machine for old `ftp.hp.com` / `hp.com/workstations` Linux pages, and search old Red Hat or SuSE mirrors for "hpgraphics".

## 9. HP's own Linux package: `hpgraphics-1.5.13-1.i386.rpm` (supplied by you, 2026-10-19)

Static inspection only. Unpacked into `scratchpad/hpg_rpm/root/`; nothing was installed or run.

### Identity
- SHA-256 `ff9009d7dfb8c9b0bb2c5cfc40c0916aa73d2568fb0981f4db86c3eebea6ca1d`, 4,583,536 bytes, RPM v3 binary, i386 — **Observed**
- Header: vendor "Hewlett-Packard, Company", "(c) Hewlett-Packard Company 1996-2000", build host `vega.fct.hp.com` (inferred: HP Fort Collins, the same site as the Windows INF), URL `hp.com/workstations/products/linux` — **Observed**
- Install scripts: run `depmod`, add `grmd 5999/tcp` (HP graphics resource manager) to `/etc/services`, and run `FXconfig.sh` on removal. Nothing else — **Observed**
- **Unknown:** the package signature. No `rpm` tool or HP key was available, so "genuine HP" is inferred from the content, not cryptographically verified. It came from a third-party mirror.

### Contents
- `lib/modules/<kernel>/video/hpgfx.o`: prebuilt kernel modules for Red Hat 7.x kernels 2.4.2-2, 2.4.9-6 and 2.4.9-21, each UP and SMP. The 2.2.16-3 entries are only links.
- `usr/X11R6/hp/bin/Xhp`: HP's own X server, with HP screen modules (`sls.1`, `rw.1`, `utils.1`, …) and config files `X0devices`/`X0screens`.
- `usr/GL/hp/lib/*/libGL.so.1`, `libddvisfxgl.so.1`, `libddvmd.so.1`: HP OpenGL. `libddvisfxgl` is the device-dependent VISUALIZE FX layer.
- `usr/GL/hp/lib/glibc2.2.4/lib35acda30.1`: **named after STI graphics ID `35acda30`, which the PA-RISC wiki assigns to the FX5 (Lego)**. So the x86 and PA-RISC fx5 are the same graphics family — **Observed name; inferred meaning**
- HP OpenGL headers, including internal `glHpInt.h` and `glhooks.h`.

### `hpgfx.o`: the GPL kernel module (the wiki's "GPL x86-AGP driver")
- `license=GPL`, `author=HP Workstation Systems Lab, Ft. Collins, Colorado`, `description=HP VISUALIZE`. The embedded version string reads "hpgfx 2.4.9-21 kernel module: Version 1.5.12-1, Built Fri 21 Jun 2002". **Not stripped** — **Observed**
- It is a 2.4-era DRM (DRI 4.x template) driver. Source files: `hpgfx_drv.c`, `hpgfx_dma.c`, `hpgfx_mtrr.c`, plus the standard DRM core (`agpsupport.c`, `auth.c`, `bufs.c`, `context.c`, `dma.c`, `drawable.c`, `fops.c`, `init.c`, `ioctl.c`, `lists.c`, `lock.c`, `memory.c`, `proc.c`, `vm.c`, `Version.c`) — **Observed**
- HP-specific functions cover DMA and command buffers, not mode setting: `hpgfx_dma_initialize`, `setup_dma_engine`, `hpgfx_dma_write_to_host`, `hpgfx_dma_cd_read_from_host`, `createMarkerWriteCDPackets`, `hpgfx_cd_buf_info`, `hpgfx_regs_info`, `hpgfx_reset_uc`, `hpgfx_mark_phys_mem_wrcomb`, `hpgfx_init_ntdma`, `allocate_dma_buffers` — **Observed**
- Messages use the documented family names: "heathrow capability register" (Heathrow = Lego command processor) and "summit device isn't mapped yet" — **Observed**
- It uses Linux PCI config, ioremap, MTRR and AGP calls, and no interrupt registration. That fits the wiki's write-to-CPU-address interrupt scheme and the Windows miniport having no interrupt handler — **Observed imports; inferred link**
- **Inferred:** as the wiki says, the card's real initialisation and 2D/3D programming live in the closed X server and OpenGL libraries. The kernel module provides AGP, DMA and command-buffer plumbing.

### Closed X server and OpenGL libraries: not stripped
| File | Symbols (dynamic / total) |
|---|---|
| `libddvisfxgl.so.1` | 484 / 2,511 |
| `libddvmd.so.1` | 163 / 1,589 |
| `Xhp` | 2,330 / 4,164 |
| `sls.1` (screen module) | 212 / 466 |
- `libddvisfxgl` symbol groups: `_hpDSM_` (251), `_hpLegoFbm_` (15), `hpgfx_` (15), `_hpLegoDSM_` (6), `_hpLego_` (4). It contains "lego" 489 times — **Observed**
- **Inferred:** `_hpDSM_` is the same "device-specific module" as the Windows driver's `_hpDsm_` exports and "KDSM/UDSM" components. The Windows and Linux 3D drivers share one HP codebase, and the Linux build keeps its function names. For mapping the hardware, the Linux libraries are a much better starting point than the Windows DLLs.

### Licence note
`hpgfx.o` is GPL and was distributed by HP, so its source was published as the separate `hpgraphics_kernel-1.5.13-1` source RPM. The X server and OpenGL libraries are HP-proprietary ("All rights reserved").

### Does this RPM contain source code? **No** — **Observed**
- The RPM lead's package-type field is `0` (binary); a source RPM would be `1`.
- The payload has no `.c` files, no tarball, no Makefile, no kernel-module source and no Lego hardware headers.
- The only source-like files are 9 OpenGL headers: standard `gl.h`, `glu.h`, `glx*.h`, plus HP-proprietary `glHpInt.h` (2,815 lines) and `glhooks.h` (436 lines). Those two cover OpenGL dispatch, threading and debug hooks and contain no register or Lego/Summit content.
- The header names its own source package as `hpgraphics-1.5.13-1.src.rpm`, a different file. The download page's "contains source code" line must refer to the separate kernel-module-source item.

### Next highest-value item
The **`hpgraphics_kernel-1.5.13-1` source RPM** (on the same driverscollection page as "fx5/fx10 Linux kernel module source code v.1.5.13-1"). It should contain `hpgfx_drv.c`, `hpgfx_dma.c` and, per the PA-RISC wiki, HP's Lego hardware headers (`lego_address.h`, `lego_hw.h`, `lego_hw_macros.h`).

## 10. Functional map of HP's Linux driver stack (from its unstripped symbol names)

Source: symbol tables of the files in section 9, saved in `scratchpad/hpg_syms/*.nm`. Function names are **Observed**. What each one does is **Inferred** from its name unless stated otherwise. None of this code has been run.

### Layers
```
 OpenGL app
   │  libGL.so.1 (HP OpenGL, 5,427 symbols)
   ▼
 libddvisfxgl.so.1  ── fx5/fx10 device layer (1,384 functions)         libddvmd.so.1 (hpDSM + hpVMD; inferred software path)
   │  hpgfx_* user library (23 functions, same set also linked into the X module)
   ▼
 /dev  ── hpgfx.o kernel module (GPL, DRM 4.x)  ◄── also used by ──  Xhp X server + rw.1 screen module (441 functions, C++)
   ▼
 card (BARs mapped into user space; command-data (CD) buffers sent by DMA)
```

### Kernel module `hpgfx.o`: the interface user space relies on
`hpgfx_open/release/ioctl/mmap`, `hpgfx_addmap`, `hpgfx_lock/unlock`, `hpgfx_version`, `hpgfx_getLog`, `hpgfx_regs_info`, `hpgfx_reset_uc`, `hpgfx_mark_phys_mem_wrcomb` / `hpgfx_restore_mtrrs` (write-combining), and DMA: `hpgfx_dma_initialize`, `hpgfx_create_dma_manager`, `hpgfx_init_ntdma`, `hpgfx_dma_alloc_buff/free_buff/free_buffers`, `hpgfx_dma_buf_info`, `hpgfx_cd_buf_info`, `hpgfx_dma_write_to_host`, `hpgfx_dma_cd_read_from_host`, `hpgfx_dma_finish/finish_all`, `hpgfx_vm_dma_nopage`.
**Inferred:** the kernel side only maps the card, manages AGP/DMA memory and moves command buffers. All hardware programming happens in user space.

### Shared user library (`hpgfx_*`, identical in `libddvisfxgl` and `rw.1`)
DMA: `hpgfx_DmaInitialize`, `DmaAllocCDBuff`, `DmaMapBuff/UnmapBuff`, `DmaWriteToHost`, `DmaCDReadFromHost`, `DmaFinish/FinishAll`, `DmaClose`, `flushReadQueue`.
Hardware contexts: `hpgfx_legoCreateContext`, `legoDeleteContext`, `legoSaveHwContext`, `legoRestoreHwContext`, `legoSwitchContext`, `PrepareToContextSwap`, `UpdateLastContext`, `initSaveCommands`.
**Inferred:** the card has no per-process hardware contexts; user space saves and restores card state with command streams when switching between X and OpenGL clients.

### X server device module `rw.1` (C++, g++ 2.96, HP "Food" framework)
- **`LegoScreen`**: `openAndMapDevice`, `setupHardware`, `initMiscHardware`, `initPrecomputedRegisters`, `resetDevice`, `resetPlanes`, `initScreenDimensions`, `initCursor`/`refreshCursor`, `initColormap`, `initDHA` (direct hardware access), `initDLEs`, `initVMDriver`, `initVisualsAndDepths`, `enableVisuals`/`enableGrayscaleVisuals`/`enableNonGrayscaleVisuals`, `enableGlxAlpha`, `enableOGLAuxBuffers`, `checkStereoDimensions`, `blankOrUnblankScreen`, `disableITE` (HP console), `dmaFinish`, `hardwareFaultCheck`/`storeFaultConditions`/`validateFaultRecovery`, `closeScreen`
- **`LegoBufferManager`** (double buffering): `PrepareToRender`, `GetDisplayedBuffer`, `PolyBlt`, `PaintBackground`, `DbeValidateBuffer`/`MbxValidateBuffer` (X double-buffer extensions), swap/clear throttling.
- **`LegoImageAttribute` / `LegoOverlayAttribute` / `LegoFallbackAttribute`**: per-window display attributes for the image and overlay planes, stereo slots, and fast clear (`setupFastClearRead/Write`, `restoreFastClearRegs`, `makeFastClearPixelsReal`).
- **Plain C:** 2D operations (`legoCopyArea`, `legoCopyAreaOrPlane`, clip-plane load/paint/blit, `legoBuildClipList`, `legoCopyPixelsAAZSten_*`), colormap/LUT (`legoAllocateDefaultColormapEntries`, `legoCalcLutChecksum`), the framebuffer memory manager `fbm*`, PIO helpers `ReadBufferPIO`, `DumpBufferPIO`, `GetCSAddr`, `createHwContext`, and **`WAIT_BDRDY`, `WAIT_WPNE`**.
- **Cross-check:** `BDRdy` and `WPNE` are the field names the PA-RISC wiki gives for the Lego `UB_STATUS` register. HP's code and the public documentation independently agree — **Observed names; inferred link**

### OpenGL device layer `libddvisfxgl.so.1`
- **Generic device-specific module `hpDSM_*`** (also the name used by the Windows driver), by area: `gl` entry points (146), colour (87), dithering (41), per-fragment (41), stencil (23), spans (20), depth (15), texture (12+3, plus `texture3dOps`), pixel/pack/unpack, logic op, blending, fog, bitmap, raster, fill, select, **visibility** (5), scheduling, validation/update.
- **Lego-specific:** `_hpLegoDSM_spoolWords`, `_hpLegoDSMint_wrapCdBuffer` (inferred: the command-data packet builder), `_hpLegoDSM_read_reg`, `_hpLegoDSM_read_one_buffer`, `_hpLegoDSM_write_buffer_to_host`, `_hpLegoDSM_get_occlusion_result`, `_hpLegoDSM_update_occlusion_cull` (matches `GL_HP_occlusion` in the Windows release notes), `_hpLego_analyzeRasterizerSelectionState`, `lego_get_device_info`, `vapiTriggerReadback`/`vapiComplete`.
- **Video-memory manager `_hpLegoFbm_*`:** `init`, `allocateSurface`, `allocateAuxSurface`, `freeSurface`, `downloadTexels`, `isResident`, `setPriority`, `availableMemory`, `getOffsets`, `activate/deactivate`, `attach/close/delete`, `check`.

### How this maps onto a modern Linux driver (design, not observed)
| HP 2002 piece | Modern equivalent |
|---|---|
| `hpgfx.o` (map card, AGP/DMA, CD buffers, MTRR) | DRM kernel driver: BAR mapping, GEM/TTM buffer objects, a command-submission ioctl, PAT write-combining |
| `hpgfx_lego*HwContext` | kernel-side context save/restore during command submission |
| `LegoScreen::setupHardware/initMiscHardware/resetDevice/initScreenDimensions/initCursor/initColormap` | KMS: CRTC/mode setting, cursor plane, gamma/LUT |
| `LegoBufferManager` (swaps, throttling) | KMS page flip + vblank events |
| `LegoOverlayAttribute`, stereo slots | extra KMS planes / stereo modes (optional) |
| `fbm*` / `_hpLegoFbm_*` | GEM VRAM/texture memory manager |
| `hpDSM_*` + `_hpLegoDSM_*` (state → CD packets) | a Mesa Gallium driver emitting the same command packets |
| `rw.1` 2D ops | not needed: modern X/Wayland use modesetting + Glamor or software |

### What to analyse next, in order of value
1. `LegoScreen::openAndMapDevice`, `setupHardware`, `initMiscHardware`, `initPrecomputedRegisters`, `resetDevice`, `initScreenDimensions`: card bring-up and mode setting, the minimum for a KMS driver.
2. `_hpLegoDSMint_wrapCdBuffer`, `_hpLegoDSM_spoolWords`, `hpgfx_DmaWriteToHost`: the command-packet path. Compare with the wiki's CD packet header.
3. `hpgfx_legoSaveHwContext` / `RestoreHwContext`: which state has to survive a context switch.
4. `hpgfx.o` `hpgfx_init`/`hpgfx_setup`: which BARs and capability registers it touches. GPL code, so its reading carries no licence question.

## 11. Files

- Extracted package: `scratchpad/fx_extract/`
- Miniport headers: `scratchpad/fx_re/mp_headers.txt`
- Miniport disassembly with import names: `scratchpad/fx_re/mp_annot.txt`
- HP Linux package (copy + unpacked payload): `scratchpad/hpg_rpm/pkg.rpm`, `scratchpad/hpg_rpm/root/`
- Symbol tables of the Linux binaries: `scratchpad/hpg_syms/*.nm`

## Sources

- Linux `mgag200_drv.c` (mainline): https://github.com/torvalds/linux/blob/master/drivers/gpu/drm/mgag200/mgag200_drv.c
- LKDDb, `CONFIG_DRM_MGAG200`: https://cateee.net/lkddb/web-lkddb/DRM_MGAG200.html
- pci.ids, `103c:100a`: https://admin.pci-ids.ucw.cz/read/PC/103c/100a
- Linux on PA-RISC, "HP Visualize FX": https://parisc.docs.kernel.org/en/latest/visualize_fx.html
- OpenPA, HP Visualize fx for Intel PCs: https://www.openpa.net/intel_visualize-fx.html
- NetBSD `summitreg.h`: https://nxr.netbsd.org/xref/src/sys/dev/ic/summitreg.h
- NetBSD `summitfb.c`: https://ftp.riken.jp/NetBSD/NetBSD-current/src/sys/arch/hppa/dev/summitfb.c
- NetBSD `xf86-video-ngle` `summit_accel.c`: https://ftp.riken.jp/NetBSD/NetBSD-current/xsrc/external/mit/xf86-video-ngle/dist/src/summit_accel.c
- HPCwire, HP announces Linux support for fx5/fx10 (July 2000): https://www.hpcwire.com/2000/07/28/hp-announces-new-products-and-services/
- HPE community, hpgraphics-1.1.0-2 thread (blocked to automated reads): https://community.hpe.com/t5/operating-system-linux/error-in-hpgraphics-1-1-0-2-i386-rpm-fx10-opengl-implementation/m-p/2481877
