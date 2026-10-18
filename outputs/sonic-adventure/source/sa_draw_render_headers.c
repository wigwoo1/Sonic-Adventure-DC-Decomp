/* Expanded render-state, texture header and visibility; names inferred.
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
SA_STAGE_EXPORT int sa_render_state_8c654e60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c654e60;
L8c654e60: *stop=0x8c654e60u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c654e62; /* mov.l r14,@-r15 */
L8c654e62: *stop=0x8c654e62u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c654e64; /* mov.l r13,@-r15 */
L8c654e64: *stop=0x8c654e64u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c654e66; /* mov.l r12,@-r15 */
L8c654e66: *stop=0x8c654e66u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c654e68; /* mov.l r11,@-r15 */
L8c654e68: *stop=0x8c654e68u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c654e6a; /* mov.l r10,@-r15 */
L8c654e6a: *stop=0x8c654e6au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto L8c654e6c; /* mov.l r9,@-r15 */
L8c654e6c: *stop=0x8c654e6cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto L8c654e6e; /* mov.l r8,@-r15 */
L8c654e6e: *stop=0x8c654e6eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c654e70; /* sts.l PR,@-r15 */
L8c654e70: *stop=0x8c654e70u;r[15]+=4294967260u;goto L8c654e72; /* add #-0x24,r15 */
L8c654e72: *stop=0x8c654e72u;r[0]=sx(m,0x8c654ee0u,2);goto L8c654e74; /* mov.w 0x8c654ee0,r0 */
L8c654e74: *stop=0x8c654e74u;r[13]=r[4];goto L8c654e76; /* mov r4,r13 */
L8c654e76: *stop=0x8c654e76u;r[2]=r[15];goto L8c654e78; /* mov r15,r2 */
L8c654e78: *stop=0x8c654e78u;r[2]+=20u;goto L8c654e7a; /* add #0x14,r2 */
L8c654e7a: *stop=0x8c654e7au;if(!valid_ram(r[0]+r[13],4u))return -3;r[11]=rd(m,r[13]+r[0],4);goto L8c654e7c; /* mov.l @(r0,r13),r11 */
L8c654e7c: *stop=0x8c654e7cu;r[0]+=4u;goto L8c654e7e; /* add #0x4,r0 */
L8c654e7e: *stop=0x8c654e7eu;if(!valid_ram(r[0]+r[13],4u))return -3;r[14]=rd(m,r[13]+r[0],4);goto L8c654e80; /* mov.l @(r0,r13),r14 */
L8c654e80: *stop=0x8c654e80u;r[0]+=4u;goto L8c654e82; /* add #0x4,r0 */
L8c654e82: *stop=0x8c654e82u;if(!valid_ram(r[0]+r[13],4u))return -3;r[3]=rd(m,r[13]+r[0],4);goto L8c654e84; /* mov.l @(r0,r13),r3 */
L8c654e84: *stop=0x8c654e84u;r[0]=sx(m,0x8c654ee2u,2);goto L8c654e86; /* mov.w 0x8c654ee2,r0 */
L8c654e86: *stop=0x8c654e86u;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[3],4);goto L8c654e88; /* mov.l r3,@(0x8,r15) */
L8c654e88: *stop=0x8c654e88u;r[3]=rd(m,0x8c654ee4u,4);goto L8c654e8a; /* mov.l 0x8c654ee4,r3 */
L8c654e8a: *stop=0x8c654e8au;if(!valid_ram(r[0]+r[13],4u))return -3;r[10]=rd(m,r[13]+r[0],4);goto L8c654e8c; /* mov.l @(r0,r13),r10 */
L8c654e8c: *stop=0x8c654e8cu;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto L8c654e8e; /* mov.l @r3,r1 */
L8c654e8e: *stop=0x8c654e8eu;if(!valid_ram(r[3]+4u,4u))return -3;r[0]=rd(m,r[3]+0x4u,4);goto L8c654e90; /* mov.l @(0x4,r3),r0 */
L8c654e90: *stop=0x8c654e90u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto L8c654e92; /* mov.l r1,@r2 */
L8c654e92: *stop=0x8c654e92u;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[0],4);goto L8c654e94; /* mov.l r0,@(0x4,r2) */
L8c654e94: *stop=0x8c654e94u;if(!valid_ram(r[3]+12u,4u))return -3;r[0]=rd(m,r[3]+0xcu,4);goto L8c654e96; /* mov.l @(0xc,r3),r0 */
L8c654e96: *stop=0x8c654e96u;if(!valid_ram(r[3]+8u,4u))return -3;r[1]=rd(m,r[3]+0x8u,4);goto L8c654e98; /* mov.l @(0x8,r3),r1 */
L8c654e98: *stop=0x8c654e98u;r[9]=rd(m,0x8c654ee8u,4);goto L8c654e9a; /* mov.l 0x8c654ee8,r9 */
L8c654e9a: *stop=0x8c654e9au;if(!valid_ram(r[2]+8u,4u))return -3;wr(m,r[2]+0x8u,r[1],4);goto L8c654e9c; /* mov.l r1,@(0x8,r2) */
L8c654e9c: *stop=0x8c654e9cu;if(!valid_ram(r[2]+12u,4u))return -3;wr(m,r[2]+0xcu,r[0],4);goto L8c654e9e; /* mov.l r0,@(0xc,r2) */
L8c654e9e: *stop=0x8c654e9eu;r[2]=1u;goto L8c654ea0; /* mov #0x1,r2 */
L8c654ea0: *stop=0x8c654ea0u;if(!valid_ram(r[13],4u))return -3;r[12]=rd(m,r[13],4);goto L8c654ea2; /* mov.l @r13,r12 */
L8c654ea2: *stop=0x8c654ea2u;*sr=(*sr&~1u)|!(r[12]&r[2]);goto L8c654ea4; /* tst r12,r2 */
L8c654ea4: take=(*sr&1u);*stop=0x8c654ea6u;r[3]=2u;if(take)goto L8c654eb6;goto L8c654ea8; /* bt/s 0x8c654eb6 */
L8c654ea8: *stop=0x8c654ea8u;if(!valid_ram(r[13]+20u,4u))return -3;r[1]=rd(m,r[13]+0x14u,4);goto L8c654eaa; /* mov.l @(0x14,r13),r1 */
L8c654eaa: *stop=0x8c654eaau;r[0]=r[11];goto L8c654eac; /* mov r11,r0 */
L8c654eac: *stop=0x8c654eacu;r[0]&=r[9];goto L8c654eae; /* and r9,r0 */
L8c654eae: *stop=0x8c654eaeu;r[2]=29u;goto L8c654eb0; /* mov #0x1d,r2 */
L8c654eb0: *stop=0x8c654eb0u;r[11]=r[0];goto L8c654eb2; /* mov r0,r11 */
L8c654eb2: *stop=0x8c654eb2u;r[1]=(r[2]&0x80000000u)?((r[2]&31u)?r[1]>>((0u-r[2])&31u):0u):(r[1]<<(r[2]&31u));goto L8c654eb4; /* shld r2,r1 */
L8c654eb4: *stop=0x8c654eb4u;r[11]|=r[1];goto L8c654eb6; /* or r1,r11 */
L8c654eb6: *stop=0x8c654eb6u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c654eb8; /* tst r12,r3 */
L8c654eb8: if((*sr&1u))goto L8c654ec8;goto L8c654eba; /* bt 0x8c654ec8 */
L8c654eba: *stop=0x8c654ebau;r[1]=rd(m,0x8c654eecu,4);goto L8c654ebc; /* mov.l 0x8c654eec,r1 */
L8c654ebc: *stop=0x8c654ebcu;r[3]=27u;goto L8c654ebe; /* mov #0x1b,r3 */
L8c654ebe: *stop=0x8c654ebeu;if(!valid_ram(r[13]+24u,4u))return -3;r[2]=rd(m,r[13]+0x18u,4);goto L8c654ec0; /* mov.l @(0x18,r13),r2 */
L8c654ec0: *stop=0x8c654ec0u;r[1]&=r[11];goto L8c654ec2; /* and r11,r1 */
L8c654ec2: *stop=0x8c654ec2u;r[11]=r[1];goto L8c654ec4; /* mov r1,r11 */
L8c654ec4: *stop=0x8c654ec4u;r[2]=(r[3]&0x80000000u)?((r[3]&31u)?r[2]>>((0u-r[3])&31u):0u):(r[2]<<(r[3]&31u));goto L8c654ec6; /* shld r3,r2 */
L8c654ec6: *stop=0x8c654ec6u;r[11]|=r[2];goto L8c654ec8; /* or r2,r11 */
L8c654ec8: *stop=0x8c654ec8u;r[3]=32u;goto L8c654eca; /* mov #0x20,r3 */
L8c654eca: *stop=0x8c654ecau;r[7]=rd(m,0x8c654ef0u,4);goto L8c654ecc; /* mov.l 0x8c654ef0,r7 */
L8c654ecc: *stop=0x8c654eccu;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c654ece; /* tst r12,r3 */
L8c654ece: if((*sr&1u))goto L8c654f04;goto L8c654ed0; /* bt 0x8c654f04 */
L8c654ed0: *stop=0x8c654ed0u;if(!valid_ram(r[13]+40u,4u))return -3;r[1]=rd(m,r[13]+0x28u,4);goto L8c654ed2; /* mov.l @(0x28,r13),r1 */
L8c654ed2: *stop=0x8c654ed2u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c654ed4; /* tst r1,r1 */
L8c654ed4: if((*sr&1u))goto L8c654f00;goto L8c654ed6; /* bt 0x8c654f00 */
L8c654ed6: *stop=0x8c654ed8u;r[11]|=r[7];goto L8c654f04; /* bra 0x8c654f04 */
L8c654f00: *stop=0x8c654f00u;r[2]=rd(m,0x8c654f84u,4);goto L8c654f02; /* mov.l 0x8c654f84,r2 */
L8c654f02: *stop=0x8c654f02u;r[11]&=r[2];goto L8c654f04; /* and r2,r11 */
L8c654f04: *stop=0x8c654f04u;r[8]=8u;goto L8c654f06; /* mov #0x8,r8 */
L8c654f06: *stop=0x8c654f06u;r[8]&=r[12];goto L8c654f08; /* and r12,r8 */
L8c654f08: *stop=0x8c654f08u;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c654f0a; /* tst r8,r8 */
L8c654f0a: if((*sr&1u))goto L8c654f1e;goto L8c654f0c; /* bt 0x8c654f1e */
L8c654f0c: *stop=0x8c654f0cu;if(!valid_ram(r[13]+32u,4u))return -3;r[3]=rd(m,r[13]+0x20u,4);goto L8c654f0e; /* mov.l @(0x20,r13),r3 */
L8c654f0e: *stop=0x8c654f0eu;r[2]=rd(m,0x8c654f88u,4);goto L8c654f10; /* mov.l 0x8c654f88,r2 */
L8c654f10: *stop=0x8c654f10u;*sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c654f12; /* shlr r3 */
L8c654f12: *stop=0x8c654f12u;r[3]<<=16;goto L8c654f14; /* shll16 r3 */
L8c654f14: *stop=0x8c654f14u;r[3]<<=8;goto L8c654f16; /* shll8 r3 */
L8c654f16: *stop=0x8c654f16u;r[2]&=r[11];goto L8c654f18; /* and r11,r2 */
L8c654f18: *stop=0x8c654f18u;r[11]=r[2];goto L8c654f1a; /* mov r2,r11 */
L8c654f1a: *stop=0x8c654f1au;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c654f1c; /* shll r3 */
L8c654f1c: *stop=0x8c654f1cu;r[11]|=r[3];goto L8c654f1e; /* or r3,r11 */
L8c654f1e: *stop=0x8c654f1eu;r[3]=sx(m,0x8c654f80u,2);goto L8c654f20; /* mov.w 0x8c654f80,r3 */
L8c654f20: *stop=0x8c654f20u;r[6]=rd(m,0x8c654f8cu,4);goto L8c654f22; /* mov.l 0x8c654f8c,r6 */
L8c654f22: *stop=0x8c654f22u;r[3]&=r[12];goto L8c654f24; /* and r12,r3 */
L8c654f24: *stop=0x8c654f24u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c654f26; /* tst r3,r3 */
L8c654f26: take=(*sr&1u);*stop=0x8c654f28u;if(!valid_ram(r[15]+12u,4u))return -3;wr(m,r[15]+0xcu,r[3],4);if(take)goto L8c654f3a;goto L8c654f2a; /* bt/s 0x8c654f3a */
L8c654f2a: *stop=0x8c654f2au;r[0]=64u;goto L8c654f2c; /* mov #0x40,r0 */
L8c654f2c: *stop=0x8c654f2cu;r[1]=r[11];goto L8c654f2e; /* mov r11,r1 */
L8c654f2e: *stop=0x8c654f2eu;if(!valid_ram(r[0]+r[13],4u))return -3;r[3]=rd(m,r[13]+r[0],4);goto L8c654f30; /* mov.l @(r0,r13),r3 */
L8c654f30: *stop=0x8c654f30u;r[1]&=r[6];goto L8c654f32; /* and r6,r1 */
L8c654f32: *stop=0x8c654f32u;r[11]=r[1];goto L8c654f34; /* mov r1,r11 */
L8c654f34: *stop=0x8c654f34u;r[3]<<=16;goto L8c654f36; /* shll16 r3 */
L8c654f36: *stop=0x8c654f36u;r[3]<<=8;goto L8c654f38; /* shll8 r3 */
L8c654f38: *stop=0x8c654f38u;r[11]|=r[3];goto L8c654f3a; /* or r3,r11 */
L8c654f3a: *stop=0x8c654f3au;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c654f3c; /* tst r8,r8 */
L8c654f3c: if((*sr&1u))goto L8c654f4e;goto L8c654f3e; /* bt 0x8c654f4e */
L8c654f3e: *stop=0x8c654f3eu;if(!valid_ram(r[13]+32u,4u))return -3;r[0]=rd(m,r[13]+0x20u,4);goto L8c654f40; /* mov.l @(0x20,r13),r0 */
L8c654f40: *stop=0x8c654f40u;r[3]=23u;goto L8c654f42; /* mov #0x17,r3 */
L8c654f42: *stop=0x8c654f42u;r[2]=rd(m,0x8c654f90u,4);goto L8c654f44; /* mov.l 0x8c654f90,r2 */
L8c654f44: *stop=0x8c654f44u;r[0]&=0x1u;goto L8c654f46; /* and #0x1,r0 */
L8c654f46: *stop=0x8c654f46u;r[2]&=r[11];goto L8c654f48; /* and r11,r2 */
L8c654f48: *stop=0x8c654f48u;r[11]=r[2];goto L8c654f4a; /* mov r2,r11 */
L8c654f4a: *stop=0x8c654f4au;r[0]=(r[3]&0x80000000u)?((r[3]&31u)?r[0]>>((0u-r[3])&31u):0u):(r[0]<<(r[3]&31u));goto L8c654f4c; /* shld r3,r0 */
L8c654f4c: *stop=0x8c654f4cu;r[11]|=r[0];goto L8c654f4e; /* or r0,r11 */
L8c654f4e: *stop=0x8c654f4eu;r[2]=rd(m,0x8c654f94u,4);goto L8c654f50; /* mov.l 0x8c654f94,r2 */
L8c654f50: *stop=0x8c654f50u;r[2]&=r[12];goto L8c654f52; /* and r12,r2 */
L8c654f52: *stop=0x8c654f52u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c654f54; /* tst r2,r2 */
L8c654f54: take=(*sr&1u);*stop=0x8c654f56u;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[2],4);if(take)goto L8c654f66;goto L8c654f58; /* bt/s 0x8c654f66 */
L8c654f58: *stop=0x8c654f58u;r[1]=rd(m,0x8c654f98u,4);goto L8c654f5a; /* mov.l 0x8c654f98,r1 */
L8c654f5a: *stop=0x8c654f5au;r[3]=22u;goto L8c654f5c; /* mov #0x16,r3 */
L8c654f5c: *stop=0x8c654f5cu;if(!valid_ram(r[13]+16u,4u))return -3;r[2]=rd(m,r[13]+0x10u,4);goto L8c654f5e; /* mov.l @(0x10,r13),r2 */
L8c654f5e: *stop=0x8c654f5eu;r[1]&=r[11];goto L8c654f60; /* and r11,r1 */
L8c654f60: *stop=0x8c654f60u;r[11]=r[1];goto L8c654f62; /* mov r1,r11 */
L8c654f62: *stop=0x8c654f62u;r[2]=(r[3]&0x80000000u)?((r[3]&31u)?r[2]>>((0u-r[3])&31u):0u):(r[2]<<(r[3]&31u));goto L8c654f64; /* shld r3,r2 */
L8c654f64: *stop=0x8c654f64u;r[11]|=r[2];goto L8c654f66; /* or r2,r11 */
L8c654f66: *stop=0x8c654f66u;r[3]=r[12];goto L8c654f68; /* mov r12,r3 */
L8c654f68: *stop=0x8c654f68u;*sr=(*sr&~1u)|!(r[7]&r[3]);goto L8c654f6a; /* tst r7,r3 */
L8c654f6a: if((*sr&1u))goto L8c654fa4;goto L8c654f6c; /* bt 0x8c654fa4 */
L8c654f6c: *stop=0x8c654f6cu;r[0]=sx(m,0x8c654f82u,2);goto L8c654f6e; /* mov.w 0x8c654f82,r0 */
L8c654f6e: *stop=0x8c654f6eu;if(!valid_ram(r[0]+r[13],4u))return -3;r[1]=rd(m,r[13]+r[0],4);goto L8c654f70; /* mov.l @(r0,r13),r1 */
L8c654f70: *stop=0x8c654f70u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c654f72; /* tst r1,r1 */
L8c654f72: if((*sr&1u))goto L8c654fa0;goto L8c654f74; /* bt 0x8c654fa0 */
L8c654f74: *stop=0x8c654f74u;r[2]=rd(m,0x8c654f9cu,4);goto L8c654f76; /* mov.l 0x8c654f9c,r2 */
L8c654f76: *stop=0x8c654f78u;r[11]|=r[2];goto L8c654fa4; /* bra 0x8c654fa4 */
L8c654fa0: *stop=0x8c654fa0u;r[1]=rd(m,0x8c654fc4u,4);goto L8c654fa2; /* mov.l 0x8c654fc4,r1 */
L8c654fa2: *stop=0x8c654fa2u;r[11]&=r[1];goto L8c654fa4; /* and r1,r11 */
L8c654fa4: *stop=0x8c654fa4u;r[3]=64u;goto L8c654fa6; /* mov #0x40,r3 */
L8c654fa6: *stop=0x8c654fa6u;r[5]=rd(m,0x8c654fc8u,4);goto L8c654fa8; /* mov.l 0x8c654fc8,r5 */
L8c654fa8: *stop=0x8c654fa8u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c654faa; /* tst r12,r3 */
L8c654faa: *stop=0x8c654faau;r[4]=r[14];goto L8c654fac; /* mov r14,r4 */
L8c654fac: take=(*sr&1u);*stop=0x8c654faeu;r[4]&=r[9];if(take)goto L8c655082;goto L8c654fb0; /* bt/s 0x8c655082 */
L8c654fb0: *stop=0x8c654fb0u;if(!valid_ram(r[13]+44u,4u))return -3;r[0]=rd(m,r[13]+0x2cu,4);goto L8c654fb2; /* mov.l @(0x2c,r13),r0 */
L8c654fb2: *stop=0x8c654fb2u;r[1]=12u;goto L8c654fb4; /* mov #0xc,r1 */
L8c654fb4: *stop=0x8c654fb4u;*sr=(*sr&~1u)|(r[0]>=r[1]);goto L8c654fb6; /* cmp/hs r1,r0 */
L8c654fb6: if((*sr&1u))goto L8c655082;goto L8c654fb8; /* bt 0x8c655082 */
L8c654fb8: *stop=0x8c654fb8u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c654fba; /* shll r0 */
L8c654fba: *stop=0x8c654fbau;r[1]=r[0];goto L8c654fbc; /* mov r0,r1 */
L8c654fbc: *stop=0x8c654fbcu;r[0]=0x8c654fccu;goto L8c654fbe; /* mova 0x8c654fcc,r0 */
L8c654fbe: *stop=0x8c654fbeu;if(!valid_ram(r[0]+r[1],2u))return -3;r[0]=sx(m,r[0]+r[1],2);goto L8c654fc0; /* mov.w @(r0,r1),r0 */
L8c654fc0: target=0x8c654fc4u+r[0];*stop=0x8c654fc2u;switch(target){case 0x8c654fe4u:goto L8c654fe4;case 0x8c654ff0u:goto L8c654ff0;case 0x8c654ffeu:goto L8c654ffe;case 0x8c655004u:goto L8c655004;case 0x8c65500au:goto L8c65500a;case 0x8c655010u:goto L8c655010;case 0x8c655016u:goto L8c655016;case 0x8c65501cu:goto L8c65501c;case 0x8c655022u:goto L8c655022;case 0x8c655080u:goto L8c655080;case 0x8c655082u:goto L8c655082;default:*stop=0x8c654fc0u;return -3;} /* braf r0 */
L8c654fe4: *stop=0x8c654fe4u;r[3]=rd(m,0x8c655040u,4);goto L8c654fe6; /* mov.l 0x8c655040,r3 */
L8c654fe6: *stop=0x8c654fe6u;r[2]=rd(m,0x8c655044u,4);goto L8c654fe8; /* mov.l 0x8c655044,r2 */
L8c654fe8: *stop=0x8c654fe8u;r[4]|=r[3];goto L8c654fea; /* or r3,r4 */
L8c654fea: *stop=0x8c654feau;r[14]=r[4];goto L8c654fec; /* mov r4,r14 */
L8c654fec: *stop=0x8c654feeu;r[14]&=r[5];goto L8c654ffa; /* bra 0x8c654ffa */
L8c654ff0: *stop=0x8c654ff0u;r[3]=rd(m,0x8c655048u,4);goto L8c654ff2; /* mov.l 0x8c655048,r3 */
L8c654ff2: *stop=0x8c654ff2u;r[2]=rd(m,0x8c65504cu,4);goto L8c654ff4; /* mov.l 0x8c65504c,r2 */
L8c654ff4: *stop=0x8c654ff4u;r[4]|=r[3];goto L8c654ff6; /* or r3,r4 */
L8c654ff6: *stop=0x8c654ff6u;r[14]=r[4];goto L8c654ff8; /* mov r4,r14 */
L8c654ff8: *stop=0x8c654ff8u;r[14]&=r[5];goto L8c654ffa; /* and r5,r14 */
L8c654ffa: *stop=0x8c654ffcu;r[14]|=r[2];goto L8c655082; /* bra 0x8c655082 */
L8c654ffe: *stop=0x8c654ffeu;r[14]=rd(m,0x8c655050u,4);goto L8c655000; /* mov.l 0x8c655050,r14 */
L8c655000: *stop=0x8c655002u;goto L8c655024; /* bra 0x8c655024 */
L8c655004: *stop=0x8c655004u;r[14]=rd(m,0x8c655054u,4);goto L8c655006; /* mov.l 0x8c655054,r14 */
L8c655006: *stop=0x8c655008u;goto L8c655024; /* bra 0x8c655024 */
L8c65500a: *stop=0x8c65500au;r[14]=rd(m,0x8c655058u,4);goto L8c65500c; /* mov.l 0x8c655058,r14 */
L8c65500c: *stop=0x8c65500eu;goto L8c655024; /* bra 0x8c655024 */
L8c655010: *stop=0x8c655010u;r[14]=rd(m,0x8c65505cu,4);goto L8c655012; /* mov.l 0x8c65505c,r14 */
L8c655012: *stop=0x8c655014u;goto L8c655024; /* bra 0x8c655024 */
L8c655016: *stop=0x8c655016u;r[14]=rd(m,0x8c655040u,4);goto L8c655018; /* mov.l 0x8c655040,r14 */
L8c655018: *stop=0x8c65501au;goto L8c655024; /* bra 0x8c655024 */
L8c65501c: *stop=0x8c65501cu;r[14]=rd(m,0x8c655048u,4);goto L8c65501e; /* mov.l 0x8c655048,r14 */
L8c65501e: *stop=0x8c655020u;goto L8c655024; /* bra 0x8c655024 */
L8c655022: *stop=0x8c655022u;r[14]=rd(m,0x8c655060u,4);goto L8c655024; /* mov.l 0x8c655060,r14 */
L8c655024: *stop=0x8c655026u;r[14]|=r[4];goto L8c655082; /* bra 0x8c655082 */
L8c655080: *stop=0x8c655080u;r[14]=r[4];goto L8c655082; /* mov r4,r14 */
L8c655082: *stop=0x8c655082u;r[3]=sx(m,0x8c6550a6u,2);goto L8c655084; /* mov.w 0x8c6550a6,r3 */
L8c655084: *stop=0x8c655084u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c655086; /* tst r12,r3 */
L8c655086: if((*sr&1u))goto L8c65510a;goto L8c655088; /* bt 0x8c65510a */
L8c655088: *stop=0x8c655088u;r[0]=r[14];goto L8c65508a; /* mov r14,r0 */
L8c65508a: *stop=0x8c65508au;r[0]&=r[9];goto L8c65508c; /* and r9,r0 */
L8c65508c: *stop=0x8c65508cu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[0],4);goto L8c65508e; /* mov.l r0,@r15 */
L8c65508e: *stop=0x8c65508eu;r[4]=r[14];goto L8c655090; /* mov r14,r4 */
L8c655090: *stop=0x8c655090u;if(!valid_ram(r[13]+48u,4u))return -3;r[0]=rd(m,r[13]+0x30u,4);goto L8c655092; /* mov.l @(0x30,r13),r0 */
L8c655092: *stop=0x8c655092u;r[4]&=r[5];goto L8c655094; /* and r5,r4 */
L8c655094: *stop=0x8c655094u;r[1]=12u;goto L8c655096; /* mov #0xc,r1 */
L8c655096: *stop=0x8c655096u;*sr=(*sr&~1u)|(r[0]>=r[1]);goto L8c655098; /* cmp/hs r1,r0 */
L8c655098: if((*sr&1u))goto L8c65510a;goto L8c65509a; /* bt 0x8c65510a */
L8c65509a: *stop=0x8c65509au;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c65509c; /* shll r0 */
L8c65509c: *stop=0x8c65509cu;r[1]=r[0];goto L8c65509e; /* mov r0,r1 */
L8c65509e: *stop=0x8c65509eu;r[0]=0x8c6550a8u;goto L8c6550a0; /* mova 0x8c6550a8,r0 */
L8c6550a0: *stop=0x8c6550a0u;if(!valid_ram(r[0]+r[1],2u))return -3;r[0]=sx(m,r[0]+r[1],2);goto L8c6550a2; /* mov.w @(r0,r1),r0 */
L8c6550a2: target=0x8c6550a6u+r[0];*stop=0x8c6550a4u;switch(target){case 0x8c6550c0u:goto L8c6550c0;case 0x8c6550ceu:goto L8c6550ce;case 0x8c6550deu:goto L8c6550de;case 0x8c6550e4u:goto L8c6550e4;case 0x8c6550eau:goto L8c6550ea;case 0x8c6550f0u:goto L8c6550f0;case 0x8c6550f6u:goto L8c6550f6;case 0x8c6550fcu:goto L8c6550fc;case 0x8c655102u:goto L8c655102;case 0x8c655108u:goto L8c655108;case 0x8c65510au:goto L8c65510a;default:*stop=0x8c6550a2u;return -3;} /* braf r0 */
L8c6550c0: *stop=0x8c6550c0u;if(!valid_ram(r[15],4u))return -3;r[4]=rd(m,r[15],4);goto L8c6550c2; /* mov.l @r15,r4 */
L8c6550c2: *stop=0x8c6550c2u;r[3]=rd(m,0x8c655140u,4);goto L8c6550c4; /* mov.l 0x8c655140,r3 */
L8c6550c4: *stop=0x8c6550c4u;r[2]=rd(m,0x8c655144u,4);goto L8c6550c6; /* mov.l 0x8c655144,r2 */
L8c6550c6: *stop=0x8c6550c6u;r[4]|=r[3];goto L8c6550c8; /* or r3,r4 */
L8c6550c8: *stop=0x8c6550c8u;r[14]=r[4];goto L8c6550ca; /* mov r4,r14 */
L8c6550ca: *stop=0x8c6550ccu;r[14]&=r[5];goto L8c6550da; /* bra 0x8c6550da */
L8c6550ce: *stop=0x8c6550ceu;if(!valid_ram(r[15],4u))return -3;r[4]=rd(m,r[15],4);goto L8c6550d0; /* mov.l @r15,r4 */
L8c6550d0: *stop=0x8c6550d0u;r[3]=rd(m,0x8c655148u,4);goto L8c6550d2; /* mov.l 0x8c655148,r3 */
L8c6550d2: *stop=0x8c6550d2u;r[2]=rd(m,0x8c65514cu,4);goto L8c6550d4; /* mov.l 0x8c65514c,r2 */
L8c6550d4: *stop=0x8c6550d4u;r[4]|=r[3];goto L8c6550d6; /* or r3,r4 */
L8c6550d6: *stop=0x8c6550d6u;r[14]=r[4];goto L8c6550d8; /* mov r4,r14 */
L8c6550d8: *stop=0x8c6550d8u;r[14]&=r[5];goto L8c6550da; /* and r5,r14 */
L8c6550da: *stop=0x8c6550dcu;r[14]|=r[2];goto L8c65510a; /* bra 0x8c65510a */
L8c6550de: *stop=0x8c6550deu;r[14]=rd(m,0x8c655150u,4);goto L8c6550e0; /* mov.l 0x8c655150,r14 */
L8c6550e0: *stop=0x8c6550e2u;goto L8c6550fe; /* bra 0x8c6550fe */
L8c6550e4: *stop=0x8c6550e4u;r[14]=rd(m,0x8c655154u,4);goto L8c6550e6; /* mov.l 0x8c655154,r14 */
L8c6550e6: *stop=0x8c6550e8u;goto L8c6550fe; /* bra 0x8c6550fe */
L8c6550ea: *stop=0x8c6550eau;r[14]=rd(m,0x8c65514cu,4);goto L8c6550ec; /* mov.l 0x8c65514c,r14 */
L8c6550ec: *stop=0x8c6550eeu;goto L8c6550fe; /* bra 0x8c6550fe */
L8c6550f0: *stop=0x8c6550f0u;r[14]=rd(m,0x8c655158u,4);goto L8c6550f2; /* mov.l 0x8c655158,r14 */
L8c6550f2: *stop=0x8c6550f4u;goto L8c6550fe; /* bra 0x8c6550fe */
L8c6550f6: *stop=0x8c6550f6u;r[14]=rd(m,0x8c655144u,4);goto L8c6550f8; /* mov.l 0x8c655144,r14 */
L8c6550f8: *stop=0x8c6550fau;goto L8c6550fe; /* bra 0x8c6550fe */
L8c6550fc: *stop=0x8c6550fcu;r[14]=rd(m,0x8c65515cu,4);goto L8c6550fe; /* mov.l 0x8c65515c,r14 */
L8c6550fe: *stop=0x8c655100u;r[14]|=r[4];goto L8c65510a; /* bra 0x8c65510a */
L8c655102: *stop=0x8c655102u;r[14]=r[4];goto L8c655104; /* mov r4,r14 */
L8c655104: *stop=0x8c655106u;r[14]|=r[7];goto L8c65510a; /* bra 0x8c65510a */
L8c655108: *stop=0x8c655108u;r[14]=r[4];goto L8c65510a; /* mov r4,r14 */
L8c65510a: *stop=0x8c65510au;r[4]=rd(m,0x8c655164u,4);goto L8c65510c; /* mov.l 0x8c655164,r4 */
L8c65510c: *stop=0x8c65510cu;r[3]=r[12];goto L8c65510e; /* mov r12,r3 */
L8c65510e: *stop=0x8c65510eu;r[5]=rd(m,0x8c655160u,4);goto L8c655110; /* mov.l 0x8c655160,r5 */
L8c655110: *stop=0x8c655110u;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c655112; /* tst r4,r3 */
L8c655112: if((*sr&1u))goto L8c655122;goto L8c655114; /* bt 0x8c655122 */
L8c655114: *stop=0x8c655114u;if(!valid_ram(r[13]+52u,4u))return -3;r[1]=rd(m,r[13]+0x34u,4);goto L8c655116; /* mov.l @(0x34,r13),r1 */
L8c655116: *stop=0x8c655116u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c655118; /* tst r1,r1 */
L8c655118: if((*sr&1u))goto L8c65511e;goto L8c65511a; /* bt 0x8c65511e */
L8c65511a: *stop=0x8c65511cu;r[14]|=r[5];goto L8c655122; /* bra 0x8c655122 */
L8c65511e: *stop=0x8c65511eu;r[2]=rd(m,0x8c655168u,4);goto L8c655120; /* mov.l 0x8c655168,r2 */
L8c655120: *stop=0x8c655120u;r[14]&=r[2];goto L8c655122; /* and r2,r14 */
L8c655122: *stop=0x8c655122u;*sr=(*sr&~1u)|!(r[12]&r[5]);goto L8c655124; /* tst r12,r5 */
L8c655124: if((*sr&1u))goto L8c655182;goto L8c655126; /* bt 0x8c655182 */
L8c655126: *stop=0x8c655126u;if(!valid_ram(r[13]+56u,4u))return -3;r[2]=rd(m,r[13]+0x38u,4);goto L8c655128; /* mov.l @(0x38,r13),r2 */
L8c655128: *stop=0x8c655128u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c65512a; /* tst r2,r2 */
L8c65512a: if((*sr&1u))goto L8c655180;goto L8c65512c; /* bt 0x8c655180 */
L8c65512c: *stop=0x8c65512eu;r[14]|=r[4];goto L8c655182; /* bra 0x8c655182 */
L8c655180: *stop=0x8c655180u;r[14]&=r[6];goto L8c655182; /* and r6,r14 */
L8c655182: *stop=0x8c655182u;r[2]=sx(m,0x8c6551e0u,2);goto L8c655184; /* mov.w 0x8c6551e0,r2 */
L8c655184: *stop=0x8c655184u;*sr=(*sr&~1u)|!(r[12]&r[2]);goto L8c655186; /* tst r12,r2 */
L8c655186: take=(*sr&1u);*stop=0x8c655188u;r[5]=3u;if(take)goto L8c65519a;goto L8c65518a; /* bt/s 0x8c65519a */
L8c65518a: *stop=0x8c65518au;r[2]=rd(m,0x8c6551e8u,4);goto L8c65518c; /* mov.l 0x8c6551e8,r2 */
L8c65518c: *stop=0x8c65518cu;r[3]=22u;goto L8c65518e; /* mov #0x16,r3 */
L8c65518e: *stop=0x8c65518eu;if(!valid_ram(r[13]+60u,4u))return -3;r[4]=rd(m,r[13]+0x3cu,4);goto L8c655190; /* mov.l @(0x3c,r13),r4 */
L8c655190: *stop=0x8c655190u;r[2]&=r[14];goto L8c655192; /* and r14,r2 */
L8c655192: *stop=0x8c655192u;r[4]&=r[5];goto L8c655194; /* and r5,r4 */
L8c655194: *stop=0x8c655194u;r[14]=r[2];goto L8c655196; /* mov r2,r14 */
L8c655196: *stop=0x8c655196u;r[4]=(r[3]&0x80000000u)?((r[3]&31u)?r[4]>>((0u-r[3])&31u):0u):(r[4]<<(r[3]&31u));goto L8c655198; /* shld r3,r4 */
L8c655198: *stop=0x8c655198u;r[14]|=r[4];goto L8c65519a; /* or r4,r14 */
L8c65519a: *stop=0x8c65519au;r[2]=rd(m,0x8c6551ecu,4);goto L8c65519c; /* mov.l 0x8c6551ec,r2 */
L8c65519c: *stop=0x8c65519cu;*sr=(*sr&~1u)|!(r[12]&r[2]);goto L8c65519e; /* tst r12,r2 */
L8c65519e: if((*sr&1u))goto L8c6551b2;goto L8c6551a0; /* bt 0x8c6551b2 */
L8c6551a0: *stop=0x8c6551a0u;r[0]=100u;goto L8c6551a2; /* mov #0x64,r0 */
L8c6551a2: *stop=0x8c6551a2u;if(!valid_ram(r[0]+r[13],4u))return -3;r[1]=rd(m,r[13]+r[0],4);goto L8c6551a4; /* mov.l @(r0,r13),r1 */
L8c6551a4: *stop=0x8c6551a4u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c6551a6; /* tst r1,r1 */
L8c6551a6: if((*sr&1u))goto L8c6551ae;goto L8c6551a8; /* bt 0x8c6551ae */
L8c6551a8: *stop=0x8c6551a8u;r[3]=rd(m,0x8c6551f0u,4);goto L8c6551aa; /* mov.l 0x8c6551f0,r3 */
L8c6551aa: *stop=0x8c6551acu;r[14]|=r[3];goto L8c6551b2; /* bra 0x8c6551b2 */
L8c6551ae: *stop=0x8c6551aeu;r[1]=rd(m,0x8c6551f4u,4);goto L8c6551b0; /* mov.l 0x8c6551f4,r1 */
L8c6551b0: *stop=0x8c6551b0u;r[14]&=r[1];goto L8c6551b2; /* and r1,r14 */
L8c6551b2: *stop=0x8c6551b2u;r[3]=sx(m,0x8c6551e2u,2);goto L8c6551b4; /* mov.w 0x8c6551e2,r3 */
L8c6551b4: *stop=0x8c6551b4u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c6551b6; /* tst r12,r3 */
L8c6551b6: if((*sr&1u))goto L8c6551ca;goto L8c6551b8; /* bt 0x8c6551ca */
L8c6551b8: *stop=0x8c6551b8u;r[0]=68u;goto L8c6551ba; /* mov #0x44,r0 */
L8c6551ba: *stop=0x8c6551bau;if(!valid_ram(r[0]+r[13],4u))return -3;r[1]=rd(m,r[13]+r[0],4);goto L8c6551bc; /* mov.l @(r0,r13),r1 */
L8c6551bc: *stop=0x8c6551bcu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c6551be; /* tst r1,r1 */
L8c6551be: if((*sr&1u))goto L8c6551c6;goto L8c6551c0; /* bt 0x8c6551c6 */
L8c6551c0: *stop=0x8c6551c0u;r[2]=rd(m,0x8c6551f8u,4);goto L8c6551c2; /* mov.l 0x8c6551f8,r2 */
L8c6551c2: *stop=0x8c6551c4u;r[14]|=r[2];goto L8c6551ca; /* bra 0x8c6551ca */
L8c6551c6: *stop=0x8c6551c6u;r[1]=rd(m,0x8c6551fcu,4);goto L8c6551c8; /* mov.l 0x8c6551fc,r1 */
L8c6551c8: *stop=0x8c6551c8u;r[14]&=r[1];goto L8c6551ca; /* and r1,r14 */
L8c6551ca: *stop=0x8c6551cau;r[3]=sx(m,0x8c6551e4u,2);goto L8c6551cc; /* mov.w 0x8c6551e4,r3 */
L8c6551cc: *stop=0x8c6551ccu;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c6551ce; /* tst r12,r3 */
L8c6551ce: if((*sr&1u))goto L8c655224;goto L8c6551d0; /* bt 0x8c655224 */
L8c6551d0: *stop=0x8c6551d0u;r[0]=72u;goto L8c6551d2; /* mov #0x48,r0 */
L8c6551d2: *stop=0x8c6551d2u;if(!valid_ram(r[0]+r[13],4u))return -3;r[1]=rd(m,r[13]+r[0],4);goto L8c6551d4; /* mov.l @(r0,r13),r1 */
L8c6551d4: *stop=0x8c6551d4u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c6551d6; /* tst r1,r1 */
L8c6551d6: if((*sr&1u))goto L8c655220;goto L8c6551d8; /* bt 0x8c655220 */
L8c6551d8: *stop=0x8c6551d8u;r[2]=rd(m,0x8c655200u,4);goto L8c6551da; /* mov.l 0x8c655200,r2 */
L8c6551da: *stop=0x8c6551dcu;r[14]|=r[2];goto L8c655224; /* bra 0x8c655224 */
L8c655220: *stop=0x8c655220u;r[1]=rd(m,0x8c6552a8u,4);goto L8c655222; /* mov.l 0x8c6552a8,r1 */
L8c655222: *stop=0x8c655222u;r[14]&=r[1];goto L8c655224; /* and r1,r14 */
L8c655224: *stop=0x8c655224u;r[6]=sx(m,0x8c6552a0u,2);goto L8c655226; /* mov.w 0x8c6552a0,r6 */
L8c655226: *stop=0x8c655226u;r[3]=r[12];goto L8c655228; /* mov r12,r3 */
L8c655228: *stop=0x8c655228u;*sr=(*sr&~1u)|!(r[6]&r[3]);goto L8c65522a; /* tst r6,r3 */
L8c65522a: if((*sr&1u))goto L8c65523c;goto L8c65522c; /* bt 0x8c65523c */
L8c65522c: *stop=0x8c65522cu;r[0]=76u;goto L8c65522e; /* mov #0x4c,r0 */
L8c65522e: *stop=0x8c65522eu;r[1]=rd(m,0x8c6552acu,4);goto L8c655230; /* mov.l 0x8c6552ac,r1 */
L8c655230: *stop=0x8c655230u;if(!valid_ram(r[0]+r[13],4u))return -3;r[2]=rd(m,r[13]+r[0],4);goto L8c655232; /* mov.l @(r0,r13),r2 */
L8c655232: *stop=0x8c655232u;r[3]=15u;goto L8c655234; /* mov #0xf,r3 */
L8c655234: *stop=0x8c655234u;r[1]&=r[14];goto L8c655236; /* and r14,r1 */
L8c655236: *stop=0x8c655236u;r[14]=r[1];goto L8c655238; /* mov r1,r14 */
L8c655238: *stop=0x8c655238u;r[2]=(r[3]&0x80000000u)?((r[3]&31u)?r[2]>>((0u-r[3])&31u):0u):(r[2]<<(r[3]&31u));goto L8c65523a; /* shld r3,r2 */
L8c65523a: *stop=0x8c65523au;r[14]|=r[2];goto L8c65523c; /* or r2,r14 */
L8c65523c: *stop=0x8c65523cu;r[3]=sx(m,0x8c6552a2u,2);goto L8c65523e; /* mov.w 0x8c6552a2,r3 */
L8c65523e: *stop=0x8c65523eu;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c655240; /* tst r12,r3 */
L8c655240: if((*sr&1u))goto L8c655252;goto L8c655242; /* bt 0x8c655252 */
L8c655242: *stop=0x8c655242u;r[0]=80u;goto L8c655244; /* mov #0x50,r0 */
L8c655244: *stop=0x8c655244u;r[1]=rd(m,0x8c6552b0u,4);goto L8c655246; /* mov.l 0x8c6552b0,r1 */
L8c655246: *stop=0x8c655246u;if(!valid_ram(r[0]+r[13],4u))return -3;r[3]=rd(m,r[13]+r[0],4);goto L8c655248; /* mov.l @(r0,r13),r3 */
L8c655248: *stop=0x8c655248u;r[1]&=r[14];goto L8c65524a; /* and r14,r1 */
L8c65524a: *stop=0x8c65524au;r[3]<<=16;goto L8c65524c; /* shll16 r3 */
L8c65524c: *stop=0x8c65524cu;r[14]=r[1];goto L8c65524e; /* mov r1,r14 */
L8c65524e: *stop=0x8c65524eu;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c655250; /* shll r3 */
L8c655250: *stop=0x8c655250u;r[14]|=r[3];goto L8c655252; /* or r3,r14 */
L8c655252: *stop=0x8c655252u;r[3]=sx(m,0x8c6552a4u,2);goto L8c655254; /* mov.w 0x8c6552a4,r3 */
L8c655254: *stop=0x8c655254u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c655256; /* tst r12,r3 */
L8c655256: if((*sr&1u))goto L8c655272;goto L8c655258; /* bt 0x8c655272 */
L8c655258: *stop=0x8c655258u;r[0]=84u;goto L8c65525a; /* mov #0x54,r0 */
L8c65525a: *stop=0x8c65525au;r[3]=r[15];goto L8c65525c; /* mov r15,r3 */
L8c65525c: *stop=0x8c65525cu;if(!valid_ram(r[0]+r[13],4u))return -3;r[4]=rd(m,r[13]+r[0],4);goto L8c65525e; /* mov.l @(r0,r13),r4 */
L8c65525e: *stop=0x8c65525eu;r[3]+=20u;goto L8c655260; /* add #0x14,r3 */
L8c655260: *stop=0x8c655260u;r[2]=sx(m,0x8c6552a6u,2);goto L8c655262; /* mov.w 0x8c6552a6,r2 */
L8c655262: *stop=0x8c655262u;r[1]=13u;goto L8c655264; /* mov #0xd,r1 */
L8c655264: *stop=0x8c655264u;r[4]<<=2;goto L8c655266; /* shll2 r4 */
L8c655266: *stop=0x8c655266u;r[4]+=r[3];goto L8c655268; /* add r3,r4 */
L8c655268: *stop=0x8c655268u;if(!valid_ram(r[4],4u))return -3;r[4]=rd(m,r[4],4);goto L8c65526a; /* mov.l @r4,r4 */
L8c65526a: *stop=0x8c65526au;r[2]&=r[14];goto L8c65526c; /* and r14,r2 */
L8c65526c: *stop=0x8c65526cu;r[14]=r[2];goto L8c65526e; /* mov r2,r14 */
L8c65526e: *stop=0x8c65526eu;r[4]=(r[1]&0x80000000u)?((r[1]&31u)?r[4]>>((0u-r[1])&31u):0u):(r[4]<<(r[1]&31u));goto L8c655270; /* shld r1,r4 */
L8c655270: *stop=0x8c655270u;r[14]|=r[4];goto L8c655272; /* or r4,r14 */
L8c655272: *stop=0x8c655272u;r[3]=rd(m,0x8c6552b4u,4);goto L8c655274; /* mov.l 0x8c6552b4,r3 */
L8c655274: *stop=0x8c655274u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c655276; /* tst r12,r3 */
L8c655276: if((*sr&1u))goto L8c6552c4;goto L8c655278; /* bt 0x8c6552c4 */
L8c655278: *stop=0x8c655278u;r[0]=88u;goto L8c65527a; /* mov #0x58,r0 */
L8c65527a: *stop=0x8c65527au;if(!valid_ram(r[0]+r[13],4u))return -3;r[1]=rd(m,r[13]+r[0],4);goto L8c65527c; /* mov.l @(r0,r13),r1 */
L8c65527c: *stop=0x8c65527cu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c65527e; /* tst r1,r1 */
L8c65527e: if((*sr&1u))goto L8c6552c0;goto L8c655280; /* bt 0x8c6552c0 */
L8c655280: *stop=0x8c655282u;r[14]|=r[6];goto L8c6552c4; /* bra 0x8c6552c4 */
L8c6552c0: *stop=0x8c6552c0u;r[2]=sx(m,0x8c6553a0u,2);goto L8c6552c2; /* mov.w 0x8c6553a0,r2 */
L8c6552c2: *stop=0x8c6552c2u;r[14]&=r[2];goto L8c6552c4; /* and r2,r14 */
L8c6552c4: *stop=0x8c6552c4u;r[3]=rd(m,0x8c6553a8u,4);goto L8c6552c6; /* mov.l 0x8c6553a8,r3 */
L8c6552c6: *stop=0x8c6552c6u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c6552c8; /* tst r12,r3 */
L8c6552c8: if((*sr&1u))goto L8c6552e2;goto L8c6552ca; /* bt 0x8c6552e2 */
L8c6552ca: *stop=0x8c6552cau;r[0]=92u;goto L8c6552cc; /* mov #0x5c,r0 */
L8c6552cc: *stop=0x8c6552ccu;if(!valid_ram(r[0]+r[13],4u))return -3;r[0]=rd(m,r[13]+r[0],4);goto L8c6552ce; /* mov.l @(r0,r13),r0 */
L8c6552ce: *stop=0x8c6552ceu;r[0]&=0xfu;goto L8c6552d0; /* and #0xf,r0 */
L8c6552d0: *stop=0x8c6552d0u;r[4]=r[0];goto L8c6552d2; /* mov r0,r4 */
L8c6552d2: *stop=0x8c6552d2u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6552d4; /* tst r4,r4 */
L8c6552d4: if(!(*sr&1u))goto L8c6552d8;goto L8c6552d6; /* bf 0x8c6552d8 */
L8c6552d6: *stop=0x8c6552d6u;r[4]=4u;goto L8c6552d8; /* mov #0x4,r4 */
L8c6552d8: *stop=0x8c6552d8u;r[2]=sx(m,0x8c6553a2u,2);goto L8c6552da; /* mov.w 0x8c6553a2,r2 */
L8c6552da: *stop=0x8c6552dau;r[4]<<=8;goto L8c6552dc; /* shll8 r4 */
L8c6552dc: *stop=0x8c6552dcu;r[2]&=r[14];goto L8c6552de; /* and r14,r2 */
L8c6552de: *stop=0x8c6552deu;r[14]=r[2];goto L8c6552e0; /* mov r2,r14 */
L8c6552e0: *stop=0x8c6552e0u;r[14]|=r[4];goto L8c6552e2; /* or r4,r14 */
L8c6552e2: *stop=0x8c6552e2u;r[3]=rd(m,0x8c6553acu,4);goto L8c6552e4; /* mov.l 0x8c6553ac,r3 */
L8c6552e4: *stop=0x8c6552e4u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c6552e6; /* tst r12,r3 */
L8c6552e6: if((*sr&1u))goto L8c6552fc;goto L8c6552e8; /* bt 0x8c6552fc */
L8c6552e8: *stop=0x8c6552e8u;r[0]=96u;goto L8c6552ea; /* mov #0x60,r0 */
L8c6552ea: *stop=0x8c6552eau;r[3]=sx(m,0x8c6553a4u,2);goto L8c6552ec; /* mov.w 0x8c6553a4,r3 */
L8c6552ec: *stop=0x8c6552ecu;if(!valid_ram(r[0]+r[13],4u))return -3;r[4]=rd(m,r[13]+r[0],4);goto L8c6552ee; /* mov.l @(r0,r13),r4 */
L8c6552ee: *stop=0x8c6552eeu;r[3]&=r[14];goto L8c6552f0; /* and r14,r3 */
L8c6552f0: *stop=0x8c6552f0u;r[4]&=r[5];goto L8c6552f2; /* and r5,r4 */
L8c6552f2: *stop=0x8c6552f2u;r[14]=r[3];goto L8c6552f4; /* mov r3,r14 */
L8c6552f4: *stop=0x8c6552f4u;r[4]<<=2;goto L8c6552f6; /* shll2 r4 */
L8c6552f6: *stop=0x8c6552f6u;r[4]<<=2;goto L8c6552f8; /* shll2 r4 */
L8c6552f8: *stop=0x8c6552f8u;r[4]<<=2;goto L8c6552fa; /* shll2 r4 */
L8c6552fa: *stop=0x8c6552fau;r[14]|=r[4];goto L8c6552fc; /* or r4,r14 */
L8c6552fc: *stop=0x8c6552fcu;if(!valid_ram(r[13]+32u,4u))return -3;r[0]=rd(m,r[13]+0x20u,4);goto L8c6552fe; /* mov.l @(0x20,r13),r0 */
L8c6552fe: *stop=0x8c6552feu;*sr=(*sr&~1u)|(r[0]&1u);r[0]=(r[0]>>1)|(r[0]&0x80000000u);goto L8c655300; /* shar r0 */
L8c655300: *stop=0x8c655300u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c655302; /* cmp/eq #0x1,r0 */
L8c655302: if(!(*sr&1u))goto L8c65532c;goto L8c655304; /* bf 0x8c65532c */
L8c655304: *stop=0x8c655304u;r[0]=108u;goto L8c655306; /* mov #0x6c,r0 */
L8c655306: *stop=0x8c655306u;if(!valid_ram(r[0]+r[13],4u))return -3;r[3]=rd(m,r[13]+r[0],4);goto L8c655308; /* mov.l @(r0,r13),r3 */
L8c655308: *stop=0x8c655308u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c65530a; /* tst r3,r3 */
L8c65530a: if((*sr&1u))goto L8c65532c;goto L8c65530c; /* bt 0x8c65532c */
L8c65530c: *stop=0x8c65530cu;if(!valid_ram(r[0]+r[13],4u))return -3;r[4]=rd(m,r[13]+r[0],4);goto L8c65530e; /* mov.l @(r0,r13),r4 */
L8c65530e: *stop=0x8c65530eu;r[3]=4294967239u;goto L8c655310; /* mov #-0x39,r3 */
L8c655310: *stop=0x8c655310u;r[14]&=r[3];goto L8c655312; /* and r3,r14 */
L8c655312: target=0x8c654b60u;*pr=0x8c655316u;*stop=0x8c655314u;if(!valid_ram(r[4]+12u,4u))return -3;r[4]=rd(m,r[4]+0xcu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c655316; /* bsr 0x8c654b60 */
L8c655316: *stop=0x8c655316u;r[0]<<=2;goto L8c655318; /* shll2 r0 */
L8c655318: *stop=0x8c655318u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c65531a; /* shll r0 */
L8c65531a: *stop=0x8c65531au;r[14]|=r[0];goto L8c65531c; /* or r0,r14 */
L8c65531c: *stop=0x8c65531cu;r[0]=108u;goto L8c65531e; /* mov #0x6c,r0 */
L8c65531e: *stop=0x8c65531eu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[14],4);goto L8c655320; /* mov.l r14,@r15 */
L8c655320: *stop=0x8c655320u;if(!valid_ram(r[0]+r[13],4u))return -3;r[4]=rd(m,r[13]+r[0],4);goto L8c655322; /* mov.l @(r0,r13),r4 */
L8c655322: *stop=0x8c655322u;r[3]=4294967288u;goto L8c655324; /* mov #-0x8,r3 */
L8c655324: *stop=0x8c655324u;r[14]&=r[3];goto L8c655326; /* and r3,r14 */
L8c655326: target=0x8c654b60u;*pr=0x8c65532au;*stop=0x8c655328u;if(!valid_ram(r[4]+16u,4u))return -3;r[4]=rd(m,r[4]+0x10u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65532a; /* bsr 0x8c654b60 */
L8c65532a: *stop=0x8c65532au;r[14]|=r[0];goto L8c65532c; /* or r0,r14 */
L8c65532c: *stop=0x8c65532cu;if(!valid_ram(r[13]+32u,4u))return -3;r[0]=rd(m,r[13]+0x20u,4);goto L8c65532e; /* mov.l @(0x20,r13),r0 */
L8c65532e: *stop=0x8c65532eu;*sr=(*sr&~1u)|(r[0]&1u);r[0]=(r[0]>>1)|(r[0]&0x80000000u);goto L8c655330; /* shar r0 */
L8c655330: *stop=0x8c655330u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c655332; /* cmp/eq #0x1,r0 */
L8c655332: take=(*sr&1u);*stop=0x8c655334u;r[5]=16u;if(take)goto L8c65533a;goto L8c655336; /* bt/s 0x8c65533a */
L8c655336: *stop=0x8c655338u;goto L8c655418; /* bra 0x8c655418 */
L8c65533a: *stop=0x8c65533au;r[0]=108u;goto L8c65533c; /* mov #0x6c,r0 */
L8c65533c: *stop=0x8c65533cu;if(!valid_ram(r[0]+r[13],4u))return -3;r[3]=rd(m,r[13]+r[0],4);goto L8c65533e; /* mov.l @(r0,r13),r3 */
L8c65533e: *stop=0x8c65533eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c655340; /* tst r3,r3 */
L8c655340: if((*sr&1u))goto L8c655418;goto L8c655342; /* bt 0x8c655418 */
L8c655342: *stop=0x8c655342u;r[0]=108u;goto L8c655344; /* mov #0x6c,r0 */
L8c655344: *stop=0x8c655344u;r[3]=rd(m,0x8c6553b0u,4);goto L8c655346; /* mov.l 0x8c6553b0,r3 */
L8c655346: *stop=0x8c655346u;if(!valid_ram(r[0]+r[13],4u))return -3;r[4]=rd(m,r[13]+r[0],4);goto L8c655348; /* mov.l @(r0,r13),r4 */
L8c655348: *stop=0x8c655348u;r[0]=1u;goto L8c65534a; /* mov #0x1,r0 */
L8c65534a: *stop=0x8c65534au;if(!valid_ram(r[15]+8u,4u))return -3;r[7]=rd(m,r[15]+0x8u,4);goto L8c65534c; /* mov.l @(0x8,r15),r7 */
L8c65534c: *stop=0x8c65534cu;r[1]=8u;goto L8c65534e; /* mov #0x8,r1 */
L8c65534e: *stop=0x8c65534eu;if(!valid_ram(r[4]+24u,4u))return -3;r[6]=rd(m,r[4]+0x18u,4);goto L8c655350; /* mov.l @(0x18,r4),r6 */
L8c655350: *stop=0x8c655350u;r[2]=rd(m,0x8c6553b4u,4);goto L8c655352; /* mov.l 0x8c6553b4,r2 */
L8c655352: *stop=0x8c655352u;r[7]&=r[3];goto L8c655354; /* and r3,r7 */
L8c655354: *stop=0x8c655354u;r[0]&=r[6];goto L8c655356; /* and r6,r0 */
L8c655356: *stop=0x8c655356u;r[0]&=0x1u;goto L8c655358; /* and #0x1,r0 */
L8c655358: *stop=0x8c655358u;*sr=(*sr&~1u)|(r[0]&1u);r[0]=(r[0]>>1)|(r[0]<<31);goto L8c65535a; /* rotr r0 */
L8c65535a: *stop=0x8c65535au;r[6]&=r[1];goto L8c65535c; /* and r1,r6 */
L8c65535c: *stop=0x8c65535cu;r[3]=27u;goto L8c65535e; /* mov #0x1b,r3 */
L8c65535e: *stop=0x8c65535eu;r[7]|=r[0];goto L8c655360; /* or r0,r7 */
L8c655360: *stop=0x8c655360u;r[0]=rd(m,0x8c6553c0u,4);goto L8c655362; /* mov.l 0x8c6553c0,r0 */
L8c655362: *stop=0x8c655362u;r[6]=(r[3]&0x80000000u)?((r[3]&31u)?r[6]>>((0u-r[3])&31u):0u):(r[6]<<(r[3]&31u));goto L8c655364; /* shld r3,r6 */
L8c655364: *stop=0x8c655364u;if(!valid_ram(r[4]+8u,4u))return -3;r[3]=rd(m,r[4]+0x8u,4);goto L8c655366; /* mov.l @(0x8,r4),r3 */
L8c655366: *stop=0x8c655366u;r[7]&=r[2];goto L8c655368; /* and r2,r7 */
L8c655368: *stop=0x8c655368u;r[2]=rd(m,0x8c6553bcu,4);goto L8c65536a; /* mov.l 0x8c6553bc,r2 */
L8c65536a: *stop=0x8c65536au;r[7]|=r[6];goto L8c65536c; /* or r6,r7 */
L8c65536c: *stop=0x8c65536cu;r[6]=rd(m,0x8c6553b8u,4);goto L8c65536e; /* mov.l 0x8c6553b8,r6 */
L8c65536e: *stop=0x8c65536eu;r[7]&=r[2];goto L8c655370; /* and r2,r7 */
L8c655370: *stop=0x8c655370u;r[4]=r[7];goto L8c655372; /* mov r7,r4 */
L8c655372: *stop=0x8c655372u;r[3]&=r[6];goto L8c655374; /* and r6,r3 */
L8c655374: *stop=0x8c655374u;r[4]|=r[3];goto L8c655376; /* or r3,r4 */
L8c655376: *stop=0x8c655376u;r[7]=r[4];goto L8c655378; /* mov r4,r7 */
L8c655378: *stop=0x8c655378u;r[7]&=r[6];goto L8c65537a; /* and r6,r7 */
L8c65537a: *stop=0x8c65537au;*sr=(*sr&~1u)|(r[0]==r[7]);goto L8c65537c; /* cmp/eq r0,r7 */
L8c65537c: if((*sr&1u))goto L8c655384;goto L8c65537e; /* bt 0x8c655384 */
L8c65537e: *stop=0x8c65537eu;r[1]=rd(m,0x8c6553c4u,4);goto L8c655380; /* mov.l 0x8c6553c4,r1 */
L8c655380: *stop=0x8c655380u;*sr=(*sr&~1u)|(r[1]==r[7]);goto L8c655382; /* cmp/eq r1,r7 */
L8c655382: if(!(*sr&1u))goto L8c6553e0;goto L8c655384; /* bf 0x8c6553e0 */
L8c655384: *stop=0x8c655384u;r[0]=104u;goto L8c655386; /* mov #0x68,r0 */
L8c655386: *stop=0x8c655386u;r[6]=rd(m,0x8c6553c8u,4);goto L8c655388; /* mov.l 0x8c6553c8,r6 */
L8c655388: *stop=0x8c655388u;if(!valid_ram(r[0]+r[13],4u))return -3;r[0]=rd(m,r[13]+r[0],4);goto L8c65538a; /* mov.l @(r0,r13),r0 */
L8c65538a: *stop=0x8c65538au;r[3]=21u;goto L8c65538c; /* mov #0x15,r3 */
L8c65538c: *stop=0x8c65538cu;r[6]&=r[4];goto L8c65538e; /* and r4,r6 */
L8c65538e: *stop=0x8c65538eu;r[0]&=0x3fu;goto L8c655390; /* and #0x3f,r0 */
L8c655390: *stop=0x8c655390u;r[0]=(r[3]&0x80000000u)?((r[3]&31u)?r[0]>>((0u-r[3])&31u):0u):(r[0]<<(r[3]&31u));goto L8c655392; /* shld r3,r0 */
L8c655392: *stop=0x8c655394u;r[6]|=r[0];goto L8c655406; /* bra 0x8c655406 */
L8c6553e0: *stop=0x8c6553e0u;r[0]=108u;goto L8c6553e2; /* mov #0x6c,r0 */
L8c6553e2: *stop=0x8c6553e2u;r[3]=rd(m,0x8c655484u,4);goto L8c6553e4; /* mov.l 0x8c655484,r3 */
L8c6553e4: *stop=0x8c6553e4u;if(!valid_ram(r[0]+r[13],4u))return -3;r[7]=rd(m,r[13]+r[0],4);goto L8c6553e6; /* mov.l @(r0,r13),r7 */
L8c6553e6: *stop=0x8c6553e6u;r[0]=4u;goto L8c6553e8; /* mov #0x4,r0 */
L8c6553e8: *stop=0x8c6553e8u;r[6]=rd(m,0x8c655488u,4);goto L8c6553ea; /* mov.l 0x8c655488,r6 */
L8c6553ea: *stop=0x8c6553eau;r[4]&=r[3];goto L8c6553ec; /* and r3,r4 */
L8c6553ec: *stop=0x8c6553ecu;if(!valid_ram(r[15]+16u,4u))return -3;wr(m,r[15]+0x10u,r[7],4);goto L8c6553ee; /* mov.l r7,@(0x10,r15) */
L8c6553ee: *stop=0x8c6553eeu;r[2]=21u;goto L8c6553f0; /* mov #0x15,r2 */
L8c6553f0: *stop=0x8c6553f0u;if(!valid_ram(r[7]+24u,4u))return -3;r[7]=rd(m,r[7]+0x18u,4);goto L8c6553f2; /* mov.l @(0x18,r7),r7 */
L8c6553f2: *stop=0x8c6553f2u;r[0]&=r[7];goto L8c6553f4; /* and r7,r0 */
L8c6553f4: *stop=0x8c6553f4u;r[0]=~r[0];goto L8c6553f6; /* not r0,r0 */
L8c6553f6: *stop=0x8c6553f6u;r[0]&=0x4u;goto L8c6553f8; /* and #0x4,r0 */
L8c6553f8: *stop=0x8c6553f8u;r[0]<<=16;goto L8c6553fa; /* shll16 r0 */
L8c6553fa: *stop=0x8c6553fau;r[0]<<=8;goto L8c6553fc; /* shll8 r0 */
L8c6553fc: *stop=0x8c6553fcu;r[7]&=r[5];goto L8c6553fe; /* and r5,r7 */
L8c6553fe: *stop=0x8c6553feu;r[4]|=r[0];goto L8c655400; /* or r0,r4 */
L8c655400: *stop=0x8c655400u;r[7]=(r[2]&0x80000000u)?((r[2]&31u)?r[7]>>((0u-r[2])&31u):0u):(r[7]<<(r[2]&31u));goto L8c655402; /* shld r2,r7 */
L8c655402: *stop=0x8c655402u;r[6]&=r[4];goto L8c655404; /* and r4,r6 */
L8c655404: *stop=0x8c655404u;r[6]|=r[7];goto L8c655406; /* or r7,r6 */
L8c655406: *stop=0x8c655406u;r[0]=108u;goto L8c655408; /* mov #0x6c,r0 */
L8c655408: *stop=0x8c655408u;r[3]=rd(m,0x8c65548cu,4);goto L8c65540a; /* mov.l 0x8c65548c,r3 */
L8c65540a: *stop=0x8c65540au;if(!valid_ram(r[0]+r[13],4u))return -3;r[2]=rd(m,r[13]+r[0],4);goto L8c65540c; /* mov.l @(r0,r13),r2 */
L8c65540c: *stop=0x8c65540cu;r[6]&=r[3];goto L8c65540e; /* and r3,r6 */
L8c65540e: *stop=0x8c65540eu;if(!valid_ram(r[2]+28u,4u))return -3;r[1]=rd(m,r[2]+0x1cu,4);goto L8c655410; /* mov.l @(0x1c,r2),r1 */
L8c655410: *stop=0x8c655410u;r[1]>>=2;goto L8c655412; /* shlr2 r1 */
L8c655412: *stop=0x8c655412u;*sr=(*sr&~1u)|(r[1]&1u);r[1]>>=1;goto L8c655414; /* shlr r1 */
L8c655414: *stop=0x8c655414u;r[6]|=r[1];goto L8c655416; /* or r1,r6 */
L8c655416: *stop=0x8c655416u;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[6],4);goto L8c655418; /* mov.l r6,@(0x8,r15) */
L8c655418: *stop=0x8c655418u;r[3]=rd(m,0x8c655490u,4);goto L8c65541a; /* mov.l 0x8c655490,r3 */
L8c65541a: *stop=0x8c65541au;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c65541c; /* tst r12,r3 */
L8c65541c: if((*sr&1u))goto L8c655446;goto L8c65541e; /* bt 0x8c655446 */
L8c65541e: *stop=0x8c65541eu;if(!valid_ram(r[13]+4u,4u))return -3;r[4]=rd(m,r[13]+0x4u,4);goto L8c655420; /* mov.l @(0x4,r13),r4 */
L8c655420: *stop=0x8c655420u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c655422; /* tst r4,r4 */
L8c655422: take=!(*sr&1u);*stop=0x8c655424u;r[0]=r[4];if(take)goto L8c65542e;goto L8c655426; /* bf/s 0x8c65542e */
L8c655426: *stop=0x8c655426u;r[2]=r[10];goto L8c655428; /* mov r10,r2 */
L8c655428: *stop=0x8c655428u;r[10]=rd(m,0x8c655494u,4);goto L8c65542a; /* mov.l 0x8c655494,r10 */
L8c65542a: *stop=0x8c65542au;r[2]&=r[9];goto L8c65542c; /* and r9,r2 */
L8c65542c: *stop=0x8c65542cu;r[10]|=r[2];goto L8c65542e; /* or r2,r10 */
L8c65542e: *stop=0x8c65542eu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c655430; /* cmp/eq #0x1,r0 */
L8c655430: take=!(*sr&1u);*stop=0x8c655432u;r[0]=r[4];if(take)goto L8c65543c;goto L8c655434; /* bf/s 0x8c65543c */
L8c655434: *stop=0x8c655434u;r[2]=r[10];goto L8c655436; /* mov r10,r2 */
L8c655436: *stop=0x8c655436u;r[10]=rd(m,0x8c655494u,4);goto L8c655438; /* mov.l 0x8c655494,r10 */
L8c655438: *stop=0x8c655438u;r[2]&=r[9];goto L8c65543a; /* and r9,r2 */
L8c65543a: *stop=0x8c65543au;r[10]|=r[2];goto L8c65543c; /* or r2,r10 */
L8c65543c: *stop=0x8c65543cu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c65543e; /* cmp/eq #0x2,r0 */
L8c65543e: if(!(*sr&1u))goto L8c655446;goto L8c655440; /* bf 0x8c655446 */
L8c655440: *stop=0x8c655440u;r[9]&=r[10];goto L8c655442; /* and r10,r9 */
L8c655442: *stop=0x8c655442u;r[10]=rd(m,0x8c655498u,4);goto L8c655444; /* mov.l 0x8c655498,r10 */
L8c655444: *stop=0x8c655444u;r[10]|=r[9];goto L8c655446; /* or r9,r10 */
L8c655446: *stop=0x8c655446u;r[2]=rd(m,0x8c65549cu,4);goto L8c655448; /* mov.l 0x8c65549c,r2 */
L8c655448: *stop=0x8c655448u;*sr=(*sr&~1u)|!(r[12]&r[2]);goto L8c65544a; /* tst r12,r2 */
L8c65544a: if((*sr&1u))goto L8c65545a;goto L8c65544c; /* bt 0x8c65545a */
L8c65544c: *stop=0x8c65544cu;if(!valid_ram(r[13]+8u,4u))return -3;r[3]=rd(m,r[13]+0x8u,4);goto L8c65544e; /* mov.l @(0x8,r13),r3 */
L8c65544e: *stop=0x8c65544eu;r[1]=rd(m,0x8c6554a0u,4);goto L8c655450; /* mov.l 0x8c6554a0,r1 */
L8c655450: *stop=0x8c655450u;r[3]<<=16;goto L8c655452; /* shll16 r3 */
L8c655452: *stop=0x8c655452u;r[1]&=r[10];goto L8c655454; /* and r10,r1 */
L8c655454: *stop=0x8c655454u;r[10]=r[1];goto L8c655456; /* mov r1,r10 */
L8c655456: *stop=0x8c655456u;r[3]<<=8;goto L8c655458; /* shll8 r3 */
L8c655458: *stop=0x8c655458u;r[10]|=r[3];goto L8c65545a; /* or r3,r10 */
L8c65545a: *stop=0x8c65545au;r[3]=rd(m,0x8c6554acu,4);goto L8c65545c; /* mov.l 0x8c6554ac,r3 */
L8c65545c: *stop=0x8c65545cu;r[2]=rd(m,0x8c6554a4u,4);goto L8c65545e; /* mov.l 0x8c6554a4,r2 */
L8c65545e: *stop=0x8c65545eu;r[6]=rd(m,0x8c6554a8u,4);goto L8c655460; /* mov.l 0x8c6554a8,r6 */
L8c655460: *stop=0x8c655460u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c655462; /* tst r12,r3 */
L8c655462: *stop=0x8c655462u;r[10]|=r[2];goto L8c655464; /* or r2,r10 */
L8c655464: take=(*sr&1u);*stop=0x8c655466u;r[6]&=r[10];if(take)goto L8c6554c0;goto L8c655468; /* bt/s 0x8c6554c0 */
L8c655468: *stop=0x8c655468u;r[0]=sx(m,0x8c655480u,2);goto L8c65546a; /* mov.w 0x8c655480,r0 */
L8c65546a: *stop=0x8c65546cu;if(!valid_ram(r[0]+r[13],4u))return -3;r[4]=rd(m,r[13]+r[0],4);goto L8c6554c4; /* bra 0x8c6554c4 */
L8c6554c0: *stop=0x8c6554c0u;r[3]=rd(m,0x8c655588u,4);goto L8c6554c2; /* mov.l 0x8c655588,r3 */
L8c6554c2: *stop=0x8c6554c2u;if(!valid_ram(r[3],4u))return -3;r[4]=rd(m,r[3],4);goto L8c6554c4; /* mov.l @r3,r4 */
L8c6554c4: *stop=0x8c6554c4u;r[2]=rd(m,0x8c65558cu,4);goto L8c6554c6; /* mov.l 0x8c65558c,r2 */
L8c6554c6: *stop=0x8c6554c6u;r[4]<<=16;goto L8c6554c8; /* shll16 r4 */
L8c6554c8: *stop=0x8c6554c8u;r[4]<<=2;goto L8c6554ca; /* shll2 r4 */
L8c6554ca: *stop=0x8c6554cau;*sr=(*sr&~1u)|!(r[12]&r[2]);goto L8c6554cc; /* tst r12,r2 */
L8c6554cc: take=(*sr&1u);*stop=0x8c6554ceu;r[4]|=r[6];if(take)goto L8c6554de;goto L8c6554d0; /* bt/s 0x8c6554de */
L8c6554d0: *stop=0x8c6554d0u;r[0]=sx(m,0x8c655580u,2);goto L8c6554d2; /* mov.w 0x8c655580,r0 */
L8c6554d2: *stop=0x8c6554d2u;r[1]=rd(m,0x8c655590u,4);goto L8c6554d4; /* mov.l 0x8c655590,r1 */
L8c6554d4: *stop=0x8c6554d4u;if(!valid_ram(r[0]+r[13],4u))return -3;r[3]=rd(m,r[13]+r[0],4);goto L8c6554d6; /* mov.l @(r0,r13),r3 */
L8c6554d6: *stop=0x8c6554d6u;r[1]&=r[4];goto L8c6554d8; /* and r4,r1 */
L8c6554d8: *stop=0x8c6554d8u;r[4]=r[1];goto L8c6554da; /* mov r1,r4 */
L8c6554da: *stop=0x8c6554dau;r[3]<<=16;goto L8c6554dc; /* shll16 r3 */
L8c6554dc: *stop=0x8c6554dcu;r[4]|=r[3];goto L8c6554de; /* or r3,r4 */
L8c6554de: *stop=0x8c6554deu;r[0]=sx(m,0x8c655584u,2);goto L8c6554e0; /* mov.w 0x8c655584,r0 */
L8c6554e0: *stop=0x8c6554e0u;r[3]=rd(m,0x8c655594u,4);goto L8c6554e2; /* mov.l 0x8c655594,r3 */
L8c6554e2: *stop=0x8c6554e2u;if(!valid_ram(r[0]+r[13],4u))return -3;r[2]=rd(m,r[13]+r[0],4);goto L8c6554e4; /* mov.l @(r0,r13),r2 */
L8c6554e4: *stop=0x8c6554e4u;r[6]=sx(m,0x8c655582u,2);goto L8c6554e6; /* mov.w 0x8c655582,r6 */
L8c6554e6: *stop=0x8c6554e6u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c6554e8; /* tst r3,r2 */
L8c6554e8: take=(*sr&1u);*stop=0x8c6554eau;r[6]&=r[4];if(take)goto L8c6554f8;goto L8c6554ec; /* bt/s 0x8c6554f8 */
L8c6554ec: *stop=0x8c6554ecu;if(!valid_ram(r[13]+8u,4u))return -3;r[0]=rd(m,r[13]+0x8u,4);goto L8c6554ee; /* mov.l @(0x8,r13),r0 */
L8c6554ee: *stop=0x8c6554eeu;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c6554f0; /* tst #0x1,r0 */
L8c6554f0: if((*sr&1u))goto L8c6554f8;goto L8c6554f2; /* bt 0x8c6554f8 */
L8c6554f2: *stop=0x8c6554f2u;r[4]=64u;goto L8c6554f4; /* mov #0x40,r4 */
L8c6554f4: *stop=0x8c6554f6u;r[4]|=r[6];goto L8c6554fa; /* bra 0x8c6554fa */
L8c6554f8: *stop=0x8c6554f8u;r[4]=r[6];goto L8c6554fa; /* mov r6,r4 */
L8c6554fa: *stop=0x8c6554fau;*sr=(*sr&~1u)|!(r[12]&r[5]);goto L8c6554fc; /* tst r12,r5 */
L8c6554fc: if((*sr&1u))goto L8c655520;goto L8c6554fe; /* bt 0x8c655520 */
L8c6554fe: *stop=0x8c6554feu;if(!valid_ram(r[13]+36u,4u))return -3;r[0]=rd(m,r[13]+0x24u,4);goto L8c655500; /* mov.l @(0x24,r13),r0 */
L8c655500: *stop=0x8c655500u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c655502; /* tst #0x1,r0 */
L8c655502: if((*sr&1u))goto L8c655520;goto L8c655504; /* bt 0x8c655520 */
L8c655504: *stop=0x8c655504u;if(!valid_ram(r[13]+36u,4u))return -3;r[2]=rd(m,r[13]+0x24u,4);goto L8c655506; /* mov.l @(0x24,r13),r2 */
L8c655506: *stop=0x8c655506u;r[3]=7u;goto L8c655508; /* mov #0x7,r3 */
L8c655508: *stop=0x8c655508u;r[5]=sx(m,0x8c655586u,2);goto L8c65550a; /* mov.w 0x8c655586,r5 */
L8c65550a: *stop=0x8c65550au;r[1]=rd(m,0x8c655598u,4);goto L8c65550c; /* mov.l 0x8c655598,r1 */
L8c65550c: *stop=0x8c65550cu;r[2]=(r[3]&0x80000000u)?((r[3]&31u)?r[2]>>((0u-r[3])&31u):0u):(r[2]<<(r[3]&31u));goto L8c65550e; /* shld r3,r2 */
L8c65550e: *stop=0x8c65550eu;r[5]&=r[4];goto L8c655510; /* and r4,r5 */
L8c655510: *stop=0x8c655510u;r[5]|=r[2];goto L8c655512; /* or r2,r5 */
L8c655512: *stop=0x8c655512u;if(!valid_ram(r[1],4u))return -3;r[2]=rd(m,r[1],4);goto L8c655514; /* mov.l @r1,r2 */
L8c655514: *stop=0x8c655514u;r[4]=4294967231u;goto L8c655516; /* mov #-0x41,r4 */
L8c655516: *stop=0x8c655516u;r[2]<<=2;goto L8c655518; /* shll2 r2 */
L8c655518: *stop=0x8c655518u;r[2]<<=2;goto L8c65551a; /* shll2 r2 */
L8c65551a: *stop=0x8c65551au;r[2]<<=2;goto L8c65551c; /* shll2 r2 */
L8c65551c: *stop=0x8c65551cu;r[4]&=r[5];goto L8c65551e; /* and r5,r4 */
L8c65551e: *stop=0x8c65551eu;r[4]|=r[2];goto L8c655520; /* or r2,r4 */
L8c655520: *stop=0x8c655520u;r[3]=rd(m,0x8c65559cu,4);goto L8c655522; /* mov.l 0x8c65559c,r3 */
L8c655522: *stop=0x8c655522u;*sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c655524; /* tst r3,r12 */
L8c655524: if((*sr&1u))goto L8c655534;goto L8c655526; /* bt 0x8c655534 */
L8c655526: *stop=0x8c655526u;if(!valid_ram(r[13]+12u,4u))return -3;r[2]=rd(m,r[13]+0xcu,4);goto L8c655528; /* mov.l @(0xc,r13),r2 */
L8c655528: *stop=0x8c655528u;r[1]=4294967247u;goto L8c65552a; /* mov #-0x31,r1 */
L8c65552a: *stop=0x8c65552au;r[1]&=r[4];goto L8c65552c; /* and r4,r1 */
L8c65552c: *stop=0x8c65552cu;r[2]<<=2;goto L8c65552e; /* shll2 r2 */
L8c65552e: *stop=0x8c65552eu;r[4]=r[1];goto L8c655530; /* mov r1,r4 */
L8c655530: *stop=0x8c655530u;r[2]<<=2;goto L8c655532; /* shll2 r2 */
L8c655532: *stop=0x8c655532u;r[4]|=r[2];goto L8c655534; /* or r2,r4 */
L8c655534: *stop=0x8c655534u;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c655536; /* tst r8,r8 */
L8c655536: if((*sr&1u))goto L8c65555a;goto L8c655538; /* bt 0x8c65555a */
L8c655538: *stop=0x8c655538u;if(!valid_ram(r[13]+32u,4u))return -3;r[3]=rd(m,r[13]+0x20u,4);goto L8c65553a; /* mov.l @(0x20,r13),r3 */
L8c65553a: *stop=0x8c65553au;r[2]=4294967287u;goto L8c65553c; /* mov #-0x9,r2 */
L8c65553c: *stop=0x8c65553cu;r[2]&=r[4];goto L8c65553e; /* and r4,r2 */
L8c65553e: *stop=0x8c65553eu;*sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c655540; /* shlr r3 */
L8c655540: *stop=0x8c655540u;r[4]=r[2];goto L8c655542; /* mov r2,r4 */
L8c655542: *stop=0x8c655542u;r[3]<<=2;goto L8c655544; /* shll2 r3 */
L8c655544: *stop=0x8c655544u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c655546; /* shll r3 */
L8c655546: *stop=0x8c655546u;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c655548; /* tst r8,r8 */
L8c655548: *stop=0x8c655548u;r[4]|=r[3];goto L8c65554a; /* or r3,r4 */
L8c65554a: if((*sr&1u))goto L8c65555a;goto L8c65554c; /* bt 0x8c65555a */
L8c65554c: *stop=0x8c65554cu;if(!valid_ram(r[13]+32u,4u))return -3;r[0]=rd(m,r[13]+0x20u,4);goto L8c65554e; /* mov.l @(0x20,r13),r0 */
L8c65554e: *stop=0x8c65554eu;r[2]=4294967293u;goto L8c655550; /* mov #-0x3,r2 */
L8c655550: *stop=0x8c655550u;r[2]&=r[4];goto L8c655552; /* and r4,r2 */
L8c655552: *stop=0x8c655552u;r[0]&=0x1u;goto L8c655554; /* and #0x1,r0 */
L8c655554: *stop=0x8c655554u;r[4]=r[2];goto L8c655556; /* mov r2,r4 */
L8c655556: *stop=0x8c655556u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c655558; /* shll r0 */
L8c655558: *stop=0x8c655558u;r[4]|=r[0];goto L8c65555a; /* or r0,r4 */
L8c65555a: *stop=0x8c65555au;if(!valid_ram(r[15]+12u,4u))return -3;r[3]=rd(m,r[15]+0xcu,4);goto L8c65555c; /* mov.l @(0xc,r15),r3 */
L8c65555c: *stop=0x8c65555cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c65555e; /* tst r3,r3 */
L8c65555e: if((*sr&1u))goto L8c6555a4;goto L8c655560; /* bt 0x8c6555a4 */
L8c655560: *stop=0x8c655560u;r[0]=64u;goto L8c655562; /* mov #0x40,r0 */
L8c655562: *stop=0x8c655562u;if(!valid_ram(r[0]+r[13],4u))return -3;r[3]=rd(m,r[13]+r[0],4);goto L8c655564; /* mov.l @(r0,r13),r3 */
L8c655564: *stop=0x8c655564u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c655566; /* tst r3,r3 */
L8c655566: if((*sr&1u))goto L8c6555a0;goto L8c655568; /* bt 0x8c6555a0 */
L8c655568: *stop=0x8c655568u;r[3]=4u;goto L8c65556a; /* mov #0x4,r3 */
L8c65556a: *stop=0x8c65556cu;r[4]|=r[3];goto L8c6555a4; /* bra 0x8c6555a4 */
L8c6555a0: *stop=0x8c6555a0u;r[1]=4294967291u;goto L8c6555a2; /* mov #-0x5,r1 */
L8c6555a2: *stop=0x8c6555a2u;r[4]&=r[1];goto L8c6555a4; /* and r1,r4 */
L8c6555a4: *stop=0x8c6555a4u;if(!valid_ram(r[15]+4u,4u))return -3;r[3]=rd(m,r[15]+0x4u,4);goto L8c6555a6; /* mov.l @(0x4,r15),r3 */
L8c6555a6: *stop=0x8c6555a6u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6555a8; /* tst r3,r3 */
L8c6555a8: if((*sr&1u))goto L8c6555b2;goto L8c6555aa; /* bt 0x8c6555b2 */
L8c6555aa: *stop=0x8c6555aau;r[3]=4294967294u;goto L8c6555ac; /* mov #-0x2,r3 */
L8c6555ac: *stop=0x8c6555acu;r[3]&=r[4];goto L8c6555ae; /* and r4,r3 */
L8c6555ae: *stop=0x8c6555aeu;if(!valid_ram(r[13]+16u,4u))return -3;r[4]=rd(m,r[13]+0x10u,4);goto L8c6555b0; /* mov.l @(0x10,r13),r4 */
L8c6555b0: *stop=0x8c6555b0u;r[4]|=r[3];goto L8c6555b2; /* or r3,r4 */
L8c6555b2: *stop=0x8c6555b2u;r[3]=rd(m,0x8c655608u,4);goto L8c6555b4; /* mov.l 0x8c655608,r3 */
L8c6555b4: *stop=0x8c6555b4u;r[0]=sx(m,0x8c655600u,2);goto L8c6555b6; /* mov.w 0x8c655600,r0 */
L8c6555b6: *stop=0x8c6555b6u;r[11]&=r[3];goto L8c6555b8; /* and r3,r11 */
L8c6555b8: *stop=0x8c6555b8u;r[2]=rd(m,0x8c655604u,4);goto L8c6555ba; /* mov.l 0x8c655604,r2 */
L8c6555ba: *stop=0x8c6555bau;if(!valid_ram(r[0]+r[13],4u))return -3;wr(m,r[13]+r[0],r[11],4);goto L8c6555bc; /* mov.l r11,@(r0,r13) */
L8c6555bc: *stop=0x8c6555bcu;r[0]+=4u;goto L8c6555be; /* add #0x4,r0 */
L8c6555be: *stop=0x8c6555beu;if(!valid_ram(r[0]+r[13],4u))return -3;wr(m,r[13]+r[0],r[14],4);goto L8c6555c0; /* mov.l r14,@(r0,r13) */
L8c6555c0: *stop=0x8c6555c0u;r[4]&=r[2];goto L8c6555c2; /* and r2,r4 */
L8c6555c2: *stop=0x8c6555c2u;r[0]=sx(m,0x8c655602u,2);goto L8c6555c4; /* mov.w 0x8c655602,r0 */
L8c6555c4: *stop=0x8c6555c4u;if(!valid_ram(r[15]+8u,4u))return -3;r[2]=rd(m,r[15]+0x8u,4);goto L8c6555c6; /* mov.l @(0x8,r15),r2 */
L8c6555c6: *stop=0x8c6555c6u;if(!valid_ram(r[0]+r[13],4u))return -3;wr(m,r[13]+r[0],r[2],4);goto L8c6555c8; /* mov.l r2,@(r0,r13) */
L8c6555c8: *stop=0x8c6555c8u;r[0]+=4294967284u;goto L8c6555ca; /* add #-0xc,r0 */
L8c6555ca: *stop=0x8c6555cau;if(!valid_ram(r[0]+r[13],4u))return -3;wr(m,r[13]+r[0],r[4],4);goto L8c6555cc; /* mov.l r4,@(r0,r13) */
L8c6555cc: *stop=0x8c6555ccu;r[0]=0u;goto L8c6555ce; /* mov #0x0,r0 */
L8c6555ce: *stop=0x8c6555ceu;r[15]+=36u;goto L8c6555d0; /* add #0x24,r15 */
L8c6555d0: *stop=0x8c6555d0u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c6555d2; /* lds.l @r15+,PR */
L8c6555d2: *stop=0x8c6555d2u;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c6555d4; /* mov.l @r15+,r8 */
L8c6555d4: *stop=0x8c6555d4u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c6555d6; /* mov.l @r15+,r9 */
L8c6555d6: *stop=0x8c6555d6u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c6555d8; /* mov.l @r15+,r10 */
L8c6555d8: *stop=0x8c6555d8u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c6555da; /* mov.l @r15+,r11 */
L8c6555da: *stop=0x8c6555dau;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c6555dc; /* mov.l @r15+,r12 */
L8c6555dc: *stop=0x8c6555dcu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c6555de; /* mov.l @r15+,r13 */
L8c6555de: *stop=0x8c6555deu;if(*pr!=entry_pr)return -3;*stop=0x8c6555e0u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_render_texture_header_8c655620(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c655620;
L8c655620: *stop=0x8c655620u;r[0]=sx(m,0x8c655740u,2);goto L8c655622; /* mov.w 0x8c655740,r0 */
L8c655622: *stop=0x8c655622u;r[6]=rd(m,0x8c655744u,4);goto L8c655624; /* mov.l 0x8c655744,r6 */
L8c655624: *stop=0x8c655624u;r[7]=rd(m,0x8c655748u,4);goto L8c655626; /* mov.l 0x8c655748,r7 */
L8c655626: *stop=0x8c655626u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c655628; /* mov.l r14,@-r15 */
L8c655628: *stop=0x8c655628u;if(!valid_ram(r[0]+r[4],4u))return -3;r[3]=rd(m,r[4]+r[0],4);goto L8c65562a; /* mov.l @(r0,r4),r3 */
L8c65562a: *stop=0x8c65562au;r[0]+=4u;goto L8c65562c; /* add #0x4,r0 */
L8c65562c: *stop=0x8c65562cu;if(!valid_ram(r[6],4u))return -3;wr(m,r[6],r[3],4);goto L8c65562e; /* mov.l r3,@r6 */
L8c65562e: *stop=0x8c65562eu;if(!valid_ram(r[0]+r[4],4u))return -3;r[2]=rd(m,r[4]+r[0],4);goto L8c655630; /* mov.l @(r0,r4),r2 */
L8c655630: *stop=0x8c655630u;r[0]+=4u;goto L8c655632; /* add #0x4,r0 */
L8c655632: *stop=0x8c655632u;if(!valid_ram(r[6]+16u,4u))return -3;wr(m,r[6]+0x10u,r[2],4);goto L8c655634; /* mov.l r2,@(0x10,r6) */
L8c655634: *stop=0x8c655634u;if(!valid_ram(r[0]+r[4],4u))return -3;r[3]=rd(m,r[4]+r[0],4);goto L8c655636; /* mov.l @(r0,r4),r3 */
L8c655636: *stop=0x8c655636u;if(!valid_ram(r[6]+20u,4u))return -3;wr(m,r[6]+0x14u,r[3],4);goto L8c655638; /* mov.l r3,@(0x14,r6) */
L8c655638: *stop=0x8c655638u;if(!valid_ram(r[6],4u))return -3;r[0]=rd(m,r[6],4);goto L8c65563a; /* mov.l @r6,r0 */
L8c65563a: *stop=0x8c65563au;r[0]>>=16;goto L8c65563c; /* shlr16 r0 */
L8c65563c: *stop=0x8c65563cu;r[0]>>=8;goto L8c65563e; /* shlr8 r0 */
L8c65563e: *stop=0x8c65563eu;*sr=(*sr&~1u)|!(r[0]&0x7u);goto L8c655640; /* tst #0x7,r0 */
L8c655640: if(!(*sr&1u))goto L8c655650;goto L8c655642; /* bf 0x8c655650 */
L8c655642: *stop=0x8c655642u;r[5]=r[6];goto L8c655644; /* mov r6,r5 */
L8c655644: *stop=0x8c655644u;r[5]+=16u;goto L8c655646; /* add #0x10,r5 */
L8c655646: *stop=0x8c655646u;if(!valid_ram(r[5]+4u,4u))return -3;r[2]=rd(m,r[5]+0x4u,4);goto L8c655648; /* mov.l @(0x4,r5),r2 */
L8c655648: *stop=0x8c655648u;r[3]=rd(m,0x8c65574cu,4);goto L8c65564a; /* mov.l 0x8c65574c,r3 */
L8c65564a: *stop=0x8c65564au;r[2]&=r[3];goto L8c65564c; /* and r3,r2 */
L8c65564c: *stop=0x8c65564cu;r[2]|=r[7];goto L8c65564e; /* or r7,r2 */
L8c65564e: *stop=0x8c65564eu;if(!valid_ram(r[5]+4u,4u))return -3;wr(m,r[5]+0x4u,r[2],4);goto L8c655650; /* mov.l r2,@(0x4,r5) */
L8c655650: *stop=0x8c655650u;r[0]=sx(m,0x8c655742u,2);goto L8c655652; /* mov.w 0x8c655742,r0 */
L8c655652: *stop=0x8c655652u;if(!valid_ram(r[0]+r[4],4u))return -3;r[3]=rd(m,r[4]+r[0],4);goto L8c655654; /* mov.l @(r0,r4),r3 */
L8c655654: *stop=0x8c655654u;r[0]=112u;goto L8c655656; /* mov #0x70,r0 */
L8c655656: *stop=0x8c655656u;if(!valid_ram(r[6]+24u,4u))return -3;wr(m,r[6]+0x18u,r[3],4);goto L8c655658; /* mov.l r3,@(0x18,r6) */
L8c655658: *stop=0x8c655658u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c65565a; /* fmov.s @(r0,r4),fr3 */
L8c65565a: *stop=0x8c65565au;r[0]=32u;goto L8c65565c; /* mov #0x20,r0 */
L8c65565c: *stop=0x8c65565cu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto L8c65565e; /* fmov fr3,@(r0,r6) */
L8c65565e: *stop=0x8c65565eu;r[0]=116u;goto L8c655660; /* mov #0x74,r0 */
L8c655660: *stop=0x8c655660u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c655662; /* fmov.s @(r0,r4),fr3 */
L8c655662: *stop=0x8c655662u;r[0]=36u;goto L8c655664; /* mov #0x24,r0 */
L8c655664: *stop=0x8c655664u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto L8c655666; /* fmov fr3,@(r0,r6) */
L8c655666: *stop=0x8c655666u;r[0]=120u;goto L8c655668; /* mov #0x78,r0 */
L8c655668: *stop=0x8c655668u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c65566a; /* fmov.s @(r0,r4),fr3 */
L8c65566a: *stop=0x8c65566au;r[0]=40u;goto L8c65566c; /* mov #0x28,r0 */
L8c65566c: *stop=0x8c65566cu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto L8c65566e; /* fmov fr3,@(r0,r6) */
L8c65566e: *stop=0x8c65566eu;r[0]=124u;goto L8c655670; /* mov #0x7c,r0 */
L8c655670: *stop=0x8c655670u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c655672; /* fmov.s @(r0,r4),fr3 */
L8c655672: *stop=0x8c655672u;r[0]=44u;goto L8c655674; /* mov #0x2c,r0 */
L8c655674: *stop=0x8c655674u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto L8c655676; /* fmov fr3,@(r0,r6) */
L8c655676: *stop=0x8c655676u;r[0]+=84u;goto L8c655678; /* add #0x54,r0 */
L8c655678: *stop=0x8c655678u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c65567a; /* fmov.s @(r0,r4),fr3 */
L8c65567a: *stop=0x8c65567au;r[0]=48u;goto L8c65567c; /* mov #0x30,r0 */
L8c65567c: *stop=0x8c65567cu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto L8c65567e; /* fmov fr3,@(r0,r6) */
L8c65567e: *stop=0x8c65567eu;r[0]+=84u;goto L8c655680; /* add #0x54,r0 */
L8c655680: *stop=0x8c655680u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c655682; /* fmov.s @(r0,r4),fr3 */
L8c655682: *stop=0x8c655682u;r[0]=52u;goto L8c655684; /* mov #0x34,r0 */
L8c655684: *stop=0x8c655684u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto L8c655686; /* fmov fr3,@(r0,r6) */
L8c655686: *stop=0x8c655686u;r[0]+=84u;goto L8c655688; /* add #0x54,r0 */
L8c655688: *stop=0x8c655688u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c65568a; /* fmov.s @(r0,r4),fr3 */
L8c65568a: *stop=0x8c65568au;r[0]=56u;goto L8c65568c; /* mov #0x38,r0 */
L8c65568c: *stop=0x8c65568cu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto L8c65568e; /* fmov fr3,@(r0,r6) */
L8c65568e: *stop=0x8c65568eu;r[0]+=84u;goto L8c655690; /* add #0x54,r0 */
L8c655690: *stop=0x8c655690u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c655692; /* fmov.s @(r0,r4),fr3 */
L8c655692: *stop=0x8c655692u;r[0]=60u;goto L8c655694; /* mov #0x3c,r0 */
L8c655694: *stop=0x8c655694u;r[2]=rd(m,0x8c655754u,4);goto L8c655696; /* mov.l 0x8c655754,r2 */
L8c655696: *stop=0x8c655696u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto L8c655698; /* fmov fr3,@(r0,r6) */
L8c655698: *stop=0x8c655698u;r[0]+=100u;goto L8c65569a; /* add #0x64,r0 */
L8c65569a: *stop=0x8c65569au;if(!valid_ram(r[0]+r[4],4u))return -3;r[3]=rd(m,r[4]+r[0],4);goto L8c65569c; /* mov.l @(r0,r4),r3 */
L8c65569c: *stop=0x8c65569cu;r[0]=64u;goto L8c65569e; /* mov #0x40,r0 */
L8c65569e: *stop=0x8c65569eu;r[5]=rd(m,0x8c655750u,4);goto L8c6556a0; /* mov.l 0x8c655750,r5 */
L8c6556a0: *stop=0x8c6556a0u;if(!valid_ram(r[0]+r[6],4u))return -3;wr(m,r[6]+r[0],r[3],4);goto L8c6556a2; /* mov.l r3,@(r0,r6) */
L8c6556a2: *stop=0x8c6556a2u;r[3]=32u;goto L8c6556a4; /* mov #0x20,r3 */
L8c6556a4: *stop=0x8c6556a4u;if(!valid_ram(r[6],4u))return -3;r[14]=rd(m,r[6],4);goto L8c6556a6; /* mov.l @r6,r14 */
L8c6556a6: *stop=0x8c6556a6u;r[4]=r[5];goto L8c6556a8; /* mov r5,r4 */
L8c6556a8: *stop=0x8c6556a8u;r[2]&=r[14];goto L8c6556aa; /* and r14,r2 */
L8c6556aa: *stop=0x8c6556aau;r[2]>>=16;goto L8c6556ac; /* shlr16 r2 */
L8c6556ac: *stop=0x8c6556acu;r[7]&=r[14];goto L8c6556ae; /* and r14,r7 */
L8c6556ae: *stop=0x8c6556aeu;r[2]>>=2;goto L8c6556b0; /* shlr2 r2 */
L8c6556b0: *stop=0x8c6556b0u;r[0]&=r[14];goto L8c6556b2; /* and r14,r0 */
L8c6556b2: *stop=0x8c6556b2u;r[7]>>=16;goto L8c6556b4; /* shlr16 r7 */
L8c6556b4: *stop=0x8c6556b4u;r[0]>>=2;goto L8c6556b6; /* shlr2 r0 */
L8c6556b6: *stop=0x8c6556b6u;r[7]>>=8;goto L8c6556b8; /* shlr8 r7 */
L8c6556b8: *stop=0x8c6556b8u;r[3]&=r[14];goto L8c6556ba; /* and r14,r3 */
L8c6556ba: *stop=0x8c6556bau;r[2]>>=2;goto L8c6556bc; /* shlr2 r2 */
L8c6556bc: *stop=0x8c6556bcu;r[3]>>=2;goto L8c6556be; /* shlr2 r3 */
L8c6556be: *stop=0x8c6556beu;*sr=(*sr&~1u)|(r[0]&1u);r[0]>>=1;goto L8c6556c0; /* shlr r0 */
L8c6556c0: *stop=0x8c6556c0u;r[7]|=r[2];goto L8c6556c2; /* or r2,r7 */
L8c6556c2: *stop=0x8c6556c2u;*sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c6556c4; /* shlr r3 */
L8c6556c4: *stop=0x8c6556c4u;r[7]|=r[0];goto L8c6556c6; /* or r0,r7 */
L8c6556c6: *stop=0x8c6556c6u;r[7]|=r[3];goto L8c6556c8; /* or r3,r7 */
L8c6556c8: *stop=0x8c6556c8u;r[3]=16u;goto L8c6556ca; /* mov #0x10,r3 */
L8c6556ca: *stop=0x8c6556cau;r[3]&=r[14];goto L8c6556cc; /* and r14,r3 */
L8c6556cc: *stop=0x8c6556ccu;r[3]>>=2;goto L8c6556ce; /* shlr2 r3 */
L8c6556ce: *stop=0x8c6556ceu;*sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c6556d0; /* shlr r3 */
L8c6556d0: *stop=0x8c6556d0u;r[7]|=r[3];goto L8c6556d2; /* or r3,r7 */
L8c6556d2: *stop=0x8c6556d2u;r[0]=rd(m,0x8c655758u,4);goto L8c6556d4; /* mov.l 0x8c655758,r0 */
L8c6556d4: *stop=0x8c6556d4u;r[3]=4u;goto L8c6556d6; /* mov #0x4,r3 */
L8c6556d6: *stop=0x8c6556d6u;r[2]=rd(m,0x8c65575cu,4);goto L8c6556d8; /* mov.l 0x8c65575c,r2 */
L8c6556d8: *stop=0x8c6556d8u;r[3]&=r[14];goto L8c6556da; /* and r14,r3 */
L8c6556da: *stop=0x8c6556dau;r[3]>>=2;goto L8c6556dc; /* shlr2 r3 */
L8c6556dc: *stop=0x8c6556dcu;r[7]|=r[3];goto L8c6556de; /* or r3,r7 */
L8c6556de: *stop=0x8c6556deu;r[7]<<=2;goto L8c6556e0; /* shll2 r7 */
L8c6556e0: *stop=0x8c6556e0u;if(!valid_ram(r[0]+r[7],4u))return -3;r[3]=rd(m,r[7]+r[0],4);goto L8c6556e2; /* mov.l @(r0,r7),r3 */
L8c6556e2: *stop=0x8c6556e2u;r[7]=rd(m,0x8c655760u,4);goto L8c6556e4; /* mov.l 0x8c655760,r7 */
L8c6556e4: *stop=0x8c6556e4u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[3],4);goto L8c6556e6; /* mov.l r3,@r2 */
L8c6556e6: *stop=0x8c6556e6u;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c6556e8; /* mov.l @r6,r3 */
L8c6556e8: *stop=0x8c6556e8u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[3],4);goto L8c6556ea; /* mov.l r3,@r4 */
L8c6556ea: *stop=0x8c6556eau;r[3]=rd(m,0x8c655764u,4);goto L8c6556ec; /* mov.l 0x8c655764,r3 */
L8c6556ec: *stop=0x8c6556ecu;if(!valid_ram(r[6],4u))return -3;r[2]=rd(m,r[6],4);goto L8c6556ee; /* mov.l @r6,r2 */
L8c6556ee: *stop=0x8c6556eeu;r[2]&=r[3];goto L8c6556f0; /* and r3,r2 */
L8c6556f0: *stop=0x8c6556f0u;r[2]>>=16;goto L8c6556f2; /* shlr16 r2 */
L8c6556f2: *stop=0x8c6556f2u;r[2]>>=8;goto L8c6556f4; /* shlr8 r2 */
L8c6556f4: *stop=0x8c6556f4u;r[0]=r[2];goto L8c6556f6; /* mov r2,r0 */
L8c6556f6: *stop=0x8c6556f6u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c6556f8; /* tst #0x1,r0 */
L8c6556f8: *stop=0x8c6556f8u;if(!valid_ram(r[7],4u))return -3;wr(m,r[7],r[2],4);goto L8c6556fa; /* mov.l r2,@r7 */
L8c6556fa: take=(*sr&1u);*stop=0x8c6556fcu;r[4]+=4u;if(take)goto L8c655702;goto L8c6556fe; /* bt/s 0x8c655702 */
L8c6556fe: *stop=0x8c655700u;goto L8c6558c2; /* bra 0x8c6558c2 */
L8c655702: *stop=0x8c655702u;if(!valid_ram(r[6]+16u,4u))return -3;r[2]=rd(m,r[6]+0x10u,4);goto L8c655704; /* mov.l @(0x10,r6),r2 */
L8c655704: *stop=0x8c655704u;r[14]=r[6];goto L8c655706; /* mov r6,r14 */
L8c655706: *stop=0x8c655706u;r[1]=rd(m,0x8c65575cu,4);goto L8c655708; /* mov.l 0x8c65575c,r1 */
L8c655708: *stop=0x8c655708u;r[14]+=16u;goto L8c65570a; /* add #0x10,r14 */
L8c65570a: *stop=0x8c65570au;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[2],4);goto L8c65570c; /* mov.l r2,@r4 */
L8c65570c: *stop=0x8c65570cu;r[4]+=4u;goto L8c65570e; /* add #0x4,r4 */
L8c65570e: *stop=0x8c65570eu;if(!valid_ram(r[6]+20u,4u))return -3;r[2]=rd(m,r[6]+0x14u,4);goto L8c655710; /* mov.l @(0x14,r6),r2 */
L8c655710: *stop=0x8c655710u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[2],4);goto L8c655712; /* mov.l r2,@r4 */
L8c655712: *stop=0x8c655712u;r[4]+=4u;goto L8c655714; /* add #0x4,r4 */
L8c655714: *stop=0x8c655714u;if(!valid_ram(r[6]+24u,4u))return -3;r[2]=rd(m,r[6]+0x18u,4);goto L8c655716; /* mov.l @(0x18,r6),r2 */
L8c655716: *stop=0x8c655716u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[2],4);goto L8c655718; /* mov.l r2,@r4 */
L8c655718: *stop=0x8c655718u;if(!valid_ram(r[1],4u))return -3;r[0]=rd(m,r[1],4);goto L8c65571a; /* mov.l @r1,r0 */
L8c65571a: *stop=0x8c65571au;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c65571c; /* cmp/eq #0x0,r0 */
L8c65571c: take=(*sr&1u);*stop=0x8c65571eu;r[4]+=4u;if(take)goto L8c655780;goto L8c655720; /* bt/s 0x8c655780 */
L8c655720: *stop=0x8c655720u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c655722; /* cmp/eq #0x1,r0 */
L8c655722: if((*sr&1u))goto L8c655784;goto L8c655724; /* bt 0x8c655784 */
L8c655724: *stop=0x8c655724u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c655726; /* cmp/eq #0x2,r0 */
L8c655726: if((*sr&1u))goto L8c65579e;goto L8c655728; /* bt 0x8c65579e */
L8c655728: *stop=0x8c655728u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c65572a; /* cmp/eq #0x3,r0 */
L8c65572a: if((*sr&1u))goto L8c6557d8;goto L8c65572c; /* bt 0x8c6557d8 */
L8c65572c: *stop=0x8c65572cu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c65572e; /* cmp/eq #0x4,r0 */
L8c65572e: if((*sr&1u))goto L8c6557ea;goto L8c655730; /* bt 0x8c6557ea */
L8c655730: *stop=0x8c655730u;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c655732; /* cmp/eq #0x10,r0 */
L8c655732: if(!(*sr&1u))goto L8c655738;goto L8c655734; /* bf 0x8c655738 */
L8c655734: *stop=0x8c655736u;goto L8c655844; /* bra 0x8c655844 */
L8c655738: *stop=0x8c65573au;goto L8c6558ca; /* bra 0x8c6558ca */
L8c655780: *stop=0x8c655782u;r[4]+=16u;goto L8c6558ca; /* bra 0x8c6558ca */
L8c655784: *stop=0x8c655784u;r[2]=r[14];goto L8c655786; /* mov r14,r2 */
L8c655786: *stop=0x8c655786u;if(!valid_ram(r[2]+16u,4u))return -3;r[0]=rd(m,r[2]+0x10u,4);goto L8c655788; /* mov.l @(0x10,r2),r0 */
L8c655788: *stop=0x8c655788u;r[1]=r[6];goto L8c65578a; /* mov r6,r1 */
L8c65578a: *stop=0x8c65578au;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[0],4);goto L8c65578c; /* mov.l r0,@r4 */
L8c65578c: *stop=0x8c65578cu;r[4]+=4u;goto L8c65578e; /* add #0x4,r4 */
L8c65578e: *stop=0x8c65578eu;if(!valid_ram(r[1]+36u,4u))return -3;r[0]=rd(m,r[1]+0x24u,4);goto L8c655790; /* mov.l @(0x24,r1),r0 */
L8c655790: *stop=0x8c655790u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[0],4);goto L8c655792; /* mov.l r0,@r4 */
L8c655792: *stop=0x8c655792u;r[4]+=4u;goto L8c655794; /* add #0x4,r4 */
L8c655794: *stop=0x8c655794u;if(!valid_ram(r[1]+40u,4u))return -3;r[0]=rd(m,r[1]+0x28u,4);goto L8c655796; /* mov.l @(0x28,r1),r0 */
L8c655796: *stop=0x8c655796u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[0],4);goto L8c655798; /* mov.l r0,@r4 */
L8c655798: *stop=0x8c655798u;r[4]+=4u;goto L8c65579a; /* add #0x4,r4 */
L8c65579a: *stop=0x8c65579cu;r[1]+=44u;goto L8c6557d2; /* bra 0x8c6557d2 */
L8c65579e: *stop=0x8c65579eu;r[2]=r[14];goto L8c6557a0; /* mov r14,r2 */
L8c6557a0: *stop=0x8c6557a0u;r[2]+=16u;goto L8c6557a2; /* add #0x10,r2 */
L8c6557a2: *stop=0x8c6557a2u;if(!valid_ram(r[2],4u))return -3;r[0]=rd(m,r[2],4);goto L8c6557a4; /* mov.l @r2,r0 */
L8c6557a4: *stop=0x8c6557a4u;r[4]+=16u;goto L8c6557a6; /* add #0x10,r4 */
L8c6557a6: *stop=0x8c6557a6u;r[1]=r[6];goto L8c6557a8; /* mov r6,r1 */
L8c6557a8: *stop=0x8c6557a8u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[0],4);goto L8c6557aa; /* mov.l r0,@r4 */
L8c6557aa: *stop=0x8c6557aau;r[4]+=4u;goto L8c6557ac; /* add #0x4,r4 */
L8c6557ac: *stop=0x8c6557acu;if(!valid_ram(r[1]+36u,4u))return -3;r[0]=rd(m,r[1]+0x24u,4);goto L8c6557ae; /* mov.l @(0x24,r1),r0 */
L8c6557ae: *stop=0x8c6557aeu;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[0],4);goto L8c6557b0; /* mov.l r0,@r4 */
L8c6557b0: *stop=0x8c6557b0u;r[4]+=4u;goto L8c6557b2; /* add #0x4,r4 */
L8c6557b2: *stop=0x8c6557b2u;if(!valid_ram(r[1]+40u,4u))return -3;r[0]=rd(m,r[1]+0x28u,4);goto L8c6557b4; /* mov.l @(0x28,r1),r0 */
L8c6557b4: *stop=0x8c6557b4u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[0],4);goto L8c6557b6; /* mov.l r0,@r4 */
L8c6557b6: *stop=0x8c6557b6u;r[4]+=4u;goto L8c6557b8; /* add #0x4,r4 */
L8c6557b8: *stop=0x8c6557b8u;if(!valid_ram(r[1]+44u,4u))return -3;r[0]=rd(m,r[1]+0x2cu,4);goto L8c6557ba; /* mov.l @(0x2c,r1),r0 */
L8c6557ba: *stop=0x8c6557bau;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[0],4);goto L8c6557bc; /* mov.l r0,@r4 */
L8c6557bc: *stop=0x8c6557bcu;r[4]+=4u;goto L8c6557be; /* add #0x4,r4 */
L8c6557be: *stop=0x8c6557beu;if(!valid_ram(r[1]+48u,4u))return -3;r[0]=rd(m,r[1]+0x30u,4);goto L8c6557c0; /* mov.l @(0x30,r1),r0 */
L8c6557c0: *stop=0x8c6557c0u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[0],4);goto L8c6557c2; /* mov.l r0,@r4 */
L8c6557c2: *stop=0x8c6557c2u;r[4]+=4u;goto L8c6557c4; /* add #0x4,r4 */
L8c6557c4: *stop=0x8c6557c4u;if(!valid_ram(r[1]+52u,4u))return -3;r[0]=rd(m,r[1]+0x34u,4);goto L8c6557c6; /* mov.l @(0x34,r1),r0 */
L8c6557c6: *stop=0x8c6557c6u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[0],4);goto L8c6557c8; /* mov.l r0,@r4 */
L8c6557c8: *stop=0x8c6557c8u;r[4]+=4u;goto L8c6557ca; /* add #0x4,r4 */
L8c6557ca: *stop=0x8c6557cau;if(!valid_ram(r[1]+56u,4u))return -3;r[0]=rd(m,r[1]+0x38u,4);goto L8c6557cc; /* mov.l @(0x38,r1),r0 */
L8c6557cc: *stop=0x8c6557ccu;r[1]+=60u;goto L8c6557ce; /* add #0x3c,r1 */
L8c6557ce: *stop=0x8c6557ceu;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[0],4);goto L8c6557d0; /* mov.l r0,@r4 */
L8c6557d0: *stop=0x8c6557d0u;r[4]+=4u;goto L8c6557d2; /* add #0x4,r4 */
L8c6557d2: *stop=0x8c6557d2u;if(!valid_ram(r[1],4u))return -3;r[0]=rd(m,r[1],4);goto L8c6557d4; /* mov.l @r1,r0 */
L8c6557d4: *stop=0x8c6557d6u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[0],4);goto L8c655840; /* bra 0x8c655840 */
L8c6557d8: *stop=0x8c6557d8u;r[0]=68u;goto L8c6557da; /* mov #0x44,r0 */
L8c6557da: *stop=0x8c6557dau;if(!valid_ram(r[0]+r[6],4u))return -3;r[2]=rd(m,r[6]+r[0],4);goto L8c6557dc; /* mov.l @(r0,r6),r2 */
L8c6557dc: *stop=0x8c6557dcu;r[0]=72u;goto L8c6557de; /* mov #0x48,r0 */
L8c6557de: *stop=0x8c6557deu;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[2],4);goto L8c6557e0; /* mov.l r2,@r4 */
L8c6557e0: *stop=0x8c6557e0u;r[4]+=4u;goto L8c6557e2; /* add #0x4,r4 */
L8c6557e2: *stop=0x8c6557e2u;if(!valid_ram(r[0]+r[6],4u))return -3;r[2]=rd(m,r[6]+r[0],4);goto L8c6557e4; /* mov.l @(r0,r6),r2 */
L8c6557e4: *stop=0x8c6557e4u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[2],4);goto L8c6557e6; /* mov.l r2,@r4 */
L8c6557e6: *stop=0x8c6557e8u;r[4]+=12u;goto L8c6558ca; /* bra 0x8c6558ca */
L8c6557ea: *stop=0x8c6557eau;r[0]=68u;goto L8c6557ec; /* mov #0x44,r0 */
L8c6557ec: *stop=0x8c6557ecu;r[1]=r[6];goto L8c6557ee; /* mov r6,r1 */
L8c6557ee: *stop=0x8c6557eeu;if(!valid_ram(r[0]+r[6],4u))return -3;r[2]=rd(m,r[6]+r[0],4);goto L8c6557f0; /* mov.l @(r0,r6),r2 */
L8c6557f0: *stop=0x8c6557f0u;r[0]=72u;goto L8c6557f2; /* mov #0x48,r0 */
L8c6557f2: *stop=0x8c6557f2u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[2],4);goto L8c6557f4; /* mov.l r2,@r4 */
L8c6557f4: *stop=0x8c6557f4u;r[4]+=4u;goto L8c6557f6; /* add #0x4,r4 */
L8c6557f6: *stop=0x8c6557f6u;if(!valid_ram(r[0]+r[6],4u))return -3;r[2]=rd(m,r[6]+r[0],4);goto L8c6557f8; /* mov.l @(r0,r6),r2 */
L8c6557f8: *stop=0x8c6557f8u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[2],4);goto L8c6557fa; /* mov.l r2,@r4 */
L8c6557fa: *stop=0x8c6557fau;r[4]+=12u;goto L8c6557fc; /* add #0xc,r4 */
L8c6557fc: *stop=0x8c6557fcu;if(!valid_ram(r[1]+32u,4u))return -3;r[1]=rd(m,r[1]+0x20u,4);goto L8c6557fe; /* mov.l @(0x20,r1),r1 */
L8c6557fe: *stop=0x8c6557feu;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[1],4);goto L8c655800; /* mov.l r1,@r4 */
L8c655800: *stop=0x8c655800u;r[1]=r[6];goto L8c655802; /* mov r6,r1 */
L8c655802: *stop=0x8c655802u;if(!valid_ram(r[1]+36u,4u))return -3;r[1]=rd(m,r[1]+0x24u,4);goto L8c655804; /* mov.l @(0x24,r1),r1 */
L8c655804: *stop=0x8c655804u;r[4]+=4u;goto L8c655806; /* add #0x4,r4 */
L8c655806: *stop=0x8c655806u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[1],4);goto L8c655808; /* mov.l r1,@r4 */
L8c655808: *stop=0x8c655808u;r[1]=r[6];goto L8c65580a; /* mov r6,r1 */
L8c65580a: *stop=0x8c65580au;if(!valid_ram(r[1]+40u,4u))return -3;r[1]=rd(m,r[1]+0x28u,4);goto L8c65580c; /* mov.l @(0x28,r1),r1 */
L8c65580c: *stop=0x8c65580cu;r[4]+=4u;goto L8c65580e; /* add #0x4,r4 */
L8c65580e: *stop=0x8c65580eu;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[1],4);goto L8c655810; /* mov.l r1,@r4 */
L8c655810: *stop=0x8c655810u;r[1]=r[6];goto L8c655812; /* mov r6,r1 */
L8c655812: *stop=0x8c655812u;if(!valid_ram(r[1]+44u,4u))return -3;r[1]=rd(m,r[1]+0x2cu,4);goto L8c655814; /* mov.l @(0x2c,r1),r1 */
L8c655814: *stop=0x8c655814u;r[4]+=4u;goto L8c655816; /* add #0x4,r4 */
L8c655816: *stop=0x8c655816u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[1],4);goto L8c655818; /* mov.l r1,@r4 */
L8c655818: *stop=0x8c655818u;r[1]=r[6];goto L8c65581a; /* mov r6,r1 */
L8c65581a: *stop=0x8c65581au;r[1]+=80u;goto L8c65581c; /* add #0x50,r1 */
L8c65581c: *stop=0x8c65581cu;if(!valid_ram(r[1],4u))return -3;r[1]=rd(m,r[1],4);goto L8c65581e; /* mov.l @r1,r1 */
L8c65581e: *stop=0x8c65581eu;r[4]+=4u;goto L8c655820; /* add #0x4,r4 */
L8c655820: *stop=0x8c655820u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[1],4);goto L8c655822; /* mov.l r1,@r4 */
L8c655822: *stop=0x8c655822u;r[1]=r[6];goto L8c655824; /* mov r6,r1 */
L8c655824: *stop=0x8c655824u;r[1]+=84u;goto L8c655826; /* add #0x54,r1 */
L8c655826: *stop=0x8c655826u;if(!valid_ram(r[1],4u))return -3;r[1]=rd(m,r[1],4);goto L8c655828; /* mov.l @r1,r1 */
L8c655828: *stop=0x8c655828u;r[4]+=4u;goto L8c65582a; /* add #0x4,r4 */
L8c65582a: *stop=0x8c65582au;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[1],4);goto L8c65582c; /* mov.l r1,@r4 */
L8c65582c: *stop=0x8c65582cu;r[1]=r[6];goto L8c65582e; /* mov r6,r1 */
L8c65582e: *stop=0x8c65582eu;r[1]+=88u;goto L8c655830; /* add #0x58,r1 */
L8c655830: *stop=0x8c655830u;if(!valid_ram(r[1],4u))return -3;r[1]=rd(m,r[1],4);goto L8c655832; /* mov.l @r1,r1 */
L8c655832: *stop=0x8c655832u;r[4]+=4u;goto L8c655834; /* add #0x4,r4 */
L8c655834: *stop=0x8c655834u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[1],4);goto L8c655836; /* mov.l r1,@r4 */
L8c655836: *stop=0x8c655836u;r[1]=r[6];goto L8c655838; /* mov r6,r1 */
L8c655838: *stop=0x8c655838u;r[1]+=92u;goto L8c65583a; /* add #0x5c,r1 */
L8c65583a: *stop=0x8c65583au;if(!valid_ram(r[1],4u))return -3;r[1]=rd(m,r[1],4);goto L8c65583c; /* mov.l @r1,r1 */
L8c65583c: *stop=0x8c65583cu;r[4]+=4u;goto L8c65583e; /* add #0x4,r4 */
L8c65583e: *stop=0x8c65583eu;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[1],4);goto L8c655840; /* mov.l r1,@r4 */
L8c655840: *stop=0x8c655842u;r[4]+=4u;goto L8c6558ca; /* bra 0x8c6558ca */
L8c655844: *stop=0x8c655844u;r[0]=0x8c6558e0u;goto L8c655846; /* mova 0x8c6558e0,r0 */
L8c655846: *stop=0x8c655846u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]);goto L8c655848; /* fmov.s @r0,fr4 */
L8c655848: *stop=0x8c655848u;r[0]=16u;goto L8c65584a; /* mov #0x10,r0 */
L8c65584a: *stop=0x8c65584au;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c65584c; /* fmov.s @(r0,r14),fr3 */
L8c65584c: *stop=0x8c65584cu;r[0]=20u;goto L8c65584e; /* mov #0x14,r0 */
L8c65584e: *stop=0x8c65584eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c655850; /* fmul fr4,fr3 */
L8c655850: *stop=0x8c655850u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c655852; /* ftrc fr3,FPUL */
L8c655852: *stop=0x8c655852u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c655854; /* fmov.s @(r0,r14),fr3 */
L8c655854: *stop=0x8c655854u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c655856; /* fmul fr4,fr3 */
L8c655856: *stop=0x8c655856u;r[1]=*fpul;goto L8c655858; /* sts FPUL,r1 */
L8c655858: *stop=0x8c655858u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c65585a; /* ftrc fr3,FPUL */
L8c65585a: *stop=0x8c65585au;r[1]<<=16;goto L8c65585c; /* shll16 r1 */
L8c65585c: *stop=0x8c65585cu;r[1]<<=8;goto L8c65585e; /* shll8 r1 */
L8c65585e: *stop=0x8c65585eu;r[0]=*fpul;goto L8c655860; /* sts FPUL,r0 */
L8c655860: *stop=0x8c655860u;r[0]<<=16;goto L8c655862; /* shll16 r0 */
L8c655862: *stop=0x8c655862u;r[1]|=r[0];goto L8c655864; /* or r0,r1 */
L8c655864: *stop=0x8c655864u;r[0]=24u;goto L8c655866; /* mov #0x18,r0 */
L8c655866: *stop=0x8c655866u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c655868; /* fmov.s @(r0,r14),fr3 */
L8c655868: *stop=0x8c655868u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c65586a; /* fmul fr4,fr3 */
L8c65586a: *stop=0x8c65586au;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c65586c; /* ftrc fr3,FPUL */
L8c65586c: *stop=0x8c65586cu;r[0]=*fpul;goto L8c65586e; /* sts FPUL,r0 */
L8c65586e: *stop=0x8c65586eu;r[0]<<=8;goto L8c655870; /* shll8 r0 */
L8c655870: *stop=0x8c655870u;r[1]|=r[0];goto L8c655872; /* or r0,r1 */
L8c655872: *stop=0x8c655872u;r[0]=28u;goto L8c655874; /* mov #0x1c,r0 */
L8c655874: *stop=0x8c655874u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c655876; /* fmov.s @(r0,r14),fr3 */
L8c655876: *stop=0x8c655876u;r[14]=r[6];goto L8c655878; /* mov r6,r14 */
L8c655878: *stop=0x8c655878u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c65587a; /* fmul fr4,fr3 */
L8c65587a: *stop=0x8c65587au;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c65587c; /* ftrc fr3,FPUL */
L8c65587c: *stop=0x8c65587cu;r[0]=*fpul;goto L8c65587e; /* sts FPUL,r0 */
L8c65587e: *stop=0x8c65587eu;r[1]|=r[0];goto L8c655880; /* or r0,r1 */
L8c655880: *stop=0x8c655880u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[1],4);goto L8c655882; /* mov.l r1,@r4 */
L8c655882: *stop=0x8c655882u;r[4]+=4u;goto L8c655884; /* add #0x4,r4 */
L8c655884: *stop=0x8c655884u;r[0]=32u;goto L8c655886; /* mov #0x20,r0 */
L8c655886: *stop=0x8c655886u;r[14]+=16u;goto L8c655888; /* add #0x10,r14 */
L8c655888: *stop=0x8c655888u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c65588a; /* fmov.s @(r0,r14),fr3 */
L8c65588a: *stop=0x8c65588au;r[0]=36u;goto L8c65588c; /* mov #0x24,r0 */
L8c65588c: *stop=0x8c65588cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c65588e; /* fmul fr4,fr3 */
L8c65588e: *stop=0x8c65588eu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c655890; /* ftrc fr3,FPUL */
L8c655890: *stop=0x8c655890u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c655892; /* fmov.s @(r0,r14),fr3 */
L8c655892: *stop=0x8c655892u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c655894; /* fmul fr4,fr3 */
L8c655894: *stop=0x8c655894u;r[1]=*fpul;goto L8c655896; /* sts FPUL,r1 */
L8c655896: *stop=0x8c655896u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c655898; /* ftrc fr3,FPUL */
L8c655898: *stop=0x8c655898u;r[1]<<=16;goto L8c65589a; /* shll16 r1 */
L8c65589a: *stop=0x8c65589au;r[1]<<=8;goto L8c65589c; /* shll8 r1 */
L8c65589c: *stop=0x8c65589cu;r[0]=*fpul;goto L8c65589e; /* sts FPUL,r0 */
L8c65589e: *stop=0x8c65589eu;r[0]<<=16;goto L8c6558a0; /* shll16 r0 */
L8c6558a0: *stop=0x8c6558a0u;r[1]|=r[0];goto L8c6558a2; /* or r0,r1 */
L8c6558a2: *stop=0x8c6558a2u;r[0]=40u;goto L8c6558a4; /* mov #0x28,r0 */
L8c6558a4: *stop=0x8c6558a4u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c6558a6; /* fmov.s @(r0,r14),fr3 */
L8c6558a6: *stop=0x8c6558a6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6558a8; /* fmul fr4,fr3 */
L8c6558a8: *stop=0x8c6558a8u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6558aa; /* ftrc fr3,FPUL */
L8c6558aa: *stop=0x8c6558aau;r[0]=*fpul;goto L8c6558ac; /* sts FPUL,r0 */
L8c6558ac: *stop=0x8c6558acu;r[0]<<=8;goto L8c6558ae; /* shll8 r0 */
L8c6558ae: *stop=0x8c6558aeu;r[1]|=r[0];goto L8c6558b0; /* or r0,r1 */
L8c6558b0: *stop=0x8c6558b0u;r[0]=44u;goto L8c6558b2; /* mov #0x2c,r0 */
L8c6558b2: *stop=0x8c6558b2u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c6558b4; /* fmov.s @(r0,r14),fr3 */
L8c6558b4: *stop=0x8c6558b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6558b6; /* fmul fr4,fr3 */
L8c6558b6: *stop=0x8c6558b6u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6558b8; /* ftrc fr3,FPUL */
L8c6558b8: *stop=0x8c6558b8u;r[0]=*fpul;goto L8c6558ba; /* sts FPUL,r0 */
L8c6558ba: *stop=0x8c6558bau;r[1]|=r[0];goto L8c6558bc; /* or r0,r1 */
L8c6558bc: *stop=0x8c6558bcu;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[1],4);goto L8c6558be; /* mov.l r1,@r4 */
L8c6558be: *stop=0x8c6558c0u;r[4]+=12u;goto L8c6558ca; /* bra 0x8c6558ca */
L8c6558c2: *stop=0x8c6558c2u;r[0]=64u;goto L8c6558c4; /* mov #0x40,r0 */
L8c6558c4: *stop=0x8c6558c4u;if(!valid_ram(r[0]+r[6],4u))return -3;r[1]=rd(m,r[6]+r[0],4);goto L8c6558c6; /* mov.l @(r0,r6),r1 */
L8c6558c6: *stop=0x8c6558c6u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[1],4);goto L8c6558c8; /* mov.l r1,@r4 */
L8c6558c8: *stop=0x8c6558c8u;r[4]+=28u;goto L8c6558ca; /* add #0x1c,r4 */
L8c6558ca: *stop=0x8c6558cau;if(!valid_ram(r[7],4u))return -3;r[3]=rd(m,r[7],4);goto L8c6558cc; /* mov.l @r7,r3 */
L8c6558cc: *stop=0x8c6558ccu;r[4]-=r[5];goto L8c6558ce; /* sub r5,r4 */
L8c6558ce: *stop=0x8c6558ceu;r[6]=rd(m,0x8c6558e4u,4);goto L8c6558d0; /* mov.l 0x8c6558e4,r6 */
L8c6558d0: *stop=0x8c6558d0u;r[0]=0u;goto L8c6558d2; /* mov #0x0,r0 */
L8c6558d2: *stop=0x8c6558d2u;if(!valid_ram(r[6],4u))return -3;wr(m,r[6],r[3],4);goto L8c6558d4; /* mov.l r3,@r6 */
L8c6558d4: *stop=0x8c6558d4u;if(!valid_ram(r[6]+4u,4u))return -3;wr(m,r[6]+0x4u,r[4],4);goto L8c6558d6; /* mov.l r4,@(0x4,r6) */
L8c6558d6: *stop=0x8c6558d6u;if(*pr!=entry_pr)return -3;*stop=0x8c6558d8u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_render_visibility_z_8c64f5bc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64f5bc;
L8c64f5bc: *stop=0x8c64f5bcu;r[7]=rd(m,0x8c64f5e4u,4);goto L8c64f5be; /* mov.l 0x8c64f5e4,r7 */
L8c64f5be: *stop=0x8c64f5beu;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c64f5c0; /* cmp/pl r5 */
L8c64f5c0: *stop=0x8c64f5c0u;r[6]=0u;goto L8c64f5c2; /* mov #0x0,r6 */
L8c64f5c2: take=!(*sr&1u);*stop=0x8c64f5c4u;if(!valid_ram(r[4],4u))return -3;r[4]=rd(m,r[4],4);if(take)goto L8c64f5dc;goto L8c64f5c6; /* bf/s 0x8c64f5dc */
L8c64f5c6: *stop=0x8c64f5c6u;r[0]=8u;goto L8c64f5c8; /* mov #0x8,r0 */
L8c64f5c8: *stop=0x8c64f5c8u;if(!valid_ram(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[7]);goto L8c64f5ca; /* fmov.s @r7,fr3 */
L8c64f5ca: *stop=0x8c64f5cau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto L8c64f5cc; /* fmov.s @(r0,r4),fr2 */
L8c64f5cc: *stop=0x8c64f5ccu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c64f5ce; /* fcmp/gt fr3,fr2 */
L8c64f5ce: if(!(*sr&1u))goto L8c64f5d4;goto L8c64f5d0; /* bf 0x8c64f5d4 */
L8c64f5d0: *stop=0x8c64f5d0u;if(*pr!=entry_pr)return -3;*stop=0x8c64f5d2u;r[0]=0u;return 0; /* rts */
L8c64f5d4: *stop=0x8c64f5d4u;r[6]+=1u;goto L8c64f5d6; /* add #0x1,r6 */
L8c64f5d6: *stop=0x8c64f5d6u;*sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[5]);goto L8c64f5d8; /* cmp/ge r5,r6 */
L8c64f5d8: take=!(*sr&1u);*stop=0x8c64f5dau;r[4]+=12u;if(take)goto L8c64f5c6;goto L8c64f5dc; /* bf/s 0x8c64f5c6 */
L8c64f5dc: *stop=0x8c64f5dcu;r[0]=1u;goto L8c64f5de; /* mov #0x1,r0 */
L8c64f5de: *stop=0x8c64f5deu;if(*pr!=entry_pr)return -3;*stop=0x8c64f5e0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_render_size_code_8c654b60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c654b60;
L8c654b60: *stop=0x8c654b60u;r[0]=r[4];goto L8c654b62; /* mov r4,r0 */
L8c654b62: *stop=0x8c654b62u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c654b64; /* cmp/eq #0x8,r0 */
L8c654b64: if(!(*sr&1u))goto L8c654b6a;goto L8c654b66; /* bf 0x8c654b6a */
L8c654b66: *stop=0x8c654b68u;r[4]=0u;goto L8c654bdc; /* bra 0x8c654bdc */
L8c654b6a: *stop=0x8c654b6au;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c654b6c; /* cmp/eq #0x10,r0 */
L8c654b6c: if(!(*sr&1u))goto L8c654b72;goto L8c654b6e; /* bf 0x8c654b72 */
L8c654b6e: *stop=0x8c654b70u;r[4]=1u;goto L8c654bdc; /* bra 0x8c654bdc */
L8c654b72: *stop=0x8c654b72u;r[0]=r[4];goto L8c654b74; /* mov r4,r0 */
L8c654b74: *stop=0x8c654b74u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c654b76; /* cmp/eq #0x20,r0 */
L8c654b76: if(!(*sr&1u))goto L8c654b7c;goto L8c654b78; /* bf 0x8c654b7c */
L8c654b78: *stop=0x8c654b7au;r[4]=2u;goto L8c654bdc; /* bra 0x8c654bdc */
L8c654b7c: *stop=0x8c654b7cu;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c654b7e; /* cmp/eq #0x40,r0 */
L8c654b7e: if(!(*sr&1u))goto L8c654b84;goto L8c654b80; /* bf 0x8c654b84 */
L8c654b80: *stop=0x8c654b82u;r[4]=3u;goto L8c654bdc; /* bra 0x8c654bdc */
L8c654b84: *stop=0x8c654b84u;r[3]=sx(m,0x8c654ba0u,2);goto L8c654b86; /* mov.w 0x8c654ba0,r3 */
L8c654b86: *stop=0x8c654b86u;*sr=(*sr&~1u)|(r[3]==r[4]);goto L8c654b88; /* cmp/eq r3,r4 */
L8c654b88: if(!(*sr&1u))goto L8c654bc0;goto L8c654b8a; /* bf 0x8c654bc0 */
L8c654b8a: *stop=0x8c654b8cu;r[4]=4u;goto L8c654bdc; /* bra 0x8c654bdc */
L8c654bc0: *stop=0x8c654bc0u;r[2]=sx(m,0x8c654be0u,2);goto L8c654bc2; /* mov.w 0x8c654be0,r2 */
L8c654bc2: *stop=0x8c654bc2u;*sr=(*sr&~1u)|(r[2]==r[4]);goto L8c654bc4; /* cmp/eq r2,r4 */
L8c654bc4: if(!(*sr&1u))goto L8c654bca;goto L8c654bc6; /* bf 0x8c654bca */
L8c654bc6: *stop=0x8c654bc8u;r[4]=5u;goto L8c654bdc; /* bra 0x8c654bdc */
L8c654bca: *stop=0x8c654bcau;r[1]=sx(m,0x8c654be2u,2);goto L8c654bcc; /* mov.w 0x8c654be2,r1 */
L8c654bcc: *stop=0x8c654bccu;*sr=(*sr&~1u)|(r[1]==r[4]);goto L8c654bce; /* cmp/eq r1,r4 */
L8c654bce: if(!(*sr&1u))goto L8c654bd4;goto L8c654bd0; /* bf 0x8c654bd4 */
L8c654bd0: *stop=0x8c654bd2u;r[4]=6u;goto L8c654bdc; /* bra 0x8c654bdc */
L8c654bd4: *stop=0x8c654bd4u;r[3]=sx(m,0x8c654be4u,2);goto L8c654bd6; /* mov.w 0x8c654be4,r3 */
L8c654bd6: *stop=0x8c654bd6u;*sr=(*sr&~1u)|(r[3]==r[4]);goto L8c654bd8; /* cmp/eq r3,r4 */
L8c654bd8: if(!(*sr&1u))goto L8c654bdc;goto L8c654bda; /* bf 0x8c654bdc */
L8c654bda: *stop=0x8c654bdau;r[4]=7u;goto L8c654bdc; /* mov #0x7,r4 */
L8c654bdc: *stop=0x8c654bdcu;if(*pr!=entry_pr)return -3;*stop=0x8c654bdeu;r[0]=r[4];return 0; /* rts */
}
