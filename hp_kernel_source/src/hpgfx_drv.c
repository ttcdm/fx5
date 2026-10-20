/* vi:set sw=8:
 ******************************************************************************
 * hpgfx.c -- HP Graphics device driver -*- linux-c -*-
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
 ******************************************************************************
 * Copyright 1999 Precision Insight, Inc., Cedar Park, Texas.
 * All Rights Reserved.
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
 *****************************************************************************/

#define EXPORT_SYMTAB
#include "drm/drmP.h"
#include "hpgfx_drv.h"
#include "hpgfx_shared_dma.h"
EXPORT_SYMBOL(hpgfx_init);
EXPORT_SYMBOL(hpgfx_cleanup);

#define HPGFX_NAME	 "hpgfx"
#define HPGFX_DESC	 "HP VISUALIZE"
#define HPGFX_DATE	 "2000731"
#define HPGFX_MAJOR	 0
#define HPGFX_MINOR	 0
#define HPGFX_PATCHLEVEL 2
#define HPGFX_MAX_DEVICES 8

#define HP_PCI_MODEL_FXX_MASK	0x100F

/* Arrays of data needed for each device. */

int hpgfx_mmap(struct file *filp, struct vm_area_struct *vma);

static struct file_operations hpgfx_fops = {
	open:	 hpgfx_open,
	flush:	 drm_flush,
	release: hpgfx_release,
	ioctl:	 hpgfx_ioctl,
	mmap:	 hpgfx_mmap,
	read:	 drm_read,
	fasync:	 drm_fasync,
};

static int                    hpgfx_num_devs;
static drm_device_t	      hpgfx_devices[HPGFX_MAX_DEVICES];

static drm_ioctl_desc_t	      hpgfx_ioctls[] = {
	[DRM_IOCTL_NR(DRM_IOCTL_VERSION)]    = { hpgfx_version,	  0, 0 },
	[DRM_IOCTL_NR(DRM_IOCTL_GET_UNIQUE)] = { drm_getunique,	  0, 0 },
	[DRM_IOCTL_NR(DRM_IOCTL_GET_MAGIC)]  = { drm_getmagic,	  0, 0 },
	[DRM_IOCTL_NR(DRM_IOCTL_IRQ_BUSID)]  = { drm_irq_busid,	  0, 1 },
#if DRM_DEBUG_CODE
	[DRM_IOCTL_NR(DRM_IOCTL_RESET_UC)]   = { hpgfx_reset_uc,  0, 1 },
#endif
	[DRM_IOCTL_NR(DRM_IOCTL_SET_UNIQUE)] = { drm_setunique,	  1, 1 },
	[DRM_IOCTL_NR(DRM_IOCTL_AUTH_MAGIC)] = { drm_authmagic,	  1, 1 },
	[DRM_IOCTL_NR(DRM_IOCTL_ADD_MAP)]    = { hpgfx_addmap,	  1, 1 },

	[DRM_IOCTL_NR(DRM_IOCTL_ADD_CTX)]    = { drm_addctx,	  1, 1 },
	[DRM_IOCTL_NR(DRM_IOCTL_RM_CTX)]     = { drm_rmctx,	  1, 1 },
	[DRM_IOCTL_NR(DRM_IOCTL_MOD_CTX)]    = { drm_modctx,	  1, 1 },
	[DRM_IOCTL_NR(DRM_IOCTL_GET_CTX)]    = { drm_getctx,	  1, 0 },
	[DRM_IOCTL_NR(DRM_IOCTL_SWITCH_CTX)] = { drm_switchctx,	  1, 1 },
	[DRM_IOCTL_NR(DRM_IOCTL_NEW_CTX)]    = { drm_newctx,	  1, 1 },
	[DRM_IOCTL_NR(DRM_IOCTL_RES_CTX)]    = { drm_resctx,	  1, 0 },
	
	[DRM_IOCTL_NR(DRM_IOCTL_LOCK)]	     = { hpgfx_lock,	  1, 0 },
	[DRM_IOCTL_NR(DRM_IOCTL_UNLOCK)]     = { hpgfx_unlock,	  1, 0 },

	[HPGFX_IOCTL_NR(HPGFX_IOCTL_DMA_ALLOC_BUFF)] 	        = { hpgfx_dma_alloc_buff,       1, 0 },
	[HPGFX_IOCTL_NR(HPGFX_IOCTL_DMA_FREE_BUFF)] 	        = { hpgfx_dma_free_buff,        1, 0 },
	[HPGFX_IOCTL_NR(HPGFX_IOCTL_DMA_INITIALIZE)] 	        = { hpgfx_dma_initialize,       1, 0 },
	[HPGFX_IOCTL_NR(HPGFX_IOCTL_DMA_WRITE_TO_HOST)] 	= { hpgfx_dma_write_to_host,    1, 0 },
	[HPGFX_IOCTL_NR(HPGFX_IOCTL_DMA_CD_READ_FROM_HOST)]     = { hpgfx_dma_cd_read_from_host,1, 0 },
	[HPGFX_IOCTL_NR(HPGFX_IOCTL_DMA_FINISH)] 		= { hpgfx_dma_finish,           1, 0 },
	[HPGFX_IOCTL_NR(HPGFX_IOCTL_DMA_FINISH_ALL)] 	        = { hpgfx_dma_finish_all,       1, 0 },
	[HPGFX_IOCTL_NR(HPGFX_IOCTL_GET_LOG)] 	        	= { hpgfx_getLog,       	1, 0 },
};
#define HPGFX_IOCTL_COUNT DRM_ARRAY_SIZE(hpgfx_ioctls)

