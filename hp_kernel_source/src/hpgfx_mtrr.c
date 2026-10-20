/* vi:set ts=8:sw=8:
 ******************************************************************************
 * hpgfx_mtrr.c
 * 
 * Utilities for managing Intel mtr registers.
 *
 * Portions leveraged from arch/i386/kernel/mtrr.c (since mtrr.c doesn't bother
 * to export most of its functionality).
 *
 * Mostly written by Brett Johnson & Keith Fish, HP.
 ******************************************************************************
 * Copyright © 2000 Hewlett-Packard Company
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *****************************************************************************/

/* define for std includes used below */
#define _LINUX 1
#define __NO_VERSION__

#include "drm/drmP.h"
#include "hpgfx_drv.h"
#include "mtrr.h"

#define MTRRcap_MSR     0x0fe
#define MTRRdefType_MSR 0x2ff

#define MTRRphysBase_MSR(reg) (0x200 + 2 * (reg))
#define MTRRphysMask_MSR(reg) (0x200 + 2 * (reg) + 1)

/*
 * Access to machine-specific registers (available on 586 and better only)
 * Note: the rd* operations modify the parameters directly (without using
 * pointer indirection), this allows gcc to optimize better
 */
#define rdmsr(msr,val1,val2) \
       __asm__ __volatile__("rdmsr" \
			    : "=a" (val1), "=d" (val2) \
			    : "c" (msr))

#define wrmsr(msr,val1,val2) \
     __asm__ __volatile__("wrmsr" \
			  : /* no outputs */ \
			  : "c" (msr), "a" (val1), "d" (val2))

#define rdtsc(low,high) \
     __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high))

#define rdpmc(counter,low,high) \
     __asm__ __volatile__("rdpmc" \
			  : "=a" (low), "=d" (high) \
			  : "c" (counter))

/* In the processor's MTRR interface, the MTRR type is always held in
   an 8 bit field: */
typedef u8 mtrr_type;

/* non-zero if we have the write-combining memory type. */
static int have_wrcomb (void)
{
    unsigned long config, dummy;

    rdmsr(MTRRcap_MSR, config, dummy);
    return (config & (1<<10));
}

static void get_mtrr (unsigned int reg, unsigned long *base,
		      unsigned long *size, mtrr_type *type)
{
    unsigned long dummy, mask_lo, base_lo;

    rdmsr(MTRRphysMask_MSR(reg), mask_lo, dummy);
    if ((mask_lo & 0x800) == 0) {
	/* Invalid (i.e. free) range. */
	*base = 0;
	*size = 0;
	*type = 0;
	return;
    }

    rdmsr(MTRRphysBase_MSR(reg), base_lo, dummy);

    /* We ignore the extra address bits (32-35). If someone wants to
       run x86 Linux on a machine with >4GB memory, this will be the
       least of their problems. */

    /* Clean up mask_lo so it gives the real address mask. */
    mask_lo = (mask_lo & 0xfffff000UL);
    
    /* This works correctly if size is a power of two, i.e. a
       contiguous range. */
    *size = ~(mask_lo - 1);

    *base = (base_lo & 0xfffff000UL);
    *type = (base_lo & 0xff);
}   /*  End Function get_mtrr  */

static char *mtrr_strings[MTRR_NUM_TYPES] =
{
    "uncachable",               /* 0 */
    "write-combining",          /* 1 */
    "?",                        /* 2 */
    "?",                        /* 3 */
    "write-through",            /* 4 */
    "write-protect",            /* 5 */
    "write-back",               /* 6 */
};

static char *attrib_to_str (int x)
{
    return (x <= 6) ? mtrr_strings[x] : "?";
}   /*  End Function attrib_to_str  */


static uintptr_t sWBMemBase=0, sWBMemSize=0;
static int sUsedMtrrs[8] = {-1,-1,-1,-1,-1,-1,-1,-1};

