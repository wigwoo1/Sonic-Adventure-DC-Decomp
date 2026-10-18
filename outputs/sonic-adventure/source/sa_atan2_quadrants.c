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
/* Static C recovery of retail Tikal helper 8c10e6a0..8c10e6e6.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_atan2_quadrants(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10e6a0;
L8c10e6a0: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10e6a2;
L8c10e6a2: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c10e6a4;
L8c10e6a4: r[3]=r[15]; /* mov r15,r3 */
 goto L8c10e6a6;
L8c10e6a6: r[3]+=4u; /* add #0x4,r3 */
 goto L8c10e6a8;
L8c10e6a8: r[0]=4u; /* mov #0x4,r0 */
 goto L8c10e6aa;
L8c10e6aa: wr(m,r[3],fr[4],4); /* fmov.s fr4,@r3 */
 goto L8c10e6ac;
L8c10e6ac: r[3]=rd(m,0x8c10e6e8u,4); /* mov.l 0x8c10e6e8,r3 */
 goto L8c10e6ae;
L8c10e6ae: wr(m,r[15],fr[5],4); /* fmov.s fr5,@r15 */
 goto L8c10e6b0;
L8c10e6b0: fr[4]=rd(m,r[0]+r[15],4); /* fmov.s @(r0,r15),fr4 */
 goto L8c10e6b2;
L8c10e6b2: fr[3]=fr[5]; /* fmov fr5,fr3 */
 goto L8c10e6b4;
L8c10e6b4: target=r[3];*pr=0x8c10e6b8u;status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10e6b8;
L8c10e6b8: r[2]=r[15]; /* mov r15,r2 */
 goto L8c10e6ba;
L8c10e6ba: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c10e6bc;
L8c10e6bc: r[4]=rd(m,0x8c10e6ecu,4); /* mov.l 0x8c10e6ec,r4 */
 goto L8c10e6be;
L8c10e6be: *sr=(*sr&~1u)|!(r[4]&r[3]); /* tst r4,r3 */
 goto L8c10e6c0;
L8c10e6c0: take=(*sr&1u);fr[4]=fr[0];if(take)goto L8c10e6de;goto L8c10e6c4; /* bt/s 0x8c10e6de */
 goto L8c10e6c4;
L8c10e6c4: r[1]=r[15]; /* mov r15,r1 */
 goto L8c10e6c6;
L8c10e6c6: r[1]+=4u; /* add #0x4,r1 */
 goto L8c10e6c8;
L8c10e6c8: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c10e6ca;
L8c10e6ca: *sr=(*sr&~1u)|!(r[3]&r[4]); /* tst r3,r4 */
 goto L8c10e6cc;
L8c10e6cc: if((*sr&1u))goto L8c10e6d6; /* bt 0x8c10e6d6 */
 goto L8c10e6ce;
L8c10e6ce: r[2]=rd(m,0x8c10e6f0u,4); /* mov.l 0x8c10e6f0,r2 */
 goto L8c10e6d0;
L8c10e6d0: fr[3]=fr[4]; /* fmov fr4,fr3 */
 goto L8c10e6d2;
L8c10e6d2: fr[4]=rd(m,r[2],4);goto L8c10e6dc; /* bra 0x8c10e6dc */
 goto L8c10e6d6;
L8c10e6d6: r[3]=rd(m,0x8c10e6f4u,4); /* mov.l 0x8c10e6f4,r3 */
 goto L8c10e6d8;
L8c10e6d8: fr[3]=fr[4]; /* fmov fr4,fr3 */
 goto L8c10e6da;
L8c10e6da: fr[4]=rd(m,r[3],4); /* fmov.s @r3,fr4 */
 goto L8c10e6dc;
L8c10e6dc: status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status; /* fadd fr3,fr4 */
 goto L8c10e6de;
L8c10e6de: r[15]+=8u; /* add #0x8,r15 */
 goto L8c10e6e0;
L8c10e6e0: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10e6e2;
L8c10e6e2: fr[0]=fr[4];return 0; /* rts */
}
