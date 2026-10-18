/* Sonic Adventure USA Rev A recovered frame request/layout setup. */
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
/* Static C recovery of retail Tikal helper 8c667240..8c6672de.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_frame_device_layout_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c667240;
L8c667240: r[5]=0u; /* mov #0x0,r5 */
 goto L8c667242;
L8c667242: r[4]=rd(m,0x8c667358u,4); /* mov.l 0x8c667358,r4 */
 goto L8c667244;
L8c667244: r[7]=sx(m,0x8c667354u,2); /* mov.w 0x8c667354,r7 */
 goto L8c667246;
L8c667246: r[6]=r[5]; /* mov r5,r6 */
 goto L8c667248;
L8c667248: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66724a;
L8c66724a: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c66724c;
L8c66724c: r[0]=r[5]; /* mov r5,r0 */
 goto L8c66724e;
L8c66724e: r[2]+=r[6]; /* add r6,r2 */
 goto L8c667250;
L8c667250: wr(m,r[2]+0x1u,r[0],1); /* mov.b r0,@(0x1,r2) */
 goto L8c667252;
L8c667252: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c667254;
L8c667254: r[2]+=r[6]; /* add r6,r2 */
 goto L8c667256;
L8c667256: r[6]+=8u; /* add #0x8,r6 */
 goto L8c667258;
L8c667258: *sr=(*sr&~1u)|(r[6]>=r[7]); /* cmp/hs r7,r6 */
 goto L8c66725a;
L8c66725a: take=!(*sr&1u);wr(m,r[2]+0x4u,r[0],4);if(take)goto L8c66724a;goto L8c66725e; /* bf/s 0x8c66724a */
 goto L8c66725e;
L8c66725e: r[7]=r[5]; /* mov r5,r7 */
 goto L8c667260;
L8c667260: r[6]=8u; /* mov #0x8,r6 */
 goto L8c667262;
L8c667262: r[1]=1u; /* mov #0x1,r1 */
 goto L8c667264;
L8c667264: r[13]=r[7]; /* mov r7,r13 */
 goto L8c667266;
L8c667266: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c667268;
L8c667268: r[13]<<=2; /* shll2 r13 */
 goto L8c66726a;
L8c66726a: r[0]=r[7]; /* mov r7,r0 */
 goto L8c66726c;
L8c66726c: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1; /* shll r13 */
 goto L8c66726e;
L8c66726e: r[3]+=8u; /* add #0x8,r3 */
 goto L8c667270;
L8c667270: r[7]+=1u; /* add #0x1,r7 */
 goto L8c667272;
L8c667272: r[3]+=r[13]; /* add r13,r3 */
 goto L8c667274;
L8c667274: *sr=(*sr&~1u)|(r[7]>=r[6]); /* cmp/hs r6,r7 */
 goto L8c667276;
L8c667276: wr(m,r[3]+0x1u,r[0],1); /* mov.b r0,@(0x1,r3) */
 goto L8c667278;
L8c667278: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c66727a;
L8c66727a: r[2]+=8u; /* add #0x8,r2 */
 goto L8c66727c;
L8c66727c: r[13]+=r[2]; /* add r2,r13 */
 goto L8c66727e;
L8c66727e: take=!(*sr&1u);wr(m,r[13],r[1],1);if(take)goto L8c667264;goto L8c667282; /* bf/s 0x8c667264 */
 goto L8c667282;
L8c667282: r[7]=r[5]; /* mov r5,r7 */
 goto L8c667284;
L8c667284: r[13]=2u; /* mov #0x2,r13 */
 goto L8c667286;
L8c667286: r[1]=r[7]; /* mov r7,r1 */
 goto L8c667288;
L8c667288: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c66728a;
L8c66728a: r[1]<<=2; /* shll2 r1 */
 goto L8c66728c;
L8c66728c: r[0]=r[7]; /* mov r7,r0 */
 goto L8c66728e;
