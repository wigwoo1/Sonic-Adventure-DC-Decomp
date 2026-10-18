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
/* Static C recovery of the USA Rev A main executable. External calls use the supplied service dispatcher. */
SA_STAGE_EXPORT int sa_engine_object_callback_dispatch(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a518;
L8c65a518: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a51a;
L8c65a51a: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c65a51c;
L8c65a51c: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65a51e;
L8c65a51e: r[13]=0u; /* mov #0x0,r13 */
 goto L8c65a520;
L8c65a520: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65a522;
L8c65a522: take=!(*sr&1u);r[14]=r[4];if(take)goto L8c65a52c;goto L8c65a526; /* bf/s 0x8c65a52c */
 goto L8c65a526;
L8c65a526: r[13]=rd(m,0x8c65a55cu,4); /* mov.l 0x8c65a55c,r13 */
 goto L8c65a528;
L8c65a528: goto L8c65a544; /* bra 0x8c65a544 */
 goto L8c65a52c;
L8c65a52c: r[3]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r3 */
 goto L8c65a52e;
L8c65a52e: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65a530;
L8c65a530: if((*sr&1u))goto L8c65a544; /* bt 0x8c65a544 */
 goto L8c65a532;
L8c65a532: r[0]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r0 */
 goto L8c65a534;
L8c65a534: *sr=(*sr&~1u)|(r[0]==0xffffffffu); /* cmp/eq #-0x1,r0 */
 goto L8c65a536;
L8c65a536: if(!(*sr&1u))goto L8c65a53e; /* bf 0x8c65a53e */
 goto L8c65a538;
L8c65a538: r[2]=1u; /* mov #0x1,r2 */
 goto L8c65a53a;
L8c65a53a: wr(m,r[14]+0x18u,r[2],4);goto L8c65a544; /* bra 0x8c65a544 */
 goto L8c65a53e;
L8c65a53e: r[1]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r1 */
 goto L8c65a540;
L8c65a540: target=r[1];*pr=0x8c65a544u;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c65a544;
L8c65a544: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c65a546;
L8c65a546: if((*sr&1u))goto L8c65a550; /* bt 0x8c65a550 */
 goto L8c65a548;
L8c65a548: r[2]=rd(m,0x8c65a564u,4); /* mov.l 0x8c65a564,r2 */
 goto L8c65a54a;
L8c65a54a: r[4]=rd(m,0x8c65a560u,4); /* mov.l 0x8c65a560,r4 */
 goto L8c65a54c;
L8c65a54c: target=r[2];*pr=0x8c65a550u;r[5]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65a550;
L8c65a550: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65a552;
L8c65a552: r[0]=r[13]; /* mov r13,r0 */
 goto L8c65a554;
L8c65a554: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65a556;
L8c65a556: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
