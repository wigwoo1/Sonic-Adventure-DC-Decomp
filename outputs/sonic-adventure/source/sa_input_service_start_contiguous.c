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
/* Static C recovery of retail Tikal helper 8c646304..8c646356.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_service_start_contiguous(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c646304;
L8c646304: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c646306;
L8c646306: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c646308;
L8c646308: r[0]=*sr; /* stc SR,r0 */
 goto L8c64630a;
L8c64630a: r[3]=sx(m,0x8c6463c4u,2); /* mov.w 0x8c6463c4,r3 */
 goto L8c64630c;
L8c64630c: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c64630e;
L8c64630e: r[0]>>=2; /* shlr2 r0 */
 goto L8c646310;
L8c646310: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c646312;
L8c646312: r[0]>>=2; /* shlr2 r0 */
 goto L8c646314;
L8c646314: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c646316;
L8c646316: wr(m,r[15]+0x8u,r[0],4); /* mov.l r0,@(0x8,r15) */
 goto L8c646318;
L8c646318: r[0]=*sr; /* stc SR,r0 */
 goto L8c64631a;
L8c64631a: r[0]&=r[3]; /* and r3,r0 */
 goto L8c64631c;
L8c64631c: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c64631e;
L8c64631e: *sr=r[0]; /* ldc r0,SR */
 goto L8c646320;
L8c646320: *pr=0x8c646324u;status=call(ctx,0x8c6461f0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6461f0 */
 goto L8c646324;
L8c646324: r[7]=rd(m,0x8c6463c8u,4); /* mov.l 0x8c6463c8,r7 */
 goto L8c646326;
L8c646326: r[3]=rd(m,0x8c6463ccu,4); /* mov.l 0x8c6463cc,r3 */
 goto L8c646328;
L8c646328: r[6]=rd(m,r[15],4); /* mov.l @r15,r6 */
 goto L8c64632a;
L8c64632a: r[5]=r[7]; /* mov r7,r5 */
 goto L8c64632c;
L8c64632c: target=r[3];*pr=0x8c646330u;r[4]=rd(m,r[15]+0x4u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c646330;
L8c646330: *pr=0x8c646334u;wr(m,r[15],r[0],4);status=call(ctx,0x8c6461aeu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6461ae */
 goto L8c646334;
L8c646334: r[3]=rd(m,0x8c6463d0u,4); /* mov.l 0x8c6463d0,r3 */
 goto L8c646336;
L8c646336: r[2]=1u; /* mov #0x1,r2 */
 goto L8c646338;
L8c646338: r[1]=*sr; /* stc SR,r1 */
 goto L8c64633a;
L8c64633a: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c64633c;
L8c64633c: r[0]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r0 */
 goto L8c64633e;
L8c64633e: r[2]=sx(m,0x8c6463c4u,2); /* mov.w 0x8c6463c4,r2 */
 goto L8c646340;
L8c646340: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c646342;
L8c646342: r[0]<<=2; /* shll2 r0 */
 goto L8c646344;
L8c646344: r[1]&=r[2]; /* and r2,r1 */
 goto L8c646346;
L8c646346: r[0]<<=2; /* shll2 r0 */
 goto L8c646348;
L8c646348: r[0]|=r[1]; /* or r1,r0 */
 goto L8c64634a;
L8c64634a: *sr=r[0]; /* ldc r0,SR */
 goto L8c64634c;
L8c64634c: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c64634e;
L8c64634e: r[15]+=12u; /* add #0xc,r15 */
 goto L8c646350;
L8c646350: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c646352;
L8c646352: return 0; /* rts */
}

