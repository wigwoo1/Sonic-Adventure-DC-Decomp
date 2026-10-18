/* Renderer initialization, geometry and explicit GBR state; names inferred.
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
SA_STAGE_EXPORT int sa_gbr_geometry_8c036f60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c036f60;
L8c036f60: *stop=0x8c036f60u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c036f62; /* sts.l PR,@-r15 */
L8c036f62: *stop=0x8c036f62u;r[1]=rd(m,0x8c036fb4u,4);goto L8c036f64; /* mov.l 0x8c036fb4,r1 */
L8c036f64: *stop=0x8c036f64u;*gbr=r[1];goto L8c036f66; /* ldc r1,GBR */
L8c036f66: *stop=0x8c036f66u;r[2]=rd(m,0x8c036fb8u,4);goto L8c036f68; /* mov.l 0x8c036fb8,r2 */
L8c036f68: *stop=0x8c036f68u;r[1]=rd(m,0x8c036fb4u,4);goto L8c036f6a; /* mov.l 0x8c036fb4,r1 */
L8c036f6a: *stop=0x8c036f6au;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[2]);goto L8c036f6c; /* fmov.s @r2,fr0 */
L8c036f6c: *stop=0x8c036f6cu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[1]);goto L8c036f6e; /* fmov.s fr0,@r1 */
L8c036f6e: *stop=0x8c036f6eu;r[1]+=4u;goto L8c036f70; /* add #0x4,r1 */
L8c036f70: *stop=0x8c036f70u;r[2]=rd(m,0x8c036fbcu,4);goto L8c036f72; /* mov.l 0x8c036fbc,r2 */
L8c036f72: *stop=0x8c036f72u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[2]);goto L8c036f74; /* fmov.s @r2,fr0 */
L8c036f74: *stop=0x8c036f74u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[1]);goto L8c036f76; /* fmov.s fr0,@r1 */
L8c036f76: *stop=0x8c036f76u;r[1]+=4u;goto L8c036f78; /* add #0x4,r1 */
L8c036f78: *stop=0x8c036f78u;r[0]=rd(m,0x8c036fc0u,4);goto L8c036f7a; /* mov.l 0x8c036fc0,r0 */
L8c036f7a: *stop=0x8c036f7au;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]);goto L8c036f7c; /* fmov.s @r0,fr4 */
L8c036f7c: *stop=0x8c036f7cu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[1]);goto L8c036f7e; /* fmov.s fr4,@r1 */
L8c036f7e: *stop=0x8c036f7eu;r[1]+=4u;goto L8c036f80; /* add #0x4,r1 */
L8c036f80: *stop=0x8c036f80u;r[2]=rd(m,0x8c036fc4u,4);goto L8c036f82; /* mov.l 0x8c036fc4,r2 */
L8c036f82: *stop=0x8c036f82u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[2]);goto L8c036f84; /* fmov.s @r2,fr0 */
L8c036f84: *stop=0x8c036f84u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[1]);goto L8c036f86; /* fmov.s fr0,@r1 */
L8c036f86: *stop=0x8c036f86u;r[1]+=4u;goto L8c036f88; /* add #0x4,r1 */
L8c036f88: *stop=0x8c036f88u;r[2]=rd(m,0x8c036fc8u,4);goto L8c036f8a; /* mov.l 0x8c036fc8,r2 */
L8c036f8a: *stop=0x8c036f8au;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[2]);goto L8c036f8c; /* fmov.s @r2,fr0 */
L8c036f8c: *stop=0x8c036f8cu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[1]);goto L8c036f8e; /* fmov.s fr0,@r1 */
L8c036f8e: *stop=0x8c036f8eu;r[1]+=4u;goto L8c036f90; /* add #0x4,r1 */
L8c036f90: *stop=0x8c036f90u;r[2]=rd(m,0x8c036fccu,4);goto L8c036f92; /* mov.l 0x8c036fcc,r2 */
L8c036f92: *stop=0x8c036f92u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[2]);goto L8c036f94; /* fmov.s @r2,fr0 */
L8c036f94: *stop=0x8c036f94u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c036f96; /* fmul fr4,fr0 */
L8c036f96: *stop=0x8c036f96u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[1]);goto L8c036f98; /* fmov.s fr0,@r1 */
L8c036f98: *stop=0x8c036f98u;r[1]+=4u;goto L8c036f9a; /* add #0x4,r1 */
L8c036f9a: *stop=0x8c036f9au;r[2]=rd(m,0x8c036fc4u,4);goto L8c036f9c; /* mov.l 0x8c036fc4,r2 */
L8c036f9c: *stop=0x8c036f9cu;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[2]);goto L8c036f9e; /* fmov.s @r2,fr0 */
L8c036f9e: *stop=0x8c036f9eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c036fa0; /* fmul fr4,fr0 */
L8c036fa0: *stop=0x8c036fa0u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,0u,r[1]);goto L8c036fa2; /* fmov.s fr0,@r1 */
L8c036fa2: *stop=0x8c036fa2u;r[1]+=4u;goto L8c036fa4; /* add #0x4,r1 */
L8c036fa4: *stop=0x8c036fa4u;r[0]=rd(m,0x8c036fd0u,4);goto L8c036fa6; /* mov.l 0x8c036fd0,r0 */
L8c036fa6: *stop=0x8c036fa6u;if(!valid_ram(*gbr+0x58u,4u))return -3;wr(m,*gbr+0x58u,r[0],4);goto L8c036fa8; /* mov.l r0,@(0x58,GBR) */
L8c036fa8: *stop=0x8c036fa8u;r[0]=rd(m,0x8c036fd4u,4);goto L8c036faa; /* mov.l 0x8c036fd4,r0 */
L8c036faa: *stop=0x8c036faau;if(!valid_ram(*gbr+0x38u,4u))return -3;wr(m,*gbr+0x38u,r[0],4);goto L8c036fac; /* mov.l r0,@(0x38,GBR) */
L8c036fac: *stop=0x8c036facu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c036fae; /* lds.l @r15+,PR */
L8c036fae: *stop=0x8c036faeu;if(*pr!=entry_pr)return -3;*stop=0x8c036fb0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_gbr_geometry_8c03718c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c03718c;
L8c03718c: *stop=0x8c03718cu;r[0]=24u;goto L8c03718e; /* mov #0x18,r0 */
L8c03718e: *stop=0x8c03718eu;r[0]+=r[4];goto L8c037190; /* add r4,r0 */
L8c037190: *stop=0x8c037190u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037192; /* fmov.s @r0+,fr0 */
L8c037192: *stop=0x8c037192u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037194; /* fmov.s @r0+,fr1 */
L8c037194: *stop=0x8c037194u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037196; /* fmov.s @r0+,fr2 */
L8c037196: *stop=0x8c037196u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c037198; /* fldi1 fr3 */
L8c037198: *stop=0x8c037198u;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c03719a; /* ftrv xmtrx,fv0 */
L8c03719a: *stop=0x8c03719au;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03719c; /* fmov.s @r0+,fr3 */
L8c03719c: *stop=0x8c03719cu;if(!valid_ram(*gbr+0x10u,4u))return -3;r[0]=rd(m,*gbr+0x10u,4);goto L8c03719e; /* mov.l @(0x10,GBR),r0 */
L8c03719e: *stop=0x8c03719eu;*fpul=r[0];goto L8c0371a0; /* lds r0,FPUL */
L8c0371a0: *stop=0x8c0371a0u;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c0371a2; /* fsts FPUL,fr4 */
L8c0371a2: *stop=0x8c0371a2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c0371a4; /* fsub fr3,fr4 */
L8c0371a4: *stop=0x8c0371a4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c0371a6; /* fcmp/gt fr4,fr2 */
L8c0371a6: if(!(*sr&1u))goto L8c037208;goto L8c0371a8; /* bf 0x8c037208 */
L8c0371a8: *stop=0x8c0371a8u;r[1]=rd(m,0x8c03720cu,4);goto L8c0371aa; /* mov.l 0x8c03720c,r1 */
L8c0371aa: *stop=0x8c0371aau;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[1]);goto L8c0371ac; /* fmov.s @r1,fr4 */
L8c0371ac: *stop=0x8c0371acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c0371ae; /* fadd fr3,fr4 */
L8c0371ae: *stop=0x8c0371aeu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c0371b0; /* fcmp/gt fr4,fr2 */
L8c0371b0: if((*sr&1u))goto L8c037208;goto L8c0371b2; /* bt 0x8c037208 */
L8c0371b2: *stop=0x8c0371b2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c0371b4; /* fldi1 fr5 */
L8c0371b4: *stop=0x8c0371b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0371b6; /* fadd fr3,fr2 */
L8c0371b6: *stop=0x8c0371b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[2],&f->fpscr,&fr[5]);if(status)return status;}goto L8c0371b8; /* fdiv fr2,fr5 */
L8c0371b8: *stop=0x8c0371b8u;if(!valid_ram(*gbr+0x8u,4u))return -3;r[0]=rd(m,*gbr+0x8u,4);goto L8c0371ba; /* mov.l @(0x8,GBR),r0 */
L8c0371ba: *stop=0x8c0371bau;*fpul=r[0];goto L8c0371bc; /* lds r0,FPUL */
L8c0371bc: *stop=0x8c0371bcu;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c0371be; /* fsts FPUL,fr4 */
L8c0371be: *stop=0x8c0371beu;fcopy(f,6u,4u);goto L8c0371c0; /* fmov fr4,fr6 */
L8c0371c0: *stop=0x8c0371c0u;r[3]=rd(m,0x8c037210u,4);goto L8c0371c2; /* mov.l 0x8c037210,r3 */
L8c0371c2: *stop=0x8c0371c2u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[3]);r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0371c4; /* fmov.s @r3+,fr8 */
L8c0371c4: *stop=0x8c0371c4u;r[1]=rd(m,0x8c037214u,4);goto L8c0371c6; /* mov.l 0x8c037214,r1 */
L8c0371c6: *stop=0x8c0371c6u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[1]);r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0371c8; /* fmov.s @r1+,fr4 */
L8c0371c8: *stop=0x8c0371c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[5],&f->fpscr,&fr[6]);if(status)return status;}goto L8c0371ca; /* fmul fr5,fr6 */
L8c0371ca: *stop=0x8c0371cau;fcopy(f,11u,0u);goto L8c0371cc; /* fmov fr0,fr11 */
L8c0371cc: *stop=0x8c0371ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c0371ce; /* fadd fr3,fr0 */
L8c0371ce: *stop=0x8c0371ceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[6],&f->fpscr,&fr[0]);if(status)return status;}goto L8c0371d0; /* fmul fr6,fr0 */
L8c0371d0: *stop=0x8c0371d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[4],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c0371d2; /* fadd fr4,fr0 */
L8c0371d2: *stop=0x8c0371d2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][8],&f->fpscr,sr,1);if(status)return status;goto L8c0371d4; /* fcmp/gt fr8,fr0 */
L8c0371d4: if(!(*sr&1u))goto L8c037208;goto L8c0371d6; /* bf 0x8c037208 */
L8c0371d6: *stop=0x8c0371d6u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[3]);r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0371d8; /* fmov.s @r3+,fr9 */
L8c0371d8: *stop=0x8c0371d8u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[3]);r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0371da; /* fmov.s @r3+,fr8 */
L8c0371da: *stop=0x8c0371dau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c0371dc; /* fsub fr3,fr11 */
L8c0371dc: *stop=0x8c0371dcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[6],&f->fpscr,&fr[11]);if(status)return status;}goto L8c0371de; /* fmul fr6,fr11 */
L8c0371de: *stop=0x8c0371deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[4],&f->fpscr,&fr[11],0);if(status)return status;}goto L8c0371e0; /* fadd fr4,fr11 */
L8c0371e0: *stop=0x8c0371e0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][11],f->bank[(f->fpscr>>21)&1u][8],&f->fpscr,sr,1);if(status)return status;goto L8c0371e2; /* fcmp/gt fr8,fr11 */
L8c0371e2: if((*sr&1u))goto L8c037208;goto L8c0371e4; /* bt 0x8c037208 */
L8c0371e4: *stop=0x8c0371e4u;if(!valid_ram(*gbr+0xcu,4u))return -3;r[0]=rd(m,*gbr+0xcu,4);goto L8c0371e6; /* mov.l @(0xc,GBR),r0 */
L8c0371e6: *stop=0x8c0371e6u;*fpul=r[0];goto L8c0371e8; /* lds r0,FPUL */
L8c0371e8: *stop=0x8c0371e8u;f->bank[(f->fpscr>>21)&1u][7]=*fpul;goto L8c0371ea; /* fsts FPUL,fr7 */
L8c0371ea: *stop=0x8c0371eau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[4],&f->fpscr,&fr[7]);if(status)return status;}goto L8c0371ec; /* fmul fr4,fr7 */
L8c0371ec: *stop=0x8c0371ecu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[1]);r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0371ee; /* fmov.s @r1+,fr4 */
L8c0371ee: *stop=0x8c0371eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[5],&f->fpscr,&fr[7]);if(status)return status;}goto L8c0371f0; /* fmul fr5,fr7 */
L8c0371f0: *stop=0x8c0371f0u;fcopy(f,11u,1u);goto L8c0371f2; /* fmov fr1,fr11 */
L8c0371f2: *stop=0x8c0371f2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c0371f4; /* fadd fr3,fr1 */
L8c0371f4: *stop=0x8c0371f4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c0371f6; /* fmul fr7,fr1 */
L8c0371f6: *stop=0x8c0371f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[4],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c0371f8; /* fadd fr4,fr1 */
L8c0371f8: *stop=0x8c0371f8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][9],&f->fpscr,sr,1);if(status)return status;goto L8c0371fa; /* fcmp/gt fr9,fr1 */
L8c0371fa: if(!(*sr&1u))goto L8c037208;goto L8c0371fc; /* bf 0x8c037208 */
L8c0371fc: *stop=0x8c0371fcu;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[3]);r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0371fe; /* fmov.s @r3+,fr9 */
L8c0371fe: *stop=0x8c0371feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c037200; /* fsub fr3,fr11 */
L8c037200: *stop=0x8c037200u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[6],&f->fpscr,&fr[11]);if(status)return status;}goto L8c037202; /* fmul fr6,fr11 */
L8c037202: *stop=0x8c037202u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[4],&f->fpscr,&fr[11],0);if(status)return status;}goto L8c037204; /* fadd fr4,fr11 */
L8c037204: *stop=0x8c037204u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][11],f->bank[(f->fpscr>>21)&1u][8],&f->fpscr,sr,1);if(status)return status;goto L8c037206; /* fcmp/gt fr8,fr11 */
L8c037206: if(!(*sr&1u))goto L8c037218;goto L8c037208; /* bf 0x8c037218 */
L8c037208: *stop=0x8c037208u;if(*pr!=entry_pr)return -3;*stop=0x8c03720au;*sr|=1u;return 0; /* rts */
L8c037218: *stop=0x8c037218u;r[0]=0u;goto L8c03721a; /* mov #0x0,r0 */
L8c03721a: *stop=0x8c03721au;r[2]=rd(m,0x8c037278u,4);goto L8c03721c; /* mov.l 0x8c037278,r2 */
L8c03721c: *stop=0x8c03721cu;if(!valid_ram(r[2],4u))return -3;r[6]=rd(m,r[2],4);goto L8c03721e; /* mov.l @r2,r6 */
L8c03721e: *stop=0x8c03721eu;r[3]=rd(m,0x8c03727cu,4);goto L8c037220; /* mov.l 0x8c03727c,r3 */
L8c037220: *stop=0x8c037220u;*sr=(*sr&~1u)|!(r[3]&r[6]);goto L8c037222; /* tst r3,r6 */
L8c037222: if((*sr&1u))goto L8c037226;goto L8c037224; /* bt 0x8c037226 */
L8c037224: *stop=0x8c037224u;r[0]=1u;goto L8c037226; /* mov #0x1,r0 */
L8c037226: *stop=0x8c037226u;if(!valid_ram(*gbr+0x1cu,4u))return -3;wr(m,*gbr+0x1cu,r[0],4);goto L8c037228; /* mov.l r0,@(0x1c,GBR) */
L8c037228: *stop=0x8c037228u;r[0]=rd(m,0x8c037280u,4);goto L8c03722a; /* mov.l 0x8c037280,r0 */
L8c03722a: *stop=0x8c03722au;if(!valid_ram(r[0],4u))return -3;r[0]=rd(m,r[0],4);goto L8c03722c; /* mov.l @r0,r0 */
L8c03722c: *stop=0x8c03722cu;if(!valid_ram(*gbr+0x20u,4u))return -3;wr(m,*gbr+0x20u,r[0],4);goto L8c03722e; /* mov.l r0,@(0x20,GBR) */
L8c03722e: *stop=0x8c03722eu;r[0]=rd(m,0x8c037284u,4);goto L8c037230; /* mov.l 0x8c037284,r0 */
L8c037230: *stop=0x8c037230u;if(!valid_ram(r[0],4u))return -3;r[0]=rd(m,r[0],4);goto L8c037232; /* mov.l @r0,r0 */
L8c037232: *stop=0x8c037232u;if(!valid_ram(*gbr+0x24u,4u))return -3;wr(m,*gbr+0x24u,r[0],4);goto L8c037234; /* mov.l r0,@(0x24,GBR) */
L8c037234: *stop=0x8c037234u;r[0]=0u;goto L8c037236; /* mov #0x0,r0 */
L8c037236: *stop=0x8c037236u;r[3]=rd(m,0x8c037288u,4);goto L8c037238; /* mov.l 0x8c037288,r3 */
L8c037238: *stop=0x8c037238u;*sr=(*sr&~1u)|!(r[3]&r[6]);goto L8c03723a; /* tst r3,r6 */
L8c03723a: if((*sr&1u))goto L8c03723e;goto L8c03723c; /* bt 0x8c03723e */
L8c03723c: *stop=0x8c03723cu;r[0]=4294967295u;goto L8c03723e; /* mov #-0x1,r0 */
L8c03723e: *stop=0x8c03723eu;r[3]=rd(m,0x8c03728cu,4);goto L8c037240; /* mov.l 0x8c03728c,r3 */
L8c037240: *stop=0x8c037240u;*sr=(*sr&~1u)|!(r[3]&r[6]);goto L8c037242; /* tst r3,r6 */
L8c037242: if((*sr&1u))goto L8c037246;goto L8c037244; /* bt 0x8c037246 */
L8c037244: *stop=0x8c037244u;r[0]=1u;goto L8c037246; /* mov #0x1,r0 */
L8c037246: *stop=0x8c037246u;if(!valid_ram(*gbr+0x28u,4u))return -3;wr(m,*gbr+0x28u,r[0],4);goto L8c037248; /* mov.l r0,@(0x28,GBR) */
L8c037248: *stop=0x8c037248u;if(!valid_ram(r[4]+12u,4u))return -3;r[2]=rd(m,r[4]+0xcu,4);goto L8c03724a; /* mov.l @(0xc,r4),r2 */
L8c03724a: *stop=0x8c03724au;if(!valid_ram(r[4]+16u,4u))return -3;r[3]=rd(m,r[4]+0x10u,4);goto L8c03724c; /* mov.l @(0x10,r4),r3 */
L8c03724c: *stop=0x8c03724cu;if(!valid_ram(r[2]+0u,2u))return -3;r[0]=sx(m,r[2]+0x0u,2);goto L8c03724e; /* mov.w @(0x0,r2),r0 */
L8c03724e: *stop=0x8c03724eu;r[6]=sx(m,0x8c037276u,2);goto L8c037250; /* mov.w 0x8c037276,r6 */
L8c037250: *stop=0x8c037250u;r[0]&=r[6];goto L8c037252; /* and r6,r0 */
L8c037252: *stop=0x8c037252u;r[5]=20u;goto L8c037254; /* mov #0x14,r5 */
L8c037254: *stop=0x8c037254u;*macl=(r[0]&65535u)*(r[5]&65535u);goto L8c037256; /* mulu.w r0,r5 */
L8c037256: *stop=0x8c037256u;r[5]=*macl;goto L8c037258; /* sts MACL,r5 */
L8c037258: *stop=0x8c037258u;r[5]+=r[3];goto L8c03725a; /* add r3,r5 */
L8c03725a: *stop=0x8c03725au;r[0]=r[5];goto L8c03725c; /* mov r5,r0 */
L8c03725c: *stop=0x8c03725cu;if(!valid_ram(*gbr+0x34u,4u))return -3;wr(m,*gbr+0x34u,r[0],4);goto L8c03725e; /* mov.l r0,@(0x34,GBR) */
L8c03725e: *stop=0x8c03725eu;if(!valid_ram(r[5]+16u,4u))return -3;r[0]=rd(m,r[5]+0x10u,4);goto L8c037260; /* mov.l @(0x10,r5),r0 */
L8c037260: *stop=0x8c037260u;r[1]=rd(m,0x8c037290u,4);goto L8c037262; /* mov.l 0x8c037290,r1 */
L8c037262: *stop=0x8c037262u;if(!valid_ram(r[1],4u))return -3;r[6]=rd(m,r[1],4);r[1]+=4;goto L8c037264; /* mov.l @r1+,r6 */
L8c037264: *stop=0x8c037264u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c037266; /* tst r6,r6 */
L8c037266: if((*sr&1u))goto L8c037270;goto L8c037268; /* bt 0x8c037270 */
L8c037268: *stop=0x8c037268u;if(!valid_ram(r[1],4u))return -3;r[6]=rd(m,r[1],4);r[1]+=4;goto L8c03726a; /* mov.l @r1+,r6 */
L8c03726a: *stop=0x8c03726au;r[0]&=r[6];goto L8c03726c; /* and r6,r0 */
L8c03726c: *stop=0x8c03726cu;if(!valid_ram(r[1],4u))return -3;r[6]=rd(m,r[1],4);r[1]+=4;goto L8c03726e; /* mov.l @r1+,r6 */
L8c03726e: *stop=0x8c03726eu;r[0]|=r[6];goto L8c037270; /* or r6,r0 */
L8c037270: *stop=0x8c037270u;if(!valid_ram(*gbr+0x2cu,4u))return -3;wr(m,*gbr+0x2cu,r[0],4);goto L8c037272; /* mov.l r0,@(0x2c,GBR) */
L8c037272: *stop=0x8c037272u;if(*pr!=entry_pr)return -3;*stop=0x8c037274u;*sr&=~1u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_gbr_geometry_8c60a3f8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60a3f8;
L8c60a3f8: *stop=0x8c60a3f8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c60a3fa; /* mov.l r14,@-r15 */
L8c60a3fa: *stop=0x8c60a3fau;r[5]=28u;goto L8c60a3fc; /* mov #0x1c,r5 */
L8c60a3fc: *stop=0x8c60a3fcu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c60a3fe; /* mov.l r13,@-r15 */
L8c60a3fe: *stop=0x8c60a3feu;r[13]=rd(m,0x8c60a574u,4);goto L8c60a400; /* mov.l 0x8c60a574,r13 */
L8c60a400: *stop=0x8c60a400u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c60a402; /* mov.l r12,@-r15 */
L8c60a402: *stop=0x8c60a402u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c60a404; /* mov.l r11,@-r15 */
L8c60a404: *stop=0x8c60a404u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto L8c60a406; /* mov.l r8,@-r15 */
L8c60a406: *stop=0x8c60a406u;r[8]=rd(m,0x8c60a570u,4);goto L8c60a408; /* mov.l 0x8c60a570,r8 */
L8c60a408: *stop=0x8c60a408u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c60a40a; /* sts.l PR,@-r15 */
L8c60a40a: *stop=0x8c60a40au;if(!valid_ram(r[13],2u))return -3;r[11]=sx(m,r[13],2);goto L8c60a40c; /* mov.w @r13,r11 */
L8c60a40c: *stop=0x8c60a40cu;r[12]=rd(m,0x8c60a578u,4);goto L8c60a40e; /* mov.l 0x8c60a578,r12 */
L8c60a40e: *stop=0x8c60a40eu;*macl=(r[5]&65535u)*(r[11]&65535u);goto L8c60a410; /* mulu.w r5,r11 */
L8c60a410: *stop=0x8c60a410u;r[5]=*macl;goto L8c60a412; /* sts MACL,r5 */
L8c60a412: *stop=0x8c60a412u;r[5]+=r[8];goto L8c60a414; /* add r8,r5 */
L8c60a414: *stop=0x8c60a414u;if(!valid_ram(r[5]+20u,4u))return -3;r[14]=rd(m,r[5]+0x14u,4);goto L8c60a416; /* mov.l @(0x14,r5),r14 */
L8c60a416: *stop=0x8c60a416u;if(!valid_ram(r[5]+4u,4u))return -3;r[0]=rd(m,r[5]+0x4u,4);goto L8c60a418; /* mov.l @(0x4,r5),r0 */
L8c60a418: *stop=0x8c60a418u;r[14]<<=2;goto L8c60a41a; /* shll2 r14 */
L8c60a41a: *stop=0x8c60a41au;if(!valid_ram(r[0]+r[14],4u))return -3;r[14]=rd(m,r[14]+r[0],4);goto L8c60a41c; /* mov.l @(r0,r14),r14 */
L8c60a41c: *stop=0x8c60a41cu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c60a41e; /* tst r14,r14 */
L8c60a41e: if((*sr&1u))goto L8c60a49a;goto L8c60a420; /* bt 0x8c60a49a */
L8c60a420: *stop=0x8c60a420u;r[4]=28u;goto L8c60a422; /* mov #0x1c,r4 */
L8c60a422: *stop=0x8c60a422u;*macl=(r[4]&65535u)*(r[11]&65535u);goto L8c60a424; /* mulu.w r4,r11 */
L8c60a424: *stop=0x8c60a424u;r[5]=0u;goto L8c60a426; /* mov #0x0,r5 */
L8c60a426: *stop=0x8c60a426u;r[4]=*macl;goto L8c60a428; /* sts MACL,r4 */
L8c60a428: *stop=0x8c60a428u;r[4]+=r[8];goto L8c60a42a; /* add r8,r4 */
L8c60a42a: *stop=0x8c60a42au;if(!valid_ram(r[4]+20u,4u))return -3;r[6]=rd(m,r[4]+0x14u,4);goto L8c60a42c; /* mov.l @(0x14,r4),r6 */
L8c60a42c: *stop=0x8c60a42cu;if(!valid_ram(r[4]+8u,4u))return -3;r[0]=rd(m,r[4]+0x8u,4);goto L8c60a42e; /* mov.l @(0x8,r4),r0 */
L8c60a42e: *stop=0x8c60a42eu;r[6]<<=2;goto L8c60a430; /* shll2 r6 */
L8c60a430: *stop=0x8c60a430u;if(!valid_ram(r[0]+r[6],4u))return -3;r[6]=rd(m,r[6]+r[0],4);goto L8c60a432; /* mov.l @(r0,r6),r6 */
L8c60a432: *stop=0x8c60a432u;r[0]=12u;goto L8c60a434; /* mov #0xc,r0 */
L8c60a434: *stop=0x8c60a434u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[4]);goto L8c60a436; /* fmov.s @(r0,r4),fr4 */
L8c60a436: *stop=0x8c60a436u;r[0]=0x8c60a57cu;goto L8c60a438; /* mova 0x8c60a57c,r0 */
L8c60a438: *stop=0x8c60a438u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c60a43a; /* fmov.s @r0,fr3 */
L8c60a43a: *stop=0x8c60a43au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c60a43c; /* fcmp/gt fr3,fr4 */
L8c60a43c: take=!(*sr&1u);*stop=0x8c60a43eu;r[7]=r[6];if(take)goto L8c60a44a;goto L8c60a440; /* bf/s 0x8c60a44a */
L8c60a440: *stop=0x8c60a440u;r[0]=0x8c60a580u;goto L8c60a442; /* mova 0x8c60a580,r0 */
L8c60a442: *stop=0x8c60a442u;fcopy(f,3u,4u);goto L8c60a444; /* fmov fr4,fr3 */
L8c60a444: *stop=0x8c60a444u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]);goto L8c60a446; /* fmov.s @r0,fr2 */
L8c60a446: *stop=0x8c60a448u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c60a44c; /* bra 0x8c60a44c */
L8c60a44a: *stop=0x8c60a44au;fcopy(f,3u,4u);goto L8c60a44c; /* fmov fr4,fr3 */
L8c60a44c: *stop=0x8c60a44cu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c60a44e; /* ftrc fr3,FPUL */
L8c60a44e: *stop=0x8c60a44eu;r[1]=*fpul;goto L8c60a450; /* sts FPUL,r1 */
L8c60a450: *stop=0x8c60a452u;r[0]=1u;goto L8c60a46e; /* bra 0x8c60a46e */
L8c60a454: *stop=0x8c60a454u;r[4]=r[5];goto L8c60a456; /* mov r5,r4 */
L8c60a456: *stop=0x8c60a456u;r[4]+=r[7];goto L8c60a458; /* add r7,r4 */
L8c60a458: *stop=0x8c60a458u;*sr=(*sr&~1u)|(r[4]&1u);r[4]>>=1;goto L8c60a45a; /* shlr r4 */
L8c60a45a: *stop=0x8c60a45au;r[2]=r[4];goto L8c60a45c; /* mov r4,r2 */
L8c60a45c: *stop=0x8c60a45cu;r[2]<<=2;goto L8c60a45e; /* shll2 r2 */
L8c60a45e: *stop=0x8c60a45eu;r[2]<<=2;goto L8c60a460; /* shll2 r2 */
L8c60a460: *stop=0x8c60a460u;r[2]+=r[14];goto L8c60a462; /* add r14,r2 */
L8c60a462: *stop=0x8c60a462u;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c60a464; /* mov.l @r2,r3 */
L8c60a464: *stop=0x8c60a464u;*sr=(*sr&~1u)|(r[1]>=r[3]);goto L8c60a466; /* cmp/hs r3,r1 */
L8c60a466: if(!(*sr&1u))goto L8c60a46c;goto L8c60a468; /* bf 0x8c60a46c */
L8c60a468: *stop=0x8c60a46au;r[5]=r[4];goto L8c60a46e; /* bra 0x8c60a46e */
L8c60a46c: *stop=0x8c60a46cu;r[7]=r[4];goto L8c60a46e; /* mov r4,r7 */
L8c60a46e: *stop=0x8c60a46eu;r[2]=r[7];goto L8c60a470; /* mov r7,r2 */
L8c60a470: *stop=0x8c60a470u;r[2]-=r[5];goto L8c60a472; /* sub r5,r2 */
L8c60a472: *stop=0x8c60a472u;*sr=(*sr&~1u)|(r[2]>r[0]);goto L8c60a474; /* cmp/hi r0,r2 */
L8c60a474: if((*sr&1u))goto L8c60a454;goto L8c60a476; /* bt 0x8c60a454 */
L8c60a476: *stop=0x8c60a476u;r[0]=r[11]&65535u;goto L8c60a478; /* extu.w r11,r0 */
L8c60a478: *stop=0x8c60a478u;r[3]=rd(m,0x8c60a584u,4);goto L8c60a47a; /* mov.l 0x8c60a584,r3 */
L8c60a47a: *stop=0x8c60a47au;r[1]=r[0];goto L8c60a47c; /* mov r0,r1 */
L8c60a47c: *stop=0x8c60a47cu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60a47e; /* shll r0 */
L8c60a47e: *stop=0x8c60a47eu;r[0]+=r[1];goto L8c60a480; /* add r1,r0 */
L8c60a480: *stop=0x8c60a480u;if(!valid_ram(r[3],4u))return -3;r[2]=rd(m,r[3],4);goto L8c60a482; /* mov.l @r3,r2 */
L8c60a482: *stop=0x8c60a482u;r[0]<<=2;goto L8c60a484; /* shll2 r0 */
L8c60a484: *stop=0x8c60a484u;r[7]=r[0];goto L8c60a486; /* mov r0,r7 */
L8c60a486: *stop=0x8c60a486u;r[7]+=r[12];goto L8c60a488; /* add r12,r7 */
L8c60a488: target=r[2];*pr=0x8c60a48cu;*stop=0x8c60a48au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60a48c; /* jsr @r2 */
L8c60a48c: *stop=0x8c60a48cu;r[4]=rd(m,0x8c60a588u,4);goto L8c60a48e; /* mov.l 0x8c60a588,r4 */
L8c60a48e: *stop=0x8c60a48eu;if(!valid_ram(r[13],2u))return -3;r[3]=sx(m,r[13],2);goto L8c60a490; /* mov.w @r13,r3 */
L8c60a490: *stop=0x8c60a490u;if(!valid_ram(r[4],2u))return -3;r[2]=sx(m,r[4],2);goto L8c60a492; /* mov.w @r4,r2 */
L8c60a492: *stop=0x8c60a492u;r[3]+=1u;goto L8c60a494; /* add #0x1,r3 */
L8c60a494: *stop=0x8c60a494u;r[2]|=r[3];goto L8c60a496; /* or r3,r2 */
L8c60a496: *stop=0x8c60a498u;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[2],2);goto L8c60a4d8; /* bra 0x8c60a4d8 */
L8c60a49a: *stop=0x8c60a49au;r[0]=r[11]&65535u;goto L8c60a49c; /* extu.w r11,r0 */
L8c60a49c: *stop=0x8c60a49cu;r[1]=8u;goto L8c60a49e; /* mov #0x8,r1 */
L8c60a49e: *stop=0x8c60a49eu;r[3]=r[0];goto L8c60a4a0; /* mov r0,r3 */
L8c60a4a0: *stop=0x8c60a4a0u;r[1]+=r[4];goto L8c60a4a2; /* add r4,r1 */
L8c60a4a2: *stop=0x8c60a4a2u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60a4a4; /* shll r0 */
L8c60a4a4: *stop=0x8c60a4a4u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto L8c60a4a6; /* fmov.s @r1,fr3 */
L8c60a4a6: *stop=0x8c60a4a6u;r[0]+=r[3];goto L8c60a4a8; /* add r3,r0 */
L8c60a4a8: *stop=0x8c60a4a8u;r[0]<<=2;goto L8c60a4aa; /* shll2 r0 */
L8c60a4aa: *stop=0x8c60a4aau;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[12]);goto L8c60a4ac; /* fmov fr3,@(r0,r12) */
L8c60a4ac: *stop=0x8c60a4acu;r[1]=12u;goto L8c60a4ae; /* mov #0xc,r1 */
L8c60a4ae: *stop=0x8c60a4aeu;if(!valid_ram(r[13],2u))return -3;r[0]=sx(m,r[13],2);goto L8c60a4b0; /* mov.w @r13,r0 */
L8c60a4b0: *stop=0x8c60a4b0u;r[1]+=r[4];goto L8c60a4b2; /* add r4,r1 */
L8c60a4b2: *stop=0x8c60a4b2u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto L8c60a4b4; /* fmov.s @r1,fr3 */
L8c60a4b4: *stop=0x8c60a4b4u;r[1]=16u;goto L8c60a4b6; /* mov #0x10,r1 */
L8c60a4b6: *stop=0x8c60a4b6u;r[0]=r[0]&65535u;goto L8c60a4b8; /* extu.w r0,r0 */
L8c60a4b8: *stop=0x8c60a4b8u;r[3]=r[0];goto L8c60a4ba; /* mov r0,r3 */
L8c60a4ba: *stop=0x8c60a4bau;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60a4bc; /* shll r0 */
L8c60a4bc: *stop=0x8c60a4bcu;r[0]+=r[3];goto L8c60a4be; /* add r3,r0 */
L8c60a4be: *stop=0x8c60a4beu;r[0]+=1u;goto L8c60a4c0; /* add #0x1,r0 */
L8c60a4c0: *stop=0x8c60a4c0u;r[0]<<=2;goto L8c60a4c2; /* shll2 r0 */
L8c60a4c2: *stop=0x8c60a4c2u;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[12]);goto L8c60a4c4; /* fmov fr3,@(r0,r12) */
L8c60a4c4: *stop=0x8c60a4c4u;r[1]+=r[4];goto L8c60a4c6; /* add r4,r1 */
L8c60a4c6: *stop=0x8c60a4c6u;if(!valid_ram(r[13],2u))return -3;r[0]=sx(m,r[13],2);goto L8c60a4c8; /* mov.w @r13,r0 */
L8c60a4c8: *stop=0x8c60a4c8u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto L8c60a4ca; /* fmov.s @r1,fr3 */
L8c60a4ca: *stop=0x8c60a4cau;r[0]=r[0]&65535u;goto L8c60a4cc; /* extu.w r0,r0 */
L8c60a4cc: *stop=0x8c60a4ccu;r[3]=r[0];goto L8c60a4ce; /* mov r0,r3 */
L8c60a4ce: *stop=0x8c60a4ceu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60a4d0; /* shll r0 */
L8c60a4d0: *stop=0x8c60a4d0u;r[0]+=r[3];goto L8c60a4d2; /* add r3,r0 */
L8c60a4d2: *stop=0x8c60a4d2u;r[0]+=2u;goto L8c60a4d4; /* add #0x2,r0 */
L8c60a4d4: *stop=0x8c60a4d4u;r[0]<<=2;goto L8c60a4d6; /* shll2 r0 */
L8c60a4d6: *stop=0x8c60a4d6u;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[12]);goto L8c60a4d8; /* fmov fr3,@(r0,r12) */
L8c60a4d8: *stop=0x8c60a4d8u;if(!valid_ram(r[13],2u))return -3;r[2]=sx(m,r[13],2);goto L8c60a4da; /* mov.w @r13,r2 */
L8c60a4da: *stop=0x8c60a4dau;r[3]=28u;goto L8c60a4dc; /* mov #0x1c,r3 */
L8c60a4dc: *stop=0x8c60a4dcu;*macl=(r[3]&65535u)*(r[2]&65535u);goto L8c60a4de; /* mulu.w r3,r2 */
L8c60a4de: *stop=0x8c60a4deu;r[3]=*macl;goto L8c60a4e0; /* sts MACL,r3 */
L8c60a4e0: *stop=0x8c60a4e0u;r[3]+=r[8];goto L8c60a4e2; /* add r8,r3 */
L8c60a4e2: *stop=0x8c60a4e2u;if(!valid_ram(r[3]+20u,4u))return -3;r[2]=rd(m,r[3]+0x14u,4);goto L8c60a4e4; /* mov.l @(0x14,r3),r2 */
L8c60a4e4: *stop=0x8c60a4e4u;r[2]+=1u;goto L8c60a4e6; /* add #0x1,r2 */
L8c60a4e6: *stop=0x8c60a4e6u;if(!valid_ram(r[3]+20u,4u))return -3;wr(m,r[3]+0x14u,r[2],4);goto L8c60a4e8; /* mov.l r2,@(0x14,r3) */
L8c60a4e8: *stop=0x8c60a4e8u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c60a4ea; /* lds.l @r15+,PR */
L8c60a4ea: *stop=0x8c60a4eau;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c60a4ec; /* mov.l @r15+,r8 */
L8c60a4ec: *stop=0x8c60a4ecu;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c60a4ee; /* mov.l @r15+,r11 */
L8c60a4ee: *stop=0x8c60a4eeu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c60a4f0; /* mov.l @r15+,r12 */
L8c60a4f0: *stop=0x8c60a4f0u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c60a4f2; /* mov.l @r15+,r13 */
L8c60a4f2: *stop=0x8c60a4f2u;if(*pr!=entry_pr)return -3;*stop=0x8c60a4f4u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_gbr_geometry_8c64f31c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64f31c;
L8c64f31c: *stop=0x8c64f31cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f31e; /* fmov.s @r4+,fr4 */
L8c64f31e: *stop=0x8c64f31eu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f320; /* fmov.s @r4+,fr5 */
L8c64f320: *stop=0x8c64f320u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f322; /* fmov.s @r4+,fr6 */
L8c64f322: *stop=0x8c64f322u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c64f324; /* fldi1 fr7 */
L8c64f324: *stop=0x8c64f324u;status=sa_sh4_vector_numeric(0xf5fdu,f,*fpul);if(status)return status;goto L8c64f326; /* ftrv xmtrx,fv4 */
L8c64f326: *stop=0x8c64f326u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f328; /* fschg */
L8c64f328: *stop=0x8c64f328u;fcopy(f,13u,4u);goto L8c64f32a; /* fmov fr4,fr13 */
L8c64f32a: *stop=0x8c64f32au;fcopy(f,15u,6u);goto L8c64f32c; /* fmov fr6,fr15 */
L8c64f32c: *stop=0x8c64f32cu;if(*pr!=entry_pr)return -3;*stop=0x8c64f32eu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_gbr_geometry_8c64f244(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64f244;
L8c64f244: *stop=0x8c64f244u;if(!valid_ram(r[4],4u))return -3;r[1]=rd(m,r[4],4);r[4]+=4;goto L8c64f246; /* mov.l @r4+,r1 */
L8c64f246: *stop=0x8c64f246u;if(!valid_ram(r[4],4u))return -3;r[2]=rd(m,r[4],4);r[4]+=4;goto L8c64f248; /* mov.l @r4+,r2 */
L8c64f248: *stop=0x8c64f248u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c64f24a; /* cmp/pl r5 */
L8c64f24a: *stop=0x8c64f24au;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);r[4]+=4;goto L8c64f24c; /* mov.l @r4+,r3 */
L8c64f24c: if((*sr&1u))goto L8c64f27e;goto L8c64f24e; /* bt 0x8c64f27e */
L8c64f24e: *stop=0x8c64f24eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64f250; /* tst r3,r3 */
L8c64f250: take=(*sr&1u);*stop=0x8c64f252u;*fpul=r[3];if(take)goto L8c64f27a;goto L8c64f254; /* bt/s 0x8c64f27a */
L8c64f254: *stop=0x8c64f254u;status=sa_sh4_vector_numeric(0xf0fdu,f,*fpul);if(status)return status;goto L8c64f256; /* fsca FPUL,dr0 */
L8c64f256: *stop=0x8c64f256u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c64f258; /* fldi0 fr3 */
L8c64f258: *stop=0x8c64f258u;fcopy(f,2u,0u);goto L8c64f25a; /* fmov fr0,fr2 */
L8c64f25a: *stop=0x8c64f25au;fcopy(f,0u,1u);goto L8c64f25c; /* fmov fr1,fr0 */
L8c64f25c: *stop=0x8c64f25cu;fcopy(f,1u,2u);goto L8c64f25e; /* fmov fr2,fr1 */
L8c64f25e: *stop=0x8c64f25eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c64f260; /* fldi0 fr2 */
L8c64f260: *stop=0x8c64f260u;fcopy(f,5u,0u);goto L8c64f262; /* fmov fr0,fr5 */
L8c64f262: *stop=0x8c64f262u;fcopy(f,4u,1u);goto L8c64f264; /* fmov fr1,fr4 */
L8c64f264: *stop=0x8c64f264u;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c64f266; /* ftrv xmtrx,fv0 */
L8c64f266: *stop=0x8c64f266u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c64f268; /* fneg fr4 */
L8c64f268: *stop=0x8c64f268u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0;goto L8c64f26a; /* fldi0 fr6 */
L8c64f26a: *stop=0x8c64f26au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0;goto L8c64f26c; /* fldi0 fr7 */
L8c64f26c: *stop=0x8c64f26cu;status=sa_sh4_vector_numeric(0xf5fdu,f,*fpul);if(status)return status;goto L8c64f26e; /* ftrv xmtrx,fv4 */
L8c64f26e: *stop=0x8c64f26eu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f270; /* fschg */
L8c64f270: *stop=0x8c64f270u;fcopy(f,1u,0u);goto L8c64f272; /* fmov fr0,fr1 */
L8c64f272: *stop=0x8c64f272u;fcopy(f,3u,2u);goto L8c64f274; /* fmov fr2,fr3 */
L8c64f274: *stop=0x8c64f274u;fcopy(f,5u,4u);goto L8c64f276; /* fmov fr4,fr5 */
L8c64f276: *stop=0x8c64f276u;fcopy(f,7u,6u);goto L8c64f278; /* fmov fr6,fr7 */
L8c64f278: *stop=0x8c64f278u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f27a; /* fschg */
L8c64f27a: *stop=0x8c64f27au;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c64f27c; /* cmp/pl r5 */
L8c64f27c: if((*sr&1u))goto L8c64f2d4;goto L8c64f27e; /* bt 0x8c64f2d4 */
L8c64f27e: *stop=0x8c64f27eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64f280; /* tst r2,r2 */
L8c64f280: take=(*sr&1u);*stop=0x8c64f282u;*fpul=r[2];if(take)goto L8c64f2a8;goto L8c64f284; /* bt/s 0x8c64f2a8 */
L8c64f284: *stop=0x8c64f284u;status=sa_sh4_vector_numeric(0xf0fdu,f,*fpul);if(status)return status;goto L8c64f286; /* fsca FPUL,dr0 */
L8c64f286: *stop=0x8c64f286u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c64f288; /* fldi0 fr3 */
L8c64f288: *stop=0x8c64f288u;fcopy(f,2u,0u);goto L8c64f28a; /* fmov fr0,fr2 */
L8c64f28a: *stop=0x8c64f28au;fcopy(f,0u,1u);goto L8c64f28c; /* fmov fr1,fr0 */
L8c64f28c: *stop=0x8c64f28cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0;goto L8c64f28e; /* fldi0 fr1 */
L8c64f28e: *stop=0x8c64f28eu;fcopy(f,6u,0u);goto L8c64f290; /* fmov fr0,fr6 */
L8c64f290: *stop=0x8c64f290u;fcopy(f,4u,2u);goto L8c64f292; /* fmov fr2,fr4 */
L8c64f292: *stop=0x8c64f292u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c64f294; /* fneg fr2 */
L8c64f294: *stop=0x8c64f294u;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c64f296; /* ftrv xmtrx,fv0 */
L8c64f296: *stop=0x8c64f296u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0;goto L8c64f298; /* fldi0 fr5 */
L8c64f298: *stop=0x8c64f298u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0;goto L8c64f29a; /* fldi0 fr7 */
L8c64f29a: *stop=0x8c64f29au;status=sa_sh4_vector_numeric(0xf5fdu,f,*fpul);if(status)return status;goto L8c64f29c; /* ftrv xmtrx,fv4 */
L8c64f29c: *stop=0x8c64f29cu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f29e; /* fschg */
L8c64f29e: *stop=0x8c64f29eu;fcopy(f,1u,0u);goto L8c64f2a0; /* fmov fr0,fr1 */
L8c64f2a0: *stop=0x8c64f2a0u;fcopy(f,3u,2u);goto L8c64f2a2; /* fmov fr2,fr3 */
L8c64f2a2: *stop=0x8c64f2a2u;fcopy(f,9u,4u);goto L8c64f2a4; /* fmov fr4,fr9 */
L8c64f2a4: *stop=0x8c64f2a4u;fcopy(f,11u,6u);goto L8c64f2a6; /* fmov fr6,fr11 */
L8c64f2a6: *stop=0x8c64f2a6u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f2a8; /* fschg */
L8c64f2a8: *stop=0x8c64f2a8u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c64f2aa; /* tst r1,r1 */
L8c64f2aa: take=(*sr&1u);*stop=0x8c64f2acu;*fpul=r[1];if(take)goto L8c64f2d0;goto L8c64f2ae; /* bt/s 0x8c64f2d0 */
L8c64f2ae: *stop=0x8c64f2aeu;status=sa_sh4_vector_numeric(0xf0fdu,f,*fpul);if(status)return status;goto L8c64f2b0; /* fsca FPUL,dr0 */
L8c64f2b0: *stop=0x8c64f2b0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c64f2b2; /* fldi0 fr3 */
L8c64f2b2: *stop=0x8c64f2b2u;fcopy(f,2u,0u);goto L8c64f2b4; /* fmov fr0,fr2 */
L8c64f2b4: *stop=0x8c64f2b4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0;goto L8c64f2b6; /* fldi0 fr0 */
L8c64f2b6: *stop=0x8c64f2b6u;fcopy(f,5u,2u);goto L8c64f2b8; /* fmov fr2,fr5 */
L8c64f2b8: *stop=0x8c64f2b8u;fcopy(f,6u,1u);goto L8c64f2ba; /* fmov fr1,fr6 */
L8c64f2ba: *stop=0x8c64f2bau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]^=0x80000000u;goto L8c64f2bc; /* fneg fr5 */
L8c64f2bc: *stop=0x8c64f2bcu;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c64f2be; /* ftrv xmtrx,fv0 */
L8c64f2be: *stop=0x8c64f2beu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto L8c64f2c0; /* fldi0 fr4 */
L8c64f2c0: *stop=0x8c64f2c0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0;goto L8c64f2c2; /* fldi0 fr7 */
L8c64f2c2: *stop=0x8c64f2c2u;status=sa_sh4_vector_numeric(0xf5fdu,f,*fpul);if(status)return status;goto L8c64f2c4; /* ftrv xmtrx,fv4 */
L8c64f2c4: *stop=0x8c64f2c4u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f2c6; /* fschg */
L8c64f2c6: *stop=0x8c64f2c6u;fcopy(f,5u,0u);goto L8c64f2c8; /* fmov fr0,fr5 */
L8c64f2c8: *stop=0x8c64f2c8u;fcopy(f,7u,2u);goto L8c64f2ca; /* fmov fr2,fr7 */
L8c64f2ca: *stop=0x8c64f2cau;fcopy(f,9u,4u);goto L8c64f2cc; /* fmov fr4,fr9 */
L8c64f2cc: *stop=0x8c64f2ccu;fcopy(f,11u,6u);goto L8c64f2ce; /* fmov fr6,fr11 */
L8c64f2ce: *stop=0x8c64f2ceu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f2d0; /* fschg */
L8c64f2d0: *stop=0x8c64f2d0u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c64f2d2; /* cmp/pl r5 */
L8c64f2d2: if((*sr&1u))goto L8c64f24e;goto L8c64f2d4; /* bt 0x8c64f24e */
L8c64f2d4: *stop=0x8c64f2d4u;if(*pr!=entry_pr)return -3;*stop=0x8c64f2d6u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_gbr_geometry_8c64f2dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64f2dc;
L8c64f2dc: *stop=0x8c64f2dcu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f2de; /* fmov.s @r4+,fr4 */
L8c64f2de: *stop=0x8c64f2deu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f2e0; /* fmov.s @r4+,fr5 */
L8c64f2e0: *stop=0x8c64f2e0u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f2e2; /* fmov.s @r4+,fr6 */
L8c64f2e2: *stop=0x8c64f2e2u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f2e4; /* fschg */
L8c64f2e4: *stop=0x8c64f2e4u;fcopy(f,0u,1u);goto L8c64f2e6; /* fmov fr1,fr0 */
L8c64f2e6: *stop=0x8c64f2e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c64f2e8; /* fmul fr4,fr0 */
L8c64f2e8: *stop=0x8c64f2e8u;fcopy(f,2u,3u);goto L8c64f2ea; /* fmov fr3,fr2 */
L8c64f2ea: *stop=0x8c64f2eau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64f2ec; /* fmul fr4,fr1 */
L8c64f2ec: *stop=0x8c64f2ecu;fcopy(f,8u,5u);goto L8c64f2ee; /* fmov fr5,fr8 */
L8c64f2ee: *stop=0x8c64f2eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64f2f0; /* fmul fr4,fr2 */
L8c64f2f0: *stop=0x8c64f2f0u;fcopy(f,10u,7u);goto L8c64f2f2; /* fmov fr7,fr10 */
L8c64f2f2: *stop=0x8c64f2f2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f2f4; /* fmul fr4,fr3 */
L8c64f2f4: *stop=0x8c64f2f4u;fcopy(f,1u,0u);goto L8c64f2f6; /* fmov fr0,fr1 */
L8c64f2f6: *stop=0x8c64f2f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c64f2f8; /* fmul fr5,fr8 */
L8c64f2f8: *stop=0x8c64f2f8u;fcopy(f,3u,2u);goto L8c64f2fa; /* fmov fr2,fr3 */
L8c64f2fa: *stop=0x8c64f2fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[5],&f->fpscr,&fr[9]);if(status)return status;}goto L8c64f2fc; /* fmul fr5,fr9 */
L8c64f2fc: *stop=0x8c64f2fcu;fcopy(f,0u,9u);goto L8c64f2fe; /* fmov fr9,fr0 */
L8c64f2fe: *stop=0x8c64f2feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[5],&f->fpscr,&fr[10]);if(status)return status;}goto L8c64f300; /* fmul fr5,fr10 */
L8c64f300: *stop=0x8c64f300u;fcopy(f,2u,11u);goto L8c64f302; /* fmov fr11,fr2 */
L8c64f302: *stop=0x8c64f302u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[5],&f->fpscr,&fr[11]);if(status)return status;}goto L8c64f304; /* fmul fr5,fr11 */
L8c64f304: *stop=0x8c64f304u;fcopy(f,5u,8u);goto L8c64f306; /* fmov fr8,fr5 */
L8c64f306: *stop=0x8c64f306u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[6],&f->fpscr,&fr[0]);if(status)return status;}goto L8c64f308; /* fmul fr6,fr0 */
L8c64f308: *stop=0x8c64f308u;fcopy(f,7u,10u);goto L8c64f30a; /* fmov fr10,fr7 */
L8c64f30a: *stop=0x8c64f30au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64f30c; /* fmul fr6,fr1 */
L8c64f30c: *stop=0x8c64f30cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64f30e; /* fmul fr6,fr2 */
L8c64f30e: *stop=0x8c64f30eu;fcopy(f,9u,0u);goto L8c64f310; /* fmov fr0,fr9 */
L8c64f310: *stop=0x8c64f310u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f312; /* fmul fr6,fr3 */
L8c64f312: *stop=0x8c64f312u;fcopy(f,11u,2u);goto L8c64f314; /* fmov fr2,fr11 */
L8c64f314: *stop=0x8c64f314u;if(*pr!=entry_pr)return -3;*stop=0x8c64f316u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_gbr_geometry_8c60b3e2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60b3e2;
L8c60b3e2: *stop=0x8c60b3e2u;r[6]=rd(m,0x8c60b4fcu,4);goto L8c60b3e4; /* mov.l 0x8c60b4fc,r6 */
L8c60b3e4: *stop=0x8c60b3e4u;r[1]=8u;goto L8c60b3e6; /* mov #0x8,r1 */
L8c60b3e6: *stop=0x8c60b3e6u;r[1]+=r[4];goto L8c60b3e8; /* add r4,r1 */
L8c60b3e8: *stop=0x8c60b3e8u;r[5]=rd(m,0x8c60b500u,4);goto L8c60b3ea; /* mov.l 0x8c60b500,r5 */
L8c60b3ea: *stop=0x8c60b3eau;if(!valid_ram(r[6],2u))return -3;r[0]=sx(m,r[6],2);goto L8c60b3ec; /* mov.w @r6,r0 */
L8c60b3ec: *stop=0x8c60b3ecu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto L8c60b3ee; /* fmov.s @r1,fr3 */
L8c60b3ee: *stop=0x8c60b3eeu;r[1]=12u;goto L8c60b3f0; /* mov #0xc,r1 */
L8c60b3f0: *stop=0x8c60b3f0u;r[0]=r[0]&65535u;goto L8c60b3f2; /* extu.w r0,r0 */
L8c60b3f2: *stop=0x8c60b3f2u;r[3]=r[0];goto L8c60b3f4; /* mov r0,r3 */
L8c60b3f4: *stop=0x8c60b3f4u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60b3f6; /* shll r0 */
L8c60b3f6: *stop=0x8c60b3f6u;r[0]+=r[3];goto L8c60b3f8; /* add r3,r0 */
L8c60b3f8: *stop=0x8c60b3f8u;r[0]<<=2;goto L8c60b3fa; /* shll2 r0 */
L8c60b3fa: *stop=0x8c60b3fau;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);goto L8c60b3fc; /* fmov fr3,@(r0,r5) */
L8c60b3fc: *stop=0x8c60b3fcu;r[1]+=r[4];goto L8c60b3fe; /* add r4,r1 */
L8c60b3fe: *stop=0x8c60b3feu;if(!valid_ram(r[6],2u))return -3;r[0]=sx(m,r[6],2);goto L8c60b400; /* mov.w @r6,r0 */
L8c60b400: *stop=0x8c60b400u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto L8c60b402; /* fmov.s @r1,fr3 */
L8c60b402: *stop=0x8c60b402u;r[1]=16u;goto L8c60b404; /* mov #0x10,r1 */
L8c60b404: *stop=0x8c60b404u;r[0]=r[0]&65535u;goto L8c60b406; /* extu.w r0,r0 */
L8c60b406: *stop=0x8c60b406u;r[3]=r[0];goto L8c60b408; /* mov r0,r3 */
L8c60b408: *stop=0x8c60b408u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60b40a; /* shll r0 */
L8c60b40a: *stop=0x8c60b40au;r[0]+=r[3];goto L8c60b40c; /* add r3,r0 */
L8c60b40c: *stop=0x8c60b40cu;r[0]+=1u;goto L8c60b40e; /* add #0x1,r0 */
L8c60b40e: *stop=0x8c60b40eu;r[0]<<=2;goto L8c60b410; /* shll2 r0 */
L8c60b410: *stop=0x8c60b410u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);goto L8c60b412; /* fmov fr3,@(r0,r5) */
L8c60b412: *stop=0x8c60b412u;r[1]+=r[4];goto L8c60b414; /* add r4,r1 */
L8c60b414: *stop=0x8c60b414u;if(!valid_ram(r[6],2u))return -3;r[0]=sx(m,r[6],2);goto L8c60b416; /* mov.w @r6,r0 */
L8c60b416: *stop=0x8c60b416u;r[0]=r[0]&65535u;goto L8c60b418; /* extu.w r0,r0 */
L8c60b418: *stop=0x8c60b418u;r[3]=r[0];goto L8c60b41a; /* mov r0,r3 */
L8c60b41a: *stop=0x8c60b41au;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60b41c; /* shll r0 */
L8c60b41c: *stop=0x8c60b41cu;r[0]+=r[3];goto L8c60b41e; /* add r3,r0 */
L8c60b41e: *stop=0x8c60b41eu;r[0]+=2u;goto L8c60b420; /* add #0x2,r0 */
L8c60b420: *stop=0x8c60b420u;r[0]<<=2;goto L8c60b422; /* shll2 r0 */
L8c60b422: *stop=0x8c60b422u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto L8c60b424; /* fmov.s @r1,fr3 */
L8c60b424: *stop=0x8c60b424u;if(*pr!=entry_pr)return -3;*stop=0x8c60b426u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);return 0; /* rts */
}