#ifdef MODULE
int			      init_module(void);
void			      cleanup_module(void);
static char		      *hpgfx = NULL;

MODULE_AUTHOR("HP Workstation Systems Lab, Ft. Collins, Colorado");
MODULE_DESCRIPTION("HP VISUALIZE");
MODULE_PARM(hpgfx, "s");
#ifdef MODULE_LICENSE
MODULE_LICENSE("GPL");
#endif

/* init_module is called when insmod is used to load the module */

int init_module(void)
{
	return hpgfx_init();
}

/* cleanup_module is called when rmmod is used to unload the module */

void cleanup_module(void)
{
	hpgfx_cleanup();
}
#endif

static int hpgfx_setup(drm_device_t *dev)
{
	int i;
	
	atomic_set(&dev->ioctl_count, 0);
	atomic_set(&dev->vma_count, 0);
	dev->buf_use	  = 0;
	atomic_set(&dev->buf_alloc, 0);

	drm_dma_setup(dev);

	atomic_set(&dev->total_open, 0);
	atomic_set(&dev->total_close, 0);
	atomic_set(&dev->total_ioctl, 0);
	atomic_set(&dev->total_irq, 0);
	atomic_set(&dev->total_ctx, 0);
	atomic_set(&dev->total_locks, 0);
	atomic_set(&dev->total_unlocks, 0);
	atomic_set(&dev->total_contends, 0);
	atomic_set(&dev->total_sleeps, 0);

	for (i = 0; i < DRM_HASH_SIZE; i++) {
		dev->magiclist[i].head = NULL;
		dev->magiclist[i].tail = NULL;
	}
	dev->maplist	    = NULL;
	dev->map_count	    = 0;
	dev->vmalist	    = NULL;
	dev->lock.hw_lock   = NULL;
	init_waitqueue_head(&dev->lock.lock_queue);
	dev->queue_count    = 0;
	dev->queue_reserved = 0;
	dev->queue_slots    = 0;
	dev->queuelist	    = NULL;
	dev->irq	    = 0;
	dev->context_flag   = 0;
	dev->interrupt_flag = 0;
	dev->dma_flag	    = 0;
	dev->last_context   = 0;
	dev->last_switch    = 0;
	dev->last_checked   = 0;
	init_timer(&dev->timer);
	init_waitqueue_head(&dev->context_wait);
#if DRM_DMA_HISTO
	memset(&dev->histo, 0, sizeof(dev->histo));
#endif
	dev->ctx_start	    = 0;
	dev->lck_start	    = 0;
	
	dev->buf_rp	  = dev->buf;
	dev->buf_wp	  = dev->buf;
	dev->buf_end	  = dev->buf + DRM_BSZ;
	dev->buf_async	  = NULL;
	init_waitqueue_head(&dev->buf_readers);
	init_waitqueue_head(&dev->buf_writers);
			
	DRM_DEBUG("\n");
			
	/* The kernel's context could be created here, but is now created
	   in drm_dma_enqueue.	This is more resource-efficient for
	   hardware that does not do DMA, but may mean that
	   drm_select_queue fails between the time the interrupt is
	   initialized and the time the queues are initialized. */
			
	return 0;
}


