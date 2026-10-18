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
/* Static C recovery of retail Tikal helper 8c65eccc..8c65eeda.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_backend_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65eccc;
L8c65eccc: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65ecce;
L8c65ecce: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65ecd0;
L8c65ecd0: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65ecd2;
L8c65ecd2: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65ecd4;
L8c65ecd4: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65ecd6;
L8c65ecd6: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c65ecd8;
L8c65ecd8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65ecda;
L8c65ecda: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c65ecdc;
L8c65ecdc: r[3]=rd(m,0x8c65ee28u,4); /* mov.l 0x8c65ee28,r3 */
 goto L8c65ecde;
L8c65ecde: r[11]=31u; /* mov #0x1f,r11 */
 goto L8c65ece0;
L8c65ece0: r[14]=rd(m,0x8c65ee24u,4); /* mov.l 0x8c65ee24,r14 */
 goto L8c65ece2;
L8c65ece2: r[12]=r[7]; /* mov r7,r12 */
 goto L8c65ece4;
L8c65ece4: r[0]=sx(m,0x8c65ee10u,2); /* mov.w 0x8c65ee10,r0 */
 goto L8c65ece6;
L8c65ece6: r[2]=r[3]; /* mov r3,r2 */
 goto L8c65ece8;
L8c65ece8: wr(m,r[15]+0x8u,r[4],4); /* mov.l r4,@(0x8,r15) */
 goto L8c65ecea;
L8c65ecea: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c65ecec;
L8c65ecec: wr(m,r[14],r[3],4); /* mov.l r3,@r14 */
 goto L8c65ecee;
L8c65ecee: wr(m,r[2]+r[0],r[6],4); /* mov.l r6,@(r0,r2) */
 goto L8c65ecf0;
L8c65ecf0: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ecf2;
L8c65ecf2: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c65ecf4;
L8c65ecf4: r[0]=sx(m,0x8c65ee12u,2); /* mov.w 0x8c65ee12,r0 */
 goto L8c65ecf6;
L8c65ecf6: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65ecf8;
L8c65ecf8: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c65ecfa;
L8c65ecfa: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ecfc;
L8c65ecfc: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ecfe;
L8c65ecfe: *sr=(*sr&~1u)|!(r[11]&r[2]); /* tst r11,r2 */
 goto L8c65ed00;
L8c65ed00: take=(*sr&1u);r[9]=4294967264u;if(take)goto L8c65ed16;goto L8c65ed04; /* bt/s 0x8c65ed16 */
 goto L8c65ed04;
L8c65ed04: r[0]=sx(m,0x8c65ee10u,2); /* mov.w 0x8c65ee10,r0 */
 goto L8c65ed06;
L8c65ed06: r[1]=rd(m,r[14],4); /* mov.l @r14,r1 */
 goto L8c65ed08;
L8c65ed08: r[3]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r3 */
 goto L8c65ed0a;
L8c65ed0a: r[3]+=32u; /* add #0x20,r3 */
 goto L8c65ed0c;
L8c65ed0c: wr(m,r[1]+r[0],r[3],4); /* mov.l r3,@(r0,r1) */
 goto L8c65ed0e;
L8c65ed0e: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ed10;
L8c65ed10: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ed12;
L8c65ed12: r[2]&=r[9]; /* and r9,r2 */
 goto L8c65ed14;
L8c65ed14: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65ed16;
L8c65ed16: r[0]=sx(m,0x8c65ee12u,2); /* mov.w 0x8c65ee12,r0 */
 goto L8c65ed18;
L8c65ed18: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c65ed1a;
L8c65ed1a: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c65ed1c;
L8c65ed1c: *sr=(*sr&~1u)|!(r[11]&r[3]); /* tst r11,r3 */
 goto L8c65ed1e;
L8c65ed1e: if((*sr&1u))goto L8c65ed2e; /* bt 0x8c65ed2e */
 goto L8c65ed20;
