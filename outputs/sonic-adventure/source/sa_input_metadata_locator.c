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
/* Static C recovery of retail Tikal helper 8c646b42..8c646b72.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_metadata_locator(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c646b42;
L8c646b42: r[7]=r[4]; /* mov r4,r7 */
 goto L8c646b44;
L8c646b44: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c646b46;
L8c646b46: r[6]=rd(m,0x8c646c3cu,4); /* mov.l 0x8c646c3c,r6 */
 goto L8c646b48;
L8c646b48: r[0]=32u; /* mov #0x20,r0 */
 goto L8c646b4a;
L8c646b4a: r[12]=rd(m,r[4],4);r[4]+=4; /* mov.l @r4+,r12 */
 goto L8c646b4c;
L8c646b4c: r[7]+=4u; /* add #0x4,r7 */
 goto L8c646b4e;
L8c646b4e: r[1]=0u; /* mov #0x0,r1 */
 goto L8c646b50;
L8c646b50: r[3]=r[6]; /* mov r6,r3 */
 goto L8c646b52;
L8c646b52: *sr=(*sr&~1u)|!(r[12]&r[3]); /* tst r12,r3 */
 goto L8c646b54;
L8c646b54: if((*sr&1u))goto L8c646b64; /* bt 0x8c646b64 */
 goto L8c646b56;
L8c646b56: *sr=(*sr&~1u)|(r[6]==r[5]); /* cmp/eq r6,r5 */
 goto L8c646b58;
L8c646b58: if(!(*sr&1u))goto L8c646b60; /* bf 0x8c646b60 */
 goto L8c646b5a;
L8c646b5a: r[0]=r[7]; /* mov r7,r0 */
 goto L8c646b5c;
L8c646b5c: r[12]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c646b60: r[4]+=4u; /* add #0x4,r4 */
 goto L8c646b62;
L8c646b62: r[7]=r[4]; /* mov r4,r7 */
 goto L8c646b64;
L8c646b64: r[1]+=1u; /* add #0x1,r1 */
 goto L8c646b66;
L8c646b66: *sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1; /* shlr r6 */
 goto L8c646b68;
L8c646b68: *sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[0]); /* cmp/ge r0,r1 */
 goto L8c646b6a;
L8c646b6a: if(!(*sr&1u))goto L8c646b50; /* bf 0x8c646b50 */
 goto L8c646b6c;
L8c646b6c: r[0]=0u; /* mov #0x0,r0 */
 goto L8c646b6e;
L8c646b6e: r[12]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
