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
/* Static C recovery of retail Tikal helper 0cb00a40..0cb00b64.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int tikal_state11(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;
 goto Lcb00a40;
Lcb00a40: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto Lcb00a42;
Lcb00a42: r[14]=r[4]; /* mov r4,r14 */
 goto Lcb00a44;
Lcb00a44: r[3]=52u; /* mov #0x34,r3 */
 goto Lcb00a46;
Lcb00a46: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto Lcb00a48;
Lcb00a48: r[1]=rd(m,0x0cb00be0u,4); /* mov.l 0x0cb00be0,r1 */
 goto Lcb00a4a;
Lcb00a4a: r[15]-=4;wr(m,r[15],fr[15],4); /* fmov.s fr15,@-r15 */
 goto Lcb00a4c;
Lcb00a4c: r[15]-=4;wr(m,r[15],fr[14],4); /* fmov.s fr14,@-r15 */
 goto Lcb00a4e;
Lcb00a4e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto Lcb00a50;
Lcb00a50: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto Lcb00a52;
Lcb00a52: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto Lcb00a54;
Lcb00a54: *macl=(r[3]&65535u)*(r[0]&65535u); /* mulu.w r3,r0 */
 goto Lcb00a56;
Lcb00a56: r[0]=*macl; /* sts MACL,r0 */
 goto Lcb00a58;
Lcb00a58: r[0]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto Lcb00a5a;
Lcb00a5a: *sr=(*sr&~1u)|!(r[0]&0x2u); /* tst #0x2,r0 */
 goto Lcb00a5c;
Lcb00a5c: if((*sr&1u))goto Lcb00a64; /* bt 0x0cb00a64 */
 goto Lcb00a5e;
Lcb00a5e: r[2]=rd(m,0x0cb00be4u,4); /* mov.l 0x0cb00be4,r2 */
 goto Lcb00a60;
Lcb00a60: target=r[2];*pr=0xcb00a64u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb00a64;
Lcb00a64: r[0]=0x0cb00be8u; /* mova 0x0cb00be8,r0 */
 goto Lcb00a66;
Lcb00a66: r[4]=rd(m,0x0cb00bf0u,4); /* mov.l 0x0cb00bf0,r4 */
 goto Lcb00a68;
Lcb00a68: fr[14]=rd(m,r[0],4); /* fmov.s @r0,fr14 */
 goto Lcb00a6a;
Lcb00a6a: r[0]=0x0cb00becu; /* mova 0x0cb00bec,r0 */
 goto Lcb00a6c;
Lcb00a6c: fr[6]=rd(m,r[0],4); /* fmov.s @r0,fr6 */
 goto Lcb00a6e;
Lcb00a6e: r[3]=52u; /* mov #0x34,r3 */
 goto Lcb00a70;
Lcb00a70: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto Lcb00a72;
Lcb00a72: r[2]=4u; /* mov #0x4,r2 */
 goto Lcb00a74;
Lcb00a74: r[2]<<=8; /* shll8 r2 */
 goto Lcb00a76;
Lcb00a76: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto Lcb00a78;
Lcb00a78: *macl=(r[3]&65535u)*(r[0]&65535u); /* mulu.w r3,r0 */
 goto Lcb00a7a;
Lcb00a7a: r[0]=*macl; /* sts MACL,r0 */
 goto Lcb00a7c;
Lcb00a7c: r[0]+=r[4]; /* add r4,r0 */
 goto Lcb00a7e;
Lcb00a7e: r[1]=rd(m,r[0]+0x8u,4); /* mov.l @(0x8,r0),r1 */
 goto Lcb00a80;
Lcb00a80: *sr=(*sr&~1u)|!(r[2]&r[1]); /* tst r2,r1 */
 goto Lcb00a82;
Lcb00a82: if((*sr&1u))goto Lcb00ad8; /* bt 0x0cb00ad8 */
 goto Lcb00a84;
Lcb00a84: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto Lcb00a86;
Lcb00a86: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto Lcb00a88;
Lcb00a88: *macl=(r[3]&65535u)*(r[0]&65535u); /* mulu.w r3,r0 */
 goto Lcb00a8a;
