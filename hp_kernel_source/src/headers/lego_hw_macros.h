/*******************************************************************
 *
 *    DESCRIPTION:
 *              Lego HW Macros
 *
 *    AUTHOR:
 *              Courtney Goeltzenleuchter
 *
 *    HISTORY:    
 *              Initial version 5/7/98
 *
 *******************************************************************/

#ifndef _LEGO_HW_MACROS_H_
#define _LEGO_HW_MACROS_H_ 1

#ifdef DEBUG
#define REGISTER 
#else
#define REGISTER register
#endif

/* For linux, the cd buffer is a VM buffer, so we don't want the
 * cd buffer pointer to be volatile. */
#if defined(_HPUX_SOURCE)
#   define VOLATILE volatile
#elif defined(_LINUX) || defined(linux)
#   define VOLATILE
#endif

#define LEGO_MAX_COORD ((Uint32) 0x07FF)

/*
 * Define the macros to correctly set up the various dregs pointers. There
 * are three different dregs structures, each one has offsets from 0x00000000.
 * To make them access the real hardware locations, we need to add the offset
 * of the dregs in the control space address. See the initial comment in
 * summit_hw.h for more details.
 *
 * We are also defining a fourth macro. This macro will get the starting
 * address of the CD buffer.
 */

#define LEGO_MISC_BUFFERED_ADDRESS_MAP(_csaddr)  ((char*)(_csaddr) + 0x00000000)

/*
    This define was used to point to the CD buffer to write to,
    which means on Concorde it points to a software CD buffer.
    However, it was meant to point to the HW CD buffer. 
    The driver should use a DIFFERENT mechanism to get to the
    cd buffer pointer - this define should ONLY be used to find
    the HW CD buffer address.
*/

#if defined(NULL_DSM)
        /* This rounds the CD buffer address to an even multiple of 0x10000 */
#       define LEGO_CD_BUFFER_ADDRESS_MAP(_csaddr)      \
                ((void*)((((char *)(_csaddr) + 0x00380000) + 0x10000) & 0xFFFE0000))
#else
        /* On HP-UX, we know that this address is aligned properly
         * to our CD buffer wrapping bit.
         */
#       define LEGO_CD_BUFFER_ADDRESS_MAP(_csaddr) \
                ((void*)((char *)(_csaddr) + 0x00380000))
#endif

#define LEGO_2D_BUFFERED_ADDRESS_MAP(_csaddr)    ((char*)(_csaddr) + 0x00800000)
#define LEGO_3D_BUFFERED_ADDRESS_MAP(_csaddr)    ((char*)(_csaddr) + 0x00C00000)

#ifdef _WINDOWS /* [ */
#include "nt_hw_macros.h"
#else /* ] [ */

#include "lego_hw.h"

#define CD_BLOCK_CLOSE                        0x8000
#define CD_PACKET_HEADER_INCREMENT_ADDRESS        0
#define CD_PACKET_HEADER_NO_INCREMENT_ADDRESS     1
#define CD_PACKET_HEADER_INC_ADDRESS_MASK     0x02000000
#define CD_PACKET_DATA_WORDS_LEN              5
#define CD_PACKET_DATA_WORDS_MASK             0xf8000000
#define CD_PACKET_DATA_WORDS_START            0x08000000
#define CD_PACKET_HEADER_ADDRESS_MASK         0x00ffffff
#define CD_PACKET_HEADER_ADDRESS_START        0x00000001
#define CD_PACKET_HEADER_ADDRESS_LEN          24
#define CD_BUFFER_ADDRESS_OFFSET              0x00380000
#define BLITZEN_SPECULAR_THRESHOLD_ZERO       (0.000000000000000000001)
#define CD_NOP                                0x0

/* Macros for software spooling (i.e. spooling the software cd buffer */
#ifdef DSM_SOFT_SPOOL
#   include <stdio.h>
    extern int _hpLego_spooling;
    extern FILE *_hpLego_spoolFile;
    extern unsigned int *_hpLego_spoolHWAddr;
    
    void _hpLegoDSM_spoolWords(void *hwAddr, void *buf, int numWords);

#   define DSM_SPOOL_DMA_CD_BUFFER(_buf, _numWords) {                   \
        _hpLego_spoolHWAddr = LEGO_CD_BUFFER_ADDRESS_MAP(0);            \
        _hpLegoDSM_spoolWords(_hpLego_spoolHWAddr, _buf.userAddress,    \
                              _numWords);                               \
        _hpLego_spoolHWAddr += _numWords;                               \
    }

#   define DSM_SPOOL_WRITE(_buf, _numWords) {                           \
        _hpLegoDSM_spoolWords(_hpLego_spoolHWAddr, _buf, _numWords);    \
        _hpLego_spoolHWAddr += _numWords;                               \
    }

#   define DSM_SPOOL_WRITE_OFFSET(addr, data, type) {           \
        type _spl_buf = data;                                   \
        _hpLegoDSM_spoolWords(addr, &_spl_buf, sizeof(type)/4); \
    }

#   define DSM_RESET_HW_CD_BUFFER(csaddr)                       \
        _hpLego_spoolHWAddr = LEGO_CD_BUFFER_ADDRESS_MAP(0);    \
        HW_WRITE32_OFFS(csaddr, B3_CD_BUFFER_CTL, 0x04000000);
#else
#   define DSM_SPOOL_DMA_CD_BUFFER(_buf, _numWords)
#   define DSM_SPOOL_WRITE(_buf, _numWords)
#   define DSM_SPOOL_WRITE_OFFSET(addr, data, type)
#   define DSM_RESET_HW_CD_BUFFER(csaddr)                       \
        HW_WRITE32_OFFS(csaddr, B3_CD_BUFFER_CTL, 0x04000000);
#endif

/* How much space (in bytes) to leave free at the end of each cd buffer.
 * We'll leave 64 words (2 packets) */
#define SOFT_CD_BUFFER_PAD                  0x100

#define LEGO_CD_BUFFER_WRAP                 0x04000000 

typedef union LegoHWMapDoubleFloatUnion
{
    struct {
        float           left, right;
    } asFloat;
    double              asDouble;
} LegoHWMapDoubleFloatType;

typedef union LegoHWMapFloatUnion
{
    unsigned int        asUint;
    float               asFloat;
} LegoHWMapFloatType;

/* Define a macro to abstract the check to see if the CD buffer is full.  For
 * direct hardware writes (HP-UX), this is a simple bitwise and with the buffer
 * pointer, but when we're using a software buffer, it's a little more
 * complicated. */
#if defined(_HPUX_SOURCE)

#  define CD_BUFFER_FULL(ptr) ((uintptr_t)ptr & CD_BLOCK_CLOSE)

/* Define macros for writing to the cd buffer.. */
#define CD_WRITE(addr, data, type) {           \
        (*(VOLATILE type*) (addr)) = (data);   \
        (addr) = (((type*) (addr)) + 1);       \
    }

#define CD_WRITE_NO_INC(addr, data, type) {    \
        (*(VOLATILE type*) (addr)) = (data);   \
    }

#elif defined(_LINUX) || defined(linux)

/* DEFERRED HACK .... should conceptually not need the PAD */
#  define CD_BUFFER_FULL(ptr) \
        (((uintptr_t)(ptr))+SOFT_CD_BUFFER_PAD >= (uintptr_t)(LEGO_PRIVATE.shadowState.cdBufferEnd))

/* Define macros for writing to the cd buffer.. */
#define CD_WRITE(addr, data, type) {           \
        (*(type*) (addr)) = (data);   \
        (addr) = (((type*) (addr)) + 1);       \
    }

