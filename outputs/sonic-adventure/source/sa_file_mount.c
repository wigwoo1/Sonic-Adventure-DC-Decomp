#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
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
/* Static C recovery of retail Tikal helper 8c5ffd28..8c600032.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_mount(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c5ffd28;
L8c5ffd28: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c5ffd2a;
L8c5ffd2a: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c5ffd2c;
L8c5ffd2c: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c5ffd2e;
L8c5ffd2e: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c5ffd30;
L8c5ffd30: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c5ffd32;
L8c5ffd32: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c5ffd34;
L8c5ffd34: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c5ffd36;
L8c5ffd36: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c5ffd38;
L8c5ffd38: r[15]+=4294967264u; /* add #-0x20,r15 */
 goto L8c5ffd3a;
L8c5ffd3a: r[0]=*sr; /* stc SR,r0 */
 goto L8c5ffd3c;
L8c5ffd3c: r[10]=r[15]; /* mov r15,r10 */
 goto L8c5ffd3e;
L8c5ffd3e: r[10]+=24u; /* add #0x18,r10 */
 goto L8c5ffd40;
L8c5ffd40: r[3]=r[10]; /* mov r10,r3 */
 goto L8c5ffd42;
L8c5ffd42: r[3]+=4u; /* add #0x4,r3 */
 goto L8c5ffd44;
L8c5ffd44: r[0]>>=2; /* shlr2 r0 */
 goto L8c5ffd46;
L8c5ffd46: r[8]=rd(m,0x8c5ffe2cu,4); /* mov.l 0x8c5ffe2c,r8 */
 goto L8c5ffd48;
L8c5ffd48: r[0]>>=2; /* shlr2 r0 */
 goto L8c5ffd4a;
L8c5ffd4a: wr(m,r[15]+0x8u,r[3],4); /* mov.l r3,@(0x8,r15) */
 goto L8c5ffd4c;
L8c5ffd4c: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c5ffd4e;
L8c5ffd4e: r[4]=r[8]; /* mov r8,r4 */
 goto L8c5ffd50;
L8c5ffd50: r[3]=sx(m,0x8c5ffe22u,2); /* mov.w 0x8c5ffe22,r3 */
 goto L8c5ffd52;
L8c5ffd52: r[2]=r[8]; /* mov r8,r2 */
 goto L8c5ffd54;
L8c5ffd54: r[1]=r[4]; /* mov r4,r1 */
 goto L8c5ffd56;
L8c5ffd56: r[1]+=14u; /* add #0xe,r1 */
 goto L8c5ffd58;
L8c5ffd58: r[2]+=12u; /* add #0xc,r2 */
 goto L8c5ffd5a;
L8c5ffd5a: r[4]+=15u; /* add #0xf,r4 */
 goto L8c5ffd5c;
L8c5ffd5c: wr(m,r[15]+0x10u,r[2],4); /* mov.l r2,@(0x10,r15) */
 goto L8c5ffd5e;
L8c5ffd5e: wr(m,r[15],r[1],4); /* mov.l r1,@r15 */
 goto L8c5ffd60;
L8c5ffd60: wr(m,r[15]+0x4u,r[4],4); /* mov.l r4,@(0x4,r15) */
 goto L8c5ffd62;
L8c5ffd62: wr(m,r[15]+0xcu,r[0],4); /* mov.l r0,@(0xc,r15) */
 goto L8c5ffd64;
L8c5ffd64: r[0]=*sr; /* stc SR,r0 */
 goto L8c5ffd66;
L8c5ffd66: r[0]&=r[3]; /* and r3,r0 */
 goto L8c5ffd68;
L8c5ffd68: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c5ffd6a;
L8c5ffd6a: *sr=r[0]; /* ldc r0,SR */
 goto L8c5ffd6c;
L8c5ffd6c: r[12]=rd(m,0x8c5ffe30u,4); /* mov.l 0x8c5ffe30,r12 */
 goto L8c5ffd6e;
L8c5ffd6e: r[14]=rd(m,0x8c5ffe38u,4); /* mov.l 0x8c5ffe38,r14 */
 goto L8c5ffd70;
L8c5ffd70: r[2]=rd(m,r[12],4); /* mov.l @r12,r2 */
 goto L8c5ffd72;
L8c5ffd72: r[13]=rd(m,0x8c5ffe34u,4); /* mov.l 0x8c5ffe34,r13 */
 goto L8c5ffd74;
L8c5ffd74: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c5ffd76;
L8c5ffd76: if(!(*sr&1u))goto L8c5ffd86; /* bf 0x8c5ffd86 */
 goto L8c5ffd78;
