###############################################################################
# This makefile builds the kernel module for HP's Visualize FX graphics
# Xserver and OpenGL libraries.
# There's almost no "make smarts" in this file -- most of the work is
# done by Makefile.kernel. This Makefile just allows us to build both
# UP and SMP with parameterized calls to Makefile.kernel.
###############################################################################
GRAPHICS_VERSION=/src/multi_product_headers/linux_version
BUILD_NUMBER=/src/linux_build_number

default:  all

# Make sure these targets don't get short-circuited by a file of that name
.PHONY: hpgfx.o clean depend UP up SMP smp all install

UP/.depend:
	$(MAKE) -f Makefile.kernel SMP=0 depend
SMP/.depend:
	$(MAKE) -f Makefile.kernel SMP=1 depend

up UP:	UP/.depend
	@ echo -e "\n*** BUILDING UP MODULE ***"
	$(MAKE) -f Makefile.kernel SMP=0

smp SMP: SMP/.depend
	@ echo -e "\n*** BUILDING SMP MODULE ***"
	$(MAKE) -f Makefile.kernel SMP=1

# Compile the appropriate one last; ignore errors in each piece
DEFAULT:=$(shell grep -q '__BOOT_KERNEL_UP.*1' /boot/kernel.h 2>/dev/null \
    && echo "UP" || echo "SMP")
ifeq ($(DEFAULT),UP)
all:
	@- $(MAKE) SMP
	@- $(MAKE) UP
else
all:
	@- $(MAKE) UP
	@- $(MAKE) SMP
endif

install_up:  UP
	$(MAKE) -f Makefile.kernel SMP=0 install

install_smp:  SMP
	$(MAKE) -f Makefile.kernel SMP=1 install

install:  all
	@ echo -e "\n*** INSTALLING ***"
	install -c hpgraphics /etc/rc.d/init.d
	$(MAKE) -f Makefile.kernel SMP=0 install
	$(MAKE) -f Makefile.kernel SMP=1 install

hpgfx.o clean::
	$(MAKE) -f Makefile.kernel $@

depend:
	@ echo -e "\n*** ANALYZING MAKE DEPENDENCIES ***"
	rm -f UP/.depend SMP/.depend
	$(MAKE) UP/.depend SMP/.depend


###############################################################################
# The following target just copies some header files from our standard
# place in the build (e.g. /src/multi_product_headers) to local directories
# so that this source can be easily packaged for the user.
###############################################################################
DSMH=/src/multi_product_headers/dsm_headers
SYS=/src/multi_product_headers/sys
HPGFX=/src/multi_product_code/hpgfx
.PHONY: Headers clean_headers tar
Headers:
	@ if [ ! -d $(DSMH) ]; then \
	    echo "No header source directory $(DSMH)!" >&2; \
	    false; \
	fi
	@ if [ ! -d $(SYS) ]; then \
	    echo "No header source directory $(SYS)!" >&2; \
	    false; \
	fi
	@ if [ ! -d $(HPGFX) ]; then \
	    echo "No header source directory $(HPGFX)!" >&2; \
	    false; \
	fi
	mkdir -p headers headers/sys
	@ rm -f headers/linuxkernel
	ln -s .. headers/linuxkernel
	cp $(DSMH)/bitfield.h		headers
	cp $(DSMH)/hp_config.h		headers
	cp $(DSMH)/lego_address.h	headers
	cp $(DSMH)/lego_basictypes.h	headers
	cp $(DSMH)/lego_hw.h		headers
	cp $(DSMH)/lego_hw_macros.h	headers
	cp $(SYS)/framebuf.h		headers/sys
	cp $(HPGFX)/hpgfx_shared_dma.h	headers
	cp $(HPGFX)/hpgfx_utils.c	headers
	cp $(HPGFX)/hpgfx_utils.h	headers

clean_headers:
	rm -rf headers

KERNEL_SRC=/usr/src/redhat/SOURCES/hpgraphics_kernel-`cat $(GRAPHICS_VERSION)`-`cat $(BUILD_NUMBER)`
tar:	Headers
	@ $(MAKE) -f Makefile.kernel clean
	@ rm -f $(KERNEL_SRC).tar $(KERNEL_SRC).tgz
	tar cvf $(KERNEL_SRC).tar *
	gzip -9v $(KERNEL_SRC).tar
	mv -f $(KERNEL_SRC).tar.gz $(KERNEL_SRC).tgz
