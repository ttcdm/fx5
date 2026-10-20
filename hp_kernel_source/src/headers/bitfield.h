#ifndef _BITFIELD_H_INCLUDED
#define _BITFIELD_H_INCLUDED 1

#ifdef BITFIELD_SWAP
#define BITFIELD_1(a0)\
	a0;
#define BITFIELD_2(a0,a1)\
	a1;a0;
#define BITFIELD_3(a0,a1,a2)\
	a2;a1;a0;
#define BITFIELD_4(a0,a1,a2,a3)\
	a3;a2;a1;a0;
#define BITFIELD_5(a0,a1,a2,a3,a4)\
	a4;a3;a2;a1;a0;
#define BITFIELD_6(a0,a1,a2,a3,a4,a5)\
	a5;a4;a3;a2;a1;\
	a0;
#define BITFIELD_7(a0,a1,a2,a3,a4,a5,a6)\
	a6;a5;a4;a3;a2;\
	a1;a0;
#define BITFIELD_8(a0,a1,a2,a3,a4,a5,a6,a7)\
	a7;a6;a5;a4;a3;\
	a2;a1;a0;
#define BITFIELD_9(a0,a1,a2,a3,a4,a5,a6,a7,a8)\
	a8;a7;a6;a5;a4;\
	a3;a2;a1;a0;
#define BITFIELD_10(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9)\
	a9;a8;a7;a6;a5;\
	a4;a3;a2;a1;a0;
#define BITFIELD_11(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)\
	a10;a9;a8;a7;a6;\
	a5;a4;a3;a2;a1;\
	a0;
#define BITFIELD_12(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)\
	a11;a10;a9;a8;a7;\
	a6;a5;a4;a3;a2;\
	a1;a0;
#define BITFIELD_13(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)\
	a12;a11;a10;a9;a8;\
	a7;a6;a5;a4;a3;\
	a2;a1;a0;
#define BITFIELD_14(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13)\
	a13;a12;a11;a10;a9;\
	a8;a7;a6;a5;a4;\
	a3;a2;a1;a0;
#define BITFIELD_15(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14)\
	a14;a13;a12;a11;a10;\
	a9;a8;a7;a6;a5;\
	a4;a3;a2;a1;a0;
#define BITFIELD_16(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15)\
	a15;a14;a13;a12;a11;\
	a10;a9;a8;a7;a6;\
	a5;a4;a3;a2;a1;\
	a0;
#define BITFIELD_17(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16)\
	a16;a15;a14;a13;a12;\
	a11;a10;a9;a8;a7;\
	a6;a5;a4;a3;a2;\
	a1;a0;
#define BITFIELD_18(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17)\
	a17;a16;a15;a14;a13;\
	a12;a11;a10;a9;a8;\
	a7;a6;a5;a4;a3;\
	a2;a1;a0;
#define BITFIELD_19(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18)\
	a18;a17;a16;a15;a14;\
	a13;a12;a11;a10;a9;\
	a8;a7;a6;a5;a4;\
	a3;a2;a1;a0;
#define BITFIELD_20(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19)\
	a19;a18;a17;a16;a15;\
	a14;a13;a12;a11;a10;\
	a9;a8;a7;a6;a5;\
	a4;a3;a2;a1;a0;
#define BITFIELD_21(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20)\
	a20;a19;a18;a17;a16;\
	a15;a14;a13;a12;a11;\
	a10;a9;a8;a7;a6;\
	a5;a4;a3;a2;a1;\
	a0;
#define BITFIELD_22(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21)\
	a21;a20;a19;a18;a17;\
	a16;a15;a14;a13;a12;\
	a11;a10;a9;a8;a7;\
	a6;a5;a4;a3;a2;\
	a1;a0;
#define BITFIELD_23(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22)\
	a22;a21;a20;a19;a18;\
	a17;a16;a15;a14;a13;\
	a12;a11;a10;a9;a8;\
	a7;a6;a5;a4;a3;\
	a2;a1;a0;
#define BITFIELD_24(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23)\
	a23;a22;a21;a20;a19;\
	a18;a17;a16;a15;a14;\
	a13;a12;a11;a10;a9;\
	a8;a7;a6;a5;a4;\
	a3;a2;a1;a0;
#define BITFIELD_25(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24)\
	a24;a23;a22;a21;a20;\
	a19;a18;a17;a16;a15;\
	a14;a13;a12;a11;a10;\
	a9;a8;a7;a6;a5;\
	a4;a3;a2;a1;a0;
#define BITFIELD_26(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25)\
	a25;a24;a23;a22;a21;\
	a20;a19;a18;a17;a16;\
	a15;a14;a13;a12;a11;\
	a10;a9;a8;a7;a6;\
	a5;a4;a3;a2;a1;\
	a0;
