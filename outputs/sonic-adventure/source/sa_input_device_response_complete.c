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
/* Static C recovery of retail Tikal helper 8c65fe14..8c65ff12.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_response_complete(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65fe14;
L8c65fe14: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65fe16;
L8c65fe16: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65fe18;
L8c65fe18: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65fe1a;
L8c65fe1a: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65fe1c;
L8c65fe1c: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65fe1e;
L8c65fe1e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65fe20;
L8c65fe20: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c65fe22;
L8c65fe22: r[13]=rd(m,0x8c65ff4cu,4); /* mov.l 0x8c65ff4c,r13 */
 goto L8c65fe24;
L8c65fe24: r[2]=r[5]; /* mov r5,r2 */
 goto L8c65fe26;
L8c65fe26: r[2]<<=2; /* shll2 r2 */
 goto L8c65fe28;
L8c65fe28: r[1]=sx(m,0x8c65ff46u,2); /* mov.w 0x8c65ff46,r1 */
 goto L8c65fe2a;
L8c65fe2a: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c65fe2c;
L8c65fe2c: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c65fe2e;
L8c65fe2e: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c65fe30;
L8c65fe30: r[3]+=4u; /* add #0x4,r3 */
 goto L8c65fe32;
L8c65fe32: r[2]+=r[3]; /* add r3,r2 */
 goto L8c65fe34;
L8c65fe34: r[3]=44u; /* mov #0x2c,r3 */
 goto L8c65fe36;
L8c65fe36: wr(m,r[15]+0x4u,r[2],4); /* mov.l r2,@(0x4,r15) */
 goto L8c65fe38;
L8c65fe38: *macl=r[3]*r[4]; /* mul.l r3,r4 */
 goto L8c65fe3a;
L8c65fe3a: r[14]=rd(m,r[13],4); /* mov.l @r13,r14 */
 goto L8c65fe3c;
L8c65fe3c: r[2]=sx(m,0x8c65ff44u,2); /* mov.w 0x8c65ff44,r2 */
 goto L8c65fe3e;
L8c65fe3e: r[14]+=40u; /* add #0x28,r14 */
 goto L8c65fe40;
L8c65fe40: r[10]=rd(m,r[13],4); /* mov.l @r13,r10 */
 goto L8c65fe42;
L8c65fe42: r[3]=*macl; /* sts MACL,r3 */
 goto L8c65fe44;
L8c65fe44: r[10]+=r[2]; /* add r2,r10 */
 goto L8c65fe46;
L8c65fe46: r[0]=rd(m,r[13],4); /* mov.l @r13,r0 */
 goto L8c65fe48;
L8c65fe48: r[14]+=r[3]; /* add r3,r14 */
 goto L8c65fe4a;
L8c65fe4a: r[3]=120u; /* mov #0x78,r3 */
 goto L8c65fe4c;
L8c65fe4c: *macl=r[3]*r[4]; /* mul.l r3,r4 */
 goto L8c65fe4e;
L8c65fe4e: r[3]=*macl; /* sts MACL,r3 */
 goto L8c65fe50;
L8c65fe50: r[10]+=r[3]; /* add r3,r10 */
 goto L8c65fe52;
L8c65fe52: r[3]=r[0]; /* mov r0,r3 */
 goto L8c65fe54;
L8c65fe54: r[1]+=r[3]; /* add r3,r1 */
 goto L8c65fe56;
L8c65fe56: r[1]=rd(m,r[1],4); /* mov.l @r1,r1 */
 goto L8c65fe58;
L8c65fe58: r[1]=0u-r[1]; /* neg r1,r1 */
 goto L8c65fe5a;
L8c65fe5a: r[1]+=1u; /* add #0x1,r1 */
 goto L8c65fe5c;
L8c65fe5c: r[3]=r[1]; /* mov r1,r3 */
 goto L8c65fe5e;
L8c65fe5e: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c65fe60;
L8c65fe60: r[1]+=r[3]; /* add r3,r1 */
 goto L8c65fe62;
L8c65fe62: r[3]=sx(m,0x8c65ff48u,2); /* mov.w 0x8c65ff48,r3 */
 goto L8c65fe64;
L8c65fe64: r[1]<<=2; /* shll2 r1 */
 goto L8c65fe66;
L8c65fe66: r[11]=r[4]; /* mov r4,r11 */
 goto L8c65fe68;
L8c65fe68: r[1]<<=2; /* shll2 r1 */
 goto L8c65fe6a;
