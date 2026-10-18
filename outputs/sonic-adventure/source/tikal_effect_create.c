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
/* Static C recovery of retail Tikal helper 0cb00f60..0cb00fbe.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int tikal_effect_create(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto Lcb00f60;
Lcb00f60: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto Lcb00f62;
Lcb00f62: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto Lcb00f64;
Lcb00f64: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto Lcb00f66;
Lcb00f66: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto Lcb00f68;
Lcb00f68: r[6]=rd(m,0x0cb010c0u,4); /* mov.l 0x0cb010c0,r6 */
 goto Lcb00f6a;
Lcb00f6a: r[13]=r[5]; /* mov r5,r13 */
 goto Lcb00f6c;
Lcb00f6c: r[3]=rd(m,0x0cb010c4u,4); /* mov.l 0x0cb010c4,r3 */
 goto Lcb00f6e;
Lcb00f6e: r[5]=2u; /* mov #0x2,r5 */
 goto Lcb00f70;
Lcb00f70: wr(m,r[15],r[4],1); /* mov.b r4,@r15 */
 goto Lcb00f72;
Lcb00f72: target=r[3];*pr=0xcb00f76u;r[4]=r[5];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00f76;
Lcb00f76: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00f78;
Lcb00f78: take=(*sr&1u);r[14]=r[0];if(take)goto Lcb00fb2;goto Lcb00f7c; /* bt/s 0x0cb00fb2 */
 goto Lcb00f7c;
Lcb00f7c: r[3]=rd(m,0x0cb010c8u,4); /* mov.l 0x0cb010c8,r3 */
 goto Lcb00f7e;
Lcb00f7e: r[4]=4294967176u; /* mov #-0x78,r4 */
 goto Lcb00f80;
Lcb00f80: r[4]=r[4]&255u; /* extu.b r4,r4 */
 goto Lcb00f82;
Lcb00f82: target=r[3];*pr=0xcb00f86u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00f86;
Lcb00f86: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00f88;
Lcb00f88: take=!(*sr&1u);r[4]=r[0];if(take)goto Lcb00f96;goto Lcb00f8c; /* bf/s 0x0cb00f96 */
 goto Lcb00f8c;
Lcb00f8c: r[3]=rd(m,0x0cb010ccu,4); /* mov.l 0x0cb010cc,r3 */
 goto Lcb00f8e;
Lcb00f8e: target=r[3];*pr=0xcb00f92u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00f92;
Lcb00f92: goto Lcb00fb2; /* bra 0x0cb00fb2 */
 goto Lcb00f96;
Lcb00f96: wr(m,r[14]+0x24u,r[4],4); /* mov.l r4,@(0x24,r14) */
 goto Lcb00f98;
Lcb00f98: r[0]=sx(m,r[15],1); /* mov.b @r15,r0 */
 goto Lcb00f9a;
Lcb00f9a: r[2]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r2 */
 goto Lcb00f9c;
Lcb00f9c: wr(m,r[2]+0x8u,r[0],1); /* mov.b r0,@(0x8,r2) */
 goto Lcb00f9e;
Lcb00f9e: r[0]=88u; /* mov #0x58,r0 */
 goto Lcb00fa0;
Lcb00fa0: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto Lcb00fa2;
Lcb00fa2: wr(m,r[4]+r[0],r[3],4); /* mov.l r3,@(r0,r4) */
 goto Lcb00fa4;
Lcb00fa4: r[0]=92u; /* mov #0x5c,r0 */
 goto Lcb00fa6;
Lcb00fa6: r[2]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r2 */
 goto Lcb00fa8;
Lcb00fa8: wr(m,r[4]+r[0],r[2],4); /* mov.l r2,@(r0,r4) */
 goto Lcb00faa;
Lcb00faa: r[0]=96u; /* mov #0x60,r0 */
 goto Lcb00fac;
Lcb00fac: r[3]=rd(m,r[13]+0x8u,4); /* mov.l @(0x8,r13),r3 */
 goto Lcb00fae;
Lcb00fae: wr(m,r[4]+r[0],r[3],4);goto Lcb00fb4; /* bra 0x0cb00fb4 */
 goto Lcb00fb2;
Lcb00fb2: r[0]=0u; /* mov #0x0,r0 */
 goto Lcb00fb4;
Lcb00fb4: r[15]+=4u; /* add #0x4,r15 */
 goto Lcb00fb6;
Lcb00fb6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto Lcb00fb8;
Lcb00fb8: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto Lcb00fba;
Lcb00fba: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
