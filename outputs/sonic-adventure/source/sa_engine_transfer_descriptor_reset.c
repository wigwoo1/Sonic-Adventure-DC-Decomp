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
/* Static C recovery of USA Rev A transfer initialization. Hardware accesses require explicit IO specialization. */
SA_STAGE_EXPORT int sa_engine_transfer_descriptor_reset(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ed54;
L8c10ed54: r[7]=rd(m,r[4],4); /* mov.l @r4,r7 */
 goto L8c10ed56;
L8c10ed56: r[6]=52u; /* mov #0x34,r6 */
 goto L8c10ed58;
L8c10ed58: r[5]=rd(m,0x8c10ee1cu,4); /* mov.l 0x8c10ee1c,r5 */
 goto L8c10ed5a;
L8c10ed5a: *macl=r[6]*r[7]; /* mul.l r6,r7 */
 goto L8c10ed5c;
L8c10ed5c: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c10ed5e;
L8c10ed5e: r[3]+=4u; /* add #0x4,r3 */
 goto L8c10ed60;
L8c10ed60: r[2]=*macl; /* sts MACL,r2 */
 goto L8c10ed62;
L8c10ed62: r[2]+=r[3]; /* add r3,r2 */
 goto L8c10ed64;
L8c10ed64: wr(m,r[2],r[7],4); /* mov.l r7,@r2 */
 goto L8c10ed66;
L8c10ed66: r[7]=0u; /* mov #0x0,r7 */
 goto L8c10ed68;
L8c10ed68: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c10ed6a;
L8c10ed6a: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c10ed6c;
L8c10ed6c: *macl=r[6]*r[3]; /* mul.l r6,r3 */
 goto L8c10ed6e;
L8c10ed6e: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10ed70;
L8c10ed70: r[3]=*macl; /* sts MACL,r3 */
 goto L8c10ed72;
L8c10ed72: r[3]+=r[2]; /* add r2,r3 */
 goto L8c10ed74;
L8c10ed74: wr(m,r[3]+0x28u,r[7],4); /* mov.l r7,@(0x28,r3) */
 goto L8c10ed76;
L8c10ed76: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c10ed78;
L8c10ed78: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c10ed7a;
L8c10ed7a: *macl=r[6]*r[2]; /* mul.l r6,r2 */
 goto L8c10ed7c;
L8c10ed7c: r[3]+=4u; /* add #0x4,r3 */
 goto L8c10ed7e;
L8c10ed7e: r[2]=*macl; /* sts MACL,r2 */
 goto L8c10ed80;
L8c10ed80: r[2]+=r[3]; /* add r3,r2 */
 goto L8c10ed82;
L8c10ed82: wr(m,r[2]+0x2cu,r[7],4); /* mov.l r7,@(0x2c,r2) */
 goto L8c10ed84;
L8c10ed84: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c10ed86;
L8c10ed86: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c10ed88;
L8c10ed88: *macl=r[6]*r[3]; /* mul.l r6,r3 */
 goto L8c10ed8a;
L8c10ed8a: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10ed8c;
L8c10ed8c: r[3]=*macl; /* sts MACL,r3 */
 goto L8c10ed8e;
L8c10ed8e: r[3]+=r[2]; /* add r2,r3 */
 goto L8c10ed90;
L8c10ed90: wr(m,r[3]+0x30u,r[7],4); /* mov.l r7,@(0x30,r3) */
 goto L8c10ed92;
L8c10ed92: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c10ed94;
L8c10ed94: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c10ed96;
L8c10ed96: r[3]+=4u; /* add #0x4,r3 */
 goto L8c10ed98;
L8c10ed98: *macl=r[6]*r[2]; /* mul.l r6,r2 */
 goto L8c10ed9a;
L8c10ed9a: r[2]=*macl; /* sts MACL,r2 */
 goto L8c10ed9c;
L8c10ed9c: r[2]+=r[3]; /* add r3,r2 */
 goto L8c10ed9e;
L8c10ed9e: wr(m,r[2]+0x4u,r[7],4); /* mov.l r7,@(0x4,r2) */
 goto L8c10eda0;
L8c10eda0: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c10eda2;
L8c10eda2: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c10eda4;
L8c10eda4: *macl=r[6]*r[3]; /* mul.l r6,r3 */
 goto L8c10eda6;
L8c10eda6: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10eda8;
L8c10eda8: r[3]=*macl; /* sts MACL,r3 */
 goto L8c10edaa;
L8c10edaa: r[3]+=r[2]; /* add r2,r3 */
 goto L8c10edac;
L8c10edac: wr(m,r[3]+0x8u,r[7],4); /* mov.l r7,@(0x8,r3) */
 goto L8c10edae;
L8c10edae: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c10edb0;
L8c10edb0: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c10edb2;
L8c10edb2: *macl=r[6]*r[2]; /* mul.l r6,r2 */
 goto L8c10edb4;
L8c10edb4: r[3]+=4u; /* add #0x4,r3 */
 goto L8c10edb6;
L8c10edb6: r[2]=*macl; /* sts MACL,r2 */
 goto L8c10edb8;
L8c10edb8: r[2]+=r[3]; /* add r3,r2 */
 goto L8c10edba;
L8c10edba: wr(m,r[2]+0xcu,r[7],4); /* mov.l r7,@(0xc,r2) */
 goto L8c10edbc;
L8c10edbc: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c10edbe;
L8c10edbe: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c10edc0;
L8c10edc0: *macl=r[6]*r[3]; /* mul.l r6,r3 */
 goto L8c10edc2;
L8c10edc2: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10edc4;
L8c10edc4: r[3]=*macl; /* sts MACL,r3 */
 goto L8c10edc6;
L8c10edc6: r[3]+=r[2]; /* add r2,r3 */
 goto L8c10edc8;
L8c10edc8: wr(m,r[3]+0x10u,r[7],4); /* mov.l r7,@(0x10,r3) */
 goto L8c10edca;
L8c10edca: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c10edcc;
L8c10edcc: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c10edce;
L8c10edce: *macl=r[6]*r[2]; /* mul.l r6,r2 */
 goto L8c10edd0;
L8c10edd0: r[3]+=4u; /* add #0x4,r3 */
 goto L8c10edd2;
L8c10edd2: r[2]=*macl; /* sts MACL,r2 */
 goto L8c10edd4;
L8c10edd4: r[2]+=r[3]; /* add r3,r2 */
 goto L8c10edd6;
L8c10edd6: wr(m,r[2]+0x14u,r[7],4); /* mov.l r7,@(0x14,r2) */
 goto L8c10edd8;
L8c10edd8: r[1]=3u; /* mov #0x3,r1 */
 goto L8c10edda;
L8c10edda: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c10eddc;
L8c10eddc: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c10edde;
L8c10edde: *macl=r[6]*r[3]; /* mul.l r6,r3 */
 goto L8c10ede0;
L8c10ede0: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10ede2;
L8c10ede2: r[3]=*macl; /* sts MACL,r3 */
 goto L8c10ede4;
L8c10ede4: r[3]+=r[2]; /* add r2,r3 */
 goto L8c10ede6;
L8c10ede6: wr(m,r[3]+0x18u,r[1],4); /* mov.l r1,@(0x18,r3) */
 goto L8c10ede8;
L8c10ede8: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c10edea;
L8c10edea: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c10edec;
L8c10edec: *macl=r[6]*r[2]; /* mul.l r6,r2 */
 goto L8c10edee;
L8c10edee: r[3]+=4u; /* add #0x4,r3 */
 goto L8c10edf0;
L8c10edf0: r[2]=*macl; /* sts MACL,r2 */
 goto L8c10edf2;
L8c10edf2: r[2]+=r[3]; /* add r3,r2 */
 goto L8c10edf4;
L8c10edf4: wr(m,r[2]+0x1cu,r[7],4); /* mov.l r7,@(0x1c,r2) */
 goto L8c10edf6;
L8c10edf6: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c10edf8;
L8c10edf8: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c10edfa;
L8c10edfa: *macl=r[6]*r[3]; /* mul.l r6,r3 */
 goto L8c10edfc;
L8c10edfc: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10edfe;
L8c10edfe: r[6]=*macl; /* sts MACL,r6 */
 goto L8c10ee00;
L8c10ee00: r[6]+=r[2]; /* add r2,r6 */
 goto L8c10ee02;
L8c10ee02: wr(m,r[6]+0x20u,r[7],4); /* mov.l r7,@(0x20,r6) */
 goto L8c10ee04;
L8c10ee04: r[1]=rd(m,r[4],4); /* mov.l @r4,r1 */
 goto L8c10ee06;
L8c10ee06: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c10ee08;
L8c10ee08: r[1]<<=2; /* shll2 r1 */
 goto L8c10ee0a;
L8c10ee0a: r[3]=rd(m,r[3],4); /* mov.l @r3,r3 */
 goto L8c10ee0c;
L8c10ee0c: r[1]<<=2; /* shll2 r1 */
 goto L8c10ee0e;
L8c10ee0e: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c10ee10;
L8c10ee10: r[1]+=r[3]; /* add r3,r1 */
 goto L8c10ee12;
L8c10ee12: r[4]=r[1]; /* mov r1,r4 */
 goto L8c10ee14;
L8c10ee14: wr(m,r[4]+0x14u,r[7],4); /* mov.l r7,@(0x14,r4) */
 goto L8c10ee16;
L8c10ee16: r[0]=r[7];return 0; /* rts */
}
