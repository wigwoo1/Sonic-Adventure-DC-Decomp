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
/* Static C recovery of retail Tikal helper 8c660518..8c660546.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_request_pack(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c660518;
L8c660518: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66051a;
L8c66051a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66051c;
L8c66051c: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c66051e;
L8c66051e: r[6]<<=16; /* shll16 r6 */
 goto L8c660520;
L8c660520: r[14]=r[15]; /* mov r15,r14 */
 goto L8c660522;
L8c660522: r[7]<<=16; /* shll16 r7 */
 goto L8c660524;
L8c660524: wr(m,r[14],r[5],4); /* mov.l r5,@r14 */
 goto L8c660526;
L8c660526: r[3]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r3 */
 goto L8c660528;
L8c660528: r[6]<<=8; /* shll8 r6 */
 goto L8c66052a;
L8c66052a: r[6]|=r[7]; /* or r7,r6 */
 goto L8c66052c;
L8c66052c: r[6]|=r[3]; /* or r3,r6 */
 goto L8c66052e;
L8c66052e: r[3]=0u; /* mov #0x0,r3 */
 goto L8c660530;
L8c660530: wr(m,r[14]+0x4u,r[6],4); /* mov.l r6,@(0x4,r14) */
 goto L8c660532;
L8c660532: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c660534;
L8c660534: r[7]=2u; /* mov #0x2,r7 */
 goto L8c660536;
L8c660536: r[6]=r[14]; /* mov r14,r6 */
 goto L8c660538;
L8c660538: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c66053a;
L8c66053a: *pr=0x8c66053eu;r[5]=11u;status=call(ctx,0x8c65f810u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65f810 */
 goto L8c66053e;
L8c66053e: r[15]+=16u; /* add #0x10,r15 */
 goto L8c660540;
L8c660540: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c660542;
L8c660542: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
