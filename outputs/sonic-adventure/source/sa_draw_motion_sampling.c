/* Key search, linear interpolation and paired-motion dispatch; names inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_add_sub.h"
#include "sh4_vector_numeric.h"
#include "sh4_fmac.h"
#include "sh4_divide.h"
#include "sh4_multiply_extended.h"
#include "sh4_float_i32.h"
#include "sh4_compare_bits.h"
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
typedef int (*sa_shared_service)(void *,uint32_t,uint8_t *,
    struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=0;for(unsigned i=0;i<n;i++)value|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return value;
}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)value:(uint32_t)(int32_t)(int16_t)value;
}
static void wr(uint8_t *m,uint32_t a,uint32_t value,unsigned n){
 for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(value>>(8*i));
}
static void fload(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  f->bank[bank][reg]=rd(m,a,4);f->bank[bank][reg+1]=rd(m,a+4u,4);
 }else f->bank[bank][reg]=rd(m,a,4);
}
static void fstore(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  wr(m,a,f->bank[bank][reg],4);wr(m,a+4u,f->bank[bank][reg+1],4);
 }else wr(m,a,f->bank[bank][reg],4);
}
static void fcopy(struct sa_fpu_banks *f,unsigned n,unsigned s){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){unsigned nb=bank^(n&1u),sb=bank^(s&1u);n&=~1u;s&=~1u;
  uint32_t a=f->bank[sb][s],b=f->bank[sb][s+1];f->bank[nb][n]=a;f->bank[nb][n+1]=b;
 }else f->bank[bank][n]=f->bank[bank][s];
}
static int valid_ram(uint32_t a,unsigned n){
 return (a&0x1f000000u)==0x0c000000u && (a&0xffffffu)<=0x1000000u-n;
}
SA_STAGE_EXPORT int sa_motion_sampling_8c03feb8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c03feb8;
L8c03feb8: *stop=0x8c03feb8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c03feba; /* mov.l r14,@-r15 */
L8c03feba: *stop=0x8c03febau;r[14]=0u;goto L8c03febc; /* mov #0x0,r14 */
L8c03febc: *stop=0x8c03febcu;r[3]=rd(m,0x8c04007cu,4);goto L8c03febe; /* mov.l 0x8c04007c,r3 */
L8c03febe: *stop=0x8c03febeu;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[3]);goto L8c03fec0; /* fmov.s @r3,fr3 */
L8c03fec0: *stop=0x8c03fec0u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c03fec2; /* ftrc fr3,FPUL */
L8c03fec2: *stop=0x8c03fec4u;r[1]=*fpul;goto L8c03fee0; /* bra 0x8c03fee0 */
L8c03fec6: *stop=0x8c03fec6u;r[7]=r[5];goto L8c03fec8; /* mov r5,r7 */
L8c03fec8: *stop=0x8c03fec8u;r[7]+=r[14];goto L8c03feca; /* add r14,r7 */
L8c03feca: *stop=0x8c03fecau;*sr=(*sr&~1u)|(r[7]&1u);r[7]>>=1;goto L8c03fecc; /* shlr r7 */
L8c03fecc: *stop=0x8c03feccu;r[0]=r[7];goto L8c03fece; /* mov r7,r0 */
L8c03fece: *stop=0x8c03feceu;goto L8c03fed0; /* nop */
L8c03fed0: *stop=0x8c03fed0u;r[0]<<=2;goto L8c03fed2; /* shll2 r0 */
L8c03fed2: *stop=0x8c03fed2u;r[0]<<=2;goto L8c03fed4; /* shll2 r0 */
L8c03fed4: *stop=0x8c03fed4u;if(!valid_ram(r[0]+r[4],4u))return -3;r[0]=rd(m,r[4]+r[0],4);goto L8c03fed6; /* mov.l @(r0,r4),r0 */
L8c03fed6: *stop=0x8c03fed6u;*sr=(*sr&~1u)|(r[1]>=r[0]);goto L8c03fed8; /* cmp/hs r0,r1 */
L8c03fed8: if(!(*sr&1u))goto L8c03fede;goto L8c03feda; /* bf 0x8c03fede */
L8c03feda: *stop=0x8c03fedcu;r[14]=r[7];goto L8c03fee0; /* bra 0x8c03fee0 */
L8c03fede: *stop=0x8c03fedeu;r[5]=r[7];goto L8c03fee0; /* mov r7,r5 */
L8c03fee0: *stop=0x8c03fee0u;r[3]=r[5];goto L8c03fee2; /* mov r5,r3 */
L8c03fee2: *stop=0x8c03fee2u;r[3]-=r[14];goto L8c03fee4; /* sub r14,r3 */
L8c03fee4: *stop=0x8c03fee4u;r[3]+=4294967295u;goto L8c03fee6; /* add #-0x1,r3 */
L8c03fee6: *stop=0x8c03fee6u;r[2]=0u;goto L8c03fee8; /* mov #0x0,r2 */
L8c03fee8: *stop=0x8c03fee8u;*sr=(*sr&~1u)|(r[3]>r[2]);goto L8c03feea; /* cmp/hi r2,r3 */
L8c03feea: if((*sr&1u))goto L8c03fec6;goto L8c03feec; /* bt 0x8c03fec6 */
L8c03feec: *stop=0x8c03feecu;if(!valid_ram(r[6],4u))return -3;wr(m,r[6],r[14],4);goto L8c03feee; /* mov.l r14,@r6 */
L8c03feee: *stop=0x8c03feeeu;if(*pr!=entry_pr)return -3;*stop=0x8c03fef0u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_sampling_8c040a60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c040a60;
L8c040a60: *stop=0x8c040a60u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c040a62; /* mov.l r14,@-r15 */
L8c040a62: *stop=0x8c040a62u;r[3]=28u;goto L8c040a64; /* mov #0x1c,r3 */
L8c040a64: *stop=0x8c040a64u;r[1]=rd(m,0x8c040ce4u,4);goto L8c040a66; /* mov.l 0x8c040ce4,r1 */
L8c040a66: *stop=0x8c040a66u;r[14]=0u;goto L8c040a68; /* mov #0x0,r14 */
L8c040a68: *stop=0x8c040a68u;r[0]=rd(m,0x8c040ce8u,4);goto L8c040a6a; /* mov.l 0x8c040ce8,r0 */
L8c040a6a: *stop=0x8c040a6au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c040a6c; /* mov.l r12,@-r15 */
L8c040a6c: *stop=0x8c040a6cu;if(!valid_ram(r[1],1u))return -3;r[2]=sx(m,r[1],1);goto L8c040a6e; /* mov.b @r1,r2 */
L8c040a6e: *stop=0x8c040a6eu;*macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[2]);goto L8c040a70; /* muls.w r3,r2 */
L8c040a70: *stop=0x8c040a70u;r[12]=*macl;goto L8c040a72; /* sts MACL,r12 */
L8c040a72: *stop=0x8c040a72u;r[12]=(uint32_t)(int32_t)(int8_t)r[12];goto L8c040a74; /* exts.b r12,r12 */
L8c040a74: *stop=0x8c040a74u;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[12]);goto L8c040a76; /* fmov.s @(r0,r12),fr3 */
L8c040a76: *stop=0x8c040a76u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c040a78; /* ftrc fr3,FPUL */
L8c040a78: *stop=0x8c040a7au;r[12]=*fpul;goto L8c040a96; /* bra 0x8c040a96 */
L8c040a7c: *stop=0x8c040a7cu;r[7]=r[5];goto L8c040a7e; /* mov r5,r7 */
L8c040a7e: *stop=0x8c040a7eu;r[7]+=r[14];goto L8c040a80; /* add r14,r7 */
L8c040a80: *stop=0x8c040a80u;*sr=(*sr&~1u)|(r[7]&1u);r[7]>>=1;goto L8c040a82; /* shlr r7 */
L8c040a82: *stop=0x8c040a82u;r[0]=r[7];goto L8c040a84; /* mov r7,r0 */
L8c040a84: *stop=0x8c040a84u;goto L8c040a86; /* nop */
L8c040a86: *stop=0x8c040a86u;r[0]<<=2;goto L8c040a88; /* shll2 r0 */
L8c040a88: *stop=0x8c040a88u;r[0]<<=2;goto L8c040a8a; /* shll2 r0 */
L8c040a8a: *stop=0x8c040a8au;if(!valid_ram(r[0]+r[4],4u))return -3;r[0]=rd(m,r[4]+r[0],4);goto L8c040a8c; /* mov.l @(r0,r4),r0 */
L8c040a8c: *stop=0x8c040a8cu;*sr=(*sr&~1u)|(r[12]>=r[0]);goto L8c040a8e; /* cmp/hs r0,r12 */
L8c040a8e: if(!(*sr&1u))goto L8c040a94;goto L8c040a90; /* bf 0x8c040a94 */
L8c040a90: *stop=0x8c040a92u;r[14]=r[7];goto L8c040a96; /* bra 0x8c040a96 */
L8c040a94: *stop=0x8c040a94u;r[5]=r[7];goto L8c040a96; /* mov r7,r5 */
L8c040a96: *stop=0x8c040a96u;r[3]=r[5];goto L8c040a98; /* mov r5,r3 */
L8c040a98: *stop=0x8c040a98u;r[3]-=r[14];goto L8c040a9a; /* sub r14,r3 */
L8c040a9a: *stop=0x8c040a9au;r[3]+=4294967295u;goto L8c040a9c; /* add #-0x1,r3 */
L8c040a9c: *stop=0x8c040a9cu;r[2]=0u;goto L8c040a9e; /* mov #0x0,r2 */
L8c040a9e: *stop=0x8c040a9eu;*sr=(*sr&~1u)|(r[3]>r[2]);goto L8c040aa0; /* cmp/hi r2,r3 */
L8c040aa0: if((*sr&1u))goto L8c040a7c;goto L8c040aa2; /* bt 0x8c040a7c */
L8c040aa2: *stop=0x8c040aa2u;if(!valid_ram(r[6],4u))return -3;wr(m,r[6],r[14],4);goto L8c040aa4; /* mov.l r14,@r6 */
L8c040aa4: *stop=0x8c040aa4u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c040aa6; /* mov.l @r15+,r12 */
L8c040aa6: *stop=0x8c040aa6u;if(*pr!=entry_pr)return -3;*stop=0x8c040aa8u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_sampling_8c03ff2c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c03ff2c;
L8c03ff2c: *stop=0x8c03ff2cu;r[0]=r[5];goto L8c03ff2e; /* mov r5,r0 */
L8c03ff2e: *stop=0x8c03ff2eu;goto L8c03ff30; /* nop */
L8c03ff30: *stop=0x8c03ff30u;r[0]<<=2;goto L8c03ff32; /* shll2 r0 */
L8c03ff32: *stop=0x8c03ff32u;r[0]<<=2;goto L8c03ff34; /* shll2 r0 */
L8c03ff34: *stop=0x8c03ff34u;r[5]+=1u;goto L8c03ff36; /* add #0x1,r5 */
L8c03ff36: *stop=0x8c03ff36u;*sr=(*sr&~1u)|(r[5]>=r[6]);goto L8c03ff38; /* cmp/hs r6,r5 */
L8c03ff38: take=(*sr&1u);*stop=0x8c03ff3au;r[0]+=r[4];if(take)goto L8c03ff40;goto L8c03ff3c; /* bt/s 0x8c03ff40 */
L8c03ff3c: *stop=0x8c03ff3cu;r[4]=r[0];goto L8c03ff3e; /* mov r0,r4 */
L8c03ff3e: *stop=0x8c03ff3eu;r[4]+=16u;goto L8c03ff40; /* add #0x10,r4 */
L8c03ff40: *stop=0x8c03ff40u;if(!valid_ram(r[0],4u))return -3;r[3]=rd(m,r[0],4);goto L8c03ff42; /* mov.l @r0,r3 */
L8c03ff42: *stop=0x8c03ff42u;*fpul=r[3];goto L8c03ff44; /* lds r3,FPUL */
L8c03ff44: *stop=0x8c03ff44u;r[2]=rd(m,0x8c04007cu,4);goto L8c03ff46; /* mov.l 0x8c04007c,r2 */
L8c03ff46: *stop=0x8c03ff46u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c03ff48; /* float FPUL,fr3 */
L8c03ff48: *stop=0x8c03ff48u;if(!valid_ram(r[4],4u))return -3;r[5]=rd(m,r[4],4);goto L8c03ff4a; /* mov.l @r4,r5 */
L8c03ff4a: *stop=0x8c03ff4au;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[2]);goto L8c03ff4c; /* fmov.s @r2,fr4 */
L8c03ff4c: *stop=0x8c03ff4cu;r[5]-=r[3];goto L8c03ff4e; /* sub r3,r5 */
L8c03ff4e: *stop=0x8c03ff4eu;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c03ff50; /* cmp/pl r5 */
L8c03ff50: take=!(*sr&1u);*stop=0x8c03ff52u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],1);if(status)return status;}if(take)goto L8c03ff5e;goto L8c03ff54; /* bf/s 0x8c03ff5e */
L8c03ff54: *stop=0x8c03ff54u;*fpul=r[5];goto L8c03ff56; /* lds r5,FPUL */
L8c03ff56: *stop=0x8c03ff56u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][5]);if(status)return status;goto L8c03ff58; /* float FPUL,fr5 */
L8c03ff58: *stop=0x8c03ff58u;status=sa_sh4_vector_numeric(0xf57du,f,*fpul);if(status)return status;goto L8c03ff5a; /* fsrra fr5 */
L8c03ff5a: *stop=0x8c03ff5au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[5],&f->fpscr,&fr[5]);if(status)return status;}goto L8c03ff5c; /* fmul fr5,fr5 */
L8c03ff5c: *stop=0x8c03ff5cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[5],&f->fpscr,&fr[4]);if(status)return status;}goto L8c03ff5e; /* fmul fr5,fr4 */
L8c03ff5e: *stop=0x8c03ff5eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c03ff60; /* fldi1 fr5 */
L8c03ff60: *stop=0x8c03ff60u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[4],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c03ff62; /* fsub fr4,fr5 */
L8c03ff62: *stop=0x8c03ff62u;r[5]=r[0];goto L8c03ff64; /* mov r0,r5 */
L8c03ff64: *stop=0x8c03ff64u;r[5]+=4u;goto L8c03ff66; /* add #0x4,r5 */
L8c03ff66: *stop=0x8c03ff66u;r[4]+=4u;goto L8c03ff68; /* add #0x4,r4 */
L8c03ff68: *stop=0x8c03ff68u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03ff6a; /* fmov.s @r5+,fr8 */
L8c03ff6a: *stop=0x8c03ff6au;r[7]+=12u;goto L8c03ff6c; /* add #0xc,r7 */
L8c03ff6c: *stop=0x8c03ff6cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03ff6e; /* fmov.s @r4+,fr6 */
L8c03ff6e: *stop=0x8c03ff6eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c03ff70; /* fmul fr5,fr8 */
L8c03ff70: *stop=0x8c03ff70u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03ff72; /* fmov.s @r4+,fr7 */
L8c03ff72: *stop=0x8c03ff72u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[4],&f->fpscr,&fr[6]);if(status)return status;}goto L8c03ff74; /* fmul fr4,fr6 */
L8c03ff74: *stop=0x8c03ff74u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[6],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c03ff76; /* fadd fr6,fr8 */
L8c03ff76: *stop=0x8c03ff76u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03ff78; /* fmov.s @r5+,fr6 */
L8c03ff78: *stop=0x8c03ff78u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[5],&f->fpscr,&fr[6]);if(status)return status;}goto L8c03ff7a; /* fmul fr5,fr6 */
L8c03ff7a: *stop=0x8c03ff7au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[4],&f->fpscr,&fr[7]);if(status)return status;}goto L8c03ff7c; /* fmul fr4,fr7 */
L8c03ff7c: *stop=0x8c03ff7cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[7],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c03ff7e; /* fadd fr7,fr6 */
L8c03ff7e: *stop=0x8c03ff7eu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[5]);goto L8c03ff80; /* fmov.s @r5,fr7 */
L8c03ff80: *stop=0x8c03ff80u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[5],&f->fpscr,&fr[7]);if(status)return status;}goto L8c03ff82; /* fmul fr5,fr7 */
L8c03ff82: *stop=0x8c03ff82u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);goto L8c03ff84; /* fmov.s @r4,fr5 */
L8c03ff84: *stop=0x8c03ff84u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[4],&f->fpscr,&fr[5]);if(status)return status;}goto L8c03ff86; /* fmul fr4,fr5 */
L8c03ff86: *stop=0x8c03ff86u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[5],&f->fpscr,&fr[7],0);if(status)return status;}goto L8c03ff88; /* fadd fr5,fr7 */
L8c03ff88: *stop=0x8c03ff88u;if(!valid_ram(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[7]);goto L8c03ff8a; /* fmov.s fr7,@-r7 */
L8c03ff8a: *stop=0x8c03ff8au;if(!valid_ram(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[7]);goto L8c03ff8c; /* fmov.s fr6,@-r7 */
L8c03ff8c: *stop=0x8c03ff8cu;if(*pr!=entry_pr)return -3;*stop=0x8c03ff8eu;if(!valid_ram(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[7]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_sampling_8c03ff90(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c03ff90;
L8c03ff90: *stop=0x8c03ff90u;r[0]=r[5];goto L8c03ff92; /* mov r5,r0 */
L8c03ff92: *stop=0x8c03ff92u;goto L8c03ff94; /* nop */
L8c03ff94: *stop=0x8c03ff94u;r[0]<<=2;goto L8c03ff96; /* shll2 r0 */
L8c03ff96: *stop=0x8c03ff96u;r[0]<<=2;goto L8c03ff98; /* shll2 r0 */
L8c03ff98: *stop=0x8c03ff98u;r[5]+=1u;goto L8c03ff9a; /* add #0x1,r5 */
L8c03ff9a: *stop=0x8c03ff9au;*sr=(*sr&~1u)|(r[5]>=r[6]);goto L8c03ff9c; /* cmp/hs r6,r5 */
L8c03ff9c: take=(*sr&1u);*stop=0x8c03ff9eu;r[0]+=r[4];if(take)goto L8c03ffa4;goto L8c03ffa0; /* bt/s 0x8c03ffa4 */
L8c03ffa0: *stop=0x8c03ffa0u;r[4]=r[0];goto L8c03ffa2; /* mov r0,r4 */
L8c03ffa2: *stop=0x8c03ffa2u;r[4]+=16u;goto L8c03ffa4; /* add #0x10,r4 */
L8c03ffa4: *stop=0x8c03ffa4u;if(!valid_ram(r[0],4u))return -3;r[3]=rd(m,r[0],4);goto L8c03ffa6; /* mov.l @r0,r3 */
L8c03ffa6: *stop=0x8c03ffa6u;*fpul=r[3];goto L8c03ffa8; /* lds r3,FPUL */
L8c03ffa8: *stop=0x8c03ffa8u;r[2]=rd(m,0x8c04007cu,4);goto L8c03ffaa; /* mov.l 0x8c04007c,r2 */
L8c03ffaa: *stop=0x8c03ffaau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c03ffac; /* float FPUL,fr3 */
L8c03ffac: *stop=0x8c03ffacu;if(!valid_ram(r[4],4u))return -3;r[5]=rd(m,r[4],4);goto L8c03ffae; /* mov.l @r4,r5 */
L8c03ffae: *stop=0x8c03ffaeu;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[2]);goto L8c03ffb0; /* fmov.s @r2,fr4 */
L8c03ffb0: *stop=0x8c03ffb0u;r[5]-=r[3];goto L8c03ffb2; /* sub r3,r5 */
L8c03ffb2: *stop=0x8c03ffb2u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c03ffb4; /* cmp/pl r5 */
L8c03ffb4: take=!(*sr&1u);*stop=0x8c03ffb6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],1);if(status)return status;}if(take)goto L8c03ffc2;goto L8c03ffb8; /* bf/s 0x8c03ffc2 */
L8c03ffb8: *stop=0x8c03ffb8u;*fpul=r[5];goto L8c03ffba; /* lds r5,FPUL */
L8c03ffba: *stop=0x8c03ffbau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][5]);if(status)return status;goto L8c03ffbc; /* float FPUL,fr5 */
L8c03ffbc: *stop=0x8c03ffbcu;status=sa_sh4_vector_numeric(0xf57du,f,*fpul);if(status)return status;goto L8c03ffbe; /* fsrra fr5 */
L8c03ffbe: *stop=0x8c03ffbeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[5],&f->fpscr,&fr[5]);if(status)return status;}goto L8c03ffc0; /* fmul fr5,fr5 */
L8c03ffc0: *stop=0x8c03ffc0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[5],&f->fpscr,&fr[4]);if(status)return status;}goto L8c03ffc2; /* fmul fr5,fr4 */
L8c03ffc2: *stop=0x8c03ffc2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c03ffc4; /* fldi1 fr5 */
L8c03ffc4: *stop=0x8c03ffc4u;if(!valid_ram(r[0]+4u,4u))return -3;r[3]=rd(m,r[0]+0x4u,4);goto L8c03ffc6; /* mov.l @(0x4,r0),r3 */
L8c03ffc6: *stop=0x8c03ffc6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[4],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c03ffc8; /* fsub fr4,fr5 */
L8c03ffc8: *stop=0x8c03ffc8u;*fpul=r[3];goto L8c03ffca; /* lds r3,FPUL */
L8c03ffca: *stop=0x8c03ffcau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c03ffcc; /* float FPUL,fr3 */
L8c03ffcc: *stop=0x8c03ffccu;if(!valid_ram(r[4]+4u,4u))return -3;r[3]=rd(m,r[4]+0x4u,4);goto L8c03ffce; /* mov.l @(0x4,r4),r3 */
L8c03ffce: *stop=0x8c03ffceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c03ffd0; /* fmul fr5,fr3 */
L8c03ffd0: *stop=0x8c03ffd0u;*fpul=r[3];goto L8c03ffd2; /* lds r3,FPUL */
L8c03ffd2: *stop=0x8c03ffd2u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c03ffd4; /* float FPUL,fr2 */
L8c03ffd4: *stop=0x8c03ffd4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c03ffd6; /* fmul fr4,fr2 */
L8c03ffd6: *stop=0x8c03ffd6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c03ffd8; /* fadd fr2,fr3 */
L8c03ffd8: *stop=0x8c03ffd8u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c03ffda; /* ftrc fr3,FPUL */
L8c03ffda: *stop=0x8c03ffdau;r[3]=*fpul;goto L8c03ffdc; /* sts FPUL,r3 */
L8c03ffdc: *stop=0x8c03ffdcu;if(!valid_ram(r[7],4u))return -3;wr(m,r[7],r[3],4);goto L8c03ffde; /* mov.l r3,@r7 */
L8c03ffde: *stop=0x8c03ffdeu;if(!valid_ram(r[0]+8u,4u))return -3;r[3]=rd(m,r[0]+0x8u,4);goto L8c03ffe0; /* mov.l @(0x8,r0),r3 */
L8c03ffe0: *stop=0x8c03ffe0u;*fpul=r[3];goto L8c03ffe2; /* lds r3,FPUL */
L8c03ffe2: *stop=0x8c03ffe2u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c03ffe4; /* float FPUL,fr3 */
L8c03ffe4: *stop=0x8c03ffe4u;if(!valid_ram(r[4]+8u,4u))return -3;r[3]=rd(m,r[4]+0x8u,4);goto L8c03ffe6; /* mov.l @(0x8,r4),r3 */
L8c03ffe6: *stop=0x8c03ffe6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c03ffe8; /* fmul fr5,fr3 */
L8c03ffe8: *stop=0x8c03ffe8u;*fpul=r[3];goto L8c03ffea; /* lds r3,FPUL */
L8c03ffea: *stop=0x8c03ffeau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c03ffec; /* float FPUL,fr2 */
L8c03ffec: *stop=0x8c03ffecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c03ffee; /* fmul fr4,fr2 */
L8c03ffee: *stop=0x8c03ffeeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c03fff0; /* fadd fr2,fr3 */
L8c03fff0: *stop=0x8c03fff0u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c03fff2; /* ftrc fr3,FPUL */
L8c03fff2: *stop=0x8c03fff2u;r[3]=*fpul;goto L8c03fff4; /* sts FPUL,r3 */
L8c03fff4: *stop=0x8c03fff4u;if(!valid_ram(r[7]+4u,4u))return -3;wr(m,r[7]+0x4u,r[3],4);goto L8c03fff6; /* mov.l r3,@(0x4,r7) */
L8c03fff6: *stop=0x8c03fff6u;if(!valid_ram(r[0]+12u,4u))return -3;r[3]=rd(m,r[0]+0xcu,4);goto L8c03fff8; /* mov.l @(0xc,r0),r3 */
L8c03fff8: *stop=0x8c03fff8u;*fpul=r[3];goto L8c03fffa; /* lds r3,FPUL */
L8c03fffa: *stop=0x8c03fffau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c03fffc; /* float FPUL,fr3 */
L8c03fffc: *stop=0x8c03fffcu;if(!valid_ram(r[4]+12u,4u))return -3;r[3]=rd(m,r[4]+0xcu,4);goto L8c03fffe; /* mov.l @(0xc,r4),r3 */
L8c03fffe: *stop=0x8c03fffeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c040000; /* fmul fr5,fr3 */
L8c040000: *stop=0x8c040000u;*fpul=r[3];goto L8c040002; /* lds r3,FPUL */
L8c040002: *stop=0x8c040002u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c040004; /* float FPUL,fr2 */
L8c040004: *stop=0x8c040004u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c040006; /* fmul fr4,fr2 */
L8c040006: *stop=0x8c040006u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c040008; /* fadd fr2,fr3 */
L8c040008: *stop=0x8c040008u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c04000a; /* ftrc fr3,FPUL */
L8c04000a: *stop=0x8c04000au;r[3]=*fpul;goto L8c04000c; /* sts FPUL,r3 */
L8c04000c: *stop=0x8c04000cu;if(*pr!=entry_pr)return -3;*stop=0x8c04000eu;if(!valid_ram(r[7]+8u,4u))return -3;wr(m,r[7]+0x8u,r[3],4);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_sampling_8c040c5c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c040c5c;
L8c040c5c: *stop=0x8c040c5cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c040c5e; /* mov.l r14,@-r15 */
L8c040c5e: *stop=0x8c040c5eu;r[14]=r[5];goto L8c040c60; /* mov r5,r14 */
L8c040c60: *stop=0x8c040c60u;r[14]<<=2;goto L8c040c62; /* shll2 r14 */
L8c040c62: *stop=0x8c040c62u;r[14]<<=2;goto L8c040c64; /* shll2 r14 */
L8c040c64: *stop=0x8c040c64u;r[5]+=1u;goto L8c040c66; /* add #0x1,r5 */
L8c040c66: *stop=0x8c040c66u;*sr=(*sr&~1u)|(r[5]>=r[6]);goto L8c040c68; /* cmp/hs r6,r5 */
L8c040c68: take=(*sr&1u);*stop=0x8c040c6au;r[14]+=r[4];if(take)goto L8c040c70;goto L8c040c6c; /* bt/s 0x8c040c70 */
L8c040c6c: *stop=0x8c040c6cu;r[4]=r[14];goto L8c040c6e; /* mov r14,r4 */
L8c040c6e: *stop=0x8c040c6eu;r[4]+=16u;goto L8c040c70; /* add #0x10,r4 */
L8c040c70: *stop=0x8c040c70u;r[1]=rd(m,0x8c040ce4u,4);goto L8c040c72; /* mov.l 0x8c040ce4,r1 */
L8c040c72: *stop=0x8c040c72u;r[3]=28u;goto L8c040c74; /* mov #0x1c,r3 */
L8c040c74: *stop=0x8c040c74u;r[0]=rd(m,0x8c040ce8u,4);goto L8c040c76; /* mov.l 0x8c040ce8,r0 */
L8c040c76: *stop=0x8c040c76u;if(!valid_ram(r[1],1u))return -3;r[2]=sx(m,r[1],1);goto L8c040c78; /* mov.b @r1,r2 */
L8c040c78: *stop=0x8c040c78u;if(!valid_ram(r[4],4u))return -3;r[5]=rd(m,r[4],4);goto L8c040c7a; /* mov.l @r4,r5 */
L8c040c7a: *stop=0x8c040c7au;*macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[2]);goto L8c040c7c; /* muls.w r3,r2 */
L8c040c7c: *stop=0x8c040c7cu;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c040c7e; /* mov.l @r14,r2 */
L8c040c7e: *stop=0x8c040c7eu;r[3]=*macl;goto L8c040c80; /* sts MACL,r3 */
L8c040c80: *stop=0x8c040c80u;r[3]=(uint32_t)(int32_t)(int8_t)r[3];goto L8c040c82; /* exts.b r3,r3 */
L8c040c82: *stop=0x8c040c82u;*fpul=r[2];goto L8c040c84; /* lds r2,FPUL */
L8c040c84: *stop=0x8c040c84u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[3]);goto L8c040c86; /* fmov.s @(r0,r3),fr4 */
L8c040c86: *stop=0x8c040c86u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c040c88; /* float FPUL,fr3 */
L8c040c88: *stop=0x8c040c88u;r[3]=r[2];goto L8c040c8a; /* mov r2,r3 */
L8c040c8a: *stop=0x8c040c8au;r[5]-=r[3];goto L8c040c8c; /* sub r3,r5 */
L8c040c8c: *stop=0x8c040c8cu;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c040c8e; /* cmp/pl r5 */
L8c040c8e: take=!(*sr&1u);*stop=0x8c040c90u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],1);if(status)return status;}if(take)goto L8c040c9c;goto L8c040c92; /* bf/s 0x8c040c9c */
L8c040c92: *stop=0x8c040c92u;*fpul=r[5];goto L8c040c94; /* lds r5,FPUL */
L8c040c94: *stop=0x8c040c94u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][5]);if(status)return status;goto L8c040c96; /* float FPUL,fr5 */
L8c040c96: *stop=0x8c040c96u;status=sa_sh4_vector_numeric(0xf57du,f,*fpul);if(status)return status;goto L8c040c98; /* fsrra fr5 */
L8c040c98: *stop=0x8c040c98u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[5],&f->fpscr,&fr[5]);if(status)return status;}goto L8c040c9a; /* fmul fr5,fr5 */
L8c040c9a: *stop=0x8c040c9au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[5],&f->fpscr,&fr[4]);if(status)return status;}goto L8c040c9c; /* fmul fr5,fr4 */
L8c040c9c: *stop=0x8c040c9cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c040c9e; /* fldi1 fr5 */
L8c040c9e: *stop=0x8c040c9eu;r[5]=r[14];goto L8c040ca0; /* mov r14,r5 */
L8c040ca0: *stop=0x8c040ca0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[4],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c040ca2; /* fsub fr4,fr5 */
L8c040ca2: *stop=0x8c040ca2u;r[5]+=4u;goto L8c040ca4; /* add #0x4,r5 */
L8c040ca4: *stop=0x8c040ca4u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c040ca6; /* fmov.s @r5+,fr8 */
L8c040ca6: *stop=0x8c040ca6u;r[7]+=12u;goto L8c040ca8; /* add #0xc,r7 */
L8c040ca8: *stop=0x8c040ca8u;r[4]+=4u;goto L8c040caa; /* add #0x4,r4 */
L8c040caa: *stop=0x8c040caau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c040cac; /* fmul fr5,fr8 */
L8c040cac: *stop=0x8c040cacu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c040cae; /* fmov.s @r4+,fr6 */
L8c040cae: *stop=0x8c040caeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[4],&f->fpscr,&fr[6]);if(status)return status;}goto L8c040cb0; /* fmul fr4,fr6 */
L8c040cb0: *stop=0x8c040cb0u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c040cb2; /* fmov.s @r4+,fr7 */
L8c040cb2: *stop=0x8c040cb2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[6],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c040cb4; /* fadd fr6,fr8 */
L8c040cb4: *stop=0x8c040cb4u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c040cb6; /* fmov.s @r5+,fr6 */
L8c040cb6: *stop=0x8c040cb6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[5],&f->fpscr,&fr[6]);if(status)return status;}goto L8c040cb8; /* fmul fr5,fr6 */
L8c040cb8: *stop=0x8c040cb8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[4],&f->fpscr,&fr[7]);if(status)return status;}goto L8c040cba; /* fmul fr4,fr7 */
L8c040cba: *stop=0x8c040cbau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[7],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c040cbc; /* fadd fr7,fr6 */
L8c040cbc: *stop=0x8c040cbcu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[5]);goto L8c040cbe; /* fmov.s @r5,fr7 */
L8c040cbe: *stop=0x8c040cbeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[5],&f->fpscr,&fr[7]);if(status)return status;}goto L8c040cc0; /* fmul fr5,fr7 */
L8c040cc0: *stop=0x8c040cc0u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);goto L8c040cc2; /* fmov.s @r4,fr5 */
L8c040cc2: *stop=0x8c040cc2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[4],&f->fpscr,&fr[5]);if(status)return status;}goto L8c040cc4; /* fmul fr4,fr5 */
L8c040cc4: *stop=0x8c040cc4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[5],&f->fpscr,&fr[7],0);if(status)return status;}goto L8c040cc6; /* fadd fr5,fr7 */
L8c040cc6: *stop=0x8c040cc6u;if(!valid_ram(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,7u,r[7]);goto L8c040cc8; /* fmov.s fr7,@-r7 */
L8c040cc8: *stop=0x8c040cc8u;if(!valid_ram(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,6u,r[7]);goto L8c040cca; /* fmov.s fr6,@-r7 */
L8c040cca: *stop=0x8c040ccau;if(!valid_ram(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,8u,r[7]);goto L8c040ccc; /* fmov.s fr8,@-r7 */
L8c040ccc: *stop=0x8c040cccu;if(*pr!=entry_pr)return -3;*stop=0x8c040cceu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_sampling_8c040d60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c040d60;
L8c040d60: *stop=0x8c040d60u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c040d62; /* mov.l r14,@-r15 */
L8c040d62: *stop=0x8c040d62u;r[14]=r[5];goto L8c040d64; /* mov r5,r14 */
L8c040d64: *stop=0x8c040d64u;r[14]<<=2;goto L8c040d66; /* shll2 r14 */
L8c040d66: *stop=0x8c040d66u;r[14]<<=2;goto L8c040d68; /* shll2 r14 */
L8c040d68: *stop=0x8c040d68u;r[5]+=1u;goto L8c040d6a; /* add #0x1,r5 */
L8c040d6a: *stop=0x8c040d6au;*sr=(*sr&~1u)|(r[5]>=r[6]);goto L8c040d6c; /* cmp/hs r6,r5 */
L8c040d6c: take=(*sr&1u);*stop=0x8c040d6eu;r[14]+=r[4];if(take)goto L8c040d74;goto L8c040d70; /* bt/s 0x8c040d74 */
L8c040d70: *stop=0x8c040d70u;r[4]=r[14];goto L8c040d72; /* mov r14,r4 */
L8c040d72: *stop=0x8c040d72u;r[4]+=16u;goto L8c040d74; /* add #0x10,r4 */
L8c040d74: *stop=0x8c040d74u;r[1]=rd(m,0x8c041004u,4);goto L8c040d76; /* mov.l 0x8c041004,r1 */
L8c040d76: *stop=0x8c040d76u;r[3]=28u;goto L8c040d78; /* mov #0x1c,r3 */
L8c040d78: *stop=0x8c040d78u;r[0]=rd(m,0x8c041008u,4);goto L8c040d7a; /* mov.l 0x8c041008,r0 */
L8c040d7a: *stop=0x8c040d7au;if(!valid_ram(r[1],1u))return -3;r[2]=sx(m,r[1],1);goto L8c040d7c; /* mov.b @r1,r2 */
L8c040d7c: *stop=0x8c040d7cu;if(!valid_ram(r[4],4u))return -3;r[5]=rd(m,r[4],4);goto L8c040d7e; /* mov.l @r4,r5 */
L8c040d7e: *stop=0x8c040d7eu;*macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[2]);goto L8c040d80; /* muls.w r3,r2 */
L8c040d80: *stop=0x8c040d80u;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c040d82; /* mov.l @r14,r2 */
L8c040d82: *stop=0x8c040d82u;r[3]=*macl;goto L8c040d84; /* sts MACL,r3 */
L8c040d84: *stop=0x8c040d84u;r[3]=(uint32_t)(int32_t)(int8_t)r[3];goto L8c040d86; /* exts.b r3,r3 */
L8c040d86: *stop=0x8c040d86u;*fpul=r[2];goto L8c040d88; /* lds r2,FPUL */
L8c040d88: *stop=0x8c040d88u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[3]);goto L8c040d8a; /* fmov.s @(r0,r3),fr4 */
L8c040d8a: *stop=0x8c040d8au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c040d8c; /* float FPUL,fr3 */
L8c040d8c: *stop=0x8c040d8cu;r[3]=r[2];goto L8c040d8e; /* mov r2,r3 */
L8c040d8e: *stop=0x8c040d8eu;r[5]-=r[3];goto L8c040d90; /* sub r3,r5 */
L8c040d90: *stop=0x8c040d90u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c040d92; /* cmp/pl r5 */
L8c040d92: take=!(*sr&1u);*stop=0x8c040d94u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],1);if(status)return status;}if(take)goto L8c040da0;goto L8c040d96; /* bf/s 0x8c040da0 */
L8c040d96: *stop=0x8c040d96u;*fpul=r[5];goto L8c040d98; /* lds r5,FPUL */
L8c040d98: *stop=0x8c040d98u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][5]);if(status)return status;goto L8c040d9a; /* float FPUL,fr5 */
L8c040d9a: *stop=0x8c040d9au;status=sa_sh4_vector_numeric(0xf57du,f,*fpul);if(status)return status;goto L8c040d9c; /* fsrra fr5 */
L8c040d9c: *stop=0x8c040d9cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[5],&f->fpscr,&fr[5]);if(status)return status;}goto L8c040d9e; /* fmul fr5,fr5 */
L8c040d9e: *stop=0x8c040d9eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[5],&f->fpscr,&fr[4]);if(status)return status;}goto L8c040da0; /* fmul fr5,fr4 */
L8c040da0: *stop=0x8c040da0u;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c040da2; /* mov.l @(0x4,r14),r3 */
L8c040da2: *stop=0x8c040da2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c040da4; /* fldi1 fr5 */
L8c040da4: *stop=0x8c040da4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[4],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c040da6; /* fsub fr4,fr5 */
L8c040da6: *stop=0x8c040da6u;*fpul=r[3];goto L8c040da8; /* lds r3,FPUL */
L8c040da8: *stop=0x8c040da8u;if(!valid_ram(r[4]+4u,4u))return -3;r[3]=rd(m,r[4]+0x4u,4);goto L8c040daa; /* mov.l @(0x4,r4),r3 */
L8c040daa: *stop=0x8c040daau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c040dac; /* float FPUL,fr3 */
L8c040dac: *stop=0x8c040dacu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c040dae; /* fmul fr5,fr3 */
L8c040dae: *stop=0x8c040daeu;*fpul=r[3];goto L8c040db0; /* lds r3,FPUL */
L8c040db0: *stop=0x8c040db0u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c040db2; /* float FPUL,fr2 */
L8c040db2: *stop=0x8c040db2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c040db4; /* fmul fr4,fr2 */
L8c040db4: *stop=0x8c040db4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c040db6; /* fadd fr2,fr3 */
L8c040db6: *stop=0x8c040db6u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c040db8; /* ftrc fr3,FPUL */
L8c040db8: *stop=0x8c040db8u;r[3]=*fpul;goto L8c040dba; /* sts FPUL,r3 */
L8c040dba: *stop=0x8c040dbau;if(!valid_ram(r[7],4u))return -3;wr(m,r[7],r[3],4);goto L8c040dbc; /* mov.l r3,@r7 */
L8c040dbc: *stop=0x8c040dbcu;if(!valid_ram(r[14]+8u,4u))return -3;r[3]=rd(m,r[14]+0x8u,4);goto L8c040dbe; /* mov.l @(0x8,r14),r3 */
L8c040dbe: *stop=0x8c040dbeu;*fpul=r[3];goto L8c040dc0; /* lds r3,FPUL */
L8c040dc0: *stop=0x8c040dc0u;if(!valid_ram(r[4]+8u,4u))return -3;r[3]=rd(m,r[4]+0x8u,4);goto L8c040dc2; /* mov.l @(0x8,r4),r3 */
L8c040dc2: *stop=0x8c040dc2u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c040dc4; /* float FPUL,fr3 */
L8c040dc4: *stop=0x8c040dc4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c040dc6; /* fmul fr5,fr3 */
L8c040dc6: *stop=0x8c040dc6u;*fpul=r[3];goto L8c040dc8; /* lds r3,FPUL */
L8c040dc8: *stop=0x8c040dc8u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c040dca; /* float FPUL,fr2 */
L8c040dca: *stop=0x8c040dcau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c040dcc; /* fmul fr4,fr2 */
L8c040dcc: *stop=0x8c040dccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c040dce; /* fadd fr2,fr3 */
L8c040dce: *stop=0x8c040dceu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c040dd0; /* ftrc fr3,FPUL */
L8c040dd0: *stop=0x8c040dd0u;r[3]=*fpul;goto L8c040dd2; /* sts FPUL,r3 */
L8c040dd2: *stop=0x8c040dd2u;if(!valid_ram(r[7]+4u,4u))return -3;wr(m,r[7]+0x4u,r[3],4);goto L8c040dd4; /* mov.l r3,@(0x4,r7) */
L8c040dd4: *stop=0x8c040dd4u;if(!valid_ram(r[14]+12u,4u))return -3;r[3]=rd(m,r[14]+0xcu,4);goto L8c040dd6; /* mov.l @(0xc,r14),r3 */
L8c040dd6: *stop=0x8c040dd6u;*fpul=r[3];goto L8c040dd8; /* lds r3,FPUL */
L8c040dd8: *stop=0x8c040dd8u;if(!valid_ram(r[4]+12u,4u))return -3;r[3]=rd(m,r[4]+0xcu,4);goto L8c040dda; /* mov.l @(0xc,r4),r3 */
L8c040dda: *stop=0x8c040ddau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c040ddc; /* float FPUL,fr3 */
L8c040ddc: *stop=0x8c040ddcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c040dde; /* fmul fr5,fr3 */
L8c040dde: *stop=0x8c040ddeu;*fpul=r[3];goto L8c040de0; /* lds r3,FPUL */
L8c040de0: *stop=0x8c040de0u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c040de2; /* float FPUL,fr2 */
L8c040de2: *stop=0x8c040de2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c040de4; /* fmul fr4,fr2 */
L8c040de4: *stop=0x8c040de4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c040de6; /* fadd fr2,fr3 */
L8c040de6: *stop=0x8c040de6u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c040de8; /* ftrc fr3,FPUL */
L8c040de8: *stop=0x8c040de8u;r[3]=*fpul;goto L8c040dea; /* sts FPUL,r3 */
L8c040dea: *stop=0x8c040deau;if(!valid_ram(r[7]+8u,4u))return -3;wr(m,r[7]+0x8u,r[3],4);goto L8c040dec; /* mov.l r3,@(0x8,r7) */
L8c040dec: *stop=0x8c040decu;if(*pr!=entry_pr)return -3;*stop=0x8c040deeu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_sampling_8c60b596(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60b596;
L8c60b596: *stop=0x8c60b596u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60b598; /* mov.l r14,@-r15 */
L8c60b598: *stop=0x8c60b598u;r[3]=28u;goto L8c60b59a; /* mov #0x1c,r3 */
L8c60b59a: *stop=0x8c60b59au;r[14]=rd(m,0x8c60b7fcu,4);goto L8c60b59c; /* mov.l 0x8c60b7fc,r14 */
L8c60b59c: *stop=0x8c60b59cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60b59e; /* mov.l r13,@-r15 */
L8c60b59e: *stop=0x8c60b59eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c60b5a0; /* mov.l r12,@-r15 */
L8c60b5a0: *stop=0x8c60b5a0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c60b5a2; /* mov.l r11,@-r15 */
L8c60b5a2: *stop=0x8c60b5a2u;r[11]=r[4];goto L8c60b5a4; /* mov r4,r11 */
L8c60b5a4: *stop=0x8c60b5a4u;r[12]=rd(m,0x8c60b7f8u,4);goto L8c60b5a6; /* mov.l 0x8c60b7f8,r12 */
L8c60b5a6: *stop=0x8c60b5a6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c60b5a8; /* sts.l PR,@-r15 */
L8c60b5a8: *stop=0x8c60b5a8u;if(!valid_ram(r[14],2u))return -3;r[2]=sx(m,r[14],2);goto L8c60b5aa; /* mov.w @r14,r2 */
L8c60b5aa: *stop=0x8c60b5aau;*macl=(r[3]&65535u)*(r[2]&65535u);goto L8c60b5ac; /* mulu.w r3,r2 */
L8c60b5ac: *stop=0x8c60b5acu;r[4]=*macl;goto L8c60b5ae; /* sts MACL,r4 */
L8c60b5ae: *stop=0x8c60b5aeu;r[4]+=r[12];goto L8c60b5b0; /* add r12,r4 */
L8c60b5b0: *stop=0x8c60b5b0u;if(!valid_ram(r[4],4u))return -3;r[13]=rd(m,r[4],4);goto L8c60b5b2; /* mov.l @r4,r13 */
L8c60b5b2: *stop=0x8c60b5b2u;r[2]=r[13];goto L8c60b5b4; /* mov r13,r2 */
L8c60b5b4: *stop=0x8c60b5b4u;r[2]+=8u;goto L8c60b5b6; /* add #0x8,r2 */
L8c60b5b6: *stop=0x8c60b5b6u;if(!valid_ram(r[4]+4u,4u))return -3;wr(m,r[4]+0x4u,r[13],4);goto L8c60b5b8; /* mov.l r13,@(0x4,r4) */
L8c60b5b8: *stop=0x8c60b5b8u;if(!valid_ram(r[14],2u))return -3;r[1]=sx(m,r[14],2);goto L8c60b5ba; /* mov.w @r14,r1 */
L8c60b5ba: *stop=0x8c60b5bau;*macl=(r[3]&65535u)*(r[1]&65535u);goto L8c60b5bc; /* mulu.w r3,r1 */
L8c60b5bc: *stop=0x8c60b5bcu;r[3]=*macl;goto L8c60b5be; /* sts MACL,r3 */
L8c60b5be: *stop=0x8c60b5beu;r[3]+=r[12];goto L8c60b5c0; /* add r12,r3 */
L8c60b5c0: *stop=0x8c60b5c0u;if(!valid_ram(r[3]+8u,4u))return -3;wr(m,r[3]+0x8u,r[2],4);goto L8c60b5c2; /* mov.l r2,@(0x8,r3) */
L8c60b5c2: *stop=0x8c60b5c2u;r[3]=28u;goto L8c60b5c4; /* mov #0x1c,r3 */
L8c60b5c4: *stop=0x8c60b5c4u;if(!valid_ram(r[14],2u))return -3;r[2]=sx(m,r[14],2);goto L8c60b5c6; /* mov.w @r14,r2 */
L8c60b5c6: *stop=0x8c60b5c6u;*macl=(r[3]&65535u)*(r[2]&65535u);goto L8c60b5c8; /* mulu.w r3,r2 */
L8c60b5c8: *stop=0x8c60b5c8u;r[2]=0u;goto L8c60b5ca; /* mov #0x0,r2 */
L8c60b5ca: *stop=0x8c60b5cau;r[3]=*macl;goto L8c60b5cc; /* sts MACL,r3 */
L8c60b5cc: *stop=0x8c60b5ccu;r[3]+=r[12];goto L8c60b5ce; /* add r12,r3 */
L8c60b5ce: *stop=0x8c60b5ceu;if(!valid_ram(r[3]+20u,4u))return -3;wr(m,r[3]+0x14u,r[2],4);goto L8c60b5d0; /* mov.l r2,@(0x14,r3) */
L8c60b5d0: *stop=0x8c60b5d0u;if(!valid_ram(r[14],2u))return -3;r[3]=sx(m,r[14],2);goto L8c60b5d2; /* mov.w @r14,r3 */
L8c60b5d2: *stop=0x8c60b5d2u;r[3]=r[3]&65535u;goto L8c60b5d4; /* extu.w r3,r3 */
L8c60b5d4: *stop=0x8c60b5d4u;r[3]<<=2;goto L8c60b5d6; /* shll2 r3 */
L8c60b5d6: *stop=0x8c60b5d6u;r[0]=rd(m,0x8c60b800u,4);goto L8c60b5d8; /* mov.l 0x8c60b800,r0 */
L8c60b5d8: *stop=0x8c60b5d8u;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c60b5da; /* mov.l @(r0,r3),r2 */
L8c60b5da: target=r[2];*pr=0x8c60b5deu;*stop=0x8c60b5dcu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60b5de; /* jsr @r2 */
L8c60b5de: *stop=0x8c60b5deu;if(!valid_ram(r[14],2u))return -3;r[3]=sx(m,r[14],2);goto L8c60b5e0; /* mov.w @r14,r3 */
L8c60b5e0: *stop=0x8c60b5e0u;r[0]=rd(m,0x8c60b804u,4);goto L8c60b5e2; /* mov.l 0x8c60b804,r0 */
L8c60b5e2: *stop=0x8c60b5e2u;r[3]=r[3]&65535u;goto L8c60b5e4; /* extu.w r3,r3 */
L8c60b5e4: *stop=0x8c60b5e4u;r[3]<<=2;goto L8c60b5e6; /* shll2 r3 */
L8c60b5e6: *stop=0x8c60b5e6u;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c60b5e8; /* mov.l @(r0,r3),r2 */
L8c60b5e8: target=r[2];*pr=0x8c60b5ecu;*stop=0x8c60b5eau;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60b5ec; /* jsr @r2 */
L8c60b5ec: *stop=0x8c60b5ecu;if(!valid_ram(r[14],2u))return -3;r[3]=sx(m,r[14],2);goto L8c60b5ee; /* mov.w @r14,r3 */
L8c60b5ee: *stop=0x8c60b5eeu;r[0]=rd(m,0x8c60b808u,4);goto L8c60b5f0; /* mov.l 0x8c60b808,r0 */
L8c60b5f0: *stop=0x8c60b5f0u;r[3]=r[3]&65535u;goto L8c60b5f2; /* extu.w r3,r3 */
L8c60b5f2: *stop=0x8c60b5f2u;r[3]<<=2;goto L8c60b5f4; /* shll2 r3 */
L8c60b5f4: *stop=0x8c60b5f4u;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c60b5f6; /* mov.l @(r0,r3),r2 */
L8c60b5f6: target=r[2];*pr=0x8c60b5fau;*stop=0x8c60b5f8u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60b5fa; /* jsr @r2 */
L8c60b5fa: *stop=0x8c60b5fau;if(!valid_ram(r[14],2u))return -3;r[2]=sx(m,r[14],2);goto L8c60b5fc; /* mov.w @r14,r2 */
L8c60b5fc: *stop=0x8c60b5fcu;r[3]=28u;goto L8c60b5fe; /* mov #0x1c,r3 */
L8c60b5fe: *stop=0x8c60b5feu;r[13]+=16u;goto L8c60b600; /* add #0x10,r13 */
L8c60b600: *stop=0x8c60b600u;*macl=(r[3]&65535u)*(r[2]&65535u);goto L8c60b602; /* mulu.w r3,r2 */
L8c60b602: *stop=0x8c60b602u;r[3]=*macl;goto L8c60b604; /* sts MACL,r3 */
L8c60b604: *stop=0x8c60b604u;r[3]+=r[12];goto L8c60b606; /* add r12,r3 */
L8c60b606: *stop=0x8c60b606u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[13],4);goto L8c60b608; /* mov.l r13,@r3 */
L8c60b608: *stop=0x8c60b608u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c60b60a; /* lds.l @r15+,PR */
L8c60b60a: *stop=0x8c60b60au;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c60b60c; /* mov.l @r15+,r11 */
L8c60b60c: *stop=0x8c60b60cu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c60b60e; /* mov.l @r15+,r12 */
L8c60b60e: *stop=0x8c60b60eu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60b610; /* mov.l @r15+,r13 */
L8c60b610: *stop=0x8c60b610u;if(*pr!=entry_pr)return -3;*stop=0x8c60b612u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_sampling_8c60b4a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60b4a8;
L8c60b4a8: *stop=0x8c60b4a8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60b4aa; /* mov.l r14,@-r15 */
L8c60b4aa: *stop=0x8c60b4aau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60b4ac; /* mov.l r13,@-r15 */
L8c60b4ac: *stop=0x8c60b4acu;r[13]=rd(m,0x8c60b4fcu,4);goto L8c60b4ae; /* mov.l 0x8c60b4fc,r13 */
L8c60b4ae: *stop=0x8c60b4aeu;r[0]=rd(m,0x8c60b50cu,4);goto L8c60b4b0; /* mov.l 0x8c60b50c,r0 */
L8c60b4b0: *stop=0x8c60b4b0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c60b4b2; /* sts.l PR,@-r15 */
L8c60b4b2: *stop=0x8c60b4b2u;if(!valid_ram(r[13],2u))return -3;r[3]=sx(m,r[13],2);goto L8c60b4b4; /* mov.w @r13,r3 */
L8c60b4b4: *stop=0x8c60b4b4u;r[3]=r[3]&65535u;goto L8c60b4b6; /* extu.w r3,r3 */
L8c60b4b6: *stop=0x8c60b4b6u;r[3]<<=2;goto L8c60b4b8; /* shll2 r3 */
L8c60b4b8: *stop=0x8c60b4b8u;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c60b4ba; /* mov.l @(r0,r3),r2 */
L8c60b4ba: target=r[2];*pr=0x8c60b4beu;*stop=0x8c60b4bcu;r[14]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60b4be; /* jsr @r2 */
L8c60b4be: *stop=0x8c60b4beu;if(!valid_ram(r[13],2u))return -3;r[3]=sx(m,r[13],2);goto L8c60b4c0; /* mov.w @r13,r3 */
L8c60b4c0: *stop=0x8c60b4c0u;r[0]=rd(m,0x8c60b510u,4);goto L8c60b4c2; /* mov.l 0x8c60b510,r0 */
L8c60b4c2: *stop=0x8c60b4c2u;r[3]=r[3]&65535u;goto L8c60b4c4; /* extu.w r3,r3 */
L8c60b4c4: *stop=0x8c60b4c4u;r[3]<<=2;goto L8c60b4c6; /* shll2 r3 */
L8c60b4c6: *stop=0x8c60b4c6u;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c60b4c8; /* mov.l @(r0,r3),r2 */
L8c60b4c8: target=r[2];*pr=0x8c60b4ccu;*stop=0x8c60b4cau;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60b4cc; /* jsr @r2 */
L8c60b4cc: *stop=0x8c60b4ccu;if(!valid_ram(r[13],2u))return -3;r[3]=sx(m,r[13],2);goto L8c60b4ce; /* mov.w @r13,r3 */
L8c60b4ce: *stop=0x8c60b4ceu;r[4]=r[14];goto L8c60b4d0; /* mov r14,r4 */
L8c60b4d0: *stop=0x8c60b4d0u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c60b4d2; /* lds.l @r15+,PR */
L8c60b4d2: *stop=0x8c60b4d2u;r[0]=rd(m,0x8c60b514u,4);goto L8c60b4d4; /* mov.l 0x8c60b514,r0 */
L8c60b4d4: *stop=0x8c60b4d4u;r[3]=r[3]&65535u;goto L8c60b4d6; /* extu.w r3,r3 */
L8c60b4d6: *stop=0x8c60b4d6u;r[3]<<=2;goto L8c60b4d8; /* shll2 r3 */
L8c60b4d8: *stop=0x8c60b4d8u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60b4da; /* mov.l @r15+,r13 */
L8c60b4da: *stop=0x8c60b4dau;if(!valid_ram(r[0]+r[3],4u))return -3;r[2]=rd(m,r[3]+r[0],4);goto L8c60b4dc; /* mov.l @(r0,r3),r2 */
L8c60b4dc: target=r[2];*stop=0x8c60b4deu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_motion_sampling_8c60b81c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60b81c;
L8c60b81c: *stop=0x8c60b81cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60b81e; /* mov.l r14,@-r15 */
L8c60b81e: *stop=0x8c60b81eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60b820; /* mov.l r13,@-r15 */
L8c60b820: *stop=0x8c60b820u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c60b822; /* mov.l r12,@-r15 */
L8c60b822: *stop=0x8c60b822u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c60b824; /* mov.l r11,@-r15 */
L8c60b824: *stop=0x8c60b824u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c60b826; /* mov.l r10,@-r15 */
L8c60b826: *stop=0x8c60b826u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto L8c60b828; /* mov.l r9,@-r15 */
L8c60b828: *stop=0x8c60b828u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c60b82a; /* fmov.s fr15,@-r15 */
L8c60b82a: *stop=0x8c60b82au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c60b82c; /* sts.l PR,@-r15 */
L8c60b82c: *stop=0x8c60b82cu;r[15]+=4294967260u;goto L8c60b82e; /* add #-0x24,r15 */
L8c60b82e: *stop=0x8c60b82eu;r[3]=rd(m,0x8c60ba3cu,4);goto L8c60b830; /* mov.l 0x8c60ba3c,r3 */
L8c60b830: *stop=0x8c60b830u;r[11]=r[15];goto L8c60b832; /* mov r15,r11 */
L8c60b832: *stop=0x8c60b832u;r[4]=rd(m,0x8c60ba44u,4);goto L8c60b834; /* mov.l 0x8c60ba44,r4 */
L8c60b834: *stop=0x8c60b834u;r[11]+=24u;goto L8c60b836; /* add #0x18,r11 */
L8c60b836: *stop=0x8c60b836u;if(!valid_ram(r[3],2u))return -3;r[10]=sx(m,r[3],2);goto L8c60b838; /* mov.w @r3,r10 */
L8c60b838: *stop=0x8c60b838u;r[0]=0x8c60ba48u;goto L8c60b83a; /* mova 0x8c60ba48,r0 */
L8c60b83a: *stop=0x8c60b83au;if(!valid_ram(r[4],2u))return -3;r[5]=sx(m,r[4],2);goto L8c60b83c; /* mov.w @r4,r5 */
L8c60b83c: *stop=0x8c60b83cu;r[10]=r[10]&65535u;goto L8c60b83e; /* extu.w r10,r10 */
L8c60b83e: *stop=0x8c60b83eu;r[14]=rd(m,0x8c60ba4cu,4);goto L8c60b840; /* mov.l 0x8c60ba4c,r14 */
L8c60b840: *stop=0x8c60b840u;r[2]=r[10];goto L8c60b842; /* mov r10,r2 */
L8c60b842: *stop=0x8c60b842u;r[5]=r[5]&65535u;goto L8c60b844; /* extu.w r5,r5 */
L8c60b844: *stop=0x8c60b844u;r[9]=rd(m,0x8c60ba40u,4);goto L8c60b846; /* mov.l 0x8c60ba40,r9 */
L8c60b846: *stop=0x8c60b846u;*sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c60b848; /* shll r10 */
L8c60b848: *stop=0x8c60b848u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c60b84a; /* cmp/pl r5 */
L8c60b84a: *stop=0x8c60b84au;r[10]+=r[2];goto L8c60b84c; /* add r2,r10 */
L8c60b84c: take=!(*sr&1u);*stop=0x8c60b84eu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[0]);if(take)goto L8c60b8cc;goto L8c60b850; /* bf/s 0x8c60b8cc */
L8c60b850: *stop=0x8c60b850u;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[10]);goto L8c60b852; /* cmp/ge r10,r5 */
L8c60b852: if((*sr&1u))goto L8c60b8bc;goto L8c60b854; /* bt 0x8c60b8bc */
L8c60b854: *stop=0x8c60b854u;r[1]=12u;goto L8c60b856; /* mov #0xc,r1 */
L8c60b856: *stop=0x8c60b856u;if(!valid_ram(r[4],2u))return -3;r[2]=sx(m,r[4],2);goto L8c60b858; /* mov.w @r4,r2 */
L8c60b858: *stop=0x8c60b858u;r[1]+=r[14];goto L8c60b85a; /* add r14,r1 */
L8c60b85a: *stop=0x8c60b85au;r[2]+=4294967295u;goto L8c60b85c; /* add #-0x1,r2 */
L8c60b85c: *stop=0x8c60b85cu;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[2],2);goto L8c60b85e; /* mov.w r2,@r4 */
L8c60b85e: *stop=0x8c60b85eu;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c60b860; /* mov.w @r4,r0 */
L8c60b860: *stop=0x8c60b860u;r[0]^=0x1u;goto L8c60b862; /* xor #0x1,r0 */
L8c60b862: *stop=0x8c60b862u;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[0],2);goto L8c60b864; /* mov.w r0,@r4 */
L8c60b864: *stop=0x8c60b864u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto L8c60b866; /* fmov.s @r1,fr2 */
L8c60b866: *stop=0x8c60b866u;r[1]=4u;goto L8c60b868; /* mov #0x4,r1 */
L8c60b868: *stop=0x8c60b868u;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[14]);goto L8c60b86a; /* fmov.s @r14,fr3 */
L8c60b86a: *stop=0x8c60b86au;r[1]+=r[14];goto L8c60b86c; /* add r14,r1 */
L8c60b86c: *stop=0x8c60b86cu;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c60b86e; /* mov.w @r4,r0 */
L8c60b86e: *stop=0x8c60b86eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c60b870; /* fsub fr3,fr2 */
L8c60b870: *stop=0x8c60b870u;r[0]=r[0]&65535u;goto L8c60b872; /* extu.w r0,r0 */
L8c60b872: *stop=0x8c60b872u;r[2]=r[0];goto L8c60b874; /* mov r0,r2 */
L8c60b874: *stop=0x8c60b874u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60b876; /* shll r0 */
L8c60b876: *stop=0x8c60b876u;r[0]+=r[2];goto L8c60b878; /* add r2,r0 */
L8c60b878: *stop=0x8c60b878u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[15],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60b87a; /* fmul fr15,fr2 */
L8c60b87a: *stop=0x8c60b87au;r[0]+=6u;goto L8c60b87c; /* add #0x6,r0 */
L8c60b87c: *stop=0x8c60b87cu;r[0]<<=2;goto L8c60b87e; /* shll2 r0 */
L8c60b87e: *stop=0x8c60b87eu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[14]);goto L8c60b880; /* fmov fr2,@(r0,r14) */
L8c60b880: *stop=0x8c60b880u;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c60b882; /* mov.w @r4,r0 */
L8c60b882: *stop=0x8c60b882u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto L8c60b884; /* fmov.s @r1,fr3 */
L8c60b884: *stop=0x8c60b884u;r[1]=16u;goto L8c60b886; /* mov #0x10,r1 */
L8c60b886: *stop=0x8c60b886u;r[0]=r[0]&65535u;goto L8c60b888; /* extu.w r0,r0 */
L8c60b888: *stop=0x8c60b888u;r[2]=r[0];goto L8c60b88a; /* mov r0,r2 */
L8c60b88a: *stop=0x8c60b88au;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60b88c; /* shll r0 */
L8c60b88c: *stop=0x8c60b88cu;r[0]+=r[2];goto L8c60b88e; /* add r2,r0 */
L8c60b88e: *stop=0x8c60b88eu;r[0]+=7u;goto L8c60b890; /* add #0x7,r0 */
L8c60b890: *stop=0x8c60b890u;r[1]+=r[14];goto L8c60b892; /* add r14,r1 */
L8c60b892: *stop=0x8c60b892u;r[0]<<=2;goto L8c60b894; /* shll2 r0 */
L8c60b894: *stop=0x8c60b894u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto L8c60b896; /* fmov.s @r1,fr2 */
L8c60b896: *stop=0x8c60b896u;r[1]=8u;goto L8c60b898; /* mov #0x8,r1 */
L8c60b898: *stop=0x8c60b898u;r[1]+=r[14];goto L8c60b89a; /* add r14,r1 */
L8c60b89a: *stop=0x8c60b89au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c60b89c; /* fsub fr3,fr2 */
L8c60b89c: *stop=0x8c60b89cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[15],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60b89e; /* fmul fr15,fr2 */
L8c60b89e: *stop=0x8c60b89eu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[14]);goto L8c60b8a0; /* fmov fr2,@(r0,r14) */
L8c60b8a0: *stop=0x8c60b8a0u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto L8c60b8a2; /* fmov.s @r1,fr3 */
L8c60b8a2: *stop=0x8c60b8a2u;r[1]=20u;goto L8c60b8a4; /* mov #0x14,r1 */
L8c60b8a4: *stop=0x8c60b8a4u;r[1]+=r[14];goto L8c60b8a6; /* add r14,r1 */
L8c60b8a6: *stop=0x8c60b8a6u;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c60b8a8; /* mov.w @r4,r0 */
L8c60b8a8: *stop=0x8c60b8a8u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto L8c60b8aa; /* fmov.s @r1,fr2 */
L8c60b8aa: *stop=0x8c60b8aau;r[0]=r[0]&65535u;goto L8c60b8ac; /* extu.w r0,r0 */
L8c60b8ac: *stop=0x8c60b8acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c60b8ae; /* fsub fr3,fr2 */
L8c60b8ae: *stop=0x8c60b8aeu;r[2]=r[0];goto L8c60b8b0; /* mov r0,r2 */
L8c60b8b0: *stop=0x8c60b8b0u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60b8b2; /* shll r0 */
L8c60b8b2: *stop=0x8c60b8b2u;r[0]+=r[2];goto L8c60b8b4; /* add r2,r0 */
L8c60b8b4: *stop=0x8c60b8b4u;r[0]+=8u;goto L8c60b8b6; /* add #0x8,r0 */
L8c60b8b6: *stop=0x8c60b8b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[15],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60b8b8; /* fmul fr15,fr2 */
L8c60b8b8: *stop=0x8c60b8b8u;r[0]<<=2;goto L8c60b8ba; /* shll2 r0 */
L8c60b8ba: *stop=0x8c60b8bau;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[14]);goto L8c60b8bc; /* fmov fr2,@(r0,r14) */
L8c60b8bc: *stop=0x8c60b8bcu;r[2]=rd(m,0x8c60ba50u,4);goto L8c60b8be; /* mov.l 0x8c60ba50,r2 */
L8c60b8be: *stop=0x8c60b8beu;r[5]=r[11];goto L8c60b8c0; /* mov r11,r5 */
L8c60b8c0: *stop=0x8c60b8c0u;if(!valid_ram(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[9]);goto L8c60b8c2; /* fmov.s @r9,fr4 */
L8c60b8c2: *stop=0x8c60b8c2u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60b8c4; /* mov.l @r2,r3 */
L8c60b8c4: target=r[3];*pr=0x8c60b8c8u;*stop=0x8c60b8c6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60b8c8; /* jsr @r3 */
L8c60b8c8: *stop=0x8c60b8cau;goto L8c60b8dc; /* bra 0x8c60b8dc */
L8c60b8cc: *stop=0x8c60b8ccu;r[0]=4u;goto L8c60b8ce; /* mov #0x4,r0 */
L8c60b8ce: *stop=0x8c60b8ceu;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[14]);goto L8c60b8d0; /* fmov.s @r14,fr3 */
L8c60b8d0: *stop=0x8c60b8d0u;if(!valid_ram(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[11]);goto L8c60b8d2; /* fmov.s fr3,@r11 */
L8c60b8d2: *stop=0x8c60b8d2u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c60b8d4; /* fmov.s @(r0,r14),fr2 */
L8c60b8d4: *stop=0x8c60b8d4u;if(!valid_ram(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[11]);goto L8c60b8d6; /* fmov fr2,@(r0,r11) */
L8c60b8d6: *stop=0x8c60b8d6u;r[0]=8u;goto L8c60b8d8; /* mov #0x8,r0 */
L8c60b8d8: *stop=0x8c60b8d8u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c60b8da; /* fmov.s @(r0,r14),fr3 */
L8c60b8da: *stop=0x8c60b8dau;if(!valid_ram(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[11]);goto L8c60b8dc; /* fmov fr3,@(r0,r11) */
L8c60b8dc: *stop=0x8c60b8dcu;r[4]=rd(m,0x8c60ba54u,4);goto L8c60b8de; /* mov.l 0x8c60ba54,r4 */
L8c60b8de: *stop=0x8c60b8deu;r[14]=rd(m,0x8c60ba58u,4);goto L8c60b8e0; /* mov.l 0x8c60ba58,r14 */
L8c60b8e0: *stop=0x8c60b8e0u;if(!valid_ram(r[4],2u))return -3;r[5]=sx(m,r[4],2);goto L8c60b8e2; /* mov.w @r4,r5 */
L8c60b8e2: *stop=0x8c60b8e2u;r[5]=r[5]&65535u;goto L8c60b8e4; /* extu.w r5,r5 */
L8c60b8e4: *stop=0x8c60b8e4u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c60b8e6; /* cmp/pl r5 */
L8c60b8e6: take=!(*sr&1u);*stop=0x8c60b8e8u;r[12]=r[15];if(take)goto L8c60b96a;goto L8c60b8ea; /* bf/s 0x8c60b96a */
L8c60b8ea: *stop=0x8c60b8eau;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[10]);goto L8c60b8ec; /* cmp/ge r10,r5 */
L8c60b8ec: if((*sr&1u))goto L8c60b95a;goto L8c60b8ee; /* bt 0x8c60b95a */
L8c60b8ee: *stop=0x8c60b8eeu;if(!valid_ram(r[4],2u))return -3;r[3]=sx(m,r[4],2);goto L8c60b8f0; /* mov.w @r4,r3 */
L8c60b8f0: *stop=0x8c60b8f0u;r[3]+=4294967295u;goto L8c60b8f2; /* add #-0x1,r3 */
L8c60b8f2: *stop=0x8c60b8f2u;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[3],2);goto L8c60b8f4; /* mov.w r3,@r4 */
L8c60b8f4: *stop=0x8c60b8f4u;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c60b8f6; /* mov.w @r4,r0 */
L8c60b8f6: *stop=0x8c60b8f6u;r[0]^=0x1u;goto L8c60b8f8; /* xor #0x1,r0 */
L8c60b8f8: *stop=0x8c60b8f8u;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[0],2);goto L8c60b8fa; /* mov.w r0,@r4 */
L8c60b8fa: *stop=0x8c60b8fau;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c60b8fc; /* mov.w @r4,r0 */
L8c60b8fc: *stop=0x8c60b8fcu;if(!valid_ram(r[14]+12u,4u))return -3;r[2]=rd(m,r[14]+0xcu,4);goto L8c60b8fe; /* mov.l @(0xc,r14),r2 */
L8c60b8fe: *stop=0x8c60b8feu;r[0]=r[0]&65535u;goto L8c60b900; /* extu.w r0,r0 */
L8c60b900: *stop=0x8c60b900u;r[3]=r[0];goto L8c60b902; /* mov r0,r3 */
L8c60b902: *stop=0x8c60b902u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60b904; /* shll r0 */
L8c60b904: *stop=0x8c60b904u;r[0]+=r[3];goto L8c60b906; /* add r3,r0 */
L8c60b906: *stop=0x8c60b906u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c60b908; /* mov.l @r14,r3 */
L8c60b908: *stop=0x8c60b908u;r[0]+=6u;goto L8c60b90a; /* add #0x6,r0 */
L8c60b90a: *stop=0x8c60b90au;r[2]-=r[3];goto L8c60b90c; /* sub r3,r2 */
L8c60b90c: *stop=0x8c60b90cu;*fpul=r[2];goto L8c60b90e; /* lds r2,FPUL */
L8c60b90e: *stop=0x8c60b90eu;r[0]<<=2;goto L8c60b910; /* shll2 r0 */
L8c60b910: *stop=0x8c60b910u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60b912; /* float FPUL,fr3 */
L8c60b912: *stop=0x8c60b912u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60b914; /* fmul fr15,fr3 */
L8c60b914: *stop=0x8c60b914u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c60b916; /* ftrc fr3,FPUL */
L8c60b916: *stop=0x8c60b916u;r[3]=*fpul;goto L8c60b918; /* sts FPUL,r3 */
L8c60b918: *stop=0x8c60b918u;if(!valid_ram(r[0]+r[14],4u))return -3;wr(m,r[14]+r[0],r[3],4);goto L8c60b91a; /* mov.l r3,@(r0,r14) */
L8c60b91a: *stop=0x8c60b91au;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c60b91c; /* mov.w @r4,r0 */
L8c60b91c: *stop=0x8c60b91cu;if(!valid_ram(r[14]+16u,4u))return -3;r[2]=rd(m,r[14]+0x10u,4);goto L8c60b91e; /* mov.l @(0x10,r14),r2 */
L8c60b91e: *stop=0x8c60b91eu;r[0]=r[0]&65535u;goto L8c60b920; /* extu.w r0,r0 */
L8c60b920: *stop=0x8c60b920u;r[3]=r[0];goto L8c60b922; /* mov r0,r3 */
L8c60b922: *stop=0x8c60b922u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60b924; /* shll r0 */
L8c60b924: *stop=0x8c60b924u;r[0]+=r[3];goto L8c60b926; /* add r3,r0 */
L8c60b926: *stop=0x8c60b926u;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c60b928; /* mov.l @(0x4,r14),r3 */
L8c60b928: *stop=0x8c60b928u;r[0]+=7u;goto L8c60b92a; /* add #0x7,r0 */
L8c60b92a: *stop=0x8c60b92au;r[2]-=r[3];goto L8c60b92c; /* sub r3,r2 */
L8c60b92c: *stop=0x8c60b92cu;r[0]<<=2;goto L8c60b92e; /* shll2 r0 */
L8c60b92e: *stop=0x8c60b92eu;*fpul=r[2];goto L8c60b930; /* lds r2,FPUL */
L8c60b930: *stop=0x8c60b930u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60b932; /* float FPUL,fr3 */
L8c60b932: *stop=0x8c60b932u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60b934; /* fmul fr15,fr3 */
L8c60b934: *stop=0x8c60b934u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c60b936; /* ftrc fr3,FPUL */
L8c60b936: *stop=0x8c60b936u;r[3]=*fpul;goto L8c60b938; /* sts FPUL,r3 */
L8c60b938: *stop=0x8c60b938u;if(!valid_ram(r[0]+r[14],4u))return -3;wr(m,r[14]+r[0],r[3],4);goto L8c60b93a; /* mov.l r3,@(r0,r14) */
L8c60b93a: *stop=0x8c60b93au;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c60b93c; /* mov.w @r4,r0 */
L8c60b93c: *stop=0x8c60b93cu;if(!valid_ram(r[14]+20u,4u))return -3;r[2]=rd(m,r[14]+0x14u,4);goto L8c60b93e; /* mov.l @(0x14,r14),r2 */
L8c60b93e: *stop=0x8c60b93eu;r[0]=r[0]&65535u;goto L8c60b940; /* extu.w r0,r0 */
L8c60b940: *stop=0x8c60b940u;r[3]=r[0];goto L8c60b942; /* mov r0,r3 */
L8c60b942: *stop=0x8c60b942u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60b944; /* shll r0 */
L8c60b944: *stop=0x8c60b944u;r[0]+=r[3];goto L8c60b946; /* add r3,r0 */
L8c60b946: *stop=0x8c60b946u;if(!valid_ram(r[14]+8u,4u))return -3;r[3]=rd(m,r[14]+0x8u,4);goto L8c60b948; /* mov.l @(0x8,r14),r3 */
L8c60b948: *stop=0x8c60b948u;r[0]+=8u;goto L8c60b94a; /* add #0x8,r0 */
L8c60b94a: *stop=0x8c60b94au;r[2]-=r[3];goto L8c60b94c; /* sub r3,r2 */
L8c60b94c: *stop=0x8c60b94cu;*fpul=r[2];goto L8c60b94e; /* lds r2,FPUL */
L8c60b94e: *stop=0x8c60b94eu;r[0]<<=2;goto L8c60b950; /* shll2 r0 */
L8c60b950: *stop=0x8c60b950u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c60b952; /* float FPUL,fr3 */
L8c60b952: *stop=0x8c60b952u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60b954; /* fmul fr15,fr3 */
L8c60b954: *stop=0x8c60b954u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c60b956; /* ftrc fr3,FPUL */
L8c60b956: *stop=0x8c60b956u;r[3]=*fpul;goto L8c60b958; /* sts FPUL,r3 */
L8c60b958: *stop=0x8c60b958u;if(!valid_ram(r[0]+r[14],4u))return -3;wr(m,r[14]+r[0],r[3],4);goto L8c60b95a; /* mov.l r3,@(r0,r14) */
L8c60b95a: *stop=0x8c60b95au;r[1]=rd(m,0x8c60ba5cu,4);goto L8c60b95c; /* mov.l 0x8c60ba5c,r1 */
L8c60b95c: *stop=0x8c60b95cu;r[5]=r[12];goto L8c60b95e; /* mov r12,r5 */
L8c60b95e: *stop=0x8c60b95eu;if(!valid_ram(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[9]);goto L8c60b960; /* fmov.s @r9,fr4 */
L8c60b960: *stop=0x8c60b960u;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c60b962; /* mov.l @r1,r2 */
L8c60b962: target=r[2];*pr=0x8c60b966u;*stop=0x8c60b964u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60b966; /* jsr @r2 */
L8c60b966: *stop=0x8c60b968u;goto L8c60b976; /* bra 0x8c60b976 */
L8c60b96a: *stop=0x8c60b96au;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c60b96c; /* mov.l @r14,r2 */
L8c60b96c: *stop=0x8c60b96cu;if(!valid_ram(r[12],4u))return -3;wr(m,r[12],r[2],4);goto L8c60b96e; /* mov.l r2,@r12 */
L8c60b96e: *stop=0x8c60b96eu;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c60b970; /* mov.l @(0x4,r14),r3 */
L8c60b970: *stop=0x8c60b970u;if(!valid_ram(r[12]+4u,4u))return -3;wr(m,r[12]+0x4u,r[3],4);goto L8c60b972; /* mov.l r3,@(0x4,r12) */
L8c60b972: *stop=0x8c60b972u;if(!valid_ram(r[14]+8u,4u))return -3;r[2]=rd(m,r[14]+0x8u,4);goto L8c60b974; /* mov.l @(0x8,r14),r2 */
L8c60b974: *stop=0x8c60b974u;if(!valid_ram(r[12]+8u,4u))return -3;wr(m,r[12]+0x8u,r[2],4);goto L8c60b976; /* mov.l r2,@(0x8,r12) */
L8c60b976: *stop=0x8c60b976u;r[4]=rd(m,0x8c60ba60u,4);goto L8c60b978; /* mov.l 0x8c60ba60,r4 */
L8c60b978: *stop=0x8c60b978u;r[13]=r[15];goto L8c60b97a; /* mov r15,r13 */
L8c60b97a: *stop=0x8c60b97au;r[14]=rd(m,0x8c60ba64u,4);goto L8c60b97c; /* mov.l 0x8c60ba64,r14 */
L8c60b97c: *stop=0x8c60b97cu;if(!valid_ram(r[4],2u))return -3;r[5]=sx(m,r[4],2);goto L8c60b97e; /* mov.w @r4,r5 */
L8c60b97e: *stop=0x8c60b97eu;r[5]=r[5]&65535u;goto L8c60b980; /* extu.w r5,r5 */
L8c60b980: *stop=0x8c60b980u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c60b982; /* cmp/pl r5 */
L8c60b982: take=!(*sr&1u);*stop=0x8c60b984u;r[13]+=12u;if(take)goto L8c60ba02;goto L8c60b986; /* bf/s 0x8c60ba02 */
L8c60b986: *stop=0x8c60b986u;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[10]);goto L8c60b988; /* cmp/ge r10,r5 */
L8c60b988: if((*sr&1u))goto L8c60b9f2;goto L8c60b98a; /* bt 0x8c60b9f2 */
L8c60b98a: *stop=0x8c60b98au;r[1]=12u;goto L8c60b98c; /* mov #0xc,r1 */
L8c60b98c: *stop=0x8c60b98cu;if(!valid_ram(r[4],2u))return -3;r[3]=sx(m,r[4],2);goto L8c60b98e; /* mov.w @r4,r3 */
L8c60b98e: *stop=0x8c60b98eu;r[1]+=r[14];goto L8c60b990; /* add r14,r1 */
L8c60b990: *stop=0x8c60b990u;r[3]+=4294967295u;goto L8c60b992; /* add #-0x1,r3 */
L8c60b992: *stop=0x8c60b992u;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[3],2);goto L8c60b994; /* mov.w r3,@r4 */
L8c60b994: *stop=0x8c60b994u;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c60b996; /* mov.w @r4,r0 */
L8c60b996: *stop=0x8c60b996u;r[0]^=0x1u;goto L8c60b998; /* xor #0x1,r0 */
L8c60b998: *stop=0x8c60b998u;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[0],2);goto L8c60b99a; /* mov.w r0,@r4 */
L8c60b99a: *stop=0x8c60b99au;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto L8c60b99c; /* fmov.s @r1,fr2 */
L8c60b99c: *stop=0x8c60b99cu;r[1]=4u;goto L8c60b99e; /* mov #0x4,r1 */
L8c60b99e: *stop=0x8c60b99eu;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[14]);goto L8c60b9a0; /* fmov.s @r14,fr3 */
L8c60b9a0: *stop=0x8c60b9a0u;r[1]+=r[14];goto L8c60b9a2; /* add r14,r1 */
L8c60b9a2: *stop=0x8c60b9a2u;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c60b9a4; /* mov.w @r4,r0 */
L8c60b9a4: *stop=0x8c60b9a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c60b9a6; /* fsub fr3,fr2 */
L8c60b9a6: *stop=0x8c60b9a6u;r[0]=r[0]&65535u;goto L8c60b9a8; /* extu.w r0,r0 */
L8c60b9a8: *stop=0x8c60b9a8u;r[3]=r[0];goto L8c60b9aa; /* mov r0,r3 */
L8c60b9aa: *stop=0x8c60b9aau;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60b9ac; /* shll r0 */
L8c60b9ac: *stop=0x8c60b9acu;r[0]+=r[3];goto L8c60b9ae; /* add r3,r0 */
L8c60b9ae: *stop=0x8c60b9aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[15],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60b9b0; /* fmul fr15,fr2 */
L8c60b9b0: *stop=0x8c60b9b0u;r[0]+=6u;goto L8c60b9b2; /* add #0x6,r0 */
L8c60b9b2: *stop=0x8c60b9b2u;r[0]<<=2;goto L8c60b9b4; /* shll2 r0 */
L8c60b9b4: *stop=0x8c60b9b4u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[14]);goto L8c60b9b6; /* fmov fr2,@(r0,r14) */
L8c60b9b6: *stop=0x8c60b9b6u;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c60b9b8; /* mov.w @r4,r0 */
L8c60b9b8: *stop=0x8c60b9b8u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto L8c60b9ba; /* fmov.s @r1,fr3 */
L8c60b9ba: *stop=0x8c60b9bau;r[1]=16u;goto L8c60b9bc; /* mov #0x10,r1 */
L8c60b9bc: *stop=0x8c60b9bcu;r[0]=r[0]&65535u;goto L8c60b9be; /* extu.w r0,r0 */
L8c60b9be: *stop=0x8c60b9beu;r[3]=r[0];goto L8c60b9c0; /* mov r0,r3 */
L8c60b9c0: *stop=0x8c60b9c0u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60b9c2; /* shll r0 */
L8c60b9c2: *stop=0x8c60b9c2u;r[0]+=r[3];goto L8c60b9c4; /* add r3,r0 */
L8c60b9c4: *stop=0x8c60b9c4u;r[0]+=7u;goto L8c60b9c6; /* add #0x7,r0 */
L8c60b9c6: *stop=0x8c60b9c6u;r[1]+=r[14];goto L8c60b9c8; /* add r14,r1 */
L8c60b9c8: *stop=0x8c60b9c8u;r[0]<<=2;goto L8c60b9ca; /* shll2 r0 */
L8c60b9ca: *stop=0x8c60b9cau;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto L8c60b9cc; /* fmov.s @r1,fr2 */
L8c60b9cc: *stop=0x8c60b9ccu;r[1]=8u;goto L8c60b9ce; /* mov #0x8,r1 */
L8c60b9ce: *stop=0x8c60b9ceu;r[1]+=r[14];goto L8c60b9d0; /* add r14,r1 */
L8c60b9d0: *stop=0x8c60b9d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c60b9d2; /* fsub fr3,fr2 */
L8c60b9d2: *stop=0x8c60b9d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[15],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60b9d4; /* fmul fr15,fr2 */
L8c60b9d4: *stop=0x8c60b9d4u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[14]);goto L8c60b9d6; /* fmov fr2,@(r0,r14) */
L8c60b9d6: *stop=0x8c60b9d6u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto L8c60b9d8; /* fmov.s @r1,fr3 */
L8c60b9d8: *stop=0x8c60b9d8u;r[1]=20u;goto L8c60b9da; /* mov #0x14,r1 */
L8c60b9da: *stop=0x8c60b9dau;r[1]+=r[14];goto L8c60b9dc; /* add r14,r1 */
L8c60b9dc: *stop=0x8c60b9dcu;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c60b9de; /* mov.w @r4,r0 */
L8c60b9de: *stop=0x8c60b9deu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto L8c60b9e0; /* fmov.s @r1,fr2 */
L8c60b9e0: *stop=0x8c60b9e0u;r[0]=r[0]&65535u;goto L8c60b9e2; /* extu.w r0,r0 */
L8c60b9e2: *stop=0x8c60b9e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c60b9e4; /* fsub fr3,fr2 */
L8c60b9e4: *stop=0x8c60b9e4u;r[3]=r[0];goto L8c60b9e6; /* mov r0,r3 */
L8c60b9e6: *stop=0x8c60b9e6u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60b9e8; /* shll r0 */
L8c60b9e8: *stop=0x8c60b9e8u;r[0]+=r[3];goto L8c60b9ea; /* add r3,r0 */
L8c60b9ea: *stop=0x8c60b9eau;r[0]+=8u;goto L8c60b9ec; /* add #0x8,r0 */
L8c60b9ec: *stop=0x8c60b9ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[15],&f->fpscr,&fr[2]);if(status)return status;}goto L8c60b9ee; /* fmul fr15,fr2 */
L8c60b9ee: *stop=0x8c60b9eeu;r[0]<<=2;goto L8c60b9f0; /* shll2 r0 */
L8c60b9f0: *stop=0x8c60b9f0u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[14]);goto L8c60b9f2; /* fmov fr2,@(r0,r14) */
L8c60b9f2: *stop=0x8c60b9f2u;r[2]=rd(m,0x8c60ba50u,4);goto L8c60b9f4; /* mov.l 0x8c60ba50,r2 */
L8c60b9f4: *stop=0x8c60b9f4u;r[5]=r[13];goto L8c60b9f6; /* mov r13,r5 */
L8c60b9f6: *stop=0x8c60b9f6u;if(!valid_ram(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[9]);goto L8c60b9f8; /* fmov.s @r9,fr4 */
L8c60b9f8: *stop=0x8c60b9f8u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60b9fa; /* mov.l @r2,r3 */
L8c60b9fa: target=r[3];*pr=0x8c60b9feu;*stop=0x8c60b9fcu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60b9fe; /* jsr @r3 */
L8c60b9fe: *stop=0x8c60ba00u;goto L8c60ba12; /* bra 0x8c60ba12 */
L8c60ba02: *stop=0x8c60ba02u;r[0]=4u;goto L8c60ba04; /* mov #0x4,r0 */
L8c60ba04: *stop=0x8c60ba04u;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[14]);goto L8c60ba06; /* fmov.s @r14,fr3 */
L8c60ba06: *stop=0x8c60ba06u;if(!valid_ram(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[13]);goto L8c60ba08; /* fmov.s fr3,@r13 */
L8c60ba08: *stop=0x8c60ba08u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[14]);goto L8c60ba0a; /* fmov.s @(r0,r14),fr2 */
L8c60ba0a: *stop=0x8c60ba0au;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[13]);goto L8c60ba0c; /* fmov fr2,@(r0,r13) */
L8c60ba0c: *stop=0x8c60ba0cu;r[0]=8u;goto L8c60ba0e; /* mov #0x8,r0 */
L8c60ba0e: *stop=0x8c60ba0eu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c60ba10; /* fmov.s @(r0,r14),fr3 */
L8c60ba10: *stop=0x8c60ba10u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[13]);goto L8c60ba12; /* fmov fr3,@(r0,r13) */
L8c60ba12: *stop=0x8c60ba12u;r[3]=rd(m,0x8c60ba68u,4);goto L8c60ba14; /* mov.l 0x8c60ba68,r3 */
L8c60ba14: target=r[3];*pr=0x8c60ba18u;*stop=0x8c60ba16u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60ba18; /* jsr @r3 */
L8c60ba18: *stop=0x8c60ba18u;r[2]=rd(m,0x8c60ba6cu,4);goto L8c60ba1a; /* mov.l 0x8c60ba6c,r2 */
L8c60ba1a: *stop=0x8c60ba1au;r[3]=rd(m,0x8c60ba70u,4);goto L8c60ba1c; /* mov.l 0x8c60ba70,r3 */
L8c60ba1c: *stop=0x8c60ba1cu;if(!valid_ram(r[2],4u))return -3;r[5]=rd(m,r[2],4);goto L8c60ba1e; /* mov.l @r2,r5 */
L8c60ba1e: target=r[3];*pr=0x8c60ba22u;*stop=0x8c60ba20u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60ba22; /* jsr @r3 */
L8c60ba22: *stop=0x8c60ba22u;r[2]=rd(m,0x8c60ba74u,4);goto L8c60ba24; /* mov.l 0x8c60ba74,r2 */
L8c60ba24: target=r[2];*pr=0x8c60ba28u;*stop=0x8c60ba26u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60ba28; /* jsr @r2 */
L8c60ba28: *stop=0x8c60ba28u;r[15]+=36u;goto L8c60ba2a; /* add #0x24,r15 */
L8c60ba2a: *stop=0x8c60ba2au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c60ba2c; /* lds.l @r15+,PR */
L8c60ba2c: *stop=0x8c60ba2cu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c60ba2e; /* fmov.s @r15+,fr15 */
L8c60ba2e: *stop=0x8c60ba2eu;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c60ba30; /* mov.l @r15+,r9 */
L8c60ba30: *stop=0x8c60ba30u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c60ba32; /* mov.l @r15+,r10 */
L8c60ba32: *stop=0x8c60ba32u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c60ba34; /* mov.l @r15+,r11 */
L8c60ba34: *stop=0x8c60ba34u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c60ba36; /* mov.l @r15+,r12 */
L8c60ba36: *stop=0x8c60ba36u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60ba38; /* mov.l @r15+,r13 */
L8c60ba38: *stop=0x8c60ba38u;if(*pr!=entry_pr)return -3;*stop=0x8c60ba3au;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
