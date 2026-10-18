/* Normal-lit primitive family with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_lit_primitive_emit_8c617e46(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c617e46;
L8c617e46: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c617e48; /* mov.l r14,@-r15 */
L8c617e48: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c617e4a; /* mov.l r13,@-r15 */
L8c617e4a: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c617e4c; /* mov.l r12,@-r15 */
L8c617e4c: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c617e4e; /* mov.l r11,@-r15 */
L8c617e4e: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c617e50; /* mov.l r10,@-r15 */
L8c617e50: r[3]=rd(m,bus,&io_status,0x8c617ebcu,4);if(io_status)return io_status;goto L8c617e52; /* mov.l 0x8c617ebc,r3 */
L8c617e52: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c617e54; /* mov.l r9,@-r15 */
L8c617e54: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c617e56; /* sts.l PR,@-r15 */
L8c617e56: r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c617e58; /* mov.w @r4,r0 */
L8c617e58: r[13]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c617e5a; /* mov.l @(0x4,r4),r13 */
L8c617e5a: r[0]=r[0]&65535u;goto L8c617e5c; /* extu.w r0,r0 */
L8c617e5c: r[9]=rd(m,bus,&io_status,0x8c617eb4u,4);if(io_status)return io_status;goto L8c617e5e; /* mov.l 0x8c617eb4,r9 */
L8c617e5e: r[0]&=r[3];goto L8c617e60; /* and r3,r0 */
L8c617e60: r[11]=rd(m,bus,&io_status,0x8c617eb8u,4);if(io_status)return io_status;goto L8c617e62; /* mov.l 0x8c617eb8,r11 */
L8c617e62: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c617e64; /* cmp/eq #0x0,r0 */
L8c617e64: take=(*sr&1u);r[12]=0u;if(take)goto L8c617e8a;goto L8c617e68; /* bt/s 0x8c617e8a */
L8c617e68: r[1]=sx(m,bus,&io_status,0x8c617ea4u,2);if(io_status)return io_status;goto L8c617e6a; /* mov.w 0x8c617ea4,r1 */
L8c617e6a: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c617e6c; /* cmp/eq r1,r0 */
L8c617e6c: if(!(*sr&1u))goto L8c617e72;goto L8c617e6e; /* bf 0x8c617e72 */
L8c617e6e: goto L8c618066; /* bra 0x8c618066 */
L8c617e72: r[1]=rd(m,bus,&io_status,0x8c617ec0u,4);if(io_status)return io_status;goto L8c617e74; /* mov.l 0x8c617ec0,r1 */
L8c617e74: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c617e76; /* cmp/eq r1,r0 */
L8c617e76: if(!(*sr&1u))goto L8c617e7c;goto L8c617e78; /* bf 0x8c617e7c */
L8c617e78: goto L8c6182ae; /* bra 0x8c6182ae */
L8c617e7c: r[1]=rd(m,bus,&io_status,0x8c617ebcu,4);if(io_status)return io_status;goto L8c617e7e; /* mov.l 0x8c617ebc,r1 */
L8c617e7e: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c617e80; /* cmp/eq r1,r0 */
L8c617e80: if(!(*sr&1u))goto L8c617e86;goto L8c617e82; /* bf 0x8c617e86 */
L8c617e82: goto L8c618456; /* bra 0x8c618456 */
L8c617e86: goto L8c6184aa; /* bra 0x8c6184aa */
L8c617e8a: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c617e8c; /* mov.w @(0x2,r4),r0 */
L8c617e8c: r[1]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c617e8e; /* mov.l @r11,r1 */
L8c617e8e: r[0]=r[0]&65535u;goto L8c617e90; /* extu.w r0,r0 */
L8c617e90: r[2]=r[0];goto L8c617e92; /* mov r0,r2 */
L8c617e92: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c617e94; /* shll r0 */
L8c617e94: r[0]+=r[2];goto L8c617e96; /* add r2,r0 */
L8c617e96: r[1]+=r[0];goto L8c617e98; /* add r0,r1 */
L8c617e98: wr(m,bus,&io_status,r[11],r[1],4);if(io_status)return io_status;goto L8c617e9a; /* mov.l r1,@r11 */
L8c617e9a: r[14]=r[12];goto L8c618056; /* bra 0x8c618056 */
L8c617ec4: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c617ec6; /* mov.w @r13+,r5 */
L8c617ec6: r[0]=16u;goto L8c617ec8; /* mov #0x10,r0 */
L8c617ec8: r[3]=rd(m,bus,&io_status,0x8c61807cu,4);if(io_status)return io_status;goto L8c617eca; /* mov.l 0x8c61807c,r3 */
L8c617eca: r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c617ecc; /* mov.w @r13+,r6 */
L8c617ecc: r[5]=r[5]&65535u;goto L8c617ece; /* extu.w r5,r5 */
L8c617ece: r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c617ed0; /* mov.l @r3,r12 */
L8c617ed0: r[5]<<=2;goto L8c617ed2; /* shll2 r5 */
L8c617ed2: r[6]=r[6]&65535u;goto L8c617ed4; /* extu.w r6,r6 */
L8c617ed4: r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c617ed6; /* mov.w @r13+,r7 */
L8c617ed6: r[6]<<=2;goto L8c617ed8; /* shll2 r6 */
L8c617ed8: r[6]<<=2;goto L8c617eda; /* shll2 r6 */
L8c617eda: r[5]<<=2;goto L8c617edc; /* shll2 r5 */
L8c617edc: r[7]=r[7]&65535u;goto L8c617ede; /* extu.w r7,r7 */
L8c617ede: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c617ee0; /* shll r5 */
L8c617ee0: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c617ee2; /* shll r6 */
L8c617ee2: r[6]+=r[12];goto L8c617ee4; /* add r12,r6 */
L8c617ee4: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c617ee6; /* fmov.s @(r0,r6),fr4 */
L8c617ee6: r[5]+=r[12];goto L8c617ee8; /* add r12,r5 */
L8c617ee8: r[7]<<=2;goto L8c617eea; /* shll2 r7 */
L8c617eea: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617eec; /* fmov.s @(r0,r5),fr3 */
L8c617eec: r[7]<<=2;goto L8c617eee; /* shll2 r7 */
L8c617eee: fcopy(f,2u,4u);goto L8c617ef0; /* fmov fr4,fr2 */
L8c617ef0: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c617ef2; /* shll r7 */
L8c617ef2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c617ef4; /* fsub fr3,fr2 */
L8c617ef4: r[0]=20u;goto L8c617ef6; /* mov #0x14,r0 */
L8c617ef6: r[7]+=r[12];goto L8c617ef8; /* add r12,r7 */
L8c617ef8: fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c617efa; /* fmov.s @(r0,r6),fr5 */
L8c617efa: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c617efc; /* fmov.s @(r0,r7),fr3 */
L8c617efc: fcopy(f,1u,5u);goto L8c617efe; /* fmov fr5,fr1 */
L8c617efe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617f00; /* fsub fr5,fr3 */
L8c617f00: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617f02; /* fmul fr3,fr2 */
L8c617f02: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f04; /* fmov.s @(r0,r5),fr3 */
L8c617f04: r[0]=16u;goto L8c617f06; /* mov #0x10,r0 */
L8c617f06: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c617f08; /* fsub fr3,fr1 */
L8c617f08: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c617f0a; /* fmov.s @(r0,r7),fr3 */
L8c617f0a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617f0c; /* fsub fr4,fr3 */
L8c617f0c: fcopy(f,4u,2u);goto L8c617f0e; /* fmov fr2,fr4 */
L8c617f0e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c617f10; /* fmul fr3,fr1 */
L8c617f10: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c617f12; /* fldi0 fr3 */
L8c617f12: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c617f14; /* fsub fr1,fr4 */
L8c617f14: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c617f16; /* fcmp/gt fr3,fr4 */
L8c617f16: take=!(*sr&1u);r[14]+=1u;if(take)goto L8c617f1e;goto L8c617f1a; /* bf/s 0x8c617f1e */
L8c617f1a: goto L8c618056; /* bra 0x8c618056 */
L8c617f1e: r[3]=rd(m,bus,&io_status,0x8c618080u,4);if(io_status)return io_status;goto L8c617f20; /* mov.l 0x8c618080,r3 */
L8c617f20: r[0]=4u;goto L8c617f22; /* mov #0x4,r0 */
L8c617f22: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c617f24; /* fmov.s @r5,fr3 */
L8c617f24: r[8]+=32u;goto L8c617f26; /* add #0x20,r8 */
L8c617f26: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c617f28; /* fmov.s @r3,fr2 */
L8c617f28: r[3]+=8u;goto L8c617f2a; /* add #0x8,r3 */
L8c617f2a: r[2]=rd(m,bus,&io_status,0x8c618084u,4);if(io_status)return io_status;goto L8c617f2c; /* mov.l 0x8c618084,r2 */
L8c617f2c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617f2e; /* fmul fr3,fr2 */
L8c617f2e: r[1]=rd(m,bus,&io_status,0x8c618088u,4);if(io_status)return io_status;goto L8c617f30; /* mov.l 0x8c618088,r1 */
L8c617f30: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c617f32; /* fmov.s @r2,fr3 */
L8c617f32: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c617f34; /* fmov.s @r1,fr1 */
L8c617f34: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617f36; /* fsub fr2,fr3 */
L8c617f36: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f38; /* fmov.s @(r0,r5),fr2 */
L8c617f38: r[0]=8u;goto L8c617f3a; /* mov #0x8,r0 */
L8c617f3a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c617f3c; /* fmul fr2,fr1 */
L8c617f3c: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f3e; /* fmov.s @(r0,r5),fr2 */
L8c617f3e: r[0]=28u;goto L8c617f40; /* mov #0x1c,r0 */
L8c617f40: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617f42; /* fsub fr1,fr3 */
L8c617f42: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c617f44; /* fmov.s @r3,fr1 */
L8c617f44: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c617f46; /* fmul fr2,fr1 */
L8c617f46: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617f48; /* fsub fr1,fr3 */
L8c617f48: fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f4a; /* fmov fr3,@(r0,r5) */
L8c617f4a: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c617f4c; /* fmov.s @(r0,r9),fr2 */
L8c617f4c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617f4e; /* fmul fr3,fr2 */
L8c617f4e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c617f50; /* fmov.s fr2,@-r8 */
L8c617f50: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f52; /* fmov.s @(r0,r5),fr3 */
L8c617f52: r[0]=24u;goto L8c617f54; /* mov #0x18,r0 */
L8c617f54: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c617f56; /* fmov.s @(r0,r9),fr2 */
L8c617f56: r[0]=28u;goto L8c617f58; /* mov #0x1c,r0 */
L8c617f58: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617f5a; /* fmul fr3,fr2 */
L8c617f5a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c617f5c; /* fmov.s fr2,@-r8 */
L8c617f5c: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f5e; /* fmov.s @(r0,r5),fr3 */
L8c617f5e: r[0]=20u;goto L8c617f60; /* mov #0x14,r0 */
L8c617f60: r[2]=rd(m,bus,&io_status,0x8c61808cu,4);if(io_status)return io_status;goto L8c617f62; /* mov.l 0x8c61808c,r2 */
L8c617f62: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c617f64; /* fmov.s @(r0,r9),fr2 */
L8c617f64: r[0]=16u;goto L8c617f66; /* mov #0x10,r0 */
L8c617f66: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617f68; /* fmul fr3,fr2 */
L8c617f68: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c617f6a; /* fmov.s fr2,@-r8 */
L8c617f6a: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c617f6c; /* fmov.s @(r0,r9),fr3 */
L8c617f6c: r[0]=24u;goto L8c617f6e; /* mov #0x18,r0 */
L8c617f6e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617f70; /* fmov.s fr3,@-r8 */
L8c617f70: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f72; /* fmov.s @(r0,r5),fr3 */
L8c617f72: r[0]=20u;goto L8c617f74; /* mov #0x14,r0 */
L8c617f74: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617f76; /* fmov.s fr3,@-r8 */
L8c617f76: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f78; /* fmov.s @(r0,r5),fr3 */
L8c617f78: r[0]=16u;goto L8c617f7a; /* mov #0x10,r0 */
L8c617f7a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617f7c; /* fmov.s fr3,@-r8 */
L8c617f7c: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f7e; /* fmov.s @(r0,r5),fr3 */
L8c617f7e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617f80; /* fmov.s fr3,@-r8 */
L8c617f80: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c617f82; /* mov.l r2,@-r8 */
L8c617f82: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617f84; /* pref @r8 */
L8c617f84: r[2]=rd(m,bus,&io_status,0x8c618080u,4);if(io_status)return io_status;goto L8c617f86; /* mov.l 0x8c618080,r2 */
L8c617f86: r[0]=4u;goto L8c617f88; /* mov #0x4,r0 */
L8c617f88: fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c617f8a; /* fmov.s @r6,fr3 */
L8c617f8a: r[8]+=64u;goto L8c617f8c; /* add #0x40,r8 */
L8c617f8c: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c617f8e; /* fmov.s @r2,fr2 */
L8c617f8e: r[3]=rd(m,bus,&io_status,0x8c618084u,4);if(io_status)return io_status;goto L8c617f90; /* mov.l 0x8c618084,r3 */
L8c617f90: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617f92; /* fmul fr3,fr2 */
L8c617f92: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c617f94; /* fmov.s @r1,fr1 */
L8c617f94: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c617f96; /* fmov.s @r3,fr3 */
L8c617f96: r[3]=rd(m,bus,&io_status,0x8c618090u,4);if(io_status)return io_status;goto L8c617f98; /* mov.l 0x8c618090,r3 */
L8c617f98: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617f9a; /* fsub fr2,fr3 */
L8c617f9a: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c617f9c; /* fmov.s @(r0,r6),fr2 */
L8c617f9c: r[0]=8u;goto L8c617f9e; /* mov #0x8,r0 */
L8c617f9e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c617fa0; /* fmul fr2,fr1 */
L8c617fa0: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c617fa2; /* fmov.s @(r0,r6),fr2 */
L8c617fa2: r[0]=28u;goto L8c617fa4; /* mov #0x1c,r0 */
L8c617fa4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617fa6; /* fsub fr1,fr3 */
L8c617fa6: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c617fa8; /* fmov.s @r3,fr1 */
L8c617fa8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c617faa; /* fmul fr2,fr1 */
L8c617faa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617fac; /* fsub fr1,fr3 */
L8c617fac: fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c617fae; /* fmov fr3,@(r0,r6) */
L8c617fae: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c617fb0; /* fmov.s @(r0,r9),fr2 */
L8c617fb0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617fb2; /* fmul fr3,fr2 */
L8c617fb2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c617fb4; /* fmov.s fr2,@-r8 */
L8c617fb4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c617fb6; /* fmov.s @(r0,r6),fr3 */
L8c617fb6: r[0]=24u;goto L8c617fb8; /* mov #0x18,r0 */
L8c617fb8: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c617fba; /* fmov.s @(r0,r9),fr2 */
L8c617fba: r[0]=28u;goto L8c617fbc; /* mov #0x1c,r0 */
L8c617fbc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617fbe; /* fmul fr3,fr2 */
L8c617fbe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c617fc0; /* fmov.s fr2,@-r8 */
L8c617fc0: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c617fc2; /* fmov.s @(r0,r6),fr3 */
L8c617fc2: r[0]=20u;goto L8c617fc4; /* mov #0x14,r0 */
L8c617fc4: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c617fc6; /* fmov.s @(r0,r9),fr2 */
L8c617fc6: r[0]=16u;goto L8c617fc8; /* mov #0x10,r0 */
L8c617fc8: r[2]=rd(m,bus,&io_status,0x8c61808cu,4);if(io_status)return io_status;goto L8c617fca; /* mov.l 0x8c61808c,r2 */
L8c617fca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617fcc; /* fmul fr3,fr2 */
L8c617fcc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c617fce; /* fmov.s fr2,@-r8 */
L8c617fce: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c617fd0; /* fmov.s @(r0,r9),fr3 */
L8c617fd0: r[0]=24u;goto L8c617fd2; /* mov #0x18,r0 */
L8c617fd2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617fd4; /* fmov.s fr3,@-r8 */
L8c617fd4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c617fd6; /* fmov.s @(r0,r6),fr3 */
L8c617fd6: r[0]=20u;goto L8c617fd8; /* mov #0x14,r0 */
L8c617fd8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617fda; /* fmov.s fr3,@-r8 */
L8c617fda: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c617fdc; /* fmov.s @(r0,r6),fr3 */
L8c617fdc: r[0]=16u;goto L8c617fde; /* mov #0x10,r0 */
L8c617fde: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617fe0; /* fmov.s fr3,@-r8 */
L8c617fe0: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c617fe2; /* fmov.s @(r0,r6),fr3 */
L8c617fe2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617fe4; /* fmov.s fr3,@-r8 */
L8c617fe4: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c617fe6; /* mov.l r2,@-r8 */
L8c617fe6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617fe8; /* pref @r8 */
L8c617fe8: r[2]=rd(m,bus,&io_status,0x8c618080u,4);if(io_status)return io_status;goto L8c617fea; /* mov.l 0x8c618080,r2 */
L8c617fea: r[0]=4u;goto L8c617fec; /* mov #0x4,r0 */
L8c617fec: fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c617fee; /* fmov.s @r7,fr3 */
L8c617fee: r[8]+=64u;goto L8c617ff0; /* add #0x40,r8 */
L8c617ff0: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c617ff2; /* fmov.s @r2,fr2 */
L8c617ff2: r[3]=rd(m,bus,&io_status,0x8c618084u,4);if(io_status)return io_status;goto L8c617ff4; /* mov.l 0x8c618084,r3 */
L8c617ff4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617ff6; /* fmul fr3,fr2 */
L8c617ff6: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c617ff8; /* fmov.s @r1,fr1 */
L8c617ff8: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c617ffa; /* fmov.s @r3,fr3 */
L8c617ffa: r[3]=rd(m,bus,&io_status,0x8c618090u,4);if(io_status)return io_status;goto L8c617ffc; /* mov.l 0x8c618090,r3 */
L8c617ffc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617ffe; /* fsub fr2,fr3 */
L8c617ffe: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c618000; /* fmov.s @(r0,r7),fr2 */
L8c618000: r[0]=8u;goto L8c618002; /* mov #0x8,r0 */
L8c618002: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618004; /* fmul fr2,fr1 */
L8c618004: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c618006; /* fmov.s @(r0,r7),fr2 */
L8c618006: r[0]=28u;goto L8c618008; /* mov #0x1c,r0 */
L8c618008: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61800a; /* fsub fr1,fr3 */
L8c61800a: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61800c; /* fmov.s @r3,fr1 */
L8c61800c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61800e; /* fmul fr2,fr1 */
L8c61800e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618010; /* fsub fr1,fr3 */
L8c618010: fstore(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618012; /* fmov fr3,@(r0,r7) */
L8c618012: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618014; /* fmov.s @(r0,r9),fr2 */
L8c618014: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618016; /* fmul fr3,fr2 */
L8c618016: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618018; /* fmov.s fr2,@-r8 */
L8c618018: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61801a; /* fmov.s @(r0,r7),fr3 */
L8c61801a: r[0]=24u;goto L8c61801c; /* mov #0x18,r0 */
L8c61801c: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61801e; /* fmov.s @(r0,r9),fr2 */
L8c61801e: r[0]=28u;goto L8c618020; /* mov #0x1c,r0 */
L8c618020: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618022; /* fmul fr3,fr2 */
L8c618022: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618024; /* fmov.s fr2,@-r8 */
L8c618024: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618026; /* fmov.s @(r0,r7),fr3 */
L8c618026: r[0]=20u;goto L8c618028; /* mov #0x14,r0 */
L8c618028: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61802a; /* fmov.s @(r0,r9),fr2 */
L8c61802a: r[0]=16u;goto L8c61802c; /* mov #0x10,r0 */
L8c61802c: r[2]=rd(m,bus,&io_status,0x8c618094u,4);if(io_status)return io_status;goto L8c61802e; /* mov.l 0x8c618094,r2 */
L8c61802e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618030; /* fmul fr3,fr2 */
L8c618030: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618032; /* fmov.s fr2,@-r8 */
L8c618032: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c618034; /* fmov.s @(r0,r9),fr3 */
L8c618034: r[0]=24u;goto L8c618036; /* mov #0x18,r0 */
L8c618036: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618038; /* fmov.s fr3,@-r8 */
L8c618038: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61803a; /* fmov.s @(r0,r7),fr3 */
L8c61803a: r[0]=20u;goto L8c61803c; /* mov #0x14,r0 */
L8c61803c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61803e; /* fmov.s fr3,@-r8 */
L8c61803e: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618040; /* fmov.s @(r0,r7),fr3 */
L8c618040: r[0]=16u;goto L8c618042; /* mov #0x10,r0 */
L8c618042: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618044; /* fmov.s fr3,@-r8 */
L8c618044: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618046; /* fmov.s @(r0,r7),fr3 */
L8c618046: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618048; /* fmov.s fr3,@-r8 */
L8c618048: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61804a; /* mov.l r2,@-r8 */
L8c61804a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61804c; /* pref @r8 */
L8c61804c: r[0]=rd(m,bus,&io_status,0x8c618098u,4);if(io_status)return io_status;goto L8c61804e; /* mov.l 0x8c618098,r0 */
L8c61804e: r[8]+=32u;goto L8c618050; /* add #0x20,r8 */
L8c618050: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c618052; /* mov.l @r0,r2 */
L8c618052: r[2]+=3u;goto L8c618054; /* add #0x3,r2 */
L8c618054: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c618056; /* mov.l r2,@r0 */
L8c618056: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c618058; /* mov.w @(0x2,r4),r0 */
L8c618058: r[0]=r[0]&65535u;goto L8c61805a; /* extu.w r0,r0 */
L8c61805a: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c61805c; /* cmp/ge r0,r14 */
L8c61805c: if((*sr&1u))goto L8c618062;goto L8c61805e; /* bt 0x8c618062 */
L8c61805e: goto L8c617ec4; /* bra 0x8c617ec4 */
L8c618062: goto L8c6184aa; /* bra 0x8c6184aa */
L8c618066: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c618068; /* mov.w @(0x2,r4),r0 */
L8c618068: r[2]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c61806a; /* mov.l @r11,r2 */
L8c61806a: r[0]=r[0]&65535u;goto L8c61806c; /* extu.w r0,r0 */
L8c61806c: r[0]<<=2;goto L8c61806e; /* shll2 r0 */
L8c61806e: r[2]+=r[0];goto L8c618070; /* add r0,r2 */
L8c618070: wr(m,bus,&io_status,r[11],r[2],4);if(io_status)return io_status;goto L8c618072; /* mov.l r2,@r11 */
L8c618072: r[11]=r[12];goto L8c61829e; /* bra 0x8c61829e */
L8c61809c: r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61809e; /* mov.w @r13+,r6 */
L8c61809e: r[0]=16u;goto L8c6180a0; /* mov #0x10,r0 */
L8c6180a0: r[3]=rd(m,bus,&io_status,0x8c61831cu,4);if(io_status)return io_status;goto L8c6180a2; /* mov.l 0x8c61831c,r3 */
L8c6180a2: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6180a4; /* mov.w @r13+,r5 */
L8c6180a4: r[6]=r[6]&65535u;goto L8c6180a6; /* extu.w r6,r6 */
L8c6180a6: r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6180a8; /* mov.l @r3,r12 */
L8c6180a8: r[6]<<=2;goto L8c6180aa; /* shll2 r6 */
L8c6180aa: r[5]=r[5]&65535u;goto L8c6180ac; /* extu.w r5,r5 */
L8c6180ac: r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6180ae; /* mov.w @r13+,r7 */
L8c6180ae: r[5]<<=2;goto L8c6180b0; /* shll2 r5 */
L8c6180b0: r[5]<<=2;goto L8c6180b2; /* shll2 r5 */
L8c6180b2: r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6180b4; /* mov.w @r13+,r14 */
L8c6180b4: r[6]<<=2;goto L8c6180b6; /* shll2 r6 */
L8c6180b6: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c6180b8; /* shll r6 */
L8c6180b8: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6180ba; /* shll r5 */
L8c6180ba: r[5]+=r[12];goto L8c6180bc; /* add r12,r5 */
L8c6180bc: r[6]+=r[12];goto L8c6180be; /* add r12,r6 */
L8c6180be: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c6180c0; /* fmov.s @(r0,r5),fr4 */
L8c6180c0: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6180c2; /* fmov.s @(r0,r6),fr3 */
L8c6180c2: r[7]=r[7]&65535u;goto L8c6180c4; /* extu.w r7,r7 */
L8c6180c4: fcopy(f,2u,4u);goto L8c6180c6; /* fmov fr4,fr2 */
L8c6180c6: r[7]<<=2;goto L8c6180c8; /* shll2 r7 */
L8c6180c8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6180ca; /* fsub fr3,fr2 */
L8c6180ca: r[14]=r[14]&65535u;goto L8c6180cc; /* extu.w r14,r14 */
L8c6180cc: r[7]<<=2;goto L8c6180ce; /* shll2 r7 */
L8c6180ce: r[14]<<=2;goto L8c6180d0; /* shll2 r14 */
L8c6180d0: r[0]=20u;goto L8c6180d2; /* mov #0x14,r0 */
L8c6180d2: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c6180d4; /* fmov.s @(r0,r5),fr5 */
L8c6180d4: r[14]<<=2;goto L8c6180d6; /* shll2 r14 */
L8c6180d6: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c6180d8; /* shll r7 */
L8c6180d8: r[7]+=r[12];goto L8c6180da; /* add r12,r7 */
L8c6180da: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c6180dc; /* shll r14 */
L8c6180dc: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6180de; /* fmov.s @(r0,r7),fr3 */
L8c6180de: fcopy(f,1u,5u);goto L8c6180e0; /* fmov fr5,fr1 */
L8c6180e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6180e2; /* fsub fr5,fr3 */
L8c6180e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6180e4; /* fmul fr3,fr2 */
L8c6180e4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6180e6; /* fmov.s @(r0,r6),fr3 */
L8c6180e6: r[0]=16u;goto L8c6180e8; /* mov #0x10,r0 */
L8c6180e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6180ea; /* fsub fr3,fr1 */
L8c6180ea: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6180ec; /* fmov.s @(r0,r7),fr3 */
L8c6180ec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6180ee; /* fsub fr4,fr3 */
L8c6180ee: fcopy(f,4u,2u);goto L8c6180f0; /* fmov fr2,fr4 */
L8c6180f0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6180f2; /* fmul fr3,fr1 */
L8c6180f2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6180f4; /* fldi0 fr3 */
L8c6180f4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c6180f6; /* fsub fr1,fr4 */
L8c6180f6: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c6180f8; /* fcmp/gt fr3,fr4 */
L8c6180f8: take=!(*sr&1u);r[14]+=r[12];if(take)goto L8c618100;goto L8c6180fc; /* bf/s 0x8c618100 */
L8c6180fc: goto L8c61829c; /* bra 0x8c61829c */
L8c618100: r[3]=rd(m,bus,&io_status,0x8c618320u,4);if(io_status)return io_status;goto L8c618102; /* mov.l 0x8c618320,r3 */
L8c618102: r[0]=4u;goto L8c618104; /* mov #0x4,r0 */
L8c618104: fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c618106; /* fmov.s @r6,fr3 */
L8c618106: r[8]+=32u;goto L8c618108; /* add #0x20,r8 */
L8c618108: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61810a; /* fmov.s @r3,fr2 */
L8c61810a: r[3]+=8u;goto L8c61810c; /* add #0x8,r3 */
L8c61810c: r[2]=rd(m,bus,&io_status,0x8c618324u,4);if(io_status)return io_status;goto L8c61810e; /* mov.l 0x8c618324,r2 */
L8c61810e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618110; /* fmul fr3,fr2 */
L8c618110: r[1]=rd(m,bus,&io_status,0x8c618328u,4);if(io_status)return io_status;goto L8c618112; /* mov.l 0x8c618328,r1 */
L8c618112: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618114; /* fmov.s @r2,fr3 */
L8c618114: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c618116; /* fmov.s @r1,fr1 */
L8c618116: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618118; /* fsub fr2,fr3 */
L8c618118: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61811a; /* fmov.s @(r0,r6),fr2 */
L8c61811a: r[0]=8u;goto L8c61811c; /* mov #0x8,r0 */
L8c61811c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61811e; /* fmul fr2,fr1 */
L8c61811e: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c618120; /* fmov.s @(r0,r6),fr2 */
L8c618120: r[0]=28u;goto L8c618122; /* mov #0x1c,r0 */
L8c618122: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618124; /* fsub fr1,fr3 */
L8c618124: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618126; /* fmov.s @r3,fr1 */
L8c618126: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618128; /* fmul fr2,fr1 */
L8c618128: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61812a; /* fsub fr1,fr3 */
L8c61812a: fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61812c; /* fmov fr3,@(r0,r6) */
L8c61812c: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61812e; /* fmov.s @(r0,r9),fr2 */
L8c61812e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618130; /* fmul fr3,fr2 */
L8c618130: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618132; /* fmov.s fr2,@-r8 */
L8c618132: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618134; /* fmov.s @(r0,r6),fr3 */
L8c618134: r[0]=24u;goto L8c618136; /* mov #0x18,r0 */
L8c618136: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618138; /* fmov.s @(r0,r9),fr2 */
L8c618138: r[0]=28u;goto L8c61813a; /* mov #0x1c,r0 */
L8c61813a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61813c; /* fmul fr3,fr2 */
L8c61813c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61813e; /* fmov.s fr2,@-r8 */
L8c61813e: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618140; /* fmov.s @(r0,r6),fr3 */
L8c618140: r[0]=20u;goto L8c618142; /* mov #0x14,r0 */
L8c618142: r[2]=rd(m,bus,&io_status,0x8c61832cu,4);if(io_status)return io_status;goto L8c618144; /* mov.l 0x8c61832c,r2 */
L8c618144: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618146; /* fmov.s @(r0,r9),fr2 */
L8c618146: r[0]=16u;goto L8c618148; /* mov #0x10,r0 */
L8c618148: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61814a; /* fmul fr3,fr2 */
L8c61814a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61814c; /* fmov.s fr2,@-r8 */
L8c61814c: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61814e; /* fmov.s @(r0,r9),fr3 */
L8c61814e: r[0]=24u;goto L8c618150; /* mov #0x18,r0 */
L8c618150: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618152; /* fmov.s fr3,@-r8 */
L8c618152: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618154; /* fmov.s @(r0,r6),fr3 */
L8c618154: r[0]=20u;goto L8c618156; /* mov #0x14,r0 */
L8c618156: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618158; /* fmov.s fr3,@-r8 */
L8c618158: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61815a; /* fmov.s @(r0,r6),fr3 */
L8c61815a: r[0]=16u;goto L8c61815c; /* mov #0x10,r0 */
L8c61815c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61815e; /* fmov.s fr3,@-r8 */
L8c61815e: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618160; /* fmov.s @(r0,r6),fr3 */
L8c618160: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618162; /* fmov.s fr3,@-r8 */
L8c618162: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c618164; /* mov.l r2,@-r8 */
L8c618164: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618166; /* pref @r8 */
L8c618166: r[2]=rd(m,bus,&io_status,0x8c618320u,4);if(io_status)return io_status;goto L8c618168; /* mov.l 0x8c618320,r2 */
L8c618168: r[0]=4u;goto L8c61816a; /* mov #0x4,r0 */
L8c61816a: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61816c; /* fmov.s @r5,fr3 */
L8c61816c: r[8]+=64u;goto L8c61816e; /* add #0x40,r8 */
L8c61816e: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618170; /* fmov.s @r2,fr2 */
L8c618170: r[3]=rd(m,bus,&io_status,0x8c618324u,4);if(io_status)return io_status;goto L8c618172; /* mov.l 0x8c618324,r3 */
L8c618172: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618174; /* fmul fr3,fr2 */
L8c618174: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c618176; /* fmov.s @r1,fr1 */
L8c618176: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618178; /* fmov.s @r3,fr3 */
L8c618178: r[3]=rd(m,bus,&io_status,0x8c618330u,4);if(io_status)return io_status;goto L8c61817a; /* mov.l 0x8c618330,r3 */
L8c61817a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61817c; /* fsub fr2,fr3 */
L8c61817c: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61817e; /* fmov.s @(r0,r5),fr2 */
L8c61817e: r[0]=8u;goto L8c618180; /* mov #0x8,r0 */
L8c618180: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618182; /* fmul fr2,fr1 */
L8c618182: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c618184; /* fmov.s @(r0,r5),fr2 */
L8c618184: r[0]=28u;goto L8c618186; /* mov #0x1c,r0 */
L8c618186: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618188; /* fsub fr1,fr3 */
L8c618188: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61818a; /* fmov.s @r3,fr1 */
L8c61818a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61818c; /* fmul fr2,fr1 */
L8c61818c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61818e; /* fsub fr1,fr3 */
L8c61818e: fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618190; /* fmov fr3,@(r0,r5) */
L8c618190: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618192; /* fmov.s @(r0,r9),fr2 */
L8c618192: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618194; /* fmul fr3,fr2 */
L8c618194: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618196; /* fmov.s fr2,@-r8 */
L8c618196: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618198; /* fmov.s @(r0,r5),fr3 */
L8c618198: r[0]=24u;goto L8c61819a; /* mov #0x18,r0 */
L8c61819a: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61819c; /* fmov.s @(r0,r9),fr2 */
L8c61819c: r[0]=28u;goto L8c61819e; /* mov #0x1c,r0 */
L8c61819e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6181a0; /* fmul fr3,fr2 */
L8c6181a0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6181a2; /* fmov.s fr2,@-r8 */
L8c6181a2: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6181a4; /* fmov.s @(r0,r5),fr3 */
L8c6181a4: r[0]=20u;goto L8c6181a6; /* mov #0x14,r0 */
L8c6181a6: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c6181a8; /* fmov.s @(r0,r9),fr2 */
L8c6181a8: r[0]=16u;goto L8c6181aa; /* mov #0x10,r0 */
L8c6181aa: r[2]=rd(m,bus,&io_status,0x8c61832cu,4);if(io_status)return io_status;goto L8c6181ac; /* mov.l 0x8c61832c,r2 */
L8c6181ac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6181ae; /* fmul fr3,fr2 */
L8c6181ae: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6181b0; /* fmov.s fr2,@-r8 */
L8c6181b0: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6181b2; /* fmov.s @(r0,r9),fr3 */
L8c6181b2: r[0]=24u;goto L8c6181b4; /* mov #0x18,r0 */
L8c6181b4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6181b6; /* fmov.s fr3,@-r8 */
L8c6181b6: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6181b8; /* fmov.s @(r0,r5),fr3 */
L8c6181b8: r[0]=20u;goto L8c6181ba; /* mov #0x14,r0 */
L8c6181ba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6181bc; /* fmov.s fr3,@-r8 */
L8c6181bc: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6181be; /* fmov.s @(r0,r5),fr3 */
L8c6181be: r[0]=16u;goto L8c6181c0; /* mov #0x10,r0 */
L8c6181c0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6181c2; /* fmov.s fr3,@-r8 */
L8c6181c2: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6181c4; /* fmov.s @(r0,r5),fr3 */
L8c6181c4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6181c6; /* fmov.s fr3,@-r8 */
L8c6181c6: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c6181c8; /* mov.l r2,@-r8 */
L8c6181c8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6181ca; /* pref @r8 */
L8c6181ca: r[2]=rd(m,bus,&io_status,0x8c618320u,4);if(io_status)return io_status;goto L8c6181cc; /* mov.l 0x8c618320,r2 */
L8c6181cc: r[0]=4u;goto L8c6181ce; /* mov #0x4,r0 */
L8c6181ce: fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c6181d0; /* fmov.s @r7,fr3 */
L8c6181d0: r[8]+=64u;goto L8c6181d2; /* add #0x40,r8 */
L8c6181d2: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c6181d4; /* fmov.s @r2,fr2 */
L8c6181d4: r[3]=rd(m,bus,&io_status,0x8c618324u,4);if(io_status)return io_status;goto L8c6181d6; /* mov.l 0x8c618324,r3 */
L8c6181d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6181d8; /* fmul fr3,fr2 */
L8c6181d8: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c6181da; /* fmov.s @r1,fr1 */
L8c6181da: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6181dc; /* fmov.s @r3,fr3 */
L8c6181dc: r[3]=rd(m,bus,&io_status,0x8c618330u,4);if(io_status)return io_status;goto L8c6181de; /* mov.l 0x8c618330,r3 */
L8c6181de: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6181e0; /* fsub fr2,fr3 */
L8c6181e0: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c6181e2; /* fmov.s @(r0,r7),fr2 */
L8c6181e2: r[0]=8u;goto L8c6181e4; /* mov #0x8,r0 */
L8c6181e4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6181e6; /* fmul fr2,fr1 */
L8c6181e6: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c6181e8; /* fmov.s @(r0,r7),fr2 */
L8c6181e8: r[0]=28u;goto L8c6181ea; /* mov #0x1c,r0 */
L8c6181ea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6181ec; /* fsub fr1,fr3 */
L8c6181ec: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6181ee; /* fmov.s @r3,fr1 */
L8c6181ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6181f0; /* fmul fr2,fr1 */
L8c6181f0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6181f2; /* fsub fr1,fr3 */
L8c6181f2: fstore(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6181f4; /* fmov fr3,@(r0,r7) */
L8c6181f4: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c6181f6; /* fmov.s @(r0,r9),fr2 */
L8c6181f6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6181f8; /* fmul fr3,fr2 */
L8c6181f8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6181fa; /* fmov.s fr2,@-r8 */
L8c6181fa: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6181fc; /* fmov.s @(r0,r7),fr3 */
L8c6181fc: r[0]=24u;goto L8c6181fe; /* mov #0x18,r0 */
L8c6181fe: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618200; /* fmov.s @(r0,r9),fr2 */
L8c618200: r[0]=28u;goto L8c618202; /* mov #0x1c,r0 */
L8c618202: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618204; /* fmul fr3,fr2 */
L8c618204: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618206; /* fmov.s fr2,@-r8 */
L8c618206: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618208; /* fmov.s @(r0,r7),fr3 */
L8c618208: r[0]=20u;goto L8c61820a; /* mov #0x14,r0 */
L8c61820a: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61820c; /* fmov.s @(r0,r9),fr2 */
L8c61820c: r[0]=16u;goto L8c61820e; /* mov #0x10,r0 */
L8c61820e: r[2]=rd(m,bus,&io_status,0x8c61832cu,4);if(io_status)return io_status;goto L8c618210; /* mov.l 0x8c61832c,r2 */
L8c618210: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618212; /* fmul fr3,fr2 */
L8c618212: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618214; /* fmov.s fr2,@-r8 */
L8c618214: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c618216; /* fmov.s @(r0,r9),fr3 */
L8c618216: r[0]=24u;goto L8c618218; /* mov #0x18,r0 */
L8c618218: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61821a; /* fmov.s fr3,@-r8 */
L8c61821a: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61821c; /* fmov.s @(r0,r7),fr3 */
L8c61821c: r[0]=20u;goto L8c61821e; /* mov #0x14,r0 */
L8c61821e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618220; /* fmov.s fr3,@-r8 */
L8c618220: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618222; /* fmov.s @(r0,r7),fr3 */
L8c618222: r[0]=16u;goto L8c618224; /* mov #0x10,r0 */
L8c618224: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618226; /* fmov.s fr3,@-r8 */
L8c618226: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618228; /* fmov.s @(r0,r7),fr3 */
L8c618228: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61822a; /* fmov.s fr3,@-r8 */
L8c61822a: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61822c; /* mov.l r2,@-r8 */
L8c61822c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61822e; /* pref @r8 */
L8c61822e: r[2]=rd(m,bus,&io_status,0x8c618320u,4);if(io_status)return io_status;goto L8c618230; /* mov.l 0x8c618320,r2 */
L8c618230: r[0]=4u;goto L8c618232; /* mov #0x4,r0 */
L8c618232: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c618234; /* fmov.s @r14,fr3 */
L8c618234: r[8]+=64u;goto L8c618236; /* add #0x40,r8 */
L8c618236: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618238; /* fmov.s @r2,fr2 */
L8c618238: r[3]=rd(m,bus,&io_status,0x8c618324u,4);if(io_status)return io_status;goto L8c61823a; /* mov.l 0x8c618324,r3 */
L8c61823a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61823c; /* fmul fr3,fr2 */
L8c61823c: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61823e; /* fmov.s @r1,fr1 */
L8c61823e: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618240; /* fmov.s @r3,fr3 */
L8c618240: r[3]=rd(m,bus,&io_status,0x8c618330u,4);if(io_status)return io_status;goto L8c618242; /* mov.l 0x8c618330,r3 */
L8c618242: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618244; /* fsub fr2,fr3 */
L8c618244: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c618246; /* fmov.s @(r0,r14),fr2 */
L8c618246: r[0]=8u;goto L8c618248; /* mov #0x8,r0 */
L8c618248: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61824a; /* fmul fr2,fr1 */
L8c61824a: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61824c; /* fmov.s @(r0,r14),fr2 */
L8c61824c: r[0]=28u;goto L8c61824e; /* mov #0x1c,r0 */
L8c61824e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618250; /* fsub fr1,fr3 */
L8c618250: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618252; /* fmov.s @r3,fr1 */
L8c618252: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618254; /* fmul fr2,fr1 */
L8c618254: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618256; /* fsub fr1,fr3 */
L8c618256: fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618258; /* fmov fr3,@(r0,r14) */
L8c618258: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61825a; /* fmov.s @(r0,r9),fr2 */
L8c61825a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61825c; /* fmul fr3,fr2 */
L8c61825c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61825e; /* fmov.s fr2,@-r8 */
L8c61825e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618260; /* fmov.s @(r0,r14),fr3 */
L8c618260: r[0]=24u;goto L8c618262; /* mov #0x18,r0 */
L8c618262: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618264; /* fmov.s @(r0,r9),fr2 */
L8c618264: r[0]=28u;goto L8c618266; /* mov #0x1c,r0 */
L8c618266: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618268; /* fmul fr3,fr2 */
L8c618268: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61826a; /* fmov.s fr2,@-r8 */
L8c61826a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61826c; /* fmov.s @(r0,r14),fr3 */
L8c61826c: r[0]=20u;goto L8c61826e; /* mov #0x14,r0 */
L8c61826e: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618270; /* fmov.s @(r0,r9),fr2 */
L8c618270: r[0]=16u;goto L8c618272; /* mov #0x10,r0 */
L8c618272: r[2]=rd(m,bus,&io_status,0x8c618334u,4);if(io_status)return io_status;goto L8c618274; /* mov.l 0x8c618334,r2 */
L8c618274: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618276; /* fmul fr3,fr2 */
L8c618276: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618278; /* fmov.s fr2,@-r8 */
L8c618278: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61827a; /* fmov.s @(r0,r9),fr3 */
L8c61827a: r[0]=24u;goto L8c61827c; /* mov #0x18,r0 */
L8c61827c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61827e; /* fmov.s fr3,@-r8 */
L8c61827e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618280; /* fmov.s @(r0,r14),fr3 */
L8c618280: r[0]=20u;goto L8c618282; /* mov #0x14,r0 */
L8c618282: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618284; /* fmov.s fr3,@-r8 */
L8c618284: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618286; /* fmov.s @(r0,r14),fr3 */
L8c618286: r[0]=16u;goto L8c618288; /* mov #0x10,r0 */
L8c618288: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61828a; /* fmov.s fr3,@-r8 */
L8c61828a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61828c; /* fmov.s @(r0,r14),fr3 */
L8c61828c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61828e; /* fmov.s fr3,@-r8 */
L8c61828e: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c618290; /* mov.l r2,@-r8 */
L8c618290: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618292; /* pref @r8 */
L8c618292: r[0]=rd(m,bus,&io_status,0x8c618338u,4);if(io_status)return io_status;goto L8c618294; /* mov.l 0x8c618338,r0 */
L8c618294: r[8]+=32u;goto L8c618296; /* add #0x20,r8 */
L8c618296: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c618298; /* mov.l @r0,r2 */
L8c618298: r[2]+=4u;goto L8c61829a; /* add #0x4,r2 */
L8c61829a: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61829c; /* mov.l r2,@r0 */
L8c61829c: r[11]+=1u;goto L8c61829e; /* add #0x1,r11 */
L8c61829e: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c6182a0; /* mov.w @(0x2,r4),r0 */
L8c6182a0: r[0]=r[0]&65535u;goto L8c6182a2; /* extu.w r0,r0 */
L8c6182a2: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c6182a4; /* cmp/ge r0,r11 */
L8c6182a4: if((*sr&1u))goto L8c6182aa;goto L8c6182a6; /* bt 0x8c6182aa */
L8c6182a6: goto L8c61809c; /* bra 0x8c61809c */
L8c6182aa: goto L8c6184aa; /* bra 0x8c6184aa */
L8c6182ae: r[10]=r[12];goto L8c618446; /* bra 0x8c618446 */
L8c6182b2: r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6182b4; /* mov.w @r13+,r14 */
L8c6182b4: r[3]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c6182b6; /* mov.l @r11,r3 */
L8c6182b6: r[14]=r[14]&65535u;goto L8c6182b8; /* extu.w r14,r14 */
L8c6182b8: r[2]=rd(m,bus,&io_status,0x8c61831cu,4);if(io_status)return io_status;goto L8c6182ba; /* mov.l 0x8c61831c,r2 */
L8c6182ba: r[3]+=r[14];goto L8c6182bc; /* add r14,r3 */
L8c6182bc: wr(m,bus,&io_status,r[11],r[3],4);if(io_status)return io_status;goto L8c6182be; /* mov.l r3,@r11 */
L8c6182be: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c6182c0; /* mov.w @(0x2,r13),r0 */
L8c6182c0: r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c6182c2; /* mov.w @r13,r7 */
L8c6182c2: r[5]=r[0]&65535u;goto L8c6182c4; /* extu.w r0,r5 */
L8c6182c4: r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c6182c6; /* mov.w @(0x4,r13),r0 */
L8c6182c6: r[5]<<=2;goto L8c6182c8; /* shll2 r5 */
L8c6182c8: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6182ca; /* mov.l @r2,r3 */
L8c6182ca: r[7]=r[7]&65535u;goto L8c6182cc; /* extu.w r7,r7 */
L8c6182cc: r[7]<<=2;goto L8c6182ce; /* shll2 r7 */
L8c6182ce: r[5]<<=2;goto L8c6182d0; /* shll2 r5 */
L8c6182d0: r[6]=r[0]&65535u;goto L8c6182d2; /* extu.w r0,r6 */
L8c6182d2: r[7]<<=2;goto L8c6182d4; /* shll2 r7 */
L8c6182d4: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c6182d6; /* shll r7 */
L8c6182d6: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6182d8; /* shll r5 */
L8c6182d8: r[0]=16u;goto L8c6182da; /* mov #0x10,r0 */
L8c6182da: r[5]+=r[3];goto L8c6182dc; /* add r3,r5 */
L8c6182dc: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c6182de; /* fmov.s @(r0,r5),fr5 */
L8c6182de: r[7]+=r[3];goto L8c6182e0; /* add r3,r7 */
L8c6182e0: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6182e2; /* fmov.s @(r0,r7),fr3 */
L8c6182e2: r[6]<<=2;goto L8c6182e4; /* shll2 r6 */
L8c6182e4: fcopy(f,2u,5u);goto L8c6182e6; /* fmov fr5,fr2 */
L8c6182e6: r[6]<<=2;goto L8c6182e8; /* shll2 r6 */
L8c6182e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6182ea; /* fsub fr3,fr2 */
L8c6182ea: r[0]=20u;goto L8c6182ec; /* mov #0x14,r0 */
L8c6182ec: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c6182ee; /* shll r6 */
L8c6182ee: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c6182f0; /* fmov.s @(r0,r5),fr4 */
L8c6182f0: r[6]+=r[3];goto L8c6182f2; /* add r3,r6 */
L8c6182f2: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6182f4; /* fmov.s @(r0,r6),fr3 */
L8c6182f4: fcopy(f,1u,4u);goto L8c6182f6; /* fmov fr4,fr1 */
L8c6182f6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6182f8; /* fsub fr4,fr3 */
L8c6182f8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6182fa; /* fmul fr3,fr2 */
L8c6182fa: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6182fc; /* fmov.s @(r0,r7),fr3 */
L8c6182fc: r[0]=16u;goto L8c6182fe; /* mov #0x10,r0 */
L8c6182fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c618300; /* fsub fr3,fr1 */
L8c618300: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618302; /* fmov.s @(r0,r6),fr3 */
L8c618302: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618304; /* fsub fr5,fr3 */
L8c618304: fcopy(f,4u,2u);goto L8c618306; /* fmov fr2,fr4 */
L8c618306: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618308; /* fmul fr3,fr1 */
L8c618308: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61830a; /* fldi0 fr3 */
L8c61830a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61830c; /* fsub fr1,fr4 */
L8c61830c: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61830e; /* fcmp/gt fr3,fr4 */
L8c61830e: if(!(*sr&1u))goto L8c61833c;goto L8c618310; /* bf 0x8c61833c */
L8c618310: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c618312; /* shll r14 */
L8c618312: r[13]+=r[14];goto L8c618444; /* bra 0x8c618444 */
L8c61833c: r[3]=rd(m,bus,&io_status,0x8c6184c8u,4);if(io_status)return io_status;goto L8c61833e; /* mov.l 0x8c6184c8,r3 */
L8c61833e: r[7]=r[14];goto L8c618340; /* mov r14,r7 */
L8c618340: r[7]+=4294967295u;goto L8c618342; /* add #-0x1,r7 */
L8c618342: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c618344; /* mov.l @r3,r2 */
L8c618344: *sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c618346; /* cmp/pl r7 */
L8c618346: r[2]+=r[14];goto L8c618348; /* add r14,r2 */
L8c618348: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61834a; /* mov.l r2,@r3 */
L8c61834a: take=!(*sr&1u);r[6]=r[12];if(take)goto L8c6183cc;goto L8c61834e; /* bf/s 0x8c6183cc */
L8c61834e: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c618350; /* mov.w @r13+,r5 */
L8c618350: r[0]=4u;goto L8c618352; /* mov #0x4,r0 */
L8c618352: r[2]=rd(m,bus,&io_status,0x8c6184ccu,4);if(io_status)return io_status;goto L8c618354; /* mov.l 0x8c6184cc,r2 */
L8c618354: r[8]+=32u;goto L8c618356; /* add #0x20,r8 */
L8c618356: r[5]=r[5]&65535u;goto L8c618358; /* extu.w r5,r5 */
L8c618358: r[1]=rd(m,bus,&io_status,0x8c6184d0u,4);if(io_status)return io_status;goto L8c61835a; /* mov.l 0x8c6184d0,r1 */
L8c61835a: r[5]<<=2;goto L8c61835c; /* shll2 r5 */
L8c61835c: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61835e; /* mov.l @r2,r3 */
L8c61835e: r[5]<<=2;goto L8c618360; /* shll2 r5 */
L8c618360: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618362; /* fmov.s @r1,fr2 */
L8c618362: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c618364; /* shll r5 */
L8c618364: r[2]=rd(m,bus,&io_status,0x8c6184d8u,4);if(io_status)return io_status;goto L8c618366; /* mov.l 0x8c6184d8,r2 */
L8c618366: r[5]+=r[3];goto L8c618368; /* add r3,r5 */
L8c618368: r[3]=rd(m,bus,&io_status,0x8c6184d4u,4);if(io_status)return io_status;goto L8c61836a; /* mov.l 0x8c6184d4,r3 */
L8c61836a: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61836c; /* fmov.s @r5,fr3 */
L8c61836c: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61836e; /* fmov.s @r2,fr1 */
L8c61836e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618370; /* fmul fr3,fr2 */
L8c618370: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618372; /* fmov.s @r3,fr3 */
L8c618372: r[3]=rd(m,bus,&io_status,0x8c6184dcu,4);if(io_status)return io_status;goto L8c618374; /* mov.l 0x8c6184dc,r3 */
L8c618374: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618376; /* fsub fr2,fr3 */
L8c618376: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c618378; /* fmov.s @(r0,r5),fr2 */
L8c618378: r[0]=8u;goto L8c61837a; /* mov #0x8,r0 */
L8c61837a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61837c; /* fmul fr2,fr1 */
L8c61837c: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61837e; /* fmov.s @(r0,r5),fr2 */
L8c61837e: r[0]=28u;goto L8c618380; /* mov #0x1c,r0 */
L8c618380: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618382; /* fsub fr1,fr3 */
L8c618382: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618384; /* fmov.s @r3,fr1 */
L8c618384: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618386; /* fmul fr2,fr1 */
L8c618386: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618388; /* fsub fr1,fr3 */
L8c618388: fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61838a; /* fmov fr3,@(r0,r5) */
L8c61838a: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61838c; /* fmov.s @(r0,r9),fr2 */
L8c61838c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61838e; /* fmul fr3,fr2 */
L8c61838e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618390; /* fmov.s fr2,@-r8 */
L8c618390: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618392; /* fmov.s @(r0,r5),fr3 */
L8c618392: r[0]=24u;goto L8c618394; /* mov #0x18,r0 */
L8c618394: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618396; /* fmov.s @(r0,r9),fr2 */
L8c618396: r[0]=28u;goto L8c618398; /* mov #0x1c,r0 */
L8c618398: r[2]=rd(m,bus,&io_status,0x8c6184e0u,4);if(io_status)return io_status;goto L8c61839a; /* mov.l 0x8c6184e0,r2 */
L8c61839a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61839c; /* fmul fr3,fr2 */
L8c61839c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61839e; /* fmov.s fr2,@-r8 */
L8c61839e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6183a0; /* fmov.s @(r0,r5),fr3 */
L8c6183a0: r[0]=20u;goto L8c6183a2; /* mov #0x14,r0 */
L8c6183a2: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c6183a4; /* fmov.s @(r0,r9),fr2 */
L8c6183a4: r[0]=16u;goto L8c6183a6; /* mov #0x10,r0 */
L8c6183a6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6183a8; /* fmul fr3,fr2 */
L8c6183a8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6183aa; /* fmov.s fr2,@-r8 */
L8c6183aa: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6183ac; /* fmov.s @(r0,r9),fr3 */
L8c6183ac: r[0]=24u;goto L8c6183ae; /* mov #0x18,r0 */
L8c6183ae: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6183b0; /* fmov.s fr3,@-r8 */
L8c6183b0: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6183b2; /* fmov.s @(r0,r5),fr3 */
L8c6183b2: r[0]=20u;goto L8c6183b4; /* mov #0x14,r0 */
L8c6183b4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6183b6; /* fmov.s fr3,@-r8 */
L8c6183b6: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6183b8; /* fmov.s @(r0,r5),fr3 */
L8c6183b8: r[0]=16u;goto L8c6183ba; /* mov #0x10,r0 */
L8c6183ba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6183bc; /* fmov.s fr3,@-r8 */
L8c6183bc: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6183be; /* fmov.s @(r0,r5),fr3 */
L8c6183be: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6183c0; /* fmov.s fr3,@-r8 */
L8c6183c0: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c6183c2; /* mov.l r2,@-r8 */
L8c6183c2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6183c4; /* pref @r8 */
L8c6183c4: r[6]+=1u;goto L8c6183c6; /* add #0x1,r6 */
L8c6183c6: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c6183c8; /* cmp/ge r7,r6 */
L8c6183c8: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61834e;goto L8c6183cc; /* bf/s 0x8c61834e */
L8c6183cc: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6183ce; /* mov.w @r13+,r5 */
L8c6183ce: r[0]=4u;goto L8c6183d0; /* mov #0x4,r0 */
L8c6183d0: r[2]=rd(m,bus,&io_status,0x8c6184ccu,4);if(io_status)return io_status;goto L8c6183d2; /* mov.l 0x8c6184cc,r2 */
L8c6183d2: r[8]+=32u;goto L8c6183d4; /* add #0x20,r8 */
L8c6183d4: r[5]=r[5]&65535u;goto L8c6183d6; /* extu.w r5,r5 */
L8c6183d6: r[1]=rd(m,bus,&io_status,0x8c6184d0u,4);if(io_status)return io_status;goto L8c6183d8; /* mov.l 0x8c6184d0,r1 */
L8c6183d8: r[5]<<=2;goto L8c6183da; /* shll2 r5 */
L8c6183da: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6183dc; /* mov.l @r2,r3 */
L8c6183dc: r[5]<<=2;goto L8c6183de; /* shll2 r5 */
L8c6183de: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c6183e0; /* fmov.s @r1,fr2 */
L8c6183e0: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6183e2; /* shll r5 */
L8c6183e2: r[2]=rd(m,bus,&io_status,0x8c6184d8u,4);if(io_status)return io_status;goto L8c6183e4; /* mov.l 0x8c6184d8,r2 */
L8c6183e4: r[5]+=r[3];goto L8c6183e6; /* add r3,r5 */
L8c6183e6: r[3]=rd(m,bus,&io_status,0x8c6184d4u,4);if(io_status)return io_status;goto L8c6183e8; /* mov.l 0x8c6184d4,r3 */
L8c6183e8: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c6183ea; /* fmov.s @r5,fr3 */
L8c6183ea: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c6183ec; /* fmov.s @r2,fr1 */
L8c6183ec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6183ee; /* fmul fr3,fr2 */
L8c6183ee: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6183f0; /* fmov.s @r3,fr3 */
L8c6183f0: r[3]=rd(m,bus,&io_status,0x8c6184dcu,4);if(io_status)return io_status;goto L8c6183f2; /* mov.l 0x8c6184dc,r3 */
L8c6183f2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6183f4; /* fsub fr2,fr3 */
L8c6183f4: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c6183f6; /* fmov.s @(r0,r5),fr2 */
L8c6183f6: r[0]=8u;goto L8c6183f8; /* mov #0x8,r0 */
L8c6183f8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6183fa; /* fmul fr2,fr1 */
L8c6183fa: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c6183fc; /* fmov.s @(r0,r5),fr2 */
L8c6183fc: r[0]=28u;goto L8c6183fe; /* mov #0x1c,r0 */
L8c6183fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618400; /* fsub fr1,fr3 */
L8c618400: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618402; /* fmov.s @r3,fr1 */
L8c618402: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618404; /* fmul fr2,fr1 */
L8c618404: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618406; /* fsub fr1,fr3 */
L8c618406: fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618408; /* fmov fr3,@(r0,r5) */
L8c618408: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61840a; /* fmov.s @(r0,r9),fr2 */
L8c61840a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61840c; /* fmul fr3,fr2 */
L8c61840c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61840e; /* fmov.s fr2,@-r8 */
L8c61840e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618410; /* fmov.s @(r0,r5),fr3 */
L8c618410: r[0]=24u;goto L8c618412; /* mov #0x18,r0 */
L8c618412: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618414; /* fmov.s @(r0,r9),fr2 */
L8c618414: r[0]=28u;goto L8c618416; /* mov #0x1c,r0 */
L8c618416: r[2]=rd(m,bus,&io_status,0x8c6184e4u,4);if(io_status)return io_status;goto L8c618418; /* mov.l 0x8c6184e4,r2 */
L8c618418: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61841a; /* fmul fr3,fr2 */
L8c61841a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61841c; /* fmov.s fr2,@-r8 */
L8c61841c: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61841e; /* fmov.s @(r0,r5),fr3 */
L8c61841e: r[0]=20u;goto L8c618420; /* mov #0x14,r0 */
L8c618420: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618422; /* fmov.s @(r0,r9),fr2 */
L8c618422: r[0]=16u;goto L8c618424; /* mov #0x10,r0 */
L8c618424: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618426; /* fmul fr3,fr2 */
L8c618426: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618428; /* fmov.s fr2,@-r8 */
L8c618428: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61842a; /* fmov.s @(r0,r9),fr3 */
L8c61842a: r[0]=24u;goto L8c61842c; /* mov #0x18,r0 */
L8c61842c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61842e; /* fmov.s fr3,@-r8 */
L8c61842e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618430; /* fmov.s @(r0,r5),fr3 */
L8c618430: r[0]=20u;goto L8c618432; /* mov #0x14,r0 */
L8c618432: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618434; /* fmov.s fr3,@-r8 */
L8c618434: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618436; /* fmov.s @(r0,r5),fr3 */
L8c618436: r[0]=16u;goto L8c618438; /* mov #0x10,r0 */
L8c618438: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61843a; /* fmov.s fr3,@-r8 */
L8c61843a: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61843c; /* fmov.s @(r0,r5),fr3 */
L8c61843c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61843e; /* fmov.s fr3,@-r8 */
L8c61843e: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c618440; /* mov.l r2,@-r8 */
L8c618440: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618442; /* pref @r8 */
L8c618442: r[8]+=32u;goto L8c618444; /* add #0x20,r8 */
L8c618444: r[10]+=1u;goto L8c618446; /* add #0x1,r10 */
L8c618446: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c618448; /* mov.w @(0x2,r4),r0 */
L8c618448: r[0]=r[0]&65535u;goto L8c61844a; /* extu.w r0,r0 */
L8c61844a: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c61844c; /* cmp/ge r0,r10 */
L8c61844c: if((*sr&1u))goto L8c618452;goto L8c61844e; /* bt 0x8c618452 */
L8c61844e: goto L8c6182b2; /* bra 0x8c6182b2 */
L8c618452: goto L8c6184aa; /* bra 0x8c6184aa */
L8c618456: r[1]=rd(m,bus,&io_status,0x8c6184e8u,4);if(io_status)return io_status;goto L8c618458; /* mov.l 0x8c6184e8,r1 */
L8c618458: wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c61845a; /* mov.l r4,@r1 */
L8c61845a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61845c; /* frchg */
L8c61845c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61845e; /* fmov.s fr0,@-r15 */
L8c61845e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c618460; /* fmov.s fr1,@-r15 */
L8c618460: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c618462; /* fmov.s fr2,@-r15 */
L8c618462: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c618464; /* fmov.s fr3,@-r15 */
L8c618464: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c618466; /* fmov.s fr4,@-r15 */
L8c618466: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c618468; /* fmov.s fr5,@-r15 */
L8c618468: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61846a; /* fmov.s fr6,@-r15 */
L8c61846a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61846c; /* fmov.s fr7,@-r15 */
L8c61846c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61846e; /* fmov.s fr8,@-r15 */
L8c61846e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c618470; /* fmov.s fr9,@-r15 */
L8c618470: r[0]=rd(m,bus,&io_status,0x8c6184bcu,4);if(io_status)return io_status;goto L8c618472; /* mov.l 0x8c6184bc,r0 */
L8c618472: fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618474; /* fmov.s @r0+,fr0 */
L8c618474: fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618476; /* fmov.s @r0+,fr1 */
L8c618476: fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c618478; /* fmov.s @r0,fr2 */
L8c618478: r[0]=rd(m,bus,&io_status,0x8c6184c0u,4);if(io_status)return io_status;goto L8c61847a; /* mov.l 0x8c6184c0,r0 */
L8c61847a: r[0]+=16u;goto L8c61847c; /* add #0x10,r0 */
L8c61847c: fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61847e; /* fmov.s @r0+,fr4 */
L8c61847e: fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618480; /* fmov.s @r0+,fr5 */
L8c618480: fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618482; /* fmov.s @r0+,fr6 */
L8c618482: fload(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;goto L8c618484; /* fmov.s @r0,fr7 */
L8c618484: r[0]=rd(m,bus,&io_status,0x8c6184c4u,4);if(io_status)return io_status;goto L8c618486; /* mov.l 0x8c6184c4,r0 */
L8c618486: fload(m,bus,&io_status,f,8u,r[0]);if(io_status)return io_status;goto L8c618488; /* fmov.s @r0,fr8 */
L8c618488: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61848a; /* frchg */
L8c61848a: r[2]=rd(m,bus,&io_status,0x8c6184ecu,4);if(io_status)return io_status;goto L8c61848c; /* mov.l 0x8c6184ec,r2 */
L8c61848c: r[3]=rd(m,bus,&io_status,0x8c6184e8u,4);if(io_status)return io_status;goto L8c61848e; /* mov.l 0x8c6184e8,r3 */
L8c61848e: target=r[2];*pr=0x8c618492u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c618492; /* jsr @r2 */
L8c618492: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c618494; /* frchg */
L8c618494: fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618496; /* fmov.s @r15+,fr9 */
L8c618496: fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618498; /* fmov.s @r15+,fr8 */
L8c618498: fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61849a; /* fmov.s @r15+,fr7 */
L8c61849a: fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61849c; /* fmov.s @r15+,fr6 */
L8c61849c: fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61849e; /* fmov.s @r15+,fr5 */
L8c61849e: fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6184a0; /* fmov.s @r15+,fr4 */
L8c6184a0: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6184a2; /* fmov.s @r15+,fr3 */
L8c6184a2: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6184a4; /* fmov.s @r15+,fr2 */
L8c6184a4: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6184a6; /* fmov.s @r15+,fr1 */
L8c6184a6: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6184a8; /* fmov.s @r15+,fr0 */
L8c6184a8: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6184aa; /* frchg */
L8c6184aa: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6184ac; /* lds.l @r15+,PR */
L8c6184ac: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6184ae; /* mov.l @r15+,r9 */
L8c6184ae: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6184b0; /* mov.l @r15+,r10 */
L8c6184b0: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6184b2; /* mov.l @r15+,r11 */
L8c6184b2: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6184b4; /* mov.l @r15+,r12 */
L8c6184b4: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6184b6; /* mov.l @r15+,r13 */
L8c6184b6: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_lit_primitive_clipped_8c6184f0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6184f0;
L8c6184f0: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6184f2; /* mov.l r14,@-r15 */
L8c6184f2: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6184f4; /* mov.l r13,@-r15 */
L8c6184f4: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6184f6; /* mov.l r12,@-r15 */
L8c6184f6: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6184f8; /* mov.l r11,@-r15 */
L8c6184f8: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6184fa; /* mov.l r10,@-r15 */
L8c6184fa: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6184fc; /* mov.l r9,@-r15 */
L8c6184fc: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6184fe; /* sts.l PR,@-r15 */
L8c6184fe: r[15]+=4294967288u;goto L8c618500; /* add #-0x8,r15 */
L8c618500: r[0]=r[4];goto L8c618502; /* mov r4,r0 */
L8c618502: r[3]=rd(m,bus,&io_status,0x8c618554u,4);if(io_status)return io_status;goto L8c618504; /* mov.l 0x8c618554,r3 */
L8c618504: wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c618506; /* mov.l r4,@r15 */
L8c618506: r[11]=r[4];goto L8c618508; /* mov r4,r11 */
L8c618508: r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61850a; /* mov.w @r0,r0 */
L8c61850a: r[0]=r[0]&65535u;goto L8c61850c; /* extu.w r0,r0 */
L8c61850c: r[0]&=r[3];goto L8c61850e; /* and r3,r0 */
L8c61850e: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c618510; /* cmp/eq #0x0,r0 */
L8c618510: take=(*sr&1u);r[11]=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;if(take)goto L8c618536;goto L8c618514; /* bt/s 0x8c618536 */
L8c618514: r[1]=sx(m,bus,&io_status,0x8c618552u,2);if(io_status)return io_status;goto L8c618516; /* mov.w 0x8c618552,r1 */
L8c618516: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c618518; /* cmp/eq r1,r0 */
L8c618518: if(!(*sr&1u))goto L8c61851e;goto L8c61851a; /* bf 0x8c61851e */
L8c61851a: goto L8c61873c; /* bra 0x8c61873c */
L8c61851e: r[1]=rd(m,bus,&io_status,0x8c618558u,4);if(io_status)return io_status;goto L8c618520; /* mov.l 0x8c618558,r1 */
L8c618520: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c618522; /* cmp/eq r1,r0 */
L8c618522: if(!(*sr&1u))goto L8c618528;goto L8c618524; /* bf 0x8c618528 */
L8c618524: goto L8c618a06; /* bra 0x8c618a06 */
L8c618528: r[1]=rd(m,bus,&io_status,0x8c618554u,4);if(io_status)return io_status;goto L8c61852a; /* mov.l 0x8c618554,r1 */
L8c61852a: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61852c; /* cmp/eq r1,r0 */
L8c61852c: if(!(*sr&1u))goto L8c618532;goto L8c61852e; /* bf 0x8c618532 */
L8c61852e: goto L8c618c5e; /* bra 0x8c618c5e */
L8c618532: goto L8c618cb4; /* bra 0x8c618cb4 */
L8c618536: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c618538; /* mov.l @r15,r2 */
L8c618538: r[1]=rd(m,bus,&io_status,0x8c61855cu,4);if(io_status)return io_status;goto L8c61853a; /* mov.l 0x8c61855c,r1 */
L8c61853a: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61853c; /* mov.w @(0x2,r2),r0 */
L8c61853c: r[0]=r[0]&65535u;goto L8c61853e; /* extu.w r0,r0 */
L8c61853e: r[2]=r[0];goto L8c618540; /* mov r0,r2 */
L8c618540: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c618542; /* shll r0 */
L8c618542: r[0]+=r[2];goto L8c618544; /* add r2,r0 */
L8c618544: r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c618546; /* mov.l @r1,r2 */
L8c618546: r[2]+=r[0];goto L8c618548; /* add r0,r2 */
L8c618548: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61854a; /* mov.l r2,@r1 */
L8c61854a: r[14]=0u;goto L8c61872a; /* bra 0x8c61872a */
L8c618560: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618562; /* mov.w @r11+,r4 */
L8c618562: r[3]=rd(m,bus,&io_status,0x8c618750u,4);if(io_status)return io_status;goto L8c618564; /* mov.l 0x8c618750,r3 */
L8c618564: r[5]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618566; /* mov.w @r11+,r5 */
L8c618566: r[4]=r[4]&65535u;goto L8c618568; /* extu.w r4,r4 */
L8c618568: r[4]<<=2;goto L8c61856a; /* shll2 r4 */
L8c61856a: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61856c; /* mov.l @r3,r13 */
L8c61856c: r[6]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61856e; /* mov.w @r11+,r6 */
L8c61856e: r[5]=r[5]&65535u;goto L8c618570; /* extu.w r5,r5 */
L8c618570: r[5]<<=2;goto L8c618572; /* shll2 r5 */
L8c618572: r[6]=r[6]&65535u;goto L8c618574; /* extu.w r6,r6 */
L8c618574: r[5]<<=2;goto L8c618576; /* shll2 r5 */
L8c618576: r[6]<<=2;goto L8c618578; /* shll2 r6 */
L8c618578: r[4]<<=2;goto L8c61857a; /* shll2 r4 */
L8c61857a: r[6]<<=2;goto L8c61857c; /* shll2 r6 */
L8c61857c: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61857e; /* shll r4 */
L8c61857e: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c618580; /* shll r5 */
L8c618580: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c618582; /* shll r6 */
L8c618582: r[4]+=r[13];goto L8c618584; /* add r13,r4 */
L8c618584: r[5]+=r[13];goto L8c618586; /* add r13,r5 */
L8c618586: r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c618588; /* mov.l @(0xc,r4),r7 */
L8c618588: r[2]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c61858a; /* mov.l @(0xc,r5),r2 */
L8c61858a: r[6]+=r[13];goto L8c61858c; /* add r13,r6 */
L8c61858c: r[1]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c61858e; /* mov.l @(0xc,r6),r1 */
L8c61858e: r[7]&=r[2];goto L8c618590; /* and r2,r7 */
L8c618590: r[2]=63u;goto L8c618592; /* mov #0x3f,r2 */
L8c618592: r[7]&=r[1];goto L8c618594; /* and r1,r7 */
L8c618594: *sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c618596; /* tst r2,r7 */
L8c618596: if((*sr&1u))goto L8c61859c;goto L8c618598; /* bt 0x8c61859c */
L8c618598: goto L8c618728; /* bra 0x8c618728 */
L8c61859c: r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61859e; /* mov.l @(0xc,r4),r7 */
L8c61859e: r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c6185a0; /* mov.l @(0xc,r5),r3 */
L8c6185a0: r[2]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c6185a2; /* mov.l @(0xc,r6),r2 */
L8c6185a2: r[7]|=r[3];goto L8c6185a4; /* or r3,r7 */
L8c6185a4: r[3]=1u;goto L8c6185a6; /* mov #0x1,r3 */
L8c6185a6: r[7]|=r[2];goto L8c6185a8; /* or r2,r7 */
L8c6185a8: *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c6185aa; /* tst r3,r7 */
L8c6185aa: if((*sr&1u))goto L8c6185b0;goto L8c6185ac; /* bt 0x8c6185b0 */
L8c6185ac: goto L8c61871e; /* bra 0x8c61871e */
L8c6185b0: r[0]=16u;goto L8c6185b2; /* mov #0x10,r0 */
L8c6185b2: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c6185b4; /* fmov.s @(r0,r5),fr5 */
L8c6185b4: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6185b6; /* fmov.s @(r0,r4),fr3 */
L8c6185b6: r[0]=20u;goto L8c6185b8; /* mov #0x14,r0 */
L8c6185b8: fcopy(f,2u,5u);goto L8c6185ba; /* fmov fr5,fr2 */
L8c6185ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6185bc; /* fsub fr3,fr2 */
L8c6185bc: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6185be; /* fmov.s @(r0,r6),fr3 */
L8c6185be: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c6185c0; /* fmov.s @(r0,r5),fr4 */
L8c6185c0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6185c2; /* fsub fr4,fr3 */
L8c6185c2: fcopy(f,1u,4u);goto L8c6185c4; /* fmov fr4,fr1 */
L8c6185c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6185c6; /* fmul fr3,fr2 */
L8c6185c6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6185c8; /* fmov.s @(r0,r4),fr3 */
L8c6185c8: r[0]=16u;goto L8c6185ca; /* mov #0x10,r0 */
L8c6185ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6185cc; /* fsub fr3,fr1 */
L8c6185cc: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6185ce; /* fmov.s @(r0,r6),fr3 */
L8c6185ce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6185d0; /* fsub fr5,fr3 */
L8c6185d0: fcopy(f,4u,2u);goto L8c6185d2; /* fmov fr2,fr4 */
L8c6185d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6185d4; /* fmul fr3,fr1 */
L8c6185d4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6185d6; /* fldi0 fr3 */
L8c6185d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c6185d8; /* fsub fr1,fr4 */
L8c6185d8: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c6185da; /* fcmp/gt fr3,fr4 */
L8c6185da: if(!(*sr&1u))goto L8c6185e0;goto L8c6185dc; /* bf 0x8c6185e0 */
L8c6185dc: goto L8c618728; /* bra 0x8c618728 */
L8c6185e0: r[3]=rd(m,bus,&io_status,0x8c618754u,4);if(io_status)return io_status;goto L8c6185e2; /* mov.l 0x8c618754,r3 */
L8c6185e2: r[0]=4u;goto L8c6185e4; /* mov #0x4,r0 */
L8c6185e4: fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c6185e6; /* fmov.s @r4,fr3 */
L8c6185e6: r[8]+=32u;goto L8c6185e8; /* add #0x20,r8 */
L8c6185e8: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6185ea; /* fmov.s @r3,fr2 */
L8c6185ea: r[3]+=8u;goto L8c6185ec; /* add #0x8,r3 */
L8c6185ec: r[2]=rd(m,bus,&io_status,0x8c618758u,4);if(io_status)return io_status;goto L8c6185ee; /* mov.l 0x8c618758,r2 */
L8c6185ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6185f0; /* fmul fr3,fr2 */
L8c6185f0: r[1]=rd(m,bus,&io_status,0x8c61875cu,4);if(io_status)return io_status;goto L8c6185f2; /* mov.l 0x8c61875c,r1 */
L8c6185f2: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6185f4; /* fmov.s @r2,fr3 */
L8c6185f4: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c6185f6; /* fmov.s @r1,fr1 */
L8c6185f6: r[2]=rd(m,bus,&io_status,0x8c618760u,4);if(io_status)return io_status;goto L8c6185f8; /* mov.l 0x8c618760,r2 */
L8c6185f8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6185fa; /* fsub fr2,fr3 */
L8c6185fa: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6185fc; /* fmov.s @(r0,r4),fr2 */
L8c6185fc: r[0]=8u;goto L8c6185fe; /* mov #0x8,r0 */
L8c6185fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618600; /* fmul fr2,fr1 */
L8c618600: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c618602; /* fmov.s @(r0,r4),fr2 */
L8c618602: r[0]=28u;goto L8c618604; /* mov #0x1c,r0 */
L8c618604: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618606; /* fsub fr1,fr3 */
L8c618606: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618608; /* fmov.s @r3,fr1 */
L8c618608: r[3]=rd(m,bus,&io_status,0x8c618764u,4);if(io_status)return io_status;goto L8c61860a; /* mov.l 0x8c618764,r3 */
L8c61860a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61860c; /* fmul fr2,fr1 */
L8c61860c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61860e; /* fsub fr1,fr3 */
L8c61860e: fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618610; /* fmov fr3,@(r0,r4) */
L8c618610: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618612; /* fmov.s @r2,fr2 */
L8c618612: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618614; /* fmul fr3,fr2 */
L8c618614: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618616; /* fmov.s fr2,@-r8 */
L8c618616: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618618; /* fmov.s @r3,fr2 */
L8c618618: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61861a; /* fmov.s @(r0,r4),fr3 */
L8c61861a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61861c; /* fmul fr3,fr2 */
L8c61861c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61861e; /* fmov.s fr2,@-r8 */
L8c61861e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618620; /* fmov.s @(r0,r4),fr3 */
L8c618620: r[1]=rd(m,bus,&io_status,0x8c618768u,4);if(io_status)return io_status;goto L8c618622; /* mov.l 0x8c618768,r1 */
L8c618622: r[2]+=4294967284u;goto L8c618624; /* add #-0xc,r2 */
L8c618624: r[0]=24u;goto L8c618626; /* mov #0x18,r0 */
L8c618626: r[3]=rd(m,bus,&io_status,0x8c61876cu,4);if(io_status)return io_status;goto L8c618628; /* mov.l 0x8c61876c,r3 */
L8c618628: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61862a; /* fmov.s @r1,fr2 */
L8c61862a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61862c; /* fmul fr3,fr2 */
L8c61862c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61862e; /* fmov.s fr2,@-r8 */
L8c61862e: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618630; /* fmov.s @r2,fr3 */
L8c618630: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618632; /* fmov.s fr3,@-r8 */
L8c618632: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618634; /* fmov.s @(r0,r4),fr3 */
L8c618634: r[0]=20u;goto L8c618636; /* mov #0x14,r0 */
L8c618636: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618638; /* fmov.s fr3,@-r8 */
L8c618638: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61863a; /* fmov.s @(r0,r4),fr3 */
L8c61863a: r[0]=16u;goto L8c61863c; /* mov #0x10,r0 */
L8c61863c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61863e; /* fmov.s fr3,@-r8 */
L8c61863e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618640; /* fmov.s @(r0,r4),fr3 */
L8c618640: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618642; /* fmov.s fr3,@-r8 */
L8c618642: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c618644; /* mov.l r3,@-r8 */
L8c618644: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618646; /* pref @r8 */
L8c618646: r[3]=rd(m,bus,&io_status,0x8c618754u,4);if(io_status)return io_status;goto L8c618648; /* mov.l 0x8c618754,r3 */
L8c618648: r[0]=4u;goto L8c61864a; /* mov #0x4,r0 */
L8c61864a: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61864c; /* fmov.s @r5,fr3 */
L8c61864c: r[8]+=64u;goto L8c61864e; /* add #0x40,r8 */
L8c61864e: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618650; /* fmov.s @r3,fr2 */
L8c618650: r[3]+=8u;goto L8c618652; /* add #0x8,r3 */
L8c618652: r[1]=rd(m,bus,&io_status,0x8c618758u,4);if(io_status)return io_status;goto L8c618654; /* mov.l 0x8c618758,r1 */
L8c618654: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618656; /* fmul fr3,fr2 */
L8c618656: r[2]=rd(m,bus,&io_status,0x8c61875cu,4);if(io_status)return io_status;goto L8c618658; /* mov.l 0x8c61875c,r2 */
L8c618658: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61865a; /* fmov.s @r1,fr3 */
L8c61865a: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61865c; /* fmov.s @r2,fr1 */
L8c61865c: r[2]=rd(m,bus,&io_status,0x8c618760u,4);if(io_status)return io_status;goto L8c61865e; /* mov.l 0x8c618760,r2 */
L8c61865e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618660; /* fsub fr2,fr3 */
L8c618660: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c618662; /* fmov.s @(r0,r5),fr2 */
L8c618662: r[0]=8u;goto L8c618664; /* mov #0x8,r0 */
L8c618664: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618666; /* fmul fr2,fr1 */
L8c618666: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c618668; /* fmov.s @(r0,r5),fr2 */
L8c618668: r[0]=28u;goto L8c61866a; /* mov #0x1c,r0 */
L8c61866a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61866c; /* fsub fr1,fr3 */
L8c61866c: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61866e; /* fmov.s @r3,fr1 */
L8c61866e: r[3]=rd(m,bus,&io_status,0x8c618764u,4);if(io_status)return io_status;goto L8c618670; /* mov.l 0x8c618764,r3 */
L8c618670: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618672; /* fmul fr2,fr1 */
L8c618672: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618674; /* fsub fr1,fr3 */
L8c618674: fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618676; /* fmov fr3,@(r0,r5) */
L8c618676: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618678; /* fmov.s @r2,fr2 */
L8c618678: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61867a; /* fmul fr3,fr2 */
L8c61867a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61867c; /* fmov.s fr2,@-r8 */
L8c61867c: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61867e; /* fmov.s @r3,fr2 */
L8c61867e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618680; /* fmov.s @(r0,r5),fr3 */
L8c618680: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618682; /* fmul fr3,fr2 */
L8c618682: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618684; /* fmov.s fr2,@-r8 */
L8c618684: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618686; /* fmov.s @(r0,r5),fr3 */
L8c618686: r[1]=rd(m,bus,&io_status,0x8c618768u,4);if(io_status)return io_status;goto L8c618688; /* mov.l 0x8c618768,r1 */
L8c618688: r[2]+=4294967284u;goto L8c61868a; /* add #-0xc,r2 */
L8c61868a: r[0]=24u;goto L8c61868c; /* mov #0x18,r0 */
L8c61868c: r[3]=rd(m,bus,&io_status,0x8c61876cu,4);if(io_status)return io_status;goto L8c61868e; /* mov.l 0x8c61876c,r3 */
L8c61868e: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618690; /* fmov.s @r1,fr2 */
L8c618690: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618692; /* fmul fr3,fr2 */
L8c618692: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618694; /* fmov.s fr2,@-r8 */
L8c618694: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618696; /* fmov.s @r2,fr3 */
L8c618696: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618698; /* fmov.s fr3,@-r8 */
L8c618698: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61869a; /* fmov.s @(r0,r5),fr3 */
L8c61869a: r[0]=20u;goto L8c61869c; /* mov #0x14,r0 */
L8c61869c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61869e; /* fmov.s fr3,@-r8 */
L8c61869e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6186a0; /* fmov.s @(r0,r5),fr3 */
L8c6186a0: r[0]=16u;goto L8c6186a2; /* mov #0x10,r0 */
L8c6186a2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6186a4; /* fmov.s fr3,@-r8 */
L8c6186a4: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6186a6; /* fmov.s @(r0,r5),fr3 */
L8c6186a6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6186a8; /* fmov.s fr3,@-r8 */
L8c6186a8: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6186aa; /* mov.l r3,@-r8 */
L8c6186aa: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6186ac; /* pref @r8 */
L8c6186ac: r[3]=rd(m,bus,&io_status,0x8c618754u,4);if(io_status)return io_status;goto L8c6186ae; /* mov.l 0x8c618754,r3 */
L8c6186ae: r[0]=4u;goto L8c6186b0; /* mov #0x4,r0 */
L8c6186b0: fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c6186b2; /* fmov.s @r6,fr3 */
L8c6186b2: r[8]+=64u;goto L8c6186b4; /* add #0x40,r8 */
L8c6186b4: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6186b6; /* fmov.s @r3,fr2 */
L8c6186b6: r[3]+=8u;goto L8c6186b8; /* add #0x8,r3 */
L8c6186b8: r[1]=rd(m,bus,&io_status,0x8c618758u,4);if(io_status)return io_status;goto L8c6186ba; /* mov.l 0x8c618758,r1 */
L8c6186ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6186bc; /* fmul fr3,fr2 */
L8c6186bc: r[2]=rd(m,bus,&io_status,0x8c61875cu,4);if(io_status)return io_status;goto L8c6186be; /* mov.l 0x8c61875c,r2 */
L8c6186be: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6186c0; /* fmov.s @r1,fr3 */
L8c6186c0: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c6186c2; /* fmov.s @r2,fr1 */
L8c6186c2: r[2]=rd(m,bus,&io_status,0x8c618760u,4);if(io_status)return io_status;goto L8c6186c4; /* mov.l 0x8c618760,r2 */
L8c6186c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6186c6; /* fsub fr2,fr3 */
L8c6186c6: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c6186c8; /* fmov.s @(r0,r6),fr2 */
L8c6186c8: r[0]=8u;goto L8c6186ca; /* mov #0x8,r0 */
L8c6186ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6186cc; /* fmul fr2,fr1 */
L8c6186cc: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c6186ce; /* fmov.s @(r0,r6),fr2 */
L8c6186ce: r[0]=28u;goto L8c6186d0; /* mov #0x1c,r0 */
L8c6186d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6186d2; /* fsub fr1,fr3 */
L8c6186d2: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6186d4; /* fmov.s @r3,fr1 */
L8c6186d4: r[3]=rd(m,bus,&io_status,0x8c618764u,4);if(io_status)return io_status;goto L8c6186d6; /* mov.l 0x8c618764,r3 */
L8c6186d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6186d8; /* fmul fr2,fr1 */
L8c6186d8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6186da; /* fsub fr1,fr3 */
L8c6186da: fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6186dc; /* fmov fr3,@(r0,r6) */
L8c6186dc: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c6186de; /* fmov.s @r2,fr2 */
L8c6186de: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6186e0; /* fmul fr3,fr2 */
L8c6186e0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6186e2; /* fmov.s fr2,@-r8 */
L8c6186e2: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6186e4; /* fmov.s @r3,fr2 */
L8c6186e4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6186e6; /* fmov.s @(r0,r6),fr3 */
L8c6186e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6186e8; /* fmul fr3,fr2 */
L8c6186e8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6186ea; /* fmov.s fr2,@-r8 */
L8c6186ea: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6186ec; /* fmov.s @(r0,r6),fr3 */
L8c6186ec: r[1]=rd(m,bus,&io_status,0x8c618768u,4);if(io_status)return io_status;goto L8c6186ee; /* mov.l 0x8c618768,r1 */
L8c6186ee: r[2]+=4294967284u;goto L8c6186f0; /* add #-0xc,r2 */
L8c6186f0: r[0]=24u;goto L8c6186f2; /* mov #0x18,r0 */
L8c6186f2: r[3]=rd(m,bus,&io_status,0x8c618770u,4);if(io_status)return io_status;goto L8c6186f4; /* mov.l 0x8c618770,r3 */
L8c6186f4: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c6186f6; /* fmov.s @r1,fr2 */
L8c6186f6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6186f8; /* fmul fr3,fr2 */
L8c6186f8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6186fa; /* fmov.s fr2,@-r8 */
L8c6186fa: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6186fc; /* fmov.s @r2,fr3 */
L8c6186fc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6186fe; /* fmov.s fr3,@-r8 */
L8c6186fe: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618700; /* fmov.s @(r0,r6),fr3 */
L8c618700: r[0]=20u;goto L8c618702; /* mov #0x14,r0 */
L8c618702: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618704; /* fmov.s fr3,@-r8 */
L8c618704: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618706; /* fmov.s @(r0,r6),fr3 */
L8c618706: r[0]=16u;goto L8c618708; /* mov #0x10,r0 */
L8c618708: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61870a; /* fmov.s fr3,@-r8 */
L8c61870a: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61870c; /* fmov.s @(r0,r6),fr3 */
L8c61870c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61870e; /* fmov.s fr3,@-r8 */
L8c61870e: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c618710; /* mov.l r3,@-r8 */
L8c618710: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618712; /* pref @r8 */
L8c618712: r[0]=rd(m,bus,&io_status,0x8c618774u,4);if(io_status)return io_status;goto L8c618714; /* mov.l 0x8c618774,r0 */
L8c618714: r[8]+=32u;goto L8c618716; /* add #0x20,r8 */
L8c618716: r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c618718; /* mov.l @r0,r3 */
L8c618718: r[3]+=3u;goto L8c61871a; /* add #0x3,r3 */
L8c61871a: wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c618728; /* bra 0x8c618728 */
L8c61871e: r[2]=rd(m,bus,&io_status,0x8c618778u,4);if(io_status)return io_status;goto L8c618720; /* mov.l 0x8c618778,r2 */
L8c618720: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c618722; /* mov.l r3,@r2 */
L8c618722: r[3]=rd(m,bus,&io_status,0x8c61877cu,4);if(io_status)return io_status;goto L8c618724; /* mov.l 0x8c61877c,r3 */
L8c618724: target=r[3];*pr=0x8c618728u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c618728; /* jsr @r3 */
L8c618728: r[14]+=1u;goto L8c61872a; /* add #0x1,r14 */
L8c61872a: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61872c; /* mov.l @r15,r2 */
L8c61872c: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61872e; /* mov.w @(0x2,r2),r0 */
L8c61872e: r[0]=r[0]&65535u;goto L8c618730; /* extu.w r0,r0 */
L8c618730: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c618732; /* cmp/ge r0,r14 */
L8c618732: if((*sr&1u))goto L8c618738;goto L8c618734; /* bt 0x8c618738 */
L8c618734: goto L8c618560; /* bra 0x8c618560 */
L8c618738: goto L8c618cb4; /* bra 0x8c618cb4 */
L8c61873c: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61873e; /* mov.l @r15,r1 */
L8c61873e: r[2]=rd(m,bus,&io_status,0x8c618780u,4);if(io_status)return io_status;goto L8c618740; /* mov.l 0x8c618780,r2 */
L8c618740: r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c618742; /* mov.w @(0x2,r1),r0 */
L8c618742: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c618744; /* mov.l @r2,r1 */
L8c618744: r[0]=r[0]&65535u;goto L8c618746; /* extu.w r0,r0 */
L8c618746: r[0]<<=2;goto L8c618748; /* shll2 r0 */
L8c618748: r[1]+=r[0];goto L8c61874a; /* add r0,r1 */
L8c61874a: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61874c; /* mov.l r1,@r2 */
L8c61874c: r[10]=0u;goto L8c6189f4; /* bra 0x8c6189f4 */
L8c618784: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618786; /* mov.w @r11+,r4 */
L8c618786: r[3]=rd(m,bus,&io_status,0x8c618a10u,4);if(io_status)return io_status;goto L8c618788; /* mov.l 0x8c618a10,r3 */
L8c618788: r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61878a; /* mov.w @r11+,r13 */
L8c61878a: r[4]=r[4]&65535u;goto L8c61878c; /* extu.w r4,r4 */
L8c61878c: r[4]<<=2;goto L8c61878e; /* shll2 r4 */
L8c61878e: r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c618790; /* mov.l @r3,r6 */
L8c618790: r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618792; /* mov.w @r11+,r14 */
L8c618792: r[13]=r[13]&65535u;goto L8c618794; /* extu.w r13,r13 */
L8c618794: r[13]<<=2;goto L8c618796; /* shll2 r13 */
L8c618796: r[12]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618798; /* mov.w @r11+,r12 */
L8c618798: r[14]=r[14]&65535u;goto L8c61879a; /* extu.w r14,r14 */
L8c61879a: r[13]<<=2;goto L8c61879c; /* shll2 r13 */
L8c61879c: r[4]<<=2;goto L8c61879e; /* shll2 r4 */
L8c61879e: r[12]=r[12]&65535u;goto L8c6187a0; /* extu.w r12,r12 */
L8c6187a0: r[14]<<=2;goto L8c6187a2; /* shll2 r14 */
L8c6187a2: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6187a4; /* shll r4 */
L8c6187a4: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c6187a6; /* shll r13 */
L8c6187a6: r[4]+=r[6];goto L8c6187a8; /* add r6,r4 */
L8c6187a8: r[12]<<=2;goto L8c6187aa; /* shll2 r12 */
L8c6187aa: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6187ac; /* mov.l @(0xc,r4),r5 */
L8c6187ac: r[13]+=r[6];goto L8c6187ae; /* add r6,r13 */
L8c6187ae: r[14]<<=2;goto L8c6187b0; /* shll2 r14 */
L8c6187b0: r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c6187b2; /* mov.l @(0xc,r13),r2 */
L8c6187b2: r[12]<<=2;goto L8c6187b4; /* shll2 r12 */
L8c6187b4: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c6187b6; /* shll r14 */
L8c6187b6: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c6187b8; /* shll r12 */
L8c6187b8: r[14]+=r[6];goto L8c6187ba; /* add r6,r14 */
L8c6187ba: r[1]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c6187bc; /* mov.l @(0xc,r14),r1 */
L8c6187bc: r[5]&=r[2];goto L8c6187be; /* and r2,r5 */
L8c6187be: r[12]+=r[6];goto L8c6187c0; /* add r6,r12 */
L8c6187c0: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c6187c2; /* mov.l @(0xc,r12),r2 */
L8c6187c2: r[5]&=r[1];goto L8c6187c4; /* and r1,r5 */
L8c6187c4: r[1]=63u;goto L8c6187c6; /* mov #0x3f,r1 */
L8c6187c6: r[5]&=r[2];goto L8c6187c8; /* and r2,r5 */
L8c6187c8: *sr=(*sr&~1u)|!(r[1]&r[5]);goto L8c6187ca; /* tst r1,r5 */
L8c6187ca: if((*sr&1u))goto L8c6187d0;goto L8c6187cc; /* bt 0x8c6187d0 */
L8c6187cc: goto L8c6189f2; /* bra 0x8c6189f2 */
L8c6187d0: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6187d2; /* mov.l @(0xc,r4),r5 */
L8c6187d2: r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c6187d4; /* mov.l @(0xc,r13),r3 */
L8c6187d4: r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c6187d6; /* mov.l @(0xc,r14),r2 */
L8c6187d6: r[5]|=r[3];goto L8c6187d8; /* or r3,r5 */
L8c6187d8: r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c6187da; /* mov.l @(0xc,r12),r3 */
L8c6187da: r[5]|=r[2];goto L8c6187dc; /* or r2,r5 */
L8c6187dc: r[2]=1u;goto L8c6187de; /* mov #0x1,r2 */
L8c6187de: r[5]|=r[3];goto L8c6187e0; /* or r3,r5 */
L8c6187e0: *sr=(*sr&~1u)|!(r[2]&r[5]);goto L8c6187e2; /* tst r2,r5 */
L8c6187e2: if((*sr&1u))goto L8c6187e8;goto L8c6187e4; /* bt 0x8c6187e8 */
L8c6187e4: goto L8c6189bc; /* bra 0x8c6189bc */
L8c6187e8: r[0]=16u;goto L8c6187ea; /* mov #0x10,r0 */
L8c6187ea: fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c6187ec; /* fmov.s @(r0,r13),fr4 */
L8c6187ec: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6187ee; /* fmov.s @(r0,r4),fr3 */
L8c6187ee: r[0]=20u;goto L8c6187f0; /* mov #0x14,r0 */
L8c6187f0: fcopy(f,2u,4u);goto L8c6187f2; /* fmov fr4,fr2 */
L8c6187f2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6187f4; /* fsub fr3,fr2 */
L8c6187f4: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6187f6; /* fmov.s @(r0,r14),fr3 */
L8c6187f6: fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c6187f8; /* fmov.s @(r0,r13),fr5 */
L8c6187f8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6187fa; /* fsub fr5,fr3 */
L8c6187fa: fcopy(f,1u,5u);goto L8c6187fc; /* fmov fr5,fr1 */
L8c6187fc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6187fe; /* fmul fr3,fr2 */
L8c6187fe: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618800; /* fmov.s @(r0,r4),fr3 */
L8c618800: r[0]=16u;goto L8c618802; /* mov #0x10,r0 */
L8c618802: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c618804; /* fsub fr3,fr1 */
L8c618804: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618806; /* fmov.s @(r0,r14),fr3 */
L8c618806: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618808; /* fsub fr4,fr3 */
L8c618808: fcopy(f,4u,2u);goto L8c61880a; /* fmov fr2,fr4 */
L8c61880a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61880c; /* fmul fr3,fr1 */
L8c61880c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61880e; /* fldi0 fr3 */
L8c61880e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c618810; /* fsub fr1,fr4 */
L8c618810: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c618812; /* fcmp/gt fr3,fr4 */
L8c618812: if(!(*sr&1u))goto L8c618818;goto L8c618814; /* bf 0x8c618818 */
L8c618814: goto L8c6189f2; /* bra 0x8c6189f2 */
L8c618818: r[3]=rd(m,bus,&io_status,0x8c618a14u,4);if(io_status)return io_status;goto L8c61881a; /* mov.l 0x8c618a14,r3 */
L8c61881a: r[0]=4u;goto L8c61881c; /* mov #0x4,r0 */
L8c61881c: fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61881e; /* fmov.s @r4,fr3 */
L8c61881e: r[8]+=32u;goto L8c618820; /* add #0x20,r8 */
L8c618820: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618822; /* fmov.s @r3,fr2 */
L8c618822: r[3]+=8u;goto L8c618824; /* add #0x8,r3 */
L8c618824: r[2]=rd(m,bus,&io_status,0x8c618a18u,4);if(io_status)return io_status;goto L8c618826; /* mov.l 0x8c618a18,r2 */
L8c618826: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618828; /* fmul fr3,fr2 */
L8c618828: r[1]=rd(m,bus,&io_status,0x8c618a1cu,4);if(io_status)return io_status;goto L8c61882a; /* mov.l 0x8c618a1c,r1 */
L8c61882a: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61882c; /* fmov.s @r2,fr3 */
L8c61882c: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61882e; /* fmov.s @r1,fr1 */
L8c61882e: r[2]=rd(m,bus,&io_status,0x8c618a20u,4);if(io_status)return io_status;goto L8c618830; /* mov.l 0x8c618a20,r2 */
L8c618830: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618832; /* fsub fr2,fr3 */
L8c618832: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c618834; /* fmov.s @(r0,r4),fr2 */
L8c618834: r[0]=8u;goto L8c618836; /* mov #0x8,r0 */
L8c618836: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618838; /* fmul fr2,fr1 */
L8c618838: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61883a; /* fmov.s @(r0,r4),fr2 */
L8c61883a: r[0]=28u;goto L8c61883c; /* mov #0x1c,r0 */
L8c61883c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61883e; /* fsub fr1,fr3 */
L8c61883e: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618840; /* fmov.s @r3,fr1 */
L8c618840: r[3]=rd(m,bus,&io_status,0x8c618a24u,4);if(io_status)return io_status;goto L8c618842; /* mov.l 0x8c618a24,r3 */
L8c618842: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618844; /* fmul fr2,fr1 */
L8c618844: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618846; /* fsub fr1,fr3 */
L8c618846: fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618848; /* fmov fr3,@(r0,r4) */
L8c618848: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61884a; /* fmov.s @r2,fr2 */
L8c61884a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61884c; /* fmul fr3,fr2 */
L8c61884c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61884e; /* fmov.s fr2,@-r8 */
L8c61884e: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618850; /* fmov.s @r3,fr2 */
L8c618850: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618852; /* fmov.s @(r0,r4),fr3 */
L8c618852: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618854; /* fmul fr3,fr2 */
L8c618854: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618856; /* fmov.s fr2,@-r8 */
L8c618856: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618858; /* fmov.s @(r0,r4),fr3 */
L8c618858: r[1]=rd(m,bus,&io_status,0x8c618a28u,4);if(io_status)return io_status;goto L8c61885a; /* mov.l 0x8c618a28,r1 */
L8c61885a: r[2]+=4294967284u;goto L8c61885c; /* add #-0xc,r2 */
L8c61885c: r[0]=24u;goto L8c61885e; /* mov #0x18,r0 */
L8c61885e: r[3]=rd(m,bus,&io_status,0x8c618a2cu,4);if(io_status)return io_status;goto L8c618860; /* mov.l 0x8c618a2c,r3 */
L8c618860: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618862; /* fmov.s @r1,fr2 */
L8c618862: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618864; /* fmul fr3,fr2 */
L8c618864: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618866; /* fmov.s fr2,@-r8 */
L8c618866: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618868; /* fmov.s @r2,fr3 */
L8c618868: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61886a; /* fmov.s fr3,@-r8 */
L8c61886a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61886c; /* fmov.s @(r0,r4),fr3 */
L8c61886c: r[0]=20u;goto L8c61886e; /* mov #0x14,r0 */
L8c61886e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618870; /* fmov.s fr3,@-r8 */
L8c618870: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618872; /* fmov.s @(r0,r4),fr3 */
L8c618872: r[0]=16u;goto L8c618874; /* mov #0x10,r0 */
L8c618874: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618876; /* fmov.s fr3,@-r8 */
L8c618876: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618878; /* fmov.s @(r0,r4),fr3 */
L8c618878: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61887a; /* fmov.s fr3,@-r8 */
L8c61887a: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61887c; /* mov.l r3,@-r8 */
L8c61887c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61887e; /* pref @r8 */
L8c61887e: r[3]=rd(m,bus,&io_status,0x8c618a14u,4);if(io_status)return io_status;goto L8c618880; /* mov.l 0x8c618a14,r3 */
L8c618880: r[0]=4u;goto L8c618882; /* mov #0x4,r0 */
L8c618882: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c618884; /* fmov.s @r13,fr3 */
L8c618884: r[8]+=64u;goto L8c618886; /* add #0x40,r8 */
L8c618886: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618888; /* fmov.s @r3,fr2 */
L8c618888: r[3]+=8u;goto L8c61888a; /* add #0x8,r3 */
L8c61888a: r[1]=rd(m,bus,&io_status,0x8c618a18u,4);if(io_status)return io_status;goto L8c61888c; /* mov.l 0x8c618a18,r1 */
L8c61888c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61888e; /* fmul fr3,fr2 */
L8c61888e: r[2]=rd(m,bus,&io_status,0x8c618a1cu,4);if(io_status)return io_status;goto L8c618890; /* mov.l 0x8c618a1c,r2 */
L8c618890: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c618892; /* fmov.s @r1,fr3 */
L8c618892: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c618894; /* fmov.s @r2,fr1 */
L8c618894: r[2]=rd(m,bus,&io_status,0x8c618a20u,4);if(io_status)return io_status;goto L8c618896; /* mov.l 0x8c618a20,r2 */
L8c618896: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618898; /* fsub fr2,fr3 */
L8c618898: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61889a; /* fmov.s @(r0,r13),fr2 */
L8c61889a: r[0]=8u;goto L8c61889c; /* mov #0x8,r0 */
L8c61889c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61889e; /* fmul fr2,fr1 */
L8c61889e: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c6188a0; /* fmov.s @(r0,r13),fr2 */
L8c6188a0: r[0]=28u;goto L8c6188a2; /* mov #0x1c,r0 */
L8c6188a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6188a4; /* fsub fr1,fr3 */
L8c6188a4: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6188a6; /* fmov.s @r3,fr1 */
L8c6188a6: r[3]=rd(m,bus,&io_status,0x8c618a24u,4);if(io_status)return io_status;goto L8c6188a8; /* mov.l 0x8c618a24,r3 */
L8c6188a8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6188aa; /* fmul fr2,fr1 */
L8c6188aa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6188ac; /* fsub fr1,fr3 */
L8c6188ac: fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6188ae; /* fmov fr3,@(r0,r13) */
L8c6188ae: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c6188b0; /* fmov.s @r2,fr2 */
L8c6188b0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6188b2; /* fmul fr3,fr2 */
L8c6188b2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6188b4; /* fmov.s fr2,@-r8 */
L8c6188b4: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6188b6; /* fmov.s @r3,fr2 */
L8c6188b6: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6188b8; /* fmov.s @(r0,r13),fr3 */
L8c6188b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6188ba; /* fmul fr3,fr2 */
L8c6188ba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6188bc; /* fmov.s fr2,@-r8 */
L8c6188bc: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6188be; /* fmov.s @(r0,r13),fr3 */
L8c6188be: r[1]=rd(m,bus,&io_status,0x8c618a28u,4);if(io_status)return io_status;goto L8c6188c0; /* mov.l 0x8c618a28,r1 */
L8c6188c0: r[2]+=4294967284u;goto L8c6188c2; /* add #-0xc,r2 */
L8c6188c2: r[0]=24u;goto L8c6188c4; /* mov #0x18,r0 */
L8c6188c4: r[3]=rd(m,bus,&io_status,0x8c618a2cu,4);if(io_status)return io_status;goto L8c6188c6; /* mov.l 0x8c618a2c,r3 */
L8c6188c6: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c6188c8; /* fmov.s @r1,fr2 */
L8c6188c8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6188ca; /* fmul fr3,fr2 */
L8c6188ca: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6188cc; /* fmov.s fr2,@-r8 */
L8c6188cc: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6188ce; /* fmov.s @r2,fr3 */
L8c6188ce: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6188d0; /* fmov.s fr3,@-r8 */
L8c6188d0: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6188d2; /* fmov.s @(r0,r13),fr3 */
L8c6188d2: r[0]=20u;goto L8c6188d4; /* mov #0x14,r0 */
L8c6188d4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6188d6; /* fmov.s fr3,@-r8 */
L8c6188d6: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6188d8; /* fmov.s @(r0,r13),fr3 */
L8c6188d8: r[0]=16u;goto L8c6188da; /* mov #0x10,r0 */
L8c6188da: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6188dc; /* fmov.s fr3,@-r8 */
L8c6188dc: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6188de; /* fmov.s @(r0,r13),fr3 */
L8c6188de: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6188e0; /* fmov.s fr3,@-r8 */
L8c6188e0: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6188e2; /* mov.l r3,@-r8 */
L8c6188e2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6188e4; /* pref @r8 */
L8c6188e4: r[3]=rd(m,bus,&io_status,0x8c618a14u,4);if(io_status)return io_status;goto L8c6188e6; /* mov.l 0x8c618a14,r3 */
L8c6188e6: r[0]=4u;goto L8c6188e8; /* mov #0x4,r0 */
L8c6188e8: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c6188ea; /* fmov.s @r14,fr3 */
L8c6188ea: r[8]+=64u;goto L8c6188ec; /* add #0x40,r8 */
L8c6188ec: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6188ee; /* fmov.s @r3,fr2 */
L8c6188ee: r[3]+=8u;goto L8c6188f0; /* add #0x8,r3 */
L8c6188f0: r[1]=rd(m,bus,&io_status,0x8c618a18u,4);if(io_status)return io_status;goto L8c6188f2; /* mov.l 0x8c618a18,r1 */
L8c6188f2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6188f4; /* fmul fr3,fr2 */
L8c6188f4: r[2]=rd(m,bus,&io_status,0x8c618a1cu,4);if(io_status)return io_status;goto L8c6188f6; /* mov.l 0x8c618a1c,r2 */
L8c6188f6: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6188f8; /* fmov.s @r1,fr3 */
L8c6188f8: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c6188fa; /* fmov.s @r2,fr1 */
L8c6188fa: r[2]=rd(m,bus,&io_status,0x8c618a20u,4);if(io_status)return io_status;goto L8c6188fc; /* mov.l 0x8c618a20,r2 */
L8c6188fc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6188fe; /* fsub fr2,fr3 */
L8c6188fe: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c618900; /* fmov.s @(r0,r14),fr2 */
L8c618900: r[0]=8u;goto L8c618902; /* mov #0x8,r0 */
L8c618902: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618904; /* fmul fr2,fr1 */
L8c618904: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c618906; /* fmov.s @(r0,r14),fr2 */
L8c618906: r[0]=28u;goto L8c618908; /* mov #0x1c,r0 */
L8c618908: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61890a; /* fsub fr1,fr3 */
L8c61890a: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61890c; /* fmov.s @r3,fr1 */
L8c61890c: r[3]=rd(m,bus,&io_status,0x8c618a24u,4);if(io_status)return io_status;goto L8c61890e; /* mov.l 0x8c618a24,r3 */
L8c61890e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618910; /* fmul fr2,fr1 */
L8c618910: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618912; /* fsub fr1,fr3 */
L8c618912: fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618914; /* fmov fr3,@(r0,r14) */
L8c618914: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618916; /* fmov.s @r2,fr2 */
L8c618916: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618918; /* fmul fr3,fr2 */
L8c618918: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61891a; /* fmov.s fr2,@-r8 */
L8c61891a: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61891c; /* fmov.s @r3,fr2 */
L8c61891c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61891e; /* fmov.s @(r0,r14),fr3 */
L8c61891e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618920; /* fmul fr3,fr2 */
L8c618920: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618922; /* fmov.s fr2,@-r8 */
L8c618922: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618924; /* fmov.s @(r0,r14),fr3 */
L8c618924: r[1]=rd(m,bus,&io_status,0x8c618a28u,4);if(io_status)return io_status;goto L8c618926; /* mov.l 0x8c618a28,r1 */
L8c618926: r[2]+=4294967284u;goto L8c618928; /* add #-0xc,r2 */
L8c618928: r[0]=24u;goto L8c61892a; /* mov #0x18,r0 */
L8c61892a: r[3]=rd(m,bus,&io_status,0x8c618a2cu,4);if(io_status)return io_status;goto L8c61892c; /* mov.l 0x8c618a2c,r3 */
L8c61892c: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61892e; /* fmov.s @r1,fr2 */
L8c61892e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618930; /* fmul fr3,fr2 */
L8c618930: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618932; /* fmov.s fr2,@-r8 */
L8c618932: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618934; /* fmov.s @r2,fr3 */
L8c618934: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618936; /* fmov.s fr3,@-r8 */
L8c618936: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618938; /* fmov.s @(r0,r14),fr3 */
L8c618938: r[0]=20u;goto L8c61893a; /* mov #0x14,r0 */
L8c61893a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61893c; /* fmov.s fr3,@-r8 */
L8c61893c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61893e; /* fmov.s @(r0,r14),fr3 */
L8c61893e: r[0]=16u;goto L8c618940; /* mov #0x10,r0 */
L8c618940: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618942; /* fmov.s fr3,@-r8 */
L8c618942: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618944; /* fmov.s @(r0,r14),fr3 */
L8c618944: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618946; /* fmov.s fr3,@-r8 */
L8c618946: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c618948; /* mov.l r3,@-r8 */
L8c618948: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61894a; /* pref @r8 */
L8c61894a: r[3]=rd(m,bus,&io_status,0x8c618a14u,4);if(io_status)return io_status;goto L8c61894c; /* mov.l 0x8c618a14,r3 */
L8c61894c: r[0]=4u;goto L8c61894e; /* mov #0x4,r0 */
L8c61894e: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c618950; /* fmov.s @r12,fr3 */
L8c618950: r[8]+=64u;goto L8c618952; /* add #0x40,r8 */
L8c618952: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618954; /* fmov.s @r3,fr2 */
L8c618954: r[3]+=8u;goto L8c618956; /* add #0x8,r3 */
L8c618956: r[1]=rd(m,bus,&io_status,0x8c618a18u,4);if(io_status)return io_status;goto L8c618958; /* mov.l 0x8c618a18,r1 */
L8c618958: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61895a; /* fmul fr3,fr2 */
L8c61895a: r[2]=rd(m,bus,&io_status,0x8c618a1cu,4);if(io_status)return io_status;goto L8c61895c; /* mov.l 0x8c618a1c,r2 */
L8c61895c: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61895e; /* fmov.s @r1,fr3 */
L8c61895e: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c618960; /* fmov.s @r2,fr1 */
L8c618960: r[2]=rd(m,bus,&io_status,0x8c618a20u,4);if(io_status)return io_status;goto L8c618962; /* mov.l 0x8c618a20,r2 */
L8c618962: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618964; /* fsub fr2,fr3 */
L8c618964: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c618966; /* fmov.s @(r0,r12),fr2 */
L8c618966: r[0]=8u;goto L8c618968; /* mov #0x8,r0 */
L8c618968: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61896a; /* fmul fr2,fr1 */
L8c61896a: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61896c; /* fmov.s @(r0,r12),fr2 */
L8c61896c: r[0]=28u;goto L8c61896e; /* mov #0x1c,r0 */
L8c61896e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618970; /* fsub fr1,fr3 */
L8c618970: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618972; /* fmov.s @r3,fr1 */
L8c618972: r[3]=rd(m,bus,&io_status,0x8c618a24u,4);if(io_status)return io_status;goto L8c618974; /* mov.l 0x8c618a24,r3 */
L8c618974: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618976; /* fmul fr2,fr1 */
L8c618976: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618978; /* fsub fr1,fr3 */
L8c618978: fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61897a; /* fmov fr3,@(r0,r12) */
L8c61897a: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61897c; /* fmov.s @r2,fr2 */
L8c61897c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61897e; /* fmul fr3,fr2 */
L8c61897e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618980; /* fmov.s fr2,@-r8 */
L8c618980: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618982; /* fmov.s @r3,fr2 */
L8c618982: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c618984; /* fmov.s @(r0,r12),fr3 */
L8c618984: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618986; /* fmul fr3,fr2 */
L8c618986: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618988; /* fmov.s fr2,@-r8 */
L8c618988: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61898a; /* fmov.s @(r0,r12),fr3 */
L8c61898a: r[1]=rd(m,bus,&io_status,0x8c618a28u,4);if(io_status)return io_status;goto L8c61898c; /* mov.l 0x8c618a28,r1 */
L8c61898c: r[2]+=4294967284u;goto L8c61898e; /* add #-0xc,r2 */
L8c61898e: r[0]=24u;goto L8c618990; /* mov #0x18,r0 */
L8c618990: r[3]=rd(m,bus,&io_status,0x8c618a30u,4);if(io_status)return io_status;goto L8c618992; /* mov.l 0x8c618a30,r3 */
L8c618992: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618994; /* fmov.s @r1,fr2 */
L8c618994: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618996; /* fmul fr3,fr2 */
L8c618996: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618998; /* fmov.s fr2,@-r8 */
L8c618998: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61899a; /* fmov.s @r2,fr3 */
L8c61899a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61899c; /* fmov.s fr3,@-r8 */
L8c61899c: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61899e; /* fmov.s @(r0,r12),fr3 */
L8c61899e: r[0]=20u;goto L8c6189a0; /* mov #0x14,r0 */
L8c6189a0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6189a2; /* fmov.s fr3,@-r8 */
L8c6189a2: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c6189a4; /* fmov.s @(r0,r12),fr3 */
L8c6189a4: r[0]=16u;goto L8c6189a6; /* mov #0x10,r0 */
L8c6189a6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6189a8; /* fmov.s fr3,@-r8 */
L8c6189a8: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c6189aa; /* fmov.s @(r0,r12),fr3 */
L8c6189aa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6189ac; /* fmov.s fr3,@-r8 */
L8c6189ac: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6189ae; /* mov.l r3,@-r8 */
L8c6189ae: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6189b0; /* pref @r8 */
L8c6189b0: r[0]=rd(m,bus,&io_status,0x8c618a34u,4);if(io_status)return io_status;goto L8c6189b2; /* mov.l 0x8c618a34,r0 */
L8c6189b2: r[8]+=32u;goto L8c6189b4; /* add #0x20,r8 */
L8c6189b4: r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c6189b6; /* mov.l @r0,r3 */
L8c6189b6: r[3]+=4u;goto L8c6189b8; /* add #0x4,r3 */
L8c6189b8: wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c6189f2; /* bra 0x8c6189f2 */
L8c6189bc: r[0]=rd(m,bus,&io_status,0x8c618a38u,4);if(io_status)return io_status;goto L8c6189be; /* mov.l 0x8c618a38,r0 */
L8c6189be: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c6189c0; /* mov.l r2,@r0 */
L8c6189c0: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6189c2; /* mov.l @(0xc,r4),r5 */
L8c6189c2: r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c6189c4; /* mov.l @(0xc,r13),r3 */
L8c6189c4: r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c6189c6; /* mov.l @(0xc,r14),r2 */
L8c6189c6: r[5]&=r[3];goto L8c6189c8; /* and r3,r5 */
L8c6189c8: r[3]=1u;goto L8c6189ca; /* mov #0x1,r3 */
L8c6189ca: r[5]&=r[2];goto L8c6189cc; /* and r2,r5 */
L8c6189cc: *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c6189ce; /* tst r3,r5 */
L8c6189ce: if(!(*sr&1u))goto L8c6189d8;goto L8c6189d0; /* bf 0x8c6189d8 */
L8c6189d0: r[1]=rd(m,bus,&io_status,0x8c618a3cu,4);if(io_status)return io_status;goto L8c6189d2; /* mov.l 0x8c618a3c,r1 */
L8c6189d2: r[6]=r[14];goto L8c6189d4; /* mov r14,r6 */
L8c6189d4: target=r[1];*pr=0x8c6189d8u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6189d8; /* jsr @r1 */
L8c6189d8: r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c6189da; /* mov.l @(0xc,r13),r4 */
L8c6189da: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c6189dc; /* mov.l @(0xc,r14),r3 */
L8c6189dc: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c6189de; /* mov.l @(0xc,r12),r2 */
L8c6189de: r[4]&=r[3];goto L8c6189e0; /* and r3,r4 */
L8c6189e0: r[3]=1u;goto L8c6189e2; /* mov #0x1,r3 */
L8c6189e2: r[4]&=r[2];goto L8c6189e4; /* and r2,r4 */
L8c6189e4: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c6189e6; /* tst r3,r4 */
L8c6189e6: if(!(*sr&1u))goto L8c6189f2;goto L8c6189e8; /* bf 0x8c6189f2 */
L8c6189e8: r[1]=rd(m,bus,&io_status,0x8c618a3cu,4);if(io_status)return io_status;goto L8c6189ea; /* mov.l 0x8c618a3c,r1 */
L8c6189ea: r[5]=r[14];goto L8c6189ec; /* mov r14,r5 */
L8c6189ec: r[6]=r[12];goto L8c6189ee; /* mov r12,r6 */
L8c6189ee: target=r[1];*pr=0x8c6189f2u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6189f2; /* jsr @r1 */
L8c6189f2: r[10]+=1u;goto L8c6189f4; /* add #0x1,r10 */
L8c6189f4: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6189f6; /* mov.l @r15,r3 */
L8c6189f6: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6189f8; /* mov.w @(0x2,r3),r0 */
L8c6189f8: r[0]=r[0]&65535u;goto L8c6189fa; /* extu.w r0,r0 */
L8c6189fa: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c6189fc; /* cmp/ge r0,r10 */
L8c6189fc: if((*sr&1u))goto L8c618a02;goto L8c6189fe; /* bt 0x8c618a02 */
L8c6189fe: goto L8c618784; /* bra 0x8c618784 */
L8c618a02: goto L8c618cb4; /* bra 0x8c618cb4 */
L8c618a06: r[9]=0u;goto L8c618c4c; /* bra 0x8c618c4c */
L8c618a40: r[3]=rd(m,bus,&io_status,0x8c618cd8u,4);if(io_status)return io_status;goto L8c618a42; /* mov.l 0x8c618cd8,r3 */
L8c618a42: r[6]=0u;goto L8c618a44; /* mov #0x0,r6 */
L8c618a44: r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618a46; /* mov.w @r11+,r13 */
L8c618a46: r[4]=0u;goto L8c618a48; /* mov #0x0,r4 */
L8c618a48: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c618a4a; /* mov.l @r3,r2 */
L8c618a4a: r[5]=r[6];goto L8c618a4c; /* mov r6,r5 */
L8c618a4c: r[13]=r[13]&65535u;goto L8c618a4e; /* extu.w r13,r13 */
L8c618a4e: r[2]+=r[13];goto L8c618a50; /* add r13,r2 */
L8c618a50: *sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c618a52; /* cmp/pl r13 */
L8c618a52: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c618a54; /* mov.l r2,@r3 */
L8c618a54: r[7]=4294967295u;goto L8c618a56; /* mov #-0x1,r7 */
L8c618a56: take=!(*sr&1u);r[4]+=r[11];if(take)goto L8c618a78;goto L8c618a5a; /* bf/s 0x8c618a78 */
L8c618a5a: r[14]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c618a5c; /* mov.w @r4+,r14 */
L8c618a5c: r[5]+=1u;goto L8c618a5e; /* add #0x1,r5 */
L8c618a5e: r[2]=rd(m,bus,&io_status,0x8c618cdcu,4);if(io_status)return io_status;goto L8c618a60; /* mov.l 0x8c618cdc,r2 */
L8c618a60: r[14]=r[14]&65535u;goto L8c618a62; /* extu.w r14,r14 */
L8c618a62: r[14]<<=2;goto L8c618a64; /* shll2 r14 */
L8c618a64: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c618a66; /* mov.l @r2,r3 */
L8c618a66: r[14]<<=2;goto L8c618a68; /* shll2 r14 */
L8c618a68: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c618a6a; /* shll r14 */
L8c618a6a: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[13]);goto L8c618a6c; /* cmp/ge r13,r5 */
L8c618a6c: r[14]+=r[3];goto L8c618a6e; /* add r3,r14 */
L8c618a6e: wr(m,bus,&io_status,r[15]+0x4u,r[14],4);if(io_status)return io_status;goto L8c618a70; /* mov.l r14,@(0x4,r15) */
L8c618a70: r[14]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c618a72; /* mov.l @(0xc,r14),r14 */
L8c618a72: r[7]&=r[14];goto L8c618a74; /* and r14,r7 */
L8c618a74: take=!(*sr&1u);r[6]|=r[14];if(take)goto L8c618a5a;goto L8c618a78; /* bf/s 0x8c618a5a */
L8c618a78: r[2]=63u;goto L8c618a7a; /* mov #0x3f,r2 */
L8c618a7a: *sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c618a7c; /* tst r2,r7 */
L8c618a7c: if(!(*sr&1u))goto L8c618adc;goto L8c618a7e; /* bf 0x8c618adc */
L8c618a7e: r[1]=1u;goto L8c618a80; /* mov #0x1,r1 */
L8c618a80: *sr=(*sr&~1u)|!(r[1]&r[6]);goto L8c618a82; /* tst r1,r6 */
L8c618a82: if((*sr&1u))goto L8c618a88;goto L8c618a84; /* bt 0x8c618a88 */
L8c618a84: goto L8c618bec; /* bra 0x8c618bec */
L8c618a88: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c618a8a; /* mov.w @(0x2,r11),r0 */
L8c618a8a: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c618a8c; /* mov.w @r11,r4 */
L8c618a8c: r[3]=rd(m,bus,&io_status,0x8c618cdcu,4);if(io_status)return io_status;goto L8c618a8e; /* mov.l 0x8c618cdc,r3 */
L8c618a8e: r[5]=r[0]&65535u;goto L8c618a90; /* extu.w r0,r5 */
L8c618a90: r[5]<<=2;goto L8c618a92; /* shll2 r5 */
L8c618a92: r[0]=sx(m,bus,&io_status,r[11]+0x4u,2);if(io_status)return io_status;goto L8c618a94; /* mov.w @(0x4,r11),r0 */
L8c618a94: r[4]=r[4]&65535u;goto L8c618a96; /* extu.w r4,r4 */
L8c618a96: r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c618a98; /* mov.l @r3,r7 */
L8c618a98: r[4]<<=2;goto L8c618a9a; /* shll2 r4 */
L8c618a9a: r[5]<<=2;goto L8c618a9c; /* shll2 r5 */
L8c618a9c: r[6]=r[0]&65535u;goto L8c618a9e; /* extu.w r0,r6 */
L8c618a9e: r[4]<<=2;goto L8c618aa0; /* shll2 r4 */
L8c618aa0: r[0]=16u;goto L8c618aa2; /* mov #0x10,r0 */
L8c618aa2: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c618aa4; /* shll r4 */
L8c618aa4: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c618aa6; /* shll r5 */
L8c618aa6: r[5]+=r[7];goto L8c618aa8; /* add r7,r5 */
L8c618aa8: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c618aaa; /* fmov.s @(r0,r5),fr5 */
L8c618aaa: r[6]<<=2;goto L8c618aac; /* shll2 r6 */
L8c618aac: r[4]+=r[7];goto L8c618aae; /* add r7,r4 */
L8c618aae: r[6]<<=2;goto L8c618ab0; /* shll2 r6 */
L8c618ab0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618ab2; /* fmov.s @(r0,r4),fr3 */
L8c618ab2: fcopy(f,2u,5u);goto L8c618ab4; /* fmov fr5,fr2 */
L8c618ab4: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c618ab6; /* shll r6 */
L8c618ab6: r[0]=20u;goto L8c618ab8; /* mov #0x14,r0 */
L8c618ab8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c618aba; /* fsub fr3,fr2 */
L8c618aba: r[6]+=r[7];goto L8c618abc; /* add r7,r6 */
L8c618abc: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c618abe; /* fmov.s @(r0,r5),fr4 */
L8c618abe: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618ac0; /* fmov.s @(r0,r6),fr3 */
L8c618ac0: fcopy(f,1u,4u);goto L8c618ac2; /* fmov fr4,fr1 */
L8c618ac2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618ac4; /* fsub fr4,fr3 */
L8c618ac4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618ac6; /* fmul fr3,fr2 */
L8c618ac6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618ac8; /* fmov.s @(r0,r4),fr3 */
L8c618ac8: r[0]=16u;goto L8c618aca; /* mov #0x10,r0 */
L8c618aca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c618acc; /* fsub fr3,fr1 */
L8c618acc: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618ace; /* fmov.s @(r0,r6),fr3 */
L8c618ace: fcopy(f,4u,2u);goto L8c618ad0; /* fmov fr2,fr4 */
L8c618ad0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618ad2; /* fsub fr5,fr3 */
L8c618ad2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618ad4; /* fmul fr3,fr1 */
L8c618ad4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c618ad6; /* fldi0 fr3 */
L8c618ad6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c618ad8; /* fsub fr1,fr4 */
L8c618ad8: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c618ada; /* fcmp/gt fr3,fr4 */
L8c618ada: if(!(*sr&1u))goto L8c618ae2;goto L8c618adc; /* bf 0x8c618ae2 */
L8c618adc: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c618ade; /* shll r13 */
L8c618ade: r[11]+=r[13];goto L8c618c4a; /* bra 0x8c618c4a */
L8c618ae2: r[2]=rd(m,bus,&io_status,0x8c618ce0u,4);if(io_status)return io_status;goto L8c618ae4; /* mov.l 0x8c618ce0,r2 */
L8c618ae4: r[6]=r[13];goto L8c618ae6; /* mov r13,r6 */
L8c618ae6: r[6]+=4294967295u;goto L8c618ae8; /* add #-0x1,r6 */
L8c618ae8: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c618aea; /* mov.l @r2,r3 */
L8c618aea: *sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c618aec; /* cmp/pl r6 */
L8c618aec: r[3]+=r[13];goto L8c618aee; /* add r13,r3 */
L8c618aee: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c618af0; /* mov.l r3,@r2 */
L8c618af0: take=!(*sr&1u);r[5]=0u;if(take)goto L8c618b72;goto L8c618af4; /* bf/s 0x8c618b72 */
L8c618af4: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618af6; /* mov.w @r11+,r4 */
L8c618af6: r[0]=4u;goto L8c618af8; /* mov #0x4,r0 */
L8c618af8: r[2]=rd(m,bus,&io_status,0x8c618cdcu,4);if(io_status)return io_status;goto L8c618afa; /* mov.l 0x8c618cdc,r2 */
L8c618afa: r[8]+=32u;goto L8c618afc; /* add #0x20,r8 */
L8c618afc: r[4]=r[4]&65535u;goto L8c618afe; /* extu.w r4,r4 */
L8c618afe: r[1]=rd(m,bus,&io_status,0x8c618ce4u,4);if(io_status)return io_status;goto L8c618b00; /* mov.l 0x8c618ce4,r1 */
L8c618b00: r[4]<<=2;goto L8c618b02; /* shll2 r4 */
L8c618b02: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c618b04; /* mov.l @r2,r3 */
L8c618b04: r[4]<<=2;goto L8c618b06; /* shll2 r4 */
L8c618b06: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618b08; /* fmov.s @r1,fr2 */
L8c618b08: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c618b0a; /* shll r4 */
L8c618b0a: r[2]=rd(m,bus,&io_status,0x8c618cecu,4);if(io_status)return io_status;goto L8c618b0c; /* mov.l 0x8c618cec,r2 */
L8c618b0c: r[4]+=r[3];goto L8c618b0e; /* add r3,r4 */
L8c618b0e: r[3]=rd(m,bus,&io_status,0x8c618ce8u,4);if(io_status)return io_status;goto L8c618b10; /* mov.l 0x8c618ce8,r3 */
L8c618b10: fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c618b12; /* fmov.s @r4,fr3 */
L8c618b12: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c618b14; /* fmov.s @r2,fr1 */
L8c618b14: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618b16; /* fmul fr3,fr2 */
L8c618b16: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618b18; /* fmov.s @r3,fr3 */
L8c618b18: r[3]=rd(m,bus,&io_status,0x8c618cf0u,4);if(io_status)return io_status;goto L8c618b1a; /* mov.l 0x8c618cf0,r3 */
L8c618b1a: r[2]=rd(m,bus,&io_status,0x8c618cf4u,4);if(io_status)return io_status;goto L8c618b1c; /* mov.l 0x8c618cf4,r2 */
L8c618b1c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618b1e; /* fsub fr2,fr3 */
L8c618b1e: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b20; /* fmov.s @(r0,r4),fr2 */
L8c618b20: r[0]=8u;goto L8c618b22; /* mov #0x8,r0 */
L8c618b22: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618b24; /* fmul fr2,fr1 */
L8c618b24: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b26; /* fmov.s @(r0,r4),fr2 */
L8c618b26: r[0]=28u;goto L8c618b28; /* mov #0x1c,r0 */
L8c618b28: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618b2a; /* fsub fr1,fr3 */
L8c618b2a: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618b2c; /* fmov.s @r3,fr1 */
L8c618b2c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618b2e; /* fmul fr2,fr1 */
L8c618b2e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618b30; /* fsub fr1,fr3 */
L8c618b30: fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b32; /* fmov fr3,@(r0,r4) */
L8c618b32: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618b34; /* fmov.s @r2,fr2 */
L8c618b34: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618b36; /* fmul fr3,fr2 */
L8c618b36: r[3]=rd(m,bus,&io_status,0x8c618cf8u,4);if(io_status)return io_status;goto L8c618b38; /* mov.l 0x8c618cf8,r3 */
L8c618b38: r[1]=rd(m,bus,&io_status,0x8c618cfcu,4);if(io_status)return io_status;goto L8c618b3a; /* mov.l 0x8c618cfc,r1 */
L8c618b3a: r[2]+=4294967284u;goto L8c618b3c; /* add #-0xc,r2 */
L8c618b3c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618b3e; /* fmov.s fr2,@-r8 */
L8c618b3e: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618b40; /* fmov.s @r3,fr2 */
L8c618b40: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b42; /* fmov.s @(r0,r4),fr3 */
L8c618b42: r[3]=rd(m,bus,&io_status,0x8c618d00u,4);if(io_status)return io_status;goto L8c618b44; /* mov.l 0x8c618d00,r3 */
L8c618b44: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618b46; /* fmul fr3,fr2 */
L8c618b46: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618b48; /* fmov.s fr2,@-r8 */
L8c618b48: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618b4a; /* fmov.s @r1,fr2 */
L8c618b4a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b4c; /* fmov.s @(r0,r4),fr3 */
L8c618b4c: r[0]=24u;goto L8c618b4e; /* mov #0x18,r0 */
L8c618b4e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618b50; /* fmul fr3,fr2 */
L8c618b50: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618b52; /* fmov.s fr2,@-r8 */
L8c618b52: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618b54; /* fmov.s @r2,fr3 */
L8c618b54: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618b56; /* fmov.s fr3,@-r8 */
L8c618b56: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b58; /* fmov.s @(r0,r4),fr3 */
L8c618b58: r[0]=20u;goto L8c618b5a; /* mov #0x14,r0 */
L8c618b5a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618b5c; /* fmov.s fr3,@-r8 */
L8c618b5c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b5e; /* fmov.s @(r0,r4),fr3 */
L8c618b5e: r[0]=16u;goto L8c618b60; /* mov #0x10,r0 */
L8c618b60: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618b62; /* fmov.s fr3,@-r8 */
L8c618b62: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b64; /* fmov.s @(r0,r4),fr3 */
L8c618b64: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618b66; /* fmov.s fr3,@-r8 */
L8c618b66: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c618b68; /* mov.l r3,@-r8 */
L8c618b68: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618b6a; /* pref @r8 */
L8c618b6a: r[5]+=1u;goto L8c618b6c; /* add #0x1,r5 */
L8c618b6c: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c618b6e; /* cmp/ge r6,r5 */
L8c618b6e: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c618af4;goto L8c618b72; /* bf/s 0x8c618af4 */
L8c618b72: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618b74; /* mov.w @r11+,r4 */
L8c618b74: r[0]=4u;goto L8c618b76; /* mov #0x4,r0 */
L8c618b76: r[2]=rd(m,bus,&io_status,0x8c618cdcu,4);if(io_status)return io_status;goto L8c618b78; /* mov.l 0x8c618cdc,r2 */
L8c618b78: r[8]+=32u;goto L8c618b7a; /* add #0x20,r8 */
L8c618b7a: r[4]=r[4]&65535u;goto L8c618b7c; /* extu.w r4,r4 */
L8c618b7c: r[1]=rd(m,bus,&io_status,0x8c618ce4u,4);if(io_status)return io_status;goto L8c618b7e; /* mov.l 0x8c618ce4,r1 */
L8c618b7e: r[4]<<=2;goto L8c618b80; /* shll2 r4 */
L8c618b80: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c618b82; /* mov.l @r2,r3 */
L8c618b82: r[4]<<=2;goto L8c618b84; /* shll2 r4 */
L8c618b84: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618b86; /* fmov.s @r1,fr2 */
L8c618b86: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c618b88; /* shll r4 */
L8c618b88: r[2]=rd(m,bus,&io_status,0x8c618cecu,4);if(io_status)return io_status;goto L8c618b8a; /* mov.l 0x8c618cec,r2 */
L8c618b8a: r[4]+=r[3];goto L8c618b8c; /* add r3,r4 */
L8c618b8c: r[3]=rd(m,bus,&io_status,0x8c618ce8u,4);if(io_status)return io_status;goto L8c618b8e; /* mov.l 0x8c618ce8,r3 */
L8c618b8e: fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c618b90; /* fmov.s @r4,fr3 */
L8c618b90: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c618b92; /* fmov.s @r2,fr1 */
L8c618b92: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618b94; /* fmul fr3,fr2 */
L8c618b94: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618b96; /* fmov.s @r3,fr3 */
L8c618b96: r[3]=rd(m,bus,&io_status,0x8c618cf0u,4);if(io_status)return io_status;goto L8c618b98; /* mov.l 0x8c618cf0,r3 */
L8c618b98: r[2]=rd(m,bus,&io_status,0x8c618cf4u,4);if(io_status)return io_status;goto L8c618b9a; /* mov.l 0x8c618cf4,r2 */
L8c618b9a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618b9c; /* fsub fr2,fr3 */
L8c618b9c: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b9e; /* fmov.s @(r0,r4),fr2 */
L8c618b9e: r[0]=8u;goto L8c618ba0; /* mov #0x8,r0 */
L8c618ba0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618ba2; /* fmul fr2,fr1 */
L8c618ba2: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c618ba4; /* fmov.s @(r0,r4),fr2 */
L8c618ba4: r[0]=28u;goto L8c618ba6; /* mov #0x1c,r0 */
L8c618ba6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618ba8; /* fsub fr1,fr3 */
L8c618ba8: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618baa; /* fmov.s @r3,fr1 */
L8c618baa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618bac; /* fmul fr2,fr1 */
L8c618bac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618bae; /* fsub fr1,fr3 */
L8c618bae: fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618bb0; /* fmov fr3,@(r0,r4) */
L8c618bb0: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618bb2; /* fmov.s @r2,fr2 */
L8c618bb2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618bb4; /* fmul fr3,fr2 */
L8c618bb4: r[3]=rd(m,bus,&io_status,0x8c618cf8u,4);if(io_status)return io_status;goto L8c618bb6; /* mov.l 0x8c618cf8,r3 */
L8c618bb6: r[1]=rd(m,bus,&io_status,0x8c618cfcu,4);if(io_status)return io_status;goto L8c618bb8; /* mov.l 0x8c618cfc,r1 */
L8c618bb8: r[2]+=4294967284u;goto L8c618bba; /* add #-0xc,r2 */
L8c618bba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618bbc; /* fmov.s fr2,@-r8 */
L8c618bbc: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618bbe; /* fmov.s @r3,fr2 */
L8c618bbe: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618bc0; /* fmov.s @(r0,r4),fr3 */
L8c618bc0: r[3]=rd(m,bus,&io_status,0x8c618d04u,4);if(io_status)return io_status;goto L8c618bc2; /* mov.l 0x8c618d04,r3 */
L8c618bc2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618bc4; /* fmul fr3,fr2 */
L8c618bc4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618bc6; /* fmov.s fr2,@-r8 */
L8c618bc6: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618bc8; /* fmov.s @r1,fr2 */
L8c618bc8: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618bca; /* fmov.s @(r0,r4),fr3 */
L8c618bca: r[0]=24u;goto L8c618bcc; /* mov #0x18,r0 */
L8c618bcc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618bce; /* fmul fr3,fr2 */
L8c618bce: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618bd0; /* fmov.s fr2,@-r8 */
L8c618bd0: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618bd2; /* fmov.s @r2,fr3 */
L8c618bd2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618bd4; /* fmov.s fr3,@-r8 */
L8c618bd4: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618bd6; /* fmov.s @(r0,r4),fr3 */
L8c618bd6: r[0]=20u;goto L8c618bd8; /* mov #0x14,r0 */
L8c618bd8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618bda; /* fmov.s fr3,@-r8 */
L8c618bda: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618bdc; /* fmov.s @(r0,r4),fr3 */
L8c618bdc: r[0]=16u;goto L8c618bde; /* mov #0x10,r0 */
L8c618bde: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618be0; /* fmov.s fr3,@-r8 */
L8c618be0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618be2; /* fmov.s @(r0,r4),fr3 */
L8c618be2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618be4; /* fmov.s fr3,@-r8 */
L8c618be4: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c618be6; /* mov.l r3,@-r8 */
L8c618be6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618be8; /* pref @r8 */
L8c618be8: r[8]+=32u;goto L8c618c4a; /* bra 0x8c618c4a */
L8c618bec: r[2]=rd(m,bus,&io_status,0x8c618d08u,4);if(io_status)return io_status;goto L8c618bee; /* mov.l 0x8c618d08,r2 */
L8c618bee: r[3]=0u;goto L8c618bf0; /* mov #0x0,r3 */
L8c618bf0: r[0]=rd(m,bus,&io_status,0x8c618cdcu,4);if(io_status)return io_status;goto L8c618bf2; /* mov.l 0x8c618cdc,r0 */
L8c618bf2: r[10]=r[13];goto L8c618bf4; /* mov r13,r10 */
L8c618bf4: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c618bf6; /* mov.l r3,@r2 */
L8c618bf6: r[10]+=4294967294u;goto L8c618bf8; /* add #-0x2,r10 */
L8c618bf8: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618bfa; /* mov.w @r11+,r4 */
L8c618bfa: r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c618bfc; /* mov.l @r0,r3 */
L8c618bfc: r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618bfe; /* mov.w @r11+,r14 */
L8c618bfe: r[4]=r[4]&65535u;goto L8c618c00; /* extu.w r4,r4 */
L8c618c00: r[4]<<=2;goto L8c618c02; /* shll2 r4 */
L8c618c02: r[14]=r[14]&65535u;goto L8c618c04; /* extu.w r14,r14 */
L8c618c04: r[14]<<=2;goto L8c618c06; /* shll2 r14 */
L8c618c06: r[14]<<=2;goto L8c618c08; /* shll2 r14 */
L8c618c08: r[4]<<=2;goto L8c618c0a; /* shll2 r4 */
L8c618c0a: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c618c0c; /* shll r4 */
L8c618c0c: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c618c0e; /* shll r14 */
L8c618c0e: *sr=(*sr&~1u)|((int32_t)r[10]>0);goto L8c618c10; /* cmp/pl r10 */
L8c618c10: r[4]+=r[3];goto L8c618c12; /* add r3,r4 */
L8c618c12: r[14]+=r[3];goto L8c618c14; /* add r3,r14 */
L8c618c14: take=!(*sr&1u);r[12]=0u;if(take)goto L8c618c4a;goto L8c618c18; /* bf/s 0x8c618c4a */
L8c618c18: r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618c1a; /* mov.w @r11+,r13 */
L8c618c1a: r[2]=rd(m,bus,&io_status,0x8c618cdcu,4);if(io_status)return io_status;goto L8c618c1c; /* mov.l 0x8c618cdc,r2 */
L8c618c1c: r[13]=r[13]&65535u;goto L8c618c1e; /* extu.w r13,r13 */
L8c618c1e: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c618c20; /* mov.l @(0xc,r4),r5 */
L8c618c20: r[13]<<=2;goto L8c618c22; /* shll2 r13 */
L8c618c22: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c618c24; /* mov.l @r2,r3 */
L8c618c24: r[13]<<=2;goto L8c618c26; /* shll2 r13 */
L8c618c26: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c618c28; /* shll r13 */
L8c618c28: r[13]+=r[3];goto L8c618c2a; /* add r3,r13 */
L8c618c2a: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c618c2c; /* mov.l @(0xc,r14),r3 */
L8c618c2c: r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c618c2e; /* mov.l @(0xc,r13),r1 */
L8c618c2e: r[5]&=r[3];goto L8c618c30; /* and r3,r5 */
L8c618c30: r[3]=1u;goto L8c618c32; /* mov #0x1,r3 */
L8c618c32: r[5]&=r[1];goto L8c618c34; /* and r1,r5 */
L8c618c34: *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c618c36; /* tst r3,r5 */
L8c618c36: if(!(*sr&1u))goto L8c618c40;goto L8c618c38; /* bf 0x8c618c40 */
L8c618c38: r[1]=rd(m,bus,&io_status,0x8c618d0cu,4);if(io_status)return io_status;goto L8c618c3a; /* mov.l 0x8c618d0c,r1 */
L8c618c3a: r[6]=r[13];goto L8c618c3c; /* mov r13,r6 */
L8c618c3c: target=r[1];*pr=0x8c618c40u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c618c40; /* jsr @r1 */
L8c618c40: r[12]+=1u;goto L8c618c42; /* add #0x1,r12 */
L8c618c42: r[4]=r[14];goto L8c618c44; /* mov r14,r4 */
L8c618c44: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[10]);goto L8c618c46; /* cmp/ge r10,r12 */
L8c618c46: take=!(*sr&1u);r[14]=r[13];if(take)goto L8c618c18;goto L8c618c4a; /* bf/s 0x8c618c18 */
L8c618c4a: r[9]+=1u;goto L8c618c4c; /* add #0x1,r9 */
L8c618c4c: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c618c4e; /* mov.l @r15,r2 */
L8c618c4e: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c618c50; /* mov.w @(0x2,r2),r0 */
L8c618c50: r[0]=r[0]&65535u;goto L8c618c52; /* extu.w r0,r0 */
L8c618c52: *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c618c54; /* cmp/ge r0,r9 */
L8c618c54: if((*sr&1u))goto L8c618c5a;goto L8c618c56; /* bt 0x8c618c5a */
L8c618c56: goto L8c618a40; /* bra 0x8c618a40 */
L8c618c5a: goto L8c618cb4; /* bra 0x8c618cb4 */
L8c618c5e: r[2]=rd(m,bus,&io_status,0x8c618d10u,4);if(io_status)return io_status;goto L8c618c60; /* mov.l 0x8c618d10,r2 */
L8c618c60: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c618c62; /* mov.l @r15,r1 */
L8c618c62: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c618c64; /* mov.l r1,@r2 */
L8c618c64: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c618c66; /* frchg */
L8c618c66: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c618c68; /* fmov.s fr0,@-r15 */
L8c618c68: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c618c6a; /* fmov.s fr1,@-r15 */
L8c618c6a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c618c6c; /* fmov.s fr2,@-r15 */
L8c618c6c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c618c6e; /* fmov.s fr3,@-r15 */
L8c618c6e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c618c70; /* fmov.s fr4,@-r15 */
L8c618c70: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c618c72; /* fmov.s fr5,@-r15 */
L8c618c72: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c618c74; /* fmov.s fr6,@-r15 */
L8c618c74: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c618c76; /* fmov.s fr7,@-r15 */
L8c618c76: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c618c78; /* fmov.s fr8,@-r15 */
L8c618c78: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c618c7a; /* fmov.s fr9,@-r15 */
L8c618c7a: r[0]=rd(m,bus,&io_status,0x8c618cc8u,4);if(io_status)return io_status;goto L8c618c7c; /* mov.l 0x8c618cc8,r0 */
L8c618c7c: fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618c7e; /* fmov.s @r0+,fr0 */
L8c618c7e: fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618c80; /* fmov.s @r0+,fr1 */
L8c618c80: fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c618c82; /* fmov.s @r0,fr2 */
L8c618c82: r[0]=rd(m,bus,&io_status,0x8c618cccu,4);if(io_status)return io_status;goto L8c618c84; /* mov.l 0x8c618ccc,r0 */
L8c618c84: r[0]+=16u;goto L8c618c86; /* add #0x10,r0 */
L8c618c86: fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618c88; /* fmov.s @r0+,fr4 */
L8c618c88: fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618c8a; /* fmov.s @r0+,fr5 */
L8c618c8a: fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618c8c; /* fmov.s @r0+,fr6 */
L8c618c8c: fload(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;goto L8c618c8e; /* fmov.s @r0,fr7 */
L8c618c8e: r[0]=rd(m,bus,&io_status,0x8c618cd0u,4);if(io_status)return io_status;goto L8c618c90; /* mov.l 0x8c618cd0,r0 */
L8c618c90: fload(m,bus,&io_status,f,8u,r[0]);if(io_status)return io_status;goto L8c618c92; /* fmov.s @r0,fr8 */
L8c618c92: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c618c94; /* frchg */
L8c618c94: r[2]=rd(m,bus,&io_status,0x8c618d14u,4);if(io_status)return io_status;goto L8c618c96; /* mov.l 0x8c618d14,r2 */
L8c618c96: r[3]=rd(m,bus,&io_status,0x8c618d10u,4);if(io_status)return io_status;goto L8c618c98; /* mov.l 0x8c618d10,r3 */
L8c618c98: target=r[2];*pr=0x8c618c9cu;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c618c9c; /* jsr @r2 */
L8c618c9c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c618c9e; /* frchg */
L8c618c9e: fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618ca0; /* fmov.s @r15+,fr9 */
L8c618ca0: fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618ca2; /* fmov.s @r15+,fr8 */
L8c618ca2: fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618ca4; /* fmov.s @r15+,fr7 */
L8c618ca4: fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618ca6; /* fmov.s @r15+,fr6 */
L8c618ca6: fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618ca8; /* fmov.s @r15+,fr5 */
L8c618ca8: fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618caa; /* fmov.s @r15+,fr4 */
L8c618caa: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618cac; /* fmov.s @r15+,fr3 */
L8c618cac: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618cae; /* fmov.s @r15+,fr2 */
L8c618cae: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618cb0; /* fmov.s @r15+,fr1 */
L8c618cb0: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618cb2; /* fmov.s @r15+,fr0 */
L8c618cb2: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c618cb4; /* frchg */
L8c618cb4: r[15]+=8u;goto L8c618cb6; /* add #0x8,r15 */
L8c618cb6: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c618cb8; /* lds.l @r15+,PR */
L8c618cb8: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c618cba; /* mov.l @r15+,r9 */
L8c618cba: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c618cbc; /* mov.l @r15+,r10 */
L8c618cbc: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c618cbe; /* mov.l @r15+,r11 */
L8c618cbe: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c618cc0; /* mov.l @r15+,r12 */
L8c618cc0: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c618cc2; /* mov.l @r15+,r13 */
L8c618cc2: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
