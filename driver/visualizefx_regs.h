/* SPDX-License-Identifier: GPL-2.0 */
/*
 * visualizefx_regs.h — HP VISUALIZE fx5/fx10 ("Lego") register map.
 *
 * Provenance: offsets and field positions below were read from HP's own
 * driver source, hpgraphics_kernel-1.5.13-1.src.rpm (Internet Archive copy,
 * SHA-1 matches the archived digest), files headers/lego_address.h and
 * headers/lego_hw.h. Names are written fresh here; HP's long descriptions are
 * paraphrased, not copied. Register offsets and bit positions are facts.
 *
 * Bit order: HP's headers were written for big-endian HP-UX, where the first
 * listed bitfield is the most significant. HP's own STATUS comments
 * ("<31:30> UnBuffered Data Ready", "<11:0> FIFO Free Count") and the
 * CD_BUFFER_CTL layout ("2 LSBs should be zero") confirm that reading, so
 * every field position below uses MSB-first order.
 *
 * Status tags used in comments:
 *   KNOWN — offset/field stated in HP source.
 *   INFER — meaning inferred from HP's register name; verify on hardware.
 *   GAP   — not in HP's GPL source; needs hardware tracing.
 */
#ifndef __VISUALIZEFX_REGS_H__
#define __VISUALIZEFX_REGS_H__

#include <linux/bits.h>
#include <linux/bitfield.h>

/* ------------------------------------------------------------------------ *
 *  Card address space (KNOWN: lego_address.h)
 *
 *  The card's window is 32 MB aligned. Offsets are from the start of it.
 *  Which PCI BAR exposes this window on the x86 103c:100a board, and whether
 *  the FB1/FB2 aliases above 32 MB are present, still needs an lspci check.
 * ------------------------------------------------------------------------ */
#define VISFX_CTL_BASE		0x00000000u	/* 16 MB control space      */
#define VISFX_MISC_CTL_BASE	0x00000000u	/*  STI + privileged        */
#define VISFX_UNBUF_PRIV_BASE	0x00200000u
#define VISFX_BUF_PRIV_BASE	0x00280000u
#define VISFX_DIRECT_CD_BASE	0x00380000u
#define VISFX_UNBUF_CTL_BASE	0x00400000u	/*  unbuffered control      */
#define VISFX_BUF2D_CTL_BASE	0x00800000u	/*  2D buffered control     */
#define VISFX_BUF3D_CTL_BASE	0x00C00000u	/*  3D buffered control     */
#define VISFX_FB_BASE		0x01000000u	/* frame buffer window      */
#define VISFX_FB1_BASE		0x02000000u	/* alias (presence: verify) */
#define VISFX_FB2_BASE		0x03000000u	/* alias (presence: verify) */

/* ------------------------------------------------------------------------ *
 *  Bus interface status (KNOWN: lego_hw.h STATUS_Type, offset per wiki and
 *  cross-checked against NetBSD summitreg.h VISFX_STATUS)
 * ------------------------------------------------------------------------ */
#define VISFX_UB_STATUS		0x00641400u
#define   VISFX_STATUS_UBDRDY	GENMASK(31, 30)	/* unbuffered data ready     */
#define   VISFX_STATUS_BDRDY	GENMASK(29, 28)	/* buffered data ready       */
#define   VISFX_STATUS_FAULT	BIT(27)
#define   VISFX_STATUS_UBUFF	BIT(26)		/* unbuffered pipe busy      */
#define   VISFX_STATUS_IMODE	BIT(25)		/* input from CD buffer      */
#define   VISFX_STATUS_WPNE	BIT(24)		/* write pipe not empty      */
#define   VISFX_STATUS_HIS	BIT(23)		/* host interrupt sent       */
#define   VISFX_STATUS_HWIR	BIT(21)		/* high-water request        */
#define   VISFX_STATUS_LWIR	BIT(20)		/* low-water request         */
#define   VISFX_STATUS_TMIR	BIT(19)		/* texture-map request       */
#define   VISFX_STATUS_VBIR	BIT(18)		/* vertical-blank request    */
#define   VISFX_STATUS_UDCR	BIT(17)		/* unbuffered DMA complete   */
#define   VISFX_STATUS_BDCR	BIT(16)		/* buffered DMA complete     */
#define   VISFX_STATUS_UDPR	BIT(15)		/* unbuf priv DMA complete   */
#define   VISFX_STATUS_BDPR	BIT(14)		/* buf priv DMA complete     */
#define   VISFX_STATUS_FIFO	GENMASK(11, 0)	/* FIFO free count (LSBs)    */
/* "data ready" fields: 1 = word ready, 3 = double word ready (wiki). */

/* Indirect read control (KNOWN: IRC_Type; offsets per wiki) */
#define VISFX_UB_IRC		0x00643030u
#define   VISFX_IRC_OP		GENMASK(2, 1)
#define   VISFX_IRC_WTR		BIT(0)
#define   VISFX_IRC_OP_NOOP	0
#define   VISFX_IRC_OP_IREAD	1
#define   VISFX_IRC_OP_CLEAR	2
#define   VISFX_IRC_OP_SET	3

