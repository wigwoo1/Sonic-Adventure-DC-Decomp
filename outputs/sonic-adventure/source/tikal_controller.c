#include "fpu_cleanup_transfer.h"
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v,unsigned n){for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
/* Static C recovery of retail Tikal controller 0cb00000..0cb00352.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_tikal_controller(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;
 goto Lcb00000;
Lcb00000: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto Lcb00002;
Lcb00002: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto Lcb00004;
Lcb00004: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto Lcb00006;
Lcb00006: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto Lcb00008;
Lcb00008: r[11]=r[4]; /* mov r4,r11 */
 goto Lcb0000a;
Lcb0000a: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto Lcb0000c;
Lcb0000c: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto Lcb0000e;
Lcb0000e: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto Lcb00010;
Lcb00010: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto Lcb00012;
Lcb00012: r[14]=rd(m,r[11]+0x20u,4); /* mov.l @(0x20,r11),r14 */
 goto Lcb00014;
Lcb00014: r[12]=rd(m,r[11]+0x24u,4); /* mov.l @(0x24,r11),r12 */
 goto Lcb00016;
Lcb00016: r[0]=sx(m,r[14]+0x4u,2); /* mov.w @(0x4,r14),r0 */
 goto Lcb00018;
Lcb00018: *sr=(*sr&~1u)|!(r[0]&0x20u); /* tst #0x20,r0 */
 goto Lcb0001a;
Lcb0001a: take=(*sr&1u);r[13]=rd(m,r[12],4);if(take)goto Lcb00042;goto Lcb0001e; /* bt/s 0x0cb00042 */
 goto Lcb0001e;
Lcb0001e: r[0]=8u; /* mov #0x8,r0 */
 goto Lcb00020;
Lcb00020: fr[3]=0; /* fldi0 fr3 */
 goto Lcb00022;
Lcb00022: fr[2]=rd(m,r[0]+r[12],4); /* fmov.s @(r0,r12),fr2 */
 goto Lcb00024;
Lcb00024: status=gt(f,sr,fr[2],fr[3]);if(status)return status; /* fcmp/gt fr3,fr2 */
 goto Lcb00026;
Lcb00026: if((*sr&1u))goto Lcb00042; /* bt 0x0cb00042 */
 goto Lcb00028;
Lcb00028: r[0]=sx(m,r[14]+0x4u,2); /* mov.w @(0x4,r14),r0 */
 goto Lcb0002a;
Lcb0002a: r[0]|=0x2u; /* or #0x2,r0 */
 goto Lcb0002c;
Lcb0002c: wr(m,r[14]+0x4u,r[0],2); /* mov.w r0,@(0x4,r14) */
 goto Lcb0002e;
Lcb0002e: r[3]=rd(m,r[14]+0x38u,4); /* mov.l @(0x38,r14),r3 */
 goto Lcb00030;
Lcb00030: r[2]=rd(m,r[3]+0x10u,4); /* mov.l @(0x10,r3),r2 */
 goto Lcb00032;
Lcb00032: r[1]=rd(m,r[2]+0x20u,4); /* mov.l @(0x20,r2),r1 */
 goto Lcb00034;
Lcb00034: r[3]=rd(m,r[1]+0x14u,4); /* mov.l @(0x14,r1),r3 */
 goto Lcb00036;
Lcb00036: wr(m,r[14]+0x14u,r[3],4); /* mov.l r3,@(0x14,r14) */
 goto Lcb00038;
Lcb00038: r[2]=rd(m,r[14]+0x38u,4); /* mov.l @(0x38,r14),r2 */
 goto Lcb0003a;
Lcb0003a: r[3]=rd(m,r[2]+0x10u,4); /* mov.l @(0x10,r2),r3 */
 goto Lcb0003c;
Lcb0003c: r[1]=rd(m,r[3]+0x20u,4); /* mov.l @(0x20,r3),r1 */
 goto Lcb0003e;
Lcb0003e: r[2]=rd(m,r[1]+0x1cu,4); /* mov.l @(0x1c,r1),r2 */
 goto Lcb00040;
Lcb00040: wr(m,r[14]+0x1cu,r[2],4); /* mov.l r2,@(0x1c,r14) */
 goto Lcb00042;
Lcb00042: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto Lcb00044;
Lcb00044: *sr=(*sr&~1u)|(r[0]==0xau); /* cmp/eq #0xa,r0 */
 goto Lcb00046;
Lcb00046: if((*sr&1u))goto Lcb00050; /* bt 0x0cb00050 */
 goto Lcb00048;
Lcb00048: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb0004a;
Lcb0004a: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb0004c;
Lcb0004c: *pr=0xcb00050u;r[4]=r[14];status=call(ctx,0xcb003a0u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb003a0 */
 goto Lcb00050;
Lcb00050: r[10]=rd(m,0x0cb00078u,4); /* mov.l 0x0cb00078,r10 */
 goto Lcb00052;
Lcb00052: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto Lcb00054;
Lcb00054: r[8]=rd(m,0x0cb00070u,4); /* mov.l 0x0cb00070,r8 */
 goto Lcb00056;
Lcb00056: r[9]=rd(m,0x0cb00074u,4); /* mov.l 0x0cb00074,r9 */
 goto Lcb00058;
Lcb00058: r[1]=12u; /* mov #0xc,r1 */
 goto Lcb0005a;
Lcb0005a: *sr=(*sr&~1u)|(r[0]>=r[1]); /* cmp/hs r1,r0 */
 goto Lcb0005c;
Lcb0005c: if(!(*sr&1u))goto Lcb00062; /* bf 0x0cb00062 */
 goto Lcb0005e;
Lcb0005e: goto Lcb002da; /* bra 0x0cb002da */
 goto Lcb00062;
Lcb00062: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto Lcb00064;
Lcb00064: r[1]=r[0]; /* mov r0,r1 */
 goto Lcb00066;
Lcb00066: r[0]=0x0cb0007cu; /* mova 0x0cb0007c,r0 */
 goto Lcb00068;
Lcb00068: r[0]=sx(m,r[1]+r[0],2); /* mov.w @(r0,r1),r0 */
 goto Lcb0006a;
Lcb0006a: switch(0xcb0006eu+r[0]){case 0xcb00094u:goto Lcb00094;case 0xcb0010eu:goto Lcb0010e;case 0xcb00146u:goto Lcb00146;case 0xcb0019eu:goto Lcb0019e;case 0xcb001c8u:goto Lcb001c8;case 0xcb001d6u:goto Lcb001d6;case 0xcb00260u:goto Lcb00260;case 0xcb00286u:goto Lcb00286;case 0xcb00294u:goto Lcb00294;case 0xcb00298u:goto Lcb00298;case 0xcb002a2u:goto Lcb002a2;case 0xcb002d2u:goto Lcb002d2;default:return -2;} /* braf r0 */
Lcb00094: r[0]=4u; /* mov #0x4,r0 */
 goto Lcb00096;
Lcb00096: r[3]=rd(m,0x0cb00200u,4); /* mov.l 0x0cb00200,r3 */
 goto Lcb00098;
Lcb00098: wr(m,r[14]+0x9u,r[0],1); /* mov.b r0,@(0x9,r14) */
 goto Lcb0009a;
Lcb0009a: r[5]=r[11]; /* mov r11,r5 */
 goto Lcb0009c;
Lcb0009c: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto Lcb0009e;
Lcb0009e: target=r[3];*pr=0xcb000a2u;r[4]=r[0]&255u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb000a2;
Lcb000a2: r[0]=80u; /* mov #0x50,r0 */
 goto Lcb000a4;
Lcb000a4: r[2]=rd(m,0x0cb00204u,4); /* mov.l 0x0cb00204,r2 */
 goto Lcb000a6;
Lcb000a6: r[13]=rd(m,r[12],4); /* mov.l @r12,r13 */
 goto Lcb000a8;
Lcb000a8: r[0]<<=2; /* shll2 r0 */
 goto Lcb000aa;
Lcb000aa: r[3]=0u; /* mov #0x0,r3 */
 goto Lcb000ac;
Lcb000ac: r[5]=rd(m,0x0cb00208u,4); /* mov.l 0x0cb00208,r5 */
 goto Lcb000ae;
Lcb000ae: wr(m,r[13]+r[0],r[2],4); /* mov.l r2,@(r0,r13) */
 goto Lcb000b0;
Lcb000b0: r[0]+=4294967268u; /* add #-0x1c,r0 */
 goto Lcb000b2;
Lcb000b2: r[6]=r[3]; /* mov r3,r6 */
 goto Lcb000b4;
Lcb000b4: wr(m,r[13]+r[0],r[3],2); /* mov.w r3,@(r0,r13) */
 goto Lcb000b6;
Lcb000b6: r[2]=1u; /* mov #0x1,r2 */
 goto Lcb000b8;
Lcb000b8: r[3]=rd(m,0x0cb0020cu,4); /* mov.l 0x0cb0020c,r3 */
 goto Lcb000ba;
Lcb000ba: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto Lcb000bc;
Lcb000bc: r[7]=r[2]; /* mov r2,r7 */
 goto Lcb000be;
Lcb000be: target=r[3];*pr=0xcb000c2u;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb000c2;
Lcb000c2: r[15]+=4u; /* add #0x4,r15 */
 goto Lcb000c4;
Lcb000c4: r[0]=67u; /* mov #0x43,r0 */
 goto Lcb000c6;
Lcb000c6: r[2]=rd(m,r[14]+0x38u,4); /* mov.l @(0x38,r14),r2 */
 goto Lcb000c8;
Lcb000c8: r[0]<<=2; /* shll2 r0 */
 goto Lcb000ca;
Lcb000ca: fr[3]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr3 */
 goto Lcb000cc;
Lcb000cc: r[0]=20u; /* mov #0x14,r0 */
 goto Lcb000ce;
Lcb000ce: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto Lcb000d0;
Lcb000d0: r[2]=rd(m,0x0cb00210u,4); /* mov.l 0x0cb00210,r2 */
 goto Lcb000d2;
Lcb000d2: wr(m,r[0]+r[3],fr[3],4); /* fmov fr3,@(r0,r3) */
 goto Lcb000d4;
Lcb000d4: r[3]=1u; /* mov #0x1,r3 */
 goto Lcb000d6;
Lcb000d6: wr(m,r[14],r[3],1); /* mov.b r3,@r14 */
 goto Lcb000d8;
Lcb000d8: target=r[2];*pr=0xcb000dcu;r[4]=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb000dc;
Lcb000dc: r[3]=rd(m,0x0cb00218u,4); /* mov.l 0x0cb00218,r3 */
 goto Lcb000de;
Lcb000de: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto Lcb000e0;
Lcb000e0: r[5]=rd(m,0x0cb00214u,4); /* mov.l 0x0cb00214,r5 */
 goto Lcb000e2;
Lcb000e2: target=r[3];*pr=0xcb000e6u;r[4]=r[0];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb000e6;
Lcb000e6: r[0]=81u; /* mov #0x51,r0 */
 goto Lcb000e8;
Lcb000e8: r[2]=rd(m,0x0cb0021cu,4); /* mov.l 0x0cb0021c,r2 */
 goto Lcb000ea;
Lcb000ea: r[6]=rd(m,0x0cb00220u,4); /* mov.l 0x0cb00220,r6 */
 goto Lcb000ec;
Lcb000ec: r[0]<<=2; /* shll2 r0 */
 goto Lcb000ee;
Lcb000ee: r[3]=rd(m,0x0cb00224u,4); /* mov.l 0x0cb00224,r3 */
 goto Lcb000f0;
Lcb000f0: r[5]=3u; /* mov #0x3,r5 */
 goto Lcb000f2;
Lcb000f2: wr(m,r[13]+r[0],r[2],4); /* mov.l r2,@(r0,r13) */
 goto Lcb000f4;
Lcb000f4: target=r[3];*pr=0xcb000f8u;r[4]=2u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb000f8;
Lcb000f8: r[4]=r[0]; /* mov r0,r4 */
 goto Lcb000fa;
Lcb000fa: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto Lcb000fc;
Lcb000fc: if((*sr&1u))goto Lcb00104; /* bt 0x0cb00104 */
 goto Lcb000fe;
Lcb000fe: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto Lcb00100;
Lcb00100: r[3]=rd(m,r[4]+0x20u,4); /* mov.l @(0x20,r4),r3 */
 goto Lcb00102;
Lcb00102: wr(m,r[3]+0x8u,r[0],1); /* mov.b r0,@(0x8,r3) */
 goto Lcb00104;
Lcb00104: r[3]=rd(m,0x0cb00228u,4); /* mov.l 0x0cb00228,r3 */
 goto Lcb00106;
Lcb00106: r[2]=rd(m,0x0cb0022cu,4); /* mov.l 0x0cb0022c,r2 */
 goto Lcb00108;
Lcb00108: wr(m,r[11]+0x18u,r[3],4); /* mov.l r3,@(0x18,r11) */
 goto Lcb0010a;
Lcb0010a: wr(m,r[11]+0x14u,r[2],4);goto Lcb002da; /* bra 0x0cb002da */
 goto Lcb0010e;
Lcb0010e: r[2]=rd(m,0x0cb00230u,4); /* mov.l 0x0cb00230,r2 */
 goto Lcb00110;
Lcb00110: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00112;
Lcb00112: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00114;
Lcb00114: target=r[2];*pr=0xcb00118u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb00118;
Lcb00118: r[3]=rd(m,0x0cb00234u,4); /* mov.l 0x0cb00234,r3 */
 goto Lcb0011a;
Lcb0011a: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb0011c;
Lcb0011c: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb0011e;
Lcb0011e: target=r[3];*pr=0xcb00122u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00122;
Lcb00122: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00124;
Lcb00124: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00126;
Lcb00126: target=r[10];*pr=0xcb0012au;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r10 */
 goto Lcb0012a;
Lcb0012a: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb0012c;
Lcb0012c: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb0012e;
Lcb0012e: target=r[8];*pr=0xcb00132u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r8 */
 goto Lcb00132;
Lcb00132: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00134;
Lcb00134: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00136;
Lcb00136: target=r[9];*pr=0xcb0013au;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto Lcb0013a;
Lcb0013a: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb0013c;
Lcb0013c: r[4]=r[13]; /* mov r13,r4 */
 goto Lcb0013e;
Lcb0013e: r[0]<<=2; /* shll2 r0 */
 goto Lcb00140;
Lcb00140: r[2]=0u; /* mov #0x0,r2 */
 goto Lcb00142;
Lcb00142: wr(m,r[4]+r[0],r[2],2);goto Lcb002da; /* bra 0x0cb002da */
 goto Lcb00146;
Lcb00146: r[0]=74u; /* mov #0x4a,r0 */
 goto Lcb00148;
Lcb00148: r[0]<<=2; /* shll2 r0 */
 goto Lcb0014a;
Lcb0014a: r[0]=sx(m,r[13]+r[0],2); /* mov.w @(r0,r13),r0 */
 goto Lcb0014c;
Lcb0014c: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto Lcb0014e;
Lcb0014e: *sr=(*sr&~1u)|(r[0]==0x8u); /* cmp/eq #0x8,r0 */
 goto Lcb00150;
Lcb00150: if(!(*sr&1u))goto Lcb00170; /* bf 0x0cb00170 */
 goto Lcb00152;
Lcb00152: r[0]=1u; /* mov #0x1,r0 */
 goto Lcb00154;
Lcb00154: r[0]<<=8; /* shll8 r0 */
 goto Lcb00156;
Lcb00156: r[0]+=38u; /* add #0x26,r0 */
 goto Lcb00158;
Lcb00158: r[0]=sx(m,r[13]+r[0],2); /* mov.w @(r0,r13),r0 */
 goto Lcb0015a;
Lcb0015a: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto Lcb0015c;
Lcb0015c: *sr=(*sr&~1u)|(r[0]==0x8u); /* cmp/eq #0x8,r0 */
 goto Lcb0015e;
Lcb0015e: if((*sr&1u))goto Lcb00170; /* bt 0x0cb00170 */
 goto Lcb00160;
Lcb00160: r[1]=4294967232u; /* mov #-0x40,r1 */
 goto Lcb00162;
Lcb00162: r[1]=r[1]&255u; /* extu.b r1,r1 */
 goto Lcb00164;
Lcb00164: r[1]+=r[13]; /* add r13,r1 */
 goto Lcb00166;
Lcb00166: r[0]=56u; /* mov #0x38,r0 */
 goto Lcb00168;
Lcb00168: fr[3]=rd(m,r[1],4); /* fmov.s @r1,fr3 */
 goto Lcb0016a;
Lcb0016a: fr[2]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr2 */
 goto Lcb0016c;
Lcb0016c: status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status; /* fadd fr3,fr2 */
 goto Lcb0016e;
Lcb0016e: wr(m,r[0]+r[13],fr[2],4); /* fmov fr2,@(r0,r13) */
 goto Lcb00170;
Lcb00170: r[3]=rd(m,0x0cb00234u,4); /* mov.l 0x0cb00234,r3 */
 goto Lcb00172;
Lcb00172: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00174;
Lcb00174: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00176;
Lcb00176: target=r[3];*pr=0xcb0017au;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb0017a;
Lcb0017a: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb0017c;
Lcb0017c: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb0017e;
Lcb0017e: target=r[10];*pr=0xcb00182u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r10 */
 goto Lcb00182;
Lcb00182: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00184;
Lcb00184: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00186;
Lcb00186: target=r[8];*pr=0xcb0018au;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r8 */
 goto Lcb0018a;
Lcb0018a: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb0018c;
Lcb0018c: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb0018e;
Lcb0018e: target=r[9];*pr=0xcb00192u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto Lcb00192;
Lcb00192: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00194;
Lcb00194: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00196;
Lcb00196: *pr=0xcb0019au;r[4]=r[14];status=sa_tikal_transition(m,f,sr,r);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00994 */
 goto Lcb0019a;
Lcb0019a: goto Lcb002da; /* bra 0x0cb002da */
 goto Lcb0019e;
Lcb0019e: r[2]=rd(m,0x0cb00238u,4); /* mov.l 0x0cb00238,r2 */
 goto Lcb001a0;
Lcb001a0: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb001a2;
Lcb001a2: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb001a4;
Lcb001a4: target=r[2];*pr=0xcb001a8u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb001a8;
Lcb001a8: r[3]=rd(m,0x0cb0023cu,4); /* mov.l 0x0cb0023c,r3 */
 goto Lcb001aa;
Lcb001aa: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb001ac;
Lcb001ac: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb001ae;
Lcb001ae: target=r[3];*pr=0xcb001b2u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb001b2;
Lcb001b2: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb001b4;
Lcb001b4: if((*sr&1u))goto Lcb001ba; /* bt 0x0cb001ba */
 goto Lcb001b6;
Lcb001b6: goto Lcb002b6; /* bra 0x0cb002b6 */
 goto Lcb001ba;
Lcb001ba: r[3]=rd(m,0x0cb00240u,4); /* mov.l 0x0cb00240,r3 */
 goto Lcb001bc;
Lcb001bc: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb001be;
Lcb001be: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb001c0;
Lcb001c0: target=r[3];*pr=0xcb001c4u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb001c4;
Lcb001c4: goto Lcb002b6; /* bra 0x0cb002b6 */
 goto Lcb001c8;
Lcb001c8: r[3]=rd(m,0x0cb00240u,4); /* mov.l 0x0cb00240,r3 */
 goto Lcb001ca;
Lcb001ca: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb001cc;
Lcb001cc: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb001ce;
Lcb001ce: target=r[3];*pr=0xcb001d2u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb001d2;
Lcb001d2: goto Lcb002b6; /* bra 0x0cb002b6 */
 goto Lcb001d6;
Lcb001d6: r[0]=4294967224u; /* mov #-0x48,r0 */
 goto Lcb001d8;
Lcb001d8: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto Lcb001da;
Lcb001da: fr[3]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr3 */
 goto Lcb001dc;
Lcb001dc: r[0]=60u; /* mov #0x3c,r0 */
 goto Lcb001de;
Lcb001de: fr[2]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr2 */
 goto Lcb001e0;
Lcb001e0: status=gt(f,sr,fr[3],fr[2]);if(status)return status; /* fcmp/gt fr2,fr3 */
 goto Lcb001e2;
Lcb001e2: if((*sr&1u))goto Lcb001f2; /* bt 0x0cb001f2 */
 goto Lcb001e4;
Lcb001e4: r[2]=rd(m,0x0cb00244u,4); /* mov.l 0x0cb00244,r2 */
 goto Lcb001e6;
Lcb001e6: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb001e8;
Lcb001e8: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb001ea;
Lcb001ea: target=r[2];*pr=0xcb001eeu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb001ee;
Lcb001ee: goto Lcb002b6; /* bra 0x0cb002b6 */
 goto Lcb001f2;
Lcb001f2: r[2]=rd(m,0x0cb00240u,4); /* mov.l 0x0cb00240,r2 */
 goto Lcb001f4;
Lcb001f4: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb001f6;
Lcb001f6: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb001f8;
Lcb001f8: target=r[2];*pr=0xcb001fcu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb001fc;
Lcb001fc: goto Lcb002b6; /* bra 0x0cb002b6 */
 goto Lcb00260;
Lcb00260: r[2]=rd(m,0x0cb00360u,4); /* mov.l 0x0cb00360,r2 */
 goto Lcb00262;
Lcb00262: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb00264;
Lcb00264: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00266;
Lcb00266: target=r[2];*pr=0xcb0026au;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb0026a;
Lcb0026a: r[3]=rd(m,0x0cb00364u,4); /* mov.l 0x0cb00364,r3 */
 goto Lcb0026c;
Lcb0026c: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb0026e;
Lcb0026e: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00270;
Lcb00270: target=r[3];*pr=0xcb00274u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00274;
Lcb00274: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00276;
Lcb00276: if(!(*sr&1u))goto Lcb002b6; /* bf 0x0cb002b6 */
 goto Lcb00278;
Lcb00278: r[3]=rd(m,0x0cb00368u,4); /* mov.l 0x0cb00368,r3 */
 goto Lcb0027a;
Lcb0027a: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb0027c;
Lcb0027c: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb0027e;
Lcb0027e: target=r[3];*pr=0xcb00282u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00282;
Lcb00282: goto Lcb002b6; /* bra 0x0cb002b6 */
 goto Lcb00286;
Lcb00286: r[3]=rd(m,0x0cb0036cu,4); /* mov.l 0x0cb0036c,r3 */
 goto Lcb00288;
Lcb00288: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb0028a;
Lcb0028a: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb0028c;
Lcb0028c: target=r[3];*pr=0xcb00290u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00290;
Lcb00290: goto Lcb002b6; /* bra 0x0cb002b6 */
 goto Lcb00294;
Lcb00294: goto Lcb002da; /* bra 0x0cb002da */
 goto Lcb00298;
Lcb00298: r[2]=rd(m,0x0cb00370u,4); /* mov.l 0x0cb00370,r2 */
 goto Lcb0029a;
Lcb0029a: target=r[2];*pr=0xcb0029eu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb0029e;
Lcb0029e: goto Lcb002da; /* bra 0x0cb002da */
 goto Lcb002a2;
Lcb002a2: r[2]=rd(m,0x0cb00360u,4); /* mov.l 0x0cb00360,r2 */
 goto Lcb002a4;
Lcb002a4: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb002a6;
Lcb002a6: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb002a8;
Lcb002a8: target=r[2];*pr=0xcb002acu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb002ac;
Lcb002ac: r[3]=rd(m,0x0cb00374u,4); /* mov.l 0x0cb00374,r3 */
 goto Lcb002ae;
Lcb002ae: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb002b0;
Lcb002b0: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb002b2;
Lcb002b2: target=r[3];*pr=0xcb002b6u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb002b6;
Lcb002b6: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb002b8;
Lcb002b8: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb002ba;
Lcb002ba: target=r[10];*pr=0xcb002beu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r10 */
 goto Lcb002be;
Lcb002be: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb002c0;
Lcb002c0: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb002c2;
Lcb002c2: target=r[8];*pr=0xcb002c6u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r8 */
 goto Lcb002c6;
Lcb002c6: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb002c8;
Lcb002c8: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb002ca;
Lcb002ca: target=r[9];*pr=0xcb002ceu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto Lcb002ce;
Lcb002ce: goto Lcb002da; /* bra 0x0cb002da */
 goto Lcb002d2;
Lcb002d2: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb002d4;
Lcb002d4: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb002d6;
Lcb002d6: *pr=0xcb002dau;r[4]=r[14];status=call(ctx,0xcb00a40u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb00a40 */
 goto Lcb002da;
Lcb002da: r[2]=rd(m,0x0cb00378u,4); /* mov.l 0x0cb00378,r2 */
 goto Lcb002dc;
Lcb002dc: r[4]=72u; /* mov #0x48,r4 */
 goto Lcb002de;
Lcb002de: r[4]<<=2; /* shll2 r4 */
 goto Lcb002e0;
Lcb002e0: target=r[2];*pr=0xcb002e4u;r[4]+=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb002e4;
Lcb002e4: r[3]=rd(m,0x0cb0037cu,4); /* mov.l 0x0cb0037c,r3 */
 goto Lcb002e6;
Lcb002e6: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb002e8;
Lcb002e8: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb002ea;
Lcb002ea: target=r[3];*pr=0xcb002eeu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb002ee;
Lcb002ee: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb002f0;
Lcb002f0: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb002f2;
Lcb002f2: *pr=0xcb002f6u;r[4]=r[14];status=call(ctx,0xcb007a0u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb007a0 */
 goto Lcb002f6;
Lcb002f6: *pr=0xcb002fau;r[4]=r[11];status=call(ctx,0xcb008a0u,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x0cb008a0 */
 goto Lcb002fa;
Lcb002fa: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto Lcb002fc;
Lcb002fc: r[3]=52u; /* mov #0x34,r3 */
 goto Lcb002fe;
Lcb002fe: r[1]=rd(m,0x0cb00380u,4); /* mov.l 0x0cb00380,r1 */
 goto Lcb00300;
Lcb00300: r[2]=4u; /* mov #0x4,r2 */
 goto Lcb00302;
Lcb00302: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto Lcb00304;
Lcb00304: *macl=(r[3]&65535u)*(r[0]&65535u); /* mulu.w r3,r0 */
 goto Lcb00306;
Lcb00306: r[2]<<=8; /* shll8 r2 */
 goto Lcb00308;
Lcb00308: r[0]=*macl; /* sts MACL,r0 */
 goto Lcb0030a;
Lcb0030a: r[1]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r1 */
 goto Lcb0030c;
Lcb0030c: *sr=(*sr&~1u)|!(r[2]&r[1]); /* tst r2,r1 */
 goto Lcb0030e;
Lcb0030e: if((*sr&1u))goto Lcb00326; /* bt 0x0cb00326 */
 goto Lcb00310;
Lcb00310: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto Lcb00312;
Lcb00312: *sr=(*sr&~1u)|(r[0]==0xbu); /* cmp/eq #0xb,r0 */
 goto Lcb00314;
Lcb00314: if((*sr&1u))goto Lcb00326; /* bt 0x0cb00326 */
 goto Lcb00316;
Lcb00316: r[3]=4294967248u; /* mov #-0x30,r3 */
 goto Lcb00318;
Lcb00318: r[3]<<=8; /* shll8 r3 */
 goto Lcb0031a;
Lcb0031a: r[3]+=4294967295u; /* add #-0x1,r3 */
 goto Lcb0031c;
Lcb0031c: r[2]=11u; /* mov #0xb,r2 */
 goto Lcb0031e;
Lcb0031e: wr(m,r[14],r[2],1); /* mov.b r2,@r14 */
 goto Lcb00320;
Lcb00320: r[0]=sx(m,r[14]+0x4u,2); /* mov.w @(0x4,r14),r0 */
 goto Lcb00322;
Lcb00322: r[0]&=r[3]; /* and r3,r0 */
 goto Lcb00324;
Lcb00324: wr(m,r[14]+0x4u,r[0],2); /* mov.w r0,@(0x4,r14) */
 goto Lcb00326;
Lcb00326: r[5]=119u; /* mov #0x77,r5 */
 goto Lcb00328;
Lcb00328: r[3]=rd(m,0x0cb00384u,4); /* mov.l 0x0cb00384,r3 */
 goto Lcb0032a;
Lcb0032a: r[5]<<=2; /* shll2 r5 */
 goto Lcb0032c;
Lcb0032c: r[5]+=r[13]; /* add r13,r5 */
 goto Lcb0032e;
Lcb0032e: target=r[3];*pr=0xcb00332u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00332;
Lcb00332: r[2]=rd(m,0x0cb00388u,4); /* mov.l 0x0cb00388,r2 */
 goto Lcb00334;
Lcb00334: target=r[2];*pr=0xcb00338u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb00338;
Lcb00338: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto Lcb0033a;
Lcb0033a: r[5]=r[12]; /* mov r12,r5 */
 goto Lcb0033c;
Lcb0033c: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb0033e;
Lcb0033e: r[3]=rd(m,0x0cb0038cu,4); /* mov.l 0x0cb0038c,r3 */
 goto Lcb00340;
Lcb00340: r[4]=r[14]; /* mov r14,r4 */
 goto Lcb00342;
Lcb00342: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto Lcb00344;
Lcb00344: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto Lcb00346;
Lcb00346: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto Lcb00348;
Lcb00348: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto Lcb0034a;
Lcb0034a: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto Lcb0034c;
Lcb0034c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto Lcb0034e;
Lcb0034e: target=r[3];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
}
