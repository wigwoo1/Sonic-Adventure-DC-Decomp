#include "fpu_cleanup_transfer.h"
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
/* Static C recovery of retail Tikal helper 0cb00c20..0cb00c64.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int tikal_c20_transition(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;
 goto Lcb00c20;
Lcb00c20: r[3]=52u; /* mov #0x34,r3 */
 goto Lcb00c22;
Lcb00c22: r[1]=rd(m,0x0cb00d00u,4); /* mov.l 0x0cb00d00,r1 */
 goto Lcb00c24;
Lcb00c24: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto Lcb00c26;
Lcb00c26: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto Lcb00c28;
Lcb00c28: r[0]=sx(m,r[4]+0x8u,1); /* mov.b @(0x8,r4),r0 */
 goto Lcb00c2a;
Lcb00c2a: r[2]=rd(m,r[1],4); /* mov.l @r1,r2 */
 goto Lcb00c2c;
Lcb00c2c: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto Lcb00c2e;
Lcb00c2e: r[1]=rd(m,0x0cb00d04u,4); /* mov.l 0x0cb00d04,r1 */
 goto Lcb00c30;
Lcb00c30: *macl=(r[3]&65535u)*(r[0]&65535u); /* mulu.w r3,r0 */
 goto Lcb00c32;
Lcb00c32: r[0]=*macl; /* sts MACL,r0 */
 goto Lcb00c34;
Lcb00c34: r[1]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r1 */
 goto Lcb00c36;
Lcb00c36: *sr=(*sr&~1u)|!(r[2]&r[1]); /* tst r2,r1 */
 goto Lcb00c38;
Lcb00c38: take=(*sr&1u);r[14]=r[6];if(take)goto Lcb00c5c;goto Lcb00c3c; /* bt/s 0x0cb00c5c */
 goto Lcb00c3c;
Lcb00c3c: r[1]=rd(m,0x0cb00d08u,4); /* mov.l 0x0cb00d08,r1 */
 goto Lcb00c3e;
Lcb00c3e: r[2]=6u; /* mov #0x6,r2 */
 goto Lcb00c40;
Lcb00c40: r[5]=r[14]; /* mov r14,r5 */
 goto Lcb00c42;
Lcb00c42: wr(m,r[4],r[2],1); /* mov.b r2,@r4 */
 goto Lcb00c44;
Lcb00c44: target=r[1];*pr=0xcb00c48u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto Lcb00c48;
Lcb00c48: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb00c4a;
Lcb00c4a: fr[3]=0; /* fldi0 fr3 */
 goto Lcb00c4c;
Lcb00c4c: r[0]<<=2; /* shll2 r0 */
 goto Lcb00c4e;
Lcb00c4e: r[3]=5u; /* mov #0x5,r3 */
 goto Lcb00c50;
Lcb00c50: wr(m,r[14]+r[0],r[3],2); /* mov.w r3,@(r0,r14) */
 goto Lcb00c52;
Lcb00c52: r[0]=1u; /* mov #0x1,r0 */
 goto Lcb00c54;
Lcb00c54: wr(m,r[14],fr[3],4); /* fmov.s fr3,@r14 */
 goto Lcb00c56;
Lcb00c56: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto Lcb00c58;
Lcb00c58: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
Lcb00c5c: r[0]=0u; /* mov #0x0,r0 */
 goto Lcb00c5e;
Lcb00c5e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto Lcb00c60;
Lcb00c60: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
