# HP VISUALIZE fx5/fx10 — reverse-engineering and Linux port workspace

Everything from the investigation, gathered in one place on 2026-10-19.

## Start here
- `PORT_PLAN.md` — phased plan for a modern Linux driver, current status of
  every piece (known / partial / gap), and where HP's kernel source came from.
- `FINDINGS.md` — findings write-up: card identity, Windows and Linux driver
  structure, public documentation, functional map of HP's Linux stack.

## Folders
| Folder | Contents |
|---|---|
| `originals/` | The two files supplied: `fx_w2k_118b.exe` (Windows 2000 driver, v1.18) and `hpgraphics-1.5.13-1.i386.rpm` (HP's Linux binary package) |
| `hp_kernel_source/` | `hpgraphics_kernel-1.5.13-1.src.rpm` from the Internet Archive, plus `rpm/` (unpacked) and `src/` (HP's source tree: `hpgfx_drv.c`, `hpgfx_dma.c`, `headers/lego_address.h`, `headers/lego_hw.h`, …) |
| `driver/` | `visualizefx_drm` — Linux DRM/KMS driver scaffold (own git repo). Not compiled; mode setting incomplete. See `driver/README.md` |
| `windows_package/` | The Windows driver package, unpacked (INF, DLLs, docs) |
| `linux_package/` | HP's Linux binary package: `pkg.rpm`, `root/` (unpacked files), `symbols/` (symbol tables) |
| `notes/` | Display-register extracts from HP's headers; HP's Word document as text |
| `disassembly/` | Annotated listings of the Windows miniport and kernel module |
| `scripts/` | Small helper scripts used during analysis |

## File hashes (SHA-256)
- `originals/fx_w2k_118b.exe` — `53cb7916e5f70c8f1a099edb01988780be90ef9bf8266209a6e851aebe12d67f`
- `originals/hpgraphics-1.5.13-1.i386.rpm` — `ff9009d7dfb8c9b0bb2c5cfc40c0916aa73d2568fb0981f4db86c3eebea6ca1d`
- `hp_kernel_source/hpgraphics_kernel-1.5.13-1.src.rpm` — `b5a35295d448a722cd94b9a7a13f9e88d6716924fb05871d857264ee9a1a31cc`

## Licences to keep in mind
- HP's `hpgfx_drv.c`, `hpgfx_dma.c`, `hpgfx_mtrr.c`: GPL v2.
- HP's `lego_address.h`, `lego_hw.h`: marked "All rights reserved".
- The Windows and Linux binaries and HP's documents: HP proprietary.

## Known corrections to FINDINGS.md
- The kernel 2.2.16-3 modules in the Linux package are real files under
  `/usr/X11R6/hp/modules/`, not just links.
- The binary package is labelled 1.5.13-1 but most binaries inside are stamped
  1.5.12-1 (built 21 June 2002); the kernel 2.4.2-2 modules are an older
  v1.2.3 (May 2001).
