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
/* Static C recovery of retail Tikal helper 8c665a80..8c665aac.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_option_select(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c665a80;
L8c665a80: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c665a82;
L8c665a82: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c665a84;
L8c665a84: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c665a86;
L8c665a86: r[3]=rd(m,0x8c665ac4u,4); /* mov.l 0x8c665ac4,r3 */
 goto L8c665a88;
L8c665a88: r[5]=rd(m,0x8c665ac0u,4); /* mov.l 0x8c665ac0,r5 */
 goto L8c665a8a;
L8c665a8a: target=r[3];*pr=0x8c665a8eu;wr(m,r[15]+0x4u,r[6],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c665a8e;
L8c665a8e: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c665a90;
L8c665a90: if(!(*sr&1u))goto L8c665aa2; /* bf 0x8c665aa2 */
 goto L8c665a92;
L8c665a92: r[3]=rd(m,0x8c665ac4u,4); /* mov.l 0x8c665ac4,r3 */
 goto L8c665a94;
L8c665a94: r[5]=rd(m,0x8c665ac8u,4); /* mov.l 0x8c665ac8,r5 */
 goto L8c665a96;
L8c665a96: target=r[3];*pr=0x8c665a9au;r[4]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c665a9a;
L8c665a9a: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c665a9c;
L8c665a9c: if(!(*sr&1u))goto L8c665aa4; /* bf 0x8c665aa4 */
 goto L8c665a9e;
L8c665a9e: r[4]=0u;goto L8c665aa4; /* bra 0x8c665aa4 */
 goto L8c665aa2;
L8c665aa2: r[4]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r4 */
 goto L8c665aa4;
L8c665aa4: r[15]+=8u; /* add #0x8,r15 */
 goto L8c665aa6;
L8c665aa6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c665aa8;
L8c665aa8: r[0]=r[4];return 0; /* rts */
}
