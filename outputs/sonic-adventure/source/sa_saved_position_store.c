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
/* Static C recovery of retail Tikal helper 8c064ef6..8c064f40.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_saved_position_store(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c064ef6;
L8c064ef6: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c064ef8;
L8c064ef8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c064efa;
L8c064efa: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c064efc;
L8c064efc: r[14]=rd(m,0x8c064fe0u,4); /* mov.l 0x8c064fe0,r14 */
 goto L8c064efe;
L8c064efe: r[3]=rd(m,0x8c064ffcu,4); /* mov.l 0x8c064ffc,r3 */
 goto L8c064f00;
L8c064f00: wr(m,r[15]+0x4u,r[4],4); /* mov.l r4,@(0x4,r15) */
 goto L8c064f02;
L8c064f02: r[6]=r[14]; /* mov r14,r6 */
 goto L8c064f04;
L8c064f04: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c064f06;
L8c064f06: r[5]=r[14]; /* mov r14,r5 */
 goto L8c064f08;
L8c064f08: r[4]=r[14]; /* mov r14,r4 */
 goto L8c064f0a;
L8c064f0a: r[6]+=4u; /* add #0x4,r6 */
 goto L8c064f0c;
L8c064f0c: r[5]+=3u; /* add #0x3,r5 */
 goto L8c064f0e;
L8c064f0e: target=r[3];*pr=0x8c064f12u;r[4]+=2u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c064f12;
L8c064f12: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c064f14;
L8c064f14: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c064f16;
L8c064f16: r[1]=rd(m,r[2]+0x4u,4); /* mov.l @(0x4,r2),r1 */
 goto L8c064f18;
L8c064f18: wr(m,r[14]+0xcu,r[3],4); /* mov.l r3,@(0xc,r14) */
 goto L8c064f1a;
L8c064f1a: wr(m,r[14]+0x10u,r[1],4); /* mov.l r1,@(0x10,r14) */
 goto L8c064f1c;
L8c064f1c: r[1]=1u; /* mov #0x1,r1 */
 goto L8c064f1e;
L8c064f1e: r[3]=rd(m,r[2]+0x8u,4); /* mov.l @(0x8,r2),r3 */
 goto L8c064f20;
L8c064f20: wr(m,r[14]+0x14u,r[3],4); /* mov.l r3,@(0x14,r14) */
 goto L8c064f22;
L8c064f22: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c064f24;
L8c064f24: r[3]=rd(m,r[0],4); /* mov.l @r0,r3 */
 goto L8c064f26;
L8c064f26: r[2]=rd(m,r[0]+0x4u,4); /* mov.l @(0x4,r0),r2 */
 goto L8c064f28;
L8c064f28: wr(m,r[14]+0x18u,r[3],4); /* mov.l r3,@(0x18,r14) */
 goto L8c064f2a;
L8c064f2a: wr(m,r[14]+0x1cu,r[2],4); /* mov.l r2,@(0x1c,r14) */
 goto L8c064f2c;
L8c064f2c: r[3]=rd(m,r[0]+0x8u,4); /* mov.l @(0x8,r0),r3 */
 goto L8c064f2e;
L8c064f2e: wr(m,r[14]+0x20u,r[3],4); /* mov.l r3,@(0x20,r14) */
 goto L8c064f30;
L8c064f30: r[3]=rd(m,0x8c064fe4u,4); /* mov.l 0x8c064fe4,r3 */
 goto L8c064f32;
L8c064f32: target=r[3];*pr=0x8c064f36u;wr(m,r[14]+0x8u,r[1],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c064f36;
L8c064f36: wr(m,r[14],r[0],2); /* mov.w r0,@r14 */
 goto L8c064f38;
L8c064f38: r[15]+=8u; /* add #0x8,r15 */
 goto L8c064f3a;
L8c064f3a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c064f3c;
L8c064f3c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