#define CD_WRITE_NO_INC(addr, data, type) {    \
        (*(type*) (addr)) = (data);   \
    }

#endif 


#define CD_WRITE_FLOAT(addr, data)          CD_WRITE((addr), (data), float)
#define CD_WRITE_UINT32(addr, data)         CD_WRITE((addr), (data), Uint32)
#define CD_WRITE_DOUBLE(addr, data)         CD_WRITE((addr), (data), double)
#define CD_WRITE_FLOAT_NO_INC(addr, data)   CD_WRITE_NO_INC((addr), (data), float)
#define CD_WRITE_UINT32_NO_INC(addr, data)  CD_WRITE_NO_INC((addr), (data), Uint32)
#define CD_WRITE_DOUBLE_NO_INC(addr, data)  CD_WRITE_NO_INC((addr), (data), double)
#define CD_WRITE_INT64(addr, data)          CD_WRITE((addr), (data), long long)
#define CD_WRITE_UINT64(addr, data)         CD_WRITE((addr), (data), unsigned long long)

#define CD_WRITE8(_data,  _addr)            CD_WRITE_NO_INC(_addr, _data, char)
#define CD_WRITE32(_data, _addr)            CD_WRITE_UINT32_NO_INC(_addr, _data)


/*
 * Define macros for direct frame buffer reads and writes. We will access the hardware
 * through macros in case we want to play around with the hardware access later on.
 */

/* LEGO_ADD adds the size of 'num' elements of 'type' to addr */
#define LEGO_ADD(addr, num, type)         (void*)(((type*) (addr)) + (num))

#define HW_WRITE(addr, data, type) {                    \
        DSM_SPOOL_WRITE_OFFSET(addr, data, type);       \
        (*(volatile type*) (addr)) = (data);            \
        (addr) = (((type*) (addr)) + 1);                \
    }

#define HW_WRITE_NO_INC(addr, data, type) {            \
       DSM_SPOOL_WRITE_OFFSET(addr, data, type);       \
       (*(volatile type*) (addr)) = (data);            \
    }

#define HW_WRITE_FLOAT(addr, data)          HW_WRITE((addr), (data), float)
#define HW_WRITE_UINT32(addr, data)         HW_WRITE((addr), (data), Uint32)
#define HW_WRITE_DOUBLE(addr, data)         HW_WRITE((addr), (data), double)
#define HW_WRITE_FLOAT_NO_INC(addr, data)   HW_WRITE_NO_INC((addr), (data), float)
#define HW_WRITE_UINT32_NO_INC(addr, data)  HW_WRITE_NO_INC((addr), (data), Uint32)
#define HW_WRITE_DOUBLE_NO_INC(addr, data)  HW_WRITE_NO_INC((addr), (data), double)
#define HW_WRITE_INT64(addr, data)          HW_WRITE((addr), (data), long long)
#define HW_WRITE_UINT64(addr, data)         HW_WRITE((addr), (data), unsigned long long)

#define HW_WRITE8(_data,  _addr)            HW_WRITE_NO_INC(_addr, _data, char)
#define HW_WRITE32(_data, _addr)            HW_WRITE_UINT32_NO_INC(_addr, _data)
#define HW_READ8(_addr)                     ((Uint8) (_addr))
#define HW_READ32(_addr)                    (_addr)

/* Macros for I/O to a given word offset from the csaddr base */
/* Currently used only by timd */
#define HW_WRITE32_OFFS(addr,offs,data) \
        DSM_SPOOL_WRITE_OFFSET(offs, data, Uint32); \
        (*(volatile Uint32 *)((volatile uintptr_t)(offs)+ \
                              (volatile uintptr_t)(addr)) = (Uint32)(data))

#define HW_READ32_OFFS(addr,offs) \
        (*(volatile Uint32 *)((volatile uintptr_t)(offs)+ \
                              (volatile uintptr_t)(addr)))

#define HW_READ_OFFSET_FLOAT(addr,offs) \
        (*(volatile float *)((volatile uintptr_t)(offs)+ \
                             (volatile uintptr_t)(addr)))

/* Now define the macros to be strongly ordered writes to the hardware */
#define LEGO_STRONG_ORDERED_WRITE_UINT32(_addr, _data)  HW_WRITE_UINT32((_addr), (_data))

#if defined(_HPUX_SOURCE)
/* For linux, we want to differentiate between direct hardware writes
 * and CD buffer writes, thus the new CD_* and HW_ macros.  In order
 * to preserve backward compatibility with the current HPUX driver,
 * we define these MEM_* macros to write directly to hardware. */
#define MEM_WRITE HW_WRITE	HW_WRITE HW_WRITE
#define MEM_WRITE_NO_INC	HW_WRITE_NO_INC
#define MEM_WRITE_FLOAT	        HW_WRITE_FLOAT
#define MEM_WRITE_UINT32	HW_WRITE_UINT32
#define MEM_WRITE_DOUBLE	HW_WRITE_DOUBLE
#define MEM_WRITE_FLOAT_NO_INC	HW_WRITE_FLOAT_NO_INC
#define MEM_WRITE_UINT32_NO_INC	HW_WRITE_UINT32_NO_INC
#define MEM_WRITE_DOUBLE_NO_INC	HW_WRITE_DOUBLE_NO_INC
#define MEM_WRITE_INT64	        HW_WRITE_INT64
#define MEM_WRITE_UINT64	HW_WRITE_UINT64
#define MEM_WRITE8	        HW_WRITE8
#define MEM_WRITE32	        HW_WRITE32
#define MEM_READ8	        HW_READ8
#define MEM_READ32	        HW_READ32
#define MEM_WRITE32_OFFS	HW_WRITE32_OFFS
#define MEM_READ32_OFFS	        HW_READ32_OFFS
#define MEM_READ_OFFSET_FLOAT	HW_READ_OFFSET_FLOAT
#endif

/*
 * The UTD92 architecture defines 4 frame buffer address modes:
 *   1. bit/pixel
 *   2. byte/pixel
 *   3. short/pixel
 *   4. long/pixel
 */
 
/* These macros assume bit/pixel addressing.  */

#define LEGO_BIT_ALIGN_X(x) (((Uint32) (x)) >> 3)
#define LEGO_BIT_ALIGN_Y(y) (((Uint32) (y)) << 8)

#define LEGO_BIT_STRIDE (LEGO_BIT_ALIGN_X(LEGO_MAX_COORD))
#define LEGO_BIT_ADDR(fb_base,x,y) \
        (((uintptr_t) (fb_base)) + (LEGO_BIT_ALIGN_Y (y) | LEGO_BIT_ALIGN_X (x)))

/* These macros assume byte/pixel addressing */

#define LEGO_BYTE_ALIGN_X(x) ((Uint32) (x))
#define LEGO_BYTE_ALIGN_Y(y) (((Uint32) (y)) << 11)

#define LEGO_BYTE_STRIDE (LEGO_BYTE_ALIGN_X(LEGO_MAX_COORD))
#define LEGO_BYTE_ADDR(fb_base,x,y) \
    (((uintptr_t) (fb_base)) + (LEGO_BYTE_ALIGN_Y (y) | LEGO_BYTE_ALIGN_X (x)))

/*
 * Define the macros to correctly set up the various dregs pointers. There
 * are three different dregs structures, each one has offsets from 0x00000000.
 * To make them access the real hardware locations, we need to add the offset
 * of the dregs in the control space address. See the initial comment in
 * summit_hw.h for more details.
 *
 * We are also defining a fourth macro. This macro will get the starting
 * address of the CD buffer.
 */

