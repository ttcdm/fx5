/*------------------------------------------------------------------------------

			  Summit Talk Software Module

--------------------------------------------------------------------------------

		   G R A P H I C S   H A R D W A R E   L A B
				    ########
			 H E W L E T T   P A C K A R D

--------------------------------------------------------------------------------

	 The following is Hewlett Packard Proprietary and Confidential

--------------------------------------------------------------------------------


       (c) Copyright 1995 Hewlett-Packard Company.  All rights reserved.

       Copying or other reproduction of this program except for archival
	  purposes is prohibited without the prior written consent of
			    Hewlett-Packard Company.

			    RESTRICTED RIGHTS LEGEND

	Use, duplication, or disclosure by the Government is subject to
     restrictions as set forth in subdivision (c) (1) (ii) of the Rights in
	  Technical Data and Computer Software clause at 252.227-7013.

			    HEWLETT-PACKARD COMPANY
			     3404 East Harmony Road
			    Fort Collins, CO  80525

------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------

    Description:  Defines and typedefs for the Summit Architecture address map

    $Header: /lego99/support/summit_address.h 1.249 1998/10/06 18:04:53 krberry Exp $

    $Source: /lego99/support/summit_address.h $
    $Revision: 1.249 $
    $State: Exp $
    $Date: 1998/10/06 18:04:53 $
     
------------------------------------------------------------------------------*/

#ifndef LEGO_ADDRESS_H /* { */
#define LEGO_ADDRESS_H


/*-----------------------------
    base field
------------------------------*/
#define SAM_BASE	0x00	/* base field				    */

/*-----------------------------
    frame buffer /control select
------------------------------*/
#define SAM_CTL		0x00	/* control space 			    */
#define SAM_FB		0x01	/* frame buffer space 			    */
#define SAM_FB1		0x02	/* frame buffer space 			    */
#define SAM_FB2		0x03	/* frame buffer space 			    */

/*-----------------------------
    control space select defines
------------------------------*/
#define SAM_MISC	0x00	/* misc control space 			    */
#define SAM_UNBUF	0x01	/* unbuffered control space 		    */
#define SAM_2DBUF	0x02	/* 2D buffered control space 		    */
#define SAM_3DBUF	0x03	/* 3D buffered control space 		    */


/*-----------------------------
    misc. control space defines
------------------------------*/
#define SAM_STI_A	    0x00    /* STI ROM address space		    */
#define SAM_STI_B	    0x01    /* STI ROM address space		    */
#define SAM_STI_C	    0x02    /* STI ROM address space		    */
#define SAM_STI_D	    0x03    /* STI ROM address space		    */
#define SAM_UNBUF_PRIV	    0x04    /* unbuffered privileged address space  */
#define SAM_BUF_PRIV	    0x05    /* buffered privileged address space    */
#define SAM_PRIV_DIRECT_CD  0x06    /* direct CD buffer address space	    */
#define SAM_DIRECT_CD	    0x07    /* direct CD buffer address space	    */


/*-----------------------------
    category select defines
------------------------------*/
#define SAM_VDP  	0x00	/* Video Display Processor Category	    */
#define SAM_VIDEO_OUT	0x01	/* Video Out Category			    */
#define SAM_GA_A	0x04	/* Geometry Accelerator Category A	    */
#define SAM_GA_B	0x05	/* Geometry Accelerator Category B	    */
#define SAM_FTF		0x06	/* Float To Fix Conversion Category	    */
#define SAM_TM		0x07	/* Texture Mapping Category	    	    */
#define SAM_RASTER	0x08	/* Rasterizer Category	    		    */
#define SAM_FBC		0x09	/* Frame Buffer Controller Category	    */
#define SAM_IMAGE	0x0C	/* Imaging Category	    		    */
#define SAM_VOLUME	0x0D	/* Volumetric Category	    		    */
#define SAM_RETARG	0x0E	/* Retargeter Category		    	    */
#define SAM_SHARED	0x10	/* Shared Category		    	    */
#define SAM_HOSTIF	0x12	/* Host Interface Category		    */
#define SAM_BINC	0x13	/* Summit Binc Category		    	    */
#define SAM_DMA	    	0x15	/* DMA Controller Category   	    	    */
#define SAM_OFU		0x16	/* Object Function Unit Category    	    */
#define SAM_OGL_ST	0x17	/* OGL State Category    	            */
#define SAM_MFU	    	0x18	/* Macro Function Unit Category   	    */
#define SAM_MFU_REMAP   0x19	/* Macro Function Remapped Category 	    */


/*------------------------------------------------------------------------------

    Typedefs and defines for interpreting Summit byte enables

------------------------------------------------------------------------------*/
#ifndef NGLE_INTERPRETER /* { */
typedef
    union {
	struct {
	    unsigned  unused1	: 28;   /* upper bits do not really exist */
	    unsigned  BE0	: 1;    /* longword <03:03>               */
	    unsigned  BE1	: 1;    /* longword <02:02>               */
	    unsigned  BE2	: 1;    /* longword <01:01>               */
	    unsigned  BE3	: 1;    /* longword <00:00>               */
	} Bits;
	struct {
	    unsigned  unused1	: 28;   /* upper bits do not really exist */
	    unsigned  all_four	: 4;    /* all of the byte enables        */
	} packed;
	unsigned long       all;
    } SummitBE;				/*  byte enables */

#endif /* } */



/*------------------------------------------------------------------------------

    Defines for dealing with Summit addresses

------------------------------------------------------------------------------*/
/* general base addresses */
#define SUMMIT_BASE	    ((SAM_BASE << 25) & 0xfe000000)
#define CTL_BASE	    (SUMMIT_BASE + 0x00000000)
#define FB_BASE	    	    (SUMMIT_BASE + 0x01000000)
#define FB1_BASE	    (SUMMIT_BASE + 0x02000000)
#define FB2_BASE	    (SUMMIT_BASE + 0x03000000)
#define MISC_CTL_BASE	    (CTL_BASE + 0x00000000)
#define UNBUF_CTL_BASE	    (CTL_BASE + 0x00400000)
#define BUF2D_CTL_BASE	    (CTL_BASE + 0x00800000)
#define BUF3D_CTL_BASE	    (CTL_BASE + 0x00C00000)
#define STI_BASE	    (MISC_CTL_BASE + 0x00000000)
#define UNBUF_PRIV_BASE	    (MISC_CTL_BASE + 0x00200000)
#define BUF_PRIV_BASE	    (MISC_CTL_BASE + 0x00280000)
#define PRIV_DIRECT_CD      (MISC_CTL_BASE + 0x00300000)
#define DIRECT_CD_BASE	    (MISC_CTL_BASE + 0x00380000)

/* unbuffered privileged base addresses */
#define UP_VDP_BASE	    (UNBUF_PRIV_BASE | (SAM_VDP         << 14))
#define UP_VIDEO_OUT_BASE   (UNBUF_PRIV_BASE | (SAM_VIDEO_OUT   << 14))
#define UP_FTF_BASE	    (UNBUF_PRIV_BASE | (SAM_FTF         << 14))
#define UP_TM_BASE	    (UNBUF_PRIV_BASE | (SAM_TM          << 14))
#define UP_RASTER_BASE	    (UNBUF_PRIV_BASE | (SAM_RASTER      << 14))
#define UP_FBC_BASE	    (UNBUF_PRIV_BASE | (SAM_FBC         << 14))
#define UP_IMAGE_BASE	    (UNBUF_PRIV_BASE | (SAM_IMAGE       << 14))
#define UP_VOLUME_BASE	    (UNBUF_PRIV_BASE | (SAM_VOLUME      << 14))
#define UP_RETARG_BASE	    (UNBUF_PRIV_BASE | (SAM_RETARG      << 14))
#define UP_SHARED_BASE	    (UNBUF_PRIV_BASE | (SAM_SHARED      << 14))
#define UP_HOSTIF_BASE	    (UNBUF_PRIV_BASE | (SAM_HOSTIF      << 14))
#define UP_BINC_BASE	    (UNBUF_PRIV_BASE | (SAM_BINC        << 14))
#define UP_DMA_BASE	    (UNBUF_PRIV_BASE | (SAM_DMA         << 14))
#define UP_OFU_BASE  	    (UNBUF_PRIV_BASE | (SAM_OFU	        << 14))
#define UP_OGL_ST_BASE      (UNBUF_PRIV_BASE | (SAM_OGL_ST	<< 14))
#define UP_MFU_BASE	    (UNBUF_PRIV_BASE | (SAM_MFU         << 14))
#define UP_MFU_REMAP_BASE   (UNBUF_PRIV_BASE | (SAM_MFU_REMAP   << 14))

/* buffered privileged base addresses */
#define BP_VDP_BASE	    (BUF_PRIV_BASE | (SAM_VDP         << 14))
#define BP_VIDEO_OUT_BASE   (BUF_PRIV_BASE | (SAM_VIDEO_OUT   << 14))
#define BP_FTF_BASE	    (BUF_PRIV_BASE | (SAM_FTF         << 14))
#define BP_TM_BASE	    (BUF_PRIV_BASE | (SAM_TM          << 14))
#define BP_RASTER_BASE	    (BUF_PRIV_BASE | (SAM_RASTER      << 14))
#define BP_FBC_BASE	    (BUF_PRIV_BASE | (SAM_FBC         << 14))
#define BP_IMAGE_BASE	    (BUF_PRIV_BASE | (SAM_IMAGE       << 14))
#define BP_VOLUME_BASE	    (BUF_PRIV_BASE | (SAM_VOLUME      << 14))
#define BP_RETARG_BASE	    (BUF_PRIV_BASE | (SAM_RETARG      << 14))
#define BP_SHARED_BASE	    (BUF_PRIV_BASE | (SAM_SHARED      << 14))
#define BP_HOSTIF_BASE	    (BUF_PRIV_BASE | (SAM_HOSTIF      << 14))
#define BP_BINC_BASE	    (BUF_PRIV_BASE | (SAM_BINC        << 14))
#define BP_DMA_BASE	    (BUF_PRIV_BASE | (SAM_DMA         << 14))
#define BP_OFU_BASE  	    (BUF_PRIV_BASE | (SAM_OFU         << 14))
#define BP_OGL_ST_BASE      (BUF_PRIV_BASE | (SAM_OGL_ST      << 14))
#define BP_MFU_BASE	    (BUF_PRIV_BASE | (SAM_MFU         << 14))
#define BP_MFU_REMAP_BASE   (BUF_PRIV_BASE | (SAM_MFU_REMAP   << 14))

/* unbuffered base addresses */
#define UB_VDP_BASE	    (UNBUF_CTL_BASE | (SAM_VDP         << 17))
#define UB_VIDEO_OUT_BASE   (UNBUF_CTL_BASE | (SAM_VIDEO_OUT   << 17))
#define UB_TM_BASE	    (UNBUF_CTL_BASE | (SAM_TM          << 17))
#define UB_RASTER_BASE	    (UNBUF_CTL_BASE | (SAM_RASTER      << 17))
#define UB_FBC_BASE	    (UNBUF_CTL_BASE | (SAM_FBC         << 17))
#define UB_IMAGE_BASE	    (UNBUF_CTL_BASE | (SAM_IMAGE       << 17))
#define UB_VOLUME_BASE	    (UNBUF_CTL_BASE | (SAM_VOLUME      << 17))
#define UB_RETARG_BASE	    (UNBUF_CTL_BASE | (SAM_RETARG      << 17))
#define UB_SHARED_BASE	    (UNBUF_CTL_BASE | (SAM_SHARED      << 17))
#define UB_HOSTIF_BASE	    (UNBUF_CTL_BASE | (SAM_HOSTIF      << 17))
#define UB_BINC_BASE	    (UNBUF_CTL_BASE | (SAM_BINC        << 17))
#define UB_DMA_BASE	    (UNBUF_CTL_BASE | (SAM_DMA         << 17))
#define UB_OGL_ST_BASE 	    (UNBUF_CTL_BASE | (SAM_OGL_ST      << 17))

/* 2D buffered base addresses */
#define B2_VDP_BASE	    (BUF2D_CTL_BASE | (SAM_VDP         << 17))
#define B2_VIDEO_OUT_BASE   (BUF2D_CTL_BASE | (SAM_VIDEO_OUT   << 17))
#define B2_FTF_BASE	    (BUF2D_CTL_BASE | (SAM_FTF         << 17))
#define B2_TM_BASE	    (BUF2D_CTL_BASE | (SAM_TM          << 17))
#define B2_RASTER_BASE	    (BUF2D_CTL_BASE | (SAM_RASTER      << 17))
#define B2_FBC_BASE	    (BUF2D_CTL_BASE | (SAM_FBC         << 17))
#define B2_IMAGE_BASE	    (BUF2D_CTL_BASE | (SAM_IMAGE       << 17))
#define B2_VOLUME_BASE	    (BUF2D_CTL_BASE | (SAM_VOLUME      << 17))
#define B2_RETARG_BASE	    (BUF2D_CTL_BASE | (SAM_RETARG      << 17))
#define B2_SHARED_BASE	    (BUF2D_CTL_BASE | (SAM_SHARED      << 17))
#define B2_HOSTIF_BASE	    (BUF2D_CTL_BASE | (SAM_HOSTIF      << 17))
#define B2_BINC_BASE	    (BUF2D_CTL_BASE | (SAM_BINC        << 17))
#define B2_DMA_BASE	    (BUF2D_CTL_BASE | (SAM_DMA         << 17))
#define B2_OFU_BASE  	    (BUF2D_CTL_BASE | (SAM_OFU         << 17))
#define B2_OGL_ST_BASE      (BUF2D_CTL_BASE | (SAM_OGL_ST      << 17))
#define B2_MFU_BASE	    (BUF2D_CTL_BASE | (SAM_MFU         << 17))
#define B2_MFU_REMAP_BASE   (BUF2D_CTL_BASE | (SAM_MFU_REMAP   << 17))

/* 3D buffered base addresses */
#define B3_GA_A_BASE	    (BUF3D_CTL_BASE | (SAM_GA_A        << 17))
#define B3_GA_B_BASE	    (BUF3D_CTL_BASE | (SAM_GA_B        << 17))
#define B3_FTF_BASE	    (BUF3D_CTL_BASE | (SAM_FTF         << 17))
#define B3_TM_BASE	    (BUF3D_CTL_BASE | (SAM_TM          << 17))
#define B3_RASTER_BASE	    (BUF3D_CTL_BASE | (SAM_RASTER      << 17))
#define B3_IMAGE_BASE	    (BUF3D_CTL_BASE | (SAM_IMAGE       << 17))
#define B3_VOLUME_BASE	    (BUF3D_CTL_BASE | (SAM_VOLUME      << 17))
#define B3_RETARG_BASE	    (BUF3D_CTL_BASE | (SAM_RETARG      << 17))
#define B3_SHARED_BASE	    (BUF3D_CTL_BASE | (SAM_SHARED      << 17))
#define B3_HOSTIF_BASE	    (BUF3D_CTL_BASE | (SAM_HOSTIF      << 17))
#define B3_DMA_BASE	    (BUF3D_CTL_BASE | (SAM_DMA         << 17))
#define B3_OGL_ST_BASE 	    (BUF3D_CTL_BASE | (SAM_OGL_ST      << 17))

/* --------------------------------------------------------
    Some Defines for the Geometry Accelerator Packet Counts
   -------------------------------------------------------- */
#define     B3_GA_COUNT_16            (0x00003c00 << 2)
#define     B3_GA_COUNT_6             (0x00001400 << 2)


/* START OF AUTO GENERATED CODE */
/*  Start of Video Display Processor Registers  */
/*  End of  Video Display Processor Registers  */

/*  Start of Video Display Processor Registers  */
/*  End of Video Display Processor Registers  */

/*  Start Video Display Processor Registers  */
#define UB_START_OF_VIDBUS                0xFFFFFFFF /*  Bogus Address Used For .h File Auto Generation */
#define UB_CP                             0x00400000 /* Cursor Position Register (W) */
#define UB_CA                             0x00400004 /* Cursor Address Register (W) */
#define UB_CD                             0x00400008 /* Cursor Data Register (W) */
#define UB_CF                             0x0040000C /* Cursor Foreground Register (W) */
#define UB_CB                             0x00400010 /* Cursor Background Register (W) */
/*  End of  Video Display Processor Registers  */

/*  Start of Video Display Processor Registers  */
#define B2_CP                             0x00800000 /* Cursor Position Register (RW) */
#define B2_CA                             0x00800004 /* Cursor Address Register (RW) */
#define B2_CD                             0x00800008 /* Cursor Data Register (RW) */
#define B2_CF                             0x0080000C /* Cursor Foreground Register (RW) */
#define B2_CB                             0x00800010 /* Cursor Background Register (RW) */
#define B2_PMASK                          0x00800018 /* LUT Plane Mask Register (RW) */
#define B2_MCS                            0x0080001C /* Midlay Color Map Select Register (RW) */
#define B2_LLCA                           0x00800020 /* LUT Load Control Address Register (RW) */
#define B2_LUTD                           0x00800024 /* Look Up Table Register (RW) */
#define B2_FATTR                          0x0080003C /* Force Attribute Bits Register (RW) */
#define B2_MV                             0x00800040 /* Misc. Video Register (RW) */
#define B2_CFG                            0x00800044 /* Configuration Register (RW) */
#define B2_SCR                            0x00800048 /* Stereo Control Register (RW) */
#define B2_MPC                            0x0080004C /* Monitor Power Control Register (RW) */
#define B2_ETG                            0x00800058 /* Early Timing Generator Register (RW) */
#define B2_VHAL                           0x0080005C /* Vertical / Horizontal Active Length Register (RW) */
#define B2_HTG                            0x00800060 /* Horizontal Timing Generator Register (RW) */
#define B2_STG                            0x00800064 /* Serration Timing Generator Register (RW) */
#define B2_VTG                            0x00800068 /* Vertical Timing Generator Register (RW) */
#define B2_HS                             0x0080006C /* Horizontal State Register (RW) */
#define B2_VS                             0x00800070 /* Vertical State Register (RW) */
#define B2_DTS                            0x00800074 /* Timing & State Register (R) */
#define B2_SRBMAP0                        0x00800078 /* Stereo Right Front Block BMAP Register (RW) */
#define B2_SRBMAP1                        0x0080007C /* Stereo Right Back Block BMAP Register (RW) */
#define B2_VOP                            0x00800080 /* Video Out Position Register (RW) */
#define B2_VOS                            0x00800084 /* Video Out Size Register (RW) */
#define B2_VOC                            0x00800088 /* Video Out Control Register (RW) */
#define B2_VOP1                           0x0080008C /* Video Out Position Register (RW) */
#define B2_STSA                           0x00800090 /* Showtime CRC Register (RW) */
#define B2_BLSA                           0x00800094 /* Blither CRC Register (RW) */
#define B2_PLL_DOT_CTL                    0x008000A0 /* Dot Clock PLL Control Register (RW) */
#define B2_PLL_DOT_CNT                    0x008000A4 /* Dot Clock PLL Counter Register (RW) */
#define B2_PLL_CORE_CTL                   0x008000A8 /* Core Clock PLL Control Register (RW) */
#define B2_PLL_CORE_CNT                   0x008000AC /* Core Clock PLL Counter Register (RW) */
#define B2_PLL_RAM_CTL                    0x008000B0 /* RAM Clock PLL Control Register (RW) */
#define B2_PLL_RAM_CNT                    0x008000B4 /* RAM Clock PLL Counter Register (RW) */
#define B2_PLL_REF_CNT                    0x008000B8 /* PLL Reference Clock Counter Register (RW) */
#define B2_BLSA_MASK                      0x008000C4 /* BL Signature Analysis Mask Register (RW) */
#define B2_SA_MASK                        0x008000C8 /* Signature Analysis Mask Register (RW) */
#define B2_SA_SIG                         0x008000CC /* Signature Analysis Register (RW) */
#define B2_MON                            0x008000D0 /* Monitor Test Register (RW) */
#define B2_DDC                            0x008000D4 /* Display Data Channel Register (RW) */
#define B2_CFS0                           0x00800100 /* Colormap Function Select Register 0 (RW) */
#define B2_CFS1                           0x00800104 /* Colormap Function Select Register 1 (RW) */
#define B2_CFS2                           0x00800108 /* Colormap Function Select Register 2 (RW) */
#define B2_CFS3                           0x0080010C /* Colormap Function Select Register 3 (RW) */
#define B2_CFS4                           0x00800110 /* Colormap Function Select Register 4 (RW) */
#define B2_CFS5                           0x00800114 /* Colormap Function Select Register 5 (RW) */
#define B2_CFS6                           0x00800118 /* Colormap Function Select Register 6 (RW) */
#define B2_CFS7                           0x0080011C /* Colormap Function Select Register 7 (RW) */
#define B2_CFS8                           0x00800120 /* Colormap Function Select Register 8 (RW) */
#define B2_CFS9                           0x00800124 /* Colormap Function Select Register 9 (RW) */
#define B2_CFS10                          0x00800128 /* Colormap Function Select Register 10 (RW) */
#define B2_CFS11                          0x0080012C /* Colormap Function Select Register 11 (RW) */
#define B2_CFS12                          0x00800130 /* Colormap Function Select Register 12 (RW) */
#define B2_CFS13                          0x00800134 /* Colormap Function Select Register 13 (RW) */
#define B2_CFS14                          0x00800138 /* Colormap Function Select Register 14 (RW) */
#define B2_CFS15                          0x0080013C /* Colormap Function Select Register 15 (RW) */
#define B2_CFS16                          0x00800140 /* Colormap Function Select Register 16 (RW) */
#define B2_CFS17                          0x00800144 /* Colormap Function Select Register 17 (RW) */
#define B2_CFS18                          0x00800148 /* Colormap Function Select Register 18 (RW) */
#define B2_CFS19                          0x0080014C /* Colormap Function Select Register 19 (RW) */
#define B2_CFS20                          0x00800150 /* Colormap Function Select Register 20 (RW) */
#define B2_CFS21                          0x00800154 /* Colormap Function Select Register 21 (RW) */
#define B2_CFS22                          0x00800158 /* Colormap Function Select Register 22 (RW) */
#define B2_CFS23                          0x0080015C /* Colormap Function Select Register 23 (RW) */
#define B2_CFS24                          0x00800160 /* Colormap Function Select Register 24 (RW) */
#define B2_CFS25                          0x00800164 /* Colormap Function Select Register 25 (RW) */
#define B2_CFS26                          0x00800168 /* Colormap Function Select Register 26 (RW) */
#define B2_CFS27                          0x0080016C /* Colormap Function Select Register 27 (RW) */
#define B2_CFS28                          0x00800170 /* Colormap Function Select Register 28 (RW) */
#define B2_CFS29                          0x00800174 /* Colormap Function Select Register 29 (RW) */
#define B2_CFS30                          0x00800178 /* Colormap Function Select Register 30 (RW) */
#define B2_CFS31                          0x0080017C /* Colormap Function Select Register 31 (RW) */
#define B2_VBC_SIG_EN                     0x008001C0 /* Vid Bus Signature Enable Register ? (RW) */
#define B2_VBC_SIG                        0x008001C4 /* Vid Bus Signature Register? (RW) */
#define B2_DUM                            0x00800240 /* Display Unit Master Register (RW) */
#define B2_ABMAP                          0x00800244 /* Attribute Block BMAP Register (RW) */
#define B2_CKEY_HI                        0x00800248 /* Color Key High Register (RW) */
#define B2_CKEY_LO                        0x0080024C /* Color Key Low Register (RW) */
#define B2_CALPHA                         0x00800250 /* Constant Alpha Register (RW) */
#define B2_CCBC                           0x00800254 /* Color Conversion Brightness & Contrast Register (RW) */
#define B2_OMC                            0x00800260 /* Overlay Miscellaneous Control Register (RW) */
#define B2_OTLS                           0x00800264 /* Overlay Transparency LUT Select Register (RW) */
#define B2_OXYO                           0x00800268 /* Overlay Visible XY Offset Register (RW) */
#define B2_OBS                            0x0080026C /* Overlay Buffer Select Register (RW) */
#define B2_OBMAP0                         0x00800270 /* Overlay Primary Buffer Block BMAP Register (RW) */
#define B2_OBMAP1                         0x00800274 /* Overlay Secondary Buffer Block BMAP Register (RW) */
#define B2_OUL                            0x00800278 /* Overlay Upper Left Register (RW) */
#define B2_OLR                            0x0080027C /* Overlay  Lower Right Register (RW) */
#define B2_ODS                            0x00800280 /* Overlay  Data Size Register (RW) */
#define B2_OSCALE                         0x00800284 /* Overlay Scale Register (RW) */
#define B2_IMD                            0x00800288 /* Image Depth Register (RW) */
#define B2_IMC0                           0x00800300 /* Image Miscellaneous Control 0 Register (RW) */
#define B2_IMC1                           0x00800304 /* Image Miscellaneous Control 1 Register (RW) */
#define B2_IMC2                           0x00800308 /* Image Miscellaneous Control 2 Register (RW) */
#define B2_IMC3                           0x0080030C /* Image Miscellaneous Control 3 Register (RW) */
#define B2_IMC4                           0x00800310 /* Image Miscellaneous Control 4 Register (RW) */
#define B2_IMC5                           0x00800314 /* Image Miscellaneous Control 5 Register (RW) */
#define B2_IMC6                           0x00800318 /* Image Miscellaneous Control 6 Register (RW) */
#define B2_IMC7                           0x0080031C /* Image Miscellaneous Control 7 Register (RW) */
#define B2_IBMAP0                         0x00800320 /* Image Primary BMAP Register (RW) */
#define B2_IBMAP1                         0x00800324 /* Image Secondary BMAP Register (RW) */
#define B2_WAIT_HSYNC                     0x00800330 /* Wait for Horizontal retrace sync Register (RW) */
#define B2_IBS0                           0x00800340 /* Image Buffer Select 0 Register (RW) */
#define B2_IBS1                           0x00800344 /* Image Buffer Select 1 Register (RW) */
#define B2_IBS2                           0x00800348 /* Image Buffer Select 2 Register (RW) */
#define B2_IBS3                           0x0080034C /* Image Buffer Select 3 Register (RW) */
#define B2_IBS4                           0x00800350 /* Image Buffer Select 4 Register (RW) */
#define B2_IBS5                           0x00800354 /* Image Buffer Select 5 Register (RW) */
#define B2_IBS6                           0x00800358 /* Image Buffer Select 6 Register (RW) */
#define B2_IBS7                           0x0080035C /* Image Buffer Select 7 Register (RW) */
#define B2_ICLR0                          0x00800360 /* Image Clear 0 Register (RW) */
#define B2_ICLR1                          0x00800364 /* Image Clear 1 Register (RW) */
#define B2_ICLR2                          0x00800368 /* Image Clear 2 Register (RW) */
#define B2_ICLR3                          0x0080036C /* Image Clear 3 Register (RW) */
#define B2_ICLR4                          0x00800370 /* Image Clear 4 Register (RW) */
#define B2_ICLR5                          0x00800374 /* Image Clear 5 Register (RW) */
#define B2_ICLR6                          0x00800378 /* Image Clear 6 Register (RW) */
#define B2_ICLR7                          0x0080037C /* Image Clear 7 Register (RW) */
/*  End of Video Display Processor Registers  */

/*  Start of Reserved Address Space  */
/*  End of  Reserved Address Space  */

/*  Start of Video Out Registers  */
/*  End of  Video Out Registers  */

/*  Start of Video Out Registers  */
/*  End of Video Out Registers  */

/*  Start of Video Out Registers  */
/*  End of  Video Out Registers  */

/*  Start of  Video Out Registers  */
/*  Start Of Reserved For Digital Video Out Registers  */
#define B2_EN_AR_L                        0x00820200 /* Encoder Address Register for LUT  (RW) */
#define B2_EN_DR_L                        0x00820204 /* Encoder Data Register for LUT  (RW) */
#define B2_EN_AR_C                        0x00820208 /* Encoder Address Register for Control Stack (RW) */
#define B2_EN_DR_C                        0x0082020C /* Encoder Data Register for Control Stack (RW) */
#define B2_CCC1                           0x00820210 /* Colorspace Conversion Coefficient Byte 1 (RW) */
#define B2_CCC2                           0x00820214 /* Colorspace Conversion Coefficient Byte 2 (RW) */
#define B2_CCC3                           0x00820218 /* Colorspace Conversion Coefficient Byte 3 (RW) */
#define B2_CCC4                           0x0082021C /* Colorspace Conversion Coefficient Byte 4 (RW) */
#define B2_R_R                            0x00820220 /* Readback Red Register (RW) */
#define B2_R_G                            0x00820224 /* Readback Green Register (RW) */
#define B2_R_B                            0x00820228 /* Readback Blue Register (RW) */
#define B2_R_ID                           0x0082022C /* Readback Board ID Register (RW) */
#define B2_S_CTRL                         0x00820230 /* Video Out Status/Control Register (RW) */
#define B2_END_OF_VIDBUS                  0xFFFFFFFE /*  Bogus Address Used For .h File Auto Generation */
/*  End Of Reserved For Digital Video Out Registers  */

/*  End of  Video Out Registers  */

/*  Start of Reserved Address Space  */
/*  End of  Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Geometry Accelerator Registers A  */
#define B3_GA_MODELVIEW_A1                0x00C802C0 /* Model / View  Matrix Element A1 (RW) */
#define B3_GA_MODELVIEW_A2                0x00C802C4 /* Model / View  Matrix Element A2 (RW) */
#define B3_GA_MODELVIEW_A3                0x00C802C8 /* Model / View  Matrix Element A3 (RW) */
#define B3_GA_MODELVIEW_A4                0x00C802CC /* Model / View  Matrix Element A4 (RW) */
#define B3_GA_MODELVIEW_B1                0x00C802D0 /* Model / View  Matrix Element B1 (RW) */
#define B3_GA_MODELVIEW_B2                0x00C802D4 /* Model / View  Matrix Element B2 (RW) */
#define B3_GA_MODELVIEW_B3                0x00C802D8 /* Model / View  Matrix Element B3 (RW) */
#define B3_GA_MODELVIEW_B4                0x00C802DC /* Model / View  Matrix Element B4 (RW) */
#define B3_GA_MODELVIEW_C1                0x00C802E0 /* Model / View  Matrix Element C1 (RW) */
#define B3_GA_MODELVIEW_C2                0x00C802E4 /* Model / View  Matrix Element C2 (RW) */
#define B3_GA_MODELVIEW_C3                0x00C802E8 /* Model / View  Matrix Element C3 (RW) */
#define B3_GA_MODELVIEW_C4                0x00C802EC /* Model / View  Matrix Element C4 (RW) */
#define B3_GA_MODELVIEW_D1                0x00C802F0 /* Model / View  Matrix Element D1 (RW) */
#define B3_GA_MODELVIEW_D2                0x00C802F4 /* Model / View  Matrix Element D2 (RW) */
#define B3_GA_MODELVIEW_D3                0x00C802F8 /* Model / View  Matrix Element D3 (RW) */
#define B3_GA_MODELVIEW_D4                0x00C802FC /* Model / View  Matrix Element D4 (RW) */
#define B3_GA_TEXTURE_MATRIX_A1           0x00C80300 /* Texture Matrix Element A1 (RW) */
#define B3_GA_TEXTURE_MATRIX_A2           0x00C80304 /* Texture Matrix Element A2 (RW) */
#define B3_GA_TEXTURE_MATRIX_A3           0x00C80308 /* Texture Matrix Element A3 (RW) */
#define B3_GA_TEXTURE_MATRIX_A4           0x00C8030C /* Texture Matrix Element A4 (RW) */
#define B3_GA_TEXTURE_MATRIX_B1           0x00C80310 /* Texture Matrix Element B1 (RW) */
#define B3_GA_TEXTURE_MATRIX_B2           0x00C80314 /* Texture Matrix Element B2 (RW) */
#define B3_GA_TEXTURE_MATRIX_B3           0x00C80318 /* Texture Matrix Element B3 (RW) */
#define B3_GA_TEXTURE_MATRIX_B4           0x00C8031C /* Texture Matrix Element B4 (RW) */
#define B3_GA_TEXTURE_MATRIX_C1           0x00C80320 /* Texture Matrix Element C1 (RW) */
#define B3_GA_TEXTURE_MATRIX_C2           0x00C80324 /* Texture Matrix Element C2 (RW) */
#define B3_GA_TEXTURE_MATRIX_C3           0x00C80328 /* Texture Matrix Element C3 (RW) */
#define B3_GA_TEXTURE_MATRIX_C4           0x00C8032C /* Texture Matrix Element C4 (RW) */
#define B3_GA_TEXTURE_MATRIX_D1           0x00C80330 /* Texture Matrix Element D1 (RW) */
#define B3_GA_TEXTURE_MATRIX_D2           0x00C80334 /* Texture Matrix Element D2 (RW) */
#define B3_GA_TEXTURE_MATRIX_D3           0x00C80338 /* Texture Matrix Element D3 (RW) */
#define B3_GA_TEXTURE_MATRIX_D4           0x00C8033C /* Texture Matrix Element D4 (RW) */
#define B3_GA_XFORM_A1                    0x00C80340 /* Transformation Matrix Element A1 (RW) */
#define B3_GA_XFORM_A2                    0x00C80344 /* Transformation Matrix Element A2 (RW) */
#define B3_GA_XFORM_A3                    0x00C80348 /* Transformation Matrix Element A3 (RW) */
#define B3_GA_XFORM_A4                    0x00C8034C /* Transformation Matrix Element A4 (RW) */
#define B3_GA_XFORM_B1                    0x00C80350 /* Transformation Matrix Element B1 (RW) */
#define B3_GA_XFORM_B2                    0x00C80354 /* Transformation Matrix Element B2 (RW) */
#define B3_GA_XFORM_B3                    0x00C80358 /* Transformation Matrix Element B3 (RW) */
#define B3_GA_XFORM_B4                    0x00C8035C /* Transformation Matrix Element B4 (RW) */
#define B3_GA_XFORM_C1                    0x00C80360 /* Transformation Matrix Element C1 (RW) */
#define B3_GA_XFORM_C2                    0x00C80364 /* Transformation Matrix Element C2 (RW) */
#define B3_GA_XFORM_C3                    0x00C80368 /* Transformation Matrix Element C3 (RW) */
#define B3_GA_XFORM_C4                    0x00C8036C /* Transformation Matrix Element C4 (RW) */
#define B3_GA_XFORM_D1                    0x00C80370 /* Transformation Matrix Element D1 (RW) */
#define B3_GA_XFORM_D2                    0x00C80374 /* Transformation Matrix Element D2 (RW) */
#define B3_GA_XFORM_D3                    0x00C80378 /* Transformation Matrix Element D3 (RW) */
#define B3_GA_XFORM_D4                    0x00C8037C /* Transformation Matrix Element D4 (RW) */
#define B3_GA_ANISO_XFORM_A1              0x00C80380 /* Aniso Transformation  Element A1 (RW) */
#define B3_GA_ANISO_XFORM_A2              0x00C80384 /* Aniso Transformation Element A2 (RW) */
#define B3_GA_ANISO_XFORM_A3              0x00C80388 /* Aniso Transformation Element A3 (RW) */
#define B3_GA_ANISO_XFORM_B1              0x00C8038C /* Aniso Transformation  Element B1 (RW) */
#define B3_GA_ANISO_XFORM_B2              0x00C80390 /* Aniso Transformation Element B2 (RW) */
#define B3_GA_ANISO_XFORM_B3              0x00C80394 /* Aniso Transformation Element B3 (RW) */
#define B3_GA_ANISO_XFORM_C1              0x00C80398 /* Aniso Transformation  Element C1 (RW) */
#define B3_GA_ANISO_XFORM_C2              0x00C8039C /* Aniso Transformation Element C2 (RW) */
#define B3_GA_ANISO_XFORM_C3              0x00C803A0 /* Aniso Transformation Element C3 (RW) */
#define B3_GA_FREQ_BIAS_S                 0x00C803A8 /* Texture MIPMAP Frequency  Bias S (RW) */
#define B3_GA_FREQ_BIAS_T                 0x00C803AC /* Texture MIPMAP Frequency  Bias T (RW) */
#define B3_GA_FREQ_BIAS_R                 0x00C803B0 /* Texture MIPMAP Frequency  Bias R (RW) */
#define B3_GA_S_SCALE                     0x00C803B4 /* Texture Size Scale Factor S (RW) */
#define B3_GA_T_SCALE                     0x00C803B8 /* Texture Size Scale Factor T (RW) */
#define B3_GA_R_SCALE                     0x00C803BC /* Texture Size Scale Factor R (RW) */
#define B3_GA_CLIP_ZMIN                   0x00C803C0 /* Clip ZMIN Value (RW) */
#define B3_GA_CLIP_ZMAX                   0x00C803C4 /* Clip ZMAX Value (RW) */
#define B3_GA_CLIP_XMIN                   0x00C803C8 /* Clip YMIN Value (RW) */
#define B3_GA_CLIP_XMAX                   0x00C803CC /* Clip YMAX Value (RW) */
#define B3_GA_CLIP_YMIN                   0x00C803D0 /* Clip XMIN Value (RW) */
#define B3_GA_CLIP_YMAX                   0x00C803D4 /* Clip XMAX Value (RW) */
#define B3_GA_DC_SEP_SPEC_RED             0x00C80420 /* DC Primitive Separate Specular Red Value (RW) */
#define B3_GA_DC_SEP_SPEC_GREEN           0x00C80424 /* DC Primitive Separate Specular Green Value (RW) */
#define B3_GA_DC_SEP_SPEC_BLUE            0x00C80428 /* DC Primitive Separate Specular Blue Value (RW) */
#define B3_GA_DC_FOG                      0x00C8042C /* DC Primitive Fog Value (RW) */
#define B3_GA_V1_DC_SEP_SPEC_RED          0x00C80460 /* DC Primitive Separate Specular Red Value, Vertex 1 (RW) */
#define B3_GA_V1_DC_SEP_SPEC_GREEN        0x00C80464 /* DC Primitive Separate Specular Green Value, Vertex 1 (RW) */
#define B3_GA_V1_DC_SEP_SPEC_BLUE         0x00C80468 /* DC Primitive Separate Specular Blue Value, Vertex 1 (RW) */
#define B3_GA_V1_DC_FOG                   0x00C8046C /* DC Primitive Fog Value, Vertex 1 (RW) */
#define B3_GA_V2_DC_SEP_SPEC_RED          0x00C804A0 /* DC Primitive Separate Specular Red Value, Vertex 2 (RW) */
#define B3_GA_V2_DC_SEP_SPEC_GREEN        0x00C804A4 /* DC Primitive Separate Specular Green Value, Vertex 2 (RW) */
#define B3_GA_V2_DC_SEP_SPEC_BLUE         0x00C804A8 /* DC Primitive Separate Specular Blue Value, Vertex 2 (RW) */
#define B3_GA_V2_DC_FOG                   0x00C804AC /* DC Primitive Fog Value, Vertex 2 (RW) */
#define B3_GA_V3_DC_SEP_SPEC_RED          0x00C804E0 /* DC Primitive Separate Specular Red Value, Vertex 3 (RW) */
#define B3_GA_V3_DC_SEP_SPEC_GREEN        0x00C804E4 /* DC Primitive Separate Specular Green Value, Vertex 3 (RW) */
#define B3_GA_V3_DC_SEP_SPEC_BLUE         0x00C804E8 /* DC Primitive Separate Specular Blue Value, Vertex 3 (RW) */
#define B3_GA_V3_DC_FOG                   0x00C804EC /* DC Primitive Fog Value, Vertex 3 (RW) */
#define B3_GA_AA_FLAG_ADDRESS             0x00C80774 /* AA Flag Address Register (RW) */
#define B3_GA_AA_FLAG_DATA                0x00C80778 /* AA Flag Data Register (RW) */
#define B3_GA_NORMAL_SCALE                0x00C8077C /* Scale Factor For GL_RESCALE_NORMALS Calculations (RW) */
#define B3_GA_TEXGEN_MATRIX_A1            0x00C807C0 /* Texture Generation matrix Element A1 (RW) */
#define B3_GA_TEXGEN_MATRIX_A2            0x00C807C4 /* Texture Generation matrix Element A2 (RW) */
#define B3_GA_TEXGEN_MATRIX_A3            0x00C807C8 /* Texture Generation matrix Element A3 (RW) */
#define B3_GA_TEXGEN_MATRIX_A4            0x00C807CC /* Texture Generation matrix Element A4 (RW) */
#define B3_GA_TEXGEN_MATRIX_B1            0x00C807D0 /* Texture Generation matrix Element B1 (RW) */
#define B3_GA_TEXGEN_MATRIX_B2            0x00C807D4 /* Texture Generation matrix Element B2 (RW) */
#define B3_GA_TEXGEN_MATRIX_B3            0x00C807D8 /* Texture Generation matrix Element B3 (RW) */
#define B3_GA_TEXGEN_MATRIX_B4            0x00C807DC /* Texture Generation matrix Element B4 (RW) */
#define B3_GA_TEXGEN_MATRIX_C1            0x00C807E0 /* Texture Generation matrix Element C1 (RW) */
#define B3_GA_TEXGEN_MATRIX_C2            0x00C807E4 /* Texture Generation matrix Element C2 (RW) */
#define B3_GA_TEXGEN_MATRIX_C3            0x00C807E8 /* Texture Generation matrix Element C3 (RW) */
#define B3_GA_TEXGEN_MATRIX_C4            0x00C807EC /* Texture Generation matrix Element C4 (RW) */
#define B3_GA_TEXGEN_MATRIX_D1            0x00C807F0 /* Texture Generation matrix Element D1 (RW) */
#define B3_GA_TEXGEN_MATRIX_D2            0x00C807F4 /* Texture Generation matrix Element D2 (RW) */
#define B3_GA_TEXGEN_MATRIX_D3            0x00C807F8 /* Texture Generation matrix Element D3 (RW) */
#define B3_GA_TEXGEN_MATRIX_D4            0x00C807FC /* Texture Generation matrix Element D4 (RW) */
#define B3_GA_FRAC2_0                     0x00C809A0 /* Entry 0 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_1                     0x00C809A4 /* Entry 1 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_2                     0x00C809A8 /* Entry 2 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_3                     0x00C809AC /* Entry 3 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_4                     0x00C809B0 /* Entry 4 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_5                     0x00C809B4 /* Entry 5 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_6                     0x00C809B8 /* Entry 6 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_7                     0x00C809BC /* Entry 7 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_8                     0x00C809C0 /* Entry 8 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_9                     0x00C809C4 /* Entry 9 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_10                    0x00C809C8 /* Entry 10 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_11                    0x00C809CC /* Entry 11 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_12                    0x00C809D0 /* Entry 12 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_13                    0x00C809D4 /* Entry 13 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_14                    0x00C809D8 /* Entry 14 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_15                    0x00C809DC /* Entry 15 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_FRAC2_16                    0x00C809E0 /* Entry 16 Of Lookup Table For 2^FRAC (RW) */
#define B3_GA_MODELCLIP_PLANE1_A          0x00C80B90 /* Model Clipping Plane 1 A Element (RW) */
#define B3_GA_MODELCLIP_PLANE1_B          0x00C80B94 /* Model Clipping Plane 1 B Element (RW) */
#define B3_GA_MODELCLIP_PLANE1_C          0x00C80B98 /* Model Clipping Plane 1 C Element (RW) */
#define B3_GA_MODELCLIP_PLANE1_D          0x00C80B9C /* Model Clipping Plane 1 D Element (RW) */
#define B3_GA_MODELCLIP_PLANE2_A          0x00C80BA0 /* Model Clipping Plane 2 A Element (RW) */
#define B3_GA_MODELCLIP_PLANE2_B          0x00C80BA4 /* Model Clipping Plane 2 B Element (RW) */
#define B3_GA_MODELCLIP_PLANE2_C          0x00C80BA8 /* Model Clipping Plane 2 C Element (RW) */
#define B3_GA_MODELCLIP_PLANE2_D          0x00C80BAC /* Model Clipping Plane 2 D Element (RW) */
#define B3_GA_MODELCLIP_PLANE3_A          0x00C80BB0 /* Model Clipping Plane 3 A Element (RW) */
#define B3_GA_MODELCLIP_PLANE3_B          0x00C80BB4 /* Model Clipping Plane 3 B Element (RW) */
#define B3_GA_MODELCLIP_PLANE3_C          0x00C80BB8 /* Model Clipping Plane 3 C Element (RW) */
#define B3_GA_MODELCLIP_PLANE3_D          0x00C80BBC /* Model Clipping Plane 3 D Element (RW) */
#define B3_GA_MODELCLIP_PLANE4_A          0x00C80BC0 /* Model Clipping Plane 4 A Element (RW) */
#define B3_GA_MODELCLIP_PLANE4_B          0x00C80BC4 /* Model Clipping Plane 4 B Element (RW) */
#define B3_GA_MODELCLIP_PLANE4_C          0x00C80BC8 /* Model Clipping Plane 4 C Element (RW) */
#define B3_GA_MODELCLIP_PLANE4_D          0x00C80BCC /* Model Clipping Plane 4 D Element (RW) */
#define B3_GA_MODELCLIP_PLANE5_A          0x00C80BD0 /* Model Clipping Plane 5 A Element (RW) */
#define B3_GA_MODELCLIP_PLANE5_B          0x00C80BD4 /* Model Clipping Plane 5 B Element (RW) */
#define B3_GA_MODELCLIP_PLANE5_C          0x00C80BD8 /* Model Clipping Plane 5 C Element (RW) */
#define B3_GA_MODELCLIP_PLANE5_D          0x00C80BDC /* Model Clipping Plane 5 D Element (RW) */
#define B3_GA_MODELCLIP_PLANE6_A          0x00C80BE0 /* Model Clipping Plane 6 A Element (RW) */
#define B3_GA_MODELCLIP_PLANE6_B          0x00C80BE4 /* Model Clipping Plane 6 B Element (RW) */
#define B3_GA_MODELCLIP_PLANE6_C          0x00C80BE8 /* Model Clipping Plane 6 C Element (RW) */
#define B3_GA_MODELCLIP_PLANE6_D          0x00C80BEC /* Model Clipping Plane 6 D Element (RW) */
#define B3_GA_V0_MAT_FRONT_AMBIENT_RED    0x00C80C00 /* Vertex 0 Material Front Ambient Red Data (RW) */
#define B3_GA_V0_MAT_FRONT_AMBIENT_GREEN  0x00C80C04 /* Vertex 0 Material Front Ambient Green Data (RW) */
#define B3_GA_V0_MAT_FRONT_AMBIENT_BLUE   0x00C80C08 /* Vertex 0 Material Front Ambient Blue Data (RW) */
#define B3_GA_V0_MAT_FRONT_DIFFUSE_RED    0x00C80C0C /* Vertex 0 Material Front Diffuse Red Data (RW) */
#define B3_GA_V0_MAT_FRONT_DIFFUSE_GREEN  0x00C80C10 /* Vertex 0 Material Front Diffuse Green Data (RW) */
#define B3_GA_V0_MAT_FRONT_DIFFUSE_BLUE   0x00C80C14 /* Vertex 0 Material Front Diffuse Blue Data (RW) */
#define B3_GA_V0_MAT_FRONT_SPECULAR_RED   0x00C80C18 /* Vertex 0 Material Front Specular Red Data (RW) */
#define B3_GA_V0_MAT_FRONT_SPECULAR_GREEN 0x00C80C1C /* Vertex 0 Material Front Specular Green Data (RW) */
#define B3_GA_V0_MAT_FRONT_SPECULAR_BLUE  0x00C80C20 /* Vertex 0 Material Front Specular Blue Data (RW) */
#define B3_GA_V0_MAT_FRONT_EMISSION_RED   0x00C80C24 /* Vertex 0 Material Front Emission Red Data (RW) */
#define B3_GA_V0_MAT_FRONT_EMISSION_GREEN 0x00C80C28 /* Vertex 0 Material Front Emission Green Data (RW) */
#define B3_GA_V0_MAT_FRONT_EMISSION_BLUE  0x00C80C2C /* Vertex 0 Material Front Emission Blue Data (RW) */
#define B3_GA_V0_MAT_FRONT_SPEC_THRESH    0x00C80C30 /* Vertex 0 Material Front Specular Threshold Data (RW) */
#define B3_GA_V0_MAT_FRONT_SHININESS_FP   0x00C80C34 /* Vertex 0 Material Front Shininess Data (floating point) (RW) */
#define B3_GA_V0_MAT_FRONT_SHININESS_INT  0x00C80C38 /* Vertex 0 Material Front Shininess Data (integer) (RW) */
#define B3_GA_V0_MAT_FRONT_ALPHA          0x00C80C3C /* Vertex 0 Material Front Alpha Data (RW) */
#define B3_GA_V1_MAT_FRONT_AMBIENT_RED    0x00C80C40 /* Vertex 1 Material Front Ambient Red Data (RW) */
#define B3_GA_V1_MAT_FRONT_AMBIENT_GREEN  0x00C80C44 /* Vertex 1 Material Front Ambient Green Data (RW) */
#define B3_GA_V1_MAT_FRONT_AMBIENT_BLUE   0x00C80C48 /* Vertex 1 Material Front Ambient Blue Data (RW) */
#define B3_GA_V1_MAT_FRONT_DIFFUSE_RED    0x00C80C4C /* Vertex 1 Material Front Diffuse Red Data (RW) */
#define B3_GA_V1_MAT_FRONT_DIFFUSE_GREEN  0x00C80C50 /* Vertex 1 Material Front Diffuse Green Data (RW) */
#define B3_GA_V1_MAT_FRONT_DIFFUSE_BLUE   0x00C80C54 /* Vertex 1 Material Front Diffuse Blue Data (RW) */
#define B3_GA_V1_MAT_FRONT_SPECULAR_RED   0x00C80C58 /* Vertex 1 Material Front Specular Red Data (RW) */
#define B3_GA_V1_MAT_FRONT_SPECULAR_GREEN 0x00C80C5C /* Vertex 1 Material Front Specular Green Data (RW) */
#define B3_GA_V1_MAT_FRONT_SPECULAR_BLUE  0x00C80C60 /* Vertex 1 Material Front Specular Blue Data (RW) */
#define B3_GA_V1_MAT_FRONT_EMISSION_RED   0x00C80C64 /* Vertex 1 Material Front Emission Red Data (RW) */
#define B3_GA_V1_MAT_FRONT_EMISSION_GREEN 0x00C80C68 /* Vertex 1 Material Front Emission Green Data (RW) */
#define B3_GA_V1_MAT_FRONT_EMISSION_BLUE  0x00C80C6C /* Vertex 1 Material Front Emission Blue Data (RW) */
#define B3_GA_V1_MAT_FRONT_SPEC_THRESH    0x00C80C70 /* Vertex 1 Material Front Specular Threshold Data (RW) */
#define B3_GA_V1_MAT_FRONT_SHININESS_FP   0x00C80C74 /* Vertex 1 Material Front Shininess Data (floating point) (RW) */
#define B3_GA_V1_MAT_FRONT_SHININESS_INT  0x00C80C78 /* Vertex 1 Material Front Shininess Data (integer) (RW) */
#define B3_GA_V1_MAT_FRONT_ALPHA          0x00C80C7C /* Vertex 1 Material Front Alpha Data (RW) */
#define B3_GA_V2_MAT_FRONT_AMBIENT_RED    0x00C80C80 /* Vertex 2 Material Front Ambient Red Data (RW) */
#define B3_GA_V2_MAT_FRONT_AMBIENT_GREEN  0x00C80C84 /* Vertex 2 Material Front Ambient Green Data (RW) */
#define B3_GA_V2_MAT_FRONT_AMBIENT_BLUE   0x00C80C88 /* Vertex 2 Material Front Ambient Blue Data (RW) */
#define B3_GA_V2_MAT_FRONT_DIFFUSE_RED    0x00C80C8C /* Vertex 2 Material Front Diffuse Red Data (RW) */
#define B3_GA_V2_MAT_FRONT_DIFFUSE_GREEN  0x00C80C90 /* Vertex 2 Material Front Diffuse Green Data (RW) */
#define B3_GA_V2_MAT_FRONT_DIFFUSE_BLUE   0x00C80C94 /* Vertex 2 Material Front Diffuse Blue Data (RW) */
#define B3_GA_V2_MAT_FRONT_SPECULAR_RED   0x00C80C98 /* Vertex 2 Material Front Specular Red Data (RW) */
#define B3_GA_V2_MAT_FRONT_SPECULAR_GREEN 0x00C80C9C /* Vertex 2 Material Front Specular Green Data (RW) */
#define B3_GA_V2_MAT_FRONT_SPECULAR_BLUE  0x00C80CA0 /* Vertex 2 Material Front Specular Blue Data (RW) */
#define B3_GA_V2_MAT_FRONT_EMISSION_RED   0x00C80CA4 /* Vertex 2 Material Front Emission Red Data (RW) */
#define B3_GA_V2_MAT_FRONT_EMISSION_GREEN 0x00C80CA8 /* Vertex 2 Material Front Emission Green Data (RW) */
#define B3_GA_V2_MAT_FRONT_EMISSION_BLUE  0x00C80CAC /* Vertex 2 Material Front Emission Blue Data (RW) */
#define B3_GA_V2_MAT_FRONT_SPEC_THRESH    0x00C80CB0 /* Vertex 2 Material Front Specular Threshold Data (RW) */
#define B3_GA_V2_MAT_FRONT_SHININESS_FP   0x00C80CB4 /* Vertex 2 Material Front Shininess Data (floating point) (RW) */
#define B3_GA_V2_MAT_FRONT_SHININESS_INT  0x00C80CB8 /* Vertex 2 Material Front Shininess Data (integer) (RW) */
#define B3_GA_V2_MAT_FRONT_ALPHA          0x00C80CBC /* Vertex 2 Material Front Alpha Data (RW) */
#define B3_GA_V3_MAT_FRONT_AMBIENT_RED    0x00C80CC0 /* Vertex 3 Material Front Ambient Red Data (RW) */
#define B3_GA_V3_MAT_FRONT_AMBIENT_GREEN  0x00C80CC4 /* Vertex 3 Material Front Ambient Green Data (RW) */
#define B3_GA_V3_MAT_FRONT_AMBIENT_BLUE   0x00C80CC8 /* Vertex 3 Material Front Ambient Blue Data (RW) */
#define B3_GA_V3_MAT_FRONT_DIFFUSE_RED    0x00C80CCC /* Vertex 3 Material Front Diffuse Red Data (RW) */
#define B3_GA_V3_MAT_FRONT_DIFFUSE_GREEN  0x00C80CD0 /* Vertex 3 Material Front Diffuse Green Data (RW) */
#define B3_GA_V3_MAT_FRONT_DIFFUSE_BLUE   0x00C80CD4 /* Vertex 3 Material Front Diffuse Blue Data (RW) */
#define B3_GA_V3_MAT_FRONT_SPECULAR_RED   0x00C80CD8 /* Vertex 3 Material Front Specular Red Data (RW) */
#define B3_GA_V3_MAT_FRONT_SPECULAR_GREEN 0x00C80CDC /* Vertex 3 Material Front Specular Green Data (RW) */
#define B3_GA_V3_MAT_FRONT_SPECULAR_BLUE  0x00C80CE0 /* Vertex 3 Material Front Specular Blue Data (RW) */
#define B3_GA_V3_MAT_FRONT_EMISSION_RED   0x00C80CE4 /* Vertex 3 Material Front Emission Red Data (RW) */
#define B3_GA_V3_MAT_FRONT_EMISSION_GREEN 0x00C80CE8 /* Vertex 3 Material Front Emission Green Data (RW) */
#define B3_GA_V3_MAT_FRONT_EMISSION_BLUE  0x00C80CEC /* Vertex 3 Material Front Emission Blue Data (RW) */
#define B3_GA_V3_MAT_FRONT_SPEC_THRESH    0x00C80CF0 /* Vertex 3 Material Front Specular Threshold Data (RW) */
#define B3_GA_V3_MAT_FRONT_SHININESS_FP   0x00C80CF4 /* Vertex 3 Material Front Shininess Data (floating point) (RW) */
#define B3_GA_V3_MAT_FRONT_SHININESS_INT  0x00C80CF8 /* Vertex 3 Material Front Shininess Data (integer) (RW) */
#define B3_GA_V3_MAT_FRONT_ALPHA          0x00C80CFC /* Vertex 3 Material Front Alpha Data (RW) */
#define B3_GA_V0_MAT_BACK_AMBIENT_RED     0x00C80D00 /* Vertex 0 Material Back Ambient Red Data (RW) */
#define B3_GA_V0_MAT_BACK_AMBIENT_GREEN   0x00C80D04 /* Vertex 0 Material Back Ambient Green Data (RW) */
#define B3_GA_V0_MAT_BACK_AMBIENT_BLUE    0x00C80D08 /* Vertex 0 Material Back Ambient Blue Data (RW) */
#define B3_GA_V0_MAT_BACK_DIFFUSE_RED     0x00C80D0C /* Vertex 0 Material Back Diffuse Red Data (RW) */
#define B3_GA_V0_MAT_BACK_DIFFUSE_GREEN   0x00C80D10 /* Vertex 0 Material Back Diffuse Green Data (RW) */
#define B3_GA_V0_MAT_BACK_DIFFUSE_BLUE    0x00C80D14 /* Vertex 0 Material Back Diffuse Blue Data (RW) */
#define B3_GA_V0_MAT_BACK_SPECULAR_RED    0x00C80D18 /* Vertex 0 Material Back Specular Red Data (RW) */
#define B3_GA_V0_MAT_BACK_SPECULAR_GREEN  0x00C80D1C /* Vertex 0 Material Back Specular Green Data (RW) */
#define B3_GA_V0_MAT_BACK_SPECULAR_BLUE   0x00C80D20 /* Vertex 0 Material Back Specular Blue Data (RW) */
#define B3_GA_V0_MAT_BACK_EMISSION_RED    0x00C80D24 /* Vertex 0 Material Back Emission Red Data (RW) */
#define B3_GA_V0_MAT_BACK_EMISSION_GREEN  0x00C80D28 /* Vertex 0 Material Back Emission Green Data (RW) */
#define B3_GA_V0_MAT_BACK_EMISSION_BLUE   0x00C80D2C /* Vertex 0 Material Back Emission Blue Data (RW) */
#define B3_GA_V0_MAT_BACK_SPEC_THRESH     0x00C80D30 /* Vertex 0 Material Back Specular Threshold Data (RW) */
#define B3_GA_V0_MAT_BACK_SHININESS_FP    0x00C80D34 /* Vertex 0 Material Back Shininess Data (floating point) (RW) */
#define B3_GA_V0_MAT_BACK_SHININESS_INT   0x00C80D38 /* Vertex 0 Material Back Shininess Data (integer) (RW) */
#define B3_GA_V0_MAT_BACK_ALPHA           0x00C80D3C /* Vertex 0 Material Back Alpha Data (RW) */
#define B3_GA_V1_MAT_BACK_AMBIENT_RED     0x00C80D40 /* Vertex 1 Material Back Ambient Red Data (RW) */
#define B3_GA_V1_MAT_BACK_AMBIENT_GREEN   0x00C80D44 /* Vertex 1 Material Back Ambient Green Data (RW) */
#define B3_GA_V1_MAT_BACK_AMBIENT_BLUE    0x00C80D48 /* Vertex 1 Material Back Ambient Blue Data (RW) */
#define B3_GA_V1_MAT_BACK_DIFFUSE_RED     0x00C80D4C /* Vertex 1 Material Back Diffuse Red Data (RW) */
#define B3_GA_V1_MAT_BACK_DIFFUSE_GREEN   0x00C80D50 /* Vertex 1 Material Back Diffuse Green Data (RW) */
#define B3_GA_V1_MAT_BACK_DIFFUSE_BLUE    0x00C80D54 /* Vertex 1 Material Back Diffuse Blue Data (RW) */
#define B3_GA_V1_MAT_BACK_SPECULAR_RED    0x00C80D58 /* Vertex 1 Material Back Specular Red Data (RW) */
#define B3_GA_V1_MAT_BACK_SPECULAR_GREEN  0x00C80D5C /* Vertex 1 Material Back Specular Green Data (RW) */
#define B3_GA_V1_MAT_BACK_SPECULAR_BLUE   0x00C80D60 /* Vertex 1 Material Back Specular Blue Data (RW) */
#define B3_GA_V1_MAT_BACK_EMISSION_RED    0x00C80D64 /* Vertex 1 Material Back Emission Red Data (RW) */
#define B3_GA_V1_MAT_BACK_EMISSION_GREEN  0x00C80D68 /* Vertex 1 Material Back Emission Green Data (RW) */
#define B3_GA_V1_MAT_BACK_EMISSION_BLUE   0x00C80D6C /* Vertex 1 Material Back Emission Blue Data (RW) */
#define B3_GA_V1_MAT_BACK_SPEC_THRESH     0x00C80D70 /* Vertex 1 Material Back Specular Threshold Data (RW) */
#define B3_GA_V1_MAT_BACK_SHININESS_FP    0x00C80D74 /* Vertex 1 Material Back Shininess Data (floating point) (RW) */
#define B3_GA_V1_MAT_BACK_SHININESS_INT   0x00C80D78 /* Vertex 1 Material Back Shininess Data (integer) (RW) */
#define B3_GA_V1_MAT_BACK_ALPHA           0x00C80D7C /* Vertex 1 Material Back Alpha Data (RW) */
#define B3_GA_V2_MAT_BACK_AMBIENT_RED     0x00C80D80 /* Vertex 2 Material Back Ambient Red Data (RW) */
#define B3_GA_V2_MAT_BACK_AMBIENT_GREEN   0x00C80D84 /* Vertex 2 Material Back Ambient Green Data (RW) */
#define B3_GA_V2_MAT_BACK_AMBIENT_BLUE    0x00C80D88 /* Vertex 2 Material Back Ambient Blue Data (RW) */
#define B3_GA_V2_MAT_BACK_DIFFUSE_RED     0x00C80D8C /* Vertex 2 Material Back Diffuse Red Data (RW) */
#define B3_GA_V2_MAT_BACK_DIFFUSE_GREEN   0x00C80D90 /* Vertex 2 Material Back Diffuse Green Data (RW) */
#define B3_GA_V2_MAT_BACK_DIFFUSE_BLUE    0x00C80D94 /* Vertex 2 Material Back Diffuse Blue Data (RW) */
#define B3_GA_V2_MAT_BACK_SPECULAR_RED    0x00C80D98 /* Vertex 2 Material Back Specular Red Data (RW) */
#define B3_GA_V2_MAT_BACK_SPECULAR_GREEN  0x00C80D9C /* Vertex 2 Material Back Specular Green Data (RW) */
#define B3_GA_V2_MAT_BACK_SPECULAR_BLUE   0x00C80DA0 /* Vertex 2 Material Back Specular Blue Data (RW) */
#define B3_GA_V2_MAT_BACK_EMISSION_RED    0x00C80DA4 /* Vertex 2 Material Back Emission Red Data (RW) */
#define B3_GA_V2_MAT_BACK_EMISSION_GREEN  0x00C80DA8 /* Vertex 2 Material Back Emission Green Data (RW) */
#define B3_GA_V2_MAT_BACK_EMISSION_BLUE   0x00C80DAC /* Vertex 2 Material Back Emission Blue Data (RW) */
#define B3_GA_V2_MAT_BACK_SPEC_THRESH     0x00C80DB0 /* Vertex 2 Material Back Specular Threshold Data (RW) */
#define B3_GA_V2_MAT_BACK_SHININESS_FP    0x00C80DB4 /* Vertex 2 Material Back Shininess Data (floating point) (RW) */
#define B3_GA_V2_MAT_BACK_SHININESS_INT   0x00C80DB8 /* Vertex 2 Material Back Shininess Data (integer) (RW) */
#define B3_GA_V2_MAT_BACK_ALPHA           0x00C80DBC /* Vertex 2 Material Back Alpha Data (RW) */
#define B3_GA_V3_MAT_BACK_AMBIENT_RED     0x00C80DC0 /* Vertex 3 Material Back Ambient Red Data (RW) */
#define B3_GA_V3_MAT_BACK_AMBIENT_GREEN   0x00C80DC4 /* Vertex 3 Material Back Ambient Green Data (RW) */
#define B3_GA_V3_MAT_BACK_AMBIENT_BLUE    0x00C80DC8 /* Vertex 3 Material Back Ambient Blue Data (RW) */
#define B3_GA_V3_MAT_BACK_DIFFUSE_RED     0x00C80DCC /* Vertex 3 Material Back Diffuse Red Data (RW) */
#define B3_GA_V3_MAT_BACK_DIFFUSE_GREEN   0x00C80DD0 /* Vertex 3 Material Back Diffuse Green Data (RW) */
#define B3_GA_V3_MAT_BACK_DIFFUSE_BLUE    0x00C80DD4 /* Vertex 3 Material Back Diffuse Blue Data (RW) */
#define B3_GA_V3_MAT_BACK_SPECULAR_RED    0x00C80DD8 /* Vertex 3 Material Back Specular Red Data (RW) */
#define B3_GA_V3_MAT_BACK_SPECULAR_GREEN  0x00C80DDC /* Vertex 3 Material Back Specular Green Data (RW) */
#define B3_GA_V3_MAT_BACK_SPECULAR_BLUE   0x00C80DE0 /* Vertex 3 Material Back Specular Blue Data (RW) */
#define B3_GA_V3_MAT_BACK_EMISSION_RED    0x00C80DE4 /* Vertex 3 Material Back Emission Red Data (RW) */
#define B3_GA_V3_MAT_BACK_EMISSION_GREEN  0x00C80DE8 /* Vertex 3 Material Back Emission Green Data (RW) */
#define B3_GA_V3_MAT_BACK_EMISSION_BLUE   0x00C80DEC /* Vertex 3 Material Back Emission Blue Data (RW) */
#define B3_GA_V3_MAT_BACK_SPEC_THRESH     0x00C80DF0 /* Vertex 3 Material Back Specular Threshold Data (RW) */
#define B3_GA_V3_MAT_BACK_SHININESS_FP    0x00C80DF4 /* Vertex 3 Material Back Shininess Data (floating point) (RW) */
#define B3_GA_V3_MAT_BACK_SHININESS_INT   0x00C80DF8 /* Vertex 3 Material Back Shininess Data (integer) (RW) */
#define B3_GA_V3_MAT_BACK_ALPHA           0x00C80DFC /* Vertex 3 Material Back Alpha Data (RW) */
#define B3_GA_EYE_POSITION_X              0x00C80E00 /* Eye Position X (RW) */
#define B3_GA_EYE_POSITION_Y              0x00C80E04 /* Eye Position Y (RW) */
#define B3_GA_EYE_POSITION_Z              0x00C80E08 /* Eye Position Z (RW) */
#define B3_GA_GLOBAL_AMBIENT_RED          0x00C80E0C /* Global Ambient Red Data (RW) */
#define B3_GA_GLOBAL_AMBIENT_GREEN        0x00C80E10 /* Global Ambient Green Data (RW) */
#define B3_GA_GLOBAL_AMBIENT_BLUE         0x00C80E14 /* Global Ambient Blue Data (RW) */
#define B3_GA_MINSCALE                    0x00C80E18 /* Depth Cue Minimum Scale Factor (RW) */
#define B3_GA_MAXSCALE                    0x00C80E1C /* Depth Cue Maximum Scale Factor (RW) */
#define B3_GA_FOG_START                   0x00C80E20 /* Fog Z Start Value (RW) */
#define B3_GA_FOG_END                     0x00C80E24 /* Fog Z End Value (RW) */
#define B3_GA_FOG_DENSITY                 0x00C80E28 /* Fog Density Value (RW) */
#define B3_GA_CLAMP_ZMAX                  0x00C80E2C /* Zmax Clamp Register (RW) */
#define B3_GA_CLAMP_ZMIN                  0x00C80E30 /* Zmin Clamp Register (RW) */
#define B3_GA_SIXTEEN                     0x00C80E34 /* Blitzen RAM Sixteen Register (RW) */
#define B3_GA_LOG2_0                      0x00C80E38 /* Entry 0 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_1                      0x00C80E3C /* Entry 1 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_2                      0x00C80E40 /* Entry 2 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_3                      0x00C80E44 /* Entry 3 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_4                      0x00C80E48 /* Entry 4 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_5                      0x00C80E4C /* Entry 5 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_6                      0x00C80E50 /* Entry 6 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_7                      0x00C80E54 /* Entry 7 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_8                      0x00C80E58 /* Entry 8 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_9                      0x00C80E5C /* Entry 9 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_10                     0x00C80E60 /* Entry 10 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_11                     0x00C80E64 /* Entry 11 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_12                     0x00C80E68 /* Entry 12 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_13                     0x00C80E6C /* Entry 13 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_14                     0x00C80E70 /* Entry 14 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_15                     0x00C80E74 /* Entry 15 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_LOG2_16                     0x00C80E78 /* Entry 16 Of Lookup Table For Log Base 2 (RW) */
#define B3_GA_FLT_EPSILON                 0x00C80E7C /*  () */
#define B3_GA_ONE_EIGHTH                  0x00C80F0C /* Constant 1/8 (RW) */
#define B3_GA_ONE_HALF                    0x00C80F1C /* Constant 1/2 (0.5) (RW) */
#define B3_GA_Z_OFFSET_FACTOR             0x00C80F4C /* Z Offset Factor Used For Polygon Offset (RW) */
#define B3_GA_Z_OFFSET_BIAS               0x00C80F8C /* Z Offset Bias Used For Polygon Offset (RW) */
#define B3_GA_L0_AMBIENT_RED              0x00C90000 /* Light 0 Ambient Red Data (RW) */
#define B3_GA_L1_AMBIENT_RED              0x00C90004 /* Light 1 Ambient Red Data (RW) */
#define B3_GA_L2_AMBIENT_RED              0x00C90008 /* Light 2 Ambient Red Data (RW) */
#define B3_GA_L3_AMBIENT_RED              0x00C9000C /* Light 3 Ambient Red Data (RW) */
#define B3_GA_L4_AMBIENT_RED              0x00C90010 /* Light 4 Ambient Red Data (RW) */
#define B3_GA_L5_AMBIENT_RED              0x00C90014 /* Light 5 Ambient Red Data (RW) */
#define B3_GA_L6_AMBIENT_RED              0x00C90018 /* Light 6 Ambient Red Data (RW) */
#define B3_GA_L7_AMBIENT_RED              0x00C9001C /* Light 7 Ambient Red Data (RW) */
#define B3_GA_L0_AMBIENT_GREEN            0x00C90020 /* Light 0 Ambient Green Data (RW) */
#define B3_GA_L1_AMBIENT_GREEN            0x00C90024 /* Light 1 Ambient Green Data (RW) */
#define B3_GA_L2_AMBIENT_GREEN            0x00C90028 /* Light 2 Ambient Green Data (RW) */
#define B3_GA_L3_AMBIENT_GREEN            0x00C9002C /* Light 3 Ambient Green Data (RW) */
#define B3_GA_L4_AMBIENT_GREEN            0x00C90030 /* Light 4 Ambient Green Data (RW) */
#define B3_GA_L5_AMBIENT_GREEN            0x00C90034 /* Light 5 Ambient Green Data (RW) */
#define B3_GA_L6_AMBIENT_GREEN            0x00C90038 /* Light 6 Ambient Green Data (RW) */
#define B3_GA_L7_AMBIENT_GREEN            0x00C9003C /* Light 7 Ambient Green Data (RW) */
#define B3_GA_L0_AMBIENT_BLUE             0x00C90040 /* Light 0 Ambient Blue Data (RW) */
#define B3_GA_L1_AMBIENT_BLUE             0x00C90044 /* Light 1 Ambient Blue Data (RW) */
#define B3_GA_L2_AMBIENT_BLUE             0x00C90048 /* Light 2 Ambient Blue Data (RW) */
#define B3_GA_L3_AMBIENT_BLUE             0x00C9004C /* Light 3 Ambient Blue Data (RW) */
#define B3_GA_L4_AMBIENT_BLUE             0x00C90050 /* Light 4 Ambient Blue Data (RW) */
#define B3_GA_L5_AMBIENT_BLUE             0x00C90054 /* Light 5 Ambient Blue Data (RW) */
#define B3_GA_L6_AMBIENT_BLUE             0x00C90058 /* Light 6 Ambient Blue Data (RW) */
#define B3_GA_L7_AMBIENT_BLUE             0x00C9005C /* Light 7 Ambient Blue Data (RW) */
#define B3_GA_L0_DIFFUSE_RED              0x00C90060 /* Light 0 Diffuse Red Data (RW) */
#define B3_GA_L1_DIFFUSE_RED              0x00C90064 /* Light 1 Diffuse Red Data (RW) */
#define B3_GA_L2_DIFFUSE_RED              0x00C90068 /* Light 2 Diffuse Red Data (RW) */
#define B3_GA_L3_DIFFUSE_RED              0x00C9006C /* Light 3 Diffuse Red Data (RW) */
#define B3_GA_L4_DIFFUSE_RED              0x00C90070 /* Light 4 Diffuse Red Data (RW) */
#define B3_GA_L5_DIFFUSE_RED              0x00C90074 /* Light 5 Diffuse Red Data (RW) */
#define B3_GA_L6_DIFFUSE_RED              0x00C90078 /* Light 6 Diffuse Red Data (RW) */
#define B3_GA_L7_DIFFUSE_RED              0x00C9007C /* Light 7 Diffuse Red Data (RW) */
#define B3_GA_L0_DIFFUSE_GREEN            0x00C90080 /* Light 0 Diffuse Green Data (RW) */
#define B3_GA_L1_DIFFUSE_GREEN            0x00C90084 /* Light 1 Diffuse Green Data (RW) */
#define B3_GA_L2_DIFFUSE_GREEN            0x00C90088 /* Light 2 Diffuse Green Data (RW) */
#define B3_GA_L3_DIFFUSE_GREEN            0x00C9008C /* Light 3 Diffuse Green Data (RW) */
#define B3_GA_L4_DIFFUSE_GREEN            0x00C90090 /* Light 4 Diffuse Green Data (RW) */
#define B3_GA_L5_DIFFUSE_GREEN            0x00C90094 /* Light 5 Diffuse Green Data (RW) */
#define B3_GA_L6_DIFFUSE_GREEN            0x00C90098 /* Light 6 Diffuse Green Data (RW) */
#define B3_GA_L7_DIFFUSE_GREEN            0x00C9009C /* Light 7 Diffuse Green Data (RW) */
#define B3_GA_L0_DIFFUSE_BLUE             0x00C900A0 /* Light 0 Diffuse Blue Data (RW) */
#define B3_GA_L1_DIFFUSE_BLUE             0x00C900A4 /* Light 1 Diffuse Blue Data (RW) */
#define B3_GA_L2_DIFFUSE_BLUE             0x00C900A8 /* Light 2 Diffuse Blue Data (RW) */
#define B3_GA_L3_DIFFUSE_BLUE             0x00C900AC /* Light 3 Diffuse Blue Data (RW) */
#define B3_GA_L4_DIFFUSE_BLUE             0x00C900B0 /* Light 4 Diffuse Blue Data (RW) */
#define B3_GA_L5_DIFFUSE_BLUE             0x00C900B4 /* Light 5 Diffuse Blue Data (RW) */
#define B3_GA_L6_DIFFUSE_BLUE             0x00C900B8 /* Light 6 Diffuse Blue Data (RW) */
#define B3_GA_L7_DIFFUSE_BLUE             0x00C900BC /* Light 7 Diffuse Blue Data (RW) */
#define B3_GA_L0_SPECULAR_RED             0x00C900C0 /* Light 0 Specular Red Data (RW) */
#define B3_GA_L1_SPECULAR_RED             0x00C900C4 /* Light 1 Specular Red Data (RW) */
#define B3_GA_L2_SPECULAR_RED             0x00C900C8 /* Light 2 Specular Red Data (RW) */
#define B3_GA_L3_SPECULAR_RED             0x00C900CC /* Light 3 Specular Red Data (RW) */
#define B3_GA_L4_SPECULAR_RED             0x00C900D0 /* Light 4 Specular Red Data (RW) */
#define B3_GA_L5_SPECULAR_RED             0x00C900D4 /* Light 5 Specular Red Data (RW) */
#define B3_GA_L6_SPECULAR_RED             0x00C900D8 /* Light 6 Specular Red Data (RW) */
#define B3_GA_L7_SPECULAR_RED             0x00C900DC /* Light 7 Specular Red Data (RW) */
#define B3_GA_L0_SPECULAR_GREEN           0x00C900E0 /* Light 0 Specular Green Data (RW) */
#define B3_GA_L1_SPECULAR_GREEN           0x00C900E4 /* Light 1 Specular Green Data (RW) */
#define B3_GA_L2_SPECULAR_GREEN           0x00C900E8 /* Light 2 Specular Green Data (RW) */
#define B3_GA_L3_SPECULAR_GREEN           0x00C900EC /* Light 3 Specular Green Data (RW) */
#define B3_GA_L4_SPECULAR_GREEN           0x00C900F0 /* Light 4 Specular Green Data (RW) */
#define B3_GA_L5_SPECULAR_GREEN           0x00C900F4 /* Light 5 Specular Green Data (RW) */
#define B3_GA_L6_SPECULAR_GREEN           0x00C900F8 /* Light 6 Specular Green Data (RW) */
#define B3_GA_L7_SPECULAR_GREEN           0x00C900FC /* Light 7 Specular Green Data (RW) */
#define B3_GA_L0_SPECULAR_BLUE            0x00C90100 /* Light 0 Specular Blue Data (RW) */
#define B3_GA_L1_SPECULAR_BLUE            0x00C90104 /* Light 1 Specular Blue Data (RW) */
#define B3_GA_L2_SPECULAR_BLUE            0x00C90108 /* Light 2 Specular Blue Data (RW) */
#define B3_GA_L3_SPECULAR_BLUE            0x00C9010C /* Light 3 Specular Blue Data (RW) */
#define B3_GA_L4_SPECULAR_BLUE            0x00C90110 /* Light 4 Specular Blue Data (RW) */
#define B3_GA_L5_SPECULAR_BLUE            0x00C90114 /* Light 5 Specular Blue Data (RW) */
#define B3_GA_L6_SPECULAR_BLUE            0x00C90118 /* Light 6 Specular Blue Data (RW) */
#define B3_GA_L7_SPECULAR_BLUE            0x00C9011C /* Light 7 Specular Blue Data (RW) */
#define B3_GA_L0_POSITION_X               0x00C90120 /* Light 0 Position X Data (RW) */
#define B3_GA_L1_POSITION_X               0x00C90124 /* Light 1 Position X Data (RW) */
#define B3_GA_L2_POSITION_X               0x00C90128 /* Light 2 Position X Data (RW) */
#define B3_GA_L3_POSITION_X               0x00C9012C /* Light 3 Position X Data (RW) */
#define B3_GA_L4_POSITION_X               0x00C90130 /* Light 4 Position X Data (RW) */
#define B3_GA_L5_POSITION_X               0x00C90134 /* Light 5 Position X Data (RW) */
#define B3_GA_L6_POSITION_X               0x00C90138 /* Light 6 Position X Data (RW) */
#define B3_GA_L7_POSITION_X               0x00C9013C /* Light 7 Position X Data (RW) */
#define B3_GA_L0_POSITION_Y               0x00C90140 /* Light 0 Position Y Data (RW) */
#define B3_GA_L1_POSITION_Y               0x00C90144 /* Light 1 Position Y Data (RW) */
#define B3_GA_L2_POSITION_Y               0x00C90148 /* Light 2 Position Y Data (RW) */
#define B3_GA_L3_POSITION_Y               0x00C9014C /* Light 3 Position Y Data (RW) */
#define B3_GA_L4_POSITION_Y               0x00C90150 /* Light 4 Position Y Data (RW) */
#define B3_GA_L5_POSITION_Y               0x00C90154 /* Light 5 Position Y Data (RW) */
#define B3_GA_L6_POSITION_Y               0x00C90158 /* Light 6 Position Y Data (RW) */
#define B3_GA_L7_POSITION_Y               0x00C9015C /* Light 7 Position Y Data (RW) */
#define B3_GA_L0_POSITION_Z               0x00C90160 /* Light 0 Position Z Data (RW) */
#define B3_GA_L1_POSITION_Z               0x00C90164 /* Light 1 Position Z Data (RW) */
#define B3_GA_L2_POSITION_Z               0x00C90168 /* Light 2 Position Z Data (RW) */
#define B3_GA_L3_POSITION_Z               0x00C9016C /* Light 3 Position Z Data (RW) */
#define B3_GA_L4_POSITION_Z               0x00C90170 /* Light 4 Position Z Data (RW) */
#define B3_GA_L5_POSITION_Z               0x00C90174 /* Light 5 Position Z Data (RW) */
#define B3_GA_L6_POSITION_Z               0x00C90178 /* Light 6 Position Z Data (RW) */
#define B3_GA_L7_POSITION_Z               0x00C9017C /* Light 7 Position Z Data (RW) */
#define B3_GA_L0_SPOT_DIR_X               0x00C90180 /* Light 0 Spot Direction X Data (RW) */
#define B3_GA_L1_SPOT_DIR_X               0x00C90184 /* Light 1 Spot Direction X Data (RW) */
#define B3_GA_L2_SPOT_DIR_X               0x00C90188 /* Light 2 Spot Direction X Data (RW) */
#define B3_GA_L3_SPOT_DIR_X               0x00C9018C /* Light 3 Spot Direction X Data (RW) */
#define B3_GA_L4_SPOT_DIR_X               0x00C90190 /* Light 4 Spot Direction X Data (RW) */
#define B3_GA_L5_SPOT_DIR_X               0x00C90194 /* Light 5 Spot Direction X Data (RW) */
#define B3_GA_L6_SPOT_DIR_X               0x00C90198 /* Light 6 Spot Direction X Data (RW) */
#define B3_GA_L7_SPOT_DIR_X               0x00C9019C /* Light 7 Spot Direction X Data (RW) */
#define B3_GA_L0_SPOT_DIR_Y               0x00C901A0 /* Light 0 Spot Direction Y Data (RW) */
#define B3_GA_L1_SPOT_DIR_Y               0x00C901A4 /* Light 1 Spot Direction Y Data (RW) */
#define B3_GA_L2_SPOT_DIR_Y               0x00C901A8 /* Light 2 Spot Direction Y Data (RW) */
#define B3_GA_L3_SPOT_DIR_Y               0x00C901AC /* Light 3 Spot Direction Y Data (RW) */
#define B3_GA_L4_SPOT_DIR_Y               0x00C901B0 /* Light 4 Spot Direction Y Data (RW) */
#define B3_GA_L5_SPOT_DIR_Y               0x00C901B4 /* Light 5 Spot Direction Y Data (RW) */
#define B3_GA_L6_SPOT_DIR_Y               0x00C901B8 /* Light 6 Spot Direction Y Data (RW) */
#define B3_GA_L7_SPOT_DIR_Y               0x00C901BC /* Light 7 Spot Direction Y Data (RW) */
#define B3_GA_L0_SPOT_DIR_Z               0x00C901C0 /* Light 0 Spot Direction Z Data (RW) */
#define B3_GA_L1_SPOT_DIR_Z               0x00C901C4 /* Light 1 Spot Direction Z Data (RW) */
#define B3_GA_L2_SPOT_DIR_Z               0x00C901C8 /* Light 2 Spot Direction Z Data (RW) */
#define B3_GA_L3_SPOT_DIR_Z               0x00C901CC /* Light 3 Spot Direction Z Data (RW) */
#define B3_GA_L4_SPOT_DIR_Z               0x00C901D0 /* Light 4 Spot Direction Z Data (RW) */
#define B3_GA_L5_SPOT_DIR_Z               0x00C901D4 /* Light 5 Spot Direction Z Data (RW) */
#define B3_GA_L6_SPOT_DIR_Z               0x00C901D8 /* Light 6 Spot Direction Z Data (RW) */
#define B3_GA_L7_SPOT_DIR_Z               0x00C901DC /* Light 7 Spot Direction Z Data (RW) */
#define B3_GA_L0_SPOT_EXP_FP              0x00C901E0 /* Light 0 Spot Exponent (floating point) (RW) */
#define B3_GA_L1_SPOT_EXP_FP              0x00C901E4 /* Light 1 Spot Exponent (floating point) (RW) */
#define B3_GA_L2_SPOT_EXP_FP              0x00C901E8 /* Light 2 Spot Exponent (floating point) (RW) */
#define B3_GA_L3_SPOT_EXP_FP              0x00C901EC /* Light 3 Spot Exponent (floating point) (RW) */
#define B3_GA_L4_SPOT_EXP_FP              0x00C901F0 /* Light 4 Spot Exponent (floating point) (RW) */
#define B3_GA_L5_SPOT_EXP_FP              0x00C901F4 /* Light 5 Spot Exponent (floating point) (RW) */
#define B3_GA_L6_SPOT_EXP_FP              0x00C901F8 /* Light 6 Spot Exponent (floating point) (RW) */
#define B3_GA_L7_SPOT_EXP_FP              0x00C901FC /* Light 7 Spot Exponent (floating point) (RW) */
#define B3_GA_L0_SPOT_CUTOFF              0x00C90200 /* Light 0 Spot Cutoff (RW) */
#define B3_GA_L1_SPOT_CUTOFF              0x00C90204 /* Light 1 Spot Cutoff (RW) */
#define B3_GA_L2_SPOT_CUTOFF              0x00C90208 /* Light 2 Spot Cutoff (RW) */
#define B3_GA_L3_SPOT_CUTOFF              0x00C9020C /* Light 3 Spot Cutoff (RW) */
#define B3_GA_L4_SPOT_CUTOFF              0x00C90210 /* Light 4 Spot Cutoff (RW) */
#define B3_GA_L5_SPOT_CUTOFF              0x00C90214 /* Light 5 Spot Cutoff (RW) */
#define B3_GA_L6_SPOT_CUTOFF              0x00C90218 /* Light 6 Spot Cutoff (RW) */
#define B3_GA_L7_SPOT_CUTOFF              0x00C9021C /* Light 7 Spot Cutoff (RW) */
#define B3_GA_L0_ATTEN_CONST              0x00C90220 /* Light 0  Constant Attenuation Factor (RW) */
#define B3_GA_L1_ATTEN_CONST              0x00C90224 /* Light 1 Constant Attenuation Factor (RW) */
#define B3_GA_L2_ATTEN_CONST              0x00C90228 /* Light 2 Constant Attenuation Factor (RW) */
#define B3_GA_L3_ATTEN_CONST              0x00C9022C /* Light 3 Constant Attenuation Factor (RW) */
#define B3_GA_L4_ATTEN_CONST              0x00C90230 /* Light 4 Constant Attenuation Factor (RW) */
#define B3_GA_L5_ATTEN_CONST              0x00C90234 /* Light 5 Constant Attenuation Factor (RW) */
#define B3_GA_L6_ATTEN_CONST              0x00C90238 /* Light 6 Constant Attenuation Factor (RW) */
#define B3_GA_L7_ATTEN_CONST              0x00C9023C /* Light 7 Constant Attenuation Factor (RW) */
#define B3_GA_L0_ATTEN_LIN                0x00C90240 /* Light 0 Linear Attenuation Factor (RW) */
#define B3_GA_L1_ATTEN_LIN                0x00C90244 /* Light 1 Linear Attenuation Factor (RW) */
#define B3_GA_L2_ATTEN_LIN                0x00C90248 /* Light 2 Linear Attenuation Factor (RW) */
#define B3_GA_L3_ATTEN_LIN                0x00C9024C /* Light 3 Linear Attenuation Factor (RW) */
#define B3_GA_L4_ATTEN_LIN                0x00C90250 /* Light 4 Linear Attenuation Factor (RW) */
#define B3_GA_L5_ATTEN_LIN                0x00C90254 /* Light 5 Linear Attenuation Factor (RW) */
#define B3_GA_L6_ATTEN_LIN                0x00C90258 /* Light 6 Linear Attenuation Factor (RW) */
#define B3_GA_L7_ATTEN_LIN                0x00C9025C /* Light 7 Linear Attenuation Factor (RW) */
#define B3_GA_L0_ATTEN_QUAD               0x00C90260 /* Light 0 Quadratic Attenuation Factor (RW) */
#define B3_GA_L1_ATTEN_QUAD               0x00C90264 /* Light 1 Quadratic Attenuation Factor (RW) */
#define B3_GA_L2_ATTEN_QUAD               0x00C90268 /* Light 2 Quadratic Attenuation Factor (RW) */
#define B3_GA_L3_ATTEN_QUAD               0x00C9026C /* Light 3 Quadratic Attenuation Factor (RW) */
#define B3_GA_L4_ATTEN_QUAD               0x00C90270 /* Light 4 Quadratic Attenuation Factor (RW) */
#define B3_GA_L5_ATTEN_QUAD               0x00C90274 /* Light 5 Quadratic Attenuation Factor (RW) */
#define B3_GA_L6_ATTEN_QUAD               0x00C90278 /* Light 6 Quadratic Attenuation Factor (RW) */
#define B3_GA_L7_ATTEN_QUAD               0x00C9027C /* Light 7 Quadratic Attenuation Factor (RW) */
#define B3_GA_L0_SPOT_EXP_INT             0x00C90280 /* Light 0 Spot Exponent (integer) (RW) */
#define B3_GA_L1_SPOT_EXP_INT             0x00C90284 /* Light 1 Spot Exponent (integer) (RW) */
#define B3_GA_L2_SPOT_EXP_INT             0x00C90288 /* Light 2 Spot Exponent (integer) (RW) */
#define B3_GA_L3_SPOT_EXP_INT             0x00C9028C /* Light 3 Spot Exponent (integer) (RW) */
#define B3_GA_L4_SPOT_EXP_INT             0x00C90290 /* Light 4 Spot Exponent (integer) (RW) */
#define B3_GA_L5_SPOT_EXP_INT             0x00C90294 /* Light 5 Spot Exponent (integer) (RW) */
#define B3_GA_L6_SPOT_EXP_INT             0x00C90298 /* Light 6 Spot Exponent (integer) (RW) */
#define B3_GA_L7_SPOT_EXP_INT             0x00C9029C /* Light 7 Spot Exponent (integer) (RW) */
#define B3_GA_L0_D3D_LIGHT_RANGE          0x00C902A0 /* Light 0 D3D Positional Light Range Value (RW) */
#define B3_GA_L1_D3D_LIGHT_RANGE          0x00C902A4 /* Light 1 D3D Positional Light Range Value (RW) */
#define B3_GA_L2_D3D_LIGHT_RANGE          0x00C902A8 /* Light 2 D3D Positional Light Range Value (RW) */
#define B3_GA_L3_D3D_LIGHT_RANGE          0x00C902AC /* Light 3 D3D Positional Light Range Value (RW) */
#define B3_GA_L4_D3D_LIGHT_RANGE          0x00C902B0 /* Light 4 D3D Positional Light Range Value (RW) */
#define B3_GA_L5_D3D_LIGHT_RANGE          0x00C902B4 /* Light 5 D3D Positional Light Range Value (RW) */
#define B3_GA_L6_D3D_LIGHT_RANGE          0x00C902B8 /* Light 6 D3D Positional Light Range Value (RW) */
#define B3_GA_L7_D3D_LIGHT_RANGE          0x00C902BC /* Light 7 D3D Positional Light Range Value (RW) */
#define B3_GA_L0_PLUS_EYE_X               0x00C90EA0 /* Light 0 Direction + Eye Direction Vector X Value (RW) */
#define B3_GA_L1_PLUS_EYE_X               0x00C90EA4 /* Light 1 Direction + Eye Direction Vector X Value (RW) */
#define B3_GA_L2_PLUS_EYE_X               0x00C90EA8 /* Light 2 Direction + Eye Direction Vector X Value (RW) */
#define B3_GA_L3_PLUS_EYE_X               0x00C90EAC /* Light 3 Direction + Eye Direction Vector X Value (RW) */
#define B3_GA_L4_PLUS_EYE_X               0x00C90EB0 /* Light 4 Direction + Eye Direction Vector X Value (RW) */
#define B3_GA_L5_PLUS_EYE_X               0x00C90EB4 /* Light 5 Direction + Eye Direction Vector X Value (RW) */
#define B3_GA_L6_PLUS_EYE_X               0x00C90EB8 /* Light 6 Direction + Eye Direction Vector X Value (RW) */
#define B3_GA_L7_PLUS_EYE_X               0x00C90EBC /* Light 7 Direction + Eye Direction Vector X Value (RW) */
#define B3_GA_L0_PLUS_EYE_Y               0x00C90EC0 /* Light 0 Direction + Eye Direction Vector Y Value (RW) */
#define B3_GA_L1_PLUS_EYE_Y               0x00C90EC4 /* Light 1 Direction + Eye Direction Vector Y Value (RW) */
#define B3_GA_L2_PLUS_EYE_Y               0x00C90EC8 /* Light 2 Direction + Eye Direction Vector Y Value (RW) */
#define B3_GA_L3_PLUS_EYE_Y               0x00C90ECC /* Light 3 Direction + Eye Direction Vector Y Value (RW) */
#define B3_GA_L4_PLUS_EYE_Y               0x00C90ED0 /* Light 4 Direction + Eye Direction Vector Y Value (RW) */
#define B3_GA_L5_PLUS_EYE_Y               0x00C90ED4 /* Light 5 Direction + Eye Direction Vector Y Value (RW) */
#define B3_GA_L6_PLUS_EYE_Y               0x00C90ED8 /* Light 6 Direction + Eye Direction Vector Y Value (RW) */
#define B3_GA_L7_PLUS_EYE_Y               0x00C90EDC /* Light 7 Direction + Eye Direction Vector Y Value (RW) */
#define B3_GA_L0_PLUS_EYE_Z               0x00C90EE0 /* Light 0 Direction + Eye Direction Vector Z Value (RW) */
#define B3_GA_L1_PLUS_EYE_Z               0x00C90EE4 /* Light 1 Direction + Eye Direction Vector Z Value (RW) */
#define B3_GA_L2_PLUS_EYE_Z               0x00C90EE8 /* Light 2 Direction + Eye Direction Vector Z Value (RW) */
#define B3_GA_L3_PLUS_EYE_Z               0x00C90EEC /* Light 3 Direction + Eye Direction Vector Z Value (RW) */
#define B3_GA_L4_PLUS_EYE_Z               0x00C90EF0 /* Light 4 Direction + Eye Direction Vector Z Value (RW) */
#define B3_GA_L5_PLUS_EYE_Z               0x00C90EF4 /* Light 5 Direction + Eye Direction Vector Z Value (RW) */
#define B3_GA_L6_PLUS_EYE_Z               0x00C90EF8 /* Light 6 Direction + Eye Direction Vector Z Value (RW) */
#define B3_GA_L7_PLUS_EYE_Z               0x00C90EFC /* Light 7 Direction + Eye Direction Vector Z Value (RW) */
/*  End of Geometry Accelerator Registers A  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Geometry Accelerator Registers B  */
#define B3_GA_NOP                         0x00CA0000 /* No Operation (RW) */
#define B3_GA_STATE_RESTORE               0x00CA0030 /* Geometry Accelerator State Restore Register (RW) */
#define B3_GA_GLEDGEFLAG                  0x00CA0400 /* Used To Send Edge Flag (RW) */
#define B3_GA_GLEND                       0x00CA0800 /* Geometry Accelerator glEnd Register (RW) */
#define B3_GA_GLNORMAL3                   0x00CA2080 /* Used To Send Nx, Ny, Nz (RW) */
#define B3_GA_GLCOLOR3                    0x00CA2100 /* Used To Send R, G, B (RW) */
#define B3_GA_GLVERTEX3                   0x00CA2200 /* Used To Send X, Y, Z (RW) */
#define B3_GA_GLTEXCOORD4                 0x00CA3040 /* Used To Send Texture  S, T,R, Q (RW) */
#define B3_GA_GLTEXCOORD2                 0x00CA1040 /* Used To Send Texture  S, T (RW) */
#define B3_GA_GLCOLOR4                    0x00CA3100 /* Used To Send R, G, B, A (RW) */
#define B3_GA_GLVERTEX4                   0x00CA3200 /* Used To Send X, Y, Z, W (RW) */
#define B3_GA_CLIP_STATUS                 0x00CA4008 /* Geometry Accelerator Clip Status Register (RW) */
#define B3_GA_TRI_COUNT                   0x00CA4010 /* Geometry Accelerator Triangle Count Register (RW) */
#define B3_GA_LINE_COUNT                  0x00CA4018 /* Geometry Accelerator Line Count Register (RW) */
#define B3_GA_POINT_COUNT                 0x00CA4020 /* Geometry Accelerator Point Count Register (RW) */
#define B3_GA_VERT_COUNT                  0x00CA4028 /* Geometry Accelerator Vertex Count Register (RW) */
#define B3_GA_NOP_EOC                     0x00CA8000 /* Not Really Used This Way (RW) */
#define B3_GA_CPLL                        0x00CB0000 /* Geometry Accelerator PLL Control Register (RW) */
#define B3_GA_RENDER_MODE                 0x00CB0008 /* Geometry Accelerator Render Mode Register (RW) */
#define B3_GA_LIGHTING_MODE1              0x00CB0010 /* Geometry Accelerator Lighting Mode 1 Register (RW) */
#define B3_GA_LIGHTING_MODE2              0x00CB0018 /* Geometry Accelerator Lighting Mode 2 Register (RW) */
#define B3_GA_GLBEGIN                     0x00CB0020 /* Geometry Accelerator glBegin Register (RW) */
#define B3_GA_INPUT_STATUS                0x00CB0028 /* Geometry Accelerator Input Status Register (RW) */
#define B3_GA_CONFIG                      0x00CB0038 /* Geometry Accelerator Configuration Register (RW) */
#define B3_GA_LIGHTING_MODE3              0x00CB4000 /* Geometry Accelerator Lighting Mode 3 Register (RW) */
#define B3_GA_IPLL                        0x00CB4030 /* Geometry Accelerator I/O PLL Control Register (RW) */
/*  End of Geometry Accelerator Registers B  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Float to Fix Conversion Registers  */
/*  Start of Plane Equation Registers  */
#define B3_FTF_TM_S_Start                 0x00CC0000 /* TM_S_Start (floating point format) (W) */

#define B3_FTF_TM_T_Start                 0x00CC0008 /* TM_T_Start (floating point format) (W) */

#define B3_FTF_TM_R_Start                 0x00CC0010 /* TM_R_Start (floating point format) (W) */

#define B3_FTF_TM_G_Start                 0x00CC0018 /* TM_G_Start (floating point format) (W) */

#define B3_FTF_TM_GB_Start                0x00CC0020 /* TM_GB_Start (floating point format) (W) */

#define B3_FTF_TM_W_Start                 0x00CC0028 /* TM_W_Start (floating point format) (W) */

#define B3_FTF_dTM_S_dX                   0x00CC0030 /* dTM_S_dX (floating point format) (W) */

#define B3_FTF_dTM_T_dX                   0x00CC0038 /* dTM_T_dX (floating point format) (W) */

#define B3_FTF_dTM_R_dX                   0x00CC0040 /* dTM_R_dX (floating point format) (W) */

#define B3_FTF_dTM_G_dX                   0x00CC0048 /* dTM_G_dX (floating point format) (W) */

#define B3_FTF_dTM_GB_dX                  0x00CC0050 /* dTM_GB_dX (floating point format) (W) */

#define B3_FTF_dTM_W_dX                   0x00CC0058 /* dTM_W_dX (floating point format) (W) */

#define B3_FTF_dTM_S_dE1                  0x00CC0060 /* dTM_S_dE1 (floating point format) (W) */

#define B3_FTF_dTM_T_dE1                  0x00CC0068 /* dTM_T_dE1 (floating point format) (W) */

#define B3_FTF_dTM_R_dE1                  0x00CC0070 /* dTM_R_dE1 (floating point format) (W) */

#define B3_FTF_dTM_G_dE1                  0x00CC0078 /* dTM_G_dE1 (floating point format) (W) */

#define B3_FTF_dTM_GB_dE1                 0x00CC0080 /* dTM_GB_dE1 (floating point format) (W) */

#define B3_FTF_dTM_W_dE1                  0x00CC0088 /* dTM_W_dE1 (floating point format) (W) */

#define B3_FTF_dTM_S_dY                   0x00CC0090 /* dTM_S_dY (floating point format) (W) */

#define B3_FTF_dTM_T_dY                   0x00CC0098 /* dTM_T_dY (floating point format) (W) */

#define B3_FTF_dTM_R_dY                   0x00CC00A0 /* dTM_R_dY (floating point format) (W) */

#define B3_FTF_dTM_G_dY                   0x00CC00A8 /* dTM_G_dY (floating point format) (W) */

#define B3_FTF_dTM_GB_dY                  0x00CC00B0 /* dTM_GB_dY (floating point format) (W) */

#define B3_FTF_dTM_W_dY                   0x00CC00B8 /* dTM_W_dY (floating point format) (W) */

#define B3_FTF_Edge1_X_Start              0x00CC00C0 /* Edge1_X_Start/Minor_X_Start/Point_X_Start (float) (W) */
#define B3_FTF_Edge2_X_Start              0x00CC00C4 /* Edge2_X_Start (floating point format) (W) */
#define B3_FTF_Edge3_X_Start              0x00CC00C8 /* Edge3_X_Start (floating point format) (W) */
#define B3_FTF_Major_X_Start              0x00CC00CC /* - / Major_X_Start (floating point format) (W) */
#define B3_FTF_Major_X_Stop               0x00CC00D0 /* - / Major_X_Stop (floating point format) (W) */



#define B3_FTF_Edge1_Y_Start              0x00CC00E0 /* Edge1_Y_Start/Major_Y_Start/Point_Y_Start (float) (W) */
#define B3_FTF_Edge1_Y_Stop               0x00CC00E4 /* Edge1_Y_Stop / Major_Y_Stop (floating point format) (W) */
#define B3_FTF_Edge3_Y_Start              0x00CC00E8 /* Edge3_Y_Start (floating point format) (W) */
#define B3_FTF_Minor_Y_Start              0x00CC00EC /* - / Minor_Y_Start (floating point format) (W) */




#define B3_FTF_Edge1_Slope                0x00CC0100 /* Edge1_Slope/Minor_Slope/Point_X_Step (float) (W) */
#define B3_FTF_Edge2_Slope                0x00CC0104 /* Edge2_Slope (floating point format) (W) */
#define B3_FTF_Edge3_Slope                0x00CC0108 /* Edge3_Slope / Point_Y_Step (floating point format) (W) */



#define B3_FTF_CMD                        0x00CC0120 /* Command (W) */




#define B3_FTF_R_Start                    0x00CC0140 /* R_Start (floating point format) (W) */
#define B3_FTF_G_Start                    0x00CC0144 /* G_Start (floating point format) (W) */
#define B3_FTF_B_Start                    0x00CC0148 /* B_Start (floating point format) (W) */
#define B3_FTF_Alpha_Start                0x00CC014C /* Alpha_Start (floating point format) (W) */
#define B3_FTF_Rspec_Start                0x00CC0150 /* Rspec_Start (floating point format) (W) */
#define B3_FTF_Gspec_Start                0x00CC0154 /* Gspec_Start (floating point format) (W) */
#define B3_FTF_Bspec_Start                0x00CC0158 /* Bspec_Start (floating point format) (W) */
#define B3_FTF_DepthCue_Start             0x00CC015C /* DepthCue_Start (floating point format) (W) */
#define B3_FTF_dR_dX                      0x00CC0160 /* dR_dX (floating point format) (W) */
#define B3_FTF_dG_dX                      0x00CC0164 /* dG_dX (floating point format) (W) */
#define B3_FTF_dB_dX                      0x00CC0168 /* dB_dX (floating point format) (W) */
#define B3_FTF_dAlpha_dX                  0x00CC016C /* Dalpha_dX (floating point format) (W) */
#define B3_FTF_dRspec_dX                  0x00CC0170 /* dR_dX_Spec (floating point format) (W) */
#define B3_FTF_dGspec_dX                  0x00CC0174 /* dG_dX_Spec (floating point format) (W) */
#define B3_FTF_dBspec_dX                  0x00CC0178 /* dB_dX_Spec (floating point format) (W) */
#define B3_FTF_dDepthCue_dX               0x00CC017C /* DdepthCue_dX (floating point format) (W) */
#define B3_FTF_dR_dE1                     0x00CC0180 /* dR_dE1 (floating point format) (W) */
#define B3_FTF_dG_dE1                     0x00CC0184 /* dG_dE1 (floating point format) (W) */
#define B3_FTF_dB_dE1                     0x00CC0188 /* dB_dE1 (floating point format) (W) */
#define B3_FTF_dAlpha_dE1                 0x00CC018C /* Dalpha_dE1 (floating point format) (W) */
#define B3_FTF_dRspec_dE1                 0x00CC0190 /* dR_dE1_Spec (floating point format) (W) */
#define B3_FTF_dGspec_dE1                 0x00CC0194 /* dG_dE1_Spec (floating point format) (W) */
#define B3_FTF_dBspec_dE1                 0x00CC0198 /* dB_dE1_Spec (floating point format) (W) */
#define B3_FTF_dDepthCue_dE1              0x00CC019C /* DdepthCue_dE1 (floating point format) (W) */
#define B3_FTF_dR_dY                      0x00CC01A0 /* dR_dY (floating point format) (W) */
#define B3_FTF_dG_dY                      0x00CC01A4 /* dG_dY (floating point format) (W) */
#define B3_FTF_dB_dY                      0x00CC01A8 /* dB_dY (floating point format) (W) */
#define B3_FTF_dAlpha_dY                  0x00CC01AC /* Dalpha_dY (floating point format) (W) */
#define B3_FTF_dRspec_dY                  0x00CC01B0 /* dR_dY_Spec (floating point format) (W) */
#define B3_FTF_dGspec_dY                  0x00CC01B4 /* dG_dY_Spec (floating point format) (W) */
#define B3_FTF_dBspec_dY                  0x00CC01B8 /* dB_dY_Spec (floating point format) (W) */
#define B3_FTF_dDepthCue_dY               0x00CC01BC /* DdepthCue_dY (floating point format) (W) */
#define B3_FTF_Z_Start                    0x00CC01C0 /* Z_Start (floating point format) (W) */
#define B3_FTF_dZ_dX                      0x00CC01C4 /* dZ_dX (floating point format) (W) */
#define B3_FTF_dZ_dE1                     0x00CC01C8 /* dZ_dE1 / Z_Slope / Z_Step (floating point format) (W) */
#define B3_FTF_dZ_dY                      0x00CC01CC /* dZ_dY (floating point format) (W) */
#define B3_FTF_FogExp_Start               0x00CC01D0 /* FogExp_Start (floating point format) (W) */
#define B3_FTF_dFogExp_dX                 0x00CC01D4 /* DfogExp_dX (floating point format) (W) */
#define B3_FTF_dFogExp_dE1                0x00CC01D8 /* DfogExp_dE1 (floating point format) (W) */
#define B3_FTF_dFogExp_dY                 0x00CC01DC /* DfogExp_dY (floating point format) (W) */







#define B3_FTF_Z_Ext_Frac                 0x00CC01FC /* Z_Ext_Frac (26.14 fixed pt format) (RW) */
#define B3_CSV                            0x00CC1000 /* Color Scale Value (RW) */
#define B3_AA                             0x00CC1004 /* Anti-Alias Triangle / Polygon FTF Mode Register (RW) */
#define B3_FTF_Point_X_Width              0x00CC0410 /* Point_X_Width Register (floating point format) (W) */
#define B3_FTF_Point_Y_Width              0x00CC0414 /* Point_Y_Width Register (floating point format) (W) */
#define B3_FTF_Minor_Width                0x00CC0418 /* Minor Width Register (floating point format) (W) */
/*  End of Float to Fix Conversion Registers  */

/*  Start of Rasterizer Registers  */
/*  End of Rasterizer Registers  */

/*  Start of Rasterizer Registers  */
/*  End of Rasterizer Registers  */

/*  Start of Rasterizer Registers  */
#define UB_RAST_X_MIN_EXT                 0x00501238 /* The minimum rasterized X value in a scene  (R) */
#define UB_RAST_X_MAX_EXT                 0x0050123C /* The maximum rasterized X value in a scene  (R) */
#define UB_RAST_Y_MIN_EXT                 0x00501240 /* The minimum rasterized Y value in a scene  (R) */
#define UB_RAST_Y_MAX_EXT                 0x00501244 /* The maximum rasterized Y value in a scene  (R) */
#define UB_RAST_Z_MIN_EXT                 0x00501248 /* The minimum rasterized Z value in a scene  (R) */
#define UB_RAST_Z_MAX_EXT                 0x0050124C /* The maximum rasterized Z value in a scene  (R) */
/*  End of Rasterizer Registers  */

/*  Start of Rasterizer Registers  */
#define B2_RAST_Edge1_X_Start             0x009000C0 /* Edge1_X_Start / Minor_X_Start / Point_X_Start (16.16) (RW) */
#define B2_RAST_Edge2_X_Start             0x009000C4 /* Edge2_X_Start (16.16 fixed pt format) (RW) */
#define B2_RAST_Edge3_X_Start             0x009000C8 /* Edge3_X_Start (16.16 fixed pt format) (RW) */
#define B2_RAST_Major_X_Start             0x009000CC /* X major start (16.16) for WORG apply in Heathrow (RW) */
#define B2_RAST_Major_X_Stop              0x009000D0 /* X major stop (16.16) for WORG apply in Heathrow (RW) */

#define B2_RAST_Edge1_Y_Start             0x009000E0 /* Edge1_Y_Start / Major_Y_Start / Point_Y_Start (16.16) (RW) */
#define B2_RAST_Edge1_Y_Stop              0x009000E4 /* Edge1_Y_Stop / Major_Y_Stop (16.16 fixed pt format) (RW) */
#define B2_RAST_Edge3_Y_Start             0x009000E8 /* Edge3_Y_Start (16.16 fixed pt format) (RW) */
#define B2_RAST_Minor_Y_Start             0x009000EC /* Y major start (16.16) for WORG apply in Heathrow (RW) */



#define B2_RAST_Edge1_Slope               0x00900100 /* Edge1_Slope / Minor_Slope / Point_X_Step (16.16) (RW) */
#define B2_RAST_Edge2_Slope               0x00900104 /* Edge2_Slope (16.16 fixed pt format) (RW) */
#define B2_RAST_Edge3_Slope               0x00900108 /* Edge3_Slope / Point_Y_Step (16.16 fixed pt format) (RW) */



#define B2_RAST_CMD                       0x00900120 /* Command (RW) */


#define B2_RAST_ZE                        0x00900138 /* Z Extended bits  (RW) */
#define B2_RAST_CSV                       0x0090013C /* Color Scale Value for R,G, B and Alpha (RW) */
#define B2_RAST_Rd_Start                  0x00900140 /* Rd_Start  (2.30 fixed pt format) (RW) */
#define B2_RAST_Gd_Start                  0x00900144 /* Gd_Start (2.30 fixed pt format) (RW) */
#define B2_RAST_Bd_Start                  0x00900148 /* Bd_Start (2.30 fixed pt format) (RW) */
#define B2_RAST_Alpha_Start               0x0090014C /* Alpha_Start (2.30 fixed pt format) (RW) */
#define B2_RAST_Rs_Start                  0x00900150 /* Rs_Start  (2.30 fixed pt format) (RW) */
#define B2_RAST_Gs_Start                  0x00900154 /* Gs_Start  (2.30 fixed pt format) (RW) */
#define B2_RAST_Bs_Start                  0x00900158 /* Bs_Start  (2.30 fixed pt format) (RW) */
#define B2_RAST_Fog_Start                 0x0090015C /* Fog_Start  (2.30 fixed pt format) (RW) */
#define B2_RAST_dRd_dX                    0x00900160 /* DRd_dX  (2.30 fixed pt format) (RW) */
#define B2_RAST_dGd_dX                    0x00900164 /* DGd_dX  (2.30 fixed pt format) (RW) */
#define B2_RAST_dBd_dX                    0x00900168 /* DBd_dX  (2.30 fixed pt format) (RW) */
#define B2_RAST_dAlpha_dX                 0x0090016C /* DAlpha_dX  (2.30 fixed pt format) (RW) */
#define B2_RAST_dRs_dX                    0x00900170 /* dRs_dX  (2.30 fixed pt format) (RW) */
#define B2_RAST_dGs_dX                    0x00900174 /* DGs_dX  (2.30 fixed pt format) (RW) */
#define B2_RAST_dBs_dX                    0x00900178 /* dBs_dX  (2.30 fixed pt format) (RW) */
#define B2_RAST_dFog_dX                   0x0090017C /* Fog_dX (2.30 fixed pt format) (RW) */
#define B2_RAST_dRd_dE1                   0x00900180 /* DRd_dE1 (2.30 fixed pt format) (RW) */
#define B2_RAST_dGd_dE1                   0x00900184 /* DGd_dE1 (2.30 fixed pt format) (RW) */
#define B2_RAST_dBd_dE1                   0x00900188 /* DBd_dE1 (2.30 fixed pt format) (RW) */
#define B2_RAST_dAlpha_dE1                0x0090018C /* Dalpha_dE1 (2.30 fixed pt format) (RW) */
#define B2_RAST_dRs_dE1                   0x00900190 /* DRs_dE1  (2.30 fixed pt format) (RW) */
#define B2_RAST_dGs_dE1                   0x00900194 /* DGs_dE1  (2.30 fixed pt format) (RW) */
#define B2_RAST_dBs_dE1                   0x00900198 /* DBs_dE1  (2.30 fixed pt format) (RW) */
#define B2_RAST_dFog_dE1                  0x0090019C /* DFog_dE1 (2.30 fixed pt format) (RW) */
#define B2_RAST_Z_Start                   0x009001C0 /* Z_Start (26.14 fixed pt format) (RW) */
#define B2_RAST_dZ_dX                     0x009001C4 /* dZ_dX (26.14 fixed pt format) (RW) */
#define B2_RAST_dZ_dE1                    0x009001C8 /* dZ_dE1 / Z_Slope / Z_Step (26.14 fixed pt format) (RW) */
#define B2_RAST_FogExp_Start              0x009001D0 /* FogExp_Start (2.30 fixed point format) (W) */
#define B2_RAST_dFogExp_dX                0x009001D4 /* DfogExp_dX (2.30 fixed point format) (W) */
#define B2_RAST_dFogExp_dE1               0x009001D8 /* DfogExp_dE1 (2.30 fixed point format) (W) */
#define B2_RAST_RMR                       0x0090040C /* Render Mode Register (RW) */
#define B2_RAST_Point_X_Width             0x00900410 /* Point_X_Width Register (16.16 fixed pt format) (RW) */
#define B2_RAST_Point_Y_Width             0x00900414 /* Point_Y_Width Register (16.16 fixed pt format) (RW) */
#define B2_RAST_Minor_Width               0x00900418 /* Minor Width Register (16.16 fixed pt format) (RW) */
#define B2_RAST_DPSET                     0x0090041C /* Draw Point Setup Register (RW) */
#define B2_RAST_DPSTEP                    0x00900420 /* Draw Point Step Register (RW) */
#define B2_RAST_WORG                      0x00900818 /* The window origin register (RW) */
#define B2_RAST_EN2D                      0x00900858 /* Enable OGL coordinates (W) */
#define B2_LTR                            0x00901200 /* Line Type Register (RW) */
#define B2_LTCCR                          0x00901204 /* Line type count register (RW) */
#define B2_ZMIN                           0x00901208 /* Minimum Z Register (RW) */
#define B2_ZMAX                           0x0090120C /* Maximum Z Register (RW) */
#define B2_RAST_X_MIN_EXT                 0x00901238 /* The minimum rasterized X value in a scene  (RW) */
#define B2_RAST_X_MAX_EXT                 0x0090123C /* The maximum rasterized X value in a scene  (RW) */
#define B2_RAST_Y_MIN_EXT                 0x00901240 /* The minimum rasterized Y value in a scene  (RW) */
#define B2_RAST_Y_MAX_EXT                 0x00901244 /* The maximum rasterized Y value in a scene  (RW) */
#define B2_RAST_Z_MIN_EXT                 0x00901248 /* The minimum rasterized Z value in a scene  (RW) */
#define B2_RAST_Z_MAX_EXT                 0x0090124C /* The maximum rasterized Z value in a scene  (RW) */
#define B2_RAST_S0_Start_Int              0x00902000 /* S0_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_S0_Start_Frac             0x00902004 /* S0_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_T0_Start_Int              0x00902008 /* T0_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_T0_Start_Frac             0x0090200C /* T0_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_S1_Start_Int              0x00902010 /* S1_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_S1_Start_Frac             0x00902014 /* S1_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_T1_Start_Int              0x00902018 /* T1_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_T1_Start_Frac             0x0090201C /* T1_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_S2_Start_Int              0x00902020 /* S2_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_S2_Start_Frac             0x00902024 /* S2_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_T2_Start_Int              0x00902028 /* T2_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_T2_Start_Frac             0x0090202C /* T2_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_S3_Start_Int              0x00902030 /* S3_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_S3_Start_Frac             0x00902034 /* S3_Start_Frac (16.30 fixed pt format) (RW) */
#define B2_RAST_T3_Start_Int              0x00902038 /* T3_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_T3_Start_Frac             0x0090203C /* T3_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_R_Start_Int               0x00902040 /* R_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_R_Start_Frac              0x00902044 /* R_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_Gx0_Start_Int             0x00902048 /* Gx0_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_Gx0_Start_Frac            0x0090204C /* Gx0_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_Gy0_Start_Int             0x00902050 /* Gy0_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_Gy0_Start_Frac            0x00902054 /* Gy0_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_QoW0_Start                0x00902058 /* QoW0_Start  (2.30 fixed pt format) (RW) */
#define B2_RAST_Gx1_Start_Int             0x0090205C /* Gx1_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_Gx1_Start_Frac            0x00902060 /* Gx1_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_Gy1_Start_Int             0x00902064 /* Gy1_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_Gy1_Start_Frac            0x00902068 /* Gy1_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_QoW1_Start                0x0090206C /* QoW1_Start  (2.30 fixed pt format) (RW) */
#define B2_RAST_Gx2_Start_Int             0x00902070 /* Gx2_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_Gx2_Start_Frac            0x00902074 /* Gx2_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_Gy2_Start_Int             0x00902078 /* Gy2_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_Gy2_Start_Frac            0x0090207C /* Gy2_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_QoW2_Start                0x00902080 /* QoW2_Start  (2.30 fixed pt format) (RW) */
#define B2_RAST_Gx3_Start_Int             0x00902084 /* Gx3_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_Gx3_Start_Frac            0x00902088 /* Gx3_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_Gy3_Start_Int             0x0090208C /* Gy3_Start_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_Gy3_Start_Frac            0x00902090 /* Gy3_Start_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_QoW3_Start                0x00902094 /* QoW3_Start  (2.30 fixed pt format) (RW) */
#define B2_RAST_dS0_dX_Int                0x00902098 /* DS0_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dS0_dX_Frac               0x0090209C /* DS0_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT0_dX_Int                0x009020A0 /* DT0_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT0_dX_Frac               0x009020A4 /* DT0_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dS1_dX_Int                0x009020A8 /* DS1_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dS1_dX_Frac               0x009020AC /* DS1_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT1_dX_Int                0x009020B0 /* DT1_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT1_dX_Frac               0x009020B4 /* DT1_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dS2_dX_Int                0x009020B8 /* DS2_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dS2_dX_Frac               0x009020BC /* DS2_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT2_dX_Int                0x009020C0 /* DT2_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT2_dX_Frac               0x009020C4 /* DT2_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dS3_dX_Int                0x009020C8 /* DS3_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dS3_dX_Frac               0x009020CC /* DS3_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT3_dX_Int                0x009020D0 /* DT3_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT3_dX_Frac               0x009020D4 /* DT3_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dR_dX_Int                 0x009020D8 /* DR_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dR_dX_Frac                0x009020DC /* DR_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGx0_dX_Int               0x009020E0 /* DGx0_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGx0_dX_Frac              0x009020E4 /* DGx0_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy0_dX_Int               0x009020E8 /* DGy0_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy0_dX_Frac              0x009020EC /* DGy0_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dQoW0_dX                  0x009020F0 /* DqoW0_dX  (2.30 fixed pt format) (RW) */
#define B2_RAST_dGx1_dX_Int               0x009020F4 /* DGx1_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGx1_dX_Frac              0x009020F8 /* DGx1_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy1_dX_Int               0x009020FC /* DGy1_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy1_dX_Frac              0x00902100 /* DGy1_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dQoW1_dX                  0x00902104 /* DQoW1_dX  (2.30 fixed pt format) (RW) */
#define B2_RAST_dGx2_dX_Int               0x00902108 /* DGx2_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGx2_dX_Frac              0x0090210C /* DGx2_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy2_dX_Int               0x00902114 /* DGy2_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy2_dX_Frac              0x00902118 /* DGy2_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dQoW2_dX                  0x0090211C /* DQoW2_dX  (2.30 fixed pt format) (RW) */
#define B2_RAST_dGx3_dX_Int               0x00902120 /* DGx3_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGx3_dX_Frac              0x00902124 /* DGx3_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy3_dX_Int               0x00902128 /* DGy3_dX_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy3_dX_Frac              0x0090212C /* DGy3_dX_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dQoW3_dX                  0x00902130 /* DQoW3_dX  (2.30 fixed pt format) (RW) */
#define B2_RAST_dS0_dE1_Int               0x00902138 /* DS0_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dS0_dE1_Frac              0x0090213C /* DS0_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT0_dE1_Int               0x00902140 /* DT0_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT0_dE1_Frac              0x00902144 /* DT0_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dS1_dE1_Int               0x00902148 /* DS1_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dS1_dE1_Frac              0x0090214C /* DS1_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT1_dE1_Int               0x00902150 /* DT1_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT1_dE1_Frac              0x00902154 /* DT1_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dS2_dE1_Int               0x00902158 /* DS2_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dS2_dE1_Frac              0x0090215C /* DS2_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT2_dE1_Int               0x00902160 /* DT2_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT2_dE1_Frac              0x00902164 /* DT2_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dS3_dE1_Int               0x00902168 /* DS3_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dS3_dE1_Frac              0x0090216C /* DS3_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT3_dE1_Int               0x00902170 /* DT3_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dT3_dE1_Frac              0x00902174 /* DT3_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dR_dE1_Int                0x00902178 /* DR_dE1_Int   (16.30 fixed pt format) (RW) */
#define B2_RAST_dR_dE1_Frac               0x0090217C /* DR_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGx0_dE1_Int              0x00902180 /* DGx0_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGx0_dE1_Frac             0x00902184 /* DGx0_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy0_dE1_Int              0x00902188 /* DGy0_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy0_dE1_Frac             0x0090218C /* DGy0_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dQoW0_dE1                 0x00902190 /* DQoW0_dE1  (2.30 fixed pt format) (RW) */
#define B2_RAST_dGx1_dE1_Int              0x00902194 /* DGx1_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGx1_dE1_Frac             0x00902198 /* DGx1_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy1_dE1_Int              0x0090219C /* DGy1_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy1_dE1_Frac             0x009021A0 /* DGy1_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dQoW1_dE1                 0x009021A4 /* DqoW1_dE1  (2.30 fixed pt format) (RW) */
#define B2_RAST_dGx2_dE1_Int              0x009021A8 /* DGx2_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGx2_dE1_Frac             0x009021AC /* DGx2_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy2_dE1_Int              0x009021B0 /* DGy2_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy2_dE1_Frac             0x009021B4 /* DGy2_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dQoW2_dE1                 0x009021B8 /* DQoW2_dE1  (2.30 fixed pt format) (RW) */
#define B2_RAST_dGx3_dE1_Int              0x009021BC /* DGx3_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGx3_dE1_Frac             0x009021C0 /* DGx3_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy3_dE1_Int              0x009021C4 /* DGy3_dE1_Int  (16.30 fixed pt format) (RW) */
#define B2_RAST_dGy3_dE1_Frac             0x009021C8 /* DGy3_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B2_RAST_dQoW3_dE1                 0x009021CC /* DQoW3_dE1  (2.30 fixed pt format) (RW) */
/*  End of Rasterizer Registers  */

/*  Start of Rasterizer Registers  */
/*  Start of Plane Equation Registers Used By Both Rasterizer And Texture Mapper  */
#define B3_RAST_Edge1_X_Start             0x00D000C0 /* Edge1_X_Start / Minor_X_Start / Point_X_Start (16.16) (RW) */
#define B3_RAST_Edge2_X_Start             0x00D000C4 /* Edge2_X_Start (16.16 fixed pt format) (RW) */
#define B3_RAST_Edge3_X_Start             0x00D000C8 /* Edge3_X_Start (16.16 fixed pt format) (RW) */
#define B3_RAST_Major_X_Start             0x00D000CC /* X major start (16.16) for WORG apply in Heathrow (RW) */
#define B3_RAST_Major_X_Stop              0x00D000D0 /* X major stop (16.16) for WORG apply in Heathrow (RW) */

#define B3_RAST_Edge1_Y_Start             0x00D000E0 /* Edge1_Y_Start / Major_Y_Start / Point_Y_Start (16.16) (RW) */
#define B3_RAST_Edge1_Y_Stop              0x00D000E4 /* Edge1_Y_Stop / Major_Y_Stop (16.16 fixed pt format) (RW) */
#define B3_RAST_Edge3_Y_Start             0x00D000E8 /* Edge3_Y_Start (16.16 fixed pt format) (RW) */
#define B3_RAST_Minor_Y_Start             0x00D000EC /* Y major start (16.16) for WORG apply in Heathrow (RW) */



#define B3_RAST_Edge1_Slope               0x00D00100 /* Edge1_Slope / Minor_Slope / Point_X_Step (16.16) (RW) */
#define B3_RAST_Edge2_Slope               0x00D00104 /* Edge2_Slope (16.16 fixed pt format) (RW) */
#define B3_RAST_Edge3_Slope               0x00D00108 /* Edge3_Slope / Point_Y_Step (16.16 fixed pt format) (RW) */



#define B3_RAST_CMD                       0x00D00120 /* Command (RW) */


#define B3_RAST_ZE                        0x00D00138 /* Z Extended bits (-) */
#define B3_RAST_CSV                       0x00D0013C /* Color Scale Value for R,G, B and Alpha (RW) */
#define B3_RAST_Rd_Start                  0x00D00140 /* Rd_Start  (2.30 fixed pt format) (RW) */
#define B3_RAST_Gd_Start                  0x00D00144 /* Gd_Start  (2.30 fixed pt format) (RW) */
#define B3_RAST_Bd_Start                  0x00D00148 /* Bd_Start  (2.30 fixed pt format) (RW) */
#define B3_RAST_Alpha_Start               0x00D0014C /* Alpha_Start  (2.30 fixed pt format) (RW) */
#define B3_RAST_Rs_Start                  0x00D00150 /* Rs_Start  (2.30 fixed pt format) (RW) */
#define B3_RAST_Gs_Start                  0x00D00154 /* Gs_Start  (2.30 fixed pt format) (RW) */
#define B3_RAST_Bs_Start                  0x00D00158 /* Bs_Start  (2.30 fixed pt format) (RW) */
#define B3_RAST_Fog_Start                 0x00D0015C /* Fog_Start  (2.30 fixed pt format) (RW) */
#define B3_RAST_dRd_dX                    0x00D00160 /* dRd_dX  (2.30 fixed pt format) (RW) */
#define B3_RAST_dGd_dX                    0x00D00164 /* dGd_dX  (2.30 fixed pt format) (RW) */
#define B3_RAST_dBd_dX                    0x00D00168 /* dBd_dX  (2.30 fixed pt format) (RW) */
#define B3_RAST_dAlpha_dX                 0x00D0016C /* dAlpha_dX (2.30 fixed pt format) (RW) */
#define B3_RAST_dRs_dX                    0x00D00170 /* dRs_dX  (2.30 fixed pt format) (RW) */
#define B3_RAST_dGs_dX                    0x00D00174 /* dGs_dX  (2.30 fixed pt format) (RW) */
#define B3_RAST_dBs_dX                    0x00D00178 /* dBs_dX  (2.30 fixed pt format) (RW) */
#define B3_RAST_dFog_dX                   0x00D0017C /* dFog_dX  (2.30 fixed pt format) (RW) */
#define B3_RAST_dRd_dE1                   0x00D00180 /* dRd_dE1  (2.30 fixed pt format) (RW) */
#define B3_RAST_dGd_dE1                   0x00D00184 /* dGd_dE1  (2.30 fixed pt format) (RW) */
#define B3_RAST_dBd_dE1                   0x00D00188 /* dBd_dE1  (2.30 fixed pt format) (RW) */
#define B3_RAST_dAlpha_dE1                0x00D0018C /* dAlpha_dE1  (2.30 fixed pt format) (RW) */
#define B3_RAST_dRs_dE1                   0x00D00190 /* dRs_dE1  (2.30 fixed pt format) (RW) */
#define B3_RAST_dGs_dE1                   0x00D00194 /* dGs_dE1  (2.30 fixed pt format) (RW) */
#define B3_RAST_dBs_dE1                   0x00D00198 /* dBs_dE1  (2.30 fixed pt format) (RW) */
#define B3_RAST_dFog_dE1                  0x00D0019C /* dFog_dE1 (2.30 fixed pt format) (RW) */
#define B3_RAST_Z_Start                   0x00D001C0 /* Z_Start (26.14 fixed pt format) (RW) */
#define B3_RAST_dZ_dX                     0x00D001C4 /* dZ_dX (26.14 fixed pt format) (RW) */
#define B3_RAST_dZ_dE1                    0x00D001C8 /* dZ_dE1 / Z_Slope / Z_Step (26.14 fixed pt format) (RW) */
#define B3_RAST_FogExp_Start              0x00D001D0 /* FogExp_Start (2.30 fixed point format) (W) */
#define B3_RAST_dFogExp_dX                0x00D001D4 /* DfogExp_dX (2.30 fixed point format) (W) */
#define B3_RAST_dFogExp_dE1               0x00D001D8 /* DfogExp_dE1 (2.30 fixed point format) (W) */







#define B3_RAST_RMR                       0x00D0040C /* Render Mode Register (RW) */
#define B3_RAST_Point_X_Width             0x00D00410 /* Point_X_Width Register (16.16 fixed pt format) (RW) */
#define B3_RAST_Point_Y_Width             0x00D00414 /* Point_Y_Width Register (16.16 fixed pt format) (RW) */
#define B3_RAST_Minor_Width               0x00D00418 /* Minor_Width Register (16.16 fixed pt format) (RW) */
#define B3_RAST_DPSET                     0x00D0041C /* Draw Point Setup Register (RW) */
#define B3_RAST_DPSTEP                    0x00D00420 /* Draw Point Step Register (RW) */
#define B3_RAST_WORG                      0x00D00818 /* The window origin register (RW) */
#define B3_RAST_EN2D                      0x00D00858 /* Enable OGL coordinates (W) */
#define B3_LTR                            0x00D01200 /* Line Type Register (RW) */
#define B3_LTCCR                          0x00D01204 /* Line type count register (RW) */
#define B3_ZMIN                           0x00D01208 /* Minimum Z Register (RW) */
#define B3_ZMAX                           0x00D0120C /* Maximum Z Register (RW) */
#define B3_RAST_X_MIN_EXT                 0x00D01238 /* The minimum rasterized X value in a scene  (RW) */
#define B3_RAST_X_MAX_EXT                 0x00D0123C /* The maximum rasterized X value in a scene  (RW) */
#define B3_RAST_Y_MIN_EXT                 0x00D01240 /* The minimum rasterized Y value in a scene  (RW) */
#define B3_RAST_Y_MAX_EXT                 0x00D01244 /* The maximum rasterized Y value in a scene  (RW) */
#define B3_RAST_Z_MIN_EXT                 0x00D01248 /* The minimum rasterized Z value in a scene  (RW) */
#define B3_RAST_Z_MAX_EXT                 0x00D0124C /* The maximum rasterized Z value in a scene  (RW) */
#define B3_RAST_S0_Start_Int              0x00D02000 /* S0_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_S0_Start_Frac             0x00D02004 /* S0_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_T0_Start_Int              0x00D02008 /* T0_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_T0_Start_Frac             0x00D0200C /* T0_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_S1_Start_Int              0x00D02010 /* S1_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_S1_Start_Frac             0x00D02014 /* S1_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_T1_Start_Int              0x00D02018 /* T1_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_T1_Start_Frac             0x00D0201C /* T1_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_S2_Start_Int              0x00D02020 /* S2_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_S2_Start_Frac             0x00D02024 /* S2_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_T2_Start_Int              0x00D02028 /* T2_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_T2_Start_Frac             0x00D0202C /* T2_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_S3_Start_Int              0x00D02030 /* S3_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_S3_Start_Frac             0x00D02034 /* S3_Start_Frac (16.30 fixed pt format) (RW) */
#define B3_RAST_T3_Start_Int              0x00D02038 /* T3_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_T3_Start_Frac             0x00D0203C /* T3_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_R_Start_Int               0x00D02040 /* R_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_R_Start_Frac              0x00D02044 /* R_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_Gx0_Start_Int             0x00D02048 /* Gx0_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_Gx0_Start_Frac            0x00D0204C /* Gx0_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_Gy0_Start_Int             0x00D02050 /* Gy0_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_Gy0_Start_Frac            0x00D02054 /* Gy0_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_QoW0_Start                0x00D02058 /* QoW0_Start  (2.30 fixed pt format) (RW) */
#define B3_RAST_Gx1_Start_Int             0x00D0205C /* Gx1_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_Gx1_Start_Frac            0x00D02060 /* Gx1_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_Gy1_Start_Int             0x00D02064 /* Gy1_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_Gy1_Start_Frac            0x00D02068 /* Gy1_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_QoW1_Start                0x00D0206C /* QoW1_Start  (2.30 fixed pt format) (RW) */
#define B3_RAST_Gx2_Start_Int             0x00D02070 /* Gx2_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_Gx2_Start_Frac            0x00D02074 /* Gx2_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_Gy2_Start_Int             0x00D02078 /* Gy2_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_Gy2_Start_Frac            0x00D0207C /* Gy2_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_QoW2_Start                0x00D02080 /* QoW2_Start  (2.30 fixed pt format) (RW) */
#define B3_RAST_Gx3_Start_Int             0x00D02084 /* Gx3_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_Gx3_Start_Frac            0x00D02088 /* Gx3_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_Gy3_Start_Int             0x00D0208C /* Gy3_Start_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_Gy3_Start_Frac            0x00D02090 /* Gy3_Start_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_QoW3_Start                0x00D02094 /* QoW3_Start  (2.30 fixed pt format) (RW) */
#define B3_RAST_dS0_dX_Int                0x00D02098 /* DS0_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dS0_dX_Frac               0x00D0209C /* DS0_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT0_dX_Int                0x00D020A0 /* DT0_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT0_dX_Frac               0x00D020A4 /* DT0_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dS1_dX_Int                0x00D020A8 /* DS1_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dS1_dX_Frac               0x00D020AC /* DS1_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT1_dX_Int                0x00D020B0 /* DT1_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT1_dX_Frac               0x00D020B4 /* DT1_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dS2_dX_Int                0x00D020B8 /* DS2_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dS2_dX_Frac               0x00D020BC /* DS2_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT2_dX_Int                0x00D020C0 /* DT2_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT2_dX_Frac               0x00D020C4 /* DT2_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dS3_dX_Int                0x00D020C8 /* DS3_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dS3_dX_Frac               0x00D020CC /* DS3_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT3_dX_Int                0x00D020D0 /* DT3_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT3_dX_Frac               0x00D020D4 /* DT3_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dR_dX_Int                 0x00D020D8 /* DR_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dR_dX_Frac                0x00D020DC /* DR_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGx0_dX_Int               0x00D020E0 /* DGx0_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGx0_dX_Frac              0x00D020E4 /* DGx0_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy0_dX_Int               0x00D020E8 /* DGy0_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy0_dX_Frac              0x00D020EC /* DGy0_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dQoW0_dX                  0x00D020F0 /* DqoW0_dX  (2.30 fixed pt format) (RW) */
#define B3_RAST_dGx1_dX_Int               0x00D020F4 /* DGx1_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGx1_dX_Frac              0x00D020F8 /* DGx1_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy1_dX_Int               0x00D020FC /* DGy1_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy1_dX_Frac              0x00D02100 /* DGy1_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dQoW1_dX                  0x00D02104 /* DQoW1_dX  (2.30 fixed pt format) (RW) */
#define B3_RAST_dGx2_dX_Int               0x00D02108 /* DGx2_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGx2_dX_Frac              0x00D0210C /* DGx2_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy2_dX_Int               0x00D02114 /* DGy2_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy2_dX_Frac              0x00D02118 /* DGy2_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dQoW2_dX                  0x00D0211C /* DQoW2_dX  (2.30 fixed pt format) (RW) */
#define B3_RAST_dGx3_dX_Int               0x00D02120 /* DGx3_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGx3_dX_Frac              0x00D02124 /* DGx3_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy3_dX_Int               0x00D02128 /* DGy3_dX_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy3_dX_Frac              0x00D0212C /* DGy3_dX_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dQoW3_dX                  0x00D02130 /* DQoW3_dX  (2.30 fixed pt format) (RW) */

#define B3_RAST_dS0_dE1_Int               0x00D02138 /* DS0_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dS0_dE1_Frac              0x00D0213C /* DS0_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT0_dE1_Int               0x00D02140 /* DT0_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT0_dE1_Frac              0x00D02144 /* DT0_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dS1_dE1_Int               0x00D02148 /* DS1_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dS1_dE1_Frac              0x00D0214C /* DS1_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT1_dE1_Int               0x00D02150 /* DT1_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT1_dE1_Frac              0x00D02154 /* DT1_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dS2_dE1_Int               0x00D02158 /* DS2_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dS2_dE1_Frac              0x00D0215C /* DS2_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT2_dE1_Int               0x00D02160 /* DT2_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT2_dE1_Frac              0x00D02164 /* DT2_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dS3_dE1_Int               0x00D02168 /* DS3_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dS3_dE1_Frac              0x00D0216C /* DS3_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT3_dE1_Int               0x00D02170 /* DT3_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dT3_dE1_Frac              0x00D02174 /* DT3_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dR_dE1_Int                0x00D02178 /* DR_dE1_Int   (16.30 fixed pt format) (RW) */
#define B3_RAST_dR_dE1_Frac               0x00D0217C /* DR_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGx0_dE1_Int              0x00D02180 /* DGx0_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGx0_dE1_Frac             0x00D02184 /* DGx0_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy0_dE1_Int              0x00D02188 /* DGy0_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy0_dE1_Frac             0x00D0218C /* DGy0_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dQoW0_dE1                 0x00D02190 /* DQoW0_dE1  (2.30 fixed pt format) (RW) */
#define B3_RAST_dGx1_dE1_Int              0x00D02194 /* DGx1_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGx1_dE1_Frac             0x00D02198 /* DGx1_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy1_dE1_Int              0x00D0219C /* DGy1_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy1_dE1_Frac             0x00D021A0 /* DGy1_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dQoW1_dE1                 0x00D021A4 /* DqoW1_dE1  (2.30 fixed pt format) (RW) */
#define B3_RAST_dGx2_dE1_Int              0x00D021A8 /* DGx2_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGx2_dE1_Frac             0x00D021AC /* DGx2_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy2_dE1_Int              0x00D021B0 /* DGy2_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy2_dE1_Frac             0x00D021B4 /* DGy2_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dQoW2_dE1                 0x00D021B8 /* DQoW2_dE1  (2.30 fixed pt format) (RW) */
#define B3_RAST_dGx3_dE1_Int              0x00D021BC /* DGx3_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGx3_dE1_Frac             0x00D021C0 /* DGx3_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy3_dE1_Int              0x00D021C4 /* DGy3_dE1_Int  (16.30 fixed pt format) (RW) */
#define B3_RAST_dGy3_dE1_Frac             0x00D021C8 /* DGy3_dE1_Frac  (16.30 fixed pt format) (RW) */
#define B3_RAST_dQoW3_dE1                 0x00D021CC /* DQoW3_dE1  (2.30 fixed pt format) (RW) */
/*  End of Rasterizer Registers  */

/*  Start of Texture Mapping Registers  */
/*  End of Texture Mapping Registers  */

/*  Start of Texture Mapping Registers  */
/*  End of Texture Mapping Registers  */

/*  Start of Texture Mapping Registers  */
#define UB_TM_TSS                         0x004E5800 /* Texture Subsystem Setup (all registers) (R) */
#define UB_TM_TSS_RLI                     0x004E5000 /* RLI portion of TSS (R) */
#define UB_TM_TSS_C0                      0x004E5400 /* Cache 0 portion of TSS (R) */
#define UB_TM_TSS_C1                      0x004E5404 /* Cache 1 portion of TSS (R) */
#define UB_TM_TSS_C2                      0x004E5408 /* Cache 2 portion of TSS (R) */
#define UB_TM_TSS_C3                      0x004E540c /* Cache 3 portion of TSS (R) */
#define UB_TM_TRO0                        0x004E5810 /* Texture Raster Operation for texture #0 (R) */
#define UB_TM_TRO0_RLI                    0x004E5010 /* RLI portion of TRO0 (R) */
#define UB_TM_TRO0_TILER                  0x004E5210 /* Tiler portion of TRO0 (R) */
#define UB_TM_TRO0_C0                     0x004E5410 /* Cache 0 portion of TRO0 (R) */
#define UB_TM_TRO0_C1                     0x004E5414 /* Cache 1 portion of TRO0 (R) */
#define UB_TM_TRO0_C2                     0x004E5418 /* Cache 2 portion of TRO0 (R) */
#define UB_TM_TRO0_C3                     0x004E541c /* Cache 3 portion of TRO0 (R) */
#define UB_TM_TRO0_INT                    0x004E5610 /* Interpolator portion of TRO0 (R) */
#define UB_TM_TRO1                        0x004E5820 /* Texture Raster Operation for texture #1 (R) */
#define UB_TM_TRO1_RLI                    0x004E5020 /* RLI portion of TRO1 (R) */
#define UB_TM_TRO1_TILER                  0x004E5220 /* Tiler portion of TRO1 (R) */
#define UB_TM_TRO1_C0                     0x004E5420 /* Cache 0 portion of TRO1 (R) */
#define UB_TM_TRO1_C1                     0x004E5424 /* Cache 1 portion of TRO1 (R) */
#define UB_TM_TRO1_C2                     0x004E5428 /* Cache 2 portion of TRO1 (R) */
#define UB_TM_TRO1_C3                     0x004E542c /* Cache 3 portion of TRO1 (R) */
#define UB_TM_TRO1_INT                    0x004E5620 /* Interpolator portion of TRO1 (R) */
#define UB_TM_TRO2                        0x004E5830 /* Texture Raster Operation for texture #2 (R) */
#define UB_TM_TRO2_RLI                    0x004E5030 /* RLI portion of TRO2 (R) */
#define UB_TM_TRO2_TILER                  0x004E5230 /* Tiler portion of TRO2 (R) */
#define UB_TM_TRO2_C0                     0x004E5430 /* Cache 0 portion of TRO2 (R) */
#define UB_TM_TRO2_C1                     0x004E5434 /* Cache 1 portion of TRO2 (R) */
#define UB_TM_TRO2_C2                     0x004E5438 /* Cache 2 portion of TRO2 (R) */
#define UB_TM_TRO2_C3                     0x004E543c /* Cache 3 portion of TRO2 (R) */
#define UB_TM_TRO2_INT                    0x004E5630 /* Interpolator portion of TRO2 (R) */
#define UB_TM_TRO3                        0x004E5840 /* Texture Raster Operation for texture #3 (R) */
#define UB_TM_TRO3_RLI                    0x004E5040 /* RLI portion of TRO3 (R) */
#define UB_TM_TRO3_TILER                  0x004E5240 /* Tiler portion of TRO3 (R) */
#define UB_TM_TRO3_C0                     0x004E5440 /* Cache 0 portion of TRO3 (R) */
#define UB_TM_TRO3_C1                     0x004E5444 /* Cache 1 portion of TRO3 (R) */
#define UB_TM_TRO3_C2                     0x004E5448 /* Cache 2 portion of TRO3 (R) */
#define UB_TM_TRO3_C3                     0x004E544c /* Cache 3 portion of TRO3 (R) */
#define UB_TM_TRO3_INT                    0x004E5640 /* Interpolator portion of TRO3 (R) */
#define UB_TM_TSI0                        0x004E5850 /* Texture Size for texture #0 (R) */
#define UB_TM_TSI0_RLI                    0x004E5050 /* RLI portion of TSI0 (R) */
#define UB_TM_TSI0_TILER                  0x004E5250 /* Tiler portion of TSI0 (R) */
#define UB_TM_TSI1                        0x004E5854 /* Texture Size for texture #1 (R) */
#define UB_TM_TSI1_RLI                    0x004E5054 /* RLI portion of TSI1 (R) */
#define UB_TM_TSI1_TILER                  0x004E5254 /* Tiler portion of TSI1 (R) */
#define UB_TM_TSI2                        0x004E5858 /* Texture Size for texture #2 (R) */
#define UB_TM_TSI2_RLI                    0x004E5058 /* RLI portion of TSI2 (R) */
#define UB_TM_TSI2_TILER                  0x004E5258 /* Tiler portion of TSI2 (R) */
#define UB_TM_TSI3                        0x004E585c /* Texture Size for texture #3 (R) */
#define UB_TM_TSI3_RLI                    0x004E505c /* RLI portion of TSI3 (R) */
#define UB_TM_TSI3_TILER                  0x004E525c /* Tiler portion of TSI3 (R) */
#define UB_TM_FORW_THRESH                 0x004E5290 /* Number of words to send before waiting for LUT return (R) */
#define UB_TM_TLC0                        0x004E5060 /* LOD Clamp for texture #0 (R) */
#define UB_TM_TLC1                        0x004E5064 /* LOD Clamp for texture #1 (R) */
#define UB_TM_TLC2                        0x004E5068 /* LOD Clamp for texture #2 (R) */
#define UB_TM_TLC3                        0x004E506c /* LOD Clamp for texture #3 (R) */
#define UB_TM_TLA0                        0x004E5070 /* LOD Adjust for texture #0 (R) */
#define UB_TM_TLA1                        0x004E5074 /* LOD Adjust for texture #1 (R) */
#define UB_TM_TLA2                        0x004E5078 /* LOD Adjust for texture #2 (R) */
#define UB_TM_TLA3                        0x004E507c /* LOD Adjust for texture #3 (R) */
#define UB_TM_TBC0                        0x004E5650 /* Border Color for texture #0 (R) */
#define UB_TM_TBC1                        0x004E5654 /* Border Color for texture #1 (R) */
#define UB_TM_TBC2                        0x004E5658 /* Border Color for texture #2 (R) */
#define UB_TM_TBC3                        0x004E565c /* Border Color for texture #3 (R) */
#define UB_TM_TOFF0_0                     0x004E5300 /* Texture offset to 1x1 map for texture #0 (R) */
#define UB_TM_TOFF1_0                     0x004E5304 /* Texture offset to 2x2 map for texture #0 (R) */
#define UB_TM_TOFF2_0                     0x004E5308 /* Texture offset to 4x4 map for texture #0 (R) */
#define UB_TM_TOFF3_0                     0x004E530c /* Texture offset to 8x8 map for texture #0 (R) */
#define UB_TM_TOFF4_0                     0x004E5310 /* Texture offset to 16x16 map for texture #0 (R) */
#define UB_TM_TOFF5_0                     0x004E5314 /* Texture offset to 32x32 map for texture #0 (R) */
#define UB_TM_TOFF6_0                     0x004E5318 /* Texture offset to 64x64 map for texture #0 (R) */
#define UB_TM_TOFF7_0                     0x004E531c /* Texture offset to 128x128 map for texture #0 (R) */
#define UB_TM_TOFF8_0                     0x004E5320 /* Texture offset to 256x256 map for texture #0 (R) */
#define UB_TM_TOFF9_0                     0x004E5324 /* Texture offset to 512x512 map for texture #0 (R) */
#define UB_TM_TOFF10_0                    0x004E5328 /* Texture offset to 1024x1024 map for texture #0 (R) */
#define UB_TM_TOFF11_0                    0x004E532c /* Texture offset to 2048x2048 map for texture #0 (R) */
#define UB_TM_TOFF12_0                    0x004E5330 /* Texture offset to 4096x4096 map for texture #0 (R) */
#define UB_TM_TOFF13_0                    0x004E5334 /* Texture offset to 8192x8192 map for texture #0 (R) */
#define UB_TM_TOFF14_0                    0x004E5338 /* Texture offset to 16384x16384 map for texture #0 (R) */
#define UB_TM_TOFF15_0                    0x004E533c /* Texture offset to 32768x32768 map for texture #0 (R) */
#define UB_TM_TOFF0_1                     0x004E5340 /* Texture offset to 1x1 map for texture #1 (R) */
#define UB_TM_TOFF1_1                     0x004E5344 /* Texture offset to 2x2 map for texture #1 (R) */
#define UB_TM_TOFF2_1                     0x004E5348 /* Texture offset to 4x4 map for texture #1 (R) */
#define UB_TM_TOFF3_1                     0x004E534c /* Texture offset to 8x8 map for texture #1 (R) */
#define UB_TM_TOFF4_1                     0x004E5350 /* Texture offset to 16x16 map for texture #1 (R) */
#define UB_TM_TOFF5_1                     0x004E5354 /* Texture offset to 32x32 map for texture #1 (R) */
#define UB_TM_TOFF6_1                     0x004E5358 /* Texture offset to 64x64 map for texture #1 (R) */
#define UB_TM_TOFF7_1                     0x004E535c /* Texture offset to 128x128 map for texture #1 (R) */
#define UB_TM_TOFF8_1                     0x004E5360 /* Texture offset to 256x256 map for texture #1 (R) */
#define UB_TM_TOFF9_1                     0x004E5364 /* Texture offset to 512x512 map for texture #1 (R) */
#define UB_TM_TOFF10_1                    0x004E5368 /* Texture offset to 1024x1024 map for texture #1 (R) */
#define UB_TM_TOFF11_1                    0x004E536c /* Texture offset to 2048x2048 map for texture #1 (R) */
#define UB_TM_TOFF12_1                    0x004E5370 /* Texture offset to 4096x4096 map for texture #1 (R) */
#define UB_TM_TOFF13_1                    0x004E5374 /* Texture offset to 8192x8192 map for texture #1 (R) */
#define UB_TM_TOFF14_1                    0x004E5378 /* Texture offset to 16384x16384 map for texture #1 (R) */
#define UB_TM_TOFF15_1                    0x004E537c /* Texture offset to 32768x32768 map for texture #1 (R) */
#define UB_TM_TOFF0_2                     0x004E5380 /* Texture offset to 1x1 map for texture #2 (R) */
#define UB_TM_TOFF1_2                     0x004E5384 /* Texture offset to 2x2 map for texture #2 (R) */
#define UB_TM_TOFF2_2                     0x004E5388 /* Texture offset to 4x4 map for texture #2 (R) */
#define UB_TM_TOFF3_2                     0x004E538c /* Texture offset to 8x8 map for texture #2 (R) */
#define UB_TM_TOFF4_2                     0x004E5390 /* Texture offset to 16x16 map for texture #2 (R) */
#define UB_TM_TOFF5_2                     0x004E5394 /* Texture offset to 32x32 map for texture #2 (R) */
#define UB_TM_TOFF6_2                     0x004E5398 /* Texture offset to 64x64 map for texture #2 (R) */
#define UB_TM_TOFF7_2                     0x004E539c /* Texture offset to 128x128 map for texture #2 (R) */
#define UB_TM_TOFF8_2                     0x004E53a0 /* Texture offset to 256x256 map for texture #2 (R) */
#define UB_TM_TOFF9_2                     0x004E53a4 /* Texture offset to 512x512 map for texture #2 (R) */
#define UB_TM_TOFF10_2                    0x004E53a8 /* Texture offset to 1024x1024 map for texture #2 (R) */
#define UB_TM_TOFF11_2                    0x004E53ac /* Texture offset to 2048x2048 map for texture #2 (R) */
#define UB_TM_TOFF12_2                    0x004E53b0 /* Texture offset to 4096x4096 map for texture #2 (R) */
#define UB_TM_TOFF13_2                    0x004E53b4 /* Texture offset to 8192x8192 map for texture #2 (R) */
#define UB_TM_TOFF14_2                    0x004E53b8 /* Texture offset to 16384x16384 map for texture #2 (R) */
#define UB_TM_TOFF15_2                    0x004E53bc /* Texture offset to 32768x32768 map for texture #2 (R) */
#define UB_TM_TOFF0_3                     0x004E53c0 /* Texture offset to 1x1 map for texture #3 (R) */
#define UB_TM_TOFF1_3                     0x004E53c4 /* Texture offset to 2x2 map for texture #3 (R) */
#define UB_TM_TOFF2_3                     0x004E53c8 /* Texture offset to 4x4 map for texture #3 (R) */
#define UB_TM_TOFF3_3                     0x004E53cc /* Texture offset to 8x8 map for texture #3 (R) */
#define UB_TM_TOFF4_3                     0x004E53d0 /* Texture offset to 16x16 map for texture #3 (R) */
#define UB_TM_TOFF5_3                     0x004E53d4 /* Texture offset to 32x32 map for texture #3 (R) */
#define UB_TM_TOFF6_3                     0x004E53d8 /* Texture offset to 64x64 map for texture #3 (R) */
#define UB_TM_TOFF7_3                     0x004E53dc /* Texture offset to 128x128 map for texture #3 (R) */
#define UB_TM_TOFF8_3                     0x004E53e0 /* Texture offset to 256x256 map for texture #3 (R) */
#define UB_TM_TOFF9_3                     0x004E53e4 /* Texture offset to 512x512 map for texture #3 (R) */
#define UB_TM_TOFF10_3                    0x004E53e8 /* Texture offset to 1024x1024 map for texture #3 (R) */
#define UB_TM_TOFF11_3                    0x004E53ec /* Texture offset to 2048x2048 map for texture #3 (R) */
#define UB_TM_TOFF12_3                    0x004E53f0 /* Texture offset to 4096x4096 map for texture #3 (R) */
#define UB_TM_TOFF13_3                    0x004E53f4 /* Texture offset to 8192x8192 map for texture #3 (R) */
#define UB_TM_TOFF14_3                    0x004E53f8 /* Texture offset to 16384x16384 map for texture #3 (R) */
#define UB_TM_TOFF15_3                    0x004E53fc /* Texture offset to 32768x32768 map for texture #3 (R) */
#define UB_TM_TBOS0                       0x004E5260 /* Texture offset to S border map for texture #0 (R) */
#define UB_TM_TBOS1                       0x004E5264 /* Texture offset to S border map for texture #1 (R) */
#define UB_TM_TBOS2                       0x004E5268 /* Texture offset to S border map for texture #2 (R) */
#define UB_TM_TBOS3                       0x004E526c /* Texture offset to S border map for texture #3 (R) */
#define UB_TM_TBOT0                       0x004E5270 /* Texture offset to T border map for texture #0 (R) */
#define UB_TM_TBOT1                       0x004E5274 /* Texture offset to T border map for texture #1 (R) */
#define UB_TM_TBOT2                       0x004E5278 /* Texture offset to T border map for texture #2 (R) */
#define UB_TM_TBOT3                       0x004E527c /* Texture offset to T border map for texture #3 (R) */
#define UB_TM_TBOC0                       0x004E5280 /* Texture offset to Corner border map for texture #0 (R) */
#define UB_TM_TBOC1                       0x004E5284 /* Texture offset to Corner border map for texture #1 (R) */
#define UB_TM_TBOC2                       0x004E5288 /* Texture offset to Corner border map for texture #2 (R) */
#define UB_TM_TBOC3                       0x004E528c /* Texture offset to Corner border map for texture #3 (R) */
#define UB_TM_TLOL0                       0x004E52c0 /* Texture offset to Luminance LUT map for texture #0 (R) */
#define UB_TM_TLOL1                       0x004E52c4 /* Texture offset to Luminance LUT map for texture #1 (R) */
#define UB_TM_TLOL2                       0x004E52c8 /* Texture offset to Luminance LUT map for texture #2 (R) */
#define UB_TM_TLOL3                       0x004E52cc /* Texture offset to Luminance LUT map for texture #3 (R) */
#define UB_TM_TLOA0                       0x004E52d0 /* Texture offset to Alpha LUT map for texture #0 (R) */
#define UB_TM_TLOA1                       0x004E52d4 /* Texture offset to Alpha LUT map for texture #1 (R) */
#define UB_TM_TLOA2                       0x004E52d8 /* Texture offset to Alpha LUT map for texture #2 (R) */
#define UB_TM_TLOA3                       0x004E52dc /* Texture offset to Alpha LUT map for texture #3 (R) */
#define UB_TM_ST_OFFSET                   0x004E52e0 /* Offset to add to S & T for clipped textures (R) */
#define UB_TM_TCD                         0x004E5860 /* Texture cache De-reference (W) */
#define UB_TM_TCD_C0                      0x004E5460 /* Cache 0 portion of TCD (-) */
#define UB_TM_TCD_C1                      0x004E5464 /* Cache 1 portion of TCD (-) */
#define UB_TM_TCD_C2                      0x004E5468 /* Cache 2 portion of TCD (-) */
#define UB_TM_TCD_C3                      0x004E546c /* Cache 3 portion of TCD (-) */
#define UB_TM_TCT                         0x004E5870 /* Texture Current Tag (-) */
#define UB_TM_TCT_C0                      0x004E5470 /* Cache 0 portion of TCT (R) */
#define UB_TM_TCT_C1                      0x004E5474 /* Cache 1 portion of TCT (R) */
#define UB_TM_TCT_C2                      0x004E5478 /* Cache 2 portion of TCT (R) */
#define UB_TM_TCT_C3                      0x004E547c /* Cache 3 portion of TCT (R) */
#define UB_TM_TDT                         0x004E5880 /* Texture Directory Tag (RW) */
#define UB_TM_TDT_C0                      0x004E5480 /* Cache 0 portion of TDT (-) */
#define UB_TM_TDT_C1                      0x004E5484 /* Cache 1 portion of TDT (-) */
#define UB_TM_TDT_C2                      0x004E5488 /* Cache 2 portion of TDT (-) */
#define UB_TM_TDT_C3                      0x004E548c /* Cache 3 portion of TDT (-) */
#define UB_TM_TDAW                        0x004E5890 /* Texture Directory Access for Writes  (RW) */
#define UB_TM_TDAW_C0                     0x004E5490 /* Cache 0 portion of TDAW (R) */
#define UB_TM_TDAW_C1                     0x004E5494 /* Cache 1 portion of TDAW (R) */
#define UB_TM_TDAW_C2                     0x004E5498 /* Cache 2 portion of TDAW (R) */
#define UB_TM_TDAW_C3                     0x004E549c /* Cache 3 portion of TDAW (R) */
#define UB_TM_TDAR                        0x004E58A0 /* Texture Directory Access for Reads (RW) */
#define UB_TM_TDAR_C0                     0x004E54A0 /* Cache 0 portion of TDAR (R) */
#define UB_TM_TDAR_C1                     0x004E54A4 /* Cache 1 portion of TDAR (R) */
#define UB_TM_TDAR_C2                     0x004E54A8 /* Cache 2 portion of TDAR (R) */
#define UB_TM_TDAR_C3                     0x004E54Ac /* Cache 3 portion of TDAR (R) */
#define UB_TM_TIM0                        0x004E58B0 /* Texture Index Mask for bit 0 (R) */
#define UB_TM_TIM0_C0                     0x004E54B0 /* Cache 0 portion of TIM0 (R) */
#define UB_TM_TIM0_C1                     0x004E54B4 /* Cache 1 portion of TIM0 (R) */
#define UB_TM_TIM0_C2                     0x004E54B8 /* Cache 2 portion of TIM0 (R) */
#define UB_TM_TIM0_C3                     0x004E54Bc /* Cache 3 portion of TIM0 (R) */
#define UB_TM_TIM1                        0x004E58C0 /* Texture Index Mask for bit 1 (R) */
#define UB_TM_TIM1_C0                     0x004E54C0 /* Cache 0 portion of TIM1 (R) */
#define UB_TM_TIM1_C1                     0x004E54C4 /* Cache 1 portion of TIM1 (R) */
#define UB_TM_TIM1_C2                     0x004E54C8 /* Cache 2 portion of TIM1 (R) */
#define UB_TM_TIM1_C3                     0x004E54Cc /* Cache 3 portion of TIM1 (R) */
#define UB_TM_TIM2                        0x004E58D0 /* Texture Index Mask for bit 2 (R) */
#define UB_TM_TIM2_C0                     0x004E54D0 /* Cache 0 portion of TIM2 (R) */
#define UB_TM_TIM2_C1                     0x004E54D4 /* Cache 1 portion of TIM2 (R) */
#define UB_TM_TIM2_C2                     0x004E54D8 /* Cache 2 portion of TIM2 (R) */
#define UB_TM_TIM2_C3                     0x004E54Dc /* Cache 3 portion of TIM2 (R) */
#define UB_TM_TIM3                        0x004E58E0 /* Texture Index Mask for bit 3 (R) */
#define UB_TM_TIM3_C0                     0x004E54E0 /* Cache 0 portion of TIM3 (R) */
#define UB_TM_TIM3_C1                     0x004E54E4 /* Cache 1 portion of TIM3 (R) */
#define UB_TM_TIM3_C2                     0x004E54E8 /* Cache 2 portion of TIM3 (R) */
#define UB_TM_TIM3_C3                     0x004E54Ec /* Cache 3 portion of TIM3 (R) */
#define UB_TM_TBSY                        0x004E5080 /* Texture subsystem Busy (R) */
#define UB_TM_TWF                         0x004E5084 /* Texture subsystem wipe and flush (RW) */
/*  End of Texture Mapping Registers  */

/*  Start of Texture Mapping Registers  */
#define B2_TM_TSS                         0x008E5800 /* Texture Subsystem Setup (all registers) (RW) */
#define B2_TM_TSS_RLI                     0x008E5000 /* RLI portion of TSS (RW) */
#define B2_TM_TSS_C0                      0x008E5400 /* Cache 0 portion of TSS (RW) */
#define B2_TM_TSS_C1                      0x008E5404 /* Cache 1 portion of TSS (RW) */
#define B2_TM_TSS_C2                      0x008E5408 /* Cache 2 portion of TSS (RW) */
#define B2_TM_TSS_C3                      0x008E540c /* Cache 3 portion of TSS (RW) */
#define B2_TM_TRO0                        0x008E5810 /* Texture Raster Operation for texture #0 (RW) */
#define B2_TM_TRO0_RLI                    0x008E5010 /* RLI portion of TRO0 (RW) */
#define B2_TM_TRO0_TILER                  0x008E5210 /* Tiler portion of TRO0 (RW) */
#define B2_TM_TRO0_C0                     0x008E5410 /* Cache 0 portion of TRO0 (RW) */
#define B2_TM_TRO0_C1                     0x008E5414 /* Cache 1 portion of TRO0 (RW) */
#define B2_TM_TRO0_C2                     0x008E5418 /* Cache 2 portion of TRO0 (RW) */
#define B2_TM_TRO0_C3                     0x008E541c /* Cache 3 portion of TRO0 (RW) */
#define B2_TM_TRO0_INT                    0x008E5610 /* Interpolator portion of TRO0 (RW) */
#define B2_TM_TRO1                        0x008E5820 /* Texture Raster Operation for texture #1 (RW) */
#define B2_TM_TRO1_RLI                    0x008E5020 /* RLI portion of TRO1 (RW) */
#define B2_TM_TRO1_TILER                  0x008E5220 /* Tiler portion of TRO1 (RW) */
#define B2_TM_TRO1_C0                     0x008E5420 /* Cache 0 portion of TRO1 (RW) */
#define B2_TM_TRO1_C1                     0x008E5424 /* Cache 1 portion of TRO1 (RW) */
#define B2_TM_TRO1_C2                     0x008E5428 /* Cache 2 portion of TRO1 (RW) */
#define B2_TM_TRO1_C3                     0x008E542c /* Cache 3 portion of TRO1 (RW) */
#define B2_TM_TRO1_INT                    0x008E5620 /* Interpolator portion of TRO1 (RW) */
#define B2_TM_TRO2                        0x008E5830 /* Texture Raster Operation for texture #2 (RW) */
#define B2_TM_TRO2_RLI                    0x008E5030 /* RLI portion of TRO2 (RW) */
#define B2_TM_TRO2_TILER                  0x008E5230 /* Tiler portion of TRO2 (RW) */
#define B2_TM_TRO2_C0                     0x008E5430 /* Cache 0 portion of TRO2 (RW) */
#define B2_TM_TRO2_C1                     0x008E5434 /* Cache 1 portion of TRO2 (RW) */
#define B2_TM_TRO2_C2                     0x008E5438 /* Cache 2 portion of TRO2 (RW) */
#define B2_TM_TRO2_C3                     0x008E543c /* Cache 3 portion of TRO2 (RW) */
#define B2_TM_TRO2_INT                    0x008E5630 /* Interpolator portion of TRO2 (RW) */
#define B2_TM_TRO3                        0x008E5840 /* Texture Raster Operation for texture #3 (RW) */
#define B2_TM_TRO3_RLI                    0x008E5040 /* RLI portion of TRO3 (RW) */
#define B2_TM_TRO3_TILER                  0x008E5240 /* Tiler portion of TRO3 (RW) */
#define B2_TM_TRO3_C0                     0x008E5440 /* Cache 0 portion of TRO3 (RW) */
#define B2_TM_TRO3_C1                     0x008E5444 /* Cache 1 portion of TRO3 (RW) */
#define B2_TM_TRO3_C2                     0x008E5448 /* Cache 2 portion of TRO3 (RW) */
#define B2_TM_TRO3_C3                     0x008E544c /* Cache 3 portion of TRO3 (RW) */
#define B2_TM_TRO3_INT                    0x008E5640 /* Interpolator portion of TRO3 (RW) */
#define B2_TM_TSI0                        0x008E5850 /* Texture Size for texture #0 (RW) */
#define B2_TM_TSI0_RLI                    0x008E5050 /* RLI portion of TSI0 (RW) */
#define B2_TM_TSI0_TILER                  0x008E5250 /* Tiler portion of TSI0 (RW) */
#define B2_TM_TSI1                        0x008E5854 /* Texture Size for texture #1 (RW) */
#define B2_TM_TSI1_RLI                    0x008E5054 /* RLI portion of TSI1 (RW) */
#define B2_TM_TSI1_TILER                  0x008E5254 /* Tiler portion of TSI1 (RW) */
#define B2_TM_TSI2                        0x008E5858 /* Texture Size for texture #2 (RW) */
#define B2_TM_TSI2_RLI                    0x008E5058 /* RLI portion of TSI2 (RW) */
#define B2_TM_TSI2_TILER                  0x008E5258 /* Tiler portion of TSI2 (RW) */
#define B2_TM_TSI3                        0x008E585c /* Texture Size for texture #3 (RW) */
#define B2_TM_TSI3_RLI                    0x008E505c /* RLI portion of TSI3 (RW) */
#define B2_TM_TSI3_TILER                  0x008E525c /* Tiler portion of TSI3 (RW) */
#define B2_TM_FORW_THRESH                 0x008E5290 /* Number of words to send before waiting for LUT return (RW) */
#define B2_TM_TLC0                        0x008E5060 /* LOD Clamp for texture #0 (RW) */
#define B2_TM_TLC1                        0x008E5064 /* LOD Clamp for texture #1 (RW) */
#define B2_TM_TLC2                        0x008E5068 /* LOD Clamp for texture #2 (RW) */
#define B2_TM_TLC3                        0x008E506c /* LOD Clamp for texture #3 (RW) */
#define B2_TM_TLA0                        0x008E5070 /* LOD Adjust for texture #0 (RW) */
#define B2_TM_TLA1                        0x008E5074 /* LOD Adjust for texture #1 (RW) */
#define B2_TM_TLA2                        0x008E5078 /* LOD Adjust for texture #2 (RW) */
#define B2_TM_TLA3                        0x008E507c /* LOD Adjust for texture #3 (RW) */
#define B2_TM_TBC0                        0x008E5650 /* Border Color for texture #0 (RW) */
#define B2_TM_TBC1                        0x008E5654 /* Border Color for texture #1 (RW) */
#define B2_TM_TBC2                        0x008E5658 /* Border Color for texture #2 (RW) */
#define B2_TM_TBC3                        0x008E565c /* Border Color for texture #3 (RW) */
#define B2_TM_TOFF0_0                     0x008E5300 /* Texture offset to 1x1 map for texture #0 (RW) */
#define B2_TM_TOFF1_0                     0x008E5304 /* Texture offset to 2x2 map for texture #0 (RW) */
#define B2_TM_TOFF2_0                     0x008E5308 /* Texture offset to 4x4 map for texture #0 (RW) */
#define B2_TM_TOFF3_0                     0x008E530c /* Texture offset to 8x8 map for texture #0 (RW) */
#define B2_TM_TOFF4_0                     0x008E5310 /* Texture offset to 16x16 map for texture #0 (RW) */
#define B2_TM_TOFF5_0                     0x008E5314 /* Texture offset to 32x32 map for texture #0 (RW) */
#define B2_TM_TOFF6_0                     0x008E5318 /* Texture offset to 64x64 map for texture #0 (RW) */
#define B2_TM_TOFF7_0                     0x008E531c /* Texture offset to 128x128 map for texture #0 (RW) */
#define B2_TM_TOFF8_0                     0x008E5320 /* Texture offset to 256x256 map for texture #0 (RW) */
#define B2_TM_TOFF9_0                     0x008E5324 /* Texture offset to 512x512 map for texture #0 (RW) */
#define B2_TM_TOFF10_0                    0x008E5328 /* Texture offset to 1024x1024 map for texture #0 (RW) */
#define B2_TM_TOFF11_0                    0x008E532c /* Texture offset to 2048x2048 map for texture #0 (RW) */
#define B2_TM_TOFF12_0                    0x008E5330 /* Texture offset to 4096x4096 map for texture #0 (RW) */
#define B2_TM_TOFF13_0                    0x008E5334 /* Texture offset to 8192x8192 map for texture #0 (RW) */
#define B2_TM_TOFF14_0                    0x008E5338 /* Texture offset to 16384x16384 map for texture #0 (RW) */
#define B2_TM_TOFF15_0                    0x008E533c /* Texture offset to 32768x32768 map for texture #0 (RW) */
#define B2_TM_TOFF0_1                     0x008e5340 /* Texture offset to 1x1 map for texture #1 (RW) */
#define B2_TM_TOFF1_1                     0x008e5344 /* Texture offset to 2x2 map for texture #1 (RW) */
#define B2_TM_TOFF2_1                     0x008e5348 /* Texture offset to 4x4 map for texture #1 (RW) */
#define B2_TM_TOFF3_1                     0x008e534c /* Texture offset to 8x8 map for texture #1 (RW) */
#define B2_TM_TOFF4_1                     0x008e5350 /* Texture offset to 16x16 map for texture #1 (RW) */
#define B2_TM_TOFF5_1                     0x008e5354 /* Texture offset to 32x32 map for texture #1 (RW) */
#define B2_TM_TOFF6_1                     0x008e5358 /* Texture offset to 64x64 map for texture #1 (RW) */
#define B2_TM_TOFF7_1                     0x008e535c /* Texture offset to 128x128 map for texture #1 (RW) */
#define B2_TM_TOFF8_1                     0x008e5360 /* Texture offset to 256x256 map for texture #1 (RW) */
#define B2_TM_TOFF9_1                     0x008e5364 /* Texture offset to 512x512 map for texture #1 (RW) */
#define B2_TM_TOFF10_1                    0x008e5368 /* Texture offset to 1024x1024 map for texture #1 (RW) */
#define B2_TM_TOFF11_1                    0x008e536c /* Texture offset to 2048x2048 map for texture #1 (RW) */
#define B2_TM_TOFF12_1                    0x008e5370 /* Texture offset to 4096x4096 map for texture #1 (RW) */
#define B2_TM_TOFF13_1                    0x008e5374 /* Texture offset to 8192x8192 map for texture #1 (RW) */
#define B2_TM_TOFF14_1                    0x008e5378 /* Texture offset to 16384x16384 map for texture #1 (RW) */
#define B2_TM_TOFF15_1                    0x008e537c /* Texture offset to 32768x32768 map for texture #1 (RW) */
#define B2_TM_TOFF0_2                     0x008e5380 /* Texture offset to 1x1 map for texture #2 (RW) */
#define B2_TM_TOFF1_2                     0x008e5384 /* Texture offset to 2x2 map for texture #2 (RW) */
#define B2_TM_TOFF2_2                     0x008e5388 /* Texture offset to 4x4 map for texture #2 (RW) */
#define B2_TM_TOFF3_2                     0x008e538c /* Texture offset to 8x8 map for texture #2 (RW) */
#define B2_TM_TOFF4_2                     0x008e5390 /* Texture offset to 16x16 map for texture #2 (RW) */
#define B2_TM_TOFF5_2                     0x008e5394 /* Texture offset to 32x32 map for texture #2 (RW) */
#define B2_TM_TOFF6_2                     0x008e5398 /* Texture offset to 64x64 map for texture #2 (RW) */
#define B2_TM_TOFF7_2                     0x008e539c /* Texture offset to 128x128 map for texture #2 (RW) */
#define B2_TM_TOFF8_2                     0x008e53a0 /* Texture offset to 256x256 map for texture #2 (RW) */
#define B2_TM_TOFF9_2                     0x008e53a4 /* Texture offset to 512x512 map for texture #2 (RW) */
#define B2_TM_TOFF10_2                    0x008e53a8 /* Texture offset to 1024x1024 map for texture #2 (RW) */
#define B2_TM_TOFF11_2                    0x008e53ac /* Texture offset to 2048x2048 map for texture #2 (RW) */
#define B2_TM_TOFF12_2                    0x008e53b0 /* Texture offset to 4096x4096 map for texture #2 (RW) */
#define B2_TM_TOFF13_2                    0x008e53b4 /* Texture offset to 8192x8192 map for texture #2 (RW) */
#define B2_TM_TOFF14_2                    0x008e53b8 /* Texture offset to 16384x16384 map for texture #2 (RW) */
#define B2_TM_TOFF15_2                    0x008e53bc /* Texture offset to 32768x32768 map for texture #2 (RW) */
#define B2_TM_TOFF0_3                     0x008e53c0 /* Texture offset to 1x1 map for texture #3 (RW) */
#define B2_TM_TOFF1_3                     0x008e53c4 /* Texture offset to 2x2 map for texture #3 (RW) */
#define B2_TM_TOFF2_3                     0x008e53c8 /* Texture offset to 4x4 map for texture #3 (RW) */
#define B2_TM_TOFF3_3                     0x008e53cc /* Texture offset to 8x8 map for texture #3 (RW) */
#define B2_TM_TOFF4_3                     0x008e53d0 /* Texture offset to 16x16 map for texture #3 (RW) */
#define B2_TM_TOFF5_3                     0x008e53d4 /* Texture offset to 32x32 map for texture #3 (RW) */
#define B2_TM_TOFF6_3                     0x008e53d8 /* Texture offset to 64x64 map for texture #3 (RW) */
#define B2_TM_TOFF7_3                     0x008e53dc /* Texture offset to 128x128 map for texture #3 (RW) */
#define B2_TM_TOFF8_3                     0x008e53e0 /* Texture offset to 256x256 map for texture #3 (RW) */
#define B2_TM_TOFF9_3                     0x008e53e4 /* Texture offset to 512x512 map for texture #3 (RW) */
#define B2_TM_TOFF10_3                    0x008e53e8 /* Texture offset to 1024x1024 map for texture #3 (RW) */
#define B2_TM_TOFF11_3                    0x008e53ec /* Texture offset to 2048x2048 map for texture #3 (RW) */
#define B2_TM_TOFF12_3                    0x008e53f0 /* Texture offset to 4096x4096 map for texture #3 (RW) */
#define B2_TM_TOFF13_3                    0x008e53f4 /* Texture offset to 8192x8192 map for texture #3 (RW) */
#define B2_TM_TOFF14_3                    0x008e53f8 /* Texture offset to 16384x16384 map for texture #3 (RW) */
#define B2_TM_TOFF15_3                    0x008e53fc /* Texture offset to 32768x32768 map for texture #3 (RW) */
#define B2_TM_TBOS0                       0x008e5260 /* Texture offset to S border map for texture #0 (RW) */
#define B2_TM_TBOS1                       0x008e5264 /* Texture offset to S border map for texture #1 (RW) */
#define B2_TM_TBOS2                       0x008e5268 /* Texture offset to S border map for texture #2 (RW) */
#define B2_TM_TBOS3                       0x008e526c /* Texture offset to S border map for texture #3 (RW) */
#define B2_TM_TBOT0                       0x008e5270 /* Texture offset to T border map for texture #0 (RW) */
#define B2_TM_TBOT1                       0x008e5274 /* Texture offset to T border map for texture #1 (RW) */
#define B2_TM_TBOT2                       0x008e5278 /* Texture offset to T border map for texture #2 (RW) */
#define B2_TM_TBOT3                       0x008e527c /* Texture offset to T border map for texture #3 (RW) */
#define B2_TM_TBOC0                       0x008e5280 /* Texture offset to Corner border map for texture #0 (RW) */
#define B2_TM_TBOC1                       0x008e5284 /* Texture offset to Corner border map for texture #1 (RW) */
#define B2_TM_TBOC2                       0x008e5288 /* Texture offset to Corner border map for texture #2 (RW) */
#define B2_TM_TBOC3                       0x008e528c /* Texture offset to Corner border map for texture #3 (RW) */
#define B2_TM_TLOL0                       0x008e52c0 /* Texture offset to Luminance LUT map for texture #0 (RW) */
#define B2_TM_TLOL1                       0x008e52c4 /* Texture offset to Luminance LUT map for texture #1 (RW) */
#define B2_TM_TLOL2                       0x008e52c8 /* Texture offset to Luminance LUT map for texture #2 (RW) */
#define B2_TM_TLOL3                       0x008e52cc /* Texture offset to Luminance LUT map for texture #3 (RW) */
#define B2_TM_TLOA0                       0x008e52d0 /* Texture offset to Alpha LUT map for texture #0 (RW) */
#define B2_TM_TLOA1                       0x008e52d4 /* Texture offset to Alpha LUT map for texture #1 (RW) */
#define B2_TM_TLOA2                       0x008e52d8 /* Texture offset to Alpha LUT map for texture #2 (RW) */
#define B2_TM_TLOA3                       0x008e52dc /* Texture offset to Alpha LUT map for texture #3 (RW) */
#define B2_TM_ST_OFFSET                   0x008e52e0 /* Offset to add to S & T for clipped textures (RW) */
#define B2_TM_TCD                         0x008e5860 /* Texture cache De-reference (W) */
#define B2_TM_TCD_C0                      0x008e5460 /* Cache 0 portion of TCD (-) */
#define B2_TM_TCD_C1                      0x008e5464 /* Cache 1 portion of TCD (-) */
#define B2_TM_TCD_C2                      0x008e5468 /* Cache 2 portion of TCD (-) */
#define B2_TM_TCD_C3                      0x008e546c /* Cache 3 portion of TCD (-) */
#define B2_TM_TCT                         0x008e5870 /* Texture Current Tag (-) */
#define B2_TM_TCT_C0                      0x008e5470 /* Cache 0 portion of TCT (-) */
#define B2_TM_TCT_C1                      0x008e5474 /* Cache 1 portion of TCT (-) */
#define B2_TM_TCT_C2                      0x008e5478 /* Cache 2 portion of TCT (-) */
#define B2_TM_TCT_C3                      0x008e547c /* Cache 3 portion of TCT (-) */
#define B2_TM_TDT                         0x008e5880 /* Texture Directory Tag (-) */
#define B2_TM_TDT_C0                      0x008e5480 /* Cache 0 portion of TDT (-) */
#define B2_TM_TDT_C1                      0x008e5484 /* Cache 1 portion of TDT (-) */
#define B2_TM_TDT_C2                      0x008e5488 /* Cache 2 portion of TDT (-) */
#define B2_TM_TDT_C3                      0x008e548c /* Cache 3 portion of TDT (-) */
#define B2_TM_TDAW                        0x008e5890 /* Texture Directory Access for Writes (-) */
#define B2_TM_TDAW_C0                     0x008e5490 /* Cache 0 portion of TDAW (-) */
#define B2_TM_TDAW_C1                     0x008e5494 /* Cache 1 portion of TDAW (-) */
#define B2_TM_TDAW_C2                     0x008e5498 /* Cache 2 portion of TDAW (-) */
#define B2_TM_TDAW_C3                     0x008e549c /* Cache 3 portion of TDAW (-) */
#define B2_TM_TDAR                        0x008e58A0 /* Texture Directory Access for Reads (-) */
#define B2_TM_TDAR_C0                     0x008e54A0 /* Cache 0 portion of TDAR (-) */
#define B2_TM_TDAR_C1                     0x008e54A4 /* Cache 1 portion of TDAR (-) */
#define B2_TM_TDAR_C2                     0x008e54A8 /* Cache 2 portion of TDAR (-) */
#define B2_TM_TDAR_C3                     0x008e54Ac /* Cache 3 portion of TDAR (-) */
#define B2_TM_TIM0                        0x008e58B0 /* Texture Index Mask for bit 0 (RW) */
#define B2_TM_TIM0_C0                     0x008e54B0 /* Cache 0 portion of TIM0 (RW) */
#define B2_TM_TIM0_C1                     0x008e54B4 /* Cache 1 portion of TIM0 (RW) */
#define B2_TM_TIM0_C2                     0x008e54B8 /* Cache 2 portion of TIM0 (RW) */
#define B2_TM_TIM0_C3                     0x008e54Bc /* Cache 3 portion of TIM0 (RW) */
#define B2_TM_TIM1                        0x008e58C0 /* Texture Index Mask for bit 1 (RW) */
#define B2_TM_TIM1_C0                     0x008e54C0 /* Cache 0 portion of TIM1 (RW) */
#define B2_TM_TIM1_C1                     0x008e54C4 /* Cache 1 portion of TIM1 (RW) */
#define B2_TM_TIM1_C2                     0x008e54C8 /* Cache 2 portion of TIM1 (RW) */
#define B2_TM_TIM1_C3                     0x008e54Cc /* Cache 3 portion of TIM1 (RW) */
#define B2_TM_TIM2                        0x008e58D0 /* Texture Index Mask for bit 2 (RW) */
#define B2_TM_TIM2_C0                     0x008e54D0 /* Cache 0 portion of TIM2 (RW) */
#define B2_TM_TIM2_C1                     0x008e54D4 /* Cache 1 portion of TIM2 (RW) */
#define B2_TM_TIM2_C2                     0x008e54D8 /* Cache 2 portion of TIM2 (RW) */
#define B2_TM_TIM2_C3                     0x008e54Dc /* Cache 3 portion of TIM2 (RW) */
#define B2_TM_TIM3                        0x008e58E0 /* Texture Index Mask for bit 3 (RW) */
#define B2_TM_TIM3_C0                     0x008e54E0 /* Cache 0 portion of TIM3 (RW) */
#define B2_TM_TIM3_C1                     0x008e54E4 /* Cache 1 portion of TIM3 (RW) */
#define B2_TM_TIM3_C2                     0x008e54E8 /* Cache 2 portion of TIM3 (RW) */
#define B2_TM_TIM3_C3                     0x008e54Ec /* Cache 3 portion of TIM3 (RW) */
#define B2_TM_TBSY                        0x008e5080 /* Texture subsystem Busy (R) */
#define B2_TM_TWF                         0x008e5084 /* Texture subsystem wipe and flush (RW) */
/*  End of Texture Mapping Registers  */

/*  Start of Texture Mapping Registers  */
#define B3_TM_TSS                         0x00ce5800 /* Texture Subsystem Setup (all registers) (RW) */
#define B3_TM_TSS_RLI                     0x00ce5000 /* RLI portion of TSS (RW) */
#define B3_TM_TSS_C0                      0x00ce5400 /* Cache 0 portion of TSS (RW) */
#define B3_TM_TSS_C1                      0x00ce5404 /* Cache 1 portion of TSS (RW) */
#define B3_TM_TSS_C2                      0x00ce5408 /* Cache 2 portion of TSS (RW) */
#define B3_TM_TSS_C3                      0x00ce540c /* Cache 3 portion of TSS (RW) */
#define B3_TM_TRO0                        0x00ce5810 /* Texture Raster Operation for texture #0 (RW) */
#define B3_TM_TRO0_RLI                    0x00ce5010 /* RLI portion of TRO0 (RW) */
#define B3_TM_TRO0_TILER                  0x00ce5210 /* Tiler portion of TRO0 (RW) */
#define B3_TM_TRO0_C0                     0x00ce5410 /* Cache 0 portion of TRO0 (RW) */
#define B3_TM_TRO0_C1                     0x00ce5414 /* Cache 1 portion of TRO0 (RW) */
#define B3_TM_TRO0_C2                     0x00ce5418 /* Cache 2 portion of TRO0 (RW) */
#define B3_TM_TRO0_C3                     0x00ce541c /* Cache 3 portion of TRO0 (RW) */
#define B3_TM_TRO0_INT                    0x00ce5610 /* Interpolator portion of TRO0 (RW) */
#define B3_TM_TRO1                        0x00ce5820 /* Texture Raster Operation for texture #1 (RW) */
#define B3_TM_TRO1_RLI                    0x00ce5020 /* RLI portion of TRO1 (RW) */
#define B3_TM_TRO1_TILER                  0x00ce5220 /* Tiler portion of TRO1 (RW) */
#define B3_TM_TRO1_C0                     0x00ce5420 /* Cache 0 portion of TRO1 (RW) */
#define B3_TM_TRO1_C1                     0x00ce5424 /* Cache 1 portion of TRO1 (RW) */
#define B3_TM_TRO1_C2                     0x00ce5428 /* Cache 2 portion of TRO1 (RW) */
#define B3_TM_TRO1_C3                     0x00ce542c /* Cache 3 portion of TRO1 (RW) */
#define B3_TM_TRO1_INT                    0x00ce5620 /* Interpolator portion of TRO1 (RW) */
#define B3_TM_TRO2                        0x00ce5830 /* Texture Raster Operation for texture #2 (RW) */
#define B3_TM_TRO2_RLI                    0x00ce5030 /* RLI portion of TRO2 (RW) */
#define B3_TM_TRO2_TILER                  0x00ce5230 /* Tiler portion of TRO2 (RW) */
#define B3_TM_TRO2_C0                     0x00ce5430 /* Cache 0 portion of TRO2 (RW) */
#define B3_TM_TRO2_C1                     0x00ce5434 /* Cache 1 portion of TRO2 (RW) */
#define B3_TM_TRO2_C2                     0x00ce5438 /* Cache 2 portion of TRO2 (RW) */
#define B3_TM_TRO2_C3                     0x00ce543c /* Cache 3 portion of TRO2 (RW) */
#define B3_TM_TRO2_INT                    0x00ce5630 /* Interpolator portion of TRO2 (RW) */
#define B3_TM_TRO3                        0x00ce5840 /* Texture Raster Operation for texture #3 (RW) */
#define B3_TM_TRO3_RLI                    0x00ce5040 /* RLI portion of TRO3 (RW) */
#define B3_TM_TRO3_TILER                  0x00ce5240 /* Tiler portion of TRO3 (RW) */
#define B3_TM_TRO3_C0                     0x00ce5440 /* Cache 0 portion of TRO3 (RW) */
#define B3_TM_TRO3_C1                     0x00ce5444 /* Cache 1 portion of TRO3 (RW) */
#define B3_TM_TRO3_C2                     0x00ce5448 /* Cache 2 portion of TRO3 (RW) */
#define B3_TM_TRO3_C3                     0x00ce544c /* Cache 3 portion of TRO3 (RW) */
#define B3_TM_TRO3_INT                    0x00ce5640 /* Interpolator portion of TRO3 (RW) */
#define B3_TM_TSI0                        0x00ce5850 /* Texture Size for texture #0 (RW) */
#define B3_TM_TSI0_RLI                    0x00ce5050 /* RLI portion of TSI0 (RW) */
#define B3_TM_TSI0_TILER                  0x00ce5250 /* Tiler portion of TSI0 (RW) */
#define B3_TM_TSI1                        0x00ce5854 /* Texture Size for texture #1 (RW) */
#define B3_TM_TSI1_RLI                    0x00ce5054 /* RLI portion of TSI1 (RW) */
#define B3_TM_TSI1_TILER                  0x00ce5254 /* Tiler portion of TSI1 (RW) */
#define B3_TM_TSI2                        0x00ce5858 /* Texture Size for texture #2 (RW) */
#define B3_TM_TSI2_RLI                    0x00ce5058 /* RLI portion of TSI2 (RW) */
#define B3_TM_TSI2_TILER                  0x00ce5258 /* Tiler portion of TSI2 (RW) */
#define B3_TM_TSI3                        0x00ce585c /* Texture Size for texture #3 (RW) */
#define B3_TM_TSI3_RLI                    0x00ce505c /* RLI portion of TSI3 (RW) */
#define B3_TM_TSI3_TILER                  0x00ce525c /* Tiler portion of TSI3 (RW) */
#define B3_TM_FORW_THRESH                 0x00CE5290 /* Number of words to send before waiting for LUT return (RW) */
#define B3_TM_TLC0                        0x00ce5060 /* LOD Clamp for texture #0 (RW) */
#define B3_TM_TLC1                        0x00ce5064 /* LOD Clamp for texture #1 (RW) */
#define B3_TM_TLC2                        0x00ce5068 /* LOD Clamp for texture #2 (RW) */
#define B3_TM_TLC3                        0x00ce506c /* LOD Clamp for texture #3 (RW) */
#define B3_TM_TLA0                        0x00ce5070 /* LOD Adjust for texture #0 (RW) */
#define B3_TM_TLA1                        0x00ce5074 /* LOD Adjust for texture #1 (RW) */
#define B3_TM_TLA2                        0x00ce5078 /* LOD Adjust for texture #2 (RW) */
#define B3_TM_TLA3                        0x00ce507c /* LOD Adjust for texture #3 (RW) */
#define B3_TM_TBC0                        0x00ce5650 /* Border Color for texture #0 (RW) */
#define B3_TM_TBC1                        0x00ce5654 /* Border Color for texture #1 (RW) */
#define B3_TM_TBC2                        0x00ce5658 /* Border Color for texture #2 (RW) */
#define B3_TM_TBC3                        0x00ce565c /* Border Color for texture #3 (RW) */
#define B3_TM_TOFF0_0                     0x00ce5300 /* Texture offset to 1x1 map for texture #0 (RW) */
#define B3_TM_TOFF1_0                     0x00ce5304 /* Texture offset to 2x2 map for texture #0 (RW) */
#define B3_TM_TOFF2_0                     0x00ce5308 /* Texture offset to 4x4 map for texture #0 (RW) */
#define B3_TM_TOFF3_0                     0x00ce530c /* Texture offset to 8x8 map for texture #0 (RW) */
#define B3_TM_TOFF4_0                     0x00ce5310 /* Texture offset to 16x16 map for texture #0 (RW) */
#define B3_TM_TOFF5_0                     0x00ce5314 /* Texture offset to 32x32 map for texture #0 (RW) */
#define B3_TM_TOFF6_0                     0x00ce5318 /* Texture offset to 64x64 map for texture #0 (RW) */
#define B3_TM_TOFF7_0                     0x00ce531c /* Texture offset to 128x128 map for texture #0 (RW) */
#define B3_TM_TOFF8_0                     0x00ce5320 /* Texture offset to 256x256 map for texture #0 (RW) */
#define B3_TM_TOFF9_0                     0x00ce5324 /* Texture offset to 512x512 map for texture #0 (RW) */
#define B3_TM_TOFF10_0                    0x00ce5328 /* Texture offset to 1024x1024 map for texture #0 (RW) */
#define B3_TM_TOFF11_0                    0x00ce532c /* Texture offset to 2048x2048 map for texture #0 (RW) */
#define B3_TM_TOFF12_0                    0x00ce5330 /* Texture offset to 4096x4096 map for texture #0 (RW) */
#define B3_TM_TOFF13_0                    0x00ce5334 /* Texture offset to 8192x8192 map for texture #0 (RW) */
#define B3_TM_TOFF14_0                    0x00ce5338 /* Texture offset to 16384x16384 map for texture #0 (RW) */
#define B3_TM_TOFF15_0                    0x00ce533c /* Texture offset to 32768x32768 map for texture #0 (RW) */
#define B3_TM_TOFF0_1                     0x00ce5340 /* Texture offset to 1x1 map for texture #1 (RW) */
#define B3_TM_TOFF1_1                     0x00ce5344 /* Texture offset to 2x2 map for texture #1 (RW) */
#define B3_TM_TOFF2_1                     0x00ce5348 /* Texture offset to 4x4 map for texture #1 (RW) */
#define B3_TM_TOFF3_1                     0x00ce534c /* Texture offset to 8x8 map for texture #1 (RW) */
#define B3_TM_TOFF4_1                     0x00ce5350 /* Texture offset to 16x16 map for texture #1 (RW) */
#define B3_TM_TOFF5_1                     0x00ce5354 /* Texture offset to 32x32 map for texture #1 (RW) */
#define B3_TM_TOFF6_1                     0x00ce5358 /* Texture offset to 64x64 map for texture #1 (RW) */
#define B3_TM_TOFF7_1                     0x00ce535c /* Texture offset to 128x128 map for texture #1 (RW) */
#define B3_TM_TOFF8_1                     0x00ce5360 /* Texture offset to 256x256 map for texture #1 (RW) */
#define B3_TM_TOFF9_1                     0x00ce5364 /* Texture offset to 512x512 map for texture #1 (RW) */
#define B3_TM_TOFF10_1                    0x00ce5368 /* Texture offset to 1024x1024 map for texture #1 (RW) */
#define B3_TM_TOFF11_1                    0x00ce536c /* Texture offset to 2048x2048 map for texture #1 (RW) */
#define B3_TM_TOFF12_1                    0x00ce5370 /* Texture offset to 4096x4096 map for texture #1 (RW) */
#define B3_TM_TOFF13_1                    0x00ce5374 /* Texture offset to 8192x8192 map for texture #1 (RW) */
#define B3_TM_TOFF14_1                    0x00ce5378 /* Texture offset to 16384x16384 map for texture #1 (RW) */
#define B3_TM_TOFF15_1                    0x00ce537c /* Texture offset to 32768x32768 map for texture #1 (RW) */
#define B3_TM_TOFF0_2                     0x00ce5380 /* Texture offset to 1x1 map for texture #2 (RW) */
#define B3_TM_TOFF1_2                     0x00ce5384 /* Texture offset to 2x2 map for texture #2 (RW) */
#define B3_TM_TOFF2_2                     0x00ce5388 /* Texture offset to 4x4 map for texture #2 (RW) */
#define B3_TM_TOFF3_2                     0x00ce538c /* Texture offset to 8x8 map for texture #2 (RW) */
#define B3_TM_TOFF4_2                     0x00ce5390 /* Texture offset to 16x16 map for texture #2 (RW) */
#define B3_TM_TOFF5_2                     0x00ce5394 /* Texture offset to 32x32 map for texture #2 (RW) */
#define B3_TM_TOFF6_2                     0x00ce5398 /* Texture offset to 64x64 map for texture #2 (RW) */
#define B3_TM_TOFF7_2                     0x00ce539c /* Texture offset to 128x128 map for texture #2 (RW) */
#define B3_TM_TOFF8_2                     0x00ce53a0 /* Texture offset to 256x256 map for texture #2 (RW) */
#define B3_TM_TOFF9_2                     0x00ce53a4 /* Texture offset to 512x512 map for texture #2 (RW) */
#define B3_TM_TOFF10_2                    0x00ce53a8 /* Texture offset to 1024x1024 map for texture #2 (RW) */
#define B3_TM_TOFF11_2                    0x00ce53ac /* Texture offset to 2048x2048 map for texture #2 (RW) */
#define B3_TM_TOFF12_2                    0x00ce53b0 /* Texture offset to 4096x4096 map for texture #2 (RW) */
#define B3_TM_TOFF13_2                    0x00ce53b4 /* Texture offset to 8192x8192 map for texture #2 (RW) */
#define B3_TM_TOFF14_2                    0x00ce53b8 /* Texture offset to 16384x16384 map for texture #2 (RW) */
#define B3_TM_TOFF15_2                    0x00ce53bc /* Texture offset to 32768x32768 map for texture #2 (RW) */
#define B3_TM_TOFF0_3                     0x00ce53c0 /* Texture offset to 1x1 map for texture #3 (RW) */
#define B3_TM_TOFF1_3                     0x00ce53c4 /* Texture offset to 2x2 map for texture #3 (RW) */
#define B3_TM_TOFF2_3                     0x00ce53c8 /* Texture offset to 4x4 map for texture #3 (RW) */
#define B3_TM_TOFF3_3                     0x00ce53cc /* Texture offset to 8x8 map for texture #3 (RW) */
#define B3_TM_TOFF4_3                     0x00ce53d0 /* Texture offset to 16x16 map for texture #3 (RW) */
#define B3_TM_TOFF5_3                     0x00ce53d4 /* Texture offset to 32x32 map for texture #3 (RW) */
#define B3_TM_TOFF6_3                     0x00ce53d8 /* Texture offset to 64x64 map for texture #3 (RW) */
#define B3_TM_TOFF7_3                     0x00ce53dc /* Texture offset to 128x128 map for texture #3 (RW) */
#define B3_TM_TOFF8_3                     0x00ce53e0 /* Texture offset to 256x256 map for texture #3 (RW) */
#define B3_TM_TOFF9_3                     0x00ce53e4 /* Texture offset to 512x512 map for texture #3 (RW) */
#define B3_TM_TOFF10_3                    0x00ce53e8 /* Texture offset to 1024x1024 map for texture #3 (RW) */
#define B3_TM_TOFF11_3                    0x00ce53ec /* Texture offset to 2048x2048 map for texture #3 (RW) */
#define B3_TM_TOFF12_3                    0x00ce53f0 /* Texture offset to 4096x4096 map for texture #3 (RW) */
#define B3_TM_TOFF13_3                    0x00ce53f4 /* Texture offset to 8192x8192 map for texture #3 (RW) */
#define B3_TM_TOFF14_3                    0x00ce53f8 /* Texture offset to 16384x16384 map for texture #3 (RW) */
#define B3_TM_TOFF15_3                    0x00ce53fc /* Texture offset to 32768x32768 map for texture #3 (RW) */
#define B3_TM_TBOS0                       0x00ce5260 /* Texture offset to S border map for texture #0 (RW) */
#define B3_TM_TBOS1                       0x00ce5264 /* Texture offset to S border map for texture #1 (RW) */
#define B3_TM_TBOS2                       0x00ce5268 /* Texture offset to S border map for texture #2 (RW) */
#define B3_TM_TBOS3                       0x00ce526c /* Texture offset to S border map for texture #3 (RW) */
#define B3_TM_TBOT0                       0x00ce5270 /* Texture offset to T border map for texture #0 (RW) */
#define B3_TM_TBOT1                       0x00ce5274 /* Texture offset to T border map for texture #1 (RW) */
#define B3_TM_TBOT2                       0x00ce5278 /* Texture offset to T border map for texture #2 (RW) */
#define B3_TM_TBOT3                       0x00ce527c /* Texture offset to T border map for texture #3 (RW) */
#define B3_TM_TBOC0                       0x00ce5280 /* Texture offset to Corner border map for texture #0 (RW) */
#define B3_TM_TBOC1                       0x00ce5284 /* Texture offset to Corner border map for texture #1 (RW) */
#define B3_TM_TBOC2                       0x00ce5288 /* Texture offset to Corner border map for texture #2 (RW) */
#define B3_TM_TBOC3                       0x00ce528c /* Texture offset to Corner border map for texture #3 (RW) */
#define B3_TM_TLOL0                       0x00ce52c0 /* Texture offset to Luminance LUT map for texture #0 (RW) */
#define B3_TM_TLOL1                       0x00ce52c4 /* Texture offset to Luminance LUT map for texture #1 (RW) */
#define B3_TM_TLOL2                       0x00ce52c8 /* Texture offset to Luminance LUT map for texture #2 (RW) */
#define B3_TM_TLOL3                       0x00ce52cc /* Texture offset to Luminance LUT map for texture #3 (RW) */
#define B3_TM_TLOA0                       0x00ce52d0 /* Texture offset to Alpha LUT map for texture #0 (RW) */
#define B3_TM_TLOA1                       0x00ce52d4 /* Texture offset to Alpha LUT map for texture #1 (RW) */
#define B3_TM_TLOA2                       0x00ce52d8 /* Texture offset to Alpha LUT map for texture #2 (RW) */
#define B3_TM_TLOA3                       0x00ce52dc /* Texture offset to Alpha LUT map for texture #3 (RW) */
#define B3_TM_ST_OFFSET                   0x00ce52e0 /* Offset to add to S & T for clipped textures (RW) */
#define B3_TM_TCD                         0x00ce5860 /* Texture cache De-reference (W) */
#define B3_TM_TCD_C0                      0x00ce5460 /* Cache 0 portion of TCD (-) */
#define B3_TM_TCD_C1                      0x00ce5464 /* Cache 1 portion of TCD (-) */
#define B3_TM_TCD_C2                      0x00ce5468 /* Cache 2 portion of TCD (-) */
#define B3_TM_TCD_C3                      0x00ce546c /* Cache 3 portion of TCD (-) */
#define B3_TM_TCT                         0x00ce5870 /* Texture Current Tag (-) */
#define B3_TM_TCT_C0                      0x00ce5470 /* Cache 0 portion of TCT (-) */
#define B3_TM_TCT_C1                      0x00ce5474 /* Cache 1 portion of TCT (-) */
#define B3_TM_TCT_C2                      0x00ce5478 /* Cache 2 portion of TCT (-) */
#define B3_TM_TCT_C3                      0x00ce547c /* Cache 3 portion of TCT (-) */
#define B3_TM_TDT                         0x00ce5880 /* Texture Directory Tag (-) */
#define B3_TM_TDT_C0                      0x00ce5480 /* Cache 0 portion of TDT (-) */
#define B3_TM_TDT_C1                      0x00ce5484 /* Cache 1 portion of TDT (-) */
#define B3_TM_TDT_C2                      0x00ce5488 /* Cache 2 portion of TDT (-) */
#define B3_TM_TDT_C3                      0x00ce548c /* Cache 3 portion of TDT (-) */
#define B3_TM_TDAW                        0x00ce5890 /* Texture Directory Access for Writes (-) */
#define B3_TM_TDAW_C0                     0x00ce5490 /* Cache 0 portion of TDAW (-) */
#define B3_TM_TDAW_C1                     0x00ce5494 /* Cache 1 portion of TDAW (-) */
#define B3_TM_TDAW_C2                     0x00ce5498 /* Cache 2 portion of TDAW (-) */
#define B3_TM_TDAW_C3                     0x00ce549c /* Cache 3 portion of TDAW (-) */
#define B3_TM_TDAR                        0x00ce58A0 /* Texture Directory Access for Reads (-) */
#define B3_TM_TDAR_C0                     0x00ce54A0 /* Cache 0 portion of TDAR (-) */
#define B3_TM_TDAR_C1                     0x00ce54A4 /* Cache 1 portion of TDAR (-) */
#define B3_TM_TDAR_C2                     0x00ce54A8 /* Cache 2 portion of TDAR (-) */
#define B3_TM_TDAR_C3                     0x00ce54Ac /* Cache 3 portion of TDAR (-) */
#define B3_TM_TIM0                        0x00ce58B0 /* Texture Index Mask for bit 0 (RW) */
#define B3_TM_TIM0_C0                     0x00ce54B0 /* Cache 0 portion of TIM0 (RW) */
#define B3_TM_TIM0_C1                     0x00ce54B4 /* Cache 1 portion of TIM0 (RW) */
#define B3_TM_TIM0_C2                     0x00ce54B8 /* Cache 2 portion of TIM0 (RW) */
#define B3_TM_TIM0_C3                     0x00ce54Bc /* Cache 3 portion of TIM0 (RW) */
#define B3_TM_TIM1                        0x00ce58C0 /* Texture Index Mask for bit 1 (RW) */
#define B3_TM_TIM1_C0                     0x00ce54C0 /* Cache 0 portion of TIM1 (RW) */
#define B3_TM_TIM1_C1                     0x00ce54C4 /* Cache 1 portion of TIM1 (RW) */
#define B3_TM_TIM1_C2                     0x00ce54C8 /* Cache 2 portion of TIM1 (RW) */
#define B3_TM_TIM1_C3                     0x00ce54Cc /* Cache 3 portion of TIM1 (RW) */
#define B3_TM_TIM2                        0x00ce58D0 /* Texture Index Mask for bit 2 (RW) */
#define B3_TM_TIM2_C0                     0x00ce54D0 /* Cache 0 portion of TIM2 (RW) */
#define B3_TM_TIM2_C1                     0x00ce54D4 /* Cache 1 portion of TIM2 (RW) */
#define B3_TM_TIM2_C2                     0x00ce54D8 /* Cache 2 portion of TIM2 (RW) */
#define B3_TM_TIM2_C3                     0x00ce54Dc /* Cache 3 portion of TIM2 (RW) */
#define B3_TM_TIM3                        0x00ce58E0 /* Texture Index Mask for bit 3 (RW) */
#define B3_TM_TIM3_C0                     0x00ce54E0 /* Cache 0 portion of TIM3 (RW) */
#define B3_TM_TIM3_C1                     0x00ce54E4 /* Cache 1 portion of TIM3 (RW) */
#define B3_TM_TIM3_C2                     0x00ce54E8 /* Cache 2 portion of TIM3 (RW) */
#define B3_TM_TIM3_C3                     0x00ce54Ec /* Cache 3 portion of TIM3 (RW) */
#define B3_TM_TBSY                        0x00ce5080 /* Texture subsystem Busy (R) */
#define B3_TM_TWF                         0x00ce5084 /* Texture subsystem wipe and flush (RW) */
/*  End of Texture Mapping Registers  */

/*  Start of FBC Registers  */
/*  End of FBC Registers  */

/*  Start of FBC Registers  */
/*  End of FBC Registers  */

/*  Start of FBC Registers  */
#define UB_FBC_FOE                        0x00520404 /* Fragment Operation Enable Register (R) */
#define UB_FBC_TFO                        0x00520408 /* Texture Fragment Operation Register (R) */
#define UB_MTB0                           0x00520420 /* Multi-Texture Blend 0 Register (R) */
#define UB_MTB1                           0x00520424 /* Multi-Texture Blend 1 Register (R) */
#define UB_MTB2                           0x00520428 /* Multi-Texture Blend 2 Register (R) */
#define UB_MTB3                           0x0052042C /* Multi-Texture Blend 3 Register (R) */
/*  Start of Reserved for Future Multi-Texture Blend Regs  */
/*  End of Reserved for Future Multi-Texture Blend Regs  */

#define UB_STP0                           0x00520480 /* Stipple Register 0 (R) */
#define UB_STP1                           0x00520484 /* Stipple Register 1 (R) */
#define UB_STP2                           0x00520488 /* Stipple Register 2 (R) */
#define UB_STP3                           0x0052048C /* Stipple Register 3 (R) */
#define UB_STP4                           0x00520490 /* Stipple Register 4 (R) */
#define UB_STP5                           0x00520494 /* Stipple Register 5 (R) */
#define UB_STP6                           0x00520498 /* Stipple Register 6 (R) */
#define UB_STP7                           0x0052049C /* Stipple Register 7 (R) */
#define UB_STP8                           0x005204A0 /* Stipple Register 8 (R) */
#define UB_STP9                           0x005204A4 /* Stipple Register 9 (R) */
#define UB_STP10                          0x005204A8 /* Stipple Register 10 (R) */
#define UB_STP11                          0x005204AC /* Stipple Register 11 (R) */
#define UB_STP12                          0x005204B0 /* Stipple Register 12 (R) */
#define UB_STP13                          0x005204B4 /* Stipple Register 13 (R) */
#define UB_STP14                          0x005204B8 /* Stipple Register 14 (R) */
#define UB_STP15                          0x005204BC /* Stipple Register 15 (R) */
#define UB_STP16                          0x005204C0 /* Stipple Register 16 (R) */
#define UB_STP17                          0x005204C4 /* Stipple Register 17 (R) */
#define UB_STP18                          0x005204C8 /* Stipple Register 18 (R) */
#define UB_STP19                          0x005204CC /* Stipple Register 19 (R) */
#define UB_STP20                          0x005204D0 /* Stipple Register 20 (R) */
#define UB_STP21                          0x005204D4 /* Stipple Register 21 (R) */
#define UB_STP22                          0x005204D8 /* Stipple Register 22 (R) */
#define UB_STP23                          0x005204DC /* Stipple Register 23 (R) */
#define UB_STP24                          0x005204E0 /* Stipple Register 24 (R) */
#define UB_STP25                          0x005204E4 /* Stipple Register 25 (R) */
#define UB_STP26                          0x005204E8 /* Stipple Register 26 (R) */
#define UB_STP27                          0x005204EC /* Stipple Register 27 (R) */
#define UB_STP28                          0x005204F0 /* Stipple Register 28 (R) */
#define UB_STP29                          0x005204F4 /* Stipple Register 29 (R) */
#define UB_STP30                          0x005204F8 /* Stipple Register 30 (R) */
#define UB_STP31                          0x005204FC /* Stipple Register 31 (R) */
#define UB_FBC_BABoth                     0x00520804 /* DBA & SBA (reads return DBA) (R) */
#define UB_FBC_DBA                        0x00520808 /* Destination Bitmap Access Register (R) */
#define UB_FBC_SBA                        0x0052080C /* Source Bitmap Access Register (R) */
#define UB_FBC_DSA                        0x00520810 /* Destination Screen Address Register (R) */
#define UB_FBC_SSA                        0x00520814 /* Source Screen Address Register (R) */
#define UB_FBC_BPD                        0x00520824 /* Bitmap Pixel Data Register (R) */
#define UB_FBC_BPM                        0x0052082C /* Bitmap Pixel Mask Register (R) */
#define UB_FBC_BPC                        0x00520834 /* Bitmap Pixel Clip Register (R) */
#define UB_FBC_IFC                        0x0052083C /* Image Foreground Color Register (R) */
#define UB_FBC_IBC                        0x00520844 /* Image Background Color Register (R) */
#define UB_FBC_IPM                        0x0052084C /* Image Plane Mask (R) */
#define UB_FBC_MISC_CTL                   0x00520850 /* Miscellaneous Control Register (R) */
#define UB_FBC_FLN                        0x00520854 /* Fill Length (in X) (R) */
#define UB_FBC_RPH                        0x0052085C /* Read Prefetch Hint Register (R) */
#define UB_FBC_WCE                        0x0052086C /* Window Clip Enable Register (R) */
#define UB_FBC_PCR                        0x00520874 /* Pattern Control Register (R) */
#define UB_FBC_WEN                        0x00520884 /* FBC Write Enable Register (R) */
#define UB_FBC_REN                        0x00520888 /* FBC Read Enable Register (R) */
#define UB_FBC_UBREN                      0x0052088C /* Unbuffered FBC Read Enable Register (RW) */
#define UB_RTSTATUS                       0x00520890 /* Unbuffered RT Status Register (R) */
#define UB_FBC_WUL0                       0x00520900 /* Window Clip Upper Left 0 (R) */
#define UB_FBC_WLR0                       0x00520904 /* Window Clip Lower Right 0 (R) */
#define UB_FBC_WUL1                       0x00520908 /* Window Clip Upper Left 1 (R) */
#define UB_FBC_WLR1                       0x0052090C /* Window Clip Lower Right 1 (R) */
#define UB_FBC_WUL2                       0x00520910 /* Window Clip Upper Left 2 (R) */
#define UB_FBC_WLR2                       0x00520914 /* Window Clip Lower Right 2 (R) */
#define UB_FBC_WUL3                       0x00520918 /* Window Clip Upper Left 3 (R) */
#define UB_FBC_WLR3                       0x0052091C /* Window Clip Lower Right 3 (R) */
#define UB_FBC_WUL4                       0x00520920 /* Window Clip Upper Left 4 (R) */
#define UB_FBC_WLR4                       0x00520924 /* Window Clip Lower Right 4 (R) */
#define UB_FBC_WUL5                       0x00520928 /* Window Clip Upper Left 5 (R) */
#define UB_FBC_WLR5                       0x0052092C /* Window Clip Lower Right 5 (R) */
#define UB_FBC_WUL6                       0x00520930 /* Window Clip Upper Left 6 (R) */
#define UB_FBC_WLR6                       0x00520934 /* Window Clip Lower Right 6 (R) */
#define UB_FBC_WUL7                       0x00520938 /* Window Clip Upper Left 7 (R) */
#define UB_FBC_WLR7                       0x0052093C /* Window Clip Lower Right 7 (R) */
/*  Start Of Reserved For Future Clip Windows  */
/*  End Of Reserved For Future Clip Windows  */

#define UB_IBO                            0x00521110 /* Image Bitmap Operation Register (R) */
#define UB_CPE                            0x00521114 /* Clip Plane Enable Register (R) */
#define UB_CZ                             0x00521118 /* Constant Z data Register  (R) */
#define UB_CBR                            0x0052111C /* Constant Blend Register (R) */
#define UB_ZBO                            0x00521120 /* Z Bitmap Operation Register (R) */
#define UB_SBO                            0x00521124 /* Stencil Bitmap Operation Register (R) */
#define UB_ATO                            0x00521128 /* Alpha Test Operation Register (R) */
#define UB_DOXY                           0x0052112C /* Dither Offset  XY Register (R) */
#define UB_FOG                            0x00521134 /* Fog Color Register (R) */
#define UB_FZ                             0x00521138 /* Fast  Z Clear Register (R) */
#define UB_BMAP_Z                         0x0052113C /* BMAP Z Register (R) */
#define UB_TEC0                           0x00521140 /* Texture Environment Color  0 Register (R) */
#define UB_TEC1                           0x00521144 /* Texture Environment Color 1 Register (R) */
#define UB_TEX_2D                         0x00521154 /* Texture 2D Register (R) */
#define UB_FOG_2D                         0x00521160 /* Fog 2D Register (R) */
#define UB_SCK_UR                         0x00521170 /* Source Color Key Upper Range Register (R) */
#define UB_SCK_LR                         0x00521174 /* Source Color Key Lower Range Register (R) */
#define UB_CKC                            0x00521178 /* Color Key Configuration Register (R) */
#define UB_VTR                            0x00521180 /* Visibility Test Statistics Results (R) */
#define UB_VTE                            0x00521184 /* Visibility Test Enable (R) */
#define UB_VPCNT                          0x0052118c /* Visibility Count Pass Register (R) */
#define UB_VFCNT                          0x00521190 /* Visibility Count Fail Register (R) */
#define UB_MBWB                           0x00521194 /* Multi-buffer Write Buffer B (R) */
#define UB_MBWC                           0x00521198 /* Multi-buffer Write Buffer C (R) */
#define UB_MBWD                           0x0052119c /* Multi-buffer Write Buffer D (R) */
#define UB_FCDA                           0x005211a4 /* Fast Cleat Default Alpha Register (R) */
#define UB_BTO                            0x005211a8 /* Pixel Batching Timeout Value (R) */
#define UB_RC_CONFIG0                     0x005211b0 /* Ram controller timing configuration register 0 (R) */
#define UB_RC_CONFIG1                     0x005211b4 /* Ram controller timing configuration register 1 (R) */
#define UB_RC_CONFIG2                     0x005211b8 /* Ram controller timing configuration register 2 (R) */
#define UB_FPR                            0x00522008 /* FBC Parameters Register (R) */
/*  Start of FBC Front-End Specific Registers  */
#define UB_FE_BMAP_BABoth                 0x00528000 /* Reads of FE's copy of Shared BMAP_DBA (R) */
#define UB_FE_BMAP_DBA                    0x00528004 /* Reads of FE's copy of Shared BMAP_DBA (R) */
#define UB_FE_BMAP_SBA                    0x00528008 /* Reads of FE's copy of Shared BMAP_SBA (R) */
#define UB_FE_CPE                         0x0052800C /* Reads of FE's copy of Shared CPE (R) */
/*  End of FBC Front-End Specific Registers  */

/*  Start of  FBC Fragment Processor Specific Registers  */
#define UB_FBC_FP_WORG                    0x00529000 /* FBC/FP read address for WORG (R) */
#define UB_FBC_FP_EN2D                    0x00529004 /* FBC/FP read address for EN2D (R) */
#define UB_FBC_FP_DBA                     0x00529008 /* FBC/FP read address for DBA (R) */
#define UB_FBC_FP_RMR                     0x0052900C /* FBC/FP read address for RMR (R) */
#define UB_FBC_FP_CMD                     0x00529010 /* FBC/FP read address for CMD (R) */
#define UB_FBC_FP_BMAP_DBA                0x00529014 /* FBC/FP read address for BMAP_DBA (R) */
/*  End of  FBC Fragment Processor Specific Registers  */

/*  Start of  FBC Tile Builder Specific Registers  */
#define UB_TB_BMAP_BABoth                 0x0052A000 /* Reads of TB's copy of Shared BMAP_DBA (R) */
#define UB_TB_BMAP_DBA                    0x0052A004 /* Reads of TB's copy of Shared BMAP_DBA (R) */
#define UB_TB_BMAP_SBA                    0x0052A008 /* Reads of TB's copy of Shared BMAP_SBA (R) */
#define UB_FBC_TB_REN                     0x0052A00C /* FBC/TB read address for FBC_SREN (R) */
#define UB_FBC_TB_DBA                     0x0052A010 /* FBC/TB read address for DBA (R) */
#define UB_FBC_TB_SBA                     0x0052A014 /* FBC/TB read address for SBA (R) */
#define UB_FBC_TB_WEN                     0x0052A018 /* FBC/TB read address for FBC_SWEN (R) */
/*  End of  FBC Tile Builder Specific Registers  */

/*  Start of  FBC Memory Controller Specific Registers  */
#define UB_RC_STATE0                      0x0052B1C0 /* Ram Controller State 0 (R) */
#define UB_RC_STATE1                      0x0052B1C4 /* Ram Controller State 1 (R) */
#define UB_RC_STATE2                      0x0052B1C8 /* Ram Controller State 2 (R) */
#define UB_RC_STATE3                      0x0052B1CC /* Ram Controller State 3 (R) */
#define UB_MC_BABoth                      0x0052B804 /* DBA & SBA (reads return DBA) (R) */
#define UB_MC_DBA                         0x0052B808 /* Destination Bitmap Access Register (R) */
#define UB_MC_BMAP_BABoth                 0x0052BC20 /* BMAP DBA and SBA (reads return BMAP_DBA) (R) */
#define UB_MC_BMAP_DBA                    0x0052BC24 /* BMAP Destination Bitmap Access register (R) */
/*  End of  FBC Memory Controller Specific Registers  */

/*  End of FBC Registers  */

/*  Start of Shadowed FBC Registers(sent to all FBC blocks)  */
#define B2_FBC_FOEU                       0x00920400 /* Fragment Operation Enable Register (Upper Half) (RW) */
#define B2_FBC_FOE                        0x00920404 /* Fragment Operation Enable Register (RW) */
#define B2_FBC_TFO                        0x00920408 /* Texture Fragment Operation Register (RW) */
#define B2_MTB0                           0x00920420 /* Multi-Texture Blend 0 Register (RW) */
#define B2_MTB1                           0x00920424 /* Multi-Texture Blend 1 Register (RW) */
#define B2_MTB2                           0x00920428 /* Multi-Texture Blend 2 Register (RW) */
#define B2_MTB3                           0x0092042C /* Multi-Texture Blend 3 Register (RW) */
/*  Start of Reserved for Future Multi-Texture Blend Regs  */
/*  End of Reserved for Future Multi-Texture Blend Regs  */

#define B2_STP0                           0x00920480 /* Stipple Register 0 (RW) */
#define B2_STP1                           0x00920484 /* Stipple Register 1 (RW) */
#define B2_STP2                           0x00920488 /* Stipple Register 2 (RW) */
#define B2_STP3                           0x0092048C /* Stipple Register 3 (RW) */
#define B2_STP4                           0x00920490 /* Stipple Register 4 (RW) */
#define B2_STP5                           0x00920494 /* Stipple Register 5 (RW) */
#define B2_STP6                           0x00920498 /* Stipple Register 6 (RW) */
#define B2_STP7                           0x0092049C /* Stipple Register 7 (RW) */
#define B2_STP8                           0x009204A0 /* Stipple Register 8 (RW) */
#define B2_STP9                           0x009204A4 /* Stipple Register 9 (RW) */
#define B2_STP10                          0x009204A8 /* Stipple Register 10 (RW) */
#define B2_STP11                          0x009204AC /* Stipple Register 11 (RW) */
#define B2_STP12                          0x009204B0 /* Stipple Register 12 (RW) */
#define B2_STP13                          0x009204B4 /* Stipple Register 13 (RW) */
#define B2_STP14                          0x009204B8 /* Stipple Register 14 (RW) */
#define B2_STP15                          0x009204BC /* Stipple Register 15 (RW) */
#define B2_STP16                          0x009204C0 /* Stipple Register 16 (RW) */
#define B2_STP17                          0x009204C4 /* Stipple Register 17 (RW) */
#define B2_STP18                          0x009204C8 /* Stipple Register 18 (RW) */
#define B2_STP19                          0x009204CC /* Stipple Register 19 (RW) */
#define B2_STP20                          0x009204D0 /* Stipple Register 20 (RW) */
#define B2_STP21                          0x009204D4 /* Stipple Register 21 (RW) */
#define B2_STP22                          0x009204D8 /* Stipple Register 22 (RW) */
#define B2_STP23                          0x009204DC /* Stipple Register 23 (RW) */
#define B2_STP24                          0x009204E0 /* Stipple Register 24 (RW) */
#define B2_STP25                          0x009204E4 /* Stipple Register 25 (RW) */
#define B2_STP26                          0x009204E8 /* Stipple Register 26 (RW) */
#define B2_STP27                          0x009204EC /* Stipple Register 27 (RW) */
#define B2_STP28                          0x009204F0 /* Stipple Register 28 (RW) */
#define B2_STP29                          0x009204F4 /* Stipple Register 29 (RW) */
#define B2_STP30                          0x009204F8 /* Stipple Register 30 (RW) */
#define B2_STP31                          0x009204FC /* Stipple Register 31 (RW) */

#define B2_FBC_BABoth                     0x00920804 /* DBA & SBA (reads return DBA) (RW) */
#define B2_FBC_DBA                        0x00920808 /* Destination Bitmap Access Register (RW) */
#define B2_FBC_SBA                        0x0092080C /* Source Bitmap Access Register (RW) */
#define B2_FBC_DSA                        0x00920810 /* Destination Screen Address Register (RW) */
#define B2_FBC_SSA                        0x00920814 /* Source Screen Address Register (RW) */
#define B2_FBC_WORG                       0x00920818 /* FBC Window Origin (W) */
#define B2_FBC_FBS                        0x0092081C /* Front Buffer Select Register (RW) */
#define B2_FBC_BPDU                       0x00920820 /* Bitmap Pixel Data Register (Upper Half) (RW) */
#define B2_FBC_BPD                        0x00920824 /* Bitmap Pixel Data Register (RW) */
#define B2_FBC_BPMU                       0x00920828 /* Bitmap Pixel Mask Register (Upper Half) (RW) */
#define B2_FBC_BPM                        0x0092082C /* Bitmap Pixel Mask Register (RW) */
#define B2_FBC_BPCU                       0x00920830 /* Bitmap Pixel Clip Register (Upper Half) (RW) */
#define B2_FBC_BPC                        0x00920834 /* Bitmap Pixel Clip Register (RW) */
#define B2_FBC_IFCU                       0x00920838 /* Image Foreground Color Register (Upper Half) (RW) */
#define B2_FBC_IFC                        0x0092083C /* Image Foreground Color Register (RW) */
#define B2_FBC_IBCU                       0x00920840 /* Image Background Color Register (Upper Half) (RW) */
#define B2_FBC_IBC                        0x00920844 /* Image Background Color Register (RW) */
#define B2_FBC_IPMU                       0x00920848 /* Image Plane Mask Register (Upper Half) (RW) */
#define B2_FBC_IPM                        0x0092084C /* Image Plane Mask Register (RW) */
#define B2_FBC_MISC_CTL                   0x00920850 /* Miscellaneous Control Register (RW) */
#define B2_FBC_FLN                        0x00920854 /* Fill Length (in X) (RW) */
#define B2_FBC_RPH                        0x0092085C /* Read Prefetch Hint Register (R) */
#define B2_FBC_RBS                        0x00920860 /* Read Byte Swap Register (RW) */
#define B2_FBC_WCEU                       0x00920868 /* Window Clip Enable Register (Upper Half) (RW) */
#define B2_FBC_WCE                        0x0092086C /* Window Clip Enable Register (RW) */
#define B2_FBC_PCRU                       0x00920870 /* Pattern Control Register (Upper Half) (RW) */
#define B2_FBC_PCR                        0x00920874 /* Pattern Control Register (RW) */
#define B2_FBC_BLN                        0x00920880 /* BLT Length (in X) (RW) */
#define B2_FBC_WEN                        0x00920884 /* FBC Write Enable Register (RW) */
#define B2_FBC_REN                        0x00920888 /* FBC Read Enable Register (RW) */
#define B2_RTSTATUS                       0x00920890 /* RT Status Register (R) */
#define B2_FBC_BMAP_BABoth                0x009208A0 /* BMAP DBA and SBA (reads return BMAP_DBA) (RW) */
#define B2_FBC_BMAP_DBA                   0x009208A4 /* BMAP Destination Bitmap Access register (RW) */
#define B2_FBC_BMAP_SBA                   0x009208AC /* BMAP Source Bitmap Access register (RW) */
#define B2_FBC_WUL0                       0x00920900 /* Window Clip Upper Left 0 (RW) */
#define B2_FBC_WLR0                       0x00920904 /* Window Clip Lower Right 0 (RW) */
#define B2_FBC_WUL1                       0x00920908 /* Window Clip Upper Left 1 (RW) */
#define B2_FBC_WLR1                       0x0092090C /* Window Clip Lower Right 1 (RW) */
#define B2_FBC_WUL2                       0x00920910 /* Window Clip Upper Left 2 (RW) */
#define B2_FBC_WLR2                       0x00920914 /* Window Clip Lower Right 2 (RW) */
#define B2_FBC_WUL3                       0x00920918 /* Window Clip Upper Left 3 (RW) */
#define B2_FBC_WLR3                       0x0092091C /* Window Clip Lower Right 3 (RW) */
#define B2_FBC_WUL4                       0x00920920 /* Window Clip Upper Left 4 (RW) */
#define B2_FBC_WLR4                       0x00920924 /* Window Clip Lower Right 4 (RW) */
#define B2_FBC_WUL5                       0x00920928 /* Window Clip Upper Left 5 (RW) */
#define B2_FBC_WLR5                       0x0092092C /* Window Clip Lower Right 5 (RW) */
#define B2_FBC_WUL6                       0x00920930 /* Window Clip Upper Left 6 (RW) */
#define B2_FBC_WLR6                       0x00920934 /* Window Clip Lower Right 6 (RW) */
#define B2_FBC_WUL7                       0x00920938 /* Window Clip Upper Left 7 (RW) */
#define B2_FBC_WLR7                       0x0092093C /* Window Clip Lower Right 7 (RW) */
/*  Start Of Reserved For Future Clip Windows  */
/*  End Of Reserved For Future Clip Windows  */

#define B2_AMAP0                          0x00920A00 /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP1                          0x00920A04 /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP2                          0x00920A08 /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP3                          0x00920A0C /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP4                          0x00920A10 /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP5                          0x00920A14 /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP6                          0x00920A18 /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP7                          0x00920A1C /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP8                          0x00920A20 /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP9                          0x00920A24 /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP10                         0x00920A28 /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP11                         0x00920A2C /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP12                         0x00920A30 /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP13                         0x00920A34 /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP14                         0x00920A38 /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP15                         0x00920A3C /* Direct Frame Buffer Address Mapping  (RW) */
#define B2_AMAP_BA_U                      0x00920AF0 /* Direct Frame Buffer YUV420 U Address Mapping  (RW) */
#define B2_AMAP_BA_V                      0x00920AF4 /* Direct Frame Buffer YUV420 V Address Mapping  (RW) */
/*  End of AMAP register space  */

#define B2_BMAP_DFB0                      0x00920B00 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B2_BMAP_DFB1                      0x00920B04 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B2_BMAP_DFB2                      0x00920B08 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B2_BMAP_DFB3                      0x00920B0C /* Direct Frame Buffer Access BMAP registers (RW) */
#define B2_BMAP_DFB4                      0x00920B10 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B2_BMAP_DFB5                      0x00920B14 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B2_BMAP_DFB6                      0x00920B18 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B2_BMAP_DFB7                      0x00920B1C /* Direct Frame Buffer Access BMAP registers (RW) */
#define B2_BMAP_DFB8                      0x00920B20 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B2_BMAP_DFB9                      0x00920B24 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B2_BMAP_DFB10                     0x00920B28 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B2_BMAP_DFB11                     0x00920B2C /* Direct Frame Buffer Access BMAP registers (RW) */
#define B2_BMAP_DFB12                     0x00920B30 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B2_BMAP_DFB13                     0x00920B34 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B2_BMAP_DFB14                     0x00920B38 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B2_BMAP_DFB15                     0x00920B3C /* Direct Frame Buffer Access BMAP registers (RW) */
/*  End of BMAP_DFB register space  */

/*  End of FBC_TB reserved address space  */

/*  End of Shadowed FBC Registers(sent to all FBC blocks)  */

/*  Start of  Misc FBC Non-Shadowed registers  */
#define B2_FZ                             0x00921108 /* Fast Z Clear Register (RW) */
#define B2_IBO                            0x00921110 /* Image Bitmap Operation Register (RW) */
#define B2_CPE                            0x00921114 /* Clip Plane Enable Register (RW) */
#define B2_CZ                             0x00921118 /* Constant Z data Register  (RW) */
#define B2_CBR                            0x0092111C /* Constant Blend Register (RW) */
#define B2_ZBO                            0x00921120 /* Z Bitmap Operation Register (RW) */
#define B2_SBO                            0x00921124 /* Stencil Bitmap Operation Register (RW) */
#define B2_ATO                            0x00921128 /* Alpha Test Operation Register (RW) */
#define B2_DOXY                           0x0092112C /* Dither Offset XY Register (RW) */
#define B2_FOGU                           0x00921130 /* Fog Color Register (Upper Half) (RW) */
#define B2_FOG                            0x00921134 /* Fog Color Register (RW) */
#define B2_BMAP_Z                         0x0092113C /* BMAP Z register (RW) */
#define B2_TEC0                           0x00921140 /* Texture Environment Color  0 Register (RW) */
#define B2_TEC1                           0x00921144 /* Texture Environment Color 1 Register (RW) */
#define B2_OTR                            0x00921148 /* Overlay Transparency Register (RW) */
#define B2_TEX_2DU                        0x00921150 /* Texture 2D Register (Upper Half) (RW) */
#define B2_TEX_2D                         0x00921154 /* Texture 2D Register (RW) */
#define B2_FOG_2D                         0x00921160 /* Fog 2D Register (RW) */
#define B2_SCK_UR                         0x00921170 /* Source Color Key Upper Range Register (RW) */
#define B2_SCK_LR                         0x00921174 /* Source Color Key Lower Range Register (RW) */
#define B2_CKC                            0x00921178 /* Color Key Configuration Register (RW) */
#define B2_VTR                            0x00921180 /* Visibility Test Statistics Results (RW) */
#define B2_VTE                            0x00921184 /* Visibility Test Enable (RW) */
#define B2_VTC                            0x00921188 /* Visibility Test Clear (W) */
#define B2_VPCNT                          0x0092118c /* Visibility Count Pass Register (RW) */
#define B2_VFCNT                          0x00921190 /* Visibility Count Fail Register (RW) */
#define B2_MBWB                           0x00921194 /* Multi-buffer Write Buffer B (RW) */
#define B2_MBWC                           0x00921198 /* Multi-buffer Write Buffer C (RW) */
#define B2_MBWD                           0x0092119c /* Multi-buffer Write Buffer D (RW) */
#define B2_FSR                            0x009211C4 /* Fast Surface Clear Pixel Read Register (RW) */
#define B2_FSRMWA                         0x009211C8 /* Fast Surface Clear Register RMW buffer A (RW) */
#define B2_FSRMWB                         0x009211CC /* Fast Surface Clear Register RMW buffer B (RW) */
#define B2_FSRMWC                         0x009211D0 /* Fast Surface Clear Register RMW buffer C (RW) */
#define B2_FSRMWD                         0x009211D4 /* Fast Surface Clear Register RMW buffer D (RW) */
#define B2_FCDA                           0x009211D8 /* Fast Clear Default Alpha Register (RW) */
#define B2_BTO                            0x009211DC /* Pixel Batching Timeout Value (RW) */
#define B2_BIST                           0x009211e8 /* Built-in Self Test register (RW) */
#define B2_RC_CONFIG0                     0x009211f0 /* Ram controller timing configuration register 0 (RW) */
#define B2_RC_CONFIG1                     0x009211f4 /* Ram controller timing configuration register 1 (RW) */
#define B2_RC_CONFIG2                     0x009211f8 /* Ram controller timing configuration register 2 (RW) */
#define B2_IAA2                           0x00921208 /* Image Attribute Active 2 Register (RW) */
#define B2_IAA3                           0x0092120C /* Image Attribute Active 3 Register (RW) */
#define B2_IAA4                           0x00921210 /* Image Attribute Active 4 Register (RW) */
#define B2_IAA5                           0x00921214 /* Image Attribute Active 5 Register (RW) */
#define B2_IAA6                           0x00921218 /* Image Attribute Active 6 Register (RW) */
#define B2_IAA7                           0x0092121C /* Image Attribute Active 7 Register (RW) */
#define B2_IAA8                           0x00921220 /* Image Attribute Active 8 Register (RW) */
#define B2_IAA9                           0x00921224 /* Image Attribute Active 9 Register (RW) */
#define B2_IAA10                          0x00921228 /* Image Attribute Active 10 Register (RW) */
#define B2_IAA11                          0x0092122C /* Image Attribute Active 11 Register (RW) */
#define B2_IAA12                          0x00921230 /* Image Attribute Active 12 Register (RW) */
#define B2_IAA13                          0x00921234 /* Image Attribute Active 13 Register (RW) */
#define B2_IAA14                          0x00921238 /* Image Attribute Active 14 Register (RW) */
#define B2_IAA15                          0x0092123C /* Image Attribute Active 15 Register (RW) */
/*  Start Of Reserved For Future Image Attribute Active Regs  */
/*  End Of Reserved For Future Image Attribute Active Regs  */

#define B2_IAP0                           0x00921280 /* Image Attribute Pending 0 Register (RW) */
#define B2_IAP1                           0x00921284 /* Image Attribute Pending 1 Register (RW) */
#define B2_IAP2                           0x00921288 /* Image Attribute Pending 2 Register (RW) */
#define B2_IAP3                           0x0092128C /* Image Attribute Pending 3 Register (RW) */
#define B2_IAP4                           0x00921290 /* Image Attribute Pending 4 Register (RW) */
#define B2_IAP5                           0x00921294 /* Image Attribute Pending 5 Register (RW) */
#define B2_IAP6                           0x00921298 /* Image Attribute Pending 6 Register (RW) */
#define B2_IAP7                           0x0092129C /* Image Attribute Pending 7 Register (RW) */
#define B2_IAP8                           0x009212A0 /* Image Attribute Pending 8 Register (RW) */
#define B2_IAP9                           0x009212A4 /* Image Attribute Pending 9 Register (RW) */
#define B2_IAP10                          0x009212A8 /* Image Attribute Pending 10 Register (RW) */
#define B2_IAP11                          0x009212AC /* Image Attribute Pending 11 Register (RW) */
#define B2_IAP12                          0x009212B0 /* Image Attribute Pending 12 Register (RW) */
#define B2_IAP13                          0x009212B4 /* Image Attribute Pending 13 Register (RW) */
#define B2_IAP14                          0x009212B8 /* Image Attribute Pending 14 Register (RW) */
#define B2_IAP15                          0x009212BC /* Image Attribute Pending 15 Register (RW) */
/*  Start Of Reserved For Future Image Attribute Pend Regs  */
/*  End Of Reserved For Future Image Attribute Pend Regs  */

#define B2_DAA0                           0x00921300 /* Doublebuffer Attribute Active 0 Register (RW) */
#define B2_DAA1                           0x00921304 /* Doublebuffer Attribute Active 1 Register (RW) */
#define B2_DAA2                           0x00921308 /* Doublebuffer Attribute Active 2 Register (RW) */
#define B2_DAA3                           0x0092130C /* Doublebuffer Attribute Active 3 Register (RW) */
#define B2_DAA4                           0x00921310 /* Doublebuffer Attribute Active 4 Register (RW) */
#define B2_DAA5                           0x00921314 /* Doublebuffer Attribute Active 5 Register (RW) */
#define B2_DAA6                           0x00921318 /* Doublebuffer Attribute Active 6 Register (RW) */
#define B2_DAA7                           0x0092131C /* Doublebuffer Attribute Active 7 Register (RW) */
#define B2_DAA8                           0x00921320 /* Doublebuffer Attribute Active 8 Register (RW) */
#define B2_DAA9                           0x00921324 /* Doublebuffer Attribute Active 9 Register (RW) */
#define B2_DAA10                          0x00921328 /* Doublebuffer Attribute Active 10 Register (RW) */
#define B2_DAA11                          0x0092132C /* Doublebuffer Attribute Active 11 Register (RW) */
#define B2_DAA12                          0x00921330 /* Doublebuffer Attribute Active 12 Register (RW) */
#define B2_DAA13                          0x00921334 /* Doublebuffer Attribute Active 13 Register (RW) */
#define B2_DAA14                          0x00921338 /* Doublebuffer Attribute Active 14 Register (RW) */
#define B2_DAA15                          0x0092133C /* Doublebuffer Attribute Active 15 Register (RW) */
/*  Start Of Reserved For Future Doublebuffer Attribute Active Regs  */
/*  End Of Reserved For Future Doublebuffer Attribute Active Regs  */

#define B2_DAP0                           0x00921380 /* Doublebuffer Attribute Pending 0 Register (RW) */
#define B2_DAP1                           0x00921384 /* Doublebuffer Attribute Pending 1 Register (RW) */
#define B2_DAP2                           0x00921388 /* Doublebuffer Attribute Pending 2 Register (RW) */
#define B2_DAP3                           0x0092138C /* Doublebuffer Attribute Pending 3 Register (RW) */
#define B2_DAP4                           0x00921390 /* Doublebuffer Attribute Pending 4 Register (RW) */
#define B2_DAP5                           0x00921394 /* Doublebuffer Attribute Pending 5 Register (RW) */
#define B2_DAP6                           0x00921398 /* Doublebuffer Attribute Pending 6 Register (RW) */
#define B2_DAP7                           0x0092139C /* Doublebuffer Attribute Pending 7 Register (RW) */
#define B2_DAP8                           0x009213A0 /* Doublebuffer Attribute Pending 8 Register (RW) */
#define B2_DAP9                           0x009213A4 /* Doublebuffer Attribute Pending 9 Register (RW) */
#define B2_DAP10                          0x009213A8 /* Doublebuffer Attribute Pending 10 Register (RW) */
#define B2_DAP11                          0x009213AC /* Doublebuffer Attribute Pending 11 Register (RW) */
#define B2_DAP12                          0x009213B0 /* Doublebuffer Attribute Pending 12 Register (RW) */
#define B2_DAP13                          0x009213B4 /* Doublebuffer Attribute Pending 13 Register (RW) */
#define B2_DAP14                          0x009213B8 /* Doublebuffer Attribute Pending 14 Register (RW) */
#define B2_DAP15                          0x009213BC /* Doublebuffer Attribute Pending 15 Register (RW) */
/*  Start Of Reserved For Future Doublebuffer Attribute Pend Regs  */
/*  End Of Reserved For Future Doublebuffer Attribute Pend Regs  */

#define B2_PDR0                           0x00921400 /* Pattern Data Register 0 (RW) */
#define B2_PDR1                           0x00921404 /* Pattern Data Register 1 (RW) */
#define B2_PDR2                           0x00921408 /* Pattern Data Register 2 (RW) */
#define B2_PDR3                           0x0092140C /* Pattern Data Register 3 (RW) */
#define B2_PDR4                           0x00921410 /* Pattern Data Register 4 (RW) */
#define B2_PDR5                           0x00921414 /* Pattern Data Register 5 (RW) */
#define B2_PDR6                           0x00921418 /* Pattern Data Register 6 (RW) */
#define B2_PDR7                           0x0092141C /* Pattern Data Register 7 (RW) */
#define B2_PDR8                           0x00921420 /* Pattern Data Register 8 (RW) */
#define B2_PDR9                           0x00921424 /* Pattern Data Register 9 (RW) */
#define B2_PDR10                          0x00921428 /* Pattern Data Register 10 (RW) */
#define B2_PDR11                          0x0092142C /* Pattern Data Register 11 (RW) */
#define B2_PDR12                          0x00921430 /* Pattern Data Register 12 (RW) */
#define B2_PDR13                          0x00921434 /* Pattern Data Register 13 (RW) */
#define B2_PDR14                          0x00921438 /* Pattern Data Register 14 (RW) */
#define B2_PDR15                          0x0092143C /* Pattern Data Register 15 (RW) */
#define B2_PDR16                          0x00921440 /* Pattern Data Register 16 (RW) */
#define B2_PDR17                          0x00921444 /* Pattern Data Register 17 (RW) */
#define B2_PDR18                          0x00921448 /* Pattern Data Register 18 (RW) */
#define B2_PDR19                          0x0092144C /* Pattern Data Register 19 (RW) */
#define B2_PDR20                          0x00921450 /* Pattern Data Register 20 (RW) */
#define B2_PDR21                          0x00921454 /* Pattern Data Register 21 (RW) */
#define B2_PDR22                          0x00921458 /* Pattern Data Register 22 (RW) */
#define B2_PDR23                          0x0092145C /* Pattern Data Register 23 (RW) */
#define B2_PDR24                          0x00921460 /* Pattern Data Register 24 (RW) */
#define B2_PDR25                          0x00921464 /* Pattern Data Register 25 (RW) */
#define B2_PDR26                          0x00921468 /* Pattern Data Register 26 (RW) */
#define B2_PDR27                          0x0092146C /* Pattern Data Register 27 (RW) */
#define B2_PDR28                          0x00921470 /* Pattern Data Register 28 (RW) */
#define B2_PDR29                          0x00921474 /* Pattern Data Register 29 (RW) */
#define B2_PDR30                          0x00921478 /* Pattern Data Register 30 (RW) */
#define B2_PDR31                          0x0092147C /* Pattern Data Register 31 (RW) */
#define B2_PDR32                          0x00921480 /* Pattern Data Register 32 (RW) */
#define B2_PDR33                          0x00921484 /* Pattern Data Register 33 (RW) */
#define B2_PDR34                          0x00921488 /* Pattern Data Register 34 (RW) */
#define B2_PDR35                          0x0092148C /* Pattern Data Register 35 (RW) */
#define B2_PDR36                          0x00921490 /* Pattern Data Register 36 (RW) */
#define B2_PDR37                          0x00921494 /* Pattern Data Register 37 (RW) */
#define B2_PDR38                          0x00921498 /* Pattern Data Register 38 (RW) */
#define B2_PDR39                          0x0092149C /* Pattern Data Register 39 (RW) */
#define B2_PDR40                          0x009214A0 /* Pattern Data Register 40 (RW) */
#define B2_PDR41                          0x009214A4 /* Pattern Data Register 41 (RW) */
#define B2_PDR42                          0x009214A8 /* Pattern Data Register 42 (RW) */
#define B2_PDR43                          0x009214AC /* Pattern Data Register 43 (RW) */
#define B2_PDR44                          0x009214B0 /* Pattern Data Register 44 (RW) */
#define B2_PDR45                          0x009214B4 /* Pattern Data Register 45 (RW) */
#define B2_PDR46                          0x009214B8 /* Pattern Data Register 46 (RW) */
#define B2_PDR47                          0x009214BC /* Pattern Data Register 47 (RW) */
#define B2_PDR48                          0x009214C0 /* Pattern Data Register 48 (RW) */
#define B2_PDR49                          0x009214C4 /* Pattern Data Register 49 (RW) */
#define B2_PDR50                          0x009214C8 /* Pattern Data Register 50 (RW) */
#define B2_PDR51                          0x009214CC /* Pattern Data Register 51 (RW) */
#define B2_PDR52                          0x009214D0 /* Pattern Data Register 52 (RW) */
#define B2_PDR53                          0x009214D4 /* Pattern Data Register 53 (RW) */
#define B2_PDR54                          0x009214D8 /* Pattern Data Register 54 (RW) */
#define B2_PDR55                          0x009214DC /* Pattern Data Register 55 (RW) */
#define B2_PDR56                          0x009214E0 /* Pattern Data Register 56 (RW) */
#define B2_PDR57                          0x009214E4 /* Pattern Data Register 57 (RW) */
#define B2_PDR58                          0x009214E8 /* Pattern Data Register 58 (RW) */
#define B2_PDR59                          0x009214EC /* Pattern Data Register 59 (RW) */
#define B2_PDR60                          0x009214F0 /* Pattern Data Register 60 (RW) */
#define B2_PDR61                          0x009214F4 /* Pattern Data Register 61 (RW) */
#define B2_PDR62                          0x009214F8 /* Pattern Data Register 62 (RW) */
#define B2_PDR63                          0x009214FC /* Pattern Data Register 63 (RW) */
#define B2_VTR_MC0                        0x00921800 /* Visibility Test Statistics  MC0 (R) */
#define B2_VTR_MC1                        0x00921804 /* Visibility Test Statistics  MC1 (R) */
#define B2_VTR_MC2                        0x00921808 /* Visibility Test Statistics MC2 (R) */
#define B2_VTR_MC3                        0x0092180C /* Visibility Test Statistics  MC3 (R) */
#define B2_VPCNT_MC0                      0x00921900 /* Visibility Count Pass Register  MC0 (R) */
#define B2_VPCNT_MC1                      0x00921904 /* Visibility Count Pass Register MC1 (R) */
#define B2_VPCNT_MC2                      0x00921908 /* Visibility Count Pass Register  MC2 (R) */
#define B2_VPCNT_MC3                      0x0092190C /* Visibility Count Pass Register MC3 (R) */
#define B2_VFCNT_MC0                      0x00921A00 /* Visibility Count Fail Register MC0 (R) */
#define B2_VFCNT_MC1                      0x00921A04 /* Visibility Count Fail Register  MC1 (R) */
#define B2_VFCNT_MC2                      0x00921A08 /* Visibility Count Fail Register  MC2 (R) */
#define B2_VFCNT_MC3                      0x00921A0C /* Visibility Count Fail Register MC3 (R) */
#define B2_FPR                            0x00922008 /* FBC Parameters Register (R) */
#define B2_FBC_PERF                       0x0092200C /* FBC Performance Register (RW) */
#define B2_FCR                            0x00922010 /* FBC CRC Read Register (R) */
#define B2_FCI                            0x00922014 /* FBC CRC Initiate Register (W) */
#define B2_FCM                            0x00922018 /* FBC CRC Mask Register (W) */
#define B2_FBC_STATUS                     0x0092201C /* FBC Status Register (W) */
#define B2_FBC_CRC1U                      0x00922020 /* FBC Ridgeline CRC Phase 1 Upper Register (W) */
#define B2_FBC_CRC1                       0x00922024 /* FBC Ridgeline CRC Phase 1 Lower Register (W) */
#define B2_FBC_CRC2U                      0x00922028 /* FBC Ridgeline CRC Phase 2 Upper Register (W) */
#define B2_FBC_CRC2                       0x0092202C /* FBC Ridgeline CRC Phase 2 Lower Register (W) */
/*  End of Misc FBC Non-Shadowed registers  */

/*  Start of FBC Front-End Specific Registers  */
#define B2_FE_BMAP_BABoth                 0x00928000 /* Reads of FE's copy of Shared BMAP_DBA (R) */
#define B2_FE_BMAP_DBA                    0x00928004 /* Reads of FE's copy of Shared BMAP_DBA (R) */
#define B2_FE_BMAP_SBA                    0x00928008 /* Reads of FE's copy of Shared BMAP_SBA (R) */
#define B2_FE_CPE                         0x0092800C /* Reads of FE's copy of Shared CPE (R) */
/*  End of FBC Front-End Specific Registers  */

/*  Start of FBC Fragment Processor Specific Registers  */
#define B2_FBC_FP_WORG                    0x00929000 /* FBC/FP read address for WORG (R) */
#define B2_FBC_FP_EN2D                    0x00929004 /* FBC/FP read address for EN2D (R) */
#define B2_FBC_FP_DBA                     0x00929008 /* FBC/FP read address for DBA (R) */
#define B2_FBC_FP_RMR                     0x0092900C /* FBC/FP read address for RMR (R) */
#define B2_FBC_FP_CMD                     0x00929010 /* FBC/FP read address for CMD (R) */
#define B2_FBC_FP_BMAP_DBA                0x00929014 /* FBC/FP read address for BMAP_DBA (R) */
/*  End of FBC Fragment Processor Specific Registers  */

/*  Start of  FBC Tile Builder Specific Registers  */
#define B2_TB_BMAP_BABoth                 0x0092A000 /* Reads of TB's copy of Shared BMAP_DBA (R) */
#define B2_TB_BMAP_DBA                    0x0092A004 /* Reads of TB's copy of Shared BMAP_DBA (R) */
#define B2_TB_BMAP_SBA                    0x0092A008 /* Reads of TB's copy of Shared BMAP_SBA (R) */
#define B2_FBC_TB_REN                     0x0092A00C /* FBC/TB read address for FBC_SREN (R) */
#define B2_FBC_TB_DBA                     0x0092A010 /* FBC/TB read address for DBA (R) */
#define B2_FBC_TB_SBA                     0x0092A014 /* FBC/TB read address for SBA (R) */
#define B2_FBC_TB_WEN                     0x0092A018 /* FBC/TB read address for FBC_SWEN (R) */
/*  End of  FBC Tile Builder Specific Registers  */

/*  Start of  FBC Memory Controller Specific Registers  */
#define B2_MC_BABoth                      0x0092B804 /* DBA & SBA (reads return DBA) (R) */
#define B2_MC_DBA                         0x0092B808 /* Destination Bitmap Access Register (R) */
#define B2_MC_BMAP_BABoth                 0x0092BC20 /* BMAP DBA and SBA (reads return BMAP_DBA) (R) */
#define B2_MC_BMAP_DBA                    0x0092BC24 /* BMAP Destination Bitmap Access register (R) */
/*  End of  FBC Memory Controller Specific Registers  */

/*  Start of  FBC Object Builder Specific Registers  */
/*  End of  FBC Object Builder Specific Registers  */

/*  End of FBC Registers  */

/*  Start of Shadowed FBC Registers(sent to all FBC blocks)  */
#define B3_FBC_FOEU                       0x00D20400 /* Fragment Operation Enable Register (Upper Half) (RW) */
#define B3_FBC_FOE                        0x00D20404 /* Fragment Operation Enable Register (RW) */
#define B3_FBC_TFO                        0x00D20408 /* Texture Fragment Operation Register (RW) */
#define B3_MTB0                           0x00D20420 /* Multi-Texture Blend 0 Register (RW) */
#define B3_MTB1                           0x00D20424 /* Multi-Texture Blend 1 Register (RW) */
#define B3_MTB2                           0x00D20428 /* Multi-Texture Blend 2 Register (RW) */
#define B3_MTB3                           0x00D2042C /* Multi-Texture Blend 3 Register (RW) */
/*  Start of Reserved for Future Multi-Texture Blend Regs  */
/*  End of Reserved for Future Multi-Texture Blend Regs  */

#define B3_STP0                           0x00D20480 /* Stipple Register 0 (RW) */
#define B3_STP1                           0x00D20484 /* Stipple Register 1 (RW) */
#define B3_STP2                           0x00D20488 /* Stipple Register 2 (RW) */
#define B3_STP3                           0x00D2048C /* Stipple Register 3 (RW) */
#define B3_STP4                           0x00D20490 /* Stipple Register 4 (RW) */
#define B3_STP5                           0x00D20494 /* Stipple Register 5 (RW) */
#define B3_STP6                           0x00D20498 /* Stipple Register 6 (RW) */
#define B3_STP7                           0x00D2049C /* Stipple Register 7 (RW) */
#define B3_STP8                           0x00D204A0 /* Stipple Register 8 (RW) */
#define B3_STP9                           0x00D204A4 /* Stipple Register 9 (RW) */
#define B3_STP10                          0x00D204A8 /* Stipple Register 10 (RW) */
#define B3_STP11                          0x00D204AC /* Stipple Register 11 (RW) */
#define B3_STP12                          0x00D204B0 /* Stipple Register 12 (RW) */
#define B3_STP13                          0x00D204B4 /* Stipple Register 13 (RW) */
#define B3_STP14                          0x00D204B8 /* Stipple Register 14 (RW) */
#define B3_STP15                          0x00D204BC /* Stipple Register 15 (RW) */
#define B3_STP16                          0x00D204C0 /* Stipple Register 16 (RW) */
#define B3_STP17                          0x00D204C4 /* Stipple Register 17 (RW) */
#define B3_STP18                          0x00D204C8 /* Stipple Register 18 (RW) */
#define B3_STP19                          0x00D204CC /* Stipple Register 19 (RW) */
#define B3_STP20                          0x00D204D0 /* Stipple Register 20 (RW) */
#define B3_STP21                          0x00D204D4 /* Stipple Register 21 (RW) */
#define B3_STP22                          0x00D204D8 /* Stipple Register 22 (RW) */
#define B3_STP23                          0x00D204DC /* Stipple Register 23 (RW) */
#define B3_STP24                          0x00D204E0 /* Stipple Register 24 (RW) */
#define B3_STP25                          0x00D204E4 /* Stipple Register 25 (RW) */
#define B3_STP26                          0x00D204E8 /* Stipple Register 26 (RW) */
#define B3_STP27                          0x00D204EC /* Stipple Register 27 (RW) */
#define B3_STP28                          0x00D204F0 /* Stipple Register 28 (RW) */
#define B3_STP29                          0x00D204F4 /* Stipple Register 29 (RW) */
#define B3_STP30                          0x00D204F8 /* Stipple Register 30 (RW) */
#define B3_STP31                          0x00D204FC /* Stipple Register 31 (RW) */

#define B3_FBC_BABoth                     0x00D20804 /* DBA & SBA (reads return DBA) (RW) */
#define B3_FBC_DBA                        0x00D20808 /* Destination Bitmap Access Register (RW) */
#define B3_FBC_SBA                        0x00D2080C /* Source Bitmap Access Register (RW) */
#define B3_FBC_DSA                        0x00D20810 /* Destination Screen Address Register (RW) */
#define B3_FBC_SSA                        0x00D20814 /* Source Screen Address Register (RW) */
#define B3_FBC_FBS                        0x00D2081C /* Front Buffer Select Register (RW) */
#define B3_FBC_BPDU                       0x00D20820 /* Bitmap Pixel Data Register (Upper Half) (RW) */
#define B3_FBC_BPD                        0x00D20824 /* Bitmap Pixel Data Register (RW) */
#define B3_FBC_BPMU                       0x00D20828 /* Bitmap Pixel Mask Register (Upper Half) (RW) */
#define B3_FBC_BPM                        0x00D2082C /* Bitmap Pixel Mask Register (RW) */
#define B3_FBC_BPCU                       0x00D20830 /* Bitmap Pixel Clip Register (Upper Half) (RW) */
#define B3_FBC_BPC                        0x00D20834 /* Bitmap Pixel Clip Register (RW) */
#define B3_FBC_IFCU                       0x00D20838 /* Image Foreground Color Register (Upper Half) (RW) */
#define B3_FBC_IFC                        0x00D2083C /* Image Foreground Color Register (RW) */
#define B3_FBC_IBCU                       0x00D20840 /* Image Background Color Register (Upper Half) (RW) */
#define B3_FBC_IBC                        0x00D20844 /* Image Background Color Register (RW) */
#define B3_FBC_IPMU                       0x00D20848 /* Image Plane Mask Register (Upper Half) (RW) */
#define B3_FBC_IPM                        0x00D2084C /* Image Plane Mask Register (RW) */
#define B3_FBC_MISC_CTL                   0x00D20850 /* Miscellaneous Control Register (RW) */
#define B3_FBC_FLN                        0x00D20854 /* Fill Length (in X) (RW) */
#define B3_FBC_RPH                        0x00D2085C /* Read Prefetch Hint Register (R) */
#define B3_FBC_RBS                        0x00D20860 /* Read Byte Swap Register (RW) */
#define B3_FBC_WCEU                       0x00D20868 /* Window Clip Enable Register (Upper Half) (RW) */
#define B3_FBC_WCE                        0x00D2086C /* Window Clip Enable Register (RW) */
#define B3_FBC_PCRU                       0x00D20870 /* Pattern Control Register (Upper Half) (RW) */
#define B3_FBC_PCR                        0x00D20874 /* Pattern Control Register (RW) */
#define B3_FBC_BLN                        0x00D20880 /* BLT Length (in X) (RW) */
#define B3_FBC_WEN                        0x00D20884 /* FBC Write Enable Register (RW) */
#define B3_FBC_REN                        0x00D20888 /* FBC Read Enable Register (RW) */
#define B3_RTSTATUS                       0x00D20890 /* RT Status Register (R) */
#define B3_FBC_BMAP_BABoth                0x00D208A0 /* BMAP DBA and SBA (reads return BMAP_DBA) (RW) */
#define B3_FBC_BMAP_DBA                   0x00D208A4 /* BMAP Destination Bitmap Access register (RW) */
#define B3_FBC_BMAP_SBA                   0x00D208A8 /* BMAP Source Bitmap Access register (RW) */
#define B3_FBC_WUL0                       0x00D20900 /* Window Clip Upper Left 0 (RW) */
#define B3_FBC_WLR0                       0x00D20904 /* Window Clip Lower Right 0 (RW) */
#define B3_FBC_WUL1                       0x00D20908 /* Window Clip Upper Left 1 (RW) */
#define B3_FBC_WLR1                       0x00D2090C /* Window Clip Lower Right 1 (RW) */
#define B3_FBC_WUL2                       0x00D20910 /* Window Clip Upper Left 2 (RW) */
#define B3_FBC_WLR2                       0x00D20914 /* Window Clip Lower Right 2 (RW) */
#define B3_FBC_WUL3                       0x00D20918 /* Window Clip Upper Left 3 (RW) */
#define B3_FBC_WLR3                       0x00D2091C /* Window Clip Lower Right 3 (RW) */
#define B3_FBC_WUL4                       0x00D20920 /* Window Clip Upper Left 4 (RW) */
#define B3_FBC_WLR4                       0x00D20924 /* Window Clip Lower Right 4 (RW) */
#define B3_FBC_WUL5                       0x00D20928 /* Window Clip Upper Left 5 (RW) */
#define B3_FBC_WLR5                       0x00D2092C /* Window Clip Lower Right 5 (RW) */
#define B3_FBC_WUL6                       0x00D20930 /* Window Clip Upper Left 6 (RW) */
#define B3_FBC_WLR6                       0x00D20934 /* Window Clip Lower Right 6 (RW) */
#define B3_FBC_WUL7                       0x00D20938 /* Window Clip Upper Left 7 (RW) */
#define B3_FBC_WLR7                       0x00D2093C /* Window Clip Lower Right 7 (RW) */
/*  Start Of Reserved For Future Clip Windows  */
/*  End Of Reserved For Future Clip Windows  */

#define B3_AMAP0                          0x00D20A00 /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP1                          0x00D20A04 /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP2                          0x00D20A08 /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP3                          0x00D20A0C /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP4                          0x00D20A10 /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP5                          0x00D20A14 /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP6                          0x00D20A18 /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP7                          0x00D20A1C /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP8                          0x00D20A20 /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP9                          0x00D20A24 /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP10                         0x00D20A28 /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP11                         0x00D20A2C /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP12                         0x00D20A30 /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP13                         0x00D20A34 /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP14                         0x00D20A38 /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP15                         0x00D20A3C /* Direct Frame Buffer Address Mapping  (RW) */
#define B3_AMAP_BA_U                      0x00D20AF0 /* Direct Frame Buffer YUV420 U Address Mapping  (RW) */
#define B3_AMAP_BA_V                      0x00D20AF4 /* Direct Frame Buffer YUV420 V Address Mapping  (RW) */
/*  End of AMAP register space  */

#define B3_BMAP_DFB0                      0x00D20B00 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B3_BMAP_DFB1                      0x00D20B04 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B3_BMAP_DFB2                      0x00D20B08 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B3_BMAP_DFB3                      0x00D20B0C /* Direct Frame Buffer Access BMAP registers (RW) */
#define B3_BMAP_DFB4                      0x00D20B10 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B3_BMAP_DFB5                      0x00D20B14 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B3_BMAP_DFB6                      0x00D20B18 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B3_BMAP_DFB7                      0x00D20B1C /* Direct Frame Buffer Access BMAP registers (RW) */
#define B3_BMAP_DFB8                      0x00D20B20 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B3_BMAP_DFB9                      0x00D20B24 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B3_BMAP_DFB10                     0x00D20B28 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B3_BMAP_DFB11                     0x00D20B2C /* Direct Frame Buffer Access BMAP registers (RW) */
#define B3_BMAP_DFB12                     0x00D20B30 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B3_BMAP_DFB13                     0x00D20B34 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B3_BMAP_DFB14                     0x00D20B38 /* Direct Frame Buffer Access BMAP registers (RW) */
#define B3_BMAP_DFB15                     0x00D20B3C /* Direct Frame Buffer Access BMAP registers (RW) */
/*  End of BMAP_DFB register space  */

/*  End of FBC_TB reserved address space  */

/*  End of Shadowed FBC Registers(sent to all FBC blocks)  */

/*  Start of  Misc FBC Non-Shadowed registers  */
#define B3_FZ                             0x00D21108 /* Fast Z Clear Register (RW) */
#define B3_IBO                            0x00D21110 /* Image Bitmap Operation Register (RW) */
#define B3_CPE                            0x00D21114 /* Clip Plane Enable Register (RW) */
#define B3_CZ                             0x00D21118 /* Constant Z data Register  (RW) */
#define B3_CBR                            0x00D2111C /* Constant Blend Register (RW) */
#define B3_ZBO                            0x00D21120 /* Z Bitmap Operation Register (RW) */
#define B3_SBO                            0x00D21124 /* Stencil Bitmap Operation Register (RW) */
#define B3_DOXY                           0x00D2112C /* Dither Offset XY Register (RW) */
#define B3_ATO                            0x00D21128 /* Alpha Test Operation Register (RW) */
#define B3_FOGU                           0x00D21130 /* Fog Color Register (Upper Half) (RW) */
#define B3_FOG                            0x00D21134 /* Fog Color Register (RW) */
#define B3_BMAP_Z                         0x00D2113C /* BMAP Z register (RW) */
#define B3_TEC0                           0x00D21140 /* Texture Environment Color  0 Register (RW) */
#define B3_TEC1                           0x00D21144 /* Texture Environment Color 1 Register (RW) */
#define B3_OTR                            0x00D21148 /* Overlay Transparency Register (RW) */
#define B3_TEX_2DU                        0x00D21150 /* Texture 2D Register (Upper Half) (RW) */
#define B3_TEX_2D                         0x00D21154 /* Texture 2D Register (RW) */
#define B3_FOG_2D                         0x00D21160 /* Fog 2D Register (RW) */
#define B3_SCK_UR                         0x00D21170 /* Source Color Key Upper Range Register (RW) */
#define B3_SCK_LR                         0x00D21174 /* Source Color Key Lower Range Register (RW) */
#define B3_CKC                            0x00D21178 /* Color Key Configuration Register (RW) */
#define B3_VTR                            0x00D21180 /* Visibility Test Statistics Results (RW) */
#define B3_VTE                            0x00D21184 /* Visibility Test Enable (RW) */
#define B3_VTC                            0x00D21188 /* Visibility Test Clear (W) */
#define B3_VPCNT                          0x00D2118C /* Visibility Count Pass Register (RW) */
#define B3_VFCNT                          0x00D21190 /* Visibility Count Fail Register (RW) */
#define B3_MBWB                           0x00D21194 /* Multi-buffer Write Buffer B (RW) */
#define B3_MBWC                           0x00D21198 /* Multi-buffer Write Buffer C (RW) */
#define B3_MBWD                           0x00D2119C /* Multi-buffer Write Buffer D (RW) */
#define B3_FSR                            0x00D211C4 /* Fast  Surface Clear Register, pixel reads (RW) */
#define B3_FSRMWA                         0x00D211C8 /* Fast I Surface Clear Register A, pixel writes (RW) */
#define B3_FSRMWB                         0x00D211CC /* Fast Surface Clear Register B, pixel writes (RW) */
#define B3_FSRMWC                         0x00D211D0 /* Fast Surface Clear Register C, pixel writes (RW) */
#define B3_FSRMWD                         0x00D211D4 /* Fast Surface Clear Register D, pixel writes (RW) */
#define B3_FCDA                           0x00D211D8 /* Fast Clear Default Alpha (RW) */
#define B3_BTO                            0x00D211DC /* Pixel Batching Timeout Value (RW) */
#define B3_BIST                           0x00D211E8 /* Built-in RAM self test Register (RW) */
#define B3_RC_CONFIG0                     0x00D211F0 /* Ram controller timing configuration register 0 (RW) */
#define B3_RC_CONFIG1                     0x00D211F4 /* Ram controller timing configuration register 1 (RW) */
#define B3_RC_CONFIG2                     0x00D211F8 /* Ram controller timing configuration register 2 (RW) */
#define B3_IAA1                           0x00D21204 /* Image Attribute Active 1 Register (RW) */
#define B3_IAA2                           0x00D21208 /* Image Attribute Active 2 Register (RW) */
#define B3_IAA3                           0x00D2120C /* Image Attribute Active 3 Register (RW) */
#define B3_IAA4                           0x00D21210 /* Image Attribute Active 4 Register (RW) */
#define B3_IAA5                           0x00D21214 /* Image Attribute Active 5 Register (RW) */
#define B3_IAA6                           0x00D21218 /* Image Attribute Active 6 Register (RW) */
#define B3_IAA7                           0x00D2121C /* Image Attribute Active 7 Register (RW) */
#define B3_IAA8                           0x00D21220 /* Image Attribute Active 8 Register (RW) */
#define B3_IAA9                           0x00D21224 /* Image Attribute Active 9 Register (RW) */
#define B3_IAA10                          0x00D21228 /* Image Attribute Active 10 Register (RW) */
#define B3_IAA11                          0x00D2122C /* Image Attribute Active 11 Register (RW) */
#define B3_IAA12                          0x00D21230 /* Image Attribute Active 12 Register (RW) */
#define B3_IAA13                          0x00D21234 /* Image Attribute Active 13 Register (RW) */
#define B3_IAA14                          0x00D21238 /* Image Attribute Active 14 Register (RW) */
#define B3_IAA15                          0x00D2123C /* Image Attribute Active 15 Register (RW) */
/*  Start Of Reserved For Future Image Attribute Active Regs  */
/*  End Of Reserved For Future Image Attribute Active Regs  */

#define B3_IAP0                           0x00D21280 /* Image Attribute Pending 0 Register (RW) */
#define B3_IAP1                           0x00D21284 /* Image Attribute Pending 1 Register (RW) */
#define B3_IAP2                           0x00D21288 /* Image Attribute Pending 2 Register (RW) */
#define B3_IAP3                           0x00D2128C /* Image Attribute Pending 3 Register (RW) */
#define B3_IAP4                           0x00D21290 /* Image Attribute Pending 4 Register (RW) */
#define B3_IAP5                           0x00D21294 /* Image Attribute Pending 5 Register (RW) */
#define B3_IAP6                           0x00D21298 /* Image Attribute Pending 6 Register (RW) */
#define B3_IAP7                           0x00D2129C /* Image Attribute Pending 7 Register (RW) */
#define B3_IAP8                           0x00D212A0 /* Image Attribute Pending 8 Register (RW) */
#define B3_IAP9                           0x00D212A4 /* Image Attribute Pending 9 Register (RW) */
#define B3_IAP10                          0x00D212A8 /* Image Attribute Pending 10 Register (RW) */
#define B3_IAP11                          0x00D212AC /* Image Attribute Pending 11 Register (RW) */
#define B3_IAP12                          0x00D212B0 /* Image Attribute Pending 12 Register (RW) */
#define B3_IAP13                          0x00D212B4 /* Image Attribute Pending 13 Register (RW) */
#define B3_IAP14                          0x00D212B8 /* Image Attribute Pending 14 Register (RW) */
#define B3_IAP15                          0x00D212BC /* Image Attribute Pending 15 Register (RW) */
/*  Start Of Reserved For Future Image Attribute Pend Regs  */
/*  End Of Reserved For Future Image Attribute Pend Regs  */

#define B3_DAA0                           0x00D21300 /* Doublebuffer Attribute Active 0 Register (RW) */
#define B3_DAA1                           0x00D21304 /* Doublebuffer Attribute Active 1 Register (RW) */
#define B3_DAA2                           0x00D21308 /* Doublebuffer Attribute Active 2 Register (RW) */
#define B3_DAA3                           0x00D2130C /* Doublebuffer Attribute Active 3 Register (RW) */
#define B3_DAA4                           0x00D21310 /* Doublebuffer Attribute Active 4 Register (RW) */
#define B3_DAA5                           0x00D21314 /* Doublebuffer Attribute Active 5 Register (RW) */
#define B3_DAA6                           0x00D21318 /* Doublebuffer Attribute Active 6 Register (RW) */
#define B3_DAA7                           0x00D2131C /* Doublebuffer Attribute Active 7 Register (RW) */
#define B3_DAA8                           0x00D21320 /* Doublebuffer Attribute Active 8 Register (RW) */
#define B3_DAA9                           0x00D21324 /* Doublebuffer Attribute Active 9 Register (RW) */
#define B3_DAA10                          0x00D21328 /* Doublebuffer Attribute Active 10 Register (RW) */
#define B3_DAA11                          0x00D2132C /* Doublebuffer Attribute Active 11 Register (RW) */
#define B3_DAA12                          0x00D21330 /* Doublebuffer Attribute Active 12 Register (RW) */
#define B3_DAA13                          0x00D21334 /* Doublebuffer Attribute Active 13 Register (RW) */
#define B3_DAA14                          0x00D21338 /* Doublebuffer Attribute Active 14 Register (RW) */
#define B3_DAA15                          0x00D2133C /* Doublebuffer Attribute Active 15 Register (RW) */
/*  Start Of Reserved For Future Doublebuffer Attribute Active Regs  */
/*  End Of Reserved For Future Doublebuffer Attribute Active Regs  */

#define B3_DAP0                           0x00D21380 /* Doublebuffer Attribute Pending 0 Register (RW) */
#define B3_DAP1                           0x00D21384 /* Doublebuffer Attribute Pending 1 Register (RW) */
#define B3_DAP2                           0x00D21388 /* Doublebuffer Attribute Pending 2 Register (RW) */
#define B3_DAP3                           0x00D2138C /* Doublebuffer Attribute Pending 3 Register (RW) */
#define B3_DAP4                           0x00D21390 /* Doublebuffer Attribute Pending 4 Register (RW) */
#define B3_DAP5                           0x00D21394 /* Doublebuffer Attribute Pending 5 Register (RW) */
#define B3_DAP6                           0x00D21398 /* Doublebuffer Attribute Pending 6 Register (RW) */
#define B3_DAP7                           0x00D2139C /* Doublebuffer Attribute Pending 7 Register (RW) */
#define B3_DAP8                           0x00D213A0 /* Doublebuffer Attribute Pending 8 Register (RW) */
#define B3_DAP9                           0x00D213A4 /* Doublebuffer Attribute Pending 9 Register (RW) */
#define B3_DAP10                          0x00D213A8 /* Doublebuffer Attribute Pending 10 Register (RW) */
#define B3_DAP11                          0x00D213AC /* Doublebuffer Attribute Pending 11 Register (RW) */
#define B3_DAP12                          0x00D213B0 /* Doublebuffer Attribute Pending 12 Register (RW) */
#define B3_DAP13                          0x00D213B4 /* Doublebuffer Attribute Pending 13 Register (RW) */
#define B3_DAP14                          0x00D213B8 /* Doublebuffer Attribute Pending 14 Register (RW) */
#define B3_DAP15                          0x00D213BC /* Doublebuffer Attribute Pending 15 Register (RW) */
/*  Start Of Reserved For Future Doublebuffer Attribute Pend Regs  */
/*  End Of Reserved For Future Doublebuffer Attribute Pend Regs  */

#define B3_PDR0                           0x00D21400 /* Pattern Data Register 0 (RW) */
#define B3_PDR1                           0x00D21404 /* Pattern Data Register 1 (RW) */
#define B3_PDR2                           0x00D21408 /* Pattern Data Register 2 (RW) */
#define B3_PDR3                           0x00D2140C /* Pattern Data Register 3 (RW) */
#define B3_PDR4                           0x00D21410 /* Pattern Data Register 4 (RW) */
#define B3_PDR5                           0x00D21414 /* Pattern Data Register 5 (RW) */
#define B3_PDR6                           0x00D21418 /* Pattern Data Register 6 (RW) */
#define B3_PDR7                           0x00D2141C /* Pattern Data Register 7 (RW) */
#define B3_PDR8                           0x00D21420 /* Pattern Data Register 8 (RW) */
#define B3_PDR9                           0x00D21424 /* Pattern Data Register 9 (RW) */
#define B3_PDR10                          0x00D21428 /* Pattern Data Register 10 (RW) */
#define B3_PDR11                          0x00D2142C /* Pattern Data Register 11 (RW) */
#define B3_PDR12                          0x00D21430 /* Pattern Data Register 12 (RW) */
#define B3_PDR13                          0x00D21434 /* Pattern Data Register 13 (RW) */
#define B3_PDR14                          0x00D21438 /* Pattern Data Register 14 (RW) */
#define B3_PDR15                          0x00D2143C /* Pattern Data Register 15 (RW) */
#define B3_PDR16                          0x00D21440 /* Pattern Data Register 16 (RW) */
#define B3_PDR17                          0x00D21444 /* Pattern Data Register 17 (RW) */
#define B3_PDR18                          0x00D21448 /* Pattern Data Register 18 (RW) */
#define B3_PDR19                          0x00D2144C /* Pattern Data Register 19 (RW) */
#define B3_PDR20                          0x00D21450 /* Pattern Data Register 20 (RW) */
#define B3_PDR21                          0x00D21454 /* Pattern Data Register 21 (RW) */
#define B3_PDR22                          0x00D21458 /* Pattern Data Register 22 (RW) */
#define B3_PDR23                          0x00D2145C /* Pattern Data Register 23 (RW) */
#define B3_PDR24                          0x00D21460 /* Pattern Data Register 24 (RW) */
#define B3_PDR25                          0x00D21464 /* Pattern Data Register 25 (RW) */
#define B3_PDR26                          0x00D21468 /* Pattern Data Register 26 (RW) */
#define B3_PDR27                          0x00D2146C /* Pattern Data Register 27 (RW) */
#define B3_PDR28                          0x00D21470 /* Pattern Data Register 28 (RW) */
#define B3_PDR29                          0x00D21474 /* Pattern Data Register 29 (RW) */
#define B3_PDR30                          0x00D21478 /* Pattern Data Register 30 (RW) */
#define B3_PDR31                          0x00D2147C /* Pattern Data Register 31 (RW) */
#define B3_PDR32                          0x00D21480 /* Pattern Data Register 32 (RW) */
#define B3_PDR33                          0x00D21484 /* Pattern Data Register 33 (RW) */
#define B3_PDR34                          0x00D21488 /* Pattern Data Register 34 (RW) */
#define B3_PDR35                          0x00D2148C /* Pattern Data Register 35 (RW) */
#define B3_PDR36                          0x00D21490 /* Pattern Data Register 36 (RW) */
#define B3_PDR37                          0x00D21494 /* Pattern Data Register 37 (RW) */
#define B3_PDR38                          0x00D21498 /* Pattern Data Register 38 (RW) */
#define B3_PDR39                          0x00D2149C /* Pattern Data Register 39 (RW) */
#define B3_PDR40                          0x00D214A0 /* Pattern Data Register 40 (RW) */
#define B3_PDR41                          0x00D214A4 /* Pattern Data Register 41 (RW) */
#define B3_PDR42                          0x00D214A8 /* Pattern Data Register 42 (RW) */
#define B3_PDR43                          0x00D214AC /* Pattern Data Register 43 (RW) */
#define B3_PDR44                          0x00D214B0 /* Pattern Data Register 44 (RW) */
#define B3_PDR45                          0x00D214B4 /* Pattern Data Register 45 (RW) */
#define B3_PDR46                          0x00D214B8 /* Pattern Data Register 46 (RW) */
#define B3_PDR47                          0x00D214BC /* Pattern Data Register 47 (RW) */
#define B3_PDR48                          0x00D214C0 /* Pattern Data Register 48 (RW) */
#define B3_PDR49                          0x00D214C4 /* Pattern Data Register 49 (RW) */
#define B3_PDR50                          0x00D214C8 /* Pattern Data Register 50 (RW) */
#define B3_PDR51                          0x00D214CC /* Pattern Data Register 51 (RW) */
#define B3_PDR52                          0x00D214D0 /* Pattern Data Register 52 (RW) */
#define B3_PDR53                          0x00D214D4 /* Pattern Data Register 53 (RW) */
#define B3_PDR54                          0x00D214D8 /* Pattern Data Register 54 (RW) */
#define B3_PDR55                          0x00D214DC /* Pattern Data Register 55 (RW) */
#define B3_PDR56                          0x00D214E0 /* Pattern Data Register 56 (RW) */
#define B3_PDR57                          0x00D214E4 /* Pattern Data Register 57 (RW) */
#define B3_PDR58                          0x00D214E8 /* Pattern Data Register 58 (RW) */
#define B3_PDR59                          0x00D214EC /* Pattern Data Register 59 (RW) */
#define B3_PDR60                          0x00D214F0 /* Pattern Data Register 60 (RW) */
#define B3_PDR61                          0x00D214F4 /* Pattern Data Register 61 (RW) */
#define B3_PDR62                          0x00D214F8 /* Pattern Data Register 62 (RW) */
#define B3_PDR63                          0x00D214FC /* Pattern Data Register 63 (RW) */
#define B3_VTR_MC0                        0x00D21800 /* Visibility Test Statistics MC0 (R) */
#define B3_VTR_MC1                        0x00D21804 /* Visibility Test Statistics MC1 (R) */
#define B3_VTR_MC2                        0x00D21808 /* Visibility Test Statistics MC2 (R) */
#define B3_VTR_MC3                        0x00D2180C /* Visibility Test Statistics MC3 (R) */
#define B3_VPCNT_MC0                      0x00D21900 /* Visibility Count Pass Register MC0 (R) */
#define B3_VPCNT_MC1                      0x00D21904 /* Visibility Count Pass Register MC1 (R) */
#define B3_VPCNT_MC2                      0x00D21908 /* Visibility Count Pass Register MC2 (R) */
#define B3_VPCNT_MC3                      0x00D2190C /* Visibility Count Pass Register MC3 (R) */
#define B3_VFCNT_MC0                      0x00D21A00 /* Visibility Count Fail Register MC0 (R) */
#define B3_VFCNT_MC1                      0x00D21A04 /* Visibility Count Fail Register MC1 (R) */
#define B3_VFCNT_MC2                      0x00D21A08 /* Visibility Count Fail Register MC2 (R) */
#define B3_VFCNT_MC3                      0x00D21A0C /* Visibility Count Fail Register  MC3 (R) */
#define B3_FPR                            0x00D22008 /* FBC Parameters Register (R) */
#define B3_FBC_PERF                       0x00D2200C /* FBC Performance Register (RW) */
#define B3_FCR                            0x00D22010 /* FBC CRC Read Register (R) */
#define B3_FCI                            0x00D22014 /* FBC CRC Initiate Register (W) */
#define B3_FCM                            0x00D22018 /* FBC CRC Mask Register (W) */
#define B3_FBC_STATUS                     0x00D2201C /* FBC Status Register (W) */
#define B3_FBC_CRC1U                      0x00D22020 /* FBC Ridgeline CRC Phase 1 Upper Register (W) */
#define B3_FBC_CRC1                       0x00D22024 /* FBC Ridgeline CRC Phase 1 Lower Register (W) */
#define B3_FBC_CRC2U                      0x00D22028 /* FBC Ridgeline CRC Phase 2 Upper Register (W) */
#define B3_FBC_CRC2                       0x00D2202C /* FBC Ridgeline CRC Phase 2 Lower Register (W) */
/*  End of  Misc FBC Non-Shadowed registers  */

/*  Start of  FBC Front-End Specific Registers  */
#define B3_FE_BMAP_BABoth                 0x00D28000 /* Reads of FE's copy of Shared BMAP_DBA (R) */
#define B3_FE_BMAP_DBA                    0x00D28004 /* Reads of FE's copy of Shared BMAP_DBA (R) */
#define B3_FE_BMAP_SBA                    0x00D28008 /* Reads of FE's copy of Shared BMAP_SBA (R) */
#define B3_FE_CPE                         0x00D2800C /* Reads of FE's copy of Shared CPE (R) */
/*  End of  FBC Front-End Specific Registers  */

/*  Start of  FBC Fragment Processor Specific Registers  */
#define B3_FBC_FP_WORG                    0x00D29000 /* FBC/FP read address for WORG (R) */
#define B3_FBC_FP_EN2D                    0x00D29004 /* FBC/FP read address for EN2D (R) */
#define B3_FBC_FP_DBA                     0x00D29008 /* FBC/FP read address for DBA (R) */
#define B3_FBC_FP_RMR                     0x00D2900C /* FBC/FP read address for RMR (R) */
#define B3_FBC_FP_CMD                     0x00D29010 /* FBC/FP read address for CMD (R) */
#define B3_FBC_FP_BMAP_DBA                0x00D29014 /* FBC/FP read address for BMAP_DBA (R) */
/*  End of  FBC Fragment Processor Specific Registers  */

/*  Start of  FBC Tile Builder Specific Registers  */
#define B3_TB_BMAP_BABoth                 0x00D2A000 /* Reads of TB's copy of Shared BMAP_DBA (R) */
#define B3_TB_BMAP_DBA                    0x00D2A004 /* Reads of TB's copy of Shared BMAP_DBA (R) */
#define B3_TB_BMAP_SBA                    0x00D2A008 /* Reads of TB's copy of Shared BMAP_SBA (R) */
#define B3_FBC_TB_REN                     0x00D2A00C /* FBC/TB read address for FBC_SREN (R) */
#define B3_FBC_TB_DBA                     0x00D2A010 /* FBC/TB read address for DBA (R) */
#define B3_FBC_TB_SBA                     0x00D2A014 /* FBC/TB read address for SBA (R) */
#define B3_FBC_TB_WEN                     0x00D2A018 /* FBC/TB read address for FBC_SWEN (R) */
/*  End of  FBC Tile Builder Specific Registers  */

/*  Start of  FBC Memory Controller Specific Registers  */
#define B3_MC_BABoth                      0x00D2B804 /* DBA & SBA (reads return DBA) (R) */
#define B3_MC_DBA                         0x00D2B808 /* Destination Bitmap Access Register (R) */
#define B3_MC_BMAP_BABoth                 0x00D2BC00 /* BMAP DBA and SBA (reads return BMAP_DBA) (R) */
#define B3_MC_BMAP_DBA                    0x00D2BC04 /* BMAP Destination Bitmap Access register (R) */
/*  End of  FBC Memory Controller Specific Registers  */

/*  Start of  FBC Object Builder Specific Registers  */
/*  End of  FBC Object Builder Specific Registers  */

/*  End of FBC Registers  */

/*  Start of Imaging Registers  */
/*  End of Imaging Registers  */

/*  Start of Imaging Registers  */
/*  End of Imaging Registers  */

/*  Start of Imaging Registers  */
/*  End of Imaging Registers  */

/*  Start of Imaging Registers  */
/*  End of Imaging Registers  */

/*  Start of Imaging Registers  */
/*  End of Imaging Registers  */

/*  Start of Volumetric Registers  */
/*  End of Volumetric Registers  */

/*  Start of Volumetric Registers  */
/*  End of Volumetric Registers  */

/*  Start of Volumetric Registers  */
/*  End of Volumetric Registers  */

/*  Start of Volumetric Registers  */
#define B2_VOL_BABoth                     0x009A0804 /* DBA & SBA (reads return DBA) (RW) */
#define B2_VOL_DBA                        0x009A0808 /* Destination Bitmap Access Register (RW) */
#define B2_VOL_SBA                        0x009A080C /* Source Bitmap Access Register (RW) */
#define B2_VOL_DSA                        0x009A0810 /* Destination Screen Address Register (RW) */
#define B2_VOL_SSA                        0x009A0814 /* Source Screen Address Register (RW) */
#define B2_VOL_BPDU                       0x009A0820 /* Bitmap Pixel Data Register (Upper Half) (RW) */
#define B2_VOL_BPD                        0x009A0824 /* Bitmap Pixel Data Register (RW) */
#define B2_VOL_BPMU                       0x009A0828 /* Bitmap Pixel Mask Register (Upper Half) (RW) */
#define B2_VOL_BPM                        0x009A082C /* Bitmap Pixel Mask Register (RW) */
#define B2_VOL_BPCU                       0x009A0830 /* Bitmap Pixel Clip Register (Upper Half) (RW) */
#define B2_VOL_BPC                        0x009A0834 /* Bitmap Pixel Clip Register (RW) */
#define B2_VOL_IFCU                       0x009A0838 /* Image Foreground Color Register (Upper Half) (RW) */
#define B2_VOL_IFC                        0x009A083C /* Image Foreground Color Register (RW) */
#define B2_VOL_IBCU                       0x009A0840 /* Image Background Color Register (Upper Half) (RW) */
#define B2_VOL_IBC                        0x009A0844 /* Image Background Color Register (RW) */
#define B2_VOL_IPMU                       0x009A0848 /* Image Plane Mask Register (Upper Half) (RW) */
#define B2_VOL_IPM                        0x009A084C /* Image Plane Mask Register (RW) */
#define B2_VOL_FLN                        0x009A0854 /* Fill Length (in X) (RW) */
#define B2_VOL_RPH                        0x009A085C /* Read Prefetch Hint Register (R) */
#define B2_VOL_BLN                        0x009A0880 /* BLT Length (in X) (RW) */
/*  End of Volumetric Registers  */

/*  Start of Volumetric Registers  */
#define B3_VOL_BABoth                     0x00DA0804 /* DBA & SBA (reads return DBA) (RW) */
#define B3_VOL_DBA                        0x00DA0808 /* Destination Bitmap Access Register (RW) */
#define B3_VOL_SBA                        0x00DA080C /* Source Bitmap Access Register (RW) */
#define B3_VOL_DSA                        0x00DA0810 /* Destination Screen Address Register (RW) */
#define B3_VOL_SSA                        0x00DA0814 /* Source Screen Address Register (RW) */
#define B3_VOL_BPDU                       0x00DA0820 /* Bitmap Pixel Data Register (Upper Half) (RW) */
#define B3_VOL_BPD                        0x00DA0824 /* Bitmap Pixel Data Register (RW) */
#define B3_VOL_BPMU                       0x00DA0828 /* Bitmap Pixel Mask Register (Upper Half) (RW) */
#define B3_VOL_BPM                        0x00DA082C /* Bitmap Pixel Mask Register (RW) */
#define B3_VOL_BPCU                       0x00DA0830 /* Bitmap Pixel Clip Register (Upper Half) (RW) */
#define B3_VOL_BPC                        0x00DA0834 /* Bitmap Pixel Clip Register (RW) */
#define B3_VOL_IFCU                       0x00DA0838 /* Image Foreground Color Register (Upper Half) (RW) */
#define B3_VOL_IFC                        0x00DA083C /* Image Foreground Color Register (RW) */
#define B3_VOL_IBCU                       0x00DA0840 /* Image Background Color Register (Upper Half) (RW) */
#define B3_VOL_IBC                        0x00DA0844 /* Image Background Color Register (RW) */
#define B3_VOL_IPMU                       0x00DA0848 /* Image Plane Mask Register (Upper Half) (RW) */
#define B3_VOL_IPM                        0x00DA084C /* Image Plane Mask Register (RW) */
#define B3_VOL_FLN                        0x00DA0854 /* Fill Length (in X) (RW) */
#define B3_VOL_RPH                        0x00DA085C /* Read Prefetch Hint Register (R) */
#define B3_VOL_BLN                        0x00DA0880 /* BLT Length (in X) (RW) */
/*  End of Volumetric Registers  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of 3D Retargeted Registers  */
/*  End of 3D Retargeted Registers  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of 2D Retargeted Registers  */
#define B2_RTG_MEM_USE                    0x009C2000 /* Start Of Retargetter Memory (For Normal Use) (RW) */
/*  End Of Retargetter Memory (For Normal Use)  */

#define B2_RTG_MEM_LOAD                   0x009C3000 /* Start Of Retargetter Memory (For Loading Memory) (RW) */
/*  End Of Retargetter Memory (For Loading Memory)  */

#define B2_RTG_AREG0                      0x009C4000 /* Retargetter Address Register 0 (RW) */
#define B2_RTG_AREG1                      0x009C4004 /* Retargetter Address Register 1 (RW) */
#define B2_RTG_AREG2                      0x009C4008 /* Retargetter Address Register 2 (RW) */
#define B2_RTG_AREG3                      0x009C400C /* Retargetter Address Register 3 (RW) */
#define B2_RTG_AREG4                      0x009C4010 /* Retargetter Address Register 4 (RW) */
#define B2_RTG_AREG5                      0x009C4014 /* Retargetter Address Register 5 (RW) */
#define B2_RTG_AREG6                      0x009C4018 /* Retargetter Address Register 6 (RW) */
#define B2_RTG_AREG7                      0x009C401C /* Retargetter Address Register 7 (RW) */
#define B2_RTG_DREG0                      0x009C4020 /* Retargetter Data Register 0 (RW) */
#define B2_RTG_DREG1                      0x009C4024 /* Retargetter Data Register 1 (RW) */
#define B2_RTG_DREG2                      0x009C4028 /* Retargetter Data Register 2 (RW) */
#define B2_RTG_DREG3                      0x009C402C /* Retargetter Data Register 3 (RW) */
#define B2_RTG_DREG4                      0x009C4030 /* Retargetter Data Register 4 (RW) */
#define B2_RTG_DREG5                      0x009C4034 /* Retargetter Data Register 5 (RW) */
#define B2_RTG_DREG6                      0x009C4038 /* Retargetter Data Register 6 (RW) */
#define B2_RTG_DREG7                      0x009C403C /* Retargetter Data Register 7 (RW) */
/*  End of 2D Retargeted Registers  */

/*  Start of 3D Retargeted Registers  */
#define B3_RTG_MEM_USE                    0x00DC2000 /* Start Of Retargetter Memory (For Normal Use) (RW) */
/*  End Of Retargetter Memory (For Normal Use)  */

#define B3_RTG_MEM_LOAD                   0x00DC3000 /* Start Of Retargetter Memory (For Loading Memory) (RW) */
/*  End Of Retargetter Memory (For Loading Memory)  */

#define B3_RTG_AREG0                      0x00DC4000 /* Retargetter Address Register 0 (RW) */
#define B3_RTG_AREG1                      0x00DC4004 /* Retargetter Address Register 1 (RW) */
#define B3_RTG_AREG2                      0x00DC4008 /* Retargetter Address Register 2 (RW) */
#define B3_RTG_AREG3                      0x00DC400C /* Retargetter Address Register 3 (RW) */
#define B3_RTG_AREG4                      0x00DC4010 /* Retargetter Address Register 4 (RW) */
#define B3_RTG_AREG5                      0x00DC4014 /* Retargetter Address Register 5 (RW) */
#define B3_RTG_AREG6                      0x00DC4018 /* Retargetter Address Register 6 (RW) */
#define B3_RTG_AREG7                      0x00DC401C /* Retargetter Address Register 7 (RW) */
#define B3_RTG_DREG0                      0x00DC4020 /* Retargetter Data Register 0 (RW) */
#define B3_RTG_DREG1                      0x00DC4024 /* Retargetter Data Register 1 (RW) */
#define B3_RTG_DREG2                      0x00DC4028 /* Retargetter Data Register 2 (RW) */
#define B3_RTG_DREG3                      0x00DC402C /* Retargetter Data Register 3 (RW) */
#define B3_RTG_DREG4                      0x00DC4030 /* Retargetter Data Register 4 (RW) */
#define B3_RTG_DREG5                      0x00DC4034 /* Retargetter Data Register 5 (RW) */
#define B3_RTG_DREG6                      0x00DC4038 /* Retargetter Data Register 6 (RW) */
#define B3_RTG_DREG7                      0x00DC403C /* Retargetter Data Register 7 (RW) */
/*  End of 3D Retargeted Registers  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  Start of Shared Registers  */

#define UB_UBABoth                        0x00600C04 /* UDBA & USBA (W) */
#define UB_UDBA                           0x00600C08 /* Unbuffered Destination Bitmap Access Register (W) */
#define UB_USBA                           0x00600C0C /* Unbuffered Source Bitmap Access Register (W) */
#define UB_UDSA                           0x00600C10 /* Unbuffered Destination Screen Address Register (W) */
#define UB_USSA                           0x00600C14 /* Unbuffered Source Screen Address Register (W) */
#define UB_UBPDU                          0x00600C20 /* Unbuffered Bitmap Pixel Data Register (Upper Half) (W) */
#define UB_UBPD                           0x00600C24 /* Unbuffered Bitmap Pixel Data Register (W) */
#define UB_UBPMU                          0x00600C28 /* Unbuffered Bitmap Pixel Mask Register (Upper Half) (W) */
#define UB_UBPM                           0x00600C2C /* Unbuffered Bitmap Pixel Mask Register (W) */
#define UB_UBPCU                          0x00600C30 /* Unbuffered Bitmap Pixel Clip Register (Upper Half) (W) */
#define UB_UBPC                           0x00600C34 /* Unbuffered Bitmap Pixel Clip Register (W) */
#define UB_URPH                           0x00600C5C /* Unbuffered Read Prefetch Hint Register (W) */
#define UB_UREN                           0x00600C8C /* Unbuffered Read Enable Register (W) */
/*  Start of Shared Registers  */
#define B2_S0_Start_Int                   0x00A00000 /*  (W) */
#define B2_S0_Start_Frac                  0x00A00004 /*  (W) */
#define B2_T0_Start_Int                   0x00A00008 /*  (W) */
#define B2_T0_Start_Frac                  0x00A0000C /*  (W) */
#define B2_R_Start_Int                    0x00A00010 /*  (W) */
#define B2_R_Start_Frac                   0x00A00014 /*  (W) */
#define B2_Gx0_Start_Int                  0x00A00018 /*  (W) */
#define B2_Gx0_Start_Frac                 0x00A0001C /*  (W) */
#define B2_Gy0_Start_Int                  0x00A00020 /*  (W) */
#define B2_Gy0_Start_Frac                 0x00A00024 /*  (W) */
#define B2_QoW0_Start                     0x00A00028 /*  (W) */
#define B2_dS0_dX_Int                     0x00A00030 /*  (W) */
#define B2_dS0_dX_Frac                    0x00A00034 /*  (W) */
#define B2_dT0_dX_Int                     0x00A00038 /*  (W) */
#define B2_dT0_dX_Frac                    0x00A0003C /*  (W) */
#define B2_dR_dX_Int                      0x00A00040 /*  (W) */
#define B2_dR_dX_Frac                     0x00A00044 /*  (W) */
#define B2_dGx0_dX_Int                    0x00A00048 /*  (W) */
#define B2_dGx0_dX_Frac                   0x00A0004C /*  (W) */
#define B2_dGy0_dX_Int                    0x00A00050 /*  (W) */
#define B2_dGy0_dX_Frac                   0x00A00054 /*  (W) */
#define B2_dQoW0_dX                       0x00A00058 /*  (W) */
#define B2_dS0_dE1_Int                    0x00A00060 /*  (W) */
#define B2_dS0_dE1_Frac                   0x00A00064 /*  (W) */
#define B2_dT0_dE1_Int                    0x00A00068 /*  (W) */
#define B2_dT0_dE1_Frac                   0x00A0006C /*  (W) */
#define B2_dR_dE1_Int                     0x00A00070 /*  (W) */
#define B2_dR_dE1_Frac                    0x00A00074 /*  (W) */
#define B2_dGx0_dE1_Int                   0x00A00078 /*  (W) */
#define B2_dGx0_dE1_Frac                  0x00A0007C /*  (W) */
#define B2_dGy0_dE1_Int                   0x00A00080 /*  (W) */
#define B2_dGy0_dE1_Frac                  0x00A00084 /*  (W) */
#define B2_dQoW0_dE1                      0x00A00088 /*  (W) */
#define B2_Edge1_X_Start                  0x00A000C0 /*  (W) */
#define B2_Edge2_X_Start                  0x00A000C4 /*  (W) */
#define B2_Edge3_X_Start                  0x00A000C8 /*  (W) */
#define B2_Major_X_Start                  0x00A000CC /*  (W) */
#define B2_Major_X_Stop                   0x00A000D0 /*  (W) */
#define B2_Edge1_Y_Start                  0x00A000E0 /*  (W) */
#define B2_Edge1_Y_Stop                   0x00A000E4 /*  (W) */
#define B2_Edge3_Y_Start                  0x00A000E8 /*  (W) */
#define B2_Minor_Y_Start                  0x00A000EC /*  (W) */
#define B2_Edge1_Slope                    0x00A00100 /*  (W) */
#define B2_Edge2_Slope                    0x00A00104 /*  (W) */
#define B2_Edge3_Slope                    0x00A00108 /*  (W) */
#define B2_CMD                            0x00A00120 /*  (W) */
#define B2_Rd_Start                       0x00A00140 /* Diffuse red start (W) */
#define B2_Gd_Start                       0x00A00144 /* Diffuse green start (W) */
#define B2_Bd_Start                       0x00A00148 /* Diffuse blue start (W) */
#define B2_Alpha_Start                    0x00A0014C /* Alpha start (W) */
#define B2_Rs_Start                       0x00A00150 /* Specular red start (W) */
#define B2_Gs_Start                       0x00A00154 /* Specular green start (W) */
#define B2_Bs_Start                       0x00A00158 /* Specular blue start (W) */
#define B2_Fog_Start                      0x00A0015C /* Fog start (W) */
#define B2_dRd_dX                         0x00A00160 /* Change in diffuse red with respect to X (W) */
#define B2_dGd_dX                         0x00A00164 /* Change in diffuse green with respect to X (W) */
#define B2_dBd_dX                         0x00A00168 /* Change in diffuse blue with respect to X (W) */
#define B2_dAlpha_dX                      0x00A0016C /* Change in alpha with respect to X (W) */
#define B2_dRs_dX                         0x00A00170 /* Change in specular red with respect to X (W) */
#define B2_dGs_dX                         0x00A00174 /* Change in specular green with respect to X (W) */
#define B2_dBs_dX                         0x00A00178 /* Change in specular blue with respect to X (W) */
#define B2_dFog_dX                        0x00A0017C /* Change in fog with respect to X (W) */
#define B2_dRd_dE1                        0x00A00180 /* Change in diffuse red with respect to edge 1 (W) */
#define B2_dGd_dE1                        0x00A00184 /* Change in diffuse green with respect to edge 1 (W) */
#define B2_dBd_dE1                        0x00A00188 /* Change in diffuse blue with respect to edge 1 (W) */
#define B2_dAlpha_dE1                     0x00A0018C /* Change in alpha with respect to edge 1 (W) */
#define B2_dRs_dE1                        0x00A00190 /* Change in specular red with respect to edge 1 (W) */
#define B2_dGs_dE1                        0x00A00194 /* Change in specular green with respect to edge 1 (W) */
#define B2_dBs_dE1                        0x00A00198 /* Change in specular blue with respect to edge 1 (W) */
#define B2_dFog_dE1                       0x00A0019C /* Change in fog with respect to edge 1 (W) */
#define B2_Z_Start                        0x00A001C0 /* Z start (W) */
#define B2_dZ_dX                          0x00A001C4 /* Change in Z with respect to X (W) */
#define B2_dZ_dE1                         0x00A001C8 /* Change in Z with respect to edge 1 (W) */
#define B2_FogExp_Start                   0x00A001D0 /* FogExp_Start (2.30 fixed point format) (W) */
#define B2_dFogExp_dX                     0x00A001D4 /* DfogExp_dX (2.30 fixed point format) (W) */
#define B2_dFogExp_dE1                    0x00A001D8 /* DfogExp_dE1 (2.30 fixed point format) (W) */
#define B2_S1_Start_Int                   0x00A00200 /*  (W) */
#define B2_S1_Start_Frac                  0x00A00204 /*  (W) */
#define B2_T1_Start_Int                   0x00A00208 /*  (W) */
#define B2_T1_Start_Frac                  0x00A0020C /*  (W) */
#define B2_Gx1_Start_Int                  0x00A00210 /*  (W) */
#define B2_Gx1_Start_Frac                 0x00A00214 /*  (W) */
#define B2_Gy1_Start_Int                  0x00A00218 /*  (W) */
#define B2_Gy1_Start_Frac                 0x00A0021C /*  (W) */
#define B2_QoW1_Start                     0x00A00220 /*  (W) */
#define B2_dS1_dX_Int                     0x00A00224 /*  (W) */
#define B2_dS1_dX_Frac                    0x00A00228 /*  (W) */
#define B2_dT1_dX_Int                     0x00A0022C /*  (W) */
#define B2_dT1_dX_Frac                    0x00A00230 /*  (W) */
#define B2_dGx1_dX_Int                    0x00A00234 /*  (W) */
#define B2_dGx1_dX_Frac                   0x00A00238 /*  (W) */
#define B2_dGy1_dX_Int                    0x00A0023C /*  (W) */
#define B2_dGy1_dX_Frac                   0x00A00240 /*  (W) */
#define B2_dQoW1_dX                       0x00A00244 /*  (W) */
#define B2_dS1_dE1_Int                    0x00A00248 /*  (W) */
#define B2_dS1_dE1_Frac                   0x00A0024C /*  (W) */
#define B2_dT1_dE1_Int                    0x00A00250 /*  (W) */
#define B2_dT1_dE1_Frac                   0x00A00254 /*  (W) */
#define B2_dGx1_dE1_Int                   0x00A00258 /*  (W) */
#define B2_dGx1_dE1_Frac                  0x00A0025C /*  (W) */
#define B2_dGy1_dE1_Int                   0x00A00260 /*  (W) */
#define B2_dGy1_dE1_Frac                  0x00A00264 /*  (W) */
#define B2_dQoW1_dE1                      0x00A00268 /*  (W) */
#define B2_FOEU                           0x00A00400 /* Fragment Operation Enable Register (Upper Half) (W) */
#define B2_FOE                            0x00A00404 /* Fragment Operation Enable Register (W) */
#define B2_TFO                            0x00A00408 /* Texture Fragment Operation Register (W) */
#define B2_RMR                            0x00A0040C /*  () */
#define B2_Point_X_Width                  0x00A00410 /*  () */
#define B2_Point_Y_Width                  0x00A00414 /*  () */
#define B2_Minor_Width                    0x00A00418 /*  () */
#define B2_DPSET                          0x00A0041C /*  () */
#define B2_DPSTEP                         0x00A00420 /*  () */

#define B2_BABoth                         0x00A00804 /* DBA & SBA (reads return DBA) (W) */
#define B2_DBA                            0x00A00808 /* Destination Bitmap Access Register (W) */
#define B2_SBA                            0x00A0080C /* Source Bitmap Access Register (W) */
#define B2_DSA                            0x00A00810 /* Destination Screen Address Register (W) */
#define B2_SSA                            0x00A00814 /* Source Screen Address Register (W) */
#define B2_WORG                           0x00A00818 /* Window Origin Register (W) */
#define B2_FBS                            0x00A0081C /* Front Buffer Select Register (W) */
#define B2_BPDU                           0x00A00820 /* Bitmap Pixel Data Register (Upper Half) (W) */
#define B2_BPD                            0x00A00824 /* Bitmap Pixel Data Register (W) */
#define B2_BPMU                           0x00A00828 /* Bitmap Pixel Mask Register (Upper Half) (W) */
#define B2_BPM                            0x00A0082C /* Bitmap Pixel Mask Register (W) */
#define B2_BPCU                           0x00A00830 /* Bitmap Pixel Clip Register (Upper Half) (W) */
#define B2_BPC                            0x00A00834 /* Bitmap Pixel Clip Register (W) */
#define B2_IFCU                           0x00A00838 /* Image Foreground Color Register (Uppef Half) (W) */
#define B2_IFC                            0x00A0083C /* Image Foreground Color Register (W) */
#define B2_IBCU                           0x00A00840 /* Image Background Color Register (Upper Half) (W) */
#define B2_IBC                            0x00A00844 /* Image Background Color Register (W) */
#define B2_IPMU                           0x00A00848 /* Image Plane Mask Register (Upper Half) (W) */
#define B2_IPM                            0x00A0084C /* Image Plane Mask Register (W) */
#define B2_MISC_CTL                       0x00A00850 /* FBC Miscellaneous Control Register (W) */
#define B2_FLN                            0x00A00854 /* Fill Length (in X) (W) */
#define B2_EN2D                           0x00A00858 /* Coordinate Change Register (W) */
#define B2_RPH                            0x00A0085C /* Read Prefetch Hint Register (W) */
#define B2_WCEU                           0x00A00868 /* Window Clip Enable Register (Upper Half) (W) */
#define B2_WCE                            0x00A0086C /* Window Clip Enable Register (W) */
#define B2_PCRU                           0x00A00870 /* Pattern Control Register (Upper Half) (W) */
#define B2_PCR                            0x00A00874 /* Pattern Control Register (W) */
#define B2_BLN                            0x00A00880 /* BLT Length (in X) (W) */
#define B2_SWEN                           0x00A00884 /* FBC Write Enable Register (W) */
#define B2_SREN                           0x00A00888 /* FBC Read Enable Register (W) */
#define B2_BMAP_BABoth                    0x00A008A0 /* Writes of Shared BMAP_DBA (W) */
#define B2_BMAP_DBA                       0x00A008A4 /* Writes of Shared BMAP_DBA (W) */
#define B2_BMAP_SBA                       0x00A008A8 /* Writes of Shared BMAP_SBA (W) */
#define B2_WUL0                           0x00A00900 /* Window Clip Upper Left 0 (W) */
#define B2_WLR0                           0x00A00904 /* Window Clip Lower Right 0 (W) */
#define B2_WUL1                           0x00A00908 /* Window Clip Upper Left 1 (W) */
#define B2_WLR1                           0x00A0090C /* Window Clip Lower Right 1 (W) */
#define B2_WUL2                           0x00A00910 /* Window Clip Upper Left 2 (W) */
#define B2_WLR2                           0x00A00914 /* Window Clip Lower Right 2 (W) */
#define B2_WUL3                           0x00A00918 /* Window Clip Upper Left 3 (W) */
#define B2_WLR3                           0x00A0091C /* Window Clip Lower Right 3 (W) */
#define B2_WUL4                           0x00A00920 /* Window Clip Upper Left 4 (W) */
#define B2_WLR4                           0x00A00924 /* Window Clip Lower Right 4 (W) */
#define B2_WUL5                           0x00A00928 /* Window Clip Upper Left 5 (W) */
#define B2_WLR5                           0x00A0092C /* Window Clip Lower Right 5 (W) */
#define B2_WUL6                           0x00A00930 /* Window Clip Upper Left 6 (W) */
#define B2_WLR6                           0x00A00934 /* Window Clip Lower Right 6 (W) */
#define B2_WUL7                           0x00A00938 /* Window Clip Upper Left 7 (W) */
#define B2_WLR7                           0x00A0093C /* Window Clip Lower Right 7 (W) */
/*  Start Of Reserved For Future Clip Windows  */
/*  End Of Reserved For Future Clip Windows  */

#define B2_SFPR                           0x00A02008 /* FBC Parameters Register (W) */
/*  End of Shared Registers  */

/*  Start of Shared Registers  */
#define B3_S0_Start_Int                   0x00E00000 /*  (W) */
#define B3_S0_Start_Frac                  0x00E00004 /*  (W) */
#define B3_T0_Start_Int                   0x00E00008 /*  (W) */
#define B3_T0_Start_Frac                  0x00E0000C /*  (W) */
#define B3_R_Start_Int                    0x00E00010 /*  (W) */
#define B3_R_Start_Frac                   0x00E00014 /*  (W) */
#define B3_Gx0_Start_Int                  0x00E00018 /*  (W) */
#define B3_Gx0_Start_Frac                 0x00E0001C /*  (W) */
#define B3_Gy0_Start_Int                  0x00E00020 /*  (W) */
#define B3_Gy0_Start_Frac                 0x00E00024 /*  (W) */
#define B3_QoW0_Start                     0x00E00028 /*  (W) */
#define B3_dS0_dX_Int                     0x00E00030 /*  (W) */
#define B3_dS0_dX_Frac                    0x00E00034 /*  (W) */
#define B3_dT0_dX_Int                     0x00E00038 /*  (W) */
#define B3_dT0_dX_Frac                    0x00E0003C /*  (W) */
#define B3_dR_dX_Int                      0x00E00040 /*  (W) */
#define B3_dR_dX_Frac                     0x00E00044 /*  (W) */
#define B3_dGx0_dX_Int                    0x00E00048 /*  (W) */
#define B3_dGx0_dX_Frac                   0x00E0004C /*  (W) */
#define B3_dGy0_dX_Int                    0x00E00050 /*  (W) */
#define B3_dGy0_dX_Frac                   0x00E00054 /*  (W) */
#define B3_dQoW0_dX                       0x00E00058 /*  (W) */
#define B3_dS0_dE1_Int                    0x00E00060 /*  (W) */
#define B3_dS0_dE1_Frac                   0x00E00064 /*  (W) */
#define B3_dT0_dE1_Int                    0x00E00068 /*  (W) */
#define B3_dT0_dE1_Frac                   0x00E0006C /*  (W) */
#define B3_dR_dE1_Int                     0x00E00070 /*  (W) */
#define B3_dR_dE1_Frac                    0x00E00074 /*  (W) */
#define B3_dGx0_dE1_Int                   0x00E00078 /*  (W) */
#define B3_dGx0_dE1_Frac                  0x00E0007C /*  (W) */
#define B3_dGy0_dE1_Int                   0x00E00080 /*  (W) */
#define B3_dGy0_dE1_Frac                  0x00E00084 /*  (W) */
#define B3_dQoW0_dE1                      0x00E00088 /*  (W) */
#define B3_Edge1_X_Start                  0x00E000C0 /*  (W) */
#define B3_Edge2_X_Start                  0x00E000C4 /*  (W) */
#define B3_Edge3_X_Start                  0x00E000C8 /*  (W) */
#define B3_Major_X_Start                  0x00E000CC /*  (W) */
#define B3_Major_X_Stop                   0x00E000D0 /*  (W) */
#define B3_Edge1_Y_Start                  0x00E000E0 /*  (W) */
#define B3_Edge1_Y_Stop                   0x00E000E4 /*  (W) */
#define B3_Edge3_Y_Start                  0x00E000E8 /*  (W) */
#define B3_Minor_Y_Start                  0x00E000EC /*  (W) */
#define B3_Edge1_Slope                    0x00E00100 /*  (W) */
#define B3_Edge2_Slope                    0x00E00104 /*  (W) */
#define B3_Edge3_Slope                    0x00E00108 /*  (W) */
#define B3_CMD                            0x00E00120 /*  (W) */
#define B3_Rd_Start                       0x00E00140 /* Diffuse red start (W) */
#define B3_Gd_Start                       0x00E00144 /* Diffuse green start (W) */
#define B3_Bd_Start                       0x00E00148 /* Diffuse blue start (W) */
#define B3_Alpha_Start                    0x00E0014C /* Alpha start (W) */
#define B3_Rs_Start                       0x00E00150 /* Specular red start (W) */
#define B3_Gs_Start                       0x00E00154 /* Specular green start (W) */
#define B3_Bs_Start                       0x00E00158 /* Specular blue start (W) */
#define B3_Fog_Start                      0x00E0015C /* Fog start (W) */
#define B3_dRd_dX                         0x00E00160 /* Change in diffuse red with respect to X (W) */
#define B3_dGd_dX                         0x00E00164 /* Change in diffuse green with respect to X (W) */
#define B3_dBd_dX                         0x00E00168 /* Change in diffuse blue with respect to X (W) */
#define B3_dAlpha_dX                      0x00E0016C /* Change in alpha with respect to X (W) */
#define B3_dRs_dX                         0x00E00170 /* Change in specular red with respect to X (W) */
#define B3_dGs_dX                         0x00E00174 /* Change in specular green with respect to X (W) */
#define B3_dBs_dX                         0x00E00178 /* Change in specular blue with respect to X (W) */
#define B3_dFog_dX                        0x00E0017C /* Change in fog with respect to X (W) */
#define B3_dRd_dE1                        0x00E00180 /* Change in diffuse red with respect to edge 1 (W) */
#define B3_dGd_dE1                        0x00E00184 /* Change in diffuse green with respect to edge 1 (W) */
#define B3_dBd_dE1                        0x00E00188 /* Change in diffuse blue with respect to edge 1 (W) */
#define B3_dAlpha_dE1                     0x00E0018C /* Change in alpha with respect to edge 1 (W) */
#define B3_dRs_dE1                        0x00E00190 /* Change in specular red with respect to edge 1 (W) */
#define B3_dGs_dE1                        0x00E00194 /* Change in specular green with respect to edge 1 (W) */
#define B3_dBs_dE1                        0x00E00198 /* Change in specular blue with respect to edge 1 (W) */
#define B3_dFog_dE1                       0x00E0019C /* Change in fog with respect to edge 1 (W) */
#define B3_Z_Start                        0x00E001C0 /* Z start (W) */
#define B3_dZ_dX                          0x00E001C4 /* Change in Z with respect to X (W) */
#define B3_dZ_dE1                         0x00E001C8 /* Change in Z with respect to edge 1 (W) */
#define B3_FogExp_Start                   0x00E001D0 /* FogExp_Start (2.30 fixed point format) (W) */
#define B3_dFogExp_dX                     0x00E001D4 /* DfogExp_dX (2.30 fixed point format) (W) */
#define B3_dFogExp_dE1                    0x00E001D8 /* DfogExp_dE1 (2.30 fixed point format) (W) */
#define B3_S1_Start_Int                   0x00E00200 /*  (W) */
#define B3_S1_Start_Frac                  0x00E00204 /*  (W) */
#define B3_T1_Start_Int                   0x00E00208 /*  (W) */
#define B3_T1_Start_Frac                  0x00E0020C /*  (W) */
#define B3_Gx1_Start_Int                  0x00E00210 /*  (W) */
#define B3_Gx1_Start_Frac                 0x00E00214 /*  (W) */
#define B3_Gy1_Start_Int                  0x00E00218 /*  (W) */
#define B3_Gy1_Start_Frac                 0x00E0021C /*  (W) */
#define B3_QoW1_Start                     0x00E00220 /*  (W) */
#define B3_dS1_dX_Int                     0x00E00224 /*  (W) */
#define B3_dS1_dX_Frac                    0x00E00228 /*  (W) */
#define B3_dT1_dX_Int                     0x00E0022C /*  (W) */
#define B3_dT1_dX_Frac                    0x00E00230 /*  (W) */
#define B3_dGx1_dX_Int                    0x00E00234 /*  (W) */
#define B3_dGx1_dX_Frac                   0x00E00238 /*  (W) */
#define B3_dGy1_dX_Int                    0x00E0023C /*  (W) */
#define B3_dGy1_dX_Frac                   0x00E00240 /*  (W) */
#define B3_dQoW1_dX                       0x00E00244 /*  (W) */
#define B3_dS1_dE1_Int                    0x00E00248 /*  (W) */
#define B3_dS1_dE1_Frac                   0x00E0024C /*  (W) */
#define B3_dT1_dE1_Int                    0x00E00250 /*  (W) */
#define B3_dT1_dE1_Frac                   0x00E00254 /*  (W) */
#define B3_dGx1_dE1_Int                   0x00E00258 /*  (W) */
#define B3_dGx1_dE1_Frac                  0x00E0025C /*  (W) */
#define B3_dGy1_dE1_Int                   0x00E00260 /*  (W) */
#define B3_dGy1_dE1_Frac                  0x00E00264 /*  (W) */
#define B3_dQoW1_dE1                      0x00E00268 /*  (W) */
#define B3_FOEU                           0x00E00400 /* Fragment Operation Enable Register (Upper Half) (W) */
#define B3_FOE                            0x00E00404 /* Fragment Operation Enable Register (W) */
#define B3_TFO                            0x00E00408 /* Texture Fragment Operation Register (W) */
#define B3_RMR                            0x00E0040C /*  () */
#define B3_Point_X_Width                  0x00E00410 /*  () */
#define B3_Point_Y_Width                  0x00E00414 /*  () */
#define B3_Minor_Width                    0x00E00418 /*  () */
#define B3_DPSET                          0x00E0041C /*  () */
#define B3_DPSTEP                         0x00E00420 /*  () */

#define B3_BABoth                         0x00E00804 /* DBA & SBA (reads return DBA) (W) */
#define B3_DBA                            0x00E00808 /* Destination Bitmap Access Register (W) */
#define B3_SBA                            0x00E0080C /* Source Bitmap Access Register (W) */
#define B3_DSA                            0x00E00810 /* Destination Screen Address Register (W) */
#define B3_SSA                            0x00E00814 /* Source Screen Address Register (W) */
#define B3_WORG                           0x00E00818 /* Window Origin Register (W) */
#define B3_FBS                            0x00E0081C /* Front Buffer Select Register (W) */
#define B3_BPDU                           0x00E00820 /* Bitmap Pixel Data Register (Upper Half) (W) */
#define B3_BPD                            0x00E00824 /* Bitmap Pixel Data Register (W) */
#define B3_BPMU                           0x00E00828 /* Bitmap Pixel Mask Register (Upper Half) (W) */
#define B3_BPM                            0x00E0082C /* Bitmap Pixel Mask Register (W) */
#define B3_BPCU                           0x00E00830 /* Bitmap Pixel Clip Register (Upper Half) (W) */
#define B3_BPC                            0x00E00834 /* Bitmap Pixel Clip Register (W) */
#define B3_IFCU                           0x00E00838 /* Image Foreground Color Register (Uppef Half) (W) */
#define B3_IFC                            0x00E0083C /* Image Foreground Color Register (W) */
#define B3_IBCU                           0x00E00840 /* Image Background Color Register (Upper Half) (W) */
#define B3_IBC                            0x00E00844 /* Image Background Color Register (W) */
#define B3_IPMU                           0x00E00848 /* Image Plane Mask Register (Upper Half) (W) */
#define B3_IPM                            0x00E0084C /* Image Plane Mask Register (W) */
#define B3_MISC_CTL                       0x00E00850 /* FBC Miscellaneous Control Register (W) */
#define B3_FLN                            0x00E00854 /* Fill Length (in X) (W) */
#define B3_EN2D                           0x00E00858 /* Coordinate Change Register (W) */
#define B3_WCEU                           0x00E00868 /* Window Clip Enable Register (Upper Half) (W) */
#define B3_WCE                            0x00E0086C /* Window Clip Enable Register (W) */
#define B3_PCRU                           0x00E00870 /* Pattern Control Register (Upper Half) (W) */
#define B3_PCR                            0x00E00874 /* Pattern Control Register (W) */
#define B3_BLN                            0x00E00880 /* BLT Length (in X) (W) */
#define B3_SWEN                           0x00E00884 /* FBC Write Enable Register (W) */
#define B3_SREN                           0x00E00888 /* FBC Read Enable Register (W) */
#define B3_BMAP_BABoth                    0x00E008A0 /* Writes of Shared BMAP_DBA (W) */
#define B3_BMAP_DBA                       0x00E008A4 /* Writes of Shared BMAP_DBA (W) */
#define B3_BMAP_SBA                       0x00E008A8 /* Writes of Shared BMAP_SBA (W) */
#define B3_WUL0                           0x00E00900 /* Window Clip Upper Left 0 (W) */
#define B3_WLR0                           0x00E00904 /* Window Clip Lower Right 0 (W) */
#define B3_WUL1                           0x00E00908 /* Window Clip Upper Left 1 (W) */
#define B3_WLR1                           0x00E0090C /* Window Clip Lower Right 1 (W) */
#define B3_WUL2                           0x00E00910 /* Window Clip Upper Left 2 (W) */
#define B3_WLR2                           0x00E00914 /* Window Clip Lower Right 2 (W) */
#define B3_WUL3                           0x00E00918 /* Window Clip Upper Left 3 (W) */
#define B3_WLR3                           0x00E0091C /* Window Clip Lower Right 3 (W) */
#define B3_WUL4                           0x00E00920 /* Window Clip Upper Left 4 (W) */
#define B3_WLR4                           0x00E00924 /* Window Clip Lower Right 4 (W) */
#define B3_WUL5                           0x00E00928 /* Window Clip Upper Left 5 (W) */
#define B3_WLR5                           0x00E0092C /* Window Clip Lower Right 5 (W) */
#define B3_WUL6                           0x00E00930 /* Window Clip Upper Left 6 (W) */
#define B3_WLR6                           0x00E00934 /* Window Clip Lower Right 6 (W) */
#define B3_WUL7                           0x00E00938 /* Window Clip Upper Left 7 (W) */
#define B3_WLR7                           0x00E0093C /* Window Clip Lower Right 7 (W) */
/*  Start Of Reserved For Future Clip Windows  */
/*  End Of Reserved For Future Clip Windows  */

#define B3_SFPR                           0x00E02008 /* FBC Parameters Register (W) */
/*  End of Shared Registers  */

/*  Start of Host Interface Registers  */
#define UP_CONTROL                        0x00249000 /* Control Register (RW) */
#define UP_FC                             0x00249040 /* Fault Control Register (RW) */
#define UP_TCP                            0x00249080 /* Time-out Circuit Parameter (RW) */
#define UP_DC                             0x002490C0 /* Dribble Count Register (RW) */
#define UP_STI_COUNT                      0x00249140 /* STI Count Register (RW) */
#define UP_PSTI_SCRATCH1                  0x00249180 /* Privileged STI Scratch Register 1 (RW) */
#define UP_PSTI_SCRATCH2                  0x002491A0 /* Privileged STI Scratch Register 2 (RW) */
#define UP_PSTI_SCRATCH3                  0x002491C0 /* Privileged STI Scratch Register 3 (RW) */
#define UP_PSTI_SCRATCH4                  0x002491E0 /* Privileged STI Scratch Register 4 (RW) */
#define UP_INT_ADDR                       0x00249200 /* Interrupt Address Register (RW) */
#define UP_INT_DATA                       0x00249240 /* Interrupt Data Register (RW) */
#define UP_STI_ADDR                       0x00249280 /* STI Write Address Register (W) */
#define UP_STI_DATA                       0x002492C0 /* STI Write Data Register (W) */
#define UP_VBS_CTL                        0x00249300 /* VBS Control Register (-) */
#define UP_AGP_UDSA                       0x002499C0 /* TM AGP DMA UDSA (-) */
#define UP_AGP_UDMA_UPPER                 0x00249A00 /* TM Reader Address (-) */
#define UP_AGP_UDMA_LOWER                 0x00249A40 /* TM Reader Address (-) */
#define UP_AGP_UDMA_OTC_STRIDE            0x00249A80 /* AGP DMA Stride (-) */
#define UP_AGP_UDMA_HIGH_WIDE             0x00249AC0 /* TM AGP DMA Read Hint (-) */
#define UP_AGP_DIAG0                      0x00249B00 /* TBD (-) */
#define UP_AGP_DIAG1                      0x00249B40 /* TBD (-) */
#define UP_AGP_DIAG2                      0x00249B80 /* TBD (-) */
#define UP_AGP_DIAG3                      0x00249BC0 /* TBD (-) */


























#define UP_CD_STATE1                      0x0024B000 /* CD Buffer State Register One (RW) */
#define UP_CD_STATE2                      0x0024B004 /* CD Buffer State Register Two (RW) */
#define UP_CD_DATA                        0x0024B008 /* CD Buffer Data Register (RW) */
#define UP_CF_STATE                       0x0024B00C /* Command Fifo State Register (RW) */
#define UP_CF_DATA                        0x0024B010 /* Command Fifo Data Register (RW) */
#define UP_CD_HWC                         0x0024B014 /* CD Buffer High Water Count Register (R) */
#define UP_CD_LWC                         0x0024B018 /* CD Buffer Low Water Count Register (R) */
#define UP_CF_HWC                         0x0024B01C /* Command Fifo High Water Count Register (R) */
#define UP_CF_LWC                         0x0024B020 /* Command Fifo Low Water Count Register (R) */
#define UP_CD_DRC                         0x0024B024 /* CD Buffer Dribble Count Register (R) */
#define UP_CF_DRC                         0x0024B028 /* Command FIFO Dribble Count Register (R) */





/*  Start Reserved (PQD)  */
/*  End Reserved (PQD)  */



















/*  End of Host Interface Registers  */

/*  Start of Host Interface Registers  */
#define BP_CONTROL                        0x002C9000 /* Control Register (RW) */
#define BP_FC                             0x002C9040 /* Fault Control Register (RW) */





























#define BP_AGP_BDMA_UPPER                 0x002C9900 /* CD Buffer Reader Address (-) */
#define BP_AGP_BDMA_LOWER                 0x002C9940 /* CD Buffer Reader Address (-) */
#define BP_AGP_BDMA_OFFSET                0x002C9980 /* CD Buffer DMA Request Queue (-) */













#define BP_CD_HWC                         0x002CB014 /* CD Buffer High Water Count Register (RW) */
#define BP_CD_LWC                         0x002CB018 /* CD Buffer Low Water Count Register (RW) */
#define BP_CF_HWC                         0x002CB01C /* Command Fifo High Water Count Register (RW) */
#define BP_CF_LWC                         0x002CB020 /* Command Fifo Low Water Count Register (RW) */
#define BP_CD_DRC                         0x002CB024 /* CD Buffer Dribble Count Register (RW) */
#define BP_CF_DRC                         0x002CB028 /* Command FIFO Dribble Count Register (RW) */





/*  Start Reserved (PQD)  */
/*  End Reserved (PQD)  */

#define BP_ICR                            0x002CB800 /* Integrator Counter Register (RW) */

















/*  End of Host Interface Registers  */

/*  Start of Host Interface Registers  */
#define UB_CONTROL                        0x00641000 /* Control Register (RW) */
#define UB_FC                             0x00641040 /* Fault Control Register (R) */
#define UB_TCP                            0x00641080 /* Time-out Circuit Parameter Register (R) */
#define UB_DC                             0x006410C0 /* Dribble Count Register (R) */
#define UB_AGP_FFC                        0x00641100 /* AGP DMA Queue State (R) */
#define UB_STI_COUNT                      0x00641140 /* STI Count Register (R) */
#define UB_PSTI_SCRATCH1                  0x00641180 /* Privileged STI Scratch Register 1 (R) */
#define UB_PSTI_SCRATCH2                  0x006411A0 /* Privileged STI Scratch Register 2 (R) */
#define UB_PSTI_SCRATCH3                  0x006411C0 /* Privileged STI Scratch Register 3 (R) */
#define UB_PSTI_SCRATCH4                  0x006411E0 /* Privileged STI Scratch Register 4 (R) */
#define UB_INT_ADDR                       0x00641200 /* Interrupt Address Register (R) */
#define UB_INT_DATA                       0x00641240 /* Interrupt Data Register (R) */
#define UB_STATUS                         0x00641400 /* Status Register (R) */
#define UB_FFC                            0x00641440 /* Fifo Free Count Register (R) */
#define UB_UREAD_DATA                     0x006414C0 /* Unbuffered Read Data Register (R) */

#define UB_UIRP                           0x00641540 /* Unbuffered Indirect Read Purge Register (W) */

/*
*  The UB_CLIP_PLANE_STAMP is actually called the UB_STI_SCRATCH1 register in
*  the lego ERS. We needed a place to pass information about when the clip
*  plane had changed. The hardware seemed a reasonable choice, so we looked for
*  an unused 32 bit register. We selected UB_STI_SCRATCH1 because it was on
*  the unbuffered path and because it was not being used by STI ROM.
*/
#define UB_CLIP_PLANE_STAMP               0x00641580 /* STI Scratch 1/Clip Plane Stamp (RW) */
#define UB_STI_SCRATCH1                   0x00641580 /* STI Scratch Register 1 (RW) */
#define UB_STI_SCRATCH2                   0x006415A0 /* STI Scratch Register 2 (RW) */
#define UB_STI_SCRATCH3                   0x006415C0 /* STI Scratch Register 3 (RW) */
#define UB_STI_SCRATCH4                   0x006415E0 /* STI Scratch Register 4 (RW) */
#define UB_SIG_LOWER_LSB                  0x00641700 /* Lower Write Data Signature LSB's (RW) */
#define UB_SIG_LOWER_MSB                  0x00641740 /* Lower Write Data Signature MSB's (RW) */
#define UB_SIG_UPPER_LSB                  0x00641780 /* Upper Write Data Signature LSB's (RW) */
#define UB_SIG_UPPER_MSB                  0x006417C0 /* Upper Write Data Signature MSB's (RW) */
#define UB_TRI_STATUS                     0x00641800 /* Triaqua-specific Status Register (R) */
#define UB_INT_PENDING                    0x00641840 /* Interrupt Pending Register (Diagnostics) (R) */
#define UB_DREAD_ADDR                     0x00641880 /* Direct Read Address Register (Diagnostics) (R) */
#define UB_VREAD_ADDR                     0x006418C0 /* ROM Read Address Register (Diagnostics) (R) */
#define UB_AGP_UDSA                       0x006499C0 /* TM AGP DMA UDSA (-) */
#define UB_AGP_UDMA_UPPER                 0x00649A00 /* TM Reader Address (-) */
#define UB_AGP_UDMA_LOWER                 0x00649A40 /* TM Reader Address (-) */
#define UB_AGP_UDMA_OTC_STRIDE            0x00649A80 /* AGP DMA Stride (-) */
#define UB_AGP_UDMA_HIGH_WIDE             0x00649AC0 /* TM AGP DMA Read Hint (-) */
#define UB_AGP_DIAG0                      0x00649B00 /* TBD (-) */
#define UB_AGP_DIAG1                      0x00649B40 /* TBD (-) */
#define UB_AGP_DIAG2                      0x00649B80 /* TBD (-) */
#define UB_AGP_DIAG3                      0x00649BC0 /* TBD (-) */
#define UB_DEVID                          0x00641C00 /* PCI Device ID Register (R) */
#define UB_COM_STAT                       0x00641C04 /* PCI Command And Status Register (R) */
#define UB_REVID                          0x00641C08 /* PCI Revision ID Register (R) */
#define UB_LT                             0x00641C0C /* PCI Latency Timer Register (R) */
#define UB_MBA_LOWER                      0x00641C10 /* PCI Memory Base Address Register [31:0] (R) */
#define UB_MBA_UPPER                      0x00641C14 /* PCI Memory Base Address Register [63:32] (R) */
#define UB_FBMBA_LOWER                    0x00641C18 /* Direct FB Access (R) */
#define UB_FBMBA_UPPER                    0x00641C1C /* Direct FB Access (R) */
#define UB_SUBID                          0x00641C2C /* Subvendor ID (R) */
#define UB_RBA                            0x00641C30 /* PCI Expansion ROM Base Address Register (R) */
#define UB_AGP_CAPPTR                     0x00641C34 /* PCI New Capability (R) */
#define UB_LAT_GNT                        0x00641C3C /* PCI Minimum Grant And Maximum Latency Register (R) */
#define UB_AGP_CAPID                      0x00641C40 /* AGP Capabilities ID (R) */
#define UB_AGP_STATUS                     0x00641C44 /* AGP Capabilities List (R) */
#define UB_AGP_CMD                        0x00641C48 /* AGP Capabilities Enables (R) */
#define UB_PWR_CAPID                      0x00641C4C /* Power Management (R) */
#define UB__PWR_CMD                       0x00641C50 /* Power Management (R) */











#define UB_IRC                            0x00643030 /* Buffered Indirect Read Control Register (R) */
#define UB_UIRC                           0x00643034 /* Unbuffered Indirect Read Control Register (RW) */
#define UB_CD_BUFFER_CTL                  0x00643038 /* CD Buffer Control Register (R) */
#define UB_PDU_UBSCFB                     0x0064303C /* PDU Unbuffered Byte Swap Ctl Reg (frame buffer space) (RW) */
#define UB_PQC                            0x00643040 /* Packet Queue Control Register (R) */
#define UB_PQD                            0x00643100 /* Start Packet Queue Data Register (R) */
/*  End Packet Queue Data Register  */


#define UB_CCU_CTL                        0x00643C00 /* CCU Control Register (RW) */
#define UB_TRI_RLO_P1_0                   0x00643C20 /* Unbuffered Ridgeline Out Signature Phase 1, [31:0] (RW) */
#define UB_TRI_RLO_P1_1                   0x00643C24 /* Unbuffered Ridgeline Out Signature Phase 1, [63:32] (RW) */
#define UB_TRI_RLO_P2_0                   0x00643C28 /* Unbuffered Ridgeline Out Signature Phase 2, [31:0] (RW) */
#define UB_TRI_RLO_P2_1                   0x00643C2C /* Unbuffered Ridgeline Out Signature Phase 2, [63:32] (RW) */




#define UB_TRI_RLI_P1_0                   0x00643E20 /* Unbuffered Ridgeline In Signature Phase 1, [31:0] (RW) */
#define UB_TRI_RLI_P1_1                   0x00643E24 /* Unbuffered Ridgeline In Signature Phase 1, [63:32] (RW) */
#define UB_TRI_RLI_P2_0                   0x00643E28 /* Unbuffered Ridgeline In Signature Phase 2, [31:0] (RW) */
#define UB_TRI_RLI_P2_1                   0x00643E2C /* Unbuffered Ridgeline In Signature Phase 2, [63:32] (RW) */




/*  End of Host Interface Registers  */

/*  Start of Host Interface Address Space  */
#define B2_CONTROL                        0x00A41000 /* Control Register (RW) */
#define B2_FC                             0x00A41040 /* Fault Control Register (R) */









#define B2_STATUS                         0x00A41400 /* Status Register (R) */
#define B2_FFC                            0x00A41440 /* Fifo Free Count Register (R) */
#define B2_READ_DATA                      0x00A41480 /* Buffered Read Data Register (R) */

#define B2_IRP                            0x00A41500 /* Buffered Indirect Read Purge Register (W) */













#define B2_AGP_BDMA_UPPER                 0x00A41900 /* CD Buffer Reads (-) */
#define B2_AGP_BDMA_LOWER                 0x00A41940 /* CD Buffer Reads (-) */
#define B2_AGP_BDMA_OFFSET                0x00A41980 /* CD Buffer DMA Requests (-) */
#define B2_PDU_DDBSC                      0x00A4302C /* Direct Draw Byte Swap Control (-) */



















#define B2_IRC                            0x00A43030 /* Buffered Indirect Read Control Register (RW) */

#define B2_CD_BUFFER_CTL                  0x00A43038 /* CD Buffer Control Register (RW) */
#define B2_PDU_BSCFB                      0x00A4303C /* PDU Buffered Byte Swap Control Register (frame buffer space) (RW) */
#define B2_PQC                            0x00A43040 /* Packet Queue Control Register (RW) */
#define B2_PQD                            0x00A43100 /* Start Packet Queue Data Register (RW) */
/*  End Packet Queue Data Register  */







#define B2_TRI_RLO_P1_0                   0x00A43C30 /* Buffered Ridgeline Out Signature Phase 1, [31:0] (RW) */
#define B2_TRI_RLO_P1_1                   0x00A43C34 /* Buffered Ridgeline Out Signature Phase 1, [63:32] (RW) */
#define B2_TRI_RLO_P2_0                   0x00A43C38 /* Buffered Ridgeline Out Signature Phase 2, [31:0] (RW) */
#define B2_TRI_RLO_P2_1                   0x00A43C3C /* Buffered Ridgeline Out Signature Phase 2, [63:32] (RW) */




#define B2_TRI_RLI_P1_0                   0x00A43E30 /* Buffered Ridgeline In Signature Phase 1, [31:0] (RW) */
#define B2_TRI_RLI_P1_1                   0x00A43E34 /* Buffered Ridgeline In Signature Phase 1, [63:32] (RW) */
#define B2_TRI_RLI_P2_0                   0x00A43E38 /* Buffered Ridgeline In Signature Phase 2, [31:0] (RW) */
#define B2_TRI_RLI_P2_1                   0x00A43E3C /* Buffered Ridgeline In Signature Phase 2, [63:32] (RW) */
#define B2_CCU_FB_CNTL                    0x00A43E40 /* FB Context Control (-) */
#define B2_CCU_STATE                      0x00A43E44 /* FB CCU State (-) */
#define B2_CCU_FB1_BABoth                 0x00A43E48 /* FB CCU Context 1 BABoth (-) */
#define B2_CCU_FB2_BABoth                 0x00A43E4C /* FB CCU Context 2 BABoth (-) */
#define B2_CCU_PRG_ADDR                   0x00A43E50 /* FB CCU Programmable Shadow Registers (-) */
#define B2_CCU_SHDW_DATA                  0x00A43E54 /* FB CCU Data of Programmable Shadow Registers (-) */
#define B2_CCU_DMP_DATA                   0x00A43E58 /* FB CCU Data of Programmable Dmp Data Registers (-) */
#define B2_CCU_SHDW_DBA                   0x00A43E5C /* FB CCU Context Shadow DBA (-) */
#define B2_CCU_SHDW_SBA                   0x00A43E60 /* FB CCU Context Shadow SBA (-) */
#define B2_CCU_SHDW_IPM                   0x00A43E64 /* FB CCU Context Shadow IPM (-) */
#define B2_CCU_SHDW_FOE                   0x00A43E68 /* FB CCU Context Shadow FOE (-) */
#define B2_CCU_SHDW_WCE                   0x00A43E6C /* FB CCU Context Shadow WCE (-) */
#define B2_CCU_SHDW_CPE                   0x00A43E70 /* FB CCU Context Shadow CPE (-) */
#define B2_CCU_SHDW_FBS                   0x00A43E74 /* FB CCU Context Shadow FBS (-) */
#define B2_CCU_SHDW_BPD                   0x00A43E78 /* FB CCU Context Shadow BPD (-) */
/*  End of Host Interface Address Space  */

/*  Start of Host Interface Address Space  */
#define B3_CONTROL                        0x00E41000 /* Control Register (RW) */
#define B3_FC                             0x00E41040 /* Fault Control Register (R) */









#define B3_STATUS                         0x00E41400 /* Status Register (R) */
#define B3_FFC                            0x00E41440 /* Fifo Free Count Register (R) */
#define B3_READ_DATA                      0x00E41480 /* Buffered Read Data Register (R) */

#define B3_IRP                            0x00E41500 /* Buffered Indirect Read Purge Register (W) */













#define B3_AGP_BDMA_UPPER                 0x00E41900 /* CD Buffer Reads (-) */
#define B3_AGP_BDMA_LOWER                 0x00E41940 /* CD Buffer Reads (-) */
#define B3_AGP_BDMA_OFFSET                0x00E41980 /* CD Buffer DMA Requests (-) */
#define B3_PDU_DDBSC                      0x00E4302C /* Direct Draw Byte Swap Control (-) */



















#define B3_IRC                            0x00E43030 /* Buffered Indirect Read Control Register (RW) */

#define B3_CD_BUFFER_CTL                  0x00E43038 /* CD Buffer Control Register (RW) */
#define B3_PDU_BSCFB                      0x00E4303C /* PDU Buffered Byte Swap Control Register (frame buffer space) (RW) */
#define B3_PQC                            0x00E43040 /* Packet Queue Control Register (RW) */
#define B3_PQD                            0x00E43100 /* Start Packet Queue Data Register (RW) */
/*  End Packet Queue Data Register  */







#define B3_TRI_RLO_P1_0                   0x00E43C30 /* Buffered Ridgeline Out Signature Phase 1, [31:0] (RW) */
#define B3_TRI_RLO_P1_1                   0x00E43C34 /* Buffered Ridgeline Out Signature Phase 1, [63:32] (RW) */
#define B3_TRI_RLO_P2_0                   0x00E43C38 /* Buffered Ridgeline Out Signature Phase 2, [31:0] (RW) */
#define B3_TRI_RLO_P2_1                   0x00E43C3C /* Buffered Ridgeline Out Signature Phase 2, [63:32] (RW) */




#define B3_TRI_RLI_P1_0                   0x00E43E30 /* Buffered Ridgeline In Signature Phase 1, [31:0] (RW) */
#define B3_TRI_RLI_P1_1                   0x00E43E34 /* Buffered Ridgeline In Signature Phase 1, [63:32] (RW) */
#define B3_TRI_RLI_P2_0                   0x00E43E38 /* Buffered Ridgeline In Signature Phase 2, [31:0] (RW) */
#define B3_TRI_RLI_P2_1                   0x00E43E3C /* Buffered Ridgeline In Signature Phase 2, [63:32] (RW) */
/*  End of Host Interface Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Summit Binc Interface  */
#define UB_BTDstObj_Xi                    0x00660000 /* Start of Address For Direct Binc - dst / src obj inc X (RW) */
/*  End of Address For Direct Binc - dst / src obj inc X  */

#define UB_BTDstObj_Xd                    0x00664000 /* Start of Address For Direct Binc - dst / src obj dec X (W) */
/*  End of Address For Direct Binc - dst / src obj dec X  */

/*  End of Summit Binc Interface  */

/*  Start of Summit Binc Interface  */
#define B2_BTDstObj_Xi                    0x00A60000 /* Start For Direct Binc - dst / src obj inc X (RW) */
/*  End For Direct Binc - dst / src obj inc X  */

#define B2_BTDstObj_Xd                    0x00A64000 /* Start For Direct Binc - dst / src obj dec X (W) */
/*  End For Direct Binc - dst / src obj dec X  */

#define B2_BTDstObj_Yi                    0x00A68000 /* Start For Direct Binc - dst / src obj inc Y (W) */
/*  End For Direct Binc - dst / src obj inc Y  */

#define B2_BTDstObj_Yd                    0x00A6C000 /* Start For Direct Binc - dst / src obj dec Y (W) */
/*  End For Direct Binc - dst / src obj dec Y  */

#define B2_BTDstObj_Xi_Yi                 0x00A70000 /* Start For Direct Binc - dst / src obj inc X inc Y  (RW) */
/*  End For Direct Binc - dst / src obj inc X, inc Y   */

#define B2_BTDstObj_Xi_Yd                 0x00A74000 /* Start For Direct Binc - dst / src obj inc X, dec Y (W) */
/*  End For Direct Binc - dst / src obj inc X, dec Y  */

#define B2_BTDstObj_Xd_Yi                 0x00A78000 /* Start of For Direct Binc - dst / src obj dec X, inc Y (W) */
/*  End For Direct Binc - dst / src obj dec X, inc  */

#define B2_BTDstObj_Xd_Yd                 0x00A7C000 /* Start For Direct Binc - dst / src obj dec X, dec Y (W) */
/*  End For Direct Binc - dst / src obj dec X, dec Y  */

/*  End of Summit Binc Interface Registers  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of DMA Controller Registers  */

/*  End Reserved  */

#define UP_DMA_UPPER32                    0x00254070 /* Buffered DMA Target Reg (MS 32 Bits) (For Diag Only) (R) */
#define UP_DMA_TARGET                     0x00254074 /* Buffered DMA Target Register (For Diag Only) (R) */
#define UP_DMA_LIMIT                      0x00254078 /* Buffered DMA Limit Register (For Diag Only) (R) */
#define UP_DMA_OFFSET                     0x0025407C /* Buffered DMA Offset Register (For Diag Only) (R) */
/*  Start Reserved (DMA_DATA)  */
/*  End Reserved (DMA_DATA)  */









/*  End Reserved  */

#define UP_UDMA_UPPER32                   0x00254870 /* Unbuffered DMA Target Register (Upper 32 Bits) (RW) */
#define UP_UDMA_TARGET                    0x00254874 /* Unbuffered DMA Target Register (RW) */
#define UP_UDMA_LIMIT                     0x00254878 /* Unbuffered DMA Limit Register (RW) */
#define UP_UDMA_OFFSET                    0x0025487C /* Unbuffered DMA Offset Register (RW) */
#define UP_UDMA_DATA                      0x00254880 /* Start of Unbuffered DMA Data Register (W) */
/*  End of Unbuffered DMA Data Register  */








/*  End of DMA Controller Registers  */

/*  Start of DMA Controller Registers  */

/*  End Reserved  */

#define BP_DMA_UPPER32                    0x002D4070 /* Buffered DMA Target Register (Upper 32 Bits) (RW) */
#define BP_DMA_TARGET                     0x002D4074 /* Buffered DMA Target Register (RW) */
#define BP_DMA_LIMIT                      0x002D4078 /* Buffered DMA Limit Register (RW) */
#define BP_DMA_OFFSET                     0x002D407C /* Buffered DMA Offset Register (RW) */
#define BP_DMA_DATA                       0x002D4080 /* Start of Buffered DMA Data Register (W) */
/*  End of Buffered DMA Data Register  */









/*  End Reserved  */





/*  Start Reserved (UDMA_DATA)  */
/*  End Reserved (UDMA_DATA)  */








/*  End of DMA Controller Registers  */

/*  Start of DMA Controller Registers  */

/*  End Reserved  */




#define UB_DMA_OFFSET                     0x006A007C /* Buffered DMA Offset Register (For Diag Only) (R) */
/*  Start Reserved (DMA_DATA)  */
/*  End Reserved (DMA_DATA)  */

#define UB_DMA_BSCBLK_RO                  0x006A0404 /* Buff. Data Blanket Byte Swap Ctl Read Reg (For Diag) (R) */


#define UB_DMA_DIAG_AFLAG                 0x006A0418 /* DMA Controller Address Flags (Diagnostic) (R) */
#define UB_DMA_DIAG_ADDR                  0x006A041C /* DMA Controller Address (Diagnostic) (R) */
#define UB_DMA_BSCSAV                     0x006A0640 /* Buff. Data Blanket Byte Swap Ctl Save Reg (For Diag) (R) */

/*  End Reserved  */




#define UB_UDMA_OFFSET                    0x006A087C /* Unbuffered DMA Offset Register (RW) */
#define UB_UDMA_DATA                      0x006A0880 /* Start of Unbuffered DMA Data Register (W) */
/*  End of Unbuffered DMA Data Register  */

#define UB_DMA_UBSCBLK_RO                 0x006A0C04 /* Address for read of UB_DMA_UBSCBLK. (R) */
#define UB_DMA_UBSCFB                     0x006A0C08 /* Unbuffered DMA Byte Swap Ctl Reg (FBC space) (RW) */
#define UB_UDMA_STATE                     0x006A0C10 /* Unbuffered DMA State Register (R) */
#define UB_UDMA_DIAG_AFLAG                0x006A0C18 /* Unbuffered DMA Controller Adr Flags (Diagnostic) (R) */
#define UB_UDMA_DIAG_ADDR                 0x006A0C1C /* Unbuffered DMA Controller Adr (Diagnostic) (R) */
#define UB_DMA_UBSCBLK                    0x006A0E00 /* Unbuffered Data Blanket Byte Swap Ctl Reg. (W) */
#define UB_DMA_UBSCSAV                    0x006A0E40 /* Unbuffered Data Blanket Byte Swap Ctl Save Reg (RW) */
/*  End of DMA Controller Registers  */

/*  Start of DMA Controller Registers  */

/*  End Reserved  */




#define B2_DMA_OFFSET                     0x00AA007C /* Buffered DMA Offset Register (RW) */
#define B2_DMA_DATA                       0x00AA0080 /* Start of Buffered DMA Data Register (W) */
/*  End of Buffered DMA Data Register  */

#define B2_DMA_BSCBLK_RO                  0x00AA0404 /* Address for read of B2_DMA_BSCBLK. (R) */
#define B2_DMA_BSCFB                      0x00AA0408 /* Buffered DMA Byte Swap Ctl Reg (FBC space) (RW) */
#define B2_DMA_DDBSC                      0x00AA040C /*  () */
#define B2_DMA_STATE                      0x00AA0410 /* Buffered DMA State Register (R) */
#define B2_DMA_DIAG_AFLAG                 0x00AA0418 /* Buffered DMA Controller Adr flags (Diagnostic) (R) */
#define B2_DMA_DIAG_ADDR                  0x00AA041C /* Buffered DMA Controller Adr (Diagnostic) (R) */
#define B2_DMA_BSCBLK                     0x00AA0600 /* Buffered Data Blanket Byte Swap Ctl Reg (W) */
#define B2_DMA_BSCSAV                     0x00AA0640 /* Buffered Data Blanket Byte Swap Ctl Save Reg (RW) */

/*  End Reserved  */





/*  Start Reserved (UDMA_DATA)  */
/*  End Reserved (UDMA_DATA)  */








/*  End of DMA Controller Registers  */

/*  Start of DMA Controller Registers  */

/*  End Reserved  */




#define B3_DMA_OFFSET                     0x00EA007C /* Buffered DMA Offset Register (RW) */
#define B3_DMA_DATA                       0x00EA0080 /* Start of Buffered DMA Data Register (W) */
/*  End of Buffered DMA Data Register  */

#define B3_DMA_BSCBLK_RO                  0x00EA0404 /* Address for read of B3_DMA_BSCBLK. (R) */
#define B3_DMA_BSCFB                      0x00EA0408 /* Buffered DMA Byte Swap Control Register (frame buffer space) (RW) */
#define B3_DMA_STATE                      0x00EA0410 /* Buffered DMA State Register (R) */
#define B3_DMA_DIAG_AFLAG                 0x00EA0418 /* Buffered DMA Controller Adr flags (Diagnostic) (R) */
#define B3_DMA_DIAG_ADDR                  0x00EA041C /* Buffered DMA Controller Adr (Diagnostic) (R) */
#define B3_DMA_BSCBLK                     0x00EA0600 /* Buffered Data Blanket Byte Swap Ctl Reg (W) */
#define B3_DMA_BSCSAV                     0x00EA0640 /* Buffered Data Blanket Byte Swap Ctl Save Reg (RW) */

/*  End Reserved  */





/*  Start Reserved (UDMA_DATA)  */
/*  End Reserved (UDMA_DATA)  */








/*  End of DMA Controller Registers  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
#define UB_OFU_UBABoth                    0x006C0C04 /* UDBA & USBA (reads return UDBA) (RW) */
#define UB_OFU_UDBA                       0x006C0C08 /* Unbuffered Destination Bitmap Access Register (RW) */
#define UB_OFU_USBA                       0x006C0C0C /* Unbuffered Source Bitmap Access Register (RW) */
#define UB_OFU_UDSA                       0x006C0C10 /* Unbuffered Destination Screen Address Register (RW) */
#define UB_OFU_USSA                       0x006C0C14 /* Unbuffered Source Screen Address Register (RW) */
#define UB_OFU_URPH                       0x006C0C5C /* Unbuffered Read Prefetch Hint Register (R) */
/*  End of Reserved Address Space  */

/*  Start of OFU Registers  */

#define B2_OFU_BABoth                     0x00AC0804 /* DBA & SBA (reads return DBA) (RW) */
#define B2_OFU_DBA                        0x00AC0808 /* Destination Bitmap Access Register (RW) */
#define B2_OFU_SBA                        0x00AC080C /* Source Bitmap Access Register (RW) */
#define B2_OFU_DSA                        0x00AC0810 /* Destination Screen Address Register (RW) */
#define B2_OFU_SSA                        0x00AC0814 /* Source Screen Address Register (RW) */
#define B2_OFU_WORG                       0x00AC0818 /* Window Origin Register (W) */
#define B2_OFU_BPDU                       0x00AC0820 /* Bitmap Pixel Data Register (Upper Half) (RW) */
#define B2_OFU_BPD                        0x00AC0824 /* Bitmap Pixel Data Register (RW) */
#define B2_OFU_BPCU                       0x00AC0830 /* Bitmap Pixel Clip Register (Upper Half) (RW) */
#define B2_OFU_BPC                        0x00AC0834 /* Bitmap Pixel Clip Register (RW) */
#define B2_OFU_FLN                        0x00AC0854 /* Fill Length (in X) (RW) */
#define B2_OFU_EN2D                       0x00AC0858 /* Coordinate Change Register (RW) */
#define B2_OFU_RPH                        0x00AC085C /* Read Prefetch Hint Register (R) */
#define B2_OFU_BLN                        0x00AC0880 /* BLT  Length (in X) (RW) */
#define B2_DWA                            0x00AC1000 /* Destination Window Address Register (RW) */
#define B2_SWA                            0x00AC1004 /* Source Window Address Register (RW) */
#define B2_DWAX                           0x00AC1010 /* Destination Window X Address Register (18 bits) (RW) */
#define B2_DWAY                           0x00AC1014 /* Destination Window Y Address Register (18 bits) (RW) */
#define B2_SWAX                           0x00AC1018 /* Source Window X Address Register (18 bits) (RW) */
#define B2_SWAY                           0x00AC101C /* Source Window Y Address Register (18 bits) (RW) */
#define B2_VBS                            0x00AC1020 /* Vertical Blank Sync Register (RW) */
#define B2_TCR                            0x00AC1024 /* Throttle Control Register (RW) */
#define B2_YBC                            0x00AC1028 /* YUV to RGB Brightness Control Register (RW) */
#define B2_PSF                            0x00AC1030 /* Pixel Scale Factor Register (RW) */
#define B2_PSS                            0x00AC1034 /* Pixel Scale Setup Register (RW) */
#define B2_PS_RED                         0x00AC1038 /* Pixel Scale Accumulate Red Register (RW) */
#define B2_PS_GRN                         0x00AC1040 /* Pixel Scale Accumulate Green Register (RW) */
#define B2_PS_BLU                         0x00AC1044 /* Pixel Scale Accumulate Blue Register (RW) */
#define B2_WCLIP1UL                       0x00AC1050 /* First Level Window Clip Upper Left Corner Reg (RW) */
#define B2_WCLIP1LR                       0x00AC1054 /* First Level Window Clip Lower Right Corner Reg (RW) */
#define B2_DSAX                           0x00AC2000 /* Destination Screen X Address Reg (19 bits) (RW) */
#define B2_DSAY                           0x00AC2004 /* Destination Screen Y Address Regr (19 bits) (RW) */
#define B2_SSAX                           0x00AC2008 /* Source Screen X Address Register (19 bits) (RW) */
#define B2_SSAY                           0x00AC200C /* Source Screen Y Address Register (19 bits) (RW) */
#define B2_BSE1                           0x00AC2010 /* Binc State Element 1 Register (R) */
#define B2_BSE2                           0x00AC2014 /* Binc State Element 2 Register (R) */
/*  End of OFU Registers  */

/*  Start of Reserved Address Space  */
/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of OGL State Registers  */
#define B3_MS_FF_AMBIENT_R_M              0x00EE2000 /* Material State Front Face Ambient R Master Reg (RW) */
#define B3_MS_FF_AMBIENT_G_M              0x00EE2004 /* Material State Front Face Ambient G Master Reg (RW) */
#define B3_MS_FF_AMBIENT_B_M              0x00EE2008 /* Material State Front Face Ambient B Master Reg (RW) */
#define B3_MS_FF_DIFF_R_M                 0x00EE200C /* Material State Front Face Diffuse R Master Reg (RW) */
#define B3_MS_FF_DIFF_G_M                 0x00EE2010 /* Material State Front Face Diffuse G Master Reg (RW) */
#define B3_MS_FF_DIFF_B_M                 0x00EE2014 /* Material State Front Face Diffuse B Master Reg (RW) */
#define B3_MS_FF_DIFF_A_M                 0x00EE2018 /* Material State Front Face Diffuse A Master Reg (RW) */
#define B3_MS_FF_SPEC_R_M                 0x00EE201C /* Material State Front Face Specular R Master Reg (RW) */
#define B3_MS_FF_SPEC_G_M                 0x00EE2020 /* Material State Front Face Specular G Master Reg (RW) */
#define B3_MS_FF_SPEC_B_M                 0x00EE2024 /* Material State Front Face Specular B Master Reg (RW) */
#define B3_MS_FF_EM_R_M                   0x00EE2028 /* Material State Front Face Emissive R Master Reg (RW) */
#define B3_MS_FF_EM_G_M                   0x00EE202C /* Material State Front Face Emissive G Master Reg (RW) */
#define B3_MS_FF_EM_B_M                   0x00EE2030 /* Material State Front Face Emissive B Master Reg (RW) */
#define B3_MS_FF_SHIN_M                   0x00EE2034 /* Material State Front Face Shininess Master Reg (RW) */
#define B3_MS_FF_THRESH_M                 0x00EE2038 /* Material State Front Face Threshold Master Reg (RW) */
#define B3_MS_BF_AMBIENT_R_M              0x00EE2040 /* Material State Back Face Ambient R Master Reg (RW) */
#define B3_MS_BF_AMBIENT_G_M              0x00EE2044 /* Material State Back Face Ambient G Master Reg (RW) */
#define B3_MS_BF_AMBIENT_B_M              0x00EE2048 /* Material State Back Face Ambient B Master Reg (RW) */
#define B3_MS_BF_DIFF_R_M                 0x00EE204C /* Material State Back Face Diffuse R Master Reg (RW) */
#define B3_MS_BF_DIFF_G_M                 0x00EE2050 /* Material State Back Face Diffuse G Master Reg (RW) */
#define B3_MS_BF_DIFF_B_M                 0x00EE2054 /* Material State Back Face Diffuse B Master Reg (RW) */
#define B3_MS_BF_DIFF_A_M                 0x00EE2058 /* Material State Back Face Diffuse A Master Reg (RW) */
#define B3_MS_BF_SPEC_R_M                 0x00EE205C /* Material State Back Face Specular R Master Reg (RW) */
#define B3_MS_BF_SPEC_G_M                 0x00EE2060 /* Material State Back Face Specular G Master Reg (RW) */
#define B3_MS_BF_SPEC_B_M                 0x00EE2064 /* Material State Back Face Specular B Master Reg (RW) */
#define B3_MS_BF_EM_R_M                   0x00EE2068 /* Material State Back Face Emissive R Master Reg (RW) */
#define B3_MS_BF_EM_G_M                   0x00EE206C /* Material State Back Face Emissive G Master Reg (RW) */
#define B3_MS_BF_EM_B_M                   0x00EE2070 /* Material State Back Face Emissive B Master Reg (RW) */
#define B3_MS_BF_SHIN_M                   0x00EE2074 /* Material State Back Face Shininess Master Reg (RW) */
#define B3_MS_BF_THRESH_M                 0x00EE2078 /* Material State Back Face Threshold Master Reg (RW) */
#define B3_MS_DC_SEP_SPEC_RED_M           0x00EE207C /* Material State Separate Specular Red Master Reg (RW) */
#define B3_MS_DC_SEP_SPEC_GREEN_M         0x00EE2080 /* Material State Separate Specular Green Master Reg (RW) */
#define B3_MS_DC_SEP_SPEC_BLUE_M          0x00EE2084 /* Material State Separate Specular Blue Master Reg (RW) */
#define B3_MS_DC_FOG_M                    0x00EE2088 /* Material State Fog Master Reg (RW) */
#define B3_MS_FF_AMBIENT_R_S              0x00EE208C /* Material State Front Face Ambient R Slave Reg (RW) */
#define B3_MS_FF_AMBIENT_G_S              0x00EE2090 /* Material State Front Face Ambient G Slave Reg (RW) */
#define B3_MS_FF_AMBIENT_B_S              0x00EE2094 /* Material State Front Face Ambient B Slave Reg (RW) */
#define B3_MS_FF_DIFF_R_S                 0x00EE2098 /* Material State Front Face Diffuse R Slave Reg (RW) */
#define B3_MS_FF_DIFF_G_S                 0x00EE209C /* Material State Front Face Diffuse G Slave Reg (RW) */
#define B3_MS_FF_DIFF_B_S                 0x00EE20A0 /* Material State Front Face Diffuse B Slave Reg (RW) */
#define B3_MS_FF_DIFF_A_S                 0x00EE20A4 /* Material State Front Face Diffuse A Slave Reg (RW) */
#define B3_MS_FF_SPEC_R_S                 0x00EE20A8 /* Material State Front Face Specular R Slave Reg (RW) */
#define B3_MS_FF_SPEC_G_S                 0x00EE20AC /* Material State Front Face Specular G Slave Reg (RW) */
#define B3_MS_FF_SPEC_B_S                 0x00EE20B0 /* Material State Front Face Specular B Slave Reg (RW) */
#define B3_MS_FF_EM_R_S                   0x00EE20B4 /* Material State Front Face Emissive R Slave Reg (RW) */
#define B3_MS_FF_EM_G_S                   0x00EE20B8 /* Material State Front Face Emissive G Slave Reg (RW) */
#define B3_MS_FF_EM_B_S                   0x00EE20BC /* Material State Front Face Emissive B Slave Reg (RW) */
#define B3_MS_FF_SHIN_S                   0x00EE20C0 /* Material State Front Face Shininess Slave Reg (RW) */
#define B3_MS_FF_THRESH_S                 0x00EE20C4 /* Material State Front Face Threshold Slave Reg (RW) */
#define B3_MS_BF_AMBIENT_R_S              0x00EE20C8 /* Material State Back Face Ambient R Slave Reg (RW) */
#define B3_MS_BF_AMBIENT_G_S              0x00EE20CC /* Material State Back Face Ambient G Slave Reg (RW) */
#define B3_MS_BF_AMBIENT_B_S              0x00EE20D0 /* Material State Back Face Ambient B Slave Reg (RW) */
#define B3_MS_BF_DIFF_R_S                 0x00EE20D4 /* Material State Back Face Diffuse R Slave Reg (RW) */
#define B3_MS_BF_DIFF_G_S                 0x00EE20D8 /* Material State Back Face Diffuse G Slave Reg (RW) */
#define B3_MS_BF_DIFF_B_S                 0x00EE20DC /* Material State Back Face Diffuse B Slave Reg (RW) */
#define B3_MS_BF_DIFF_A_S                 0x00EE20E0 /* Material State Back Face Diffuse A Slave Reg (RW) */
#define B3_MS_BF_SPEC_R_S                 0x00EE20E4 /* Material State Back Face Specular R Slave Reg (RW) */
#define B3_MS_BF_SPEC_G_S                 0x00EE20E8 /* Material State Back Face Specular G Slave Reg (RW) */
#define B3_MS_BF_SPEC_B_S                 0x00EE20EC /* Material State Back Face Specular B Slave Reg (RW) */
#define B3_MS_BF_EM_R_S                   0x00EE20F0 /* Material State Back Face Emissive R Slave Reg (RW) */
#define B3_MS_BF_EM_G_S                   0x00EE20F4 /* Material State Back Face Emissive G Slave Reg (RW) */
#define B3_MS_BF_EM_B_S                   0x00EE20F8 /* Material State Back Face Emissive B Slave Reg (RW) */
#define B3_MS_BF_SHIN_S                   0x00EE20FC /* Material State Back Face Shininess Slave Reg (RW) */
#define B3_MS_BF_THRESH_S                 0x00EE2100 /* Material State Back Face Threshold Slave Reg (RW) */
#define B3_MS_DC_SEP_SPEC_RED_S           0x00EE2104 /* Material State Separate Specular Red Slave Reg (RW) */
#define B3_MS_DC_SEP_SPEC_GREEN_S         0x00EE2108 /* Material State Separate Specular Green Slave Reg (RW) */
#define B3_MS_DC_SEP_SPEC_BLUE_S          0x00EE210C /* Material State Separate Specular Blue Slave Reg (RW) */
#define B3_MS_DC_FOG_S                    0x00EE2110 /* Material State Fog Slave Register (RW) */
#define B3_VS_X_M                         0x00EE2114 /* Vertex State X Master Register (RW) */
#define B3_VS_Y_M                         0x00EE2118 /* Vertex State Y Master Register (RW) */
#define B3_VS_Z_M                         0x00EE211C /* Vertex State Z Master Register (RW) */
#define B3_VS_W_M                         0x00EE2120 /* Vertex State W Master Register (RW) */
#define B3_VS_X_S                         0x00EE2124 /* Vertex State X Slave Register (RW) */
#define B3_VS_Y_S                         0x00EE2128 /* Vertex State Y Slave Register (RW) */
#define B3_VS_Z_S                         0x00EE212C /* Vertex State Z Slave Register (RW) */
#define B3_VS_W_S                         0x00EE2130 /* Vertex State W Slave Register (RW) */
#define B3_CS_NORM_X_M                    0x00EE2200 /* Current State Normal X Master Register (RW) */
#define B3_CS_NORM_Y_M                    0x00EE2204 /* Current State Normal Y Master Register (RW) */
#define B3_CS_NORM_Z_M                    0x00EE2208 /* Current State Normal Z Master Register (RW) */
#define B3_CS_COLOR_R_M                   0x00EE220C /* Current State Color R Master Register (RW) */
#define B3_CS_COLOR_G_M                   0x00EE2210 /* Current State Color G Master Register (RW) */
#define B3_CS_COLOR_B_M                   0x00EE2214 /* Current State Color B Master Register (RW) */
#define B3_CS_COLOR_A_M                   0x00EE2218 /* Current State Color A Master Register (RW) */
#define B3_CS_TEX_COORD_S_M               0x00EE221C /* Current State Texture Coordinate S Master Reg (RW) */
#define B3_CS_TEX_COORD_T_M               0x00EE2220 /* Current State Texture Coordinate T Master Reg (RW) */
#define B3_CS_TEX_COORD_R_M               0x00EE2224 /* Current State Texture Coordinate R Master Reg (RW) */
#define B3_CS_TEX_COORD_Q_M               0x00EE2228 /* Current State Texture Coordinate Q Master Reg (RW) */
#define B3_CS_EDGE_F_M                    0x00EE222C /* Current State Edge Flag Master Register (RW) */
#define B3_CS_NORM_X_S                    0x00EE2240 /* Current State Normal X Slave Register (RW) */
#define B3_CS_NORM_Y_S                    0x00EE2244 /* Current State Normal Y Slave Register (RW) */
#define B3_CS_NORM_Z_S                    0x00EE2248 /* Current State Normal Z Slave Register (RW) */
#define B3_CS_COLOR_R_S                   0x00EE224C /* Current State Color R Slave Register (RW) */
#define B3_CS_COLOR_G_S                   0x00EE2250 /* Current State Color G Slave Register (RW) */
#define B3_CS_COLOR_B_S                   0x00EE2254 /* Current State Color B Slave Register (RW) */
#define B3_CS_COLOR_A_S                   0x00EE2258 /* Current State Color A Slave Register (RW) */
#define B3_CS_TEX_COORD_S_S               0x00EE225C /* Current State Texture Coordinate S Slave Reg (RW) */
#define B3_CS_TEX_COORD_T_S               0x00EE2260 /* Current State Texture Coordinate T Slave Reg (RW) */
#define B3_CS_TEX_COORD_R_S               0x00EE2264 /* Current State Texture Coordinate R Slave Reg (RW) */
#define B3_CS_TEX_COORD_Q_S               0x00EE2268 /* Current State Texture Coordinate Q Slave Reg (RW) */
#define B3_CS_EDGE_F_S                    0x00EE226C /* Current State Edge Flag Slave Register (RW) */
#define B3_CS_DV_NORM                     0x00EE2300 /* Current State Dirty Vector Normal Register (RW) */
#define B3_CS_DV_COLOR                    0x00EE2304 /* Current State Dirty Vector Color Register (RW) */
#define B3_CS_DV_TEX_COORD                0x00EE2308 /* Current State Dirty Vector Texture Coordinate Reg (RW) */
#define B3_CS_DV_EDGE_F                   0x00EE230C /* Current State Dirty Vector Edge Flag Register (RW) */
#define B3_CS_DB                          0x00EE2310 /* Currenst State Dirty Bits (RW) */
#define B3_MS_DB                          0x00EE2314 /* Material State Dirty Bits (RW) */
#define B3_COLOR_VAL                      0x00EE2318 /* Current State Alpha Valid Register (RW) */
#define B3_BBC_CONFIG                     0x00EE231C /* Blitzen Bus Controller Config Register (RW) */
#define B3_BIB_LOW_CRC                    0x00EE2320 /* Signature Reg For Triaqua / Blitzen Interface (RW) */
#define B3_BIB_HIGH_CRC                   0x00EE2324 /* Signature Reg For Triaqua / Blitzen Interface (RW) */
#define B3_BOB_P1_LOW_CRC                 0x00EE2328 /* Signature Reg For Triaqua / Blitzen Interface (RW) */
#define B3_BOB_P1_HIGH_CRC                0x00EE232C /* Signature Reg For Triaqua / Blitzen Interface (RW) */
#define B3_BOB_P2_LOW_CRC                 0x00EE2330 /* Signature Reg For Triaqua / Blitzen Interface (RW) */
#define B3_BOB_P2_HIGH_CRC                0x00EE2334 /* Signature Reg For Triaqua / Blitzen Interface (RW) */
#define B3_BBC_NOP                        0x00EE2338 /* BBC NOP Register (RW) */
#define B3_NUM_GA                         0x00EE233C /* Number Of Geometry Accelerators Register (RW) */
#define B3_VERT_CNT                       0x00EE2340 /* Vertex Count Register (RW) */
#define B3_W_VAL                          0x00EE2344 /* Vertex State W Valid Register (RW) */
#define B3_PRIM_TYPE                      0x00EE2348 /* Primitive Type Register (RW) */
#define B3_CURRENT_GA                     0x00EE234C /* Current Geometry Accelerator Register (RW) */
#define B3_HUNGRY                         0x00EE2350 /* Hungry Bits From GA's Register (R) */
#define B3_SHIN_FF_INT_NFP                0x00EE2354 /* Shinniness Front Face Int / FP Register (RW) */
#define B3_SHIN_BF_INT_NFP                0x00EE2358 /* Shinniness Back Face Int / FP Register (RW) */
#define B3_GLCOLOR3UB                     0x00EE235C /* Send R,G,B,don't care in byte mode (1 word) (RW) */
#define B3_GLCOLOR4UB                     0x00EE2360 /* Send R,G,B,A in byte mode (1 word) (RW) */
#define B3_CM_INHERIT_R                   0x00EE2380 /* Color Material Inherit Red Register (W) */
#define B3_CM_INHERIT_G                   0x00EE2384 /* Color Material Inherit Green Register (W) */
#define B3_CM_INHERIT_B                   0x00EE2388 /* Color Material Inherit Blue Register (W) */
#define B3_CM_INHERIT_A                   0x00EE238C /* Color Material Inherit Alpha Register (W) */
#define B3_VPC_0                          0x00EE23C0 /* Vertices per Chunk Zero Register (RW) */
#define B3_VPC_1                          0x00EE23C4 /* Vertices per Chunk One Register (RW) */
#define B3_VPC_2                          0x00EE23C8 /* Vertices per Chunk Two  Register (RW) */
/*  End of OGL State Registers  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of MFU Registers  */
/*  Start of MFU Opcodes  */
#define B2_MNOOP_R0                       0x00B00000 /* MFU Noop Opcode / Load Master Register 0 (RW) */
#define B2_MNOOP_R1                       0x00B00004 /* MFU Noop Opcode / Load Master Register 1 (RW) */
#define B2_MNOOP_R2                       0x00B00008 /* MFU Noop Opcode / Load Master Register 2 (RW) */
#define B2_MNOOP_R3                       0x00B0000C /* MFU Noop Opcode / Load Master Register 3 (RW) */
#define B2_MNOOP_R4                       0x00B00010 /* MFU Noop Opcode / Load Master Register 4 (RW) */
#define B2_MNOOP_R5                       0x00B00014 /* MFU Noop Opcode / Load Master Register 5 (RW) */
#define B2_MNOOP_R6                       0x00B00018 /* MFU Noop Opcode / Load Master Register 6 (RW) */
#define B2_MNOOP_R7                       0x00B0001C /* MFU Noop Opcode / Load Master Register 7 (RW) */
#define B2_MNOOP_R8                       0x00B00020 /* MFU Noop Opcode / Load Master Register 8 (RW) */
#define B2_MNOOP_R9                       0x00B00024 /* MFU Noop Opcode / Load Master Register 9 (RW) */
#define B2_MNOOP_R10                      0x00B00028 /* MFU Noop Opcode / Load Master Register 10 (RW) */
#define B2_MNOOP_R11                      0x00B0002C /* MFU Noop Opcode / Load Master Register 11 (RW) */
#define B2_MNOOP_R12                      0x00B00030 /* MFU Noop Opcode / Load Master Register 12 (RW) */
#define B2_MNOOP_R13                      0x00B00034 /* MFU Noop Opcode / Load Master Register 13 (RW) */
#define B2_MNOOP_TD                       0x00B00044 /* MFU Noop Opcode / Load Master TD Register (RW) */
#define B2_NOSB_R0                        0x00B00080 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 0 (RW) */
#define B2_NOSB_R1                        0x00B00084 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 1 (RW) */
#define B2_NOSB_R2                        0x00B00088 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 2 (RW) */
#define B2_NOSB_R3                        0x00B0008C /* MFU Noop Opcode (No Scoreboard) / Load Master Register 3 (RW) */
#define B2_NOSB_R4                        0x00B00090 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 4 (RW) */
#define B2_NOSB_R5                        0x00B00094 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 5 (RW) */
#define B2_NOSB_R6                        0x00B00098 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 6 (RW) */
#define B2_NOSB_R7                        0x00B0009C /* MFU Noop Opcode (No Scoreboard) / Load Master Register 7 (RW) */
#define B2_NOSB_R8                        0x00B000A0 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 8 (RW) */
#define B2_NOSB_R9                        0x00B000A4 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 9 (RW) */
#define B2_NOSB_R10                       0x00B000A8 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 10 (RW) */
#define B2_NOSB_R11                       0x00B000AC /* MFU Noop Opcode (No Scoreboard) / Load Master Register 11 (RW) */
#define B2_NOSB_R12                       0x00B000B0 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 12 (RW) */
#define B2_NOSB_R13                       0x00B000B4 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 13 (RW) */
#define B2_NOSB_TD                        0x00B000C4 /* MFU Noop Opcode (No Scoreboard) / Load Master TD Register (RW) */
#define B2_MSLOAD_R0                      0x00B00100 /* Master To Slave Load Opcode / Load Master Register 0 (W) */
#define B2_MSLOAD_R1                      0x00B00104 /* Master To Slave Load Opcode / Load Master Register 1 (W) */
#define B2_MSLOAD_R2                      0x00B00108 /* Master To Slave Load Opcode / Load Master Register 2 (W) */
#define B2_MSLOAD_R3                      0x00B0010C /* Master To Slave Load Opcode / Load Master Register 3 (W) */
#define B2_MSLOAD_R4                      0x00B00110 /* Master To Slave Load Opcode / Load Master Register 4 (W) */
#define B2_MSLOAD_R5                      0x00B00114 /* Master To Slave Load Opcode / Load Master Register 5 (W) */
#define B2_MSLOAD_R6                      0x00B00118 /* Master To Slave Load Opcode / Load Master Register 6 (W) */
#define B2_MSLOAD_R7                      0x00B0011C /* Master To Slave Load Opcode / Load Master Register 7 (W) */
#define B2_MSLOAD_R8                      0x00B00120 /* Master To Slave Load Opcode / Load Master Register 8 (W) */
#define B2_MSLOAD_R9                      0x00B00124 /* Master To Slave Load Opcode / Load Master Register 9 (W) */
#define B2_MSLOAD_R10                     0x00B00128 /* Master To Slave Load Opcode / Load Master Register 10 (W) */
#define B2_MSLOAD_R11                     0x00B0012C /* Master To Slave Load Opcode / Load Master Register 11 (W) */
#define B2_MSLOAD_R12                     0x00B00130 /* Master To Slave Load Opcode / Load Master Register 12 (W) */
#define B2_MSLOAD_R13                     0x00B00134 /* Master To Slave Load Opcode / Load Master Register 13 (W) */
#define B2_MSLOAD_TD                      0x00B00144 /* Master To Slave Load Opcode / Load Master TD Register (W) */
#define B2_TEXT16_R0                      0x00B00400 /* 16 Bit Text Fill Opcode / Load Master Register 0 (W) */
#define B2_TEXT16_R1                      0x00B00404 /* 16 Bit Text Fill Opcode / Load Master Register 1 (W) */
#define B2_TEXT16_R2                      0x00B00408 /* 16 Bit Text Fill Opcode / Load Master Register 2 (W) */
#define B2_TEXT16_R3                      0x00B0040C /* 16 Bit Text Fill Opcode / Load Master Register 3 (W) */
#define B2_TEXT16_TD                      0x00B00444 /* 16 Bit Text Fill Opcode / Load Master TD Register (W) */
#define B2_TEXT8_R0                       0x00B00480 /* 8 Bit Text Fill Opcode / Load Master Register 0 (W) */
#define B2_TEXT8_R1                       0x00B00484 /* 8 Bit Text Fill Opcode / Load Master Register 1 (W) */
#define B2_TEXT8_R2                       0x00B00488 /* 8 Bit Text Fill Opcode / Load Master Register 2 (W) */
#define B2_TEXT8_R3                       0x00B0048C /* 8 Bit Text Fill Opcode / Load Master Register 3 (W) */
#define B2_TEXT8_TD                       0x00B004C4 /* 8 Bit Text Fill Opcode / Load Master TD Register (W) */
#define B2_TEXT16SETUP_R0                 0x00B00500 /* 16 Bit Text Fill Setup Opcode / Load Master Register 0 (W) */
#define B2_TEXT16SETUP_R1                 0x00B00504 /* 16 Bit Text Fill Setup Opcode / Load Master Register 1 (W) */
#define B2_TEXT16SETUP_R2                 0x00B00508 /* 16 Bit Text Fill Setup Opcode / Load Master Register 2 (W) */
#define B2_TEXT16SETUP_R3                 0x00B0050C /* 16 Bit Text Fill Setup Opcode / Load Master Register 3 (W) */
#define B2_TEXT16SETUP_TD                 0x00B00544 /* 16 Bit Text Fill Setup Opcode / Load Master TD Register (W) */
#define B2_TEXT8SETUP_R0                  0x00B00580 /* 8 Bit Text Fill Setup Opcode / Load Master Register 0 (W) */
#define B2_TEXT8SETUP_R1                  0x00B00584 /* 8 Bit Text Fill Setup Opcode / Load Master Register 1 (W) */
#define B2_TEXT8SETUP_R2                  0x00B00588 /* 8 Bit Text Fill Setup Opcode / Load Master Register 2 (W) */
#define B2_TEXT8SETUP_R3                  0x00B0058C /* 8 Bit Text Fill Setup Opcode / Load Master Register 3 (W) */
#define B2_TEXT8SETUP_TD                  0x00B005C4 /* 8 Bit Text Fill Setup Opcode / Load Master TD Register (W) */
#define B2_SOLIDFILL_R0                   0x00B00800 /* Solid Fill Opcode / Load Master Register 0 (W) */
#define B2_SOLIDFILL_R1                   0x00B00804 /* Solid Fill Opcode / Load Master Register 1 (W) */
#define B2_SOLIDFILL_R2                   0x00B00808 /* Solid Fill Opcode / Load Master Register 2 (W) */
#define B2_SOLIDFILL_R3                   0x00B0080C /* Solid Fill Opcode / Load Master Register 3 (W) */
#define B2_SOLIDFILLT_R0                  0x00B00880 /* Solid Fill Throttled Opcode / Load Master Register 0 (W) */
#define B2_SOLIDFILLT_R1                  0x00B00884 /* Solid Fill Throttled Opcode / Load Master Register 1 (W) */
#define B2_SOLIDFILLT_R2                  0x00B00888 /* Solid Fill Throttled Opcode / Load Master Register 2 (W) */
#define B2_SOLIDFILLT_R3                  0x00B0088C /* Solid Fill Throttled Opcode / Load Master Register 3 (W) */
#define B2_TDFILL_R0                      0x00B00900 /* TD Fill Opcode / Load Master Register 0 (W) */
#define B2_TDFILL_R1                      0x00B00904 /* TD Fill Opcode / Load Master Register 1 (W) */
#define B2_TDFILL_R2                      0x00B00908 /* TD Fill Opcode / Load Master Register 2 (W) */
#define B2_TDFILL_R3                      0x00B0090C /* TD Fill Opcode / Load Master Register 3 (W) */
#define B2_TDFILL_TD                      0x00B00944 /* TD Fill Opcode / Load Master TD Register (W) */
#define B2_TDFILLT_R0                     0x00B00980 /* TD Fill Throttled Opcode / Load Master Register 0 (W) */
#define B2_TDFILLT_R1                     0x00B00984 /* TD Fill Throttled Opcode / Load Master Register 1 (W) */
#define B2_TDFILLT_R2                     0x00B00988 /* TD Fill Throttled Opcode / Load Master Register 2 (W) */
#define B2_TDFILLT_R3                     0x00B0098C /* TD Fill Throttled Opcode / Load Master Register 3 (W) */
#define B2_TDFILLT_TD                     0x00B009C4 /* TD Fill Throttled Opcode / Load Master TD Register (W) */
#define B2_BLT_R0                         0x00B00C00 /* Blt Opcode / Load Master Register 0 (W) */
#define B2_BLT_R1                         0x00B00C04 /* Blt Opcode / Load Master Register 1 (W) */
#define B2_BLT_R2                         0x00B00C08 /* Blt Opcode / Load Master Register 2 (W) */
#define B2_BLT_R3                         0x00B00C0C /* Blt Opcode / Load Master Register 3 (W) */
#define B2_BLT_R4                         0x00B00C10 /* Blt Opcode / Load Master Register 4 (W) */
#define B2_BLT_R5                         0x00B00C14 /* Blt Opcode / Load Master Register 5 (W) */
#define B2_EXBLT_R0                       0x00B00C80 /* Expanding Blt Opcode / Load Master Register 0 (W) */
#define B2_EXBLT_R1                       0x00B00C84 /* Expanding Blt Opcode / Load Master Register 1 (W) */
#define B2_EXBLT_R2                       0x00B00C88 /* Expanding Blt Opcode / Load Master Register 2 (W) */
#define B2_EXBLT_R3                       0x00B00C8C /* Expanding Blt Opcode / Load Master Register 3 (W) */
#define B2_EXBLT_R4                       0x00B00C90 /* Expanding Blt Opcode / Load Master Register 4 (W) */
#define B2_EXBLT_R5                       0x00B00C94 /* Expanding Blt Opcode / Load Master Register 5 (W) */
#define B2_EXBLT_R11                      0x00B00CAC /* Expanding Blt Opcode / Load Master Register 11 (W) */
#define B2_VEC_R0                         0x00B01000 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_R1                         0x00B01004 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_R2                         0x00B01008 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_R3                         0x00B0100C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_R4                         0x00B01010 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_R5                         0x00B01014 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_R6                         0x00B01018 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_TD                         0x00B01044 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_VEC_E_R0                       0x00B01080 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_E_R1                       0x00B01084 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_E_R2                       0x00B01088 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_E_R3                       0x00B0108C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_E_R4                       0x00B01090 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_E_R5                       0x00B01094 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_E_R6                       0x00B01098 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_E_TD                       0x00B010C4 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_VEC_C_R0                       0x00B01100 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_C_R1                       0x00B01104 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_C_R2                       0x00B01108 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_C_R3                       0x00B0110C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_C_R4                       0x00B01110 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_C_R5                       0x00B01114 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_C_R6                       0x00B01118 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_C_TD                       0x00B01144 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_VEC_CE_R0                      0x00B01180 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_CE_R1                      0x00B01184 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_CE_R2                      0x00B01188 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_CE_R3                      0x00B0118C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_CE_R4                      0x00B01190 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_CE_R5                      0x00B01194 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_CE_R6                      0x00B01198 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_CE_TD                      0x00B011C4 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_VEC_D_R0                       0x00B01200 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_D_R1                       0x00B01204 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_D_R2                       0x00B01208 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_D_R3                       0x00B0120C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_D_R4                       0x00B01210 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_D_R5                       0x00B01214 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_D_R6                       0x00B01218 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_D_TD                       0x00B01244 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_VEC_DE_R0                      0x00B01280 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_DE_R1                      0x00B01284 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_DE_R2                      0x00B01288 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_DE_R3                      0x00B0128C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_DE_R4                      0x00B01290 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_DE_R5                      0x00B01294 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_DE_R6                      0x00B01298 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_DE_TD                      0x00B012C4 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_VEC_DC_R0                      0x00B01300 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_DC_R1                      0x00B01304 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_DC_R2                      0x00B01308 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_DC_R3                      0x00B0130C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_DC_R4                      0x00B01310 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_DC_R5                      0x00B01314 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_DC_R6                      0x00B01318 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_DC_TD                      0x00B01344 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_VEC_DCE_R0                     0x00B01380 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_DCE_R1                     0x00B01384 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_DCE_R2                     0x00B01388 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_DCE_R3                     0x00B0138C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_DCE_R4                     0x00B01390 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_DCE_R5                     0x00B01394 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_DCE_R6                     0x00B01398 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_DCE_TD                     0x00B013C4 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_VEC_S_R0                       0x00B01400 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_S_R1                       0x00B01404 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_S_R2                       0x00B01408 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_S_R3                       0x00B0140C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_S_R4                       0x00B01410 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_S_R5                       0x00B01414 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_S_R6                       0x00B01418 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_S_TD                       0x00B01444 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_VEC_SE_R0                      0x00B01480 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_SE_R1                      0x00B01484 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_SE_R2                      0x00B01488 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_SE_R3                      0x00B0148C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_SE_R4                      0x00B01490 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_SE_R5                      0x00B01494 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_SE_R6                      0x00B01498 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_SE_TD                      0x00B014C4 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_VEC_SC_R0                      0x00B01500 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_SC_R1                      0x00B01504 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_SC_R2                      0x00B01508 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_SC_R3                      0x00B0150C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_SC_R4                      0x00B01510 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_SC_R5                      0x00B01514 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_SC_R6                      0x00B01518 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_SC_TD                      0x00B01544 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_VEC_SCE_R0                     0x00B01580 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_SCE_R1                     0x00B01584 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_SCE_R2                     0x00B01588 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_SCE_R3                     0x00B0158C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_SCE_R4                     0x00B01590 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_SCE_R5                     0x00B01594 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_SCE_R6                     0x00B01598 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_SCE_TD                     0x00B015C4 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_VEC_SD_R0                      0x00B01600 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_SD_R1                      0x00B01604 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_SD_R2                      0x00B01608 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_SD_R3                      0x00B0160C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_SD_R4                      0x00B01610 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_SD_R5                      0x00B01614 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_SD_R6                      0x00B01618 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_SD_TD                      0x00B01644 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_VEC_SDE_R0                     0x00B01680 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_SDE_R1                     0x00B01684 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_SDE_R2                     0x00B01688 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_SDE_R3                     0x00B0168C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_SDE_R4                     0x00B01690 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_SDE_R5                     0x00B01694 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_SDE_R6                     0x00B01698 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_SDE_TD                     0x00B016C4 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_VEC_SDC_R0                     0x00B01700 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_SDC_R1                     0x00B01704 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_SDC_R2                     0x00B01708 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_SDC_R3                     0x00B0170C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_SDC_R4                     0x00B01710 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_SDC_R5                     0x00B01714 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_SDC_R6                     0x00B01718 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_SDC_TD                     0x00B01744 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_VEC_SDCE_R0                    0x00B01780 /* Vector Opcode / Load Master Register 0 (W) */
#define B2_VEC_SDCE_R1                    0x00B01784 /* Vector Opcode / Load Master Register 1 (W) */
#define B2_VEC_SDCE_R2                    0x00B01788 /* Vector Opcode / Load Master Register 2 (W) */
#define B2_VEC_SDCE_R3                    0x00B0178C /* Vector Opcode / Load Master Register 3 (W) */
#define B2_VEC_SDCE_R4                    0x00B01790 /* Vector Opcode / Load Master Register 4 (W) */
#define B2_VEC_SDCE_R5                    0x00B01794 /* Vector Opcode / Load Master Register 5 (W) */
#define B2_VEC_SDCE_R6                    0x00B01798 /* Vector Opcode / Load Master Register 6 (W) */
#define B2_VEC_SDCE_TD                    0x00B017C4 /* Vector Opcode / Load Master Transfer Data Register (W) */
#define B2_TRAP_R0                        0x00B02600 /* Trapezoid Opcode / Load Master Register 0 (W) */
#define B2_TRAP_R1                        0x00B02604 /* Trapezoid Opcode / Load Master Register 1 (W) */
#define B2_TRAP_R2                        0x00B02608 /* Trapezoid Opcode / Load Master Register 2 (W) */
#define B2_TRAP_R3                        0x00B0260C /* Trapezoid Opcode / Load Master Register 3 (W) */
#define B2_TRAP_R4                        0x00B02610 /* Trapezoid Opcode / Load Master Register 4 (W) */
#define B2_TRAP_R5                        0x00B02614 /* Trapezoid Opcode / Load Master Register 5 (W) */
#define B2_TRAP_R6                        0x00B02618 /* Trapezoid Opcode / Load Master Register 6 (W) */
#define B2_TRAP_R8                        0x00B02620 /* Trapezoid Opcode / Load Master Register 8 (W) */
#define B2_TRAP_R9                        0x00B02624 /* Trapezoid Opcode / Load Master Register 9 (W) */
#define B2_TRAP_R10                       0x00B02628 /* Trapezoid Opcode / Load Master Register 10 (W) */
#define B2_TRAP_R11                       0x00B0262C /* Trapezoid Opcode / Load Master Register 11 (W) */
#define B2_TRAP_R12                       0x00B02630 /* Trapezoid Opcode / Load Master Register 12 (W) */
#define B2_TRAP_R13                       0x00B02634 /* Trapezoid Opcode / Load Master Register 13 (W) */
#define B2_TRAP_TD                        0x00B02644 /* Trapezoid Opcode / Load Master Register TD (W) */
#define B2_TRAP_L_R0                      0x00B02680 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 0 (W) */
#define B2_TRAP_L_R1                      0x00B02684 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 1 (W) */
#define B2_TRAP_L_R2                      0x00B02688 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 2 (W) */
#define B2_TRAP_L_R3                      0x00B0268C /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 3 (W) */
#define B2_TRAP_L_R4                      0x00B02690 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 4 (W) */
#define B2_TRAP_L_R5                      0x00B02694 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 5 (W) */
#define B2_TRAP_L_R6                      0x00B02698 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 6 (W) */
#define B2_TRAP_L_R8                      0x00B026A0 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 8 (W) */
#define B2_TRAP_L_R9                      0x00B026A4 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 9 (W) */
#define B2_TRAP_L_R10                     0x00B026A8 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 10 (W) */
#define B2_TRAP_L_R11                     0x00B026AC /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 11 (W) */
#define B2_TRAP_L_R12                     0x00B026B0 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 12 (W) */
#define B2_TRAP_L_R13                     0x00B026B4 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 13 (W) */
#define B2_TRAP_L_TD                      0x00B026C4 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register TD (W) */
#define B2_TRAP_R_R0                      0x00B02700 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 0 (W) */
#define B2_TRAP_R_R1                      0x00B02704 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 1 (W) */
#define B2_TRAP_R_R2                      0x00B02708 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 2 (W) */
#define B2_TRAP_R_R3                      0x00B0270C /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 3 (W) */
#define B2_TRAP_R_R4                      0x00B02710 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 4 (W) */
#define B2_TRAP_R_R5                      0x00B02714 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 5 (W) */
#define B2_TRAP_R_R6                      0x00B02718 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 6 (W) */
#define B2_TRAP_R_R8                      0x00B02720 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 8 (W) */
#define B2_TRAP_R_R9                      0x00B02724 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 9 (W) */
#define B2_TRAP_R_R10                     0x00B02728 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 10 (W) */
#define B2_TRAP_R_R11                     0x00B0272C /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 11 (W) */
#define B2_TRAP_R_R12                     0x00B02730 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 12 (W) */
#define B2_TRAP_R_R13                     0x00B02734 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 13 (W) */
#define B2_TRAP_R_TD                      0x00B02744 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register TD (W) */
#define B2_TRAP_B_R0                      0x00B02780 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 0 (W) */
#define B2_TRAP_B_R1                      0x00B02784 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 1 (W) */
#define B2_TRAP_B_R2                      0x00B02788 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 2 (W) */
#define B2_TRAP_B_R3                      0x00B0278C /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 3 (W) */
#define B2_TRAP_B_R4                      0x00B02790 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 4 (W) */
#define B2_TRAP_B_R5                      0x00B02794 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 5 (W) */
#define B2_TRAP_B_R6                      0x00B02798 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 6 (W) */
#define B2_TRAP_B_R8                      0x00B027A0 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 8 (W) */
#define B2_TRAP_B_R9                      0x00B027A4 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 9 (W) */
#define B2_TRAP_B_R10                     0x00B027A8 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 10 (W) */
#define B2_TRAP_B_R11                     0x00B027AC /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 11 (W) */
#define B2_TRAP_B_R12                     0x00B027B0 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 12 (W) */
#define B2_TRAP_B_R13                     0x00B027B4 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 13 (W) */
#define B2_TRAP_B_TD                      0x00B027C4 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register TD (W) */
/*  End of MFU Opcodes  */

#define B2_SR0                            0x00B04000 /* MFU Slave Register 0 (RW) */
#define B2_SR1                            0x00B04004 /* MFU Slave Register 1 (RW) */
#define B2_SR2                            0x00B04008 /* MFU Slave Register 2 (RW) */
#define B2_SR3                            0x00B0400C /* MFU Slave Register 3 (RW) */
#define B2_SR4                            0x00B04010 /* MFU Slave Register 4 (RW) */
#define B2_SR5                            0x00B04014 /* MFU Slave Register 5 (RW) */
#define B2_SR6                            0x00B04018 /* MFU Slave Register 6 (RW) */
#define B2_SR7                            0x00B0401C /* MFU Slave Register 7 (RW) */
#define B2_SR8                            0x00B04020 /* MFU Slave Register 8 (RW) */
#define B2_SR9                            0x00B04024 /* MFU Slave Register 9 (RW) */
#define B2_SR10                           0x00B04028 /* MFU Slave Register 10 (RW) */
#define B2_SR11                           0x00B0402C /* MFU Slave Register 11 (RW) */
#define B2_SR12                           0x00B04030 /* MFU Slave Register 12 (RW) */
#define B2_SR13                           0x00B04034 /* MFU Slave Register 13 (RW) */
#define B2_SR14                           0x00B04038 /* MFU Slave Register 14 (RW) */
#define B2_SR15                           0x00B0403C /* MFU Slave Register 15 (RW) */
#define B2_STD                            0x00B04044 /* MFU Slave Transfer Data Register (RW) */
#define B2_SOV                            0x00B08000 /* MFU Scoreboard Override Register (RW) */
#define B2_SB                             0x00B08004 /* MFU Scoreboard Register (R) */
#define B2_VTB                            0x00B08008 /* MFU Vector Tie Breaker Register (RW) */
#define B2_TTB                            0x00B0800C /* MFU Trapezoid Tie Breaker Register (RW) */
#define B2_SV1                            0x00B08010 /* MFU Styled Vector Register 1 (RW) */
#define B2_SV2                            0x00B08014 /* MFU Styled Vector Register 2 (RW) */
#define B2_SV3                            0x00B08018 /* MFU Styled Vector Register 3 (RW) */
#define B2_SV4                            0x00B0801C /* MFU Styled Vector Register 4 (RW) */
#define B2_SV5                            0x00B08020 /* MFU Styled Vector Register 5 (RW) */
#define B2_MFU_DIAG                       0x00B08040 /* MFU Diagnostic Register (R) */
#define B2_MFU_BSCTD                      0x00B08044 /* MFU Byte Swap Control Register (TD operations) (RW) */
#define B2_MFU_BSCCTL                     0x00B08048 /* MFU Byte Swap Control Register (register pair opcodes) (RW) */
#define B2_MNOOP                          0x00B0C000 /* MFU Noop Opcode / Don't Load Any Master Registers (W) */
#define B2_NOSB                           0x00B0C080 /* MFU Noop Opcode (No Scoreboard) / Don't Load Any Master Registers (W) */
#define B2_MSLOAD                         0x00B0C100 /* Master To Slave Load Opcode / Don't Load Any Master Registers (W) */
#define B2_TEXT16                         0x00B0C400 /* 16 Bit Text Fill Opcode / Don't Load Any Master Registers (W) */
#define B2_TEXT8                          0x00B0C480 /* 8 Bit Text Fill Opcode / Don't Load Any Master Registers (W) */
#define B2_TEXT16SETUP                    0x00B0C500 /* 16 Bit Text Fill Setup Opcode / Don't Load Any Master Registers (W) */
#define B2_TEXT8SETUP                     0x00B0C580 /* 8 Bit Text Fill Setup Opcode / Don't Load Any Master Registers (W) */
#define B2_SOLIDFILL                      0x00B0C800 /* Solid Fill Opcode / Don't Load Any Master Registers (W) */
#define B2_SOLIDFILLT                     0x00B0C880 /* Solid Fill Throttled Opcode / Don't Load Any Master Registers (W) */
#define B2_TDFILL                         0x00B0C900 /* TD Fill Opcode / Don't Load Any Master Registers (W) */
#define B2_TDFILLT                        0x00B0C980 /* TD Fill Throttled Opcode / Don't Load Any Master Registers (W) */
#define B2_BLT                            0x00B0CC00 /* Blt Opcode / Don't Load Any Master Registers (W) */
#define B2_EXBLT                          0x00B0CC80 /* Expanding Blt Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC                            0x00B0D000 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC_E                          0x00B0D080 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC_C                          0x00B0D100 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC_CE                         0x00B0D180 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC_D                          0x00B0D200 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC_DE                         0x00B0D280 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC_DC                         0x00B0D300 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC_DCE                        0x00B0D380 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC_S                          0x00B0D400 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC_SE                         0x00B0D480 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC_SC                         0x00B0D500 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC_SCE                        0x00B0D580 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC_SD                         0x00B0D600 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC_SDE                        0x00B0D680 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC_SDC                        0x00B0D700 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_VEC_SDCE                       0x00B0D780 /* Vector Opcode / Don't Load Any Master Registers (W) */
#define B2_TRAP                           0x00B0E600 /* Trapezoid Opcode / Don't Load Any Master Registers (W) */
#define B2_TRAP_L                         0x00B0E680 /* Trapezoid Add Left Edge Setup Opcode / Don't Load Any Master Registers (W) */
#define B2_TRAP_R                         0x00B0E700 /* Trapezoid Add Right Edge Setup Opcode / Don't Load Any Master Registers (W) */
#define B2_TRAP_B                         0x00B0E780 /* Trapezoid Add Both Edges Setup Opcode / Don't Load Any Master Registers (W) */
#define B2_SS0                            0x00B10000 /* MFU Setup Slave Register 0 (RW) */
#define B2_SS1                            0x00B10004 /* MFU Setup Slave Register 1 (RW) */
#define B2_SS2                            0x00B10008 /* MFU Setup Slave Register 2 (RW) */
#define B2_SS3                            0x00B1000C /* MFU Setup Slave Register 3 (RW) */
#define B2_SS4                            0x00B10010 /* MFU Setup Slave Register 4 (RW) */
#define B2_SS5                            0x00B10014 /* MFU Setup Slave Register 5 (RW) */
#define B2_SS6                            0x00B10018 /* MFU Setup Slave Register 6 (RW) */
#define B2_SSTD                           0x00B10044 /* MFU Setup Slave Transfer Data Register (RW) */
/*  End of MFU Registers  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */

/*  Start of Remapped MFU Registers  */
#define B2_MNOOP_R0R1_REMAP               0x00B20000 /* Start Of MFU Noop Opcode / Load Master Register 0 and 1 (W) */
#define B2_NOSB_R0R1_REMAP                0x00B20080 /* Start Of MFU Noop Opcode (No Scoreboard) / Load Master Register 0 and 1 (W) */
#define B2_MSLOAD_R0R1_REMAP              0x00B20100 /* Master To Slave Load Opcode / Load Master Register 0 and 1 (W) */
#define B2_TEXT16_R0R1_REMAP              0x00B20400 /* 16 Bit Text Fill Opcode / Load Master Register 0 and 1 (W) */
#define B2_TEXT8_R0R1_REMAP               0x00B20480 /* 8 Bit Text Fill Opcode / Load Master Register 0 and 1 (W) */
#define B2_TEXT16SETUP_R0R1_REMAP         0x00B20500 /* 16 Bit Text Fill Setup Opcode / Load Master Register 0 and 1 (W) */
#define B2_TEXT8SETUP_R0R1_REMAP          0x00B20580 /* 8 Bit Text Fill Setup Opcode / Load Master Register 0 and 1 (W) */
#define B2_SOLIDFILL_R0R1_REMAP           0x00B20800 /* Solid Fill Opcode / Load Master Register 0 and 1 (W) */
#define B2_SOLIDFILLT_R0R1_REMAP          0x00B20880 /* Solid Fill Throttled Opcode / Load Master Register 0 and 1 (W) */
#define B2_TDFILL_R0R1_REMAP              0x00B20900 /* TD Fill Opcode / Load Master Register 0 and 1 (W) */
#define B2_TDFILLT_R0R1_REMAP             0x00B20980 /* TD Fill Throttled Opcode / Load Master Register 0 and 1 (W) */
#define B2_BLT_R0R1_REMAP                 0x00B20C00 /* Blt Opcode / Load Master Register 0 and 1 (W) */
#define B2_EXBLT_R0R1_REMAP               0x00B20C80 /* Expanding Blt Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_R0R1_REMAP                 0x00B21000 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_VEC_E_R0R1_REMAP               0x00B21080 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_VEC_C_R0R1_REMAP               0x00B21100 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_VEC_CE_R0R1_REMAP              0x00B21180 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_VEC_D_R0R1_REMAP               0x00B21200 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_VEC_DE_R0R1_REMAP              0x00B21280 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_VEC_DC_R0R1_REMAP              0x00B21300 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_VEC_DCE_R0R1_REMAP             0x00B21380 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_VEC_S_R0R1_REMAP               0x00B21400 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_VEC_SE_R0R1_REMAP              0x00B21480 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_VEC_SC_R0R1_REMAP              0x00B21500 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_VEC_SCE_R0R1_REMAP             0x00B21580 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_VEC_SD_R0R1_REMAP              0x00B21600 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_VEC_SDE_R0R1_REMAP             0x00B21680 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_VEC_SDC_R0R1_REMAP             0x00B21700 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_VEC_SDCE_R0R1_REMAP            0x00B21780 /* Vector Opcode / Load Master Registers 0 and 1 (W) */
#define B2_TRAP_R0R1_REMAP                0x00B22600 /* Trapezoid Opcode / Load Master Register 0 and 1 (W) */
#define B2_TRAP_L_R0R1_REMAP              0x00B22680 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 0 and 1 (W) */
#define B2_TRAP_R_R0R1_REMAP              0x00B22700 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 0 and 1 (W) */
#define B2_TRAP_B_R0R1_REMAP              0x00B22780 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 0 and 1 (W) */
#define B2_MNOOP_R2R3_REMAP               0x00B24000 /* Start Of MFU Noop Opcode / Load Master Register 2 and 3 (W) */
#define B2_NOSB_R2R3_REMAP                0x00B24080 /* Start Of MFU Noop Opcode (No Scoreboard) / Load Master Register 2 and 3 (W) */
#define B2_MSLOAD_R2R3_REMAP              0x00B24100 /* Master To Slave Load Opcode / Load Master Register 2 and 3 (W) */
#define B2_TEXT16_R2R3_REMAP              0x00B24400 /* 16 Bit Text Fill Opcode / Load Master Register 2 and 3 (W) */
#define B2_TEXT8_R2R3_REMAP               0x00B24480 /* 8 Bit Text Fill Opcode / Load Master Register 2 and 3 (W) */
#define B2_TEXT16SETUP_R2R3_REMAP         0x00B24500 /* 16 Bit Text Fill Setup Opcode / Load Master Register 2 and 3 (W) */
#define B2_TEXT8SETUP_R2R3_REMAP          0x00B24580 /* 8 Bit Text Fill Setup Opcode / Load Master Register 2 and 3 (W) */
#define B2_SOLIDFILL_R2R3_REMAP           0x00B24800 /* Solid Fill Opcode / Load Master Register 2 and 3 (W) */
#define B2_SOLIDFILLT_R2R3_REMAP          0x00B24880 /* Solid Fill Throttled Opcode / Load Master Register 2 and 3 (W) */
#define B2_TDFILL_R2R3_REMAP              0x00B24900 /* TD Fill Opcode / Load Master Register 2 and 3 (W) */
#define B2_TDFILLT_R2R3_REMAP             0x00B24980 /* TD Fill Throttled Opcode / Load Master Register 2 and 3 (W) */
#define B2_BLT_R2R3_REMAP                 0x00B24C00 /* Blt Opcode / Load Master Register 2 and 3 (W) */
#define B2_EXBLT_R2R3_REMAP               0x00B24C80 /* Expanding Blt Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_R2R3_REMAP                 0x00B25000 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_VEC_E_R2R3_REMAP               0x00B25080 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_VEC_C_R2R3_REMAP               0x00B25100 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_VEC_CE_R2R3_REMAP              0x00B25180 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_VEC_D_R2R3_REMAP               0x00B25200 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_VEC_DE_R2R3_REMAP              0x00B25280 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_VEC_DC_R2R3_REMAP              0x00B25300 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_VEC_DCE_R2R3_REMAP             0x00B25380 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_VEC_S_R2R3_REMAP               0x00B25400 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_VEC_SE_R2R3_REMAP              0x00B25480 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_VEC_SC_R2R3_REMAP              0x00B25500 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_VEC_SCE_R2R3_REMAP             0x00B25580 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_VEC_SD_R2R3_REMAP              0x00B25600 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_VEC_SDE_R2R3_REMAP             0x00B25680 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_VEC_SDC_R2R3_REMAP             0x00B25700 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_VEC_SDCE_R2R3_REMAP            0x00B25780 /* Vector Opcode / Load Master Registers 2 and 3 (W) */
#define B2_TRAP_R2R3_REMAP                0x00B26600 /* Trapezoid Opcode / Load Master Register 2 and 3 (W) */
#define B2_TRAP_L_R2R3_REMAP              0x00B26680 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 2 and 3 (W) */
#define B2_TRAP_R_R2R3_REMAP              0x00B26700 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 2 and 3 (W) */
#define B2_TRAP_B_R2R3_REMAP              0x00B26780 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 2 and 3 (W) */
#define B2_MNOOP_TD_REMAP                 0x00B28000 /* Start Of MFU Noop Opcode / Load Master Register TD (W) */
#define B2_NOSB_TD_REMAP                  0x00B28080 /* Start Of MFU Noop Opcode (No Scoreboard) / Load Master Register TD (W) */
#define B2_MSLOAD_TD_REMAP                0x00B28100 /* Master To Slave Load Opcode / Load Master Register TD (W) */
#define B2_TEXT16_TD_REMAP                0x00B28400 /* 16 Bit Text Fill Opcode / Load Master Register TD (W) */
#define B2_TEXT8_TD_REMAP                 0x00B28480 /* 8 Bit Text Fill Opcode / Load Master Register TD (W) */
#define B2_TEXT16SETUP_TD_REMAP           0x00B28500 /* 16 Bit Text Fill Setup Opcode / Load Master Register TD (W) */
#define B2_TEXT8SETUP_TD_REMAP            0x00B28580 /* 8 Bit Text Fill Setup Opcode / Load Master Register TD (W) */
#define B2_SOLIDFILL_TD_REMAP             0x00B28800 /* Solid Fill Opcode / Load Master Register TD (W) */
#define B2_SOLIDFILLT_TD_REMAP            0x00B28880 /* Solid Fill Throttled Opcode / Load Master Register TD (W) */
#define B2_TDFILL_TD_REMAP                0x00B28900 /* TD Fill Opcode / Load Master Register TD (W) */
#define B2_TDFILLT_TD_REMAP               0x00B28980 /* TD Fill Throttled Opcode / Load Master Register TD (W) */
#define B2_BLT_TD_REMAP                   0x00B28C00 /* Blt Opcode / Load Master Register TD (W) */
#define B2_EXBLT_TD_REMAP                 0x00B28C80 /* Expanding Blt Opcode / Load Master Register TD (W) */
#define B2_VEC_TD_REMAP                   0x00B29000 /* Vector Opcode / Load Master Registers TD (W) */
#define B2_VEC_E_TD_REMAP                 0x00B29080 /* Vector Opcode / Load Master Registers TD (W) */
#define B2_VEC_C_TD_REMAP                 0x00B29100 /* Vector Opcode / Load Master Registers TD (W) */
#define B2_VEC_CE_TD_REMAP                0x00B29180 /* Vector Opcode / Load Master Registers TD (W) */
#define B2_VEC_D_TD_REMAP                 0x00B29200 /* Vector Opcode / Load Master Registers TD (W) */
#define B2_VEC_DE_TD_REMAP                0x00B29280 /* Vector Opcode / Load Master Registers TD (W) */
#define B2_VEC_DC_TD_REMAP                0x00B29300 /* Vector Opcode / Load Master Registers TD (W) */
#define B2_VEC_DCE_TD_REMAP               0x00B29380 /* Vector Opcode / Load Master Registers TD (W) */
#define B2_VEC_S_TD_REMAP                 0x00B29400 /* Vector Opcode / Load Master Registers TD (W) */
#define B2_VEC_SE_TD_REMAP                0x00B29480 /* Vector Opcode / Load Master Registers TD (W) */
#define B2_VEC_SC_TD_REMAP                0x00B29500 /* Vector Opcode / Load Master RegistersTD (W) */
#define B2_VEC_SCE_TD_REMAP               0x00B29580 /* Vector Opcode / Load Master Registers TD (W) */
#define B2_VEC_SD_TD_REMAP                0x00B29600 /* Vector Opcode / Load Master Registers TD (W) */
#define B2_VEC_SDE_TD_REMAP               0x00B29680 /* Vector Opcode / Load Master Registers TD (W) */
#define B2_VEC_SDC_TD_REMAP               0x00B29700 /* Vector Opcode / Load Master Registers TD (W) */
#define B2_VEC_SDCE_TD_REMAP              0x00B29780 /* Vector Opcode / Load Master Registers TD (W) */
#define B2_TRAP_TD_REMAP                  0x00B2A600 /* Trapezoid Opcode / Load Master Register TD (W) */
#define B2_TRAP_L_TD_REMAP                0x00B2A680 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register TD (W) */
#define B2_TRAP_R_TD_REMAP                0x00B2A700 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register TD (W) */
#define B2_TRAP_B_TD_REMAP                0x00B2A780 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register TD (W) */
#define B2_MNOOP_ALT                      0x00B2C000 /* Start Of MFU Noop Opcode / Load Master Register Alternating Between MNOOP_R0R1 and MNOOP_R2R3 (W) */
#define B2_NOSB_ALT                       0x00B2C080 /* Start Of MFU Noop Opcode (No Scoreboard) (W) */
#define B2_MSLOAD_ALT                     0x00B2C100 /* Master To Slave Load Opcode (W) */
#define B2_TEXT16_ALT                     0x00B2C400 /* 16 Bit Text Fill Opcode / Load Master Register Alternating Between MNOOP_R0R1 and TEXT16_R2R3 (W) */
#define B2_TEXT8_ALT                      0x00B2C480 /* 8 Bit Text Fill Opcode / Load Master Register Alternating Between MNOOP_R0R1 and TEXT8_R2R3 (W) */
#define B2_TEXT16SETUP_ALT                0x00B2C500 /* 16 Bit Text Fill Setup Opcode (W) */
#define B2_TEXT8SETUP_ALT                 0x00B2C580 /* 8 Bit Text Fill Setup Opcode (W) */
#define B2_SOLIDFILL_ALT                  0x00B2C800 /* Solid Fill Opcode / Load Master Register Alternating Between MNOOP_R0R1 and SOLIDFILL_R2R3 (W) */
#define B2_SOLIDFILLT_ALT                 0x00B2C880 /* Solid Fill Throttled Opcode / Load Master Register Alternating Between MNOOP_R0R1/SOLIDFILLT_R2R3 (W) */
#define B2_TDFILL_ALT                     0x00B2C900 /* TD Fill Opcode / Load Master Register Alternating Between MNOOP_R0R1 and TDFILL_R2R3 (W) */
#define B2_TDFILLT_ALT                    0x00B2C980 /* TD Fill Throttled Opcode / Load Master Register Alternating Between MNOOP_R0R1 and TDFILLT_R2R3 (W) */
#define B2_BLT_ALT                        0x00B2CC00 /* Blt Opcode / Load Master Register Alternating Between MNOOP_R0R1 and BLT_R2R3 (W) */
#define B2_EXBLT_ALT                      0x00B2CC80 /* Expanding Blt Opcode / Load Master Register Alternating Between MNOOP_R0R1 and EXBLT_R2R3 (W) */
#define B2_VEC_ALT                        0x00B2D000 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_R2R3 (W) */
#define B2_VEC_E_ALT                      0x00B2D080 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_E_R2R3 (W) */
#define B2_VEC_C_ALT                      0x00B2D100 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_C_R2R3 (W) */
#define B2_VEC_CE_ALT                     0x00B2D180 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_CE_R2R3 (W) */
#define B2_VEC_D_ALT                      0x00B2D200 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_X_R2R3 (W) */
#define B2_VEC_DE_ALT                     0x00B2D280 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_XE_R2R3 (W) */
#define B2_VEC_DC_ALT                     0x00B2D300 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_XC_R2R3 (W) */
#define B2_VEC_DCE_ALT                    0x00B2D380 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_XCE_R2R3 (W) */
#define B2_VEC_S_ALT                      0x00B2D400 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_S_R2R3 (W) */
#define B2_VEC_SE_ALT                     0x00B2D480 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_SE_R2R3 (W) */
#define B2_VEC_SC_ALT                     0x00B2D500 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_SC_R2R3 (W) */
#define B2_VEC_SCE_ALT                    0x00B2D580 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_SCE_R2R3 (W) */
#define B2_VEC_SD_ALT                     0x00B2D600 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_SX_R2R3 (W) */
#define B2_VEC_SDE_ALT                    0x00B2D680 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_SXE_R2R3 (W) */
#define B2_VEC_SDC_ALT                    0x00B2D700 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_SXC_R2R3 (W) */
#define B2_VEC_SDCE_ALT                   0x00B2D780 /* Vector Opcode / Load Master Registers Alternating Between MNOOP_R0R1 and VEC_SXCE_R2R3 (W) */
#define B2_TRAP_ALT                       0x00B2E600 /* Trapezoid Opcode / Load Master Register Alternating Between MNOOP_R0R1 and TRAP_R2R3 (W) */
#define B2_TRAP_L_ALT                     0x00B2E680 /* Trapezoid Add Left Edge Setup Opcode (W) */
#define B2_TRAP_R_ALT                     0x00B2E700 /* Trapezoid Add Right Edge Setup Opcode (W) */
#define B2_TRAP_B_ALT                     0x00B2E780 /* Trapezoid Add Both Edges Setup Opcode  (W) */
#define B2_MNOOP_R0R1                     0x00B3C000 /* MFU Noop Opcode / Load Master Register 0 and 1 (W) */
#define B2_MNOOP_R2R3                     0x00B3C008 /* MFU Noop Opcode / Load Master Register 2 and 3 (W) */
#define B2_MNOOP_R4R5                     0x00B3C010 /* MFU Noop Opcode / Load Master Register 4 and 5 (W) */
#define B2_MNOOP_R6R7                     0x00B3C018 /* MFU Noop Opcode / Load Master Register 6 and 7 (W) */
#define B2_MNOOP_R8R9                     0x00B3C020 /* MFU Noop Opcode / Load Master Register 8 and 9 (W) */
#define B2_MNOOP_R10R11                   0x00B3C028 /* MFU Noop Opcode / Load Master Register 10 and 11 (W) */
#define B2_MNOOP_R12R13                   0x00B3C030 /* MFU Noop Opcode / Load Master Register 12 and 13 (W) */
#define B2_NOSB_R0R1                      0x00B3C080 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 0 and 1 (W) */
#define B2_NOSB_R2R3                      0x00B3C088 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 2 and 3 (W) */
#define B2_NOSB_R4R5                      0x00B3C090 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 4 and 5 (W) */
#define B2_NOSB_R6R7                      0x00B3C098 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 6 and 7 (W) */
#define B2_NOSB_R8R9                      0x00B3C0A0 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 8 and 9 (W) */
#define B2_NOSB_R10R11                    0x00B3C0A8 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 10 and 11 (W) */
#define B2_NOSB_R12R13                    0x00B3C0B0 /* MFU Noop Opcode (No Scoreboard) / Load Master Register 12 and 13 (W) */
#define B2_MSLOAD_R0R1                    0x00B3C100 /* Master To Slave Load Opcode / Load Master Register 0 and 1 (W) */
#define B2_MSLOAD_R2R3                    0x00B3C108 /* Master To Slave Load Opcode / Load Master Register 2 and 3 (W) */
#define B2_MSLOAD_R4R5                    0x00B3C110 /* Master To Slave Load Opcode / Load Master Register 4 and 5 (W) */
#define B2_MSLOAD_R6R7                    0x00B3C118 /* Master To Slave Load Opcode / Load Master Register 6 and 7 (W) */
#define B2_MSLOAD_R8R9                    0x00B3C120 /* Master To Slave Load Opcode / Load Master Register 8 and 9 (W) */
#define B2_MSLOAD_R10R11                  0x00B3C128 /* Master To Slave Load Opcode / Load Master Register 10 and 11 (W) */
#define B2_MSLOAD_R12R13                  0x00B3C130 /* Master To Slave Load Opcode / Load Master Register 12 and 13 (W) */
#define B2_TEXT16_R0R1                    0x00B3C400 /* 16 Bit Text Fill Opcode / Load Master Register 0 and 1 (W) */
#define B2_TEXT16_R2R3                    0x00B3C408 /* 16 Bit Text Fill Opcode / Load Master Register 2 and 3 (W) */
#define B2_TEXT8_R0R1                     0x00B3C480 /* 8 Bit Text Fill Opcode / Load Master Register 0 and 1 (W) */
#define B2_TEXT8_R2R3                     0x00B3C488 /* 8 Bit Text Fill Opcode / Load Master Register 2 and 3 (W) */
#define B2_TEXT16SETUP_R0R1               0x00B3C500 /* 16 Bit Text Fill Setup Opcode / Load Master Register 0 and 1 (W) */
#define B2_TEXT16SETUP_R2R3               0x00B3C508 /* 16 Bit Text Fill Setup Opcode / Load Master Register 2 and 3 (W) */
#define B2_TEXT8SETUP_R0R1                0x00B3C580 /* 8 Bit Text Fill Setup Opcode / Load Master Register 0 and 1 (W) */
#define B2_TEXT8SETUP_R2R3                0x00B3C588 /* 8 Bit Text Fill Setup Opcode / Load Master Register 2 and 3 (W) */
#define B2_SOLIDFILL_R0R1                 0x00B3C800 /* Solid Fill Opcode / Load Master Register 0 and 1 (W) */
#define B2_SOLIDFILL_R2R3                 0x00B3C808 /* Solid Fill Opcode / Load Master Register 2 and 3 (W) */
#define B2_SOLIDFILLT_R0R1                0x00B3C880 /* Solid Fill Throttled Opcode / Load Master Register 0 and 1 (W) */
#define B2_SOLIDFILLT_R2R3                0x00B3C888 /* Solid Fill Throttled Opcode / Load Master Register 2 and 3 (W) */
#define B2_TDFILL_R0R1                    0x00B3C900 /* TD Fill Opcode / Load Master Register 0 and 1 (W) */
#define B2_TDFILL_R2R3                    0x00B3C908 /* TD Fill Opcode / Load Master Register 2 and 3 (W) */
#define B2_TDFILLT_R0R1                   0x00B3C980 /* TD Fill Throttled Opcode / Load Master Register 0 and 1 (W) */
#define B2_TDFILLT_R2R3                   0x00B3C988 /* TD Fill Throttled Opcode / Load Master Register 2 and 3 (W) */
#define B2_BLT_R0R1                       0x00B3CC00 /* Blt Opcode / Load Master Register 0 and 1 (W) */
#define B2_BLT_R2R3                       0x00B3CC08 /* Blt Opcode / Load Master Register 2 and 3 (W) */
#define B2_BLT_R4R5                       0x00B3CC10 /* Blt Opcode / Load Master Register 4 and 5 (W) */
#define B2_EXBLT_R0R1                     0x00B3CC80 /* Expanding Blt Opcode / Load Master Register 0 and 1 (W) */
#define B2_EXBLT_R2R3                     0x00B3CC88 /* Expanding Blt Opcode / Load Master Register 2 and 3 (W) */
#define B2_EXBLT_R4R5                     0x00B3CC90 /* Expanding Blt Opcode / Load Master Register 4 and 5 (W) */
#define B2_EXBLT_R10R11                   0x00B3CCA8 /* Expanding Blt Opcode / Load Master Register 10 and 11 (W) */
#define B2_VEC_R0R1                       0x00B3D000 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_R2R3                       0x00B3D008 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_R4R5                       0x00B3D010 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_R6R7                       0x00B3D018 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_VEC_E_R0R1                     0x00B3D080 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_E_R2R3                     0x00B3D088 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_E_R4R5                     0x00B3D090 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_E_R6R7                     0x00B3D098 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_VEC_C_R0R1                     0x00B3D100 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_C_R2R3                     0x00B3D108 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_C_R4R5                     0x00B3D110 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_C_R6R7                     0x00B3D118 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_VEC_CE_R0R1                    0x00B3D180 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_CE_R2R3                    0x00B3D188 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_CE_R4R5                    0x00B3D190 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_CE_R6R7                    0x00B3D198 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_VEC_D_R0R1                     0x00B3D200 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_D_R2R3                     0x00B3D208 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_D_R4R5                     0x00B3D210 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_D_R6R7                     0x00B3D218 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_VEC_DE_R0R1                    0x00B3D280 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_DE_R2R3                    0x00B3D288 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_DE_R4R5                    0x00B3D290 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_DE_R6R7                    0x00B3D298 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_VEC_DC_R0R1                    0x00B3D300 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_DC_R2R3                    0x00B3D308 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_DC_R4R5                    0x00B3D310 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_DC_R6R7                    0x00B3D318 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_VEC_DCE_R0R1                   0x00B3D380 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_DCE_R2R3                   0x00B3D388 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_DCE_R4R5                   0x00B3D390 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_DCE_R6R7                   0x00B3D398 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_VEC_S_R0R1                     0x00B3D400 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_S_R2R3                     0x00B3D408 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_S_R4R5                     0x00B3D410 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_S_R6R7                     0x00B3D418 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_VEC_SE_R0R1                    0x00B3D480 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_SE_R2R3                    0x00B3D488 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_SE_R4R5                    0x00B3D490 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_SE_R6R7                    0x00B3D498 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_VEC_SC_R0R1                    0x00B3D500 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_SC_R2R3                    0x00B3D508 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_SC_R4R5                    0x00B3D510 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_SC_R6R7                    0x00B3D518 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_VEC_SCE_R0R1                   0x00B3D580 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_SCE_R2R3                   0x00B3D588 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_SCE_R4R5                   0x00B3D590 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_SCE_R6R7                   0x00B3D598 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_VEC_SD_R0R1                    0x00B3D600 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_SD_R2R3                    0x00B3D608 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_SD_R4R5                    0x00B3D610 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_SD_R6R7                    0x00B3D618 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_VEC_SDE_R0R1                   0x00B3D680 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_SDE_R2R3                   0x00B3D688 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_SDE_R4R5                   0x00B3D690 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_SDE_R6R7                   0x00B3D698 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_VEC_SDC_R0R1                   0x00B3D700 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_SDC_R2R3                   0x00B3D708 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_SDC_R4R5                   0x00B3D710 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_SDC_R6R7                   0x00B3D718 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_VEC_SDCE_R0R1                  0x00B3D780 /* Vector Opcode / Load Master Register 0 and 1 (W) */
#define B2_VEC_SDCE_R2R3                  0x00B3D788 /* Vector Opcode / Load Master Register 2 and 3 (W) */
#define B2_VEC_SDCE_R4R5                  0x00B3D790 /* Vector Opcode / Load Master Register 4 and 5 (W) */
#define B2_VEC_SDCE_R6R7                  0x00B3D798 /* Vector Opcode / Load Master Register 6 and 7 (W) */
#define B2_TRAP_R0R1                      0x00B3E600 /* Trapezoid Opcode / Load Master Register 0 and 1 (W) */
#define B2_TRAP_R2R3                      0x00B3E608 /* Trapezoid Opcode / Load Master Register 2 and 3 (W) */
#define B2_TRAP_R4R5                      0x00B3E610 /* Trapezoid Opcode / Load Master Register 4 and 5 (W) */
#define B2_TRAP_R8R9                      0x00B3E620 /* Trapezoid Opcode / Load Master Register 8 and 9 (W) */
#define B2_TRAP_R10R11                    0x00B3E628 /* Trapezoid Opcode / Load Master Register 10 and 11 (W) */
#define B2_TRAP_R12R13                    0x00B3E630 /* Trapezoid Opcode / Load Master Register 12 and 13 (W) */
#define B2_TRAP_L_R0R1                    0x00B3E680 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 0 and 1 (W) */
#define B2_TRAP_L_R2R3                    0x00B3E688 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 2 and 3 (W) */
#define B2_TRAP_L_R4R5                    0x00B3E690 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 4 and 5 (W) */
#define B2_TRAP_L_R8R9                    0x00B3E6A0 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 8 and 9 (W) */
#define B2_TRAP_L_R10R11                  0x00B3E6A8 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 10 and 11 (W) */
#define B2_TRAP_L_R12R13                  0x00B3E6B0 /* Trapezoid Add Left Edge Setup Opcode / Load Master Register 12 and 13 (W) */
#define B2_TRAP_R_R0R1                    0x00B3E700 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 0 and 1 (W) */
#define B2_TRAP_R_R2R3                    0x00B3E708 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 2 and 3 (W) */
#define B2_TRAP_R_R4R5                    0x00B3E710 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 4 and 5 (W) */
#define B2_TRAP_R_R8R9                    0x00B3E720 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 8 and 9 (W) */
#define B2_TRAP_R_R10R11                  0x00B3E728 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 10 and 11 (W) */
#define B2_TRAP_R_R12R13                  0x00B3E730 /* Trapezoid Add Right Edge Setup Opcode / Load Master Register 12 and 13 (W) */
#define B2_TRAP_B_R0R1                    0x00B3E780 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 0 and 1 (W) */
#define B2_TRAP_B_R2R3                    0x00B3E788 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 2 and 3 (W) */
#define B2_TRAP_B_R4R5                    0x00B3E790 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 4 and 5 (W) */
#define B2_TRAP_B_R8R9                    0x00B3E7A0 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 8 and 9 (W) */
#define B2_TRAP_B_R10R11                  0x00B3E7A8 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 10 and 11 (W) */
#define B2_TRAP_B_R12R13                  0x00B3E7B0 /* Trapezoid Add Both Edges Setup Opcode / Load Master Register 12 and 13 (W) */
/*  End of Remapped MFU Registers  */

/*  Start of Reserved Address Space  */
/*  End of Reserved Address Space  */


#endif /* } */

/* END OF AUTO GENERATED CODE */
