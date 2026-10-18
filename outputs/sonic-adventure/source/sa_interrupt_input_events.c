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
/* Static C recovery of retail Tikal helper 8c602cb8..8c602dda.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_interrupt_input_events(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c602cb8;
L8c602cb8: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c602cba;
L8c602cba: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c602cbc;
L8c602cbc: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c602cbe;
L8c602cbe: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c602cc0;
L8c602cc0: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c602cc2;
L8c602cc2: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c602cc4;
L8c602cc4: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c602cc6;
L8c602cc6: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c602cc8;
L8c602cc8: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c602cca;
L8c602cca: r[2]=rd(m,0x8c602f00u,4); /* mov.l 0x8c602f00,r2 */
 goto L8c602ccc;
L8c602ccc: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c602cce;
L8c602cce: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c602cd0;
L8c602cd0: if((*sr&1u))goto L8c602dc6; /* bt 0x8c602dc6 */
 goto L8c602cd2;
L8c602cd2: r[2]=rd(m,0x8c602f04u,4); /* mov.l 0x8c602f04,r2 */
 goto L8c602cd4;
L8c602cd4: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c602cd6;
L8c602cd6: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c602cd8;
L8c602cd8: if((*sr&1u))goto L8c602ce0; /* bt 0x8c602ce0 */
 goto L8c602cda;
L8c602cda: r[9]=rd(m,0x8c602f08u,4); /* mov.l 0x8c602f08,r9 */
 goto L8c602cdc;
L8c602cdc: goto L8c602ce2; /* bra 0x8c602ce2 */
 goto L8c602ce0;
L8c602ce0: r[9]=rd(m,0x8c602f0cu,4); /* mov.l 0x8c602f0c,r9 */
 goto L8c602ce2;
L8c602ce2: r[2]=rd(m,0x8c602f10u,4); /* mov.l 0x8c602f10,r2 */
 goto L8c602ce4;
L8c602ce4: target=r[2];*pr=0x8c602ce8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c602ce8;
L8c602ce8: r[8]=rd(m,0x8c602f14u,4); /* mov.l 0x8c602f14,r8 */
 goto L8c602cea;
L8c602cea: r[1]=rd(m,0x8c602f1cu,4); /* mov.l 0x8c602f1c,r1 */
 goto L8c602cec;
L8c602cec: r[2]=rd(m,0x8c602f18u,4); /* mov.l 0x8c602f18,r2 */
 goto L8c602cee;
L8c602cee: r[3]=rd(m,r[8],4); /* mov.l @r8,r3 */
 goto L8c602cf0;
