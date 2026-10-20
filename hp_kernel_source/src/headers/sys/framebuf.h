#ifndef _SYS_FRAMEBUF_INCLUDED	/* allows multiple inclusion */
#define _SYS_FRAMEBUF_INCLUDED

#ifndef _SYS_STDSYMS_INCLUDED
#  ifdef _KERNEL_BUILD
#    include "../h/stdsyms.h"
#  else  /* ! _KERNEL_BUILD */
#    ifndef __i386__
#      include <sys/stdsyms.h>
#    endif /* __i386__ */
#  endif /* _KERNEL_BUILD */
#endif   /* _SYS_STDSYMS_INCLUDED  */

#if defined(_INCLUDE_HPUX_SOURCE) || defined (__i386__)

#ifdef _KERNEL_BUILD
#  include "../h/ioctl.h"
#else  /* ! _KERNEL_BUILD */
#  include <sys/ioctl.h>
#endif /* _KERNEL_BUILD */

#ifdef _KERNEL_BUILD

/* Graphics global function pointer extern declarations */

extern int  (*graphics_pfaultptr)();
extern void (*graphics_exitptr)();
extern void (*graphics_sigptr)();
extern void (*graphics_resumeptr)();
extern void (*graph3_inheritptr)();
#endif /* _KERNEL_BUILD */



#define CRT_MAX_REGIONS		6
#define CRT_NAME_LENGTH		32

/*
 * Note the lock page is actually 4K, but for compatibility, this
 * constant remains at 2K. The crt_fastlock_t structure is 4K bytes
 * in size.
 */

#define LOCKED_PAGE_SIZE	2048	/* locked page size = 2 ** 11 */
					/* user code uses this */
/*
 * Declare the following constants for the FastLock page.  All values
 * are derived from the default page size.
 *   MAX_SLOTS         - the size of the "in-use-bit" slot_array[]
 *   MAX_SLOT_ENTRIES  - the highest # of processes holding FastLocks
 *   ITE_LOCK_SLOT     - the LAST+1 slot is reserved for the ITE
 */
#define MAX_SLOTS		LOCKED_PAGE_SIZE - 2
#define MAX_SLOT_ENTRIES	LOCKED_PAGE_SIZE - 6
#define ITE_LOCK_SLOT		LOCKED_PAGE_SIZE - 4

/* MP
 * Define a "pseudo process ID" for the ITE's use of the FastLock
 */
#define ITE_PSEUDO_PROC_ID	-1

/* MP
 * Define the framebuf timeslice timeout duration assuming a standard
 * 100 ms user process timeslice is in use.
 */
#define FB_TIMESLICE		HZ/10

/*
 * Structure that defines the fastlock page returned by ioctl(GCSLOT)
 * (actually crt_gcslot_t->lock_struct_ptr).
 *
 * MP
 * Add the 'volatile' declaration because the new MP FastLock scheme must
 * rely on "strong ordering" from user-space processes.
 */

struct crt_fastlock_t
{
    volatile short               locking_slot;
    volatile unsigned char       slot_array[MAX_SLOTS];
    int                          future[511];
    volatile unsigned int        StateDescOffset; /* Offset from base of    */
						  /* GRM segment to current */
						  /* state descriptor.      */
};

/*
 * Structure returned by driver (through the third argument)
 * by ioctl(GCSLOT).
 */
struct crt_gcslot_t
{
   struct crt_fastlock_t    *lock_struct_ptr;
   short                    slot;
};

/*
 * Structure is returned by the driver by ioctl(GCDESCRIBE).
 * It provides information sufficient to characterize the attached
 * graphics device/processor.
 */
