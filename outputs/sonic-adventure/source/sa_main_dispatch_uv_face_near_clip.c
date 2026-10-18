/* UV/face-factor triangle near-plane clipper with portable queue interface; name inferred.
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
SA_STAGE_EXPORT int sa_uv_face_triangle_near_clip_8c616f18(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c616f18;
L8c616f18: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c616f1a; /* mov.l r14,@-r15 */
L8c616f1a: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c616f1c; /* mov.l r13,@-r15 */
L8c616f1c: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c616f1e; /* mov.l r12,@-r15 */
L8c616f1e: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c616f20; /* mov.l r11,@-r15 */
L8c616f20: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c616f22; /* mov.l r10,@-r15 */
L8c616f22: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c616f24; /* mov.l r9,@-r15 */
L8c616f24: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c616f26; /* fmov.s fr12,@-r15 */
L8c616f26: r[15]+=4294967228u;goto L8c616f28; /* add #-0x44,r15 */
L8c616f28: r[3]=rd(m,bus,&io_status,0x8c616fc4u,4);if(io_status)return io_status;goto L8c616f2a; /* mov.l 0x8c616fc4,r3 */
L8c616f2a: r[14]=r[15];goto L8c616f2c; /* mov r15,r14 */
L8c616f2c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c616f2e; /* fldi1 fr7 */
L8c616f2e: r[14]+=56u;goto L8c616f30; /* add #0x38,r14 */
L8c616f30: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c616f32; /* fmov.s @r3,fr3 */
L8c616f32: r[9]=r[15];goto L8c616f34; /* mov r15,r9 */
L8c616f34: fcopy(f,6u,7u);goto L8c616f36; /* fmov fr7,fr6 */
L8c616f36: r[2]=r[14];goto L8c616f38; /* mov r14,r2 */
L8c616f38: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[3],&f->fpscr,&fr[6]);if(status)return status;}goto L8c616f3a; /* fdiv fr3,fr6 */
L8c616f3a: r[12]=rd(m,bus,&io_status,0x8c616fc8u,4);if(io_status)return io_status;goto L8c616f3c; /* mov.l 0x8c616fc8,r12 */
L8c616f3c: r[9]+=20u;goto L8c616f3e; /* add #0x14,r9 */
L8c616f3e: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c616f40; /* mov.l r2,@r15 */
L8c616f40: wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c616f42; /* mov.l r4,@r2 */
L8c616f42: r[4]=1u;goto L8c616f44; /* mov #0x1,r4 */
L8c616f44: wr(m,bus,&io_status,r[14]+0x4u,r[5],4);if(io_status)return io_status;goto L8c616f46; /* mov.l r5,@(0x4,r14) */
L8c616f46: r[5]=r[9];goto L8c616f48; /* mov r9,r5 */
L8c616f48: wr(m,bus,&io_status,r[14]+0x8u,r[6],4);if(io_status)return io_status;goto L8c616f4a; /* mov.l r6,@(0x8,r14) */
L8c616f4a: r[11]=0u;goto L8c616f4c; /* mov #0x0,r11 */
L8c616f4c: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616f4e; /* mov.l @r15,r13 */
L8c616f4e: r[10]=r[9];goto L8c616f50; /* mov r9,r10 */
L8c616f50: r[6]=r[9];goto L8c616f52; /* mov r9,r6 */
L8c616f52: r[0]=r[11];goto L8c616f54; /* mov r11,r0 */
L8c616f54: r[0]<<=2;goto L8c616f56; /* shll2 r0 */
L8c616f56: r[3]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c616f58; /* mov.l @(r0,r14),r3 */
L8c616f58: r[2]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c616f5a; /* mov.l @(0xc,r3),r2 */
L8c616f5a: *sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c616f5c; /* tst r4,r2 */
L8c616f5c: if(!(*sr&1u))goto L8c616fd4;goto L8c616f5e; /* bf 0x8c616fd4 */
L8c616f5e: r[1]=r[11];goto L8c616f60; /* mov r11,r1 */
L8c616f60: r[1]<<=2;goto L8c616f62; /* shll2 r1 */
L8c616f62: r[1]+=r[14];goto L8c616f64; /* add r14,r1 */
L8c616f64: fcopy(f,5u,7u);goto L8c616f66; /* fmov fr7,fr5 */
L8c616f66: r[0]=24u;goto L8c616f68; /* mov #0x18,r0 */
L8c616f68: wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c616f6a; /* mov.l r1,@(0x4,r15) */
L8c616f6a: r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c616f6c; /* mov.l @r1,r3 */
L8c616f6c: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c616f6e; /* mov.l r3,@r15 */
L8c616f6e: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616f70; /* fmov.s @(r0,r3),fr3 */
L8c616f70: r[0]=12u;goto L8c616f72; /* mov #0xc,r0 */
L8c616f72: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c616f74; /* fdiv fr3,fr5 */
L8c616f74: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c616f76; /* fmov.s @(r0,r12),fr3 */
L8c616f76: r[0]=16u;goto L8c616f78; /* mov #0x10,r0 */
L8c616f78: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c616f7a; /* fmov.s @(r0,r3),fr2 */
L8c616f7a: r[0]=16u;goto L8c616f7c; /* mov #0x10,r0 */
L8c616f7c: r[3]=rd(m,bus,&io_status,0x8c616fccu,4);if(io_status)return io_status;goto L8c616f7e; /* mov.l 0x8c616fcc,r3 */
L8c616f7e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616f80; /* fsub fr3,fr2 */
L8c616f80: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c616f82; /* fmov.s @r3,fr3 */
L8c616f82: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616f84; /* fmul fr5,fr2 */
L8c616f84: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616f86; /* fdiv fr3,fr2 */
L8c616f86: fstore(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c616f88; /* fmov.s fr2,@r5 */
L8c616f88: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616f8a; /* mov.l @(0x4,r15),r2 */
L8c616f8a: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c616f8c; /* fmov.s @(r0,r12),fr3 */
L8c616f8c: r[0]=20u;goto L8c616f8e; /* mov #0x14,r0 */
L8c616f8e: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c616f90; /* mov.l @r2,r1 */
L8c616f90: r[2]=rd(m,bus,&io_status,0x8c616fd0u,4);if(io_status)return io_status;goto L8c616f92; /* mov.l 0x8c616fd0,r2 */
L8c616f92: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c616f94; /* fmov.s @(r0,r1),fr2 */
L8c616f94: r[0]=4u;goto L8c616f96; /* mov #0x4,r0 */
L8c616f96: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616f98; /* fsub fr3,fr2 */
L8c616f98: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c616f9a; /* fmov.s @r2,fr3 */
L8c616f9a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616f9c; /* fmul fr5,fr2 */
L8c616f9c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616f9e; /* fdiv fr3,fr2 */
L8c616f9e: fstore(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c616fa0; /* fmov fr2,@(r0,r5) */
L8c616fa0: r[0]=8u;goto L8c616fa2; /* mov #0x8,r0 */
L8c616fa2: fstore(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c616ff0; /* bra 0x8c616ff0 */
L8c616fd4: r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c616fd6; /* mov.l @r13,r3 */
L8c616fd6: r[0]=16u;goto L8c616fd8; /* mov #0x10,r0 */
L8c616fd8: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616fda; /* fmov.s @(r0,r3),fr3 */
L8c616fda: r[0]=20u;goto L8c616fdc; /* mov #0x14,r0 */
L8c616fdc: fstore(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c616fde; /* fmov.s fr3,@r6 */
L8c616fde: r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c616fe0; /* mov.l @r13,r3 */
L8c616fe0: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616fe2; /* fmov.s @(r0,r3),fr3 */
L8c616fe2: r[0]=4u;goto L8c616fe4; /* mov #0x4,r0 */
L8c616fe4: fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c616fe6; /* fmov fr3,@(r0,r6) */
L8c616fe6: r[0]=24u;goto L8c616fe8; /* mov #0x18,r0 */
L8c616fe8: r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c616fea; /* mov.l @r13,r3 */
L8c616fea: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616fec; /* fmov.s @(r0,r3),fr3 */
L8c616fec: r[0]=8u;goto L8c616fee; /* mov #0x8,r0 */
L8c616fee: fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c616ff0; /* fmov fr3,@(r0,r6) */
L8c616ff0: r[3]=3u;goto L8c616ff2; /* mov #0x3,r3 */
L8c616ff2: r[11]+=1u;goto L8c616ff4; /* add #0x1,r11 */
L8c616ff4: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[3]);goto L8c616ff6; /* cmp/ge r3,r11 */
L8c616ff6: r[13]+=4u;goto L8c616ff8; /* add #0x4,r13 */
L8c616ff8: r[6]+=12u;goto L8c616ffa; /* add #0xc,r6 */
L8c616ffa: take=!(*sr&1u);r[5]+=12u;if(take)goto L8c616f52;goto L8c616ffe; /* bf/s 0x8c616f52 */
L8c616ffe: r[0]=8u;goto L8c617000; /* mov #0x8,r0 */
L8c617000: r[6]=r[9];goto L8c617002; /* mov r9,r6 */
L8c617002: fload(m,bus,&io_status,f,8u,r[0]+r[10]);if(io_status)return io_status;goto L8c617004; /* fmov.s @(r0,r10),fr8 */
L8c617004: r[6]+=12u;goto L8c617006; /* add #0xc,r6 */
L8c617006: r[0]=4u;goto L8c617008; /* mov #0x4,r0 */
L8c617008: fload(m,bus,&io_status,f,11u,r[10]);if(io_status)return io_status;goto L8c61700a; /* fmov.s @r10,fr11 */
L8c61700a: fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c61700c; /* fmov.s @(r0,r6),fr5 */
L8c61700c: r[5]=r[9];goto L8c61700e; /* mov r9,r5 */
L8c61700e: fload(m,bus,&io_status,f,10u,r[0]+r[10]);if(io_status)return io_status;goto L8c617010; /* fmov.s @(r0,r10),fr10 */
L8c617010: r[0]=8u;goto L8c617012; /* mov #0x8,r0 */
L8c617012: fload(m,bus,&io_status,f,7u,r[0]+r[6]);if(io_status)return io_status;goto L8c617014; /* fmov.s @(r0,r6),fr7 */
L8c617014: r[0]=4u;goto L8c617016; /* mov #0x4,r0 */
L8c617016: fcopy(f,3u,10u);goto L8c617018; /* fmov fr10,fr3 */
L8c617018: r[5]+=24u;goto L8c61701a; /* add #0x18,r5 */
L8c61701a: fcopy(f,0u,8u);goto L8c61701c; /* fmov fr8,fr0 */
L8c61701c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61701e; /* fmul fr5,fr0 */
L8c61701e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617020; /* fmul fr7,fr3 */
L8c617020: fload(m,bus,&io_status,f,9u,r[6]);if(io_status)return io_status;goto L8c617022; /* fmov.s @r6,fr9 */
L8c617022: fcopy(f,2u,11u);goto L8c617024; /* fmov fr11,fr2 */
L8c617024: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617026; /* fmul fr7,fr2 */
L8c617026: fcopy(f,12u,11u);goto L8c617028; /* fmov fr11,fr12 */
L8c617028: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[5],&f->fpscr,&fr[12]);if(status)return status;}goto L8c61702a; /* fmul fr5,fr12 */
L8c61702a: fcopy(f,1u,9u);goto L8c61702c; /* fmov fr9,fr1 */
L8c61702c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[10],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61702e; /* fmul fr10,fr1 */
L8c61702e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c617030; /* fsub fr3,fr0 */
L8c617030: fcopy(f,3u,8u);goto L8c617032; /* fmov fr8,fr3 */
L8c617032: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617034; /* fmul fr9,fr3 */
L8c617034: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c617036; /* fsub fr3,fr2 */
L8c617036: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617038; /* fmov.s @(r0,r5),fr3 */
L8c617038: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61703a; /* fmul fr2,fr3 */
L8c61703a: fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c61703c; /* fmov.s @r5,fr2 */
L8c61703c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61703e; /* fmac fr0,fr2,fr3 */
L8c61703e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[12],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c617040; /* fsub fr12,fr1 */
L8c617040: r[0]=8u;goto L8c617042; /* mov #0x8,r0 */
L8c617042: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c617044; /* fmov.s @(r0,r5),fr2 */
L8c617044: r[5]=rd(m,bus,&io_status,0x8c617294u,4);if(io_status)return io_status;goto L8c617046; /* mov.l 0x8c617294,r5 */
L8c617046: fcopy(f,0u,1u);goto L8c617048; /* fmov fr1,fr0 */
L8c617048: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61704a; /* fmac fr0,fr2,fr3 */
L8c61704a: r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c61704c; /* mov.l @r5,r3 */
L8c61704c: fcopy(f,5u,3u);goto L8c61704e; /* fmov fr3,fr5 */
L8c61704e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c617050; /* fldi0 fr3 */
L8c617050: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c617052; /* fcmp/gt fr5,fr3 */
L8c617052: r[0]=*sr&1u;goto L8c617054; /* movt r0 */
L8c617054: *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c617056; /* tst r3,r0 */
L8c617056: if((*sr&1u))goto L8c61705c;goto L8c617058; /* bt 0x8c61705c */
L8c617058: goto L8c6173a8; /* bra 0x8c6173a8 */
L8c61705c: r[1]=0u;goto L8c61705e; /* mov #0x0,r1 */
L8c61705e: r[6]=rd(m,bus,&io_status,0x8c617298u,4);if(io_status)return io_status;goto L8c617060; /* mov.l 0x8c617298,r6 */
L8c617060: wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c617062; /* mov.l r1,@r5 */
L8c617062: r[3]=2u;goto L8c617064; /* mov #0x2,r3 */
L8c617064: r[5]=rd(m,bus,&io_status,0x8c61729cu,4);if(io_status)return io_status;goto L8c617066; /* mov.l 0x8c61729c,r5 */
L8c617066: r[13]=r[1];goto L8c617068; /* mov r1,r13 */
L8c617068: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61706a; /* mov.l r3,@r15 */
L8c61706a: r[10]=r[13];goto L8c61706c; /* mov r13,r10 */
L8c61706c: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61706e; /* mov.l @r15,r3 */
L8c61706e: r[0]=r[14];goto L8c617070; /* mov r14,r0 */
L8c617070: r[11]=r[10];goto L8c617072; /* mov r10,r11 */
L8c617072: r[11]<<=2;goto L8c617074; /* shll2 r11 */
L8c617074: r[3]<<=2;goto L8c617076; /* shll2 r3 */
L8c617076: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c617078; /* mov.l r3,@(0x4,r15) */
L8c617078: r[2]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c61707a; /* mov.l @(r0,r3),r2 */
L8c61707a: r[3]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c61707c; /* mov.l @(0xc,r2),r3 */
L8c61707c: r[2]=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;goto L8c61707e; /* mov.l @(r0,r11),r2 */
L8c61707e: r[3]&=r[4];goto L8c617080; /* and r4,r3 */
L8c617080: if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[2]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c617082; /* mov.l @(0xc,r2),r2 */
L8c617082: r[2]&=r[4];goto L8c617084; /* and r4,r2 */
L8c617084: r[3]^=r[2];goto L8c617086; /* xor r2,r3 */
L8c617086: r[0]=r[3];goto L8c617088; /* mov r3,r0 */
L8c617088: *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c61708a; /* cmp/eq #0x1,r0 */
L8c61708a: if(!(*sr&1u))goto L8c617170;goto L8c61708c; /* bf 0x8c617170 */
L8c61708c: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61708e; /* mov.l @r15,r2 */
L8c61708e: r[0]=8u;goto L8c617090; /* mov #0x8,r0 */
L8c617090: r[1]=r[10];goto L8c617092; /* mov r10,r1 */
L8c617092: r[3]=r[2];goto L8c617094; /* mov r2,r3 */
L8c617094: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c617096; /* shll r2 */
L8c617096: r[2]+=r[3];goto L8c617098; /* add r3,r2 */
L8c617098: r[3]=rd(m,bus,&io_status,0x8c6172a0u,4);if(io_status)return io_status;goto L8c61709a; /* mov.l 0x8c6172a0,r3 */
L8c61709a: r[2]<<=2;goto L8c61709c; /* shll2 r2 */
L8c61709c: r[2]+=r[9];goto L8c61709e; /* add r9,r2 */
L8c61709e: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6170a0; /* mov.l r2,@(0x8,r15) */
L8c6170a0: fload(m,bus,&io_status,f,5u,r[0]+r[2]);if(io_status)return io_status;goto L8c6170a2; /* fmov.s @(r0,r2),fr5 */
L8c6170a2: r[2]=r[10];goto L8c6170a4; /* mov r10,r2 */
L8c6170a4: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6170a6; /* shll r2 */
L8c6170a6: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6170a8; /* fmov.s @r3,fr3 */
L8c6170a8: r[2]+=r[1];goto L8c6170aa; /* add r1,r2 */
L8c6170aa: r[2]<<=2;goto L8c6170ac; /* shll2 r2 */
L8c6170ac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6170ae; /* fsub fr5,fr3 */
L8c6170ae: r[0]=8u;goto L8c6170b0; /* mov #0x8,r0 */
L8c6170b0: r[2]+=r[9];goto L8c6170b2; /* add r9,r2 */
L8c6170b2: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6170b4; /* mov.l r2,@(0xc,r15) */
L8c6170b4: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6170b6; /* fmov.s @(r0,r2),fr2 */
L8c6170b6: r[1]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6170b8; /* mov.l @(0xc,r15),r1 */
L8c6170b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6170ba; /* fsub fr5,fr2 */
L8c6170ba: fcopy(f,5u,3u);goto L8c6170bc; /* fmov fr3,fr5 */
L8c6170bc: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6170be; /* mov.l @(0x8,r15),r2 */
L8c6170be: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6170c0; /* fmov.s @r1,fr3 */
L8c6170c0: fload(m,bus,&io_status,f,7u,r[2]);if(io_status)return io_status;goto L8c6170c2; /* fmov.s @r2,fr7 */
L8c6170c2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[2],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6170c4; /* fdiv fr2,fr5 */
L8c6170c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6170c6; /* fsub fr7,fr3 */
L8c6170c6: fcopy(f,2u,7u);goto L8c6170c8; /* fmov fr7,fr2 */
L8c6170c8: fcopy(f,0u,5u);goto L8c6170ca; /* fmov fr5,fr0 */
L8c6170ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6170cc; /* fmac fr0,fr3,fr2 */
L8c6170cc: r[1]=rd(m,bus,&io_status,0x8c6172a4u,4);if(io_status)return io_status;goto L8c6170ce; /* mov.l 0x8c6172a4,r1 */
L8c6170ce: r[0]=12u;goto L8c6170d0; /* mov #0xc,r0 */
L8c6170d0: fcopy(f,7u,2u);goto L8c6170d2; /* fmov fr2,fr7 */
L8c6170d2: r[2]=r[13];goto L8c6170d4; /* mov r13,r2 */
L8c6170d4: fcopy(f,0u,6u);goto L8c6170d6; /* fmov fr6,fr0 */
L8c6170d6: r[2]<<=2;goto L8c6170d8; /* shll2 r2 */
L8c6170d8: r[2]<<=2;goto L8c6170da; /* shll2 r2 */
L8c6170da: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6170dc; /* shll r2 */
L8c6170dc: r[2]+=r[6];goto L8c6170de; /* add r6,r2 */
L8c6170de: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6170e0; /* mov.l r2,@r15 */
L8c6170e0: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6170e2; /* fmov.s @r1,fr3 */
L8c6170e2: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6170e4; /* fmov.s @(r0,r12),fr2 */
L8c6170e4: r[0]=16u;goto L8c6170e6; /* mov #0x10,r0 */
L8c6170e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6170e8; /* fmul fr7,fr3 */
L8c6170e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6170ea; /* fmac fr0,fr3,fr2 */
L8c6170ea: fcopy(f,0u,5u);goto L8c6170ec; /* fmov fr5,fr0 */
L8c6170ec: fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6170ee; /* fmov fr2,@(r0,r2) */
L8c6170ee: r[0]=4u;goto L8c6170f0; /* mov #0x4,r0 */
L8c6170f0: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6170f2; /* mov.l @(0x8,r15),r2 */
L8c6170f2: fload(m,bus,&io_status,f,7u,r[0]+r[2]);if(io_status)return io_status;goto L8c6170f4; /* fmov.s @(r0,r2),fr7 */
L8c6170f4: r[0]=4u;goto L8c6170f6; /* mov #0x4,r0 */
L8c6170f6: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6170f8; /* mov.l @(0xc,r15),r2 */
L8c6170f8: fcopy(f,2u,7u);goto L8c6170fa; /* fmov fr7,fr2 */
L8c6170fa: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c6170fc; /* fmov.s @(r0,r2),fr3 */
L8c6170fc: r[0]=rd(m,bus,&io_status,0x8c6172a8u,4);if(io_status)return io_status;goto L8c6170fe; /* mov.l 0x8c6172a8,r0 */
L8c6170fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617100; /* fsub fr7,fr3 */
L8c617100: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617102; /* mov.l @r15,r2 */
L8c617102: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617104; /* fmac fr0,fr3,fr2 */
L8c617104: fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c617106; /* fmov.s @r0,fr3 */
L8c617106: r[0]=16u;goto L8c617108; /* mov #0x10,r0 */
L8c617108: fcopy(f,7u,2u);goto L8c61710a; /* fmov fr2,fr7 */
L8c61710a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61710c; /* fmul fr7,fr3 */
L8c61710c: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61710e; /* fmov.s @(r0,r12),fr2 */
L8c61710e: r[0]=20u;goto L8c617110; /* mov #0x14,r0 */
L8c617110: fcopy(f,0u,6u);goto L8c617112; /* fmov fr6,fr0 */
L8c617112: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617114; /* fmac fr0,fr3,fr2 */
L8c617114: fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c617116; /* fmov fr2,@(r0,r2) */
L8c617116: r[0]=24u;goto L8c617118; /* mov #0x18,r0 */
L8c617118: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61711a; /* mov.l @r15,r3 */
L8c61711a: r[2]=r[11];goto L8c61711c; /* mov r11,r2 */
L8c61711c: r[2]+=r[7];goto L8c61711e; /* add r7,r2 */
L8c61711e: fstore(m,bus,&io_status,f,0u,r[0]+r[3]);if(io_status)return io_status;goto L8c617120; /* fmov fr0,@(r0,r3) */
L8c617120: r[3]=r[13];goto L8c617122; /* mov r13,r3 */
L8c617122: fcopy(f,0u,5u);goto L8c617124; /* fmov fr5,fr0 */
L8c617124: r[3]<<=2;goto L8c617126; /* shll2 r3 */
L8c617126: r[3]+=r[5];goto L8c617128; /* add r5,r3 */
L8c617128: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c61712a; /* mov.l r3,@(0x8,r15) */
L8c61712a: r[0]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61712c; /* mov.l @(0x4,r15),r0 */
L8c61712c: r[0]+=r[7];goto L8c61712e; /* add r7,r0 */
L8c61712e: wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c617130; /* mov.l r0,@(0xc,r15) */
L8c617130: r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c617132; /* mov.w @r0,r0 */
L8c617132: wr(m,bus,&io_status,r[15]+0x10u,r[0],2);if(io_status)return io_status;goto L8c617134; /* mov.w r0,@(0x10,r15) */
L8c617134: *fpul=r[0];goto L8c617136; /* lds r0,FPUL */
L8c617136: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c617138; /* mov.l r2,@r15 */
L8c617138: r[0]=sx(m,bus,&io_status,r[15]+0x10u,2);if(io_status)return io_status;goto L8c61713a; /* mov.w @(0x10,r15),r0 */
L8c61713a: r[2]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c61713c; /* mov.w @r2,r2 */
L8c61713c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61713e; /* float FPUL,fr3 */
L8c61713e: r[2]-=r[0];goto L8c617140; /* sub r0,r2 */
L8c617140: *fpul=r[2];goto L8c617142; /* lds r2,FPUL */
L8c617142: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c617144; /* float FPUL,fr2 */
L8c617144: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617146; /* fmac fr0,fr2,fr3 */
L8c617146: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c617148; /* ftrc fr3,FPUL */
L8c617148: r[2]=*fpul;goto L8c61714a; /* sts FPUL,r2 */
L8c61714a: wr(m,bus,&io_status,r[3],r[2],2);if(io_status)return io_status;goto L8c61714c; /* mov.w r2,@r3 */
L8c61714c: r[0]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61714e; /* mov.l @(0xc,r15),r0 */
L8c61714e: r[13]+=1u;goto L8c617150; /* add #0x1,r13 */
L8c617150: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c617152; /* mov.l @(0x8,r15),r3 */
L8c617152: r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c617154; /* mov.w @(0x2,r0),r0 */
L8c617154: *fpul=r[0];goto L8c617156; /* lds r0,FPUL */
L8c617156: wr(m,bus,&io_status,r[15]+0x4u,r[0],2);if(io_status)return io_status;goto L8c617158; /* mov.w r0,@(0x4,r15) */
L8c617158: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61715a; /* mov.l @r15,r2 */
L8c61715a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61715c; /* float FPUL,fr3 */
L8c61715c: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61715e; /* mov.w @(0x2,r2),r0 */
L8c61715e: r[2]=r[0];goto L8c617160; /* mov r0,r2 */
L8c617160: r[0]=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;goto L8c617162; /* mov.w @(0x4,r15),r0 */
L8c617162: r[2]-=r[0];goto L8c617164; /* sub r0,r2 */
L8c617164: *fpul=r[2];goto L8c617166; /* lds r2,FPUL */
L8c617166: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c617168; /* float FPUL,fr2 */
L8c617168: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61716a; /* fmac fr0,fr2,fr3 */
L8c61716a: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61716c; /* ftrc fr3,FPUL */
L8c61716c: r[0]=*fpul;goto L8c61716e; /* sts FPUL,r0 */
L8c61716e: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c617170; /* mov.w r0,@(0x2,r3) */
L8c617170: r[0]=r[11];goto L8c617172; /* mov r11,r0 */
L8c617172: r[3]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c617174; /* mov.l @(r0,r14),r3 */
L8c617174: r[3]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c617176; /* mov.l @(0xc,r3),r3 */
L8c617176: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c617178; /* tst r4,r3 */
L8c617178: if(!(*sr&1u))goto L8c6171c8;goto L8c61717a; /* bf 0x8c6171c8 */
L8c61717a: r[3]=r[11];goto L8c61717c; /* mov r11,r3 */
L8c61717c: r[2]=r[13];goto L8c61717e; /* mov r13,r2 */
L8c61717e: r[3]+=r[14];goto L8c617180; /* add r14,r3 */
L8c617180: r[2]<<=2;goto L8c617182; /* shll2 r2 */
L8c617182: r[0]=16u;goto L8c617184; /* mov #0x10,r0 */
L8c617184: r[2]<<=2;goto L8c617186; /* shll2 r2 */
L8c617186: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c617188; /* shll r2 */
L8c617188: r[2]+=r[6];goto L8c61718a; /* add r6,r2 */
L8c61718a: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61718c; /* mov.l r2,@r15 */
L8c61718c: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61718e; /* mov.l r3,@(0x4,r15) */
L8c61718e: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c617190; /* mov.l @r3,r1 */
L8c617190: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c617192; /* fmov.s @(r0,r1),fr3 */
L8c617192: fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c617194; /* fmov fr3,@(r0,r2) */
L8c617194: r[0]=20u;goto L8c617196; /* mov #0x14,r0 */
L8c617196: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c617198; /* mov.l @(0x4,r15),r2 */
L8c617198: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61719a; /* mov.l @r15,r3 */
L8c61719a: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61719c; /* mov.l @r2,r1 */
L8c61719c: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61719e; /* fmov.s @(r0,r1),fr3 */
L8c61719e: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6171a0; /* fmov fr3,@(r0,r3) */
L8c6171a0: r[0]=24u;goto L8c6171a2; /* mov #0x18,r0 */
L8c6171a2: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6171a4; /* mov.l @(0x4,r15),r2 */
L8c6171a4: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6171a6; /* mov.l @r15,r3 */
L8c6171a6: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6171a8; /* mov.l @r2,r1 */
L8c6171a8: r[2]=r[11];goto L8c6171aa; /* mov r11,r2 */
L8c6171aa: r[2]+=r[7];goto L8c6171ac; /* add r7,r2 */
L8c6171ac: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6171ae; /* fmov.s @(r0,r1),fr3 */
L8c6171ae: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6171b0; /* fmov fr3,@(r0,r3) */
L8c6171b0: r[3]=r[13];goto L8c6171b2; /* mov r13,r3 */
L8c6171b2: r[3]<<=2;goto L8c6171b4; /* shll2 r3 */
L8c6171b4: r[3]+=r[5];goto L8c6171b6; /* add r5,r3 */
L8c6171b6: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6171b8; /* mov.l r3,@r15 */
L8c6171b8: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6171ba; /* mov.l r2,@(0x4,r15) */
L8c6171ba: r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c6171bc; /* mov.w @r2,r1 */
L8c6171bc: r[13]+=1u;goto L8c6171be; /* add #0x1,r13 */
L8c6171be: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c6171c0; /* mov.w r1,@r3 */
L8c6171c0: r[0]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6171c2; /* mov.l @(0x4,r15),r0 */
L8c6171c2: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6171c4; /* mov.l @r15,r3 */
L8c6171c4: r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c6171c6; /* mov.w @(0x2,r0),r0 */
L8c6171c6: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6171c8; /* mov.w r0,@(0x2,r3) */
L8c6171c8: wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6171ca; /* mov.l r10,@r15 */
L8c6171ca: r[3]=3u;goto L8c6171cc; /* mov #0x3,r3 */
L8c6171cc: r[10]+=1u;goto L8c6171ce; /* add #0x1,r10 */
L8c6171ce: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[3]);goto L8c6171d0; /* cmp/ge r3,r10 */
L8c6171d0: if((*sr&1u))goto L8c6171d6;goto L8c6171d2; /* bt 0x8c6171d6 */
L8c6171d2: goto L8c61706c; /* bra 0x8c61706c */
L8c6171d6: r[0]=0x8c6172b4u;goto L8c6171d8; /* mova 0x8c6172b4,r0 */
L8c6171d8: r[4]=rd(m,bus,&io_status,0x8c6172b0u,4);if(io_status)return io_status;goto L8c6171da; /* mov.l 0x8c6172b0,r4 */
L8c6171da: fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c6171dc; /* fmov.s @r0,fr5 */
L8c6171dc: r[0]=r[13];goto L8c6171de; /* mov r13,r0 */
L8c6171de: r[7]=rd(m,bus,&io_status,0x8c6172acu,4);if(io_status)return io_status;goto L8c6171e0; /* mov.l 0x8c6172ac,r7 */
L8c6171e0: *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c6171e2; /* cmp/eq #0x3,r0 */
L8c6171e2: if(!(*sr&1u))goto L8c6172bc;goto L8c6171e4; /* bf 0x8c6172bc */
L8c6171e4: r[3]=r[5];goto L8c6171e6; /* mov r5,r3 */
L8c6171e6: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6171e8; /* mov.l r3,@r15 */
L8c6171e8: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6171ea; /* mov.w @(0x2,r3),r0 */
L8c6171ea: r[8]+=24u;goto L8c6171ec; /* add #0x18,r8 */
L8c6171ec: r[14]=r[6];goto L8c6171ee; /* mov r6,r14 */
L8c6171ee: r[3]=r[0];goto L8c6171f0; /* mov r0,r3 */
L8c6171f0: *fpul=r[3];goto L8c6171f2; /* lds r3,FPUL */
L8c6171f2: r[0]=24u;goto L8c6171f4; /* mov #0x18,r0 */
L8c6171f4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6171f6; /* float FPUL,fr3 */
L8c6171f6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6171f8; /* fmul fr5,fr3 */
L8c6171f8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6171fa; /* fmov.s fr3,@-r8 */
L8c6171fa: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6171fc; /* mov.l @r15,r3 */
L8c6171fc: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6171fe; /* mov.w @r3,r3 */
L8c6171fe: *fpul=r[3];goto L8c617200; /* lds r3,FPUL */
L8c617200: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617202; /* float FPUL,fr3 */
L8c617202: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617204; /* fmul fr5,fr3 */
L8c617204: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617206; /* fmov.s fr3,@-r8 */
L8c617206: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617208; /* fmov.s @(r0,r14),fr3 */
L8c617208: r[0]=20u;goto L8c61720a; /* mov #0x14,r0 */
L8c61720a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61720c; /* fmov.s fr3,@-r8 */
L8c61720c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61720e; /* fmov.s @(r0,r14),fr3 */
L8c61720e: r[0]=16u;goto L8c617210; /* mov #0x10,r0 */
L8c617210: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617212; /* fmov.s fr3,@-r8 */
L8c617212: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617214; /* fmov.s @(r0,r14),fr3 */
L8c617214: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617216; /* fmov.s fr3,@-r8 */
L8c617216: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c617218; /* mov.l r4,@-r8 */
L8c617218: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61721a; /* pref @r8 */
L8c61721a: r[3]=r[5];goto L8c61721c; /* mov r5,r3 */
L8c61721c: r[3]+=4u;goto L8c61721e; /* add #0x4,r3 */
L8c61721e: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c617220; /* mov.l r3,@r15 */
L8c617220: r[8]+=56u;goto L8c617222; /* add #0x38,r8 */
L8c617222: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c617224; /* mov.w @(0x2,r3),r0 */
L8c617224: r[14]+=32u;goto L8c617226; /* add #0x20,r14 */
L8c617226: r[3]=r[0];goto L8c617228; /* mov r0,r3 */
L8c617228: *fpul=r[3];goto L8c61722a; /* lds r3,FPUL */
L8c61722a: r[0]=24u;goto L8c61722c; /* mov #0x18,r0 */
L8c61722c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61722e; /* float FPUL,fr3 */
L8c61722e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617230; /* fmul fr5,fr3 */
L8c617230: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617232; /* fmov.s fr3,@-r8 */
L8c617232: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617234; /* mov.l @r15,r3 */
L8c617234: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c617236; /* mov.w @r3,r3 */
L8c617236: *fpul=r[3];goto L8c617238; /* lds r3,FPUL */
L8c617238: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61723a; /* float FPUL,fr3 */
L8c61723a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61723c; /* fmul fr5,fr3 */
L8c61723c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61723e; /* fmov.s fr3,@-r8 */
L8c61723e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617240; /* fmov.s @(r0,r14),fr3 */
L8c617240: r[0]=20u;goto L8c617242; /* mov #0x14,r0 */
L8c617242: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617244; /* fmov.s fr3,@-r8 */
L8c617244: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617246; /* fmov.s @(r0,r14),fr3 */
L8c617246: r[0]=16u;goto L8c617248; /* mov #0x10,r0 */
L8c617248: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61724a; /* fmov.s fr3,@-r8 */
L8c61724a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61724c; /* fmov.s @(r0,r14),fr3 */
L8c61724c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61724e; /* fmov.s fr3,@-r8 */
L8c61724e: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c617250; /* mov.l r4,@-r8 */
L8c617250: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617252; /* pref @r8 */
L8c617252: r[4]=r[5];goto L8c617254; /* mov r5,r4 */
L8c617254: r[4]+=8u;goto L8c617256; /* add #0x8,r4 */
L8c617256: r[8]+=60u;goto L8c617258; /* add #0x3c,r8 */
L8c617258: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61725a; /* fmov.s fr4,@-r8 */
L8c61725a: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61725c; /* mov.w @(0x2,r4),r0 */
L8c61725c: r[3]=r[0];goto L8c61725e; /* mov r0,r3 */
L8c61725e: *fpul=r[3];goto L8c617260; /* lds r3,FPUL */
L8c617260: r[0]=24u;goto L8c617262; /* mov #0x18,r0 */
L8c617262: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617264; /* float FPUL,fr3 */
L8c617264: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617266; /* fmul fr5,fr3 */
L8c617266: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617268; /* fmov.s fr3,@-r8 */
L8c617268: r[3]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61726a; /* mov.w @r4,r3 */
L8c61726a: r[4]=r[6];goto L8c61726c; /* mov r6,r4 */
L8c61726c: r[4]+=64u;goto L8c61726e; /* add #0x40,r4 */
L8c61726e: *fpul=r[3];goto L8c617270; /* lds r3,FPUL */
L8c617270: r[3]=rd(m,bus,&io_status,0x8c6172b8u,4);if(io_status)return io_status;goto L8c617272; /* mov.l 0x8c6172b8,r3 */
L8c617272: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617274; /* float FPUL,fr3 */
L8c617274: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617276; /* fmul fr5,fr3 */
L8c617276: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617278; /* fmov.s fr3,@-r8 */
L8c617278: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61727a; /* fmov.s @(r0,r4),fr3 */
L8c61727a: r[0]=20u;goto L8c61727c; /* mov #0x14,r0 */
L8c61727c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61727e; /* fmov.s fr3,@-r8 */
L8c61727e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c617280; /* fmov.s @(r0,r4),fr3 */
L8c617280: r[0]=16u;goto L8c617282; /* mov #0x10,r0 */
L8c617282: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617284; /* fmov.s fr3,@-r8 */
L8c617284: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c617286; /* fmov.s @(r0,r4),fr3 */
L8c617286: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617288; /* fmov.s fr3,@-r8 */
L8c617288: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61728a; /* mov.l r3,@-r8 */
L8c61728a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61728c; /* pref @r8 */
L8c61728c: r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c61728e; /* mov.l @r7,r2 */
L8c61728e: r[8]+=32u;goto L8c617290; /* add #0x20,r8 */
L8c617290: r[2]+=3u;goto L8c6173a6; /* bra 0x8c6173a6 */
L8c6172bc: r[3]=r[5];goto L8c6172be; /* mov r5,r3 */
L8c6172be: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6172c0; /* mov.l r3,@r15 */
L8c6172c0: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6172c2; /* mov.w @(0x2,r3),r0 */
L8c6172c2: r[8]+=24u;goto L8c6172c4; /* add #0x18,r8 */
L8c6172c4: r[14]=r[6];goto L8c6172c6; /* mov r6,r14 */
L8c6172c6: r[3]=r[0];goto L8c6172c8; /* mov r0,r3 */
L8c6172c8: *fpul=r[3];goto L8c6172ca; /* lds r3,FPUL */
L8c6172ca: r[0]=24u;goto L8c6172cc; /* mov #0x18,r0 */
L8c6172cc: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6172ce; /* float FPUL,fr3 */
L8c6172ce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6172d0; /* fmul fr5,fr3 */
L8c6172d0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6172d2; /* fmov.s fr3,@-r8 */
L8c6172d2: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6172d4; /* mov.l @r15,r3 */
L8c6172d4: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6172d6; /* mov.w @r3,r3 */
L8c6172d6: *fpul=r[3];goto L8c6172d8; /* lds r3,FPUL */
L8c6172d8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6172da; /* float FPUL,fr3 */
L8c6172da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6172dc; /* fmul fr5,fr3 */
L8c6172dc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6172de; /* fmov.s fr3,@-r8 */
L8c6172de: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6172e0; /* fmov.s @(r0,r14),fr3 */
L8c6172e0: r[0]=20u;goto L8c6172e2; /* mov #0x14,r0 */
L8c6172e2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6172e4; /* fmov.s fr3,@-r8 */
L8c6172e4: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6172e6; /* fmov.s @(r0,r14),fr3 */
L8c6172e6: r[0]=16u;goto L8c6172e8; /* mov #0x10,r0 */
L8c6172e8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6172ea; /* fmov.s fr3,@-r8 */
L8c6172ea: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6172ec; /* fmov.s @(r0,r14),fr3 */
L8c6172ec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6172ee; /* fmov.s fr3,@-r8 */
L8c6172ee: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c6172f0; /* mov.l r4,@-r8 */
L8c6172f0: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6172f2; /* pref @r8 */
L8c6172f2: r[3]=r[5];goto L8c6172f4; /* mov r5,r3 */
L8c6172f4: r[3]+=4u;goto L8c6172f6; /* add #0x4,r3 */
L8c6172f6: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6172f8; /* mov.l r3,@r15 */
L8c6172f8: r[8]+=56u;goto L8c6172fa; /* add #0x38,r8 */
L8c6172fa: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6172fc; /* mov.w @(0x2,r3),r0 */
L8c6172fc: r[14]+=32u;goto L8c6172fe; /* add #0x20,r14 */
L8c6172fe: r[3]=r[0];goto L8c617300; /* mov r0,r3 */
L8c617300: *fpul=r[3];goto L8c617302; /* lds r3,FPUL */
L8c617302: r[0]=24u;goto L8c617304; /* mov #0x18,r0 */
L8c617304: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617306; /* float FPUL,fr3 */
L8c617306: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617308; /* fmul fr5,fr3 */
L8c617308: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61730a; /* fmov.s fr3,@-r8 */
L8c61730a: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61730c; /* mov.l @r15,r3 */
L8c61730c: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c61730e; /* mov.w @r3,r3 */
L8c61730e: *fpul=r[3];goto L8c617310; /* lds r3,FPUL */
L8c617310: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617312; /* float FPUL,fr3 */
L8c617312: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617314; /* fmul fr5,fr3 */
L8c617314: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617316; /* fmov.s fr3,@-r8 */
L8c617316: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617318; /* fmov.s @(r0,r14),fr3 */
L8c617318: r[0]=20u;goto L8c61731a; /* mov #0x14,r0 */
L8c61731a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61731c; /* fmov.s fr3,@-r8 */
L8c61731c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61731e; /* fmov.s @(r0,r14),fr3 */
L8c61731e: r[0]=16u;goto L8c617320; /* mov #0x10,r0 */
L8c617320: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617322; /* fmov.s fr3,@-r8 */
L8c617322: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617324; /* fmov.s @(r0,r14),fr3 */
L8c617324: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617326; /* fmov.s fr3,@-r8 */
L8c617326: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c617328; /* mov.l r4,@-r8 */
L8c617328: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61732a; /* pref @r8 */
L8c61732a: r[3]=r[5];goto L8c61732c; /* mov r5,r3 */
L8c61732c: r[3]+=12u;goto L8c61732e; /* add #0xc,r3 */
L8c61732e: r[8]+=60u;goto L8c617330; /* add #0x3c,r8 */
L8c617330: r[14]=r[6];goto L8c617332; /* mov r6,r14 */
L8c617332: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c617334; /* fmov.s fr4,@-r8 */
L8c617334: r[14]+=96u;goto L8c617336; /* add #0x60,r14 */
L8c617336: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c617338; /* mov.l r3,@r15 */
L8c617338: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61733a; /* mov.w @(0x2,r3),r0 */
L8c61733a: r[3]=r[0];goto L8c61733c; /* mov r0,r3 */
L8c61733c: *fpul=r[3];goto L8c61733e; /* lds r3,FPUL */
L8c61733e: r[0]=24u;goto L8c617340; /* mov #0x18,r0 */
L8c617340: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617342; /* float FPUL,fr3 */
L8c617342: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617344; /* fmul fr5,fr3 */
L8c617344: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617346; /* fmov.s fr3,@-r8 */
L8c617346: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617348; /* mov.l @r15,r3 */
L8c617348: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c61734a; /* mov.w @r3,r3 */
L8c61734a: *fpul=r[3];goto L8c61734c; /* lds r3,FPUL */
L8c61734c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61734e; /* float FPUL,fr3 */
L8c61734e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617350; /* fmul fr5,fr3 */
L8c617350: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617352; /* fmov.s fr3,@-r8 */
L8c617352: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617354; /* fmov.s @(r0,r14),fr3 */
L8c617354: r[0]=20u;goto L8c617356; /* mov #0x14,r0 */
L8c617356: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617358; /* fmov.s fr3,@-r8 */
L8c617358: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61735a; /* fmov.s @(r0,r14),fr3 */
L8c61735a: r[0]=16u;goto L8c61735c; /* mov #0x10,r0 */
L8c61735c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61735e; /* fmov.s fr3,@-r8 */
L8c61735e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617360; /* fmov.s @(r0,r14),fr3 */
L8c617360: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617362; /* fmov.s fr3,@-r8 */
L8c617362: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c617364; /* mov.l r4,@-r8 */
L8c617364: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617366; /* pref @r8 */
L8c617366: r[4]=r[5];goto L8c617368; /* mov r5,r4 */
L8c617368: r[4]+=8u;goto L8c61736a; /* add #0x8,r4 */
L8c61736a: r[8]+=60u;goto L8c61736c; /* add #0x3c,r8 */
L8c61736c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61736e; /* fmov.s fr4,@-r8 */
L8c61736e: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c617370; /* mov.w @(0x2,r4),r0 */
L8c617370: r[3]=r[0];goto L8c617372; /* mov r0,r3 */
L8c617372: *fpul=r[3];goto L8c617374; /* lds r3,FPUL */
L8c617374: r[0]=24u;goto L8c617376; /* mov #0x18,r0 */
L8c617376: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617378; /* float FPUL,fr3 */
L8c617378: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61737a; /* fmul fr5,fr3 */
L8c61737a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61737c; /* fmov.s fr3,@-r8 */
L8c61737c: r[3]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61737e; /* mov.w @r4,r3 */
L8c61737e: r[4]=r[6];goto L8c617380; /* mov r6,r4 */
L8c617380: r[4]+=64u;goto L8c617382; /* add #0x40,r4 */
L8c617382: *fpul=r[3];goto L8c617384; /* lds r3,FPUL */
L8c617384: r[3]=rd(m,bus,&io_status,0x8c6174bcu,4);if(io_status)return io_status;goto L8c617386; /* mov.l 0x8c6174bc,r3 */
L8c617386: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617388; /* float FPUL,fr3 */
L8c617388: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61738a; /* fmul fr5,fr3 */
L8c61738a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61738c; /* fmov.s fr3,@-r8 */
L8c61738c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61738e; /* fmov.s @(r0,r4),fr3 */
L8c61738e: r[0]=20u;goto L8c617390; /* mov #0x14,r0 */
L8c617390: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617392; /* fmov.s fr3,@-r8 */
L8c617392: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c617394; /* fmov.s @(r0,r4),fr3 */
L8c617394: r[0]=16u;goto L8c617396; /* mov #0x10,r0 */
L8c617396: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617398; /* fmov.s fr3,@-r8 */
L8c617398: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61739a; /* fmov.s @(r0,r4),fr3 */
L8c61739a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61739c; /* fmov.s fr3,@-r8 */
L8c61739c: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61739e; /* mov.l r3,@-r8 */
L8c61739e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6173a0; /* pref @r8 */
L8c6173a0: r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c6173a2; /* mov.l @r7,r2 */
L8c6173a2: r[8]+=32u;goto L8c6173a4; /* add #0x20,r8 */
L8c6173a4: r[2]+=4u;goto L8c6173a6; /* add #0x4,r2 */
L8c6173a6: wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c6173a8; /* mov.l r2,@r7 */
L8c6173a8: r[15]+=68u;goto L8c6173aa; /* add #0x44,r15 */
L8c6173aa: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6173ac; /* fmov.s @r15+,fr12 */
L8c6173ac: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6173ae; /* mov.l @r15+,r9 */
L8c6173ae: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6173b0; /* mov.l @r15+,r10 */
L8c6173b0: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6173b2; /* mov.l @r15+,r11 */
L8c6173b2: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6173b4; /* mov.l @r15+,r12 */
L8c6173b4: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6173b6; /* mov.l @r15+,r13 */
L8c6173b6: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