L8c602cf0: target=r[1];*pr=0x8c602cf4u;wr(m,r[2],r[3],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c602cf4;
L8c602cf4: r[10]=0u; /* mov #0x0,r10 */
 goto L8c602cf6;
L8c602cf6: wr(m,r[8],r[0],4); /* mov.l r0,@r8 */
 goto L8c602cf8;
L8c602cf8: r[14]=r[10]; /* mov r10,r14 */
 goto L8c602cfa;
L8c602cfa: r[13]=1u; /* mov #0x1,r13 */
 goto L8c602cfc;
L8c602cfc: r[12]=8u; /* mov #0x8,r12 */
 goto L8c602cfe;
L8c602cfe: *pr=0x8c602d02u;r[4]=r[14];status=call(ctx,0x8c6033e4u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6033e4 */
 goto L8c602d02;
L8c602d02: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c602d04;
L8c602d04: if(!(*sr&1u))goto L8c602d0a; /* bf 0x8c602d0a */
 goto L8c602d06;
L8c602d06: r[3]=r[13];goto L8c602d0c; /* bra 0x8c602d0c */
 goto L8c602d0a;
L8c602d0a: r[3]=0u; /* mov #0x0,r3 */
 goto L8c602d0c;
L8c602d0c: r[14]+=1u; /* add #0x1,r14 */
 goto L8c602d0e;
L8c602d0e: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1; /* shll r13 */
 goto L8c602d10;
L8c602d10: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[12]); /* cmp/ge r12,r14 */
 goto L8c602d12;
L8c602d12: r[10]|=r[3]; /* or r3,r10 */
 goto L8c602d14;
L8c602d14: if(!(*sr&1u))goto L8c602cfe; /* bf 0x8c602cfe */
 goto L8c602d16;
L8c602d16: r[1]=rd(m,0x8c602f18u,4); /* mov.l 0x8c602f18,r1 */
 goto L8c602d18;
L8c602d18: r[13]=1u; /* mov #0x1,r13 */
 goto L8c602d1a;
L8c602d1a: r[3]=rd(m,r[8],4); /* mov.l @r8,r3 */
 goto L8c602d1c;
L8c602d1c: r[11]=0u; /* mov #0x0,r11 */
 goto L8c602d1e;
L8c602d1e: r[2]=rd(m,r[1],4); /* mov.l @r1,r2 */
 goto L8c602d20;
L8c602d20: r[0]=r[3]; /* mov r3,r0 */
 goto L8c602d22;
L8c602d22: r[14]=rd(m,0x8c602f20u,4); /* mov.l 0x8c602f20,r14 */
 goto L8c602d24;
L8c602d24: r[2]^=r[3]; /* xor r3,r2 */
 goto L8c602d26;
L8c602d26: r[2]&=r[0]; /* and r0,r2 */
 goto L8c602d28;
L8c602d28: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto L8c602d2a;
L8c602d2a: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c602d2c;
L8c602d2c: *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c602d2e;
L8c602d2e: if((*sr&1u))goto L8c602d4a; /* bt 0x8c602d4a */
 goto L8c602d30;
L8c602d30: r[1]=r[13]; /* mov r13,r1 */
 goto L8c602d32;
L8c602d32: *sr=(*sr&~1u)|!(r[10]&r[1]); /* tst r10,r1 */
 goto L8c602d34;
L8c602d34: if((*sr&1u))goto L8c602d44; /* bt 0x8c602d44 */
 goto L8c602d36;
L8c602d36: target=r[9];*pr=0x8c602d3au;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto L8c602d3a;
L8c602d3a: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c602d3c;
L8c602d3c: r[2]=~r[13]; /* not r13,r2 */
 goto L8c602d3e;
L8c602d3e: r[3]&=r[2]; /* and r2,r3 */
 goto L8c602d40;
L8c602d40: wr(m,r[14],r[3],4);goto L8c602d4a; /* bra 0x8c602d4a */
 goto L8c602d44;
L8c602d44: r[1]=rd(m,r[14],4); /* mov.l @r14,r1 */
 goto L8c602d46;
L8c602d46: r[1]|=r[13]; /* or r13,r1 */
 goto L8c602d48;
L8c602d48: wr(m,r[14],r[1],4); /* mov.l r1,@r14 */
 goto L8c602d4a;
L8c602d4a: r[3]=r[13]; /* mov r13,r3 */
 goto L8c602d4c;
L8c602d4c: *sr=(*sr&~1u)|!(r[10]&r[3]); /* tst r10,r3 */
 goto L8c602d4e;
L8c602d4e: if((*sr&1u))goto L8c602d62; /* bt 0x8c602d62 */
 goto L8c602d50;
L8c602d50: r[1]=rd(m,r[14],4); /* mov.l @r14,r1 */
 goto L8c602d52;
L8c602d52: *sr=(*sr&~1u)|!(r[13]&r[1]); /* tst r13,r1 */
 goto L8c602d54;
L8c602d54: if((*sr&1u))goto L8c602d62; /* bt 0x8c602d62 */
 goto L8c602d56;
L8c602d56: target=r[9];*pr=0x8c602d5au;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto L8c602d5a;
L8c602d5a: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c602d5c;
L8c602d5c: r[2]=~r[13]; /* not r13,r2 */
 goto L8c602d5e;
L8c602d5e: r[3]&=r[2]; /* and r2,r3 */
 goto L8c602d60;
L8c602d60: wr(m,r[14],r[3],4); /* mov.l r3,@r14 */
 goto L8c602d62;
L8c602d62: r[11]+=1u; /* add #0x1,r11 */
 goto L8c602d64;
L8c602d64: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1; /* shll r13 */
 goto L8c602d66;
L8c602d66: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[12]); /* cmp/ge r12,r11 */
 goto L8c602d68;
L8c602d68: if(!(*sr&1u))goto L8c602d2a; /* bf 0x8c602d2a */
 goto L8c602d6a;
L8c602d6a: r[3]=rd(m,0x8c602f18u,4); /* mov.l 0x8c602f18,r3 */
 goto L8c602d6c;
L8c602d6c: r[13]=1u; /* mov #0x1,r13 */
 goto L8c602d6e;
L8c602d6e: r[2]=rd(m,r[8],4); /* mov.l @r8,r2 */
 goto L8c602d70;
L8c602d70: r[11]=0u; /* mov #0x0,r11 */
 goto L8c602d72;
