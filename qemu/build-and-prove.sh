#!/usr/bin/env bash
#
# build-and-prove.sh — runs INSIDE the Debian trixie container (see Dockerfile).
#
# 1. Build qemu-system-x86_64 from a pinned tag with the visualizefx device.
# 2. Build the visualizefx.ko driver against the installed 6.12 kernel headers.
# 3. Assemble a minimal busybox initramfs that insmods the driver + its deps.
# 4. Boot the kernel under TCG with `-device visualizefx` and capture:
#       /out/serial.log      — guest dmesg (driver bind + R_ID + mode_set)
#       /out/qemu-trace.log  — device register-access trace (VHAL/VTG/CFG ...)
#
# Expects the fx5 repo bind-mounted at /work and an output dir at /out.
set -euo pipefail

QEMU_TAG="${QEMU_TAG:-v9.1.2}"
REPO=/work
QEMU_SRC=/opt/qemu
OUT=/out
JOBS="$(nproc)"

mkdir -p "$OUT"
KVER="$(ls /lib/modules | head -1)"
echo "=== kernel version in container: $KVER ==="

# --------------------------------------------------------------------------
# 1. Build QEMU with the visualizefx device
# --------------------------------------------------------------------------
QEMU_BIN="$QEMU_SRC/build/qemu-system-x86_64"
if [ ! -x "$QEMU_BIN" ]; then
    echo "=== [1/4] cloning + building QEMU $QEMU_TAG ==="
    # $QEMU_SRC may be a (non-empty) bind mount: clear its contents, not the dir.
    rm -rf "${QEMU_SRC:?}"/* "${QEMU_SRC}"/.[!.]* 2>/dev/null || true
    git clone --depth 1 --branch "$QEMU_TAG" \
        https://gitlab.com/qemu-project/qemu.git "$QEMU_SRC"

    "$REPO/qemu/integrate.sh" "$QEMU_SRC"

    cd "$QEMU_SRC"
    ./configure \
        --target-list=x86_64-softmmu \
        --enable-trace-backends=log \
        --disable-docs --disable-werror --disable-plugins
    make -j"$JOBS"
else
    echo "=== [1/4] QEMU already built at $QEMU_BIN — skipping ==="
fi

# Prove the device is in the build.
echo "=== visualizefx present in QEMU? ==="
"$QEMU_BIN" -device help 2>&1 | grep -i visualizefx \
    || { echo "FATAL: visualizefx device not registered in QEMU build"; exit 1; }

# --------------------------------------------------------------------------
# 2. Build the driver module
# --------------------------------------------------------------------------
echo "=== [2/4] building visualizefx.ko against $KVER ==="
make -C "$REPO/driver" clean KDIR="/lib/modules/$KVER/build" >/dev/null 2>&1 || true
make -C "$REPO/driver" KDIR="/lib/modules/$KVER/build"
test -f "$REPO/driver/visualizefx.ko"
modinfo "$REPO/driver/visualizefx.ko" | head -20

# Install into the module tree so dependencies resolve, then list dep order.
install -D "$REPO/driver/visualizefx.ko" "/lib/modules/$KVER/extra/visualizefx.ko"
depmod "$KVER"

# --------------------------------------------------------------------------
# 3. Build the initramfs
# --------------------------------------------------------------------------
echo "=== [3/4] assembling initramfs ==="
IRD=/tmp/initramfs
rm -rf "$IRD"; mkdir -p "$IRD"/{bin,proc,sys,dev,mods}
cp /bin/busybox "$IRD/bin/busybox"
for applet in sh mount ls cat dmesg insmod sleep poweroff echo grep; do
    ln -sf busybox "$IRD/bin/$applet"
done

# Static-linked raw-ioctl KMS demo (drives several modesets at runtime so the
# trace captures VHAL/VTG/CFG for multiple resolutions). Static so it needs no
# shared libs in the busybox initramfs.
echo "    building visfx-demo (KMS multi-mode demo)"
gcc -static -O2 -Wall -o "$IRD/bin/visfx-demo" "$REPO/qemu/visfx-kms-demo.c"

# Copy the dependency chain (decompressing .zst/.xz) in modprobe's load order.
i=0
modprobe --show-depends -S "$KVER" visualizefx | awk '$1=="insmod"{print $2}' | \
while read -r ko; do
    base="$(basename "$ko")"; base="${base%.zst}"; base="${base%.xz}"
    dst="$IRD/mods/$(printf '%02d' "$i")_$base"
    case "$ko" in
        *.zst) zstd -dcq "$ko" > "$dst" ;;
        *.xz)  xz -dc  "$ko" > "$dst" ;;
        *)     cp "$ko" "$dst" ;;
    esac
    echo "    + $base"
    i=$((i+1))
done

cat > "$IRD/init" <<'INIT'
#!/bin/busybox sh
export PATH=/bin
/bin/busybox --install -s /bin 2>/dev/null
mount -t proc  proc  /proc
mount -t sysfs sysfs /sys
mount -t devtmpfs dev /dev 2>/dev/null
exec >/dev/console 2>&1

echo "=================== visualizefx harness guest ==================="
for m in /mods/*; do
    echo ">> insmod $m"
    insmod "$m" || echo "   (insmod $m failed)"
done

echo "=================== lspci ==================="
# busybox has no lspci; read the PCI id from sysfs instead.
for d in /sys/bus/pci/devices/*; do
    printf '%s  vendor=%s device=%s driver=%s\n' \
        "$(basename "$d")" "$(cat "$d/vendor")" "$(cat "$d/device")" \
        "$(basename "$(readlink -f "$d/driver" 2>/dev/null)" 2>/dev/null)"
done

echo "=================== dmesg (visualize + any fault) ==================="
dmesg | grep -i -E 'visualize|drm|103c:100a|BUG|Oops|WARNING|RIP:|Call Trace|NULL pointer|Kernel panic|general protection|Killed' || true

echo "=================== /dev/dri ==================="
ls -l /dev/dri 2>/dev/null || echo "(no /dev/dri)"

echo "=================== DRM state (/sys/class/drm) ==================="
for s in /sys/class/drm/card0*/status; do [ -e "$s" ] && echo "$s = $(cat "$s")"; done
for s in /sys/class/drm/card0*/modes; do [ -e "$s" ] && { echo "$s:"; cat "$s"; }; done

