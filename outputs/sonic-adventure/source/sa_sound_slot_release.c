#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
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
/* Retail sound slot release/validation. */
SA_STAGE_EXPORT int sa_sound_slot_release(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66b888;
L8c66b888: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66b88a;
L8c66b88a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66b88c;
L8c66b88c: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c66b88e;
L8c66b88e: r[3]=rd(m,0x8c66b8b8u,4); /* mov.l 0x8c66b8b8,r3 */
 goto L8c66b890;
L8c66b890: target=r[3];*pr=0x8c66b894u;wr(m,r[15],r[4],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66b894;
L8c66b894: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c66b896;
L8c66b896: take=!(*sr&1u);r[14]=r[0];if(take)goto L8c66b8a0;goto L8c66b89a; /* bf/s 0x8c66b8a0 */
 goto L8c66b89a;
L8c66b89a: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c66b89c;
L8c66b89c: r[3]=0u; /* mov #0x0,r3 */
 goto L8c66b89e;
L8c66b89e: wr(m,r[2]+0x4u,r[3],4); /* mov.l r3,@(0x4,r2) */
 goto L8c66b8a0;
L8c66b8a0: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66b8a2;
L8c66b8a2: if((*sr&1u))goto L8c66b8ac; /* bt 0x8c66b8ac */
 goto L8c66b8a4;
L8c66b8a4: r[3]=rd(m,0x8c66b8c0u,4); /* mov.l 0x8c66b8c0,r3 */
 goto L8c66b8a6;
L8c66b8a6: r[4]=rd(m,0x8c66b8bcu,4); /* mov.l 0x8c66b8bc,r4 */
 goto L8c66b8a8;
L8c66b8a8: target=r[3];*pr=0x8c66b8acu;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66b8ac;
L8c66b8ac: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66b8ae;
L8c66b8ae: r[15]+=4u; /* add #0x4,r15 */
 goto L8c66b8b0;
L8c66b8b0: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66b8b2;
L8c66b8b2: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