/*
 * On Lego devices, we will be using the CD buffer interface instead of
 * a FIFO interface. This means instead of writing directly to a register on
 * the hardware, we will be creating a packet. This entire packet o' fun will
 * be written to the hardware. 
 */

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
 * For more information, refer to section 3.5.1 in the Lego96 ERS
 */
#define CREATE_CD_PACKET_HEADER(_count, _noCount, _wrap, _addr) \
    (((_count) << 27) | ((_noCount) << 25) | ((_wrap) << 24) | (_addr))

/*
 * First we will define a group of macros for stuffing a commands into a packet.
 * The command order is address then the data. We are assuming that the packet is
 * 32 bits wide.
 *
 * NOTE: These macros have a side effect in that the _packet parameter gets
 * incremented. So when this macro is called we need to be sure of two things:
 *    1) _packet can be incremented.
 *    2) we want _packet to be incremented
 */
#define WRITE_1INT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data) \
    {                                            \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr)); \
        CD_WRITE_UINT32((cdBufPtr),          \
                 CREATE_CD_PACKET_HEADER(1, 0, 0, ((addr)))); \
        CD_WRITE_UINT32((cdBufPtr), (data)); \
    }

#define WRITE_2INT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2) \
    {                                             \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr));  \
        CD_WRITE_UINT32((cdBufPtr),           \
                 CREATE_CD_PACKET_HEADER(2, 0, 0, ((addr)))); \
        CD_WRITE_UINT32((cdBufPtr), (data1)); \
        CD_WRITE_UINT32((cdBufPtr), (data2)); \
    }

#define WRITE_3INT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3)  \
    {                                             \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr));  \
        CD_WRITE_UINT32((cdBufPtr),           \
                 CREATE_CD_PACKET_HEADER(3, 0, 0, ((addr)))); \
        CD_WRITE_UINT32((cdBufPtr), (data1)); \
        CD_WRITE_UINT32((cdBufPtr), (data2)); \
        CD_WRITE_UINT32((cdBufPtr), (data3)); \
    }

#define WRITE_4INT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4) \
    {                                             \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr));  \
        CD_WRITE_UINT32((cdBufPtr),           \
                 CREATE_CD_PACKET_HEADER(4, 0, 0, ((addr)))); \
        CD_WRITE_UINT32((cdBufPtr), (data1)); \
        CD_WRITE_UINT32((cdBufPtr), (data2)); \
        CD_WRITE_UINT32((cdBufPtr), (data3)); \
        CD_WRITE_UINT32((cdBufPtr), (data4)); \
    }

#define WRITE_5INT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4, data5) \
    {                                             \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr));  \
        CD_WRITE_UINT32((cdBufPtr),           \
                 CREATE_CD_PACKET_HEADER(5, 0, 0, ((addr)))); \
        CD_WRITE_UINT32((cdBufPtr), (data1)); \
        CD_WRITE_UINT32((cdBufPtr), (data2)); \
        CD_WRITE_UINT32((cdBufPtr), (data3)); \
        CD_WRITE_UINT32((cdBufPtr), (data4)); \
        CD_WRITE_UINT32((cdBufPtr), (data5)); \
    }

#define WRITE_8INT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4, data5, data6, data7, data8) \
    {                                             \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr));  \
        CD_WRITE_UINT32((cdBufPtr),           \
                 CREATE_CD_PACKET_HEADER(8, 0, 0, ((addr)))); \
        CD_WRITE_UINT32((cdBufPtr), (data1)); \
        CD_WRITE_UINT32((cdBufPtr), (data2)); \
        CD_WRITE_UINT32((cdBufPtr), (data3)); \
        CD_WRITE_UINT32((cdBufPtr), (data4)); \
        CD_WRITE_UINT32((cdBufPtr), (data5)); \
        CD_WRITE_UINT32((cdBufPtr), (data6)); \
        CD_WRITE_UINT32((cdBufPtr), (data7)); \
        CD_WRITE_UINT32((cdBufPtr), (data8)); \
    }

#define WRITE_1FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data) \
    {                                            \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr)); \
        CD_WRITE_UINT32((cdBufPtr),          \
                 CREATE_CD_PACKET_HEADER(1, 0, 0, ((addr)))); \
        CD_WRITE_FLOAT((cdBufPtr), (data));  \
    }

#define WRITE_2FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2) \
    {                                            \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr)); \
        CD_WRITE_UINT32((cdBufPtr),          \
                 CREATE_CD_PACKET_HEADER(2, 0, 0, ((addr)))); \
        CD_WRITE_FLOAT((cdBufPtr), (data1)); \
        CD_WRITE_FLOAT((cdBufPtr), (data2)); \
    }

#define WRITE_3FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3)  \
    {                                            \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr)); \
        CD_WRITE_UINT32((cdBufPtr),          \
                 CREATE_CD_PACKET_HEADER(3, 0, 0, ((addr)))); \
        CD_WRITE_FLOAT((cdBufPtr), (data1)); \
        CD_WRITE_FLOAT((cdBufPtr), (data2)); \
        CD_WRITE_FLOAT((cdBufPtr), (data3)); \
    }

#define WRITE_4FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4) \
    {                                            \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr)); \
        CD_WRITE_UINT32((cdBufPtr),          \
                 CREATE_CD_PACKET_HEADER(4, 0, 0, ((addr)))); \
        CD_WRITE_FLOAT((cdBufPtr), (data1)); \
        CD_WRITE_FLOAT((cdBufPtr), (data2)); \
        CD_WRITE_FLOAT((cdBufPtr), (data3)); \
        CD_WRITE_FLOAT((cdBufPtr), (data4)); \
    }

#define WRITE_5FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4, data5) \
    {                                            \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr)); \
        CD_WRITE_UINT32((cdBufPtr),          \
                 CREATE_CD_PACKET_HEADER(5, 0, 0, ((addr)))); \
        CD_WRITE_FLOAT((cdBufPtr), (data1)); \
        CD_WRITE_FLOAT((cdBufPtr), (data2)); \
        CD_WRITE_FLOAT((cdBufPtr), (data3)); \
        CD_WRITE_FLOAT((cdBufPtr), (data4)); \
        CD_WRITE_FLOAT((cdBufPtr), (data5)); \
    }

#define WRITE_9FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4, data5, data6, data7, data8, data9) \
    {                                            \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr)); \
        CD_WRITE_UINT32((cdBufPtr),          \
                 CREATE_CD_PACKET_HEADER(9, 0, 0, ((addr)))); \
        CD_WRITE_FLOAT((cdBufPtr), (data1)); \
        CD_WRITE_FLOAT((cdBufPtr), (data2)); \
        CD_WRITE_FLOAT((cdBufPtr), (data3)); \
        CD_WRITE_FLOAT((cdBufPtr), (data4)); \
        CD_WRITE_FLOAT((cdBufPtr), (data5)); \
        CD_WRITE_FLOAT((cdBufPtr), (data6)); \
        CD_WRITE_FLOAT((cdBufPtr), (data7)); \
        CD_WRITE_FLOAT((cdBufPtr), (data8)); \
        CD_WRITE_FLOAT((cdBufPtr), (data9)); \
    }

