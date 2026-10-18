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
/* Retail sound-object query 8c65de56..8c65de7a. */
SA_STAGE_EXPORT int sa_sound_subordinate_pan_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65de56;
L8c65de56: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65de58;
L8c65de58: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c65de5a;
L8c65de5a: if(!(*sr&1u))goto L8c65de6a; /* bf 0x8c65de6a */
 goto L8c65de5c;
L8c65de5c: r[2]=rd(m,0x8c65deb0u,4); /* mov.l 0x8c65deb0,r2 */
 goto L8c65de5e;
L8c65de5e: r[4]=rd(m,0x8c65deacu,4); /* mov.l 0x8c65deac,r4 */
 goto L8c65de60;
L8c65de60: target=r[2];*pr=0x8c65de64u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65de64;
L8c65de64: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65de66;
L8c65de66: r[0]=4294967295u;return 0; /* rts */
L8c65de6a: r[3]=r[4]; /* mov r4,r3 */
 goto L8c65de6c;
L8c65de6c: r[0]=r[5]; /* mov r5,r0 */
 goto L8c65de6e;
L8c65de6e: r[3]+=112u; /* add #0x70,r3 */
 goto L8c65de70;
L8c65de70: r[0]<<=2; /* shll2 r0 */
 goto L8c65de72;
L8c65de72: r[0]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r0 */
 goto L8c65de74;
L8c65de74: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65de76;
L8c65de76: return 0; /* rts */
}
