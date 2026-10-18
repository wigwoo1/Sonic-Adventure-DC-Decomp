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
/* Static C recovery of retail File handle initializer helper 8c600464..8c6004d4.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_handle_open_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c600464;
L8c600464: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c600466;
L8c600466: r[3]=sx(m,r[4]+r[0],2); /* mov.w @(r0,r4),r3 */
 goto L8c600468;
L8c600468: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c60046a;
L8c60046a: if((*sr&1u))goto L8c600484; /* bt 0x8c600484 */
 goto L8c60046c;
L8c60046c: r[0]=0u;return 0; /* rts */
L8c600484: r[3]=sx(m,0x8c600594u,2); /* mov.w 0x8c600594,r3 */
 goto L8c600486;
L8c600486: r[2]=r[7]; /* mov r7,r2 */
 goto L8c600488;
L8c600488: r[1]=4294967285u; /* mov #-0xb,r1 */
 goto L8c60048a;
L8c60048a: wr(m,r[4]+0x4u,r[5],4); /* mov.l r5,@(0x4,r4) */
 goto L8c60048c;
L8c60048c: r[2]+=r[3]; /* add r3,r2 */
 goto L8c60048e;
L8c60048e: wr(m,r[4]+0x8u,r[6],4); /* mov.l r6,@(0x8,r4) */
 goto L8c600490;
L8c600490: r[5]=0u; /* mov #0x0,r5 */
 goto L8c600492;
L8c600492: wr(m,r[4]+0xcu,r[7],4); /* mov.l r7,@(0xc,r4) */
 goto L8c600494;
L8c600494: {uint32_t shift=r[1];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}} /* shad r1,r2 */
 goto L8c600496;
L8c600496: r[0]=64u; /* mov #0x40,r0 */
 goto L8c600498;
L8c600498: wr(m,r[4]+0x10u,r[2],4); /* mov.l r2,@(0x10,r4) */
 goto L8c60049a;
L8c60049a: wr(m,r[4]+0x14u,r[5],4); /* mov.l r5,@(0x14,r4) */
 goto L8c60049c;
L8c60049c: wr(m,r[4]+0x18u,r[5],4); /* mov.l r5,@(0x18,r4) */
 goto L8c60049e;
L8c60049e: wr(m,r[4]+0x1cu,r[5],4); /* mov.l r5,@(0x1c,r4) */
 goto L8c6004a0;
L8c6004a0: wr(m,r[4]+0x20u,r[5],4); /* mov.l r5,@(0x20,r4) */
 goto L8c6004a2;
L8c6004a2: wr(m,r[4]+0x24u,r[5],4); /* mov.l r5,@(0x24,r4) */
 goto L8c6004a4;
L8c6004a4: wr(m,r[4]+0x2cu,r[5],4); /* mov.l r5,@(0x2c,r4) */
 goto L8c6004a6;
L8c6004a6: wr(m,r[4]+0x34u,r[5],4); /* mov.l r5,@(0x34,r4) */
 goto L8c6004a8;
L8c6004a8: wr(m,r[4]+0x3cu,r[5],4); /* mov.l r5,@(0x3c,r4) */
 goto L8c6004aa;
L8c6004aa: wr(m,r[4]+r[0],r[5],4); /* mov.l r5,@(r0,r4) */
 goto L8c6004ac;
L8c6004ac: r[0]=68u; /* mov #0x44,r0 */
 goto L8c6004ae;
L8c6004ae: wr(m,r[4]+r[0],r[5],4); /* mov.l r5,@(r0,r4) */
 goto L8c6004b0;
L8c6004b0: r[0]=72u; /* mov #0x48,r0 */
 goto L8c6004b2;
L8c6004b2: wr(m,r[4]+r[0],r[5],2); /* mov.w r5,@(r0,r4) */
 goto L8c6004b4;
L8c6004b4: r[0]=74u; /* mov #0x4a,r0 */
 goto L8c6004b6;
L8c6004b6: wr(m,r[4]+r[0],r[5],2); /* mov.w r5,@(r0,r4) */
 goto L8c6004b8;
L8c6004b8: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c6004ba;
L8c6004ba: r[2]=1u; /* mov #0x1,r2 */
 goto L8c6004bc;
L8c6004bc: wr(m,r[4]+r[0],r[2],2); /* mov.w r2,@(r0,r4) */
 goto L8c6004be;
L8c6004be: r[0]=78u; /* mov #0x4e,r0 */
 goto L8c6004c0;
L8c6004c0: r[2]=rd(m,0x8c600598u,4); /* mov.l 0x8c600598,r2 */
 goto L8c6004c2;
L8c6004c2: r[1]=sx(m,r[2],2); /* mov.w @r2,r1 */
 goto L8c6004c4;
L8c6004c4: wr(m,r[4]+r[0],r[1],2); /* mov.w r1,@(r0,r4) */
 goto L8c6004c6;
L8c6004c6: r[0]=80u; /* mov #0x50,r0 */
 goto L8c6004c8;
L8c6004c8: wr(m,r[4]+r[0],r[5],2); /* mov.w r5,@(r0,r4) */
 goto L8c6004ca;
L8c6004ca: r[0]=82u; /* mov #0x52,r0 */
 goto L8c6004cc;
L8c6004cc: wr(m,r[4]+r[0],r[5],2); /* mov.w r5,@(r0,r4) */
 goto L8c6004ce;
L8c6004ce: r[0]=r[4]; /* mov r4,r0 */
 goto L8c6004d0;
L8c6004d0: return 0; /* rts */
}
