#include "sa_draw_motion_stop.h"
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
/* Static C recovery of retail Tikal helper 8c098b0c..8c098b42.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_heap_allocate_zeroed(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c098b0c;
L8c098b0c: sa_motion_publish_stop(ctx,0x8c098b0cu); r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c098b0e;
L8c098b0e: sa_motion_publish_stop(ctx,0x8c098b0eu); r[14]=r[5]; /* mov r5,r14 */
 goto L8c098b10;
L8c098b10: sa_motion_publish_stop(ctx,0x8c098b10u); r[3]=4294967292u; /* mov #-0x4,r3 */
 goto L8c098b12;
L8c098b12: sa_motion_publish_stop(ctx,0x8c098b12u); r[14]+=3u; /* add #0x3,r14 */
 goto L8c098b14;
L8c098b14: sa_motion_publish_stop(ctx,0x8c098b14u); r[14]&=r[3]; /* and r3,r14 */
 goto L8c098b16;
L8c098b16: sa_motion_publish_stop(ctx,0x8c098b16u); *macl=r[4]*r[14]; /* mul.l r4,r14 */
 goto L8c098b18;
L8c098b18: sa_motion_publish_stop(ctx,0x8c098b18u); r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c098b1a;
L8c098b1a: sa_motion_publish_stop(ctx,0x8c098b1au); r[14]=*macl; /* sts MACL,r14 */
 goto L8c098b1c;
L8c098b1c: sa_motion_publish_stop(ctx,0x8c098b1eu); *pr=0x8c098b20u;r[4]=r[14];status=call(ctx,0x8c098a80u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098a80 */
 goto L8c098b20;
L8c098b20: sa_motion_publish_stop(ctx,0x8c098b20u); r[6]=r[0]; /* mov r0,r6 */
 goto L8c098b22;
L8c098b22: sa_motion_publish_stop(ctx,0x8c098b22u); *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c098b24;
L8c098b24: sa_motion_publish_stop(ctx,0x8c098b24u); if((*sr&1u))goto L8c098b3a; /* bt 0x8c098b3a */
 goto L8c098b26;
L8c098b26: sa_motion_publish_stop(ctx,0x8c098b26u); r[4]=r[14]; /* mov r14,r4 */
 goto L8c098b28;
L8c098b28: sa_motion_publish_stop(ctx,0x8c098b28u); r[4]>>=2; /* shlr2 r4 */
 goto L8c098b2a;
L8c098b2a: sa_motion_publish_stop(ctx,0x8c098b2au); *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c098b2c;
L8c098b2c: sa_motion_publish_stop(ctx,0x8c098b2cu); r[7]=r[6]; /* mov r6,r7 */
 goto L8c098b2e;
L8c098b2e: sa_motion_publish_stop(ctx,0x8c098b30u); take=(*sr&1u);r[5]=0u;if(take)goto L8c098b3a;goto L8c098b32; /* bt/s 0x8c098b3a */
 goto L8c098b32;
L8c098b32: sa_motion_publish_stop(ctx,0x8c098b32u); r[4]--;*sr=(*sr&~1u)|(!r[4]); /* dt r4 */
 goto L8c098b34;
L8c098b34: sa_motion_publish_stop(ctx,0x8c098b34u); wr(m,r[7],r[5],4); /* mov.l r5,@r7 */
 goto L8c098b36;
L8c098b36: sa_motion_publish_stop(ctx,0x8c098b38u); take=!(*sr&1u);r[7]+=4u;if(take)goto L8c098b32;goto L8c098b3a; /* bf/s 0x8c098b32 */
 goto L8c098b3a;
L8c098b3a: sa_motion_publish_stop(ctx,0x8c098b3au); *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c098b3c;
L8c098b3c: sa_motion_publish_stop(ctx,0x8c098b3cu); r[0]=r[6]; /* mov r6,r0 */
 goto L8c098b3e;
L8c098b3e: sa_motion_publish_stop(ctx,0x8c098b40u); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