#define WRITE_16FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4, data5, data6, data7, data8, \
                                                data9, data10, data11, data12, data13, data14, data15, data16) \
    {                                            \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr)); \
        CD_WRITE_UINT32((cdBufPtr),          \
                 CREATE_CD_PACKET_HEADER(16, 0, 0, ((addr)))); \
        CD_WRITE_FLOAT((cdBufPtr), (data1)); \
        CD_WRITE_FLOAT((cdBufPtr), (data2)); \
        CD_WRITE_FLOAT((cdBufPtr), (data3)); \
        CD_WRITE_FLOAT((cdBufPtr), (data4)); \
        CD_WRITE_FLOAT((cdBufPtr), (data5)); \
        CD_WRITE_FLOAT((cdBufPtr), (data6)); \
        CD_WRITE_FLOAT((cdBufPtr), (data7)); \
        CD_WRITE_FLOAT((cdBufPtr), (data8)); \
        CD_WRITE_FLOAT((cdBufPtr), (data9)); \
        CD_WRITE_FLOAT((cdBufPtr), (data10)); \
        CD_WRITE_FLOAT((cdBufPtr), (data11)); \
        CD_WRITE_FLOAT((cdBufPtr), (data12)); \
        CD_WRITE_FLOAT((cdBufPtr), (data13)); \
        CD_WRITE_FLOAT((cdBufPtr), (data14)); \
        CD_WRITE_FLOAT((cdBufPtr), (data15)); \
        CD_WRITE_FLOAT((cdBufPtr), (data16)); \
    }

#define BLITZEN_WRITE_16FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4, data5, data6, data7, data8, \
                                                data9, data10, data11, data12, data13, data14, data15, data16) \
    {                                            \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr)); \
        CD_WRITE_UINT32((cdBufPtr),          \
                 CREATE_CD_PACKET_HEADER(16, 0, 0, ((addr) | 0x0f << 12))); \
        CD_WRITE_FLOAT((cdBufPtr), (data1)); \
        CD_WRITE_FLOAT((cdBufPtr), (data2)); \
        CD_WRITE_FLOAT((cdBufPtr), (data3)); \
        CD_WRITE_FLOAT((cdBufPtr), (data4)); \
        CD_WRITE_FLOAT((cdBufPtr), (data5)); \
        CD_WRITE_FLOAT((cdBufPtr), (data6)); \
        CD_WRITE_FLOAT((cdBufPtr), (data7)); \
        CD_WRITE_FLOAT((cdBufPtr), (data8)); \
        CD_WRITE_FLOAT((cdBufPtr), (data9)); \
        CD_WRITE_FLOAT((cdBufPtr), (data10)); \
        CD_WRITE_FLOAT((cdBufPtr), (data11)); \
        CD_WRITE_FLOAT((cdBufPtr), (data12)); \
        CD_WRITE_FLOAT((cdBufPtr), (data13)); \
        CD_WRITE_FLOAT((cdBufPtr), (data14)); \
        CD_WRITE_FLOAT((cdBufPtr), (data15)); \
        CD_WRITE_FLOAT((cdBufPtr), (data16)); \
    }

#define BLITZEN_WRITE_12FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4, data5, data6, data7, data8, \
                                                data9, data10, data11, data12) \
    {                                            \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr)); \
        CD_WRITE_UINT32((cdBufPtr),          \
                 CREATE_CD_PACKET_HEADER(12, 0, 0, ((addr) | 0x0b << 12))); \
        CD_WRITE_FLOAT((cdBufPtr), (data1)); \
        CD_WRITE_FLOAT((cdBufPtr), (data2)); \
        CD_WRITE_FLOAT((cdBufPtr), (data3)); \
        CD_WRITE_FLOAT((cdBufPtr), (data4)); \
        CD_WRITE_FLOAT((cdBufPtr), (data5)); \
        CD_WRITE_FLOAT((cdBufPtr), (data6)); \
        CD_WRITE_FLOAT((cdBufPtr), (data7)); \
        CD_WRITE_FLOAT((cdBufPtr), (data8)); \
        CD_WRITE_FLOAT((cdBufPtr), (data9)); \
        CD_WRITE_FLOAT((cdBufPtr), (data10)); \
        CD_WRITE_FLOAT((cdBufPtr), (data11)); \
        CD_WRITE_FLOAT((cdBufPtr), (data12)); \
    }

#define BLITZEN_WRITE_9FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4, data5, data6, data7, data8, data9) \
    {                                            \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr)); \
        CD_WRITE_UINT32((cdBufPtr),          \
                 CREATE_CD_PACKET_HEADER(9, 0, 0, ((addr) | 0x08 << 12))); \
        CD_WRITE_FLOAT((cdBufPtr), (data1)); \
        CD_WRITE_FLOAT((cdBufPtr), (data2)); \
        CD_WRITE_FLOAT((cdBufPtr), (data3)); \
        CD_WRITE_FLOAT((cdBufPtr), (data4)); \
        CD_WRITE_FLOAT((cdBufPtr), (data5)); \
        CD_WRITE_FLOAT((cdBufPtr), (data6)); \
        CD_WRITE_FLOAT((cdBufPtr), (data7)); \
        CD_WRITE_FLOAT((cdBufPtr), (data8)); \
        CD_WRITE_FLOAT((cdBufPtr), (data9)); \
    }

#define BLITZEN_WRITE_6FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4, data5, data6) \
    {                                            \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr)); \
        CD_WRITE_UINT32((cdBufPtr),          \
                 CREATE_CD_PACKET_HEADER(6, 0, 0, ((addr) | 0x05 << 12))); \
        CD_WRITE_FLOAT((cdBufPtr), (data1)); \
        CD_WRITE_FLOAT((cdBufPtr), (data2)); \
        CD_WRITE_FLOAT((cdBufPtr), (data3)); \
        CD_WRITE_FLOAT((cdBufPtr), (data4)); \
        CD_WRITE_FLOAT((cdBufPtr), (data5)); \
        CD_WRITE_FLOAT((cdBufPtr), (data6)); \
    }

#define BLITZEN_WRITE_5FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4, data5) \
    {                                            \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr)); \
        CD_WRITE_UINT32((cdBufPtr),          \
                 CREATE_CD_PACKET_HEADER(5, 0, 0, ((addr) | 0x04 << 12))); \
        CD_WRITE_FLOAT((cdBufPtr), (data1)); \
        CD_WRITE_FLOAT((cdBufPtr), (data2)); \
        CD_WRITE_FLOAT((cdBufPtr), (data3)); \
        CD_WRITE_FLOAT((cdBufPtr), (data4)); \
        CD_WRITE_FLOAT((cdBufPtr), (data5)); \
    }

#define BLITZEN_WRITE_3FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3) \
    {                                            \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr)); \
        CD_WRITE_UINT32((cdBufPtr),          \
                 CREATE_CD_PACKET_HEADER(3, 0, 0, ((addr) | 0x02 << 12))); \
        CD_WRITE_FLOAT((cdBufPtr), (data1)); \
        CD_WRITE_FLOAT((cdBufPtr), (data2)); \
        CD_WRITE_FLOAT((cdBufPtr), (data3)); \
    }

#define BLITZEN_WRITE_2FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2) \
    {                                            \
        UPDATE_CD_BUF_PTR((csAddr), (cdBufPtr)); \
        CD_WRITE_UINT32((cdBufPtr),          \
                 CREATE_CD_PACKET_HEADER(2, 0, 0, ((addr) | 0x01 << 12))); \
        CD_WRITE_FLOAT((cdBufPtr), (data1)); \
        CD_WRITE_FLOAT((cdBufPtr), (data2)); \
    }

#if defined(_HPUX_SOURCE)  /* [ */

