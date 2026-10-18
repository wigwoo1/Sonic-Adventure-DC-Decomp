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
/* Static C recovery of retail Tikal helper 8c65ad8e..8c65adb4.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_critical_exit(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65ad8e;
L8c65ad8e: r[4]=rd(m,0x8c65adb8u,4); /* mov.l 0x8c65adb8,r4 */
 goto L8c65ad90;
L8c65ad90: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c65ad92;
L8c65ad92: r[3]+=4294967295u; /* add #-0x1,r3 */
 goto L8c65ad94;
L8c65ad94: r[2]=r[3]; /* mov r3,r2 */
 goto L8c65ad96;
L8c65ad96: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c65ad98;
L8c65ad98: take=!(*sr&1u);wr(m,r[4],r[3],4);if(take)goto L8c65adb0;goto L8c65ad9c; /* bf/s 0x8c65adb0 */
 goto L8c65ad9c;
L8c65ad9c: r[2]=rd(m,0x8c65adbcu,4); /* mov.l 0x8c65adbc,r2 */
 goto L8c65ad9e;
L8c65ad9e: r[1]=sx(m,0x8c65adb4u,2); /* mov.w 0x8c65adb4,r1 */
 goto L8c65ada0;
L8c65ada0: r[0]=rd(m,r[2],4); /* mov.l @r2,r0 */
 goto L8c65ada2;
L8c65ada2: r[2]=*sr; /* stc SR,r2 */
 goto L8c65ada4;
L8c65ada4: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c65ada6;
L8c65ada6: r[0]<<=2; /* shll2 r0 */
 goto L8c65ada8;
L8c65ada8: r[2]&=r[1]; /* and r1,r2 */
 goto L8c65adaa;
L8c65adaa: r[0]<<=2; /* shll2 r0 */
 goto L8c65adac;
L8c65adac: r[0]|=r[2]; /* or r2,r0 */
 goto L8c65adae;
L8c65adae: *sr=r[0]; /* ldc r0,SR */
 goto L8c65adb0;
L8c65adb0: return 0; /* rts */
}
