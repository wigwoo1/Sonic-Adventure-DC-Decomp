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
/* Static C recovery of retail Tikal helper 8c08f210..8c08f26c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_setup(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c08f210;
L8c08f210: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c08f212;
L8c08f212: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c08f214;
L8c08f214: r[3]=rd(m,0x8c08f318u,4); /* mov.l 0x8c08f318,r3 */
 goto L8c08f216;
L8c08f216: target=r[3];*pr=0x8c08f21au;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c08f21a;
L8c08f21a: r[2]=rd(m,0x8c08f31cu,4); /* mov.l 0x8c08f31c,r2 */
 goto L8c08f21c;
L8c08f21c: target=r[2];*pr=0x8c08f220u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c08f220;
L8c08f220: r[3]=rd(m,0x8c08f320u,4); /* mov.l 0x8c08f320,r3 */
 goto L8c08f222;
L8c08f222: target=r[3];*pr=0x8c08f226u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c08f226;
L8c08f226: r[2]=rd(m,0x8c08f324u,4); /* mov.l 0x8c08f324,r2 */
 goto L8c08f228;
L8c08f228: r[6]=0u; /* mov #0x0,r6 */
 goto L8c08f22a;
L8c08f22a: r[5]=r[6]; /* mov r6,r5 */
 goto L8c08f22c;
L8c08f22c: target=r[2];*pr=0x8c08f230u;r[4]=r[6];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c08f230;
L8c08f230: r[3]=rd(m,0x8c08f32cu,4); /* mov.l 0x8c08f32c,r3 */
 goto L8c08f232;
L8c08f232: r[4]=rd(m,0x8c08f328u,4); /* mov.l 0x8c08f328,r4 */
 goto L8c08f234;
L8c08f234: r[5]=rd(m,0x8c08f300u,4); /* mov.l 0x8c08f300,r5 */
 goto L8c08f236;
L8c08f236: target=r[3];*pr=0x8c08f23au;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c08f23a;
L8c08f23a: r[3]=rd(m,0x8c08f330u,4); /* mov.l 0x8c08f330,r3 */
 goto L8c08f23c;
L8c08f23c: r[2]=1u; /* mov #0x1,r2 */
 goto L8c08f23e;
L8c08f23e: r[5]=r[2]; /* mov r2,r5 */
 goto L8c08f240;
L8c08f240: r[4]=r[2]; /* mov r2,r4 */
 goto L8c08f242;
L8c08f242: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c08f244;
L8c08f244: r[1]=0u; /* mov #0x0,r1 */
 goto L8c08f246;
L8c08f246: r[2]=rd(m,0x8c08f338u,4); /* mov.l 0x8c08f338,r2 */
 goto L8c08f248;
L8c08f248: r[0]=rd(m,0x8c08f334u,4); /* mov.l 0x8c08f334,r0 */
 goto L8c08f24a;
L8c08f24a: target=r[2];*pr=0x8c08f24eu;wr(m,r[0],r[1],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c08f24e;
L8c08f24e: r[4]=0u; /* mov #0x0,r4 */
 goto L8c08f250;
L8c08f250: r[3]=rd(m,0x8c08f33cu,4); /* mov.l 0x8c08f33c,r3 */
 goto L8c08f252;
L8c08f252: r[0]=r[4]; /* mov r4,r0 */
 goto L8c08f254;
L8c08f254: wr(m,r[15]+0x3u,r[0],1); /* mov.b r0,@(0x3,r15) */
 goto L8c08f256;
L8c08f256: r[0]=r[4]; /* mov r4,r0 */
 goto L8c08f258;
L8c08f258: wr(m,r[15]+0x2u,r[0],1); /* mov.b r0,@(0x2,r15) */
 goto L8c08f25a;
L8c08f25a: r[0]=r[4]; /* mov r4,r0 */
 goto L8c08f25c;
L8c08f25c: wr(m,r[15]+0x1u,r[0],1); /* mov.b r0,@(0x1,r15) */
 goto L8c08f25e;
L8c08f25e: wr(m,r[15],r[4],1); /* mov.b r4,@r15 */
 goto L8c08f260;
L8c08f260: target=r[3];*pr=0x8c08f264u;r[4]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c08f264;
L8c08f264: r[15]+=4u; /* add #0x4,r15 */
 goto L8c08f266;
L8c08f266: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c08f268;
L8c08f268: return 0; /* rts */
}