/*
 * THESE VERSIONS OF THE MACROS ASSUME THAT THE cdBufPtr IS DOUBLE ALIGNED, AND LEAVE UPDATE_CD_BUF_PTR AS AN
 * EXERCISE FOR THE CALLER.  MAKE SURE THAT YOU HAVE ROOM IN THE CD BUFFER!!!!!!!!!!!
 *
 * These macros have two advantages. They avoid the usually unnecessary UPDATE_CD_BUF_PTR and they
 * do a double word write. The double word write helps the PCXU in a big way and still helps the PCXL.
 * In some functions where we take advantage of these macros, the PCXL has seen a 20% performance
 * improvement while PCXU has seen a 100% improvement. PCXW has seen 300% improvement.
 *
 * There is one disadvantage. There is no immediate load to a float register on the PA-RISC processors.
 * As a result, the header gets constructed into a general register, put on the stack, then loaded into
 * the left half of a float register.
 *
 * The DWRITE_1INT_TO_CD_BUFFER can potentially take advantage of the 64 bit registers on
 * PCXU and PCXW. If we have the appropriate definition, we use this more efficient version.
 * Otherwise, we bounce the data off the stack and still get a performance gain.
 *
 * (The occasional screwball ordering is to put the int/float load/store as far apart as possible, so as
 * to help the optimizer.)
 */

#if defined(__LP64__) || defined(PA20_COMPILE)

#define DWRITE_1INT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data) {                \
    register Uint64 _dword_data =                                               \
        (((Uint64)CREATE_CD_PACKET_HEADER(1, 0, 0, ((addr))) << 32) |           \
        (Uint32)(data));                                                        \
    CD_WRITE_UINT64((cdBufPtr), _dword_data);                                  \
}

#else

#define DWRITE_1INT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data) {                \
    LegoMapDoubleType _map;                                                     \
    _map.asUint.left = CREATE_CD_PACKET_HEADER(1, 0, 0, ((addr)));              \
    _map.asUint.right = (data);                                                 \
    CD_WRITE_DOUBLE((cdBufPtr), _map.asDouble);                                \
}

#endif

#define DWRITE_2INT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2) {                                \
        register VOLATILE double *_pPacket=(cdBufPtr);                                                  \
        register LegoHWMapFloatType     _i1,_i2,_i3;                                                    \
        register LegoHWMapDoubleFloatType _d,_e;                                                        \
        _i1.asUint = CREATE_CD_PACKET_HEADER(2, 0, 0, ((addr)));                                        \
        _i2.asUint = (data1);                                                                           \
        _d.asFloat.left  = _i1.asFloat;                                                                 \
        _d.asFloat.right = _i2.asFloat;                                                                 \
        _i3.asUint = (data2);                                                                           \
        _e.asFloat.right = 0.0f; /* nop */                                                              \
        _e.asFloat.left  = _i3.asFloat;                                                                 \
        *_pPacket   = _d.asDouble;                                                                      \
        _pPacket[1] = _e.asDouble;                                                                      \
        (cdBufPtr)  = _pPacket+2;                                                                       \
    }

#define DWRITE_3INT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3) {                         \
        register VOLATILE double *_pPacket=(cdBufPtr);                                                  \
        register LegoHWMapFloatType     _i1,_i2,_i3,_i4;                                                \
        register LegoHWMapDoubleFloatType _d,_e;                                                        \
        _i1.asUint = CREATE_CD_PACKET_HEADER(3, 0, 0, ((addr)));                                        \
        _i2.asUint = (data1);                                                                           \
        _d.asFloat.left  = _i1.asFloat;                                                                 \
        _d.asFloat.right = _i2.asFloat;                                                                 \
        _i3.asUint = (data2);                                                                           \
        _i4.asUint = (data3);                                                                           \
        _e.asFloat.left  = _i3.asFloat;                                                                 \
        _e.asFloat.right = _i4.asFloat;                                                                 \
        *_pPacket   = _d.asDouble;                                                                      \
        _pPacket[1] = _e.asDouble;                                                                      \
        (cdBufPtr)  = _pPacket+2;                                                                       \
    }

#define DWRITE_4INT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4) {                  \
        register VOLATILE double *_pPacket=(cdBufPtr);                                                  \
        register LegoHWMapFloatType     _i1,_i2,_i3,_i4,_i5;                                            \
        register LegoHWMapDoubleFloatType _d,_e,_f;                                                     \
        _i1.asUint = CREATE_CD_PACKET_HEADER(4, 0, 0, ((addr)));                                        \
        _i2.asUint = (data1);                                                                           \
        _i3.asUint = (data2);                                                                           \
        _d.asFloat.left  = _i1.asFloat;                                                                 \
        _d.asFloat.right = _i2.asFloat;                                                                 \
        _e.asFloat.left  = _i3.asFloat;                                                                 \
        _i4.asUint = (data3);                                                                           \
        _i5.asUint = (data4);                                                                           \
        _f.asFloat.right = 0.0f;                                                                        \
        _e.asFloat.right = _i4.asFloat;                                                                 \
        _f.asFloat.left  = _i5.asFloat;                                                                 \
        *_pPacket   = _d.asDouble;                                                                      \
        _pPacket[1] = _e.asDouble;                                                                      \
        _pPacket[2] = _f.asDouble;                                                                      \
        (cdBufPtr)  = _pPacket+3;                                                                       \
    }

#define DWRITE_5INT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4, data5) {           \
        register VOLATILE double *_pPacket=(cdBufPtr);                                                  \
        register LegoHWMapFloatType     _i1,_i2,_i3,_i4,_i5,_i6;                                        \
        register LegoHWMapDoubleFloatType _d,_e,_f;                                                     \
        _i1.asUint = CREATE_CD_PACKET_HEADER(5, 0, 0, ((addr)));                                        \
        _i2.asUint = (data1);                                                                           \
        _i3.asUint = (data2);                                                                           \
        _d.asFloat.left  = _i1.asFloat;                                                                 \
        _d.asFloat.right = _i2.asFloat;                                                                 \
        _e.asFloat.left  = _i3.asFloat;                                                                 \
        _i4.asUint = (data3);                                                                           \
        _i5.asUint = (data4);                                                                           \
        _i6.asUint = (data5);                                                                           \
        _e.asFloat.right = _i4.asFloat;                                                                 \
        _f.asFloat.left  = _i5.asFloat;                                                                 \
        _f.asFloat.right = _i6.asFloat;                                                                 \
        *_pPacket   = _d.asDouble;                                                                      \
        _pPacket[1] = _e.asDouble;                                                                      \
        _pPacket[2] = _f.asDouble;                                                                      \
        (cdBufPtr)  = _pPacket+3;                                                                       \
    }

#define DWRITE_1FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1) {                                     \
        register VOLATILE double *_pPacket=(cdBufPtr);                                                  \
        register LegoHWMapFloatType     _i;                                                             \
        register LegoHWMapDoubleFloatType _d;                                                           \
        _i.asUint = CREATE_CD_PACKET_HEADER(1, 0, 0, ((addr)));                                         \
        _d.asFloat.left  = _i.asFloat;                                                                  \
        _d.asFloat.right = (data1);                                                                     \
        *_pPacket  = _d.asDouble;                                                                       \
        (cdBufPtr) = _pPacket+1;                                                                        \
    }

#define DWRITE_2FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2) {                              \
        register VOLATILE double *_pPacket=(cdBufPtr);                                                  \
        register LegoHWMapFloatType     _i;                                                             \
        register LegoHWMapDoubleFloatType _d,_e;                                                        \
        _i.asUint = CREATE_CD_PACKET_HEADER(2, 0, 0, ((addr)));                                         \
        _d.asFloat.right = (data1);                                                                     \
        _e.asFloat.left  = (data2);                                                                     \
        _d.asFloat.left  = _i.asFloat;                                                                  \
        _e.asFloat.right = (0.0f);                                                                      \
        *_pPacket   = _d.asDouble;                                                                      \
        _pPacket[1] = _e.asDouble;                                                                      \
        (cdBufPtr)  = _pPacket+2;                                                                       \
    }