L8c65fe6a: r[11]<<=2; /* shll2 r11 */
 goto L8c65fe6c;
L8c65fe6c: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c65fe6e;
L8c65fe6e: wr(m,r[15]+0x8u,r[11],4); /* mov.l r11,@(0x8,r15) */
 goto L8c65fe70;
L8c65fe70: r[0]+=r[3]; /* add r3,r0 */
 goto L8c65fe72;
L8c65fe72: r[0]+=r[1]; /* add r1,r0 */
 goto L8c65fe74;
L8c65fe74: r[11]=rd(m,r[11]+r[0],4); /* mov.l @(r0,r11),r11 */
 goto L8c65fe76;
L8c65fe76: r[0]=sx(m,r[14],1); /* mov.b @r14,r0 */
 goto L8c65fe78;
L8c65fe78: wr(m,r[14]+0x1u,r[0],1); /* mov.b r0,@(0x1,r14) */
 goto L8c65fe7a;
L8c65fe7a: r[0]=sx(m,r[14]+0x4u,1); /* mov.b @(0x4,r14),r0 */
 goto L8c65fe7c;
L8c65fe7c: wr(m,r[14]+0x5u,r[0],1); /* mov.b r0,@(0x5,r14) */
 goto L8c65fe7e;
L8c65fe7e: r[1]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r1 */
 goto L8c65fe80;
L8c65fe80: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c65fe82;
L8c65fe82: take=(*sr&1u);r[12]=0u;if(take)goto L8c65fe8c;goto L8c65fe86; /* bt/s 0x8c65fe8c */
 goto L8c65fe86;
L8c65fe86: r[1]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r1 */
 goto L8c65fe88;
L8c65fe88: wr(m,r[14]+0x20u,r[1],4); /* mov.l r1,@(0x20,r14) */
 goto L8c65fe8a;
L8c65fe8a: wr(m,r[14]+0x1cu,r[12],4); /* mov.l r12,@(0x1c,r14) */
 goto L8c65fe8c;
L8c65fe8c: r[0]=rd(m,0x8c65ff50u,4); /* mov.l 0x8c65ff50,r0 */
 goto L8c65fe8e;
L8c65fe8e: r[2]=31u; /* mov #0x1f,r2 */
 goto L8c65fe90;
L8c65fe90: r[3]=sx(m,r[4]+r[0],1); /* mov.b @(r0,r4),r3 */
 goto L8c65fe92;
L8c65fe92: r[3]&=r[2]; /* and r2,r3 */
 goto L8c65fe94;
L8c65fe94: r[4]=r[3]; /* mov r3,r4 */
 goto L8c65fe96;
L8c65fe96: r[1]=r[4]&255u; /* extu.b r4,r1 */
 goto L8c65fe98;
L8c65fe98: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c65fe9a;
L8c65fe9a: if((*sr&1u))goto L8c65feaa; /* bt 0x8c65feaa */
 goto L8c65fe9c;
L8c65fe9c: r[6]=r[10]; /* mov r10,r6 */
 goto L8c65fe9e;
L8c65fe9e: r[7]=r[14]; /* mov r14,r7 */
 goto L8c65fea0;
