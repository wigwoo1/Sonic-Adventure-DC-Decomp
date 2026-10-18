#include "fpu_cleanup_transfer.h"
#include "input_io.h"
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,const struct sa_input_io *io,uint32_t a,unsigned n){if((a&0x1fffffffu)<0x0c000000u||(a&0x1fffffffu)>=0x0d000000u)return io->read(io->user,a,n);uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(uint8_t *m,const struct sa_input_io *io,uint32_t a,unsigned n){uint32_t v=rd(m,io,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(uint8_t *m,const struct sa_input_io *io,uint32_t a,uint32_t v,unsigned n){if((a&0x1fffffffu)<0x0c000000u||(a&0x1fffffffu)>=0x0d000000u){io->write(io->user,a,v,n);return;}for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|((a==b)||(!(a&0x7fffffffu)&&!(b&0x7fffffffu)));return 0;
}
/* Static C recovery of retail Tikal helper 8c650aa0..8c650b10.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_graphics_state_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,const struct sa_input_io *io,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c650aa0;
L8c650aa0: r[15]-=4;wr(m,io,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c650aa2;
L8c650aa2: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c650aa4;
L8c650aa4: r[5]=rd(m,io,0x8c650b24u,4); /* mov.l 0x8c650b24,r5 */
 goto L8c650aa6;
L8c650aa6: r[3]=rd(m,io,r[5],4); /* mov.l @r5,r3 */
 goto L8c650aa8;
L8c650aa8: r[2]=rd(m,io,0x8c650b28u,4); /* mov.l 0x8c650b28,r2 */
 goto L8c650aaa;
L8c650aaa: r[4]=0u; /* mov #0x0,r4 */
 goto L8c650aac;
L8c650aac: wr(m,io,r[2],r[4],4); /* mov.l r4,@r2 */
 goto L8c650aae;
L8c650aae: r[3]=rd(m,io,0x8c650b2cu,4); /* mov.l 0x8c650b2c,r3 */
 goto L8c650ab0;
L8c650ab0: wr(m,io,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c650ab2;
L8c650ab2: r[1]=rd(m,io,0x8c650b30u,4); /* mov.l 0x8c650b30,r1 */
 goto L8c650ab4;
L8c650ab4: wr(m,io,r[1],r[4],4); /* mov.l r4,@r1 */
 goto L8c650ab6;
L8c650ab6: r[2]=sx(m,io,0x8c650b20u,2); /* mov.w 0x8c650b20,r2 */
 goto L8c650ab8;
L8c650ab8: wr(m,io,r[5],r[2],4); /* mov.l r2,@r5 */
 goto L8c650aba;
L8c650aba: r[2]=rd(m,io,0x8c650b38u,4); /* mov.l 0x8c650b38,r2 */
 goto L8c650abc;
L8c650abc: r[0]=rd(m,io,0x8c650b34u,4); /* mov.l 0x8c650b34,r0 */
 goto L8c650abe;
L8c650abe: wr(m,io,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c650ac0;
L8c650ac0: r[3]=rd(m,io,0x8c650b3cu,4); /* mov.l 0x8c650b3c,r3 */
 goto L8c650ac2;
L8c650ac2: wr(m,io,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c650ac4;
L8c650ac4: r[1]=rd(m,io,0x8c650b40u,4); /* mov.l 0x8c650b40,r1 */
 goto L8c650ac6;
L8c650ac6: r[5]=16u; /* mov #0x10,r5 */
 goto L8c650ac8;
L8c650ac8: wr(m,io,r[1],r[5],4); /* mov.l r5,@r1 */
 goto L8c650aca;
L8c650aca: r[2]=rd(m,io,0x8c650b44u,4); /* mov.l 0x8c650b44,r2 */
 goto L8c650acc;
L8c650acc: wr(m,io,r[2],r[5],4); /* mov.l r5,@r2 */
 goto L8c650ace;
L8c650ace: r[0]=rd(m,io,0x8c650b54u,4); /* mov.l 0x8c650b54,r0 */
 goto L8c650ad0;
L8c650ad0: r[1]=7u; /* mov #0x7,r1 */
 goto L8c650ad2;
L8c650ad2: r[5]=rd(m,io,0x8c650b48u,4); /* mov.l 0x8c650b48,r5 */
 goto L8c650ad4;
L8c650ad4: r[2]=1u; /* mov #0x1,r2 */
 goto L8c650ad6;
L8c650ad6: r[13]=rd(m,io,0x8c650b5cu,4); /* mov.l 0x8c650b5c,r13 */
 goto L8c650ad8;
L8c650ad8: r[7]=8u; /* mov #0x8,r7 */
 goto L8c650ada;
L8c650ada: wr(m,io,r[5],r[4],4); /* mov.l r4,@r5 */
 goto L8c650adc;
L8c650adc: wr(m,io,r[5]+0x4u,r[4],4); /* mov.l r4,@(0x4,r5) */
 goto L8c650ade;
L8c650ade: r[5]=rd(m,io,0x8c650b4cu,4); /* mov.l 0x8c650b4c,r5 */
 goto L8c650ae0;
L8c650ae0: r[6]=sx(m,io,0x8c650b22u,2); /* mov.w 0x8c650b22,r6 */
 goto L8c650ae2;
L8c650ae2: r[3]=rd(m,io,0x8c650b50u,4); /* mov.l 0x8c650b50,r3 */
 goto L8c650ae4;
L8c650ae4: wr(m,io,r[5],r[4],4); /* mov.l r4,@r5 */
 goto L8c650ae6;
L8c650ae6: wr(m,io,r[5]+0x4u,r[4],4); /* mov.l r4,@(0x4,r5) */
 goto L8c650ae8;
L8c650ae8: r[5]=r[4]; /* mov r4,r5 */
 goto L8c650aea;
L8c650aea: wr(m,io,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c650aec;
L8c650aec: wr(m,io,r[0],r[1],4); /* mov.l r1,@r0 */
 goto L8c650aee;
L8c650aee: r[1]=rd(m,io,0x8c650b58u,4); /* mov.l 0x8c650b58,r1 */
 goto L8c650af0;
L8c650af0: wr(m,io,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c650af2;
L8c650af2: r[1]=r[13]; /* mov r13,r1 */
 goto L8c650af4;
L8c650af4: r[1]+=r[6]; /* add r6,r1 */
 goto L8c650af6;
L8c650af6: wr(m,io,r[15],r[13],4); /* mov.l r13,@r15 */
 goto L8c650af8;
L8c650af8: r[2]=r[1]; /* mov r1,r2 */
 goto L8c650afa;
L8c650afa: r[3]=r[13]; /* mov r13,r3 */
 goto L8c650afc;
L8c650afc: r[2]+=r[5]; /* add r5,r2 */
 goto L8c650afe;
L8c650afe: r[3]+=r[5]; /* add r5,r3 */
 goto L8c650b00;
L8c650b00: r[5]+=32u; /* add #0x20,r5 */
 goto L8c650b02;
L8c650b02: wr(m,io,r[3]+0x18u,r[4],4); /* mov.l r4,@(0x18,r3) */
 goto L8c650b04;
L8c650b04: *sr=(*sr&~1u)|(r[5]>=r[6]); /* cmp/hs r6,r5 */
 goto L8c650b06;
L8c650b06: take=!(*sr&1u);wr(m,io,r[2]+0x18u,r[7],4);if(take)goto L8c650af8;goto L8c650b0a; /* bf/s 0x8c650af8 */
 goto L8c650b0a;
L8c650b0a: r[15]+=4u; /* add #0x4,r15 */
 goto L8c650b0c;
L8c650b0c: r[13]=rd(m,io,r[15],4);r[15]+=4;return 0; /* rts */
}