static int hpgfx_takedown(drm_device_t *dev)
{
	int		  i;
	drm_magic_entry_t *pt, *next;
	drm_map_t	  *map;
	drm_vma_entry_t	  *vma, *vma_next;

	DRM_DEBUG("\n");

	down(&dev->struct_sem);
	del_timer(&dev->timer);
				/* Clear pid list */
	for (i = 0; i < DRM_HASH_SIZE; i++) {
		for (pt = dev->magiclist[i].head; pt; pt = next) {
			next = pt->next;
			drm_free(pt, sizeof(*pt), DRM_MEM_MAGIC);
		}
		dev->magiclist[i].head = dev->magiclist[i].tail = NULL;
	}
	
				/* Clear vma list (only built for debugging) */
	if (dev->vmalist) {
		for (vma = dev->vmalist; vma; vma = vma_next) {
			vma_next = vma->next;
			drm_free(vma, sizeof(*vma), DRM_MEM_VMAS);
		}
		dev->vmalist = NULL;
	}
	
				/* Clear map area and mtrr information */
	if (dev->maplist) {
		for (i = 0; i < dev->map_count; i++) {
			map = dev->maplist[i];
			switch (map->type) {
			case _DRM_REGISTERS:
			case _DRM_FRAME_BUFFER:
#ifdef CONFIG_MTRR
				if (map->mtrr >= 0) {
					int retcode;
					retcode = mtrr_del(map->mtrr,
							   map->offset,
							   map->size);
					DRM_DEBUG("mtrr_del = %d\n", retcode);
				}
#endif
				drm_ioremapfree(map->handle, map->size);
				break;
			case _DRM_SHM:
				drm_free_pages((unsigned long)map->handle,
					       drm_order(map->size)
					       - PAGE_SHIFT,
					       DRM_MEM_SAREA);
				break;
			}
			drm_free(map, sizeof(*map), DRM_MEM_MAPS);
		}
		drm_free(dev->maplist,
			 dev->map_count * sizeof(*dev->maplist),
			 DRM_MEM_MAPS);
		dev->maplist   = NULL;
		dev->map_count = 0;
	}
	
	if (dev->queuelist) {
		for (i = 0; i < dev->queue_count; i++) {
			drm_waitlist_destroy(&dev->queuelist[i]->waitlist);
			if (dev->queuelist[i]) {
				drm_free(dev->queuelist[i],
					 sizeof(*dev->queuelist[0]),
					 DRM_MEM_QUEUES);
				dev->queuelist[i] = NULL;
			}
		}
		drm_free(dev->queuelist,
			 dev->queue_slots * sizeof(*dev->queuelist),
			 DRM_MEM_QUEUES);
		dev->queuelist	 = NULL;
	}

	drm_dma_takedown(dev);

	dev->queue_count     = 0;
	if (dev->lock.hw_lock) {
		dev->lock.hw_lock    = NULL; /* SHM removed */
		dev->lock.pid	     = 0;
		wake_up_interruptible(&dev->lock.lock_queue);
	}

        /* NULL the control space address pointer.  When the X server
         * recycles, it unmaps and then remaps the device.  drmAddMap()
         * may create a new mapping that differs from that of the
         * previous mapping.  Make sure that we store the correct control
         * space address pointer by NULL'ing the dev->csaddr field.
         * This field gets initialized by hpgfx_addmap, but only when
         * csaddr is non-NULL.
         *
         * NOTE: the DMA manager also maintains a copy of this pointer.
         * We fool it into reinitializing this pointer as well.  See
         * hpgfx_dma_free_buffers() for more details.
         */
        dev->csaddr = (uintptr_t)NULL;
 
	up(&dev->struct_sem);
	
	return 0;
}

