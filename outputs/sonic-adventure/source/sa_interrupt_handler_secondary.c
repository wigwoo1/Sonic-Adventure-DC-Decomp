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
/* Static C recovery of retail interrupt handler helper 8c64137c..8c6413a6.
 * Subordinate routines use the supplied service dispatcher. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_interrupt_handler_secondary(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c64137c;
L8c64137c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c64137e;
L8c64137e: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c641380;
L8c641380: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c641382;
L8c641382: r[3]=rd(m,0x8c6413ccu,4); /* mov.l 0x8c6413cc,r3 */
 goto L8c641384;
L8c641384: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c641386;
L8c641386: r[4]=rd(m,r[15],4); /* mov.l @r15,r4 */
 goto L8c641388;
L8c641388: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c64138a;
L8c64138a: r[4]=*sr&1u; /* movt r4 */
 goto L8c64138c;
L8c64138c: r[4]+=4294967295u; /* add #-0x1,r4 */
 goto L8c64138e;
L8c64138e: target=r[3];*pr=0x8c641392u;r[4]=0u-r[4];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c641392;
L8c641392: r[3]=rd(m,0x8c6413c4u,4); /* mov.l 0x8c6413c4,r3 */
 goto L8c641394;
L8c641394: r[5]=2u; /* mov #0x2,r5 */
 goto L8c641396;
L8c641396: r[6]=rd(m,r[15],4); /* mov.l @r15,r6 */
 goto L8c641398;
L8c641398: r[7]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r7 */
 goto L8c64139a;
L8c64139a: *pr=0x8c64139eu;r[4]=rd(m,r[3],4);status=call(ctx,0x8c6412c2u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6412c2 */
 goto L8c64139e;
L8c64139e: r[15]+=8u; /* add #0x8,r15 */
 goto L8c6413a0;
L8c6413a0: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6413a2;
L8c6413a2: r[0]=0u;return 0; /* rts */
}
