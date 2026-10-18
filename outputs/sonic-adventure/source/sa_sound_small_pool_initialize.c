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
/* Retail eight-entry sound slot pool helper 8c66ab9c..8c66abc2.
 * Entries contain occupancy word and identifiers15..8. */
SA_STAGE_EXPORT int sa_sound_small_pool_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66ab9c;
L8c66ab9c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66ab9e;
L8c66ab9e: r[4]=rd(m,0x8c66abf4u,4); /* mov.l 0x8c66abf4,r4 */
 goto L8c66aba0;
L8c66aba0: r[6]=64u; /* mov #0x40,r6 */
 goto L8c66aba2;
L8c66aba2: r[3]=rd(m,0x8c66abf8u,4); /* mov.l 0x8c66abf8,r3 */
 goto L8c66aba4;
L8c66aba4: target=r[3];*pr=0x8c66aba8u;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66aba8;
L8c66aba8: r[6]=rd(m,0x8c66abf4u,4); /* mov.l 0x8c66abf4,r6 */
 goto L8c66abaa;
L8c66abaa: r[5]=15u; /* mov #0xf,r5 */
 goto L8c66abac;
L8c66abac: r[7]=r[6]; /* mov r6,r7 */
 goto L8c66abae;
L8c66abae: r[7]+=64u; /* add #0x40,r7 */
 goto L8c66abb0;
L8c66abb0: r[4]=r[6]; /* mov r6,r4 */
 goto L8c66abb2;
L8c66abb2: wr(m,r[4]+0x4u,r[5],4); /* mov.l r5,@(0x4,r4) */
 goto L8c66abb4;
L8c66abb4: r[4]+=8u; /* add #0x8,r4 */
 goto L8c66abb6;
L8c66abb6: *sr=(*sr&~1u)|(r[4]>=r[7]); /* cmp/hs r7,r4 */
 goto L8c66abb8;
L8c66abb8: take=!(*sr&1u);r[5]+=4294967295u;if(take)goto L8c66abb2;goto L8c66abbc; /* bf/s 0x8c66abb2 */
 goto L8c66abbc;
L8c66abbc: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66abbe;
L8c66abbe: return 0; /* rts */
}
