/* vi:ts=8:sw=8:
 ******************************************************************************
 * hpgfx_dma.c
 * 
 * Linux DMA manager for Summit
 *
 * Original author of NT Dma Manager: Scott Ruff, HP
 *                     Linux Changes: Alan Ward, HP
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

#include "stddef.h"
#include "stdarg.h"

#if defined(DEBUG)
#define MARKER_TIMEOUT 1000
#else
/* A timeout of around 5 seconds before we check the hardware */
#define MARKER_TIMEOUT 0x7FF0000
#endif

#include "drm/drmP.h"
#include "hpgfx_drv.h"
#include "hpgfx_dma.h"

extern int hpgfx_mark_phys_mem_wrcomb(uintptr_t base, uintptr_t size);
extern void hpgfx_restore_mtrrs(int flags);

/* This macro creates a BBC_NOP packet of the given size.  Note the */
/* subtraction of one: this is deliberate.  The size given is the */
/* desired size of the whole packet, including header. */
#define MAKE_BBC_NOP(size) CREATE_CD_PACKET_HEADER( ((size)-1), 1, 0, B3_BBC_NOP )

#define ROUND_TO_PAGES(Size)  (((uint32_t)(Size) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
/* convert size to the number of pages able to contain  */
/* the data (round up if necessary) */
#define btopr(Size)             ((ROUND_TO_PAGES(Size))/PAGE_SIZE)


#define WAIT_AGP_CD_FFC(count, csAddr) { \
    volatile int _num; \
    DECLARE_DELAY_COUNTER(csAddr); \
    _num = MEM_READ32_OFFS(csAddr, UB_AGP_FFC); \
    _num &= 0x000000FF; \
    while( _num < count) { \
            HPGFX_DELAY(1); \
            _num = MEM_READ32_OFFS(csAddr, UB_AGP_FFC); \
            _num &= 0x000000FF; \
    } \
}

#define WAIT_AGP_UNBUSY(csAddr) { \
    volatile int _num; \
    DECLARE_DELAY_COUNTER(csAddr); \
    _num = MEM_READ32_OFFS(csAddr, UB_AGP_FFC); \
    _num &= 0x0000FF00; \
    while( _num != 0x0000F000 ) { \
            HPGFX_DELAY(1); \
            _num = MEM_READ32_OFFS(csAddr, UB_AGP_FFC); \
            _num &= 0x0000FF00; \
    } \
}

#define SOFTCD_WRITE32_OFFS(addr, reg, data) \
        *(addr++) = CREATE_CD_PACKET_HEADER(1, 0, 0, (reg)); \
        *(addr++) = (data);


/* MONDO HACKERAMA!  This macro is defined to work around the */
/* case where we can't get the marker for whatever reason. */
/* The basic algorithm is: if the hardware is unbusy, and the */
/* marker hasn't come back, hack that value into the marker */
/* slot... */

#ifndef CK_BDRDY_IN_WAIT_FOR_MARKER /* [ */

#define WAIT_FOR_MARKER(globalMarker, marker, csAddr) do { \
        int i = 0; \
        while (globalMarker < marker) { \
                if (++i >= MARKER_TIMEOUT) { \
                        DRM_DEBUG("Marker %d never came back!  Polling hardware...\n", marker); \
                        WAIT_AGP_UNBUSY(csAddr); \
                        WAIT_WPNE(csAddr); \
                        globalMarker = marker; \
                        break; \
                } \
        } \
} while(0)

#else /* ] [ */

/*
 * This version of WAIT_FOR_MARKER also checks the bdrdy bits in the status register.
 * If they are set, it reads data until the bdrdy bits aren't set.
 * USED ONLY FOR DEBUGGING.
 */

#define ALL_ZEROS 0
typedef unsigned int  Uint32;
#define BITFIELD_3(a0,a1,a2) a2;a1;a0;
typedef union {
    struct {
        BITFIELD_3(
        Uint32  unused1         : 29,
        Uint32  op              :  2,
        Uint32  WTR             :  1
        )
    } fields;
    Uint32  bits;
} IRC_Type;
 
#define PDU_IRC_NOOP    0
#define PDU_IRC_IREAD   1
#define PDU_IRC_CLEAR   2
#define PDU_IRC_SET     3

int bdrdy;

#define WAIT_FOR_MARKER(globalMarker, marker, csAddr) do
{ \
    int i = 0; \
    while (globalMarker < marker) { \
        if (++i >= MARKER_TIMEOUT) { \
	    DRM_DEBUG("Marker %d never came back!  Polling hardware...\n", marker); \
	    DRM_DEBUG("Global marker is %d\n", globalMarker); \
	    WAIT_AGP_UNBUSY(csAddr); \
	    do { \
		volatile uint32_t status; \
		int cnt = 0; \
		status = MEM_READ32_OFFS((csAddr), UB_STATUS); \
		DRM_DEBUG("   UB_STATUS = %08x\n", status); \
		if (status & 0x3000000) { \
		    IRC_Type Irc; \
		    Irc.bits = ALL_ZEROS; \
		    Irc.fields.op = PDU_IRC_CLEAR; \
		    bdrdy = 1; \
		    MEM_WRITE32_OFFS((csAddr), B2_IRC, Irc.bits); \
		    DRM_DEBUG("WAIT_WPNE:BDRDY was set!\n"); \
		} \
		while (status &0x30000000) { \
		    DRM_DEBUG("   Data: %08x\n", MEM_READ32_OFFS((csAddr),B2_READ_DATA)); \
		    cnt++; \
		    status = MEM_READ32_OFFS((csAddr), UB_STATUS); \
		    DRM_DEBUG("   UB_STATUS = %08x\n", status); \
		} \
		DRM_DEBUG("   Num words read: %d\n", cnt); \
		if (!(status & 0x01000000)) \
		    break; \
	    } while (1); \
	    globalMarker = marker; \
	    break; \
	} \
    } \
} while(0)

#endif /* ] */


/******************************************************************************
 *
 * Logging buffer
 *
 ******************************************************************************/

char logBuffer[LOGBUFFERSIZE];
int lbIdx = 0;
int lbOverflow = 0;
spinlock_t logBuffer_lock = SPIN_LOCK_UNLOCKED;

