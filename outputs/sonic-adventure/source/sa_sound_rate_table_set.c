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
/* Static C recovery of retail Tikal helper 8c6693ec..8c669430.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_rate_table_set(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6693ec;
L8c6693ec: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6693ee;
L8c6693ee: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6693f0;
L8c6693f0: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6693f2;
L8c6693f2: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6693f4;
L8c6693f4: r[14]=rd(m,0x8c669434u,4); /* mov.l 0x8c669434,r14 */
 goto L8c6693f6;
L8c6693f6: r[12]=rd(m,0x8c669430u,4); /* mov.l 0x8c669430,r12 */
 goto L8c6693f8;
L8c6693f8: goto L8c669410; /* bra 0x8c669410 */
 goto L8c6693fc;
L8c6693fc: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c6693fe;
L8c6693fe: r[2]=r[6]&65535u; /* extu.w r6,r2 */
 goto L8c669400;
L8c669400: *sr=(*sr&~1u)|(r[3]==r[2]); /* cmp/eq r3,r2 */
 goto L8c669402;
L8c669402: if(!(*sr&1u))goto L8c66940e; /* bf 0x8c66940e */
 goto L8c669404;
L8c669404: r[0]=sx(m,r[14]+0x4u,2); /* mov.w @(0x4,r14),r0 */
 goto L8c669406;
L8c669406: target=r[12];*pr=0x8c66940au;r[6]=r[0];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c66940a;
L8c66940a: r[13]=r[0];goto L8c669418; /* bra 0x8c669418 */
 goto L8c66940e;
L8c66940e: r[14]+=8u; /* add #0x8,r14 */
 goto L8c669410;
L8c669410: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c669412;
L8c669412: *sr=(*sr&~1u)|((int32_t)r[2]>0); /* cmp/pl r2 */
 goto L8c669414;
L8c669414: if((*sr&1u))goto L8c6693fc; /* bt 0x8c6693fc */
 goto L8c669416;
L8c669416: r[13]=rd(m,0x8c669438u,4); /* mov.l 0x8c669438,r13 */
 goto L8c669418;
L8c669418: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c66941a;
L8c66941a: if((*sr&1u))goto L8c669424; /* bt 0x8c669424 */
 goto L8c66941c;
L8c66941c: r[3]=rd(m,0x8c669440u,4); /* mov.l 0x8c669440,r3 */
 goto L8c66941e;
L8c66941e: r[4]=rd(m,0x8c66943cu,4); /* mov.l 0x8c66943c,r4 */
 goto L8c669420;
L8c669420: target=r[3];*pr=0x8c669424u;r[5]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c669424;
L8c669424: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c669426;
L8c669426: r[0]=r[13]; /* mov r13,r0 */
 goto L8c669428;
L8c669428: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66942a;
L8c66942a: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66942c;
L8c66942c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