L8c65ed20: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c65ed22;
L8c65ed22: r[3]+=32u; /* add #0x20,r3 */
 goto L8c65ed24;
L8c65ed24: wr(m,r[4]+r[0],r[3],4); /* mov.l r3,@(r0,r4) */
 goto L8c65ed26;
L8c65ed26: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ed28;
L8c65ed28: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ed2a;
L8c65ed2a: r[2]&=r[9]; /* and r9,r2 */
 goto L8c65ed2c;
L8c65ed2c: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65ed2e;
L8c65ed2e: r[0]=sx(m,0x8c65ee10u,2); /* mov.w 0x8c65ee10,r0 */
 goto L8c65ed30;
L8c65ed30: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ed32;
L8c65ed32: r[10]=rd(m,0x8c65ee2cu,4); /* mov.l 0x8c65ee2c,r10 */
 goto L8c65ed34;
L8c65ed34: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ed36;
L8c65ed36: r[2]|=r[10]; /* or r10,r2 */
 goto L8c65ed38;
L8c65ed38: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65ed3a;
L8c65ed3a: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65ed3c;
L8c65ed3c: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ed3e;
L8c65ed3e: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ed40;
L8c65ed40: r[2]|=r[10]; /* or r10,r2 */
 goto L8c65ed42;
L8c65ed42: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65ed44;
L8c65ed44: r[3]=rd(m,0x8c65ee30u,4); /* mov.l 0x8c65ee30,r3 */
 goto L8c65ed46;
L8c65ed46: target=r[3];*pr=0x8c65ed4au;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65ed4a;
L8c65ed4a: r[13]=0u; /* mov #0x0,r13 */
 goto L8c65ed4c;
L8c65ed4c: r[4]=r[13]; /* mov r13,r4 */
 goto L8c65ed4e;
L8c65ed4e: r[5]=96u; /* mov #0x60,r5 */
 goto L8c65ed50;
L8c65ed50: r[3]=sx(m,0x8c65ee14u,2); /* mov.w 0x8c65ee14,r3 */
 goto L8c65ed52;
L8c65ed52: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65ed54;
L8c65ed54: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65ed56;
L8c65ed56: r[3]+=r[4]; /* add r4,r3 */
 goto L8c65ed58;
L8c65ed58: wr(m,r[3],r[13],4); /* mov.l r13,@r3 */
 goto L8c65ed5a;
L8c65ed5a: r[3]=sx(m,0x8c65ee16u,2); /* mov.w 0x8c65ee16,r3 */
 goto L8c65ed5c;
L8c65ed5c: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65ed5e;
L8c65ed5e: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65ed60;
L8c65ed60: r[3]+=r[4]; /* add r4,r3 */
 goto L8c65ed62;
L8c65ed62: r[4]+=4u; /* add #0x4,r4 */
 goto L8c65ed64;
L8c65ed64: *sr=(*sr&~1u)|(r[4]>=r[5]); /* cmp/hs r5,r4 */
 goto L8c65ed66;
L8c65ed66: take=!(*sr&1u);wr(m,r[3],r[13],4);if(take)goto L8c65ed50;goto L8c65ed6a; /* bf/s 0x8c65ed50 */
 goto L8c65ed6a;
L8c65ed6a: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c65ed6c;
L8c65ed6c: *sr=(*sr&~1u)|((int32_t)r[12]>0); /* cmp/pl r12 */
 goto L8c65ed6e;
L8c65ed6e: r[7]=sx(m,0x8c65ee18u,2); /* mov.w 0x8c65ee18,r7 */
 goto L8c65ed70;
L8c65ed70: wr(m,r[15]+0x4u,r[3],4); /* mov.l r3,@(0x4,r15) */
 goto L8c65ed72;
L8c65ed72: take=!(*sr&1u);r[5]=r[13];if(take)goto L8c65edb6;goto L8c65ed76; /* bf/s 0x8c65edb6 */
 goto L8c65ed76;
L8c65ed76: r[4]=rd(m,r[15],4); /* mov.l @r15,r4 */
 goto L8c65ed78;
