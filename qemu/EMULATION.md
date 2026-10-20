# Emulating the HP VISUALIZE fx5/fx10 in QEMU — a probe/modeset harness

There is no QEMU model for the HP VISUALIZE fx5/fx10 ("Lego") engine
(`103c:100a`), and no emulator for it anywhere — QEMU's PA-RISC machine only
models HP's unrelated "Artist" framebuffer, and the companion Matrox G200 is a
separate chip. The real card is unavailable. So the DRM driver in
[`../driver/`](../driver) has never actually run.

This directory adds **just enough of the card to QEMU for the driver's probe and
mode-set logic to execute** inside an ordinary x86 VM. It is explicitly:

- **A probe/mode-set *trace* harness, not a GPU emulator.** It renders no
  pixels, implements none of the 2D/3D command engine, and lights no display.
- Modelled only for the registers the driver's probe + mode-set path actually
  touches. Everything else reads back as `0`; unknown writes are logged
  generically.

The success bar is: **the driver binds `103c:100a`, logs its `R_ID` readback,
and its `VHAL`/`VTG`/`CFG` mode-set writes are observable in QEMU's trace.**

## What the device models

`visualizefx.c` is a `TYPE_PCI_DEVICE` (vendor `0x103c`, device `0x100a`, rev
`0x80`, subsystem `103c:10d4`) exposing one **32 MB BAR0** = 16 MB MMIO control
space at `+0` + a 16 MB frame-buffer RAM window at `+16 MB` (the layout from
HP's `lego_address.h`, mirrored in `../driver/visualizefx_regs.h`). The read /
write handlers satisfy exactly what the driver needs:

| Register        | Offset       | Behaviour |
|-----------------|--------------|-----------|
| `R_ID`          | `0x0082022C` | returns a synthetic board id `0x0000000a` (real value is a GAP; the driver only logs it) |
| `UB_STATUS`     | `0x00641400` | `WPNE` (bit 24) **clear** so `visfx_wait_idle()` returns 0; `FAULT` (bit 27) clear; FIFO reported empty |
| `VS`            | `0x00800070` | state field (bits 13:12) cycles `SYNC→…`, so `visfx_wait_vsync()` always reaches `SYNC`; line counter advances |
| `DDC`           | `0x008000D4` | open-drain loopback with no I2C slave — the bit-bang bus runs cleanly, gets no EDID, driver falls back to its fixed mode table (an honest "no monitor") |
| `CFG`/`VHAL`/`VTG` | `0x00800044` / `…5C` / `…68` | stored and **traced** (`VHAL` is decoded to active lines × pixels) |

The driver's `drm_fbdev_shmem_setup()` runs an **initial mode-set during
`insmod`** (its unknown-status connector is treated as enabled, so fbcon commits
a mode to the preferred 1280×1024 timing). That is what drives the observable
`VHAL`/`VTG`/`CFG` writes — no `modetest` required.

## Files

| File | Purpose |
|------|---------|
| `visualizefx.c` | the QEMU device model |
| `trace-events.snippet`, `Kconfig.snippet`, `meson.snippet` | build glue appended into `hw/display/` |
| `integrate.sh` | copies the `.c` and appends the glue into a QEMU tree (idempotent) |
| `Dockerfile` | Debian trixie (= Linux 6.12 LTS) build/prove environment |
| `build-and-prove.sh` | in-container: build QEMU, build the `.ko`, make an initramfs, boot the VM, collect logs |
| `run.sh` | host (macOS) entry point: start colima, build the image, run the above |
| `out/` | generated: `serial.log`, `qemu-trace.log`, `initramfs.cpio.gz` |

## How to run it

One command from the repo root (macOS host with colima + docker installed):

```bash
./qemu/run.sh
```

> This **starts colima if it is stopped** (requesting 4 CPU / 8 GB / 60 GB).
> The box had colima installed but not running; `run.sh` does not otherwise
> touch your colima config. To stop it again afterwards: `colima stop`.

### The exact build commands (what `build-and-prove.sh` runs in the container)

```bash
# 1. QEMU with the device
git clone --depth 1 --branch v9.1.2 https://gitlab.com/qemu-project/qemu.git /opt/qemu
/work/qemu/integrate.sh /opt/qemu            # hw/display/visualizefx.c + glue
cd /opt/qemu
./configure --target-list=x86_64-softmmu --enable-trace-backends=log \
            --disable-docs --disable-werror --disable-plugins
make -j"$(nproc)"
/opt/qemu/build/qemu-system-x86_64 -device help | grep visualizefx   # confirms it's in

# 2. the driver, against the installed 6.12 headers
make -C /work/driver KDIR=/lib/modules/$(ls /lib/modules)/build

# 3 + 4. initramfs + boot (see the script); the boot invocation is:
qemu-system-x86_64 -M pc -accel tcg -m 512 \
    -kernel /boot/vmlinuz-$KVER -initrd out/initramfs.cpio.gz \
    -append "console=ttyS0 panic=-1 loglevel=7" \
    -device visualizefx \
    -trace enable=visualizefx_* -D out/qemu-trace.log \
    -display none -vga none -monitor none \
    -serial file:out/serial.log -no-reboot
```

To re-run just the device in an already-built QEMU with a kernel/initramfs of
your own, the only new flag is `-device visualizefx` (optionally
`-trace enable=visualizefx_*`).

## Verified output

The excerpts below are the **verbatim** captured output from a clean end-to-end
run (`out/serial.log` and `out/qemu-trace.log`); kernel timestamps will vary
between runs but the values do not.

`out/serial.log` — the device enumerates, the driver binds, reads back the
board id, creates the console fb and prints its scaffold warning:

```
[    5.671385] pci 0000:00:03.0: [103c:100a] type 00 class 0x038000 conventional PCI endpoint
[    5.694739] pci 0000:00:03.0: BAR 0 [mem 0xfc000000-0xfdffffff pref]
...
[   24.980553] visualizefx 0000:00:03.0: [drm] board id readback (R_ID) = 0x0000000a
[   25.081115] [drm] Initialized visualizefx 0.0.0 for 0000:00:03.0 on minor 0
[   25.857366] visualizefx 0000:00:03.0: [drm] fb0: visualizefxdrmf frame buffer device
[   25.940336] visualizefx 0000:00:03.0: [drm] SCAFFOLD: bound 103c:100a but modeset is a stub; no output yet
```

with `0000:00:03.0  vendor=0x103c device=0x100a driver=visualizefx` in the PCI
listing and `/dev/dri/card0` present in the guest (`crw------- 226, 0 card0`).
insmod returns success — no stack trace, no "Killed".

(The driver's `mode_set …` and `plane update …` lines are `drm_dbg_kms` debug
prints; the harness boots with `loglevel=7`, which suppresses `KERN_DEBUG`, so
they do not appear on the serial console. The mode-set is proven by the device
trace below rather than by a debug log line.)

`out/qemu-trace.log` — the register reads and the mode-set writes actually
reaching the device (one `CFG` write from `mode_set_nofb`, a second from
`atomic_enable`'s DAC-on):

```
visualizefx_realize HP VISUALIZE fx: realize vendor 0x103c device 0x100a
visualizefx_ctl_read ctl read  off 0x82022c = 0x0000000a size 4        # R_ID
visualizefx_ctl_read ctl read  off 0x641400 = 0x00000fff size 4        # STATUS (WPNE clear)
... (hundreds of DDC bit-bang reads/writes at off 0x8000d4 — the EDID probe) ...
visualizefx_vhal_write VHAL write 0x04000500 active 1024 lines x 1280 pixels
visualizefx_vtg_write VTG  write 0x00260301 (vertical timing)
visualizefx_cfg_write CFG  write 0x000c0000 (sync polarity / DAC enable)
```

Those values decode exactly to the preferred 1280×1024@75 mode:
- `VHAL 0x04000500` → ALC (bits 27:16) `0x400` = 1024 active lines, APC (bits 11:0) `0x500` = 1280 active pixels.
- `VTG 0x00260301` → VBP `0x26` = 38, VSYNC `0x03` = 3, VFP `0x01` = 1 lines (1280×1024@75: vtotal 1066, vsync 1025→1028, vdisplay 1024).
- `CFG 0x000c0000` → bits 19 and 18 set = +VSYNC and +HSYNC polarity.

## Honest limitations

- No pixels, no scanout, no 2D/3D engine, no PLL/HTG modelling — the driver
  itself marks those as GAPs that need real-hardware tracing.
- `R_ID` is a made-up value; the real board-id encoding is unknown.
- DDC returns no EDID on purpose (no virtual monitor); the driver uses its
  fixed HP timing table, which is the path this harness exercises.
- The device is enough to *bind and mode-set against*, which is what makes the
  driver's probe/mode-set code testable without the physical card — nothing more.