/* setunique is only called internally to set (or reset) the unique ID for a
 * device. */
static int setunique(drm_device_t *dev, unsigned char bus, unsigned char devfn)
{
        char str[128];

	if (dev->unique) {
                drm_free(dev->unique, strlen(dev->unique)+1, DRM_MEM_DRIVER);
        }

	if (dev->devname) {
                drm_free(dev->devname, strlen(dev->devname)+1, DRM_MEM_DRIVER);
        }

        sprintf(str, "%d:%d:%d", bus, PCI_SLOT(devfn), PCI_FUNC(devfn));
	
	dev->unique_len = strlen(str);
	dev->unique	= drm_alloc(dev->unique_len+1, DRM_MEM_DRIVER);
        if (dev->unique == NULL) {
                return -ENOMEM;
        }
        memcpy(dev->unique, str, strlen(str));
	dev->unique[dev->unique_len] = '\0';

	dev->devname = drm_alloc(strlen(dev->name) + strlen(dev->unique) + 2,
				 DRM_MEM_DRIVER);
	sprintf(dev->devname, "%s@%s", dev->name, dev->unique);

	return 0;
}

#define RCC_VENDOR_ID 0x00081166

/* For explanations of these magical numbers, see chapter 6 of the AGP spec. */
#define PCI_VENDOR_OFFSET 0x00
#define PCI_STATUS_OFFSET 0x04
#define PCI_CAP_PTR_OFFSET 0x34
#define PCI_CAP_ID_MASK 0xFF
#define PCI_AGP_CAP_ID 0x02

static int hpgfx_agp_cap_ptr(struct pci_dev *pcidev, uint8_t *capPtr)
{
	uint32_t scratch;

        /* HW_WORKAROUND
         * The RCC chipset doesn't bother to set the CAP_LIST bit in the
         * status register, OR the CAP_PTR.  So, we just throw the PCI spec
         * in the trash, and hardcode the CAP_PTR. */
        pci_read_config_dword(pcidev, PCI_VENDOR_OFFSET, &scratch);
        if (scratch == RCC_VENDOR_ID) {
                *capPtr = 0x80;
                return 0;
        }

        /* If we're any chipset besides the POC RCC, we'll do it correctly...
         * First, do we even have AGP capability? */
        pci_read_config_dword(pcidev, PCI_STATUS_OFFSET, &scratch);
        if (!(scratch & 0x00100000)) {
                return -ENODEV;
        }

        /* If so, lets find the capability index (i.e. where the list of 
         * capPtr's are stored). */
        pci_read_config_byte(pcidev, PCI_CAP_PTR_OFFSET, capPtr);
        if (*capPtr != 0x00) {
                do {
                        /* Finally, we go searching for the AGP capPtr. */
                        pci_read_config_dword(pcidev, *capPtr, &scratch);

                        if ((scratch & PCI_CAP_ID_MASK) != PCI_AGP_CAP_ID)
                                *capPtr = (scratch >> 8) & PCI_CAP_ID_MASK;
                }
                while (((scratch & PCI_CAP_ID_MASK) != PCI_AGP_CAP_ID) &&
                       (*capPtr != 0x00));
        }
        if (*capPtr == 0x00) {
                return -ENODEV;
        }

        return 0;

}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
struct pci_dev *
pci_find_base_class(unsigned int class, struct pci_dev *from)
{
        DRM_DEBUG("class = 0x%0X\n", class);
	if (!from)
		from = pci_devices;
	else
		from = from->next;

	while (from && ((from->class & 0xFF0000) != class)) {
		from = from->next;
        }
	return from;
}
#else
static struct pci_dev *
pci_find_base_class(unsigned int class, struct pci_dev *from)
{
        DRM_DEBUG("class = 0x%0X\n", class);
	if (!from)
		from = pci_dev_g(pci_devices.next);
	else
		from = pci_dev_g(from->global_list.next);

	while (from && (from != pci_dev_g(&pci_devices))) {
		if ((from->class & 0xFF0000) == class) return(from);
	        from = pci_dev_g(from->global_list.next);
	}

	/* If here, we didn't get a hit. */
	return NULL;
}
#endif /* <2.4 else */

