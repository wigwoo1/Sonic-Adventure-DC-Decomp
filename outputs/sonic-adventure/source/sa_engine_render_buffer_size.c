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
/* Retail render-buffer size calculation, 8c665e00..8c665e3c.
 * No external services; integer instructions preserve FPU state. */
SA_STAGE_EXPORT int sa_engine_render_buffer_size(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c665e00;
L8c665e00: r[3]=rd(m,0x8c665ef0u,4); /* mov.l 0x8c665ef0,r3 */
 goto L8c665e02;
L8c665e02: r[5]=rd(m,0x8c665eecu,4); /* mov.l 0x8c665eec,r5 */
 goto L8c665e04;
L8c665e04: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c665e06;
L8c665e06: r[0]<<=2; /* shll2 r0 */
 goto L8c665e08;
L8c665e08: r[1]=rd(m,0x8c665ef4u,4); /* mov.l 0x8c665ef4,r1 */
 goto L8c665e0a;
L8c665e0a: r[2]=rd(m,r[5]+r[0],4); /* mov.l @(r0,r5),r2 */
 goto L8c665e0c;
L8c665e0c: r[0]=rd(m,r[1],4); /* mov.l @r1,r0 */
 goto L8c665e0e;
L8c665e0e: r[0]<<=2; /* shll2 r0 */
 goto L8c665e10;
L8c665e10: r[0]=rd(m,r[5]+r[0],4); /* mov.l @(r0,r5),r0 */
 goto L8c665e12;
L8c665e12: r[3]+=8u; /* add #0x8,r3 */
 goto L8c665e14;
L8c665e14: r[2]+=r[0]; /* add r0,r2 */
 goto L8c665e16;
L8c665e16: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c665e18;
L8c665e18: r[0]<<=2; /* shll2 r0 */
 goto L8c665e1a;
L8c665e1a: r[0]=rd(m,r[5]+r[0],4); /* mov.l @(r0,r5),r0 */
 goto L8c665e1c;
L8c665e1c: r[3]+=4u; /* add #0x4,r3 */
 goto L8c665e1e;
L8c665e1e: r[2]+=r[0]; /* add r0,r2 */
 goto L8c665e20;
L8c665e20: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c665e22;
L8c665e22: r[0]<<=2; /* shll2 r0 */
 goto L8c665e24;
L8c665e24: r[0]=rd(m,r[5]+r[0],4); /* mov.l @(r0,r5),r0 */
 goto L8c665e26;
L8c665e26: r[2]+=r[0]; /* add r0,r2 */
 goto L8c665e28;
L8c665e28: r[0]=rd(m,r[3]+0x4u,4); /* mov.l @(0x4,r3),r0 */
 goto L8c665e2a;
L8c665e2a: r[0]<<=2; /* shll2 r0 */
 goto L8c665e2c;
L8c665e2c: r[0]=rd(m,r[5]+r[0],4); /* mov.l @(r0,r5),r0 */
 goto L8c665e2e;
L8c665e2e: r[2]+=r[0]; /* add r0,r2 */
 goto L8c665e30;
L8c665e30: *macl=r[4]*r[2]; /* mul.l r4,r2 */
 goto L8c665e32;
L8c665e32: r[2]=*macl; /* sts MACL,r2 */
 goto L8c665e34;
L8c665e34: r[4]=r[2]; /* mov r2,r4 */
 goto L8c665e36;
L8c665e36: r[4]<<=2; /* shll2 r4 */
 goto L8c665e38;
L8c665e38: r[0]=r[4];return 0; /* rts */
}
