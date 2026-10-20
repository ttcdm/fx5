/************************************************************************
 *
 * hpgfx_dma.h
 *
 * Private structures and prototypes for the Linux DMA manager (NT-style)
 *
 * Copywrite 1998 Hewlett Packard Company
 *
 * Original author of NT Dma Manager: Scott Ruff
 *                    Linux Changes: Alan Ward
 *
 ************************************************************************/

#ifndef _HPGFX_DMA_H_
#define _HPGFX_DMA_H_

#include "hpgfx_shared_dma.h"
#include <linux/delay.h>
#include <linux/kernel.h>

#define TRUE 1
#define FALSE 0

typedef struct {
	unsigned long LowPart;
	unsigned long HighPart;
} PHYSICAL_ADDRESS;

typedef struct hpgfx_DmaPrivateBuffStruct hpgfx_DmaPrivateBuff;
typedef struct hpgfx_DmaPrivateBuffStruct * hpgfx_pDmaPrivateBuff;
struct hpgfx_DmaPrivateBuffStruct {
	hpgfx_DmaBuff publicBuff;
	uint32_t marker;
	PHYSICAL_ADDRESS physAddress;
	uint32_t pageCount;
	hpgfx_DmaBuffPtr nextBuff;
	hpgfx_DmaBuffPtr prevBuff;
	char pending;
	unsigned long hostDmaOffset;
	char appendMarker;
	hpgfx_DmaAllocType bufferType;
	void * physContext;
	void * virtualContext;

        /* Stuff to keep a single linked list of buffers around, so
         * we can free them when a process dies. */
	pid_t pid;
	hpgfx_DmaPrivateBuff *next;
	hpgfx_DmaPrivateBuff *prev;

};

typedef struct {
	void* associatedPointer;
} hpgfx_DmaPage, *hpgfx_pDmaPage;

/* HW_WORKAROUND [:
 *	Two Heathrow bugs cause us to make sure these two things
 *	are 64 bytes long.  The first bug causes DMA which is not
 *	a multiple of 32 bytes to possibly get lost.  The second
 *	bug is a bug in the AGP DMA FIFO whereby small single packets
 *	get lost.  When we set the AGP64 bit, "small" means less than
 *	64 bytes.  So, these two bugs converge on a 64-byte size for
 *	these small packets.  We have 16 of each of them, so the
 *	total size is 16*64 + 16*64, or 2048 bytes for the sum total.
 *	This is well under 4096 bytes, the size of the utility page.
 */
typedef struct {
	uint32_t headerWords[3];
	uint32_t markerWord;
	uint32_t alignPad[12];
} markerWriteCDPackets;

typedef struct {
	uint32_t cdPacketHeader;
	uint32_t hostDmaOffsetword;
	uint32_t alignPad[14];
} hostDmaSetupCDPacket;
/* ] HW_WORKAROUND */

typedef struct {
	volatile uint32_t globalMarker;

	uint32_t pad1; /* Padding to force marker packets to be 8 byte aligned */
} hpgfx_DmaCacheableUtilityPageStruct;

typedef struct {
	
	/* This structure represents the memory within the utility page */
	/* that will be used to create the CD buffer packets which will */
	/* cause a marker to be written to the globalMarker, also */
	/* contained within this page. The headerWords are the words */
	/* of the packets that will stay the same. The markerWord will */
	/* be assigned for each new marker written. Of course this whole */
	/* mechanism is part of the flow control design. See the design */
	/* document for more information. */

	markerWriteCDPackets markerPackets[16];

	hostDmaSetupCDPacket dmaWriteSetupPackets[16];

} hpgfx_DmaWriteCombinedUtilityPageStruct;

