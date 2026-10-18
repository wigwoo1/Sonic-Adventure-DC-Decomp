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
/* Static C recovery of retail Tikal helper 8c04c960..8c04c98e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_stage_mode_query(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c04c960;
L8c04c960: r[3]=rd(m,0x8c04ca00u,4); /* mov.l 0x8c04ca00,r3 */
 goto L8c04c962;
L8c04c962: r[2]=14u; /* mov #0xe,r2 */
 goto L8c04c964;
L8c04c964: r[4]=sx(m,r[3],2); /* mov.w @r3,r4 */
 goto L8c04c966;
L8c04c966: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c04c968;
L8c04c968: take=!(*sr&1u);r[5]=2u;if(take)goto L8c04c984;goto L8c04c96c; /* bf/s 0x8c04c984 */
 goto L8c04c96c;
L8c04c96c: r[1]=25u; /* mov #0x19,r1 */
 goto L8c04c96e;
L8c04c96e: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[1]); /* cmp/gt r1,r4 */
 goto L8c04c970;
L8c04c970: if(!(*sr&1u))goto L8c04c984; /* bf 0x8c04c984 */
 goto L8c04c972;
L8c04c972: r[3]=34u; /* mov #0x22,r3 */
 goto L8c04c974;
L8c04c974: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[3]); /* cmp/gt r3,r4 */
 goto L8c04c976;
L8c04c976: take=(*sr&1u);r[6]=1u;if(take)goto L8c04c97e;goto L8c04c97a; /* bt/s 0x8c04c97e */
 goto L8c04c97a;
L8c04c97a: r[4]=r[6];goto L8c04c98a; /* bra 0x8c04c98a */
 goto L8c04c97e;
L8c04c97e: r[2]=38u; /* mov #0x26,r2 */
 goto L8c04c980;
L8c04c980: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c04c982;
L8c04c982: if((*sr&1u))goto L8c04c988; /* bt 0x8c04c988 */
 goto L8c04c984;
L8c04c984: r[4]=r[5];goto L8c04c98a; /* bra 0x8c04c98a */
 goto L8c04c988;
L8c04c988: r[4]=r[6]; /* mov r6,r4 */
 goto L8c04c98a;
L8c04c98a: r[0]=r[4];return 0; /* rts */
}
