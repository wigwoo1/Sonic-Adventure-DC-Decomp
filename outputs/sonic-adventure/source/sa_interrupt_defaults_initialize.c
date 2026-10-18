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
/* Static C recovery of retail Tikal helper 8c641160..8c6411b0.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_interrupt_defaults_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c641160;
L8c641160: r[3]=rd(m,0x8c641200u,4); /* mov.l 0x8c641200,r3 */
 goto L8c641162;
L8c641162: r[4]=0u; /* mov #0x0,r4 */
 goto L8c641164;
L8c641164: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c641166;
L8c641166: r[1]=rd(m,0x8c641208u,4); /* mov.l 0x8c641208,r1 */
 goto L8c641168;
L8c641168: r[2]=rd(m,0x8c641204u,4); /* mov.l 0x8c641204,r2 */
 goto L8c64116a;
L8c64116a: wr(m,r[2],r[1],4); /* mov.l r1,@r2 */
 goto L8c64116c;
L8c64116c: wr(m,r[2]+0x4u,r[4],4); /* mov.l r4,@(0x4,r2) */
 goto L8c64116e;
L8c64116e: r[1]=rd(m,0x8c64120cu,4); /* mov.l 0x8c64120c,r1 */
 goto L8c641170;
L8c641170: r[3]+=4294967188u; /* add #-0x6c,r3 */
 goto L8c641172;
L8c641172: wr(m,r[3],r[1],4); /* mov.l r1,@r3 */
 goto L8c641174;
L8c641174: wr(m,r[3]+0x4u,r[4],4); /* mov.l r4,@(0x4,r3) */
 goto L8c641176;
L8c641176: r[1]=rd(m,0x8c641210u,4); /* mov.l 0x8c641210,r1 */
 goto L8c641178;
L8c641178: r[2]+=16u; /* add #0x10,r2 */
 goto L8c64117a;
L8c64117a: wr(m,r[2],r[1],4); /* mov.l r1,@r2 */
 goto L8c64117c;
L8c64117c: wr(m,r[2]+0x4u,r[4],4); /* mov.l r4,@(0x4,r2) */
 goto L8c64117e;
L8c64117e: r[1]=rd(m,0x8c641214u,4); /* mov.l 0x8c641214,r1 */
 goto L8c641180;
L8c641180: r[3]+=16u; /* add #0x10,r3 */
 goto L8c641182;
L8c641182: wr(m,r[3],r[1],4); /* mov.l r1,@r3 */
 goto L8c641184;
L8c641184: wr(m,r[3]+0x4u,r[4],4); /* mov.l r4,@(0x4,r3) */
 goto L8c641186;
L8c641186: r[1]=rd(m,0x8c641218u,4); /* mov.l 0x8c641218,r1 */
 goto L8c641188;
L8c641188: r[2]+=16u; /* add #0x10,r2 */
 goto L8c64118a;
L8c64118a: wr(m,r[2],r[1],4); /* mov.l r1,@r2 */
 goto L8c64118c;
L8c64118c: wr(m,r[2]+0x4u,r[4],4); /* mov.l r4,@(0x4,r2) */
 goto L8c64118e;
L8c64118e: r[1]=rd(m,0x8c64121cu,4); /* mov.l 0x8c64121c,r1 */
 goto L8c641190;
L8c641190: r[3]+=16u; /* add #0x10,r3 */
 goto L8c641192;
L8c641192: wr(m,r[3],r[1],4); /* mov.l r1,@r3 */
 goto L8c641194;
L8c641194: wr(m,r[3]+0x4u,r[4],4); /* mov.l r4,@(0x4,r3) */
 goto L8c641196;
L8c641196: r[1]=rd(m,0x8c641220u,4); /* mov.l 0x8c641220,r1 */
 goto L8c641198;
L8c641198: r[2]+=16u; /* add #0x10,r2 */
 goto L8c64119a;
L8c64119a: wr(m,r[2],r[1],4); /* mov.l r1,@r2 */
 goto L8c64119c;
L8c64119c: wr(m,r[2]+0x4u,r[4],4); /* mov.l r4,@(0x4,r2) */
 goto L8c64119e;
L8c64119e: r[1]=rd(m,0x8c641224u,4); /* mov.l 0x8c641224,r1 */
 goto L8c6411a0;
L8c6411a0: r[3]+=16u; /* add #0x10,r3 */
 goto L8c6411a2;
L8c6411a2: wr(m,r[3],r[1],4); /* mov.l r1,@r3 */
 goto L8c6411a4;
L8c6411a4: wr(m,r[3]+0x4u,r[4],4); /* mov.l r4,@(0x4,r3) */
 goto L8c6411a6;
L8c6411a6: r[1]=rd(m,0x8c641228u,4); /* mov.l 0x8c641228,r1 */
 goto L8c6411a8;
L8c6411a8: r[2]+=16u; /* add #0x10,r2 */
 goto L8c6411aa;
L8c6411aa: wr(m,r[2],r[1],4); /* mov.l r1,@r2 */
 goto L8c6411ac;
L8c6411ac: wr(m,r[2]+0x4u,r[4],4);return 0; /* rts */
}
