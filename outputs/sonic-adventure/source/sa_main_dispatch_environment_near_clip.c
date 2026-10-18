/* Environment-map triangle near-plane clipper with portable queue interface; name inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "vertex_bus.h"
#include "sh4_vector_numeric.h"
#include "sh4_fmac.h"
#include "sh4_divide.h"
#include "sh4_add_sub.h"
#include "sh4_multiply_extended.h"
#include "sh4_float_i32.h"
#include "sh4_compare_bits.h"
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
typedef int (*sa_shared_service)(void *,uint32_t,uint8_t *,
    struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,uint32_t a,unsigned n){
 if((a&0x1f000000u)!=0x0c000000u){uint32_t value=0;if(!bus||!bus->read){*io_status=-2;return 0;}*io_status=bus->read(bus->context,a,n,&value);return value;}
 uint32_t value=0;for(unsigned i=0;i<n;i++)value|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return value;
}
static uint32_t sx(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,uint32_t a,unsigned n){
 uint32_t value=rd(m,bus,io_status,a,n);return n==1?(uint32_t)(int32_t)(int8_t)value:(uint32_t)(int32_t)(int16_t)value;
}
static void wr(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,uint32_t a,uint32_t value,unsigned n){
 if((a&0x1f000000u)!=0x0c000000u){if(!bus||!bus->write){*io_status=-2;return;}*io_status=bus->write(bus->context,a,value,n);return;}
 for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(value>>(8*i));
}
static void fload(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  f->bank[bank][reg]=rd(m,bus,io_status,a,4);f->bank[bank][reg+1]=rd(m,bus,io_status,a+4u,4);
 }else f->bank[bank][reg]=rd(m,bus,io_status,a,4);
}
static void fstore(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  wr(m,bus,io_status,a,f->bank[bank][reg],4);wr(m,bus,io_status,a+4u,f->bank[bank][reg+1],4);
 }else wr(m,bus,io_status,a,f->bank[bank][reg],4);
}
static void fcopy(struct sa_fpu_banks *f,unsigned n,unsigned s){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){unsigned nb=bank^(n&1u),sb=bank^(s&1u);n&=~1u;s&=~1u;
  uint32_t a=f->bank[sb][s],b=f->bank[sb][s+1];f->bank[nb][n]=a;f->bank[nb][n+1]=b;
 }else f->bank[bank][n]=f->bank[bank][s];
}
SA_STAGE_EXPORT int sa_environment_triangle_near_clip_8c611f24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c611f24;
L8c611f24: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c611f26; /* mov.l r14,@-r15 */
L8c611f26: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c611f28; /* mov.l r13,@-r15 */
L8c611f28: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c611f2a; /* mov.l r12,@-r15 */
L8c611f2a: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c611f2c; /* mov.l r11,@-r15 */
L8c611f2c: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c611f2e; /* mov.l r10,@-r15 */
L8c611f2e: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c611f30; /* mov.l r9,@-r15 */
L8c611f30: r[15]+=4294967212u;goto L8c611f32; /* add #-0x54,r15 */
L8c611f32: r[3]=rd(m,bus,&io_status,0x8c61205cu,4);if(io_status)return io_status;goto L8c611f34; /* mov.l 0x8c61205c,r3 */
L8c611f34: r[7]=r[15];goto L8c611f36; /* mov r15,r7 */
L8c611f36: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c611f38; /* fldi1 fr7 */
L8c611f38: r[7]+=72u;goto L8c611f3a; /* add #0x48,r7 */
L8c611f3a: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c611f3c; /* fmov.s @r3,fr3 */
L8c611f3c: r[9]=r[15];goto L8c611f3e; /* mov r15,r9 */
L8c611f3e: fcopy(f,5u,7u);goto L8c611f40; /* fmov fr7,fr5 */
L8c611f40: r[2]=r[7];goto L8c611f42; /* mov r7,r2 */
L8c611f42: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c611f44; /* fdiv fr3,fr5 */
L8c611f44: r[9]+=36u;goto L8c611f46; /* add #0x24,r9 */
L8c611f46: r[0]=0x8c612064u;goto L8c611f48; /* mova 0x8c612064,r0 */
L8c611f48: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c611f4a; /* mov.l r2,@(0x4,r15) */
L8c611f4a: wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c611f4c; /* mov.l r4,@r2 */
L8c611f4c: r[11]=r[15];goto L8c611f4e; /* mov r15,r11 */
L8c611f4e: wr(m,bus,&io_status,r[7]+0x4u,r[5],4);if(io_status)return io_status;goto L8c611f50; /* mov.l r5,@(0x4,r7) */
L8c611f50: r[14]=r[9];goto L8c611f52; /* mov r9,r14 */
L8c611f52: r[12]=rd(m,bus,&io_status,0x8c612060u,4);if(io_status)return io_status;goto L8c611f54; /* mov.l 0x8c612060,r12 */
L8c611f54: r[2]=0u;goto L8c611f56; /* mov #0x0,r2 */
L8c611f56: r[5]=rd(m,bus,&io_status,0x8c612068u,4);if(io_status)return io_status;goto L8c611f58; /* mov.l 0x8c612068,r5 */
L8c611f58: r[4]=1u;goto L8c611f5a; /* mov #0x1,r4 */
L8c611f5a: wr(m,bus,&io_status,r[7]+0x8u,r[6],4);if(io_status)return io_status;goto L8c611f5c; /* mov.l r6,@(0x8,r7) */
L8c611f5c: r[11]+=24u;goto L8c611f5e; /* add #0x18,r11 */
L8c611f5e: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c611f60; /* mov.l r2,@r15 */
L8c611f60: r[13]=r[14];goto L8c611f62; /* mov r14,r13 */
L8c611f62: r[10]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611f64; /* mov.l @(0x4,r15),r10 */
L8c611f64: wr(m,bus,&io_status,r[15]+0x8u,r[14],4);if(io_status)return io_status;goto L8c611f66; /* mov.l r14,@(0x8,r15) */
L8c611f66: fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c611f68; /* fmov.s @r0,fr4 */
L8c611f68: r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611f6a; /* mov.l @r15,r6 */
L8c611f6a: r[0]=4u;goto L8c611f6c; /* mov #0x4,r0 */
L8c611f6c: r[3]=rd(m,bus,&io_status,0x8c61206cu,4);if(io_status)return io_status;goto L8c611f6e; /* mov.l 0x8c61206c,r3 */
L8c611f6e: r[6]<<=2;goto L8c611f70; /* shll2 r6 */
L8c611f70: r[2]=rd(m,bus,&io_status,0x8c612070u,4);if(io_status)return io_status;goto L8c611f72; /* mov.l 0x8c612070,r2 */
L8c611f72: wr(m,bus,&io_status,r[15]+0x10u,r[6],4);if(io_status)return io_status;goto L8c611f74; /* mov.l r6,@(0x10,r15) */
L8c611f74: r[6]+=r[7];goto L8c611f76; /* add r7,r6 */
L8c611f76: wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c611f78; /* mov.l r6,@(0x4,r15) */
L8c611f78: r[6]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c611f7a; /* mov.l @r6,r6 */
L8c611f7a: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c611f7c; /* fmov.s @r3,fr2 */
L8c611f7c: r[3]+=8u;goto L8c611f7e; /* add #0x8,r3 */
L8c611f7e: fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c611f80; /* fmov.s @r6,fr3 */
L8c611f80: r[1]=rd(m,bus,&io_status,0x8c612074u,4);if(io_status)return io_status;goto L8c611f82; /* mov.l 0x8c612074,r1 */
L8c611f82: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611f84; /* fmul fr3,fr2 */
L8c611f84: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c611f86; /* fmov.s @r2,fr3 */
L8c611f86: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c611f88; /* fmov.s @r1,fr1 */
L8c611f88: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611f8a; /* fsub fr2,fr3 */
L8c611f8a: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c611f8c; /* fmov.s @(r0,r6),fr2 */
L8c611f8c: r[0]=8u;goto L8c611f8e; /* mov #0x8,r0 */
L8c611f8e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c611f90; /* fmul fr2,fr1 */
L8c611f90: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c611f92; /* fmov.s @(r0,r6),fr2 */
L8c611f92: r[0]=28u;goto L8c611f94; /* mov #0x1c,r0 */
L8c611f94: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611f96; /* fsub fr1,fr3 */
L8c611f96: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c611f98; /* fmov.s @r3,fr1 */
L8c611f98: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c611f9a; /* fmul fr2,fr1 */
L8c611f9a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611f9c; /* fsub fr1,fr3 */
L8c611f9c: fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c611f9e; /* fmov fr3,@(r0,r6) */
L8c611f9e: r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c611fa0; /* mov.l @(0x10,r15),r2 */
L8c611fa0: r[6]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611fa2; /* mov.l @(0x4,r15),r6 */
L8c611fa2: r[2]+=r[11];goto L8c611fa4; /* add r11,r2 */
L8c611fa4: r[6]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c611fa6; /* mov.l @r6,r6 */
L8c611fa6: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c611fa8; /* mov.l r2,@(0xc,r15) */
L8c611fa8: r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c611faa; /* mov.l @r5,r3 */
L8c611faa: r[0]=r[3];goto L8c611fac; /* mov r3,r0 */
L8c611fac: fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c611fae; /* fmov.s @r0,fr2 */
L8c611fae: r[0]=4u;goto L8c611fb0; /* mov #0x4,r0 */
L8c611fb0: r[1]=r[3];goto L8c611fb2; /* mov r3,r1 */
L8c611fb2: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c611fb4; /* fmov.s @(r0,r6),fr0 */
L8c611fb4: r[0]=16u;goto L8c611fb6; /* mov #0x10,r0 */
L8c611fb6: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c611fb8; /* fmov.s @(r0,r1),fr3 */
L8c611fb8: r[0]=8u;goto L8c611fba; /* mov #0x8,r0 */
L8c611fba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611fbc; /* fmul fr0,fr3 */
L8c611fbc: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c611fbe; /* fmov.s @r6,fr0 */
L8c611fbe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611fc0; /* fmac fr0,fr2,fr3 */
L8c611fc0: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c611fc2; /* fmov.s @(r0,r6),fr0 */
L8c611fc2: r[0]=32u;goto L8c611fc4; /* mov #0x20,r0 */
L8c611fc4: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c611fc6; /* fmov.s @(r0,r3),fr2 */
L8c611fc6: r[0]=36u;goto L8c611fc8; /* mov #0x24,r0 */
L8c611fc8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611fca; /* fmac fr0,fr2,fr3 */
L8c611fca: fcopy(f,0u,4u);goto L8c611fcc; /* fmov fr4,fr0 */
L8c611fcc: fcopy(f,2u,4u);goto L8c611fce; /* fmov fr4,fr2 */
L8c611fce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611fd0; /* fmac fr0,fr3,fr2 */
L8c611fd0: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c611fd2; /* ftrc fr2,FPUL */
L8c611fd2: r[3]=*fpul;goto L8c611fd4; /* sts FPUL,r3 */
L8c611fd4: wr(m,bus,&io_status,r[2],r[3],2);if(io_status)return io_status;goto L8c611fd6; /* mov.w r3,@r2 */
L8c611fd6: r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c611fd8; /* mov.l @r5,r3 */
L8c611fd8: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c611fda; /* mov.l @(0xc,r15),r2 */
L8c611fda: fload(m,bus,&io_status,f,1u,r[0]+r[3]);if(io_status)return io_status;goto L8c611fdc; /* fmov.s @(r0,r3),fr1 */
L8c611fdc: r[0]=4u;goto L8c611fde; /* mov #0x4,r0 */
L8c611fde: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c611fe0; /* fmov.s @(r0,r6),fr0 */
L8c611fe0: r[0]=20u;goto L8c611fe2; /* mov #0x14,r0 */
L8c611fe2: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c611fe4; /* fmov.s @(r0,r3),fr3 */
L8c611fe4: r[1]=r[3];goto L8c611fe6; /* mov r3,r1 */
L8c611fe6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611fe8; /* fmul fr0,fr3 */
L8c611fe8: r[0]=4u;goto L8c611fea; /* mov #0x4,r0 */
L8c611fea: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c611fec; /* fmov.s @r6,fr0 */
L8c611fec: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c611fee; /* fmov.s @(r0,r1),fr2 */
L8c611fee: r[0]=8u;goto L8c611ff0; /* mov #0x8,r0 */
L8c611ff0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611ff2; /* fmac fr0,fr2,fr3 */
L8c611ff2: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c611ff4; /* fmov.s @(r0,r6),fr0 */
L8c611ff4: fcopy(f,2u,4u);goto L8c611ff6; /* fmov fr4,fr2 */
L8c611ff6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611ff8; /* fmac fr0,fr1,fr3 */
L8c611ff8: fcopy(f,0u,4u);goto L8c611ffa; /* fmov fr4,fr0 */
L8c611ffa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611ffc; /* fmac fr0,fr3,fr2 */
L8c611ffc: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c611ffe; /* ftrc fr2,FPUL */
L8c611ffe: r[0]=*fpul;goto L8c612000; /* sts FPUL,r0 */
L8c612000: wr(m,bus,&io_status,r[2]+0x2u,r[0],2);if(io_status)return io_status;goto L8c612002; /* mov.w r0,@(0x2,r2) */
L8c612002: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612004; /* mov.l @(0x4,r15),r3 */
L8c612004: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c612006; /* mov.l @r3,r2 */
L8c612006: r[1]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612008; /* mov.l @(0xc,r2),r1 */
L8c612008: *sr=(*sr&~1u)|!(r[4]&r[1]);goto L8c61200a; /* tst r4,r1 */
L8c61200a: if(!(*sr&1u))goto L8c612080;goto L8c61200c; /* bf 0x8c612080 */
L8c61200c: r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61200e; /* mov.l @r15,r6 */
L8c61200e: r[0]=24u;goto L8c612010; /* mov #0x18,r0 */
L8c612010: fcopy(f,6u,7u);goto L8c612012; /* fmov fr7,fr6 */
L8c612012: r[6]<<=2;goto L8c612014; /* shll2 r6 */
L8c612014: r[3]=rd(m,bus,&io_status,0x8c612078u,4);if(io_status)return io_status;goto L8c612016; /* mov.l 0x8c612078,r3 */
L8c612016: r[6]+=r[7];goto L8c612018; /* add r7,r6 */
L8c612018: wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c61201a; /* mov.l r6,@(0x4,r15) */
L8c61201a: r[6]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c61201c; /* mov.l @r6,r6 */
L8c61201c: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61201e; /* fmov.s @(r0,r6),fr3 */
L8c61201e: r[0]=12u;goto L8c612020; /* mov #0xc,r0 */
L8c612020: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[3],&f->fpscr,&fr[6]);if(status)return status;}goto L8c612022; /* fdiv fr3,fr6 */
L8c612022: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c612024; /* fmov.s @(r0,r12),fr3 */
L8c612024: r[0]=16u;goto L8c612026; /* mov #0x10,r0 */
L8c612026: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c612028; /* fmov.s @(r0,r6),fr2 */
L8c612028: r[0]=16u;goto L8c61202a; /* mov #0x10,r0 */
L8c61202a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61202c; /* fsub fr3,fr2 */
L8c61202c: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61202e; /* fmov.s @r3,fr3 */
L8c61202e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612030; /* fmul fr6,fr2 */
L8c612030: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612032; /* fdiv fr3,fr2 */
L8c612032: fstore(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c612034; /* fmov.s fr2,@r13 */
L8c612034: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612036; /* mov.l @(0x4,r15),r2 */
L8c612036: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c612038; /* fmov.s @(r0,r12),fr3 */
L8c612038: r[0]=20u;goto L8c61203a; /* mov #0x14,r0 */
L8c61203a: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61203c; /* mov.l @r2,r1 */
L8c61203c: r[2]=rd(m,bus,&io_status,0x8c61207cu,4);if(io_status)return io_status;goto L8c61203e; /* mov.l 0x8c61207c,r2 */
L8c61203e: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c612040; /* fmov.s @(r0,r1),fr2 */
L8c612040: r[0]=4u;goto L8c612042; /* mov #0x4,r0 */
L8c612042: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612044; /* fsub fr3,fr2 */
L8c612044: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c612046; /* fmov.s @r2,fr3 */
L8c612046: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612048; /* fmul fr6,fr2 */
L8c612048: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61204a; /* fdiv fr3,fr2 */
L8c61204a: fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61204c; /* fmov fr2,@(r0,r13) */
L8c61204c: r[0]=8u;goto L8c61204e; /* mov #0x8,r0 */
L8c61204e: fstore(m,bus,&io_status,f,6u,r[0]+r[13]);if(io_status)return io_status;goto L8c61209c; /* bra 0x8c61209c */
L8c612080: r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c612082; /* mov.l @r10,r3 */
L8c612082: r[0]=16u;goto L8c612084; /* mov #0x10,r0 */
L8c612084: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612086; /* fmov.s @(r0,r3),fr3 */
L8c612086: r[0]=20u;goto L8c612088; /* mov #0x14,r0 */
L8c612088: fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61208a; /* fmov.s fr3,@r14 */
L8c61208a: r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c61208c; /* mov.l @r10,r3 */
L8c61208c: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61208e; /* fmov.s @(r0,r3),fr3 */
L8c61208e: r[0]=4u;goto L8c612090; /* mov #0x4,r0 */
L8c612090: fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c612092; /* fmov fr3,@(r0,r14) */
L8c612092: r[0]=24u;goto L8c612094; /* mov #0x18,r0 */
L8c612094: r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c612096; /* mov.l @r10,r3 */
L8c612096: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612098; /* fmov.s @(r0,r3),fr3 */
L8c612098: r[0]=8u;goto L8c61209a; /* mov #0x8,r0 */
L8c61209a: fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61209c; /* fmov fr3,@(r0,r14) */
L8c61209c: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61209e; /* mov.l @r15,r3 */
L8c61209e: r[2]=3u;goto L8c6120a0; /* mov #0x3,r2 */
L8c6120a0: r[14]+=12u;goto L8c6120a2; /* add #0xc,r14 */
L8c6120a2: r[3]+=1u;goto L8c6120a4; /* add #0x1,r3 */
L8c6120a4: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c6120a6; /* cmp/ge r2,r3 */
L8c6120a6: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6120a8; /* mov.l r3,@r15 */
L8c6120a8: r[10]+=4u;goto L8c6120aa; /* add #0x4,r10 */
L8c6120aa: take=(*sr&1u);r[13]+=12u;if(take)goto L8c6120b2;goto L8c6120ae; /* bt/s 0x8c6120b2 */
L8c6120ae: goto L8c611f68; /* bra 0x8c611f68 */
L8c6120b2: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6120b4; /* mov.l @(0x8,r15),r3 */
L8c6120b4: r[0]=8u;goto L8c6120b6; /* mov #0x8,r0 */
L8c6120b6: r[6]=r[9];goto L8c6120b8; /* mov r9,r6 */
L8c6120b8: r[6]+=12u;goto L8c6120ba; /* add #0xc,r6 */
L8c6120ba: fload(m,bus,&io_status,f,7u,r[0]+r[3]);if(io_status)return io_status;goto L8c6120bc; /* fmov.s @(r0,r3),fr7 */
L8c6120bc: r[0]=4u;goto L8c6120be; /* mov #0x4,r0 */
L8c6120be: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c6120c0; /* fmov.s @(r0,r6),fr4 */
L8c6120c0: r[5]=r[9];goto L8c6120c2; /* mov r9,r5 */
L8c6120c2: fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c6120c4; /* fmov.s @(r0,r3),fr9 */
L8c6120c4: r[0]=8u;goto L8c6120c6; /* mov #0x8,r0 */
L8c6120c6: fload(m,bus,&io_status,f,6u,r[0]+r[6]);if(io_status)return io_status;goto L8c6120c8; /* fmov.s @(r0,r6),fr6 */
L8c6120c8: r[0]=4u;goto L8c6120ca; /* mov #0x4,r0 */
L8c6120ca: fcopy(f,3u,9u);goto L8c6120cc; /* fmov fr9,fr3 */
L8c6120cc: r[5]+=24u;goto L8c6120ce; /* add #0x18,r5 */
L8c6120ce: fcopy(f,0u,7u);goto L8c6120d0; /* fmov fr7,fr0 */
L8c6120d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6120d2; /* fmul fr6,fr3 */
L8c6120d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6120d4; /* fmul fr4,fr0 */
L8c6120d4: fload(m,bus,&io_status,f,10u,r[3]);if(io_status)return io_status;goto L8c6120d6; /* fmov.s @r3,fr10 */
L8c6120d6: fload(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;goto L8c6120d8; /* fmov.s @r6,fr8 */
L8c6120d8: fcopy(f,2u,7u);goto L8c6120da; /* fmov fr7,fr2 */
L8c6120da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6120dc; /* fmul fr8,fr2 */
L8c6120dc: fcopy(f,1u,8u);goto L8c6120de; /* fmov fr8,fr1 */
L8c6120de: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6120e0; /* fsub fr3,fr0 */
L8c6120e0: fcopy(f,3u,10u);goto L8c6120e2; /* fmov fr10,fr3 */
L8c6120e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6120e4; /* fmul fr6,fr3 */
L8c6120e4: fcopy(f,11u,10u);goto L8c6120e6; /* fmov fr10,fr11 */
L8c6120e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[9],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6120e8; /* fmul fr9,fr1 */
L8c6120e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6120ea; /* fsub fr2,fr3 */
L8c6120ea: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c6120ec; /* fmov.s @(r0,r5),fr2 */
L8c6120ec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6120ee; /* fmul fr3,fr2 */
L8c6120ee: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c6120f0; /* fmov.s @r5,fr3 */
L8c6120f0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6120f2; /* fmac fr0,fr3,fr2 */
L8c6120f2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[4],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6120f4; /* fmul fr4,fr11 */
L8c6120f4: r[0]=8u;goto L8c6120f6; /* mov #0x8,r0 */
L8c6120f6: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6120f8; /* fmov.s @(r0,r5),fr3 */
L8c6120f8: r[5]=rd(m,bus,&io_status,0x8c612388u,4);if(io_status)return io_status;goto L8c6120fa; /* mov.l 0x8c612388,r5 */
L8c6120fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[11],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6120fc; /* fsub fr11,fr1 */
L8c6120fc: r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c6120fe; /* mov.l @r5,r3 */
L8c6120fe: fcopy(f,0u,1u);goto L8c612100; /* fmov fr1,fr0 */
L8c612100: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612102; /* fmac fr0,fr3,fr2 */
L8c612102: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c612104; /* fldi0 fr3 */
L8c612104: fcopy(f,4u,2u);goto L8c612106; /* fmov fr2,fr4 */
L8c612106: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c612108; /* fcmp/gt fr4,fr3 */
L8c612108: r[0]=*sr&1u;goto L8c61210a; /* movt r0 */
L8c61210a: *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c61210c; /* tst r3,r0 */
L8c61210c: if((*sr&1u))goto L8c612112;goto L8c61210e; /* bt 0x8c612112 */
L8c61210e: goto L8c6124b0; /* bra 0x8c6124b0 */
L8c612112: r[1]=0u;goto L8c612114; /* mov #0x0,r1 */
L8c612114: r[13]=rd(m,bus,&io_status,0x8c61238cu,4);if(io_status)return io_status;goto L8c612116; /* mov.l 0x8c61238c,r13 */
L8c612116: wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c612118; /* mov.l r1,@r5 */
L8c612118: r[3]=2u;goto L8c61211a; /* mov #0x2,r3 */
L8c61211a: r[5]=rd(m,bus,&io_status,0x8c612390u,4);if(io_status)return io_status;goto L8c61211c; /* mov.l 0x8c612390,r5 */
L8c61211c: r[6]=r[1];goto L8c61211e; /* mov r1,r6 */
L8c61211e: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c612120; /* mov.l r3,@(0x4,r15) */
L8c612120: r[10]=r[6];goto L8c612122; /* mov r6,r10 */
L8c612122: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612124; /* mov.l @(0x4,r15),r3 */
L8c612124: r[0]=r[7];goto L8c612126; /* mov r7,r0 */
L8c612126: r[14]=r[10];goto L8c612128; /* mov r10,r14 */
L8c612128: r[14]<<=2;goto L8c61212a; /* shll2 r14 */
L8c61212a: r[3]<<=2;goto L8c61212c; /* shll2 r3 */
L8c61212c: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61212e; /* mov.l r3,@r15 */
L8c61212e: r[2]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c612130; /* mov.l @(r0,r3),r2 */
L8c612130: r[3]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612132; /* mov.l @(0xc,r2),r3 */
L8c612132: r[2]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c612134; /* mov.l @(r0,r14),r2 */
L8c612134: r[3]&=r[4];goto L8c612136; /* and r4,r3 */
L8c612136: if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[2]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612138; /* mov.l @(0xc,r2),r2 */
L8c612138: r[2]&=r[4];goto L8c61213a; /* and r4,r2 */
L8c61213a: r[3]^=r[2];goto L8c61213c; /* xor r2,r3 */
L8c61213c: r[0]=r[3];goto L8c61213e; /* mov r3,r0 */
L8c61213e: *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c612140; /* cmp/eq #0x1,r0 */
L8c612140: if((*sr&1u))goto L8c612146;goto L8c612142; /* bt 0x8c612146 */
L8c612142: goto L8c612248; /* bra 0x8c612248 */
L8c612146: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612148; /* mov.l @(0x4,r15),r2 */
L8c612148: r[0]=8u;goto L8c61214a; /* mov #0x8,r0 */
L8c61214a: r[1]=r[10];goto L8c61214c; /* mov r10,r1 */
L8c61214c: r[3]=r[2];goto L8c61214e; /* mov r2,r3 */
L8c61214e: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612150; /* shll r2 */
L8c612150: r[2]+=r[3];goto L8c612152; /* add r3,r2 */
L8c612152: r[3]=rd(m,bus,&io_status,0x8c612394u,4);if(io_status)return io_status;goto L8c612154; /* mov.l 0x8c612394,r3 */
L8c612154: r[2]<<=2;goto L8c612156; /* shll2 r2 */
L8c612156: r[2]+=r[9];goto L8c612158; /* add r9,r2 */
L8c612158: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c61215a; /* mov.l r2,@(0xc,r15) */
L8c61215a: fload(m,bus,&io_status,f,4u,r[0]+r[2]);if(io_status)return io_status;goto L8c61215c; /* fmov.s @(r0,r2),fr4 */
L8c61215c: r[2]=r[10];goto L8c61215e; /* mov r10,r2 */
L8c61215e: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612160; /* shll r2 */
L8c612160: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c612162; /* fmov.s @r3,fr3 */
L8c612162: r[2]+=r[1];goto L8c612164; /* add r1,r2 */
L8c612164: r[2]<<=2;goto L8c612166; /* shll2 r2 */
L8c612166: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612168; /* fsub fr4,fr3 */
L8c612168: r[0]=8u;goto L8c61216a; /* mov #0x8,r0 */
L8c61216a: r[2]+=r[9];goto L8c61216c; /* add r9,r2 */
L8c61216c: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c61216e; /* mov.l r2,@(0x8,r15) */
L8c61216e: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c612170; /* fmov.s @(r0,r2),fr2 */
L8c612170: r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612172; /* mov.l @(0x8,r15),r1 */
L8c612172: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612174; /* fsub fr4,fr2 */
L8c612174: fcopy(f,4u,3u);goto L8c612176; /* fmov fr3,fr4 */
L8c612176: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c612178; /* mov.l @(0xc,r15),r2 */
L8c612178: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61217a; /* fmov.s @r1,fr3 */
L8c61217a: fload(m,bus,&io_status,f,6u,r[2]);if(io_status)return io_status;goto L8c61217c; /* fmov.s @r2,fr6 */
L8c61217c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61217e; /* fdiv fr2,fr4 */
L8c61217e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612180; /* fsub fr6,fr3 */
L8c612180: fcopy(f,2u,6u);goto L8c612182; /* fmov fr6,fr2 */
L8c612182: fcopy(f,0u,4u);goto L8c612184; /* fmov fr4,fr0 */
L8c612184: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612186; /* fmac fr0,fr3,fr2 */
L8c612186: r[1]=rd(m,bus,&io_status,0x8c612398u,4);if(io_status)return io_status;goto L8c612188; /* mov.l 0x8c612398,r1 */
L8c612188: r[0]=12u;goto L8c61218a; /* mov #0xc,r0 */
L8c61218a: fcopy(f,6u,2u);goto L8c61218c; /* fmov fr2,fr6 */
L8c61218c: r[2]=r[6];goto L8c61218e; /* mov r6,r2 */
L8c61218e: fcopy(f,0u,5u);goto L8c612190; /* fmov fr5,fr0 */
L8c612190: r[2]<<=2;goto L8c612192; /* shll2 r2 */
L8c612192: r[2]<<=2;goto L8c612194; /* shll2 r2 */
L8c612194: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612196; /* shll r2 */
L8c612196: r[2]+=r[13];goto L8c612198; /* add r13,r2 */
L8c612198: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c61219a; /* mov.l r2,@(0x4,r15) */
L8c61219a: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61219c; /* fmov.s @r1,fr3 */
L8c61219c: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61219e; /* fmov.s @(r0,r12),fr2 */
L8c61219e: r[0]=16u;goto L8c6121a0; /* mov #0x10,r0 */
L8c6121a0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6121a2; /* fmul fr6,fr3 */
L8c6121a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6121a4; /* fmac fr0,fr3,fr2 */
L8c6121a4: fcopy(f,0u,4u);goto L8c6121a6; /* fmov fr4,fr0 */
L8c6121a6: fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6121a8; /* fmov fr2,@(r0,r2) */
L8c6121a8: r[0]=4u;goto L8c6121aa; /* mov #0x4,r0 */
L8c6121aa: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6121ac; /* mov.l @(0xc,r15),r2 */
L8c6121ac: fload(m,bus,&io_status,f,6u,r[0]+r[2]);if(io_status)return io_status;goto L8c6121ae; /* fmov.s @(r0,r2),fr6 */
L8c6121ae: r[0]=4u;goto L8c6121b0; /* mov #0x4,r0 */
L8c6121b0: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6121b2; /* mov.l @(0x8,r15),r2 */
L8c6121b2: fcopy(f,2u,6u);goto L8c6121b4; /* fmov fr6,fr2 */
L8c6121b4: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c6121b6; /* fmov.s @(r0,r2),fr3 */
L8c6121b6: r[0]=rd(m,bus,&io_status,0x8c61239cu,4);if(io_status)return io_status;goto L8c6121b8; /* mov.l 0x8c61239c,r0 */
L8c6121b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6121ba; /* fsub fr6,fr3 */
L8c6121ba: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6121bc; /* mov.l @(0x4,r15),r2 */
L8c6121bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6121be; /* fmac fr0,fr3,fr2 */
L8c6121be: fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c6121c0; /* fmov.s @r0,fr3 */
L8c6121c0: r[0]=16u;goto L8c6121c2; /* mov #0x10,r0 */
L8c6121c2: fcopy(f,6u,2u);goto L8c6121c4; /* fmov fr2,fr6 */
L8c6121c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6121c6; /* fmul fr6,fr3 */
L8c6121c6: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6121c8; /* fmov.s @(r0,r12),fr2 */
L8c6121c8: r[0]=20u;goto L8c6121ca; /* mov #0x14,r0 */
L8c6121ca: fcopy(f,0u,5u);goto L8c6121cc; /* fmov fr5,fr0 */
L8c6121cc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6121ce; /* fmac fr0,fr3,fr2 */
L8c6121ce: fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6121d0; /* fmov fr2,@(r0,r2) */
L8c6121d0: r[0]=24u;goto L8c6121d2; /* mov #0x18,r0 */
L8c6121d2: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6121d4; /* mov.l @(0x4,r15),r3 */
L8c6121d4: fstore(m,bus,&io_status,f,0u,r[0]+r[3]);if(io_status)return io_status;goto L8c6121d6; /* fmov fr0,@(r0,r3) */
L8c6121d6: r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6121d8; /* mov.l @r15,r0 */
L8c6121d8: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6121da; /* mov.l @(0x4,r15),r3 */
L8c6121da: r[2]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c6121dc; /* mov.l @(r0,r7),r2 */
L8c6121dc: r[0]=28u;goto L8c6121de; /* mov #0x1c,r0 */
L8c6121de: fcopy(f,0u,4u);goto L8c6121e0; /* fmov fr4,fr0 */
L8c6121e0: wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c6121e2; /* mov.l r2,@(0x14,r15) */
L8c6121e2: fload(m,bus,&io_status,f,6u,r[0]+r[2]);if(io_status)return io_status;goto L8c6121e4; /* fmov.s @(r0,r2),fr6 */
L8c6121e4: r[0]=r[7];goto L8c6121e6; /* mov r7,r0 */
L8c6121e6: r[2]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c6121e8; /* mov.l @(r0,r14),r2 */
L8c6121e8: r[0]=28u;goto L8c6121ea; /* mov #0x1c,r0 */
L8c6121ea: fcopy(f,2u,6u);goto L8c6121ec; /* fmov fr6,fr2 */
L8c6121ec: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c6121ee; /* fmov.s @(r0,r2),fr3 */
L8c6121ee: r[2]=r[0];goto L8c6121f0; /* mov r0,r2 */
L8c6121f0: r[2]+=r[3];goto L8c6121f2; /* add r3,r2 */
L8c6121f2: r[3]=r[6];goto L8c6121f4; /* mov r6,r3 */
L8c6121f4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6121f6; /* fsub fr6,fr3 */
L8c6121f6: r[3]<<=2;goto L8c6121f8; /* shll2 r3 */
L8c6121f8: r[3]+=r[5];goto L8c6121fa; /* add r5,r3 */
L8c6121fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6121fc; /* fmac fr0,fr3,fr2 */
L8c6121fc: fstore(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c6121fe; /* fmov.s fr2,@r2 */
L8c6121fe: wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c612200; /* mov.l r3,@(0xc,r15) */
L8c612200: r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612202; /* mov.l @r15,r0 */
L8c612202: r[0]+=r[11];goto L8c612204; /* add r11,r0 */
L8c612204: wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;goto L8c612206; /* mov.l r0,@(0x8,r15) */
L8c612206: r[2]=r[14];goto L8c612208; /* mov r14,r2 */
L8c612208: r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61220a; /* mov.w @r0,r0 */
L8c61220a: r[2]+=r[11];goto L8c61220c; /* add r11,r2 */
L8c61220c: wr(m,bus,&io_status,r[15]+0x10u,r[0],2);if(io_status)return io_status;goto L8c61220e; /* mov.w r0,@(0x10,r15) */
L8c61220e: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c612210; /* mov.l r2,@(0x4,r15) */
L8c612210: *fpul=r[0];goto L8c612212; /* lds r0,FPUL */
L8c612212: r[0]=sx(m,bus,&io_status,r[15]+0x10u,2);if(io_status)return io_status;goto L8c612214; /* mov.w @(0x10,r15),r0 */
L8c612214: r[2]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c612216; /* mov.w @r2,r2 */
L8c612216: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612218; /* float FPUL,fr3 */
L8c612218: r[2]-=r[0];goto L8c61221a; /* sub r0,r2 */
L8c61221a: *fpul=r[2];goto L8c61221c; /* lds r2,FPUL */
L8c61221c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c61221e; /* float FPUL,fr2 */
L8c61221e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612220; /* fmac fr0,fr2,fr3 */
L8c612220: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c612222; /* ftrc fr3,FPUL */
L8c612222: r[2]=*fpul;goto L8c612224; /* sts FPUL,r2 */
L8c612224: wr(m,bus,&io_status,r[3],r[2],2);if(io_status)return io_status;goto L8c612226; /* mov.w r2,@r3 */
L8c612226: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612228; /* mov.l @(0x8,r15),r2 */
L8c612228: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61222a; /* mov.l @(0xc,r15),r3 */
L8c61222a: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61222c; /* mov.w @(0x2,r2),r0 */
L8c61222c: *fpul=r[0];goto L8c61222e; /* lds r0,FPUL */
L8c61222e: wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c612230; /* mov.w r0,@r15 */
L8c612230: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612232; /* mov.l @(0x4,r15),r2 */
L8c612232: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612234; /* float FPUL,fr3 */
L8c612234: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c612236; /* mov.w @(0x2,r2),r0 */
L8c612236: r[2]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c612238; /* mov.w @r15,r2 */
L8c612238: r[0]-=r[2];goto L8c61223a; /* sub r2,r0 */
L8c61223a: *fpul=r[0];goto L8c61223c; /* lds r0,FPUL */
L8c61223c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c61223e; /* float FPUL,fr2 */
L8c61223e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612240; /* fmac fr0,fr2,fr3 */
L8c612240: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c612242; /* ftrc fr3,FPUL */
L8c612242: r[0]=*fpul;goto L8c612244; /* sts FPUL,r0 */
L8c612244: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c612246; /* mov.w r0,@(0x2,r3) */
L8c612246: r[6]+=1u;goto L8c612248; /* add #0x1,r6 */
L8c612248: r[0]=r[14];goto L8c61224a; /* mov r14,r0 */
L8c61224a: r[3]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c61224c; /* mov.l @(r0,r7),r3 */
L8c61224c: r[3]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c61224e; /* mov.l @(0xc,r3),r3 */
L8c61224e: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c612250; /* tst r4,r3 */
L8c612250: if(!(*sr&1u))goto L8c6122ac;goto L8c612252; /* bf 0x8c6122ac */
L8c612252: r[3]=r[14];goto L8c612254; /* mov r14,r3 */
L8c612254: r[2]=r[6];goto L8c612256; /* mov r6,r2 */
L8c612256: r[3]+=r[7];goto L8c612258; /* add r7,r3 */
L8c612258: r[2]<<=2;goto L8c61225a; /* shll2 r2 */
L8c61225a: r[0]=16u;goto L8c61225c; /* mov #0x10,r0 */
L8c61225c: r[2]<<=2;goto L8c61225e; /* shll2 r2 */
L8c61225e: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612260; /* shll r2 */
L8c612260: r[2]+=r[13];goto L8c612262; /* add r13,r2 */
L8c612262: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c612264; /* mov.l r2,@(0x4,r15) */
L8c612264: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c612266; /* mov.l r3,@r15 */
L8c612266: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c612268; /* mov.l @r3,r1 */
L8c612268: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61226a; /* fmov.s @(r0,r1),fr3 */
L8c61226a: fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61226c; /* fmov fr3,@(r0,r2) */
L8c61226c: r[0]=20u;goto L8c61226e; /* mov #0x14,r0 */
L8c61226e: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612270; /* mov.l @r15,r2 */
L8c612270: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612272; /* mov.l @(0x4,r15),r3 */
L8c612272: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c612274; /* mov.l @r2,r1 */
L8c612274: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612276; /* fmov.s @(r0,r1),fr3 */
L8c612276: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612278; /* fmov fr3,@(r0,r3) */
L8c612278: r[0]=24u;goto L8c61227a; /* mov #0x18,r0 */
L8c61227a: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61227c; /* mov.l @r15,r2 */
L8c61227c: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61227e; /* mov.l @(0x4,r15),r3 */
L8c61227e: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c612280; /* mov.l @r2,r1 */
L8c612280: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612282; /* fmov.s @(r0,r1),fr3 */
L8c612282: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612284; /* fmov fr3,@(r0,r3) */
L8c612284: r[0]=28u;goto L8c612286; /* mov #0x1c,r0 */
L8c612286: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612288; /* mov.l @r15,r2 */
L8c612288: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61228a; /* mov.l @(0x4,r15),r3 */
L8c61228a: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61228c; /* mov.l @r2,r1 */
L8c61228c: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61228e; /* fmov.s @(r0,r1),fr3 */
L8c61228e: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612290; /* fmov fr3,@(r0,r3) */
L8c612290: r[3]=r[6];goto L8c612292; /* mov r6,r3 */
L8c612292: r[2]=r[14];goto L8c612294; /* mov r14,r2 */
L8c612294: r[2]+=r[11];goto L8c612296; /* add r11,r2 */
L8c612296: r[3]<<=2;goto L8c612298; /* shll2 r3 */
L8c612298: r[3]+=r[5];goto L8c61229a; /* add r5,r3 */
L8c61229a: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61229c; /* mov.l r3,@(0x4,r15) */
L8c61229c: r[6]+=1u;goto L8c61229e; /* add #0x1,r6 */
L8c61229e: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6122a0; /* mov.l r2,@r15 */
L8c6122a0: r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c6122a2; /* mov.w @r2,r1 */
L8c6122a2: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c6122a4; /* mov.w r1,@r3 */
L8c6122a4: r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6122a6; /* mov.l @r15,r0 */
L8c6122a6: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6122a8; /* mov.l @(0x4,r15),r3 */
L8c6122a8: r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c6122aa; /* mov.w @(0x2,r0),r0 */
L8c6122aa: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6122ac; /* mov.w r0,@(0x2,r3) */
L8c6122ac: wr(m,bus,&io_status,r[15]+0x4u,r[10],4);if(io_status)return io_status;goto L8c6122ae; /* mov.l r10,@(0x4,r15) */
L8c6122ae: r[3]=3u;goto L8c6122b0; /* mov #0x3,r3 */
L8c6122b0: r[10]+=1u;goto L8c6122b2; /* add #0x1,r10 */
L8c6122b2: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[3]);goto L8c6122b4; /* cmp/ge r3,r10 */
L8c6122b4: if((*sr&1u))goto L8c6122ba;goto L8c6122b6; /* bt 0x8c6122ba */
L8c6122b6: goto L8c612122; /* bra 0x8c612122 */
L8c6122ba: r[0]=0x8c6123a8u;goto L8c6122bc; /* mova 0x8c6123a8,r0 */
L8c6122bc: r[4]=rd(m,bus,&io_status,0x8c6123a4u,4);if(io_status)return io_status;goto L8c6122be; /* mov.l 0x8c6123a4,r4 */
L8c6122be: fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c6122c0; /* fmov.s @r0,fr4 */
L8c6122c0: r[0]=r[6];goto L8c6122c2; /* mov r6,r0 */
L8c6122c2: r[7]=rd(m,bus,&io_status,0x8c6123a0u,4);if(io_status)return io_status;goto L8c6122c4; /* mov.l 0x8c6123a0,r7 */
L8c6122c4: *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c6122c6; /* cmp/eq #0x3,r0 */
L8c6122c6: if(!(*sr&1u))goto L8c6123b0;goto L8c6122c8; /* bf 0x8c6123b0 */
L8c6122c8: r[0]=28u;goto L8c6122ca; /* mov #0x1c,r0 */
L8c6122ca: r[6]=r[13];goto L8c6122cc; /* mov r13,r6 */
L8c6122cc: r[3]=r[5];goto L8c6122ce; /* mov r5,r3 */
L8c6122ce: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6122d0; /* fmov.s @(r0,r6),fr3 */
L8c6122d0: r[8]+=28u;goto L8c6122d2; /* add #0x1c,r8 */
L8c6122d2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6122d4; /* fmov.s fr3,@-r8 */
L8c6122d4: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6122d6; /* mov.l r3,@r15 */
L8c6122d6: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6122d8; /* mov.w @(0x2,r3),r0 */
L8c6122d8: r[3]=r[0];goto L8c6122da; /* mov r0,r3 */
L8c6122da: *fpul=r[3];goto L8c6122dc; /* lds r3,FPUL */
L8c6122dc: r[0]=24u;goto L8c6122de; /* mov #0x18,r0 */
L8c6122de: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6122e0; /* float FPUL,fr3 */
L8c6122e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6122e2; /* fmul fr4,fr3 */
L8c6122e2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6122e4; /* fmov.s fr3,@-r8 */
L8c6122e4: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6122e6; /* mov.l @r15,r3 */
L8c6122e6: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6122e8; /* mov.w @r3,r3 */
L8c6122e8: *fpul=r[3];goto L8c6122ea; /* lds r3,FPUL */
L8c6122ea: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6122ec; /* float FPUL,fr3 */
L8c6122ec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6122ee; /* fmul fr4,fr3 */
L8c6122ee: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6122f0; /* fmov.s fr3,@-r8 */
L8c6122f0: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6122f2; /* fmov.s @(r0,r6),fr3 */
L8c6122f2: r[0]=20u;goto L8c6122f4; /* mov #0x14,r0 */
L8c6122f4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6122f6; /* fmov.s fr3,@-r8 */
L8c6122f6: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6122f8; /* fmov.s @(r0,r6),fr3 */
L8c6122f8: r[0]=16u;goto L8c6122fa; /* mov #0x10,r0 */
L8c6122fa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6122fc; /* fmov.s fr3,@-r8 */
L8c6122fc: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6122fe; /* fmov.s @(r0,r6),fr3 */
L8c6122fe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612300; /* fmov.s fr3,@-r8 */
L8c612300: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612302; /* mov.l r4,@-r8 */
L8c612302: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612304; /* pref @r8 */
L8c612304: r[0]=28u;goto L8c612306; /* mov #0x1c,r0 */
L8c612306: r[3]=r[5];goto L8c612308; /* mov r5,r3 */
L8c612308: r[6]+=32u;goto L8c61230a; /* add #0x20,r6 */
L8c61230a: r[3]+=4u;goto L8c61230c; /* add #0x4,r3 */
L8c61230c: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61230e; /* fmov.s @(r0,r6),fr3 */
L8c61230e: r[8]+=60u;goto L8c612310; /* add #0x3c,r8 */
L8c612310: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612312; /* fmov.s fr3,@-r8 */
L8c612312: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c612314; /* mov.l r3,@r15 */
L8c612314: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c612316; /* mov.w @(0x2,r3),r0 */
L8c612316: r[3]=r[0];goto L8c612318; /* mov r0,r3 */
L8c612318: *fpul=r[3];goto L8c61231a; /* lds r3,FPUL */
L8c61231a: r[0]=24u;goto L8c61231c; /* mov #0x18,r0 */
L8c61231c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61231e; /* float FPUL,fr3 */
L8c61231e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612320; /* fmul fr4,fr3 */
L8c612320: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612322; /* fmov.s fr3,@-r8 */
L8c612322: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612324; /* mov.l @r15,r3 */
L8c612324: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c612326; /* mov.w @r3,r3 */
L8c612326: *fpul=r[3];goto L8c612328; /* lds r3,FPUL */
L8c612328: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61232a; /* float FPUL,fr3 */
L8c61232a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61232c; /* fmul fr4,fr3 */
L8c61232c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61232e; /* fmov.s fr3,@-r8 */
L8c61232e: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612330; /* fmov.s @(r0,r6),fr3 */
L8c612330: r[0]=20u;goto L8c612332; /* mov #0x14,r0 */
L8c612332: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612334; /* fmov.s fr3,@-r8 */
L8c612334: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612336; /* fmov.s @(r0,r6),fr3 */
L8c612336: r[0]=16u;goto L8c612338; /* mov #0x10,r0 */
L8c612338: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61233a; /* fmov.s fr3,@-r8 */
L8c61233a: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61233c; /* fmov.s @(r0,r6),fr3 */
L8c61233c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61233e; /* fmov.s fr3,@-r8 */
L8c61233e: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612340; /* mov.l r4,@-r8 */
L8c612340: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612342; /* pref @r8 */
L8c612342: r[4]=r[13];goto L8c612344; /* mov r13,r4 */
L8c612344: r[0]=28u;goto L8c612346; /* mov #0x1c,r0 */
L8c612346: r[6]=r[5];goto L8c612348; /* mov r5,r6 */
L8c612348: r[4]+=64u;goto L8c61234a; /* add #0x40,r4 */
L8c61234a: r[6]+=8u;goto L8c61234c; /* add #0x8,r6 */
L8c61234c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61234e; /* fmov.s @(r0,r4),fr3 */
L8c61234e: r[8]+=60u;goto L8c612350; /* add #0x3c,r8 */
L8c612350: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612352; /* fmov.s fr3,@-r8 */
L8c612352: r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c612354; /* mov.w @(0x2,r6),r0 */
L8c612354: r[3]=r[0];goto L8c612356; /* mov r0,r3 */
L8c612356: *fpul=r[3];goto L8c612358; /* lds r3,FPUL */
L8c612358: r[0]=24u;goto L8c61235a; /* mov #0x18,r0 */
L8c61235a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61235c; /* float FPUL,fr3 */
L8c61235c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61235e; /* fmul fr4,fr3 */
L8c61235e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612360; /* fmov.s fr3,@-r8 */
L8c612360: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c612362; /* mov.w @r6,r3 */
L8c612362: *fpul=r[3];goto L8c612364; /* lds r3,FPUL */
L8c612364: r[3]=rd(m,bus,&io_status,0x8c6123acu,4);if(io_status)return io_status;goto L8c612366; /* mov.l 0x8c6123ac,r3 */
L8c612366: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612368; /* float FPUL,fr3 */
L8c612368: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61236a; /* fmul fr4,fr3 */
L8c61236a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61236c; /* fmov.s fr3,@-r8 */
L8c61236c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61236e; /* fmov.s @(r0,r4),fr3 */
L8c61236e: r[0]=20u;goto L8c612370; /* mov #0x14,r0 */
L8c612370: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612372; /* fmov.s fr3,@-r8 */
L8c612372: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612374; /* fmov.s @(r0,r4),fr3 */
L8c612374: r[0]=16u;goto L8c612376; /* mov #0x10,r0 */
L8c612376: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612378; /* fmov.s fr3,@-r8 */
L8c612378: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61237a; /* fmov.s @(r0,r4),fr3 */
L8c61237a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61237c; /* fmov.s fr3,@-r8 */
L8c61237c: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61237e; /* mov.l r3,@-r8 */
L8c61237e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612380; /* pref @r8 */
L8c612380: r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c612382; /* mov.l @r7,r2 */
L8c612382: r[8]+=32u;goto L8c612384; /* add #0x20,r8 */
L8c612384: r[2]+=3u;goto L8c6124ae; /* bra 0x8c6124ae */
L8c6123b0: r[0]=28u;goto L8c6123b2; /* mov #0x1c,r0 */
L8c6123b2: r[6]=r[13];goto L8c6123b4; /* mov r13,r6 */
L8c6123b4: r[3]=r[5];goto L8c6123b6; /* mov r5,r3 */
L8c6123b6: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6123b8; /* fmov.s @(r0,r6),fr3 */
L8c6123b8: r[8]+=28u;goto L8c6123ba; /* add #0x1c,r8 */
L8c6123ba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6123bc; /* fmov.s fr3,@-r8 */
L8c6123bc: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6123be; /* mov.l r3,@r15 */
L8c6123be: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6123c0; /* mov.w @(0x2,r3),r0 */
L8c6123c0: r[3]=r[0];goto L8c6123c2; /* mov r0,r3 */
L8c6123c2: *fpul=r[3];goto L8c6123c4; /* lds r3,FPUL */
L8c6123c4: r[0]=24u;goto L8c6123c6; /* mov #0x18,r0 */
L8c6123c6: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6123c8; /* float FPUL,fr3 */
L8c6123c8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6123ca; /* fmul fr4,fr3 */
L8c6123ca: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6123cc; /* fmov.s fr3,@-r8 */
L8c6123cc: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6123ce; /* mov.l @r15,r3 */
L8c6123ce: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6123d0; /* mov.w @r3,r3 */
L8c6123d0: *fpul=r[3];goto L8c6123d2; /* lds r3,FPUL */
L8c6123d2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6123d4; /* float FPUL,fr3 */
L8c6123d4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6123d6; /* fmul fr4,fr3 */
L8c6123d6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6123d8; /* fmov.s fr3,@-r8 */
L8c6123d8: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6123da; /* fmov.s @(r0,r6),fr3 */
L8c6123da: r[0]=20u;goto L8c6123dc; /* mov #0x14,r0 */
L8c6123dc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6123de; /* fmov.s fr3,@-r8 */
L8c6123de: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6123e0; /* fmov.s @(r0,r6),fr3 */
L8c6123e0: r[0]=16u;goto L8c6123e2; /* mov #0x10,r0 */
L8c6123e2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6123e4; /* fmov.s fr3,@-r8 */
L8c6123e4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6123e6; /* fmov.s @(r0,r6),fr3 */
L8c6123e6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6123e8; /* fmov.s fr3,@-r8 */
L8c6123e8: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c6123ea; /* mov.l r4,@-r8 */
L8c6123ea: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6123ec; /* pref @r8 */
L8c6123ec: r[0]=28u;goto L8c6123ee; /* mov #0x1c,r0 */
L8c6123ee: r[3]=r[5];goto L8c6123f0; /* mov r5,r3 */
L8c6123f0: r[6]+=32u;goto L8c6123f2; /* add #0x20,r6 */
L8c6123f2: r[3]+=4u;goto L8c6123f4; /* add #0x4,r3 */
L8c6123f4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6123f6; /* fmov.s @(r0,r6),fr3 */
L8c6123f6: r[8]+=60u;goto L8c6123f8; /* add #0x3c,r8 */
L8c6123f8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6123fa; /* fmov.s fr3,@-r8 */
L8c6123fa: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6123fc; /* mov.l r3,@r15 */
L8c6123fc: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6123fe; /* mov.w @(0x2,r3),r0 */
L8c6123fe: r[3]=r[0];goto L8c612400; /* mov r0,r3 */
L8c612400: *fpul=r[3];goto L8c612402; /* lds r3,FPUL */
L8c612402: r[0]=24u;goto L8c612404; /* mov #0x18,r0 */
L8c612404: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612406; /* float FPUL,fr3 */
L8c612406: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612408; /* fmul fr4,fr3 */
L8c612408: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61240a; /* fmov.s fr3,@-r8 */
L8c61240a: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61240c; /* mov.l @r15,r3 */
L8c61240c: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c61240e; /* mov.w @r3,r3 */
L8c61240e: *fpul=r[3];goto L8c612410; /* lds r3,FPUL */
L8c612410: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612412; /* float FPUL,fr3 */
L8c612412: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612414; /* fmul fr4,fr3 */
L8c612414: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612416; /* fmov.s fr3,@-r8 */
L8c612416: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612418; /* fmov.s @(r0,r6),fr3 */
L8c612418: r[0]=20u;goto L8c61241a; /* mov #0x14,r0 */
L8c61241a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61241c; /* fmov.s fr3,@-r8 */
L8c61241c: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61241e; /* fmov.s @(r0,r6),fr3 */
L8c61241e: r[0]=16u;goto L8c612420; /* mov #0x10,r0 */
L8c612420: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612422; /* fmov.s fr3,@-r8 */
L8c612422: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612424; /* fmov.s @(r0,r6),fr3 */
L8c612424: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612426; /* fmov.s fr3,@-r8 */
L8c612426: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612428; /* mov.l r4,@-r8 */
L8c612428: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61242a; /* pref @r8 */
L8c61242a: r[6]=r[13];goto L8c61242c; /* mov r13,r6 */
L8c61242c: r[0]=28u;goto L8c61242e; /* mov #0x1c,r0 */
L8c61242e: r[3]=r[5];goto L8c612430; /* mov r5,r3 */
L8c612430: r[6]+=96u;goto L8c612432; /* add #0x60,r6 */
L8c612432: r[3]+=12u;goto L8c612434; /* add #0xc,r3 */
L8c612434: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612436; /* fmov.s @(r0,r6),fr3 */
L8c612436: r[8]+=60u;goto L8c612438; /* add #0x3c,r8 */
L8c612438: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61243a; /* fmov.s fr3,@-r8 */
L8c61243a: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61243c; /* mov.l r3,@r15 */
L8c61243c: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61243e; /* mov.w @(0x2,r3),r0 */
L8c61243e: r[3]=r[0];goto L8c612440; /* mov r0,r3 */
L8c612440: *fpul=r[3];goto L8c612442; /* lds r3,FPUL */
L8c612442: r[0]=24u;goto L8c612444; /* mov #0x18,r0 */
L8c612444: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612446; /* float FPUL,fr3 */
L8c612446: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612448; /* fmul fr4,fr3 */
L8c612448: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61244a; /* fmov.s fr3,@-r8 */
L8c61244a: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61244c; /* mov.l @r15,r3 */
L8c61244c: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c61244e; /* mov.w @r3,r3 */
L8c61244e: *fpul=r[3];goto L8c612450; /* lds r3,FPUL */
L8c612450: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612452; /* float FPUL,fr3 */
L8c612452: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612454; /* fmul fr4,fr3 */
L8c612454: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612456; /* fmov.s fr3,@-r8 */
L8c612456: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612458; /* fmov.s @(r0,r6),fr3 */
L8c612458: r[0]=20u;goto L8c61245a; /* mov #0x14,r0 */
L8c61245a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61245c; /* fmov.s fr3,@-r8 */
L8c61245c: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61245e; /* fmov.s @(r0,r6),fr3 */
L8c61245e: r[0]=16u;goto L8c612460; /* mov #0x10,r0 */
L8c612460: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612462; /* fmov.s fr3,@-r8 */
L8c612462: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612464; /* fmov.s @(r0,r6),fr3 */
L8c612464: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612466; /* fmov.s fr3,@-r8 */
L8c612466: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612468; /* mov.l r4,@-r8 */
L8c612468: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61246a; /* pref @r8 */
L8c61246a: r[4]=r[13];goto L8c61246c; /* mov r13,r4 */
L8c61246c: r[0]=28u;goto L8c61246e; /* mov #0x1c,r0 */
L8c61246e: r[6]=r[5];goto L8c612470; /* mov r5,r6 */
L8c612470: r[4]+=64u;goto L8c612472; /* add #0x40,r4 */
L8c612472: r[6]+=8u;goto L8c612474; /* add #0x8,r6 */
L8c612474: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612476; /* fmov.s @(r0,r4),fr3 */
L8c612476: r[8]+=60u;goto L8c612478; /* add #0x3c,r8 */
L8c612478: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61247a; /* fmov.s fr3,@-r8 */
L8c61247a: r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c61247c; /* mov.w @(0x2,r6),r0 */
L8c61247c: r[3]=r[0];goto L8c61247e; /* mov r0,r3 */
L8c61247e: *fpul=r[3];goto L8c612480; /* lds r3,FPUL */
L8c612480: r[0]=24u;goto L8c612482; /* mov #0x18,r0 */
L8c612482: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612484; /* float FPUL,fr3 */
L8c612484: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612486; /* fmul fr4,fr3 */
L8c612486: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612488; /* fmov.s fr3,@-r8 */
L8c612488: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c61248a; /* mov.w @r6,r3 */
L8c61248a: *fpul=r[3];goto L8c61248c; /* lds r3,FPUL */
L8c61248c: r[3]=rd(m,bus,&io_status,0x8c612554u,4);if(io_status)return io_status;goto L8c61248e; /* mov.l 0x8c612554,r3 */
L8c61248e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612490; /* float FPUL,fr3 */
L8c612490: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612492; /* fmul fr4,fr3 */
L8c612492: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612494; /* fmov.s fr3,@-r8 */
L8c612494: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612496; /* fmov.s @(r0,r4),fr3 */
L8c612496: r[0]=20u;goto L8c612498; /* mov #0x14,r0 */
L8c612498: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61249a; /* fmov.s fr3,@-r8 */
L8c61249a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61249c; /* fmov.s @(r0,r4),fr3 */
L8c61249c: r[0]=16u;goto L8c61249e; /* mov #0x10,r0 */
L8c61249e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6124a0; /* fmov.s fr3,@-r8 */
L8c6124a0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6124a2; /* fmov.s @(r0,r4),fr3 */
L8c6124a2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6124a4; /* fmov.s fr3,@-r8 */
L8c6124a4: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6124a6; /* mov.l r3,@-r8 */
L8c6124a6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6124a8; /* pref @r8 */
L8c6124a8: r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c6124aa; /* mov.l @r7,r2 */
L8c6124aa: r[8]+=32u;goto L8c6124ac; /* add #0x20,r8 */
L8c6124ac: r[2]+=4u;goto L8c6124ae; /* add #0x4,r2 */
L8c6124ae: wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c6124b0; /* mov.l r2,@r7 */
L8c6124b0: r[15]+=84u;goto L8c6124b2; /* add #0x54,r15 */
L8c6124b2: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6124b4; /* mov.l @r15+,r9 */
L8c6124b4: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6124b6; /* mov.l @r15+,r10 */
L8c6124b6: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6124b8; /* mov.l @r15+,r11 */
L8c6124b8: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6124ba; /* mov.l @r15+,r12 */
L8c6124ba: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6124bc; /* mov.l @r15+,r13 */
L8c6124bc: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