#define DWRITE_3FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3) {                       \
        register VOLATILE double *_pPacket=(cdBufPtr);                                                  \
        register LegoHWMapFloatType     _i;                                                             \
        register LegoHWMapDoubleFloatType _d,_e,_f;                                                     \
        _i.asUint = CREATE_CD_PACKET_HEADER(3, 0, 0, ((addr)));                                         \
        _d.asFloat.right = (data1);                                                                     \
        _e.asFloat.left  = (data2);                                                                     \
        _d.asFloat.left  = _i.asFloat;                                                                  \
        _e.asFloat.right = (data3);                                                                     \
        *_pPacket   = _d.asDouble;                                                                      \
        _pPacket[1] = _e.asDouble;                                                                      \
        (cdBufPtr)  = _pPacket+2;                                                                       \
    }

#define DWRITE_4FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4) {                \
        register VOLATILE double *_pPacket=(cdBufPtr);                                                  \
        register LegoHWMapFloatType     _i;                                                             \
        register LegoHWMapDoubleFloatType _d,_e,_f;                                                     \
        _i.asUint = CREATE_CD_PACKET_HEADER(4, 0, 0, ((addr)));                                         \
        _d.asFloat.right = (data1);                                                                     \
        _e.asFloat.left  = (data2);                                                                     \
        _e.asFloat.right = (data3);                                                                     \
        _d.asFloat.left  = _i.asFloat;                                                                  \
        _f.asFloat.left  = (data4);                                                                     \
        _f.asFloat.right = (0.0f);                                                                      \
        *_pPacket   = _d.asDouble;                                                                      \
        _pPacket[1] = _e.asDouble;                                                                      \
        _pPacket[2] = _f.asDouble;                                                                      \
        (cdBufPtr)  = _pPacket+3;                                                                       \
    }

#define DWRITE_5FLOAT_TO_CD_BUFFER(csAddr, cdBufPtr, addr, data1, data2, data3, data4, data5) {         \
        register VOLATILE double *_pPacket=(cdBufPtr);                                                  \
        register LegoHWMapFloatType     _i;                                                             \
        register LegoHWMapDoubleFloatType _d,_e,_f;                                                     \
        _i.asUint = CREATE_CD_PACKET_HEADER(5, 0, 0, ((addr)));                                         \
        _d.asFloat.right = (data1);                                                                     \
        _e.asFloat.left  = (data2);                                                                     \
        _e.asFloat.right = (data3);                                                                     \
        _d.asFloat.left  = _i.asFloat;                                                                  \
        _f.asFloat.left  = (data4);                                                                     \
        _f.asFloat.right = (data5);                                                                     \
        *_pPacket   = _d.asDouble;                                                                      \
        _pPacket[1] = _e.asDouble;                                                                      \
        _pPacket[2] = _f.asDouble;                                                                      \
        (cdBufPtr)  = _pPacket+3;                                                                       \
    }

/* 
 * Macros for writing double word packets to CD buffer. One thing to note about these macros
 * is that we use the *_NO_INC version of the write macros. This is done to be more efficient.
 * Since we know how many writes we are doing, we can just do one large increment instead of
 * incrementing everytime we do a write.
 */
#define WRITE_1_DOUBLE_TO_CD_BUFFER(pPacket, regValue1)                                 \
{                                                                                       \
    register VOLATILE double *p = (double *)pPacket;                                    \
    if (!CD_BUFFER_FULL(p))                                                     \
    {                                                                                   \
        CD_WRITE_DOUBLE_NO_INC(p, regValue1.asDouble);                         \
        pPacket = p+1;                                                                  \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        WRAP_CD_BUF_DOUBLE((Uint32 *)LEGO_PRIVATE.shadowState.controlSpaceAddress, p);\
        CD_WRITE_DOUBLE_NO_INC(p, regValue1.asDouble);                         \
        pPacket = p+1;                                                                  \
    }                                                                                   \
}


#define WRITE_2_DOUBLE_TO_CD_BUFFER(pPacket, regValue1, regValue2)                      \
{                                                                                       \
    register VOLATILE double *p = (double *)pPacket;                                    \
    if (!CD_BUFFER_FULL(p))                                                     \
    {                                                                                   \
        CD_WRITE_DOUBLE_NO_INC(p, regValue1.asDouble);                         \
        CD_WRITE_DOUBLE_NO_INC((p+1), regValue2.asDouble);                             \
        pPacket = p+2;                                                                  \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        WRAP_CD_BUF_DOUBLE((Uint32 *)LEGO_PRIVATE.shadowState.controlSpaceAddress, p);\
        CD_WRITE_DOUBLE_NO_INC(p, regValue1.asDouble);                         \
        CD_WRITE_DOUBLE_NO_INC((p+1), regValue2.asDouble);                             \
        pPacket = p+2;                                                                  \
    }                                                                                   \
}


#define WRITE_3_DOUBLE_TO_CD_BUFFER(pPacket, regValue1, regValue2, regValue3)           \
{                                                                                       \
    register VOLATILE double *p = (double *)pPacket;                                    \
    if (!CD_BUFFER_FULL(p))                                                     \
    {                                                                                   \
        CD_WRITE_DOUBLE_NO_INC(p, regValue1.asDouble);                         \
        CD_WRITE_DOUBLE_NO_INC((p+1), regValue2.asDouble);                             \
        CD_WRITE_DOUBLE_NO_INC((p+2), regValue3.asDouble);                             \
        pPacket = p+3;                                                                  \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        WRAP_CD_BUF_DOUBLE((Uint32 *)LEGO_PRIVATE.shadowState.controlSpaceAddress, p);\
        CD_WRITE_DOUBLE_NO_INC(p, regValue1.asDouble);                         \
        CD_WRITE_DOUBLE_NO_INC((p+1), regValue2.asDouble);                             \
        CD_WRITE_DOUBLE_NO_INC((p+2), regValue3.asDouble);                             \
        pPacket = p+3;                                                                  \
    }                                                                                   \
}


#define WRITE_4_DOUBLE_TO_CD_BUFFER(pPacket, regValue1, regValue2, regValue3, regValue4) \
{                                                                                       \
    register VOLATILE double *p = (double *)pPacket;                                    \
    if (!CD_BUFFER_FULL(p))                                                     \
    {                                                                                   \
        CD_WRITE_DOUBLE_NO_INC(p, regValue1.asDouble);                         \
        CD_WRITE_DOUBLE_NO_INC((p+1), regValue2.asDouble);                             \
        CD_WRITE_DOUBLE_NO_INC((p+2), regValue3.asDouble);                             \
        CD_WRITE_DOUBLE_NO_INC((p+3), regValue4.asDouble);                             \
        pPacket = p+4;                                                                  \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        WRAP_CD_BUF_DOUBLE((Uint32 *)LEGO_PRIVATE.shadowState.controlSpaceAddress, p);\
        CD_WRITE_DOUBLE_NO_INC(p, regValue1.asDouble);                         \
        CD_WRITE_DOUBLE_NO_INC((p+1), regValue2.asDouble);                             \
        CD_WRITE_DOUBLE_NO_INC((p+2), regValue3.asDouble);                             \
        CD_WRITE_DOUBLE_NO_INC((p+3), regValue4.asDouble);                             \
        pPacket = p+4;                                                                  \
    }                                                                                   \
}


