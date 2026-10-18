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
SA_STAGE_EXPORT int sa_sound_slot_validate(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c669194;
L8c669194: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c669196;
L8c669196: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c669198;
L8c669198: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66919a;
L8c66919a: take=!(*sr&1u);r[14]=0u;if(take)goto L8c6691a4;goto L8c66919e; /* bf/s 0x8c6691a4 */
 goto L8c66919e;
L8c66919e: r[14]=rd(m,0x8c6691c0u,4); /* mov.l 0x8c6691c0,r14 */
 goto L8c6691a0;
L8c6691a0: goto L8c6691ac; /* bra 0x8c6691ac */
 goto L8c6691a4;
L8c6691a4: r[0]=rd(m,r[4]+0x4u,4); /* mov.l @(0x4,r4),r0 */
 goto L8c6691a6;
L8c6691a6: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c6691a8;
L8c6691a8: if((*sr&1u))goto L8c6691ac; /* bt 0x8c6691ac */
 goto L8c6691aa;
L8c6691aa: r[14]=rd(m,0x8c6691c4u,4); /* mov.l 0x8c6691c4,r14 */
 goto L8c6691ac;
L8c6691ac: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c6691ae;
L8c6691ae: if((*sr&1u))goto L8c6691b8; /* bt 0x8c6691b8 */
 goto L8c6691b0;
L8c6691b0: r[3]=rd(m,0x8c6691ccu,4); /* mov.l 0x8c6691cc,r3 */
 goto L8c6691b2;
L8c6691b2: r[4]=rd(m,0x8c6691c8u,4); /* mov.l 0x8c6691c8,r4 */
 goto L8c6691b4;
L8c6691b4: target=r[3];*pr=0x8c6691b8u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6691b8;
L8c6691b8: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6691ba;
L8c6691ba: r[0]=r[14]; /* mov r14,r0 */
 goto L8c6691bc;
L8c6691bc: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