typedef struct {
	void* writeCombinedCommonBufferVirtual;
	PHYSICAL_ADDRESS writeCombinedCommonBufferLogical;
	unsigned long writeCombinedPageCount;
	void* cacheableCommonBufferVirtual;
	PHYSICAL_ADDRESS cacheableCommonBufferLogical;
	unsigned long cacheablePageCount;
	hpgfx_DmaPage* writeCombinedPageTable;
	hpgfx_DmaPage* cacheablePageTable;
	unsigned char dmaWriteSetupSlot;
	unsigned char directDrawRunning;
	unsigned long MarkerCounter;
	unsigned char markerSetupSlot;
	unsigned char bincActive;
	hpgfx_DmaBuffPtr pendingListStart;
	hpgfx_DmaBuffPtr pendingListEnd;
	hpgfx_DmaBuffPtr DmaWriteCombinedUtilityPage;
	hpgfx_DmaBuffPtr DmaCacheableUtilityPage;
	hpgfx_DmaBuffPtr dmaWrite_writeBuffptr;
	hpgfx_DmaBuffPtr dmaWrite_readBuffptr;
	unsigned long CDBuffPageCount;
	volatile unsigned long *csAddr;
	char * pUseMSDmaAlloc;
} hpgfx_DmaManagerPrivate, *hpgfx_pDmaManagerPrivate;


#define MEM_WRITE32_OFFS(addr,offs,data) \
        (*(volatile uint32_t *)((volatile uintptr_t)(offs)+ \
                              (volatile uintptr_t)(addr)) = (uint32_t)(data))

#define MEM_READ32_OFFS(addr,offs) \
        (*(volatile uint32_t *)((offs)+(volatile uint8_t *)(addr)))

#if defined(DEBUG)
void *drm_alloc_debug(size_t size, int area,
                      const char *funcname, const char *filename, int line);
void drm_free_debug(void *ptr, size_t size, int area,
                    const char *funcname, const char *filename, int line);

#define ALLOC_KERNEL_MEM(_size) drm_alloc_debug(_size, DRM_MEM_MAPS, __FUNCTION__, __FILE__, __LINE__)
#define FREE_KERNEL_MEM(_ptr,_size) drm_free_debug(_ptr, _size, DRM_MEM_MAPS, __FUNCTION__, __FILE__, __LINE__)

#define HPGFX_MAX_UDELAY 100
#define HPGFX_HW_READ_DELAY 10000
#define TEN_SECOND_DELAY (10000000 / HPGFX_HW_READ_DELAY)

#define DECLARE_DELAY_COUNTER(csaddr) \
        int _sWedgedBailCount; \
        volatile uintptr_t _sdcCsAddr = (uintptr_t) (csaddr); \
        _sWedgedBailCount = 0

#define HPGFX_SLEEP(seconds) do { \
        unsigned long j = jiffies + seconds * HZ; \
        while (jiffies < j) { schedule(); }

#define HPGFX_DELAY(count) do { \
        int32_t _delay; _delay = HPGFX_HW_READ_DELAY*count; \
        if (_sWedgedBailCount++ > TEN_SECOND_DELAY) { \
                DRM_ERROR("+%d %s: Hardware is wedged!  We're outta here!\n", \
                          __LINE__, __FILE__); \
                HPGFX_ABANDON_SHIP(); \
         } \
         do { \
            udelay(HPGFX_MAX_UDELAY); \
            _delay -= HPGFX_MAX_UDELAY; \
        } while ((_delay > 0) && (drm_flags & DRM_FLAG_DEBUG)); \
} while(0)

