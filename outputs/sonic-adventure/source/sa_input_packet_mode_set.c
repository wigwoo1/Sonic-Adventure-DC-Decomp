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
/* Static C recovery of retail Tikal helper 8c647de8..8c647e16.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_packet_mode_set(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c647de8;
L8c647de8: r[5]=rd(m,0x8c647e9cu,4); /* mov.l 0x8c647e9c,r5 */
 goto L8c647dea;
L8c647dea: r[3]=1u; /* mov #0x1,r3 */
 goto L8c647dec;
L8c647dec: r[4]&=r[3]; /* and r3,r4 */
 goto L8c647dee;
L8c647dee: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c647df0;
L8c647df0: r[0]=r[4]; /* mov r4,r0 */
 goto L8c647df2;
L8c647df2: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c647df4;
L8c647df4: wr(m,r[2],r[4],4); /* mov.l r4,@r2 */
 goto L8c647df6;
L8c647df6: take=!(*sr&1u);r[6]=rd(m,r[5],4);if(take)goto L8c647e08;goto L8c647dfa; /* bf/s 0x8c647e08 */
 goto L8c647dfa;
L8c647dfa: r[2]=rd(m,0x8c647ea0u,4); /* mov.l 0x8c647ea0,r2 */
 goto L8c647dfc;
L8c647dfc: wr(m,r[6]+0xcu,r[2],4); /* mov.l r2,@(0xc,r6) */
 goto L8c647dfe;
L8c647dfe: r[2]=rd(m,0x8c647ea4u,4); /* mov.l 0x8c647ea4,r2 */
 goto L8c647e00;
L8c647e00: r[1]=rd(m,r[5],4); /* mov.l @r5,r1 */
 goto L8c647e02;
L8c647e02: wr(m,r[1]+0x10u,r[2],4); /* mov.l r2,@(0x10,r1) */
 goto L8c647e04;
L8c647e04: goto L8c647e12; /* bra 0x8c647e12 */
 goto L8c647e08;
L8c647e08: r[1]=rd(m,0x8c647eacu,4); /* mov.l 0x8c647eac,r1 */
 goto L8c647e0a;
L8c647e0a: r[0]=rd(m,0x8c647ea8u,4); /* mov.l 0x8c647ea8,r0 */
 goto L8c647e0c;
L8c647e0c: wr(m,r[6]+0xcu,r[0],4); /* mov.l r0,@(0xc,r6) */
 goto L8c647e0e;
L8c647e0e: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c647e10;
L8c647e10: wr(m,r[2]+0x10u,r[1],4); /* mov.l r1,@(0x10,r2) */
 goto L8c647e12;
L8c647e12: return 0; /* rts */
}
