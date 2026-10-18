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
/* Static C recovery of retail register-table initializer 8c665a00..8c665a54.
 * Register writer653444 remains an external service;
 *  Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_register_table_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c665a00;
L8c665a00: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c665a02;
L8c665a02: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c665a04;
L8c665a04: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c665a06;
L8c665a06: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c665a08;
L8c665a08: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c665a0a;
L8c665a0a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c665a0c;
L8c665a0c: r[4]=sx(m,0x8c665a60u,2); /* mov.w 0x8c665a60,r4 */
 goto L8c665a0e;
L8c665a0e: r[5]=rd(m,0x8c665a70u,4); /* mov.l 0x8c665a70,r5 */
 goto L8c665a10;
L8c665a10: r[12]=rd(m,0x8c665a6cu,4); /* mov.l 0x8c665a6c,r12 */
 goto L8c665a12;
L8c665a12: target=r[12];*pr=0x8c665a16u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c665a16;
L8c665a16: r[4]=sx(m,0x8c665a62u,2); /* mov.w 0x8c665a62,r4 */
 goto L8c665a18;
L8c665a18: r[5]=rd(m,0x8c665a74u,4); /* mov.l 0x8c665a74,r5 */
 goto L8c665a1a;
L8c665a1a: target=r[12];*pr=0x8c665a1eu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c665a1e;
L8c665a1e: r[4]=sx(m,0x8c665a64u,2); /* mov.w 0x8c665a64,r4 */
 goto L8c665a20;
L8c665a20: r[5]=rd(m,0x8c665a74u,4); /* mov.l 0x8c665a74,r5 */
 goto L8c665a22;
L8c665a22: target=r[12];*pr=0x8c665a26u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c665a26;
L8c665a26: r[14]=sx(m,0x8c665a6au,2); /* mov.w 0x8c665a6a,r14 */
 goto L8c665a28;
L8c665a28: r[10]=127u; /* mov #0x7f,r10 */
 goto L8c665a2a;
L8c665a2a: r[13]=sx(m,0x8c665a68u,2); /* mov.w 0x8c665a68,r13 */
 goto L8c665a2c;
L8c665a2c: r[11]=sx(m,0x8c665a66u,2); /* mov.w 0x8c665a66,r11 */
 goto L8c665a2e;
L8c665a2e: r[5]=r[14]&255u; /* extu.b r14,r5 */
 goto L8c665a30;
L8c665a30: r[3]=r[13]&255u; /* extu.b r13,r3 */
 goto L8c665a32;
L8c665a32: r[5]<<=8; /* shll8 r5 */
 goto L8c665a34;
L8c665a34: r[5]|=r[3]; /* or r3,r5 */
 goto L8c665a36;
L8c665a36: r[5]=r[5]&65535u; /* extu.w r5,r5 */
 goto L8c665a38;
L8c665a38: target=r[12];*pr=0x8c665a3cu;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c665a3c;
L8c665a3c: r[14]+=4294967295u; /* add #-0x1,r14 */
 goto L8c665a3e;
L8c665a3e: *sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[10]); /* cmp/gt r10,r14 */
 goto L8c665a40;
L8c665a40: r[11]+=4u; /* add #0x4,r11 */
 goto L8c665a42;
L8c665a42: take=(*sr&1u);r[13]+=4294967295u;if(take)goto L8c665a2e;goto L8c665a46; /* bt/s 0x8c665a2e */
 goto L8c665a46;
L8c665a46: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c665a48;
L8c665a48: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c665a4a;
L8c665a4a: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c665a4c;
L8c665a4c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c665a4e;
L8c665a4e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c665a50;
L8c665a50: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
