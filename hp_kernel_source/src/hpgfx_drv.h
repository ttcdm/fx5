/* hpgfx_drv.h -- Private header for HP Visualize driver 
 * Created: Mon Jan  4 10:05:05 1999 by faith@precisioninsight.com
 * Revised: Fri Aug 20 09:24:27 1999 by faith@precisioninsight.com
 *
 * Copyright 1999 Precision Insight, Inc., Cedar Park, Texas.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * 
 * $PI: xc/programs/Xserver/hw/xfree86/os-support/linux/drm/kernel/hpgfx_drv.h,v 1.4 1999/08/30 13:05:00 faith Exp $
 * $XFree86: xc/programs/Xserver/hw/xfree86/os-support/linux/drm/kernel/hpgfx_drv.h,v 1.1 1999/09/25 14:38:00 dawes Exp $
 * 
 */

#ifndef _HPGFX_DRV_H_
#define _HPGFX_DRV_H_

#include <linux/types.h>
#include <linux/fs.h>
#include "lego_address.h"

/* Copied from <stdint.h>.. */
/* Types for `void *' pointers.  */
#if __WORDSIZE == 64
# ifndef intptr_t
typedef long int		intptr_t;
#  define intptr_t intptr_t
# endif
typedef unsigned long int	uintptr_t;
#else
# ifndef intptr_t
typedef int			intptr_t;
#  define intptr_t intptr_t
# endif
typedef unsigned int		uintptr_t;
#endif


/* Largest integral types.  */
#if __WORDSIZE == 64
typedef long int		intmax_t;
typedef unsigned long int	uintmax_t;
#else
__extension__
typedef long long int		intmax_t;
__extension__
typedef unsigned long long int	uintmax_t;
#endif

				/* hpgfx_drv.c */
extern int  hpgfx_init(void);
extern void hpgfx_cleanup(void);
extern int  hpgfx_reset_uc(struct inode *inode, struct file *filp,
			  unsigned int cmd, unsigned long arg);
extern int  hpgfx_version(struct inode *inode, struct file *filp,
			  unsigned int cmd, unsigned long arg);
extern int  hpgfx_open(struct inode *inode, struct file *filp);
extern int  hpgfx_release(struct inode *inode, struct file *filp);
extern int  hpgfx_ioctl(struct inode *inode, struct file *filp,
			unsigned int cmd, unsigned long arg);
extern int  hpgfx_lock(struct inode *inode, struct file *filp,
		       unsigned int cmd, unsigned long arg);
extern int  hpgfx_unlock(struct inode *inode, struct file *filp,
			 unsigned int cmd, unsigned long arg);

				/* hpgfx_dma.c */
extern int  hpgfx_dma_schedule(drm_device_t *dev, int locked);
extern int  hpgfx_dma(struct inode *inode, struct file *filp,
		      unsigned int cmd, unsigned long arg);
extern int  hpgfx_irq_install(drm_device_t *dev, int irq);
extern int  hpgfx_irq_uninstall(drm_device_t *dev);
extern int  hpgfx_control(struct inode *inode, struct file *filp,
			  unsigned int cmd, unsigned long arg);
extern int hpgfx_addmap(struct inode *inode, struct file *filp, unsigned int cmd,
                        unsigned long arg);

extern int hpgfx_flush_block_and_flush(drm_device_t *dev, int context,
                                       drm_lock_flags_t flags);

extern int hpgfx_dma_initialize( struct inode *inode, struct file *filp, unsigned int cmd,
                                 unsigned long arg);

extern int hpgfx_ntdma_close(struct inode *inode, struct file *filp, unsigned int cmd,
                             unsigned long arg);

extern int hpgfx_dma_alloc_buff(struct inode *inode, struct file *filp, unsigned int cmd,
                                unsigned long arg);

extern int hpgfx_dma_free_buff(struct inode *inode, struct file *filp, unsigned int cmd,
                               unsigned long arg);

extern int hpgfx_dma_cd_read_from_host(struct inode *inode, struct file *filp, unsigned int cmd,
                                       unsigned long arg);

extern int hpgfx_dma_write_to_host (struct inode *inode, struct file *filp, unsigned int cmd,
                                    unsigned long arg);

extern int hpgfx_dma_finish (struct inode *inode, struct file *filp, unsigned int cmd,
                             unsigned long arg);

extern int hpgfx_dma_finish_all (struct inode *inode, struct file *filp, unsigned int cmd,
                                 unsigned long arg);
extern int hpgfx_getLog (struct inode *inode, struct file *filp, unsigned int cmd,
                                 unsigned long arg);

