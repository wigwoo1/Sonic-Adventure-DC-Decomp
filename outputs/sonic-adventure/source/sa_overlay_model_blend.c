/* AL_MAIN model morph and word-copy dispatcher; names inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_add_sub.h"
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
SA_STAGE_EXPORT int sa_overlay_model_blend_0c904e80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc904e80;
Lc904e80: *stop=0xc904e80u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto Lc904e82; /* mov.l r14,@-r15 */
Lc904e82: *stop=0xc904e82u;r[0]=4294967295u;goto Lc904e84; /* mov #-0x1,r0 */
Lc904e84: *stop=0xc904e84u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto Lc904e86; /* mov.l r13,@-r15 */
Lc904e86: *stop=0xc904e86u;r[0]<<=8;goto Lc904e88; /* shll8 r0 */
Lc904e88: *stop=0xc904e88u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto Lc904e8a; /* mov.l r12,@-r15 */
Lc904e8a: *stop=0xc904e8au;r[0]+=92u;goto Lc904e8c; /* add #0x5c,r0 */
Lc904e8c: *stop=0xc904e8cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto Lc904e8e; /* mov.l r11,@-r15 */
Lc904e8e: *stop=0xc904e8eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto Lc904e90; /* mov.l r10,@-r15 */
Lc904e90: *stop=0xc904e90u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto Lc904e92; /* mov.l r9,@-r15 */
Lc904e92: *stop=0xc904e92u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto Lc904e94; /* mov.l r8,@-r15 */
Lc904e94: *stop=0xc904e94u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto Lc904e96; /* fmov.s fr15,@-r15 */
Lc904e96: *stop=0xc904e96u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[15]);goto Lc904e98; /* fmov.s fr14,@-r15 */
Lc904e98: *stop=0xc904e98u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[15]);goto Lc904e9a; /* fmov.s fr13,@-r15 */
Lc904e9a: *stop=0xc904e9au;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[15]);goto Lc904e9c; /* fmov.s fr12,@-r15 */
Lc904e9c: *stop=0xc904e9cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto Lc904e9e; /* sts.l PR,@-r15 */
Lc904e9e: *stop=0xc904e9eu;r[15]+=r[0];goto Lc904ea0; /* add r0,r15 */
Lc904ea0: *stop=0xc904ea0u;r[0]=4294967276u;goto Lc904ea2; /* mov #-0x14,r0 */
Lc904ea2: *stop=0xc904ea2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto Lc904ea4; /* fldi0 fr3 */
Lc904ea4: *stop=0xc904ea4u;r[0]=r[0]&255u;goto Lc904ea6; /* extu.b r0,r0 */
Lc904ea6: *stop=0xc904ea6u;if(!valid_ram(r[0]+r[5],4u))return -3;r[4]=rd(m,r[5]+r[0],4);goto Lc904ea8; /* mov.l @(r0,r5),r4 */
Lc904ea8: *stop=0xc904ea8u;r[0]=32u;goto Lc904eaa; /* mov #0x20,r0 */
Lc904eaa: *stop=0xc904eaau;r[3]=4294967244u;goto Lc904eac; /* mov #-0x34,r3 */
Lc904eac: *stop=0xc904eacu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[4]);goto Lc904eae; /* fmov.s @(r0,r4),fr5 */
Lc904eae: *stop=0xc904eaeu;r[0]=36u;goto Lc904eb0; /* mov #0x24,r0 */
Lc904eb0: *stop=0xc904eb0u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[4]);goto Lc904eb2; /* fmov.s @(r0,r4),fr6 */
Lc904eb2: *stop=0xc904eb2u;r[0]=40u;goto Lc904eb4; /* mov #0x28,r0 */
Lc904eb4: *stop=0xc904eb4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[0]+r[4]);goto Lc904eb6; /* fmov.s @(r0,r4),fr9 */
Lc904eb6: *stop=0xc904eb6u;r[4]=2u;goto Lc904eb8; /* mov #0x2,r4 */
Lc904eb8: *stop=0xc904eb8u;r[4]<<=8;goto Lc904eba; /* shll8 r4 */
Lc904eba: *stop=0xc904ebau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto Lc904ebc; /* fcmp/eq fr3,fr5 */
Lc904ebc: *stop=0xc904ebcu;r[2]=101u;goto Lc904ebe; /* mov #0x65,r2 */
Lc904ebe: *stop=0xc904ebeu;r[4]+=72u;goto Lc904ec0; /* add #0x48,r4 */
Lc904ec0: *stop=0xc904ec0u;r[2]<<=2;goto Lc904ec2; /* shll2 r2 */
Lc904ec2: *stop=0xc904ec2u;r[3]=r[3]&255u;goto Lc904ec4; /* extu.b r3,r3 */
Lc904ec4: *stop=0xc904ec4u;r[4]+=r[5];goto Lc904ec6; /* add r5,r4 */
Lc904ec6: *stop=0xc904ec6u;r[2]+=r[4];goto Lc904ec8; /* add r4,r2 */
Lc904ec8: *stop=0xc904ec8u;r[13]=r[4];goto Lc904eca; /* mov r4,r13 */
Lc904eca: *stop=0xc904ecau;r[3]+=r[4];goto Lc904ecc; /* add r4,r3 */
Lc904ecc: *stop=0xc904eccu;if(!valid_ram(r[15]+36u,4u))return -3;wr(m,r[15]+0x24u,r[3],4);goto Lc904ece; /* mov.l r3,@(0x24,r15) */
Lc904ece: *stop=0xc904eceu;r[0]=0x0c904f20u;goto Lc904ed0; /* mova 0x0c904f20,r0 */
Lc904ed0: *stop=0xc904ed0u;r[13]+=4u;goto Lc904ed2; /* add #0x4,r13 */
Lc904ed2: *stop=0xc904ed2u;if(!valid_ram(r[15]+40u,4u))return -3;wr(m,r[15]+0x28u,r[2],4);goto Lc904ed4; /* mov.l r2,@(0x28,r15) */
Lc904ed4: take=!(*sr&1u);*stop=0xc904ed6u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]);if(take)goto Lc904eda;goto Lc904ed8; /* bf/s 0x0c904eda */
Lc904ed8: *stop=0xc904ed8u;fcopy(f,5u,4u);goto Lc904eda; /* fmov fr4,fr5 */
Lc904eda: *stop=0xc904edau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto Lc904edc; /* fldi0 fr3 */
Lc904edc: *stop=0xc904edcu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][6],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto Lc904ede; /* fcmp/eq fr3,fr6 */
Lc904ede: take=!(*sr&1u);*stop=0xc904ee0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;if(take)goto Lc904ee4;goto Lc904ee2; /* bf/s 0x0c904ee4 */
Lc904ee2: *stop=0xc904ee2u;fcopy(f,6u,4u);goto Lc904ee4; /* fmov fr4,fr6 */
Lc904ee4: *stop=0xc904ee4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc904ee6; /* fcmp/gt fr3,fr5 */
Lc904ee6: take=!(*sr&1u);*stop=0xc904ee8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;if(take)goto Lc904ef4;goto Lc904eea; /* bf/s 0x0c904ef4 */
Lc904eea: *stop=0xc904eeau;r[8]=3u;goto Lc904eec; /* mov #0x3,r8 */
Lc904eec: *stop=0xc904eecu;r[8]<<=8;goto Lc904eee; /* shll8 r8 */
Lc904eee: *stop=0xc904eeeu;r[8]+=36u;goto Lc904ef0; /* add #0x24,r8 */
Lc904ef0: *stop=0xc904ef2u;r[8]+=r[4];goto Lc904efe; /* bra 0x0c904efe */
Lc904ef4: *stop=0xc904ef4u;r[8]=4u;goto Lc904ef6; /* mov #0x4,r8 */
Lc904ef6: *stop=0xc904ef6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]^=0x80000000u;goto Lc904ef8; /* fneg fr5 */
Lc904ef8: *stop=0xc904ef8u;r[8]<<=8;goto Lc904efa; /* shll8 r8 */
Lc904efa: *stop=0xc904efau;r[8]+=4294967276u;goto Lc904efc; /* add #-0x14,r8 */
Lc904efc: *stop=0xc904efcu;r[8]+=r[4];goto Lc904efe; /* add r4,r8 */
Lc904efe: *stop=0xc904efeu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][6],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc904f00; /* fcmp/gt fr3,fr6 */
Lc904f00: take=!(*sr&1u);*stop=0xc904f02u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][14]=0x3f800000u;if(take)goto Lc904f40;goto Lc904f04; /* bf/s 0x0c904f40 */
Lc904f04: *stop=0xc904f04u;r[10]=2u;goto Lc904f06; /* mov #0x2,r10 */
Lc904f06: *stop=0xc904f06u;r[10]<<=8;goto Lc904f08; /* shll8 r10 */
Lc904f08: *stop=0xc904f08u;r[10]+=92u;goto Lc904f0a; /* add #0x5c,r10 */
Lc904f0a: *stop=0xc904f0cu;r[10]+=r[4];goto Lc904f4a; /* bra 0x0c904f4a */
Lc904f40: *stop=0xc904f40u;r[10]=5u;goto Lc904f42; /* mov #0x5,r10 */
Lc904f42: *stop=0xc904f42u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]^=0x80000000u;goto Lc904f44; /* fneg fr6 */
Lc904f44: *stop=0xc904f44u;r[10]<<=8;goto Lc904f46; /* shll8 r10 */
Lc904f46: *stop=0xc904f46u;r[10]+=4294967220u;goto Lc904f48; /* add #-0x4c,r10 */
Lc904f48: *stop=0xc904f48u;r[10]+=r[4];goto Lc904f4a; /* add r4,r10 */
Lc904f4a: *stop=0xc904f4au;r[3]=rd(m,0x0c905220u,4);goto Lc904f4c; /* mov.l 0x0c905220,r3 */
Lc904f4c: *stop=0xc904f4cu;r[2]=r[15];goto Lc904f4e; /* mov r15,r2 */
Lc904f4e: *stop=0xc904f4eu;r[2]+=108u;goto Lc904f50; /* add #0x6c,r2 */
Lc904f50: *stop=0xc904f50u;fcopy(f,7u,14u);goto Lc904f52; /* fmov fr14,fr7 */
Lc904f52: *stop=0xc904f52u;if(!valid_ram(r[3]+4u,4u))return -3;r[0]=rd(m,r[3]+0x4u,4);goto Lc904f54; /* mov.l @(0x4,r3),r0 */
Lc904f54: *stop=0xc904f54u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[6],&f->fpscr,&fr[7],1);if(status)return status;}goto Lc904f56; /* fsub fr6,fr7 */
Lc904f56: *stop=0xc904f56u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto Lc904f58; /* mov.l @r3,r1 */
Lc904f58: *stop=0xc904f58u;fcopy(f,8u,14u);goto Lc904f5a; /* fmov fr14,fr8 */
Lc904f5a: *stop=0xc904f5au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[5],&f->fpscr,&fr[8],1);if(status)return status;}goto Lc904f5c; /* fsub fr5,fr8 */
Lc904f5c: *stop=0xc904f5cu;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[1],4);goto Lc904f5e; /* mov.l r1,@r2 */
Lc904f5e: *stop=0xc904f5eu;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[0],4);goto Lc904f60; /* mov.l r0,@(0x4,r2) */
Lc904f60: *stop=0xc904f60u;if(!valid_ram(r[3]+12u,4u))return -3;r[0]=rd(m,r[3]+0xcu,4);goto Lc904f62; /* mov.l @(0xc,r3),r0 */
Lc904f62: *stop=0xc904f62u;if(!valid_ram(r[3]+8u,4u))return -3;r[1]=rd(m,r[3]+0x8u,4);goto Lc904f64; /* mov.l @(0x8,r3),r1 */
Lc904f64: *stop=0xc904f64u;fcopy(f,13u,14u);goto Lc904f66; /* fmov fr14,fr13 */
Lc904f66: *stop=0xc904f66u;if(!valid_ram(r[2]+8u,4u))return -3;wr(m,r[2]+0x8u,r[1],4);goto Lc904f68; /* mov.l r1,@(0x8,r2) */
Lc904f68: *stop=0xc904f68u;if(!valid_ram(r[2]+12u,4u))return -3;wr(m,r[2]+0xcu,r[0],4);goto Lc904f6a; /* mov.l r0,@(0xc,r2) */
Lc904f6a: *stop=0xc904f6au;if(!valid_ram(r[3]+20u,4u))return -3;r[0]=rd(m,r[3]+0x14u,4);goto Lc904f6c; /* mov.l @(0x14,r3),r0 */
Lc904f6c: *stop=0xc904f6cu;if(!valid_ram(r[3]+16u,4u))return -3;r[1]=rd(m,r[3]+0x10u,4);goto Lc904f6e; /* mov.l @(0x10,r3),r1 */
Lc904f6e: *stop=0xc904f6eu;if(!valid_ram(r[2]+16u,4u))return -3;wr(m,r[2]+0x10u,r[1],4);goto Lc904f70; /* mov.l r1,@(0x10,r2) */
Lc904f70: *stop=0xc904f70u;if(!valid_ram(r[2]+20u,4u))return -3;wr(m,r[2]+0x14u,r[0],4);goto Lc904f72; /* mov.l r0,@(0x14,r2) */
Lc904f72: *stop=0xc904f72u;if(!valid_ram(r[3]+28u,4u))return -3;r[0]=rd(m,r[3]+0x1cu,4);goto Lc904f74; /* mov.l @(0x1c,r3),r0 */
Lc904f74: *stop=0xc904f74u;if(!valid_ram(r[3]+24u,4u))return -3;r[1]=rd(m,r[3]+0x18u,4);goto Lc904f76; /* mov.l @(0x18,r3),r1 */
Lc904f76: *stop=0xc904f76u;if(!valid_ram(r[2]+24u,4u))return -3;wr(m,r[2]+0x18u,r[1],4);goto Lc904f78; /* mov.l r1,@(0x18,r2) */
Lc904f78: *stop=0xc904f78u;if(!valid_ram(r[2]+28u,4u))return -3;wr(m,r[2]+0x1cu,r[0],4);goto Lc904f7a; /* mov.l r0,@(0x1c,r2) */
Lc904f7a: *stop=0xc904f7au;if(!valid_ram(r[3]+36u,4u))return -3;r[0]=rd(m,r[3]+0x24u,4);goto Lc904f7c; /* mov.l @(0x24,r3),r0 */
Lc904f7c: *stop=0xc904f7cu;if(!valid_ram(r[3]+32u,4u))return -3;r[1]=rd(m,r[3]+0x20u,4);goto Lc904f7e; /* mov.l @(0x20,r3),r1 */
Lc904f7e: *stop=0xc904f7eu;if(!valid_ram(r[2]+32u,4u))return -3;wr(m,r[2]+0x20u,r[1],4);goto Lc904f80; /* mov.l r1,@(0x20,r2) */
Lc904f80: *stop=0xc904f80u;if(!valid_ram(r[2]+36u,4u))return -3;wr(m,r[2]+0x24u,r[0],4);goto Lc904f82; /* mov.l r0,@(0x24,r2) */
Lc904f82: *stop=0xc904f82u;if(!valid_ram(r[3]+44u,4u))return -3;r[0]=rd(m,r[3]+0x2cu,4);goto Lc904f84; /* mov.l @(0x2c,r3),r0 */
Lc904f84: *stop=0xc904f84u;if(!valid_ram(r[3]+40u,4u))return -3;r[1]=rd(m,r[3]+0x28u,4);goto Lc904f86; /* mov.l @(0x28,r3),r1 */
Lc904f86: *stop=0xc904f86u;if(!valid_ram(r[2]+40u,4u))return -3;wr(m,r[2]+0x28u,r[1],4);goto Lc904f88; /* mov.l r1,@(0x28,r2) */
Lc904f88: *stop=0xc904f88u;if(!valid_ram(r[2]+44u,4u))return -3;wr(m,r[2]+0x2cu,r[0],4);goto Lc904f8a; /* mov.l r0,@(0x2c,r2) */
Lc904f8a: *stop=0xc904f8au;if(!valid_ram(r[3]+52u,4u))return -3;r[0]=rd(m,r[3]+0x34u,4);goto Lc904f8c; /* mov.l @(0x34,r3),r0 */
Lc904f8c: *stop=0xc904f8cu;if(!valid_ram(r[3]+48u,4u))return -3;r[1]=rd(m,r[3]+0x30u,4);goto Lc904f8e; /* mov.l @(0x30,r3),r1 */
Lc904f8e: *stop=0xc904f8eu;if(!valid_ram(r[2]+48u,4u))return -3;wr(m,r[2]+0x30u,r[1],4);goto Lc904f90; /* mov.l r1,@(0x30,r2) */
Lc904f90: *stop=0xc904f90u;if(!valid_ram(r[2]+52u,4u))return -3;wr(m,r[2]+0x34u,r[0],4);goto Lc904f92; /* mov.l r0,@(0x34,r2) */
Lc904f92: *stop=0xc904f92u;r[0]=4294967276u;goto Lc904f94; /* mov #-0x14,r0 */
Lc904f94: *stop=0xc904f94u;r[0]=r[0]&255u;goto Lc904f96; /* extu.b r0,r0 */
Lc904f96: *stop=0xc904f96u;r[2]=14u;goto Lc904f98; /* mov #0xe,r2 */
Lc904f98: *stop=0xc904f98u;if(!valid_ram(r[15]+60u,4u))return -3;wr(m,r[15]+0x3cu,r[2],4);goto Lc904f9a; /* mov.l r2,@(0x3c,r15) */
Lc904f9a: *stop=0xc904f9au;if(!valid_ram(r[0]+r[5],4u))return -3;r[1]=rd(m,r[5]+r[0],4);goto Lc904f9c; /* mov.l @(r0,r5),r1 */
Lc904f9c: *stop=0xc904f9cu;if(!valid_ram(r[1],1u))return -3;r[2]=sx(m,r[1],1);goto Lc904f9e; /* mov.b @r1,r2 */
Lc904f9e: *stop=0xc904f9eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto Lc904fa0; /* tst r2,r2 */
Lc904fa0: take=!(*sr&1u);*stop=0xc904fa2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[9],&f->fpscr,&fr[13],1);if(status)return status;}if(take)goto Lc904fa8;goto Lc904fa4; /* bf/s 0x0c904fa8 */
Lc904fa4: *stop=0xc904fa4u;r[2]=12u;goto Lc904fa6; /* mov #0xc,r2 */
Lc904fa6: *stop=0xc904fa6u;if(!valid_ram(r[15]+60u,4u))return -3;wr(m,r[15]+0x3cu,r[2],4);goto Lc904fa8; /* mov.l r2,@(0x3c,r15) */
Lc904fa8: *stop=0xc904fa8u;fcopy(f,3u,5u);goto Lc904faa; /* fmov fr5,fr3 */
Lc904faa: *stop=0xc904faau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],0);if(status)return status;}goto Lc904fac; /* fadd fr6,fr3 */
Lc904fac: *stop=0xc904facu;r[0]=48u;goto Lc904fae; /* mov #0x30,r0 */
Lc904fae: *stop=0xc904faeu;r[6]=0u;goto Lc904fb0; /* mov #0x0,r6 */
Lc904fb0: *stop=0xc904fb0u;if(!valid_ram(r[15]+32u,4u))return -3;wr(m,r[15]+0x20u,r[6],4);goto Lc904fb2; /* mov.l r6,@(0x20,r15) */
Lc904fb2: *stop=0xc904fb2u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto Lc904fb4; /* fmov fr3,@(r0,r15) */
Lc904fb4: *stop=0xc904fb4u;r[0]=0x0c905224u;goto Lc904fb6; /* mova 0x0c905224,r0 */
Lc904fb6: *stop=0xc904fb6u;if(!valid_ram(r[15]+60u,4u))return -3;r[3]=rd(m,r[15]+0x3cu,4);goto Lc904fb8; /* mov.l @(0x3c,r15),r3 */
Lc904fb8: *stop=0xc904fb8u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]);goto Lc904fba; /* fmov.s @r0,fr4 */
Lc904fba: *stop=0xc904fbau;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto Lc904fbc; /* cmp/pl r3 */
Lc904fbc: take=(*sr&1u);*stop=0xc904fbeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]=0;if(take)goto Lc904fc4;goto Lc904fc0; /* bt/s 0x0c904fc4 */
Lc904fc0: *stop=0xc904fc2u;goto Lc90544a; /* bra 0x0c90544a */
Lc904fc4: *stop=0xc904fc4u;r[0]=48u;goto Lc904fc6; /* mov #0x30,r0 */
Lc904fc6: *stop=0xc904fc6u;fcopy(f,12u,14u);goto Lc904fc8; /* fmov fr14,fr12 */
Lc904fc8: *stop=0xc904fc8u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto Lc904fca; /* fmov.s @(r0,r15),fr3 */
Lc904fca: *stop=0xc904fcau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc904fcc; /* fldi0 fr2 */
Lc904fcc: *stop=0xc904fccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[12],&fr[3],&f->fpscr,&fr[12]);if(status)return status;}goto Lc904fce; /* fdiv fr3,fr12 */
Lc904fce: *stop=0xc904fceu;fcopy(f,11u,5u);goto Lc904fd0; /* fmov fr5,fr11 */
Lc904fd0: *stop=0xc904fd0u;fcopy(f,15u,6u);goto Lc904fd2; /* fmov fr6,fr15 */
Lc904fd2: *stop=0xc904fd2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,0);if(status)return status;goto Lc904fd4; /* fcmp/eq fr2,fr12 */
Lc904fd4: *stop=0xc904fd4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[12],&f->fpscr,&fr[11]);if(status)return status;}goto Lc904fd6; /* fmul fr12,fr11 */
Lc904fd6: take=!(*sr&1u);*stop=0xc904fd8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[12],&f->fpscr,&fr[15]);if(status)return status;}if(take)goto Lc904fde;goto Lc904fda; /* bf/s 0x0c904fde */
Lc904fda: *stop=0xc904fdcu;goto Lc90543a; /* bra 0x0c90543a */
Lc904fde: *stop=0xc904fdeu;if(!valid_ram(r[15]+32u,4u))return -3;r[2]=rd(m,r[15]+0x20u,4);goto Lc904fe0; /* mov.l @(0x20,r15),r2 */
Lc904fe0: *stop=0xc904fe0u;r[3]=r[15];goto Lc904fe2; /* mov r15,r3 */
Lc904fe2: *stop=0xc904fe2u;r[3]+=108u;goto Lc904fe4; /* add #0x6c,r3 */
Lc904fe4: *stop=0xc904fe4u;r[2]<<=2;goto Lc904fe6; /* shll2 r2 */
Lc904fe6: *stop=0xc904fe6u;r[2]+=r[3];goto Lc904fe8; /* add r3,r2 */
Lc904fe8: *stop=0xc904fe8u;if(!valid_ram(r[2],4u))return -3;r[1]=rd(m,r[2],4);goto Lc904fea; /* mov.l @r2,r1 */
Lc904fea: *stop=0xc904feau;r[14]=r[1];goto Lc904fec; /* mov r1,r14 */
Lc904fec: *stop=0xc904fecu;r[14]<<=2;goto Lc904fee; /* shll2 r14 */
Lc904fee: *stop=0xc904feeu;r[0]=r[14];goto Lc904ff0; /* mov r14,r0 */
Lc904ff0: *stop=0xc904ff0u;if(!valid_ram(r[15]+44u,4u))return -3;wr(m,r[15]+0x2cu,r[1],4);goto Lc904ff2; /* mov.l r1,@(0x2c,r15) */
Lc904ff2: *stop=0xc904ff2u;if(!valid_ram(r[0]+r[13],4u))return -3;r[2]=rd(m,r[13]+r[0],4);goto Lc904ff4; /* mov.l @(r0,r13),r2 */
Lc904ff4: *stop=0xc904ff4u;if(!valid_ram(r[2]+4u,4u))return -3;r[2]=rd(m,r[2]+0x4u,4);goto Lc904ff6; /* mov.l @(0x4,r2),r2 */
Lc904ff6: *stop=0xc904ff6u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto Lc904ff8; /* tst r2,r2 */
Lc904ff8: if(!(*sr&1u))goto Lc904ffe;goto Lc904ffa; /* bf 0x0c904ffe */
Lc904ffa: *stop=0xc904ffcu;goto Lc905528; /* bra 0x0c905528 */
Lc904ffe: *stop=0xc904ffeu;r[0]=r[14];goto Lc905000; /* mov r14,r0 */
Lc905000: *stop=0xc905000u;if(!valid_ram(r[0]+r[13],4u))return -3;r[4]=rd(m,r[13]+r[0],4);goto Lc905002; /* mov.l @(r0,r13),r4 */
Lc905002: *stop=0xc905002u;r[3]=r[15];goto Lc905004; /* mov r15,r3 */
Lc905004: *stop=0xc905004u;r[3]+=72u;goto Lc905006; /* add #0x48,r3 */
Lc905006: *stop=0xc905006u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[4],4);goto Lc905008; /* mov.l r4,@r3 */
Lc905008: *stop=0xc905008u;r[2]=0u;goto Lc90500a; /* mov #0x0,r2 */
Lc90500a: *stop=0xc90500au;if(!valid_ram(r[4]+4u,4u))return -3;r[4]=rd(m,r[4]+0x4u,4);goto Lc90500c; /* mov.l @(0x4,r4),r4 */
Lc90500c: *stop=0xc90500cu;if(!valid_ram(r[4]+8u,4u))return -3;r[3]=rd(m,r[4]+0x8u,4);goto Lc90500e; /* mov.l @(0x8,r4),r3 */
Lc90500e: *stop=0xc90500eu;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[3],4);goto Lc905010; /* mov.l r3,@(0x8,r15) */
Lc905010: *stop=0xc905010u;if(!valid_ram(r[15]+40u,4u))return -3;r[0]=rd(m,r[15]+0x28u,4);goto Lc905012; /* mov.l @(0x28,r15),r0 */
Lc905012: *stop=0xc905012u;if(!valid_ram(r[4],4u))return -3;r[4]=rd(m,r[4],4);goto Lc905014; /* mov.l @r4,r4 */
Lc905014: *stop=0xc905014u;if(!valid_ram(r[0]+r[14],4u))return -3;r[12]=rd(m,r[14]+r[0],4);goto Lc905016; /* mov.l @(r0,r14),r12 */
Lc905016: *stop=0xc905016u;r[0]=r[14];goto Lc905018; /* mov r14,r0 */
Lc905018: *stop=0xc905018u;if(!valid_ram(r[0]+r[8],4u))return -3;r[11]=rd(m,r[8]+r[0],4);goto Lc90501a; /* mov.l @(r0,r8),r11 */
Lc90501a: *stop=0xc90501au;if(!valid_ram(r[0]+r[10],4u))return -3;r[9]=rd(m,r[10]+r[0],4);goto Lc90501c; /* mov.l @(r0,r10),r9 */
Lc90501c: *stop=0xc90501cu;if(!valid_ram(r[15]+36u,4u))return -3;r[0]=rd(m,r[15]+0x24u,4);goto Lc90501e; /* mov.l @(0x24,r15),r0 */
Lc90501e: *stop=0xc90501eu;if(!valid_ram(r[9]+4u,4u))return -3;r[9]=rd(m,r[9]+0x4u,4);goto Lc905020; /* mov.l @(0x4,r9),r9 */
Lc905020: *stop=0xc905020u;if(!valid_ram(r[0]+r[14],4u))return -3;r[7]=rd(m,r[14]+r[0],4);goto Lc905022; /* mov.l @(r0,r14),r7 */
Lc905022: *stop=0xc905022u;if(!valid_ram(r[11]+4u,4u))return -3;r[11]=rd(m,r[11]+0x4u,4);goto Lc905024; /* mov.l @(0x4,r11),r11 */
Lc905024: *stop=0xc905024u;if(!valid_ram(r[7]+4u,4u))return -3;r[7]=rd(m,r[7]+0x4u,4);goto Lc905026; /* mov.l @(0x4,r7),r7 */
Lc905026: *stop=0xc905026u;if(!valid_ram(r[12]+4u,4u))return -3;r[12]=rd(m,r[12]+0x4u,4);goto Lc905028; /* mov.l @(0x4,r12),r12 */
Lc905028: *stop=0xc905028u;if(!valid_ram(r[7],4u))return -3;r[7]=rd(m,r[7],4);goto Lc90502a; /* mov.l @r7,r7 */
Lc90502a: *stop=0xc90502au;if(!valid_ram(r[12],4u))return -3;r[12]=rd(m,r[12],4);goto Lc90502c; /* mov.l @r12,r12 */
Lc90502c: *stop=0xc90502cu;if(!valid_ram(r[9],4u))return -3;r[9]=rd(m,r[9],4);goto Lc90502e; /* mov.l @r9,r9 */
Lc90502e: *stop=0xc90502eu;if(!valid_ram(r[11],4u))return -3;r[11]=rd(m,r[11],4);goto Lc905030; /* mov.l @r11,r11 */
Lc905030: *stop=0xc905030u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[6],4);goto Lc905032; /* mov.l r6,@r15 */
Lc905032: *stop=0xc905032u;if(!valid_ram(r[15]+8u,4u))return -3;r[3]=rd(m,r[15]+0x8u,4);goto Lc905034; /* mov.l @(0x8,r15),r3 */
Lc905034: *stop=0xc905034u;*sr=(*sr&~1u)|(r[2]>=r[3]);goto Lc905036; /* cmp/hs r3,r2 */
Lc905036: if((*sr&1u))goto Lc9050c6;goto Lc905038; /* bt 0x0c9050c6 */
Lc905038: *stop=0xc905038u;if(!valid_ram(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[9]);goto Lc90503a; /* fmov.s @r9,fr2 */
Lc90503a: *stop=0xc90503au;r[0]=4u;goto Lc90503c; /* mov #0x4,r0 */
Lc90503c: *stop=0xc90503cu;if(!valid_ram(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[12]);goto Lc90503e; /* fmov.s @r12,fr0 */
Lc90503e: *stop=0xc90503eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905040; /* fmul fr6,fr2 */
Lc905040: *stop=0xc905040u;if(!valid_ram(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[11]);goto Lc905042; /* fmov.s @r11,fr3 */
Lc905042: *stop=0xc905042u;fcopy(f,1u,15u);goto Lc905044; /* fmov fr15,fr1 */
Lc905044: *stop=0xc905044u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905046; /* fmul fr5,fr3 */
Lc905046: *stop=0xc905046u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[7],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905048; /* fmac fr0,fr7,fr2 */
Lc905048: *stop=0xc905048u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[8],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc90504a; /* fmac fr0,fr8,fr3 */
Lc90504a: *stop=0xc90504au;fcopy(f,0u,11u);goto Lc90504c; /* fmov fr11,fr0 */
Lc90504c: *stop=0xc90504cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto Lc90504e; /* fmul fr2,fr1 */
Lc90504e: *stop=0xc90504eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto Lc905050; /* fmac fr0,fr3,fr1 */
Lc905050: *stop=0xc905050u;fcopy(f,0u,8u);goto Lc905052; /* fmov fr8,fr0 */
Lc905052: *stop=0xc905052u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[4]);goto Lc905054; /* fmov.s fr1,@r4 */
Lc905054: *stop=0xc905054u;if(!valid_ram(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[11]);goto Lc905056; /* fmov.s @(r0,r11),fr3 */
Lc905056: *stop=0xc905056u;if(!valid_ram(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[9]);goto Lc905058; /* fmov.s @(r0,r9),fr2 */
Lc905058: *stop=0xc905058u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto Lc90505a; /* fmul fr5,fr3 */
Lc90505a: *stop=0xc90505au;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[0]+r[12]);goto Lc90505c; /* fmov.s @(r0,r12),fr12 */
Lc90505c: *stop=0xc90505cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto Lc90505e; /* fmul fr6,fr2 */
Lc90505e: *stop=0xc90505eu;fcopy(f,1u,15u);goto Lc905060; /* fmov fr15,fr1 */
Lc905060: *stop=0xc905060u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[12],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905062; /* fmac fr0,fr12,fr3 */
Lc905062: *stop=0xc905062u;fcopy(f,0u,7u);goto Lc905064; /* fmov fr7,fr0 */
Lc905064: *stop=0xc905064u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[12],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905066; /* fmac fr0,fr12,fr2 */
Lc905066: *stop=0xc905066u;fcopy(f,0u,11u);goto Lc905068; /* fmov fr11,fr0 */
Lc905068: *stop=0xc905068u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto Lc90506a; /* fmul fr2,fr1 */
Lc90506a: *stop=0xc90506au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto Lc90506c; /* fmac fr0,fr3,fr1 */
Lc90506c: *stop=0xc90506cu;fcopy(f,0u,8u);goto Lc90506e; /* fmov fr8,fr0 */
Lc90506e: *stop=0xc90506eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc905070; /* fmov fr1,@(r0,r4) */
Lc905070: *stop=0xc905070u;r[0]=8u;goto Lc905072; /* mov #0x8,r0 */
Lc905072: *stop=0xc905072u;if(!valid_ram(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[11]);goto Lc905074; /* fmov.s @(r0,r11),fr3 */
Lc905074: *stop=0xc905074u;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[0]+r[12]);goto Lc905076; /* fmov.s @(r0,r12),fr12 */
Lc905076: *stop=0xc905076u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905078; /* fmul fr5,fr3 */
Lc905078: *stop=0xc905078u;if(!valid_ram(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[9]);goto Lc90507a; /* fmov.s @(r0,r9),fr2 */
Lc90507a: *stop=0xc90507au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[12],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc90507c; /* fmac fr0,fr12,fr3 */
Lc90507c: *stop=0xc90507cu;fcopy(f,0u,7u);goto Lc90507e; /* fmov fr7,fr0 */
Lc90507e: *stop=0xc90507eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905080; /* fmul fr6,fr2 */
Lc905080: *stop=0xc905080u;fcopy(f,1u,15u);goto Lc905082; /* fmov fr15,fr1 */
Lc905082: *stop=0xc905082u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[12],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905084; /* fmac fr0,fr12,fr2 */
Lc905084: *stop=0xc905084u;fcopy(f,0u,11u);goto Lc905086; /* fmov fr11,fr0 */
Lc905086: *stop=0xc905086u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto Lc905088; /* fmul fr2,fr1 */
Lc905088: *stop=0xc905088u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto Lc90508a; /* fmac fr0,fr3,fr1 */
Lc90508a: *stop=0xc90508au;fcopy(f,0u,13u);goto Lc90508c; /* fmov fr13,fr0 */
Lc90508c: *stop=0xc90508cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc90508e; /* fmov fr1,@(r0,r4) */
Lc90508e: *stop=0xc90508eu;r[0]=4u;goto Lc905090; /* mov #0x4,r0 */
Lc905090: *stop=0xc905090u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);goto Lc905092; /* fmov.s @r4,fr3 */
Lc905092: *stop=0xc905092u;if(!valid_ram(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[7]);goto Lc905094; /* fmov.s @r7,fr2 */
Lc905094: *stop=0xc905094u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905096; /* fmul fr9,fr3 */
Lc905096: *stop=0xc905096u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905098; /* fmac fr0,fr2,fr3 */
Lc905098: *stop=0xc905098u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[4]);goto Lc90509a; /* fmov.s fr3,@r4 */
Lc90509a: *stop=0xc90509au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc90509c; /* fmov.s @(r0,r4),fr3 */
Lc90509c: *stop=0xc90509cu;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[7]);goto Lc90509e; /* fmov.s @(r0,r7),fr2 */
Lc90509e: *stop=0xc90509eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto Lc9050a0; /* fmul fr9,fr3 */
Lc9050a0: *stop=0xc9050a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc9050a2; /* fmac fr0,fr2,fr3 */
Lc9050a2: *stop=0xc9050a2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc9050a4; /* fmov fr3,@(r0,r4) */
Lc9050a4: *stop=0xc9050a4u;r[0]=8u;goto Lc9050a6; /* mov #0x8,r0 */
Lc9050a6: *stop=0xc9050a6u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc9050a8; /* fmov.s @(r0,r4),fr3 */
Lc9050a8: *stop=0xc9050a8u;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[7]);goto Lc9050aa; /* fmov.s @(r0,r7),fr2 */
Lc9050aa: *stop=0xc9050aau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto Lc9050ac; /* fmul fr9,fr3 */
Lc9050ac: *stop=0xc9050acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc9050ae; /* fmac fr0,fr2,fr3 */
Lc9050ae: *stop=0xc9050aeu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc9050b0; /* fmov fr3,@(r0,r4) */
Lc9050b0: *stop=0xc9050b0u;r[4]+=12u;goto Lc9050b2; /* add #0xc,r4 */
Lc9050b2: *stop=0xc9050b2u;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);goto Lc9050b4; /* mov.l @r15,r3 */
Lc9050b4: *stop=0xc9050b4u;r[3]+=1u;goto Lc9050b6; /* add #0x1,r3 */
Lc9050b6: *stop=0xc9050b6u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto Lc9050b8; /* mov.l r3,@r15 */
Lc9050b8: *stop=0xc9050b8u;if(!valid_ram(r[15]+8u,4u))return -3;r[2]=rd(m,r[15]+0x8u,4);goto Lc9050ba; /* mov.l @(0x8,r15),r2 */
Lc9050ba: *stop=0xc9050bau;r[9]+=12u;goto Lc9050bc; /* add #0xc,r9 */
Lc9050bc: *stop=0xc9050bcu;r[12]+=12u;goto Lc9050be; /* add #0xc,r12 */
Lc9050be: *stop=0xc9050beu;*sr=(*sr&~1u)|(r[3]>=r[2]);goto Lc9050c0; /* cmp/hs r2,r3 */
Lc9050c0: *stop=0xc9050c0u;r[11]+=12u;goto Lc9050c2; /* add #0xc,r11 */
Lc9050c2: take=!(*sr&1u);*stop=0xc9050c4u;r[7]+=12u;if(take)goto Lc905038;goto Lc9050c6; /* bf/s 0x0c905038 */
Lc9050c6: *stop=0xc9050c6u;r[0]=r[14];goto Lc9050c8; /* mov r14,r0 */
Lc9050c8: *stop=0xc9050c8u;if(!valid_ram(r[0]+r[13],4u))return -3;r[4]=rd(m,r[13]+r[0],4);goto Lc9050ca; /* mov.l @(r0,r13),r4 */
Lc9050ca: *stop=0xc9050cau;r[3]=r[15];goto Lc9050cc; /* mov r15,r3 */
Lc9050cc: *stop=0xc9050ccu;r[3]+=68u;goto Lc9050ce; /* add #0x44,r3 */
Lc9050ce: *stop=0xc9050ceu;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[4],4);goto Lc9050d0; /* mov.l r4,@r3 */
Lc9050d0: *stop=0xc9050d0u;r[2]=0u;goto Lc9050d2; /* mov #0x0,r2 */
Lc9050d2: *stop=0xc9050d2u;if(!valid_ram(r[4]+4u,4u))return -3;r[4]=rd(m,r[4]+0x4u,4);goto Lc9050d4; /* mov.l @(0x4,r4),r4 */
Lc9050d4: *stop=0xc9050d4u;if(!valid_ram(r[4]+8u,4u))return -3;r[3]=rd(m,r[4]+0x8u,4);goto Lc9050d6; /* mov.l @(0x8,r4),r3 */
Lc9050d6: *stop=0xc9050d6u;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[3],4);goto Lc9050d8; /* mov.l r3,@(0x8,r15) */
Lc9050d8: *stop=0xc9050d8u;if(!valid_ram(r[15]+40u,4u))return -3;r[0]=rd(m,r[15]+0x28u,4);goto Lc9050da; /* mov.l @(0x28,r15),r0 */
Lc9050da: *stop=0xc9050dau;if(!valid_ram(r[4]+4u,4u))return -3;r[4]=rd(m,r[4]+0x4u,4);goto Lc9050dc; /* mov.l @(0x4,r4),r4 */
Lc9050dc: *stop=0xc9050dcu;if(!valid_ram(r[0]+r[14],4u))return -3;r[7]=rd(m,r[14]+r[0],4);goto Lc9050de; /* mov.l @(r0,r14),r7 */
Lc9050de: *stop=0xc9050deu;r[0]=r[14];goto Lc9050e0; /* mov r14,r0 */
Lc9050e0: *stop=0xc9050e0u;if(!valid_ram(r[0]+r[8],4u))return -3;r[12]=rd(m,r[8]+r[0],4);goto Lc9050e2; /* mov.l @(r0,r8),r12 */
Lc9050e2: *stop=0xc9050e2u;if(!valid_ram(r[0]+r[10],4u))return -3;r[11]=rd(m,r[10]+r[0],4);goto Lc9050e4; /* mov.l @(r0,r10),r11 */
Lc9050e4: *stop=0xc9050e4u;if(!valid_ram(r[15]+36u,4u))return -3;r[0]=rd(m,r[15]+0x24u,4);goto Lc9050e6; /* mov.l @(0x24,r15),r0 */
Lc9050e6: *stop=0xc9050e6u;if(!valid_ram(r[11]+4u,4u))return -3;r[11]=rd(m,r[11]+0x4u,4);goto Lc9050e8; /* mov.l @(0x4,r11),r11 */
Lc9050e8: *stop=0xc9050e8u;if(!valid_ram(r[0]+r[14],4u))return -3;r[9]=rd(m,r[14]+r[0],4);goto Lc9050ea; /* mov.l @(r0,r14),r9 */
Lc9050ea: *stop=0xc9050eau;if(!valid_ram(r[12]+4u,4u))return -3;r[12]=rd(m,r[12]+0x4u,4);goto Lc9050ec; /* mov.l @(0x4,r12),r12 */
Lc9050ec: *stop=0xc9050ecu;if(!valid_ram(r[9]+4u,4u))return -3;r[9]=rd(m,r[9]+0x4u,4);goto Lc9050ee; /* mov.l @(0x4,r9),r9 */
Lc9050ee: *stop=0xc9050eeu;if(!valid_ram(r[7]+4u,4u))return -3;r[7]=rd(m,r[7]+0x4u,4);goto Lc9050f0; /* mov.l @(0x4,r7),r7 */
Lc9050f0: *stop=0xc9050f0u;if(!valid_ram(r[9]+4u,4u))return -3;r[9]=rd(m,r[9]+0x4u,4);goto Lc9050f2; /* mov.l @(0x4,r9),r9 */
Lc9050f2: *stop=0xc9050f2u;if(!valid_ram(r[7]+4u,4u))return -3;r[7]=rd(m,r[7]+0x4u,4);goto Lc9050f4; /* mov.l @(0x4,r7),r7 */
Lc9050f4: *stop=0xc9050f4u;if(!valid_ram(r[11]+4u,4u))return -3;r[11]=rd(m,r[11]+0x4u,4);goto Lc9050f6; /* mov.l @(0x4,r11),r11 */
Lc9050f6: *stop=0xc9050f6u;if(!valid_ram(r[12]+4u,4u))return -3;r[12]=rd(m,r[12]+0x4u,4);goto Lc9050f8; /* mov.l @(0x4,r12),r12 */
Lc9050f8: *stop=0xc9050f8u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[6],4);goto Lc9050fa; /* mov.l r6,@r15 */
Lc9050fa: *stop=0xc9050fau;if(!valid_ram(r[15]+8u,4u))return -3;r[3]=rd(m,r[15]+0x8u,4);goto Lc9050fc; /* mov.l @(0x8,r15),r3 */
Lc9050fc: *stop=0xc9050fcu;*sr=(*sr&~1u)|(r[2]>=r[3]);goto Lc9050fe; /* cmp/hs r3,r2 */
Lc9050fe: if((*sr&1u))goto Lc90518e;goto Lc905100; /* bt 0x0c90518e */
Lc905100: *stop=0xc905100u;if(!valid_ram(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[11]);goto Lc905102; /* fmov.s @r11,fr2 */
Lc905102: *stop=0xc905102u;r[0]=4u;goto Lc905104; /* mov #0x4,r0 */
Lc905104: *stop=0xc905104u;if(!valid_ram(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[7]);goto Lc905106; /* fmov.s @r7,fr0 */
Lc905106: *stop=0xc905106u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905108; /* fmul fr6,fr2 */
Lc905108: *stop=0xc905108u;if(!valid_ram(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[12]);goto Lc90510a; /* fmov.s @r12,fr3 */
Lc90510a: *stop=0xc90510au;fcopy(f,1u,15u);goto Lc90510c; /* fmov fr15,fr1 */
Lc90510c: *stop=0xc90510cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto Lc90510e; /* fmul fr5,fr3 */
Lc90510e: *stop=0xc90510eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[7],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905110; /* fmac fr0,fr7,fr2 */
Lc905110: *stop=0xc905110u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[8],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905112; /* fmac fr0,fr8,fr3 */
Lc905112: *stop=0xc905112u;fcopy(f,0u,11u);goto Lc905114; /* fmov fr11,fr0 */
Lc905114: *stop=0xc905114u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto Lc905116; /* fmul fr2,fr1 */
Lc905116: *stop=0xc905116u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto Lc905118; /* fmac fr0,fr3,fr1 */
Lc905118: *stop=0xc905118u;fcopy(f,0u,8u);goto Lc90511a; /* fmov fr8,fr0 */
Lc90511a: *stop=0xc90511au;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[4]);goto Lc90511c; /* fmov.s fr1,@r4 */
Lc90511c: *stop=0xc90511cu;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[12]);goto Lc90511e; /* fmov.s @(r0,r12),fr3 */
Lc90511e: *stop=0xc90511eu;if(!valid_ram(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[11]);goto Lc905120; /* fmov.s @(r0,r11),fr2 */
Lc905120: *stop=0xc905120u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905122; /* fmul fr5,fr3 */
Lc905122: *stop=0xc905122u;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[0]+r[7]);goto Lc905124; /* fmov.s @(r0,r7),fr12 */
Lc905124: *stop=0xc905124u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905126; /* fmul fr6,fr2 */
Lc905126: *stop=0xc905126u;fcopy(f,1u,15u);goto Lc905128; /* fmov fr15,fr1 */
Lc905128: *stop=0xc905128u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[12],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc90512a; /* fmac fr0,fr12,fr3 */
Lc90512a: *stop=0xc90512au;fcopy(f,0u,7u);goto Lc90512c; /* fmov fr7,fr0 */
Lc90512c: *stop=0xc90512cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[12],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto Lc90512e; /* fmac fr0,fr12,fr2 */
Lc90512e: *stop=0xc90512eu;fcopy(f,0u,11u);goto Lc905130; /* fmov fr11,fr0 */
Lc905130: *stop=0xc905130u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto Lc905132; /* fmul fr2,fr1 */
Lc905132: *stop=0xc905132u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto Lc905134; /* fmac fr0,fr3,fr1 */
Lc905134: *stop=0xc905134u;fcopy(f,0u,8u);goto Lc905136; /* fmov fr8,fr0 */
Lc905136: *stop=0xc905136u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc905138; /* fmov fr1,@(r0,r4) */
Lc905138: *stop=0xc905138u;r[0]=8u;goto Lc90513a; /* mov #0x8,r0 */
Lc90513a: *stop=0xc90513au;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[12]);goto Lc90513c; /* fmov.s @(r0,r12),fr3 */
Lc90513c: *stop=0xc90513cu;if(!valid_ram(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[0]+r[7]);goto Lc90513e; /* fmov.s @(r0,r7),fr12 */
Lc90513e: *stop=0xc90513eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905140; /* fmul fr5,fr3 */
Lc905140: *stop=0xc905140u;if(!valid_ram(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[11]);goto Lc905142; /* fmov.s @(r0,r11),fr2 */
Lc905142: *stop=0xc905142u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[12],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905144; /* fmac fr0,fr12,fr3 */
Lc905144: *stop=0xc905144u;fcopy(f,0u,7u);goto Lc905146; /* fmov fr7,fr0 */
Lc905146: *stop=0xc905146u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905148; /* fmul fr6,fr2 */
Lc905148: *stop=0xc905148u;fcopy(f,1u,15u);goto Lc90514a; /* fmov fr15,fr1 */
Lc90514a: *stop=0xc90514au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[12],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto Lc90514c; /* fmac fr0,fr12,fr2 */
Lc90514c: *stop=0xc90514cu;fcopy(f,0u,11u);goto Lc90514e; /* fmov fr11,fr0 */
Lc90514e: *stop=0xc90514eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto Lc905150; /* fmul fr2,fr1 */
Lc905150: *stop=0xc905150u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto Lc905152; /* fmac fr0,fr3,fr1 */
Lc905152: *stop=0xc905152u;fcopy(f,0u,13u);goto Lc905154; /* fmov fr13,fr0 */
Lc905154: *stop=0xc905154u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc905156; /* fmov fr1,@(r0,r4) */
Lc905156: *stop=0xc905156u;r[0]=4u;goto Lc905158; /* mov #0x4,r0 */
Lc905158: *stop=0xc905158u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);goto Lc90515a; /* fmov.s @r4,fr3 */
Lc90515a: *stop=0xc90515au;if(!valid_ram(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[9]);goto Lc90515c; /* fmov.s @r9,fr2 */
Lc90515c: *stop=0xc90515cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto Lc90515e; /* fmul fr9,fr3 */
Lc90515e: *stop=0xc90515eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905160; /* fmac fr0,fr2,fr3 */
Lc905160: *stop=0xc905160u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[4]);goto Lc905162; /* fmov.s fr3,@r4 */
Lc905162: *stop=0xc905162u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc905164; /* fmov.s @(r0,r4),fr3 */
Lc905164: *stop=0xc905164u;if(!valid_ram(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[9]);goto Lc905166; /* fmov.s @(r0,r9),fr2 */
Lc905166: *stop=0xc905166u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905168; /* fmul fr9,fr3 */
Lc905168: *stop=0xc905168u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc90516a; /* fmac fr0,fr2,fr3 */
Lc90516a: *stop=0xc90516au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc90516c; /* fmov fr3,@(r0,r4) */
Lc90516c: *stop=0xc90516cu;r[0]=8u;goto Lc90516e; /* mov #0x8,r0 */
Lc90516e: *stop=0xc90516eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto Lc905170; /* fmov.s @(r0,r4),fr3 */
Lc905170: *stop=0xc905170u;if(!valid_ram(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[9]);goto Lc905172; /* fmov.s @(r0,r9),fr2 */
Lc905172: *stop=0xc905172u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905174; /* fmul fr9,fr3 */
Lc905174: *stop=0xc905174u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905176; /* fmac fr0,fr2,fr3 */
Lc905176: *stop=0xc905176u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto Lc905178; /* fmov fr3,@(r0,r4) */
Lc905178: *stop=0xc905178u;r[4]+=12u;goto Lc90517a; /* add #0xc,r4 */
Lc90517a: *stop=0xc90517au;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);goto Lc90517c; /* mov.l @r15,r3 */
Lc90517c: *stop=0xc90517cu;r[3]+=1u;goto Lc90517e; /* add #0x1,r3 */
Lc90517e: *stop=0xc90517eu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto Lc905180; /* mov.l r3,@r15 */
Lc905180: *stop=0xc905180u;if(!valid_ram(r[15]+8u,4u))return -3;r[2]=rd(m,r[15]+0x8u,4);goto Lc905182; /* mov.l @(0x8,r15),r2 */
Lc905182: *stop=0xc905182u;r[11]+=12u;goto Lc905184; /* add #0xc,r11 */
Lc905184: *stop=0xc905184u;r[7]+=12u;goto Lc905186; /* add #0xc,r7 */
Lc905186: *stop=0xc905186u;*sr=(*sr&~1u)|(r[3]>=r[2]);goto Lc905188; /* cmp/hs r2,r3 */
Lc905188: *stop=0xc905188u;r[12]+=12u;goto Lc90518a; /* add #0xc,r12 */
Lc90518a: take=!(*sr&1u);*stop=0xc90518cu;r[9]+=12u;if(take)goto Lc905100;goto Lc90518e; /* bf/s 0x0c905100 */
Lc90518e: *stop=0xc90518eu;if(!valid_ram(r[15]+44u,4u))return -3;r[0]=rd(m,r[15]+0x2cu,4);goto Lc905190; /* mov.l @(0x2c,r15),r0 */
Lc905190: *stop=0xc905190u;*sr=(*sr&~1u)|(r[0]==0x0000001bu);goto Lc905192; /* cmp/eq #0x1b,r0 */
Lc905192: if(!(*sr&1u))goto Lc905198;goto Lc905194; /* bf 0x0c905198 */
Lc905194: *stop=0xc905196u;goto Lc90543a; /* bra 0x0c90543a */
Lc905198: *stop=0xc905198u;r[0]=r[14];goto Lc90519a; /* mov r14,r0 */
Lc90519a: *stop=0xc90519au;if(!valid_ram(r[0]+r[13],4u))return -3;r[4]=rd(m,r[13]+r[0],4);goto Lc90519c; /* mov.l @(r0,r13),r4 */
Lc90519c: *stop=0xc90519cu;r[3]=r[15];goto Lc90519e; /* mov r15,r3 */
Lc90519e: *stop=0xc90519eu;r[3]+=64u;goto Lc9051a0; /* add #0x40,r3 */
Lc9051a0: *stop=0xc9051a0u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[4],4);goto Lc9051a2; /* mov.l r4,@r3 */
Lc9051a2: *stop=0xc9051a2u;r[3]=22u;goto Lc9051a4; /* mov #0x16,r3 */
Lc9051a4: *stop=0xc9051a4u;if(!valid_ram(r[4]+4u,4u))return -3;r[4]=rd(m,r[4]+0x4u,4);goto Lc9051a6; /* mov.l @(0x4,r4),r4 */
Lc9051a6: *stop=0xc9051a6u;r[3]+=r[4];goto Lc9051a8; /* add r4,r3 */
Lc9051a8: *stop=0xc9051a8u;if(!valid_ram(r[3],2u))return -3;r[3]=sx(m,r[3],2);goto Lc9051aa; /* mov.w @r3,r3 */
Lc9051aa: *stop=0xc9051aau;r[3]=r[3]&65535u;goto Lc9051ac; /* extu.w r3,r3 */
Lc9051ac: *stop=0xc9051acu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto Lc9051ae; /* mov.l r3,@r15 */
Lc9051ae: *stop=0xc9051aeu;if(!valid_ram(r[15]+40u,4u))return -3;r[0]=rd(m,r[15]+0x28u,4);goto Lc9051b0; /* mov.l @(0x28,r15),r0 */
Lc9051b0: *stop=0xc9051b0u;if(!valid_ram(r[4]+16u,4u))return -3;r[4]=rd(m,r[4]+0x10u,4);goto Lc9051b2; /* mov.l @(0x10,r4),r4 */
Lc9051b2: *stop=0xc9051b2u;if(!valid_ram(r[0]+r[14],4u))return -3;r[9]=rd(m,r[14]+r[0],4);goto Lc9051b4; /* mov.l @(r0,r14),r9 */
Lc9051b4: *stop=0xc9051b4u;r[0]=r[14];goto Lc9051b6; /* mov r14,r0 */
Lc9051b6: *stop=0xc9051b6u;if(!valid_ram(r[0]+r[8],4u))return -3;r[12]=rd(m,r[8]+r[0],4);goto Lc9051b8; /* mov.l @(r0,r8),r12 */
Lc9051b8: *stop=0xc9051b8u;if(!valid_ram(r[0]+r[10],4u))return -3;r[11]=rd(m,r[10]+r[0],4);goto Lc9051ba; /* mov.l @(r0,r10),r11 */
Lc9051ba: *stop=0xc9051bau;if(!valid_ram(r[15]+36u,4u))return -3;r[0]=rd(m,r[15]+0x24u,4);goto Lc9051bc; /* mov.l @(0x24,r15),r0 */
Lc9051bc: *stop=0xc9051bcu;if(!valid_ram(r[11]+4u,4u))return -3;r[11]=rd(m,r[11]+0x4u,4);goto Lc9051be; /* mov.l @(0x4,r11),r11 */
Lc9051be: *stop=0xc9051beu;if(!valid_ram(r[0]+r[14],4u))return -3;r[7]=rd(m,r[14]+r[0],4);goto Lc9051c0; /* mov.l @(r0,r14),r7 */
Lc9051c0: *stop=0xc9051c0u;if(!valid_ram(r[15]+32u,4u))return -3;r[0]=rd(m,r[15]+0x20u,4);goto Lc9051c2; /* mov.l @(0x20,r15),r0 */
Lc9051c2: *stop=0xc9051c2u;if(!valid_ram(r[7]+4u,4u))return -3;r[7]=rd(m,r[7]+0x4u,4);goto Lc9051c4; /* mov.l @(0x4,r7),r7 */
Lc9051c4: *stop=0xc9051c4u;if(!valid_ram(r[9]+4u,4u))return -3;r[9]=rd(m,r[9]+0x4u,4);goto Lc9051c6; /* mov.l @(0x4,r9),r9 */
Lc9051c6: *stop=0xc9051c6u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto Lc9051c8; /* tst r0,r0 */
Lc9051c8: *stop=0xc9051c8u;if(!valid_ram(r[12]+4u,4u))return -3;r[12]=rd(m,r[12]+0x4u,4);goto Lc9051ca; /* mov.l @(0x4,r12),r12 */
Lc9051ca: *stop=0xc9051cau;r[0]=*sr&1u;goto Lc9051cc; /* movt r0 */
Lc9051cc: *stop=0xc9051ccu;if(!valid_ram(r[7]+16u,4u))return -3;r[7]=rd(m,r[7]+0x10u,4);goto Lc9051ce; /* mov.l @(0x10,r7),r7 */
Lc9051ce: *stop=0xc9051ceu;if(!valid_ram(r[9]+16u,4u))return -3;r[9]=rd(m,r[9]+0x10u,4);goto Lc9051d0; /* mov.l @(0x10,r9),r9 */
Lc9051d0: *stop=0xc9051d0u;if(!valid_ram(r[11]+16u,4u))return -3;r[11]=rd(m,r[11]+0x10u,4);goto Lc9051d2; /* mov.l @(0x10,r11),r11 */
Lc9051d2: *stop=0xc9051d2u;if(!valid_ram(r[12]+16u,4u))return -3;r[12]=rd(m,r[12]+0x10u,4);goto Lc9051d4; /* mov.l @(0x10,r12),r12 */
Lc9051d4: *stop=0xc9051d4u;if(!valid_ram(r[15]+44u,4u))return -3;wr(m,r[15]+0x2cu,r[0],4);goto Lc9051d6; /* mov.l r0,@(0x2c,r15) */
Lc9051d6: *stop=0xc9051d6u;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);goto Lc9051d8; /* mov.l @r15,r3 */
Lc9051d8: *stop=0xc9051d8u;r[2]=0u;goto Lc9051da; /* mov #0x0,r2 */
Lc9051da: *stop=0xc9051dau;*sr=(*sr&~1u)|(r[2]>=r[3]);goto Lc9051dc; /* cmp/hs r3,r2 */
Lc9051dc: take=!(*sr&1u);*stop=0xc9051deu;r[14]=r[6];if(take)goto Lc9051e4;goto Lc9051e0; /* bf/s 0x0c9051e4 */
Lc9051e0: *stop=0xc9051e2u;goto Lc90543a; /* bra 0x0c90543a */
Lc9051e4: *stop=0xc9051e4u;r[0]=4294967276u;goto Lc9051e6; /* mov #-0x14,r0 */
Lc9051e6: *stop=0xc9051e6u;r[0]=r[0]&255u;goto Lc9051e8; /* extu.b r0,r0 */
Lc9051e8: *stop=0xc9051e8u;if(!valid_ram(r[0]+r[5],4u))return -3;r[0]=rd(m,r[5]+r[0],4);goto Lc9051ea; /* mov.l @(r0,r5),r0 */
Lc9051ea: *stop=0xc9051eau;r[1]=54u;goto Lc9051ec; /* mov #0x36,r1 */
Lc9051ec: *stop=0xc9051ecu;if(!valid_ram(r[0]+r[1],2u))return -3;r[0]=sx(m,r[0]+r[1],2);goto Lc9051ee; /* mov.w @(r0,r1),r0 */
Lc9051ee: *stop=0xc9051eeu;r[0]=r[0]&65535u;goto Lc9051f0; /* extu.w r0,r0 */
Lc9051f0: *stop=0xc9051f0u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto Lc9051f2; /* tst #0x2,r0 */
Lc9051f2: if((*sr&1u))goto Lc905240;goto Lc9051f4; /* bt 0x0c905240 */
Lc9051f4: *stop=0xc9051f4u;r[0]=0x0c905228u;goto Lc9051f6; /* mova 0x0c905228,r0 */
Lc9051f6: *stop=0xc9051f6u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto Lc9051f8; /* fmov.s @r0,fr3 */
Lc9051f8: *stop=0xc9051f8u;r[0]=24u;goto Lc9051fa; /* mov #0x18,r0 */
Lc9051fa: *stop=0xc9051fau;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto Lc9051fc; /* fmov fr3,@(r0,r15) */
Lc9051fc: *stop=0xc9051fcu;r[0]=28u;goto Lc9051fe; /* mov #0x1c,r0 */
Lc9051fe: *stop=0xc9051feu;fcopy(f,12u,3u);goto Lc905200; /* fmov fr3,fr12 */
Lc905200: *stop=0xc905202u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto Lc9053d6; /* bra 0x0c9053d6 */
Lc905240: *stop=0xc905240u;if(!valid_ram(r[9]+2u,1u))return -3;r[0]=sx(m,r[9]+0x2u,1);goto Lc905242; /* mov.b @(0x2,r9),r0 */
Lc905242: *stop=0xc905242u;fcopy(f,1u,15u);goto Lc905244; /* fmov fr15,fr1 */
Lc905244: *stop=0xc905244u;r[3]=r[0]&255u;goto Lc905246; /* extu.b r0,r3 */
Lc905246: *stop=0xc905246u;if(!valid_ram(r[12]+2u,1u))return -3;r[0]=sx(m,r[12]+0x2u,1);goto Lc905248; /* mov.b @(0x2,r12),r0 */
Lc905248: *stop=0xc905248u;*fpul=r[3];goto Lc90524a; /* lds r3,FPUL */
Lc90524a: *stop=0xc90524au;r[3]=r[0]&255u;goto Lc90524c; /* extu.b r0,r3 */
Lc90524c: *stop=0xc90524cu;if(!valid_ram(r[11]+2u,1u))return -3;r[0]=sx(m,r[11]+0x2u,1);goto Lc90524e; /* mov.b @(0x2,r11),r0 */
Lc90524e: *stop=0xc90524eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][12]);if(status)return status;goto Lc905250; /* float FPUL,fr12 */
Lc905250: *stop=0xc905250u;*fpul=r[3];goto Lc905252; /* lds r3,FPUL */
Lc905252: *stop=0xc905252u;r[3]=r[0]&255u;goto Lc905254; /* extu.b r0,r3 */
Lc905254: *stop=0xc905254u;if(!valid_ram(r[9]+1u,1u))return -3;r[0]=sx(m,r[9]+0x1u,1);goto Lc905256; /* mov.b @(0x1,r9),r0 */
Lc905256: *stop=0xc905256u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc905258; /* float FPUL,fr3 */
Lc905258: *stop=0xc905258u;*fpul=r[3];goto Lc90525a; /* lds r3,FPUL */
Lc90525a: *stop=0xc90525au;r[3]=r[0]&255u;goto Lc90525c; /* extu.b r0,r3 */
Lc90525c: *stop=0xc90525cu;fcopy(f,0u,12u);goto Lc90525e; /* fmov fr12,fr0 */
Lc90525e: *stop=0xc90525eu;r[0]=52u;goto Lc905260; /* mov #0x34,r0 */
Lc905260: *stop=0xc905260u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto Lc905262; /* float FPUL,fr2 */
Lc905262: *stop=0xc905262u;*fpul=r[3];goto Lc905264; /* lds r3,FPUL */
Lc905264: *stop=0xc905264u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905266; /* fmul fr5,fr3 */
Lc905266: *stop=0xc905266u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905268; /* fmul fr6,fr2 */
Lc905268: *stop=0xc905268u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[8],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc90526a; /* fmac fr0,fr8,fr3 */
Lc90526a: *stop=0xc90526au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[7],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto Lc90526c; /* fmac fr0,fr7,fr2 */
Lc90526c: *stop=0xc90526cu;fcopy(f,0u,11u);goto Lc90526e; /* fmov fr11,fr0 */
Lc90526e: *stop=0xc90526eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto Lc905270; /* fmul fr2,fr1 */
Lc905270: *stop=0xc905270u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto Lc905272; /* fmac fr0,fr3,fr1 */
Lc905272: *stop=0xc905272u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc905274; /* float FPUL,fr3 */
Lc905274: *stop=0xc905274u;fcopy(f,12u,1u);goto Lc905276; /* fmov fr1,fr12 */
Lc905276: *stop=0xc905276u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto Lc905278; /* fmov fr3,@(r0,r15) */
Lc905278: *stop=0xc905278u;if(!valid_ram(r[12]+1u,1u))return -3;r[0]=sx(m,r[12]+0x1u,1);goto Lc90527a; /* mov.b @(0x1,r12),r0 */
Lc90527a: *stop=0xc90527au;r[3]=r[0]&255u;goto Lc90527c; /* extu.b r0,r3 */
Lc90527c: *stop=0xc90527cu;*fpul=r[3];goto Lc90527e; /* lds r3,FPUL */
Lc90527e: *stop=0xc90527eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto Lc905280; /* float FPUL,fr2 */
Lc905280: *stop=0xc905280u;if(!valid_ram(r[11]+1u,1u))return -3;r[0]=sx(m,r[11]+0x1u,1);goto Lc905282; /* mov.b @(0x1,r11),r0 */
Lc905282: *stop=0xc905282u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905284; /* fmul fr5,fr2 */
Lc905284: *stop=0xc905284u;fcopy(f,0u,8u);goto Lc905286; /* fmov fr8,fr0 */
Lc905286: *stop=0xc905286u;r[3]=r[0]&255u;goto Lc905288; /* extu.b r0,r3 */
Lc905288: *stop=0xc905288u;*fpul=r[3];goto Lc90528a; /* lds r3,FPUL */
Lc90528a: *stop=0xc90528au;r[0]=20u;goto Lc90528c; /* mov #0x14,r0 */
Lc90528c: *stop=0xc90528cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto Lc90528e; /* fmac fr0,fr3,fr2 */
Lc90528e: *stop=0xc90528eu;fcopy(f,0u,7u);goto Lc905290; /* fmov fr7,fr0 */
Lc905290: *stop=0xc905290u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto Lc905292; /* float FPUL,fr1 */
Lc905292: *stop=0xc905292u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto Lc905294; /* fmul fr6,fr1 */
Lc905294: *stop=0xc905294u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto Lc905296; /* fmac fr0,fr3,fr1 */
Lc905296: *stop=0xc905296u;fcopy(f,3u,15u);goto Lc905298; /* fmov fr15,fr3 */
Lc905298: *stop=0xc905298u;fcopy(f,0u,11u);goto Lc90529a; /* fmov fr11,fr0 */
Lc90529a: *stop=0xc90529au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[1],&f->fpscr,&fr[3]);if(status)return status;}goto Lc90529c; /* fmul fr1,fr3 */
Lc90529c: *stop=0xc90529cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc90529e; /* fmac fr0,fr2,fr3 */
Lc90529e: *stop=0xc90529eu;fcopy(f,0u,8u);goto Lc9052a0; /* fmov fr8,fr0 */
Lc9052a0: *stop=0xc9052a0u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto Lc9052a2; /* fmov fr3,@(r0,r15) */
Lc9052a2: *stop=0xc9052a2u;r[0]=56u;goto Lc9052a4; /* mov #0x38,r0 */
Lc9052a4: *stop=0xc9052a4u;if(!valid_ram(r[9],1u))return -3;r[3]=sx(m,r[9],1);goto Lc9052a6; /* mov.b @r9,r3 */
Lc9052a6: *stop=0xc9052a6u;r[3]=r[3]&255u;goto Lc9052a8; /* extu.b r3,r3 */
Lc9052a8: *stop=0xc9052a8u;*fpul=r[3];goto Lc9052aa; /* lds r3,FPUL */
Lc9052aa: *stop=0xc9052aau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc9052ac; /* float FPUL,fr3 */
Lc9052ac: *stop=0xc9052acu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto Lc9052ae; /* fmov fr3,@(r0,r15) */
Lc9052ae: *stop=0xc9052aeu;if(!valid_ram(r[12],1u))return -3;r[3]=sx(m,r[12],1);goto Lc9052b0; /* mov.b @r12,r3 */
Lc9052b0: *stop=0xc9052b0u;r[3]=r[3]&255u;goto Lc9052b2; /* extu.b r3,r3 */
Lc9052b2: *stop=0xc9052b2u;*fpul=r[3];goto Lc9052b4; /* lds r3,FPUL */
Lc9052b4: *stop=0xc9052b4u;if(!valid_ram(r[11],1u))return -3;r[3]=sx(m,r[11],1);goto Lc9052b6; /* mov.b @r11,r3 */
Lc9052b6: *stop=0xc9052b6u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto Lc9052b8; /* float FPUL,fr2 */
Lc9052b8: *stop=0xc9052b8u;r[3]=r[3]&255u;goto Lc9052ba; /* extu.b r3,r3 */
Lc9052ba: *stop=0xc9052bau;*fpul=r[3];goto Lc9052bc; /* lds r3,FPUL */
Lc9052bc: *stop=0xc9052bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto Lc9052be; /* fmul fr5,fr2 */
Lc9052be: *stop=0xc9052beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto Lc9052c0; /* fmac fr0,fr3,fr2 */
Lc9052c0: *stop=0xc9052c0u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto Lc9052c2; /* float FPUL,fr1 */
Lc9052c2: *stop=0xc9052c2u;fcopy(f,0u,7u);goto Lc9052c4; /* fmov fr7,fr0 */
Lc9052c4: *stop=0xc9052c4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto Lc9052c6; /* fcmp/gt fr4,fr12 */
Lc9052c6: *stop=0xc9052c6u;r[0]=16u;goto Lc9052c8; /* mov #0x10,r0 */
Lc9052c8: *stop=0xc9052c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto Lc9052ca; /* fmul fr6,fr1 */
Lc9052ca: *stop=0xc9052cau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto Lc9052cc; /* fmac fr0,fr3,fr1 */
Lc9052cc: *stop=0xc9052ccu;fcopy(f,3u,15u);goto Lc9052ce; /* fmov fr15,fr3 */
Lc9052ce: *stop=0xc9052ceu;fcopy(f,0u,11u);goto Lc9052d0; /* fmov fr11,fr0 */
Lc9052d0: *stop=0xc9052d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[1],&f->fpscr,&fr[3]);if(status)return status;}goto Lc9052d2; /* fmul fr1,fr3 */
Lc9052d2: *stop=0xc9052d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc9052d4; /* fmac fr0,fr2,fr3 */
Lc9052d4: take=!(*sr&1u);*stop=0xc9052d6u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);if(take)goto Lc9052dc;goto Lc9052d8; /* bf/s 0x0c9052dc */
Lc9052d8: *stop=0xc9052dau;fcopy(f,12u,4u);goto Lc9052e4; /* bra 0x0c9052e4 */
Lc9052dc: *stop=0xc9052dcu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc9052de; /* fldi0 fr2 */
Lc9052de: *stop=0xc9052deu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][12],&f->fpscr,sr,1);if(status)return status;goto Lc9052e0; /* fcmp/gt fr12,fr2 */
Lc9052e0: if(!(*sr&1u))goto Lc9052e4;goto Lc9052e2; /* bf 0x0c9052e4 */
Lc9052e2: *stop=0xc9052e2u;fcopy(f,12u,10u);goto Lc9052e4; /* fmov fr10,fr12 */
Lc9052e4: *stop=0xc9052e4u;r[0]=20u;goto Lc9052e6; /* mov #0x14,r0 */
Lc9052e6: *stop=0xc9052e6u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto Lc9052e8; /* fmov.s @(r0,r15),fr3 */
Lc9052e8: *stop=0xc9052e8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto Lc9052ea; /* fcmp/gt fr4,fr3 */
Lc9052ea: if(!(*sr&1u))goto Lc9052f2;goto Lc9052ec; /* bf 0x0c9052f2 */
Lc9052ec: *stop=0xc9052ecu;r[0]=20u;goto Lc9052ee; /* mov #0x14,r0 */
Lc9052ee: *stop=0xc9052f0u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[15]);goto Lc9052fc; /* bra 0x0c9052fc */
Lc9052f2: *stop=0xc9052f2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc9052f4; /* fldi0 fr2 */
Lc9052f4: *stop=0xc9052f4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc9052f6; /* fcmp/gt fr3,fr2 */
Lc9052f6: if(!(*sr&1u))goto Lc9052fc;goto Lc9052f8; /* bf 0x0c9052fc */
Lc9052f8: *stop=0xc9052f8u;r[0]=20u;goto Lc9052fa; /* mov #0x14,r0 */
Lc9052fa: *stop=0xc9052fau;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,10u,r[0]+r[15]);goto Lc9052fc; /* fmov fr10,@(r0,r15) */
Lc9052fc: *stop=0xc9052fcu;r[0]=16u;goto Lc9052fe; /* mov #0x10,r0 */
Lc9052fe: *stop=0xc9052feu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto Lc905300; /* fmov.s @(r0,r15),fr3 */
Lc905300: *stop=0xc905300u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto Lc905302; /* fcmp/gt fr4,fr3 */
Lc905302: if(!(*sr&1u))goto Lc90530a;goto Lc905304; /* bf 0x0c90530a */
Lc905304: *stop=0xc905304u;r[0]=16u;goto Lc905306; /* mov #0x10,r0 */
Lc905306: *stop=0xc905308u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[15]);goto Lc905314; /* bra 0x0c905314 */
Lc90530a: *stop=0xc90530au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc90530c; /* fldi0 fr2 */
Lc90530c: *stop=0xc90530cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc90530e; /* fcmp/gt fr3,fr2 */
Lc90530e: if(!(*sr&1u))goto Lc905314;goto Lc905310; /* bf 0x0c905314 */
Lc905310: *stop=0xc905310u;r[0]=16u;goto Lc905312; /* mov #0x10,r0 */
Lc905312: *stop=0xc905312u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,10u,r[0]+r[15]);goto Lc905314; /* fmov fr10,@(r0,r15) */
Lc905314: *stop=0xc905314u;fcopy(f,3u,9u);goto Lc905316; /* fmov fr9,fr3 */
Lc905316: *stop=0xc905316u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][14],&f->fpscr,sr,1);if(status)return status;goto Lc905318; /* fcmp/gt fr14,fr3 */
Lc905318: *stop=0xc905318u;r[0]=4u;goto Lc90531a; /* mov #0x4,r0 */
Lc90531a: take=!(*sr&1u);*stop=0xc90531cu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,9u,r[0]+r[15]);if(take)goto Lc905322;goto Lc90531e; /* bf/s 0x0c905322 */
Lc90531e: *stop=0xc90531eu;r[0]=4u;goto Lc905320; /* mov #0x4,r0 */
Lc905320: *stop=0xc905320u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,14u,r[0]+r[15]);goto Lc905322; /* fmov fr14,@(r0,r15) */
Lc905322: *stop=0xc905322u;r[0]=4u;goto Lc905324; /* mov #0x4,r0 */
Lc905324: *stop=0xc905324u;fcopy(f,2u,14u);goto Lc905326; /* fmov fr14,fr2 */
Lc905326: *stop=0xc905326u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto Lc905328; /* fmov.s @(r0,r15),fr3 */
Lc905328: *stop=0xc905328u;r[0]=12u;goto Lc90532a; /* mov #0xc,r0 */
Lc90532a: *stop=0xc90532au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto Lc90532c; /* fsub fr3,fr2 */
Lc90532c: *stop=0xc90532cu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[15]);goto Lc90532e; /* fmov fr2,@(r0,r15) */
Lc90532e: *stop=0xc90532eu;if(!valid_ram(r[7]+2u,1u))return -3;r[0]=sx(m,r[7]+0x2u,1);goto Lc905330; /* mov.b @(0x2,r7),r0 */
Lc905330: *stop=0xc905330u;r[3]=r[0]&255u;goto Lc905332; /* extu.b r0,r3 */
Lc905332: *stop=0xc905332u;*fpul=r[3];goto Lc905334; /* lds r3,FPUL */
Lc905334: *stop=0xc905334u;r[0]=4u;goto Lc905336; /* mov #0x4,r0 */
Lc905336: *stop=0xc905336u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[15]);goto Lc905338; /* fmov.s @(r0,r15),fr0 */
Lc905338: *stop=0xc905338u;r[0]=24u;goto Lc90533a; /* mov #0x18,r0 */
Lc90533a: *stop=0xc90533au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc90533c; /* float FPUL,fr3 */
Lc90533c: *stop=0xc90533cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[0],&f->fpscr,&fr[12]);if(status)return status;}goto Lc90533e; /* fmul fr0,fr12 */
Lc90533e: *stop=0xc90533eu;fcopy(f,0u,2u);goto Lc905340; /* fmov fr2,fr0 */
Lc905340: *stop=0xc905340u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[12],&f->fpscr,&fr[12]);if(status)return status;}goto Lc905342; /* fmac fr0,fr3,fr12 */
Lc905342: *stop=0xc905342u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][12],&f->fpscr,fpul);if(status)return status;goto Lc905344; /* ftrc fr12,FPUL */
Lc905344: *stop=0xc905344u;r[3]=*fpul;goto Lc905346; /* sts FPUL,r3 */
Lc905346: *stop=0xc905346u;r[3]=r[3]&255u;goto Lc905348; /* extu.b r3,r3 */
Lc905348: *stop=0xc905348u;*fpul=r[3];goto Lc90534a; /* lds r3,FPUL */
Lc90534a: *stop=0xc90534au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc90534c; /* float FPUL,fr3 */
Lc90534c: *stop=0xc90534cu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto Lc90534e; /* fmov fr3,@(r0,r15) */
Lc90534e: *stop=0xc90534eu;if(!valid_ram(r[7]+1u,1u))return -3;r[0]=sx(m,r[7]+0x1u,1);goto Lc905350; /* mov.b @(0x1,r7),r0 */
Lc905350: *stop=0xc905350u;r[3]=r[0]&255u;goto Lc905352; /* extu.b r0,r3 */
Lc905352: *stop=0xc905352u;r[0]=4u;goto Lc905354; /* mov #0x4,r0 */
Lc905354: *stop=0xc905354u;*fpul=r[3];goto Lc905356; /* lds r3,FPUL */
Lc905356: *stop=0xc905356u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[15]);goto Lc905358; /* fmov.s @(r0,r15),fr0 */
Lc905358: *stop=0xc905358u;r[0]=20u;goto Lc90535a; /* mov #0x14,r0 */
Lc90535a: *stop=0xc90535au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto Lc90535c; /* fmov.s @(r0,r15),fr2 */
Lc90535c: *stop=0xc90535cu;r[0]=12u;goto Lc90535e; /* mov #0xc,r0 */
Lc90535e: *stop=0xc90535eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc905360; /* float FPUL,fr3 */
Lc905360: *stop=0xc905360u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905362; /* fmul fr0,fr2 */
Lc905362: *stop=0xc905362u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[15]);goto Lc905364; /* fmov.s @(r0,r15),fr0 */
Lc905364: *stop=0xc905364u;r[0]=4u;goto Lc905366; /* mov #0x4,r0 */
Lc905366: *stop=0xc905366u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905368; /* fmac fr0,fr3,fr2 */
Lc905368: *stop=0xc905368u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[15]);goto Lc90536a; /* fmov.s @(r0,r15),fr0 */
Lc90536a: *stop=0xc90536au;r[0]=16u;goto Lc90536c; /* mov #0x10,r0 */
Lc90536c: *stop=0xc90536cu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto Lc90536e; /* ftrc fr2,FPUL */
Lc90536e: *stop=0xc90536eu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto Lc905370; /* fmov.s @(r0,r15),fr2 */
Lc905370: *stop=0xc905370u;r[0]=12u;goto Lc905372; /* mov #0xc,r0 */
Lc905372: *stop=0xc905372u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905374; /* fmul fr0,fr2 */
Lc905374: *stop=0xc905374u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[15]);goto Lc905376; /* fmov.s @(r0,r15),fr0 */
Lc905376: *stop=0xc905376u;r[0]=28u;goto Lc905378; /* mov #0x1c,r0 */
Lc905378: *stop=0xc905378u;r[3]=*fpul;goto Lc90537a; /* sts FPUL,r3 */
Lc90537a: *stop=0xc90537au;r[3]=r[3]&255u;goto Lc90537c; /* extu.b r3,r3 */
Lc90537c: *stop=0xc90537cu;*fpul=r[3];goto Lc90537e; /* lds r3,FPUL */
Lc90537e: *stop=0xc90537eu;if(!valid_ram(r[7],1u))return -3;r[3]=sx(m,r[7],1);goto Lc905380; /* mov.b @r7,r3 */
Lc905380: *stop=0xc905380u;r[3]=r[3]&255u;goto Lc905382; /* extu.b r3,r3 */
Lc905382: *stop=0xc905382u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][12]);if(status)return status;goto Lc905384; /* float FPUL,fr12 */
Lc905384: *stop=0xc905384u;*fpul=r[3];goto Lc905386; /* lds r3,FPUL */
Lc905386: *stop=0xc905386u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc905388; /* float FPUL,fr3 */
Lc905388: *stop=0xc905388u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto Lc90538a; /* fmac fr0,fr3,fr2 */
Lc90538a: *stop=0xc90538au;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto Lc90538c; /* ftrc fr2,FPUL */
Lc90538c: *stop=0xc90538cu;r[3]=*fpul;goto Lc90538e; /* sts FPUL,r3 */
Lc90538e: *stop=0xc90538eu;r[3]=r[3]&255u;goto Lc905390; /* extu.b r3,r3 */
Lc905390: *stop=0xc905390u;*fpul=r[3];goto Lc905392; /* lds r3,FPUL */
Lc905392: *stop=0xc905392u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto Lc905394; /* float FPUL,fr3 */
Lc905394: *stop=0xc905394u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto Lc905396; /* fmov fr3,@(r0,r15) */
Lc905396: *stop=0xc905396u;r[0]=24u;goto Lc905398; /* mov #0x18,r0 */
Lc905398: *stop=0xc905398u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[15]);goto Lc90539a; /* fmov.s @(r0,r15),fr2 */
Lc90539a: *stop=0xc90539au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto Lc90539c; /* fcmp/gt fr4,fr2 */
Lc90539c: if(!(*sr&1u))goto Lc9053a4;goto Lc90539e; /* bf 0x0c9053a4 */
Lc90539e: *stop=0xc90539eu;r[0]=24u;goto Lc9053a0; /* mov #0x18,r0 */
Lc9053a0: *stop=0xc9053a2u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[15]);goto Lc9053ae; /* bra 0x0c9053ae */
Lc9053a4: *stop=0xc9053a4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0;goto Lc9053a6; /* fldi0 fr1 */
Lc9053a6: *stop=0xc9053a6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto Lc9053a8; /* fcmp/gt fr2,fr1 */
Lc9053a8: if(!(*sr&1u))goto Lc9053ae;goto Lc9053aa; /* bf 0x0c9053ae */
Lc9053aa: *stop=0xc9053aau;r[0]=24u;goto Lc9053ac; /* mov #0x18,r0 */
Lc9053ac: *stop=0xc9053acu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,10u,r[0]+r[15]);goto Lc9053ae; /* fmov fr10,@(r0,r15) */
Lc9053ae: *stop=0xc9053aeu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto Lc9053b0; /* fcmp/gt fr4,fr12 */
Lc9053b0: if(!(*sr&1u))goto Lc9053b6;goto Lc9053b2; /* bf 0x0c9053b6 */
Lc9053b2: *stop=0xc9053b4u;fcopy(f,12u,4u);goto Lc9053be; /* bra 0x0c9053be */
Lc9053b6: *stop=0xc9053b6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto Lc9053b8; /* fldi0 fr3 */
Lc9053b8: *stop=0xc9053b8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][12],&f->fpscr,sr,1);if(status)return status;goto Lc9053ba; /* fcmp/gt fr12,fr3 */
Lc9053ba: if(!(*sr&1u))goto Lc9053be;goto Lc9053bc; /* bf 0x0c9053be */
Lc9053bc: *stop=0xc9053bcu;fcopy(f,12u,10u);goto Lc9053be; /* fmov fr10,fr12 */
Lc9053be: *stop=0xc9053beu;r[0]=28u;goto Lc9053c0; /* mov #0x1c,r0 */
Lc9053c0: *stop=0xc9053c0u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto Lc9053c2; /* fmov.s @(r0,r15),fr3 */
Lc9053c2: *stop=0xc9053c2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto Lc9053c4; /* fcmp/gt fr4,fr3 */
Lc9053c4: if(!(*sr&1u))goto Lc9053cc;goto Lc9053c6; /* bf 0x0c9053cc */
Lc9053c6: *stop=0xc9053c6u;r[0]=28u;goto Lc9053c8; /* mov #0x1c,r0 */
Lc9053c8: *stop=0xc9053cau;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[15]);goto Lc9053d6; /* bra 0x0c9053d6 */
Lc9053cc: *stop=0xc9053ccu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc9053ce; /* fldi0 fr2 */
Lc9053ce: *stop=0xc9053ceu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc9053d0; /* fcmp/gt fr3,fr2 */
Lc9053d0: if(!(*sr&1u))goto Lc9053d6;goto Lc9053d2; /* bf 0x0c9053d6 */
Lc9053d2: *stop=0xc9053d2u;r[0]=28u;goto Lc9053d4; /* mov #0x1c,r0 */
Lc9053d4: *stop=0xc9053d4u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,10u,r[0]+r[15]);goto Lc9053d6; /* fmov fr10,@(r0,r15) */
Lc9053d6: *stop=0xc9053d6u;r[0]=24u;goto Lc9053d8; /* mov #0x18,r0 */
Lc9053d8: *stop=0xc9053d8u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto Lc9053da; /* fmov.s @(r0,r15),fr3 */
Lc9053da: *stop=0xc9053dau;*sr=(*sr&~1u)|!(r[14]&r[14]);goto Lc9053dc; /* tst r14,r14 */
Lc9053dc: *stop=0xc9053dcu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto Lc9053de; /* ftrc fr3,FPUL */
Lc9053de: *stop=0xc9053deu;r[0]=*fpul;goto Lc9053e0; /* sts FPUL,r0 */
Lc9053e0: *stop=0xc9053e0u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][12],&f->fpscr,fpul);if(status)return status;goto Lc9053e2; /* ftrc fr12,FPUL */
Lc9053e2: *stop=0xc9053e2u;if(!valid_ram(r[4]+2u,1u))return -3;wr(m,r[4]+0x2u,r[0],1);goto Lc9053e4; /* mov.b r0,@(0x2,r4) */
Lc9053e4: *stop=0xc9053e4u;r[0]=*fpul;goto Lc9053e6; /* sts FPUL,r0 */
Lc9053e6: *stop=0xc9053e6u;if(!valid_ram(r[4]+1u,1u))return -3;wr(m,r[4]+0x1u,r[0],1);goto Lc9053e8; /* mov.b r0,@(0x1,r4) */
Lc9053e8: *stop=0xc9053e8u;r[0]=28u;goto Lc9053ea; /* mov #0x1c,r0 */
Lc9053ea: *stop=0xc9053eau;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto Lc9053ec; /* fmov.s @(r0,r15),fr3 */
Lc9053ec: *stop=0xc9053ecu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto Lc9053ee; /* ftrc fr3,FPUL */
Lc9053ee: *stop=0xc9053eeu;r[3]=*fpul;goto Lc9053f0; /* sts FPUL,r3 */
Lc9053f0: take=!(*sr&1u);*stop=0xc9053f2u;if(!valid_ram(r[4],1u))return -3;wr(m,r[4],r[3],1);if(take)goto Lc905424;goto Lc9053f4; /* bf/s 0x0c905424 */
Lc9053f4: *stop=0xc9053f4u;if(!valid_ram(r[15]+44u,4u))return -3;r[1]=rd(m,r[15]+0x2cu,4);goto Lc9053f6; /* mov.l @(0x2c,r15),r1 */
Lc9053f6: *stop=0xc9053f6u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto Lc9053f8; /* tst r1,r1 */
Lc9053f8: if((*sr&1u))goto Lc905424;goto Lc9053fa; /* bt 0x0c905424 */
Lc9053fa: *stop=0xc9053fau;r[1]=9u;goto Lc9053fc; /* mov #0x9,r1 */
Lc9053fc: *stop=0xc9053fcu;if(!valid_ram(r[4]+2u,1u))return -3;r[0]=sx(m,r[4]+0x2u,1);goto Lc9053fe; /* mov.b @(0x2,r4),r0 */
Lc9053fe: *stop=0xc9053feu;r[1]<<=8;goto Lc905400; /* shll8 r1 */
Lc905400: *stop=0xc905400u;r[1]+=4294967238u;goto Lc905402; /* add #-0x3a,r1 */
Lc905402: *stop=0xc905402u;r[2]=9u;goto Lc905404; /* mov #0x9,r2 */
Lc905404: *stop=0xc905404u;r[1]+=r[5];goto Lc905406; /* add r5,r1 */
Lc905406: *stop=0xc905406u;r[2]<<=8;goto Lc905408; /* shll8 r2 */
Lc905408: *stop=0xc905408u;if(!valid_ram(r[1],1u))return -3;wr(m,r[1],r[0],1);goto Lc90540a; /* mov.b r0,@r1 */
Lc90540a: *stop=0xc90540au;r[2]+=4294967237u;goto Lc90540c; /* add #-0x3b,r2 */
Lc90540c: *stop=0xc90540cu;if(!valid_ram(r[4]+1u,1u))return -3;r[0]=sx(m,r[4]+0x1u,1);goto Lc90540e; /* mov.b @(0x1,r4),r0 */
Lc90540e: *stop=0xc90540eu;r[2]+=r[5];goto Lc905410; /* add r5,r2 */
Lc905410: *stop=0xc905410u;if(!valid_ram(r[2],1u))return -3;wr(m,r[2],r[0],1);goto Lc905412; /* mov.b r0,@r2 */
Lc905412: *stop=0xc905412u;r[0]=9u;goto Lc905414; /* mov #0x9,r0 */
Lc905414: *stop=0xc905414u;r[0]<<=8;goto Lc905416; /* shll8 r0 */
Lc905416: *stop=0xc905416u;if(!valid_ram(r[4],1u))return -3;r[3]=sx(m,r[4],1);goto Lc905418; /* mov.b @r4,r3 */
Lc905418: *stop=0xc905418u;r[0]+=4294967236u;goto Lc90541a; /* add #-0x3c,r0 */
Lc90541a: *stop=0xc90541au;r[2]=4294967295u;goto Lc90541c; /* mov #-0x1,r2 */
Lc90541c: *stop=0xc90541cu;if(!valid_ram(r[0]+r[5],1u))return -3;wr(m,r[5]+r[0],r[3],1);goto Lc90541e; /* mov.b r3,@(r0,r5) */
Lc90541e: *stop=0xc90541eu;r[0]+=3u;goto Lc905420; /* add #0x3,r0 */
Lc905420: *stop=0xc905420u;r[2]=r[2]&255u;goto Lc905422; /* extu.b r2,r2 */
Lc905422: *stop=0xc905422u;if(!valid_ram(r[0]+r[5],1u))return -3;wr(m,r[5]+r[0],r[2],1);goto Lc905424; /* mov.b r2,@(r0,r5) */
Lc905424: *stop=0xc905424u;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);goto Lc905426; /* mov.l @r15,r3 */
Lc905426: *stop=0xc905426u;r[14]+=1u;goto Lc905428; /* add #0x1,r14 */
Lc905428: *stop=0xc905428u;r[11]+=20u;goto Lc90542a; /* add #0x14,r11 */
Lc90542a: *stop=0xc90542au;*sr=(*sr&~1u)|(r[14]>=r[3]);goto Lc90542c; /* cmp/hs r3,r14 */
Lc90542c: *stop=0xc90542cu;r[4]+=20u;goto Lc90542e; /* add #0x14,r4 */
Lc90542e: *stop=0xc90542eu;r[9]+=20u;goto Lc905430; /* add #0x14,r9 */
Lc905430: *stop=0xc905430u;r[12]+=20u;goto Lc905432; /* add #0x14,r12 */
Lc905432: take=(*sr&1u);*stop=0xc905434u;r[7]+=20u;if(take)goto Lc90543a;goto Lc905436; /* bt/s 0x0c90543a */
Lc905436: *stop=0xc905438u;goto Lc9051e4; /* bra 0x0c9051e4 */
Lc90543a: *stop=0xc90543au;if(!valid_ram(r[15]+32u,4u))return -3;r[1]=rd(m,r[15]+0x20u,4);goto Lc90543c; /* mov.l @(0x20,r15),r1 */
Lc90543c: *stop=0xc90543cu;r[1]+=1u;goto Lc90543e; /* add #0x1,r1 */
Lc90543e: *stop=0xc90543eu;if(!valid_ram(r[15]+32u,4u))return -3;wr(m,r[15]+0x20u,r[1],4);goto Lc905440; /* mov.l r1,@(0x20,r15) */
Lc905440: *stop=0xc905440u;if(!valid_ram(r[15]+60u,4u))return -3;r[3]=rd(m,r[15]+0x3cu,4);goto Lc905442; /* mov.l @(0x3c,r15),r3 */
Lc905442: *stop=0xc905442u;*sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[3]);goto Lc905444; /* cmp/ge r3,r1 */
Lc905444: if((*sr&1u))goto Lc90544a;goto Lc905446; /* bt 0x0c90544a */
Lc905446: *stop=0xc905448u;goto Lc904fc4; /* bra 0x0c904fc4 */
Lc90544a: *stop=0xc90544au;r[2]=rd(m,0x0c9055e0u,4);goto Lc90544c; /* mov.l 0x0c9055e0,r2 */
Lc90544c: *stop=0xc90544cu;r[1]=r[15];goto Lc90544e; /* mov r15,r1 */
Lc90544e: *stop=0xc90544eu;r[3]=rd(m,0x0c9055e4u,4);goto Lc905450; /* mov.l 0x0c9055e4,r3 */
Lc905450: *stop=0xc905450u;r[4]=r[15];goto Lc905452; /* mov r15,r4 */
Lc905452: *stop=0xc905452u;r[4]+=76u;goto Lc905454; /* add #0x4c,r4 */
Lc905454: *stop=0xc905454u;r[1]+=76u;goto Lc905456; /* add #0x4c,r1 */
Lc905456: target=r[3];*pr=0x0c90545au;*stop=0xc905458u;r[0]=88u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90545a; /* jsr @r3 */
Lc90545a: *stop=0xc90545au;r[12]=0u;goto Lc90545c; /* mov #0x0,r12 */
Lc90545c: *stop=0xc90545cu;r[11]=r[6];goto Lc90545e; /* mov r6,r11 */
Lc90545e: *stop=0xc90545eu;r[12]+=r[4];goto Lc905460; /* add r4,r12 */
Lc905460: *stop=0xc905460u;r[9]=22u;goto Lc905462; /* mov #0x16,r9 */
Lc905462: *stop=0xc905462u;r[0]=48u;goto Lc905464; /* mov #0x30,r0 */
Lc905464: *stop=0xc905464u;fcopy(f,4u,14u);goto Lc905466; /* fmov fr14,fr4 */
Lc905466: *stop=0xc905466u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto Lc905468; /* fmov.s @(r0,r15),fr3 */
Lc905468: *stop=0xc905468u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto Lc90546a; /* fldi0 fr2 */
Lc90546a: *stop=0xc90546au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto Lc90546c; /* fdiv fr3,fr4 */
Lc90546c: *stop=0xc90546cu;fcopy(f,11u,5u);goto Lc90546e; /* fmov fr5,fr11 */
Lc90546e: *stop=0xc90546eu;fcopy(f,10u,6u);goto Lc905470; /* fmov fr6,fr10 */
Lc905470: *stop=0xc905470u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,0);if(status)return status;goto Lc905472; /* fcmp/eq fr2,fr4 */
Lc905472: *stop=0xc905472u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[4],&f->fpscr,&fr[11]);if(status)return status;}goto Lc905474; /* fmul fr4,fr11 */
Lc905474: take=(*sr&1u);*stop=0xc905476u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[4],&f->fpscr,&fr[10]);if(status)return status;}if(take)goto Lc905520;goto Lc905478; /* bt/s 0x0c905520 */
Lc905478: *stop=0xc905478u;if(!valid_ram(r[12],4u))return -3;r[4]=rd(m,r[12],4);goto Lc90547a; /* mov.l @r12,r4 */
Lc90547a: *stop=0xc90547au;r[4]<<=2;goto Lc90547c; /* shll2 r4 */
Lc90547c: *stop=0xc90547cu;r[0]=r[4];goto Lc90547e; /* mov r4,r0 */
Lc90547e: *stop=0xc90547eu;if(!valid_ram(r[0]+r[13],4u))return -3;r[3]=rd(m,r[13]+r[0],4);goto Lc905480; /* mov.l @(r0,r13),r3 */
Lc905480: *stop=0xc905480u;if(!valid_ram(r[15]+40u,4u))return -3;r[0]=rd(m,r[15]+0x28u,4);goto Lc905482; /* mov.l @(0x28,r15),r0 */
Lc905482: *stop=0xc905482u;r[5]=r[4];goto Lc905484; /* mov r4,r5 */
Lc905484: *stop=0xc905484u;r[4]=r[3];goto Lc905486; /* mov r3,r4 */
Lc905486: *stop=0xc905486u;r[4]+=8u;goto Lc905488; /* add #0x8,r4 */
Lc905488: *stop=0xc905488u;if(!valid_ram(r[0]+r[5],4u))return -3;r[6]=rd(m,r[5]+r[0],4);goto Lc90548a; /* mov.l @(r0,r5),r6 */
Lc90548a: *stop=0xc90548au;r[0]=r[5];goto Lc90548c; /* mov r5,r0 */
Lc90548c: *stop=0xc90548cu;if(!valid_ram(r[0]+r[8],4u))return -3;r[7]=rd(m,r[8]+r[0],4);goto Lc90548e; /* mov.l @(r0,r8),r7 */
Lc90548e: *stop=0xc90548eu;r[6]+=8u;goto Lc905490; /* add #0x8,r6 */
Lc905490: *stop=0xc905490u;if(!valid_ram(r[0]+r[10],4u))return -3;r[14]=rd(m,r[10]+r[0],4);goto Lc905492; /* mov.l @(r0,r10),r14 */
Lc905492: *stop=0xc905492u;r[7]+=8u;goto Lc905494; /* add #0x8,r7 */
Lc905494: *stop=0xc905494u;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[6]);r[6]+=(f->fpscr&0x100000u)?8u:4u;goto Lc905496; /* fmov.s @r6+,fr4 */
Lc905496: *stop=0xc905496u;if(!valid_ram(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[7]);r[7]+=(f->fpscr&0x100000u)?8u:4u;goto Lc905498; /* fmov.s @r7+,fr3 */
Lc905498: *stop=0xc905498u;r[14]+=8u;goto Lc90549a; /* add #0x8,r14 */
Lc90549a: *stop=0xc90549au;fcopy(f,0u,4u);goto Lc90549c; /* fmov fr4,fr0 */
Lc90549c: *stop=0xc90549cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto Lc90549e; /* fmul fr5,fr3 */
Lc90549e: *stop=0xc90549eu;fcopy(f,1u,4u);goto Lc9054a0; /* fmov fr4,fr1 */
Lc9054a0: *stop=0xc9054a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto Lc9054a2; /* fmul fr7,fr1 */
Lc9054a2: *stop=0xc9054a2u;if(!valid_ram(r[15]+36u,4u))return -3;r[0]=rd(m,r[15]+0x24u,4);goto Lc9054a4; /* mov.l @(0x24,r15),r0 */
Lc9054a4: *stop=0xc9054a4u;if(!valid_ram(r[0]+r[5],4u))return -3;r[3]=rd(m,r[5]+r[0],4);goto Lc9054a6; /* mov.l @(r0,r5),r3 */
Lc9054a6: *stop=0xc9054a6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[8],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc9054a8; /* fmac fr0,fr8,fr3 */
Lc9054a8: *stop=0xc9054a8u;fcopy(f,0u,6u);goto Lc9054aa; /* fmov fr6,fr0 */
Lc9054aa: *stop=0xc9054aau;r[5]=r[3];goto Lc9054ac; /* mov r3,r5 */
Lc9054ac: *stop=0xc9054acu;r[5]+=8u;goto Lc9054ae; /* add #0x8,r5 */
Lc9054ae: *stop=0xc9054aeu;fcopy(f,2u,3u);goto Lc9054b0; /* fmov fr3,fr2 */
Lc9054b0: *stop=0xc9054b0u;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[14]);r[14]+=(f->fpscr&0x100000u)?8u:4u;goto Lc9054b2; /* fmov.s @r14+,fr3 */
Lc9054b2: *stop=0xc9054b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto Lc9054b4; /* fmac fr0,fr3,fr1 */
Lc9054b4: *stop=0xc9054b4u;fcopy(f,0u,11u);goto Lc9054b6; /* fmov fr11,fr0 */
Lc9054b6: *stop=0xc9054b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[10],&f->fpscr,&fr[1]);if(status)return status;}goto Lc9054b8; /* fmul fr10,fr1 */
Lc9054b8: *stop=0xc9054b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto Lc9054ba; /* fmac fr0,fr2,fr1 */
Lc9054ba: *stop=0xc9054bau;fcopy(f,0u,13u);goto Lc9054bc; /* fmov fr13,fr0 */
Lc9054bc: *stop=0xc9054bcu;fcopy(f,3u,1u);goto Lc9054be; /* fmov fr1,fr3 */
Lc9054be: *stop=0xc9054beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto Lc9054c0; /* fmul fr9,fr3 */
Lc9054c0: *stop=0xc9054c0u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[4]);goto Lc9054c2; /* fmov.s fr1,@r4 */
Lc9054c2: *stop=0xc9054c2u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto Lc9054c4; /* fmov.s @r5+,fr2 */
Lc9054c4: *stop=0xc9054c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc9054c6; /* fmac fr0,fr2,fr3 */
Lc9054c6: *stop=0xc9054c6u;fcopy(f,0u,8u);goto Lc9054c8; /* fmov fr8,fr0 */
Lc9054c8: *stop=0xc9054c8u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[4]);goto Lc9054ca; /* fmov.s fr3,@r4 */
Lc9054ca: *stop=0xc9054cau;r[4]+=4u;goto Lc9054cc; /* add #0x4,r4 */
Lc9054cc: *stop=0xc9054ccu;if(!valid_ram(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[7]);r[7]+=(f->fpscr&0x100000u)?8u:4u;goto Lc9054ce; /* fmov.s @r7+,fr3 */
Lc9054ce: *stop=0xc9054ceu;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[6]);r[6]+=(f->fpscr&0x100000u)?8u:4u;goto Lc9054d0; /* fmov.s @r6+,fr4 */
Lc9054d0: *stop=0xc9054d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto Lc9054d2; /* fmul fr5,fr3 */
Lc9054d2: *stop=0xc9054d2u;fcopy(f,2u,4u);goto Lc9054d4; /* fmov fr4,fr2 */
Lc9054d4: *stop=0xc9054d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto Lc9054d6; /* fmul fr7,fr2 */
Lc9054d6: *stop=0xc9054d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc9054d8; /* fmac fr0,fr4,fr3 */
Lc9054d8: *stop=0xc9054d8u;fcopy(f,0u,6u);goto Lc9054da; /* fmov fr6,fr0 */
Lc9054da: *stop=0xc9054dau;fcopy(f,1u,3u);goto Lc9054dc; /* fmov fr3,fr1 */
Lc9054dc: *stop=0xc9054dcu;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[14]);r[14]+=(f->fpscr&0x100000u)?8u:4u;goto Lc9054de; /* fmov.s @r14+,fr3 */
Lc9054de: *stop=0xc9054deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto Lc9054e0; /* fmac fr0,fr3,fr2 */
Lc9054e0: *stop=0xc9054e0u;fcopy(f,0u,11u);goto Lc9054e2; /* fmov fr11,fr0 */
Lc9054e2: *stop=0xc9054e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[10],&f->fpscr,&fr[2]);if(status)return status;}goto Lc9054e4; /* fmul fr10,fr2 */
Lc9054e4: *stop=0xc9054e4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto Lc9054e6; /* fmac fr0,fr1,fr2 */
Lc9054e6: *stop=0xc9054e6u;fcopy(f,0u,13u);goto Lc9054e8; /* fmov fr13,fr0 */
Lc9054e8: *stop=0xc9054e8u;fcopy(f,3u,2u);goto Lc9054ea; /* fmov fr2,fr3 */
Lc9054ea: *stop=0xc9054eau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto Lc9054ec; /* fmul fr9,fr3 */
Lc9054ec: *stop=0xc9054ecu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[4]);goto Lc9054ee; /* fmov.s fr2,@r4 */
Lc9054ee: *stop=0xc9054eeu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto Lc9054f0; /* fmov.s @r5+,fr2 */
Lc9054f0: *stop=0xc9054f0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc9054f2; /* fmac fr0,fr2,fr3 */
Lc9054f2: *stop=0xc9054f2u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[4]);goto Lc9054f4; /* fmov.s fr3,@r4 */
Lc9054f4: *stop=0xc9054f4u;r[4]+=4u;goto Lc9054f6; /* add #0x4,r4 */
Lc9054f6: *stop=0xc9054f6u;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[6]);goto Lc9054f8; /* fmov.s @r6,fr4 */
Lc9054f8: *stop=0xc9054f8u;if(!valid_ram(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[7]);goto Lc9054fa; /* fmov.s @r7,fr3 */
Lc9054fa: *stop=0xc9054fau;fcopy(f,0u,8u);goto Lc9054fc; /* fmov fr8,fr0 */
Lc9054fc: *stop=0xc9054fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto Lc9054fe; /* fmul fr5,fr3 */
Lc9054fe: *stop=0xc9054feu;fcopy(f,2u,4u);goto Lc905500; /* fmov fr4,fr2 */
Lc905500: *stop=0xc905500u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905502; /* fmul fr7,fr2 */
Lc905502: *stop=0xc905502u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905504; /* fmac fr0,fr4,fr3 */
Lc905504: *stop=0xc905504u;fcopy(f,0u,6u);goto Lc905506; /* fmov fr6,fr0 */
Lc905506: *stop=0xc905506u;fcopy(f,1u,3u);goto Lc905508; /* fmov fr3,fr1 */
Lc905508: *stop=0xc905508u;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[14]);goto Lc90550a; /* fmov.s @r14,fr3 */
Lc90550a: *stop=0xc90550au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto Lc90550c; /* fmac fr0,fr3,fr2 */
Lc90550c: *stop=0xc90550cu;fcopy(f,0u,11u);goto Lc90550e; /* fmov fr11,fr0 */
Lc90550e: *stop=0xc90550eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[10],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905510; /* fmul fr10,fr2 */
Lc905510: *stop=0xc905510u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto Lc905512; /* fmac fr0,fr1,fr2 */
Lc905512: *stop=0xc905512u;fcopy(f,0u,13u);goto Lc905514; /* fmov fr13,fr0 */
Lc905514: *stop=0xc905514u;fcopy(f,3u,2u);goto Lc905516; /* fmov fr2,fr3 */
Lc905516: *stop=0xc905516u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto Lc905518; /* fmul fr9,fr3 */
Lc905518: *stop=0xc905518u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[4]);goto Lc90551a; /* fmov.s fr2,@r4 */
Lc90551a: *stop=0xc90551au;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[5]);goto Lc90551c; /* fmov.s @r5,fr2 */
Lc90551c: *stop=0xc90551cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto Lc90551e; /* fmac fr0,fr2,fr3 */
Lc90551e: *stop=0xc90551eu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[4]);goto Lc905520; /* fmov.s fr3,@r4 */
Lc905520: *stop=0xc905520u;r[11]+=1u;goto Lc905522; /* add #0x1,r11 */
Lc905522: *stop=0xc905522u;*sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[9]);goto Lc905524; /* cmp/ge r9,r11 */
Lc905524: take=!(*sr&1u);*stop=0xc905526u;r[12]+=4u;if(take)goto Lc905462;goto Lc905528; /* bf/s 0x0c905462 */
Lc905528: *stop=0xc905528u;r[1]=4294967204u;goto Lc90552a; /* mov #-0x5c,r1 */
Lc90552a: *stop=0xc90552au;r[1]=r[1]&255u;goto Lc90552c; /* extu.b r1,r1 */
Lc90552c: *stop=0xc90552cu;r[15]+=r[1];goto Lc90552e; /* add r1,r15 */
Lc90552e: *stop=0xc90552eu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto Lc905530; /* lds.l @r15+,PR */
Lc905530: *stop=0xc905530u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc905532; /* fmov.s @r15+,fr12 */
Lc905532: *stop=0xc905532u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc905534; /* fmov.s @r15+,fr13 */
Lc905534: *stop=0xc905534u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc905536; /* fmov.s @r15+,fr14 */
Lc905536: *stop=0xc905536u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto Lc905538; /* fmov.s @r15+,fr15 */
Lc905538: *stop=0xc905538u;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto Lc90553a; /* mov.l @r15+,r8 */
Lc90553a: *stop=0xc90553au;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto Lc90553c; /* mov.l @r15+,r9 */
Lc90553c: *stop=0xc90553cu;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto Lc90553e; /* mov.l @r15+,r10 */
Lc90553e: *stop=0xc90553eu;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto Lc905540; /* mov.l @r15+,r11 */
Lc905540: *stop=0xc905540u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto Lc905542; /* mov.l @r15+,r12 */
Lc905542: *stop=0xc905542u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto Lc905544; /* mov.l @r15+,r13 */
Lc905544: *stop=0xc905544u;if(*pr!=entry_pr)return -3;*stop=0xc905546u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_word_copy_dispatch_8c10cddc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10cddc;
L8c10cddc: *stop=0x8c10cddcu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[3],4);goto L8c10cdde; /* mov.l r3,@-r15 */
L8c10cdde: *stop=0x8c10cddeu;r[3]=64u;goto L8c10cde0; /* mov #0x40,r3 */
L8c10cde0: *stop=0x8c10cde0u;*sr=(*sr&~1u)|(r[3]>=r[0]);goto L8c10cde2; /* cmp/hs r0,r3 */
L8c10cde2: if(!(*sr&1u))goto L8c10ce7c;goto L8c10cde4; /* bf 0x8c10ce7c */
L8c10cde4: *stop=0x8c10cde4u;r[3]=rd(m,0x8c10cdecu,4);goto L8c10cde6; /* mov.l 0x8c10cdec,r3 */
L8c10cde6: *stop=0x8c10cde6u;if(!valid_ram(r[0]+r[3],4u))return -3;r[3]=rd(m,r[3]+r[0],4);goto L8c10cde8; /* mov.l @(r0,r3),r3 */
L8c10cde8: target=r[3];*stop=0x8c10cdeau;switch(target){case 0x8c10cdf2u:goto L8c10cdf2;case 0x8c10cdf6u:goto L8c10cdf6;case 0x8c10cdfau:goto L8c10cdfa;case 0x8c10cdfeu:goto L8c10cdfe;case 0x8c10ce02u:goto L8c10ce02;case 0x8c10ce06u:goto L8c10ce06;case 0x8c10ce0au:goto L8c10ce0a;case 0x8c10ce0eu:goto L8c10ce0e;case 0x8c10ce12u:goto L8c10ce12;case 0x8c10ce16u:goto L8c10ce16;case 0x8c10ce1au:goto L8c10ce1a;case 0x8c10ce1eu:goto L8c10ce1e;case 0x8c10ce22u:goto L8c10ce22;case 0x8c10ce26u:goto L8c10ce26;case 0x8c10ce2au:goto L8c10ce2a;case 0x8c10ce2eu:goto L8c10ce2e;case 0x8c10ce32u:goto L8c10ce32;default:*stop=0x8c10cde8u;return -3;} /* jmp @r3 */
L8c10cdf2: *stop=0x8c10cdf2u;if(!valid_ram(r[2]+60u,4u))return -3;r[0]=rd(m,r[2]+0x3cu,4);goto L8c10cdf4; /* mov.l @(0x3c,r2),r0 */
L8c10cdf4: *stop=0x8c10cdf4u;if(!valid_ram(r[1]+60u,4u))return -3;wr(m,r[1]+0x3cu,r[0],4);goto L8c10cdf6; /* mov.l r0,@(0x3c,r1) */
L8c10cdf6: *stop=0x8c10cdf6u;if(!valid_ram(r[2]+56u,4u))return -3;r[0]=rd(m,r[2]+0x38u,4);goto L8c10cdf8; /* mov.l @(0x38,r2),r0 */
L8c10cdf8: *stop=0x8c10cdf8u;if(!valid_ram(r[1]+56u,4u))return -3;wr(m,r[1]+0x38u,r[0],4);goto L8c10cdfa; /* mov.l r0,@(0x38,r1) */
L8c10cdfa: *stop=0x8c10cdfau;if(!valid_ram(r[2]+52u,4u))return -3;r[0]=rd(m,r[2]+0x34u,4);goto L8c10cdfc; /* mov.l @(0x34,r2),r0 */
L8c10cdfc: *stop=0x8c10cdfcu;if(!valid_ram(r[1]+52u,4u))return -3;wr(m,r[1]+0x34u,r[0],4);goto L8c10cdfe; /* mov.l r0,@(0x34,r1) */
L8c10cdfe: *stop=0x8c10cdfeu;if(!valid_ram(r[2]+48u,4u))return -3;r[0]=rd(m,r[2]+0x30u,4);goto L8c10ce00; /* mov.l @(0x30,r2),r0 */
L8c10ce00: *stop=0x8c10ce00u;if(!valid_ram(r[1]+48u,4u))return -3;wr(m,r[1]+0x30u,r[0],4);goto L8c10ce02; /* mov.l r0,@(0x30,r1) */
L8c10ce02: *stop=0x8c10ce02u;if(!valid_ram(r[2]+44u,4u))return -3;r[0]=rd(m,r[2]+0x2cu,4);goto L8c10ce04; /* mov.l @(0x2c,r2),r0 */
L8c10ce04: *stop=0x8c10ce04u;if(!valid_ram(r[1]+44u,4u))return -3;wr(m,r[1]+0x2cu,r[0],4);goto L8c10ce06; /* mov.l r0,@(0x2c,r1) */
L8c10ce06: *stop=0x8c10ce06u;if(!valid_ram(r[2]+40u,4u))return -3;r[0]=rd(m,r[2]+0x28u,4);goto L8c10ce08; /* mov.l @(0x28,r2),r0 */
L8c10ce08: *stop=0x8c10ce08u;if(!valid_ram(r[1]+40u,4u))return -3;wr(m,r[1]+0x28u,r[0],4);goto L8c10ce0a; /* mov.l r0,@(0x28,r1) */
L8c10ce0a: *stop=0x8c10ce0au;if(!valid_ram(r[2]+36u,4u))return -3;r[0]=rd(m,r[2]+0x24u,4);goto L8c10ce0c; /* mov.l @(0x24,r2),r0 */
L8c10ce0c: *stop=0x8c10ce0cu;if(!valid_ram(r[1]+36u,4u))return -3;wr(m,r[1]+0x24u,r[0],4);goto L8c10ce0e; /* mov.l r0,@(0x24,r1) */
L8c10ce0e: *stop=0x8c10ce0eu;if(!valid_ram(r[2]+32u,4u))return -3;r[0]=rd(m,r[2]+0x20u,4);goto L8c10ce10; /* mov.l @(0x20,r2),r0 */
L8c10ce10: *stop=0x8c10ce10u;if(!valid_ram(r[1]+32u,4u))return -3;wr(m,r[1]+0x20u,r[0],4);goto L8c10ce12; /* mov.l r0,@(0x20,r1) */
L8c10ce12: *stop=0x8c10ce12u;if(!valid_ram(r[2]+28u,4u))return -3;r[0]=rd(m,r[2]+0x1cu,4);goto L8c10ce14; /* mov.l @(0x1c,r2),r0 */
L8c10ce14: *stop=0x8c10ce14u;if(!valid_ram(r[1]+28u,4u))return -3;wr(m,r[1]+0x1cu,r[0],4);goto L8c10ce16; /* mov.l r0,@(0x1c,r1) */
L8c10ce16: *stop=0x8c10ce16u;if(!valid_ram(r[2]+24u,4u))return -3;r[0]=rd(m,r[2]+0x18u,4);goto L8c10ce18; /* mov.l @(0x18,r2),r0 */
L8c10ce18: *stop=0x8c10ce18u;if(!valid_ram(r[1]+24u,4u))return -3;wr(m,r[1]+0x18u,r[0],4);goto L8c10ce1a; /* mov.l r0,@(0x18,r1) */
L8c10ce1a: *stop=0x8c10ce1au;if(!valid_ram(r[2]+20u,4u))return -3;r[0]=rd(m,r[2]+0x14u,4);goto L8c10ce1c; /* mov.l @(0x14,r2),r0 */
L8c10ce1c: *stop=0x8c10ce1cu;if(!valid_ram(r[1]+20u,4u))return -3;wr(m,r[1]+0x14u,r[0],4);goto L8c10ce1e; /* mov.l r0,@(0x14,r1) */
L8c10ce1e: *stop=0x8c10ce1eu;if(!valid_ram(r[2]+16u,4u))return -3;r[0]=rd(m,r[2]+0x10u,4);goto L8c10ce20; /* mov.l @(0x10,r2),r0 */
L8c10ce20: *stop=0x8c10ce20u;if(!valid_ram(r[1]+16u,4u))return -3;wr(m,r[1]+0x10u,r[0],4);goto L8c10ce22; /* mov.l r0,@(0x10,r1) */
L8c10ce22: *stop=0x8c10ce22u;if(!valid_ram(r[2]+12u,4u))return -3;r[0]=rd(m,r[2]+0xcu,4);goto L8c10ce24; /* mov.l @(0xc,r2),r0 */
L8c10ce24: *stop=0x8c10ce24u;if(!valid_ram(r[1]+12u,4u))return -3;wr(m,r[1]+0xcu,r[0],4);goto L8c10ce26; /* mov.l r0,@(0xc,r1) */
L8c10ce26: *stop=0x8c10ce26u;if(!valid_ram(r[2]+8u,4u))return -3;r[0]=rd(m,r[2]+0x8u,4);goto L8c10ce28; /* mov.l @(0x8,r2),r0 */
L8c10ce28: *stop=0x8c10ce28u;if(!valid_ram(r[1]+8u,4u))return -3;wr(m,r[1]+0x8u,r[0],4);goto L8c10ce2a; /* mov.l r0,@(0x8,r1) */
L8c10ce2a: *stop=0x8c10ce2au;if(!valid_ram(r[2]+4u,4u))return -3;r[0]=rd(m,r[2]+0x4u,4);goto L8c10ce2c; /* mov.l @(0x4,r2),r0 */
L8c10ce2c: *stop=0x8c10ce2cu;if(!valid_ram(r[1]+4u,4u))return -3;wr(m,r[1]+0x4u,r[0],4);goto L8c10ce2e; /* mov.l r0,@(0x4,r1) */
L8c10ce2e: *stop=0x8c10ce2eu;if(!valid_ram(r[2],4u))return -3;r[0]=rd(m,r[2],4);goto L8c10ce30; /* mov.l @r2,r0 */
L8c10ce30: *stop=0x8c10ce30u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[0],4);goto L8c10ce32; /* mov.l r0,@r1 */
L8c10ce32: *stop=0x8c10ce32u;if(*pr!=entry_pr)return -3;*stop=0x8c10ce34u;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c10ce7c: *stop=0x8c10ce7cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[2],4);goto L8c10ce7e; /* mov.l r2,@-r15 */
L8c10ce7e: *stop=0x8c10ce7eu;r[3]=r[2];goto L8c10ce80; /* mov r2,r3 */
L8c10ce80: *stop=0x8c10ce80u;r[3]+=r[0];goto L8c10ce82; /* add r0,r3 */
L8c10ce82: *stop=0x8c10ce82u;if(!valid_ram(r[2],4u))return -3;r[0]=rd(m,r[2],4);r[2]+=4;goto L8c10ce84; /* mov.l @r2+,r0 */
L8c10ce84: *stop=0x8c10ce84u;*sr=(*sr&~1u)|(r[3]>=r[2]);goto L8c10ce86; /* cmp/hs r2,r3 */
L8c10ce86: if(!(*sr&1u))goto L8c10ce96;goto L8c10ce88; /* bf 0x8c10ce96 */
L8c10ce88: *stop=0x8c10ce88u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[0],4);goto L8c10ce8a; /* mov.l r0,@r1 */
L8c10ce8a: *stop=0x8c10ce8au;if(!valid_ram(r[2],4u))return -3;r[0]=rd(m,r[2],4);r[2]+=4;goto L8c10ce8c; /* mov.l @r2+,r0 */
L8c10ce8c: *stop=0x8c10ce8cu;*sr=(*sr&~1u)|(r[3]>=r[2]);goto L8c10ce8e; /* cmp/hs r2,r3 */
L8c10ce8e: if(!(*sr&1u))goto L8c10ce96;goto L8c10ce90; /* bf 0x8c10ce96 */
L8c10ce90: *stop=0x8c10ce90u;if(!valid_ram(r[1]+4u,4u))return -3;wr(m,r[1]+0x4u,r[0],4);goto L8c10ce92; /* mov.l r0,@(0x4,r1) */
L8c10ce92: *stop=0x8c10ce94u;r[1]+=8u;goto L8c10ce82; /* bra 0x8c10ce82 */
L8c10ce96: *stop=0x8c10ce96u;if(!valid_ram(r[15],4u))return -3;r[2]=rd(m,r[15],4);r[15]+=4;goto L8c10ce98; /* mov.l @r15+,r2 */
L8c10ce98: *stop=0x8c10ce98u;if(*pr!=entry_pr)return -3;*stop=0x8c10ce9au;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
