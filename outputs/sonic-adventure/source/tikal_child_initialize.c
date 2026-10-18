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
/* Static C recovery of retail Tikal helper 0cb00ed4..0cb00f08.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int tikal_child_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto Lcb00ed4;
Lcb00ed4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto Lcb00ed6;
Lcb00ed6: r[5]=32u; /* mov #0x20,r5 */
 goto Lcb00ed8;
Lcb00ed8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto Lcb00eda;
Lcb00eda: r[3]=rd(m,0x0cb00f28u,4); /* mov.l 0x0cb00f28,r3 */
 goto Lcb00edc;
Lcb00edc: r[14]=r[4]; /* mov r4,r14 */
 goto Lcb00ede;
Lcb00ede: target=r[3];*pr=0xcb00ee2u;r[4]=1u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00ee2;
Lcb00ee2: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00ee4;
Lcb00ee4: take=(*sr&1u);r[4]=r[0];if(take)goto Lcb00f02;goto Lcb00ee8; /* bt/s 0x0cb00f02 */
 goto Lcb00ee8;
Lcb00ee8: r[2]=rd(m,0x0cb00f30u,4); /* mov.l 0x0cb00f30,r2 */
 goto Lcb00eea;
Lcb00eea: r[3]=rd(m,0x0cb00f2cu,4); /* mov.l 0x0cb00f2c,r3 */
 goto Lcb00eec;
Lcb00eec: wr(m,r[14]+0x24u,r[4],4); /* mov.l r4,@(0x24,r14) */
 goto Lcb00eee;
Lcb00eee: wr(m,r[4]+0x10u,r[3],4); /* mov.l r3,@(0x10,r4) */
 goto Lcb00ef0;
Lcb00ef0: wr(m,r[4]+0x14u,r[2],4); /* mov.l r2,@(0x14,r4) */
 goto Lcb00ef2;
Lcb00ef2: r[2]=rd(m,0x0cb00f38u,4); /* mov.l 0x0cb00f38,r2 */
 goto Lcb00ef4;
Lcb00ef4: r[3]=rd(m,0x0cb00f34u,4); /* mov.l 0x0cb00f34,r3 */
 goto Lcb00ef6;
Lcb00ef6: wr(m,r[4]+0x18u,r[3],4); /* mov.l r3,@(0x18,r4) */
 goto Lcb00ef8;
Lcb00ef8: wr(m,r[4]+0x1cu,r[2],4); /* mov.l r2,@(0x1c,r4) */
 goto Lcb00efa;
Lcb00efa: r[2]=rd(m,0x0cb00f40u,4); /* mov.l 0x0cb00f40,r2 */
 goto Lcb00efc;
Lcb00efc: r[3]=rd(m,0x0cb00f3cu,4); /* mov.l 0x0cb00f3c,r3 */
 goto Lcb00efe;
Lcb00efe: wr(m,r[14]+0x18u,r[3],4); /* mov.l r3,@(0x18,r14) */
 goto Lcb00f00;
Lcb00f00: wr(m,r[14]+0x10u,r[2],4); /* mov.l r2,@(0x10,r14) */
 goto Lcb00f02;
Lcb00f02: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto Lcb00f04;
Lcb00f04: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
