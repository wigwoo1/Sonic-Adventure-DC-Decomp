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
/* Static C recovery of retail channel control installation 8c10fa60..8c10fa96.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_channel_control_install(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10fa60;
L8c10fa60: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c10fa62;
L8c10fa62: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10fa64;
L8c10fa64: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c10fa66;
L8c10fa66: r[3]=rd(m,0x8c10fa98u,4); /* mov.l 0x8c10fa98,r3 */
 goto L8c10fa68;
L8c10fa68: r[14]=r[4]; /* mov r4,r14 */
 goto L8c10fa6a;
L8c10fa6a: target=r[3];*pr=0x8c10fa6eu;r[5]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10fa6e;
L8c10fa6e: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c10fa70;
L8c10fa70: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c10fa72;
L8c10fa72: take=!(*sr&1u);r[0]=0u;if(take)goto L8c10fa8e;goto L8c10fa76; /* bf/s 0x8c10fa8e */
 goto L8c10fa76;
L8c10fa76: r[3]=rd(m,0x8c10fa9cu,4); /* mov.l 0x8c10fa9c,r3 */
 goto L8c10fa78;
L8c10fa78: r[1]=2u; /* mov #0x2,r1 */
 goto L8c10fa7a;
L8c10fa7a: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10fa7c;
L8c10fa7c: r[4]=rd(m,r[3],4); /* mov.l @r3,r4 */
 goto L8c10fa7e;
L8c10fa7e: r[2]<<=2; /* shll2 r2 */
 goto L8c10fa80;
L8c10fa80: r[2]<<=2; /* shll2 r2 */
 goto L8c10fa82;
L8c10fa82: r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c10fa84;
L8c10fa84: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c10fa86;
L8c10fa86: r[4]+=r[2]; /* add r2,r4 */
 goto L8c10fa88;
L8c10fa88: r[2]=1u; /* mov #0x1,r2 */
 goto L8c10fa8a;
L8c10fa8a: wr(m,r[4]+0x1cu,r[2],4); /* mov.l r2,@(0x1c,r4) */
 goto L8c10fa8c;
L8c10fa8c: wr(m,r[14]+0x18u,r[1],4); /* mov.l r1,@(0x18,r14) */
 goto L8c10fa8e;
L8c10fa8e: r[15]+=4u; /* add #0x4,r15 */
 goto L8c10fa90;
L8c10fa90: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10fa92;
L8c10fa92: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