/* hpgfx_parse_option parses a single option.  See description for
   drm_parse_drm for details. */

static void hpgfx_parse_option(char *s)
{
	char *c, *r;
	
	if (!s || !*s) return;
	for (c = s; *c && *c != ':'; c++); /* find : or \0 */
	if (*c) r = c + 1; else r = NULL;  /* remember remainder */
	*c = '\0';			   /* terminate */
	if (!strcmp(s, "noctx")) {
		drm_flags |= DRM_FLAG_NOCTX;
		DRM_INFO("Server-mediated context switching OFF\n");
		return;
	}
	if (!strcmp(s, "debug")) {
		drm_flags |= DRM_FLAG_DEBUG;
		DRM_INFO("Debug messages ON\n");
		return;
	}
	if (!strcmp(s, "nodebug")) {
		drm_flags &= ~DRM_FLAG_DEBUG;
		DRM_INFO("Debug messages OFF\n");
		return;
	}
	DRM_ERROR("\"%s\" is not a valid option\n", s);
}

/* Replaces drm_parse_options, see drm/init.c for documentation
 * (replace drm= with hpgfx=) */
void hpgfx_parse_options(char *s)
{
	char *h, *t, *n;
	
	if (!s || !*s) return;

	for (h = t = n = s; h && *h; h = n) {
		for (; *t && *t != ';'; t++);	       /* find ; or \0 */
		if (*t) n = t + 1; else n = NULL;      /* remember next */
		*t = '\0';			       /* terminate */
		hpgfx_parse_option(h);		       /* parse */
	}
	DRM_DEBUG("\"%s\"\n", s ?: "");
}


/* This value (0x00000302) enables sideband addressing, agp operations
 * and agp2x (use 0x00000301 for agp1x) */
#define AGP_CONFIG_VALUE 0x00000302

/* Mask out all the config bits we don't care about. */
#define AGP_CONFIG_MASK  0x00000307

/* hpgfx_init is called via init_module at module load time, or via
 * linux/init/main.c (this is not currently supported). */
