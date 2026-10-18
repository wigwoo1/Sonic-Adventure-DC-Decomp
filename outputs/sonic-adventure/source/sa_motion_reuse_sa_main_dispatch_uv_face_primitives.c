#include "sa_draw_motion_stop.h"
/* UV/face primitive family with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_uv_face_primitive_emit_8c61be44(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 uint32_t entry_pr=*pr;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61be44;
L8c61be44: sa_motion_publish_stop(ctx,0x8c61be44u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61be46; /* mov.l r14,@-r15 */
L8c61be46: sa_motion_publish_stop(ctx,0x8c61be46u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61be48; /* mov.l r13,@-r15 */
L8c61be48: sa_motion_publish_stop(ctx,0x8c61be48u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61be4a; /* mov.l r12,@-r15 */
L8c61be4a: sa_motion_publish_stop(ctx,0x8c61be4au); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61be4c; /* mov.l r11,@-r15 */
L8c61be4c: sa_motion_publish_stop(ctx,0x8c61be4cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61be4e; /* mov.l r10,@-r15 */
L8c61be4e: sa_motion_publish_stop(ctx,0x8c61be4eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61be50; /* mov.l r9,@-r15 */
L8c61be50: sa_motion_publish_stop(ctx,0x8c61be50u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61be52; /* fmov.s fr15,@-r15 */
L8c61be52: sa_motion_publish_stop(ctx,0x8c61be52u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61be54; /* sts.l PR,@-r15 */
L8c61be54: sa_motion_publish_stop(ctx,0x8c61be54u); r[15]+=4294967276u;goto L8c61be56; /* add #-0x14,r15 */
L8c61be56: sa_motion_publish_stop(ctx,0x8c61be56u); r[0]=0x8c61bf08u;goto L8c61be58; /* mova 0x8c61bf08,r0 */
L8c61be58: sa_motion_publish_stop(ctx,0x8c61be58u); r[5]=r[4];goto L8c61be5a; /* mov r4,r5 */
L8c61be5a: sa_motion_publish_stop(ctx,0x8c61be5au); r[3]=rd(m,bus,&io_status,0x8c61bf10u,4);if(io_status)return io_status;goto L8c61be5c; /* mov.l 0x8c61bf10,r3 */
L8c61be5c: sa_motion_publish_stop(ctx,0x8c61be5cu); r[13]=rd(m,bus,&io_status,0x8c61bf0cu,4);if(io_status)return io_status;goto L8c61be5e; /* mov.l 0x8c61bf0c,r13 */
L8c61be5e: sa_motion_publish_stop(ctx,0x8c61be5eu); wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61be60; /* mov.l r4,@r15 */
L8c61be60: sa_motion_publish_stop(ctx,0x8c61be60u); fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c61be62; /* fmov.s @r0,fr15 */
L8c61be62: sa_motion_publish_stop(ctx,0x8c61be62u); r[0]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;goto L8c61be64; /* mov.w @r5,r0 */
L8c61be64: sa_motion_publish_stop(ctx,0x8c61be64u); r[9]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c61be66; /* mov.l @(0x4,r5),r9 */
L8c61be66: sa_motion_publish_stop(ctx,0x8c61be66u); r[0]=r[0]&65535u;goto L8c61be68; /* extu.w r0,r0 */
L8c61be68: sa_motion_publish_stop(ctx,0x8c61be68u); r[10]=rd(m,bus,&io_status,r[5]+0x14u,4);if(io_status)return io_status;goto L8c61be6a; /* mov.l @(0x14,r5),r10 */
L8c61be6a: sa_motion_publish_stop(ctx,0x8c61be6au); r[0]&=r[3];goto L8c61be6c; /* and r3,r0 */
L8c61be6c: sa_motion_publish_stop(ctx,0x8c61be6cu); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61be6e; /* cmp/eq #0x0,r0 */
L8c61be6e: sa_motion_publish_stop(ctx,0x8c61be70u); take=(*sr&1u);r[4]=0u;if(take)goto L8c61be94;goto L8c61be72; /* bt/s 0x8c61be94 */
L8c61be72: sa_motion_publish_stop(ctx,0x8c61be72u); r[1]=sx(m,bus,&io_status,0x8c61bf06u,2);if(io_status)return io_status;goto L8c61be74; /* mov.w 0x8c61bf06,r1 */
L8c61be74: sa_motion_publish_stop(ctx,0x8c61be74u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61be76; /* cmp/eq r1,r0 */
L8c61be76: sa_motion_publish_stop(ctx,0x8c61be76u); if(!(*sr&1u))goto L8c61be7c;goto L8c61be78; /* bf 0x8c61be7c */
L8c61be78: sa_motion_publish_stop(ctx,0x8c61be7au); goto L8c61c032; /* bra 0x8c61c032 */
L8c61be7c: sa_motion_publish_stop(ctx,0x8c61be7cu); r[1]=rd(m,bus,&io_status,0x8c61bf14u,4);if(io_status)return io_status;goto L8c61be7e; /* mov.l 0x8c61bf14,r1 */
L8c61be7e: sa_motion_publish_stop(ctx,0x8c61be7eu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61be80; /* cmp/eq r1,r0 */
L8c61be80: sa_motion_publish_stop(ctx,0x8c61be80u); if(!(*sr&1u))goto L8c61be86;goto L8c61be82; /* bf 0x8c61be86 */
L8c61be82: sa_motion_publish_stop(ctx,0x8c61be84u); goto L8c61c1f8; /* bra 0x8c61c1f8 */
L8c61be86: sa_motion_publish_stop(ctx,0x8c61be86u); r[1]=rd(m,bus,&io_status,0x8c61bf10u,4);if(io_status)return io_status;goto L8c61be88; /* mov.l 0x8c61bf10,r1 */
L8c61be88: sa_motion_publish_stop(ctx,0x8c61be88u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61be8a; /* cmp/eq r1,r0 */
L8c61be8a: sa_motion_publish_stop(ctx,0x8c61be8au); if(!(*sr&1u))goto L8c61be90;goto L8c61be8c; /* bf 0x8c61be90 */
L8c61be8c: sa_motion_publish_stop(ctx,0x8c61be8eu); goto L8c61c42c; /* bra 0x8c61c42c */
L8c61be90: sa_motion_publish_stop(ctx,0x8c61be92u); goto L8c61c46a; /* bra 0x8c61c46a */
L8c61be94: sa_motion_publish_stop(ctx,0x8c61be94u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61be96; /* mov.l @r15,r2 */
L8c61be96: sa_motion_publish_stop(ctx,0x8c61be96u); r[1]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61be98; /* mov.l @r13,r1 */
L8c61be98: sa_motion_publish_stop(ctx,0x8c61be98u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61be9a; /* mov.w @(0x2,r2),r0 */
L8c61be9a: sa_motion_publish_stop(ctx,0x8c61be9au); r[0]=r[0]&65535u;goto L8c61be9c; /* extu.w r0,r0 */
L8c61be9c: sa_motion_publish_stop(ctx,0x8c61be9cu); r[2]=r[0];goto L8c61be9e; /* mov r0,r2 */
L8c61be9e: sa_motion_publish_stop(ctx,0x8c61be9eu); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61bea0; /* shll r0 */
L8c61bea0: sa_motion_publish_stop(ctx,0x8c61bea0u); r[0]+=r[2];goto L8c61bea2; /* add r2,r0 */
L8c61bea2: sa_motion_publish_stop(ctx,0x8c61bea2u); r[1]+=r[0];goto L8c61bea4; /* add r0,r1 */
L8c61bea4: sa_motion_publish_stop(ctx,0x8c61bea4u); wr(m,bus,&io_status,r[13],r[1],4);if(io_status)return io_status;goto L8c61bea6; /* mov.l r1,@r13 */
L8c61bea6: sa_motion_publish_stop(ctx,0x8c61bea8u); r[11]=r[4];goto L8c61c020; /* bra 0x8c61c020 */
L8c61beaa: sa_motion_publish_stop(ctx,0x8c61beaau); r[13]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61beac; /* mov.w @r9+,r13 */
L8c61beac: sa_motion_publish_stop(ctx,0x8c61beacu); r[0]=16u;goto L8c61beae; /* mov #0x10,r0 */
L8c61beae: sa_motion_publish_stop(ctx,0x8c61beaeu); r[3]=rd(m,bus,&io_status,0x8c61bf18u,4);if(io_status)return io_status;goto L8c61beb0; /* mov.l 0x8c61bf18,r3 */
L8c61beb0: sa_motion_publish_stop(ctx,0x8c61beb0u); r[12]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61beb2; /* mov.w @r9+,r12 */
L8c61beb2: sa_motion_publish_stop(ctx,0x8c61beb2u); r[13]=r[13]&65535u;goto L8c61beb4; /* extu.w r13,r13 */
L8c61beb4: sa_motion_publish_stop(ctx,0x8c61beb4u); r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61beb6; /* mov.l @r3,r4 */
L8c61beb6: sa_motion_publish_stop(ctx,0x8c61beb6u); r[13]<<=2;goto L8c61beb8; /* shll2 r13 */
L8c61beb8: sa_motion_publish_stop(ctx,0x8c61beb8u); r[12]=r[12]&65535u;goto L8c61beba; /* extu.w r12,r12 */
L8c61beba: sa_motion_publish_stop(ctx,0x8c61bebau); r[14]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61bebc; /* mov.w @r9+,r14 */
L8c61bebc: sa_motion_publish_stop(ctx,0x8c61bebcu); r[12]<<=2;goto L8c61bebe; /* shll2 r12 */
L8c61bebe: sa_motion_publish_stop(ctx,0x8c61bebeu); r[12]<<=2;goto L8c61bec0; /* shll2 r12 */
L8c61bec0: sa_motion_publish_stop(ctx,0x8c61bec0u); r[13]<<=2;goto L8c61bec2; /* shll2 r13 */
L8c61bec2: sa_motion_publish_stop(ctx,0x8c61bec2u); r[14]=r[14]&65535u;goto L8c61bec4; /* extu.w r14,r14 */
L8c61bec4: sa_motion_publish_stop(ctx,0x8c61bec4u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61bec6; /* shll r13 */
L8c61bec6: sa_motion_publish_stop(ctx,0x8c61bec6u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61bec8; /* shll r12 */
L8c61bec8: sa_motion_publish_stop(ctx,0x8c61bec8u); r[12]+=r[4];goto L8c61beca; /* add r4,r12 */
L8c61beca: sa_motion_publish_stop(ctx,0x8c61becau); fload(m,bus,&io_status,f,5u,r[0]+r[12]);if(io_status)return io_status;goto L8c61becc; /* fmov.s @(r0,r12),fr5 */
L8c61becc: sa_motion_publish_stop(ctx,0x8c61beccu); r[13]+=r[4];goto L8c61bece; /* add r4,r13 */
L8c61bece: sa_motion_publish_stop(ctx,0x8c61beceu); r[14]<<=2;goto L8c61bed0; /* shll2 r14 */
L8c61bed0: sa_motion_publish_stop(ctx,0x8c61bed0u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bed2; /* fmov.s @(r0,r13),fr3 */
L8c61bed2: sa_motion_publish_stop(ctx,0x8c61bed2u); r[14]<<=2;goto L8c61bed4; /* shll2 r14 */
L8c61bed4: sa_motion_publish_stop(ctx,0x8c61bed4u); fcopy(f,2u,5u);goto L8c61bed6; /* fmov fr5,fr2 */
L8c61bed6: sa_motion_publish_stop(ctx,0x8c61bed6u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61bed8; /* shll r14 */
L8c61bed8: sa_motion_publish_stop(ctx,0x8c61bed8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61beda; /* fsub fr3,fr2 */
L8c61beda: sa_motion_publish_stop(ctx,0x8c61bedau); r[0]=20u;goto L8c61bedc; /* mov #0x14,r0 */
L8c61bedc: sa_motion_publish_stop(ctx,0x8c61bedcu); r[14]+=r[4];goto L8c61bede; /* add r4,r14 */
L8c61bede: sa_motion_publish_stop(ctx,0x8c61bedeu); fload(m,bus,&io_status,f,4u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bee0; /* fmov.s @(r0,r12),fr4 */
L8c61bee0: sa_motion_publish_stop(ctx,0x8c61bee0u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bee2; /* fmov.s @(r0,r14),fr3 */
L8c61bee2: sa_motion_publish_stop(ctx,0x8c61bee2u); fcopy(f,1u,4u);goto L8c61bee4; /* fmov fr4,fr1 */
L8c61bee4: sa_motion_publish_stop(ctx,0x8c61bee4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bee6; /* fsub fr4,fr3 */
L8c61bee6: sa_motion_publish_stop(ctx,0x8c61bee6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61bee8; /* fmul fr3,fr2 */
L8c61bee8: sa_motion_publish_stop(ctx,0x8c61bee8u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61beea; /* fmov.s @(r0,r13),fr3 */
L8c61beea: sa_motion_publish_stop(ctx,0x8c61beeau); r[0]=16u;goto L8c61beec; /* mov #0x10,r0 */
L8c61beec: sa_motion_publish_stop(ctx,0x8c61beecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61beee; /* fsub fr3,fr1 */
L8c61beee: sa_motion_publish_stop(ctx,0x8c61beeeu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bef0; /* fmov.s @(r0,r14),fr3 */
L8c61bef0: sa_motion_publish_stop(ctx,0x8c61bef0u); fcopy(f,4u,2u);goto L8c61bef2; /* fmov fr2,fr4 */
L8c61bef2: sa_motion_publish_stop(ctx,0x8c61bef2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bef4; /* fsub fr5,fr3 */
L8c61bef4: sa_motion_publish_stop(ctx,0x8c61bef4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bef6; /* fmul fr3,fr1 */
L8c61bef6: sa_motion_publish_stop(ctx,0x8c61bef6u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61bef8; /* fldi0 fr3 */
L8c61bef8: sa_motion_publish_stop(ctx,0x8c61bef8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61befa; /* fsub fr1,fr4 */
L8c61befa: sa_motion_publish_stop(ctx,0x8c61befau); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61befc; /* fcmp/gt fr3,fr4 */
L8c61befc: sa_motion_publish_stop(ctx,0x8c61befcu); if(!(*sr&1u))goto L8c61bf1c;goto L8c61befe; /* bf 0x8c61bf1c */
L8c61befe: sa_motion_publish_stop(ctx,0x8c61bf00u); r[10]+=12u;goto L8c61c01e; /* bra 0x8c61c01e */
L8c61bf1c: sa_motion_publish_stop(ctx,0x8c61bf1cu); fload(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c61bf1e; /* fmov.s @r13,fr2 */
L8c61bf1e: sa_motion_publish_stop(ctx,0x8c61bf1eu); r[0]=8u;goto L8c61bf20; /* mov #0x8,r0 */
L8c61bf20: sa_motion_publish_stop(ctx,0x8c61bf20u); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61bf22; /* fmov.s @r12,fr3 */
L8c61bf22: sa_motion_publish_stop(ctx,0x8c61bf22u); r[4]=r[15];goto L8c61bf24; /* mov r15,r4 */
L8c61bf24: sa_motion_publish_stop(ctx,0x8c61bf24u); r[3]=rd(m,bus,&io_status,0x8c61c204u,4);if(io_status)return io_status;goto L8c61bf26; /* mov.l 0x8c61c204,r3 */
L8c61bf26: sa_motion_publish_stop(ctx,0x8c61bf26u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bf28; /* fadd fr3,fr2 */
L8c61bf28: sa_motion_publish_stop(ctx,0x8c61bf28u); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61bf2a; /* fmov.s @r14,fr3 */
L8c61bf2a: sa_motion_publish_stop(ctx,0x8c61bf2au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bf2c; /* fadd fr3,fr2 */
L8c61bf2c: sa_motion_publish_stop(ctx,0x8c61bf2cu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bf2e; /* fmov fr2,@(r0,r15) */
L8c61bf2e: sa_motion_publish_stop(ctx,0x8c61bf2eu); r[0]=4u;goto L8c61bf30; /* mov #0x4,r0 */
L8c61bf30: sa_motion_publish_stop(ctx,0x8c61bf30u); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bf32; /* fmov.s @(r0,r13),fr2 */
L8c61bf32: sa_motion_publish_stop(ctx,0x8c61bf32u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bf34; /* fmov.s @(r0,r12),fr3 */
L8c61bf34: sa_motion_publish_stop(ctx,0x8c61bf34u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bf36; /* fadd fr3,fr2 */
L8c61bf36: sa_motion_publish_stop(ctx,0x8c61bf36u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bf38; /* fmov.s @(r0,r14),fr3 */
L8c61bf38: sa_motion_publish_stop(ctx,0x8c61bf38u); r[0]=12u;goto L8c61bf3a; /* mov #0xc,r0 */
L8c61bf3a: sa_motion_publish_stop(ctx,0x8c61bf3au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bf3c; /* fadd fr3,fr2 */
L8c61bf3c: sa_motion_publish_stop(ctx,0x8c61bf3cu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bf3e; /* fmov fr2,@(r0,r15) */
L8c61bf3e: sa_motion_publish_stop(ctx,0x8c61bf3eu); r[0]=8u;goto L8c61bf40; /* mov #0x8,r0 */
L8c61bf40: sa_motion_publish_stop(ctx,0x8c61bf40u); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bf42; /* fmov.s @(r0,r13),fr2 */
L8c61bf42: sa_motion_publish_stop(ctx,0x8c61bf42u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bf44; /* fmov.s @(r0,r12),fr3 */
L8c61bf44: sa_motion_publish_stop(ctx,0x8c61bf44u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bf46; /* fadd fr3,fr2 */
L8c61bf46: sa_motion_publish_stop(ctx,0x8c61bf46u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bf48; /* fmov.s @(r0,r14),fr3 */
L8c61bf48: sa_motion_publish_stop(ctx,0x8c61bf48u); r[0]=16u;goto L8c61bf4a; /* mov #0x10,r0 */
L8c61bf4a: sa_motion_publish_stop(ctx,0x8c61bf4au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bf4c; /* fadd fr3,fr2 */
L8c61bf4c: sa_motion_publish_stop(ctx,0x8c61bf4cu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bf4e; /* fmov fr2,@(r0,r15) */
L8c61bf4e: sa_motion_publish_stop(ctx,0x8c61bf50u); target=r[3];*pr=0x8c61bf52u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61bf52; /* jsr @r3 */
L8c61bf52: sa_motion_publish_stop(ctx,0x8c61bf52u); r[3]=rd(m,bus,&io_status,0x8c61c208u,4);if(io_status)return io_status;goto L8c61bf54; /* mov.l 0x8c61c208,r3 */
L8c61bf54: sa_motion_publish_stop(ctx,0x8c61bf54u); r[0]=8u;goto L8c61bf56; /* mov #0x8,r0 */
L8c61bf56: sa_motion_publish_stop(ctx,0x8c61bf56u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bf58; /* fmov.s @(r0,r15),fr3 */
L8c61bf58: sa_motion_publish_stop(ctx,0x8c61bf58u); r[0]=12u;goto L8c61bf5a; /* mov #0xc,r0 */
L8c61bf5a: sa_motion_publish_stop(ctx,0x8c61bf5au); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61bf5c; /* fmov.s @r3,fr2 */
L8c61bf5c: sa_motion_publish_stop(ctx,0x8c61bf5cu); r[8]+=24u;goto L8c61bf5e; /* add #0x18,r8 */
L8c61bf5e: sa_motion_publish_stop(ctx,0x8c61bf5eu); r[2]=rd(m,bus,&io_status,0x8c61c20cu,4);if(io_status)return io_status;goto L8c61bf60; /* mov.l 0x8c61c20c,r2 */
L8c61bf60: sa_motion_publish_stop(ctx,0x8c61bf60u); r[3]+=8u;goto L8c61bf62; /* add #0x8,r3 */
L8c61bf62: sa_motion_publish_stop(ctx,0x8c61bf62u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61bf64; /* fmul fr3,fr2 */
L8c61bf64: sa_motion_publish_stop(ctx,0x8c61bf64u); r[1]=rd(m,bus,&io_status,0x8c61c210u,4);if(io_status)return io_status;goto L8c61bf66; /* mov.l 0x8c61c210,r1 */
L8c61bf66: sa_motion_publish_stop(ctx,0x8c61bf66u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61bf68; /* fmov.s @r2,fr3 */
L8c61bf68: sa_motion_publish_stop(ctx,0x8c61bf68u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61bf6a; /* fmov.s @r1,fr1 */
L8c61bf6a: sa_motion_publish_stop(ctx,0x8c61bf6au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bf6c; /* fsub fr2,fr3 */
L8c61bf6c: sa_motion_publish_stop(ctx,0x8c61bf6cu); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bf6e; /* fmov.s @(r0,r15),fr2 */
L8c61bf6e: sa_motion_publish_stop(ctx,0x8c61bf6eu); r[0]=16u;goto L8c61bf70; /* mov #0x10,r0 */
L8c61bf70: sa_motion_publish_stop(ctx,0x8c61bf70u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bf72; /* fmul fr2,fr1 */
L8c61bf72: sa_motion_publish_stop(ctx,0x8c61bf72u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bf74; /* fmov.s @(r0,r15),fr2 */
L8c61bf74: sa_motion_publish_stop(ctx,0x8c61bf74u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61bf76; /* mov.w @(0x2,r10),r0 */
L8c61bf76: sa_motion_publish_stop(ctx,0x8c61bf76u); r[2]=r[0];goto L8c61bf78; /* mov r0,r2 */
L8c61bf78: sa_motion_publish_stop(ctx,0x8c61bf78u); *fpul=r[2];goto L8c61bf7a; /* lds r2,FPUL */
L8c61bf7a: sa_motion_publish_stop(ctx,0x8c61bf7au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bf7c; /* fsub fr1,fr3 */
L8c61bf7c: sa_motion_publish_stop(ctx,0x8c61bf7cu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61bf7e; /* fmov.s @r3,fr1 */
L8c61bf7e: sa_motion_publish_stop(ctx,0x8c61bf7eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bf80; /* fmul fr2,fr1 */
L8c61bf80: sa_motion_publish_stop(ctx,0x8c61bf80u); fcopy(f,4u,3u);goto L8c61bf82; /* fmov fr3,fr4 */
L8c61bf82: sa_motion_publish_stop(ctx,0x8c61bf82u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61bf84; /* float FPUL,fr3 */
L8c61bf84: sa_motion_publish_stop(ctx,0x8c61bf84u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61bf86; /* fsub fr1,fr4 */
L8c61bf86: sa_motion_publish_stop(ctx,0x8c61bf86u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bf88; /* fmul fr15,fr3 */
L8c61bf88: sa_motion_publish_stop(ctx,0x8c61bf88u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bf8a; /* fmov.s fr3,@-r8 */
L8c61bf8a: sa_motion_publish_stop(ctx,0x8c61bf8au); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61bf8c; /* mov.w @r10,r2 */
L8c61bf8c: sa_motion_publish_stop(ctx,0x8c61bf8cu); *fpul=r[2];goto L8c61bf8e; /* lds r2,FPUL */
L8c61bf8e: sa_motion_publish_stop(ctx,0x8c61bf8eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61bf90; /* float FPUL,fr3 */
L8c61bf90: sa_motion_publish_stop(ctx,0x8c61bf90u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bf92; /* fmul fr15,fr3 */
L8c61bf92: sa_motion_publish_stop(ctx,0x8c61bf92u); r[0]=24u;goto L8c61bf94; /* mov #0x18,r0 */
L8c61bf94: sa_motion_publish_stop(ctx,0x8c61bf94u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bf96; /* fmov.s fr3,@-r8 */
L8c61bf96: sa_motion_publish_stop(ctx,0x8c61bf96u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bf98; /* fmov.s @(r0,r13),fr3 */
L8c61bf98: sa_motion_publish_stop(ctx,0x8c61bf98u); r[0]=20u;goto L8c61bf9a; /* mov #0x14,r0 */
L8c61bf9a: sa_motion_publish_stop(ctx,0x8c61bf9au); r[2]=rd(m,bus,&io_status,0x8c61c214u,4);if(io_status)return io_status;goto L8c61bf9c; /* mov.l 0x8c61c214,r2 */
L8c61bf9c: sa_motion_publish_stop(ctx,0x8c61bf9cu); r[10]+=4u;goto L8c61bf9e; /* add #0x4,r10 */
L8c61bf9e: sa_motion_publish_stop(ctx,0x8c61bf9eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfa0; /* fmov.s fr3,@-r8 */
L8c61bfa0: sa_motion_publish_stop(ctx,0x8c61bfa0u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bfa2; /* fmov.s @(r0,r13),fr3 */
L8c61bfa2: sa_motion_publish_stop(ctx,0x8c61bfa2u); r[0]=16u;goto L8c61bfa4; /* mov #0x10,r0 */
L8c61bfa4: sa_motion_publish_stop(ctx,0x8c61bfa4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfa6; /* fmov.s fr3,@-r8 */
L8c61bfa6: sa_motion_publish_stop(ctx,0x8c61bfa6u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bfa8; /* fmov.s @(r0,r13),fr3 */
L8c61bfa8: sa_motion_publish_stop(ctx,0x8c61bfa8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfaa; /* fmov.s fr3,@-r8 */
L8c61bfaa: sa_motion_publish_stop(ctx,0x8c61bfaau); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61bfac; /* mov.l r2,@-r8 */
L8c61bfac: sa_motion_publish_stop(ctx,0x8c61bfacu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61bfae; /* pref @r8 */
L8c61bfae: sa_motion_publish_stop(ctx,0x8c61bfaeu); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61bfb0; /* mov.w @(0x2,r10),r0 */
L8c61bfb0: sa_motion_publish_stop(ctx,0x8c61bfb0u); r[8]+=56u;goto L8c61bfb2; /* add #0x38,r8 */
L8c61bfb2: sa_motion_publish_stop(ctx,0x8c61bfb2u); r[2]=r[0];goto L8c61bfb4; /* mov r0,r2 */
L8c61bfb4: sa_motion_publish_stop(ctx,0x8c61bfb4u); *fpul=r[2];goto L8c61bfb6; /* lds r2,FPUL */
L8c61bfb6: sa_motion_publish_stop(ctx,0x8c61bfb6u); r[0]=24u;goto L8c61bfb8; /* mov #0x18,r0 */
L8c61bfb8: sa_motion_publish_stop(ctx,0x8c61bfb8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61bfba; /* float FPUL,fr3 */
L8c61bfba: sa_motion_publish_stop(ctx,0x8c61bfbau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bfbc; /* fmul fr15,fr3 */
L8c61bfbc: sa_motion_publish_stop(ctx,0x8c61bfbcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfbe; /* fmov.s fr3,@-r8 */
L8c61bfbe: sa_motion_publish_stop(ctx,0x8c61bfbeu); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61bfc0; /* mov.w @r10,r2 */
L8c61bfc0: sa_motion_publish_stop(ctx,0x8c61bfc0u); r[10]+=4u;goto L8c61bfc2; /* add #0x4,r10 */
L8c61bfc2: sa_motion_publish_stop(ctx,0x8c61bfc2u); *fpul=r[2];goto L8c61bfc4; /* lds r2,FPUL */
L8c61bfc4: sa_motion_publish_stop(ctx,0x8c61bfc4u); r[2]=rd(m,bus,&io_status,0x8c61c214u,4);if(io_status)return io_status;goto L8c61bfc6; /* mov.l 0x8c61c214,r2 */
L8c61bfc6: sa_motion_publish_stop(ctx,0x8c61bfc6u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61bfc8; /* float FPUL,fr3 */
L8c61bfc8: sa_motion_publish_stop(ctx,0x8c61bfc8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bfca; /* fmul fr15,fr3 */
L8c61bfca: sa_motion_publish_stop(ctx,0x8c61bfcau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfcc; /* fmov.s fr3,@-r8 */
L8c61bfcc: sa_motion_publish_stop(ctx,0x8c61bfccu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bfce; /* fmov.s @(r0,r12),fr3 */
L8c61bfce: sa_motion_publish_stop(ctx,0x8c61bfceu); r[0]=20u;goto L8c61bfd0; /* mov #0x14,r0 */
L8c61bfd0: sa_motion_publish_stop(ctx,0x8c61bfd0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfd2; /* fmov.s fr3,@-r8 */
L8c61bfd2: sa_motion_publish_stop(ctx,0x8c61bfd2u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bfd4; /* fmov.s @(r0,r12),fr3 */
L8c61bfd4: sa_motion_publish_stop(ctx,0x8c61bfd4u); r[0]=16u;goto L8c61bfd6; /* mov #0x10,r0 */
L8c61bfd6: sa_motion_publish_stop(ctx,0x8c61bfd6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfd8; /* fmov.s fr3,@-r8 */
L8c61bfd8: sa_motion_publish_stop(ctx,0x8c61bfd8u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bfda; /* fmov.s @(r0,r12),fr3 */
L8c61bfda: sa_motion_publish_stop(ctx,0x8c61bfdau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfdc; /* fmov.s fr3,@-r8 */
L8c61bfdc: sa_motion_publish_stop(ctx,0x8c61bfdcu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61bfde; /* mov.l r2,@-r8 */
L8c61bfde: sa_motion_publish_stop(ctx,0x8c61bfdeu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61bfe0; /* pref @r8 */
L8c61bfe0: sa_motion_publish_stop(ctx,0x8c61bfe0u); r[8]+=60u;goto L8c61bfe2; /* add #0x3c,r8 */
L8c61bfe2: sa_motion_publish_stop(ctx,0x8c61bfe2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61bfe4; /* fmov.s fr4,@-r8 */
L8c61bfe4: sa_motion_publish_stop(ctx,0x8c61bfe4u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61bfe6; /* mov.w @(0x2,r10),r0 */
L8c61bfe6: sa_motion_publish_stop(ctx,0x8c61bfe6u); r[2]=r[0];goto L8c61bfe8; /* mov r0,r2 */
L8c61bfe8: sa_motion_publish_stop(ctx,0x8c61bfe8u); *fpul=r[2];goto L8c61bfea; /* lds r2,FPUL */
L8c61bfea: sa_motion_publish_stop(ctx,0x8c61bfeau); r[0]=24u;goto L8c61bfec; /* mov #0x18,r0 */
L8c61bfec: sa_motion_publish_stop(ctx,0x8c61bfecu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61bfee; /* float FPUL,fr3 */
L8c61bfee: sa_motion_publish_stop(ctx,0x8c61bfeeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bff0; /* fmul fr15,fr3 */
L8c61bff0: sa_motion_publish_stop(ctx,0x8c61bff0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bff2; /* fmov.s fr3,@-r8 */
L8c61bff2: sa_motion_publish_stop(ctx,0x8c61bff2u); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61bff4; /* mov.w @r10,r2 */
L8c61bff4: sa_motion_publish_stop(ctx,0x8c61bff4u); r[10]+=4u;goto L8c61bff6; /* add #0x4,r10 */
L8c61bff6: sa_motion_publish_stop(ctx,0x8c61bff6u); *fpul=r[2];goto L8c61bff8; /* lds r2,FPUL */
L8c61bff8: sa_motion_publish_stop(ctx,0x8c61bff8u); r[2]=rd(m,bus,&io_status,0x8c61c218u,4);if(io_status)return io_status;goto L8c61bffa; /* mov.l 0x8c61c218,r2 */
L8c61bffa: sa_motion_publish_stop(ctx,0x8c61bffau); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61bffc; /* float FPUL,fr3 */
L8c61bffc: sa_motion_publish_stop(ctx,0x8c61bffcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bffe; /* fmul fr15,fr3 */
L8c61bffe: sa_motion_publish_stop(ctx,0x8c61bffeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c000; /* fmov.s fr3,@-r8 */
L8c61c000: sa_motion_publish_stop(ctx,0x8c61c000u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c002; /* fmov.s @(r0,r14),fr3 */
L8c61c002: sa_motion_publish_stop(ctx,0x8c61c002u); r[0]=20u;goto L8c61c004; /* mov #0x14,r0 */
L8c61c004: sa_motion_publish_stop(ctx,0x8c61c004u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c006; /* fmov.s fr3,@-r8 */
L8c61c006: sa_motion_publish_stop(ctx,0x8c61c006u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c008; /* fmov.s @(r0,r14),fr3 */
L8c61c008: sa_motion_publish_stop(ctx,0x8c61c008u); r[0]=16u;goto L8c61c00a; /* mov #0x10,r0 */
L8c61c00a: sa_motion_publish_stop(ctx,0x8c61c00au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c00c; /* fmov.s fr3,@-r8 */
L8c61c00c: sa_motion_publish_stop(ctx,0x8c61c00cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c00e; /* fmov.s @(r0,r14),fr3 */
L8c61c00e: sa_motion_publish_stop(ctx,0x8c61c00eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c010; /* fmov.s fr3,@-r8 */
L8c61c010: sa_motion_publish_stop(ctx,0x8c61c010u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c012; /* mov.l r2,@-r8 */
L8c61c012: sa_motion_publish_stop(ctx,0x8c61c012u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c014; /* pref @r8 */
L8c61c014: sa_motion_publish_stop(ctx,0x8c61c014u); r[0]=rd(m,bus,&io_status,0x8c61c21cu,4);if(io_status)return io_status;goto L8c61c016; /* mov.l 0x8c61c21c,r0 */
L8c61c016: sa_motion_publish_stop(ctx,0x8c61c016u); r[8]+=32u;goto L8c61c018; /* add #0x20,r8 */
L8c61c018: sa_motion_publish_stop(ctx,0x8c61c018u); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61c01a; /* mov.l @r0,r2 */
L8c61c01a: sa_motion_publish_stop(ctx,0x8c61c01au); r[2]+=3u;goto L8c61c01c; /* add #0x3,r2 */
L8c61c01c: sa_motion_publish_stop(ctx,0x8c61c01cu); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61c01e; /* mov.l r2,@r0 */
L8c61c01e: sa_motion_publish_stop(ctx,0x8c61c01eu); r[11]+=1u;goto L8c61c020; /* add #0x1,r11 */
L8c61c020: sa_motion_publish_stop(ctx,0x8c61c020u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61c022; /* mov.l @r15,r3 */
L8c61c022: sa_motion_publish_stop(ctx,0x8c61c022u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61c024; /* mov.w @(0x2,r3),r0 */
L8c61c024: sa_motion_publish_stop(ctx,0x8c61c024u); r[0]=r[0]&65535u;goto L8c61c026; /* extu.w r0,r0 */
L8c61c026: sa_motion_publish_stop(ctx,0x8c61c026u); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61c028; /* cmp/ge r0,r11 */
L8c61c028: sa_motion_publish_stop(ctx,0x8c61c028u); if((*sr&1u))goto L8c61c02e;goto L8c61c02a; /* bt 0x8c61c02e */
L8c61c02a: sa_motion_publish_stop(ctx,0x8c61c02cu); goto L8c61beaa; /* bra 0x8c61beaa */
L8c61c02e: sa_motion_publish_stop(ctx,0x8c61c030u); goto L8c61c46a; /* bra 0x8c61c46a */
L8c61c032: sa_motion_publish_stop(ctx,0x8c61c032u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61c034; /* mov.l @r15,r1 */
L8c61c034: sa_motion_publish_stop(ctx,0x8c61c034u); r[2]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61c036; /* mov.l @r13,r2 */
L8c61c036: sa_motion_publish_stop(ctx,0x8c61c036u); r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61c038; /* mov.w @(0x2,r1),r0 */
L8c61c038: sa_motion_publish_stop(ctx,0x8c61c038u); r[0]=r[0]&65535u;goto L8c61c03a; /* extu.w r0,r0 */
L8c61c03a: sa_motion_publish_stop(ctx,0x8c61c03au); r[0]<<=2;goto L8c61c03c; /* shll2 r0 */
L8c61c03c: sa_motion_publish_stop(ctx,0x8c61c03cu); r[2]+=r[0];goto L8c61c03e; /* add r0,r2 */
L8c61c03e: sa_motion_publish_stop(ctx,0x8c61c03eu); wr(m,bus,&io_status,r[13],r[2],4);if(io_status)return io_status;goto L8c61c040; /* mov.l r2,@r13 */
L8c61c040: sa_motion_publish_stop(ctx,0x8c61c042u); wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c61c1e4; /* bra 0x8c61c1e4 */
L8c61c044: sa_motion_publish_stop(ctx,0x8c61c044u); r[14]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c046; /* mov.w @r9+,r14 */
L8c61c046: sa_motion_publish_stop(ctx,0x8c61c046u); r[0]=16u;goto L8c61c048; /* mov #0x10,r0 */
L8c61c048: sa_motion_publish_stop(ctx,0x8c61c048u); r[3]=rd(m,bus,&io_status,0x8c61c220u,4);if(io_status)return io_status;goto L8c61c04a; /* mov.l 0x8c61c220,r3 */
L8c61c04a: sa_motion_publish_stop(ctx,0x8c61c04au); r[13]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c04c; /* mov.w @r9+,r13 */
L8c61c04c: sa_motion_publish_stop(ctx,0x8c61c04cu); r[14]=r[14]&65535u;goto L8c61c04e; /* extu.w r14,r14 */
L8c61c04e: sa_motion_publish_stop(ctx,0x8c61c04eu); r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61c050; /* mov.l @r3,r4 */
L8c61c050: sa_motion_publish_stop(ctx,0x8c61c050u); r[14]<<=2;goto L8c61c052; /* shll2 r14 */
L8c61c052: sa_motion_publish_stop(ctx,0x8c61c052u); r[13]=r[13]&65535u;goto L8c61c054; /* extu.w r13,r13 */
L8c61c054: sa_motion_publish_stop(ctx,0x8c61c054u); r[12]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c056; /* mov.w @r9+,r12 */
L8c61c056: sa_motion_publish_stop(ctx,0x8c61c056u); r[13]<<=2;goto L8c61c058; /* shll2 r13 */
L8c61c058: sa_motion_publish_stop(ctx,0x8c61c058u); r[13]<<=2;goto L8c61c05a; /* shll2 r13 */
L8c61c05a: sa_motion_publish_stop(ctx,0x8c61c05au); r[11]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c05c; /* mov.w @r9+,r11 */
L8c61c05c: sa_motion_publish_stop(ctx,0x8c61c05cu); r[14]<<=2;goto L8c61c05e; /* shll2 r14 */
L8c61c05e: sa_motion_publish_stop(ctx,0x8c61c05eu); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61c060; /* shll r14 */
L8c61c060: sa_motion_publish_stop(ctx,0x8c61c060u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61c062; /* shll r13 */
L8c61c062: sa_motion_publish_stop(ctx,0x8c61c062u); r[13]+=r[4];goto L8c61c064; /* add r4,r13 */
L8c61c064: sa_motion_publish_stop(ctx,0x8c61c064u); r[14]+=r[4];goto L8c61c066; /* add r4,r14 */
L8c61c066: sa_motion_publish_stop(ctx,0x8c61c066u); fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c068; /* fmov.s @(r0,r13),fr5 */
L8c61c068: sa_motion_publish_stop(ctx,0x8c61c068u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c06a; /* fmov.s @(r0,r14),fr3 */
L8c61c06a: sa_motion_publish_stop(ctx,0x8c61c06au); r[12]=r[12]&65535u;goto L8c61c06c; /* extu.w r12,r12 */
L8c61c06c: sa_motion_publish_stop(ctx,0x8c61c06cu); fcopy(f,2u,5u);goto L8c61c06e; /* fmov fr5,fr2 */
L8c61c06e: sa_motion_publish_stop(ctx,0x8c61c06eu); r[12]<<=2;goto L8c61c070; /* shll2 r12 */
L8c61c070: sa_motion_publish_stop(ctx,0x8c61c070u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61c072; /* fsub fr3,fr2 */
L8c61c072: sa_motion_publish_stop(ctx,0x8c61c072u); r[11]=r[11]&65535u;goto L8c61c074; /* extu.w r11,r11 */
L8c61c074: sa_motion_publish_stop(ctx,0x8c61c074u); r[12]<<=2;goto L8c61c076; /* shll2 r12 */
L8c61c076: sa_motion_publish_stop(ctx,0x8c61c076u); r[11]<<=2;goto L8c61c078; /* shll2 r11 */
L8c61c078: sa_motion_publish_stop(ctx,0x8c61c078u); r[0]=20u;goto L8c61c07a; /* mov #0x14,r0 */
L8c61c07a: sa_motion_publish_stop(ctx,0x8c61c07au); fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c07c; /* fmov.s @(r0,r13),fr4 */
L8c61c07c: sa_motion_publish_stop(ctx,0x8c61c07cu); r[11]<<=2;goto L8c61c07e; /* shll2 r11 */
L8c61c07e: sa_motion_publish_stop(ctx,0x8c61c07eu); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61c080; /* shll r12 */
L8c61c080: sa_motion_publish_stop(ctx,0x8c61c080u); r[12]+=r[4];goto L8c61c082; /* add r4,r12 */
L8c61c082: sa_motion_publish_stop(ctx,0x8c61c082u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61c084; /* shll r11 */
L8c61c084: sa_motion_publish_stop(ctx,0x8c61c084u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c086; /* fmov.s @(r0,r12),fr3 */
L8c61c086: sa_motion_publish_stop(ctx,0x8c61c086u); fcopy(f,1u,4u);goto L8c61c088; /* fmov fr4,fr1 */
L8c61c088: sa_motion_publish_stop(ctx,0x8c61c088u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c08a; /* fsub fr4,fr3 */
L8c61c08a: sa_motion_publish_stop(ctx,0x8c61c08au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c08c; /* fmul fr3,fr2 */
L8c61c08c: sa_motion_publish_stop(ctx,0x8c61c08cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c08e; /* fmov.s @(r0,r14),fr3 */
L8c61c08e: sa_motion_publish_stop(ctx,0x8c61c08eu); r[0]=16u;goto L8c61c090; /* mov #0x10,r0 */
L8c61c090: sa_motion_publish_stop(ctx,0x8c61c090u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61c092; /* fsub fr3,fr1 */
L8c61c092: sa_motion_publish_stop(ctx,0x8c61c092u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c094; /* fmov.s @(r0,r12),fr3 */
L8c61c094: sa_motion_publish_stop(ctx,0x8c61c094u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c096; /* fsub fr5,fr3 */
L8c61c096: sa_motion_publish_stop(ctx,0x8c61c096u); fcopy(f,4u,2u);goto L8c61c098; /* fmov fr2,fr4 */
L8c61c098: sa_motion_publish_stop(ctx,0x8c61c098u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c09a; /* fmul fr3,fr1 */
L8c61c09a: sa_motion_publish_stop(ctx,0x8c61c09au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61c09c; /* fldi0 fr3 */
L8c61c09c: sa_motion_publish_stop(ctx,0x8c61c09cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c09e; /* fsub fr1,fr4 */
L8c61c09e: sa_motion_publish_stop(ctx,0x8c61c09eu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61c0a0; /* fcmp/gt fr3,fr4 */
L8c61c0a0: sa_motion_publish_stop(ctx,0x8c61c0a2u); take=!(*sr&1u);r[11]+=r[4];if(take)goto L8c61c0a8;goto L8c61c0a4; /* bf/s 0x8c61c0a8 */
L8c61c0a4: sa_motion_publish_stop(ctx,0x8c61c0a6u); r[10]+=16u;goto L8c61c1de; /* bra 0x8c61c1de */
L8c61c0a8: sa_motion_publish_stop(ctx,0x8c61c0a8u); fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c61c0aa; /* fmov.s @r14,fr2 */
L8c61c0aa: sa_motion_publish_stop(ctx,0x8c61c0aau); r[0]=8u;goto L8c61c0ac; /* mov #0x8,r0 */
L8c61c0ac: sa_motion_publish_stop(ctx,0x8c61c0acu); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61c0ae; /* fmov.s @r13,fr3 */
L8c61c0ae: sa_motion_publish_stop(ctx,0x8c61c0aeu); r[4]=r[15];goto L8c61c0b0; /* mov r15,r4 */
L8c61c0b0: sa_motion_publish_stop(ctx,0x8c61c0b0u); r[3]=rd(m,bus,&io_status,0x8c61c204u,4);if(io_status)return io_status;goto L8c61c0b2; /* mov.l 0x8c61c204,r3 */
L8c61c0b2: sa_motion_publish_stop(ctx,0x8c61c0b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c0b4; /* fadd fr3,fr2 */
L8c61c0b4: sa_motion_publish_stop(ctx,0x8c61c0b4u); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61c0b6; /* fmov.s @r12,fr3 */
L8c61c0b6: sa_motion_publish_stop(ctx,0x8c61c0b6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c0b8; /* fadd fr3,fr2 */
L8c61c0b8: sa_motion_publish_stop(ctx,0x8c61c0b8u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c0ba; /* fmov fr2,@(r0,r15) */
L8c61c0ba: sa_motion_publish_stop(ctx,0x8c61c0bau); r[0]=4u;goto L8c61c0bc; /* mov #0x4,r0 */
L8c61c0bc: sa_motion_publish_stop(ctx,0x8c61c0bcu); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c0be; /* fmov.s @(r0,r14),fr2 */
L8c61c0be: sa_motion_publish_stop(ctx,0x8c61c0beu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c0c0; /* fmov.s @(r0,r13),fr3 */
L8c61c0c0: sa_motion_publish_stop(ctx,0x8c61c0c0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c0c2; /* fadd fr3,fr2 */
L8c61c0c2: sa_motion_publish_stop(ctx,0x8c61c0c2u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c0c4; /* fmov.s @(r0,r12),fr3 */
L8c61c0c4: sa_motion_publish_stop(ctx,0x8c61c0c4u); r[0]=12u;goto L8c61c0c6; /* mov #0xc,r0 */
L8c61c0c6: sa_motion_publish_stop(ctx,0x8c61c0c6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c0c8; /* fadd fr3,fr2 */
L8c61c0c8: sa_motion_publish_stop(ctx,0x8c61c0c8u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c0ca; /* fmov fr2,@(r0,r15) */
L8c61c0ca: sa_motion_publish_stop(ctx,0x8c61c0cau); r[0]=8u;goto L8c61c0cc; /* mov #0x8,r0 */
L8c61c0cc: sa_motion_publish_stop(ctx,0x8c61c0ccu); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c0ce; /* fmov.s @(r0,r14),fr2 */
L8c61c0ce: sa_motion_publish_stop(ctx,0x8c61c0ceu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c0d0; /* fmov.s @(r0,r13),fr3 */
L8c61c0d0: sa_motion_publish_stop(ctx,0x8c61c0d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c0d2; /* fadd fr3,fr2 */
L8c61c0d2: sa_motion_publish_stop(ctx,0x8c61c0d2u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c0d4; /* fmov.s @(r0,r12),fr3 */
L8c61c0d4: sa_motion_publish_stop(ctx,0x8c61c0d4u); r[0]=16u;goto L8c61c0d6; /* mov #0x10,r0 */
L8c61c0d6: sa_motion_publish_stop(ctx,0x8c61c0d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c0d8; /* fadd fr3,fr2 */
L8c61c0d8: sa_motion_publish_stop(ctx,0x8c61c0d8u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c0da; /* fmov fr2,@(r0,r15) */
L8c61c0da: sa_motion_publish_stop(ctx,0x8c61c0dcu); target=r[3];*pr=0x8c61c0deu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61c0de; /* jsr @r3 */
L8c61c0de: sa_motion_publish_stop(ctx,0x8c61c0deu); r[3]=rd(m,bus,&io_status,0x8c61c208u,4);if(io_status)return io_status;goto L8c61c0e0; /* mov.l 0x8c61c208,r3 */
L8c61c0e0: sa_motion_publish_stop(ctx,0x8c61c0e0u); r[0]=8u;goto L8c61c0e2; /* mov #0x8,r0 */
L8c61c0e2: sa_motion_publish_stop(ctx,0x8c61c0e2u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c0e4; /* fmov.s @(r0,r15),fr3 */
L8c61c0e4: sa_motion_publish_stop(ctx,0x8c61c0e4u); r[0]=12u;goto L8c61c0e6; /* mov #0xc,r0 */
L8c61c0e6: sa_motion_publish_stop(ctx,0x8c61c0e6u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61c0e8; /* fmov.s @r3,fr2 */
L8c61c0e8: sa_motion_publish_stop(ctx,0x8c61c0e8u); r[8]+=24u;goto L8c61c0ea; /* add #0x18,r8 */
L8c61c0ea: sa_motion_publish_stop(ctx,0x8c61c0eau); r[2]=rd(m,bus,&io_status,0x8c61c20cu,4);if(io_status)return io_status;goto L8c61c0ec; /* mov.l 0x8c61c20c,r2 */
L8c61c0ec: sa_motion_publish_stop(ctx,0x8c61c0ecu); r[3]+=8u;goto L8c61c0ee; /* add #0x8,r3 */
L8c61c0ee: sa_motion_publish_stop(ctx,0x8c61c0eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c0f0; /* fmul fr3,fr2 */
L8c61c0f0: sa_motion_publish_stop(ctx,0x8c61c0f0u); r[1]=rd(m,bus,&io_status,0x8c61c210u,4);if(io_status)return io_status;goto L8c61c0f2; /* mov.l 0x8c61c210,r1 */
L8c61c0f2: sa_motion_publish_stop(ctx,0x8c61c0f2u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61c0f4; /* fmov.s @r2,fr3 */
L8c61c0f4: sa_motion_publish_stop(ctx,0x8c61c0f4u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61c0f6; /* fmov.s @r1,fr1 */
L8c61c0f6: sa_motion_publish_stop(ctx,0x8c61c0f6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c0f8; /* fsub fr2,fr3 */
L8c61c0f8: sa_motion_publish_stop(ctx,0x8c61c0f8u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c0fa; /* fmov.s @(r0,r15),fr2 */
L8c61c0fa: sa_motion_publish_stop(ctx,0x8c61c0fau); r[0]=16u;goto L8c61c0fc; /* mov #0x10,r0 */
L8c61c0fc: sa_motion_publish_stop(ctx,0x8c61c0fcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c0fe; /* fmul fr2,fr1 */
L8c61c0fe: sa_motion_publish_stop(ctx,0x8c61c0feu); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c100; /* fmov.s @(r0,r15),fr2 */
L8c61c100: sa_motion_publish_stop(ctx,0x8c61c100u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c102; /* mov.w @(0x2,r10),r0 */
L8c61c102: sa_motion_publish_stop(ctx,0x8c61c102u); r[2]=r[0];goto L8c61c104; /* mov r0,r2 */
L8c61c104: sa_motion_publish_stop(ctx,0x8c61c104u); *fpul=r[2];goto L8c61c106; /* lds r2,FPUL */
L8c61c106: sa_motion_publish_stop(ctx,0x8c61c106u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c108; /* fsub fr1,fr3 */
L8c61c108: sa_motion_publish_stop(ctx,0x8c61c108u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61c10a; /* fmov.s @r3,fr1 */
L8c61c10a: sa_motion_publish_stop(ctx,0x8c61c10au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c10c; /* fmul fr2,fr1 */
L8c61c10c: sa_motion_publish_stop(ctx,0x8c61c10cu); fcopy(f,4u,3u);goto L8c61c10e; /* fmov fr3,fr4 */
L8c61c10e: sa_motion_publish_stop(ctx,0x8c61c10eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c110; /* float FPUL,fr3 */
L8c61c110: sa_motion_publish_stop(ctx,0x8c61c110u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c112; /* fsub fr1,fr4 */
L8c61c112: sa_motion_publish_stop(ctx,0x8c61c112u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c114; /* fmul fr15,fr3 */
L8c61c114: sa_motion_publish_stop(ctx,0x8c61c114u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c116; /* fmov.s fr3,@-r8 */
L8c61c116: sa_motion_publish_stop(ctx,0x8c61c116u); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c118; /* mov.w @r10,r2 */
L8c61c118: sa_motion_publish_stop(ctx,0x8c61c118u); *fpul=r[2];goto L8c61c11a; /* lds r2,FPUL */
L8c61c11a: sa_motion_publish_stop(ctx,0x8c61c11au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c11c; /* float FPUL,fr3 */
L8c61c11c: sa_motion_publish_stop(ctx,0x8c61c11cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c11e; /* fmul fr15,fr3 */
L8c61c11e: sa_motion_publish_stop(ctx,0x8c61c11eu); r[0]=24u;goto L8c61c120; /* mov #0x18,r0 */
L8c61c120: sa_motion_publish_stop(ctx,0x8c61c120u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c122; /* fmov.s fr3,@-r8 */
L8c61c122: sa_motion_publish_stop(ctx,0x8c61c122u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c124; /* fmov.s @(r0,r14),fr3 */
L8c61c124: sa_motion_publish_stop(ctx,0x8c61c124u); r[0]=20u;goto L8c61c126; /* mov #0x14,r0 */
L8c61c126: sa_motion_publish_stop(ctx,0x8c61c126u); r[2]=rd(m,bus,&io_status,0x8c61c214u,4);if(io_status)return io_status;goto L8c61c128; /* mov.l 0x8c61c214,r2 */
L8c61c128: sa_motion_publish_stop(ctx,0x8c61c128u); r[10]+=4u;goto L8c61c12a; /* add #0x4,r10 */
L8c61c12a: sa_motion_publish_stop(ctx,0x8c61c12au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c12c; /* fmov.s fr3,@-r8 */
L8c61c12c: sa_motion_publish_stop(ctx,0x8c61c12cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c12e; /* fmov.s @(r0,r14),fr3 */
L8c61c12e: sa_motion_publish_stop(ctx,0x8c61c12eu); r[0]=16u;goto L8c61c130; /* mov #0x10,r0 */
L8c61c130: sa_motion_publish_stop(ctx,0x8c61c130u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c132; /* fmov.s fr3,@-r8 */
L8c61c132: sa_motion_publish_stop(ctx,0x8c61c132u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c134; /* fmov.s @(r0,r14),fr3 */
L8c61c134: sa_motion_publish_stop(ctx,0x8c61c134u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c136; /* fmov.s fr3,@-r8 */
L8c61c136: sa_motion_publish_stop(ctx,0x8c61c136u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c138; /* mov.l r2,@-r8 */
L8c61c138: sa_motion_publish_stop(ctx,0x8c61c138u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c13a; /* pref @r8 */
L8c61c13a: sa_motion_publish_stop(ctx,0x8c61c13au); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c13c; /* mov.w @(0x2,r10),r0 */
L8c61c13c: sa_motion_publish_stop(ctx,0x8c61c13cu); r[8]+=56u;goto L8c61c13e; /* add #0x38,r8 */
L8c61c13e: sa_motion_publish_stop(ctx,0x8c61c13eu); r[2]=r[0];goto L8c61c140; /* mov r0,r2 */
L8c61c140: sa_motion_publish_stop(ctx,0x8c61c140u); *fpul=r[2];goto L8c61c142; /* lds r2,FPUL */
L8c61c142: sa_motion_publish_stop(ctx,0x8c61c142u); r[0]=24u;goto L8c61c144; /* mov #0x18,r0 */
L8c61c144: sa_motion_publish_stop(ctx,0x8c61c144u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c146; /* float FPUL,fr3 */
L8c61c146: sa_motion_publish_stop(ctx,0x8c61c146u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c148; /* fmul fr15,fr3 */
L8c61c148: sa_motion_publish_stop(ctx,0x8c61c148u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c14a; /* fmov.s fr3,@-r8 */
L8c61c14a: sa_motion_publish_stop(ctx,0x8c61c14au); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c14c; /* mov.w @r10,r2 */
L8c61c14c: sa_motion_publish_stop(ctx,0x8c61c14cu); r[10]+=4u;goto L8c61c14e; /* add #0x4,r10 */
L8c61c14e: sa_motion_publish_stop(ctx,0x8c61c14eu); *fpul=r[2];goto L8c61c150; /* lds r2,FPUL */
L8c61c150: sa_motion_publish_stop(ctx,0x8c61c150u); r[2]=rd(m,bus,&io_status,0x8c61c214u,4);if(io_status)return io_status;goto L8c61c152; /* mov.l 0x8c61c214,r2 */
L8c61c152: sa_motion_publish_stop(ctx,0x8c61c152u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c154; /* float FPUL,fr3 */
L8c61c154: sa_motion_publish_stop(ctx,0x8c61c154u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c156; /* fmul fr15,fr3 */
L8c61c156: sa_motion_publish_stop(ctx,0x8c61c156u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c158; /* fmov.s fr3,@-r8 */
L8c61c158: sa_motion_publish_stop(ctx,0x8c61c158u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c15a; /* fmov.s @(r0,r13),fr3 */
L8c61c15a: sa_motion_publish_stop(ctx,0x8c61c15au); r[0]=20u;goto L8c61c15c; /* mov #0x14,r0 */
L8c61c15c: sa_motion_publish_stop(ctx,0x8c61c15cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c15e; /* fmov.s fr3,@-r8 */
L8c61c15e: sa_motion_publish_stop(ctx,0x8c61c15eu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c160; /* fmov.s @(r0,r13),fr3 */
L8c61c160: sa_motion_publish_stop(ctx,0x8c61c160u); r[0]=16u;goto L8c61c162; /* mov #0x10,r0 */
L8c61c162: sa_motion_publish_stop(ctx,0x8c61c162u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c164; /* fmov.s fr3,@-r8 */
L8c61c164: sa_motion_publish_stop(ctx,0x8c61c164u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c166; /* fmov.s @(r0,r13),fr3 */
L8c61c166: sa_motion_publish_stop(ctx,0x8c61c166u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c168; /* fmov.s fr3,@-r8 */
L8c61c168: sa_motion_publish_stop(ctx,0x8c61c168u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c16a; /* mov.l r2,@-r8 */
L8c61c16a: sa_motion_publish_stop(ctx,0x8c61c16au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c16c; /* pref @r8 */
L8c61c16c: sa_motion_publish_stop(ctx,0x8c61c16cu); r[8]+=60u;goto L8c61c16e; /* add #0x3c,r8 */
L8c61c16e: sa_motion_publish_stop(ctx,0x8c61c16eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61c170; /* fmov.s fr4,@-r8 */
L8c61c170: sa_motion_publish_stop(ctx,0x8c61c170u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c172; /* mov.w @(0x2,r10),r0 */
L8c61c172: sa_motion_publish_stop(ctx,0x8c61c172u); r[2]=r[0];goto L8c61c174; /* mov r0,r2 */
L8c61c174: sa_motion_publish_stop(ctx,0x8c61c174u); *fpul=r[2];goto L8c61c176; /* lds r2,FPUL */
L8c61c176: sa_motion_publish_stop(ctx,0x8c61c176u); r[0]=24u;goto L8c61c178; /* mov #0x18,r0 */
L8c61c178: sa_motion_publish_stop(ctx,0x8c61c178u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c17a; /* float FPUL,fr3 */
L8c61c17a: sa_motion_publish_stop(ctx,0x8c61c17au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c17c; /* fmul fr15,fr3 */
L8c61c17c: sa_motion_publish_stop(ctx,0x8c61c17cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c17e; /* fmov.s fr3,@-r8 */
L8c61c17e: sa_motion_publish_stop(ctx,0x8c61c17eu); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c180; /* mov.w @r10,r2 */
L8c61c180: sa_motion_publish_stop(ctx,0x8c61c180u); r[10]+=4u;goto L8c61c182; /* add #0x4,r10 */
L8c61c182: sa_motion_publish_stop(ctx,0x8c61c182u); *fpul=r[2];goto L8c61c184; /* lds r2,FPUL */
L8c61c184: sa_motion_publish_stop(ctx,0x8c61c184u); r[2]=rd(m,bus,&io_status,0x8c61c214u,4);if(io_status)return io_status;goto L8c61c186; /* mov.l 0x8c61c214,r2 */
L8c61c186: sa_motion_publish_stop(ctx,0x8c61c186u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c188; /* float FPUL,fr3 */
L8c61c188: sa_motion_publish_stop(ctx,0x8c61c188u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c18a; /* fmul fr15,fr3 */
L8c61c18a: sa_motion_publish_stop(ctx,0x8c61c18au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c18c; /* fmov.s fr3,@-r8 */
L8c61c18c: sa_motion_publish_stop(ctx,0x8c61c18cu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c18e; /* fmov.s @(r0,r12),fr3 */
L8c61c18e: sa_motion_publish_stop(ctx,0x8c61c18eu); r[0]=20u;goto L8c61c190; /* mov #0x14,r0 */
L8c61c190: sa_motion_publish_stop(ctx,0x8c61c190u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c192; /* fmov.s fr3,@-r8 */
L8c61c192: sa_motion_publish_stop(ctx,0x8c61c192u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c194; /* fmov.s @(r0,r12),fr3 */
L8c61c194: sa_motion_publish_stop(ctx,0x8c61c194u); r[0]=16u;goto L8c61c196; /* mov #0x10,r0 */
L8c61c196: sa_motion_publish_stop(ctx,0x8c61c196u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c198; /* fmov.s fr3,@-r8 */
L8c61c198: sa_motion_publish_stop(ctx,0x8c61c198u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c19a; /* fmov.s @(r0,r12),fr3 */
L8c61c19a: sa_motion_publish_stop(ctx,0x8c61c19au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c19c; /* fmov.s fr3,@-r8 */
L8c61c19c: sa_motion_publish_stop(ctx,0x8c61c19cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c19e; /* mov.l r2,@-r8 */
L8c61c19e: sa_motion_publish_stop(ctx,0x8c61c19eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c1a0; /* pref @r8 */
L8c61c1a0: sa_motion_publish_stop(ctx,0x8c61c1a0u); r[8]+=60u;goto L8c61c1a2; /* add #0x3c,r8 */
L8c61c1a2: sa_motion_publish_stop(ctx,0x8c61c1a2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61c1a4; /* fmov.s fr4,@-r8 */
L8c61c1a4: sa_motion_publish_stop(ctx,0x8c61c1a4u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c1a6; /* mov.w @(0x2,r10),r0 */
L8c61c1a6: sa_motion_publish_stop(ctx,0x8c61c1a6u); r[2]=r[0];goto L8c61c1a8; /* mov r0,r2 */
L8c61c1a8: sa_motion_publish_stop(ctx,0x8c61c1a8u); *fpul=r[2];goto L8c61c1aa; /* lds r2,FPUL */
L8c61c1aa: sa_motion_publish_stop(ctx,0x8c61c1aau); r[0]=24u;goto L8c61c1ac; /* mov #0x18,r0 */
L8c61c1ac: sa_motion_publish_stop(ctx,0x8c61c1acu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c1ae; /* float FPUL,fr3 */
L8c61c1ae: sa_motion_publish_stop(ctx,0x8c61c1aeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c1b0; /* fmul fr15,fr3 */
L8c61c1b0: sa_motion_publish_stop(ctx,0x8c61c1b0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c1b2; /* fmov.s fr3,@-r8 */
L8c61c1b2: sa_motion_publish_stop(ctx,0x8c61c1b2u); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c1b4; /* mov.w @r10,r2 */
L8c61c1b4: sa_motion_publish_stop(ctx,0x8c61c1b4u); r[10]+=4u;goto L8c61c1b6; /* add #0x4,r10 */
L8c61c1b6: sa_motion_publish_stop(ctx,0x8c61c1b6u); *fpul=r[2];goto L8c61c1b8; /* lds r2,FPUL */
L8c61c1b8: sa_motion_publish_stop(ctx,0x8c61c1b8u); r[2]=rd(m,bus,&io_status,0x8c61c218u,4);if(io_status)return io_status;goto L8c61c1ba; /* mov.l 0x8c61c218,r2 */
L8c61c1ba: sa_motion_publish_stop(ctx,0x8c61c1bau); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c1bc; /* float FPUL,fr3 */
L8c61c1bc: sa_motion_publish_stop(ctx,0x8c61c1bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c1be; /* fmul fr15,fr3 */
L8c61c1be: sa_motion_publish_stop(ctx,0x8c61c1beu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c1c0; /* fmov.s fr3,@-r8 */
L8c61c1c0: sa_motion_publish_stop(ctx,0x8c61c1c0u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c1c2; /* fmov.s @(r0,r11),fr3 */
L8c61c1c2: sa_motion_publish_stop(ctx,0x8c61c1c2u); r[0]=20u;goto L8c61c1c4; /* mov #0x14,r0 */
L8c61c1c4: sa_motion_publish_stop(ctx,0x8c61c1c4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c1c6; /* fmov.s fr3,@-r8 */
L8c61c1c6: sa_motion_publish_stop(ctx,0x8c61c1c6u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c1c8; /* fmov.s @(r0,r11),fr3 */
L8c61c1c8: sa_motion_publish_stop(ctx,0x8c61c1c8u); r[0]=16u;goto L8c61c1ca; /* mov #0x10,r0 */
L8c61c1ca: sa_motion_publish_stop(ctx,0x8c61c1cau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c1cc; /* fmov.s fr3,@-r8 */
L8c61c1cc: sa_motion_publish_stop(ctx,0x8c61c1ccu); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c1ce; /* fmov.s @(r0,r11),fr3 */
L8c61c1ce: sa_motion_publish_stop(ctx,0x8c61c1ceu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c1d0; /* fmov.s fr3,@-r8 */
L8c61c1d0: sa_motion_publish_stop(ctx,0x8c61c1d0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c1d2; /* mov.l r2,@-r8 */
L8c61c1d2: sa_motion_publish_stop(ctx,0x8c61c1d2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c1d4; /* pref @r8 */
L8c61c1d4: sa_motion_publish_stop(ctx,0x8c61c1d4u); r[0]=rd(m,bus,&io_status,0x8c61c21cu,4);if(io_status)return io_status;goto L8c61c1d6; /* mov.l 0x8c61c21c,r0 */
L8c61c1d6: sa_motion_publish_stop(ctx,0x8c61c1d6u); r[8]+=32u;goto L8c61c1d8; /* add #0x20,r8 */
L8c61c1d8: sa_motion_publish_stop(ctx,0x8c61c1d8u); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61c1da; /* mov.l @r0,r2 */
L8c61c1da: sa_motion_publish_stop(ctx,0x8c61c1dau); r[2]+=4u;goto L8c61c1dc; /* add #0x4,r2 */
L8c61c1dc: sa_motion_publish_stop(ctx,0x8c61c1dcu); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61c1de; /* mov.l r2,@r0 */
L8c61c1de: sa_motion_publish_stop(ctx,0x8c61c1deu); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61c1e0; /* mov.l @(0x4,r15),r3 */
L8c61c1e0: sa_motion_publish_stop(ctx,0x8c61c1e0u); r[3]+=1u;goto L8c61c1e2; /* add #0x1,r3 */
L8c61c1e2: sa_motion_publish_stop(ctx,0x8c61c1e2u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61c1e4; /* mov.l r3,@(0x4,r15) */
L8c61c1e4: sa_motion_publish_stop(ctx,0x8c61c1e4u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61c1e6; /* mov.l @r15,r2 */
L8c61c1e6: sa_motion_publish_stop(ctx,0x8c61c1e6u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61c1e8; /* mov.l @(0x4,r15),r3 */
L8c61c1e8: sa_motion_publish_stop(ctx,0x8c61c1e8u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61c1ea; /* mov.w @(0x2,r2),r0 */
L8c61c1ea: sa_motion_publish_stop(ctx,0x8c61c1eau); r[0]=r[0]&65535u;goto L8c61c1ec; /* extu.w r0,r0 */
L8c61c1ec: sa_motion_publish_stop(ctx,0x8c61c1ecu); *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c61c1ee; /* cmp/ge r0,r3 */
L8c61c1ee: sa_motion_publish_stop(ctx,0x8c61c1eeu); if((*sr&1u))goto L8c61c1f4;goto L8c61c1f0; /* bt 0x8c61c1f4 */
L8c61c1f0: sa_motion_publish_stop(ctx,0x8c61c1f2u); goto L8c61c044; /* bra 0x8c61c044 */
L8c61c1f4: sa_motion_publish_stop(ctx,0x8c61c1f6u); goto L8c61c46a; /* bra 0x8c61c46a */
L8c61c1f8: sa_motion_publish_stop(ctx,0x8c61c1f8u); r[11]=2u;goto L8c61c1fa; /* mov #0x2,r11 */
L8c61c1fa: sa_motion_publish_stop(ctx,0x8c61c1fcu); r[12]=r[4];goto L8c61c41a; /* bra 0x8c61c41a */
L8c61c224: sa_motion_publish_stop(ctx,0x8c61c224u); r[14]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c226; /* mov.w @r9+,r14 */
L8c61c226: sa_motion_publish_stop(ctx,0x8c61c226u); r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61c228; /* mov.l @r13,r3 */
L8c61c228: sa_motion_publish_stop(ctx,0x8c61c228u); r[14]=r[14]&65535u;goto L8c61c22a; /* extu.w r14,r14 */
L8c61c22a: sa_motion_publish_stop(ctx,0x8c61c22au); r[3]+=r[14];goto L8c61c22c; /* add r14,r3 */
L8c61c22c: sa_motion_publish_stop(ctx,0x8c61c22cu); wr(m,bus,&io_status,r[13],r[3],4);if(io_status)return io_status;goto L8c61c22e; /* mov.l r3,@r13 */
L8c61c22e: sa_motion_publish_stop(ctx,0x8c61c22eu); r[0]=sx(m,bus,&io_status,r[9]+0x2u,2);if(io_status)return io_status;goto L8c61c230; /* mov.w @(0x2,r9),r0 */
L8c61c230: sa_motion_publish_stop(ctx,0x8c61c230u); r[6]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;goto L8c61c232; /* mov.w @r9,r6 */
L8c61c232: sa_motion_publish_stop(ctx,0x8c61c232u); r[3]=rd(m,bus,&io_status,0x8c61c48cu,4);if(io_status)return io_status;goto L8c61c234; /* mov.l 0x8c61c48c,r3 */
L8c61c234: sa_motion_publish_stop(ctx,0x8c61c234u); r[4]=r[0]&65535u;goto L8c61c236; /* extu.w r0,r4 */
L8c61c236: sa_motion_publish_stop(ctx,0x8c61c236u); r[0]=sx(m,bus,&io_status,r[9]+0x4u,2);if(io_status)return io_status;goto L8c61c238; /* mov.w @(0x4,r9),r0 */
L8c61c238: sa_motion_publish_stop(ctx,0x8c61c238u); r[4]<<=2;goto L8c61c23a; /* shll2 r4 */
L8c61c23a: sa_motion_publish_stop(ctx,0x8c61c23au); r[6]=r[6]&65535u;goto L8c61c23c; /* extu.w r6,r6 */
L8c61c23c: sa_motion_publish_stop(ctx,0x8c61c23cu); r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61c23e; /* mov.l @r3,r7 */
L8c61c23e: sa_motion_publish_stop(ctx,0x8c61c23eu); r[4]<<=2;goto L8c61c240; /* shll2 r4 */
L8c61c240: sa_motion_publish_stop(ctx,0x8c61c240u); r[6]<<=2;goto L8c61c242; /* shll2 r6 */
L8c61c242: sa_motion_publish_stop(ctx,0x8c61c242u); r[5]=r[0]&65535u;goto L8c61c244; /* extu.w r0,r5 */
L8c61c244: sa_motion_publish_stop(ctx,0x8c61c244u); r[6]<<=2;goto L8c61c246; /* shll2 r6 */
L8c61c246: sa_motion_publish_stop(ctx,0x8c61c246u); r[0]=16u;goto L8c61c248; /* mov #0x10,r0 */
L8c61c248: sa_motion_publish_stop(ctx,0x8c61c248u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61c24a; /* shll r6 */
L8c61c24a: sa_motion_publish_stop(ctx,0x8c61c24au); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61c24c; /* shll r4 */
L8c61c24c: sa_motion_publish_stop(ctx,0x8c61c24cu); r[4]+=r[7];goto L8c61c24e; /* add r7,r4 */
L8c61c24e: sa_motion_publish_stop(ctx,0x8c61c24eu); fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c250; /* fmov.s @(r0,r4),fr5 */
L8c61c250: sa_motion_publish_stop(ctx,0x8c61c250u); r[6]+=r[7];goto L8c61c252; /* add r7,r6 */
L8c61c252: sa_motion_publish_stop(ctx,0x8c61c252u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61c254; /* fmov.s @(r0,r6),fr3 */
L8c61c254: sa_motion_publish_stop(ctx,0x8c61c254u); r[5]<<=2;goto L8c61c256; /* shll2 r5 */
L8c61c256: sa_motion_publish_stop(ctx,0x8c61c256u); fcopy(f,2u,5u);goto L8c61c258; /* fmov fr5,fr2 */
L8c61c258: sa_motion_publish_stop(ctx,0x8c61c258u); r[0]=20u;goto L8c61c25a; /* mov #0x14,r0 */
L8c61c25a: sa_motion_publish_stop(ctx,0x8c61c25au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61c25c; /* fsub fr3,fr2 */
L8c61c25c: sa_motion_publish_stop(ctx,0x8c61c25cu); r[5]<<=2;goto L8c61c25e; /* shll2 r5 */
L8c61c25e: sa_motion_publish_stop(ctx,0x8c61c25eu); fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c260; /* fmov.s @(r0,r4),fr4 */
L8c61c260: sa_motion_publish_stop(ctx,0x8c61c260u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61c262; /* shll r5 */
L8c61c262: sa_motion_publish_stop(ctx,0x8c61c262u); r[5]+=r[7];goto L8c61c264; /* add r7,r5 */
L8c61c264: sa_motion_publish_stop(ctx,0x8c61c264u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61c266; /* fmov.s @(r0,r5),fr3 */
L8c61c266: sa_motion_publish_stop(ctx,0x8c61c266u); fcopy(f,1u,4u);goto L8c61c268; /* fmov fr4,fr1 */
L8c61c268: sa_motion_publish_stop(ctx,0x8c61c268u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c26a; /* fsub fr4,fr3 */
L8c61c26a: sa_motion_publish_stop(ctx,0x8c61c26au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c26c; /* fmul fr3,fr2 */
L8c61c26c: sa_motion_publish_stop(ctx,0x8c61c26cu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61c26e; /* fmov.s @(r0,r6),fr3 */
L8c61c26e: sa_motion_publish_stop(ctx,0x8c61c26eu); r[0]=16u;goto L8c61c270; /* mov #0x10,r0 */
L8c61c270: sa_motion_publish_stop(ctx,0x8c61c270u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61c272; /* fsub fr3,fr1 */
L8c61c272: sa_motion_publish_stop(ctx,0x8c61c272u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61c274; /* fmov.s @(r0,r5),fr3 */
L8c61c274: sa_motion_publish_stop(ctx,0x8c61c274u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c276; /* fsub fr5,fr3 */
L8c61c276: sa_motion_publish_stop(ctx,0x8c61c276u); fcopy(f,4u,2u);goto L8c61c278; /* fmov fr2,fr4 */
L8c61c278: sa_motion_publish_stop(ctx,0x8c61c278u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c27a; /* fmul fr3,fr1 */
L8c61c27a: sa_motion_publish_stop(ctx,0x8c61c27au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61c27c; /* fldi0 fr3 */
L8c61c27c: sa_motion_publish_stop(ctx,0x8c61c27cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c27e; /* fsub fr1,fr4 */
L8c61c27e: sa_motion_publish_stop(ctx,0x8c61c27eu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61c280; /* fcmp/gt fr3,fr4 */
L8c61c280: sa_motion_publish_stop(ctx,0x8c61c280u); if(!(*sr&1u))goto L8c61c28e;goto L8c61c282; /* bf 0x8c61c28e */
L8c61c282: sa_motion_publish_stop(ctx,0x8c61c282u); r[1]=r[14];goto L8c61c284; /* mov r14,r1 */
L8c61c284: sa_motion_publish_stop(ctx,0x8c61c284u); *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c61c286; /* shll r1 */
L8c61c286: sa_motion_publish_stop(ctx,0x8c61c286u); r[14]<<=2;goto L8c61c288; /* shll2 r14 */
L8c61c288: sa_motion_publish_stop(ctx,0x8c61c288u); r[9]+=r[1];goto L8c61c28a; /* add r1,r9 */
L8c61c28a: sa_motion_publish_stop(ctx,0x8c61c28cu); r[10]+=r[14];goto L8c61c418; /* bra 0x8c61c418 */
L8c61c28e: sa_motion_publish_stop(ctx,0x8c61c28eu); r[0]=8u;goto L8c61c290; /* mov #0x8,r0 */
L8c61c290: sa_motion_publish_stop(ctx,0x8c61c290u); r[3]=rd(m,bus,&io_status,0x8c61c490u,4);if(io_status)return io_status;goto L8c61c292; /* mov.l 0x8c61c490,r3 */
L8c61c292: sa_motion_publish_stop(ctx,0x8c61c292u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61c294; /* mov.l @r3,r2 */
L8c61c294: sa_motion_publish_stop(ctx,0x8c61c294u); r[2]+=r[14];goto L8c61c296; /* add r14,r2 */
L8c61c296: sa_motion_publish_stop(ctx,0x8c61c296u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61c298; /* mov.l r2,@r3 */
L8c61c298: sa_motion_publish_stop(ctx,0x8c61c298u); fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61c29a; /* fmov.s @r4,fr3 */
L8c61c29a: sa_motion_publish_stop(ctx,0x8c61c29au); fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c61c29c; /* fmov.s @r6,fr2 */
L8c61c29c: sa_motion_publish_stop(ctx,0x8c61c29cu); r[3]=rd(m,bus,&io_status,0x8c61c494u,4);if(io_status)return io_status;goto L8c61c29e; /* mov.l 0x8c61c494,r3 */
L8c61c29e: sa_motion_publish_stop(ctx,0x8c61c29eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c2a0; /* fadd fr3,fr2 */
L8c61c2a0: sa_motion_publish_stop(ctx,0x8c61c2a0u); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61c2a2; /* fmov.s @r5,fr3 */
L8c61c2a2: sa_motion_publish_stop(ctx,0x8c61c2a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c2a4; /* fadd fr3,fr2 */
L8c61c2a4: sa_motion_publish_stop(ctx,0x8c61c2a4u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c2a6; /* fmov fr2,@(r0,r15) */
L8c61c2a6: sa_motion_publish_stop(ctx,0x8c61c2a6u); r[0]=4u;goto L8c61c2a8; /* mov #0x4,r0 */
L8c61c2a8: sa_motion_publish_stop(ctx,0x8c61c2a8u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61c2aa; /* fmov.s @(r0,r6),fr2 */
L8c61c2aa: sa_motion_publish_stop(ctx,0x8c61c2aau); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c2ac; /* fmov.s @(r0,r4),fr3 */
L8c61c2ac: sa_motion_publish_stop(ctx,0x8c61c2acu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c2ae; /* fadd fr3,fr2 */
L8c61c2ae: sa_motion_publish_stop(ctx,0x8c61c2aeu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61c2b0; /* fmov.s @(r0,r5),fr3 */
L8c61c2b0: sa_motion_publish_stop(ctx,0x8c61c2b0u); r[0]=12u;goto L8c61c2b2; /* mov #0xc,r0 */
L8c61c2b2: sa_motion_publish_stop(ctx,0x8c61c2b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c2b4; /* fadd fr3,fr2 */
L8c61c2b4: sa_motion_publish_stop(ctx,0x8c61c2b4u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c2b6; /* fmov fr2,@(r0,r15) */
L8c61c2b6: sa_motion_publish_stop(ctx,0x8c61c2b6u); r[0]=8u;goto L8c61c2b8; /* mov #0x8,r0 */
L8c61c2b8: sa_motion_publish_stop(ctx,0x8c61c2b8u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c2ba; /* fmov.s @(r0,r4),fr3 */
L8c61c2ba: sa_motion_publish_stop(ctx,0x8c61c2bau); r[4]=r[15];goto L8c61c2bc; /* mov r15,r4 */
L8c61c2bc: sa_motion_publish_stop(ctx,0x8c61c2bcu); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61c2be; /* fmov.s @(r0,r6),fr2 */
L8c61c2be: sa_motion_publish_stop(ctx,0x8c61c2beu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c2c0; /* fadd fr3,fr2 */
L8c61c2c0: sa_motion_publish_stop(ctx,0x8c61c2c0u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61c2c2; /* fmov.s @(r0,r5),fr3 */
L8c61c2c2: sa_motion_publish_stop(ctx,0x8c61c2c2u); r[0]=16u;goto L8c61c2c4; /* mov #0x10,r0 */
L8c61c2c4: sa_motion_publish_stop(ctx,0x8c61c2c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c2c6; /* fadd fr3,fr2 */
L8c61c2c6: sa_motion_publish_stop(ctx,0x8c61c2c6u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c2c8; /* fmov fr2,@(r0,r15) */
L8c61c2c8: sa_motion_publish_stop(ctx,0x8c61c2cau); target=r[3];*pr=0x8c61c2ccu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61c2cc; /* jsr @r3 */
L8c61c2cc: sa_motion_publish_stop(ctx,0x8c61c2ccu); r[3]=rd(m,bus,&io_status,0x8c61c498u,4);if(io_status)return io_status;goto L8c61c2ce; /* mov.l 0x8c61c498,r3 */
L8c61c2ce: sa_motion_publish_stop(ctx,0x8c61c2ceu); r[0]=8u;goto L8c61c2d0; /* mov #0x8,r0 */
L8c61c2d0: sa_motion_publish_stop(ctx,0x8c61c2d0u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c2d2; /* fmov.s @(r0,r15),fr3 */
L8c61c2d2: sa_motion_publish_stop(ctx,0x8c61c2d2u); r[0]=12u;goto L8c61c2d4; /* mov #0xc,r0 */
L8c61c2d4: sa_motion_publish_stop(ctx,0x8c61c2d4u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61c2d6; /* fmov.s @r3,fr2 */
L8c61c2d6: sa_motion_publish_stop(ctx,0x8c61c2d6u); r[3]+=8u;goto L8c61c2d8; /* add #0x8,r3 */
L8c61c2d8: sa_motion_publish_stop(ctx,0x8c61c2d8u); r[2]=rd(m,bus,&io_status,0x8c61c49cu,4);if(io_status)return io_status;goto L8c61c2da; /* mov.l 0x8c61c49c,r2 */
L8c61c2da: sa_motion_publish_stop(ctx,0x8c61c2dau); r[8]+=24u;goto L8c61c2dc; /* add #0x18,r8 */
L8c61c2dc: sa_motion_publish_stop(ctx,0x8c61c2dcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c2de; /* fmul fr3,fr2 */
L8c61c2de: sa_motion_publish_stop(ctx,0x8c61c2deu); r[1]=rd(m,bus,&io_status,0x8c61c4a0u,4);if(io_status)return io_status;goto L8c61c2e0; /* mov.l 0x8c61c4a0,r1 */
L8c61c2e0: sa_motion_publish_stop(ctx,0x8c61c2e0u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61c2e2; /* fmov.s @r2,fr3 */
L8c61c2e2: sa_motion_publish_stop(ctx,0x8c61c2e2u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61c2e4; /* fmov.s @r1,fr1 */
L8c61c2e4: sa_motion_publish_stop(ctx,0x8c61c2e4u); r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c2e6; /* mov.w @r9+,r4 */
L8c61c2e6: sa_motion_publish_stop(ctx,0x8c61c2e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c2e8; /* fsub fr2,fr3 */
L8c61c2e8: sa_motion_publish_stop(ctx,0x8c61c2e8u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c2ea; /* fmov.s @(r0,r15),fr2 */
L8c61c2ea: sa_motion_publish_stop(ctx,0x8c61c2eau); r[0]=16u;goto L8c61c2ec; /* mov #0x10,r0 */
L8c61c2ec: sa_motion_publish_stop(ctx,0x8c61c2ecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c2ee; /* fmul fr2,fr1 */
L8c61c2ee: sa_motion_publish_stop(ctx,0x8c61c2eeu); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c2f0; /* fmov.s @(r0,r15),fr2 */
L8c61c2f0: sa_motion_publish_stop(ctx,0x8c61c2f0u); r[0]=rd(m,bus,&io_status,0x8c61c48cu,4);if(io_status)return io_status;goto L8c61c2f2; /* mov.l 0x8c61c48c,r0 */
L8c61c2f2: sa_motion_publish_stop(ctx,0x8c61c2f2u); r[4]=r[4]&65535u;goto L8c61c2f4; /* extu.w r4,r4 */
L8c61c2f4: sa_motion_publish_stop(ctx,0x8c61c2f4u); r[4]<<=2;goto L8c61c2f6; /* shll2 r4 */
L8c61c2f6: sa_motion_publish_stop(ctx,0x8c61c2f6u); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61c2f8; /* mov.l @r0,r2 */
L8c61c2f8: sa_motion_publish_stop(ctx,0x8c61c2f8u); r[4]<<=2;goto L8c61c2fa; /* shll2 r4 */
L8c61c2fa: sa_motion_publish_stop(ctx,0x8c61c2fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c2fc; /* fsub fr1,fr3 */
L8c61c2fc: sa_motion_publish_stop(ctx,0x8c61c2fcu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61c2fe; /* fmov.s @r3,fr1 */
L8c61c2fe: sa_motion_publish_stop(ctx,0x8c61c2feu); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c300; /* mov.w @(0x2,r10),r0 */
L8c61c300: sa_motion_publish_stop(ctx,0x8c61c300u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61c302; /* shll r4 */
L8c61c302: sa_motion_publish_stop(ctx,0x8c61c302u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c304; /* fmul fr2,fr1 */
L8c61c304: sa_motion_publish_stop(ctx,0x8c61c304u); r[4]+=r[2];goto L8c61c306; /* add r2,r4 */
L8c61c306: sa_motion_publish_stop(ctx,0x8c61c306u); r[3]=r[0];goto L8c61c308; /* mov r0,r3 */
L8c61c308: sa_motion_publish_stop(ctx,0x8c61c308u); fcopy(f,4u,3u);goto L8c61c30a; /* fmov fr3,fr4 */
L8c61c30a: sa_motion_publish_stop(ctx,0x8c61c30au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c30c; /* fsub fr1,fr4 */
L8c61c30c: sa_motion_publish_stop(ctx,0x8c61c30cu); *fpul=r[3];goto L8c61c30e; /* lds r3,FPUL */
L8c61c30e: sa_motion_publish_stop(ctx,0x8c61c30eu); r[0]=24u;goto L8c61c310; /* mov #0x18,r0 */
L8c61c310: sa_motion_publish_stop(ctx,0x8c61c310u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c312; /* float FPUL,fr3 */
L8c61c312: sa_motion_publish_stop(ctx,0x8c61c312u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c314; /* fmul fr15,fr3 */
L8c61c314: sa_motion_publish_stop(ctx,0x8c61c314u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c316; /* fmov.s fr3,@-r8 */
L8c61c316: sa_motion_publish_stop(ctx,0x8c61c316u); r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c318; /* mov.w @r10,r3 */
L8c61c318: sa_motion_publish_stop(ctx,0x8c61c318u); r[10]+=4u;goto L8c61c31a; /* add #0x4,r10 */
L8c61c31a: sa_motion_publish_stop(ctx,0x8c61c31au); *fpul=r[3];goto L8c61c31c; /* lds r3,FPUL */
L8c61c31c: sa_motion_publish_stop(ctx,0x8c61c31cu); r[3]=rd(m,bus,&io_status,0x8c61c4a4u,4);if(io_status)return io_status;goto L8c61c31e; /* mov.l 0x8c61c4a4,r3 */
L8c61c31e: sa_motion_publish_stop(ctx,0x8c61c31eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c320; /* float FPUL,fr3 */
L8c61c320: sa_motion_publish_stop(ctx,0x8c61c320u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c322; /* fmul fr15,fr3 */
L8c61c322: sa_motion_publish_stop(ctx,0x8c61c322u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c324; /* fmov.s fr3,@-r8 */
L8c61c324: sa_motion_publish_stop(ctx,0x8c61c324u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c326; /* fmov.s @(r0,r4),fr3 */
L8c61c326: sa_motion_publish_stop(ctx,0x8c61c326u); r[0]=20u;goto L8c61c328; /* mov #0x14,r0 */
L8c61c328: sa_motion_publish_stop(ctx,0x8c61c328u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c32a; /* fmov.s fr3,@-r8 */
L8c61c32a: sa_motion_publish_stop(ctx,0x8c61c32au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c32c; /* fmov.s @(r0,r4),fr3 */
L8c61c32c: sa_motion_publish_stop(ctx,0x8c61c32cu); r[0]=16u;goto L8c61c32e; /* mov #0x10,r0 */
L8c61c32e: sa_motion_publish_stop(ctx,0x8c61c32eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c330; /* fmov.s fr3,@-r8 */
L8c61c330: sa_motion_publish_stop(ctx,0x8c61c330u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c332; /* fmov.s @(r0,r4),fr3 */
L8c61c332: sa_motion_publish_stop(ctx,0x8c61c332u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c334; /* fmov.s fr3,@-r8 */
L8c61c334: sa_motion_publish_stop(ctx,0x8c61c334u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61c336; /* mov.l r3,@-r8 */
L8c61c336: sa_motion_publish_stop(ctx,0x8c61c336u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c338; /* pref @r8 */
L8c61c338: sa_motion_publish_stop(ctx,0x8c61c338u); r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c33a; /* mov.w @r9+,r4 */
L8c61c33a: sa_motion_publish_stop(ctx,0x8c61c33au); r[8]+=56u;goto L8c61c33c; /* add #0x38,r8 */
L8c61c33c: sa_motion_publish_stop(ctx,0x8c61c33cu); r[2]=rd(m,bus,&io_status,0x8c61c48cu,4);if(io_status)return io_status;goto L8c61c33e; /* mov.l 0x8c61c48c,r2 */
L8c61c33e: sa_motion_publish_stop(ctx,0x8c61c33eu); r[4]=r[4]&65535u;goto L8c61c340; /* extu.w r4,r4 */
L8c61c340: sa_motion_publish_stop(ctx,0x8c61c340u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c342; /* mov.w @(0x2,r10),r0 */
L8c61c342: sa_motion_publish_stop(ctx,0x8c61c342u); r[4]<<=2;goto L8c61c344; /* shll2 r4 */
L8c61c344: sa_motion_publish_stop(ctx,0x8c61c344u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61c346; /* mov.l @r2,r3 */
L8c61c346: sa_motion_publish_stop(ctx,0x8c61c346u); r[4]<<=2;goto L8c61c348; /* shll2 r4 */
L8c61c348: sa_motion_publish_stop(ctx,0x8c61c348u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61c34a; /* shll r4 */
L8c61c34a: sa_motion_publish_stop(ctx,0x8c61c34au); r[4]+=r[3];goto L8c61c34c; /* add r3,r4 */
L8c61c34c: sa_motion_publish_stop(ctx,0x8c61c34cu); r[3]=r[0];goto L8c61c34e; /* mov r0,r3 */
L8c61c34e: sa_motion_publish_stop(ctx,0x8c61c34eu); *fpul=r[3];goto L8c61c350; /* lds r3,FPUL */
L8c61c350: sa_motion_publish_stop(ctx,0x8c61c350u); r[0]=24u;goto L8c61c352; /* mov #0x18,r0 */
L8c61c352: sa_motion_publish_stop(ctx,0x8c61c352u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c354; /* float FPUL,fr3 */
L8c61c354: sa_motion_publish_stop(ctx,0x8c61c354u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c356; /* fmul fr15,fr3 */
L8c61c356: sa_motion_publish_stop(ctx,0x8c61c356u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c358; /* fmov.s fr3,@-r8 */
L8c61c358: sa_motion_publish_stop(ctx,0x8c61c358u); r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c35a; /* mov.w @r10,r3 */
L8c61c35a: sa_motion_publish_stop(ctx,0x8c61c35au); r[10]+=4u;goto L8c61c35c; /* add #0x4,r10 */
L8c61c35c: sa_motion_publish_stop(ctx,0x8c61c35cu); *fpul=r[3];goto L8c61c35e; /* lds r3,FPUL */
L8c61c35e: sa_motion_publish_stop(ctx,0x8c61c35eu); r[3]=rd(m,bus,&io_status,0x8c61c4a4u,4);if(io_status)return io_status;goto L8c61c360; /* mov.l 0x8c61c4a4,r3 */
L8c61c360: sa_motion_publish_stop(ctx,0x8c61c360u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c362; /* float FPUL,fr3 */
L8c61c362: sa_motion_publish_stop(ctx,0x8c61c362u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c364; /* fmul fr15,fr3 */
L8c61c364: sa_motion_publish_stop(ctx,0x8c61c364u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c366; /* fmov.s fr3,@-r8 */
L8c61c366: sa_motion_publish_stop(ctx,0x8c61c366u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c368; /* fmov.s @(r0,r4),fr3 */
L8c61c368: sa_motion_publish_stop(ctx,0x8c61c368u); r[0]=20u;goto L8c61c36a; /* mov #0x14,r0 */
L8c61c36a: sa_motion_publish_stop(ctx,0x8c61c36au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c36c; /* fmov.s fr3,@-r8 */
L8c61c36c: sa_motion_publish_stop(ctx,0x8c61c36cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c36e; /* fmov.s @(r0,r4),fr3 */
L8c61c36e: sa_motion_publish_stop(ctx,0x8c61c36eu); r[0]=16u;goto L8c61c370; /* mov #0x10,r0 */
L8c61c370: sa_motion_publish_stop(ctx,0x8c61c370u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c372; /* fmov.s fr3,@-r8 */
L8c61c372: sa_motion_publish_stop(ctx,0x8c61c372u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c374; /* fmov.s @(r0,r4),fr3 */
L8c61c374: sa_motion_publish_stop(ctx,0x8c61c374u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c376; /* fmov.s fr3,@-r8 */
L8c61c376: sa_motion_publish_stop(ctx,0x8c61c376u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61c378; /* mov.l r3,@-r8 */
L8c61c378: sa_motion_publish_stop(ctx,0x8c61c378u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c37a; /* pref @r8 */
L8c61c37a: sa_motion_publish_stop(ctx,0x8c61c37au); r[6]=r[14];goto L8c61c37c; /* mov r14,r6 */
L8c61c37c: sa_motion_publish_stop(ctx,0x8c61c37cu); r[6]+=4294967295u;goto L8c61c37e; /* add #-0x1,r6 */
L8c61c37e: sa_motion_publish_stop(ctx,0x8c61c37eu); r[5]=r[11];goto L8c61c380; /* mov r11,r5 */
L8c61c380: sa_motion_publish_stop(ctx,0x8c61c380u); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61c382; /* cmp/ge r6,r5 */
L8c61c382: sa_motion_publish_stop(ctx,0x8c61c384u); take=(*sr&1u);r[8]+=32u;if(take)goto L8c61c3d2;goto L8c61c386; /* bt/s 0x8c61c3d2 */
L8c61c386: sa_motion_publish_stop(ctx,0x8c61c386u); r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c388; /* mov.w @r9+,r4 */
L8c61c388: sa_motion_publish_stop(ctx,0x8c61c388u); r[8]+=28u;goto L8c61c38a; /* add #0x1c,r8 */
L8c61c38a: sa_motion_publish_stop(ctx,0x8c61c38au); r[2]=rd(m,bus,&io_status,0x8c61c48cu,4);if(io_status)return io_status;goto L8c61c38c; /* mov.l 0x8c61c48c,r2 */
L8c61c38c: sa_motion_publish_stop(ctx,0x8c61c38cu); r[4]=r[4]&65535u;goto L8c61c38e; /* extu.w r4,r4 */
L8c61c38e: sa_motion_publish_stop(ctx,0x8c61c38eu); r[4]<<=2;goto L8c61c390; /* shll2 r4 */
L8c61c390: sa_motion_publish_stop(ctx,0x8c61c390u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61c392; /* mov.l @r2,r3 */
L8c61c392: sa_motion_publish_stop(ctx,0x8c61c392u); r[4]<<=2;goto L8c61c394; /* shll2 r4 */
L8c61c394: sa_motion_publish_stop(ctx,0x8c61c394u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61c396; /* fmov.s fr4,@-r8 */
L8c61c396: sa_motion_publish_stop(ctx,0x8c61c396u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c398; /* mov.w @(0x2,r10),r0 */
L8c61c398: sa_motion_publish_stop(ctx,0x8c61c398u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61c39a; /* shll r4 */
L8c61c39a: sa_motion_publish_stop(ctx,0x8c61c39au); r[4]+=r[3];goto L8c61c39c; /* add r3,r4 */
L8c61c39c: sa_motion_publish_stop(ctx,0x8c61c39cu); r[3]=r[0];goto L8c61c39e; /* mov r0,r3 */
L8c61c39e: sa_motion_publish_stop(ctx,0x8c61c39eu); *fpul=r[3];goto L8c61c3a0; /* lds r3,FPUL */
L8c61c3a0: sa_motion_publish_stop(ctx,0x8c61c3a0u); r[0]=24u;goto L8c61c3a2; /* mov #0x18,r0 */
L8c61c3a2: sa_motion_publish_stop(ctx,0x8c61c3a2u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c3a4; /* float FPUL,fr3 */
L8c61c3a4: sa_motion_publish_stop(ctx,0x8c61c3a4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c3a6; /* fmul fr15,fr3 */
L8c61c3a6: sa_motion_publish_stop(ctx,0x8c61c3a6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c3a8; /* fmov.s fr3,@-r8 */
L8c61c3a8: sa_motion_publish_stop(ctx,0x8c61c3a8u); r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c3aa; /* mov.w @r10,r3 */
L8c61c3aa: sa_motion_publish_stop(ctx,0x8c61c3aau); r[10]+=4u;goto L8c61c3ac; /* add #0x4,r10 */
L8c61c3ac: sa_motion_publish_stop(ctx,0x8c61c3acu); *fpul=r[3];goto L8c61c3ae; /* lds r3,FPUL */
L8c61c3ae: sa_motion_publish_stop(ctx,0x8c61c3aeu); r[3]=rd(m,bus,&io_status,0x8c61c4a4u,4);if(io_status)return io_status;goto L8c61c3b0; /* mov.l 0x8c61c4a4,r3 */
L8c61c3b0: sa_motion_publish_stop(ctx,0x8c61c3b0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c3b2; /* float FPUL,fr3 */
L8c61c3b2: sa_motion_publish_stop(ctx,0x8c61c3b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c3b4; /* fmul fr15,fr3 */
L8c61c3b4: sa_motion_publish_stop(ctx,0x8c61c3b4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c3b6; /* fmov.s fr3,@-r8 */
L8c61c3b6: sa_motion_publish_stop(ctx,0x8c61c3b6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c3b8; /* fmov.s @(r0,r4),fr3 */
L8c61c3b8: sa_motion_publish_stop(ctx,0x8c61c3b8u); r[0]=20u;goto L8c61c3ba; /* mov #0x14,r0 */
L8c61c3ba: sa_motion_publish_stop(ctx,0x8c61c3bau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c3bc; /* fmov.s fr3,@-r8 */
L8c61c3bc: sa_motion_publish_stop(ctx,0x8c61c3bcu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c3be; /* fmov.s @(r0,r4),fr3 */
L8c61c3be: sa_motion_publish_stop(ctx,0x8c61c3beu); r[0]=16u;goto L8c61c3c0; /* mov #0x10,r0 */
L8c61c3c0: sa_motion_publish_stop(ctx,0x8c61c3c0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c3c2; /* fmov.s fr3,@-r8 */
L8c61c3c2: sa_motion_publish_stop(ctx,0x8c61c3c2u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c3c4; /* fmov.s @(r0,r4),fr3 */
L8c61c3c4: sa_motion_publish_stop(ctx,0x8c61c3c4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c3c6; /* fmov.s fr3,@-r8 */
L8c61c3c6: sa_motion_publish_stop(ctx,0x8c61c3c6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61c3c8; /* mov.l r3,@-r8 */
L8c61c3c8: sa_motion_publish_stop(ctx,0x8c61c3c8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c3ca; /* pref @r8 */
L8c61c3ca: sa_motion_publish_stop(ctx,0x8c61c3cau); r[5]+=1u;goto L8c61c3cc; /* add #0x1,r5 */
L8c61c3cc: sa_motion_publish_stop(ctx,0x8c61c3ccu); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61c3ce; /* cmp/ge r6,r5 */
L8c61c3ce: sa_motion_publish_stop(ctx,0x8c61c3d0u); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61c386;goto L8c61c3d2; /* bf/s 0x8c61c386 */
L8c61c3d2: sa_motion_publish_stop(ctx,0x8c61c3d2u); r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c3d4; /* mov.w @r9+,r4 */
L8c61c3d4: sa_motion_publish_stop(ctx,0x8c61c3d4u); r[8]+=28u;goto L8c61c3d6; /* add #0x1c,r8 */
L8c61c3d6: sa_motion_publish_stop(ctx,0x8c61c3d6u); r[2]=rd(m,bus,&io_status,0x8c61c48cu,4);if(io_status)return io_status;goto L8c61c3d8; /* mov.l 0x8c61c48c,r2 */
L8c61c3d8: sa_motion_publish_stop(ctx,0x8c61c3d8u); r[4]=r[4]&65535u;goto L8c61c3da; /* extu.w r4,r4 */
L8c61c3da: sa_motion_publish_stop(ctx,0x8c61c3dau); r[4]<<=2;goto L8c61c3dc; /* shll2 r4 */
L8c61c3dc: sa_motion_publish_stop(ctx,0x8c61c3dcu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61c3de; /* mov.l @r2,r3 */
L8c61c3de: sa_motion_publish_stop(ctx,0x8c61c3deu); r[4]<<=2;goto L8c61c3e0; /* shll2 r4 */
L8c61c3e0: sa_motion_publish_stop(ctx,0x8c61c3e0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61c3e2; /* fmov.s fr4,@-r8 */
L8c61c3e2: sa_motion_publish_stop(ctx,0x8c61c3e2u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c3e4; /* mov.w @(0x2,r10),r0 */
L8c61c3e4: sa_motion_publish_stop(ctx,0x8c61c3e4u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61c3e6; /* shll r4 */
L8c61c3e6: sa_motion_publish_stop(ctx,0x8c61c3e6u); r[4]+=r[3];goto L8c61c3e8; /* add r3,r4 */
L8c61c3e8: sa_motion_publish_stop(ctx,0x8c61c3e8u); r[3]=r[0];goto L8c61c3ea; /* mov r0,r3 */
L8c61c3ea: sa_motion_publish_stop(ctx,0x8c61c3eau); *fpul=r[3];goto L8c61c3ec; /* lds r3,FPUL */
L8c61c3ec: sa_motion_publish_stop(ctx,0x8c61c3ecu); r[0]=24u;goto L8c61c3ee; /* mov #0x18,r0 */
L8c61c3ee: sa_motion_publish_stop(ctx,0x8c61c3eeu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c3f0; /* float FPUL,fr3 */
L8c61c3f0: sa_motion_publish_stop(ctx,0x8c61c3f0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c3f2; /* fmul fr15,fr3 */
L8c61c3f2: sa_motion_publish_stop(ctx,0x8c61c3f2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c3f4; /* fmov.s fr3,@-r8 */
L8c61c3f4: sa_motion_publish_stop(ctx,0x8c61c3f4u); r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c3f6; /* mov.w @r10,r3 */
L8c61c3f6: sa_motion_publish_stop(ctx,0x8c61c3f6u); r[10]+=4u;goto L8c61c3f8; /* add #0x4,r10 */
L8c61c3f8: sa_motion_publish_stop(ctx,0x8c61c3f8u); *fpul=r[3];goto L8c61c3fa; /* lds r3,FPUL */
L8c61c3fa: sa_motion_publish_stop(ctx,0x8c61c3fau); r[3]=rd(m,bus,&io_status,0x8c61c4a8u,4);if(io_status)return io_status;goto L8c61c3fc; /* mov.l 0x8c61c4a8,r3 */
L8c61c3fc: sa_motion_publish_stop(ctx,0x8c61c3fcu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c3fe; /* float FPUL,fr3 */
L8c61c3fe: sa_motion_publish_stop(ctx,0x8c61c3feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c400; /* fmul fr15,fr3 */
L8c61c400: sa_motion_publish_stop(ctx,0x8c61c400u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c402; /* fmov.s fr3,@-r8 */
L8c61c402: sa_motion_publish_stop(ctx,0x8c61c402u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c404; /* fmov.s @(r0,r4),fr3 */
L8c61c404: sa_motion_publish_stop(ctx,0x8c61c404u); r[0]=20u;goto L8c61c406; /* mov #0x14,r0 */
L8c61c406: sa_motion_publish_stop(ctx,0x8c61c406u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c408; /* fmov.s fr3,@-r8 */
L8c61c408: sa_motion_publish_stop(ctx,0x8c61c408u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c40a; /* fmov.s @(r0,r4),fr3 */
L8c61c40a: sa_motion_publish_stop(ctx,0x8c61c40au); r[0]=16u;goto L8c61c40c; /* mov #0x10,r0 */
L8c61c40c: sa_motion_publish_stop(ctx,0x8c61c40cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c40e; /* fmov.s fr3,@-r8 */
L8c61c40e: sa_motion_publish_stop(ctx,0x8c61c40eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c410; /* fmov.s @(r0,r4),fr3 */
L8c61c410: sa_motion_publish_stop(ctx,0x8c61c410u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c412; /* fmov.s fr3,@-r8 */
L8c61c412: sa_motion_publish_stop(ctx,0x8c61c412u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61c414; /* mov.l r3,@-r8 */
L8c61c414: sa_motion_publish_stop(ctx,0x8c61c414u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c416; /* pref @r8 */
L8c61c416: sa_motion_publish_stop(ctx,0x8c61c416u); r[8]+=32u;goto L8c61c418; /* add #0x20,r8 */
L8c61c418: sa_motion_publish_stop(ctx,0x8c61c418u); r[12]+=1u;goto L8c61c41a; /* add #0x1,r12 */
L8c61c41a: sa_motion_publish_stop(ctx,0x8c61c41au); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61c41c; /* mov.l @r15,r2 */
L8c61c41c: sa_motion_publish_stop(ctx,0x8c61c41cu); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61c41e; /* mov.w @(0x2,r2),r0 */
L8c61c41e: sa_motion_publish_stop(ctx,0x8c61c41eu); r[0]=r[0]&65535u;goto L8c61c420; /* extu.w r0,r0 */
L8c61c420: sa_motion_publish_stop(ctx,0x8c61c420u); *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[0]);goto L8c61c422; /* cmp/ge r0,r12 */
L8c61c422: sa_motion_publish_stop(ctx,0x8c61c422u); if((*sr&1u))goto L8c61c428;goto L8c61c424; /* bt 0x8c61c428 */
L8c61c424: sa_motion_publish_stop(ctx,0x8c61c426u); goto L8c61c224; /* bra 0x8c61c224 */
L8c61c428: sa_motion_publish_stop(ctx,0x8c61c42au); goto L8c61c46a; /* bra 0x8c61c46a */
L8c61c42c: sa_motion_publish_stop(ctx,0x8c61c42cu); r[2]=rd(m,bus,&io_status,0x8c61c4acu,4);if(io_status)return io_status;goto L8c61c42e; /* mov.l 0x8c61c4ac,r2 */
L8c61c42e: sa_motion_publish_stop(ctx,0x8c61c42eu); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61c430; /* mov.l @r15,r1 */
L8c61c430: sa_motion_publish_stop(ctx,0x8c61c430u); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61c432; /* mov.l r1,@r2 */
L8c61c432: sa_motion_publish_stop(ctx,0x8c61c432u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61c434; /* frchg */
L8c61c434: sa_motion_publish_stop(ctx,0x8c61c434u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61c436; /* fmov.s fr0,@-r15 */
L8c61c436: sa_motion_publish_stop(ctx,0x8c61c436u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61c438; /* fmov.s fr1,@-r15 */
L8c61c438: sa_motion_publish_stop(ctx,0x8c61c438u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61c43a; /* fmov.s fr2,@-r15 */
L8c61c43a: sa_motion_publish_stop(ctx,0x8c61c43au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61c43c; /* fmov.s fr3,@-r15 */
L8c61c43c: sa_motion_publish_stop(ctx,0x8c61c43cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61c43e; /* fmov.s fr4,@-r15 */
L8c61c43e: sa_motion_publish_stop(ctx,0x8c61c43eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61c440; /* fmov.s fr5,@-r15 */
L8c61c440: sa_motion_publish_stop(ctx,0x8c61c440u); r[0]=rd(m,bus,&io_status,0x8c61c480u,4);if(io_status)return io_status;goto L8c61c442; /* mov.l 0x8c61c480,r0 */
L8c61c442: sa_motion_publish_stop(ctx,0x8c61c442u); fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c444; /* fmov.s @r0+,fr0 */
L8c61c444: sa_motion_publish_stop(ctx,0x8c61c444u); fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c446; /* fmov.s @r0+,fr1 */
L8c61c446: sa_motion_publish_stop(ctx,0x8c61c446u); fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c61c448; /* fmov.s @r0,fr2 */
L8c61c448: sa_motion_publish_stop(ctx,0x8c61c448u); r[0]=rd(m,bus,&io_status,0x8c61c484u,4);if(io_status)return io_status;goto L8c61c44a; /* mov.l 0x8c61c484,r0 */
L8c61c44a: sa_motion_publish_stop(ctx,0x8c61c44au); fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c61c44c; /* fmov.s @r0,fr3 */
L8c61c44c: sa_motion_publish_stop(ctx,0x8c61c44cu); r[0]=0x8c61c488u;goto L8c61c44e; /* mova 0x8c61c488,r0 */
L8c61c44e: sa_motion_publish_stop(ctx,0x8c61c44eu); fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c61c450; /* fmov.s @r0,fr4 */
L8c61c450: sa_motion_publish_stop(ctx,0x8c61c450u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61c452; /* frchg */
L8c61c452: sa_motion_publish_stop(ctx,0x8c61c452u); r[2]=rd(m,bus,&io_status,0x8c61c4b0u,4);if(io_status)return io_status;goto L8c61c454; /* mov.l 0x8c61c4b0,r2 */
L8c61c454: sa_motion_publish_stop(ctx,0x8c61c454u); r[3]=rd(m,bus,&io_status,0x8c61c4acu,4);if(io_status)return io_status;goto L8c61c456; /* mov.l 0x8c61c4ac,r3 */
L8c61c456: sa_motion_publish_stop(ctx,0x8c61c458u); target=r[2];*pr=0x8c61c45au;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61c45a; /* jsr @r2 */
L8c61c45a: sa_motion_publish_stop(ctx,0x8c61c45au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61c45c; /* frchg */
L8c61c45c: sa_motion_publish_stop(ctx,0x8c61c45cu); fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c45e; /* fmov.s @r15+,fr5 */
L8c61c45e: sa_motion_publish_stop(ctx,0x8c61c45eu); fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c460; /* fmov.s @r15+,fr4 */
L8c61c460: sa_motion_publish_stop(ctx,0x8c61c460u); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c462; /* fmov.s @r15+,fr3 */
L8c61c462: sa_motion_publish_stop(ctx,0x8c61c462u); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c464; /* fmov.s @r15+,fr2 */
L8c61c464: sa_motion_publish_stop(ctx,0x8c61c464u); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c466; /* fmov.s @r15+,fr1 */
L8c61c466: sa_motion_publish_stop(ctx,0x8c61c466u); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c468; /* fmov.s @r15+,fr0 */
L8c61c468: sa_motion_publish_stop(ctx,0x8c61c468u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61c46a; /* frchg */
L8c61c46a: sa_motion_publish_stop(ctx,0x8c61c46au); r[15]+=20u;goto L8c61c46c; /* add #0x14,r15 */
L8c61c46c: sa_motion_publish_stop(ctx,0x8c61c46cu); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61c46e; /* lds.l @r15+,PR */
L8c61c46e: sa_motion_publish_stop(ctx,0x8c61c46eu); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c470; /* fmov.s @r15+,fr15 */
L8c61c470: sa_motion_publish_stop(ctx,0x8c61c470u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61c472; /* mov.l @r15+,r9 */
L8c61c472: sa_motion_publish_stop(ctx,0x8c61c472u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61c474; /* mov.l @r15+,r10 */
L8c61c474: sa_motion_publish_stop(ctx,0x8c61c474u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61c476; /* mov.l @r15+,r11 */
L8c61c476: sa_motion_publish_stop(ctx,0x8c61c476u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61c478; /* mov.l @r15+,r12 */
L8c61c478: sa_motion_publish_stop(ctx,0x8c61c478u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61c47a; /* mov.l @r15+,r13 */
L8c61c47a: sa_motion_publish_stop(ctx,0x8c61c47cu); if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_uv_face_primitive_clipped_8c61c4b4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 uint32_t entry_pr=*pr;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61c4b4;
L8c61c4b4: sa_motion_publish_stop(ctx,0x8c61c4b4u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61c4b6; /* mov.l r14,@-r15 */
L8c61c4b6: sa_motion_publish_stop(ctx,0x8c61c4b6u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61c4b8; /* mov.l r13,@-r15 */
L8c61c4b8: sa_motion_publish_stop(ctx,0x8c61c4b8u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61c4ba; /* mov.l r12,@-r15 */
L8c61c4ba: sa_motion_publish_stop(ctx,0x8c61c4bau); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61c4bc; /* mov.l r11,@-r15 */
L8c61c4bc: sa_motion_publish_stop(ctx,0x8c61c4bcu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61c4be; /* mov.l r10,@-r15 */
L8c61c4be: sa_motion_publish_stop(ctx,0x8c61c4beu); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61c4c0; /* mov.l r9,@-r15 */
L8c61c4c0: sa_motion_publish_stop(ctx,0x8c61c4c0u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61c4c2; /* fmov.s fr15,@-r15 */
L8c61c4c2: sa_motion_publish_stop(ctx,0x8c61c4c2u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61c4c4; /* fmov.s fr14,@-r15 */
L8c61c4c4: sa_motion_publish_stop(ctx,0x8c61c4c4u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61c4c6; /* sts.l PR,@-r15 */
L8c61c4c6: sa_motion_publish_stop(ctx,0x8c61c4c6u); r[15]+=4294967256u;goto L8c61c4c8; /* add #-0x28,r15 */
L8c61c4c8: sa_motion_publish_stop(ctx,0x8c61c4c8u); r[0]=0x8c61c5a4u;goto L8c61c4ca; /* mova 0x8c61c5a4,r0 */
L8c61c4ca: sa_motion_publish_stop(ctx,0x8c61c4cau); r[3]=rd(m,bus,&io_status,0x8c61c5a8u,4);if(io_status)return io_status;goto L8c61c4cc; /* mov.l 0x8c61c5a8,r3 */
L8c61c4cc: sa_motion_publish_stop(ctx,0x8c61c4ccu); fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c61c4ce; /* fmov.s @r0,fr14 */
L8c61c4ce: sa_motion_publish_stop(ctx,0x8c61c4ceu); wr(m,bus,&io_status,r[15]+0xcu,r[4],4);if(io_status)return io_status;goto L8c61c4d0; /* mov.l r4,@(0xc,r15) */
L8c61c4d0: sa_motion_publish_stop(ctx,0x8c61c4d0u); r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61c4d2; /* mov.w @r4,r0 */
L8c61c4d2: sa_motion_publish_stop(ctx,0x8c61c4d2u); r[10]=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;goto L8c61c4d4; /* mov.l @(0x14,r4),r10 */
L8c61c4d4: sa_motion_publish_stop(ctx,0x8c61c4d4u); r[0]=r[0]&65535u;goto L8c61c4d6; /* extu.w r0,r0 */
L8c61c4d6: sa_motion_publish_stop(ctx,0x8c61c4d6u); r[0]&=r[3];goto L8c61c4d8; /* and r3,r0 */
L8c61c4d8: sa_motion_publish_stop(ctx,0x8c61c4d8u); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61c4da; /* cmp/eq #0x0,r0 */
L8c61c4da: sa_motion_publish_stop(ctx,0x8c61c4dcu); take=(*sr&1u);r[9]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;if(take)goto L8c61c500;goto L8c61c4de; /* bt/s 0x8c61c500 */
L8c61c4de: sa_motion_publish_stop(ctx,0x8c61c4deu); r[1]=sx(m,bus,&io_status,0x8c61c5a0u,2);if(io_status)return io_status;goto L8c61c4e0; /* mov.w 0x8c61c5a0,r1 */
L8c61c4e0: sa_motion_publish_stop(ctx,0x8c61c4e0u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61c4e2; /* cmp/eq r1,r0 */
L8c61c4e2: sa_motion_publish_stop(ctx,0x8c61c4e2u); if(!(*sr&1u))goto L8c61c4e8;goto L8c61c4e4; /* bf 0x8c61c4e8 */
L8c61c4e4: sa_motion_publish_stop(ctx,0x8c61c4e6u); goto L8c61c7a8; /* bra 0x8c61c7a8 */
L8c61c4e8: sa_motion_publish_stop(ctx,0x8c61c4e8u); r[1]=rd(m,bus,&io_status,0x8c61c5acu,4);if(io_status)return io_status;goto L8c61c4ea; /* mov.l 0x8c61c5ac,r1 */
L8c61c4ea: sa_motion_publish_stop(ctx,0x8c61c4eau); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61c4ec; /* cmp/eq r1,r0 */
L8c61c4ec: sa_motion_publish_stop(ctx,0x8c61c4ecu); if(!(*sr&1u))goto L8c61c4f2;goto L8c61c4ee; /* bf 0x8c61c4f2 */
L8c61c4ee: sa_motion_publish_stop(ctx,0x8c61c4f0u); goto L8c61cb0e; /* bra 0x8c61cb0e */
L8c61c4f2: sa_motion_publish_stop(ctx,0x8c61c4f2u); r[1]=rd(m,bus,&io_status,0x8c61c5a8u,4);if(io_status)return io_status;goto L8c61c4f4; /* mov.l 0x8c61c5a8,r1 */
L8c61c4f4: sa_motion_publish_stop(ctx,0x8c61c4f4u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61c4f6; /* cmp/eq r1,r0 */
L8c61c4f6: sa_motion_publish_stop(ctx,0x8c61c4f6u); if(!(*sr&1u))goto L8c61c4fc;goto L8c61c4f8; /* bf 0x8c61c4fc */
L8c61c4f8: sa_motion_publish_stop(ctx,0x8c61c4fau); goto L8c61cf0e; /* bra 0x8c61cf0e */
L8c61c4fc: sa_motion_publish_stop(ctx,0x8c61c4feu); goto L8c61cf4c; /* bra 0x8c61cf4c */
L8c61c500: sa_motion_publish_stop(ctx,0x8c61c500u); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61c502; /* mov.l @(0xc,r15),r2 */
L8c61c502: sa_motion_publish_stop(ctx,0x8c61c502u); r[11]=0u;goto L8c61c504; /* mov #0x0,r11 */
L8c61c504: sa_motion_publish_stop(ctx,0x8c61c504u); r[1]=rd(m,bus,&io_status,0x8c61c5b0u,4);if(io_status)return io_status;goto L8c61c506; /* mov.l 0x8c61c5b0,r1 */
L8c61c506: sa_motion_publish_stop(ctx,0x8c61c506u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61c508; /* mov.w @(0x2,r2),r0 */
L8c61c508: sa_motion_publish_stop(ctx,0x8c61c508u); r[3]=rd(m,bus,&io_status,0x8c61c5b8u,4);if(io_status)return io_status;goto L8c61c50a; /* mov.l 0x8c61c5b8,r3 */
L8c61c50a: sa_motion_publish_stop(ctx,0x8c61c50au); r[0]=r[0]&65535u;goto L8c61c50c; /* extu.w r0,r0 */
L8c61c50c: sa_motion_publish_stop(ctx,0x8c61c50cu); r[2]=r[0];goto L8c61c50e; /* mov r0,r2 */
L8c61c50e: sa_motion_publish_stop(ctx,0x8c61c50eu); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61c510; /* shll r0 */
L8c61c510: sa_motion_publish_stop(ctx,0x8c61c510u); r[0]+=r[2];goto L8c61c512; /* add r2,r0 */
L8c61c512: sa_motion_publish_stop(ctx,0x8c61c512u); r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61c514; /* mov.l @r1,r2 */
L8c61c514: sa_motion_publish_stop(ctx,0x8c61c514u); r[2]+=r[0];goto L8c61c516; /* add r0,r2 */
L8c61c516: sa_motion_publish_stop(ctx,0x8c61c516u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61c518; /* mov.l r2,@r1 */
L8c61c518: sa_motion_publish_stop(ctx,0x8c61c518u); r[1]=rd(m,bus,&io_status,0x8c61c5b4u,4);if(io_status)return io_status;goto L8c61c51a; /* mov.l 0x8c61c5b4,r1 */
L8c61c51a: sa_motion_publish_stop(ctx,0x8c61c51au); wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c61c51c; /* mov.l r1,@r15 */
L8c61c51c: sa_motion_publish_stop(ctx,0x8c61c51cu); r[1]+=8u;goto L8c61c51e; /* add #0x8,r1 */
L8c61c51e: sa_motion_publish_stop(ctx,0x8c61c51eu); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61c520; /* mov.l r3,@(0x4,r15) */
L8c61c520: sa_motion_publish_stop(ctx,0x8c61c522u); wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c61c796; /* bra 0x8c61c796 */
L8c61c524: sa_motion_publish_stop(ctx,0x8c61c524u); r[14]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c526; /* mov.w @r9+,r14 */
L8c61c526: sa_motion_publish_stop(ctx,0x8c61c526u); r[2]=rd(m,bus,&io_status,0x8c61c5bcu,4);if(io_status)return io_status;goto L8c61c528; /* mov.l 0x8c61c5bc,r2 */
L8c61c528: sa_motion_publish_stop(ctx,0x8c61c528u); r[12]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c52a; /* mov.w @r9+,r12 */
L8c61c52a: sa_motion_publish_stop(ctx,0x8c61c52au); r[14]=r[14]&65535u;goto L8c61c52c; /* extu.w r14,r14 */
L8c61c52c: sa_motion_publish_stop(ctx,0x8c61c52cu); r[14]<<=2;goto L8c61c52e; /* shll2 r14 */
L8c61c52e: sa_motion_publish_stop(ctx,0x8c61c52eu); r[5]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61c530; /* mov.l @r2,r5 */
L8c61c530: sa_motion_publish_stop(ctx,0x8c61c530u); r[13]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c532; /* mov.w @r9+,r13 */
L8c61c532: sa_motion_publish_stop(ctx,0x8c61c532u); r[12]=r[12]&65535u;goto L8c61c534; /* extu.w r12,r12 */
L8c61c534: sa_motion_publish_stop(ctx,0x8c61c534u); r[12]<<=2;goto L8c61c536; /* shll2 r12 */
L8c61c536: sa_motion_publish_stop(ctx,0x8c61c536u); r[13]=r[13]&65535u;goto L8c61c538; /* extu.w r13,r13 */
L8c61c538: sa_motion_publish_stop(ctx,0x8c61c538u); r[12]<<=2;goto L8c61c53a; /* shll2 r12 */
L8c61c53a: sa_motion_publish_stop(ctx,0x8c61c53au); r[13]<<=2;goto L8c61c53c; /* shll2 r13 */
L8c61c53c: sa_motion_publish_stop(ctx,0x8c61c53cu); r[14]<<=2;goto L8c61c53e; /* shll2 r14 */
L8c61c53e: sa_motion_publish_stop(ctx,0x8c61c53eu); r[13]<<=2;goto L8c61c540; /* shll2 r13 */
L8c61c540: sa_motion_publish_stop(ctx,0x8c61c540u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61c542; /* shll r14 */
L8c61c542: sa_motion_publish_stop(ctx,0x8c61c542u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61c544; /* shll r12 */
L8c61c544: sa_motion_publish_stop(ctx,0x8c61c544u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61c546; /* shll r13 */
L8c61c546: sa_motion_publish_stop(ctx,0x8c61c546u); r[14]+=r[5];goto L8c61c548; /* add r5,r14 */
L8c61c548: sa_motion_publish_stop(ctx,0x8c61c548u); r[12]+=r[5];goto L8c61c54a; /* add r5,r12 */
L8c61c54a: sa_motion_publish_stop(ctx,0x8c61c54au); r[4]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61c54c; /* mov.l @(0xc,r14),r4 */
L8c61c54c: sa_motion_publish_stop(ctx,0x8c61c54cu); r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61c54e; /* mov.l @(0xc,r12),r3 */
L8c61c54e: sa_motion_publish_stop(ctx,0x8c61c54eu); r[13]+=r[5];goto L8c61c550; /* add r5,r13 */
L8c61c550: sa_motion_publish_stop(ctx,0x8c61c550u); r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61c552; /* mov.l @(0xc,r13),r1 */
L8c61c552: sa_motion_publish_stop(ctx,0x8c61c552u); r[4]&=r[3];goto L8c61c554; /* and r3,r4 */
L8c61c554: sa_motion_publish_stop(ctx,0x8c61c554u); r[3]=63u;goto L8c61c556; /* mov #0x3f,r3 */
L8c61c556: sa_motion_publish_stop(ctx,0x8c61c556u); r[4]&=r[1];goto L8c61c558; /* and r1,r4 */
L8c61c558: sa_motion_publish_stop(ctx,0x8c61c558u); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61c55a; /* tst r3,r4 */
L8c61c55a: sa_motion_publish_stop(ctx,0x8c61c55au); if(!(*sr&1u))goto L8c61c59c;goto L8c61c55c; /* bf 0x8c61c59c */
L8c61c55c: sa_motion_publish_stop(ctx,0x8c61c55cu); r[4]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61c55e; /* mov.l @(0xc,r14),r4 */
L8c61c55e: sa_motion_publish_stop(ctx,0x8c61c55eu); r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61c560; /* mov.l @(0xc,r12),r3 */
L8c61c560: sa_motion_publish_stop(ctx,0x8c61c560u); r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61c562; /* mov.l @(0xc,r13),r2 */
L8c61c562: sa_motion_publish_stop(ctx,0x8c61c562u); r[4]|=r[3];goto L8c61c564; /* or r3,r4 */
L8c61c564: sa_motion_publish_stop(ctx,0x8c61c564u); r[3]=1u;goto L8c61c566; /* mov #0x1,r3 */
L8c61c566: sa_motion_publish_stop(ctx,0x8c61c566u); r[4]|=r[2];goto L8c61c568; /* or r2,r4 */
L8c61c568: sa_motion_publish_stop(ctx,0x8c61c568u); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61c56a; /* tst r3,r4 */
L8c61c56a: sa_motion_publish_stop(ctx,0x8c61c56au); if((*sr&1u))goto L8c61c570;goto L8c61c56c; /* bt 0x8c61c570 */
L8c61c56c: sa_motion_publish_stop(ctx,0x8c61c56eu); goto L8c61c6c4; /* bra 0x8c61c6c4 */
L8c61c570: sa_motion_publish_stop(ctx,0x8c61c570u); r[0]=16u;goto L8c61c572; /* mov #0x10,r0 */
L8c61c572: sa_motion_publish_stop(ctx,0x8c61c572u); fload(m,bus,&io_status,f,4u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c574; /* fmov.s @(r0,r12),fr4 */
L8c61c574: sa_motion_publish_stop(ctx,0x8c61c574u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c576; /* fmov.s @(r0,r14),fr3 */
L8c61c576: sa_motion_publish_stop(ctx,0x8c61c576u); r[0]=20u;goto L8c61c578; /* mov #0x14,r0 */
L8c61c578: sa_motion_publish_stop(ctx,0x8c61c578u); fcopy(f,2u,4u);goto L8c61c57a; /* fmov fr4,fr2 */
L8c61c57a: sa_motion_publish_stop(ctx,0x8c61c57au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61c57c; /* fsub fr3,fr2 */
L8c61c57c: sa_motion_publish_stop(ctx,0x8c61c57cu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c57e; /* fmov.s @(r0,r13),fr3 */
L8c61c57e: sa_motion_publish_stop(ctx,0x8c61c57eu); fload(m,bus,&io_status,f,5u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c580; /* fmov.s @(r0,r12),fr5 */
L8c61c580: sa_motion_publish_stop(ctx,0x8c61c580u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c582; /* fsub fr5,fr3 */
L8c61c582: sa_motion_publish_stop(ctx,0x8c61c582u); fcopy(f,1u,5u);goto L8c61c584; /* fmov fr5,fr1 */
L8c61c584: sa_motion_publish_stop(ctx,0x8c61c584u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c586; /* fmul fr3,fr2 */
L8c61c586: sa_motion_publish_stop(ctx,0x8c61c586u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c588; /* fmov.s @(r0,r14),fr3 */
L8c61c588: sa_motion_publish_stop(ctx,0x8c61c588u); r[0]=16u;goto L8c61c58a; /* mov #0x10,r0 */
L8c61c58a: sa_motion_publish_stop(ctx,0x8c61c58au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61c58c; /* fsub fr3,fr1 */
L8c61c58c: sa_motion_publish_stop(ctx,0x8c61c58cu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c58e; /* fmov.s @(r0,r13),fr3 */
L8c61c58e: sa_motion_publish_stop(ctx,0x8c61c58eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c590; /* fsub fr4,fr3 */
L8c61c590: sa_motion_publish_stop(ctx,0x8c61c590u); fcopy(f,4u,2u);goto L8c61c592; /* fmov fr2,fr4 */
L8c61c592: sa_motion_publish_stop(ctx,0x8c61c592u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c594; /* fmul fr3,fr1 */
L8c61c594: sa_motion_publish_stop(ctx,0x8c61c594u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61c596; /* fldi0 fr3 */
L8c61c596: sa_motion_publish_stop(ctx,0x8c61c596u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c598; /* fsub fr1,fr4 */
L8c61c598: sa_motion_publish_stop(ctx,0x8c61c598u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61c59a; /* fcmp/gt fr3,fr4 */
L8c61c59a: sa_motion_publish_stop(ctx,0x8c61c59au); if(!(*sr&1u))goto L8c61c5c0;goto L8c61c59c; /* bf 0x8c61c5c0 */
L8c61c59c: sa_motion_publish_stop(ctx,0x8c61c59eu); r[10]+=12u;goto L8c61c794; /* bra 0x8c61c794 */
L8c61c5c0: sa_motion_publish_stop(ctx,0x8c61c5c0u); fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c61c5c2; /* fmov.s @r14,fr2 */
L8c61c5c2: sa_motion_publish_stop(ctx,0x8c61c5c2u); r[0]=28u;goto L8c61c5c4; /* mov #0x1c,r0 */
L8c61c5c4: sa_motion_publish_stop(ctx,0x8c61c5c4u); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61c5c6; /* fmov.s @r12,fr3 */
L8c61c5c6: sa_motion_publish_stop(ctx,0x8c61c5c6u); r[4]=r[15];goto L8c61c5c8; /* mov r15,r4 */
L8c61c5c8: sa_motion_publish_stop(ctx,0x8c61c5c8u); r[3]=rd(m,bus,&io_status,0x8c61c864u,4);if(io_status)return io_status;goto L8c61c5ca; /* mov.l 0x8c61c864,r3 */
L8c61c5ca: sa_motion_publish_stop(ctx,0x8c61c5cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c5cc; /* fadd fr3,fr2 */
L8c61c5cc: sa_motion_publish_stop(ctx,0x8c61c5ccu); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61c5ce; /* fmov.s @r13,fr3 */
L8c61c5ce: sa_motion_publish_stop(ctx,0x8c61c5ceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c5d0; /* fadd fr3,fr2 */
L8c61c5d0: sa_motion_publish_stop(ctx,0x8c61c5d0u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c5d2; /* fmov fr2,@(r0,r15) */
L8c61c5d2: sa_motion_publish_stop(ctx,0x8c61c5d2u); r[0]=4u;goto L8c61c5d4; /* mov #0x4,r0 */
L8c61c5d4: sa_motion_publish_stop(ctx,0x8c61c5d4u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c5d6; /* fmov.s @(r0,r14),fr2 */
L8c61c5d6: sa_motion_publish_stop(ctx,0x8c61c5d6u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c5d8; /* fmov.s @(r0,r12),fr3 */
L8c61c5d8: sa_motion_publish_stop(ctx,0x8c61c5d8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c5da; /* fadd fr3,fr2 */
L8c61c5da: sa_motion_publish_stop(ctx,0x8c61c5dau); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c5dc; /* fmov.s @(r0,r13),fr3 */
L8c61c5dc: sa_motion_publish_stop(ctx,0x8c61c5dcu); r[0]=32u;goto L8c61c5de; /* mov #0x20,r0 */
L8c61c5de: sa_motion_publish_stop(ctx,0x8c61c5deu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c5e0; /* fadd fr3,fr2 */
L8c61c5e0: sa_motion_publish_stop(ctx,0x8c61c5e0u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c5e2; /* fmov fr2,@(r0,r15) */
L8c61c5e2: sa_motion_publish_stop(ctx,0x8c61c5e2u); r[0]=8u;goto L8c61c5e4; /* mov #0x8,r0 */
L8c61c5e4: sa_motion_publish_stop(ctx,0x8c61c5e4u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c5e6; /* fmov.s @(r0,r14),fr2 */
L8c61c5e6: sa_motion_publish_stop(ctx,0x8c61c5e6u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c5e8; /* fmov.s @(r0,r12),fr3 */
L8c61c5e8: sa_motion_publish_stop(ctx,0x8c61c5e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c5ea; /* fadd fr3,fr2 */
L8c61c5ea: sa_motion_publish_stop(ctx,0x8c61c5eau); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c5ec; /* fmov.s @(r0,r13),fr3 */
L8c61c5ec: sa_motion_publish_stop(ctx,0x8c61c5ecu); r[0]=36u;goto L8c61c5ee; /* mov #0x24,r0 */
L8c61c5ee: sa_motion_publish_stop(ctx,0x8c61c5eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c5f0; /* fadd fr3,fr2 */
L8c61c5f0: sa_motion_publish_stop(ctx,0x8c61c5f0u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c5f2; /* fmov fr2,@(r0,r15) */
L8c61c5f2: sa_motion_publish_stop(ctx,0x8c61c5f4u); target=r[3];*pr=0x8c61c5f6u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61c5f6; /* jsr @r3 */
L8c61c5f6: sa_motion_publish_stop(ctx,0x8c61c5f6u); r[3]=rd(m,bus,&io_status,0x8c61c868u,4);if(io_status)return io_status;goto L8c61c5f8; /* mov.l 0x8c61c868,r3 */
L8c61c5f8: sa_motion_publish_stop(ctx,0x8c61c5f8u); r[0]=28u;goto L8c61c5fa; /* mov #0x1c,r0 */
L8c61c5fa: sa_motion_publish_stop(ctx,0x8c61c5fau); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c5fc; /* fmov.s @(r0,r15),fr3 */
L8c61c5fc: sa_motion_publish_stop(ctx,0x8c61c5fcu); r[0]=32u;goto L8c61c5fe; /* mov #0x20,r0 */
L8c61c5fe: sa_motion_publish_stop(ctx,0x8c61c5feu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61c600; /* fmov.s @r3,fr2 */
L8c61c600: sa_motion_publish_stop(ctx,0x8c61c600u); r[8]+=24u;goto L8c61c602; /* add #0x18,r8 */
L8c61c602: sa_motion_publish_stop(ctx,0x8c61c602u); r[2]=rd(m,bus,&io_status,0x8c61c86cu,4);if(io_status)return io_status;goto L8c61c604; /* mov.l 0x8c61c86c,r2 */
L8c61c604: sa_motion_publish_stop(ctx,0x8c61c604u); r[3]+=8u;goto L8c61c606; /* add #0x8,r3 */
L8c61c606: sa_motion_publish_stop(ctx,0x8c61c606u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c608; /* fmul fr3,fr2 */
L8c61c608: sa_motion_publish_stop(ctx,0x8c61c608u); r[1]=rd(m,bus,&io_status,0x8c61c870u,4);if(io_status)return io_status;goto L8c61c60a; /* mov.l 0x8c61c870,r1 */
L8c61c60a: sa_motion_publish_stop(ctx,0x8c61c60au); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61c60c; /* fmov.s @r2,fr3 */
L8c61c60c: sa_motion_publish_stop(ctx,0x8c61c60cu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61c60e; /* fmov.s @r1,fr1 */
L8c61c60e: sa_motion_publish_stop(ctx,0x8c61c60eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c610; /* fsub fr2,fr3 */
L8c61c610: sa_motion_publish_stop(ctx,0x8c61c610u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c612; /* fmov.s @(r0,r15),fr2 */
L8c61c612: sa_motion_publish_stop(ctx,0x8c61c612u); r[0]=36u;goto L8c61c614; /* mov #0x24,r0 */
L8c61c614: sa_motion_publish_stop(ctx,0x8c61c614u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c616; /* fmul fr2,fr1 */
L8c61c616: sa_motion_publish_stop(ctx,0x8c61c616u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c618; /* fmov.s @(r0,r15),fr2 */
L8c61c618: sa_motion_publish_stop(ctx,0x8c61c618u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c61a; /* mov.w @(0x2,r10),r0 */
L8c61c61a: sa_motion_publish_stop(ctx,0x8c61c61au); r[2]=r[0];goto L8c61c61c; /* mov r0,r2 */
L8c61c61c: sa_motion_publish_stop(ctx,0x8c61c61cu); *fpul=r[2];goto L8c61c61e; /* lds r2,FPUL */
L8c61c61e: sa_motion_publish_stop(ctx,0x8c61c61eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c620; /* fsub fr1,fr3 */
L8c61c620: sa_motion_publish_stop(ctx,0x8c61c620u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61c622; /* fmov.s @r3,fr1 */
L8c61c622: sa_motion_publish_stop(ctx,0x8c61c622u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c624; /* fmul fr2,fr1 */
L8c61c624: sa_motion_publish_stop(ctx,0x8c61c624u); fcopy(f,4u,3u);goto L8c61c626; /* fmov fr3,fr4 */
L8c61c626: sa_motion_publish_stop(ctx,0x8c61c626u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c628; /* float FPUL,fr3 */
L8c61c628: sa_motion_publish_stop(ctx,0x8c61c628u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c62a; /* fsub fr1,fr4 */
L8c61c62a: sa_motion_publish_stop(ctx,0x8c61c62au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c62c; /* fmul fr14,fr3 */
L8c61c62c: sa_motion_publish_stop(ctx,0x8c61c62cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c62e; /* fmov.s fr3,@-r8 */
L8c61c62e: sa_motion_publish_stop(ctx,0x8c61c62eu); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c630; /* mov.w @r10,r2 */
L8c61c630: sa_motion_publish_stop(ctx,0x8c61c630u); *fpul=r[2];goto L8c61c632; /* lds r2,FPUL */
L8c61c632: sa_motion_publish_stop(ctx,0x8c61c632u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c634; /* float FPUL,fr3 */
L8c61c634: sa_motion_publish_stop(ctx,0x8c61c634u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c636; /* fmul fr14,fr3 */
L8c61c636: sa_motion_publish_stop(ctx,0x8c61c636u); r[0]=24u;goto L8c61c638; /* mov #0x18,r0 */
L8c61c638: sa_motion_publish_stop(ctx,0x8c61c638u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c63a; /* fmov.s fr3,@-r8 */
L8c61c63a: sa_motion_publish_stop(ctx,0x8c61c63au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c63c; /* fmov.s @(r0,r14),fr3 */
L8c61c63c: sa_motion_publish_stop(ctx,0x8c61c63cu); r[0]=20u;goto L8c61c63e; /* mov #0x14,r0 */
L8c61c63e: sa_motion_publish_stop(ctx,0x8c61c63eu); r[2]=rd(m,bus,&io_status,0x8c61c874u,4);if(io_status)return io_status;goto L8c61c640; /* mov.l 0x8c61c874,r2 */
L8c61c640: sa_motion_publish_stop(ctx,0x8c61c640u); r[10]+=4u;goto L8c61c642; /* add #0x4,r10 */
L8c61c642: sa_motion_publish_stop(ctx,0x8c61c642u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c644; /* fmov.s fr3,@-r8 */
L8c61c644: sa_motion_publish_stop(ctx,0x8c61c644u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c646; /* fmov.s @(r0,r14),fr3 */
L8c61c646: sa_motion_publish_stop(ctx,0x8c61c646u); r[0]=16u;goto L8c61c648; /* mov #0x10,r0 */
L8c61c648: sa_motion_publish_stop(ctx,0x8c61c648u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c64a; /* fmov.s fr3,@-r8 */
L8c61c64a: sa_motion_publish_stop(ctx,0x8c61c64au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c64c; /* fmov.s @(r0,r14),fr3 */
L8c61c64c: sa_motion_publish_stop(ctx,0x8c61c64cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c64e; /* fmov.s fr3,@-r8 */
L8c61c64e: sa_motion_publish_stop(ctx,0x8c61c64eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c650; /* mov.l r2,@-r8 */
L8c61c650: sa_motion_publish_stop(ctx,0x8c61c650u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c652; /* pref @r8 */
L8c61c652: sa_motion_publish_stop(ctx,0x8c61c652u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c654; /* mov.w @(0x2,r10),r0 */
L8c61c654: sa_motion_publish_stop(ctx,0x8c61c654u); r[8]+=56u;goto L8c61c656; /* add #0x38,r8 */
L8c61c656: sa_motion_publish_stop(ctx,0x8c61c656u); r[2]=r[0];goto L8c61c658; /* mov r0,r2 */
L8c61c658: sa_motion_publish_stop(ctx,0x8c61c658u); *fpul=r[2];goto L8c61c65a; /* lds r2,FPUL */
L8c61c65a: sa_motion_publish_stop(ctx,0x8c61c65au); r[0]=24u;goto L8c61c65c; /* mov #0x18,r0 */
L8c61c65c: sa_motion_publish_stop(ctx,0x8c61c65cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c65e; /* float FPUL,fr3 */
L8c61c65e: sa_motion_publish_stop(ctx,0x8c61c65eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c660; /* fmul fr14,fr3 */
L8c61c660: sa_motion_publish_stop(ctx,0x8c61c660u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c662; /* fmov.s fr3,@-r8 */
L8c61c662: sa_motion_publish_stop(ctx,0x8c61c662u); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c664; /* mov.w @r10,r2 */
L8c61c664: sa_motion_publish_stop(ctx,0x8c61c664u); r[10]+=4u;goto L8c61c666; /* add #0x4,r10 */
L8c61c666: sa_motion_publish_stop(ctx,0x8c61c666u); *fpul=r[2];goto L8c61c668; /* lds r2,FPUL */
L8c61c668: sa_motion_publish_stop(ctx,0x8c61c668u); r[2]=rd(m,bus,&io_status,0x8c61c874u,4);if(io_status)return io_status;goto L8c61c66a; /* mov.l 0x8c61c874,r2 */
L8c61c66a: sa_motion_publish_stop(ctx,0x8c61c66au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c66c; /* float FPUL,fr3 */
L8c61c66c: sa_motion_publish_stop(ctx,0x8c61c66cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c66e; /* fmul fr14,fr3 */
L8c61c66e: sa_motion_publish_stop(ctx,0x8c61c66eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c670; /* fmov.s fr3,@-r8 */
L8c61c670: sa_motion_publish_stop(ctx,0x8c61c670u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c672; /* fmov.s @(r0,r12),fr3 */
L8c61c672: sa_motion_publish_stop(ctx,0x8c61c672u); r[0]=20u;goto L8c61c674; /* mov #0x14,r0 */
L8c61c674: sa_motion_publish_stop(ctx,0x8c61c674u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c676; /* fmov.s fr3,@-r8 */
L8c61c676: sa_motion_publish_stop(ctx,0x8c61c676u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c678; /* fmov.s @(r0,r12),fr3 */
L8c61c678: sa_motion_publish_stop(ctx,0x8c61c678u); r[0]=16u;goto L8c61c67a; /* mov #0x10,r0 */
L8c61c67a: sa_motion_publish_stop(ctx,0x8c61c67au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c67c; /* fmov.s fr3,@-r8 */
L8c61c67c: sa_motion_publish_stop(ctx,0x8c61c67cu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c67e; /* fmov.s @(r0,r12),fr3 */
L8c61c67e: sa_motion_publish_stop(ctx,0x8c61c67eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c680; /* fmov.s fr3,@-r8 */
L8c61c680: sa_motion_publish_stop(ctx,0x8c61c680u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c682; /* mov.l r2,@-r8 */
L8c61c682: sa_motion_publish_stop(ctx,0x8c61c682u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c684; /* pref @r8 */
L8c61c684: sa_motion_publish_stop(ctx,0x8c61c684u); r[8]+=60u;goto L8c61c686; /* add #0x3c,r8 */
L8c61c686: sa_motion_publish_stop(ctx,0x8c61c686u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61c688; /* fmov.s fr4,@-r8 */
L8c61c688: sa_motion_publish_stop(ctx,0x8c61c688u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c68a; /* mov.w @(0x2,r10),r0 */
L8c61c68a: sa_motion_publish_stop(ctx,0x8c61c68au); r[2]=r[0];goto L8c61c68c; /* mov r0,r2 */
L8c61c68c: sa_motion_publish_stop(ctx,0x8c61c68cu); *fpul=r[2];goto L8c61c68e; /* lds r2,FPUL */
L8c61c68e: sa_motion_publish_stop(ctx,0x8c61c68eu); r[0]=24u;goto L8c61c690; /* mov #0x18,r0 */
L8c61c690: sa_motion_publish_stop(ctx,0x8c61c690u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c692; /* float FPUL,fr3 */
L8c61c692: sa_motion_publish_stop(ctx,0x8c61c692u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c694; /* fmul fr14,fr3 */
L8c61c694: sa_motion_publish_stop(ctx,0x8c61c694u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c696; /* fmov.s fr3,@-r8 */
L8c61c696: sa_motion_publish_stop(ctx,0x8c61c696u); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c698; /* mov.w @r10,r2 */
L8c61c698: sa_motion_publish_stop(ctx,0x8c61c698u); r[10]+=4u;goto L8c61c69a; /* add #0x4,r10 */
L8c61c69a: sa_motion_publish_stop(ctx,0x8c61c69au); *fpul=r[2];goto L8c61c69c; /* lds r2,FPUL */
L8c61c69c: sa_motion_publish_stop(ctx,0x8c61c69cu); r[2]=rd(m,bus,&io_status,0x8c61c878u,4);if(io_status)return io_status;goto L8c61c69e; /* mov.l 0x8c61c878,r2 */
L8c61c69e: sa_motion_publish_stop(ctx,0x8c61c69eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c6a0; /* float FPUL,fr3 */
L8c61c6a0: sa_motion_publish_stop(ctx,0x8c61c6a0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c6a2; /* fmul fr14,fr3 */
L8c61c6a2: sa_motion_publish_stop(ctx,0x8c61c6a2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c6a4; /* fmov.s fr3,@-r8 */
L8c61c6a4: sa_motion_publish_stop(ctx,0x8c61c6a4u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c6a6; /* fmov.s @(r0,r13),fr3 */
L8c61c6a6: sa_motion_publish_stop(ctx,0x8c61c6a6u); r[0]=20u;goto L8c61c6a8; /* mov #0x14,r0 */
L8c61c6a8: sa_motion_publish_stop(ctx,0x8c61c6a8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c6aa; /* fmov.s fr3,@-r8 */
L8c61c6aa: sa_motion_publish_stop(ctx,0x8c61c6aau); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c6ac; /* fmov.s @(r0,r13),fr3 */
L8c61c6ac: sa_motion_publish_stop(ctx,0x8c61c6acu); r[0]=16u;goto L8c61c6ae; /* mov #0x10,r0 */
L8c61c6ae: sa_motion_publish_stop(ctx,0x8c61c6aeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c6b0; /* fmov.s fr3,@-r8 */
L8c61c6b0: sa_motion_publish_stop(ctx,0x8c61c6b0u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c6b2; /* fmov.s @(r0,r13),fr3 */
L8c61c6b2: sa_motion_publish_stop(ctx,0x8c61c6b2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c6b4; /* fmov.s fr3,@-r8 */
L8c61c6b4: sa_motion_publish_stop(ctx,0x8c61c6b4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c6b6; /* mov.l r2,@-r8 */
L8c61c6b6: sa_motion_publish_stop(ctx,0x8c61c6b6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c6b8; /* pref @r8 */
L8c61c6b8: sa_motion_publish_stop(ctx,0x8c61c6b8u); r[0]=rd(m,bus,&io_status,0x8c61c87cu,4);if(io_status)return io_status;goto L8c61c6ba; /* mov.l 0x8c61c87c,r0 */
L8c61c6ba: sa_motion_publish_stop(ctx,0x8c61c6bau); r[8]+=32u;goto L8c61c6bc; /* add #0x20,r8 */
L8c61c6bc: sa_motion_publish_stop(ctx,0x8c61c6bcu); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61c6be; /* mov.l @r0,r2 */
L8c61c6be: sa_motion_publish_stop(ctx,0x8c61c6beu); r[2]+=3u;goto L8c61c6c0; /* add #0x3,r2 */
L8c61c6c0: sa_motion_publish_stop(ctx,0x8c61c6c2u); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61c794; /* bra 0x8c61c794 */
L8c61c6c4: sa_motion_publish_stop(ctx,0x8c61c6c4u); r[0]=28u;goto L8c61c6c6; /* mov #0x1c,r0 */
L8c61c6c6: sa_motion_publish_stop(ctx,0x8c61c6c6u); r[1]=rd(m,bus,&io_status,0x8c61c880u,4);if(io_status)return io_status;goto L8c61c6c8; /* mov.l 0x8c61c880,r1 */
L8c61c6c8: sa_motion_publish_stop(ctx,0x8c61c6c8u); r[4]=r[15];goto L8c61c6ca; /* mov r15,r4 */
L8c61c6ca: sa_motion_publish_stop(ctx,0x8c61c6cau); wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c61c6cc; /* mov.l r3,@r1 */
L8c61c6cc: sa_motion_publish_stop(ctx,0x8c61c6ccu); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61c6ce; /* fmov.s @r12,fr3 */
L8c61c6ce: sa_motion_publish_stop(ctx,0x8c61c6ceu); fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c61c6d0; /* fmov.s @r14,fr2 */
L8c61c6d0: sa_motion_publish_stop(ctx,0x8c61c6d0u); r[3]=rd(m,bus,&io_status,0x8c61c864u,4);if(io_status)return io_status;goto L8c61c6d2; /* mov.l 0x8c61c864,r3 */
L8c61c6d2: sa_motion_publish_stop(ctx,0x8c61c6d2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c6d4; /* fadd fr3,fr2 */
L8c61c6d4: sa_motion_publish_stop(ctx,0x8c61c6d4u); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61c6d6; /* fmov.s @r13,fr3 */
L8c61c6d6: sa_motion_publish_stop(ctx,0x8c61c6d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c6d8; /* fadd fr3,fr2 */
L8c61c6d8: sa_motion_publish_stop(ctx,0x8c61c6d8u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c6da; /* fmov fr2,@(r0,r15) */
L8c61c6da: sa_motion_publish_stop(ctx,0x8c61c6dau); r[0]=4u;goto L8c61c6dc; /* mov #0x4,r0 */
L8c61c6dc: sa_motion_publish_stop(ctx,0x8c61c6dcu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c6de; /* fmov.s @(r0,r12),fr3 */
L8c61c6de: sa_motion_publish_stop(ctx,0x8c61c6deu); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c6e0; /* fmov.s @(r0,r14),fr2 */
L8c61c6e0: sa_motion_publish_stop(ctx,0x8c61c6e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c6e2; /* fadd fr3,fr2 */
L8c61c6e2: sa_motion_publish_stop(ctx,0x8c61c6e2u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c6e4; /* fmov.s @(r0,r13),fr3 */
L8c61c6e4: sa_motion_publish_stop(ctx,0x8c61c6e4u); r[0]=32u;goto L8c61c6e6; /* mov #0x20,r0 */
L8c61c6e6: sa_motion_publish_stop(ctx,0x8c61c6e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c6e8; /* fadd fr3,fr2 */
L8c61c6e8: sa_motion_publish_stop(ctx,0x8c61c6e8u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c6ea; /* fmov fr2,@(r0,r15) */
L8c61c6ea: sa_motion_publish_stop(ctx,0x8c61c6eau); r[0]=8u;goto L8c61c6ec; /* mov #0x8,r0 */
L8c61c6ec: sa_motion_publish_stop(ctx,0x8c61c6ecu); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c6ee; /* fmov.s @(r0,r14),fr2 */
L8c61c6ee: sa_motion_publish_stop(ctx,0x8c61c6eeu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c6f0; /* fmov.s @(r0,r12),fr3 */
L8c61c6f0: sa_motion_publish_stop(ctx,0x8c61c6f0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c6f2; /* fadd fr3,fr2 */
L8c61c6f2: sa_motion_publish_stop(ctx,0x8c61c6f2u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c6f4; /* fmov.s @(r0,r13),fr3 */
L8c61c6f4: sa_motion_publish_stop(ctx,0x8c61c6f4u); r[0]=36u;goto L8c61c6f6; /* mov #0x24,r0 */
L8c61c6f6: sa_motion_publish_stop(ctx,0x8c61c6f6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c6f8; /* fadd fr3,fr2 */
L8c61c6f8: sa_motion_publish_stop(ctx,0x8c61c6f8u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c6fa; /* fmov fr2,@(r0,r15) */
L8c61c6fa: sa_motion_publish_stop(ctx,0x8c61c6fcu); target=r[3];*pr=0x8c61c6feu;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61c6fe; /* jsr @r3 */
L8c61c6fe: sa_motion_publish_stop(ctx,0x8c61c6feu); r[3]=rd(m,bus,&io_status,0x8c61c868u,4);if(io_status)return io_status;goto L8c61c700; /* mov.l 0x8c61c868,r3 */
L8c61c700: sa_motion_publish_stop(ctx,0x8c61c700u); r[0]=28u;goto L8c61c702; /* mov #0x1c,r0 */
L8c61c702: sa_motion_publish_stop(ctx,0x8c61c702u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c704; /* fmov.s @(r0,r15),fr3 */
L8c61c704: sa_motion_publish_stop(ctx,0x8c61c704u); r[0]=32u;goto L8c61c706; /* mov #0x20,r0 */
L8c61c706: sa_motion_publish_stop(ctx,0x8c61c706u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61c708; /* fmov.s @r3,fr2 */
L8c61c708: sa_motion_publish_stop(ctx,0x8c61c708u); r[3]+=8u;goto L8c61c70a; /* add #0x8,r3 */
L8c61c70a: sa_motion_publish_stop(ctx,0x8c61c70au); r[2]=rd(m,bus,&io_status,0x8c61c86cu,4);if(io_status)return io_status;goto L8c61c70c; /* mov.l 0x8c61c86c,r2 */
L8c61c70c: sa_motion_publish_stop(ctx,0x8c61c70cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c70e; /* fmul fr3,fr2 */
L8c61c70e: sa_motion_publish_stop(ctx,0x8c61c70eu); r[1]=rd(m,bus,&io_status,0x8c61c870u,4);if(io_status)return io_status;goto L8c61c710; /* mov.l 0x8c61c870,r1 */
L8c61c710: sa_motion_publish_stop(ctx,0x8c61c710u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61c712; /* fmov.s @r2,fr3 */
L8c61c712: sa_motion_publish_stop(ctx,0x8c61c712u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61c714; /* fmov.s @r1,fr1 */
L8c61c714: sa_motion_publish_stop(ctx,0x8c61c714u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61c716; /* mov.l @r15,r2 */
L8c61c716: sa_motion_publish_stop(ctx,0x8c61c716u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c718; /* fsub fr2,fr3 */
L8c61c718: sa_motion_publish_stop(ctx,0x8c61c718u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c71a; /* fmov.s @(r0,r15),fr2 */
L8c61c71a: sa_motion_publish_stop(ctx,0x8c61c71au); r[0]=36u;goto L8c61c71c; /* mov #0x24,r0 */
L8c61c71c: sa_motion_publish_stop(ctx,0x8c61c71cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c71e; /* fmul fr2,fr1 */
L8c61c71e: sa_motion_publish_stop(ctx,0x8c61c71eu); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c720; /* fmov.s @(r0,r15),fr2 */
L8c61c720: sa_motion_publish_stop(ctx,0x8c61c720u); r[0]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c722; /* mov.w @r10,r0 */
L8c61c722: sa_motion_publish_stop(ctx,0x8c61c722u); *fpul=r[0];goto L8c61c724; /* lds r0,FPUL */
L8c61c724: sa_motion_publish_stop(ctx,0x8c61c724u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c726; /* fsub fr1,fr3 */
L8c61c726: sa_motion_publish_stop(ctx,0x8c61c726u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61c728; /* fmov.s @r3,fr1 */
L8c61c728: sa_motion_publish_stop(ctx,0x8c61c728u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c72a; /* fmul fr2,fr1 */
L8c61c72a: sa_motion_publish_stop(ctx,0x8c61c72au); fcopy(f,4u,3u);goto L8c61c72c; /* fmov fr3,fr4 */
L8c61c72c: sa_motion_publish_stop(ctx,0x8c61c72cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c72e; /* float FPUL,fr3 */
L8c61c72e: sa_motion_publish_stop(ctx,0x8c61c72eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c730; /* fsub fr1,fr4 */
L8c61c730: sa_motion_publish_stop(ctx,0x8c61c730u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61c732; /* ftrc fr3,FPUL */
L8c61c732: sa_motion_publish_stop(ctx,0x8c61c732u); r[0]=*fpul;goto L8c61c734; /* sts FPUL,r0 */
L8c61c734: sa_motion_publish_stop(ctx,0x8c61c734u); wr(m,bus,&io_status,r[2],r[0],2);if(io_status)return io_status;goto L8c61c736; /* mov.w r0,@r2 */
L8c61c736: sa_motion_publish_stop(ctx,0x8c61c736u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c738; /* mov.w @(0x2,r10),r0 */
L8c61c738: sa_motion_publish_stop(ctx,0x8c61c738u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61c73a; /* mov.l @r15,r2 */
L8c61c73a: sa_motion_publish_stop(ctx,0x8c61c73au); r[3]=r[0];goto L8c61c73c; /* mov r0,r3 */
L8c61c73c: sa_motion_publish_stop(ctx,0x8c61c73cu); *fpul=r[3];goto L8c61c73e; /* lds r3,FPUL */
L8c61c73e: sa_motion_publish_stop(ctx,0x8c61c73eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c740; /* float FPUL,fr3 */
L8c61c740: sa_motion_publish_stop(ctx,0x8c61c740u); r[10]+=4u;goto L8c61c742; /* add #0x4,r10 */
L8c61c742: sa_motion_publish_stop(ctx,0x8c61c742u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61c744; /* ftrc fr3,FPUL */
L8c61c744: sa_motion_publish_stop(ctx,0x8c61c744u); r[0]=*fpul;goto L8c61c746; /* sts FPUL,r0 */
L8c61c746: sa_motion_publish_stop(ctx,0x8c61c746u); wr(m,bus,&io_status,r[2]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61c748; /* mov.w r0,@(0x2,r2) */
L8c61c748: sa_motion_publish_stop(ctx,0x8c61c748u); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c74a; /* mov.w @r10,r2 */
L8c61c74a: sa_motion_publish_stop(ctx,0x8c61c74au); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61c74c; /* mov.l @(0x4,r15),r3 */
L8c61c74c: sa_motion_publish_stop(ctx,0x8c61c74cu); *fpul=r[2];goto L8c61c74e; /* lds r2,FPUL */
L8c61c74e: sa_motion_publish_stop(ctx,0x8c61c74eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c750; /* float FPUL,fr3 */
L8c61c750: sa_motion_publish_stop(ctx,0x8c61c750u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61c752; /* ftrc fr3,FPUL */
L8c61c752: sa_motion_publish_stop(ctx,0x8c61c752u); r[0]=*fpul;goto L8c61c754; /* sts FPUL,r0 */
L8c61c754: sa_motion_publish_stop(ctx,0x8c61c754u); wr(m,bus,&io_status,r[3],r[0],2);if(io_status)return io_status;goto L8c61c756; /* mov.w r0,@r3 */
L8c61c756: sa_motion_publish_stop(ctx,0x8c61c756u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c758; /* mov.w @(0x2,r10),r0 */
L8c61c758: sa_motion_publish_stop(ctx,0x8c61c758u); r[10]+=4u;goto L8c61c75a; /* add #0x4,r10 */
L8c61c75a: sa_motion_publish_stop(ctx,0x8c61c75au); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61c75c; /* mov.l @(0x4,r15),r3 */
L8c61c75c: sa_motion_publish_stop(ctx,0x8c61c75cu); r[2]=r[0];goto L8c61c75e; /* mov r0,r2 */
L8c61c75e: sa_motion_publish_stop(ctx,0x8c61c75eu); *fpul=r[2];goto L8c61c760; /* lds r2,FPUL */
L8c61c760: sa_motion_publish_stop(ctx,0x8c61c760u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c762; /* float FPUL,fr3 */
L8c61c762: sa_motion_publish_stop(ctx,0x8c61c762u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61c764; /* ftrc fr3,FPUL */
L8c61c764: sa_motion_publish_stop(ctx,0x8c61c764u); r[0]=*fpul;goto L8c61c766; /* sts FPUL,r0 */
L8c61c766: sa_motion_publish_stop(ctx,0x8c61c766u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61c768; /* mov.w r0,@(0x2,r3) */
L8c61c768: sa_motion_publish_stop(ctx,0x8c61c768u); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c76a; /* mov.w @r10,r2 */
L8c61c76a: sa_motion_publish_stop(ctx,0x8c61c76au); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61c76c; /* mov.l @(0x8,r15),r3 */
L8c61c76c: sa_motion_publish_stop(ctx,0x8c61c76cu); *fpul=r[2];goto L8c61c76e; /* lds r2,FPUL */
L8c61c76e: sa_motion_publish_stop(ctx,0x8c61c76eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c770; /* float FPUL,fr3 */
L8c61c770: sa_motion_publish_stop(ctx,0x8c61c770u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61c772; /* ftrc fr3,FPUL */
L8c61c772: sa_motion_publish_stop(ctx,0x8c61c772u); r[0]=*fpul;goto L8c61c774; /* sts FPUL,r0 */
L8c61c774: sa_motion_publish_stop(ctx,0x8c61c774u); wr(m,bus,&io_status,r[3],r[0],2);if(io_status)return io_status;goto L8c61c776; /* mov.w r0,@r3 */
L8c61c776: sa_motion_publish_stop(ctx,0x8c61c776u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c778; /* mov.w @(0x2,r10),r0 */
L8c61c778: sa_motion_publish_stop(ctx,0x8c61c778u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61c77a; /* mov.l @(0x8,r15),r3 */
L8c61c77a: sa_motion_publish_stop(ctx,0x8c61c77au); r[2]=r[0];goto L8c61c77c; /* mov r0,r2 */
L8c61c77c: sa_motion_publish_stop(ctx,0x8c61c77cu); *fpul=r[2];goto L8c61c77e; /* lds r2,FPUL */
L8c61c77e: sa_motion_publish_stop(ctx,0x8c61c77eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c780; /* float FPUL,fr3 */
L8c61c780: sa_motion_publish_stop(ctx,0x8c61c780u); r[7]=rd(m,bus,&io_status,0x8c61c884u,4);if(io_status)return io_status;goto L8c61c782; /* mov.l 0x8c61c884,r7 */
L8c61c782: sa_motion_publish_stop(ctx,0x8c61c782u); r[5]=r[12];goto L8c61c784; /* mov r12,r5 */
L8c61c784: sa_motion_publish_stop(ctx,0x8c61c784u); r[10]+=4u;goto L8c61c786; /* add #0x4,r10 */
L8c61c786: sa_motion_publish_stop(ctx,0x8c61c786u); r[6]=r[13];goto L8c61c788; /* mov r13,r6 */
L8c61c788: sa_motion_publish_stop(ctx,0x8c61c788u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61c78a; /* ftrc fr3,FPUL */
L8c61c78a: sa_motion_publish_stop(ctx,0x8c61c78au); r[0]=*fpul;goto L8c61c78c; /* sts FPUL,r0 */
L8c61c78c: sa_motion_publish_stop(ctx,0x8c61c78cu); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61c78e; /* mov.w r0,@(0x2,r3) */
L8c61c78e: sa_motion_publish_stop(ctx,0x8c61c78eu); r[3]=rd(m,bus,&io_status,0x8c61c888u,4);if(io_status)return io_status;goto L8c61c790; /* mov.l 0x8c61c888,r3 */
L8c61c790: sa_motion_publish_stop(ctx,0x8c61c792u); target=r[3];*pr=0x8c61c794u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61c794; /* jsr @r3 */
L8c61c794: sa_motion_publish_stop(ctx,0x8c61c794u); r[11]+=1u;goto L8c61c796; /* add #0x1,r11 */
L8c61c796: sa_motion_publish_stop(ctx,0x8c61c796u); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61c798; /* mov.l @(0xc,r15),r2 */
L8c61c798: sa_motion_publish_stop(ctx,0x8c61c798u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61c79a; /* mov.w @(0x2,r2),r0 */
L8c61c79a: sa_motion_publish_stop(ctx,0x8c61c79au); r[0]=r[0]&65535u;goto L8c61c79c; /* extu.w r0,r0 */
L8c61c79c: sa_motion_publish_stop(ctx,0x8c61c79cu); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61c79e; /* cmp/ge r0,r11 */
L8c61c79e: sa_motion_publish_stop(ctx,0x8c61c79eu); if((*sr&1u))goto L8c61c7a4;goto L8c61c7a0; /* bt 0x8c61c7a4 */
L8c61c7a0: sa_motion_publish_stop(ctx,0x8c61c7a2u); goto L8c61c524; /* bra 0x8c61c524 */
L8c61c7a4: sa_motion_publish_stop(ctx,0x8c61c7a6u); goto L8c61cf4c; /* bra 0x8c61cf4c */
L8c61c7a8: sa_motion_publish_stop(ctx,0x8c61c7a8u); r[1]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61c7aa; /* mov.l @(0xc,r15),r1 */
L8c61c7aa: sa_motion_publish_stop(ctx,0x8c61c7aau); r[2]=rd(m,bus,&io_status,0x8c61c88cu,4);if(io_status)return io_status;goto L8c61c7ac; /* mov.l 0x8c61c88c,r2 */
L8c61c7ac: sa_motion_publish_stop(ctx,0x8c61c7acu); r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61c7ae; /* mov.w @(0x2,r1),r0 */
L8c61c7ae: sa_motion_publish_stop(ctx,0x8c61c7aeu); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61c7b0; /* mov.l @r2,r1 */
L8c61c7b0: sa_motion_publish_stop(ctx,0x8c61c7b0u); r[0]=r[0]&65535u;goto L8c61c7b2; /* extu.w r0,r0 */
L8c61c7b2: sa_motion_publish_stop(ctx,0x8c61c7b2u); r[3]=rd(m,bus,&io_status,0x8c61c890u,4);if(io_status)return io_status;goto L8c61c7b4; /* mov.l 0x8c61c890,r3 */
L8c61c7b4: sa_motion_publish_stop(ctx,0x8c61c7b4u); r[0]<<=2;goto L8c61c7b6; /* shll2 r0 */
L8c61c7b6: sa_motion_publish_stop(ctx,0x8c61c7b6u); r[1]+=r[0];goto L8c61c7b8; /* add r0,r1 */
L8c61c7b8: sa_motion_publish_stop(ctx,0x8c61c7b8u); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61c7ba; /* mov.l r1,@r2 */
L8c61c7ba: sa_motion_publish_stop(ctx,0x8c61c7bau); r[2]=0u;goto L8c61c7bc; /* mov #0x0,r2 */
L8c61c7bc: sa_motion_publish_stop(ctx,0x8c61c7bcu); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c61c7be; /* mov.l r2,@(0x8,r15) */
L8c61c7be: sa_motion_publish_stop(ctx,0x8c61c7beu); r[2]=rd(m,bus,&io_status,0x8c61c884u,4);if(io_status)return io_status;goto L8c61c7c0; /* mov.l 0x8c61c884,r2 */
L8c61c7c0: sa_motion_publish_stop(ctx,0x8c61c7c0u); r[1]=rd(m,bus,&io_status,0x8c61c894u,4);if(io_status)return io_status;goto L8c61c7c2; /* mov.l 0x8c61c894,r1 */
L8c61c7c2: sa_motion_publish_stop(ctx,0x8c61c7c2u); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61c7c4; /* mov.l r2,@r15 */
L8c61c7c4: sa_motion_publish_stop(ctx,0x8c61c7c4u); r[2]+=12u;goto L8c61c7c6; /* add #0xc,r2 */
L8c61c7c6: sa_motion_publish_stop(ctx,0x8c61c7c6u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61c7c8; /* mov.l r3,@(0x4,r15) */
L8c61c7c8: sa_motion_publish_stop(ctx,0x8c61c7c8u); wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c61c7ca; /* mov.l r1,@(0x10,r15) */
L8c61c7ca: sa_motion_publish_stop(ctx,0x8c61c7ccu); wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c61cafa; /* bra 0x8c61cafa */
L8c61c7ce: sa_motion_publish_stop(ctx,0x8c61c7ceu); r[11]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c7d0; /* mov.w @r9+,r11 */
L8c61c7d0: sa_motion_publish_stop(ctx,0x8c61c7d0u); r[3]=rd(m,bus,&io_status,0x8c61c898u,4);if(io_status)return io_status;goto L8c61c7d2; /* mov.l 0x8c61c898,r3 */
L8c61c7d2: sa_motion_publish_stop(ctx,0x8c61c7d2u); r[13]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c7d4; /* mov.w @r9+,r13 */
L8c61c7d4: sa_motion_publish_stop(ctx,0x8c61c7d4u); r[11]=r[11]&65535u;goto L8c61c7d6; /* extu.w r11,r11 */
L8c61c7d6: sa_motion_publish_stop(ctx,0x8c61c7d6u); r[11]<<=2;goto L8c61c7d8; /* shll2 r11 */
L8c61c7d8: sa_motion_publish_stop(ctx,0x8c61c7d8u); r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61c7da; /* mov.l @r3,r5 */
L8c61c7da: sa_motion_publish_stop(ctx,0x8c61c7dau); r[14]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c7dc; /* mov.w @r9+,r14 */
L8c61c7dc: sa_motion_publish_stop(ctx,0x8c61c7dcu); r[13]=r[13]&65535u;goto L8c61c7de; /* extu.w r13,r13 */
L8c61c7de: sa_motion_publish_stop(ctx,0x8c61c7deu); r[13]<<=2;goto L8c61c7e0; /* shll2 r13 */
L8c61c7e0: sa_motion_publish_stop(ctx,0x8c61c7e0u); r[12]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c7e2; /* mov.w @r9+,r12 */
L8c61c7e2: sa_motion_publish_stop(ctx,0x8c61c7e2u); r[14]=r[14]&65535u;goto L8c61c7e4; /* extu.w r14,r14 */
L8c61c7e4: sa_motion_publish_stop(ctx,0x8c61c7e4u); r[13]<<=2;goto L8c61c7e6; /* shll2 r13 */
L8c61c7e6: sa_motion_publish_stop(ctx,0x8c61c7e6u); r[11]<<=2;goto L8c61c7e8; /* shll2 r11 */
L8c61c7e8: sa_motion_publish_stop(ctx,0x8c61c7e8u); r[12]=r[12]&65535u;goto L8c61c7ea; /* extu.w r12,r12 */
L8c61c7ea: sa_motion_publish_stop(ctx,0x8c61c7eau); r[14]<<=2;goto L8c61c7ec; /* shll2 r14 */
L8c61c7ec: sa_motion_publish_stop(ctx,0x8c61c7ecu); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61c7ee; /* shll r11 */
L8c61c7ee: sa_motion_publish_stop(ctx,0x8c61c7eeu); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61c7f0; /* shll r13 */
L8c61c7f0: sa_motion_publish_stop(ctx,0x8c61c7f0u); r[11]+=r[5];goto L8c61c7f2; /* add r5,r11 */
L8c61c7f2: sa_motion_publish_stop(ctx,0x8c61c7f2u); r[12]<<=2;goto L8c61c7f4; /* shll2 r12 */
L8c61c7f4: sa_motion_publish_stop(ctx,0x8c61c7f4u); r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61c7f6; /* mov.l @(0xc,r11),r4 */
L8c61c7f6: sa_motion_publish_stop(ctx,0x8c61c7f6u); r[13]+=r[5];goto L8c61c7f8; /* add r5,r13 */
L8c61c7f8: sa_motion_publish_stop(ctx,0x8c61c7f8u); r[14]<<=2;goto L8c61c7fa; /* shll2 r14 */
L8c61c7fa: sa_motion_publish_stop(ctx,0x8c61c7fau); r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61c7fc; /* mov.l @(0xc,r13),r2 */
L8c61c7fc: sa_motion_publish_stop(ctx,0x8c61c7fcu); r[12]<<=2;goto L8c61c7fe; /* shll2 r12 */
L8c61c7fe: sa_motion_publish_stop(ctx,0x8c61c7feu); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61c800; /* shll r14 */
L8c61c800: sa_motion_publish_stop(ctx,0x8c61c800u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61c802; /* shll r12 */
L8c61c802: sa_motion_publish_stop(ctx,0x8c61c802u); r[14]+=r[5];goto L8c61c804; /* add r5,r14 */
L8c61c804: sa_motion_publish_stop(ctx,0x8c61c804u); r[1]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61c806; /* mov.l @(0xc,r14),r1 */
L8c61c806: sa_motion_publish_stop(ctx,0x8c61c806u); r[4]&=r[2];goto L8c61c808; /* and r2,r4 */
L8c61c808: sa_motion_publish_stop(ctx,0x8c61c808u); r[12]+=r[5];goto L8c61c80a; /* add r5,r12 */
L8c61c80a: sa_motion_publish_stop(ctx,0x8c61c80au); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61c80c; /* mov.l @(0xc,r12),r2 */
L8c61c80c: sa_motion_publish_stop(ctx,0x8c61c80cu); r[4]&=r[1];goto L8c61c80e; /* and r1,r4 */
L8c61c80e: sa_motion_publish_stop(ctx,0x8c61c80eu); r[1]=63u;goto L8c61c810; /* mov #0x3f,r1 */
L8c61c810: sa_motion_publish_stop(ctx,0x8c61c810u); r[4]&=r[2];goto L8c61c812; /* and r2,r4 */
L8c61c812: sa_motion_publish_stop(ctx,0x8c61c812u); *sr=(*sr&~1u)|!(r[1]&r[4]);goto L8c61c814; /* tst r1,r4 */
L8c61c814: sa_motion_publish_stop(ctx,0x8c61c814u); if(!(*sr&1u))goto L8c61c85a;goto L8c61c816; /* bf 0x8c61c85a */
L8c61c816: sa_motion_publish_stop(ctx,0x8c61c816u); r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61c818; /* mov.l @(0xc,r11),r4 */
L8c61c818: sa_motion_publish_stop(ctx,0x8c61c818u); r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61c81a; /* mov.l @(0xc,r13),r3 */
L8c61c81a: sa_motion_publish_stop(ctx,0x8c61c81au); r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61c81c; /* mov.l @(0xc,r14),r2 */
L8c61c81c: sa_motion_publish_stop(ctx,0x8c61c81cu); r[4]|=r[3];goto L8c61c81e; /* or r3,r4 */
L8c61c81e: sa_motion_publish_stop(ctx,0x8c61c81eu); r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61c820; /* mov.l @(0xc,r12),r3 */
L8c61c820: sa_motion_publish_stop(ctx,0x8c61c820u); r[4]|=r[2];goto L8c61c822; /* or r2,r4 */
L8c61c822: sa_motion_publish_stop(ctx,0x8c61c822u); r[2]=1u;goto L8c61c824; /* mov #0x1,r2 */
L8c61c824: sa_motion_publish_stop(ctx,0x8c61c824u); r[4]|=r[3];goto L8c61c826; /* or r3,r4 */
L8c61c826: sa_motion_publish_stop(ctx,0x8c61c826u); *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61c828; /* tst r2,r4 */
L8c61c828: sa_motion_publish_stop(ctx,0x8c61c828u); if((*sr&1u))goto L8c61c82e;goto L8c61c82a; /* bt 0x8c61c82e */
L8c61c82a: sa_motion_publish_stop(ctx,0x8c61c82cu); goto L8c61c9d4; /* bra 0x8c61c9d4 */
L8c61c82e: sa_motion_publish_stop(ctx,0x8c61c82eu); r[0]=16u;goto L8c61c830; /* mov #0x10,r0 */
L8c61c830: sa_motion_publish_stop(ctx,0x8c61c830u); fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c832; /* fmov.s @(r0,r13),fr4 */
L8c61c832: sa_motion_publish_stop(ctx,0x8c61c832u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c834; /* fmov.s @(r0,r11),fr3 */
L8c61c834: sa_motion_publish_stop(ctx,0x8c61c834u); r[0]=20u;goto L8c61c836; /* mov #0x14,r0 */
L8c61c836: sa_motion_publish_stop(ctx,0x8c61c836u); fcopy(f,2u,4u);goto L8c61c838; /* fmov fr4,fr2 */
L8c61c838: sa_motion_publish_stop(ctx,0x8c61c838u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61c83a; /* fsub fr3,fr2 */
L8c61c83a: sa_motion_publish_stop(ctx,0x8c61c83au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c83c; /* fmov.s @(r0,r14),fr3 */
L8c61c83c: sa_motion_publish_stop(ctx,0x8c61c83cu); fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c83e; /* fmov.s @(r0,r13),fr5 */
L8c61c83e: sa_motion_publish_stop(ctx,0x8c61c83eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c840; /* fsub fr5,fr3 */
L8c61c840: sa_motion_publish_stop(ctx,0x8c61c840u); fcopy(f,1u,5u);goto L8c61c842; /* fmov fr5,fr1 */
L8c61c842: sa_motion_publish_stop(ctx,0x8c61c842u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c844; /* fmul fr3,fr2 */
L8c61c844: sa_motion_publish_stop(ctx,0x8c61c844u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c846; /* fmov.s @(r0,r11),fr3 */
L8c61c846: sa_motion_publish_stop(ctx,0x8c61c846u); r[0]=16u;goto L8c61c848; /* mov #0x10,r0 */
L8c61c848: sa_motion_publish_stop(ctx,0x8c61c848u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61c84a; /* fsub fr3,fr1 */
L8c61c84a: sa_motion_publish_stop(ctx,0x8c61c84au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c84c; /* fmov.s @(r0,r14),fr3 */
L8c61c84c: sa_motion_publish_stop(ctx,0x8c61c84cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c84e; /* fsub fr4,fr3 */
L8c61c84e: sa_motion_publish_stop(ctx,0x8c61c84eu); fcopy(f,4u,2u);goto L8c61c850; /* fmov fr2,fr4 */
L8c61c850: sa_motion_publish_stop(ctx,0x8c61c850u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c852; /* fmul fr3,fr1 */
L8c61c852: sa_motion_publish_stop(ctx,0x8c61c852u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61c854; /* fldi0 fr3 */
L8c61c854: sa_motion_publish_stop(ctx,0x8c61c854u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c856; /* fsub fr1,fr4 */
L8c61c856: sa_motion_publish_stop(ctx,0x8c61c856u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61c858; /* fcmp/gt fr3,fr4 */
L8c61c858: sa_motion_publish_stop(ctx,0x8c61c858u); if(!(*sr&1u))goto L8c61c89c;goto L8c61c85a; /* bf 0x8c61c89c */
L8c61c85a: sa_motion_publish_stop(ctx,0x8c61c85cu); r[10]+=16u;goto L8c61caf4; /* bra 0x8c61caf4 */
L8c61c89c: sa_motion_publish_stop(ctx,0x8c61c89cu); fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c61c89e; /* fmov.s @r11,fr2 */
L8c61c89e: sa_motion_publish_stop(ctx,0x8c61c89eu); r[0]=28u;goto L8c61c8a0; /* mov #0x1c,r0 */
L8c61c8a0: sa_motion_publish_stop(ctx,0x8c61c8a0u); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61c8a2; /* fmov.s @r13,fr3 */
L8c61c8a2: sa_motion_publish_stop(ctx,0x8c61c8a2u); r[4]=r[15];goto L8c61c8a4; /* mov r15,r4 */
L8c61c8a4: sa_motion_publish_stop(ctx,0x8c61c8a4u); r[3]=rd(m,bus,&io_status,0x8c61cb74u,4);if(io_status)return io_status;goto L8c61c8a6; /* mov.l 0x8c61cb74,r3 */
L8c61c8a6: sa_motion_publish_stop(ctx,0x8c61c8a6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c8a8; /* fadd fr3,fr2 */
L8c61c8a8: sa_motion_publish_stop(ctx,0x8c61c8a8u); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61c8aa; /* fmov.s @r14,fr3 */
L8c61c8aa: sa_motion_publish_stop(ctx,0x8c61c8aau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c8ac; /* fadd fr3,fr2 */
L8c61c8ac: sa_motion_publish_stop(ctx,0x8c61c8acu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c8ae; /* fmov fr2,@(r0,r15) */
L8c61c8ae: sa_motion_publish_stop(ctx,0x8c61c8aeu); r[0]=4u;goto L8c61c8b0; /* mov #0x4,r0 */
L8c61c8b0: sa_motion_publish_stop(ctx,0x8c61c8b0u); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c8b2; /* fmov.s @(r0,r11),fr2 */
L8c61c8b2: sa_motion_publish_stop(ctx,0x8c61c8b2u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c8b4; /* fmov.s @(r0,r13),fr3 */
L8c61c8b4: sa_motion_publish_stop(ctx,0x8c61c8b4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c8b6; /* fadd fr3,fr2 */
L8c61c8b6: sa_motion_publish_stop(ctx,0x8c61c8b6u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c8b8; /* fmov.s @(r0,r14),fr3 */
L8c61c8b8: sa_motion_publish_stop(ctx,0x8c61c8b8u); r[0]=32u;goto L8c61c8ba; /* mov #0x20,r0 */
L8c61c8ba: sa_motion_publish_stop(ctx,0x8c61c8bau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c8bc; /* fadd fr3,fr2 */
L8c61c8bc: sa_motion_publish_stop(ctx,0x8c61c8bcu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c8be; /* fmov fr2,@(r0,r15) */
L8c61c8be: sa_motion_publish_stop(ctx,0x8c61c8beu); r[0]=8u;goto L8c61c8c0; /* mov #0x8,r0 */
L8c61c8c0: sa_motion_publish_stop(ctx,0x8c61c8c0u); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c8c2; /* fmov.s @(r0,r11),fr2 */
L8c61c8c2: sa_motion_publish_stop(ctx,0x8c61c8c2u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c8c4; /* fmov.s @(r0,r13),fr3 */
L8c61c8c4: sa_motion_publish_stop(ctx,0x8c61c8c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c8c6; /* fadd fr3,fr2 */
L8c61c8c6: sa_motion_publish_stop(ctx,0x8c61c8c6u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c8c8; /* fmov.s @(r0,r14),fr3 */
L8c61c8c8: sa_motion_publish_stop(ctx,0x8c61c8c8u); r[0]=36u;goto L8c61c8ca; /* mov #0x24,r0 */
L8c61c8ca: sa_motion_publish_stop(ctx,0x8c61c8cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c8cc; /* fadd fr3,fr2 */
L8c61c8cc: sa_motion_publish_stop(ctx,0x8c61c8ccu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c8ce; /* fmov fr2,@(r0,r15) */
L8c61c8ce: sa_motion_publish_stop(ctx,0x8c61c8d0u); target=r[3];*pr=0x8c61c8d2u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61c8d2; /* jsr @r3 */
L8c61c8d2: sa_motion_publish_stop(ctx,0x8c61c8d2u); r[3]=rd(m,bus,&io_status,0x8c61cb78u,4);if(io_status)return io_status;goto L8c61c8d4; /* mov.l 0x8c61cb78,r3 */
L8c61c8d4: sa_motion_publish_stop(ctx,0x8c61c8d4u); r[0]=28u;goto L8c61c8d6; /* mov #0x1c,r0 */
L8c61c8d6: sa_motion_publish_stop(ctx,0x8c61c8d6u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c8d8; /* fmov.s @(r0,r15),fr3 */
L8c61c8d8: sa_motion_publish_stop(ctx,0x8c61c8d8u); r[0]=32u;goto L8c61c8da; /* mov #0x20,r0 */
L8c61c8da: sa_motion_publish_stop(ctx,0x8c61c8dau); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61c8dc; /* fmov.s @r3,fr2 */
L8c61c8dc: sa_motion_publish_stop(ctx,0x8c61c8dcu); r[8]+=24u;goto L8c61c8de; /* add #0x18,r8 */
L8c61c8de: sa_motion_publish_stop(ctx,0x8c61c8deu); r[2]=rd(m,bus,&io_status,0x8c61cb7cu,4);if(io_status)return io_status;goto L8c61c8e0; /* mov.l 0x8c61cb7c,r2 */
L8c61c8e0: sa_motion_publish_stop(ctx,0x8c61c8e0u); r[3]+=8u;goto L8c61c8e2; /* add #0x8,r3 */
L8c61c8e2: sa_motion_publish_stop(ctx,0x8c61c8e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c8e4; /* fmul fr3,fr2 */
L8c61c8e4: sa_motion_publish_stop(ctx,0x8c61c8e4u); r[1]=rd(m,bus,&io_status,0x8c61cb80u,4);if(io_status)return io_status;goto L8c61c8e6; /* mov.l 0x8c61cb80,r1 */
L8c61c8e6: sa_motion_publish_stop(ctx,0x8c61c8e6u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61c8e8; /* fmov.s @r2,fr3 */
L8c61c8e8: sa_motion_publish_stop(ctx,0x8c61c8e8u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61c8ea; /* fmov.s @r1,fr1 */
L8c61c8ea: sa_motion_publish_stop(ctx,0x8c61c8eau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c8ec; /* fsub fr2,fr3 */
L8c61c8ec: sa_motion_publish_stop(ctx,0x8c61c8ecu); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c8ee; /* fmov.s @(r0,r15),fr2 */
L8c61c8ee: sa_motion_publish_stop(ctx,0x8c61c8eeu); r[0]=36u;goto L8c61c8f0; /* mov #0x24,r0 */
L8c61c8f0: sa_motion_publish_stop(ctx,0x8c61c8f0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c8f2; /* fmul fr2,fr1 */
L8c61c8f2: sa_motion_publish_stop(ctx,0x8c61c8f2u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c8f4; /* fmov.s @(r0,r15),fr2 */
L8c61c8f4: sa_motion_publish_stop(ctx,0x8c61c8f4u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c8f6; /* mov.w @(0x2,r10),r0 */
L8c61c8f6: sa_motion_publish_stop(ctx,0x8c61c8f6u); r[2]=r[0];goto L8c61c8f8; /* mov r0,r2 */
L8c61c8f8: sa_motion_publish_stop(ctx,0x8c61c8f8u); *fpul=r[2];goto L8c61c8fa; /* lds r2,FPUL */
L8c61c8fa: sa_motion_publish_stop(ctx,0x8c61c8fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c8fc; /* fsub fr1,fr3 */
L8c61c8fc: sa_motion_publish_stop(ctx,0x8c61c8fcu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61c8fe; /* fmov.s @r3,fr1 */
L8c61c8fe: sa_motion_publish_stop(ctx,0x8c61c8feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c900; /* fmul fr2,fr1 */
L8c61c900: sa_motion_publish_stop(ctx,0x8c61c900u); fcopy(f,4u,3u);goto L8c61c902; /* fmov fr3,fr4 */
L8c61c902: sa_motion_publish_stop(ctx,0x8c61c902u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c904; /* float FPUL,fr3 */
L8c61c904: sa_motion_publish_stop(ctx,0x8c61c904u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c906; /* fsub fr1,fr4 */
L8c61c906: sa_motion_publish_stop(ctx,0x8c61c906u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c908; /* fmul fr14,fr3 */
L8c61c908: sa_motion_publish_stop(ctx,0x8c61c908u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c90a; /* fmov.s fr3,@-r8 */
L8c61c90a: sa_motion_publish_stop(ctx,0x8c61c90au); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c90c; /* mov.w @r10,r2 */
L8c61c90c: sa_motion_publish_stop(ctx,0x8c61c90cu); *fpul=r[2];goto L8c61c90e; /* lds r2,FPUL */
L8c61c90e: sa_motion_publish_stop(ctx,0x8c61c90eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c910; /* float FPUL,fr3 */
L8c61c910: sa_motion_publish_stop(ctx,0x8c61c910u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c912; /* fmul fr14,fr3 */
L8c61c912: sa_motion_publish_stop(ctx,0x8c61c912u); r[0]=24u;goto L8c61c914; /* mov #0x18,r0 */
L8c61c914: sa_motion_publish_stop(ctx,0x8c61c914u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c916; /* fmov.s fr3,@-r8 */
L8c61c916: sa_motion_publish_stop(ctx,0x8c61c916u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c918; /* fmov.s @(r0,r11),fr3 */
L8c61c918: sa_motion_publish_stop(ctx,0x8c61c918u); r[0]=20u;goto L8c61c91a; /* mov #0x14,r0 */
L8c61c91a: sa_motion_publish_stop(ctx,0x8c61c91au); r[2]=rd(m,bus,&io_status,0x8c61cb84u,4);if(io_status)return io_status;goto L8c61c91c; /* mov.l 0x8c61cb84,r2 */
L8c61c91c: sa_motion_publish_stop(ctx,0x8c61c91cu); r[10]+=4u;goto L8c61c91e; /* add #0x4,r10 */
L8c61c91e: sa_motion_publish_stop(ctx,0x8c61c91eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c920; /* fmov.s fr3,@-r8 */
L8c61c920: sa_motion_publish_stop(ctx,0x8c61c920u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c922; /* fmov.s @(r0,r11),fr3 */
L8c61c922: sa_motion_publish_stop(ctx,0x8c61c922u); r[0]=16u;goto L8c61c924; /* mov #0x10,r0 */
L8c61c924: sa_motion_publish_stop(ctx,0x8c61c924u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c926; /* fmov.s fr3,@-r8 */
L8c61c926: sa_motion_publish_stop(ctx,0x8c61c926u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c928; /* fmov.s @(r0,r11),fr3 */
L8c61c928: sa_motion_publish_stop(ctx,0x8c61c928u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c92a; /* fmov.s fr3,@-r8 */
L8c61c92a: sa_motion_publish_stop(ctx,0x8c61c92au); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c92c; /* mov.l r2,@-r8 */
L8c61c92c: sa_motion_publish_stop(ctx,0x8c61c92cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c92e; /* pref @r8 */
L8c61c92e: sa_motion_publish_stop(ctx,0x8c61c92eu); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c930; /* mov.w @(0x2,r10),r0 */
L8c61c930: sa_motion_publish_stop(ctx,0x8c61c930u); r[8]+=56u;goto L8c61c932; /* add #0x38,r8 */
L8c61c932: sa_motion_publish_stop(ctx,0x8c61c932u); r[2]=r[0];goto L8c61c934; /* mov r0,r2 */
L8c61c934: sa_motion_publish_stop(ctx,0x8c61c934u); *fpul=r[2];goto L8c61c936; /* lds r2,FPUL */
L8c61c936: sa_motion_publish_stop(ctx,0x8c61c936u); r[0]=24u;goto L8c61c938; /* mov #0x18,r0 */
L8c61c938: sa_motion_publish_stop(ctx,0x8c61c938u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c93a; /* float FPUL,fr3 */
L8c61c93a: sa_motion_publish_stop(ctx,0x8c61c93au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c93c; /* fmul fr14,fr3 */
L8c61c93c: sa_motion_publish_stop(ctx,0x8c61c93cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c93e; /* fmov.s fr3,@-r8 */
L8c61c93e: sa_motion_publish_stop(ctx,0x8c61c93eu); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c940; /* mov.w @r10,r2 */
L8c61c940: sa_motion_publish_stop(ctx,0x8c61c940u); r[10]+=4u;goto L8c61c942; /* add #0x4,r10 */
L8c61c942: sa_motion_publish_stop(ctx,0x8c61c942u); *fpul=r[2];goto L8c61c944; /* lds r2,FPUL */
L8c61c944: sa_motion_publish_stop(ctx,0x8c61c944u); r[2]=rd(m,bus,&io_status,0x8c61cb84u,4);if(io_status)return io_status;goto L8c61c946; /* mov.l 0x8c61cb84,r2 */
L8c61c946: sa_motion_publish_stop(ctx,0x8c61c946u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c948; /* float FPUL,fr3 */
L8c61c948: sa_motion_publish_stop(ctx,0x8c61c948u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c94a; /* fmul fr14,fr3 */
L8c61c94a: sa_motion_publish_stop(ctx,0x8c61c94au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c94c; /* fmov.s fr3,@-r8 */
L8c61c94c: sa_motion_publish_stop(ctx,0x8c61c94cu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c94e; /* fmov.s @(r0,r13),fr3 */
L8c61c94e: sa_motion_publish_stop(ctx,0x8c61c94eu); r[0]=20u;goto L8c61c950; /* mov #0x14,r0 */
L8c61c950: sa_motion_publish_stop(ctx,0x8c61c950u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c952; /* fmov.s fr3,@-r8 */
L8c61c952: sa_motion_publish_stop(ctx,0x8c61c952u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c954; /* fmov.s @(r0,r13),fr3 */
L8c61c954: sa_motion_publish_stop(ctx,0x8c61c954u); r[0]=16u;goto L8c61c956; /* mov #0x10,r0 */
L8c61c956: sa_motion_publish_stop(ctx,0x8c61c956u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c958; /* fmov.s fr3,@-r8 */
L8c61c958: sa_motion_publish_stop(ctx,0x8c61c958u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c95a; /* fmov.s @(r0,r13),fr3 */
L8c61c95a: sa_motion_publish_stop(ctx,0x8c61c95au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c95c; /* fmov.s fr3,@-r8 */
L8c61c95c: sa_motion_publish_stop(ctx,0x8c61c95cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c95e; /* mov.l r2,@-r8 */
L8c61c95e: sa_motion_publish_stop(ctx,0x8c61c95eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c960; /* pref @r8 */
L8c61c960: sa_motion_publish_stop(ctx,0x8c61c960u); r[8]+=60u;goto L8c61c962; /* add #0x3c,r8 */
L8c61c962: sa_motion_publish_stop(ctx,0x8c61c962u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61c964; /* fmov.s fr4,@-r8 */
L8c61c964: sa_motion_publish_stop(ctx,0x8c61c964u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c966; /* mov.w @(0x2,r10),r0 */
L8c61c966: sa_motion_publish_stop(ctx,0x8c61c966u); r[2]=r[0];goto L8c61c968; /* mov r0,r2 */
L8c61c968: sa_motion_publish_stop(ctx,0x8c61c968u); *fpul=r[2];goto L8c61c96a; /* lds r2,FPUL */
L8c61c96a: sa_motion_publish_stop(ctx,0x8c61c96au); r[0]=24u;goto L8c61c96c; /* mov #0x18,r0 */
L8c61c96c: sa_motion_publish_stop(ctx,0x8c61c96cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c96e; /* float FPUL,fr3 */
L8c61c96e: sa_motion_publish_stop(ctx,0x8c61c96eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c970; /* fmul fr14,fr3 */
L8c61c970: sa_motion_publish_stop(ctx,0x8c61c970u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c972; /* fmov.s fr3,@-r8 */
L8c61c972: sa_motion_publish_stop(ctx,0x8c61c972u); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c974; /* mov.w @r10,r2 */
L8c61c974: sa_motion_publish_stop(ctx,0x8c61c974u); r[10]+=4u;goto L8c61c976; /* add #0x4,r10 */
L8c61c976: sa_motion_publish_stop(ctx,0x8c61c976u); *fpul=r[2];goto L8c61c978; /* lds r2,FPUL */
L8c61c978: sa_motion_publish_stop(ctx,0x8c61c978u); r[2]=rd(m,bus,&io_status,0x8c61cb84u,4);if(io_status)return io_status;goto L8c61c97a; /* mov.l 0x8c61cb84,r2 */
L8c61c97a: sa_motion_publish_stop(ctx,0x8c61c97au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c97c; /* float FPUL,fr3 */
L8c61c97c: sa_motion_publish_stop(ctx,0x8c61c97cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c97e; /* fmul fr14,fr3 */
L8c61c97e: sa_motion_publish_stop(ctx,0x8c61c97eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c980; /* fmov.s fr3,@-r8 */
L8c61c980: sa_motion_publish_stop(ctx,0x8c61c980u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c982; /* fmov.s @(r0,r14),fr3 */
L8c61c982: sa_motion_publish_stop(ctx,0x8c61c982u); r[0]=20u;goto L8c61c984; /* mov #0x14,r0 */
L8c61c984: sa_motion_publish_stop(ctx,0x8c61c984u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c986; /* fmov.s fr3,@-r8 */
L8c61c986: sa_motion_publish_stop(ctx,0x8c61c986u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c988; /* fmov.s @(r0,r14),fr3 */
L8c61c988: sa_motion_publish_stop(ctx,0x8c61c988u); r[0]=16u;goto L8c61c98a; /* mov #0x10,r0 */
L8c61c98a: sa_motion_publish_stop(ctx,0x8c61c98au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c98c; /* fmov.s fr3,@-r8 */
L8c61c98c: sa_motion_publish_stop(ctx,0x8c61c98cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c98e; /* fmov.s @(r0,r14),fr3 */
L8c61c98e: sa_motion_publish_stop(ctx,0x8c61c98eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c990; /* fmov.s fr3,@-r8 */
L8c61c990: sa_motion_publish_stop(ctx,0x8c61c990u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c992; /* mov.l r2,@-r8 */
L8c61c992: sa_motion_publish_stop(ctx,0x8c61c992u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c994; /* pref @r8 */
L8c61c994: sa_motion_publish_stop(ctx,0x8c61c994u); r[8]+=60u;goto L8c61c996; /* add #0x3c,r8 */
L8c61c996: sa_motion_publish_stop(ctx,0x8c61c996u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61c998; /* fmov.s fr4,@-r8 */
L8c61c998: sa_motion_publish_stop(ctx,0x8c61c998u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c99a; /* mov.w @(0x2,r10),r0 */
L8c61c99a: sa_motion_publish_stop(ctx,0x8c61c99au); r[2]=r[0];goto L8c61c99c; /* mov r0,r2 */
L8c61c99c: sa_motion_publish_stop(ctx,0x8c61c99cu); *fpul=r[2];goto L8c61c99e; /* lds r2,FPUL */
L8c61c99e: sa_motion_publish_stop(ctx,0x8c61c99eu); r[0]=24u;goto L8c61c9a0; /* mov #0x18,r0 */
L8c61c9a0: sa_motion_publish_stop(ctx,0x8c61c9a0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c9a2; /* float FPUL,fr3 */
L8c61c9a2: sa_motion_publish_stop(ctx,0x8c61c9a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c9a4; /* fmul fr14,fr3 */
L8c61c9a4: sa_motion_publish_stop(ctx,0x8c61c9a4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c9a6; /* fmov.s fr3,@-r8 */
L8c61c9a6: sa_motion_publish_stop(ctx,0x8c61c9a6u); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c9a8; /* mov.w @r10,r2 */
L8c61c9a8: sa_motion_publish_stop(ctx,0x8c61c9a8u); r[10]+=4u;goto L8c61c9aa; /* add #0x4,r10 */
L8c61c9aa: sa_motion_publish_stop(ctx,0x8c61c9aau); *fpul=r[2];goto L8c61c9ac; /* lds r2,FPUL */
L8c61c9ac: sa_motion_publish_stop(ctx,0x8c61c9acu); r[2]=rd(m,bus,&io_status,0x8c61cb88u,4);if(io_status)return io_status;goto L8c61c9ae; /* mov.l 0x8c61cb88,r2 */
L8c61c9ae: sa_motion_publish_stop(ctx,0x8c61c9aeu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c9b0; /* float FPUL,fr3 */
L8c61c9b0: sa_motion_publish_stop(ctx,0x8c61c9b0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c9b2; /* fmul fr14,fr3 */
L8c61c9b2: sa_motion_publish_stop(ctx,0x8c61c9b2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c9b4; /* fmov.s fr3,@-r8 */
L8c61c9b4: sa_motion_publish_stop(ctx,0x8c61c9b4u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c9b6; /* fmov.s @(r0,r12),fr3 */
L8c61c9b6: sa_motion_publish_stop(ctx,0x8c61c9b6u); r[0]=20u;goto L8c61c9b8; /* mov #0x14,r0 */
L8c61c9b8: sa_motion_publish_stop(ctx,0x8c61c9b8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c9ba; /* fmov.s fr3,@-r8 */
L8c61c9ba: sa_motion_publish_stop(ctx,0x8c61c9bau); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c9bc; /* fmov.s @(r0,r12),fr3 */
L8c61c9bc: sa_motion_publish_stop(ctx,0x8c61c9bcu); r[0]=16u;goto L8c61c9be; /* mov #0x10,r0 */
L8c61c9be: sa_motion_publish_stop(ctx,0x8c61c9beu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c9c0; /* fmov.s fr3,@-r8 */
L8c61c9c0: sa_motion_publish_stop(ctx,0x8c61c9c0u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c9c2; /* fmov.s @(r0,r12),fr3 */
L8c61c9c2: sa_motion_publish_stop(ctx,0x8c61c9c2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c9c4; /* fmov.s fr3,@-r8 */
L8c61c9c4: sa_motion_publish_stop(ctx,0x8c61c9c4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c9c6; /* mov.l r2,@-r8 */
L8c61c9c6: sa_motion_publish_stop(ctx,0x8c61c9c6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c9c8; /* pref @r8 */
L8c61c9c8: sa_motion_publish_stop(ctx,0x8c61c9c8u); r[0]=rd(m,bus,&io_status,0x8c61cb8cu,4);if(io_status)return io_status;goto L8c61c9ca; /* mov.l 0x8c61cb8c,r0 */
L8c61c9ca: sa_motion_publish_stop(ctx,0x8c61c9cau); r[8]+=32u;goto L8c61c9cc; /* add #0x20,r8 */
L8c61c9cc: sa_motion_publish_stop(ctx,0x8c61c9ccu); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61c9ce; /* mov.l @r0,r2 */
L8c61c9ce: sa_motion_publish_stop(ctx,0x8c61c9ceu); r[2]+=4u;goto L8c61c9d0; /* add #0x4,r2 */
L8c61c9d0: sa_motion_publish_stop(ctx,0x8c61c9d2u); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61caf4; /* bra 0x8c61caf4 */
L8c61c9d4: sa_motion_publish_stop(ctx,0x8c61c9d4u); r[0]=28u;goto L8c61c9d6; /* mov #0x1c,r0 */
L8c61c9d6: sa_motion_publish_stop(ctx,0x8c61c9d6u); r[1]=rd(m,bus,&io_status,0x8c61cb90u,4);if(io_status)return io_status;goto L8c61c9d8; /* mov.l 0x8c61cb90,r1 */
L8c61c9d8: sa_motion_publish_stop(ctx,0x8c61c9d8u); r[3]=rd(m,bus,&io_status,0x8c61cb74u,4);if(io_status)return io_status;goto L8c61c9da; /* mov.l 0x8c61cb74,r3 */
L8c61c9da: sa_motion_publish_stop(ctx,0x8c61c9dau); r[4]=r[15];goto L8c61c9dc; /* mov r15,r4 */
L8c61c9dc: sa_motion_publish_stop(ctx,0x8c61c9dcu); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61c9de; /* mov.l r2,@r1 */
L8c61c9de: sa_motion_publish_stop(ctx,0x8c61c9deu); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61c9e0; /* fmov.s @r13,fr3 */
L8c61c9e0: sa_motion_publish_stop(ctx,0x8c61c9e0u); fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c61c9e2; /* fmov.s @r11,fr2 */
L8c61c9e2: sa_motion_publish_stop(ctx,0x8c61c9e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c9e4; /* fadd fr3,fr2 */
L8c61c9e4: sa_motion_publish_stop(ctx,0x8c61c9e4u); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61c9e6; /* fmov.s @r14,fr3 */
L8c61c9e6: sa_motion_publish_stop(ctx,0x8c61c9e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c9e8; /* fadd fr3,fr2 */
L8c61c9e8: sa_motion_publish_stop(ctx,0x8c61c9e8u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c9ea; /* fmov fr2,@(r0,r15) */
L8c61c9ea: sa_motion_publish_stop(ctx,0x8c61c9eau); r[0]=4u;goto L8c61c9ec; /* mov #0x4,r0 */
L8c61c9ec: sa_motion_publish_stop(ctx,0x8c61c9ecu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c9ee; /* fmov.s @(r0,r13),fr3 */
L8c61c9ee: sa_motion_publish_stop(ctx,0x8c61c9eeu); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c9f0; /* fmov.s @(r0,r11),fr2 */
L8c61c9f0: sa_motion_publish_stop(ctx,0x8c61c9f0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c9f2; /* fadd fr3,fr2 */
L8c61c9f2: sa_motion_publish_stop(ctx,0x8c61c9f2u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c9f4; /* fmov.s @(r0,r14),fr3 */
L8c61c9f4: sa_motion_publish_stop(ctx,0x8c61c9f4u); r[0]=32u;goto L8c61c9f6; /* mov #0x20,r0 */
L8c61c9f6: sa_motion_publish_stop(ctx,0x8c61c9f6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c9f8; /* fadd fr3,fr2 */
L8c61c9f8: sa_motion_publish_stop(ctx,0x8c61c9f8u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c9fa; /* fmov fr2,@(r0,r15) */
L8c61c9fa: sa_motion_publish_stop(ctx,0x8c61c9fau); r[0]=8u;goto L8c61c9fc; /* mov #0x8,r0 */
L8c61c9fc: sa_motion_publish_stop(ctx,0x8c61c9fcu); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c9fe; /* fmov.s @(r0,r11),fr2 */
L8c61c9fe: sa_motion_publish_stop(ctx,0x8c61c9feu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ca00; /* fmov.s @(r0,r13),fr3 */
L8c61ca00: sa_motion_publish_stop(ctx,0x8c61ca00u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ca02; /* fadd fr3,fr2 */
L8c61ca02: sa_motion_publish_stop(ctx,0x8c61ca02u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ca04; /* fmov.s @(r0,r14),fr3 */
L8c61ca04: sa_motion_publish_stop(ctx,0x8c61ca04u); r[0]=36u;goto L8c61ca06; /* mov #0x24,r0 */
L8c61ca06: sa_motion_publish_stop(ctx,0x8c61ca06u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ca08; /* fadd fr3,fr2 */
L8c61ca08: sa_motion_publish_stop(ctx,0x8c61ca08u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ca0a; /* fmov fr2,@(r0,r15) */
L8c61ca0a: sa_motion_publish_stop(ctx,0x8c61ca0cu); target=r[3];*pr=0x8c61ca0eu;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ca0e; /* jsr @r3 */
L8c61ca0e: sa_motion_publish_stop(ctx,0x8c61ca0eu); r[3]=rd(m,bus,&io_status,0x8c61cb78u,4);if(io_status)return io_status;goto L8c61ca10; /* mov.l 0x8c61cb78,r3 */
L8c61ca10: sa_motion_publish_stop(ctx,0x8c61ca10u); r[0]=28u;goto L8c61ca12; /* mov #0x1c,r0 */
L8c61ca12: sa_motion_publish_stop(ctx,0x8c61ca12u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ca14; /* fmov.s @(r0,r15),fr3 */
L8c61ca14: sa_motion_publish_stop(ctx,0x8c61ca14u); r[0]=32u;goto L8c61ca16; /* mov #0x20,r0 */
L8c61ca16: sa_motion_publish_stop(ctx,0x8c61ca16u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61ca18; /* fmov.s @r3,fr2 */
L8c61ca18: sa_motion_publish_stop(ctx,0x8c61ca18u); r[3]+=8u;goto L8c61ca1a; /* add #0x8,r3 */
L8c61ca1a: sa_motion_publish_stop(ctx,0x8c61ca1au); r[2]=rd(m,bus,&io_status,0x8c61cb7cu,4);if(io_status)return io_status;goto L8c61ca1c; /* mov.l 0x8c61cb7c,r2 */
L8c61ca1c: sa_motion_publish_stop(ctx,0x8c61ca1cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ca1e; /* fmul fr3,fr2 */
L8c61ca1e: sa_motion_publish_stop(ctx,0x8c61ca1eu); r[1]=rd(m,bus,&io_status,0x8c61cb80u,4);if(io_status)return io_status;goto L8c61ca20; /* mov.l 0x8c61cb80,r1 */
L8c61ca20: sa_motion_publish_stop(ctx,0x8c61ca20u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61ca22; /* fmov.s @r2,fr3 */
L8c61ca22: sa_motion_publish_stop(ctx,0x8c61ca22u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61ca24; /* fmov.s @r1,fr1 */
L8c61ca24: sa_motion_publish_stop(ctx,0x8c61ca24u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ca26; /* mov.l @r15,r2 */
L8c61ca26: sa_motion_publish_stop(ctx,0x8c61ca26u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ca28; /* fsub fr2,fr3 */
L8c61ca28: sa_motion_publish_stop(ctx,0x8c61ca28u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ca2a; /* fmov.s @(r0,r15),fr2 */
L8c61ca2a: sa_motion_publish_stop(ctx,0x8c61ca2au); r[0]=36u;goto L8c61ca2c; /* mov #0x24,r0 */
L8c61ca2c: sa_motion_publish_stop(ctx,0x8c61ca2cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ca2e; /* fmul fr2,fr1 */
L8c61ca2e: sa_motion_publish_stop(ctx,0x8c61ca2eu); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ca30; /* fmov.s @(r0,r15),fr2 */
L8c61ca30: sa_motion_publish_stop(ctx,0x8c61ca30u); r[0]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ca32; /* mov.w @r10,r0 */
L8c61ca32: sa_motion_publish_stop(ctx,0x8c61ca32u); *fpul=r[0];goto L8c61ca34; /* lds r0,FPUL */
L8c61ca34: sa_motion_publish_stop(ctx,0x8c61ca34u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ca36; /* fsub fr1,fr3 */
L8c61ca36: sa_motion_publish_stop(ctx,0x8c61ca36u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61ca38; /* fmov.s @r3,fr1 */
L8c61ca38: sa_motion_publish_stop(ctx,0x8c61ca38u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ca3a; /* fmul fr2,fr1 */
L8c61ca3a: sa_motion_publish_stop(ctx,0x8c61ca3au); fcopy(f,15u,3u);goto L8c61ca3c; /* fmov fr3,fr15 */
L8c61ca3c: sa_motion_publish_stop(ctx,0x8c61ca3cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ca3e; /* float FPUL,fr3 */
L8c61ca3e: sa_motion_publish_stop(ctx,0x8c61ca3eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[1],&f->fpscr,&fr[15],1);if(status)return status;}goto L8c61ca40; /* fsub fr1,fr15 */
L8c61ca40: sa_motion_publish_stop(ctx,0x8c61ca40u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ca42; /* ftrc fr3,FPUL */
L8c61ca42: sa_motion_publish_stop(ctx,0x8c61ca42u); r[0]=*fpul;goto L8c61ca44; /* sts FPUL,r0 */
L8c61ca44: sa_motion_publish_stop(ctx,0x8c61ca44u); wr(m,bus,&io_status,r[2],r[0],2);if(io_status)return io_status;goto L8c61ca46; /* mov.w r0,@r2 */
L8c61ca46: sa_motion_publish_stop(ctx,0x8c61ca46u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ca48; /* mov.w @(0x2,r10),r0 */
L8c61ca48: sa_motion_publish_stop(ctx,0x8c61ca48u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ca4a; /* mov.l @r15,r2 */
L8c61ca4a: sa_motion_publish_stop(ctx,0x8c61ca4au); r[3]=r[0];goto L8c61ca4c; /* mov r0,r3 */
L8c61ca4c: sa_motion_publish_stop(ctx,0x8c61ca4cu); *fpul=r[3];goto L8c61ca4e; /* lds r3,FPUL */
L8c61ca4e: sa_motion_publish_stop(ctx,0x8c61ca4eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ca50; /* float FPUL,fr3 */
L8c61ca50: sa_motion_publish_stop(ctx,0x8c61ca50u); r[10]+=4u;goto L8c61ca52; /* add #0x4,r10 */
L8c61ca52: sa_motion_publish_stop(ctx,0x8c61ca52u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ca54; /* ftrc fr3,FPUL */
L8c61ca54: sa_motion_publish_stop(ctx,0x8c61ca54u); r[0]=*fpul;goto L8c61ca56; /* sts FPUL,r0 */
L8c61ca56: sa_motion_publish_stop(ctx,0x8c61ca56u); wr(m,bus,&io_status,r[2]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61ca58; /* mov.w r0,@(0x2,r2) */
L8c61ca58: sa_motion_publish_stop(ctx,0x8c61ca58u); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ca5a; /* mov.w @r10,r2 */
L8c61ca5a: sa_motion_publish_stop(ctx,0x8c61ca5au); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61ca5c; /* mov.l @(0x4,r15),r3 */
L8c61ca5c: sa_motion_publish_stop(ctx,0x8c61ca5cu); *fpul=r[2];goto L8c61ca5e; /* lds r2,FPUL */
L8c61ca5e: sa_motion_publish_stop(ctx,0x8c61ca5eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ca60; /* float FPUL,fr3 */
L8c61ca60: sa_motion_publish_stop(ctx,0x8c61ca60u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ca62; /* ftrc fr3,FPUL */
L8c61ca62: sa_motion_publish_stop(ctx,0x8c61ca62u); r[0]=*fpul;goto L8c61ca64; /* sts FPUL,r0 */
L8c61ca64: sa_motion_publish_stop(ctx,0x8c61ca64u); wr(m,bus,&io_status,r[3],r[0],2);if(io_status)return io_status;goto L8c61ca66; /* mov.w r0,@r3 */
L8c61ca66: sa_motion_publish_stop(ctx,0x8c61ca66u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ca68; /* mov.w @(0x2,r10),r0 */
L8c61ca68: sa_motion_publish_stop(ctx,0x8c61ca68u); r[10]+=4u;goto L8c61ca6a; /* add #0x4,r10 */
L8c61ca6a: sa_motion_publish_stop(ctx,0x8c61ca6au); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61ca6c; /* mov.l @(0x4,r15),r3 */
L8c61ca6c: sa_motion_publish_stop(ctx,0x8c61ca6cu); r[2]=r[0];goto L8c61ca6e; /* mov r0,r2 */
L8c61ca6e: sa_motion_publish_stop(ctx,0x8c61ca6eu); *fpul=r[2];goto L8c61ca70; /* lds r2,FPUL */
L8c61ca70: sa_motion_publish_stop(ctx,0x8c61ca70u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ca72; /* float FPUL,fr3 */
L8c61ca72: sa_motion_publish_stop(ctx,0x8c61ca72u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ca74; /* ftrc fr3,FPUL */
L8c61ca74: sa_motion_publish_stop(ctx,0x8c61ca74u); r[0]=*fpul;goto L8c61ca76; /* sts FPUL,r0 */
L8c61ca76: sa_motion_publish_stop(ctx,0x8c61ca76u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61ca78; /* mov.w r0,@(0x2,r3) */
L8c61ca78: sa_motion_publish_stop(ctx,0x8c61ca78u); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ca7a; /* mov.w @r10,r2 */
L8c61ca7a: sa_motion_publish_stop(ctx,0x8c61ca7au); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61ca7c; /* mov.l @(0x10,r15),r3 */
L8c61ca7c: sa_motion_publish_stop(ctx,0x8c61ca7cu); *fpul=r[2];goto L8c61ca7e; /* lds r2,FPUL */
L8c61ca7e: sa_motion_publish_stop(ctx,0x8c61ca7eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ca80; /* float FPUL,fr3 */
L8c61ca80: sa_motion_publish_stop(ctx,0x8c61ca80u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ca82; /* ftrc fr3,FPUL */
L8c61ca82: sa_motion_publish_stop(ctx,0x8c61ca82u); r[0]=*fpul;goto L8c61ca84; /* sts FPUL,r0 */
L8c61ca84: sa_motion_publish_stop(ctx,0x8c61ca84u); wr(m,bus,&io_status,r[3],r[0],2);if(io_status)return io_status;goto L8c61ca86; /* mov.w r0,@r3 */
L8c61ca86: sa_motion_publish_stop(ctx,0x8c61ca86u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ca88; /* mov.w @(0x2,r10),r0 */
L8c61ca88: sa_motion_publish_stop(ctx,0x8c61ca88u); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61ca8a; /* mov.l @(0x10,r15),r3 */
L8c61ca8a: sa_motion_publish_stop(ctx,0x8c61ca8au); r[2]=r[0];goto L8c61ca8c; /* mov r0,r2 */
L8c61ca8c: sa_motion_publish_stop(ctx,0x8c61ca8cu); *fpul=r[2];goto L8c61ca8e; /* lds r2,FPUL */
L8c61ca8e: sa_motion_publish_stop(ctx,0x8c61ca8eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ca90; /* float FPUL,fr3 */
L8c61ca90: sa_motion_publish_stop(ctx,0x8c61ca90u); r[10]+=4u;goto L8c61ca92; /* add #0x4,r10 */
L8c61ca92: sa_motion_publish_stop(ctx,0x8c61ca92u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ca94; /* ftrc fr3,FPUL */
L8c61ca94: sa_motion_publish_stop(ctx,0x8c61ca94u); r[0]=*fpul;goto L8c61ca96; /* sts FPUL,r0 */
L8c61ca96: sa_motion_publish_stop(ctx,0x8c61ca96u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61ca98; /* mov.w r0,@(0x2,r3) */
L8c61ca98: sa_motion_publish_stop(ctx,0x8c61ca98u); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ca9a; /* mov.w @r10,r2 */
L8c61ca9a: sa_motion_publish_stop(ctx,0x8c61ca9au); r[3]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c61ca9c; /* mov.l @(0x14,r15),r3 */
L8c61ca9c: sa_motion_publish_stop(ctx,0x8c61ca9cu); *fpul=r[2];goto L8c61ca9e; /* lds r2,FPUL */
L8c61ca9e: sa_motion_publish_stop(ctx,0x8c61ca9eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61caa0; /* float FPUL,fr3 */
L8c61caa0: sa_motion_publish_stop(ctx,0x8c61caa0u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61caa2; /* ftrc fr3,FPUL */
L8c61caa2: sa_motion_publish_stop(ctx,0x8c61caa2u); r[0]=*fpul;goto L8c61caa4; /* sts FPUL,r0 */
L8c61caa4: sa_motion_publish_stop(ctx,0x8c61caa4u); wr(m,bus,&io_status,r[3],r[0],2);if(io_status)return io_status;goto L8c61caa6; /* mov.w r0,@r3 */
L8c61caa6: sa_motion_publish_stop(ctx,0x8c61caa6u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61caa8; /* mov.w @(0x2,r10),r0 */
L8c61caa8: sa_motion_publish_stop(ctx,0x8c61caa8u); r[3]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c61caaa; /* mov.l @(0x14,r15),r3 */
L8c61caaa: sa_motion_publish_stop(ctx,0x8c61caaau); r[2]=r[0];goto L8c61caac; /* mov r0,r2 */
L8c61caac: sa_motion_publish_stop(ctx,0x8c61caacu); *fpul=r[2];goto L8c61caae; /* lds r2,FPUL */
L8c61caae: sa_motion_publish_stop(ctx,0x8c61caaeu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cab0; /* float FPUL,fr3 */
L8c61cab0: sa_motion_publish_stop(ctx,0x8c61cab0u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61cab2; /* ftrc fr3,FPUL */
L8c61cab2: sa_motion_publish_stop(ctx,0x8c61cab2u); r[0]=*fpul;goto L8c61cab4; /* sts FPUL,r0 */
L8c61cab4: sa_motion_publish_stop(ctx,0x8c61cab4u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61cab6; /* mov.w r0,@(0x2,r3) */
L8c61cab6: sa_motion_publish_stop(ctx,0x8c61cab6u); r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61cab8; /* mov.l @(0xc,r11),r4 */
L8c61cab8: sa_motion_publish_stop(ctx,0x8c61cab8u); r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61caba; /* mov.l @(0xc,r13),r3 */
L8c61caba: sa_motion_publish_stop(ctx,0x8c61cabau); r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61cabc; /* mov.l @(0xc,r14),r2 */
L8c61cabc: sa_motion_publish_stop(ctx,0x8c61cabcu); r[4]&=r[3];goto L8c61cabe; /* and r3,r4 */
L8c61cabe: sa_motion_publish_stop(ctx,0x8c61cabeu); r[3]=1u;goto L8c61cac0; /* mov #0x1,r3 */
L8c61cac0: sa_motion_publish_stop(ctx,0x8c61cac0u); r[4]&=r[2];goto L8c61cac2; /* and r2,r4 */
L8c61cac2: sa_motion_publish_stop(ctx,0x8c61cac2u); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61cac4; /* tst r3,r4 */
L8c61cac4: sa_motion_publish_stop(ctx,0x8c61cac6u); take=!(*sr&1u);r[10]+=4u;if(take)goto L8c61cad6;goto L8c61cac8; /* bf/s 0x8c61cad6 */
L8c61cac8: sa_motion_publish_stop(ctx,0x8c61cac8u); r[7]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61caca; /* mov.l @r15,r7 */
L8c61caca: sa_motion_publish_stop(ctx,0x8c61cacau); r[5]=r[13];goto L8c61cacc; /* mov r13,r5 */
L8c61cacc: sa_motion_publish_stop(ctx,0x8c61caccu); r[2]=rd(m,bus,&io_status,0x8c61cb94u,4);if(io_status)return io_status;goto L8c61cace; /* mov.l 0x8c61cb94,r2 */
L8c61cace: sa_motion_publish_stop(ctx,0x8c61caceu); r[6]=r[14];goto L8c61cad0; /* mov r14,r6 */
L8c61cad0: sa_motion_publish_stop(ctx,0x8c61cad0u); fcopy(f,4u,15u);goto L8c61cad2; /* fmov fr15,fr4 */
L8c61cad2: sa_motion_publish_stop(ctx,0x8c61cad4u); target=r[2];*pr=0x8c61cad6u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61cad6; /* jsr @r2 */
L8c61cad6: sa_motion_publish_stop(ctx,0x8c61cad6u); r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61cad8; /* mov.l @(0xc,r13),r4 */
L8c61cad8: sa_motion_publish_stop(ctx,0x8c61cad8u); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61cada; /* mov.l @(0xc,r14),r3 */
L8c61cada: sa_motion_publish_stop(ctx,0x8c61cadau); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61cadc; /* mov.l @(0xc,r12),r2 */
L8c61cadc: sa_motion_publish_stop(ctx,0x8c61cadcu); r[4]&=r[3];goto L8c61cade; /* and r3,r4 */
L8c61cade: sa_motion_publish_stop(ctx,0x8c61cadeu); r[3]=1u;goto L8c61cae0; /* mov #0x1,r3 */
L8c61cae0: sa_motion_publish_stop(ctx,0x8c61cae0u); r[4]&=r[2];goto L8c61cae2; /* and r2,r4 */
L8c61cae2: sa_motion_publish_stop(ctx,0x8c61cae2u); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61cae4; /* tst r3,r4 */
L8c61cae4: sa_motion_publish_stop(ctx,0x8c61cae4u); if(!(*sr&1u))goto L8c61caf4;goto L8c61cae6; /* bf 0x8c61caf4 */
L8c61cae6: sa_motion_publish_stop(ctx,0x8c61cae6u); r[7]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61cae8; /* mov.l @(0x4,r15),r7 */
L8c61cae8: sa_motion_publish_stop(ctx,0x8c61cae8u); r[5]=r[14];goto L8c61caea; /* mov r14,r5 */
L8c61caea: sa_motion_publish_stop(ctx,0x8c61caeau); r[2]=rd(m,bus,&io_status,0x8c61cb94u,4);if(io_status)return io_status;goto L8c61caec; /* mov.l 0x8c61cb94,r2 */
L8c61caec: sa_motion_publish_stop(ctx,0x8c61caecu); r[6]=r[12];goto L8c61caee; /* mov r12,r6 */
L8c61caee: sa_motion_publish_stop(ctx,0x8c61caeeu); fcopy(f,4u,15u);goto L8c61caf0; /* fmov fr15,fr4 */
L8c61caf0: sa_motion_publish_stop(ctx,0x8c61caf2u); target=r[2];*pr=0x8c61caf4u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61caf4; /* jsr @r2 */
L8c61caf4: sa_motion_publish_stop(ctx,0x8c61caf4u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61caf6; /* mov.l @(0x8,r15),r3 */
L8c61caf6: sa_motion_publish_stop(ctx,0x8c61caf6u); r[3]+=1u;goto L8c61caf8; /* add #0x1,r3 */
L8c61caf8: sa_motion_publish_stop(ctx,0x8c61caf8u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c61cafa; /* mov.l r3,@(0x8,r15) */
L8c61cafa: sa_motion_publish_stop(ctx,0x8c61cafau); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61cafc; /* mov.l @(0xc,r15),r2 */
L8c61cafc: sa_motion_publish_stop(ctx,0x8c61cafcu); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61cafe; /* mov.l @(0x8,r15),r3 */
L8c61cafe: sa_motion_publish_stop(ctx,0x8c61cafeu); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61cb00; /* mov.w @(0x2,r2),r0 */
L8c61cb00: sa_motion_publish_stop(ctx,0x8c61cb00u); r[0]=r[0]&65535u;goto L8c61cb02; /* extu.w r0,r0 */
L8c61cb02: sa_motion_publish_stop(ctx,0x8c61cb02u); *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c61cb04; /* cmp/ge r0,r3 */
L8c61cb04: sa_motion_publish_stop(ctx,0x8c61cb04u); if((*sr&1u))goto L8c61cb0a;goto L8c61cb06; /* bt 0x8c61cb0a */
L8c61cb06: sa_motion_publish_stop(ctx,0x8c61cb08u); goto L8c61c7ce; /* bra 0x8c61c7ce */
L8c61cb0a: sa_motion_publish_stop(ctx,0x8c61cb0cu); goto L8c61cf4c; /* bra 0x8c61cf4c */
L8c61cb0e: sa_motion_publish_stop(ctx,0x8c61cb0eu); r[4]=rd(m,bus,&io_status,0x8c61cb98u,4);if(io_status)return io_status;goto L8c61cb10; /* mov.l 0x8c61cb98,r4 */
L8c61cb10: sa_motion_publish_stop(ctx,0x8c61cb10u); r[2]=0u;goto L8c61cb12; /* mov #0x0,r2 */
L8c61cb12: sa_motion_publish_stop(ctx,0x8c61cb12u); wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c61cb14; /* mov.l r2,@(0x14,r15) */
L8c61cb14: sa_motion_publish_stop(ctx,0x8c61cb14u); r[2]=r[4];goto L8c61cb16; /* mov r4,r2 */
L8c61cb16: sa_motion_publish_stop(ctx,0x8c61cb16u); r[1]=r[4];goto L8c61cb18; /* mov r4,r1 */
L8c61cb18: sa_motion_publish_stop(ctx,0x8c61cb18u); r[2]+=8u;goto L8c61cb1a; /* add #0x8,r2 */
L8c61cb1a: sa_motion_publish_stop(ctx,0x8c61cb1au); r[4]+=4u;goto L8c61cb1c; /* add #0x4,r4 */
L8c61cb1c: sa_motion_publish_stop(ctx,0x8c61cb1cu); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c61cb1e; /* mov.l r2,@(0x8,r15) */
L8c61cb1e: sa_motion_publish_stop(ctx,0x8c61cb1eu); wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c61cb20; /* mov.l r1,@(0x10,r15) */
L8c61cb20: sa_motion_publish_stop(ctx,0x8c61cb22u); wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c61cefa; /* bra 0x8c61cefa */
L8c61cb24: sa_motion_publish_stop(ctx,0x8c61cb24u); r[2]=rd(m,bus,&io_status,0x8c61cb9cu,4);if(io_status)return io_status;goto L8c61cb26; /* mov.l 0x8c61cb9c,r2 */
L8c61cb26: sa_motion_publish_stop(ctx,0x8c61cb26u); r[5]=0u;goto L8c61cb28; /* mov #0x0,r5 */
L8c61cb28: sa_motion_publish_stop(ctx,0x8c61cb28u); r[3]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61cb2a; /* mov.w @r9+,r3 */
L8c61cb2a: sa_motion_publish_stop(ctx,0x8c61cb2au); r[7]=r[5];goto L8c61cb2c; /* mov r5,r7 */
L8c61cb2c: sa_motion_publish_stop(ctx,0x8c61cb2cu); r[6]=0u;goto L8c61cb2e; /* mov #0x0,r6 */
L8c61cb2e: sa_motion_publish_stop(ctx,0x8c61cb2eu); r[3]=r[3]&65535u;goto L8c61cb30; /* extu.w r3,r3 */
L8c61cb30: sa_motion_publish_stop(ctx,0x8c61cb30u); *sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c61cb32; /* cmp/pl r3 */
L8c61cb32: sa_motion_publish_stop(ctx,0x8c61cb32u); r[4]=4294967295u;goto L8c61cb34; /* mov #-0x1,r4 */
L8c61cb34: sa_motion_publish_stop(ctx,0x8c61cb34u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61cb36; /* mov.l r3,@r15 */
L8c61cb36: sa_motion_publish_stop(ctx,0x8c61cb36u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61cb38; /* mov.l @r2,r1 */
L8c61cb38: sa_motion_publish_stop(ctx,0x8c61cb38u); r[1]+=r[3];goto L8c61cb3a; /* add r3,r1 */
L8c61cb3a: sa_motion_publish_stop(ctx,0x8c61cb3au); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61cb3c; /* mov.l r1,@r2 */
L8c61cb3c: sa_motion_publish_stop(ctx,0x8c61cb3eu); take=!(*sr&1u);r[6]+=r[9];if(take)goto L8c61cb60;goto L8c61cb40; /* bf/s 0x8c61cb60 */
L8c61cb40: sa_motion_publish_stop(ctx,0x8c61cb40u); r[11]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c61cb42; /* mov.w @r6+,r11 */
L8c61cb42: sa_motion_publish_stop(ctx,0x8c61cb42u); r[7]+=1u;goto L8c61cb44; /* add #0x1,r7 */
L8c61cb44: sa_motion_publish_stop(ctx,0x8c61cb44u); r[2]=rd(m,bus,&io_status,0x8c61cba0u,4);if(io_status)return io_status;goto L8c61cb46; /* mov.l 0x8c61cba0,r2 */
L8c61cb46: sa_motion_publish_stop(ctx,0x8c61cb46u); r[11]=r[11]&65535u;goto L8c61cb48; /* extu.w r11,r11 */
L8c61cb48: sa_motion_publish_stop(ctx,0x8c61cb48u); r[11]<<=2;goto L8c61cb4a; /* shll2 r11 */
L8c61cb4a: sa_motion_publish_stop(ctx,0x8c61cb4au); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61cb4c; /* mov.l @r2,r3 */
L8c61cb4c: sa_motion_publish_stop(ctx,0x8c61cb4cu); r[11]<<=2;goto L8c61cb4e; /* shll2 r11 */
L8c61cb4e: sa_motion_publish_stop(ctx,0x8c61cb4eu); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61cb50; /* shll r11 */
L8c61cb50: sa_motion_publish_stop(ctx,0x8c61cb50u); r[11]+=r[3];goto L8c61cb52; /* add r3,r11 */
L8c61cb52: sa_motion_publish_stop(ctx,0x8c61cb52u); wr(m,bus,&io_status,r[15]+0x18u,r[11],4);if(io_status)return io_status;goto L8c61cb54; /* mov.l r11,@(0x18,r15) */
L8c61cb54: sa_motion_publish_stop(ctx,0x8c61cb54u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61cb56; /* mov.l @r15,r3 */
L8c61cb56: sa_motion_publish_stop(ctx,0x8c61cb56u); r[11]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61cb58; /* mov.l @(0xc,r11),r11 */
L8c61cb58: sa_motion_publish_stop(ctx,0x8c61cb58u); *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[3]);goto L8c61cb5a; /* cmp/ge r3,r7 */
L8c61cb5a: sa_motion_publish_stop(ctx,0x8c61cb5au); r[4]&=r[11];goto L8c61cb5c; /* and r11,r4 */
L8c61cb5c: sa_motion_publish_stop(ctx,0x8c61cb5eu); take=!(*sr&1u);r[5]|=r[11];if(take)goto L8c61cb40;goto L8c61cb60; /* bf/s 0x8c61cb40 */
L8c61cb60: sa_motion_publish_stop(ctx,0x8c61cb60u); r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61cb62; /* mov.l @r15,r6 */
L8c61cb62: sa_motion_publish_stop(ctx,0x8c61cb62u); r[3]=63u;goto L8c61cb64; /* mov #0x3f,r3 */
L8c61cb64: sa_motion_publish_stop(ctx,0x8c61cb64u); r[7]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61cb66; /* mov.l @r15,r7 */
L8c61cb66: sa_motion_publish_stop(ctx,0x8c61cb66u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61cb68; /* shll r6 */
L8c61cb68: sa_motion_publish_stop(ctx,0x8c61cb68u); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61cb6a; /* tst r3,r4 */
L8c61cb6a: sa_motion_publish_stop(ctx,0x8c61cb6cu); take=(*sr&1u);r[7]<<=2;if(take)goto L8c61cba4;goto L8c61cb6e; /* bt/s 0x8c61cba4 */
L8c61cb6e: sa_motion_publish_stop(ctx,0x8c61cb6eu); r[10]+=r[7];goto L8c61cb70; /* add r7,r10 */
L8c61cb70: sa_motion_publish_stop(ctx,0x8c61cb72u); r[9]+=r[6];goto L8c61cef4; /* bra 0x8c61cef4 */
L8c61cba4: sa_motion_publish_stop(ctx,0x8c61cba4u); r[3]=1u;goto L8c61cba6; /* mov #0x1,r3 */
L8c61cba6: sa_motion_publish_stop(ctx,0x8c61cba6u); *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61cba8; /* tst r3,r5 */
L8c61cba8: sa_motion_publish_stop(ctx,0x8c61cba8u); if((*sr&1u))goto L8c61cbae;goto L8c61cbaa; /* bt 0x8c61cbae */
L8c61cbaa: sa_motion_publish_stop(ctx,0x8c61cbacu); goto L8c61cdbc; /* bra 0x8c61cdbc */
L8c61cbae: sa_motion_publish_stop(ctx,0x8c61cbaeu); r[0]=sx(m,bus,&io_status,r[9]+0x2u,2);if(io_status)return io_status;goto L8c61cbb0; /* mov.w @(0x2,r9),r0 */
L8c61cbb0: sa_motion_publish_stop(ctx,0x8c61cbb0u); r[12]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;goto L8c61cbb2; /* mov.w @r9,r12 */
L8c61cbb2: sa_motion_publish_stop(ctx,0x8c61cbb2u); r[2]=rd(m,bus,&io_status,0x8c61cd9cu,4);if(io_status)return io_status;goto L8c61cbb4; /* mov.l 0x8c61cd9c,r2 */
L8c61cbb4: sa_motion_publish_stop(ctx,0x8c61cbb4u); r[14]=r[0]&65535u;goto L8c61cbb6; /* extu.w r0,r14 */
L8c61cbb6: sa_motion_publish_stop(ctx,0x8c61cbb6u); r[14]<<=2;goto L8c61cbb8; /* shll2 r14 */
L8c61cbb8: sa_motion_publish_stop(ctx,0x8c61cbb8u); r[0]=sx(m,bus,&io_status,r[9]+0x4u,2);if(io_status)return io_status;goto L8c61cbba; /* mov.w @(0x4,r9),r0 */
L8c61cbba: sa_motion_publish_stop(ctx,0x8c61cbbau); r[12]=r[12]&65535u;goto L8c61cbbc; /* extu.w r12,r12 */
L8c61cbbc: sa_motion_publish_stop(ctx,0x8c61cbbcu); r[4]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61cbbe; /* mov.l @r2,r4 */
L8c61cbbe: sa_motion_publish_stop(ctx,0x8c61cbbeu); r[12]<<=2;goto L8c61cbc0; /* shll2 r12 */
L8c61cbc0: sa_motion_publish_stop(ctx,0x8c61cbc0u); r[14]<<=2;goto L8c61cbc2; /* shll2 r14 */
L8c61cbc2: sa_motion_publish_stop(ctx,0x8c61cbc2u); r[13]=r[0]&65535u;goto L8c61cbc4; /* extu.w r0,r13 */
L8c61cbc4: sa_motion_publish_stop(ctx,0x8c61cbc4u); r[12]<<=2;goto L8c61cbc6; /* shll2 r12 */
L8c61cbc6: sa_motion_publish_stop(ctx,0x8c61cbc6u); r[0]=16u;goto L8c61cbc8; /* mov #0x10,r0 */
L8c61cbc8: sa_motion_publish_stop(ctx,0x8c61cbc8u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61cbca; /* shll r12 */
L8c61cbca: sa_motion_publish_stop(ctx,0x8c61cbcau); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61cbcc; /* shll r14 */
L8c61cbcc: sa_motion_publish_stop(ctx,0x8c61cbccu); r[14]+=r[4];goto L8c61cbce; /* add r4,r14 */
L8c61cbce: sa_motion_publish_stop(ctx,0x8c61cbceu); fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c61cbd0; /* fmov.s @(r0,r14),fr5 */
L8c61cbd0: sa_motion_publish_stop(ctx,0x8c61cbd0u); r[13]<<=2;goto L8c61cbd2; /* shll2 r13 */
L8c61cbd2: sa_motion_publish_stop(ctx,0x8c61cbd2u); r[12]+=r[4];goto L8c61cbd4; /* add r4,r12 */
L8c61cbd4: sa_motion_publish_stop(ctx,0x8c61cbd4u); r[13]<<=2;goto L8c61cbd6; /* shll2 r13 */
L8c61cbd6: sa_motion_publish_stop(ctx,0x8c61cbd6u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61cbd8; /* fmov.s @(r0,r12),fr3 */
L8c61cbd8: sa_motion_publish_stop(ctx,0x8c61cbd8u); fcopy(f,2u,5u);goto L8c61cbda; /* fmov fr5,fr2 */
L8c61cbda: sa_motion_publish_stop(ctx,0x8c61cbdau); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61cbdc; /* shll r13 */
L8c61cbdc: sa_motion_publish_stop(ctx,0x8c61cbdcu); r[0]=20u;goto L8c61cbde; /* mov #0x14,r0 */
L8c61cbde: sa_motion_publish_stop(ctx,0x8c61cbdeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61cbe0; /* fsub fr3,fr2 */
L8c61cbe0: sa_motion_publish_stop(ctx,0x8c61cbe0u); r[13]+=r[4];goto L8c61cbe2; /* add r4,r13 */
L8c61cbe2: sa_motion_publish_stop(ctx,0x8c61cbe2u); fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c61cbe4; /* fmov.s @(r0,r14),fr4 */
L8c61cbe4: sa_motion_publish_stop(ctx,0x8c61cbe4u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61cbe6; /* fmov.s @(r0,r13),fr3 */
L8c61cbe6: sa_motion_publish_stop(ctx,0x8c61cbe6u); fcopy(f,1u,4u);goto L8c61cbe8; /* fmov fr4,fr1 */
L8c61cbe8: sa_motion_publish_stop(ctx,0x8c61cbe8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61cbea; /* fsub fr4,fr3 */
L8c61cbea: sa_motion_publish_stop(ctx,0x8c61cbeau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61cbec; /* fmul fr3,fr2 */
L8c61cbec: sa_motion_publish_stop(ctx,0x8c61cbecu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61cbee; /* fmov.s @(r0,r12),fr3 */
L8c61cbee: sa_motion_publish_stop(ctx,0x8c61cbeeu); r[0]=16u;goto L8c61cbf0; /* mov #0x10,r0 */
L8c61cbf0: sa_motion_publish_stop(ctx,0x8c61cbf0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61cbf2; /* fsub fr3,fr1 */
L8c61cbf2: sa_motion_publish_stop(ctx,0x8c61cbf2u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61cbf4; /* fmov.s @(r0,r13),fr3 */
L8c61cbf4: sa_motion_publish_stop(ctx,0x8c61cbf4u); fcopy(f,4u,2u);goto L8c61cbf6; /* fmov fr2,fr4 */
L8c61cbf6: sa_motion_publish_stop(ctx,0x8c61cbf6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61cbf8; /* fsub fr5,fr3 */
L8c61cbf8: sa_motion_publish_stop(ctx,0x8c61cbf8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61cbfa; /* fmul fr3,fr1 */
L8c61cbfa: sa_motion_publish_stop(ctx,0x8c61cbfau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61cbfc; /* fldi0 fr3 */
L8c61cbfc: sa_motion_publish_stop(ctx,0x8c61cbfcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61cbfe; /* fsub fr1,fr4 */
L8c61cbfe: sa_motion_publish_stop(ctx,0x8c61cbfeu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61cc00; /* fcmp/gt fr3,fr4 */
L8c61cc00: sa_motion_publish_stop(ctx,0x8c61cc00u); if(!(*sr&1u))goto L8c61cc08;goto L8c61cc02; /* bf 0x8c61cc08 */
L8c61cc02: sa_motion_publish_stop(ctx,0x8c61cc02u); r[9]+=r[6];goto L8c61cc04; /* add r6,r9 */
L8c61cc04: sa_motion_publish_stop(ctx,0x8c61cc06u); r[10]+=r[7];goto L8c61cef4; /* bra 0x8c61cef4 */
L8c61cc08: sa_motion_publish_stop(ctx,0x8c61cc08u); r[0]=28u;goto L8c61cc0a; /* mov #0x1c,r0 */
L8c61cc0a: sa_motion_publish_stop(ctx,0x8c61cc0au); r[3]=rd(m,bus,&io_status,0x8c61cda0u,4);if(io_status)return io_status;goto L8c61cc0c; /* mov.l 0x8c61cda0,r3 */
L8c61cc0c: sa_motion_publish_stop(ctx,0x8c61cc0cu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61cc0e; /* mov.l @r15,r2 */
L8c61cc0e: sa_motion_publish_stop(ctx,0x8c61cc0eu); r[4]=r[15];goto L8c61cc10; /* mov r15,r4 */
L8c61cc10: sa_motion_publish_stop(ctx,0x8c61cc10u); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61cc12; /* mov.l @r3,r1 */
L8c61cc12: sa_motion_publish_stop(ctx,0x8c61cc12u); r[1]+=r[2];goto L8c61cc14; /* add r2,r1 */
L8c61cc14: sa_motion_publish_stop(ctx,0x8c61cc14u); wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c61cc16; /* mov.l r1,@r3 */
L8c61cc16: sa_motion_publish_stop(ctx,0x8c61cc16u); fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61cc18; /* fmov.s @r12,fr2 */
L8c61cc18: sa_motion_publish_stop(ctx,0x8c61cc18u); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61cc1a; /* fmov.s @r14,fr3 */
L8c61cc1a: sa_motion_publish_stop(ctx,0x8c61cc1au); r[3]=rd(m,bus,&io_status,0x8c61cda4u,4);if(io_status)return io_status;goto L8c61cc1c; /* mov.l 0x8c61cda4,r3 */
L8c61cc1c: sa_motion_publish_stop(ctx,0x8c61cc1cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cc1e; /* fadd fr3,fr2 */
L8c61cc1e: sa_motion_publish_stop(ctx,0x8c61cc1eu); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61cc20; /* fmov.s @r13,fr3 */
L8c61cc20: sa_motion_publish_stop(ctx,0x8c61cc20u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cc22; /* fadd fr3,fr2 */
L8c61cc22: sa_motion_publish_stop(ctx,0x8c61cc22u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cc24; /* fmov fr2,@(r0,r15) */
L8c61cc24: sa_motion_publish_stop(ctx,0x8c61cc24u); r[0]=4u;goto L8c61cc26; /* mov #0x4,r0 */
L8c61cc26: sa_motion_publish_stop(ctx,0x8c61cc26u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61cc28; /* fmov.s @(r0,r12),fr2 */
L8c61cc28: sa_motion_publish_stop(ctx,0x8c61cc28u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61cc2a; /* fmov.s @(r0,r14),fr3 */
L8c61cc2a: sa_motion_publish_stop(ctx,0x8c61cc2au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cc2c; /* fadd fr3,fr2 */
L8c61cc2c: sa_motion_publish_stop(ctx,0x8c61cc2cu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61cc2e; /* fmov.s @(r0,r13),fr3 */
L8c61cc2e: sa_motion_publish_stop(ctx,0x8c61cc2eu); r[0]=32u;goto L8c61cc30; /* mov #0x20,r0 */
L8c61cc30: sa_motion_publish_stop(ctx,0x8c61cc30u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cc32; /* fadd fr3,fr2 */
L8c61cc32: sa_motion_publish_stop(ctx,0x8c61cc32u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cc34; /* fmov fr2,@(r0,r15) */
L8c61cc34: sa_motion_publish_stop(ctx,0x8c61cc34u); r[0]=8u;goto L8c61cc36; /* mov #0x8,r0 */
L8c61cc36: sa_motion_publish_stop(ctx,0x8c61cc36u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61cc38; /* fmov.s @(r0,r14),fr3 */
L8c61cc38: sa_motion_publish_stop(ctx,0x8c61cc38u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61cc3a; /* fmov.s @(r0,r12),fr2 */
L8c61cc3a: sa_motion_publish_stop(ctx,0x8c61cc3au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cc3c; /* fadd fr3,fr2 */
L8c61cc3c: sa_motion_publish_stop(ctx,0x8c61cc3cu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61cc3e; /* fmov.s @(r0,r13),fr3 */
L8c61cc3e: sa_motion_publish_stop(ctx,0x8c61cc3eu); r[0]=36u;goto L8c61cc40; /* mov #0x24,r0 */
L8c61cc40: sa_motion_publish_stop(ctx,0x8c61cc40u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cc42; /* fadd fr3,fr2 */
L8c61cc42: sa_motion_publish_stop(ctx,0x8c61cc42u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cc44; /* fmov fr2,@(r0,r15) */
L8c61cc44: sa_motion_publish_stop(ctx,0x8c61cc46u); target=r[3];*pr=0x8c61cc48u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61cc48; /* jsr @r3 */
L8c61cc48: sa_motion_publish_stop(ctx,0x8c61cc48u); r[3]=rd(m,bus,&io_status,0x8c61cda8u,4);if(io_status)return io_status;goto L8c61cc4a; /* mov.l 0x8c61cda8,r3 */
L8c61cc4a: sa_motion_publish_stop(ctx,0x8c61cc4au); r[0]=28u;goto L8c61cc4c; /* mov #0x1c,r0 */
L8c61cc4c: sa_motion_publish_stop(ctx,0x8c61cc4cu); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cc4e; /* fmov.s @(r0,r15),fr3 */
L8c61cc4e: sa_motion_publish_stop(ctx,0x8c61cc4eu); r[0]=32u;goto L8c61cc50; /* mov #0x20,r0 */
L8c61cc50: sa_motion_publish_stop(ctx,0x8c61cc50u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61cc52; /* fmov.s @r3,fr2 */
L8c61cc52: sa_motion_publish_stop(ctx,0x8c61cc52u); r[3]+=8u;goto L8c61cc54; /* add #0x8,r3 */
L8c61cc54: sa_motion_publish_stop(ctx,0x8c61cc54u); r[2]=rd(m,bus,&io_status,0x8c61cdacu,4);if(io_status)return io_status;goto L8c61cc56; /* mov.l 0x8c61cdac,r2 */
L8c61cc56: sa_motion_publish_stop(ctx,0x8c61cc56u); r[8]+=24u;goto L8c61cc58; /* add #0x18,r8 */
L8c61cc58: sa_motion_publish_stop(ctx,0x8c61cc58u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61cc5a; /* fmul fr3,fr2 */
L8c61cc5a: sa_motion_publish_stop(ctx,0x8c61cc5au); r[1]=rd(m,bus,&io_status,0x8c61cdb0u,4);if(io_status)return io_status;goto L8c61cc5c; /* mov.l 0x8c61cdb0,r1 */
L8c61cc5c: sa_motion_publish_stop(ctx,0x8c61cc5cu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61cc5e; /* fmov.s @r2,fr3 */
L8c61cc5e: sa_motion_publish_stop(ctx,0x8c61cc5eu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61cc60; /* fmov.s @r1,fr1 */
L8c61cc60: sa_motion_publish_stop(ctx,0x8c61cc60u); r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61cc62; /* mov.w @r9+,r4 */
L8c61cc62: sa_motion_publish_stop(ctx,0x8c61cc62u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61cc64; /* fsub fr2,fr3 */
L8c61cc64: sa_motion_publish_stop(ctx,0x8c61cc64u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cc66; /* fmov.s @(r0,r15),fr2 */
L8c61cc66: sa_motion_publish_stop(ctx,0x8c61cc66u); r[0]=36u;goto L8c61cc68; /* mov #0x24,r0 */
L8c61cc68: sa_motion_publish_stop(ctx,0x8c61cc68u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61cc6a; /* fmul fr2,fr1 */
L8c61cc6a: sa_motion_publish_stop(ctx,0x8c61cc6au); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cc6c; /* fmov.s @(r0,r15),fr2 */
L8c61cc6c: sa_motion_publish_stop(ctx,0x8c61cc6cu); r[0]=rd(m,bus,&io_status,0x8c61cd9cu,4);if(io_status)return io_status;goto L8c61cc6e; /* mov.l 0x8c61cd9c,r0 */
L8c61cc6e: sa_motion_publish_stop(ctx,0x8c61cc6eu); r[4]=r[4]&65535u;goto L8c61cc70; /* extu.w r4,r4 */
L8c61cc70: sa_motion_publish_stop(ctx,0x8c61cc70u); r[4]<<=2;goto L8c61cc72; /* shll2 r4 */
L8c61cc72: sa_motion_publish_stop(ctx,0x8c61cc72u); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61cc74; /* mov.l @r0,r2 */
L8c61cc74: sa_motion_publish_stop(ctx,0x8c61cc74u); r[4]<<=2;goto L8c61cc76; /* shll2 r4 */
L8c61cc76: sa_motion_publish_stop(ctx,0x8c61cc76u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61cc78; /* fsub fr1,fr3 */
L8c61cc78: sa_motion_publish_stop(ctx,0x8c61cc78u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61cc7a; /* fmov.s @r3,fr1 */
L8c61cc7a: sa_motion_publish_stop(ctx,0x8c61cc7au); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61cc7c; /* mov.w @(0x2,r10),r0 */
L8c61cc7c: sa_motion_publish_stop(ctx,0x8c61cc7cu); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61cc7e; /* shll r4 */
L8c61cc7e: sa_motion_publish_stop(ctx,0x8c61cc7eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61cc80; /* fmul fr2,fr1 */
L8c61cc80: sa_motion_publish_stop(ctx,0x8c61cc80u); r[4]+=r[2];goto L8c61cc82; /* add r2,r4 */
L8c61cc82: sa_motion_publish_stop(ctx,0x8c61cc82u); r[3]=r[0];goto L8c61cc84; /* mov r0,r3 */
L8c61cc84: sa_motion_publish_stop(ctx,0x8c61cc84u); fcopy(f,4u,3u);goto L8c61cc86; /* fmov fr3,fr4 */
L8c61cc86: sa_motion_publish_stop(ctx,0x8c61cc86u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61cc88; /* fsub fr1,fr4 */
L8c61cc88: sa_motion_publish_stop(ctx,0x8c61cc88u); *fpul=r[3];goto L8c61cc8a; /* lds r3,FPUL */
L8c61cc8a: sa_motion_publish_stop(ctx,0x8c61cc8au); r[0]=24u;goto L8c61cc8c; /* mov #0x18,r0 */
L8c61cc8c: sa_motion_publish_stop(ctx,0x8c61cc8cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cc8e; /* float FPUL,fr3 */
L8c61cc8e: sa_motion_publish_stop(ctx,0x8c61cc8eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61cc90; /* fmul fr14,fr3 */
L8c61cc90: sa_motion_publish_stop(ctx,0x8c61cc90u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cc92; /* fmov.s fr3,@-r8 */
L8c61cc92: sa_motion_publish_stop(ctx,0x8c61cc92u); r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61cc94; /* mov.w @r10,r3 */
L8c61cc94: sa_motion_publish_stop(ctx,0x8c61cc94u); r[10]+=4u;goto L8c61cc96; /* add #0x4,r10 */
L8c61cc96: sa_motion_publish_stop(ctx,0x8c61cc96u); *fpul=r[3];goto L8c61cc98; /* lds r3,FPUL */
L8c61cc98: sa_motion_publish_stop(ctx,0x8c61cc98u); r[3]=rd(m,bus,&io_status,0x8c61cdb4u,4);if(io_status)return io_status;goto L8c61cc9a; /* mov.l 0x8c61cdb4,r3 */
L8c61cc9a: sa_motion_publish_stop(ctx,0x8c61cc9au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cc9c; /* float FPUL,fr3 */
L8c61cc9c: sa_motion_publish_stop(ctx,0x8c61cc9cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61cc9e; /* fmul fr14,fr3 */
L8c61cc9e: sa_motion_publish_stop(ctx,0x8c61cc9eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cca0; /* fmov.s fr3,@-r8 */
L8c61cca0: sa_motion_publish_stop(ctx,0x8c61cca0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cca2; /* fmov.s @(r0,r4),fr3 */
L8c61cca2: sa_motion_publish_stop(ctx,0x8c61cca2u); r[0]=20u;goto L8c61cca4; /* mov #0x14,r0 */
L8c61cca4: sa_motion_publish_stop(ctx,0x8c61cca4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cca6; /* fmov.s fr3,@-r8 */
L8c61cca6: sa_motion_publish_stop(ctx,0x8c61cca6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cca8; /* fmov.s @(r0,r4),fr3 */
L8c61cca8: sa_motion_publish_stop(ctx,0x8c61cca8u); r[0]=16u;goto L8c61ccaa; /* mov #0x10,r0 */
L8c61ccaa: sa_motion_publish_stop(ctx,0x8c61ccaau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ccac; /* fmov.s fr3,@-r8 */
L8c61ccac: sa_motion_publish_stop(ctx,0x8c61ccacu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ccae; /* fmov.s @(r0,r4),fr3 */
L8c61ccae: sa_motion_publish_stop(ctx,0x8c61ccaeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ccb0; /* fmov.s fr3,@-r8 */
L8c61ccb0: sa_motion_publish_stop(ctx,0x8c61ccb0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61ccb2; /* mov.l r3,@-r8 */
L8c61ccb2: sa_motion_publish_stop(ctx,0x8c61ccb2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61ccb4; /* pref @r8 */
L8c61ccb4: sa_motion_publish_stop(ctx,0x8c61ccb4u); r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61ccb6; /* mov.w @r9+,r4 */
L8c61ccb6: sa_motion_publish_stop(ctx,0x8c61ccb6u); r[8]+=56u;goto L8c61ccb8; /* add #0x38,r8 */
L8c61ccb8: sa_motion_publish_stop(ctx,0x8c61ccb8u); r[2]=rd(m,bus,&io_status,0x8c61cd9cu,4);if(io_status)return io_status;goto L8c61ccba; /* mov.l 0x8c61cd9c,r2 */
L8c61ccba: sa_motion_publish_stop(ctx,0x8c61ccbau); r[4]=r[4]&65535u;goto L8c61ccbc; /* extu.w r4,r4 */
L8c61ccbc: sa_motion_publish_stop(ctx,0x8c61ccbcu); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ccbe; /* mov.w @(0x2,r10),r0 */
L8c61ccbe: sa_motion_publish_stop(ctx,0x8c61ccbeu); r[4]<<=2;goto L8c61ccc0; /* shll2 r4 */
L8c61ccc0: sa_motion_publish_stop(ctx,0x8c61ccc0u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ccc2; /* mov.l @r2,r3 */
L8c61ccc2: sa_motion_publish_stop(ctx,0x8c61ccc2u); r[4]<<=2;goto L8c61ccc4; /* shll2 r4 */
L8c61ccc4: sa_motion_publish_stop(ctx,0x8c61ccc4u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61ccc6; /* shll r4 */
L8c61ccc6: sa_motion_publish_stop(ctx,0x8c61ccc6u); r[4]+=r[3];goto L8c61ccc8; /* add r3,r4 */
L8c61ccc8: sa_motion_publish_stop(ctx,0x8c61ccc8u); r[3]=r[0];goto L8c61ccca; /* mov r0,r3 */
L8c61ccca: sa_motion_publish_stop(ctx,0x8c61cccau); *fpul=r[3];goto L8c61cccc; /* lds r3,FPUL */
L8c61cccc: sa_motion_publish_stop(ctx,0x8c61ccccu); r[0]=24u;goto L8c61ccce; /* mov #0x18,r0 */
L8c61ccce: sa_motion_publish_stop(ctx,0x8c61ccceu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ccd0; /* float FPUL,fr3 */
L8c61ccd0: sa_motion_publish_stop(ctx,0x8c61ccd0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ccd2; /* fmul fr14,fr3 */
L8c61ccd2: sa_motion_publish_stop(ctx,0x8c61ccd2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ccd4; /* fmov.s fr3,@-r8 */
L8c61ccd4: sa_motion_publish_stop(ctx,0x8c61ccd4u); r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ccd6; /* mov.w @r10,r3 */
L8c61ccd6: sa_motion_publish_stop(ctx,0x8c61ccd6u); r[10]+=4u;goto L8c61ccd8; /* add #0x4,r10 */
L8c61ccd8: sa_motion_publish_stop(ctx,0x8c61ccd8u); *fpul=r[3];goto L8c61ccda; /* lds r3,FPUL */
L8c61ccda: sa_motion_publish_stop(ctx,0x8c61ccdau); r[3]=rd(m,bus,&io_status,0x8c61cdb4u,4);if(io_status)return io_status;goto L8c61ccdc; /* mov.l 0x8c61cdb4,r3 */
L8c61ccdc: sa_motion_publish_stop(ctx,0x8c61ccdcu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ccde; /* float FPUL,fr3 */
L8c61ccde: sa_motion_publish_stop(ctx,0x8c61ccdeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61cce0; /* fmul fr14,fr3 */
L8c61cce0: sa_motion_publish_stop(ctx,0x8c61cce0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cce2; /* fmov.s fr3,@-r8 */
L8c61cce2: sa_motion_publish_stop(ctx,0x8c61cce2u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cce4; /* fmov.s @(r0,r4),fr3 */
L8c61cce4: sa_motion_publish_stop(ctx,0x8c61cce4u); r[0]=20u;goto L8c61cce6; /* mov #0x14,r0 */
L8c61cce6: sa_motion_publish_stop(ctx,0x8c61cce6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cce8; /* fmov.s fr3,@-r8 */
L8c61cce8: sa_motion_publish_stop(ctx,0x8c61cce8u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ccea; /* fmov.s @(r0,r4),fr3 */
L8c61ccea: sa_motion_publish_stop(ctx,0x8c61cceau); r[0]=16u;goto L8c61ccec; /* mov #0x10,r0 */
L8c61ccec: sa_motion_publish_stop(ctx,0x8c61ccecu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ccee; /* fmov.s fr3,@-r8 */
L8c61ccee: sa_motion_publish_stop(ctx,0x8c61cceeu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ccf0; /* fmov.s @(r0,r4),fr3 */
L8c61ccf0: sa_motion_publish_stop(ctx,0x8c61ccf0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ccf2; /* fmov.s fr3,@-r8 */
L8c61ccf2: sa_motion_publish_stop(ctx,0x8c61ccf2u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61ccf4; /* mov.l r3,@-r8 */
L8c61ccf4: sa_motion_publish_stop(ctx,0x8c61ccf4u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61ccf6; /* pref @r8 */
L8c61ccf6: sa_motion_publish_stop(ctx,0x8c61ccf6u); r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ccf8; /* mov.l @r15,r6 */
L8c61ccf8: sa_motion_publish_stop(ctx,0x8c61ccf8u); r[5]=2u;goto L8c61ccfa; /* mov #0x2,r5 */
L8c61ccfa: sa_motion_publish_stop(ctx,0x8c61ccfau); r[6]+=4294967295u;goto L8c61ccfc; /* add #-0x1,r6 */
L8c61ccfc: sa_motion_publish_stop(ctx,0x8c61ccfcu); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61ccfe; /* cmp/ge r6,r5 */
L8c61ccfe: sa_motion_publish_stop(ctx,0x8c61cd00u); take=(*sr&1u);r[8]+=32u;if(take)goto L8c61cd4e;goto L8c61cd02; /* bt/s 0x8c61cd4e */
L8c61cd02: sa_motion_publish_stop(ctx,0x8c61cd02u); r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61cd04; /* mov.w @r9+,r4 */
L8c61cd04: sa_motion_publish_stop(ctx,0x8c61cd04u); r[8]+=28u;goto L8c61cd06; /* add #0x1c,r8 */
L8c61cd06: sa_motion_publish_stop(ctx,0x8c61cd06u); r[2]=rd(m,bus,&io_status,0x8c61cd9cu,4);if(io_status)return io_status;goto L8c61cd08; /* mov.l 0x8c61cd9c,r2 */
L8c61cd08: sa_motion_publish_stop(ctx,0x8c61cd08u); r[4]=r[4]&65535u;goto L8c61cd0a; /* extu.w r4,r4 */
L8c61cd0a: sa_motion_publish_stop(ctx,0x8c61cd0au); r[4]<<=2;goto L8c61cd0c; /* shll2 r4 */
L8c61cd0c: sa_motion_publish_stop(ctx,0x8c61cd0cu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61cd0e; /* mov.l @r2,r3 */
L8c61cd0e: sa_motion_publish_stop(ctx,0x8c61cd0eu); r[4]<<=2;goto L8c61cd10; /* shll2 r4 */
L8c61cd10: sa_motion_publish_stop(ctx,0x8c61cd10u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61cd12; /* fmov.s fr4,@-r8 */
L8c61cd12: sa_motion_publish_stop(ctx,0x8c61cd12u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61cd14; /* mov.w @(0x2,r10),r0 */
L8c61cd14: sa_motion_publish_stop(ctx,0x8c61cd14u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61cd16; /* shll r4 */
L8c61cd16: sa_motion_publish_stop(ctx,0x8c61cd16u); r[4]+=r[3];goto L8c61cd18; /* add r3,r4 */
L8c61cd18: sa_motion_publish_stop(ctx,0x8c61cd18u); r[3]=r[0];goto L8c61cd1a; /* mov r0,r3 */
L8c61cd1a: sa_motion_publish_stop(ctx,0x8c61cd1au); *fpul=r[3];goto L8c61cd1c; /* lds r3,FPUL */
L8c61cd1c: sa_motion_publish_stop(ctx,0x8c61cd1cu); r[0]=24u;goto L8c61cd1e; /* mov #0x18,r0 */
L8c61cd1e: sa_motion_publish_stop(ctx,0x8c61cd1eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cd20; /* float FPUL,fr3 */
L8c61cd20: sa_motion_publish_stop(ctx,0x8c61cd20u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61cd22; /* fmul fr14,fr3 */
L8c61cd22: sa_motion_publish_stop(ctx,0x8c61cd22u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd24; /* fmov.s fr3,@-r8 */
L8c61cd24: sa_motion_publish_stop(ctx,0x8c61cd24u); r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61cd26; /* mov.w @r10,r3 */
L8c61cd26: sa_motion_publish_stop(ctx,0x8c61cd26u); r[10]+=4u;goto L8c61cd28; /* add #0x4,r10 */
L8c61cd28: sa_motion_publish_stop(ctx,0x8c61cd28u); *fpul=r[3];goto L8c61cd2a; /* lds r3,FPUL */
L8c61cd2a: sa_motion_publish_stop(ctx,0x8c61cd2au); r[3]=rd(m,bus,&io_status,0x8c61cdb4u,4);if(io_status)return io_status;goto L8c61cd2c; /* mov.l 0x8c61cdb4,r3 */
L8c61cd2c: sa_motion_publish_stop(ctx,0x8c61cd2cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cd2e; /* float FPUL,fr3 */
L8c61cd2e: sa_motion_publish_stop(ctx,0x8c61cd2eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61cd30; /* fmul fr14,fr3 */
L8c61cd30: sa_motion_publish_stop(ctx,0x8c61cd30u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd32; /* fmov.s fr3,@-r8 */
L8c61cd32: sa_motion_publish_stop(ctx,0x8c61cd32u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cd34; /* fmov.s @(r0,r4),fr3 */
L8c61cd34: sa_motion_publish_stop(ctx,0x8c61cd34u); r[0]=20u;goto L8c61cd36; /* mov #0x14,r0 */
L8c61cd36: sa_motion_publish_stop(ctx,0x8c61cd36u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd38; /* fmov.s fr3,@-r8 */
L8c61cd38: sa_motion_publish_stop(ctx,0x8c61cd38u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cd3a; /* fmov.s @(r0,r4),fr3 */
L8c61cd3a: sa_motion_publish_stop(ctx,0x8c61cd3au); r[0]=16u;goto L8c61cd3c; /* mov #0x10,r0 */
L8c61cd3c: sa_motion_publish_stop(ctx,0x8c61cd3cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd3e; /* fmov.s fr3,@-r8 */
L8c61cd3e: sa_motion_publish_stop(ctx,0x8c61cd3eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cd40; /* fmov.s @(r0,r4),fr3 */
L8c61cd40: sa_motion_publish_stop(ctx,0x8c61cd40u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd42; /* fmov.s fr3,@-r8 */
L8c61cd42: sa_motion_publish_stop(ctx,0x8c61cd42u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61cd44; /* mov.l r3,@-r8 */
L8c61cd44: sa_motion_publish_stop(ctx,0x8c61cd44u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61cd46; /* pref @r8 */
L8c61cd46: sa_motion_publish_stop(ctx,0x8c61cd46u); r[5]+=1u;goto L8c61cd48; /* add #0x1,r5 */
L8c61cd48: sa_motion_publish_stop(ctx,0x8c61cd48u); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61cd4a; /* cmp/ge r6,r5 */
L8c61cd4a: sa_motion_publish_stop(ctx,0x8c61cd4cu); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61cd02;goto L8c61cd4e; /* bf/s 0x8c61cd02 */
L8c61cd4e: sa_motion_publish_stop(ctx,0x8c61cd4eu); r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61cd50; /* mov.w @r9+,r4 */
L8c61cd50: sa_motion_publish_stop(ctx,0x8c61cd50u); r[8]+=28u;goto L8c61cd52; /* add #0x1c,r8 */
L8c61cd52: sa_motion_publish_stop(ctx,0x8c61cd52u); r[2]=rd(m,bus,&io_status,0x8c61cd9cu,4);if(io_status)return io_status;goto L8c61cd54; /* mov.l 0x8c61cd9c,r2 */
L8c61cd54: sa_motion_publish_stop(ctx,0x8c61cd54u); r[4]=r[4]&65535u;goto L8c61cd56; /* extu.w r4,r4 */
L8c61cd56: sa_motion_publish_stop(ctx,0x8c61cd56u); r[4]<<=2;goto L8c61cd58; /* shll2 r4 */
L8c61cd58: sa_motion_publish_stop(ctx,0x8c61cd58u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61cd5a; /* mov.l @r2,r3 */
L8c61cd5a: sa_motion_publish_stop(ctx,0x8c61cd5au); r[4]<<=2;goto L8c61cd5c; /* shll2 r4 */
L8c61cd5c: sa_motion_publish_stop(ctx,0x8c61cd5cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61cd5e; /* fmov.s fr4,@-r8 */
L8c61cd5e: sa_motion_publish_stop(ctx,0x8c61cd5eu); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61cd60; /* mov.w @(0x2,r10),r0 */
L8c61cd60: sa_motion_publish_stop(ctx,0x8c61cd60u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61cd62; /* shll r4 */
L8c61cd62: sa_motion_publish_stop(ctx,0x8c61cd62u); r[4]+=r[3];goto L8c61cd64; /* add r3,r4 */
L8c61cd64: sa_motion_publish_stop(ctx,0x8c61cd64u); r[3]=r[0];goto L8c61cd66; /* mov r0,r3 */
L8c61cd66: sa_motion_publish_stop(ctx,0x8c61cd66u); *fpul=r[3];goto L8c61cd68; /* lds r3,FPUL */
L8c61cd68: sa_motion_publish_stop(ctx,0x8c61cd68u); r[0]=24u;goto L8c61cd6a; /* mov #0x18,r0 */
L8c61cd6a: sa_motion_publish_stop(ctx,0x8c61cd6au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cd6c; /* float FPUL,fr3 */
L8c61cd6c: sa_motion_publish_stop(ctx,0x8c61cd6cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61cd6e; /* fmul fr14,fr3 */
L8c61cd6e: sa_motion_publish_stop(ctx,0x8c61cd6eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd70; /* fmov.s fr3,@-r8 */
L8c61cd70: sa_motion_publish_stop(ctx,0x8c61cd70u); r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61cd72; /* mov.w @r10,r3 */
L8c61cd72: sa_motion_publish_stop(ctx,0x8c61cd72u); r[10]+=4u;goto L8c61cd74; /* add #0x4,r10 */
L8c61cd74: sa_motion_publish_stop(ctx,0x8c61cd74u); *fpul=r[3];goto L8c61cd76; /* lds r3,FPUL */
L8c61cd76: sa_motion_publish_stop(ctx,0x8c61cd76u); r[3]=rd(m,bus,&io_status,0x8c61cdb8u,4);if(io_status)return io_status;goto L8c61cd78; /* mov.l 0x8c61cdb8,r3 */
L8c61cd78: sa_motion_publish_stop(ctx,0x8c61cd78u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cd7a; /* float FPUL,fr3 */
L8c61cd7a: sa_motion_publish_stop(ctx,0x8c61cd7au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61cd7c; /* fmul fr14,fr3 */
L8c61cd7c: sa_motion_publish_stop(ctx,0x8c61cd7cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd7e; /* fmov.s fr3,@-r8 */
L8c61cd7e: sa_motion_publish_stop(ctx,0x8c61cd7eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cd80; /* fmov.s @(r0,r4),fr3 */
L8c61cd80: sa_motion_publish_stop(ctx,0x8c61cd80u); r[0]=20u;goto L8c61cd82; /* mov #0x14,r0 */
L8c61cd82: sa_motion_publish_stop(ctx,0x8c61cd82u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd84; /* fmov.s fr3,@-r8 */
L8c61cd84: sa_motion_publish_stop(ctx,0x8c61cd84u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cd86; /* fmov.s @(r0,r4),fr3 */
L8c61cd86: sa_motion_publish_stop(ctx,0x8c61cd86u); r[0]=16u;goto L8c61cd88; /* mov #0x10,r0 */
L8c61cd88: sa_motion_publish_stop(ctx,0x8c61cd88u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd8a; /* fmov.s fr3,@-r8 */
L8c61cd8a: sa_motion_publish_stop(ctx,0x8c61cd8au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cd8c; /* fmov.s @(r0,r4),fr3 */
L8c61cd8c: sa_motion_publish_stop(ctx,0x8c61cd8cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd8e; /* fmov.s fr3,@-r8 */
L8c61cd8e: sa_motion_publish_stop(ctx,0x8c61cd8eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61cd90; /* mov.l r3,@-r8 */
L8c61cd90: sa_motion_publish_stop(ctx,0x8c61cd90u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61cd92; /* pref @r8 */
L8c61cd92: sa_motion_publish_stop(ctx,0x8c61cd94u); r[8]+=32u;goto L8c61cef4; /* bra 0x8c61cef4 */
L8c61cdbc: sa_motion_publish_stop(ctx,0x8c61cdbcu); r[0]=28u;goto L8c61cdbe; /* mov #0x1c,r0 */
L8c61cdbe: sa_motion_publish_stop(ctx,0x8c61cdbeu); r[1]=rd(m,bus,&io_status,0x8c61cf74u,4);if(io_status)return io_status;goto L8c61cdc0; /* mov.l 0x8c61cf74,r1 */
L8c61cdc0: sa_motion_publish_stop(ctx,0x8c61cdc0u); r[2]=0u;goto L8c61cdc2; /* mov #0x0,r2 */
L8c61cdc2: sa_motion_publish_stop(ctx,0x8c61cdc2u); r[4]=r[15];goto L8c61cdc4; /* mov r15,r4 */
L8c61cdc4: sa_motion_publish_stop(ctx,0x8c61cdc4u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61cdc6; /* mov.l r2,@r1 */
L8c61cdc6: sa_motion_publish_stop(ctx,0x8c61cdc6u); if((r[14]&0x1f000000u)!=0x0c000000u)return -3;fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61cdc8; /* fmov.s @r14,fr3 */
L8c61cdc8: sa_motion_publish_stop(ctx,0x8c61cdc8u); fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61cdca; /* fmov.s @r12,fr2 */
L8c61cdca: sa_motion_publish_stop(ctx,0x8c61cdcau); r[2]=rd(m,bus,&io_status,0x8c61cf78u,4);if(io_status)return io_status;goto L8c61cdcc; /* mov.l 0x8c61cf78,r2 */
L8c61cdcc: sa_motion_publish_stop(ctx,0x8c61cdccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cdce; /* fadd fr3,fr2 */
L8c61cdce: sa_motion_publish_stop(ctx,0x8c61cdceu); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61cdd0; /* fmov.s @r13,fr3 */
L8c61cdd0: sa_motion_publish_stop(ctx,0x8c61cdd0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cdd2; /* fadd fr3,fr2 */
L8c61cdd2: sa_motion_publish_stop(ctx,0x8c61cdd2u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cdd4; /* fmov fr2,@(r0,r15) */
L8c61cdd4: sa_motion_publish_stop(ctx,0x8c61cdd4u); r[0]=4u;goto L8c61cdd6; /* mov #0x4,r0 */
L8c61cdd6: sa_motion_publish_stop(ctx,0x8c61cdd6u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61cdd8; /* fmov.s @(r0,r14),fr3 */
L8c61cdd8: sa_motion_publish_stop(ctx,0x8c61cdd8u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61cdda; /* fmov.s @(r0,r12),fr2 */
L8c61cdda: sa_motion_publish_stop(ctx,0x8c61cddau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cddc; /* fadd fr3,fr2 */
L8c61cddc: sa_motion_publish_stop(ctx,0x8c61cddcu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61cdde; /* fmov.s @(r0,r13),fr3 */
L8c61cdde: sa_motion_publish_stop(ctx,0x8c61cddeu); r[0]=32u;goto L8c61cde0; /* mov #0x20,r0 */
L8c61cde0: sa_motion_publish_stop(ctx,0x8c61cde0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cde2; /* fadd fr3,fr2 */
L8c61cde2: sa_motion_publish_stop(ctx,0x8c61cde2u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cde4; /* fmov fr2,@(r0,r15) */
L8c61cde4: sa_motion_publish_stop(ctx,0x8c61cde4u); r[0]=8u;goto L8c61cde6; /* mov #0x8,r0 */
L8c61cde6: sa_motion_publish_stop(ctx,0x8c61cde6u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61cde8; /* fmov.s @(r0,r14),fr3 */
L8c61cde8: sa_motion_publish_stop(ctx,0x8c61cde8u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61cdea; /* fmov.s @(r0,r12),fr2 */
L8c61cdea: sa_motion_publish_stop(ctx,0x8c61cdeau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cdec; /* fadd fr3,fr2 */
L8c61cdec: sa_motion_publish_stop(ctx,0x8c61cdecu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61cdee; /* fmov.s @(r0,r13),fr3 */
L8c61cdee: sa_motion_publish_stop(ctx,0x8c61cdeeu); r[0]=36u;goto L8c61cdf0; /* mov #0x24,r0 */
L8c61cdf0: sa_motion_publish_stop(ctx,0x8c61cdf0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cdf2; /* fadd fr3,fr2 */
L8c61cdf2: sa_motion_publish_stop(ctx,0x8c61cdf2u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cdf4; /* fmov fr2,@(r0,r15) */
L8c61cdf4: sa_motion_publish_stop(ctx,0x8c61cdf6u); target=r[2];*pr=0x8c61cdf8u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61cdf8; /* jsr @r2 */
L8c61cdf8: sa_motion_publish_stop(ctx,0x8c61cdf8u); r[3]=rd(m,bus,&io_status,0x8c61cf7cu,4);if(io_status)return io_status;goto L8c61cdfa; /* mov.l 0x8c61cf7c,r3 */
L8c61cdfa: sa_motion_publish_stop(ctx,0x8c61cdfau); r[0]=28u;goto L8c61cdfc; /* mov #0x1c,r0 */
L8c61cdfc: sa_motion_publish_stop(ctx,0x8c61cdfcu); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cdfe; /* fmov.s @(r0,r15),fr3 */
L8c61cdfe: sa_motion_publish_stop(ctx,0x8c61cdfeu); r[0]=32u;goto L8c61ce00; /* mov #0x20,r0 */
L8c61ce00: sa_motion_publish_stop(ctx,0x8c61ce00u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61ce02; /* fmov.s @r3,fr2 */
L8c61ce02: sa_motion_publish_stop(ctx,0x8c61ce02u); r[3]+=8u;goto L8c61ce04; /* add #0x8,r3 */
L8c61ce04: sa_motion_publish_stop(ctx,0x8c61ce04u); r[2]=rd(m,bus,&io_status,0x8c61cf80u,4);if(io_status)return io_status;goto L8c61ce06; /* mov.l 0x8c61cf80,r2 */
L8c61ce06: sa_motion_publish_stop(ctx,0x8c61ce06u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ce08; /* fmul fr3,fr2 */
L8c61ce08: sa_motion_publish_stop(ctx,0x8c61ce08u); r[1]=rd(m,bus,&io_status,0x8c61cf84u,4);if(io_status)return io_status;goto L8c61ce0a; /* mov.l 0x8c61cf84,r1 */
L8c61ce0a: sa_motion_publish_stop(ctx,0x8c61ce0au); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61ce0c; /* fmov.s @r2,fr3 */
L8c61ce0c: sa_motion_publish_stop(ctx,0x8c61ce0cu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61ce0e; /* fmov.s @r1,fr1 */
L8c61ce0e: sa_motion_publish_stop(ctx,0x8c61ce0eu); r[12]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61ce10; /* mov.w @r9+,r12 */
L8c61ce10: sa_motion_publish_stop(ctx,0x8c61ce10u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ce12; /* fsub fr2,fr3 */
L8c61ce12: sa_motion_publish_stop(ctx,0x8c61ce12u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ce14; /* fmov.s @(r0,r15),fr2 */
L8c61ce14: sa_motion_publish_stop(ctx,0x8c61ce14u); r[0]=36u;goto L8c61ce16; /* mov #0x24,r0 */
L8c61ce16: sa_motion_publish_stop(ctx,0x8c61ce16u); r[14]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61ce18; /* mov.w @r9+,r14 */
L8c61ce18: sa_motion_publish_stop(ctx,0x8c61ce18u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ce1a; /* fmul fr2,fr1 */
L8c61ce1a: sa_motion_publish_stop(ctx,0x8c61ce1au); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ce1c; /* fmov.s @(r0,r15),fr2 */
L8c61ce1c: sa_motion_publish_stop(ctx,0x8c61ce1cu); r[12]=r[12]&65535u;goto L8c61ce1e; /* extu.w r12,r12 */
L8c61ce1e: sa_motion_publish_stop(ctx,0x8c61ce1eu); r[0]=rd(m,bus,&io_status,0x8c61cf88u,4);if(io_status)return io_status;goto L8c61ce20; /* mov.l 0x8c61cf88,r0 */
L8c61ce20: sa_motion_publish_stop(ctx,0x8c61ce20u); r[12]<<=2;goto L8c61ce22; /* shll2 r12 */
L8c61ce22: sa_motion_publish_stop(ctx,0x8c61ce22u); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61ce24; /* mov.l @r0,r2 */
L8c61ce24: sa_motion_publish_stop(ctx,0x8c61ce24u); r[12]<<=2;goto L8c61ce26; /* shll2 r12 */
L8c61ce26: sa_motion_publish_stop(ctx,0x8c61ce26u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ce28; /* fsub fr1,fr3 */
L8c61ce28: sa_motion_publish_stop(ctx,0x8c61ce28u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61ce2a; /* fmov.s @r3,fr1 */
L8c61ce2a: sa_motion_publish_stop(ctx,0x8c61ce2au); r[14]=r[14]&65535u;goto L8c61ce2c; /* extu.w r14,r14 */
L8c61ce2c: sa_motion_publish_stop(ctx,0x8c61ce2cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ce2e; /* fmul fr2,fr1 */
L8c61ce2e: sa_motion_publish_stop(ctx,0x8c61ce2eu); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61ce30; /* shll r12 */
L8c61ce30: sa_motion_publish_stop(ctx,0x8c61ce30u); r[14]<<=2;goto L8c61ce32; /* shll2 r14 */
L8c61ce32: sa_motion_publish_stop(ctx,0x8c61ce32u); fcopy(f,15u,3u);goto L8c61ce34; /* fmov fr3,fr15 */
L8c61ce34: sa_motion_publish_stop(ctx,0x8c61ce34u); r[12]+=r[2];goto L8c61ce36; /* add r2,r12 */
L8c61ce36: sa_motion_publish_stop(ctx,0x8c61ce36u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[1],&f->fpscr,&fr[15],1);if(status)return status;}goto L8c61ce38; /* fsub fr1,fr15 */
L8c61ce38: sa_motion_publish_stop(ctx,0x8c61ce38u); r[14]<<=2;goto L8c61ce3a; /* shll2 r14 */
L8c61ce3a: sa_motion_publish_stop(ctx,0x8c61ce3au); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61ce3c; /* mov.l @(0x10,r15),r3 */
L8c61ce3c: sa_motion_publish_stop(ctx,0x8c61ce3cu); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61ce3e; /* shll r14 */
L8c61ce3e: sa_motion_publish_stop(ctx,0x8c61ce3eu); r[14]+=r[2];goto L8c61ce40; /* add r2,r14 */
L8c61ce40: sa_motion_publish_stop(ctx,0x8c61ce40u); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ce42; /* mov.w @r10,r2 */
L8c61ce42: sa_motion_publish_stop(ctx,0x8c61ce42u); *fpul=r[2];goto L8c61ce44; /* lds r2,FPUL */
L8c61ce44: sa_motion_publish_stop(ctx,0x8c61ce44u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ce46; /* float FPUL,fr3 */
L8c61ce46: sa_motion_publish_stop(ctx,0x8c61ce46u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ce48; /* ftrc fr3,FPUL */
L8c61ce48: sa_motion_publish_stop(ctx,0x8c61ce48u); r[1]=*fpul;goto L8c61ce4a; /* sts FPUL,r1 */
L8c61ce4a: sa_motion_publish_stop(ctx,0x8c61ce4au); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c61ce4c; /* mov.w r1,@r3 */
L8c61ce4c: sa_motion_publish_stop(ctx,0x8c61ce4cu); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ce4e; /* mov.w @(0x2,r10),r0 */
L8c61ce4e: sa_motion_publish_stop(ctx,0x8c61ce4eu); r[10]+=4u;goto L8c61ce50; /* add #0x4,r10 */
L8c61ce50: sa_motion_publish_stop(ctx,0x8c61ce50u); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61ce52; /* mov.l @(0x10,r15),r3 */
L8c61ce52: sa_motion_publish_stop(ctx,0x8c61ce52u); r[2]=r[0];goto L8c61ce54; /* mov r0,r2 */
L8c61ce54: sa_motion_publish_stop(ctx,0x8c61ce54u); *fpul=r[2];goto L8c61ce56; /* lds r2,FPUL */
L8c61ce56: sa_motion_publish_stop(ctx,0x8c61ce56u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ce58; /* float FPUL,fr3 */
L8c61ce58: sa_motion_publish_stop(ctx,0x8c61ce58u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ce5a; /* ftrc fr3,FPUL */
L8c61ce5a: sa_motion_publish_stop(ctx,0x8c61ce5au); r[0]=*fpul;goto L8c61ce5c; /* sts FPUL,r0 */
L8c61ce5c: sa_motion_publish_stop(ctx,0x8c61ce5cu); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61ce5e; /* mov.w r0,@(0x2,r3) */
L8c61ce5e: sa_motion_publish_stop(ctx,0x8c61ce5eu); r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ce60; /* mov.w @r10,r2 */
L8c61ce60: sa_motion_publish_stop(ctx,0x8c61ce60u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61ce62; /* mov.l @(0x4,r15),r3 */
L8c61ce62: sa_motion_publish_stop(ctx,0x8c61ce62u); *fpul=r[2];goto L8c61ce64; /* lds r2,FPUL */
L8c61ce64: sa_motion_publish_stop(ctx,0x8c61ce64u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ce66; /* float FPUL,fr3 */
L8c61ce66: sa_motion_publish_stop(ctx,0x8c61ce66u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ce68; /* ftrc fr3,FPUL */
L8c61ce68: sa_motion_publish_stop(ctx,0x8c61ce68u); r[1]=*fpul;goto L8c61ce6a; /* sts FPUL,r1 */
L8c61ce6a: sa_motion_publish_stop(ctx,0x8c61ce6au); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c61ce6c; /* mov.w r1,@r3 */
L8c61ce6c: sa_motion_publish_stop(ctx,0x8c61ce6cu); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ce6e; /* mov.w @(0x2,r10),r0 */
L8c61ce6e: sa_motion_publish_stop(ctx,0x8c61ce6eu); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61ce70; /* mov.l @(0x4,r15),r3 */
L8c61ce70: sa_motion_publish_stop(ctx,0x8c61ce70u); r[2]=r[0];goto L8c61ce72; /* mov r0,r2 */
L8c61ce72: sa_motion_publish_stop(ctx,0x8c61ce72u); *fpul=r[2];goto L8c61ce74; /* lds r2,FPUL */
L8c61ce74: sa_motion_publish_stop(ctx,0x8c61ce74u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ce76; /* float FPUL,fr3 */
L8c61ce76: sa_motion_publish_stop(ctx,0x8c61ce76u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ce78; /* ftrc fr3,FPUL */
L8c61ce78: sa_motion_publish_stop(ctx,0x8c61ce78u); r[0]=*fpul;goto L8c61ce7a; /* sts FPUL,r0 */
L8c61ce7a: sa_motion_publish_stop(ctx,0x8c61ce7au); r[10]+=4u;goto L8c61ce7c; /* add #0x4,r10 */
L8c61ce7c: sa_motion_publish_stop(ctx,0x8c61ce7cu); r[11]=0u;goto L8c61ce7e; /* mov #0x0,r11 */
L8c61ce7e: sa_motion_publish_stop(ctx,0x8c61ce7eu); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61ce80; /* mov.w r0,@(0x2,r3) */
L8c61ce80: sa_motion_publish_stop(ctx,0x8c61ce80u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ce82; /* mov.l @r15,r3 */
L8c61ce82: sa_motion_publish_stop(ctx,0x8c61ce82u); r[3]+=4294967294u;goto L8c61ce84; /* add #-0x2,r3 */
L8c61ce84: sa_motion_publish_stop(ctx,0x8c61ce84u); *sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c61ce86; /* cmp/pl r3 */
L8c61ce86: sa_motion_publish_stop(ctx,0x8c61ce88u); take=!(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c61cef4;goto L8c61ce8a; /* bf/s 0x8c61cef4 */
L8c61ce8a: sa_motion_publish_stop(ctx,0x8c61ce8au); r[13]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61ce8c; /* mov.w @r9+,r13 */
L8c61ce8c: sa_motion_publish_stop(ctx,0x8c61ce8cu); r[2]=rd(m,bus,&io_status,0x8c61cf88u,4);if(io_status)return io_status;goto L8c61ce8e; /* mov.l 0x8c61cf88,r2 */
L8c61ce8e: sa_motion_publish_stop(ctx,0x8c61ce8eu); r[13]=r[13]&65535u;goto L8c61ce90; /* extu.w r13,r13 */
L8c61ce90: sa_motion_publish_stop(ctx,0x8c61ce90u); r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61ce92; /* mov.l @(0x8,r15),r1 */
L8c61ce92: sa_motion_publish_stop(ctx,0x8c61ce92u); r[13]<<=2;goto L8c61ce94; /* shll2 r13 */
L8c61ce94: sa_motion_publish_stop(ctx,0x8c61ce94u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ce96; /* mov.l @r2,r3 */
L8c61ce96: sa_motion_publish_stop(ctx,0x8c61ce96u); r[13]<<=2;goto L8c61ce98; /* shll2 r13 */
L8c61ce98: sa_motion_publish_stop(ctx,0x8c61ce98u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61ce9a; /* shll r13 */
L8c61ce9a: sa_motion_publish_stop(ctx,0x8c61ce9au); r[13]+=r[3];goto L8c61ce9c; /* add r3,r13 */
L8c61ce9c: sa_motion_publish_stop(ctx,0x8c61ce9cu); r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ce9e; /* mov.w @r10,r3 */
L8c61ce9e: sa_motion_publish_stop(ctx,0x8c61ce9eu); *fpul=r[3];goto L8c61cea0; /* lds r3,FPUL */
L8c61cea0: sa_motion_publish_stop(ctx,0x8c61cea0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cea2; /* float FPUL,fr3 */
L8c61cea2: sa_motion_publish_stop(ctx,0x8c61cea2u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61cea4; /* ftrc fr3,FPUL */
L8c61cea4: sa_motion_publish_stop(ctx,0x8c61cea4u); r[0]=*fpul;goto L8c61cea6; /* sts FPUL,r0 */
L8c61cea6: sa_motion_publish_stop(ctx,0x8c61cea6u); wr(m,bus,&io_status,r[1],r[0],2);if(io_status)return io_status;goto L8c61cea8; /* mov.w r0,@r1 */
L8c61cea8: sa_motion_publish_stop(ctx,0x8c61cea8u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ceaa; /* mov.w @(0x2,r10),r0 */
L8c61ceaa: sa_motion_publish_stop(ctx,0x8c61ceaau); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61ceac; /* mov.l @(0x8,r15),r3 */
L8c61ceac: sa_motion_publish_stop(ctx,0x8c61ceacu); r[1]=r[0];goto L8c61ceae; /* mov r0,r1 */
L8c61ceae: sa_motion_publish_stop(ctx,0x8c61ceaeu); *fpul=r[1];goto L8c61ceb0; /* lds r1,FPUL */
L8c61ceb0: sa_motion_publish_stop(ctx,0x8c61ceb0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ceb2; /* float FPUL,fr3 */
L8c61ceb2: sa_motion_publish_stop(ctx,0x8c61ceb2u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ceb4; /* ftrc fr3,FPUL */
L8c61ceb4: sa_motion_publish_stop(ctx,0x8c61ceb4u); r[0]=*fpul;goto L8c61ceb6; /* sts FPUL,r0 */
L8c61ceb6: sa_motion_publish_stop(ctx,0x8c61ceb6u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61ceb8; /* mov.w r0,@(0x2,r3) */
L8c61ceb8: sa_motion_publish_stop(ctx,0x8c61ceb8u); r[4]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61ceba; /* mov.l @(0xc,r12),r4 */
L8c61ceba: sa_motion_publish_stop(ctx,0x8c61cebau); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61cebc; /* mov.l @(0xc,r14),r3 */
L8c61cebc: sa_motion_publish_stop(ctx,0x8c61cebcu); r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61cebe; /* mov.l @(0xc,r13),r1 */
L8c61cebe: sa_motion_publish_stop(ctx,0x8c61cebeu); r[4]&=r[3];goto L8c61cec0; /* and r3,r4 */
L8c61cec0: sa_motion_publish_stop(ctx,0x8c61cec0u); r[3]=1u;goto L8c61cec2; /* mov #0x1,r3 */
L8c61cec2: sa_motion_publish_stop(ctx,0x8c61cec2u); r[4]&=r[1];goto L8c61cec4; /* and r1,r4 */
L8c61cec4: sa_motion_publish_stop(ctx,0x8c61cec4u); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61cec6; /* tst r3,r4 */
L8c61cec6: sa_motion_publish_stop(ctx,0x8c61cec8u); take=!(*sr&1u);r[10]+=4u;if(take)goto L8c61ced8;goto L8c61ceca; /* bf/s 0x8c61ced8 */
L8c61ceca: sa_motion_publish_stop(ctx,0x8c61cecau); r[7]=rd(m,bus,&io_status,0x8c61cf8cu,4);if(io_status)return io_status;goto L8c61cecc; /* mov.l 0x8c61cf8c,r7 */
L8c61cecc: sa_motion_publish_stop(ctx,0x8c61ceccu); r[5]=r[14];goto L8c61cece; /* mov r14,r5 */
L8c61cece: sa_motion_publish_stop(ctx,0x8c61ceceu); r[1]=rd(m,bus,&io_status,0x8c61cf90u,4);if(io_status)return io_status;goto L8c61ced0; /* mov.l 0x8c61cf90,r1 */
L8c61ced0: sa_motion_publish_stop(ctx,0x8c61ced0u); r[6]=r[13];goto L8c61ced2; /* mov r13,r6 */
L8c61ced2: sa_motion_publish_stop(ctx,0x8c61ced2u); fcopy(f,4u,15u);goto L8c61ced4; /* fmov fr15,fr4 */
L8c61ced4: sa_motion_publish_stop(ctx,0x8c61ced6u); target=r[1];*pr=0x8c61ced8u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ced8; /* jsr @r1 */
L8c61ced8: sa_motion_publish_stop(ctx,0x8c61ced8u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61ceda; /* mov.l @(0x4,r15),r2 */
L8c61ceda: sa_motion_publish_stop(ctx,0x8c61cedau); r[11]+=1u;goto L8c61cedc; /* add #0x1,r11 */
L8c61cedc: sa_motion_publish_stop(ctx,0x8c61cedcu); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61cede; /* mov.l @(0x10,r15),r3 */
L8c61cede: sa_motion_publish_stop(ctx,0x8c61cedeu); r[12]=r[14];goto L8c61cee0; /* mov r14,r12 */
L8c61cee0: sa_motion_publish_stop(ctx,0x8c61cee0u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61cee2; /* mov.l @r2,r1 */
L8c61cee2: sa_motion_publish_stop(ctx,0x8c61cee2u); wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c61cee4; /* mov.l r1,@r3 */
L8c61cee4: sa_motion_publish_stop(ctx,0x8c61cee4u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61cee6; /* mov.l @(0x8,r15),r3 */
L8c61cee6: sa_motion_publish_stop(ctx,0x8c61cee6u); r[0]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61cee8; /* mov.l @(0x4,r15),r0 */
L8c61cee8: sa_motion_publish_stop(ctx,0x8c61cee8u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ceea; /* mov.l @r3,r2 */
L8c61ceea: sa_motion_publish_stop(ctx,0x8c61ceeau); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61ceec; /* mov.l r2,@r0 */
L8c61ceec: sa_motion_publish_stop(ctx,0x8c61ceecu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ceee; /* mov.l @r15,r3 */
L8c61ceee: sa_motion_publish_stop(ctx,0x8c61ceeeu); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[3]);goto L8c61cef0; /* cmp/ge r3,r11 */
L8c61cef0: sa_motion_publish_stop(ctx,0x8c61cef2u); take=!(*sr&1u);r[14]=r[13];if(take)goto L8c61ce8a;goto L8c61cef4; /* bf/s 0x8c61ce8a */
L8c61cef4: sa_motion_publish_stop(ctx,0x8c61cef4u); r[1]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c61cef6; /* mov.l @(0x14,r15),r1 */
L8c61cef6: sa_motion_publish_stop(ctx,0x8c61cef6u); r[1]+=1u;goto L8c61cef8; /* add #0x1,r1 */
L8c61cef8: sa_motion_publish_stop(ctx,0x8c61cef8u); wr(m,bus,&io_status,r[15]+0x14u,r[1],4);if(io_status)return io_status;goto L8c61cefa; /* mov.l r1,@(0x14,r15) */
L8c61cefa: sa_motion_publish_stop(ctx,0x8c61cefau); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61cefc; /* mov.l @(0xc,r15),r3 */
L8c61cefc: sa_motion_publish_stop(ctx,0x8c61cefcu); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61cefe; /* mov.w @(0x2,r3),r0 */
L8c61cefe: sa_motion_publish_stop(ctx,0x8c61cefeu); r[3]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c61cf00; /* mov.l @(0x14,r15),r3 */
L8c61cf00: sa_motion_publish_stop(ctx,0x8c61cf00u); r[0]=r[0]&65535u;goto L8c61cf02; /* extu.w r0,r0 */
L8c61cf02: sa_motion_publish_stop(ctx,0x8c61cf02u); *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c61cf04; /* cmp/ge r0,r3 */
L8c61cf04: sa_motion_publish_stop(ctx,0x8c61cf04u); if((*sr&1u))goto L8c61cf0a;goto L8c61cf06; /* bt 0x8c61cf0a */
L8c61cf06: sa_motion_publish_stop(ctx,0x8c61cf08u); goto L8c61cb24; /* bra 0x8c61cb24 */
L8c61cf0a: sa_motion_publish_stop(ctx,0x8c61cf0cu); goto L8c61cf4c; /* bra 0x8c61cf4c */
L8c61cf0e: sa_motion_publish_stop(ctx,0x8c61cf0eu); r[1]=rd(m,bus,&io_status,0x8c61cf94u,4);if(io_status)return io_status;goto L8c61cf10; /* mov.l 0x8c61cf94,r1 */
L8c61cf10: sa_motion_publish_stop(ctx,0x8c61cf10u); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61cf12; /* mov.l @(0xc,r15),r2 */
L8c61cf12: sa_motion_publish_stop(ctx,0x8c61cf12u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61cf14; /* mov.l r2,@r1 */
L8c61cf14: sa_motion_publish_stop(ctx,0x8c61cf14u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61cf16; /* frchg */
L8c61cf16: sa_motion_publish_stop(ctx,0x8c61cf16u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61cf18; /* fmov.s fr0,@-r15 */
L8c61cf18: sa_motion_publish_stop(ctx,0x8c61cf18u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61cf1a; /* fmov.s fr1,@-r15 */
L8c61cf1a: sa_motion_publish_stop(ctx,0x8c61cf1au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61cf1c; /* fmov.s fr2,@-r15 */
L8c61cf1c: sa_motion_publish_stop(ctx,0x8c61cf1cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61cf1e; /* fmov.s fr3,@-r15 */
L8c61cf1e: sa_motion_publish_stop(ctx,0x8c61cf1eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61cf20; /* fmov.s fr4,@-r15 */
L8c61cf20: sa_motion_publish_stop(ctx,0x8c61cf20u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61cf22; /* fmov.s fr5,@-r15 */
L8c61cf22: sa_motion_publish_stop(ctx,0x8c61cf22u); r[0]=rd(m,bus,&io_status,0x8c61cf64u,4);if(io_status)return io_status;goto L8c61cf24; /* mov.l 0x8c61cf64,r0 */
L8c61cf24: sa_motion_publish_stop(ctx,0x8c61cf24u); fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf26; /* fmov.s @r0+,fr0 */
L8c61cf26: sa_motion_publish_stop(ctx,0x8c61cf26u); fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf28; /* fmov.s @r0+,fr1 */
L8c61cf28: sa_motion_publish_stop(ctx,0x8c61cf28u); fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c61cf2a; /* fmov.s @r0,fr2 */
L8c61cf2a: sa_motion_publish_stop(ctx,0x8c61cf2au); r[0]=rd(m,bus,&io_status,0x8c61cf68u,4);if(io_status)return io_status;goto L8c61cf2c; /* mov.l 0x8c61cf68,r0 */
L8c61cf2c: sa_motion_publish_stop(ctx,0x8c61cf2cu); fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c61cf2e; /* fmov.s @r0,fr3 */
L8c61cf2e: sa_motion_publish_stop(ctx,0x8c61cf2eu); r[0]=0x8c61cf6cu;goto L8c61cf30; /* mova 0x8c61cf6c,r0 */
L8c61cf30: sa_motion_publish_stop(ctx,0x8c61cf30u); fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c61cf32; /* fmov.s @r0,fr4 */
L8c61cf32: sa_motion_publish_stop(ctx,0x8c61cf32u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61cf34; /* frchg */
L8c61cf34: sa_motion_publish_stop(ctx,0x8c61cf34u); r[2]=rd(m,bus,&io_status,0x8c61cf98u,4);if(io_status)return io_status;goto L8c61cf36; /* mov.l 0x8c61cf98,r2 */
L8c61cf36: sa_motion_publish_stop(ctx,0x8c61cf36u); r[3]=rd(m,bus,&io_status,0x8c61cf94u,4);if(io_status)return io_status;goto L8c61cf38; /* mov.l 0x8c61cf94,r3 */
L8c61cf38: sa_motion_publish_stop(ctx,0x8c61cf3au); target=r[2];*pr=0x8c61cf3cu;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61cf3c; /* jsr @r2 */
L8c61cf3c: sa_motion_publish_stop(ctx,0x8c61cf3cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61cf3e; /* frchg */
L8c61cf3e: sa_motion_publish_stop(ctx,0x8c61cf3eu); fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf40; /* fmov.s @r15+,fr5 */
L8c61cf40: sa_motion_publish_stop(ctx,0x8c61cf40u); fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf42; /* fmov.s @r15+,fr4 */
L8c61cf42: sa_motion_publish_stop(ctx,0x8c61cf42u); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf44; /* fmov.s @r15+,fr3 */
L8c61cf44: sa_motion_publish_stop(ctx,0x8c61cf44u); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf46; /* fmov.s @r15+,fr2 */
L8c61cf46: sa_motion_publish_stop(ctx,0x8c61cf46u); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf48; /* fmov.s @r15+,fr1 */
L8c61cf48: sa_motion_publish_stop(ctx,0x8c61cf48u); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf4a; /* fmov.s @r15+,fr0 */
L8c61cf4a: sa_motion_publish_stop(ctx,0x8c61cf4au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61cf4c; /* frchg */
L8c61cf4c: sa_motion_publish_stop(ctx,0x8c61cf4cu); r[15]+=40u;goto L8c61cf4e; /* add #0x28,r15 */
L8c61cf4e: sa_motion_publish_stop(ctx,0x8c61cf4eu); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61cf50; /* lds.l @r15+,PR */
L8c61cf50: sa_motion_publish_stop(ctx,0x8c61cf50u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf52; /* fmov.s @r15+,fr14 */
L8c61cf52: sa_motion_publish_stop(ctx,0x8c61cf52u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf54; /* fmov.s @r15+,fr15 */
L8c61cf54: sa_motion_publish_stop(ctx,0x8c61cf54u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61cf56; /* mov.l @r15+,r9 */
L8c61cf56: sa_motion_publish_stop(ctx,0x8c61cf56u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61cf58; /* mov.l @r15+,r10 */
L8c61cf58: sa_motion_publish_stop(ctx,0x8c61cf58u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61cf5a; /* mov.l @r15+,r11 */
L8c61cf5a: sa_motion_publish_stop(ctx,0x8c61cf5au); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61cf5c; /* mov.l @r15+,r12 */
L8c61cf5c: sa_motion_publish_stop(ctx,0x8c61cf5cu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61cf5e; /* mov.l @r15+,r13 */
L8c61cf5e: sa_motion_publish_stop(ctx,0x8c61cf60u); if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
