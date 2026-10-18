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
/* Static C recovery of retail Tikal helper 8c65ff12..8c65ff5a.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_presence_decode(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65ff12;
L8c65ff12: r[0]=sx(m,r[4]+0x7u,1); /* mov.b @(0x7,r4),r0 */
 goto L8c65ff14;
L8c65ff14: r[6]=1u; /* mov #0x1,r6 */
 goto L8c65ff16;
L8c65ff16: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c65ff18;
L8c65ff18: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65ff1a;
L8c65ff1a: take=!(*sr&1u);r[5]=0u;if(take)goto L8c65ff2e;goto L8c65ff1e; /* bf/s 0x8c65ff2e */
 goto L8c65ff1e;
L8c65ff1e: r[0]=sx(m,r[4]+0x8u,1); /* mov.b @(0x8,r4),r0 */
 goto L8c65ff20;
L8c65ff20: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c65ff22;
L8c65ff22: *sr=(*sr&~1u)|(r[0]==0x5u); /* cmp/eq #0x5,r0 */
 goto L8c65ff24;
L8c65ff24: if(!(*sr&1u))goto L8c65ff2a; /* bf 0x8c65ff2a */
 goto L8c65ff26;
L8c65ff26: wr(m,r[4],r[6],1);goto L8c65ff56; /* bra 0x8c65ff56 */
 goto L8c65ff2a;
L8c65ff2a: wr(m,r[4],r[5],1);goto L8c65ff56; /* bra 0x8c65ff56 */
 goto L8c65ff2e;
L8c65ff2e: r[0]=sx(m,r[4]+0x8u,1); /* mov.b @(0x8,r4),r0 */
 goto L8c65ff30;
L8c65ff30: r[1]=sx(m,0x8c65ff4au,2); /* mov.w 0x8c65ff4a,r1 */
 goto L8c65ff32;
L8c65ff32: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c65ff34;
L8c65ff34: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c65ff36;
L8c65ff36: if((*sr&1u))goto L8c65ff40; /* bt 0x8c65ff40 */
 goto L8c65ff38;
L8c65ff38: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c65ff3a;
L8c65ff3a: if((*sr&1u))goto L8c65ff40; /* bt 0x8c65ff40 */
 goto L8c65ff3c;
L8c65ff3c: goto L8c65ff54; /* bra 0x8c65ff54 */
 goto L8c65ff40;
L8c65ff40: wr(m,r[4],r[5],1);goto L8c65ff56; /* bra 0x8c65ff56 */
 goto L8c65ff54;
L8c65ff54: wr(m,r[4],r[6],1); /* mov.b r6,@r4 */
 goto L8c65ff56;
L8c65ff56: r[0]=0u;return 0; /* rts */
}