L8c65ed78: r[4]+=4u; /* add #0x4,r4 */
 goto L8c65ed7a;
L8c65ed7a: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c65ed7c;
L8c65ed7c: r[4]+=4294967292u; /* add #-0x4,r4 */
 goto L8c65ed7e;
L8c65ed7e: r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c65ed80;
L8c65ed80: r[3]=r[4]; /* mov r4,r3 */
 goto L8c65ed82;
L8c65ed82: *sr=(*sr&~1u)|!(r[11]&r[3]); /* tst r11,r3 */
 goto L8c65ed84;
L8c65ed84: if((*sr&1u))goto L8c65ed8a; /* bt 0x8c65ed8a */
 goto L8c65ed86;
L8c65ed86: r[4]+=32u; /* add #0x20,r4 */
 goto L8c65ed88;
L8c65ed88: r[4]&=r[9]; /* and r9,r4 */
 goto L8c65ed8a;
L8c65ed8a: r[1]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r1 */
 goto L8c65ed8c;
L8c65ed8c: r[4]|=r[10]; /* or r10,r4 */
 goto L8c65ed8e;
L8c65ed8e: r[3]=sx(m,0x8c65ee14u,2); /* mov.w 0x8c65ee14,r3 */
 goto L8c65ed90;
L8c65ed90: r[5]+=1u; /* add #0x1,r5 */
 goto L8c65ed92;
L8c65ed92: r[1]+=4u; /* add #0x4,r1 */
 goto L8c65ed94;
L8c65ed94: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[12]); /* cmp/ge r12,r5 */
 goto L8c65ed96;
L8c65ed96: wr(m,r[15]+0x4u,r[1],4); /* mov.l r1,@(0x4,r15) */
 goto L8c65ed98;
L8c65ed98: r[1]+=4294967292u; /* add #-0x4,r1 */
 goto L8c65ed9a;
L8c65ed9a: r[1]=rd(m,r[1],4); /* mov.l @r1,r1 */
 goto L8c65ed9c;
L8c65ed9c: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65ed9e;
L8c65ed9e: r[6]=r[1]; /* mov r1,r6 */
 goto L8c65eda0;
L8c65eda0: r[6]<<=2; /* shll2 r6 */
 goto L8c65eda2;
L8c65eda2: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65eda4;
L8c65eda4: r[3]+=r[6]; /* add r6,r3 */
 goto L8c65eda6;
L8c65eda6: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c65eda8;
L8c65eda8: r[4]+=r[7]; /* add r7,r4 */
 goto L8c65edaa;
L8c65edaa: r[3]=sx(m,0x8c65ee16u,2); /* mov.w 0x8c65ee16,r3 */
 goto L8c65edac;
L8c65edac: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65edae;
L8c65edae: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65edb0;
L8c65edb0: r[6]+=r[3]; /* add r3,r6 */
 goto L8c65edb2;
L8c65edb2: take=!(*sr&1u);wr(m,r[6],r[4],4);if(take)goto L8c65ed76;goto L8c65edb6; /* bf/s 0x8c65ed76 */
 goto L8c65edb6;
L8c65edb6: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65edb8;
L8c65edb8: r[4]=r[13]; /* mov r13,r4 */
 goto L8c65edba;
L8c65edba: r[0]=sx(m,0x8c65ee10u,2); /* mov.w 0x8c65ee10,r0 */
 goto L8c65edbc;
L8c65edbc: r[5]=24u; /* mov #0x18,r5 */
 goto L8c65edbe;
L8c65edbe: r[2]=r[3]; /* mov r3,r2 */
 goto L8c65edc0;
L8c65edc0: r[1]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c65edc2;
L8c65edc2: r[0]=sx(m,0x8c65ee1au,2); /* mov.w 0x8c65ee1a,r0 */
 goto L8c65edc4;
L8c65edc4: wr(m,r[3]+r[0],r[1],4); /* mov.l r1,@(r0,r3) */
 goto L8c65edc6;
