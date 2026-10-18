#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_cache_invalidate(void *,uint32_t);
int sa_sh4_cache_purge(void *,uint32_t);
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_sh4_sqrt(uint32_t,uint32_t *,uint32_t *);
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
/* Retail audio work destruction65b72c..65b760.
 * Releases a non-null stream and clears four work-state bytes in a critical section.
 * Stream release and critical-section integration require separate verification. */
SA_STAGE_EXPORT int sa_sound_work_destroy(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65b72c;
L8c65b72c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65b72e;
L8c65b72e: r[14]=r[4]; /* mov r4,r14 */
 goto L8c65b730;
L8c65b730: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65b732;
L8c65b732: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65b734;
L8c65b734: if((*sr&1u))goto L8c65b75a; /* bt 0x8c65b75a */
 goto L8c65b736;
L8c65b736: r[2]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r2 */
 goto L8c65b738;
L8c65b738: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c65b73a;
L8c65b73a: if((*sr&1u))goto L8c65b742; /* bt 0x8c65b742 */
 goto L8c65b73c;
L8c65b73c: r[3]=rd(m,0x8c65b824u,4); /* mov.l 0x8c65b824,r3 */
 goto L8c65b73e;
L8c65b73e: target=r[3];*pr=0x8c65b742u;r[4]=rd(m,r[14]+0x4u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65b742;
L8c65b742: r[2]=rd(m,0x8c65b828u,4); /* mov.l 0x8c65b828,r2 */
 goto L8c65b744;
L8c65b744: target=r[2];*pr=0x8c65b748u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65b748;
L8c65b748: r[3]=rd(m,0x8c65b82cu,4); /* mov.l 0x8c65b82c,r3 */
 goto L8c65b74a;
L8c65b74a: r[5]=0u; /* mov #0x0,r5 */
 goto L8c65b74c;
L8c65b74c: r[6]=4u; /* mov #0x4,r6 */
 goto L8c65b74e;
L8c65b74e: target=r[3];*pr=0x8c65b752u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65b752;
L8c65b752: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65b754;
L8c65b754: r[2]=rd(m,0x8c65b830u,4); /* mov.l 0x8c65b830,r2 */
 goto L8c65b756;
L8c65b756: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
L8c65b75a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65b75c;
L8c65b75c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
