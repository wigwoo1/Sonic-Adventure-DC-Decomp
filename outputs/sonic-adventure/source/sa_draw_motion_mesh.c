/* Motion mesh matrix/light preprocessing and wrapper; names inferred.
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
SA_STAGE_EXPORT int sa_motion_mesh_prepare_8c617c34(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c617c34;
L8c617c34: *stop=0x8c617c34u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c617c36; /* mov.l r14,@-r15 */
L8c617c36: *stop=0x8c617c36u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c617c38; /* mov.l r13,@-r15 */
L8c617c38: *stop=0x8c617c38u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c617c3a; /* mov.l r12,@-r15 */
L8c617c3a: *stop=0x8c617c3au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c617c3c; /* mov.l r11,@-r15 */
L8c617c3c: *stop=0x8c617c3cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c617c3e; /* mov.l r10,@-r15 */
L8c617c3e: *stop=0x8c617c3eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto L8c617c40; /* mov.l r9,@-r15 */
L8c617c40: *stop=0x8c617c40u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c617c42; /* sts.l PR,@-r15 */
L8c617c42: *stop=0x8c617c42u;r[15]+=4294967184u;goto L8c617c44; /* add #-0x70,r15 */
L8c617c44: *stop=0x8c617c44u;r[5]=rd(m,0x8c617e14u,4);goto L8c617c46; /* mov.l 0x8c617e14,r5 */
L8c617c46: *stop=0x8c617c46u;r[14]=r[15];goto L8c617c48; /* mov r15,r14 */
L8c617c48: *stop=0x8c617c48u;r[6]=rd(m,0x8c617e18u,4);goto L8c617c4a; /* mov.l 0x8c617e18,r6 */
L8c617c4a: *stop=0x8c617c4au;r[14]+=48u;goto L8c617c4c; /* add #0x30,r14 */
L8c617c4c: *stop=0x8c617c4cu;if(!valid_ram(r[5],4u))return -3;r[5]=rd(m,r[5],4);goto L8c617c4e; /* mov.l @r5,r5 */
L8c617c4e: *stop=0x8c617c4eu;r[1]=r[6];goto L8c617c50; /* mov r6,r1 */
L8c617c50: *stop=0x8c617c50u;r[1]+=32u;goto L8c617c52; /* add #0x20,r1 */
L8c617c52: *stop=0x8c617c52u;r[7]=r[5];goto L8c617c54; /* mov r5,r7 */
L8c617c54: *stop=0x8c617c54u;r[2]=r[7];goto L8c617c56; /* mov r7,r2 */
L8c617c56: *stop=0x8c617c56u;r[2]+=8u;goto L8c617c58; /* add #0x8,r2 */
L8c617c58: *stop=0x8c617c58u;r[9]=r[5];goto L8c617c5a; /* mov r5,r9 */
L8c617c5a: *stop=0x8c617c5au;r[3]=r[6];goto L8c617c5c; /* mov r6,r3 */
L8c617c5c: *stop=0x8c617c5cu;r[3]+=16u;goto L8c617c5e; /* add #0x10,r3 */
L8c617c5e: *stop=0x8c617c5eu;r[9]+=4u;goto L8c617c60; /* add #0x4,r9 */
L8c617c60: *stop=0x8c617c60u;if(!valid_ram(r[15]+12u,4u))return -3;wr(m,r[15]+0xcu,r[3],4);goto L8c617c62; /* mov.l r3,@(0xc,r15) */
L8c617c62: *stop=0x8c617c62u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[2],4);goto L8c617c64; /* mov.l r2,@r15 */
L8c617c64: *stop=0x8c617c64u;if(!valid_ram(r[15]+16u,4u))return -3;wr(m,r[15]+0x10u,r[1],4);goto L8c617c66; /* mov.l r1,@(0x10,r15) */
L8c617c66: *stop=0x8c617c66u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[1]);goto L8c617c68; /* fmov.s @r1,fr0 */
L8c617c68: *stop=0x8c617c68u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[2]);goto L8c617c6a; /* fmov.s @r2,fr3 */
L8c617c6a: *stop=0x8c617c6au;r[2]=r[6];goto L8c617c6c; /* mov r6,r2 */
L8c617c6c: *stop=0x8c617c6cu;if(!valid_ram(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[9]);goto L8c617c6e; /* fmov.s @r9,fr2 */
L8c617c6e: *stop=0x8c617c6eu;r[2]+=20u;goto L8c617c70; /* add #0x14,r2 */
L8c617c70: *stop=0x8c617c70u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617c72; /* fmul fr0,fr3 */
L8c617c72: *stop=0x8c617c72u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[3]);goto L8c617c74; /* fmov.s @r3,fr0 */
L8c617c74: *stop=0x8c617c74u;if(!valid_ram(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[7]);goto L8c617c76; /* fmov.s @r7,fr1 */
L8c617c76: *stop=0x8c617c76u;r[3]=r[14];goto L8c617c78; /* mov r14,r3 */
L8c617c78: *stop=0x8c617c78u;r[3]+=4u;goto L8c617c7a; /* add #0x4,r3 */
L8c617c7a: *stop=0x8c617c7au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617c7c; /* fmac fr0,fr2,fr3 */
L8c617c7c: *stop=0x8c617c7cu;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[6]);goto L8c617c7e; /* fmov.s @r6,fr0 */
L8c617c7e: *stop=0x8c617c7eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617c80; /* fmac fr0,fr1,fr3 */
L8c617c80: *stop=0x8c617c80u;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[14]);goto L8c617c82; /* fmov.s fr3,@r14 */
L8c617c82: *stop=0x8c617c82u;if(!valid_ram(r[15]+40u,4u))return -3;wr(m,r[15]+0x28u,r[3],4);goto L8c617c84; /* mov.l r3,@(0x28,r15) */
L8c617c84: *stop=0x8c617c84u;r[0]=r[6];goto L8c617c86; /* mov r6,r0 */
L8c617c86: *stop=0x8c617c86u;r[0]+=36u;goto L8c617c88; /* add #0x24,r0 */
L8c617c88: *stop=0x8c617c88u;r[10]=r[6];goto L8c617c8a; /* mov r6,r10 */
L8c617c8a: *stop=0x8c617c8au;r[10]+=4u;goto L8c617c8c; /* add #0x4,r10 */
L8c617c8c: *stop=0x8c617c8cu;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[2],4);goto L8c617c8e; /* mov.l r2,@(0x8,r15) */
L8c617c8e: *stop=0x8c617c8eu;r[12]=r[6];goto L8c617c90; /* mov r6,r12 */
L8c617c90: *stop=0x8c617c90u;if(!valid_ram(r[15],4u))return -3;r[1]=rd(m,r[15],4);goto L8c617c92; /* mov.l @r15,r1 */
L8c617c92: *stop=0x8c617c92u;r[12]+=40u;goto L8c617c94; /* add #0x28,r12 */
L8c617c94: *stop=0x8c617c94u;r[11]=r[6];goto L8c617c96; /* mov r6,r11 */
L8c617c96: *stop=0x8c617c96u;r[11]+=24u;goto L8c617c98; /* add #0x18,r11 */
L8c617c98: *stop=0x8c617c98u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto L8c617c9a; /* fmov.s @r1,fr3 */
L8c617c9a: *stop=0x8c617c9au;r[13]=r[6];goto L8c617c9c; /* mov r6,r13 */
L8c617c9c: *stop=0x8c617c9cu;if(!valid_ram(r[15]+20u,4u))return -3;wr(m,r[15]+0x14u,r[0],4);goto L8c617c9e; /* mov.l r0,@(0x14,r15) */
L8c617c9e: *stop=0x8c617c9eu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]);goto L8c617ca0; /* fmov.s @r0,fr0 */
L8c617ca0: *stop=0x8c617ca0u;if(!valid_ram(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[9]);goto L8c617ca2; /* fmov.s @r9,fr2 */
L8c617ca2: *stop=0x8c617ca2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617ca4; /* fmul fr0,fr3 */
L8c617ca4: *stop=0x8c617ca4u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[2]);goto L8c617ca6; /* fmov.s @r2,fr0 */
L8c617ca6: *stop=0x8c617ca6u;if(!valid_ram(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[10]);goto L8c617ca8; /* fmov.s @r10,fr1 */
L8c617ca8: *stop=0x8c617ca8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617caa; /* fmac fr0,fr2,fr3 */
L8c617caa: *stop=0x8c617caau;if(!valid_ram(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[7]);goto L8c617cac; /* fmov.s @r7,fr0 */
L8c617cac: *stop=0x8c617cacu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617cae; /* fmac fr0,fr1,fr3 */
L8c617cae: *stop=0x8c617caeu;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[3]);goto L8c617cb0; /* fmov.s fr3,@r3 */
L8c617cb0: *stop=0x8c617cb0u;r[3]=r[14];goto L8c617cb2; /* mov r14,r3 */
L8c617cb2: *stop=0x8c617cb2u;r[3]+=8u;goto L8c617cb4; /* add #0x8,r3 */
L8c617cb4: *stop=0x8c617cb4u;if(!valid_ram(r[15]+44u,4u))return -3;wr(m,r[15]+0x2cu,r[3],4);goto L8c617cb6; /* mov.l r3,@(0x2c,r15) */
L8c617cb6: *stop=0x8c617cb6u;if(!valid_ram(r[15],4u))return -3;r[2]=rd(m,r[15],4);goto L8c617cb8; /* mov.l @r15,r2 */
L8c617cb8: *stop=0x8c617cb8u;if(!valid_ram(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[12]);goto L8c617cba; /* fmov.s @r12,fr0 */
L8c617cba: *stop=0x8c617cbau;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[2]);goto L8c617cbc; /* fmov.s @r2,fr3 */
L8c617cbc: *stop=0x8c617cbcu;if(!valid_ram(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[9]);goto L8c617cbe; /* fmov.s @r9,fr2 */
L8c617cbe: *stop=0x8c617cbeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617cc0; /* fmul fr0,fr3 */
L8c617cc0: *stop=0x8c617cc0u;if(!valid_ram(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[11]);goto L8c617cc2; /* fmov.s @r11,fr0 */
L8c617cc2: *stop=0x8c617cc2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617cc4; /* fmac fr0,fr2,fr3 */
L8c617cc4: *stop=0x8c617cc4u;r[13]+=8u;goto L8c617cc6; /* add #0x8,r13 */
L8c617cc6: *stop=0x8c617cc6u;if(!valid_ram(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[7]);goto L8c617cc8; /* fmov.s @r7,fr0 */
L8c617cc8: *stop=0x8c617cc8u;if(!valid_ram(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[13]);goto L8c617cca; /* fmov.s @r13,fr1 */
L8c617cca: *stop=0x8c617ccau;r[1]=r[7];goto L8c617ccc; /* mov r7,r1 */
L8c617ccc: *stop=0x8c617cccu;r[1]+=24u;goto L8c617cce; /* add #0x18,r1 */
L8c617cce: *stop=0x8c617cceu;r[9]=r[7];goto L8c617cd0; /* mov r7,r9 */
L8c617cd0: *stop=0x8c617cd0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617cd2; /* fmac fr0,fr1,fr3 */
L8c617cd2: *stop=0x8c617cd2u;r[9]+=20u;goto L8c617cd4; /* add #0x14,r9 */
L8c617cd4: *stop=0x8c617cd4u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[3]);goto L8c617cd6; /* fmov.s fr3,@r3 */
L8c617cd6: *stop=0x8c617cd6u;r[3]=r[14];goto L8c617cd8; /* mov r14,r3 */
L8c617cd8: *stop=0x8c617cd8u;r[3]+=16u;goto L8c617cda; /* add #0x10,r3 */
L8c617cda: *stop=0x8c617cdau;if(!valid_ram(r[15]+24u,4u))return -3;wr(m,r[15]+0x18u,r[3],4);goto L8c617cdc; /* mov.l r3,@(0x18,r15) */
L8c617cdc: *stop=0x8c617cdcu;if(!valid_ram(r[15]+12u,4u))return -3;r[2]=rd(m,r[15]+0xcu,4);goto L8c617cde; /* mov.l @(0xc,r15),r2 */
L8c617cde: *stop=0x8c617cdeu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[1],4);goto L8c617ce0; /* mov.l r1,@r15 */
L8c617ce0: *stop=0x8c617ce0u;if(!valid_ram(r[15]+16u,4u))return -3;r[0]=rd(m,r[15]+0x10u,4);goto L8c617ce2; /* mov.l @(0x10,r15),r0 */
L8c617ce2: *stop=0x8c617ce2u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto L8c617ce4; /* fmov.s @r1,fr3 */
L8c617ce4: *stop=0x8c617ce4u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]);goto L8c617ce6; /* fmov.s @r0,fr0 */
L8c617ce6: *stop=0x8c617ce6u;r[0]=r[7];goto L8c617ce8; /* mov r7,r0 */
L8c617ce8: *stop=0x8c617ce8u;if(!valid_ram(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[9]);goto L8c617cea; /* fmov.s @r9,fr2 */
L8c617cea: *stop=0x8c617ceau;r[0]+=16u;goto L8c617cec; /* add #0x10,r0 */
L8c617cec: *stop=0x8c617cecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617cee; /* fmul fr0,fr3 */
L8c617cee: *stop=0x8c617ceeu;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[2]);goto L8c617cf0; /* fmov.s @r2,fr0 */
L8c617cf0: *stop=0x8c617cf0u;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[0],4);goto L8c617cf2; /* mov.l r0,@(0x4,r15) */
L8c617cf2: *stop=0x8c617cf2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617cf4; /* fmac fr0,fr2,fr3 */
L8c617cf4: *stop=0x8c617cf4u;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[6]);goto L8c617cf6; /* fmov.s @r6,fr0 */
L8c617cf6: *stop=0x8c617cf6u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto L8c617cf8; /* fmov.s @r0,fr2 */
L8c617cf8: *stop=0x8c617cf8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617cfa; /* fmac fr0,fr2,fr3 */
L8c617cfa: *stop=0x8c617cfau;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[3]);goto L8c617cfc; /* fmov.s fr3,@r3 */
L8c617cfc: *stop=0x8c617cfcu;r[3]=r[14];goto L8c617cfe; /* mov r14,r3 */
L8c617cfe: *stop=0x8c617cfeu;r[3]+=20u;goto L8c617d00; /* add #0x14,r3 */
L8c617d00: *stop=0x8c617d00u;if(!valid_ram(r[15]+36u,4u))return -3;wr(m,r[15]+0x24u,r[3],4);goto L8c617d02; /* mov.l r3,@(0x24,r15) */
L8c617d02: *stop=0x8c617d02u;if(!valid_ram(r[15],4u))return -3;r[2]=rd(m,r[15],4);goto L8c617d04; /* mov.l @r15,r2 */
L8c617d04: *stop=0x8c617d04u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[2]);goto L8c617d06; /* fmov.s @r2,fr1 */
L8c617d06: *stop=0x8c617d06u;if(!valid_ram(r[15]+8u,4u))return -3;r[2]=rd(m,r[15]+0x8u,4);goto L8c617d08; /* mov.l @(0x8,r15),r2 */
L8c617d08: *stop=0x8c617d08u;if(!valid_ram(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[9]);goto L8c617d0a; /* fmov.s @r9,fr3 */
L8c617d0a: *stop=0x8c617d0au;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[2]);goto L8c617d0c; /* fmov.s @r2,fr0 */
L8c617d0c: *stop=0x8c617d0cu;if(!valid_ram(r[15]+4u,4u))return -3;r[0]=rd(m,r[15]+0x4u,4);goto L8c617d0e; /* mov.l @(0x4,r15),r0 */
L8c617d0e: *stop=0x8c617d0eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617d10; /* fmul fr0,fr3 */
L8c617d10: *stop=0x8c617d10u;if(!valid_ram(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[10]);goto L8c617d12; /* fmov.s @r10,fr0 */
L8c617d12: *stop=0x8c617d12u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto L8c617d14; /* fmov.s @r0,fr2 */
L8c617d14: *stop=0x8c617d14u;if(!valid_ram(r[15]+20u,4u))return -3;r[1]=rd(m,r[15]+0x14u,4);goto L8c617d16; /* mov.l @(0x14,r15),r1 */
L8c617d16: *stop=0x8c617d16u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617d18; /* fmac fr0,fr2,fr3 */
L8c617d18: *stop=0x8c617d18u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[1]);goto L8c617d1a; /* fmov.s @r1,fr0 */
L8c617d1a: *stop=0x8c617d1au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617d1c; /* fmac fr0,fr1,fr3 */
L8c617d1c: *stop=0x8c617d1cu;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[3]);goto L8c617d1e; /* fmov.s fr3,@r3 */
L8c617d1e: *stop=0x8c617d1eu;r[3]=r[14];goto L8c617d20; /* mov r14,r3 */
L8c617d20: *stop=0x8c617d20u;r[3]+=24u;goto L8c617d22; /* add #0x18,r3 */
L8c617d22: *stop=0x8c617d22u;if(!valid_ram(r[15]+28u,4u))return -3;wr(m,r[15]+0x1cu,r[3],4);goto L8c617d24; /* mov.l r3,@(0x1c,r15) */
L8c617d24: *stop=0x8c617d24u;if(!valid_ram(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[11]);goto L8c617d26; /* fmov.s @r11,fr0 */
L8c617d26: *stop=0x8c617d26u;if(!valid_ram(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[9]);goto L8c617d28; /* fmov.s @r9,fr3 */
L8c617d28: *stop=0x8c617d28u;if(!valid_ram(r[15]+4u,4u))return -3;r[1]=rd(m,r[15]+0x4u,4);goto L8c617d2a; /* mov.l @(0x4,r15),r1 */
L8c617d2a: *stop=0x8c617d2au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617d2c; /* fmul fr0,fr3 */
L8c617d2c: *stop=0x8c617d2cu;if(!valid_ram(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[13]);goto L8c617d2e; /* fmov.s @r13,fr0 */
L8c617d2e: *stop=0x8c617d2eu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto L8c617d30; /* fmov.s @r1,fr2 */
L8c617d30: *stop=0x8c617d30u;if(!valid_ram(r[15],4u))return -3;r[2]=rd(m,r[15],4);goto L8c617d32; /* mov.l @r15,r2 */
L8c617d32: *stop=0x8c617d32u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617d34; /* fmac fr0,fr2,fr3 */
L8c617d34: *stop=0x8c617d34u;if(!valid_ram(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[12]);goto L8c617d36; /* fmov.s @r12,fr0 */
L8c617d36: *stop=0x8c617d36u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[2]);goto L8c617d38; /* fmov.s @r2,fr1 */
L8c617d38: *stop=0x8c617d38u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617d3a; /* fmac fr0,fr1,fr3 */
L8c617d3a: *stop=0x8c617d3au;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[3]);goto L8c617d3c; /* fmov.s fr3,@r3 */
L8c617d3c: *stop=0x8c617d3cu;r[3]=r[14];goto L8c617d3e; /* mov r14,r3 */
L8c617d3e: *stop=0x8c617d3eu;r[3]+=32u;goto L8c617d40; /* add #0x20,r3 */
L8c617d40: *stop=0x8c617d40u;if(!valid_ram(r[15]+32u,4u))return -3;wr(m,r[15]+0x20u,r[3],4);goto L8c617d42; /* mov.l r3,@(0x20,r15) */
L8c617d42: *stop=0x8c617d42u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[3],4);goto L8c617d44; /* mov.l r3,@-r15 */
L8c617d44: *stop=0x8c617d44u;r[0]=r[7];goto L8c617d46; /* mov r7,r0 */
L8c617d46: *stop=0x8c617d46u;r[0]+=40u;goto L8c617d48; /* add #0x28,r0 */
L8c617d48: *stop=0x8c617d48u;r[2]=r[7];goto L8c617d4a; /* mov r7,r2 */
L8c617d4a: *stop=0x8c617d4au;r[2]+=36u;goto L8c617d4c; /* add #0x24,r2 */
L8c617d4c: *stop=0x8c617d4cu;r[9]=r[7];goto L8c617d4e; /* mov r7,r9 */
L8c617d4e: *stop=0x8c617d4eu;r[9]+=32u;goto L8c617d50; /* add #0x20,r9 */
L8c617d50: *stop=0x8c617d50u;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[2],4);goto L8c617d52; /* mov.l r2,@(0x4,r15) */
L8c617d52: *stop=0x8c617d52u;r[7]=r[14];goto L8c617d54; /* mov r14,r7 */
L8c617d54: *stop=0x8c617d54u;if(!valid_ram(r[15]+16u,4u))return -3;r[1]=rd(m,r[15]+0x10u,4);goto L8c617d56; /* mov.l @(0x10,r15),r1 */
L8c617d56: *stop=0x8c617d56u;r[7]+=36u;goto L8c617d58; /* add #0x24,r7 */
L8c617d58: *stop=0x8c617d58u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[1]);goto L8c617d5a; /* fmov.s @r1,fr2 */
L8c617d5a: *stop=0x8c617d5au;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[0],4);goto L8c617d5c; /* mov.l r0,@(0x8,r15) */
L8c617d5c: *stop=0x8c617d5cu;if(!valid_ram(r[15]+20u,4u))return -3;r[3]=rd(m,r[15]+0x14u,4);goto L8c617d5e; /* mov.l @(0x14,r15),r3 */
L8c617d5e: *stop=0x8c617d5eu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c617d60; /* fmov.s @r0,fr3 */
L8c617d60: *stop=0x8c617d60u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[3]);goto L8c617d62; /* fmov.s @r3,fr0 */
L8c617d62: *stop=0x8c617d62u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[2]);goto L8c617d64; /* fmov.s @r2,fr1 */
L8c617d64: *stop=0x8c617d64u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617d66; /* fmul fr0,fr3 */
L8c617d66: *stop=0x8c617d66u;fcopy(f,0u,2u);goto L8c617d68; /* fmov fr2,fr0 */
L8c617d68: *stop=0x8c617d68u;if(!valid_ram(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[9]);goto L8c617d6a; /* fmov.s @r9,fr2 */
L8c617d6a: *stop=0x8c617d6au;if(!valid_ram(r[15],4u))return -3;r[1]=rd(m,r[15],4);r[15]+=4;goto L8c617d6c; /* mov.l @r15+,r1 */
L8c617d6c: *stop=0x8c617d6cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617d6e; /* fmac fr0,fr1,fr3 */
L8c617d6e: *stop=0x8c617d6eu;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[6]);goto L8c617d70; /* fmov.s @r6,fr0 */
L8c617d70: *stop=0x8c617d70u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617d72; /* fmac fr0,fr2,fr3 */
L8c617d72: *stop=0x8c617d72u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[1]);goto L8c617d74; /* fmov.s fr3,@r1 */
L8c617d74: *stop=0x8c617d74u;if(!valid_ram(r[15],4u))return -3;r[1]=rd(m,r[15],4);goto L8c617d76; /* mov.l @r15,r1 */
L8c617d76: *stop=0x8c617d76u;if(!valid_ram(r[15]+8u,4u))return -3;r[0]=rd(m,r[15]+0x8u,4);goto L8c617d78; /* mov.l @(0x8,r15),r0 */
L8c617d78: *stop=0x8c617d78u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto L8c617d7a; /* fmov.s @r1,fr3 */
L8c617d7a: *stop=0x8c617d7au;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]);goto L8c617d7c; /* fmov.s @r0,fr0 */
L8c617d7c: *stop=0x8c617d7cu;if(!valid_ram(r[15]+4u,4u))return -3;r[3]=rd(m,r[15]+0x4u,4);goto L8c617d7e; /* mov.l @(0x4,r15),r3 */
L8c617d7e: *stop=0x8c617d7eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617d80; /* fmul fr0,fr3 */
L8c617d80: *stop=0x8c617d80u;if(!valid_ram(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[10]);goto L8c617d82; /* fmov.s @r10,fr0 */
L8c617d82: *stop=0x8c617d82u;if(!valid_ram(r[15]+20u,4u))return -3;r[2]=rd(m,r[15]+0x14u,4);goto L8c617d84; /* mov.l @(0x14,r15),r2 */
L8c617d84: *stop=0x8c617d84u;if(!valid_ram(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[9]);goto L8c617d86; /* fmov.s @r9,fr2 */
L8c617d86: *stop=0x8c617d86u;r[0]=80u;goto L8c617d88; /* mov #0x50,r0 */
L8c617d88: *stop=0x8c617d88u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[3]);goto L8c617d8a; /* fmov.s @r3,fr1 */
L8c617d8a: *stop=0x8c617d8au;r[3]=r[14];goto L8c617d8c; /* mov r14,r3 */
L8c617d8c: *stop=0x8c617d8cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617d8e; /* fmac fr0,fr2,fr3 */
L8c617d8e: *stop=0x8c617d8eu;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[2]);goto L8c617d90; /* fmov.s @r2,fr0 */
L8c617d90: *stop=0x8c617d90u;r[3]+=40u;goto L8c617d92; /* add #0x28,r3 */
L8c617d92: *stop=0x8c617d92u;r[6]=rd(m,0x8c617e1cu,4);goto L8c617d94; /* mov.l 0x8c617e1c,r6 */
L8c617d94: *stop=0x8c617d94u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617d96; /* fmac fr0,fr1,fr3 */
L8c617d96: *stop=0x8c617d96u;if(!valid_ram(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[7]);goto L8c617d98; /* fmov.s fr3,@r7 */
L8c617d98: *stop=0x8c617d98u;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[3],4);goto L8c617d9a; /* mov.l r3,@(0x8,r15) */
L8c617d9a: *stop=0x8c617d9au;if(!valid_ram(r[15],4u))return -3;r[1]=rd(m,r[15],4);goto L8c617d9c; /* mov.l @r15,r1 */
L8c617d9c: *stop=0x8c617d9cu;if(!valid_ram(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[11]);goto L8c617d9e; /* fmov.s @r11,fr0 */
L8c617d9e: *stop=0x8c617d9eu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto L8c617da0; /* fmov.s @r1,fr3 */
L8c617da0: *stop=0x8c617da0u;if(!valid_ram(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[9]);goto L8c617da2; /* fmov.s @r9,fr2 */
L8c617da2: *stop=0x8c617da2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617da4; /* fmul fr0,fr3 */
L8c617da4: *stop=0x8c617da4u;if(!valid_ram(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[13]);goto L8c617da6; /* fmov.s @r13,fr0 */
L8c617da6: *stop=0x8c617da6u;if(!valid_ram(r[15]+4u,4u))return -3;r[2]=rd(m,r[15]+0x4u,4);goto L8c617da8; /* mov.l @(0x4,r15),r2 */
L8c617da8: *stop=0x8c617da8u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[2]);goto L8c617daa; /* fmov.s @r2,fr1 */
L8c617daa: *stop=0x8c617daau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617dac; /* fmac fr0,fr2,fr3 */
L8c617dac: *stop=0x8c617dacu;if(!valid_ram(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[12]);goto L8c617dae; /* fmov.s @r12,fr0 */
L8c617dae: *stop=0x8c617daeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617db0; /* fmac fr0,fr1,fr3 */
L8c617db0: *stop=0x8c617db0u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[3]);goto L8c617db2; /* fmov.s fr3,@r3 */
L8c617db2: *stop=0x8c617db2u;if(!valid_ram(r[15]+24u,4u))return -3;r[2]=rd(m,r[15]+0x18u,4);goto L8c617db4; /* mov.l @(0x18,r15),r2 */
L8c617db4: *stop=0x8c617db4u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[5]);goto L8c617db6; /* fmov.s @(r0,r5),fr0 */
L8c617db6: *stop=0x8c617db6u;r[0]=76u;goto L8c617db8; /* mov #0x4c,r0 */
L8c617db8: *stop=0x8c617db8u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[2]);goto L8c617dba; /* fmov.s @r2,fr3 */
L8c617dba: *stop=0x8c617dbau;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[14]);goto L8c617dbc; /* fmov.s @r14,fr2 */
L8c617dbc: *stop=0x8c617dbcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617dbe; /* fmul fr0,fr3 */
L8c617dbe: *stop=0x8c617dbeu;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[5]);goto L8c617dc0; /* fmov.s @(r0,r5),fr0 */
L8c617dc0: *stop=0x8c617dc0u;if(!valid_ram(r[15]+32u,4u))return -3;r[3]=rd(m,r[15]+0x20u,4);goto L8c617dc2; /* mov.l @(0x20,r15),r3 */
L8c617dc2: *stop=0x8c617dc2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617dc4; /* fmac fr0,fr2,fr3 */
L8c617dc4: *stop=0x8c617dc4u;r[0]=84u;goto L8c617dc6; /* mov #0x54,r0 */
L8c617dc6: *stop=0x8c617dc6u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[3]);goto L8c617dc8; /* fmov.s @r3,fr2 */
L8c617dc8: *stop=0x8c617dc8u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[5]);goto L8c617dca; /* fmov.s @(r0,r5),fr0 */
L8c617dca: *stop=0x8c617dcau;r[0]=80u;goto L8c617dcc; /* mov #0x50,r0 */
L8c617dcc: *stop=0x8c617dccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617dce; /* fmac fr0,fr2,fr3 */
L8c617dce: *stop=0x8c617dceu;if(!valid_ram(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[6]);goto L8c617dd0; /* fmov.s fr3,@r6 */
L8c617dd0: *stop=0x8c617dd0u;if(!valid_ram(r[15]+36u,4u))return -3;r[2]=rd(m,r[15]+0x24u,4);goto L8c617dd2; /* mov.l @(0x24,r15),r2 */
L8c617dd2: *stop=0x8c617dd2u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[5]);goto L8c617dd4; /* fmov.s @(r0,r5),fr0 */
L8c617dd4: *stop=0x8c617dd4u;r[0]=76u;goto L8c617dd6; /* mov #0x4c,r0 */
L8c617dd6: *stop=0x8c617dd6u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[2]);goto L8c617dd8; /* fmov.s @r2,fr3 */
L8c617dd8: *stop=0x8c617dd8u;if(!valid_ram(r[15]+40u,4u))return -3;r[3]=rd(m,r[15]+0x28u,4);goto L8c617dda; /* mov.l @(0x28,r15),r3 */
L8c617dda: *stop=0x8c617ddau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617ddc; /* fmul fr0,fr3 */
L8c617ddc: *stop=0x8c617ddcu;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[5]);goto L8c617dde; /* fmov.s @(r0,r5),fr0 */
L8c617dde: *stop=0x8c617ddeu;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[3]);goto L8c617de0; /* fmov.s @r3,fr2 */
L8c617de0: *stop=0x8c617de0u;r[0]=84u;goto L8c617de2; /* mov #0x54,r0 */
L8c617de2: *stop=0x8c617de2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617de4; /* fmac fr0,fr2,fr3 */
L8c617de4: *stop=0x8c617de4u;if(!valid_ram(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[7]);goto L8c617de6; /* fmov.s @r7,fr2 */
L8c617de6: *stop=0x8c617de6u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[5]);goto L8c617de8; /* fmov.s @(r0,r5),fr0 */
L8c617de8: *stop=0x8c617de8u;r[0]=4u;goto L8c617dea; /* mov #0x4,r0 */
L8c617dea: *stop=0x8c617deau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617dec; /* fmac fr0,fr2,fr3 */
L8c617dec: *stop=0x8c617decu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto L8c617dee; /* fmov fr3,@(r0,r6) */
L8c617dee: *stop=0x8c617deeu;r[0]=80u;goto L8c617df0; /* mov #0x50,r0 */
L8c617df0: *stop=0x8c617df0u;if(!valid_ram(r[15]+28u,4u))return -3;r[1]=rd(m,r[15]+0x1cu,4);goto L8c617df2; /* mov.l @(0x1c,r15),r1 */
L8c617df2: *stop=0x8c617df2u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[5]);goto L8c617df4; /* fmov.s @(r0,r5),fr0 */
L8c617df4: *stop=0x8c617df4u;r[0]=76u;goto L8c617df6; /* mov #0x4c,r0 */
L8c617df6: *stop=0x8c617df6u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto L8c617df8; /* fmov.s @r1,fr3 */
L8c617df8: *stop=0x8c617df8u;if(!valid_ram(r[15]+44u,4u))return -3;r[2]=rd(m,r[15]+0x2cu,4);goto L8c617dfa; /* mov.l @(0x2c,r15),r2 */
L8c617dfa: *stop=0x8c617dfau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617dfc; /* fmul fr0,fr3 */
L8c617dfc: *stop=0x8c617dfcu;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[5]);goto L8c617dfe; /* fmov.s @(r0,r5),fr0 */
L8c617dfe: *stop=0x8c617dfeu;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[2]);goto L8c617e00; /* fmov.s @r2,fr2 */
L8c617e00: *stop=0x8c617e00u;if(!valid_ram(r[15]+8u,4u))return -3;r[3]=rd(m,r[15]+0x8u,4);goto L8c617e02; /* mov.l @(0x8,r15),r3 */
L8c617e02: *stop=0x8c617e02u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617e04; /* fmac fr0,fr2,fr3 */
L8c617e04: *stop=0x8c617e04u;r[0]=84u;goto L8c617e06; /* mov #0x54,r0 */
L8c617e06: *stop=0x8c617e06u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[3]);goto L8c617e08; /* fmov.s @r3,fr2 */
L8c617e08: *stop=0x8c617e08u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[5]);goto L8c617e0a; /* fmov.s @(r0,r5),fr0 */
L8c617e0a: *stop=0x8c617e0au;r[0]=8u;goto L8c617e0c; /* mov #0x8,r0 */
L8c617e0c: *stop=0x8c617e0cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617e0e; /* fmac fr0,fr2,fr3 */
L8c617e0e: *stop=0x8c617e0eu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto L8c617e10; /* fmov fr3,@(r0,r6) */
L8c617e10: *stop=0x8c617e12u;goto L8c617e20; /* bra 0x8c617e20 */
L8c617e20: *stop=0x8c617e20u;r[0]=sx(m,0x8c617ea2u,2);goto L8c617e22; /* mov.w 0x8c617ea2,r0 */
L8c617e22: *stop=0x8c617e22u;r[3]=rd(m,0x8c617ea8u,4);goto L8c617e24; /* mov.l 0x8c617ea8,r3 */
L8c617e24: *stop=0x8c617e24u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto L8c617e26; /* fmov.s @(r0,r5),fr3 */
L8c617e26: *stop=0x8c617e26u;r[0]+=4294967292u;goto L8c617e28; /* add #-0x4,r0 */
L8c617e28: *stop=0x8c617e28u;r[1]=rd(m,0x8c617eb0u,4);goto L8c617e2a; /* mov.l 0x8c617eb0,r1 */
L8c617e2a: *stop=0x8c617e2au;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[3]);goto L8c617e2c; /* fmov.s fr3,@r3 */
L8c617e2c: *stop=0x8c617e2cu;r[2]=rd(m,0x8c617eacu,4);goto L8c617e2e; /* mov.l 0x8c617eac,r2 */
L8c617e2e: *stop=0x8c617e2eu;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto L8c617e30; /* fmov.s @(r0,r5),fr3 */
L8c617e30: target=r[1];*pr=0x8c617e34u;*stop=0x8c617e32u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[2]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c617e34; /* jsr @r1 */
L8c617e34: *stop=0x8c617e34u;r[15]+=112u;goto L8c617e36; /* add #0x70,r15 */
L8c617e36: *stop=0x8c617e36u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c617e38; /* lds.l @r15+,PR */
L8c617e38: *stop=0x8c617e38u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c617e3a; /* mov.l @r15+,r9 */
L8c617e3a: *stop=0x8c617e3au;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c617e3c; /* mov.l @r15+,r10 */
L8c617e3c: *stop=0x8c617e3cu;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c617e3e; /* mov.l @r15+,r11 */
L8c617e3e: *stop=0x8c617e3eu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c617e40; /* mov.l @r15+,r12 */
L8c617e40: *stop=0x8c617e40u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c617e42; /* mov.l @r15+,r13 */
L8c617e42: *stop=0x8c617e42u;if(*pr!=entry_pr)return -3;*stop=0x8c617e44u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_motion_mesh_wrapper_8c621574(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c621574;
L8c621574: *stop=0x8c621574u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c621576; /* sts.l PR,@-r15 */
L8c621576: *stop=0x8c621576u;r[15]+=4294967288u;goto L8c621578; /* add #-0x8,r15 */
L8c621578: *stop=0x8c621578u;r[3]=rd(m,0x8c62161cu,4);goto L8c62157a; /* mov.l 0x8c62161c,r3 */
L8c62157a: target=r[3];*pr=0x8c62157eu;*stop=0x8c62157cu;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[4],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62157e; /* jsr @r3 */
L8c62157e: *stop=0x8c62157eu;r[4]=r[15];goto L8c621580; /* mov r15,r4 */
L8c621580: *stop=0x8c621580u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[8],4);goto L8c621582; /* mov.l r8,@r4 */
L8c621582: *stop=0x8c621582u;r[3]=rd(m,0x8c621620u,4);goto L8c621584; /* mov.l 0x8c621620,r3 */
L8c621584: target=r[3];*pr=0x8c621588u;*stop=0x8c621586u;if(!valid_ram(r[15]+4u,4u))return -3;r[4]=rd(m,r[15]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621588; /* jsr @r3 */
L8c621588: *stop=0x8c621588u;r[4]=r[15];goto L8c62158a; /* mov r15,r4 */
L8c62158a: *stop=0x8c62158au;if(!valid_ram(r[4],4u))return -3;r[8]=rd(m,r[4],4);goto L8c62158c; /* mov.l @r4,r8 */
L8c62158c: *stop=0x8c62158cu;r[3]=rd(m,0x8c621624u,4);goto L8c62158e; /* mov.l 0x8c621624,r3 */
L8c62158e: target=r[3];*pr=0x8c621592u;*stop=0x8c621590u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621592; /* jsr @r3 */
L8c621592: *stop=0x8c621592u;r[15]+=8u;goto L8c621594; /* add #0x8,r15 */
L8c621594: *stop=0x8c621594u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c621596; /* lds.l @r15+,PR */
L8c621596: *stop=0x8c621596u;if(*pr!=entry_pr)return -3;*stop=0x8c621598u;return 0; /* rts */
}
