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
/* Static C recovery of retail Tikal helper 8c65a27c..8c65a2e6.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_start(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a27c;
L8c65a27c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a27e;
L8c65a27e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65a280;
L8c65a280: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c65a282;
L8c65a282: r[0]=*sr; /* stc SR,r0 */
 goto L8c65a284;
L8c65a284: r[3]=sx(m,0x8c65a2e6u,2); /* mov.w 0x8c65a2e6,r3 */
 goto L8c65a286;
L8c65a286: r[0]>>=2; /* shlr2 r0 */
 goto L8c65a288;
L8c65a288: r[0]>>=2; /* shlr2 r0 */
 goto L8c65a28a;
L8c65a28a: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c65a28c;
L8c65a28c: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c65a28e;
L8c65a28e: r[0]=*sr; /* stc SR,r0 */
 goto L8c65a290;
L8c65a290: r[0]&=r[3]; /* and r3,r0 */
 goto L8c65a292;
L8c65a292: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c65a294;
L8c65a294: *sr=r[0]; /* ldc r0,SR */
 goto L8c65a296;
L8c65a296: r[14]=1u; /* mov #0x1,r14 */
 goto L8c65a298;
L8c65a298: r[4]=rd(m,0x8c65a2f0u,4); /* mov.l 0x8c65a2f0,r4 */
 goto L8c65a29a;
L8c65a29a: r[5]=rd(m,0x8c65a2ecu,4); /* mov.l 0x8c65a2ec,r5 */
 goto L8c65a29c;
L8c65a29c: r[7]=r[14]; /* mov r14,r7 */
 goto L8c65a29e;
L8c65a29e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a2a0;
L8c65a2a0: r[2]=rd(m,0x8c65a2f4u,4); /* mov.l 0x8c65a2f4,r2 */
 goto L8c65a2a2;
L8c65a2a2: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a2a4;
L8c65a2a4: target=r[2];*pr=0x8c65a2a8u;r[6]=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65a2a8;
L8c65a2a8: r[3]=rd(m,0x8c65a2ecu,4); /* mov.l 0x8c65a2ec,r3 */
 goto L8c65a2aa;
L8c65a2aa: r[2]=4294967294u; /* mov #-0x2,r2 */
 goto L8c65a2ac;
L8c65a2ac: r[5]=rd(m,0x8c65a2f0u,4); /* mov.l 0x8c65a2f0,r5 */
 goto L8c65a2ae;
L8c65a2ae: r[7]=r[14]; /* mov r14,r7 */
 goto L8c65a2b0;
L8c65a2b0: r[1]=rd(m,r[3],4); /* mov.l @r3,r1 */
 goto L8c65a2b2;
L8c65a2b2: r[4]=r[3]; /* mov r3,r4 */
 goto L8c65a2b4;
L8c65a2b4: r[1]&=r[2]; /* and r2,r1 */
 goto L8c65a2b6;
L8c65a2b6: wr(m,r[3],r[1],4); /* mov.l r1,@r3 */
 goto L8c65a2b8;
L8c65a2b8: r[3]=rd(m,0x8c65a2f8u,4); /* mov.l 0x8c65a2f8,r3 */
 goto L8c65a2ba;
L8c65a2ba: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a2bc;
L8c65a2bc: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a2be;
L8c65a2be: target=r[3];*pr=0x8c65a2c2u;r[6]=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65a2c2;
L8c65a2c2: r[15]+=16u; /* add #0x10,r15 */
 goto L8c65a2c4;
L8c65a2c4: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c65a2c6;
L8c65a2c6: r[2]=*sr; /* stc SR,r2 */
 goto L8c65a2c8;
L8c65a2c8: r[3]=sx(m,0x8c65a2e6u,2); /* mov.w 0x8c65a2e6,r3 */
 goto L8c65a2ca;
L8c65a2ca: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c65a2cc;
L8c65a2cc: r[0]<<=2; /* shll2 r0 */
 goto L8c65a2ce;
L8c65a2ce: r[2]&=r[3]; /* and r3,r2 */
 goto L8c65a2d0;
L8c65a2d0: r[0]<<=2; /* shll2 r0 */
 goto L8c65a2d2;
L8c65a2d2: r[0]|=r[2]; /* or r2,r0 */
 goto L8c65a2d4;
L8c65a2d4: *sr=r[0]; /* ldc r0,SR */
 goto L8c65a2d6;
L8c65a2d6: r[4]=sx(m,0x8c65a2e8u,2); /* mov.w 0x8c65a2e8,r4 */
 goto L8c65a2d8;
L8c65a2d8: r[4]--;*sr=(*sr&~1u)|(!r[4]); /* dt r4 */
 goto L8c65a2da;
L8c65a2da: if(!(*sr&1u))goto L8c65a2d8; /* bf 0x8c65a2d8 */
 goto L8c65a2dc;
L8c65a2dc: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65a2de;
L8c65a2de: r[15]+=4u; /* add #0x4,r15 */
 goto L8c65a2e0;
L8c65a2e0: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65a2e2;
L8c65a2e2: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