int hpgfx_init(void)
{
        int err = 0;
        uint8_t capPtr;
        struct pci_dev *pcidev = NULL;
        drm_device_t *drmdev;
        u32 pciVal = 0;
        unsigned int savefn;

        hpgfx_num_devs = 0;
#ifdef MODULE
	hpgfx_parse_options(hpgfx);
#endif
	DRM_DEBUG("\n");

#ifdef CONFIG_PCI /*[*/

        /* First, we'll initialize the AGP controler */
        if ((pcidev = pci_find_class(PCI_CLASS_BRIDGE_HOST << 8, NULL)) == NULL) {
                return -ENODEV;
        }
        DRM_DEBUG("Found a PCI bridge host: vendor=0x%X, device=0x%X, class=0x%X\n",
                  pcidev->vendor, pcidev->device, pcidev->class);

        if ((err = hpgfx_agp_cap_ptr(pcidev, &capPtr)) < 0) {
                DRM_DEBUG("    ... but can't find it's AGP capability pointer!  err=%d\n", err);
                return err;
        }

        /* HW_WORKAROUND
         * Another RCC chipset workaround...  For some reason, we have to set
         * the function field to 1 in order for this next write to work correctly
         * with the RCC chipset. */
        savefn = pcidev->devfn;
        if (capPtr == 0x80 /* RCC's hardcoded capPtr */) { 
                pcidev->devfn = 1;
        }
        pci_read_config_dword(pcidev, capPtr+8, &pciVal);
        DRM_DEBUG("Read \"0x%X\" from bridge capability register (0x%X)\n",
                  pciVal, capPtr+8);

        if ((pciVal & AGP_CONFIG_MASK) != AGP_CONFIG_VALUE) {
                pci_write_config_dword(pcidev, capPtr+8, (pciVal & 0xFFFFFCF8) | AGP_CONFIG_VALUE);
                DRM_DEBUG("Wrote \"0x%X\" to bridge capability register (0x%X)\n",
                         (pciVal & 0xFFFFFCF8) | AGP_CONFIG_VALUE , capPtr+8);
                /* For some reason, we have to read the register right back in order to
                 * get the previous write to work reliably. */
                pci_read_config_dword(pcidev, capPtr+8, &pciVal);
                DRM_DEBUG("Read \"0x%X\" back from bridge capability register (0x%X)\n",
                          pciVal, capPtr+8);
        }

        /* HW_WORKAROUND
         * Restore the devfn, just in case this is the RCC chipset. */
        pcidev->devfn = savefn;

        drm_mem_init();

        pcidev = NULL;
        /* Now we init all the HP display devices */
	while ((pcidev = pci_find_base_class(PCI_BASE_CLASS_DISPLAY << 16, pcidev)) != NULL) {
                int isAGP;

                DRM_DEBUG("Found a display: vendor=0x%X, device=0x%X, class=0x%X\n",
                          pcidev->vendor, pcidev->device, pcidev->class);

                if ((pcidev->vendor != PCI_VENDOR_ID_HP) ||
                     !(pcidev->device & HP_PCI_MODEL_FXX_MASK)) {
                        DRM_DEBUG("    ... but it's not an HP FX Card\n");
                        continue;
                }


                isAGP = 0;
                if ((err = hpgfx_agp_cap_ptr(pcidev, &capPtr)) < 0) {
                        DRM_DEBUG("    ... Hmm, must be a PCI FX card.  Disabling DMA...\n");
                        /* Enable memory access response on heathrow (just in case the bios
                         * got "smart" and disabled access to all displays excepth the
                         * VGA adapter. */
                        pci_write_config_dword(pcidev, PCI_COMMAND, PCI_COMMAND_MEMORY);
                }
                else {
                        isAGP = 1;
                        /* Since we've got an AGP card, also make Heathrow able to master
                         * the bus, so we can do DMA. */
                        pci_write_config_dword(pcidev, PCI_COMMAND,
                                               PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER);
                        /* Write the same value we wrote to the bridge (or'd with 0x0F000000
                         * for some unknown reason) */
                        pci_write_config_dword(pcidev, capPtr+8, AGP_CONFIG_VALUE | 0x0F000000);
                        /* Read it back, just for good measure. */
                        pci_read_config_dword(pcidev, capPtr+8, &pciVal);
                        DRM_DEBUG("Wrote \"0x%X\" to heathrow capability register (0x%X)\n",
                                  pciVal, capPtr+8);
                }

                /* Make sure we don't have more devices than the array holds! */
                REQUIRE(hpgfx_num_devs < HPGFX_MAX_DEVICES, return -ENOMEM);

                drmdev = &hpgfx_devices[hpgfx_num_devs];
                hpgfx_num_devs += 1;

                memset((void *)drmdev, 0, sizeof(*drmdev));
                drmdev->count_lock  = SPIN_LOCK_UNLOCKED;
                sema_init(&drmdev->struct_sem, 1);
                
                drmdev->misc.minor = MISC_DYNAMIC_MINOR;
                drmdev->misc.name = HPGFX_NAME;
                drmdev->misc.fops = &hpgfx_fops;
                if ((err = misc_register(&drmdev->misc))) {
                        DRM_ERROR("Cannot register \"%s\"\n", HPGFX_NAME);
                        return err;
                }
                drmdev->device = MKDEV(MISC_MAJOR, drmdev->misc.minor);
                drmdev->name   = HPGFX_NAME;
                drmdev->ioctls = hpgfx_ioctls;
                drmdev->num_ioctls = DRM_ARRAY_SIZE(hpgfx_ioctls);
                drmdev->isAGP = isAGP;

                /* Set up a unique identification string for this device (we'll use its
                 * pci bus, slot & function numbers). */
                if (err = setunique(drmdev, pcidev->bus->number, pcidev->devfn)) {
                        DRM_ERROR("Cannot set unique ID.\n");
                        return err;
                }

                drm_proc_init(drmdev);

                DRM_INFO("Initialized \"%s\" on major:minor <%d:%d>\n",
                         drmdev->devname, MISC_MAJOR, drmdev->misc.minor);

                if (drmdev->isAGP) {
                        err = hpgfx_init_ntdma();
                }
        }
#endif /*]*/

        DRM_DEBUG("Returning %d\n", err);
        return err;
}