L8c5ffd78: r[5]=rd(m,0x8c5ffe3cu,4); /* mov.l 0x8c5ffe3c,r5 */
 goto L8c5ffd7a;
L8c5ffd7a: r[6]=80u; /* mov #0x50,r6 */
 goto L8c5ffd7c;
L8c5ffd7c: r[7]=rd(m,r[14],4); /* mov.l @r14,r7 */
 goto L8c5ffd7e;
L8c5ffd7e: r[11]=sx(m,0x8c5ffe24u,2); /* mov.w 0x8c5ffe24,r11 */
 goto L8c5ffd80;
L8c5ffd80: target=r[13];*pr=0x8c5ffd84u;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c5ffd84;
L8c5ffd84: wr(m,r[12],r[0],4); /* mov.l r0,@r12 */
 goto L8c5ffd86;
L8c5ffd86: r[12]=rd(m,0x8c5ffe40u,4); /* mov.l 0x8c5ffe40,r12 */
 goto L8c5ffd88;
L8c5ffd88: r[3]=rd(m,r[12],4); /* mov.l @r12,r3 */
 goto L8c5ffd8a;
L8c5ffd8a: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c5ffd8c;
L8c5ffd8c: if(!(*sr&1u))goto L8c5ffd9c; /* bf 0x8c5ffd9c */
 goto L8c5ffd8e;
L8c5ffd8e: r[5]=rd(m,0x8c5ffe44u,4); /* mov.l 0x8c5ffe44,r5 */
 goto L8c5ffd90;
L8c5ffd90: r[6]=16u; /* mov #0x10,r6 */
 goto L8c5ffd92;
L8c5ffd92: r[7]=rd(m,r[14],4); /* mov.l @r14,r7 */
 goto L8c5ffd94;
L8c5ffd94: r[11]=sx(m,0x8c5ffe26u,2); /* mov.w 0x8c5ffe26,r11 */
 goto L8c5ffd96;
L8c5ffd96: target=r[13];*pr=0x8c5ffd9au;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c5ffd9a;
L8c5ffd9a: wr(m,r[12],r[0],4); /* mov.l r0,@r12 */
 goto L8c5ffd9c;
L8c5ffd9c: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c5ffd9e;
L8c5ffd9e: r[7]=0u; /* mov #0x0,r7 */
 goto L8c5ffda0;
L8c5ffda0: r[12]=rd(m,0x8c5ffe48u,4); /* mov.l 0x8c5ffe48,r12 */
 goto L8c5ffda2;
L8c5ffda2: r[5]=r[7]; /* mov r7,r5 */
 goto L8c5ffda4;
L8c5ffda4: r[6]=r[7]; /* mov r7,r6 */
 goto L8c5ffda6;