L8c66728e: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c667290;
L8c667290: r[3]+=72u; /* add #0x48,r3 */
 goto L8c667292;
L8c667292: r[7]+=1u; /* add #0x1,r7 */
 goto L8c667294;
L8c667294: r[3]+=r[1]; /* add r1,r3 */
 goto L8c667296;
L8c667296: *sr=(*sr&~1u)|(r[7]>=r[6]); /* cmp/hs r6,r7 */
 goto L8c667298;
L8c667298: wr(m,r[3]+0x1u,r[0],1); /* mov.b r0,@(0x1,r3) */
 goto L8c66729a;
L8c66729a: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c66729c;
L8c66729c: r[2]+=72u; /* add #0x48,r2 */
 goto L8c66729e;
L8c66729e: r[1]+=r[2]; /* add r2,r1 */
 goto L8c6672a0;
L8c6672a0: take=!(*sr&1u);wr(m,r[1],r[13],1);if(take)goto L8c667286;goto L8c6672a4; /* bf/s 0x8c667286 */
 goto L8c6672a4;
L8c6672a4: r[6]=r[5]; /* mov r5,r6 */
 goto L8c6672a6;
L8c6672a6: r[1]=3u; /* mov #0x3,r1 */
 goto L8c6672a8;
L8c6672a8: r[13]=4u; /* mov #0x4,r13 */
 goto L8c6672aa;
L8c6672aa: r[3]=sx(m,0x8c667356u,2); /* mov.w 0x8c667356,r3 */
 goto L8c6672ac;
L8c6672ac: r[7]=r[6]; /* mov r6,r7 */
 goto L8c6672ae;
L8c6672ae: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c6672b0;
L8c6672b0: r[7]<<=2; /* shll2 r7 */
 goto L8c6672b2;
L8c6672b2: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1; /* shll r7 */
 goto L8c6672b4;
L8c6672b4: r[0]=r[6]; /* mov r6,r0 */
 goto L8c6672b6;
L8c6672b6: r[3]+=r[2]; /* add r2,r3 */
 goto L8c6672b8;
L8c6672b8: r[2]=sx(m,0x8c667356u,2); /* mov.w 0x8c667356,r2 */
 goto L8c6672ba;
L8c6672ba: r[3]+=r[7]; /* add r7,r3 */
 goto L8c6672bc;
L8c6672bc: wr(m,r[3]+0x1u,r[0],1); /* mov.b r0,@(0x1,r3) */
 goto L8c6672be;
L8c6672be: r[6]+=1u; /* add #0x1,r6 */
 goto L8c6672c0;
L8c6672c0: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c6672c2;
L8c6672c2: *sr=(*sr&~1u)|(r[6]>=r[13]); /* cmp/hs r13,r6 */
 goto L8c6672c4;
L8c6672c4: r[2]+=r[3]; /* add r3,r2 */
 goto L8c6672c6;
L8c6672c6: r[7]+=r[2]; /* add r2,r7 */
 goto L8c6672c8;
L8c6672c8: take=!(*sr&1u);wr(m,r[7],r[1],1);if(take)goto L8c6672aa;goto L8c6672cc; /* bf/s 0x8c6672aa */
 goto L8c6672cc;
L8c6672cc: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c6672ce;
L8c6672ce: r[0]=r[5]; /* mov r5,r0 */
 goto L8c6672d0;
L8c6672d0: wr(m,r[2]+0x1u,r[0],1); /* mov.b r0,@(0x1,r2) */
 goto L8c6672d2;
L8c6672d2: r[2]=9u; /* mov #0x9,r2 */
 goto L8c6672d4;
L8c6672d4: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c6672d6;
L8c6672d6: r[0]=0u; /* mov #0x0,r0 */
 goto L8c6672d8;
L8c6672d8: wr(m,r[3],r[2],1); /* mov.b r2,@r3 */
 goto L8c6672da;
L8c6672da: r[13]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
