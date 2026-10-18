/* Sonic Adventure USA Rev A main executable recovered transfer routine. */
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
/* Static C recovery of retail Tikal helper 8c10f9bc..8c10f9ec.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_transfer_channel_release(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10f9bc;
L8c10f9bc: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10f9be;
L8c10f9be: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c10f9c0;
L8c10f9c0: r[3]=rd(m,0x8c10f9ecu,4); /* mov.l 0x8c10f9ec,r3 */
 goto L8c10f9c2;
L8c10f9c2: wr(m,r[15]+0x4u,r[4],4); /* mov.l r4,@(0x4,r15) */
 goto L8c10f9c4;
L8c10f9c4: target=r[3];*pr=0x8c10f9c8u;r[5]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10f9c8;
L8c10f9c8: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c10f9ca;
L8c10f9ca: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10f9cc;
L8c10f9cc: if(!(*sr&1u))goto L8c10f9e4; /* bf 0x8c10f9e4 */
 goto L8c10f9ce;
L8c10f9ce: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c10f9d0;
L8c10f9d0: r[3]=rd(m,0x8c10f9f0u,4); /* mov.l 0x8c10f9f0,r3 */
 goto L8c10f9d2;
L8c10f9d2: r[1]=rd(m,r[2],4); /* mov.l @r2,r1 */
 goto L8c10f9d4;
L8c10f9d4: r[2]=0u; /* mov #0x0,r2 */
 goto L8c10f9d6;
L8c10f9d6: r[4]=rd(m,r[3],4); /* mov.l @r3,r4 */
 goto L8c10f9d8;
L8c10f9d8: r[1]<<=2; /* shll2 r1 */
 goto L8c10f9da;
L8c10f9da: r[1]<<=2; /* shll2 r1 */
 goto L8c10f9dc;
L8c10f9dc: r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c10f9de;
L8c10f9de: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c10f9e0;
L8c10f9e0: r[4]+=r[1]; /* add r1,r4 */
 goto L8c10f9e2;
L8c10f9e2: wr(m,r[4]+0x1cu,r[2],4); /* mov.l r2,@(0x1c,r4) */
 goto L8c10f9e4;
L8c10f9e4: r[15]+=8u; /* add #0x8,r15 */
 goto L8c10f9e6;
L8c10f9e6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10f9e8;
L8c10f9e8: r[0]=0u;return 0; /* rts */
}