/* Command-data packet header (KNOWN: wiki, Lego96 ERS 3.5.1) */
#define VISFX_CD_WORDCOUNT	GENMASK(31, 27)
#define VISFX_CD_NOINCR		BIT(25)
#define VISFX_CD_WRAP		BIT(24)
#define VISFX_CD_ADDR		GENMASK(23, 0)

/* CD buffer control (KNOWN: UB_CD_BUFFER_CTL_Type field layout) */
#define   VISFX_CDCTL_RESET	BIT(26)
#define   VISFX_CDCTL_PTR	GENMASK(18, 0)	/* 512 KB space, 2 LSBs 0    */

/* ------------------------------------------------------------------------ *
 *  Video display processor — all in the 2D buffered region. Only the
 *  cursor position also has an unbuffered alias. (KNOWN: lego_address.h)
 * ------------------------------------------------------------------------ */
#define VISFX_VDP(off)		(VISFX_BUF2D_CTL_BASE + (off))

/* Hardware cursor */
#define VISFX_CP		VISFX_VDP(0x000)	/* position              */
#define VISFX_UB_CP		0x00400000u		/* position, unbuffered  */
#define VISFX_CA		VISFX_VDP(0x004)	/* image address         */
#define VISFX_CD		VISFX_VDP(0x008)	/* image data            */
#define VISFX_CF		VISFX_VDP(0x00C)	/* foreground colour     */
#define VISFX_CB		VISFX_VDP(0x010)	/* background colour     */

/* Colour LUT */
#define VISFX_PMASK		VISFX_VDP(0x018)	/* LUT plane mask        */
#define VISFX_MCS		VISFX_VDP(0x01C)	/* midlay colormap sel.  */
#define VISFX_LLCA		VISFX_VDP(0x020)	/* LUT load ctl/address  */
#define VISFX_LUTD		VISFX_VDP(0x024)	/* LUT data              */
#define   VISFX_LLCA_LUT0	0x0	/* KNOWN: LLCA LUT select values */
#define   VISFX_LLCA_LUT1	0x1
#define   VISFX_LLCA_LUT2	0x2
#define   VISFX_LLCA_GAMMA	0x3

/* Misc video and configuration */
#define VISFX_FATTR		VISFX_VDP(0x03C)	/* force attribute bits  */
#define VISFX_MV		VISFX_VDP(0x040)	/* misc video            */
#define VISFX_CFG		VISFX_VDP(0x044)	/* configuration         */
#define   VISFX_CFG_POS_VSYNC	BIT(19)		/* KNOWN field; INFER: 1 = + */
#define   VISFX_CFG_POS_HSYNC	BIT(18)		/* KNOWN field; INFER: 1 = + */
#define   VISFX_CFG_DUAL_MON	BIT(17)
#define   VISFX_CFG_FP_EN	BIT(16)		/* flat panel enable         */
#define   VISFX_CFG_DAC_DISABLE	BIT(14)
#define   VISFX_CFG_VMS		BIT(9)		/* INFER: meaning unknown    */
#define VISFX_SCR		VISFX_VDP(0x048)	/* stereo control        */
#define   VISFX_SCR_B		BIT(31)		/* INFER: field names only   */
#define   VISFX_SCR_P		BIT(30)
#define   VISFX_SCR_MODE	BIT(29)
#define   VISFX_SCR_FBP		BIT(28)
#define VISFX_MPC		VISFX_VDP(0x04C)	/* monitor power (DPMS); fields: GAP */

/* Timing generators */
#define VISFX_ETG		VISFX_VDP(0x058)	/* early timing; fields: GAP        */
#define VISFX_VHAL		VISFX_VDP(0x05C)	/* vertical/horizontal active length */
#define   VISFX_VHAL_ALC	GENMASK(27, 16)	/* INFER: active lines        */
#define   VISFX_VHAL_APC	GENMASK(11, 0)	/* INFER: active pixels       */
#define VISFX_HTG		VISFX_VDP(0x060)	/* horizontal timing; fields: GAP   */
#define VISFX_STG		VISFX_VDP(0x064)	/* serration timing; fields: GAP    */
#define VISFX_VTG		VISFX_VDP(0x068)	/* vertical timing                   */
#define   VISFX_VTG_VBP		GENMASK(23, 16)	/* back porch, lines          */
#define   VISFX_VTG_VSYNC	GENMASK(15, 8)	/* sync width, lines          */
#define   VISFX_VTG_VFP		GENMASK(7, 0)	/* front porch, lines         */
#define VISFX_HS		VISFX_VDP(0x06C)	/* horizontal state; fields: GAP    */
#define VISFX_VS		VISFX_VDP(0x070)	/* vertical state                    */
#define   VISFX_VS_STATE	GENMASK(13, 12)
#define     VISFX_VS_STATE_SYNC		0
#define     VISFX_VS_STATE_BACK_PORCH	1
#define     VISFX_VS_STATE_ACTIVE	2
#define     VISFX_VS_STATE_FRONT_PORCH	3
#define   VISFX_VS_COUNT	GENMASK(11, 0)	/* INFER: line counter        */
#define VISFX_DTS		VISFX_VDP(0x074)	/* timing & state (read only)        */

