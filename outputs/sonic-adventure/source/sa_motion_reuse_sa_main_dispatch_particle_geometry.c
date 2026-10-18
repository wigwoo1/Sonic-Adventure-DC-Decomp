#include "sa_draw_motion_stop.h"
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
L8c620f2c: sa_motion_publish_stop(ctx,0x8c620f2cu); r[15]-=4;wr(m,r[15],r[14],4);goto L8c620f2e; /* mov.l r14,@-r15 */
L8c620f2e: sa_motion_publish_stop(ctx,0x8c620f2eu); r[15]-=4;wr(m,r[15],r[13],4);goto L8c620f30; /* mov.l r13,@-r15 */
L8c620f30: sa_motion_publish_stop(ctx,0x8c620f30u); r[15]-=4;wr(m,r[15],r[12],4);goto L8c620f32; /* mov.l r12,@-r15 */
L8c620f32: sa_motion_publish_stop(ctx,0x8c620f32u); r[15]-=4;wr(m,r[15],r[11],4);goto L8c620f34; /* mov.l r11,@-r15 */
L8c620f34: sa_motion_publish_stop(ctx,0x8c620f34u); r[15]-=4;wr(m,r[15],r[10],4);goto L8c620f36; /* mov.l r10,@-r15 */
L8c620f36: sa_motion_publish_stop(ctx,0x8c620f36u); r[15]-=4;wr(m,r[15],r[9],4);goto L8c620f38; /* mov.l r9,@-r15 */
L8c620f38: sa_motion_publish_stop(ctx,0x8c620f38u); r[15]-=4;wr(m,r[15],*pr,4);goto L8c620f3a; /* sts.l PR,@-r15 */
L8c620f3a: sa_motion_publish_stop(ctx,0x8c620f3au); r[15]+=4294967184u;goto L8c620f3c; /* add #-0x70,r15 */
L8c620f3c: sa_motion_publish_stop(ctx,0x8c620f3cu); r[5]=rd(m,0x8c62110cu,4);goto L8c620f3e; /* mov.l 0x8c62110c,r5 */
L8c620f3e: sa_motion_publish_stop(ctx,0x8c620f3eu); r[14]=r[15];goto L8c620f40; /* mov r15,r14 */
L8c620f40: sa_motion_publish_stop(ctx,0x8c620f40u); r[6]=rd(m,0x8c621110u,4);goto L8c620f42; /* mov.l 0x8c621110,r6 */
L8c620f42: sa_motion_publish_stop(ctx,0x8c620f42u); r[14]+=48u;goto L8c620f44; /* add #0x30,r14 */
L8c620f44: sa_motion_publish_stop(ctx,0x8c620f44u); r[5]=rd(m,r[5],4);goto L8c620f46; /* mov.l @r5,r5 */
L8c620f46: sa_motion_publish_stop(ctx,0x8c620f46u); r[1]=r[6];goto L8c620f48; /* mov r6,r1 */
L8c620f48: sa_motion_publish_stop(ctx,0x8c620f48u); r[1]+=32u;goto L8c620f4a; /* add #0x20,r1 */
L8c620f4a: sa_motion_publish_stop(ctx,0x8c620f4au); r[7]=r[5];goto L8c620f4c; /* mov r5,r7 */
L8c620f4c: sa_motion_publish_stop(ctx,0x8c620f4cu); r[2]=r[7];goto L8c620f4e; /* mov r7,r2 */
L8c620f4e: sa_motion_publish_stop(ctx,0x8c620f4eu); r[2]+=8u;goto L8c620f50; /* add #0x8,r2 */
L8c620f50: sa_motion_publish_stop(ctx,0x8c620f50u); r[9]=r[5];goto L8c620f52; /* mov r5,r9 */
L8c620f52: sa_motion_publish_stop(ctx,0x8c620f52u); r[3]=r[6];goto L8c620f54; /* mov r6,r3 */
L8c620f54: sa_motion_publish_stop(ctx,0x8c620f54u); r[3]+=16u;goto L8c620f56; /* add #0x10,r3 */
L8c620f56: sa_motion_publish_stop(ctx,0x8c620f56u); r[9]+=4u;goto L8c620f58; /* add #0x4,r9 */
L8c620f58: sa_motion_publish_stop(ctx,0x8c620f58u); wr(m,r[15]+0xcu,r[3],4);goto L8c620f5a; /* mov.l r3,@(0xc,r15) */
L8c620f5a: sa_motion_publish_stop(ctx,0x8c620f5au); wr(m,r[15],r[2],4);goto L8c620f5c; /* mov.l r2,@r15 */
L8c620f5c: sa_motion_publish_stop(ctx,0x8c620f5cu); wr(m,r[15]+0x10u,r[1],4);goto L8c620f5e; /* mov.l r1,@(0x10,r15) */
L8c620f5e: sa_motion_publish_stop(ctx,0x8c620f5eu); fload(m,f,0u,r[1]);goto L8c620f60; /* fmov.s @r1,fr0 */
L8c620f60: sa_motion_publish_stop(ctx,0x8c620f60u); fload(m,f,3u,r[2]);goto L8c620f62; /* fmov.s @r2,fr3 */
L8c620f62: sa_motion_publish_stop(ctx,0x8c620f62u); r[2]=r[6];goto L8c620f64; /* mov r6,r2 */
L8c620f64: sa_motion_publish_stop(ctx,0x8c620f64u); fload(m,f,2u,r[9]);goto L8c620f66; /* fmov.s @r9,fr2 */
L8c620f66: sa_motion_publish_stop(ctx,0x8c620f66u); r[2]+=20u;goto L8c620f68; /* add #0x14,r2 */
L8c620f68: sa_motion_publish_stop(ctx,0x8c620f68u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620f6a; /* fmul fr0,fr3 */
L8c620f6a: sa_motion_publish_stop(ctx,0x8c620f6au); fload(m,f,0u,r[3]);goto L8c620f6c; /* fmov.s @r3,fr0 */
L8c620f6c: sa_motion_publish_stop(ctx,0x8c620f6cu); fload(m,f,1u,r[7]);goto L8c620f6e; /* fmov.s @r7,fr1 */
L8c620f6e: sa_motion_publish_stop(ctx,0x8c620f6eu); r[3]=r[14];goto L8c620f70; /* mov r14,r3 */
L8c620f70: sa_motion_publish_stop(ctx,0x8c620f70u); r[3]+=4u;goto L8c620f72; /* add #0x4,r3 */
L8c620f72: sa_motion_publish_stop(ctx,0x8c620f72u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620f74; /* fmac fr0,fr2,fr3 */
L8c620f74: sa_motion_publish_stop(ctx,0x8c620f74u); fload(m,f,0u,r[6]);goto L8c620f76; /* fmov.s @r6,fr0 */
L8c620f76: sa_motion_publish_stop(ctx,0x8c620f76u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620f78; /* fmac fr0,fr1,fr3 */
L8c620f78: sa_motion_publish_stop(ctx,0x8c620f78u); fstore(m,f,3u,r[14]);goto L8c620f7a; /* fmov.s fr3,@r14 */
L8c620f7a: sa_motion_publish_stop(ctx,0x8c620f7au); wr(m,r[15]+0x28u,r[3],4);goto L8c620f7c; /* mov.l r3,@(0x28,r15) */
L8c620f7c: sa_motion_publish_stop(ctx,0x8c620f7cu); r[0]=r[6];goto L8c620f7e; /* mov r6,r0 */
L8c620f7e: sa_motion_publish_stop(ctx,0x8c620f7eu); r[0]+=36u;goto L8c620f80; /* add #0x24,r0 */
L8c620f80: sa_motion_publish_stop(ctx,0x8c620f80u); r[10]=r[6];goto L8c620f82; /* mov r6,r10 */
L8c620f82: sa_motion_publish_stop(ctx,0x8c620f82u); r[10]+=4u;goto L8c620f84; /* add #0x4,r10 */
L8c620f84: sa_motion_publish_stop(ctx,0x8c620f84u); wr(m,r[15]+0x8u,r[2],4);goto L8c620f86; /* mov.l r2,@(0x8,r15) */
L8c620f86: sa_motion_publish_stop(ctx,0x8c620f86u); r[12]=r[6];goto L8c620f88; /* mov r6,r12 */
L8c620f88: sa_motion_publish_stop(ctx,0x8c620f88u); r[1]=rd(m,r[15],4);goto L8c620f8a; /* mov.l @r15,r1 */
L8c620f8a: sa_motion_publish_stop(ctx,0x8c620f8au); r[12]+=40u;goto L8c620f8c; /* add #0x28,r12 */
L8c620f8c: sa_motion_publish_stop(ctx,0x8c620f8cu); r[11]=r[6];goto L8c620f8e; /* mov r6,r11 */
L8c620f8e: sa_motion_publish_stop(ctx,0x8c620f8eu); r[11]+=24u;goto L8c620f90; /* add #0x18,r11 */
L8c620f90: sa_motion_publish_stop(ctx,0x8c620f90u); fload(m,f,3u,r[1]);goto L8c620f92; /* fmov.s @r1,fr3 */
L8c620f92: sa_motion_publish_stop(ctx,0x8c620f92u); r[13]=r[6];goto L8c620f94; /* mov r6,r13 */
L8c620f94: sa_motion_publish_stop(ctx,0x8c620f94u); wr(m,r[15]+0x14u,r[0],4);goto L8c620f96; /* mov.l r0,@(0x14,r15) */
L8c620f96: sa_motion_publish_stop(ctx,0x8c620f96u); fload(m,f,0u,r[0]);goto L8c620f98; /* fmov.s @r0,fr0 */
L8c620f98: sa_motion_publish_stop(ctx,0x8c620f98u); fload(m,f,2u,r[9]);goto L8c620f9a; /* fmov.s @r9,fr2 */
L8c620f9a: sa_motion_publish_stop(ctx,0x8c620f9au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620f9c; /* fmul fr0,fr3 */
L8c620f9c: sa_motion_publish_stop(ctx,0x8c620f9cu); fload(m,f,0u,r[2]);goto L8c620f9e; /* fmov.s @r2,fr0 */
L8c620f9e: sa_motion_publish_stop(ctx,0x8c620f9eu); fload(m,f,1u,r[10]);goto L8c620fa0; /* fmov.s @r10,fr1 */
L8c620fa0: sa_motion_publish_stop(ctx,0x8c620fa0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620fa2; /* fmac fr0,fr2,fr3 */
L8c620fa2: sa_motion_publish_stop(ctx,0x8c620fa2u); fload(m,f,0u,r[7]);goto L8c620fa4; /* fmov.s @r7,fr0 */
L8c620fa4: sa_motion_publish_stop(ctx,0x8c620fa4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620fa6; /* fmac fr0,fr1,fr3 */
L8c620fa6: sa_motion_publish_stop(ctx,0x8c620fa6u); fstore(m,f,3u,r[3]);goto L8c620fa8; /* fmov.s fr3,@r3 */
L8c620fa8: sa_motion_publish_stop(ctx,0x8c620fa8u); r[3]=r[14];goto L8c620faa; /* mov r14,r3 */
L8c620faa: sa_motion_publish_stop(ctx,0x8c620faau); r[3]+=8u;goto L8c620fac; /* add #0x8,r3 */
L8c620fac: sa_motion_publish_stop(ctx,0x8c620facu); wr(m,r[15]+0x2cu,r[3],4);goto L8c620fae; /* mov.l r3,@(0x2c,r15) */
L8c620fae: sa_motion_publish_stop(ctx,0x8c620faeu); r[2]=rd(m,r[15],4);goto L8c620fb0; /* mov.l @r15,r2 */
L8c620fb0: sa_motion_publish_stop(ctx,0x8c620fb0u); fload(m,f,0u,r[12]);goto L8c620fb2; /* fmov.s @r12,fr0 */
L8c620fb2: sa_motion_publish_stop(ctx,0x8c620fb2u); fload(m,f,3u,r[2]);goto L8c620fb4; /* fmov.s @r2,fr3 */
L8c620fb4: sa_motion_publish_stop(ctx,0x8c620fb4u); fload(m,f,2u,r[9]);goto L8c620fb6; /* fmov.s @r9,fr2 */
L8c620fb6: sa_motion_publish_stop(ctx,0x8c620fb6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620fb8; /* fmul fr0,fr3 */
L8c620fb8: sa_motion_publish_stop(ctx,0x8c620fb8u); fload(m,f,0u,r[11]);goto L8c620fba; /* fmov.s @r11,fr0 */
L8c620fba: sa_motion_publish_stop(ctx,0x8c620fbau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620fbc; /* fmac fr0,fr2,fr3 */
L8c620fbc: sa_motion_publish_stop(ctx,0x8c620fbcu); r[13]+=8u;goto L8c620fbe; /* add #0x8,r13 */
L8c620fbe: sa_motion_publish_stop(ctx,0x8c620fbeu); fload(m,f,0u,r[7]);goto L8c620fc0; /* fmov.s @r7,fr0 */
L8c620fc0: sa_motion_publish_stop(ctx,0x8c620fc0u); fload(m,f,1u,r[13]);goto L8c620fc2; /* fmov.s @r13,fr1 */
L8c620fc2: sa_motion_publish_stop(ctx,0x8c620fc2u); r[1]=r[7];goto L8c620fc4; /* mov r7,r1 */
L8c620fc4: sa_motion_publish_stop(ctx,0x8c620fc4u); r[1]+=24u;goto L8c620fc6; /* add #0x18,r1 */
L8c620fc6: sa_motion_publish_stop(ctx,0x8c620fc6u); r[9]=r[7];goto L8c620fc8; /* mov r7,r9 */
L8c620fc8: sa_motion_publish_stop(ctx,0x8c620fc8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620fca; /* fmac fr0,fr1,fr3 */
L8c620fca: sa_motion_publish_stop(ctx,0x8c620fcau); r[9]+=20u;goto L8c620fcc; /* add #0x14,r9 */
L8c620fcc: sa_motion_publish_stop(ctx,0x8c620fccu); fstore(m,f,3u,r[3]);goto L8c620fce; /* fmov.s fr3,@r3 */
L8c620fce: sa_motion_publish_stop(ctx,0x8c620fceu); r[3]=r[14];goto L8c620fd0; /* mov r14,r3 */
L8c620fd0: sa_motion_publish_stop(ctx,0x8c620fd0u); r[3]+=16u;goto L8c620fd2; /* add #0x10,r3 */
L8c620fd2: sa_motion_publish_stop(ctx,0x8c620fd2u); wr(m,r[15]+0x18u,r[3],4);goto L8c620fd4; /* mov.l r3,@(0x18,r15) */
L8c620fd4: sa_motion_publish_stop(ctx,0x8c620fd4u); r[2]=rd(m,r[15]+0xcu,4);goto L8c620fd6; /* mov.l @(0xc,r15),r2 */
L8c620fd6: sa_motion_publish_stop(ctx,0x8c620fd6u); wr(m,r[15],r[1],4);goto L8c620fd8; /* mov.l r1,@r15 */
L8c620fd8: sa_motion_publish_stop(ctx,0x8c620fd8u); r[0]=rd(m,r[15]+0x10u,4);goto L8c620fda; /* mov.l @(0x10,r15),r0 */
L8c620fda: sa_motion_publish_stop(ctx,0x8c620fdau); fload(m,f,3u,r[1]);goto L8c620fdc; /* fmov.s @r1,fr3 */
L8c620fdc: sa_motion_publish_stop(ctx,0x8c620fdcu); fload(m,f,0u,r[0]);goto L8c620fde; /* fmov.s @r0,fr0 */
L8c620fde: sa_motion_publish_stop(ctx,0x8c620fdeu); r[0]=r[7];goto L8c620fe0; /* mov r7,r0 */
L8c620fe0: sa_motion_publish_stop(ctx,0x8c620fe0u); fload(m,f,2u,r[9]);goto L8c620fe2; /* fmov.s @r9,fr2 */
L8c620fe2: sa_motion_publish_stop(ctx,0x8c620fe2u); r[0]+=16u;goto L8c620fe4; /* add #0x10,r0 */
L8c620fe4: sa_motion_publish_stop(ctx,0x8c620fe4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620fe6; /* fmul fr0,fr3 */
L8c620fe6: sa_motion_publish_stop(ctx,0x8c620fe6u); fload(m,f,0u,r[2]);goto L8c620fe8; /* fmov.s @r2,fr0 */
L8c620fe8: sa_motion_publish_stop(ctx,0x8c620fe8u); wr(m,r[15]+0x4u,r[0],4);goto L8c620fea; /* mov.l r0,@(0x4,r15) */
L8c620fea: sa_motion_publish_stop(ctx,0x8c620feau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620fec; /* fmac fr0,fr2,fr3 */
L8c620fec: sa_motion_publish_stop(ctx,0x8c620fecu); fload(m,f,0u,r[6]);goto L8c620fee; /* fmov.s @r6,fr0 */
L8c620fee: sa_motion_publish_stop(ctx,0x8c620feeu); fload(m,f,2u,r[0]);goto L8c620ff0; /* fmov.s @r0,fr2 */
L8c620ff0: sa_motion_publish_stop(ctx,0x8c620ff0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c620ff2; /* fmac fr0,fr2,fr3 */
L8c620ff2: sa_motion_publish_stop(ctx,0x8c620ff2u); fstore(m,f,3u,r[3]);goto L8c620ff4; /* fmov.s fr3,@r3 */
L8c620ff4: sa_motion_publish_stop(ctx,0x8c620ff4u); r[3]=r[14];goto L8c620ff6; /* mov r14,r3 */
L8c620ff6: sa_motion_publish_stop(ctx,0x8c620ff6u); r[3]+=20u;goto L8c620ff8; /* add #0x14,r3 */
L8c620ff8: sa_motion_publish_stop(ctx,0x8c620ff8u); wr(m,r[15]+0x24u,r[3],4);goto L8c620ffa; /* mov.l r3,@(0x24,r15) */
L8c620ffa: sa_motion_publish_stop(ctx,0x8c620ffau); r[2]=rd(m,r[15],4);goto L8c620ffc; /* mov.l @r15,r2 */
L8c620ffc: sa_motion_publish_stop(ctx,0x8c620ffcu); fload(m,f,1u,r[2]);goto L8c620ffe; /* fmov.s @r2,fr1 */
L8c620ffe: sa_motion_publish_stop(ctx,0x8c620ffeu); r[2]=rd(m,r[15]+0x8u,4);goto L8c621000; /* mov.l @(0x8,r15),r2 */
L8c621000: sa_motion_publish_stop(ctx,0x8c621000u); fload(m,f,3u,r[9]);goto L8c621002; /* fmov.s @r9,fr3 */
L8c621002: sa_motion_publish_stop(ctx,0x8c621002u); fload(m,f,0u,r[2]);goto L8c621004; /* fmov.s @r2,fr0 */
L8c621004: sa_motion_publish_stop(ctx,0x8c621004u); r[0]=rd(m,r[15]+0x4u,4);goto L8c621006; /* mov.l @(0x4,r15),r0 */
L8c621006: sa_motion_publish_stop(ctx,0x8c621006u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621008; /* fmul fr0,fr3 */
L8c621008: sa_motion_publish_stop(ctx,0x8c621008u); fload(m,f,0u,r[10]);goto L8c62100a; /* fmov.s @r10,fr0 */
L8c62100a: sa_motion_publish_stop(ctx,0x8c62100au); fload(m,f,2u,r[0]);goto L8c62100c; /* fmov.s @r0,fr2 */
L8c62100c: sa_motion_publish_stop(ctx,0x8c62100cu); r[1]=rd(m,r[15]+0x14u,4);goto L8c62100e; /* mov.l @(0x14,r15),r1 */
L8c62100e: sa_motion_publish_stop(ctx,0x8c62100eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621010; /* fmac fr0,fr2,fr3 */
L8c621010: sa_motion_publish_stop(ctx,0x8c621010u); fload(m,f,0u,r[1]);goto L8c621012; /* fmov.s @r1,fr0 */
L8c621012: sa_motion_publish_stop(ctx,0x8c621012u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621014; /* fmac fr0,fr1,fr3 */
L8c621014: sa_motion_publish_stop(ctx,0x8c621014u); fstore(m,f,3u,r[3]);goto L8c621016; /* fmov.s fr3,@r3 */
L8c621016: sa_motion_publish_stop(ctx,0x8c621016u); r[3]=r[14];goto L8c621018; /* mov r14,r3 */
L8c621018: sa_motion_publish_stop(ctx,0x8c621018u); r[3]+=24u;goto L8c62101a; /* add #0x18,r3 */
L8c62101a: sa_motion_publish_stop(ctx,0x8c62101au); wr(m,r[15]+0x1cu,r[3],4);goto L8c62101c; /* mov.l r3,@(0x1c,r15) */
L8c62101c: sa_motion_publish_stop(ctx,0x8c62101cu); fload(m,f,0u,r[11]);goto L8c62101e; /* fmov.s @r11,fr0 */
L8c62101e: sa_motion_publish_stop(ctx,0x8c62101eu); fload(m,f,3u,r[9]);goto L8c621020; /* fmov.s @r9,fr3 */
L8c621020: sa_motion_publish_stop(ctx,0x8c621020u); r[1]=rd(m,r[15]+0x4u,4);goto L8c621022; /* mov.l @(0x4,r15),r1 */
L8c621022: sa_motion_publish_stop(ctx,0x8c621022u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621024; /* fmul fr0,fr3 */
L8c621024: sa_motion_publish_stop(ctx,0x8c621024u); fload(m,f,0u,r[13]);goto L8c621026; /* fmov.s @r13,fr0 */
L8c621026: sa_motion_publish_stop(ctx,0x8c621026u); fload(m,f,2u,r[1]);goto L8c621028; /* fmov.s @r1,fr2 */
L8c621028: sa_motion_publish_stop(ctx,0x8c621028u); r[2]=rd(m,r[15],4);goto L8c62102a; /* mov.l @r15,r2 */
L8c62102a: sa_motion_publish_stop(ctx,0x8c62102au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c62102c; /* fmac fr0,fr2,fr3 */
L8c62102c: sa_motion_publish_stop(ctx,0x8c62102cu); fload(m,f,0u,r[12]);goto L8c62102e; /* fmov.s @r12,fr0 */
L8c62102e: sa_motion_publish_stop(ctx,0x8c62102eu); fload(m,f,1u,r[2]);goto L8c621030; /* fmov.s @r2,fr1 */
L8c621030: sa_motion_publish_stop(ctx,0x8c621030u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621032; /* fmac fr0,fr1,fr3 */
L8c621032: sa_motion_publish_stop(ctx,0x8c621032u); fstore(m,f,3u,r[3]);goto L8c621034; /* fmov.s fr3,@r3 */
L8c621034: sa_motion_publish_stop(ctx,0x8c621034u); r[3]=r[14];goto L8c621036; /* mov r14,r3 */
L8c621036: sa_motion_publish_stop(ctx,0x8c621036u); r[3]+=32u;goto L8c621038; /* add #0x20,r3 */
L8c621038: sa_motion_publish_stop(ctx,0x8c621038u); wr(m,r[15]+0x20u,r[3],4);goto L8c62103a; /* mov.l r3,@(0x20,r15) */
L8c62103a: sa_motion_publish_stop(ctx,0x8c62103au); r[15]-=4;wr(m,r[15],r[3],4);goto L8c62103c; /* mov.l r3,@-r15 */
L8c62103c: sa_motion_publish_stop(ctx,0x8c62103cu); r[0]=r[7];goto L8c62103e; /* mov r7,r0 */
L8c62103e: sa_motion_publish_stop(ctx,0x8c62103eu); r[0]+=40u;goto L8c621040; /* add #0x28,r0 */
L8c621040: sa_motion_publish_stop(ctx,0x8c621040u); r[2]=r[7];goto L8c621042; /* mov r7,r2 */
L8c621042: sa_motion_publish_stop(ctx,0x8c621042u); r[2]+=36u;goto L8c621044; /* add #0x24,r2 */
L8c621044: sa_motion_publish_stop(ctx,0x8c621044u); r[9]=r[7];goto L8c621046; /* mov r7,r9 */
L8c621046: sa_motion_publish_stop(ctx,0x8c621046u); r[9]+=32u;goto L8c621048; /* add #0x20,r9 */
L8c621048: sa_motion_publish_stop(ctx,0x8c621048u); wr(m,r[15]+0x4u,r[2],4);goto L8c62104a; /* mov.l r2,@(0x4,r15) */
L8c62104a: sa_motion_publish_stop(ctx,0x8c62104au); r[7]=r[14];goto L8c62104c; /* mov r14,r7 */
L8c62104c: sa_motion_publish_stop(ctx,0x8c62104cu); r[1]=rd(m,r[15]+0x10u,4);goto L8c62104e; /* mov.l @(0x10,r15),r1 */
L8c62104e: sa_motion_publish_stop(ctx,0x8c62104eu); r[7]+=36u;goto L8c621050; /* add #0x24,r7 */
L8c621050: sa_motion_publish_stop(ctx,0x8c621050u); fload(m,f,2u,r[1]);goto L8c621052; /* fmov.s @r1,fr2 */
L8c621052: sa_motion_publish_stop(ctx,0x8c621052u); wr(m,r[15]+0x8u,r[0],4);goto L8c621054; /* mov.l r0,@(0x8,r15) */
L8c621054: sa_motion_publish_stop(ctx,0x8c621054u); r[3]=rd(m,r[15]+0x14u,4);goto L8c621056; /* mov.l @(0x14,r15),r3 */
L8c621056: sa_motion_publish_stop(ctx,0x8c621056u); fload(m,f,3u,r[0]);goto L8c621058; /* fmov.s @r0,fr3 */
L8c621058: sa_motion_publish_stop(ctx,0x8c621058u); fload(m,f,0u,r[3]);goto L8c62105a; /* fmov.s @r3,fr0 */
L8c62105a: sa_motion_publish_stop(ctx,0x8c62105au); fload(m,f,1u,r[2]);goto L8c62105c; /* fmov.s @r2,fr1 */
L8c62105c: sa_motion_publish_stop(ctx,0x8c62105cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c62105e; /* fmul fr0,fr3 */
L8c62105e: sa_motion_publish_stop(ctx,0x8c62105eu); fcopy(f,0u,2u);goto L8c621060; /* fmov fr2,fr0 */
L8c621060: sa_motion_publish_stop(ctx,0x8c621060u); fload(m,f,2u,r[9]);goto L8c621062; /* fmov.s @r9,fr2 */
L8c621062: sa_motion_publish_stop(ctx,0x8c621062u); r[1]=rd(m,r[15],4);r[15]+=4;goto L8c621064; /* mov.l @r15+,r1 */
L8c621064: sa_motion_publish_stop(ctx,0x8c621064u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621066; /* fmac fr0,fr1,fr3 */
L8c621066: sa_motion_publish_stop(ctx,0x8c621066u); fload(m,f,0u,r[6]);goto L8c621068; /* fmov.s @r6,fr0 */
L8c621068: sa_motion_publish_stop(ctx,0x8c621068u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c62106a; /* fmac fr0,fr2,fr3 */
L8c62106a: sa_motion_publish_stop(ctx,0x8c62106au); fstore(m,f,3u,r[1]);goto L8c62106c; /* fmov.s fr3,@r1 */
L8c62106c: sa_motion_publish_stop(ctx,0x8c62106cu); r[1]=rd(m,r[15],4);goto L8c62106e; /* mov.l @r15,r1 */
L8c62106e: sa_motion_publish_stop(ctx,0x8c62106eu); r[0]=rd(m,r[15]+0x8u,4);goto L8c621070; /* mov.l @(0x8,r15),r0 */
L8c621070: sa_motion_publish_stop(ctx,0x8c621070u); fload(m,f,3u,r[1]);goto L8c621072; /* fmov.s @r1,fr3 */
L8c621072: sa_motion_publish_stop(ctx,0x8c621072u); fload(m,f,0u,r[0]);goto L8c621074; /* fmov.s @r0,fr0 */
L8c621074: sa_motion_publish_stop(ctx,0x8c621074u); r[3]=rd(m,r[15]+0x4u,4);goto L8c621076; /* mov.l @(0x4,r15),r3 */
L8c621076: sa_motion_publish_stop(ctx,0x8c621076u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621078; /* fmul fr0,fr3 */
L8c621078: sa_motion_publish_stop(ctx,0x8c621078u); fload(m,f,0u,r[10]);goto L8c62107a; /* fmov.s @r10,fr0 */
L8c62107a: sa_motion_publish_stop(ctx,0x8c62107au); r[2]=rd(m,r[15]+0x14u,4);goto L8c62107c; /* mov.l @(0x14,r15),r2 */
L8c62107c: sa_motion_publish_stop(ctx,0x8c62107cu); fload(m,f,2u,r[9]);goto L8c62107e; /* fmov.s @r9,fr2 */
L8c62107e: sa_motion_publish_stop(ctx,0x8c62107eu); r[0]=80u;goto L8c621080; /* mov #0x50,r0 */
L8c621080: sa_motion_publish_stop(ctx,0x8c621080u); fload(m,f,1u,r[3]);goto L8c621082; /* fmov.s @r3,fr1 */
L8c621082: sa_motion_publish_stop(ctx,0x8c621082u); r[3]=r[14];goto L8c621084; /* mov r14,r3 */
L8c621084: sa_motion_publish_stop(ctx,0x8c621084u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621086; /* fmac fr0,fr2,fr3 */
L8c621086: sa_motion_publish_stop(ctx,0x8c621086u); fload(m,f,0u,r[2]);goto L8c621088; /* fmov.s @r2,fr0 */
L8c621088: sa_motion_publish_stop(ctx,0x8c621088u); r[3]+=40u;goto L8c62108a; /* add #0x28,r3 */
L8c62108a: sa_motion_publish_stop(ctx,0x8c62108au); r[6]=rd(m,0x8c621114u,4);goto L8c62108c; /* mov.l 0x8c621114,r6 */
L8c62108c: sa_motion_publish_stop(ctx,0x8c62108cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c62108e; /* fmac fr0,fr1,fr3 */
L8c62108e: sa_motion_publish_stop(ctx,0x8c62108eu); fstore(m,f,3u,r[7]);goto L8c621090; /* fmov.s fr3,@r7 */
L8c621090: sa_motion_publish_stop(ctx,0x8c621090u); wr(m,r[15]+0x8u,r[3],4);goto L8c621092; /* mov.l r3,@(0x8,r15) */
L8c621092: sa_motion_publish_stop(ctx,0x8c621092u); r[1]=rd(m,r[15],4);goto L8c621094; /* mov.l @r15,r1 */
L8c621094: sa_motion_publish_stop(ctx,0x8c621094u); fload(m,f,0u,r[11]);goto L8c621096; /* fmov.s @r11,fr0 */
L8c621096: sa_motion_publish_stop(ctx,0x8c621096u); fload(m,f,3u,r[1]);goto L8c621098; /* fmov.s @r1,fr3 */
L8c621098: sa_motion_publish_stop(ctx,0x8c621098u); fload(m,f,2u,r[9]);goto L8c62109a; /* fmov.s @r9,fr2 */
L8c62109a: sa_motion_publish_stop(ctx,0x8c62109au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c62109c; /* fmul fr0,fr3 */
L8c62109c: sa_motion_publish_stop(ctx,0x8c62109cu); fload(m,f,0u,r[13]);goto L8c62109e; /* fmov.s @r13,fr0 */
L8c62109e: sa_motion_publish_stop(ctx,0x8c62109eu); r[2]=rd(m,r[15]+0x4u,4);goto L8c6210a0; /* mov.l @(0x4,r15),r2 */
L8c6210a0: sa_motion_publish_stop(ctx,0x8c6210a0u); fload(m,f,1u,r[2]);goto L8c6210a2; /* fmov.s @r2,fr1 */
L8c6210a2: sa_motion_publish_stop(ctx,0x8c6210a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210a4; /* fmac fr0,fr2,fr3 */
L8c6210a4: sa_motion_publish_stop(ctx,0x8c6210a4u); fload(m,f,0u,r[12]);goto L8c6210a6; /* fmov.s @r12,fr0 */
L8c6210a6: sa_motion_publish_stop(ctx,0x8c6210a6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210a8; /* fmac fr0,fr1,fr3 */
L8c6210a8: sa_motion_publish_stop(ctx,0x8c6210a8u); fstore(m,f,3u,r[3]);goto L8c6210aa; /* fmov.s fr3,@r3 */
L8c6210aa: sa_motion_publish_stop(ctx,0x8c6210aau); r[2]=rd(m,r[15]+0x18u,4);goto L8c6210ac; /* mov.l @(0x18,r15),r2 */
L8c6210ac: sa_motion_publish_stop(ctx,0x8c6210acu); fload(m,f,0u,r[0]+r[5]);goto L8c6210ae; /* fmov.s @(r0,r5),fr0 */
L8c6210ae: sa_motion_publish_stop(ctx,0x8c6210aeu); r[0]=76u;goto L8c6210b0; /* mov #0x4c,r0 */
L8c6210b0: sa_motion_publish_stop(ctx,0x8c6210b0u); fload(m,f,3u,r[2]);goto L8c6210b2; /* fmov.s @r2,fr3 */
L8c6210b2: sa_motion_publish_stop(ctx,0x8c6210b2u); fload(m,f,2u,r[14]);goto L8c6210b4; /* fmov.s @r14,fr2 */
L8c6210b4: sa_motion_publish_stop(ctx,0x8c6210b4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210b6; /* fmul fr0,fr3 */
L8c6210b6: sa_motion_publish_stop(ctx,0x8c6210b6u); fload(m,f,0u,r[0]+r[5]);goto L8c6210b8; /* fmov.s @(r0,r5),fr0 */
L8c6210b8: sa_motion_publish_stop(ctx,0x8c6210b8u); r[3]=rd(m,r[15]+0x20u,4);goto L8c6210ba; /* mov.l @(0x20,r15),r3 */
L8c6210ba: sa_motion_publish_stop(ctx,0x8c6210bau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210bc; /* fmac fr0,fr2,fr3 */
L8c6210bc: sa_motion_publish_stop(ctx,0x8c6210bcu); r[0]=84u;goto L8c6210be; /* mov #0x54,r0 */
L8c6210be: sa_motion_publish_stop(ctx,0x8c6210beu); fload(m,f,2u,r[3]);goto L8c6210c0; /* fmov.s @r3,fr2 */
L8c6210c0: sa_motion_publish_stop(ctx,0x8c6210c0u); fload(m,f,0u,r[0]+r[5]);goto L8c6210c2; /* fmov.s @(r0,r5),fr0 */
L8c6210c2: sa_motion_publish_stop(ctx,0x8c6210c2u); r[0]=80u;goto L8c6210c4; /* mov #0x50,r0 */
L8c6210c4: sa_motion_publish_stop(ctx,0x8c6210c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210c6; /* fmac fr0,fr2,fr3 */
L8c6210c6: sa_motion_publish_stop(ctx,0x8c6210c6u); fstore(m,f,3u,r[6]);goto L8c6210c8; /* fmov.s fr3,@r6 */
L8c6210c8: sa_motion_publish_stop(ctx,0x8c6210c8u); r[2]=rd(m,r[15]+0x24u,4);goto L8c6210ca; /* mov.l @(0x24,r15),r2 */
L8c6210ca: sa_motion_publish_stop(ctx,0x8c6210cau); fload(m,f,0u,r[0]+r[5]);goto L8c6210cc; /* fmov.s @(r0,r5),fr0 */
L8c6210cc: sa_motion_publish_stop(ctx,0x8c6210ccu); r[0]=76u;goto L8c6210ce; /* mov #0x4c,r0 */
L8c6210ce: sa_motion_publish_stop(ctx,0x8c6210ceu); fload(m,f,3u,r[2]);goto L8c6210d0; /* fmov.s @r2,fr3 */
L8c6210d0: sa_motion_publish_stop(ctx,0x8c6210d0u); r[3]=rd(m,r[15]+0x28u,4);goto L8c6210d2; /* mov.l @(0x28,r15),r3 */
L8c6210d2: sa_motion_publish_stop(ctx,0x8c6210d2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210d4; /* fmul fr0,fr3 */
L8c6210d4: sa_motion_publish_stop(ctx,0x8c6210d4u); fload(m,f,0u,r[0]+r[5]);goto L8c6210d6; /* fmov.s @(r0,r5),fr0 */
L8c6210d6: sa_motion_publish_stop(ctx,0x8c6210d6u); fload(m,f,2u,r[3]);goto L8c6210d8; /* fmov.s @r3,fr2 */
L8c6210d8: sa_motion_publish_stop(ctx,0x8c6210d8u); r[0]=84u;goto L8c6210da; /* mov #0x54,r0 */
L8c6210da: sa_motion_publish_stop(ctx,0x8c6210dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210dc; /* fmac fr0,fr2,fr3 */
L8c6210dc: sa_motion_publish_stop(ctx,0x8c6210dcu); fload(m,f,2u,r[7]);goto L8c6210de; /* fmov.s @r7,fr2 */
L8c6210de: sa_motion_publish_stop(ctx,0x8c6210deu); fload(m,f,0u,r[0]+r[5]);goto L8c6210e0; /* fmov.s @(r0,r5),fr0 */
L8c6210e0: sa_motion_publish_stop(ctx,0x8c6210e0u); r[0]=4u;goto L8c6210e2; /* mov #0x4,r0 */
L8c6210e2: sa_motion_publish_stop(ctx,0x8c6210e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210e4; /* fmac fr0,fr2,fr3 */
L8c6210e4: sa_motion_publish_stop(ctx,0x8c6210e4u); fstore(m,f,3u,r[0]+r[6]);goto L8c6210e6; /* fmov fr3,@(r0,r6) */
L8c6210e6: sa_motion_publish_stop(ctx,0x8c6210e6u); r[0]=80u;goto L8c6210e8; /* mov #0x50,r0 */
L8c6210e8: sa_motion_publish_stop(ctx,0x8c6210e8u); r[1]=rd(m,r[15]+0x1cu,4);goto L8c6210ea; /* mov.l @(0x1c,r15),r1 */
L8c6210ea: sa_motion_publish_stop(ctx,0x8c6210eau); fload(m,f,0u,r[0]+r[5]);goto L8c6210ec; /* fmov.s @(r0,r5),fr0 */
L8c6210ec: sa_motion_publish_stop(ctx,0x8c6210ecu); r[0]=76u;goto L8c6210ee; /* mov #0x4c,r0 */
L8c6210ee: sa_motion_publish_stop(ctx,0x8c6210eeu); fload(m,f,3u,r[1]);goto L8c6210f0; /* fmov.s @r1,fr3 */
L8c6210f0: sa_motion_publish_stop(ctx,0x8c6210f0u); r[2]=rd(m,r[15]+0x2cu,4);goto L8c6210f2; /* mov.l @(0x2c,r15),r2 */
L8c6210f2: sa_motion_publish_stop(ctx,0x8c6210f2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210f4; /* fmul fr0,fr3 */
L8c6210f4: sa_motion_publish_stop(ctx,0x8c6210f4u); fload(m,f,0u,r[0]+r[5]);goto L8c6210f6; /* fmov.s @(r0,r5),fr0 */
L8c6210f6: sa_motion_publish_stop(ctx,0x8c6210f6u); fload(m,f,2u,r[2]);goto L8c6210f8; /* fmov.s @r2,fr2 */
L8c6210f8: sa_motion_publish_stop(ctx,0x8c6210f8u); r[3]=rd(m,r[15]+0x8u,4);goto L8c6210fa; /* mov.l @(0x8,r15),r3 */
L8c6210fa: sa_motion_publish_stop(ctx,0x8c6210fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6210fc; /* fmac fr0,fr2,fr3 */
L8c6210fc: sa_motion_publish_stop(ctx,0x8c6210fcu); r[0]=84u;goto L8c6210fe; /* mov #0x54,r0 */
L8c6210fe: sa_motion_publish_stop(ctx,0x8c6210feu); fload(m,f,2u,r[3]);goto L8c621100; /* fmov.s @r3,fr2 */
L8c621100: sa_motion_publish_stop(ctx,0x8c621100u); fload(m,f,0u,r[0]+r[5]);goto L8c621102; /* fmov.s @(r0,r5),fr0 */
L8c621102: sa_motion_publish_stop(ctx,0x8c621102u); r[0]=8u;goto L8c621104; /* mov #0x8,r0 */
L8c621104: sa_motion_publish_stop(ctx,0x8c621104u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c621106; /* fmac fr0,fr2,fr3 */
L8c621106: sa_motion_publish_stop(ctx,0x8c621106u); fstore(m,f,3u,r[0]+r[6]);goto L8c621108; /* fmov fr3,@(r0,r6) */
L8c621108: sa_motion_publish_stop(ctx,0x8c62110au); goto L8c621118; /* bra 0x8c621118 */
L8c621118: sa_motion_publish_stop(ctx,0x8c621118u); r[0]=sx(m,0x8c621234u,2);goto L8c62111a; /* mov.w 0x8c621234,r0 */
L8c62111a: sa_motion_publish_stop(ctx,0x8c62111au); r[3]=rd(m,0x8c621238u,4);goto L8c62111c; /* mov.l 0x8c621238,r3 */
L8c62111c: sa_motion_publish_stop(ctx,0x8c62111cu); fload(m,f,3u,r[0]+r[5]);goto L8c62111e; /* fmov.s @(r0,r5),fr3 */
L8c62111e: sa_motion_publish_stop(ctx,0x8c62111eu); r[0]+=4294967292u;goto L8c621120; /* add #-0x4,r0 */
L8c621120: sa_motion_publish_stop(ctx,0x8c621120u); r[2]=rd(m,0x8c62123cu,4);goto L8c621122; /* mov.l 0x8c62123c,r2 */
L8c621122: sa_motion_publish_stop(ctx,0x8c621122u); fstore(m,f,3u,r[3]);goto L8c621124; /* fmov.s fr3,@r3 */
L8c621124: sa_motion_publish_stop(ctx,0x8c621124u); fload(m,f,3u,r[0]+r[5]);goto L8c621126; /* fmov.s @(r0,r5),fr3 */
L8c621126: sa_motion_publish_stop(ctx,0x8c621128u); target=0x8c6215fau;*pr=0x8c62112au;fstore(m,f,3u,r[2]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62112a; /* bsr 0x8c6215fa */
L8c62112a: sa_motion_publish_stop(ctx,0x8c62112au); r[15]+=112u;goto L8c62112c; /* add #0x70,r15 */
L8c62112c: sa_motion_publish_stop(ctx,0x8c62112cu); *pr=rd(m,r[15],4);r[15]+=4;goto L8c62112e; /* lds.l @r15+,PR */
L8c62112e: sa_motion_publish_stop(ctx,0x8c62112eu); r[9]=rd(m,r[15],4);r[15]+=4;goto L8c621130; /* mov.l @r15+,r9 */
L8c621130: sa_motion_publish_stop(ctx,0x8c621130u); r[10]=rd(m,r[15],4);r[15]+=4;goto L8c621132; /* mov.l @r15+,r10 */
L8c621132: sa_motion_publish_stop(ctx,0x8c621132u); r[11]=rd(m,r[15],4);r[15]+=4;goto L8c621134; /* mov.l @r15+,r11 */
L8c621134: sa_motion_publish_stop(ctx,0x8c621134u); r[12]=rd(m,r[15],4);r[15]+=4;goto L8c621136; /* mov.l @r15+,r12 */
L8c621136: sa_motion_publish_stop(ctx,0x8c621136u); r[13]=rd(m,r[15],4);r[15]+=4;goto L8c621138; /* mov.l @r15+,r13 */
L8c621138: sa_motion_publish_stop(ctx,0x8c62113au); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_particle_model_wrapper_8c6215fa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6215fa;
L8c6215fa: sa_motion_publish_stop(ctx,0x8c6215fau); r[15]-=4;wr(m,r[15],*pr,4);goto L8c6215fc; /* sts.l PR,@-r15 */
L8c6215fc: sa_motion_publish_stop(ctx,0x8c6215fcu); r[15]+=4294967288u;goto L8c6215fe; /* add #-0x8,r15 */
L8c6215fe: sa_motion_publish_stop(ctx,0x8c6215feu); wr(m,r[15]+0x4u,r[4],4);goto L8c621600; /* mov.l r4,@(0x4,r15) */
L8c621600: sa_motion_publish_stop(ctx,0x8c621600u); r[4]=r[15];goto L8c621602; /* mov r15,r4 */
L8c621602: sa_motion_publish_stop(ctx,0x8c621602u); wr(m,r[4],r[8],4);goto L8c621604; /* mov.l r8,@r4 */
L8c621604: sa_motion_publish_stop(ctx,0x8c621606u); target=0x8c62113cu;*pr=0x8c621608u;r[4]=rd(m,r[15]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c621608; /* bsr 0x8c62113c */
L8c621608: sa_motion_publish_stop(ctx,0x8c621608u); r[4]=r[15];goto L8c62160a; /* mov r15,r4 */
L8c62160a: sa_motion_publish_stop(ctx,0x8c62160au); r[8]=rd(m,r[4],4);goto L8c62160c; /* mov.l @r4,r8 */
L8c62160c: sa_motion_publish_stop(ctx,0x8c62160cu); r[15]+=8u;goto L8c62160e; /* add #0x8,r15 */
L8c62160e: sa_motion_publish_stop(ctx,0x8c62160eu); *pr=rd(m,r[15],4);r[15]+=4;goto L8c621610; /* lds.l @r15+,PR */
L8c621610: sa_motion_publish_stop(ctx,0x8c621612u); return 0; /* rts */
}