#define BITFIELD_27(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26)\
	a26;a25;a24;a23;a22;\
	a21;a20;a19;a18;a17;\
	a16;a15;a14;a13;a12;\
	a11;a10;a9;a8;a7;\
	a6;a5;a4;a3;a2;\
	a1;a0;
#define BITFIELD_28(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27)\
	a27;a26;a25;a24;a23;\
	a22;a21;a20;a19;a18;\
	a17;a16;a15;a14;a13;\
	a12;a11;a10;a9;a8;\
	a7;a6;a5;a4;a3;\
	a2;a1;a0;
#define BITFIELD_29(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28)\
	a28;a27;a26;a25;a24;\
	a23;a22;a21;a20;a19;\
	a18;a17;a16;a15;a14;\
	a13;a12;a11;a10;a9;\
	a8;a7;a6;a5;a4;\
	a3;a2;a1;a0;
#define BITFIELD_30(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29)\
	a29;a28;a27;a26;a25;\
	a24;a23;a22;a21;a20;\
	a19;a18;a17;a16;a15;\
	a14;a13;a12;a11;a10;\
	a9;a8;a7;a6;a5;\
	a4;a3;a2;a1;a0;
#define BITFIELD_31(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30)\
	a30;a29;a28;a27;a26;\
	a25;a24;a23;a22;a21;\
	a20;a19;a18;a17;a16;\
	a15;a14;a13;a12;a11;\
	a10;a9;a8;a7;a6;\
	a5;a4;a3;a2;a1;\
	a0;
#define BITFIELD_32(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31)\
	a31;a30;a29;a28;a27;\
	a26;a25;a24;a23;a22;\
	a21;a20;a19;a18;a17;\
	a16;a15;a14;a13;a12;\
	a11;a10;a9;a8;a7;\
	a6;a5;a4;a3;a2;\
	a1;a0;
#else
#define BITFIELD_1(a0)\
	a0;
#define BITFIELD_2(a0,a1)\
	a0;a1;
#define BITFIELD_3(a0,a1,a2)\
	a0;a1;a2;
#define BITFIELD_4(a0,a1,a2,a3)\
	a0;a1;a2;a3;
#define BITFIELD_5(a0,a1,a2,a3,a4)\
	a0;a1;a2;a3;a4;
#define BITFIELD_6(a0,a1,a2,a3,a4,a5)\
	a0;a1;a2;a3;a4;\
	a5;
#define BITFIELD_7(a0,a1,a2,a3,a4,a5,a6)\
	a0;a1;a2;a3;a4;\
	a5;a6;
#define BITFIELD_8(a0,a1,a2,a3,a4,a5,a6,a7)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;
#define BITFIELD_9(a0,a1,a2,a3,a4,a5,a6,a7,a8)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;
#define BITFIELD_10(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;
#define BITFIELD_11(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;
#define BITFIELD_12(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;
#define BITFIELD_13(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;
#define BITFIELD_14(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;
#define BITFIELD_15(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;
#define BITFIELD_16(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;
#define BITFIELD_17(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;
#define BITFIELD_18(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;a17;
#define BITFIELD_19(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;a17;a18;
#define BITFIELD_20(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;a17;a18;a19;
#define BITFIELD_21(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;a17;a18;a19;\
	a20;
#define BITFIELD_22(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;a17;a18;a19;\
	a20;a21;
#define BITFIELD_23(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;a17;a18;a19;\
	a20;a21;a22;
#define BITFIELD_24(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;a17;a18;a19;\
	a20;a21;a22;a23;
#define BITFIELD_25(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;a17;a18;a19;\
	a20;a21;a22;a23;a24;
#define BITFIELD_26(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;a17;a18;a19;\
	a20;a21;a22;a23;a24;\
	a25;
#define BITFIELD_27(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;a17;a18;a19;\
	a20;a21;a22;a23;a24;\
	a25;a26;
#define BITFIELD_28(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;a17;a18;a19;\
	a20;a21;a22;a23;a24;\
	a25;a26;a27;
#define BITFIELD_29(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;a17;a18;a19;\
	a20;a21;a22;a23;a24;\
	a25;a26;a27;a28;
#define BITFIELD_30(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;a17;a18;a19;\
	a20;a21;a22;a23;a24;\
	a25;a26;a27;a28;a29;
#define BITFIELD_31(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;a17;a18;a19;\
	a20;a21;a22;a23;a24;\
	a25;a26;a27;a28;a29;\
	a30;
#define BITFIELD_32(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31)\
	a0;a1;a2;a3;a4;\
	a5;a6;a7;a8;a9;\
	a10;a11;a12;a13;a14;\
	a15;a16;a17;a18;a19;\
	a20;a21;a22;a23;a24;\
	a25;a26;a27;a28;a29;\
	a30;a31;
#endif

#endif
