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
/* Static C recovery of retail Tikal helper 8c660dec..8c660e56.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_available(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c660dec;
L8c660dec: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c660dee;
L8c660dee: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c660df0;
L8c660df0: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c660df2;
L8c660df2: r[0]=rd(m,0x8c660e3cu,4); /* mov.l 0x8c660e3c,r0 */
 goto L8c660df4;
L8c660df4: r[14]=r[4]; /* mov r4,r14 */
 goto L8c660df6;
L8c660df6: r[14]<<=2; /* shll2 r14 */
 goto L8c660df8;
L8c660df8: r[3]=rd(m,0x8c660e40u,4); /* mov.l 0x8c660e40,r3 */
 goto L8c660dfa;
L8c660dfa: r[14]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r14 */
 goto L8c660dfc;
L8c660dfc: target=r[3];*pr=0x8c660e00u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c660e00;
L8c660e00: r[3]=rd(m,0x8c660e44u,4); /* mov.l 0x8c660e44,r3 */
 goto L8c660e02;
L8c660e02: wr(m,r[15]+0x4u,r[0],4); /* mov.l r0,@(0x4,r15) */
 goto L8c660e04;
L8c660e04: target=r[3];*pr=0x8c660e08u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c660e08;
L8c660e08: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c660e0a;
L8c660e0a: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c660e0c;
L8c660e0c: r[2]=sx(m,r[3],1); /* mov.b @r3,r2 */
 goto L8c660e0e;
L8c660e0e: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c660e10;
L8c660e10: if((*sr&1u))goto L8c660e24; /* bt 0x8c660e24 */
 goto L8c660e12;
L8c660e12: r[3]=rd(m,0x8c660e48u,4); /* mov.l 0x8c660e48,r3 */
 goto L8c660e14;
L8c660e14: target=r[3];*pr=0x8c660e18u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c660e18;
L8c660e18: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c660e1a;
L8c660e1a: if(!(*sr&1u))goto L8c660e24; /* bf 0x8c660e24 */
 goto L8c660e1c;
L8c660e1c: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c660e1e;
L8c660e1e: r[0]=rd(m,r[0],4); /* mov.l @r0,r0 */
 goto L8c660e20;
L8c660e20: *sr=(*sr&~1u)|!(r[0]&0x2u); /* tst #0x2,r0 */
 goto L8c660e22;
L8c660e22: if(!(*sr&1u))goto L8c660e4c; /* bf 0x8c660e4c */
 goto L8c660e24;
L8c660e24: r[0]=0u; /* mov #0x0,r0 */
 goto L8c660e26;
L8c660e26: r[15]+=8u; /* add #0x8,r15 */
 goto L8c660e28;
L8c660e28: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c660e2a;
L8c660e2a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c660e4c: r[0]=1u; /* mov #0x1,r0 */
 goto L8c660e4e;
L8c660e4e: r[15]+=8u; /* add #0x8,r15 */
 goto L8c660e50;
L8c660e50: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c660e52;
L8c660e52: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
