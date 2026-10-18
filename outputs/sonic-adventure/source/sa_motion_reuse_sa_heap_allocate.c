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
/* Static C recovery of retail Tikal helper 8c098a80..8c098b0c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_heap_allocate(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c098a80;
L8c098a80: sa_motion_publish_stop(ctx,0x8c098a80u); *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c098a82;
L8c098a82: sa_motion_publish_stop(ctx,0x8c098a82u); if((*sr&1u))goto L8c098b06; /* bt 0x8c098b06 */
 goto L8c098a84;
L8c098a84: sa_motion_publish_stop(ctx,0x8c098a84u); r[7]=rd(m,0x8c098b60u,4); /* mov.l 0x8c098b60,r7 */
 goto L8c098a86;
L8c098a86: sa_motion_publish_stop(ctx,0x8c098a86u); r[6]=rd(m,r[7],4); /* mov.l @r7,r6 */
 goto L8c098a88;
L8c098a88: sa_motion_publish_stop(ctx,0x8c098a88u); *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c098a8a;
L8c098a8a: sa_motion_publish_stop(ctx,0x8c098a8au); if((*sr&1u))goto L8c098b06; /* bt 0x8c098b06 */
 goto L8c098a8c;
L8c098a8c: sa_motion_publish_stop(ctx,0x8c098a8cu); r[3]=rd(m,r[6]+0x8u,4); /* mov.l @(0x8,r6),r3 */
 goto L8c098a8e;
L8c098a8e: sa_motion_publish_stop(ctx,0x8c098a8eu); *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c098a90;
L8c098a90: sa_motion_publish_stop(ctx,0x8c098a90u); if((*sr&1u))goto L8c098b06; /* bt 0x8c098b06 */
 goto L8c098a92;
L8c098a92: sa_motion_publish_stop(ctx,0x8c098a92u); r[5]=r[4]; /* mov r4,r5 */
 goto L8c098a94;
L8c098a94: sa_motion_publish_stop(ctx,0x8c098a94u); r[4]=r[6]; /* mov r6,r4 */
 goto L8c098a96;
L8c098a96: sa_motion_publish_stop(ctx,0x8c098a96u); r[3]=4294967280u; /* mov #-0x10,r3 */
 goto L8c098a98;
L8c098a98: sa_motion_publish_stop(ctx,0x8c098a98u); r[5]+=31u; /* add #0x1f,r5 */
 goto L8c098a9a;
L8c098a9a: sa_motion_publish_stop(ctx,0x8c098a9au); *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c098a9c;
L8c098a9c: sa_motion_publish_stop(ctx,0x8c098a9cu); r[5]&=r[3]; /* and r3,r5 */
 goto L8c098a9e;
L8c098a9e: sa_motion_publish_stop(ctx,0x8c098aa0u); take=(*sr&1u);r[6]=0u;if(take)goto L8c098b06;goto L8c098aa2; /* bt/s 0x8c098b06 */
 goto L8c098aa2;
L8c098aa2: sa_motion_publish_stop(ctx,0x8c098aa2u); r[1]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r1 */
 goto L8c098aa4;
L8c098aa4: sa_motion_publish_stop(ctx,0x8c098aa4u); *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c098aa6;
L8c098aa6: sa_motion_publish_stop(ctx,0x8c098aa6u); if((*sr&1u))goto L8c098b06; /* bt 0x8c098b06 */
 goto L8c098aa8;
L8c098aa8: sa_motion_publish_stop(ctx,0x8c098aa8u); r[1]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r1 */
 goto L8c098aaa;
L8c098aaa: sa_motion_publish_stop(ctx,0x8c098aaau); r[1]-=r[4]; /* sub r4,r1 */
 goto L8c098aac;
L8c098aac: sa_motion_publish_stop(ctx,0x8c098aacu); *sr=(*sr&~1u)|(r[5]==r[1]); /* cmp/eq r5,r1 */
 goto L8c098aae;
L8c098aae: sa_motion_publish_stop(ctx,0x8c098aaeu); if(!(*sr&1u))goto L8c098ac6; /* bf 0x8c098ac6 */
 goto L8c098ab0;
L8c098ab0: sa_motion_publish_stop(ctx,0x8c098ab0u); r[5]=rd(m,r[4],4); /* mov.l @r4,r5 */
 goto L8c098ab2;
L8c098ab2: sa_motion_publish_stop(ctx,0x8c098ab2u); *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c098ab4;
L8c098ab4: sa_motion_publish_stop(ctx,0x8c098ab6u); take=(*sr&1u);r[1]=rd(m,r[4]+0x4u,4);if(take)goto L8c098aba;goto L8c098ab8; /* bt/s 0x8c098aba */
 goto L8c098ab8;
L8c098ab8: sa_motion_publish_stop(ctx,0x8c098ab8u); wr(m,r[5]+0x4u,r[1],4); /* mov.l r1,@(0x4,r5) */
 goto L8c098aba;
L8c098aba: sa_motion_publish_stop(ctx,0x8c098abau); *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c098abc;
L8c098abc: sa_motion_publish_stop(ctx,0x8c098abcu); if((*sr&1u))goto L8c098ac2; /* bt 0x8c098ac2 */
 goto L8c098abe;
L8c098abe: sa_motion_publish_stop(ctx,0x8c098ac0u); wr(m,r[1],r[5],4);goto L8c098af0; /* bra 0x8c098af0 */
 goto L8c098ac2;