/* Stereo right-eye block maps, video out */
#define VISFX_SRBMAP0		VISFX_VDP(0x078)
#define VISFX_SRBMAP1		VISFX_VDP(0x07C)
#define VISFX_VOP		VISFX_VDP(0x080)	/* video out position */
#define VISFX_VOS		VISFX_VDP(0x084)	/* video out size     */
#define VISFX_VOC		VISFX_VDP(0x088)	/* video out control  */
#define VISFX_VOP1		VISFX_VDP(0x08C)

/* CRC / signature analysis (used by HP's diagnostics) */
#define VISFX_STSA		VISFX_VDP(0x090)
#define VISFX_BLSA		VISFX_VDP(0x094)
#define VISFX_BLSA_MASK		VISFX_VDP(0x0C4)
#define VISFX_SA_MASK		VISFX_VDP(0x0C8)
#define VISFX_SA_SIG		VISFX_VDP(0x0CC)

/* Clock PLLs. Offsets KNOWN; field layout and the reference frequency are
 * a GAP — HP's GPL source has no PLL structure and never programs them. */
#define VISFX_PLL_DOT_CTL	VISFX_VDP(0x0A0)	/* pixel clock        */
#define VISFX_PLL_DOT_CNT	VISFX_VDP(0x0A4)
#define VISFX_PLL_CORE_CTL	VISFX_VDP(0x0A8)
#define VISFX_PLL_CORE_CNT	VISFX_VDP(0x0AC)
#define VISFX_PLL_RAM_CTL	VISFX_VDP(0x0B0)
#define VISFX_PLL_RAM_CNT	VISFX_VDP(0x0B4)
#define VISFX_PLL_REF_CNT	VISFX_VDP(0x0B8)	/* reference counter  */

/* Monitor and DDC */
#define VISFX_MON		VISFX_VDP(0x0D0)	/* monitor test       */
#define VISFX_DDC		VISFX_VDP(0x0D4)	/* bit-banged I2C     */
#define   VISFX_DDC_SCL_PIN	BIT(3)		/* read: SCL line level   */
#define   VISFX_DDC_SDA_PIN	BIT(2)		/* read: SDA line level   */
#define   VISFX_DDC_SCL_REG	BIT(1)		/* write: drive SCL       */
#define   VISFX_DDC_SDA_REG	BIT(0)		/* write: drive SDA       */

/* Colormap function select 0..31 */
#define VISFX_CFS(n)		VISFX_VDP(0x100 + 4 * (n))

/* Display unit, attribute map, colour key, alpha, colour conversion */
#define VISFX_DUM		VISFX_VDP(0x240)
#define VISFX_ABMAP		VISFX_VDP(0x244)
#define VISFX_CKEY_HI		VISFX_VDP(0x248)
#define VISFX_CKEY_LO		VISFX_VDP(0x24C)
#define VISFX_CALPHA		VISFX_VDP(0x250)
#define VISFX_CCBC		VISFX_VDP(0x254)	/* brightness/contrast */

/* Overlay plane */
#define VISFX_OMC		VISFX_VDP(0x260)
#define VISFX_OTLS		VISFX_VDP(0x264)
#define VISFX_OXYO		VISFX_VDP(0x268)
#define VISFX_OBS		VISFX_VDP(0x26C)
#define VISFX_OBMAP0		VISFX_VDP(0x270)
#define VISFX_OBMAP1		VISFX_VDP(0x274)
#define VISFX_OUL		VISFX_VDP(0x278)
#define VISFX_OLR		VISFX_VDP(0x27C)
#define VISFX_ODS		VISFX_VDP(0x280)
#define VISFX_OSCALE		VISFX_VDP(0x284)

/* Image plane */
#define VISFX_IMD		VISFX_VDP(0x288)	/* image depth        */
#define VISFX_IMC(n)		VISFX_VDP(0x300 + 4 * (n))	/* n = 0..7 */
#define VISFX_IBMAP0		VISFX_VDP(0x320)
#define VISFX_IBMAP1		VISFX_VDP(0x324)
#define VISFX_WAIT_HSYNC	VISFX_VDP(0x330)
#define VISFX_IBS(n)		VISFX_VDP(0x340 + 4 * (n))	/* n = 0..7 */
#define VISFX_ICLR(n)		VISFX_VDP(0x360 + 4 * (n))	/* n = 0..7 */

/* Video encoder, readback, board id */
#define VISFX_EN_AR_L		0x00820200u
#define VISFX_EN_DR_L		0x00820204u
#define VISFX_EN_AR_C		0x00820208u
#define VISFX_EN_DR_C		0x0082020Cu
#define VISFX_CCC(n)		(0x00820210u + 4 * ((n) - 1))	/* n = 1..4 */
#define VISFX_R_R		0x00820220u
#define VISFX_R_G		0x00820224u
#define VISFX_R_B		0x00820228u
#define VISFX_R_ID		0x0082022Cu	/* readback board id  */
#define VISFX_S_CTRL		0x00820230u	/* video out status/control */

#endif /* __VISUALIZEFX_REGS_H__ */
