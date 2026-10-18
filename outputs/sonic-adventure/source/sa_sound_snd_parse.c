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
/* Retail .snd/.sd-NUL header parser, 8c669c14..8c669e38.
 * Encoding 1/2 produces 8-bit samples; encoding 3 produces 16-bit samples.
 * Returns data+big-endian offset and computes frames using native quotient. */
SA_STAGE_EXPORT int sa_sound_snd_parse(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c669c14;
L8c669c14: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c669c16;
L8c669c16: r[14]=r[4]; /* mov r4,r14 */
 goto L8c669c18;
L8c669c18: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c669c1a;
L8c669c1a: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c669c1c;
L8c669c1c: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c669c1e;
L8c669c1e: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c669c20;
L8c669c20: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c669c22;
L8c669c22: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c669c24;
L8c669c24: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c669c26;
L8c669c26: r[0]=sx(m,r[14]+0x1u,1); /* mov.b @(0x1,r14),r0 */
 goto L8c669c28;
L8c669c28: r[3]=sx(m,r[14],1); /* mov.b @r14,r3 */
 goto L8c669c2a;
L8c669c2a: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669c2c;
L8c669c2c: r[2]=rd(m,0x8c669ec8u,4); /* mov.l 0x8c669ec8,r2 */
 goto L8c669c2e;
L8c669c2e: r[0]<<=8; /* shll8 r0 */
 goto L8c669c30;
L8c669c30: r[12]=rd(m,0x8c669ec4u,4); /* mov.l 0x8c669ec4,r12 */
 goto L8c669c32;
L8c669c32: r[10]=r[0]; /* mov r0,r10 */
 goto L8c669c34;
L8c669c34: r[0]=sx(m,r[14]+0x2u,1); /* mov.b @(0x2,r14),r0 */
 goto L8c669c36;
L8c669c36: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c669c38;
L8c669c38: r[13]=rd(m,r[15]+0x28u,4); /* mov.l @(0x28,r15),r13 */
 goto L8c669c3a;
L8c669c3a: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669c3c;
L8c669c3c: r[9]=rd(m,r[15]+0x24u,4); /* mov.l @(0x24,r15),r9 */
 goto L8c669c3e;
L8c669c3e: r[0]<<=16; /* shll16 r0 */
 goto L8c669c40;
L8c669c40: r[11]=rd(m,0x8c669ec0u,4); /* mov.l 0x8c669ec0,r11 */
 goto L8c669c42;
L8c669c42: r[10]|=r[0]; /* or r0,r10 */
 goto L8c669c44;
L8c669c44: r[0]=sx(m,r[14]+0x3u,1); /* mov.b @(0x3,r14),r0 */
 goto L8c669c46;
L8c669c46: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669c48;
L8c669c48: r[0]<<=16; /* shll16 r0 */
 goto L8c669c4a;
L8c669c4a: r[0]<<=8; /* shll8 r0 */
 goto L8c669c4c;
L8c669c4c: r[10]|=r[0]; /* or r0,r10 */
 goto L8c669c4e;
L8c669c4e: r[10]|=r[3]; /* or r3,r10 */
 goto L8c669c50;
L8c669c50: *sr=(*sr&~1u)|(r[2]==r[10]); /* cmp/eq r2,r10 */
 goto L8c669c52;
L8c669c52: take=(*sr&1u);r[14]+=4u;if(take)goto L8c669c60;goto L8c669c56; /* bt/s 0x8c669c60 */
 goto L8c669c56;
L8c669c56: r[1]=rd(m,0x8c669eccu,4); /* mov.l 0x8c669ecc,r1 */
 goto L8c669c58;
L8c669c58: *sr=(*sr&~1u)|(r[1]==r[10]); /* cmp/eq r1,r10 */
 goto L8c669c5a;
L8c669c5a: if((*sr&1u))goto L8c669c60; /* bt 0x8c669c60 */
 goto L8c669c5c;
L8c669c5c: goto L8c669d60; /* bra 0x8c669d60 */
 goto L8c669c60;
L8c669c60: r[0]=sx(m,r[14]+0x1u,1); /* mov.b @(0x1,r14),r0 */
 goto L8c669c62;
L8c669c62: r[3]=sx(m,r[14],1); /* mov.b @r14,r3 */
 goto L8c669c64;
L8c669c64: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669c66;
L8c669c66: r[0]<<=8; /* shll8 r0 */
 goto L8c669c68;
L8c669c68: r[10]=r[0]; /* mov r0,r10 */
 goto L8c669c6a;
L8c669c6a: r[0]=sx(m,r[14]+0x2u,1); /* mov.b @(0x2,r14),r0 */
 goto L8c669c6c;
L8c669c6c: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c669c6e;
L8c669c6e: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669c70;
L8c669c70: r[0]<<=16; /* shll16 r0 */
 goto L8c669c72;
L8c669c72: r[10]|=r[0]; /* or r0,r10 */
 goto L8c669c74;
L8c669c74: r[0]=sx(m,r[14]+0x3u,1); /* mov.b @(0x3,r14),r0 */
 goto L8c669c76;
L8c669c76: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669c78;
L8c669c78: r[0]<<=16; /* shll16 r0 */
 goto L8c669c7a;
L8c669c7a: r[0]<<=8; /* shll8 r0 */
 goto L8c669c7c;
L8c669c7c: r[10]|=r[0]; /* or r0,r10 */
 goto L8c669c7e;
L8c669c7e: r[10]|=r[3]; /* or r3,r10 */
 goto L8c669c80;
L8c669c80: r[8]=r[10]; /* mov r10,r8 */
 goto L8c669c82;
L8c669c82: r[2]=r[10]; /* mov r10,r2 */
 goto L8c669c84;
L8c669c84: r[8]>>=16; /* shlr16 r8 */
 goto L8c669c86;
L8c669c86: r[3]=r[10]; /* mov r10,r3 */
 goto L8c669c88;
L8c669c88: r[2]>>=8; /* shlr8 r2 */
 goto L8c669c8a;
L8c669c8a: r[8]>>=8; /* shlr8 r8 */
 goto L8c669c8c;
L8c669c8c: r[2]&=r[11]; /* and r11,r2 */
 goto L8c669c8e;
L8c669c8e: r[8]=r[8]&255u; /* extu.b r8,r8 */
 goto L8c669c90;
L8c669c90: r[8]|=r[2]; /* or r2,r8 */
 goto L8c669c92;
L8c669c92: r[2]=r[10]; /* mov r10,r2 */
 goto L8c669c94;
L8c669c94: r[2]<<=8; /* shll8 r2 */
 goto L8c669c96;
L8c669c96: r[3]<<=16; /* shll16 r3 */
 goto L8c669c98;
L8c669c98: r[2]&=r[12]; /* and r12,r2 */
 goto L8c669c9a;
L8c669c9a: r[3]<<=8; /* shll8 r3 */
 goto L8c669c9c;
L8c669c9c: r[8]|=r[2]; /* or r2,r8 */
 goto L8c669c9e;
L8c669c9e: r[8]|=r[3]; /* or r3,r8 */
 goto L8c669ca0;
L8c669ca0: *sr=(*sr&~1u)|(r[8]>r[5]); /* cmp/hi r5,r8 */
 goto L8c669ca2;
L8c669ca2: take=(*sr&1u);r[14]+=4u;if(take)goto L8c669d60;goto L8c669ca6; /* bt/s 0x8c669d60 */
 goto L8c669ca6;
L8c669ca6: r[0]=sx(m,r[14]+0x1u,1); /* mov.b @(0x1,r14),r0 */
 goto L8c669ca8;
L8c669ca8: r[3]=sx(m,r[14],1); /* mov.b @r14,r3 */
 goto L8c669caa;
L8c669caa: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669cac;
L8c669cac: r[0]<<=8; /* shll8 r0 */
 goto L8c669cae;
L8c669cae: r[5]=r[0]; /* mov r0,r5 */
 goto L8c669cb0;
L8c669cb0: r[0]=sx(m,r[14]+0x2u,1); /* mov.b @(0x2,r14),r0 */
 goto L8c669cb2;
L8c669cb2: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c669cb4;
L8c669cb4: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669cb6;
L8c669cb6: r[0]<<=16; /* shll16 r0 */
 goto L8c669cb8;
L8c669cb8: r[5]|=r[0]; /* or r0,r5 */
 goto L8c669cba;
L8c669cba: r[0]=sx(m,r[14]+0x3u,1); /* mov.b @(0x3,r14),r0 */
 goto L8c669cbc;
L8c669cbc: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669cbe;
L8c669cbe: r[0]<<=16; /* shll16 r0 */
 goto L8c669cc0;
L8c669cc0: r[0]<<=8; /* shll8 r0 */
 goto L8c669cc2;
L8c669cc2: r[5]|=r[0]; /* or r0,r5 */
 goto L8c669cc4;
L8c669cc4: r[5]|=r[3]; /* or r3,r5 */
 goto L8c669cc6;
L8c669cc6: r[10]=r[5]; /* mov r5,r10 */
 goto L8c669cc8;
L8c669cc8: r[2]=r[5]; /* mov r5,r2 */
 goto L8c669cca;
L8c669cca: r[10]>>=16; /* shlr16 r10 */
 goto L8c669ccc;
L8c669ccc: r[3]=r[5]; /* mov r5,r3 */
 goto L8c669cce;
L8c669cce: r[2]>>=8; /* shlr8 r2 */
 goto L8c669cd0;
L8c669cd0: r[10]>>=8; /* shlr8 r10 */
 goto L8c669cd2;
L8c669cd2: r[2]&=r[11]; /* and r11,r2 */
 goto L8c669cd4;
L8c669cd4: r[10]=r[10]&255u; /* extu.b r10,r10 */
 goto L8c669cd6;
L8c669cd6: r[10]|=r[2]; /* or r2,r10 */
 goto L8c669cd8;
L8c669cd8: r[2]=r[5]; /* mov r5,r2 */
 goto L8c669cda;
L8c669cda: r[2]<<=8; /* shll8 r2 */
 goto L8c669cdc;
L8c669cdc: r[3]<<=16; /* shll16 r3 */
 goto L8c669cde;
L8c669cde: r[2]&=r[12]; /* and r12,r2 */
 goto L8c669ce0;
L8c669ce0: r[3]<<=8; /* shll8 r3 */
 goto L8c669ce2;
L8c669ce2: r[10]|=r[2]; /* or r2,r10 */
 goto L8c669ce4;
L8c669ce4: r[10]|=r[3]; /* or r3,r10 */
 goto L8c669ce6;
L8c669ce6: r[14]+=4u; /* add #0x4,r14 */
 goto L8c669ce8;
L8c669ce8: r[0]=sx(m,r[14]+0x1u,1); /* mov.b @(0x1,r14),r0 */
 goto L8c669cea;
L8c669cea: r[3]=sx(m,r[14],1); /* mov.b @r14,r3 */
 goto L8c669cec;
L8c669cec: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669cee;
L8c669cee: r[0]<<=8; /* shll8 r0 */
 goto L8c669cf0;
L8c669cf0: r[5]=r[0]; /* mov r0,r5 */
 goto L8c669cf2;
L8c669cf2: r[0]=sx(m,r[14]+0x2u,1); /* mov.b @(0x2,r14),r0 */
 goto L8c669cf4;
L8c669cf4: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c669cf6;
L8c669cf6: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669cf8;
L8c669cf8: r[0]<<=16; /* shll16 r0 */
 goto L8c669cfa;
L8c669cfa: r[5]|=r[0]; /* or r0,r5 */
 goto L8c669cfc;
L8c669cfc: r[0]=sx(m,r[14]+0x3u,1); /* mov.b @(0x3,r14),r0 */
 goto L8c669cfe;
L8c669cfe: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669d00;
L8c669d00: r[0]<<=16; /* shll16 r0 */
 goto L8c669d02;
L8c669d02: r[0]<<=8; /* shll8 r0 */
 goto L8c669d04;
L8c669d04: r[5]|=r[0]; /* or r0,r5 */
 goto L8c669d06;
L8c669d06: r[5]|=r[3]; /* or r3,r5 */
 goto L8c669d08;
L8c669d08: r[2]=r[5]; /* mov r5,r2 */
 goto L8c669d0a;
L8c669d0a: r[1]=r[5]; /* mov r5,r1 */
 goto L8c669d0c;
L8c669d0c: r[2]>>=16; /* shlr16 r2 */
 goto L8c669d0e;
L8c669d0e: r[3]=r[5]; /* mov r5,r3 */
 goto L8c669d10;
L8c669d10: r[1]>>=8; /* shlr8 r1 */
 goto L8c669d12;
L8c669d12: r[2]>>=8; /* shlr8 r2 */
 goto L8c669d14;
L8c669d14: r[1]&=r[11]; /* and r11,r1 */
 goto L8c669d16;
L8c669d16: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c669d18;
L8c669d18: r[2]|=r[1]; /* or r1,r2 */
 goto L8c669d1a;
L8c669d1a: r[1]=r[5]; /* mov r5,r1 */
 goto L8c669d1c;
L8c669d1c: r[1]<<=8; /* shll8 r1 */
 goto L8c669d1e;
L8c669d1e: r[3]<<=16; /* shll16 r3 */
 goto L8c669d20;
L8c669d20: r[1]&=r[12]; /* and r12,r1 */
 goto L8c669d22;
L8c669d22: r[3]<<=8; /* shll8 r3 */
 goto L8c669d24;
L8c669d24: r[2]|=r[1]; /* or r1,r2 */
 goto L8c669d26;
L8c669d26: r[5]=r[2]; /* mov r2,r5 */
 goto L8c669d28;
L8c669d28: r[5]|=r[3]; /* or r3,r5 */
 goto L8c669d2a;
L8c669d2a: r[0]=r[5]; /* mov r5,r0 */
 goto L8c669d2c;
L8c669d2c: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c669d2e;
L8c669d2e: take=(*sr&1u);r[14]+=4u;if(take)goto L8c669d3e;goto L8c669d32; /* bt/s 0x8c669d3e */
 goto L8c669d32;
L8c669d32: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c669d34;
L8c669d34: if((*sr&1u))goto L8c669d44; /* bt 0x8c669d44 */
 goto L8c669d36;
L8c669d36: *sr=(*sr&~1u)|(r[0]==0x00000003u); /* cmp/eq #0x3,r0 */
 goto L8c669d38;
L8c669d38: if((*sr&1u))goto L8c669d52; /* bt 0x8c669d52 */
 goto L8c669d3a;
L8c669d3a: goto L8c669d60; /* bra 0x8c669d60 */
 goto L8c669d3e;
L8c669d3e: r[2]=2u; /* mov #0x2,r2 */
 goto L8c669d40;
L8c669d40: wr(m,r[13],r[2],4);goto L8c669d48; /* bra 0x8c669d48 */
 goto L8c669d44;
L8c669d44: r[3]=1u; /* mov #0x1,r3 */
 goto L8c669d46;
L8c669d46: wr(m,r[13],r[3],4); /* mov.l r3,@r13 */
 goto L8c669d48;
L8c669d48: r[2]=rd(m,r[15]+0x20u,4); /* mov.l @(0x20,r15),r2 */
 goto L8c669d4a;
L8c669d4a: r[3]=8u; /* mov #0x8,r3 */
 goto L8c669d4c;
L8c669d4c: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c669d4e;
L8c669d4e: goto L8c669d64; /* bra 0x8c669d64 */
 goto L8c669d52;
L8c669d52: r[2]=16u; /* mov #0x10,r2 */
 goto L8c669d54;
L8c669d54: r[1]=0u; /* mov #0x0,r1 */
 goto L8c669d56;
L8c669d56: wr(m,r[13],r[1],4); /* mov.l r1,@r13 */
 goto L8c669d58;
L8c669d58: r[3]=rd(m,r[15]+0x20u,4); /* mov.l @(0x20,r15),r3 */
 goto L8c669d5a;
L8c669d5a: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c669d5c;
L8c669d5c: goto L8c669d64; /* bra 0x8c669d64 */
 goto L8c669d60;
L8c669d60: r[0]=0u;goto L8c669e26; /* bra 0x8c669e26 */
 goto L8c669d64;
L8c669d64: r[0]=sx(m,r[14]+0x1u,1); /* mov.b @(0x1,r14),r0 */
 goto L8c669d66;
L8c669d66: r[2]=sx(m,r[14],1); /* mov.b @r14,r2 */
 goto L8c669d68;
L8c669d68: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669d6a;
L8c669d6a: r[0]<<=8; /* shll8 r0 */
 goto L8c669d6c;
L8c669d6c: r[3]=r[0]; /* mov r0,r3 */
 goto L8c669d6e;
L8c669d6e: r[0]=sx(m,r[14]+0x2u,1); /* mov.b @(0x2,r14),r0 */
 goto L8c669d70;
L8c669d70: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c669d72;
L8c669d72: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669d74;
L8c669d74: r[0]<<=16; /* shll16 r0 */
 goto L8c669d76;
L8c669d76: r[3]|=r[0]; /* or r0,r3 */
 goto L8c669d78;
L8c669d78: r[0]=sx(m,r[14]+0x3u,1); /* mov.b @(0x3,r14),r0 */
 goto L8c669d7a;
L8c669d7a: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669d7c;
L8c669d7c: r[0]<<=16; /* shll16 r0 */
 goto L8c669d7e;
L8c669d7e: r[0]<<=8; /* shll8 r0 */
 goto L8c669d80;
L8c669d80: r[3]|=r[0]; /* or r0,r3 */
 goto L8c669d82;
L8c669d82: r[3]|=r[2]; /* or r2,r3 */
 goto L8c669d84;
L8c669d84: r[5]=r[3]; /* mov r3,r5 */
 goto L8c669d86;
L8c669d86: r[3]>>=16; /* shlr16 r3 */
 goto L8c669d88;
L8c669d88: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c669d8a;
L8c669d8a: r[1]=r[5]; /* mov r5,r1 */
 goto L8c669d8c;
L8c669d8c: r[3]>>=8; /* shlr8 r3 */
 goto L8c669d8e;
L8c669d8e: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r1 */
 goto L8c669d90;
L8c669d90: r[2]=r[5]; /* mov r5,r2 */
 goto L8c669d92;
L8c669d92: r[3]=(uint32_t)(int32_t)(int8_t)r[3]; /* exts.b r3,r3 */
 goto L8c669d94;
L8c669d94: r[1]&=r[11]; /* and r11,r1 */
 goto L8c669d96;
L8c669d96: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c669d98;
L8c669d98: r[3]|=r[1]; /* or r1,r3 */
 goto L8c669d9a;
L8c669d9a: r[1]=r[5]; /* mov r5,r1 */
 goto L8c669d9c;
L8c669d9c: r[1]<<=8; /* shll8 r1 */
 goto L8c669d9e;
L8c669d9e: r[1]&=r[12]; /* and r12,r1 */
 goto L8c669da0;
L8c669da0: r[2]<<=16; /* shll16 r2 */
 goto L8c669da2;
L8c669da2: r[3]|=r[1]; /* or r1,r3 */
 goto L8c669da4;
L8c669da4: r[14]+=4u; /* add #0x4,r14 */
 goto L8c669da6;
L8c669da6: r[2]<<=8; /* shll8 r2 */
 goto L8c669da8;
L8c669da8: r[3]|=r[2]; /* or r2,r3 */
 goto L8c669daa;
L8c669daa: wr(m,r[6],r[3],4); /* mov.l r3,@r6 */
 goto L8c669dac;
L8c669dac: r[0]=sx(m,r[14]+0x1u,1); /* mov.b @(0x1,r14),r0 */
 goto L8c669dae;
L8c669dae: r[2]=sx(m,r[14],1); /* mov.b @r14,r2 */
 goto L8c669db0;
L8c669db0: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669db2;
L8c669db2: r[0]<<=8; /* shll8 r0 */
 goto L8c669db4;
L8c669db4: r[3]=r[0]; /* mov r0,r3 */
 goto L8c669db6;
L8c669db6: r[0]=sx(m,r[14]+0x2u,1); /* mov.b @(0x2,r14),r0 */
 goto L8c669db8;
L8c669db8: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c669dba;
L8c669dba: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669dbc;
L8c669dbc: r[0]<<=16; /* shll16 r0 */
 goto L8c669dbe;
L8c669dbe: r[3]|=r[0]; /* or r0,r3 */
 goto L8c669dc0;
L8c669dc0: r[0]=sx(m,r[14]+0x3u,1); /* mov.b @(0x3,r14),r0 */
 goto L8c669dc2;
L8c669dc2: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669dc4;
L8c669dc4: r[0]<<=16; /* shll16 r0 */
 goto L8c669dc6;
L8c669dc6: r[0]<<=8; /* shll8 r0 */
 goto L8c669dc8;
L8c669dc8: r[3]|=r[0]; /* or r0,r3 */
 goto L8c669dca;
L8c669dca: r[3]|=r[2]; /* or r2,r3 */
 goto L8c669dcc;
L8c669dcc: r[5]=r[3]; /* mov r3,r5 */
 goto L8c669dce;
L8c669dce: r[3]>>=16; /* shlr16 r3 */
 goto L8c669dd0;
L8c669dd0: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c669dd2;
L8c669dd2: r[1]=r[5]; /* mov r5,r1 */
 goto L8c669dd4;
L8c669dd4: r[3]>>=8; /* shlr8 r3 */
 goto L8c669dd6;
L8c669dd6: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r1 */
 goto L8c669dd8;
L8c669dd8: r[3]=(uint32_t)(int32_t)(int8_t)r[3]; /* exts.b r3,r3 */
 goto L8c669dda;
L8c669dda: r[1]&=r[11]; /* and r11,r1 */
 goto L8c669ddc;
L8c669ddc: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c669dde;
L8c669dde: r[3]|=r[1]; /* or r1,r3 */
 goto L8c669de0;
L8c669de0: r[1]=r[5]; /* mov r5,r1 */
 goto L8c669de2;
L8c669de2: r[1]<<=8; /* shll8 r1 */
 goto L8c669de4;
L8c669de4: r[2]=r[5]; /* mov r5,r2 */
 goto L8c669de6;
L8c669de6: r[2]<<=16; /* shll16 r2 */
 goto L8c669de8;
L8c669de8: r[1]&=r[12]; /* and r12,r1 */
 goto L8c669dea;
L8c669dea: r[2]<<=8; /* shll8 r2 */
 goto L8c669dec;
L8c669dec: r[3]|=r[1]; /* or r1,r3 */
 goto L8c669dee;
L8c669dee: r[3]|=r[2]; /* or r2,r3 */
 goto L8c669df0;
L8c669df0: wr(m,r[7],r[3],4); /* mov.l r3,@r7 */
 goto L8c669df2;
L8c669df2: r[0]=rd(m,r[13],4); /* mov.l @r13,r0 */
 goto L8c669df4;
L8c669df4: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c669df6;
L8c669df6: if((*sr&1u))goto L8c669dfe; /* bt 0x8c669dfe */
 goto L8c669df8;
L8c669df8: r[0]=rd(m,r[13],4); /* mov.l @r13,r0 */
 goto L8c669dfa;
L8c669dfa: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c669dfc;
L8c669dfc: if(!(*sr&1u))goto L8c669e0a; /* bf 0x8c669e0a */
 goto L8c669dfe;
L8c669dfe: r[3]=rd(m,0x8c669ed0u,4); /* mov.l 0x8c669ed0,r3 */
 goto L8c669e00;
L8c669e00: r[1]=r[10]; /* mov r10,r1 */
 goto L8c669e02;
L8c669e02: target=r[3];*pr=0x8c669e06u;r[0]=rd(m,r[7],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c669e06;
L8c669e06: goto L8c669e1a; /* bra 0x8c669e1a */
 goto L8c669e0a;
L8c669e0a: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c669e0c;
L8c669e0c: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c669e0e;
L8c669e0e: if(!(*sr&1u))goto L8c669e1e; /* bf 0x8c669e1e */
 goto L8c669e10;
L8c669e10: r[2]=rd(m,0x8c669ed0u,4); /* mov.l 0x8c669ed0,r2 */
 goto L8c669e12;
L8c669e12: *sr=(*sr&~1u)|(r[10]&1u);r[10]>>=1; /* shlr r10 */
 goto L8c669e14;
L8c669e14: r[1]=r[10]; /* mov r10,r1 */
 goto L8c669e16;
L8c669e16: target=r[2];*pr=0x8c669e1au;r[0]=rd(m,r[7],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c669e1a;
L8c669e1a: wr(m,r[9],r[0],4);goto L8c669e22; /* bra 0x8c669e22 */
 goto L8c669e1e;
L8c669e1e: r[2]=rd(m,0x8c669ed4u,4); /* mov.l 0x8c669ed4,r2 */
 goto L8c669e20;
L8c669e20: wr(m,r[9],r[2],4); /* mov.l r2,@r9 */
 goto L8c669e22;
L8c669e22: r[0]=r[4]; /* mov r4,r0 */
 goto L8c669e24;
L8c669e24: r[0]+=r[8]; /* add r8,r0 */
 goto L8c669e26;
L8c669e26: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c669e28;
L8c669e28: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c669e2a;
L8c669e2a: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c669e2c;
L8c669e2c: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c669e2e;
L8c669e2e: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c669e30;
L8c669e30: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c669e32;
L8c669e32: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c669e34;
L8c669e34: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
