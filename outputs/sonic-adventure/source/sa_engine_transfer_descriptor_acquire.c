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
/* Static C recovery of USA Rev A fixed transfer descriptor acquisition. */
SA_STAGE_EXPORT int sa_engine_transfer_descriptor_acquire(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ec40;
L8c10ec40: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c10ec42;
L8c10ec42: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c10ec44;
L8c10ec44: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c10ec46;
L8c10ec46: r[6]=52u; /* mov #0x34,r6 */
 goto L8c10ec48;
L8c10ec48: r[7]=rd(m,0x8c10ed4cu,4); /* mov.l 0x8c10ed4c,r7 */
 goto L8c10ec4a;
L8c10ec4a: *macl=r[6]*r[5]; /* mul.l r6,r5 */
 goto L8c10ec4c;
L8c10ec4c: r[3]=rd(m,r[7],4); /* mov.l @r7,r3 */
 goto L8c10ec4e;
L8c10ec4e: r[14]=0u; /* mov #0x0,r14 */
 goto L8c10ec50;
L8c10ec50: wr(m,r[15]+0x8u,r[3],4); /* mov.l r3,@(0x8,r15) */
 goto L8c10ec52;
L8c10ec52: r[3]+=4u; /* add #0x4,r3 */
 goto L8c10ec54;
L8c10ec54: r[5]=*macl; /* sts MACL,r5 */
 goto L8c10ec56;
L8c10ec56: r[5]+=r[3]; /* add r3,r5 */
 goto L8c10ec58;
L8c10ec58: r[0]=rd(m,r[5]+0x28u,4); /* mov.l @(0x28,r5),r0 */
 goto L8c10ec5a;
L8c10ec5a: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10ec5c;
L8c10ec5c: take=!(*sr&1u);r[13]=r[14];if(take)goto L8c10ec66;goto L8c10ec60; /* bf/s 0x8c10ec66 */
 goto L8c10ec60;
L8c10ec60: r[13]=rd(m,0x8c10ed50u,4); /* mov.l 0x8c10ed50,r13 */
 goto L8c10ec62;
L8c10ec62: goto L8c10ed40; /* bra 0x8c10ed40 */
 goto L8c10ec66;
L8c10ec66: r[0]=*sr; /* stc SR,r0 */
 goto L8c10ec68;
L8c10ec68: r[3]=sx(m,0x8c10ed4au,2); /* mov.w 0x8c10ed4a,r3 */
 goto L8c10ec6a;
L8c10ec6a: r[0]>>=2; /* shlr2 r0 */
 goto L8c10ec6c;
L8c10ec6c: r[0]>>=2; /* shlr2 r0 */
 goto L8c10ec6e;
L8c10ec6e: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c10ec70;
L8c10ec70: wr(m,r[15]+0x4u,r[0],4); /* mov.l r0,@(0x4,r15) */
 goto L8c10ec72;
L8c10ec72: r[0]=*sr; /* stc SR,r0 */
 goto L8c10ec74;
L8c10ec74: r[0]&=r[3]; /* and r3,r0 */
 goto L8c10ec76;
L8c10ec76: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c10ec78;
L8c10ec78: *sr=r[0]; /* ldc r0,SR */
 goto L8c10ec7a;
L8c10ec7a: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c10ec7c;
L8c10ec7c: r[2]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r2 */
 goto L8c10ec7e;
L8c10ec7e: *macl=r[6]*r[3]; /* mul.l r6,r3 */
 goto L8c10ec80;
L8c10ec80: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c10ec82;
L8c10ec82: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10ec84;
L8c10ec84: r[1]=rd(m,r[15],4); /* mov.l @r15,r1 */
 goto L8c10ec86;
L8c10ec86: r[3]=*macl; /* sts MACL,r3 */
 goto L8c10ec88;
L8c10ec88: r[3]+=r[2]; /* add r2,r3 */
 goto L8c10ec8a;
L8c10ec8a: wr(m,r[3],r[1],4); /* mov.l r1,@r3 */
 goto L8c10ec8c;
L8c10ec8c: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c10ec8e;
L8c10ec8e: r[3]=rd(m,r[7],4); /* mov.l @r7,r3 */
 goto L8c10ec90;
L8c10ec90: *macl=r[6]*r[2]; /* mul.l r6,r2 */
 goto L8c10ec92;
L8c10ec92: r[3]+=4u; /* add #0x4,r3 */
 goto L8c10ec94;
L8c10ec94: r[2]=*macl; /* sts MACL,r2 */
 goto L8c10ec96;
L8c10ec96: r[2]+=r[3]; /* add r3,r2 */
 goto L8c10ec98;
L8c10ec98: wr(m,r[2]+0x28u,r[14],4); /* mov.l r14,@(0x28,r2) */
 goto L8c10ec9a;
L8c10ec9a: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c10ec9c;
L8c10ec9c: r[2]=rd(m,r[7],4); /* mov.l @r7,r2 */
 goto L8c10ec9e;
L8c10ec9e: *macl=r[6]*r[3]; /* mul.l r6,r3 */
 goto L8c10eca0;
L8c10eca0: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10eca2;
L8c10eca2: r[3]=*macl; /* sts MACL,r3 */
 goto L8c10eca4;
L8c10eca4: r[3]+=r[2]; /* add r2,r3 */
 goto L8c10eca6;
L8c10eca6: wr(m,r[3]+0x2cu,r[14],4); /* mov.l r14,@(0x2c,r3) */
 goto L8c10eca8;
L8c10eca8: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c10ecaa;
L8c10ecaa: r[3]=rd(m,r[7],4); /* mov.l @r7,r3 */
 goto L8c10ecac;
L8c10ecac: *macl=r[6]*r[2]; /* mul.l r6,r2 */
 goto L8c10ecae;
L8c10ecae: r[3]+=4u; /* add #0x4,r3 */
 goto L8c10ecb0;
L8c10ecb0: r[2]=*macl; /* sts MACL,r2 */
 goto L8c10ecb2;
L8c10ecb2: r[2]+=r[3]; /* add r3,r2 */
 goto L8c10ecb4;
L8c10ecb4: wr(m,r[2]+0x30u,r[14],4); /* mov.l r14,@(0x30,r2) */
 goto L8c10ecb6;
L8c10ecb6: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c10ecb8;
L8c10ecb8: r[2]=rd(m,r[7],4); /* mov.l @r7,r2 */
 goto L8c10ecba;
L8c10ecba: *macl=r[6]*r[3]; /* mul.l r6,r3 */
 goto L8c10ecbc;
L8c10ecbc: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10ecbe;
L8c10ecbe: r[3]=*macl; /* sts MACL,r3 */
 goto L8c10ecc0;
L8c10ecc0: r[3]+=r[2]; /* add r2,r3 */
 goto L8c10ecc2;
L8c10ecc2: wr(m,r[3]+0x4u,r[14],4); /* mov.l r14,@(0x4,r3) */
 goto L8c10ecc4;
L8c10ecc4: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c10ecc6;
L8c10ecc6: r[3]=rd(m,r[7],4); /* mov.l @r7,r3 */
 goto L8c10ecc8;
L8c10ecc8: *macl=r[6]*r[2]; /* mul.l r6,r2 */
 goto L8c10ecca;
L8c10ecca: r[3]+=4u; /* add #0x4,r3 */
 goto L8c10eccc;
L8c10eccc: r[2]=*macl; /* sts MACL,r2 */
 goto L8c10ecce;
L8c10ecce: r[2]+=r[3]; /* add r3,r2 */
 goto L8c10ecd0;
L8c10ecd0: wr(m,r[2]+0x8u,r[14],4); /* mov.l r14,@(0x8,r2) */
 goto L8c10ecd2;
L8c10ecd2: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c10ecd4;
L8c10ecd4: r[2]=rd(m,r[7],4); /* mov.l @r7,r2 */
 goto L8c10ecd6;
L8c10ecd6: *macl=r[6]*r[3]; /* mul.l r6,r3 */
 goto L8c10ecd8;
L8c10ecd8: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10ecda;
L8c10ecda: r[3]=*macl; /* sts MACL,r3 */
 goto L8c10ecdc;
L8c10ecdc: r[3]+=r[2]; /* add r2,r3 */
 goto L8c10ecde;
L8c10ecde: wr(m,r[3]+0xcu,r[14],4); /* mov.l r14,@(0xc,r3) */
 goto L8c10ece0;
L8c10ece0: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c10ece2;
L8c10ece2: r[3]=rd(m,r[7],4); /* mov.l @r7,r3 */
 goto L8c10ece4;
L8c10ece4: *macl=r[6]*r[2]; /* mul.l r6,r2 */
 goto L8c10ece6;
L8c10ece6: r[3]+=4u; /* add #0x4,r3 */
 goto L8c10ece8;
L8c10ece8: r[2]=*macl; /* sts MACL,r2 */
 goto L8c10ecea;
L8c10ecea: r[2]+=r[3]; /* add r3,r2 */
 goto L8c10ecec;
L8c10ecec: wr(m,r[2]+0x10u,r[14],4); /* mov.l r14,@(0x10,r2) */
 goto L8c10ecee;
L8c10ecee: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c10ecf0;
L8c10ecf0: r[2]=rd(m,r[7],4); /* mov.l @r7,r2 */
 goto L8c10ecf2;
L8c10ecf2: *macl=r[6]*r[3]; /* mul.l r6,r3 */
 goto L8c10ecf4;
L8c10ecf4: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10ecf6;
L8c10ecf6: r[3]=*macl; /* sts MACL,r3 */
 goto L8c10ecf8;
L8c10ecf8: r[3]+=r[2]; /* add r2,r3 */
 goto L8c10ecfa;
L8c10ecfa: wr(m,r[3]+0x14u,r[14],4); /* mov.l r14,@(0x14,r3) */
 goto L8c10ecfc;
L8c10ecfc: r[1]=3u; /* mov #0x3,r1 */
 goto L8c10ecfe;
L8c10ecfe: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c10ed00;
L8c10ed00: r[3]=rd(m,r[7],4); /* mov.l @r7,r3 */
 goto L8c10ed02;
L8c10ed02: *macl=r[6]*r[2]; /* mul.l r6,r2 */
 goto L8c10ed04;
L8c10ed04: r[3]+=4u; /* add #0x4,r3 */
 goto L8c10ed06;
L8c10ed06: r[2]=*macl; /* sts MACL,r2 */
 goto L8c10ed08;
L8c10ed08: r[2]+=r[3]; /* add r3,r2 */
 goto L8c10ed0a;
L8c10ed0a: wr(m,r[2]+0x18u,r[1],4); /* mov.l r1,@(0x18,r2) */
 goto L8c10ed0c;
L8c10ed0c: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c10ed0e;
L8c10ed0e: r[3]=rd(m,r[7],4); /* mov.l @r7,r3 */
 goto L8c10ed10;
L8c10ed10: *macl=r[6]*r[2]; /* mul.l r6,r2 */
 goto L8c10ed12;
L8c10ed12: r[3]+=4u; /* add #0x4,r3 */
 goto L8c10ed14;
L8c10ed14: r[2]=*macl; /* sts MACL,r2 */
 goto L8c10ed16;
L8c10ed16: r[2]+=r[3]; /* add r3,r2 */
 goto L8c10ed18;
L8c10ed18: wr(m,r[2]+0x1cu,r[14],4); /* mov.l r14,@(0x1c,r2) */
 goto L8c10ed1a;
L8c10ed1a: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c10ed1c;
L8c10ed1c: r[2]=rd(m,r[7],4); /* mov.l @r7,r2 */
 goto L8c10ed1e;
L8c10ed1e: *macl=r[6]*r[3]; /* mul.l r6,r3 */
 goto L8c10ed20;
L8c10ed20: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10ed22;
L8c10ed22: r[3]=1u; /* mov #0x1,r3 */
 goto L8c10ed24;
L8c10ed24: r[6]=*macl; /* sts MACL,r6 */
 goto L8c10ed26;
L8c10ed26: r[6]+=r[2]; /* add r2,r6 */
 goto L8c10ed28;
L8c10ed28: r[2]=*sr; /* stc SR,r2 */
 goto L8c10ed2a;
L8c10ed2a: wr(m,r[6]+0x20u,r[14],4); /* mov.l r14,@(0x20,r6) */
 goto L8c10ed2c;
L8c10ed2c: wr(m,r[5]+0x28u,r[3],4); /* mov.l r3,@(0x28,r5) */
 goto L8c10ed2e;
L8c10ed2e: r[3]=sx(m,0x8c10ed4au,2); /* mov.w 0x8c10ed4a,r3 */
 goto L8c10ed30;
L8c10ed30: wr(m,r[4],r[5],4); /* mov.l r5,@r4 */
 goto L8c10ed32;
L8c10ed32: r[0]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r0 */
 goto L8c10ed34;
L8c10ed34: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c10ed36;
L8c10ed36: r[0]<<=2; /* shll2 r0 */
 goto L8c10ed38;
L8c10ed38: r[0]<<=2; /* shll2 r0 */
 goto L8c10ed3a;
L8c10ed3a: r[2]&=r[3]; /* and r3,r2 */
 goto L8c10ed3c;
L8c10ed3c: r[0]|=r[2]; /* or r2,r0 */
 goto L8c10ed3e;
L8c10ed3e: *sr=r[0]; /* ldc r0,SR */
 goto L8c10ed40;
L8c10ed40: r[0]=r[13]; /* mov r13,r0 */
 goto L8c10ed42;
L8c10ed42: r[15]+=12u; /* add #0xc,r15 */
 goto L8c10ed44;
L8c10ed44: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c10ed46;
L8c10ed46: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