echo "=================== KMS demo: multi-mode modeset ==================="
/bin/visfx-demo || echo "(visfx-demo exited $?)"

echo "=================== done; powering off ==================="
sync
poweroff -f
INIT
chmod +x "$IRD/init"

( cd "$IRD" && find . | cpio -o -H newc 2>/dev/null | gzip -9 ) > "$OUT/initramfs.cpio.gz"
echo "    initramfs: $(du -h "$OUT/initramfs.cpio.gz" | cut -f1)"

# --------------------------------------------------------------------------
# 4. Boot the VM
# --------------------------------------------------------------------------
echo "=== [4/4] booting VM with -device visualizefx ==="
rm -f "$OUT/serial.log" "$OUT/qemu-trace.log" "$OUT/qemu-stderr.log"

# Full distro kernel under TCG (no KVM in the colima VM) is slow, so give it
# room. drm.debug is dropped on purpose: the mode-set *register writes* are the
# proof and they show in the trace; keeping drm.debug + a high loglevel would
# flood the emulated serial and roughly double boot time.
timeout 600 "$QEMU_BIN" \
    -M pc -accel tcg,thread=multi -smp 2 -m 512 \
    -kernel "/boot/vmlinuz-$KVER" \
    -initrd "$OUT/initramfs.cpio.gz" \
    -append "console=ttyS0 panic=-1 loglevel=7" \
    -device visualizefx \
    -trace "enable=visualizefx_*" \
    -D "$OUT/qemu-trace.log" \
    -display none -vga none -monitor none \
    -serial "file:$OUT/serial.log" \
    -no-reboot 2> "$OUT/qemu-stderr.log" || true

echo
echo "########################### RESULTS ###########################"
echo "----- serial.log: driver bind / R_ID / mode_set -----"
grep -i -E 'visualize|board id|mode_set|bound|103c|vendor=0x103c' "$OUT/serial.log" || true
echo
echo "----- qemu-trace.log: device register accesses (tail) -----"
grep -i -E 'visualizefx_(realize|cfg|vhal|vtg|ddc)' "$OUT/qemu-trace.log" | tail -40 || \
    tail -40 "$OUT/qemu-trace.log" || true
echo "###############################################################"
echo "Full logs in $OUT/  (serial.log, qemu-trace.log, qemu-stderr.log)"