typedef struct
{
    int     crt_id;             /* display identifier                   */
    int     crt_map_size;       /* size in bytes of the total memory 	*/
    				/* the system maps in for the frame	*/
				/* buffer.				*/
    int     crt_x;              /* width in pixels (displayed part)     */
    int     crt_y;              /* length in pixels (displayed part)    */
    int     crt_total_x;        /* width in pixels: total area          */
    int     crt_total_y;        /* length in pixels: total area         */
    int     crt_x_pitch;        /* length of one row in bytes: total    */
				/* area					*/
    int     crt_bits_per_pixel; /* maximum number of bits that can 	*/
				/* be associated with one screen pixel 	*/
				/* with a one word read or write	*/
    int     crt_bits_used;      /* number of those bits that actually  	*/
				/* can contain valid data.		*/
    int     crt_planes;         /* number of frame buffer planes.  A	*/
                          	/* system wih 8 planes can display 	*/
				/* 256 (2^8) colors.			*/
    int     crt_plane_size;     /* size in bytes of the total frame	*/
    				/* buffer area of a minimun configur-	*/
				/* ation of this graphics device	*/
    char    crt_name[CRT_NAME_LENGTH];  /* null terminated product name */
    unsigned int   crt_attributes;     	/* flags denoting attributes    */
    char    *crt_frame_base;    /* address of the first word in the	*/
                                /* frame buffer memory.            	*/
    char    *crt_control_base;  /* address of the first word of the 	*/
                                /* control registers of the device      */
    char    *crt_region [ CRT_MAX_REGIONS ];       
                                        /* other regions associated     */
                                        /* with the frame buffer that   */
                                        /* might be mapped in      	*/
} crt_frame_buffer_t;

/*
 * Bit definitions of attributes as found in
 * crt_frame_buffer_t->crt_attributes.
 */
#define CRT_Y_EQUALS_2X         0x00000001  /* pixel height is 2x width     */
#define CRT_BLOCK_MOVER_PRESENT 0x00000002  /* hardware Block Mover exists  */
#define CRT_ADVANCED_HARDWARE   0x00000004  /* a hardware Transform Engine  */
                                            /* or other sophisticated       */
                                            /* features exist               */
#define CRT_CAN_INTERRUPT       0x00000008  /* the device is capable of     */
                                            /* generating interrupts        */
#define CRT_GRAPHICS_ON_OFF     0x00000010  /* has GCON, GCOFF capability   */
#define CRT_ALPHA_ON_OFF        0x00000020  /* has GCAON, GCAOFF capability */
#define CRT_VARIABLE_Y_SIZE     0x00000040  /* number of lines in the frame */
                                            /* buffer memory is variable;   */
                                            /* this depends on control      */
                                            /* register settings.           */
#define CRT_ODD_BYTES_ONLY      0x00000080  /* use only odd frame buffer    */
                                            /* bytes                        */
#define CRT_NEEDS_FLUSHING      0x00000100  /* the graphics hardware cache  */
                                            /* needs to be flushed in order */
                                            /* to make the memory image     */
                                            /* current with the display     */
#define CRT_DMA_HARDWARE	0x00000200  /* the "display card" is capable*/
					    /* of supporting hardware DMA   */
#define CRT_VDMA_HARDWARE	0x00000400  /* the interface card is capable*/
					    /* of supporting hardware VDMA  */
#define CRT_DBL_WORD_ACCESS	0x00000800  /* Graphics handles double word */
                                            /* accesses                     */
#define CRT_COLOR_RECOVERY_1	0x00001000  /* Graphics has color recovery  */
                                            /* hardware                     */
#define CRT_YUV_TYPE_1		0x00002000  /* CRX YUV type                */
#define CRT_YUV_TYPE_2		0x00004000  /* HCRX YUV type               */
#define CRT_YUV_TYPE_3		0x00008000  /* VISUALIZE-EG YUV type       */
#define CRT_STEREO              0x00010000  /* stereo mode is enabled       */
#define CRT_GRAYSCALE           0x00020000  /* grayscale mode is enabled    */
#define CRT_NEW_GA		0x40000000  /* VISUALIZE-48XP */
#define CRT_NEW_FBC		0x80000000  /* VISUALIZE-48XP */

/*
 * Structure passed to driver as third parameter of ioctl(GCPHYSADDR).  This 
 * call exports the physical address of the frame buffer and control space.
 */
typedef struct 
{
	char 	*crt_phys_frame_base;
	char 	*crt_phys_control_base;
} gc_phys_addr_t;

/*
 * Structure passed to driver as third parameter of ioctl(GCREGSIGINFO).
 * This call registers a value to be returned via the siginfo structure
 * when entering the SIGGFAULT signal handler.
 */

typedef struct {
	long dev_id;              /* tag returned via siginfo structure */
	long _reserve[4];         /* future use?                        */
} gc_siginfo_t;

/*
 * Structure passed to driver as third parameter for GCHANDLER_DONE
 * ioctl.
 */

typedef enum gc_access_perm {
    GRAPH_ALLOW_ACCESS     = 0,
    GRAPH_DISALLOW_ACCESS  = 1    /* only  disallows buffered path */
} gc_access_perm_t;

typedef struct {
    unsigned int     _pad;
    unsigned int     NewStateDescOffset;
    gc_access_perm_t AccessPerm;
    unsigned int     _reserve[5];
} gc_handlerdone_t;

/*
 * Structure passed to driver as third parameter for GCDMA_ALLOC,
 * GCDMA_SET, GCDMA_FREE, and GCDMA_WAIT ioctl's.
 */

typedef enum gc_dma_flags {
    GCDMA_SAVE_STATE = 0x0001, /* savestate or private */
    GCDMA_UNBUFFERED = 0x0002  /* buffered or unbuffered */
} gc_dma_flags_t;

typedef enum gc_dma_wait_types {
    GCDMA_WAIT_LAST_MARKER = 0,
    GCDMA_WAIT_CURRENT     = 1
} gc_dma_wait_type_t;

typedef struct {
    gc_dma_flags_t flags;
    void *address;       /* return pointer for allocation */
    int size;            /* size of buffer to be allocated */
    gc_dma_wait_type_t wait_type;   /* for GCDMA_WAIT only */
    long _reserve[4];    /* future use? */
} gc_dma_t;

/*
 * Structure passed to driver as third parameter of ioctl(GCFLOWCTRL_SET).
 * This call sets the various flow control parameters.
 */

typedef struct {
    unsigned int highwater_fifo_count;
    unsigned int lowwater_fifo_count;
    unsigned int cycle_stretch_fifo_count;
    unsigned int cycle_stretch_fine;
    unsigned int cycle_stretch_coarse;
    int flowctrl_retry_count;
    unsigned int flowctrl_buffer_zone;
    long _reserve[4];   /* future use? */
} gc_flowctrl_t;

/* Values for ioctl(GCREGLOCK) */

#define GRAPH_LOCK_IMPLICIT        0
#define GRAPH_LOCK_IMPLICIT_PRIV   1
#define GRAPH_LOCK_EXPLICIT        2

/*
 * Structure passed to driver as third parameter of ioctl(GCGETFAULTINFO).
 * This call returns the faulting information for the current thread.
 * NOTE: the file descriptor can be any valid graphics device file
 * descriptor. It does not necessarily have to be the one associated
 * with the faulting device (i.e. the file descriptor is simply used
 * as a hook into the graphics driver, which then determines which
 * device is the current faulting device for this thread).
 */

typedef struct {
	void *lockpage; /* base address of fast lock page */
	long  dev_id;   /* graphics device id */
	long  _reserve[6]; /* future use? */
} gc_getfaultinfo_t;

/* Values for ioctl(GCTEST) */

#define MAX_TEST_PARAM 10

#define GCTEST_HIGHWATER_SIMULATE 0  /* Simulate a highwater condition */

typedef struct {
    int command;
    long param[MAX_TEST_PARAM];
    long pad[10];
} gc_test_t;

/*
 * Structure passed to driver as third parameter of ioctl(GCCONFIG).
 */
#define CRT_SYS_CONFIG_LENGTH   2
#define CRT_CONFIG_LENGTH       3
#define CRT_CONFIG_NUM_LENGTH   3
#define CRT_PAD_LENGTH          7

typedef struct
{
    int crt_id;
    int crt_x;
    int crt_y;
    int crt_x_pitch;
    int crt_planes;
    int cache_stride;
    unsigned int crt_system_config[CRT_SYS_CONFIG_LENGTH];
    unsigned int crt_configuration[CRT_CONFIG_LENGTH];
    unsigned int crt_config_nums[CRT_CONFIG_NUM_LENGTH];
    unsigned int crt_max_dma_size;
    int crt_is_agp_card;
    int pad[CRT_PAD_LENGTH];
} crt_config_t;

typedef struct
{
    unsigned char t[8191];
} gc_8k_t;

/* Bit flags for crt_config_t.crt_system_config[0] */

#define GCCONFIG_ACCESSED_COHERENT                      0x00000001
#define GCCONFIG_MODIFIED_COHERENT                      0x00000002
#define GCCONFIG_ACCESSED_ACC                           0x00000004
#define GCCONFIG_MODIFIED_ACC                           0x00000008
#define GCCONFIG_ACCESSED_DIRTY                         0x00000010
#define GCCONFIG_MODIFIED_DIRTY                         0x00000020

/* Bit flags for crt_config_t.crt_configuration[0] : For VisX devices */

#define GCCONFIG_VISX_GA_PRESENT			0x00000004
#define GCCONFIG_VISX_TM_PRESENT			0x00000008
#define GCCONFIG_VISX_IMAGING_PRESENT			0x00000010
#define GCCONFIG_VISX_VIDEO_OUT_PRESENT			0x00000020
#define GCCONFIG_VISX_VIDEO_OUT_HEADMOUNT_PRESENT	0x00000040
#define GCCONFIG_VISX_ZBUFFER_PRESENT			0x00000080
#define GCCONFIG_VISX_ALPHA_PRESENT			0x00000100
#define GCCONFIG_VISX_ACCUMULATION_PRESENT		0x00000200
#define GCCONFIG_VISX_DIGITAL_VIDEO_PRESENT		0x00000400
#define GCCONFIG_VISX_AUXILARY_PRESENT			0x00000800
#define GCCONFIG_VISX_MPEG_PRESENT			0x00001000
#define GCCONFIG_VISX_EXTERNAL_ACCELERATION_PRESENT	0x00002000

/* Structure for crt_config_t.crt_config_nums[0] : For VisX devcies */
typedef struct
{
	unsigned int ga		:  4;
	unsigned int fbc	:  3;
	unsigned int texmap	:  3;
	unsigned int pad	:  6;
	unsigned int video_out	: 16;
} crt_config_nums_visx_entry0_t;

/* Values for the integer argument to GCNOITE */
#define ITE_ON		0
#define ITE_OFF		1

/* Values for the integer argument to GCSETSELECT */
#define	SELECT_TEXTURE_INTR	0x00000001	/* Device texture interrupt */
#define SELECT_LOCK_REQST	0x00000002	/* Another requesting lock  */
#define SELECT_CONTEXT_INTR	0x00000004	/* Device context interrupt */

/*
 * Bit definitions for Venom "context" control field of *arg parameter 
 * for the ioctl(GCCOPROC) request.
 */
#define VENOM_SET_CONTEXT	0x00000001 /* Change Venom "context" to  */
					   /* indicated graphics device  */
#define VENOM_SET_EXPLICIT 	0x00000002 /* Set for explicit altering  */
					   /* of Venom "context"         */
#define VENOM_SET_IMPLICIT 	0x00000004 /* Set for implicit altering  */
					   /* of Venom "context"         */
#define VENOM_SET_MASK \
	(VENOM_SET_CONTEXT | VENOM_SET_EXPLICIT | VENOM_SET_IMPLICIT)

/*
 * framebuf(DEV) ioctl() command defines
 */
#define	GCID			_IOR('G',0,int)
#define	GCON			_IO('G',1)
#define	GCOFF			_IO('G',2)
#define	GCAON			_IO('G',3)
#define	GCAOFF			_IO('G',4)
#define	GCMAP			_IO('G',5)
#define	GCUNMAP			_IO('G',6)
#define	GCLOCK_BLOCKSIG		_IO('G',7)
#define	GCUNLOCK		_IO('G',8)
#define	GCLOCK			_IO('G',9)
#define GCSTATIC_MAP		_IO('G',11)
#define GCSTATIC_CMAP		_IO('G',11)
#define GCVARIABLE_MAP		_IO('G',12)
#define GCVARIABLE_CMAP		_IO('G',12)
#define GCRESET			_IO('G',18)
#define GCTERM 			_IOWR('G',20,int)
#define GCDESCRIBE		_IOR('G',21,crt_frame_buffer_t)
#define	GCLOCK_BLOCKSIG_NOWAIT	_IO('G',22)
#define	GCLOCK_NOWAIT		_IO('G',23)
#define GCDMA_ALLOC             _IOWR('G',24,gc_dma_t)
#define	GCSLOT			_IOR('G',25,struct crt_gcslot_t)
#define	GCFASTLOCK		_IO('G',26)
#define	GCFASTLOCK_NOWAIT	_IO('G',27)
#define GCDMA_SET               _IOW('G',28,gc_dma_t)
#define GCTEST                  _IOWR('G',29,gc_test_t)
#define GCCOPROC 		_IOW('G',30,int)
#define GCNOITE			_IO('G',32)
#define GCPHYSADDR		_IOR('G',35,gc_phys_addr_t)
/* Note: _IO 36, and 37 are reserved for internal use */
#define GCREGLOCK               _IO('G',38)
#define GCCONFIG                _IOR('G',39,crt_config_t)
#define GCREGSIGINFO            _IOW('G',40,gc_siginfo_t)
#define GCHANDLER_DONE          _IOW('G',41,gc_handlerdone_t)
/* Note: _IO 42 is reserved for internal use */
#define GCDMA_FREE              _IOW('G',43,gc_dma_t)
#define GCDMA_WAIT              _IOW('G',44,gc_dma_t)
#define GCSETSELECT		_IO('G',45)
#define GCSETMON		_IO('G',46)
#define GCFLOWCTRL_SET          _IOW('G',47,gc_flowctrl_t)
#define GCTMTAB                 _IOW('G',48,gc_8k_t)
#define GCVERTBLANK_WAIT	_IO('G',49)
#define GCGETFAULTINFO          _IOR('G',50,gc_getfaultinfo_t)
#define GCDIAG_COMMAND          _IOWR('G',99,struct crt_diag_args)
#define GCGETMONTYPE		_IOR('G',51,int)
#define GCSETMONTYPE		_IO('G',52)
#define GCCLEMENCY		_IO('G',53)

/*
 * Following are the Frame Buffer Identification numbers.  Each frame
 * buffer has a unique id returned by the GCDESCRIBE ioctl() command
 * in the 'crt_id' field.  The same id is also returned by the GCID call.
 *
 * The Series 800 identifies every hardware graphics subsystem uniquely,
 * while the Series 300 often groups families of graphics subsystems
 * together under one major category.  This is, for instance, why
 * S9000_ID_98550 is only defined below for the Series 800, because the
 * Series 300 has historically grouped it under the ID for a family
 * of displays.
 */
#define S9000_ID_98204A         1
#define S9000_ID_9826A          2
#define S9000_ID_9836A          3
#define S9000_ID_9836C          4
#define S9000_ID_98627A         5
#define S9000_ID_98204B         6
#define S9000_ID_9837           7
#define S9000_ID_98700          8
#define S9000_ID_S300           9
#define S9000_ID_98720		10

#define S9000_ID_98550	11

#define S9000_ID_A1096A		12
#define S9000_ID_FRI		13
#define S9000_ID_98730		14
#define S9000_ID_98705		0x26C08070
#define S9000_ID_98736		0x26D148AB
#define S9000_ID_A1659A		0x26D1482A
#define S9000_ID_A1439A		0x26D148EE
#define S9000_ID_ELM		0x26D1488C
#define S9000_ID_98765		0x27480DEF
#define S9000_ID_ELK_768	0x27482101
#define S9000_ID_STINGER	0x27A4A402

/* 
 * framebuffer diagnostic structure for series 700(graph3).  This
 * structure comes from the series 300.
 */
struct crt_diag_args {
    int          request;               /* function to perform          */
    union {
        unsigned int diag_attributes;   /* flags denoting attributes */
	unsigned char *diag_mapaddr;	/* base of mapped address space */
    } u_diag;
};

/*
 * Legal diagnostic request values for the graph2 driver
 */
#define GRAPH_DIAG_IDENT           0	/* return four words of IODC */
#define GRAPH_DIAG_WHICH_INTERFACE 1	/* return interface card ID */
#   define GRAPH_INTERFACE_A1017A  0
#   define GRAPH_INTERFACE_A1047A  1
#define GRAPH_DIAG_READ_REG        2	/* read  "allowed" hpa register */
#define GRAPH_DIAG_WRITE_REG       3	/* write "allowed" hpa register */
#define GRAPH_DIAG_WHICH_DISPLAY   5	/* 0 for display A, 1 for display B */

#define GRAPH_DIAG_RESET           4	/* Resets graphics device. */
#define GRAPH_DIAG_ATTRIBUTES      6 /* Checks to see if device is console */
#define GRAPH_DIAG_MAPADDR	   7	/* Returns base of mapped space */

#define GRAPH_DIAG_ITE		 0x1   /* ITE on this display */

#endif /* _INCLUDE_HPUX_SOURCE */

#endif /* _SYS_FRAMEBUF_INCLUDED */


