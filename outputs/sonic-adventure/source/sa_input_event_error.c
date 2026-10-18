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
/* Static C recovery of retail Tikal helper 8c64850a..8c648534.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_event_error(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c64850a;
L8c64850a: r[1]=sx(m,0x8c6485aeu,2); /* mov.w 0x8c6485ae,r1 */
 goto L8c64850c;
L8c64850c: r[3]=rd(m,0x8c6485c8u,4); /* mov.l 0x8c6485c8,r3 */
 goto L8c64850e;
L8c64850e: *macl=r[1]*r[4]; /* mul.l r1,r4 */
 goto L8c648510;
L8c648510: r[1]=*macl; /* sts MACL,r1 */
 goto L8c648512;
L8c648512: r[1]+=r[3]; /* add r3,r1 */
 goto L8c648514;
L8c648514: r[0]=sx(m,r[1]+0x2u,1); /* mov.b @(0x2,r1),r0 */
 goto L8c648516;
L8c648516: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c648518;
L8c648518: if(!(*sr&1u))goto L8c648520; /* bf 0x8c648520 */
 goto L8c64851a;
L8c64851a: r[0]=sx(m,0x8c6485b0u,2); /* mov.w 0x8c6485b0,r0 */
 goto L8c64851c;
L8c64851c: return 0; /* rts */
L8c648520: r[4]=0u; /* mov #0x0,r4 */
 goto L8c648522;
L8c648522: r[0]=r[4]; /* mov r4,r0 */
 goto L8c648524;
L8c648524: wr(m,r[1]+0x9u,r[0],1); /* mov.b r0,@(0x9,r1) */
 goto L8c648526;
L8c648526: wr(m,r[1]+0x8u,r[0],1); /* mov.b r0,@(0x8,r1) */
 goto L8c648528;
L8c648528: wr(m,r[1]+0xau,r[0],1); /* mov.b r0,@(0xa,r1) */
 goto L8c64852a;
L8c64852a: wr(m,r[1]+0xbu,r[0],1); /* mov.b r0,@(0xb,r1) */
 goto L8c64852c;
L8c64852c: wr(m,r[1]+0xcu,r[0],1); /* mov.b r0,@(0xc,r1) */
 goto L8c64852e;
L8c64852e: wr(m,r[1]+0x4u,r[0],4); /* mov.l r0,@(0x4,r1) */
 goto L8c648530;
L8c648530: return 0; /* rts */
}
