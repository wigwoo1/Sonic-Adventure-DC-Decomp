#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v,unsigned n){for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|((a==b)||(!(a&0x7fffffffu)&&!(b&0x7fffffffu)));return 0;
}
/* Static C recovery of retail Tikal helper 8c666706..8c666968.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_render_layout_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c666706;
L8c666706: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c666708;
L8c666708: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66670a;
L8c66670a: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66670c;
L8c66670c: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66670e;
L8c66670e: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c666710;
L8c666710: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c666712;
L8c666712: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c666714;
L8c666714: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c666716;
L8c666716: r[3]=rd(m,0x8c66689cu,4); /* mov.l 0x8c66689c,r3 */
 goto L8c666718;
L8c666718: r[4]=rd(m,r[15]+0x20u,4); /* mov.l @(0x20,r15),r4 */
 goto L8c66671a;
L8c66671a: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c66671c;
L8c66671c: r[2]=r[4]; /* mov r4,r2 */
 goto L8c66671e;
L8c66671e: r[5]=1u; /* mov #0x1,r5 */
 goto L8c666720;
L8c666720: *sr=(*sr&~1u)|!(r[5]&r[2]); /* tst r5,r2 */
 goto L8c666722;
L8c666722: take=(*sr&1u);r[11]=0u;if(take)goto L8c66672a;goto L8c666726; /* bt/s 0x8c66672a */
 goto L8c666726;
L8c666726: r[9]=r[11];goto L8c66672c; /* bra 0x8c66672c */
 goto L8c66672a;
L8c66672a: r[9]=r[5]; /* mov r5,r9 */
 goto L8c66672c;
L8c66672c: r[3]=rd(m,0x8c6668a0u,4); /* mov.l 0x8c6668a0,r3 */
 goto L8c66672e;
L8c66672e: r[1]=rd(m,0x8c666898u,4); /* mov.l 0x8c666898,r1 */
 goto L8c666730;
L8c666730: r[3]&=r[4]; /* and r4,r3 */
 goto L8c666732;
L8c666732: r[3]>>=16; /* shlr16 r3 */
 goto L8c666734;
L8c666734: wr(m,r[1],r[3],4); /* mov.l r3,@r1 */
 goto L8c666736;
L8c666736: r[2]=rd(m,0x8c6668a4u,4); /* mov.l 0x8c6668a4,r2 */
 goto L8c666738;
L8c666738: r[0]=rd(m,0x8c6668a8u,4); /* mov.l 0x8c6668a8,r0 */
 goto L8c66673a;
L8c66673a: r[2]&=r[4]; /* and r4,r2 */
 goto L8c66673c;
L8c66673c: r[2]>>=16; /* shlr16 r2 */
 goto L8c66673e;
L8c66673e: *sr=(*sr&~1u)|(r[2]&1u);r[2]>>=1; /* shlr r2 */
 goto L8c666740;
L8c666740: wr(m,r[0],r[2],4); /* mov.l r2,@r0 */
 goto L8c666742;
L8c666742: r[3]=rd(m,0x8c6668acu,4); /* mov.l 0x8c6668ac,r3 */
 goto L8c666744;
L8c666744: r[1]+=8u; /* add #0x8,r1 */
 goto L8c666746;
L8c666746: r[3]&=r[4]; /* and r4,r3 */
 goto L8c666748;
L8c666748: r[3]>>=16; /* shlr16 r3 */
 goto L8c66674a;
L8c66674a: r[3]>>=2; /* shlr2 r3 */
 goto L8c66674c;
L8c66674c: *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1; /* shlr r3 */
 goto L8c66674e;
L8c66674e: wr(m,r[1],r[3],4); /* mov.l r3,@r1 */
 goto L8c666750;
L8c666750: r[2]=rd(m,0x8c666890u,4); /* mov.l 0x8c666890,r2 */
 goto L8c666752;
L8c666752: r[3]=4294967275u; /* mov #-0x15,r3 */
 goto L8c666754;
L8c666754: r[0]+=8u; /* add #0x8,r0 */
 goto L8c666756;
