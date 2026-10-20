#!/usr/bin/env bash
# Integrate the visualizefx device into a QEMU source tree.
#
#   ./integrate.sh /path/to/qemu-src
#
# Idempotent: copies hw/display/visualizefx.c and appends the Kconfig, meson and
# trace-events glue only if not already present.
set -euo pipefail

QEMU_SRC="${1:?usage: integrate.sh /path/to/qemu-src}"
HERE="$(cd "$(dirname "$0")" && pwd)"

disp="$QEMU_SRC/hw/display"
[ -d "$disp" ] || { echo "error: $disp not found — is $QEMU_SRC a QEMU tree?" >&2; exit 1; }

echo ">> copying visualizefx.c into hw/display/"
cp "$HERE/visualizefx.c" "$disp/visualizefx.c"

append_once() {
    local marker="$1" file="$2" snippet="$3"
    if grep -q "$marker" "$file"; then
        echo ">> $file already has $marker — skipping"
    else
        echo ">> appending to $file"
        printf '\n' >> "$file"
        cat "$snippet" >> "$file"
    fi
}

append_once "CONFIG_VISUALIZEFX" "$disp/meson.build"   "$HERE/meson.snippet"
append_once "config VISUALIZEFX" "$disp/Kconfig"       "$HERE/Kconfig.snippet"
append_once "visualizefx_realize" "$disp/trace-events" "$HERE/trace-events.snippet"

echo ">> integration done."
