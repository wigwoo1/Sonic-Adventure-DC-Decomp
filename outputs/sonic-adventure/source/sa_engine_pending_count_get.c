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
SA_STAGE_EXPORT int sa_engine_pending_count_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66abfc;
L8c66abfc: r[3]=rd(m,0x8c66ac2cu,4); /* mov.l 0x8c66ac2c,r3 */
 goto L8c66abfe;
L8c66abfe: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66ac00;
L8c66ac00: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66ac02;
L8c66ac02: r[5]=rd(m,r[3],4); /* mov.l @r3,r5 */
 goto L8c66ac04;
L8c66ac04: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c66ac06;
L8c66ac06: take=!(*sr&1u);r[14]=0u;if(take)goto L8c66ac10;goto L8c66ac0a; /* bf/s 0x8c66ac10 */
 goto L8c66ac0a;
L8c66ac0a: r[14]=rd(m,0x8c66ac30u,4); /* mov.l 0x8c66ac30,r14 */
 goto L8c66ac0c;
L8c66ac0c: goto L8c66ac16; /* bra 0x8c66ac16 */
 goto L8c66ac10;
L8c66ac10: r[0]=sx(m,0x8c66ac2au,2); /* mov.w 0x8c66ac2a,r0 */
 goto L8c66ac12;
L8c66ac12: r[3]=rd(m,r[5]+r[0],4); /* mov.l @(r0,r5),r3 */
 goto L8c66ac14;
L8c66ac14: wr(m,r[4],r[3],4); /* mov.l r3,@r4 */
 goto L8c66ac16;
L8c66ac16: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66ac18;
L8c66ac18: if((*sr&1u))goto L8c66ac22; /* bt 0x8c66ac22 */
 goto L8c66ac1a;
L8c66ac1a: r[3]=rd(m,0x8c66ac38u,4); /* mov.l 0x8c66ac38,r3 */
 goto L8c66ac1c;
L8c66ac1c: r[4]=rd(m,0x8c66ac34u,4); /* mov.l 0x8c66ac34,r4 */
 goto L8c66ac1e;
L8c66ac1e: target=r[3];*pr=0x8c66ac22u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66ac22;
L8c66ac22: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66ac24;
L8c66ac24: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66ac26;
L8c66ac26: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