L8c098ac2: sa_motion_publish_stop(ctx,0x8c098ac4u); wr(m,r[7],r[5],4);goto L8c098af0; /* bra 0x8c098af0 */
 goto L8c098ac6;
L8c098ac6: sa_motion_publish_stop(ctx,0x8c098ac6u); *sr=(*sr&~1u)|(r[1]>r[5]); /* cmp/hi r5,r1 */
 goto L8c098ac8;
L8c098ac8: sa_motion_publish_stop(ctx,0x8c098ac8u); if(!(*sr&1u))goto L8c098afa; /* bf 0x8c098afa */
 goto L8c098aca;
L8c098aca: sa_motion_publish_stop(ctx,0x8c098acau); r[1]=rd(m,r[4]+0x4u,4); /* mov.l @(0x4,r4),r1 */
 goto L8c098acc;
L8c098acc: sa_motion_publish_stop(ctx,0x8c098accu); r[5]+=r[4]; /* add r4,r5 */
 goto L8c098ace;
L8c098ace: sa_motion_publish_stop(ctx,0x8c098aceu); r[0]=rd(m,r[4],4); /* mov.l @r4,r0 */
 goto L8c098ad0;
L8c098ad0: sa_motion_publish_stop(ctx,0x8c098ad0u); *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c098ad2;
L8c098ad2: sa_motion_publish_stop(ctx,0x8c098ad2u); wr(m,r[5],r[0],4); /* mov.l r0,@r5 */
 goto L8c098ad4;
L8c098ad4: sa_motion_publish_stop(ctx,0x8c098ad4u); wr(m,r[5]+0x4u,r[1],4); /* mov.l r1,@(0x4,r5) */
 goto L8c098ad6;
L8c098ad6: sa_motion_publish_stop(ctx,0x8c098ad6u); r[3]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r3 */
 goto L8c098ad8;
L8c098ad8: sa_motion_publish_stop(ctx,0x8c098ad8u); wr(m,r[5]+0x8u,r[3],4); /* mov.l r3,@(0x8,r5) */
 goto L8c098ada;
L8c098ada: sa_motion_publish_stop(ctx,0x8c098adau); wr(m,r[5]+0xcu,r[4],4); /* mov.l r4,@(0xc,r5) */
 goto L8c098adc;
L8c098adc: sa_motion_publish_stop(ctx,0x8c098adcu); r[2]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r2 */
 goto L8c098ade;
L8c098ade: sa_motion_publish_stop(ctx,0x8c098adeu); wr(m,r[2]+0xcu,r[5],4); /* mov.l r5,@(0xc,r2) */
 goto L8c098ae0;
L8c098ae0: sa_motion_publish_stop(ctx,0x8c098ae2u); take=(*sr&1u);wr(m,r[4]+0x8u,r[5],4);if(take)goto L8c098ae6;goto L8c098ae4; /* bt/s 0x8c098ae6 */
 goto L8c098ae4;
L8c098ae4: sa_motion_publish_stop(ctx,0x8c098ae4u); wr(m,r[0]+0x4u,r[5],4); /* mov.l r5,@(0x4,r0) */
 goto L8c098ae6;
L8c098ae6: sa_motion_publish_stop(ctx,0x8c098ae6u); *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c098ae8;
L8c098ae8: sa_motion_publish_stop(ctx,0x8c098ae8u); if((*sr&1u))goto L8c098aee; /* bt 0x8c098aee */
 goto L8c098aea;
L8c098aea: sa_motion_publish_stop(ctx,0x8c098aecu); wr(m,r[1],r[5],4);goto L8c098af0; /* bra 0x8c098af0 */
 goto L8c098aee;
L8c098aee: sa_motion_publish_stop(ctx,0x8c098aeeu); wr(m,r[7],r[5],4); /* mov.l r5,@r7 */
 goto L8c098af0;
L8c098af0: sa_motion_publish_stop(ctx,0x8c098af0u); wr(m,r[4],r[6],4); /* mov.l r6,@r4 */
 goto L8c098af2;
L8c098af2: sa_motion_publish_stop(ctx,0x8c098af2u); wr(m,r[4]+0x4u,r[6],4); /* mov.l r6,@(0x4,r4) */
 goto L8c098af4;
L8c098af4: sa_motion_publish_stop(ctx,0x8c098af4u); r[4]+=16u; /* add #0x10,r4 */
 goto L8c098af6;
L8c098af6: sa_motion_publish_stop(ctx,0x8c098af8u); r[0]=r[4];return 0; /* rts */
L8c098afa: sa_motion_publish_stop(ctx,0x8c098afau); r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c098afc;
L8c098afc: sa_motion_publish_stop(ctx,0x8c098afcu); *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c098afe;
L8c098afe: sa_motion_publish_stop(ctx,0x8c098afeu); if((*sr&1u))goto L8c098b06; /* bt 0x8c098b06 */
 goto L8c098b00;
L8c098b00: sa_motion_publish_stop(ctx,0x8c098b00u); r[3]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r3 */
 goto L8c098b02;
L8c098b02: sa_motion_publish_stop(ctx,0x8c098b02u); *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c098b04;
L8c098b04: sa_motion_publish_stop(ctx,0x8c098b04u); if(!(*sr&1u))goto L8c098aa8; /* bf 0x8c098aa8 */
 goto L8c098b06;
L8c098b06: sa_motion_publish_stop(ctx,0x8c098b06u); r[0]=0u; /* mov #0x0,r0 */
 goto L8c098b08;
L8c098b08: sa_motion_publish_stop(ctx,0x8c098b0au); return 0; /* rts */
}
