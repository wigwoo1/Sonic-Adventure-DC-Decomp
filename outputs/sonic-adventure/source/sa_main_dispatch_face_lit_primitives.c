/* Face-lit primitives and numerical normalizer with portable queues; names inferred.
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
SA_STAGE_EXPORT int sa_face_lit_primitive_emit_8c61ae3c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 uint32_t entry_pr=*pr;
 (void)fpul;(void)macl;(void)sx;(void)sr;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61ae3c;
L8c61ae3c: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61ae3e; /* mov.l r14,@-r15 */
L8c61ae3e: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61ae40; /* mov.l r13,@-r15 */
L8c61ae40: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61ae42; /* mov.l r12,@-r15 */
L8c61ae42: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61ae44; /* mov.l r11,@-r15 */
L8c61ae44: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61ae46; /* mov.l r10,@-r15 */
L8c61ae46: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61ae48; /* mov.l r9,@-r15 */
L8c61ae48: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61ae4a; /* sts.l PR,@-r15 */
L8c61ae4a: r[15]+=4294967264u;goto L8c61ae4c; /* add #-0x20,r15 */
L8c61ae4c: r[3]=rd(m,bus,&io_status,0x8c61aea4u,4);if(io_status)return io_status;goto L8c61ae4e; /* mov.l 0x8c61aea4,r3 */
L8c61ae4e: r[10]=r[4];goto L8c61ae50; /* mov r4,r10 */
L8c61ae50: r[13]=rd(m,bus,&io_status,0x8c61aea0u,4);if(io_status)return io_status;goto L8c61ae52; /* mov.l 0x8c61aea0,r13 */
L8c61ae52: wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61ae54; /* mov.l r4,@r15 */
L8c61ae54: r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ae56; /* mov.l @r15,r0 */
L8c61ae56: r[10]=rd(m,bus,&io_status,r[10]+0x4u,4);if(io_status)return io_status;goto L8c61ae58; /* mov.l @(0x4,r10),r10 */
L8c61ae58: r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61ae5a; /* mov.w @r0,r0 */
L8c61ae5a: r[0]=r[0]&65535u;goto L8c61ae5c; /* extu.w r0,r0 */
L8c61ae5c: r[0]&=r[3];goto L8c61ae5e; /* and r3,r0 */
L8c61ae5e: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61ae60; /* cmp/eq #0x0,r0 */
L8c61ae60: take=(*sr&1u);r[4]=0u;if(take)goto L8c61ae86;goto L8c61ae64; /* bt/s 0x8c61ae86 */
L8c61ae64: r[1]=sx(m,bus,&io_status,0x8c61ae9cu,2);if(io_status)return io_status;goto L8c61ae66; /* mov.w 0x8c61ae9c,r1 */
L8c61ae66: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61ae68; /* cmp/eq r1,r0 */
L8c61ae68: if(!(*sr&1u))goto L8c61ae6e;goto L8c61ae6a; /* bf 0x8c61ae6e */
L8c61ae6a: goto L8c61b008; /* bra 0x8c61b008 */
L8c61ae6e: r[1]=rd(m,bus,&io_status,0x8c61aea8u,4);if(io_status)return io_status;goto L8c61ae70; /* mov.l 0x8c61aea8,r1 */
L8c61ae70: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61ae72; /* cmp/eq r1,r0 */
L8c61ae72: if(!(*sr&1u))goto L8c61ae78;goto L8c61ae74; /* bf 0x8c61ae78 */
L8c61ae74: goto L8c61b1b6; /* bra 0x8c61b1b6 */
L8c61ae78: r[1]=rd(m,bus,&io_status,0x8c61aea4u,4);if(io_status)return io_status;goto L8c61ae7a; /* mov.l 0x8c61aea4,r1 */
L8c61ae7a: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61ae7c; /* cmp/eq r1,r0 */
L8c61ae7c: if(!(*sr&1u))goto L8c61ae82;goto L8c61ae7e; /* bf 0x8c61ae82 */
L8c61ae7e: goto L8c61b3dc; /* bra 0x8c61b3dc */
L8c61ae82: goto L8c61b432; /* bra 0x8c61b432 */
L8c61ae86: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ae88; /* mov.l @r15,r2 */
L8c61ae88: r[1]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61ae8a; /* mov.l @r13,r1 */
L8c61ae8a: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61ae8c; /* mov.w @(0x2,r2),r0 */
L8c61ae8c: r[0]=r[0]&65535u;goto L8c61ae8e; /* extu.w r0,r0 */
L8c61ae8e: r[2]=r[0];goto L8c61ae90; /* mov r0,r2 */
L8c61ae90: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61ae92; /* shll r0 */
L8c61ae92: r[0]+=r[2];goto L8c61ae94; /* add r2,r0 */
L8c61ae94: r[1]+=r[0];goto L8c61ae96; /* add r0,r1 */
L8c61ae96: wr(m,bus,&io_status,r[13],r[1],4);if(io_status)return io_status;goto L8c61ae98; /* mov.l r1,@r13 */
L8c61ae98: r[11]=r[4];goto L8c61aff6; /* bra 0x8c61aff6 */
L8c61aeac: r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61aeae; /* mov.w @r10+,r12 */
L8c61aeae: r[0]=16u;goto L8c61aeb0; /* mov #0x10,r0 */
L8c61aeb0: r[3]=rd(m,bus,&io_status,0x8c61b1bcu,4);if(io_status)return io_status;goto L8c61aeb2; /* mov.l 0x8c61b1bc,r3 */
L8c61aeb2: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61aeb4; /* mov.w @r10+,r13 */
L8c61aeb4: r[12]=r[12]&65535u;goto L8c61aeb6; /* extu.w r12,r12 */
L8c61aeb6: r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61aeb8; /* mov.l @r3,r4 */
L8c61aeb8: r[12]<<=2;goto L8c61aeba; /* shll2 r12 */
L8c61aeba: r[13]=r[13]&65535u;goto L8c61aebc; /* extu.w r13,r13 */
L8c61aebc: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61aebe; /* mov.w @r10+,r14 */
L8c61aebe: r[13]<<=2;goto L8c61aec0; /* shll2 r13 */
L8c61aec0: r[13]<<=2;goto L8c61aec2; /* shll2 r13 */
L8c61aec2: r[12]<<=2;goto L8c61aec4; /* shll2 r12 */
L8c61aec4: r[14]=r[14]&65535u;goto L8c61aec6; /* extu.w r14,r14 */
L8c61aec6: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61aec8; /* shll r12 */
L8c61aec8: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61aeca; /* shll r13 */
L8c61aeca: r[13]+=r[4];goto L8c61aecc; /* add r4,r13 */
L8c61aecc: fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61aece; /* fmov.s @(r0,r13),fr5 */
L8c61aece: r[12]+=r[4];goto L8c61aed0; /* add r4,r12 */
L8c61aed0: r[14]<<=2;goto L8c61aed2; /* shll2 r14 */
L8c61aed2: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aed4; /* fmov.s @(r0,r12),fr3 */
L8c61aed4: r[14]<<=2;goto L8c61aed6; /* shll2 r14 */
L8c61aed6: fcopy(f,2u,5u);goto L8c61aed8; /* fmov fr5,fr2 */
L8c61aed8: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61aeda; /* shll r14 */
L8c61aeda: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61aedc; /* fsub fr3,fr2 */
L8c61aedc: r[0]=20u;goto L8c61aede; /* mov #0x14,r0 */
L8c61aede: r[14]+=r[4];goto L8c61aee0; /* add r4,r14 */
L8c61aee0: fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61aee2; /* fmov.s @(r0,r13),fr4 */
L8c61aee2: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61aee4; /* fmov.s @(r0,r14),fr3 */
L8c61aee4: fcopy(f,1u,4u);goto L8c61aee6; /* fmov fr4,fr1 */
L8c61aee6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61aee8; /* fsub fr4,fr3 */
L8c61aee8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61aeea; /* fmul fr3,fr2 */
L8c61aeea: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aeec; /* fmov.s @(r0,r12),fr3 */
L8c61aeec: r[0]=16u;goto L8c61aeee; /* mov #0x10,r0 */
L8c61aeee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61aef0; /* fsub fr3,fr1 */
L8c61aef0: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61aef2; /* fmov.s @(r0,r14),fr3 */
L8c61aef2: fcopy(f,4u,2u);goto L8c61aef4; /* fmov fr2,fr4 */
L8c61aef4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61aef6; /* fsub fr5,fr3 */
L8c61aef6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61aef8; /* fmul fr3,fr1 */
L8c61aef8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61aefa; /* fldi0 fr3 */
L8c61aefa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61aefc; /* fsub fr1,fr4 */
L8c61aefc: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61aefe; /* fcmp/gt fr3,fr4 */
L8c61aefe: if((*sr&1u))goto L8c61aff4;goto L8c61af00; /* bt 0x8c61aff4 */
L8c61af00: fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61af02; /* fmov.s @r12,fr2 */
L8c61af02: r[0]=20u;goto L8c61af04; /* mov #0x14,r0 */
L8c61af04: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61af06; /* fmov.s @r13,fr3 */
L8c61af06: r[4]=r[15];goto L8c61af08; /* mov r15,r4 */
L8c61af08: r[3]=rd(m,bus,&io_status,0x8c61b1c0u,4);if(io_status)return io_status;goto L8c61af0a; /* mov.l 0x8c61b1c0,r3 */
L8c61af0a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61af0c; /* fadd fr3,fr2 */
L8c61af0c: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61af0e; /* fmov.s @r14,fr3 */
L8c61af0e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61af10; /* fadd fr3,fr2 */
L8c61af10: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af12; /* fmov fr2,@(r0,r15) */
L8c61af12: r[0]=4u;goto L8c61af14; /* mov #0x4,r0 */
L8c61af14: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61af16; /* fmov.s @(r0,r12),fr2 */
L8c61af16: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61af18; /* fmov.s @(r0,r13),fr3 */
L8c61af18: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61af1a; /* fadd fr3,fr2 */
L8c61af1a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61af1c; /* fmov.s @(r0,r14),fr3 */
L8c61af1c: r[0]=24u;goto L8c61af1e; /* mov #0x18,r0 */
L8c61af1e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61af20; /* fadd fr3,fr2 */
L8c61af20: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af22; /* fmov fr2,@(r0,r15) */
L8c61af22: r[0]=8u;goto L8c61af24; /* mov #0x8,r0 */
L8c61af24: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61af26; /* fmov.s @(r0,r12),fr2 */
L8c61af26: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61af28; /* fmov.s @(r0,r13),fr3 */
L8c61af28: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61af2a; /* fadd fr3,fr2 */
L8c61af2a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61af2c; /* fmov.s @(r0,r14),fr3 */
L8c61af2c: r[0]=28u;goto L8c61af2e; /* mov #0x1c,r0 */
L8c61af2e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61af30; /* fadd fr3,fr2 */
L8c61af30: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af32; /* fmov fr2,@(r0,r15) */
L8c61af32: target=r[3];*pr=0x8c61af36u;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61af36; /* jsr @r3 */
L8c61af36: r[3]=rd(m,bus,&io_status,0x8c61b1c4u,4);if(io_status)return io_status;goto L8c61af38; /* mov.l 0x8c61b1c4,r3 */
L8c61af38: r[0]=20u;goto L8c61af3a; /* mov #0x14,r0 */
L8c61af3a: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af3c; /* fmov.s @(r0,r15),fr3 */
L8c61af3c: r[0]=24u;goto L8c61af3e; /* mov #0x18,r0 */
L8c61af3e: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61af40; /* fmov.s @r3,fr2 */
L8c61af40: r[3]+=8u;goto L8c61af42; /* add #0x8,r3 */
L8c61af42: r[2]=rd(m,bus,&io_status,0x8c61b1c8u,4);if(io_status)return io_status;goto L8c61af44; /* mov.l 0x8c61b1c8,r2 */
L8c61af44: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61af46; /* fmul fr3,fr2 */
L8c61af46: r[1]=rd(m,bus,&io_status,0x8c61b1ccu,4);if(io_status)return io_status;goto L8c61af48; /* mov.l 0x8c61b1cc,r1 */
L8c61af48: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61af4a; /* fmov.s @r2,fr3 */
L8c61af4a: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61af4c; /* fmov.s @r1,fr1 */
L8c61af4c: r[2]=rd(m,bus,&io_status,0x8c61b1d0u,4);if(io_status)return io_status;goto L8c61af4e; /* mov.l 0x8c61b1d0,r2 */
L8c61af4e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61af50; /* fsub fr2,fr3 */
L8c61af50: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af52; /* fmov.s @(r0,r15),fr2 */
L8c61af52: r[0]=28u;goto L8c61af54; /* mov #0x1c,r0 */
L8c61af54: r[1]=rd(m,bus,&io_status,0x8c61b1d8u,4);if(io_status)return io_status;goto L8c61af56; /* mov.l 0x8c61b1d8,r1 */
L8c61af56: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61af58; /* fmul fr2,fr1 */
L8c61af58: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af5a; /* fmov.s @(r0,r15),fr2 */
L8c61af5a: r[0]=4u;goto L8c61af5c; /* mov #0x4,r0 */
L8c61af5c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61af5e; /* fsub fr1,fr3 */
L8c61af5e: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61af60; /* fmov.s @r3,fr1 */
L8c61af60: r[3]=rd(m,bus,&io_status,0x8c61b1d4u,4);if(io_status)return io_status;goto L8c61af62; /* mov.l 0x8c61b1d4,r3 */
L8c61af62: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61af64; /* fmul fr2,fr1 */
L8c61af64: fcopy(f,4u,3u);goto L8c61af66; /* fmov fr3,fr4 */
L8c61af66: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61af68; /* fmov.s @r2,fr3 */
L8c61af68: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61af6a; /* fsub fr1,fr4 */
L8c61af6a: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af6c; /* fmov fr3,@(r0,r15) */
L8c61af6c: r[0]=8u;goto L8c61af6e; /* mov #0x8,r0 */
L8c61af6e: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61af70; /* fmov.s @r3,fr3 */
L8c61af70: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61af72; /* fmul fr4,fr3 */
L8c61af72: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af74; /* fmov fr3,@(r0,r15) */
L8c61af74: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61af76; /* fmov.s @r1,fr3 */
L8c61af76: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61af78; /* fmul fr4,fr3 */
L8c61af78: r[0]=12u;goto L8c61af7a; /* mov #0xc,r0 */
L8c61af7a: r[2]+=12u;goto L8c61af7c; /* add #0xc,r2 */
L8c61af7c: r[3]=rd(m,bus,&io_status,0x8c61b1dcu,4);if(io_status)return io_status;goto L8c61af7e; /* mov.l 0x8c61b1dc,r3 */
L8c61af7e: r[8]+=16u;goto L8c61af80; /* add #0x10,r8 */
L8c61af80: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af82; /* fmov fr3,@(r0,r15) */
L8c61af82: r[0]=16u;goto L8c61af84; /* mov #0x10,r0 */
L8c61af84: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61af86; /* fmov.s @r2,fr3 */
L8c61af86: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61af88; /* fmul fr4,fr3 */
L8c61af88: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af8a; /* fmov fr3,@(r0,r15) */
L8c61af8a: r[0]=24u;goto L8c61af8c; /* mov #0x18,r0 */
L8c61af8c: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61af8e; /* fmov.s @(r0,r12),fr3 */
L8c61af8e: r[0]=20u;goto L8c61af90; /* mov #0x14,r0 */
L8c61af90: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61af92; /* fmov.s fr3,@-r8 */
L8c61af92: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61af94; /* fmov.s @(r0,r12),fr3 */
L8c61af94: r[0]=16u;goto L8c61af96; /* mov #0x10,r0 */
L8c61af96: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61af98; /* fmov.s fr3,@-r8 */
L8c61af98: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61af9a; /* fmov.s @(r0,r12),fr3 */
L8c61af9a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61af9c; /* fmov.s fr3,@-r8 */
L8c61af9c: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61af9e; /* mov.l r3,@-r8 */
L8c61af9e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61afa0; /* pref @r8 */
L8c61afa0: r[0]=24u;goto L8c61afa2; /* mov #0x18,r0 */
L8c61afa2: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61afa4; /* fmov.s @(r0,r13),fr3 */
L8c61afa4: r[8]+=48u;goto L8c61afa6; /* add #0x30,r8 */
L8c61afa6: r[0]=20u;goto L8c61afa8; /* mov #0x14,r0 */
L8c61afa8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afaa; /* fmov.s fr3,@-r8 */
L8c61afaa: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61afac; /* fmov.s @(r0,r13),fr3 */
L8c61afac: r[0]=16u;goto L8c61afae; /* mov #0x10,r0 */
L8c61afae: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afb0; /* fmov.s fr3,@-r8 */
L8c61afb0: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61afb2; /* fmov.s @(r0,r13),fr3 */
L8c61afb2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afb4; /* fmov.s fr3,@-r8 */
L8c61afb4: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61afb6; /* mov.l r3,@-r8 */
L8c61afb6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61afb8; /* pref @r8 */
L8c61afb8: r[0]=16u;goto L8c61afba; /* mov #0x10,r0 */
L8c61afba: r[3]=rd(m,bus,&io_status,0x8c61b1e0u,4);if(io_status)return io_status;goto L8c61afbc; /* mov.l 0x8c61b1e0,r3 */
L8c61afbc: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61afbe; /* fmov.s @(r0,r15),fr3 */
L8c61afbe: r[8]+=64u;goto L8c61afc0; /* add #0x40,r8 */
L8c61afc0: r[0]=12u;goto L8c61afc2; /* mov #0xc,r0 */
L8c61afc2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afc4; /* fmov.s fr3,@-r8 */
L8c61afc4: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61afc6; /* fmov.s @(r0,r15),fr3 */
L8c61afc6: r[0]=8u;goto L8c61afc8; /* mov #0x8,r0 */
L8c61afc8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afca; /* fmov.s fr3,@-r8 */
L8c61afca: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61afcc; /* fmov.s @(r0,r15),fr3 */
L8c61afcc: r[0]=4u;goto L8c61afce; /* mov #0x4,r0 */
L8c61afce: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afd0; /* fmov.s fr3,@-r8 */
L8c61afd0: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61afd2; /* fmov.s @(r0,r15),fr3 */
L8c61afd2: r[0]=24u;goto L8c61afd4; /* mov #0x18,r0 */
L8c61afd4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afd6; /* fmov.s fr3,@-r8 */
L8c61afd6: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61afd8; /* fmov.s @(r0,r14),fr3 */
L8c61afd8: r[0]=20u;goto L8c61afda; /* mov #0x14,r0 */
L8c61afda: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afdc; /* fmov.s fr3,@-r8 */
L8c61afdc: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61afde; /* fmov.s @(r0,r14),fr3 */
L8c61afde: r[0]=16u;goto L8c61afe0; /* mov #0x10,r0 */
L8c61afe0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afe2; /* fmov.s fr3,@-r8 */
L8c61afe2: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61afe4; /* fmov.s @(r0,r14),fr3 */
L8c61afe4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afe6; /* fmov.s fr3,@-r8 */
L8c61afe6: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61afe8; /* mov.l r3,@-r8 */
L8c61afe8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61afea; /* pref @r8 */
L8c61afea: r[0]=rd(m,bus,&io_status,0x8c61b1e4u,4);if(io_status)return io_status;goto L8c61afec; /* mov.l 0x8c61b1e4,r0 */
L8c61afec: r[8]+=32u;goto L8c61afee; /* add #0x20,r8 */
L8c61afee: r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61aff0; /* mov.l @r0,r3 */
L8c61aff0: r[3]+=3u;goto L8c61aff2; /* add #0x3,r3 */
L8c61aff2: wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61aff4; /* mov.l r3,@r0 */
L8c61aff4: r[11]+=1u;goto L8c61aff6; /* add #0x1,r11 */
L8c61aff6: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61aff8; /* mov.l @r15,r2 */
L8c61aff8: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61affa; /* mov.w @(0x2,r2),r0 */
L8c61affa: r[0]=r[0]&65535u;goto L8c61affc; /* extu.w r0,r0 */
L8c61affc: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61affe; /* cmp/ge r0,r11 */
L8c61affe: if((*sr&1u))goto L8c61b004;goto L8c61b000; /* bt 0x8c61b004 */
L8c61b000: goto L8c61aeac; /* bra 0x8c61aeac */
L8c61b004: goto L8c61b432; /* bra 0x8c61b432 */
L8c61b008: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b00a; /* mov.l @r15,r1 */
L8c61b00a: r[2]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61b00c; /* mov.l @r13,r2 */
L8c61b00c: r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61b00e; /* mov.w @(0x2,r1),r0 */
L8c61b00e: r[0]=r[0]&65535u;goto L8c61b010; /* extu.w r0,r0 */
L8c61b010: r[0]<<=2;goto L8c61b012; /* shll2 r0 */
L8c61b012: r[2]+=r[0];goto L8c61b014; /* add r0,r2 */
L8c61b014: wr(m,bus,&io_status,r[13],r[2],4);if(io_status)return io_status;goto L8c61b016; /* mov.l r2,@r13 */
L8c61b016: r[9]=r[4];goto L8c61b1a4; /* bra 0x8c61b1a4 */
L8c61b01a: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b01c; /* mov.w @r10+,r14 */
L8c61b01c: r[0]=16u;goto L8c61b01e; /* mov #0x10,r0 */
L8c61b01e: r[3]=rd(m,bus,&io_status,0x8c61b1bcu,4);if(io_status)return io_status;goto L8c61b020; /* mov.l 0x8c61b1bc,r3 */
L8c61b020: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b022; /* mov.w @r10+,r13 */
L8c61b022: r[14]=r[14]&65535u;goto L8c61b024; /* extu.w r14,r14 */
L8c61b024: r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61b026; /* mov.l @r3,r4 */
L8c61b026: r[14]<<=2;goto L8c61b028; /* shll2 r14 */
L8c61b028: r[13]=r[13]&65535u;goto L8c61b02a; /* extu.w r13,r13 */
L8c61b02a: r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b02c; /* mov.w @r10+,r12 */
L8c61b02c: r[13]<<=2;goto L8c61b02e; /* shll2 r13 */
L8c61b02e: r[13]<<=2;goto L8c61b030; /* shll2 r13 */
L8c61b030: r[11]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b032; /* mov.w @r10+,r11 */
L8c61b032: r[14]<<=2;goto L8c61b034; /* shll2 r14 */
L8c61b034: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61b036; /* shll r14 */
L8c61b036: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61b038; /* shll r13 */
L8c61b038: r[13]+=r[4];goto L8c61b03a; /* add r4,r13 */
L8c61b03a: r[14]+=r[4];goto L8c61b03c; /* add r4,r14 */
L8c61b03c: fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b03e; /* fmov.s @(r0,r13),fr4 */
L8c61b03e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b040; /* fmov.s @(r0,r14),fr3 */
L8c61b040: r[12]=r[12]&65535u;goto L8c61b042; /* extu.w r12,r12 */
L8c61b042: fcopy(f,2u,4u);goto L8c61b044; /* fmov fr4,fr2 */
L8c61b044: r[12]<<=2;goto L8c61b046; /* shll2 r12 */
L8c61b046: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61b048; /* fsub fr3,fr2 */
L8c61b048: r[11]=r[11]&65535u;goto L8c61b04a; /* extu.w r11,r11 */
L8c61b04a: r[12]<<=2;goto L8c61b04c; /* shll2 r12 */
L8c61b04c: r[11]<<=2;goto L8c61b04e; /* shll2 r11 */
L8c61b04e: r[0]=20u;goto L8c61b050; /* mov #0x14,r0 */
L8c61b050: fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b052; /* fmov.s @(r0,r13),fr5 */
L8c61b052: r[11]<<=2;goto L8c61b054; /* shll2 r11 */
L8c61b054: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61b056; /* shll r12 */
L8c61b056: r[12]+=r[4];goto L8c61b058; /* add r4,r12 */
L8c61b058: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61b05a; /* shll r11 */
L8c61b05a: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b05c; /* fmov.s @(r0,r12),fr3 */
L8c61b05c: fcopy(f,1u,5u);goto L8c61b05e; /* fmov fr5,fr1 */
L8c61b05e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b060; /* fsub fr5,fr3 */
L8c61b060: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b062; /* fmul fr3,fr2 */
L8c61b062: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b064; /* fmov.s @(r0,r14),fr3 */
L8c61b064: r[0]=16u;goto L8c61b066; /* mov #0x10,r0 */
L8c61b066: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61b068; /* fsub fr3,fr1 */
L8c61b068: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b06a; /* fmov.s @(r0,r12),fr3 */
L8c61b06a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b06c; /* fsub fr4,fr3 */
L8c61b06c: fcopy(f,4u,2u);goto L8c61b06e; /* fmov fr2,fr4 */
L8c61b06e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b070; /* fmul fr3,fr1 */
L8c61b070: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61b072; /* fldi0 fr3 */
L8c61b072: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b074; /* fsub fr1,fr4 */
L8c61b074: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61b076; /* fcmp/gt fr3,fr4 */
L8c61b076: take=!(*sr&1u);r[11]+=r[4];if(take)goto L8c61b07e;goto L8c61b07a; /* bf/s 0x8c61b07e */
L8c61b07a: goto L8c61b1a2; /* bra 0x8c61b1a2 */
L8c61b07e: fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c61b080; /* fmov.s @r14,fr2 */
L8c61b080: r[0]=20u;goto L8c61b082; /* mov #0x14,r0 */
L8c61b082: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61b084; /* fmov.s @r13,fr3 */
L8c61b084: r[4]=r[15];goto L8c61b086; /* mov r15,r4 */
L8c61b086: r[3]=rd(m,bus,&io_status,0x8c61b1c0u,4);if(io_status)return io_status;goto L8c61b088; /* mov.l 0x8c61b1c0,r3 */
L8c61b088: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b08a; /* fadd fr3,fr2 */
L8c61b08a: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61b08c; /* fmov.s @r12,fr3 */
L8c61b08c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b08e; /* fadd fr3,fr2 */
L8c61b08e: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b090; /* fmov fr2,@(r0,r15) */
L8c61b090: r[0]=4u;goto L8c61b092; /* mov #0x4,r0 */
L8c61b092: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b094; /* fmov.s @(r0,r14),fr2 */
L8c61b094: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b096; /* fmov.s @(r0,r13),fr3 */
L8c61b096: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b098; /* fadd fr3,fr2 */
L8c61b098: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b09a; /* fmov.s @(r0,r12),fr3 */
L8c61b09a: r[0]=24u;goto L8c61b09c; /* mov #0x18,r0 */
L8c61b09c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b09e; /* fadd fr3,fr2 */
L8c61b09e: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b0a0; /* fmov fr2,@(r0,r15) */
L8c61b0a0: r[0]=8u;goto L8c61b0a2; /* mov #0x8,r0 */
L8c61b0a2: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b0a4; /* fmov.s @(r0,r14),fr2 */
L8c61b0a4: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b0a6; /* fmov.s @(r0,r13),fr3 */
L8c61b0a6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b0a8; /* fadd fr3,fr2 */
L8c61b0a8: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b0aa; /* fmov.s @(r0,r12),fr3 */
L8c61b0aa: r[0]=28u;goto L8c61b0ac; /* mov #0x1c,r0 */
L8c61b0ac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b0ae; /* fadd fr3,fr2 */
L8c61b0ae: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b0b0; /* fmov fr2,@(r0,r15) */
L8c61b0b0: target=r[3];*pr=0x8c61b0b4u;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b0b4; /* jsr @r3 */
L8c61b0b4: r[3]=rd(m,bus,&io_status,0x8c61b1c4u,4);if(io_status)return io_status;goto L8c61b0b6; /* mov.l 0x8c61b1c4,r3 */
L8c61b0b6: r[0]=20u;goto L8c61b0b8; /* mov #0x14,r0 */
L8c61b0b8: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b0ba; /* fmov.s @(r0,r15),fr3 */
L8c61b0ba: r[0]=24u;goto L8c61b0bc; /* mov #0x18,r0 */
L8c61b0bc: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61b0be; /* fmov.s @r3,fr2 */
L8c61b0be: r[3]+=8u;goto L8c61b0c0; /* add #0x8,r3 */
L8c61b0c0: r[2]=rd(m,bus,&io_status,0x8c61b1c8u,4);if(io_status)return io_status;goto L8c61b0c2; /* mov.l 0x8c61b1c8,r2 */
L8c61b0c2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b0c4; /* fmul fr3,fr2 */
L8c61b0c4: r[1]=rd(m,bus,&io_status,0x8c61b1ccu,4);if(io_status)return io_status;goto L8c61b0c6; /* mov.l 0x8c61b1cc,r1 */
L8c61b0c6: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b0c8; /* fmov.s @r2,fr3 */
L8c61b0c8: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61b0ca; /* fmov.s @r1,fr1 */
L8c61b0ca: r[2]=rd(m,bus,&io_status,0x8c61b1d0u,4);if(io_status)return io_status;goto L8c61b0cc; /* mov.l 0x8c61b1d0,r2 */
L8c61b0cc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b0ce; /* fsub fr2,fr3 */
L8c61b0ce: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b0d0; /* fmov.s @(r0,r15),fr2 */
L8c61b0d0: r[0]=28u;goto L8c61b0d2; /* mov #0x1c,r0 */
L8c61b0d2: r[1]=rd(m,bus,&io_status,0x8c61b1d8u,4);if(io_status)return io_status;goto L8c61b0d4; /* mov.l 0x8c61b1d8,r1 */
L8c61b0d4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b0d6; /* fmul fr2,fr1 */
L8c61b0d6: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b0d8; /* fmov.s @(r0,r15),fr2 */
L8c61b0d8: r[0]=4u;goto L8c61b0da; /* mov #0x4,r0 */
L8c61b0da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b0dc; /* fsub fr1,fr3 */
L8c61b0dc: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61b0de; /* fmov.s @r3,fr1 */
L8c61b0de: r[3]=rd(m,bus,&io_status,0x8c61b1d4u,4);if(io_status)return io_status;goto L8c61b0e0; /* mov.l 0x8c61b1d4,r3 */
L8c61b0e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b0e2; /* fmul fr2,fr1 */
L8c61b0e2: fcopy(f,4u,3u);goto L8c61b0e4; /* fmov fr3,fr4 */
L8c61b0e4: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b0e6; /* fmov.s @r2,fr3 */
L8c61b0e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b0e8; /* fsub fr1,fr4 */
L8c61b0e8: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b0ea; /* fmov fr3,@(r0,r15) */
L8c61b0ea: r[0]=8u;goto L8c61b0ec; /* mov #0x8,r0 */
L8c61b0ec: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61b0ee; /* fmov.s @r3,fr3 */
L8c61b0ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b0f0; /* fmul fr4,fr3 */
L8c61b0f0: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b0f2; /* fmov fr3,@(r0,r15) */
L8c61b0f2: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61b0f4; /* fmov.s @r1,fr3 */
L8c61b0f4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b0f6; /* fmul fr4,fr3 */
L8c61b0f6: r[0]=12u;goto L8c61b0f8; /* mov #0xc,r0 */
L8c61b0f8: r[2]+=12u;goto L8c61b0fa; /* add #0xc,r2 */
L8c61b0fa: r[3]=rd(m,bus,&io_status,0x8c61b1dcu,4);if(io_status)return io_status;goto L8c61b0fc; /* mov.l 0x8c61b1dc,r3 */
L8c61b0fc: r[8]+=16u;goto L8c61b0fe; /* add #0x10,r8 */
L8c61b0fe: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b100; /* fmov fr3,@(r0,r15) */
L8c61b100: r[0]=16u;goto L8c61b102; /* mov #0x10,r0 */
L8c61b102: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b104; /* fmov.s @r2,fr3 */
L8c61b104: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b106; /* fmul fr4,fr3 */
L8c61b106: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b108; /* fmov fr3,@(r0,r15) */
L8c61b108: r[0]=24u;goto L8c61b10a; /* mov #0x18,r0 */
L8c61b10a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b10c; /* fmov.s @(r0,r14),fr3 */
L8c61b10c: r[0]=20u;goto L8c61b10e; /* mov #0x14,r0 */
L8c61b10e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b110; /* fmov.s fr3,@-r8 */
L8c61b110: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b112; /* fmov.s @(r0,r14),fr3 */
L8c61b112: r[0]=16u;goto L8c61b114; /* mov #0x10,r0 */
L8c61b114: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b116; /* fmov.s fr3,@-r8 */
L8c61b116: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b118; /* fmov.s @(r0,r14),fr3 */
L8c61b118: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b11a; /* fmov.s fr3,@-r8 */
L8c61b11a: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b11c; /* mov.l r3,@-r8 */
L8c61b11c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b11e; /* pref @r8 */
L8c61b11e: r[0]=24u;goto L8c61b120; /* mov #0x18,r0 */
L8c61b120: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b122; /* fmov.s @(r0,r13),fr3 */
L8c61b122: r[8]+=48u;goto L8c61b124; /* add #0x30,r8 */
L8c61b124: r[0]=20u;goto L8c61b126; /* mov #0x14,r0 */
L8c61b126: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b128; /* fmov.s fr3,@-r8 */
L8c61b128: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b12a; /* fmov.s @(r0,r13),fr3 */
L8c61b12a: r[0]=16u;goto L8c61b12c; /* mov #0x10,r0 */
L8c61b12c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b12e; /* fmov.s fr3,@-r8 */
L8c61b12e: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b130; /* fmov.s @(r0,r13),fr3 */
L8c61b130: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b132; /* fmov.s fr3,@-r8 */
L8c61b132: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b134; /* mov.l r3,@-r8 */
L8c61b134: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b136; /* pref @r8 */
L8c61b136: r[0]=16u;goto L8c61b138; /* mov #0x10,r0 */
L8c61b138: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b13a; /* fmov.s @(r0,r15),fr3 */
L8c61b13a: r[8]+=64u;goto L8c61b13c; /* add #0x40,r8 */
L8c61b13c: r[0]=12u;goto L8c61b13e; /* mov #0xc,r0 */
L8c61b13e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b140; /* fmov.s fr3,@-r8 */
L8c61b140: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b142; /* fmov.s @(r0,r15),fr3 */
L8c61b142: r[0]=8u;goto L8c61b144; /* mov #0x8,r0 */
L8c61b144: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b146; /* fmov.s fr3,@-r8 */
L8c61b146: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b148; /* fmov.s @(r0,r15),fr3 */
L8c61b148: r[0]=4u;goto L8c61b14a; /* mov #0x4,r0 */
L8c61b14a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b14c; /* fmov.s fr3,@-r8 */
L8c61b14c: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b14e; /* fmov.s @(r0,r15),fr3 */
L8c61b14e: r[0]=24u;goto L8c61b150; /* mov #0x18,r0 */
L8c61b150: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b152; /* fmov.s fr3,@-r8 */
L8c61b152: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b154; /* fmov.s @(r0,r12),fr3 */
L8c61b154: r[0]=20u;goto L8c61b156; /* mov #0x14,r0 */
L8c61b156: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b158; /* fmov.s fr3,@-r8 */
L8c61b158: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b15a; /* fmov.s @(r0,r12),fr3 */
L8c61b15a: r[0]=16u;goto L8c61b15c; /* mov #0x10,r0 */
L8c61b15c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b15e; /* fmov.s fr3,@-r8 */
L8c61b15e: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b160; /* fmov.s @(r0,r12),fr3 */
L8c61b160: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b162; /* fmov.s fr3,@-r8 */
L8c61b162: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b164; /* mov.l r3,@-r8 */
L8c61b164: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b166; /* pref @r8 */
L8c61b166: r[0]=16u;goto L8c61b168; /* mov #0x10,r0 */
L8c61b168: r[3]=rd(m,bus,&io_status,0x8c61b1e0u,4);if(io_status)return io_status;goto L8c61b16a; /* mov.l 0x8c61b1e0,r3 */
L8c61b16a: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b16c; /* fmov.s @(r0,r15),fr3 */
L8c61b16c: r[8]+=64u;goto L8c61b16e; /* add #0x40,r8 */
L8c61b16e: r[0]=12u;goto L8c61b170; /* mov #0xc,r0 */
L8c61b170: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b172; /* fmov.s fr3,@-r8 */
L8c61b172: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b174; /* fmov.s @(r0,r15),fr3 */
L8c61b174: r[0]=8u;goto L8c61b176; /* mov #0x8,r0 */
L8c61b176: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b178; /* fmov.s fr3,@-r8 */
L8c61b178: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b17a; /* fmov.s @(r0,r15),fr3 */
L8c61b17a: r[0]=4u;goto L8c61b17c; /* mov #0x4,r0 */
L8c61b17c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b17e; /* fmov.s fr3,@-r8 */
L8c61b17e: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b180; /* fmov.s @(r0,r15),fr3 */
L8c61b180: r[0]=24u;goto L8c61b182; /* mov #0x18,r0 */
L8c61b182: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b184; /* fmov.s fr3,@-r8 */
L8c61b184: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b186; /* fmov.s @(r0,r11),fr3 */
L8c61b186: r[0]=20u;goto L8c61b188; /* mov #0x14,r0 */
L8c61b188: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b18a; /* fmov.s fr3,@-r8 */
L8c61b18a: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b18c; /* fmov.s @(r0,r11),fr3 */
L8c61b18c: r[0]=16u;goto L8c61b18e; /* mov #0x10,r0 */
L8c61b18e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b190; /* fmov.s fr3,@-r8 */
L8c61b190: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b192; /* fmov.s @(r0,r11),fr3 */
L8c61b192: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b194; /* fmov.s fr3,@-r8 */
L8c61b194: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b196; /* mov.l r3,@-r8 */
L8c61b196: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b198; /* pref @r8 */
L8c61b198: r[0]=rd(m,bus,&io_status,0x8c61b1e4u,4);if(io_status)return io_status;goto L8c61b19a; /* mov.l 0x8c61b1e4,r0 */
L8c61b19a: r[8]+=32u;goto L8c61b19c; /* add #0x20,r8 */
L8c61b19c: r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61b19e; /* mov.l @r0,r3 */
L8c61b19e: r[3]+=4u;goto L8c61b1a0; /* add #0x4,r3 */
L8c61b1a0: wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61b1a2; /* mov.l r3,@r0 */
L8c61b1a2: r[9]+=1u;goto L8c61b1a4; /* add #0x1,r9 */
L8c61b1a4: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b1a6; /* mov.l @r15,r2 */
L8c61b1a6: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61b1a8; /* mov.w @(0x2,r2),r0 */
L8c61b1a8: r[0]=r[0]&65535u;goto L8c61b1aa; /* extu.w r0,r0 */
L8c61b1aa: *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61b1ac; /* cmp/ge r0,r9 */
L8c61b1ac: if((*sr&1u))goto L8c61b1b2;goto L8c61b1ae; /* bt 0x8c61b1b2 */
L8c61b1ae: goto L8c61b01a; /* bra 0x8c61b01a */
L8c61b1b2: goto L8c61b432; /* bra 0x8c61b432 */
L8c61b1b6: r[11]=2u;goto L8c61b1b8; /* mov #0x2,r11 */
L8c61b1b8: r[12]=r[4];goto L8c61b3ca; /* bra 0x8c61b3ca */
L8c61b1e8: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b1ea; /* mov.w @r10+,r14 */
L8c61b1ea: r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61b1ec; /* mov.l @r13,r3 */
L8c61b1ec: r[14]=r[14]&65535u;goto L8c61b1ee; /* extu.w r14,r14 */
L8c61b1ee: r[3]+=r[14];goto L8c61b1f0; /* add r14,r3 */
L8c61b1f0: wr(m,bus,&io_status,r[13],r[3],4);if(io_status)return io_status;goto L8c61b1f2; /* mov.l r3,@r13 */
L8c61b1f2: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61b1f4; /* mov.w @(0x2,r10),r0 */
L8c61b1f4: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61b1f6; /* mov.w @r10,r4 */
L8c61b1f6: r[3]=rd(m,bus,&io_status,0x8c61b454u,4);if(io_status)return io_status;goto L8c61b1f8; /* mov.l 0x8c61b454,r3 */
L8c61b1f8: r[6]=r[0]&65535u;goto L8c61b1fa; /* extu.w r0,r6 */
L8c61b1fa: r[0]=sx(m,bus,&io_status,r[10]+0x4u,2);if(io_status)return io_status;goto L8c61b1fc; /* mov.w @(0x4,r10),r0 */
L8c61b1fc: r[6]<<=2;goto L8c61b1fe; /* shll2 r6 */
L8c61b1fe: r[4]=r[4]&65535u;goto L8c61b200; /* extu.w r4,r4 */
L8c61b200: r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61b202; /* mov.l @r3,r7 */
L8c61b202: r[6]<<=2;goto L8c61b204; /* shll2 r6 */
L8c61b204: r[4]<<=2;goto L8c61b206; /* shll2 r4 */
L8c61b206: r[5]=r[0]&65535u;goto L8c61b208; /* extu.w r0,r5 */
L8c61b208: r[4]<<=2;goto L8c61b20a; /* shll2 r4 */
L8c61b20a: r[0]=16u;goto L8c61b20c; /* mov #0x10,r0 */
L8c61b20c: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61b20e; /* shll r4 */
L8c61b20e: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61b210; /* shll r6 */
L8c61b210: r[6]+=r[7];goto L8c61b212; /* add r7,r6 */
L8c61b212: fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c61b214; /* fmov.s @(r0,r6),fr5 */
L8c61b214: r[4]+=r[7];goto L8c61b216; /* add r7,r4 */
L8c61b216: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b218; /* fmov.s @(r0,r4),fr3 */
L8c61b218: r[5]<<=2;goto L8c61b21a; /* shll2 r5 */
L8c61b21a: fcopy(f,2u,5u);goto L8c61b21c; /* fmov fr5,fr2 */
L8c61b21c: r[0]=20u;goto L8c61b21e; /* mov #0x14,r0 */
L8c61b21e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61b220; /* fsub fr3,fr2 */
L8c61b220: r[5]<<=2;goto L8c61b222; /* shll2 r5 */
L8c61b222: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c61b224; /* fmov.s @(r0,r6),fr4 */
L8c61b224: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61b226; /* shll r5 */
L8c61b226: r[5]+=r[7];goto L8c61b228; /* add r7,r5 */
L8c61b228: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61b22a; /* fmov.s @(r0,r5),fr3 */
L8c61b22a: fcopy(f,1u,4u);goto L8c61b22c; /* fmov fr4,fr1 */
L8c61b22c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b22e; /* fsub fr4,fr3 */
L8c61b22e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b230; /* fmul fr3,fr2 */
L8c61b230: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b232; /* fmov.s @(r0,r4),fr3 */
L8c61b232: r[0]=16u;goto L8c61b234; /* mov #0x10,r0 */
L8c61b234: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61b236; /* fsub fr3,fr1 */
L8c61b236: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61b238; /* fmov.s @(r0,r5),fr3 */
L8c61b238: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b23a; /* fsub fr5,fr3 */
L8c61b23a: fcopy(f,4u,2u);goto L8c61b23c; /* fmov fr2,fr4 */
L8c61b23c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b23e; /* fmul fr3,fr1 */
L8c61b23e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61b240; /* fldi0 fr3 */
L8c61b240: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b242; /* fsub fr1,fr4 */
L8c61b242: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61b244; /* fcmp/gt fr3,fr4 */
L8c61b244: if(!(*sr&1u))goto L8c61b24c;goto L8c61b246; /* bf 0x8c61b24c */
L8c61b246: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61b248; /* shll r14 */
L8c61b248: r[10]+=r[14];goto L8c61b3c8; /* bra 0x8c61b3c8 */
L8c61b24c: r[0]=20u;goto L8c61b24e; /* mov #0x14,r0 */
L8c61b24e: r[3]=rd(m,bus,&io_status,0x8c61b458u,4);if(io_status)return io_status;goto L8c61b250; /* mov.l 0x8c61b458,r3 */
L8c61b250: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61b252; /* mov.l @r3,r2 */
L8c61b252: r[2]+=r[14];goto L8c61b254; /* add r14,r2 */
L8c61b254: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61b256; /* mov.l r2,@r3 */
L8c61b256: fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c61b258; /* fmov.s @r6,fr3 */
L8c61b258: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c61b25a; /* fmov.s @r4,fr2 */
L8c61b25a: r[3]=rd(m,bus,&io_status,0x8c61b45cu,4);if(io_status)return io_status;goto L8c61b25c; /* mov.l 0x8c61b45c,r3 */
L8c61b25c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b25e; /* fadd fr3,fr2 */
L8c61b25e: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61b260; /* fmov.s @r5,fr3 */
L8c61b260: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b262; /* fadd fr3,fr2 */
L8c61b262: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b264; /* fmov fr2,@(r0,r15) */
L8c61b264: r[0]=4u;goto L8c61b266; /* mov #0x4,r0 */
L8c61b266: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b268; /* fmov.s @(r0,r4),fr2 */
L8c61b268: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61b26a; /* fmov.s @(r0,r6),fr3 */
L8c61b26a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b26c; /* fadd fr3,fr2 */
L8c61b26c: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61b26e; /* fmov.s @(r0,r5),fr3 */
L8c61b26e: r[0]=24u;goto L8c61b270; /* mov #0x18,r0 */
L8c61b270: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b272; /* fadd fr3,fr2 */
L8c61b272: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b274; /* fmov fr2,@(r0,r15) */
L8c61b274: r[0]=8u;goto L8c61b276; /* mov #0x8,r0 */
L8c61b276: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61b278; /* fmov.s @(r0,r6),fr3 */
L8c61b278: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b27a; /* fmov.s @(r0,r4),fr2 */
L8c61b27a: r[4]=r[15];goto L8c61b27c; /* mov r15,r4 */
L8c61b27c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b27e; /* fadd fr3,fr2 */
L8c61b27e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61b280; /* fmov.s @(r0,r5),fr3 */
L8c61b280: r[0]=28u;goto L8c61b282; /* mov #0x1c,r0 */
L8c61b282: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b284; /* fadd fr3,fr2 */
L8c61b284: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b286; /* fmov fr2,@(r0,r15) */
L8c61b286: target=r[3];*pr=0x8c61b28au;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b28a; /* jsr @r3 */
L8c61b28a: r[3]=rd(m,bus,&io_status,0x8c61b460u,4);if(io_status)return io_status;goto L8c61b28c; /* mov.l 0x8c61b460,r3 */
L8c61b28c: r[0]=20u;goto L8c61b28e; /* mov #0x14,r0 */
L8c61b28e: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b290; /* fmov.s @(r0,r15),fr3 */
L8c61b290: r[0]=24u;goto L8c61b292; /* mov #0x18,r0 */
L8c61b292: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61b294; /* fmov.s @r3,fr2 */
L8c61b294: r[3]+=8u;goto L8c61b296; /* add #0x8,r3 */
L8c61b296: r[2]=rd(m,bus,&io_status,0x8c61b464u,4);if(io_status)return io_status;goto L8c61b298; /* mov.l 0x8c61b464,r2 */
L8c61b298: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b29a; /* fmul fr3,fr2 */
L8c61b29a: r[1]=rd(m,bus,&io_status,0x8c61b468u,4);if(io_status)return io_status;goto L8c61b29c; /* mov.l 0x8c61b468,r1 */
L8c61b29c: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b29e; /* fmov.s @r2,fr3 */
L8c61b29e: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61b2a0; /* fmov.s @r1,fr1 */
L8c61b2a0: r[2]=rd(m,bus,&io_status,0x8c61b46cu,4);if(io_status)return io_status;goto L8c61b2a2; /* mov.l 0x8c61b46c,r2 */
L8c61b2a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b2a4; /* fsub fr2,fr3 */
L8c61b2a4: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b2a6; /* fmov.s @(r0,r15),fr2 */
L8c61b2a6: r[0]=28u;goto L8c61b2a8; /* mov #0x1c,r0 */
L8c61b2a8: r[1]=rd(m,bus,&io_status,0x8c61b474u,4);if(io_status)return io_status;goto L8c61b2aa; /* mov.l 0x8c61b474,r1 */
L8c61b2aa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b2ac; /* fmul fr2,fr1 */
L8c61b2ac: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b2ae; /* fmov.s @(r0,r15),fr2 */
L8c61b2ae: r[0]=4u;goto L8c61b2b0; /* mov #0x4,r0 */
L8c61b2b0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b2b2; /* fsub fr1,fr3 */
L8c61b2b2: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61b2b4; /* fmov.s @r3,fr1 */
L8c61b2b4: r[3]=rd(m,bus,&io_status,0x8c61b470u,4);if(io_status)return io_status;goto L8c61b2b6; /* mov.l 0x8c61b470,r3 */
L8c61b2b6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b2b8; /* fmul fr2,fr1 */
L8c61b2b8: fcopy(f,4u,3u);goto L8c61b2ba; /* fmov fr3,fr4 */
L8c61b2ba: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b2bc; /* fmov.s @r2,fr3 */
L8c61b2bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b2be; /* fsub fr1,fr4 */
L8c61b2be: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b2c0; /* fmov fr3,@(r0,r15) */
L8c61b2c0: r[0]=8u;goto L8c61b2c2; /* mov #0x8,r0 */
L8c61b2c2: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61b2c4; /* fmov.s @r3,fr3 */
L8c61b2c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b2c6; /* fmul fr4,fr3 */
L8c61b2c6: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b2c8; /* fmov fr3,@(r0,r15) */
L8c61b2c8: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61b2ca; /* fmov.s @r1,fr3 */
L8c61b2ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b2cc; /* fmul fr4,fr3 */
L8c61b2cc: r[0]=12u;goto L8c61b2ce; /* mov #0xc,r0 */
L8c61b2ce: r[2]+=12u;goto L8c61b2d0; /* add #0xc,r2 */
L8c61b2d0: r[8]+=16u;goto L8c61b2d2; /* add #0x10,r8 */
L8c61b2d2: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b2d4; /* fmov fr3,@(r0,r15) */
L8c61b2d4: r[0]=16u;goto L8c61b2d6; /* mov #0x10,r0 */
L8c61b2d6: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b2d8; /* fmov.s @r2,fr3 */
L8c61b2d8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b2da; /* fmul fr4,fr3 */
L8c61b2da: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b2dc; /* fmov fr3,@(r0,r15) */
L8c61b2dc: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b2de; /* mov.w @r10+,r4 */
L8c61b2de: r[0]=rd(m,bus,&io_status,0x8c61b454u,4);if(io_status)return io_status;goto L8c61b2e0; /* mov.l 0x8c61b454,r0 */
L8c61b2e0: r[4]=r[4]&65535u;goto L8c61b2e2; /* extu.w r4,r4 */
L8c61b2e2: r[4]<<=2;goto L8c61b2e4; /* shll2 r4 */
L8c61b2e4: r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61b2e6; /* mov.l @r0,r3 */
L8c61b2e6: r[4]<<=2;goto L8c61b2e8; /* shll2 r4 */
L8c61b2e8: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61b2ea; /* shll r4 */
L8c61b2ea: r[0]=24u;goto L8c61b2ec; /* mov #0x18,r0 */
L8c61b2ec: r[4]+=r[3];goto L8c61b2ee; /* add r3,r4 */
L8c61b2ee: r[3]=rd(m,bus,&io_status,0x8c61b478u,4);if(io_status)return io_status;goto L8c61b2f0; /* mov.l 0x8c61b478,r3 */
L8c61b2f0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b2f2; /* fmov.s @(r0,r4),fr3 */
L8c61b2f2: r[0]=20u;goto L8c61b2f4; /* mov #0x14,r0 */
L8c61b2f4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b2f6; /* fmov.s fr3,@-r8 */
L8c61b2f6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b2f8; /* fmov.s @(r0,r4),fr3 */
L8c61b2f8: r[0]=16u;goto L8c61b2fa; /* mov #0x10,r0 */
L8c61b2fa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b2fc; /* fmov.s fr3,@-r8 */
L8c61b2fc: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b2fe; /* fmov.s @(r0,r4),fr3 */
L8c61b2fe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b300; /* fmov.s fr3,@-r8 */
L8c61b300: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b302; /* mov.l r3,@-r8 */
L8c61b302: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b304; /* pref @r8 */
L8c61b304: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b306; /* mov.w @r10+,r4 */
L8c61b306: r[0]=24u;goto L8c61b308; /* mov #0x18,r0 */
L8c61b308: r[2]=rd(m,bus,&io_status,0x8c61b454u,4);if(io_status)return io_status;goto L8c61b30a; /* mov.l 0x8c61b454,r2 */
L8c61b30a: r[8]+=48u;goto L8c61b30c; /* add #0x30,r8 */
L8c61b30c: r[4]=r[4]&65535u;goto L8c61b30e; /* extu.w r4,r4 */
L8c61b30e: r[4]<<=2;goto L8c61b310; /* shll2 r4 */
L8c61b310: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61b312; /* mov.l @r2,r3 */
L8c61b312: r[4]<<=2;goto L8c61b314; /* shll2 r4 */
L8c61b314: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61b316; /* shll r4 */
L8c61b316: r[4]+=r[3];goto L8c61b318; /* add r3,r4 */
L8c61b318: r[3]=rd(m,bus,&io_status,0x8c61b478u,4);if(io_status)return io_status;goto L8c61b31a; /* mov.l 0x8c61b478,r3 */
L8c61b31a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b31c; /* fmov.s @(r0,r4),fr3 */
L8c61b31c: r[0]=20u;goto L8c61b31e; /* mov #0x14,r0 */
L8c61b31e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b320; /* fmov.s fr3,@-r8 */
L8c61b320: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b322; /* fmov.s @(r0,r4),fr3 */
L8c61b322: r[0]=16u;goto L8c61b324; /* mov #0x10,r0 */
L8c61b324: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b326; /* fmov.s fr3,@-r8 */
L8c61b326: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b328; /* fmov.s @(r0,r4),fr3 */
L8c61b328: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b32a; /* fmov.s fr3,@-r8 */
L8c61b32a: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b32c; /* mov.l r3,@-r8 */
L8c61b32c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b32e; /* pref @r8 */
L8c61b32e: r[6]=r[14];goto L8c61b330; /* mov r14,r6 */
L8c61b330: r[6]+=4294967295u;goto L8c61b332; /* add #-0x1,r6 */
L8c61b332: r[5]=r[11];goto L8c61b334; /* mov r11,r5 */
L8c61b334: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61b336; /* cmp/ge r6,r5 */
L8c61b336: take=(*sr&1u);r[8]+=32u;if(take)goto L8c61b384;goto L8c61b33a; /* bt/s 0x8c61b384 */
L8c61b33a: r[0]=16u;goto L8c61b33c; /* mov #0x10,r0 */
L8c61b33c: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b33e; /* mov.w @r10+,r4 */
L8c61b33e: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b340; /* fmov.s @(r0,r15),fr3 */
L8c61b340: r[8]+=32u;goto L8c61b342; /* add #0x20,r8 */
L8c61b342: r[0]=12u;goto L8c61b344; /* mov #0xc,r0 */
L8c61b344: r[2]=rd(m,bus,&io_status,0x8c61b454u,4);if(io_status)return io_status;goto L8c61b346; /* mov.l 0x8c61b454,r2 */
L8c61b346: r[4]=r[4]&65535u;goto L8c61b348; /* extu.w r4,r4 */
L8c61b348: r[4]<<=2;goto L8c61b34a; /* shll2 r4 */
L8c61b34a: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61b34c; /* mov.l @r2,r3 */
L8c61b34c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b34e; /* fmov.s fr3,@-r8 */
L8c61b34e: r[4]<<=2;goto L8c61b350; /* shll2 r4 */
L8c61b350: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b352; /* fmov.s @(r0,r15),fr3 */
L8c61b352: r[0]=8u;goto L8c61b354; /* mov #0x8,r0 */
L8c61b354: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61b356; /* shll r4 */
L8c61b356: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b358; /* fmov.s fr3,@-r8 */
L8c61b358: r[4]+=r[3];goto L8c61b35a; /* add r3,r4 */
L8c61b35a: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b35c; /* fmov.s @(r0,r15),fr3 */
L8c61b35c: r[0]=4u;goto L8c61b35e; /* mov #0x4,r0 */
L8c61b35e: r[3]=rd(m,bus,&io_status,0x8c61b478u,4);if(io_status)return io_status;goto L8c61b360; /* mov.l 0x8c61b478,r3 */
L8c61b360: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b362; /* fmov.s fr3,@-r8 */
L8c61b362: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b364; /* fmov.s @(r0,r15),fr3 */
L8c61b364: r[0]=24u;goto L8c61b366; /* mov #0x18,r0 */
L8c61b366: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b368; /* fmov.s fr3,@-r8 */
L8c61b368: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b36a; /* fmov.s @(r0,r4),fr3 */
L8c61b36a: r[0]=20u;goto L8c61b36c; /* mov #0x14,r0 */
L8c61b36c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b36e; /* fmov.s fr3,@-r8 */
L8c61b36e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b370; /* fmov.s @(r0,r4),fr3 */
L8c61b370: r[0]=16u;goto L8c61b372; /* mov #0x10,r0 */
L8c61b372: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b374; /* fmov.s fr3,@-r8 */
L8c61b374: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b376; /* fmov.s @(r0,r4),fr3 */
L8c61b376: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b378; /* fmov.s fr3,@-r8 */
L8c61b378: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b37a; /* mov.l r3,@-r8 */
L8c61b37a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b37c; /* pref @r8 */
L8c61b37c: r[5]+=1u;goto L8c61b37e; /* add #0x1,r5 */
L8c61b37e: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61b380; /* cmp/ge r6,r5 */
L8c61b380: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61b33a;goto L8c61b384; /* bf/s 0x8c61b33a */
L8c61b384: r[0]=16u;goto L8c61b386; /* mov #0x10,r0 */
L8c61b386: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b388; /* mov.w @r10+,r4 */
L8c61b388: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b38a; /* fmov.s @(r0,r15),fr3 */
L8c61b38a: r[8]+=32u;goto L8c61b38c; /* add #0x20,r8 */
L8c61b38c: r[0]=12u;goto L8c61b38e; /* mov #0xc,r0 */
L8c61b38e: r[2]=rd(m,bus,&io_status,0x8c61b454u,4);if(io_status)return io_status;goto L8c61b390; /* mov.l 0x8c61b454,r2 */
L8c61b390: r[4]=r[4]&65535u;goto L8c61b392; /* extu.w r4,r4 */
L8c61b392: r[4]<<=2;goto L8c61b394; /* shll2 r4 */
L8c61b394: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61b396; /* mov.l @r2,r3 */
L8c61b396: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b398; /* fmov.s fr3,@-r8 */
L8c61b398: r[4]<<=2;goto L8c61b39a; /* shll2 r4 */
L8c61b39a: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b39c; /* fmov.s @(r0,r15),fr3 */
L8c61b39c: r[0]=8u;goto L8c61b39e; /* mov #0x8,r0 */
L8c61b39e: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61b3a0; /* shll r4 */
L8c61b3a0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b3a2; /* fmov.s fr3,@-r8 */
L8c61b3a2: r[4]+=r[3];goto L8c61b3a4; /* add r3,r4 */
L8c61b3a4: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b3a6; /* fmov.s @(r0,r15),fr3 */
L8c61b3a6: r[0]=4u;goto L8c61b3a8; /* mov #0x4,r0 */
L8c61b3a8: r[3]=rd(m,bus,&io_status,0x8c61b47cu,4);if(io_status)return io_status;goto L8c61b3aa; /* mov.l 0x8c61b47c,r3 */
L8c61b3aa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b3ac; /* fmov.s fr3,@-r8 */
L8c61b3ac: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b3ae; /* fmov.s @(r0,r15),fr3 */
L8c61b3ae: r[0]=24u;goto L8c61b3b0; /* mov #0x18,r0 */
L8c61b3b0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b3b2; /* fmov.s fr3,@-r8 */
L8c61b3b2: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b3b4; /* fmov.s @(r0,r4),fr3 */
L8c61b3b4: r[0]=20u;goto L8c61b3b6; /* mov #0x14,r0 */
L8c61b3b6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b3b8; /* fmov.s fr3,@-r8 */
L8c61b3b8: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b3ba; /* fmov.s @(r0,r4),fr3 */
L8c61b3ba: r[0]=16u;goto L8c61b3bc; /* mov #0x10,r0 */
L8c61b3bc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b3be; /* fmov.s fr3,@-r8 */
L8c61b3be: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b3c0; /* fmov.s @(r0,r4),fr3 */
L8c61b3c0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b3c2; /* fmov.s fr3,@-r8 */
L8c61b3c2: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b3c4; /* mov.l r3,@-r8 */
L8c61b3c4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b3c6; /* pref @r8 */
L8c61b3c6: r[8]+=32u;goto L8c61b3c8; /* add #0x20,r8 */
L8c61b3c8: r[12]+=1u;goto L8c61b3ca; /* add #0x1,r12 */
L8c61b3ca: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b3cc; /* mov.l @r15,r2 */
L8c61b3cc: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61b3ce; /* mov.w @(0x2,r2),r0 */
L8c61b3ce: r[0]=r[0]&65535u;goto L8c61b3d0; /* extu.w r0,r0 */
L8c61b3d0: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[0]);goto L8c61b3d2; /* cmp/ge r0,r12 */
L8c61b3d2: if((*sr&1u))goto L8c61b3d8;goto L8c61b3d4; /* bt 0x8c61b3d8 */
L8c61b3d4: goto L8c61b1e8; /* bra 0x8c61b1e8 */
L8c61b3d8: goto L8c61b432; /* bra 0x8c61b432 */
L8c61b3dc: r[2]=rd(m,bus,&io_status,0x8c61b480u,4);if(io_status)return io_status;goto L8c61b3de; /* mov.l 0x8c61b480,r2 */
L8c61b3de: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b3e0; /* mov.l @r15,r1 */
L8c61b3e0: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61b3e2; /* mov.l r1,@r2 */
L8c61b3e2: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61b3e4; /* frchg */
L8c61b3e4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61b3e6; /* fmov.s fr0,@-r15 */
L8c61b3e6: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61b3e8; /* fmov.s fr1,@-r15 */
L8c61b3e8: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61b3ea; /* fmov.s fr2,@-r15 */
L8c61b3ea: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61b3ec; /* fmov.s fr3,@-r15 */
L8c61b3ec: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61b3ee; /* fmov.s fr4,@-r15 */
L8c61b3ee: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61b3f0; /* fmov.s fr5,@-r15 */
L8c61b3f0: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61b3f2; /* fmov.s fr6,@-r15 */
L8c61b3f2: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61b3f4; /* fmov.s fr7,@-r15 */
L8c61b3f4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61b3f6; /* fmov.s fr8,@-r15 */
L8c61b3f6: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61b3f8; /* fmov.s fr9,@-r15 */
L8c61b3f8: r[0]=rd(m,bus,&io_status,0x8c61b444u,4);if(io_status)return io_status;goto L8c61b3fa; /* mov.l 0x8c61b444,r0 */
L8c61b3fa: fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b3fc; /* fmov.s @r0+,fr0 */
L8c61b3fc: fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b3fe; /* fmov.s @r0+,fr1 */
L8c61b3fe: fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c61b400; /* fmov.s @r0,fr2 */
L8c61b400: r[0]=rd(m,bus,&io_status,0x8c61b448u,4);if(io_status)return io_status;goto L8c61b402; /* mov.l 0x8c61b448,r0 */
L8c61b402: r[0]+=16u;goto L8c61b404; /* add #0x10,r0 */
L8c61b404: fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b406; /* fmov.s @r0+,fr4 */
L8c61b406: fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b408; /* fmov.s @r0+,fr5 */
L8c61b408: fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b40a; /* fmov.s @r0+,fr6 */
L8c61b40a: fload(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;goto L8c61b40c; /* fmov.s @r0,fr7 */
L8c61b40c: r[0]=rd(m,bus,&io_status,0x8c61b44cu,4);if(io_status)return io_status;goto L8c61b40e; /* mov.l 0x8c61b44c,r0 */
L8c61b40e: fload(m,bus,&io_status,f,8u,r[0]);if(io_status)return io_status;goto L8c61b410; /* fmov.s @r0,fr8 */
L8c61b410: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61b412; /* frchg */
L8c61b412: r[2]=rd(m,bus,&io_status,0x8c61b484u,4);if(io_status)return io_status;goto L8c61b414; /* mov.l 0x8c61b484,r2 */
L8c61b414: r[3]=rd(m,bus,&io_status,0x8c61b480u,4);if(io_status)return io_status;goto L8c61b416; /* mov.l 0x8c61b480,r3 */
L8c61b416: target=r[2];*pr=0x8c61b41au;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b41a; /* jsr @r2 */
L8c61b41a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61b41c; /* frchg */
L8c61b41c: fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b41e; /* fmov.s @r15+,fr9 */
L8c61b41e: fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b420; /* fmov.s @r15+,fr8 */
L8c61b420: fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b422; /* fmov.s @r15+,fr7 */
L8c61b422: fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b424; /* fmov.s @r15+,fr6 */
L8c61b424: fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b426; /* fmov.s @r15+,fr5 */
L8c61b426: fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b428; /* fmov.s @r15+,fr4 */
L8c61b428: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b42a; /* fmov.s @r15+,fr3 */
L8c61b42a: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b42c; /* fmov.s @r15+,fr2 */
L8c61b42c: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b42e; /* fmov.s @r15+,fr1 */
L8c61b42e: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b430; /* fmov.s @r15+,fr0 */
L8c61b430: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61b432; /* frchg */
L8c61b432: r[15]+=32u;goto L8c61b434; /* add #0x20,r15 */
L8c61b434: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61b436; /* lds.l @r15+,PR */
L8c61b436: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61b438; /* mov.l @r15+,r9 */
L8c61b438: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61b43a; /* mov.l @r15+,r10 */
L8c61b43a: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61b43c; /* mov.l @r15+,r11 */
L8c61b43c: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61b43e; /* mov.l @r15+,r12 */
L8c61b43e: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61b440; /* mov.l @r15+,r13 */
L8c61b440: if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_face_lit_primitive_clipped_8c61b488(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 uint32_t entry_pr=*pr;
 (void)fpul;(void)macl;(void)sx;(void)sr;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61b488;
L8c61b488: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61b48a; /* mov.l r14,@-r15 */
L8c61b48a: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61b48c; /* mov.l r13,@-r15 */
L8c61b48c: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61b48e; /* mov.l r12,@-r15 */
L8c61b48e: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61b490; /* mov.l r11,@-r15 */
L8c61b490: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61b492; /* mov.l r10,@-r15 */
L8c61b492: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61b494; /* mov.l r9,@-r15 */
L8c61b494: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61b496; /* sts.l PR,@-r15 */
L8c61b496: r[15]+=4294967256u;goto L8c61b498; /* add #-0x28,r15 */
L8c61b498: r[0]=r[4];goto L8c61b49a; /* mov r4,r0 */
L8c61b49a: r[3]=rd(m,bus,&io_status,0x8c61b4ecu,4);if(io_status)return io_status;goto L8c61b49c; /* mov.l 0x8c61b4ec,r3 */
L8c61b49c: wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61b49e; /* mov.l r4,@r15 */
L8c61b49e: r[10]=r[4];goto L8c61b4a0; /* mov r4,r10 */
L8c61b4a0: r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61b4a2; /* mov.w @r0,r0 */
L8c61b4a2: r[0]=r[0]&65535u;goto L8c61b4a4; /* extu.w r0,r0 */
L8c61b4a4: r[0]&=r[3];goto L8c61b4a6; /* and r3,r0 */
L8c61b4a6: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61b4a8; /* cmp/eq #0x0,r0 */
L8c61b4a8: take=(*sr&1u);r[10]=rd(m,bus,&io_status,r[10]+0x4u,4);if(io_status)return io_status;if(take)goto L8c61b4ce;goto L8c61b4ac; /* bt/s 0x8c61b4ce */
L8c61b4ac: r[1]=sx(m,bus,&io_status,0x8c61b4eau,2);if(io_status)return io_status;goto L8c61b4ae; /* mov.w 0x8c61b4ea,r1 */
L8c61b4ae: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61b4b0; /* cmp/eq r1,r0 */
L8c61b4b0: if(!(*sr&1u))goto L8c61b4b6;goto L8c61b4b2; /* bf 0x8c61b4b6 */
L8c61b4b2: goto L8c61b71a; /* bra 0x8c61b71a */
L8c61b4b6: r[1]=rd(m,bus,&io_status,0x8c61b4f0u,4);if(io_status)return io_status;goto L8c61b4b8; /* mov.l 0x8c61b4f0,r1 */
L8c61b4b8: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61b4ba; /* cmp/eq r1,r0 */
L8c61b4ba: if(!(*sr&1u))goto L8c61b4c0;goto L8c61b4bc; /* bf 0x8c61b4c0 */
L8c61b4bc: goto L8c61ba00; /* bra 0x8c61ba00 */
L8c61b4c0: r[1]=rd(m,bus,&io_status,0x8c61b4ecu,4);if(io_status)return io_status;goto L8c61b4c2; /* mov.l 0x8c61b4ec,r1 */
L8c61b4c2: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61b4c4; /* cmp/eq r1,r0 */
L8c61b4c4: if(!(*sr&1u))goto L8c61b4ca;goto L8c61b4c6; /* bf 0x8c61b4ca */
L8c61b4c6: goto L8c61bd9a; /* bra 0x8c61bd9a */
L8c61b4ca: goto L8c61bdf0; /* bra 0x8c61bdf0 */
L8c61b4ce: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b4d0; /* mov.l @r15,r2 */
L8c61b4d0: r[1]=rd(m,bus,&io_status,0x8c61b4f4u,4);if(io_status)return io_status;goto L8c61b4d2; /* mov.l 0x8c61b4f4,r1 */
L8c61b4d2: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61b4d4; /* mov.w @(0x2,r2),r0 */
L8c61b4d4: r[0]=r[0]&65535u;goto L8c61b4d6; /* extu.w r0,r0 */
L8c61b4d6: r[2]=r[0];goto L8c61b4d8; /* mov r0,r2 */
L8c61b4d8: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61b4da; /* shll r0 */
L8c61b4da: r[0]+=r[2];goto L8c61b4dc; /* add r2,r0 */
L8c61b4dc: r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61b4de; /* mov.l @r1,r2 */
L8c61b4de: r[2]+=r[0];goto L8c61b4e0; /* add r0,r2 */
L8c61b4e0: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61b4e2; /* mov.l r2,@r1 */
L8c61b4e2: r[11]=0u;goto L8c61b708; /* bra 0x8c61b708 */
L8c61b4f8: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b4fa; /* mov.w @r10+,r13 */
L8c61b4fa: r[3]=rd(m,bus,&io_status,0x8c61b734u,4);if(io_status)return io_status;goto L8c61b4fc; /* mov.l 0x8c61b734,r3 */
L8c61b4fc: r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b4fe; /* mov.w @r10+,r12 */
L8c61b4fe: r[13]=r[13]&65535u;goto L8c61b500; /* extu.w r13,r13 */
L8c61b500: r[13]<<=2;goto L8c61b502; /* shll2 r13 */
L8c61b502: r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61b504; /* mov.l @r3,r5 */
L8c61b504: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b506; /* mov.w @r10+,r14 */
L8c61b506: r[12]=r[12]&65535u;goto L8c61b508; /* extu.w r12,r12 */
L8c61b508: r[12]<<=2;goto L8c61b50a; /* shll2 r12 */
L8c61b50a: r[14]=r[14]&65535u;goto L8c61b50c; /* extu.w r14,r14 */
L8c61b50c: r[12]<<=2;goto L8c61b50e; /* shll2 r12 */
L8c61b50e: r[14]<<=2;goto L8c61b510; /* shll2 r14 */
L8c61b510: r[13]<<=2;goto L8c61b512; /* shll2 r13 */
L8c61b512: r[14]<<=2;goto L8c61b514; /* shll2 r14 */
L8c61b514: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61b516; /* shll r13 */
L8c61b516: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61b518; /* shll r12 */
L8c61b518: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61b51a; /* shll r14 */
L8c61b51a: r[13]+=r[5];goto L8c61b51c; /* add r5,r13 */
L8c61b51c: r[12]+=r[5];goto L8c61b51e; /* add r5,r12 */
L8c61b51e: r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61b520; /* mov.l @(0xc,r13),r4 */
L8c61b520: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61b522; /* mov.l @(0xc,r12),r2 */
L8c61b522: r[14]+=r[5];goto L8c61b524; /* add r5,r14 */
L8c61b524: r[1]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61b526; /* mov.l @(0xc,r14),r1 */
L8c61b526: r[4]&=r[2];goto L8c61b528; /* and r2,r4 */
L8c61b528: r[2]=63u;goto L8c61b52a; /* mov #0x3f,r2 */
L8c61b52a: r[4]&=r[1];goto L8c61b52c; /* and r1,r4 */
L8c61b52c: *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61b52e; /* tst r2,r4 */
L8c61b52e: if((*sr&1u))goto L8c61b534;goto L8c61b530; /* bt 0x8c61b534 */
L8c61b530: goto L8c61b706; /* bra 0x8c61b706 */
L8c61b534: r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61b536; /* mov.l @(0xc,r13),r4 */
L8c61b536: r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61b538; /* mov.l @(0xc,r12),r3 */
L8c61b538: r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61b53a; /* mov.l @(0xc,r14),r2 */
L8c61b53a: r[4]|=r[3];goto L8c61b53c; /* or r3,r4 */
L8c61b53c: r[3]=1u;goto L8c61b53e; /* mov #0x1,r3 */
L8c61b53e: r[4]|=r[2];goto L8c61b540; /* or r2,r4 */
L8c61b540: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61b542; /* tst r3,r4 */
L8c61b542: if((*sr&1u))goto L8c61b548;goto L8c61b544; /* bt 0x8c61b548 */
L8c61b544: goto L8c61b66e; /* bra 0x8c61b66e */
L8c61b548: r[0]=16u;goto L8c61b54a; /* mov #0x10,r0 */
L8c61b54a: fload(m,bus,&io_status,f,4u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b54c; /* fmov.s @(r0,r12),fr4 */
L8c61b54c: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b54e; /* fmov.s @(r0,r13),fr3 */
L8c61b54e: r[0]=20u;goto L8c61b550; /* mov #0x14,r0 */
L8c61b550: fcopy(f,2u,4u);goto L8c61b552; /* fmov fr4,fr2 */
L8c61b552: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61b554; /* fsub fr3,fr2 */
L8c61b554: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b556; /* fmov.s @(r0,r14),fr3 */
L8c61b556: fload(m,bus,&io_status,f,5u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b558; /* fmov.s @(r0,r12),fr5 */
L8c61b558: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b55a; /* fsub fr5,fr3 */
L8c61b55a: fcopy(f,1u,5u);goto L8c61b55c; /* fmov fr5,fr1 */
L8c61b55c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b55e; /* fmul fr3,fr2 */
L8c61b55e: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b560; /* fmov.s @(r0,r13),fr3 */
L8c61b560: r[0]=16u;goto L8c61b562; /* mov #0x10,r0 */
L8c61b562: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61b564; /* fsub fr3,fr1 */
L8c61b564: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b566; /* fmov.s @(r0,r14),fr3 */
L8c61b566: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b568; /* fsub fr4,fr3 */
L8c61b568: fcopy(f,4u,2u);goto L8c61b56a; /* fmov fr2,fr4 */
L8c61b56a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b56c; /* fmul fr3,fr1 */
L8c61b56c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61b56e; /* fldi0 fr3 */
L8c61b56e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b570; /* fsub fr1,fr4 */
L8c61b570: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61b572; /* fcmp/gt fr3,fr4 */
L8c61b572: if(!(*sr&1u))goto L8c61b578;goto L8c61b574; /* bf 0x8c61b578 */
L8c61b574: goto L8c61b706; /* bra 0x8c61b706 */
L8c61b578: fload(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c61b57a; /* fmov.s @r13,fr2 */
L8c61b57a: r[0]=28u;goto L8c61b57c; /* mov #0x1c,r0 */
L8c61b57c: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61b57e; /* fmov.s @r12,fr3 */
L8c61b57e: r[4]=r[15];goto L8c61b580; /* mov r15,r4 */
L8c61b580: r[3]=rd(m,bus,&io_status,0x8c61b738u,4);if(io_status)return io_status;goto L8c61b582; /* mov.l 0x8c61b738,r3 */
L8c61b582: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b584; /* fadd fr3,fr2 */
L8c61b584: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61b586; /* fmov.s @r14,fr3 */
L8c61b586: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b588; /* fadd fr3,fr2 */
L8c61b588: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b58a; /* fmov fr2,@(r0,r15) */
L8c61b58a: r[0]=4u;goto L8c61b58c; /* mov #0x4,r0 */
L8c61b58c: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b58e; /* fmov.s @(r0,r13),fr2 */
L8c61b58e: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b590; /* fmov.s @(r0,r12),fr3 */
L8c61b590: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b592; /* fadd fr3,fr2 */
L8c61b592: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b594; /* fmov.s @(r0,r14),fr3 */
L8c61b594: r[0]=32u;goto L8c61b596; /* mov #0x20,r0 */
L8c61b596: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b598; /* fadd fr3,fr2 */
L8c61b598: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b59a; /* fmov fr2,@(r0,r15) */
L8c61b59a: r[0]=8u;goto L8c61b59c; /* mov #0x8,r0 */
L8c61b59c: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b59e; /* fmov.s @(r0,r13),fr2 */
L8c61b59e: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b5a0; /* fmov.s @(r0,r12),fr3 */
L8c61b5a0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b5a2; /* fadd fr3,fr2 */
L8c61b5a2: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b5a4; /* fmov.s @(r0,r14),fr3 */
L8c61b5a4: r[0]=36u;goto L8c61b5a6; /* mov #0x24,r0 */
L8c61b5a6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b5a8; /* fadd fr3,fr2 */
L8c61b5a8: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b5aa; /* fmov fr2,@(r0,r15) */
L8c61b5aa: target=r[3];*pr=0x8c61b5aeu;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b5ae; /* jsr @r3 */
L8c61b5ae: r[3]=rd(m,bus,&io_status,0x8c61b73cu,4);if(io_status)return io_status;goto L8c61b5b0; /* mov.l 0x8c61b73c,r3 */
L8c61b5b0: r[0]=28u;goto L8c61b5b2; /* mov #0x1c,r0 */
L8c61b5b2: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b5b4; /* fmov.s @(r0,r15),fr3 */
L8c61b5b4: r[0]=32u;goto L8c61b5b6; /* mov #0x20,r0 */
L8c61b5b6: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61b5b8; /* fmov.s @r3,fr2 */
L8c61b5b8: r[3]+=8u;goto L8c61b5ba; /* add #0x8,r3 */
L8c61b5ba: r[2]=rd(m,bus,&io_status,0x8c61b740u,4);if(io_status)return io_status;goto L8c61b5bc; /* mov.l 0x8c61b740,r2 */
L8c61b5bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b5be; /* fmul fr3,fr2 */
L8c61b5be: r[1]=rd(m,bus,&io_status,0x8c61b744u,4);if(io_status)return io_status;goto L8c61b5c0; /* mov.l 0x8c61b744,r1 */
L8c61b5c0: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b5c2; /* fmov.s @r2,fr3 */
L8c61b5c2: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61b5c4; /* fmov.s @r1,fr1 */
L8c61b5c4: r[2]=rd(m,bus,&io_status,0x8c61b748u,4);if(io_status)return io_status;goto L8c61b5c6; /* mov.l 0x8c61b748,r2 */
L8c61b5c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b5c8; /* fsub fr2,fr3 */
L8c61b5c8: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b5ca; /* fmov.s @(r0,r15),fr2 */
L8c61b5ca: r[0]=36u;goto L8c61b5cc; /* mov #0x24,r0 */
L8c61b5cc: r[1]=rd(m,bus,&io_status,0x8c61b750u,4);if(io_status)return io_status;goto L8c61b5ce; /* mov.l 0x8c61b750,r1 */
L8c61b5ce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b5d0; /* fmul fr2,fr1 */
L8c61b5d0: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b5d2; /* fmov.s @(r0,r15),fr2 */
L8c61b5d2: r[0]=12u;goto L8c61b5d4; /* mov #0xc,r0 */
L8c61b5d4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b5d6; /* fsub fr1,fr3 */
L8c61b5d6: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61b5d8; /* fmov.s @r3,fr1 */
L8c61b5d8: r[3]=rd(m,bus,&io_status,0x8c61b74cu,4);if(io_status)return io_status;goto L8c61b5da; /* mov.l 0x8c61b74c,r3 */
L8c61b5da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b5dc; /* fmul fr2,fr1 */
L8c61b5dc: fcopy(f,4u,3u);goto L8c61b5de; /* fmov fr3,fr4 */
L8c61b5de: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b5e0; /* fmov.s @r2,fr3 */
L8c61b5e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b5e2; /* fsub fr1,fr4 */
L8c61b5e2: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b5e4; /* fmov fr3,@(r0,r15) */
L8c61b5e4: r[0]=16u;goto L8c61b5e6; /* mov #0x10,r0 */
L8c61b5e6: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61b5e8; /* fmov.s @r3,fr3 */
L8c61b5e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b5ea; /* fmul fr4,fr3 */
L8c61b5ea: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b5ec; /* fmov fr3,@(r0,r15) */
L8c61b5ec: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61b5ee; /* fmov.s @r1,fr3 */
L8c61b5ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b5f0; /* fmul fr4,fr3 */
L8c61b5f0: r[0]=20u;goto L8c61b5f2; /* mov #0x14,r0 */
L8c61b5f2: r[2]+=12u;goto L8c61b5f4; /* add #0xc,r2 */
L8c61b5f4: r[3]=rd(m,bus,&io_status,0x8c61b754u,4);if(io_status)return io_status;goto L8c61b5f6; /* mov.l 0x8c61b754,r3 */
L8c61b5f6: r[8]+=16u;goto L8c61b5f8; /* add #0x10,r8 */
L8c61b5f8: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b5fa; /* fmov fr3,@(r0,r15) */
L8c61b5fa: r[0]=24u;goto L8c61b5fc; /* mov #0x18,r0 */
L8c61b5fc: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b5fe; /* fmov.s @r2,fr3 */
L8c61b5fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b600; /* fmul fr4,fr3 */
L8c61b600: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b602; /* fmov fr3,@(r0,r15) */
L8c61b602: r[0]=24u;goto L8c61b604; /* mov #0x18,r0 */
L8c61b604: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b606; /* fmov.s @(r0,r13),fr3 */
L8c61b606: r[0]=20u;goto L8c61b608; /* mov #0x14,r0 */
L8c61b608: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b60a; /* fmov.s fr3,@-r8 */
L8c61b60a: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b60c; /* fmov.s @(r0,r13),fr3 */
L8c61b60c: r[0]=16u;goto L8c61b60e; /* mov #0x10,r0 */
L8c61b60e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b610; /* fmov.s fr3,@-r8 */
L8c61b610: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b612; /* fmov.s @(r0,r13),fr3 */
L8c61b612: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b614; /* fmov.s fr3,@-r8 */
L8c61b614: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b616; /* mov.l r3,@-r8 */
L8c61b616: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b618; /* pref @r8 */
L8c61b618: r[0]=24u;goto L8c61b61a; /* mov #0x18,r0 */
L8c61b61a: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b61c; /* fmov.s @(r0,r12),fr3 */
L8c61b61c: r[8]+=48u;goto L8c61b61e; /* add #0x30,r8 */
L8c61b61e: r[0]=20u;goto L8c61b620; /* mov #0x14,r0 */
L8c61b620: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b622; /* fmov.s fr3,@-r8 */
L8c61b622: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b624; /* fmov.s @(r0,r12),fr3 */
L8c61b624: r[0]=16u;goto L8c61b626; /* mov #0x10,r0 */
L8c61b626: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b628; /* fmov.s fr3,@-r8 */
L8c61b628: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b62a; /* fmov.s @(r0,r12),fr3 */
L8c61b62a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b62c; /* fmov.s fr3,@-r8 */
L8c61b62c: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b62e; /* mov.l r3,@-r8 */
L8c61b62e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b630; /* pref @r8 */
L8c61b630: r[0]=24u;goto L8c61b632; /* mov #0x18,r0 */
L8c61b632: r[3]=rd(m,bus,&io_status,0x8c61b758u,4);if(io_status)return io_status;goto L8c61b634; /* mov.l 0x8c61b758,r3 */
L8c61b634: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b636; /* fmov.s @(r0,r15),fr3 */
L8c61b636: r[8]+=64u;goto L8c61b638; /* add #0x40,r8 */
L8c61b638: r[0]=20u;goto L8c61b63a; /* mov #0x14,r0 */
L8c61b63a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b63c; /* fmov.s fr3,@-r8 */
L8c61b63c: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b63e; /* fmov.s @(r0,r15),fr3 */
L8c61b63e: r[0]=16u;goto L8c61b640; /* mov #0x10,r0 */
L8c61b640: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b642; /* fmov.s fr3,@-r8 */
L8c61b642: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b644; /* fmov.s @(r0,r15),fr3 */
L8c61b644: r[0]=12u;goto L8c61b646; /* mov #0xc,r0 */
L8c61b646: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b648; /* fmov.s fr3,@-r8 */
L8c61b648: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b64a; /* fmov.s @(r0,r15),fr3 */
L8c61b64a: r[0]=24u;goto L8c61b64c; /* mov #0x18,r0 */
L8c61b64c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b64e; /* fmov.s fr3,@-r8 */
L8c61b64e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b650; /* fmov.s @(r0,r14),fr3 */
L8c61b650: r[0]=20u;goto L8c61b652; /* mov #0x14,r0 */
L8c61b652: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b654; /* fmov.s fr3,@-r8 */
L8c61b654: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b656; /* fmov.s @(r0,r14),fr3 */
L8c61b656: r[0]=16u;goto L8c61b658; /* mov #0x10,r0 */
L8c61b658: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b65a; /* fmov.s fr3,@-r8 */
L8c61b65a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b65c; /* fmov.s @(r0,r14),fr3 */
L8c61b65c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b65e; /* fmov.s fr3,@-r8 */
L8c61b65e: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b660; /* mov.l r3,@-r8 */
L8c61b660: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b662; /* pref @r8 */
L8c61b662: r[0]=rd(m,bus,&io_status,0x8c61b75cu,4);if(io_status)return io_status;goto L8c61b664; /* mov.l 0x8c61b75c,r0 */
L8c61b664: r[8]+=32u;goto L8c61b666; /* add #0x20,r8 */
L8c61b666: r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61b668; /* mov.l @r0,r3 */
L8c61b668: r[3]+=3u;goto L8c61b66a; /* add #0x3,r3 */
L8c61b66a: wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61b706; /* bra 0x8c61b706 */
L8c61b66e: r[0]=28u;goto L8c61b670; /* mov #0x1c,r0 */
L8c61b670: r[2]=rd(m,bus,&io_status,0x8c61b760u,4);if(io_status)return io_status;goto L8c61b672; /* mov.l 0x8c61b760,r2 */
L8c61b672: r[4]=r[15];goto L8c61b674; /* mov r15,r4 */
L8c61b674: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61b676; /* mov.l r3,@r2 */
L8c61b676: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61b678; /* fmov.s @r12,fr3 */
L8c61b678: fload(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c61b67a; /* fmov.s @r13,fr2 */
L8c61b67a: r[3]=rd(m,bus,&io_status,0x8c61b738u,4);if(io_status)return io_status;goto L8c61b67c; /* mov.l 0x8c61b738,r3 */
L8c61b67c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b67e; /* fadd fr3,fr2 */
L8c61b67e: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61b680; /* fmov.s @r14,fr3 */
L8c61b680: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b682; /* fadd fr3,fr2 */
L8c61b682: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b684; /* fmov fr2,@(r0,r15) */
L8c61b684: r[0]=4u;goto L8c61b686; /* mov #0x4,r0 */
L8c61b686: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b688; /* fmov.s @(r0,r12),fr3 */
L8c61b688: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b68a; /* fmov.s @(r0,r13),fr2 */
L8c61b68a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b68c; /* fadd fr3,fr2 */
L8c61b68c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b68e; /* fmov.s @(r0,r14),fr3 */
L8c61b68e: r[0]=32u;goto L8c61b690; /* mov #0x20,r0 */
L8c61b690: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b692; /* fadd fr3,fr2 */
L8c61b692: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b694; /* fmov fr2,@(r0,r15) */
L8c61b694: r[0]=8u;goto L8c61b696; /* mov #0x8,r0 */
L8c61b696: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b698; /* fmov.s @(r0,r13),fr2 */
L8c61b698: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b69a; /* fmov.s @(r0,r12),fr3 */
L8c61b69a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b69c; /* fadd fr3,fr2 */
L8c61b69c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b69e; /* fmov.s @(r0,r14),fr3 */
L8c61b69e: r[0]=36u;goto L8c61b6a0; /* mov #0x24,r0 */
L8c61b6a0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b6a2; /* fadd fr3,fr2 */
L8c61b6a2: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b6a4; /* fmov fr2,@(r0,r15) */
L8c61b6a4: target=r[3];*pr=0x8c61b6a8u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b6a8; /* jsr @r3 */
L8c61b6a8: r[3]=rd(m,bus,&io_status,0x8c61b73cu,4);if(io_status)return io_status;goto L8c61b6aa; /* mov.l 0x8c61b73c,r3 */
L8c61b6aa: r[0]=28u;goto L8c61b6ac; /* mov #0x1c,r0 */
L8c61b6ac: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b6ae; /* fmov.s @(r0,r15),fr3 */
L8c61b6ae: r[0]=32u;goto L8c61b6b0; /* mov #0x20,r0 */
L8c61b6b0: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61b6b2; /* fmov.s @r3,fr2 */
L8c61b6b2: r[3]+=8u;goto L8c61b6b4; /* add #0x8,r3 */
L8c61b6b4: r[2]=rd(m,bus,&io_status,0x8c61b740u,4);if(io_status)return io_status;goto L8c61b6b6; /* mov.l 0x8c61b740,r2 */
L8c61b6b6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b6b8; /* fmul fr3,fr2 */
L8c61b6b8: r[1]=rd(m,bus,&io_status,0x8c61b744u,4);if(io_status)return io_status;goto L8c61b6ba; /* mov.l 0x8c61b744,r1 */
L8c61b6ba: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b6bc; /* fmov.s @r2,fr3 */
L8c61b6bc: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61b6be; /* fmov.s @r1,fr1 */
L8c61b6be: r[2]=rd(m,bus,&io_status,0x8c61b748u,4);if(io_status)return io_status;goto L8c61b6c0; /* mov.l 0x8c61b748,r2 */
L8c61b6c0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b6c2; /* fsub fr2,fr3 */
L8c61b6c2: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b6c4; /* fmov.s @(r0,r15),fr2 */
L8c61b6c4: r[0]=36u;goto L8c61b6c6; /* mov #0x24,r0 */
L8c61b6c6: r[1]=rd(m,bus,&io_status,0x8c61b750u,4);if(io_status)return io_status;goto L8c61b6c8; /* mov.l 0x8c61b750,r1 */
L8c61b6c8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b6ca; /* fmul fr2,fr1 */
L8c61b6ca: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b6cc; /* fmov.s @(r0,r15),fr2 */
L8c61b6cc: r[0]=12u;goto L8c61b6ce; /* mov #0xc,r0 */
L8c61b6ce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b6d0; /* fsub fr1,fr3 */
L8c61b6d0: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61b6d2; /* fmov.s @r3,fr1 */
L8c61b6d2: r[3]=rd(m,bus,&io_status,0x8c61b74cu,4);if(io_status)return io_status;goto L8c61b6d4; /* mov.l 0x8c61b74c,r3 */
L8c61b6d4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b6d6; /* fmul fr2,fr1 */
L8c61b6d6: fcopy(f,4u,3u);goto L8c61b6d8; /* fmov fr3,fr4 */
L8c61b6d8: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b6da; /* fmov.s @r2,fr3 */
L8c61b6da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b6dc; /* fsub fr1,fr4 */
L8c61b6dc: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b6de; /* fmov fr3,@(r0,r15) */
L8c61b6de: r[0]=16u;goto L8c61b6e0; /* mov #0x10,r0 */
L8c61b6e0: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61b6e2; /* fmov.s @r3,fr3 */
L8c61b6e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b6e4; /* fmul fr4,fr3 */
L8c61b6e4: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b6e6; /* fmov fr3,@(r0,r15) */
L8c61b6e6: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61b6e8; /* fmov.s @r1,fr3 */
L8c61b6e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b6ea; /* fmul fr4,fr3 */
L8c61b6ea: r[0]=20u;goto L8c61b6ec; /* mov #0x14,r0 */
L8c61b6ec: r[2]+=12u;goto L8c61b6ee; /* add #0xc,r2 */
L8c61b6ee: r[7]=r[15];goto L8c61b6f0; /* mov r15,r7 */
L8c61b6f0: r[3]=rd(m,bus,&io_status,0x8c61b764u,4);if(io_status)return io_status;goto L8c61b6f2; /* mov.l 0x8c61b764,r3 */
L8c61b6f2: r[5]=r[12];goto L8c61b6f4; /* mov r12,r5 */
L8c61b6f4: r[6]=r[14];goto L8c61b6f6; /* mov r14,r6 */
L8c61b6f6: r[7]+=12u;goto L8c61b6f8; /* add #0xc,r7 */
L8c61b6f8: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b6fa; /* fmov fr3,@(r0,r15) */
L8c61b6fa: r[0]=24u;goto L8c61b6fc; /* mov #0x18,r0 */
L8c61b6fc: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b6fe; /* fmov.s @r2,fr3 */
L8c61b6fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b700; /* fmul fr4,fr3 */
L8c61b700: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b702; /* fmov fr3,@(r0,r15) */
L8c61b702: target=r[3];*pr=0x8c61b706u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b706; /* jsr @r3 */
L8c61b706: r[11]+=1u;goto L8c61b708; /* add #0x1,r11 */
L8c61b708: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b70a; /* mov.l @r15,r2 */
L8c61b70a: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61b70c; /* mov.w @(0x2,r2),r0 */
L8c61b70c: r[0]=r[0]&65535u;goto L8c61b70e; /* extu.w r0,r0 */
L8c61b70e: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61b710; /* cmp/ge r0,r11 */
L8c61b710: if((*sr&1u))goto L8c61b716;goto L8c61b712; /* bt 0x8c61b716 */
L8c61b712: goto L8c61b4f8; /* bra 0x8c61b4f8 */
L8c61b716: goto L8c61bdf0; /* bra 0x8c61bdf0 */
L8c61b71a: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b71c; /* mov.l @r15,r1 */
L8c61b71c: r[2]=rd(m,bus,&io_status,0x8c61b768u,4);if(io_status)return io_status;goto L8c61b71e; /* mov.l 0x8c61b768,r2 */
L8c61b71e: r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61b720; /* mov.w @(0x2,r1),r0 */
L8c61b720: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61b722; /* mov.l @r2,r1 */
L8c61b722: r[0]=r[0]&65535u;goto L8c61b724; /* extu.w r0,r0 */
L8c61b724: r[0]<<=2;goto L8c61b726; /* shll2 r0 */
L8c61b726: r[1]+=r[0];goto L8c61b728; /* add r0,r1 */
L8c61b728: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61b72a; /* mov.l r1,@r2 */
L8c61b72a: r[9]=0u;goto L8c61b9ee; /* bra 0x8c61b9ee */
L8c61b76c: r[11]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b76e; /* mov.w @r10+,r11 */
L8c61b76e: r[3]=rd(m,bus,&io_status,0x8c61ba0cu,4);if(io_status)return io_status;goto L8c61b770; /* mov.l 0x8c61ba0c,r3 */
L8c61b770: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b772; /* mov.w @r10+,r13 */
L8c61b772: r[11]=r[11]&65535u;goto L8c61b774; /* extu.w r11,r11 */
L8c61b774: r[11]<<=2;goto L8c61b776; /* shll2 r11 */
L8c61b776: r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61b778; /* mov.l @r3,r5 */
L8c61b778: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b77a; /* mov.w @r10+,r14 */
L8c61b77a: r[13]=r[13]&65535u;goto L8c61b77c; /* extu.w r13,r13 */
L8c61b77c: r[13]<<=2;goto L8c61b77e; /* shll2 r13 */
L8c61b77e: r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b780; /* mov.w @r10+,r12 */
L8c61b780: r[14]=r[14]&65535u;goto L8c61b782; /* extu.w r14,r14 */
L8c61b782: r[13]<<=2;goto L8c61b784; /* shll2 r13 */
L8c61b784: r[11]<<=2;goto L8c61b786; /* shll2 r11 */
L8c61b786: r[12]=r[12]&65535u;goto L8c61b788; /* extu.w r12,r12 */
L8c61b788: r[14]<<=2;goto L8c61b78a; /* shll2 r14 */
L8c61b78a: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61b78c; /* shll r11 */
L8c61b78c: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61b78e; /* shll r13 */
L8c61b78e: r[11]+=r[5];goto L8c61b790; /* add r5,r11 */
L8c61b790: r[12]<<=2;goto L8c61b792; /* shll2 r12 */
L8c61b792: r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61b794; /* mov.l @(0xc,r11),r4 */
L8c61b794: r[13]+=r[5];goto L8c61b796; /* add r5,r13 */
L8c61b796: r[14]<<=2;goto L8c61b798; /* shll2 r14 */
L8c61b798: r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61b79a; /* mov.l @(0xc,r13),r2 */
L8c61b79a: r[12]<<=2;goto L8c61b79c; /* shll2 r12 */
L8c61b79c: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61b79e; /* shll r14 */
L8c61b79e: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61b7a0; /* shll r12 */
L8c61b7a0: r[14]+=r[5];goto L8c61b7a2; /* add r5,r14 */
L8c61b7a2: r[1]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61b7a4; /* mov.l @(0xc,r14),r1 */
L8c61b7a4: r[4]&=r[2];goto L8c61b7a6; /* and r2,r4 */
L8c61b7a6: r[12]+=r[5];goto L8c61b7a8; /* add r5,r12 */
L8c61b7a8: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61b7aa; /* mov.l @(0xc,r12),r2 */
L8c61b7aa: r[4]&=r[1];goto L8c61b7ac; /* and r1,r4 */
L8c61b7ac: r[1]=63u;goto L8c61b7ae; /* mov #0x3f,r1 */
L8c61b7ae: r[4]&=r[2];goto L8c61b7b0; /* and r2,r4 */
L8c61b7b0: *sr=(*sr&~1u)|!(r[1]&r[4]);goto L8c61b7b2; /* tst r1,r4 */
L8c61b7b2: if((*sr&1u))goto L8c61b7b8;goto L8c61b7b4; /* bt 0x8c61b7b8 */
L8c61b7b4: goto L8c61b9ec; /* bra 0x8c61b9ec */
L8c61b7b8: r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61b7ba; /* mov.l @(0xc,r11),r4 */
L8c61b7ba: r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61b7bc; /* mov.l @(0xc,r13),r3 */
L8c61b7bc: r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61b7be; /* mov.l @(0xc,r14),r2 */
L8c61b7be: r[4]|=r[3];goto L8c61b7c0; /* or r3,r4 */
L8c61b7c0: r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61b7c2; /* mov.l @(0xc,r12),r3 */
L8c61b7c2: r[4]|=r[2];goto L8c61b7c4; /* or r2,r4 */
L8c61b7c4: r[2]=1u;goto L8c61b7c6; /* mov #0x1,r2 */
L8c61b7c6: r[4]|=r[3];goto L8c61b7c8; /* or r3,r4 */
L8c61b7c8: *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61b7ca; /* tst r2,r4 */
L8c61b7ca: if((*sr&1u))goto L8c61b7d0;goto L8c61b7cc; /* bt 0x8c61b7d0 */
L8c61b7cc: goto L8c61b926; /* bra 0x8c61b926 */
L8c61b7d0: r[0]=16u;goto L8c61b7d2; /* mov #0x10,r0 */
L8c61b7d2: fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b7d4; /* fmov.s @(r0,r13),fr5 */
L8c61b7d4: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b7d6; /* fmov.s @(r0,r11),fr3 */
L8c61b7d6: r[0]=20u;goto L8c61b7d8; /* mov #0x14,r0 */
L8c61b7d8: fcopy(f,2u,5u);goto L8c61b7da; /* fmov fr5,fr2 */
L8c61b7da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61b7dc; /* fsub fr3,fr2 */
L8c61b7dc: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b7de; /* fmov.s @(r0,r14),fr3 */
L8c61b7de: fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b7e0; /* fmov.s @(r0,r13),fr4 */
L8c61b7e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b7e2; /* fsub fr4,fr3 */
L8c61b7e2: fcopy(f,1u,4u);goto L8c61b7e4; /* fmov fr4,fr1 */
L8c61b7e4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b7e6; /* fmul fr3,fr2 */
L8c61b7e6: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b7e8; /* fmov.s @(r0,r11),fr3 */
L8c61b7e8: r[0]=16u;goto L8c61b7ea; /* mov #0x10,r0 */
L8c61b7ea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61b7ec; /* fsub fr3,fr1 */
L8c61b7ec: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b7ee; /* fmov.s @(r0,r14),fr3 */
L8c61b7ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b7f0; /* fsub fr5,fr3 */
L8c61b7f0: fcopy(f,4u,2u);goto L8c61b7f2; /* fmov fr2,fr4 */
L8c61b7f2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b7f4; /* fmul fr3,fr1 */
L8c61b7f4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61b7f6; /* fldi0 fr3 */
L8c61b7f6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b7f8; /* fsub fr1,fr4 */
L8c61b7f8: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61b7fa; /* fcmp/gt fr3,fr4 */
L8c61b7fa: if(!(*sr&1u))goto L8c61b800;goto L8c61b7fc; /* bf 0x8c61b800 */
L8c61b7fc: goto L8c61b9ec; /* bra 0x8c61b9ec */
L8c61b800: fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c61b802; /* fmov.s @r11,fr2 */
L8c61b802: r[0]=28u;goto L8c61b804; /* mov #0x1c,r0 */
L8c61b804: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61b806; /* fmov.s @r13,fr3 */
L8c61b806: r[4]=r[15];goto L8c61b808; /* mov r15,r4 */
L8c61b808: r[3]=rd(m,bus,&io_status,0x8c61ba10u,4);if(io_status)return io_status;goto L8c61b80a; /* mov.l 0x8c61ba10,r3 */
L8c61b80a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b80c; /* fadd fr3,fr2 */
L8c61b80c: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61b80e; /* fmov.s @r14,fr3 */
L8c61b80e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b810; /* fadd fr3,fr2 */
L8c61b810: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b812; /* fmov fr2,@(r0,r15) */
L8c61b812: r[0]=4u;goto L8c61b814; /* mov #0x4,r0 */
L8c61b814: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b816; /* fmov.s @(r0,r11),fr2 */
L8c61b816: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b818; /* fmov.s @(r0,r13),fr3 */
L8c61b818: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b81a; /* fadd fr3,fr2 */
L8c61b81a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b81c; /* fmov.s @(r0,r14),fr3 */
L8c61b81c: r[0]=32u;goto L8c61b81e; /* mov #0x20,r0 */
L8c61b81e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b820; /* fadd fr3,fr2 */
L8c61b820: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b822; /* fmov fr2,@(r0,r15) */
L8c61b822: r[0]=8u;goto L8c61b824; /* mov #0x8,r0 */
L8c61b824: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b826; /* fmov.s @(r0,r11),fr2 */
L8c61b826: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b828; /* fmov.s @(r0,r13),fr3 */
L8c61b828: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b82a; /* fadd fr3,fr2 */
L8c61b82a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b82c; /* fmov.s @(r0,r14),fr3 */
L8c61b82c: r[0]=36u;goto L8c61b82e; /* mov #0x24,r0 */
L8c61b82e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b830; /* fadd fr3,fr2 */
L8c61b830: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b832; /* fmov fr2,@(r0,r15) */
L8c61b832: target=r[3];*pr=0x8c61b836u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b836; /* jsr @r3 */
L8c61b836: r[3]=rd(m,bus,&io_status,0x8c61ba14u,4);if(io_status)return io_status;goto L8c61b838; /* mov.l 0x8c61ba14,r3 */
L8c61b838: r[0]=28u;goto L8c61b83a; /* mov #0x1c,r0 */
L8c61b83a: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b83c; /* fmov.s @(r0,r15),fr3 */
L8c61b83c: r[0]=32u;goto L8c61b83e; /* mov #0x20,r0 */
L8c61b83e: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61b840; /* fmov.s @r3,fr2 */
L8c61b840: r[3]+=8u;goto L8c61b842; /* add #0x8,r3 */
L8c61b842: r[2]=rd(m,bus,&io_status,0x8c61ba18u,4);if(io_status)return io_status;goto L8c61b844; /* mov.l 0x8c61ba18,r2 */
L8c61b844: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b846; /* fmul fr3,fr2 */
L8c61b846: r[1]=rd(m,bus,&io_status,0x8c61ba1cu,4);if(io_status)return io_status;goto L8c61b848; /* mov.l 0x8c61ba1c,r1 */
L8c61b848: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b84a; /* fmov.s @r2,fr3 */
L8c61b84a: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61b84c; /* fmov.s @r1,fr1 */
L8c61b84c: r[2]=rd(m,bus,&io_status,0x8c61ba20u,4);if(io_status)return io_status;goto L8c61b84e; /* mov.l 0x8c61ba20,r2 */
L8c61b84e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b850; /* fsub fr2,fr3 */
L8c61b850: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b852; /* fmov.s @(r0,r15),fr2 */
L8c61b852: r[0]=36u;goto L8c61b854; /* mov #0x24,r0 */
L8c61b854: r[1]=rd(m,bus,&io_status,0x8c61ba28u,4);if(io_status)return io_status;goto L8c61b856; /* mov.l 0x8c61ba28,r1 */
L8c61b856: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b858; /* fmul fr2,fr1 */
L8c61b858: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b85a; /* fmov.s @(r0,r15),fr2 */
L8c61b85a: r[0]=12u;goto L8c61b85c; /* mov #0xc,r0 */
L8c61b85c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b85e; /* fsub fr1,fr3 */
L8c61b85e: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61b860; /* fmov.s @r3,fr1 */
L8c61b860: r[3]=rd(m,bus,&io_status,0x8c61ba24u,4);if(io_status)return io_status;goto L8c61b862; /* mov.l 0x8c61ba24,r3 */
L8c61b862: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b864; /* fmul fr2,fr1 */
L8c61b864: fcopy(f,4u,3u);goto L8c61b866; /* fmov fr3,fr4 */
L8c61b866: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b868; /* fmov.s @r2,fr3 */
L8c61b868: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b86a; /* fsub fr1,fr4 */
L8c61b86a: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b86c; /* fmov fr3,@(r0,r15) */
L8c61b86c: r[0]=16u;goto L8c61b86e; /* mov #0x10,r0 */
L8c61b86e: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61b870; /* fmov.s @r3,fr3 */
L8c61b870: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b872; /* fmul fr4,fr3 */
L8c61b872: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b874; /* fmov fr3,@(r0,r15) */
L8c61b874: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61b876; /* fmov.s @r1,fr3 */
L8c61b876: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b878; /* fmul fr4,fr3 */
L8c61b878: r[0]=20u;goto L8c61b87a; /* mov #0x14,r0 */
L8c61b87a: r[2]+=12u;goto L8c61b87c; /* add #0xc,r2 */
L8c61b87c: r[3]=rd(m,bus,&io_status,0x8c61ba2cu,4);if(io_status)return io_status;goto L8c61b87e; /* mov.l 0x8c61ba2c,r3 */
L8c61b87e: r[8]+=16u;goto L8c61b880; /* add #0x10,r8 */
L8c61b880: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b882; /* fmov fr3,@(r0,r15) */
L8c61b882: r[0]=24u;goto L8c61b884; /* mov #0x18,r0 */
L8c61b884: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b886; /* fmov.s @r2,fr3 */
L8c61b886: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b888; /* fmul fr4,fr3 */
L8c61b888: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b88a; /* fmov fr3,@(r0,r15) */
L8c61b88a: r[0]=24u;goto L8c61b88c; /* mov #0x18,r0 */
L8c61b88c: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b88e; /* fmov.s @(r0,r11),fr3 */
L8c61b88e: r[0]=20u;goto L8c61b890; /* mov #0x14,r0 */
L8c61b890: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b892; /* fmov.s fr3,@-r8 */
L8c61b892: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b894; /* fmov.s @(r0,r11),fr3 */
L8c61b894: r[0]=16u;goto L8c61b896; /* mov #0x10,r0 */
L8c61b896: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b898; /* fmov.s fr3,@-r8 */
L8c61b898: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b89a; /* fmov.s @(r0,r11),fr3 */
L8c61b89a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b89c; /* fmov.s fr3,@-r8 */
L8c61b89c: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b89e; /* mov.l r3,@-r8 */
L8c61b89e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b8a0; /* pref @r8 */
L8c61b8a0: r[0]=24u;goto L8c61b8a2; /* mov #0x18,r0 */
L8c61b8a2: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b8a4; /* fmov.s @(r0,r13),fr3 */
L8c61b8a4: r[8]+=48u;goto L8c61b8a6; /* add #0x30,r8 */
L8c61b8a6: r[0]=20u;goto L8c61b8a8; /* mov #0x14,r0 */
L8c61b8a8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8aa; /* fmov.s fr3,@-r8 */
L8c61b8aa: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b8ac; /* fmov.s @(r0,r13),fr3 */
L8c61b8ac: r[0]=16u;goto L8c61b8ae; /* mov #0x10,r0 */
L8c61b8ae: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8b0; /* fmov.s fr3,@-r8 */
L8c61b8b0: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b8b2; /* fmov.s @(r0,r13),fr3 */
L8c61b8b2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8b4; /* fmov.s fr3,@-r8 */
L8c61b8b4: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b8b6; /* mov.l r3,@-r8 */
L8c61b8b6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b8b8; /* pref @r8 */
L8c61b8b8: r[0]=24u;goto L8c61b8ba; /* mov #0x18,r0 */
L8c61b8ba: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b8bc; /* fmov.s @(r0,r15),fr3 */
L8c61b8bc: r[8]+=64u;goto L8c61b8be; /* add #0x40,r8 */
L8c61b8be: r[0]=20u;goto L8c61b8c0; /* mov #0x14,r0 */
L8c61b8c0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8c2; /* fmov.s fr3,@-r8 */
L8c61b8c2: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b8c4; /* fmov.s @(r0,r15),fr3 */
L8c61b8c4: r[0]=16u;goto L8c61b8c6; /* mov #0x10,r0 */
L8c61b8c6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8c8; /* fmov.s fr3,@-r8 */
L8c61b8c8: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b8ca; /* fmov.s @(r0,r15),fr3 */
L8c61b8ca: r[0]=12u;goto L8c61b8cc; /* mov #0xc,r0 */
L8c61b8cc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8ce; /* fmov.s fr3,@-r8 */
L8c61b8ce: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b8d0; /* fmov.s @(r0,r15),fr3 */
L8c61b8d0: r[0]=24u;goto L8c61b8d2; /* mov #0x18,r0 */
L8c61b8d2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8d4; /* fmov.s fr3,@-r8 */
L8c61b8d4: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b8d6; /* fmov.s @(r0,r14),fr3 */
L8c61b8d6: r[0]=20u;goto L8c61b8d8; /* mov #0x14,r0 */
L8c61b8d8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8da; /* fmov.s fr3,@-r8 */
L8c61b8da: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b8dc; /* fmov.s @(r0,r14),fr3 */
L8c61b8dc: r[0]=16u;goto L8c61b8de; /* mov #0x10,r0 */
L8c61b8de: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8e0; /* fmov.s fr3,@-r8 */
L8c61b8e0: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b8e2; /* fmov.s @(r0,r14),fr3 */
L8c61b8e2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8e4; /* fmov.s fr3,@-r8 */
L8c61b8e4: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b8e6; /* mov.l r3,@-r8 */
L8c61b8e6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b8e8; /* pref @r8 */
L8c61b8e8: r[0]=24u;goto L8c61b8ea; /* mov #0x18,r0 */
L8c61b8ea: r[3]=rd(m,bus,&io_status,0x8c61ba30u,4);if(io_status)return io_status;goto L8c61b8ec; /* mov.l 0x8c61ba30,r3 */
L8c61b8ec: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b8ee; /* fmov.s @(r0,r15),fr3 */
L8c61b8ee: r[8]+=64u;goto L8c61b8f0; /* add #0x40,r8 */
L8c61b8f0: r[0]=20u;goto L8c61b8f2; /* mov #0x14,r0 */
L8c61b8f2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8f4; /* fmov.s fr3,@-r8 */
L8c61b8f4: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b8f6; /* fmov.s @(r0,r15),fr3 */
L8c61b8f6: r[0]=16u;goto L8c61b8f8; /* mov #0x10,r0 */
L8c61b8f8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8fa; /* fmov.s fr3,@-r8 */
L8c61b8fa: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b8fc; /* fmov.s @(r0,r15),fr3 */
L8c61b8fc: r[0]=12u;goto L8c61b8fe; /* mov #0xc,r0 */
L8c61b8fe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b900; /* fmov.s fr3,@-r8 */
L8c61b900: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b902; /* fmov.s @(r0,r15),fr3 */
L8c61b902: r[0]=24u;goto L8c61b904; /* mov #0x18,r0 */
L8c61b904: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b906; /* fmov.s fr3,@-r8 */
L8c61b906: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b908; /* fmov.s @(r0,r12),fr3 */
L8c61b908: r[0]=20u;goto L8c61b90a; /* mov #0x14,r0 */
L8c61b90a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b90c; /* fmov.s fr3,@-r8 */
L8c61b90c: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b90e; /* fmov.s @(r0,r12),fr3 */
L8c61b90e: r[0]=16u;goto L8c61b910; /* mov #0x10,r0 */
L8c61b910: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b912; /* fmov.s fr3,@-r8 */
L8c61b912: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b914; /* fmov.s @(r0,r12),fr3 */
L8c61b914: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b916; /* fmov.s fr3,@-r8 */
L8c61b916: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b918; /* mov.l r3,@-r8 */
L8c61b918: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b91a; /* pref @r8 */
L8c61b91a: r[0]=rd(m,bus,&io_status,0x8c61ba34u,4);if(io_status)return io_status;goto L8c61b91c; /* mov.l 0x8c61ba34,r0 */
L8c61b91c: r[8]+=32u;goto L8c61b91e; /* add #0x20,r8 */
L8c61b91e: r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61b920; /* mov.l @r0,r3 */
L8c61b920: r[3]+=4u;goto L8c61b922; /* add #0x4,r3 */
L8c61b922: wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61b9ec; /* bra 0x8c61b9ec */
L8c61b926: r[0]=rd(m,bus,&io_status,0x8c61ba38u,4);if(io_status)return io_status;goto L8c61b928; /* mov.l 0x8c61ba38,r0 */
L8c61b928: r[4]=r[15];goto L8c61b92a; /* mov r15,r4 */
L8c61b92a: r[3]=rd(m,bus,&io_status,0x8c61ba10u,4);if(io_status)return io_status;goto L8c61b92c; /* mov.l 0x8c61ba10,r3 */
L8c61b92c: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61b92e; /* mov.l r2,@r0 */
L8c61b92e: r[0]=28u;goto L8c61b930; /* mov #0x1c,r0 */
L8c61b930: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61b932; /* fmov.s @r13,fr3 */
L8c61b932: fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c61b934; /* fmov.s @r11,fr2 */
L8c61b934: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b936; /* fadd fr3,fr2 */
L8c61b936: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61b938; /* fmov.s @r14,fr3 */
L8c61b938: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b93a; /* fadd fr3,fr2 */
L8c61b93a: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b93c; /* fmov fr2,@(r0,r15) */
L8c61b93c: r[0]=4u;goto L8c61b93e; /* mov #0x4,r0 */
L8c61b93e: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b940; /* fmov.s @(r0,r11),fr2 */
L8c61b940: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b942; /* fmov.s @(r0,r13),fr3 */
L8c61b942: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b944; /* fadd fr3,fr2 */
L8c61b944: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b946; /* fmov.s @(r0,r14),fr3 */
L8c61b946: r[0]=32u;goto L8c61b948; /* mov #0x20,r0 */
L8c61b948: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b94a; /* fadd fr3,fr2 */
L8c61b94a: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b94c; /* fmov fr2,@(r0,r15) */
L8c61b94c: r[0]=8u;goto L8c61b94e; /* mov #0x8,r0 */
L8c61b94e: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b950; /* fmov.s @(r0,r11),fr2 */
L8c61b950: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b952; /* fmov.s @(r0,r13),fr3 */
L8c61b952: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b954; /* fadd fr3,fr2 */
L8c61b954: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b956; /* fmov.s @(r0,r14),fr3 */
L8c61b956: r[0]=36u;goto L8c61b958; /* mov #0x24,r0 */
L8c61b958: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b95a; /* fadd fr3,fr2 */
L8c61b95a: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b95c; /* fmov fr2,@(r0,r15) */
L8c61b95c: target=r[3];*pr=0x8c61b960u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b960; /* jsr @r3 */
L8c61b960: r[3]=rd(m,bus,&io_status,0x8c61ba14u,4);if(io_status)return io_status;goto L8c61b962; /* mov.l 0x8c61ba14,r3 */
L8c61b962: r[0]=28u;goto L8c61b964; /* mov #0x1c,r0 */
L8c61b964: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b966; /* fmov.s @(r0,r15),fr3 */
L8c61b966: r[0]=32u;goto L8c61b968; /* mov #0x20,r0 */
L8c61b968: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61b96a; /* fmov.s @r3,fr2 */
L8c61b96a: r[3]+=8u;goto L8c61b96c; /* add #0x8,r3 */
L8c61b96c: r[2]=rd(m,bus,&io_status,0x8c61ba18u,4);if(io_status)return io_status;goto L8c61b96e; /* mov.l 0x8c61ba18,r2 */
L8c61b96e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b970; /* fmul fr3,fr2 */
L8c61b970: r[1]=rd(m,bus,&io_status,0x8c61ba1cu,4);if(io_status)return io_status;goto L8c61b972; /* mov.l 0x8c61ba1c,r1 */
L8c61b972: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b974; /* fmov.s @r2,fr3 */
L8c61b974: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61b976; /* fmov.s @r1,fr1 */
L8c61b976: r[2]=rd(m,bus,&io_status,0x8c61ba20u,4);if(io_status)return io_status;goto L8c61b978; /* mov.l 0x8c61ba20,r2 */
L8c61b978: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b97a; /* fsub fr2,fr3 */
L8c61b97a: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b97c; /* fmov.s @(r0,r15),fr2 */
L8c61b97c: r[0]=36u;goto L8c61b97e; /* mov #0x24,r0 */
L8c61b97e: r[1]=rd(m,bus,&io_status,0x8c61ba28u,4);if(io_status)return io_status;goto L8c61b980; /* mov.l 0x8c61ba28,r1 */
L8c61b980: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b982; /* fmul fr2,fr1 */
L8c61b982: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b984; /* fmov.s @(r0,r15),fr2 */
L8c61b984: r[0]=12u;goto L8c61b986; /* mov #0xc,r0 */
L8c61b986: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b988; /* fsub fr1,fr3 */
L8c61b988: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61b98a; /* fmov.s @r3,fr1 */
L8c61b98a: r[3]=rd(m,bus,&io_status,0x8c61ba24u,4);if(io_status)return io_status;goto L8c61b98c; /* mov.l 0x8c61ba24,r3 */
L8c61b98c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b98e; /* fmul fr2,fr1 */
L8c61b98e: fcopy(f,4u,3u);goto L8c61b990; /* fmov fr3,fr4 */
L8c61b990: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b992; /* fmov.s @r2,fr3 */
L8c61b992: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b994; /* fsub fr1,fr4 */
L8c61b994: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b996; /* fmov fr3,@(r0,r15) */
L8c61b996: r[0]=16u;goto L8c61b998; /* mov #0x10,r0 */
L8c61b998: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61b99a; /* fmov.s @r3,fr3 */
L8c61b99a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b99c; /* fmul fr4,fr3 */
L8c61b99c: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b99e; /* fmov fr3,@(r0,r15) */
L8c61b99e: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61b9a0; /* fmov.s @r1,fr3 */
L8c61b9a0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b9a2; /* fmul fr4,fr3 */
L8c61b9a2: r[0]=20u;goto L8c61b9a4; /* mov #0x14,r0 */
L8c61b9a4: r[2]+=12u;goto L8c61b9a6; /* add #0xc,r2 */
L8c61b9a6: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b9a8; /* fmov fr3,@(r0,r15) */
L8c61b9a8: r[0]=24u;goto L8c61b9aa; /* mov #0x18,r0 */
L8c61b9aa: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b9ac; /* fmov.s @r2,fr3 */
L8c61b9ac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b9ae; /* fmul fr4,fr3 */
L8c61b9ae: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b9b0; /* fmov fr3,@(r0,r15) */
L8c61b9b0: r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61b9b2; /* mov.l @(0xc,r11),r4 */
L8c61b9b2: r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61b9b4; /* mov.l @(0xc,r13),r3 */
L8c61b9b4: r[0]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61b9b6; /* mov.l @(0xc,r14),r0 */
L8c61b9b6: r[4]&=r[3];goto L8c61b9b8; /* and r3,r4 */
L8c61b9b8: r[3]=1u;goto L8c61b9ba; /* mov #0x1,r3 */
L8c61b9ba: r[4]&=r[0];goto L8c61b9bc; /* and r0,r4 */
L8c61b9bc: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61b9be; /* tst r3,r4 */
L8c61b9be: if(!(*sr&1u))goto L8c61b9ce;goto L8c61b9c0; /* bf 0x8c61b9ce */
L8c61b9c0: r[1]=rd(m,bus,&io_status,0x8c61ba3cu,4);if(io_status)return io_status;goto L8c61b9c2; /* mov.l 0x8c61ba3c,r1 */
L8c61b9c2: r[7]=r[15];goto L8c61b9c4; /* mov r15,r7 */
L8c61b9c4: r[5]=r[13];goto L8c61b9c6; /* mov r13,r5 */
L8c61b9c6: r[7]+=12u;goto L8c61b9c8; /* add #0xc,r7 */
L8c61b9c8: r[6]=r[14];goto L8c61b9ca; /* mov r14,r6 */
L8c61b9ca: target=r[1];*pr=0x8c61b9ceu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b9ce; /* jsr @r1 */
L8c61b9ce: r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61b9d0; /* mov.l @(0xc,r13),r4 */
L8c61b9d0: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61b9d2; /* mov.l @(0xc,r14),r3 */
L8c61b9d2: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61b9d4; /* mov.l @(0xc,r12),r2 */
L8c61b9d4: r[4]&=r[3];goto L8c61b9d6; /* and r3,r4 */
L8c61b9d6: r[3]=1u;goto L8c61b9d8; /* mov #0x1,r3 */
L8c61b9d8: r[4]&=r[2];goto L8c61b9da; /* and r2,r4 */
L8c61b9da: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61b9dc; /* tst r3,r4 */
L8c61b9dc: if(!(*sr&1u))goto L8c61b9ec;goto L8c61b9de; /* bf 0x8c61b9ec */
L8c61b9de: r[1]=rd(m,bus,&io_status,0x8c61ba3cu,4);if(io_status)return io_status;goto L8c61b9e0; /* mov.l 0x8c61ba3c,r1 */
L8c61b9e0: r[7]=r[15];goto L8c61b9e2; /* mov r15,r7 */
L8c61b9e2: r[5]=r[14];goto L8c61b9e4; /* mov r14,r5 */
L8c61b9e4: r[7]+=12u;goto L8c61b9e6; /* add #0xc,r7 */
L8c61b9e6: r[6]=r[12];goto L8c61b9e8; /* mov r12,r6 */
L8c61b9e8: target=r[1];*pr=0x8c61b9ecu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b9ec; /* jsr @r1 */
L8c61b9ec: r[9]+=1u;goto L8c61b9ee; /* add #0x1,r9 */
L8c61b9ee: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b9f0; /* mov.l @r15,r3 */
L8c61b9f0: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61b9f2; /* mov.w @(0x2,r3),r0 */
L8c61b9f2: r[0]=r[0]&65535u;goto L8c61b9f4; /* extu.w r0,r0 */
L8c61b9f4: *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61b9f6; /* cmp/ge r0,r9 */
L8c61b9f6: if((*sr&1u))goto L8c61b9fc;goto L8c61b9f8; /* bt 0x8c61b9fc */
L8c61b9f8: goto L8c61b76c; /* bra 0x8c61b76c */
L8c61b9fc: goto L8c61bdf0; /* bra 0x8c61bdf0 */
L8c61ba00: r[1]=0u;goto L8c61ba02; /* mov #0x0,r1 */
L8c61ba02: wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c61bd86; /* bra 0x8c61bd86 */
L8c61ba40: r[3]=rd(m,bus,&io_status,0x8c61bc68u,4);if(io_status)return io_status;goto L8c61ba42; /* mov.l 0x8c61bc68,r3 */
L8c61ba42: r[7]=0u;goto L8c61ba44; /* mov #0x0,r7 */
L8c61ba44: r[9]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61ba46; /* mov.w @r10+,r9 */
L8c61ba46: r[4]=0u;goto L8c61ba48; /* mov #0x0,r4 */
L8c61ba48: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ba4a; /* mov.l @r3,r2 */
L8c61ba4a: r[5]=r[7];goto L8c61ba4c; /* mov r7,r5 */
L8c61ba4c: r[9]=r[9]&65535u;goto L8c61ba4e; /* extu.w r9,r9 */
L8c61ba4e: r[2]+=r[9];goto L8c61ba50; /* add r9,r2 */
L8c61ba50: *sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c61ba52; /* cmp/pl r9 */
L8c61ba52: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61ba54; /* mov.l r2,@r3 */
L8c61ba54: r[6]=4294967295u;goto L8c61ba56; /* mov #-0x1,r6 */
L8c61ba56: take=!(*sr&1u);r[4]+=r[10];if(take)goto L8c61ba78;goto L8c61ba5a; /* bf/s 0x8c61ba78 */
L8c61ba5a: r[11]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c61ba5c; /* mov.w @r4+,r11 */
L8c61ba5c: r[5]+=1u;goto L8c61ba5e; /* add #0x1,r5 */
L8c61ba5e: r[2]=rd(m,bus,&io_status,0x8c61bc6cu,4);if(io_status)return io_status;goto L8c61ba60; /* mov.l 0x8c61bc6c,r2 */
L8c61ba60: r[11]=r[11]&65535u;goto L8c61ba62; /* extu.w r11,r11 */
L8c61ba62: r[11]<<=2;goto L8c61ba64; /* shll2 r11 */
L8c61ba64: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ba66; /* mov.l @r2,r3 */
L8c61ba66: r[11]<<=2;goto L8c61ba68; /* shll2 r11 */
L8c61ba68: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61ba6a; /* shll r11 */
L8c61ba6a: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[9]);goto L8c61ba6c; /* cmp/ge r9,r5 */
L8c61ba6c: r[11]+=r[3];goto L8c61ba6e; /* add r3,r11 */
L8c61ba6e: wr(m,bus,&io_status,r[15]+0x8u,r[11],4);if(io_status)return io_status;goto L8c61ba70; /* mov.l r11,@(0x8,r15) */
L8c61ba70: r[11]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61ba72; /* mov.l @(0xc,r11),r11 */
L8c61ba72: r[6]&=r[11];goto L8c61ba74; /* and r11,r6 */
L8c61ba74: take=!(*sr&1u);r[7]|=r[11];if(take)goto L8c61ba5a;goto L8c61ba78; /* bf/s 0x8c61ba5a */
L8c61ba78: r[2]=63u;goto L8c61ba7a; /* mov #0x3f,r2 */
L8c61ba7a: *sr=(*sr&~1u)|!(r[2]&r[6]);goto L8c61ba7c; /* tst r2,r6 */
L8c61ba7c: if(!(*sr&1u))goto L8c61badc;goto L8c61ba7e; /* bf 0x8c61badc */
L8c61ba7e: r[1]=1u;goto L8c61ba80; /* mov #0x1,r1 */
L8c61ba80: *sr=(*sr&~1u)|!(r[1]&r[7]);goto L8c61ba82; /* tst r1,r7 */
L8c61ba82: if((*sr&1u))goto L8c61ba88;goto L8c61ba84; /* bt 0x8c61ba88 */
L8c61ba84: goto L8c61bc98; /* bra 0x8c61bc98 */
L8c61ba88: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ba8a; /* mov.w @(0x2,r10),r0 */
L8c61ba8a: r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ba8c; /* mov.w @r10,r12 */
L8c61ba8c: r[3]=rd(m,bus,&io_status,0x8c61bc6cu,4);if(io_status)return io_status;goto L8c61ba8e; /* mov.l 0x8c61bc6c,r3 */
L8c61ba8e: r[14]=r[0]&65535u;goto L8c61ba90; /* extu.w r0,r14 */
L8c61ba90: r[14]<<=2;goto L8c61ba92; /* shll2 r14 */
L8c61ba92: r[0]=sx(m,bus,&io_status,r[10]+0x4u,2);if(io_status)return io_status;goto L8c61ba94; /* mov.w @(0x4,r10),r0 */
L8c61ba94: r[12]=r[12]&65535u;goto L8c61ba96; /* extu.w r12,r12 */
L8c61ba96: r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ba98; /* mov.l @r3,r4 */
L8c61ba98: r[12]<<=2;goto L8c61ba9a; /* shll2 r12 */
L8c61ba9a: r[14]<<=2;goto L8c61ba9c; /* shll2 r14 */
L8c61ba9c: r[13]=r[0]&65535u;goto L8c61ba9e; /* extu.w r0,r13 */
L8c61ba9e: r[12]<<=2;goto L8c61baa0; /* shll2 r12 */
L8c61baa0: r[0]=16u;goto L8c61baa2; /* mov #0x10,r0 */
L8c61baa2: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61baa4; /* shll r12 */
L8c61baa4: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61baa6; /* shll r14 */
L8c61baa6: r[14]+=r[4];goto L8c61baa8; /* add r4,r14 */
L8c61baa8: fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c61baaa; /* fmov.s @(r0,r14),fr5 */
L8c61baaa: r[13]<<=2;goto L8c61baac; /* shll2 r13 */
L8c61baac: r[12]+=r[4];goto L8c61baae; /* add r4,r12 */
L8c61baae: r[13]<<=2;goto L8c61bab0; /* shll2 r13 */
L8c61bab0: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bab2; /* fmov.s @(r0,r12),fr3 */
L8c61bab2: fcopy(f,2u,5u);goto L8c61bab4; /* fmov fr5,fr2 */
L8c61bab4: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61bab6; /* shll r13 */
L8c61bab6: r[0]=20u;goto L8c61bab8; /* mov #0x14,r0 */
L8c61bab8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61baba; /* fsub fr3,fr2 */
L8c61baba: r[13]+=r[4];goto L8c61babc; /* add r4,r13 */
L8c61babc: fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c61babe; /* fmov.s @(r0,r14),fr4 */
L8c61babe: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bac0; /* fmov.s @(r0,r13),fr3 */
L8c61bac0: fcopy(f,1u,4u);goto L8c61bac2; /* fmov fr4,fr1 */
L8c61bac2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bac4; /* fsub fr4,fr3 */
L8c61bac4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61bac6; /* fmul fr3,fr2 */
L8c61bac6: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bac8; /* fmov.s @(r0,r12),fr3 */
L8c61bac8: r[0]=16u;goto L8c61baca; /* mov #0x10,r0 */
L8c61baca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61bacc; /* fsub fr3,fr1 */
L8c61bacc: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bace; /* fmov.s @(r0,r13),fr3 */
L8c61bace: fcopy(f,4u,2u);goto L8c61bad0; /* fmov fr2,fr4 */
L8c61bad0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bad2; /* fsub fr5,fr3 */
L8c61bad2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bad4; /* fmul fr3,fr1 */
L8c61bad4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61bad6; /* fldi0 fr3 */
L8c61bad6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61bad8; /* fsub fr1,fr4 */
L8c61bad8: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61bada; /* fcmp/gt fr3,fr4 */
L8c61bada: if(!(*sr&1u))goto L8c61bae2;goto L8c61badc; /* bf 0x8c61bae2 */
L8c61badc: *sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c61bade; /* shll r9 */
L8c61bade: r[10]+=r[9];goto L8c61bd80; /* bra 0x8c61bd80 */
L8c61bae2: r[0]=28u;goto L8c61bae4; /* mov #0x1c,r0 */
L8c61bae4: r[2]=rd(m,bus,&io_status,0x8c61bc70u,4);if(io_status)return io_status;goto L8c61bae6; /* mov.l 0x8c61bc70,r2 */
L8c61bae6: r[4]=r[15];goto L8c61bae8; /* mov r15,r4 */
L8c61bae8: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61baea; /* mov.l @r2,r3 */
L8c61baea: r[3]+=r[9];goto L8c61baec; /* add r9,r3 */
L8c61baec: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61baee; /* mov.l r3,@r2 */
L8c61baee: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61baf0; /* fmov.s @r14,fr3 */
L8c61baf0: fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61baf2; /* fmov.s @r12,fr2 */
L8c61baf2: r[3]=rd(m,bus,&io_status,0x8c61bc74u,4);if(io_status)return io_status;goto L8c61baf4; /* mov.l 0x8c61bc74,r3 */
L8c61baf4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61baf6; /* fadd fr3,fr2 */
L8c61baf6: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61baf8; /* fmov.s @r13,fr3 */
L8c61baf8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bafa; /* fadd fr3,fr2 */
L8c61bafa: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bafc; /* fmov fr2,@(r0,r15) */
L8c61bafc: r[0]=4u;goto L8c61bafe; /* mov #0x4,r0 */
L8c61bafe: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bb00; /* fmov.s @(r0,r12),fr2 */
L8c61bb00: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bb02; /* fmov.s @(r0,r14),fr3 */
L8c61bb02: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bb04; /* fadd fr3,fr2 */
L8c61bb04: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bb06; /* fmov.s @(r0,r13),fr3 */
L8c61bb06: r[0]=32u;goto L8c61bb08; /* mov #0x20,r0 */
L8c61bb08: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bb0a; /* fadd fr3,fr2 */
L8c61bb0a: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb0c; /* fmov fr2,@(r0,r15) */
L8c61bb0c: r[0]=8u;goto L8c61bb0e; /* mov #0x8,r0 */
L8c61bb0e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bb10; /* fmov.s @(r0,r14),fr3 */
L8c61bb10: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bb12; /* fmov.s @(r0,r12),fr2 */
L8c61bb12: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bb14; /* fadd fr3,fr2 */
L8c61bb14: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bb16; /* fmov.s @(r0,r13),fr3 */
L8c61bb16: r[0]=36u;goto L8c61bb18; /* mov #0x24,r0 */
L8c61bb18: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bb1a; /* fadd fr3,fr2 */
L8c61bb1a: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb1c; /* fmov fr2,@(r0,r15) */
L8c61bb1c: target=r[3];*pr=0x8c61bb20u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61bb20; /* jsr @r3 */
L8c61bb20: r[3]=rd(m,bus,&io_status,0x8c61bc78u,4);if(io_status)return io_status;goto L8c61bb22; /* mov.l 0x8c61bc78,r3 */
L8c61bb22: r[0]=28u;goto L8c61bb24; /* mov #0x1c,r0 */
L8c61bb24: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb26; /* fmov.s @(r0,r15),fr3 */
L8c61bb26: r[0]=32u;goto L8c61bb28; /* mov #0x20,r0 */
L8c61bb28: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61bb2a; /* fmov.s @r3,fr2 */
L8c61bb2a: r[3]+=8u;goto L8c61bb2c; /* add #0x8,r3 */
L8c61bb2c: r[2]=rd(m,bus,&io_status,0x8c61bc7cu,4);if(io_status)return io_status;goto L8c61bb2e; /* mov.l 0x8c61bc7c,r2 */
L8c61bb2e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61bb30; /* fmul fr3,fr2 */
L8c61bb30: r[1]=rd(m,bus,&io_status,0x8c61bc80u,4);if(io_status)return io_status;goto L8c61bb32; /* mov.l 0x8c61bc80,r1 */
L8c61bb32: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61bb34; /* fmov.s @r2,fr3 */
L8c61bb34: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61bb36; /* fmov.s @r1,fr1 */
L8c61bb36: r[2]=rd(m,bus,&io_status,0x8c61bc84u,4);if(io_status)return io_status;goto L8c61bb38; /* mov.l 0x8c61bc84,r2 */
L8c61bb38: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bb3a; /* fsub fr2,fr3 */
L8c61bb3a: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb3c; /* fmov.s @(r0,r15),fr2 */
L8c61bb3c: r[0]=36u;goto L8c61bb3e; /* mov #0x24,r0 */
L8c61bb3e: r[1]=rd(m,bus,&io_status,0x8c61bc8cu,4);if(io_status)return io_status;goto L8c61bb40; /* mov.l 0x8c61bc8c,r1 */
L8c61bb40: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bb42; /* fmul fr2,fr1 */
L8c61bb42: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb44; /* fmov.s @(r0,r15),fr2 */
L8c61bb44: r[0]=12u;goto L8c61bb46; /* mov #0xc,r0 */
L8c61bb46: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bb48; /* fsub fr1,fr3 */
L8c61bb48: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61bb4a; /* fmov.s @r3,fr1 */
L8c61bb4a: r[3]=rd(m,bus,&io_status,0x8c61bc88u,4);if(io_status)return io_status;goto L8c61bb4c; /* mov.l 0x8c61bc88,r3 */
L8c61bb4c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bb4e; /* fmul fr2,fr1 */
L8c61bb4e: fcopy(f,4u,3u);goto L8c61bb50; /* fmov fr3,fr4 */
L8c61bb50: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61bb52; /* fmov.s @r2,fr3 */
L8c61bb52: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61bb54; /* fsub fr1,fr4 */
L8c61bb54: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb56; /* fmov fr3,@(r0,r15) */
L8c61bb56: r[0]=16u;goto L8c61bb58; /* mov #0x10,r0 */
L8c61bb58: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61bb5a; /* fmov.s @r3,fr3 */
L8c61bb5a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bb5c; /* fmul fr4,fr3 */
L8c61bb5c: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb5e; /* fmov fr3,@(r0,r15) */
L8c61bb5e: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61bb60; /* fmov.s @r1,fr3 */
L8c61bb60: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bb62; /* fmul fr4,fr3 */
L8c61bb62: r[0]=20u;goto L8c61bb64; /* mov #0x14,r0 */
L8c61bb64: r[2]+=12u;goto L8c61bb66; /* add #0xc,r2 */
L8c61bb66: r[8]+=16u;goto L8c61bb68; /* add #0x10,r8 */
L8c61bb68: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb6a; /* fmov fr3,@(r0,r15) */
L8c61bb6a: r[0]=24u;goto L8c61bb6c; /* mov #0x18,r0 */
L8c61bb6c: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61bb6e; /* fmov.s @r2,fr3 */
L8c61bb6e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bb70; /* fmul fr4,fr3 */
L8c61bb70: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb72; /* fmov fr3,@(r0,r15) */
L8c61bb72: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61bb74; /* mov.w @r10+,r4 */
L8c61bb74: r[0]=rd(m,bus,&io_status,0x8c61bc6cu,4);if(io_status)return io_status;goto L8c61bb76; /* mov.l 0x8c61bc6c,r0 */
L8c61bb76: r[4]=r[4]&65535u;goto L8c61bb78; /* extu.w r4,r4 */
L8c61bb78: r[4]<<=2;goto L8c61bb7a; /* shll2 r4 */
L8c61bb7a: r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61bb7c; /* mov.l @r0,r3 */
L8c61bb7c: r[4]<<=2;goto L8c61bb7e; /* shll2 r4 */
L8c61bb7e: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61bb80; /* shll r4 */
L8c61bb80: r[0]=24u;goto L8c61bb82; /* mov #0x18,r0 */
L8c61bb82: r[4]+=r[3];goto L8c61bb84; /* add r3,r4 */
L8c61bb84: r[3]=rd(m,bus,&io_status,0x8c61bc90u,4);if(io_status)return io_status;goto L8c61bb86; /* mov.l 0x8c61bc90,r3 */
L8c61bb86: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bb88; /* fmov.s @(r0,r4),fr3 */
L8c61bb88: r[0]=20u;goto L8c61bb8a; /* mov #0x14,r0 */
L8c61bb8a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bb8c; /* fmov.s fr3,@-r8 */
L8c61bb8c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bb8e; /* fmov.s @(r0,r4),fr3 */
L8c61bb8e: r[0]=16u;goto L8c61bb90; /* mov #0x10,r0 */
L8c61bb90: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bb92; /* fmov.s fr3,@-r8 */
L8c61bb92: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bb94; /* fmov.s @(r0,r4),fr3 */
L8c61bb94: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bb96; /* fmov.s fr3,@-r8 */
L8c61bb96: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61bb98; /* mov.l r3,@-r8 */
L8c61bb98: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61bb9a; /* pref @r8 */
L8c61bb9a: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61bb9c; /* mov.w @r10+,r4 */
L8c61bb9c: r[0]=24u;goto L8c61bb9e; /* mov #0x18,r0 */
L8c61bb9e: r[2]=rd(m,bus,&io_status,0x8c61bc6cu,4);if(io_status)return io_status;goto L8c61bba0; /* mov.l 0x8c61bc6c,r2 */
L8c61bba0: r[8]+=48u;goto L8c61bba2; /* add #0x30,r8 */
L8c61bba2: r[4]=r[4]&65535u;goto L8c61bba4; /* extu.w r4,r4 */
L8c61bba4: r[4]<<=2;goto L8c61bba6; /* shll2 r4 */
L8c61bba6: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61bba8; /* mov.l @r2,r3 */
L8c61bba8: r[4]<<=2;goto L8c61bbaa; /* shll2 r4 */
L8c61bbaa: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61bbac; /* shll r4 */
L8c61bbac: r[4]+=r[3];goto L8c61bbae; /* add r3,r4 */
L8c61bbae: r[3]=rd(m,bus,&io_status,0x8c61bc90u,4);if(io_status)return io_status;goto L8c61bbb0; /* mov.l 0x8c61bc90,r3 */
L8c61bbb0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bbb2; /* fmov.s @(r0,r4),fr3 */
L8c61bbb2: r[0]=20u;goto L8c61bbb4; /* mov #0x14,r0 */
L8c61bbb4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bbb6; /* fmov.s fr3,@-r8 */
L8c61bbb6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bbb8; /* fmov.s @(r0,r4),fr3 */
L8c61bbb8: r[0]=16u;goto L8c61bbba; /* mov #0x10,r0 */
L8c61bbba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bbbc; /* fmov.s fr3,@-r8 */
L8c61bbbc: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bbbe; /* fmov.s @(r0,r4),fr3 */
L8c61bbbe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bbc0; /* fmov.s fr3,@-r8 */
L8c61bbc0: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61bbc2; /* mov.l r3,@-r8 */
L8c61bbc2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61bbc4; /* pref @r8 */
L8c61bbc4: r[6]=r[9];goto L8c61bbc6; /* mov r9,r6 */
L8c61bbc6: r[4]=2u;goto L8c61bbc8; /* mov #0x2,r4 */
L8c61bbc8: r[6]+=4294967295u;goto L8c61bbca; /* add #-0x1,r6 */
L8c61bbca: r[5]=r[4];goto L8c61bbcc; /* mov r4,r5 */
L8c61bbcc: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61bbce; /* cmp/ge r6,r5 */
L8c61bbce: take=(*sr&1u);r[8]+=32u;if(take)goto L8c61bc1c;goto L8c61bbd2; /* bt/s 0x8c61bc1c */
L8c61bbd2: r[0]=24u;goto L8c61bbd4; /* mov #0x18,r0 */
L8c61bbd4: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61bbd6; /* mov.w @r10+,r4 */
L8c61bbd6: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bbd8; /* fmov.s @(r0,r15),fr3 */
L8c61bbd8: r[8]+=32u;goto L8c61bbda; /* add #0x20,r8 */
L8c61bbda: r[0]=20u;goto L8c61bbdc; /* mov #0x14,r0 */
L8c61bbdc: r[2]=rd(m,bus,&io_status,0x8c61bc6cu,4);if(io_status)return io_status;goto L8c61bbde; /* mov.l 0x8c61bc6c,r2 */
L8c61bbde: r[4]=r[4]&65535u;goto L8c61bbe0; /* extu.w r4,r4 */
L8c61bbe0: r[4]<<=2;goto L8c61bbe2; /* shll2 r4 */
L8c61bbe2: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61bbe4; /* mov.l @r2,r3 */
L8c61bbe4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bbe6; /* fmov.s fr3,@-r8 */
L8c61bbe6: r[4]<<=2;goto L8c61bbe8; /* shll2 r4 */
L8c61bbe8: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bbea; /* fmov.s @(r0,r15),fr3 */
L8c61bbea: r[0]=16u;goto L8c61bbec; /* mov #0x10,r0 */
L8c61bbec: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61bbee; /* shll r4 */
L8c61bbee: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bbf0; /* fmov.s fr3,@-r8 */
L8c61bbf0: r[4]+=r[3];goto L8c61bbf2; /* add r3,r4 */
L8c61bbf2: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bbf4; /* fmov.s @(r0,r15),fr3 */
L8c61bbf4: r[0]=12u;goto L8c61bbf6; /* mov #0xc,r0 */
L8c61bbf6: r[3]=rd(m,bus,&io_status,0x8c61bc90u,4);if(io_status)return io_status;goto L8c61bbf8; /* mov.l 0x8c61bc90,r3 */
L8c61bbf8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bbfa; /* fmov.s fr3,@-r8 */
L8c61bbfa: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bbfc; /* fmov.s @(r0,r15),fr3 */
L8c61bbfc: r[0]=24u;goto L8c61bbfe; /* mov #0x18,r0 */
L8c61bbfe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc00; /* fmov.s fr3,@-r8 */
L8c61bc00: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bc02; /* fmov.s @(r0,r4),fr3 */
L8c61bc02: r[0]=20u;goto L8c61bc04; /* mov #0x14,r0 */
L8c61bc04: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc06; /* fmov.s fr3,@-r8 */
L8c61bc06: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bc08; /* fmov.s @(r0,r4),fr3 */
L8c61bc08: r[0]=16u;goto L8c61bc0a; /* mov #0x10,r0 */
L8c61bc0a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc0c; /* fmov.s fr3,@-r8 */
L8c61bc0c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bc0e; /* fmov.s @(r0,r4),fr3 */
L8c61bc0e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc10; /* fmov.s fr3,@-r8 */
L8c61bc10: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61bc12; /* mov.l r3,@-r8 */
L8c61bc12: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61bc14; /* pref @r8 */
L8c61bc14: r[5]+=1u;goto L8c61bc16; /* add #0x1,r5 */
L8c61bc16: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61bc18; /* cmp/ge r6,r5 */
L8c61bc18: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61bbd2;goto L8c61bc1c; /* bf/s 0x8c61bbd2 */
L8c61bc1c: r[0]=24u;goto L8c61bc1e; /* mov #0x18,r0 */
L8c61bc1e: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61bc20; /* mov.w @r10+,r4 */
L8c61bc20: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bc22; /* fmov.s @(r0,r15),fr3 */
L8c61bc22: r[8]+=32u;goto L8c61bc24; /* add #0x20,r8 */
L8c61bc24: r[0]=20u;goto L8c61bc26; /* mov #0x14,r0 */
L8c61bc26: r[2]=rd(m,bus,&io_status,0x8c61bc6cu,4);if(io_status)return io_status;goto L8c61bc28; /* mov.l 0x8c61bc6c,r2 */
L8c61bc28: r[4]=r[4]&65535u;goto L8c61bc2a; /* extu.w r4,r4 */
L8c61bc2a: r[4]<<=2;goto L8c61bc2c; /* shll2 r4 */
L8c61bc2c: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61bc2e; /* mov.l @r2,r3 */
L8c61bc2e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc30; /* fmov.s fr3,@-r8 */
L8c61bc30: r[4]<<=2;goto L8c61bc32; /* shll2 r4 */
L8c61bc32: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bc34; /* fmov.s @(r0,r15),fr3 */
L8c61bc34: r[0]=16u;goto L8c61bc36; /* mov #0x10,r0 */
L8c61bc36: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61bc38; /* shll r4 */
L8c61bc38: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc3a; /* fmov.s fr3,@-r8 */
L8c61bc3a: r[4]+=r[3];goto L8c61bc3c; /* add r3,r4 */
L8c61bc3c: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bc3e; /* fmov.s @(r0,r15),fr3 */
L8c61bc3e: r[0]=12u;goto L8c61bc40; /* mov #0xc,r0 */
L8c61bc40: r[3]=rd(m,bus,&io_status,0x8c61bc94u,4);if(io_status)return io_status;goto L8c61bc42; /* mov.l 0x8c61bc94,r3 */
L8c61bc42: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc44; /* fmov.s fr3,@-r8 */
L8c61bc44: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bc46; /* fmov.s @(r0,r15),fr3 */
L8c61bc46: r[0]=24u;goto L8c61bc48; /* mov #0x18,r0 */
L8c61bc48: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc4a; /* fmov.s fr3,@-r8 */
L8c61bc4a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bc4c; /* fmov.s @(r0,r4),fr3 */
L8c61bc4c: r[0]=20u;goto L8c61bc4e; /* mov #0x14,r0 */
L8c61bc4e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc50; /* fmov.s fr3,@-r8 */
L8c61bc50: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bc52; /* fmov.s @(r0,r4),fr3 */
L8c61bc52: r[0]=16u;goto L8c61bc54; /* mov #0x10,r0 */
L8c61bc54: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc56; /* fmov.s fr3,@-r8 */
L8c61bc56: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bc58; /* fmov.s @(r0,r4),fr3 */
L8c61bc58: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc5a; /* fmov.s fr3,@-r8 */
L8c61bc5a: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61bc5c; /* mov.l r3,@-r8 */
L8c61bc5c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61bc5e; /* pref @r8 */
L8c61bc5e: r[8]+=32u;goto L8c61bd80; /* bra 0x8c61bd80 */
L8c61bc98: r[0]=28u;goto L8c61bc9a; /* mov #0x1c,r0 */
L8c61bc9a: r[2]=rd(m,bus,&io_status,0x8c61be14u,4);if(io_status)return io_status;goto L8c61bc9c; /* mov.l 0x8c61be14,r2 */
L8c61bc9c: r[3]=0u;goto L8c61bc9e; /* mov #0x0,r3 */
L8c61bc9e: r[4]=r[15];goto L8c61bca0; /* mov r15,r4 */
L8c61bca0: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61bca2; /* mov.l r3,@r2 */
L8c61bca2: if((r[14]&0x1f000000u)!=0x0c000000u)return -3;fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61bca4; /* fmov.s @r14,fr3 */
L8c61bca4: fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61bca6; /* fmov.s @r12,fr2 */
L8c61bca6: r[3]=rd(m,bus,&io_status,0x8c61be18u,4);if(io_status)return io_status;goto L8c61bca8; /* mov.l 0x8c61be18,r3 */
L8c61bca8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bcaa; /* fadd fr3,fr2 */
L8c61bcaa: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61bcac; /* fmov.s @r13,fr3 */
L8c61bcac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bcae; /* fadd fr3,fr2 */
L8c61bcae: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bcb0; /* fmov fr2,@(r0,r15) */
L8c61bcb0: r[0]=4u;goto L8c61bcb2; /* mov #0x4,r0 */
L8c61bcb2: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bcb4; /* fmov.s @(r0,r14),fr3 */
L8c61bcb4: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bcb6; /* fmov.s @(r0,r12),fr2 */
L8c61bcb6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bcb8; /* fadd fr3,fr2 */
L8c61bcb8: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bcba; /* fmov.s @(r0,r13),fr3 */
L8c61bcba: r[0]=32u;goto L8c61bcbc; /* mov #0x20,r0 */
L8c61bcbc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bcbe; /* fadd fr3,fr2 */
L8c61bcbe: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bcc0; /* fmov fr2,@(r0,r15) */
L8c61bcc0: r[0]=8u;goto L8c61bcc2; /* mov #0x8,r0 */
L8c61bcc2: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bcc4; /* fmov.s @(r0,r14),fr3 */
L8c61bcc4: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bcc6; /* fmov.s @(r0,r12),fr2 */
L8c61bcc6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bcc8; /* fadd fr3,fr2 */
L8c61bcc8: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bcca; /* fmov.s @(r0,r13),fr3 */
L8c61bcca: r[0]=36u;goto L8c61bccc; /* mov #0x24,r0 */
L8c61bccc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bcce; /* fadd fr3,fr2 */
L8c61bcce: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bcd0; /* fmov fr2,@(r0,r15) */
L8c61bcd0: target=r[3];*pr=0x8c61bcd4u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61bcd4; /* jsr @r3 */
L8c61bcd4: r[3]=rd(m,bus,&io_status,0x8c61be1cu,4);if(io_status)return io_status;goto L8c61bcd6; /* mov.l 0x8c61be1c,r3 */
L8c61bcd6: r[0]=28u;goto L8c61bcd8; /* mov #0x1c,r0 */
L8c61bcd8: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bcda; /* fmov.s @(r0,r15),fr3 */
L8c61bcda: r[0]=32u;goto L8c61bcdc; /* mov #0x20,r0 */
L8c61bcdc: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61bcde; /* fmov.s @r3,fr2 */
L8c61bcde: r[3]+=8u;goto L8c61bce0; /* add #0x8,r3 */
L8c61bce0: r[2]=rd(m,bus,&io_status,0x8c61be20u,4);if(io_status)return io_status;goto L8c61bce2; /* mov.l 0x8c61be20,r2 */
L8c61bce2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61bce4; /* fmul fr3,fr2 */
L8c61bce4: r[1]=rd(m,bus,&io_status,0x8c61be24u,4);if(io_status)return io_status;goto L8c61bce6; /* mov.l 0x8c61be24,r1 */
L8c61bce6: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61bce8; /* fmov.s @r2,fr3 */
L8c61bce8: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61bcea; /* fmov.s @r1,fr1 */
L8c61bcea: r[2]=rd(m,bus,&io_status,0x8c61be28u,4);if(io_status)return io_status;goto L8c61bcec; /* mov.l 0x8c61be28,r2 */
L8c61bcec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bcee; /* fsub fr2,fr3 */
L8c61bcee: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bcf0; /* fmov.s @(r0,r15),fr2 */
L8c61bcf0: r[0]=36u;goto L8c61bcf2; /* mov #0x24,r0 */
L8c61bcf2: r[1]=rd(m,bus,&io_status,0x8c61be30u,4);if(io_status)return io_status;goto L8c61bcf4; /* mov.l 0x8c61be30,r1 */
L8c61bcf4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bcf6; /* fmul fr2,fr1 */
L8c61bcf6: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bcf8; /* fmov.s @(r0,r15),fr2 */
L8c61bcf8: r[0]=12u;goto L8c61bcfa; /* mov #0xc,r0 */
L8c61bcfa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bcfc; /* fsub fr1,fr3 */
L8c61bcfc: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61bcfe; /* fmov.s @r3,fr1 */
L8c61bcfe: r[3]=rd(m,bus,&io_status,0x8c61be2cu,4);if(io_status)return io_status;goto L8c61bd00; /* mov.l 0x8c61be2c,r3 */
L8c61bd00: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bd02; /* fmul fr2,fr1 */
L8c61bd02: fcopy(f,4u,3u);goto L8c61bd04; /* fmov fr3,fr4 */
L8c61bd04: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61bd06; /* fmov.s @r2,fr3 */
L8c61bd06: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61bd08; /* fsub fr1,fr4 */
L8c61bd08: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bd0a; /* fmov fr3,@(r0,r15) */
L8c61bd0a: r[0]=16u;goto L8c61bd0c; /* mov #0x10,r0 */
L8c61bd0c: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61bd0e; /* fmov.s @r3,fr3 */
L8c61bd0e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bd10; /* fmul fr4,fr3 */
L8c61bd10: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bd12; /* fmov fr3,@(r0,r15) */
L8c61bd12: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61bd14; /* fmov.s @r1,fr3 */
L8c61bd14: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bd16; /* fmul fr4,fr3 */
L8c61bd16: r[0]=20u;goto L8c61bd18; /* mov #0x14,r0 */
L8c61bd18: r[2]+=12u;goto L8c61bd1a; /* add #0xc,r2 */
L8c61bd1a: r[9]+=4294967294u;goto L8c61bd1c; /* add #-0x2,r9 */
L8c61bd1c: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bd1e; /* fmov fr3,@(r0,r15) */
L8c61bd1e: r[0]=24u;goto L8c61bd20; /* mov #0x18,r0 */
L8c61bd20: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61bd22; /* fmov.s @r2,fr3 */
L8c61bd22: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bd24; /* fmul fr4,fr3 */
L8c61bd24: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bd26; /* fmov fr3,@(r0,r15) */
L8c61bd26: r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61bd28; /* mov.w @r10+,r12 */
L8c61bd28: r[0]=rd(m,bus,&io_status,0x8c61be34u,4);if(io_status)return io_status;goto L8c61bd2a; /* mov.l 0x8c61be34,r0 */
L8c61bd2a: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61bd2c; /* mov.w @r10+,r14 */
L8c61bd2c: r[12]=r[12]&65535u;goto L8c61bd2e; /* extu.w r12,r12 */
L8c61bd2e: r[12]<<=2;goto L8c61bd30; /* shll2 r12 */
L8c61bd30: r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61bd32; /* mov.l @r0,r3 */
L8c61bd32: r[14]=r[14]&65535u;goto L8c61bd34; /* extu.w r14,r14 */
L8c61bd34: r[14]<<=2;goto L8c61bd36; /* shll2 r14 */
L8c61bd36: r[14]<<=2;goto L8c61bd38; /* shll2 r14 */
L8c61bd38: r[12]<<=2;goto L8c61bd3a; /* shll2 r12 */
L8c61bd3a: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61bd3c; /* shll r12 */
L8c61bd3c: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61bd3e; /* shll r14 */
L8c61bd3e: *sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c61bd40; /* cmp/pl r9 */
L8c61bd40: r[12]+=r[3];goto L8c61bd42; /* add r3,r12 */
L8c61bd42: r[14]+=r[3];goto L8c61bd44; /* add r3,r14 */
L8c61bd44: take=!(*sr&1u);r[11]=0u;if(take)goto L8c61bd80;goto L8c61bd48; /* bf/s 0x8c61bd80 */
L8c61bd48: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61bd4a; /* mov.w @r10+,r13 */
L8c61bd4a: r[2]=rd(m,bus,&io_status,0x8c61be34u,4);if(io_status)return io_status;goto L8c61bd4c; /* mov.l 0x8c61be34,r2 */
L8c61bd4c: r[13]=r[13]&65535u;goto L8c61bd4e; /* extu.w r13,r13 */
L8c61bd4e: r[4]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61bd50; /* mov.l @(0xc,r12),r4 */
L8c61bd50: r[13]<<=2;goto L8c61bd52; /* shll2 r13 */
L8c61bd52: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61bd54; /* mov.l @r2,r3 */
L8c61bd54: r[13]<<=2;goto L8c61bd56; /* shll2 r13 */
L8c61bd56: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61bd58; /* shll r13 */
L8c61bd58: r[13]+=r[3];goto L8c61bd5a; /* add r3,r13 */
L8c61bd5a: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61bd5c; /* mov.l @(0xc,r14),r3 */
L8c61bd5c: r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61bd5e; /* mov.l @(0xc,r13),r1 */
L8c61bd5e: r[4]&=r[3];goto L8c61bd60; /* and r3,r4 */
L8c61bd60: r[3]=1u;goto L8c61bd62; /* mov #0x1,r3 */
L8c61bd62: r[4]&=r[1];goto L8c61bd64; /* and r1,r4 */
L8c61bd64: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61bd66; /* tst r3,r4 */
L8c61bd66: if(!(*sr&1u))goto L8c61bd76;goto L8c61bd68; /* bf 0x8c61bd76 */
L8c61bd68: r[1]=rd(m,bus,&io_status,0x8c61be38u,4);if(io_status)return io_status;goto L8c61bd6a; /* mov.l 0x8c61be38,r1 */
L8c61bd6a: r[7]=r[15];goto L8c61bd6c; /* mov r15,r7 */
L8c61bd6c: r[5]=r[14];goto L8c61bd6e; /* mov r14,r5 */
L8c61bd6e: r[7]+=12u;goto L8c61bd70; /* add #0xc,r7 */
L8c61bd70: r[6]=r[13];goto L8c61bd72; /* mov r13,r6 */
L8c61bd72: target=r[1];*pr=0x8c61bd76u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61bd76; /* jsr @r1 */
L8c61bd76: r[11]+=1u;goto L8c61bd78; /* add #0x1,r11 */
L8c61bd78: r[12]=r[14];goto L8c61bd7a; /* mov r14,r12 */
L8c61bd7a: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[9]);goto L8c61bd7c; /* cmp/ge r9,r11 */
L8c61bd7c: take=!(*sr&1u);r[14]=r[13];if(take)goto L8c61bd48;goto L8c61bd80; /* bf/s 0x8c61bd48 */
L8c61bd80: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61bd82; /* mov.l @(0x4,r15),r2 */
L8c61bd82: r[2]+=1u;goto L8c61bd84; /* add #0x1,r2 */
L8c61bd84: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c61bd86; /* mov.l r2,@(0x4,r15) */
L8c61bd86: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61bd88; /* mov.l @r15,r3 */
L8c61bd88: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61bd8a; /* mov.w @(0x2,r3),r0 */
L8c61bd8a: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61bd8c; /* mov.l @(0x4,r15),r3 */
L8c61bd8c: r[0]=r[0]&65535u;goto L8c61bd8e; /* extu.w r0,r0 */
L8c61bd8e: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c61bd90; /* cmp/ge r0,r3 */
L8c61bd90: if((*sr&1u))goto L8c61bd96;goto L8c61bd92; /* bt 0x8c61bd96 */
L8c61bd92: goto L8c61ba40; /* bra 0x8c61ba40 */
L8c61bd96: goto L8c61bdf0; /* bra 0x8c61bdf0 */
L8c61bd9a: r[1]=rd(m,bus,&io_status,0x8c61be3cu,4);if(io_status)return io_status;goto L8c61bd9c; /* mov.l 0x8c61be3c,r1 */
L8c61bd9c: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61bd9e; /* mov.l @r15,r2 */
L8c61bd9e: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61bda0; /* mov.l r2,@r1 */
L8c61bda0: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61bda2; /* frchg */
L8c61bda2: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61bda4; /* fmov.s fr0,@-r15 */
L8c61bda4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61bda6; /* fmov.s fr1,@-r15 */
L8c61bda6: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61bda8; /* fmov.s fr2,@-r15 */
L8c61bda8: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61bdaa; /* fmov.s fr3,@-r15 */
L8c61bdaa: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61bdac; /* fmov.s fr4,@-r15 */
L8c61bdac: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61bdae; /* fmov.s fr5,@-r15 */
L8c61bdae: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61bdb0; /* fmov.s fr6,@-r15 */
L8c61bdb0: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61bdb2; /* fmov.s fr7,@-r15 */
L8c61bdb2: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61bdb4; /* fmov.s fr8,@-r15 */
L8c61bdb4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61bdb6; /* fmov.s fr9,@-r15 */
L8c61bdb6: r[0]=rd(m,bus,&io_status,0x8c61be04u,4);if(io_status)return io_status;goto L8c61bdb8; /* mov.l 0x8c61be04,r0 */
L8c61bdb8: fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdba; /* fmov.s @r0+,fr0 */
L8c61bdba: fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdbc; /* fmov.s @r0+,fr1 */
L8c61bdbc: fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c61bdbe; /* fmov.s @r0,fr2 */
L8c61bdbe: r[0]=rd(m,bus,&io_status,0x8c61be08u,4);if(io_status)return io_status;goto L8c61bdc0; /* mov.l 0x8c61be08,r0 */
L8c61bdc0: r[0]+=16u;goto L8c61bdc2; /* add #0x10,r0 */
L8c61bdc2: fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdc4; /* fmov.s @r0+,fr4 */
L8c61bdc4: fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdc6; /* fmov.s @r0+,fr5 */
L8c61bdc6: fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdc8; /* fmov.s @r0+,fr6 */
L8c61bdc8: fload(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;goto L8c61bdca; /* fmov.s @r0,fr7 */
L8c61bdca: r[0]=rd(m,bus,&io_status,0x8c61be0cu,4);if(io_status)return io_status;goto L8c61bdcc; /* mov.l 0x8c61be0c,r0 */
L8c61bdcc: fload(m,bus,&io_status,f,8u,r[0]);if(io_status)return io_status;goto L8c61bdce; /* fmov.s @r0,fr8 */
L8c61bdce: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61bdd0; /* frchg */
L8c61bdd0: r[2]=rd(m,bus,&io_status,0x8c61be40u,4);if(io_status)return io_status;goto L8c61bdd2; /* mov.l 0x8c61be40,r2 */
L8c61bdd2: r[3]=rd(m,bus,&io_status,0x8c61be3cu,4);if(io_status)return io_status;goto L8c61bdd4; /* mov.l 0x8c61be3c,r3 */
L8c61bdd4: target=r[2];*pr=0x8c61bdd8u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61bdd8; /* jsr @r2 */
L8c61bdd8: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61bdda; /* frchg */
L8c61bdda: fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bddc; /* fmov.s @r15+,fr9 */
L8c61bddc: fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdde; /* fmov.s @r15+,fr8 */
L8c61bdde: fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bde0; /* fmov.s @r15+,fr7 */
L8c61bde0: fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bde2; /* fmov.s @r15+,fr6 */
L8c61bde2: fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bde4; /* fmov.s @r15+,fr5 */
L8c61bde4: fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bde6; /* fmov.s @r15+,fr4 */
L8c61bde6: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bde8; /* fmov.s @r15+,fr3 */
L8c61bde8: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdea; /* fmov.s @r15+,fr2 */
L8c61bdea: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdec; /* fmov.s @r15+,fr1 */
L8c61bdec: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdee; /* fmov.s @r15+,fr0 */
L8c61bdee: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61bdf0; /* frchg */
L8c61bdf0: r[15]+=40u;goto L8c61bdf2; /* add #0x28,r15 */
L8c61bdf2: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61bdf4; /* lds.l @r15+,PR */
L8c61bdf4: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61bdf6; /* mov.l @r15+,r9 */
L8c61bdf6: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61bdf8; /* mov.l @r15+,r10 */
L8c61bdf8: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61bdfa; /* mov.l @r15+,r11 */
L8c61bdfa: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61bdfc; /* mov.l @r15+,r12 */
L8c61bdfc: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61bdfe; /* mov.l @r15+,r13 */
L8c61bdfe: if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_face_vector_normalize_8c63a94c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)sr;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63a94c;
L8c63a94c: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a94e; /* fmov.s @r4+,fr0 */
L8c63a94e: fload(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a950; /* fmov.s @r4+,fr1 */
L8c63a950: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a952; /* fmov.s @r4+,fr2 */
L8c63a952: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c63a954; /* fldi0 fr3 */
L8c63a954: status=sa_sh4_vector_numeric(0xf0edu,f,*fpul);if(status)return status;goto L8c63a956; /* fipr fv0,fv0 */
L8c63a956: fcopy(f,4u,0u);goto L8c63a958; /* fmov fr0,fr4 */
L8c63a958: fcopy(f,0u,3u);goto L8c63a95a; /* fmov fr3,fr0 */
L8c63a95a: status=sa_sh4_vector_numeric(0xf37du,f,*fpul);if(status)return status;goto L8c63a95c; /* fsrra fr3 */
L8c63a95c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63a95e; /* fmul fr3,fr2 */
L8c63a95e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63a960; /* fmul fr3,fr1 */
L8c63a960: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63a962; /* fmul fr3,fr4 */
L8c63a962: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c63a964; /* fmov.s fr2,@-r4 */
L8c63a964: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;goto L8c63a966; /* fmov.s fr1,@-r4 */
L8c63a966: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c63a968; /* fmov.s fr4,@-r4 */
L8c63a968: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}return 0; /* rts */
}
