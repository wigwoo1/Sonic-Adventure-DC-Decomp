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
/* Retail linear-source descriptor operation 8c65e242..8c65e282. */
SA_STAGE_EXPORT int sa_sound_linear_source_commit(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65e242;
L8c65e242: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65e244;
L8c65e244: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65e246;
L8c65e246: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c65e248;
L8c65e248: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c65e24a;
L8c65e24a: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c65e24c;
L8c65e24c: *pr=0x8c65e250u;r[14]=r[4];status=call(ctx,0x8c65e08cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65e08c */
 goto L8c65e250;
L8c65e250: wr(m,r[15]+0x8u,r[0],4); /* mov.l r0,@(0x8,r15) */
 goto L8c65e252;
L8c65e252: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65e254;
L8c65e254: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e256;
L8c65e256: if((*sr&1u))goto L8c65e276; /* bt 0x8c65e276 */
 goto L8c65e258;
L8c65e258: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c65e25a;
L8c65e25a: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c65e25c;
L8c65e25c: if((*sr&1u))goto L8c65e276; /* bt 0x8c65e276 */
 goto L8c65e25e;
L8c65e25e: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c65e260;
L8c65e260: r[4]=0u; /* mov #0x0,r4 */
 goto L8c65e262;
L8c65e262: wr(m,r[3]+0x4u,r[4],4); /* mov.l r4,@(0x4,r3) */
 goto L8c65e264;
L8c65e264: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c65e266;
L8c65e266: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c65e268;
L8c65e268: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65e26a;
L8c65e26a: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e26c;
L8c65e26c: if((*sr&1u))goto L8c65e276; /* bt 0x8c65e276 */
 goto L8c65e26e;
L8c65e26e: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65e270;
L8c65e270: r[5]=4294967293u; /* mov #-0x3,r5 */
 goto L8c65e272;
L8c65e272: target=r[3];*pr=0x8c65e276u;r[4]=rd(m,r[14]+0x20u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65e276;
L8c65e276: r[5]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r5 */
 goto L8c65e278;
L8c65e278: r[4]=r[14]; /* mov r14,r4 */
 goto L8c65e27a;
L8c65e27a: r[15]+=12u; /* add #0xc,r15 */
 goto L8c65e27c;
L8c65e27c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e27e;
L8c65e27e: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c65e0a4u,m,f,sr,r,pr); /* bra 0x8c65e0a4 */
}
