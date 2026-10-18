/* Lit triangle near-plane clipper with portable queue interface; name inferred.
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
SA_STAGE_EXPORT int sa_lit_triangle_near_clip_8c612e64(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c612e64;
L8c612e64: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c612e66; /* mov.l r14,@-r15 */
L8c612e66: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c612e68; /* mov.l r13,@-r15 */
L8c612e68: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c612e6a; /* mov.l r12,@-r15 */
L8c612e6a: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c612e6c; /* mov.l r11,@-r15 */
L8c612e6c: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c612e6e; /* mov.l r10,@-r15 */
L8c612e6e: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c612e70; /* mov.l r9,@-r15 */
L8c612e70: r[15]+=4294967232u;goto L8c612e72; /* add #-0x40,r15 */
L8c612e72: r[3]=rd(m,bus,&io_status,0x8c612f30u,4);if(io_status)return io_status;goto L8c612e74; /* mov.l 0x8c612f30,r3 */
L8c612e74: r[7]=r[15];goto L8c612e76; /* mov r15,r7 */
L8c612e76: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c612e78; /* fldi1 fr6 */
L8c612e78: r[13]=r[15];goto L8c612e7a; /* mov r15,r13 */
L8c612e7a: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c612e7c; /* fmov.s @r3,fr3 */
L8c612e7c: r[7]+=52u;goto L8c612e7e; /* add #0x34,r7 */
L8c612e7e: fcopy(f,4u,6u);goto L8c612e80; /* fmov fr6,fr4 */
L8c612e80: r[13]+=16u;goto L8c612e82; /* add #0x10,r13 */
L8c612e82: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c612e84; /* fdiv fr3,fr4 */
L8c612e84: r[2]=r[7];goto L8c612e86; /* mov r7,r2 */
L8c612e86: r[9]=rd(m,bus,&io_status,0x8c612f34u,4);if(io_status)return io_status;goto L8c612e88; /* mov.l 0x8c612f34,r9 */
L8c612e88: r[10]=3u;goto L8c612e8a; /* mov #0x3,r10 */
L8c612e8a: r[11]=0u;goto L8c612e8c; /* mov #0x0,r11 */
L8c612e8c: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c612e8e; /* mov.l r2,@r15 */
L8c612e8e: wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c612e90; /* mov.l r4,@r2 */
L8c612e90: r[4]=1u;goto L8c612e92; /* mov #0x1,r4 */
L8c612e92: wr(m,bus,&io_status,r[7]+0x4u,r[5],4);if(io_status)return io_status;goto L8c612e94; /* mov.l r5,@(0x4,r7) */
L8c612e94: r[5]=r[13];goto L8c612e96; /* mov r13,r5 */
L8c612e96: wr(m,bus,&io_status,r[7]+0x8u,r[6],4);if(io_status)return io_status;goto L8c612e98; /* mov.l r6,@(0x8,r7) */
L8c612e98: r[14]=r[5];goto L8c612e9a; /* mov r5,r14 */
L8c612e9a: r[6]=rd(m,bus,&io_status,0x8c612f38u,4);if(io_status)return io_status;goto L8c612e9c; /* mov.l 0x8c612f38,r6 */
L8c612e9c: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612e9e; /* mov.l @r15,r12 */
L8c612e9e: wr(m,bus,&io_status,r[15]+0x8u,r[5],4);if(io_status)return io_status;goto L8c612ea0; /* mov.l r5,@(0x8,r15) */
L8c612ea0: r[3]=r[11];goto L8c612ea2; /* mov r11,r3 */
L8c612ea2: r[3]<<=2;goto L8c612ea4; /* shll2 r3 */
L8c612ea4: r[3]+=r[7];goto L8c612ea6; /* add r7,r3 */
L8c612ea6: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c612ea8; /* mov.l r3,@(0x4,r15) */
L8c612ea8: r[0]=4u;goto L8c612eaa; /* mov #0x4,r0 */
L8c612eaa: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c612eac; /* mov.l @r3,r2 */
L8c612eac: r[3]=r[2];goto L8c612eae; /* mov r2,r3 */
L8c612eae: r[1]=r[2];goto L8c612eb0; /* mov r2,r1 */
L8c612eb0: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c612eb2; /* mov.l r2,@r15 */
L8c612eb2: fload(m,bus,&io_status,f,2u,r[9]);if(io_status)return io_status;goto L8c612eb4; /* fmov.s @r9,fr2 */
L8c612eb4: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c612eb6; /* fmov.s @r3,fr3 */
L8c612eb6: r[3]=rd(m,bus,&io_status,0x8c612f3cu,4);if(io_status)return io_status;goto L8c612eb8; /* mov.l 0x8c612f3c,r3 */
L8c612eb8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612eba; /* fmul fr3,fr2 */
L8c612eba: fload(m,bus,&io_status,f,1u,r[0]+r[9]);if(io_status)return io_status;goto L8c612ebc; /* fmov.s @(r0,r9),fr1 */
L8c612ebc: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c612ebe; /* fmov.s @r3,fr3 */
L8c612ebe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612ec0; /* fsub fr2,fr3 */
L8c612ec0: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c612ec2; /* fmov.s @(r0,r1),fr2 */
L8c612ec2: r[0]=8u;goto L8c612ec4; /* mov #0x8,r0 */
L8c612ec4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c612ec6; /* fmul fr2,fr1 */
L8c612ec6: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c612ec8; /* fmov.s @(r0,r1),fr2 */
L8c612ec8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612eca; /* fsub fr1,fr3 */
L8c612eca: fload(m,bus,&io_status,f,1u,r[0]+r[9]);if(io_status)return io_status;goto L8c612ecc; /* fmov.s @(r0,r9),fr1 */
L8c612ecc: r[0]=28u;goto L8c612ece; /* mov #0x1c,r0 */
L8c612ece: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c612ed0; /* fmul fr2,fr1 */
L8c612ed0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612ed2; /* fsub fr1,fr3 */
L8c612ed2: fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612ed4; /* fmov fr3,@(r0,r2) */
L8c612ed4: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612ed6; /* mov.l @(0x4,r15),r2 */
L8c612ed6: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c612ed8; /* mov.l @r2,r1 */
L8c612ed8: r[0]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c612eda; /* mov.l @(0xc,r1),r0 */
L8c612eda: *sr=(*sr&~1u)|!(r[4]&r[0]);goto L8c612edc; /* tst r4,r0 */
L8c612edc: if(!(*sr&1u))goto L8c612f48;goto L8c612ede; /* bf 0x8c612f48 */
L8c612ede: r[1]=r[11];goto L8c612ee0; /* mov r11,r1 */
L8c612ee0: r[1]<<=2;goto L8c612ee2; /* shll2 r1 */
L8c612ee2: r[1]+=r[7];goto L8c612ee4; /* add r7,r1 */
L8c612ee4: fcopy(f,5u,6u);goto L8c612ee6; /* fmov fr6,fr5 */
L8c612ee6: r[0]=24u;goto L8c612ee8; /* mov #0x18,r0 */
L8c612ee8: wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c612eea; /* mov.l r1,@r15 */
L8c612eea: r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c612eec; /* mov.l @r1,r2 */
L8c612eec: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c612eee; /* mov.l r2,@(0x4,r15) */
L8c612eee: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612ef0; /* fmov.s @(r0,r2),fr3 */
L8c612ef0: r[0]=12u;goto L8c612ef2; /* mov #0xc,r0 */
L8c612ef2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c612ef4; /* fdiv fr3,fr5 */
L8c612ef4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612ef6; /* fmov.s @(r0,r6),fr3 */
L8c612ef6: r[0]=16u;goto L8c612ef8; /* mov #0x10,r0 */
L8c612ef8: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c612efa; /* fmov.s @(r0,r2),fr2 */
L8c612efa: r[0]=16u;goto L8c612efc; /* mov #0x10,r0 */
L8c612efc: r[2]=rd(m,bus,&io_status,0x8c612f40u,4);if(io_status)return io_status;goto L8c612efe; /* mov.l 0x8c612f40,r2 */
L8c612efe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612f00; /* fsub fr3,fr2 */
L8c612f00: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c612f02; /* fmov.s @r2,fr3 */
L8c612f02: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612f04; /* fmul fr5,fr2 */
L8c612f04: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612f06; /* fdiv fr3,fr2 */
L8c612f06: fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c612f08; /* fmov.s fr2,@r14 */
L8c612f08: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612f0a; /* mov.l @r15,r1 */
L8c612f0a: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612f0c; /* fmov.s @(r0,r6),fr3 */
L8c612f0c: r[0]=20u;goto L8c612f0e; /* mov #0x14,r0 */
L8c612f0e: r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c612f10; /* mov.l @r1,r1 */
L8c612f10: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c612f12; /* fmov.s @(r0,r1),fr2 */
L8c612f12: r[0]=4u;goto L8c612f14; /* mov #0x4,r0 */
L8c612f14: r[1]=rd(m,bus,&io_status,0x8c612f44u,4);if(io_status)return io_status;goto L8c612f16; /* mov.l 0x8c612f44,r1 */
L8c612f16: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612f18; /* fsub fr3,fr2 */
L8c612f18: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c612f1a; /* fmov.s @r1,fr3 */
L8c612f1a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612f1c; /* fmul fr5,fr2 */
L8c612f1c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612f1e; /* fdiv fr3,fr2 */
L8c612f1e: fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c612f20; /* fmov fr2,@(r0,r14) */
L8c612f20: r[0]=8u;goto L8c612f22; /* mov #0x8,r0 */
L8c612f22: fstore(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c612f64; /* bra 0x8c612f64 */
L8c612f48: r[2]=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;goto L8c612f4a; /* mov.l @r12,r2 */
L8c612f4a: r[0]=16u;goto L8c612f4c; /* mov #0x10,r0 */
L8c612f4c: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612f4e; /* fmov.s @(r0,r2),fr3 */
L8c612f4e: r[0]=20u;goto L8c612f50; /* mov #0x14,r0 */
L8c612f50: fstore(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c612f52; /* fmov.s fr3,@r5 */
L8c612f52: r[2]=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;goto L8c612f54; /* mov.l @r12,r2 */
L8c612f54: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612f56; /* fmov.s @(r0,r2),fr3 */
L8c612f56: r[0]=4u;goto L8c612f58; /* mov #0x4,r0 */
L8c612f58: fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c612f5a; /* fmov fr3,@(r0,r5) */
L8c612f5a: r[0]=24u;goto L8c612f5c; /* mov #0x18,r0 */
L8c612f5c: r[2]=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;goto L8c612f5e; /* mov.l @r12,r2 */
L8c612f5e: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612f60; /* fmov.s @(r0,r2),fr3 */
L8c612f60: r[0]=8u;goto L8c612f62; /* mov #0x8,r0 */
L8c612f62: fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c612f64; /* fmov fr3,@(r0,r5) */
L8c612f64: r[11]+=1u;goto L8c612f66; /* add #0x1,r11 */
L8c612f66: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[10]);goto L8c612f68; /* cmp/ge r10,r11 */
L8c612f68: r[5]+=12u;goto L8c612f6a; /* add #0xc,r5 */
L8c612f6a: r[12]+=4u;goto L8c612f6c; /* add #0x4,r12 */
L8c612f6c: take=!(*sr&1u);r[14]+=12u;if(take)goto L8c612ea0;goto L8c612f70; /* bf/s 0x8c612ea0 */
L8c612f70: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612f72; /* mov.l @(0x8,r15),r2 */
L8c612f72: r[0]=8u;goto L8c612f74; /* mov #0x8,r0 */
L8c612f74: r[14]=r[13];goto L8c612f76; /* mov r13,r14 */
L8c612f76: r[14]+=12u;goto L8c612f78; /* add #0xc,r14 */
L8c612f78: fload(m,bus,&io_status,f,6u,r[0]+r[2]);if(io_status)return io_status;goto L8c612f7a; /* fmov.s @(r0,r2),fr6 */
L8c612f7a: r[0]=4u;goto L8c612f7c; /* mov #0x4,r0 */
L8c612f7c: r[3]=r[2];goto L8c612f7e; /* mov r2,r3 */
L8c612f7e: fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c612f80; /* fmov.s @(r0,r14),fr5 */
L8c612f80: fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c612f82; /* fmov.s @(r0,r3),fr9 */
L8c612f82: r[0]=8u;goto L8c612f84; /* mov #0x8,r0 */
L8c612f84: fload(m,bus,&io_status,f,7u,r[0]+r[14]);if(io_status)return io_status;goto L8c612f86; /* fmov.s @(r0,r14),fr7 */
L8c612f86: r[5]=r[13];goto L8c612f88; /* mov r13,r5 */
L8c612f88: fcopy(f,3u,9u);goto L8c612f8a; /* fmov fr9,fr3 */
L8c612f8a: r[0]=4u;goto L8c612f8c; /* mov #0x4,r0 */
L8c612f8c: fcopy(f,0u,6u);goto L8c612f8e; /* fmov fr6,fr0 */
L8c612f8e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612f90; /* fmul fr7,fr3 */
L8c612f90: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c612f92; /* fmul fr5,fr0 */
L8c612f92: fload(m,bus,&io_status,f,10u,r[3]);if(io_status)return io_status;goto L8c612f94; /* fmov.s @r3,fr10 */
L8c612f94: fload(m,bus,&io_status,f,8u,r[14]);if(io_status)return io_status;goto L8c612f96; /* fmov.s @r14,fr8 */
L8c612f96: r[5]+=24u;goto L8c612f98; /* add #0x18,r5 */
L8c612f98: fcopy(f,2u,6u);goto L8c612f9a; /* fmov fr6,fr2 */
L8c612f9a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612f9c; /* fmul fr8,fr2 */
L8c612f9c: fcopy(f,1u,8u);goto L8c612f9e; /* fmov fr8,fr1 */
L8c612f9e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c612fa0; /* fsub fr3,fr0 */
L8c612fa0: fcopy(f,3u,10u);goto L8c612fa2; /* fmov fr10,fr3 */
L8c612fa2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612fa4; /* fmul fr7,fr3 */
L8c612fa4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[9],&f->fpscr,&fr[1]);if(status)return status;}goto L8c612fa6; /* fmul fr9,fr1 */
L8c612fa6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612fa8; /* fsub fr2,fr3 */
L8c612fa8: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c612faa; /* fmov.s @(r0,r5),fr2 */
L8c612faa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612fac; /* fmul fr3,fr2 */
L8c612fac: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c612fae; /* fmov.s @r5,fr3 */
L8c612fae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612fb0; /* fmac fr0,fr3,fr2 */
L8c612fb0: fcopy(f,11u,10u);goto L8c612fb2; /* fmov fr10,fr11 */
L8c612fb2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[5],&f->fpscr,&fr[11]);if(status)return status;}goto L8c612fb4; /* fmul fr5,fr11 */
L8c612fb4: r[0]=8u;goto L8c612fb6; /* mov #0x8,r0 */
L8c612fb6: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c612fb8; /* fmov.s @(r0,r5),fr3 */
L8c612fb8: r[5]=rd(m,bus,&io_status,0x8c6131d0u,4);if(io_status)return io_status;goto L8c612fba; /* mov.l 0x8c6131d0,r5 */
L8c612fba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[11],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c612fbc; /* fsub fr11,fr1 */
L8c612fbc: r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c612fbe; /* mov.l @r5,r3 */
L8c612fbe: fcopy(f,0u,1u);goto L8c612fc0; /* fmov fr1,fr0 */
L8c612fc0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612fc2; /* fmac fr0,fr3,fr2 */
L8c612fc2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c612fc4; /* fldi0 fr3 */
L8c612fc4: fcopy(f,5u,2u);goto L8c612fc6; /* fmov fr2,fr5 */
L8c612fc6: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c612fc8; /* fcmp/gt fr5,fr3 */
L8c612fc8: r[0]=*sr&1u;goto L8c612fca; /* movt r0 */
L8c612fca: *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c612fcc; /* tst r3,r0 */
L8c612fcc: if((*sr&1u))goto L8c612fd2;goto L8c612fce; /* bt 0x8c612fd2 */
L8c612fce: goto L8c613302; /* bra 0x8c613302 */
L8c612fd2: r[1]=0u;goto L8c612fd4; /* mov #0x0,r1 */
L8c612fd4: wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c612fd6; /* mov.l r1,@r5 */
L8c612fd6: r[14]=r[1];goto L8c612fd8; /* mov r1,r14 */
L8c612fd8: r[5]=rd(m,bus,&io_status,0x8c6131d4u,4);if(io_status)return io_status;goto L8c612fda; /* mov.l 0x8c6131d4,r5 */
L8c612fda: r[12]=r[1];goto L8c612fdc; /* mov r1,r12 */
L8c612fdc: r[9]=2u;goto L8c612fde; /* mov #0x2,r9 */
L8c612fde: r[3]=r[9];goto L8c612fe0; /* mov r9,r3 */
L8c612fe0: r[0]=r[7];goto L8c612fe2; /* mov r7,r0 */
L8c612fe2: r[3]<<=2;goto L8c612fe4; /* shll2 r3 */
L8c612fe4: r[11]=r[14];goto L8c612fe6; /* mov r14,r11 */
L8c612fe6: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c612fe8; /* mov.l r3,@r15 */
L8c612fe8: r[11]<<=2;goto L8c612fea; /* shll2 r11 */
L8c612fea: r[2]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c612fec; /* mov.l @(r0,r3),r2 */
L8c612fec: r[3]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612fee; /* mov.l @(0xc,r2),r3 */
L8c612fee: r[2]=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;goto L8c612ff0; /* mov.l @(r0,r11),r2 */
L8c612ff0: r[3]&=r[4];goto L8c612ff2; /* and r4,r3 */
L8c612ff2: if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[2]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612ff4; /* mov.l @(0xc,r2),r2 */
L8c612ff4: r[2]&=r[4];goto L8c612ff6; /* and r4,r2 */
L8c612ff6: r[3]^=r[2];goto L8c612ff8; /* xor r2,r3 */
L8c612ff8: r[0]=r[3];goto L8c612ffa; /* mov r3,r0 */
L8c612ffa: *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c612ffc; /* cmp/eq #0x1,r0 */
L8c612ffc: if(!(*sr&1u))goto L8c6130ac;goto L8c612ffe; /* bf 0x8c6130ac */
L8c612ffe: r[3]=r[9];goto L8c613000; /* mov r9,r3 */
L8c613000: *sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c613002; /* shll r9 */
L8c613002: r[2]=r[14];goto L8c613004; /* mov r14,r2 */
L8c613004: r[9]+=r[3];goto L8c613006; /* add r3,r9 */
L8c613006: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c613008; /* shll r2 */
L8c613008: r[1]=r[14];goto L8c61300a; /* mov r14,r1 */
L8c61300a: r[9]<<=2;goto L8c61300c; /* shll2 r9 */
L8c61300c: r[3]=rd(m,bus,&io_status,0x8c6131d8u,4);if(io_status)return io_status;goto L8c61300e; /* mov.l 0x8c6131d8,r3 */
L8c61300e: r[9]+=r[13];goto L8c613010; /* add r13,r9 */
L8c613010: r[0]=8u;goto L8c613012; /* mov #0x8,r0 */
L8c613012: wr(m,bus,&io_status,r[15]+0x8u,r[9],4);if(io_status)return io_status;goto L8c613014; /* mov.l r9,@(0x8,r15) */
L8c613014: r[2]+=r[1];goto L8c613016; /* add r1,r2 */
L8c613016: fload(m,bus,&io_status,f,5u,r[0]+r[9]);if(io_status)return io_status;goto L8c613018; /* fmov.s @(r0,r9),fr5 */
L8c613018: r[2]<<=2;goto L8c61301a; /* shll2 r2 */
L8c61301a: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61301c; /* fmov.s @r3,fr3 */
L8c61301c: r[2]+=r[13];goto L8c61301e; /* add r13,r2 */
L8c61301e: r[0]=8u;goto L8c613020; /* mov #0x8,r0 */
L8c613020: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613022; /* fsub fr5,fr3 */
L8c613022: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c613024; /* mov.l r2,@(0x4,r15) */
L8c613024: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c613026; /* fmov.s @(r0,r2),fr2 */
L8c613026: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613028; /* mov.l @(0x4,r15),r1 */
L8c613028: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61302a; /* fsub fr5,fr2 */
L8c61302a: fcopy(f,5u,3u);goto L8c61302c; /* fmov fr3,fr5 */
L8c61302c: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61302e; /* mov.l @(0x8,r15),r2 */
L8c61302e: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c613030; /* fmov.s @r1,fr3 */
L8c613030: fload(m,bus,&io_status,f,6u,r[2]);if(io_status)return io_status;goto L8c613032; /* fmov.s @r2,fr6 */
L8c613032: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[2],&f->fpscr,&fr[5]);if(status)return status;}goto L8c613034; /* fdiv fr2,fr5 */
L8c613034: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613036; /* fsub fr6,fr3 */
L8c613036: fcopy(f,2u,6u);goto L8c613038; /* fmov fr6,fr2 */
L8c613038: fcopy(f,0u,5u);goto L8c61303a; /* fmov fr5,fr0 */
L8c61303a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61303c; /* fmac fr0,fr3,fr2 */
L8c61303c: fcopy(f,6u,2u);goto L8c61303e; /* fmov fr2,fr6 */
L8c61303e: r[2]=rd(m,bus,&io_status,0x8c6131dcu,4);if(io_status)return io_status;goto L8c613040; /* mov.l 0x8c6131dc,r2 */
L8c613040: r[0]=12u;goto L8c613042; /* mov #0xc,r0 */
L8c613042: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c613044; /* fmov.s @(r0,r6),fr2 */
L8c613044: r[9]=r[12];goto L8c613046; /* mov r12,r9 */
L8c613046: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c613048; /* fmov.s @r2,fr3 */
L8c613048: r[9]<<=2;goto L8c61304a; /* shll2 r9 */
L8c61304a: fcopy(f,0u,4u);goto L8c61304c; /* fmov fr4,fr0 */
L8c61304c: r[9]<<=2;goto L8c61304e; /* shll2 r9 */
L8c61304e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c613050; /* fmul fr6,fr3 */
L8c613050: *sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c613052; /* shll r9 */
L8c613052: r[0]=16u;goto L8c613054; /* mov #0x10,r0 */
L8c613054: r[9]+=r[5];goto L8c613056; /* add r5,r9 */
L8c613056: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613058; /* fmac fr0,fr3,fr2 */
L8c613058: fcopy(f,0u,5u);goto L8c61305a; /* fmov fr5,fr0 */
L8c61305a: fstore(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61305c; /* fmov fr2,@(r0,r9) */
L8c61305c: r[0]=4u;goto L8c61305e; /* mov #0x4,r0 */
L8c61305e: r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613060; /* mov.l @(0x8,r15),r1 */
L8c613060: fload(m,bus,&io_status,f,6u,r[0]+r[1]);if(io_status)return io_status;goto L8c613062; /* fmov.s @(r0,r1),fr6 */
L8c613062: r[0]=4u;goto L8c613064; /* mov #0x4,r0 */
L8c613064: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613066; /* mov.l @(0x4,r15),r1 */
L8c613066: fcopy(f,2u,6u);goto L8c613068; /* fmov fr6,fr2 */
L8c613068: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61306a; /* fmov.s @(r0,r1),fr3 */
L8c61306a: r[0]=16u;goto L8c61306c; /* mov #0x10,r0 */
L8c61306c: r[1]=rd(m,bus,&io_status,0x8c6131e0u,4);if(io_status)return io_status;goto L8c61306e; /* mov.l 0x8c6131e0,r1 */
L8c61306e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613070; /* fsub fr6,fr3 */
L8c613070: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613072; /* fmac fr0,fr3,fr2 */
L8c613072: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c613074; /* fmov.s @r1,fr3 */
L8c613074: fcopy(f,0u,4u);goto L8c613076; /* fmov fr4,fr0 */
L8c613076: fcopy(f,6u,2u);goto L8c613078; /* fmov fr2,fr6 */
L8c613078: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61307a; /* fmul fr6,fr3 */
L8c61307a: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61307c; /* fmov.s @(r0,r6),fr2 */
L8c61307c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61307e; /* fmac fr0,fr3,fr2 */
L8c61307e: r[0]=20u;goto L8c613080; /* mov #0x14,r0 */
L8c613080: fstore(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c613082; /* fmov fr2,@(r0,r9) */
L8c613082: r[0]=24u;goto L8c613084; /* mov #0x18,r0 */
L8c613084: fstore(m,bus,&io_status,f,0u,r[0]+r[9]);if(io_status)return io_status;goto L8c613086; /* fmov fr0,@(r0,r9) */
L8c613086: r[2]=28u;goto L8c613088; /* mov #0x1c,r2 */
L8c613088: r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61308a; /* mov.l @r15,r0 */
L8c61308a: r[12]+=1u;goto L8c61308c; /* add #0x1,r12 */
L8c61308c: fcopy(f,0u,5u);goto L8c61308e; /* fmov fr5,fr0 */
L8c61308e: r[3]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c613090; /* mov.l @(r0,r7),r3 */
L8c613090: r[0]=r[7];goto L8c613092; /* mov r7,r0 */
L8c613092: r[2]+=r[3];goto L8c613094; /* add r3,r2 */
L8c613094: wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c613096; /* mov.l r3,@(0xc,r15) */
L8c613096: r[3]=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;goto L8c613098; /* mov.l @(r0,r11),r3 */
L8c613098: fload(m,bus,&io_status,f,6u,r[2]);if(io_status)return io_status;goto L8c61309a; /* fmov.s @r2,fr6 */
L8c61309a: r[2]=28u;goto L8c61309c; /* mov #0x1c,r2 */
L8c61309c: r[2]+=r[3];goto L8c61309e; /* add r3,r2 */
L8c61309e: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6130a0; /* fmov.s @r2,fr3 */
L8c6130a0: r[3]=28u;goto L8c6130a2; /* mov #0x1c,r3 */
L8c6130a2: fcopy(f,2u,6u);goto L8c6130a4; /* fmov fr6,fr2 */
L8c6130a4: r[3]+=r[9];goto L8c6130a6; /* add r9,r3 */
L8c6130a6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6130a8; /* fsub fr6,fr3 */
L8c6130a8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6130aa; /* fmac fr0,fr3,fr2 */
L8c6130aa: fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6130ac; /* fmov.s fr2,@r3 */
L8c6130ac: r[0]=r[11];goto L8c6130ae; /* mov r11,r0 */
L8c6130ae: r[3]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c6130b0; /* mov.l @(r0,r7),r3 */
L8c6130b0: r[3]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c6130b2; /* mov.l @(0xc,r3),r3 */
L8c6130b2: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6130b4; /* tst r4,r3 */
L8c6130b4: if(!(*sr&1u))goto L8c6130ec;goto L8c6130b6; /* bf 0x8c6130ec */
L8c6130b6: r[11]+=r[7];goto L8c6130b8; /* add r7,r11 */
L8c6130b8: r[9]=r[12];goto L8c6130ba; /* mov r12,r9 */
L8c6130ba: r[9]<<=2;goto L8c6130bc; /* shll2 r9 */
L8c6130bc: wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6130be; /* mov.l r11,@r15 */
L8c6130be: r[3]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c6130c0; /* mov.l @r11,r3 */
L8c6130c0: r[0]=16u;goto L8c6130c2; /* mov #0x10,r0 */
L8c6130c2: r[9]<<=2;goto L8c6130c4; /* shll2 r9 */
L8c6130c4: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6130c6; /* fmov.s @(r0,r3),fr3 */
L8c6130c6: *sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c6130c8; /* shll r9 */
L8c6130c8: r[9]+=r[5];goto L8c6130ca; /* add r5,r9 */
L8c6130ca: fstore(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6130cc; /* fmov fr3,@(r0,r9) */
L8c6130cc: r[0]=20u;goto L8c6130ce; /* mov #0x14,r0 */
L8c6130ce: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6130d0; /* mov.l @r15,r3 */
L8c6130d0: r[12]+=1u;goto L8c6130d2; /* add #0x1,r12 */
L8c6130d2: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6130d4; /* mov.l @r3,r2 */
L8c6130d4: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c6130d6; /* fmov.s @(r0,r2),fr3 */
L8c6130d6: fstore(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6130d8; /* fmov fr3,@(r0,r9) */
L8c6130d8: r[0]=24u;goto L8c6130da; /* mov #0x18,r0 */
L8c6130da: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6130dc; /* mov.l @r15,r3 */
L8c6130dc: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6130de; /* mov.l @r3,r2 */
L8c6130de: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c6130e0; /* fmov.s @(r0,r2),fr3 */
L8c6130e0: fstore(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6130e2; /* fmov fr3,@(r0,r9) */
L8c6130e2: r[0]=28u;goto L8c6130e4; /* mov #0x1c,r0 */
L8c6130e4: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6130e6; /* mov.l @r15,r3 */
L8c6130e6: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6130e8; /* mov.l @r3,r2 */
L8c6130e8: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c6130ea; /* fmov.s @(r0,r2),fr3 */
L8c6130ea: fstore(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6130ec; /* fmov fr3,@(r0,r9) */
L8c6130ec: r[9]=r[14];goto L8c6130ee; /* mov r14,r9 */
L8c6130ee: r[14]+=1u;goto L8c6130f0; /* add #0x1,r14 */
L8c6130f0: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[10]);goto L8c6130f2; /* cmp/ge r10,r14 */
L8c6130f2: if((*sr&1u))goto L8c6130f8;goto L8c6130f4; /* bt 0x8c6130f8 */
L8c6130f4: goto L8c612fde; /* bra 0x8c612fde */
L8c6130f8: r[4]=rd(m,bus,&io_status,0x8c6131ecu,4);if(io_status)return io_status;goto L8c6130fa; /* mov.l 0x8c6131ec,r4 */
L8c6130fa: r[0]=r[12];goto L8c6130fc; /* mov r12,r0 */
L8c6130fc: r[14]=rd(m,bus,&io_status,0x8c6131e4u,4);if(io_status)return io_status;goto L8c6130fe; /* mov.l 0x8c6131e4,r14 */
L8c6130fe: *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c613100; /* cmp/eq #0x3,r0 */
L8c613100: r[6]=rd(m,bus,&io_status,0x8c6131e8u,4);if(io_status)return io_status;goto L8c613102; /* mov.l 0x8c6131e8,r6 */
L8c613102: if(!(*sr&1u))goto L8c6131f4;goto L8c613104; /* bf 0x8c6131f4 */
L8c613104: r[0]=28u;goto L8c613106; /* mov #0x1c,r0 */
L8c613106: r[7]=r[5];goto L8c613108; /* mov r5,r7 */
L8c613108: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61310a; /* fmov.s @(r0,r4),fr2 */
L8c61310a: r[8]+=32u;goto L8c61310c; /* add #0x20,r8 */
L8c61310c: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61310e; /* fmov.s @(r0,r7),fr3 */
L8c61310e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613110; /* fmul fr3,fr2 */
L8c613110: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613112; /* fmov.s fr2,@-r8 */
L8c613112: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613114; /* fmov.s @(r0,r7),fr3 */
L8c613114: r[0]=24u;goto L8c613116; /* mov #0x18,r0 */
L8c613116: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613118; /* fmov.s @(r0,r4),fr2 */
L8c613118: r[0]=28u;goto L8c61311a; /* mov #0x1c,r0 */
L8c61311a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61311c; /* fmul fr3,fr2 */
L8c61311c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61311e; /* fmov.s fr2,@-r8 */
L8c61311e: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613120; /* fmov.s @(r0,r7),fr3 */
L8c613120: r[0]=20u;goto L8c613122; /* mov #0x14,r0 */
L8c613122: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613124; /* fmov.s @(r0,r4),fr2 */
L8c613124: r[0]=16u;goto L8c613126; /* mov #0x10,r0 */
L8c613126: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613128; /* fmul fr3,fr2 */
L8c613128: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61312a; /* fmov.s fr2,@-r8 */
L8c61312a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61312c; /* fmov.s @(r0,r4),fr3 */
L8c61312c: r[0]=24u;goto L8c61312e; /* mov #0x18,r0 */
L8c61312e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613130; /* fmov.s fr3,@-r8 */
L8c613130: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613132; /* fmov.s @(r0,r7),fr3 */
L8c613132: r[0]=20u;goto L8c613134; /* mov #0x14,r0 */
L8c613134: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613136; /* fmov.s fr3,@-r8 */
L8c613136: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613138; /* fmov.s @(r0,r7),fr3 */
L8c613138: r[0]=16u;goto L8c61313a; /* mov #0x10,r0 */
L8c61313a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61313c; /* fmov.s fr3,@-r8 */
L8c61313c: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61313e; /* fmov.s @(r0,r7),fr3 */
L8c61313e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613140; /* fmov.s fr3,@-r8 */
L8c613140: r[8]-=4;wr(m,bus,&io_status,r[8],r[6],4);if(io_status)return io_status;goto L8c613142; /* mov.l r6,@-r8 */
L8c613142: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613144; /* pref @r8 */
L8c613144: r[0]=28u;goto L8c613146; /* mov #0x1c,r0 */
L8c613146: r[7]+=32u;goto L8c613148; /* add #0x20,r7 */
L8c613148: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61314a; /* fmov.s @(r0,r4),fr2 */
L8c61314a: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61314c; /* fmov.s @(r0,r7),fr3 */
L8c61314c: r[8]+=64u;goto L8c61314e; /* add #0x40,r8 */
L8c61314e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613150; /* fmul fr3,fr2 */
L8c613150: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613152; /* fmov.s fr2,@-r8 */
L8c613152: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613154; /* fmov.s @(r0,r7),fr3 */
L8c613154: r[0]=24u;goto L8c613156; /* mov #0x18,r0 */
L8c613156: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613158; /* fmov.s @(r0,r4),fr2 */
L8c613158: r[0]=28u;goto L8c61315a; /* mov #0x1c,r0 */
L8c61315a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61315c; /* fmul fr3,fr2 */
L8c61315c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61315e; /* fmov.s fr2,@-r8 */
L8c61315e: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613160; /* fmov.s @(r0,r7),fr3 */
L8c613160: r[0]=20u;goto L8c613162; /* mov #0x14,r0 */
L8c613162: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613164; /* fmov.s @(r0,r4),fr2 */
L8c613164: r[0]=16u;goto L8c613166; /* mov #0x10,r0 */
L8c613166: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613168; /* fmul fr3,fr2 */
L8c613168: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61316a; /* fmov.s fr2,@-r8 */
L8c61316a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61316c; /* fmov.s @(r0,r4),fr3 */
L8c61316c: r[0]=24u;goto L8c61316e; /* mov #0x18,r0 */
L8c61316e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613170; /* fmov.s fr3,@-r8 */
L8c613170: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613172; /* fmov.s @(r0,r7),fr3 */
L8c613172: r[0]=20u;goto L8c613174; /* mov #0x14,r0 */
L8c613174: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613176; /* fmov.s fr3,@-r8 */
L8c613176: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613178; /* fmov.s @(r0,r7),fr3 */
L8c613178: r[0]=16u;goto L8c61317a; /* mov #0x10,r0 */
L8c61317a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61317c; /* fmov.s fr3,@-r8 */
L8c61317c: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61317e; /* fmov.s @(r0,r7),fr3 */
L8c61317e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613180; /* fmov.s fr3,@-r8 */
L8c613180: r[8]-=4;wr(m,bus,&io_status,r[8],r[6],4);if(io_status)return io_status;goto L8c613182; /* mov.l r6,@-r8 */
L8c613182: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613184; /* pref @r8 */
L8c613184: r[6]=r[5];goto L8c613186; /* mov r5,r6 */
L8c613186: r[0]=28u;goto L8c613188; /* mov #0x1c,r0 */
L8c613188: r[6]+=64u;goto L8c61318a; /* add #0x40,r6 */
L8c61318a: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61318c; /* fmov.s @(r0,r4),fr2 */
L8c61318c: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61318e; /* fmov.s @(r0,r6),fr3 */
L8c61318e: r[8]+=64u;goto L8c613190; /* add #0x40,r8 */
L8c613190: r[3]=rd(m,bus,&io_status,0x8c6131f0u,4);if(io_status)return io_status;goto L8c613192; /* mov.l 0x8c6131f0,r3 */
L8c613192: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613194; /* fmul fr3,fr2 */
L8c613194: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613196; /* fmov.s fr2,@-r8 */
L8c613196: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c613198; /* fmov.s @(r0,r6),fr3 */
L8c613198: r[0]=24u;goto L8c61319a; /* mov #0x18,r0 */
L8c61319a: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61319c; /* fmov.s @(r0,r4),fr2 */
L8c61319c: r[0]=28u;goto L8c61319e; /* mov #0x1c,r0 */
L8c61319e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6131a0; /* fmul fr3,fr2 */
L8c6131a0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6131a2; /* fmov.s fr2,@-r8 */
L8c6131a2: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6131a4; /* fmov.s @(r0,r6),fr3 */
L8c6131a4: r[0]=20u;goto L8c6131a6; /* mov #0x14,r0 */
L8c6131a6: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6131a8; /* fmov.s @(r0,r4),fr2 */
L8c6131a8: r[0]=16u;goto L8c6131aa; /* mov #0x10,r0 */
L8c6131aa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6131ac; /* fmul fr3,fr2 */
L8c6131ac: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6131ae; /* fmov.s fr2,@-r8 */
L8c6131ae: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6131b0; /* fmov.s @(r0,r4),fr3 */
L8c6131b0: r[0]=24u;goto L8c6131b2; /* mov #0x18,r0 */
L8c6131b2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6131b4; /* fmov.s fr3,@-r8 */
L8c6131b4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6131b6; /* fmov.s @(r0,r6),fr3 */
L8c6131b6: r[0]=20u;goto L8c6131b8; /* mov #0x14,r0 */
L8c6131b8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6131ba; /* fmov.s fr3,@-r8 */
L8c6131ba: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6131bc; /* fmov.s @(r0,r6),fr3 */
L8c6131bc: r[0]=16u;goto L8c6131be; /* mov #0x10,r0 */
L8c6131be: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6131c0; /* fmov.s fr3,@-r8 */
L8c6131c0: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6131c2; /* fmov.s @(r0,r6),fr3 */
L8c6131c2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6131c4; /* fmov.s fr3,@-r8 */
L8c6131c4: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6131c6; /* mov.l r3,@-r8 */
L8c6131c6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6131c8; /* pref @r8 */
L8c6131c8: r[2]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c6131ca; /* mov.l @r14,r2 */
L8c6131ca: r[8]+=32u;goto L8c6131cc; /* add #0x20,r8 */
L8c6131cc: r[2]+=3u;goto L8c613300; /* bra 0x8c613300 */
L8c6131f4: r[0]=28u;goto L8c6131f6; /* mov #0x1c,r0 */
L8c6131f6: r[7]=r[5];goto L8c6131f8; /* mov r5,r7 */
L8c6131f8: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6131fa; /* fmov.s @(r0,r4),fr2 */
L8c6131fa: r[8]+=32u;goto L8c6131fc; /* add #0x20,r8 */
L8c6131fc: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6131fe; /* fmov.s @(r0,r7),fr3 */
L8c6131fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613200; /* fmul fr3,fr2 */
L8c613200: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613202; /* fmov.s fr2,@-r8 */
L8c613202: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613204; /* fmov.s @(r0,r7),fr3 */
L8c613204: r[0]=24u;goto L8c613206; /* mov #0x18,r0 */
L8c613206: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613208; /* fmov.s @(r0,r4),fr2 */
L8c613208: r[0]=28u;goto L8c61320a; /* mov #0x1c,r0 */
L8c61320a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61320c; /* fmul fr3,fr2 */
L8c61320c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61320e; /* fmov.s fr2,@-r8 */
L8c61320e: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613210; /* fmov.s @(r0,r7),fr3 */
L8c613210: r[0]=20u;goto L8c613212; /* mov #0x14,r0 */
L8c613212: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613214; /* fmov.s @(r0,r4),fr2 */
L8c613214: r[0]=16u;goto L8c613216; /* mov #0x10,r0 */
L8c613216: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613218; /* fmul fr3,fr2 */
L8c613218: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61321a; /* fmov.s fr2,@-r8 */
L8c61321a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61321c; /* fmov.s @(r0,r4),fr3 */
L8c61321c: r[0]=24u;goto L8c61321e; /* mov #0x18,r0 */
L8c61321e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613220; /* fmov.s fr3,@-r8 */
L8c613220: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613222; /* fmov.s @(r0,r7),fr3 */
L8c613222: r[0]=20u;goto L8c613224; /* mov #0x14,r0 */
L8c613224: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613226; /* fmov.s fr3,@-r8 */
L8c613226: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613228; /* fmov.s @(r0,r7),fr3 */
L8c613228: r[0]=16u;goto L8c61322a; /* mov #0x10,r0 */
L8c61322a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61322c; /* fmov.s fr3,@-r8 */
L8c61322c: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61322e; /* fmov.s @(r0,r7),fr3 */
L8c61322e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613230; /* fmov.s fr3,@-r8 */
L8c613230: r[8]-=4;wr(m,bus,&io_status,r[8],r[6],4);if(io_status)return io_status;goto L8c613232; /* mov.l r6,@-r8 */
L8c613232: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613234; /* pref @r8 */
L8c613234: r[0]=28u;goto L8c613236; /* mov #0x1c,r0 */
L8c613236: r[7]+=32u;goto L8c613238; /* add #0x20,r7 */
L8c613238: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61323a; /* fmov.s @(r0,r4),fr2 */
L8c61323a: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61323c; /* fmov.s @(r0,r7),fr3 */
L8c61323c: r[8]+=64u;goto L8c61323e; /* add #0x40,r8 */
L8c61323e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613240; /* fmul fr3,fr2 */
L8c613240: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613242; /* fmov.s fr2,@-r8 */
L8c613242: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613244; /* fmov.s @(r0,r7),fr3 */
L8c613244: r[0]=24u;goto L8c613246; /* mov #0x18,r0 */
L8c613246: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613248; /* fmov.s @(r0,r4),fr2 */
L8c613248: r[0]=28u;goto L8c61324a; /* mov #0x1c,r0 */
L8c61324a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61324c; /* fmul fr3,fr2 */
L8c61324c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61324e; /* fmov.s fr2,@-r8 */
L8c61324e: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613250; /* fmov.s @(r0,r7),fr3 */
L8c613250: r[0]=20u;goto L8c613252; /* mov #0x14,r0 */
L8c613252: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613254; /* fmov.s @(r0,r4),fr2 */
L8c613254: r[0]=16u;goto L8c613256; /* mov #0x10,r0 */
L8c613256: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613258; /* fmul fr3,fr2 */
L8c613258: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61325a; /* fmov.s fr2,@-r8 */
L8c61325a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61325c; /* fmov.s @(r0,r4),fr3 */
L8c61325c: r[0]=24u;goto L8c61325e; /* mov #0x18,r0 */
L8c61325e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613260; /* fmov.s fr3,@-r8 */
L8c613260: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613262; /* fmov.s @(r0,r7),fr3 */
L8c613262: r[0]=20u;goto L8c613264; /* mov #0x14,r0 */
L8c613264: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613266; /* fmov.s fr3,@-r8 */
L8c613266: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613268; /* fmov.s @(r0,r7),fr3 */
L8c613268: r[0]=16u;goto L8c61326a; /* mov #0x10,r0 */
L8c61326a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61326c; /* fmov.s fr3,@-r8 */
L8c61326c: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61326e; /* fmov.s @(r0,r7),fr3 */
L8c61326e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613270; /* fmov.s fr3,@-r8 */
L8c613270: r[8]-=4;wr(m,bus,&io_status,r[8],r[6],4);if(io_status)return io_status;goto L8c613272; /* mov.l r6,@-r8 */
L8c613272: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613274; /* pref @r8 */
L8c613274: r[7]=r[5];goto L8c613276; /* mov r5,r7 */
L8c613276: r[0]=28u;goto L8c613278; /* mov #0x1c,r0 */
L8c613278: r[7]+=96u;goto L8c61327a; /* add #0x60,r7 */
L8c61327a: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61327c; /* fmov.s @(r0,r4),fr2 */
L8c61327c: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61327e; /* fmov.s @(r0,r7),fr3 */
L8c61327e: r[8]+=64u;goto L8c613280; /* add #0x40,r8 */
L8c613280: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613282; /* fmul fr3,fr2 */
L8c613282: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613284; /* fmov.s fr2,@-r8 */
L8c613284: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613286; /* fmov.s @(r0,r7),fr3 */
L8c613286: r[0]=24u;goto L8c613288; /* mov #0x18,r0 */
L8c613288: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61328a; /* fmov.s @(r0,r4),fr2 */
L8c61328a: r[0]=28u;goto L8c61328c; /* mov #0x1c,r0 */
L8c61328c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61328e; /* fmul fr3,fr2 */
L8c61328e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613290; /* fmov.s fr2,@-r8 */
L8c613290: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613292; /* fmov.s @(r0,r7),fr3 */
L8c613292: r[0]=20u;goto L8c613294; /* mov #0x14,r0 */
L8c613294: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613296; /* fmov.s @(r0,r4),fr2 */
L8c613296: r[0]=16u;goto L8c613298; /* mov #0x10,r0 */
L8c613298: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61329a; /* fmul fr3,fr2 */
L8c61329a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61329c; /* fmov.s fr2,@-r8 */
L8c61329c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61329e; /* fmov.s @(r0,r4),fr3 */
L8c61329e: r[0]=24u;goto L8c6132a0; /* mov #0x18,r0 */
L8c6132a0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132a2; /* fmov.s fr3,@-r8 */
L8c6132a2: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6132a4; /* fmov.s @(r0,r7),fr3 */
L8c6132a4: r[0]=20u;goto L8c6132a6; /* mov #0x14,r0 */
L8c6132a6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132a8; /* fmov.s fr3,@-r8 */
L8c6132a8: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6132aa; /* fmov.s @(r0,r7),fr3 */
L8c6132aa: r[0]=16u;goto L8c6132ac; /* mov #0x10,r0 */
L8c6132ac: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132ae; /* fmov.s fr3,@-r8 */
L8c6132ae: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6132b0; /* fmov.s @(r0,r7),fr3 */
L8c6132b0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132b2; /* fmov.s fr3,@-r8 */
L8c6132b2: r[8]-=4;wr(m,bus,&io_status,r[8],r[6],4);if(io_status)return io_status;goto L8c6132b4; /* mov.l r6,@-r8 */
L8c6132b4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6132b6; /* pref @r8 */
L8c6132b6: r[6]=r[5];goto L8c6132b8; /* mov r5,r6 */
L8c6132b8: r[0]=28u;goto L8c6132ba; /* mov #0x1c,r0 */
L8c6132ba: r[6]+=64u;goto L8c6132bc; /* add #0x40,r6 */
L8c6132bc: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6132be; /* fmov.s @(r0,r4),fr2 */
L8c6132be: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6132c0; /* fmov.s @(r0,r6),fr3 */
L8c6132c0: r[8]+=64u;goto L8c6132c2; /* add #0x40,r8 */
L8c6132c2: r[3]=rd(m,bus,&io_status,0x8c6133a0u,4);if(io_status)return io_status;goto L8c6132c4; /* mov.l 0x8c6133a0,r3 */
L8c6132c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6132c6; /* fmul fr3,fr2 */
L8c6132c6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6132c8; /* fmov.s fr2,@-r8 */
L8c6132c8: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6132ca; /* fmov.s @(r0,r6),fr3 */
L8c6132ca: r[0]=24u;goto L8c6132cc; /* mov #0x18,r0 */
L8c6132cc: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6132ce; /* fmov.s @(r0,r4),fr2 */
L8c6132ce: r[0]=28u;goto L8c6132d0; /* mov #0x1c,r0 */
L8c6132d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6132d2; /* fmul fr3,fr2 */
L8c6132d2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6132d4; /* fmov.s fr2,@-r8 */
L8c6132d4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6132d6; /* fmov.s @(r0,r6),fr3 */
L8c6132d6: r[0]=20u;goto L8c6132d8; /* mov #0x14,r0 */
L8c6132d8: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6132da; /* fmov.s @(r0,r4),fr2 */
L8c6132da: r[0]=16u;goto L8c6132dc; /* mov #0x10,r0 */
L8c6132dc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6132de; /* fmul fr3,fr2 */
L8c6132de: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6132e0; /* fmov.s fr2,@-r8 */
L8c6132e0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6132e2; /* fmov.s @(r0,r4),fr3 */
L8c6132e2: r[0]=24u;goto L8c6132e4; /* mov #0x18,r0 */
L8c6132e4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132e6; /* fmov.s fr3,@-r8 */
L8c6132e6: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6132e8; /* fmov.s @(r0,r6),fr3 */
L8c6132e8: r[0]=20u;goto L8c6132ea; /* mov #0x14,r0 */
L8c6132ea: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132ec; /* fmov.s fr3,@-r8 */
L8c6132ec: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6132ee; /* fmov.s @(r0,r6),fr3 */
L8c6132ee: r[0]=16u;goto L8c6132f0; /* mov #0x10,r0 */
L8c6132f0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132f2; /* fmov.s fr3,@-r8 */
L8c6132f2: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6132f4; /* fmov.s @(r0,r6),fr3 */
L8c6132f4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132f6; /* fmov.s fr3,@-r8 */
L8c6132f6: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6132f8; /* mov.l r3,@-r8 */
L8c6132f8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6132fa; /* pref @r8 */
L8c6132fa: r[2]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c6132fc; /* mov.l @r14,r2 */
L8c6132fc: r[8]+=32u;goto L8c6132fe; /* add #0x20,r8 */
L8c6132fe: r[2]+=4u;goto L8c613300; /* add #0x4,r2 */
L8c613300: wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c613302; /* mov.l r2,@r14 */
L8c613302: r[15]+=64u;goto L8c613304; /* add #0x40,r15 */
L8c613304: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613306; /* mov.l @r15+,r9 */
L8c613306: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613308; /* mov.l @r15+,r10 */
L8c613308: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61330a; /* mov.l @r15+,r11 */
L8c61330a: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61330c; /* mov.l @r15+,r12 */
L8c61330c: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61330e; /* mov.l @r15+,r13 */
L8c61330e: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
