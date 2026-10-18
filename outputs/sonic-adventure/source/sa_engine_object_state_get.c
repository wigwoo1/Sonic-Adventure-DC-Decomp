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
/* Static C recovery of the USA Rev A main executable. External calls use the supplied dispatcher. */
SA_STAGE_EXPORT int sa_engine_object_state_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a568;
L8c65a568: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a56a;
L8c65a56a: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c65a56c;
L8c65a56c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65a56e;
L8c65a56e: take=!(*sr&1u);r[14]=0u;if(take)goto L8c65a578;goto L8c65a572; /* bf/s 0x8c65a578 */
 goto L8c65a572;
L8c65a572: r[14]=rd(m,0x8c65a59cu,4); /* mov.l 0x8c65a59c,r14 */
 goto L8c65a574;
L8c65a574: goto L8c65a586; /* bra 0x8c65a586 */
 goto L8c65a578;
L8c65a578: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c65a57a;
L8c65a57a: if(!(*sr&1u))goto L8c65a582; /* bf 0x8c65a582 */
 goto L8c65a57c;
L8c65a57c: r[14]=rd(m,0x8c65a5a0u,4); /* mov.l 0x8c65a5a0,r14 */
 goto L8c65a57e;
L8c65a57e: goto L8c65a586; /* bra 0x8c65a586 */
 goto L8c65a582;
L8c65a582: r[2]=rd(m,r[4]+0x4u,4); /* mov.l @(0x4,r4),r2 */
 goto L8c65a584;
L8c65a584: wr(m,r[5],r[2],4); /* mov.l r2,@r5 */
 goto L8c65a586;
L8c65a586: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65a588;
L8c65a588: if((*sr&1u))goto L8c65a592; /* bt 0x8c65a592 */
 goto L8c65a58a;
L8c65a58a: r[2]=rd(m,0x8c65a5a8u,4); /* mov.l 0x8c65a5a8,r2 */
 goto L8c65a58c;
L8c65a58c: r[4]=rd(m,0x8c65a5a4u,4); /* mov.l 0x8c65a5a4,r4 */
 goto L8c65a58e;
L8c65a58e: target=r[2];*pr=0x8c65a592u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65a592;
L8c65a592: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65a594;
L8c65a594: r[0]=r[14]; /* mov r14,r0 */
 goto L8c65a596;
L8c65a596: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