#define HPGFX_LOG(fmt, arg...) do				\
{								\
    char fs[80];						\
    if (!lbOverflow) {						\
	sprintf(fs, fmt, ##arg); 				\
	spin_lock(&logBuffer_lock);				\
	if (lbIdx + strlen(fs) > LOGBUFFERSIZE-32) {		\
								\
	    /* Logging buffer almost full */			\
								\
	    /* Add "OVERFLOW" to logging buffer.   */		\
	    /* We set a flag so performance is not */		\
	    /* affected when buffer is full.       */		\
	   if (lbIdx <= LOGBUFFERSIZE-12) {			\
		strcpy(&logBuffer[lbIdx], "OVERFLOW\n");	\
		lbIdx += strlen("OVERFLOW\n");			\
		lbOverflow = 1;					\
	   }							\
	} else {						\
	    /* Append fs to logging buffer. */			\
	    strcpy(&logBuffer[lbIdx], fs);			\
	    lbIdx += strlen(fs);				\
	}							\
	spin_unlock(&logBuffer_lock);				\
    }								\
} while (0)


/******************************************************************************
 *
 *   DMA Buffer Allocation Utils
 *
 ******************************************************************************/

#define SIXTEEN_PAGES           (0x10000)
#define EIGHT_PAGES             (0x08000)
#define DMA_ENABLE_BIT 0x1

/* HACK These can't be globals if we're going to support DMA on more than
 * one adapter at a time.  */
static void *pool_writeCombinedCommonBufferVirtual = NULL;
static PHYSICAL_ADDRESS pool_writeCombinedCommonBufferLogical;
static int pool_writeCombinedPageCount = 0;
static void *pool_cacheableCommonBufferVirtual = NULL;
static PHYSICAL_ADDRESS pool_cacheableCommonBufferLogical;
static int pool_cacheablePageCount = 0;
static void *pool_bufstart = 0;
static void *pool_bufend = 0;

static int AGP_2X = 0;
static hpgfx_DmaManagerPrivate *sDMAMgr = NULL;
static int sDMAInited = 0;

static hpgfx_DmaPrivateBuff *sBuffList = NULL;

/***********************************************************************************
 * ntdma_alloc_pages
 *
 * allocation routine used to get large contiguous buffers, by making
 * consecutive calls to drm_alloc_pages which only allows 128k size.
 * Each buffer returned from drm_alloc_pages is checked to see if
 * it is contiguous with the previous one.  Calls with a size < 128k
 * will be rounded up to 128k.
 *
 ***********************************************************************************/

/* Maximum number of page allocs to try before giving up. */
#define MAX_PAGE_ALLOCS 4096

/* This is the maximum size that the kernel will allow us to allocate in
 * one chunk. (taken from page_alloc.c) */
#define MAX_PAGE_ORDER 5
#define MAX_NUM_PAGES (1U<<MAX_PAGE_ORDER)
#define MAX_PAGE_ALLOC_SIZE (MAX_NUM_PAGES * PAGE_SIZE)

static uintptr_t sFreeList[MAX_PAGE_ALLOCS];
static int sTotalAllocs = 0;

uintptr_t ntdma_alloc_pages(size_t byteSize, size_t alignment, hpgfx_DmaAllocType allocType)
{
	uintptr_t retval = 0;
	int sizeOrder, pageOrder, numAllocsNeeded;
	int localTotalAllocs = sTotalAllocs;

	DRM_DEBUG("ARGS: byteSize =%d, alignment=%d, allocType=%d\n", byteSize, alignment, allocType);

	sizeOrder = drm_order(byteSize);
	pageOrder = (sizeOrder - PAGE_SHIFT > 0) ? sizeOrder - PAGE_SHIFT : 0;
	numAllocsNeeded = ((1U << pageOrder) / (1U << MAX_PAGE_ORDER)) + 1;

	DRM_DEBUG("sTotalAllocs=%d, sizeOrder=%d, pageOrder=%d, numMallocsNeeded=%d\n",
		  sTotalAllocs, sizeOrder, pageOrder, numAllocsNeeded);
	while (sTotalAllocs < MAX_PAGE_ALLOCS) {
		uintptr_t buf, lastBuf;
		int numAllocs = 1;

		buf = sFreeList[sTotalAllocs++] = drm_alloc_pages(MAX_PAGE_ORDER, allocType);
		if (!buf) {
			sTotalAllocs--;
			DRM_DEBUG("D'oh!  Call to first drm_alloc_pages(%d, %d) failed!  sTotalAllocs=%d\n",
				MAX_PAGE_ORDER, allocType, sTotalAllocs);
			break;
		}
		do {
			lastBuf = buf;
			if ((numAllocs >= numAllocsNeeded) && ((alignment ? (buf % alignment) : 0) == 0)
#if 1 /* DEFERRED HACK -- don't allow the buffer address to interfere with the frame buffer mapping. */
                            && (buf<0xF0000000 || buf>0xFE000000)
#endif
                            ) {
				DRM_DEBUG("Wuhoo!  Got the memory we needed!  numAllocs=%d, sTotalAllocs=%d\n",
					  numAllocs, sTotalAllocs);
				retval = buf;
				break;
			}
			buf = sFreeList[sTotalAllocs++] = drm_alloc_pages(MAX_PAGE_ORDER, allocType);
			if (!buf) {
				sTotalAllocs--;
				DRM_DEBUG("D'oh!  Call to drm_alloc_pages(%d, %d) failed!"
					  "  numAllocs=%d, sTotalAllocs=%d\n", MAX_PAGE_ORDER,
				     allocType, numAllocs, sTotalAllocs);
				break;
			}
			numAllocs++;

#if 0
			DRM_DEBUG("numAllocs=%d, sTotalAllocs=%d, buf=0x%0X, lastBuf=0x%0X\n",
				  numAllocs, sTotalAllocs, buf, lastBuf);
#endif

		} while ((buf + MAX_PAGE_ALLOC_SIZE) == lastBuf);

		if (retval) {
			break;
		}
	}
	if (retval) { /* Success! Free the unused "alignment" pages. */
		int i;

		DRM_DEBUG("Freeing pages from sFreeList[%d..%d]\n", localTotalAllocs,
			  sTotalAllocs - numAllocsNeeded - 1);

		for (i = localTotalAllocs; i < (sTotalAllocs - numAllocsNeeded); i++) {
			if (sFreeList[i]) {
				drm_free_pages(sFreeList[i], MAX_PAGE_ORDER, allocType);
				sFreeList[i] = 0;
			}
		}
	}

	DRM_DEBUG("returning 0x%0X\n", retval);

	return retval;
}

static int mtrr_flags = -1;
/***********************************************************************************
 * ntdma_free_all_pages
 *
 * free routine used to release the large contiguous buffers, created by
 * ntdma_alloc_pages which was composed of consecutive contiguous calls
 * to drm_alloc_pages.  drm_free_pages is called once to free all pages.
 *
 ***********************************************************************************/
int ntdma_free_all_pages(hpgfx_DmaAllocType allocType)
{
	int i;

	DRM_DEBUG("Freeing pages from sFreeList[0..%d]\n", sTotalAllocs - 1);

	for (i = 0; i < sTotalAllocs; i++) {
		if (sFreeList[i]) {
			drm_free_pages(sFreeList[i], MAX_PAGE_ORDER, allocType);
			sFreeList[i] = 0;
		}
	}
        /* Reset all the globals, just in case we come back in without being reloaded. */
	sTotalAllocs = 0;
        pool_writeCombinedCommonBufferVirtual = NULL;
        pool_writeCombinedCommonBufferLogical;
        pool_writeCombinedPageCount = 0;
        pool_cacheableCommonBufferVirtual = NULL;
        pool_cacheableCommonBufferLogical;
        pool_cacheablePageCount = 0;
        pool_bufstart = 0;
        pool_bufend = 0;

	/* We'll also free the DmaManager object here... */
        if (sDMAMgr) {
		FREE_KERNEL_MEM(sDMAMgr->writeCombinedPageTable,
                                sizeof(hpgfx_DmaPage) * sDMAMgr->writeCombinedPageCount);
		FREE_KERNEL_MEM(sDMAMgr->cacheablePageTable,
                                sizeof(hpgfx_DmaPage) * sDMAMgr->cacheablePageCount);
		FREE_KERNEL_MEM(sDMAMgr, sizeof(hpgfx_DmaManagerPrivate));
        }
        sDMAMgr = NULL;

        /* This is a suckful place to put this, but it'll have to do... */
        if (mtrr_flags >= 0) {
                hpgfx_restore_mtrrs(mtrr_flags);
        }
}

int hpgfx_create_dma_manager(void);

/***********************************************************************************
 * hpgfx_init_ntdma
 * 
 * ntdma initialization called when the module is first loaded.
 * Does dma buffer allocation.
 *
 ***********************************************************************************/
int hpgfx_init_ntdma()
{
	int err = 0;

        /* This routine is only called once (at module load time). */
        REQUIRE(pool_writeCombinedCommonBufferVirtual == NULL, return -EINVAL);

	DRM_DEBUG("\n");
        err = allocate_dma_buffers(&pool_writeCombinedCommonBufferVirtual,
                                   &pool_writeCombinedCommonBufferLogical.LowPart,
                                   &pool_writeCombinedPageCount,
                                   &pool_cacheableCommonBufferVirtual,
                                   &pool_cacheableCommonBufferLogical.LowPart,
                                   &pool_cacheablePageCount, &pool_bufstart,
                                   &pool_bufend);
	if (!err) {
		err = hpgfx_create_dma_manager();
	}
	return err;
}


/***********************************************************************************
 * get_dma_pool_ptrs
 * 
 * called during initialization of each dma manager (once per context)
 * Puts copy of dma buffer pointers (from dma memory pool) into 
 * dma manager private area(dmaMgr).
 *
 ***********************************************************************************/
int get_dma_pool_ptrs(hpgfx_DmaManagerPrivate * dmaMgr)
{
	int err;

        REQUIRE((dmaMgr != NULL) && (dmaMgr != (void *)0xdeadbeef), return -EINVAL);
	REQUIRE(pool_writeCombinedCommonBufferVirtual &&
                pool_cacheableCommonBufferVirtual, return -ENOMEM);

	DRM_DEBUG("\n");
	dmaMgr->writeCombinedCommonBufferVirtual = pool_writeCombinedCommonBufferVirtual;
	dmaMgr->writeCombinedCommonBufferLogical = pool_writeCombinedCommonBufferLogical;
	dmaMgr->writeCombinedPageCount = pool_writeCombinedPageCount;

	dmaMgr->cacheableCommonBufferVirtual = pool_cacheableCommonBufferVirtual;
	dmaMgr->cacheableCommonBufferLogical = pool_cacheableCommonBufferLogical;
	dmaMgr->cacheablePageCount = pool_cacheablePageCount;
	return 0;
}


void get_ntdma_pool_range(uintptr_t * bufstart, uintptr_t * bufend)
{
    	*bufstart = (uintptr_t) pool_bufstart;
	*bufend = (uintptr_t) pool_bufend;
}


/***********************************************************************************
 * attempt_dma_allocate
 *
 * This function returns two address spaces if successful. One is write combined, 
 * and the other is cacheable. The write combined chunk is used for CD buffers. 
 * The cacheable chunk is used for any DMA operations from device to main memory.
 *
 ***********************************************************************************/
static int attempt_dma_allocate( unsigned long size,
                                 void **WCAddr,
                                 unsigned long *WCLogical,
                                 unsigned long *WCPageCount,
                                 void **cacheAddr,
                                 unsigned long *cacheLogical,
                                 unsigned long *cachePageCount,
                                 void **bufstart,
                                 void **bufend)
{
	uintptr_t wc_pool, cached_pool;

	DRM_DEBUG("\n");

	/* First set up the main chunk of write combined memory. */
	if (!(wc_pool = ntdma_alloc_pages(size, size, DRM_MEM_DMA))) {
		DRM_DEBUG("ntdma_alloc_pages(%d, %d, %d) failed for wc_pool!\n",
			  size, size, DRM_MEM_DMA);
		return -ENOMEM;
	}
	pool_bufstart = (void *) wc_pool;
	pool_bufend = (void *) (wc_pool + size);

	DRM_DEBUG("pool_bufstart=0x%0X, pool_bufend=0x%0X\n", pool_bufstart, pool_bufend);

	*WCAddr = (void *) wc_pool;
	*WCLogical = virt_to_phys((void *) wc_pool);
	*WCPageCount = size / PAGE_SIZE;

	/* Now set up the cached memory for incoming DMA writes (from the device) */
	if (!(cached_pool = ntdma_alloc_pages(SIXTEEN_PAGES, 0, DRM_MEM_DMA))) {
		DRM_DEBUG("ntdma_alloc_pages(%d, %d, %d) failed for cached pool!\n",
			  size, size, DRM_MEM_DMA);
		ntdma_free_all_pages(DRM_MEM_DMA);
		return -ENOMEM;
	}
	*cacheLogical = virt_to_phys((void *) cached_pool);
	*cacheAddr = (void *) cached_pool;
	*cachePageCount = SIXTEEN_PAGES / PAGE_SIZE;

	/* Finally, map the WC memory to be WRITE combined */
        mtrr_flags = hpgfx_mark_phys_mem_wrcomb(*WCLogical, *WCPageCount * PAGE_SIZE);
	if (mtrr_flags < 0) {
		DRM_DEBUG("Can't allocate mtrrs! failed with err=%d!\n", mtrr_flags);
		ntdma_free_all_pages(DRM_MEM_DMA);
		return mtrr_flags;
	}

	return 0;
}


/***********************************************************************************
 * allocate_dma_buffers
 *
 * This function calls attempt_dma_allocate with different sizes until it succeeds
 *
 ***********************************************************************************/
int allocate_dma_buffers(void **WCAddr,
                         unsigned long *WCLogical,
                         unsigned long *WCPageCount,
                         void **cacheAddr,
                         unsigned long *cacheLogical,
                         unsigned long *cachePageCount,
                         void **bufstart,
                         void **bufend)
{
	char buff[256];
	int n, err;
	unsigned long size, total, order;

	*WCAddr = NULL;
	*WCLogical = 0;
	*WCPageCount = 0;

	*cacheAddr = NULL;
	*cacheLogical = 0;
	*cachePageCount = 0;

	DRM_DEBUG("\n");

        /* Make the buffer size a function of the amount of physical RAM we have,
         * with a maximum size of 32M, and a minimum size of 4M.  Also, we want
         * to keep the size on a power of two boundary (i.e. 4M, 8M, 16M or 32M),
         * so as to not waste mtr registers later. */
        total = max_mapnr << PAGE_SHIFT; /* Total physical RAM. */

        /* Align the size to a power of two. */
        /* 64 BIT WARNING! 32 bit memory size hardcode! */
        order = 31;
        while ((order > 0) && 
               ((total >> order) <= 0)) {
            order--;
        }

        order -= 5; /* Take at most 1/32 of physical RAM */

        /* Don't take less than 4M. */
        if (order < 22) { /* 4M */
            order = 22;
        }
        
	size = 0x1 << order;
        DRM_DEBUG("Allocating %dMB dma buffer\n", size >> 20);

	err = attempt_dma_allocate(size, WCAddr, WCLogical, WCPageCount, cacheAddr,
			 cacheLogical, cachePageCount, bufstart, bufend);
	return err;
}


int DmaAllocBuff(hpgfx_DmaManager * dmaMgr, hpgfx_DmaAllocType allocType, unsigned long byteCount,
                 hpgfx_DmaBuff ** buff);

int hpgfx_create_dma_manager(void)
{
	hpgfx_DmaManagerPrivate *dmaMgr;
	DMA_ERROR result;
	unsigned int pageCount;

	DRM_DEBUG("\n");

	/* Allocate the DmaManagerObject */
	if (!(dmaMgr = ALLOC_KERNEL_MEM(sizeof(hpgfx_DmaManagerPrivate))))
		goto noMemFailure;

	/* Initialize dmaMgr */
	dmaMgr->writeCombinedPageTable = NULL;
	dmaMgr->cacheablePageTable = NULL;
	dmaMgr->MarkerCounter = 1;/*Initializing to 1 prevents having to wrap the first time */
	dmaMgr->pendingListStart = NULL;
	dmaMgr->pendingListEnd = NULL;
	dmaMgr->csAddr = NULL;	/* For now...
                                   Will initialize when called by client for the first time. */

	/* Use the same CD buffer size that ring3 DSM uses + 1. We add an extra page so we  */
	/* always have enough room to append the marker packets to the end of the buffer. */
	dmaMgr->CDBuffPageCount = CD_BUFFER_MAX / PAGE_SIZE + 1;

	get_dma_pool_ptrs(dmaMgr);

	/* Allocate memory for write combined memory allocation page table, and initialize */
	dmaMgr->writeCombinedPageTable = (hpgfx_pDmaPage) ALLOC_KERNEL_MEM(sizeof(hpgfx_DmaPage) *
					 dmaMgr->writeCombinedPageCount);
	if (!dmaMgr->writeCombinedPageTable)
		goto noMemFailure;

	for (pageCount = 0; pageCount < dmaMgr->writeCombinedPageCount; pageCount++) {
		dmaMgr->writeCombinedPageTable[pageCount].associatedPointer = NULL;
	}

	/* Allocate memory for the Cacheable memory allocation page table, and initialize */

	dmaMgr->cacheablePageTable = (hpgfx_pDmaPage) ALLOC_KERNEL_MEM(sizeof(hpgfx_DmaPage) *
					     dmaMgr->cacheablePageCount);
	if (!dmaMgr->cacheablePageTable)
		goto noMemFailure;

	for (pageCount = 0; pageCount < dmaMgr->cacheablePageCount; pageCount++) {
		dmaMgr->cacheablePageTable[pageCount].associatedPointer = NULL;
	}

	/* Now that memory manager (Trival as it is) is initialized, we call DmaAllocBuff */
	/* to allocate DMA memory which we use to store the global marker. */
	result = DmaAllocBuff((hpgfx_DmaManager *) dmaMgr, DMA_HOST_BUFFER,
                              sizeof(hpgfx_DmaCacheableUtilityPageStruct),
			      &(dmaMgr->DmaCacheableUtilityPage));

	DRM_DEBUG("dmaMgr->DmaCacheableUtilityPage->virtualAddress=0x%0X\n",
		  dmaMgr->DmaCacheableUtilityPage->virtualAddress);

	if (result < 0)
		goto noMemFailure;

	/* Call DmaAllocBuff to allocate DMA memory for CD buffers we use to setup DMA */
	/* marker writes to the host.                                                  */

	result = DmaAllocBuff((hpgfx_DmaManager *) dmaMgr, DMA_CD_BUFFER,
			      sizeof(hpgfx_DmaWriteCombinedUtilityPageStruct),
			      &(dmaMgr->DmaWriteCombinedUtilityPage));

	DRM_DEBUG("dmaMgr->DmaWriteCombinedUtilityPage->virtualAddress=0x%0X\n",
		  dmaMgr->DmaWriteCombinedUtilityPage->virtualAddress);

	if (result < 0)
		goto noMemFailure;

	/* Allocate DMA memory for general DMA writes. */
        result = DmaAllocBuff((hpgfx_DmaManager *) dmaMgr, DMA_HOST_BUFFER, EIGHT_PAGES,
	                      &dmaMgr->dmaWrite_writeBuffptr);
	if (result < 0)
	    goto noMemFailure;
	DRM_DEBUG("dmaMtr->writeBuffptr->virtualAddress = %08x\n",
	          dmaMgr->dmaWrite_writeBuffptr->virtualAddress);

        result = DmaAllocBuff((hpgfx_DmaManager *) dmaMgr, DMA_CD_BUFFER, 4096,
	                      &dmaMgr->dmaWrite_readBuffptr);
	if (result < 0)
	    goto noMemFailure;
	DRM_DEBUG("dmaMtr->readBuffptr->virtualAddress = %08x\n",
	          dmaMgr->dmaWrite_readBuffptr->virtualAddress);

	/* Init the global marker to 0 */
	((hpgfx_DmaCacheableUtilityPageStruct *)
         (dmaMgr->DmaCacheableUtilityPage->virtualAddress))->globalMarker = 0;
	dmaMgr->markerSetupSlot = 0;

	/* Mark BINC as not running */
	dmaMgr->bincActive = 0;

	/* Create CD packets that will be used to cause the global marker to be written. */
	/* These packets will be sent to the hardware after each DMA operation, as part of */
	/* the flow control algorithm. */
	createMarkerWriteCDPackets((hpgfx_DmaManager *) dmaMgr);

	sDMAMgr = dmaMgr;

        ENSURE(sDMAMgr != NULL, return -ENOMEM);

	return 0;
	
noMemFailure:
	if (dmaMgr) {
	    if (dmaMgr->writeCombinedPageTable)
	        FREE_KERNEL_MEM(dmaMgr->writeCombinedPageTable,
                                sizeof(hpgfx_DmaPage) * dmaMgr->writeCombinedPageCount);
	    if (dmaMgr->cacheablePageTable)
		FREE_KERNEL_MEM(dmaMgr->cacheablePageTable,
                                sizeof(hpgfx_DmaPage) * dmaMgr->cacheablePageCount);
	    FREE_KERNEL_MEM(dmaMgr, sizeof(hpgfx_DmaManagerPrivate));
	}
	return -ENOMEM;

}

/***********************************************************************************
 * hpgfx_dma_initialize
 *
 * This function is an ioctl function implementing the dmaManager call "dmaInitialize"
 * to setup dma for current graphics context.  (See dmaManager design doc)
 *
 ***********************************************************************************/
int hpgfx_dma_initialize(struct inode *inode, struct file *filp, unsigned int cmd,
			 unsigned long arg)
{
	drm_file_t *priv = filp->private_data;
	drm_device_t *dev = priv->dev;
	hpgfx_DmaIoctlData iodata;
	hpgfx_DmaManagerPrivate *dmaMgr = sDMAMgr;
	DMA_ERROR result;
	unsigned int pageCount;
	int retCount;
        u32 regVal;
	volatile u32 csAddr;

	DRM_DEBUG("\n");

	copy_from_user_ret(&iodata, (hpgfx_DmaIoctlData *) arg, sizeof(iodata), -EFAULT);

        if (dev->isAGP) {
                REQUIRE(dmaMgr != NULL, return -EFAULT);
                REQUIRE(dev->csaddr != 0, return -EFAULT);
	
                dmaMgr->csAddr = (volatile unsigned long *) csAddr = dev->csaddr;
                
	        /*
	         * Wait for any pending DMAs to finish because we are about to
		 * reset a bunch of DMA registers.
		 */
		DmaFinishAll(dmaMgr);

                /* Set up the Fault Control (BP_FC) Register. */
                /* Bits set are as follows: */
                /*  0x000800        CD Read Abort Enable */
                /*  0x000200        Direct Draw Abort Enable */
                /*  0x000040        Frame Buffer Is Direct Draw */
                /*  0x000010        AGP 64 */
                /* Bits NOT set are as follows: */
                /*  0x800000        Indirect Read No Wait */
                /*  0x400000        Ignore Buffered */
                /*  0x100000        Parity Error Signal Enable */
                /*  0x001000        Timeout Circuit Enabled */
                /*  0x000100        High Water Abort Enable */
                /*  0x000400        Writes Wait For Reads Enable */
                /*  0x000080        Allow Secondary Writes Enable */
                /*  0x000020        Direct Draw Ignore Enable */
                /* All other bits are reserved and written to 0 */
                /* We will toggle the Direct Draw Ignore Enable bit when direct */
                /* draw goes active/inactive. */
                MEM_WRITE32_OFFS(csAddr, BP_FC, 0x00000A50);

                /* Setup DMA TO HOST registers. We do this with PIO because these registers can't  */
                /* be setup through a CD packet. We initialize the target and limit registers */
                /* to the first 64K of our DMA memory pool. This means that all CD packets which */
                /* setup DMA TO HOST operations must use the offset register to specify the */
                /* memory location. Also, the memory location must fall within the first 64K of */
                /* the memory pool */
                MEM_WRITE32_OFFS(csAddr, BP_DMA_UPPER32, 0);
                MEM_WRITE32_OFFS(csAddr, BP_DMA_TARGET,
                           dmaMgr->cacheableCommonBufferLogical.LowPart | 0x001);
                MEM_WRITE32_OFFS(csAddr, BP_DMA_LIMIT,
                                    (dmaMgr->cacheableCommonBufferLogical.LowPart + 64 * 1024) | 0x001);

                /* These are possibly the right values */
                MEM_WRITE32_OFFS(csAddr, BP_CD_HWC, 0x004);
                MEM_WRITE32_OFFS(csAddr, BP_CF_HWC, 0x110);
                MEM_WRITE32_OFFS(csAddr, BP_CF_DRC, 0x133);

                MEM_WRITE32_OFFS(csAddr, UP_DC, 0x00030301);

                MEM_WRITE32_OFFS(csAddr, BP_DMA_OFFSET, 0);

                /* HW_WORKAROUND [ */
                /*  Figure out if the miniport set us up for AGP 1X or AGP 2X. */
                /*  This is so we can minimize the number of NOPs we insert into */
                /*  the pipeline in setup_dma_engine, below. */
                regVal = MEM_READ32_OFFS(csAddr, UB_AGP_CMD);
                AGP_2X = 0;
                if (regVal & 0x00000002) {
                        AGP_2X = 1;
                }
                else {
                        DRM_DEBUG("Running AGP_1X\n");
                }
                /* ] HW_WORKAROUND */

                /* Setup for AGP DMA reads from host.  This write has the side effect of
                 * clearing the LOWER & OFFSET registers as well. */
                MEM_WRITE32_OFFS(csAddr, BP_AGP_BDMA_UPPER, 0);

                /* Initialize AGP BINC reader UPPER */
                MEM_WRITE32_OFFS(csAddr, UP_AGP_UDMA_UPPER, 0);

                iodata.dmaMgr = dmaMgr;

                sDMAInited = 1;
        }
        else {
                iodata.dmaMgr = (void *)0xdeadbeef;
        }
	copy_to_user_ret((hpgfx_DmaIoctlData *) arg, &iodata, sizeof(iodata), EFAULT);


	return 0;
}


/* Since the DMA_TARGET register cannot be setup through a CD packet, it is setup */
/* to point to the begining of the DMA memory pool. The following defines the number of */
/* pages that can be DMAed to. For summit hardware, this value is 16 */

#define WRITE_TO_HOST_PAGE_COUNT 16

/***********************************************************************************
 * DmaAllocBuff
 *
 * This function implements the dmaManager call "dmaAllocBuff" to do needed allocation
 * for a dmaManager dma buffer for the current graphics context.  It is also called
 * internally for initial buffer allocation, so hpgfx_DmaAllocBuff is the ioctl 
 * function that is just a wrapper to this function where the real work is done.
 * (See dmaManager design doc)
 *
 ***********************************************************************************/
int DmaAllocBuff(hpgfx_DmaManager * dmaMgr, hpgfx_DmaAllocType allocType, unsigned long byteCount,
		 hpgfx_DmaBuff ** buff)
{
	hpgfx_DmaManagerPrivate *_dmaMgr = ((hpgfx_DmaManagerPrivate *) dmaMgr);
	hpgfx_DmaPrivateBuff *_buff;
	unsigned int contiguousPages = 0;
	unsigned int pageCount;
	int found = FALSE;
	unsigned long physicalAddressLow;
	unsigned long hostDmaOffset;
	void *startingAddr;
	int tmp;

	DRM_DEBUG("\n");
	pageCount = btopr(byteCount);

	switch (allocType) {

	case DMA_HOST_BUFFER:
        {
                unsigned long pageNum;
                uintptr_t ccBuff = (uintptr_t) _dmaMgr->cacheableCommonBufferVirtual;

                DRM_DEBUG("Got DMA_HOST_BUFFER type. \n");
                /* Allocate the DmaBuff structure */
                _buff = (hpgfx_DmaPrivateBuff *) ALLOC_KERNEL_MEM(sizeof(hpgfx_DmaPrivateBuff));
                if (!_buff)
                        return -ENOMEM;

                for (pageNum = 0; pageNum < WRITE_TO_HOST_PAGE_COUNT; pageNum++) {
                        /* Search the page table until a contiguous block of memory is found */
                        /* Which is large enough to hold the buffer */
                        if (_dmaMgr->cacheablePageTable[pageNum].associatedPointer == NULL) {
                                contiguousPages++;
                                DRM_DEBUG("Cacheable free page: contiguousPages=%d, pageNum=%d, pageAddr=0x%0X\n",
                                          contiguousPages, pageNum, ccBuff + PAGE_SIZE * pageNum);
                        }
                        else {
                                contiguousPages = 0;
                        }

                        /* If space is found, mark all the pages as used */
                        if (contiguousPages == pageCount) {
                                long reservedPages;

                                /* Calculate starting address for the allocated memory */
                                startingAddr = (void *) (ccBuff + PAGE_SIZE * (pageNum - pageCount + 1));

                                DRM_DEBUG("Found enough contigious cacheable pages (%d).  Marking them as used.\n"
                                          "startingAddr=0x%0X\n", contiguousPages, startingAddr);

                                /* Mark pages as used */
                                for (reservedPages = pageNum;
                                     reservedPages > (long) pageNum - (long) pageCount;
                                     reservedPages--) {
                                        _dmaMgr->cacheablePageTable[reservedPages].associatedPointer = startingAddr;

                                }

                                found = TRUE;
                                break;
                        }
                }
                if (found) {
                        _buff->publicBuff.virtualAddress = startingAddr;
                        _buff->physAddress.LowPart = virt_to_phys(startingAddr);
                        _buff->publicBuff.copyBuffer = FALSE;
                        /* If allocating out of our own contigous pool, this is the buffer offset into the pool */
                        _buff->hostDmaOffset = ((uintptr_t) startingAddr) - ((uintptr_t) ccBuff);
                }
                else {
                        DRM_DEBUG("D'oh!  Can't find %d contiguous pages.\n", pageCount);
                        _buff->physAddress.LowPart = 0;
                        _buff->hostDmaOffset = 0;
                        _buff->publicBuff.virtualAddress = (void *)0xdeadbeef;
                        _buff->publicBuff.copyBuffer = TRUE;
                }

                _buff->marker = 0;
                _buff->physAddress.HighPart = 0;
                _buff->pageCount = pageCount;
                _buff->nextBuff = NULL;
                _buff->prevBuff = NULL;
                _buff->pending = 0;
                _buff->appendMarker = 0;
                _buff->bufferType = allocType;
                _buff->publicBuff.byteCount = byteCount;

                _buff->publicBuff.userAddress = NULL;
                _buff->publicBuff.kernelPrivate = _buff;

                /* Insert into the global list of buffers. */
                _buff->pid = current->pid;
                _buff->next = sBuffList;
                _buff->prev = NULL;
                if (sBuffList) {
                        sBuffList->prev = _buff;
                }
                sBuffList = _buff;

                /* Return buff structure */
                *buff = (hpgfx_DmaBuff *) _buff;
                DRM_DEBUG("Allocated host buffer successfuly. \n");
                return 0;
        }

	case DMA_CD_BUFFER:
        {
                unsigned long pageNum;
                uintptr_t wcBuff = (uintptr_t) _dmaMgr->writeCombinedCommonBufferVirtual;

                /* Adjust byteCount to account for padding and marker return */
                /* appending */
#if 0 /* HACK?? Do I really need this? */
                byteCount += 32;	/* 16 bytes for marker return, 32 for padding */
#endif

                /* Recalculate the page count */
                pageCount = btopr(byteCount);
		HPGFX_LOG("A1 %d\n",pageCount);

                DRM_DEBUG("Allocating a DMA generic or CD buffer of size %d. \n", byteCount);

                /* Allocate the hpgfx_DmaBuff structure */
                _buff = (hpgfx_DmaPrivateBuff *) ALLOC_KERNEL_MEM(sizeof(hpgfx_DmaPrivateBuff));
                if (!_buff) {
                        DRM_DEBUG("D'oh!  Can't allocate hpgfx_DmaBuff structure!\n");
                        HPGFX_LOG("A2  Can't allocate!\n");
                        return -ENOMEM;
                }
                for (pageNum = WRITE_TO_HOST_PAGE_COUNT; pageNum < _dmaMgr->writeCombinedPageCount;
                     pageNum++) {
                        /* Search the page table until a contiguous block of memory is found */
                        /* Which is large enough to hold the buffer. */

                        if (_dmaMgr->writeCombinedPageTable[pageNum].associatedPointer == NULL) {
                                contiguousPages++;
                                DRM_DEBUG("Free page: contiguousPages=%d, pageNum=%d, pageAddr=0x%0X\n",
                                          contiguousPages, pageNum, wcBuff + PAGE_SIZE * pageNum);
                        }
                        else {
                                contiguousPages = 0;
                        }

                        /* If space is found, mark all the pages as used */
                        if (contiguousPages == pageCount) {
                                long reservedPages;

                                /* Calculate starting address for the allocated memory */
                                startingAddr = (void *) (wcBuff + PAGE_SIZE * (pageNum - pageCount + 1));

                                DRM_DEBUG("Found enough (%d) contigious WC pages.  Marking them as used.\n"
                                          "startingAddr=0x%0X\n", contiguousPages, startingAddr);

                                /* Mark pages as used */

                                for (reservedPages = pageNum;
                                     reservedPages > (long) pageNum - (long) pageCount;
                                     reservedPages--) {
                                        _dmaMgr->writeCombinedPageTable[reservedPages].associatedPointer = startingAddr;
                                }
                                found = TRUE;
                                break;
                        }
                }
                if (found) {
                        _buff->physAddress.LowPart = _dmaMgr->writeCombinedCommonBufferLogical.LowPart +
                            PAGE_SIZE * (pageNum - pageCount + 1);
                        _buff->publicBuff.virtualAddress = startingAddr;
                        _buff->publicBuff.copyBuffer = FALSE;
                }
                else {
                        DRM_DEBUG("D'oh!  Can't find %d contiguous pages...copyBuffer=true\n", pageCount);
                        _buff->physAddress.LowPart = 0;
                        _buff->publicBuff.virtualAddress = (void *)0xdeadbeef;
                        _buff->publicBuff.copyBuffer = TRUE;
		    
			/* Couldn't get a buffer, dump our current dma buffer state */
		        HPGFX_LOG("A4 COULD NOT ALLOCATE DMA BUFFER\n");
                        hpgfx_dma_buf_info(0, 0, 0, 0, &tmp, 0);
                }

                _buff->physAddress.HighPart = 0;
                _buff->marker = 0;
                _buff->pageCount = pageCount;
                _buff->nextBuff = NULL;
                _buff->prevBuff = NULL;
                _buff->pending = 0;
                _buff->hostDmaOffset = 0;
                _buff->bufferType = allocType;
                _buff->publicBuff.byteCount = byteCount;
                _buff->appendMarker = 1;
                _buff->publicBuff.userAddress = NULL;
                _buff->publicBuff.kernelPrivate = (hpgfx_DmaPrivateBuff *) _buff;

                /* Insert into the global list of buffers. */
                _buff->pid = current->pid;
                _buff->next = sBuffList;
                _buff->prev = NULL;
                if (sBuffList) {
                        sBuffList->prev = _buff;
                }
                sBuffList = _buff;

		HPGFX_LOG("A6 %X\n", _buff->physAddress.LowPart);

                /* Return buff structure */
                *buff = (hpgfx_DmaBuff *) _buff;
                return 0;
        }
	default:
		DRM_DEBUG("D'oh!  Unrecognized buffer type! (0x%0X)\n", allocType);
		return -EINVAL;
	}
}

/***********************************************************************************
 * hpgfx_dma_alloc_buff
 *
 * Wrapper ioctl function for DmaAllocBuff function above 
 *
 ***********************************************************************************/
int hpgfx_dma_alloc_buff(struct inode *inode, struct file *filp, unsigned int cmd,
			 unsigned long arg)
{
	drm_file_t *priv = filp->private_data;
	drm_device_t *dev = priv->dev;
	hpgfx_DmaIoctlData iodata;
	hpgfx_DmaBuffPtr buffptr;
	int err = 0;

	DRM_DEBUG("\n");
	copy_from_user_ret(&iodata, (hpgfx_DmaIoctlData *) arg, sizeof(iodata), -EFAULT);
        if (dev->isAGP) {
                REQUIRE(iodata.dmaMgr != NULL, return -EFAULT);
                REQUIRE(iodata.dmaMgr != (hpgfx_DmaManager *)0xdeadbeef, return -EFAULT);

                err = DmaAllocBuff(iodata.dmaMgr, iodata.allocType, iodata.count, &buffptr);
                if (err == 0) {
                        iodata.buff = *buffptr;
                }
        }
        else {
                iodata.buff.copyBuffer = TRUE;
                iodata.buff.virtualAddress = (void *)0xdeadbeef;
                iodata.buff.userAddress = NULL;
                iodata.buff.byteCount = 0;
                iodata.buff.kernelPrivate = (void *)0xdeadbeef;
        }
	copy_to_user_ret((hpgfx_DmaIoctlData *) arg, &iodata, sizeof(iodata), EFAULT);
	return err;
}

void DmaFinish(hpgfx_DmaManager * dmaMgr, hpgfx_DmaBuff * buff);

int DmaFreeBuff(hpgfx_DmaManager * dmaMgr, hpgfx_DmaBuff * buff)
{
	hpgfx_DmaManagerPrivate *_dmaMgr = ((hpgfx_DmaManagerPrivate *) dmaMgr);
	hpgfx_DmaPrivateBuff *_buff = (hpgfx_DmaPrivateBuff *)buff;
	unsigned long pageCount;
	hpgfx_DmaPage *pageTable;
	unsigned long pageTablePageCount;
	unsigned int page_order;

	/* First call DmaFinish with the input buff */
	DRM_DEBUG("\n");
	DmaFinish(dmaMgr, buff);

	/* Figure out which memory pool the buffer uses (cacheable or write combined ) */
	if (((hpgfx_pDmaPrivateBuff) buff)->bufferType == DMA_HOST_BUFFER) {
		pageTable = _dmaMgr->cacheablePageTable;
		pageTablePageCount = _dmaMgr->cacheablePageCount;
	}
	else {
		HPGFX_LOG("F1 %X\n", _buff->physAddress.LowPart);
		pageTable = _dmaMgr->writeCombinedPageTable;
		pageTablePageCount = _dmaMgr->writeCombinedPageCount;
	}

        /* Remove the buffer from the global linked list. */
        if (_buff->next) {
                _buff->next->prev = _buff->prev;
        }
        if (_buff->prev) {
                _buff->prev->next = _buff->next;
        }
        else {
                sBuffList = _buff->next;
        }

	/* Since the page table is small, we will simply scan the table from */
	/* the beginning */
	for (pageCount = 0; pageCount < pageTablePageCount - 1; pageCount++) {
		if (pageTable[pageCount].associatedPointer == buff->virtualAddress) {
			/* Once we have found the first matching page, we mark all contiguous */
			/* pages, and then break out of the for loop. */
			while (pageTable[pageCount].associatedPointer == buff->virtualAddress) {
				pageTable[pageCount].associatedPointer = NULL;
				pageCount++;
			}

			break;
		}
	}
	FREE_KERNEL_MEM(buff, sizeof(hpgfx_DmaPrivateBuff));
	return 0;
}


/***********************************************************************************
 * hpgfx_dma_free_buff
 *
 * Wrapper ioctl function for DmaFreeBuff function above 
 *
 ***********************************************************************************/
int hpgfx_dma_free_buff(struct inode *inode, struct file *filp, unsigned int cmd,
			unsigned long arg)
{
	drm_file_t *priv = filp->private_data;
	drm_device_t *dev = priv->dev;
	hpgfx_DmaIoctlData iodata;

	DRM_DEBUG("\n");
	copy_from_user_ret(&iodata, (hpgfx_DmaIoctlData *) arg, sizeof(iodata), -EFAULT);
        if (dev->isAGP) {
                REQUIRE(iodata.dmaMgr != NULL, return -EFAULT);
                REQUIRE(iodata.dmaMgr != (hpgfx_DmaManager *)0xdeadbeef, return -EFAULT);

                return DmaFreeBuff(iodata.dmaMgr, iodata.buff.kernelPrivate);
        }

        return 0;
}

static hpgfx_DmaReadInput *sLastCDBuffer = NULL;

#define CHUNK_SIZE_WORDS 256
#define LEGO_CD_BUFFER_ADDRESS_MAP(_csaddr) \
                ((void*)((char *)(_csaddr) + 0x00380000))

int hpgfx_dma_cd_read_from_host(struct inode *inode, struct file *filp, unsigned int cmd,
				unsigned long arg)
{
	drm_file_t *priv = filp->private_data;
	drm_device_t *dev = priv->dev;
	hpgfx_DmaManagerPrivate *_dmaMgr;
	hpgfx_DmaPrivateBuff *_privateBuff;

	static hpgfx_DmaReadInput input;

        sLastCDBuffer = &input;

	DRM_DEBUG("\n");
	copy_from_user_ret(&input, (hpgfx_DmaReadInput *) arg, sizeof(input), -EFAULT);
	_dmaMgr = ((hpgfx_DmaManagerPrivate *) input.dmaMgr);
	_privateBuff = ((hpgfx_DmaPrivateBuff *) input.sourceBuff.kernelPrivate);

        REQUIRE(!input.sourceBuff.copyBuffer, return 0);
        REQUIRE(_dmaMgr && (_dmaMgr != (void *)0xdeadbeef), return -EFAULT);


        /* Put buffer on the pending queue. */
        /* Gotta do this before we call _add_to_pending_list, */
        /* and get_next_marker, below */
	if (_privateBuff->appendMarker)
	{
		// If this buffer was sent down and not yet waited for,
		// wait for it now before we modify it!
		if(_privateBuff->pending) {
			DmaFinish(input.dmaMgr, &input.sourceBuff);
		}
	}
        /* Get next marker and store in hpgfx_DmaBuff */
        _privateBuff->marker = _get_next_marker((hpgfx_DmaManager *)_dmaMgr);

        _add_to_pending_list(_dmaMgr, (hpgfx_DmaBuff *) _privateBuff);

        setup_dma_read(_dmaMgr, (hpgfx_DmaReadInput *) &input, TRUE);

	return 0;
}

int hpgfx_dma_buf_info(char *buf, char **start, off_t offset, int len,
                           int *eof, void *data)
{
	hpgfx_DmaPrivateBuff *dmabuf = sBuffList;

	if (offset > 0) return 0; /* no partial requests */
	len  = 0;
	*eof = 1;

        DRM_DEBUG("HPGFX DMA Buffer List:\n");
	DRM_DEBUG("  PID  Pending  Pages  Address\n");
        HPGFX_LOG("HPGFX DMA Buffer List:\n");
	HPGFX_LOG("  PID  Pending  Pages  Address\n");
        while (dmabuf) {
                DRM_DEBUG(" %.5d   %s       %3d 0x%.8X\n",
                               dmabuf->pid, dmabuf->pending?"YES":"NO ",
                               dmabuf->pageCount, dmabuf->physAddress.LowPart);
                HPGFX_LOG     (" %.5d   %s       %3d 0x%.8X\n",
                               dmabuf->pid, dmabuf->pending?"YES":"NO ",
                               dmabuf->pageCount, dmabuf->physAddress.LowPart);
                dmabuf = dmabuf->next;
        }

        return len;
}

/* Dump info about the last cd buffer sent to the hardware to the proc entry */
int hpgfx_cd_buf_info(char *buf, char **start, off_t offset, int len,
                           int *eof, void *data)
{
        u32 i, *buffSrc, buffSize, *dest;

	if (offset > 0) return 0; /* no partial requests */
	len  = 0;
	*eof = 1;

        /* Do nothing if we haven't sent a cd buffer yet. */
        if (sLastCDBuffer == NULL) {
                return 0;
        }

        buffSrc = (u32 *)(((uintptr_t) sLastCDBuffer->sourceBuff.virtualAddress) +
                  sLastCDBuffer->offset);
        
#if 0
        dest = (uint32_t *) buf;
        if (sLastCDBuffer->byteCount > DRM_PROC_LIMIT) {
                buffSize = DRM_PROC_LIMIT >> 2; /* Make it a word count */
        }
        else {
                buffSize = sLastCDBuffer->byteCount >> 2; /* Make it a word count */
        }

        for (i=0; i<buffSize; i++) {
                dest[i] = buffSrc[i];
        }
        return (buffSize << 2);
#else
        *start = (char *) buffSrc;
        return sLastCDBuffer->byteCount;
#endif

}

/* Show the contents of a selection of summit registers. */
int hpgfx_regs_info(char *buf, char **start, off_t offset, int len,
                           int *eof, void *data)
{
	drm_device_t *dev = (drm_device_t *)data;
        uint32_t reg;
        uintptr_t csaddr = dev->csaddr;

	if (offset > 0) return 0; /* no partial requests */
	len  = 0;
	*eof = 1;

        /* Do nothing if we haven't sent a cd buffer yet. */
        if (csaddr == 0) {
                DRM_PROC_PRINT("D'oh!  The summit device isn't mapped yet!\n");
        }
        else {
                DRM_PROC_PRINT("UB_STATUS=      0x%08X\n", MEM_READ32_OFFS(csaddr, UB_STATUS));
                DRM_PROC_PRINT("UB_CONTROL=     0x%08X\n", MEM_READ32_OFFS(csaddr, UB_CONTROL));
                DRM_PROC_PRINT("UB_FC=          0x%08X\n", MEM_READ32_OFFS(csaddr, UB_FC));
                DRM_PROC_PRINT("UB_FFC=         0x%08X\n", MEM_READ32_OFFS(csaddr, UB_FFC));
                DRM_PROC_PRINT("UB_DC=          0x%08X\n", MEM_READ32_OFFS(csaddr, UB_DC));
                DRM_PROC_PRINT("UB_AGP_FFC=     0x%08X\n", MEM_READ32_OFFS(csaddr, UB_AGP_FFC));
                DRM_PROC_PRINT("UB_TRI_STATUS=  0x%08X\n", MEM_READ32_OFFS(csaddr, UB_TRI_STATUS));
                DRM_PROC_PRINT("\nDMA Registers:\n");
                DRM_PROC_PRINT("UP_DMA_UPPER32= 0x%08X\n", MEM_READ32_OFFS(csaddr, UP_DMA_UPPER32));
                DRM_PROC_PRINT("UP_DMA_TARGET=  0x%08X\n", MEM_READ32_OFFS(csaddr, UP_DMA_TARGET));
                DRM_PROC_PRINT("UP_DMA_LIMIT=   0x%08X\n", MEM_READ32_OFFS(csaddr, UP_DMA_LIMIT));
                DRM_PROC_PRINT("UB_DMA_OFFSET=  0x%08X\n", MEM_READ32_OFFS(csaddr, UB_DMA_OFFSET));
        }

        return len;
}

int hpgfx_dma_write_to_host(struct inode *inode, struct file *filp, unsigned int cmd,
			    unsigned long arg)
{
	drm_file_t	  *priv	  = filp->private_data;
	drm_device_t	  *dev	  = priv->dev;
	hpgfx_DmaWriteInput input;
	hpgfx_DmaManagerPrivate *_dmaMgr;
	hpgfx_DmaPrivateBuff *dmaPrivBuff;
	hpgfx_DmaReadInput readInput;
	unsigned long *CDBuffAddr, *CDBuffBase;
	hpgfx_DmaBuffPtr writeBuffptr;
	hpgfx_DmaBuffPtr readBuffptr;
	unsigned remnant;

#if CK_BDRDY_IN_WAIT_FOR_MARKER
        bdrdy = 0;
#endif
	DRM_DEBUG("\n");
        REQUIRE(dev->isAGP, return DMA_FAILURE);
        REQUIRE(input.targetByteCount <= EIGHT_PAGES, return DMA_FAILURE);
	copy_from_user_ret(&input, (hpgfx_DmaIoctlData *) arg, sizeof(input), -EFAULT);
	DRM_DEBUG("  input.targetByteCount = %d\n",input.targetByteCount);
	DRM_DEBUG("  input.targetOffset = %d\n",input.targetOffset);
	DRM_DEBUG("  input.commandBuff = %d\n",input.commandBuff);
	DRM_DEBUG("  input.commandByteCount = %d\n",input.commandByteCount);
	DRM_DEBUG("  input.commandOffset = %d\n",input.commandOffset);

	_dmaMgr = ((hpgfx_DmaManagerPrivate *) input.dmaMgr);
        writeBuffptr = _dmaMgr->dmaWrite_writeBuffptr; /* Buffer in which data will dma'ed into */
        readBuffptr = _dmaMgr->dmaWrite_readBuffptr;   /* Buffer containing commands to start a DMA write */

	CDBuffAddr = (unsigned long *)readBuffptr->virtualAddress;
	CDBuffBase = CDBuffAddr;
	
	SOFTCD_WRITE32_OFFS(CDBuffAddr,
			    BP_DMA_OFFSET,
			    ((input.targetByteCount >> 2) << 18)
			    | writeBuffptr->virtualAddress - _dmaMgr->cacheableCommonBufferVirtual
			    | DMA_ENABLE_BIT);              
        
	/* Send a DMA read that starts a DMA write */
	readInput.dmaMgr = _dmaMgr;
	readInput.sourceBuff = *readBuffptr;
	readInput.byteCount = (unsigned int) CDBuffAddr - (unsigned int) CDBuffBase;
	readInput.offset = 0;
	readInput.flushHint = 1;
	setup_dma_read(_dmaMgr, &readInput, FALSE);
	
	/* Prepare to send a DMA read that generates data for DMA write via write-to-read */
	readInput.sourceBuff =  input.commandBuff;
	readInput.byteCount = input.commandByteCount;
	readInput.offset = input.commandOffset;

	/* Create a marker that we can wait for to make sure dma's are complete */
	dmaPrivBuff = ((hpgfx_pDmaPrivateBuff) readInput.sourceBuff.kernelPrivate);
	dmaPrivBuff->marker = _get_next_marker((hpgfx_DmaManager *)_dmaMgr);
	DRM_DEBUG("  dmaPrivBuff->marker = %x\n",dmaPrivBuff->marker);

	/* Start the DMA read */
	setup_dma_read(_dmaMgr, &readInput, TRUE);

	/* Put command buffer on the pending queue */
	_add_to_pending_list(_dmaMgr, (hpgfx_DmaBuff *) dmaPrivBuff);
	
	/* Wait for data to be available */
        DmaFinish(_dmaMgr, (hpgfx_DmaBuff *) dmaPrivBuff);

	/* Copy dma result to caller's buffer */
	copy_to_user_ret(input.targetBuff+input.targetOffset, writeBuffptr->virtualAddress,
			 input.targetByteCount, -EFAULT);
	
#if CK_BDRDY_IN_WAIT_FOR_MARKER
        if (bdrdy) return DMA_FAILURE;
#endif

	return 0;

}


unsigned long _get_next_marker(hpgfx_DmaManager * dmaMgr)
{
	hpgfx_DmaManagerPrivate *_dmaMgr = ((hpgfx_DmaManagerPrivate *) dmaMgr);

	/* Wrap marker when 0 */
	DRM_DEBUG("\n");
	if (_dmaMgr->MarkerCounter == 0) {
		DmaFinishAll(dmaMgr);
	}

	return _dmaMgr->MarkerCounter++;
}


void DmaFinish(hpgfx_DmaManager * dmaMgr, hpgfx_DmaBuff * buff)
{
	hpgfx_DmaManagerPrivate *_dmaMgr = ((hpgfx_DmaManagerPrivate *) dmaMgr);
	hpgfx_DmaPrivateBuff *privateBuff = ((hpgfx_DmaPrivateBuff *) (buff));
	hpgfx_DmaCacheableUtilityPageStruct *utilPage = (hpgfx_DmaCacheableUtilityPageStruct *) _dmaMgr->DmaCacheableUtilityPage->virtualAddress;
        uint32_t status;

	DRM_DEBUG("privateBuff->marker = %x\n",privateBuff->marker);

	/* If the buffer is still pending wait for global marker to catch up */
	if (privateBuff->pending) {

                WAIT_FOR_MARKER(utilPage->globalMarker, privateBuff->marker,
                                (volatile uintptr_t) _dmaMgr->csAddr);

		privateBuff->pending = 0;

		/* Remove buffer from the pending list */
		if (_dmaMgr->pendingListEnd == buff) {
			_dmaMgr->pendingListEnd = privateBuff->prevBuff;
		}
		else {
			((hpgfx_DmaPrivateBuff *) (privateBuff->nextBuff))->prevBuff = privateBuff->prevBuff;
		}

		if (_dmaMgr->pendingListStart == buff) {
			_dmaMgr->pendingListStart = privateBuff->nextBuff;
		}
		else {
			((hpgfx_DmaPrivateBuff *) (privateBuff->prevBuff))->nextBuff = privateBuff->nextBuff;
		}

		privateBuff->prevBuff = NULL;
		privateBuff->nextBuff = NULL;
	}
}


int hpgfx_dma_finish(struct inode *inode, struct file *filp, unsigned int cmd,
		     unsigned long arg)
{
	drm_file_t	  *priv	  = filp->private_data;
	drm_device_t	  *dev	  = priv->dev;
        hpgfx_DmaIoctlData iodata;

        DRM_DEBUG("\n");
        REQUIRE(dev->isAGP, return -EFAULT);

        copy_from_user_ret(&iodata, (hpgfx_DmaIoctlData *) arg, sizeof(iodata), -EFAULT);

        REQUIRE(iodata.dmaMgr != NULL, return -EFAULT);
        REQUIRE(iodata.dmaMgr != (hpgfx_DmaManager *)0xdeadbeef, return -EFAULT);

        DmaFinish(iodata.dmaMgr, iodata.buff.kernelPrivate);

	return 0;
}


int DmaFinishAll(hpgfx_DmaManager * dmaMgr)
{
	hpgfx_DmaManagerPrivate *_dmaMgr = ((hpgfx_DmaManagerPrivate *) dmaMgr);

        DRM_DEBUG("\n");
        REQUIRE(_dmaMgr->csAddr != 0, return -EFAULT);

	/* First, wait for all of the AGPs to go to the device */
	WAIT_AGP_UNBUSY(_dmaMgr->csAddr);

	/* Call DmaFinish for all buffers on the pending list */
	while (_dmaMgr->pendingListStart) {
		DmaFinish(dmaMgr, _dmaMgr->pendingListStart);
        }

	_dmaMgr->bincActive = 0;
	return 0;
}


int hpgfx_dma_finish_all(struct inode *inode, struct file *filp, unsigned int cmd,
			 unsigned long arg)
{
	drm_file_t	  *priv	  = filp->private_data;
	drm_device_t	  *dev	  = priv->dev;

	DRM_DEBUG("\n");

        if (dev->isAGP) {
                hpgfx_DmaIoctlData iodata;

                copy_from_user_ret(&iodata, (hpgfx_DmaIoctlData *) arg, sizeof(iodata), -EFAULT);
                
                REQUIRE(iodata.dmaMgr != NULL, return -EFAULT);
                REQUIRE(iodata.dmaMgr != (hpgfx_DmaManager *)0xdeadbeef, return -EFAULT);

                return DmaFinishAll(iodata.dmaMgr);
        }
	return 0;
}


int hpgfx_getLog(struct inode *inode, struct file *filp, unsigned int cmd,
			 unsigned long arg)
{
	drm_file_t	  *priv	  = filp->private_data;
	drm_device_t	  *dev	  = priv->dev;
        hpgfx_GetLogData iodata;

	DRM_DEBUG("\n");
	
	spin_lock(&logBuffer_lock);

	/* Copy the log buffer to the ioctl struct */
	strcpy(iodata.logBuffer, logBuffer);
	
	/* log buffer is now empty */
	lbIdx = 0;
	lbOverflow = 0;
	logBuffer[0] = 0;

	spin_unlock(&logBuffer_lock);

	copy_to_user_ret((hpgfx_GetLogData *) arg, &iodata, sizeof(iodata), EFAULT);

	return 0;
}


static void createMarkerWriteCDPackets(hpgfx_DmaManager * dmaMgr)
{
	int i;
	unsigned long utilityOffset, cdPacketOffset;
	hpgfx_DmaManagerPrivate *_dmaMgr = ((hpgfx_DmaManagerPrivate *) dmaMgr);
	hpgfx_DmaWriteCombinedUtilityPageStruct *
                utilPage = ((hpgfx_DmaWriteCombinedUtilityPageStruct *)
                            (_dmaMgr->DmaWriteCombinedUtilityPage->virtualAddress));
	hpgfx_DmaCacheableUtilityPageStruct *
                cacheableUtilPage = ((hpgfx_DmaCacheableUtilityPageStruct *)
                                     (_dmaMgr->DmaCacheableUtilityPage->virtualAddress));
	uint32_t *CDBuffAddr = (uint32_t *) (&(utilPage->markerPackets));

	/* Get offset of the utility page.  */
	utilityOffset = ((hpgfx_DmaPrivateBuff *) _dmaMgr->DmaCacheableUtilityPage)->hostDmaOffset;

	/* Calculate offset of globalMarker */
	cdPacketOffset = ((unsigned long) &(cacheableUtilPage->globalMarker)) -
	    ((unsigned long) cacheableUtilPage);

	/* Initialize 16 marker setup areas. By rotating through the areas, we dont need to */
	/* worry about flow controlling the marker setup area. The number of areas is 16 */
	/* because thats how many AGP dma slots exist in hardware. */
	DRM_DEBUG("Marker packets: BP_DMA_OFFSET=0x%0X\n",
                  (1 << 18) | (utilityOffset + cdPacketOffset) | DMA_ENABLE_BIT);
	for (i = 0; i < 16; i++) {
		uint32_t *CDBuffBase;
		int remnant;

		/* Setup DMA offset */
		CDBuffBase = CDBuffAddr;
		SOFTCD_WRITE32_OFFS(CDBuffAddr, BP_DMA_OFFSET,
                                    (1 << 18) | (utilityOffset + cdPacketOffset) | DMA_ENABLE_BIT);

		/* The value here will be rewritten before the packets are sent to the hardware */
		SOFTCD_WRITE32_OFFS(CDBuffAddr, BP_DMA_DATA, 0xdeadbeef);

		/* HW_WORKAROUND [ Pad it out to 64 bytes (16 words) with a CD packet */
		/* to BBC_NOP: */
		remnant = 16 - (CDBuffAddr - CDBuffBase);
		*CDBuffAddr = MAKE_BBC_NOP(remnant);
		CDBuffAddr += remnant;
		/* ] HW_WORKAROUND */
	}
}

static void setup_dma_engine(hpgfx_pDmaManagerPrivate _dmaMgr, hpgfx_DmaReadInput * input)
{
	hpgfx_DmaPrivateBuff *_privateBuff = input->sourceBuff.kernelPrivate;
	uint32_t lowerValue, offsetValue, count = 0;
	static uint32_t maxCount = 50000;

	REQUIRE(_dmaMgr->csAddr, return);

        lowerValue =
            ((unsigned long) _privateBuff->physAddress.LowPart & 0xFFFFF000)
            | 0x1;
        offsetValue =
            ((input->byteCount >> 3) << 19)
            | ((input->offset >> 3) << 3)
            | 0x3;

        DRM_DEBUG("input->byteCount=%d, input->offset=%d, lowerValue=0x%08X,"
                  " offsetValue=0x%08X\n",
                  input->byteCount, input->offset, lowerValue, offsetValue);

        /* HW_WORKAROUND [ */
        /* Heathrow has a bug with handshaking between a couple of */
        /* places in its DMA engine.  To work around it, we insert */
        /* a number of "dead" states into its FIFO.  This number */
        /* is 4 for AGP 1X and 2 for AGP 2X. */
        if (_dmaMgr->bincActive) {
                WAIT_AGP_UNBUSY(_dmaMgr->csAddr);
                _dmaMgr->bincActive = 0;
        }
        if (AGP_2X) {
                WAIT_AGP_CD_FFC(3, _dmaMgr->csAddr);
                MEM_WRITE32_OFFS(_dmaMgr->csAddr, BP_AGP_BDMA_LOWER, lowerValue);
        }
        else {
                WAIT_AGP_CD_FFC(6, _dmaMgr->csAddr);
                MEM_WRITE32_OFFS(_dmaMgr->csAddr, BP_AGP_BDMA_LOWER, lowerValue);
                MEM_WRITE32_OFFS(_dmaMgr->csAddr, BP_AGP_BDMA_LOWER, lowerValue);
                MEM_WRITE32_OFFS(_dmaMgr->csAddr, BP_AGP_BDMA_LOWER, lowerValue);
                MEM_WRITE32_OFFS(_dmaMgr->csAddr, BP_AGP_BDMA_LOWER, lowerValue);
        }
        /* ] HW_WORKAROUND */
        MEM_WRITE32_OFFS(_dmaMgr->csAddr, BP_AGP_BDMA_LOWER, lowerValue);
        MEM_WRITE32_OFFS(_dmaMgr->csAddr, BP_AGP_BDMA_OFFSET, offsetValue);
}


static void setup_dma_read(hpgfx_pDmaManagerPrivate _dmaMgr, hpgfx_DmaReadInput * input, int doMarker)
{
	hpgfx_DmaPrivateBuff *_privateBuff = input->sourceBuff.kernelPrivate;
	unsigned remnant;
	uint32_t *pLastPacket;

	DRM_DEBUG("\n");

	if (doMarker && _privateBuff->appendMarker) {
                DRM_DEBUG("Appending marker at %d\n", input->byteCount + input->offset);
		_append_marker(_dmaMgr, (hpgfx_DmaBuff *)_privateBuff, input->byteCount + input->offset);
		input->byteCount += 16;
	}
	/* HW_WORKAROUND [ */
	/* We need to make sure no packets are smaller than 64 bytes */
	if (input->byteCount < 64) {
		pLastPacket = (uint32_t *) (input->byteCount + input->offset
			  + (char *) (input->sourceBuff.virtualAddress));
		remnant = (64 - input->byteCount);
		*pLastPacket = MAKE_BBC_NOP(remnant >> 2);
		input->byteCount += remnant;
	}
	/* ] HW_WORKAROUND */

	/* HW_WORKAROUND [ */
	/*  Heathrow really needs things 32-byte aligned! */
	remnant = input->byteCount % 32;	/* Was % 8 */
	/* ] HW_WORKAROUND */

	if (remnant) {
		/* HW_WORKAROUND [ */
		remnant = 32 - remnant;		/* Was 8 - */
		/* ] HW_WORKAROUND */

		pLastPacket = (uint32_t *) (input->byteCount + input->offset
		       + ((uintptr_t) input->sourceBuff.virtualAddress));
		*pLastPacket = MAKE_BBC_NOP(remnant >> 2);
		input->byteCount += remnant;
	}
        
	/* Make sure the CD buffer writes have been flushed */
	__asm__ __volatile("in $128,%al");

	setup_dma_engine(_dmaMgr, (hpgfx_DmaReadInput *) input);

	if (doMarker && !_privateBuff->appendMarker) {
                DRM_DEBUG("Sending marker at %d\n", input->byteCount + input->offset);
		_send_marker(_dmaMgr, ((hpgfx_DmaPrivateBuff *) input->sourceBuff.kernelPrivate)->marker);
	}
}



void _add_to_pending_list(hpgfx_DmaManagerPrivate * _dmaMgr, hpgfx_DmaBuff * buff)
{
	hpgfx_DmaPrivateBuff *privateBuff = (hpgfx_DmaPrivateBuff *) buff;

	/* If the buffer is already on the pending list then return; */
	DRM_DEBUG("\n");
	if (privateBuff->pending)
		return;

	privateBuff->pending = 1;
	privateBuff->prevBuff = NULL;
	privateBuff->nextBuff = NULL;

	if (_dmaMgr->pendingListStart == NULL) {
		_dmaMgr->pendingListStart = buff;
		_dmaMgr->pendingListEnd = buff;
	}
	else {
		((hpgfx_DmaPrivateBuff *) _dmaMgr->pendingListEnd)->nextBuff = buff;
		((hpgfx_DmaPrivateBuff *) buff)->prevBuff = _dmaMgr->pendingListEnd;
		_dmaMgr->pendingListEnd = buff;
	}
}


void _send_marker(hpgfx_DmaManagerPrivate * _dmaMgr, unsigned long marker)
{

	hpgfx_DmaReadInput input;
	hpgfx_DmaWriteCombinedUtilityPageStruct *utilPage =
	((hpgfx_DmaWriteCombinedUtilityPageStruct *) (_dmaMgr->DmaWriteCombinedUtilityPage->virtualAddress));

	/* Setup to dma the marker packets */
	if (_dmaMgr->markerSetupSlot == 16)
		_dmaMgr->markerSetupSlot = 0;

	input.sourceBuff = *((hpgfx_DmaBuff *) _dmaMgr->DmaWriteCombinedUtilityPage);
	input.byteCount = sizeof(markerWriteCDPackets);
	input.offset = (unsigned long) &utilPage->markerPackets[_dmaMgr->markerSetupSlot] -
	    (unsigned long) _dmaMgr->DmaWriteCombinedUtilityPage->virtualAddress;

	utilPage->markerPackets[_dmaMgr->markerSetupSlot].markerWord = marker;
	_dmaMgr->markerSetupSlot++;

	DRM_DEBUG("marker = 0x%0X, slot=%d, input.offset=%d, input.byteCount=%d \n",
	marker, _dmaMgr->markerSetupSlot, input.offset, input.byteCount);

	setup_dma_engine(_dmaMgr, (hpgfx_DmaReadInput *) &input);
}


void _append_marker(hpgfx_DmaManagerPrivate * _dmaMgr, hpgfx_DmaBuff * buff, unsigned long byteCount)
{
	hpgfx_DmaWriteCombinedUtilityPageStruct *utilPage =
	((hpgfx_DmaWriteCombinedUtilityPageStruct *) (_dmaMgr->DmaWriteCombinedUtilityPage->virtualAddress));
	u32 *CDBuffAddr = (u32 *)(((uintptr_t) buff->virtualAddress) + byteCount);

	CDBuffAddr[0] = utilPage->markerPackets->headerWords[0];
	CDBuffAddr[1] = utilPage->markerPackets->headerWords[1];
	CDBuffAddr[2] = utilPage->markerPackets->headerWords[2];
	CDBuffAddr[3] = ((hpgfx_DmaPrivateBuff *) buff)->marker;

	DRM_DEBUG("marker = 0x%0X, packets=[0x%0X, 0x%0X, 0x%0X, 0x%0X] \n",
		  ((hpgfx_DmaPrivateBuff *) buff)->marker, CDBuffAddr[0], CDBuffAddr[1],
		  CDBuffAddr[2], CDBuffAddr[3]);
}

#if LINUX_VERSION_CODE < 0x020317
unsigned long
#else				/* Return type changed in 2.3.23 */
struct page *
#endif
 hpgfx_vm_dma_nopage(struct vm_area_struct *vma, unsigned long address, int write_access)
{
	unsigned long physical;

	if (address > vma->vm_end) {
		return NOPAGE_SIGBUS;	/* Disallow mremap */
	}
        /* Physical address of this page =
         *   Physical start of allocated dma buffer (i.e. vm_offset) + 
         *   offset into vma (i.e. address - vm_start). */
	physical = vma_get_offset(vma) + (address - vma_get_start(vma));

        /* Increment the use count for this page.  Will be decremented by the kernel on unmap. */
#if LINUX_VERSION_CODE >= 0x020400
	atomic_inc(&virt_to_page(physical)->count); /* Dec. by kernel */
#else
        mem_map_inc_count(MAP_NR(physical));
#endif

#if LINUX_VERSION_CODE >= 0x020400
	DRM_DEBUG("page=%lu, address=0x%08lx, vm_offset=0x%08lx, physical=0x%08lx.\n",
		virt_to_page(physical), address, vma_get_offset(vma), physical);
#else
	DRM_DEBUG("page=%lu, address=0x%08lx, vm_offset=0x%08lx, physical=0x%08lx.\n",
		MAP_NR(physical), address, vma_get_offset(vma), physical);
#endif

#if LINUX_VERSION_CODE < 0x020317
	return (physical & PAGE_MASK);
#else
	return virt_to_page(physical);
#endif
}

static struct vm_operations_struct hpgfx_vm_dma_ops =
{
	nopage:hpgfx_vm_dma_nopage,
	open:drm_vm_open,
	close:drm_vm_close,
};

int hpgfx_mmap(struct file *filp, struct vm_area_struct *vma)
{
	uintptr_t bufstart, bufend;

	DRM_DEBUG("vm_start = 0x%lx, vm_end = 0x%lx, vm_offset = 0x%lx\n",
		  vma->vm_start, vma->vm_end, vma_get_offset(vma));

	get_ntdma_pool_range(&bufstart, &bufend);
	DRM_DEBUG("bufstart=0x%0X, bufend=0x%0X\n", bufstart, bufend);

	if ((vma_get_offset(vma) >= bufstart) && (vma_get_offset(vma) < bufend)) {
		vma->vm_ops = &hpgfx_vm_dma_ops;
		vma->vm_flags |= VM_LOCKED | VM_SHM;	/* Don't swap */

#if LINUX_VERSION_CODE < 0x020203 /* KERNEL_VERSION(2,2,3) */
				/* In Linux 2.2.3 and above, this is
				   handled in do_mmap() in mm/mmap.c. */
                ++filp->f_count;
#endif
		vma->vm_file = filp;	/* Needed for drm_vm_open() */
		drm_vm_open(vma);
		return 0;
	}
	else {
		return drm_mmap(filp, vma);
	}
}

int hpgfx_lock(struct inode *inode, struct file *filp, unsigned int cmd,
	       unsigned long arg)
{
	drm_file_t	  *priv	  = filp->private_data;
	drm_device_t	  *dev	  = priv->dev;
	DECLARE_WAITQUEUE(entry, current);
	int		  ret	= 0;
	drm_lock_t	  lock;
	drm_queue_t	  *q;
#if DRM_DMA_HISTOGRAM
	cycles_t	  start;

	dev->lck_start = start = get_cycles();
#endif

	copy_from_user_ret(&lock, (drm_lock_t *)arg, sizeof(lock), -EFAULT);

	if (lock.context == DRM_KERNEL_CONTEXT) {
		DRM_ERROR("Process %d using kernel context %d\n",
			  current->pid, lock.context);
		return -EINVAL;
	}

	DRM_DEBUG("%d (pid %d) requests lock (0x%08x), flags = 0x%08x\n",
		  lock.context, current->pid, dev->lock.hw_lock->lock,
		  lock.flags);

	if (lock.context < 0 || lock.context >= dev->queue_count) {
		return -EINVAL;
        }
	q = dev->queuelist[lock.context];
	
        /* Hmm, should we always flush all DMA?  I think so... */
        /* Don't do it unless we're on an AGP device though! */
        if (dev->isAGP && sDMAMgr && sDMAInited) {
                ret = DmaFinishAll((hpgfx_DmaManager *) sDMAMgr);
        }

	if (!ret) {
		if (_DRM_LOCKING_CONTEXT(dev->lock.hw_lock->lock) != lock.context) {
			long j = jiffies - dev->lock.lock_time;

			if (j > 0 && j <= DRM_LOCK_SLICE) {
				/* Can't take lock if we just had it and
				   there is contention. */
				current->state = TASK_INTERRUPTIBLE;
				schedule_timeout(j);
			}
		}
		add_wait_queue(&dev->lock.lock_queue, &entry);
		for (;;) {
			if (!dev->lock.hw_lock) {
				/* Device has been unregistered */
				ret = -EINTR;
				break;
			}
			if (drm_lock_take(&dev->lock.hw_lock->lock, lock.context)) {
				dev->lock.pid	    = current->pid;
				dev->lock.lock_time = jiffies;
				atomic_inc(&dev->total_locks);
				atomic_inc(&q->total_locks);
				break;	/* Got lock */
			}
			
				/* Contention */
			atomic_inc(&dev->total_sleeps);
			current->state = TASK_INTERRUPTIBLE;
			schedule();
			if (signal_pending(current)) {
				ret = -ERESTARTSYS;
				break;
			}
		}
		current->state = TASK_RUNNING;
		remove_wait_queue(&dev->lock.lock_queue, &entry);
	}

	drm_flush_unblock(dev, lock.context, lock.flags); /* cleanup phase */
	
	DRM_DEBUG("%d %s\n", lock.context, ret ? "interrupted" : "has lock");

#if DRM_DMA_HISTOGRAM
	atomic_inc(&dev->histo.lacq[drm_histogram_slot(get_cycles() - start)]);
#endif
	
	return ret;
}

void hpgfx_dma_free_buffers(void)
{
	hpgfx_DmaPrivateBuff *buf = sBuffList;
	
	DRM_DEBUG("\n");

        while (buf) {
                hpgfx_DmaPrivateBuff *tbuf;
                
                tbuf = buf;
                buf = buf->next;
                if (tbuf->pid == current->pid) {
                        /* Remove the buffer from the global linked list. */
                        if (tbuf->next) {
                                tbuf->next->prev = tbuf->prev;
                        }
                        if (tbuf->prev) {
                                tbuf->prev->next = tbuf->next;
                        }
                        else {
                                sBuffList = tbuf->next;
                        }
                        DmaFreeBuff((hpgfx_DmaManager *) sDMAMgr, (hpgfx_DmaBuff *) tbuf);
                }
        }

        /* Set the DMA manager to "uninitialized".  This will ensure that
         * we do not attempt to access the device through an invalid
         * control space address.  This can happen when the X server recycles.
         * A server recycle results in the device being remapped using drmAddMap.
         * This could result in a different mapping and thus a different control
         * space address.  By uninitializing the DMA manager, we will not call
         * DmaFinishAll when the X server tries to regain the lock.  If we did,
         * the DMA manager would be using a potentially invalid control space
         * pointer and may end up hanging.  Once the X server initializes DMA,
         * eventually resulting in a call to hpgfx_dma_initialize, the control
         * space pointer will be updated and everything will be OK again.  Well,
         * maybe not everything...
         */
        sDMAInited = FALSE;
}

int hpgfx_unlock(struct inode *inode, struct file *filp, unsigned int cmd,
		 unsigned long arg)
{
	drm_file_t	  *priv	  = filp->private_data;
	drm_device_t	  *dev	  = priv->dev;
	drm_lock_t	  lock;

	copy_from_user_ret(&lock, (drm_lock_t *)arg, sizeof(lock), -EFAULT);
	
	if (lock.context == DRM_KERNEL_CONTEXT) {
		DRM_ERROR("Process %d using kernel context %d\n",
			  current->pid, lock.context);
		return -EINVAL;
	}

	DRM_DEBUG("%d frees lock (%d holds)\n", lock.context,
		  _DRM_LOCKING_CONTEXT(dev->lock.hw_lock->lock));
	atomic_inc(&dev->total_unlocks);
	if (_DRM_LOCK_IS_CONT(dev->lock.hw_lock->lock)) {
		atomic_inc(&dev->total_contends);
        }

	drm_lock_transfer(dev, &dev->lock.hw_lock->lock, DRM_KERNEL_CONTEXT);

	if (!dev->context_flag) {
		if (drm_lock_free(dev, &dev->lock.hw_lock->lock, DRM_KERNEL_CONTEXT)) {
			DRM_ERROR("\n");
		}
	}
#if DRM_DMA_HISTOGRAM
	atomic_inc(&dev->histo.lhld[drm_histogram_slot(get_cycles()
						       - dev->lck_start)]);
#endif
	
	return 0;
}

#if defined(DEBUG)
void *drm_alloc_debug(size_t size, int area,
                      const char *funcname, const char *filename, int line)
{
        void *retval = drm_alloc(size, area);

        if (drm_flags&DRM_FLAG_DEBUG) {
                DRM_PRINTK(KERN_DEBUG "[" DRM_NAME ": %s : line %d ] allocated %d bytes at 0x%08X\n",
                           funcname, line, size, retval);
        }
        return retval;
}

void drm_free_debug(void *ptr, size_t size, int area,
                    const char *funcname, const char *filename, int line)
{
        drm_free(ptr, size, area);

        if (drm_flags&DRM_FLAG_DEBUG) {
                DRM_PRINTK(KERN_DEBUG "[" DRM_NAME ": %s : line %d ] freed %d bytes from 0x%08X\n",
                           funcname, line, size, ptr);
        }
}
#endif