Lcb00a8a: r[5]=*macl; /* sts MACL,r5 */
 goto Lcb00a8c;
Lcb00a8c: r[5]+=r[4]; /* add r4,r5 */
 goto Lcb00a8e;
Lcb00a8e: r[0]=sx(m,r[5]+0x1cu,2); /* mov.w @(0x1c,r5),r0 */
 goto Lcb00a90;
Lcb00a90: r[0]<<=8; /* shll8 r0 */
 goto Lcb00a92;
Lcb00a92: *fpul=r[0]; /* lds r0,FPUL */
 goto Lcb00a94;
Lcb00a94: r[0]=sx(m,r[5]+0x1eu,2); /* mov.w @(0x1e,r5),r0 */
 goto Lcb00a96;
Lcb00a96: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto Lcb00a98;
Lcb00a98: r[0]<<=8; /* shll8 r0 */
 goto Lcb00a9a;
Lcb00a9a: *fpul=r[0]; /* lds r0,FPUL */
 goto Lcb00a9c;
Lcb00a9c: fr[4]=fr[3]; /* fmov fr3,fr4 */
 goto Lcb00a9e;
Lcb00a9e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto Lcb00aa0;
Lcb00aa0: status=gt(f,sr,fr[4],fr[6]);if(status)return status; /* fcmp/gt fr6,fr4 */
 goto Lcb00aa2;
Lcb00aa2: take=(*sr&1u);fr[15]=fr[3];if(take)goto Lcb00ab6;goto Lcb00aa6; /* bt/s 0x0cb00ab6 */
 goto Lcb00aa6;
Lcb00aa6: r[0]=0x0cb00bf4u; /* mova 0x0cb00bf4,r0 */
 goto Lcb00aa8;
Lcb00aa8: fr[3]=rd(m,r[0],4); /* fmov.s @r0,fr3 */
 goto Lcb00aaa;
Lcb00aaa: status=gt(f,sr,fr[3],fr[4]);if(status)return status; /* fcmp/gt fr4,fr3 */
 goto Lcb00aac;
Lcb00aac: if((*sr&1u))goto Lcb00ab6; /* bt 0x0cb00ab6 */
 goto Lcb00aae;
Lcb00aae: status=gt(f,sr,fr[15],fr[6]);if(status)return status; /* fcmp/gt fr6,fr15 */
 goto Lcb00ab0;
Lcb00ab0: if((*sr&1u))goto Lcb00ab6; /* bt 0x0cb00ab6 */
 goto Lcb00ab2;
Lcb00ab2: status=gt(f,sr,fr[3],fr[15]);if(status)return status; /* fcmp/gt fr15,fr3 */
 goto Lcb00ab4;
Lcb00ab4: if(!(*sr&1u))goto Lcb00b58; /* bf 0x0cb00b58 */
 goto Lcb00ab6;
Lcb00ab6: fr[3]=0; /* fldi0 fr3 */
 goto Lcb00ab8;
Lcb00ab8: status=eq(f,sr,fr[15],fr[3]);if(status)return status; /* fcmp/eq fr3,fr15 */
 goto Lcb00aba;
Lcb00aba: if((*sr&1u))goto Lcb00aca; /* bt 0x0cb00aca */
 goto Lcb00abc;
Lcb00abc: r[2]=rd(m,0x0cb00bf8u,4); /* mov.l 0x0cb00bf8,r2 */
 goto Lcb00abe;
Lcb00abe: fr[4]=fr[15]; /* fmov fr15,fr4 */
 goto Lcb00ac0;
