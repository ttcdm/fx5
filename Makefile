# SPDX-License-Identifier: GPL-2.0
#
# Out-of-tree build for the visualizefx_drm scaffold.
#
#   make -C /lib/modules/$(uname -r)/build M=$(pwd) modules
#
# NOTE: this is a SCAFFOLD. It is written against the Linux 6.x DRM API but has
# not been compiled here (no kernel tree on the authoring machine). Expect to
# fix small API drift for your exact kernel version before it builds, and know
# that even once it builds it will not light a display until the GAP items in
# the source and in PORT_PLAN.md are filled.

obj-m += visualizefx.o
visualizefx-y := visualizefx_drv.o visualizefx_modes.o

KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(CURDIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean
