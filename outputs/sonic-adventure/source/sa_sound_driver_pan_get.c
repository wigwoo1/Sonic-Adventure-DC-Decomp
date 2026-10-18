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
/* Static C recovery of retail Tikal helper 8c668112..8c668146.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_pan_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c668112;
L8c668112: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c668114;
L8c668114: r[15]+=4294967260u; /* add #-0x24,r15 */
 goto L8c668116;
L8c668116: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c668118;
L8c668118: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c66811a;
L8c66811a: r[6]=r[15]; /* mov r15,r6 */
 goto L8c66811c;
L8c66811c: r[4]=rd(m,r[15],4); /* mov.l @r15,r4 */
 goto L8c66811e;
L8c66811e: r[6]+=8u; /* add #0x8,r6 */
 goto L8c668120;
L8c668120: r[3]=rd(m,0x8c6681d4u,4); /* mov.l 0x8c6681d4,r3 */
 goto L8c668122;
L8c668122: target=r[3];*pr=0x8c668126u;r[4]=rd(m,r[4]+0x28u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668126;
L8c668126: r[4]=r[0]; /* mov r0,r4 */
 goto L8c668128;
L8c668128: r[0]=sx(m,r[15]+0xbu,1); /* mov.b @(0xb,r15),r0 */
 goto L8c66812a;
L8c66812a: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c66812c;
L8c66812c: r[3]=r[0]; /* mov r0,r3 */
 goto L8c66812e;
L8c66812e: r[0]<<=2; /* shll2 r0 */
 goto L8c668130;
L8c668130: r[0]<<=2; /* shll2 r0 */
 goto L8c668132;
L8c668132: r[0]-=r[3]; /* sub r3,r0 */
 goto L8c668134;
L8c668134: r[3]=rd(m,0x8c6681d8u,4); /* mov.l 0x8c6681d8,r3 */
 goto L8c668136;
L8c668136: r[1]=r[0]; /* mov r0,r1 */
 goto L8c668138;
L8c668138: target=r[3];*pr=0x8c66813cu;r[0]=127u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66813c;
L8c66813c: wr(m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c66813e;
L8c66813e: r[15]+=36u; /* add #0x24,r15 */
 goto L8c668140;
L8c668140: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c668142;
L8c668142: r[0]=r[4];return 0; /* rts */
}
