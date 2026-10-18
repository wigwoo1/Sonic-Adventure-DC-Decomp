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
/* Static C recovery of retail interrupt handler helper 8c6412c2..8c64133e.
 * Subordinate routines use the supplied service dispatcher. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_interrupt_handler_set(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6412c2;
L8c6412c2: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6412c4;
L8c6412c4: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c6412c6;
L8c6412c6: r[0]=*sr; /* stc SR,r0 */
 goto L8c6412c8;
L8c6412c8: r[3]=sx(m,0x8c641308u,2); /* mov.w 0x8c641308,r3 */
 goto L8c6412ca;
L8c6412ca: r[0]>>=2; /* shlr2 r0 */
 goto L8c6412cc;
L8c6412cc: r[0]>>=2; /* shlr2 r0 */
 goto L8c6412ce;
L8c6412ce: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c6412d0;
L8c6412d0: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c6412d2;
L8c6412d2: r[0]=*sr; /* stc SR,r0 */
 goto L8c6412d4;
L8c6412d4: r[0]&=r[3]; /* and r3,r0 */
 goto L8c6412d6;
L8c6412d6: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c6412d8;
L8c6412d8: *sr=r[0]; /* ldc r0,SR */
 goto L8c6412da;
L8c6412da: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c6412dc;
L8c6412dc: if((*sr&1u))goto L8c641320; /* bt 0x8c641320 */
 goto L8c6412de;
L8c6412de: r[3]=sx(m,0x8c64130au,2); /* mov.w 0x8c64130a,r3 */
 goto L8c6412e0;
L8c6412e0: r[5]<<=2; /* shll2 r5 */
 goto L8c6412e2;
L8c6412e2: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1; /* shll r5 */
 goto L8c6412e4;
L8c6412e4: r[2]=sx(m,0x8c64130au,2); /* mov.w 0x8c64130a,r2 */
 goto L8c6412e6;
L8c6412e6: r[3]+=r[4]; /* add r4,r3 */
 goto L8c6412e8;
L8c6412e8: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c6412ea;
L8c6412ea: r[5]+=r[3]; /* add r3,r5 */
 goto L8c6412ec;
L8c6412ec: r[2]+=r[4]; /* add r4,r2 */
 goto L8c6412ee;
L8c6412ee: wr(m,r[5],r[6],4); /* mov.l r6,@r5 */
 goto L8c6412f0;
L8c6412f0: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c6412f2;
L8c6412f2: r[2]+=r[3]; /* add r3,r2 */
 goto L8c6412f4;
L8c6412f4: wr(m,r[2]+0x4u,r[7],4);goto L8c641324; /* bra 0x8c641324 */
 goto L8c641320;
L8c641320: *pr=0x8c641324u;status=call(ctx,0x8c6411b0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6411b0 */
 goto L8c641324;
L8c641324: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c641326;
L8c641326: r[3]=*sr; /* stc SR,r3 */
 goto L8c641328;
L8c641328: r[2]=sx(m,0x8c6413c0u,2); /* mov.w 0x8c6413c0,r2 */
 goto L8c64132a;
L8c64132a: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c64132c;
L8c64132c: r[0]<<=2; /* shll2 r0 */
 goto L8c64132e;
L8c64132e: r[3]&=r[2]; /* and r2,r3 */
 goto L8c641330;
L8c641330: r[0]<<=2; /* shll2 r0 */
 goto L8c641332;
L8c641332: r[0]|=r[3]; /* or r3,r0 */
 goto L8c641334;
L8c641334: *sr=r[0]; /* ldc r0,SR */
 goto L8c641336;
L8c641336: r[15]+=8u; /* add #0x8,r15 */
 goto L8c641338;
L8c641338: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c64133a;
L8c64133a: return 0; /* rts */
}
