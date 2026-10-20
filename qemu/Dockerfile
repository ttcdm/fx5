# Debian trixie (= Linux 6.12 LTS) build+prove environment for the visualizefx
# QEMU device harness. Matches the kernel the driver/ scaffold was verified
# against. Everything (QEMU build, .ko build, VM boot) happens inside here.
FROM debian:trixie

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        ca-certificates git build-essential bc flex bison \
        ninja-build meson pkg-config python3 python3-venv \
        libglib2.0-dev libpixman-1-dev zlib1g-dev libfdt-dev \
        libslirp-dev \
        linux-image-amd64 linux-headers-amd64 libelf-dev kmod \
        zstd xz-utils cpio busybox-static pciutils \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /work
# The fx5 repo (driver/ + qemu/) is bind-mounted at /work at run time.
# Outputs are written to /out (also bind-mounted).
ENTRYPOINT ["/bin/bash"]
