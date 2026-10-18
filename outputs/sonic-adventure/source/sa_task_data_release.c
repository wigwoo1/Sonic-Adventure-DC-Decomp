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
/* Retail task payload release 8c098998..8c0989c6. Cleanup and memory release remain external services. */
SA_STAGE_EXPORT int sa_task_data_release(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c098998;
L8c098998: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c09899a;
L8c09899a: r[14]=r[4]; /* mov r4,r14 */
 goto L8c09899c;
L8c09899c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c09899e;
L8c09899e: r[3]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r3 */
 goto L8c0989a0;
L8c0989a0: r[2]=rd(m,r[3]+0x38u,4); /* mov.l @(0x38,r3),r2 */
 goto L8c0989a2;
L8c0989a2: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c0989a4;
L8c0989a4: if((*sr&1u))goto L8c0989ac; /* bt 0x8c0989ac */
 goto L8c0989a6;
L8c0989a6: r[3]=rd(m,0x8c098a60u,4); /* mov.l 0x8c098a60,r3 */
 goto L8c0989a8;
L8c0989a8: target=r[3];*pr=0x8c0989acu;r[4]=rd(m,r[14]+0x20u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c0989ac;
L8c0989ac: r[2]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r2 */
 goto L8c0989ae;
L8c0989ae: r[3]=rd(m,r[2]+0x3cu,4); /* mov.l @(0x3c,r2),r3 */
 goto L8c0989b0;
L8c0989b0: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c0989b2;
L8c0989b2: if((*sr&1u))goto L8c0989ba; /* bt 0x8c0989ba */
 goto L8c0989b4;
L8c0989b4: r[3]=rd(m,0x8c098a64u,4); /* mov.l 0x8c098a64,r3 */
 goto L8c0989b6;
L8c0989b6: target=r[3];*pr=0x8c0989bau;r[4]=rd(m,r[14]+0x20u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c0989ba;
L8c0989ba: *pr=0x8c0989beu;r[4]=rd(m,r[14]+0x20u,4);status=call(ctx,0x8c098b42u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098b42 */
 goto L8c0989be;
L8c0989be: r[3]=0u; /* mov #0x0,r3 */
 goto L8c0989c0;
L8c0989c0: wr(m,r[14]+0x20u,r[3],4); /* mov.l r3,@(0x20,r14) */
 goto L8c0989c2;
L8c0989c2: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c0989c4;
L8c0989c4: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
