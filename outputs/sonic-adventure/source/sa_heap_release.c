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
/* Retail heap release 8c098b42..8c098c2a. Original block coalescing and free-list writes, including invalid-header fail-stop loop. */
SA_STAGE_EXPORT int sa_heap_release(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c098b42;
L8c098b42: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c098b44;
L8c098b44: if(!(*sr&1u))goto L8c098b4a; /* bf 0x8c098b4a */
 goto L8c098b46;
L8c098b46: goto L8c098c28; /* bra 0x8c098c28 */
 goto L8c098b4a;
L8c098b4a: r[5]=r[4]; /* mov r4,r5 */
 goto L8c098b4c;
L8c098b4c: r[5]+=4294967280u; /* add #-0x10,r5 */
 goto L8c098b4e;
L8c098b4e: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c098b50;
L8c098b50: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098b52;
L8c098b52: if((*sr&1u))goto L8c098b80; /* bt 0x8c098b80 */
 goto L8c098b54;
L8c098b54: r[3]=rd(m,r[5]+0x4u,4); /* mov.l @(0x4,r5),r3 */
 goto L8c098b56;
L8c098b56: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c098b58;
L8c098b58: if((*sr&1u))goto L8c098b80; /* bt 0x8c098b80 */
 goto L8c098b5a;
L8c098b5a: goto L8c098b5a; /* bra 0x8c098b5a */
 goto L8c098b80;
L8c098b80: r[6]=rd(m,r[5]+0xcu,4); /* mov.l @(0xc,r5),r6 */
 goto L8c098b82;
L8c098b82: r[7]=rd(m,0x8c098c80u,4); /* mov.l 0x8c098c80,r7 */
 goto L8c098b84;
L8c098b84: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c098b86;
L8c098b86: take=(*sr&1u);r[4]=rd(m,r[5]+0x8u,4);if(take)goto L8c098be0;goto L8c098b8a; /* bt/s 0x8c098be0 */
 goto L8c098b8a;
L8c098b8a: r[2]=rd(m,r[6],4); /* mov.l @r6,r2 */
 goto L8c098b8c;
L8c098b8c: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098b8e;
L8c098b8e: if(!(*sr&1u))goto L8c098b96; /* bf 0x8c098b96 */
 goto L8c098b90;
L8c098b90: r[3]=rd(m,r[6]+0x4u,4); /* mov.l @(0x4,r6),r3 */
 goto L8c098b92;
L8c098b92: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c098b94;
L8c098b94: if((*sr&1u))goto L8c098bca; /* bt 0x8c098bca */
 goto L8c098b96;
L8c098b96: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c098b98;
L8c098b98: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c098b9a;
L8c098b9a: if(!(*sr&1u))goto L8c098ba2; /* bf 0x8c098ba2 */
 goto L8c098b9c;
L8c098b9c: r[3]=rd(m,r[4]+0x4u,4); /* mov.l @(0x4,r4),r3 */
 goto L8c098b9e;
L8c098b9e: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c098ba0;
L8c098ba0: if((*sr&1u))goto L8c098bca; /* bt 0x8c098bca */
 goto L8c098ba2;
L8c098ba2: r[3]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r3 */
 goto L8c098ba4;
L8c098ba4: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c098ba6;
L8c098ba6: if((*sr&1u))goto L8c098bca; /* bt 0x8c098bca */
 goto L8c098ba8;
L8c098ba8: wr(m,r[4]+0xcu,r[6],4); /* mov.l r6,@(0xc,r4) */
 goto L8c098baa;
L8c098baa: wr(m,r[6]+0x8u,r[4],4); /* mov.l r4,@(0x8,r6) */
 goto L8c098bac;
L8c098bac: r[5]=rd(m,r[4],4); /* mov.l @r4,r5 */
 goto L8c098bae;
L8c098bae: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c098bb0;
L8c098bb0: take=(*sr&1u);r[0]=rd(m,r[4]+0x4u,4);if(take)goto L8c098bb6;goto L8c098bb4; /* bt/s 0x8c098bb6 */
 goto L8c098bb4;
L8c098bb4: wr(m,r[5]+0x4u,r[0],4); /* mov.l r0,@(0x4,r5) */
 goto L8c098bb6;
L8c098bb6: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c098bb8;
L8c098bb8: if((*sr&1u))goto L8c098bbe; /* bt 0x8c098bbe */
 goto L8c098bba;
L8c098bba: wr(m,r[0],r[5],4);goto L8c098bc0; /* bra 0x8c098bc0 */
 goto L8c098bbe;
L8c098bbe: wr(m,r[7],r[5],4); /* mov.l r5,@r7 */
 goto L8c098bc0;
L8c098bc0: r[2]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r2 */
 goto L8c098bc2;
L8c098bc2: wr(m,r[2]+0xcu,r[6],4); /* mov.l r6,@(0xc,r2) */
 goto L8c098bc4;
L8c098bc4: r[3]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r3 */
 goto L8c098bc6;
L8c098bc6: wr(m,r[6]+0x8u,r[3],4);goto L8c098c28; /* bra 0x8c098c28 */
 goto L8c098bca;
L8c098bca: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c098bcc;
L8c098bcc: if((*sr&1u))goto L8c098be0; /* bt 0x8c098be0 */
 goto L8c098bce;
L8c098bce: r[2]=rd(m,r[6],4); /* mov.l @r6,r2 */
 goto L8c098bd0;
L8c098bd0: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098bd2;
L8c098bd2: if(!(*sr&1u))goto L8c098bda; /* bf 0x8c098bda */
 goto L8c098bd4;
L8c098bd4: r[2]=rd(m,r[6]+0x4u,4); /* mov.l @(0x4,r6),r2 */
 goto L8c098bd6;
L8c098bd6: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098bd8;
L8c098bd8: if((*sr&1u))goto L8c098be0; /* bt 0x8c098be0 */
 goto L8c098bda;
L8c098bda: wr(m,r[4]+0xcu,r[6],4); /* mov.l r6,@(0xc,r4) */
 goto L8c098bdc;
L8c098bdc: wr(m,r[6]+0x8u,r[4],4);goto L8c098c28; /* bra 0x8c098c28 */
 goto L8c098be0;
L8c098be0: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c098be2;
L8c098be2: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098be4;
L8c098be4: if(!(*sr&1u))goto L8c098bec; /* bf 0x8c098bec */
 goto L8c098be6;
L8c098be6: r[3]=rd(m,r[4]+0x4u,4); /* mov.l @(0x4,r4),r3 */
 goto L8c098be8;
L8c098be8: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c098bea;
L8c098bea: if((*sr&1u))goto L8c098c18; /* bt 0x8c098c18 */
 goto L8c098bec;
L8c098bec: r[3]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r3 */
 goto L8c098bee;
L8c098bee: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c098bf0;
L8c098bf0: if((*sr&1u))goto L8c098c18; /* bt 0x8c098c18 */
 goto L8c098bf2;
L8c098bf2: r[6]=rd(m,r[4],4); /* mov.l @r4,r6 */
 goto L8c098bf4;
L8c098bf4: r[0]=rd(m,r[4]+0x4u,4); /* mov.l @(0x4,r4),r0 */
 goto L8c098bf6;
L8c098bf6: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c098bf8;
L8c098bf8: wr(m,r[5],r[6],4); /* mov.l r6,@r5 */
 goto L8c098bfa;
L8c098bfa: take=(*sr&1u);wr(m,r[5]+0x4u,r[0],4);if(take)goto L8c098c02;goto L8c098bfe; /* bt/s 0x8c098c02 */
 goto L8c098bfe;
L8c098bfe: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c098c00;
L8c098c00: wr(m,r[2]+0x4u,r[5],4); /* mov.l r5,@(0x4,r2) */
 goto L8c098c02;
L8c098c02: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c098c04;
L8c098c04: if((*sr&1u))goto L8c098c0c; /* bt 0x8c098c0c */
 goto L8c098c06;
L8c098c06: r[2]=rd(m,r[4]+0x4u,4); /* mov.l @(0x4,r4),r2 */
 goto L8c098c08;
L8c098c08: wr(m,r[2],r[5],4);goto L8c098c0e; /* bra 0x8c098c0e */
 goto L8c098c0c;
L8c098c0c: wr(m,r[7],r[5],4); /* mov.l r5,@r7 */
 goto L8c098c0e;
L8c098c0e: r[2]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r2 */
 goto L8c098c10;
L8c098c10: wr(m,r[5]+0x8u,r[2],4); /* mov.l r2,@(0x8,r5) */
 goto L8c098c12;
L8c098c12: r[3]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r3 */
 goto L8c098c14;
L8c098c14: wr(m,r[3]+0xcu,r[5],4);goto L8c098c28; /* bra 0x8c098c28 */
 goto L8c098c18;
L8c098c18: r[4]=rd(m,r[7],4); /* mov.l @r7,r4 */
 goto L8c098c1a;
L8c098c1a: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c098c1c;
L8c098c1c: take=(*sr&1u);wr(m,r[7],r[5],4);if(take)goto L8c098c22;goto L8c098c20; /* bt/s 0x8c098c22 */
 goto L8c098c20;
L8c098c20: wr(m,r[4]+0x4u,r[5],4); /* mov.l r5,@(0x4,r4) */
 goto L8c098c22;
L8c098c22: r[2]=0u; /* mov #0x0,r2 */
 goto L8c098c24;
L8c098c24: wr(m,r[5],r[4],4); /* mov.l r4,@r5 */
 goto L8c098c26;
L8c098c26: wr(m,r[5]+0x4u,r[2],4); /* mov.l r2,@(0x4,r5) */
 goto L8c098c28;
L8c098c28: return 0; /* rts */
}