L8c65edc6: r[1]=r[12]; /* mov r12,r1 */
 goto L8c65edc8;
L8c65edc8: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65edca;
L8c65edca: r[1]<<=8; /* shll8 r1 */
 goto L8c65edcc;
L8c65edcc: r[0]=sx(m,0x8c65ee10u,2); /* mov.w 0x8c65ee10,r0 */
 goto L8c65edce;
L8c65edce: r[1]<<=2; /* shll2 r1 */
 goto L8c65edd0;
L8c65edd0: r[2]=r[3]; /* mov r3,r2 */
 goto L8c65edd2;
L8c65edd2: r[2]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r2 */
 goto L8c65edd4;
L8c65edd4: r[0]=sx(m,0x8c65ee1cu,2); /* mov.w 0x8c65ee1c,r0 */
 goto L8c65edd6;
L8c65edd6: r[1]+=r[2]; /* add r2,r1 */
 goto L8c65edd8;
L8c65edd8: wr(m,r[3]+r[0],r[1],4); /* mov.l r1,@(r0,r3) */
 goto L8c65edda;
L8c65edda: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c65eddc;
L8c65eddc: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65edde;
L8c65edde: r[2]=r[3]; /* mov r3,r2 */
 goto L8c65ede0;
L8c65ede0: r[1]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c65ede2;
L8c65ede2: r[0]=sx(m,0x8c65ee1eu,2); /* mov.w 0x8c65ee1e,r0 */
 goto L8c65ede4;
L8c65ede4: wr(m,r[3]+r[0],r[1],4); /* mov.l r1,@(r0,r3) */
 goto L8c65ede6;
L8c65ede6: r[3]=sx(m,0x8c65ee20u,2); /* mov.w 0x8c65ee20,r3 */
 goto L8c65ede8;
L8c65ede8: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65edea;
L8c65edea: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65edec;
L8c65edec: r[3]+=r[4]; /* add r4,r3 */
 goto L8c65edee;
L8c65edee: r[4]+=1u; /* add #0x1,r4 */
 goto L8c65edf0;
L8c65edf0: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]); /* cmp/ge r5,r4 */
 goto L8c65edf2;
L8c65edf2: take=!(*sr&1u);wr(m,r[3],r[13],1);if(take)goto L8c65ede6;goto L8c65edf6; /* bf/s 0x8c65ede6 */
 goto L8c65edf6;
L8c65edf6: r[11]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r11 */
 goto L8c65edf8;
L8c65edf8: *sr=(*sr&~1u)|((int32_t)r[12]>0); /* cmp/pl r12 */
 goto L8c65edfa;
L8c65edfa: r[7]=101u; /* mov #0x65,r7 */
 goto L8c65edfc;
L8c65edfc: r[5]=r[13]; /* mov r13,r5 */
 goto L8c65edfe;
L8c65edfe: take=!(*sr&1u);r[6]=114u;if(take)goto L8c65ee5e;goto L8c65ee02; /* bf/s 0x8c65ee5e */
 goto L8c65ee02;
L8c65ee02: r[4]=rd(m,r[11],4);r[11]+=4; /* mov.l @r11+,r4 */
 goto L8c65ee04;
L8c65ee04: r[0]=r[4]; /* mov r4,r0 */
 goto L8c65ee06;
L8c65ee06: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c65ee08;
L8c65ee08: if((*sr&1u))goto L8c65ee42; /* bt 0x8c65ee42 */
 goto L8c65ee0a;
L8c65ee0a: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c65ee0c;
L8c65ee0c: goto L8c65ee34; /* bra 0x8c65ee34 */
 goto L8c65ee34;
L8c65ee34: if((*sr&1u))goto L8c65ee42; /* bt 0x8c65ee42 */
 goto L8c65ee36;
L8c65ee36: *sr=(*sr&~1u)|(r[0]==0xcu); /* cmp/eq #0xc,r0 */
 goto L8c65ee38;
