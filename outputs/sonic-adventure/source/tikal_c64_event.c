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
/* Static C recovery of retail Tikal helper 0cb00c64..0cb00cd2.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int tikal_c64_event(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;
 goto Lcb00c64;
Lcb00c64: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto Lcb00c66;
Lcb00c66: r[14]=r[4]; /* mov r4,r14 */
 goto Lcb00c68;
Lcb00c68: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto Lcb00c6a;
Lcb00c6a: r[0]=36u; /* mov #0x24,r0 */
 goto Lcb00c6c;
Lcb00c6c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto Lcb00c6e;
Lcb00c6e: r[13]=rd(m,r[14]+0x3cu,4); /* mov.l @(0x3c,r14),r13 */
 goto Lcb00c70;
Lcb00c70: r[0]=sx(m,r[13]+r[0],1); /* mov.b @(r0,r13),r0 */
 goto Lcb00c72;
Lcb00c72: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto Lcb00c74;
Lcb00c74: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto Lcb00c76;
Lcb00c76: if((*sr&1u))goto Lcb00c90; /* bt 0x0cb00c90 */
 goto Lcb00c78;
Lcb00c78: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto Lcb00c7a;
Lcb00c7a: if((*sr&1u))goto Lcb00c90; /* bt 0x0cb00c90 */
 goto Lcb00c7c;
Lcb00c7c: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto Lcb00c7e;
Lcb00c7e: if((*sr&1u))goto Lcb00c90; /* bt 0x0cb00c90 */
 goto Lcb00c80;
Lcb00c80: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto Lcb00c82;
Lcb00c82: if((*sr&1u))goto Lcb00c90; /* bt 0x0cb00c90 */
 goto Lcb00c84;
Lcb00c84: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto Lcb00c86;
Lcb00c86: if((*sr&1u))goto Lcb00c9c; /* bt 0x0cb00c9c */
 goto Lcb00c88;
Lcb00c88: *sr=(*sr&~1u)|(r[0]==0x5u); /* cmp/eq #0x5,r0 */
 goto Lcb00c8a;
Lcb00c8a: if((*sr&1u))goto Lcb00ca0; /* bt 0x0cb00ca0 */
 goto Lcb00c8c;
Lcb00c8c: goto Lcb00cb8; /* bra 0x0cb00cb8 */
 goto Lcb00c90;
Lcb00c90: r[0]=sx(m,r[14]+0x4u,2); /* mov.w @(0x4,r14),r0 */
 goto Lcb00c92;
Lcb00c92: r[3]=64u; /* mov #0x40,r3 */
 goto Lcb00c94;
Lcb00c94: r[3]<<=8; /* shll8 r3 */
 goto Lcb00c96;
Lcb00c96: r[0]|=r[3]; /* or r3,r0 */
 goto Lcb00c98;
Lcb00c98: wr(m,r[14]+0x4u,r[0],2);goto Lcb00cc8; /* bra 0x0cb00cc8 */
 goto Lcb00c9c;
Lcb00c9c: r[5]=rd(m,r[13]+0x2cu,4);goto Lcb00cac; /* bra 0x0cb00cac */
 goto Lcb00ca0;
Lcb00ca0: r[5]=rd(m,r[13]+0x2cu,4); /* mov.l @(0x2c,r13),r5 */
 goto Lcb00ca2;
Lcb00ca2: r[2]=64u; /* mov #0x40,r2 */
 goto Lcb00ca4;
Lcb00ca4: r[3]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r3 */
 goto Lcb00ca6;
Lcb00ca6: r[2]<<=8; /* shll8 r2 */
 goto Lcb00ca8;
Lcb00ca8: r[5]-=r[3]; /* sub r3,r5 */
 goto Lcb00caa;
Lcb00caa: r[5]+=r[2]; /* add r2,r5 */
 goto Lcb00cac;
Lcb00cac: r[3]=rd(m,0x0cb00d0cu,4); /* mov.l 0x0cb00d0c,r3 */
 goto Lcb00cae;
Lcb00cae: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto Lcb00cb0;
Lcb00cb0: target=r[3];*pr=0xcb00cb4u;r[4]=r[0];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00cb4;
Lcb00cb4: goto Lcb00cc8; /* bra 0x0cb00cc8 */
 goto Lcb00cb8;
Lcb00cb8: r[3]=4294967232u; /* mov #-0x40,r3 */
 goto Lcb00cba;
Lcb00cba: r[0]=sx(m,r[14]+0x4u,2); /* mov.w @(0x4,r14),r0 */
 goto Lcb00cbc;
Lcb00cbc: r[3]<<=8; /* shll8 r3 */
 goto Lcb00cbe;
Lcb00cbe: r[3]+=4294967295u; /* add #-0x1,r3 */
 goto Lcb00cc0;
Lcb00cc0: r[0]&=r[3]; /* and r3,r0 */
 goto Lcb00cc2;
Lcb00cc2: wr(m,r[14]+0x4u,r[0],2); /* mov.w r0,@(0x4,r14) */
 goto Lcb00cc4;
Lcb00cc4: r[0]=0u;goto Lcb00cca; /* bra 0x0cb00cca */
 goto Lcb00cc8;
Lcb00cc8: r[0]=1u; /* mov #0x1,r0 */
 goto Lcb00cca;
Lcb00cca: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto Lcb00ccc;
Lcb00ccc: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto Lcb00cce;
Lcb00cce: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
