/* Geometry preparation and R8-preserving model wrapper; names inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_add_sub.h"
#include "sh4_fmac.h"
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
SA_STAGE_EXPORT int sa_particle_geometry_prepare_8c620f2c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c620f2c;
L8c620f2c: r[15]-=4;wr(m,r[15],r[14],4);goto L8c620f2e; /* mov.l r14,@-r15 */
L8c620f2e: r[15]-=4;wr(m,r[15],r[13],4);goto L8c620f30; /* mov.l r13,@-r15 */
L8c620f30: r[15]-=4;wr(m,r[15],r[12],4);goto L8c620f32; /* mov.l r12,@-r15 */
L8c620f32: r[15]-=4;wr(m,r[15],r[11],4);goto L8c620f34; /* mov.l r11,@-r15 */
L8c620f34: r[15]-=4;wr(m,r[15],r[10],4);goto L8c620f36; /* mov.l r10,@-r15 */
L8c620f36: r[15]-=4;wr(m,r[15],r[9],4);goto L8c620f38; /* mov.l r9,@-r15 */
L8c620f38: r[15]-=4;wr(m,r[15],*pr,4);goto L8c620f3a; /* sts.l PR,@-r15 */
L8c620f3a: r[15]+=4294967184u;goto L8c620f3c; /* add #-0x70,r15 */
L8c620f3c: r[5]=rd(m,0x8c62110cu,4);goto L8c620f3e; /* mov.l 0x8c62110c,r5 */
L8c620f3e: r[14]=r[15];goto L8c620f40; /* mov r15,r14 */
L8c620f40: r[6]=rd(m,0x8c621110u,4);goto L8c620f42; /* mov.l 0x8c621110,r6 */
L8c620f42: r[14]+=48u;goto L8c620f44; /* add #0x30,r14 */
L8c620f44: r[5]=rd(m,r[5],4);goto L8c620f46; /* mov.l @r5,r5 */
L8c620f46: r[1]=r[6];goto L8c620f48; /* mov r6,r1 */
L8c620f48: r[1]+=32u;goto L8c620f4a; /* add #0x20,r1 */
L8c620f4a: r[7]=r[5];goto L8c620f4c; /* mov r5,r7 */
L8c620f4c: r[2]=r[7];goto L8c620f4e; /* mov r7,r2 */
L8c620f4e: r[2]+=8u;goto L8c620f50; /* add #0x8,r2 */
L8c620f50: r[9]=r[5];goto L8c620f52; /* mov r5,r9 */
L8c620f52: r[3]=r[6];goto L8c620f54; /* mov r6,r3 */
L8c620f54: r[3]+=16u;goto L8c620f56; /* add #0x10,r3 */
L8c620f56: r[9]+=4u;goto L8c620f58; /* add #0x4,r9 */
L8c620f58: wr(m,r[15]+0xcu,r[3],4);goto L8c620f5a; /* mov.l r3,@(0xc,r15) */
L8c620f5a: wr(m,r[15],r[2],4);goto L8c620f5c; /* mov.l r2,@r15 */
L8c620f5c: wr(m,r[15]+0x10u,r[1],4);goto L8c620f5e; /* mov.l r1,@(0x10,r15) */
L8c620f5e: fload(m,f,0u,r[1]);goto L8c620f60; /* fmov.s @r1,fr0 */
L8c620f60: fload(m,f,3u,r[2]);goto L8c620f62; /* fmov.s @r2,fr3 */
L8c620f62: r[2]=r[6];goto L8c620f64; /* mov r6,r2 */
L8c620f64: fload(m,f,2u,r[9]);goto L8c620f66; /* fmov.s @r9,fr2 */
L8c620f66: r[2]+=20u;goto L8c620f68; /* add #0x14,r2 */
L8c620f68: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620f6a; /* fmul fr0,fr3 */
L8c620f6a: fload(m,f,0u,r[3]);goto L8c620f6c; /* fmov.s @r3,fr0 */
L8c620f6c: fload(m,f,1u,r[7]);goto L8c620f6e; /* fmov.s @r7,fr1 */
L8c620f6e: r[3]=r[14];goto L8c620f70; /* mov r14,r3 */
L8c620f70: r[3]+=4u;goto L8c620f72; /* add #0x4,r3 */
L8c620f72: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620f74; /* fmac fr0,fr2,fr3 */
L8c620f74: fload(m,f,0u,r[6]);goto L8c620f76; /* fmov.s @r6,fr0 */
L8c620f76: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620f78; /* fmac fr0,fr1,fr3 */
L8c620f78: fstore(m,f,3u,r[14]);goto L8c620f7a; /* fmov.s fr3,@r14 */
L8c620f7a: wr(m,r[15]+0x28u,r[3],4);goto L8c620f7c; /* mov.l r3,@(0x28,r15) */
L8c620f7c: r[0]=r[6];goto L8c620f7e; /* mov r6,r0 */
L8c620f7e: r[0]+=36u;goto L8c620f80; /* add #0x24,r0 */
L8c620f80: r[10]=r[6];goto L8c620f82; /* mov r6,r10 */
L8c620f82: r[10]+=4u;goto L8c620f84; /* add #0x4,r10 */
L8c620f84: wr(m,r[15]+0x8u,r[2],4);goto L8c620f86; /* mov.l r2,@(0x8,r15) */
L8c620f86: r[12]=r[6];goto L8c620f88; /* mov r6,r12 */
L8c620f88: r[1]=rd(m,r[15],4);goto L8c620f8a; /* mov.l @r15,r1 */
L8c620f8a: r[12]+=40u;goto L8c620f8c; /* add #0x28,r12 */
L8c620f8c: r[11]=r[6];goto L8c620f8e; /* mov r6,r11 */
L8c620f8e: r[11]+=24u;goto L8c620f90; /* add #0x18,r11 */
L8c620f90: fload(m,f,3u,r[1]);goto L8c620f92; /* fmov.s @r1,fr3 */
L8c620f92: r[13]=r[6];goto L8c620f94; /* mov r6,r13 */
L8c620f94: wr(m,r[15]+0x14u,r[0],4);goto L8c620f96; /* mov.l r0,@(0x14,r15) */
L8c620f96: fload(m,f,0u,r[0]);goto L8c620f98; /* fmov.s @r0,fr0 */
L8c620f98: fload(m,f,2u,r[9]);goto L8c620f9a; /* fmov.s @r9,fr2 */
L8c620f9a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620f9c; /* fmul fr0,fr3 */
L8c620f9c: fload(m,f,0u,r[2]);goto L8c620f9e; /* fmov.s @r2,fr0 */
L8c620f9e: fload(m,f,1u,r[10]);goto L8c620fa0; /* fmov.s @r10,fr1 */
L8c620fa0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620fa2; /* fmac fr0,fr2,fr3 */
L8c620fa2: fload(m,f,0u,r[7]);goto L8c620fa4; /* fmov.s @r7,fr0 */
L8c620fa4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620fa6; /* fmac fr0,fr1,fr3 */
L8c620fa6: fstore(m,f,3u,r[3]);goto L8c620fa8; /* fmov.s fr3,@r3 */
L8c620fa8: r[3]=r[14];goto L8c620faa; /* mov r14,r3 */
L8c620faa: r[3]+=8u;goto L8c620fac; /* add #0x8,r3 */
L8c620fac: wr(m,r[15]+0x2cu,r[3],4);goto L8c620fae; /* mov.l r3,@(0x2c,r15) */
L8c620fae: r[2]=rd(m,r[15],4);goto L8c620fb0; /* mov.l @r15,r2 */
L8c620fb0: fload(m,f,0u,r[12]);goto L8c620fb2; /* fmov.s @r12,fr0 */
L8c620fb2: fload(m,f,3u,r[2]);goto L8c620fb4; /* fmov.s @r2,fr3 */
L8c620fb4: fload(m,f,2u,r[9]);goto L8c620fb6; /* fmov.s @r9,fr2 */
L8c620fb6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620fb8; /* fmul fr0,fr3 */
L8c620fb8: fload(m,f,0u,r[11]);goto L8c620fba; /* fmov.s @r11,fr0 */
L8c620fba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620fbc; /* fmac fr0,fr2,fr3 */
L8c620fbc: r[13]+=8u;goto L8c620fbe; /* add #0x8,r13 */
L8c620fbe: fload(m,f,0u,r[7]);goto L8c620fc0; /* fmov.s @r7,fr0 */
L8c620fc0: fload(m,f,1u,r[13]);goto L8c620fc2; /* fmov.s @r13,fr1 */
L8c620fc2: r[1]=r[7];goto L8c620fc4; /* mov r7,r1 */
L8c620fc4: r[1]+=24u;goto L8c620fc6; /* add #0x18,r1 */
L8c620fc6: r[9]=r[7];goto L8c620fc8; /* mov r7,r9 */
L8c620fc8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620fca; /* fmac fr0,fr1,fr3 */
L8c620fca: r[9]+=20u;goto L8c620fcc; /* add #0x14,r9 */
L8c620fcc: fstore(m,f,3u,r[3]);goto L8c620fce; /* fmov.s fr3,@r3 */
L8c620fce: r[3]=r[14];goto L8c620fd0; /* mov r14,r3 */
L8c620fd0: r[3]+=16u;goto L8c620fd2; /* add #0x10,r3 */
L8c620fd2: wr(m,r[15]+0x18u,r[3],4);goto L8c620fd4; /* mov.l r3,@(0x18,r15) */
L8c620fd4: r[2]=rd(m,r[15]+0xcu,4);goto L8c620fd6; /* mov.l @(0xc,r15),r2 */
L8c620fd6: wr(m,r[15],r[1],4);goto L8c620fd8; /* mov.l r1,@r15 */
L8c620fd8: r[0]=rd(m,r[15]+0x10u,4);goto L8c620fda; /* mov.l @(0x10,r15),r0 */
L8c620fda: fload(m,f,3u,r[1]);goto L8c620fdc; /* fmov.s @r1,fr3 */
L8c620fdc: fload(m,f,0u,r[0]);goto L8c620fde; /* fmov.s @r0,fr0 */
L8c620fde: r[0]=r[7];goto L8c620fe0; /* mov r7,r0 */
L8c620fe0: fload(m,f,2u,r[9]);goto L8c620fe2; /* fmov.s @r9,fr2 */
L8c620fe2: r[0]+=16u;goto L8c620fe4; /* add #0x10,r0 */
L8c620fe4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620fe6; /* fmul fr0,fr3 */
L8c620fe6: fload(m,f,0u,r[2]);goto L8c620fe8; /* fmov.s @r2,fr0 */
L8c620fe8: wr(m,r[15]+0x4u,r[0],4);goto L8c620fea; /* mov.l r0,@(0x4,r15) */
L8c620fea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620fec; /* fmac fr0,fr2,fr3 */
L8c620fec: fload(m,f,0u,r[6]);goto L8c620fee; /* fmov.s @r6,fr0 */
L8c620fee: fload(m,f,2u,r[0]);goto L8c620ff0; /* fmov.s @r0,fr2 */
L8c620ff0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620ff2; /* fmac fr0,fr2,fr3 */
L8c620ff2: fstore(m,f,3u,r[3]);goto L8c620ff4; /* fmov.s fr3,@r3 */
L8c620ff4: r[3]=r[14];goto L8c620ff6; /* mov r14,r3 */
L8c620ff6: r[3]+=20u;goto L8c620ff8; /* add #0x14,r3 */
L8c620ff8: wr(m,r[15]+0x24u,r[3],4);goto L8c620ffa; /* mov.l r3,@(0x24,r15) */
L8c620ffa: r[2]=rd(m,r[15],4);goto L8c620ffc; /* mov.l @r15,r2 */
L8c620ffc: fload(m,f,1u,r[2]);goto L8c620ffe; /* fmov.s @r2,fr1 */
L8c620ffe: r[2]=rd(m,r[15]+0x8u,4);goto L8c621000; /* mov.l @(0x8,r15),r2 */
L8c621000: fload(m,f,3u,r[9]);goto L8c621002; /* fmov.s @r9,fr3 */
L8c621002: fload(m,f,0u,r[2]);goto L8c621004; /* fmov.s @r2,fr0 */
L8c621004: r[0]=rd(m,r[15]+0x4u,4);goto L8c621006; /* mov.l @(0x4,r15),r0 */
L8c621006: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621008; /* fmul fr0,fr3 */
L8c621008: fload(m,f,0u,r[10]);goto L8c62100a; /* fmov.s @r10,fr0 */
L8c62100a: fload(m,f,2u,r[0]);goto L8c62100c; /* fmov.s @r0,fr2 */
L8c62100c: r[1]=rd(m,r[15]+0x14u,4);goto L8c62100e; /* mov.l @(0x14,r15),r1 */
L8c62100e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621010; /* fmac fr0,fr2,fr3 */
L8c621010: fload(m,f,0u,r[1]);goto L8c621012; /* fmov.s @r1,fr0 */
L8c621012: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621014; /* fmac fr0,fr1,fr3 */
L8c621014: fstore(m,f,3u,r[3]);goto L8c621016; /* fmov.s fr3,@r3 */
L8c621016: r[3]=r[14];goto L8c621018; /* mov r14,r3 */
L8c621018: r[3]+=24u;goto L8c62101a; /* add #0x18,r3 */
L8c62101a: wr(m,r[15]+0x1cu,r[3],4);goto L8c62101c; /* mov.l r3,@(0x1c,r15) */
L8c62101c: fload(m,f,0u,r[11]);goto L8c62101e; /* fmov.s @r11,fr0 */
L8c62101e: fload(m,f,3u,r[9]);goto L8c621020; /* fmov.s @r9,fr3 */
L8c621020: r[1]=rd(m,r[15]+0x4u,4);goto L8c621022; /* mov.l @(0x4,r15),r1 */
L8c621022: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621024; /* fmul fr0,fr3 */
L8c621024: fload(m,f,0u,r[13]);goto L8c621026; /* fmov.s @r13,fr0 */
L8c621026: fload(m,f,2u,r[1]);goto L8c621028; /* fmov.s @r1,fr2 */
L8c621028: r[2]=rd(m,r[15],4);goto L8c62102a; /* mov.l @r15,r2 */
L8c62102a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c62102c; /* fmac fr0,fr2,fr3 */
L8c62102c: fload(m,f,0u,r[12]);goto L8c62102e; /* fmov.s @r12,fr0 */
L8c62102e: fload(m,f,1u,r[2]);goto L8c621030; /* fmov.s @r2,fr1 */
L8c621030: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621032; /* fmac fr0,fr1,fr3 */
L8c621032: fstore(m,f,3u,r[3]);goto L8c621034; /* fmov.s fr3,@r3 */
L8c621034: r[3]=r[14];goto L8c621036; /* mov r14,r3 */
L8c621036: r[3]+=32u;goto L8c621038; /* add #0x20,r3 */
L8c621038: wr(m,r[15]+0x20u,r[3],4);goto L8c62103a; /* mov.l r3,@(0x20,r15) */
L8c62103a: r[15]-=4;wr(m,r[15],r[3],4);goto L8c62103c; /* mov.l r3,@-r15 */
L8c62103c: r[0]=r[7];goto L8c62103e; /* mov r7,r0 */
L8c62103e: r[0]+=40u;goto L8c621040; /* add #0x28,r0 */
L8c621040: r[2]=r[7];goto L8c621042; /* mov r7,r2 */
L8c621042: r[2]+=36u;goto L8c621044; /* add #0x24,r2 */
L8c621044: r[9]=r[7];goto L8c621046; /* mov r7,r9 */
L8c621046: r[9]+=32u;goto L8c621048; /* add #0x20,r9 */
L8c621048: wr(m,r[15]+0x4u,r[2],4);goto L8c62104a; /* mov.l r2,@(0x4,r15) */
L8c62104a: r[7]=r[14];goto L8c62104c; /* mov r14,r7 */
L8c62104c: r[1]=rd(m,r[15]+0x10u,4);goto L8c62104e; /* mov.l @(0x10,r15),r1 */
L8c62104e: r[7]+=36u;goto L8c621050; /* add #0x24,r7 */
L8c621050: fload(m,f,2u,r[1]);goto L8c621052; /* fmov.s @r1,fr2 */
L8c621052: wr(m,r[15]+0x8u,r[0],4);goto L8c621054; /* mov.l r0,@(0x8,r15) */
L8c621054: r[3]=rd(m,r[15]+0x14u,4);goto L8c621056; /* mov.l @(0x14,r15),r3 */
L8c621056: fload(m,f,3u,r[0]);goto L8c621058; /* fmov.s @r0,fr3 */
L8c621058: fload(m,f,0u,r[3]);goto L8c62105a; /* fmov.s @r3,fr0 */
L8c62105a: fload(m,f,1u,r[2]);goto L8c62105c; /* fmov.s @r2,fr1 */
L8c62105c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c62105e; /* fmul fr0,fr3 */
L8c62105e: fcopy(f,0u,2u);goto L8c621060; /* fmov fr2,fr0 */
L8c621060: fload(m,f,2u,r[9]);goto L8c621062; /* fmov.s @r9,fr2 */
L8c621062: r[1]=rd(m,r[15],4);r[15]+=4;goto L8c621064; /* mov.l @r15+,r1 */
L8c621064: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621066; /* fmac fr0,fr1,fr3 */
L8c621066: fload(m,f,0u,r[6]);goto L8c621068; /* fmov.s @r6,fr0 */
L8c621068: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c62106a; /* fmac fr0,fr2,fr3 */
L8c62106a: fstore(m,f,3u,r[1]);goto L8c62106c; /* fmov.s fr3,@r1 */
L8c62106c: r[1]=rd(m,r[15],4);goto L8c62106e; /* mov.l @r15,r1 */
L8c62106e: r[0]=rd(m,r[15]+0x8u,4);goto L8c621070; /* mov.l @(0x8,r15),r0 */
L8c621070: fload(m,f,3u,r[1]);goto L8c621072; /* fmov.s @r1,fr3 */
L8c621072: fload(m,f,0u,r[0]);goto L8c621074; /* fmov.s @r0,fr0 */
L8c621074: r[3]=rd(m,r[15]+0x4u,4);goto L8c621076; /* mov.l @(0x4,r15),r3 */
L8c621076: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621078; /* fmul fr0,fr3 */
L8c621078: fload(m,f,0u,r[10]);goto L8c62107a; /* fmov.s @r10,fr0 */
L8c62107a: r[2]=rd(m,r[15]+0x14u,4);goto L8c62107c; /* mov.l @(0x14,r15),r2 */
L8c62107c: fload(m,f,2u,r[9]);goto L8c62107e; /* fmov.s @r9,fr2 */
L8c62107e: r[0]=80u;goto L8c621080; /* mov #0x50,r0 */
L8c621080: fload(m,f,1u,r[3]);goto L8c621082; /* fmov.s @r3,fr1 */
L8c621082: r[3]=r[14];goto L8c621084; /* mov r14,r3 */
L8c621084: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621086; /* fmac fr0,fr2,fr3 */
L8c621086: fload(m,f,0u,r[2]);goto L8c621088; /* fmov.s @r2,fr0 */
L8c621088: r[3]+=40u;goto L8c62108a; /* add #0x28,r3 */
L8c62108a: r[6]=rd(m,0x8c621114u,4);goto L8c62108c; /* mov.l 0x8c621114,r6 */
L8c62108c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c62108e; /* fmac fr0,fr1,fr3 */
L8c62108e: fstore(m,f,3u,r[7]);goto L8c621090; /* fmov.s fr3,@r7 */
L8c621090: wr(m,r[15]+0x8u,r[3],4);goto L8c621092; /* mov.l r3,@(0x8,r15) */
L8c621092: r[1]=rd(m,r[15],4);goto L8c621094; /* mov.l @r15,r1 */
L8c621094: fload(m,f,0u,r[11]);goto L8c621096; /* fmov.s @r11,fr0 */
L8c621096: fload(m,f,3u,r[1]);goto L8c621098; /* fmov.s @r1,fr3 */
L8c621098: fload(m,f,2u,r[9]);goto L8c62109a; /* fmov.s @r9,fr2 */
L8c62109a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c62109c; /* fmul fr0,fr3 */
L8c62109c: fload(m,f,0u,r[13]);goto L8c62109e; /* fmov.s @r13,fr0 */
L8c62109e: r[2]=rd(m,r[15]+0x4u,4);goto L8c6210a0; /* mov.l @(0x4,r15),r2 */
L8c6210a0: fload(m,f,1u,r[2]);goto L8c6210a2; /* fmov.s @r2,fr1 */
L8c6210a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210a4; /* fmac fr0,fr2,fr3 */
L8c6210a4: fload(m,f,0u,r[12]);goto L8c6210a6; /* fmov.s @r12,fr0 */
L8c6210a6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210a8; /* fmac fr0,fr1,fr3 */
L8c6210a8: fstore(m,f,3u,r[3]);goto L8c6210aa; /* fmov.s fr3,@r3 */
L8c6210aa: r[2]=rd(m,r[15]+0x18u,4);goto L8c6210ac; /* mov.l @(0x18,r15),r2 */
L8c6210ac: fload(m,f,0u,r[0]+r[5]);goto L8c6210ae; /* fmov.s @(r0,r5),fr0 */
L8c6210ae: r[0]=76u;goto L8c6210b0; /* mov #0x4c,r0 */
L8c6210b0: fload(m,f,3u,r[2]);goto L8c6210b2; /* fmov.s @r2,fr3 */
L8c6210b2: fload(m,f,2u,r[14]);goto L8c6210b4; /* fmov.s @r14,fr2 */
L8c6210b4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210b6; /* fmul fr0,fr3 */
L8c6210b6: fload(m,f,0u,r[0]+r[5]);goto L8c6210b8; /* fmov.s @(r0,r5),fr0 */
L8c6210b8: r[3]=rd(m,r[15]+0x20u,4);goto L8c6210ba; /* mov.l @(0x20,r15),r3 */
L8c6210ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210bc; /* fmac fr0,fr2,fr3 */
L8c6210bc: r[0]=84u;goto L8c6210be; /* mov #0x54,r0 */
L8c6210be: fload(m,f,2u,r[3]);goto L8c6210c0; /* fmov.s @r3,fr2 */
L8c6210c0: fload(m,f,0u,r[0]+r[5]);goto L8c6210c2; /* fmov.s @(r0,r5),fr0 */
L8c6210c2: r[0]=80u;goto L8c6210c4; /* mov #0x50,r0 */
L8c6210c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210c6; /* fmac fr0,fr2,fr3 */
L8c6210c6: fstore(m,f,3u,r[6]);goto L8c6210c8; /* fmov.s fr3,@r6 */
L8c6210c8: r[2]=rd(m,r[15]+0x24u,4);goto L8c6210ca; /* mov.l @(0x24,r15),r2 */
L8c6210ca: fload(m,f,0u,r[0]+r[5]);goto L8c6210cc; /* fmov.s @(r0,r5),fr0 */
L8c6210cc: r[0]=76u;goto L8c6210ce; /* mov #0x4c,r0 */
L8c6210ce: fload(m,f,3u,r[2]);goto L8c6210d0; /* fmov.s @r2,fr3 */
L8c6210d0: r[3]=rd(m,r[15]+0x28u,4);goto L8c6210d2; /* mov.l @(0x28,r15),r3 */
L8c6210d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210d4; /* fmul fr0,fr3 */
L8c6210d4: fload(m,f,0u,r[0]+r[5]);goto L8c6210d6; /* fmov.s @(r0,r5),fr0 */
L8c6210d6: fload(m,f,2u,r[3]);goto L8c6210d8; /* fmov.s @r3,fr2 */
L8c6210d8: r[0]=84u;goto L8c6210da; /* mov #0x54,r0 */
L8c6210da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210dc; /* fmac fr0,fr2,fr3 */
L8c6210dc: fload(m,f,2u,r[7]);goto L8c6210de; /* fmov.s @r7,fr2 */
L8c6210de: fload(m,f,0u,r[0]+r[5]);goto L8c6210e0; /* fmov.s @(r0,r5),fr0 */
L8c6210e0: r[0]=4u;goto L8c6210e2; /* mov #0x4,r0 */
L8c6210e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210e4; /* fmac fr0,fr2,fr3 */
L8c6210e4: fstore(m,f,3u,r[0]+r[6]);goto L8c6210e6; /* fmov fr3,@(r0,r6) */
L8c6210e6: r[0]=80u;goto L8c6210e8; /* mov #0x50,r0 */
L8c6210e8: r[1]=rd(m,r[15]+0x1cu,4);goto L8c6210ea; /* mov.l @(0x1c,r15),r1 */
L8c6210ea: fload(m,f,0u,r[0]+r[5]);goto L8c6210ec; /* fmov.s @(r0,r5),fr0 */
L8c6210ec: r[0]=76u;goto L8c6210ee; /* mov #0x4c,r0 */
L8c6210ee: fload(m,f,3u,r[1]);goto L8c6210f0; /* fmov.s @r1,fr3 */
L8c6210f0: r[2]=rd(m,r[15]+0x2cu,4);goto L8c6210f2; /* mov.l @(0x2c,r15),r2 */
L8c6210f2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210f4; /* fmul fr0,fr3 */
L8c6210f4: fload(m,f,0u,r[0]+r[5]);goto L8c6210f6; /* fmov.s @(r0,r5),fr0 */
L8c6210f6: fload(m,f,2u,r[2]);goto L8c6210f8; /* fmov.s @r2,fr2 */
L8c6210f8: r[3]=rd(m,r[15]+0x8u,4);goto L8c6210fa; /* mov.l @(0x8,r15),r3 */
L8c6210fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210fc; /* fmac fr0,fr2,fr3 */
L8c6210fc: r[0]=84u;goto L8c6210fe; /* mov #0x54,r0 */
L8c6210fe: fload(m,f,2u,r[3]);goto L8c621100; /* fmov.s @r3,fr2 */
L8c621100: fload(m,f,0u,r[0]+r[5]);goto L8c621102; /* fmov.s @(r0,r5),fr0 */
L8c621102: r[0]=8u;goto L8c621104; /* mov #0x8,r0 */
L8c621104: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621106; /* fmac fr0,fr2,fr3 */
L8c621106: fstore(m,f,3u,r[0]+r[6]);goto L8c621108; /* fmov fr3,@(r0,r6) */
L8c621108: goto L8c621118; /* bra 0x8c621118 */
L8c621118: r[0]=sx(m,0x8c621234u,2);goto L8c62111a; /* mov.w 0x8c621234,r0 */
L8c62111a: r[3]=rd(m,0x8c621238u,4);goto L8c62111c; /* mov.l 0x8c621238,r3 */
L8c62111c: fload(m,f,3u,r[0]+r[5]);goto L8c62111e; /* fmov.s @(r0,r5),fr3 */
L8c62111e: r[0]+=4294967292u;goto L8c621120; /* add #-0x4,r0 */
L8c621120: r[2]=rd(m,0x8c62123cu,4);goto L8c621122; /* mov.l 0x8c62123c,r2 */
L8c621122: fstore(m,f,3u,r[3]);goto L8c621124; /* fmov.s fr3,@r3 */
L8c621124: fload(m,f,3u,r[0]+r[5]);goto L8c621126; /* fmov.s @(r0,r5),fr3 */
L8c621126: target=0x8c6215fau;*pr=0x8c62112au;fstore(m,f,3u,r[2]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62112a; /* bsr 0x8c6215fa */
L8c62112a: r[15]+=112u;goto L8c62112c; /* add #0x70,r15 */
L8c62112c: *pr=rd(m,r[15],4);r[15]+=4;goto L8c62112e; /* lds.l @r15+,PR */
L8c62112e: r[9]=rd(m,r[15],4);r[15]+=4;goto L8c621130; /* mov.l @r15+,r9 */
L8c621130: r[10]=rd(m,r[15],4);r[15]+=4;goto L8c621132; /* mov.l @r15+,r10 */
L8c621132: r[11]=rd(m,r[15],4);r[15]+=4;goto L8c621134; /* mov.l @r15+,r11 */
L8c621134: r[12]=rd(m,r[15],4);r[15]+=4;goto L8c621136; /* mov.l @r15+,r12 */
L8c621136: r[13]=rd(m,r[15],4);r[15]+=4;goto L8c621138; /* mov.l @r15+,r13 */
L8c621138: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_particle_model_wrapper_8c6215fa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6215fa;
L8c6215fa: r[15]-=4;wr(m,r[15],*pr,4);goto L8c6215fc; /* sts.l PR,@-r15 */
L8c6215fc: r[15]+=4294967288u;goto L8c6215fe; /* add #-0x8,r15 */
L8c6215fe: wr(m,r[15]+0x4u,r[4],4);goto L8c621600; /* mov.l r4,@(0x4,r15) */
L8c621600: r[4]=r[15];goto L8c621602; /* mov r15,r4 */
L8c621602: wr(m,r[4],r[8],4);goto L8c621604; /* mov.l r8,@r4 */
L8c621604: target=0x8c62113cu;*pr=0x8c621608u;r[4]=rd(m,r[15]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621608; /* bsr 0x8c62113c */
L8c621608: r[4]=r[15];goto L8c62160a; /* mov r15,r4 */
L8c62160a: r[8]=rd(m,r[4],4);goto L8c62160c; /* mov.l @r4,r8 */
L8c62160c: r[15]+=8u;goto L8c62160e; /* add #0x8,r15 */
L8c62160e: *pr=rd(m,r[15],4);r[15]+=4;goto L8c621610; /* lds.l @r15+,PR */
L8c621610: return 0; /* rts */
}