L8c65ee38: if((*sr&1u))goto L8c65ee42; /* bt 0x8c65ee42 */
 goto L8c65ee3a;
L8c65ee3a: *sr=(*sr&~1u)|(r[0]==0x12u); /* cmp/eq #0x12,r0 */
 goto L8c65ee3c;
L8c65ee3c: if((*sr&1u))goto L8c65ee42; /* bt 0x8c65ee42 */
 goto L8c65ee3e;
L8c65ee3e: goto L8c65ee4e; /* bra 0x8c65ee4e */
 goto L8c65ee42;
L8c65ee42: r[3]=sx(m,0x8c65ef0eu,2); /* mov.w 0x8c65ef0e,r3 */
 goto L8c65ee44;
L8c65ee44: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65ee46;
L8c65ee46: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65ee48;
L8c65ee48: r[4]+=r[3]; /* add r3,r4 */
 goto L8c65ee4a;
L8c65ee4a: wr(m,r[4],r[6],1);goto L8c65ee58; /* bra 0x8c65ee58 */
 goto L8c65ee4e;
L8c65ee4e: r[3]=sx(m,0x8c65ef0eu,2); /* mov.w 0x8c65ef0e,r3 */
 goto L8c65ee50;
L8c65ee50: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65ee52;
L8c65ee52: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65ee54;
L8c65ee54: r[4]+=r[3]; /* add r3,r4 */
 goto L8c65ee56;
L8c65ee56: wr(m,r[4],r[7],1); /* mov.b r7,@r4 */
 goto L8c65ee58;
L8c65ee58: r[5]+=1u; /* add #0x1,r5 */
 goto L8c65ee5a;
L8c65ee5a: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[12]); /* cmp/ge r12,r5 */
 goto L8c65ee5c;
L8c65ee5c: if(!(*sr&1u))goto L8c65ee02; /* bf 0x8c65ee02 */
 goto L8c65ee5e;
L8c65ee5e: r[4]=rd(m,0x8c65ef1cu,4); /* mov.l 0x8c65ef1c,r4 */
 goto L8c65ee60;
L8c65ee60: r[9]=rd(m,0x8c65ef20u,4); /* mov.l 0x8c65ef20,r9 */
 goto L8c65ee62;
L8c65ee62: r[10]=r[4]; /* mov r4,r10 */
 goto L8c65ee64;
L8c65ee64: r[10]+=16u; /* add #0x10,r10 */
 goto L8c65ee66;
L8c65ee66: r[5]=r[4]; /* mov r4,r5 */
 goto L8c65ee68;
L8c65ee68: r[11]=r[4]; /* mov r4,r11 */
 goto L8c65ee6a;
L8c65ee6a: r[12]=r[4]; /* mov r4,r12 */
 goto L8c65ee6c;
L8c65ee6c: r[2]=sx(m,0x8c65ef0eu,2); /* mov.w 0x8c65ef0e,r2 */
 goto L8c65ee6e;
L8c65ee6e: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65ee70;
L8c65ee70: r[3]=rd(m,r[12],4); /* mov.l @r12,r3 */
 goto L8c65ee72;
L8c65ee72: r[2]+=r[0]; /* add r0,r2 */
 goto L8c65ee74;
L8c65ee74: r[2]+=r[3]; /* add r3,r2 */
 goto L8c65ee76;
L8c65ee76: r[1]=sx(m,r[2],1); /* mov.b @r2,r1 */
 goto L8c65ee78;
L8c65ee78: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c65ee7a;
L8c65ee7a: if((*sr&1u))goto L8c65ee80; /* bt 0x8c65ee80 */
 goto L8c65ee7c;
