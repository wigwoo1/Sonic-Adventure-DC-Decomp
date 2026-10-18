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
/* Static C recovery of retail Tikal helper 8c647f4c..8c6480a2.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_system_configure(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c647f4c;
L8c647f4c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c647f4e;
L8c647f4e: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c647f50;
L8c647f50: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c647f52;
L8c647f52: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c647f54;
L8c647f54: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c647f56;
L8c647f56: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c647f58;
L8c647f58: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c647f5a;
L8c647f5a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c647f5c;
L8c647f5c: r[15]+=4294967276u; /* add #-0x14,r15 */
 goto L8c647f5e;
L8c647f5e: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c647f60;
L8c647f60: r[0]=r[4]; /* mov r4,r0 */
 goto L8c647f62;
L8c647f62: r[3]=rd(m,0x8c647fc8u,4); /* mov.l 0x8c647fc8,r3 */
 goto L8c647f64;
L8c647f64: *sr=(*sr&~1u)|(r[0]==0xfu); /* cmp/eq #0xf,r0 */
 goto L8c647f66;
L8c647f66: r[5]=rd(m,0x8c647fd4u,4); /* mov.l 0x8c647fd4,r5 */
 goto L8c647f68;
L8c647f68: r[10]=1u; /* mov #0x1,r10 */
 goto L8c647f6a;
L8c647f6a: r[14]=rd(m,0x8c647fccu,4); /* mov.l 0x8c647fcc,r14 */
 goto L8c647f6c;
L8c647f6c: r[13]=0u; /* mov #0x0,r13 */
 goto L8c647f6e;
L8c647f6e: r[12]=rd(m,0x8c647fd0u,4); /* mov.l 0x8c647fd0,r12 */
 goto L8c647f70;
L8c647f70: take=(*sr&1u);r[7]=rd(m,r[3],4);if(take)goto L8c647f84;goto L8c647f74; /* bt/s 0x8c647f84 */
 goto L8c647f74;
L8c647f74: *sr=(*sr&~1u)|(r[0]==0x21u); /* cmp/eq #0x21,r0 */
 goto L8c647f76;
L8c647f76: if((*sr&1u))goto L8c647f8c; /* bt 0x8c647f8c */
 goto L8c647f78;
L8c647f78: *sr=(*sr&~1u)|(r[0]==0x41u); /* cmp/eq #0x41,r0 */
 goto L8c647f7a;
L8c647f7a: if((*sr&1u))goto L8c647f96; /* bt 0x8c647f96 */
 goto L8c647f7c;
L8c647f7c: *sr=(*sr&~1u)|(r[0]==0x7au); /* cmp/eq #0x7a,r0 */
 goto L8c647f7e;
L8c647f7e: if((*sr&1u))goto L8c647fa0; /* bt 0x8c647fa0 */
 goto L8c647f80;
L8c647f80: goto L8c647fac; /* bra 0x8c647fac */
 goto L8c647f84;
L8c647f84: wr(m,r[7]+0x4u,r[13],4); /* mov.l r13,@(0x4,r7) */
 goto L8c647f86;
L8c647f86: r[8]=rd(m,r[12],4); /* mov.l @r12,r8 */
 goto L8c647f88;
L8c647f88: r[2]=rd(m,r[5],4);goto L8c647f92; /* bra 0x8c647f92 */
 goto L8c647f8c;
L8c647f8c: wr(m,r[7]+0x4u,r[10],4); /* mov.l r10,@(0x4,r7) */
 goto L8c647f8e;
L8c647f8e: r[2]=rd(m,r[5]+0x4u,4); /* mov.l @(0x4,r5),r2 */
 goto L8c647f90;
L8c647f90: r[8]=rd(m,r[12]+0x4u,4); /* mov.l @(0x4,r12),r8 */
 goto L8c647f92;
L8c647f92: wr(m,r[15]+0x10u,r[2],4);goto L8c647fd8; /* bra 0x8c647fd8 */
 goto L8c647f96;
L8c647f96: r[2]=2u; /* mov #0x2,r2 */
 goto L8c647f98;
L8c647f98: wr(m,r[7]+0x4u,r[2],4); /* mov.l r2,@(0x4,r7) */
 goto L8c647f9a;
L8c647f9a: r[8]=rd(m,r[12]+0x8u,4); /* mov.l @(0x8,r12),r8 */
 goto L8c647f9c;
L8c647f9c: r[1]=rd(m,r[5]+0x8u,4);goto L8c647fa8; /* bra 0x8c647fa8 */
 goto L8c647fa0;
L8c647fa0: r[2]=3u; /* mov #0x3,r2 */
 goto L8c647fa2;
L8c647fa2: wr(m,r[7]+0x4u,r[2],4); /* mov.l r2,@(0x4,r7) */
 goto L8c647fa4;
L8c647fa4: r[8]=rd(m,r[12]+0xcu,4); /* mov.l @(0xc,r12),r8 */
 goto L8c647fa6;