L8c666756: r[2]&=r[4]; /* and r4,r2 */
 goto L8c666758;
L8c666758: r[2]=(r[3]&0x80000000u)?((r[3]&31u)?r[2]>>((0u-r[3])&31u):0u):(r[2]<<(r[3]&31u)); /* shld r3,r2 */
 goto L8c66675a;
L8c66675a: wr(m,r[0],r[2],4); /* mov.l r2,@r0 */
 goto L8c66675c;
L8c66675c: r[3]=rd(m,0x8c6668b0u,4); /* mov.l 0x8c6668b0,r3 */
 goto L8c66675e;
L8c66675e: r[2]=4294967273u; /* mov #-0x17,r2 */
 goto L8c666760;
L8c666760: r[1]+=8u; /* add #0x8,r1 */
 goto L8c666762;
L8c666762: r[3]&=r[4]; /* and r4,r3 */
 goto L8c666764;
L8c666764: r[3]=(r[2]&0x80000000u)?((r[2]&31u)?r[3]>>((0u-r[2])&31u):0u):(r[3]<<(r[2]&31u)); /* shld r2,r3 */
 goto L8c666766;
L8c666766: wr(m,r[1],r[3],4); /* mov.l r3,@r1 */
 goto L8c666768;
L8c666768: r[2]=rd(m,0x8c6668b4u,4); /* mov.l 0x8c6668b4,r2 */
 goto L8c66676a;
L8c66676a: r[0]+=8u; /* add #0x8,r0 */
 goto L8c66676c;
L8c66676c: r[4]&=r[2]; /* and r2,r4 */
 goto L8c66676e;
L8c66676e: r[4]>>=16; /* shlr16 r4 */
 goto L8c666770;
L8c666770: r[4]>>=8; /* shlr8 r4 */
 goto L8c666772;
L8c666772: *sr=(*sr&~1u)|(r[4]&1u);r[4]>>=1; /* shlr r4 */
 goto L8c666774;
L8c666774: wr(m,r[0],r[4],4); /* mov.l r4,@r0 */
 goto L8c666776;
L8c666776: r[1]+=4294967196u; /* add #-0x64,r1 */
 goto L8c666778;
L8c666778: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c66677a;
L8c66677a: r[3]>>=2; /* shlr2 r3 */
 goto L8c66677c;
L8c66677c: r[2]=r[3]; /* mov r3,r2 */
 goto L8c66677e;
L8c66677e: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c666780;
L8c666780: r[1]+=4294967284u; /* add #-0xc,r1 */
 goto L8c666782;
L8c666782: r[3]+=r[2]; /* add r2,r3 */
 goto L8c666784;
L8c666784: wr(m,r[1],r[3],4); /* mov.l r3,@r1 */
 goto L8c666786;
L8c666786: r[2]=rd(m,0x8c6668b8u,4); /* mov.l 0x8c6668b8,r2 */
 goto L8c666788;
L8c666788: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c66678a;
L8c66678a: r[0]+=4294967196u; /* add #-0x64,r0 */
 goto L8c66678c;
L8c66678c: r[2]=rd(m,r[0],4); /* mov.l @r0,r2 */
 goto L8c66678e;
L8c66678e: *macl=r[3]*r[2]; /* mul.l r3,r2 */
 goto L8c666790;