#define HPGFX_ABANDON_SHIP() do { \
        DRM_ERROR("############################################################\n\n"); \
        DRM_ERROR("Terminating PID %d (\"%s\") with extreme prejudice!\n\n", \
                  current->pid, current->exec_domain->name); \
        DRM_ERROR("############################################################\n\n" \
                  "  General Status Registers:\n" \
                  "     UB_STATUS=      0x%08X\n" \
                  "     UB_CONTROL=     0x%08X\n" \
                  "     UB_FC=          0x%08X\n" \
                  "     UB_FFC=         0x%08X\n" \
                  "     UB_DC=          0x%08X\n" \
                  "     UB_TRI_STATUS=  0x%08X\n\n" \
                  "  AGP Registers:\n" \
                  "     UB_AGP_FFC=         0x%08X\n" \
                  "     BP_AGP_BDMA_LOWER=  0x%08X\n" \
                  "     BP_AGP_BDMA_OFFSET= 0x%08X\n\n" \
                  "  DMA Registers:\n" \
                  "     UP_DMA_UPPER32=     0x%08X\n" \
                  "     UP_DMA_TARGET=      0x%08X\n" \
                  "     UP_DMA_LIMIT=       0x%08X\n" \
                  "     UB_DMA_OFFSET=      0x%08X\n\n", \
                  MEM_READ32_OFFS(_sdcCsAddr, UB_STATUS), \
                  MEM_READ32_OFFS(_sdcCsAddr, UB_CONTROL), \
                  MEM_READ32_OFFS(_sdcCsAddr, UB_FC), \
                  MEM_READ32_OFFS(_sdcCsAddr, UB_FFC), \
                  MEM_READ32_OFFS(_sdcCsAddr, UB_DC), \
                  MEM_READ32_OFFS(_sdcCsAddr, UB_TRI_STATUS), \
                  MEM_READ32_OFFS(_sdcCsAddr, UB_AGP_FFC), \
                  MEM_READ32_OFFS(_sdcCsAddr, BP_AGP_BDMA_LOWER), \
                  MEM_READ32_OFFS(_sdcCsAddr, BP_AGP_BDMA_OFFSET), \
                  MEM_READ32_OFFS(_sdcCsAddr, UP_DMA_UPPER32), \
                  MEM_READ32_OFFS(_sdcCsAddr, UP_DMA_TARGET), \
                  MEM_READ32_OFFS(_sdcCsAddr, UP_DMA_LIMIT), \
                  MEM_READ32_OFFS(_sdcCsAddr, UB_DMA_OFFSET)); \
        DRM_ERROR("############################################################\n"); \
        MOD_DEC_USE_COUNT; \
        ((int *)0)[0] = 1; \
} while (0)

#else

#define ALLOC_KERNEL_MEM(_size) drm_alloc(_size,DRM_MEM_MAPS)
#define FREE_KERNEL_MEM(_ptr,_size) drm_free(_ptr,_size,DRM_MEM_MAPS)

#define HPGFX_HW_READ_DELAY 1

#define DECLARE_DELAY_COUNTER(csaddr)
#define HPGFX_DELAY(count) do { \
        udelay(HPGFX_HW_READ_DELAY*count); \
} while(0)

#define HPGFX_ABANDON_SHIP()

#endif

#define WAIT_WPNE(csaddr) \
do { \
    DECLARE_DELAY_COUNTER(csaddr); \
    do { \
        volatile uint32_t status; \
        status = MEM_READ32_OFFS((csaddr), UB_STATUS); \
        if (!(status & 0x01000000)) { \
            break; \
        } \
        HPGFX_DELAY(1); \
    } while (1); \
} while (0) 

#define GET_SLOTS(csaddr, count) \
do { \
    DECLARE_DELAY_COUNTER(csaddr); \
    do { \
        int tmp; \
        tmp = MEM_READ32_OFFS((csaddr), UB_FFC); \
        tmp = (tmp >> 1) - 10; \
        if (tmp >= count) { \
            break; \
        } \
        HPGFX_DELAY(1); \
    } while (1); \
} while (0)

/* Private function declarations */

unsigned long _get_next_marker(hpgfx_DmaManager* dmaMgr);
static void createMarkerWriteCDPackets(hpgfx_DmaManager* dmaMgr);
static void setup_dma_read (hpgfx_DmaManagerPrivate* _dmaMgr, hpgfx_DmaReadInput* input, int doMarker);
void _add_to_pending_list (hpgfx_DmaManagerPrivate* _dmaMgr, hpgfx_DmaBuff* buff);
void _send_marker (hpgfx_DmaManagerPrivate* _dmaMgr, unsigned long marker);
void _append_marker (hpgfx_DmaManagerPrivate* _dmaMgr, hpgfx_DmaBuff* buff, unsigned long marker);

static void setup_dma_engine ( hpgfx_pDmaManagerPrivate _dmaMgr, hpgfx_DmaReadInput* input);

#if LINUX_VERSION_CODE >= 0x020400
#define vma_get_offset(_v) (_v->vm_pgoff << PAGE_SHIFT)
#define vma_get_start(_v) _v->vm_start
#endif

#endif