L8c5ffda6: target=r[12];*pr=0x8c5ffdaau;r[4]=rd(m,r[4]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c5ffdaa;
L8c5ffdaa: r[13]=r[0]; /* mov r0,r13 */
 goto L8c5ffdac;
L8c5ffdac: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c5ffdae;
L8c5ffdae: if((*sr&1u))goto L8c5ffe1e; /* bt 0x8c5ffe1e */
 goto L8c5ffdb0;
L8c5ffdb0: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c5ffdb2;
L8c5ffdb2: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c5ffdb4;
L8c5ffdb4: r[1]=rd(m,r[3]+0x1cu,4); /* mov.l @(0x1c,r3),r1 */
 goto L8c5ffdb6;
L8c5ffdb6: target=r[1];*pr=0x8c5ffdbau;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c5ffdba;
L8c5ffdba: r[4]=r[0]; /* mov r0,r4 */
 goto L8c5ffdbc;
L8c5ffdbc: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c5ffdbe;
L8c5ffdbe: if((*sr&1u))goto L8c5ffe1e; /* bt 0x8c5ffe1e */
 goto L8c5ffdc0;
L8c5ffdc0: r[0]=80u; /* mov #0x50,r0 */
 goto L8c5ffdc2;
L8c5ffdc2: wr(m,r[13]+0x3cu,r[4],4); /* mov.l r4,@(0x3c,r13) */
 goto L8c5ffdc4;
L8c5ffdc4: r[9]=4u; /* mov #0x4,r9 */
 goto L8c5ffdc6;
L8c5ffdc6: wr(m,r[13]+r[0],r[9],2); /* mov.w r9,@(r0,r13) */
 goto L8c5ffdc8;
L8c5ffdc8: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c5ffdca;
L8c5ffdca: wr(m,r[3]+0x1cu,r[13],4); /* mov.l r13,@(0x1c,r3) */
 goto L8c5ffdcc;
L8c5ffdcc: r[2]=rd(m,0x8c5ffe4cu,4); /* mov.l 0x8c5ffe4c,r2 */
 goto L8c5ffdce;
L8c5ffdce: target=r[2];*pr=0x8c5ffdd2u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c5ffdd2;
L8c5ffdd2: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c5ffdd4;
L8c5ffdd4: r[11]=r[0]; /* mov r0,r11 */
 goto L8c5ffdd6;
L8c5ffdd6: r[2]=rd(m,r[3]+0xcu,4); /* mov.l @(0xc,r3),r2 */
 goto L8c5ffdd8;
L8c5ffdd8: r[1]=rd(m,r[2]+0x20u,4); /* mov.l @(0x20,r2),r1 */
 goto L8c5ffdda;
L8c5ffdda: target=r[1];*pr=0x8c5ffddeu;r[4]=r[10];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c5ffdde;
L8c5ffdde: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c5ffde0;
L8c5ffde0: if((*sr&1u))goto L8c5ffe1e; /* bt 0x8c5ffe1e */
 goto L8c5ffde2;
L8c5ffde2: *sr=(*sr&~1u)|!(r[11]&r[11]); /* tst r11,r11 */
 goto L8c5ffde4;
L8c5ffde4: if((*sr&1u))goto L8c5ffdfe; /* bt 0x8c5ffdfe */
 goto L8c5ffde6;
L8c5ffde6: r[0]=r[11]; /* mov r11,r0 */
 goto L8c5ffde8;
L8c5ffde8: *sr=(*sr&~1u)|(r[0]==0xffffffe3u); /* cmp/eq #-0x1d,r0 */
 goto L8c5ffdea;
L8c5ffdea: if((*sr&1u))goto L8c5ffdf0; /* bt 0x8c5ffdf0 */
 goto L8c5ffdec;
L8c5ffdec: goto L8c5fffde; /* bra 0x8c5fffde */
 goto L8c5ffdf0;
L8c5ffdf0: r[0]=rd(m,r[15]+0x18u,4); /* mov.l @(0x18,r15),r0 */
 goto L8c5ffdf2;
L8c5ffdf2: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c5ffdf4;
L8c5ffdf4: if((*sr&1u))goto L8c5ffdfa; /* bt 0x8c5ffdfa */
 goto L8c5ffdf6;
L8c5ffdf6: goto L8c5fffde; /* bra 0x8c5fffde */
 goto L8c5ffdfa;
L8c5ffdfa: r[5]=4294967273u;goto L8c5ffff6; /* bra 0x8c5ffff6 */
 goto L8c5ffdfe;
L8c5ffdfe: r[2]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r2 */
 goto L8c5ffe00;
L8c5ffe00: r[4]=sx(m,0x8c5ffe28u,2); /* mov.w 0x8c5ffe28,r4 */
 goto L8c5ffe02;
L8c5ffe02: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c5ffe04;
L8c5ffe04: *sr=(*sr&~1u)|(r[4]==r[3]); /* cmp/eq r4,r3 */
 goto L8c5ffe06;
L8c5ffe06: if((*sr&1u))goto L8c5ffe0c; /* bt 0x8c5ffe0c */
 goto L8c5ffe08;
L8c5ffe08: goto L8c5ffff4; /* bra 0x8c5ffff4 */
 goto L8c5ffe0c;
L8c5ffe0c: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c5ffe0e;
L8c5ffe0e: r[7]=0u; /* mov #0x0,r7 */
 goto L8c5ffe10;
L8c5ffe10: r[5]=r[7]; /* mov r7,r5 */
 goto L8c5ffe12;
L8c5ffe12: r[6]=r[7]; /* mov r7,r6 */
 goto L8c5ffe14;
L8c5ffe14: target=r[12];*pr=0x8c5ffe18u;r[4]=rd(m,r[4]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c5ffe18;
L8c5ffe18: r[12]=r[0]; /* mov r0,r12 */
 goto L8c5ffe1a;
L8c5ffe1a: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c5ffe1c;
L8c5ffe1c: if(!(*sr&1u))goto L8c5ffe50; /* bf 0x8c5ffe50 */
 goto L8c5ffe1e;
L8c5ffe1e: r[5]=4294967294u;goto L8c5ffff6; /* bra 0x8c5ffff6 */
 goto L8c5ffe50;
L8c5ffe50: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c5ffe52;
L8c5ffe52: r[13]=0u; /* mov #0x0,r13 */
 goto L8c5ffe54;
L8c5ffe54: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c5ffe56;
L8c5ffe56: wr(m,r[3]+0x2cu,r[13],4); /* mov.l r13,@(0x2c,r3) */
 goto L8c5ffe58;
L8c5ffe58: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c5ffe5a;
L8c5ffe5a: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c5ffe5c;
L8c5ffe5c: r[1]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r1 */
 goto L8c5ffe5e;
L8c5ffe5e: target=r[1];*pr=0x8c5ffe62u;r[4]=r[8];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c5ffe62;
L8c5ffe62: r[4]=r[0]; /* mov r0,r4 */
 goto L8c5ffe64;
L8c5ffe64: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c5ffe66;
L8c5ffe66: if((*sr&1u))goto L8c5ffeac; /* bt 0x8c5ffeac */
 goto L8c5ffe68;
L8c5ffe68: r[0]=80u; /* mov #0x50,r0 */
 goto L8c5ffe6a;
L8c5ffe6a: wr(m,r[12]+0x3cu,r[4],4); /* mov.l r4,@(0x3c,r12) */
 goto L8c5ffe6c;
L8c5ffe6c: wr(m,r[12]+r[0],r[9],2); /* mov.w r9,@(r0,r12) */
 goto L8c5ffe6e;
L8c5ffe6e: r[3]=rd(m,r[12],4); /* mov.l @r12,r3 */
 goto L8c5ffe70;
L8c5ffe70: wr(m,r[3]+0x1cu,r[12],4); /* mov.l r12,@(0x1c,r3) */
 goto L8c5ffe72;
L8c5ffe72: r[2]=rd(m,0x8c5fff94u,4); /* mov.l 0x8c5fff94,r2 */
 goto L8c5ffe74;
L8c5ffe74: target=r[2];*pr=0x8c5ffe78u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c5ffe78;
L8c5ffe78: r[4]=r[0]; /* mov r0,r4 */
 goto L8c5ffe7a;
L8c5ffe7a: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c5ffe7c;
L8c5ffe7c: if(!(*sr&1u))goto L8c5ffeac; /* bf 0x8c5ffeac */
 goto L8c5ffe7e;
L8c5ffe7e: r[2]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r2 */
 goto L8c5ffe80;
L8c5ffe80: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c5ffe82;
L8c5ffe82: r[1]=sx(m,r[2],1); /* mov.b @r2,r1 */
 goto L8c5ffe84;
L8c5ffe84: r[2]=rd(m,r[3]+0x2cu,4); /* mov.l @(0x2c,r3),r2 */
 goto L8c5ffe86;
L8c5ffe86: r[1]+=4294967248u; /* add #-0x30,r1 */
 goto L8c5ffe88;
L8c5ffe88: r[1]<<=16; /* shll16 r1 */
 goto L8c5ffe8a;
L8c5ffe8a: r[2]|=r[1]; /* or r1,r2 */
 goto L8c5ffe8c;
L8c5ffe8c: wr(m,r[3]+0x2cu,r[2],4); /* mov.l r2,@(0x2c,r3) */
 goto L8c5ffe8e;
L8c5ffe8e: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c5ffe90;
L8c5ffe90: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c5ffe92;
L8c5ffe92: r[1]=sx(m,r[2],1); /* mov.b @r2,r1 */
 goto L8c5ffe94;
L8c5ffe94: r[2]=rd(m,r[3]+0x2cu,4); /* mov.l @(0x2c,r3),r2 */
 goto L8c5ffe96;
L8c5ffe96: r[1]+=4294967248u; /* add #-0x30,r1 */
 goto L8c5ffe98;
L8c5ffe98: r[1]<<=8; /* shll8 r1 */
 goto L8c5ffe9a;
L8c5ffe9a: r[2]|=r[1]; /* or r1,r2 */
 goto L8c5ffe9c;
L8c5ffe9c: wr(m,r[3]+0x2cu,r[2],4); /* mov.l r2,@(0x2c,r3) */
 goto L8c5ffe9e;
L8c5ffe9e: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c5ffea0;
L8c5ffea0: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c5ffea2;
L8c5ffea2: r[1]=sx(m,r[2],1); /* mov.b @r2,r1 */
 goto L8c5ffea4;
L8c5ffea4: r[2]=rd(m,r[3]+0x2cu,4); /* mov.l @(0x2c,r3),r2 */
 goto L8c5ffea6;
L8c5ffea6: r[1]+=4294967248u; /* add #-0x30,r1 */
 goto L8c5ffea8;
L8c5ffea8: r[2]|=r[1]; /* or r1,r2 */
 goto L8c5ffeaa;
L8c5ffeaa: wr(m,r[3]+0x2cu,r[2],4); /* mov.l r2,@(0x2c,r3) */
 goto L8c5ffeac;
L8c5ffeac: r[6]=rd(m,r[14],4); /* mov.l @r14,r6 */
 goto L8c5ffeae;
L8c5ffeae: r[5]=1u; /* mov #0x1,r5 */
 goto L8c5ffeb0;
L8c5ffeb0: r[12]=rd(m,0x8c5fff98u,4); /* mov.l 0x8c5fff98,r12 */
 goto L8c5ffeb2;
L8c5ffeb2: r[6]+=64u; /* add #0x40,r6 */
 goto L8c5ffeb4;
L8c5ffeb4: *pr=0x8c5ffeb8u;r[4]=rd(m,r[12],4);status=call(ctx,0x8c5ffc98u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c5ffc98 */
 goto L8c5ffeb8;
L8c5ffeb8: r[4]=r[0]; /* mov r0,r4 */
 goto L8c5ffeba;
L8c5ffeba: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c5ffebc;
L8c5ffebc: if((*sr&1u))goto L8c5ffec2; /* bt 0x8c5ffec2 */
 goto L8c5ffebe;
L8c5ffebe: goto L8c5fffe6; /* bra 0x8c5fffe6 */
 goto L8c5ffec2;
L8c5ffec2: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c5ffec4;
L8c5ffec4: *pr=0x8c5ffec8u;r[4]+=64u;status=call(ctx,0x8c5ff9e6u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c5ff9e6 */
 goto L8c5ffec8;
L8c5ffec8: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c5ffeca;
L8c5ffeca: if(!(*sr&1u))goto L8c5ffed0; /* bf 0x8c5ffed0 */
 goto L8c5ffecc;
L8c5ffecc: goto L8c5ffff4; /* bra 0x8c5ffff4 */
 goto L8c5ffed0;
L8c5ffed0: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c5ffed2;
L8c5ffed2: r[1]=4u; /* mov #0x4,r1 */
 goto L8c5ffed4;
L8c5ffed4: r[12]=sx(m,0x8c5fff8au,2); /* mov.w 0x8c5fff8a,r12 */
 goto L8c5ffed6;
L8c5ffed6: r[1]+=r[2]; /* add r2,r1 */
 goto L8c5ffed8;
L8c5ffed8: r[15]-=4;wr(m,r[15],r[1],4); /* mov.l r1,@-r15 */
 goto L8c5ffeda;
L8c5ffeda: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c5ffedc;
L8c5ffedc: r[4]+=64u; /* add #0x40,r4 */
 goto L8c5ffede;
L8c5ffede: *pr=0x8c5ffee2u;r[4]+=r[12];status=call(ctx,0x8c5ff9c0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c5ff9c0 */
 goto L8c5ffee2;
L8c5ffee2: r[3]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r3 */
 goto L8c5ffee4;
L8c5ffee4: r[1]=8u; /* mov #0x8,r1 */
 goto L8c5ffee6;
L8c5ffee6: r[11]=sx(m,0x8c5fff8cu,2); /* mov.w 0x8c5fff8c,r11 */
 goto L8c5ffee8;
L8c5ffee8: r[12]+=4294967288u; /* add #-0x8,r12 */
 goto L8c5ffeea;
L8c5ffeea: wr(m,r[3],r[0],4); /* mov.l r0,@r3 */
 goto L8c5ffeec;
L8c5ffeec: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c5ffeee;
L8c5ffeee: r[3]=rd(m,r[2]+0x4u,4); /* mov.l @(0x4,r2),r3 */
 goto L8c5ffef0;
L8c5ffef0: r[3]+=r[11]; /* add r11,r3 */
 goto L8c5ffef2;
L8c5ffef2: wr(m,r[2]+0x4u,r[3],4); /* mov.l r3,@(0x4,r2) */
 goto L8c5ffef4;
L8c5ffef4: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c5ffef6;
L8c5ffef6: r[1]+=r[2]; /* add r2,r1 */
 goto L8c5ffef8;
L8c5ffef8: r[15]-=4;wr(m,r[15],r[1],4); /* mov.l r1,@-r15 */
 goto L8c5ffefa;
L8c5ffefa: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c5ffefc;
L8c5ffefc: r[4]+=64u; /* add #0x40,r4 */
 goto L8c5ffefe;
L8c5ffefe: *pr=0x8c5fff02u;r[4]+=r[12];status=call(ctx,0x8c5ff9c0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c5ff9c0 */
 goto L8c5fff02;
L8c5fff02: r[3]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r3 */
 goto L8c5fff04;
L8c5fff04: r[4]=rd(m,0x8c5fffa4u,4); /* mov.l 0x8c5fffa4,r4 */
 goto L8c5fff06;
L8c5fff06: wr(m,r[3],r[0],4); /* mov.l r0,@r3 */
 goto L8c5fff08;
L8c5fff08: r[2]=rd(m,0x8c5fffa0u,4); /* mov.l 0x8c5fffa0,r2 */
 goto L8c5fff0a;
L8c5fff0a: r[12]=rd(m,0x8c5fff9cu,4); /* mov.l 0x8c5fff9c,r12 */
 goto L8c5fff0c;
L8c5fff0c: r[3]=r[2]; /* mov r2,r3 */
 goto L8c5fff0e;
L8c5fff0e: wr(m,r[12],r[2],4); /* mov.l r2,@r12 */
 goto L8c5fff10;
L8c5fff10: r[0]=sx(m,r[4]+0xcu,1); /* mov.b @(0xc,r4),r0 */
 goto L8c5fff12;
L8c5fff12: wr(m,r[3],r[0],2); /* mov.w r0,@r3 */
 goto L8c5fff14;
L8c5fff14: r[0]=rd(m,r[12],4); /* mov.l @r12,r0 */
 goto L8c5fff16;
L8c5fff16: r[0]=sx(m,r[0],2); /* mov.w @r0,r0 */
 goto L8c5fff18;
L8c5fff18: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c5fff1a;
L8c5fff1a: if(!(*sr&1u))goto L8c5fff44; /* bf 0x8c5fff44 */
 goto L8c5fff1c;
L8c5fff1c: r[2]=rd(m,r[12],4); /* mov.l @r12,r2 */
 goto L8c5fff1e;
L8c5fff1e: r[5]=r[4]; /* mov r4,r5 */
 goto L8c5fff20;
L8c5fff20: r[0]=sx(m,r[4]+0xdu,1); /* mov.b @(0xd,r4),r0 */
 goto L8c5fff22;
L8c5fff22: r[4]=r[13]; /* mov r13,r4 */
 goto L8c5fff24;
L8c5fff24: wr(m,r[2]+0x2u,r[0],2); /* mov.w r0,@(0x2,r2) */
 goto L8c5fff26;
L8c5fff26: r[3]=rd(m,r[12],4); /* mov.l @r12,r3 */
 goto L8c5fff28;
L8c5fff28: wr(m,r[3]+0x4u,r[13],4); /* mov.l r13,@(0x4,r3) */
 goto L8c5fff2a;
L8c5fff2a: r[6]=rd(m,r[12],4); /* mov.l @r12,r6 */
 goto L8c5fff2c;
L8c5fff2c: r[6]+=8u; /* add #0x8,r6 */
 goto L8c5fff2e;
L8c5fff2e: r[0]=sx(m,r[5],1); /* mov.b @r5,r0 */
 goto L8c5fff30;
L8c5fff30: *sr=(*sr&~1u)|(r[0]==0x20u); /* cmp/eq #0x20,r0 */
 goto L8c5fff32;
L8c5fff32: if((*sr&1u))goto L8c5fff42; /* bt 0x8c5fff42 */
 goto L8c5fff34;
L8c5fff34: r[2]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r2 */
 goto L8c5fff36;
L8c5fff36: r[7]=12u; /* mov #0xc,r7 */
 goto L8c5fff38;
L8c5fff38: r[4]+=1u; /* add #0x1,r4 */
 goto L8c5fff3a;
L8c5fff3a: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[7]); /* cmp/ge r7,r4 */
 goto L8c5fff3c;
L8c5fff3c: wr(m,r[6],r[2],1); /* mov.b r2,@r6 */
 goto L8c5fff3e;
L8c5fff3e: take=!(*sr&1u);r[6]+=1u;if(take)goto L8c5fff2e;goto L8c5fff42; /* bf/s 0x8c5fff2e */
 goto L8c5fff42;
L8c5fff42: wr(m,r[6],r[13],1); /* mov.b r13,@r6 */
 goto L8c5fff44;
L8c5fff44: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c5fff46;
L8c5fff46: r[9]=sx(m,0x8c5fff8eu,2); /* mov.w 0x8c5fff8e,r9 */
 goto L8c5fff48;
L8c5fff48: r[4]+=64u; /* add #0x40,r4 */
 goto L8c5fff4a;
L8c5fff4a: *pr=0x8c5fff4eu;r[4]+=r[9];status=call(ctx,0x8c5ff9c0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c5ff9c0 */
 goto L8c5fff4e;
L8c5fff4e: r[9]=r[0]; /* mov r0,r9 */
 goto L8c5fff50;
L8c5fff50: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c5fff52;
L8c5fff52: r[9]+=r[11]; /* add r11,r9 */
 goto L8c5fff54;
L8c5fff54: r[11]=sx(m,0x8c5fff90u,2); /* mov.w 0x8c5fff90,r11 */
 goto L8c5fff56;
L8c5fff56: r[4]+=64u; /* add #0x40,r4 */
 goto L8c5fff58;
L8c5fff58: *pr=0x8c5fff5cu;r[4]+=r[11];status=call(ctx,0x8c5ff9c0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c5ff9c0 */
 goto L8c5fff5c;
L8c5fff5c: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c5fff5e;
L8c5fff5e: r[7]=r[0]; /* mov r0,r7 */
 goto L8c5fff60;
L8c5fff60: r[5]=r[9]; /* mov r9,r5 */
 goto L8c5fff62;
L8c5fff62: r[3]=rd(m,r[2]+0x10u,4); /* mov.l @(0x10,r2),r3 */
 goto L8c5fff64;
L8c5fff64: wr(m,r[3]+0x8u,r[13],4); /* mov.l r13,@(0x8,r3) */
 goto L8c5fff66;
L8c5fff66: r[6]=rd(m,r[14],4); /* mov.l @r14,r6 */
 goto L8c5fff68;
L8c5fff68: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c5fff6a;
L8c5fff6a: r[3]=rd(m,0x8c5fffa8u,4); /* mov.l 0x8c5fffa8,r3 */
 goto L8c5fff6c;
L8c5fff6c: r[6]=rd(m,r[6]+0x10u,4); /* mov.l @(0x10,r6),r6 */
 goto L8c5fff6e;
L8c5fff6e: target=r[3];*pr=0x8c5fff72u;r[4]+=64u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c5fff72;
L8c5fff72: *sr=(*sr&~1u)|(r[0]==0xfffffff4u); /* cmp/eq #-0xc,r0 */
 goto L8c5fff74;
L8c5fff74: take=(*sr&1u);r[11]=r[0];if(take)goto L8c5fffde;goto L8c5fff78; /* bt/s 0x8c5fffde */
 goto L8c5fff78;
L8c5fff78: *sr=(*sr&~1u)|!(r[11]&r[11]); /* tst r11,r11 */
 goto L8c5fff7a;
L8c5fff7a: if(!(*sr&1u))goto L8c5fffe6; /* bf 0x8c5fffe6 */
 goto L8c5fff7c;
L8c5fff7c: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c5fff7e;
L8c5fff7e: r[3]=rd(m,r[2]+0x10u,4); /* mov.l @(0x10,r2),r3 */
 goto L8c5fff80;
L8c5fff80: r[1]=rd(m,r[3],4); /* mov.l @r3,r1 */
 goto L8c5fff82;
L8c5fff82: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c5fff84;
L8c5fff84: if((*sr&1u))goto L8c5ffff4; /* bt 0x8c5ffff4 */
 goto L8c5fff86;
L8c5fff86: goto L8c5fffac; /* bra 0x8c5fffac */
 goto L8c5fffac;
L8c5fffac: r[11]=rd(m,r[12],4); /* mov.l @r12,r11 */
 goto L8c5fffae;
L8c5fffae: r[0]=sx(m,r[11],2); /* mov.w @r11,r0 */
 goto L8c5fffb0;
L8c5fffb0: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c5fffb2;
L8c5fffb2: if(!(*sr&1u))goto L8c600000; /* bf 0x8c600000 */
 goto L8c5fffb4;
L8c5fffb4: r[5]=r[15]; /* mov r15,r5 */
 goto L8c5fffb6;
L8c5fffb6: r[4]=r[11]; /* mov r11,r4 */
 goto L8c5fffb8;
L8c5fffb8: r[5]+=20u; /* add #0x14,r5 */
 goto L8c5fffba;
L8c5fffba: *pr=0x8c5fffbeu;r[4]+=8u;status=call(ctx,0x8c60041au,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c60041a */
 goto L8c5fffbe;
L8c5fffbe: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c5fffc0;
L8c5fffc0: if(!(*sr&1u))goto L8c5fffe6; /* bf 0x8c5fffe6 */
 goto L8c5fffc2;
L8c5fffc2: r[3]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r3 */
 goto L8c5fffc4;
L8c5fffc4: r[7]=1u; /* mov #0x1,r7 */
 goto L8c5fffc6;
L8c5fffc6: r[5]=rd(m,r[12],4); /* mov.l @r12,r5 */
 goto L8c5fffc8;
L8c5fffc8: wr(m,r[5]+0x4u,r[3],4); /* mov.l r3,@(0x4,r5) */
 goto L8c5fffca;
L8c5fffca: r[5]=r[3]; /* mov r3,r5 */
 goto L8c5fffcc;
L8c5fffcc: r[6]=rd(m,r[14],4); /* mov.l @r14,r6 */
 goto L8c5fffce;
L8c5fffce: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c5fffd0;
L8c5fffd0: r[6]=rd(m,r[6]+0x10u,4); /* mov.l @(0x10,r6),r6 */
 goto L8c5fffd2;
L8c5fffd2: r[3]=rd(m,0x8c6000dcu,4); /* mov.l 0x8c6000dc,r3 */
 goto L8c5fffd4;
L8c5fffd4: target=r[3];*pr=0x8c5fffd8u;r[4]+=64u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c5fffd8;
L8c5fffd8: *sr=(*sr&~1u)|(r[0]==0xfffffff4u); /* cmp/eq #-0xc,r0 */
 goto L8c5fffda;
L8c5fffda: take=!(*sr&1u);r[11]=r[0];if(take)goto L8c5fffe2;goto L8c5fffde; /* bf/s 0x8c5fffe2 */
 goto L8c5fffde;
L8c5fffde: r[4]=r[11];goto L8c60000a; /* bra 0x8c60000a */
 goto L8c5fffe2;
L8c5fffe2: *sr=(*sr&~1u)|!(r[11]&r[11]); /* tst r11,r11 */
 goto L8c5fffe4;
L8c5fffe4: if((*sr&1u))goto L8c5fffea; /* bt 0x8c5fffea */
 goto L8c5fffe6;
L8c5fffe6: r[5]=4294967292u;goto L8c5ffff6; /* bra 0x8c5ffff6 */
 goto L8c5fffea;
L8c5fffea: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c5fffec;
L8c5fffec: r[2]=rd(m,r[3]+0x10u,4); /* mov.l @(0x10,r3),r2 */
 goto L8c5fffee;
L8c5fffee: r[1]=rd(m,r[2],4); /* mov.l @r2,r1 */
 goto L8c5ffff0;
L8c5ffff0: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c5ffff2;
L8c5ffff2: if(!(*sr&1u))goto L8c5ffffa; /* bf 0x8c5ffffa */
 goto L8c5ffff4;
L8c5ffff4: r[5]=4294967291u; /* mov #-0x5,r5 */
 goto L8c5ffff6;
L8c5ffff6: r[4]=r[5];goto L8c60000a; /* bra 0x8c60000a */
 goto L8c5ffffa;
L8c5ffffa: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c5ffffc;
L8c5ffffc: *pr=0x8c600000u;r[4]=rd(m,r[4]+0x10u,4);status=call(ctx,0x8c5ffcfeu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c5ffcfe */
 goto L8c600000;
L8c600000: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c600002;
L8c600002: r[0]=56u; /* mov #0x38,r0 */
 goto L8c600004;
L8c600004: r[4]=1u; /* mov #0x1,r4 */
 goto L8c600006;
L8c600006: wr(m,r[2]+r[0],r[4],2); /* mov.w r4,@(r0,r2) */
 goto L8c600008;
L8c600008: r[4]=r[13]; /* mov r13,r4 */
 goto L8c60000a;
L8c60000a: r[0]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r0 */
 goto L8c60000c;
L8c60000c: r[3]=*sr; /* stc SR,r3 */
 goto L8c60000e;
L8c60000e: r[2]=sx(m,0x8c6000d6u,2); /* mov.w 0x8c6000d6,r2 */
 goto L8c600010;
L8c600010: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c600012;
L8c600012: r[0]<<=2; /* shll2 r0 */
 goto L8c600014;
L8c600014: r[3]&=r[2]; /* and r2,r3 */
 goto L8c600016;
L8c600016: r[0]<<=2; /* shll2 r0 */
 goto L8c600018;
L8c600018: r[0]|=r[3]; /* or r3,r0 */
 goto L8c60001a;
L8c60001a: *sr=r[0]; /* ldc r0,SR */
 goto L8c60001c;
L8c60001c: r[0]=r[4]; /* mov r4,r0 */
 goto L8c60001e;
L8c60001e: r[15]+=32u; /* add #0x20,r15 */
 goto L8c600020;
L8c600020: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c600022;
L8c600022: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c600024;
L8c600024: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c600026;
L8c600026: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c600028;
L8c600028: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c60002a;
L8c60002a: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c60002c;
L8c60002c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c60002e;
L8c60002e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
