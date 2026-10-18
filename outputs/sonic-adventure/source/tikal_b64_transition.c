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
/* Static C recovery of retail Tikal helper 0cb00b64..0cb00bdc.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int tikal_b64_transition(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;
 goto Lcb00b64;
Lcb00b64: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto Lcb00b66;
Lcb00b66: r[14]=r[6]; /* mov r6,r14 */
 goto Lcb00b68;
Lcb00b68: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto Lcb00b6a;
Lcb00b6a: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto Lcb00b6c;
Lcb00b6c: r[12]=r[4]; /* mov r4,r12 */
 goto Lcb00b6e;
Lcb00b6e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto Lcb00b70;
Lcb00b70: r[3]=rd(m,0x0cb00c10u,4); /* mov.l 0x0cb00c10,r3 */
 goto Lcb00b72;
Lcb00b72: target=r[3];*pr=0xcb00b76u;r[5]=r[6];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00b76;
Lcb00b76: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb00b78;
Lcb00b78: if((*sr&1u))goto Lcb00bd0; /* bt 0x0cb00bd0 */
 goto Lcb00b7a;
Lcb00b7a: r[0]=4294967220u; /* mov #-0x4c,r0 */
 goto Lcb00b7c;
Lcb00b7c: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto Lcb00b7e;
Lcb00b7e: r[3]=3u; /* mov #0x3,r3 */
 goto Lcb00b80;
Lcb00b80: wr(m,r[12],r[3],1); /* mov.b r3,@r12 */
 goto Lcb00b82;
Lcb00b82: fr[3]=rd(m,r[0]+r[14],4); /* fmov.s @(r0,r14),fr3 */
 goto Lcb00b84;
Lcb00b84: r[0]=60u; /* mov #0x3c,r0 */
 goto Lcb00b86;
Lcb00b86: wr(m,r[0]+r[14],fr[3],4); /* fmov fr3,@(r0,r14) */
 goto Lcb00b88;
Lcb00b88: r[0]=108u; /* mov #0x6c,r0 */
 goto Lcb00b8a;
Lcb00b8a: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto Lcb00b8c;
Lcb00b8c: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto Lcb00b8e;
Lcb00b8e: if((*sr&1u))goto Lcb00bac; /* bt 0x0cb00bac */
 goto Lcb00b90;
Lcb00b90: r[13]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r13 */
 goto Lcb00b92;
Lcb00b92: r[13]=rd(m,r[13]+0x28u,4); /* mov.l @(0x28,r13),r13 */
 goto Lcb00b94;
Lcb00b94: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto Lcb00b96;
Lcb00b96: if((*sr&1u))goto Lcb00bac; /* bt 0x0cb00bac */
 goto Lcb00b98;
Lcb00b98: r[0]=24u; /* mov #0x18,r0 */
 goto Lcb00b9a;
Lcb00b9a: r[3]=rd(m,0x0cb00c14u,4); /* mov.l 0x0cb00c14,r3 */
 goto Lcb00b9c;
Lcb00b9c: fr[6]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr6 */
 goto Lcb00b9e;
Lcb00b9e: r[0]=20u; /* mov #0x14,r0 */
 goto Lcb00ba0;
Lcb00ba0: fr[5]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr5 */
 goto Lcb00ba2;
Lcb00ba2: r[0]=16u; /* mov #0x10,r0 */
 goto Lcb00ba4;
Lcb00ba4: fr[4]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr4 */
 goto Lcb00ba6;
Lcb00ba6: r[0]=sx(m,r[12]+0x8u,1); /* mov.b @(0x8,r12),r0 */
 goto Lcb00ba8;
Lcb00ba8: target=r[3];*pr=0xcb00bacu;r[4]=r[0];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00bac;
Lcb00bac: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb00bae;
Lcb00bae: r[0]<<=2; /* shll2 r0 */
 goto Lcb00bb0;
Lcb00bb0: r[2]=3u; /* mov #0x3,r2 */
 goto Lcb00bb2;
Lcb00bb2: wr(m,r[14]+r[0],r[2],2); /* mov.w r2,@(r0,r14) */
 goto Lcb00bb4;
Lcb00bb4: r[3]=4294967293u; /* mov #-0x3,r3 */
 goto Lcb00bb6;
Lcb00bb6: r[0]=sx(m,r[12]+0x4u,2); /* mov.w @(0x4,r12),r0 */
 goto Lcb00bb8;
Lcb00bb8: r[2]=rd(m,0x0cb00c18u,4); /* mov.l 0x0cb00c18,r2 */
 goto Lcb00bba;
Lcb00bba: r[0]&=r[3]; /* and r3,r0 */
 goto Lcb00bbc;
Lcb00bbc: wr(m,r[12]+0x4u,r[0],2); /* mov.w r0,@(0x4,r12) */
 goto Lcb00bbe;
Lcb00bbe: r[0]=0u; /* mov #0x0,r0 */
 goto Lcb00bc0;
Lcb00bc0: r[5]=r[0]; /* mov r0,r5 */
 goto Lcb00bc2;
Lcb00bc2: r[6]=r[0]; /* mov r0,r6 */
 goto Lcb00bc4;
Lcb00bc4: wr(m,r[14]+0x8u,r[0],2); /* mov.w r0,@(0x8,r14) */
 goto Lcb00bc6;
Lcb00bc6: r[7]=r[0]; /* mov r0,r7 */
 goto Lcb00bc8;
Lcb00bc8: target=r[2];*pr=0xcb00bccu;r[4]=17u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb00bcc;
Lcb00bcc: r[0]=1u;goto Lcb00bd2; /* bra 0x0cb00bd2 */
 goto Lcb00bd0;
Lcb00bd0: r[0]=0u; /* mov #0x0,r0 */
 goto Lcb00bd2;
Lcb00bd2: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto Lcb00bd4;
Lcb00bd4: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto Lcb00bd6;
Lcb00bd6: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto Lcb00bd8;
Lcb00bd8: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