int hpgfx_mark_phys_mem_wrcomb(uintptr_t base, uintptr_t size)
{
    uintptr_t high1, high2, low1, low2;
    int num_mtrrs, retval;

#if DRM_DEBUG_CODE
    /* Throw a fit if we've already been called! */
    if (sWBMemBase + sWBMemSize != 0) {
        DRM_ERROR("We've been called before.  Bailing out!\n");
        return -EINVAL;
    }
#endif

    if (base & (size-1)) {
        DRM_ERROR("base=0x%x is not a multiple of size=0x%x -- request ignored\n",
                  base, size);
        return -EINVAL;
    }

    /* Look for the existing WB mtrr that encompasses our WC block, then nuke it
     * (but first save it's configuration so we can restore it later). */
    for (num_mtrrs=0; num_mtrrs<8; num_mtrrs++) {
        ulong tbase, tsize;
        mtrr_type ttype;

        get_mtrr(num_mtrrs, &tbase, &tsize, &ttype);

        /* Is this a WB segment, and does our block fit inside it? */
        if (ttype == MTRR_TYPE_WRBACK) {
            if ((tsize == 0) && (tbase == 0)) {
                DRM_DEBUG("Eww...  We must have a 4 gig system.  Engaging Corbomite "
                          "device to hack around kernel bugs...\n");
                /* YUCK!  This is one stinky hack to work around the kernel's mtrr
                 * code.  Apparently, we have a 4 gig system here.  The BIOS sets
                 * up MTRR 1 to map 0->4G as WB cacheable, but the mtrr kernel code
                 * just throws away the extra address bits (which include bit 32,
                 * which specifies the 4Gb size).  So, the kernel reports the
                 * size of this region as 0.
                 *
                 * The stinky workaround for this ugly situation is to modify the
                 * default mapping to use 2 MTR regions of 2GB each, then start
                 * over in this loop. EWW, YUCKY! */
                
                num_mtrrs = mtrr_add(0x00000000, 0x80000000, MTRR_TYPE_WRBACK, 0);
                /* mtrr_add(0x80000000, 0x80000000, MTRR_TYPE_WRBACK, 0); */
                /* Another HACK!  mtrr_add chokes on this.  It has some code that
                 * adds the base & size together, stores the result in a 32-bit 
                 * integer, then checks its size (which fails, since the resulting
                 * 4G number overflows the 32 bit number.  So, we'll have to set
                 * this register directly...  We'll make the (hopefully valid!)
                 * assumption that the next register is free...  Yikes!... */
                wrmsr (MTRRphysBase_MSR (num_mtrrs+1), 0x80000000 | MTRR_TYPE_WRBACK, 0);
                wrmsr (MTRRphysMask_MSR (num_mtrrs+1), ~(0x80000000 - 1) | 0x800, 0);

                num_mtrrs = 0;
                continue;
            }

            if ((tsize > 0) && (base >= tbase) && (base+size <= tbase+tsize)) {
                DRM_DEBUG("Found an mtrr(%d) to resegment: tbase=0x%x, tsize=%dMB, type=\"%s\"\n",
                          num_mtrrs, tbase, tsize>>20, attrib_to_str(ttype));

                if (mtrr_del(num_mtrrs, tbase, tsize) < 0) {
                    return -ENOMEM;
                }
                sWBMemBase = tbase;
                sWBMemSize = tsize;
                break;
            }
        }
    } 

    num_mtrrs = 0;
    /* Now, use one mtrr to map our wrcomb memory... */
    retval = mtrr_add(base, size, MTRR_TYPE_WRCOMB, 0);
    if (retval < 0) {
        DRM_DEBUG("Mapping failed for: base=0x%x, size=%dMB write-combined!\n", base, size>>20);
        return retval;
    }

    DRM_DEBUG("Mapped base=0x%x, size=%dMB write-combined, with mtrr %d\n",
              base, size>>20, retval);

    sUsedMtrrs[num_mtrrs++] = retval;

    /* Finally, try to map the rest of memory write-through (starting with the biggest
     * chunks available).  Bail if we run out of mtrrs. */
    high1 = sWBMemBase + sWBMemSize;
    low1 = base+size;
    high2 = base;
    low2 = sWBMemBase;

    while ((high1 > low1) || (high2 > low2)) {
        uint order;

        if ((high1-low1) > (high2-low2)) {
            size = high1-low1;
            base = low1;
        }
        else {
            size = high2-low2;
            base = low2;
        }

        /* Align the size to a power of two. */
        /* 64 BIT WARNING! 32 bit memory size hardcode! */
        order = 31;
        while ((order > 0) && 
               ((size >> order) <= 0)) {
            order--;
        }
        size = 0x1 << order;

        /* Align the base to the same power of two that the size is aligned to. */
        base = (base + (size - 1)) & (~(size - 1));

        /* Now, use an mtrr to map the memory write-through cacheable... */
        retval = mtrr_add(base, size, MTRR_TYPE_WRBACK, 0);
        if (retval < 0) {
            DRM_DEBUG("Mapping failed for: base=0x%x, size=%dMB write-back cacheable!\n",
                      base, size>>20);
            break;
        }
        DRM_DEBUG("Mapped base=0x%x, size=%dMB write-back cacheable, with mtrr %d\n",
                  base, size>>20, retval);

        sUsedMtrrs[num_mtrrs++] = retval;
        if (base > high2) { /* We took it from the high memory range. */
            high1 -= size;
        }
        else {
            low2 += size;
        }
    }

    return retval;
}

void hpgfx_restore_mtrrs(int flags)
{
    if (sWBMemBase + sWBMemSize > 0) {
        int i;

        for (i=0; i<8; i++) {
            if (sUsedMtrrs[i] >= 0) {
                DRM_DEBUG("Deleting mtrr %d\n", sUsedMtrrs[i]);
                mtrr_del(sUsedMtrrs[i], 0, 0);
                sUsedMtrrs[i] = -1;
            }
        }
        DRM_DEBUG("Restoring WB mtrr: base=0x%x, size=%dMB\n", sWBMemBase, sWBMemSize>>20);
        mtrr_add(sWBMemBase, sWBMemSize, MTRR_TYPE_WRBACK, 0);
        sWBMemBase = sWBMemSize = 0;
    }
}