/* hpgfx_cleanup is called via cleanup_module at module unload time. */

void hpgfx_cleanup(void)
{
        int i=0;

	DRM_DEBUG("\n");
        for (i=0; i<hpgfx_num_devs; i++)
        {
                drm_device_t *dev = &hpgfx_devices[i];

                /* Known bug.  This routine is called once for each device, but
                 * it cleans up the entire /proc/dri filesystem.  Doesn't seem to
                 * have any ill effects, but it's a possible problem. */
                drm_proc_cleanup(dev);

                DRM_DEBUG("calling misc_deregister(0x%0X)\n", &dev->misc);
                if (misc_deregister(&dev->misc)) {
                        DRM_ERROR("Cannot deregister device %d\n", i);
                        return;
                }
	
                if (dev->devname) {
                        DRM_DEBUG("calling drm_free(\"%s\", %d, DRM_MEM_DRIVER);\n",
                                  dev->devname, strlen(dev->devname)+1);
                        drm_free(dev->devname, strlen(dev->devname)+1, DRM_MEM_DRIVER);
                        dev->devname = NULL;
                }
                
                if (dev->unique) {
                        drm_free(dev->unique, strlen(dev->unique)+1, DRM_MEM_DRIVER);
                        DRM_DEBUG("calling drm_free(\"%s\", %d, DRM_MEM_DRIVER);\n",
                                  dev->unique, strlen(dev->unique)+1);
                        dev->unique = NULL;
                        dev->unique_len = 0;
                }

                hpgfx_takedown(dev);
        }
        /* Free the page tables */
        ntdma_free_all_pages(DRM_MEM_DMA);
}

#ifdef MODULE
#define MOD_USE_COUNT ( GET_USE_COUNT (&__this_module))
#endif				/* MODULE */
int hpgfx_reset_uc(struct inode *inode, struct file *filp, unsigned int cmd,
		   unsigned long arg)
{
        /* reset the usage counter to 1, to allow unloading of the module
         * in case of problems. */
        DRM_DEBUG("MOD_USE_COUNT = %d\n", MOD_USE_COUNT);

        while(MOD_USE_COUNT < 1)
            MOD_INC_USE_COUNT;

        while(MOD_USE_COUNT > 1)
            MOD_DEC_USE_COUNT;

        return 0;
}