#define WRITE_5_DOUBLE_TO_CD_BUFFER(pPacket, regValue1, regValue2, regValue3, regValue4, regValue5)     \
{                                                                                       \
    register VOLATILE double *p = (double *)pPacket;                                    \
    if (!CD_BUFFER_FULL(p))                                                     \
    {                                                                                   \
        CD_WRITE_DOUBLE_NO_INC(p, regValue1.asDouble);                         \
        CD_WRITE_DOUBLE_NO_INC((p+1), regValue2.asDouble);                             \
        CD_WRITE_DOUBLE_NO_INC((p+2), regValue3.asDouble);                             \
        CD_WRITE_DOUBLE_NO_INC((p+3), regValue4.asDouble);                             \
        CD_WRITE_DOUBLE_NO_INC((p+4), regValue5.asDouble);                             \
        pPacket = p+5;                                                                  \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        WRAP_CD_BUF_DOUBLE((Uint32 *)LEGO_PRIVATE.shadowState.controlSpaceAddress, p);\
        CD_WRITE_DOUBLE_NO_INC(p, regValue1.asDouble);                         \
        CD_WRITE_DOUBLE_NO_INC((p+1), regValue2.asDouble);                             \
        CD_WRITE_DOUBLE_NO_INC((p+2), regValue3.asDouble);                             \
        CD_WRITE_DOUBLE_NO_INC((p+3), regValue4.asDouble);                             \
        CD_WRITE_DOUBLE_NO_INC((p+4), regValue5.asDouble);                             \
        pPacket = p+5;                                                                  \
    }                                                                                   \
}


#define WRITE_MATRIX_16_AS_DOUBLE_TO_CD_BUFFER(pPacket, header, matrix)                 \
{                                                                                       \
    register VOLATILE double *p = (double *)pPacket;                                    \
    if (!CD_BUFFER_FULL(p))                                                     \
    {                                                                                   \
        CD_WRITE_DOUBLE_NO_INC(p, header->asDouble);                           \
        CD_WRITE_DOUBLE_NO_INC((p+1), *matrix++);                                      \
        CD_WRITE_DOUBLE_NO_INC((p+2), *matrix++);                                      \
        CD_WRITE_DOUBLE_NO_INC((p+3), *matrix++);                                      \
        CD_WRITE_DOUBLE_NO_INC((p+4), *matrix++);                                      \
        CD_WRITE_DOUBLE_NO_INC((p+5), *matrix++);                                      \
        CD_WRITE_DOUBLE_NO_INC((p+6), *matrix++);                                      \
        CD_WRITE_DOUBLE_NO_INC((p+7), *matrix++);                                      \
        CD_WRITE_DOUBLE_NO_INC((p+8), *matrix);                                        \
        pPacket = p+9;                                                                  \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        WRAP_CD_BUF_DOUBLE((Uint32 *)LEGO_PRIVATE.shadowState.controlSpaceAddress, p);\
        CD_WRITE_DOUBLE_NO_INC(p, header->asDouble);                           \
        CD_WRITE_DOUBLE_NO_INC((p+1), *matrix++);                                      \
        CD_WRITE_DOUBLE_NO_INC((p+2), *matrix++);                                      \
        CD_WRITE_DOUBLE_NO_INC((p+3), *matrix++);                                      \
        CD_WRITE_DOUBLE_NO_INC((p+4), *matrix++);                                      \
        CD_WRITE_DOUBLE_NO_INC((p+5), *matrix++);                                      \
        CD_WRITE_DOUBLE_NO_INC((p+6), *matrix++);                                      \
        CD_WRITE_DOUBLE_NO_INC((p+7), *matrix++);                                      \
        CD_WRITE_DOUBLE_NO_INC((p+8), *matrix);                                        \
        pPacket = p+9;                                                                  \
    }                                                                                   \
}

/*
 * Define a macro to align the cd buffer pointer on a double-word boundry
 * The streamlines all assume that the cd buffer pointer is double-aligned,
 * and it is the responsiblity of all dsm code to ensure that the pointer
 * is aligned correctly when -
 *
 *   exiting                 [since a streamline could be called]
 *   calling another routine [unless the caller guarentees that the callee will not use double stores]
 *
 *   - If the cd buffer pointer is not double-aligned
 *       - if the cd buffer pointer is not past the wrap point
 *           - store a 1-word "NOP" packet and advance the cd buffer pointer to a double-word boundry
 *
 * Note that, if the cd buffer pointer is in the "wrap" area, nothing is done.  Thats because the
 * next user will wrap the pointer .. back to a double-aligned boundry.
 */
#define DOUBLE_ALIGN_CD_BUF_PTR(cdBufPtr)                                               \
    {                                                                                   \
        if (((intptr_t)(cdBufPtr) & 0x4) && (!CD_BUFFER_FULL(cdBufPtr))) {              \
            CD_WRITE_UINT32((cdBufPtr), CREATE_CD_PACKET_HEADER(0, 0, 0, 0));          \
        }                                                                               \
    }

/* 
 * THE FOLLOWING 2 MACROS SHOULD BE KEPT IN SYNCH
 */
/*
 * This macro is use to wrap the CD buffer. This macro supports sync calls to
 * the DSM. We will tell Lego to reset the CD buffer pointer back to the
 * top of the CD buffer space. We then need to reset our software CD buffer
 * pointer.
 */
#define WRAP_CD_BUF(csaddr, cdBufPtr)                                                           \
    {                                                                                           \
        UB_CD_BUFFER_CTL_Type _cdBufferCtlData;                                                         \
        void *_csaddr = (((char *) (csaddr)) + B3_CD_BUFFER_CTL);                               \
                                                                                                \
        _cdBufferCtlData.all   = 0;                                                             \
        _cdBufferCtlData.CBP.R = 1;                                                             \
                LEGO_STRONG_ORDERED_WRITE_UINT32(_csaddr, _cdBufferCtlData.all);                \
                ((cdBufPtr)) =  LEGO_CD_BUFFER_ADDRESS_MAP((csaddr));                           \
    }

/*
 * This macro is a clone of WRAP_CD_BUF. However, cdBufPtr in this macro is treated
 * as a double pointer where as it is treated as void * above. Yes we could have
 * written a central macro to take the type of cdBufPtr as an argument. However, we
 * don't want to encourage using any ol' type and we have enough nested macros already.
 */
#define WRAP_CD_BUF_DOUBLE(csaddr, cdBufPtr)                                                    \
    {                                                                                           \
        UB_CD_BUFFER_CTL_Type _cdBufferCtlData;                                                         \
        void *_csaddr = (((char *) (csaddr)) + B3_CD_BUFFER_CTL);                               \
                                                                                                \
        _cdBufferCtlData.all   = 0;                                                             \
        _cdBufferCtlData.CBP.R = 1;                                                             \
                LEGO_STRONG_ORDERED_WRITE_UINT32(_csaddr, _cdBufferCtlData.all);                \
                ((cdBufPtr)) =  (double *) LEGO_CD_BUFFER_ADDRESS_MAP((csaddr));                \
    }

#elif defined(_LINUX) || defined(linux) /* ] [ */

/* The x86 architecture has a terrible time with double word writes.  Redefine
 * these macros so that writes are always Uint32. */

#define DWRITE_1INT_TO_CD_BUFFER        WRITE_1INT_TO_CD_BUFFER
#define DWRITE_2INT_TO_CD_BUFFER        WRITE_2INT_TO_CD_BUFFER
#define DWRITE_3INT_TO_CD_BUFFER        WRITE_3INT_TO_CD_BUFFER
#define DWRITE_4INT_TO_CD_BUFFER        WRITE_4INT_TO_CD_BUFFER
#define DWRITE_5INT_TO_CD_BUFFER        WRITE_5INT_TO_CD_BUFFER
#define DWRITE_1FLOAT_TO_CD_BUFFER      WRITE_1FLOAT_TO_CD_BUFFER
#define DWRITE_2FLOAT_TO_CD_BUFFER      WRITE_2FLOAT_TO_CD_BUFFER
#define DWRITE_3FLOAT_TO_CD_BUFFER      WRITE_3FLOAT_TO_CD_BUFFER
#define DWRITE_4FLOAT_TO_CD_BUFFER      WRITE_4FLOAT_TO_CD_BUFFER
#define DWRITE_5FLOAT_TO_CD_BUFFER      WRITE_5FLOAT_TO_CD_BUFFER

