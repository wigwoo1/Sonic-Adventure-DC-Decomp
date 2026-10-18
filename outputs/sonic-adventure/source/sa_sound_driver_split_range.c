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
/* Static C recovery of retail Tikal helper 8c668af4..8c668b20.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_split_range(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c668af4;
L8c668af4: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c668af6;
L8c668af6: r[3]=rd(m,0x8c668b20u,4); /* mov.l 0x8c668b20,r3 */
 goto L8c668af8;
L8c668af8: r[2]=r[4]; /* mov r4,r2 */
 goto L8c668afa;
L8c668afa: r[1]=r[6]; /* mov r6,r1 */
 goto L8c668afc;
L8c668afc: target=r[3];*pr=0x8c668b00u;r[0]=8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668b00;
L8c668b00: r[2]=rd(m,r[6]+0x4u,4); /* mov.l @(0x4,r6),r2 */
 goto L8c668b02;
L8c668b02: wr(m,r[7]+0x4u,r[2],4); /* mov.l r2,@(0x4,r7) */
 goto L8c668b04;
L8c668b04: r[3]=rd(m,r[6]+0x4u,4); /* mov.l @(0x4,r6),r3 */
 goto L8c668b06;
L8c668b06: *sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[5]); /* cmp/gt r5,r3 */
 goto L8c668b08;
L8c668b08: if(!(*sr&1u))goto L8c668b0c; /* bf 0x8c668b0c */
 goto L8c668b0a;
L8c668b0a: wr(m,r[6]+0x4u,r[5],4); /* mov.l r5,@(0x4,r6) */
 goto L8c668b0c;
L8c668b0c: r[2]=rd(m,r[6],4); /* mov.l @r6,r2 */
 goto L8c668b0e;
L8c668b0e: r[3]=rd(m,r[6]+0x4u,4); /* mov.l @(0x4,r6),r3 */
 goto L8c668b10;
L8c668b10: r[2]+=r[3]; /* add r3,r2 */
 goto L8c668b12;
L8c668b12: wr(m,r[7],r[2],4); /* mov.l r2,@r7 */
 goto L8c668b14;
L8c668b14: r[1]=rd(m,r[7]+0x4u,4); /* mov.l @(0x4,r7),r1 */
 goto L8c668b16;
L8c668b16: r[3]=rd(m,r[6]+0x4u,4); /* mov.l @(0x4,r6),r3 */
 goto L8c668b18;
L8c668b18: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c668b1a;
L8c668b1a: r[1]-=r[3]; /* sub r3,r1 */
 goto L8c668b1c;
L8c668b1c: wr(m,r[7]+0x4u,r[1],4);return 0; /* rts */
}