int hpgfx_version(struct inode *inode, struct file *filp, unsigned int cmd,
		  unsigned long arg)
{
	drm_version_t version;
	int	      len;

	copy_from_user_ret(&version,
			   (drm_version_t *)arg,
			   sizeof(version),
			   -EFAULT);

#define DRM_COPY(name,value)				     \
	len = strlen(value);				     \
	if (len > name##_len) len = name##_len;		     \
	name##_len = strlen(value);			     \
	if (len && name) {				     \
		copy_to_user_ret(name, value, len, -EFAULT); \
	}

	version.version_major	   = HPGFX_MAJOR;
	version.version_minor	   = HPGFX_MINOR;
	version.version_patchlevel = HPGFX_PATCHLEVEL;

	DRM_COPY(version.name, HPGFX_NAME);
	DRM_COPY(version.date, HPGFX_DATE);
	DRM_COPY(version.desc, HPGFX_DESC);

	copy_to_user_ret((drm_version_t *)arg,
			 &version,
			 sizeof(version),
			 -EFAULT);
	return 0;
}

int hpgfx_open(struct inode *inode, struct file *filp)
{
	int	      retcode = -EINVAL, i = hpgfx_num_devs;
	drm_device_t  *dev = NULL;
        
        /* Find the right device.. */
        while (--i >= 0) {
                dev = &hpgfx_devices[i];
                if (dev->device == inode->i_rdev) {
                        /* Found it.  Now open it. */
                        DRM_DEBUG("Found device(%d,%d): open_count = %d\n",
                                  MAJOR(inode->i_rdev), MINOR(inode->i_rdev),
                                  dev->open_count);
                        if (!(retcode = drm_open_helper(inode, filp, dev))) {
                                MOD_INC_USE_COUNT;
                                atomic_inc(&dev->total_open);
                                spin_lock(&dev->count_lock);
                                if (!dev->open_count++) {
                                        spin_unlock(&dev->count_lock);
                                        return hpgfx_setup(dev);
                                }
                                spin_unlock(&dev->count_lock);
                        }
                        break;
                }
        }
	
	return retcode;
}

int hpgfx_release(struct inode *inode, struct file *filp)
{
	drm_file_t    *priv   = filp->private_data;
	drm_device_t  *dev    = priv->dev;
	int	      retcode = 0;

	DRM_DEBUG("open_count = %d\n", dev->open_count);
	if (!(retcode = drm_release(inode, filp))) {
		MOD_DEC_USE_COUNT;
		atomic_inc(&dev->total_close);
		spin_lock(&dev->count_lock);
                /* Only free for AGP DMA */
                if (dev->isAGP)
                    hpgfx_dma_free_buffers();
		if (!--dev->open_count) {
			if (atomic_read(&dev->ioctl_count) || dev->blocked) {
				DRM_ERROR("Device busy: %d %d\n",
					  atomic_read(&dev->ioctl_count),
					  dev->blocked);
				spin_unlock(&dev->count_lock);
				return -EBUSY;
			}
			spin_unlock(&dev->count_lock);
			return hpgfx_takedown(dev);
		}
		spin_unlock(&dev->count_lock);
	}
	return retcode;
}

/* drm_ioctl is called whenever a process performs an ioctl on /dev/drm. */

int hpgfx_ioctl(struct inode *inode, struct file *filp, unsigned int cmd,
		unsigned long arg)
{
	int		 nr	 = HPGFX_IOCTL_NR(cmd);
	drm_file_t	 *priv	 = filp->private_data;
	drm_device_t	 *dev	 = priv->dev;
	int		 retcode = 0;
	drm_ioctl_desc_t *ioctl;
	drm_ioctl_t	 *func;

	atomic_inc(&dev->ioctl_count);
	atomic_inc(&dev->total_ioctl);
	++priv->ioctl_count;
	
	DRM_DEBUG("pid = %d, cmd = 0x%02x, nr = 0x%02x, dev 0x%x, auth = %d\n",
		  current->pid, cmd, nr, dev->device, priv->authenticated);

	if (nr >= dev->num_ioctls) {
		retcode = -EINVAL;
	}
        else {
		ioctl	  = &dev->ioctls[nr];
		func	  = ioctl->func;

		if (!func) {
			DRM_DEBUG("no function\n");
			retcode = -EINVAL;
		}
#if 0 /* HACK!!! Don't authenticate at all.. [ */
                else if ((ioctl->root_only && !capable(CAP_SYS_ADMIN))
			    || (ioctl->auth_needed && !priv->authenticated))  {
			retcode = -EACCES;
		}
#endif /* ] */
                else {
			retcode = (func)(inode, filp, cmd, arg);
		}
	}
	
	atomic_dec(&dev->ioctl_count);

	return retcode;
}


int hpgfx_addmap(struct inode *inode, struct file *filp, unsigned int cmd,
	       unsigned long arg)
{
	drm_file_t	*priv	= filp->private_data;
	drm_device_t	*dev	= priv->dev;
        int             err;
	
        if ((err = drm_addmap(inode, filp, cmd, arg)) == 0) {
                drm_map_t *map = dev->maplist[dev->map_count-1];

                if (map && map->type == _DRM_REGISTERS && !dev->csaddr) {
                        /* Possible HACK!!  Found the config space address mapping,
                         * set the global.  Could break if more than one DRM_REGISTERS
                         * mapping (not likely, but...). */
                        DRM_DEBUG("Stowing away dev->csaddr=0x%08X\n", dev->csaddr);
                        dev->csaddr = (uintptr_t) map->handle;
                }
        }

        return err;
}