L8c647fa6: r[1]=rd(m,r[5]+0xcu,4); /* mov.l @(0xc,r5),r1 */
 goto L8c647fa8;
L8c647fa8: wr(m,r[15]+0x10u,r[1],4);goto L8c647fd8; /* bra 0x8c647fd8 */
 goto L8c647fac;
L8c647fac: r[0]=sx(m,0x8c647fb2u,2); /* mov.w 0x8c647fb2,r0 */
 goto L8c647fae;
L8c647fae: goto L8c64808e; /* bra 0x8c64808e */
 goto L8c647fd8;
L8c647fd8: r[3]=r[8]; /* mov r8,r3 */
 goto L8c647fda;
L8c647fda: r[4]<<=8; /* shll8 r4 */
 goto L8c647fdc;
L8c647fdc: r[3]+=1u; /* add #0x1,r3 */
 goto L8c647fde;
L8c647fde: r[12]=rd(m,0x8c6480e0u,4); /* mov.l 0x8c6480e0,r12 */
 goto L8c647fe0;
L8c647fe0: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c647fe2;
L8c647fe2: r[11]=r[6]; /* mov r6,r11 */
 goto L8c647fe4;
L8c647fe4: r[4]+=32u; /* add #0x20,r4 */
 goto L8c647fe6;
L8c647fe6: r[9]=r[13]; /* mov r13,r9 */
 goto L8c647fe8;
L8c647fe8: r[3]<<=8; /* shll8 r3 */
 goto L8c647fea;
L8c647fea: wr(m,r[15]+0x8u,r[4],4); /* mov.l r4,@(0x8,r15) */
 goto L8c647fec;
L8c647fec: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c647fee;
L8c647fee: wr(m,r[15],r[10],4); /* mov.l r10,@r15 */
 goto L8c647ff0;
L8c647ff0: wr(m,r[15]+0xcu,r[3],4);goto L8c648086; /* bra 0x8c648086 */
 goto L8c647ff4;
L8c647ff4: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c647ff6;
L8c647ff6: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c647ff8;
L8c647ff8: *sr=(*sr&~1u)|!(r[3]&r[2]); /* tst r3,r2 */
 goto L8c647ffa;
L8c647ffa: if((*sr&1u))goto L8c64807a; /* bt 0x8c64807a */
 goto L8c647ffc;
L8c647ffc: r[3]=76u; /* mov #0x4c,r3 */
 goto L8c647ffe;
L8c647ffe: r[2]=rd(m,0x8c6480e4u,4); /* mov.l 0x8c6480e4,r2 */
 goto L8c648000;
L8c648000: *macl=r[3]*r[9]; /* mul.l r3,r9 */
 goto L8c648002;
L8c648002: r[0]=r[10]; /* mov r10,r0 */
 goto L8c648004;
L8c648004: wr(m,r[14]+0x2u,r[0],1); /* mov.b r0,@(0x2,r14) */
 goto L8c648006;
L8c648006: r[0]=r[13]; /* mov r13,r0 */
 goto L8c648008;
L8c648008: r[1]=sx(m,0x8c6480d8u,2); /* mov.w 0x8c6480d8,r1 */
 goto L8c64800a;
L8c64800a: wr(m,r[14],r[9],2); /* mov.w r9,@r14 */
 goto L8c64800c;
L8c64800c: r[3]=*macl; /* sts MACL,r3 */
 goto L8c64800e;
L8c64800e: r[1]+=r[11]; /* add r11,r1 */
 goto L8c648010;
L8c648010: r[3]+=r[2]; /* add r2,r3 */
 goto L8c648012;
L8c648012: wr(m,r[14]+0x1cu,r[3],4); /* mov.l r3,@(0x1c,r14) */
 goto L8c648014;
L8c648014: r[3]=r[11]; /* mov r11,r3 */
 goto L8c648016;
L8c648016: wr(m,r[14]+0xau,r[0],1); /* mov.b r0,@(0xa,r14) */
 goto L8c648018;
L8c648018: r[3]+=32u; /* add #0x20,r3 */
 goto L8c64801a;
L8c64801a: wr(m,r[14]+0xbu,r[0],1); /* mov.b r0,@(0xb,r14) */
 goto L8c64801c;
L8c64801c: wr(m,r[14]+0xcu,r[0],1); /* mov.b r0,@(0xc,r14) */
 goto L8c64801e;
L8c64801e: r[0]=r[13]; /* mov r13,r0 */
 goto L8c648020;
L8c648020: wr(m,r[14]+0x2cu,r[11],4); /* mov.l r11,@(0x2c,r14) */
 goto L8c648022;
L8c648022: wr(m,r[14]+0x20u,r[3],4); /* mov.l r3,@(0x20,r14) */
 goto L8c648024;
L8c648024: wr(m,r[14]+0x24u,r[1],4); /* mov.l r1,@(0x24,r14) */
 goto L8c648026;
L8c648026: r[3]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r3 */
 goto L8c648028;
L8c648028: r[3]+=r[11]; /* add r11,r3 */
 goto L8c64802a;
