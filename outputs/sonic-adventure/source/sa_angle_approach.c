#include "fpu_cleanup_transfer.h"
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
/* Static C recovery of retail Tikal helper 8c055d20..8c055d4e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_angle_approach(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c055d20;
L8c055d20: r[5]=r[5]&65535u; /* extu.w r5,r5 */
 goto L8c055d22;
L8c055d22: r[7]=r[5]; /* mov r5,r7 */
 goto L8c055d24;
L8c055d24: r[4]=r[4]&65535u; /* extu.w r4,r4 */
 goto L8c055d26;
L8c055d26: r[7]-=r[4]; /* sub r4,r7 */
 goto L8c055d28;
L8c055d28: r[0]=(uint32_t)(int32_t)(int16_t)r[7]; /* exts.w r7,r0 */
 goto L8c055d2a;
L8c055d2a: *sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[6]); /* cmp/gt r6,r0 */
 goto L8c055d2c;
L8c055d2c: if((*sr&1u))goto L8c055d38; /* bt 0x8c055d38 */
 goto L8c055d2e;
L8c055d2e: r[2]=0u-r[6]; /* neg r6,r2 */
 goto L8c055d30;
L8c055d30: *sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[2]); /* cmp/ge r2,r0 */
 goto L8c055d32;
L8c055d32: if(!(*sr&1u))goto L8c055d38; /* bf 0x8c055d38 */
 goto L8c055d34;
L8c055d34: r[0]=r[5];return 0; /* rts */
L8c055d38: r[2]=rd(m,0x8c055e20u,4); /* mov.l 0x8c055e20,r2 */
 goto L8c055d3a;
L8c055d3a: *sr=(*sr&~1u)|!(r[0]&r[2]); /* tst r0,r2 */
 goto L8c055d3c;
L8c055d3c: if((*sr&1u))goto L8c055d44; /* bt 0x8c055d44 */
 goto L8c055d3e;
L8c055d3e: r[5]=r[4]; /* mov r4,r5 */
 goto L8c055d40;
L8c055d40: r[5]-=r[6];goto L8c055d48; /* bra 0x8c055d48 */
 goto L8c055d44;
L8c055d44: r[5]=r[4]; /* mov r4,r5 */
 goto L8c055d46;
L8c055d46: r[5]+=r[6]; /* add r6,r5 */
 goto L8c055d48;
L8c055d48: r[0]=r[5]&65535u; /* extu.w r5,r0 */
 goto L8c055d4a;
L8c055d4a: return 0; /* rts */
}
