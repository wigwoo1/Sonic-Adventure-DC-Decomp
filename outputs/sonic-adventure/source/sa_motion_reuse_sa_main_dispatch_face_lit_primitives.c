#include "sa_draw_motion_stop.h"
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
L8c61ae3c: sa_motion_publish_stop(ctx,0x8c61ae3cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61ae3e; /* mov.l r14,@-r15 */
L8c61ae3e: sa_motion_publish_stop(ctx,0x8c61ae3eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61ae40; /* mov.l r13,@-r15 */
L8c61ae40: sa_motion_publish_stop(ctx,0x8c61ae40u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61ae42; /* mov.l r12,@-r15 */
L8c61ae42: sa_motion_publish_stop(ctx,0x8c61ae42u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61ae44; /* mov.l r11,@-r15 */
L8c61ae44: sa_motion_publish_stop(ctx,0x8c61ae44u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61ae46; /* mov.l r10,@-r15 */
L8c61ae46: sa_motion_publish_stop(ctx,0x8c61ae46u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61ae48; /* mov.l r9,@-r15 */
L8c61ae48: sa_motion_publish_stop(ctx,0x8c61ae48u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61ae4a; /* sts.l PR,@-r15 */
L8c61ae4a: sa_motion_publish_stop(ctx,0x8c61ae4au); r[15]+=4294967264u;goto L8c61ae4c; /* add #-0x20,r15 */
L8c61ae4c: sa_motion_publish_stop(ctx,0x8c61ae4cu); r[3]=rd(m,bus,&io_status,0x8c61aea4u,4);if(io_status)return io_status;goto L8c61ae4e; /* mov.l 0x8c61aea4,r3 */
L8c61ae4e: sa_motion_publish_stop(ctx,0x8c61ae4eu); r[10]=r[4];goto L8c61ae50; /* mov r4,r10 */
L8c61ae50: sa_motion_publish_stop(ctx,0x8c61ae50u); r[13]=rd(m,bus,&io_status,0x8c61aea0u,4);if(io_status)return io_status;goto L8c61ae52; /* mov.l 0x8c61aea0,r13 */
L8c61ae52: sa_motion_publish_stop(ctx,0x8c61ae52u); wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61ae54; /* mov.l r4,@r15 */
L8c61ae54: sa_motion_publish_stop(ctx,0x8c61ae54u); r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ae56; /* mov.l @r15,r0 */
L8c61ae56: sa_motion_publish_stop(ctx,0x8c61ae56u); r[10]=rd(m,bus,&io_status,r[10]+0x4u,4);if(io_status)return io_status;goto L8c61ae58; /* mov.l @(0x4,r10),r10 */
L8c61ae58: sa_motion_publish_stop(ctx,0x8c61ae58u); r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61ae5a; /* mov.w @r0,r0 */
L8c61ae5a: sa_motion_publish_stop(ctx,0x8c61ae5au); r[0]=r[0]&65535u;goto L8c61ae5c; /* extu.w r0,r0 */
L8c61ae5c: sa_motion_publish_stop(ctx,0x8c61ae5cu); r[0]&=r[3];goto L8c61ae5e; /* and r3,r0 */
L8c61ae5e: sa_motion_publish_stop(ctx,0x8c61ae5eu); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61ae60; /* cmp/eq #0x0,r0 */
L8c61ae60: sa_motion_publish_stop(ctx,0x8c61ae62u); take=(*sr&1u);r[4]=0u;if(take)goto L8c61ae86;goto L8c61ae64; /* bt/s 0x8c61ae86 */
L8c61ae64: sa_motion_publish_stop(ctx,0x8c61ae64u); r[1]=sx(m,bus,&io_status,0x8c61ae9cu,2);if(io_status)return io_status;goto L8c61ae66; /* mov.w 0x8c61ae9c,r1 */
L8c61ae66: sa_motion_publish_stop(ctx,0x8c61ae66u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61ae68; /* cmp/eq r1,r0 */
L8c61ae68: sa_motion_publish_stop(ctx,0x8c61ae68u); if(!(*sr&1u))goto L8c61ae6e;goto L8c61ae6a; /* bf 0x8c61ae6e */
L8c61ae6a: sa_motion_publish_stop(ctx,0x8c61ae6cu); goto L8c61b008; /* bra 0x8c61b008 */
L8c61ae6e: sa_motion_publish_stop(ctx,0x8c61ae6eu); r[1]=rd(m,bus,&io_status,0x8c61aea8u,4);if(io_status)return io_status;goto L8c61ae70; /* mov.l 0x8c61aea8,r1 */
L8c61ae70: sa_motion_publish_stop(ctx,0x8c61ae70u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61ae72; /* cmp/eq r1,r0 */
L8c61ae72: sa_motion_publish_stop(ctx,0x8c61ae72u); if(!(*sr&1u))goto L8c61ae78;goto L8c61ae74; /* bf 0x8c61ae78 */
L8c61ae74: sa_motion_publish_stop(ctx,0x8c61ae76u); goto L8c61b1b6; /* bra 0x8c61b1b6 */
L8c61ae78: sa_motion_publish_stop(ctx,0x8c61ae78u); r[1]=rd(m,bus,&io_status,0x8c61aea4u,4);if(io_status)return io_status;goto L8c61ae7a; /* mov.l 0x8c61aea4,r1 */
L8c61ae7a: sa_motion_publish_stop(ctx,0x8c61ae7au); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61ae7c; /* cmp/eq r1,r0 */
L8c61ae7c: sa_motion_publish_stop(ctx,0x8c61ae7cu); if(!(*sr&1u))goto L8c61ae82;goto L8c61ae7e; /* bf 0x8c61ae82 */
L8c61ae7e: sa_motion_publish_stop(ctx,0x8c61ae80u); goto L8c61b3dc; /* bra 0x8c61b3dc */
L8c61ae82: sa_motion_publish_stop(ctx,0x8c61ae84u); goto L8c61b432; /* bra 0x8c61b432 */
L8c61ae86: sa_motion_publish_stop(ctx,0x8c61ae86u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ae88; /* mov.l @r15,r2 */
L8c61ae88: sa_motion_publish_stop(ctx,0x8c61ae88u); r[1]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61ae8a; /* mov.l @r13,r1 */
L8c61ae8a: sa_motion_publish_stop(ctx,0x8c61ae8au); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61ae8c; /* mov.w @(0x2,r2),r0 */
L8c61ae8c: sa_motion_publish_stop(ctx,0x8c61ae8cu); r[0]=r[0]&65535u;goto L8c61ae8e; /* extu.w r0,r0 */
L8c61ae8e: sa_motion_publish_stop(ctx,0x8c61ae8eu); r[2]=r[0];goto L8c61ae90; /* mov r0,r2 */
L8c61ae90: sa_motion_publish_stop(ctx,0x8c61ae90u); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61ae92; /* shll r0 */
L8c61ae92: sa_motion_publish_stop(ctx,0x8c61ae92u); r[0]+=r[2];goto L8c61ae94; /* add r2,r0 */
L8c61ae94: sa_motion_publish_stop(ctx,0x8c61ae94u); r[1]+=r[0];goto L8c61ae96; /* add r0,r1 */
L8c61ae96: sa_motion_publish_stop(ctx,0x8c61ae96u); wr(m,bus,&io_status,r[13],r[1],4);if(io_status)return io_status;goto L8c61ae98; /* mov.l r1,@r13 */
L8c61ae98: sa_motion_publish_stop(ctx,0x8c61ae9au); r[11]=r[4];goto L8c61aff6; /* bra 0x8c61aff6 */
L8c61aeac: sa_motion_publish_stop(ctx,0x8c61aeacu); r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61aeae; /* mov.w @r10+,r12 */
L8c61aeae: sa_motion_publish_stop(ctx,0x8c61aeaeu); r[0]=16u;goto L8c61aeb0; /* mov #0x10,r0 */
L8c61aeb0: sa_motion_publish_stop(ctx,0x8c61aeb0u); r[3]=rd(m,bus,&io_status,0x8c61b1bcu,4);if(io_status)return io_status;goto L8c61aeb2; /* mov.l 0x8c61b1bc,r3 */
L8c61aeb2: sa_motion_publish_stop(ctx,0x8c61aeb2u); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61aeb4; /* mov.w @r10+,r13 */
L8c61aeb4: sa_motion_publish_stop(ctx,0x8c61aeb4u); r[12]=r[12]&65535u;goto L8c61aeb6; /* extu.w r12,r12 */
L8c61aeb6: sa_motion_publish_stop(ctx,0x8c61aeb6u); r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61aeb8; /* mov.l @r3,r4 */
L8c61aeb8: sa_motion_publish_stop(ctx,0x8c61aeb8u); r[12]<<=2;goto L8c61aeba; /* shll2 r12 */
L8c61aeba: sa_motion_publish_stop(ctx,0x8c61aebau); r[13]=r[13]&65535u;goto L8c61aebc; /* extu.w r13,r13 */
L8c61aebc: sa_motion_publish_stop(ctx,0x8c61aebcu); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61aebe; /* mov.w @r10+,r14 */
L8c61aebe: sa_motion_publish_stop(ctx,0x8c61aebeu); r[13]<<=2;goto L8c61aec0; /* shll2 r13 */
L8c61aec0: sa_motion_publish_stop(ctx,0x8c61aec0u); r[13]<<=2;goto L8c61aec2; /* shll2 r13 */
L8c61aec2: sa_motion_publish_stop(ctx,0x8c61aec2u); r[12]<<=2;goto L8c61aec4; /* shll2 r12 */
L8c61aec4: sa_motion_publish_stop(ctx,0x8c61aec4u); r[14]=r[14]&65535u;goto L8c61aec6; /* extu.w r14,r14 */
L8c61aec6: sa_motion_publish_stop(ctx,0x8c61aec6u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61aec8; /* shll r12 */
L8c61aec8: sa_motion_publish_stop(ctx,0x8c61aec8u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61aeca; /* shll r13 */
L8c61aeca: sa_motion_publish_stop(ctx,0x8c61aecau); r[13]+=r[4];goto L8c61aecc; /* add r4,r13 */
L8c61aecc: sa_motion_publish_stop(ctx,0x8c61aeccu); fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61aece; /* fmov.s @(r0,r13),fr5 */
L8c61aece: sa_motion_publish_stop(ctx,0x8c61aeceu); r[12]+=r[4];goto L8c61aed0; /* add r4,r12 */
L8c61aed0: sa_motion_publish_stop(ctx,0x8c61aed0u); r[14]<<=2;goto L8c61aed2; /* shll2 r14 */
L8c61aed2: sa_motion_publish_stop(ctx,0x8c61aed2u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aed4; /* fmov.s @(r0,r12),fr3 */
L8c61aed4: sa_motion_publish_stop(ctx,0x8c61aed4u); r[14]<<=2;goto L8c61aed6; /* shll2 r14 */
L8c61aed6: sa_motion_publish_stop(ctx,0x8c61aed6u); fcopy(f,2u,5u);goto L8c61aed8; /* fmov fr5,fr2 */
L8c61aed8: sa_motion_publish_stop(ctx,0x8c61aed8u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61aeda; /* shll r14 */
L8c61aeda: sa_motion_publish_stop(ctx,0x8c61aedau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61aedc; /* fsub fr3,fr2 */
L8c61aedc: sa_motion_publish_stop(ctx,0x8c61aedcu); r[0]=20u;goto L8c61aede; /* mov #0x14,r0 */
L8c61aede: sa_motion_publish_stop(ctx,0x8c61aedeu); r[14]+=r[4];goto L8c61aee0; /* add r4,r14 */
L8c61aee0: sa_motion_publish_stop(ctx,0x8c61aee0u); fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61aee2; /* fmov.s @(r0,r13),fr4 */
L8c61aee2: sa_motion_publish_stop(ctx,0x8c61aee2u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61aee4; /* fmov.s @(r0,r14),fr3 */
L8c61aee4: sa_motion_publish_stop(ctx,0x8c61aee4u); fcopy(f,1u,4u);goto L8c61aee6; /* fmov fr4,fr1 */
L8c61aee6: sa_motion_publish_stop(ctx,0x8c61aee6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61aee8; /* fsub fr4,fr3 */
L8c61aee8: sa_motion_publish_stop(ctx,0x8c61aee8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61aeea; /* fmul fr3,fr2 */
L8c61aeea: sa_motion_publish_stop(ctx,0x8c61aeeau); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aeec; /* fmov.s @(r0,r12),fr3 */
L8c61aeec: sa_motion_publish_stop(ctx,0x8c61aeecu); r[0]=16u;goto L8c61aeee; /* mov #0x10,r0 */
L8c61aeee: sa_motion_publish_stop(ctx,0x8c61aeeeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61aef0; /* fsub fr3,fr1 */
L8c61aef0: sa_motion_publish_stop(ctx,0x8c61aef0u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61aef2; /* fmov.s @(r0,r14),fr3 */
L8c61aef2: sa_motion_publish_stop(ctx,0x8c61aef2u); fcopy(f,4u,2u);goto L8c61aef4; /* fmov fr2,fr4 */
L8c61aef4: sa_motion_publish_stop(ctx,0x8c61aef4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61aef6; /* fsub fr5,fr3 */
L8c61aef6: sa_motion_publish_stop(ctx,0x8c61aef6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61aef8; /* fmul fr3,fr1 */
L8c61aef8: sa_motion_publish_stop(ctx,0x8c61aef8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61aefa; /* fldi0 fr3 */
L8c61aefa: sa_motion_publish_stop(ctx,0x8c61aefau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61aefc; /* fsub fr1,fr4 */
L8c61aefc: sa_motion_publish_stop(ctx,0x8c61aefcu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61aefe; /* fcmp/gt fr3,fr4 */
L8c61aefe: sa_motion_publish_stop(ctx,0x8c61aefeu); if((*sr&1u))goto L8c61aff4;goto L8c61af00; /* bt 0x8c61aff4 */
L8c61af00: sa_motion_publish_stop(ctx,0x8c61af00u); fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61af02; /* fmov.s @r12,fr2 */
L8c61af02: sa_motion_publish_stop(ctx,0x8c61af02u); r[0]=20u;goto L8c61af04; /* mov #0x14,r0 */
L8c61af04: sa_motion_publish_stop(ctx,0x8c61af04u); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61af06; /* fmov.s @r13,fr3 */
L8c61af06: sa_motion_publish_stop(ctx,0x8c61af06u); r[4]=r[15];goto L8c61af08; /* mov r15,r4 */
L8c61af08: sa_motion_publish_stop(ctx,0x8c61af08u); r[3]=rd(m,bus,&io_status,0x8c61b1c0u,4);if(io_status)return io_status;goto L8c61af0a; /* mov.l 0x8c61b1c0,r3 */
L8c61af0a: sa_motion_publish_stop(ctx,0x8c61af0au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61af0c; /* fadd fr3,fr2 */
L8c61af0c: sa_motion_publish_stop(ctx,0x8c61af0cu); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61af0e; /* fmov.s @r14,fr3 */
L8c61af0e: sa_motion_publish_stop(ctx,0x8c61af0eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61af10; /* fadd fr3,fr2 */
L8c61af10: sa_motion_publish_stop(ctx,0x8c61af10u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af12; /* fmov fr2,@(r0,r15) */
L8c61af12: sa_motion_publish_stop(ctx,0x8c61af12u); r[0]=4u;goto L8c61af14; /* mov #0x4,r0 */
L8c61af14: sa_motion_publish_stop(ctx,0x8c61af14u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61af16; /* fmov.s @(r0,r12),fr2 */
L8c61af16: sa_motion_publish_stop(ctx,0x8c61af16u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61af18; /* fmov.s @(r0,r13),fr3 */
L8c61af18: sa_motion_publish_stop(ctx,0x8c61af18u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61af1a; /* fadd fr3,fr2 */
L8c61af1a: sa_motion_publish_stop(ctx,0x8c61af1au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61af1c; /* fmov.s @(r0,r14),fr3 */
L8c61af1c: sa_motion_publish_stop(ctx,0x8c61af1cu); r[0]=24u;goto L8c61af1e; /* mov #0x18,r0 */
L8c61af1e: sa_motion_publish_stop(ctx,0x8c61af1eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61af20; /* fadd fr3,fr2 */
L8c61af20: sa_motion_publish_stop(ctx,0x8c61af20u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af22; /* fmov fr2,@(r0,r15) */
L8c61af22: sa_motion_publish_stop(ctx,0x8c61af22u); r[0]=8u;goto L8c61af24; /* mov #0x8,r0 */
L8c61af24: sa_motion_publish_stop(ctx,0x8c61af24u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61af26; /* fmov.s @(r0,r12),fr2 */
L8c61af26: sa_motion_publish_stop(ctx,0x8c61af26u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61af28; /* fmov.s @(r0,r13),fr3 */
L8c61af28: sa_motion_publish_stop(ctx,0x8c61af28u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61af2a; /* fadd fr3,fr2 */
L8c61af2a: sa_motion_publish_stop(ctx,0x8c61af2au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61af2c; /* fmov.s @(r0,r14),fr3 */
L8c61af2c: sa_motion_publish_stop(ctx,0x8c61af2cu); r[0]=28u;goto L8c61af2e; /* mov #0x1c,r0 */
L8c61af2e: sa_motion_publish_stop(ctx,0x8c61af2eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61af30; /* fadd fr3,fr2 */
L8c61af30: sa_motion_publish_stop(ctx,0x8c61af30u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af32; /* fmov fr2,@(r0,r15) */
L8c61af32: sa_motion_publish_stop(ctx,0x8c61af34u); target=r[3];*pr=0x8c61af36u;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61af36; /* jsr @r3 */
L8c61af36: sa_motion_publish_stop(ctx,0x8c61af36u); r[3]=rd(m,bus,&io_status,0x8c61b1c4u,4);if(io_status)return io_status;goto L8c61af38; /* mov.l 0x8c61b1c4,r3 */
L8c61af38: sa_motion_publish_stop(ctx,0x8c61af38u); r[0]=20u;goto L8c61af3a; /* mov #0x14,r0 */
L8c61af3a: sa_motion_publish_stop(ctx,0x8c61af3au); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af3c; /* fmov.s @(r0,r15),fr3 */
L8c61af3c: sa_motion_publish_stop(ctx,0x8c61af3cu); r[0]=24u;goto L8c61af3e; /* mov #0x18,r0 */
L8c61af3e: sa_motion_publish_stop(ctx,0x8c61af3eu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61af40; /* fmov.s @r3,fr2 */
L8c61af40: sa_motion_publish_stop(ctx,0x8c61af40u); r[3]+=8u;goto L8c61af42; /* add #0x8,r3 */
L8c61af42: sa_motion_publish_stop(ctx,0x8c61af42u); r[2]=rd(m,bus,&io_status,0x8c61b1c8u,4);if(io_status)return io_status;goto L8c61af44; /* mov.l 0x8c61b1c8,r2 */
L8c61af44: sa_motion_publish_stop(ctx,0x8c61af44u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61af46; /* fmul fr3,fr2 */
L8c61af46: sa_motion_publish_stop(ctx,0x8c61af46u); r[1]=rd(m,bus,&io_status,0x8c61b1ccu,4);if(io_status)return io_status;goto L8c61af48; /* mov.l 0x8c61b1cc,r1 */
L8c61af48: sa_motion_publish_stop(ctx,0x8c61af48u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61af4a; /* fmov.s @r2,fr3 */
L8c61af4a: sa_motion_publish_stop(ctx,0x8c61af4au); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61af4c; /* fmov.s @r1,fr1 */
L8c61af4c: sa_motion_publish_stop(ctx,0x8c61af4cu); r[2]=rd(m,bus,&io_status,0x8c61b1d0u,4);if(io_status)return io_status;goto L8c61af4e; /* mov.l 0x8c61b1d0,r2 */
L8c61af4e: sa_motion_publish_stop(ctx,0x8c61af4eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61af50; /* fsub fr2,fr3 */
L8c61af50: sa_motion_publish_stop(ctx,0x8c61af50u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af52; /* fmov.s @(r0,r15),fr2 */
L8c61af52: sa_motion_publish_stop(ctx,0x8c61af52u); r[0]=28u;goto L8c61af54; /* mov #0x1c,r0 */
L8c61af54: sa_motion_publish_stop(ctx,0x8c61af54u); r[1]=rd(m,bus,&io_status,0x8c61b1d8u,4);if(io_status)return io_status;goto L8c61af56; /* mov.l 0x8c61b1d8,r1 */
L8c61af56: sa_motion_publish_stop(ctx,0x8c61af56u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61af58; /* fmul fr2,fr1 */
L8c61af58: sa_motion_publish_stop(ctx,0x8c61af58u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af5a; /* fmov.s @(r0,r15),fr2 */
L8c61af5a: sa_motion_publish_stop(ctx,0x8c61af5au); r[0]=4u;goto L8c61af5c; /* mov #0x4,r0 */
L8c61af5c: sa_motion_publish_stop(ctx,0x8c61af5cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61af5e; /* fsub fr1,fr3 */
L8c61af5e: sa_motion_publish_stop(ctx,0x8c61af5eu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61af60; /* fmov.s @r3,fr1 */
L8c61af60: sa_motion_publish_stop(ctx,0x8c61af60u); r[3]=rd(m,bus,&io_status,0x8c61b1d4u,4);if(io_status)return io_status;goto L8c61af62; /* mov.l 0x8c61b1d4,r3 */
L8c61af62: sa_motion_publish_stop(ctx,0x8c61af62u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61af64; /* fmul fr2,fr1 */
L8c61af64: sa_motion_publish_stop(ctx,0x8c61af64u); fcopy(f,4u,3u);goto L8c61af66; /* fmov fr3,fr4 */
L8c61af66: sa_motion_publish_stop(ctx,0x8c61af66u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61af68; /* fmov.s @r2,fr3 */
L8c61af68: sa_motion_publish_stop(ctx,0x8c61af68u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61af6a; /* fsub fr1,fr4 */
L8c61af6a: sa_motion_publish_stop(ctx,0x8c61af6au); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af6c; /* fmov fr3,@(r0,r15) */
L8c61af6c: sa_motion_publish_stop(ctx,0x8c61af6cu); r[0]=8u;goto L8c61af6e; /* mov #0x8,r0 */
L8c61af6e: sa_motion_publish_stop(ctx,0x8c61af6eu); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61af70; /* fmov.s @r3,fr3 */
L8c61af70: sa_motion_publish_stop(ctx,0x8c61af70u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61af72; /* fmul fr4,fr3 */
L8c61af72: sa_motion_publish_stop(ctx,0x8c61af72u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af74; /* fmov fr3,@(r0,r15) */
L8c61af74: sa_motion_publish_stop(ctx,0x8c61af74u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61af76; /* fmov.s @r1,fr3 */
L8c61af76: sa_motion_publish_stop(ctx,0x8c61af76u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61af78; /* fmul fr4,fr3 */
L8c61af78: sa_motion_publish_stop(ctx,0x8c61af78u); r[0]=12u;goto L8c61af7a; /* mov #0xc,r0 */
L8c61af7a: sa_motion_publish_stop(ctx,0x8c61af7au); r[2]+=12u;goto L8c61af7c; /* add #0xc,r2 */
L8c61af7c: sa_motion_publish_stop(ctx,0x8c61af7cu); r[3]=rd(m,bus,&io_status,0x8c61b1dcu,4);if(io_status)return io_status;goto L8c61af7e; /* mov.l 0x8c61b1dc,r3 */
L8c61af7e: sa_motion_publish_stop(ctx,0x8c61af7eu); r[8]+=16u;goto L8c61af80; /* add #0x10,r8 */
L8c61af80: sa_motion_publish_stop(ctx,0x8c61af80u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af82; /* fmov fr3,@(r0,r15) */
L8c61af82: sa_motion_publish_stop(ctx,0x8c61af82u); r[0]=16u;goto L8c61af84; /* mov #0x10,r0 */
L8c61af84: sa_motion_publish_stop(ctx,0x8c61af84u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61af86; /* fmov.s @r2,fr3 */
L8c61af86: sa_motion_publish_stop(ctx,0x8c61af86u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61af88; /* fmul fr4,fr3 */
L8c61af88: sa_motion_publish_stop(ctx,0x8c61af88u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61af8a; /* fmov fr3,@(r0,r15) */
L8c61af8a: sa_motion_publish_stop(ctx,0x8c61af8au); r[0]=24u;goto L8c61af8c; /* mov #0x18,r0 */
L8c61af8c: sa_motion_publish_stop(ctx,0x8c61af8cu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61af8e; /* fmov.s @(r0,r12),fr3 */
L8c61af8e: sa_motion_publish_stop(ctx,0x8c61af8eu); r[0]=20u;goto L8c61af90; /* mov #0x14,r0 */
L8c61af90: sa_motion_publish_stop(ctx,0x8c61af90u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61af92; /* fmov.s fr3,@-r8 */
L8c61af92: sa_motion_publish_stop(ctx,0x8c61af92u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61af94; /* fmov.s @(r0,r12),fr3 */
L8c61af94: sa_motion_publish_stop(ctx,0x8c61af94u); r[0]=16u;goto L8c61af96; /* mov #0x10,r0 */
L8c61af96: sa_motion_publish_stop(ctx,0x8c61af96u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61af98; /* fmov.s fr3,@-r8 */
L8c61af98: sa_motion_publish_stop(ctx,0x8c61af98u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61af9a; /* fmov.s @(r0,r12),fr3 */
L8c61af9a: sa_motion_publish_stop(ctx,0x8c61af9au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61af9c; /* fmov.s fr3,@-r8 */
L8c61af9c: sa_motion_publish_stop(ctx,0x8c61af9cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61af9e; /* mov.l r3,@-r8 */
L8c61af9e: sa_motion_publish_stop(ctx,0x8c61af9eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61afa0; /* pref @r8 */
L8c61afa0: sa_motion_publish_stop(ctx,0x8c61afa0u); r[0]=24u;goto L8c61afa2; /* mov #0x18,r0 */
L8c61afa2: sa_motion_publish_stop(ctx,0x8c61afa2u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61afa4; /* fmov.s @(r0,r13),fr3 */
L8c61afa4: sa_motion_publish_stop(ctx,0x8c61afa4u); r[8]+=48u;goto L8c61afa6; /* add #0x30,r8 */
L8c61afa6: sa_motion_publish_stop(ctx,0x8c61afa6u); r[0]=20u;goto L8c61afa8; /* mov #0x14,r0 */
L8c61afa8: sa_motion_publish_stop(ctx,0x8c61afa8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afaa; /* fmov.s fr3,@-r8 */
L8c61afaa: sa_motion_publish_stop(ctx,0x8c61afaau); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61afac; /* fmov.s @(r0,r13),fr3 */
L8c61afac: sa_motion_publish_stop(ctx,0x8c61afacu); r[0]=16u;goto L8c61afae; /* mov #0x10,r0 */
L8c61afae: sa_motion_publish_stop(ctx,0x8c61afaeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afb0; /* fmov.s fr3,@-r8 */
L8c61afb0: sa_motion_publish_stop(ctx,0x8c61afb0u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61afb2; /* fmov.s @(r0,r13),fr3 */
L8c61afb2: sa_motion_publish_stop(ctx,0x8c61afb2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afb4; /* fmov.s fr3,@-r8 */
L8c61afb4: sa_motion_publish_stop(ctx,0x8c61afb4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61afb6; /* mov.l r3,@-r8 */
L8c61afb6: sa_motion_publish_stop(ctx,0x8c61afb6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61afb8; /* pref @r8 */
L8c61afb8: sa_motion_publish_stop(ctx,0x8c61afb8u); r[0]=16u;goto L8c61afba; /* mov #0x10,r0 */
L8c61afba: sa_motion_publish_stop(ctx,0x8c61afbau); r[3]=rd(m,bus,&io_status,0x8c61b1e0u,4);if(io_status)return io_status;goto L8c61afbc; /* mov.l 0x8c61b1e0,r3 */
L8c61afbc: sa_motion_publish_stop(ctx,0x8c61afbcu); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61afbe; /* fmov.s @(r0,r15),fr3 */
L8c61afbe: sa_motion_publish_stop(ctx,0x8c61afbeu); r[8]+=64u;goto L8c61afc0; /* add #0x40,r8 */
L8c61afc0: sa_motion_publish_stop(ctx,0x8c61afc0u); r[0]=12u;goto L8c61afc2; /* mov #0xc,r0 */
L8c61afc2: sa_motion_publish_stop(ctx,0x8c61afc2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afc4; /* fmov.s fr3,@-r8 */
L8c61afc4: sa_motion_publish_stop(ctx,0x8c61afc4u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61afc6; /* fmov.s @(r0,r15),fr3 */
L8c61afc6: sa_motion_publish_stop(ctx,0x8c61afc6u); r[0]=8u;goto L8c61afc8; /* mov #0x8,r0 */
L8c61afc8: sa_motion_publish_stop(ctx,0x8c61afc8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afca; /* fmov.s fr3,@-r8 */
L8c61afca: sa_motion_publish_stop(ctx,0x8c61afcau); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61afcc; /* fmov.s @(r0,r15),fr3 */
L8c61afcc: sa_motion_publish_stop(ctx,0x8c61afccu); r[0]=4u;goto L8c61afce; /* mov #0x4,r0 */
L8c61afce: sa_motion_publish_stop(ctx,0x8c61afceu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afd0; /* fmov.s fr3,@-r8 */
L8c61afd0: sa_motion_publish_stop(ctx,0x8c61afd0u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61afd2; /* fmov.s @(r0,r15),fr3 */
L8c61afd2: sa_motion_publish_stop(ctx,0x8c61afd2u); r[0]=24u;goto L8c61afd4; /* mov #0x18,r0 */
L8c61afd4: sa_motion_publish_stop(ctx,0x8c61afd4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afd6; /* fmov.s fr3,@-r8 */
L8c61afd6: sa_motion_publish_stop(ctx,0x8c61afd6u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61afd8; /* fmov.s @(r0,r14),fr3 */
L8c61afd8: sa_motion_publish_stop(ctx,0x8c61afd8u); r[0]=20u;goto L8c61afda; /* mov #0x14,r0 */
L8c61afda: sa_motion_publish_stop(ctx,0x8c61afdau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afdc; /* fmov.s fr3,@-r8 */
L8c61afdc: sa_motion_publish_stop(ctx,0x8c61afdcu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61afde; /* fmov.s @(r0,r14),fr3 */
L8c61afde: sa_motion_publish_stop(ctx,0x8c61afdeu); r[0]=16u;goto L8c61afe0; /* mov #0x10,r0 */
L8c61afe0: sa_motion_publish_stop(ctx,0x8c61afe0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afe2; /* fmov.s fr3,@-r8 */
L8c61afe2: sa_motion_publish_stop(ctx,0x8c61afe2u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61afe4; /* fmov.s @(r0,r14),fr3 */
L8c61afe4: sa_motion_publish_stop(ctx,0x8c61afe4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61afe6; /* fmov.s fr3,@-r8 */
L8c61afe6: sa_motion_publish_stop(ctx,0x8c61afe6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61afe8; /* mov.l r3,@-r8 */
L8c61afe8: sa_motion_publish_stop(ctx,0x8c61afe8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61afea; /* pref @r8 */
L8c61afea: sa_motion_publish_stop(ctx,0x8c61afeau); r[0]=rd(m,bus,&io_status,0x8c61b1e4u,4);if(io_status)return io_status;goto L8c61afec; /* mov.l 0x8c61b1e4,r0 */
L8c61afec: sa_motion_publish_stop(ctx,0x8c61afecu); r[8]+=32u;goto L8c61afee; /* add #0x20,r8 */
L8c61afee: sa_motion_publish_stop(ctx,0x8c61afeeu); r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61aff0; /* mov.l @r0,r3 */
L8c61aff0: sa_motion_publish_stop(ctx,0x8c61aff0u); r[3]+=3u;goto L8c61aff2; /* add #0x3,r3 */
L8c61aff2: sa_motion_publish_stop(ctx,0x8c61aff2u); wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61aff4; /* mov.l r3,@r0 */
L8c61aff4: sa_motion_publish_stop(ctx,0x8c61aff4u); r[11]+=1u;goto L8c61aff6; /* add #0x1,r11 */
L8c61aff6: sa_motion_publish_stop(ctx,0x8c61aff6u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61aff8; /* mov.l @r15,r2 */
L8c61aff8: sa_motion_publish_stop(ctx,0x8c61aff8u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61affa; /* mov.w @(0x2,r2),r0 */
L8c61affa: sa_motion_publish_stop(ctx,0x8c61affau); r[0]=r[0]&65535u;goto L8c61affc; /* extu.w r0,r0 */
L8c61affc: sa_motion_publish_stop(ctx,0x8c61affcu); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61affe; /* cmp/ge r0,r11 */
L8c61affe: sa_motion_publish_stop(ctx,0x8c61affeu); if((*sr&1u))goto L8c61b004;goto L8c61b000; /* bt 0x8c61b004 */
L8c61b000: sa_motion_publish_stop(ctx,0x8c61b002u); goto L8c61aeac; /* bra 0x8c61aeac */
L8c61b004: sa_motion_publish_stop(ctx,0x8c61b006u); goto L8c61b432; /* bra 0x8c61b432 */
L8c61b008: sa_motion_publish_stop(ctx,0x8c61b008u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b00a; /* mov.l @r15,r1 */
L8c61b00a: sa_motion_publish_stop(ctx,0x8c61b00au); r[2]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61b00c; /* mov.l @r13,r2 */
L8c61b00c: sa_motion_publish_stop(ctx,0x8c61b00cu); r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61b00e; /* mov.w @(0x2,r1),r0 */
L8c61b00e: sa_motion_publish_stop(ctx,0x8c61b00eu); r[0]=r[0]&65535u;goto L8c61b010; /* extu.w r0,r0 */
L8c61b010: sa_motion_publish_stop(ctx,0x8c61b010u); r[0]<<=2;goto L8c61b012; /* shll2 r0 */
L8c61b012: sa_motion_publish_stop(ctx,0x8c61b012u); r[2]+=r[0];goto L8c61b014; /* add r0,r2 */
L8c61b014: sa_motion_publish_stop(ctx,0x8c61b014u); wr(m,bus,&io_status,r[13],r[2],4);if(io_status)return io_status;goto L8c61b016; /* mov.l r2,@r13 */
L8c61b016: sa_motion_publish_stop(ctx,0x8c61b018u); r[9]=r[4];goto L8c61b1a4; /* bra 0x8c61b1a4 */
L8c61b01a: sa_motion_publish_stop(ctx,0x8c61b01au); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b01c; /* mov.w @r10+,r14 */
L8c61b01c: sa_motion_publish_stop(ctx,0x8c61b01cu); r[0]=16u;goto L8c61b01e; /* mov #0x10,r0 */
L8c61b01e: sa_motion_publish_stop(ctx,0x8c61b01eu); r[3]=rd(m,bus,&io_status,0x8c61b1bcu,4);if(io_status)return io_status;goto L8c61b020; /* mov.l 0x8c61b1bc,r3 */
L8c61b020: sa_motion_publish_stop(ctx,0x8c61b020u); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b022; /* mov.w @r10+,r13 */
L8c61b022: sa_motion_publish_stop(ctx,0x8c61b022u); r[14]=r[14]&65535u;goto L8c61b024; /* extu.w r14,r14 */
L8c61b024: sa_motion_publish_stop(ctx,0x8c61b024u); r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61b026; /* mov.l @r3,r4 */
L8c61b026: sa_motion_publish_stop(ctx,0x8c61b026u); r[14]<<=2;goto L8c61b028; /* shll2 r14 */
L8c61b028: sa_motion_publish_stop(ctx,0x8c61b028u); r[13]=r[13]&65535u;goto L8c61b02a; /* extu.w r13,r13 */
L8c61b02a: sa_motion_publish_stop(ctx,0x8c61b02au); r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b02c; /* mov.w @r10+,r12 */
L8c61b02c: sa_motion_publish_stop(ctx,0x8c61b02cu); r[13]<<=2;goto L8c61b02e; /* shll2 r13 */
L8c61b02e: sa_motion_publish_stop(ctx,0x8c61b02eu); r[13]<<=2;goto L8c61b030; /* shll2 r13 */
L8c61b030: sa_motion_publish_stop(ctx,0x8c61b030u); r[11]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b032; /* mov.w @r10+,r11 */
L8c61b032: sa_motion_publish_stop(ctx,0x8c61b032u); r[14]<<=2;goto L8c61b034; /* shll2 r14 */
L8c61b034: sa_motion_publish_stop(ctx,0x8c61b034u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61b036; /* shll r14 */
L8c61b036: sa_motion_publish_stop(ctx,0x8c61b036u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61b038; /* shll r13 */
L8c61b038: sa_motion_publish_stop(ctx,0x8c61b038u); r[13]+=r[4];goto L8c61b03a; /* add r4,r13 */
L8c61b03a: sa_motion_publish_stop(ctx,0x8c61b03au); r[14]+=r[4];goto L8c61b03c; /* add r4,r14 */
L8c61b03c: sa_motion_publish_stop(ctx,0x8c61b03cu); fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b03e; /* fmov.s @(r0,r13),fr4 */
L8c61b03e: sa_motion_publish_stop(ctx,0x8c61b03eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b040; /* fmov.s @(r0,r14),fr3 */
L8c61b040: sa_motion_publish_stop(ctx,0x8c61b040u); r[12]=r[12]&65535u;goto L8c61b042; /* extu.w r12,r12 */
L8c61b042: sa_motion_publish_stop(ctx,0x8c61b042u); fcopy(f,2u,4u);goto L8c61b044; /* fmov fr4,fr2 */
L8c61b044: sa_motion_publish_stop(ctx,0x8c61b044u); r[12]<<=2;goto L8c61b046; /* shll2 r12 */
L8c61b046: sa_motion_publish_stop(ctx,0x8c61b046u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61b048; /* fsub fr3,fr2 */
L8c61b048: sa_motion_publish_stop(ctx,0x8c61b048u); r[11]=r[11]&65535u;goto L8c61b04a; /* extu.w r11,r11 */
L8c61b04a: sa_motion_publish_stop(ctx,0x8c61b04au); r[12]<<=2;goto L8c61b04c; /* shll2 r12 */
L8c61b04c: sa_motion_publish_stop(ctx,0x8c61b04cu); r[11]<<=2;goto L8c61b04e; /* shll2 r11 */
L8c61b04e: sa_motion_publish_stop(ctx,0x8c61b04eu); r[0]=20u;goto L8c61b050; /* mov #0x14,r0 */
L8c61b050: sa_motion_publish_stop(ctx,0x8c61b050u); fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b052; /* fmov.s @(r0,r13),fr5 */
L8c61b052: sa_motion_publish_stop(ctx,0x8c61b052u); r[11]<<=2;goto L8c61b054; /* shll2 r11 */
L8c61b054: sa_motion_publish_stop(ctx,0x8c61b054u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61b056; /* shll r12 */
L8c61b056: sa_motion_publish_stop(ctx,0x8c61b056u); r[12]+=r[4];goto L8c61b058; /* add r4,r12 */
L8c61b058: sa_motion_publish_stop(ctx,0x8c61b058u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61b05a; /* shll r11 */
L8c61b05a: sa_motion_publish_stop(ctx,0x8c61b05au); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b05c; /* fmov.s @(r0,r12),fr3 */
L8c61b05c: sa_motion_publish_stop(ctx,0x8c61b05cu); fcopy(f,1u,5u);goto L8c61b05e; /* fmov fr5,fr1 */
L8c61b05e: sa_motion_publish_stop(ctx,0x8c61b05eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b060; /* fsub fr5,fr3 */
L8c61b060: sa_motion_publish_stop(ctx,0x8c61b060u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b062; /* fmul fr3,fr2 */
L8c61b062: sa_motion_publish_stop(ctx,0x8c61b062u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b064; /* fmov.s @(r0,r14),fr3 */
L8c61b064: sa_motion_publish_stop(ctx,0x8c61b064u); r[0]=16u;goto L8c61b066; /* mov #0x10,r0 */
L8c61b066: sa_motion_publish_stop(ctx,0x8c61b066u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61b068; /* fsub fr3,fr1 */
L8c61b068: sa_motion_publish_stop(ctx,0x8c61b068u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b06a; /* fmov.s @(r0,r12),fr3 */
L8c61b06a: sa_motion_publish_stop(ctx,0x8c61b06au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b06c; /* fsub fr4,fr3 */
L8c61b06c: sa_motion_publish_stop(ctx,0x8c61b06cu); fcopy(f,4u,2u);goto L8c61b06e; /* fmov fr2,fr4 */
L8c61b06e: sa_motion_publish_stop(ctx,0x8c61b06eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b070; /* fmul fr3,fr1 */
L8c61b070: sa_motion_publish_stop(ctx,0x8c61b070u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61b072; /* fldi0 fr3 */
L8c61b072: sa_motion_publish_stop(ctx,0x8c61b072u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b074; /* fsub fr1,fr4 */
L8c61b074: sa_motion_publish_stop(ctx,0x8c61b074u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61b076; /* fcmp/gt fr3,fr4 */
L8c61b076: sa_motion_publish_stop(ctx,0x8c61b078u); take=!(*sr&1u);r[11]+=r[4];if(take)goto L8c61b07e;goto L8c61b07a; /* bf/s 0x8c61b07e */
L8c61b07a: sa_motion_publish_stop(ctx,0x8c61b07cu); goto L8c61b1a2; /* bra 0x8c61b1a2 */
L8c61b07e: sa_motion_publish_stop(ctx,0x8c61b07eu); fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c61b080; /* fmov.s @r14,fr2 */
L8c61b080: sa_motion_publish_stop(ctx,0x8c61b080u); r[0]=20u;goto L8c61b082; /* mov #0x14,r0 */
L8c61b082: sa_motion_publish_stop(ctx,0x8c61b082u); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61b084; /* fmov.s @r13,fr3 */
L8c61b084: sa_motion_publish_stop(ctx,0x8c61b084u); r[4]=r[15];goto L8c61b086; /* mov r15,r4 */
L8c61b086: sa_motion_publish_stop(ctx,0x8c61b086u); r[3]=rd(m,bus,&io_status,0x8c61b1c0u,4);if(io_status)return io_status;goto L8c61b088; /* mov.l 0x8c61b1c0,r3 */
L8c61b088: sa_motion_publish_stop(ctx,0x8c61b088u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b08a; /* fadd fr3,fr2 */
L8c61b08a: sa_motion_publish_stop(ctx,0x8c61b08au); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61b08c; /* fmov.s @r12,fr3 */
L8c61b08c: sa_motion_publish_stop(ctx,0x8c61b08cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b08e; /* fadd fr3,fr2 */
L8c61b08e: sa_motion_publish_stop(ctx,0x8c61b08eu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b090; /* fmov fr2,@(r0,r15) */
L8c61b090: sa_motion_publish_stop(ctx,0x8c61b090u); r[0]=4u;goto L8c61b092; /* mov #0x4,r0 */
L8c61b092: sa_motion_publish_stop(ctx,0x8c61b092u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b094; /* fmov.s @(r0,r14),fr2 */
L8c61b094: sa_motion_publish_stop(ctx,0x8c61b094u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b096; /* fmov.s @(r0,r13),fr3 */
L8c61b096: sa_motion_publish_stop(ctx,0x8c61b096u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b098; /* fadd fr3,fr2 */
L8c61b098: sa_motion_publish_stop(ctx,0x8c61b098u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b09a; /* fmov.s @(r0,r12),fr3 */
L8c61b09a: sa_motion_publish_stop(ctx,0x8c61b09au); r[0]=24u;goto L8c61b09c; /* mov #0x18,r0 */
L8c61b09c: sa_motion_publish_stop(ctx,0x8c61b09cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b09e; /* fadd fr3,fr2 */
L8c61b09e: sa_motion_publish_stop(ctx,0x8c61b09eu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b0a0; /* fmov fr2,@(r0,r15) */
L8c61b0a0: sa_motion_publish_stop(ctx,0x8c61b0a0u); r[0]=8u;goto L8c61b0a2; /* mov #0x8,r0 */
L8c61b0a2: sa_motion_publish_stop(ctx,0x8c61b0a2u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b0a4; /* fmov.s @(r0,r14),fr2 */
L8c61b0a4: sa_motion_publish_stop(ctx,0x8c61b0a4u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b0a6; /* fmov.s @(r0,r13),fr3 */
L8c61b0a6: sa_motion_publish_stop(ctx,0x8c61b0a6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b0a8; /* fadd fr3,fr2 */
L8c61b0a8: sa_motion_publish_stop(ctx,0x8c61b0a8u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b0aa; /* fmov.s @(r0,r12),fr3 */
L8c61b0aa: sa_motion_publish_stop(ctx,0x8c61b0aau); r[0]=28u;goto L8c61b0ac; /* mov #0x1c,r0 */
L8c61b0ac: sa_motion_publish_stop(ctx,0x8c61b0acu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b0ae; /* fadd fr3,fr2 */
L8c61b0ae: sa_motion_publish_stop(ctx,0x8c61b0aeu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b0b0; /* fmov fr2,@(r0,r15) */
L8c61b0b0: sa_motion_publish_stop(ctx,0x8c61b0b2u); target=r[3];*pr=0x8c61b0b4u;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b0b4; /* jsr @r3 */
L8c61b0b4: sa_motion_publish_stop(ctx,0x8c61b0b4u); r[3]=rd(m,bus,&io_status,0x8c61b1c4u,4);if(io_status)return io_status;goto L8c61b0b6; /* mov.l 0x8c61b1c4,r3 */
L8c61b0b6: sa_motion_publish_stop(ctx,0x8c61b0b6u); r[0]=20u;goto L8c61b0b8; /* mov #0x14,r0 */
L8c61b0b8: sa_motion_publish_stop(ctx,0x8c61b0b8u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b0ba; /* fmov.s @(r0,r15),fr3 */
L8c61b0ba: sa_motion_publish_stop(ctx,0x8c61b0bau); r[0]=24u;goto L8c61b0bc; /* mov #0x18,r0 */
L8c61b0bc: sa_motion_publish_stop(ctx,0x8c61b0bcu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61b0be; /* fmov.s @r3,fr2 */
L8c61b0be: sa_motion_publish_stop(ctx,0x8c61b0beu); r[3]+=8u;goto L8c61b0c0; /* add #0x8,r3 */
L8c61b0c0: sa_motion_publish_stop(ctx,0x8c61b0c0u); r[2]=rd(m,bus,&io_status,0x8c61b1c8u,4);if(io_status)return io_status;goto L8c61b0c2; /* mov.l 0x8c61b1c8,r2 */
L8c61b0c2: sa_motion_publish_stop(ctx,0x8c61b0c2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b0c4; /* fmul fr3,fr2 */
L8c61b0c4: sa_motion_publish_stop(ctx,0x8c61b0c4u); r[1]=rd(m,bus,&io_status,0x8c61b1ccu,4);if(io_status)return io_status;goto L8c61b0c6; /* mov.l 0x8c61b1cc,r1 */
L8c61b0c6: sa_motion_publish_stop(ctx,0x8c61b0c6u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b0c8; /* fmov.s @r2,fr3 */
L8c61b0c8: sa_motion_publish_stop(ctx,0x8c61b0c8u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61b0ca; /* fmov.s @r1,fr1 */
L8c61b0ca: sa_motion_publish_stop(ctx,0x8c61b0cau); r[2]=rd(m,bus,&io_status,0x8c61b1d0u,4);if(io_status)return io_status;goto L8c61b0cc; /* mov.l 0x8c61b1d0,r2 */
L8c61b0cc: sa_motion_publish_stop(ctx,0x8c61b0ccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b0ce; /* fsub fr2,fr3 */
L8c61b0ce: sa_motion_publish_stop(ctx,0x8c61b0ceu); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b0d0; /* fmov.s @(r0,r15),fr2 */
L8c61b0d0: sa_motion_publish_stop(ctx,0x8c61b0d0u); r[0]=28u;goto L8c61b0d2; /* mov #0x1c,r0 */
L8c61b0d2: sa_motion_publish_stop(ctx,0x8c61b0d2u); r[1]=rd(m,bus,&io_status,0x8c61b1d8u,4);if(io_status)return io_status;goto L8c61b0d4; /* mov.l 0x8c61b1d8,r1 */
L8c61b0d4: sa_motion_publish_stop(ctx,0x8c61b0d4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b0d6; /* fmul fr2,fr1 */
L8c61b0d6: sa_motion_publish_stop(ctx,0x8c61b0d6u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b0d8; /* fmov.s @(r0,r15),fr2 */
L8c61b0d8: sa_motion_publish_stop(ctx,0x8c61b0d8u); r[0]=4u;goto L8c61b0da; /* mov #0x4,r0 */
L8c61b0da: sa_motion_publish_stop(ctx,0x8c61b0dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b0dc; /* fsub fr1,fr3 */
L8c61b0dc: sa_motion_publish_stop(ctx,0x8c61b0dcu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61b0de; /* fmov.s @r3,fr1 */
L8c61b0de: sa_motion_publish_stop(ctx,0x8c61b0deu); r[3]=rd(m,bus,&io_status,0x8c61b1d4u,4);if(io_status)return io_status;goto L8c61b0e0; /* mov.l 0x8c61b1d4,r3 */
L8c61b0e0: sa_motion_publish_stop(ctx,0x8c61b0e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b0e2; /* fmul fr2,fr1 */
L8c61b0e2: sa_motion_publish_stop(ctx,0x8c61b0e2u); fcopy(f,4u,3u);goto L8c61b0e4; /* fmov fr3,fr4 */
L8c61b0e4: sa_motion_publish_stop(ctx,0x8c61b0e4u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b0e6; /* fmov.s @r2,fr3 */
L8c61b0e6: sa_motion_publish_stop(ctx,0x8c61b0e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b0e8; /* fsub fr1,fr4 */
L8c61b0e8: sa_motion_publish_stop(ctx,0x8c61b0e8u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b0ea; /* fmov fr3,@(r0,r15) */
L8c61b0ea: sa_motion_publish_stop(ctx,0x8c61b0eau); r[0]=8u;goto L8c61b0ec; /* mov #0x8,r0 */
L8c61b0ec: sa_motion_publish_stop(ctx,0x8c61b0ecu); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61b0ee; /* fmov.s @r3,fr3 */
L8c61b0ee: sa_motion_publish_stop(ctx,0x8c61b0eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b0f0; /* fmul fr4,fr3 */
L8c61b0f0: sa_motion_publish_stop(ctx,0x8c61b0f0u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b0f2; /* fmov fr3,@(r0,r15) */
L8c61b0f2: sa_motion_publish_stop(ctx,0x8c61b0f2u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61b0f4; /* fmov.s @r1,fr3 */
L8c61b0f4: sa_motion_publish_stop(ctx,0x8c61b0f4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b0f6; /* fmul fr4,fr3 */
L8c61b0f6: sa_motion_publish_stop(ctx,0x8c61b0f6u); r[0]=12u;goto L8c61b0f8; /* mov #0xc,r0 */
L8c61b0f8: sa_motion_publish_stop(ctx,0x8c61b0f8u); r[2]+=12u;goto L8c61b0fa; /* add #0xc,r2 */
L8c61b0fa: sa_motion_publish_stop(ctx,0x8c61b0fau); r[3]=rd(m,bus,&io_status,0x8c61b1dcu,4);if(io_status)return io_status;goto L8c61b0fc; /* mov.l 0x8c61b1dc,r3 */
L8c61b0fc: sa_motion_publish_stop(ctx,0x8c61b0fcu); r[8]+=16u;goto L8c61b0fe; /* add #0x10,r8 */
L8c61b0fe: sa_motion_publish_stop(ctx,0x8c61b0feu); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b100; /* fmov fr3,@(r0,r15) */
L8c61b100: sa_motion_publish_stop(ctx,0x8c61b100u); r[0]=16u;goto L8c61b102; /* mov #0x10,r0 */
L8c61b102: sa_motion_publish_stop(ctx,0x8c61b102u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b104; /* fmov.s @r2,fr3 */
L8c61b104: sa_motion_publish_stop(ctx,0x8c61b104u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b106; /* fmul fr4,fr3 */
L8c61b106: sa_motion_publish_stop(ctx,0x8c61b106u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b108; /* fmov fr3,@(r0,r15) */
L8c61b108: sa_motion_publish_stop(ctx,0x8c61b108u); r[0]=24u;goto L8c61b10a; /* mov #0x18,r0 */
L8c61b10a: sa_motion_publish_stop(ctx,0x8c61b10au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b10c; /* fmov.s @(r0,r14),fr3 */
L8c61b10c: sa_motion_publish_stop(ctx,0x8c61b10cu); r[0]=20u;goto L8c61b10e; /* mov #0x14,r0 */
L8c61b10e: sa_motion_publish_stop(ctx,0x8c61b10eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b110; /* fmov.s fr3,@-r8 */
L8c61b110: sa_motion_publish_stop(ctx,0x8c61b110u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b112; /* fmov.s @(r0,r14),fr3 */
L8c61b112: sa_motion_publish_stop(ctx,0x8c61b112u); r[0]=16u;goto L8c61b114; /* mov #0x10,r0 */
L8c61b114: sa_motion_publish_stop(ctx,0x8c61b114u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b116; /* fmov.s fr3,@-r8 */
L8c61b116: sa_motion_publish_stop(ctx,0x8c61b116u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b118; /* fmov.s @(r0,r14),fr3 */
L8c61b118: sa_motion_publish_stop(ctx,0x8c61b118u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b11a; /* fmov.s fr3,@-r8 */
L8c61b11a: sa_motion_publish_stop(ctx,0x8c61b11au); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b11c; /* mov.l r3,@-r8 */
L8c61b11c: sa_motion_publish_stop(ctx,0x8c61b11cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b11e; /* pref @r8 */
L8c61b11e: sa_motion_publish_stop(ctx,0x8c61b11eu); r[0]=24u;goto L8c61b120; /* mov #0x18,r0 */
L8c61b120: sa_motion_publish_stop(ctx,0x8c61b120u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b122; /* fmov.s @(r0,r13),fr3 */
L8c61b122: sa_motion_publish_stop(ctx,0x8c61b122u); r[8]+=48u;goto L8c61b124; /* add #0x30,r8 */
L8c61b124: sa_motion_publish_stop(ctx,0x8c61b124u); r[0]=20u;goto L8c61b126; /* mov #0x14,r0 */
L8c61b126: sa_motion_publish_stop(ctx,0x8c61b126u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b128; /* fmov.s fr3,@-r8 */
L8c61b128: sa_motion_publish_stop(ctx,0x8c61b128u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b12a; /* fmov.s @(r0,r13),fr3 */
L8c61b12a: sa_motion_publish_stop(ctx,0x8c61b12au); r[0]=16u;goto L8c61b12c; /* mov #0x10,r0 */
L8c61b12c: sa_motion_publish_stop(ctx,0x8c61b12cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b12e; /* fmov.s fr3,@-r8 */
L8c61b12e: sa_motion_publish_stop(ctx,0x8c61b12eu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b130; /* fmov.s @(r0,r13),fr3 */
L8c61b130: sa_motion_publish_stop(ctx,0x8c61b130u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b132; /* fmov.s fr3,@-r8 */
L8c61b132: sa_motion_publish_stop(ctx,0x8c61b132u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b134; /* mov.l r3,@-r8 */
L8c61b134: sa_motion_publish_stop(ctx,0x8c61b134u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b136; /* pref @r8 */
L8c61b136: sa_motion_publish_stop(ctx,0x8c61b136u); r[0]=16u;goto L8c61b138; /* mov #0x10,r0 */
L8c61b138: sa_motion_publish_stop(ctx,0x8c61b138u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b13a; /* fmov.s @(r0,r15),fr3 */
L8c61b13a: sa_motion_publish_stop(ctx,0x8c61b13au); r[8]+=64u;goto L8c61b13c; /* add #0x40,r8 */
L8c61b13c: sa_motion_publish_stop(ctx,0x8c61b13cu); r[0]=12u;goto L8c61b13e; /* mov #0xc,r0 */
L8c61b13e: sa_motion_publish_stop(ctx,0x8c61b13eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b140; /* fmov.s fr3,@-r8 */
L8c61b140: sa_motion_publish_stop(ctx,0x8c61b140u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b142; /* fmov.s @(r0,r15),fr3 */
L8c61b142: sa_motion_publish_stop(ctx,0x8c61b142u); r[0]=8u;goto L8c61b144; /* mov #0x8,r0 */
L8c61b144: sa_motion_publish_stop(ctx,0x8c61b144u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b146; /* fmov.s fr3,@-r8 */
L8c61b146: sa_motion_publish_stop(ctx,0x8c61b146u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b148; /* fmov.s @(r0,r15),fr3 */
L8c61b148: sa_motion_publish_stop(ctx,0x8c61b148u); r[0]=4u;goto L8c61b14a; /* mov #0x4,r0 */
L8c61b14a: sa_motion_publish_stop(ctx,0x8c61b14au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b14c; /* fmov.s fr3,@-r8 */
L8c61b14c: sa_motion_publish_stop(ctx,0x8c61b14cu); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b14e; /* fmov.s @(r0,r15),fr3 */
L8c61b14e: sa_motion_publish_stop(ctx,0x8c61b14eu); r[0]=24u;goto L8c61b150; /* mov #0x18,r0 */
L8c61b150: sa_motion_publish_stop(ctx,0x8c61b150u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b152; /* fmov.s fr3,@-r8 */
L8c61b152: sa_motion_publish_stop(ctx,0x8c61b152u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b154; /* fmov.s @(r0,r12),fr3 */
L8c61b154: sa_motion_publish_stop(ctx,0x8c61b154u); r[0]=20u;goto L8c61b156; /* mov #0x14,r0 */
L8c61b156: sa_motion_publish_stop(ctx,0x8c61b156u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b158; /* fmov.s fr3,@-r8 */
L8c61b158: sa_motion_publish_stop(ctx,0x8c61b158u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b15a; /* fmov.s @(r0,r12),fr3 */
L8c61b15a: sa_motion_publish_stop(ctx,0x8c61b15au); r[0]=16u;goto L8c61b15c; /* mov #0x10,r0 */
L8c61b15c: sa_motion_publish_stop(ctx,0x8c61b15cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b15e; /* fmov.s fr3,@-r8 */
L8c61b15e: sa_motion_publish_stop(ctx,0x8c61b15eu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b160; /* fmov.s @(r0,r12),fr3 */
L8c61b160: sa_motion_publish_stop(ctx,0x8c61b160u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b162; /* fmov.s fr3,@-r8 */
L8c61b162: sa_motion_publish_stop(ctx,0x8c61b162u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b164; /* mov.l r3,@-r8 */
L8c61b164: sa_motion_publish_stop(ctx,0x8c61b164u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b166; /* pref @r8 */
L8c61b166: sa_motion_publish_stop(ctx,0x8c61b166u); r[0]=16u;goto L8c61b168; /* mov #0x10,r0 */
L8c61b168: sa_motion_publish_stop(ctx,0x8c61b168u); r[3]=rd(m,bus,&io_status,0x8c61b1e0u,4);if(io_status)return io_status;goto L8c61b16a; /* mov.l 0x8c61b1e0,r3 */
L8c61b16a: sa_motion_publish_stop(ctx,0x8c61b16au); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b16c; /* fmov.s @(r0,r15),fr3 */
L8c61b16c: sa_motion_publish_stop(ctx,0x8c61b16cu); r[8]+=64u;goto L8c61b16e; /* add #0x40,r8 */
L8c61b16e: sa_motion_publish_stop(ctx,0x8c61b16eu); r[0]=12u;goto L8c61b170; /* mov #0xc,r0 */
L8c61b170: sa_motion_publish_stop(ctx,0x8c61b170u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b172; /* fmov.s fr3,@-r8 */
L8c61b172: sa_motion_publish_stop(ctx,0x8c61b172u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b174; /* fmov.s @(r0,r15),fr3 */
L8c61b174: sa_motion_publish_stop(ctx,0x8c61b174u); r[0]=8u;goto L8c61b176; /* mov #0x8,r0 */
L8c61b176: sa_motion_publish_stop(ctx,0x8c61b176u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b178; /* fmov.s fr3,@-r8 */
L8c61b178: sa_motion_publish_stop(ctx,0x8c61b178u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b17a; /* fmov.s @(r0,r15),fr3 */
L8c61b17a: sa_motion_publish_stop(ctx,0x8c61b17au); r[0]=4u;goto L8c61b17c; /* mov #0x4,r0 */
L8c61b17c: sa_motion_publish_stop(ctx,0x8c61b17cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b17e; /* fmov.s fr3,@-r8 */
L8c61b17e: sa_motion_publish_stop(ctx,0x8c61b17eu); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b180; /* fmov.s @(r0,r15),fr3 */
L8c61b180: sa_motion_publish_stop(ctx,0x8c61b180u); r[0]=24u;goto L8c61b182; /* mov #0x18,r0 */
L8c61b182: sa_motion_publish_stop(ctx,0x8c61b182u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b184; /* fmov.s fr3,@-r8 */
L8c61b184: sa_motion_publish_stop(ctx,0x8c61b184u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b186; /* fmov.s @(r0,r11),fr3 */
L8c61b186: sa_motion_publish_stop(ctx,0x8c61b186u); r[0]=20u;goto L8c61b188; /* mov #0x14,r0 */
L8c61b188: sa_motion_publish_stop(ctx,0x8c61b188u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b18a; /* fmov.s fr3,@-r8 */
L8c61b18a: sa_motion_publish_stop(ctx,0x8c61b18au); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b18c; /* fmov.s @(r0,r11),fr3 */
L8c61b18c: sa_motion_publish_stop(ctx,0x8c61b18cu); r[0]=16u;goto L8c61b18e; /* mov #0x10,r0 */
L8c61b18e: sa_motion_publish_stop(ctx,0x8c61b18eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b190; /* fmov.s fr3,@-r8 */
L8c61b190: sa_motion_publish_stop(ctx,0x8c61b190u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b192; /* fmov.s @(r0,r11),fr3 */
L8c61b192: sa_motion_publish_stop(ctx,0x8c61b192u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b194; /* fmov.s fr3,@-r8 */
L8c61b194: sa_motion_publish_stop(ctx,0x8c61b194u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b196; /* mov.l r3,@-r8 */
L8c61b196: sa_motion_publish_stop(ctx,0x8c61b196u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b198; /* pref @r8 */
L8c61b198: sa_motion_publish_stop(ctx,0x8c61b198u); r[0]=rd(m,bus,&io_status,0x8c61b1e4u,4);if(io_status)return io_status;goto L8c61b19a; /* mov.l 0x8c61b1e4,r0 */
L8c61b19a: sa_motion_publish_stop(ctx,0x8c61b19au); r[8]+=32u;goto L8c61b19c; /* add #0x20,r8 */
L8c61b19c: sa_motion_publish_stop(ctx,0x8c61b19cu); r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61b19e; /* mov.l @r0,r3 */
L8c61b19e: sa_motion_publish_stop(ctx,0x8c61b19eu); r[3]+=4u;goto L8c61b1a0; /* add #0x4,r3 */
L8c61b1a0: sa_motion_publish_stop(ctx,0x8c61b1a0u); wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61b1a2; /* mov.l r3,@r0 */
L8c61b1a2: sa_motion_publish_stop(ctx,0x8c61b1a2u); r[9]+=1u;goto L8c61b1a4; /* add #0x1,r9 */
L8c61b1a4: sa_motion_publish_stop(ctx,0x8c61b1a4u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b1a6; /* mov.l @r15,r2 */
L8c61b1a6: sa_motion_publish_stop(ctx,0x8c61b1a6u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61b1a8; /* mov.w @(0x2,r2),r0 */
L8c61b1a8: sa_motion_publish_stop(ctx,0x8c61b1a8u); r[0]=r[0]&65535u;goto L8c61b1aa; /* extu.w r0,r0 */
L8c61b1aa: sa_motion_publish_stop(ctx,0x8c61b1aau); *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61b1ac; /* cmp/ge r0,r9 */
L8c61b1ac: sa_motion_publish_stop(ctx,0x8c61b1acu); if((*sr&1u))goto L8c61b1b2;goto L8c61b1ae; /* bt 0x8c61b1b2 */
L8c61b1ae: sa_motion_publish_stop(ctx,0x8c61b1b0u); goto L8c61b01a; /* bra 0x8c61b01a */
L8c61b1b2: sa_motion_publish_stop(ctx,0x8c61b1b4u); goto L8c61b432; /* bra 0x8c61b432 */
L8c61b1b6: sa_motion_publish_stop(ctx,0x8c61b1b6u); r[11]=2u;goto L8c61b1b8; /* mov #0x2,r11 */
L8c61b1b8: sa_motion_publish_stop(ctx,0x8c61b1bau); r[12]=r[4];goto L8c61b3ca; /* bra 0x8c61b3ca */
L8c61b1e8: sa_motion_publish_stop(ctx,0x8c61b1e8u); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b1ea; /* mov.w @r10+,r14 */
L8c61b1ea: sa_motion_publish_stop(ctx,0x8c61b1eau); r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61b1ec; /* mov.l @r13,r3 */
L8c61b1ec: sa_motion_publish_stop(ctx,0x8c61b1ecu); r[14]=r[14]&65535u;goto L8c61b1ee; /* extu.w r14,r14 */
L8c61b1ee: sa_motion_publish_stop(ctx,0x8c61b1eeu); r[3]+=r[14];goto L8c61b1f0; /* add r14,r3 */
L8c61b1f0: sa_motion_publish_stop(ctx,0x8c61b1f0u); wr(m,bus,&io_status,r[13],r[3],4);if(io_status)return io_status;goto L8c61b1f2; /* mov.l r3,@r13 */
L8c61b1f2: sa_motion_publish_stop(ctx,0x8c61b1f2u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61b1f4; /* mov.w @(0x2,r10),r0 */
L8c61b1f4: sa_motion_publish_stop(ctx,0x8c61b1f4u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61b1f6; /* mov.w @r10,r4 */
L8c61b1f6: sa_motion_publish_stop(ctx,0x8c61b1f6u); r[3]=rd(m,bus,&io_status,0x8c61b454u,4);if(io_status)return io_status;goto L8c61b1f8; /* mov.l 0x8c61b454,r3 */
L8c61b1f8: sa_motion_publish_stop(ctx,0x8c61b1f8u); r[6]=r[0]&65535u;goto L8c61b1fa; /* extu.w r0,r6 */
L8c61b1fa: sa_motion_publish_stop(ctx,0x8c61b1fau); r[0]=sx(m,bus,&io_status,r[10]+0x4u,2);if(io_status)return io_status;goto L8c61b1fc; /* mov.w @(0x4,r10),r0 */
L8c61b1fc: sa_motion_publish_stop(ctx,0x8c61b1fcu); r[6]<<=2;goto L8c61b1fe; /* shll2 r6 */
L8c61b1fe: sa_motion_publish_stop(ctx,0x8c61b1feu); r[4]=r[4]&65535u;goto L8c61b200; /* extu.w r4,r4 */
L8c61b200: sa_motion_publish_stop(ctx,0x8c61b200u); r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61b202; /* mov.l @r3,r7 */
L8c61b202: sa_motion_publish_stop(ctx,0x8c61b202u); r[6]<<=2;goto L8c61b204; /* shll2 r6 */
L8c61b204: sa_motion_publish_stop(ctx,0x8c61b204u); r[4]<<=2;goto L8c61b206; /* shll2 r4 */
L8c61b206: sa_motion_publish_stop(ctx,0x8c61b206u); r[5]=r[0]&65535u;goto L8c61b208; /* extu.w r0,r5 */
L8c61b208: sa_motion_publish_stop(ctx,0x8c61b208u); r[4]<<=2;goto L8c61b20a; /* shll2 r4 */
L8c61b20a: sa_motion_publish_stop(ctx,0x8c61b20au); r[0]=16u;goto L8c61b20c; /* mov #0x10,r0 */
L8c61b20c: sa_motion_publish_stop(ctx,0x8c61b20cu); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61b20e; /* shll r4 */
L8c61b20e: sa_motion_publish_stop(ctx,0x8c61b20eu); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61b210; /* shll r6 */
L8c61b210: sa_motion_publish_stop(ctx,0x8c61b210u); r[6]+=r[7];goto L8c61b212; /* add r7,r6 */
L8c61b212: sa_motion_publish_stop(ctx,0x8c61b212u); fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c61b214; /* fmov.s @(r0,r6),fr5 */
L8c61b214: sa_motion_publish_stop(ctx,0x8c61b214u); r[4]+=r[7];goto L8c61b216; /* add r7,r4 */
L8c61b216: sa_motion_publish_stop(ctx,0x8c61b216u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b218; /* fmov.s @(r0,r4),fr3 */
L8c61b218: sa_motion_publish_stop(ctx,0x8c61b218u); r[5]<<=2;goto L8c61b21a; /* shll2 r5 */
L8c61b21a: sa_motion_publish_stop(ctx,0x8c61b21au); fcopy(f,2u,5u);goto L8c61b21c; /* fmov fr5,fr2 */
L8c61b21c: sa_motion_publish_stop(ctx,0x8c61b21cu); r[0]=20u;goto L8c61b21e; /* mov #0x14,r0 */
L8c61b21e: sa_motion_publish_stop(ctx,0x8c61b21eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61b220; /* fsub fr3,fr2 */
L8c61b220: sa_motion_publish_stop(ctx,0x8c61b220u); r[5]<<=2;goto L8c61b222; /* shll2 r5 */
L8c61b222: sa_motion_publish_stop(ctx,0x8c61b222u); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c61b224; /* fmov.s @(r0,r6),fr4 */
L8c61b224: sa_motion_publish_stop(ctx,0x8c61b224u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61b226; /* shll r5 */
L8c61b226: sa_motion_publish_stop(ctx,0x8c61b226u); r[5]+=r[7];goto L8c61b228; /* add r7,r5 */
L8c61b228: sa_motion_publish_stop(ctx,0x8c61b228u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61b22a; /* fmov.s @(r0,r5),fr3 */
L8c61b22a: sa_motion_publish_stop(ctx,0x8c61b22au); fcopy(f,1u,4u);goto L8c61b22c; /* fmov fr4,fr1 */
L8c61b22c: sa_motion_publish_stop(ctx,0x8c61b22cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b22e; /* fsub fr4,fr3 */
L8c61b22e: sa_motion_publish_stop(ctx,0x8c61b22eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b230; /* fmul fr3,fr2 */
L8c61b230: sa_motion_publish_stop(ctx,0x8c61b230u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b232; /* fmov.s @(r0,r4),fr3 */
L8c61b232: sa_motion_publish_stop(ctx,0x8c61b232u); r[0]=16u;goto L8c61b234; /* mov #0x10,r0 */
L8c61b234: sa_motion_publish_stop(ctx,0x8c61b234u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61b236; /* fsub fr3,fr1 */
L8c61b236: sa_motion_publish_stop(ctx,0x8c61b236u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61b238; /* fmov.s @(r0,r5),fr3 */
L8c61b238: sa_motion_publish_stop(ctx,0x8c61b238u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b23a; /* fsub fr5,fr3 */
L8c61b23a: sa_motion_publish_stop(ctx,0x8c61b23au); fcopy(f,4u,2u);goto L8c61b23c; /* fmov fr2,fr4 */
L8c61b23c: sa_motion_publish_stop(ctx,0x8c61b23cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b23e; /* fmul fr3,fr1 */
L8c61b23e: sa_motion_publish_stop(ctx,0x8c61b23eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61b240; /* fldi0 fr3 */
L8c61b240: sa_motion_publish_stop(ctx,0x8c61b240u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b242; /* fsub fr1,fr4 */
L8c61b242: sa_motion_publish_stop(ctx,0x8c61b242u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61b244; /* fcmp/gt fr3,fr4 */
L8c61b244: sa_motion_publish_stop(ctx,0x8c61b244u); if(!(*sr&1u))goto L8c61b24c;goto L8c61b246; /* bf 0x8c61b24c */
L8c61b246: sa_motion_publish_stop(ctx,0x8c61b246u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61b248; /* shll r14 */
L8c61b248: sa_motion_publish_stop(ctx,0x8c61b24au); r[10]+=r[14];goto L8c61b3c8; /* bra 0x8c61b3c8 */
L8c61b24c: sa_motion_publish_stop(ctx,0x8c61b24cu); r[0]=20u;goto L8c61b24e; /* mov #0x14,r0 */
L8c61b24e: sa_motion_publish_stop(ctx,0x8c61b24eu); r[3]=rd(m,bus,&io_status,0x8c61b458u,4);if(io_status)return io_status;goto L8c61b250; /* mov.l 0x8c61b458,r3 */
L8c61b250: sa_motion_publish_stop(ctx,0x8c61b250u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61b252; /* mov.l @r3,r2 */
L8c61b252: sa_motion_publish_stop(ctx,0x8c61b252u); r[2]+=r[14];goto L8c61b254; /* add r14,r2 */
L8c61b254: sa_motion_publish_stop(ctx,0x8c61b254u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61b256; /* mov.l r2,@r3 */
L8c61b256: sa_motion_publish_stop(ctx,0x8c61b256u); fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c61b258; /* fmov.s @r6,fr3 */
L8c61b258: sa_motion_publish_stop(ctx,0x8c61b258u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c61b25a; /* fmov.s @r4,fr2 */
L8c61b25a: sa_motion_publish_stop(ctx,0x8c61b25au); r[3]=rd(m,bus,&io_status,0x8c61b45cu,4);if(io_status)return io_status;goto L8c61b25c; /* mov.l 0x8c61b45c,r3 */
L8c61b25c: sa_motion_publish_stop(ctx,0x8c61b25cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b25e; /* fadd fr3,fr2 */
L8c61b25e: sa_motion_publish_stop(ctx,0x8c61b25eu); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61b260; /* fmov.s @r5,fr3 */
L8c61b260: sa_motion_publish_stop(ctx,0x8c61b260u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b262; /* fadd fr3,fr2 */
L8c61b262: sa_motion_publish_stop(ctx,0x8c61b262u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b264; /* fmov fr2,@(r0,r15) */
L8c61b264: sa_motion_publish_stop(ctx,0x8c61b264u); r[0]=4u;goto L8c61b266; /* mov #0x4,r0 */
L8c61b266: sa_motion_publish_stop(ctx,0x8c61b266u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b268; /* fmov.s @(r0,r4),fr2 */
L8c61b268: sa_motion_publish_stop(ctx,0x8c61b268u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61b26a; /* fmov.s @(r0,r6),fr3 */
L8c61b26a: sa_motion_publish_stop(ctx,0x8c61b26au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b26c; /* fadd fr3,fr2 */
L8c61b26c: sa_motion_publish_stop(ctx,0x8c61b26cu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61b26e; /* fmov.s @(r0,r5),fr3 */
L8c61b26e: sa_motion_publish_stop(ctx,0x8c61b26eu); r[0]=24u;goto L8c61b270; /* mov #0x18,r0 */
L8c61b270: sa_motion_publish_stop(ctx,0x8c61b270u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b272; /* fadd fr3,fr2 */
L8c61b272: sa_motion_publish_stop(ctx,0x8c61b272u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b274; /* fmov fr2,@(r0,r15) */
L8c61b274: sa_motion_publish_stop(ctx,0x8c61b274u); r[0]=8u;goto L8c61b276; /* mov #0x8,r0 */
L8c61b276: sa_motion_publish_stop(ctx,0x8c61b276u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61b278; /* fmov.s @(r0,r6),fr3 */
L8c61b278: sa_motion_publish_stop(ctx,0x8c61b278u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b27a; /* fmov.s @(r0,r4),fr2 */
L8c61b27a: sa_motion_publish_stop(ctx,0x8c61b27au); r[4]=r[15];goto L8c61b27c; /* mov r15,r4 */
L8c61b27c: sa_motion_publish_stop(ctx,0x8c61b27cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b27e; /* fadd fr3,fr2 */
L8c61b27e: sa_motion_publish_stop(ctx,0x8c61b27eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61b280; /* fmov.s @(r0,r5),fr3 */
L8c61b280: sa_motion_publish_stop(ctx,0x8c61b280u); r[0]=28u;goto L8c61b282; /* mov #0x1c,r0 */
L8c61b282: sa_motion_publish_stop(ctx,0x8c61b282u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b284; /* fadd fr3,fr2 */
L8c61b284: sa_motion_publish_stop(ctx,0x8c61b284u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b286; /* fmov fr2,@(r0,r15) */
L8c61b286: sa_motion_publish_stop(ctx,0x8c61b288u); target=r[3];*pr=0x8c61b28au;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b28a; /* jsr @r3 */
L8c61b28a: sa_motion_publish_stop(ctx,0x8c61b28au); r[3]=rd(m,bus,&io_status,0x8c61b460u,4);if(io_status)return io_status;goto L8c61b28c; /* mov.l 0x8c61b460,r3 */
L8c61b28c: sa_motion_publish_stop(ctx,0x8c61b28cu); r[0]=20u;goto L8c61b28e; /* mov #0x14,r0 */
L8c61b28e: sa_motion_publish_stop(ctx,0x8c61b28eu); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b290; /* fmov.s @(r0,r15),fr3 */
L8c61b290: sa_motion_publish_stop(ctx,0x8c61b290u); r[0]=24u;goto L8c61b292; /* mov #0x18,r0 */
L8c61b292: sa_motion_publish_stop(ctx,0x8c61b292u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61b294; /* fmov.s @r3,fr2 */
L8c61b294: sa_motion_publish_stop(ctx,0x8c61b294u); r[3]+=8u;goto L8c61b296; /* add #0x8,r3 */
L8c61b296: sa_motion_publish_stop(ctx,0x8c61b296u); r[2]=rd(m,bus,&io_status,0x8c61b464u,4);if(io_status)return io_status;goto L8c61b298; /* mov.l 0x8c61b464,r2 */
L8c61b298: sa_motion_publish_stop(ctx,0x8c61b298u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b29a; /* fmul fr3,fr2 */
L8c61b29a: sa_motion_publish_stop(ctx,0x8c61b29au); r[1]=rd(m,bus,&io_status,0x8c61b468u,4);if(io_status)return io_status;goto L8c61b29c; /* mov.l 0x8c61b468,r1 */
L8c61b29c: sa_motion_publish_stop(ctx,0x8c61b29cu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b29e; /* fmov.s @r2,fr3 */
L8c61b29e: sa_motion_publish_stop(ctx,0x8c61b29eu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61b2a0; /* fmov.s @r1,fr1 */
L8c61b2a0: sa_motion_publish_stop(ctx,0x8c61b2a0u); r[2]=rd(m,bus,&io_status,0x8c61b46cu,4);if(io_status)return io_status;goto L8c61b2a2; /* mov.l 0x8c61b46c,r2 */
L8c61b2a2: sa_motion_publish_stop(ctx,0x8c61b2a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b2a4; /* fsub fr2,fr3 */
L8c61b2a4: sa_motion_publish_stop(ctx,0x8c61b2a4u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b2a6; /* fmov.s @(r0,r15),fr2 */
L8c61b2a6: sa_motion_publish_stop(ctx,0x8c61b2a6u); r[0]=28u;goto L8c61b2a8; /* mov #0x1c,r0 */
L8c61b2a8: sa_motion_publish_stop(ctx,0x8c61b2a8u); r[1]=rd(m,bus,&io_status,0x8c61b474u,4);if(io_status)return io_status;goto L8c61b2aa; /* mov.l 0x8c61b474,r1 */
L8c61b2aa: sa_motion_publish_stop(ctx,0x8c61b2aau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b2ac; /* fmul fr2,fr1 */
L8c61b2ac: sa_motion_publish_stop(ctx,0x8c61b2acu); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b2ae; /* fmov.s @(r0,r15),fr2 */
L8c61b2ae: sa_motion_publish_stop(ctx,0x8c61b2aeu); r[0]=4u;goto L8c61b2b0; /* mov #0x4,r0 */
L8c61b2b0: sa_motion_publish_stop(ctx,0x8c61b2b0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b2b2; /* fsub fr1,fr3 */
L8c61b2b2: sa_motion_publish_stop(ctx,0x8c61b2b2u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61b2b4; /* fmov.s @r3,fr1 */
L8c61b2b4: sa_motion_publish_stop(ctx,0x8c61b2b4u); r[3]=rd(m,bus,&io_status,0x8c61b470u,4);if(io_status)return io_status;goto L8c61b2b6; /* mov.l 0x8c61b470,r3 */
L8c61b2b6: sa_motion_publish_stop(ctx,0x8c61b2b6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b2b8; /* fmul fr2,fr1 */
L8c61b2b8: sa_motion_publish_stop(ctx,0x8c61b2b8u); fcopy(f,4u,3u);goto L8c61b2ba; /* fmov fr3,fr4 */
L8c61b2ba: sa_motion_publish_stop(ctx,0x8c61b2bau); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b2bc; /* fmov.s @r2,fr3 */
L8c61b2bc: sa_motion_publish_stop(ctx,0x8c61b2bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b2be; /* fsub fr1,fr4 */
L8c61b2be: sa_motion_publish_stop(ctx,0x8c61b2beu); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b2c0; /* fmov fr3,@(r0,r15) */
L8c61b2c0: sa_motion_publish_stop(ctx,0x8c61b2c0u); r[0]=8u;goto L8c61b2c2; /* mov #0x8,r0 */
L8c61b2c2: sa_motion_publish_stop(ctx,0x8c61b2c2u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61b2c4; /* fmov.s @r3,fr3 */
L8c61b2c4: sa_motion_publish_stop(ctx,0x8c61b2c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b2c6; /* fmul fr4,fr3 */
L8c61b2c6: sa_motion_publish_stop(ctx,0x8c61b2c6u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b2c8; /* fmov fr3,@(r0,r15) */
L8c61b2c8: sa_motion_publish_stop(ctx,0x8c61b2c8u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61b2ca; /* fmov.s @r1,fr3 */
L8c61b2ca: sa_motion_publish_stop(ctx,0x8c61b2cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b2cc; /* fmul fr4,fr3 */
L8c61b2cc: sa_motion_publish_stop(ctx,0x8c61b2ccu); r[0]=12u;goto L8c61b2ce; /* mov #0xc,r0 */
L8c61b2ce: sa_motion_publish_stop(ctx,0x8c61b2ceu); r[2]+=12u;goto L8c61b2d0; /* add #0xc,r2 */
L8c61b2d0: sa_motion_publish_stop(ctx,0x8c61b2d0u); r[8]+=16u;goto L8c61b2d2; /* add #0x10,r8 */
L8c61b2d2: sa_motion_publish_stop(ctx,0x8c61b2d2u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b2d4; /* fmov fr3,@(r0,r15) */
L8c61b2d4: sa_motion_publish_stop(ctx,0x8c61b2d4u); r[0]=16u;goto L8c61b2d6; /* mov #0x10,r0 */
L8c61b2d6: sa_motion_publish_stop(ctx,0x8c61b2d6u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b2d8; /* fmov.s @r2,fr3 */
L8c61b2d8: sa_motion_publish_stop(ctx,0x8c61b2d8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b2da; /* fmul fr4,fr3 */
L8c61b2da: sa_motion_publish_stop(ctx,0x8c61b2dau); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b2dc; /* fmov fr3,@(r0,r15) */
L8c61b2dc: sa_motion_publish_stop(ctx,0x8c61b2dcu); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b2de; /* mov.w @r10+,r4 */
L8c61b2de: sa_motion_publish_stop(ctx,0x8c61b2deu); r[0]=rd(m,bus,&io_status,0x8c61b454u,4);if(io_status)return io_status;goto L8c61b2e0; /* mov.l 0x8c61b454,r0 */
L8c61b2e0: sa_motion_publish_stop(ctx,0x8c61b2e0u); r[4]=r[4]&65535u;goto L8c61b2e2; /* extu.w r4,r4 */
L8c61b2e2: sa_motion_publish_stop(ctx,0x8c61b2e2u); r[4]<<=2;goto L8c61b2e4; /* shll2 r4 */
L8c61b2e4: sa_motion_publish_stop(ctx,0x8c61b2e4u); r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61b2e6; /* mov.l @r0,r3 */
L8c61b2e6: sa_motion_publish_stop(ctx,0x8c61b2e6u); r[4]<<=2;goto L8c61b2e8; /* shll2 r4 */
L8c61b2e8: sa_motion_publish_stop(ctx,0x8c61b2e8u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61b2ea; /* shll r4 */
L8c61b2ea: sa_motion_publish_stop(ctx,0x8c61b2eau); r[0]=24u;goto L8c61b2ec; /* mov #0x18,r0 */
L8c61b2ec: sa_motion_publish_stop(ctx,0x8c61b2ecu); r[4]+=r[3];goto L8c61b2ee; /* add r3,r4 */
L8c61b2ee: sa_motion_publish_stop(ctx,0x8c61b2eeu); r[3]=rd(m,bus,&io_status,0x8c61b478u,4);if(io_status)return io_status;goto L8c61b2f0; /* mov.l 0x8c61b478,r3 */
L8c61b2f0: sa_motion_publish_stop(ctx,0x8c61b2f0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b2f2; /* fmov.s @(r0,r4),fr3 */
L8c61b2f2: sa_motion_publish_stop(ctx,0x8c61b2f2u); r[0]=20u;goto L8c61b2f4; /* mov #0x14,r0 */
L8c61b2f4: sa_motion_publish_stop(ctx,0x8c61b2f4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b2f6; /* fmov.s fr3,@-r8 */
L8c61b2f6: sa_motion_publish_stop(ctx,0x8c61b2f6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b2f8; /* fmov.s @(r0,r4),fr3 */
L8c61b2f8: sa_motion_publish_stop(ctx,0x8c61b2f8u); r[0]=16u;goto L8c61b2fa; /* mov #0x10,r0 */
L8c61b2fa: sa_motion_publish_stop(ctx,0x8c61b2fau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b2fc; /* fmov.s fr3,@-r8 */
L8c61b2fc: sa_motion_publish_stop(ctx,0x8c61b2fcu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b2fe; /* fmov.s @(r0,r4),fr3 */
L8c61b2fe: sa_motion_publish_stop(ctx,0x8c61b2feu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b300; /* fmov.s fr3,@-r8 */
L8c61b300: sa_motion_publish_stop(ctx,0x8c61b300u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b302; /* mov.l r3,@-r8 */
L8c61b302: sa_motion_publish_stop(ctx,0x8c61b302u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b304; /* pref @r8 */
L8c61b304: sa_motion_publish_stop(ctx,0x8c61b304u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b306; /* mov.w @r10+,r4 */
L8c61b306: sa_motion_publish_stop(ctx,0x8c61b306u); r[0]=24u;goto L8c61b308; /* mov #0x18,r0 */
L8c61b308: sa_motion_publish_stop(ctx,0x8c61b308u); r[2]=rd(m,bus,&io_status,0x8c61b454u,4);if(io_status)return io_status;goto L8c61b30a; /* mov.l 0x8c61b454,r2 */
L8c61b30a: sa_motion_publish_stop(ctx,0x8c61b30au); r[8]+=48u;goto L8c61b30c; /* add #0x30,r8 */
L8c61b30c: sa_motion_publish_stop(ctx,0x8c61b30cu); r[4]=r[4]&65535u;goto L8c61b30e; /* extu.w r4,r4 */
L8c61b30e: sa_motion_publish_stop(ctx,0x8c61b30eu); r[4]<<=2;goto L8c61b310; /* shll2 r4 */
L8c61b310: sa_motion_publish_stop(ctx,0x8c61b310u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61b312; /* mov.l @r2,r3 */
L8c61b312: sa_motion_publish_stop(ctx,0x8c61b312u); r[4]<<=2;goto L8c61b314; /* shll2 r4 */
L8c61b314: sa_motion_publish_stop(ctx,0x8c61b314u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61b316; /* shll r4 */
L8c61b316: sa_motion_publish_stop(ctx,0x8c61b316u); r[4]+=r[3];goto L8c61b318; /* add r3,r4 */
L8c61b318: sa_motion_publish_stop(ctx,0x8c61b318u); r[3]=rd(m,bus,&io_status,0x8c61b478u,4);if(io_status)return io_status;goto L8c61b31a; /* mov.l 0x8c61b478,r3 */
L8c61b31a: sa_motion_publish_stop(ctx,0x8c61b31au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b31c; /* fmov.s @(r0,r4),fr3 */
L8c61b31c: sa_motion_publish_stop(ctx,0x8c61b31cu); r[0]=20u;goto L8c61b31e; /* mov #0x14,r0 */
L8c61b31e: sa_motion_publish_stop(ctx,0x8c61b31eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b320; /* fmov.s fr3,@-r8 */
L8c61b320: sa_motion_publish_stop(ctx,0x8c61b320u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b322; /* fmov.s @(r0,r4),fr3 */
L8c61b322: sa_motion_publish_stop(ctx,0x8c61b322u); r[0]=16u;goto L8c61b324; /* mov #0x10,r0 */
L8c61b324: sa_motion_publish_stop(ctx,0x8c61b324u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b326; /* fmov.s fr3,@-r8 */
L8c61b326: sa_motion_publish_stop(ctx,0x8c61b326u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b328; /* fmov.s @(r0,r4),fr3 */
L8c61b328: sa_motion_publish_stop(ctx,0x8c61b328u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b32a; /* fmov.s fr3,@-r8 */
L8c61b32a: sa_motion_publish_stop(ctx,0x8c61b32au); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b32c; /* mov.l r3,@-r8 */
L8c61b32c: sa_motion_publish_stop(ctx,0x8c61b32cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b32e; /* pref @r8 */
L8c61b32e: sa_motion_publish_stop(ctx,0x8c61b32eu); r[6]=r[14];goto L8c61b330; /* mov r14,r6 */
L8c61b330: sa_motion_publish_stop(ctx,0x8c61b330u); r[6]+=4294967295u;goto L8c61b332; /* add #-0x1,r6 */
L8c61b332: sa_motion_publish_stop(ctx,0x8c61b332u); r[5]=r[11];goto L8c61b334; /* mov r11,r5 */
L8c61b334: sa_motion_publish_stop(ctx,0x8c61b334u); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61b336; /* cmp/ge r6,r5 */
L8c61b336: sa_motion_publish_stop(ctx,0x8c61b338u); take=(*sr&1u);r[8]+=32u;if(take)goto L8c61b384;goto L8c61b33a; /* bt/s 0x8c61b384 */
L8c61b33a: sa_motion_publish_stop(ctx,0x8c61b33au); r[0]=16u;goto L8c61b33c; /* mov #0x10,r0 */
L8c61b33c: sa_motion_publish_stop(ctx,0x8c61b33cu); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b33e; /* mov.w @r10+,r4 */
L8c61b33e: sa_motion_publish_stop(ctx,0x8c61b33eu); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b340; /* fmov.s @(r0,r15),fr3 */
L8c61b340: sa_motion_publish_stop(ctx,0x8c61b340u); r[8]+=32u;goto L8c61b342; /* add #0x20,r8 */
L8c61b342: sa_motion_publish_stop(ctx,0x8c61b342u); r[0]=12u;goto L8c61b344; /* mov #0xc,r0 */
L8c61b344: sa_motion_publish_stop(ctx,0x8c61b344u); r[2]=rd(m,bus,&io_status,0x8c61b454u,4);if(io_status)return io_status;goto L8c61b346; /* mov.l 0x8c61b454,r2 */
L8c61b346: sa_motion_publish_stop(ctx,0x8c61b346u); r[4]=r[4]&65535u;goto L8c61b348; /* extu.w r4,r4 */
L8c61b348: sa_motion_publish_stop(ctx,0x8c61b348u); r[4]<<=2;goto L8c61b34a; /* shll2 r4 */
L8c61b34a: sa_motion_publish_stop(ctx,0x8c61b34au); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61b34c; /* mov.l @r2,r3 */
L8c61b34c: sa_motion_publish_stop(ctx,0x8c61b34cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b34e; /* fmov.s fr3,@-r8 */
L8c61b34e: sa_motion_publish_stop(ctx,0x8c61b34eu); r[4]<<=2;goto L8c61b350; /* shll2 r4 */
L8c61b350: sa_motion_publish_stop(ctx,0x8c61b350u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b352; /* fmov.s @(r0,r15),fr3 */
L8c61b352: sa_motion_publish_stop(ctx,0x8c61b352u); r[0]=8u;goto L8c61b354; /* mov #0x8,r0 */
L8c61b354: sa_motion_publish_stop(ctx,0x8c61b354u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61b356; /* shll r4 */
L8c61b356: sa_motion_publish_stop(ctx,0x8c61b356u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b358; /* fmov.s fr3,@-r8 */
L8c61b358: sa_motion_publish_stop(ctx,0x8c61b358u); r[4]+=r[3];goto L8c61b35a; /* add r3,r4 */
L8c61b35a: sa_motion_publish_stop(ctx,0x8c61b35au); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b35c; /* fmov.s @(r0,r15),fr3 */
L8c61b35c: sa_motion_publish_stop(ctx,0x8c61b35cu); r[0]=4u;goto L8c61b35e; /* mov #0x4,r0 */
L8c61b35e: sa_motion_publish_stop(ctx,0x8c61b35eu); r[3]=rd(m,bus,&io_status,0x8c61b478u,4);if(io_status)return io_status;goto L8c61b360; /* mov.l 0x8c61b478,r3 */
L8c61b360: sa_motion_publish_stop(ctx,0x8c61b360u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b362; /* fmov.s fr3,@-r8 */
L8c61b362: sa_motion_publish_stop(ctx,0x8c61b362u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b364; /* fmov.s @(r0,r15),fr3 */
L8c61b364: sa_motion_publish_stop(ctx,0x8c61b364u); r[0]=24u;goto L8c61b366; /* mov #0x18,r0 */
L8c61b366: sa_motion_publish_stop(ctx,0x8c61b366u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b368; /* fmov.s fr3,@-r8 */
L8c61b368: sa_motion_publish_stop(ctx,0x8c61b368u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b36a; /* fmov.s @(r0,r4),fr3 */
L8c61b36a: sa_motion_publish_stop(ctx,0x8c61b36au); r[0]=20u;goto L8c61b36c; /* mov #0x14,r0 */
L8c61b36c: sa_motion_publish_stop(ctx,0x8c61b36cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b36e; /* fmov.s fr3,@-r8 */
L8c61b36e: sa_motion_publish_stop(ctx,0x8c61b36eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b370; /* fmov.s @(r0,r4),fr3 */
L8c61b370: sa_motion_publish_stop(ctx,0x8c61b370u); r[0]=16u;goto L8c61b372; /* mov #0x10,r0 */
L8c61b372: sa_motion_publish_stop(ctx,0x8c61b372u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b374; /* fmov.s fr3,@-r8 */
L8c61b374: sa_motion_publish_stop(ctx,0x8c61b374u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b376; /* fmov.s @(r0,r4),fr3 */
L8c61b376: sa_motion_publish_stop(ctx,0x8c61b376u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b378; /* fmov.s fr3,@-r8 */
L8c61b378: sa_motion_publish_stop(ctx,0x8c61b378u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b37a; /* mov.l r3,@-r8 */
L8c61b37a: sa_motion_publish_stop(ctx,0x8c61b37au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b37c; /* pref @r8 */
L8c61b37c: sa_motion_publish_stop(ctx,0x8c61b37cu); r[5]+=1u;goto L8c61b37e; /* add #0x1,r5 */
L8c61b37e: sa_motion_publish_stop(ctx,0x8c61b37eu); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61b380; /* cmp/ge r6,r5 */
L8c61b380: sa_motion_publish_stop(ctx,0x8c61b382u); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61b33a;goto L8c61b384; /* bf/s 0x8c61b33a */
L8c61b384: sa_motion_publish_stop(ctx,0x8c61b384u); r[0]=16u;goto L8c61b386; /* mov #0x10,r0 */
L8c61b386: sa_motion_publish_stop(ctx,0x8c61b386u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b388; /* mov.w @r10+,r4 */
L8c61b388: sa_motion_publish_stop(ctx,0x8c61b388u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b38a; /* fmov.s @(r0,r15),fr3 */
L8c61b38a: sa_motion_publish_stop(ctx,0x8c61b38au); r[8]+=32u;goto L8c61b38c; /* add #0x20,r8 */
L8c61b38c: sa_motion_publish_stop(ctx,0x8c61b38cu); r[0]=12u;goto L8c61b38e; /* mov #0xc,r0 */
L8c61b38e: sa_motion_publish_stop(ctx,0x8c61b38eu); r[2]=rd(m,bus,&io_status,0x8c61b454u,4);if(io_status)return io_status;goto L8c61b390; /* mov.l 0x8c61b454,r2 */
L8c61b390: sa_motion_publish_stop(ctx,0x8c61b390u); r[4]=r[4]&65535u;goto L8c61b392; /* extu.w r4,r4 */
L8c61b392: sa_motion_publish_stop(ctx,0x8c61b392u); r[4]<<=2;goto L8c61b394; /* shll2 r4 */
L8c61b394: sa_motion_publish_stop(ctx,0x8c61b394u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61b396; /* mov.l @r2,r3 */
L8c61b396: sa_motion_publish_stop(ctx,0x8c61b396u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b398; /* fmov.s fr3,@-r8 */
L8c61b398: sa_motion_publish_stop(ctx,0x8c61b398u); r[4]<<=2;goto L8c61b39a; /* shll2 r4 */
L8c61b39a: sa_motion_publish_stop(ctx,0x8c61b39au); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b39c; /* fmov.s @(r0,r15),fr3 */
L8c61b39c: sa_motion_publish_stop(ctx,0x8c61b39cu); r[0]=8u;goto L8c61b39e; /* mov #0x8,r0 */
L8c61b39e: sa_motion_publish_stop(ctx,0x8c61b39eu); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61b3a0; /* shll r4 */
L8c61b3a0: sa_motion_publish_stop(ctx,0x8c61b3a0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b3a2; /* fmov.s fr3,@-r8 */
L8c61b3a2: sa_motion_publish_stop(ctx,0x8c61b3a2u); r[4]+=r[3];goto L8c61b3a4; /* add r3,r4 */
L8c61b3a4: sa_motion_publish_stop(ctx,0x8c61b3a4u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b3a6; /* fmov.s @(r0,r15),fr3 */
L8c61b3a6: sa_motion_publish_stop(ctx,0x8c61b3a6u); r[0]=4u;goto L8c61b3a8; /* mov #0x4,r0 */
L8c61b3a8: sa_motion_publish_stop(ctx,0x8c61b3a8u); r[3]=rd(m,bus,&io_status,0x8c61b47cu,4);if(io_status)return io_status;goto L8c61b3aa; /* mov.l 0x8c61b47c,r3 */
L8c61b3aa: sa_motion_publish_stop(ctx,0x8c61b3aau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b3ac; /* fmov.s fr3,@-r8 */
L8c61b3ac: sa_motion_publish_stop(ctx,0x8c61b3acu); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b3ae; /* fmov.s @(r0,r15),fr3 */
L8c61b3ae: sa_motion_publish_stop(ctx,0x8c61b3aeu); r[0]=24u;goto L8c61b3b0; /* mov #0x18,r0 */
L8c61b3b0: sa_motion_publish_stop(ctx,0x8c61b3b0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b3b2; /* fmov.s fr3,@-r8 */
L8c61b3b2: sa_motion_publish_stop(ctx,0x8c61b3b2u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b3b4; /* fmov.s @(r0,r4),fr3 */
L8c61b3b4: sa_motion_publish_stop(ctx,0x8c61b3b4u); r[0]=20u;goto L8c61b3b6; /* mov #0x14,r0 */
L8c61b3b6: sa_motion_publish_stop(ctx,0x8c61b3b6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b3b8; /* fmov.s fr3,@-r8 */
L8c61b3b8: sa_motion_publish_stop(ctx,0x8c61b3b8u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b3ba; /* fmov.s @(r0,r4),fr3 */
L8c61b3ba: sa_motion_publish_stop(ctx,0x8c61b3bau); r[0]=16u;goto L8c61b3bc; /* mov #0x10,r0 */
L8c61b3bc: sa_motion_publish_stop(ctx,0x8c61b3bcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b3be; /* fmov.s fr3,@-r8 */
L8c61b3be: sa_motion_publish_stop(ctx,0x8c61b3beu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61b3c0; /* fmov.s @(r0,r4),fr3 */
L8c61b3c0: sa_motion_publish_stop(ctx,0x8c61b3c0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b3c2; /* fmov.s fr3,@-r8 */
L8c61b3c2: sa_motion_publish_stop(ctx,0x8c61b3c2u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b3c4; /* mov.l r3,@-r8 */
L8c61b3c4: sa_motion_publish_stop(ctx,0x8c61b3c4u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b3c6; /* pref @r8 */
L8c61b3c6: sa_motion_publish_stop(ctx,0x8c61b3c6u); r[8]+=32u;goto L8c61b3c8; /* add #0x20,r8 */
L8c61b3c8: sa_motion_publish_stop(ctx,0x8c61b3c8u); r[12]+=1u;goto L8c61b3ca; /* add #0x1,r12 */
L8c61b3ca: sa_motion_publish_stop(ctx,0x8c61b3cau); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b3cc; /* mov.l @r15,r2 */
L8c61b3cc: sa_motion_publish_stop(ctx,0x8c61b3ccu); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61b3ce; /* mov.w @(0x2,r2),r0 */
L8c61b3ce: sa_motion_publish_stop(ctx,0x8c61b3ceu); r[0]=r[0]&65535u;goto L8c61b3d0; /* extu.w r0,r0 */
L8c61b3d0: sa_motion_publish_stop(ctx,0x8c61b3d0u); *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[0]);goto L8c61b3d2; /* cmp/ge r0,r12 */
L8c61b3d2: sa_motion_publish_stop(ctx,0x8c61b3d2u); if((*sr&1u))goto L8c61b3d8;goto L8c61b3d4; /* bt 0x8c61b3d8 */
L8c61b3d4: sa_motion_publish_stop(ctx,0x8c61b3d6u); goto L8c61b1e8; /* bra 0x8c61b1e8 */
L8c61b3d8: sa_motion_publish_stop(ctx,0x8c61b3dau); goto L8c61b432; /* bra 0x8c61b432 */
L8c61b3dc: sa_motion_publish_stop(ctx,0x8c61b3dcu); r[2]=rd(m,bus,&io_status,0x8c61b480u,4);if(io_status)return io_status;goto L8c61b3de; /* mov.l 0x8c61b480,r2 */
L8c61b3de: sa_motion_publish_stop(ctx,0x8c61b3deu); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b3e0; /* mov.l @r15,r1 */
L8c61b3e0: sa_motion_publish_stop(ctx,0x8c61b3e0u); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61b3e2; /* mov.l r1,@r2 */
L8c61b3e2: sa_motion_publish_stop(ctx,0x8c61b3e2u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61b3e4; /* frchg */
L8c61b3e4: sa_motion_publish_stop(ctx,0x8c61b3e4u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61b3e6; /* fmov.s fr0,@-r15 */
L8c61b3e6: sa_motion_publish_stop(ctx,0x8c61b3e6u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61b3e8; /* fmov.s fr1,@-r15 */
L8c61b3e8: sa_motion_publish_stop(ctx,0x8c61b3e8u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61b3ea; /* fmov.s fr2,@-r15 */
L8c61b3ea: sa_motion_publish_stop(ctx,0x8c61b3eau); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61b3ec; /* fmov.s fr3,@-r15 */
L8c61b3ec: sa_motion_publish_stop(ctx,0x8c61b3ecu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61b3ee; /* fmov.s fr4,@-r15 */
L8c61b3ee: sa_motion_publish_stop(ctx,0x8c61b3eeu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61b3f0; /* fmov.s fr5,@-r15 */
L8c61b3f0: sa_motion_publish_stop(ctx,0x8c61b3f0u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61b3f2; /* fmov.s fr6,@-r15 */
L8c61b3f2: sa_motion_publish_stop(ctx,0x8c61b3f2u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61b3f4; /* fmov.s fr7,@-r15 */
L8c61b3f4: sa_motion_publish_stop(ctx,0x8c61b3f4u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61b3f6; /* fmov.s fr8,@-r15 */
L8c61b3f6: sa_motion_publish_stop(ctx,0x8c61b3f6u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61b3f8; /* fmov.s fr9,@-r15 */
L8c61b3f8: sa_motion_publish_stop(ctx,0x8c61b3f8u); r[0]=rd(m,bus,&io_status,0x8c61b444u,4);if(io_status)return io_status;goto L8c61b3fa; /* mov.l 0x8c61b444,r0 */
L8c61b3fa: sa_motion_publish_stop(ctx,0x8c61b3fau); fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b3fc; /* fmov.s @r0+,fr0 */
L8c61b3fc: sa_motion_publish_stop(ctx,0x8c61b3fcu); fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b3fe; /* fmov.s @r0+,fr1 */
L8c61b3fe: sa_motion_publish_stop(ctx,0x8c61b3feu); fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c61b400; /* fmov.s @r0,fr2 */
L8c61b400: sa_motion_publish_stop(ctx,0x8c61b400u); r[0]=rd(m,bus,&io_status,0x8c61b448u,4);if(io_status)return io_status;goto L8c61b402; /* mov.l 0x8c61b448,r0 */
L8c61b402: sa_motion_publish_stop(ctx,0x8c61b402u); r[0]+=16u;goto L8c61b404; /* add #0x10,r0 */
L8c61b404: sa_motion_publish_stop(ctx,0x8c61b404u); fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b406; /* fmov.s @r0+,fr4 */
L8c61b406: sa_motion_publish_stop(ctx,0x8c61b406u); fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b408; /* fmov.s @r0+,fr5 */
L8c61b408: sa_motion_publish_stop(ctx,0x8c61b408u); fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b40a; /* fmov.s @r0+,fr6 */
L8c61b40a: sa_motion_publish_stop(ctx,0x8c61b40au); fload(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;goto L8c61b40c; /* fmov.s @r0,fr7 */
L8c61b40c: sa_motion_publish_stop(ctx,0x8c61b40cu); r[0]=rd(m,bus,&io_status,0x8c61b44cu,4);if(io_status)return io_status;goto L8c61b40e; /* mov.l 0x8c61b44c,r0 */
L8c61b40e: sa_motion_publish_stop(ctx,0x8c61b40eu); fload(m,bus,&io_status,f,8u,r[0]);if(io_status)return io_status;goto L8c61b410; /* fmov.s @r0,fr8 */
L8c61b410: sa_motion_publish_stop(ctx,0x8c61b410u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61b412; /* frchg */
L8c61b412: sa_motion_publish_stop(ctx,0x8c61b412u); r[2]=rd(m,bus,&io_status,0x8c61b484u,4);if(io_status)return io_status;goto L8c61b414; /* mov.l 0x8c61b484,r2 */
L8c61b414: sa_motion_publish_stop(ctx,0x8c61b414u); r[3]=rd(m,bus,&io_status,0x8c61b480u,4);if(io_status)return io_status;goto L8c61b416; /* mov.l 0x8c61b480,r3 */
L8c61b416: sa_motion_publish_stop(ctx,0x8c61b418u); target=r[2];*pr=0x8c61b41au;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b41a; /* jsr @r2 */
L8c61b41a: sa_motion_publish_stop(ctx,0x8c61b41au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61b41c; /* frchg */
L8c61b41c: sa_motion_publish_stop(ctx,0x8c61b41cu); fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b41e; /* fmov.s @r15+,fr9 */
L8c61b41e: sa_motion_publish_stop(ctx,0x8c61b41eu); fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b420; /* fmov.s @r15+,fr8 */
L8c61b420: sa_motion_publish_stop(ctx,0x8c61b420u); fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b422; /* fmov.s @r15+,fr7 */
L8c61b422: sa_motion_publish_stop(ctx,0x8c61b422u); fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b424; /* fmov.s @r15+,fr6 */
L8c61b424: sa_motion_publish_stop(ctx,0x8c61b424u); fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b426; /* fmov.s @r15+,fr5 */
L8c61b426: sa_motion_publish_stop(ctx,0x8c61b426u); fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b428; /* fmov.s @r15+,fr4 */
L8c61b428: sa_motion_publish_stop(ctx,0x8c61b428u); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b42a; /* fmov.s @r15+,fr3 */
L8c61b42a: sa_motion_publish_stop(ctx,0x8c61b42au); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b42c; /* fmov.s @r15+,fr2 */
L8c61b42c: sa_motion_publish_stop(ctx,0x8c61b42cu); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b42e; /* fmov.s @r15+,fr1 */
L8c61b42e: sa_motion_publish_stop(ctx,0x8c61b42eu); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61b430; /* fmov.s @r15+,fr0 */
L8c61b430: sa_motion_publish_stop(ctx,0x8c61b430u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61b432; /* frchg */
L8c61b432: sa_motion_publish_stop(ctx,0x8c61b432u); r[15]+=32u;goto L8c61b434; /* add #0x20,r15 */
L8c61b434: sa_motion_publish_stop(ctx,0x8c61b434u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61b436; /* lds.l @r15+,PR */
L8c61b436: sa_motion_publish_stop(ctx,0x8c61b436u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61b438; /* mov.l @r15+,r9 */
L8c61b438: sa_motion_publish_stop(ctx,0x8c61b438u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61b43a; /* mov.l @r15+,r10 */
L8c61b43a: sa_motion_publish_stop(ctx,0x8c61b43au); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61b43c; /* mov.l @r15+,r11 */
L8c61b43c: sa_motion_publish_stop(ctx,0x8c61b43cu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61b43e; /* mov.l @r15+,r12 */
L8c61b43e: sa_motion_publish_stop(ctx,0x8c61b43eu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61b440; /* mov.l @r15+,r13 */
L8c61b440: sa_motion_publish_stop(ctx,0x8c61b442u); if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
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
L8c61b488: sa_motion_publish_stop(ctx,0x8c61b488u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61b48a; /* mov.l r14,@-r15 */
L8c61b48a: sa_motion_publish_stop(ctx,0x8c61b48au); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61b48c; /* mov.l r13,@-r15 */
L8c61b48c: sa_motion_publish_stop(ctx,0x8c61b48cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61b48e; /* mov.l r12,@-r15 */
L8c61b48e: sa_motion_publish_stop(ctx,0x8c61b48eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61b490; /* mov.l r11,@-r15 */
L8c61b490: sa_motion_publish_stop(ctx,0x8c61b490u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61b492; /* mov.l r10,@-r15 */
L8c61b492: sa_motion_publish_stop(ctx,0x8c61b492u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61b494; /* mov.l r9,@-r15 */
L8c61b494: sa_motion_publish_stop(ctx,0x8c61b494u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61b496; /* sts.l PR,@-r15 */
L8c61b496: sa_motion_publish_stop(ctx,0x8c61b496u); r[15]+=4294967256u;goto L8c61b498; /* add #-0x28,r15 */
L8c61b498: sa_motion_publish_stop(ctx,0x8c61b498u); r[0]=r[4];goto L8c61b49a; /* mov r4,r0 */
L8c61b49a: sa_motion_publish_stop(ctx,0x8c61b49au); r[3]=rd(m,bus,&io_status,0x8c61b4ecu,4);if(io_status)return io_status;goto L8c61b49c; /* mov.l 0x8c61b4ec,r3 */
L8c61b49c: sa_motion_publish_stop(ctx,0x8c61b49cu); wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61b49e; /* mov.l r4,@r15 */
L8c61b49e: sa_motion_publish_stop(ctx,0x8c61b49eu); r[10]=r[4];goto L8c61b4a0; /* mov r4,r10 */
L8c61b4a0: sa_motion_publish_stop(ctx,0x8c61b4a0u); r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61b4a2; /* mov.w @r0,r0 */
L8c61b4a2: sa_motion_publish_stop(ctx,0x8c61b4a2u); r[0]=r[0]&65535u;goto L8c61b4a4; /* extu.w r0,r0 */
L8c61b4a4: sa_motion_publish_stop(ctx,0x8c61b4a4u); r[0]&=r[3];goto L8c61b4a6; /* and r3,r0 */
L8c61b4a6: sa_motion_publish_stop(ctx,0x8c61b4a6u); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61b4a8; /* cmp/eq #0x0,r0 */
L8c61b4a8: sa_motion_publish_stop(ctx,0x8c61b4aau); take=(*sr&1u);r[10]=rd(m,bus,&io_status,r[10]+0x4u,4);if(io_status)return io_status;if(take)goto L8c61b4ce;goto L8c61b4ac; /* bt/s 0x8c61b4ce */
L8c61b4ac: sa_motion_publish_stop(ctx,0x8c61b4acu); r[1]=sx(m,bus,&io_status,0x8c61b4eau,2);if(io_status)return io_status;goto L8c61b4ae; /* mov.w 0x8c61b4ea,r1 */
L8c61b4ae: sa_motion_publish_stop(ctx,0x8c61b4aeu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61b4b0; /* cmp/eq r1,r0 */
L8c61b4b0: sa_motion_publish_stop(ctx,0x8c61b4b0u); if(!(*sr&1u))goto L8c61b4b6;goto L8c61b4b2; /* bf 0x8c61b4b6 */
L8c61b4b2: sa_motion_publish_stop(ctx,0x8c61b4b4u); goto L8c61b71a; /* bra 0x8c61b71a */
L8c61b4b6: sa_motion_publish_stop(ctx,0x8c61b4b6u); r[1]=rd(m,bus,&io_status,0x8c61b4f0u,4);if(io_status)return io_status;goto L8c61b4b8; /* mov.l 0x8c61b4f0,r1 */
L8c61b4b8: sa_motion_publish_stop(ctx,0x8c61b4b8u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61b4ba; /* cmp/eq r1,r0 */
L8c61b4ba: sa_motion_publish_stop(ctx,0x8c61b4bau); if(!(*sr&1u))goto L8c61b4c0;goto L8c61b4bc; /* bf 0x8c61b4c0 */
L8c61b4bc: sa_motion_publish_stop(ctx,0x8c61b4beu); goto L8c61ba00; /* bra 0x8c61ba00 */
L8c61b4c0: sa_motion_publish_stop(ctx,0x8c61b4c0u); r[1]=rd(m,bus,&io_status,0x8c61b4ecu,4);if(io_status)return io_status;goto L8c61b4c2; /* mov.l 0x8c61b4ec,r1 */
L8c61b4c2: sa_motion_publish_stop(ctx,0x8c61b4c2u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61b4c4; /* cmp/eq r1,r0 */
L8c61b4c4: sa_motion_publish_stop(ctx,0x8c61b4c4u); if(!(*sr&1u))goto L8c61b4ca;goto L8c61b4c6; /* bf 0x8c61b4ca */
L8c61b4c6: sa_motion_publish_stop(ctx,0x8c61b4c8u); goto L8c61bd9a; /* bra 0x8c61bd9a */
L8c61b4ca: sa_motion_publish_stop(ctx,0x8c61b4ccu); goto L8c61bdf0; /* bra 0x8c61bdf0 */
L8c61b4ce: sa_motion_publish_stop(ctx,0x8c61b4ceu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b4d0; /* mov.l @r15,r2 */
L8c61b4d0: sa_motion_publish_stop(ctx,0x8c61b4d0u); r[1]=rd(m,bus,&io_status,0x8c61b4f4u,4);if(io_status)return io_status;goto L8c61b4d2; /* mov.l 0x8c61b4f4,r1 */
L8c61b4d2: sa_motion_publish_stop(ctx,0x8c61b4d2u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61b4d4; /* mov.w @(0x2,r2),r0 */
L8c61b4d4: sa_motion_publish_stop(ctx,0x8c61b4d4u); r[0]=r[0]&65535u;goto L8c61b4d6; /* extu.w r0,r0 */
L8c61b4d6: sa_motion_publish_stop(ctx,0x8c61b4d6u); r[2]=r[0];goto L8c61b4d8; /* mov r0,r2 */
L8c61b4d8: sa_motion_publish_stop(ctx,0x8c61b4d8u); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61b4da; /* shll r0 */
L8c61b4da: sa_motion_publish_stop(ctx,0x8c61b4dau); r[0]+=r[2];goto L8c61b4dc; /* add r2,r0 */
L8c61b4dc: sa_motion_publish_stop(ctx,0x8c61b4dcu); r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61b4de; /* mov.l @r1,r2 */
L8c61b4de: sa_motion_publish_stop(ctx,0x8c61b4deu); r[2]+=r[0];goto L8c61b4e0; /* add r0,r2 */
L8c61b4e0: sa_motion_publish_stop(ctx,0x8c61b4e0u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61b4e2; /* mov.l r2,@r1 */
L8c61b4e2: sa_motion_publish_stop(ctx,0x8c61b4e4u); r[11]=0u;goto L8c61b708; /* bra 0x8c61b708 */
L8c61b4f8: sa_motion_publish_stop(ctx,0x8c61b4f8u); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b4fa; /* mov.w @r10+,r13 */
L8c61b4fa: sa_motion_publish_stop(ctx,0x8c61b4fau); r[3]=rd(m,bus,&io_status,0x8c61b734u,4);if(io_status)return io_status;goto L8c61b4fc; /* mov.l 0x8c61b734,r3 */
L8c61b4fc: sa_motion_publish_stop(ctx,0x8c61b4fcu); r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b4fe; /* mov.w @r10+,r12 */
L8c61b4fe: sa_motion_publish_stop(ctx,0x8c61b4feu); r[13]=r[13]&65535u;goto L8c61b500; /* extu.w r13,r13 */
L8c61b500: sa_motion_publish_stop(ctx,0x8c61b500u); r[13]<<=2;goto L8c61b502; /* shll2 r13 */
L8c61b502: sa_motion_publish_stop(ctx,0x8c61b502u); r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61b504; /* mov.l @r3,r5 */
L8c61b504: sa_motion_publish_stop(ctx,0x8c61b504u); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b506; /* mov.w @r10+,r14 */
L8c61b506: sa_motion_publish_stop(ctx,0x8c61b506u); r[12]=r[12]&65535u;goto L8c61b508; /* extu.w r12,r12 */
L8c61b508: sa_motion_publish_stop(ctx,0x8c61b508u); r[12]<<=2;goto L8c61b50a; /* shll2 r12 */
L8c61b50a: sa_motion_publish_stop(ctx,0x8c61b50au); r[14]=r[14]&65535u;goto L8c61b50c; /* extu.w r14,r14 */
L8c61b50c: sa_motion_publish_stop(ctx,0x8c61b50cu); r[12]<<=2;goto L8c61b50e; /* shll2 r12 */
L8c61b50e: sa_motion_publish_stop(ctx,0x8c61b50eu); r[14]<<=2;goto L8c61b510; /* shll2 r14 */
L8c61b510: sa_motion_publish_stop(ctx,0x8c61b510u); r[13]<<=2;goto L8c61b512; /* shll2 r13 */
L8c61b512: sa_motion_publish_stop(ctx,0x8c61b512u); r[14]<<=2;goto L8c61b514; /* shll2 r14 */
L8c61b514: sa_motion_publish_stop(ctx,0x8c61b514u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61b516; /* shll r13 */
L8c61b516: sa_motion_publish_stop(ctx,0x8c61b516u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61b518; /* shll r12 */
L8c61b518: sa_motion_publish_stop(ctx,0x8c61b518u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61b51a; /* shll r14 */
L8c61b51a: sa_motion_publish_stop(ctx,0x8c61b51au); r[13]+=r[5];goto L8c61b51c; /* add r5,r13 */
L8c61b51c: sa_motion_publish_stop(ctx,0x8c61b51cu); r[12]+=r[5];goto L8c61b51e; /* add r5,r12 */
L8c61b51e: sa_motion_publish_stop(ctx,0x8c61b51eu); r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61b520; /* mov.l @(0xc,r13),r4 */
L8c61b520: sa_motion_publish_stop(ctx,0x8c61b520u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61b522; /* mov.l @(0xc,r12),r2 */
L8c61b522: sa_motion_publish_stop(ctx,0x8c61b522u); r[14]+=r[5];goto L8c61b524; /* add r5,r14 */
L8c61b524: sa_motion_publish_stop(ctx,0x8c61b524u); r[1]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61b526; /* mov.l @(0xc,r14),r1 */
L8c61b526: sa_motion_publish_stop(ctx,0x8c61b526u); r[4]&=r[2];goto L8c61b528; /* and r2,r4 */
L8c61b528: sa_motion_publish_stop(ctx,0x8c61b528u); r[2]=63u;goto L8c61b52a; /* mov #0x3f,r2 */
L8c61b52a: sa_motion_publish_stop(ctx,0x8c61b52au); r[4]&=r[1];goto L8c61b52c; /* and r1,r4 */
L8c61b52c: sa_motion_publish_stop(ctx,0x8c61b52cu); *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61b52e; /* tst r2,r4 */
L8c61b52e: sa_motion_publish_stop(ctx,0x8c61b52eu); if((*sr&1u))goto L8c61b534;goto L8c61b530; /* bt 0x8c61b534 */
L8c61b530: sa_motion_publish_stop(ctx,0x8c61b532u); goto L8c61b706; /* bra 0x8c61b706 */
L8c61b534: sa_motion_publish_stop(ctx,0x8c61b534u); r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61b536; /* mov.l @(0xc,r13),r4 */
L8c61b536: sa_motion_publish_stop(ctx,0x8c61b536u); r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61b538; /* mov.l @(0xc,r12),r3 */
L8c61b538: sa_motion_publish_stop(ctx,0x8c61b538u); r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61b53a; /* mov.l @(0xc,r14),r2 */
L8c61b53a: sa_motion_publish_stop(ctx,0x8c61b53au); r[4]|=r[3];goto L8c61b53c; /* or r3,r4 */
L8c61b53c: sa_motion_publish_stop(ctx,0x8c61b53cu); r[3]=1u;goto L8c61b53e; /* mov #0x1,r3 */
L8c61b53e: sa_motion_publish_stop(ctx,0x8c61b53eu); r[4]|=r[2];goto L8c61b540; /* or r2,r4 */
L8c61b540: sa_motion_publish_stop(ctx,0x8c61b540u); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61b542; /* tst r3,r4 */
L8c61b542: sa_motion_publish_stop(ctx,0x8c61b542u); if((*sr&1u))goto L8c61b548;goto L8c61b544; /* bt 0x8c61b548 */
L8c61b544: sa_motion_publish_stop(ctx,0x8c61b546u); goto L8c61b66e; /* bra 0x8c61b66e */
L8c61b548: sa_motion_publish_stop(ctx,0x8c61b548u); r[0]=16u;goto L8c61b54a; /* mov #0x10,r0 */
L8c61b54a: sa_motion_publish_stop(ctx,0x8c61b54au); fload(m,bus,&io_status,f,4u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b54c; /* fmov.s @(r0,r12),fr4 */
L8c61b54c: sa_motion_publish_stop(ctx,0x8c61b54cu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b54e; /* fmov.s @(r0,r13),fr3 */
L8c61b54e: sa_motion_publish_stop(ctx,0x8c61b54eu); r[0]=20u;goto L8c61b550; /* mov #0x14,r0 */
L8c61b550: sa_motion_publish_stop(ctx,0x8c61b550u); fcopy(f,2u,4u);goto L8c61b552; /* fmov fr4,fr2 */
L8c61b552: sa_motion_publish_stop(ctx,0x8c61b552u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61b554; /* fsub fr3,fr2 */
L8c61b554: sa_motion_publish_stop(ctx,0x8c61b554u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b556; /* fmov.s @(r0,r14),fr3 */
L8c61b556: sa_motion_publish_stop(ctx,0x8c61b556u); fload(m,bus,&io_status,f,5u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b558; /* fmov.s @(r0,r12),fr5 */
L8c61b558: sa_motion_publish_stop(ctx,0x8c61b558u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b55a; /* fsub fr5,fr3 */
L8c61b55a: sa_motion_publish_stop(ctx,0x8c61b55au); fcopy(f,1u,5u);goto L8c61b55c; /* fmov fr5,fr1 */
L8c61b55c: sa_motion_publish_stop(ctx,0x8c61b55cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b55e; /* fmul fr3,fr2 */
L8c61b55e: sa_motion_publish_stop(ctx,0x8c61b55eu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b560; /* fmov.s @(r0,r13),fr3 */
L8c61b560: sa_motion_publish_stop(ctx,0x8c61b560u); r[0]=16u;goto L8c61b562; /* mov #0x10,r0 */
L8c61b562: sa_motion_publish_stop(ctx,0x8c61b562u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61b564; /* fsub fr3,fr1 */
L8c61b564: sa_motion_publish_stop(ctx,0x8c61b564u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b566; /* fmov.s @(r0,r14),fr3 */
L8c61b566: sa_motion_publish_stop(ctx,0x8c61b566u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b568; /* fsub fr4,fr3 */
L8c61b568: sa_motion_publish_stop(ctx,0x8c61b568u); fcopy(f,4u,2u);goto L8c61b56a; /* fmov fr2,fr4 */
L8c61b56a: sa_motion_publish_stop(ctx,0x8c61b56au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b56c; /* fmul fr3,fr1 */
L8c61b56c: sa_motion_publish_stop(ctx,0x8c61b56cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61b56e; /* fldi0 fr3 */
L8c61b56e: sa_motion_publish_stop(ctx,0x8c61b56eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b570; /* fsub fr1,fr4 */
L8c61b570: sa_motion_publish_stop(ctx,0x8c61b570u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61b572; /* fcmp/gt fr3,fr4 */
L8c61b572: sa_motion_publish_stop(ctx,0x8c61b572u); if(!(*sr&1u))goto L8c61b578;goto L8c61b574; /* bf 0x8c61b578 */
L8c61b574: sa_motion_publish_stop(ctx,0x8c61b576u); goto L8c61b706; /* bra 0x8c61b706 */
L8c61b578: sa_motion_publish_stop(ctx,0x8c61b578u); fload(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c61b57a; /* fmov.s @r13,fr2 */
L8c61b57a: sa_motion_publish_stop(ctx,0x8c61b57au); r[0]=28u;goto L8c61b57c; /* mov #0x1c,r0 */
L8c61b57c: sa_motion_publish_stop(ctx,0x8c61b57cu); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61b57e; /* fmov.s @r12,fr3 */
L8c61b57e: sa_motion_publish_stop(ctx,0x8c61b57eu); r[4]=r[15];goto L8c61b580; /* mov r15,r4 */
L8c61b580: sa_motion_publish_stop(ctx,0x8c61b580u); r[3]=rd(m,bus,&io_status,0x8c61b738u,4);if(io_status)return io_status;goto L8c61b582; /* mov.l 0x8c61b738,r3 */
L8c61b582: sa_motion_publish_stop(ctx,0x8c61b582u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b584; /* fadd fr3,fr2 */
L8c61b584: sa_motion_publish_stop(ctx,0x8c61b584u); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61b586; /* fmov.s @r14,fr3 */
L8c61b586: sa_motion_publish_stop(ctx,0x8c61b586u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b588; /* fadd fr3,fr2 */
L8c61b588: sa_motion_publish_stop(ctx,0x8c61b588u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b58a; /* fmov fr2,@(r0,r15) */
L8c61b58a: sa_motion_publish_stop(ctx,0x8c61b58au); r[0]=4u;goto L8c61b58c; /* mov #0x4,r0 */
L8c61b58c: sa_motion_publish_stop(ctx,0x8c61b58cu); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b58e; /* fmov.s @(r0,r13),fr2 */
L8c61b58e: sa_motion_publish_stop(ctx,0x8c61b58eu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b590; /* fmov.s @(r0,r12),fr3 */
L8c61b590: sa_motion_publish_stop(ctx,0x8c61b590u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b592; /* fadd fr3,fr2 */
L8c61b592: sa_motion_publish_stop(ctx,0x8c61b592u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b594; /* fmov.s @(r0,r14),fr3 */
L8c61b594: sa_motion_publish_stop(ctx,0x8c61b594u); r[0]=32u;goto L8c61b596; /* mov #0x20,r0 */
L8c61b596: sa_motion_publish_stop(ctx,0x8c61b596u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b598; /* fadd fr3,fr2 */
L8c61b598: sa_motion_publish_stop(ctx,0x8c61b598u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b59a; /* fmov fr2,@(r0,r15) */
L8c61b59a: sa_motion_publish_stop(ctx,0x8c61b59au); r[0]=8u;goto L8c61b59c; /* mov #0x8,r0 */
L8c61b59c: sa_motion_publish_stop(ctx,0x8c61b59cu); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b59e; /* fmov.s @(r0,r13),fr2 */
L8c61b59e: sa_motion_publish_stop(ctx,0x8c61b59eu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b5a0; /* fmov.s @(r0,r12),fr3 */
L8c61b5a0: sa_motion_publish_stop(ctx,0x8c61b5a0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b5a2; /* fadd fr3,fr2 */
L8c61b5a2: sa_motion_publish_stop(ctx,0x8c61b5a2u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b5a4; /* fmov.s @(r0,r14),fr3 */
L8c61b5a4: sa_motion_publish_stop(ctx,0x8c61b5a4u); r[0]=36u;goto L8c61b5a6; /* mov #0x24,r0 */
L8c61b5a6: sa_motion_publish_stop(ctx,0x8c61b5a6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b5a8; /* fadd fr3,fr2 */
L8c61b5a8: sa_motion_publish_stop(ctx,0x8c61b5a8u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b5aa; /* fmov fr2,@(r0,r15) */
L8c61b5aa: sa_motion_publish_stop(ctx,0x8c61b5acu); target=r[3];*pr=0x8c61b5aeu;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b5ae; /* jsr @r3 */
L8c61b5ae: sa_motion_publish_stop(ctx,0x8c61b5aeu); r[3]=rd(m,bus,&io_status,0x8c61b73cu,4);if(io_status)return io_status;goto L8c61b5b0; /* mov.l 0x8c61b73c,r3 */
L8c61b5b0: sa_motion_publish_stop(ctx,0x8c61b5b0u); r[0]=28u;goto L8c61b5b2; /* mov #0x1c,r0 */
L8c61b5b2: sa_motion_publish_stop(ctx,0x8c61b5b2u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b5b4; /* fmov.s @(r0,r15),fr3 */
L8c61b5b4: sa_motion_publish_stop(ctx,0x8c61b5b4u); r[0]=32u;goto L8c61b5b6; /* mov #0x20,r0 */
L8c61b5b6: sa_motion_publish_stop(ctx,0x8c61b5b6u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61b5b8; /* fmov.s @r3,fr2 */
L8c61b5b8: sa_motion_publish_stop(ctx,0x8c61b5b8u); r[3]+=8u;goto L8c61b5ba; /* add #0x8,r3 */
L8c61b5ba: sa_motion_publish_stop(ctx,0x8c61b5bau); r[2]=rd(m,bus,&io_status,0x8c61b740u,4);if(io_status)return io_status;goto L8c61b5bc; /* mov.l 0x8c61b740,r2 */
L8c61b5bc: sa_motion_publish_stop(ctx,0x8c61b5bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b5be; /* fmul fr3,fr2 */
L8c61b5be: sa_motion_publish_stop(ctx,0x8c61b5beu); r[1]=rd(m,bus,&io_status,0x8c61b744u,4);if(io_status)return io_status;goto L8c61b5c0; /* mov.l 0x8c61b744,r1 */
L8c61b5c0: sa_motion_publish_stop(ctx,0x8c61b5c0u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b5c2; /* fmov.s @r2,fr3 */
L8c61b5c2: sa_motion_publish_stop(ctx,0x8c61b5c2u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61b5c4; /* fmov.s @r1,fr1 */
L8c61b5c4: sa_motion_publish_stop(ctx,0x8c61b5c4u); r[2]=rd(m,bus,&io_status,0x8c61b748u,4);if(io_status)return io_status;goto L8c61b5c6; /* mov.l 0x8c61b748,r2 */
L8c61b5c6: sa_motion_publish_stop(ctx,0x8c61b5c6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b5c8; /* fsub fr2,fr3 */
L8c61b5c8: sa_motion_publish_stop(ctx,0x8c61b5c8u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b5ca; /* fmov.s @(r0,r15),fr2 */
L8c61b5ca: sa_motion_publish_stop(ctx,0x8c61b5cau); r[0]=36u;goto L8c61b5cc; /* mov #0x24,r0 */
L8c61b5cc: sa_motion_publish_stop(ctx,0x8c61b5ccu); r[1]=rd(m,bus,&io_status,0x8c61b750u,4);if(io_status)return io_status;goto L8c61b5ce; /* mov.l 0x8c61b750,r1 */
L8c61b5ce: sa_motion_publish_stop(ctx,0x8c61b5ceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b5d0; /* fmul fr2,fr1 */
L8c61b5d0: sa_motion_publish_stop(ctx,0x8c61b5d0u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b5d2; /* fmov.s @(r0,r15),fr2 */
L8c61b5d2: sa_motion_publish_stop(ctx,0x8c61b5d2u); r[0]=12u;goto L8c61b5d4; /* mov #0xc,r0 */
L8c61b5d4: sa_motion_publish_stop(ctx,0x8c61b5d4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b5d6; /* fsub fr1,fr3 */
L8c61b5d6: sa_motion_publish_stop(ctx,0x8c61b5d6u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61b5d8; /* fmov.s @r3,fr1 */
L8c61b5d8: sa_motion_publish_stop(ctx,0x8c61b5d8u); r[3]=rd(m,bus,&io_status,0x8c61b74cu,4);if(io_status)return io_status;goto L8c61b5da; /* mov.l 0x8c61b74c,r3 */
L8c61b5da: sa_motion_publish_stop(ctx,0x8c61b5dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b5dc; /* fmul fr2,fr1 */
L8c61b5dc: sa_motion_publish_stop(ctx,0x8c61b5dcu); fcopy(f,4u,3u);goto L8c61b5de; /* fmov fr3,fr4 */
L8c61b5de: sa_motion_publish_stop(ctx,0x8c61b5deu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b5e0; /* fmov.s @r2,fr3 */
L8c61b5e0: sa_motion_publish_stop(ctx,0x8c61b5e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b5e2; /* fsub fr1,fr4 */
L8c61b5e2: sa_motion_publish_stop(ctx,0x8c61b5e2u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b5e4; /* fmov fr3,@(r0,r15) */
L8c61b5e4: sa_motion_publish_stop(ctx,0x8c61b5e4u); r[0]=16u;goto L8c61b5e6; /* mov #0x10,r0 */
L8c61b5e6: sa_motion_publish_stop(ctx,0x8c61b5e6u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61b5e8; /* fmov.s @r3,fr3 */
L8c61b5e8: sa_motion_publish_stop(ctx,0x8c61b5e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b5ea; /* fmul fr4,fr3 */
L8c61b5ea: sa_motion_publish_stop(ctx,0x8c61b5eau); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b5ec; /* fmov fr3,@(r0,r15) */
L8c61b5ec: sa_motion_publish_stop(ctx,0x8c61b5ecu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61b5ee; /* fmov.s @r1,fr3 */
L8c61b5ee: sa_motion_publish_stop(ctx,0x8c61b5eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b5f0; /* fmul fr4,fr3 */
L8c61b5f0: sa_motion_publish_stop(ctx,0x8c61b5f0u); r[0]=20u;goto L8c61b5f2; /* mov #0x14,r0 */
L8c61b5f2: sa_motion_publish_stop(ctx,0x8c61b5f2u); r[2]+=12u;goto L8c61b5f4; /* add #0xc,r2 */
L8c61b5f4: sa_motion_publish_stop(ctx,0x8c61b5f4u); r[3]=rd(m,bus,&io_status,0x8c61b754u,4);if(io_status)return io_status;goto L8c61b5f6; /* mov.l 0x8c61b754,r3 */
L8c61b5f6: sa_motion_publish_stop(ctx,0x8c61b5f6u); r[8]+=16u;goto L8c61b5f8; /* add #0x10,r8 */
L8c61b5f8: sa_motion_publish_stop(ctx,0x8c61b5f8u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b5fa; /* fmov fr3,@(r0,r15) */
L8c61b5fa: sa_motion_publish_stop(ctx,0x8c61b5fau); r[0]=24u;goto L8c61b5fc; /* mov #0x18,r0 */
L8c61b5fc: sa_motion_publish_stop(ctx,0x8c61b5fcu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b5fe; /* fmov.s @r2,fr3 */
L8c61b5fe: sa_motion_publish_stop(ctx,0x8c61b5feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b600; /* fmul fr4,fr3 */
L8c61b600: sa_motion_publish_stop(ctx,0x8c61b600u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b602; /* fmov fr3,@(r0,r15) */
L8c61b602: sa_motion_publish_stop(ctx,0x8c61b602u); r[0]=24u;goto L8c61b604; /* mov #0x18,r0 */
L8c61b604: sa_motion_publish_stop(ctx,0x8c61b604u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b606; /* fmov.s @(r0,r13),fr3 */
L8c61b606: sa_motion_publish_stop(ctx,0x8c61b606u); r[0]=20u;goto L8c61b608; /* mov #0x14,r0 */
L8c61b608: sa_motion_publish_stop(ctx,0x8c61b608u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b60a; /* fmov.s fr3,@-r8 */
L8c61b60a: sa_motion_publish_stop(ctx,0x8c61b60au); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b60c; /* fmov.s @(r0,r13),fr3 */
L8c61b60c: sa_motion_publish_stop(ctx,0x8c61b60cu); r[0]=16u;goto L8c61b60e; /* mov #0x10,r0 */
L8c61b60e: sa_motion_publish_stop(ctx,0x8c61b60eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b610; /* fmov.s fr3,@-r8 */
L8c61b610: sa_motion_publish_stop(ctx,0x8c61b610u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b612; /* fmov.s @(r0,r13),fr3 */
L8c61b612: sa_motion_publish_stop(ctx,0x8c61b612u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b614; /* fmov.s fr3,@-r8 */
L8c61b614: sa_motion_publish_stop(ctx,0x8c61b614u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b616; /* mov.l r3,@-r8 */
L8c61b616: sa_motion_publish_stop(ctx,0x8c61b616u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b618; /* pref @r8 */
L8c61b618: sa_motion_publish_stop(ctx,0x8c61b618u); r[0]=24u;goto L8c61b61a; /* mov #0x18,r0 */
L8c61b61a: sa_motion_publish_stop(ctx,0x8c61b61au); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b61c; /* fmov.s @(r0,r12),fr3 */
L8c61b61c: sa_motion_publish_stop(ctx,0x8c61b61cu); r[8]+=48u;goto L8c61b61e; /* add #0x30,r8 */
L8c61b61e: sa_motion_publish_stop(ctx,0x8c61b61eu); r[0]=20u;goto L8c61b620; /* mov #0x14,r0 */
L8c61b620: sa_motion_publish_stop(ctx,0x8c61b620u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b622; /* fmov.s fr3,@-r8 */
L8c61b622: sa_motion_publish_stop(ctx,0x8c61b622u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b624; /* fmov.s @(r0,r12),fr3 */
L8c61b624: sa_motion_publish_stop(ctx,0x8c61b624u); r[0]=16u;goto L8c61b626; /* mov #0x10,r0 */
L8c61b626: sa_motion_publish_stop(ctx,0x8c61b626u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b628; /* fmov.s fr3,@-r8 */
L8c61b628: sa_motion_publish_stop(ctx,0x8c61b628u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b62a; /* fmov.s @(r0,r12),fr3 */
L8c61b62a: sa_motion_publish_stop(ctx,0x8c61b62au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b62c; /* fmov.s fr3,@-r8 */
L8c61b62c: sa_motion_publish_stop(ctx,0x8c61b62cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b62e; /* mov.l r3,@-r8 */
L8c61b62e: sa_motion_publish_stop(ctx,0x8c61b62eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b630; /* pref @r8 */
L8c61b630: sa_motion_publish_stop(ctx,0x8c61b630u); r[0]=24u;goto L8c61b632; /* mov #0x18,r0 */
L8c61b632: sa_motion_publish_stop(ctx,0x8c61b632u); r[3]=rd(m,bus,&io_status,0x8c61b758u,4);if(io_status)return io_status;goto L8c61b634; /* mov.l 0x8c61b758,r3 */
L8c61b634: sa_motion_publish_stop(ctx,0x8c61b634u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b636; /* fmov.s @(r0,r15),fr3 */
L8c61b636: sa_motion_publish_stop(ctx,0x8c61b636u); r[8]+=64u;goto L8c61b638; /* add #0x40,r8 */
L8c61b638: sa_motion_publish_stop(ctx,0x8c61b638u); r[0]=20u;goto L8c61b63a; /* mov #0x14,r0 */
L8c61b63a: sa_motion_publish_stop(ctx,0x8c61b63au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b63c; /* fmov.s fr3,@-r8 */
L8c61b63c: sa_motion_publish_stop(ctx,0x8c61b63cu); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b63e; /* fmov.s @(r0,r15),fr3 */
L8c61b63e: sa_motion_publish_stop(ctx,0x8c61b63eu); r[0]=16u;goto L8c61b640; /* mov #0x10,r0 */
L8c61b640: sa_motion_publish_stop(ctx,0x8c61b640u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b642; /* fmov.s fr3,@-r8 */
L8c61b642: sa_motion_publish_stop(ctx,0x8c61b642u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b644; /* fmov.s @(r0,r15),fr3 */
L8c61b644: sa_motion_publish_stop(ctx,0x8c61b644u); r[0]=12u;goto L8c61b646; /* mov #0xc,r0 */
L8c61b646: sa_motion_publish_stop(ctx,0x8c61b646u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b648; /* fmov.s fr3,@-r8 */
L8c61b648: sa_motion_publish_stop(ctx,0x8c61b648u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b64a; /* fmov.s @(r0,r15),fr3 */
L8c61b64a: sa_motion_publish_stop(ctx,0x8c61b64au); r[0]=24u;goto L8c61b64c; /* mov #0x18,r0 */
L8c61b64c: sa_motion_publish_stop(ctx,0x8c61b64cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b64e; /* fmov.s fr3,@-r8 */
L8c61b64e: sa_motion_publish_stop(ctx,0x8c61b64eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b650; /* fmov.s @(r0,r14),fr3 */
L8c61b650: sa_motion_publish_stop(ctx,0x8c61b650u); r[0]=20u;goto L8c61b652; /* mov #0x14,r0 */
L8c61b652: sa_motion_publish_stop(ctx,0x8c61b652u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b654; /* fmov.s fr3,@-r8 */
L8c61b654: sa_motion_publish_stop(ctx,0x8c61b654u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b656; /* fmov.s @(r0,r14),fr3 */
L8c61b656: sa_motion_publish_stop(ctx,0x8c61b656u); r[0]=16u;goto L8c61b658; /* mov #0x10,r0 */
L8c61b658: sa_motion_publish_stop(ctx,0x8c61b658u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b65a; /* fmov.s fr3,@-r8 */
L8c61b65a: sa_motion_publish_stop(ctx,0x8c61b65au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b65c; /* fmov.s @(r0,r14),fr3 */
L8c61b65c: sa_motion_publish_stop(ctx,0x8c61b65cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b65e; /* fmov.s fr3,@-r8 */
L8c61b65e: sa_motion_publish_stop(ctx,0x8c61b65eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b660; /* mov.l r3,@-r8 */
L8c61b660: sa_motion_publish_stop(ctx,0x8c61b660u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b662; /* pref @r8 */
L8c61b662: sa_motion_publish_stop(ctx,0x8c61b662u); r[0]=rd(m,bus,&io_status,0x8c61b75cu,4);if(io_status)return io_status;goto L8c61b664; /* mov.l 0x8c61b75c,r0 */
L8c61b664: sa_motion_publish_stop(ctx,0x8c61b664u); r[8]+=32u;goto L8c61b666; /* add #0x20,r8 */
L8c61b666: sa_motion_publish_stop(ctx,0x8c61b666u); r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61b668; /* mov.l @r0,r3 */
L8c61b668: sa_motion_publish_stop(ctx,0x8c61b668u); r[3]+=3u;goto L8c61b66a; /* add #0x3,r3 */
L8c61b66a: sa_motion_publish_stop(ctx,0x8c61b66cu); wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61b706; /* bra 0x8c61b706 */
L8c61b66e: sa_motion_publish_stop(ctx,0x8c61b66eu); r[0]=28u;goto L8c61b670; /* mov #0x1c,r0 */
L8c61b670: sa_motion_publish_stop(ctx,0x8c61b670u); r[2]=rd(m,bus,&io_status,0x8c61b760u,4);if(io_status)return io_status;goto L8c61b672; /* mov.l 0x8c61b760,r2 */
L8c61b672: sa_motion_publish_stop(ctx,0x8c61b672u); r[4]=r[15];goto L8c61b674; /* mov r15,r4 */
L8c61b674: sa_motion_publish_stop(ctx,0x8c61b674u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61b676; /* mov.l r3,@r2 */
L8c61b676: sa_motion_publish_stop(ctx,0x8c61b676u); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61b678; /* fmov.s @r12,fr3 */
L8c61b678: sa_motion_publish_stop(ctx,0x8c61b678u); fload(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c61b67a; /* fmov.s @r13,fr2 */
L8c61b67a: sa_motion_publish_stop(ctx,0x8c61b67au); r[3]=rd(m,bus,&io_status,0x8c61b738u,4);if(io_status)return io_status;goto L8c61b67c; /* mov.l 0x8c61b738,r3 */
L8c61b67c: sa_motion_publish_stop(ctx,0x8c61b67cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b67e; /* fadd fr3,fr2 */
L8c61b67e: sa_motion_publish_stop(ctx,0x8c61b67eu); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61b680; /* fmov.s @r14,fr3 */
L8c61b680: sa_motion_publish_stop(ctx,0x8c61b680u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b682; /* fadd fr3,fr2 */
L8c61b682: sa_motion_publish_stop(ctx,0x8c61b682u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b684; /* fmov fr2,@(r0,r15) */
L8c61b684: sa_motion_publish_stop(ctx,0x8c61b684u); r[0]=4u;goto L8c61b686; /* mov #0x4,r0 */
L8c61b686: sa_motion_publish_stop(ctx,0x8c61b686u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b688; /* fmov.s @(r0,r12),fr3 */
L8c61b688: sa_motion_publish_stop(ctx,0x8c61b688u); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b68a; /* fmov.s @(r0,r13),fr2 */
L8c61b68a: sa_motion_publish_stop(ctx,0x8c61b68au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b68c; /* fadd fr3,fr2 */
L8c61b68c: sa_motion_publish_stop(ctx,0x8c61b68cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b68e; /* fmov.s @(r0,r14),fr3 */
L8c61b68e: sa_motion_publish_stop(ctx,0x8c61b68eu); r[0]=32u;goto L8c61b690; /* mov #0x20,r0 */
L8c61b690: sa_motion_publish_stop(ctx,0x8c61b690u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b692; /* fadd fr3,fr2 */
L8c61b692: sa_motion_publish_stop(ctx,0x8c61b692u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b694; /* fmov fr2,@(r0,r15) */
L8c61b694: sa_motion_publish_stop(ctx,0x8c61b694u); r[0]=8u;goto L8c61b696; /* mov #0x8,r0 */
L8c61b696: sa_motion_publish_stop(ctx,0x8c61b696u); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b698; /* fmov.s @(r0,r13),fr2 */
L8c61b698: sa_motion_publish_stop(ctx,0x8c61b698u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b69a; /* fmov.s @(r0,r12),fr3 */
L8c61b69a: sa_motion_publish_stop(ctx,0x8c61b69au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b69c; /* fadd fr3,fr2 */
L8c61b69c: sa_motion_publish_stop(ctx,0x8c61b69cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b69e; /* fmov.s @(r0,r14),fr3 */
L8c61b69e: sa_motion_publish_stop(ctx,0x8c61b69eu); r[0]=36u;goto L8c61b6a0; /* mov #0x24,r0 */
L8c61b6a0: sa_motion_publish_stop(ctx,0x8c61b6a0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b6a2; /* fadd fr3,fr2 */
L8c61b6a2: sa_motion_publish_stop(ctx,0x8c61b6a2u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b6a4; /* fmov fr2,@(r0,r15) */
L8c61b6a4: sa_motion_publish_stop(ctx,0x8c61b6a6u); target=r[3];*pr=0x8c61b6a8u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b6a8; /* jsr @r3 */
L8c61b6a8: sa_motion_publish_stop(ctx,0x8c61b6a8u); r[3]=rd(m,bus,&io_status,0x8c61b73cu,4);if(io_status)return io_status;goto L8c61b6aa; /* mov.l 0x8c61b73c,r3 */
L8c61b6aa: sa_motion_publish_stop(ctx,0x8c61b6aau); r[0]=28u;goto L8c61b6ac; /* mov #0x1c,r0 */
L8c61b6ac: sa_motion_publish_stop(ctx,0x8c61b6acu); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b6ae; /* fmov.s @(r0,r15),fr3 */
L8c61b6ae: sa_motion_publish_stop(ctx,0x8c61b6aeu); r[0]=32u;goto L8c61b6b0; /* mov #0x20,r0 */
L8c61b6b0: sa_motion_publish_stop(ctx,0x8c61b6b0u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61b6b2; /* fmov.s @r3,fr2 */
L8c61b6b2: sa_motion_publish_stop(ctx,0x8c61b6b2u); r[3]+=8u;goto L8c61b6b4; /* add #0x8,r3 */
L8c61b6b4: sa_motion_publish_stop(ctx,0x8c61b6b4u); r[2]=rd(m,bus,&io_status,0x8c61b740u,4);if(io_status)return io_status;goto L8c61b6b6; /* mov.l 0x8c61b740,r2 */
L8c61b6b6: sa_motion_publish_stop(ctx,0x8c61b6b6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b6b8; /* fmul fr3,fr2 */
L8c61b6b8: sa_motion_publish_stop(ctx,0x8c61b6b8u); r[1]=rd(m,bus,&io_status,0x8c61b744u,4);if(io_status)return io_status;goto L8c61b6ba; /* mov.l 0x8c61b744,r1 */
L8c61b6ba: sa_motion_publish_stop(ctx,0x8c61b6bau); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b6bc; /* fmov.s @r2,fr3 */
L8c61b6bc: sa_motion_publish_stop(ctx,0x8c61b6bcu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61b6be; /* fmov.s @r1,fr1 */
L8c61b6be: sa_motion_publish_stop(ctx,0x8c61b6beu); r[2]=rd(m,bus,&io_status,0x8c61b748u,4);if(io_status)return io_status;goto L8c61b6c0; /* mov.l 0x8c61b748,r2 */
L8c61b6c0: sa_motion_publish_stop(ctx,0x8c61b6c0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b6c2; /* fsub fr2,fr3 */
L8c61b6c2: sa_motion_publish_stop(ctx,0x8c61b6c2u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b6c4; /* fmov.s @(r0,r15),fr2 */
L8c61b6c4: sa_motion_publish_stop(ctx,0x8c61b6c4u); r[0]=36u;goto L8c61b6c6; /* mov #0x24,r0 */
L8c61b6c6: sa_motion_publish_stop(ctx,0x8c61b6c6u); r[1]=rd(m,bus,&io_status,0x8c61b750u,4);if(io_status)return io_status;goto L8c61b6c8; /* mov.l 0x8c61b750,r1 */
L8c61b6c8: sa_motion_publish_stop(ctx,0x8c61b6c8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b6ca; /* fmul fr2,fr1 */
L8c61b6ca: sa_motion_publish_stop(ctx,0x8c61b6cau); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b6cc; /* fmov.s @(r0,r15),fr2 */
L8c61b6cc: sa_motion_publish_stop(ctx,0x8c61b6ccu); r[0]=12u;goto L8c61b6ce; /* mov #0xc,r0 */
L8c61b6ce: sa_motion_publish_stop(ctx,0x8c61b6ceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b6d0; /* fsub fr1,fr3 */
L8c61b6d0: sa_motion_publish_stop(ctx,0x8c61b6d0u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61b6d2; /* fmov.s @r3,fr1 */
L8c61b6d2: sa_motion_publish_stop(ctx,0x8c61b6d2u); r[3]=rd(m,bus,&io_status,0x8c61b74cu,4);if(io_status)return io_status;goto L8c61b6d4; /* mov.l 0x8c61b74c,r3 */
L8c61b6d4: sa_motion_publish_stop(ctx,0x8c61b6d4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b6d6; /* fmul fr2,fr1 */
L8c61b6d6: sa_motion_publish_stop(ctx,0x8c61b6d6u); fcopy(f,4u,3u);goto L8c61b6d8; /* fmov fr3,fr4 */
L8c61b6d8: sa_motion_publish_stop(ctx,0x8c61b6d8u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b6da; /* fmov.s @r2,fr3 */
L8c61b6da: sa_motion_publish_stop(ctx,0x8c61b6dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b6dc; /* fsub fr1,fr4 */
L8c61b6dc: sa_motion_publish_stop(ctx,0x8c61b6dcu); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b6de; /* fmov fr3,@(r0,r15) */
L8c61b6de: sa_motion_publish_stop(ctx,0x8c61b6deu); r[0]=16u;goto L8c61b6e0; /* mov #0x10,r0 */
L8c61b6e0: sa_motion_publish_stop(ctx,0x8c61b6e0u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61b6e2; /* fmov.s @r3,fr3 */
L8c61b6e2: sa_motion_publish_stop(ctx,0x8c61b6e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b6e4; /* fmul fr4,fr3 */
L8c61b6e4: sa_motion_publish_stop(ctx,0x8c61b6e4u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b6e6; /* fmov fr3,@(r0,r15) */
L8c61b6e6: sa_motion_publish_stop(ctx,0x8c61b6e6u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61b6e8; /* fmov.s @r1,fr3 */
L8c61b6e8: sa_motion_publish_stop(ctx,0x8c61b6e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b6ea; /* fmul fr4,fr3 */
L8c61b6ea: sa_motion_publish_stop(ctx,0x8c61b6eau); r[0]=20u;goto L8c61b6ec; /* mov #0x14,r0 */
L8c61b6ec: sa_motion_publish_stop(ctx,0x8c61b6ecu); r[2]+=12u;goto L8c61b6ee; /* add #0xc,r2 */
L8c61b6ee: sa_motion_publish_stop(ctx,0x8c61b6eeu); r[7]=r[15];goto L8c61b6f0; /* mov r15,r7 */
L8c61b6f0: sa_motion_publish_stop(ctx,0x8c61b6f0u); r[3]=rd(m,bus,&io_status,0x8c61b764u,4);if(io_status)return io_status;goto L8c61b6f2; /* mov.l 0x8c61b764,r3 */
L8c61b6f2: sa_motion_publish_stop(ctx,0x8c61b6f2u); r[5]=r[12];goto L8c61b6f4; /* mov r12,r5 */
L8c61b6f4: sa_motion_publish_stop(ctx,0x8c61b6f4u); r[6]=r[14];goto L8c61b6f6; /* mov r14,r6 */
L8c61b6f6: sa_motion_publish_stop(ctx,0x8c61b6f6u); r[7]+=12u;goto L8c61b6f8; /* add #0xc,r7 */
L8c61b6f8: sa_motion_publish_stop(ctx,0x8c61b6f8u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b6fa; /* fmov fr3,@(r0,r15) */
L8c61b6fa: sa_motion_publish_stop(ctx,0x8c61b6fau); r[0]=24u;goto L8c61b6fc; /* mov #0x18,r0 */
L8c61b6fc: sa_motion_publish_stop(ctx,0x8c61b6fcu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b6fe; /* fmov.s @r2,fr3 */
L8c61b6fe: sa_motion_publish_stop(ctx,0x8c61b6feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b700; /* fmul fr4,fr3 */
L8c61b700: sa_motion_publish_stop(ctx,0x8c61b700u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b702; /* fmov fr3,@(r0,r15) */
L8c61b702: sa_motion_publish_stop(ctx,0x8c61b704u); target=r[3];*pr=0x8c61b706u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b706; /* jsr @r3 */
L8c61b706: sa_motion_publish_stop(ctx,0x8c61b706u); r[11]+=1u;goto L8c61b708; /* add #0x1,r11 */
L8c61b708: sa_motion_publish_stop(ctx,0x8c61b708u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b70a; /* mov.l @r15,r2 */
L8c61b70a: sa_motion_publish_stop(ctx,0x8c61b70au); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61b70c; /* mov.w @(0x2,r2),r0 */
L8c61b70c: sa_motion_publish_stop(ctx,0x8c61b70cu); r[0]=r[0]&65535u;goto L8c61b70e; /* extu.w r0,r0 */
L8c61b70e: sa_motion_publish_stop(ctx,0x8c61b70eu); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61b710; /* cmp/ge r0,r11 */
L8c61b710: sa_motion_publish_stop(ctx,0x8c61b710u); if((*sr&1u))goto L8c61b716;goto L8c61b712; /* bt 0x8c61b716 */
L8c61b712: sa_motion_publish_stop(ctx,0x8c61b714u); goto L8c61b4f8; /* bra 0x8c61b4f8 */
L8c61b716: sa_motion_publish_stop(ctx,0x8c61b718u); goto L8c61bdf0; /* bra 0x8c61bdf0 */
L8c61b71a: sa_motion_publish_stop(ctx,0x8c61b71au); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b71c; /* mov.l @r15,r1 */
L8c61b71c: sa_motion_publish_stop(ctx,0x8c61b71cu); r[2]=rd(m,bus,&io_status,0x8c61b768u,4);if(io_status)return io_status;goto L8c61b71e; /* mov.l 0x8c61b768,r2 */
L8c61b71e: sa_motion_publish_stop(ctx,0x8c61b71eu); r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61b720; /* mov.w @(0x2,r1),r0 */
L8c61b720: sa_motion_publish_stop(ctx,0x8c61b720u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61b722; /* mov.l @r2,r1 */
L8c61b722: sa_motion_publish_stop(ctx,0x8c61b722u); r[0]=r[0]&65535u;goto L8c61b724; /* extu.w r0,r0 */
L8c61b724: sa_motion_publish_stop(ctx,0x8c61b724u); r[0]<<=2;goto L8c61b726; /* shll2 r0 */
L8c61b726: sa_motion_publish_stop(ctx,0x8c61b726u); r[1]+=r[0];goto L8c61b728; /* add r0,r1 */
L8c61b728: sa_motion_publish_stop(ctx,0x8c61b728u); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61b72a; /* mov.l r1,@r2 */
L8c61b72a: sa_motion_publish_stop(ctx,0x8c61b72cu); r[9]=0u;goto L8c61b9ee; /* bra 0x8c61b9ee */
L8c61b76c: sa_motion_publish_stop(ctx,0x8c61b76cu); r[11]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b76e; /* mov.w @r10+,r11 */
L8c61b76e: sa_motion_publish_stop(ctx,0x8c61b76eu); r[3]=rd(m,bus,&io_status,0x8c61ba0cu,4);if(io_status)return io_status;goto L8c61b770; /* mov.l 0x8c61ba0c,r3 */
L8c61b770: sa_motion_publish_stop(ctx,0x8c61b770u); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b772; /* mov.w @r10+,r13 */
L8c61b772: sa_motion_publish_stop(ctx,0x8c61b772u); r[11]=r[11]&65535u;goto L8c61b774; /* extu.w r11,r11 */
L8c61b774: sa_motion_publish_stop(ctx,0x8c61b774u); r[11]<<=2;goto L8c61b776; /* shll2 r11 */
L8c61b776: sa_motion_publish_stop(ctx,0x8c61b776u); r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61b778; /* mov.l @r3,r5 */
L8c61b778: sa_motion_publish_stop(ctx,0x8c61b778u); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b77a; /* mov.w @r10+,r14 */
L8c61b77a: sa_motion_publish_stop(ctx,0x8c61b77au); r[13]=r[13]&65535u;goto L8c61b77c; /* extu.w r13,r13 */
L8c61b77c: sa_motion_publish_stop(ctx,0x8c61b77cu); r[13]<<=2;goto L8c61b77e; /* shll2 r13 */
L8c61b77e: sa_motion_publish_stop(ctx,0x8c61b77eu); r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61b780; /* mov.w @r10+,r12 */
L8c61b780: sa_motion_publish_stop(ctx,0x8c61b780u); r[14]=r[14]&65535u;goto L8c61b782; /* extu.w r14,r14 */
L8c61b782: sa_motion_publish_stop(ctx,0x8c61b782u); r[13]<<=2;goto L8c61b784; /* shll2 r13 */
L8c61b784: sa_motion_publish_stop(ctx,0x8c61b784u); r[11]<<=2;goto L8c61b786; /* shll2 r11 */
L8c61b786: sa_motion_publish_stop(ctx,0x8c61b786u); r[12]=r[12]&65535u;goto L8c61b788; /* extu.w r12,r12 */
L8c61b788: sa_motion_publish_stop(ctx,0x8c61b788u); r[14]<<=2;goto L8c61b78a; /* shll2 r14 */
L8c61b78a: sa_motion_publish_stop(ctx,0x8c61b78au); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61b78c; /* shll r11 */
L8c61b78c: sa_motion_publish_stop(ctx,0x8c61b78cu); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61b78e; /* shll r13 */
L8c61b78e: sa_motion_publish_stop(ctx,0x8c61b78eu); r[11]+=r[5];goto L8c61b790; /* add r5,r11 */
L8c61b790: sa_motion_publish_stop(ctx,0x8c61b790u); r[12]<<=2;goto L8c61b792; /* shll2 r12 */
L8c61b792: sa_motion_publish_stop(ctx,0x8c61b792u); r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61b794; /* mov.l @(0xc,r11),r4 */
L8c61b794: sa_motion_publish_stop(ctx,0x8c61b794u); r[13]+=r[5];goto L8c61b796; /* add r5,r13 */
L8c61b796: sa_motion_publish_stop(ctx,0x8c61b796u); r[14]<<=2;goto L8c61b798; /* shll2 r14 */
L8c61b798: sa_motion_publish_stop(ctx,0x8c61b798u); r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61b79a; /* mov.l @(0xc,r13),r2 */
L8c61b79a: sa_motion_publish_stop(ctx,0x8c61b79au); r[12]<<=2;goto L8c61b79c; /* shll2 r12 */
L8c61b79c: sa_motion_publish_stop(ctx,0x8c61b79cu); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61b79e; /* shll r14 */
L8c61b79e: sa_motion_publish_stop(ctx,0x8c61b79eu); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61b7a0; /* shll r12 */
L8c61b7a0: sa_motion_publish_stop(ctx,0x8c61b7a0u); r[14]+=r[5];goto L8c61b7a2; /* add r5,r14 */
L8c61b7a2: sa_motion_publish_stop(ctx,0x8c61b7a2u); r[1]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61b7a4; /* mov.l @(0xc,r14),r1 */
L8c61b7a4: sa_motion_publish_stop(ctx,0x8c61b7a4u); r[4]&=r[2];goto L8c61b7a6; /* and r2,r4 */
L8c61b7a6: sa_motion_publish_stop(ctx,0x8c61b7a6u); r[12]+=r[5];goto L8c61b7a8; /* add r5,r12 */
L8c61b7a8: sa_motion_publish_stop(ctx,0x8c61b7a8u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61b7aa; /* mov.l @(0xc,r12),r2 */
L8c61b7aa: sa_motion_publish_stop(ctx,0x8c61b7aau); r[4]&=r[1];goto L8c61b7ac; /* and r1,r4 */
L8c61b7ac: sa_motion_publish_stop(ctx,0x8c61b7acu); r[1]=63u;goto L8c61b7ae; /* mov #0x3f,r1 */
L8c61b7ae: sa_motion_publish_stop(ctx,0x8c61b7aeu); r[4]&=r[2];goto L8c61b7b0; /* and r2,r4 */
L8c61b7b0: sa_motion_publish_stop(ctx,0x8c61b7b0u); *sr=(*sr&~1u)|!(r[1]&r[4]);goto L8c61b7b2; /* tst r1,r4 */
L8c61b7b2: sa_motion_publish_stop(ctx,0x8c61b7b2u); if((*sr&1u))goto L8c61b7b8;goto L8c61b7b4; /* bt 0x8c61b7b8 */
L8c61b7b4: sa_motion_publish_stop(ctx,0x8c61b7b6u); goto L8c61b9ec; /* bra 0x8c61b9ec */
L8c61b7b8: sa_motion_publish_stop(ctx,0x8c61b7b8u); r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61b7ba; /* mov.l @(0xc,r11),r4 */
L8c61b7ba: sa_motion_publish_stop(ctx,0x8c61b7bau); r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61b7bc; /* mov.l @(0xc,r13),r3 */
L8c61b7bc: sa_motion_publish_stop(ctx,0x8c61b7bcu); r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61b7be; /* mov.l @(0xc,r14),r2 */
L8c61b7be: sa_motion_publish_stop(ctx,0x8c61b7beu); r[4]|=r[3];goto L8c61b7c0; /* or r3,r4 */
L8c61b7c0: sa_motion_publish_stop(ctx,0x8c61b7c0u); r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61b7c2; /* mov.l @(0xc,r12),r3 */
L8c61b7c2: sa_motion_publish_stop(ctx,0x8c61b7c2u); r[4]|=r[2];goto L8c61b7c4; /* or r2,r4 */
L8c61b7c4: sa_motion_publish_stop(ctx,0x8c61b7c4u); r[2]=1u;goto L8c61b7c6; /* mov #0x1,r2 */
L8c61b7c6: sa_motion_publish_stop(ctx,0x8c61b7c6u); r[4]|=r[3];goto L8c61b7c8; /* or r3,r4 */
L8c61b7c8: sa_motion_publish_stop(ctx,0x8c61b7c8u); *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61b7ca; /* tst r2,r4 */
L8c61b7ca: sa_motion_publish_stop(ctx,0x8c61b7cau); if((*sr&1u))goto L8c61b7d0;goto L8c61b7cc; /* bt 0x8c61b7d0 */
L8c61b7cc: sa_motion_publish_stop(ctx,0x8c61b7ceu); goto L8c61b926; /* bra 0x8c61b926 */
L8c61b7d0: sa_motion_publish_stop(ctx,0x8c61b7d0u); r[0]=16u;goto L8c61b7d2; /* mov #0x10,r0 */
L8c61b7d2: sa_motion_publish_stop(ctx,0x8c61b7d2u); fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b7d4; /* fmov.s @(r0,r13),fr5 */
L8c61b7d4: sa_motion_publish_stop(ctx,0x8c61b7d4u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b7d6; /* fmov.s @(r0,r11),fr3 */
L8c61b7d6: sa_motion_publish_stop(ctx,0x8c61b7d6u); r[0]=20u;goto L8c61b7d8; /* mov #0x14,r0 */
L8c61b7d8: sa_motion_publish_stop(ctx,0x8c61b7d8u); fcopy(f,2u,5u);goto L8c61b7da; /* fmov fr5,fr2 */
L8c61b7da: sa_motion_publish_stop(ctx,0x8c61b7dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61b7dc; /* fsub fr3,fr2 */
L8c61b7dc: sa_motion_publish_stop(ctx,0x8c61b7dcu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b7de; /* fmov.s @(r0,r14),fr3 */
L8c61b7de: sa_motion_publish_stop(ctx,0x8c61b7deu); fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b7e0; /* fmov.s @(r0,r13),fr4 */
L8c61b7e0: sa_motion_publish_stop(ctx,0x8c61b7e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b7e2; /* fsub fr4,fr3 */
L8c61b7e2: sa_motion_publish_stop(ctx,0x8c61b7e2u); fcopy(f,1u,4u);goto L8c61b7e4; /* fmov fr4,fr1 */
L8c61b7e4: sa_motion_publish_stop(ctx,0x8c61b7e4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b7e6; /* fmul fr3,fr2 */
L8c61b7e6: sa_motion_publish_stop(ctx,0x8c61b7e6u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b7e8; /* fmov.s @(r0,r11),fr3 */
L8c61b7e8: sa_motion_publish_stop(ctx,0x8c61b7e8u); r[0]=16u;goto L8c61b7ea; /* mov #0x10,r0 */
L8c61b7ea: sa_motion_publish_stop(ctx,0x8c61b7eau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61b7ec; /* fsub fr3,fr1 */
L8c61b7ec: sa_motion_publish_stop(ctx,0x8c61b7ecu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b7ee; /* fmov.s @(r0,r14),fr3 */
L8c61b7ee: sa_motion_publish_stop(ctx,0x8c61b7eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b7f0; /* fsub fr5,fr3 */
L8c61b7f0: sa_motion_publish_stop(ctx,0x8c61b7f0u); fcopy(f,4u,2u);goto L8c61b7f2; /* fmov fr2,fr4 */
L8c61b7f2: sa_motion_publish_stop(ctx,0x8c61b7f2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b7f4; /* fmul fr3,fr1 */
L8c61b7f4: sa_motion_publish_stop(ctx,0x8c61b7f4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61b7f6; /* fldi0 fr3 */
L8c61b7f6: sa_motion_publish_stop(ctx,0x8c61b7f6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b7f8; /* fsub fr1,fr4 */
L8c61b7f8: sa_motion_publish_stop(ctx,0x8c61b7f8u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61b7fa; /* fcmp/gt fr3,fr4 */
L8c61b7fa: sa_motion_publish_stop(ctx,0x8c61b7fau); if(!(*sr&1u))goto L8c61b800;goto L8c61b7fc; /* bf 0x8c61b800 */
L8c61b7fc: sa_motion_publish_stop(ctx,0x8c61b7feu); goto L8c61b9ec; /* bra 0x8c61b9ec */
L8c61b800: sa_motion_publish_stop(ctx,0x8c61b800u); fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c61b802; /* fmov.s @r11,fr2 */
L8c61b802: sa_motion_publish_stop(ctx,0x8c61b802u); r[0]=28u;goto L8c61b804; /* mov #0x1c,r0 */
L8c61b804: sa_motion_publish_stop(ctx,0x8c61b804u); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61b806; /* fmov.s @r13,fr3 */
L8c61b806: sa_motion_publish_stop(ctx,0x8c61b806u); r[4]=r[15];goto L8c61b808; /* mov r15,r4 */
L8c61b808: sa_motion_publish_stop(ctx,0x8c61b808u); r[3]=rd(m,bus,&io_status,0x8c61ba10u,4);if(io_status)return io_status;goto L8c61b80a; /* mov.l 0x8c61ba10,r3 */
L8c61b80a: sa_motion_publish_stop(ctx,0x8c61b80au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b80c; /* fadd fr3,fr2 */
L8c61b80c: sa_motion_publish_stop(ctx,0x8c61b80cu); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61b80e; /* fmov.s @r14,fr3 */
L8c61b80e: sa_motion_publish_stop(ctx,0x8c61b80eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b810; /* fadd fr3,fr2 */
L8c61b810: sa_motion_publish_stop(ctx,0x8c61b810u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b812; /* fmov fr2,@(r0,r15) */
L8c61b812: sa_motion_publish_stop(ctx,0x8c61b812u); r[0]=4u;goto L8c61b814; /* mov #0x4,r0 */
L8c61b814: sa_motion_publish_stop(ctx,0x8c61b814u); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b816; /* fmov.s @(r0,r11),fr2 */
L8c61b816: sa_motion_publish_stop(ctx,0x8c61b816u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b818; /* fmov.s @(r0,r13),fr3 */
L8c61b818: sa_motion_publish_stop(ctx,0x8c61b818u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b81a; /* fadd fr3,fr2 */
L8c61b81a: sa_motion_publish_stop(ctx,0x8c61b81au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b81c; /* fmov.s @(r0,r14),fr3 */
L8c61b81c: sa_motion_publish_stop(ctx,0x8c61b81cu); r[0]=32u;goto L8c61b81e; /* mov #0x20,r0 */
L8c61b81e: sa_motion_publish_stop(ctx,0x8c61b81eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b820; /* fadd fr3,fr2 */
L8c61b820: sa_motion_publish_stop(ctx,0x8c61b820u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b822; /* fmov fr2,@(r0,r15) */
L8c61b822: sa_motion_publish_stop(ctx,0x8c61b822u); r[0]=8u;goto L8c61b824; /* mov #0x8,r0 */
L8c61b824: sa_motion_publish_stop(ctx,0x8c61b824u); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b826; /* fmov.s @(r0,r11),fr2 */
L8c61b826: sa_motion_publish_stop(ctx,0x8c61b826u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b828; /* fmov.s @(r0,r13),fr3 */
L8c61b828: sa_motion_publish_stop(ctx,0x8c61b828u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b82a; /* fadd fr3,fr2 */
L8c61b82a: sa_motion_publish_stop(ctx,0x8c61b82au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b82c; /* fmov.s @(r0,r14),fr3 */
L8c61b82c: sa_motion_publish_stop(ctx,0x8c61b82cu); r[0]=36u;goto L8c61b82e; /* mov #0x24,r0 */
L8c61b82e: sa_motion_publish_stop(ctx,0x8c61b82eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b830; /* fadd fr3,fr2 */
L8c61b830: sa_motion_publish_stop(ctx,0x8c61b830u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b832; /* fmov fr2,@(r0,r15) */
L8c61b832: sa_motion_publish_stop(ctx,0x8c61b834u); target=r[3];*pr=0x8c61b836u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b836; /* jsr @r3 */
L8c61b836: sa_motion_publish_stop(ctx,0x8c61b836u); r[3]=rd(m,bus,&io_status,0x8c61ba14u,4);if(io_status)return io_status;goto L8c61b838; /* mov.l 0x8c61ba14,r3 */
L8c61b838: sa_motion_publish_stop(ctx,0x8c61b838u); r[0]=28u;goto L8c61b83a; /* mov #0x1c,r0 */
L8c61b83a: sa_motion_publish_stop(ctx,0x8c61b83au); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b83c; /* fmov.s @(r0,r15),fr3 */
L8c61b83c: sa_motion_publish_stop(ctx,0x8c61b83cu); r[0]=32u;goto L8c61b83e; /* mov #0x20,r0 */
L8c61b83e: sa_motion_publish_stop(ctx,0x8c61b83eu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61b840; /* fmov.s @r3,fr2 */
L8c61b840: sa_motion_publish_stop(ctx,0x8c61b840u); r[3]+=8u;goto L8c61b842; /* add #0x8,r3 */
L8c61b842: sa_motion_publish_stop(ctx,0x8c61b842u); r[2]=rd(m,bus,&io_status,0x8c61ba18u,4);if(io_status)return io_status;goto L8c61b844; /* mov.l 0x8c61ba18,r2 */
L8c61b844: sa_motion_publish_stop(ctx,0x8c61b844u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b846; /* fmul fr3,fr2 */
L8c61b846: sa_motion_publish_stop(ctx,0x8c61b846u); r[1]=rd(m,bus,&io_status,0x8c61ba1cu,4);if(io_status)return io_status;goto L8c61b848; /* mov.l 0x8c61ba1c,r1 */
L8c61b848: sa_motion_publish_stop(ctx,0x8c61b848u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b84a; /* fmov.s @r2,fr3 */
L8c61b84a: sa_motion_publish_stop(ctx,0x8c61b84au); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61b84c; /* fmov.s @r1,fr1 */
L8c61b84c: sa_motion_publish_stop(ctx,0x8c61b84cu); r[2]=rd(m,bus,&io_status,0x8c61ba20u,4);if(io_status)return io_status;goto L8c61b84e; /* mov.l 0x8c61ba20,r2 */
L8c61b84e: sa_motion_publish_stop(ctx,0x8c61b84eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b850; /* fsub fr2,fr3 */
L8c61b850: sa_motion_publish_stop(ctx,0x8c61b850u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b852; /* fmov.s @(r0,r15),fr2 */
L8c61b852: sa_motion_publish_stop(ctx,0x8c61b852u); r[0]=36u;goto L8c61b854; /* mov #0x24,r0 */
L8c61b854: sa_motion_publish_stop(ctx,0x8c61b854u); r[1]=rd(m,bus,&io_status,0x8c61ba28u,4);if(io_status)return io_status;goto L8c61b856; /* mov.l 0x8c61ba28,r1 */
L8c61b856: sa_motion_publish_stop(ctx,0x8c61b856u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b858; /* fmul fr2,fr1 */
L8c61b858: sa_motion_publish_stop(ctx,0x8c61b858u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b85a; /* fmov.s @(r0,r15),fr2 */
L8c61b85a: sa_motion_publish_stop(ctx,0x8c61b85au); r[0]=12u;goto L8c61b85c; /* mov #0xc,r0 */
L8c61b85c: sa_motion_publish_stop(ctx,0x8c61b85cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b85e; /* fsub fr1,fr3 */
L8c61b85e: sa_motion_publish_stop(ctx,0x8c61b85eu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61b860; /* fmov.s @r3,fr1 */
L8c61b860: sa_motion_publish_stop(ctx,0x8c61b860u); r[3]=rd(m,bus,&io_status,0x8c61ba24u,4);if(io_status)return io_status;goto L8c61b862; /* mov.l 0x8c61ba24,r3 */
L8c61b862: sa_motion_publish_stop(ctx,0x8c61b862u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b864; /* fmul fr2,fr1 */
L8c61b864: sa_motion_publish_stop(ctx,0x8c61b864u); fcopy(f,4u,3u);goto L8c61b866; /* fmov fr3,fr4 */
L8c61b866: sa_motion_publish_stop(ctx,0x8c61b866u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b868; /* fmov.s @r2,fr3 */
L8c61b868: sa_motion_publish_stop(ctx,0x8c61b868u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b86a; /* fsub fr1,fr4 */
L8c61b86a: sa_motion_publish_stop(ctx,0x8c61b86au); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b86c; /* fmov fr3,@(r0,r15) */
L8c61b86c: sa_motion_publish_stop(ctx,0x8c61b86cu); r[0]=16u;goto L8c61b86e; /* mov #0x10,r0 */
L8c61b86e: sa_motion_publish_stop(ctx,0x8c61b86eu); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61b870; /* fmov.s @r3,fr3 */
L8c61b870: sa_motion_publish_stop(ctx,0x8c61b870u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b872; /* fmul fr4,fr3 */
L8c61b872: sa_motion_publish_stop(ctx,0x8c61b872u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b874; /* fmov fr3,@(r0,r15) */
L8c61b874: sa_motion_publish_stop(ctx,0x8c61b874u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61b876; /* fmov.s @r1,fr3 */
L8c61b876: sa_motion_publish_stop(ctx,0x8c61b876u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b878; /* fmul fr4,fr3 */
L8c61b878: sa_motion_publish_stop(ctx,0x8c61b878u); r[0]=20u;goto L8c61b87a; /* mov #0x14,r0 */
L8c61b87a: sa_motion_publish_stop(ctx,0x8c61b87au); r[2]+=12u;goto L8c61b87c; /* add #0xc,r2 */
L8c61b87c: sa_motion_publish_stop(ctx,0x8c61b87cu); r[3]=rd(m,bus,&io_status,0x8c61ba2cu,4);if(io_status)return io_status;goto L8c61b87e; /* mov.l 0x8c61ba2c,r3 */
L8c61b87e: sa_motion_publish_stop(ctx,0x8c61b87eu); r[8]+=16u;goto L8c61b880; /* add #0x10,r8 */
L8c61b880: sa_motion_publish_stop(ctx,0x8c61b880u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b882; /* fmov fr3,@(r0,r15) */
L8c61b882: sa_motion_publish_stop(ctx,0x8c61b882u); r[0]=24u;goto L8c61b884; /* mov #0x18,r0 */
L8c61b884: sa_motion_publish_stop(ctx,0x8c61b884u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b886; /* fmov.s @r2,fr3 */
L8c61b886: sa_motion_publish_stop(ctx,0x8c61b886u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b888; /* fmul fr4,fr3 */
L8c61b888: sa_motion_publish_stop(ctx,0x8c61b888u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b88a; /* fmov fr3,@(r0,r15) */
L8c61b88a: sa_motion_publish_stop(ctx,0x8c61b88au); r[0]=24u;goto L8c61b88c; /* mov #0x18,r0 */
L8c61b88c: sa_motion_publish_stop(ctx,0x8c61b88cu); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b88e; /* fmov.s @(r0,r11),fr3 */
L8c61b88e: sa_motion_publish_stop(ctx,0x8c61b88eu); r[0]=20u;goto L8c61b890; /* mov #0x14,r0 */
L8c61b890: sa_motion_publish_stop(ctx,0x8c61b890u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b892; /* fmov.s fr3,@-r8 */
L8c61b892: sa_motion_publish_stop(ctx,0x8c61b892u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b894; /* fmov.s @(r0,r11),fr3 */
L8c61b894: sa_motion_publish_stop(ctx,0x8c61b894u); r[0]=16u;goto L8c61b896; /* mov #0x10,r0 */
L8c61b896: sa_motion_publish_stop(ctx,0x8c61b896u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b898; /* fmov.s fr3,@-r8 */
L8c61b898: sa_motion_publish_stop(ctx,0x8c61b898u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b89a; /* fmov.s @(r0,r11),fr3 */
L8c61b89a: sa_motion_publish_stop(ctx,0x8c61b89au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b89c; /* fmov.s fr3,@-r8 */
L8c61b89c: sa_motion_publish_stop(ctx,0x8c61b89cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b89e; /* mov.l r3,@-r8 */
L8c61b89e: sa_motion_publish_stop(ctx,0x8c61b89eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b8a0; /* pref @r8 */
L8c61b8a0: sa_motion_publish_stop(ctx,0x8c61b8a0u); r[0]=24u;goto L8c61b8a2; /* mov #0x18,r0 */
L8c61b8a2: sa_motion_publish_stop(ctx,0x8c61b8a2u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b8a4; /* fmov.s @(r0,r13),fr3 */
L8c61b8a4: sa_motion_publish_stop(ctx,0x8c61b8a4u); r[8]+=48u;goto L8c61b8a6; /* add #0x30,r8 */
L8c61b8a6: sa_motion_publish_stop(ctx,0x8c61b8a6u); r[0]=20u;goto L8c61b8a8; /* mov #0x14,r0 */
L8c61b8a8: sa_motion_publish_stop(ctx,0x8c61b8a8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8aa; /* fmov.s fr3,@-r8 */
L8c61b8aa: sa_motion_publish_stop(ctx,0x8c61b8aau); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b8ac; /* fmov.s @(r0,r13),fr3 */
L8c61b8ac: sa_motion_publish_stop(ctx,0x8c61b8acu); r[0]=16u;goto L8c61b8ae; /* mov #0x10,r0 */
L8c61b8ae: sa_motion_publish_stop(ctx,0x8c61b8aeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8b0; /* fmov.s fr3,@-r8 */
L8c61b8b0: sa_motion_publish_stop(ctx,0x8c61b8b0u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b8b2; /* fmov.s @(r0,r13),fr3 */
L8c61b8b2: sa_motion_publish_stop(ctx,0x8c61b8b2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8b4; /* fmov.s fr3,@-r8 */
L8c61b8b4: sa_motion_publish_stop(ctx,0x8c61b8b4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b8b6; /* mov.l r3,@-r8 */
L8c61b8b6: sa_motion_publish_stop(ctx,0x8c61b8b6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b8b8; /* pref @r8 */
L8c61b8b8: sa_motion_publish_stop(ctx,0x8c61b8b8u); r[0]=24u;goto L8c61b8ba; /* mov #0x18,r0 */
L8c61b8ba: sa_motion_publish_stop(ctx,0x8c61b8bau); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b8bc; /* fmov.s @(r0,r15),fr3 */
L8c61b8bc: sa_motion_publish_stop(ctx,0x8c61b8bcu); r[8]+=64u;goto L8c61b8be; /* add #0x40,r8 */
L8c61b8be: sa_motion_publish_stop(ctx,0x8c61b8beu); r[0]=20u;goto L8c61b8c0; /* mov #0x14,r0 */
L8c61b8c0: sa_motion_publish_stop(ctx,0x8c61b8c0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8c2; /* fmov.s fr3,@-r8 */
L8c61b8c2: sa_motion_publish_stop(ctx,0x8c61b8c2u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b8c4; /* fmov.s @(r0,r15),fr3 */
L8c61b8c4: sa_motion_publish_stop(ctx,0x8c61b8c4u); r[0]=16u;goto L8c61b8c6; /* mov #0x10,r0 */
L8c61b8c6: sa_motion_publish_stop(ctx,0x8c61b8c6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8c8; /* fmov.s fr3,@-r8 */
L8c61b8c8: sa_motion_publish_stop(ctx,0x8c61b8c8u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b8ca; /* fmov.s @(r0,r15),fr3 */
L8c61b8ca: sa_motion_publish_stop(ctx,0x8c61b8cau); r[0]=12u;goto L8c61b8cc; /* mov #0xc,r0 */
L8c61b8cc: sa_motion_publish_stop(ctx,0x8c61b8ccu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8ce; /* fmov.s fr3,@-r8 */
L8c61b8ce: sa_motion_publish_stop(ctx,0x8c61b8ceu); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b8d0; /* fmov.s @(r0,r15),fr3 */
L8c61b8d0: sa_motion_publish_stop(ctx,0x8c61b8d0u); r[0]=24u;goto L8c61b8d2; /* mov #0x18,r0 */
L8c61b8d2: sa_motion_publish_stop(ctx,0x8c61b8d2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8d4; /* fmov.s fr3,@-r8 */
L8c61b8d4: sa_motion_publish_stop(ctx,0x8c61b8d4u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b8d6; /* fmov.s @(r0,r14),fr3 */
L8c61b8d6: sa_motion_publish_stop(ctx,0x8c61b8d6u); r[0]=20u;goto L8c61b8d8; /* mov #0x14,r0 */
L8c61b8d8: sa_motion_publish_stop(ctx,0x8c61b8d8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8da; /* fmov.s fr3,@-r8 */
L8c61b8da: sa_motion_publish_stop(ctx,0x8c61b8dau); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b8dc; /* fmov.s @(r0,r14),fr3 */
L8c61b8dc: sa_motion_publish_stop(ctx,0x8c61b8dcu); r[0]=16u;goto L8c61b8de; /* mov #0x10,r0 */
L8c61b8de: sa_motion_publish_stop(ctx,0x8c61b8deu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8e0; /* fmov.s fr3,@-r8 */
L8c61b8e0: sa_motion_publish_stop(ctx,0x8c61b8e0u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b8e2; /* fmov.s @(r0,r14),fr3 */
L8c61b8e2: sa_motion_publish_stop(ctx,0x8c61b8e2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8e4; /* fmov.s fr3,@-r8 */
L8c61b8e4: sa_motion_publish_stop(ctx,0x8c61b8e4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b8e6; /* mov.l r3,@-r8 */
L8c61b8e6: sa_motion_publish_stop(ctx,0x8c61b8e6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b8e8; /* pref @r8 */
L8c61b8e8: sa_motion_publish_stop(ctx,0x8c61b8e8u); r[0]=24u;goto L8c61b8ea; /* mov #0x18,r0 */
L8c61b8ea: sa_motion_publish_stop(ctx,0x8c61b8eau); r[3]=rd(m,bus,&io_status,0x8c61ba30u,4);if(io_status)return io_status;goto L8c61b8ec; /* mov.l 0x8c61ba30,r3 */
L8c61b8ec: sa_motion_publish_stop(ctx,0x8c61b8ecu); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b8ee; /* fmov.s @(r0,r15),fr3 */
L8c61b8ee: sa_motion_publish_stop(ctx,0x8c61b8eeu); r[8]+=64u;goto L8c61b8f0; /* add #0x40,r8 */
L8c61b8f0: sa_motion_publish_stop(ctx,0x8c61b8f0u); r[0]=20u;goto L8c61b8f2; /* mov #0x14,r0 */
L8c61b8f2: sa_motion_publish_stop(ctx,0x8c61b8f2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8f4; /* fmov.s fr3,@-r8 */
L8c61b8f4: sa_motion_publish_stop(ctx,0x8c61b8f4u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b8f6; /* fmov.s @(r0,r15),fr3 */
L8c61b8f6: sa_motion_publish_stop(ctx,0x8c61b8f6u); r[0]=16u;goto L8c61b8f8; /* mov #0x10,r0 */
L8c61b8f8: sa_motion_publish_stop(ctx,0x8c61b8f8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b8fa; /* fmov.s fr3,@-r8 */
L8c61b8fa: sa_motion_publish_stop(ctx,0x8c61b8fau); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b8fc; /* fmov.s @(r0,r15),fr3 */
L8c61b8fc: sa_motion_publish_stop(ctx,0x8c61b8fcu); r[0]=12u;goto L8c61b8fe; /* mov #0xc,r0 */
L8c61b8fe: sa_motion_publish_stop(ctx,0x8c61b8feu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b900; /* fmov.s fr3,@-r8 */
L8c61b900: sa_motion_publish_stop(ctx,0x8c61b900u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b902; /* fmov.s @(r0,r15),fr3 */
L8c61b902: sa_motion_publish_stop(ctx,0x8c61b902u); r[0]=24u;goto L8c61b904; /* mov #0x18,r0 */
L8c61b904: sa_motion_publish_stop(ctx,0x8c61b904u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b906; /* fmov.s fr3,@-r8 */
L8c61b906: sa_motion_publish_stop(ctx,0x8c61b906u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b908; /* fmov.s @(r0,r12),fr3 */
L8c61b908: sa_motion_publish_stop(ctx,0x8c61b908u); r[0]=20u;goto L8c61b90a; /* mov #0x14,r0 */
L8c61b90a: sa_motion_publish_stop(ctx,0x8c61b90au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b90c; /* fmov.s fr3,@-r8 */
L8c61b90c: sa_motion_publish_stop(ctx,0x8c61b90cu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b90e; /* fmov.s @(r0,r12),fr3 */
L8c61b90e: sa_motion_publish_stop(ctx,0x8c61b90eu); r[0]=16u;goto L8c61b910; /* mov #0x10,r0 */
L8c61b910: sa_motion_publish_stop(ctx,0x8c61b910u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b912; /* fmov.s fr3,@-r8 */
L8c61b912: sa_motion_publish_stop(ctx,0x8c61b912u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61b914; /* fmov.s @(r0,r12),fr3 */
L8c61b914: sa_motion_publish_stop(ctx,0x8c61b914u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61b916; /* fmov.s fr3,@-r8 */
L8c61b916: sa_motion_publish_stop(ctx,0x8c61b916u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61b918; /* mov.l r3,@-r8 */
L8c61b918: sa_motion_publish_stop(ctx,0x8c61b918u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61b91a; /* pref @r8 */
L8c61b91a: sa_motion_publish_stop(ctx,0x8c61b91au); r[0]=rd(m,bus,&io_status,0x8c61ba34u,4);if(io_status)return io_status;goto L8c61b91c; /* mov.l 0x8c61ba34,r0 */
L8c61b91c: sa_motion_publish_stop(ctx,0x8c61b91cu); r[8]+=32u;goto L8c61b91e; /* add #0x20,r8 */
L8c61b91e: sa_motion_publish_stop(ctx,0x8c61b91eu); r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61b920; /* mov.l @r0,r3 */
L8c61b920: sa_motion_publish_stop(ctx,0x8c61b920u); r[3]+=4u;goto L8c61b922; /* add #0x4,r3 */
L8c61b922: sa_motion_publish_stop(ctx,0x8c61b924u); wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61b9ec; /* bra 0x8c61b9ec */
L8c61b926: sa_motion_publish_stop(ctx,0x8c61b926u); r[0]=rd(m,bus,&io_status,0x8c61ba38u,4);if(io_status)return io_status;goto L8c61b928; /* mov.l 0x8c61ba38,r0 */
L8c61b928: sa_motion_publish_stop(ctx,0x8c61b928u); r[4]=r[15];goto L8c61b92a; /* mov r15,r4 */
L8c61b92a: sa_motion_publish_stop(ctx,0x8c61b92au); r[3]=rd(m,bus,&io_status,0x8c61ba10u,4);if(io_status)return io_status;goto L8c61b92c; /* mov.l 0x8c61ba10,r3 */
L8c61b92c: sa_motion_publish_stop(ctx,0x8c61b92cu); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61b92e; /* mov.l r2,@r0 */
L8c61b92e: sa_motion_publish_stop(ctx,0x8c61b92eu); r[0]=28u;goto L8c61b930; /* mov #0x1c,r0 */
L8c61b930: sa_motion_publish_stop(ctx,0x8c61b930u); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61b932; /* fmov.s @r13,fr3 */
L8c61b932: sa_motion_publish_stop(ctx,0x8c61b932u); fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c61b934; /* fmov.s @r11,fr2 */
L8c61b934: sa_motion_publish_stop(ctx,0x8c61b934u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b936; /* fadd fr3,fr2 */
L8c61b936: sa_motion_publish_stop(ctx,0x8c61b936u); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61b938; /* fmov.s @r14,fr3 */
L8c61b938: sa_motion_publish_stop(ctx,0x8c61b938u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b93a; /* fadd fr3,fr2 */
L8c61b93a: sa_motion_publish_stop(ctx,0x8c61b93au); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b93c; /* fmov fr2,@(r0,r15) */
L8c61b93c: sa_motion_publish_stop(ctx,0x8c61b93cu); r[0]=4u;goto L8c61b93e; /* mov #0x4,r0 */
L8c61b93e: sa_motion_publish_stop(ctx,0x8c61b93eu); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b940; /* fmov.s @(r0,r11),fr2 */
L8c61b940: sa_motion_publish_stop(ctx,0x8c61b940u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b942; /* fmov.s @(r0,r13),fr3 */
L8c61b942: sa_motion_publish_stop(ctx,0x8c61b942u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b944; /* fadd fr3,fr2 */
L8c61b944: sa_motion_publish_stop(ctx,0x8c61b944u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b946; /* fmov.s @(r0,r14),fr3 */
L8c61b946: sa_motion_publish_stop(ctx,0x8c61b946u); r[0]=32u;goto L8c61b948; /* mov #0x20,r0 */
L8c61b948: sa_motion_publish_stop(ctx,0x8c61b948u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b94a; /* fadd fr3,fr2 */
L8c61b94a: sa_motion_publish_stop(ctx,0x8c61b94au); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b94c; /* fmov fr2,@(r0,r15) */
L8c61b94c: sa_motion_publish_stop(ctx,0x8c61b94cu); r[0]=8u;goto L8c61b94e; /* mov #0x8,r0 */
L8c61b94e: sa_motion_publish_stop(ctx,0x8c61b94eu); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61b950; /* fmov.s @(r0,r11),fr2 */
L8c61b950: sa_motion_publish_stop(ctx,0x8c61b950u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61b952; /* fmov.s @(r0,r13),fr3 */
L8c61b952: sa_motion_publish_stop(ctx,0x8c61b952u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b954; /* fadd fr3,fr2 */
L8c61b954: sa_motion_publish_stop(ctx,0x8c61b954u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61b956; /* fmov.s @(r0,r14),fr3 */
L8c61b956: sa_motion_publish_stop(ctx,0x8c61b956u); r[0]=36u;goto L8c61b958; /* mov #0x24,r0 */
L8c61b958: sa_motion_publish_stop(ctx,0x8c61b958u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61b95a; /* fadd fr3,fr2 */
L8c61b95a: sa_motion_publish_stop(ctx,0x8c61b95au); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b95c; /* fmov fr2,@(r0,r15) */
L8c61b95c: sa_motion_publish_stop(ctx,0x8c61b95eu); target=r[3];*pr=0x8c61b960u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b960; /* jsr @r3 */
L8c61b960: sa_motion_publish_stop(ctx,0x8c61b960u); r[3]=rd(m,bus,&io_status,0x8c61ba14u,4);if(io_status)return io_status;goto L8c61b962; /* mov.l 0x8c61ba14,r3 */
L8c61b962: sa_motion_publish_stop(ctx,0x8c61b962u); r[0]=28u;goto L8c61b964; /* mov #0x1c,r0 */
L8c61b964: sa_motion_publish_stop(ctx,0x8c61b964u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b966; /* fmov.s @(r0,r15),fr3 */
L8c61b966: sa_motion_publish_stop(ctx,0x8c61b966u); r[0]=32u;goto L8c61b968; /* mov #0x20,r0 */
L8c61b968: sa_motion_publish_stop(ctx,0x8c61b968u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61b96a; /* fmov.s @r3,fr2 */
L8c61b96a: sa_motion_publish_stop(ctx,0x8c61b96au); r[3]+=8u;goto L8c61b96c; /* add #0x8,r3 */
L8c61b96c: sa_motion_publish_stop(ctx,0x8c61b96cu); r[2]=rd(m,bus,&io_status,0x8c61ba18u,4);if(io_status)return io_status;goto L8c61b96e; /* mov.l 0x8c61ba18,r2 */
L8c61b96e: sa_motion_publish_stop(ctx,0x8c61b96eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61b970; /* fmul fr3,fr2 */
L8c61b970: sa_motion_publish_stop(ctx,0x8c61b970u); r[1]=rd(m,bus,&io_status,0x8c61ba1cu,4);if(io_status)return io_status;goto L8c61b972; /* mov.l 0x8c61ba1c,r1 */
L8c61b972: sa_motion_publish_stop(ctx,0x8c61b972u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b974; /* fmov.s @r2,fr3 */
L8c61b974: sa_motion_publish_stop(ctx,0x8c61b974u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61b976; /* fmov.s @r1,fr1 */
L8c61b976: sa_motion_publish_stop(ctx,0x8c61b976u); r[2]=rd(m,bus,&io_status,0x8c61ba20u,4);if(io_status)return io_status;goto L8c61b978; /* mov.l 0x8c61ba20,r2 */
L8c61b978: sa_motion_publish_stop(ctx,0x8c61b978u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b97a; /* fsub fr2,fr3 */
L8c61b97a: sa_motion_publish_stop(ctx,0x8c61b97au); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b97c; /* fmov.s @(r0,r15),fr2 */
L8c61b97c: sa_motion_publish_stop(ctx,0x8c61b97cu); r[0]=36u;goto L8c61b97e; /* mov #0x24,r0 */
L8c61b97e: sa_motion_publish_stop(ctx,0x8c61b97eu); r[1]=rd(m,bus,&io_status,0x8c61ba28u,4);if(io_status)return io_status;goto L8c61b980; /* mov.l 0x8c61ba28,r1 */
L8c61b980: sa_motion_publish_stop(ctx,0x8c61b980u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b982; /* fmul fr2,fr1 */
L8c61b982: sa_motion_publish_stop(ctx,0x8c61b982u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b984; /* fmov.s @(r0,r15),fr2 */
L8c61b984: sa_motion_publish_stop(ctx,0x8c61b984u); r[0]=12u;goto L8c61b986; /* mov #0xc,r0 */
L8c61b986: sa_motion_publish_stop(ctx,0x8c61b986u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61b988; /* fsub fr1,fr3 */
L8c61b988: sa_motion_publish_stop(ctx,0x8c61b988u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61b98a; /* fmov.s @r3,fr1 */
L8c61b98a: sa_motion_publish_stop(ctx,0x8c61b98au); r[3]=rd(m,bus,&io_status,0x8c61ba24u,4);if(io_status)return io_status;goto L8c61b98c; /* mov.l 0x8c61ba24,r3 */
L8c61b98c: sa_motion_publish_stop(ctx,0x8c61b98cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61b98e; /* fmul fr2,fr1 */
L8c61b98e: sa_motion_publish_stop(ctx,0x8c61b98eu); fcopy(f,4u,3u);goto L8c61b990; /* fmov fr3,fr4 */
L8c61b990: sa_motion_publish_stop(ctx,0x8c61b990u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b992; /* fmov.s @r2,fr3 */
L8c61b992: sa_motion_publish_stop(ctx,0x8c61b992u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61b994; /* fsub fr1,fr4 */
L8c61b994: sa_motion_publish_stop(ctx,0x8c61b994u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b996; /* fmov fr3,@(r0,r15) */
L8c61b996: sa_motion_publish_stop(ctx,0x8c61b996u); r[0]=16u;goto L8c61b998; /* mov #0x10,r0 */
L8c61b998: sa_motion_publish_stop(ctx,0x8c61b998u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61b99a; /* fmov.s @r3,fr3 */
L8c61b99a: sa_motion_publish_stop(ctx,0x8c61b99au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b99c; /* fmul fr4,fr3 */
L8c61b99c: sa_motion_publish_stop(ctx,0x8c61b99cu); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b99e; /* fmov fr3,@(r0,r15) */
L8c61b99e: sa_motion_publish_stop(ctx,0x8c61b99eu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61b9a0; /* fmov.s @r1,fr3 */
L8c61b9a0: sa_motion_publish_stop(ctx,0x8c61b9a0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b9a2; /* fmul fr4,fr3 */
L8c61b9a2: sa_motion_publish_stop(ctx,0x8c61b9a2u); r[0]=20u;goto L8c61b9a4; /* mov #0x14,r0 */
L8c61b9a4: sa_motion_publish_stop(ctx,0x8c61b9a4u); r[2]+=12u;goto L8c61b9a6; /* add #0xc,r2 */
L8c61b9a6: sa_motion_publish_stop(ctx,0x8c61b9a6u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b9a8; /* fmov fr3,@(r0,r15) */
L8c61b9a8: sa_motion_publish_stop(ctx,0x8c61b9a8u); r[0]=24u;goto L8c61b9aa; /* mov #0x18,r0 */
L8c61b9aa: sa_motion_publish_stop(ctx,0x8c61b9aau); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61b9ac; /* fmov.s @r2,fr3 */
L8c61b9ac: sa_motion_publish_stop(ctx,0x8c61b9acu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61b9ae; /* fmul fr4,fr3 */
L8c61b9ae: sa_motion_publish_stop(ctx,0x8c61b9aeu); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61b9b0; /* fmov fr3,@(r0,r15) */
L8c61b9b0: sa_motion_publish_stop(ctx,0x8c61b9b0u); r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61b9b2; /* mov.l @(0xc,r11),r4 */
L8c61b9b2: sa_motion_publish_stop(ctx,0x8c61b9b2u); r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61b9b4; /* mov.l @(0xc,r13),r3 */
L8c61b9b4: sa_motion_publish_stop(ctx,0x8c61b9b4u); r[0]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61b9b6; /* mov.l @(0xc,r14),r0 */
L8c61b9b6: sa_motion_publish_stop(ctx,0x8c61b9b6u); r[4]&=r[3];goto L8c61b9b8; /* and r3,r4 */
L8c61b9b8: sa_motion_publish_stop(ctx,0x8c61b9b8u); r[3]=1u;goto L8c61b9ba; /* mov #0x1,r3 */
L8c61b9ba: sa_motion_publish_stop(ctx,0x8c61b9bau); r[4]&=r[0];goto L8c61b9bc; /* and r0,r4 */
L8c61b9bc: sa_motion_publish_stop(ctx,0x8c61b9bcu); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61b9be; /* tst r3,r4 */
L8c61b9be: sa_motion_publish_stop(ctx,0x8c61b9beu); if(!(*sr&1u))goto L8c61b9ce;goto L8c61b9c0; /* bf 0x8c61b9ce */
L8c61b9c0: sa_motion_publish_stop(ctx,0x8c61b9c0u); r[1]=rd(m,bus,&io_status,0x8c61ba3cu,4);if(io_status)return io_status;goto L8c61b9c2; /* mov.l 0x8c61ba3c,r1 */
L8c61b9c2: sa_motion_publish_stop(ctx,0x8c61b9c2u); r[7]=r[15];goto L8c61b9c4; /* mov r15,r7 */
L8c61b9c4: sa_motion_publish_stop(ctx,0x8c61b9c4u); r[5]=r[13];goto L8c61b9c6; /* mov r13,r5 */
L8c61b9c6: sa_motion_publish_stop(ctx,0x8c61b9c6u); r[7]+=12u;goto L8c61b9c8; /* add #0xc,r7 */
L8c61b9c8: sa_motion_publish_stop(ctx,0x8c61b9c8u); r[6]=r[14];goto L8c61b9ca; /* mov r14,r6 */
L8c61b9ca: sa_motion_publish_stop(ctx,0x8c61b9ccu); target=r[1];*pr=0x8c61b9ceu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b9ce; /* jsr @r1 */
L8c61b9ce: sa_motion_publish_stop(ctx,0x8c61b9ceu); r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61b9d0; /* mov.l @(0xc,r13),r4 */
L8c61b9d0: sa_motion_publish_stop(ctx,0x8c61b9d0u); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61b9d2; /* mov.l @(0xc,r14),r3 */
L8c61b9d2: sa_motion_publish_stop(ctx,0x8c61b9d2u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61b9d4; /* mov.l @(0xc,r12),r2 */
L8c61b9d4: sa_motion_publish_stop(ctx,0x8c61b9d4u); r[4]&=r[3];goto L8c61b9d6; /* and r3,r4 */
L8c61b9d6: sa_motion_publish_stop(ctx,0x8c61b9d6u); r[3]=1u;goto L8c61b9d8; /* mov #0x1,r3 */
L8c61b9d8: sa_motion_publish_stop(ctx,0x8c61b9d8u); r[4]&=r[2];goto L8c61b9da; /* and r2,r4 */
L8c61b9da: sa_motion_publish_stop(ctx,0x8c61b9dau); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61b9dc; /* tst r3,r4 */
L8c61b9dc: sa_motion_publish_stop(ctx,0x8c61b9dcu); if(!(*sr&1u))goto L8c61b9ec;goto L8c61b9de; /* bf 0x8c61b9ec */
L8c61b9de: sa_motion_publish_stop(ctx,0x8c61b9deu); r[1]=rd(m,bus,&io_status,0x8c61ba3cu,4);if(io_status)return io_status;goto L8c61b9e0; /* mov.l 0x8c61ba3c,r1 */
L8c61b9e0: sa_motion_publish_stop(ctx,0x8c61b9e0u); r[7]=r[15];goto L8c61b9e2; /* mov r15,r7 */
L8c61b9e2: sa_motion_publish_stop(ctx,0x8c61b9e2u); r[5]=r[14];goto L8c61b9e4; /* mov r14,r5 */
L8c61b9e4: sa_motion_publish_stop(ctx,0x8c61b9e4u); r[7]+=12u;goto L8c61b9e6; /* add #0xc,r7 */
L8c61b9e6: sa_motion_publish_stop(ctx,0x8c61b9e6u); r[6]=r[12];goto L8c61b9e8; /* mov r12,r6 */
L8c61b9e8: sa_motion_publish_stop(ctx,0x8c61b9eau); target=r[1];*pr=0x8c61b9ecu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61b9ec; /* jsr @r1 */
L8c61b9ec: sa_motion_publish_stop(ctx,0x8c61b9ecu); r[9]+=1u;goto L8c61b9ee; /* add #0x1,r9 */
L8c61b9ee: sa_motion_publish_stop(ctx,0x8c61b9eeu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61b9f0; /* mov.l @r15,r3 */
L8c61b9f0: sa_motion_publish_stop(ctx,0x8c61b9f0u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61b9f2; /* mov.w @(0x2,r3),r0 */
L8c61b9f2: sa_motion_publish_stop(ctx,0x8c61b9f2u); r[0]=r[0]&65535u;goto L8c61b9f4; /* extu.w r0,r0 */
L8c61b9f4: sa_motion_publish_stop(ctx,0x8c61b9f4u); *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61b9f6; /* cmp/ge r0,r9 */
L8c61b9f6: sa_motion_publish_stop(ctx,0x8c61b9f6u); if((*sr&1u))goto L8c61b9fc;goto L8c61b9f8; /* bt 0x8c61b9fc */
L8c61b9f8: sa_motion_publish_stop(ctx,0x8c61b9fau); goto L8c61b76c; /* bra 0x8c61b76c */
L8c61b9fc: sa_motion_publish_stop(ctx,0x8c61b9feu); goto L8c61bdf0; /* bra 0x8c61bdf0 */
L8c61ba00: sa_motion_publish_stop(ctx,0x8c61ba00u); r[1]=0u;goto L8c61ba02; /* mov #0x0,r1 */
L8c61ba02: sa_motion_publish_stop(ctx,0x8c61ba04u); wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c61bd86; /* bra 0x8c61bd86 */
L8c61ba40: sa_motion_publish_stop(ctx,0x8c61ba40u); r[3]=rd(m,bus,&io_status,0x8c61bc68u,4);if(io_status)return io_status;goto L8c61ba42; /* mov.l 0x8c61bc68,r3 */
L8c61ba42: sa_motion_publish_stop(ctx,0x8c61ba42u); r[7]=0u;goto L8c61ba44; /* mov #0x0,r7 */
L8c61ba44: sa_motion_publish_stop(ctx,0x8c61ba44u); r[9]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61ba46; /* mov.w @r10+,r9 */
L8c61ba46: sa_motion_publish_stop(ctx,0x8c61ba46u); r[4]=0u;goto L8c61ba48; /* mov #0x0,r4 */
L8c61ba48: sa_motion_publish_stop(ctx,0x8c61ba48u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ba4a; /* mov.l @r3,r2 */
L8c61ba4a: sa_motion_publish_stop(ctx,0x8c61ba4au); r[5]=r[7];goto L8c61ba4c; /* mov r7,r5 */
L8c61ba4c: sa_motion_publish_stop(ctx,0x8c61ba4cu); r[9]=r[9]&65535u;goto L8c61ba4e; /* extu.w r9,r9 */
L8c61ba4e: sa_motion_publish_stop(ctx,0x8c61ba4eu); r[2]+=r[9];goto L8c61ba50; /* add r9,r2 */
L8c61ba50: sa_motion_publish_stop(ctx,0x8c61ba50u); *sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c61ba52; /* cmp/pl r9 */
L8c61ba52: sa_motion_publish_stop(ctx,0x8c61ba52u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61ba54; /* mov.l r2,@r3 */
L8c61ba54: sa_motion_publish_stop(ctx,0x8c61ba54u); r[6]=4294967295u;goto L8c61ba56; /* mov #-0x1,r6 */
L8c61ba56: sa_motion_publish_stop(ctx,0x8c61ba58u); take=!(*sr&1u);r[4]+=r[10];if(take)goto L8c61ba78;goto L8c61ba5a; /* bf/s 0x8c61ba78 */
L8c61ba5a: sa_motion_publish_stop(ctx,0x8c61ba5au); r[11]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c61ba5c; /* mov.w @r4+,r11 */
L8c61ba5c: sa_motion_publish_stop(ctx,0x8c61ba5cu); r[5]+=1u;goto L8c61ba5e; /* add #0x1,r5 */
L8c61ba5e: sa_motion_publish_stop(ctx,0x8c61ba5eu); r[2]=rd(m,bus,&io_status,0x8c61bc6cu,4);if(io_status)return io_status;goto L8c61ba60; /* mov.l 0x8c61bc6c,r2 */
L8c61ba60: sa_motion_publish_stop(ctx,0x8c61ba60u); r[11]=r[11]&65535u;goto L8c61ba62; /* extu.w r11,r11 */
L8c61ba62: sa_motion_publish_stop(ctx,0x8c61ba62u); r[11]<<=2;goto L8c61ba64; /* shll2 r11 */
L8c61ba64: sa_motion_publish_stop(ctx,0x8c61ba64u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ba66; /* mov.l @r2,r3 */
L8c61ba66: sa_motion_publish_stop(ctx,0x8c61ba66u); r[11]<<=2;goto L8c61ba68; /* shll2 r11 */
L8c61ba68: sa_motion_publish_stop(ctx,0x8c61ba68u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61ba6a; /* shll r11 */
L8c61ba6a: sa_motion_publish_stop(ctx,0x8c61ba6au); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[9]);goto L8c61ba6c; /* cmp/ge r9,r5 */
L8c61ba6c: sa_motion_publish_stop(ctx,0x8c61ba6cu); r[11]+=r[3];goto L8c61ba6e; /* add r3,r11 */
L8c61ba6e: sa_motion_publish_stop(ctx,0x8c61ba6eu); wr(m,bus,&io_status,r[15]+0x8u,r[11],4);if(io_status)return io_status;goto L8c61ba70; /* mov.l r11,@(0x8,r15) */
L8c61ba70: sa_motion_publish_stop(ctx,0x8c61ba70u); r[11]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61ba72; /* mov.l @(0xc,r11),r11 */
L8c61ba72: sa_motion_publish_stop(ctx,0x8c61ba72u); r[6]&=r[11];goto L8c61ba74; /* and r11,r6 */
L8c61ba74: sa_motion_publish_stop(ctx,0x8c61ba76u); take=!(*sr&1u);r[7]|=r[11];if(take)goto L8c61ba5a;goto L8c61ba78; /* bf/s 0x8c61ba5a */
L8c61ba78: sa_motion_publish_stop(ctx,0x8c61ba78u); r[2]=63u;goto L8c61ba7a; /* mov #0x3f,r2 */
L8c61ba7a: sa_motion_publish_stop(ctx,0x8c61ba7au); *sr=(*sr&~1u)|!(r[2]&r[6]);goto L8c61ba7c; /* tst r2,r6 */
L8c61ba7c: sa_motion_publish_stop(ctx,0x8c61ba7cu); if(!(*sr&1u))goto L8c61badc;goto L8c61ba7e; /* bf 0x8c61badc */
L8c61ba7e: sa_motion_publish_stop(ctx,0x8c61ba7eu); r[1]=1u;goto L8c61ba80; /* mov #0x1,r1 */
L8c61ba80: sa_motion_publish_stop(ctx,0x8c61ba80u); *sr=(*sr&~1u)|!(r[1]&r[7]);goto L8c61ba82; /* tst r1,r7 */
L8c61ba82: sa_motion_publish_stop(ctx,0x8c61ba82u); if((*sr&1u))goto L8c61ba88;goto L8c61ba84; /* bt 0x8c61ba88 */
L8c61ba84: sa_motion_publish_stop(ctx,0x8c61ba86u); goto L8c61bc98; /* bra 0x8c61bc98 */
L8c61ba88: sa_motion_publish_stop(ctx,0x8c61ba88u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ba8a; /* mov.w @(0x2,r10),r0 */
L8c61ba8a: sa_motion_publish_stop(ctx,0x8c61ba8au); r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ba8c; /* mov.w @r10,r12 */
L8c61ba8c: sa_motion_publish_stop(ctx,0x8c61ba8cu); r[3]=rd(m,bus,&io_status,0x8c61bc6cu,4);if(io_status)return io_status;goto L8c61ba8e; /* mov.l 0x8c61bc6c,r3 */
L8c61ba8e: sa_motion_publish_stop(ctx,0x8c61ba8eu); r[14]=r[0]&65535u;goto L8c61ba90; /* extu.w r0,r14 */
L8c61ba90: sa_motion_publish_stop(ctx,0x8c61ba90u); r[14]<<=2;goto L8c61ba92; /* shll2 r14 */
L8c61ba92: sa_motion_publish_stop(ctx,0x8c61ba92u); r[0]=sx(m,bus,&io_status,r[10]+0x4u,2);if(io_status)return io_status;goto L8c61ba94; /* mov.w @(0x4,r10),r0 */
L8c61ba94: sa_motion_publish_stop(ctx,0x8c61ba94u); r[12]=r[12]&65535u;goto L8c61ba96; /* extu.w r12,r12 */
L8c61ba96: sa_motion_publish_stop(ctx,0x8c61ba96u); r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ba98; /* mov.l @r3,r4 */
L8c61ba98: sa_motion_publish_stop(ctx,0x8c61ba98u); r[12]<<=2;goto L8c61ba9a; /* shll2 r12 */
L8c61ba9a: sa_motion_publish_stop(ctx,0x8c61ba9au); r[14]<<=2;goto L8c61ba9c; /* shll2 r14 */
L8c61ba9c: sa_motion_publish_stop(ctx,0x8c61ba9cu); r[13]=r[0]&65535u;goto L8c61ba9e; /* extu.w r0,r13 */
L8c61ba9e: sa_motion_publish_stop(ctx,0x8c61ba9eu); r[12]<<=2;goto L8c61baa0; /* shll2 r12 */
L8c61baa0: sa_motion_publish_stop(ctx,0x8c61baa0u); r[0]=16u;goto L8c61baa2; /* mov #0x10,r0 */
L8c61baa2: sa_motion_publish_stop(ctx,0x8c61baa2u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61baa4; /* shll r12 */
L8c61baa4: sa_motion_publish_stop(ctx,0x8c61baa4u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61baa6; /* shll r14 */
L8c61baa6: sa_motion_publish_stop(ctx,0x8c61baa6u); r[14]+=r[4];goto L8c61baa8; /* add r4,r14 */
L8c61baa8: sa_motion_publish_stop(ctx,0x8c61baa8u); fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c61baaa; /* fmov.s @(r0,r14),fr5 */
L8c61baaa: sa_motion_publish_stop(ctx,0x8c61baaau); r[13]<<=2;goto L8c61baac; /* shll2 r13 */
L8c61baac: sa_motion_publish_stop(ctx,0x8c61baacu); r[12]+=r[4];goto L8c61baae; /* add r4,r12 */
L8c61baae: sa_motion_publish_stop(ctx,0x8c61baaeu); r[13]<<=2;goto L8c61bab0; /* shll2 r13 */
L8c61bab0: sa_motion_publish_stop(ctx,0x8c61bab0u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bab2; /* fmov.s @(r0,r12),fr3 */
L8c61bab2: sa_motion_publish_stop(ctx,0x8c61bab2u); fcopy(f,2u,5u);goto L8c61bab4; /* fmov fr5,fr2 */
L8c61bab4: sa_motion_publish_stop(ctx,0x8c61bab4u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61bab6; /* shll r13 */
L8c61bab6: sa_motion_publish_stop(ctx,0x8c61bab6u); r[0]=20u;goto L8c61bab8; /* mov #0x14,r0 */
L8c61bab8: sa_motion_publish_stop(ctx,0x8c61bab8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61baba; /* fsub fr3,fr2 */
L8c61baba: sa_motion_publish_stop(ctx,0x8c61babau); r[13]+=r[4];goto L8c61babc; /* add r4,r13 */
L8c61babc: sa_motion_publish_stop(ctx,0x8c61babcu); fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c61babe; /* fmov.s @(r0,r14),fr4 */
L8c61babe: sa_motion_publish_stop(ctx,0x8c61babeu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bac0; /* fmov.s @(r0,r13),fr3 */
L8c61bac0: sa_motion_publish_stop(ctx,0x8c61bac0u); fcopy(f,1u,4u);goto L8c61bac2; /* fmov fr4,fr1 */
L8c61bac2: sa_motion_publish_stop(ctx,0x8c61bac2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bac4; /* fsub fr4,fr3 */
L8c61bac4: sa_motion_publish_stop(ctx,0x8c61bac4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61bac6; /* fmul fr3,fr2 */
L8c61bac6: sa_motion_publish_stop(ctx,0x8c61bac6u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bac8; /* fmov.s @(r0,r12),fr3 */
L8c61bac8: sa_motion_publish_stop(ctx,0x8c61bac8u); r[0]=16u;goto L8c61baca; /* mov #0x10,r0 */
L8c61baca: sa_motion_publish_stop(ctx,0x8c61bacau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61bacc; /* fsub fr3,fr1 */
L8c61bacc: sa_motion_publish_stop(ctx,0x8c61baccu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bace; /* fmov.s @(r0,r13),fr3 */
L8c61bace: sa_motion_publish_stop(ctx,0x8c61baceu); fcopy(f,4u,2u);goto L8c61bad0; /* fmov fr2,fr4 */
L8c61bad0: sa_motion_publish_stop(ctx,0x8c61bad0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bad2; /* fsub fr5,fr3 */
L8c61bad2: sa_motion_publish_stop(ctx,0x8c61bad2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bad4; /* fmul fr3,fr1 */
L8c61bad4: sa_motion_publish_stop(ctx,0x8c61bad4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61bad6; /* fldi0 fr3 */
L8c61bad6: sa_motion_publish_stop(ctx,0x8c61bad6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61bad8; /* fsub fr1,fr4 */
L8c61bad8: sa_motion_publish_stop(ctx,0x8c61bad8u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61bada; /* fcmp/gt fr3,fr4 */
L8c61bada: sa_motion_publish_stop(ctx,0x8c61badau); if(!(*sr&1u))goto L8c61bae2;goto L8c61badc; /* bf 0x8c61bae2 */
L8c61badc: sa_motion_publish_stop(ctx,0x8c61badcu); *sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c61bade; /* shll r9 */
L8c61bade: sa_motion_publish_stop(ctx,0x8c61bae0u); r[10]+=r[9];goto L8c61bd80; /* bra 0x8c61bd80 */
L8c61bae2: sa_motion_publish_stop(ctx,0x8c61bae2u); r[0]=28u;goto L8c61bae4; /* mov #0x1c,r0 */
L8c61bae4: sa_motion_publish_stop(ctx,0x8c61bae4u); r[2]=rd(m,bus,&io_status,0x8c61bc70u,4);if(io_status)return io_status;goto L8c61bae6; /* mov.l 0x8c61bc70,r2 */
L8c61bae6: sa_motion_publish_stop(ctx,0x8c61bae6u); r[4]=r[15];goto L8c61bae8; /* mov r15,r4 */
L8c61bae8: sa_motion_publish_stop(ctx,0x8c61bae8u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61baea; /* mov.l @r2,r3 */
L8c61baea: sa_motion_publish_stop(ctx,0x8c61baeau); r[3]+=r[9];goto L8c61baec; /* add r9,r3 */
L8c61baec: sa_motion_publish_stop(ctx,0x8c61baecu); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61baee; /* mov.l r3,@r2 */
L8c61baee: sa_motion_publish_stop(ctx,0x8c61baeeu); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61baf0; /* fmov.s @r14,fr3 */
L8c61baf0: sa_motion_publish_stop(ctx,0x8c61baf0u); fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61baf2; /* fmov.s @r12,fr2 */
L8c61baf2: sa_motion_publish_stop(ctx,0x8c61baf2u); r[3]=rd(m,bus,&io_status,0x8c61bc74u,4);if(io_status)return io_status;goto L8c61baf4; /* mov.l 0x8c61bc74,r3 */
L8c61baf4: sa_motion_publish_stop(ctx,0x8c61baf4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61baf6; /* fadd fr3,fr2 */
L8c61baf6: sa_motion_publish_stop(ctx,0x8c61baf6u); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61baf8; /* fmov.s @r13,fr3 */
L8c61baf8: sa_motion_publish_stop(ctx,0x8c61baf8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bafa; /* fadd fr3,fr2 */
L8c61bafa: sa_motion_publish_stop(ctx,0x8c61bafau); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bafc; /* fmov fr2,@(r0,r15) */
L8c61bafc: sa_motion_publish_stop(ctx,0x8c61bafcu); r[0]=4u;goto L8c61bafe; /* mov #0x4,r0 */
L8c61bafe: sa_motion_publish_stop(ctx,0x8c61bafeu); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bb00; /* fmov.s @(r0,r12),fr2 */
L8c61bb00: sa_motion_publish_stop(ctx,0x8c61bb00u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bb02; /* fmov.s @(r0,r14),fr3 */
L8c61bb02: sa_motion_publish_stop(ctx,0x8c61bb02u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bb04; /* fadd fr3,fr2 */
L8c61bb04: sa_motion_publish_stop(ctx,0x8c61bb04u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bb06; /* fmov.s @(r0,r13),fr3 */
L8c61bb06: sa_motion_publish_stop(ctx,0x8c61bb06u); r[0]=32u;goto L8c61bb08; /* mov #0x20,r0 */
L8c61bb08: sa_motion_publish_stop(ctx,0x8c61bb08u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bb0a; /* fadd fr3,fr2 */
L8c61bb0a: sa_motion_publish_stop(ctx,0x8c61bb0au); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb0c; /* fmov fr2,@(r0,r15) */
L8c61bb0c: sa_motion_publish_stop(ctx,0x8c61bb0cu); r[0]=8u;goto L8c61bb0e; /* mov #0x8,r0 */
L8c61bb0e: sa_motion_publish_stop(ctx,0x8c61bb0eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bb10; /* fmov.s @(r0,r14),fr3 */
L8c61bb10: sa_motion_publish_stop(ctx,0x8c61bb10u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bb12; /* fmov.s @(r0,r12),fr2 */
L8c61bb12: sa_motion_publish_stop(ctx,0x8c61bb12u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bb14; /* fadd fr3,fr2 */
L8c61bb14: sa_motion_publish_stop(ctx,0x8c61bb14u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bb16; /* fmov.s @(r0,r13),fr3 */
L8c61bb16: sa_motion_publish_stop(ctx,0x8c61bb16u); r[0]=36u;goto L8c61bb18; /* mov #0x24,r0 */
L8c61bb18: sa_motion_publish_stop(ctx,0x8c61bb18u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bb1a; /* fadd fr3,fr2 */
L8c61bb1a: sa_motion_publish_stop(ctx,0x8c61bb1au); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb1c; /* fmov fr2,@(r0,r15) */
L8c61bb1c: sa_motion_publish_stop(ctx,0x8c61bb1eu); target=r[3];*pr=0x8c61bb20u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61bb20; /* jsr @r3 */
L8c61bb20: sa_motion_publish_stop(ctx,0x8c61bb20u); r[3]=rd(m,bus,&io_status,0x8c61bc78u,4);if(io_status)return io_status;goto L8c61bb22; /* mov.l 0x8c61bc78,r3 */
L8c61bb22: sa_motion_publish_stop(ctx,0x8c61bb22u); r[0]=28u;goto L8c61bb24; /* mov #0x1c,r0 */
L8c61bb24: sa_motion_publish_stop(ctx,0x8c61bb24u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb26; /* fmov.s @(r0,r15),fr3 */
L8c61bb26: sa_motion_publish_stop(ctx,0x8c61bb26u); r[0]=32u;goto L8c61bb28; /* mov #0x20,r0 */
L8c61bb28: sa_motion_publish_stop(ctx,0x8c61bb28u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61bb2a; /* fmov.s @r3,fr2 */
L8c61bb2a: sa_motion_publish_stop(ctx,0x8c61bb2au); r[3]+=8u;goto L8c61bb2c; /* add #0x8,r3 */
L8c61bb2c: sa_motion_publish_stop(ctx,0x8c61bb2cu); r[2]=rd(m,bus,&io_status,0x8c61bc7cu,4);if(io_status)return io_status;goto L8c61bb2e; /* mov.l 0x8c61bc7c,r2 */
L8c61bb2e: sa_motion_publish_stop(ctx,0x8c61bb2eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61bb30; /* fmul fr3,fr2 */
L8c61bb30: sa_motion_publish_stop(ctx,0x8c61bb30u); r[1]=rd(m,bus,&io_status,0x8c61bc80u,4);if(io_status)return io_status;goto L8c61bb32; /* mov.l 0x8c61bc80,r1 */
L8c61bb32: sa_motion_publish_stop(ctx,0x8c61bb32u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61bb34; /* fmov.s @r2,fr3 */
L8c61bb34: sa_motion_publish_stop(ctx,0x8c61bb34u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61bb36; /* fmov.s @r1,fr1 */
L8c61bb36: sa_motion_publish_stop(ctx,0x8c61bb36u); r[2]=rd(m,bus,&io_status,0x8c61bc84u,4);if(io_status)return io_status;goto L8c61bb38; /* mov.l 0x8c61bc84,r2 */
L8c61bb38: sa_motion_publish_stop(ctx,0x8c61bb38u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bb3a; /* fsub fr2,fr3 */
L8c61bb3a: sa_motion_publish_stop(ctx,0x8c61bb3au); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb3c; /* fmov.s @(r0,r15),fr2 */
L8c61bb3c: sa_motion_publish_stop(ctx,0x8c61bb3cu); r[0]=36u;goto L8c61bb3e; /* mov #0x24,r0 */
L8c61bb3e: sa_motion_publish_stop(ctx,0x8c61bb3eu); r[1]=rd(m,bus,&io_status,0x8c61bc8cu,4);if(io_status)return io_status;goto L8c61bb40; /* mov.l 0x8c61bc8c,r1 */
L8c61bb40: sa_motion_publish_stop(ctx,0x8c61bb40u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bb42; /* fmul fr2,fr1 */
L8c61bb42: sa_motion_publish_stop(ctx,0x8c61bb42u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb44; /* fmov.s @(r0,r15),fr2 */
L8c61bb44: sa_motion_publish_stop(ctx,0x8c61bb44u); r[0]=12u;goto L8c61bb46; /* mov #0xc,r0 */
L8c61bb46: sa_motion_publish_stop(ctx,0x8c61bb46u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bb48; /* fsub fr1,fr3 */
L8c61bb48: sa_motion_publish_stop(ctx,0x8c61bb48u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61bb4a; /* fmov.s @r3,fr1 */
L8c61bb4a: sa_motion_publish_stop(ctx,0x8c61bb4au); r[3]=rd(m,bus,&io_status,0x8c61bc88u,4);if(io_status)return io_status;goto L8c61bb4c; /* mov.l 0x8c61bc88,r3 */
L8c61bb4c: sa_motion_publish_stop(ctx,0x8c61bb4cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bb4e; /* fmul fr2,fr1 */
L8c61bb4e: sa_motion_publish_stop(ctx,0x8c61bb4eu); fcopy(f,4u,3u);goto L8c61bb50; /* fmov fr3,fr4 */
L8c61bb50: sa_motion_publish_stop(ctx,0x8c61bb50u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61bb52; /* fmov.s @r2,fr3 */
L8c61bb52: sa_motion_publish_stop(ctx,0x8c61bb52u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61bb54; /* fsub fr1,fr4 */
L8c61bb54: sa_motion_publish_stop(ctx,0x8c61bb54u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb56; /* fmov fr3,@(r0,r15) */
L8c61bb56: sa_motion_publish_stop(ctx,0x8c61bb56u); r[0]=16u;goto L8c61bb58; /* mov #0x10,r0 */
L8c61bb58: sa_motion_publish_stop(ctx,0x8c61bb58u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61bb5a; /* fmov.s @r3,fr3 */
L8c61bb5a: sa_motion_publish_stop(ctx,0x8c61bb5au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bb5c; /* fmul fr4,fr3 */
L8c61bb5c: sa_motion_publish_stop(ctx,0x8c61bb5cu); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb5e; /* fmov fr3,@(r0,r15) */
L8c61bb5e: sa_motion_publish_stop(ctx,0x8c61bb5eu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61bb60; /* fmov.s @r1,fr3 */
L8c61bb60: sa_motion_publish_stop(ctx,0x8c61bb60u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bb62; /* fmul fr4,fr3 */
L8c61bb62: sa_motion_publish_stop(ctx,0x8c61bb62u); r[0]=20u;goto L8c61bb64; /* mov #0x14,r0 */
L8c61bb64: sa_motion_publish_stop(ctx,0x8c61bb64u); r[2]+=12u;goto L8c61bb66; /* add #0xc,r2 */
L8c61bb66: sa_motion_publish_stop(ctx,0x8c61bb66u); r[8]+=16u;goto L8c61bb68; /* add #0x10,r8 */
L8c61bb68: sa_motion_publish_stop(ctx,0x8c61bb68u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb6a; /* fmov fr3,@(r0,r15) */
L8c61bb6a: sa_motion_publish_stop(ctx,0x8c61bb6au); r[0]=24u;goto L8c61bb6c; /* mov #0x18,r0 */
L8c61bb6c: sa_motion_publish_stop(ctx,0x8c61bb6cu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61bb6e; /* fmov.s @r2,fr3 */
L8c61bb6e: sa_motion_publish_stop(ctx,0x8c61bb6eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bb70; /* fmul fr4,fr3 */
L8c61bb70: sa_motion_publish_stop(ctx,0x8c61bb70u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bb72; /* fmov fr3,@(r0,r15) */
L8c61bb72: sa_motion_publish_stop(ctx,0x8c61bb72u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61bb74; /* mov.w @r10+,r4 */
L8c61bb74: sa_motion_publish_stop(ctx,0x8c61bb74u); r[0]=rd(m,bus,&io_status,0x8c61bc6cu,4);if(io_status)return io_status;goto L8c61bb76; /* mov.l 0x8c61bc6c,r0 */
L8c61bb76: sa_motion_publish_stop(ctx,0x8c61bb76u); r[4]=r[4]&65535u;goto L8c61bb78; /* extu.w r4,r4 */
L8c61bb78: sa_motion_publish_stop(ctx,0x8c61bb78u); r[4]<<=2;goto L8c61bb7a; /* shll2 r4 */
L8c61bb7a: sa_motion_publish_stop(ctx,0x8c61bb7au); r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61bb7c; /* mov.l @r0,r3 */
L8c61bb7c: sa_motion_publish_stop(ctx,0x8c61bb7cu); r[4]<<=2;goto L8c61bb7e; /* shll2 r4 */
L8c61bb7e: sa_motion_publish_stop(ctx,0x8c61bb7eu); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61bb80; /* shll r4 */
L8c61bb80: sa_motion_publish_stop(ctx,0x8c61bb80u); r[0]=24u;goto L8c61bb82; /* mov #0x18,r0 */
L8c61bb82: sa_motion_publish_stop(ctx,0x8c61bb82u); r[4]+=r[3];goto L8c61bb84; /* add r3,r4 */
L8c61bb84: sa_motion_publish_stop(ctx,0x8c61bb84u); r[3]=rd(m,bus,&io_status,0x8c61bc90u,4);if(io_status)return io_status;goto L8c61bb86; /* mov.l 0x8c61bc90,r3 */
L8c61bb86: sa_motion_publish_stop(ctx,0x8c61bb86u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bb88; /* fmov.s @(r0,r4),fr3 */
L8c61bb88: sa_motion_publish_stop(ctx,0x8c61bb88u); r[0]=20u;goto L8c61bb8a; /* mov #0x14,r0 */
L8c61bb8a: sa_motion_publish_stop(ctx,0x8c61bb8au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bb8c; /* fmov.s fr3,@-r8 */
L8c61bb8c: sa_motion_publish_stop(ctx,0x8c61bb8cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bb8e; /* fmov.s @(r0,r4),fr3 */
L8c61bb8e: sa_motion_publish_stop(ctx,0x8c61bb8eu); r[0]=16u;goto L8c61bb90; /* mov #0x10,r0 */
L8c61bb90: sa_motion_publish_stop(ctx,0x8c61bb90u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bb92; /* fmov.s fr3,@-r8 */
L8c61bb92: sa_motion_publish_stop(ctx,0x8c61bb92u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bb94; /* fmov.s @(r0,r4),fr3 */
L8c61bb94: sa_motion_publish_stop(ctx,0x8c61bb94u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bb96; /* fmov.s fr3,@-r8 */
L8c61bb96: sa_motion_publish_stop(ctx,0x8c61bb96u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61bb98; /* mov.l r3,@-r8 */
L8c61bb98: sa_motion_publish_stop(ctx,0x8c61bb98u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61bb9a; /* pref @r8 */
L8c61bb9a: sa_motion_publish_stop(ctx,0x8c61bb9au); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61bb9c; /* mov.w @r10+,r4 */
L8c61bb9c: sa_motion_publish_stop(ctx,0x8c61bb9cu); r[0]=24u;goto L8c61bb9e; /* mov #0x18,r0 */
L8c61bb9e: sa_motion_publish_stop(ctx,0x8c61bb9eu); r[2]=rd(m,bus,&io_status,0x8c61bc6cu,4);if(io_status)return io_status;goto L8c61bba0; /* mov.l 0x8c61bc6c,r2 */
L8c61bba0: sa_motion_publish_stop(ctx,0x8c61bba0u); r[8]+=48u;goto L8c61bba2; /* add #0x30,r8 */
L8c61bba2: sa_motion_publish_stop(ctx,0x8c61bba2u); r[4]=r[4]&65535u;goto L8c61bba4; /* extu.w r4,r4 */
L8c61bba4: sa_motion_publish_stop(ctx,0x8c61bba4u); r[4]<<=2;goto L8c61bba6; /* shll2 r4 */
L8c61bba6: sa_motion_publish_stop(ctx,0x8c61bba6u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61bba8; /* mov.l @r2,r3 */
L8c61bba8: sa_motion_publish_stop(ctx,0x8c61bba8u); r[4]<<=2;goto L8c61bbaa; /* shll2 r4 */
L8c61bbaa: sa_motion_publish_stop(ctx,0x8c61bbaau); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61bbac; /* shll r4 */
L8c61bbac: sa_motion_publish_stop(ctx,0x8c61bbacu); r[4]+=r[3];goto L8c61bbae; /* add r3,r4 */
L8c61bbae: sa_motion_publish_stop(ctx,0x8c61bbaeu); r[3]=rd(m,bus,&io_status,0x8c61bc90u,4);if(io_status)return io_status;goto L8c61bbb0; /* mov.l 0x8c61bc90,r3 */
L8c61bbb0: sa_motion_publish_stop(ctx,0x8c61bbb0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bbb2; /* fmov.s @(r0,r4),fr3 */
L8c61bbb2: sa_motion_publish_stop(ctx,0x8c61bbb2u); r[0]=20u;goto L8c61bbb4; /* mov #0x14,r0 */
L8c61bbb4: sa_motion_publish_stop(ctx,0x8c61bbb4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bbb6; /* fmov.s fr3,@-r8 */
L8c61bbb6: sa_motion_publish_stop(ctx,0x8c61bbb6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bbb8; /* fmov.s @(r0,r4),fr3 */
L8c61bbb8: sa_motion_publish_stop(ctx,0x8c61bbb8u); r[0]=16u;goto L8c61bbba; /* mov #0x10,r0 */
L8c61bbba: sa_motion_publish_stop(ctx,0x8c61bbbau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bbbc; /* fmov.s fr3,@-r8 */
L8c61bbbc: sa_motion_publish_stop(ctx,0x8c61bbbcu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bbbe; /* fmov.s @(r0,r4),fr3 */
L8c61bbbe: sa_motion_publish_stop(ctx,0x8c61bbbeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bbc0; /* fmov.s fr3,@-r8 */
L8c61bbc0: sa_motion_publish_stop(ctx,0x8c61bbc0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61bbc2; /* mov.l r3,@-r8 */
L8c61bbc2: sa_motion_publish_stop(ctx,0x8c61bbc2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61bbc4; /* pref @r8 */
L8c61bbc4: sa_motion_publish_stop(ctx,0x8c61bbc4u); r[6]=r[9];goto L8c61bbc6; /* mov r9,r6 */
L8c61bbc6: sa_motion_publish_stop(ctx,0x8c61bbc6u); r[4]=2u;goto L8c61bbc8; /* mov #0x2,r4 */
L8c61bbc8: sa_motion_publish_stop(ctx,0x8c61bbc8u); r[6]+=4294967295u;goto L8c61bbca; /* add #-0x1,r6 */
L8c61bbca: sa_motion_publish_stop(ctx,0x8c61bbcau); r[5]=r[4];goto L8c61bbcc; /* mov r4,r5 */
L8c61bbcc: sa_motion_publish_stop(ctx,0x8c61bbccu); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61bbce; /* cmp/ge r6,r5 */
L8c61bbce: sa_motion_publish_stop(ctx,0x8c61bbd0u); take=(*sr&1u);r[8]+=32u;if(take)goto L8c61bc1c;goto L8c61bbd2; /* bt/s 0x8c61bc1c */
L8c61bbd2: sa_motion_publish_stop(ctx,0x8c61bbd2u); r[0]=24u;goto L8c61bbd4; /* mov #0x18,r0 */
L8c61bbd4: sa_motion_publish_stop(ctx,0x8c61bbd4u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61bbd6; /* mov.w @r10+,r4 */
L8c61bbd6: sa_motion_publish_stop(ctx,0x8c61bbd6u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bbd8; /* fmov.s @(r0,r15),fr3 */
L8c61bbd8: sa_motion_publish_stop(ctx,0x8c61bbd8u); r[8]+=32u;goto L8c61bbda; /* add #0x20,r8 */
L8c61bbda: sa_motion_publish_stop(ctx,0x8c61bbdau); r[0]=20u;goto L8c61bbdc; /* mov #0x14,r0 */
L8c61bbdc: sa_motion_publish_stop(ctx,0x8c61bbdcu); r[2]=rd(m,bus,&io_status,0x8c61bc6cu,4);if(io_status)return io_status;goto L8c61bbde; /* mov.l 0x8c61bc6c,r2 */
L8c61bbde: sa_motion_publish_stop(ctx,0x8c61bbdeu); r[4]=r[4]&65535u;goto L8c61bbe0; /* extu.w r4,r4 */
L8c61bbe0: sa_motion_publish_stop(ctx,0x8c61bbe0u); r[4]<<=2;goto L8c61bbe2; /* shll2 r4 */
L8c61bbe2: sa_motion_publish_stop(ctx,0x8c61bbe2u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61bbe4; /* mov.l @r2,r3 */
L8c61bbe4: sa_motion_publish_stop(ctx,0x8c61bbe4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bbe6; /* fmov.s fr3,@-r8 */
L8c61bbe6: sa_motion_publish_stop(ctx,0x8c61bbe6u); r[4]<<=2;goto L8c61bbe8; /* shll2 r4 */
L8c61bbe8: sa_motion_publish_stop(ctx,0x8c61bbe8u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bbea; /* fmov.s @(r0,r15),fr3 */
L8c61bbea: sa_motion_publish_stop(ctx,0x8c61bbeau); r[0]=16u;goto L8c61bbec; /* mov #0x10,r0 */
L8c61bbec: sa_motion_publish_stop(ctx,0x8c61bbecu); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61bbee; /* shll r4 */
L8c61bbee: sa_motion_publish_stop(ctx,0x8c61bbeeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bbf0; /* fmov.s fr3,@-r8 */
L8c61bbf0: sa_motion_publish_stop(ctx,0x8c61bbf0u); r[4]+=r[3];goto L8c61bbf2; /* add r3,r4 */
L8c61bbf2: sa_motion_publish_stop(ctx,0x8c61bbf2u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bbf4; /* fmov.s @(r0,r15),fr3 */
L8c61bbf4: sa_motion_publish_stop(ctx,0x8c61bbf4u); r[0]=12u;goto L8c61bbf6; /* mov #0xc,r0 */
L8c61bbf6: sa_motion_publish_stop(ctx,0x8c61bbf6u); r[3]=rd(m,bus,&io_status,0x8c61bc90u,4);if(io_status)return io_status;goto L8c61bbf8; /* mov.l 0x8c61bc90,r3 */
L8c61bbf8: sa_motion_publish_stop(ctx,0x8c61bbf8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bbfa; /* fmov.s fr3,@-r8 */
L8c61bbfa: sa_motion_publish_stop(ctx,0x8c61bbfau); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bbfc; /* fmov.s @(r0,r15),fr3 */
L8c61bbfc: sa_motion_publish_stop(ctx,0x8c61bbfcu); r[0]=24u;goto L8c61bbfe; /* mov #0x18,r0 */
L8c61bbfe: sa_motion_publish_stop(ctx,0x8c61bbfeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc00; /* fmov.s fr3,@-r8 */
L8c61bc00: sa_motion_publish_stop(ctx,0x8c61bc00u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bc02; /* fmov.s @(r0,r4),fr3 */
L8c61bc02: sa_motion_publish_stop(ctx,0x8c61bc02u); r[0]=20u;goto L8c61bc04; /* mov #0x14,r0 */
L8c61bc04: sa_motion_publish_stop(ctx,0x8c61bc04u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc06; /* fmov.s fr3,@-r8 */
L8c61bc06: sa_motion_publish_stop(ctx,0x8c61bc06u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bc08; /* fmov.s @(r0,r4),fr3 */
L8c61bc08: sa_motion_publish_stop(ctx,0x8c61bc08u); r[0]=16u;goto L8c61bc0a; /* mov #0x10,r0 */
L8c61bc0a: sa_motion_publish_stop(ctx,0x8c61bc0au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc0c; /* fmov.s fr3,@-r8 */
L8c61bc0c: sa_motion_publish_stop(ctx,0x8c61bc0cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bc0e; /* fmov.s @(r0,r4),fr3 */
L8c61bc0e: sa_motion_publish_stop(ctx,0x8c61bc0eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc10; /* fmov.s fr3,@-r8 */
L8c61bc10: sa_motion_publish_stop(ctx,0x8c61bc10u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61bc12; /* mov.l r3,@-r8 */
L8c61bc12: sa_motion_publish_stop(ctx,0x8c61bc12u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61bc14; /* pref @r8 */
L8c61bc14: sa_motion_publish_stop(ctx,0x8c61bc14u); r[5]+=1u;goto L8c61bc16; /* add #0x1,r5 */
L8c61bc16: sa_motion_publish_stop(ctx,0x8c61bc16u); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61bc18; /* cmp/ge r6,r5 */
L8c61bc18: sa_motion_publish_stop(ctx,0x8c61bc1au); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61bbd2;goto L8c61bc1c; /* bf/s 0x8c61bbd2 */
L8c61bc1c: sa_motion_publish_stop(ctx,0x8c61bc1cu); r[0]=24u;goto L8c61bc1e; /* mov #0x18,r0 */
L8c61bc1e: sa_motion_publish_stop(ctx,0x8c61bc1eu); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61bc20; /* mov.w @r10+,r4 */
L8c61bc20: sa_motion_publish_stop(ctx,0x8c61bc20u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bc22; /* fmov.s @(r0,r15),fr3 */
L8c61bc22: sa_motion_publish_stop(ctx,0x8c61bc22u); r[8]+=32u;goto L8c61bc24; /* add #0x20,r8 */
L8c61bc24: sa_motion_publish_stop(ctx,0x8c61bc24u); r[0]=20u;goto L8c61bc26; /* mov #0x14,r0 */
L8c61bc26: sa_motion_publish_stop(ctx,0x8c61bc26u); r[2]=rd(m,bus,&io_status,0x8c61bc6cu,4);if(io_status)return io_status;goto L8c61bc28; /* mov.l 0x8c61bc6c,r2 */
L8c61bc28: sa_motion_publish_stop(ctx,0x8c61bc28u); r[4]=r[4]&65535u;goto L8c61bc2a; /* extu.w r4,r4 */
L8c61bc2a: sa_motion_publish_stop(ctx,0x8c61bc2au); r[4]<<=2;goto L8c61bc2c; /* shll2 r4 */
L8c61bc2c: sa_motion_publish_stop(ctx,0x8c61bc2cu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61bc2e; /* mov.l @r2,r3 */
L8c61bc2e: sa_motion_publish_stop(ctx,0x8c61bc2eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc30; /* fmov.s fr3,@-r8 */
L8c61bc30: sa_motion_publish_stop(ctx,0x8c61bc30u); r[4]<<=2;goto L8c61bc32; /* shll2 r4 */
L8c61bc32: sa_motion_publish_stop(ctx,0x8c61bc32u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bc34; /* fmov.s @(r0,r15),fr3 */
L8c61bc34: sa_motion_publish_stop(ctx,0x8c61bc34u); r[0]=16u;goto L8c61bc36; /* mov #0x10,r0 */
L8c61bc36: sa_motion_publish_stop(ctx,0x8c61bc36u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61bc38; /* shll r4 */
L8c61bc38: sa_motion_publish_stop(ctx,0x8c61bc38u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc3a; /* fmov.s fr3,@-r8 */
L8c61bc3a: sa_motion_publish_stop(ctx,0x8c61bc3au); r[4]+=r[3];goto L8c61bc3c; /* add r3,r4 */
L8c61bc3c: sa_motion_publish_stop(ctx,0x8c61bc3cu); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bc3e; /* fmov.s @(r0,r15),fr3 */
L8c61bc3e: sa_motion_publish_stop(ctx,0x8c61bc3eu); r[0]=12u;goto L8c61bc40; /* mov #0xc,r0 */
L8c61bc40: sa_motion_publish_stop(ctx,0x8c61bc40u); r[3]=rd(m,bus,&io_status,0x8c61bc94u,4);if(io_status)return io_status;goto L8c61bc42; /* mov.l 0x8c61bc94,r3 */
L8c61bc42: sa_motion_publish_stop(ctx,0x8c61bc42u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc44; /* fmov.s fr3,@-r8 */
L8c61bc44: sa_motion_publish_stop(ctx,0x8c61bc44u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bc46; /* fmov.s @(r0,r15),fr3 */
L8c61bc46: sa_motion_publish_stop(ctx,0x8c61bc46u); r[0]=24u;goto L8c61bc48; /* mov #0x18,r0 */
L8c61bc48: sa_motion_publish_stop(ctx,0x8c61bc48u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc4a; /* fmov.s fr3,@-r8 */
L8c61bc4a: sa_motion_publish_stop(ctx,0x8c61bc4au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bc4c; /* fmov.s @(r0,r4),fr3 */
L8c61bc4c: sa_motion_publish_stop(ctx,0x8c61bc4cu); r[0]=20u;goto L8c61bc4e; /* mov #0x14,r0 */
L8c61bc4e: sa_motion_publish_stop(ctx,0x8c61bc4eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc50; /* fmov.s fr3,@-r8 */
L8c61bc50: sa_motion_publish_stop(ctx,0x8c61bc50u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bc52; /* fmov.s @(r0,r4),fr3 */
L8c61bc52: sa_motion_publish_stop(ctx,0x8c61bc52u); r[0]=16u;goto L8c61bc54; /* mov #0x10,r0 */
L8c61bc54: sa_motion_publish_stop(ctx,0x8c61bc54u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc56; /* fmov.s fr3,@-r8 */
L8c61bc56: sa_motion_publish_stop(ctx,0x8c61bc56u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61bc58; /* fmov.s @(r0,r4),fr3 */
L8c61bc58: sa_motion_publish_stop(ctx,0x8c61bc58u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bc5a; /* fmov.s fr3,@-r8 */
L8c61bc5a: sa_motion_publish_stop(ctx,0x8c61bc5au); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61bc5c; /* mov.l r3,@-r8 */
L8c61bc5c: sa_motion_publish_stop(ctx,0x8c61bc5cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61bc5e; /* pref @r8 */
L8c61bc5e: sa_motion_publish_stop(ctx,0x8c61bc60u); r[8]+=32u;goto L8c61bd80; /* bra 0x8c61bd80 */
L8c61bc98: sa_motion_publish_stop(ctx,0x8c61bc98u); r[0]=28u;goto L8c61bc9a; /* mov #0x1c,r0 */
L8c61bc9a: sa_motion_publish_stop(ctx,0x8c61bc9au); r[2]=rd(m,bus,&io_status,0x8c61be14u,4);if(io_status)return io_status;goto L8c61bc9c; /* mov.l 0x8c61be14,r2 */
L8c61bc9c: sa_motion_publish_stop(ctx,0x8c61bc9cu); r[3]=0u;goto L8c61bc9e; /* mov #0x0,r3 */
L8c61bc9e: sa_motion_publish_stop(ctx,0x8c61bc9eu); r[4]=r[15];goto L8c61bca0; /* mov r15,r4 */
L8c61bca0: sa_motion_publish_stop(ctx,0x8c61bca0u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61bca2; /* mov.l r3,@r2 */
L8c61bca2: sa_motion_publish_stop(ctx,0x8c61bca2u); if((r[14]&0x1f000000u)!=0x0c000000u)return -3;fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61bca4; /* fmov.s @r14,fr3 */
L8c61bca4: sa_motion_publish_stop(ctx,0x8c61bca4u); fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61bca6; /* fmov.s @r12,fr2 */
L8c61bca6: sa_motion_publish_stop(ctx,0x8c61bca6u); r[3]=rd(m,bus,&io_status,0x8c61be18u,4);if(io_status)return io_status;goto L8c61bca8; /* mov.l 0x8c61be18,r3 */
L8c61bca8: sa_motion_publish_stop(ctx,0x8c61bca8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bcaa; /* fadd fr3,fr2 */
L8c61bcaa: sa_motion_publish_stop(ctx,0x8c61bcaau); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61bcac; /* fmov.s @r13,fr3 */
L8c61bcac: sa_motion_publish_stop(ctx,0x8c61bcacu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bcae; /* fadd fr3,fr2 */
L8c61bcae: sa_motion_publish_stop(ctx,0x8c61bcaeu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bcb0; /* fmov fr2,@(r0,r15) */
L8c61bcb0: sa_motion_publish_stop(ctx,0x8c61bcb0u); r[0]=4u;goto L8c61bcb2; /* mov #0x4,r0 */
L8c61bcb2: sa_motion_publish_stop(ctx,0x8c61bcb2u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bcb4; /* fmov.s @(r0,r14),fr3 */
L8c61bcb4: sa_motion_publish_stop(ctx,0x8c61bcb4u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bcb6; /* fmov.s @(r0,r12),fr2 */
L8c61bcb6: sa_motion_publish_stop(ctx,0x8c61bcb6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bcb8; /* fadd fr3,fr2 */
L8c61bcb8: sa_motion_publish_stop(ctx,0x8c61bcb8u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bcba; /* fmov.s @(r0,r13),fr3 */
L8c61bcba: sa_motion_publish_stop(ctx,0x8c61bcbau); r[0]=32u;goto L8c61bcbc; /* mov #0x20,r0 */
L8c61bcbc: sa_motion_publish_stop(ctx,0x8c61bcbcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bcbe; /* fadd fr3,fr2 */
L8c61bcbe: sa_motion_publish_stop(ctx,0x8c61bcbeu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bcc0; /* fmov fr2,@(r0,r15) */
L8c61bcc0: sa_motion_publish_stop(ctx,0x8c61bcc0u); r[0]=8u;goto L8c61bcc2; /* mov #0x8,r0 */
L8c61bcc2: sa_motion_publish_stop(ctx,0x8c61bcc2u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bcc4; /* fmov.s @(r0,r14),fr3 */
L8c61bcc4: sa_motion_publish_stop(ctx,0x8c61bcc4u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bcc6; /* fmov.s @(r0,r12),fr2 */
L8c61bcc6: sa_motion_publish_stop(ctx,0x8c61bcc6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bcc8; /* fadd fr3,fr2 */
L8c61bcc8: sa_motion_publish_stop(ctx,0x8c61bcc8u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bcca; /* fmov.s @(r0,r13),fr3 */
L8c61bcca: sa_motion_publish_stop(ctx,0x8c61bccau); r[0]=36u;goto L8c61bccc; /* mov #0x24,r0 */
L8c61bccc: sa_motion_publish_stop(ctx,0x8c61bcccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bcce; /* fadd fr3,fr2 */
L8c61bcce: sa_motion_publish_stop(ctx,0x8c61bcceu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bcd0; /* fmov fr2,@(r0,r15) */
L8c61bcd0: sa_motion_publish_stop(ctx,0x8c61bcd2u); target=r[3];*pr=0x8c61bcd4u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61bcd4; /* jsr @r3 */
L8c61bcd4: sa_motion_publish_stop(ctx,0x8c61bcd4u); r[3]=rd(m,bus,&io_status,0x8c61be1cu,4);if(io_status)return io_status;goto L8c61bcd6; /* mov.l 0x8c61be1c,r3 */
L8c61bcd6: sa_motion_publish_stop(ctx,0x8c61bcd6u); r[0]=28u;goto L8c61bcd8; /* mov #0x1c,r0 */
L8c61bcd8: sa_motion_publish_stop(ctx,0x8c61bcd8u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bcda; /* fmov.s @(r0,r15),fr3 */
L8c61bcda: sa_motion_publish_stop(ctx,0x8c61bcdau); r[0]=32u;goto L8c61bcdc; /* mov #0x20,r0 */
L8c61bcdc: sa_motion_publish_stop(ctx,0x8c61bcdcu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61bcde; /* fmov.s @r3,fr2 */
L8c61bcde: sa_motion_publish_stop(ctx,0x8c61bcdeu); r[3]+=8u;goto L8c61bce0; /* add #0x8,r3 */
L8c61bce0: sa_motion_publish_stop(ctx,0x8c61bce0u); r[2]=rd(m,bus,&io_status,0x8c61be20u,4);if(io_status)return io_status;goto L8c61bce2; /* mov.l 0x8c61be20,r2 */
L8c61bce2: sa_motion_publish_stop(ctx,0x8c61bce2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61bce4; /* fmul fr3,fr2 */
L8c61bce4: sa_motion_publish_stop(ctx,0x8c61bce4u); r[1]=rd(m,bus,&io_status,0x8c61be24u,4);if(io_status)return io_status;goto L8c61bce6; /* mov.l 0x8c61be24,r1 */
L8c61bce6: sa_motion_publish_stop(ctx,0x8c61bce6u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61bce8; /* fmov.s @r2,fr3 */
L8c61bce8: sa_motion_publish_stop(ctx,0x8c61bce8u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61bcea; /* fmov.s @r1,fr1 */
L8c61bcea: sa_motion_publish_stop(ctx,0x8c61bceau); r[2]=rd(m,bus,&io_status,0x8c61be28u,4);if(io_status)return io_status;goto L8c61bcec; /* mov.l 0x8c61be28,r2 */
L8c61bcec: sa_motion_publish_stop(ctx,0x8c61bcecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bcee; /* fsub fr2,fr3 */
L8c61bcee: sa_motion_publish_stop(ctx,0x8c61bceeu); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bcf0; /* fmov.s @(r0,r15),fr2 */
L8c61bcf0: sa_motion_publish_stop(ctx,0x8c61bcf0u); r[0]=36u;goto L8c61bcf2; /* mov #0x24,r0 */
L8c61bcf2: sa_motion_publish_stop(ctx,0x8c61bcf2u); r[1]=rd(m,bus,&io_status,0x8c61be30u,4);if(io_status)return io_status;goto L8c61bcf4; /* mov.l 0x8c61be30,r1 */
L8c61bcf4: sa_motion_publish_stop(ctx,0x8c61bcf4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bcf6; /* fmul fr2,fr1 */
L8c61bcf6: sa_motion_publish_stop(ctx,0x8c61bcf6u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bcf8; /* fmov.s @(r0,r15),fr2 */
L8c61bcf8: sa_motion_publish_stop(ctx,0x8c61bcf8u); r[0]=12u;goto L8c61bcfa; /* mov #0xc,r0 */
L8c61bcfa: sa_motion_publish_stop(ctx,0x8c61bcfau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bcfc; /* fsub fr1,fr3 */
L8c61bcfc: sa_motion_publish_stop(ctx,0x8c61bcfcu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61bcfe; /* fmov.s @r3,fr1 */
L8c61bcfe: sa_motion_publish_stop(ctx,0x8c61bcfeu); r[3]=rd(m,bus,&io_status,0x8c61be2cu,4);if(io_status)return io_status;goto L8c61bd00; /* mov.l 0x8c61be2c,r3 */
L8c61bd00: sa_motion_publish_stop(ctx,0x8c61bd00u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bd02; /* fmul fr2,fr1 */
L8c61bd02: sa_motion_publish_stop(ctx,0x8c61bd02u); fcopy(f,4u,3u);goto L8c61bd04; /* fmov fr3,fr4 */
L8c61bd04: sa_motion_publish_stop(ctx,0x8c61bd04u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61bd06; /* fmov.s @r2,fr3 */
L8c61bd06: sa_motion_publish_stop(ctx,0x8c61bd06u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61bd08; /* fsub fr1,fr4 */
L8c61bd08: sa_motion_publish_stop(ctx,0x8c61bd08u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bd0a; /* fmov fr3,@(r0,r15) */
L8c61bd0a: sa_motion_publish_stop(ctx,0x8c61bd0au); r[0]=16u;goto L8c61bd0c; /* mov #0x10,r0 */
L8c61bd0c: sa_motion_publish_stop(ctx,0x8c61bd0cu); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61bd0e; /* fmov.s @r3,fr3 */
L8c61bd0e: sa_motion_publish_stop(ctx,0x8c61bd0eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bd10; /* fmul fr4,fr3 */
L8c61bd10: sa_motion_publish_stop(ctx,0x8c61bd10u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bd12; /* fmov fr3,@(r0,r15) */
L8c61bd12: sa_motion_publish_stop(ctx,0x8c61bd12u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61bd14; /* fmov.s @r1,fr3 */
L8c61bd14: sa_motion_publish_stop(ctx,0x8c61bd14u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bd16; /* fmul fr4,fr3 */
L8c61bd16: sa_motion_publish_stop(ctx,0x8c61bd16u); r[0]=20u;goto L8c61bd18; /* mov #0x14,r0 */
L8c61bd18: sa_motion_publish_stop(ctx,0x8c61bd18u); r[2]+=12u;goto L8c61bd1a; /* add #0xc,r2 */
L8c61bd1a: sa_motion_publish_stop(ctx,0x8c61bd1au); r[9]+=4294967294u;goto L8c61bd1c; /* add #-0x2,r9 */
L8c61bd1c: sa_motion_publish_stop(ctx,0x8c61bd1cu); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bd1e; /* fmov fr3,@(r0,r15) */
L8c61bd1e: sa_motion_publish_stop(ctx,0x8c61bd1eu); r[0]=24u;goto L8c61bd20; /* mov #0x18,r0 */
L8c61bd20: sa_motion_publish_stop(ctx,0x8c61bd20u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61bd22; /* fmov.s @r2,fr3 */
L8c61bd22: sa_motion_publish_stop(ctx,0x8c61bd22u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bd24; /* fmul fr4,fr3 */
L8c61bd24: sa_motion_publish_stop(ctx,0x8c61bd24u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bd26; /* fmov fr3,@(r0,r15) */
L8c61bd26: sa_motion_publish_stop(ctx,0x8c61bd26u); r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61bd28; /* mov.w @r10+,r12 */
L8c61bd28: sa_motion_publish_stop(ctx,0x8c61bd28u); r[0]=rd(m,bus,&io_status,0x8c61be34u,4);if(io_status)return io_status;goto L8c61bd2a; /* mov.l 0x8c61be34,r0 */
L8c61bd2a: sa_motion_publish_stop(ctx,0x8c61bd2au); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61bd2c; /* mov.w @r10+,r14 */
L8c61bd2c: sa_motion_publish_stop(ctx,0x8c61bd2cu); r[12]=r[12]&65535u;goto L8c61bd2e; /* extu.w r12,r12 */
L8c61bd2e: sa_motion_publish_stop(ctx,0x8c61bd2eu); r[12]<<=2;goto L8c61bd30; /* shll2 r12 */
L8c61bd30: sa_motion_publish_stop(ctx,0x8c61bd30u); r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61bd32; /* mov.l @r0,r3 */
L8c61bd32: sa_motion_publish_stop(ctx,0x8c61bd32u); r[14]=r[14]&65535u;goto L8c61bd34; /* extu.w r14,r14 */
L8c61bd34: sa_motion_publish_stop(ctx,0x8c61bd34u); r[14]<<=2;goto L8c61bd36; /* shll2 r14 */
L8c61bd36: sa_motion_publish_stop(ctx,0x8c61bd36u); r[14]<<=2;goto L8c61bd38; /* shll2 r14 */
L8c61bd38: sa_motion_publish_stop(ctx,0x8c61bd38u); r[12]<<=2;goto L8c61bd3a; /* shll2 r12 */
L8c61bd3a: sa_motion_publish_stop(ctx,0x8c61bd3au); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61bd3c; /* shll r12 */
L8c61bd3c: sa_motion_publish_stop(ctx,0x8c61bd3cu); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61bd3e; /* shll r14 */
L8c61bd3e: sa_motion_publish_stop(ctx,0x8c61bd3eu); *sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c61bd40; /* cmp/pl r9 */
L8c61bd40: sa_motion_publish_stop(ctx,0x8c61bd40u); r[12]+=r[3];goto L8c61bd42; /* add r3,r12 */
L8c61bd42: sa_motion_publish_stop(ctx,0x8c61bd42u); r[14]+=r[3];goto L8c61bd44; /* add r3,r14 */
L8c61bd44: sa_motion_publish_stop(ctx,0x8c61bd46u); take=!(*sr&1u);r[11]=0u;if(take)goto L8c61bd80;goto L8c61bd48; /* bf/s 0x8c61bd80 */
L8c61bd48: sa_motion_publish_stop(ctx,0x8c61bd48u); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61bd4a; /* mov.w @r10+,r13 */
L8c61bd4a: sa_motion_publish_stop(ctx,0x8c61bd4au); r[2]=rd(m,bus,&io_status,0x8c61be34u,4);if(io_status)return io_status;goto L8c61bd4c; /* mov.l 0x8c61be34,r2 */
L8c61bd4c: sa_motion_publish_stop(ctx,0x8c61bd4cu); r[13]=r[13]&65535u;goto L8c61bd4e; /* extu.w r13,r13 */
L8c61bd4e: sa_motion_publish_stop(ctx,0x8c61bd4eu); r[4]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61bd50; /* mov.l @(0xc,r12),r4 */
L8c61bd50: sa_motion_publish_stop(ctx,0x8c61bd50u); r[13]<<=2;goto L8c61bd52; /* shll2 r13 */
L8c61bd52: sa_motion_publish_stop(ctx,0x8c61bd52u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61bd54; /* mov.l @r2,r3 */
L8c61bd54: sa_motion_publish_stop(ctx,0x8c61bd54u); r[13]<<=2;goto L8c61bd56; /* shll2 r13 */
L8c61bd56: sa_motion_publish_stop(ctx,0x8c61bd56u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61bd58; /* shll r13 */
L8c61bd58: sa_motion_publish_stop(ctx,0x8c61bd58u); r[13]+=r[3];goto L8c61bd5a; /* add r3,r13 */
L8c61bd5a: sa_motion_publish_stop(ctx,0x8c61bd5au); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61bd5c; /* mov.l @(0xc,r14),r3 */
L8c61bd5c: sa_motion_publish_stop(ctx,0x8c61bd5cu); r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61bd5e; /* mov.l @(0xc,r13),r1 */
L8c61bd5e: sa_motion_publish_stop(ctx,0x8c61bd5eu); r[4]&=r[3];goto L8c61bd60; /* and r3,r4 */
L8c61bd60: sa_motion_publish_stop(ctx,0x8c61bd60u); r[3]=1u;goto L8c61bd62; /* mov #0x1,r3 */
L8c61bd62: sa_motion_publish_stop(ctx,0x8c61bd62u); r[4]&=r[1];goto L8c61bd64; /* and r1,r4 */
L8c61bd64: sa_motion_publish_stop(ctx,0x8c61bd64u); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61bd66; /* tst r3,r4 */
L8c61bd66: sa_motion_publish_stop(ctx,0x8c61bd66u); if(!(*sr&1u))goto L8c61bd76;goto L8c61bd68; /* bf 0x8c61bd76 */
L8c61bd68: sa_motion_publish_stop(ctx,0x8c61bd68u); r[1]=rd(m,bus,&io_status,0x8c61be38u,4);if(io_status)return io_status;goto L8c61bd6a; /* mov.l 0x8c61be38,r1 */
L8c61bd6a: sa_motion_publish_stop(ctx,0x8c61bd6au); r[7]=r[15];goto L8c61bd6c; /* mov r15,r7 */
L8c61bd6c: sa_motion_publish_stop(ctx,0x8c61bd6cu); r[5]=r[14];goto L8c61bd6e; /* mov r14,r5 */
L8c61bd6e: sa_motion_publish_stop(ctx,0x8c61bd6eu); r[7]+=12u;goto L8c61bd70; /* add #0xc,r7 */
L8c61bd70: sa_motion_publish_stop(ctx,0x8c61bd70u); r[6]=r[13];goto L8c61bd72; /* mov r13,r6 */
L8c61bd72: sa_motion_publish_stop(ctx,0x8c61bd74u); target=r[1];*pr=0x8c61bd76u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61bd76; /* jsr @r1 */
L8c61bd76: sa_motion_publish_stop(ctx,0x8c61bd76u); r[11]+=1u;goto L8c61bd78; /* add #0x1,r11 */
L8c61bd78: sa_motion_publish_stop(ctx,0x8c61bd78u); r[12]=r[14];goto L8c61bd7a; /* mov r14,r12 */
L8c61bd7a: sa_motion_publish_stop(ctx,0x8c61bd7au); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[9]);goto L8c61bd7c; /* cmp/ge r9,r11 */
L8c61bd7c: sa_motion_publish_stop(ctx,0x8c61bd7eu); take=!(*sr&1u);r[14]=r[13];if(take)goto L8c61bd48;goto L8c61bd80; /* bf/s 0x8c61bd48 */
L8c61bd80: sa_motion_publish_stop(ctx,0x8c61bd80u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61bd82; /* mov.l @(0x4,r15),r2 */
L8c61bd82: sa_motion_publish_stop(ctx,0x8c61bd82u); r[2]+=1u;goto L8c61bd84; /* add #0x1,r2 */
L8c61bd84: sa_motion_publish_stop(ctx,0x8c61bd84u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c61bd86; /* mov.l r2,@(0x4,r15) */
L8c61bd86: sa_motion_publish_stop(ctx,0x8c61bd86u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61bd88; /* mov.l @r15,r3 */
L8c61bd88: sa_motion_publish_stop(ctx,0x8c61bd88u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61bd8a; /* mov.w @(0x2,r3),r0 */
L8c61bd8a: sa_motion_publish_stop(ctx,0x8c61bd8au); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61bd8c; /* mov.l @(0x4,r15),r3 */
L8c61bd8c: sa_motion_publish_stop(ctx,0x8c61bd8cu); r[0]=r[0]&65535u;goto L8c61bd8e; /* extu.w r0,r0 */
L8c61bd8e: sa_motion_publish_stop(ctx,0x8c61bd8eu); *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c61bd90; /* cmp/ge r0,r3 */
L8c61bd90: sa_motion_publish_stop(ctx,0x8c61bd90u); if((*sr&1u))goto L8c61bd96;goto L8c61bd92; /* bt 0x8c61bd96 */
L8c61bd92: sa_motion_publish_stop(ctx,0x8c61bd94u); goto L8c61ba40; /* bra 0x8c61ba40 */
L8c61bd96: sa_motion_publish_stop(ctx,0x8c61bd98u); goto L8c61bdf0; /* bra 0x8c61bdf0 */
L8c61bd9a: sa_motion_publish_stop(ctx,0x8c61bd9au); r[1]=rd(m,bus,&io_status,0x8c61be3cu,4);if(io_status)return io_status;goto L8c61bd9c; /* mov.l 0x8c61be3c,r1 */
L8c61bd9c: sa_motion_publish_stop(ctx,0x8c61bd9cu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61bd9e; /* mov.l @r15,r2 */
L8c61bd9e: sa_motion_publish_stop(ctx,0x8c61bd9eu); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61bda0; /* mov.l r2,@r1 */
L8c61bda0: sa_motion_publish_stop(ctx,0x8c61bda0u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61bda2; /* frchg */
L8c61bda2: sa_motion_publish_stop(ctx,0x8c61bda2u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61bda4; /* fmov.s fr0,@-r15 */
L8c61bda4: sa_motion_publish_stop(ctx,0x8c61bda4u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61bda6; /* fmov.s fr1,@-r15 */
L8c61bda6: sa_motion_publish_stop(ctx,0x8c61bda6u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61bda8; /* fmov.s fr2,@-r15 */
L8c61bda8: sa_motion_publish_stop(ctx,0x8c61bda8u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61bdaa; /* fmov.s fr3,@-r15 */
L8c61bdaa: sa_motion_publish_stop(ctx,0x8c61bdaau); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61bdac; /* fmov.s fr4,@-r15 */
L8c61bdac: sa_motion_publish_stop(ctx,0x8c61bdacu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61bdae; /* fmov.s fr5,@-r15 */
L8c61bdae: sa_motion_publish_stop(ctx,0x8c61bdaeu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61bdb0; /* fmov.s fr6,@-r15 */
L8c61bdb0: sa_motion_publish_stop(ctx,0x8c61bdb0u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61bdb2; /* fmov.s fr7,@-r15 */
L8c61bdb2: sa_motion_publish_stop(ctx,0x8c61bdb2u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61bdb4; /* fmov.s fr8,@-r15 */
L8c61bdb4: sa_motion_publish_stop(ctx,0x8c61bdb4u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61bdb6; /* fmov.s fr9,@-r15 */
L8c61bdb6: sa_motion_publish_stop(ctx,0x8c61bdb6u); r[0]=rd(m,bus,&io_status,0x8c61be04u,4);if(io_status)return io_status;goto L8c61bdb8; /* mov.l 0x8c61be04,r0 */
L8c61bdb8: sa_motion_publish_stop(ctx,0x8c61bdb8u); fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdba; /* fmov.s @r0+,fr0 */
L8c61bdba: sa_motion_publish_stop(ctx,0x8c61bdbau); fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdbc; /* fmov.s @r0+,fr1 */
L8c61bdbc: sa_motion_publish_stop(ctx,0x8c61bdbcu); fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c61bdbe; /* fmov.s @r0,fr2 */
L8c61bdbe: sa_motion_publish_stop(ctx,0x8c61bdbeu); r[0]=rd(m,bus,&io_status,0x8c61be08u,4);if(io_status)return io_status;goto L8c61bdc0; /* mov.l 0x8c61be08,r0 */
L8c61bdc0: sa_motion_publish_stop(ctx,0x8c61bdc0u); r[0]+=16u;goto L8c61bdc2; /* add #0x10,r0 */
L8c61bdc2: sa_motion_publish_stop(ctx,0x8c61bdc2u); fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdc4; /* fmov.s @r0+,fr4 */
L8c61bdc4: sa_motion_publish_stop(ctx,0x8c61bdc4u); fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdc6; /* fmov.s @r0+,fr5 */
L8c61bdc6: sa_motion_publish_stop(ctx,0x8c61bdc6u); fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdc8; /* fmov.s @r0+,fr6 */
L8c61bdc8: sa_motion_publish_stop(ctx,0x8c61bdc8u); fload(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;goto L8c61bdca; /* fmov.s @r0,fr7 */
L8c61bdca: sa_motion_publish_stop(ctx,0x8c61bdcau); r[0]=rd(m,bus,&io_status,0x8c61be0cu,4);if(io_status)return io_status;goto L8c61bdcc; /* mov.l 0x8c61be0c,r0 */
L8c61bdcc: sa_motion_publish_stop(ctx,0x8c61bdccu); fload(m,bus,&io_status,f,8u,r[0]);if(io_status)return io_status;goto L8c61bdce; /* fmov.s @r0,fr8 */
L8c61bdce: sa_motion_publish_stop(ctx,0x8c61bdceu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61bdd0; /* frchg */
L8c61bdd0: sa_motion_publish_stop(ctx,0x8c61bdd0u); r[2]=rd(m,bus,&io_status,0x8c61be40u,4);if(io_status)return io_status;goto L8c61bdd2; /* mov.l 0x8c61be40,r2 */
L8c61bdd2: sa_motion_publish_stop(ctx,0x8c61bdd2u); r[3]=rd(m,bus,&io_status,0x8c61be3cu,4);if(io_status)return io_status;goto L8c61bdd4; /* mov.l 0x8c61be3c,r3 */
L8c61bdd4: sa_motion_publish_stop(ctx,0x8c61bdd6u); target=r[2];*pr=0x8c61bdd8u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61bdd8; /* jsr @r2 */
L8c61bdd8: sa_motion_publish_stop(ctx,0x8c61bdd8u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61bdda; /* frchg */
L8c61bdda: sa_motion_publish_stop(ctx,0x8c61bddau); fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bddc; /* fmov.s @r15+,fr9 */
L8c61bddc: sa_motion_publish_stop(ctx,0x8c61bddcu); fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdde; /* fmov.s @r15+,fr8 */
L8c61bdde: sa_motion_publish_stop(ctx,0x8c61bddeu); fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bde0; /* fmov.s @r15+,fr7 */
L8c61bde0: sa_motion_publish_stop(ctx,0x8c61bde0u); fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bde2; /* fmov.s @r15+,fr6 */
L8c61bde2: sa_motion_publish_stop(ctx,0x8c61bde2u); fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bde4; /* fmov.s @r15+,fr5 */
L8c61bde4: sa_motion_publish_stop(ctx,0x8c61bde4u); fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bde6; /* fmov.s @r15+,fr4 */
L8c61bde6: sa_motion_publish_stop(ctx,0x8c61bde6u); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bde8; /* fmov.s @r15+,fr3 */
L8c61bde8: sa_motion_publish_stop(ctx,0x8c61bde8u); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdea; /* fmov.s @r15+,fr2 */
L8c61bdea: sa_motion_publish_stop(ctx,0x8c61bdeau); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdec; /* fmov.s @r15+,fr1 */
L8c61bdec: sa_motion_publish_stop(ctx,0x8c61bdecu); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61bdee; /* fmov.s @r15+,fr0 */
L8c61bdee: sa_motion_publish_stop(ctx,0x8c61bdeeu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61bdf0; /* frchg */
L8c61bdf0: sa_motion_publish_stop(ctx,0x8c61bdf0u); r[15]+=40u;goto L8c61bdf2; /* add #0x28,r15 */
L8c61bdf2: sa_motion_publish_stop(ctx,0x8c61bdf2u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61bdf4; /* lds.l @r15+,PR */
L8c61bdf4: sa_motion_publish_stop(ctx,0x8c61bdf4u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61bdf6; /* mov.l @r15+,r9 */
L8c61bdf6: sa_motion_publish_stop(ctx,0x8c61bdf6u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61bdf8; /* mov.l @r15+,r10 */
L8c61bdf8: sa_motion_publish_stop(ctx,0x8c61bdf8u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61bdfa; /* mov.l @r15+,r11 */
L8c61bdfa: sa_motion_publish_stop(ctx,0x8c61bdfau); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61bdfc; /* mov.l @r15+,r12 */
L8c61bdfc: sa_motion_publish_stop(ctx,0x8c61bdfcu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61bdfe; /* mov.l @r15+,r13 */
L8c61bdfe: sa_motion_publish_stop(ctx,0x8c61be00u); if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_face_vector_normalize_8c63a94c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)sr;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63a94c;
L8c63a94c: sa_motion_publish_stop(ctx,0x8c63a94cu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a94e; /* fmov.s @r4+,fr0 */
L8c63a94e: sa_motion_publish_stop(ctx,0x8c63a94eu); fload(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a950; /* fmov.s @r4+,fr1 */
L8c63a950: sa_motion_publish_stop(ctx,0x8c63a950u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a952; /* fmov.s @r4+,fr2 */
L8c63a952: sa_motion_publish_stop(ctx,0x8c63a952u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c63a954; /* fldi0 fr3 */
L8c63a954: sa_motion_publish_stop(ctx,0x8c63a954u); status=sa_sh4_vector_numeric(0xf0edu,f,*fpul);if(status)return status;goto L8c63a956; /* fipr fv0,fv0 */
L8c63a956: sa_motion_publish_stop(ctx,0x8c63a956u); fcopy(f,4u,0u);goto L8c63a958; /* fmov fr0,fr4 */
L8c63a958: sa_motion_publish_stop(ctx,0x8c63a958u); fcopy(f,0u,3u);goto L8c63a95a; /* fmov fr3,fr0 */
L8c63a95a: sa_motion_publish_stop(ctx,0x8c63a95au); status=sa_sh4_vector_numeric(0xf37du,f,*fpul);if(status)return status;goto L8c63a95c; /* fsrra fr3 */
L8c63a95c: sa_motion_publish_stop(ctx,0x8c63a95cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63a95e; /* fmul fr3,fr2 */
L8c63a95e: sa_motion_publish_stop(ctx,0x8c63a95eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63a960; /* fmul fr3,fr1 */
L8c63a960: sa_motion_publish_stop(ctx,0x8c63a960u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63a962; /* fmul fr3,fr4 */
L8c63a962: sa_motion_publish_stop(ctx,0x8c63a962u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c63a964; /* fmov.s fr2,@-r4 */
L8c63a964: sa_motion_publish_stop(ctx,0x8c63a964u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;goto L8c63a966; /* fmov.s fr1,@-r4 */
L8c63a966: sa_motion_publish_stop(ctx,0x8c63a966u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c63a968; /* fmov.s fr4,@-r4 */
L8c63a968: sa_motion_publish_stop(ctx,0x8c63a96au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}return 0; /* rts */
}
