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
/* Static C recovery of retail Tikal helper 8c098876..8c0988a4.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_task_delete_list(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c098876;
L8c098876: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c098878;
L8c098878: r[0]=r[4]; /* mov r4,r0 */
 goto L8c09887a;
L8c09887a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c09887c;
L8c09887c: *sr=(*sr&~1u)|(r[0]==0x8u); /* cmp/eq #0x8,r0 */
 goto L8c09887e;
L8c09887e: if(!(*sr&1u))goto L8c098882; /* bf 0x8c098882 */
 goto L8c098880;
L8c098880: r[4]=7u; /* mov #0x7,r4 */
 goto L8c098882;
L8c098882: r[0]=rd(m,0x8c098940u,4); /* mov.l 0x8c098940,r0 */
 goto L8c098884;
L8c098884: r[4]<<=2; /* shll2 r4 */
 goto L8c098886;
L8c098886: r[4]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r4 */
 goto L8c098888;
L8c098888: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c09888a;
L8c09888a: if((*sr&1u))goto L8c09889e; /* bt 0x8c09889e */
 goto L8c09888c;
L8c09888c: r[5]=rd(m,r[4]+0x10u,4); /* mov.l @(0x10,r4),r5 */
 goto L8c09888e;
L8c09888e: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c098890;
L8c098890: take=(*sr&1u);r[14]=rd(m,r[4],4);if(take)goto L8c098898;goto L8c098894; /* bt/s 0x8c098898 */
 goto L8c098894;
L8c098894: *pr=0x8c098898u;status=call(ctx,0x8c09865cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c09865c */
 goto L8c098898;
L8c098898: r[4]=r[14]; /* mov r14,r4 */
 goto L8c09889a;
L8c09889a: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c09889c;
L8c09889c: if(!(*sr&1u))goto L8c09888c; /* bf 0x8c09888c */
 goto L8c09889e;
L8c09889e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c0988a0;
L8c0988a0: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
