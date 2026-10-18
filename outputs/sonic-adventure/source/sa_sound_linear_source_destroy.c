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
/* Retail linear-source lifecycle 8c65e150..8c65e16e. */
SA_STAGE_EXPORT int sa_sound_linear_source_destroy(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65e150;
L8c65e150: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65e152;
L8c65e152: r[14]=r[4]; /* mov r4,r14 */
 goto L8c65e154;
L8c65e154: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65e156;
L8c65e156: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65e158;
L8c65e158: if((*sr&1u))goto L8c65e168; /* bt 0x8c65e168 */
 goto L8c65e15a;
L8c65e15a: r[2]=rd(m,0x8c65e1c0u,4); /* mov.l 0x8c65e1c0,r2 */
 goto L8c65e15c;
L8c65e15c: r[5]=0u; /* mov #0x0,r5 */
 goto L8c65e15e;
L8c65e15e: r[6]=36u; /* mov #0x24,r6 */
 goto L8c65e160;
L8c65e160: target=r[2];*pr=0x8c65e164u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65e164;
L8c65e164: r[3]=0u; /* mov #0x0,r3 */
 goto L8c65e166;
L8c65e166: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c65e168;
L8c65e168: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e16a;
L8c65e16a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
