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
/* Static C recovery of retail Tikal helper 8c5ff9c0..8c5ff9e6.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_volume_word_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c5ff9c0;
L8c5ff9c0: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c5ff9c2;
L8c5ff9c2: r[2]=sx(m,r[4],1); /* mov.b @r4,r2 */
 goto L8c5ff9c4;
L8c5ff9c4: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c5ff9c6;
L8c5ff9c6: r[0]<<=8; /* shll8 r0 */
 goto L8c5ff9c8;
L8c5ff9c8: r[3]=r[0]; /* mov r0,r3 */
 goto L8c5ff9ca;
L8c5ff9ca: r[0]=sx(m,r[4]+0x2u,1); /* mov.b @(0x2,r4),r0 */
 goto L8c5ff9cc;
L8c5ff9cc: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c5ff9ce;
L8c5ff9ce: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c5ff9d0;
L8c5ff9d0: r[0]<<=16; /* shll16 r0 */
 goto L8c5ff9d2;
L8c5ff9d2: r[3]|=r[0]; /* or r0,r3 */
 goto L8c5ff9d4;
L8c5ff9d4: r[0]=sx(m,r[4]+0x3u,1); /* mov.b @(0x3,r4),r0 */
 goto L8c5ff9d6;
L8c5ff9d6: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c5ff9d8;
L8c5ff9d8: r[0]<<=16; /* shll16 r0 */
 goto L8c5ff9da;
L8c5ff9da: r[0]<<=8; /* shll8 r0 */
 goto L8c5ff9dc;
L8c5ff9dc: r[3]|=r[0]; /* or r0,r3 */
 goto L8c5ff9de;
L8c5ff9de: r[4]=r[3]; /* mov r3,r4 */
 goto L8c5ff9e0;
L8c5ff9e0: r[4]|=r[2]; /* or r2,r4 */
 goto L8c5ff9e2;
L8c5ff9e2: r[0]=r[4];return 0; /* rts */
}
