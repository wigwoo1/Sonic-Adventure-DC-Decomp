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
/* Static C recovery of retail Tikal helper 8c646356..8c6463ac.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_service_start_explicit(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c646356;
L8c646356: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c646358;
L8c646358: r[15]+=4294967276u; /* add #-0x14,r15 */
 goto L8c64635a;
L8c64635a: r[0]=*sr; /* stc SR,r0 */
 goto L8c64635c;
L8c64635c: r[3]=sx(m,0x8c6463c4u,2); /* mov.w 0x8c6463c4,r3 */
 goto L8c64635e;
L8c64635e: wr(m,r[15]+0x8u,r[4],4); /* mov.l r4,@(0x8,r15) */
 goto L8c646360;
L8c646360: r[0]>>=2; /* shlr2 r0 */
 goto L8c646362;
L8c646362: wr(m,r[15]+0xcu,r[5],4); /* mov.l r5,@(0xc,r15) */
 goto L8c646364;
L8c646364: r[0]>>=2; /* shlr2 r0 */
 goto L8c646366;
L8c646366: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c646368;
L8c646368: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c64636a;
L8c64636a: wr(m,r[15],r[7],4); /* mov.l r7,@r15 */
 goto L8c64636c;
L8c64636c: wr(m,r[15]+0x10u,r[0],4); /* mov.l r0,@(0x10,r15) */
 goto L8c64636e;
L8c64636e: r[0]=*sr; /* stc SR,r0 */
 goto L8c646370;
L8c646370: r[0]&=r[3]; /* and r3,r0 */
 goto L8c646372;
L8c646372: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c646374;
L8c646374: *sr=r[0]; /* ldc r0,SR */
 goto L8c646376;
L8c646376: *pr=0x8c64637au;status=call(ctx,0x8c6461f0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6461f0 */
 goto L8c64637a;
L8c64637a: r[5]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r5 */
 goto L8c64637c;
L8c64637c: r[3]=rd(m,0x8c6463d4u,4); /* mov.l 0x8c6463d4,r3 */
 goto L8c64637e;
L8c64637e: r[7]=rd(m,r[15],4); /* mov.l @r15,r7 */
 goto L8c646380;
L8c646380: r[6]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r6 */
 goto L8c646382;
L8c646382: target=r[3];*pr=0x8c646386u;r[4]=rd(m,r[15]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c646386;
L8c646386: *pr=0x8c64638au;wr(m,r[15],r[0],4);status=call(ctx,0x8c6461aeu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6461ae */
 goto L8c64638a;
L8c64638a: r[3]=rd(m,0x8c6463d0u,4); /* mov.l 0x8c6463d0,r3 */
 goto L8c64638c;
L8c64638c: r[2]=1u; /* mov #0x1,r2 */
 goto L8c64638e;
L8c64638e: r[1]=*sr; /* stc SR,r1 */
 goto L8c646390;
L8c646390: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c646392;
L8c646392: r[0]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r0 */
 goto L8c646394;
L8c646394: r[2]=sx(m,0x8c6463c4u,2); /* mov.w 0x8c6463c4,r2 */
 goto L8c646396;
L8c646396: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c646398;
L8c646398: r[0]<<=2; /* shll2 r0 */
 goto L8c64639a;
L8c64639a: r[1]&=r[2]; /* and r2,r1 */
 goto L8c64639c;
L8c64639c: r[0]<<=2; /* shll2 r0 */
 goto L8c64639e;
L8c64639e: r[0]|=r[1]; /* or r1,r0 */
 goto L8c6463a0;
L8c6463a0: *sr=r[0]; /* ldc r0,SR */
 goto L8c6463a2;
L8c6463a2: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c6463a4;
L8c6463a4: r[15]+=20u; /* add #0x14,r15 */
 goto L8c6463a6;
L8c6463a6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6463a8;
L8c6463a8: return 0; /* rts */
}
