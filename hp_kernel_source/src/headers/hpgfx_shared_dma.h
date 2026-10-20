/********************************************************************
 * hpgfx_shared_dma.h
 *
 * Public structures and prototypes for the NT-Style DMA manager
 * leveraged from the NT kernel code
 *
 ********************************************************************/

#ifndef _HPGFX_SHARED_DMA_H
#define _HPGFX_SHARED_DMA_H

#include <asm/ioctl.h>

/********************************************************************
 *
 *  Public enums
 *
 ********************************************************************/

typedef enum {
#ifdef __KERNEL__
        /* Host buffer is a buffer used to store data DMAed from */
        /* the device. There is a very limited number of pages */
        /* of this type.  This is only used in the kernel. */
        DMA_HOST_BUFFER=0,
#endif
        /* This type is a CD buffer. It is always the same size, */
        /* which is determined by an environment variable. These */
        /* buffers tend to be large, and are allocated on the */
        /* opposite end of the memory pool from the other types. */
        /* This is done to avoid fragmenting memory. */
        DMA_CD_BUFFER=1

} hpgfx_DmaAllocType;


typedef enum {
        DMA_SUCCESS=0,
        DMA_FAILURE,
} DMA_ERROR;

/********************************************************************
 *
 *  Public structures
 *
 ********************************************************************/

typedef struct {
	int copyBuffer;
        hpgfx_DmaAllocType type;
	void* virtualAddress;
	void* userAddress;
	int byteCount;
	void* kernelPrivate;
} hpgfx_DmaBuff, *hpgfx_DmaBuffPtr, *hpgfx_pDmaBuff;

typedef void* hpgfx_DmaManager; 
typedef hpgfx_DmaManager* hpgfx_DmaManagerPtr; 

typedef struct {
	hpgfx_DmaManager dmaMgr;
	unsigned char *targetBuff;
	unsigned int targetByteCount;
	unsigned int targetOffset;
	hpgfx_DmaBuff commandBuff;
	unsigned int commandByteCount;
	unsigned int commandOffset;
} hpgfx_DmaWriteInput;

typedef struct {
	hpgfx_DmaManager dmaMgr;
	hpgfx_DmaBuff sourceBuff;
	unsigned int byteCount;
	unsigned int offset;
	unsigned char flushHint;
} hpgfx_DmaReadInput;

#define _DMA_RASTER_UNBUF 0x00000001
#define _DMA_RASTER_FLUSH 0x00000010
#define _DMA_FLAGS_FLUSH                         (0x00000001)
#define _DMA_FLAGS_SEPARATE_MARKER               (0x00000002)

typedef struct {
	hpgfx_DmaManager dmaMgr;
	hpgfx_DmaBuff buff;
	hpgfx_DmaAllocType allocType;
	unsigned long count;
} hpgfx_DmaIoctlData;

#define LOGBUFFERSIZE 4096
typedef struct {
	/* A null terminated string of log buffer text is returned */
	char logBuffer[LOGBUFFERSIZE];
} hpgfx_GetLogData;

#define HPGFX_IOCTL_BASE       'd'
#define HPGFX_IOCTL_NR(n)       _IOC_NR(n)
#define HPGFX_IO(nr)	        _IO(HPGFX_IOCTL_BASE,nr)
#define HPGFX_IOR(nr,size)      _IOR(HPGFX_IOCTL_BASE,nr,size)
#define HPGFX_IOW(nr,size)      _IOW(HPGFX_IOCTL_BASE,nr,size)
#define HPGFX_IOWR(nr,size)     _IOWR(HPGFX_IOCTL_BASE,nr,size)

#define HPGFX_IOCTL_DMA_ALLOC_BUFF        HPGFX_IOWR(0x30, hpgfx_DmaIoctlData)
#define HPGFX_IOCTL_DMA_FREE_BUFF         HPGFX_IOWR(0x31, hpgfx_DmaIoctlData)
#define HPGFX_IOCTL_DMA_INITIALIZE        HPGFX_IOWR(0x32, hpgfx_DmaIoctlData)
#define HPGFX_IOCTL_DMA_CLOSE             HPGFX_IOWR(0x33, hpgfx_DmaIoctlData)
#define HPGFX_IOCTL_DMA_WRITE_TO_HOST     HPGFX_IOWR(0x34, hpgfx_DmaIoctlData)
#define HPGFX_IOCTL_DMA_CD_READ_FROM_HOST HPGFX_IOWR(0x35, hpgfx_DmaIoctlData)
#define HPGFX_IOCTL_DMA_FINISH            HPGFX_IOWR(0x36, hpgfx_DmaIoctlData)
#define HPGFX_IOCTL_DMA_RASTER_TO_DEVICE  HPGFX_IOWR(0x37, hpgfx_DmaIoctlData)
#define HPGFX_IOCTL_DMA_FINISH_ALL        HPGFX_IOWR(0x38, hpgfx_DmaIoctlData)
#define HPGFX_IOCTL_GET_LOG               HPGFX_IOWR(0x39, hpgfx_GetLogData)

#endif 