L8c64802a: r[3]+=32u; /* add #0x20,r3 */
 goto L8c64802c;
L8c64802c: wr(m,r[14]+0x28u,r[3],4); /* mov.l r3,@(0x28,r14) */
 goto L8c64802e;
L8c64802e: wr(m,r[14]+0x8u,r[0],1); /* mov.b r0,@(0x8,r14) */
 goto L8c648030;
L8c648030: wr(m,r[14]+0x9u,r[0],1); /* mov.b r0,@(0x9,r14) */
 goto L8c648032;
L8c648032: r[0]=r[10]; /* mov r10,r0 */
 goto L8c648034;
L8c648034: wr(m,r[14]+0xeu,r[0],2); /* mov.w r0,@(0xe,r14) */
 goto L8c648036;
L8c648036: r[0]=r[8]; /* mov r8,r0 */
 goto L8c648038;
L8c648038: wr(m,r[14]+0x10u,r[0],2); /* mov.w r0,@(0x10,r14) */
 goto L8c64803a;
L8c64803a: r[0]=sx(m,r[15]+0x10u,2); /* mov.w @(0x10,r15),r0 */
 goto L8c64803c;
L8c64803c: wr(m,r[14]+0x12u,r[0],2); /* mov.w r0,@(0x12,r14) */
 goto L8c64803e;
L8c64803e: r[0]=32u; /* mov #0x20,r0 */
 goto L8c648040;
L8c648040: r[5]=0u; /* mov #0x0,r5 */
 goto L8c648042;
L8c648042: r[4]=r[14]; /* mov r14,r4 */
 goto L8c648044;
L8c648044: wr(m,r[14]+0x14u,r[0],2); /* mov.w r0,@(0x14,r14) */
 goto L8c648046;
L8c648046: r[6]=16u; /* mov #0x10,r6 */
 goto L8c648048;
L8c648048: target=r[12];*pr=0x8c64804cu;r[4]+=56u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c64804c;
L8c64804c: r[6]=sx(m,0x8c6480dau,2); /* mov.w 0x8c6480da,r6 */
 goto L8c64804e;
L8c64804e: r[4]=r[14]; /* mov r14,r4 */
 goto L8c648050;
L8c648050: r[5]=0u; /* mov #0x0,r5 */
 goto L8c648052;
L8c648052: target=r[12];*pr=0x8c648056u;r[4]+=72u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c648056;
L8c648056: r[5]=71u; /* mov #0x47,r5 */
 goto L8c648058;
L8c648058: r[6]=32u; /* mov #0x20,r6 */
 goto L8c64805a;
L8c64805a: target=r[12];*pr=0x8c64805eu;r[4]=rd(m,r[14]+0x2cu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c64805e;
L8c64805e: r[5]=83u; /* mov #0x53,r5 */
 goto L8c648060;
L8c648060: r[6]=32u; /* mov #0x20,r6 */
 goto L8c648062;
L8c648062: target=r[12];*pr=0x8c648066u;r[4]=rd(m,r[14]+0x20u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c648066;
L8c648066: r[5]=70u; /* mov #0x46,r5 */
 goto L8c648068;
L8c648068: r[6]=32u; /* mov #0x20,r6 */
 goto L8c64806a;
L8c64806a: target=r[12];*pr=0x8c64806eu;r[4]=rd(m,r[14]+0x24u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c64806e;
L8c64806e: r[5]=68u; /* mov #0x44,r5 */
 goto L8c648070;
L8c648070: r[6]=32u; /* mov #0x20,r6 */
 goto L8c648072;
L8c648072: target=r[12];*pr=0x8c648076u;r[4]=rd(m,r[14]+0x28u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c648076;
L8c648076: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c648078;
L8c648078: r[11]+=r[3]; /* add r3,r11 */
 goto L8c64807a;
L8c64807a: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c64807c;
L8c64807c: r[9]+=1u; /* add #0x1,r9 */
 goto L8c64807e;
L8c64807e: r[2]=sx(m,0x8c6480dcu,2); /* mov.w 0x8c6480dc,r2 */
 goto L8c648080;
L8c648080: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c648082;
L8c648082: r[14]+=r[2]; /* add r2,r14 */
 goto L8c648084;
L8c648084: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c648086;
L8c648086: r[3]=8u; /* mov #0x8,r3 */
 goto L8c648088;
L8c648088: *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[3]); /* cmp/ge r3,r9 */
 goto L8c64808a;
L8c64808a: if(!(*sr&1u))goto L8c647ff4; /* bf 0x8c647ff4 */
 goto L8c64808c;
L8c64808c: r[0]=0u; /* mov #0x0,r0 */
 goto L8c64808e;
L8c64808e: r[15]+=20u; /* add #0x14,r15 */
 goto L8c648090;
L8c648090: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c648092;
L8c648092: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c648094;
L8c648094: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c648096;
L8c648096: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c648098;
L8c648098: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c64809a;
L8c64809a: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c64809c;
L8c64809c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c64809e;
L8c64809e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
