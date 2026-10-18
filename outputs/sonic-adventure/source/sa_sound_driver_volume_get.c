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
/* Static C recovery of retail Tikal helper 8c6680ea..8c668112.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_volume_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6680ea;
L8c6680ea: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6680ec;
L8c6680ec: r[15]+=4294967260u; /* add #-0x24,r15 */
 goto L8c6680ee;
L8c6680ee: wr(m,r[15]+0x4u,r[4],4); /* mov.l r4,@(0x4,r15) */
 goto L8c6680f0;
L8c6680f0: r[6]=r[15]; /* mov r15,r6 */
 goto L8c6680f2;
L8c6680f2: r[3]=rd(m,0x8c6681d4u,4); /* mov.l 0x8c6681d4,r3 */
 goto L8c6680f4;
L8c6680f4: r[6]+=8u; /* add #0x8,r6 */
 goto L8c6680f6;
L8c6680f6: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c6680f8;
L8c6680f8: r[5]=0u; /* mov #0x0,r5 */
 goto L8c6680fa;
L8c6680fa: r[4]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r4 */
 goto L8c6680fc;
L8c6680fc: target=r[3];*pr=0x8c668100u;r[4]=rd(m,r[4]+0x28u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668100;
L8c668100: r[4]=r[0]; /* mov r0,r4 */
 goto L8c668102;
L8c668102: r[0]=sx(m,r[15]+0xau,1); /* mov.b @(0xa,r15),r0 */
 goto L8c668104;
L8c668104: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c668106;
L8c668106: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto L8c668108;
L8c668108: wr(m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c66810a;
L8c66810a: r[15]+=36u; /* add #0x24,r15 */
 goto L8c66810c;
L8c66810c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66810e;
L8c66810e: r[0]=r[4];return 0; /* rts */
}
