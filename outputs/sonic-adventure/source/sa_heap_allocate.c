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
/* Static C recovery of retail Tikal helper 8c098a80..8c098b0c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_heap_allocate(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c098a80;
L8c098a80: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c098a82;
L8c098a82: if((*sr&1u))goto L8c098b06; /* bt 0x8c098b06 */
 goto L8c098a84;
L8c098a84: r[7]=rd(m,0x8c098b60u,4); /* mov.l 0x8c098b60,r7 */
 goto L8c098a86;
L8c098a86: r[6]=rd(m,r[7],4); /* mov.l @r7,r6 */
 goto L8c098a88;
L8c098a88: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c098a8a;
L8c098a8a: if((*sr&1u))goto L8c098b06; /* bt 0x8c098b06 */
 goto L8c098a8c;
L8c098a8c: r[3]=rd(m,r[6]+0x8u,4); /* mov.l @(0x8,r6),r3 */
 goto L8c098a8e;
L8c098a8e: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c098a90;
L8c098a90: if((*sr&1u))goto L8c098b06; /* bt 0x8c098b06 */
 goto L8c098a92;
L8c098a92: r[5]=r[4]; /* mov r4,r5 */
 goto L8c098a94;
L8c098a94: r[4]=r[6]; /* mov r6,r4 */
 goto L8c098a96;
L8c098a96: r[3]=4294967280u; /* mov #-0x10,r3 */
 goto L8c098a98;
L8c098a98: r[5]+=31u; /* add #0x1f,r5 */
 goto L8c098a9a;
L8c098a9a: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c098a9c;
L8c098a9c: r[5]&=r[3]; /* and r3,r5 */
 goto L8c098a9e;
L8c098a9e: take=(*sr&1u);r[6]=0u;if(take)goto L8c098b06;goto L8c098aa2; /* bt/s 0x8c098b06 */
 goto L8c098aa2;
L8c098aa2: r[1]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r1 */
 goto L8c098aa4;
L8c098aa4: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c098aa6;
L8c098aa6: if((*sr&1u))goto L8c098b06; /* bt 0x8c098b06 */
 goto L8c098aa8;
L8c098aa8: r[1]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r1 */
 goto L8c098aaa;
L8c098aaa: r[1]-=r[4]; /* sub r4,r1 */
 goto L8c098aac;
L8c098aac: *sr=(*sr&~1u)|(r[5]==r[1]); /* cmp/eq r5,r1 */
 goto L8c098aae;
L8c098aae: if(!(*sr&1u))goto L8c098ac6; /* bf 0x8c098ac6 */
 goto L8c098ab0;
L8c098ab0: r[5]=rd(m,r[4],4); /* mov.l @r4,r5 */
 goto L8c098ab2;
L8c098ab2: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c098ab4;
L8c098ab4: take=(*sr&1u);r[1]=rd(m,r[4]+0x4u,4);if(take)goto L8c098aba;goto L8c098ab8; /* bt/s 0x8c098aba */
 goto L8c098ab8;
L8c098ab8: wr(m,r[5]+0x4u,r[1],4); /* mov.l r1,@(0x4,r5) */
 goto L8c098aba;
L8c098aba: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c098abc;
L8c098abc: if((*sr&1u))goto L8c098ac2; /* bt 0x8c098ac2 */
 goto L8c098abe;
L8c098abe: wr(m,r[1],r[5],4);goto L8c098af0; /* bra 0x8c098af0 */
 goto L8c098ac2;
L8c098ac2: wr(m,r[7],r[5],4);goto L8c098af0; /* bra 0x8c098af0 */
 goto L8c098ac6;
L8c098ac6: *sr=(*sr&~1u)|(r[1]>r[5]); /* cmp/hi r5,r1 */
 goto L8c098ac8;
L8c098ac8: if(!(*sr&1u))goto L8c098afa; /* bf 0x8c098afa */
 goto L8c098aca;
L8c098aca: r[1]=rd(m,r[4]+0x4u,4); /* mov.l @(0x4,r4),r1 */
 goto L8c098acc;
L8c098acc: r[5]+=r[4]; /* add r4,r5 */
 goto L8c098ace;
L8c098ace: r[0]=rd(m,r[4],4); /* mov.l @r4,r0 */
 goto L8c098ad0;
L8c098ad0: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c098ad2;
L8c098ad2: wr(m,r[5],r[0],4); /* mov.l r0,@r5 */
 goto L8c098ad4;
L8c098ad4: wr(m,r[5]+0x4u,r[1],4); /* mov.l r1,@(0x4,r5) */
 goto L8c098ad6;
L8c098ad6: r[3]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r3 */
 goto L8c098ad8;
L8c098ad8: wr(m,r[5]+0x8u,r[3],4); /* mov.l r3,@(0x8,r5) */
 goto L8c098ada;
L8c098ada: wr(m,r[5]+0xcu,r[4],4); /* mov.l r4,@(0xc,r5) */
 goto L8c098adc;
L8c098adc: r[2]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r2 */
 goto L8c098ade;
L8c098ade: wr(m,r[2]+0xcu,r[5],4); /* mov.l r5,@(0xc,r2) */
 goto L8c098ae0;
L8c098ae0: take=(*sr&1u);wr(m,r[4]+0x8u,r[5],4);if(take)goto L8c098ae6;goto L8c098ae4; /* bt/s 0x8c098ae6 */
 goto L8c098ae4;
L8c098ae4: wr(m,r[0]+0x4u,r[5],4); /* mov.l r5,@(0x4,r0) */
 goto L8c098ae6;
L8c098ae6: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c098ae8;
L8c098ae8: if((*sr&1u))goto L8c098aee; /* bt 0x8c098aee */
 goto L8c098aea;
L8c098aea: wr(m,r[1],r[5],4);goto L8c098af0; /* bra 0x8c098af0 */
 goto L8c098aee;
L8c098aee: wr(m,r[7],r[5],4); /* mov.l r5,@r7 */
 goto L8c098af0;
L8c098af0: wr(m,r[4],r[6],4); /* mov.l r6,@r4 */
 goto L8c098af2;
L8c098af2: wr(m,r[4]+0x4u,r[6],4); /* mov.l r6,@(0x4,r4) */
 goto L8c098af4;
L8c098af4: r[4]+=16u; /* add #0x10,r4 */
 goto L8c098af6;
L8c098af6: r[0]=r[4];return 0; /* rts */
L8c098afa: r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c098afc;
L8c098afc: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c098afe;
L8c098afe: if((*sr&1u))goto L8c098b06; /* bt 0x8c098b06 */
 goto L8c098b00;
L8c098b00: r[3]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r3 */
 goto L8c098b02;
L8c098b02: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c098b04;
L8c098b04: if(!(*sr&1u))goto L8c098aa8; /* bf 0x8c098aa8 */
 goto L8c098b06;
L8c098b06: r[0]=0u; /* mov #0x0,r0 */
 goto L8c098b08;
L8c098b08: return 0; /* rts */
}