L8c602d72: r[4]=rd(m,r[3],4); /* mov.l @r3,r4 */
 goto L8c602d74;
L8c602d74: r[9]=rd(m,0x8c602f24u,4); /* mov.l 0x8c602f24,r9 */
 goto L8c602d76;
L8c602d76: r[2]^=r[4]; /* xor r4,r2 */
 goto L8c602d78;
L8c602d78: r[14]=rd(m,0x8c602f28u,4); /* mov.l 0x8c602f28,r14 */
 goto L8c602d7a;
L8c602d7a: r[2]&=r[4]; /* and r4,r2 */
 goto L8c602d7c;
L8c602d7c: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto L8c602d7e;
L8c602d7e: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c602d80;
L8c602d80: r[8]=r[13]; /* mov r13,r8 */
 goto L8c602d82;
L8c602d82: *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c602d84;
L8c602d84: take=(*sr&1u);r[8]&=r[10];if(take)goto L8c602da4;goto L8c602d88; /* bt/s 0x8c602da4 */
 goto L8c602d88;
L8c602d88: *sr=(*sr&~1u)|!(r[8]&r[8]); /* tst r8,r8 */
 goto L8c602d8a;
L8c602d8a: if((*sr&1u))goto L8c602d9e; /* bt 0x8c602d9e */
 goto L8c602d8c;
L8c602d8c: r[5]=2u; /* mov #0x2,r5 */
 goto L8c602d8e;
L8c602d8e: r[6]=0u; /* mov #0x0,r6 */
 goto L8c602d90;
L8c602d90: target=r[9];*pr=0x8c602d94u;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto L8c602d94;
L8c602d94: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c602d96;
L8c602d96: r[3]=~r[13]; /* not r13,r3 */
 goto L8c602d98;
L8c602d98: r[2]&=r[3]; /* and r3,r2 */
 goto L8c602d9a;
L8c602d9a: wr(m,r[14],r[2],4);goto L8c602da4; /* bra 0x8c602da4 */
 goto L8c602d9e;
L8c602d9e: r[1]=rd(m,r[14],4); /* mov.l @r14,r1 */
 goto L8c602da0;
L8c602da0: r[1]|=r[13]; /* or r13,r1 */
 goto L8c602da2;
L8c602da2: wr(m,r[14],r[1],4); /* mov.l r1,@r14 */
 goto L8c602da4;
L8c602da4: *sr=(*sr&~1u)|!(r[8]&r[8]); /* tst r8,r8 */
 goto L8c602da6;
L8c602da6: if((*sr&1u))goto L8c602dbe; /* bt 0x8c602dbe */
 goto L8c602da8;
L8c602da8: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c602daa;
L8c602daa: *sr=(*sr&~1u)|!(r[13]&r[2]); /* tst r13,r2 */
 goto L8c602dac;
L8c602dac: if((*sr&1u))goto L8c602dbe; /* bt 0x8c602dbe */
 goto L8c602dae;
L8c602dae: r[5]=2u; /* mov #0x2,r5 */
 goto L8c602db0;
L8c602db0: r[6]=0u; /* mov #0x0,r6 */
 goto L8c602db2;
L8c602db2: target=r[9];*pr=0x8c602db6u;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto L8c602db6;
L8c602db6: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c602db8;
L8c602db8: r[2]=~r[13]; /* not r13,r2 */
 goto L8c602dba;
L8c602dba: r[3]&=r[2]; /* and r2,r3 */
 goto L8c602dbc;
L8c602dbc: wr(m,r[14],r[3],4); /* mov.l r3,@r14 */
 goto L8c602dbe;
L8c602dbe: r[11]+=1u; /* add #0x1,r11 */
 goto L8c602dc0;
L8c602dc0: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1; /* shll r13 */
 goto L8c602dc2;
L8c602dc2: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[12]); /* cmp/ge r12,r11 */
 goto L8c602dc4;
L8c602dc4: if(!(*sr&1u))goto L8c602d7e; /* bf 0x8c602d7e */
 goto L8c602dc6;
L8c602dc6: r[15]+=4u; /* add #0x4,r15 */
 goto L8c602dc8;
L8c602dc8: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c602dca;
L8c602dca: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c602dcc;
L8c602dcc: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c602dce;
L8c602dce: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c602dd0;
L8c602dd0: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c602dd2;
L8c602dd2: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c602dd4;
L8c602dd4: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c602dd6;
L8c602dd6: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
