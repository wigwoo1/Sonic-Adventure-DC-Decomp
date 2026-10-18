#include "sa_draw_motion_stop.h"
/* Environment-UV/constant-factor primitive family with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_environment_constant_primitive_emit_8c61f108(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 uint32_t entry_pr=*pr;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61f108;
L8c61f108: sa_motion_publish_stop(ctx,0x8c61f108u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61f10a; /* mov.l r14,@-r15 */
L8c61f10a: sa_motion_publish_stop(ctx,0x8c61f10au); r[0]=0x8c61f16cu;goto L8c61f10c; /* mova 0x8c61f16c,r0 */
L8c61f10c: sa_motion_publish_stop(ctx,0x8c61f10cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61f10e; /* mov.l r13,@-r15 */
L8c61f10e: sa_motion_publish_stop(ctx,0x8c61f10eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61f110; /* mov.l r12,@-r15 */
L8c61f110: sa_motion_publish_stop(ctx,0x8c61f110u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61f112; /* mov.l r11,@-r15 */
L8c61f112: sa_motion_publish_stop(ctx,0x8c61f112u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61f114; /* mov.l r10,@-r15 */
L8c61f114: sa_motion_publish_stop(ctx,0x8c61f114u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61f116; /* mov.l r9,@-r15 */
L8c61f116: sa_motion_publish_stop(ctx,0x8c61f116u); r[3]=rd(m,bus,&io_status,0x8c61f178u,4);if(io_status)return io_status;goto L8c61f118; /* mov.l 0x8c61f178,r3 */
L8c61f118: sa_motion_publish_stop(ctx,0x8c61f118u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61f11a; /* sts.l PR,@-r15 */
L8c61f11a: sa_motion_publish_stop(ctx,0x8c61f11au); fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c61f11c; /* fmov.s @r0,fr6 */
L8c61f11c: sa_motion_publish_stop(ctx,0x8c61f11cu); r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61f11e; /* mov.w @r4,r0 */
L8c61f11e: sa_motion_publish_stop(ctx,0x8c61f11eu); r[11]=rd(m,bus,&io_status,0x8c61f174u,4);if(io_status)return io_status;goto L8c61f120; /* mov.l 0x8c61f174,r11 */
L8c61f120: sa_motion_publish_stop(ctx,0x8c61f120u); r[0]=r[0]&65535u;goto L8c61f122; /* extu.w r0,r0 */
L8c61f122: sa_motion_publish_stop(ctx,0x8c61f122u); r[13]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c61f124; /* mov.l @(0x4,r4),r13 */
L8c61f124: sa_motion_publish_stop(ctx,0x8c61f124u); r[0]&=r[3];goto L8c61f126; /* and r3,r0 */
L8c61f126: sa_motion_publish_stop(ctx,0x8c61f126u); r[9]=rd(m,bus,&io_status,0x8c61f170u,4);if(io_status)return io_status;goto L8c61f128; /* mov.l 0x8c61f170,r9 */
L8c61f128: sa_motion_publish_stop(ctx,0x8c61f128u); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61f12a; /* cmp/eq #0x0,r0 */
L8c61f12a: sa_motion_publish_stop(ctx,0x8c61f12au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c61f12c; /* fldi1 fr7 */
L8c61f12c: sa_motion_publish_stop(ctx,0x8c61f12eu); take=(*sr&1u);r[12]=0u;if(take)goto L8c61f152;goto L8c61f130; /* bt/s 0x8c61f152 */
L8c61f130: sa_motion_publish_stop(ctx,0x8c61f130u); r[1]=sx(m,bus,&io_status,0x8c61f16au,2);if(io_status)return io_status;goto L8c61f132; /* mov.w 0x8c61f16a,r1 */
L8c61f132: sa_motion_publish_stop(ctx,0x8c61f132u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61f134; /* cmp/eq r1,r0 */
L8c61f134: sa_motion_publish_stop(ctx,0x8c61f134u); if(!(*sr&1u))goto L8c61f13a;goto L8c61f136; /* bf 0x8c61f13a */
L8c61f136: sa_motion_publish_stop(ctx,0x8c61f138u); goto L8c61f33e; /* bra 0x8c61f33e */
L8c61f13a: sa_motion_publish_stop(ctx,0x8c61f13au); r[1]=rd(m,bus,&io_status,0x8c61f17cu,4);if(io_status)return io_status;goto L8c61f13c; /* mov.l 0x8c61f17c,r1 */
L8c61f13c: sa_motion_publish_stop(ctx,0x8c61f13cu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61f13e; /* cmp/eq r1,r0 */
L8c61f13e: sa_motion_publish_stop(ctx,0x8c61f13eu); if(!(*sr&1u))goto L8c61f144;goto L8c61f140; /* bf 0x8c61f144 */
L8c61f140: sa_motion_publish_stop(ctx,0x8c61f142u); goto L8c61f59c; /* bra 0x8c61f59c */
L8c61f144: sa_motion_publish_stop(ctx,0x8c61f144u); r[1]=rd(m,bus,&io_status,0x8c61f178u,4);if(io_status)return io_status;goto L8c61f146; /* mov.l 0x8c61f178,r1 */
L8c61f146: sa_motion_publish_stop(ctx,0x8c61f146u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61f148; /* cmp/eq r1,r0 */
L8c61f148: sa_motion_publish_stop(ctx,0x8c61f148u); if(!(*sr&1u))goto L8c61f14e;goto L8c61f14a; /* bf 0x8c61f14e */
L8c61f14a: sa_motion_publish_stop(ctx,0x8c61f14cu); goto L8c61f73e; /* bra 0x8c61f73e */
L8c61f14e: sa_motion_publish_stop(ctx,0x8c61f150u); goto L8c61f79c; /* bra 0x8c61f79c */
L8c61f152: sa_motion_publish_stop(ctx,0x8c61f152u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61f154; /* mov.w @(0x2,r4),r0 */
L8c61f154: sa_motion_publish_stop(ctx,0x8c61f154u); r[1]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c61f156; /* mov.l @r11,r1 */
L8c61f156: sa_motion_publish_stop(ctx,0x8c61f156u); r[0]=r[0]&65535u;goto L8c61f158; /* extu.w r0,r0 */
L8c61f158: sa_motion_publish_stop(ctx,0x8c61f158u); r[2]=r[0];goto L8c61f15a; /* mov r0,r2 */
L8c61f15a: sa_motion_publish_stop(ctx,0x8c61f15au); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61f15c; /* shll r0 */
L8c61f15c: sa_motion_publish_stop(ctx,0x8c61f15cu); r[0]+=r[2];goto L8c61f15e; /* add r2,r0 */
L8c61f15e: sa_motion_publish_stop(ctx,0x8c61f15eu); r[1]+=r[0];goto L8c61f160; /* add r0,r1 */
L8c61f160: sa_motion_publish_stop(ctx,0x8c61f160u); wr(m,bus,&io_status,r[11],r[1],4);if(io_status)return io_status;goto L8c61f162; /* mov.l r1,@r11 */
L8c61f162: sa_motion_publish_stop(ctx,0x8c61f164u); r[14]=r[12];goto L8c61f32e; /* bra 0x8c61f32e */
L8c61f180: sa_motion_publish_stop(ctx,0x8c61f180u); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f182; /* mov.w @r13+,r5 */
L8c61f182: sa_motion_publish_stop(ctx,0x8c61f182u); r[0]=16u;goto L8c61f184; /* mov #0x10,r0 */
L8c61f184: sa_motion_publish_stop(ctx,0x8c61f184u); r[3]=rd(m,bus,&io_status,0x8c61f354u,4);if(io_status)return io_status;goto L8c61f186; /* mov.l 0x8c61f354,r3 */
L8c61f186: sa_motion_publish_stop(ctx,0x8c61f186u); r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f188; /* mov.w @r13+,r6 */
L8c61f188: sa_motion_publish_stop(ctx,0x8c61f188u); r[5]=r[5]&65535u;goto L8c61f18a; /* extu.w r5,r5 */
L8c61f18a: sa_motion_publish_stop(ctx,0x8c61f18au); r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f18c; /* mov.l @r3,r12 */
L8c61f18c: sa_motion_publish_stop(ctx,0x8c61f18cu); r[5]<<=2;goto L8c61f18e; /* shll2 r5 */
L8c61f18e: sa_motion_publish_stop(ctx,0x8c61f18eu); r[6]=r[6]&65535u;goto L8c61f190; /* extu.w r6,r6 */
L8c61f190: sa_motion_publish_stop(ctx,0x8c61f190u); r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f192; /* mov.w @r13+,r7 */
L8c61f192: sa_motion_publish_stop(ctx,0x8c61f192u); r[6]<<=2;goto L8c61f194; /* shll2 r6 */
L8c61f194: sa_motion_publish_stop(ctx,0x8c61f194u); r[6]<<=2;goto L8c61f196; /* shll2 r6 */
L8c61f196: sa_motion_publish_stop(ctx,0x8c61f196u); r[5]<<=2;goto L8c61f198; /* shll2 r5 */
L8c61f198: sa_motion_publish_stop(ctx,0x8c61f198u); r[7]=r[7]&65535u;goto L8c61f19a; /* extu.w r7,r7 */
L8c61f19a: sa_motion_publish_stop(ctx,0x8c61f19au); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61f19c; /* shll r5 */
L8c61f19c: sa_motion_publish_stop(ctx,0x8c61f19cu); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61f19e; /* shll r6 */
L8c61f19e: sa_motion_publish_stop(ctx,0x8c61f19eu); r[6]+=r[12];goto L8c61f1a0; /* add r12,r6 */
L8c61f1a0: sa_motion_publish_stop(ctx,0x8c61f1a0u); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f1a2; /* fmov.s @(r0,r6),fr4 */
L8c61f1a2: sa_motion_publish_stop(ctx,0x8c61f1a2u); r[5]+=r[12];goto L8c61f1a4; /* add r12,r5 */
L8c61f1a4: sa_motion_publish_stop(ctx,0x8c61f1a4u); r[7]<<=2;goto L8c61f1a6; /* shll2 r7 */
L8c61f1a6: sa_motion_publish_stop(ctx,0x8c61f1a6u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f1a8; /* fmov.s @(r0,r5),fr3 */
L8c61f1a8: sa_motion_publish_stop(ctx,0x8c61f1a8u); r[7]<<=2;goto L8c61f1aa; /* shll2 r7 */
L8c61f1aa: sa_motion_publish_stop(ctx,0x8c61f1aau); fcopy(f,2u,4u);goto L8c61f1ac; /* fmov fr4,fr2 */
L8c61f1ac: sa_motion_publish_stop(ctx,0x8c61f1acu); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61f1ae; /* shll r7 */
L8c61f1ae: sa_motion_publish_stop(ctx,0x8c61f1aeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61f1b0; /* fsub fr3,fr2 */
L8c61f1b0: sa_motion_publish_stop(ctx,0x8c61f1b0u); r[0]=20u;goto L8c61f1b2; /* mov #0x14,r0 */
L8c61f1b2: sa_motion_publish_stop(ctx,0x8c61f1b2u); r[7]+=r[12];goto L8c61f1b4; /* add r12,r7 */
L8c61f1b4: sa_motion_publish_stop(ctx,0x8c61f1b4u); fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f1b6; /* fmov.s @(r0,r6),fr5 */
L8c61f1b6: sa_motion_publish_stop(ctx,0x8c61f1b6u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f1b8; /* fmov.s @(r0,r7),fr3 */
L8c61f1b8: sa_motion_publish_stop(ctx,0x8c61f1b8u); fcopy(f,1u,5u);goto L8c61f1ba; /* fmov fr5,fr1 */
L8c61f1ba: sa_motion_publish_stop(ctx,0x8c61f1bau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f1bc; /* fsub fr5,fr3 */
L8c61f1bc: sa_motion_publish_stop(ctx,0x8c61f1bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f1be; /* fmul fr3,fr2 */
L8c61f1be: sa_motion_publish_stop(ctx,0x8c61f1beu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f1c0; /* fmov.s @(r0,r5),fr3 */
L8c61f1c0: sa_motion_publish_stop(ctx,0x8c61f1c0u); r[0]=16u;goto L8c61f1c2; /* mov #0x10,r0 */
L8c61f1c2: sa_motion_publish_stop(ctx,0x8c61f1c2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61f1c4; /* fsub fr3,fr1 */
L8c61f1c4: sa_motion_publish_stop(ctx,0x8c61f1c4u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f1c6; /* fmov.s @(r0,r7),fr3 */
L8c61f1c6: sa_motion_publish_stop(ctx,0x8c61f1c6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f1c8; /* fsub fr4,fr3 */
L8c61f1c8: sa_motion_publish_stop(ctx,0x8c61f1c8u); fcopy(f,4u,2u);goto L8c61f1ca; /* fmov fr2,fr4 */
L8c61f1ca: sa_motion_publish_stop(ctx,0x8c61f1cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61f1cc; /* fmul fr3,fr1 */
L8c61f1cc: sa_motion_publish_stop(ctx,0x8c61f1ccu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61f1ce; /* fldi0 fr3 */
L8c61f1ce: sa_motion_publish_stop(ctx,0x8c61f1ceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61f1d0; /* fsub fr1,fr4 */
L8c61f1d0: sa_motion_publish_stop(ctx,0x8c61f1d0u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61f1d2; /* fcmp/gt fr3,fr4 */
L8c61f1d2: sa_motion_publish_stop(ctx,0x8c61f1d4u); take=!(*sr&1u);r[14]+=1u;if(take)goto L8c61f1da;goto L8c61f1d6; /* bf/s 0x8c61f1da */
L8c61f1d6: sa_motion_publish_stop(ctx,0x8c61f1d8u); goto L8c61f32e; /* bra 0x8c61f32e */
L8c61f1da: sa_motion_publish_stop(ctx,0x8c61f1dau); r[0]=4u;goto L8c61f1dc; /* mov #0x4,r0 */
L8c61f1dc: sa_motion_publish_stop(ctx,0x8c61f1dcu); r[8]+=28u;goto L8c61f1de; /* add #0x1c,r8 */
L8c61f1de: sa_motion_publish_stop(ctx,0x8c61f1deu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f1e0; /* fmov.s fr7,@-r8 */
L8c61f1e0: sa_motion_publish_stop(ctx,0x8c61f1e0u); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f1e2; /* mov.l @r9,r3 */
L8c61f1e2: sa_motion_publish_stop(ctx,0x8c61f1e2u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f1e4; /* fmov.s @(r0,r5),fr0 */
L8c61f1e4: sa_motion_publish_stop(ctx,0x8c61f1e4u); r[0]=20u;goto L8c61f1e6; /* mov #0x14,r0 */
L8c61f1e6: sa_motion_publish_stop(ctx,0x8c61f1e6u); r[1]=r[3];goto L8c61f1e8; /* mov r3,r1 */
L8c61f1e8: sa_motion_publish_stop(ctx,0x8c61f1e8u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f1ea; /* fmov.s @(r0,r1),fr3 */
L8c61f1ea: sa_motion_publish_stop(ctx,0x8c61f1eau); r[0]=4u;goto L8c61f1ec; /* mov #0x4,r0 */
L8c61f1ec: sa_motion_publish_stop(ctx,0x8c61f1ecu); r[2]=r[3];goto L8c61f1ee; /* mov r3,r2 */
L8c61f1ee: sa_motion_publish_stop(ctx,0x8c61f1eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f1f0; /* fmul fr0,fr3 */
L8c61f1f0: sa_motion_publish_stop(ctx,0x8c61f1f0u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f1f2; /* fmov.s @(r0,r2),fr2 */
L8c61f1f2: sa_motion_publish_stop(ctx,0x8c61f1f2u); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f1f4; /* fmov.s @r5,fr0 */
L8c61f1f4: sa_motion_publish_stop(ctx,0x8c61f1f4u); r[0]=8u;goto L8c61f1f6; /* mov #0x8,r0 */
L8c61f1f6: sa_motion_publish_stop(ctx,0x8c61f1f6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f1f8; /* fmac fr0,fr2,fr3 */
L8c61f1f8: sa_motion_publish_stop(ctx,0x8c61f1f8u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f1fa; /* fmov.s @(r0,r5),fr0 */
L8c61f1fa: sa_motion_publish_stop(ctx,0x8c61f1fau); r[0]=36u;goto L8c61f1fc; /* mov #0x24,r0 */
L8c61f1fc: sa_motion_publish_stop(ctx,0x8c61f1fcu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f1fe; /* fmov.s @(r0,r3),fr2 */
L8c61f1fe: sa_motion_publish_stop(ctx,0x8c61f1feu); r[0]=4u;goto L8c61f200; /* mov #0x4,r0 */
L8c61f200: sa_motion_publish_stop(ctx,0x8c61f200u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f202; /* fmac fr0,fr2,fr3 */
L8c61f202: sa_motion_publish_stop(ctx,0x8c61f202u); fcopy(f,2u,6u);goto L8c61f204; /* fmov fr6,fr2 */
L8c61f204: sa_motion_publish_stop(ctx,0x8c61f204u); fcopy(f,0u,6u);goto L8c61f206; /* fmov fr6,fr0 */
L8c61f206: sa_motion_publish_stop(ctx,0x8c61f206u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f208; /* fmac fr0,fr3,fr2 */
L8c61f208: sa_motion_publish_stop(ctx,0x8c61f208u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f20a; /* fmov.s fr2,@-r8 */
L8c61f20a: sa_motion_publish_stop(ctx,0x8c61f20au); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f20c; /* mov.l @r9,r3 */
L8c61f20c: sa_motion_publish_stop(ctx,0x8c61f20cu); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f20e; /* fmov.s @(r0,r5),fr0 */
L8c61f20e: sa_motion_publish_stop(ctx,0x8c61f20eu); r[0]=16u;goto L8c61f210; /* mov #0x10,r0 */
L8c61f210: sa_motion_publish_stop(ctx,0x8c61f210u); r[1]=r[3];goto L8c61f212; /* mov r3,r1 */
L8c61f212: sa_motion_publish_stop(ctx,0x8c61f212u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f214; /* fmov.s @(r0,r1),fr3 */
L8c61f214: sa_motion_publish_stop(ctx,0x8c61f214u); r[2]=r[3];goto L8c61f216; /* mov r3,r2 */
L8c61f216: sa_motion_publish_stop(ctx,0x8c61f216u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f218; /* fmul fr0,fr3 */
L8c61f218: sa_motion_publish_stop(ctx,0x8c61f218u); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f21a; /* fmov.s @r5,fr0 */
L8c61f21a: sa_motion_publish_stop(ctx,0x8c61f21au); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f21c; /* fmov.s @r2,fr2 */
L8c61f21c: sa_motion_publish_stop(ctx,0x8c61f21cu); r[0]=8u;goto L8c61f21e; /* mov #0x8,r0 */
L8c61f21e: sa_motion_publish_stop(ctx,0x8c61f21eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f220; /* fmac fr0,fr2,fr3 */
L8c61f220: sa_motion_publish_stop(ctx,0x8c61f220u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f222; /* fmov.s @(r0,r5),fr0 */
L8c61f222: sa_motion_publish_stop(ctx,0x8c61f222u); r[0]=32u;goto L8c61f224; /* mov #0x20,r0 */
L8c61f224: sa_motion_publish_stop(ctx,0x8c61f224u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f226; /* fmov.s @(r0,r3),fr2 */
L8c61f226: sa_motion_publish_stop(ctx,0x8c61f226u); r[0]=24u;goto L8c61f228; /* mov #0x18,r0 */
L8c61f228: sa_motion_publish_stop(ctx,0x8c61f228u); r[3]=rd(m,bus,&io_status,0x8c61f358u,4);if(io_status)return io_status;goto L8c61f22a; /* mov.l 0x8c61f358,r3 */
L8c61f22a: sa_motion_publish_stop(ctx,0x8c61f22au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f22c; /* fmac fr0,fr2,fr3 */
L8c61f22c: sa_motion_publish_stop(ctx,0x8c61f22cu); fcopy(f,0u,6u);goto L8c61f22e; /* fmov fr6,fr0 */
L8c61f22e: sa_motion_publish_stop(ctx,0x8c61f22eu); fcopy(f,2u,6u);goto L8c61f230; /* fmov fr6,fr2 */
L8c61f230: sa_motion_publish_stop(ctx,0x8c61f230u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f232; /* fmac fr0,fr3,fr2 */
L8c61f232: sa_motion_publish_stop(ctx,0x8c61f232u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f234; /* fmov.s fr2,@-r8 */
L8c61f234: sa_motion_publish_stop(ctx,0x8c61f234u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f236; /* fmov.s @(r0,r5),fr3 */
L8c61f236: sa_motion_publish_stop(ctx,0x8c61f236u); r[0]=20u;goto L8c61f238; /* mov #0x14,r0 */
L8c61f238: sa_motion_publish_stop(ctx,0x8c61f238u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f23a; /* fmov.s fr3,@-r8 */
L8c61f23a: sa_motion_publish_stop(ctx,0x8c61f23au); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f23c; /* fmov.s @(r0,r5),fr3 */
L8c61f23c: sa_motion_publish_stop(ctx,0x8c61f23cu); r[0]=16u;goto L8c61f23e; /* mov #0x10,r0 */
L8c61f23e: sa_motion_publish_stop(ctx,0x8c61f23eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f240; /* fmov.s fr3,@-r8 */
L8c61f240: sa_motion_publish_stop(ctx,0x8c61f240u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f242; /* fmov.s @(r0,r5),fr3 */
L8c61f242: sa_motion_publish_stop(ctx,0x8c61f242u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f244; /* fmov.s fr3,@-r8 */
L8c61f244: sa_motion_publish_stop(ctx,0x8c61f244u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f246; /* mov.l r3,@-r8 */
L8c61f246: sa_motion_publish_stop(ctx,0x8c61f246u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f248; /* pref @r8 */
L8c61f248: sa_motion_publish_stop(ctx,0x8c61f248u); r[0]=4u;goto L8c61f24a; /* mov #0x4,r0 */
L8c61f24a: sa_motion_publish_stop(ctx,0x8c61f24au); r[8]+=60u;goto L8c61f24c; /* add #0x3c,r8 */
L8c61f24c: sa_motion_publish_stop(ctx,0x8c61f24cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f24e; /* fmov.s fr7,@-r8 */
L8c61f24e: sa_motion_publish_stop(ctx,0x8c61f24eu); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f250; /* mov.l @r9,r3 */
L8c61f250: sa_motion_publish_stop(ctx,0x8c61f250u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f252; /* fmov.s @(r0,r6),fr0 */
L8c61f252: sa_motion_publish_stop(ctx,0x8c61f252u); r[0]=20u;goto L8c61f254; /* mov #0x14,r0 */
L8c61f254: sa_motion_publish_stop(ctx,0x8c61f254u); r[1]=r[3];goto L8c61f256; /* mov r3,r1 */
L8c61f256: sa_motion_publish_stop(ctx,0x8c61f256u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f258; /* fmov.s @(r0,r1),fr3 */
L8c61f258: sa_motion_publish_stop(ctx,0x8c61f258u); r[0]=4u;goto L8c61f25a; /* mov #0x4,r0 */
L8c61f25a: sa_motion_publish_stop(ctx,0x8c61f25au); r[2]=r[3];goto L8c61f25c; /* mov r3,r2 */
L8c61f25c: sa_motion_publish_stop(ctx,0x8c61f25cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f25e; /* fmul fr0,fr3 */
L8c61f25e: sa_motion_publish_stop(ctx,0x8c61f25eu); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f260; /* fmov.s @(r0,r2),fr2 */
L8c61f260: sa_motion_publish_stop(ctx,0x8c61f260u); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61f262; /* fmov.s @r6,fr0 */
L8c61f262: sa_motion_publish_stop(ctx,0x8c61f262u); r[0]=8u;goto L8c61f264; /* mov #0x8,r0 */
L8c61f264: sa_motion_publish_stop(ctx,0x8c61f264u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f266; /* fmac fr0,fr2,fr3 */
L8c61f266: sa_motion_publish_stop(ctx,0x8c61f266u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f268; /* fmov.s @(r0,r6),fr0 */
L8c61f268: sa_motion_publish_stop(ctx,0x8c61f268u); r[0]=36u;goto L8c61f26a; /* mov #0x24,r0 */
L8c61f26a: sa_motion_publish_stop(ctx,0x8c61f26au); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f26c; /* fmov.s @(r0,r3),fr2 */
L8c61f26c: sa_motion_publish_stop(ctx,0x8c61f26cu); r[0]=4u;goto L8c61f26e; /* mov #0x4,r0 */
L8c61f26e: sa_motion_publish_stop(ctx,0x8c61f26eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f270; /* fmac fr0,fr2,fr3 */
L8c61f270: sa_motion_publish_stop(ctx,0x8c61f270u); fcopy(f,2u,6u);goto L8c61f272; /* fmov fr6,fr2 */
L8c61f272: sa_motion_publish_stop(ctx,0x8c61f272u); fcopy(f,0u,6u);goto L8c61f274; /* fmov fr6,fr0 */
L8c61f274: sa_motion_publish_stop(ctx,0x8c61f274u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f276; /* fmac fr0,fr3,fr2 */
L8c61f276: sa_motion_publish_stop(ctx,0x8c61f276u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f278; /* fmov.s fr2,@-r8 */
L8c61f278: sa_motion_publish_stop(ctx,0x8c61f278u); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f27a; /* mov.l @r9,r3 */
L8c61f27a: sa_motion_publish_stop(ctx,0x8c61f27au); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f27c; /* fmov.s @(r0,r6),fr0 */
L8c61f27c: sa_motion_publish_stop(ctx,0x8c61f27cu); r[0]=16u;goto L8c61f27e; /* mov #0x10,r0 */
L8c61f27e: sa_motion_publish_stop(ctx,0x8c61f27eu); r[1]=r[3];goto L8c61f280; /* mov r3,r1 */
L8c61f280: sa_motion_publish_stop(ctx,0x8c61f280u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f282; /* fmov.s @(r0,r1),fr3 */
L8c61f282: sa_motion_publish_stop(ctx,0x8c61f282u); r[2]=r[3];goto L8c61f284; /* mov r3,r2 */
L8c61f284: sa_motion_publish_stop(ctx,0x8c61f284u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f286; /* fmul fr0,fr3 */
L8c61f286: sa_motion_publish_stop(ctx,0x8c61f286u); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61f288; /* fmov.s @r6,fr0 */
L8c61f288: sa_motion_publish_stop(ctx,0x8c61f288u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f28a; /* fmov.s @r2,fr2 */
L8c61f28a: sa_motion_publish_stop(ctx,0x8c61f28au); r[0]=8u;goto L8c61f28c; /* mov #0x8,r0 */
L8c61f28c: sa_motion_publish_stop(ctx,0x8c61f28cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f28e; /* fmac fr0,fr2,fr3 */
L8c61f28e: sa_motion_publish_stop(ctx,0x8c61f28eu); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f290; /* fmov.s @(r0,r6),fr0 */
L8c61f290: sa_motion_publish_stop(ctx,0x8c61f290u); r[0]=32u;goto L8c61f292; /* mov #0x20,r0 */
L8c61f292: sa_motion_publish_stop(ctx,0x8c61f292u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f294; /* fmov.s @(r0,r3),fr2 */
L8c61f294: sa_motion_publish_stop(ctx,0x8c61f294u); r[0]=24u;goto L8c61f296; /* mov #0x18,r0 */
L8c61f296: sa_motion_publish_stop(ctx,0x8c61f296u); r[3]=rd(m,bus,&io_status,0x8c61f358u,4);if(io_status)return io_status;goto L8c61f298; /* mov.l 0x8c61f358,r3 */
L8c61f298: sa_motion_publish_stop(ctx,0x8c61f298u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f29a; /* fmac fr0,fr2,fr3 */
L8c61f29a: sa_motion_publish_stop(ctx,0x8c61f29au); fcopy(f,0u,6u);goto L8c61f29c; /* fmov fr6,fr0 */
L8c61f29c: sa_motion_publish_stop(ctx,0x8c61f29cu); fcopy(f,2u,6u);goto L8c61f29e; /* fmov fr6,fr2 */
L8c61f29e: sa_motion_publish_stop(ctx,0x8c61f29eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f2a0; /* fmac fr0,fr3,fr2 */
L8c61f2a0: sa_motion_publish_stop(ctx,0x8c61f2a0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f2a2; /* fmov.s fr2,@-r8 */
L8c61f2a2: sa_motion_publish_stop(ctx,0x8c61f2a2u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f2a4; /* fmov.s @(r0,r6),fr3 */
L8c61f2a4: sa_motion_publish_stop(ctx,0x8c61f2a4u); r[0]=20u;goto L8c61f2a6; /* mov #0x14,r0 */
L8c61f2a6: sa_motion_publish_stop(ctx,0x8c61f2a6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f2a8; /* fmov.s fr3,@-r8 */
L8c61f2a8: sa_motion_publish_stop(ctx,0x8c61f2a8u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f2aa; /* fmov.s @(r0,r6),fr3 */
L8c61f2aa: sa_motion_publish_stop(ctx,0x8c61f2aau); r[0]=16u;goto L8c61f2ac; /* mov #0x10,r0 */
L8c61f2ac: sa_motion_publish_stop(ctx,0x8c61f2acu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f2ae; /* fmov.s fr3,@-r8 */
L8c61f2ae: sa_motion_publish_stop(ctx,0x8c61f2aeu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f2b0; /* fmov.s @(r0,r6),fr3 */
L8c61f2b0: sa_motion_publish_stop(ctx,0x8c61f2b0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f2b2; /* fmov.s fr3,@-r8 */
L8c61f2b2: sa_motion_publish_stop(ctx,0x8c61f2b2u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f2b4; /* mov.l r3,@-r8 */
L8c61f2b4: sa_motion_publish_stop(ctx,0x8c61f2b4u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f2b6; /* pref @r8 */
L8c61f2b6: sa_motion_publish_stop(ctx,0x8c61f2b6u); r[0]=4u;goto L8c61f2b8; /* mov #0x4,r0 */
L8c61f2b8: sa_motion_publish_stop(ctx,0x8c61f2b8u); r[8]+=60u;goto L8c61f2ba; /* add #0x3c,r8 */
L8c61f2ba: sa_motion_publish_stop(ctx,0x8c61f2bau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f2bc; /* fmov.s fr7,@-r8 */
L8c61f2bc: sa_motion_publish_stop(ctx,0x8c61f2bcu); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f2be; /* mov.l @r9,r3 */
L8c61f2be: sa_motion_publish_stop(ctx,0x8c61f2beu); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f2c0; /* fmov.s @(r0,r7),fr0 */
L8c61f2c0: sa_motion_publish_stop(ctx,0x8c61f2c0u); r[0]=20u;goto L8c61f2c2; /* mov #0x14,r0 */
L8c61f2c2: sa_motion_publish_stop(ctx,0x8c61f2c2u); r[1]=r[3];goto L8c61f2c4; /* mov r3,r1 */
L8c61f2c4: sa_motion_publish_stop(ctx,0x8c61f2c4u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f2c6; /* fmov.s @(r0,r1),fr3 */
L8c61f2c6: sa_motion_publish_stop(ctx,0x8c61f2c6u); r[0]=4u;goto L8c61f2c8; /* mov #0x4,r0 */
L8c61f2c8: sa_motion_publish_stop(ctx,0x8c61f2c8u); r[2]=r[3];goto L8c61f2ca; /* mov r3,r2 */
L8c61f2ca: sa_motion_publish_stop(ctx,0x8c61f2cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f2cc; /* fmul fr0,fr3 */
L8c61f2cc: sa_motion_publish_stop(ctx,0x8c61f2ccu); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f2ce; /* fmov.s @(r0,r2),fr2 */
L8c61f2ce: sa_motion_publish_stop(ctx,0x8c61f2ceu); fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c61f2d0; /* fmov.s @r7,fr0 */
L8c61f2d0: sa_motion_publish_stop(ctx,0x8c61f2d0u); r[0]=8u;goto L8c61f2d2; /* mov #0x8,r0 */
L8c61f2d2: sa_motion_publish_stop(ctx,0x8c61f2d2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f2d4; /* fmac fr0,fr2,fr3 */
L8c61f2d4: sa_motion_publish_stop(ctx,0x8c61f2d4u); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f2d6; /* fmov.s @(r0,r7),fr0 */
L8c61f2d6: sa_motion_publish_stop(ctx,0x8c61f2d6u); r[0]=36u;goto L8c61f2d8; /* mov #0x24,r0 */
L8c61f2d8: sa_motion_publish_stop(ctx,0x8c61f2d8u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f2da; /* fmov.s @(r0,r3),fr2 */
L8c61f2da: sa_motion_publish_stop(ctx,0x8c61f2dau); r[0]=4u;goto L8c61f2dc; /* mov #0x4,r0 */
L8c61f2dc: sa_motion_publish_stop(ctx,0x8c61f2dcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f2de; /* fmac fr0,fr2,fr3 */
L8c61f2de: sa_motion_publish_stop(ctx,0x8c61f2deu); fcopy(f,2u,6u);goto L8c61f2e0; /* fmov fr6,fr2 */
L8c61f2e0: sa_motion_publish_stop(ctx,0x8c61f2e0u); fcopy(f,0u,6u);goto L8c61f2e2; /* fmov fr6,fr0 */
L8c61f2e2: sa_motion_publish_stop(ctx,0x8c61f2e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f2e4; /* fmac fr0,fr3,fr2 */
L8c61f2e4: sa_motion_publish_stop(ctx,0x8c61f2e4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f2e6; /* fmov.s fr2,@-r8 */
L8c61f2e6: sa_motion_publish_stop(ctx,0x8c61f2e6u); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f2e8; /* mov.l @r9,r3 */
L8c61f2e8: sa_motion_publish_stop(ctx,0x8c61f2e8u); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f2ea; /* fmov.s @(r0,r7),fr0 */
L8c61f2ea: sa_motion_publish_stop(ctx,0x8c61f2eau); r[0]=16u;goto L8c61f2ec; /* mov #0x10,r0 */
L8c61f2ec: sa_motion_publish_stop(ctx,0x8c61f2ecu); r[1]=r[3];goto L8c61f2ee; /* mov r3,r1 */
L8c61f2ee: sa_motion_publish_stop(ctx,0x8c61f2eeu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f2f0; /* fmov.s @(r0,r1),fr3 */
L8c61f2f0: sa_motion_publish_stop(ctx,0x8c61f2f0u); r[2]=r[3];goto L8c61f2f2; /* mov r3,r2 */
L8c61f2f2: sa_motion_publish_stop(ctx,0x8c61f2f2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f2f4; /* fmul fr0,fr3 */
L8c61f2f4: sa_motion_publish_stop(ctx,0x8c61f2f4u); fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c61f2f6; /* fmov.s @r7,fr0 */
L8c61f2f6: sa_motion_publish_stop(ctx,0x8c61f2f6u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f2f8; /* fmov.s @r2,fr2 */
L8c61f2f8: sa_motion_publish_stop(ctx,0x8c61f2f8u); r[0]=8u;goto L8c61f2fa; /* mov #0x8,r0 */
L8c61f2fa: sa_motion_publish_stop(ctx,0x8c61f2fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f2fc; /* fmac fr0,fr2,fr3 */
L8c61f2fc: sa_motion_publish_stop(ctx,0x8c61f2fcu); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f2fe; /* fmov.s @(r0,r7),fr0 */
L8c61f2fe: sa_motion_publish_stop(ctx,0x8c61f2feu); r[0]=32u;goto L8c61f300; /* mov #0x20,r0 */
L8c61f300: sa_motion_publish_stop(ctx,0x8c61f300u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f302; /* fmov.s @(r0,r3),fr2 */
L8c61f302: sa_motion_publish_stop(ctx,0x8c61f302u); r[0]=24u;goto L8c61f304; /* mov #0x18,r0 */
L8c61f304: sa_motion_publish_stop(ctx,0x8c61f304u); r[3]=rd(m,bus,&io_status,0x8c61f35cu,4);if(io_status)return io_status;goto L8c61f306; /* mov.l 0x8c61f35c,r3 */
L8c61f306: sa_motion_publish_stop(ctx,0x8c61f306u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f308; /* fmac fr0,fr2,fr3 */
L8c61f308: sa_motion_publish_stop(ctx,0x8c61f308u); fcopy(f,0u,6u);goto L8c61f30a; /* fmov fr6,fr0 */
L8c61f30a: sa_motion_publish_stop(ctx,0x8c61f30au); fcopy(f,2u,6u);goto L8c61f30c; /* fmov fr6,fr2 */
L8c61f30c: sa_motion_publish_stop(ctx,0x8c61f30cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f30e; /* fmac fr0,fr3,fr2 */
L8c61f30e: sa_motion_publish_stop(ctx,0x8c61f30eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f310; /* fmov.s fr2,@-r8 */
L8c61f310: sa_motion_publish_stop(ctx,0x8c61f310u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f312; /* fmov.s @(r0,r7),fr3 */
L8c61f312: sa_motion_publish_stop(ctx,0x8c61f312u); r[0]=20u;goto L8c61f314; /* mov #0x14,r0 */
L8c61f314: sa_motion_publish_stop(ctx,0x8c61f314u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f316; /* fmov.s fr3,@-r8 */
L8c61f316: sa_motion_publish_stop(ctx,0x8c61f316u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f318; /* fmov.s @(r0,r7),fr3 */
L8c61f318: sa_motion_publish_stop(ctx,0x8c61f318u); r[0]=16u;goto L8c61f31a; /* mov #0x10,r0 */
L8c61f31a: sa_motion_publish_stop(ctx,0x8c61f31au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f31c; /* fmov.s fr3,@-r8 */
L8c61f31c: sa_motion_publish_stop(ctx,0x8c61f31cu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f31e; /* fmov.s @(r0,r7),fr3 */
L8c61f31e: sa_motion_publish_stop(ctx,0x8c61f31eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f320; /* fmov.s fr3,@-r8 */
L8c61f320: sa_motion_publish_stop(ctx,0x8c61f320u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f322; /* mov.l r3,@-r8 */
L8c61f322: sa_motion_publish_stop(ctx,0x8c61f322u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f324; /* pref @r8 */
L8c61f324: sa_motion_publish_stop(ctx,0x8c61f324u); r[2]=rd(m,bus,&io_status,0x8c61f360u,4);if(io_status)return io_status;goto L8c61f326; /* mov.l 0x8c61f360,r2 */
L8c61f326: sa_motion_publish_stop(ctx,0x8c61f326u); r[8]+=32u;goto L8c61f328; /* add #0x20,r8 */
L8c61f328: sa_motion_publish_stop(ctx,0x8c61f328u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61f32a; /* mov.l @r2,r3 */
L8c61f32a: sa_motion_publish_stop(ctx,0x8c61f32au); r[3]+=3u;goto L8c61f32c; /* add #0x3,r3 */
L8c61f32c: sa_motion_publish_stop(ctx,0x8c61f32cu); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61f32e; /* mov.l r3,@r2 */
L8c61f32e: sa_motion_publish_stop(ctx,0x8c61f32eu); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61f330; /* mov.w @(0x2,r4),r0 */
L8c61f330: sa_motion_publish_stop(ctx,0x8c61f330u); r[0]=r[0]&65535u;goto L8c61f332; /* extu.w r0,r0 */
L8c61f332: sa_motion_publish_stop(ctx,0x8c61f332u); *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c61f334; /* cmp/ge r0,r14 */
L8c61f334: sa_motion_publish_stop(ctx,0x8c61f334u); if((*sr&1u))goto L8c61f33a;goto L8c61f336; /* bt 0x8c61f33a */
L8c61f336: sa_motion_publish_stop(ctx,0x8c61f338u); goto L8c61f180; /* bra 0x8c61f180 */
L8c61f33a: sa_motion_publish_stop(ctx,0x8c61f33cu); goto L8c61f79c; /* bra 0x8c61f79c */
L8c61f33e: sa_motion_publish_stop(ctx,0x8c61f33eu); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61f340; /* mov.w @(0x2,r4),r0 */
L8c61f340: sa_motion_publish_stop(ctx,0x8c61f340u); r[2]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c61f342; /* mov.l @r11,r2 */
L8c61f342: sa_motion_publish_stop(ctx,0x8c61f342u); r[0]=r[0]&65535u;goto L8c61f344; /* extu.w r0,r0 */
L8c61f344: sa_motion_publish_stop(ctx,0x8c61f344u); r[0]<<=2;goto L8c61f346; /* shll2 r0 */
L8c61f346: sa_motion_publish_stop(ctx,0x8c61f346u); r[2]+=r[0];goto L8c61f348; /* add r0,r2 */
L8c61f348: sa_motion_publish_stop(ctx,0x8c61f348u); wr(m,bus,&io_status,r[11],r[2],4);if(io_status)return io_status;goto L8c61f34a; /* mov.l r2,@r11 */
L8c61f34a: sa_motion_publish_stop(ctx,0x8c61f34cu); r[11]=r[12];goto L8c61f58c; /* bra 0x8c61f58c */
L8c61f364: sa_motion_publish_stop(ctx,0x8c61f364u); r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f366; /* mov.w @r13+,r6 */
L8c61f366: sa_motion_publish_stop(ctx,0x8c61f366u); r[0]=16u;goto L8c61f368; /* mov #0x10,r0 */
L8c61f368: sa_motion_publish_stop(ctx,0x8c61f368u); r[3]=rd(m,bus,&io_status,0x8c61f604u,4);if(io_status)return io_status;goto L8c61f36a; /* mov.l 0x8c61f604,r3 */
L8c61f36a: sa_motion_publish_stop(ctx,0x8c61f36au); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f36c; /* mov.w @r13+,r5 */
L8c61f36c: sa_motion_publish_stop(ctx,0x8c61f36cu); r[6]=r[6]&65535u;goto L8c61f36e; /* extu.w r6,r6 */
L8c61f36e: sa_motion_publish_stop(ctx,0x8c61f36eu); r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f370; /* mov.l @r3,r12 */
L8c61f370: sa_motion_publish_stop(ctx,0x8c61f370u); r[6]<<=2;goto L8c61f372; /* shll2 r6 */
L8c61f372: sa_motion_publish_stop(ctx,0x8c61f372u); r[5]=r[5]&65535u;goto L8c61f374; /* extu.w r5,r5 */
L8c61f374: sa_motion_publish_stop(ctx,0x8c61f374u); r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f376; /* mov.w @r13+,r7 */
L8c61f376: sa_motion_publish_stop(ctx,0x8c61f376u); r[5]<<=2;goto L8c61f378; /* shll2 r5 */
L8c61f378: sa_motion_publish_stop(ctx,0x8c61f378u); r[5]<<=2;goto L8c61f37a; /* shll2 r5 */
L8c61f37a: sa_motion_publish_stop(ctx,0x8c61f37au); r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f37c; /* mov.w @r13+,r14 */
L8c61f37c: sa_motion_publish_stop(ctx,0x8c61f37cu); r[6]<<=2;goto L8c61f37e; /* shll2 r6 */
L8c61f37e: sa_motion_publish_stop(ctx,0x8c61f37eu); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61f380; /* shll r6 */
L8c61f380: sa_motion_publish_stop(ctx,0x8c61f380u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61f382; /* shll r5 */
L8c61f382: sa_motion_publish_stop(ctx,0x8c61f382u); r[5]+=r[12];goto L8c61f384; /* add r12,r5 */
L8c61f384: sa_motion_publish_stop(ctx,0x8c61f384u); r[6]+=r[12];goto L8c61f386; /* add r12,r6 */
L8c61f386: sa_motion_publish_stop(ctx,0x8c61f386u); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f388; /* fmov.s @(r0,r5),fr4 */
L8c61f388: sa_motion_publish_stop(ctx,0x8c61f388u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f38a; /* fmov.s @(r0,r6),fr3 */
L8c61f38a: sa_motion_publish_stop(ctx,0x8c61f38au); r[7]=r[7]&65535u;goto L8c61f38c; /* extu.w r7,r7 */
L8c61f38c: sa_motion_publish_stop(ctx,0x8c61f38cu); fcopy(f,2u,4u);goto L8c61f38e; /* fmov fr4,fr2 */
L8c61f38e: sa_motion_publish_stop(ctx,0x8c61f38eu); r[7]<<=2;goto L8c61f390; /* shll2 r7 */
L8c61f390: sa_motion_publish_stop(ctx,0x8c61f390u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61f392; /* fsub fr3,fr2 */
L8c61f392: sa_motion_publish_stop(ctx,0x8c61f392u); r[14]=r[14]&65535u;goto L8c61f394; /* extu.w r14,r14 */
L8c61f394: sa_motion_publish_stop(ctx,0x8c61f394u); r[7]<<=2;goto L8c61f396; /* shll2 r7 */
L8c61f396: sa_motion_publish_stop(ctx,0x8c61f396u); r[14]<<=2;goto L8c61f398; /* shll2 r14 */
L8c61f398: sa_motion_publish_stop(ctx,0x8c61f398u); r[0]=20u;goto L8c61f39a; /* mov #0x14,r0 */
L8c61f39a: sa_motion_publish_stop(ctx,0x8c61f39au); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f39c; /* fmov.s @(r0,r5),fr5 */
L8c61f39c: sa_motion_publish_stop(ctx,0x8c61f39cu); r[14]<<=2;goto L8c61f39e; /* shll2 r14 */
L8c61f39e: sa_motion_publish_stop(ctx,0x8c61f39eu); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61f3a0; /* shll r7 */
L8c61f3a0: sa_motion_publish_stop(ctx,0x8c61f3a0u); r[7]+=r[12];goto L8c61f3a2; /* add r12,r7 */
L8c61f3a2: sa_motion_publish_stop(ctx,0x8c61f3a2u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61f3a4; /* shll r14 */
L8c61f3a4: sa_motion_publish_stop(ctx,0x8c61f3a4u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f3a6; /* fmov.s @(r0,r7),fr3 */
L8c61f3a6: sa_motion_publish_stop(ctx,0x8c61f3a6u); fcopy(f,1u,5u);goto L8c61f3a8; /* fmov fr5,fr1 */
L8c61f3a8: sa_motion_publish_stop(ctx,0x8c61f3a8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f3aa; /* fsub fr5,fr3 */
L8c61f3aa: sa_motion_publish_stop(ctx,0x8c61f3aau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f3ac; /* fmul fr3,fr2 */
L8c61f3ac: sa_motion_publish_stop(ctx,0x8c61f3acu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f3ae; /* fmov.s @(r0,r6),fr3 */
L8c61f3ae: sa_motion_publish_stop(ctx,0x8c61f3aeu); r[0]=16u;goto L8c61f3b0; /* mov #0x10,r0 */
L8c61f3b0: sa_motion_publish_stop(ctx,0x8c61f3b0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61f3b2; /* fsub fr3,fr1 */
L8c61f3b2: sa_motion_publish_stop(ctx,0x8c61f3b2u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f3b4; /* fmov.s @(r0,r7),fr3 */
L8c61f3b4: sa_motion_publish_stop(ctx,0x8c61f3b4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f3b6; /* fsub fr4,fr3 */
L8c61f3b6: sa_motion_publish_stop(ctx,0x8c61f3b6u); fcopy(f,4u,2u);goto L8c61f3b8; /* fmov fr2,fr4 */
L8c61f3b8: sa_motion_publish_stop(ctx,0x8c61f3b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61f3ba; /* fmul fr3,fr1 */
L8c61f3ba: sa_motion_publish_stop(ctx,0x8c61f3bau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61f3bc; /* fldi0 fr3 */
L8c61f3bc: sa_motion_publish_stop(ctx,0x8c61f3bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61f3be; /* fsub fr1,fr4 */
L8c61f3be: sa_motion_publish_stop(ctx,0x8c61f3beu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61f3c0; /* fcmp/gt fr3,fr4 */
L8c61f3c0: sa_motion_publish_stop(ctx,0x8c61f3c2u); take=!(*sr&1u);r[14]+=r[12];if(take)goto L8c61f3c8;goto L8c61f3c4; /* bf/s 0x8c61f3c8 */
L8c61f3c4: sa_motion_publish_stop(ctx,0x8c61f3c6u); goto L8c61f58a; /* bra 0x8c61f58a */
L8c61f3c8: sa_motion_publish_stop(ctx,0x8c61f3c8u); r[0]=4u;goto L8c61f3ca; /* mov #0x4,r0 */
L8c61f3ca: sa_motion_publish_stop(ctx,0x8c61f3cau); r[8]+=28u;goto L8c61f3cc; /* add #0x1c,r8 */
L8c61f3cc: sa_motion_publish_stop(ctx,0x8c61f3ccu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f3ce; /* fmov.s fr7,@-r8 */
L8c61f3ce: sa_motion_publish_stop(ctx,0x8c61f3ceu); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f3d0; /* mov.l @r9,r3 */
L8c61f3d0: sa_motion_publish_stop(ctx,0x8c61f3d0u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f3d2; /* fmov.s @(r0,r6),fr0 */
L8c61f3d2: sa_motion_publish_stop(ctx,0x8c61f3d2u); r[0]=20u;goto L8c61f3d4; /* mov #0x14,r0 */
L8c61f3d4: sa_motion_publish_stop(ctx,0x8c61f3d4u); r[1]=r[3];goto L8c61f3d6; /* mov r3,r1 */
L8c61f3d6: sa_motion_publish_stop(ctx,0x8c61f3d6u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f3d8; /* fmov.s @(r0,r1),fr3 */
L8c61f3d8: sa_motion_publish_stop(ctx,0x8c61f3d8u); r[0]=4u;goto L8c61f3da; /* mov #0x4,r0 */
L8c61f3da: sa_motion_publish_stop(ctx,0x8c61f3dau); r[2]=r[3];goto L8c61f3dc; /* mov r3,r2 */
L8c61f3dc: sa_motion_publish_stop(ctx,0x8c61f3dcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f3de; /* fmul fr0,fr3 */
L8c61f3de: sa_motion_publish_stop(ctx,0x8c61f3deu); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f3e0; /* fmov.s @(r0,r2),fr2 */
L8c61f3e0: sa_motion_publish_stop(ctx,0x8c61f3e0u); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61f3e2; /* fmov.s @r6,fr0 */
L8c61f3e2: sa_motion_publish_stop(ctx,0x8c61f3e2u); r[0]=8u;goto L8c61f3e4; /* mov #0x8,r0 */
L8c61f3e4: sa_motion_publish_stop(ctx,0x8c61f3e4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f3e6; /* fmac fr0,fr2,fr3 */
L8c61f3e6: sa_motion_publish_stop(ctx,0x8c61f3e6u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f3e8; /* fmov.s @(r0,r6),fr0 */
L8c61f3e8: sa_motion_publish_stop(ctx,0x8c61f3e8u); r[0]=36u;goto L8c61f3ea; /* mov #0x24,r0 */
L8c61f3ea: sa_motion_publish_stop(ctx,0x8c61f3eau); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f3ec; /* fmov.s @(r0,r3),fr2 */
L8c61f3ec: sa_motion_publish_stop(ctx,0x8c61f3ecu); r[0]=4u;goto L8c61f3ee; /* mov #0x4,r0 */
L8c61f3ee: sa_motion_publish_stop(ctx,0x8c61f3eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f3f0; /* fmac fr0,fr2,fr3 */
L8c61f3f0: sa_motion_publish_stop(ctx,0x8c61f3f0u); fcopy(f,2u,6u);goto L8c61f3f2; /* fmov fr6,fr2 */
L8c61f3f2: sa_motion_publish_stop(ctx,0x8c61f3f2u); fcopy(f,0u,6u);goto L8c61f3f4; /* fmov fr6,fr0 */
L8c61f3f4: sa_motion_publish_stop(ctx,0x8c61f3f4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f3f6; /* fmac fr0,fr3,fr2 */
L8c61f3f6: sa_motion_publish_stop(ctx,0x8c61f3f6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f3f8; /* fmov.s fr2,@-r8 */
L8c61f3f8: sa_motion_publish_stop(ctx,0x8c61f3f8u); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f3fa; /* mov.l @r9,r3 */
L8c61f3fa: sa_motion_publish_stop(ctx,0x8c61f3fau); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f3fc; /* fmov.s @(r0,r6),fr0 */
L8c61f3fc: sa_motion_publish_stop(ctx,0x8c61f3fcu); r[0]=16u;goto L8c61f3fe; /* mov #0x10,r0 */
L8c61f3fe: sa_motion_publish_stop(ctx,0x8c61f3feu); r[1]=r[3];goto L8c61f400; /* mov r3,r1 */
L8c61f400: sa_motion_publish_stop(ctx,0x8c61f400u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f402; /* fmov.s @(r0,r1),fr3 */
L8c61f402: sa_motion_publish_stop(ctx,0x8c61f402u); r[2]=r[3];goto L8c61f404; /* mov r3,r2 */
L8c61f404: sa_motion_publish_stop(ctx,0x8c61f404u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f406; /* fmul fr0,fr3 */
L8c61f406: sa_motion_publish_stop(ctx,0x8c61f406u); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61f408; /* fmov.s @r6,fr0 */
L8c61f408: sa_motion_publish_stop(ctx,0x8c61f408u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f40a; /* fmov.s @r2,fr2 */
L8c61f40a: sa_motion_publish_stop(ctx,0x8c61f40au); r[0]=8u;goto L8c61f40c; /* mov #0x8,r0 */
L8c61f40c: sa_motion_publish_stop(ctx,0x8c61f40cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f40e; /* fmac fr0,fr2,fr3 */
L8c61f40e: sa_motion_publish_stop(ctx,0x8c61f40eu); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f410; /* fmov.s @(r0,r6),fr0 */
L8c61f410: sa_motion_publish_stop(ctx,0x8c61f410u); r[0]=32u;goto L8c61f412; /* mov #0x20,r0 */
L8c61f412: sa_motion_publish_stop(ctx,0x8c61f412u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f414; /* fmov.s @(r0,r3),fr2 */
L8c61f414: sa_motion_publish_stop(ctx,0x8c61f414u); r[0]=24u;goto L8c61f416; /* mov #0x18,r0 */
L8c61f416: sa_motion_publish_stop(ctx,0x8c61f416u); r[3]=rd(m,bus,&io_status,0x8c61f608u,4);if(io_status)return io_status;goto L8c61f418; /* mov.l 0x8c61f608,r3 */
L8c61f418: sa_motion_publish_stop(ctx,0x8c61f418u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f41a; /* fmac fr0,fr2,fr3 */
L8c61f41a: sa_motion_publish_stop(ctx,0x8c61f41au); fcopy(f,0u,6u);goto L8c61f41c; /* fmov fr6,fr0 */
L8c61f41c: sa_motion_publish_stop(ctx,0x8c61f41cu); fcopy(f,2u,6u);goto L8c61f41e; /* fmov fr6,fr2 */
L8c61f41e: sa_motion_publish_stop(ctx,0x8c61f41eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f420; /* fmac fr0,fr3,fr2 */
L8c61f420: sa_motion_publish_stop(ctx,0x8c61f420u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f422; /* fmov.s fr2,@-r8 */
L8c61f422: sa_motion_publish_stop(ctx,0x8c61f422u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f424; /* fmov.s @(r0,r6),fr3 */
L8c61f424: sa_motion_publish_stop(ctx,0x8c61f424u); r[0]=20u;goto L8c61f426; /* mov #0x14,r0 */
L8c61f426: sa_motion_publish_stop(ctx,0x8c61f426u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f428; /* fmov.s fr3,@-r8 */
L8c61f428: sa_motion_publish_stop(ctx,0x8c61f428u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f42a; /* fmov.s @(r0,r6),fr3 */
L8c61f42a: sa_motion_publish_stop(ctx,0x8c61f42au); r[0]=16u;goto L8c61f42c; /* mov #0x10,r0 */
L8c61f42c: sa_motion_publish_stop(ctx,0x8c61f42cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f42e; /* fmov.s fr3,@-r8 */
L8c61f42e: sa_motion_publish_stop(ctx,0x8c61f42eu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f430; /* fmov.s @(r0,r6),fr3 */
L8c61f430: sa_motion_publish_stop(ctx,0x8c61f430u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f432; /* fmov.s fr3,@-r8 */
L8c61f432: sa_motion_publish_stop(ctx,0x8c61f432u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f434; /* mov.l r3,@-r8 */
L8c61f434: sa_motion_publish_stop(ctx,0x8c61f434u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f436; /* pref @r8 */
L8c61f436: sa_motion_publish_stop(ctx,0x8c61f436u); r[0]=4u;goto L8c61f438; /* mov #0x4,r0 */
L8c61f438: sa_motion_publish_stop(ctx,0x8c61f438u); r[8]+=60u;goto L8c61f43a; /* add #0x3c,r8 */
L8c61f43a: sa_motion_publish_stop(ctx,0x8c61f43au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f43c; /* fmov.s fr7,@-r8 */
L8c61f43c: sa_motion_publish_stop(ctx,0x8c61f43cu); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f43e; /* mov.l @r9,r3 */
L8c61f43e: sa_motion_publish_stop(ctx,0x8c61f43eu); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f440; /* fmov.s @(r0,r5),fr0 */
L8c61f440: sa_motion_publish_stop(ctx,0x8c61f440u); r[0]=20u;goto L8c61f442; /* mov #0x14,r0 */
L8c61f442: sa_motion_publish_stop(ctx,0x8c61f442u); r[1]=r[3];goto L8c61f444; /* mov r3,r1 */
L8c61f444: sa_motion_publish_stop(ctx,0x8c61f444u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f446; /* fmov.s @(r0,r1),fr3 */
L8c61f446: sa_motion_publish_stop(ctx,0x8c61f446u); r[0]=4u;goto L8c61f448; /* mov #0x4,r0 */
L8c61f448: sa_motion_publish_stop(ctx,0x8c61f448u); r[2]=r[3];goto L8c61f44a; /* mov r3,r2 */
L8c61f44a: sa_motion_publish_stop(ctx,0x8c61f44au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f44c; /* fmul fr0,fr3 */
L8c61f44c: sa_motion_publish_stop(ctx,0x8c61f44cu); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f44e; /* fmov.s @(r0,r2),fr2 */
L8c61f44e: sa_motion_publish_stop(ctx,0x8c61f44eu); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f450; /* fmov.s @r5,fr0 */
L8c61f450: sa_motion_publish_stop(ctx,0x8c61f450u); r[0]=8u;goto L8c61f452; /* mov #0x8,r0 */
L8c61f452: sa_motion_publish_stop(ctx,0x8c61f452u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f454; /* fmac fr0,fr2,fr3 */
L8c61f454: sa_motion_publish_stop(ctx,0x8c61f454u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f456; /* fmov.s @(r0,r5),fr0 */
L8c61f456: sa_motion_publish_stop(ctx,0x8c61f456u); r[0]=36u;goto L8c61f458; /* mov #0x24,r0 */
L8c61f458: sa_motion_publish_stop(ctx,0x8c61f458u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f45a; /* fmov.s @(r0,r3),fr2 */
L8c61f45a: sa_motion_publish_stop(ctx,0x8c61f45au); r[0]=4u;goto L8c61f45c; /* mov #0x4,r0 */
L8c61f45c: sa_motion_publish_stop(ctx,0x8c61f45cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f45e; /* fmac fr0,fr2,fr3 */
L8c61f45e: sa_motion_publish_stop(ctx,0x8c61f45eu); fcopy(f,2u,6u);goto L8c61f460; /* fmov fr6,fr2 */
L8c61f460: sa_motion_publish_stop(ctx,0x8c61f460u); fcopy(f,0u,6u);goto L8c61f462; /* fmov fr6,fr0 */
L8c61f462: sa_motion_publish_stop(ctx,0x8c61f462u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f464; /* fmac fr0,fr3,fr2 */
L8c61f464: sa_motion_publish_stop(ctx,0x8c61f464u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f466; /* fmov.s fr2,@-r8 */
L8c61f466: sa_motion_publish_stop(ctx,0x8c61f466u); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f468; /* mov.l @r9,r3 */
L8c61f468: sa_motion_publish_stop(ctx,0x8c61f468u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f46a; /* fmov.s @(r0,r5),fr0 */
L8c61f46a: sa_motion_publish_stop(ctx,0x8c61f46au); r[0]=16u;goto L8c61f46c; /* mov #0x10,r0 */
L8c61f46c: sa_motion_publish_stop(ctx,0x8c61f46cu); r[1]=r[3];goto L8c61f46e; /* mov r3,r1 */
L8c61f46e: sa_motion_publish_stop(ctx,0x8c61f46eu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f470; /* fmov.s @(r0,r1),fr3 */
L8c61f470: sa_motion_publish_stop(ctx,0x8c61f470u); r[2]=r[3];goto L8c61f472; /* mov r3,r2 */
L8c61f472: sa_motion_publish_stop(ctx,0x8c61f472u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f474; /* fmul fr0,fr3 */
L8c61f474: sa_motion_publish_stop(ctx,0x8c61f474u); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f476; /* fmov.s @r5,fr0 */
L8c61f476: sa_motion_publish_stop(ctx,0x8c61f476u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f478; /* fmov.s @r2,fr2 */
L8c61f478: sa_motion_publish_stop(ctx,0x8c61f478u); r[0]=8u;goto L8c61f47a; /* mov #0x8,r0 */
L8c61f47a: sa_motion_publish_stop(ctx,0x8c61f47au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f47c; /* fmac fr0,fr2,fr3 */
L8c61f47c: sa_motion_publish_stop(ctx,0x8c61f47cu); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f47e; /* fmov.s @(r0,r5),fr0 */
L8c61f47e: sa_motion_publish_stop(ctx,0x8c61f47eu); r[0]=32u;goto L8c61f480; /* mov #0x20,r0 */
L8c61f480: sa_motion_publish_stop(ctx,0x8c61f480u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f482; /* fmov.s @(r0,r3),fr2 */
L8c61f482: sa_motion_publish_stop(ctx,0x8c61f482u); r[0]=24u;goto L8c61f484; /* mov #0x18,r0 */
L8c61f484: sa_motion_publish_stop(ctx,0x8c61f484u); r[3]=rd(m,bus,&io_status,0x8c61f608u,4);if(io_status)return io_status;goto L8c61f486; /* mov.l 0x8c61f608,r3 */
L8c61f486: sa_motion_publish_stop(ctx,0x8c61f486u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f488; /* fmac fr0,fr2,fr3 */
L8c61f488: sa_motion_publish_stop(ctx,0x8c61f488u); fcopy(f,0u,6u);goto L8c61f48a; /* fmov fr6,fr0 */
L8c61f48a: sa_motion_publish_stop(ctx,0x8c61f48au); fcopy(f,2u,6u);goto L8c61f48c; /* fmov fr6,fr2 */
L8c61f48c: sa_motion_publish_stop(ctx,0x8c61f48cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f48e; /* fmac fr0,fr3,fr2 */
L8c61f48e: sa_motion_publish_stop(ctx,0x8c61f48eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f490; /* fmov.s fr2,@-r8 */
L8c61f490: sa_motion_publish_stop(ctx,0x8c61f490u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f492; /* fmov.s @(r0,r5),fr3 */
L8c61f492: sa_motion_publish_stop(ctx,0x8c61f492u); r[0]=20u;goto L8c61f494; /* mov #0x14,r0 */
L8c61f494: sa_motion_publish_stop(ctx,0x8c61f494u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f496; /* fmov.s fr3,@-r8 */
L8c61f496: sa_motion_publish_stop(ctx,0x8c61f496u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f498; /* fmov.s @(r0,r5),fr3 */
L8c61f498: sa_motion_publish_stop(ctx,0x8c61f498u); r[0]=16u;goto L8c61f49a; /* mov #0x10,r0 */
L8c61f49a: sa_motion_publish_stop(ctx,0x8c61f49au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f49c; /* fmov.s fr3,@-r8 */
L8c61f49c: sa_motion_publish_stop(ctx,0x8c61f49cu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f49e; /* fmov.s @(r0,r5),fr3 */
L8c61f49e: sa_motion_publish_stop(ctx,0x8c61f49eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f4a0; /* fmov.s fr3,@-r8 */
L8c61f4a0: sa_motion_publish_stop(ctx,0x8c61f4a0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f4a2; /* mov.l r3,@-r8 */
L8c61f4a2: sa_motion_publish_stop(ctx,0x8c61f4a2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f4a4; /* pref @r8 */
L8c61f4a4: sa_motion_publish_stop(ctx,0x8c61f4a4u); r[0]=4u;goto L8c61f4a6; /* mov #0x4,r0 */
L8c61f4a6: sa_motion_publish_stop(ctx,0x8c61f4a6u); r[8]+=60u;goto L8c61f4a8; /* add #0x3c,r8 */
L8c61f4a8: sa_motion_publish_stop(ctx,0x8c61f4a8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f4aa; /* fmov.s fr7,@-r8 */
L8c61f4aa: sa_motion_publish_stop(ctx,0x8c61f4aau); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f4ac; /* mov.l @r9,r3 */
L8c61f4ac: sa_motion_publish_stop(ctx,0x8c61f4acu); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f4ae; /* fmov.s @(r0,r7),fr0 */
L8c61f4ae: sa_motion_publish_stop(ctx,0x8c61f4aeu); r[0]=20u;goto L8c61f4b0; /* mov #0x14,r0 */
L8c61f4b0: sa_motion_publish_stop(ctx,0x8c61f4b0u); r[1]=r[3];goto L8c61f4b2; /* mov r3,r1 */
L8c61f4b2: sa_motion_publish_stop(ctx,0x8c61f4b2u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f4b4; /* fmov.s @(r0,r1),fr3 */
L8c61f4b4: sa_motion_publish_stop(ctx,0x8c61f4b4u); r[0]=4u;goto L8c61f4b6; /* mov #0x4,r0 */
L8c61f4b6: sa_motion_publish_stop(ctx,0x8c61f4b6u); r[2]=r[3];goto L8c61f4b8; /* mov r3,r2 */
L8c61f4b8: sa_motion_publish_stop(ctx,0x8c61f4b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f4ba; /* fmul fr0,fr3 */
L8c61f4ba: sa_motion_publish_stop(ctx,0x8c61f4bau); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f4bc; /* fmov.s @(r0,r2),fr2 */
L8c61f4bc: sa_motion_publish_stop(ctx,0x8c61f4bcu); fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c61f4be; /* fmov.s @r7,fr0 */
L8c61f4be: sa_motion_publish_stop(ctx,0x8c61f4beu); r[0]=8u;goto L8c61f4c0; /* mov #0x8,r0 */
L8c61f4c0: sa_motion_publish_stop(ctx,0x8c61f4c0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f4c2; /* fmac fr0,fr2,fr3 */
L8c61f4c2: sa_motion_publish_stop(ctx,0x8c61f4c2u); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f4c4; /* fmov.s @(r0,r7),fr0 */
L8c61f4c4: sa_motion_publish_stop(ctx,0x8c61f4c4u); r[0]=36u;goto L8c61f4c6; /* mov #0x24,r0 */
L8c61f4c6: sa_motion_publish_stop(ctx,0x8c61f4c6u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f4c8; /* fmov.s @(r0,r3),fr2 */
L8c61f4c8: sa_motion_publish_stop(ctx,0x8c61f4c8u); r[0]=4u;goto L8c61f4ca; /* mov #0x4,r0 */
L8c61f4ca: sa_motion_publish_stop(ctx,0x8c61f4cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f4cc; /* fmac fr0,fr2,fr3 */
L8c61f4cc: sa_motion_publish_stop(ctx,0x8c61f4ccu); fcopy(f,2u,6u);goto L8c61f4ce; /* fmov fr6,fr2 */
L8c61f4ce: sa_motion_publish_stop(ctx,0x8c61f4ceu); fcopy(f,0u,6u);goto L8c61f4d0; /* fmov fr6,fr0 */
L8c61f4d0: sa_motion_publish_stop(ctx,0x8c61f4d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f4d2; /* fmac fr0,fr3,fr2 */
L8c61f4d2: sa_motion_publish_stop(ctx,0x8c61f4d2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f4d4; /* fmov.s fr2,@-r8 */
L8c61f4d4: sa_motion_publish_stop(ctx,0x8c61f4d4u); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f4d6; /* mov.l @r9,r3 */
L8c61f4d6: sa_motion_publish_stop(ctx,0x8c61f4d6u); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f4d8; /* fmov.s @(r0,r7),fr0 */
L8c61f4d8: sa_motion_publish_stop(ctx,0x8c61f4d8u); r[0]=16u;goto L8c61f4da; /* mov #0x10,r0 */
L8c61f4da: sa_motion_publish_stop(ctx,0x8c61f4dau); r[1]=r[3];goto L8c61f4dc; /* mov r3,r1 */
L8c61f4dc: sa_motion_publish_stop(ctx,0x8c61f4dcu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f4de; /* fmov.s @(r0,r1),fr3 */
L8c61f4de: sa_motion_publish_stop(ctx,0x8c61f4deu); r[2]=r[3];goto L8c61f4e0; /* mov r3,r2 */
L8c61f4e0: sa_motion_publish_stop(ctx,0x8c61f4e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f4e2; /* fmul fr0,fr3 */
L8c61f4e2: sa_motion_publish_stop(ctx,0x8c61f4e2u); fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c61f4e4; /* fmov.s @r7,fr0 */
L8c61f4e4: sa_motion_publish_stop(ctx,0x8c61f4e4u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f4e6; /* fmov.s @r2,fr2 */
L8c61f4e6: sa_motion_publish_stop(ctx,0x8c61f4e6u); r[0]=8u;goto L8c61f4e8; /* mov #0x8,r0 */
L8c61f4e8: sa_motion_publish_stop(ctx,0x8c61f4e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f4ea; /* fmac fr0,fr2,fr3 */
L8c61f4ea: sa_motion_publish_stop(ctx,0x8c61f4eau); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f4ec; /* fmov.s @(r0,r7),fr0 */
L8c61f4ec: sa_motion_publish_stop(ctx,0x8c61f4ecu); r[0]=32u;goto L8c61f4ee; /* mov #0x20,r0 */
L8c61f4ee: sa_motion_publish_stop(ctx,0x8c61f4eeu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f4f0; /* fmov.s @(r0,r3),fr2 */
L8c61f4f0: sa_motion_publish_stop(ctx,0x8c61f4f0u); r[0]=24u;goto L8c61f4f2; /* mov #0x18,r0 */
L8c61f4f2: sa_motion_publish_stop(ctx,0x8c61f4f2u); r[3]=rd(m,bus,&io_status,0x8c61f608u,4);if(io_status)return io_status;goto L8c61f4f4; /* mov.l 0x8c61f608,r3 */
L8c61f4f4: sa_motion_publish_stop(ctx,0x8c61f4f4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f4f6; /* fmac fr0,fr2,fr3 */
L8c61f4f6: sa_motion_publish_stop(ctx,0x8c61f4f6u); fcopy(f,0u,6u);goto L8c61f4f8; /* fmov fr6,fr0 */
L8c61f4f8: sa_motion_publish_stop(ctx,0x8c61f4f8u); fcopy(f,2u,6u);goto L8c61f4fa; /* fmov fr6,fr2 */
L8c61f4fa: sa_motion_publish_stop(ctx,0x8c61f4fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f4fc; /* fmac fr0,fr3,fr2 */
L8c61f4fc: sa_motion_publish_stop(ctx,0x8c61f4fcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f4fe; /* fmov.s fr2,@-r8 */
L8c61f4fe: sa_motion_publish_stop(ctx,0x8c61f4feu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f500; /* fmov.s @(r0,r7),fr3 */
L8c61f500: sa_motion_publish_stop(ctx,0x8c61f500u); r[0]=20u;goto L8c61f502; /* mov #0x14,r0 */
L8c61f502: sa_motion_publish_stop(ctx,0x8c61f502u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f504; /* fmov.s fr3,@-r8 */
L8c61f504: sa_motion_publish_stop(ctx,0x8c61f504u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f506; /* fmov.s @(r0,r7),fr3 */
L8c61f506: sa_motion_publish_stop(ctx,0x8c61f506u); r[0]=16u;goto L8c61f508; /* mov #0x10,r0 */
L8c61f508: sa_motion_publish_stop(ctx,0x8c61f508u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f50a; /* fmov.s fr3,@-r8 */
L8c61f50a: sa_motion_publish_stop(ctx,0x8c61f50au); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f50c; /* fmov.s @(r0,r7),fr3 */
L8c61f50c: sa_motion_publish_stop(ctx,0x8c61f50cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f50e; /* fmov.s fr3,@-r8 */
L8c61f50e: sa_motion_publish_stop(ctx,0x8c61f50eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f510; /* mov.l r3,@-r8 */
L8c61f510: sa_motion_publish_stop(ctx,0x8c61f510u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f512; /* pref @r8 */
L8c61f512: sa_motion_publish_stop(ctx,0x8c61f512u); r[0]=4u;goto L8c61f514; /* mov #0x4,r0 */
L8c61f514: sa_motion_publish_stop(ctx,0x8c61f514u); r[8]+=60u;goto L8c61f516; /* add #0x3c,r8 */
L8c61f516: sa_motion_publish_stop(ctx,0x8c61f516u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f518; /* fmov.s fr7,@-r8 */
L8c61f518: sa_motion_publish_stop(ctx,0x8c61f518u); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f51a; /* mov.l @r9,r3 */
L8c61f51a: sa_motion_publish_stop(ctx,0x8c61f51au); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61f51c; /* fmov.s @(r0,r14),fr0 */
L8c61f51c: sa_motion_publish_stop(ctx,0x8c61f51cu); r[0]=20u;goto L8c61f51e; /* mov #0x14,r0 */
L8c61f51e: sa_motion_publish_stop(ctx,0x8c61f51eu); r[1]=r[3];goto L8c61f520; /* mov r3,r1 */
L8c61f520: sa_motion_publish_stop(ctx,0x8c61f520u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f522; /* fmov.s @(r0,r1),fr3 */
L8c61f522: sa_motion_publish_stop(ctx,0x8c61f522u); r[0]=4u;goto L8c61f524; /* mov #0x4,r0 */
L8c61f524: sa_motion_publish_stop(ctx,0x8c61f524u); r[2]=r[3];goto L8c61f526; /* mov r3,r2 */
L8c61f526: sa_motion_publish_stop(ctx,0x8c61f526u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f528; /* fmul fr0,fr3 */
L8c61f528: sa_motion_publish_stop(ctx,0x8c61f528u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f52a; /* fmov.s @(r0,r2),fr2 */
L8c61f52a: sa_motion_publish_stop(ctx,0x8c61f52au); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61f52c; /* fmov.s @r14,fr0 */
L8c61f52c: sa_motion_publish_stop(ctx,0x8c61f52cu); r[0]=8u;goto L8c61f52e; /* mov #0x8,r0 */
L8c61f52e: sa_motion_publish_stop(ctx,0x8c61f52eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f530; /* fmac fr0,fr2,fr3 */
L8c61f530: sa_motion_publish_stop(ctx,0x8c61f530u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61f532; /* fmov.s @(r0,r14),fr0 */
L8c61f532: sa_motion_publish_stop(ctx,0x8c61f532u); r[0]=36u;goto L8c61f534; /* mov #0x24,r0 */
L8c61f534: sa_motion_publish_stop(ctx,0x8c61f534u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f536; /* fmov.s @(r0,r3),fr2 */
L8c61f536: sa_motion_publish_stop(ctx,0x8c61f536u); r[0]=4u;goto L8c61f538; /* mov #0x4,r0 */
L8c61f538: sa_motion_publish_stop(ctx,0x8c61f538u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f53a; /* fmac fr0,fr2,fr3 */
L8c61f53a: sa_motion_publish_stop(ctx,0x8c61f53au); fcopy(f,2u,6u);goto L8c61f53c; /* fmov fr6,fr2 */
L8c61f53c: sa_motion_publish_stop(ctx,0x8c61f53cu); fcopy(f,0u,6u);goto L8c61f53e; /* fmov fr6,fr0 */
L8c61f53e: sa_motion_publish_stop(ctx,0x8c61f53eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f540; /* fmac fr0,fr3,fr2 */
L8c61f540: sa_motion_publish_stop(ctx,0x8c61f540u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f542; /* fmov.s fr2,@-r8 */
L8c61f542: sa_motion_publish_stop(ctx,0x8c61f542u); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f544; /* mov.l @r9,r3 */
L8c61f544: sa_motion_publish_stop(ctx,0x8c61f544u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61f546; /* fmov.s @(r0,r14),fr0 */
L8c61f546: sa_motion_publish_stop(ctx,0x8c61f546u); r[0]=16u;goto L8c61f548; /* mov #0x10,r0 */
L8c61f548: sa_motion_publish_stop(ctx,0x8c61f548u); r[1]=r[3];goto L8c61f54a; /* mov r3,r1 */
L8c61f54a: sa_motion_publish_stop(ctx,0x8c61f54au); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f54c; /* fmov.s @(r0,r1),fr3 */
L8c61f54c: sa_motion_publish_stop(ctx,0x8c61f54cu); r[2]=r[3];goto L8c61f54e; /* mov r3,r2 */
L8c61f54e: sa_motion_publish_stop(ctx,0x8c61f54eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f550; /* fmul fr0,fr3 */
L8c61f550: sa_motion_publish_stop(ctx,0x8c61f550u); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61f552; /* fmov.s @r14,fr0 */
L8c61f552: sa_motion_publish_stop(ctx,0x8c61f552u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f554; /* fmov.s @r2,fr2 */
L8c61f554: sa_motion_publish_stop(ctx,0x8c61f554u); r[0]=8u;goto L8c61f556; /* mov #0x8,r0 */
L8c61f556: sa_motion_publish_stop(ctx,0x8c61f556u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f558; /* fmac fr0,fr2,fr3 */
L8c61f558: sa_motion_publish_stop(ctx,0x8c61f558u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61f55a; /* fmov.s @(r0,r14),fr0 */
L8c61f55a: sa_motion_publish_stop(ctx,0x8c61f55au); r[0]=32u;goto L8c61f55c; /* mov #0x20,r0 */
L8c61f55c: sa_motion_publish_stop(ctx,0x8c61f55cu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f55e; /* fmov.s @(r0,r3),fr2 */
L8c61f55e: sa_motion_publish_stop(ctx,0x8c61f55eu); r[0]=24u;goto L8c61f560; /* mov #0x18,r0 */
L8c61f560: sa_motion_publish_stop(ctx,0x8c61f560u); r[3]=rd(m,bus,&io_status,0x8c61f60cu,4);if(io_status)return io_status;goto L8c61f562; /* mov.l 0x8c61f60c,r3 */
L8c61f562: sa_motion_publish_stop(ctx,0x8c61f562u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f564; /* fmac fr0,fr2,fr3 */
L8c61f564: sa_motion_publish_stop(ctx,0x8c61f564u); fcopy(f,0u,6u);goto L8c61f566; /* fmov fr6,fr0 */
L8c61f566: sa_motion_publish_stop(ctx,0x8c61f566u); fcopy(f,2u,6u);goto L8c61f568; /* fmov fr6,fr2 */
L8c61f568: sa_motion_publish_stop(ctx,0x8c61f568u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f56a; /* fmac fr0,fr3,fr2 */
L8c61f56a: sa_motion_publish_stop(ctx,0x8c61f56au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f56c; /* fmov.s fr2,@-r8 */
L8c61f56c: sa_motion_publish_stop(ctx,0x8c61f56cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61f56e; /* fmov.s @(r0,r14),fr3 */
L8c61f56e: sa_motion_publish_stop(ctx,0x8c61f56eu); r[0]=20u;goto L8c61f570; /* mov #0x14,r0 */
L8c61f570: sa_motion_publish_stop(ctx,0x8c61f570u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f572; /* fmov.s fr3,@-r8 */
L8c61f572: sa_motion_publish_stop(ctx,0x8c61f572u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61f574; /* fmov.s @(r0,r14),fr3 */
L8c61f574: sa_motion_publish_stop(ctx,0x8c61f574u); r[0]=16u;goto L8c61f576; /* mov #0x10,r0 */
L8c61f576: sa_motion_publish_stop(ctx,0x8c61f576u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f578; /* fmov.s fr3,@-r8 */
L8c61f578: sa_motion_publish_stop(ctx,0x8c61f578u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61f57a; /* fmov.s @(r0,r14),fr3 */
L8c61f57a: sa_motion_publish_stop(ctx,0x8c61f57au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f57c; /* fmov.s fr3,@-r8 */
L8c61f57c: sa_motion_publish_stop(ctx,0x8c61f57cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f57e; /* mov.l r3,@-r8 */
L8c61f57e: sa_motion_publish_stop(ctx,0x8c61f57eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f580; /* pref @r8 */
L8c61f580: sa_motion_publish_stop(ctx,0x8c61f580u); r[2]=rd(m,bus,&io_status,0x8c61f610u,4);if(io_status)return io_status;goto L8c61f582; /* mov.l 0x8c61f610,r2 */
L8c61f582: sa_motion_publish_stop(ctx,0x8c61f582u); r[8]+=32u;goto L8c61f584; /* add #0x20,r8 */
L8c61f584: sa_motion_publish_stop(ctx,0x8c61f584u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61f586; /* mov.l @r2,r3 */
L8c61f586: sa_motion_publish_stop(ctx,0x8c61f586u); r[3]+=4u;goto L8c61f588; /* add #0x4,r3 */
L8c61f588: sa_motion_publish_stop(ctx,0x8c61f588u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61f58a; /* mov.l r3,@r2 */
L8c61f58a: sa_motion_publish_stop(ctx,0x8c61f58au); r[11]+=1u;goto L8c61f58c; /* add #0x1,r11 */
L8c61f58c: sa_motion_publish_stop(ctx,0x8c61f58cu); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61f58e; /* mov.w @(0x2,r4),r0 */
L8c61f58e: sa_motion_publish_stop(ctx,0x8c61f58eu); r[0]=r[0]&65535u;goto L8c61f590; /* extu.w r0,r0 */
L8c61f590: sa_motion_publish_stop(ctx,0x8c61f590u); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61f592; /* cmp/ge r0,r11 */
L8c61f592: sa_motion_publish_stop(ctx,0x8c61f592u); if((*sr&1u))goto L8c61f598;goto L8c61f594; /* bt 0x8c61f598 */
L8c61f594: sa_motion_publish_stop(ctx,0x8c61f596u); goto L8c61f364; /* bra 0x8c61f364 */
L8c61f598: sa_motion_publish_stop(ctx,0x8c61f59au); goto L8c61f79c; /* bra 0x8c61f79c */
L8c61f59c: sa_motion_publish_stop(ctx,0x8c61f59eu); r[10]=r[12];goto L8c61f72e; /* bra 0x8c61f72e */
L8c61f5a0: sa_motion_publish_stop(ctx,0x8c61f5a0u); r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f5a2; /* mov.w @r13+,r14 */
L8c61f5a2: sa_motion_publish_stop(ctx,0x8c61f5a2u); r[3]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c61f5a4; /* mov.l @r11,r3 */
L8c61f5a4: sa_motion_publish_stop(ctx,0x8c61f5a4u); r[14]=r[14]&65535u;goto L8c61f5a6; /* extu.w r14,r14 */
L8c61f5a6: sa_motion_publish_stop(ctx,0x8c61f5a6u); r[2]=rd(m,bus,&io_status,0x8c61f604u,4);if(io_status)return io_status;goto L8c61f5a8; /* mov.l 0x8c61f604,r2 */
L8c61f5a8: sa_motion_publish_stop(ctx,0x8c61f5a8u); r[3]+=r[14];goto L8c61f5aa; /* add r14,r3 */
L8c61f5aa: sa_motion_publish_stop(ctx,0x8c61f5aau); wr(m,bus,&io_status,r[11],r[3],4);if(io_status)return io_status;goto L8c61f5ac; /* mov.l r3,@r11 */
L8c61f5ac: sa_motion_publish_stop(ctx,0x8c61f5acu); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61f5ae; /* mov.w @(0x2,r13),r0 */
L8c61f5ae: sa_motion_publish_stop(ctx,0x8c61f5aeu); r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61f5b0; /* mov.w @r13,r7 */
L8c61f5b0: sa_motion_publish_stop(ctx,0x8c61f5b0u); r[5]=r[0]&65535u;goto L8c61f5b2; /* extu.w r0,r5 */
L8c61f5b2: sa_motion_publish_stop(ctx,0x8c61f5b2u); r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c61f5b4; /* mov.w @(0x4,r13),r0 */
L8c61f5b4: sa_motion_publish_stop(ctx,0x8c61f5b4u); r[5]<<=2;goto L8c61f5b6; /* shll2 r5 */
L8c61f5b6: sa_motion_publish_stop(ctx,0x8c61f5b6u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61f5b8; /* mov.l @r2,r3 */
L8c61f5b8: sa_motion_publish_stop(ctx,0x8c61f5b8u); r[7]=r[7]&65535u;goto L8c61f5ba; /* extu.w r7,r7 */
L8c61f5ba: sa_motion_publish_stop(ctx,0x8c61f5bau); r[7]<<=2;goto L8c61f5bc; /* shll2 r7 */
L8c61f5bc: sa_motion_publish_stop(ctx,0x8c61f5bcu); r[5]<<=2;goto L8c61f5be; /* shll2 r5 */
L8c61f5be: sa_motion_publish_stop(ctx,0x8c61f5beu); r[6]=r[0]&65535u;goto L8c61f5c0; /* extu.w r0,r6 */
L8c61f5c0: sa_motion_publish_stop(ctx,0x8c61f5c0u); r[7]<<=2;goto L8c61f5c2; /* shll2 r7 */
L8c61f5c2: sa_motion_publish_stop(ctx,0x8c61f5c2u); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61f5c4; /* shll r7 */
L8c61f5c4: sa_motion_publish_stop(ctx,0x8c61f5c4u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61f5c6; /* shll r5 */
L8c61f5c6: sa_motion_publish_stop(ctx,0x8c61f5c6u); r[0]=16u;goto L8c61f5c8; /* mov #0x10,r0 */
L8c61f5c8: sa_motion_publish_stop(ctx,0x8c61f5c8u); r[5]+=r[3];goto L8c61f5ca; /* add r3,r5 */
L8c61f5ca: sa_motion_publish_stop(ctx,0x8c61f5cau); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f5cc; /* fmov.s @(r0,r5),fr5 */
L8c61f5cc: sa_motion_publish_stop(ctx,0x8c61f5ccu); r[7]+=r[3];goto L8c61f5ce; /* add r3,r7 */
L8c61f5ce: sa_motion_publish_stop(ctx,0x8c61f5ceu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f5d0; /* fmov.s @(r0,r7),fr3 */
L8c61f5d0: sa_motion_publish_stop(ctx,0x8c61f5d0u); r[6]<<=2;goto L8c61f5d2; /* shll2 r6 */
L8c61f5d2: sa_motion_publish_stop(ctx,0x8c61f5d2u); fcopy(f,2u,5u);goto L8c61f5d4; /* fmov fr5,fr2 */
L8c61f5d4: sa_motion_publish_stop(ctx,0x8c61f5d4u); r[6]<<=2;goto L8c61f5d6; /* shll2 r6 */
L8c61f5d6: sa_motion_publish_stop(ctx,0x8c61f5d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61f5d8; /* fsub fr3,fr2 */
L8c61f5d8: sa_motion_publish_stop(ctx,0x8c61f5d8u); r[0]=20u;goto L8c61f5da; /* mov #0x14,r0 */
L8c61f5da: sa_motion_publish_stop(ctx,0x8c61f5dau); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61f5dc; /* shll r6 */
L8c61f5dc: sa_motion_publish_stop(ctx,0x8c61f5dcu); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f5de; /* fmov.s @(r0,r5),fr4 */
L8c61f5de: sa_motion_publish_stop(ctx,0x8c61f5deu); r[6]+=r[3];goto L8c61f5e0; /* add r3,r6 */
L8c61f5e0: sa_motion_publish_stop(ctx,0x8c61f5e0u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f5e2; /* fmov.s @(r0,r6),fr3 */
L8c61f5e2: sa_motion_publish_stop(ctx,0x8c61f5e2u); fcopy(f,1u,4u);goto L8c61f5e4; /* fmov fr4,fr1 */
L8c61f5e4: sa_motion_publish_stop(ctx,0x8c61f5e4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f5e6; /* fsub fr4,fr3 */
L8c61f5e6: sa_motion_publish_stop(ctx,0x8c61f5e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f5e8; /* fmul fr3,fr2 */
L8c61f5e8: sa_motion_publish_stop(ctx,0x8c61f5e8u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f5ea; /* fmov.s @(r0,r7),fr3 */
L8c61f5ea: sa_motion_publish_stop(ctx,0x8c61f5eau); r[0]=16u;goto L8c61f5ec; /* mov #0x10,r0 */
L8c61f5ec: sa_motion_publish_stop(ctx,0x8c61f5ecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61f5ee; /* fsub fr3,fr1 */
L8c61f5ee: sa_motion_publish_stop(ctx,0x8c61f5eeu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f5f0; /* fmov.s @(r0,r6),fr3 */
L8c61f5f0: sa_motion_publish_stop(ctx,0x8c61f5f0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f5f2; /* fsub fr5,fr3 */
L8c61f5f2: sa_motion_publish_stop(ctx,0x8c61f5f2u); fcopy(f,4u,2u);goto L8c61f5f4; /* fmov fr2,fr4 */
L8c61f5f4: sa_motion_publish_stop(ctx,0x8c61f5f4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61f5f6; /* fmul fr3,fr1 */
L8c61f5f6: sa_motion_publish_stop(ctx,0x8c61f5f6u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61f5f8; /* fldi0 fr3 */
L8c61f5f8: sa_motion_publish_stop(ctx,0x8c61f5f8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61f5fa; /* fsub fr1,fr4 */
L8c61f5fa: sa_motion_publish_stop(ctx,0x8c61f5fau); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61f5fc; /* fcmp/gt fr3,fr4 */
L8c61f5fc: sa_motion_publish_stop(ctx,0x8c61f5fcu); if(!(*sr&1u))goto L8c61f614;goto L8c61f5fe; /* bf 0x8c61f614 */
L8c61f5fe: sa_motion_publish_stop(ctx,0x8c61f5feu); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61f600; /* shll r14 */
L8c61f600: sa_motion_publish_stop(ctx,0x8c61f602u); r[13]+=r[14];goto L8c61f72c; /* bra 0x8c61f72c */
L8c61f614: sa_motion_publish_stop(ctx,0x8c61f614u); r[3]=rd(m,bus,&io_status,0x8c61f7b0u,4);if(io_status)return io_status;goto L8c61f616; /* mov.l 0x8c61f7b0,r3 */
L8c61f616: sa_motion_publish_stop(ctx,0x8c61f616u); r[7]=r[14];goto L8c61f618; /* mov r14,r7 */
L8c61f618: sa_motion_publish_stop(ctx,0x8c61f618u); r[7]+=4294967295u;goto L8c61f61a; /* add #-0x1,r7 */
L8c61f61a: sa_motion_publish_stop(ctx,0x8c61f61au); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f61c; /* mov.l @r3,r2 */
L8c61f61c: sa_motion_publish_stop(ctx,0x8c61f61cu); *sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c61f61e; /* cmp/pl r7 */
L8c61f61e: sa_motion_publish_stop(ctx,0x8c61f61eu); r[2]+=r[14];goto L8c61f620; /* add r14,r2 */
L8c61f620: sa_motion_publish_stop(ctx,0x8c61f620u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61f622; /* mov.l r2,@r3 */
L8c61f622: sa_motion_publish_stop(ctx,0x8c61f624u); take=!(*sr&1u);r[6]=r[12];if(take)goto L8c61f6ac;goto L8c61f626; /* bf/s 0x8c61f6ac */
L8c61f626: sa_motion_publish_stop(ctx,0x8c61f626u); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f628; /* mov.w @r13+,r5 */
L8c61f628: sa_motion_publish_stop(ctx,0x8c61f628u); r[0]=4u;goto L8c61f62a; /* mov #0x4,r0 */
L8c61f62a: sa_motion_publish_stop(ctx,0x8c61f62au); r[2]=rd(m,bus,&io_status,0x8c61f7b4u,4);if(io_status)return io_status;goto L8c61f62c; /* mov.l 0x8c61f7b4,r2 */
L8c61f62c: sa_motion_publish_stop(ctx,0x8c61f62cu); r[8]+=28u;goto L8c61f62e; /* add #0x1c,r8 */
L8c61f62e: sa_motion_publish_stop(ctx,0x8c61f62eu); r[5]=r[5]&65535u;goto L8c61f630; /* extu.w r5,r5 */
L8c61f630: sa_motion_publish_stop(ctx,0x8c61f630u); r[5]<<=2;goto L8c61f632; /* shll2 r5 */
L8c61f632: sa_motion_publish_stop(ctx,0x8c61f632u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61f634; /* mov.l @r2,r3 */
L8c61f634: sa_motion_publish_stop(ctx,0x8c61f634u); r[5]<<=2;goto L8c61f636; /* shll2 r5 */
L8c61f636: sa_motion_publish_stop(ctx,0x8c61f636u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f638; /* fmov.s fr7,@-r8 */
L8c61f638: sa_motion_publish_stop(ctx,0x8c61f638u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61f63a; /* shll r5 */
L8c61f63a: sa_motion_publish_stop(ctx,0x8c61f63au); r[5]+=r[3];goto L8c61f63c; /* add r3,r5 */
L8c61f63c: sa_motion_publish_stop(ctx,0x8c61f63cu); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f63e; /* mov.l @r9,r3 */
L8c61f63e: sa_motion_publish_stop(ctx,0x8c61f63eu); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f640; /* fmov.s @(r0,r5),fr0 */
L8c61f640: sa_motion_publish_stop(ctx,0x8c61f640u); r[0]=20u;goto L8c61f642; /* mov #0x14,r0 */
L8c61f642: sa_motion_publish_stop(ctx,0x8c61f642u); r[2]=r[3];goto L8c61f644; /* mov r3,r2 */
L8c61f644: sa_motion_publish_stop(ctx,0x8c61f644u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f646; /* fmov.s @(r0,r2),fr3 */
L8c61f646: sa_motion_publish_stop(ctx,0x8c61f646u); r[0]=4u;goto L8c61f648; /* mov #0x4,r0 */
L8c61f648: sa_motion_publish_stop(ctx,0x8c61f648u); r[1]=r[3];goto L8c61f64a; /* mov r3,r1 */
L8c61f64a: sa_motion_publish_stop(ctx,0x8c61f64au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f64c; /* fmul fr0,fr3 */
L8c61f64c: sa_motion_publish_stop(ctx,0x8c61f64cu); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f64e; /* fmov.s @r5,fr0 */
L8c61f64e: sa_motion_publish_stop(ctx,0x8c61f64eu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f650; /* fmov.s @(r0,r1),fr2 */
L8c61f650: sa_motion_publish_stop(ctx,0x8c61f650u); r[0]=8u;goto L8c61f652; /* mov #0x8,r0 */
L8c61f652: sa_motion_publish_stop(ctx,0x8c61f652u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f654; /* fmac fr0,fr2,fr3 */
L8c61f654: sa_motion_publish_stop(ctx,0x8c61f654u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f656; /* fmov.s @(r0,r5),fr0 */
L8c61f656: sa_motion_publish_stop(ctx,0x8c61f656u); r[0]=36u;goto L8c61f658; /* mov #0x24,r0 */
L8c61f658: sa_motion_publish_stop(ctx,0x8c61f658u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f65a; /* fmov.s @(r0,r3),fr2 */
L8c61f65a: sa_motion_publish_stop(ctx,0x8c61f65au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f65c; /* fmac fr0,fr2,fr3 */
L8c61f65c: sa_motion_publish_stop(ctx,0x8c61f65cu); fcopy(f,2u,6u);goto L8c61f65e; /* fmov fr6,fr2 */
L8c61f65e: sa_motion_publish_stop(ctx,0x8c61f65eu); fcopy(f,0u,6u);goto L8c61f660; /* fmov fr6,fr0 */
L8c61f660: sa_motion_publish_stop(ctx,0x8c61f660u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f662; /* fmac fr0,fr3,fr2 */
L8c61f662: sa_motion_publish_stop(ctx,0x8c61f662u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f664; /* fmov.s fr2,@-r8 */
L8c61f664: sa_motion_publish_stop(ctx,0x8c61f664u); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f666; /* mov.l @r9,r3 */
L8c61f666: sa_motion_publish_stop(ctx,0x8c61f666u); r[0]=4u;goto L8c61f668; /* mov #0x4,r0 */
L8c61f668: sa_motion_publish_stop(ctx,0x8c61f668u); r[1]=r[3];goto L8c61f66a; /* mov r3,r1 */
L8c61f66a: sa_motion_publish_stop(ctx,0x8c61f66au); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f66c; /* fmov.s @(r0,r5),fr0 */
L8c61f66c: sa_motion_publish_stop(ctx,0x8c61f66cu); r[0]=16u;goto L8c61f66e; /* mov #0x10,r0 */
L8c61f66e: sa_motion_publish_stop(ctx,0x8c61f66eu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f670; /* fmov.s @(r0,r1),fr3 */
L8c61f670: sa_motion_publish_stop(ctx,0x8c61f670u); r[2]=r[3];goto L8c61f672; /* mov r3,r2 */
L8c61f672: sa_motion_publish_stop(ctx,0x8c61f672u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f674; /* fmov.s @r2,fr2 */
L8c61f674: sa_motion_publish_stop(ctx,0x8c61f674u); r[0]=8u;goto L8c61f676; /* mov #0x8,r0 */
L8c61f676: sa_motion_publish_stop(ctx,0x8c61f676u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f678; /* fmul fr0,fr3 */
L8c61f678: sa_motion_publish_stop(ctx,0x8c61f678u); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f67a; /* fmov.s @r5,fr0 */
L8c61f67a: sa_motion_publish_stop(ctx,0x8c61f67au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f67c; /* fmac fr0,fr2,fr3 */
L8c61f67c: sa_motion_publish_stop(ctx,0x8c61f67cu); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f67e; /* fmov.s @(r0,r5),fr0 */
L8c61f67e: sa_motion_publish_stop(ctx,0x8c61f67eu); r[0]=32u;goto L8c61f680; /* mov #0x20,r0 */
L8c61f680: sa_motion_publish_stop(ctx,0x8c61f680u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f682; /* fmov.s @(r0,r3),fr2 */
L8c61f682: sa_motion_publish_stop(ctx,0x8c61f682u); r[0]=24u;goto L8c61f684; /* mov #0x18,r0 */
L8c61f684: sa_motion_publish_stop(ctx,0x8c61f684u); r[3]=rd(m,bus,&io_status,0x8c61f7b8u,4);if(io_status)return io_status;goto L8c61f686; /* mov.l 0x8c61f7b8,r3 */
L8c61f686: sa_motion_publish_stop(ctx,0x8c61f686u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f688; /* fmac fr0,fr2,fr3 */
L8c61f688: sa_motion_publish_stop(ctx,0x8c61f688u); fcopy(f,0u,6u);goto L8c61f68a; /* fmov fr6,fr0 */
L8c61f68a: sa_motion_publish_stop(ctx,0x8c61f68au); fcopy(f,2u,6u);goto L8c61f68c; /* fmov fr6,fr2 */
L8c61f68c: sa_motion_publish_stop(ctx,0x8c61f68cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f68e; /* fmac fr0,fr3,fr2 */
L8c61f68e: sa_motion_publish_stop(ctx,0x8c61f68eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f690; /* fmov.s fr2,@-r8 */
L8c61f690: sa_motion_publish_stop(ctx,0x8c61f690u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f692; /* fmov.s @(r0,r5),fr3 */
L8c61f692: sa_motion_publish_stop(ctx,0x8c61f692u); r[0]=20u;goto L8c61f694; /* mov #0x14,r0 */
L8c61f694: sa_motion_publish_stop(ctx,0x8c61f694u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f696; /* fmov.s fr3,@-r8 */
L8c61f696: sa_motion_publish_stop(ctx,0x8c61f696u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f698; /* fmov.s @(r0,r5),fr3 */
L8c61f698: sa_motion_publish_stop(ctx,0x8c61f698u); r[0]=16u;goto L8c61f69a; /* mov #0x10,r0 */
L8c61f69a: sa_motion_publish_stop(ctx,0x8c61f69au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f69c; /* fmov.s fr3,@-r8 */
L8c61f69c: sa_motion_publish_stop(ctx,0x8c61f69cu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f69e; /* fmov.s @(r0,r5),fr3 */
L8c61f69e: sa_motion_publish_stop(ctx,0x8c61f69eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f6a0; /* fmov.s fr3,@-r8 */
L8c61f6a0: sa_motion_publish_stop(ctx,0x8c61f6a0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f6a2; /* mov.l r3,@-r8 */
L8c61f6a2: sa_motion_publish_stop(ctx,0x8c61f6a2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f6a4; /* pref @r8 */
L8c61f6a4: sa_motion_publish_stop(ctx,0x8c61f6a4u); r[6]+=1u;goto L8c61f6a6; /* add #0x1,r6 */
L8c61f6a6: sa_motion_publish_stop(ctx,0x8c61f6a6u); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c61f6a8; /* cmp/ge r7,r6 */
L8c61f6a8: sa_motion_publish_stop(ctx,0x8c61f6aau); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61f626;goto L8c61f6ac; /* bf/s 0x8c61f626 */
L8c61f6ac: sa_motion_publish_stop(ctx,0x8c61f6acu); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f6ae; /* mov.w @r13+,r5 */
L8c61f6ae: sa_motion_publish_stop(ctx,0x8c61f6aeu); r[0]=4u;goto L8c61f6b0; /* mov #0x4,r0 */
L8c61f6b0: sa_motion_publish_stop(ctx,0x8c61f6b0u); r[2]=rd(m,bus,&io_status,0x8c61f7b4u,4);if(io_status)return io_status;goto L8c61f6b2; /* mov.l 0x8c61f7b4,r2 */
L8c61f6b2: sa_motion_publish_stop(ctx,0x8c61f6b2u); r[8]+=28u;goto L8c61f6b4; /* add #0x1c,r8 */
L8c61f6b4: sa_motion_publish_stop(ctx,0x8c61f6b4u); r[5]=r[5]&65535u;goto L8c61f6b6; /* extu.w r5,r5 */
L8c61f6b6: sa_motion_publish_stop(ctx,0x8c61f6b6u); r[5]<<=2;goto L8c61f6b8; /* shll2 r5 */
L8c61f6b8: sa_motion_publish_stop(ctx,0x8c61f6b8u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61f6ba; /* mov.l @r2,r3 */
L8c61f6ba: sa_motion_publish_stop(ctx,0x8c61f6bau); r[5]<<=2;goto L8c61f6bc; /* shll2 r5 */
L8c61f6bc: sa_motion_publish_stop(ctx,0x8c61f6bcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f6be; /* fmov.s fr7,@-r8 */
L8c61f6be: sa_motion_publish_stop(ctx,0x8c61f6beu); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61f6c0; /* shll r5 */
L8c61f6c0: sa_motion_publish_stop(ctx,0x8c61f6c0u); r[5]+=r[3];goto L8c61f6c2; /* add r3,r5 */
L8c61f6c2: sa_motion_publish_stop(ctx,0x8c61f6c2u); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f6c4; /* mov.l @r9,r3 */
L8c61f6c4: sa_motion_publish_stop(ctx,0x8c61f6c4u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f6c6; /* fmov.s @(r0,r5),fr0 */
L8c61f6c6: sa_motion_publish_stop(ctx,0x8c61f6c6u); r[0]=20u;goto L8c61f6c8; /* mov #0x14,r0 */
L8c61f6c8: sa_motion_publish_stop(ctx,0x8c61f6c8u); r[2]=r[3];goto L8c61f6ca; /* mov r3,r2 */
L8c61f6ca: sa_motion_publish_stop(ctx,0x8c61f6cau); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f6cc; /* fmov.s @(r0,r2),fr3 */
L8c61f6cc: sa_motion_publish_stop(ctx,0x8c61f6ccu); r[0]=4u;goto L8c61f6ce; /* mov #0x4,r0 */
L8c61f6ce: sa_motion_publish_stop(ctx,0x8c61f6ceu); r[1]=r[3];goto L8c61f6d0; /* mov r3,r1 */
L8c61f6d0: sa_motion_publish_stop(ctx,0x8c61f6d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f6d2; /* fmul fr0,fr3 */
L8c61f6d2: sa_motion_publish_stop(ctx,0x8c61f6d2u); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f6d4; /* fmov.s @r5,fr0 */
L8c61f6d4: sa_motion_publish_stop(ctx,0x8c61f6d4u); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f6d6; /* fmov.s @(r0,r1),fr2 */
L8c61f6d6: sa_motion_publish_stop(ctx,0x8c61f6d6u); r[0]=8u;goto L8c61f6d8; /* mov #0x8,r0 */
L8c61f6d8: sa_motion_publish_stop(ctx,0x8c61f6d8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f6da; /* fmac fr0,fr2,fr3 */
L8c61f6da: sa_motion_publish_stop(ctx,0x8c61f6dau); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f6dc; /* fmov.s @(r0,r5),fr0 */
L8c61f6dc: sa_motion_publish_stop(ctx,0x8c61f6dcu); r[0]=36u;goto L8c61f6de; /* mov #0x24,r0 */
L8c61f6de: sa_motion_publish_stop(ctx,0x8c61f6deu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f6e0; /* fmov.s @(r0,r3),fr2 */
L8c61f6e0: sa_motion_publish_stop(ctx,0x8c61f6e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f6e2; /* fmac fr0,fr2,fr3 */
L8c61f6e2: sa_motion_publish_stop(ctx,0x8c61f6e2u); fcopy(f,2u,6u);goto L8c61f6e4; /* fmov fr6,fr2 */
L8c61f6e4: sa_motion_publish_stop(ctx,0x8c61f6e4u); fcopy(f,0u,6u);goto L8c61f6e6; /* fmov fr6,fr0 */
L8c61f6e6: sa_motion_publish_stop(ctx,0x8c61f6e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f6e8; /* fmac fr0,fr3,fr2 */
L8c61f6e8: sa_motion_publish_stop(ctx,0x8c61f6e8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f6ea; /* fmov.s fr2,@-r8 */
L8c61f6ea: sa_motion_publish_stop(ctx,0x8c61f6eau); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f6ec; /* mov.l @r9,r3 */
L8c61f6ec: sa_motion_publish_stop(ctx,0x8c61f6ecu); r[0]=4u;goto L8c61f6ee; /* mov #0x4,r0 */
L8c61f6ee: sa_motion_publish_stop(ctx,0x8c61f6eeu); r[1]=r[3];goto L8c61f6f0; /* mov r3,r1 */
L8c61f6f0: sa_motion_publish_stop(ctx,0x8c61f6f0u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f6f2; /* fmov.s @(r0,r5),fr0 */
L8c61f6f2: sa_motion_publish_stop(ctx,0x8c61f6f2u); r[0]=16u;goto L8c61f6f4; /* mov #0x10,r0 */
L8c61f6f4: sa_motion_publish_stop(ctx,0x8c61f6f4u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f6f6; /* fmov.s @(r0,r1),fr3 */
L8c61f6f6: sa_motion_publish_stop(ctx,0x8c61f6f6u); r[2]=r[3];goto L8c61f6f8; /* mov r3,r2 */
L8c61f6f8: sa_motion_publish_stop(ctx,0x8c61f6f8u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f6fa; /* fmov.s @r2,fr2 */
L8c61f6fa: sa_motion_publish_stop(ctx,0x8c61f6fau); r[0]=8u;goto L8c61f6fc; /* mov #0x8,r0 */
L8c61f6fc: sa_motion_publish_stop(ctx,0x8c61f6fcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f6fe; /* fmul fr0,fr3 */
L8c61f6fe: sa_motion_publish_stop(ctx,0x8c61f6feu); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f700; /* fmov.s @r5,fr0 */
L8c61f700: sa_motion_publish_stop(ctx,0x8c61f700u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f702; /* fmac fr0,fr2,fr3 */
L8c61f702: sa_motion_publish_stop(ctx,0x8c61f702u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f704; /* fmov.s @(r0,r5),fr0 */
L8c61f704: sa_motion_publish_stop(ctx,0x8c61f704u); r[0]=32u;goto L8c61f706; /* mov #0x20,r0 */
L8c61f706: sa_motion_publish_stop(ctx,0x8c61f706u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f708; /* fmov.s @(r0,r3),fr2 */
L8c61f708: sa_motion_publish_stop(ctx,0x8c61f708u); r[0]=24u;goto L8c61f70a; /* mov #0x18,r0 */
L8c61f70a: sa_motion_publish_stop(ctx,0x8c61f70au); r[3]=rd(m,bus,&io_status,0x8c61f7bcu,4);if(io_status)return io_status;goto L8c61f70c; /* mov.l 0x8c61f7bc,r3 */
L8c61f70c: sa_motion_publish_stop(ctx,0x8c61f70cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f70e; /* fmac fr0,fr2,fr3 */
L8c61f70e: sa_motion_publish_stop(ctx,0x8c61f70eu); fcopy(f,0u,6u);goto L8c61f710; /* fmov fr6,fr0 */
L8c61f710: sa_motion_publish_stop(ctx,0x8c61f710u); fcopy(f,2u,6u);goto L8c61f712; /* fmov fr6,fr2 */
L8c61f712: sa_motion_publish_stop(ctx,0x8c61f712u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f714; /* fmac fr0,fr3,fr2 */
L8c61f714: sa_motion_publish_stop(ctx,0x8c61f714u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f716; /* fmov.s fr2,@-r8 */
L8c61f716: sa_motion_publish_stop(ctx,0x8c61f716u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f718; /* fmov.s @(r0,r5),fr3 */
L8c61f718: sa_motion_publish_stop(ctx,0x8c61f718u); r[0]=20u;goto L8c61f71a; /* mov #0x14,r0 */
L8c61f71a: sa_motion_publish_stop(ctx,0x8c61f71au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f71c; /* fmov.s fr3,@-r8 */
L8c61f71c: sa_motion_publish_stop(ctx,0x8c61f71cu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f71e; /* fmov.s @(r0,r5),fr3 */
L8c61f71e: sa_motion_publish_stop(ctx,0x8c61f71eu); r[0]=16u;goto L8c61f720; /* mov #0x10,r0 */
L8c61f720: sa_motion_publish_stop(ctx,0x8c61f720u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f722; /* fmov.s fr3,@-r8 */
L8c61f722: sa_motion_publish_stop(ctx,0x8c61f722u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f724; /* fmov.s @(r0,r5),fr3 */
L8c61f724: sa_motion_publish_stop(ctx,0x8c61f724u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f726; /* fmov.s fr3,@-r8 */
L8c61f726: sa_motion_publish_stop(ctx,0x8c61f726u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f728; /* mov.l r3,@-r8 */
L8c61f728: sa_motion_publish_stop(ctx,0x8c61f728u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f72a; /* pref @r8 */
L8c61f72a: sa_motion_publish_stop(ctx,0x8c61f72au); r[8]+=32u;goto L8c61f72c; /* add #0x20,r8 */
L8c61f72c: sa_motion_publish_stop(ctx,0x8c61f72cu); r[10]+=1u;goto L8c61f72e; /* add #0x1,r10 */
L8c61f72e: sa_motion_publish_stop(ctx,0x8c61f72eu); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61f730; /* mov.w @(0x2,r4),r0 */
L8c61f730: sa_motion_publish_stop(ctx,0x8c61f730u); r[0]=r[0]&65535u;goto L8c61f732; /* extu.w r0,r0 */
L8c61f732: sa_motion_publish_stop(ctx,0x8c61f732u); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c61f734; /* cmp/ge r0,r10 */
L8c61f734: sa_motion_publish_stop(ctx,0x8c61f734u); if((*sr&1u))goto L8c61f73a;goto L8c61f736; /* bt 0x8c61f73a */
L8c61f736: sa_motion_publish_stop(ctx,0x8c61f738u); goto L8c61f5a0; /* bra 0x8c61f5a0 */
L8c61f73a: sa_motion_publish_stop(ctx,0x8c61f73cu); goto L8c61f79c; /* bra 0x8c61f79c */
L8c61f73e: sa_motion_publish_stop(ctx,0x8c61f73eu); r[1]=rd(m,bus,&io_status,0x8c61f7c0u,4);if(io_status)return io_status;goto L8c61f740; /* mov.l 0x8c61f7c0,r1 */
L8c61f740: sa_motion_publish_stop(ctx,0x8c61f740u); wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c61f742; /* mov.l r4,@r1 */
L8c61f742: sa_motion_publish_stop(ctx,0x8c61f742u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f744; /* frchg */
L8c61f744: sa_motion_publish_stop(ctx,0x8c61f744u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61f746; /* fmov.s fr0,@-r15 */
L8c61f746: sa_motion_publish_stop(ctx,0x8c61f746u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61f748; /* fmov.s fr1,@-r15 */
L8c61f748: sa_motion_publish_stop(ctx,0x8c61f748u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61f74a; /* fmov.s fr2,@-r15 */
L8c61f74a: sa_motion_publish_stop(ctx,0x8c61f74au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61f74c; /* fmov.s fr3,@-r15 */
L8c61f74c: sa_motion_publish_stop(ctx,0x8c61f74cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61f74e; /* fmov.s fr4,@-r15 */
L8c61f74e: sa_motion_publish_stop(ctx,0x8c61f74eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61f750; /* fmov.s fr5,@-r15 */
L8c61f750: sa_motion_publish_stop(ctx,0x8c61f750u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61f752; /* fmov.s fr6,@-r15 */
L8c61f752: sa_motion_publish_stop(ctx,0x8c61f752u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61f754; /* fmov.s fr7,@-r15 */
L8c61f754: sa_motion_publish_stop(ctx,0x8c61f754u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61f756; /* fmov.s fr8,@-r15 */
L8c61f756: sa_motion_publish_stop(ctx,0x8c61f756u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61f758; /* fmov.s fr9,@-r15 */
L8c61f758: sa_motion_publish_stop(ctx,0x8c61f758u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c61f75a; /* fmov.s fr10,@-r15 */
L8c61f75a: sa_motion_publish_stop(ctx,0x8c61f75au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c61f75c; /* fmov.s fr11,@-r15 */
L8c61f75c: sa_motion_publish_stop(ctx,0x8c61f75cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c61f75e; /* fmov.s fr12,@-r15 */
L8c61f75e: sa_motion_publish_stop(ctx,0x8c61f75eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61f760; /* fmov.s fr13,@-r15 */
L8c61f760: sa_motion_publish_stop(ctx,0x8c61f760u); r[0]=0x8c61f7acu;goto L8c61f762; /* mova 0x8c61f7ac,r0 */
L8c61f762: sa_motion_publish_stop(ctx,0x8c61f762u); fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c61f764; /* fmov.s @r0,fr12 */
L8c61f764: sa_motion_publish_stop(ctx,0x8c61f764u); fcopy(f,13u,12u);goto L8c61f766; /* fmov fr12,fr13 */
L8c61f766: sa_motion_publish_stop(ctx,0x8c61f766u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[12],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61f768; /* fmul fr12,fr0 */
L8c61f768: sa_motion_publish_stop(ctx,0x8c61f768u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61f76a; /* fmul fr12,fr4 */
L8c61f76a: sa_motion_publish_stop(ctx,0x8c61f76au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[12],&f->fpscr,&fr[8]);if(status)return status;}goto L8c61f76c; /* fmul fr12,fr8 */
L8c61f76c: sa_motion_publish_stop(ctx,0x8c61f76cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[12],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61f76e; /* fmul fr12,fr1 */
L8c61f76e: sa_motion_publish_stop(ctx,0x8c61f76eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[12],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61f770; /* fmul fr12,fr5 */
L8c61f770: sa_motion_publish_stop(ctx,0x8c61f770u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[12],&f->fpscr,&fr[9]);if(status)return status;}goto L8c61f772; /* fmul fr12,fr9 */
L8c61f772: sa_motion_publish_stop(ctx,0x8c61f772u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f774; /* frchg */
L8c61f774: sa_motion_publish_stop(ctx,0x8c61f774u); r[2]=rd(m,bus,&io_status,0x8c61f7c4u,4);if(io_status)return io_status;goto L8c61f776; /* mov.l 0x8c61f7c4,r2 */
L8c61f776: sa_motion_publish_stop(ctx,0x8c61f776u); r[3]=rd(m,bus,&io_status,0x8c61f7c0u,4);if(io_status)return io_status;goto L8c61f778; /* mov.l 0x8c61f7c0,r3 */
L8c61f778: sa_motion_publish_stop(ctx,0x8c61f77au); target=r[2];*pr=0x8c61f77cu;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61f77c; /* jsr @r2 */
L8c61f77c: sa_motion_publish_stop(ctx,0x8c61f77cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f77e; /* frchg */
L8c61f77e: sa_motion_publish_stop(ctx,0x8c61f77eu); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f780; /* fmov.s @r15+,fr13 */
L8c61f780: sa_motion_publish_stop(ctx,0x8c61f780u); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f782; /* fmov.s @r15+,fr12 */
L8c61f782: sa_motion_publish_stop(ctx,0x8c61f782u); fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f784; /* fmov.s @r15+,fr11 */
L8c61f784: sa_motion_publish_stop(ctx,0x8c61f784u); fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f786; /* fmov.s @r15+,fr10 */
L8c61f786: sa_motion_publish_stop(ctx,0x8c61f786u); fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f788; /* fmov.s @r15+,fr9 */
L8c61f788: sa_motion_publish_stop(ctx,0x8c61f788u); fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f78a; /* fmov.s @r15+,fr8 */
L8c61f78a: sa_motion_publish_stop(ctx,0x8c61f78au); fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f78c; /* fmov.s @r15+,fr7 */
L8c61f78c: sa_motion_publish_stop(ctx,0x8c61f78cu); fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f78e; /* fmov.s @r15+,fr6 */
L8c61f78e: sa_motion_publish_stop(ctx,0x8c61f78eu); fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f790; /* fmov.s @r15+,fr5 */
L8c61f790: sa_motion_publish_stop(ctx,0x8c61f790u); fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f792; /* fmov.s @r15+,fr4 */
L8c61f792: sa_motion_publish_stop(ctx,0x8c61f792u); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f794; /* fmov.s @r15+,fr3 */
L8c61f794: sa_motion_publish_stop(ctx,0x8c61f794u); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f796; /* fmov.s @r15+,fr2 */
L8c61f796: sa_motion_publish_stop(ctx,0x8c61f796u); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f798; /* fmov.s @r15+,fr1 */
L8c61f798: sa_motion_publish_stop(ctx,0x8c61f798u); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f79a; /* fmov.s @r15+,fr0 */
L8c61f79a: sa_motion_publish_stop(ctx,0x8c61f79au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f79c; /* frchg */
L8c61f79c: sa_motion_publish_stop(ctx,0x8c61f79cu); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f79e; /* lds.l @r15+,PR */
L8c61f79e: sa_motion_publish_stop(ctx,0x8c61f79eu); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f7a0; /* mov.l @r15+,r9 */
L8c61f7a0: sa_motion_publish_stop(ctx,0x8c61f7a0u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f7a2; /* mov.l @r15+,r10 */
L8c61f7a2: sa_motion_publish_stop(ctx,0x8c61f7a2u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f7a4; /* mov.l @r15+,r11 */
L8c61f7a4: sa_motion_publish_stop(ctx,0x8c61f7a4u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f7a6; /* mov.l @r15+,r12 */
L8c61f7a6: sa_motion_publish_stop(ctx,0x8c61f7a6u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f7a8; /* mov.l @r15+,r13 */
L8c61f7a8: sa_motion_publish_stop(ctx,0x8c61f7aau); if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_environment_constant_primitive_clipped_8c61f7c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 uint32_t entry_pr=*pr;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61f7c8;
L8c61f7c8: sa_motion_publish_stop(ctx,0x8c61f7c8u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61f7ca; /* mov.l r14,@-r15 */
L8c61f7ca: sa_motion_publish_stop(ctx,0x8c61f7cau); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61f7cc; /* mov.l r13,@-r15 */
L8c61f7cc: sa_motion_publish_stop(ctx,0x8c61f7ccu); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61f7ce; /* mov.l r12,@-r15 */
L8c61f7ce: sa_motion_publish_stop(ctx,0x8c61f7ceu); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61f7d0; /* mov.l r11,@-r15 */
L8c61f7d0: sa_motion_publish_stop(ctx,0x8c61f7d0u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61f7d2; /* mov.l r10,@-r15 */
L8c61f7d2: sa_motion_publish_stop(ctx,0x8c61f7d2u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61f7d4; /* mov.l r9,@-r15 */
L8c61f7d4: sa_motion_publish_stop(ctx,0x8c61f7d4u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61f7d6; /* fmov.s fr15,@-r15 */
L8c61f7d6: sa_motion_publish_stop(ctx,0x8c61f7d6u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61f7d8; /* fmov.s fr14,@-r15 */
L8c61f7d8: sa_motion_publish_stop(ctx,0x8c61f7d8u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61f7da; /* sts.l PR,@-r15 */
L8c61f7da: sa_motion_publish_stop(ctx,0x8c61f7dau); r[15]+=4294967288u;goto L8c61f7dc; /* add #-0x8,r15 */
L8c61f7dc: sa_motion_publish_stop(ctx,0x8c61f7dcu); r[0]=0x8c61f834u;goto L8c61f7de; /* mova 0x8c61f834,r0 */
L8c61f7de: sa_motion_publish_stop(ctx,0x8c61f7deu); r[3]=rd(m,bus,&io_status,0x8c61f838u,4);if(io_status)return io_status;goto L8c61f7e0; /* mov.l 0x8c61f838,r3 */
L8c61f7e0: sa_motion_publish_stop(ctx,0x8c61f7e0u); fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c61f7e2; /* fmov.s @r0,fr15 */
L8c61f7e2: sa_motion_publish_stop(ctx,0x8c61f7e2u); r[0]=r[4];goto L8c61f7e4; /* mov r4,r0 */
L8c61f7e4: sa_motion_publish_stop(ctx,0x8c61f7e4u); r[11]=r[4];goto L8c61f7e6; /* mov r4,r11 */
L8c61f7e6: sa_motion_publish_stop(ctx,0x8c61f7e6u); wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61f7e8; /* mov.l r4,@r15 */
L8c61f7e8: sa_motion_publish_stop(ctx,0x8c61f7e8u); r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61f7ea; /* mov.w @r0,r0 */
L8c61f7ea: sa_motion_publish_stop(ctx,0x8c61f7eau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][14]=0x3f800000u;goto L8c61f7ec; /* fldi1 fr14 */
L8c61f7ec: sa_motion_publish_stop(ctx,0x8c61f7ecu); r[0]=r[0]&65535u;goto L8c61f7ee; /* extu.w r0,r0 */
L8c61f7ee: sa_motion_publish_stop(ctx,0x8c61f7eeu); r[0]&=r[3];goto L8c61f7f0; /* and r3,r0 */
L8c61f7f0: sa_motion_publish_stop(ctx,0x8c61f7f0u); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61f7f2; /* cmp/eq #0x0,r0 */
L8c61f7f2: sa_motion_publish_stop(ctx,0x8c61f7f4u); take=(*sr&1u);r[11]=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;if(take)goto L8c61f818;goto L8c61f7f6; /* bt/s 0x8c61f818 */
L8c61f7f6: sa_motion_publish_stop(ctx,0x8c61f7f6u); r[1]=sx(m,bus,&io_status,0x8c61f830u,2);if(io_status)return io_status;goto L8c61f7f8; /* mov.w 0x8c61f830,r1 */
L8c61f7f8: sa_motion_publish_stop(ctx,0x8c61f7f8u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61f7fa; /* cmp/eq r1,r0 */
L8c61f7fa: sa_motion_publish_stop(ctx,0x8c61f7fau); if(!(*sr&1u))goto L8c61f800;goto L8c61f7fc; /* bf 0x8c61f800 */
L8c61f7fc: sa_motion_publish_stop(ctx,0x8c61f7feu); goto L8c61fa26; /* bra 0x8c61fa26 */
L8c61f800: sa_motion_publish_stop(ctx,0x8c61f800u); r[1]=rd(m,bus,&io_status,0x8c61f83cu,4);if(io_status)return io_status;goto L8c61f802; /* mov.l 0x8c61f83c,r1 */
L8c61f802: sa_motion_publish_stop(ctx,0x8c61f802u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61f804; /* cmp/eq r1,r0 */
L8c61f804: sa_motion_publish_stop(ctx,0x8c61f804u); if(!(*sr&1u))goto L8c61f80a;goto L8c61f806; /* bf 0x8c61f80a */
L8c61f806: sa_motion_publish_stop(ctx,0x8c61f808u); goto L8c61fcfc; /* bra 0x8c61fcfc */
L8c61f80a: sa_motion_publish_stop(ctx,0x8c61f80au); r[1]=rd(m,bus,&io_status,0x8c61f838u,4);if(io_status)return io_status;goto L8c61f80c; /* mov.l 0x8c61f838,r1 */
L8c61f80c: sa_motion_publish_stop(ctx,0x8c61f80cu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61f80e; /* cmp/eq r1,r0 */
L8c61f80e: sa_motion_publish_stop(ctx,0x8c61f80eu); if(!(*sr&1u))goto L8c61f814;goto L8c61f810; /* bf 0x8c61f814 */
L8c61f810: sa_motion_publish_stop(ctx,0x8c61f812u); goto L8c61ff3e; /* bra 0x8c61ff3e */
L8c61f814: sa_motion_publish_stop(ctx,0x8c61f816u); goto L8c61ff9e; /* bra 0x8c61ff9e */
L8c61f818: sa_motion_publish_stop(ctx,0x8c61f818u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61f81a; /* mov.l @r15,r2 */
L8c61f81a: sa_motion_publish_stop(ctx,0x8c61f81au); r[1]=rd(m,bus,&io_status,0x8c61f840u,4);if(io_status)return io_status;goto L8c61f81c; /* mov.l 0x8c61f840,r1 */
L8c61f81c: sa_motion_publish_stop(ctx,0x8c61f81cu); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61f81e; /* mov.w @(0x2,r2),r0 */
L8c61f81e: sa_motion_publish_stop(ctx,0x8c61f81eu); r[0]=r[0]&65535u;goto L8c61f820; /* extu.w r0,r0 */
L8c61f820: sa_motion_publish_stop(ctx,0x8c61f820u); r[2]=r[0];goto L8c61f822; /* mov r0,r2 */
L8c61f822: sa_motion_publish_stop(ctx,0x8c61f822u); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61f824; /* shll r0 */
L8c61f824: sa_motion_publish_stop(ctx,0x8c61f824u); r[0]+=r[2];goto L8c61f826; /* add r2,r0 */
L8c61f826: sa_motion_publish_stop(ctx,0x8c61f826u); r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61f828; /* mov.l @r1,r2 */
L8c61f828: sa_motion_publish_stop(ctx,0x8c61f828u); r[2]+=r[0];goto L8c61f82a; /* add r0,r2 */
L8c61f82a: sa_motion_publish_stop(ctx,0x8c61f82au); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61f82c; /* mov.l r2,@r1 */
L8c61f82c: sa_motion_publish_stop(ctx,0x8c61f82eu); r[14]=0u;goto L8c61fa14; /* bra 0x8c61fa14 */
L8c61f844: sa_motion_publish_stop(ctx,0x8c61f844u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61f846; /* mov.w @r11+,r4 */
L8c61f846: sa_motion_publish_stop(ctx,0x8c61f846u); r[3]=rd(m,bus,&io_status,0x8c61fa40u,4);if(io_status)return io_status;goto L8c61f848; /* mov.l 0x8c61fa40,r3 */
L8c61f848: sa_motion_publish_stop(ctx,0x8c61f848u); r[5]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61f84a; /* mov.w @r11+,r5 */
L8c61f84a: sa_motion_publish_stop(ctx,0x8c61f84au); r[4]=r[4]&65535u;goto L8c61f84c; /* extu.w r4,r4 */
L8c61f84c: sa_motion_publish_stop(ctx,0x8c61f84cu); r[4]<<=2;goto L8c61f84e; /* shll2 r4 */
L8c61f84e: sa_motion_publish_stop(ctx,0x8c61f84eu); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f850; /* mov.l @r3,r13 */
L8c61f850: sa_motion_publish_stop(ctx,0x8c61f850u); r[6]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61f852; /* mov.w @r11+,r6 */
L8c61f852: sa_motion_publish_stop(ctx,0x8c61f852u); r[5]=r[5]&65535u;goto L8c61f854; /* extu.w r5,r5 */
L8c61f854: sa_motion_publish_stop(ctx,0x8c61f854u); r[5]<<=2;goto L8c61f856; /* shll2 r5 */
L8c61f856: sa_motion_publish_stop(ctx,0x8c61f856u); r[6]=r[6]&65535u;goto L8c61f858; /* extu.w r6,r6 */
L8c61f858: sa_motion_publish_stop(ctx,0x8c61f858u); r[5]<<=2;goto L8c61f85a; /* shll2 r5 */
L8c61f85a: sa_motion_publish_stop(ctx,0x8c61f85au); r[6]<<=2;goto L8c61f85c; /* shll2 r6 */
L8c61f85c: sa_motion_publish_stop(ctx,0x8c61f85cu); r[4]<<=2;goto L8c61f85e; /* shll2 r4 */
L8c61f85e: sa_motion_publish_stop(ctx,0x8c61f85eu); r[6]<<=2;goto L8c61f860; /* shll2 r6 */
L8c61f860: sa_motion_publish_stop(ctx,0x8c61f860u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61f862; /* shll r4 */
L8c61f862: sa_motion_publish_stop(ctx,0x8c61f862u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61f864; /* shll r5 */
L8c61f864: sa_motion_publish_stop(ctx,0x8c61f864u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61f866; /* shll r6 */
L8c61f866: sa_motion_publish_stop(ctx,0x8c61f866u); r[4]+=r[13];goto L8c61f868; /* add r13,r4 */
L8c61f868: sa_motion_publish_stop(ctx,0x8c61f868u); r[5]+=r[13];goto L8c61f86a; /* add r13,r5 */
L8c61f86a: sa_motion_publish_stop(ctx,0x8c61f86au); r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61f86c; /* mov.l @(0xc,r4),r7 */
L8c61f86c: sa_motion_publish_stop(ctx,0x8c61f86cu); r[2]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c61f86e; /* mov.l @(0xc,r5),r2 */
L8c61f86e: sa_motion_publish_stop(ctx,0x8c61f86eu); r[6]+=r[13];goto L8c61f870; /* add r13,r6 */
L8c61f870: sa_motion_publish_stop(ctx,0x8c61f870u); r[1]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c61f872; /* mov.l @(0xc,r6),r1 */
L8c61f872: sa_motion_publish_stop(ctx,0x8c61f872u); r[7]&=r[2];goto L8c61f874; /* and r2,r7 */
L8c61f874: sa_motion_publish_stop(ctx,0x8c61f874u); r[2]=63u;goto L8c61f876; /* mov #0x3f,r2 */
L8c61f876: sa_motion_publish_stop(ctx,0x8c61f876u); r[7]&=r[1];goto L8c61f878; /* and r1,r7 */
L8c61f878: sa_motion_publish_stop(ctx,0x8c61f878u); *sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c61f87a; /* tst r2,r7 */
L8c61f87a: sa_motion_publish_stop(ctx,0x8c61f87au); if((*sr&1u))goto L8c61f880;goto L8c61f87c; /* bt 0x8c61f880 */
L8c61f87c: sa_motion_publish_stop(ctx,0x8c61f87eu); goto L8c61fa12; /* bra 0x8c61fa12 */
L8c61f880: sa_motion_publish_stop(ctx,0x8c61f880u); r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61f882; /* mov.l @(0xc,r4),r7 */
L8c61f882: sa_motion_publish_stop(ctx,0x8c61f882u); r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c61f884; /* mov.l @(0xc,r5),r3 */
L8c61f884: sa_motion_publish_stop(ctx,0x8c61f884u); r[2]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c61f886; /* mov.l @(0xc,r6),r2 */
L8c61f886: sa_motion_publish_stop(ctx,0x8c61f886u); r[7]|=r[3];goto L8c61f888; /* or r3,r7 */
L8c61f888: sa_motion_publish_stop(ctx,0x8c61f888u); r[3]=1u;goto L8c61f88a; /* mov #0x1,r3 */
L8c61f88a: sa_motion_publish_stop(ctx,0x8c61f88au); r[7]|=r[2];goto L8c61f88c; /* or r2,r7 */
L8c61f88c: sa_motion_publish_stop(ctx,0x8c61f88cu); *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c61f88e; /* tst r3,r7 */
L8c61f88e: sa_motion_publish_stop(ctx,0x8c61f88eu); if((*sr&1u))goto L8c61f894;goto L8c61f890; /* bt 0x8c61f894 */
L8c61f890: sa_motion_publish_stop(ctx,0x8c61f892u); goto L8c61fa08; /* bra 0x8c61fa08 */
L8c61f894: sa_motion_publish_stop(ctx,0x8c61f894u); r[0]=16u;goto L8c61f896; /* mov #0x10,r0 */
L8c61f896: sa_motion_publish_stop(ctx,0x8c61f896u); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f898; /* fmov.s @(r0,r5),fr5 */
L8c61f898: sa_motion_publish_stop(ctx,0x8c61f898u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f89a; /* fmov.s @(r0,r4),fr3 */
L8c61f89a: sa_motion_publish_stop(ctx,0x8c61f89au); r[0]=20u;goto L8c61f89c; /* mov #0x14,r0 */
L8c61f89c: sa_motion_publish_stop(ctx,0x8c61f89cu); fcopy(f,2u,5u);goto L8c61f89e; /* fmov fr5,fr2 */
L8c61f89e: sa_motion_publish_stop(ctx,0x8c61f89eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61f8a0; /* fsub fr3,fr2 */
L8c61f8a0: sa_motion_publish_stop(ctx,0x8c61f8a0u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f8a2; /* fmov.s @(r0,r6),fr3 */
L8c61f8a2: sa_motion_publish_stop(ctx,0x8c61f8a2u); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f8a4; /* fmov.s @(r0,r5),fr4 */
L8c61f8a4: sa_motion_publish_stop(ctx,0x8c61f8a4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f8a6; /* fsub fr4,fr3 */
L8c61f8a6: sa_motion_publish_stop(ctx,0x8c61f8a6u); fcopy(f,1u,4u);goto L8c61f8a8; /* fmov fr4,fr1 */
L8c61f8a8: sa_motion_publish_stop(ctx,0x8c61f8a8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f8aa; /* fmul fr3,fr2 */
L8c61f8aa: sa_motion_publish_stop(ctx,0x8c61f8aau); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f8ac; /* fmov.s @(r0,r4),fr3 */
L8c61f8ac: sa_motion_publish_stop(ctx,0x8c61f8acu); r[0]=16u;goto L8c61f8ae; /* mov #0x10,r0 */
L8c61f8ae: sa_motion_publish_stop(ctx,0x8c61f8aeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61f8b0; /* fsub fr3,fr1 */
L8c61f8b0: sa_motion_publish_stop(ctx,0x8c61f8b0u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f8b2; /* fmov.s @(r0,r6),fr3 */
L8c61f8b2: sa_motion_publish_stop(ctx,0x8c61f8b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f8b4; /* fsub fr5,fr3 */
L8c61f8b4: sa_motion_publish_stop(ctx,0x8c61f8b4u); fcopy(f,4u,2u);goto L8c61f8b6; /* fmov fr2,fr4 */
L8c61f8b6: sa_motion_publish_stop(ctx,0x8c61f8b6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61f8b8; /* fmul fr3,fr1 */
L8c61f8b8: sa_motion_publish_stop(ctx,0x8c61f8b8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61f8ba; /* fldi0 fr3 */
L8c61f8ba: sa_motion_publish_stop(ctx,0x8c61f8bau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61f8bc; /* fsub fr1,fr4 */
L8c61f8bc: sa_motion_publish_stop(ctx,0x8c61f8bcu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61f8be; /* fcmp/gt fr3,fr4 */
L8c61f8be: sa_motion_publish_stop(ctx,0x8c61f8beu); if(!(*sr&1u))goto L8c61f8c4;goto L8c61f8c0; /* bf 0x8c61f8c4 */
L8c61f8c0: sa_motion_publish_stop(ctx,0x8c61f8c2u); goto L8c61fa12; /* bra 0x8c61fa12 */
L8c61f8c4: sa_motion_publish_stop(ctx,0x8c61f8c4u); r[2]=rd(m,bus,&io_status,0x8c61fa44u,4);if(io_status)return io_status;goto L8c61f8c6; /* mov.l 0x8c61fa44,r2 */
L8c61f8c6: sa_motion_publish_stop(ctx,0x8c61f8c6u); r[0]=4u;goto L8c61f8c8; /* mov #0x4,r0 */
L8c61f8c8: sa_motion_publish_stop(ctx,0x8c61f8c8u); r[8]+=28u;goto L8c61f8ca; /* add #0x1c,r8 */
L8c61f8ca: sa_motion_publish_stop(ctx,0x8c61f8cau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61f8cc; /* fmov.s fr14,@-r8 */
L8c61f8cc: sa_motion_publish_stop(ctx,0x8c61f8ccu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61f8ce; /* mov.l @r2,r3 */
L8c61f8ce: sa_motion_publish_stop(ctx,0x8c61f8ceu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f8d0; /* fmov.s @(r0,r4),fr0 */
L8c61f8d0: sa_motion_publish_stop(ctx,0x8c61f8d0u); r[0]=20u;goto L8c61f8d2; /* mov #0x14,r0 */
L8c61f8d2: sa_motion_publish_stop(ctx,0x8c61f8d2u); r[2]=r[3];goto L8c61f8d4; /* mov r3,r2 */
L8c61f8d4: sa_motion_publish_stop(ctx,0x8c61f8d4u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f8d6; /* fmov.s @(r0,r2),fr3 */
L8c61f8d6: sa_motion_publish_stop(ctx,0x8c61f8d6u); r[1]=r[3];goto L8c61f8d8; /* mov r3,r1 */
L8c61f8d8: sa_motion_publish_stop(ctx,0x8c61f8d8u); r[0]=4u;goto L8c61f8da; /* mov #0x4,r0 */
L8c61f8da: sa_motion_publish_stop(ctx,0x8c61f8dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f8dc; /* fmul fr0,fr3 */
L8c61f8dc: sa_motion_publish_stop(ctx,0x8c61f8dcu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f8de; /* fmov.s @(r0,r1),fr2 */
L8c61f8de: sa_motion_publish_stop(ctx,0x8c61f8deu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61f8e0; /* fmov.s @r4,fr0 */
L8c61f8e0: sa_motion_publish_stop(ctx,0x8c61f8e0u); r[0]=8u;goto L8c61f8e2; /* mov #0x8,r0 */
L8c61f8e2: sa_motion_publish_stop(ctx,0x8c61f8e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f8e4; /* fmac fr0,fr2,fr3 */
L8c61f8e4: sa_motion_publish_stop(ctx,0x8c61f8e4u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f8e6; /* fmov.s @(r0,r4),fr0 */
L8c61f8e6: sa_motion_publish_stop(ctx,0x8c61f8e6u); r[0]=36u;goto L8c61f8e8; /* mov #0x24,r0 */
L8c61f8e8: sa_motion_publish_stop(ctx,0x8c61f8e8u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f8ea; /* fmov.s @(r0,r3),fr2 */
L8c61f8ea: sa_motion_publish_stop(ctx,0x8c61f8eau); r[0]=4u;goto L8c61f8ec; /* mov #0x4,r0 */
L8c61f8ec: sa_motion_publish_stop(ctx,0x8c61f8ecu); r[3]=rd(m,bus,&io_status,0x8c61fa44u,4);if(io_status)return io_status;goto L8c61f8ee; /* mov.l 0x8c61fa44,r3 */
L8c61f8ee: sa_motion_publish_stop(ctx,0x8c61f8eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f8f0; /* fmac fr0,fr2,fr3 */
L8c61f8f0: sa_motion_publish_stop(ctx,0x8c61f8f0u); fcopy(f,2u,15u);goto L8c61f8f2; /* fmov fr15,fr2 */
L8c61f8f2: sa_motion_publish_stop(ctx,0x8c61f8f2u); fcopy(f,0u,15u);goto L8c61f8f4; /* fmov fr15,fr0 */
L8c61f8f4: sa_motion_publish_stop(ctx,0x8c61f8f4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f8f6; /* fmac fr0,fr3,fr2 */
L8c61f8f6: sa_motion_publish_stop(ctx,0x8c61f8f6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f8f8; /* fmov.s fr2,@-r8 */
L8c61f8f8: sa_motion_publish_stop(ctx,0x8c61f8f8u); r[9]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f8fa; /* mov.l @r3,r9 */
L8c61f8fa: sa_motion_publish_stop(ctx,0x8c61f8fau); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f8fc; /* fmov.s @(r0,r4),fr0 */
L8c61f8fc: sa_motion_publish_stop(ctx,0x8c61f8fcu); r[0]=16u;goto L8c61f8fe; /* mov #0x10,r0 */
L8c61f8fe: sa_motion_publish_stop(ctx,0x8c61f8feu); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61f900; /* fmov.s @(r0,r9),fr3 */
L8c61f900: sa_motion_publish_stop(ctx,0x8c61f900u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f902; /* fmul fr0,fr3 */
L8c61f902: sa_motion_publish_stop(ctx,0x8c61f902u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61f904; /* fmov.s @r4,fr0 */
L8c61f904: sa_motion_publish_stop(ctx,0x8c61f904u); fload(m,bus,&io_status,f,2u,r[9]);if(io_status)return io_status;goto L8c61f906; /* fmov.s @r9,fr2 */
L8c61f906: sa_motion_publish_stop(ctx,0x8c61f906u); r[0]=8u;goto L8c61f908; /* mov #0x8,r0 */
L8c61f908: sa_motion_publish_stop(ctx,0x8c61f908u); r[2]=rd(m,bus,&io_status,0x8c61fa48u,4);if(io_status)return io_status;goto L8c61f90a; /* mov.l 0x8c61fa48,r2 */
L8c61f90a: sa_motion_publish_stop(ctx,0x8c61f90au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f90c; /* fmac fr0,fr2,fr3 */
L8c61f90c: sa_motion_publish_stop(ctx,0x8c61f90cu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f90e; /* fmov.s @(r0,r4),fr0 */
L8c61f90e: sa_motion_publish_stop(ctx,0x8c61f90eu); r[0]=32u;goto L8c61f910; /* mov #0x20,r0 */
L8c61f910: sa_motion_publish_stop(ctx,0x8c61f910u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61f912; /* fmov.s @(r0,r9),fr2 */
L8c61f912: sa_motion_publish_stop(ctx,0x8c61f912u); r[0]=24u;goto L8c61f914; /* mov #0x18,r0 */
L8c61f914: sa_motion_publish_stop(ctx,0x8c61f914u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f916; /* fmac fr0,fr2,fr3 */
L8c61f916: sa_motion_publish_stop(ctx,0x8c61f916u); fcopy(f,0u,15u);goto L8c61f918; /* fmov fr15,fr0 */
L8c61f918: sa_motion_publish_stop(ctx,0x8c61f918u); fcopy(f,2u,15u);goto L8c61f91a; /* fmov fr15,fr2 */
L8c61f91a: sa_motion_publish_stop(ctx,0x8c61f91au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f91c; /* fmac fr0,fr3,fr2 */
L8c61f91c: sa_motion_publish_stop(ctx,0x8c61f91cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f91e; /* fmov.s fr2,@-r8 */
L8c61f91e: sa_motion_publish_stop(ctx,0x8c61f91eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f920; /* fmov.s @(r0,r4),fr3 */
L8c61f920: sa_motion_publish_stop(ctx,0x8c61f920u); r[0]=20u;goto L8c61f922; /* mov #0x14,r0 */
L8c61f922: sa_motion_publish_stop(ctx,0x8c61f922u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f924; /* fmov.s fr3,@-r8 */
L8c61f924: sa_motion_publish_stop(ctx,0x8c61f924u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f926; /* fmov.s @(r0,r4),fr3 */
L8c61f926: sa_motion_publish_stop(ctx,0x8c61f926u); r[0]=16u;goto L8c61f928; /* mov #0x10,r0 */
L8c61f928: sa_motion_publish_stop(ctx,0x8c61f928u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f92a; /* fmov.s fr3,@-r8 */
L8c61f92a: sa_motion_publish_stop(ctx,0x8c61f92au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f92c; /* fmov.s @(r0,r4),fr3 */
L8c61f92c: sa_motion_publish_stop(ctx,0x8c61f92cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f92e; /* fmov.s fr3,@-r8 */
L8c61f92e: sa_motion_publish_stop(ctx,0x8c61f92eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61f930; /* mov.l r2,@-r8 */
L8c61f930: sa_motion_publish_stop(ctx,0x8c61f930u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f932; /* pref @r8 */
L8c61f932: sa_motion_publish_stop(ctx,0x8c61f932u); r[0]=4u;goto L8c61f934; /* mov #0x4,r0 */
L8c61f934: sa_motion_publish_stop(ctx,0x8c61f934u); r[8]+=60u;goto L8c61f936; /* add #0x3c,r8 */
L8c61f936: sa_motion_publish_stop(ctx,0x8c61f936u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61f938; /* fmov.s fr14,@-r8 */
L8c61f938: sa_motion_publish_stop(ctx,0x8c61f938u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f93a; /* fmov.s @(r0,r5),fr0 */
L8c61f93a: sa_motion_publish_stop(ctx,0x8c61f93au); r[0]=20u;goto L8c61f93c; /* mov #0x14,r0 */
L8c61f93c: sa_motion_publish_stop(ctx,0x8c61f93cu); r[10]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f93e; /* mov.l @r3,r10 */
L8c61f93e: sa_motion_publish_stop(ctx,0x8c61f93eu); fload(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c61f940; /* fmov.s @(r0,r10),fr3 */
L8c61f940: sa_motion_publish_stop(ctx,0x8c61f940u); r[0]=4u;goto L8c61f942; /* mov #0x4,r0 */
L8c61f942: sa_motion_publish_stop(ctx,0x8c61f942u); fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c61f944; /* fmov.s @(r0,r10),fr2 */
L8c61f944: sa_motion_publish_stop(ctx,0x8c61f944u); r[0]=8u;goto L8c61f946; /* mov #0x8,r0 */
L8c61f946: sa_motion_publish_stop(ctx,0x8c61f946u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f948; /* fmul fr0,fr3 */
L8c61f948: sa_motion_publish_stop(ctx,0x8c61f948u); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f94a; /* fmov.s @r5,fr0 */
L8c61f94a: sa_motion_publish_stop(ctx,0x8c61f94au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f94c; /* fmac fr0,fr2,fr3 */
L8c61f94c: sa_motion_publish_stop(ctx,0x8c61f94cu); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f94e; /* fmov.s @(r0,r5),fr0 */
L8c61f94e: sa_motion_publish_stop(ctx,0x8c61f94eu); r[0]=36u;goto L8c61f950; /* mov #0x24,r0 */
L8c61f950: sa_motion_publish_stop(ctx,0x8c61f950u); fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c61f952; /* fmov.s @(r0,r10),fr2 */
L8c61f952: sa_motion_publish_stop(ctx,0x8c61f952u); r[0]=4u;goto L8c61f954; /* mov #0x4,r0 */
L8c61f954: sa_motion_publish_stop(ctx,0x8c61f954u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f956; /* fmac fr0,fr2,fr3 */
L8c61f956: sa_motion_publish_stop(ctx,0x8c61f956u); fcopy(f,2u,15u);goto L8c61f958; /* fmov fr15,fr2 */
L8c61f958: sa_motion_publish_stop(ctx,0x8c61f958u); fcopy(f,0u,15u);goto L8c61f95a; /* fmov fr15,fr0 */
L8c61f95a: sa_motion_publish_stop(ctx,0x8c61f95au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f95c; /* fmac fr0,fr3,fr2 */
L8c61f95c: sa_motion_publish_stop(ctx,0x8c61f95cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f95e; /* fmov.s fr2,@-r8 */
L8c61f95e: sa_motion_publish_stop(ctx,0x8c61f95eu); r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f960; /* mov.l @r3,r12 */
L8c61f960: sa_motion_publish_stop(ctx,0x8c61f960u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f962; /* fmov.s @(r0,r5),fr0 */
L8c61f962: sa_motion_publish_stop(ctx,0x8c61f962u); r[0]=16u;goto L8c61f964; /* mov #0x10,r0 */
L8c61f964: sa_motion_publish_stop(ctx,0x8c61f964u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61f966; /* fmov.s @(r0,r12),fr3 */
L8c61f966: sa_motion_publish_stop(ctx,0x8c61f966u); r[0]=8u;goto L8c61f968; /* mov #0x8,r0 */
L8c61f968: sa_motion_publish_stop(ctx,0x8c61f968u); fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61f96a; /* fmov.s @r12,fr2 */
L8c61f96a: sa_motion_publish_stop(ctx,0x8c61f96au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f96c; /* fmul fr0,fr3 */
L8c61f96c: sa_motion_publish_stop(ctx,0x8c61f96cu); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f96e; /* fmov.s @r5,fr0 */
L8c61f96e: sa_motion_publish_stop(ctx,0x8c61f96eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f970; /* fmac fr0,fr2,fr3 */
L8c61f970: sa_motion_publish_stop(ctx,0x8c61f970u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f972; /* fmov.s @(r0,r5),fr0 */
L8c61f972: sa_motion_publish_stop(ctx,0x8c61f972u); r[0]=32u;goto L8c61f974; /* mov #0x20,r0 */
L8c61f974: sa_motion_publish_stop(ctx,0x8c61f974u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61f976; /* fmov.s @(r0,r12),fr2 */
L8c61f976: sa_motion_publish_stop(ctx,0x8c61f976u); r[0]=24u;goto L8c61f978; /* mov #0x18,r0 */
L8c61f978: sa_motion_publish_stop(ctx,0x8c61f978u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f97a; /* fmac fr0,fr2,fr3 */
L8c61f97a: sa_motion_publish_stop(ctx,0x8c61f97au); fcopy(f,0u,15u);goto L8c61f97c; /* fmov fr15,fr0 */
L8c61f97c: sa_motion_publish_stop(ctx,0x8c61f97cu); fcopy(f,2u,15u);goto L8c61f97e; /* fmov fr15,fr2 */
L8c61f97e: sa_motion_publish_stop(ctx,0x8c61f97eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f980; /* fmac fr0,fr3,fr2 */
L8c61f980: sa_motion_publish_stop(ctx,0x8c61f980u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f982; /* fmov.s fr2,@-r8 */
L8c61f982: sa_motion_publish_stop(ctx,0x8c61f982u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f984; /* fmov.s @(r0,r5),fr3 */
L8c61f984: sa_motion_publish_stop(ctx,0x8c61f984u); r[0]=20u;goto L8c61f986; /* mov #0x14,r0 */
L8c61f986: sa_motion_publish_stop(ctx,0x8c61f986u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f988; /* fmov.s fr3,@-r8 */
L8c61f988: sa_motion_publish_stop(ctx,0x8c61f988u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f98a; /* fmov.s @(r0,r5),fr3 */
L8c61f98a: sa_motion_publish_stop(ctx,0x8c61f98au); r[0]=16u;goto L8c61f98c; /* mov #0x10,r0 */
L8c61f98c: sa_motion_publish_stop(ctx,0x8c61f98cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f98e; /* fmov.s fr3,@-r8 */
L8c61f98e: sa_motion_publish_stop(ctx,0x8c61f98eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f990; /* fmov.s @(r0,r5),fr3 */
L8c61f990: sa_motion_publish_stop(ctx,0x8c61f990u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f992; /* fmov.s fr3,@-r8 */
L8c61f992: sa_motion_publish_stop(ctx,0x8c61f992u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61f994; /* mov.l r2,@-r8 */
L8c61f994: sa_motion_publish_stop(ctx,0x8c61f994u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f996; /* pref @r8 */
L8c61f996: sa_motion_publish_stop(ctx,0x8c61f996u); r[0]=4u;goto L8c61f998; /* mov #0x4,r0 */
L8c61f998: sa_motion_publish_stop(ctx,0x8c61f998u); r[8]+=60u;goto L8c61f99a; /* add #0x3c,r8 */
L8c61f99a: sa_motion_publish_stop(ctx,0x8c61f99au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61f99c; /* fmov.s fr14,@-r8 */
L8c61f99c: sa_motion_publish_stop(ctx,0x8c61f99cu); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f99e; /* fmov.s @(r0,r6),fr0 */
L8c61f99e: sa_motion_publish_stop(ctx,0x8c61f99eu); r[0]=20u;goto L8c61f9a0; /* mov #0x14,r0 */
L8c61f9a0: sa_motion_publish_stop(ctx,0x8c61f9a0u); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f9a2; /* mov.l @r3,r13 */
L8c61f9a2: sa_motion_publish_stop(ctx,0x8c61f9a2u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61f9a4; /* fmov.s @(r0,r13),fr3 */
L8c61f9a4: sa_motion_publish_stop(ctx,0x8c61f9a4u); r[0]=4u;goto L8c61f9a6; /* mov #0x4,r0 */
L8c61f9a6: sa_motion_publish_stop(ctx,0x8c61f9a6u); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61f9a8; /* fmov.s @(r0,r13),fr2 */
L8c61f9a8: sa_motion_publish_stop(ctx,0x8c61f9a8u); r[0]=8u;goto L8c61f9aa; /* mov #0x8,r0 */
L8c61f9aa: sa_motion_publish_stop(ctx,0x8c61f9aau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f9ac; /* fmul fr0,fr3 */
L8c61f9ac: sa_motion_publish_stop(ctx,0x8c61f9acu); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61f9ae; /* fmov.s @r6,fr0 */
L8c61f9ae: sa_motion_publish_stop(ctx,0x8c61f9aeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f9b0; /* fmac fr0,fr2,fr3 */
L8c61f9b0: sa_motion_publish_stop(ctx,0x8c61f9b0u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f9b2; /* fmov.s @(r0,r6),fr0 */
L8c61f9b2: sa_motion_publish_stop(ctx,0x8c61f9b2u); r[0]=36u;goto L8c61f9b4; /* mov #0x24,r0 */
L8c61f9b4: sa_motion_publish_stop(ctx,0x8c61f9b4u); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61f9b6; /* fmov.s @(r0,r13),fr2 */
L8c61f9b6: sa_motion_publish_stop(ctx,0x8c61f9b6u); r[0]=4u;goto L8c61f9b8; /* mov #0x4,r0 */
L8c61f9b8: sa_motion_publish_stop(ctx,0x8c61f9b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f9ba; /* fmac fr0,fr2,fr3 */
L8c61f9ba: sa_motion_publish_stop(ctx,0x8c61f9bau); fcopy(f,2u,15u);goto L8c61f9bc; /* fmov fr15,fr2 */
L8c61f9bc: sa_motion_publish_stop(ctx,0x8c61f9bcu); fcopy(f,0u,15u);goto L8c61f9be; /* fmov fr15,fr0 */
L8c61f9be: sa_motion_publish_stop(ctx,0x8c61f9beu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f9c0; /* fmac fr0,fr3,fr2 */
L8c61f9c0: sa_motion_publish_stop(ctx,0x8c61f9c0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f9c2; /* fmov.s fr2,@-r8 */
L8c61f9c2: sa_motion_publish_stop(ctx,0x8c61f9c2u); r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f9c4; /* mov.l @r3,r7 */
L8c61f9c4: sa_motion_publish_stop(ctx,0x8c61f9c4u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f9c6; /* fmov.s @(r0,r6),fr0 */
L8c61f9c6: sa_motion_publish_stop(ctx,0x8c61f9c6u); r[0]=16u;goto L8c61f9c8; /* mov #0x10,r0 */
L8c61f9c8: sa_motion_publish_stop(ctx,0x8c61f9c8u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f9ca; /* fmov.s @(r0,r7),fr3 */
L8c61f9ca: sa_motion_publish_stop(ctx,0x8c61f9cau); r[0]=8u;goto L8c61f9cc; /* mov #0x8,r0 */
L8c61f9cc: sa_motion_publish_stop(ctx,0x8c61f9ccu); fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61f9ce; /* fmov.s @r7,fr2 */
L8c61f9ce: sa_motion_publish_stop(ctx,0x8c61f9ceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f9d0; /* fmul fr0,fr3 */
L8c61f9d0: sa_motion_publish_stop(ctx,0x8c61f9d0u); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61f9d2; /* fmov.s @r6,fr0 */
L8c61f9d2: sa_motion_publish_stop(ctx,0x8c61f9d2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f9d4; /* fmac fr0,fr2,fr3 */
L8c61f9d4: sa_motion_publish_stop(ctx,0x8c61f9d4u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f9d6; /* fmov.s @(r0,r6),fr0 */
L8c61f9d6: sa_motion_publish_stop(ctx,0x8c61f9d6u); r[0]=32u;goto L8c61f9d8; /* mov #0x20,r0 */
L8c61f9d8: sa_motion_publish_stop(ctx,0x8c61f9d8u); r[2]=rd(m,bus,&io_status,0x8c61fa4cu,4);if(io_status)return io_status;goto L8c61f9da; /* mov.l 0x8c61fa4c,r2 */
L8c61f9da: sa_motion_publish_stop(ctx,0x8c61f9dau); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f9dc; /* fmov.s @(r0,r7),fr2 */
L8c61f9dc: sa_motion_publish_stop(ctx,0x8c61f9dcu); r[0]=24u;goto L8c61f9de; /* mov #0x18,r0 */
L8c61f9de: sa_motion_publish_stop(ctx,0x8c61f9deu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f9e0; /* fmac fr0,fr2,fr3 */
L8c61f9e0: sa_motion_publish_stop(ctx,0x8c61f9e0u); fcopy(f,0u,15u);goto L8c61f9e2; /* fmov fr15,fr0 */
L8c61f9e2: sa_motion_publish_stop(ctx,0x8c61f9e2u); fcopy(f,2u,15u);goto L8c61f9e4; /* fmov fr15,fr2 */
L8c61f9e4: sa_motion_publish_stop(ctx,0x8c61f9e4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f9e6; /* fmac fr0,fr3,fr2 */
L8c61f9e6: sa_motion_publish_stop(ctx,0x8c61f9e6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f9e8; /* fmov.s fr2,@-r8 */
L8c61f9e8: sa_motion_publish_stop(ctx,0x8c61f9e8u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f9ea; /* fmov.s @(r0,r6),fr3 */
L8c61f9ea: sa_motion_publish_stop(ctx,0x8c61f9eau); r[0]=20u;goto L8c61f9ec; /* mov #0x14,r0 */
L8c61f9ec: sa_motion_publish_stop(ctx,0x8c61f9ecu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f9ee; /* fmov.s fr3,@-r8 */
L8c61f9ee: sa_motion_publish_stop(ctx,0x8c61f9eeu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f9f0; /* fmov.s @(r0,r6),fr3 */
L8c61f9f0: sa_motion_publish_stop(ctx,0x8c61f9f0u); r[0]=16u;goto L8c61f9f2; /* mov #0x10,r0 */
L8c61f9f2: sa_motion_publish_stop(ctx,0x8c61f9f2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f9f4; /* fmov.s fr3,@-r8 */
L8c61f9f4: sa_motion_publish_stop(ctx,0x8c61f9f4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f9f6; /* fmov.s @(r0,r6),fr3 */
L8c61f9f6: sa_motion_publish_stop(ctx,0x8c61f9f6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f9f8; /* fmov.s fr3,@-r8 */
L8c61f9f8: sa_motion_publish_stop(ctx,0x8c61f9f8u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61f9fa; /* mov.l r2,@-r8 */
L8c61f9fa: sa_motion_publish_stop(ctx,0x8c61f9fau); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f9fc; /* pref @r8 */
L8c61f9fc: sa_motion_publish_stop(ctx,0x8c61f9fcu); r[1]=rd(m,bus,&io_status,0x8c61fa50u,4);if(io_status)return io_status;goto L8c61f9fe; /* mov.l 0x8c61fa50,r1 */
L8c61f9fe: sa_motion_publish_stop(ctx,0x8c61f9feu); r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61fa00; /* mov.l @r1,r2 */
L8c61fa00: sa_motion_publish_stop(ctx,0x8c61fa00u); r[2]+=3u;goto L8c61fa02; /* add #0x3,r2 */
L8c61fa02: sa_motion_publish_stop(ctx,0x8c61fa02u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61fa04; /* mov.l r2,@r1 */
L8c61fa04: sa_motion_publish_stop(ctx,0x8c61fa06u); r[8]+=32u;goto L8c61fa12; /* bra 0x8c61fa12 */
L8c61fa08: sa_motion_publish_stop(ctx,0x8c61fa08u); r[0]=rd(m,bus,&io_status,0x8c61fa54u,4);if(io_status)return io_status;goto L8c61fa0a; /* mov.l 0x8c61fa54,r0 */
L8c61fa0a: sa_motion_publish_stop(ctx,0x8c61fa0au); wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61fa0c; /* mov.l r3,@r0 */
L8c61fa0c: sa_motion_publish_stop(ctx,0x8c61fa0cu); r[3]=rd(m,bus,&io_status,0x8c61fa58u,4);if(io_status)return io_status;goto L8c61fa0e; /* mov.l 0x8c61fa58,r3 */
L8c61fa0e: sa_motion_publish_stop(ctx,0x8c61fa10u); target=r[3];*pr=0x8c61fa12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61fa12; /* jsr @r3 */
L8c61fa12: sa_motion_publish_stop(ctx,0x8c61fa12u); r[14]+=1u;goto L8c61fa14; /* add #0x1,r14 */
L8c61fa14: sa_motion_publish_stop(ctx,0x8c61fa14u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61fa16; /* mov.l @r15,r2 */
L8c61fa16: sa_motion_publish_stop(ctx,0x8c61fa16u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61fa18; /* mov.w @(0x2,r2),r0 */
L8c61fa18: sa_motion_publish_stop(ctx,0x8c61fa18u); r[0]=r[0]&65535u;goto L8c61fa1a; /* extu.w r0,r0 */
L8c61fa1a: sa_motion_publish_stop(ctx,0x8c61fa1au); *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c61fa1c; /* cmp/ge r0,r14 */
L8c61fa1c: sa_motion_publish_stop(ctx,0x8c61fa1cu); if((*sr&1u))goto L8c61fa22;goto L8c61fa1e; /* bt 0x8c61fa22 */
L8c61fa1e: sa_motion_publish_stop(ctx,0x8c61fa20u); goto L8c61f844; /* bra 0x8c61f844 */
L8c61fa22: sa_motion_publish_stop(ctx,0x8c61fa24u); goto L8c61ff9e; /* bra 0x8c61ff9e */
L8c61fa26: sa_motion_publish_stop(ctx,0x8c61fa26u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61fa28; /* mov.l @r15,r1 */
L8c61fa28: sa_motion_publish_stop(ctx,0x8c61fa28u); r[2]=rd(m,bus,&io_status,0x8c61fa5cu,4);if(io_status)return io_status;goto L8c61fa2a; /* mov.l 0x8c61fa5c,r2 */
L8c61fa2a: sa_motion_publish_stop(ctx,0x8c61fa2au); r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61fa2c; /* mov.w @(0x2,r1),r0 */
L8c61fa2c: sa_motion_publish_stop(ctx,0x8c61fa2cu); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fa2e; /* mov.l @r2,r1 */
L8c61fa2e: sa_motion_publish_stop(ctx,0x8c61fa2eu); r[0]=r[0]&65535u;goto L8c61fa30; /* extu.w r0,r0 */
L8c61fa30: sa_motion_publish_stop(ctx,0x8c61fa30u); r[0]<<=2;goto L8c61fa32; /* shll2 r0 */
L8c61fa32: sa_motion_publish_stop(ctx,0x8c61fa32u); r[1]+=r[0];goto L8c61fa34; /* add r0,r1 */
L8c61fa34: sa_motion_publish_stop(ctx,0x8c61fa34u); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61fa36; /* mov.l r1,@r2 */
L8c61fa36: sa_motion_publish_stop(ctx,0x8c61fa38u); r[10]=0u;goto L8c61fcea; /* bra 0x8c61fcea */
L8c61fa60: sa_motion_publish_stop(ctx,0x8c61fa60u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fa62; /* mov.w @r11+,r4 */
L8c61fa62: sa_motion_publish_stop(ctx,0x8c61fa62u); r[3]=rd(m,bus,&io_status,0x8c61fd00u,4);if(io_status)return io_status;goto L8c61fa64; /* mov.l 0x8c61fd00,r3 */
L8c61fa64: sa_motion_publish_stop(ctx,0x8c61fa64u); r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fa66; /* mov.w @r11+,r13 */
L8c61fa66: sa_motion_publish_stop(ctx,0x8c61fa66u); r[4]=r[4]&65535u;goto L8c61fa68; /* extu.w r4,r4 */
L8c61fa68: sa_motion_publish_stop(ctx,0x8c61fa68u); r[4]<<=2;goto L8c61fa6a; /* shll2 r4 */
L8c61fa6a: sa_motion_publish_stop(ctx,0x8c61fa6au); r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fa6c; /* mov.l @r3,r6 */
L8c61fa6c: sa_motion_publish_stop(ctx,0x8c61fa6cu); r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fa6e; /* mov.w @r11+,r14 */
L8c61fa6e: sa_motion_publish_stop(ctx,0x8c61fa6eu); r[13]=r[13]&65535u;goto L8c61fa70; /* extu.w r13,r13 */
L8c61fa70: sa_motion_publish_stop(ctx,0x8c61fa70u); r[13]<<=2;goto L8c61fa72; /* shll2 r13 */
L8c61fa72: sa_motion_publish_stop(ctx,0x8c61fa72u); r[12]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fa74; /* mov.w @r11+,r12 */
L8c61fa74: sa_motion_publish_stop(ctx,0x8c61fa74u); r[14]=r[14]&65535u;goto L8c61fa76; /* extu.w r14,r14 */
L8c61fa76: sa_motion_publish_stop(ctx,0x8c61fa76u); r[13]<<=2;goto L8c61fa78; /* shll2 r13 */
L8c61fa78: sa_motion_publish_stop(ctx,0x8c61fa78u); r[4]<<=2;goto L8c61fa7a; /* shll2 r4 */
L8c61fa7a: sa_motion_publish_stop(ctx,0x8c61fa7au); r[12]=r[12]&65535u;goto L8c61fa7c; /* extu.w r12,r12 */
L8c61fa7c: sa_motion_publish_stop(ctx,0x8c61fa7cu); r[14]<<=2;goto L8c61fa7e; /* shll2 r14 */
L8c61fa7e: sa_motion_publish_stop(ctx,0x8c61fa7eu); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61fa80; /* shll r4 */
L8c61fa80: sa_motion_publish_stop(ctx,0x8c61fa80u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61fa82; /* shll r13 */
L8c61fa82: sa_motion_publish_stop(ctx,0x8c61fa82u); r[4]+=r[6];goto L8c61fa84; /* add r6,r4 */
L8c61fa84: sa_motion_publish_stop(ctx,0x8c61fa84u); r[12]<<=2;goto L8c61fa86; /* shll2 r12 */
L8c61fa86: sa_motion_publish_stop(ctx,0x8c61fa86u); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61fa88; /* mov.l @(0xc,r4),r5 */
L8c61fa88: sa_motion_publish_stop(ctx,0x8c61fa88u); r[13]+=r[6];goto L8c61fa8a; /* add r6,r13 */
L8c61fa8a: sa_motion_publish_stop(ctx,0x8c61fa8au); r[14]<<=2;goto L8c61fa8c; /* shll2 r14 */
L8c61fa8c: sa_motion_publish_stop(ctx,0x8c61fa8cu); r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61fa8e; /* mov.l @(0xc,r13),r2 */
L8c61fa8e: sa_motion_publish_stop(ctx,0x8c61fa8eu); r[12]<<=2;goto L8c61fa90; /* shll2 r12 */
L8c61fa90: sa_motion_publish_stop(ctx,0x8c61fa90u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61fa92; /* shll r14 */
L8c61fa92: sa_motion_publish_stop(ctx,0x8c61fa92u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61fa94; /* shll r12 */
L8c61fa94: sa_motion_publish_stop(ctx,0x8c61fa94u); r[14]+=r[6];goto L8c61fa96; /* add r6,r14 */
L8c61fa96: sa_motion_publish_stop(ctx,0x8c61fa96u); r[1]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61fa98; /* mov.l @(0xc,r14),r1 */
L8c61fa98: sa_motion_publish_stop(ctx,0x8c61fa98u); r[5]&=r[2];goto L8c61fa9a; /* and r2,r5 */
L8c61fa9a: sa_motion_publish_stop(ctx,0x8c61fa9au); r[12]+=r[6];goto L8c61fa9c; /* add r6,r12 */
L8c61fa9c: sa_motion_publish_stop(ctx,0x8c61fa9cu); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61fa9e; /* mov.l @(0xc,r12),r2 */
L8c61fa9e: sa_motion_publish_stop(ctx,0x8c61fa9eu); r[5]&=r[1];goto L8c61faa0; /* and r1,r5 */
L8c61faa0: sa_motion_publish_stop(ctx,0x8c61faa0u); r[1]=63u;goto L8c61faa2; /* mov #0x3f,r1 */
L8c61faa2: sa_motion_publish_stop(ctx,0x8c61faa2u); r[5]&=r[2];goto L8c61faa4; /* and r2,r5 */
L8c61faa4: sa_motion_publish_stop(ctx,0x8c61faa4u); *sr=(*sr&~1u)|!(r[1]&r[5]);goto L8c61faa6; /* tst r1,r5 */
L8c61faa6: sa_motion_publish_stop(ctx,0x8c61faa6u); if((*sr&1u))goto L8c61faac;goto L8c61faa8; /* bt 0x8c61faac */
L8c61faa8: sa_motion_publish_stop(ctx,0x8c61faaau); goto L8c61fce8; /* bra 0x8c61fce8 */
L8c61faac: sa_motion_publish_stop(ctx,0x8c61faacu); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61faae; /* mov.l @(0xc,r4),r5 */
L8c61faae: sa_motion_publish_stop(ctx,0x8c61faaeu); r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61fab0; /* mov.l @(0xc,r13),r3 */
L8c61fab0: sa_motion_publish_stop(ctx,0x8c61fab0u); r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61fab2; /* mov.l @(0xc,r14),r2 */
L8c61fab2: sa_motion_publish_stop(ctx,0x8c61fab2u); r[5]|=r[3];goto L8c61fab4; /* or r3,r5 */
L8c61fab4: sa_motion_publish_stop(ctx,0x8c61fab4u); r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61fab6; /* mov.l @(0xc,r12),r3 */
L8c61fab6: sa_motion_publish_stop(ctx,0x8c61fab6u); r[5]|=r[2];goto L8c61fab8; /* or r2,r5 */
L8c61fab8: sa_motion_publish_stop(ctx,0x8c61fab8u); r[2]=1u;goto L8c61faba; /* mov #0x1,r2 */
L8c61faba: sa_motion_publish_stop(ctx,0x8c61fabau); r[5]|=r[3];goto L8c61fabc; /* or r3,r5 */
L8c61fabc: sa_motion_publish_stop(ctx,0x8c61fabcu); *sr=(*sr&~1u)|!(r[2]&r[5]);goto L8c61fabe; /* tst r2,r5 */
L8c61fabe: sa_motion_publish_stop(ctx,0x8c61fabeu); if((*sr&1u))goto L8c61fac4;goto L8c61fac0; /* bt 0x8c61fac4 */
L8c61fac0: sa_motion_publish_stop(ctx,0x8c61fac2u); goto L8c61fcb2; /* bra 0x8c61fcb2 */
L8c61fac4: sa_motion_publish_stop(ctx,0x8c61fac4u); r[0]=16u;goto L8c61fac6; /* mov #0x10,r0 */
L8c61fac6: sa_motion_publish_stop(ctx,0x8c61fac6u); fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fac8; /* fmov.s @(r0,r13),fr4 */
L8c61fac8: sa_motion_publish_stop(ctx,0x8c61fac8u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61faca; /* fmov.s @(r0,r4),fr3 */
L8c61faca: sa_motion_publish_stop(ctx,0x8c61facau); r[0]=20u;goto L8c61facc; /* mov #0x14,r0 */
L8c61facc: sa_motion_publish_stop(ctx,0x8c61faccu); fcopy(f,2u,4u);goto L8c61face; /* fmov fr4,fr2 */
L8c61face: sa_motion_publish_stop(ctx,0x8c61faceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61fad0; /* fsub fr3,fr2 */
L8c61fad0: sa_motion_publish_stop(ctx,0x8c61fad0u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fad2; /* fmov.s @(r0,r14),fr3 */
L8c61fad2: sa_motion_publish_stop(ctx,0x8c61fad2u); fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fad4; /* fmov.s @(r0,r13),fr5 */
L8c61fad4: sa_motion_publish_stop(ctx,0x8c61fad4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61fad6; /* fsub fr5,fr3 */
L8c61fad6: sa_motion_publish_stop(ctx,0x8c61fad6u); fcopy(f,1u,5u);goto L8c61fad8; /* fmov fr5,fr1 */
L8c61fad8: sa_motion_publish_stop(ctx,0x8c61fad8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fada; /* fmul fr3,fr2 */
L8c61fada: sa_motion_publish_stop(ctx,0x8c61fadau); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fadc; /* fmov.s @(r0,r4),fr3 */
L8c61fadc: sa_motion_publish_stop(ctx,0x8c61fadcu); r[0]=16u;goto L8c61fade; /* mov #0x10,r0 */
L8c61fade: sa_motion_publish_stop(ctx,0x8c61fadeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61fae0; /* fsub fr3,fr1 */
L8c61fae0: sa_motion_publish_stop(ctx,0x8c61fae0u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fae2; /* fmov.s @(r0,r14),fr3 */
L8c61fae2: sa_motion_publish_stop(ctx,0x8c61fae2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61fae4; /* fsub fr4,fr3 */
L8c61fae4: sa_motion_publish_stop(ctx,0x8c61fae4u); fcopy(f,4u,2u);goto L8c61fae6; /* fmov fr2,fr4 */
L8c61fae6: sa_motion_publish_stop(ctx,0x8c61fae6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61fae8; /* fmul fr3,fr1 */
L8c61fae8: sa_motion_publish_stop(ctx,0x8c61fae8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61faea; /* fldi0 fr3 */
L8c61faea: sa_motion_publish_stop(ctx,0x8c61faeau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61faec; /* fsub fr1,fr4 */
L8c61faec: sa_motion_publish_stop(ctx,0x8c61faecu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61faee; /* fcmp/gt fr3,fr4 */
L8c61faee: sa_motion_publish_stop(ctx,0x8c61faeeu); if(!(*sr&1u))goto L8c61faf4;goto L8c61faf0; /* bf 0x8c61faf4 */
L8c61faf0: sa_motion_publish_stop(ctx,0x8c61faf2u); goto L8c61fce8; /* bra 0x8c61fce8 */
L8c61faf4: sa_motion_publish_stop(ctx,0x8c61faf4u); r[2]=rd(m,bus,&io_status,0x8c61fd04u,4);if(io_status)return io_status;goto L8c61faf6; /* mov.l 0x8c61fd04,r2 */
L8c61faf6: sa_motion_publish_stop(ctx,0x8c61faf6u); r[0]=4u;goto L8c61faf8; /* mov #0x4,r0 */
L8c61faf8: sa_motion_publish_stop(ctx,0x8c61faf8u); r[8]+=28u;goto L8c61fafa; /* add #0x1c,r8 */
L8c61fafa: sa_motion_publish_stop(ctx,0x8c61fafau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61fafc; /* fmov.s fr14,@-r8 */
L8c61fafc: sa_motion_publish_stop(ctx,0x8c61fafcu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fafe; /* mov.l @r2,r3 */
L8c61fafe: sa_motion_publish_stop(ctx,0x8c61fafeu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fb00; /* fmov.s @(r0,r4),fr0 */
L8c61fb00: sa_motion_publish_stop(ctx,0x8c61fb00u); r[0]=20u;goto L8c61fb02; /* mov #0x14,r0 */
L8c61fb02: sa_motion_publish_stop(ctx,0x8c61fb02u); r[2]=r[3];goto L8c61fb04; /* mov r3,r2 */
L8c61fb04: sa_motion_publish_stop(ctx,0x8c61fb04u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61fb06; /* fmov.s @(r0,r2),fr3 */
L8c61fb06: sa_motion_publish_stop(ctx,0x8c61fb06u); r[1]=r[3];goto L8c61fb08; /* mov r3,r1 */
L8c61fb08: sa_motion_publish_stop(ctx,0x8c61fb08u); r[0]=4u;goto L8c61fb0a; /* mov #0x4,r0 */
L8c61fb0a: sa_motion_publish_stop(ctx,0x8c61fb0au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb0c; /* fmul fr0,fr3 */
L8c61fb0c: sa_motion_publish_stop(ctx,0x8c61fb0cu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61fb0e; /* fmov.s @(r0,r1),fr2 */
L8c61fb0e: sa_motion_publish_stop(ctx,0x8c61fb0eu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61fb10; /* fmov.s @r4,fr0 */
L8c61fb10: sa_motion_publish_stop(ctx,0x8c61fb10u); r[0]=8u;goto L8c61fb12; /* mov #0x8,r0 */
L8c61fb12: sa_motion_publish_stop(ctx,0x8c61fb12u); r[2]=rd(m,bus,&io_status,0x8c61fd04u,4);if(io_status)return io_status;goto L8c61fb14; /* mov.l 0x8c61fd04,r2 */
L8c61fb14: sa_motion_publish_stop(ctx,0x8c61fb14u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb16; /* fmac fr0,fr2,fr3 */
L8c61fb16: sa_motion_publish_stop(ctx,0x8c61fb16u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fb18; /* fmov.s @(r0,r4),fr0 */
L8c61fb18: sa_motion_publish_stop(ctx,0x8c61fb18u); r[0]=36u;goto L8c61fb1a; /* mov #0x24,r0 */
L8c61fb1a: sa_motion_publish_stop(ctx,0x8c61fb1au); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61fb1c; /* fmov.s @(r0,r3),fr2 */
L8c61fb1c: sa_motion_publish_stop(ctx,0x8c61fb1cu); r[0]=4u;goto L8c61fb1e; /* mov #0x4,r0 */
L8c61fb1e: sa_motion_publish_stop(ctx,0x8c61fb1eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb20; /* fmac fr0,fr2,fr3 */
L8c61fb20: sa_motion_publish_stop(ctx,0x8c61fb20u); fcopy(f,0u,15u);goto L8c61fb22; /* fmov fr15,fr0 */
L8c61fb22: sa_motion_publish_stop(ctx,0x8c61fb22u); fcopy(f,2u,15u);goto L8c61fb24; /* fmov fr15,fr2 */
L8c61fb24: sa_motion_publish_stop(ctx,0x8c61fb24u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fb26; /* fmac fr0,fr3,fr2 */
L8c61fb26: sa_motion_publish_stop(ctx,0x8c61fb26u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fb28; /* fmov.s fr2,@-r8 */
L8c61fb28: sa_motion_publish_stop(ctx,0x8c61fb28u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fb2a; /* mov.l @r2,r3 */
L8c61fb2a: sa_motion_publish_stop(ctx,0x8c61fb2au); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fb2c; /* fmov.s @(r0,r4),fr0 */
L8c61fb2c: sa_motion_publish_stop(ctx,0x8c61fb2cu); r[0]=16u;goto L8c61fb2e; /* mov #0x10,r0 */
L8c61fb2e: sa_motion_publish_stop(ctx,0x8c61fb2eu); r[2]=r[3];goto L8c61fb30; /* mov r3,r2 */
L8c61fb30: sa_motion_publish_stop(ctx,0x8c61fb30u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61fb32; /* fmov.s @(r0,r2),fr3 */
L8c61fb32: sa_motion_publish_stop(ctx,0x8c61fb32u); r[1]=r[3];goto L8c61fb34; /* mov r3,r1 */
L8c61fb34: sa_motion_publish_stop(ctx,0x8c61fb34u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb36; /* fmul fr0,fr3 */
L8c61fb36: sa_motion_publish_stop(ctx,0x8c61fb36u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61fb38; /* fmov.s @r1,fr2 */
L8c61fb38: sa_motion_publish_stop(ctx,0x8c61fb38u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61fb3a; /* fmov.s @r4,fr0 */
L8c61fb3a: sa_motion_publish_stop(ctx,0x8c61fb3au); r[0]=8u;goto L8c61fb3c; /* mov #0x8,r0 */
L8c61fb3c: sa_motion_publish_stop(ctx,0x8c61fb3cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb3e; /* fmac fr0,fr2,fr3 */
L8c61fb3e: sa_motion_publish_stop(ctx,0x8c61fb3eu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fb40; /* fmov.s @(r0,r4),fr0 */
L8c61fb40: sa_motion_publish_stop(ctx,0x8c61fb40u); r[0]=32u;goto L8c61fb42; /* mov #0x20,r0 */
L8c61fb42: sa_motion_publish_stop(ctx,0x8c61fb42u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61fb44; /* fmov.s @(r0,r3),fr2 */
L8c61fb44: sa_motion_publish_stop(ctx,0x8c61fb44u); r[0]=24u;goto L8c61fb46; /* mov #0x18,r0 */
L8c61fb46: sa_motion_publish_stop(ctx,0x8c61fb46u); r[3]=rd(m,bus,&io_status,0x8c61fd08u,4);if(io_status)return io_status;goto L8c61fb48; /* mov.l 0x8c61fd08,r3 */
L8c61fb48: sa_motion_publish_stop(ctx,0x8c61fb48u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb4a; /* fmac fr0,fr2,fr3 */
L8c61fb4a: sa_motion_publish_stop(ctx,0x8c61fb4au); fcopy(f,0u,15u);goto L8c61fb4c; /* fmov fr15,fr0 */
L8c61fb4c: sa_motion_publish_stop(ctx,0x8c61fb4cu); fcopy(f,2u,15u);goto L8c61fb4e; /* fmov fr15,fr2 */
L8c61fb4e: sa_motion_publish_stop(ctx,0x8c61fb4eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fb50; /* fmac fr0,fr3,fr2 */
L8c61fb50: sa_motion_publish_stop(ctx,0x8c61fb50u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fb52; /* fmov.s fr2,@-r8 */
L8c61fb52: sa_motion_publish_stop(ctx,0x8c61fb52u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fb54; /* fmov.s @(r0,r4),fr3 */
L8c61fb54: sa_motion_publish_stop(ctx,0x8c61fb54u); r[0]=20u;goto L8c61fb56; /* mov #0x14,r0 */
L8c61fb56: sa_motion_publish_stop(ctx,0x8c61fb56u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fb58; /* fmov.s fr3,@-r8 */
L8c61fb58: sa_motion_publish_stop(ctx,0x8c61fb58u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fb5a; /* fmov.s @(r0,r4),fr3 */
L8c61fb5a: sa_motion_publish_stop(ctx,0x8c61fb5au); r[0]=16u;goto L8c61fb5c; /* mov #0x10,r0 */
L8c61fb5c: sa_motion_publish_stop(ctx,0x8c61fb5cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fb5e; /* fmov.s fr3,@-r8 */
L8c61fb5e: sa_motion_publish_stop(ctx,0x8c61fb5eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fb60; /* fmov.s @(r0,r4),fr3 */
L8c61fb60: sa_motion_publish_stop(ctx,0x8c61fb60u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fb62; /* fmov.s fr3,@-r8 */
L8c61fb62: sa_motion_publish_stop(ctx,0x8c61fb62u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61fb64; /* mov.l r3,@-r8 */
L8c61fb64: sa_motion_publish_stop(ctx,0x8c61fb64u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61fb66; /* pref @r8 */
L8c61fb66: sa_motion_publish_stop(ctx,0x8c61fb66u); r[2]=rd(m,bus,&io_status,0x8c61fd04u,4);if(io_status)return io_status;goto L8c61fb68; /* mov.l 0x8c61fd04,r2 */
L8c61fb68: sa_motion_publish_stop(ctx,0x8c61fb68u); r[0]=4u;goto L8c61fb6a; /* mov #0x4,r0 */
L8c61fb6a: sa_motion_publish_stop(ctx,0x8c61fb6au); r[8]+=60u;goto L8c61fb6c; /* add #0x3c,r8 */
L8c61fb6c: sa_motion_publish_stop(ctx,0x8c61fb6cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61fb6e; /* fmov.s fr14,@-r8 */
L8c61fb6e: sa_motion_publish_stop(ctx,0x8c61fb6eu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fb70; /* mov.l @r2,r3 */
L8c61fb70: sa_motion_publish_stop(ctx,0x8c61fb70u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fb72; /* fmov.s @(r0,r13),fr0 */
L8c61fb72: sa_motion_publish_stop(ctx,0x8c61fb72u); r[0]=20u;goto L8c61fb74; /* mov #0x14,r0 */
L8c61fb74: sa_motion_publish_stop(ctx,0x8c61fb74u); r[2]=r[3];goto L8c61fb76; /* mov r3,r2 */
L8c61fb76: sa_motion_publish_stop(ctx,0x8c61fb76u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61fb78; /* fmov.s @(r0,r2),fr3 */
L8c61fb78: sa_motion_publish_stop(ctx,0x8c61fb78u); r[1]=r[3];goto L8c61fb7a; /* mov r3,r1 */
L8c61fb7a: sa_motion_publish_stop(ctx,0x8c61fb7au); r[0]=4u;goto L8c61fb7c; /* mov #0x4,r0 */
L8c61fb7c: sa_motion_publish_stop(ctx,0x8c61fb7cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb7e; /* fmul fr0,fr3 */
L8c61fb7e: sa_motion_publish_stop(ctx,0x8c61fb7eu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61fb80; /* fmov.s @(r0,r1),fr2 */
L8c61fb80: sa_motion_publish_stop(ctx,0x8c61fb80u); fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61fb82; /* fmov.s @r13,fr0 */
L8c61fb82: sa_motion_publish_stop(ctx,0x8c61fb82u); r[0]=8u;goto L8c61fb84; /* mov #0x8,r0 */
L8c61fb84: sa_motion_publish_stop(ctx,0x8c61fb84u); r[2]=rd(m,bus,&io_status,0x8c61fd04u,4);if(io_status)return io_status;goto L8c61fb86; /* mov.l 0x8c61fd04,r2 */
L8c61fb86: sa_motion_publish_stop(ctx,0x8c61fb86u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb88; /* fmac fr0,fr2,fr3 */
L8c61fb88: sa_motion_publish_stop(ctx,0x8c61fb88u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fb8a; /* fmov.s @(r0,r13),fr0 */
L8c61fb8a: sa_motion_publish_stop(ctx,0x8c61fb8au); r[0]=36u;goto L8c61fb8c; /* mov #0x24,r0 */
L8c61fb8c: sa_motion_publish_stop(ctx,0x8c61fb8cu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61fb8e; /* fmov.s @(r0,r3),fr2 */
L8c61fb8e: sa_motion_publish_stop(ctx,0x8c61fb8eu); r[0]=4u;goto L8c61fb90; /* mov #0x4,r0 */
L8c61fb90: sa_motion_publish_stop(ctx,0x8c61fb90u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb92; /* fmac fr0,fr2,fr3 */
L8c61fb92: sa_motion_publish_stop(ctx,0x8c61fb92u); fcopy(f,0u,15u);goto L8c61fb94; /* fmov fr15,fr0 */
L8c61fb94: sa_motion_publish_stop(ctx,0x8c61fb94u); fcopy(f,2u,15u);goto L8c61fb96; /* fmov fr15,fr2 */
L8c61fb96: sa_motion_publish_stop(ctx,0x8c61fb96u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fb98; /* fmac fr0,fr3,fr2 */
L8c61fb98: sa_motion_publish_stop(ctx,0x8c61fb98u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fb9a; /* fmov.s fr2,@-r8 */
L8c61fb9a: sa_motion_publish_stop(ctx,0x8c61fb9au); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fb9c; /* mov.l @r2,r3 */
L8c61fb9c: sa_motion_publish_stop(ctx,0x8c61fb9cu); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fb9e; /* fmov.s @(r0,r13),fr0 */
L8c61fb9e: sa_motion_publish_stop(ctx,0x8c61fb9eu); r[0]=16u;goto L8c61fba0; /* mov #0x10,r0 */
L8c61fba0: sa_motion_publish_stop(ctx,0x8c61fba0u); r[2]=r[3];goto L8c61fba2; /* mov r3,r2 */
L8c61fba2: sa_motion_publish_stop(ctx,0x8c61fba2u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61fba4; /* fmov.s @(r0,r2),fr3 */
L8c61fba4: sa_motion_publish_stop(ctx,0x8c61fba4u); r[1]=r[3];goto L8c61fba6; /* mov r3,r1 */
L8c61fba6: sa_motion_publish_stop(ctx,0x8c61fba6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fba8; /* fmul fr0,fr3 */
L8c61fba8: sa_motion_publish_stop(ctx,0x8c61fba8u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61fbaa; /* fmov.s @r1,fr2 */
L8c61fbaa: sa_motion_publish_stop(ctx,0x8c61fbaau); fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61fbac; /* fmov.s @r13,fr0 */
L8c61fbac: sa_motion_publish_stop(ctx,0x8c61fbacu); r[0]=8u;goto L8c61fbae; /* mov #0x8,r0 */
L8c61fbae: sa_motion_publish_stop(ctx,0x8c61fbaeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fbb0; /* fmac fr0,fr2,fr3 */
L8c61fbb0: sa_motion_publish_stop(ctx,0x8c61fbb0u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fbb2; /* fmov.s @(r0,r13),fr0 */
L8c61fbb2: sa_motion_publish_stop(ctx,0x8c61fbb2u); r[0]=32u;goto L8c61fbb4; /* mov #0x20,r0 */
L8c61fbb4: sa_motion_publish_stop(ctx,0x8c61fbb4u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61fbb6; /* fmov.s @(r0,r3),fr2 */
L8c61fbb6: sa_motion_publish_stop(ctx,0x8c61fbb6u); r[0]=24u;goto L8c61fbb8; /* mov #0x18,r0 */
L8c61fbb8: sa_motion_publish_stop(ctx,0x8c61fbb8u); r[3]=rd(m,bus,&io_status,0x8c61fd08u,4);if(io_status)return io_status;goto L8c61fbba; /* mov.l 0x8c61fd08,r3 */
L8c61fbba: sa_motion_publish_stop(ctx,0x8c61fbbau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fbbc; /* fmac fr0,fr2,fr3 */
L8c61fbbc: sa_motion_publish_stop(ctx,0x8c61fbbcu); fcopy(f,0u,15u);goto L8c61fbbe; /* fmov fr15,fr0 */
L8c61fbbe: sa_motion_publish_stop(ctx,0x8c61fbbeu); fcopy(f,2u,15u);goto L8c61fbc0; /* fmov fr15,fr2 */
L8c61fbc0: sa_motion_publish_stop(ctx,0x8c61fbc0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fbc2; /* fmac fr0,fr3,fr2 */
L8c61fbc2: sa_motion_publish_stop(ctx,0x8c61fbc2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fbc4; /* fmov.s fr2,@-r8 */
L8c61fbc4: sa_motion_publish_stop(ctx,0x8c61fbc4u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fbc6; /* fmov.s @(r0,r13),fr3 */
L8c61fbc6: sa_motion_publish_stop(ctx,0x8c61fbc6u); r[0]=20u;goto L8c61fbc8; /* mov #0x14,r0 */
L8c61fbc8: sa_motion_publish_stop(ctx,0x8c61fbc8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fbca; /* fmov.s fr3,@-r8 */
L8c61fbca: sa_motion_publish_stop(ctx,0x8c61fbcau); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fbcc; /* fmov.s @(r0,r13),fr3 */
L8c61fbcc: sa_motion_publish_stop(ctx,0x8c61fbccu); r[0]=16u;goto L8c61fbce; /* mov #0x10,r0 */
L8c61fbce: sa_motion_publish_stop(ctx,0x8c61fbceu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fbd0; /* fmov.s fr3,@-r8 */
L8c61fbd0: sa_motion_publish_stop(ctx,0x8c61fbd0u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fbd2; /* fmov.s @(r0,r13),fr3 */
L8c61fbd2: sa_motion_publish_stop(ctx,0x8c61fbd2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fbd4; /* fmov.s fr3,@-r8 */
L8c61fbd4: sa_motion_publish_stop(ctx,0x8c61fbd4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61fbd6; /* mov.l r3,@-r8 */
L8c61fbd6: sa_motion_publish_stop(ctx,0x8c61fbd6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61fbd8; /* pref @r8 */
L8c61fbd8: sa_motion_publish_stop(ctx,0x8c61fbd8u); r[3]=rd(m,bus,&io_status,0x8c61fd04u,4);if(io_status)return io_status;goto L8c61fbda; /* mov.l 0x8c61fd04,r3 */
L8c61fbda: sa_motion_publish_stop(ctx,0x8c61fbdau); r[0]=4u;goto L8c61fbdc; /* mov #0x4,r0 */
L8c61fbdc: sa_motion_publish_stop(ctx,0x8c61fbdcu); r[8]+=60u;goto L8c61fbde; /* add #0x3c,r8 */
L8c61fbde: sa_motion_publish_stop(ctx,0x8c61fbdeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61fbe0; /* fmov.s fr14,@-r8 */
L8c61fbe0: sa_motion_publish_stop(ctx,0x8c61fbe0u); r[9]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fbe2; /* mov.l @r3,r9 */
L8c61fbe2: sa_motion_publish_stop(ctx,0x8c61fbe2u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fbe4; /* fmov.s @(r0,r14),fr0 */
L8c61fbe4: sa_motion_publish_stop(ctx,0x8c61fbe4u); r[0]=20u;goto L8c61fbe6; /* mov #0x14,r0 */
L8c61fbe6: sa_motion_publish_stop(ctx,0x8c61fbe6u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61fbe8; /* fmov.s @(r0,r9),fr3 */
L8c61fbe8: sa_motion_publish_stop(ctx,0x8c61fbe8u); r[0]=4u;goto L8c61fbea; /* mov #0x4,r0 */
L8c61fbea: sa_motion_publish_stop(ctx,0x8c61fbeau); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61fbec; /* fmov.s @(r0,r9),fr2 */
L8c61fbec: sa_motion_publish_stop(ctx,0x8c61fbecu); r[0]=8u;goto L8c61fbee; /* mov #0x8,r0 */
L8c61fbee: sa_motion_publish_stop(ctx,0x8c61fbeeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fbf0; /* fmul fr0,fr3 */
L8c61fbf0: sa_motion_publish_stop(ctx,0x8c61fbf0u); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61fbf2; /* fmov.s @r14,fr0 */
L8c61fbf2: sa_motion_publish_stop(ctx,0x8c61fbf2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fbf4; /* fmac fr0,fr2,fr3 */
L8c61fbf4: sa_motion_publish_stop(ctx,0x8c61fbf4u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fbf6; /* fmov.s @(r0,r14),fr0 */
L8c61fbf6: sa_motion_publish_stop(ctx,0x8c61fbf6u); r[0]=36u;goto L8c61fbf8; /* mov #0x24,r0 */
L8c61fbf8: sa_motion_publish_stop(ctx,0x8c61fbf8u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61fbfa; /* fmov.s @(r0,r9),fr2 */
L8c61fbfa: sa_motion_publish_stop(ctx,0x8c61fbfau); r[0]=4u;goto L8c61fbfc; /* mov #0x4,r0 */
L8c61fbfc: sa_motion_publish_stop(ctx,0x8c61fbfcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fbfe; /* fmac fr0,fr2,fr3 */
L8c61fbfe: sa_motion_publish_stop(ctx,0x8c61fbfeu); fcopy(f,2u,15u);goto L8c61fc00; /* fmov fr15,fr2 */
L8c61fc00: sa_motion_publish_stop(ctx,0x8c61fc00u); fcopy(f,0u,15u);goto L8c61fc02; /* fmov fr15,fr0 */
L8c61fc02: sa_motion_publish_stop(ctx,0x8c61fc02u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fc04; /* fmac fr0,fr3,fr2 */
L8c61fc04: sa_motion_publish_stop(ctx,0x8c61fc04u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fc06; /* fmov.s fr2,@-r8 */
L8c61fc06: sa_motion_publish_stop(ctx,0x8c61fc06u); r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fc08; /* mov.l @r3,r7 */
L8c61fc08: sa_motion_publish_stop(ctx,0x8c61fc08u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fc0a; /* fmov.s @(r0,r14),fr0 */
L8c61fc0a: sa_motion_publish_stop(ctx,0x8c61fc0au); r[0]=16u;goto L8c61fc0c; /* mov #0x10,r0 */
L8c61fc0c: sa_motion_publish_stop(ctx,0x8c61fc0cu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61fc0e; /* fmov.s @(r0,r7),fr3 */
L8c61fc0e: sa_motion_publish_stop(ctx,0x8c61fc0eu); r[0]=8u;goto L8c61fc10; /* mov #0x8,r0 */
L8c61fc10: sa_motion_publish_stop(ctx,0x8c61fc10u); fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61fc12; /* fmov.s @r7,fr2 */
L8c61fc12: sa_motion_publish_stop(ctx,0x8c61fc12u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc14; /* fmul fr0,fr3 */
L8c61fc14: sa_motion_publish_stop(ctx,0x8c61fc14u); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61fc16; /* fmov.s @r14,fr0 */
L8c61fc16: sa_motion_publish_stop(ctx,0x8c61fc16u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc18; /* fmac fr0,fr2,fr3 */
L8c61fc18: sa_motion_publish_stop(ctx,0x8c61fc18u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fc1a; /* fmov.s @(r0,r14),fr0 */
L8c61fc1a: sa_motion_publish_stop(ctx,0x8c61fc1au); r[0]=32u;goto L8c61fc1c; /* mov #0x20,r0 */
L8c61fc1c: sa_motion_publish_stop(ctx,0x8c61fc1cu); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61fc1e; /* fmov.s @(r0,r7),fr2 */
L8c61fc1e: sa_motion_publish_stop(ctx,0x8c61fc1eu); r[0]=24u;goto L8c61fc20; /* mov #0x18,r0 */
L8c61fc20: sa_motion_publish_stop(ctx,0x8c61fc20u); r[2]=rd(m,bus,&io_status,0x8c61fd08u,4);if(io_status)return io_status;goto L8c61fc22; /* mov.l 0x8c61fd08,r2 */
L8c61fc22: sa_motion_publish_stop(ctx,0x8c61fc22u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc24; /* fmac fr0,fr2,fr3 */
L8c61fc24: sa_motion_publish_stop(ctx,0x8c61fc24u); fcopy(f,0u,15u);goto L8c61fc26; /* fmov fr15,fr0 */
L8c61fc26: sa_motion_publish_stop(ctx,0x8c61fc26u); fcopy(f,2u,15u);goto L8c61fc28; /* fmov fr15,fr2 */
L8c61fc28: sa_motion_publish_stop(ctx,0x8c61fc28u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fc2a; /* fmac fr0,fr3,fr2 */
L8c61fc2a: sa_motion_publish_stop(ctx,0x8c61fc2au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fc2c; /* fmov.s fr2,@-r8 */
L8c61fc2c: sa_motion_publish_stop(ctx,0x8c61fc2cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fc2e; /* fmov.s @(r0,r14),fr3 */
L8c61fc2e: sa_motion_publish_stop(ctx,0x8c61fc2eu); r[0]=20u;goto L8c61fc30; /* mov #0x14,r0 */
L8c61fc30: sa_motion_publish_stop(ctx,0x8c61fc30u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fc32; /* fmov.s fr3,@-r8 */
L8c61fc32: sa_motion_publish_stop(ctx,0x8c61fc32u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fc34; /* fmov.s @(r0,r14),fr3 */
L8c61fc34: sa_motion_publish_stop(ctx,0x8c61fc34u); r[0]=16u;goto L8c61fc36; /* mov #0x10,r0 */
L8c61fc36: sa_motion_publish_stop(ctx,0x8c61fc36u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fc38; /* fmov.s fr3,@-r8 */
L8c61fc38: sa_motion_publish_stop(ctx,0x8c61fc38u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fc3a; /* fmov.s @(r0,r14),fr3 */
L8c61fc3a: sa_motion_publish_stop(ctx,0x8c61fc3au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fc3c; /* fmov.s fr3,@-r8 */
L8c61fc3c: sa_motion_publish_stop(ctx,0x8c61fc3cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61fc3e; /* mov.l r2,@-r8 */
L8c61fc3e: sa_motion_publish_stop(ctx,0x8c61fc3eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61fc40; /* pref @r8 */
L8c61fc40: sa_motion_publish_stop(ctx,0x8c61fc40u); r[0]=4u;goto L8c61fc42; /* mov #0x4,r0 */
L8c61fc42: sa_motion_publish_stop(ctx,0x8c61fc42u); r[8]+=60u;goto L8c61fc44; /* add #0x3c,r8 */
L8c61fc44: sa_motion_publish_stop(ctx,0x8c61fc44u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61fc46; /* fmov.s fr14,@-r8 */
L8c61fc46: sa_motion_publish_stop(ctx,0x8c61fc46u); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61fc48; /* fmov.s @(r0,r12),fr0 */
L8c61fc48: sa_motion_publish_stop(ctx,0x8c61fc48u); r[0]=20u;goto L8c61fc4a; /* mov #0x14,r0 */
L8c61fc4a: sa_motion_publish_stop(ctx,0x8c61fc4au); r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fc4c; /* mov.l @r3,r6 */
L8c61fc4c: sa_motion_publish_stop(ctx,0x8c61fc4cu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fc4e; /* fmov.s @(r0,r6),fr3 */
L8c61fc4e: sa_motion_publish_stop(ctx,0x8c61fc4eu); r[0]=4u;goto L8c61fc50; /* mov #0x4,r0 */
L8c61fc50: sa_motion_publish_stop(ctx,0x8c61fc50u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fc52; /* fmov.s @(r0,r6),fr2 */
L8c61fc52: sa_motion_publish_stop(ctx,0x8c61fc52u); r[0]=8u;goto L8c61fc54; /* mov #0x8,r0 */
L8c61fc54: sa_motion_publish_stop(ctx,0x8c61fc54u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc56; /* fmul fr0,fr3 */
L8c61fc56: sa_motion_publish_stop(ctx,0x8c61fc56u); fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61fc58; /* fmov.s @r12,fr0 */
L8c61fc58: sa_motion_publish_stop(ctx,0x8c61fc58u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc5a; /* fmac fr0,fr2,fr3 */
L8c61fc5a: sa_motion_publish_stop(ctx,0x8c61fc5au); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61fc5c; /* fmov.s @(r0,r12),fr0 */
L8c61fc5c: sa_motion_publish_stop(ctx,0x8c61fc5cu); r[0]=36u;goto L8c61fc5e; /* mov #0x24,r0 */
L8c61fc5e: sa_motion_publish_stop(ctx,0x8c61fc5eu); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fc60; /* fmov.s @(r0,r6),fr2 */
L8c61fc60: sa_motion_publish_stop(ctx,0x8c61fc60u); r[0]=4u;goto L8c61fc62; /* mov #0x4,r0 */
L8c61fc62: sa_motion_publish_stop(ctx,0x8c61fc62u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc64; /* fmac fr0,fr2,fr3 */
L8c61fc64: sa_motion_publish_stop(ctx,0x8c61fc64u); fcopy(f,2u,15u);goto L8c61fc66; /* fmov fr15,fr2 */
L8c61fc66: sa_motion_publish_stop(ctx,0x8c61fc66u); fcopy(f,0u,15u);goto L8c61fc68; /* fmov fr15,fr0 */
L8c61fc68: sa_motion_publish_stop(ctx,0x8c61fc68u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fc6a; /* fmac fr0,fr3,fr2 */
L8c61fc6a: sa_motion_publish_stop(ctx,0x8c61fc6au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fc6c; /* fmov.s fr2,@-r8 */
L8c61fc6c: sa_motion_publish_stop(ctx,0x8c61fc6cu); r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fc6e; /* mov.l @r3,r5 */
L8c61fc6e: sa_motion_publish_stop(ctx,0x8c61fc6eu); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61fc70; /* fmov.s @(r0,r12),fr0 */
L8c61fc70: sa_motion_publish_stop(ctx,0x8c61fc70u); r[0]=16u;goto L8c61fc72; /* mov #0x10,r0 */
L8c61fc72: sa_motion_publish_stop(ctx,0x8c61fc72u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61fc74; /* fmov.s @(r0,r5),fr3 */
L8c61fc74: sa_motion_publish_stop(ctx,0x8c61fc74u); r[0]=8u;goto L8c61fc76; /* mov #0x8,r0 */
L8c61fc76: sa_motion_publish_stop(ctx,0x8c61fc76u); fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c61fc78; /* fmov.s @r5,fr2 */
L8c61fc78: sa_motion_publish_stop(ctx,0x8c61fc78u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc7a; /* fmul fr0,fr3 */
L8c61fc7a: sa_motion_publish_stop(ctx,0x8c61fc7au); fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61fc7c; /* fmov.s @r12,fr0 */
L8c61fc7c: sa_motion_publish_stop(ctx,0x8c61fc7cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc7e; /* fmac fr0,fr2,fr3 */
L8c61fc7e: sa_motion_publish_stop(ctx,0x8c61fc7eu); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61fc80; /* fmov.s @(r0,r12),fr0 */
L8c61fc80: sa_motion_publish_stop(ctx,0x8c61fc80u); r[0]=32u;goto L8c61fc82; /* mov #0x20,r0 */
L8c61fc82: sa_motion_publish_stop(ctx,0x8c61fc82u); r[2]=rd(m,bus,&io_status,0x8c61fd0cu,4);if(io_status)return io_status;goto L8c61fc84; /* mov.l 0x8c61fd0c,r2 */
L8c61fc84: sa_motion_publish_stop(ctx,0x8c61fc84u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61fc86; /* fmov.s @(r0,r5),fr2 */
L8c61fc86: sa_motion_publish_stop(ctx,0x8c61fc86u); r[0]=24u;goto L8c61fc88; /* mov #0x18,r0 */
L8c61fc88: sa_motion_publish_stop(ctx,0x8c61fc88u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc8a; /* fmac fr0,fr2,fr3 */
L8c61fc8a: sa_motion_publish_stop(ctx,0x8c61fc8au); fcopy(f,0u,15u);goto L8c61fc8c; /* fmov fr15,fr0 */
L8c61fc8c: sa_motion_publish_stop(ctx,0x8c61fc8cu); fcopy(f,2u,15u);goto L8c61fc8e; /* fmov fr15,fr2 */
L8c61fc8e: sa_motion_publish_stop(ctx,0x8c61fc8eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fc90; /* fmac fr0,fr3,fr2 */
L8c61fc90: sa_motion_publish_stop(ctx,0x8c61fc90u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fc92; /* fmov.s fr2,@-r8 */
L8c61fc92: sa_motion_publish_stop(ctx,0x8c61fc92u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61fc94; /* fmov.s @(r0,r12),fr3 */
L8c61fc94: sa_motion_publish_stop(ctx,0x8c61fc94u); r[0]=20u;goto L8c61fc96; /* mov #0x14,r0 */
L8c61fc96: sa_motion_publish_stop(ctx,0x8c61fc96u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fc98; /* fmov.s fr3,@-r8 */
L8c61fc98: sa_motion_publish_stop(ctx,0x8c61fc98u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61fc9a; /* fmov.s @(r0,r12),fr3 */
L8c61fc9a: sa_motion_publish_stop(ctx,0x8c61fc9au); r[0]=16u;goto L8c61fc9c; /* mov #0x10,r0 */
L8c61fc9c: sa_motion_publish_stop(ctx,0x8c61fc9cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fc9e; /* fmov.s fr3,@-r8 */
L8c61fc9e: sa_motion_publish_stop(ctx,0x8c61fc9eu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61fca0; /* fmov.s @(r0,r12),fr3 */
L8c61fca0: sa_motion_publish_stop(ctx,0x8c61fca0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fca2; /* fmov.s fr3,@-r8 */
L8c61fca2: sa_motion_publish_stop(ctx,0x8c61fca2u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61fca4; /* mov.l r2,@-r8 */
L8c61fca4: sa_motion_publish_stop(ctx,0x8c61fca4u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61fca6; /* pref @r8 */
L8c61fca6: sa_motion_publish_stop(ctx,0x8c61fca6u); r[1]=rd(m,bus,&io_status,0x8c61fd10u,4);if(io_status)return io_status;goto L8c61fca8; /* mov.l 0x8c61fd10,r1 */
L8c61fca8: sa_motion_publish_stop(ctx,0x8c61fca8u); r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61fcaa; /* mov.l @r1,r2 */
L8c61fcaa: sa_motion_publish_stop(ctx,0x8c61fcaau); r[2]+=4u;goto L8c61fcac; /* add #0x4,r2 */
L8c61fcac: sa_motion_publish_stop(ctx,0x8c61fcacu); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61fcae; /* mov.l r2,@r1 */
L8c61fcae: sa_motion_publish_stop(ctx,0x8c61fcb0u); r[8]+=32u;goto L8c61fce8; /* bra 0x8c61fce8 */
L8c61fcb2: sa_motion_publish_stop(ctx,0x8c61fcb2u); r[3]=rd(m,bus,&io_status,0x8c61fd14u,4);if(io_status)return io_status;goto L8c61fcb4; /* mov.l 0x8c61fd14,r3 */
L8c61fcb4: sa_motion_publish_stop(ctx,0x8c61fcb4u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61fcb6; /* mov.l r2,@r3 */
L8c61fcb6: sa_motion_publish_stop(ctx,0x8c61fcb6u); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61fcb8; /* mov.l @(0xc,r4),r5 */
L8c61fcb8: sa_motion_publish_stop(ctx,0x8c61fcb8u); r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61fcba; /* mov.l @(0xc,r13),r3 */
L8c61fcba: sa_motion_publish_stop(ctx,0x8c61fcbau); r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61fcbc; /* mov.l @(0xc,r14),r2 */
L8c61fcbc: sa_motion_publish_stop(ctx,0x8c61fcbcu); r[5]&=r[3];goto L8c61fcbe; /* and r3,r5 */
L8c61fcbe: sa_motion_publish_stop(ctx,0x8c61fcbeu); r[3]=1u;goto L8c61fcc0; /* mov #0x1,r3 */
L8c61fcc0: sa_motion_publish_stop(ctx,0x8c61fcc0u); r[5]&=r[2];goto L8c61fcc2; /* and r2,r5 */
L8c61fcc2: sa_motion_publish_stop(ctx,0x8c61fcc2u); *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61fcc4; /* tst r3,r5 */
L8c61fcc4: sa_motion_publish_stop(ctx,0x8c61fcc4u); if(!(*sr&1u))goto L8c61fcce;goto L8c61fcc6; /* bf 0x8c61fcce */
L8c61fcc6: sa_motion_publish_stop(ctx,0x8c61fcc6u); r[1]=rd(m,bus,&io_status,0x8c61fd18u,4);if(io_status)return io_status;goto L8c61fcc8; /* mov.l 0x8c61fd18,r1 */
L8c61fcc8: sa_motion_publish_stop(ctx,0x8c61fcc8u); r[6]=r[14];goto L8c61fcca; /* mov r14,r6 */
L8c61fcca: sa_motion_publish_stop(ctx,0x8c61fcccu); target=r[1];*pr=0x8c61fcceu;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61fcce; /* jsr @r1 */
L8c61fcce: sa_motion_publish_stop(ctx,0x8c61fcceu); r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61fcd0; /* mov.l @(0xc,r13),r4 */
L8c61fcd0: sa_motion_publish_stop(ctx,0x8c61fcd0u); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61fcd2; /* mov.l @(0xc,r14),r3 */
L8c61fcd2: sa_motion_publish_stop(ctx,0x8c61fcd2u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61fcd4; /* mov.l @(0xc,r12),r2 */
L8c61fcd4: sa_motion_publish_stop(ctx,0x8c61fcd4u); r[4]&=r[3];goto L8c61fcd6; /* and r3,r4 */
L8c61fcd6: sa_motion_publish_stop(ctx,0x8c61fcd6u); r[3]=1u;goto L8c61fcd8; /* mov #0x1,r3 */
L8c61fcd8: sa_motion_publish_stop(ctx,0x8c61fcd8u); r[4]&=r[2];goto L8c61fcda; /* and r2,r4 */
L8c61fcda: sa_motion_publish_stop(ctx,0x8c61fcdau); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61fcdc; /* tst r3,r4 */
L8c61fcdc: sa_motion_publish_stop(ctx,0x8c61fcdcu); if(!(*sr&1u))goto L8c61fce8;goto L8c61fcde; /* bf 0x8c61fce8 */
L8c61fcde: sa_motion_publish_stop(ctx,0x8c61fcdeu); r[1]=rd(m,bus,&io_status,0x8c61fd18u,4);if(io_status)return io_status;goto L8c61fce0; /* mov.l 0x8c61fd18,r1 */
L8c61fce0: sa_motion_publish_stop(ctx,0x8c61fce0u); r[5]=r[14];goto L8c61fce2; /* mov r14,r5 */
L8c61fce2: sa_motion_publish_stop(ctx,0x8c61fce2u); r[6]=r[12];goto L8c61fce4; /* mov r12,r6 */
L8c61fce4: sa_motion_publish_stop(ctx,0x8c61fce6u); target=r[1];*pr=0x8c61fce8u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61fce8; /* jsr @r1 */
L8c61fce8: sa_motion_publish_stop(ctx,0x8c61fce8u); r[10]+=1u;goto L8c61fcea; /* add #0x1,r10 */
L8c61fcea: sa_motion_publish_stop(ctx,0x8c61fceau); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61fcec; /* mov.l @r15,r3 */
L8c61fcec: sa_motion_publish_stop(ctx,0x8c61fcecu); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61fcee; /* mov.w @(0x2,r3),r0 */
L8c61fcee: sa_motion_publish_stop(ctx,0x8c61fceeu); r[0]=r[0]&65535u;goto L8c61fcf0; /* extu.w r0,r0 */
L8c61fcf0: sa_motion_publish_stop(ctx,0x8c61fcf0u); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c61fcf2; /* cmp/ge r0,r10 */
L8c61fcf2: sa_motion_publish_stop(ctx,0x8c61fcf2u); if((*sr&1u))goto L8c61fcf8;goto L8c61fcf4; /* bt 0x8c61fcf8 */
L8c61fcf4: sa_motion_publish_stop(ctx,0x8c61fcf6u); goto L8c61fa60; /* bra 0x8c61fa60 */
L8c61fcf8: sa_motion_publish_stop(ctx,0x8c61fcfau); goto L8c61ff9e; /* bra 0x8c61ff9e */
L8c61fcfc: sa_motion_publish_stop(ctx,0x8c61fcfeu); r[9]=0u;goto L8c61ff2c; /* bra 0x8c61ff2c */
L8c61fd1c: sa_motion_publish_stop(ctx,0x8c61fd1cu); r[3]=rd(m,bus,&io_status,0x8c61ffbcu,4);if(io_status)return io_status;goto L8c61fd1e; /* mov.l 0x8c61ffbc,r3 */
L8c61fd1e: sa_motion_publish_stop(ctx,0x8c61fd1eu); r[6]=0u;goto L8c61fd20; /* mov #0x0,r6 */
L8c61fd20: sa_motion_publish_stop(ctx,0x8c61fd20u); r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fd22; /* mov.w @r11+,r13 */
L8c61fd22: sa_motion_publish_stop(ctx,0x8c61fd22u); r[4]=0u;goto L8c61fd24; /* mov #0x0,r4 */
L8c61fd24: sa_motion_publish_stop(ctx,0x8c61fd24u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fd26; /* mov.l @r3,r2 */
L8c61fd26: sa_motion_publish_stop(ctx,0x8c61fd26u); r[5]=r[6];goto L8c61fd28; /* mov r6,r5 */
L8c61fd28: sa_motion_publish_stop(ctx,0x8c61fd28u); r[13]=r[13]&65535u;goto L8c61fd2a; /* extu.w r13,r13 */
L8c61fd2a: sa_motion_publish_stop(ctx,0x8c61fd2au); r[2]+=r[13];goto L8c61fd2c; /* add r13,r2 */
L8c61fd2c: sa_motion_publish_stop(ctx,0x8c61fd2cu); *sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c61fd2e; /* cmp/pl r13 */
L8c61fd2e: sa_motion_publish_stop(ctx,0x8c61fd2eu); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61fd30; /* mov.l r2,@r3 */
L8c61fd30: sa_motion_publish_stop(ctx,0x8c61fd30u); r[7]=4294967295u;goto L8c61fd32; /* mov #-0x1,r7 */
L8c61fd32: sa_motion_publish_stop(ctx,0x8c61fd34u); take=!(*sr&1u);r[4]+=r[11];if(take)goto L8c61fd54;goto L8c61fd36; /* bf/s 0x8c61fd54 */
L8c61fd36: sa_motion_publish_stop(ctx,0x8c61fd36u); r[14]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c61fd38; /* mov.w @r4+,r14 */
L8c61fd38: sa_motion_publish_stop(ctx,0x8c61fd38u); r[5]+=1u;goto L8c61fd3a; /* add #0x1,r5 */
L8c61fd3a: sa_motion_publish_stop(ctx,0x8c61fd3au); r[2]=rd(m,bus,&io_status,0x8c61ffc0u,4);if(io_status)return io_status;goto L8c61fd3c; /* mov.l 0x8c61ffc0,r2 */
L8c61fd3c: sa_motion_publish_stop(ctx,0x8c61fd3cu); r[14]=r[14]&65535u;goto L8c61fd3e; /* extu.w r14,r14 */
L8c61fd3e: sa_motion_publish_stop(ctx,0x8c61fd3eu); r[14]<<=2;goto L8c61fd40; /* shll2 r14 */
L8c61fd40: sa_motion_publish_stop(ctx,0x8c61fd40u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fd42; /* mov.l @r2,r3 */
L8c61fd42: sa_motion_publish_stop(ctx,0x8c61fd42u); r[14]<<=2;goto L8c61fd44; /* shll2 r14 */
L8c61fd44: sa_motion_publish_stop(ctx,0x8c61fd44u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61fd46; /* shll r14 */
L8c61fd46: sa_motion_publish_stop(ctx,0x8c61fd46u); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[13]);goto L8c61fd48; /* cmp/ge r13,r5 */
L8c61fd48: sa_motion_publish_stop(ctx,0x8c61fd48u); r[14]+=r[3];goto L8c61fd4a; /* add r3,r14 */
L8c61fd4a: sa_motion_publish_stop(ctx,0x8c61fd4au); wr(m,bus,&io_status,r[15]+0x4u,r[14],4);if(io_status)return io_status;goto L8c61fd4c; /* mov.l r14,@(0x4,r15) */
L8c61fd4c: sa_motion_publish_stop(ctx,0x8c61fd4cu); r[14]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61fd4e; /* mov.l @(0xc,r14),r14 */
L8c61fd4e: sa_motion_publish_stop(ctx,0x8c61fd4eu); r[7]&=r[14];goto L8c61fd50; /* and r14,r7 */
L8c61fd50: sa_motion_publish_stop(ctx,0x8c61fd52u); take=!(*sr&1u);r[6]|=r[14];if(take)goto L8c61fd36;goto L8c61fd54; /* bf/s 0x8c61fd36 */
L8c61fd54: sa_motion_publish_stop(ctx,0x8c61fd54u); r[2]=63u;goto L8c61fd56; /* mov #0x3f,r2 */
L8c61fd56: sa_motion_publish_stop(ctx,0x8c61fd56u); *sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c61fd58; /* tst r2,r7 */
L8c61fd58: sa_motion_publish_stop(ctx,0x8c61fd58u); if(!(*sr&1u))goto L8c61fdb8;goto L8c61fd5a; /* bf 0x8c61fdb8 */
L8c61fd5a: sa_motion_publish_stop(ctx,0x8c61fd5au); r[1]=1u;goto L8c61fd5c; /* mov #0x1,r1 */
L8c61fd5c: sa_motion_publish_stop(ctx,0x8c61fd5cu); *sr=(*sr&~1u)|!(r[1]&r[6]);goto L8c61fd5e; /* tst r1,r6 */
L8c61fd5e: sa_motion_publish_stop(ctx,0x8c61fd5eu); if((*sr&1u))goto L8c61fd64;goto L8c61fd60; /* bt 0x8c61fd64 */
L8c61fd60: sa_motion_publish_stop(ctx,0x8c61fd62u); goto L8c61fecc; /* bra 0x8c61fecc */
L8c61fd64: sa_motion_publish_stop(ctx,0x8c61fd64u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61fd66; /* mov.w @(0x2,r11),r0 */
L8c61fd66: sa_motion_publish_stop(ctx,0x8c61fd66u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61fd68; /* mov.w @r11,r4 */
L8c61fd68: sa_motion_publish_stop(ctx,0x8c61fd68u); r[3]=rd(m,bus,&io_status,0x8c61ffc0u,4);if(io_status)return io_status;goto L8c61fd6a; /* mov.l 0x8c61ffc0,r3 */
L8c61fd6a: sa_motion_publish_stop(ctx,0x8c61fd6au); r[5]=r[0]&65535u;goto L8c61fd6c; /* extu.w r0,r5 */
L8c61fd6c: sa_motion_publish_stop(ctx,0x8c61fd6cu); r[5]<<=2;goto L8c61fd6e; /* shll2 r5 */
L8c61fd6e: sa_motion_publish_stop(ctx,0x8c61fd6eu); r[0]=sx(m,bus,&io_status,r[11]+0x4u,2);if(io_status)return io_status;goto L8c61fd70; /* mov.w @(0x4,r11),r0 */
L8c61fd70: sa_motion_publish_stop(ctx,0x8c61fd70u); r[4]=r[4]&65535u;goto L8c61fd72; /* extu.w r4,r4 */
L8c61fd72: sa_motion_publish_stop(ctx,0x8c61fd72u); r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fd74; /* mov.l @r3,r7 */
L8c61fd74: sa_motion_publish_stop(ctx,0x8c61fd74u); r[4]<<=2;goto L8c61fd76; /* shll2 r4 */
L8c61fd76: sa_motion_publish_stop(ctx,0x8c61fd76u); r[5]<<=2;goto L8c61fd78; /* shll2 r5 */
L8c61fd78: sa_motion_publish_stop(ctx,0x8c61fd78u); r[6]=r[0]&65535u;goto L8c61fd7a; /* extu.w r0,r6 */
L8c61fd7a: sa_motion_publish_stop(ctx,0x8c61fd7au); r[4]<<=2;goto L8c61fd7c; /* shll2 r4 */
L8c61fd7c: sa_motion_publish_stop(ctx,0x8c61fd7cu); r[0]=16u;goto L8c61fd7e; /* mov #0x10,r0 */
L8c61fd7e: sa_motion_publish_stop(ctx,0x8c61fd7eu); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61fd80; /* shll r4 */
L8c61fd80: sa_motion_publish_stop(ctx,0x8c61fd80u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61fd82; /* shll r5 */
L8c61fd82: sa_motion_publish_stop(ctx,0x8c61fd82u); r[5]+=r[7];goto L8c61fd84; /* add r7,r5 */
L8c61fd84: sa_motion_publish_stop(ctx,0x8c61fd84u); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61fd86; /* fmov.s @(r0,r5),fr5 */
L8c61fd86: sa_motion_publish_stop(ctx,0x8c61fd86u); r[6]<<=2;goto L8c61fd88; /* shll2 r6 */
L8c61fd88: sa_motion_publish_stop(ctx,0x8c61fd88u); r[4]+=r[7];goto L8c61fd8a; /* add r7,r4 */
L8c61fd8a: sa_motion_publish_stop(ctx,0x8c61fd8au); r[6]<<=2;goto L8c61fd8c; /* shll2 r6 */
L8c61fd8c: sa_motion_publish_stop(ctx,0x8c61fd8cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fd8e; /* fmov.s @(r0,r4),fr3 */
L8c61fd8e: sa_motion_publish_stop(ctx,0x8c61fd8eu); fcopy(f,2u,5u);goto L8c61fd90; /* fmov fr5,fr2 */
L8c61fd90: sa_motion_publish_stop(ctx,0x8c61fd90u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61fd92; /* shll r6 */
L8c61fd92: sa_motion_publish_stop(ctx,0x8c61fd92u); r[0]=20u;goto L8c61fd94; /* mov #0x14,r0 */
L8c61fd94: sa_motion_publish_stop(ctx,0x8c61fd94u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61fd96; /* fsub fr3,fr2 */
L8c61fd96: sa_motion_publish_stop(ctx,0x8c61fd96u); r[6]+=r[7];goto L8c61fd98; /* add r7,r6 */
L8c61fd98: sa_motion_publish_stop(ctx,0x8c61fd98u); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61fd9a; /* fmov.s @(r0,r5),fr4 */
L8c61fd9a: sa_motion_publish_stop(ctx,0x8c61fd9au); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fd9c; /* fmov.s @(r0,r6),fr3 */
L8c61fd9c: sa_motion_publish_stop(ctx,0x8c61fd9cu); fcopy(f,1u,4u);goto L8c61fd9e; /* fmov fr4,fr1 */
L8c61fd9e: sa_motion_publish_stop(ctx,0x8c61fd9eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61fda0; /* fsub fr4,fr3 */
L8c61fda0: sa_motion_publish_stop(ctx,0x8c61fda0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fda2; /* fmul fr3,fr2 */
L8c61fda2: sa_motion_publish_stop(ctx,0x8c61fda2u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fda4; /* fmov.s @(r0,r4),fr3 */
L8c61fda4: sa_motion_publish_stop(ctx,0x8c61fda4u); r[0]=16u;goto L8c61fda6; /* mov #0x10,r0 */
L8c61fda6: sa_motion_publish_stop(ctx,0x8c61fda6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61fda8; /* fsub fr3,fr1 */
L8c61fda8: sa_motion_publish_stop(ctx,0x8c61fda8u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fdaa; /* fmov.s @(r0,r6),fr3 */
L8c61fdaa: sa_motion_publish_stop(ctx,0x8c61fdaau); fcopy(f,4u,2u);goto L8c61fdac; /* fmov fr2,fr4 */
L8c61fdac: sa_motion_publish_stop(ctx,0x8c61fdacu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61fdae; /* fsub fr5,fr3 */
L8c61fdae: sa_motion_publish_stop(ctx,0x8c61fdaeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61fdb0; /* fmul fr3,fr1 */
L8c61fdb0: sa_motion_publish_stop(ctx,0x8c61fdb0u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61fdb2; /* fldi0 fr3 */
L8c61fdb2: sa_motion_publish_stop(ctx,0x8c61fdb2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61fdb4; /* fsub fr1,fr4 */
L8c61fdb4: sa_motion_publish_stop(ctx,0x8c61fdb4u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61fdb6; /* fcmp/gt fr3,fr4 */
L8c61fdb6: sa_motion_publish_stop(ctx,0x8c61fdb6u); if(!(*sr&1u))goto L8c61fdbe;goto L8c61fdb8; /* bf 0x8c61fdbe */
L8c61fdb8: sa_motion_publish_stop(ctx,0x8c61fdb8u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61fdba; /* shll r13 */
L8c61fdba: sa_motion_publish_stop(ctx,0x8c61fdbcu); r[11]+=r[13];goto L8c61ff2a; /* bra 0x8c61ff2a */
L8c61fdbe: sa_motion_publish_stop(ctx,0x8c61fdbeu); r[2]=rd(m,bus,&io_status,0x8c61ffc4u,4);if(io_status)return io_status;goto L8c61fdc0; /* mov.l 0x8c61ffc4,r2 */
L8c61fdc0: sa_motion_publish_stop(ctx,0x8c61fdc0u); r[6]=r[13];goto L8c61fdc2; /* mov r13,r6 */
L8c61fdc2: sa_motion_publish_stop(ctx,0x8c61fdc2u); r[6]+=4294967295u;goto L8c61fdc4; /* add #-0x1,r6 */
L8c61fdc4: sa_motion_publish_stop(ctx,0x8c61fdc4u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fdc6; /* mov.l @r2,r3 */
L8c61fdc6: sa_motion_publish_stop(ctx,0x8c61fdc6u); *sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c61fdc8; /* cmp/pl r6 */
L8c61fdc8: sa_motion_publish_stop(ctx,0x8c61fdc8u); r[3]+=r[13];goto L8c61fdca; /* add r13,r3 */
L8c61fdca: sa_motion_publish_stop(ctx,0x8c61fdcau); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61fdcc; /* mov.l r3,@r2 */
L8c61fdcc: sa_motion_publish_stop(ctx,0x8c61fdceu); take=!(*sr&1u);r[5]=0u;if(take)goto L8c61fe50;goto L8c61fdd0; /* bf/s 0x8c61fe50 */
L8c61fdd0: sa_motion_publish_stop(ctx,0x8c61fdd0u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fdd2; /* mov.w @r11+,r4 */
L8c61fdd2: sa_motion_publish_stop(ctx,0x8c61fdd2u); r[0]=4u;goto L8c61fdd4; /* mov #0x4,r0 */
L8c61fdd4: sa_motion_publish_stop(ctx,0x8c61fdd4u); r[2]=rd(m,bus,&io_status,0x8c61ffc0u,4);if(io_status)return io_status;goto L8c61fdd6; /* mov.l 0x8c61ffc0,r2 */
L8c61fdd6: sa_motion_publish_stop(ctx,0x8c61fdd6u); r[8]+=28u;goto L8c61fdd8; /* add #0x1c,r8 */
L8c61fdd8: sa_motion_publish_stop(ctx,0x8c61fdd8u); r[4]=r[4]&65535u;goto L8c61fdda; /* extu.w r4,r4 */
L8c61fdda: sa_motion_publish_stop(ctx,0x8c61fddau); r[4]<<=2;goto L8c61fddc; /* shll2 r4 */
L8c61fddc: sa_motion_publish_stop(ctx,0x8c61fddcu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fdde; /* mov.l @r2,r3 */
L8c61fdde: sa_motion_publish_stop(ctx,0x8c61fddeu); r[4]<<=2;goto L8c61fde0; /* shll2 r4 */
L8c61fde0: sa_motion_publish_stop(ctx,0x8c61fde0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61fde2; /* fmov.s fr14,@-r8 */
L8c61fde2: sa_motion_publish_stop(ctx,0x8c61fde2u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61fde4; /* shll r4 */
L8c61fde4: sa_motion_publish_stop(ctx,0x8c61fde4u); r[4]+=r[3];goto L8c61fde6; /* add r3,r4 */
L8c61fde6: sa_motion_publish_stop(ctx,0x8c61fde6u); r[3]=rd(m,bus,&io_status,0x8c61ffc8u,4);if(io_status)return io_status;goto L8c61fde8; /* mov.l 0x8c61ffc8,r3 */
L8c61fde8: sa_motion_publish_stop(ctx,0x8c61fde8u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fdea; /* fmov.s @(r0,r4),fr0 */
L8c61fdea: sa_motion_publish_stop(ctx,0x8c61fdeau); r[0]=20u;goto L8c61fdec; /* mov #0x14,r0 */
L8c61fdec: sa_motion_publish_stop(ctx,0x8c61fdecu); r[14]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fdee; /* mov.l @r3,r14 */
L8c61fdee: sa_motion_publish_stop(ctx,0x8c61fdeeu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fdf0; /* fmov.s @(r0,r14),fr3 */
L8c61fdf0: sa_motion_publish_stop(ctx,0x8c61fdf0u); r[0]=4u;goto L8c61fdf2; /* mov #0x4,r0 */
L8c61fdf2: sa_motion_publish_stop(ctx,0x8c61fdf2u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fdf4; /* fmov.s @(r0,r14),fr2 */
L8c61fdf4: sa_motion_publish_stop(ctx,0x8c61fdf4u); r[0]=8u;goto L8c61fdf6; /* mov #0x8,r0 */
L8c61fdf6: sa_motion_publish_stop(ctx,0x8c61fdf6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fdf8; /* fmul fr0,fr3 */
L8c61fdf8: sa_motion_publish_stop(ctx,0x8c61fdf8u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61fdfa; /* fmov.s @r4,fr0 */
L8c61fdfa: sa_motion_publish_stop(ctx,0x8c61fdfau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fdfc; /* fmac fr0,fr2,fr3 */
L8c61fdfc: sa_motion_publish_stop(ctx,0x8c61fdfcu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fdfe; /* fmov.s @(r0,r4),fr0 */
L8c61fdfe: sa_motion_publish_stop(ctx,0x8c61fdfeu); r[0]=36u;goto L8c61fe00; /* mov #0x24,r0 */
L8c61fe00: sa_motion_publish_stop(ctx,0x8c61fe00u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fe02; /* fmov.s @(r0,r14),fr2 */
L8c61fe02: sa_motion_publish_stop(ctx,0x8c61fe02u); r[0]=4u;goto L8c61fe04; /* mov #0x4,r0 */
L8c61fe04: sa_motion_publish_stop(ctx,0x8c61fe04u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe06; /* fmac fr0,fr2,fr3 */
L8c61fe06: sa_motion_publish_stop(ctx,0x8c61fe06u); fcopy(f,2u,15u);goto L8c61fe08; /* fmov fr15,fr2 */
L8c61fe08: sa_motion_publish_stop(ctx,0x8c61fe08u); fcopy(f,0u,15u);goto L8c61fe0a; /* fmov fr15,fr0 */
L8c61fe0a: sa_motion_publish_stop(ctx,0x8c61fe0au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fe0c; /* fmac fr0,fr3,fr2 */
L8c61fe0c: sa_motion_publish_stop(ctx,0x8c61fe0cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fe0e; /* fmov.s fr2,@-r8 */
L8c61fe0e: sa_motion_publish_stop(ctx,0x8c61fe0eu); r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fe10; /* mov.l @r3,r7 */
L8c61fe10: sa_motion_publish_stop(ctx,0x8c61fe10u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe12; /* fmov.s @(r0,r4),fr0 */
L8c61fe12: sa_motion_publish_stop(ctx,0x8c61fe12u); r[0]=16u;goto L8c61fe14; /* mov #0x10,r0 */
L8c61fe14: sa_motion_publish_stop(ctx,0x8c61fe14u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61fe16; /* fmov.s @(r0,r7),fr3 */
L8c61fe16: sa_motion_publish_stop(ctx,0x8c61fe16u); r[0]=8u;goto L8c61fe18; /* mov #0x8,r0 */
L8c61fe18: sa_motion_publish_stop(ctx,0x8c61fe18u); fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61fe1a; /* fmov.s @r7,fr2 */
L8c61fe1a: sa_motion_publish_stop(ctx,0x8c61fe1au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe1c; /* fmul fr0,fr3 */
L8c61fe1c: sa_motion_publish_stop(ctx,0x8c61fe1cu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61fe1e; /* fmov.s @r4,fr0 */
L8c61fe1e: sa_motion_publish_stop(ctx,0x8c61fe1eu); r[1]=rd(m,bus,&io_status,0x8c61ffccu,4);if(io_status)return io_status;goto L8c61fe20; /* mov.l 0x8c61ffcc,r1 */
L8c61fe20: sa_motion_publish_stop(ctx,0x8c61fe20u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe22; /* fmac fr0,fr2,fr3 */
L8c61fe22: sa_motion_publish_stop(ctx,0x8c61fe22u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe24; /* fmov.s @(r0,r4),fr0 */
L8c61fe24: sa_motion_publish_stop(ctx,0x8c61fe24u); r[0]=32u;goto L8c61fe26; /* mov #0x20,r0 */
L8c61fe26: sa_motion_publish_stop(ctx,0x8c61fe26u); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61fe28; /* fmov.s @(r0,r7),fr2 */
L8c61fe28: sa_motion_publish_stop(ctx,0x8c61fe28u); r[0]=24u;goto L8c61fe2a; /* mov #0x18,r0 */
L8c61fe2a: sa_motion_publish_stop(ctx,0x8c61fe2au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe2c; /* fmac fr0,fr2,fr3 */
L8c61fe2c: sa_motion_publish_stop(ctx,0x8c61fe2cu); fcopy(f,0u,15u);goto L8c61fe2e; /* fmov fr15,fr0 */
L8c61fe2e: sa_motion_publish_stop(ctx,0x8c61fe2eu); fcopy(f,2u,15u);goto L8c61fe30; /* fmov fr15,fr2 */
L8c61fe30: sa_motion_publish_stop(ctx,0x8c61fe30u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fe32; /* fmac fr0,fr3,fr2 */
L8c61fe32: sa_motion_publish_stop(ctx,0x8c61fe32u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fe34; /* fmov.s fr2,@-r8 */
L8c61fe34: sa_motion_publish_stop(ctx,0x8c61fe34u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe36; /* fmov.s @(r0,r4),fr3 */
L8c61fe36: sa_motion_publish_stop(ctx,0x8c61fe36u); r[0]=20u;goto L8c61fe38; /* mov #0x14,r0 */
L8c61fe38: sa_motion_publish_stop(ctx,0x8c61fe38u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fe3a; /* fmov.s fr3,@-r8 */
L8c61fe3a: sa_motion_publish_stop(ctx,0x8c61fe3au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe3c; /* fmov.s @(r0,r4),fr3 */
L8c61fe3c: sa_motion_publish_stop(ctx,0x8c61fe3cu); r[0]=16u;goto L8c61fe3e; /* mov #0x10,r0 */
L8c61fe3e: sa_motion_publish_stop(ctx,0x8c61fe3eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fe40; /* fmov.s fr3,@-r8 */
L8c61fe40: sa_motion_publish_stop(ctx,0x8c61fe40u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe42; /* fmov.s @(r0,r4),fr3 */
L8c61fe42: sa_motion_publish_stop(ctx,0x8c61fe42u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fe44; /* fmov.s fr3,@-r8 */
L8c61fe44: sa_motion_publish_stop(ctx,0x8c61fe44u); r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c61fe46; /* mov.l r1,@-r8 */
L8c61fe46: sa_motion_publish_stop(ctx,0x8c61fe46u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61fe48; /* pref @r8 */
L8c61fe48: sa_motion_publish_stop(ctx,0x8c61fe48u); r[5]+=1u;goto L8c61fe4a; /* add #0x1,r5 */
L8c61fe4a: sa_motion_publish_stop(ctx,0x8c61fe4au); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61fe4c; /* cmp/ge r6,r5 */
L8c61fe4c: sa_motion_publish_stop(ctx,0x8c61fe4eu); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61fdd0;goto L8c61fe50; /* bf/s 0x8c61fdd0 */
L8c61fe50: sa_motion_publish_stop(ctx,0x8c61fe50u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fe52; /* mov.w @r11+,r4 */
L8c61fe52: sa_motion_publish_stop(ctx,0x8c61fe52u); r[0]=4u;goto L8c61fe54; /* mov #0x4,r0 */
L8c61fe54: sa_motion_publish_stop(ctx,0x8c61fe54u); r[2]=rd(m,bus,&io_status,0x8c61ffc0u,4);if(io_status)return io_status;goto L8c61fe56; /* mov.l 0x8c61ffc0,r2 */
L8c61fe56: sa_motion_publish_stop(ctx,0x8c61fe56u); r[8]+=28u;goto L8c61fe58; /* add #0x1c,r8 */
L8c61fe58: sa_motion_publish_stop(ctx,0x8c61fe58u); r[4]=r[4]&65535u;goto L8c61fe5a; /* extu.w r4,r4 */
L8c61fe5a: sa_motion_publish_stop(ctx,0x8c61fe5au); r[4]<<=2;goto L8c61fe5c; /* shll2 r4 */
L8c61fe5c: sa_motion_publish_stop(ctx,0x8c61fe5cu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fe5e; /* mov.l @r2,r3 */
L8c61fe5e: sa_motion_publish_stop(ctx,0x8c61fe5eu); r[4]<<=2;goto L8c61fe60; /* shll2 r4 */
L8c61fe60: sa_motion_publish_stop(ctx,0x8c61fe60u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61fe62; /* fmov.s fr14,@-r8 */
L8c61fe62: sa_motion_publish_stop(ctx,0x8c61fe62u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61fe64; /* shll r4 */
L8c61fe64: sa_motion_publish_stop(ctx,0x8c61fe64u); r[4]+=r[3];goto L8c61fe66; /* add r3,r4 */
L8c61fe66: sa_motion_publish_stop(ctx,0x8c61fe66u); r[3]=rd(m,bus,&io_status,0x8c61ffc8u,4);if(io_status)return io_status;goto L8c61fe68; /* mov.l 0x8c61ffc8,r3 */
L8c61fe68: sa_motion_publish_stop(ctx,0x8c61fe68u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe6a; /* fmov.s @(r0,r4),fr0 */
L8c61fe6a: sa_motion_publish_stop(ctx,0x8c61fe6au); r[0]=20u;goto L8c61fe6c; /* mov #0x14,r0 */
L8c61fe6c: sa_motion_publish_stop(ctx,0x8c61fe6cu); r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fe6e; /* mov.l @r3,r6 */
L8c61fe6e: sa_motion_publish_stop(ctx,0x8c61fe6eu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fe70; /* fmov.s @(r0,r6),fr3 */
L8c61fe70: sa_motion_publish_stop(ctx,0x8c61fe70u); r[0]=4u;goto L8c61fe72; /* mov #0x4,r0 */
L8c61fe72: sa_motion_publish_stop(ctx,0x8c61fe72u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fe74; /* fmov.s @(r0,r6),fr2 */
L8c61fe74: sa_motion_publish_stop(ctx,0x8c61fe74u); r[0]=8u;goto L8c61fe76; /* mov #0x8,r0 */
L8c61fe76: sa_motion_publish_stop(ctx,0x8c61fe76u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe78; /* fmul fr0,fr3 */
L8c61fe78: sa_motion_publish_stop(ctx,0x8c61fe78u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61fe7a; /* fmov.s @r4,fr0 */
L8c61fe7a: sa_motion_publish_stop(ctx,0x8c61fe7au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe7c; /* fmac fr0,fr2,fr3 */
L8c61fe7c: sa_motion_publish_stop(ctx,0x8c61fe7cu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe7e; /* fmov.s @(r0,r4),fr0 */
L8c61fe7e: sa_motion_publish_stop(ctx,0x8c61fe7eu); r[0]=36u;goto L8c61fe80; /* mov #0x24,r0 */
L8c61fe80: sa_motion_publish_stop(ctx,0x8c61fe80u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fe82; /* fmov.s @(r0,r6),fr2 */
L8c61fe82: sa_motion_publish_stop(ctx,0x8c61fe82u); r[0]=4u;goto L8c61fe84; /* mov #0x4,r0 */
L8c61fe84: sa_motion_publish_stop(ctx,0x8c61fe84u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe86; /* fmac fr0,fr2,fr3 */
L8c61fe86: sa_motion_publish_stop(ctx,0x8c61fe86u); fcopy(f,2u,15u);goto L8c61fe88; /* fmov fr15,fr2 */
L8c61fe88: sa_motion_publish_stop(ctx,0x8c61fe88u); fcopy(f,0u,15u);goto L8c61fe8a; /* fmov fr15,fr0 */
L8c61fe8a: sa_motion_publish_stop(ctx,0x8c61fe8au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fe8c; /* fmac fr0,fr3,fr2 */
L8c61fe8c: sa_motion_publish_stop(ctx,0x8c61fe8cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fe8e; /* fmov.s fr2,@-r8 */
L8c61fe8e: sa_motion_publish_stop(ctx,0x8c61fe8eu); r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fe90; /* mov.l @r3,r5 */
L8c61fe90: sa_motion_publish_stop(ctx,0x8c61fe90u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe92; /* fmov.s @(r0,r4),fr0 */
L8c61fe92: sa_motion_publish_stop(ctx,0x8c61fe92u); r[0]=16u;goto L8c61fe94; /* mov #0x10,r0 */
L8c61fe94: sa_motion_publish_stop(ctx,0x8c61fe94u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61fe96; /* fmov.s @(r0,r5),fr3 */
L8c61fe96: sa_motion_publish_stop(ctx,0x8c61fe96u); r[0]=8u;goto L8c61fe98; /* mov #0x8,r0 */
L8c61fe98: sa_motion_publish_stop(ctx,0x8c61fe98u); fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c61fe9a; /* fmov.s @r5,fr2 */
L8c61fe9a: sa_motion_publish_stop(ctx,0x8c61fe9au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe9c; /* fmul fr0,fr3 */
L8c61fe9c: sa_motion_publish_stop(ctx,0x8c61fe9cu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61fe9e; /* fmov.s @r4,fr0 */
L8c61fe9e: sa_motion_publish_stop(ctx,0x8c61fe9eu); r[1]=rd(m,bus,&io_status,0x8c61ffd0u,4);if(io_status)return io_status;goto L8c61fea0; /* mov.l 0x8c61ffd0,r1 */
L8c61fea0: sa_motion_publish_stop(ctx,0x8c61fea0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fea2; /* fmac fr0,fr2,fr3 */
L8c61fea2: sa_motion_publish_stop(ctx,0x8c61fea2u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fea4; /* fmov.s @(r0,r4),fr0 */
L8c61fea4: sa_motion_publish_stop(ctx,0x8c61fea4u); r[0]=32u;goto L8c61fea6; /* mov #0x20,r0 */
L8c61fea6: sa_motion_publish_stop(ctx,0x8c61fea6u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61fea8; /* fmov.s @(r0,r5),fr2 */
L8c61fea8: sa_motion_publish_stop(ctx,0x8c61fea8u); r[0]=24u;goto L8c61feaa; /* mov #0x18,r0 */
L8c61feaa: sa_motion_publish_stop(ctx,0x8c61feaau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61feac; /* fmac fr0,fr2,fr3 */
L8c61feac: sa_motion_publish_stop(ctx,0x8c61feacu); fcopy(f,0u,15u);goto L8c61feae; /* fmov fr15,fr0 */
L8c61feae: sa_motion_publish_stop(ctx,0x8c61feaeu); fcopy(f,2u,15u);goto L8c61feb0; /* fmov fr15,fr2 */
L8c61feb0: sa_motion_publish_stop(ctx,0x8c61feb0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61feb2; /* fmac fr0,fr3,fr2 */
L8c61feb2: sa_motion_publish_stop(ctx,0x8c61feb2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61feb4; /* fmov.s fr2,@-r8 */
L8c61feb4: sa_motion_publish_stop(ctx,0x8c61feb4u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61feb6; /* fmov.s @(r0,r4),fr3 */
L8c61feb6: sa_motion_publish_stop(ctx,0x8c61feb6u); r[0]=20u;goto L8c61feb8; /* mov #0x14,r0 */
L8c61feb8: sa_motion_publish_stop(ctx,0x8c61feb8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61feba; /* fmov.s fr3,@-r8 */
L8c61feba: sa_motion_publish_stop(ctx,0x8c61febau); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61febc; /* fmov.s @(r0,r4),fr3 */
L8c61febc: sa_motion_publish_stop(ctx,0x8c61febcu); r[0]=16u;goto L8c61febe; /* mov #0x10,r0 */
L8c61febe: sa_motion_publish_stop(ctx,0x8c61febeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fec0; /* fmov.s fr3,@-r8 */
L8c61fec0: sa_motion_publish_stop(ctx,0x8c61fec0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fec2; /* fmov.s @(r0,r4),fr3 */
L8c61fec2: sa_motion_publish_stop(ctx,0x8c61fec2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fec4; /* fmov.s fr3,@-r8 */
L8c61fec4: sa_motion_publish_stop(ctx,0x8c61fec4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c61fec6; /* mov.l r1,@-r8 */
L8c61fec6: sa_motion_publish_stop(ctx,0x8c61fec6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61fec8; /* pref @r8 */
L8c61fec8: sa_motion_publish_stop(ctx,0x8c61fecau); r[8]+=32u;goto L8c61ff2a; /* bra 0x8c61ff2a */
L8c61fecc: sa_motion_publish_stop(ctx,0x8c61feccu); r[2]=rd(m,bus,&io_status,0x8c61ffd4u,4);if(io_status)return io_status;goto L8c61fece; /* mov.l 0x8c61ffd4,r2 */
L8c61fece: sa_motion_publish_stop(ctx,0x8c61feceu); r[3]=0u;goto L8c61fed0; /* mov #0x0,r3 */
L8c61fed0: sa_motion_publish_stop(ctx,0x8c61fed0u); r[0]=rd(m,bus,&io_status,0x8c61ffc0u,4);if(io_status)return io_status;goto L8c61fed2; /* mov.l 0x8c61ffc0,r0 */
L8c61fed2: sa_motion_publish_stop(ctx,0x8c61fed2u); r[10]=r[13];goto L8c61fed4; /* mov r13,r10 */
L8c61fed4: sa_motion_publish_stop(ctx,0x8c61fed4u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61fed6; /* mov.l r3,@r2 */
L8c61fed6: sa_motion_publish_stop(ctx,0x8c61fed6u); r[10]+=4294967294u;goto L8c61fed8; /* add #-0x2,r10 */
L8c61fed8: sa_motion_publish_stop(ctx,0x8c61fed8u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61feda; /* mov.w @r11+,r4 */
L8c61feda: sa_motion_publish_stop(ctx,0x8c61fedau); r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61fedc; /* mov.l @r0,r3 */
L8c61fedc: sa_motion_publish_stop(ctx,0x8c61fedcu); r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fede; /* mov.w @r11+,r14 */
L8c61fede: sa_motion_publish_stop(ctx,0x8c61fedeu); r[4]=r[4]&65535u;goto L8c61fee0; /* extu.w r4,r4 */
L8c61fee0: sa_motion_publish_stop(ctx,0x8c61fee0u); r[4]<<=2;goto L8c61fee2; /* shll2 r4 */
L8c61fee2: sa_motion_publish_stop(ctx,0x8c61fee2u); r[14]=r[14]&65535u;goto L8c61fee4; /* extu.w r14,r14 */
L8c61fee4: sa_motion_publish_stop(ctx,0x8c61fee4u); r[14]<<=2;goto L8c61fee6; /* shll2 r14 */
L8c61fee6: sa_motion_publish_stop(ctx,0x8c61fee6u); r[14]<<=2;goto L8c61fee8; /* shll2 r14 */
L8c61fee8: sa_motion_publish_stop(ctx,0x8c61fee8u); r[4]<<=2;goto L8c61feea; /* shll2 r4 */
L8c61feea: sa_motion_publish_stop(ctx,0x8c61feeau); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61feec; /* shll r4 */
L8c61feec: sa_motion_publish_stop(ctx,0x8c61feecu); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61feee; /* shll r14 */
L8c61feee: sa_motion_publish_stop(ctx,0x8c61feeeu); *sr=(*sr&~1u)|((int32_t)r[10]>0);goto L8c61fef0; /* cmp/pl r10 */
L8c61fef0: sa_motion_publish_stop(ctx,0x8c61fef0u); r[4]+=r[3];goto L8c61fef2; /* add r3,r4 */
L8c61fef2: sa_motion_publish_stop(ctx,0x8c61fef2u); r[14]+=r[3];goto L8c61fef4; /* add r3,r14 */
L8c61fef4: sa_motion_publish_stop(ctx,0x8c61fef6u); take=!(*sr&1u);r[12]=0u;if(take)goto L8c61ff2a;goto L8c61fef8; /* bf/s 0x8c61ff2a */
L8c61fef8: sa_motion_publish_stop(ctx,0x8c61fef8u); r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fefa; /* mov.w @r11+,r13 */
L8c61fefa: sa_motion_publish_stop(ctx,0x8c61fefau); r[2]=rd(m,bus,&io_status,0x8c61ffc0u,4);if(io_status)return io_status;goto L8c61fefc; /* mov.l 0x8c61ffc0,r2 */
L8c61fefc: sa_motion_publish_stop(ctx,0x8c61fefcu); r[13]=r[13]&65535u;goto L8c61fefe; /* extu.w r13,r13 */
L8c61fefe: sa_motion_publish_stop(ctx,0x8c61fefeu); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61ff00; /* mov.l @(0xc,r4),r5 */
L8c61ff00: sa_motion_publish_stop(ctx,0x8c61ff00u); r[13]<<=2;goto L8c61ff02; /* shll2 r13 */
L8c61ff02: sa_motion_publish_stop(ctx,0x8c61ff02u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ff04; /* mov.l @r2,r3 */
L8c61ff04: sa_motion_publish_stop(ctx,0x8c61ff04u); r[13]<<=2;goto L8c61ff06; /* shll2 r13 */
L8c61ff06: sa_motion_publish_stop(ctx,0x8c61ff06u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61ff08; /* shll r13 */
L8c61ff08: sa_motion_publish_stop(ctx,0x8c61ff08u); r[13]+=r[3];goto L8c61ff0a; /* add r3,r13 */
L8c61ff0a: sa_motion_publish_stop(ctx,0x8c61ff0au); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61ff0c; /* mov.l @(0xc,r14),r3 */
L8c61ff0c: sa_motion_publish_stop(ctx,0x8c61ff0cu); r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61ff0e; /* mov.l @(0xc,r13),r1 */
L8c61ff0e: sa_motion_publish_stop(ctx,0x8c61ff0eu); r[5]&=r[3];goto L8c61ff10; /* and r3,r5 */
L8c61ff10: sa_motion_publish_stop(ctx,0x8c61ff10u); r[3]=1u;goto L8c61ff12; /* mov #0x1,r3 */
L8c61ff12: sa_motion_publish_stop(ctx,0x8c61ff12u); r[5]&=r[1];goto L8c61ff14; /* and r1,r5 */
L8c61ff14: sa_motion_publish_stop(ctx,0x8c61ff14u); *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61ff16; /* tst r3,r5 */
L8c61ff16: sa_motion_publish_stop(ctx,0x8c61ff16u); if(!(*sr&1u))goto L8c61ff20;goto L8c61ff18; /* bf 0x8c61ff20 */
L8c61ff18: sa_motion_publish_stop(ctx,0x8c61ff18u); r[1]=rd(m,bus,&io_status,0x8c61ffd8u,4);if(io_status)return io_status;goto L8c61ff1a; /* mov.l 0x8c61ffd8,r1 */
L8c61ff1a: sa_motion_publish_stop(ctx,0x8c61ff1au); r[6]=r[13];goto L8c61ff1c; /* mov r13,r6 */
L8c61ff1c: sa_motion_publish_stop(ctx,0x8c61ff1eu); target=r[1];*pr=0x8c61ff20u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ff20; /* jsr @r1 */
L8c61ff20: sa_motion_publish_stop(ctx,0x8c61ff20u); r[12]+=1u;goto L8c61ff22; /* add #0x1,r12 */
L8c61ff22: sa_motion_publish_stop(ctx,0x8c61ff22u); r[4]=r[14];goto L8c61ff24; /* mov r14,r4 */
L8c61ff24: sa_motion_publish_stop(ctx,0x8c61ff24u); *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[10]);goto L8c61ff26; /* cmp/ge r10,r12 */
L8c61ff26: sa_motion_publish_stop(ctx,0x8c61ff28u); take=!(*sr&1u);r[14]=r[13];if(take)goto L8c61fef8;goto L8c61ff2a; /* bf/s 0x8c61fef8 */
L8c61ff2a: sa_motion_publish_stop(ctx,0x8c61ff2au); r[9]+=1u;goto L8c61ff2c; /* add #0x1,r9 */
L8c61ff2c: sa_motion_publish_stop(ctx,0x8c61ff2cu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ff2e; /* mov.l @r15,r2 */
L8c61ff2e: sa_motion_publish_stop(ctx,0x8c61ff2eu); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61ff30; /* mov.w @(0x2,r2),r0 */
L8c61ff30: sa_motion_publish_stop(ctx,0x8c61ff30u); r[0]=r[0]&65535u;goto L8c61ff32; /* extu.w r0,r0 */
L8c61ff32: sa_motion_publish_stop(ctx,0x8c61ff32u); *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61ff34; /* cmp/ge r0,r9 */
L8c61ff34: sa_motion_publish_stop(ctx,0x8c61ff34u); if((*sr&1u))goto L8c61ff3a;goto L8c61ff36; /* bt 0x8c61ff3a */
L8c61ff36: sa_motion_publish_stop(ctx,0x8c61ff38u); goto L8c61fd1c; /* bra 0x8c61fd1c */
L8c61ff3a: sa_motion_publish_stop(ctx,0x8c61ff3cu); goto L8c61ff9e; /* bra 0x8c61ff9e */
L8c61ff3e: sa_motion_publish_stop(ctx,0x8c61ff3eu); r[2]=rd(m,bus,&io_status,0x8c61ffdcu,4);if(io_status)return io_status;goto L8c61ff40; /* mov.l 0x8c61ffdc,r2 */
L8c61ff40: sa_motion_publish_stop(ctx,0x8c61ff40u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ff42; /* mov.l @r15,r1 */
L8c61ff42: sa_motion_publish_stop(ctx,0x8c61ff42u); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61ff44; /* mov.l r1,@r2 */
L8c61ff44: sa_motion_publish_stop(ctx,0x8c61ff44u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61ff46; /* frchg */
L8c61ff46: sa_motion_publish_stop(ctx,0x8c61ff46u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61ff48; /* fmov.s fr0,@-r15 */
L8c61ff48: sa_motion_publish_stop(ctx,0x8c61ff48u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61ff4a; /* fmov.s fr1,@-r15 */
L8c61ff4a: sa_motion_publish_stop(ctx,0x8c61ff4au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61ff4c; /* fmov.s fr2,@-r15 */
L8c61ff4c: sa_motion_publish_stop(ctx,0x8c61ff4cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61ff4e; /* fmov.s fr3,@-r15 */
L8c61ff4e: sa_motion_publish_stop(ctx,0x8c61ff4eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61ff50; /* fmov.s fr4,@-r15 */
L8c61ff50: sa_motion_publish_stop(ctx,0x8c61ff50u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61ff52; /* fmov.s fr5,@-r15 */
L8c61ff52: sa_motion_publish_stop(ctx,0x8c61ff52u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61ff54; /* fmov.s fr6,@-r15 */
L8c61ff54: sa_motion_publish_stop(ctx,0x8c61ff54u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61ff56; /* fmov.s fr7,@-r15 */
L8c61ff56: sa_motion_publish_stop(ctx,0x8c61ff56u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61ff58; /* fmov.s fr8,@-r15 */
L8c61ff58: sa_motion_publish_stop(ctx,0x8c61ff58u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61ff5a; /* fmov.s fr9,@-r15 */
L8c61ff5a: sa_motion_publish_stop(ctx,0x8c61ff5au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c61ff5c; /* fmov.s fr10,@-r15 */
L8c61ff5c: sa_motion_publish_stop(ctx,0x8c61ff5cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c61ff5e; /* fmov.s fr11,@-r15 */
L8c61ff5e: sa_motion_publish_stop(ctx,0x8c61ff5eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c61ff60; /* fmov.s fr12,@-r15 */
L8c61ff60: sa_motion_publish_stop(ctx,0x8c61ff60u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61ff62; /* fmov.s fr13,@-r15 */
L8c61ff62: sa_motion_publish_stop(ctx,0x8c61ff62u); r[0]=0x8c61ffb4u;goto L8c61ff64; /* mova 0x8c61ffb4,r0 */
L8c61ff64: sa_motion_publish_stop(ctx,0x8c61ff64u); fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c61ff66; /* fmov.s @r0,fr12 */
L8c61ff66: sa_motion_publish_stop(ctx,0x8c61ff66u); fcopy(f,13u,12u);goto L8c61ff68; /* fmov fr12,fr13 */
L8c61ff68: sa_motion_publish_stop(ctx,0x8c61ff68u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[12],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61ff6a; /* fmul fr12,fr0 */
L8c61ff6a: sa_motion_publish_stop(ctx,0x8c61ff6au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61ff6c; /* fmul fr12,fr4 */
L8c61ff6c: sa_motion_publish_stop(ctx,0x8c61ff6cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[12],&f->fpscr,&fr[8]);if(status)return status;}goto L8c61ff6e; /* fmul fr12,fr8 */
L8c61ff6e: sa_motion_publish_stop(ctx,0x8c61ff6eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[12],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ff70; /* fmul fr12,fr1 */
L8c61ff70: sa_motion_publish_stop(ctx,0x8c61ff70u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[12],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61ff72; /* fmul fr12,fr5 */
L8c61ff72: sa_motion_publish_stop(ctx,0x8c61ff72u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[12],&f->fpscr,&fr[9]);if(status)return status;}goto L8c61ff74; /* fmul fr12,fr9 */
L8c61ff74: sa_motion_publish_stop(ctx,0x8c61ff74u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61ff76; /* frchg */
L8c61ff76: sa_motion_publish_stop(ctx,0x8c61ff76u); r[2]=rd(m,bus,&io_status,0x8c61ffe0u,4);if(io_status)return io_status;goto L8c61ff78; /* mov.l 0x8c61ffe0,r2 */
L8c61ff78: sa_motion_publish_stop(ctx,0x8c61ff78u); r[3]=rd(m,bus,&io_status,0x8c61ffdcu,4);if(io_status)return io_status;goto L8c61ff7a; /* mov.l 0x8c61ffdc,r3 */
L8c61ff7a: sa_motion_publish_stop(ctx,0x8c61ff7cu); target=r[2];*pr=0x8c61ff7eu;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ff7e; /* jsr @r2 */
L8c61ff7e: sa_motion_publish_stop(ctx,0x8c61ff7eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61ff80; /* frchg */
L8c61ff80: sa_motion_publish_stop(ctx,0x8c61ff80u); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff82; /* fmov.s @r15+,fr13 */
L8c61ff82: sa_motion_publish_stop(ctx,0x8c61ff82u); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff84; /* fmov.s @r15+,fr12 */
L8c61ff84: sa_motion_publish_stop(ctx,0x8c61ff84u); fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff86; /* fmov.s @r15+,fr11 */
L8c61ff86: sa_motion_publish_stop(ctx,0x8c61ff86u); fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff88; /* fmov.s @r15+,fr10 */
L8c61ff88: sa_motion_publish_stop(ctx,0x8c61ff88u); fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff8a; /* fmov.s @r15+,fr9 */
L8c61ff8a: sa_motion_publish_stop(ctx,0x8c61ff8au); fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff8c; /* fmov.s @r15+,fr8 */
L8c61ff8c: sa_motion_publish_stop(ctx,0x8c61ff8cu); fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff8e; /* fmov.s @r15+,fr7 */
L8c61ff8e: sa_motion_publish_stop(ctx,0x8c61ff8eu); fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff90; /* fmov.s @r15+,fr6 */
L8c61ff90: sa_motion_publish_stop(ctx,0x8c61ff90u); fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff92; /* fmov.s @r15+,fr5 */
L8c61ff92: sa_motion_publish_stop(ctx,0x8c61ff92u); fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff94; /* fmov.s @r15+,fr4 */
L8c61ff94: sa_motion_publish_stop(ctx,0x8c61ff94u); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff96; /* fmov.s @r15+,fr3 */
L8c61ff96: sa_motion_publish_stop(ctx,0x8c61ff96u); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff98; /* fmov.s @r15+,fr2 */
L8c61ff98: sa_motion_publish_stop(ctx,0x8c61ff98u); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff9a; /* fmov.s @r15+,fr1 */
L8c61ff9a: sa_motion_publish_stop(ctx,0x8c61ff9au); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff9c; /* fmov.s @r15+,fr0 */
L8c61ff9c: sa_motion_publish_stop(ctx,0x8c61ff9cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61ff9e; /* frchg */
L8c61ff9e: sa_motion_publish_stop(ctx,0x8c61ff9eu); r[15]+=8u;goto L8c61ffa0; /* add #0x8,r15 */
L8c61ffa0: sa_motion_publish_stop(ctx,0x8c61ffa0u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61ffa2; /* lds.l @r15+,PR */
L8c61ffa2: sa_motion_publish_stop(ctx,0x8c61ffa2u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ffa4; /* fmov.s @r15+,fr14 */
L8c61ffa4: sa_motion_publish_stop(ctx,0x8c61ffa4u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ffa6; /* fmov.s @r15+,fr15 */
L8c61ffa6: sa_motion_publish_stop(ctx,0x8c61ffa6u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61ffa8; /* mov.l @r15+,r9 */
L8c61ffa8: sa_motion_publish_stop(ctx,0x8c61ffa8u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61ffaa; /* mov.l @r15+,r10 */
L8c61ffaa: sa_motion_publish_stop(ctx,0x8c61ffaau); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61ffac; /* mov.l @r15+,r11 */
L8c61ffac: sa_motion_publish_stop(ctx,0x8c61ffacu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61ffae; /* mov.l @r15+,r12 */
L8c61ffae: sa_motion_publish_stop(ctx,0x8c61ffaeu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61ffb0; /* mov.l @r15+,r13 */
L8c61ffb0: sa_motion_publish_stop(ctx,0x8c61ffb2u); if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