#define WRITE_DOUBLE_REG(p, regValue)                     \
    CD_WRITE_UINT32_NO_INC(p, regValue.asUint.left);      \
    CD_WRITE_UINT32_NO_INC(p+1, regValue.asUint.right);   \

#define WRITE_1_DOUBLE_TO_CD_BUFFER(pPacket, regValue)    \
{                                           \
    register Uint32 *p = pPacket;           \
    MAKE_CD_BUFFER_ROOM((Uint32 *)LEGO_PRIVATE.shadowState.controlSpaceAddress, p, 2); \
    WRITE_DOUBLE_REG(p, regValue);          \
    pPacket = p+2;                          \
}

#define WRITE_2_DOUBLE_TO_CD_BUFFER(pPacket, regValue1, regValue2) \
{                                           \
    register Uint32 *p = pPacket;           \
    MAKE_CD_BUFFER_ROOM((Uint32 *)LEGO_PRIVATE.shadowState.controlSpaceAddress, p, 4); \
    WRITE_DOUBLE_REG(p, regValue1);         \
    WRITE_DOUBLE_REG(p+2, regValue2);       \
    pPacket = p+4;                          \
}

#define WRITE_3_DOUBLE_TO_CD_BUFFER(pPacket, regValue1, regValue2, regValue3) \
{                                          \
    register Uint32 *p = pPacket;          \
    MAKE_CD_BUFFER_ROOM((Uint32 *)LEGO_PRIVATE.shadowState.controlSpaceAddress, p, 6); \
    WRITE_DOUBLE_REG(p, regValue1);        \
    WRITE_DOUBLE_REG(p+2, regValue2);      \
    WRITE_DOUBLE_REG(p+4, regValue3);      \
    pPacket = p+6;                         \
}

#define WRITE_4_DOUBLE_TO_CD_BUFFER(pPacket, regValue1, regValue2, regValue3, regValue4) \
{                                         \
    register Uint32 *p = pPacket;         \
    MAKE_CD_BUFFER_ROOM((Uint32 *)LEGO_PRIVATE.shadowState.controlSpaceAddress, p, 8); \
    WRITE_DOUBLE_REG(p, regValue1);       \
    WRITE_DOUBLE_REG(p+2, regValue2);     \
    WRITE_DOUBLE_REG(p+4, regValue3);     \
    WRITE_DOUBLE_REG(p+6, regValue4);     \
    pPacket = p+8;                        \
}

#define WRITE_5_DOUBLE_TO_CD_BUFFER(pPacket, regValue1, regValue2, regValue3, regValue4, regValue5) \
{                                        \
    register Uint32 *p = pPacket;        \
    MAKE_CD_BUFFER_ROOM((Uint32 *)LEGO_PRIVATE.shadowState.controlSpaceAddress, p, 10); \
    WRITE_DOUBLE_REG(p, regValue1);      \
    WRITE_DOUBLE_REG(p+2, regValue2);    \
    WRITE_DOUBLE_REG(p+4, regValue3);    \
    WRITE_DOUBLE_REG(p+6, regValue4);    \
    WRITE_DOUBLE_REG(p+8, regValue5);    \
    pPacket = p+10;                      \
}

#define WRITE_MATRIX_16_AS_DOUBLE_TO_CD_BUFFER(pPacket, header, matrix) \
{                                          \
    register Uint32 *p = pPacket;          \
    register Uint32 *m = matrix;           \
    MAKE_CD_BUFFER_ROOM((Uint32 *)LEGO_PRIVATE.shadowState.controlSpaceAddress, p, 16); \
    WRITE_DOUBLE_REG(p, (*header));        \
    CD_WRITE_UINT32_NO_INC(p+2, m[0]);     \
    CD_WRITE_UINT32_NO_INC(p+3, m[1]);     \
    CD_WRITE_UINT32_NO_INC(p+4, m[2]);     \
    CD_WRITE_UINT32_NO_INC(p+5, m[3]);     \
    CD_WRITE_UINT32_NO_INC(p+6, m[4]);     \
    CD_WRITE_UINT32_NO_INC(p+7, m[5]);     \
    CD_WRITE_UINT32_NO_INC(p+8, m[6]);     \
    CD_WRITE_UINT32_NO_INC(p+9, m[7]);     \
    CD_WRITE_UINT32_NO_INC(p+10, m[8]);    \
    CD_WRITE_UINT32_NO_INC(p+11, m[9]);    \
    CD_WRITE_UINT32_NO_INC(p+12, m[10]);   \
    CD_WRITE_UINT32_NO_INC(p+13, m[11]);   \
    CD_WRITE_UINT32_NO_INC(p+14, m[12]);   \
    CD_WRITE_UINT32_NO_INC(p+15, m[13]);   \
    CD_WRITE_UINT32_NO_INC(p+16, m[14]);   \
    CD_WRITE_UINT32_NO_INC(p+17, m[15]);   \
    pPacket = p+18;                        \
    matrix += 8;                           \
}

#define DOUBLE_ALIGN_CD_BUF_PTR(cdBufPtr)

#define WRAP_CD_BUF(csaddr, cdBufPtr) \
        {  \
	CLEAR_CD_BUFFER_SEMAPHORE; \
	(cdBufPtr) = WRAP_CD_BUFFER_FUNC( cdBufPtr, 1 );  \
	SET_CD_BUFFER_SEMAPHORE; \
	}

#define WRAP_CD_BUF_DOUBLE(csaddr, cdBufPtr) WRAP_CD_BUF(csaddr, cdBufPtr)

#endif /* ] */


/*
 * Now we want to define the macro to check if we need to go to wrap the CD buffer
 * pointer. To do this, we need to do the following steps:
 *
 *   - Check if we have passed a 256K CD buffer boundary
 *     - If so:
 *       - Tell the hardware to wrap its CD buffer pointer
 *         - We want to do a strongly ordered write when we do this. We will do
 *           this by sending down a NOOP to the CD buffer.
 *       - Wrap the software CD buffer back to the top
 */
#define UPDATE_CD_BUF_PTR(csaddr, cdBufPtr)      \
    {                                            \
        if (CD_BUFFER_FULL(cdBufPtr)) {          \
            WRAP_CD_BUF((csaddr), (cdBufPtr));   \
        }                                        \
     }
/*
 * There are times we need to make sure we won't wrap the CD buffer in the middle
 * of a called routine (because write-to-read is set).
 */
#define MAKE_CD_BUFFER_ROOM(csaddr, cdBufPtr, numWords)                 \
    {                                                                   \
        register Uint32 *wouldBe=(Uint32*)cdBufPtr+(numWords);          \
        if (CD_BUFFER_FULL(wouldBe)) {                                  \
            WRAP_CD_BUF((csaddr), (cdBufPtr));                          \
        }                                                               \
     }

#define CD_BUF_PTR_TYPE VOLATILE void *
#define DECLARE_CD_BUF_PTR(name)                                        \
        REGISTER CD_BUF_PTR_TYPE name


#endif /* ] */

#endif /* _LEGO_HW_MACROS_H_ */