L8c65fea0: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65fea2;
L8c65fea2: *pr=0x8c65fea6u;r[5]=rd(m,r[15]+0x8u,4);status=call(ctx,0x8c66005cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c66005c */
 goto L8c65fea6;
L8c65fea6: r[15]+=4u;goto L8c65feb4; /* bra 0x8c65feb4 */
 goto L8c65feaa;
L8c65feaa: r[5]=r[10]; /* mov r10,r5 */
 goto L8c65feac;
L8c65feac: r[6]=r[14]; /* mov r14,r6 */
 goto L8c65feae;
L8c65feae: r[7]=r[11]; /* mov r11,r7 */
 goto L8c65feb0;
L8c65feb0: *pr=0x8c65feb4u;r[4]=rd(m,r[15],4);status=call(ctx,0x8c65ffb8u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65ffb8 */
 goto L8c65feb4;
L8c65feb4: r[5]=r[0]; /* mov r0,r5 */
 goto L8c65feb6;
L8c65feb6: r[0]=sx(m,r[14]+0x7u,1); /* mov.b @(0x7,r14),r0 */
 goto L8c65feb8;
L8c65feb8: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c65feba;
L8c65feba: if((*sr&1u))goto L8c65fee0; /* bt 0x8c65fee0 */
 goto L8c65febc;
L8c65febc: r[2]=rd(m,r[13],4); /* mov.l @r13,r2 */
 goto L8c65febe;
L8c65febe: r[0]=sx(m,0x8c65ff46u,2); /* mov.w 0x8c65ff46,r0 */
 goto L8c65fec0;
L8c65fec0: r[3]=r[2]; /* mov r2,r3 */
 goto L8c65fec2;
L8c65fec2: r[1]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r1 */
 goto L8c65fec4;
L8c65fec4: r[0]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r0 */
 goto L8c65fec6;
L8c65fec6: r[1]=0u-r[1]; /* neg r1,r1 */
 goto L8c65fec8;
L8c65fec8: r[1]+=1u; /* add #0x1,r1 */
 goto L8c65feca;
L8c65feca: r[3]=r[1]; /* mov r1,r3 */
 goto L8c65fecc;
L8c65fecc: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c65fece;
L8c65fece: r[1]+=r[3]; /* add r3,r1 */
 goto L8c65fed0;
L8c65fed0: r[3]=sx(m,0x8c65ff48u,2); /* mov.w 0x8c65ff48,r3 */
 goto L8c65fed2;
L8c65fed2: r[1]<<=2; /* shll2 r1 */
 goto L8c65fed4;
L8c65fed4: r[1]<<=2; /* shll2 r1 */
 goto L8c65fed6;
L8c65fed6: r[2]+=r[3]; /* add r3,r2 */
 goto L8c65fed8;
L8c65fed8: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c65feda;
L8c65feda: r[1]+=r[2]; /* add r2,r1 */
 goto L8c65fedc;
L8c65fedc: r[2]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r2 */
 goto L8c65fede;
L8c65fede: wr(m,r[14]+0x10u,r[2],4); /* mov.l r2,@(0x10,r14) */
 goto L8c65fee0;
L8c65fee0: r[0]=sx(m,r[14]+0xdu,1); /* mov.b @(0xd,r14),r0 */
 goto L8c65fee2;
L8c65fee2: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c65fee4;
L8c65fee4: if((*sr&1u))goto L8c65feec; /* bt 0x8c65feec */
 goto L8c65fee6;
L8c65fee6: r[0]=r[12]; /* mov r12,r0 */
 goto L8c65fee8;
L8c65fee8: wr(m,r[14]+0xcu,r[0],1); /* mov.b r0,@(0xc,r14) */
 goto L8c65feea;
L8c65feea: wr(m,r[14]+0xdu,r[0],1); /* mov.b r0,@(0xd,r14) */
 goto L8c65feec;
L8c65feec: r[0]=sx(m,r[14]+0x2u,1); /* mov.b @(0x2,r14),r0 */
 goto L8c65feee;
L8c65feee: r[3]=r[0]; /* mov r0,r3 */
 goto L8c65fef0;
L8c65fef0: r[0]=sx(m,r[14]+0xcu,1); /* mov.b @(0xc,r14),r0 */
 goto L8c65fef2;
L8c65fef2: r[0]|=r[3]; /* or r3,r0 */
 goto L8c65fef4;
L8c65fef4: wr(m,r[14]+0xcu,r[0],1); /* mov.b r0,@(0xc,r14) */
 goto L8c65fef6;
L8c65fef6: r[0]=sx(m,r[14]+0x3u,1); /* mov.b @(0x3,r14),r0 */
 goto L8c65fef8;
L8c65fef8: r[3]=r[0]; /* mov r0,r3 */
 goto L8c65fefa;
L8c65fefa: r[0]=sx(m,r[14]+0xcu,1); /* mov.b @(0xc,r14),r0 */
 goto L8c65fefc;
L8c65fefc: r[0]|=r[3]; /* or r3,r0 */
 goto L8c65fefe;
L8c65fefe: wr(m,r[14]+0xcu,r[0],1); /* mov.b r0,@(0xc,r14) */
 goto L8c65ff00;
L8c65ff00: r[0]=r[5]; /* mov r5,r0 */
 goto L8c65ff02;
L8c65ff02: r[15]+=12u; /* add #0xc,r15 */
 goto L8c65ff04;
L8c65ff04: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65ff06;
L8c65ff06: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65ff08;
L8c65ff08: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65ff0a;
L8c65ff0a: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65ff0c;
L8c65ff0c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65ff0e;
L8c65ff0e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
