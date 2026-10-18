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
/* Static C recovery of retail Tikal helper 8c647cc4..8c647ce8.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_channel_request_set(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c647cc4;
L8c647cc4: r[4]<<=2; /* shll2 r4 */
 goto L8c647cc6;
L8c647cc6: r[3]=rd(m,0x8c647d10u,4); /* mov.l 0x8c647d10,r3 */
 goto L8c647cc8;
L8c647cc8: r[4]<<=2; /* shll2 r4 */
 goto L8c647cca;
L8c647cca: r[4]<<=2; /* shll2 r4 */
 goto L8c647ccc;
L8c647ccc: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c647cce;
L8c647cce: r[4]+=r[3]; /* add r3,r4 */
 goto L8c647cd0;
L8c647cd0: wr(m,r[4]+0x14u,r[5],4); /* mov.l r5,@(0x14,r4) */
 goto L8c647cd2;
L8c647cd2: wr(m,r[4]+0x18u,r[6],4); /* mov.l r6,@(0x18,r4) */
 goto L8c647cd4;
L8c647cd4: wr(m,r[4]+0x1cu,r[7],4); /* mov.l r7,@(0x1c,r4) */
 goto L8c647cd6;
L8c647cd6: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c647cd8;
L8c647cd8: wr(m,r[4]+0x20u,r[2],4); /* mov.l r2,@(0x20,r4) */
 goto L8c647cda;
L8c647cda: r[1]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r1 */
 goto L8c647cdc;
L8c647cdc: wr(m,r[4]+0x24u,r[1],4); /* mov.l r1,@(0x24,r4) */
 goto L8c647cde;
L8c647cde: r[1]=1u; /* mov #0x1,r1 */
 goto L8c647ce0;
L8c647ce0: r[2]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r2 */
 goto L8c647ce2;
L8c647ce2: wr(m,r[4]+0x28u,r[2],4); /* mov.l r2,@(0x28,r4) */
 goto L8c647ce4;
L8c647ce4: wr(m,r[4]+0x4u,r[1],4);return 0; /* rts */
}
