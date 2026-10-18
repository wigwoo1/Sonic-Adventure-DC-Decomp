#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_cache_invalidate(void *,uint32_t);
int sa_sh4_cache_purge(void *,uint32_t);
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
/* Static C recovery of retail Tikal helper 8c66837c..8c6683be.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_pan(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66837c;
L8c66837c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66837e;
L8c66837e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c668380;
L8c668380: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c668382;
L8c668382: r[1]=r[6]; /* mov r6,r1 */
 goto L8c668384;
L8c668384: r[1]<<=2; /* shll2 r1 */
 goto L8c668386;
L8c668386: r[1]<<=2; /* shll2 r1 */
 goto L8c668388;
L8c668388: r[2]=rd(m,0x8c668414u,4); /* mov.l 0x8c668414,r2 */
 goto L8c66838a;
L8c66838a: r[1]<<=2; /* shll2 r1 */
 goto L8c66838c;
L8c66838c: r[3]=r[6]; /* mov r6,r3 */
 goto L8c66838e;
L8c66838e: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c668390;
L8c668390: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c668392;
L8c668392: r[1]-=r[3]; /* sub r3,r1 */
 goto L8c668394;
L8c668394: target=r[2];*pr=0x8c668398u;r[0]=15u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c668398;
L8c668398: r[3]=rd(m,0x8c668418u,4); /* mov.l 0x8c668418,r3 */
 goto L8c66839a;
L8c66839a: r[6]=r[0]; /* mov r0,r6 */
 goto L8c66839c;
L8c66839c: wr(m,r[15]+0x4u,r[0],1); /* mov.b r0,@(0x4,r15) */
 goto L8c66839e;
L8c66839e: r[7]=0u; /* mov #0x0,r7 */
 goto L8c6683a0;
L8c6683a0: r[4]=rd(m,r[15],4); /* mov.l @r15,r4 */
 goto L8c6683a2;
L8c6683a2: target=r[3];*pr=0x8c6683a6u;r[4]=rd(m,r[4]+0x28u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6683a6;
L8c6683a6: r[14]=r[0]; /* mov r0,r14 */
 goto L8c6683a8;
L8c6683a8: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c6683aa;
L8c6683aa: if((*sr&1u))goto L8c6683b4; /* bt 0x8c6683b4 */
 goto L8c6683ac;
L8c6683ac: r[3]=rd(m,0x8c668400u,4); /* mov.l 0x8c668400,r3 */
 goto L8c6683ae;
L8c6683ae: r[4]=rd(m,0x8c66841cu,4); /* mov.l 0x8c66841c,r4 */
 goto L8c6683b0;
L8c6683b0: target=r[3];*pr=0x8c6683b4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6683b4;
L8c6683b4: r[0]=r[14]; /* mov r14,r0 */
 goto L8c6683b6;
L8c6683b6: r[15]+=8u; /* add #0x8,r15 */
 goto L8c6683b8;
L8c6683b8: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6683ba;
L8c6683ba: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
