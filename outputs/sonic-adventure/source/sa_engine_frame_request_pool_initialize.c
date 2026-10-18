/* Sonic Adventure USA Rev A recovered frame request/layout setup. */
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
/* Static C recovery of retail Tikal helper 8c6669c4..8c666a46.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_frame_request_pool_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6669c4;
L8c6669c4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6669c6;
L8c6669c6: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6669c8;
L8c6669c8: r[13]=0u; /* mov #0x0,r13 */
 goto L8c6669ca;
L8c6669ca: r[14]=rd(m,0x8c666a48u,4); /* mov.l 0x8c666a48,r14 */
 goto L8c6669cc;
L8c6669cc: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6669ce;
L8c6669ce: r[12]=r[5]; /* mov r5,r12 */
 goto L8c6669d0;
L8c6669d0: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c6669d2;
L8c6669d2: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6669d4;
L8c6669d4: r[5]=rd(m,r[14],4); /* mov.l @r14,r5 */
 goto L8c6669d6;
L8c6669d6: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c6669d8;
L8c6669d8: take=!(*sr&1u);r[11]=r[13];if(take)goto L8c6669e2;goto L8c6669dc; /* bf/s 0x8c6669e2 */
 goto L8c6669dc;
L8c6669dc: r[11]=rd(m,0x8c666a4cu,4); /* mov.l 0x8c666a4c,r11 */
 goto L8c6669de;
L8c6669de: goto L8c666a2c; /* bra 0x8c666a2c */
 goto L8c6669e2;
L8c6669e2: r[0]=sx(m,0x8c666a46u,2); /* mov.w 0x8c666a46,r0 */
 goto L8c6669e4;
L8c6669e4: r[6]=28u; /* mov #0x1c,r6 */
 goto L8c6669e6;
L8c6669e6: *macl=r[6]*r[12]; /* mul.l r6,r12 */
 goto L8c6669e8;
L8c6669e8: wr(m,r[5]+r[0],r[4],4); /* mov.l r4,@(r0,r5) */
 goto L8c6669ea;
L8c6669ea: r[0]+=4u; /* add #0x4,r0 */
 goto L8c6669ec;
L8c6669ec: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c6669ee;
L8c6669ee: r[2]=rd(m,0x8c666a54u,4); /* mov.l 0x8c666a54,r2 */
 goto L8c6669f0;
L8c6669f0: wr(m,r[3]+r[0],r[12],4); /* mov.l r12,@(r0,r3) */
 goto L8c6669f2;
L8c6669f2: r[0]+=12u; /* add #0xc,r0 */
 goto L8c6669f4;
L8c6669f4: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c6669f6;
L8c6669f6: r[6]=*macl; /* sts MACL,r6 */
 goto L8c6669f8;
L8c6669f8: wr(m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c6669fa;
L8c6669fa: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c6669fc;
L8c6669fc: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c6669fe;
L8c6669fe: wr(m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c666a00;
L8c666a00: r[3]=rd(m,0x8c666a50u,4); /* mov.l 0x8c666a50,r3 */
 goto L8c666a02;
L8c666a02: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c666a04;
L8c666a04: target=r[2];*pr=0x8c666a08u;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c666a08;
L8c666a08: r[3]=0u; /* mov #0x0,r3 */
 goto L8c666a0a;
L8c666a0a: r[6]=rd(m,0x8c666a58u,4); /* mov.l 0x8c666a58,r6 */
 goto L8c666a0c;
L8c666a0c: *sr=(*sr&~1u)|(r[3]>=r[12]); /* cmp/hs r12,r3 */
 goto L8c666a0e;
L8c666a0e: r[5]=r[13]; /* mov r13,r5 */
 goto L8c666a10;
L8c666a10: take=(*sr&1u);r[4]=r[13];if(take)goto L8c666a2c;goto L8c666a14; /* bt/s 0x8c666a2c */
 goto L8c666a14;
L8c666a14: r[0]=sx(m,0x8c666a46u,2); /* mov.w 0x8c666a46,r0 */
 goto L8c666a16;
L8c666a16: r[2]=r[6]; /* mov r6,r2 */
 goto L8c666a18;
L8c666a18: r[1]=rd(m,r[14],4); /* mov.l @r14,r1 */
 goto L8c666a1a;
L8c666a1a: r[3]=rd(m,0x8c666a5cu,4); /* mov.l 0x8c666a5c,r3 */
 goto L8c666a1c;
L8c666a1c: r[1]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r1 */
 goto L8c666a1e;
L8c666a1e: r[1]+=r[4]; /* add r4,r1 */
 goto L8c666a20;
L8c666a20: target=r[3];*pr=0x8c666a24u;r[0]=28u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666a24;
L8c666a24: r[5]+=1u; /* add #0x1,r5 */
 goto L8c666a26;
L8c666a26: *sr=(*sr&~1u)|(r[5]>=r[12]); /* cmp/hs r12,r5 */
 goto L8c666a28;
L8c666a28: take=!(*sr&1u);r[4]+=28u;if(take)goto L8c666a14;goto L8c666a2c; /* bf/s 0x8c666a14 */
 goto L8c666a2c;
L8c666a2c: *sr=(*sr&~1u)|!(r[11]&r[11]); /* tst r11,r11 */
 goto L8c666a2e;
L8c666a2e: if((*sr&1u))goto L8c666a38; /* bt 0x8c666a38 */
 goto L8c666a30;
L8c666a30: r[2]=rd(m,0x8c666a64u,4); /* mov.l 0x8c666a64,r2 */
 goto L8c666a32;
L8c666a32: r[4]=rd(m,0x8c666a60u,4); /* mov.l 0x8c666a60,r4 */
 goto L8c666a34;
L8c666a34: target=r[2];*pr=0x8c666a38u;r[5]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c666a38;
L8c666a38: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c666a3a;
L8c666a3a: r[0]=r[11]; /* mov r11,r0 */
 goto L8c666a3c;
L8c666a3c: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c666a3e;
L8c666a3e: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c666a40;
L8c666a40: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c666a42;
L8c666a42: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