extern int hpgfx_dma_raster_to_device (struct inode *inode, struct file *filp, unsigned int cmd,
                                        unsigned long arg);

#ifdef MULTITHREAD       /* [ */
#define	CD_ALIGN	0x0800		/* That is, 2Kbytes */
#else
#define	CD_ALIGN	0x1000		/* That is, 4Kbytes */
#endif            /* ] */
#define	CD_BUFFER_MAX	0x20000/2		/* That is, 64Kbytes */
#define	CD_BUFFER_HW	0x8000		/* That is, 32Kbytes */
#define	CD_PAD		128		/* That is, 32 words or 1 packet */


#ifdef PAGE_ALIGNED_CD_BUFFER       /* [ */
#define	CD_NEED_WRAP(p) ((((long)(p)) & (CD_ALIGN-1)) >= (CD_ALIGN-CD_PAD))
#define	CD_NEED_REAL_WRAP(p) \
	((((char *)(p))-(char *)(DSM_CD_BUFFER_BASE)) >= \
			((DSM_CD_BUFFER_SIZE)-CD_PAD))
#else
#define	CD_NEED_WRAP(p) (((long)(p)) & CD_BLOCK_CLOSE)
#define	CD_NEED_REAL_WRAP(p)	CD_NEED_WRAP(p)
#endif            /* ] */

/*
 * First we want a macro that will create a CD packet header. This header has
 * three fields:
 *
 *   o bits 27-31 : The number of data words in this packet
 *   o bit  25    : Flag telling us whether to increment the address every time
 *                  we write the packet data. 0 means increment.
 *   o bit  24    : Flag telling us to wrap the hardware CD buffer pointer back to
 *                  the top of the CD buffer. This needs to be done every time we
 *                  wrap the software CD buffer pointer.
 *   o bits 0-23  : The starting address where we will write the packet data
 *
 * For more information, refer to section 3.5.1 in the Summit96 ERS
 */
#define CREATE_CD_PACKET_HEADER(_count, _noCount, _wrap, _addr) \
    (((_count) << 27) | ((_noCount) << 25) | ((_wrap) << 24) | (_addr))


/* Macros for I/O to a given word offset from the csaddr base */
#define	HW_WRITE32_OFFS(_a,offs,_d) \
	(*(volatile uint32_t *)((offs)+(volatile uint8_t *)(_a)) = (uint32_t)(_d))

#define	HW_READ32_OFFS(_a,offs) \
	(*(volatile uint32_t *)((offs)+(volatile uint8_t *)(_a)))

#define	HW_READ_OFFSET_FLOAT(_a,offs) \
	(*(volatile float *)((offs)+(volatile uint8_t *)(_a)))

/* Overload these macros, so I don't have to change the NT source. */
#define SUMMIT_READ32_OFFS HW_READ32_OFFS
#define SUMMIT_WRITE32_OFFS HW_WRITE32_OFFS


/*
**  Programming by contract macros.  Since I can't use exceptions, these are pretty
**  crummy attempts to mimic the post and pre condition clauses in the Eiffel language.
**  Basically, the REQUIRE clause is placed at the begining of the function, and spells
**  out the entry portion of the contract.  The ENSURE macro is placed at the end of the
**  function, and spells out the exit contract.  The CHECK macro can be used anywhere
**  just to check an assertion.  The "thendo" argument can be any kind of statement
**  that specifies what is done if the expression fails evaluation (i.e. it can
**  return an error, deref a NULL pointer, do nothing, whatever...).
*/
#ifdef DEBUG

#define REQUIRE(exp, elsedo); \
            if (!(exp)) {           \
                printk(KERN_ERR "[" DRM_NAME ":" __FUNCTION__ "] " \
                       "Failed REQUIRE(" #exp " " #elsedo ") in "__FILE__" @ line %d\n",__LINE__); \
                elsedo;  }

#define CHECK(exp); \
            if (!(exp)) {           \
                printk(KERN_DEBUG "[" DRM_NAME ":" __FUNCTION__ "] " \
                       "Failed CHECK(" #exp ") in "__FILE__" @ line %d\n",__LINE__); }

#define ENSURE(exp, elsedo); \
            if (!(exp)) {           \
                printk(KERN_ERR "[" DRM_NAME ":" __FUNCTION__ "] " \
                       "Failed ENSURE(" #exp " " #elsedo ") in "__FILE__" @ line %d\n",__LINE__); \
                elsedo;  }
#else

#define REQUIRE(exp, elsedo);

#define CHECK(exp);

#define ENSURE(exp, elsedo);

#endif /* DEBUG */

#endif

