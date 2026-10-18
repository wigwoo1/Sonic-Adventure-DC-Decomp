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
L8c61be44: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61be46; /* mov.l r14,@-r15 */
L8c61be46: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61be48; /* mov.l r13,@-r15 */
L8c61be48: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61be4a; /* mov.l r12,@-r15 */
L8c61be4a: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61be4c; /* mov.l r11,@-r15 */
L8c61be4c: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61be4e; /* mov.l r10,@-r15 */
L8c61be4e: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61be50; /* mov.l r9,@-r15 */
L8c61be50: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61be52; /* fmov.s fr15,@-r15 */
L8c61be52: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61be54; /* sts.l PR,@-r15 */
L8c61be54: r[15]+=4294967276u;goto L8c61be56; /* add #-0x14,r15 */
L8c61be56: r[0]=0x8c61bf08u;goto L8c61be58; /* mova 0x8c61bf08,r0 */
L8c61be58: r[5]=r[4];goto L8c61be5a; /* mov r4,r5 */
L8c61be5a: r[3]=rd(m,bus,&io_status,0x8c61bf10u,4);if(io_status)return io_status;goto L8c61be5c; /* mov.l 0x8c61bf10,r3 */
L8c61be5c: r[13]=rd(m,bus,&io_status,0x8c61bf0cu,4);if(io_status)return io_status;goto L8c61be5e; /* mov.l 0x8c61bf0c,r13 */
L8c61be5e: wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61be60; /* mov.l r4,@r15 */
L8c61be60: fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c61be62; /* fmov.s @r0,fr15 */
L8c61be62: r[0]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;goto L8c61be64; /* mov.w @r5,r0 */
L8c61be64: r[9]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c61be66; /* mov.l @(0x4,r5),r9 */
L8c61be66: r[0]=r[0]&65535u;goto L8c61be68; /* extu.w r0,r0 */
L8c61be68: r[10]=rd(m,bus,&io_status,r[5]+0x14u,4);if(io_status)return io_status;goto L8c61be6a; /* mov.l @(0x14,r5),r10 */
L8c61be6a: r[0]&=r[3];goto L8c61be6c; /* and r3,r0 */
L8c61be6c: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61be6e; /* cmp/eq #0x0,r0 */
L8c61be6e: take=(*sr&1u);r[4]=0u;if(take)goto L8c61be94;goto L8c61be72; /* bt/s 0x8c61be94 */
L8c61be72: r[1]=sx(m,bus,&io_status,0x8c61bf06u,2);if(io_status)return io_status;goto L8c61be74; /* mov.w 0x8c61bf06,r1 */
L8c61be74: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61be76; /* cmp/eq r1,r0 */
L8c61be76: if(!(*sr&1u))goto L8c61be7c;goto L8c61be78; /* bf 0x8c61be7c */
L8c61be78: goto L8c61c032; /* bra 0x8c61c032 */
L8c61be7c: r[1]=rd(m,bus,&io_status,0x8c61bf14u,4);if(io_status)return io_status;goto L8c61be7e; /* mov.l 0x8c61bf14,r1 */
L8c61be7e: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61be80; /* cmp/eq r1,r0 */
L8c61be80: if(!(*sr&1u))goto L8c61be86;goto L8c61be82; /* bf 0x8c61be86 */
L8c61be82: goto L8c61c1f8; /* bra 0x8c61c1f8 */
L8c61be86: r[1]=rd(m,bus,&io_status,0x8c61bf10u,4);if(io_status)return io_status;goto L8c61be88; /* mov.l 0x8c61bf10,r1 */
L8c61be88: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61be8a; /* cmp/eq r1,r0 */
L8c61be8a: if(!(*sr&1u))goto L8c61be90;goto L8c61be8c; /* bf 0x8c61be90 */
L8c61be8c: goto L8c61c42c; /* bra 0x8c61c42c */
L8c61be90: goto L8c61c46a; /* bra 0x8c61c46a */
L8c61be94: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61be96; /* mov.l @r15,r2 */
L8c61be96: r[1]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61be98; /* mov.l @r13,r1 */
L8c61be98: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61be9a; /* mov.w @(0x2,r2),r0 */
L8c61be9a: r[0]=r[0]&65535u;goto L8c61be9c; /* extu.w r0,r0 */
L8c61be9c: r[2]=r[0];goto L8c61be9e; /* mov r0,r2 */
L8c61be9e: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61bea0; /* shll r0 */
L8c61bea0: r[0]+=r[2];goto L8c61bea2; /* add r2,r0 */
L8c61bea2: r[1]+=r[0];goto L8c61bea4; /* add r0,r1 */
L8c61bea4: wr(m,bus,&io_status,r[13],r[1],4);if(io_status)return io_status;goto L8c61bea6; /* mov.l r1,@r13 */
L8c61bea6: r[11]=r[4];goto L8c61c020; /* bra 0x8c61c020 */
L8c61beaa: r[13]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61beac; /* mov.w @r9+,r13 */
L8c61beac: r[0]=16u;goto L8c61beae; /* mov #0x10,r0 */
L8c61beae: r[3]=rd(m,bus,&io_status,0x8c61bf18u,4);if(io_status)return io_status;goto L8c61beb0; /* mov.l 0x8c61bf18,r3 */
L8c61beb0: r[12]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61beb2; /* mov.w @r9+,r12 */
L8c61beb2: r[13]=r[13]&65535u;goto L8c61beb4; /* extu.w r13,r13 */
L8c61beb4: r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61beb6; /* mov.l @r3,r4 */
L8c61beb6: r[13]<<=2;goto L8c61beb8; /* shll2 r13 */
L8c61beb8: r[12]=r[12]&65535u;goto L8c61beba; /* extu.w r12,r12 */
L8c61beba: r[14]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61bebc; /* mov.w @r9+,r14 */
L8c61bebc: r[12]<<=2;goto L8c61bebe; /* shll2 r12 */
L8c61bebe: r[12]<<=2;goto L8c61bec0; /* shll2 r12 */
L8c61bec0: r[13]<<=2;goto L8c61bec2; /* shll2 r13 */
L8c61bec2: r[14]=r[14]&65535u;goto L8c61bec4; /* extu.w r14,r14 */
L8c61bec4: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61bec6; /* shll r13 */
L8c61bec6: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61bec8; /* shll r12 */
L8c61bec8: r[12]+=r[4];goto L8c61beca; /* add r4,r12 */
L8c61beca: fload(m,bus,&io_status,f,5u,r[0]+r[12]);if(io_status)return io_status;goto L8c61becc; /* fmov.s @(r0,r12),fr5 */
L8c61becc: r[13]+=r[4];goto L8c61bece; /* add r4,r13 */
L8c61bece: r[14]<<=2;goto L8c61bed0; /* shll2 r14 */
L8c61bed0: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bed2; /* fmov.s @(r0,r13),fr3 */
L8c61bed2: r[14]<<=2;goto L8c61bed4; /* shll2 r14 */
L8c61bed4: fcopy(f,2u,5u);goto L8c61bed6; /* fmov fr5,fr2 */
L8c61bed6: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61bed8; /* shll r14 */
L8c61bed8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61beda; /* fsub fr3,fr2 */
L8c61beda: r[0]=20u;goto L8c61bedc; /* mov #0x14,r0 */
L8c61bedc: r[14]+=r[4];goto L8c61bede; /* add r4,r14 */
L8c61bede: fload(m,bus,&io_status,f,4u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bee0; /* fmov.s @(r0,r12),fr4 */
L8c61bee0: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bee2; /* fmov.s @(r0,r14),fr3 */
L8c61bee2: fcopy(f,1u,4u);goto L8c61bee4; /* fmov fr4,fr1 */
L8c61bee4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bee6; /* fsub fr4,fr3 */
L8c61bee6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61bee8; /* fmul fr3,fr2 */
L8c61bee8: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61beea; /* fmov.s @(r0,r13),fr3 */
L8c61beea: r[0]=16u;goto L8c61beec; /* mov #0x10,r0 */
L8c61beec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61beee; /* fsub fr3,fr1 */
L8c61beee: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bef0; /* fmov.s @(r0,r14),fr3 */
L8c61bef0: fcopy(f,4u,2u);goto L8c61bef2; /* fmov fr2,fr4 */
L8c61bef2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bef4; /* fsub fr5,fr3 */
L8c61bef4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bef6; /* fmul fr3,fr1 */
L8c61bef6: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61bef8; /* fldi0 fr3 */
L8c61bef8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61befa; /* fsub fr1,fr4 */
L8c61befa: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61befc; /* fcmp/gt fr3,fr4 */
L8c61befc: if(!(*sr&1u))goto L8c61bf1c;goto L8c61befe; /* bf 0x8c61bf1c */
L8c61befe: r[10]+=12u;goto L8c61c01e; /* bra 0x8c61c01e */
L8c61bf1c: fload(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c61bf1e; /* fmov.s @r13,fr2 */
L8c61bf1e: r[0]=8u;goto L8c61bf20; /* mov #0x8,r0 */
L8c61bf20: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61bf22; /* fmov.s @r12,fr3 */
L8c61bf22: r[4]=r[15];goto L8c61bf24; /* mov r15,r4 */
L8c61bf24: r[3]=rd(m,bus,&io_status,0x8c61c204u,4);if(io_status)return io_status;goto L8c61bf26; /* mov.l 0x8c61c204,r3 */
L8c61bf26: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bf28; /* fadd fr3,fr2 */
L8c61bf28: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61bf2a; /* fmov.s @r14,fr3 */
L8c61bf2a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bf2c; /* fadd fr3,fr2 */
L8c61bf2c: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bf2e; /* fmov fr2,@(r0,r15) */
L8c61bf2e: r[0]=4u;goto L8c61bf30; /* mov #0x4,r0 */
L8c61bf30: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bf32; /* fmov.s @(r0,r13),fr2 */
L8c61bf32: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bf34; /* fmov.s @(r0,r12),fr3 */
L8c61bf34: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bf36; /* fadd fr3,fr2 */
L8c61bf36: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bf38; /* fmov.s @(r0,r14),fr3 */
L8c61bf38: r[0]=12u;goto L8c61bf3a; /* mov #0xc,r0 */
L8c61bf3a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bf3c; /* fadd fr3,fr2 */
L8c61bf3c: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bf3e; /* fmov fr2,@(r0,r15) */
L8c61bf3e: r[0]=8u;goto L8c61bf40; /* mov #0x8,r0 */
L8c61bf40: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bf42; /* fmov.s @(r0,r13),fr2 */
L8c61bf42: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bf44; /* fmov.s @(r0,r12),fr3 */
L8c61bf44: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bf46; /* fadd fr3,fr2 */
L8c61bf46: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61bf48; /* fmov.s @(r0,r14),fr3 */
L8c61bf48: r[0]=16u;goto L8c61bf4a; /* mov #0x10,r0 */
L8c61bf4a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61bf4c; /* fadd fr3,fr2 */
L8c61bf4c: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bf4e; /* fmov fr2,@(r0,r15) */
L8c61bf4e: target=r[3];*pr=0x8c61bf52u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61bf52; /* jsr @r3 */
L8c61bf52: r[3]=rd(m,bus,&io_status,0x8c61c208u,4);if(io_status)return io_status;goto L8c61bf54; /* mov.l 0x8c61c208,r3 */
L8c61bf54: r[0]=8u;goto L8c61bf56; /* mov #0x8,r0 */
L8c61bf56: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bf58; /* fmov.s @(r0,r15),fr3 */
L8c61bf58: r[0]=12u;goto L8c61bf5a; /* mov #0xc,r0 */
L8c61bf5a: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61bf5c; /* fmov.s @r3,fr2 */
L8c61bf5c: r[8]+=24u;goto L8c61bf5e; /* add #0x18,r8 */
L8c61bf5e: r[2]=rd(m,bus,&io_status,0x8c61c20cu,4);if(io_status)return io_status;goto L8c61bf60; /* mov.l 0x8c61c20c,r2 */
L8c61bf60: r[3]+=8u;goto L8c61bf62; /* add #0x8,r3 */
L8c61bf62: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61bf64; /* fmul fr3,fr2 */
L8c61bf64: r[1]=rd(m,bus,&io_status,0x8c61c210u,4);if(io_status)return io_status;goto L8c61bf66; /* mov.l 0x8c61c210,r1 */
L8c61bf66: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61bf68; /* fmov.s @r2,fr3 */
L8c61bf68: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61bf6a; /* fmov.s @r1,fr1 */
L8c61bf6a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bf6c; /* fsub fr2,fr3 */
L8c61bf6c: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bf6e; /* fmov.s @(r0,r15),fr2 */
L8c61bf6e: r[0]=16u;goto L8c61bf70; /* mov #0x10,r0 */
L8c61bf70: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bf72; /* fmul fr2,fr1 */
L8c61bf72: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61bf74; /* fmov.s @(r0,r15),fr2 */
L8c61bf74: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61bf76; /* mov.w @(0x2,r10),r0 */
L8c61bf76: r[2]=r[0];goto L8c61bf78; /* mov r0,r2 */
L8c61bf78: *fpul=r[2];goto L8c61bf7a; /* lds r2,FPUL */
L8c61bf7a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61bf7c; /* fsub fr1,fr3 */
L8c61bf7c: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61bf7e; /* fmov.s @r3,fr1 */
L8c61bf7e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61bf80; /* fmul fr2,fr1 */
L8c61bf80: fcopy(f,4u,3u);goto L8c61bf82; /* fmov fr3,fr4 */
L8c61bf82: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61bf84; /* float FPUL,fr3 */
L8c61bf84: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61bf86; /* fsub fr1,fr4 */
L8c61bf86: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bf88; /* fmul fr15,fr3 */
L8c61bf88: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bf8a; /* fmov.s fr3,@-r8 */
L8c61bf8a: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61bf8c; /* mov.w @r10,r2 */
L8c61bf8c: *fpul=r[2];goto L8c61bf8e; /* lds r2,FPUL */
L8c61bf8e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61bf90; /* float FPUL,fr3 */
L8c61bf90: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bf92; /* fmul fr15,fr3 */
L8c61bf92: r[0]=24u;goto L8c61bf94; /* mov #0x18,r0 */
L8c61bf94: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bf96; /* fmov.s fr3,@-r8 */
L8c61bf96: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bf98; /* fmov.s @(r0,r13),fr3 */
L8c61bf98: r[0]=20u;goto L8c61bf9a; /* mov #0x14,r0 */
L8c61bf9a: r[2]=rd(m,bus,&io_status,0x8c61c214u,4);if(io_status)return io_status;goto L8c61bf9c; /* mov.l 0x8c61c214,r2 */
L8c61bf9c: r[10]+=4u;goto L8c61bf9e; /* add #0x4,r10 */
L8c61bf9e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfa0; /* fmov.s fr3,@-r8 */
L8c61bfa0: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bfa2; /* fmov.s @(r0,r13),fr3 */
L8c61bfa2: r[0]=16u;goto L8c61bfa4; /* mov #0x10,r0 */
L8c61bfa4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfa6; /* fmov.s fr3,@-r8 */
L8c61bfa6: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61bfa8; /* fmov.s @(r0,r13),fr3 */
L8c61bfa8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfaa; /* fmov.s fr3,@-r8 */
L8c61bfaa: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61bfac; /* mov.l r2,@-r8 */
L8c61bfac: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61bfae; /* pref @r8 */
L8c61bfae: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61bfb0; /* mov.w @(0x2,r10),r0 */
L8c61bfb0: r[8]+=56u;goto L8c61bfb2; /* add #0x38,r8 */
L8c61bfb2: r[2]=r[0];goto L8c61bfb4; /* mov r0,r2 */
L8c61bfb4: *fpul=r[2];goto L8c61bfb6; /* lds r2,FPUL */
L8c61bfb6: r[0]=24u;goto L8c61bfb8; /* mov #0x18,r0 */
L8c61bfb8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61bfba; /* float FPUL,fr3 */
L8c61bfba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bfbc; /* fmul fr15,fr3 */
L8c61bfbc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfbe; /* fmov.s fr3,@-r8 */
L8c61bfbe: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61bfc0; /* mov.w @r10,r2 */
L8c61bfc0: r[10]+=4u;goto L8c61bfc2; /* add #0x4,r10 */
L8c61bfc2: *fpul=r[2];goto L8c61bfc4; /* lds r2,FPUL */
L8c61bfc4: r[2]=rd(m,bus,&io_status,0x8c61c214u,4);if(io_status)return io_status;goto L8c61bfc6; /* mov.l 0x8c61c214,r2 */
L8c61bfc6: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61bfc8; /* float FPUL,fr3 */
L8c61bfc8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bfca; /* fmul fr15,fr3 */
L8c61bfca: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfcc; /* fmov.s fr3,@-r8 */
L8c61bfcc: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bfce; /* fmov.s @(r0,r12),fr3 */
L8c61bfce: r[0]=20u;goto L8c61bfd0; /* mov #0x14,r0 */
L8c61bfd0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfd2; /* fmov.s fr3,@-r8 */
L8c61bfd2: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bfd4; /* fmov.s @(r0,r12),fr3 */
L8c61bfd4: r[0]=16u;goto L8c61bfd6; /* mov #0x10,r0 */
L8c61bfd6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfd8; /* fmov.s fr3,@-r8 */
L8c61bfd8: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61bfda; /* fmov.s @(r0,r12),fr3 */
L8c61bfda: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bfdc; /* fmov.s fr3,@-r8 */
L8c61bfdc: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61bfde; /* mov.l r2,@-r8 */
L8c61bfde: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61bfe0; /* pref @r8 */
L8c61bfe0: r[8]+=60u;goto L8c61bfe2; /* add #0x3c,r8 */
L8c61bfe2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61bfe4; /* fmov.s fr4,@-r8 */
L8c61bfe4: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61bfe6; /* mov.w @(0x2,r10),r0 */
L8c61bfe6: r[2]=r[0];goto L8c61bfe8; /* mov r0,r2 */
L8c61bfe8: *fpul=r[2];goto L8c61bfea; /* lds r2,FPUL */
L8c61bfea: r[0]=24u;goto L8c61bfec; /* mov #0x18,r0 */
L8c61bfec: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61bfee; /* float FPUL,fr3 */
L8c61bfee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bff0; /* fmul fr15,fr3 */
L8c61bff0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61bff2; /* fmov.s fr3,@-r8 */
L8c61bff2: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61bff4; /* mov.w @r10,r2 */
L8c61bff4: r[10]+=4u;goto L8c61bff6; /* add #0x4,r10 */
L8c61bff6: *fpul=r[2];goto L8c61bff8; /* lds r2,FPUL */
L8c61bff8: r[2]=rd(m,bus,&io_status,0x8c61c218u,4);if(io_status)return io_status;goto L8c61bffa; /* mov.l 0x8c61c218,r2 */
L8c61bffa: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61bffc; /* float FPUL,fr3 */
L8c61bffc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61bffe; /* fmul fr15,fr3 */
L8c61bffe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c000; /* fmov.s fr3,@-r8 */
L8c61c000: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c002; /* fmov.s @(r0,r14),fr3 */
L8c61c002: r[0]=20u;goto L8c61c004; /* mov #0x14,r0 */
L8c61c004: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c006; /* fmov.s fr3,@-r8 */
L8c61c006: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c008; /* fmov.s @(r0,r14),fr3 */
L8c61c008: r[0]=16u;goto L8c61c00a; /* mov #0x10,r0 */
L8c61c00a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c00c; /* fmov.s fr3,@-r8 */
L8c61c00c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c00e; /* fmov.s @(r0,r14),fr3 */
L8c61c00e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c010; /* fmov.s fr3,@-r8 */
L8c61c010: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c012; /* mov.l r2,@-r8 */
L8c61c012: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c014; /* pref @r8 */
L8c61c014: r[0]=rd(m,bus,&io_status,0x8c61c21cu,4);if(io_status)return io_status;goto L8c61c016; /* mov.l 0x8c61c21c,r0 */
L8c61c016: r[8]+=32u;goto L8c61c018; /* add #0x20,r8 */
L8c61c018: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61c01a; /* mov.l @r0,r2 */
L8c61c01a: r[2]+=3u;goto L8c61c01c; /* add #0x3,r2 */
L8c61c01c: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61c01e; /* mov.l r2,@r0 */
L8c61c01e: r[11]+=1u;goto L8c61c020; /* add #0x1,r11 */
L8c61c020: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61c022; /* mov.l @r15,r3 */
L8c61c022: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61c024; /* mov.w @(0x2,r3),r0 */
L8c61c024: r[0]=r[0]&65535u;goto L8c61c026; /* extu.w r0,r0 */
L8c61c026: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61c028; /* cmp/ge r0,r11 */
L8c61c028: if((*sr&1u))goto L8c61c02e;goto L8c61c02a; /* bt 0x8c61c02e */
L8c61c02a: goto L8c61beaa; /* bra 0x8c61beaa */
L8c61c02e: goto L8c61c46a; /* bra 0x8c61c46a */
L8c61c032: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61c034; /* mov.l @r15,r1 */
L8c61c034: r[2]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61c036; /* mov.l @r13,r2 */
L8c61c036: r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61c038; /* mov.w @(0x2,r1),r0 */
L8c61c038: r[0]=r[0]&65535u;goto L8c61c03a; /* extu.w r0,r0 */
L8c61c03a: r[0]<<=2;goto L8c61c03c; /* shll2 r0 */
L8c61c03c: r[2]+=r[0];goto L8c61c03e; /* add r0,r2 */
L8c61c03e: wr(m,bus,&io_status,r[13],r[2],4);if(io_status)return io_status;goto L8c61c040; /* mov.l r2,@r13 */
L8c61c040: wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c61c1e4; /* bra 0x8c61c1e4 */
L8c61c044: r[14]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c046; /* mov.w @r9+,r14 */
L8c61c046: r[0]=16u;goto L8c61c048; /* mov #0x10,r0 */
L8c61c048: r[3]=rd(m,bus,&io_status,0x8c61c220u,4);if(io_status)return io_status;goto L8c61c04a; /* mov.l 0x8c61c220,r3 */
L8c61c04a: r[13]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c04c; /* mov.w @r9+,r13 */
L8c61c04c: r[14]=r[14]&65535u;goto L8c61c04e; /* extu.w r14,r14 */
L8c61c04e: r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61c050; /* mov.l @r3,r4 */
L8c61c050: r[14]<<=2;goto L8c61c052; /* shll2 r14 */
L8c61c052: r[13]=r[13]&65535u;goto L8c61c054; /* extu.w r13,r13 */
L8c61c054: r[12]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c056; /* mov.w @r9+,r12 */
L8c61c056: r[13]<<=2;goto L8c61c058; /* shll2 r13 */
L8c61c058: r[13]<<=2;goto L8c61c05a; /* shll2 r13 */
L8c61c05a: r[11]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c05c; /* mov.w @r9+,r11 */
L8c61c05c: r[14]<<=2;goto L8c61c05e; /* shll2 r14 */
L8c61c05e: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61c060; /* shll r14 */
L8c61c060: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61c062; /* shll r13 */
L8c61c062: r[13]+=r[4];goto L8c61c064; /* add r4,r13 */
L8c61c064: r[14]+=r[4];goto L8c61c066; /* add r4,r14 */
L8c61c066: fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c068; /* fmov.s @(r0,r13),fr5 */
L8c61c068: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c06a; /* fmov.s @(r0,r14),fr3 */
L8c61c06a: r[12]=r[12]&65535u;goto L8c61c06c; /* extu.w r12,r12 */
L8c61c06c: fcopy(f,2u,5u);goto L8c61c06e; /* fmov fr5,fr2 */
L8c61c06e: r[12]<<=2;goto L8c61c070; /* shll2 r12 */
L8c61c070: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61c072; /* fsub fr3,fr2 */
L8c61c072: r[11]=r[11]&65535u;goto L8c61c074; /* extu.w r11,r11 */
L8c61c074: r[12]<<=2;goto L8c61c076; /* shll2 r12 */
L8c61c076: r[11]<<=2;goto L8c61c078; /* shll2 r11 */
L8c61c078: r[0]=20u;goto L8c61c07a; /* mov #0x14,r0 */
L8c61c07a: fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c07c; /* fmov.s @(r0,r13),fr4 */
L8c61c07c: r[11]<<=2;goto L8c61c07e; /* shll2 r11 */
L8c61c07e: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61c080; /* shll r12 */
L8c61c080: r[12]+=r[4];goto L8c61c082; /* add r4,r12 */
L8c61c082: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61c084; /* shll r11 */
L8c61c084: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c086; /* fmov.s @(r0,r12),fr3 */
L8c61c086: fcopy(f,1u,4u);goto L8c61c088; /* fmov fr4,fr1 */
L8c61c088: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c08a; /* fsub fr4,fr3 */
L8c61c08a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c08c; /* fmul fr3,fr2 */
L8c61c08c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c08e; /* fmov.s @(r0,r14),fr3 */
L8c61c08e: r[0]=16u;goto L8c61c090; /* mov #0x10,r0 */
L8c61c090: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61c092; /* fsub fr3,fr1 */
L8c61c092: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c094; /* fmov.s @(r0,r12),fr3 */
L8c61c094: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c096; /* fsub fr5,fr3 */
L8c61c096: fcopy(f,4u,2u);goto L8c61c098; /* fmov fr2,fr4 */
L8c61c098: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c09a; /* fmul fr3,fr1 */
L8c61c09a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61c09c; /* fldi0 fr3 */
L8c61c09c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c09e; /* fsub fr1,fr4 */
L8c61c09e: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61c0a0; /* fcmp/gt fr3,fr4 */
L8c61c0a0: take=!(*sr&1u);r[11]+=r[4];if(take)goto L8c61c0a8;goto L8c61c0a4; /* bf/s 0x8c61c0a8 */
L8c61c0a4: r[10]+=16u;goto L8c61c1de; /* bra 0x8c61c1de */
L8c61c0a8: fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c61c0aa; /* fmov.s @r14,fr2 */
L8c61c0aa: r[0]=8u;goto L8c61c0ac; /* mov #0x8,r0 */
L8c61c0ac: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61c0ae; /* fmov.s @r13,fr3 */
L8c61c0ae: r[4]=r[15];goto L8c61c0b0; /* mov r15,r4 */
L8c61c0b0: r[3]=rd(m,bus,&io_status,0x8c61c204u,4);if(io_status)return io_status;goto L8c61c0b2; /* mov.l 0x8c61c204,r3 */
L8c61c0b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c0b4; /* fadd fr3,fr2 */
L8c61c0b4: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61c0b6; /* fmov.s @r12,fr3 */
L8c61c0b6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c0b8; /* fadd fr3,fr2 */
L8c61c0b8: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c0ba; /* fmov fr2,@(r0,r15) */
L8c61c0ba: r[0]=4u;goto L8c61c0bc; /* mov #0x4,r0 */
L8c61c0bc: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c0be; /* fmov.s @(r0,r14),fr2 */
L8c61c0be: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c0c0; /* fmov.s @(r0,r13),fr3 */
L8c61c0c0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c0c2; /* fadd fr3,fr2 */
L8c61c0c2: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c0c4; /* fmov.s @(r0,r12),fr3 */
L8c61c0c4: r[0]=12u;goto L8c61c0c6; /* mov #0xc,r0 */
L8c61c0c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c0c8; /* fadd fr3,fr2 */
L8c61c0c8: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c0ca; /* fmov fr2,@(r0,r15) */
L8c61c0ca: r[0]=8u;goto L8c61c0cc; /* mov #0x8,r0 */
L8c61c0cc: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c0ce; /* fmov.s @(r0,r14),fr2 */
L8c61c0ce: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c0d0; /* fmov.s @(r0,r13),fr3 */
L8c61c0d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c0d2; /* fadd fr3,fr2 */
L8c61c0d2: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c0d4; /* fmov.s @(r0,r12),fr3 */
L8c61c0d4: r[0]=16u;goto L8c61c0d6; /* mov #0x10,r0 */
L8c61c0d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c0d8; /* fadd fr3,fr2 */
L8c61c0d8: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c0da; /* fmov fr2,@(r0,r15) */
L8c61c0da: target=r[3];*pr=0x8c61c0deu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61c0de; /* jsr @r3 */
L8c61c0de: r[3]=rd(m,bus,&io_status,0x8c61c208u,4);if(io_status)return io_status;goto L8c61c0e0; /* mov.l 0x8c61c208,r3 */
L8c61c0e0: r[0]=8u;goto L8c61c0e2; /* mov #0x8,r0 */
L8c61c0e2: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c0e4; /* fmov.s @(r0,r15),fr3 */
L8c61c0e4: r[0]=12u;goto L8c61c0e6; /* mov #0xc,r0 */
L8c61c0e6: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61c0e8; /* fmov.s @r3,fr2 */
L8c61c0e8: r[8]+=24u;goto L8c61c0ea; /* add #0x18,r8 */
L8c61c0ea: r[2]=rd(m,bus,&io_status,0x8c61c20cu,4);if(io_status)return io_status;goto L8c61c0ec; /* mov.l 0x8c61c20c,r2 */
L8c61c0ec: r[3]+=8u;goto L8c61c0ee; /* add #0x8,r3 */
L8c61c0ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c0f0; /* fmul fr3,fr2 */
L8c61c0f0: r[1]=rd(m,bus,&io_status,0x8c61c210u,4);if(io_status)return io_status;goto L8c61c0f2; /* mov.l 0x8c61c210,r1 */
L8c61c0f2: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61c0f4; /* fmov.s @r2,fr3 */
L8c61c0f4: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61c0f6; /* fmov.s @r1,fr1 */
L8c61c0f6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c0f8; /* fsub fr2,fr3 */
L8c61c0f8: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c0fa; /* fmov.s @(r0,r15),fr2 */
L8c61c0fa: r[0]=16u;goto L8c61c0fc; /* mov #0x10,r0 */
L8c61c0fc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c0fe; /* fmul fr2,fr1 */
L8c61c0fe: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c100; /* fmov.s @(r0,r15),fr2 */
L8c61c100: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c102; /* mov.w @(0x2,r10),r0 */
L8c61c102: r[2]=r[0];goto L8c61c104; /* mov r0,r2 */
L8c61c104: *fpul=r[2];goto L8c61c106; /* lds r2,FPUL */
L8c61c106: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c108; /* fsub fr1,fr3 */
L8c61c108: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61c10a; /* fmov.s @r3,fr1 */
L8c61c10a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c10c; /* fmul fr2,fr1 */
L8c61c10c: fcopy(f,4u,3u);goto L8c61c10e; /* fmov fr3,fr4 */
L8c61c10e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c110; /* float FPUL,fr3 */
L8c61c110: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c112; /* fsub fr1,fr4 */
L8c61c112: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c114; /* fmul fr15,fr3 */
L8c61c114: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c116; /* fmov.s fr3,@-r8 */
L8c61c116: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c118; /* mov.w @r10,r2 */
L8c61c118: *fpul=r[2];goto L8c61c11a; /* lds r2,FPUL */
L8c61c11a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c11c; /* float FPUL,fr3 */
L8c61c11c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c11e; /* fmul fr15,fr3 */
L8c61c11e: r[0]=24u;goto L8c61c120; /* mov #0x18,r0 */
L8c61c120: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c122; /* fmov.s fr3,@-r8 */
L8c61c122: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c124; /* fmov.s @(r0,r14),fr3 */
L8c61c124: r[0]=20u;goto L8c61c126; /* mov #0x14,r0 */
L8c61c126: r[2]=rd(m,bus,&io_status,0x8c61c214u,4);if(io_status)return io_status;goto L8c61c128; /* mov.l 0x8c61c214,r2 */
L8c61c128: r[10]+=4u;goto L8c61c12a; /* add #0x4,r10 */
L8c61c12a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c12c; /* fmov.s fr3,@-r8 */
L8c61c12c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c12e; /* fmov.s @(r0,r14),fr3 */
L8c61c12e: r[0]=16u;goto L8c61c130; /* mov #0x10,r0 */
L8c61c130: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c132; /* fmov.s fr3,@-r8 */
L8c61c132: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c134; /* fmov.s @(r0,r14),fr3 */
L8c61c134: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c136; /* fmov.s fr3,@-r8 */
L8c61c136: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c138; /* mov.l r2,@-r8 */
L8c61c138: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c13a; /* pref @r8 */
L8c61c13a: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c13c; /* mov.w @(0x2,r10),r0 */
L8c61c13c: r[8]+=56u;goto L8c61c13e; /* add #0x38,r8 */
L8c61c13e: r[2]=r[0];goto L8c61c140; /* mov r0,r2 */
L8c61c140: *fpul=r[2];goto L8c61c142; /* lds r2,FPUL */
L8c61c142: r[0]=24u;goto L8c61c144; /* mov #0x18,r0 */
L8c61c144: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c146; /* float FPUL,fr3 */
L8c61c146: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c148; /* fmul fr15,fr3 */
L8c61c148: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c14a; /* fmov.s fr3,@-r8 */
L8c61c14a: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c14c; /* mov.w @r10,r2 */
L8c61c14c: r[10]+=4u;goto L8c61c14e; /* add #0x4,r10 */
L8c61c14e: *fpul=r[2];goto L8c61c150; /* lds r2,FPUL */
L8c61c150: r[2]=rd(m,bus,&io_status,0x8c61c214u,4);if(io_status)return io_status;goto L8c61c152; /* mov.l 0x8c61c214,r2 */
L8c61c152: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c154; /* float FPUL,fr3 */
L8c61c154: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c156; /* fmul fr15,fr3 */
L8c61c156: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c158; /* fmov.s fr3,@-r8 */
L8c61c158: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c15a; /* fmov.s @(r0,r13),fr3 */
L8c61c15a: r[0]=20u;goto L8c61c15c; /* mov #0x14,r0 */
L8c61c15c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c15e; /* fmov.s fr3,@-r8 */
L8c61c15e: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c160; /* fmov.s @(r0,r13),fr3 */
L8c61c160: r[0]=16u;goto L8c61c162; /* mov #0x10,r0 */
L8c61c162: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c164; /* fmov.s fr3,@-r8 */
L8c61c164: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c166; /* fmov.s @(r0,r13),fr3 */
L8c61c166: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c168; /* fmov.s fr3,@-r8 */
L8c61c168: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c16a; /* mov.l r2,@-r8 */
L8c61c16a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c16c; /* pref @r8 */
L8c61c16c: r[8]+=60u;goto L8c61c16e; /* add #0x3c,r8 */
L8c61c16e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61c170; /* fmov.s fr4,@-r8 */
L8c61c170: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c172; /* mov.w @(0x2,r10),r0 */
L8c61c172: r[2]=r[0];goto L8c61c174; /* mov r0,r2 */
L8c61c174: *fpul=r[2];goto L8c61c176; /* lds r2,FPUL */
L8c61c176: r[0]=24u;goto L8c61c178; /* mov #0x18,r0 */
L8c61c178: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c17a; /* float FPUL,fr3 */
L8c61c17a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c17c; /* fmul fr15,fr3 */
L8c61c17c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c17e; /* fmov.s fr3,@-r8 */
L8c61c17e: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c180; /* mov.w @r10,r2 */
L8c61c180: r[10]+=4u;goto L8c61c182; /* add #0x4,r10 */
L8c61c182: *fpul=r[2];goto L8c61c184; /* lds r2,FPUL */
L8c61c184: r[2]=rd(m,bus,&io_status,0x8c61c214u,4);if(io_status)return io_status;goto L8c61c186; /* mov.l 0x8c61c214,r2 */
L8c61c186: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c188; /* float FPUL,fr3 */
L8c61c188: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c18a; /* fmul fr15,fr3 */
L8c61c18a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c18c; /* fmov.s fr3,@-r8 */
L8c61c18c: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c18e; /* fmov.s @(r0,r12),fr3 */
L8c61c18e: r[0]=20u;goto L8c61c190; /* mov #0x14,r0 */
L8c61c190: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c192; /* fmov.s fr3,@-r8 */
L8c61c192: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c194; /* fmov.s @(r0,r12),fr3 */
L8c61c194: r[0]=16u;goto L8c61c196; /* mov #0x10,r0 */
L8c61c196: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c198; /* fmov.s fr3,@-r8 */
L8c61c198: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c19a; /* fmov.s @(r0,r12),fr3 */
L8c61c19a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c19c; /* fmov.s fr3,@-r8 */
L8c61c19c: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c19e; /* mov.l r2,@-r8 */
L8c61c19e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c1a0; /* pref @r8 */
L8c61c1a0: r[8]+=60u;goto L8c61c1a2; /* add #0x3c,r8 */
L8c61c1a2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61c1a4; /* fmov.s fr4,@-r8 */
L8c61c1a4: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c1a6; /* mov.w @(0x2,r10),r0 */
L8c61c1a6: r[2]=r[0];goto L8c61c1a8; /* mov r0,r2 */
L8c61c1a8: *fpul=r[2];goto L8c61c1aa; /* lds r2,FPUL */
L8c61c1aa: r[0]=24u;goto L8c61c1ac; /* mov #0x18,r0 */
L8c61c1ac: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c1ae; /* float FPUL,fr3 */
L8c61c1ae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c1b0; /* fmul fr15,fr3 */
L8c61c1b0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c1b2; /* fmov.s fr3,@-r8 */
L8c61c1b2: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c1b4; /* mov.w @r10,r2 */
L8c61c1b4: r[10]+=4u;goto L8c61c1b6; /* add #0x4,r10 */
L8c61c1b6: *fpul=r[2];goto L8c61c1b8; /* lds r2,FPUL */
L8c61c1b8: r[2]=rd(m,bus,&io_status,0x8c61c218u,4);if(io_status)return io_status;goto L8c61c1ba; /* mov.l 0x8c61c218,r2 */
L8c61c1ba: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c1bc; /* float FPUL,fr3 */
L8c61c1bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c1be; /* fmul fr15,fr3 */
L8c61c1be: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c1c0; /* fmov.s fr3,@-r8 */
L8c61c1c0: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c1c2; /* fmov.s @(r0,r11),fr3 */
L8c61c1c2: r[0]=20u;goto L8c61c1c4; /* mov #0x14,r0 */
L8c61c1c4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c1c6; /* fmov.s fr3,@-r8 */
L8c61c1c6: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c1c8; /* fmov.s @(r0,r11),fr3 */
L8c61c1c8: r[0]=16u;goto L8c61c1ca; /* mov #0x10,r0 */
L8c61c1ca: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c1cc; /* fmov.s fr3,@-r8 */
L8c61c1cc: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c1ce; /* fmov.s @(r0,r11),fr3 */
L8c61c1ce: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c1d0; /* fmov.s fr3,@-r8 */
L8c61c1d0: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c1d2; /* mov.l r2,@-r8 */
L8c61c1d2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c1d4; /* pref @r8 */
L8c61c1d4: r[0]=rd(m,bus,&io_status,0x8c61c21cu,4);if(io_status)return io_status;goto L8c61c1d6; /* mov.l 0x8c61c21c,r0 */
L8c61c1d6: r[8]+=32u;goto L8c61c1d8; /* add #0x20,r8 */
L8c61c1d8: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61c1da; /* mov.l @r0,r2 */
L8c61c1da: r[2]+=4u;goto L8c61c1dc; /* add #0x4,r2 */
L8c61c1dc: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61c1de; /* mov.l r2,@r0 */
L8c61c1de: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61c1e0; /* mov.l @(0x4,r15),r3 */
L8c61c1e0: r[3]+=1u;goto L8c61c1e2; /* add #0x1,r3 */
L8c61c1e2: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61c1e4; /* mov.l r3,@(0x4,r15) */
L8c61c1e4: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61c1e6; /* mov.l @r15,r2 */
L8c61c1e6: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61c1e8; /* mov.l @(0x4,r15),r3 */
L8c61c1e8: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61c1ea; /* mov.w @(0x2,r2),r0 */
L8c61c1ea: r[0]=r[0]&65535u;goto L8c61c1ec; /* extu.w r0,r0 */
L8c61c1ec: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c61c1ee; /* cmp/ge r0,r3 */
L8c61c1ee: if((*sr&1u))goto L8c61c1f4;goto L8c61c1f0; /* bt 0x8c61c1f4 */
L8c61c1f0: goto L8c61c044; /* bra 0x8c61c044 */
L8c61c1f4: goto L8c61c46a; /* bra 0x8c61c46a */
L8c61c1f8: r[11]=2u;goto L8c61c1fa; /* mov #0x2,r11 */
L8c61c1fa: r[12]=r[4];goto L8c61c41a; /* bra 0x8c61c41a */
L8c61c224: r[14]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c226; /* mov.w @r9+,r14 */
L8c61c226: r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61c228; /* mov.l @r13,r3 */
L8c61c228: r[14]=r[14]&65535u;goto L8c61c22a; /* extu.w r14,r14 */
L8c61c22a: r[3]+=r[14];goto L8c61c22c; /* add r14,r3 */
L8c61c22c: wr(m,bus,&io_status,r[13],r[3],4);if(io_status)return io_status;goto L8c61c22e; /* mov.l r3,@r13 */
L8c61c22e: r[0]=sx(m,bus,&io_status,r[9]+0x2u,2);if(io_status)return io_status;goto L8c61c230; /* mov.w @(0x2,r9),r0 */
L8c61c230: r[6]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;goto L8c61c232; /* mov.w @r9,r6 */
L8c61c232: r[3]=rd(m,bus,&io_status,0x8c61c48cu,4);if(io_status)return io_status;goto L8c61c234; /* mov.l 0x8c61c48c,r3 */
L8c61c234: r[4]=r[0]&65535u;goto L8c61c236; /* extu.w r0,r4 */
L8c61c236: r[0]=sx(m,bus,&io_status,r[9]+0x4u,2);if(io_status)return io_status;goto L8c61c238; /* mov.w @(0x4,r9),r0 */
L8c61c238: r[4]<<=2;goto L8c61c23a; /* shll2 r4 */
L8c61c23a: r[6]=r[6]&65535u;goto L8c61c23c; /* extu.w r6,r6 */
L8c61c23c: r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61c23e; /* mov.l @r3,r7 */
L8c61c23e: r[4]<<=2;goto L8c61c240; /* shll2 r4 */
L8c61c240: r[6]<<=2;goto L8c61c242; /* shll2 r6 */
L8c61c242: r[5]=r[0]&65535u;goto L8c61c244; /* extu.w r0,r5 */
L8c61c244: r[6]<<=2;goto L8c61c246; /* shll2 r6 */
L8c61c246: r[0]=16u;goto L8c61c248; /* mov #0x10,r0 */
L8c61c248: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61c24a; /* shll r6 */
L8c61c24a: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61c24c; /* shll r4 */
L8c61c24c: r[4]+=r[7];goto L8c61c24e; /* add r7,r4 */
L8c61c24e: fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c250; /* fmov.s @(r0,r4),fr5 */
L8c61c250: r[6]+=r[7];goto L8c61c252; /* add r7,r6 */
L8c61c252: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61c254; /* fmov.s @(r0,r6),fr3 */
L8c61c254: r[5]<<=2;goto L8c61c256; /* shll2 r5 */
L8c61c256: fcopy(f,2u,5u);goto L8c61c258; /* fmov fr5,fr2 */
L8c61c258: r[0]=20u;goto L8c61c25a; /* mov #0x14,r0 */
L8c61c25a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61c25c; /* fsub fr3,fr2 */
L8c61c25c: r[5]<<=2;goto L8c61c25e; /* shll2 r5 */
L8c61c25e: fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c260; /* fmov.s @(r0,r4),fr4 */
L8c61c260: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61c262; /* shll r5 */
L8c61c262: r[5]+=r[7];goto L8c61c264; /* add r7,r5 */
L8c61c264: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61c266; /* fmov.s @(r0,r5),fr3 */
L8c61c266: fcopy(f,1u,4u);goto L8c61c268; /* fmov fr4,fr1 */
L8c61c268: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c26a; /* fsub fr4,fr3 */
L8c61c26a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c26c; /* fmul fr3,fr2 */
L8c61c26c: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61c26e; /* fmov.s @(r0,r6),fr3 */
L8c61c26e: r[0]=16u;goto L8c61c270; /* mov #0x10,r0 */
L8c61c270: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61c272; /* fsub fr3,fr1 */
L8c61c272: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61c274; /* fmov.s @(r0,r5),fr3 */
L8c61c274: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c276; /* fsub fr5,fr3 */
L8c61c276: fcopy(f,4u,2u);goto L8c61c278; /* fmov fr2,fr4 */
L8c61c278: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c27a; /* fmul fr3,fr1 */
L8c61c27a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61c27c; /* fldi0 fr3 */
L8c61c27c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c27e; /* fsub fr1,fr4 */
L8c61c27e: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61c280; /* fcmp/gt fr3,fr4 */
L8c61c280: if(!(*sr&1u))goto L8c61c28e;goto L8c61c282; /* bf 0x8c61c28e */
L8c61c282: r[1]=r[14];goto L8c61c284; /* mov r14,r1 */
L8c61c284: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c61c286; /* shll r1 */
L8c61c286: r[14]<<=2;goto L8c61c288; /* shll2 r14 */
L8c61c288: r[9]+=r[1];goto L8c61c28a; /* add r1,r9 */
L8c61c28a: r[10]+=r[14];goto L8c61c418; /* bra 0x8c61c418 */
L8c61c28e: r[0]=8u;goto L8c61c290; /* mov #0x8,r0 */
L8c61c290: r[3]=rd(m,bus,&io_status,0x8c61c490u,4);if(io_status)return io_status;goto L8c61c292; /* mov.l 0x8c61c490,r3 */
L8c61c292: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61c294; /* mov.l @r3,r2 */
L8c61c294: r[2]+=r[14];goto L8c61c296; /* add r14,r2 */
L8c61c296: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61c298; /* mov.l r2,@r3 */
L8c61c298: fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61c29a; /* fmov.s @r4,fr3 */
L8c61c29a: fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c61c29c; /* fmov.s @r6,fr2 */
L8c61c29c: r[3]=rd(m,bus,&io_status,0x8c61c494u,4);if(io_status)return io_status;goto L8c61c29e; /* mov.l 0x8c61c494,r3 */
L8c61c29e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c2a0; /* fadd fr3,fr2 */
L8c61c2a0: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61c2a2; /* fmov.s @r5,fr3 */
L8c61c2a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c2a4; /* fadd fr3,fr2 */
L8c61c2a4: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c2a6; /* fmov fr2,@(r0,r15) */
L8c61c2a6: r[0]=4u;goto L8c61c2a8; /* mov #0x4,r0 */
L8c61c2a8: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61c2aa; /* fmov.s @(r0,r6),fr2 */
L8c61c2aa: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c2ac; /* fmov.s @(r0,r4),fr3 */
L8c61c2ac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c2ae; /* fadd fr3,fr2 */
L8c61c2ae: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61c2b0; /* fmov.s @(r0,r5),fr3 */
L8c61c2b0: r[0]=12u;goto L8c61c2b2; /* mov #0xc,r0 */
L8c61c2b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c2b4; /* fadd fr3,fr2 */
L8c61c2b4: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c2b6; /* fmov fr2,@(r0,r15) */
L8c61c2b6: r[0]=8u;goto L8c61c2b8; /* mov #0x8,r0 */
L8c61c2b8: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c2ba; /* fmov.s @(r0,r4),fr3 */
L8c61c2ba: r[4]=r[15];goto L8c61c2bc; /* mov r15,r4 */
L8c61c2bc: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61c2be; /* fmov.s @(r0,r6),fr2 */
L8c61c2be: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c2c0; /* fadd fr3,fr2 */
L8c61c2c0: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61c2c2; /* fmov.s @(r0,r5),fr3 */
L8c61c2c2: r[0]=16u;goto L8c61c2c4; /* mov #0x10,r0 */
L8c61c2c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c2c6; /* fadd fr3,fr2 */
L8c61c2c6: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c2c8; /* fmov fr2,@(r0,r15) */
L8c61c2c8: target=r[3];*pr=0x8c61c2ccu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61c2cc; /* jsr @r3 */
L8c61c2cc: r[3]=rd(m,bus,&io_status,0x8c61c498u,4);if(io_status)return io_status;goto L8c61c2ce; /* mov.l 0x8c61c498,r3 */
L8c61c2ce: r[0]=8u;goto L8c61c2d0; /* mov #0x8,r0 */
L8c61c2d0: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c2d2; /* fmov.s @(r0,r15),fr3 */
L8c61c2d2: r[0]=12u;goto L8c61c2d4; /* mov #0xc,r0 */
L8c61c2d4: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61c2d6; /* fmov.s @r3,fr2 */
L8c61c2d6: r[3]+=8u;goto L8c61c2d8; /* add #0x8,r3 */
L8c61c2d8: r[2]=rd(m,bus,&io_status,0x8c61c49cu,4);if(io_status)return io_status;goto L8c61c2da; /* mov.l 0x8c61c49c,r2 */
L8c61c2da: r[8]+=24u;goto L8c61c2dc; /* add #0x18,r8 */
L8c61c2dc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c2de; /* fmul fr3,fr2 */
L8c61c2de: r[1]=rd(m,bus,&io_status,0x8c61c4a0u,4);if(io_status)return io_status;goto L8c61c2e0; /* mov.l 0x8c61c4a0,r1 */
L8c61c2e0: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61c2e2; /* fmov.s @r2,fr3 */
L8c61c2e2: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61c2e4; /* fmov.s @r1,fr1 */
L8c61c2e4: r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c2e6; /* mov.w @r9+,r4 */
L8c61c2e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c2e8; /* fsub fr2,fr3 */
L8c61c2e8: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c2ea; /* fmov.s @(r0,r15),fr2 */
L8c61c2ea: r[0]=16u;goto L8c61c2ec; /* mov #0x10,r0 */
L8c61c2ec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c2ee; /* fmul fr2,fr1 */
L8c61c2ee: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c2f0; /* fmov.s @(r0,r15),fr2 */
L8c61c2f0: r[0]=rd(m,bus,&io_status,0x8c61c48cu,4);if(io_status)return io_status;goto L8c61c2f2; /* mov.l 0x8c61c48c,r0 */
L8c61c2f2: r[4]=r[4]&65535u;goto L8c61c2f4; /* extu.w r4,r4 */
L8c61c2f4: r[4]<<=2;goto L8c61c2f6; /* shll2 r4 */
L8c61c2f6: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61c2f8; /* mov.l @r0,r2 */
L8c61c2f8: r[4]<<=2;goto L8c61c2fa; /* shll2 r4 */
L8c61c2fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c2fc; /* fsub fr1,fr3 */
L8c61c2fc: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61c2fe; /* fmov.s @r3,fr1 */
L8c61c2fe: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c300; /* mov.w @(0x2,r10),r0 */
L8c61c300: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61c302; /* shll r4 */
L8c61c302: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c304; /* fmul fr2,fr1 */
L8c61c304: r[4]+=r[2];goto L8c61c306; /* add r2,r4 */
L8c61c306: r[3]=r[0];goto L8c61c308; /* mov r0,r3 */
L8c61c308: fcopy(f,4u,3u);goto L8c61c30a; /* fmov fr3,fr4 */
L8c61c30a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c30c; /* fsub fr1,fr4 */
L8c61c30c: *fpul=r[3];goto L8c61c30e; /* lds r3,FPUL */
L8c61c30e: r[0]=24u;goto L8c61c310; /* mov #0x18,r0 */
L8c61c310: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c312; /* float FPUL,fr3 */
L8c61c312: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c314; /* fmul fr15,fr3 */
L8c61c314: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c316; /* fmov.s fr3,@-r8 */
L8c61c316: r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c318; /* mov.w @r10,r3 */
L8c61c318: r[10]+=4u;goto L8c61c31a; /* add #0x4,r10 */
L8c61c31a: *fpul=r[3];goto L8c61c31c; /* lds r3,FPUL */
L8c61c31c: r[3]=rd(m,bus,&io_status,0x8c61c4a4u,4);if(io_status)return io_status;goto L8c61c31e; /* mov.l 0x8c61c4a4,r3 */
L8c61c31e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c320; /* float FPUL,fr3 */
L8c61c320: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c322; /* fmul fr15,fr3 */
L8c61c322: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c324; /* fmov.s fr3,@-r8 */
L8c61c324: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c326; /* fmov.s @(r0,r4),fr3 */
L8c61c326: r[0]=20u;goto L8c61c328; /* mov #0x14,r0 */
L8c61c328: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c32a; /* fmov.s fr3,@-r8 */
L8c61c32a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c32c; /* fmov.s @(r0,r4),fr3 */
L8c61c32c: r[0]=16u;goto L8c61c32e; /* mov #0x10,r0 */
L8c61c32e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c330; /* fmov.s fr3,@-r8 */
L8c61c330: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c332; /* fmov.s @(r0,r4),fr3 */
L8c61c332: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c334; /* fmov.s fr3,@-r8 */
L8c61c334: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61c336; /* mov.l r3,@-r8 */
L8c61c336: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c338; /* pref @r8 */
L8c61c338: r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c33a; /* mov.w @r9+,r4 */
L8c61c33a: r[8]+=56u;goto L8c61c33c; /* add #0x38,r8 */
L8c61c33c: r[2]=rd(m,bus,&io_status,0x8c61c48cu,4);if(io_status)return io_status;goto L8c61c33e; /* mov.l 0x8c61c48c,r2 */
L8c61c33e: r[4]=r[4]&65535u;goto L8c61c340; /* extu.w r4,r4 */
L8c61c340: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c342; /* mov.w @(0x2,r10),r0 */
L8c61c342: r[4]<<=2;goto L8c61c344; /* shll2 r4 */
L8c61c344: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61c346; /* mov.l @r2,r3 */
L8c61c346: r[4]<<=2;goto L8c61c348; /* shll2 r4 */
L8c61c348: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61c34a; /* shll r4 */
L8c61c34a: r[4]+=r[3];goto L8c61c34c; /* add r3,r4 */
L8c61c34c: r[3]=r[0];goto L8c61c34e; /* mov r0,r3 */
L8c61c34e: *fpul=r[3];goto L8c61c350; /* lds r3,FPUL */
L8c61c350: r[0]=24u;goto L8c61c352; /* mov #0x18,r0 */
L8c61c352: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c354; /* float FPUL,fr3 */
L8c61c354: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c356; /* fmul fr15,fr3 */
L8c61c356: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c358; /* fmov.s fr3,@-r8 */
L8c61c358: r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c35a; /* mov.w @r10,r3 */
L8c61c35a: r[10]+=4u;goto L8c61c35c; /* add #0x4,r10 */
L8c61c35c: *fpul=r[3];goto L8c61c35e; /* lds r3,FPUL */
L8c61c35e: r[3]=rd(m,bus,&io_status,0x8c61c4a4u,4);if(io_status)return io_status;goto L8c61c360; /* mov.l 0x8c61c4a4,r3 */
L8c61c360: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c362; /* float FPUL,fr3 */
L8c61c362: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c364; /* fmul fr15,fr3 */
L8c61c364: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c366; /* fmov.s fr3,@-r8 */
L8c61c366: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c368; /* fmov.s @(r0,r4),fr3 */
L8c61c368: r[0]=20u;goto L8c61c36a; /* mov #0x14,r0 */
L8c61c36a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c36c; /* fmov.s fr3,@-r8 */
L8c61c36c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c36e; /* fmov.s @(r0,r4),fr3 */
L8c61c36e: r[0]=16u;goto L8c61c370; /* mov #0x10,r0 */
L8c61c370: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c372; /* fmov.s fr3,@-r8 */
L8c61c372: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c374; /* fmov.s @(r0,r4),fr3 */
L8c61c374: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c376; /* fmov.s fr3,@-r8 */
L8c61c376: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61c378; /* mov.l r3,@-r8 */
L8c61c378: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c37a; /* pref @r8 */
L8c61c37a: r[6]=r[14];goto L8c61c37c; /* mov r14,r6 */
L8c61c37c: r[6]+=4294967295u;goto L8c61c37e; /* add #-0x1,r6 */
L8c61c37e: r[5]=r[11];goto L8c61c380; /* mov r11,r5 */
L8c61c380: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61c382; /* cmp/ge r6,r5 */
L8c61c382: take=(*sr&1u);r[8]+=32u;if(take)goto L8c61c3d2;goto L8c61c386; /* bt/s 0x8c61c3d2 */
L8c61c386: r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c388; /* mov.w @r9+,r4 */
L8c61c388: r[8]+=28u;goto L8c61c38a; /* add #0x1c,r8 */
L8c61c38a: r[2]=rd(m,bus,&io_status,0x8c61c48cu,4);if(io_status)return io_status;goto L8c61c38c; /* mov.l 0x8c61c48c,r2 */
L8c61c38c: r[4]=r[4]&65535u;goto L8c61c38e; /* extu.w r4,r4 */
L8c61c38e: r[4]<<=2;goto L8c61c390; /* shll2 r4 */
L8c61c390: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61c392; /* mov.l @r2,r3 */
L8c61c392: r[4]<<=2;goto L8c61c394; /* shll2 r4 */
L8c61c394: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61c396; /* fmov.s fr4,@-r8 */
L8c61c396: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c398; /* mov.w @(0x2,r10),r0 */
L8c61c398: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61c39a; /* shll r4 */
L8c61c39a: r[4]+=r[3];goto L8c61c39c; /* add r3,r4 */
L8c61c39c: r[3]=r[0];goto L8c61c39e; /* mov r0,r3 */
L8c61c39e: *fpul=r[3];goto L8c61c3a0; /* lds r3,FPUL */
L8c61c3a0: r[0]=24u;goto L8c61c3a2; /* mov #0x18,r0 */
L8c61c3a2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c3a4; /* float FPUL,fr3 */
L8c61c3a4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c3a6; /* fmul fr15,fr3 */
L8c61c3a6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c3a8; /* fmov.s fr3,@-r8 */
L8c61c3a8: r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c3aa; /* mov.w @r10,r3 */
L8c61c3aa: r[10]+=4u;goto L8c61c3ac; /* add #0x4,r10 */
L8c61c3ac: *fpul=r[3];goto L8c61c3ae; /* lds r3,FPUL */
L8c61c3ae: r[3]=rd(m,bus,&io_status,0x8c61c4a4u,4);if(io_status)return io_status;goto L8c61c3b0; /* mov.l 0x8c61c4a4,r3 */
L8c61c3b0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c3b2; /* float FPUL,fr3 */
L8c61c3b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c3b4; /* fmul fr15,fr3 */
L8c61c3b4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c3b6; /* fmov.s fr3,@-r8 */
L8c61c3b6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c3b8; /* fmov.s @(r0,r4),fr3 */
L8c61c3b8: r[0]=20u;goto L8c61c3ba; /* mov #0x14,r0 */
L8c61c3ba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c3bc; /* fmov.s fr3,@-r8 */
L8c61c3bc: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c3be; /* fmov.s @(r0,r4),fr3 */
L8c61c3be: r[0]=16u;goto L8c61c3c0; /* mov #0x10,r0 */
L8c61c3c0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c3c2; /* fmov.s fr3,@-r8 */
L8c61c3c2: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c3c4; /* fmov.s @(r0,r4),fr3 */
L8c61c3c4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c3c6; /* fmov.s fr3,@-r8 */
L8c61c3c6: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61c3c8; /* mov.l r3,@-r8 */
L8c61c3c8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c3ca; /* pref @r8 */
L8c61c3ca: r[5]+=1u;goto L8c61c3cc; /* add #0x1,r5 */
L8c61c3cc: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61c3ce; /* cmp/ge r6,r5 */
L8c61c3ce: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61c386;goto L8c61c3d2; /* bf/s 0x8c61c386 */
L8c61c3d2: r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c3d4; /* mov.w @r9+,r4 */
L8c61c3d4: r[8]+=28u;goto L8c61c3d6; /* add #0x1c,r8 */
L8c61c3d6: r[2]=rd(m,bus,&io_status,0x8c61c48cu,4);if(io_status)return io_status;goto L8c61c3d8; /* mov.l 0x8c61c48c,r2 */
L8c61c3d8: r[4]=r[4]&65535u;goto L8c61c3da; /* extu.w r4,r4 */
L8c61c3da: r[4]<<=2;goto L8c61c3dc; /* shll2 r4 */
L8c61c3dc: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61c3de; /* mov.l @r2,r3 */
L8c61c3de: r[4]<<=2;goto L8c61c3e0; /* shll2 r4 */
L8c61c3e0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61c3e2; /* fmov.s fr4,@-r8 */
L8c61c3e2: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c3e4; /* mov.w @(0x2,r10),r0 */
L8c61c3e4: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61c3e6; /* shll r4 */
L8c61c3e6: r[4]+=r[3];goto L8c61c3e8; /* add r3,r4 */
L8c61c3e8: r[3]=r[0];goto L8c61c3ea; /* mov r0,r3 */
L8c61c3ea: *fpul=r[3];goto L8c61c3ec; /* lds r3,FPUL */
L8c61c3ec: r[0]=24u;goto L8c61c3ee; /* mov #0x18,r0 */
L8c61c3ee: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c3f0; /* float FPUL,fr3 */
L8c61c3f0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c3f2; /* fmul fr15,fr3 */
L8c61c3f2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c3f4; /* fmov.s fr3,@-r8 */
L8c61c3f4: r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c3f6; /* mov.w @r10,r3 */
L8c61c3f6: r[10]+=4u;goto L8c61c3f8; /* add #0x4,r10 */
L8c61c3f8: *fpul=r[3];goto L8c61c3fa; /* lds r3,FPUL */
L8c61c3fa: r[3]=rd(m,bus,&io_status,0x8c61c4a8u,4);if(io_status)return io_status;goto L8c61c3fc; /* mov.l 0x8c61c4a8,r3 */
L8c61c3fc: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c3fe; /* float FPUL,fr3 */
L8c61c3fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c400; /* fmul fr15,fr3 */
L8c61c400: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c402; /* fmov.s fr3,@-r8 */
L8c61c402: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c404; /* fmov.s @(r0,r4),fr3 */
L8c61c404: r[0]=20u;goto L8c61c406; /* mov #0x14,r0 */
L8c61c406: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c408; /* fmov.s fr3,@-r8 */
L8c61c408: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c40a; /* fmov.s @(r0,r4),fr3 */
L8c61c40a: r[0]=16u;goto L8c61c40c; /* mov #0x10,r0 */
L8c61c40c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c40e; /* fmov.s fr3,@-r8 */
L8c61c40e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61c410; /* fmov.s @(r0,r4),fr3 */
L8c61c410: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c412; /* fmov.s fr3,@-r8 */
L8c61c412: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61c414; /* mov.l r3,@-r8 */
L8c61c414: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c416; /* pref @r8 */
L8c61c416: r[8]+=32u;goto L8c61c418; /* add #0x20,r8 */
L8c61c418: r[12]+=1u;goto L8c61c41a; /* add #0x1,r12 */
L8c61c41a: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61c41c; /* mov.l @r15,r2 */
L8c61c41c: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61c41e; /* mov.w @(0x2,r2),r0 */
L8c61c41e: r[0]=r[0]&65535u;goto L8c61c420; /* extu.w r0,r0 */
L8c61c420: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[0]);goto L8c61c422; /* cmp/ge r0,r12 */
L8c61c422: if((*sr&1u))goto L8c61c428;goto L8c61c424; /* bt 0x8c61c428 */
L8c61c424: goto L8c61c224; /* bra 0x8c61c224 */
L8c61c428: goto L8c61c46a; /* bra 0x8c61c46a */
L8c61c42c: r[2]=rd(m,bus,&io_status,0x8c61c4acu,4);if(io_status)return io_status;goto L8c61c42e; /* mov.l 0x8c61c4ac,r2 */
L8c61c42e: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61c430; /* mov.l @r15,r1 */
L8c61c430: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61c432; /* mov.l r1,@r2 */
L8c61c432: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61c434; /* frchg */
L8c61c434: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61c436; /* fmov.s fr0,@-r15 */
L8c61c436: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61c438; /* fmov.s fr1,@-r15 */
L8c61c438: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61c43a; /* fmov.s fr2,@-r15 */
L8c61c43a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61c43c; /* fmov.s fr3,@-r15 */
L8c61c43c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61c43e; /* fmov.s fr4,@-r15 */
L8c61c43e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61c440; /* fmov.s fr5,@-r15 */
L8c61c440: r[0]=rd(m,bus,&io_status,0x8c61c480u,4);if(io_status)return io_status;goto L8c61c442; /* mov.l 0x8c61c480,r0 */
L8c61c442: fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c444; /* fmov.s @r0+,fr0 */
L8c61c444: fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c446; /* fmov.s @r0+,fr1 */
L8c61c446: fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c61c448; /* fmov.s @r0,fr2 */
L8c61c448: r[0]=rd(m,bus,&io_status,0x8c61c484u,4);if(io_status)return io_status;goto L8c61c44a; /* mov.l 0x8c61c484,r0 */
L8c61c44a: fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c61c44c; /* fmov.s @r0,fr3 */
L8c61c44c: r[0]=0x8c61c488u;goto L8c61c44e; /* mova 0x8c61c488,r0 */
L8c61c44e: fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c61c450; /* fmov.s @r0,fr4 */
L8c61c450: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61c452; /* frchg */
L8c61c452: r[2]=rd(m,bus,&io_status,0x8c61c4b0u,4);if(io_status)return io_status;goto L8c61c454; /* mov.l 0x8c61c4b0,r2 */
L8c61c454: r[3]=rd(m,bus,&io_status,0x8c61c4acu,4);if(io_status)return io_status;goto L8c61c456; /* mov.l 0x8c61c4ac,r3 */
L8c61c456: target=r[2];*pr=0x8c61c45au;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61c45a; /* jsr @r2 */
L8c61c45a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61c45c; /* frchg */
L8c61c45c: fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c45e; /* fmov.s @r15+,fr5 */
L8c61c45e: fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c460; /* fmov.s @r15+,fr4 */
L8c61c460: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c462; /* fmov.s @r15+,fr3 */
L8c61c462: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c464; /* fmov.s @r15+,fr2 */
L8c61c464: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c466; /* fmov.s @r15+,fr1 */
L8c61c466: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c468; /* fmov.s @r15+,fr0 */
L8c61c468: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61c46a; /* frchg */
L8c61c46a: r[15]+=20u;goto L8c61c46c; /* add #0x14,r15 */
L8c61c46c: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61c46e; /* lds.l @r15+,PR */
L8c61c46e: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61c470; /* fmov.s @r15+,fr15 */
L8c61c470: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61c472; /* mov.l @r15+,r9 */
L8c61c472: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61c474; /* mov.l @r15+,r10 */
L8c61c474: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61c476; /* mov.l @r15+,r11 */
L8c61c476: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61c478; /* mov.l @r15+,r12 */
L8c61c478: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61c47a; /* mov.l @r15+,r13 */
L8c61c47a: if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
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
L8c61c4b4: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61c4b6; /* mov.l r14,@-r15 */
L8c61c4b6: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61c4b8; /* mov.l r13,@-r15 */
L8c61c4b8: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61c4ba; /* mov.l r12,@-r15 */
L8c61c4ba: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61c4bc; /* mov.l r11,@-r15 */
L8c61c4bc: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61c4be; /* mov.l r10,@-r15 */
L8c61c4be: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61c4c0; /* mov.l r9,@-r15 */
L8c61c4c0: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61c4c2; /* fmov.s fr15,@-r15 */
L8c61c4c2: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61c4c4; /* fmov.s fr14,@-r15 */
L8c61c4c4: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61c4c6; /* sts.l PR,@-r15 */
L8c61c4c6: r[15]+=4294967256u;goto L8c61c4c8; /* add #-0x28,r15 */
L8c61c4c8: r[0]=0x8c61c5a4u;goto L8c61c4ca; /* mova 0x8c61c5a4,r0 */
L8c61c4ca: r[3]=rd(m,bus,&io_status,0x8c61c5a8u,4);if(io_status)return io_status;goto L8c61c4cc; /* mov.l 0x8c61c5a8,r3 */
L8c61c4cc: fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c61c4ce; /* fmov.s @r0,fr14 */
L8c61c4ce: wr(m,bus,&io_status,r[15]+0xcu,r[4],4);if(io_status)return io_status;goto L8c61c4d0; /* mov.l r4,@(0xc,r15) */
L8c61c4d0: r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61c4d2; /* mov.w @r4,r0 */
L8c61c4d2: r[10]=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;goto L8c61c4d4; /* mov.l @(0x14,r4),r10 */
L8c61c4d4: r[0]=r[0]&65535u;goto L8c61c4d6; /* extu.w r0,r0 */
L8c61c4d6: r[0]&=r[3];goto L8c61c4d8; /* and r3,r0 */
L8c61c4d8: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61c4da; /* cmp/eq #0x0,r0 */
L8c61c4da: take=(*sr&1u);r[9]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;if(take)goto L8c61c500;goto L8c61c4de; /* bt/s 0x8c61c500 */
L8c61c4de: r[1]=sx(m,bus,&io_status,0x8c61c5a0u,2);if(io_status)return io_status;goto L8c61c4e0; /* mov.w 0x8c61c5a0,r1 */
L8c61c4e0: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61c4e2; /* cmp/eq r1,r0 */
L8c61c4e2: if(!(*sr&1u))goto L8c61c4e8;goto L8c61c4e4; /* bf 0x8c61c4e8 */
L8c61c4e4: goto L8c61c7a8; /* bra 0x8c61c7a8 */
L8c61c4e8: r[1]=rd(m,bus,&io_status,0x8c61c5acu,4);if(io_status)return io_status;goto L8c61c4ea; /* mov.l 0x8c61c5ac,r1 */
L8c61c4ea: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61c4ec; /* cmp/eq r1,r0 */
L8c61c4ec: if(!(*sr&1u))goto L8c61c4f2;goto L8c61c4ee; /* bf 0x8c61c4f2 */
L8c61c4ee: goto L8c61cb0e; /* bra 0x8c61cb0e */
L8c61c4f2: r[1]=rd(m,bus,&io_status,0x8c61c5a8u,4);if(io_status)return io_status;goto L8c61c4f4; /* mov.l 0x8c61c5a8,r1 */
L8c61c4f4: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61c4f6; /* cmp/eq r1,r0 */
L8c61c4f6: if(!(*sr&1u))goto L8c61c4fc;goto L8c61c4f8; /* bf 0x8c61c4fc */
L8c61c4f8: goto L8c61cf0e; /* bra 0x8c61cf0e */
L8c61c4fc: goto L8c61cf4c; /* bra 0x8c61cf4c */
L8c61c500: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61c502; /* mov.l @(0xc,r15),r2 */
L8c61c502: r[11]=0u;goto L8c61c504; /* mov #0x0,r11 */
L8c61c504: r[1]=rd(m,bus,&io_status,0x8c61c5b0u,4);if(io_status)return io_status;goto L8c61c506; /* mov.l 0x8c61c5b0,r1 */
L8c61c506: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61c508; /* mov.w @(0x2,r2),r0 */
L8c61c508: r[3]=rd(m,bus,&io_status,0x8c61c5b8u,4);if(io_status)return io_status;goto L8c61c50a; /* mov.l 0x8c61c5b8,r3 */
L8c61c50a: r[0]=r[0]&65535u;goto L8c61c50c; /* extu.w r0,r0 */
L8c61c50c: r[2]=r[0];goto L8c61c50e; /* mov r0,r2 */
L8c61c50e: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61c510; /* shll r0 */
L8c61c510: r[0]+=r[2];goto L8c61c512; /* add r2,r0 */
L8c61c512: r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61c514; /* mov.l @r1,r2 */
L8c61c514: r[2]+=r[0];goto L8c61c516; /* add r0,r2 */
L8c61c516: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61c518; /* mov.l r2,@r1 */
L8c61c518: r[1]=rd(m,bus,&io_status,0x8c61c5b4u,4);if(io_status)return io_status;goto L8c61c51a; /* mov.l 0x8c61c5b4,r1 */
L8c61c51a: wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c61c51c; /* mov.l r1,@r15 */
L8c61c51c: r[1]+=8u;goto L8c61c51e; /* add #0x8,r1 */
L8c61c51e: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61c520; /* mov.l r3,@(0x4,r15) */
L8c61c520: wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c61c796; /* bra 0x8c61c796 */
L8c61c524: r[14]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c526; /* mov.w @r9+,r14 */
L8c61c526: r[2]=rd(m,bus,&io_status,0x8c61c5bcu,4);if(io_status)return io_status;goto L8c61c528; /* mov.l 0x8c61c5bc,r2 */
L8c61c528: r[12]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c52a; /* mov.w @r9+,r12 */
L8c61c52a: r[14]=r[14]&65535u;goto L8c61c52c; /* extu.w r14,r14 */
L8c61c52c: r[14]<<=2;goto L8c61c52e; /* shll2 r14 */
L8c61c52e: r[5]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61c530; /* mov.l @r2,r5 */
L8c61c530: r[13]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c532; /* mov.w @r9+,r13 */
L8c61c532: r[12]=r[12]&65535u;goto L8c61c534; /* extu.w r12,r12 */
L8c61c534: r[12]<<=2;goto L8c61c536; /* shll2 r12 */
L8c61c536: r[13]=r[13]&65535u;goto L8c61c538; /* extu.w r13,r13 */
L8c61c538: r[12]<<=2;goto L8c61c53a; /* shll2 r12 */
L8c61c53a: r[13]<<=2;goto L8c61c53c; /* shll2 r13 */
L8c61c53c: r[14]<<=2;goto L8c61c53e; /* shll2 r14 */
L8c61c53e: r[13]<<=2;goto L8c61c540; /* shll2 r13 */
L8c61c540: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61c542; /* shll r14 */
L8c61c542: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61c544; /* shll r12 */
L8c61c544: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61c546; /* shll r13 */
L8c61c546: r[14]+=r[5];goto L8c61c548; /* add r5,r14 */
L8c61c548: r[12]+=r[5];goto L8c61c54a; /* add r5,r12 */
L8c61c54a: r[4]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61c54c; /* mov.l @(0xc,r14),r4 */
L8c61c54c: r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61c54e; /* mov.l @(0xc,r12),r3 */
L8c61c54e: r[13]+=r[5];goto L8c61c550; /* add r5,r13 */
L8c61c550: r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61c552; /* mov.l @(0xc,r13),r1 */
L8c61c552: r[4]&=r[3];goto L8c61c554; /* and r3,r4 */
L8c61c554: r[3]=63u;goto L8c61c556; /* mov #0x3f,r3 */
L8c61c556: r[4]&=r[1];goto L8c61c558; /* and r1,r4 */
L8c61c558: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61c55a; /* tst r3,r4 */
L8c61c55a: if(!(*sr&1u))goto L8c61c59c;goto L8c61c55c; /* bf 0x8c61c59c */
L8c61c55c: r[4]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61c55e; /* mov.l @(0xc,r14),r4 */
L8c61c55e: r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61c560; /* mov.l @(0xc,r12),r3 */
L8c61c560: r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61c562; /* mov.l @(0xc,r13),r2 */
L8c61c562: r[4]|=r[3];goto L8c61c564; /* or r3,r4 */
L8c61c564: r[3]=1u;goto L8c61c566; /* mov #0x1,r3 */
L8c61c566: r[4]|=r[2];goto L8c61c568; /* or r2,r4 */
L8c61c568: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61c56a; /* tst r3,r4 */
L8c61c56a: if((*sr&1u))goto L8c61c570;goto L8c61c56c; /* bt 0x8c61c570 */
L8c61c56c: goto L8c61c6c4; /* bra 0x8c61c6c4 */
L8c61c570: r[0]=16u;goto L8c61c572; /* mov #0x10,r0 */
L8c61c572: fload(m,bus,&io_status,f,4u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c574; /* fmov.s @(r0,r12),fr4 */
L8c61c574: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c576; /* fmov.s @(r0,r14),fr3 */
L8c61c576: r[0]=20u;goto L8c61c578; /* mov #0x14,r0 */
L8c61c578: fcopy(f,2u,4u);goto L8c61c57a; /* fmov fr4,fr2 */
L8c61c57a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61c57c; /* fsub fr3,fr2 */
L8c61c57c: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c57e; /* fmov.s @(r0,r13),fr3 */
L8c61c57e: fload(m,bus,&io_status,f,5u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c580; /* fmov.s @(r0,r12),fr5 */
L8c61c580: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c582; /* fsub fr5,fr3 */
L8c61c582: fcopy(f,1u,5u);goto L8c61c584; /* fmov fr5,fr1 */
L8c61c584: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c586; /* fmul fr3,fr2 */
L8c61c586: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c588; /* fmov.s @(r0,r14),fr3 */
L8c61c588: r[0]=16u;goto L8c61c58a; /* mov #0x10,r0 */
L8c61c58a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61c58c; /* fsub fr3,fr1 */
L8c61c58c: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c58e; /* fmov.s @(r0,r13),fr3 */
L8c61c58e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c590; /* fsub fr4,fr3 */
L8c61c590: fcopy(f,4u,2u);goto L8c61c592; /* fmov fr2,fr4 */
L8c61c592: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c594; /* fmul fr3,fr1 */
L8c61c594: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61c596; /* fldi0 fr3 */
L8c61c596: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c598; /* fsub fr1,fr4 */
L8c61c598: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61c59a; /* fcmp/gt fr3,fr4 */
L8c61c59a: if(!(*sr&1u))goto L8c61c5c0;goto L8c61c59c; /* bf 0x8c61c5c0 */
L8c61c59c: r[10]+=12u;goto L8c61c794; /* bra 0x8c61c794 */
L8c61c5c0: fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c61c5c2; /* fmov.s @r14,fr2 */
L8c61c5c2: r[0]=28u;goto L8c61c5c4; /* mov #0x1c,r0 */
L8c61c5c4: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61c5c6; /* fmov.s @r12,fr3 */
L8c61c5c6: r[4]=r[15];goto L8c61c5c8; /* mov r15,r4 */
L8c61c5c8: r[3]=rd(m,bus,&io_status,0x8c61c864u,4);if(io_status)return io_status;goto L8c61c5ca; /* mov.l 0x8c61c864,r3 */
L8c61c5ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c5cc; /* fadd fr3,fr2 */
L8c61c5cc: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61c5ce; /* fmov.s @r13,fr3 */
L8c61c5ce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c5d0; /* fadd fr3,fr2 */
L8c61c5d0: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c5d2; /* fmov fr2,@(r0,r15) */
L8c61c5d2: r[0]=4u;goto L8c61c5d4; /* mov #0x4,r0 */
L8c61c5d4: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c5d6; /* fmov.s @(r0,r14),fr2 */
L8c61c5d6: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c5d8; /* fmov.s @(r0,r12),fr3 */
L8c61c5d8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c5da; /* fadd fr3,fr2 */
L8c61c5da: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c5dc; /* fmov.s @(r0,r13),fr3 */
L8c61c5dc: r[0]=32u;goto L8c61c5de; /* mov #0x20,r0 */
L8c61c5de: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c5e0; /* fadd fr3,fr2 */
L8c61c5e0: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c5e2; /* fmov fr2,@(r0,r15) */
L8c61c5e2: r[0]=8u;goto L8c61c5e4; /* mov #0x8,r0 */
L8c61c5e4: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c5e6; /* fmov.s @(r0,r14),fr2 */
L8c61c5e6: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c5e8; /* fmov.s @(r0,r12),fr3 */
L8c61c5e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c5ea; /* fadd fr3,fr2 */
L8c61c5ea: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c5ec; /* fmov.s @(r0,r13),fr3 */
L8c61c5ec: r[0]=36u;goto L8c61c5ee; /* mov #0x24,r0 */
L8c61c5ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c5f0; /* fadd fr3,fr2 */
L8c61c5f0: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c5f2; /* fmov fr2,@(r0,r15) */
L8c61c5f2: target=r[3];*pr=0x8c61c5f6u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61c5f6; /* jsr @r3 */
L8c61c5f6: r[3]=rd(m,bus,&io_status,0x8c61c868u,4);if(io_status)return io_status;goto L8c61c5f8; /* mov.l 0x8c61c868,r3 */
L8c61c5f8: r[0]=28u;goto L8c61c5fa; /* mov #0x1c,r0 */
L8c61c5fa: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c5fc; /* fmov.s @(r0,r15),fr3 */
L8c61c5fc: r[0]=32u;goto L8c61c5fe; /* mov #0x20,r0 */
L8c61c5fe: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61c600; /* fmov.s @r3,fr2 */
L8c61c600: r[8]+=24u;goto L8c61c602; /* add #0x18,r8 */
L8c61c602: r[2]=rd(m,bus,&io_status,0x8c61c86cu,4);if(io_status)return io_status;goto L8c61c604; /* mov.l 0x8c61c86c,r2 */
L8c61c604: r[3]+=8u;goto L8c61c606; /* add #0x8,r3 */
L8c61c606: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c608; /* fmul fr3,fr2 */
L8c61c608: r[1]=rd(m,bus,&io_status,0x8c61c870u,4);if(io_status)return io_status;goto L8c61c60a; /* mov.l 0x8c61c870,r1 */
L8c61c60a: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61c60c; /* fmov.s @r2,fr3 */
L8c61c60c: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61c60e; /* fmov.s @r1,fr1 */
L8c61c60e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c610; /* fsub fr2,fr3 */
L8c61c610: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c612; /* fmov.s @(r0,r15),fr2 */
L8c61c612: r[0]=36u;goto L8c61c614; /* mov #0x24,r0 */
L8c61c614: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c616; /* fmul fr2,fr1 */
L8c61c616: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c618; /* fmov.s @(r0,r15),fr2 */
L8c61c618: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c61a; /* mov.w @(0x2,r10),r0 */
L8c61c61a: r[2]=r[0];goto L8c61c61c; /* mov r0,r2 */
L8c61c61c: *fpul=r[2];goto L8c61c61e; /* lds r2,FPUL */
L8c61c61e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c620; /* fsub fr1,fr3 */
L8c61c620: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61c622; /* fmov.s @r3,fr1 */
L8c61c622: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c624; /* fmul fr2,fr1 */
L8c61c624: fcopy(f,4u,3u);goto L8c61c626; /* fmov fr3,fr4 */
L8c61c626: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c628; /* float FPUL,fr3 */
L8c61c628: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c62a; /* fsub fr1,fr4 */
L8c61c62a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c62c; /* fmul fr14,fr3 */
L8c61c62c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c62e; /* fmov.s fr3,@-r8 */
L8c61c62e: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c630; /* mov.w @r10,r2 */
L8c61c630: *fpul=r[2];goto L8c61c632; /* lds r2,FPUL */
L8c61c632: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c634; /* float FPUL,fr3 */
L8c61c634: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c636; /* fmul fr14,fr3 */
L8c61c636: r[0]=24u;goto L8c61c638; /* mov #0x18,r0 */
L8c61c638: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c63a; /* fmov.s fr3,@-r8 */
L8c61c63a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c63c; /* fmov.s @(r0,r14),fr3 */
L8c61c63c: r[0]=20u;goto L8c61c63e; /* mov #0x14,r0 */
L8c61c63e: r[2]=rd(m,bus,&io_status,0x8c61c874u,4);if(io_status)return io_status;goto L8c61c640; /* mov.l 0x8c61c874,r2 */
L8c61c640: r[10]+=4u;goto L8c61c642; /* add #0x4,r10 */
L8c61c642: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c644; /* fmov.s fr3,@-r8 */
L8c61c644: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c646; /* fmov.s @(r0,r14),fr3 */
L8c61c646: r[0]=16u;goto L8c61c648; /* mov #0x10,r0 */
L8c61c648: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c64a; /* fmov.s fr3,@-r8 */
L8c61c64a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c64c; /* fmov.s @(r0,r14),fr3 */
L8c61c64c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c64e; /* fmov.s fr3,@-r8 */
L8c61c64e: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c650; /* mov.l r2,@-r8 */
L8c61c650: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c652; /* pref @r8 */
L8c61c652: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c654; /* mov.w @(0x2,r10),r0 */
L8c61c654: r[8]+=56u;goto L8c61c656; /* add #0x38,r8 */
L8c61c656: r[2]=r[0];goto L8c61c658; /* mov r0,r2 */
L8c61c658: *fpul=r[2];goto L8c61c65a; /* lds r2,FPUL */
L8c61c65a: r[0]=24u;goto L8c61c65c; /* mov #0x18,r0 */
L8c61c65c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c65e; /* float FPUL,fr3 */
L8c61c65e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c660; /* fmul fr14,fr3 */
L8c61c660: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c662; /* fmov.s fr3,@-r8 */
L8c61c662: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c664; /* mov.w @r10,r2 */
L8c61c664: r[10]+=4u;goto L8c61c666; /* add #0x4,r10 */
L8c61c666: *fpul=r[2];goto L8c61c668; /* lds r2,FPUL */
L8c61c668: r[2]=rd(m,bus,&io_status,0x8c61c874u,4);if(io_status)return io_status;goto L8c61c66a; /* mov.l 0x8c61c874,r2 */
L8c61c66a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c66c; /* float FPUL,fr3 */
L8c61c66c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c66e; /* fmul fr14,fr3 */
L8c61c66e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c670; /* fmov.s fr3,@-r8 */
L8c61c670: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c672; /* fmov.s @(r0,r12),fr3 */
L8c61c672: r[0]=20u;goto L8c61c674; /* mov #0x14,r0 */
L8c61c674: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c676; /* fmov.s fr3,@-r8 */
L8c61c676: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c678; /* fmov.s @(r0,r12),fr3 */
L8c61c678: r[0]=16u;goto L8c61c67a; /* mov #0x10,r0 */
L8c61c67a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c67c; /* fmov.s fr3,@-r8 */
L8c61c67c: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c67e; /* fmov.s @(r0,r12),fr3 */
L8c61c67e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c680; /* fmov.s fr3,@-r8 */
L8c61c680: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c682; /* mov.l r2,@-r8 */
L8c61c682: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c684; /* pref @r8 */
L8c61c684: r[8]+=60u;goto L8c61c686; /* add #0x3c,r8 */
L8c61c686: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61c688; /* fmov.s fr4,@-r8 */
L8c61c688: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c68a; /* mov.w @(0x2,r10),r0 */
L8c61c68a: r[2]=r[0];goto L8c61c68c; /* mov r0,r2 */
L8c61c68c: *fpul=r[2];goto L8c61c68e; /* lds r2,FPUL */
L8c61c68e: r[0]=24u;goto L8c61c690; /* mov #0x18,r0 */
L8c61c690: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c692; /* float FPUL,fr3 */
L8c61c692: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c694; /* fmul fr14,fr3 */
L8c61c694: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c696; /* fmov.s fr3,@-r8 */
L8c61c696: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c698; /* mov.w @r10,r2 */
L8c61c698: r[10]+=4u;goto L8c61c69a; /* add #0x4,r10 */
L8c61c69a: *fpul=r[2];goto L8c61c69c; /* lds r2,FPUL */
L8c61c69c: r[2]=rd(m,bus,&io_status,0x8c61c878u,4);if(io_status)return io_status;goto L8c61c69e; /* mov.l 0x8c61c878,r2 */
L8c61c69e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c6a0; /* float FPUL,fr3 */
L8c61c6a0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c6a2; /* fmul fr14,fr3 */
L8c61c6a2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c6a4; /* fmov.s fr3,@-r8 */
L8c61c6a4: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c6a6; /* fmov.s @(r0,r13),fr3 */
L8c61c6a6: r[0]=20u;goto L8c61c6a8; /* mov #0x14,r0 */
L8c61c6a8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c6aa; /* fmov.s fr3,@-r8 */
L8c61c6aa: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c6ac; /* fmov.s @(r0,r13),fr3 */
L8c61c6ac: r[0]=16u;goto L8c61c6ae; /* mov #0x10,r0 */
L8c61c6ae: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c6b0; /* fmov.s fr3,@-r8 */
L8c61c6b0: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c6b2; /* fmov.s @(r0,r13),fr3 */
L8c61c6b2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c6b4; /* fmov.s fr3,@-r8 */
L8c61c6b4: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c6b6; /* mov.l r2,@-r8 */
L8c61c6b6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c6b8; /* pref @r8 */
L8c61c6b8: r[0]=rd(m,bus,&io_status,0x8c61c87cu,4);if(io_status)return io_status;goto L8c61c6ba; /* mov.l 0x8c61c87c,r0 */
L8c61c6ba: r[8]+=32u;goto L8c61c6bc; /* add #0x20,r8 */
L8c61c6bc: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61c6be; /* mov.l @r0,r2 */
L8c61c6be: r[2]+=3u;goto L8c61c6c0; /* add #0x3,r2 */
L8c61c6c0: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61c794; /* bra 0x8c61c794 */
L8c61c6c4: r[0]=28u;goto L8c61c6c6; /* mov #0x1c,r0 */
L8c61c6c6: r[1]=rd(m,bus,&io_status,0x8c61c880u,4);if(io_status)return io_status;goto L8c61c6c8; /* mov.l 0x8c61c880,r1 */
L8c61c6c8: r[4]=r[15];goto L8c61c6ca; /* mov r15,r4 */
L8c61c6ca: wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c61c6cc; /* mov.l r3,@r1 */
L8c61c6cc: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61c6ce; /* fmov.s @r12,fr3 */
L8c61c6ce: fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c61c6d0; /* fmov.s @r14,fr2 */
L8c61c6d0: r[3]=rd(m,bus,&io_status,0x8c61c864u,4);if(io_status)return io_status;goto L8c61c6d2; /* mov.l 0x8c61c864,r3 */
L8c61c6d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c6d4; /* fadd fr3,fr2 */
L8c61c6d4: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61c6d6; /* fmov.s @r13,fr3 */
L8c61c6d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c6d8; /* fadd fr3,fr2 */
L8c61c6d8: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c6da; /* fmov fr2,@(r0,r15) */
L8c61c6da: r[0]=4u;goto L8c61c6dc; /* mov #0x4,r0 */
L8c61c6dc: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c6de; /* fmov.s @(r0,r12),fr3 */
L8c61c6de: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c6e0; /* fmov.s @(r0,r14),fr2 */
L8c61c6e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c6e2; /* fadd fr3,fr2 */
L8c61c6e2: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c6e4; /* fmov.s @(r0,r13),fr3 */
L8c61c6e4: r[0]=32u;goto L8c61c6e6; /* mov #0x20,r0 */
L8c61c6e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c6e8; /* fadd fr3,fr2 */
L8c61c6e8: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c6ea; /* fmov fr2,@(r0,r15) */
L8c61c6ea: r[0]=8u;goto L8c61c6ec; /* mov #0x8,r0 */
L8c61c6ec: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c6ee; /* fmov.s @(r0,r14),fr2 */
L8c61c6ee: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c6f0; /* fmov.s @(r0,r12),fr3 */
L8c61c6f0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c6f2; /* fadd fr3,fr2 */
L8c61c6f2: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c6f4; /* fmov.s @(r0,r13),fr3 */
L8c61c6f4: r[0]=36u;goto L8c61c6f6; /* mov #0x24,r0 */
L8c61c6f6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c6f8; /* fadd fr3,fr2 */
L8c61c6f8: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c6fa; /* fmov fr2,@(r0,r15) */
L8c61c6fa: target=r[3];*pr=0x8c61c6feu;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61c6fe; /* jsr @r3 */
L8c61c6fe: r[3]=rd(m,bus,&io_status,0x8c61c868u,4);if(io_status)return io_status;goto L8c61c700; /* mov.l 0x8c61c868,r3 */
L8c61c700: r[0]=28u;goto L8c61c702; /* mov #0x1c,r0 */
L8c61c702: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c704; /* fmov.s @(r0,r15),fr3 */
L8c61c704: r[0]=32u;goto L8c61c706; /* mov #0x20,r0 */
L8c61c706: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61c708; /* fmov.s @r3,fr2 */
L8c61c708: r[3]+=8u;goto L8c61c70a; /* add #0x8,r3 */
L8c61c70a: r[2]=rd(m,bus,&io_status,0x8c61c86cu,4);if(io_status)return io_status;goto L8c61c70c; /* mov.l 0x8c61c86c,r2 */
L8c61c70c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c70e; /* fmul fr3,fr2 */
L8c61c70e: r[1]=rd(m,bus,&io_status,0x8c61c870u,4);if(io_status)return io_status;goto L8c61c710; /* mov.l 0x8c61c870,r1 */
L8c61c710: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61c712; /* fmov.s @r2,fr3 */
L8c61c712: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61c714; /* fmov.s @r1,fr1 */
L8c61c714: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61c716; /* mov.l @r15,r2 */
L8c61c716: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c718; /* fsub fr2,fr3 */
L8c61c718: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c71a; /* fmov.s @(r0,r15),fr2 */
L8c61c71a: r[0]=36u;goto L8c61c71c; /* mov #0x24,r0 */
L8c61c71c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c71e; /* fmul fr2,fr1 */
L8c61c71e: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c720; /* fmov.s @(r0,r15),fr2 */
L8c61c720: r[0]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c722; /* mov.w @r10,r0 */
L8c61c722: *fpul=r[0];goto L8c61c724; /* lds r0,FPUL */
L8c61c724: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c726; /* fsub fr1,fr3 */
L8c61c726: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61c728; /* fmov.s @r3,fr1 */
L8c61c728: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c72a; /* fmul fr2,fr1 */
L8c61c72a: fcopy(f,4u,3u);goto L8c61c72c; /* fmov fr3,fr4 */
L8c61c72c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c72e; /* float FPUL,fr3 */
L8c61c72e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c730; /* fsub fr1,fr4 */
L8c61c730: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61c732; /* ftrc fr3,FPUL */
L8c61c732: r[0]=*fpul;goto L8c61c734; /* sts FPUL,r0 */
L8c61c734: wr(m,bus,&io_status,r[2],r[0],2);if(io_status)return io_status;goto L8c61c736; /* mov.w r0,@r2 */
L8c61c736: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c738; /* mov.w @(0x2,r10),r0 */
L8c61c738: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61c73a; /* mov.l @r15,r2 */
L8c61c73a: r[3]=r[0];goto L8c61c73c; /* mov r0,r3 */
L8c61c73c: *fpul=r[3];goto L8c61c73e; /* lds r3,FPUL */
L8c61c73e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c740; /* float FPUL,fr3 */
L8c61c740: r[10]+=4u;goto L8c61c742; /* add #0x4,r10 */
L8c61c742: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61c744; /* ftrc fr3,FPUL */
L8c61c744: r[0]=*fpul;goto L8c61c746; /* sts FPUL,r0 */
L8c61c746: wr(m,bus,&io_status,r[2]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61c748; /* mov.w r0,@(0x2,r2) */
L8c61c748: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c74a; /* mov.w @r10,r2 */
L8c61c74a: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61c74c; /* mov.l @(0x4,r15),r3 */
L8c61c74c: *fpul=r[2];goto L8c61c74e; /* lds r2,FPUL */
L8c61c74e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c750; /* float FPUL,fr3 */
L8c61c750: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61c752; /* ftrc fr3,FPUL */
L8c61c752: r[0]=*fpul;goto L8c61c754; /* sts FPUL,r0 */
L8c61c754: wr(m,bus,&io_status,r[3],r[0],2);if(io_status)return io_status;goto L8c61c756; /* mov.w r0,@r3 */
L8c61c756: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c758; /* mov.w @(0x2,r10),r0 */
L8c61c758: r[10]+=4u;goto L8c61c75a; /* add #0x4,r10 */
L8c61c75a: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61c75c; /* mov.l @(0x4,r15),r3 */
L8c61c75c: r[2]=r[0];goto L8c61c75e; /* mov r0,r2 */
L8c61c75e: *fpul=r[2];goto L8c61c760; /* lds r2,FPUL */
L8c61c760: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c762; /* float FPUL,fr3 */
L8c61c762: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61c764; /* ftrc fr3,FPUL */
L8c61c764: r[0]=*fpul;goto L8c61c766; /* sts FPUL,r0 */
L8c61c766: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61c768; /* mov.w r0,@(0x2,r3) */
L8c61c768: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c76a; /* mov.w @r10,r2 */
L8c61c76a: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61c76c; /* mov.l @(0x8,r15),r3 */
L8c61c76c: *fpul=r[2];goto L8c61c76e; /* lds r2,FPUL */
L8c61c76e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c770; /* float FPUL,fr3 */
L8c61c770: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61c772; /* ftrc fr3,FPUL */
L8c61c772: r[0]=*fpul;goto L8c61c774; /* sts FPUL,r0 */
L8c61c774: wr(m,bus,&io_status,r[3],r[0],2);if(io_status)return io_status;goto L8c61c776; /* mov.w r0,@r3 */
L8c61c776: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c778; /* mov.w @(0x2,r10),r0 */
L8c61c778: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61c77a; /* mov.l @(0x8,r15),r3 */
L8c61c77a: r[2]=r[0];goto L8c61c77c; /* mov r0,r2 */
L8c61c77c: *fpul=r[2];goto L8c61c77e; /* lds r2,FPUL */
L8c61c77e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c780; /* float FPUL,fr3 */
L8c61c780: r[7]=rd(m,bus,&io_status,0x8c61c884u,4);if(io_status)return io_status;goto L8c61c782; /* mov.l 0x8c61c884,r7 */
L8c61c782: r[5]=r[12];goto L8c61c784; /* mov r12,r5 */
L8c61c784: r[10]+=4u;goto L8c61c786; /* add #0x4,r10 */
L8c61c786: r[6]=r[13];goto L8c61c788; /* mov r13,r6 */
L8c61c788: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61c78a; /* ftrc fr3,FPUL */
L8c61c78a: r[0]=*fpul;goto L8c61c78c; /* sts FPUL,r0 */
L8c61c78c: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61c78e; /* mov.w r0,@(0x2,r3) */
L8c61c78e: r[3]=rd(m,bus,&io_status,0x8c61c888u,4);if(io_status)return io_status;goto L8c61c790; /* mov.l 0x8c61c888,r3 */
L8c61c790: target=r[3];*pr=0x8c61c794u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61c794; /* jsr @r3 */
L8c61c794: r[11]+=1u;goto L8c61c796; /* add #0x1,r11 */
L8c61c796: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61c798; /* mov.l @(0xc,r15),r2 */
L8c61c798: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61c79a; /* mov.w @(0x2,r2),r0 */
L8c61c79a: r[0]=r[0]&65535u;goto L8c61c79c; /* extu.w r0,r0 */
L8c61c79c: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61c79e; /* cmp/ge r0,r11 */
L8c61c79e: if((*sr&1u))goto L8c61c7a4;goto L8c61c7a0; /* bt 0x8c61c7a4 */
L8c61c7a0: goto L8c61c524; /* bra 0x8c61c524 */
L8c61c7a4: goto L8c61cf4c; /* bra 0x8c61cf4c */
L8c61c7a8: r[1]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61c7aa; /* mov.l @(0xc,r15),r1 */
L8c61c7aa: r[2]=rd(m,bus,&io_status,0x8c61c88cu,4);if(io_status)return io_status;goto L8c61c7ac; /* mov.l 0x8c61c88c,r2 */
L8c61c7ac: r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61c7ae; /* mov.w @(0x2,r1),r0 */
L8c61c7ae: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61c7b0; /* mov.l @r2,r1 */
L8c61c7b0: r[0]=r[0]&65535u;goto L8c61c7b2; /* extu.w r0,r0 */
L8c61c7b2: r[3]=rd(m,bus,&io_status,0x8c61c890u,4);if(io_status)return io_status;goto L8c61c7b4; /* mov.l 0x8c61c890,r3 */
L8c61c7b4: r[0]<<=2;goto L8c61c7b6; /* shll2 r0 */
L8c61c7b6: r[1]+=r[0];goto L8c61c7b8; /* add r0,r1 */
L8c61c7b8: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61c7ba; /* mov.l r1,@r2 */
L8c61c7ba: r[2]=0u;goto L8c61c7bc; /* mov #0x0,r2 */
L8c61c7bc: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c61c7be; /* mov.l r2,@(0x8,r15) */
L8c61c7be: r[2]=rd(m,bus,&io_status,0x8c61c884u,4);if(io_status)return io_status;goto L8c61c7c0; /* mov.l 0x8c61c884,r2 */
L8c61c7c0: r[1]=rd(m,bus,&io_status,0x8c61c894u,4);if(io_status)return io_status;goto L8c61c7c2; /* mov.l 0x8c61c894,r1 */
L8c61c7c2: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61c7c4; /* mov.l r2,@r15 */
L8c61c7c4: r[2]+=12u;goto L8c61c7c6; /* add #0xc,r2 */
L8c61c7c6: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61c7c8; /* mov.l r3,@(0x4,r15) */
L8c61c7c8: wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c61c7ca; /* mov.l r1,@(0x10,r15) */
L8c61c7ca: wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c61cafa; /* bra 0x8c61cafa */
L8c61c7ce: r[11]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c7d0; /* mov.w @r9+,r11 */
L8c61c7d0: r[3]=rd(m,bus,&io_status,0x8c61c898u,4);if(io_status)return io_status;goto L8c61c7d2; /* mov.l 0x8c61c898,r3 */
L8c61c7d2: r[13]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c7d4; /* mov.w @r9+,r13 */
L8c61c7d4: r[11]=r[11]&65535u;goto L8c61c7d6; /* extu.w r11,r11 */
L8c61c7d6: r[11]<<=2;goto L8c61c7d8; /* shll2 r11 */
L8c61c7d8: r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61c7da; /* mov.l @r3,r5 */
L8c61c7da: r[14]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c7dc; /* mov.w @r9+,r14 */
L8c61c7dc: r[13]=r[13]&65535u;goto L8c61c7de; /* extu.w r13,r13 */
L8c61c7de: r[13]<<=2;goto L8c61c7e0; /* shll2 r13 */
L8c61c7e0: r[12]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61c7e2; /* mov.w @r9+,r12 */
L8c61c7e2: r[14]=r[14]&65535u;goto L8c61c7e4; /* extu.w r14,r14 */
L8c61c7e4: r[13]<<=2;goto L8c61c7e6; /* shll2 r13 */
L8c61c7e6: r[11]<<=2;goto L8c61c7e8; /* shll2 r11 */
L8c61c7e8: r[12]=r[12]&65535u;goto L8c61c7ea; /* extu.w r12,r12 */
L8c61c7ea: r[14]<<=2;goto L8c61c7ec; /* shll2 r14 */
L8c61c7ec: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61c7ee; /* shll r11 */
L8c61c7ee: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61c7f0; /* shll r13 */
L8c61c7f0: r[11]+=r[5];goto L8c61c7f2; /* add r5,r11 */
L8c61c7f2: r[12]<<=2;goto L8c61c7f4; /* shll2 r12 */
L8c61c7f4: r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61c7f6; /* mov.l @(0xc,r11),r4 */
L8c61c7f6: r[13]+=r[5];goto L8c61c7f8; /* add r5,r13 */
L8c61c7f8: r[14]<<=2;goto L8c61c7fa; /* shll2 r14 */
L8c61c7fa: r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61c7fc; /* mov.l @(0xc,r13),r2 */
L8c61c7fc: r[12]<<=2;goto L8c61c7fe; /* shll2 r12 */
L8c61c7fe: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61c800; /* shll r14 */
L8c61c800: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61c802; /* shll r12 */
L8c61c802: r[14]+=r[5];goto L8c61c804; /* add r5,r14 */
L8c61c804: r[1]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61c806; /* mov.l @(0xc,r14),r1 */
L8c61c806: r[4]&=r[2];goto L8c61c808; /* and r2,r4 */
L8c61c808: r[12]+=r[5];goto L8c61c80a; /* add r5,r12 */
L8c61c80a: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61c80c; /* mov.l @(0xc,r12),r2 */
L8c61c80c: r[4]&=r[1];goto L8c61c80e; /* and r1,r4 */
L8c61c80e: r[1]=63u;goto L8c61c810; /* mov #0x3f,r1 */
L8c61c810: r[4]&=r[2];goto L8c61c812; /* and r2,r4 */
L8c61c812: *sr=(*sr&~1u)|!(r[1]&r[4]);goto L8c61c814; /* tst r1,r4 */
L8c61c814: if(!(*sr&1u))goto L8c61c85a;goto L8c61c816; /* bf 0x8c61c85a */
L8c61c816: r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61c818; /* mov.l @(0xc,r11),r4 */
L8c61c818: r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61c81a; /* mov.l @(0xc,r13),r3 */
L8c61c81a: r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61c81c; /* mov.l @(0xc,r14),r2 */
L8c61c81c: r[4]|=r[3];goto L8c61c81e; /* or r3,r4 */
L8c61c81e: r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61c820; /* mov.l @(0xc,r12),r3 */
L8c61c820: r[4]|=r[2];goto L8c61c822; /* or r2,r4 */
L8c61c822: r[2]=1u;goto L8c61c824; /* mov #0x1,r2 */
L8c61c824: r[4]|=r[3];goto L8c61c826; /* or r3,r4 */
L8c61c826: *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61c828; /* tst r2,r4 */
L8c61c828: if((*sr&1u))goto L8c61c82e;goto L8c61c82a; /* bt 0x8c61c82e */
L8c61c82a: goto L8c61c9d4; /* bra 0x8c61c9d4 */
L8c61c82e: r[0]=16u;goto L8c61c830; /* mov #0x10,r0 */
L8c61c830: fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c832; /* fmov.s @(r0,r13),fr4 */
L8c61c832: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c834; /* fmov.s @(r0,r11),fr3 */
L8c61c834: r[0]=20u;goto L8c61c836; /* mov #0x14,r0 */
L8c61c836: fcopy(f,2u,4u);goto L8c61c838; /* fmov fr4,fr2 */
L8c61c838: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61c83a; /* fsub fr3,fr2 */
L8c61c83a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c83c; /* fmov.s @(r0,r14),fr3 */
L8c61c83c: fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c83e; /* fmov.s @(r0,r13),fr5 */
L8c61c83e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c840; /* fsub fr5,fr3 */
L8c61c840: fcopy(f,1u,5u);goto L8c61c842; /* fmov fr5,fr1 */
L8c61c842: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c844; /* fmul fr3,fr2 */
L8c61c844: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c846; /* fmov.s @(r0,r11),fr3 */
L8c61c846: r[0]=16u;goto L8c61c848; /* mov #0x10,r0 */
L8c61c848: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61c84a; /* fsub fr3,fr1 */
L8c61c84a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c84c; /* fmov.s @(r0,r14),fr3 */
L8c61c84c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c84e; /* fsub fr4,fr3 */
L8c61c84e: fcopy(f,4u,2u);goto L8c61c850; /* fmov fr2,fr4 */
L8c61c850: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c852; /* fmul fr3,fr1 */
L8c61c852: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61c854; /* fldi0 fr3 */
L8c61c854: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c856; /* fsub fr1,fr4 */
L8c61c856: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61c858; /* fcmp/gt fr3,fr4 */
L8c61c858: if(!(*sr&1u))goto L8c61c89c;goto L8c61c85a; /* bf 0x8c61c89c */
L8c61c85a: r[10]+=16u;goto L8c61caf4; /* bra 0x8c61caf4 */
L8c61c89c: fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c61c89e; /* fmov.s @r11,fr2 */
L8c61c89e: r[0]=28u;goto L8c61c8a0; /* mov #0x1c,r0 */
L8c61c8a0: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61c8a2; /* fmov.s @r13,fr3 */
L8c61c8a2: r[4]=r[15];goto L8c61c8a4; /* mov r15,r4 */
L8c61c8a4: r[3]=rd(m,bus,&io_status,0x8c61cb74u,4);if(io_status)return io_status;goto L8c61c8a6; /* mov.l 0x8c61cb74,r3 */
L8c61c8a6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c8a8; /* fadd fr3,fr2 */
L8c61c8a8: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61c8aa; /* fmov.s @r14,fr3 */
L8c61c8aa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c8ac; /* fadd fr3,fr2 */
L8c61c8ac: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c8ae; /* fmov fr2,@(r0,r15) */
L8c61c8ae: r[0]=4u;goto L8c61c8b0; /* mov #0x4,r0 */
L8c61c8b0: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c8b2; /* fmov.s @(r0,r11),fr2 */
L8c61c8b2: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c8b4; /* fmov.s @(r0,r13),fr3 */
L8c61c8b4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c8b6; /* fadd fr3,fr2 */
L8c61c8b6: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c8b8; /* fmov.s @(r0,r14),fr3 */
L8c61c8b8: r[0]=32u;goto L8c61c8ba; /* mov #0x20,r0 */
L8c61c8ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c8bc; /* fadd fr3,fr2 */
L8c61c8bc: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c8be; /* fmov fr2,@(r0,r15) */
L8c61c8be: r[0]=8u;goto L8c61c8c0; /* mov #0x8,r0 */
L8c61c8c0: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c8c2; /* fmov.s @(r0,r11),fr2 */
L8c61c8c2: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c8c4; /* fmov.s @(r0,r13),fr3 */
L8c61c8c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c8c6; /* fadd fr3,fr2 */
L8c61c8c6: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c8c8; /* fmov.s @(r0,r14),fr3 */
L8c61c8c8: r[0]=36u;goto L8c61c8ca; /* mov #0x24,r0 */
L8c61c8ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c8cc; /* fadd fr3,fr2 */
L8c61c8cc: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c8ce; /* fmov fr2,@(r0,r15) */
L8c61c8ce: target=r[3];*pr=0x8c61c8d2u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61c8d2; /* jsr @r3 */
L8c61c8d2: r[3]=rd(m,bus,&io_status,0x8c61cb78u,4);if(io_status)return io_status;goto L8c61c8d4; /* mov.l 0x8c61cb78,r3 */
L8c61c8d4: r[0]=28u;goto L8c61c8d6; /* mov #0x1c,r0 */
L8c61c8d6: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c8d8; /* fmov.s @(r0,r15),fr3 */
L8c61c8d8: r[0]=32u;goto L8c61c8da; /* mov #0x20,r0 */
L8c61c8da: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61c8dc; /* fmov.s @r3,fr2 */
L8c61c8dc: r[8]+=24u;goto L8c61c8de; /* add #0x18,r8 */
L8c61c8de: r[2]=rd(m,bus,&io_status,0x8c61cb7cu,4);if(io_status)return io_status;goto L8c61c8e0; /* mov.l 0x8c61cb7c,r2 */
L8c61c8e0: r[3]+=8u;goto L8c61c8e2; /* add #0x8,r3 */
L8c61c8e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61c8e4; /* fmul fr3,fr2 */
L8c61c8e4: r[1]=rd(m,bus,&io_status,0x8c61cb80u,4);if(io_status)return io_status;goto L8c61c8e6; /* mov.l 0x8c61cb80,r1 */
L8c61c8e6: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61c8e8; /* fmov.s @r2,fr3 */
L8c61c8e8: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61c8ea; /* fmov.s @r1,fr1 */
L8c61c8ea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c8ec; /* fsub fr2,fr3 */
L8c61c8ec: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c8ee; /* fmov.s @(r0,r15),fr2 */
L8c61c8ee: r[0]=36u;goto L8c61c8f0; /* mov #0x24,r0 */
L8c61c8f0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c8f2; /* fmul fr2,fr1 */
L8c61c8f2: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c8f4; /* fmov.s @(r0,r15),fr2 */
L8c61c8f4: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c8f6; /* mov.w @(0x2,r10),r0 */
L8c61c8f6: r[2]=r[0];goto L8c61c8f8; /* mov r0,r2 */
L8c61c8f8: *fpul=r[2];goto L8c61c8fa; /* lds r2,FPUL */
L8c61c8fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61c8fc; /* fsub fr1,fr3 */
L8c61c8fc: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61c8fe; /* fmov.s @r3,fr1 */
L8c61c8fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61c900; /* fmul fr2,fr1 */
L8c61c900: fcopy(f,4u,3u);goto L8c61c902; /* fmov fr3,fr4 */
L8c61c902: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c904; /* float FPUL,fr3 */
L8c61c904: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61c906; /* fsub fr1,fr4 */
L8c61c906: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c908; /* fmul fr14,fr3 */
L8c61c908: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c90a; /* fmov.s fr3,@-r8 */
L8c61c90a: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c90c; /* mov.w @r10,r2 */
L8c61c90c: *fpul=r[2];goto L8c61c90e; /* lds r2,FPUL */
L8c61c90e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c910; /* float FPUL,fr3 */
L8c61c910: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c912; /* fmul fr14,fr3 */
L8c61c912: r[0]=24u;goto L8c61c914; /* mov #0x18,r0 */
L8c61c914: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c916; /* fmov.s fr3,@-r8 */
L8c61c916: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c918; /* fmov.s @(r0,r11),fr3 */
L8c61c918: r[0]=20u;goto L8c61c91a; /* mov #0x14,r0 */
L8c61c91a: r[2]=rd(m,bus,&io_status,0x8c61cb84u,4);if(io_status)return io_status;goto L8c61c91c; /* mov.l 0x8c61cb84,r2 */
L8c61c91c: r[10]+=4u;goto L8c61c91e; /* add #0x4,r10 */
L8c61c91e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c920; /* fmov.s fr3,@-r8 */
L8c61c920: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c922; /* fmov.s @(r0,r11),fr3 */
L8c61c922: r[0]=16u;goto L8c61c924; /* mov #0x10,r0 */
L8c61c924: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c926; /* fmov.s fr3,@-r8 */
L8c61c926: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c928; /* fmov.s @(r0,r11),fr3 */
L8c61c928: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c92a; /* fmov.s fr3,@-r8 */
L8c61c92a: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c92c; /* mov.l r2,@-r8 */
L8c61c92c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c92e; /* pref @r8 */
L8c61c92e: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c930; /* mov.w @(0x2,r10),r0 */
L8c61c930: r[8]+=56u;goto L8c61c932; /* add #0x38,r8 */
L8c61c932: r[2]=r[0];goto L8c61c934; /* mov r0,r2 */
L8c61c934: *fpul=r[2];goto L8c61c936; /* lds r2,FPUL */
L8c61c936: r[0]=24u;goto L8c61c938; /* mov #0x18,r0 */
L8c61c938: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c93a; /* float FPUL,fr3 */
L8c61c93a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c93c; /* fmul fr14,fr3 */
L8c61c93c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c93e; /* fmov.s fr3,@-r8 */
L8c61c93e: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c940; /* mov.w @r10,r2 */
L8c61c940: r[10]+=4u;goto L8c61c942; /* add #0x4,r10 */
L8c61c942: *fpul=r[2];goto L8c61c944; /* lds r2,FPUL */
L8c61c944: r[2]=rd(m,bus,&io_status,0x8c61cb84u,4);if(io_status)return io_status;goto L8c61c946; /* mov.l 0x8c61cb84,r2 */
L8c61c946: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c948; /* float FPUL,fr3 */
L8c61c948: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c94a; /* fmul fr14,fr3 */
L8c61c94a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c94c; /* fmov.s fr3,@-r8 */
L8c61c94c: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c94e; /* fmov.s @(r0,r13),fr3 */
L8c61c94e: r[0]=20u;goto L8c61c950; /* mov #0x14,r0 */
L8c61c950: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c952; /* fmov.s fr3,@-r8 */
L8c61c952: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c954; /* fmov.s @(r0,r13),fr3 */
L8c61c954: r[0]=16u;goto L8c61c956; /* mov #0x10,r0 */
L8c61c956: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c958; /* fmov.s fr3,@-r8 */
L8c61c958: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c95a; /* fmov.s @(r0,r13),fr3 */
L8c61c95a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c95c; /* fmov.s fr3,@-r8 */
L8c61c95c: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c95e; /* mov.l r2,@-r8 */
L8c61c95e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c960; /* pref @r8 */
L8c61c960: r[8]+=60u;goto L8c61c962; /* add #0x3c,r8 */
L8c61c962: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61c964; /* fmov.s fr4,@-r8 */
L8c61c964: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c966; /* mov.w @(0x2,r10),r0 */
L8c61c966: r[2]=r[0];goto L8c61c968; /* mov r0,r2 */
L8c61c968: *fpul=r[2];goto L8c61c96a; /* lds r2,FPUL */
L8c61c96a: r[0]=24u;goto L8c61c96c; /* mov #0x18,r0 */
L8c61c96c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c96e; /* float FPUL,fr3 */
L8c61c96e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c970; /* fmul fr14,fr3 */
L8c61c970: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c972; /* fmov.s fr3,@-r8 */
L8c61c972: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c974; /* mov.w @r10,r2 */
L8c61c974: r[10]+=4u;goto L8c61c976; /* add #0x4,r10 */
L8c61c976: *fpul=r[2];goto L8c61c978; /* lds r2,FPUL */
L8c61c978: r[2]=rd(m,bus,&io_status,0x8c61cb84u,4);if(io_status)return io_status;goto L8c61c97a; /* mov.l 0x8c61cb84,r2 */
L8c61c97a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c97c; /* float FPUL,fr3 */
L8c61c97c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c97e; /* fmul fr14,fr3 */
L8c61c97e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c980; /* fmov.s fr3,@-r8 */
L8c61c980: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c982; /* fmov.s @(r0,r14),fr3 */
L8c61c982: r[0]=20u;goto L8c61c984; /* mov #0x14,r0 */
L8c61c984: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c986; /* fmov.s fr3,@-r8 */
L8c61c986: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c988; /* fmov.s @(r0,r14),fr3 */
L8c61c988: r[0]=16u;goto L8c61c98a; /* mov #0x10,r0 */
L8c61c98a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c98c; /* fmov.s fr3,@-r8 */
L8c61c98c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c98e; /* fmov.s @(r0,r14),fr3 */
L8c61c98e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c990; /* fmov.s fr3,@-r8 */
L8c61c990: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c992; /* mov.l r2,@-r8 */
L8c61c992: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c994; /* pref @r8 */
L8c61c994: r[8]+=60u;goto L8c61c996; /* add #0x3c,r8 */
L8c61c996: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61c998; /* fmov.s fr4,@-r8 */
L8c61c998: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61c99a; /* mov.w @(0x2,r10),r0 */
L8c61c99a: r[2]=r[0];goto L8c61c99c; /* mov r0,r2 */
L8c61c99c: *fpul=r[2];goto L8c61c99e; /* lds r2,FPUL */
L8c61c99e: r[0]=24u;goto L8c61c9a0; /* mov #0x18,r0 */
L8c61c9a0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c9a2; /* float FPUL,fr3 */
L8c61c9a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c9a4; /* fmul fr14,fr3 */
L8c61c9a4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c9a6; /* fmov.s fr3,@-r8 */
L8c61c9a6: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61c9a8; /* mov.w @r10,r2 */
L8c61c9a8: r[10]+=4u;goto L8c61c9aa; /* add #0x4,r10 */
L8c61c9aa: *fpul=r[2];goto L8c61c9ac; /* lds r2,FPUL */
L8c61c9ac: r[2]=rd(m,bus,&io_status,0x8c61cb88u,4);if(io_status)return io_status;goto L8c61c9ae; /* mov.l 0x8c61cb88,r2 */
L8c61c9ae: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61c9b0; /* float FPUL,fr3 */
L8c61c9b0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61c9b2; /* fmul fr14,fr3 */
L8c61c9b2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c9b4; /* fmov.s fr3,@-r8 */
L8c61c9b4: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c9b6; /* fmov.s @(r0,r12),fr3 */
L8c61c9b6: r[0]=20u;goto L8c61c9b8; /* mov #0x14,r0 */
L8c61c9b8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c9ba; /* fmov.s fr3,@-r8 */
L8c61c9ba: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c9bc; /* fmov.s @(r0,r12),fr3 */
L8c61c9bc: r[0]=16u;goto L8c61c9be; /* mov #0x10,r0 */
L8c61c9be: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c9c0; /* fmov.s fr3,@-r8 */
L8c61c9c0: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61c9c2; /* fmov.s @(r0,r12),fr3 */
L8c61c9c2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61c9c4; /* fmov.s fr3,@-r8 */
L8c61c9c4: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61c9c6; /* mov.l r2,@-r8 */
L8c61c9c6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61c9c8; /* pref @r8 */
L8c61c9c8: r[0]=rd(m,bus,&io_status,0x8c61cb8cu,4);if(io_status)return io_status;goto L8c61c9ca; /* mov.l 0x8c61cb8c,r0 */
L8c61c9ca: r[8]+=32u;goto L8c61c9cc; /* add #0x20,r8 */
L8c61c9cc: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61c9ce; /* mov.l @r0,r2 */
L8c61c9ce: r[2]+=4u;goto L8c61c9d0; /* add #0x4,r2 */
L8c61c9d0: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61caf4; /* bra 0x8c61caf4 */
L8c61c9d4: r[0]=28u;goto L8c61c9d6; /* mov #0x1c,r0 */
L8c61c9d6: r[1]=rd(m,bus,&io_status,0x8c61cb90u,4);if(io_status)return io_status;goto L8c61c9d8; /* mov.l 0x8c61cb90,r1 */
L8c61c9d8: r[3]=rd(m,bus,&io_status,0x8c61cb74u,4);if(io_status)return io_status;goto L8c61c9da; /* mov.l 0x8c61cb74,r3 */
L8c61c9da: r[4]=r[15];goto L8c61c9dc; /* mov r15,r4 */
L8c61c9dc: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61c9de; /* mov.l r2,@r1 */
L8c61c9de: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61c9e0; /* fmov.s @r13,fr3 */
L8c61c9e0: fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c61c9e2; /* fmov.s @r11,fr2 */
L8c61c9e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c9e4; /* fadd fr3,fr2 */
L8c61c9e4: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61c9e6; /* fmov.s @r14,fr3 */
L8c61c9e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c9e8; /* fadd fr3,fr2 */
L8c61c9e8: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c9ea; /* fmov fr2,@(r0,r15) */
L8c61c9ea: r[0]=4u;goto L8c61c9ec; /* mov #0x4,r0 */
L8c61c9ec: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61c9ee; /* fmov.s @(r0,r13),fr3 */
L8c61c9ee: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c9f0; /* fmov.s @(r0,r11),fr2 */
L8c61c9f0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c9f2; /* fadd fr3,fr2 */
L8c61c9f2: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61c9f4; /* fmov.s @(r0,r14),fr3 */
L8c61c9f4: r[0]=32u;goto L8c61c9f6; /* mov #0x20,r0 */
L8c61c9f6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61c9f8; /* fadd fr3,fr2 */
L8c61c9f8: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61c9fa; /* fmov fr2,@(r0,r15) */
L8c61c9fa: r[0]=8u;goto L8c61c9fc; /* mov #0x8,r0 */
L8c61c9fc: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61c9fe; /* fmov.s @(r0,r11),fr2 */
L8c61c9fe: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ca00; /* fmov.s @(r0,r13),fr3 */
L8c61ca00: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ca02; /* fadd fr3,fr2 */
L8c61ca02: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ca04; /* fmov.s @(r0,r14),fr3 */
L8c61ca04: r[0]=36u;goto L8c61ca06; /* mov #0x24,r0 */
L8c61ca06: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ca08; /* fadd fr3,fr2 */
L8c61ca08: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ca0a; /* fmov fr2,@(r0,r15) */
L8c61ca0a: target=r[3];*pr=0x8c61ca0eu;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ca0e; /* jsr @r3 */
L8c61ca0e: r[3]=rd(m,bus,&io_status,0x8c61cb78u,4);if(io_status)return io_status;goto L8c61ca10; /* mov.l 0x8c61cb78,r3 */
L8c61ca10: r[0]=28u;goto L8c61ca12; /* mov #0x1c,r0 */
L8c61ca12: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ca14; /* fmov.s @(r0,r15),fr3 */
L8c61ca14: r[0]=32u;goto L8c61ca16; /* mov #0x20,r0 */
L8c61ca16: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61ca18; /* fmov.s @r3,fr2 */
L8c61ca18: r[3]+=8u;goto L8c61ca1a; /* add #0x8,r3 */
L8c61ca1a: r[2]=rd(m,bus,&io_status,0x8c61cb7cu,4);if(io_status)return io_status;goto L8c61ca1c; /* mov.l 0x8c61cb7c,r2 */
L8c61ca1c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ca1e; /* fmul fr3,fr2 */
L8c61ca1e: r[1]=rd(m,bus,&io_status,0x8c61cb80u,4);if(io_status)return io_status;goto L8c61ca20; /* mov.l 0x8c61cb80,r1 */
L8c61ca20: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61ca22; /* fmov.s @r2,fr3 */
L8c61ca22: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61ca24; /* fmov.s @r1,fr1 */
L8c61ca24: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ca26; /* mov.l @r15,r2 */
L8c61ca26: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ca28; /* fsub fr2,fr3 */
L8c61ca28: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ca2a; /* fmov.s @(r0,r15),fr2 */
L8c61ca2a: r[0]=36u;goto L8c61ca2c; /* mov #0x24,r0 */
L8c61ca2c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ca2e; /* fmul fr2,fr1 */
L8c61ca2e: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ca30; /* fmov.s @(r0,r15),fr2 */
L8c61ca30: r[0]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ca32; /* mov.w @r10,r0 */
L8c61ca32: *fpul=r[0];goto L8c61ca34; /* lds r0,FPUL */
L8c61ca34: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ca36; /* fsub fr1,fr3 */
L8c61ca36: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61ca38; /* fmov.s @r3,fr1 */
L8c61ca38: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ca3a; /* fmul fr2,fr1 */
L8c61ca3a: fcopy(f,15u,3u);goto L8c61ca3c; /* fmov fr3,fr15 */
L8c61ca3c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ca3e; /* float FPUL,fr3 */
L8c61ca3e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[1],&f->fpscr,&fr[15],1);if(status)return status;}goto L8c61ca40; /* fsub fr1,fr15 */
L8c61ca40: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ca42; /* ftrc fr3,FPUL */
L8c61ca42: r[0]=*fpul;goto L8c61ca44; /* sts FPUL,r0 */
L8c61ca44: wr(m,bus,&io_status,r[2],r[0],2);if(io_status)return io_status;goto L8c61ca46; /* mov.w r0,@r2 */
L8c61ca46: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ca48; /* mov.w @(0x2,r10),r0 */
L8c61ca48: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ca4a; /* mov.l @r15,r2 */
L8c61ca4a: r[3]=r[0];goto L8c61ca4c; /* mov r0,r3 */
L8c61ca4c: *fpul=r[3];goto L8c61ca4e; /* lds r3,FPUL */
L8c61ca4e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ca50; /* float FPUL,fr3 */
L8c61ca50: r[10]+=4u;goto L8c61ca52; /* add #0x4,r10 */
L8c61ca52: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ca54; /* ftrc fr3,FPUL */
L8c61ca54: r[0]=*fpul;goto L8c61ca56; /* sts FPUL,r0 */
L8c61ca56: wr(m,bus,&io_status,r[2]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61ca58; /* mov.w r0,@(0x2,r2) */
L8c61ca58: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ca5a; /* mov.w @r10,r2 */
L8c61ca5a: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61ca5c; /* mov.l @(0x4,r15),r3 */
L8c61ca5c: *fpul=r[2];goto L8c61ca5e; /* lds r2,FPUL */
L8c61ca5e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ca60; /* float FPUL,fr3 */
L8c61ca60: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ca62; /* ftrc fr3,FPUL */
L8c61ca62: r[0]=*fpul;goto L8c61ca64; /* sts FPUL,r0 */
L8c61ca64: wr(m,bus,&io_status,r[3],r[0],2);if(io_status)return io_status;goto L8c61ca66; /* mov.w r0,@r3 */
L8c61ca66: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ca68; /* mov.w @(0x2,r10),r0 */
L8c61ca68: r[10]+=4u;goto L8c61ca6a; /* add #0x4,r10 */
L8c61ca6a: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61ca6c; /* mov.l @(0x4,r15),r3 */
L8c61ca6c: r[2]=r[0];goto L8c61ca6e; /* mov r0,r2 */
L8c61ca6e: *fpul=r[2];goto L8c61ca70; /* lds r2,FPUL */
L8c61ca70: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ca72; /* float FPUL,fr3 */
L8c61ca72: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ca74; /* ftrc fr3,FPUL */
L8c61ca74: r[0]=*fpul;goto L8c61ca76; /* sts FPUL,r0 */
L8c61ca76: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61ca78; /* mov.w r0,@(0x2,r3) */
L8c61ca78: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ca7a; /* mov.w @r10,r2 */
L8c61ca7a: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61ca7c; /* mov.l @(0x10,r15),r3 */
L8c61ca7c: *fpul=r[2];goto L8c61ca7e; /* lds r2,FPUL */
L8c61ca7e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ca80; /* float FPUL,fr3 */
L8c61ca80: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ca82; /* ftrc fr3,FPUL */
L8c61ca82: r[0]=*fpul;goto L8c61ca84; /* sts FPUL,r0 */
L8c61ca84: wr(m,bus,&io_status,r[3],r[0],2);if(io_status)return io_status;goto L8c61ca86; /* mov.w r0,@r3 */
L8c61ca86: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ca88; /* mov.w @(0x2,r10),r0 */
L8c61ca88: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61ca8a; /* mov.l @(0x10,r15),r3 */
L8c61ca8a: r[2]=r[0];goto L8c61ca8c; /* mov r0,r2 */
L8c61ca8c: *fpul=r[2];goto L8c61ca8e; /* lds r2,FPUL */
L8c61ca8e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ca90; /* float FPUL,fr3 */
L8c61ca90: r[10]+=4u;goto L8c61ca92; /* add #0x4,r10 */
L8c61ca92: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ca94; /* ftrc fr3,FPUL */
L8c61ca94: r[0]=*fpul;goto L8c61ca96; /* sts FPUL,r0 */
L8c61ca96: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61ca98; /* mov.w r0,@(0x2,r3) */
L8c61ca98: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ca9a; /* mov.w @r10,r2 */
L8c61ca9a: r[3]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c61ca9c; /* mov.l @(0x14,r15),r3 */
L8c61ca9c: *fpul=r[2];goto L8c61ca9e; /* lds r2,FPUL */
L8c61ca9e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61caa0; /* float FPUL,fr3 */
L8c61caa0: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61caa2; /* ftrc fr3,FPUL */
L8c61caa2: r[0]=*fpul;goto L8c61caa4; /* sts FPUL,r0 */
L8c61caa4: wr(m,bus,&io_status,r[3],r[0],2);if(io_status)return io_status;goto L8c61caa6; /* mov.w r0,@r3 */
L8c61caa6: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61caa8; /* mov.w @(0x2,r10),r0 */
L8c61caa8: r[3]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c61caaa; /* mov.l @(0x14,r15),r3 */
L8c61caaa: r[2]=r[0];goto L8c61caac; /* mov r0,r2 */
L8c61caac: *fpul=r[2];goto L8c61caae; /* lds r2,FPUL */
L8c61caae: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cab0; /* float FPUL,fr3 */
L8c61cab0: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61cab2; /* ftrc fr3,FPUL */
L8c61cab2: r[0]=*fpul;goto L8c61cab4; /* sts FPUL,r0 */
L8c61cab4: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61cab6; /* mov.w r0,@(0x2,r3) */
L8c61cab6: r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61cab8; /* mov.l @(0xc,r11),r4 */
L8c61cab8: r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61caba; /* mov.l @(0xc,r13),r3 */
L8c61caba: r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61cabc; /* mov.l @(0xc,r14),r2 */
L8c61cabc: r[4]&=r[3];goto L8c61cabe; /* and r3,r4 */
L8c61cabe: r[3]=1u;goto L8c61cac0; /* mov #0x1,r3 */
L8c61cac0: r[4]&=r[2];goto L8c61cac2; /* and r2,r4 */
L8c61cac2: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61cac4; /* tst r3,r4 */
L8c61cac4: take=!(*sr&1u);r[10]+=4u;if(take)goto L8c61cad6;goto L8c61cac8; /* bf/s 0x8c61cad6 */
L8c61cac8: r[7]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61caca; /* mov.l @r15,r7 */
L8c61caca: r[5]=r[13];goto L8c61cacc; /* mov r13,r5 */
L8c61cacc: r[2]=rd(m,bus,&io_status,0x8c61cb94u,4);if(io_status)return io_status;goto L8c61cace; /* mov.l 0x8c61cb94,r2 */
L8c61cace: r[6]=r[14];goto L8c61cad0; /* mov r14,r6 */
L8c61cad0: fcopy(f,4u,15u);goto L8c61cad2; /* fmov fr15,fr4 */
L8c61cad2: target=r[2];*pr=0x8c61cad6u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61cad6; /* jsr @r2 */
L8c61cad6: r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61cad8; /* mov.l @(0xc,r13),r4 */
L8c61cad8: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61cada; /* mov.l @(0xc,r14),r3 */
L8c61cada: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61cadc; /* mov.l @(0xc,r12),r2 */
L8c61cadc: r[4]&=r[3];goto L8c61cade; /* and r3,r4 */
L8c61cade: r[3]=1u;goto L8c61cae0; /* mov #0x1,r3 */
L8c61cae0: r[4]&=r[2];goto L8c61cae2; /* and r2,r4 */
L8c61cae2: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61cae4; /* tst r3,r4 */
L8c61cae4: if(!(*sr&1u))goto L8c61caf4;goto L8c61cae6; /* bf 0x8c61caf4 */
L8c61cae6: r[7]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61cae8; /* mov.l @(0x4,r15),r7 */
L8c61cae8: r[5]=r[14];goto L8c61caea; /* mov r14,r5 */
L8c61caea: r[2]=rd(m,bus,&io_status,0x8c61cb94u,4);if(io_status)return io_status;goto L8c61caec; /* mov.l 0x8c61cb94,r2 */
L8c61caec: r[6]=r[12];goto L8c61caee; /* mov r12,r6 */
L8c61caee: fcopy(f,4u,15u);goto L8c61caf0; /* fmov fr15,fr4 */
L8c61caf0: target=r[2];*pr=0x8c61caf4u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61caf4; /* jsr @r2 */
L8c61caf4: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61caf6; /* mov.l @(0x8,r15),r3 */
L8c61caf6: r[3]+=1u;goto L8c61caf8; /* add #0x1,r3 */
L8c61caf8: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c61cafa; /* mov.l r3,@(0x8,r15) */
L8c61cafa: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61cafc; /* mov.l @(0xc,r15),r2 */
L8c61cafc: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61cafe; /* mov.l @(0x8,r15),r3 */
L8c61cafe: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61cb00; /* mov.w @(0x2,r2),r0 */
L8c61cb00: r[0]=r[0]&65535u;goto L8c61cb02; /* extu.w r0,r0 */
L8c61cb02: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c61cb04; /* cmp/ge r0,r3 */
L8c61cb04: if((*sr&1u))goto L8c61cb0a;goto L8c61cb06; /* bt 0x8c61cb0a */
L8c61cb06: goto L8c61c7ce; /* bra 0x8c61c7ce */
L8c61cb0a: goto L8c61cf4c; /* bra 0x8c61cf4c */
L8c61cb0e: r[4]=rd(m,bus,&io_status,0x8c61cb98u,4);if(io_status)return io_status;goto L8c61cb10; /* mov.l 0x8c61cb98,r4 */
L8c61cb10: r[2]=0u;goto L8c61cb12; /* mov #0x0,r2 */
L8c61cb12: wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c61cb14; /* mov.l r2,@(0x14,r15) */
L8c61cb14: r[2]=r[4];goto L8c61cb16; /* mov r4,r2 */
L8c61cb16: r[1]=r[4];goto L8c61cb18; /* mov r4,r1 */
L8c61cb18: r[2]+=8u;goto L8c61cb1a; /* add #0x8,r2 */
L8c61cb1a: r[4]+=4u;goto L8c61cb1c; /* add #0x4,r4 */
L8c61cb1c: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c61cb1e; /* mov.l r2,@(0x8,r15) */
L8c61cb1e: wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c61cb20; /* mov.l r1,@(0x10,r15) */
L8c61cb20: wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c61cefa; /* bra 0x8c61cefa */
L8c61cb24: r[2]=rd(m,bus,&io_status,0x8c61cb9cu,4);if(io_status)return io_status;goto L8c61cb26; /* mov.l 0x8c61cb9c,r2 */
L8c61cb26: r[5]=0u;goto L8c61cb28; /* mov #0x0,r5 */
L8c61cb28: r[3]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61cb2a; /* mov.w @r9+,r3 */
L8c61cb2a: r[7]=r[5];goto L8c61cb2c; /* mov r5,r7 */
L8c61cb2c: r[6]=0u;goto L8c61cb2e; /* mov #0x0,r6 */
L8c61cb2e: r[3]=r[3]&65535u;goto L8c61cb30; /* extu.w r3,r3 */
L8c61cb30: *sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c61cb32; /* cmp/pl r3 */
L8c61cb32: r[4]=4294967295u;goto L8c61cb34; /* mov #-0x1,r4 */
L8c61cb34: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61cb36; /* mov.l r3,@r15 */
L8c61cb36: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61cb38; /* mov.l @r2,r1 */
L8c61cb38: r[1]+=r[3];goto L8c61cb3a; /* add r3,r1 */
L8c61cb3a: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61cb3c; /* mov.l r1,@r2 */
L8c61cb3c: take=!(*sr&1u);r[6]+=r[9];if(take)goto L8c61cb60;goto L8c61cb40; /* bf/s 0x8c61cb60 */
L8c61cb40: r[11]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c61cb42; /* mov.w @r6+,r11 */
L8c61cb42: r[7]+=1u;goto L8c61cb44; /* add #0x1,r7 */
L8c61cb44: r[2]=rd(m,bus,&io_status,0x8c61cba0u,4);if(io_status)return io_status;goto L8c61cb46; /* mov.l 0x8c61cba0,r2 */
L8c61cb46: r[11]=r[11]&65535u;goto L8c61cb48; /* extu.w r11,r11 */
L8c61cb48: r[11]<<=2;goto L8c61cb4a; /* shll2 r11 */
L8c61cb4a: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61cb4c; /* mov.l @r2,r3 */
L8c61cb4c: r[11]<<=2;goto L8c61cb4e; /* shll2 r11 */
L8c61cb4e: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61cb50; /* shll r11 */
L8c61cb50: r[11]+=r[3];goto L8c61cb52; /* add r3,r11 */
L8c61cb52: wr(m,bus,&io_status,r[15]+0x18u,r[11],4);if(io_status)return io_status;goto L8c61cb54; /* mov.l r11,@(0x18,r15) */
L8c61cb54: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61cb56; /* mov.l @r15,r3 */
L8c61cb56: r[11]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61cb58; /* mov.l @(0xc,r11),r11 */
L8c61cb58: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[3]);goto L8c61cb5a; /* cmp/ge r3,r7 */
L8c61cb5a: r[4]&=r[11];goto L8c61cb5c; /* and r11,r4 */
L8c61cb5c: take=!(*sr&1u);r[5]|=r[11];if(take)goto L8c61cb40;goto L8c61cb60; /* bf/s 0x8c61cb40 */
L8c61cb60: r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61cb62; /* mov.l @r15,r6 */
L8c61cb62: r[3]=63u;goto L8c61cb64; /* mov #0x3f,r3 */
L8c61cb64: r[7]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61cb66; /* mov.l @r15,r7 */
L8c61cb66: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61cb68; /* shll r6 */
L8c61cb68: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61cb6a; /* tst r3,r4 */
L8c61cb6a: take=(*sr&1u);r[7]<<=2;if(take)goto L8c61cba4;goto L8c61cb6e; /* bt/s 0x8c61cba4 */
L8c61cb6e: r[10]+=r[7];goto L8c61cb70; /* add r7,r10 */
L8c61cb70: r[9]+=r[6];goto L8c61cef4; /* bra 0x8c61cef4 */
L8c61cba4: r[3]=1u;goto L8c61cba6; /* mov #0x1,r3 */
L8c61cba6: *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61cba8; /* tst r3,r5 */
L8c61cba8: if((*sr&1u))goto L8c61cbae;goto L8c61cbaa; /* bt 0x8c61cbae */
L8c61cbaa: goto L8c61cdbc; /* bra 0x8c61cdbc */
L8c61cbae: r[0]=sx(m,bus,&io_status,r[9]+0x2u,2);if(io_status)return io_status;goto L8c61cbb0; /* mov.w @(0x2,r9),r0 */
L8c61cbb0: r[12]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;goto L8c61cbb2; /* mov.w @r9,r12 */
L8c61cbb2: r[2]=rd(m,bus,&io_status,0x8c61cd9cu,4);if(io_status)return io_status;goto L8c61cbb4; /* mov.l 0x8c61cd9c,r2 */
L8c61cbb4: r[14]=r[0]&65535u;goto L8c61cbb6; /* extu.w r0,r14 */
L8c61cbb6: r[14]<<=2;goto L8c61cbb8; /* shll2 r14 */
L8c61cbb8: r[0]=sx(m,bus,&io_status,r[9]+0x4u,2);if(io_status)return io_status;goto L8c61cbba; /* mov.w @(0x4,r9),r0 */
L8c61cbba: r[12]=r[12]&65535u;goto L8c61cbbc; /* extu.w r12,r12 */
L8c61cbbc: r[4]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61cbbe; /* mov.l @r2,r4 */
L8c61cbbe: r[12]<<=2;goto L8c61cbc0; /* shll2 r12 */
L8c61cbc0: r[14]<<=2;goto L8c61cbc2; /* shll2 r14 */
L8c61cbc2: r[13]=r[0]&65535u;goto L8c61cbc4; /* extu.w r0,r13 */
L8c61cbc4: r[12]<<=2;goto L8c61cbc6; /* shll2 r12 */
L8c61cbc6: r[0]=16u;goto L8c61cbc8; /* mov #0x10,r0 */
L8c61cbc8: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61cbca; /* shll r12 */
L8c61cbca: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61cbcc; /* shll r14 */
L8c61cbcc: r[14]+=r[4];goto L8c61cbce; /* add r4,r14 */
L8c61cbce: fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c61cbd0; /* fmov.s @(r0,r14),fr5 */
L8c61cbd0: r[13]<<=2;goto L8c61cbd2; /* shll2 r13 */
L8c61cbd2: r[12]+=r[4];goto L8c61cbd4; /* add r4,r12 */
L8c61cbd4: r[13]<<=2;goto L8c61cbd6; /* shll2 r13 */
L8c61cbd6: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61cbd8; /* fmov.s @(r0,r12),fr3 */
L8c61cbd8: fcopy(f,2u,5u);goto L8c61cbda; /* fmov fr5,fr2 */
L8c61cbda: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61cbdc; /* shll r13 */
L8c61cbdc: r[0]=20u;goto L8c61cbde; /* mov #0x14,r0 */
L8c61cbde: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61cbe0; /* fsub fr3,fr2 */
L8c61cbe0: r[13]+=r[4];goto L8c61cbe2; /* add r4,r13 */
L8c61cbe2: fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c61cbe4; /* fmov.s @(r0,r14),fr4 */
L8c61cbe4: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61cbe6; /* fmov.s @(r0,r13),fr3 */
L8c61cbe6: fcopy(f,1u,4u);goto L8c61cbe8; /* fmov fr4,fr1 */
L8c61cbe8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61cbea; /* fsub fr4,fr3 */
L8c61cbea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61cbec; /* fmul fr3,fr2 */
L8c61cbec: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61cbee; /* fmov.s @(r0,r12),fr3 */
L8c61cbee: r[0]=16u;goto L8c61cbf0; /* mov #0x10,r0 */
L8c61cbf0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61cbf2; /* fsub fr3,fr1 */
L8c61cbf2: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61cbf4; /* fmov.s @(r0,r13),fr3 */
L8c61cbf4: fcopy(f,4u,2u);goto L8c61cbf6; /* fmov fr2,fr4 */
L8c61cbf6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61cbf8; /* fsub fr5,fr3 */
L8c61cbf8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61cbfa; /* fmul fr3,fr1 */
L8c61cbfa: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61cbfc; /* fldi0 fr3 */
L8c61cbfc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61cbfe; /* fsub fr1,fr4 */
L8c61cbfe: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61cc00; /* fcmp/gt fr3,fr4 */
L8c61cc00: if(!(*sr&1u))goto L8c61cc08;goto L8c61cc02; /* bf 0x8c61cc08 */
L8c61cc02: r[9]+=r[6];goto L8c61cc04; /* add r6,r9 */
L8c61cc04: r[10]+=r[7];goto L8c61cef4; /* bra 0x8c61cef4 */
L8c61cc08: r[0]=28u;goto L8c61cc0a; /* mov #0x1c,r0 */
L8c61cc0a: r[3]=rd(m,bus,&io_status,0x8c61cda0u,4);if(io_status)return io_status;goto L8c61cc0c; /* mov.l 0x8c61cda0,r3 */
L8c61cc0c: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61cc0e; /* mov.l @r15,r2 */
L8c61cc0e: r[4]=r[15];goto L8c61cc10; /* mov r15,r4 */
L8c61cc10: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61cc12; /* mov.l @r3,r1 */
L8c61cc12: r[1]+=r[2];goto L8c61cc14; /* add r2,r1 */
L8c61cc14: wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c61cc16; /* mov.l r1,@r3 */
L8c61cc16: fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61cc18; /* fmov.s @r12,fr2 */
L8c61cc18: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61cc1a; /* fmov.s @r14,fr3 */
L8c61cc1a: r[3]=rd(m,bus,&io_status,0x8c61cda4u,4);if(io_status)return io_status;goto L8c61cc1c; /* mov.l 0x8c61cda4,r3 */
L8c61cc1c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cc1e; /* fadd fr3,fr2 */
L8c61cc1e: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61cc20; /* fmov.s @r13,fr3 */
L8c61cc20: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cc22; /* fadd fr3,fr2 */
L8c61cc22: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cc24; /* fmov fr2,@(r0,r15) */
L8c61cc24: r[0]=4u;goto L8c61cc26; /* mov #0x4,r0 */
L8c61cc26: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61cc28; /* fmov.s @(r0,r12),fr2 */
L8c61cc28: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61cc2a; /* fmov.s @(r0,r14),fr3 */
L8c61cc2a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cc2c; /* fadd fr3,fr2 */
L8c61cc2c: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61cc2e; /* fmov.s @(r0,r13),fr3 */
L8c61cc2e: r[0]=32u;goto L8c61cc30; /* mov #0x20,r0 */
L8c61cc30: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cc32; /* fadd fr3,fr2 */
L8c61cc32: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cc34; /* fmov fr2,@(r0,r15) */
L8c61cc34: r[0]=8u;goto L8c61cc36; /* mov #0x8,r0 */
L8c61cc36: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61cc38; /* fmov.s @(r0,r14),fr3 */
L8c61cc38: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61cc3a; /* fmov.s @(r0,r12),fr2 */
L8c61cc3a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cc3c; /* fadd fr3,fr2 */
L8c61cc3c: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61cc3e; /* fmov.s @(r0,r13),fr3 */
L8c61cc3e: r[0]=36u;goto L8c61cc40; /* mov #0x24,r0 */
L8c61cc40: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cc42; /* fadd fr3,fr2 */
L8c61cc42: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cc44; /* fmov fr2,@(r0,r15) */
L8c61cc44: target=r[3];*pr=0x8c61cc48u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61cc48; /* jsr @r3 */
L8c61cc48: r[3]=rd(m,bus,&io_status,0x8c61cda8u,4);if(io_status)return io_status;goto L8c61cc4a; /* mov.l 0x8c61cda8,r3 */
L8c61cc4a: r[0]=28u;goto L8c61cc4c; /* mov #0x1c,r0 */
L8c61cc4c: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cc4e; /* fmov.s @(r0,r15),fr3 */
L8c61cc4e: r[0]=32u;goto L8c61cc50; /* mov #0x20,r0 */
L8c61cc50: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61cc52; /* fmov.s @r3,fr2 */
L8c61cc52: r[3]+=8u;goto L8c61cc54; /* add #0x8,r3 */
L8c61cc54: r[2]=rd(m,bus,&io_status,0x8c61cdacu,4);if(io_status)return io_status;goto L8c61cc56; /* mov.l 0x8c61cdac,r2 */
L8c61cc56: r[8]+=24u;goto L8c61cc58; /* add #0x18,r8 */
L8c61cc58: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61cc5a; /* fmul fr3,fr2 */
L8c61cc5a: r[1]=rd(m,bus,&io_status,0x8c61cdb0u,4);if(io_status)return io_status;goto L8c61cc5c; /* mov.l 0x8c61cdb0,r1 */
L8c61cc5c: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61cc5e; /* fmov.s @r2,fr3 */
L8c61cc5e: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61cc60; /* fmov.s @r1,fr1 */
L8c61cc60: r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61cc62; /* mov.w @r9+,r4 */
L8c61cc62: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61cc64; /* fsub fr2,fr3 */
L8c61cc64: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cc66; /* fmov.s @(r0,r15),fr2 */
L8c61cc66: r[0]=36u;goto L8c61cc68; /* mov #0x24,r0 */
L8c61cc68: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61cc6a; /* fmul fr2,fr1 */
L8c61cc6a: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cc6c; /* fmov.s @(r0,r15),fr2 */
L8c61cc6c: r[0]=rd(m,bus,&io_status,0x8c61cd9cu,4);if(io_status)return io_status;goto L8c61cc6e; /* mov.l 0x8c61cd9c,r0 */
L8c61cc6e: r[4]=r[4]&65535u;goto L8c61cc70; /* extu.w r4,r4 */
L8c61cc70: r[4]<<=2;goto L8c61cc72; /* shll2 r4 */
L8c61cc72: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61cc74; /* mov.l @r0,r2 */
L8c61cc74: r[4]<<=2;goto L8c61cc76; /* shll2 r4 */
L8c61cc76: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61cc78; /* fsub fr1,fr3 */
L8c61cc78: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61cc7a; /* fmov.s @r3,fr1 */
L8c61cc7a: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61cc7c; /* mov.w @(0x2,r10),r0 */
L8c61cc7c: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61cc7e; /* shll r4 */
L8c61cc7e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61cc80; /* fmul fr2,fr1 */
L8c61cc80: r[4]+=r[2];goto L8c61cc82; /* add r2,r4 */
L8c61cc82: r[3]=r[0];goto L8c61cc84; /* mov r0,r3 */
L8c61cc84: fcopy(f,4u,3u);goto L8c61cc86; /* fmov fr3,fr4 */
L8c61cc86: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61cc88; /* fsub fr1,fr4 */
L8c61cc88: *fpul=r[3];goto L8c61cc8a; /* lds r3,FPUL */
L8c61cc8a: r[0]=24u;goto L8c61cc8c; /* mov #0x18,r0 */
L8c61cc8c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cc8e; /* float FPUL,fr3 */
L8c61cc8e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61cc90; /* fmul fr14,fr3 */
L8c61cc90: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cc92; /* fmov.s fr3,@-r8 */
L8c61cc92: r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61cc94; /* mov.w @r10,r3 */
L8c61cc94: r[10]+=4u;goto L8c61cc96; /* add #0x4,r10 */
L8c61cc96: *fpul=r[3];goto L8c61cc98; /* lds r3,FPUL */
L8c61cc98: r[3]=rd(m,bus,&io_status,0x8c61cdb4u,4);if(io_status)return io_status;goto L8c61cc9a; /* mov.l 0x8c61cdb4,r3 */
L8c61cc9a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cc9c; /* float FPUL,fr3 */
L8c61cc9c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61cc9e; /* fmul fr14,fr3 */
L8c61cc9e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cca0; /* fmov.s fr3,@-r8 */
L8c61cca0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cca2; /* fmov.s @(r0,r4),fr3 */
L8c61cca2: r[0]=20u;goto L8c61cca4; /* mov #0x14,r0 */
L8c61cca4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cca6; /* fmov.s fr3,@-r8 */
L8c61cca6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cca8; /* fmov.s @(r0,r4),fr3 */
L8c61cca8: r[0]=16u;goto L8c61ccaa; /* mov #0x10,r0 */
L8c61ccaa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ccac; /* fmov.s fr3,@-r8 */
L8c61ccac: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ccae; /* fmov.s @(r0,r4),fr3 */
L8c61ccae: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ccb0; /* fmov.s fr3,@-r8 */
L8c61ccb0: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61ccb2; /* mov.l r3,@-r8 */
L8c61ccb2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61ccb4; /* pref @r8 */
L8c61ccb4: r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61ccb6; /* mov.w @r9+,r4 */
L8c61ccb6: r[8]+=56u;goto L8c61ccb8; /* add #0x38,r8 */
L8c61ccb8: r[2]=rd(m,bus,&io_status,0x8c61cd9cu,4);if(io_status)return io_status;goto L8c61ccba; /* mov.l 0x8c61cd9c,r2 */
L8c61ccba: r[4]=r[4]&65535u;goto L8c61ccbc; /* extu.w r4,r4 */
L8c61ccbc: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ccbe; /* mov.w @(0x2,r10),r0 */
L8c61ccbe: r[4]<<=2;goto L8c61ccc0; /* shll2 r4 */
L8c61ccc0: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ccc2; /* mov.l @r2,r3 */
L8c61ccc2: r[4]<<=2;goto L8c61ccc4; /* shll2 r4 */
L8c61ccc4: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61ccc6; /* shll r4 */
L8c61ccc6: r[4]+=r[3];goto L8c61ccc8; /* add r3,r4 */
L8c61ccc8: r[3]=r[0];goto L8c61ccca; /* mov r0,r3 */
L8c61ccca: *fpul=r[3];goto L8c61cccc; /* lds r3,FPUL */
L8c61cccc: r[0]=24u;goto L8c61ccce; /* mov #0x18,r0 */
L8c61ccce: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ccd0; /* float FPUL,fr3 */
L8c61ccd0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ccd2; /* fmul fr14,fr3 */
L8c61ccd2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ccd4; /* fmov.s fr3,@-r8 */
L8c61ccd4: r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ccd6; /* mov.w @r10,r3 */
L8c61ccd6: r[10]+=4u;goto L8c61ccd8; /* add #0x4,r10 */
L8c61ccd8: *fpul=r[3];goto L8c61ccda; /* lds r3,FPUL */
L8c61ccda: r[3]=rd(m,bus,&io_status,0x8c61cdb4u,4);if(io_status)return io_status;goto L8c61ccdc; /* mov.l 0x8c61cdb4,r3 */
L8c61ccdc: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ccde; /* float FPUL,fr3 */
L8c61ccde: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61cce0; /* fmul fr14,fr3 */
L8c61cce0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cce2; /* fmov.s fr3,@-r8 */
L8c61cce2: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cce4; /* fmov.s @(r0,r4),fr3 */
L8c61cce4: r[0]=20u;goto L8c61cce6; /* mov #0x14,r0 */
L8c61cce6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cce8; /* fmov.s fr3,@-r8 */
L8c61cce8: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ccea; /* fmov.s @(r0,r4),fr3 */
L8c61ccea: r[0]=16u;goto L8c61ccec; /* mov #0x10,r0 */
L8c61ccec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ccee; /* fmov.s fr3,@-r8 */
L8c61ccee: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ccf0; /* fmov.s @(r0,r4),fr3 */
L8c61ccf0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ccf2; /* fmov.s fr3,@-r8 */
L8c61ccf2: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61ccf4; /* mov.l r3,@-r8 */
L8c61ccf4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61ccf6; /* pref @r8 */
L8c61ccf6: r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ccf8; /* mov.l @r15,r6 */
L8c61ccf8: r[5]=2u;goto L8c61ccfa; /* mov #0x2,r5 */
L8c61ccfa: r[6]+=4294967295u;goto L8c61ccfc; /* add #-0x1,r6 */
L8c61ccfc: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61ccfe; /* cmp/ge r6,r5 */
L8c61ccfe: take=(*sr&1u);r[8]+=32u;if(take)goto L8c61cd4e;goto L8c61cd02; /* bt/s 0x8c61cd4e */
L8c61cd02: r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61cd04; /* mov.w @r9+,r4 */
L8c61cd04: r[8]+=28u;goto L8c61cd06; /* add #0x1c,r8 */
L8c61cd06: r[2]=rd(m,bus,&io_status,0x8c61cd9cu,4);if(io_status)return io_status;goto L8c61cd08; /* mov.l 0x8c61cd9c,r2 */
L8c61cd08: r[4]=r[4]&65535u;goto L8c61cd0a; /* extu.w r4,r4 */
L8c61cd0a: r[4]<<=2;goto L8c61cd0c; /* shll2 r4 */
L8c61cd0c: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61cd0e; /* mov.l @r2,r3 */
L8c61cd0e: r[4]<<=2;goto L8c61cd10; /* shll2 r4 */
L8c61cd10: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61cd12; /* fmov.s fr4,@-r8 */
L8c61cd12: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61cd14; /* mov.w @(0x2,r10),r0 */
L8c61cd14: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61cd16; /* shll r4 */
L8c61cd16: r[4]+=r[3];goto L8c61cd18; /* add r3,r4 */
L8c61cd18: r[3]=r[0];goto L8c61cd1a; /* mov r0,r3 */
L8c61cd1a: *fpul=r[3];goto L8c61cd1c; /* lds r3,FPUL */
L8c61cd1c: r[0]=24u;goto L8c61cd1e; /* mov #0x18,r0 */
L8c61cd1e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cd20; /* float FPUL,fr3 */
L8c61cd20: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61cd22; /* fmul fr14,fr3 */
L8c61cd22: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd24; /* fmov.s fr3,@-r8 */
L8c61cd24: r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61cd26; /* mov.w @r10,r3 */
L8c61cd26: r[10]+=4u;goto L8c61cd28; /* add #0x4,r10 */
L8c61cd28: *fpul=r[3];goto L8c61cd2a; /* lds r3,FPUL */
L8c61cd2a: r[3]=rd(m,bus,&io_status,0x8c61cdb4u,4);if(io_status)return io_status;goto L8c61cd2c; /* mov.l 0x8c61cdb4,r3 */
L8c61cd2c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cd2e; /* float FPUL,fr3 */
L8c61cd2e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61cd30; /* fmul fr14,fr3 */
L8c61cd30: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd32; /* fmov.s fr3,@-r8 */
L8c61cd32: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cd34; /* fmov.s @(r0,r4),fr3 */
L8c61cd34: r[0]=20u;goto L8c61cd36; /* mov #0x14,r0 */
L8c61cd36: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd38; /* fmov.s fr3,@-r8 */
L8c61cd38: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cd3a; /* fmov.s @(r0,r4),fr3 */
L8c61cd3a: r[0]=16u;goto L8c61cd3c; /* mov #0x10,r0 */
L8c61cd3c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd3e; /* fmov.s fr3,@-r8 */
L8c61cd3e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cd40; /* fmov.s @(r0,r4),fr3 */
L8c61cd40: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd42; /* fmov.s fr3,@-r8 */
L8c61cd42: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61cd44; /* mov.l r3,@-r8 */
L8c61cd44: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61cd46; /* pref @r8 */
L8c61cd46: r[5]+=1u;goto L8c61cd48; /* add #0x1,r5 */
L8c61cd48: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61cd4a; /* cmp/ge r6,r5 */
L8c61cd4a: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61cd02;goto L8c61cd4e; /* bf/s 0x8c61cd02 */
L8c61cd4e: r[4]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61cd50; /* mov.w @r9+,r4 */
L8c61cd50: r[8]+=28u;goto L8c61cd52; /* add #0x1c,r8 */
L8c61cd52: r[2]=rd(m,bus,&io_status,0x8c61cd9cu,4);if(io_status)return io_status;goto L8c61cd54; /* mov.l 0x8c61cd9c,r2 */
L8c61cd54: r[4]=r[4]&65535u;goto L8c61cd56; /* extu.w r4,r4 */
L8c61cd56: r[4]<<=2;goto L8c61cd58; /* shll2 r4 */
L8c61cd58: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61cd5a; /* mov.l @r2,r3 */
L8c61cd5a: r[4]<<=2;goto L8c61cd5c; /* shll2 r4 */
L8c61cd5c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61cd5e; /* fmov.s fr4,@-r8 */
L8c61cd5e: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61cd60; /* mov.w @(0x2,r10),r0 */
L8c61cd60: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61cd62; /* shll r4 */
L8c61cd62: r[4]+=r[3];goto L8c61cd64; /* add r3,r4 */
L8c61cd64: r[3]=r[0];goto L8c61cd66; /* mov r0,r3 */
L8c61cd66: *fpul=r[3];goto L8c61cd68; /* lds r3,FPUL */
L8c61cd68: r[0]=24u;goto L8c61cd6a; /* mov #0x18,r0 */
L8c61cd6a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cd6c; /* float FPUL,fr3 */
L8c61cd6c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61cd6e; /* fmul fr14,fr3 */
L8c61cd6e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd70; /* fmov.s fr3,@-r8 */
L8c61cd70: r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61cd72; /* mov.w @r10,r3 */
L8c61cd72: r[10]+=4u;goto L8c61cd74; /* add #0x4,r10 */
L8c61cd74: *fpul=r[3];goto L8c61cd76; /* lds r3,FPUL */
L8c61cd76: r[3]=rd(m,bus,&io_status,0x8c61cdb8u,4);if(io_status)return io_status;goto L8c61cd78; /* mov.l 0x8c61cdb8,r3 */
L8c61cd78: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cd7a; /* float FPUL,fr3 */
L8c61cd7a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[14],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61cd7c; /* fmul fr14,fr3 */
L8c61cd7c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd7e; /* fmov.s fr3,@-r8 */
L8c61cd7e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cd80; /* fmov.s @(r0,r4),fr3 */
L8c61cd80: r[0]=20u;goto L8c61cd82; /* mov #0x14,r0 */
L8c61cd82: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd84; /* fmov.s fr3,@-r8 */
L8c61cd84: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cd86; /* fmov.s @(r0,r4),fr3 */
L8c61cd86: r[0]=16u;goto L8c61cd88; /* mov #0x10,r0 */
L8c61cd88: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd8a; /* fmov.s fr3,@-r8 */
L8c61cd8a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61cd8c; /* fmov.s @(r0,r4),fr3 */
L8c61cd8c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61cd8e; /* fmov.s fr3,@-r8 */
L8c61cd8e: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61cd90; /* mov.l r3,@-r8 */
L8c61cd90: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61cd92; /* pref @r8 */
L8c61cd92: r[8]+=32u;goto L8c61cef4; /* bra 0x8c61cef4 */
L8c61cdbc: r[0]=28u;goto L8c61cdbe; /* mov #0x1c,r0 */
L8c61cdbe: r[1]=rd(m,bus,&io_status,0x8c61cf74u,4);if(io_status)return io_status;goto L8c61cdc0; /* mov.l 0x8c61cf74,r1 */
L8c61cdc0: r[2]=0u;goto L8c61cdc2; /* mov #0x0,r2 */
L8c61cdc2: r[4]=r[15];goto L8c61cdc4; /* mov r15,r4 */
L8c61cdc4: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61cdc6; /* mov.l r2,@r1 */
L8c61cdc6: if((r[14]&0x1f000000u)!=0x0c000000u)return -3;fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61cdc8; /* fmov.s @r14,fr3 */
L8c61cdc8: fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61cdca; /* fmov.s @r12,fr2 */
L8c61cdca: r[2]=rd(m,bus,&io_status,0x8c61cf78u,4);if(io_status)return io_status;goto L8c61cdcc; /* mov.l 0x8c61cf78,r2 */
L8c61cdcc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cdce; /* fadd fr3,fr2 */
L8c61cdce: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61cdd0; /* fmov.s @r13,fr3 */
L8c61cdd0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cdd2; /* fadd fr3,fr2 */
L8c61cdd2: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cdd4; /* fmov fr2,@(r0,r15) */
L8c61cdd4: r[0]=4u;goto L8c61cdd6; /* mov #0x4,r0 */
L8c61cdd6: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61cdd8; /* fmov.s @(r0,r14),fr3 */
L8c61cdd8: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61cdda; /* fmov.s @(r0,r12),fr2 */
L8c61cdda: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cddc; /* fadd fr3,fr2 */
L8c61cddc: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61cdde; /* fmov.s @(r0,r13),fr3 */
L8c61cdde: r[0]=32u;goto L8c61cde0; /* mov #0x20,r0 */
L8c61cde0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cde2; /* fadd fr3,fr2 */
L8c61cde2: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cde4; /* fmov fr2,@(r0,r15) */
L8c61cde4: r[0]=8u;goto L8c61cde6; /* mov #0x8,r0 */
L8c61cde6: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61cde8; /* fmov.s @(r0,r14),fr3 */
L8c61cde8: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61cdea; /* fmov.s @(r0,r12),fr2 */
L8c61cdea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cdec; /* fadd fr3,fr2 */
L8c61cdec: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61cdee; /* fmov.s @(r0,r13),fr3 */
L8c61cdee: r[0]=36u;goto L8c61cdf0; /* mov #0x24,r0 */
L8c61cdf0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61cdf2; /* fadd fr3,fr2 */
L8c61cdf2: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cdf4; /* fmov fr2,@(r0,r15) */
L8c61cdf4: target=r[2];*pr=0x8c61cdf8u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61cdf8; /* jsr @r2 */
L8c61cdf8: r[3]=rd(m,bus,&io_status,0x8c61cf7cu,4);if(io_status)return io_status;goto L8c61cdfa; /* mov.l 0x8c61cf7c,r3 */
L8c61cdfa: r[0]=28u;goto L8c61cdfc; /* mov #0x1c,r0 */
L8c61cdfc: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61cdfe; /* fmov.s @(r0,r15),fr3 */
L8c61cdfe: r[0]=32u;goto L8c61ce00; /* mov #0x20,r0 */
L8c61ce00: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61ce02; /* fmov.s @r3,fr2 */
L8c61ce02: r[3]+=8u;goto L8c61ce04; /* add #0x8,r3 */
L8c61ce04: r[2]=rd(m,bus,&io_status,0x8c61cf80u,4);if(io_status)return io_status;goto L8c61ce06; /* mov.l 0x8c61cf80,r2 */
L8c61ce06: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ce08; /* fmul fr3,fr2 */
L8c61ce08: r[1]=rd(m,bus,&io_status,0x8c61cf84u,4);if(io_status)return io_status;goto L8c61ce0a; /* mov.l 0x8c61cf84,r1 */
L8c61ce0a: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61ce0c; /* fmov.s @r2,fr3 */
L8c61ce0c: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61ce0e; /* fmov.s @r1,fr1 */
L8c61ce0e: r[12]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61ce10; /* mov.w @r9+,r12 */
L8c61ce10: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ce12; /* fsub fr2,fr3 */
L8c61ce12: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ce14; /* fmov.s @(r0,r15),fr2 */
L8c61ce14: r[0]=36u;goto L8c61ce16; /* mov #0x24,r0 */
L8c61ce16: r[14]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61ce18; /* mov.w @r9+,r14 */
L8c61ce18: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ce1a; /* fmul fr2,fr1 */
L8c61ce1a: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ce1c; /* fmov.s @(r0,r15),fr2 */
L8c61ce1c: r[12]=r[12]&65535u;goto L8c61ce1e; /* extu.w r12,r12 */
L8c61ce1e: r[0]=rd(m,bus,&io_status,0x8c61cf88u,4);if(io_status)return io_status;goto L8c61ce20; /* mov.l 0x8c61cf88,r0 */
L8c61ce20: r[12]<<=2;goto L8c61ce22; /* shll2 r12 */
L8c61ce22: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61ce24; /* mov.l @r0,r2 */
L8c61ce24: r[12]<<=2;goto L8c61ce26; /* shll2 r12 */
L8c61ce26: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ce28; /* fsub fr1,fr3 */
L8c61ce28: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61ce2a; /* fmov.s @r3,fr1 */
L8c61ce2a: r[14]=r[14]&65535u;goto L8c61ce2c; /* extu.w r14,r14 */
L8c61ce2c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ce2e; /* fmul fr2,fr1 */
L8c61ce2e: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61ce30; /* shll r12 */
L8c61ce30: r[14]<<=2;goto L8c61ce32; /* shll2 r14 */
L8c61ce32: fcopy(f,15u,3u);goto L8c61ce34; /* fmov fr3,fr15 */
L8c61ce34: r[12]+=r[2];goto L8c61ce36; /* add r2,r12 */
L8c61ce36: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[1],&f->fpscr,&fr[15],1);if(status)return status;}goto L8c61ce38; /* fsub fr1,fr15 */
L8c61ce38: r[14]<<=2;goto L8c61ce3a; /* shll2 r14 */
L8c61ce3a: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61ce3c; /* mov.l @(0x10,r15),r3 */
L8c61ce3c: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61ce3e; /* shll r14 */
L8c61ce3e: r[14]+=r[2];goto L8c61ce40; /* add r2,r14 */
L8c61ce40: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ce42; /* mov.w @r10,r2 */
L8c61ce42: *fpul=r[2];goto L8c61ce44; /* lds r2,FPUL */
L8c61ce44: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ce46; /* float FPUL,fr3 */
L8c61ce46: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ce48; /* ftrc fr3,FPUL */
L8c61ce48: r[1]=*fpul;goto L8c61ce4a; /* sts FPUL,r1 */
L8c61ce4a: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c61ce4c; /* mov.w r1,@r3 */
L8c61ce4c: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ce4e; /* mov.w @(0x2,r10),r0 */
L8c61ce4e: r[10]+=4u;goto L8c61ce50; /* add #0x4,r10 */
L8c61ce50: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61ce52; /* mov.l @(0x10,r15),r3 */
L8c61ce52: r[2]=r[0];goto L8c61ce54; /* mov r0,r2 */
L8c61ce54: *fpul=r[2];goto L8c61ce56; /* lds r2,FPUL */
L8c61ce56: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ce58; /* float FPUL,fr3 */
L8c61ce58: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ce5a; /* ftrc fr3,FPUL */
L8c61ce5a: r[0]=*fpul;goto L8c61ce5c; /* sts FPUL,r0 */
L8c61ce5c: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61ce5e; /* mov.w r0,@(0x2,r3) */
L8c61ce5e: r[2]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ce60; /* mov.w @r10,r2 */
L8c61ce60: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61ce62; /* mov.l @(0x4,r15),r3 */
L8c61ce62: *fpul=r[2];goto L8c61ce64; /* lds r2,FPUL */
L8c61ce64: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ce66; /* float FPUL,fr3 */
L8c61ce66: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ce68; /* ftrc fr3,FPUL */
L8c61ce68: r[1]=*fpul;goto L8c61ce6a; /* sts FPUL,r1 */
L8c61ce6a: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c61ce6c; /* mov.w r1,@r3 */
L8c61ce6c: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ce6e; /* mov.w @(0x2,r10),r0 */
L8c61ce6e: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61ce70; /* mov.l @(0x4,r15),r3 */
L8c61ce70: r[2]=r[0];goto L8c61ce72; /* mov r0,r2 */
L8c61ce72: *fpul=r[2];goto L8c61ce74; /* lds r2,FPUL */
L8c61ce74: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ce76; /* float FPUL,fr3 */
L8c61ce76: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ce78; /* ftrc fr3,FPUL */
L8c61ce78: r[0]=*fpul;goto L8c61ce7a; /* sts FPUL,r0 */
L8c61ce7a: r[10]+=4u;goto L8c61ce7c; /* add #0x4,r10 */
L8c61ce7c: r[11]=0u;goto L8c61ce7e; /* mov #0x0,r11 */
L8c61ce7e: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61ce80; /* mov.w r0,@(0x2,r3) */
L8c61ce80: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ce82; /* mov.l @r15,r3 */
L8c61ce82: r[3]+=4294967294u;goto L8c61ce84; /* add #-0x2,r3 */
L8c61ce84: *sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c61ce86; /* cmp/pl r3 */
L8c61ce86: take=!(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c61cef4;goto L8c61ce8a; /* bf/s 0x8c61cef4 */
L8c61ce8a: r[13]=sx(m,bus,&io_status,r[9],2);if(io_status)return io_status;r[9]+=2;goto L8c61ce8c; /* mov.w @r9+,r13 */
L8c61ce8c: r[2]=rd(m,bus,&io_status,0x8c61cf88u,4);if(io_status)return io_status;goto L8c61ce8e; /* mov.l 0x8c61cf88,r2 */
L8c61ce8e: r[13]=r[13]&65535u;goto L8c61ce90; /* extu.w r13,r13 */
L8c61ce90: r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61ce92; /* mov.l @(0x8,r15),r1 */
L8c61ce92: r[13]<<=2;goto L8c61ce94; /* shll2 r13 */
L8c61ce94: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ce96; /* mov.l @r2,r3 */
L8c61ce96: r[13]<<=2;goto L8c61ce98; /* shll2 r13 */
L8c61ce98: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61ce9a; /* shll r13 */
L8c61ce9a: r[13]+=r[3];goto L8c61ce9c; /* add r3,r13 */
L8c61ce9c: r[3]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ce9e; /* mov.w @r10,r3 */
L8c61ce9e: *fpul=r[3];goto L8c61cea0; /* lds r3,FPUL */
L8c61cea0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61cea2; /* float FPUL,fr3 */
L8c61cea2: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61cea4; /* ftrc fr3,FPUL */
L8c61cea4: r[0]=*fpul;goto L8c61cea6; /* sts FPUL,r0 */
L8c61cea6: wr(m,bus,&io_status,r[1],r[0],2);if(io_status)return io_status;goto L8c61cea8; /* mov.w r0,@r1 */
L8c61cea8: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ceaa; /* mov.w @(0x2,r10),r0 */
L8c61ceaa: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61ceac; /* mov.l @(0x8,r15),r3 */
L8c61ceac: r[1]=r[0];goto L8c61ceae; /* mov r0,r1 */
L8c61ceae: *fpul=r[1];goto L8c61ceb0; /* lds r1,FPUL */
L8c61ceb0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61ceb2; /* float FPUL,fr3 */
L8c61ceb2: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61ceb4; /* ftrc fr3,FPUL */
L8c61ceb4: r[0]=*fpul;goto L8c61ceb6; /* sts FPUL,r0 */
L8c61ceb6: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61ceb8; /* mov.w r0,@(0x2,r3) */
L8c61ceb8: r[4]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61ceba; /* mov.l @(0xc,r12),r4 */
L8c61ceba: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61cebc; /* mov.l @(0xc,r14),r3 */
L8c61cebc: r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61cebe; /* mov.l @(0xc,r13),r1 */
L8c61cebe: r[4]&=r[3];goto L8c61cec0; /* and r3,r4 */
L8c61cec0: r[3]=1u;goto L8c61cec2; /* mov #0x1,r3 */
L8c61cec2: r[4]&=r[1];goto L8c61cec4; /* and r1,r4 */
L8c61cec4: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61cec6; /* tst r3,r4 */
L8c61cec6: take=!(*sr&1u);r[10]+=4u;if(take)goto L8c61ced8;goto L8c61ceca; /* bf/s 0x8c61ced8 */
L8c61ceca: r[7]=rd(m,bus,&io_status,0x8c61cf8cu,4);if(io_status)return io_status;goto L8c61cecc; /* mov.l 0x8c61cf8c,r7 */
L8c61cecc: r[5]=r[14];goto L8c61cece; /* mov r14,r5 */
L8c61cece: r[1]=rd(m,bus,&io_status,0x8c61cf90u,4);if(io_status)return io_status;goto L8c61ced0; /* mov.l 0x8c61cf90,r1 */
L8c61ced0: r[6]=r[13];goto L8c61ced2; /* mov r13,r6 */
L8c61ced2: fcopy(f,4u,15u);goto L8c61ced4; /* fmov fr15,fr4 */
L8c61ced4: target=r[1];*pr=0x8c61ced8u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ced8; /* jsr @r1 */
L8c61ced8: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61ceda; /* mov.l @(0x4,r15),r2 */
L8c61ceda: r[11]+=1u;goto L8c61cedc; /* add #0x1,r11 */
L8c61cedc: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61cede; /* mov.l @(0x10,r15),r3 */
L8c61cede: r[12]=r[14];goto L8c61cee0; /* mov r14,r12 */
L8c61cee0: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61cee2; /* mov.l @r2,r1 */
L8c61cee2: wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c61cee4; /* mov.l r1,@r3 */
L8c61cee4: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61cee6; /* mov.l @(0x8,r15),r3 */
L8c61cee6: r[0]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61cee8; /* mov.l @(0x4,r15),r0 */
L8c61cee8: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ceea; /* mov.l @r3,r2 */
L8c61ceea: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61ceec; /* mov.l r2,@r0 */
L8c61ceec: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ceee; /* mov.l @r15,r3 */
L8c61ceee: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[3]);goto L8c61cef0; /* cmp/ge r3,r11 */
L8c61cef0: take=!(*sr&1u);r[14]=r[13];if(take)goto L8c61ce8a;goto L8c61cef4; /* bf/s 0x8c61ce8a */
L8c61cef4: r[1]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c61cef6; /* mov.l @(0x14,r15),r1 */
L8c61cef6: r[1]+=1u;goto L8c61cef8; /* add #0x1,r1 */
L8c61cef8: wr(m,bus,&io_status,r[15]+0x14u,r[1],4);if(io_status)return io_status;goto L8c61cefa; /* mov.l r1,@(0x14,r15) */
L8c61cefa: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61cefc; /* mov.l @(0xc,r15),r3 */
L8c61cefc: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61cefe; /* mov.w @(0x2,r3),r0 */
L8c61cefe: r[3]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c61cf00; /* mov.l @(0x14,r15),r3 */
L8c61cf00: r[0]=r[0]&65535u;goto L8c61cf02; /* extu.w r0,r0 */
L8c61cf02: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c61cf04; /* cmp/ge r0,r3 */
L8c61cf04: if((*sr&1u))goto L8c61cf0a;goto L8c61cf06; /* bt 0x8c61cf0a */
L8c61cf06: goto L8c61cb24; /* bra 0x8c61cb24 */
L8c61cf0a: goto L8c61cf4c; /* bra 0x8c61cf4c */
L8c61cf0e: r[1]=rd(m,bus,&io_status,0x8c61cf94u,4);if(io_status)return io_status;goto L8c61cf10; /* mov.l 0x8c61cf94,r1 */
L8c61cf10: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61cf12; /* mov.l @(0xc,r15),r2 */
L8c61cf12: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61cf14; /* mov.l r2,@r1 */
L8c61cf14: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61cf16; /* frchg */
L8c61cf16: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61cf18; /* fmov.s fr0,@-r15 */
L8c61cf18: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61cf1a; /* fmov.s fr1,@-r15 */
L8c61cf1a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61cf1c; /* fmov.s fr2,@-r15 */
L8c61cf1c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61cf1e; /* fmov.s fr3,@-r15 */
L8c61cf1e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61cf20; /* fmov.s fr4,@-r15 */
L8c61cf20: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61cf22; /* fmov.s fr5,@-r15 */
L8c61cf22: r[0]=rd(m,bus,&io_status,0x8c61cf64u,4);if(io_status)return io_status;goto L8c61cf24; /* mov.l 0x8c61cf64,r0 */
L8c61cf24: fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf26; /* fmov.s @r0+,fr0 */
L8c61cf26: fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf28; /* fmov.s @r0+,fr1 */
L8c61cf28: fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c61cf2a; /* fmov.s @r0,fr2 */
L8c61cf2a: r[0]=rd(m,bus,&io_status,0x8c61cf68u,4);if(io_status)return io_status;goto L8c61cf2c; /* mov.l 0x8c61cf68,r0 */
L8c61cf2c: fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c61cf2e; /* fmov.s @r0,fr3 */
L8c61cf2e: r[0]=0x8c61cf6cu;goto L8c61cf30; /* mova 0x8c61cf6c,r0 */
L8c61cf30: fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c61cf32; /* fmov.s @r0,fr4 */
L8c61cf32: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61cf34; /* frchg */
L8c61cf34: r[2]=rd(m,bus,&io_status,0x8c61cf98u,4);if(io_status)return io_status;goto L8c61cf36; /* mov.l 0x8c61cf98,r2 */
L8c61cf36: r[3]=rd(m,bus,&io_status,0x8c61cf94u,4);if(io_status)return io_status;goto L8c61cf38; /* mov.l 0x8c61cf94,r3 */
L8c61cf38: target=r[2];*pr=0x8c61cf3cu;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61cf3c; /* jsr @r2 */
L8c61cf3c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61cf3e; /* frchg */
L8c61cf3e: fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf40; /* fmov.s @r15+,fr5 */
L8c61cf40: fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf42; /* fmov.s @r15+,fr4 */
L8c61cf42: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf44; /* fmov.s @r15+,fr3 */
L8c61cf44: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf46; /* fmov.s @r15+,fr2 */
L8c61cf46: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf48; /* fmov.s @r15+,fr1 */
L8c61cf48: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf4a; /* fmov.s @r15+,fr0 */
L8c61cf4a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61cf4c; /* frchg */
L8c61cf4c: r[15]+=40u;goto L8c61cf4e; /* add #0x28,r15 */
L8c61cf4e: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61cf50; /* lds.l @r15+,PR */
L8c61cf50: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf52; /* fmov.s @r15+,fr14 */
L8c61cf52: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61cf54; /* fmov.s @r15+,fr15 */
L8c61cf54: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61cf56; /* mov.l @r15+,r9 */
L8c61cf56: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61cf58; /* mov.l @r15+,r10 */
L8c61cf58: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61cf5a; /* mov.l @r15+,r11 */
L8c61cf5a: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61cf5c; /* mov.l @r15+,r12 */
L8c61cf5c: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61cf5e; /* mov.l @r15+,r13 */
L8c61cf5e: if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
