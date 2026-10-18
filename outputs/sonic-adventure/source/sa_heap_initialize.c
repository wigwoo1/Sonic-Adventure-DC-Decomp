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
/* Static C recovery of retail Tikal helper 8c098a10..8c098a5c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_heap_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c098a10;
L8c098a10: r[4]=rd(m,0x8c098a68u,4); /* mov.l 0x8c098a68,r4 */
 goto L8c098a12;
L8c098a12: r[1]=0u; /* mov #0x0,r1 */
 goto L8c098a14;
L8c098a14: r[0]=rd(m,0x8c098a70u,4); /* mov.l 0x8c098a70,r0 */
 goto L8c098a16;
L8c098a16: r[5]=4294967295u; /* mov #-0x1,r5 */
 goto L8c098a18;
L8c098a18: r[7]=rd(m,0x8c098a6cu,4); /* mov.l 0x8c098a6c,r7 */
 goto L8c098a1a;
L8c098a1a: r[5]=r[5]&255u; /* extu.b r5,r5 */
 goto L8c098a1c;
L8c098a1c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c098a1e;
L8c098a1e: r[14]=r[4]; /* mov r4,r14 */
 goto L8c098a20;
L8c098a20: wr(m,r[7],r[4],4); /* mov.l r4,@r7 */
 goto L8c098a22;
L8c098a22: r[14]+=r[0]; /* add r0,r14 */
 goto L8c098a24;
L8c098a24: r[6]=r[1]; /* mov r1,r6 */
 goto L8c098a26;
L8c098a26: wr(m,r[4],r[5],1); /* mov.b r5,@r4 */
 goto L8c098a28;
L8c098a28: r[4]+=1u; /* add #0x1,r4 */
 goto L8c098a2a;
L8c098a2a: wr(m,r[4],r[5],1); /* mov.b r5,@r4 */
 goto L8c098a2c;
L8c098a2c: r[6]+=3u; /* add #0x3,r6 */
 goto L8c098a2e;
L8c098a2e: r[4]+=1u; /* add #0x1,r4 */
 goto L8c098a30;
L8c098a30: *sr=(*sr&~1u)|(r[6]>=r[0]); /* cmp/hs r0,r6 */
 goto L8c098a32;
L8c098a32: wr(m,r[4],r[5],1); /* mov.b r5,@r4 */
 goto L8c098a34;
L8c098a34: take=!(*sr&1u);r[4]+=1u;if(take)goto L8c098a26;goto L8c098a38; /* bf/s 0x8c098a26 */
 goto L8c098a38;
L8c098a38: r[2]=rd(m,r[7],4); /* mov.l @r7,r2 */
 goto L8c098a3a;
L8c098a3a: wr(m,r[2],r[14],4); /* mov.l r14,@r2 */
 goto L8c098a3c;
L8c098a3c: r[3]=rd(m,r[7],4); /* mov.l @r7,r3 */
 goto L8c098a3e;
L8c098a3e: wr(m,r[3]+0x4u,r[1],4); /* mov.l r1,@(0x4,r3) */
 goto L8c098a40;
L8c098a40: r[2]=rd(m,r[7],4); /* mov.l @r7,r2 */
 goto L8c098a42;
L8c098a42: wr(m,r[2]+0x8u,r[14],4); /* mov.l r14,@(0x8,r2) */
 goto L8c098a44;
L8c098a44: r[3]=rd(m,r[7],4); /* mov.l @r7,r3 */
 goto L8c098a46;
L8c098a46: wr(m,r[3]+0xcu,r[1],4); /* mov.l r1,@(0xc,r3) */
 goto L8c098a48;
L8c098a48: wr(m,r[14],r[1],4); /* mov.l r1,@r14 */
 goto L8c098a4a;
L8c098a4a: r[2]=rd(m,r[7],4); /* mov.l @r7,r2 */
 goto L8c098a4c;
L8c098a4c: wr(m,r[14]+0x4u,r[2],4); /* mov.l r2,@(0x4,r14) */
 goto L8c098a4e;
L8c098a4e: r[2]=rd(m,0x8c098a74u,4); /* mov.l 0x8c098a74,r2 */
 goto L8c098a50;
L8c098a50: wr(m,r[14]+0x8u,r[1],4); /* mov.l r1,@(0x8,r14) */
 goto L8c098a52;
L8c098a52: r[3]=rd(m,r[7],4); /* mov.l @r7,r3 */
 goto L8c098a54;
L8c098a54: wr(m,r[14]+0xcu,r[3],4); /* mov.l r3,@(0xc,r14) */
 goto L8c098a56;
L8c098a56: wr(m,r[2],r[1],4); /* mov.l r1,@r2 */
 goto L8c098a58;
L8c098a58: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
