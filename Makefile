# SPDX-License-Identifier: GPL-2.0
#
# Out-of-tree build for the visualizefx_drm scaffold.
#
#   make -C /lib/modules/$(uname -r)/build M=$(pwd) modules
#
# NOTE: this is a SCAFFOLD. It builds cleanly as an out-of-tree module against
# Linux 6.12 LTS (verified: kernel headers 6.12.113, x86_64, make W=1 clean).
# A different kernel version may need small API fixups. Even once it builds it
# will not light a display until the GAP items in the source and in
# PORT_PLAN.md are filled.

obj-m += visualizefx.o
visualizefx-y := visualizefx_drv.o visualizefx_modes.o

KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(CURDIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean
