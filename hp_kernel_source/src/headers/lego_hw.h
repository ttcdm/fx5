/*******************************************************************************
**  (c) Copyright Hewlett-Packard Company, 1998.   All rights are reserved.
**  Copying or other reproduction of this program except for archival purposes
**  is prohibited without the prior written consent of Hewlett-Packard Company. 
**                
**                RESTRICTED RIGHTS LEGEND
**    
**  Use, duplication, or disclosure by the U.S. Government is subject to
**  restrictions as set forth in subdivision (b) (3) (ii) of the Rights in
**  Technical Data and Computer Software clause at  52.227-7013.
**    
**  HEWLETT-PACKARD COMPANY
**  Fort Collins, Colorado
*******************************************************************************/

/*******************************************************************
 *
 *    DESCRIPTION:
 *      This file contains the structure type definitions and
 *      related data (enums, defines, etc) that are needed to
 *      describe the various hardware registers for Lego.
 *
 *      Each developer will need to add structures (if not already
 *      present) that they need. Be sure to follow the Lego naming
 *      conventions!
 *
 *      We will use the BITFIELD macros developed by Cyclone. These
 *      allow us to list the registers in the order we are accustomed
 *      and gives WSY something more useful to start with.
 *
 *      If you need a template to start with, look in summit_hw.h.
 *
 *    AUTHOR:
 *      Courtney Goeltzenleuchter
 *
 *    HISTORY:    
 *      Initial version 6/1/98
 *
 *******************************************************************/

#ifndef _LEGO_HW_H_
#define _LEGO_HW_H_ 1

/** include files **/
#include "bitfield.h"
#include "lego_basictypes.h"
#ifndef XSERVER
#include "lego_address.h"
#endif

/*******************************************************************************
**                
** Bus Interface Registers
**    
*******************************************************************************/

/* PDU IRC (indirect Read Control) */
typedef union {
    struct {
        BITFIELD_3(
    	Uint32	unused1	    	: 29,
    	Uint32	op		:  2,	/* opcode for setting WTR function    */
    	Uint32	WTR		:  1	/* Write To Read state flag           */
        )
    } fields;
    Uint32  bits;
} IRC_Type;

#define PDU_IRC_NOOP	0   /* write is ignored */
#define PDU_IRC_IREAD	1   /* indirect read of IRC register */
#define PDU_IRC_CLEAR	2   /* clear WTR function (disable it) */
#define PDU_IRC_SET	3   /* set WTR function (enable it) */

typedef union {
    struct {
        BITFIELD_3(
        Uint32  unused1         : 12,
        Uint32  SUBID           : 4,    /* Board rev */
        Uint32  unused2         : 16
        )
    } fields;
    Uint32      bits;
} PCI_SUBID_Type;

/* Status register */

typedef union {
    struct {
        BITFIELD_18(
    	Uint32	UBDRdy 	: 2,	/* <31:30> UnBuffered Data Ready */
    	Uint32	BDRdy  	: 2,	/* <29:28> Buffered Data Ready */
    	Uint32	Fault  	: 1,	/* <27> Fault Detected */
    	Uint32	Ubuff  	: 1,	/* <26> Unbuffered Pipe Busy */
    	Uint32	IMODE  	: 1,	/* <25> Input Model (1=>CD Buffer) */
    	Uint32	WPNE  	: 1,	/* <24> Write Pipe Not Empty */
    	Uint32	HIS  	: 1,	/* <23> Host Interrupt Sent */
    	Uint32	res22  	: 1,	/* <22> reserved (was FIR) */
    	Uint32	HWIR  	: 1,	/* <21> High Water Interrupt Req. */
    	Uint32	LWIR  	: 1,	/* <20> Low Water Interrupt Req. */
    	Uint32	TMIR  	: 1,	/* <19> Texture Map Interrupt Req. */
    	Uint32	VBIR  	: 1,	/* <18> Vertical Blank Interrupt Req. */
    	Uint32	UDCR  	: 1,	/* <17> Unbuffered DMA Complete Req. */
    	Uint32	BDCR  	: 1,	/* <16> Buffered DMA Complete Req. */
    	Uint32	UDPR  	: 1,	/* <15> Unbuff Priv DMA Complete Req.*/
    	Uint32	BDPR  	: 1,	/* <14> Buff Priv  DMA Complete Req. */
    	Uint32	res12  	: 2,	/* <13:12> reserved */
    	Uint32	ffc12 	: 12	/* <11:0> FIFO Free Count (LSBs) */
        )
    }	fields;
    Uint32 bits;
} STATUS_Type;


/* PDU CD_BUFFER_CNTL */

typedef union {
    struct {
	Uint32	res0        	:  5;
	Uint32	R		:  1;	/* RESET cd buff                    */
	Uint32	res2		:  2;	/* formerly the Close All bit       */
	Uint32	res1		:  5;	
	Uint32	CBP		: 19;	/* CD Buffer Pointer (512K space )  */
	} CBP;
    struct {
	Uint32	res0        	:  5;
	Uint32	R		:  1;	/* RESET cd buff                    */
	Uint32	res3		:  2;	/* formerly the Close All bit       */
	Uint32	res1		:  5;	
	Uint32  res2		:  7;	/* multiply mapped */
	Uint32  block		:  3;   /* block id from this pointer */
	Uint32  offset		:  7;   /* offset, including safety bit */
	Uint32  zeros		:  2;   /* 2 LSB's, should be zero */
	} decode;
    Uint32  all;
} UB_CD_BUFFER_CTL_Type;

/*
    The following two defines are the possible values (other than 0) for
    UBDRdy and BDRdy.  WordRdy means that there is a single 32bit word
    in the READ_DATA register.  DoubleRdy means there is a double word
    (64 bits) ready. Have to know what you are expecting and wait for
    the appropriate case before reading the data from READ_DATA.
*/
#define TRIAQUA_STATUS_WordRdy		(1)
#define TRIAQUA_STATUS_DoubleRdy	(3)

/*
 * Triaqua DMA setup registers
 */

typedef union {
    struct {
        Uint32 wordCount                : 14;   /* 31-18 */
        Uint32 wordOffset               : 16;   /* 17- 2 */
        Uint32 abort                    : 1;    /*     1 */
        Uint32 valid                    : 1;    /*     0 */
    }   fields;
    Uint32 bits;
} TriaquaDMAOffsetType;

typedef union {
    struct {
        Uint32 unused1                  : 29;   /* 31-3 */
        Uint32 opcode                   : 2;    /*  2-1 */
        Uint32 writeToReadBit           : 1;    /*    0 */
    }   fields;
    Uint32 bits;
} TriaquaIndirectReadControlType;

/* Define opcodes that will be used */
#define TRIAQUA_SET_WRITE_TO_READ       0x03
#define TRIAQUA_CLEAR_WRITE_TO_READ     0x02


/*
    The following two defines are the possible values (other than 0) for
    UBDRdy and BDRdy.  WordRdy means that there is a single 32bit word
    in the READ_DATA register.  DoubleRdy means there is a double word
    (64 bits) ready. Have to know what you are expecting and wait for
    the appropriate case before reading the data from READ_DATA.
*/
#define STATUS_WordRdy		(1)
#define STATUS_DoubleRdy	(3)


/*******************************************************************************
**                
** Video related
**    
*******************************************************************************/

/* Throttle bounds of 4096, 4096 (way offscreen)  which effectively disables throttling. */
#define LEGO_THROTTLE_OFFSCREEN 	(0x10001000)
#define LEGO_THROTTLE_ADJUST		(2)

typedef union {
	struct {
		BITFIELD_2(
        Uint32      yTop    :16,
        Uint32      yBottom :16
		)
    } fields;
    Uint32  bits;
} TCR_Type;



#define YBC_FORMAT_1Y		(0)
#define YBC_FORMAT_2Y		(1)

typedef union {
	struct {
		BITFIELD_3(
        Uint32      brightness	:16,
        Uint32      format	:1,
	Uint32      contrast	:15
		)
    } fields;
    Uint32  bits;
} YBC_Type;


typedef
    union {
        struct {
            Uint32      unused1	: 4;
            Uint32      ALC	: 12;
            Uint32      unused2	: 4;
            Uint32      APC	: 12;
        } fields;
        Uint32  bits;
    } VHAL_Type;


typedef
    union {
        struct {
            Uint32      B	: 1;
            Uint32      P	: 1;
            Uint32      mode	: 1;
            Uint32      FBP	: 1;
            Uint32      unused1	: 28;
        } fields;
        Uint32  bits;
    } SCR_Type;


typedef 
    union {
	struct {
	    Uint32	unused1	: 8;	
	    Uint32	VBP 	: 8;	
	    Uint32	VSYNC	: 8;	
	    Uint32	VFP 	: 8;	
	} fields;
        Uint32	bits;
    } VTG_Type;


/*******************************************************************************
**                
** Retargeter Registers
**    
*******************************************************************************/

typedef union {
    struct {
	    BITFIELD_3(
		Uint32	unused1		:4,
		Uint32	ReleaseBits	:8,
		Uint32	Address		:20
	    )
    } fields;
    Uint32	bits;
} RTG_MEM_LOAD_Type;


/*******************************************************************************
**                
** MFU Registers
**    
*******************************************************************************/

typedef union {
    struct {
        BITFIELD_2(
        Uint32	X	: 16,
        Uint32	Y	: 16
        )
    } fields;
    Uint32		bits;
} MFUData_Type;


/*******************************************************************************
**                
** OFU Registers
**    
*******************************************************************************/

typedef union {
    struct {
        BITFIELD_2(
        Uint32	Y	: 16,
        Uint32	X	: 16
        )
    } fields;
    Uint32		bits;
} WORG_Type;

typedef union {
    struct {
        BITFIELD_7(
        Uint32	unused0	: 24,
        Uint32	D	: 1,
        Uint32	A      	: 3,
        Uint32	P      	: 1,
        Uint32	Y      	: 1,
        Uint32	unused1	: 1,
        Uint32	FY	: 1
        )
    } fields;
    Uint32		bits;
} EN2D_Type;

typedef union {
    struct {
	BITFIELD_2(
	Uint32	unused0	: 31,
	Uint32	V	: 1
	)
    } fields;
    Uint32		bits;
} VBS_Type;

typedef union {
    struct {
        BITFIELD_2(
        Uint32	X1	: 16,
        Uint32	Y1	: 16
        )
    } fields;
    Uint32		bits;
} WCLIP1UL_Type;

typedef union {
    struct {
        BITFIELD_2(
        Uint32	X2	: 16,
        Uint32	Y2	: 16
        )
    } fields;
    Uint32		bits;
} WCLIP1LR_Type;

/*******************************************************************************
**                
** Float To Fix (FTF) Registers
**    
*******************************************************************************/

/*
 * Color scale values for the CSV register
 */
#define CSV_COLOR_SCALE_8I		0xE74
#define CSV_COLOR_SCALE_12I		0xD74
#define CSV_COLOR_SCALE_332F		0x3D
#define CSV_COLOR_SCALE_4F		0x3C
#define CSV_COLOR_SCALE_8F		0x38
#define CSV_COLOR_SCALE_MONOTONIC	0x40

/* FTF doesn't support 565. So, disable color scaling in the FTF unit and use */
/* the new RAST_CSV color scaling register in the rasterizer to handle 565. */
#define CSV_COLOR_SCALE_565F		CSV_COLOR_SCALE_MONOTONIC

typedef union {
    struct {
        BITFIELD_4(
                Uint32 alpha	: 8,
                Uint32 blue	: 8,
                Uint32 green	: 8,
                Uint32 red	: 8
        )
    } fields;
    Uint32 bits;
} RAST_CSV_Type;

#define RAST_CSV_COLOR_SCALE_NONE	0
#define RAST_CSV_COLOR_SCALE_1BIT	1 /* 0x80 or 128/256 */
#define RAST_CSV_COLOR_SCALE_2BIT	2 /* 0xC0 or 192/256 */
#define RAST_CSV_COLOR_SCALE_3BIT	3 /* 0xE0 or 224/256 */
#define RAST_CSV_COLOR_SCALE_4BIT	4 /* 0xF0 or 240/256 */
#define RAST_CSV_COLOR_SCALE_5BIT	5 /* 0xF8 or 248/256 */
#define RAST_CSV_COLOR_SCALE_6BIT	6 /* 0xFC or 252/256 */
#define RAST_CSV_COLOR_SCALE_7BIT	7 /* 0xFE or 254/256 */

/*******************************************************************************
**                
** 3D Rasterizer Registers
**    
*******************************************************************************/

typedef union {
    struct {
        BITFIELD_23(
        Uint32	RAP		: 1,
        Uint32	R3D		: 1,
        Uint32	unused0		: 1,
        Uint32	DC3		: 1,
        Uint32	ZCE		: 1,
        Uint32	MTE		: 4,
        Uint32	IS		: 1,
        Uint32	IA		: 1,
        Uint32	IC		: 1,
        Uint32	unused1		: 2,
        Uint32	PW		: 1,
        Uint32	PAA		: 1,
        Uint32	unused2		: 2,
        Uint32	RLT		: 1,
        Uint32	unused3		: 2,
        Uint32	LTE		: 1,
        Uint32	LW		: 1,
        Uint32	LAA		: 1,
        Uint32	unused4		: 4,
        Uint32	RIT      	: 1,
        Uint32	LFT      	: 1,
        Uint32	BOT      	: 1,
        Uint32	TOP		: 1
        )
    } fields;
    Uint32		bits;
} RMR_Type;

typedef union {
    struct {
        BITFIELD_2(
        Uint32	CNT	: 16,
        Uint32	PAT	: 16
        )
    } fields;
    Uint32		bits;
} LTR_Type;

typedef union {
  struct {
      BITFIELD_7(
      Uint32 z_start_extfrac	: 8,
      Uint32 dz_dx_extfrac	: 8,
      Uint32 dz_de_extfrac	: 8,
      Uint32 maj		: 1,
      Uint32 inc		: 1,
      Uint32 unused		: 3,
      Uint32 op			: 3
      )
  } fields;
  Uint32 bits;
} CMD_Type;

/* CMD.maj Field Values */
#define	CMD_XMAJOR		0x0
#define	CMD_YMAJOR		0x1

/* CMD.inc Field Values */
#define	CMD_DECREMENT		0x0
#define	CMD_INCREMENT		0x1

/* CMD.op Field Values */
#define	CMD_DIAGNOSTICS		0x0
#define	CMD_PRODUCE_VECTOR	0x1
#define	CMD_RESET_LINE_TYPE	0x2
#define	CMD_RLT_PRODUCE_VECTOR	0x3
#define	CMD_PRODUCE_TRIANGLE	0x4
#define	CMD_OPCODE_5		0x5
#define	CMD_PRODUCE_POINT	0x6
#define	CMD_PRODUCE_MIPMAP	0x7

/*******************************************************************************
**                
** Texture Mapping Registers
**    
*******************************************************************************/

/* Enumerated types for MTB register */
typedef enum {
	AOP_ALPHA_IN		= 0,
	AOP_ALPHA_MODULATE	= 1,
	AOP_ALPHA_ADD		= 2,
	AOP_ALPHA_BLEND		= 3
} TextureAlphaOpType;

typedef enum {
	AARG_FRAG_ALPHA		= 0,
	AARG_TEXTURE_ALPHA	= 1,
	AARG_ENV_ALPHA		= 2,
	AARG_ALPHA_IN		= 3
} TextureAlphaArgType;

typedef enum { 
	COP_COLOR_ARG1		= 0,
	COP_COLOR_MODULATE	= 1,
	COP_COLOR_ADD		= 2,
	COP_COLOR_BLEND		= 3
} TextureColorOpType;

typedef enum {
	CARG_FRAG_COLOR		= 0,
	CARG_TEXTURE_COLOR	= 1,
	CARG_TEX_ENV_COLOR	= 2,
	CARG_INPUT_COLOR	= 3,
	CARG_FRAG_ALPHA		= 4,
	CARG_TEXTURE_ALPHA	= 5,
	CARG_TEX_ENV_ALPHA	= 6,
	CARG_INPUT_ALPHA	= 7
} TextureColorArgType;

typedef union {
    struct {
        BITFIELD_8(
        Uint32	unused0		: 6,
        Uint32	FM		: 2,
        Uint32	unused1		: 7,
        Uint32	CDA      	: 1,
        Uint32	unused2		: 6,
        Uint32	NT      	: 2,
        Uint32	unused3		: 7,
        Uint32	TSE		: 1
        )
    } fields;
    Uint32		bits;
} TSS_Type;

/* TSS.FM Field Values */
typedef enum {
    FogModeDepthCue	= 0,
    FogModeLinear	= 1,
    FogModeExp		= 2,
    FogModeExp2		= 3 
} FogModeEnumType;

/* TSS.NT Field Values */
typedef enum {
	TM_ENABLE_1_TEXTURE 	= 0,
	TM_ENABLE_2_TEXTURES	= 1
}NumTexturesEnumType;

/* TSS.TSE Field Values */
typedef enum {
	TM_DISABLE	= 0,
	TM_ENABLE	= 1
} TexSubSysEnableEnumType;

/* Minification modes */
typedef enum {
    MIN_NEAREST			= 0,
    MIN_LINEAR			= 1,
    MIN_NEAREST_MIPMAP_NEAREST	= 2,
    MIN_LINEAR_MIPMAP_NEAREST	= 3,
    MIN_NEAREST_MIPMAP_LINEAR	= 4,
    MIN_LINEAR_MIPMAP_LINEAR	= 5,
    MIN_UNDEFINED6		= 6,
    MIN_PRODUCE_MIPMAP		= 7
} TextureMinType;

/* Magnification modes */
typedef enum {
    MAG_NEAREST			= 0,
    MAG_LINEAR			= 1
} TextureMagType;

/* Texture storage modes */
typedef enum {
    MOD_INTENSITY		= 0,
    MOD_ALPHA			= 1,
    MOD_LUMINANCE		= 2,
    MOD_LUMINANCE_ALPHA		= 3,
    MOD_RGB			= 4,
    MOD_ARGB			= 5,
    MOD_DEPTH			= 6
} TextureModType;

/* Component resolutions */
typedef enum {
    COMPONENT_RESOLUTION_4	= 0,
    COMPONENT_RESOLUTION_8	= 1,
    COMPONENT_RESOLUTION_12	= 2,
    COMPONENT_RESOLUTION_16	= 3,
    COMPONENT_RESOLUTION_24	= 4,
    COMPONENT_RESOLUTION_32	= 5,
    COMPONENT_RESOLUTION_565	= 6,
    COMPONENT_RESOLUTION_1555	= 6
} ComponentResolutionType;

/* Border modes */
typedef enum {
    BM_TEXEL			= 0,
    BM_COLOR			= 1
} TextureBorderType;

/* Boundary types */
typedef enum {
    BOUND_UNDEF0		= 0,
    BOUND_MIRROR		= 1,
    BOUND_REPEAT		= 2,
    BOUND_UNDEF3		= 3,
    BOUND_CLAMP_BORDER		= 4,
    BOUND_CLAMP_BORDER_OGL	= 5,
    BOUND_CLAMP_EDGE		= 6,
    BOUND_CLAMP_COLOR		= 7
} TextureBoundType;

typedef enum {
    TM_DIM_1D			= 0,
    TM_DIM_2D			= 1,
    TM_DIM_3D			= 2
} TextureDimensionType;

typedef enum {
    TM_ENABLE_NONE		= 0,
    TM_ENABLE_S			= 1,
    TM_ENABLE_ST		= 2,
    TM_ENABLE_STR		= 3
} TextureEnableType;

typedef union {
    struct {
    	BITFIELD_4(
		Uint32 A	: 8,
		Uint32 R	: 8,
		Uint32 G	: 8,
		Uint32 B	: 8
    	)
    } fields;
    Uint32 bits;
} TBC_type;

typedef union {
    struct {
    	BITFIELD_16(
		Uint32 Pad0	: 2,	/* 31:30 - unused */
		Uint32 TLO	: 3,	/* 29:27 - Texel LUT format */
		Uint32 SGE	: 1,	/* 26 - Shadow comparison mode  */
		Uint32 SEN	: 1,	/* 25 - Turn on/off shadow texture */
		Uint32 D	: 2,	/* 24:23 - dim. of texture map */
		Uint32 RLI	: 1,	/* 22 - Rational Linear Interpolation */
		Uint32 MIN	: 3,	/* 21:19 - Minification filter */
		Uint32 MAG	: 1,	/* 18 - Magnification filter */
		Uint32 CR	: 3,	/* 17:15 - Component Resolution */
		Uint32 MOD	: 3,	/* 14:12 - Texel format */
		Uint32 BM	: 1,	/* 11 - 0 = texel, 1 = color */
		Uint32 BS	: 3,	/* 10:8 Boundary in S */
		Uint32 BT	: 3,	/* 7:5 Boundary in T */
		Uint32 BR	: 3,	/* 4:2 Boundary in R */
		Uint32 TMLEN	: 1,	/* 1 - Enables use of texel LUT */
		Uint32 PREnPOST	: 1	/* 0 - Pre or post lookup LUT */
    	)
    } fields;
    Uint32 	entire;
} TRO_type;

typedef union {
    struct {
    	BITFIELD_8(
		Uint32 Pad0	: 1,
		Uint32 HCF	: 1,  /* Half cache fetch */
		Uint32 BT	: 2,  /* Log 2 bytes per texel */
		Uint32 MMC	: 4,
		Uint32 Pad1	: 12, /* 23:12 unused */
		Uint32 BSS	: 4,  /* Log 2 size S */
		Uint32 BST	: 4,  /* Log 2 size T */
		Uint32 BSR	: 4   /* Log 2 size R */
    	)
    } fields;
    Uint32 entire;
} TSI_type;

typedef union {
    struct {
    	BITFIELD_6(
		Uint32 Pad0		: 3,
		Uint32 LMAX_int		: 5,
		Uint32 LMAX_frac	: 8,
		Uint32 Pad1		: 3,
		Uint32 LMIN_int		: 5,
		Uint32 LMIN_frac	: 8
    	)
    } fields;
    Uint32 entire;
} TLC_type;

typedef union {
    struct {
    	BITFIELD_3(
		Uint32 Pad0		: 19,
		Uint32 LMIN_int		: 5,
		Uint32 LMIN_frac	: 8
    	)
    } fields;
    Uint32 entire;
} TLA_type;

typedef union {
	struct {
		BITFIELD_4(
		Uint32 A	: 8,
		Uint32 R	: 8,
		Uint32 G	: 8,
		Uint32 B	: 8
		)
	} fields;
	Uint32	entire;
} TEC_type;

typedef union { 
	struct { 
		BITFIELD_15(
		Uint32 pad0	: 2, /* 31:30 unused */
		Uint32 AOP	: 2, /* 29:28 Alpha operation */
		Uint32 pad1	: 2, /* 27:26 unused */
		Uint32 AA1	: 2, /* 25:24 first alpha argument to alpha blend op */
		Uint32 pad2	: 2, /* 23:22 unused */
		Uint32 AA2	: 2, /* 21:20 second alpha arg to alpha blend op */
                Uint32 pad3	: 3, /* 19:17 unused */
                Uint32 AA3	: 1, /* 16 third arg to alpha blend op */
                Uint32 pad4	: 2, /* 15:14 unused */
                Uint32 COP	: 2, /* 13:12 color operation */
                Uint32 pad5	: 2, /* 11:10 unused */
                Uint32 CA1	: 2, /* 9:8 first arg to color blend op */
                Uint32 pad6	: 1, /* 7 unused */
                Uint32 CA2	: 3, /* 6:4 second arg to color op */
                Uint32 pad7	: 4
                )
	} fields;
	Uint32 entire;
} MTB_type;

typedef union {
	struct { 
		BITFIELD_2(
		Uint32 pad0	: 24, /* 31:8 unused */
		Uint32 Thresh	: 8   /* 7:0 max number of commands set to interp */
		)
	} fields;
	Uint32 entire;
} FWT_type;

typedef union { 
	struct {
		BITFIELD_3(
		Uint32 pad0	: 30, 
		Uint32 wipe	: 1,
		Uint32 flush	: 1
		)
	} fields;
	Uint32 entire;
} TWF_type;


/*******************************************************************************
**                
** Fragment Unit Registers
**    
*******************************************************************************/

typedef union {
    struct {
        BITFIELD_16(
        Uint32	unused0		: 3,
        Uint32	TE		: 1,
        Uint32	unused1		: 2,
        Uint32	SE      	: 1,
        Uint32	unused2		: 2,
        Uint32	FE      	: 1,
        Uint32	unused3		: 5,
        Uint32	AT      	: 1,
        Uint32	unused4		: 3,
        Uint32	SK      	: 1,
        Uint32	unused5		: 1,
        Uint32	TSE      	: 1,
        Uint32	LSE      	: 1,
        Uint32	PSE      	: 1,
        Uint32	unused6		: 7,
        Uint32	_2D      	: 1
        )
    } fields;
    Uint32		bits;
} FOE_Type;

typedef union {
    struct {
        BITFIELD_3(
        Uint32	unused0	: 12,
        Uint32	RUL	: 4,
        Uint32	REF	: 16 
        )
    } fields;
    Uint32		bits;
} ATO_Type;

typedef union {
    struct {
        BITFIELD_4(
        Uint32	unused0		: 8,
        Uint32	R		: 8,
        Uint32	G      		: 8,
        Uint32	B		: 8
        )
    } fields;
    Uint32		bits;
} FOG_Type;

typedef union {
    struct {
        BITFIELD_2(
        Uint32	unused0		: 24,
        Uint32	F		: 8
        )
    } fields;
    Uint32		bits;
} FOG_2D_Type;

typedef union {
    struct {
        BITFIELD_4(
        Uint32	unused0		: 5,
        Uint32	AB		: 3,
        Uint32	CB      	: 4,
        Uint32	unused1		: 20
        )
    } fields;
    Uint32		bits;
} TFO_Type;

typedef union {
    struct {
        BITFIELD_2(
        Uint32	Xleft	: 16,
        Uint32	Yupper	: 16
        )
    } fields;
    Uint32		bits;
} WUL_Type;

typedef union {
    struct {
        BITFIELD_2(
        Uint32	Xright	: 16,
        Uint32	Ylower	: 16 
        )
    } fields;
    Uint32		bits;
} WLR_Type;

typedef union {
    struct {
        BITFIELD_10(
        Uint32	unused0	: 12,
        Uint32	E3	: 1,
        Uint32	E2	: 1,
        Uint32	E1	: 1,
        Uint32	E0	: 1,
        Uint32	unused1	: 12,
        Uint32	W3	: 1,
        Uint32	W2	: 1,
        Uint32	W1	: 1,
        Uint32	W0	: 1
        )
    } fields;
    struct {
        BITFIELD_4(
        Uint32	unused0	: 12,
        Uint32	Exclude	: 4,
        Uint32	unused1	: 12,
        Uint32	Enable	: 4
        )
    } masks;
    Uint32		bits;
} WCE_Type;

/*******************************************************************************
**                
** Framebuffer Controller (FBC) Registers
**    
*******************************************************************************/

/** Lego enumerated types */
typedef enum {
    BMAP_ImageData          = 0,
    BMAP_ZData              = 1,
    BMAP_1DTextureData      = 4,
    BMAP_2DTextureData      = 5,
    BMAP_3DTextureData      = 6,
    BMAP_ScaledVideo        = 7
} BmapRegionType;

typedef enum {
    BMAP_1BPP	= 0,
    BMAP_2BPP	= 1,
    BMAP_4BPP	= 2
} BmapRegionDepth;

typedef union {
    struct {
        BITFIELD_4(
        Uint32   BNO : 15,
        Uint32   BS  : 12,
        Uint32   RT  : 3,
        Uint32   RD  : 2
        )
    } fields;
    Uint32      bits;
} BMAP_Type;

typedef union {
    struct {
        BITFIELD_7(
        Uint32  BMAP    : 4,
        Uint32  Y       : 1,
        Uint32  unused1 : 2,
        Uint32  BA      : 13,
        Uint32  unused2 : 6,
        Uint32  BSF1    : 2,
        Uint32  BSF2    : 4
        )
    } fields;
    Uint32      bits;
} AMAP_Type;


typedef union {
	struct {
        BITFIELD_4(
        Uint32	unused1		: 4,
        Uint32	R	        : 4,        /* RT enable */
        Uint32  unused2         : 20,
        Uint32  C               : 4         /* mem ctrl enable */
        )
    } fields;
	Uint32		bits;
} FBC_REN_Type;

typedef union {
	struct {
        BITFIELD_4(
        Uint32	unused1		: 4,
        Uint32	R		: 4,        /* RT enable */
        Uint32  unused2         : 20,
        Uint32  C               : 4         /* mem ctrl enable */
        )
    } fields;
	Uint32		bits;
} FBC_WEN_Type;

typedef union {
    struct {
        BITFIELD_10(
	Uint32	unused0		: 2,
	Uint32	D		: 1,	/* Dual display mode */
        Uint32  unused1         : 12,
        Uint32  SDR		: 1,
        Uint32  unused2         : 2,
        Uint32  DID             : 2,    /* RT Device ID Number */
        Uint32  unused3         : 2,
	Uint32	NUM		: 2,	/* Number of RTs in system (set by SW) */
        Uint32  unused4         : 4,
        Uint32  REVISION        : 4     /* RT Chip Revision */
        )
    } fields;
    Uint32      bits;
} FPR_Type;

/*
 * The following definitions are for the Bitmap Access registers. 
 * For more information, see Lego99 ERS.
 */

/*
 * The ObjectTransCnt field (OTC) of the access register defines how
 * many pixels are present in each longword transfer.  
 */
typedef enum {
    Otc01	= 0,	/* Pixels in each longword transfer (1)     */
    Otc02	= 1,	/* Pixels in each longword transfer (2)     */
    Otc04	= 2,	/* Pixels in each longword transfer (4)     */
    Otc08	= 3,	/* Pixels in each longword transfer (8)     */
    Otc16	= 4,	/* Pixels in each longword transfer (16)    */
    Otc32	= 5,	/* Pixels in each longword transfer (32)    */
    Otc64	= 6,	/* Not implemented in OS 32		  */
    Otc2C	= 7	/* 2 cycle transfer of 32 bits/cycle	  */
} BA_OTC_Enum;

/* Bin Ids */
/* These aren't much use for Lego. */
typedef enum {
    BIN8I	= 0x00,
    BIN4F	= 0x05,
    BIN565F	= 0x06,
    BIN8F	= 0x07,
    BINAttr	= 0x11,
    BINZ	= 0x14,
    BINSten	= 0x15,
    BINHost	= 0x30
} BA_BIN_Enum;

/* DBA/SBA BUF Field Values*/
typedef enum {
    BUF_A		= 0x0,
    BUF_AB		= 0x1,
    BUF_ABC		= 0x2,
    BUF_ABCD		= 0x3
} BA_BUF_Enum;

typedef enum
{
    DBA_Direct			= 0x0,
    DBA_IndirectBackground	= 0x1,
    DBA_IndirectForeground	= 0x2,
    DBA_IndirectBoth		= 0x3
} DBA_DI_Enum;

typedef union {
    struct {
        BITFIELD_14(
	Uint32	O		: 1,
	Uint32	R		: 1,
        Uint32	unused1		: 3,
	Uint32	OTC		: 3,
        Uint32	unused2		: 2,
	Uint32	BIN		: 6,
	Uint32	BUF		: 4,
        Uint32	unused3		: 1,
	Uint32	S		: 1,
	Uint32	P		: 1,
        Uint32	unused4		: 1,
	Uint32	di		: 2, /* This field name cannot be uppercase because */
				     /* it collides with the DI macro in di_public.h */
        Uint32	unused5		: 1,
        Uint32	PS		: 5  /* This is only used to put "r" in for 3D textures */
	)
    } fields;
    Uint32		bits;
} DBA_Type;

typedef union {
    struct {
        BITFIELD_8(
        Uint32	unused0		: 1,
        Uint32	R   		: 1,
        Uint32	unused1		: 3,
        Uint32	OTC     	: 3,
        Uint32	unused2		: 2,
        Uint32	BIN     	: 6,
        Uint32	BUF 		: 4,
        Uint32	unused3		: 12
        )
    } fields;
    Uint32		bits;
} SBA_Type; 

/* This structure is intended for setting IPM, IFC and IBC register values. */
typedef union {
    struct {
    BITFIELD_4(
        Uint32	index4		: 8,
        Uint32	index3		: 8,
        Uint32	index2		: 8,
        Uint32	index 		: 8
    )
    } BIN8I_fields;
    struct {
    BITFIELD_6(
        Uint32	red2		: 5,
        Uint32	green2		: 6,
        Uint32	blue2		: 5,
        Uint32	red		: 5,
        Uint32	green		: 6,
        Uint32	blue		: 5
    )
    } BIN565F_fields;
    struct {
    BITFIELD_8(
        Uint32	alpha2		: 4,
        Uint32	red2		: 4,
        Uint32	green2		: 4,
        Uint32	blue2		: 4,
        Uint32	alpha		: 4,
        Uint32	red		: 4,
        Uint32	green		: 4,
        Uint32	blue		: 4
    )
    } BIN4F_fields;
    struct {
    BITFIELD_4(
        Uint32	alpha		: 8,
        Uint32	red		: 8,
        Uint32	green		: 8,
        Uint32	blue		: 8
    )
    } BIN8F_fields;
    struct {
    BITFIELD_4(
        Uint32	z		: 24,
        Uint32	cp		: 1,
        Uint32	zclr		: 3,
        Uint32	stencil		: 4
    )
    } BIN32Z_fields;
    struct {
    BITFIELD_2(
        Uint32	high		: 16,
        Uint32	low		: 16
    )
    } half;
    Uint32		bits;
} ImageColorAndMaskType;

typedef union {
    struct {
        BITFIELD_2(
        Uint32	BNDELTA		: 16,
        Uint32	unused0		: 16
        )
    } fields;
    Uint32		bits;
} MBW_Type; 


/*******************************************************************************
**                
** Memory Controller (MC) Registers
**    
*******************************************************************************/

typedef union {
    struct {
        BITFIELD_12(
        Uint32	unused0		: 3,
        Uint32	ST      	: 1,
        Uint32	ZT      	: 1,
        Uint32	unused1		: 1,
        Uint32	BL      	: 1,
        Uint32	DT      	: 1,
        Uint32	unused2		: 7,
	Uint32	FCS		: 1,
        Uint32	unused3		: 5,
        Uint32	BEQ      	: 3,
        Uint32	BFA      	: 4,
        Uint32	BFB      	: 4
        )
    } fields;
    Uint32		bits;
} IBO_Type;

/* IBO.BEQ Field Values */
typedef enum
{
    BEQ_BROP                = 0x0,
    BEQ_NewPlusOld          = 0x2,
    BEQ_NewMinusOld         = 0x4,
    BEQ_OldMinusNew         = 0x5,
    BEQ_MinNewOld           = 0x6,
    BEQ_MaxNewOld           = 0x7
} BlendEquationEnumType;

/* IBO.BFA and IBO.BFB Field Values */
typedef enum
{
    BFx_Zero			= 0x0,
    BFx_One			= 0x1,
    BFx_NewColor		= 0x2,
    BFx_OneMinusNewColor	= 0x3,
    BFx_NewAlpha		= 0x4,
    BFx_OneMinusNewAlpha	= 0x5,
    BFx_OldAlpha		= 0x6,
    BFx_OneMinusOldAlpha	= 0x7,
    BFx_OldColor		= 0x8,
    BFx_OneMinusOldColor	= 0x9,
    BFx_AlphaSaturate		= 0xa,
    /* Yep, I meant to skip 0xb */
    BFx_ConstColor		= 0xc,
    BFx_OneMinusConstColor	= 0xd,
    BFx_ConstAlpha		= 0xe,
    BFx_OneMinusConstAlpha	= 0xf
} BlendFunctionEnumType;

/* Boolean Raster Op (BROP) values */
typedef enum {
    RopZeros                = 0x0,
    RopSrcAndDst            = 0x1,
    RopSrcAndNotDst         = 0x2,
    RopSrc                  = 0x3,
    RopNotSrcAndDst         = 0x4,
    RopDst                  = 0x5,
    RopSrcXorDst            = 0x6,
    RopSrcOrDst             = 0x7,
    RopNotSrcAndNotDst      = 0x8,
    RopSrcEquivDst          = 0x9,
    RopNotDst               = 0xa,
    RopSrcOrNotDst          = 0xb,
    RopNotSrc               = 0xc,
    RopNotSrcOrDst          = 0xd,
    RopNotSrcOrNotDst       = 0xe,
    RopOnes                 = 0xf
} BooleanRasterOpEnumType;

typedef union {
    struct {
        BITFIELD_13(
        Uint32  CZ              : 1,
        Uint32  RUL             : 3,
        Uint32  unused0         : 3,
        Uint32  ZEG             : 1,
        Uint32  unused1         : 3,
        Uint32  ZWE             : 1,
        Uint32  unused2         : 1,
        Uint32  VCE             : 1,
        Uint32  unused3         : 8,
        Uint32  ZTE             : 1,
        Uint32  ZRE             : 1,
        Uint32  unused4         : 4,
        Uint32  SWM             : 4
        )
    } fields;
    Uint32              bits;
} ZBO_Type;

typedef union {
    struct {
        BITFIELD_9(
        Uint32	SFAIL      	: 4,
        Uint32	ZFAIL      	: 4,
        Uint32	ZPASS      	: 4,
        Uint32	unused0		: 1,
        Uint32	RUL      	: 3,
        Uint32	unused1		: 4,
        Uint32	MSK      	: 4,
        Uint32	unused2		: 4,
        Uint32	REF      	: 4
        )
    } fields;
    Uint32		bits;
} SBO_Type;

/* SBO.SFAIL, SBO.ZFAIL and SBO.ZPASS Field Values */
typedef enum
{
    SBO_GL_ZERO		= 0x0,
    SBO_GL_KEEP		= 0x1,
    SBO_GL_REPLACE	= 0x2,
    SBO_GL_INCR		= 0x3,
    SBO_GL_DECR		= 0x4,
    SBO_GL_INVERT	= 0x5,
    SBO_SET		= 0x6,
    SBO_D3D_INCR	= 0xB,
    SBO_D3D_DECR	= 0xC
} StencilOpEnumType;

#define	SBO_D3D_INCRSAT	SBO_GL_INCR
#define	SBO_D3D_DECRSAT	SBO_GL_DECR

typedef union {
    struct {
        BITFIELD_3(
        Uint32	O	: 1,
        Uint32	unused0	: 30, 
        Uint32	CP	: 1
        )
    } fields;
    Uint32		bits;
} CPE_Type;

typedef union {
    struct {
        BITFIELD_4(
        Uint32	unused0	: 14,
        Uint32	TY	: 2,
        Uint32	unused1	: 14, 
        Uint32	TX	: 2
        )
    } fields;
    Uint32		bits;
} DOXY_Type;

typedef union {
    struct {
	BITFIELD_5(
	    Uint32	unused0	: 5,
	    Uint32	ICLR	: 3,
	    Uint32	RED	: 8,
	    Uint32	GREEN	: 8,
	    Uint32	BLUE	: 8
	    )
    } fields;
    Uint32		bits;
} FI_Type;

typedef union {
    struct {
	BITFIELD_4(
	    Uint32	Z	: 24,
	    Uint32	unused0	: 1,
	    Uint32	ZCLR	: 3,
	    Uint32	Stencil	: 4
	    )
    } fields;
    Uint32		bits;
} FZ_Type;

#define FZ_ZCLR_MASK	0x00000070	/* Have to use real mask to dodge clip plane & stencil */
#define FI_ICLR_MASK	0xFF000000	/* Use all on for performance reasons */

typedef union {
    struct {
        BITFIELD_8(
		Uint32	Alpha	: 8,
		Uint32	unused0	: 3,
		Uint32	FMT	: 1,
		Uint32	FZE	: 1,
		Uint32	SRE	: 1,
		Uint32	SWE	: 1,
		Uint32	DAE	: 1,
		Uint32	unused1	: 16
		)
    } fields;
    Uint32		bits;
} FCDA_Type;


/* Occlusion culling registers */
typedef union {
    struct {
	BITFIELD_1(
	Uint32	CS	: 32
	)
    } fields;
    Uint32		bits;
} VTR_Type;

typedef union {
    struct {
	BITFIELD_1(
	Uint32	CSMASK	: 32
	)
    } fields;
    Uint32		bits;
} VTE_Type;

typedef union {
    struct {
	BITFIELD_1(
	Uint32	CLR	: 32
	)
    } fields;
    Uint32		bits;
} VTC_Type;

typedef union {
    struct {
	BITFIELD_2(
	Uint32	unused0	: 10,
	Uint32	VPASS	: 22
	)
    } fields;
    Uint32		bits;
} VPCNT_Type;

typedef union {
    struct {
	BITFIELD_2(
	Uint32	unused0	: 10,
	Uint32	VFAIL	: 22
	)
    } fields;
    Uint32		bits;
} VFCNT_Type;


/*******************************************************************************
**                
** Display Unit (SRU) Registers
**    
*******************************************************************************/

typedef union {
    struct { 
        BITFIELD_11( 
        Uint32	I8   		: 1,
        Uint32	unused0		: 3,
        Uint32	FMT   		: 4,
        Uint32	unused1		: 7,
        Uint32	S         	: 1,
        Uint32	unused2		: 7,
        Uint32	CE       	: 1,
        Uint32	GE  		: 1,
        Uint32	unused3 	: 4,
        Uint32	B    		: 1,
        Uint32	LUT      	: 2
        )
    } fields;
    Uint32		bits;
} IMC_Type;

#define IMC_FMT_8I              (0x2)   /* 8bpp index */
#define IMC_FMT_565RGB          (0x4)   /* 16bpp RGB */
#define IMC_FMT_4444ARGB        (0x5)   /* 16bpp ARGB */
#define IMC_FMT_8888XRGB        (0x8)   /* 32bpp RGB */
#define IMC_FMT_8888ARGB        (0x9)   /* 32bpp ARGB */

typedef union {
    struct {
        BITFIELD_7(
        Uint32	unused0		: 4,
        Uint32	FMT   		: 4,
        Uint32	unused1		: 16,
        Uint32	GE  		: 1,
        Uint32	unused3 	: 4,
        Uint32	B    		: 1,
        Uint32	LUT      	: 2
        )
    } fields;
    Uint32		bits;
} OMC_Type;

#define OMC_FMT_8I              (0x2)   /* 8bpp index */
#define OMC_FMT_565RGB          (0x4)   /* 16bpp RGB */
#define OMC_FMT_4444ARGB        (0x5)   /* 16bpp ARGB */
#define OMC_FMT_8888XRGB        (0x8)   /* 32bpp RGB */
#define OMC_FMT_8888ARGB        (0x9)   /* 32bpp ARGB */
#define OMC_FMT_8888XYUV        (0xC)   /* overlay ONLY */
#define OMC_FMT_4444YUYV        (0xD)   /* overlay ONLY */
#define OMC_FMT_4444UYVY        (0xE)   /* overlay ONLY */

typedef union {
    struct {
        BITFIELD_6(
        Uint32	unused0		: 14,
        Uint32	CK1   		: 1,
        Uint32	L1   		: 1,
        Uint32	unused1		: 14,
        Uint32	CK0      	: 1,
        Uint32	L0      	: 1
        )
    } fields;
    Uint32		bits;
} OTLS_Type;


typedef union {
    struct {
        BITFIELD_6(
        Uint32	OSX   		: 1,
        Uint32	unused0		: 3,
        Uint32	X   		: 12,
        Uint32	OSY		: 1,
        Uint32	unused1    	: 3,
        Uint32	Y	      	: 12
        )
    } fields;
    Uint32		bits;
} OXYO_Type;

typedef union {
    struct {
        BITFIELD_2(
        Uint32	unused0		: 31,
        Uint32	BS	      	: 1
        )
    } fields;
    Uint32		bits;
} OBS_Type;

typedef union {
    struct {
        BITFIELD_12(
        Uint32	O_EN   		: 1,
        Uint32	unused0		: 6,
        Uint32	FS_OV  		: 1,
        Uint32	unused1		: 6,
        Uint32	CK_MODE 	: 2,
        Uint32	unused2 	: 3,
        Uint32	A_EQN  		: 1,
        Uint32	unused3 	: 2,
        Uint32	A_MODE     	: 2,
        Uint32	unused4 	: 6,
        Uint32	ST_WIN     	: 1,
        Uint32	ST_FULL    	: 1
        )
    } fields;
    Uint32		bits;
} DUM_Type;

typedef union {
    struct {
        BITFIELD_2(
        Uint32	unused0		: 31,
        Uint32	BS      	: 1
        )
    } fields;
    Uint32		bits;
} IBS_Type;

typedef union {
    struct {
        BITFIELD_7(
		Uint32	LBF		: 1,
		Uint32	unused0		: 2,
		Uint32	RD		: 1,
		Uint32	unused1		: 14,
		Uint32	LUT		: 2,
		Uint32	unused2		: 4,
		Uint32	ADDRESS		: 8
		)
    } fields;
    Uint32		bits;
} LLCA_Type;

#define LLCA_LUT0		0x0
#define LLCA_LUT1		0x1
#define LLCA_LUT2		0x2
#define LLCA_LUT_GAMMA	0x3

typedef union {
    struct {
        BITFIELD_2(
        Uint32	X		: 16,
        Uint32	Y		: 16
        )
    } fields;
    Uint32		bits;
} OUL_Type, OLR_Type;

typedef union {
    struct {
        BITFIELD_4(
        Uint32	unused		: 8,
        Uint32	RED		: 8,
        Uint32	GREEN		: 8,
        Uint32	BLUE		: 8
        )
    } fields;
    Uint32		bits;
} CKEY_HI_Type, CKEY_LOW_Type;

typedef union {
    struct {
        BITFIELD_2(
        Uint32	unused		: 24,
        Uint32	ALPHA		: 8
        )
    } fields;
    Uint32		bits;
} CALPHA_Type;

typedef union 
{
	struct 
	{
		BITFIELD_3(
		Uint32	unused		: 18,
		Uint32	VS1		: 2,
		Uint32	VS0		: 12
		)
	} fields;
	Uint32		bits;
} VS_Type;

#define VS1_VERTICAL_SYNC		(0)
#define VS1_VERTICAL_BACK_PORCH		(1)
#define VS1_ACTIVE_PIXEL		(2)
#define VS1_VERTICAL_FRONT_PORCH	(3)


typedef union
{
	struct 
	{
		BITFIELD_10(
		Uint32	unused		: 12,
		Uint32	POS_VSYNC	: 1,
		Uint32	POS_HSYNC	: 1,
		Uint32	DUAL_MON	: 1,
		Uint32	FP_EN		: 1,
		Uint32	unused1		: 1,
		Uint32	DAC_DISABLE	: 1,
		Uint32	unused2		: 4,
		Uint32	VMS		: 1,
		Uint32	unused3		: 9
		)
	} fields;
	Uint32		bits;
} CFG_Type;

typedef union
{
	struct 
	{
		BITFIELD_5(
		Uint32	unused		: 28,
		Uint32	SCL_PIN		: 1,
		Uint32	SDA_PIN		: 1,
		Uint32	SCL_REG		: 1,
		Uint32	SDA_REG		: 1
		)
	} fields;
	Uint32		bits;
} DDC_Type;

typedef union
{
    struct {
        Uint32  reserved        : 30;
        Uint32  masterValid     : 1;
        Uint32  slaveValid      : 1;
    } Fields;
    Uint32              Bits;
} alphaValidType;

typedef union
{
    struct {
        Uint32  reserved        : 30;
        Uint32  masterValid     : 1;
        Uint32  slaveValid      : 1;
    } Fields;
    Uint32              Bits;
} integerShininessValidType;

/*******************************************************************************
**                
** Geometry Accelerator (GA) Registers
**    
*******************************************************************************/

typedef union {
    struct {
    BITFIELD_29(
	Uint32  FrontPolygonMode	: 2,
	Uint32  BackPolygonMode		: 2,
	Uint32	ReversePolygonOrder	: 1,
	Uint32	BackFaceCullEnable	: 1,
	Uint32	FrontFaceCullEnable	: 1,
	Uint32	IncludeVectorStop	: 1, 
	Uint32	Declassify		: 1, 
	Uint32	FrequencyBiasedLOD	: 1, 
	Uint32	TextureMapEnable	: 2,
	Uint32	TexGenSEnable		: 1,
	Uint32	TexGenTEnable		: 1,
	Uint32	TexGenREnable		: 1,
	Uint32	TexGenQEnable		: 1,
	Uint32	TextureMatrixEnable	: 1,
	Uint32	PolygonAAEnable		: 1,
	Uint32	TextureMapGradientEnable: 1,
	Uint32	SeparateSpecularEnable	: 1,
	Uint32	OglClipEnable		: 1,
	Uint32	DCPrimitive		: 1,
	Uint32	EatPower		: 1,
	Uint32	AlphaEnable		: 1,
	Uint32	DefaultBackFacing	: 1,
	Uint32	RightHandSystem		: 1,
	Uint32	LightingEnable		: 1,
	Uint32	EndpointCappingEnable	: 1,
	Uint32	TossSubpixelVectors	: 1,
	Uint32	UnidirectionalVectors	: 1,
	Uint32	TMPerspectiveEnable	: 1,
	Uint32	PerspectiveEnable	: 1
	)
    } 		fields;
    Uint32	bits;
} GA_RENDER_MODE_Type;

/* GA_RENDER_MODE.FrontPolygonMode/BackPolygonMode Field Values */
typedef enum {
    GA_POLYGON_FILL_MODE 	= 0,
    GA_POLYGON_LINE_MODE 	= 1,
    GA_POLYGON_POINT_MODE	= 2
} GaPolygonModeEnumType;

typedef union {
    struct {
    BITFIELD_18(
	Uint32  IndexModeEnable		: 1,
	Uint32  GLFogExp2Enable		: 1,
	Uint32  GLFogExpEnable		: 1,
	Uint32  GLFogLinearEnable	: 1,
	Uint32  DepthCueEnable		: 1,
	Uint32  AnisotropicXform	: 1,
	Uint32  BackFaceMaterialEnable	: 1,
	Uint32  ReverseNormals		: 1,
	Uint32  SpotLightFlags		: 8,
	Uint32  PositionalFlags		: 8,
	Uint32  PositionalEye		: 1,
	Uint32  UnitVertexNormals	: 1,
	Uint32  RescaleNormals		: 1,
	Uint32  SpecularEnable		: 1,
	Uint32  BackfaceFlipNormals	: 1,
	Uint32  BackfacingPrimitives	: 1,
	Uint32  FrontfacingPrimitives	: 1,
	Uint32  AmbientLightEnable	: 1
    )
    } 		fields;
    Uint32	bits;
} GA_LIGHTING_MODE1_Type;


typedef union {
    struct {
    BITFIELD_16(
	Uint32  NumberModelClipPlanes	: 3,
	Uint32  TexGenSphericalS	: 1,
	Uint32  TexGenSphericalT	: 1,
	Uint32  ColorToMaterialFront	: 1,
	Uint32  ColorToMaterialBack	: 1,
	Uint32  ColorToMaterialAmbient	: 1,
	Uint32  ColorToMaterialDiffuse	: 1,
	Uint32  ColorToMaterialSpecular	: 1,
	Uint32  ColorToMaterialEmission	: 1,
	Uint32  ShadeFlat		: 1,
	Uint32  Emission		: 1,
	Uint32  PointPgonOffsetEnable	: 1,
	Uint32  LinePgonOffsetEnable	: 1,
	Uint32  FillPgonOffsetEnable	: 1,
	Uint32  AttenuationFlags	: 8,
	Uint32  LightsEnabled		: 8
    )
    } 		fields;
    Uint32	bits;
} GA_LIGHTING_MODE2_Type;

/* Polygon offset enable mask. Used to check all 3 polygon offset bits
 * (PointPgonOffsetEnable, LinePgonOffsetEnable, FillPgonOffsetEnable)
 * in the GA_LIGHTING_MODE2 register to see if polygon offset is enabled.
 */
#define GA_POLYGON_OFFSET_ENABLE_MASK 0x00070000

typedef union {
    struct {
    BITFIELD_5(
	Uint32  revId			: 4,
	Uint32  unused1			: 22,
	Uint32	DisableConstAlpha	: 1,
	Uint32	DisableConstColor	: 1,
	Uint32  NumSlices		: 4
    )
    } 		fields;
    Uint32	bits;
} GA_CONFIG_Type;

typedef union {
    struct {
    BITFIELD_3(
	Uint32  unused1		: 29,
	Uint32	ChunkEnable	: 1,
	Uint32  unused2		: 2
    )
    } 		fields;
    Uint32	bits;
} BBC_CONFIG_Type;

typedef union {
    struct {
    BITFIELD_2(
     Uint32	reserved	: 28,
     Uint32	NUM_GA		: 4
    )
    } 		fields;
    Uint32	bits;
} NUM_GA_Type;

/* Verts Per Chunk registers */
typedef union {
    struct {
    BITFIELD_4(
     Uint32	lineStrip	: 8,
     Uint32	lineLoop	: 8,
     Uint32	lines		: 8,
     Uint32	points		: 8
    )
    } 		fields;
    Uint32	bits;
} VPC_0_Type;

typedef union {
    struct {
    BITFIELD_4(
     Uint32	quads		: 8,
     Uint32	triFans		: 8,
     Uint32	triStrip	: 8,
     Uint32	triangles	: 8
    )
    } 		fields;
    Uint32	bits;
} VPC_1_Type;

typedef union {
    struct {
    BITFIELD_3(
     Uint32	reserved	: 16,
     Uint32	polygons	: 8,
     Uint32	quadStrips	: 8
    )
    } 		fields;
    Uint32	bits;
} VPC_2_Type;

typedef union {
    struct {
        BITFIELD_4(
                Uint32 alpha	: 8,
                Uint32 red      : 8,
                Uint32 green	: 8,
                Uint32 blue     : 8
        )
    } fields;
    Uint32 bits;
} CBR_Type;

/*
** Byte swap control register values
*/
#define DSM_NO_BYTE_SWAP             0xe4e4e4e4
#define DSM_RGBA_TO_ARGB_BYTE_SWAP   0x39393939
#define DSM_ARGB_TO_RGBA_BYTE_SWAP   0x93939393
#define DSM_PDU_BYTE_SWAP_DEFAULT    0x1b1b1b1b
#define DSM_DMA_BYTE_SWAP_DEFAULT    0x1b1b1b1b
#define DSM_MFU_BYTE_SWAP_DEFAULT    0x1b1b1b1b
#define DSM_HOST_IS_LITTLE_ENDIAN    0x01010101
#define DSM_HOST_IS_BIG_ENDIAN       0x00000000

/* Define for the texture border size. Not likely to change from  */
/* device to device, but nominally it is device specific. */
#define LEGO_BORDER_SIZE			(1)

#endif /* _LEGO_HW_H_ */