L8c666790: *pr=0x8c666794u;r[4]=*macl;status=call(ctx,0x8c665e00u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c665e00 */
 goto L8c666794;
L8c666794: r[8]=2u; /* mov #0x2,r8 */
 goto L8c666796;
L8c666796: r[4]=r[11]; /* mov r11,r4 */
 goto L8c666798;
L8c666798: r[7]=4294967168u; /* mov #-0x80,r7 */
 goto L8c66679a;
L8c66679a: r[10]=r[11]; /* mov r11,r10 */
 goto L8c66679c;
L8c66679c: r[13]=4294967264u; /* mov #-0x20,r13 */
 goto L8c66679e;
L8c66679e: r[12]=r[0]; /* mov r0,r12 */
 goto L8c6667a0;
L8c6667a0: r[5]=r[11]; /* mov r11,r5 */
 goto L8c6667a2;
L8c6667a2: r[6]=r[11]; /* mov r11,r6 */
 goto L8c6667a4;
L8c6667a4: r[14]=r[10]; /* mov r10,r14 */
 goto L8c6667a6;
L8c6667a6: r[14]<<=2; /* shll2 r14 */
 goto L8c6667a8;
L8c6667a8: r[1]=rd(m,0x8c6668bcu,4); /* mov.l 0x8c6668bc,r1 */
 goto L8c6667aa;
L8c6667aa: r[14]<<=2; /* shll2 r14 */
 goto L8c6667ac;
L8c6667ac: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1; /* shll r14 */
 goto L8c6667ae;
L8c6667ae: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c6667b0;
L8c6667b0: r[0]=rd(m,0x8c6668c0u,4); /* mov.l 0x8c6668c0,r0 */
 goto L8c6667b2;
L8c6667b2: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c6667b4;
L8c6667b4: r[0]+=4u; /* add #0x4,r0 */
 goto L8c6667b6;
L8c6667b6: r[2]+=r[3]; /* add r3,r2 */
 goto L8c6667b8;
L8c6667b8: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c6667ba;
L8c6667ba: r[3]=rd(m,0x8c6668c4u,4); /* mov.l 0x8c6668c4,r3 */
 goto L8c6667bc;
L8c6667bc: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c6667be;
L8c6667be: *sr=(*sr&~1u)|!(r[0]&0x20u); /* tst #0x20,r0 */
 goto L8c6667c0;
L8c6667c0: if((*sr&1u))goto L8c666814; /* bt 0x8c666814 */
 goto L8c6667c2;
L8c6667c2: r[0]=rd(m,0x8c6668c8u,4); /* mov.l 0x8c6668c8,r0 */
 goto L8c6667c4;
L8c6667c4: r[2]=rd(m,r[0],4); /* mov.l @r0,r2 */
 goto L8c6667c6;
L8c6667c6: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c6667c8;
L8c6667c8: if((*sr&1u))goto L8c6667f2; /* bt 0x8c6667f2 */
 goto L8c6667ca;
L8c6667ca: r[0]=rd(m,0x8c6668ccu,4); /* mov.l 0x8c6668cc,r0 */
 goto L8c6667cc;
L8c6667cc: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c6667ce;
L8c6667ce: r[2]+=4294967228u; /* add #-0x44,r2 */
 goto L8c6667d0;
L8c6667d0: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c6667d2;
L8c6667d2: r[1]=rd(m,0x8c6668b8u,4); /* mov.l 0x8c6668b8,r1 */
 goto L8c6667d4;
L8c6667d4: r[2]=rd(m,r[1],4); /* mov.l @r1,r2 */
 goto L8c6667d6;
L8c6667d6: r[3]=rd(m,0x8c6668d0u,4); /* mov.l 0x8c6668d0,r3 */
 goto L8c6667d8;
L8c6667d8: r[1]=rd(m,r[3],4); /* mov.l @r3,r1 */
 goto L8c6667da;
L8c6667da: *macl=r[2]*r[1]; /* mul.l r2,r1 */
 goto L8c6667dc;
L8c6667dc: r[2]=*macl; /* sts MACL,r2 */
 goto L8c6667de;
L8c6667de: r[1]=r[2]; /* mov r2,r1 */
 goto L8c6667e0;
L8c6667e0: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c6667e2;
L8c6667e2: r[2]+=r[1]; /* add r1,r2 */
 goto L8c6667e4;
L8c6667e4: r[2]<<=2; /* shll2 r2 */
 goto L8c6667e6;
L8c6667e6: r[2]<<=2; /* shll2 r2 */
 goto L8c6667e8;
L8c6667e8: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c6667ea;
L8c6667ea: r[1]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r1 */
 goto L8c6667ec;
L8c6667ec: r[1]-=r[2]; /* sub r2,r1 */
 goto L8c6667ee;
L8c6667ee: goto L8c666810; /* bra 0x8c666810 */
 goto L8c6667f2;
L8c6667f2: r[0]=rd(m,0x8c6668ccu,4); /* mov.l 0x8c6668cc,r0 */
 goto L8c6667f4;
L8c6667f4: r[1]=r[0]; /* mov r0,r1 */
 goto L8c6667f6;
L8c6667f6: r[1]+=4294967280u; /* add #-0x10,r1 */
 goto L8c6667f8;
L8c6667f8: r[2]=rd(m,r[1],4); /* mov.l @r1,r2 */
 goto L8c6667fa;
L8c6667fa: r[3]=rd(m,0x8c6668d0u,4); /* mov.l 0x8c6668d0,r3 */
 goto L8c6667fc;
L8c6667fc: r[1]=rd(m,r[3],4); /* mov.l @r3,r1 */
 goto L8c6667fe;
L8c6667fe: *macl=r[2]*r[1]; /* mul.l r2,r1 */
 goto L8c666800;
L8c666800: r[2]=*macl; /* sts MACL,r2 */
 goto L8c666802;
L8c666802: r[1]=r[2]; /* mov r2,r1 */
 goto L8c666804;
L8c666804: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c666806;
L8c666806: r[2]+=r[1]; /* add r1,r2 */
 goto L8c666808;
L8c666808: r[2]<<=2; /* shll2 r2 */
 goto L8c66680a;
L8c66680a: r[2]<<=2; /* shll2 r2 */
 goto L8c66680c;
L8c66680c: r[1]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r1 */
 goto L8c66680e;
L8c66680e: r[1]-=r[2]; /* sub r2,r1 */
 goto L8c666810;
L8c666810: wr(m,r[14]+r[0],r[1],4);goto L8c666834; /* bra 0x8c666834 */
 goto L8c666814;
L8c666814: r[0]=rd(m,0x8c6668ccu,4); /* mov.l 0x8c6668cc,r0 */
 goto L8c666816;
L8c666816: r[1]=r[0]; /* mov r0,r1 */
 goto L8c666818;
L8c666818: r[1]+=4294967280u; /* add #-0x10,r1 */
 goto L8c66681a;
L8c66681a: r[2]=rd(m,r[1],4); /* mov.l @r1,r2 */
 goto L8c66681c;
L8c66681c: r[3]=rd(m,0x8c6668d0u,4); /* mov.l 0x8c6668d0,r3 */
 goto L8c66681e;
L8c66681e: r[1]=rd(m,r[3],4); /* mov.l @r3,r1 */
 goto L8c666820;
L8c666820: *macl=r[2]*r[1]; /* mul.l r2,r1 */
 goto L8c666822;
L8c666822: r[2]=*macl; /* sts MACL,r2 */
 goto L8c666824;
L8c666824: r[1]=r[2]; /* mov r2,r1 */
 goto L8c666826;
L8c666826: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c666828;
L8c666828: r[2]+=r[1]; /* add r1,r2 */
 goto L8c66682a;
L8c66682a: r[2]<<=2; /* shll2 r2 */
 goto L8c66682c;
L8c66682c: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c66682e;
L8c66682e: r[1]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r1 */
 goto L8c666830;
L8c666830: r[1]-=r[2]; /* sub r2,r1 */
 goto L8c666832;
L8c666832: wr(m,r[14]+r[0],r[1],4); /* mov.l r1,@(r0,r14) */
 goto L8c666834;
L8c666834: r[2]=rd(m,0x8c666898u,4); /* mov.l 0x8c666898,r2 */
 goto L8c666836;
L8c666836: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c666838;
L8c666838: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c66683a;
L8c66683a: if(!(*sr&1u))goto L8c6668e0; /* bf 0x8c6668e0 */
 goto L8c66683c;
L8c66683c: r[1]=rd(m,0x8c6668d4u,4); /* mov.l 0x8c6668d4,r1 */
 goto L8c66683e;
L8c66683e: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c666840;
L8c666840: r[0]=rd(m,0x8c6668c0u,4); /* mov.l 0x8c6668c0,r0 */
 goto L8c666842;
L8c666842: r[0]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r0 */
 goto L8c666844;
L8c666844: r[2]=rd(m,0x8c6668d8u,4); /* mov.l 0x8c6668d8,r2 */
 goto L8c666846;
L8c666846: r[0]+=r[3]; /* add r3,r0 */
 goto L8c666848;
L8c666848: r[0]&=r[13]; /* and r13,r0 */
 goto L8c66684a;
L8c66684a: r[2]+=r[4]; /* add r4,r2 */
 goto L8c66684c;
L8c66684c: wr(m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c66684e;
L8c66684e: r[0]=rd(m,0x8c6668ccu,4); /* mov.l 0x8c6668cc,r0 */
 goto L8c666850;
L8c666850: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c666852;
L8c666852: r[3]-=r[12]; /* sub r12,r3 */
 goto L8c666854;
L8c666854: r[0]+=12u; /* add #0xc,r0 */
 goto L8c666856;
L8c666856: r[3]&=r[7]; /* and r7,r3 */
 goto L8c666858;
L8c666858: wr(m,r[4]+r[0],r[3],4); /* mov.l r3,@(r0,r4) */
 goto L8c66685a;
L8c66685a: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c66685c;
L8c66685c: r[2]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r2 */
 goto L8c66685e;
L8c66685e: r[0]+=8u; /* add #0x8,r0 */
 goto L8c666860;
L8c666860: r[2]+=r[12]; /* add r12,r2 */
 goto L8c666862;
L8c666862: wr(m,r[4]+r[0],r[2],4);goto L8c666904; /* bra 0x8c666904 */
 goto L8c6668e0;
L8c6668e0: r[0]=rd(m,0x8c6669a4u,4); /* mov.l 0x8c6669a4,r0 */
 goto L8c6668e2;
L8c6668e2: r[1]=rd(m,r[5]+r[0],4); /* mov.l @(r0,r5),r1 */
 goto L8c6668e4;
L8c6668e4: r[1]-=r[12]; /* sub r12,r1 */
 goto L8c6668e6;
L8c6668e6: r[0]+=8u; /* add #0x8,r0 */
 goto L8c6668e8;
L8c6668e8: r[1]&=r[7]; /* and r7,r1 */
 goto L8c6668ea;
L8c6668ea: wr(m,r[5]+r[0],r[1],4); /* mov.l r1,@(r0,r5) */
 goto L8c6668ec;
L8c6668ec: r[1]=rd(m,0x8c6669a8u,4); /* mov.l 0x8c6669a8,r1 */
 goto L8c6668ee;
L8c6668ee: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c6668f0;
L8c6668f0: r[0]+=4294967284u; /* add #-0xc,r0 */
 goto L8c6668f2;
L8c6668f2: r[1]=rd(m,r[5]+r[0],4); /* mov.l @(r0,r5),r1 */
 goto L8c6668f4;
L8c6668f4: r[1]+=r[3]; /* add r3,r1 */
 goto L8c6668f6;
L8c6668f6: r[0]+=16u; /* add #0x10,r0 */
 goto L8c6668f8;
L8c6668f8: r[1]&=r[13]; /* and r13,r1 */
 goto L8c6668fa;
L8c6668fa: wr(m,r[5]+r[0],r[1],4); /* mov.l r1,@(r0,r5) */
 goto L8c6668fc;
L8c6668fc: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c6668fe;
L8c6668fe: r[3]=rd(m,r[5]+r[0],4); /* mov.l @(r0,r5),r3 */
 goto L8c666900;
L8c666900: r[0]+=8u; /* add #0x8,r0 */
 goto L8c666902;
L8c666902: wr(m,r[5]+r[0],r[3],4); /* mov.l r3,@(r0,r5) */
 goto L8c666904;
L8c666904: r[0]=rd(m,0x8c6669acu,4); /* mov.l 0x8c6669ac,r0 */
 goto L8c666906;
L8c666906: r[2]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r2 */
 goto L8c666908;
L8c666908: r[0]+=24u; /* add #0x18,r0 */
 goto L8c66690a;
L8c66690a: wr(m,r[6]+r[0],r[2],4); /* mov.l r2,@(r0,r6) */
 goto L8c66690c;
L8c66690c: r[2]=rd(m,0x8c6669a8u,4); /* mov.l 0x8c6669a8,r2 */
 goto L8c66690e;
L8c66690e: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c666910;
L8c666910: r[0]+=4294967272u; /* add #-0x18,r0 */
 goto L8c666912;
L8c666912: r[1]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r1 */
 goto L8c666914;
L8c666914: r[1]+=r[3]; /* add r3,r1 */
 goto L8c666916;
L8c666916: r[3]=sx(m,0x8c6669a0u,2); /* mov.w 0x8c6669a0,r3 */
 goto L8c666918;
L8c666918: r[0]+=28u; /* add #0x1c,r0 */
 goto L8c66691a;
L8c66691a: r[1]+=r[3]; /* add r3,r1 */
 goto L8c66691c;
L8c66691c: wr(m,r[6]+r[0],r[1],4); /* mov.l r1,@(r0,r6) */
 goto L8c66691e;
L8c66691e: r[10]+=1u; /* add #0x1,r10 */
 goto L8c666920;
L8c666920: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[8]); /* cmp/ge r8,r10 */
 goto L8c666922;
L8c666922: r[5]+=32u; /* add #0x20,r5 */
 goto L8c666924;
L8c666924: r[6]+=32u; /* add #0x20,r6 */
 goto L8c666926;
L8c666926: take=(*sr&1u);r[4]+=32u;if(take)goto L8c66692e;goto L8c66692a; /* bt/s 0x8c66692e */
 goto L8c66692a;
L8c66692a: goto L8c6667a4; /* bra 0x8c6667a4 */
 goto L8c66692e;
L8c66692e: r[3]=rd(m,0x8c6669b0u,4); /* mov.l 0x8c6669b0,r3 */
 goto L8c666930;
L8c666930: r[6]=0u; /* mov #0x0,r6 */
 goto L8c666932;
L8c666932: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c666934;
L8c666934: r[7]=1u; /* mov #0x1,r7 */
 goto L8c666936;
L8c666936: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c666938;
L8c666938: r[5]=rd(m,r[3],4); /* mov.l @r3,r5 */
 goto L8c66693a;
L8c66693a: r[2]=rd(m,0x8c6669b4u,4); /* mov.l 0x8c6669b4,r2 */
 goto L8c66693c;
L8c66693c: *pr=0x8c666940u;r[4]=rd(m,r[2],4);status=call(ctx,0x8c665fa8u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c665fa8 */
 goto L8c666940;
L8c666940: r[2]=rd(m,0x8c6669b0u,4); /* mov.l 0x8c6669b0,r2 */
 goto L8c666942;
L8c666942: r[3]=1u; /* mov #0x1,r3 */
 goto L8c666944;
L8c666944: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c666946;
L8c666946: r[6]=0u; /* mov #0x0,r6 */
 goto L8c666948;
L8c666948: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c66694a;
L8c66694a: r[7]=r[3]; /* mov r3,r7 */
 goto L8c66694c;
L8c66694c: r[5]=rd(m,r[2],4); /* mov.l @r2,r5 */
 goto L8c66694e;
L8c66694e: r[1]=rd(m,0x8c6669b4u,4); /* mov.l 0x8c6669b4,r1 */
 goto L8c666950;
L8c666950: *pr=0x8c666954u;r[4]=rd(m,r[1],4);status=call(ctx,0x8c665fa8u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c665fa8 */
 goto L8c666954;
L8c666954: r[15]+=16u; /* add #0x10,r15 */
 goto L8c666956;
L8c666956: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c666958;
L8c666958: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c66695a;
L8c66695a: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c66695c;
L8c66695c: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c66695e;
L8c66695e: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c666960;
L8c666960: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c666962;
L8c666962: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c666964;
L8c666964: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