L8c65ee7c: target=r[9];*pr=0x8c65ee80u;r[4]=rd(m,r[11],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto L8c65ee80;
L8c65ee80: r[12]+=4u; /* add #0x4,r12 */
 goto L8c65ee82;
L8c65ee82: *sr=(*sr&~1u)|(r[12]>=r[10]); /* cmp/hs r10,r12 */
 goto L8c65ee84;
L8c65ee84: take=!(*sr&1u);r[11]+=4u;if(take)goto L8c65ee6c;goto L8c65ee88; /* bf/s 0x8c65ee6c */
 goto L8c65ee88;
L8c65ee88: r[2]=rd(m,0x8c65ef24u,4); /* mov.l 0x8c65ef24,r2 */
 goto L8c65ee8a;
L8c65ee8a: target=r[2];*pr=0x8c65ee8eu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65ee8e;
L8c65ee8e: r[0]=sx(m,0x8c65ef10u,2); /* mov.w 0x8c65ef10,r0 */
 goto L8c65ee90;
L8c65ee90: r[4]=1u; /* mov #0x1,r4 */
 goto L8c65ee92;
L8c65ee92: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ee94;
L8c65ee94: r[1]=rd(m,0x8c65ef28u,4); /* mov.l 0x8c65ef28,r1 */
 goto L8c65ee96;
L8c65ee96: wr(m,r[3]+r[0],r[4],4); /* mov.l r4,@(r0,r3) */
 goto L8c65ee98;
L8c65ee98: r[0]=sx(m,0x8c65ef12u,2); /* mov.w 0x8c65ef12,r0 */
 goto L8c65ee9a;
L8c65ee9a: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ee9c;
L8c65ee9c: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ee9e;
L8c65ee9e: r[0]=sx(m,0x8c65ef14u,2); /* mov.w 0x8c65ef14,r0 */
 goto L8c65eea0;
L8c65eea0: wr(m,r[2]+r[0],r[1],4); /* mov.l r1,@(r0,r2) */
 goto L8c65eea2;
L8c65eea2: r[0]=sx(m,0x8c65ef12u,2); /* mov.w 0x8c65ef12,r0 */
 goto L8c65eea4;
L8c65eea4: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65eea6;
L8c65eea6: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65eea8;
L8c65eea8: wr(m,r[2]+0x10u,r[13],4); /* mov.l r13,@(0x10,r2) */
 goto L8c65eeaa;
L8c65eeaa: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65eeac;
L8c65eeac: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65eeae;
L8c65eeae: r[0]+=4294967260u; /* add #-0x24,r0 */
 goto L8c65eeb0;
L8c65eeb0: r[1]=r[3]; /* mov r3,r1 */
 goto L8c65eeb2;
L8c65eeb2: r[3]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r3 */
 goto L8c65eeb4;
L8c65eeb4: r[0]+=4294967280u; /* add #-0x10,r0 */
 goto L8c65eeb6;
L8c65eeb6: r[1]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r1 */
 goto L8c65eeb8;
L8c65eeb8: r[0]=sx(m,0x8c65ef16u,2); /* mov.w 0x8c65ef16,r0 */
 goto L8c65eeba;
L8c65eeba: r[1]|=r[3]; /* or r3,r1 */
 goto L8c65eebc;
L8c65eebc: wr(m,r[2]+r[0],r[1],4); /* mov.l r1,@(r0,r2) */
 goto L8c65eebe;
L8c65eebe: r[0]=sx(m,0x8c65ef12u,2); /* mov.w 0x8c65ef12,r0 */
 goto L8c65eec0;
L8c65eec0: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65eec2;
L8c65eec2: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65eec4;
L8c65eec4: wr(m,r[2]+0x14u,r[4],4); /* mov.l r4,@(0x14,r2) */
 goto L8c65eec6;
L8c65eec6: r[0]=r[13]; /* mov r13,r0 */
 goto L8c65eec8;
L8c65eec8: r[15]+=12u; /* add #0xc,r15 */
 goto L8c65eeca;
L8c65eeca: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65eecc;
L8c65eecc: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c65eece;
L8c65eece: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65eed0;
L8c65eed0: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65eed2;
L8c65eed2: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65eed4;
L8c65eed4: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65eed6;
L8c65eed6: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
