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
/* Static C recovery of retail Tikal helper 0cb00fbe..0cb0101c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int tikal_effect_apply(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto Lcb00fbe;
Lcb00fbe: r[4]=rd(m,r[4]+0x24u,4); /* mov.l @(0x24,r4),r4 */
 goto Lcb00fc0;
Lcb00fc0: r[0]=88u; /* mov #0x58,r0 */
 goto Lcb00fc2;
Lcb00fc2: r[2]=r[4]; /* mov r4,r2 */
 goto Lcb00fc4;
Lcb00fc4: r[2]+=100u; /* add #0x64,r2 */
 goto Lcb00fc6;
Lcb00fc6: r[1]=rd(m,r[2],4); /* mov.l @r2,r1 */
 goto Lcb00fc8;
Lcb00fc8: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto Lcb00fca;
Lcb00fca: wr(m,r[3]+0x14u,r[1],4); /* mov.l r1,@(0x14,r3) */
 goto Lcb00fcc;
Lcb00fcc: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto Lcb00fce;
Lcb00fce: r[0]=104u; /* mov #0x68,r0 */
 goto Lcb00fd0;
Lcb00fd0: r[2]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r2 */
 goto Lcb00fd2;
Lcb00fd2: r[0]=88u; /* mov #0x58,r0 */
 goto Lcb00fd4;
Lcb00fd4: wr(m,r[3]+0x18u,r[2],4); /* mov.l r2,@(0x18,r3) */
 goto Lcb00fd6;
Lcb00fd6: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto Lcb00fd8;
Lcb00fd8: r[0]=108u; /* mov #0x6c,r0 */
 goto Lcb00fda;
Lcb00fda: r[2]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r2 */
 goto Lcb00fdc;
Lcb00fdc: r[0]=96u; /* mov #0x60,r0 */
 goto Lcb00fde;
Lcb00fde: wr(m,r[3]+0x1cu,r[2],4); /* mov.l r2,@(0x1c,r3) */
 goto Lcb00fe0;
Lcb00fe0: r[2]=r[4]; /* mov r4,r2 */
 goto Lcb00fe2;
Lcb00fe2: r[2]+=124u; /* add #0x7c,r2 */
 goto Lcb00fe4;
Lcb00fe4: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto Lcb00fe6;
Lcb00fe6: r[1]=rd(m,r[2],4); /* mov.l @r2,r1 */
 goto Lcb00fe8;
Lcb00fe8: wr(m,r[3]+0x14u,r[1],4); /* mov.l r1,@(0x14,r3) */
 goto Lcb00fea;
Lcb00fea: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto Lcb00fec;
Lcb00fec: r[0]+=32u; /* add #0x20,r0 */
 goto Lcb00fee;
Lcb00fee: r[2]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r2 */
 goto Lcb00ff0;
Lcb00ff0: r[0]=96u; /* mov #0x60,r0 */
 goto Lcb00ff2;
Lcb00ff2: wr(m,r[3]+0x18u,r[2],4); /* mov.l r2,@(0x18,r3) */
 goto Lcb00ff4;
Lcb00ff4: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto Lcb00ff6;
Lcb00ff6: r[0]+=36u; /* add #0x24,r0 */
 goto Lcb00ff8;
Lcb00ff8: r[2]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r2 */
 goto Lcb00ffa;
Lcb00ffa: r[0]=92u; /* mov #0x5c,r0 */
 goto Lcb00ffc;
Lcb00ffc: wr(m,r[3]+0x1cu,r[2],4); /* mov.l r2,@(0x1c,r3) */
 goto Lcb00ffe;
Lcb00ffe: r[2]=r[4]; /* mov r4,r2 */
 goto Lcb01000;
Lcb01000: r[2]+=112u; /* add #0x70,r2 */
 goto Lcb01002;
Lcb01002: r[1]=rd(m,r[2],4); /* mov.l @r2,r1 */
 goto Lcb01004;
Lcb01004: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto Lcb01006;
Lcb01006: wr(m,r[3]+0x14u,r[1],4); /* mov.l r1,@(0x14,r3) */
 goto Lcb01008;
Lcb01008: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto Lcb0100a;
Lcb0100a: r[0]=116u; /* mov #0x74,r0 */
 goto Lcb0100c;
Lcb0100c: r[2]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r2 */
 goto Lcb0100e;
Lcb0100e: r[0]=92u; /* mov #0x5c,r0 */
 goto Lcb01010;
Lcb01010: wr(m,r[3]+0x18u,r[2],4); /* mov.l r2,@(0x18,r3) */
 goto Lcb01012;
Lcb01012: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto Lcb01014;
Lcb01014: r[0]=120u; /* mov #0x78,r0 */
 goto Lcb01016;
Lcb01016: r[2]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r2 */
 goto Lcb01018;
Lcb01018: wr(m,r[3]+0x1cu,r[2],4);return 0; /* rts */
}
