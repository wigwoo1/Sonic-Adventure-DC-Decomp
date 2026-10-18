#include "fpu_cleanup_transfer.h"
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
/* Static C recovery of retail Tikal helper 8c064eb2..8c064ef6.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_saved_position_load(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c064eb2;
L8c064eb2: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c064eb4;
L8c064eb4: r[14]=rd(m,0x8c064fe0u,4); /* mov.l 0x8c064fe0,r14 */
 goto L8c064eb6;
L8c064eb6: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c064eb8;
L8c064eb8: r[3]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r3 */
 goto L8c064eba;
L8c064eba: r[2]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r2 */
 goto L8c064ebc;
L8c064ebc: wr(m,r[4],r[3],4); /* mov.l r3,@r4 */
 goto L8c064ebe;
L8c064ebe: wr(m,r[4]+0x4u,r[2],4); /* mov.l r2,@(0x4,r4) */
 goto L8c064ec0;
L8c064ec0: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto L8c064ec2;
L8c064ec2: wr(m,r[4]+0x8u,r[3],4); /* mov.l r3,@(0x8,r4) */
 goto L8c064ec4;
L8c064ec4: r[0]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r0 */
 goto L8c064ec6;
L8c064ec6: r[1]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r1 */
 goto L8c064ec8;
L8c064ec8: r[3]=rd(m,0x8c064fecu,4); /* mov.l 0x8c064fec,r3 */
 goto L8c064eca;
L8c064eca: wr(m,r[5],r[1],4); /* mov.l r1,@r5 */
 goto L8c064ecc;
L8c064ecc: wr(m,r[5]+0x4u,r[0],4); /* mov.l r0,@(0x4,r5) */
 goto L8c064ece;
L8c064ece: r[1]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r1 */
 goto L8c064ed0;
L8c064ed0: wr(m,r[5]+0x8u,r[1],4); /* mov.l r1,@(0x8,r5) */
 goto L8c064ed2;
L8c064ed2: r[0]=sx(m,r[14]+0x4u,1); /* mov.b @(0x4,r14),r0 */
 goto L8c064ed4;
L8c064ed4: r[6]=r[0]; /* mov r0,r6 */
 goto L8c064ed6;
L8c064ed6: r[0]=sx(m,r[14]+0x3u,1); /* mov.b @(0x3,r14),r0 */
 goto L8c064ed8;
L8c064ed8: r[5]=r[0]; /* mov r0,r5 */
 goto L8c064eda;
L8c064eda: r[0]=sx(m,r[14]+0x2u,1); /* mov.b @(0x2,r14),r0 */
 goto L8c064edc;
L8c064edc: target=r[3];*pr=0x8c064ee0u;r[4]=r[0];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c064ee0;
L8c064ee0: r[2]=rd(m,0x8c064ff0u,4); /* mov.l 0x8c064ff0,r2 */
 goto L8c064ee2;
L8c064ee2: target=r[2];*pr=0x8c064ee6u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c064ee6;
L8c064ee6: r[3]=rd(m,0x8c064ff4u,4); /* mov.l 0x8c064ff4,r3 */
 goto L8c064ee8;
L8c064ee8: target=r[3];*pr=0x8c064eecu;r[4]=0u-r[0];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c064eec;
L8c064eec: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c064eee;
L8c064eee: r[2]=rd(m,0x8c064ff8u,4); /* mov.l 0x8c064ff8,r2 */
 goto L8c064ef0;
L8c064ef0: r[4]=0u; /* mov #0x0,r4 */
 goto L8c064ef2;
L8c064ef2: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
}
