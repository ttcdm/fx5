#!/usr/bin/env bash
#
# run.sh — host entry point (macOS). Brings up the Linux container runtime,
# builds the Debian image, and runs build-and-prove.sh inside it.
#
#   ./qemu/run.sh
#
# NOTE: this starts colima if it is not already running (the box had it
# installed but stopped). It requests 4 CPUs / 8 GB / 60 GB disk so the QEMU
# build fits; see EMULATION.md if you need to adjust or restore your colima.
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
REPO="$(cd "$HERE/.." && pwd)"
OUT="$REPO/qemu/out"
# QEMU checkout+build is cached in a docker NAMED VOLUME (stored on the colima
# VM disk, which has room) rather than a host bind mount — the Mac host disk is
# nearly full and a QEMU build is multiple GB.
QVOL=visualizefx-qemu-build
IMAGE=visualizefx-qemu:trixie

mkdir -p "$OUT"

if ! docker info >/dev/null 2>&1; then
    echo ">> docker daemon not reachable; starting colima (4 CPU / 8 GB / 60 GB)"
    colima start --cpu 4 --memory 8 --disk 60 \
        || { echo ">> retrying with existing colima profile"; colima start; }
fi

echo ">> building image $IMAGE"
docker build -t "$IMAGE" "$HERE"

echo ">> running build-and-prove.sh in container"
docker run --rm \
    -v "$REPO":/work \
    -v "$OUT":/out \
    -v "$QVOL":/opt/qemu \
    "$IMAGE" -c '/work/qemu/build-and-prove.sh'

echo ">> done. Outputs in $OUT/"
ls -la "$OUT"