Lcb00ac0: target=r[2];*pr=0xcb00ac4u;status=sa_sh4_multiply(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb00ac4;
Lcb00ac4: fr[4]=fr[15]; /* fmov fr15,fr4 */
 goto Lcb00ac6;
Lcb00ac6: status=sa_sh4_divide(&fr[4],&fr[0],&f->fpscr,&fr[4]);if(status)return status;goto Lcb00acc; /* bra 0x0cb00acc */
 goto Lcb00aca;
Lcb00aca: fr[4]=fr[3]; /* fmov fr3,fr4 */
 goto Lcb00acc;
Lcb00acc: status=sa_sh4_multiply(&fr[4],&fr[14],&f->fpscr,&fr[4]);if(status)return status; /* fmul fr14,fr4 */
 goto Lcb00ace;
Lcb00ace: r[0]=36u; /* mov #0x24,r0 */
 goto Lcb00ad0;
Lcb00ad0: fr[3]=rd(m,r[0]+r[14],4); /* fmov.s @(r0,r14),fr3 */
 goto Lcb00ad2;
Lcb00ad2: status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status; /* fsub fr4,fr3 */
 goto Lcb00ad4;
Lcb00ad4: wr(m,r[0]+r[14],fr[3],4);goto Lcb00b58; /* bra 0x0cb00b58 */
 goto Lcb00ad8;
Lcb00ad8: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto Lcb00ada;
Lcb00ada: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto Lcb00adc;
Lcb00adc: *macl=(r[3]&65535u)*(r[0]&65535u); /* mulu.w r3,r0 */
 goto Lcb00ade;
Lcb00ade: r[5]=*macl; /* sts MACL,r5 */
 goto Lcb00ae0;
Lcb00ae0: r[5]+=r[4]; /* add r4,r5 */
 goto Lcb00ae2;
Lcb00ae2: r[0]=sx(m,r[5]+0x1cu,2); /* mov.w @(0x1c,r5),r0 */
 goto Lcb00ae4;
Lcb00ae4: r[0]<<=8; /* shll8 r0 */
 goto Lcb00ae6;
Lcb00ae6: *fpul=r[0]; /* lds r0,FPUL */
 goto Lcb00ae8;
Lcb00ae8: r[0]=sx(m,r[5]+0x1eu,2); /* mov.w @(0x1e,r5),r0 */
 goto Lcb00aea;
Lcb00aea: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto Lcb00aec;
Lcb00aec: r[0]<<=8; /* shll8 r0 */
 goto Lcb00aee;
Lcb00aee: *fpul=r[0]; /* lds r0,FPUL */
 goto Lcb00af0;
Lcb00af0: fr[5]=fr[3]; /* fmov fr3,fr5 */
 goto Lcb00af2;
Lcb00af2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto Lcb00af4;
Lcb00af4: status=gt(f,sr,fr[5],fr[6]);if(status)return status; /* fcmp/gt fr6,fr5 */
 goto Lcb00af6;
Lcb00af6: take=(*sr&1u);fr[4]=fr[3];if(take)goto Lcb00b0a;goto Lcb00afa; /* bt/s 0x0cb00b0a */
 goto Lcb00afa;
Lcb00afa: r[0]=0x0cb00bf4u; /* mova 0x0cb00bf4,r0 */
 goto Lcb00afc;
Lcb00afc: fr[3]=rd(m,r[0],4); /* fmov.s @r0,fr3 */
 goto Lcb00afe;
Lcb00afe: status=gt(f,sr,fr[3],fr[5]);if(status)return status; /* fcmp/gt fr5,fr3 */
 goto Lcb00b00;
Lcb00b00: if((*sr&1u))goto Lcb00b0a; /* bt 0x0cb00b0a */
 goto Lcb00b02;
Lcb00b02: status=gt(f,sr,fr[4],fr[6]);if(status)return status; /* fcmp/gt fr6,fr4 */
 goto Lcb00b04;
Lcb00b04: if((*sr&1u))goto Lcb00b0a; /* bt 0x0cb00b0a */
 goto Lcb00b06;
Lcb00b06: status=gt(f,sr,fr[3],fr[4]);if(status)return status; /* fcmp/gt fr4,fr3 */
 goto Lcb00b08;
Lcb00b08: if(!(*sr&1u))goto Lcb00b58; /* bf 0x0cb00b58 */
 goto Lcb00b0a;
Lcb00b0a: r[2]=rd(m,0x0cb00bfcu,4); /* mov.l 0x0cb00bfc,r2 */
 goto Lcb00b0c;
Lcb00b0c: target=r[2];*pr=0xcb00b10u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb00b10;
Lcb00b10: r[0]=0x0cb00c00u; /* mova 0x0cb00c00,r0 */
 goto Lcb00b12;
Lcb00b12: fr[2]=fr[0]; /* fmov fr0,fr2 */
 goto Lcb00b14;
Lcb00b14: fr[3]=rd(m,r[0],4); /* fmov.s @r0,fr3 */
 goto Lcb00b16;
Lcb00b16: r[0]=32u; /* mov #0x20,r0 */
 goto Lcb00b18;
Lcb00b18: r[2]=rd(m,0x0cb00c04u,4); /* mov.l 0x0cb00c04,r2 */
 goto Lcb00b1a;
Lcb00b1a: status=sa_sh4_multiply(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status; /* fmul fr3,fr2 */
 goto Lcb00b1c;
Lcb00b1c: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto Lcb00b1e;
Lcb00b1e: r[1]=rd(m,r[3]+0x18u,4); /* mov.l @(0x18,r3),r1 */
 goto Lcb00b20;
Lcb00b20: r[3]=r[14]; /* mov r14,r3 */
 goto Lcb00b22;
Lcb00b22: status=sa_sh4_truncate_i32(fr[2],&f->fpscr,fpul);if(status)return status; /* ftrc fr2,FPUL */
 goto Lcb00b24;
Lcb00b24: r[3]+=r[0]; /* add r0,r3 */
 goto Lcb00b26;
Lcb00b26: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto Lcb00b28;
Lcb00b28: r[3]=rd(m,0x0cb00c08u,4); /* mov.l 0x0cb00c08,r3 */
 goto Lcb00b2a;
Lcb00b2a: r[13]=*fpul; /* sts FPUL,r13 */
 goto Lcb00b2c;
Lcb00b2c: r[13]-=r[1]; /* sub r1,r13 */
 goto Lcb00b2e;
Lcb00b2e: target=r[3];*pr=0xcb00b32u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00b32;
Lcb00b32: r[2]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r2 */
 goto Lcb00b34;
Lcb00b34: r[0]=40u; /* mov #0x28,r0 */
 goto Lcb00b36;
Lcb00b36: fr[3]=fr[0]; /* fmov fr0,fr3 */
 goto Lcb00b38;
Lcb00b38: r[3]=r[14]; /* mov r14,r3 */
 goto Lcb00b3a;
Lcb00b3a: fr[2]=rd(m,r[2],4); /* fmov.s @r2,fr2 */
 goto Lcb00b3c;
Lcb00b3c: r[3]+=r[0]; /* add r0,r3 */
 goto Lcb00b3e;
Lcb00b3e: fr[0]=fr[14]; /* fmov fr14,fr0 */
 goto Lcb00b40;
Lcb00b40: status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status; /* fmac fr0,fr3,fr2 */
 goto Lcb00b42;
Lcb00b42: wr(m,r[2],fr[2],4); /* fmov.s fr2,@r2 */
 goto Lcb00b44;
Lcb00b44: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto Lcb00b46;
Lcb00b46: r[3]=rd(m,0x0cb00c0cu,4); /* mov.l 0x0cb00c0c,r3 */
 goto Lcb00b48;
Lcb00b48: target=r[3];*pr=0xcb00b4cu;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00b4c;
Lcb00b4c: r[2]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r2 */
 goto Lcb00b4e;
Lcb00b4e: fr[3]=fr[0]; /* fmov fr0,fr3 */
 goto Lcb00b50;
Lcb00b50: fr[2]=rd(m,r[2],4); /* fmov.s @r2,fr2 */
 goto Lcb00b52;
Lcb00b52: fr[0]=fr[14]; /* fmov fr14,fr0 */
 goto Lcb00b54;
Lcb00b54: status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status; /* fmac fr0,fr3,fr2 */
 goto Lcb00b56;
Lcb00b56: wr(m,r[2],fr[2],4); /* fmov.s fr2,@r2 */
 goto Lcb00b58;
Lcb00b58: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto Lcb00b5a;
Lcb00b5a: fr[14]=rd(m,r[15],4);r[15]+=4; /* fmov.s @r15+,fr14 */
 goto Lcb00b5c;
Lcb00b5c: fr[15]=rd(m,r[15],4);r[15]+=4; /* fmov.s @r15+,fr15 */
 goto Lcb00b5e;
Lcb00b5e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto Lcb00b60;
Lcb00b60: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
