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
/* Static C recovery of retail Tikal helper 8c667188..8c6671bc.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_byte_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c667188;
L8c667188: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66718a;
L8c66718a: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c66718c;
L8c66718c: r[3]=rd(m,0x8c6671bcu,4); /* mov.l 0x8c6671bc,r3 */
 goto L8c66718e;
L8c66718e: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]); /* cmp/ge r3,r4 */
 goto L8c667190;
L8c667190: take=(*sr&1u);wr(m,r[15],r[5],4);if(take)goto L8c6671b4;goto L8c667194; /* bt/s 0x8c6671b4 */
 goto L8c667194;
L8c667194: r[1]=3u; /* mov #0x3,r1 */
 goto L8c667196;
L8c667196: r[3]=rd(m,0x8c6671c0u,4); /* mov.l 0x8c6671c0,r3 */
 goto L8c667198;
L8c667198: r[2]=4294967292u; /* mov #-0x4,r2 */
 goto L8c66719a;
L8c66719a: r[5]=r[15]; /* mov r15,r5 */
 goto L8c66719c;
L8c66719c: r[1]&=r[4]; /* and r4,r1 */
 goto L8c66719e;
L8c66719e: wr(m,r[15]+0x4u,r[1],4); /* mov.l r1,@(0x4,r15) */
 goto L8c6671a0;
L8c6671a0: r[4]&=r[2]; /* and r2,r4 */
 goto L8c6671a2;
L8c6671a2: target=r[3];*pr=0x8c6671a6u;r[5]+=8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6671a6;
L8c6671a6: r[1]=r[15]; /* mov r15,r1 */
 goto L8c6671a8;
L8c6671a8: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c6671aa;
L8c6671aa: r[1]+=8u; /* add #0x8,r1 */
 goto L8c6671ac;
L8c6671ac: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c6671ae;
L8c6671ae: r[1]+=r[3]; /* add r3,r1 */
 goto L8c6671b0;
L8c6671b0: r[1]=sx(m,r[1],1); /* mov.b @r1,r1 */
 goto L8c6671b2;
L8c6671b2: wr(m,r[2],r[1],1); /* mov.b r1,@r2 */
 goto L8c6671b4;
L8c6671b4: r[15]+=12u; /* add #0xc,r15 */
 goto L8c6671b6;
L8c6671b6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6671b8;
L8c6671b8: r[0]=0u;return 0; /* rts */
}
