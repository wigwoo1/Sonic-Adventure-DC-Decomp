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
L8c61f108: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61f10a; /* mov.l r14,@-r15 */
L8c61f10a: r[0]=0x8c61f16cu;goto L8c61f10c; /* mova 0x8c61f16c,r0 */
L8c61f10c: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61f10e; /* mov.l r13,@-r15 */
L8c61f10e: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61f110; /* mov.l r12,@-r15 */
L8c61f110: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61f112; /* mov.l r11,@-r15 */
L8c61f112: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61f114; /* mov.l r10,@-r15 */
L8c61f114: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61f116; /* mov.l r9,@-r15 */
L8c61f116: r[3]=rd(m,bus,&io_status,0x8c61f178u,4);if(io_status)return io_status;goto L8c61f118; /* mov.l 0x8c61f178,r3 */
L8c61f118: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61f11a; /* sts.l PR,@-r15 */
L8c61f11a: fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c61f11c; /* fmov.s @r0,fr6 */
L8c61f11c: r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61f11e; /* mov.w @r4,r0 */
L8c61f11e: r[11]=rd(m,bus,&io_status,0x8c61f174u,4);if(io_status)return io_status;goto L8c61f120; /* mov.l 0x8c61f174,r11 */
L8c61f120: r[0]=r[0]&65535u;goto L8c61f122; /* extu.w r0,r0 */
L8c61f122: r[13]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c61f124; /* mov.l @(0x4,r4),r13 */
L8c61f124: r[0]&=r[3];goto L8c61f126; /* and r3,r0 */
L8c61f126: r[9]=rd(m,bus,&io_status,0x8c61f170u,4);if(io_status)return io_status;goto L8c61f128; /* mov.l 0x8c61f170,r9 */
L8c61f128: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61f12a; /* cmp/eq #0x0,r0 */
L8c61f12a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c61f12c; /* fldi1 fr7 */
L8c61f12c: take=(*sr&1u);r[12]=0u;if(take)goto L8c61f152;goto L8c61f130; /* bt/s 0x8c61f152 */
L8c61f130: r[1]=sx(m,bus,&io_status,0x8c61f16au,2);if(io_status)return io_status;goto L8c61f132; /* mov.w 0x8c61f16a,r1 */
L8c61f132: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61f134; /* cmp/eq r1,r0 */
L8c61f134: if(!(*sr&1u))goto L8c61f13a;goto L8c61f136; /* bf 0x8c61f13a */
L8c61f136: goto L8c61f33e; /* bra 0x8c61f33e */
L8c61f13a: r[1]=rd(m,bus,&io_status,0x8c61f17cu,4);if(io_status)return io_status;goto L8c61f13c; /* mov.l 0x8c61f17c,r1 */
L8c61f13c: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61f13e; /* cmp/eq r1,r0 */
L8c61f13e: if(!(*sr&1u))goto L8c61f144;goto L8c61f140; /* bf 0x8c61f144 */
L8c61f140: goto L8c61f59c; /* bra 0x8c61f59c */
L8c61f144: r[1]=rd(m,bus,&io_status,0x8c61f178u,4);if(io_status)return io_status;goto L8c61f146; /* mov.l 0x8c61f178,r1 */
L8c61f146: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61f148; /* cmp/eq r1,r0 */
L8c61f148: if(!(*sr&1u))goto L8c61f14e;goto L8c61f14a; /* bf 0x8c61f14e */
L8c61f14a: goto L8c61f73e; /* bra 0x8c61f73e */
L8c61f14e: goto L8c61f79c; /* bra 0x8c61f79c */
L8c61f152: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61f154; /* mov.w @(0x2,r4),r0 */
L8c61f154: r[1]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c61f156; /* mov.l @r11,r1 */
L8c61f156: r[0]=r[0]&65535u;goto L8c61f158; /* extu.w r0,r0 */
L8c61f158: r[2]=r[0];goto L8c61f15a; /* mov r0,r2 */
L8c61f15a: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61f15c; /* shll r0 */
L8c61f15c: r[0]+=r[2];goto L8c61f15e; /* add r2,r0 */
L8c61f15e: r[1]+=r[0];goto L8c61f160; /* add r0,r1 */
L8c61f160: wr(m,bus,&io_status,r[11],r[1],4);if(io_status)return io_status;goto L8c61f162; /* mov.l r1,@r11 */
L8c61f162: r[14]=r[12];goto L8c61f32e; /* bra 0x8c61f32e */
L8c61f180: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f182; /* mov.w @r13+,r5 */
L8c61f182: r[0]=16u;goto L8c61f184; /* mov #0x10,r0 */
L8c61f184: r[3]=rd(m,bus,&io_status,0x8c61f354u,4);if(io_status)return io_status;goto L8c61f186; /* mov.l 0x8c61f354,r3 */
L8c61f186: r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f188; /* mov.w @r13+,r6 */
L8c61f188: r[5]=r[5]&65535u;goto L8c61f18a; /* extu.w r5,r5 */
L8c61f18a: r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f18c; /* mov.l @r3,r12 */
L8c61f18c: r[5]<<=2;goto L8c61f18e; /* shll2 r5 */
L8c61f18e: r[6]=r[6]&65535u;goto L8c61f190; /* extu.w r6,r6 */
L8c61f190: r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f192; /* mov.w @r13+,r7 */
L8c61f192: r[6]<<=2;goto L8c61f194; /* shll2 r6 */
L8c61f194: r[6]<<=2;goto L8c61f196; /* shll2 r6 */
L8c61f196: r[5]<<=2;goto L8c61f198; /* shll2 r5 */
L8c61f198: r[7]=r[7]&65535u;goto L8c61f19a; /* extu.w r7,r7 */
L8c61f19a: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61f19c; /* shll r5 */
L8c61f19c: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61f19e; /* shll r6 */
L8c61f19e: r[6]+=r[12];goto L8c61f1a0; /* add r12,r6 */
L8c61f1a0: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f1a2; /* fmov.s @(r0,r6),fr4 */
L8c61f1a2: r[5]+=r[12];goto L8c61f1a4; /* add r12,r5 */
L8c61f1a4: r[7]<<=2;goto L8c61f1a6; /* shll2 r7 */
L8c61f1a6: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f1a8; /* fmov.s @(r0,r5),fr3 */
L8c61f1a8: r[7]<<=2;goto L8c61f1aa; /* shll2 r7 */
L8c61f1aa: fcopy(f,2u,4u);goto L8c61f1ac; /* fmov fr4,fr2 */
L8c61f1ac: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61f1ae; /* shll r7 */
L8c61f1ae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61f1b0; /* fsub fr3,fr2 */
L8c61f1b0: r[0]=20u;goto L8c61f1b2; /* mov #0x14,r0 */
L8c61f1b2: r[7]+=r[12];goto L8c61f1b4; /* add r12,r7 */
L8c61f1b4: fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f1b6; /* fmov.s @(r0,r6),fr5 */
L8c61f1b6: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f1b8; /* fmov.s @(r0,r7),fr3 */
L8c61f1b8: fcopy(f,1u,5u);goto L8c61f1ba; /* fmov fr5,fr1 */
L8c61f1ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f1bc; /* fsub fr5,fr3 */
L8c61f1bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f1be; /* fmul fr3,fr2 */
L8c61f1be: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f1c0; /* fmov.s @(r0,r5),fr3 */
L8c61f1c0: r[0]=16u;goto L8c61f1c2; /* mov #0x10,r0 */
L8c61f1c2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61f1c4; /* fsub fr3,fr1 */
L8c61f1c4: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f1c6; /* fmov.s @(r0,r7),fr3 */
L8c61f1c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f1c8; /* fsub fr4,fr3 */
L8c61f1c8: fcopy(f,4u,2u);goto L8c61f1ca; /* fmov fr2,fr4 */
L8c61f1ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61f1cc; /* fmul fr3,fr1 */
L8c61f1cc: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61f1ce; /* fldi0 fr3 */
L8c61f1ce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61f1d0; /* fsub fr1,fr4 */
L8c61f1d0: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61f1d2; /* fcmp/gt fr3,fr4 */
L8c61f1d2: take=!(*sr&1u);r[14]+=1u;if(take)goto L8c61f1da;goto L8c61f1d6; /* bf/s 0x8c61f1da */
L8c61f1d6: goto L8c61f32e; /* bra 0x8c61f32e */
L8c61f1da: r[0]=4u;goto L8c61f1dc; /* mov #0x4,r0 */
L8c61f1dc: r[8]+=28u;goto L8c61f1de; /* add #0x1c,r8 */
L8c61f1de: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f1e0; /* fmov.s fr7,@-r8 */
L8c61f1e0: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f1e2; /* mov.l @r9,r3 */
L8c61f1e2: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f1e4; /* fmov.s @(r0,r5),fr0 */
L8c61f1e4: r[0]=20u;goto L8c61f1e6; /* mov #0x14,r0 */
L8c61f1e6: r[1]=r[3];goto L8c61f1e8; /* mov r3,r1 */
L8c61f1e8: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f1ea; /* fmov.s @(r0,r1),fr3 */
L8c61f1ea: r[0]=4u;goto L8c61f1ec; /* mov #0x4,r0 */
L8c61f1ec: r[2]=r[3];goto L8c61f1ee; /* mov r3,r2 */
L8c61f1ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f1f0; /* fmul fr0,fr3 */
L8c61f1f0: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f1f2; /* fmov.s @(r0,r2),fr2 */
L8c61f1f2: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f1f4; /* fmov.s @r5,fr0 */
L8c61f1f4: r[0]=8u;goto L8c61f1f6; /* mov #0x8,r0 */
L8c61f1f6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f1f8; /* fmac fr0,fr2,fr3 */
L8c61f1f8: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f1fa; /* fmov.s @(r0,r5),fr0 */
L8c61f1fa: r[0]=36u;goto L8c61f1fc; /* mov #0x24,r0 */
L8c61f1fc: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f1fe; /* fmov.s @(r0,r3),fr2 */
L8c61f1fe: r[0]=4u;goto L8c61f200; /* mov #0x4,r0 */
L8c61f200: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f202; /* fmac fr0,fr2,fr3 */
L8c61f202: fcopy(f,2u,6u);goto L8c61f204; /* fmov fr6,fr2 */
L8c61f204: fcopy(f,0u,6u);goto L8c61f206; /* fmov fr6,fr0 */
L8c61f206: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f208; /* fmac fr0,fr3,fr2 */
L8c61f208: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f20a; /* fmov.s fr2,@-r8 */
L8c61f20a: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f20c; /* mov.l @r9,r3 */
L8c61f20c: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f20e; /* fmov.s @(r0,r5),fr0 */
L8c61f20e: r[0]=16u;goto L8c61f210; /* mov #0x10,r0 */
L8c61f210: r[1]=r[3];goto L8c61f212; /* mov r3,r1 */
L8c61f212: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f214; /* fmov.s @(r0,r1),fr3 */
L8c61f214: r[2]=r[3];goto L8c61f216; /* mov r3,r2 */
L8c61f216: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f218; /* fmul fr0,fr3 */
L8c61f218: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f21a; /* fmov.s @r5,fr0 */
L8c61f21a: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f21c; /* fmov.s @r2,fr2 */
L8c61f21c: r[0]=8u;goto L8c61f21e; /* mov #0x8,r0 */
L8c61f21e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f220; /* fmac fr0,fr2,fr3 */
L8c61f220: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f222; /* fmov.s @(r0,r5),fr0 */
L8c61f222: r[0]=32u;goto L8c61f224; /* mov #0x20,r0 */
L8c61f224: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f226; /* fmov.s @(r0,r3),fr2 */
L8c61f226: r[0]=24u;goto L8c61f228; /* mov #0x18,r0 */
L8c61f228: r[3]=rd(m,bus,&io_status,0x8c61f358u,4);if(io_status)return io_status;goto L8c61f22a; /* mov.l 0x8c61f358,r3 */
L8c61f22a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f22c; /* fmac fr0,fr2,fr3 */
L8c61f22c: fcopy(f,0u,6u);goto L8c61f22e; /* fmov fr6,fr0 */
L8c61f22e: fcopy(f,2u,6u);goto L8c61f230; /* fmov fr6,fr2 */
L8c61f230: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f232; /* fmac fr0,fr3,fr2 */
L8c61f232: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f234; /* fmov.s fr2,@-r8 */
L8c61f234: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f236; /* fmov.s @(r0,r5),fr3 */
L8c61f236: r[0]=20u;goto L8c61f238; /* mov #0x14,r0 */
L8c61f238: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f23a; /* fmov.s fr3,@-r8 */
L8c61f23a: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f23c; /* fmov.s @(r0,r5),fr3 */
L8c61f23c: r[0]=16u;goto L8c61f23e; /* mov #0x10,r0 */
L8c61f23e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f240; /* fmov.s fr3,@-r8 */
L8c61f240: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f242; /* fmov.s @(r0,r5),fr3 */
L8c61f242: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f244; /* fmov.s fr3,@-r8 */
L8c61f244: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f246; /* mov.l r3,@-r8 */
L8c61f246: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f248; /* pref @r8 */
L8c61f248: r[0]=4u;goto L8c61f24a; /* mov #0x4,r0 */
L8c61f24a: r[8]+=60u;goto L8c61f24c; /* add #0x3c,r8 */
L8c61f24c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f24e; /* fmov.s fr7,@-r8 */
L8c61f24e: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f250; /* mov.l @r9,r3 */
L8c61f250: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f252; /* fmov.s @(r0,r6),fr0 */
L8c61f252: r[0]=20u;goto L8c61f254; /* mov #0x14,r0 */
L8c61f254: r[1]=r[3];goto L8c61f256; /* mov r3,r1 */
L8c61f256: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f258; /* fmov.s @(r0,r1),fr3 */
L8c61f258: r[0]=4u;goto L8c61f25a; /* mov #0x4,r0 */
L8c61f25a: r[2]=r[3];goto L8c61f25c; /* mov r3,r2 */
L8c61f25c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f25e; /* fmul fr0,fr3 */
L8c61f25e: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f260; /* fmov.s @(r0,r2),fr2 */
L8c61f260: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61f262; /* fmov.s @r6,fr0 */
L8c61f262: r[0]=8u;goto L8c61f264; /* mov #0x8,r0 */
L8c61f264: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f266; /* fmac fr0,fr2,fr3 */
L8c61f266: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f268; /* fmov.s @(r0,r6),fr0 */
L8c61f268: r[0]=36u;goto L8c61f26a; /* mov #0x24,r0 */
L8c61f26a: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f26c; /* fmov.s @(r0,r3),fr2 */
L8c61f26c: r[0]=4u;goto L8c61f26e; /* mov #0x4,r0 */
L8c61f26e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f270; /* fmac fr0,fr2,fr3 */
L8c61f270: fcopy(f,2u,6u);goto L8c61f272; /* fmov fr6,fr2 */
L8c61f272: fcopy(f,0u,6u);goto L8c61f274; /* fmov fr6,fr0 */
L8c61f274: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f276; /* fmac fr0,fr3,fr2 */
L8c61f276: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f278; /* fmov.s fr2,@-r8 */
L8c61f278: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f27a; /* mov.l @r9,r3 */
L8c61f27a: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f27c; /* fmov.s @(r0,r6),fr0 */
L8c61f27c: r[0]=16u;goto L8c61f27e; /* mov #0x10,r0 */
L8c61f27e: r[1]=r[3];goto L8c61f280; /* mov r3,r1 */
L8c61f280: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f282; /* fmov.s @(r0,r1),fr3 */
L8c61f282: r[2]=r[3];goto L8c61f284; /* mov r3,r2 */
L8c61f284: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f286; /* fmul fr0,fr3 */
L8c61f286: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61f288; /* fmov.s @r6,fr0 */
L8c61f288: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f28a; /* fmov.s @r2,fr2 */
L8c61f28a: r[0]=8u;goto L8c61f28c; /* mov #0x8,r0 */
L8c61f28c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f28e; /* fmac fr0,fr2,fr3 */
L8c61f28e: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f290; /* fmov.s @(r0,r6),fr0 */
L8c61f290: r[0]=32u;goto L8c61f292; /* mov #0x20,r0 */
L8c61f292: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f294; /* fmov.s @(r0,r3),fr2 */
L8c61f294: r[0]=24u;goto L8c61f296; /* mov #0x18,r0 */
L8c61f296: r[3]=rd(m,bus,&io_status,0x8c61f358u,4);if(io_status)return io_status;goto L8c61f298; /* mov.l 0x8c61f358,r3 */
L8c61f298: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f29a; /* fmac fr0,fr2,fr3 */
L8c61f29a: fcopy(f,0u,6u);goto L8c61f29c; /* fmov fr6,fr0 */
L8c61f29c: fcopy(f,2u,6u);goto L8c61f29e; /* fmov fr6,fr2 */
L8c61f29e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f2a0; /* fmac fr0,fr3,fr2 */
L8c61f2a0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f2a2; /* fmov.s fr2,@-r8 */
L8c61f2a2: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f2a4; /* fmov.s @(r0,r6),fr3 */
L8c61f2a4: r[0]=20u;goto L8c61f2a6; /* mov #0x14,r0 */
L8c61f2a6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f2a8; /* fmov.s fr3,@-r8 */
L8c61f2a8: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f2aa; /* fmov.s @(r0,r6),fr3 */
L8c61f2aa: r[0]=16u;goto L8c61f2ac; /* mov #0x10,r0 */
L8c61f2ac: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f2ae; /* fmov.s fr3,@-r8 */
L8c61f2ae: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f2b0; /* fmov.s @(r0,r6),fr3 */
L8c61f2b0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f2b2; /* fmov.s fr3,@-r8 */
L8c61f2b2: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f2b4; /* mov.l r3,@-r8 */
L8c61f2b4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f2b6; /* pref @r8 */
L8c61f2b6: r[0]=4u;goto L8c61f2b8; /* mov #0x4,r0 */
L8c61f2b8: r[8]+=60u;goto L8c61f2ba; /* add #0x3c,r8 */
L8c61f2ba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f2bc; /* fmov.s fr7,@-r8 */
L8c61f2bc: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f2be; /* mov.l @r9,r3 */
L8c61f2be: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f2c0; /* fmov.s @(r0,r7),fr0 */
L8c61f2c0: r[0]=20u;goto L8c61f2c2; /* mov #0x14,r0 */
L8c61f2c2: r[1]=r[3];goto L8c61f2c4; /* mov r3,r1 */
L8c61f2c4: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f2c6; /* fmov.s @(r0,r1),fr3 */
L8c61f2c6: r[0]=4u;goto L8c61f2c8; /* mov #0x4,r0 */
L8c61f2c8: r[2]=r[3];goto L8c61f2ca; /* mov r3,r2 */
L8c61f2ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f2cc; /* fmul fr0,fr3 */
L8c61f2cc: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f2ce; /* fmov.s @(r0,r2),fr2 */
L8c61f2ce: fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c61f2d0; /* fmov.s @r7,fr0 */
L8c61f2d0: r[0]=8u;goto L8c61f2d2; /* mov #0x8,r0 */
L8c61f2d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f2d4; /* fmac fr0,fr2,fr3 */
L8c61f2d4: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f2d6; /* fmov.s @(r0,r7),fr0 */
L8c61f2d6: r[0]=36u;goto L8c61f2d8; /* mov #0x24,r0 */
L8c61f2d8: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f2da; /* fmov.s @(r0,r3),fr2 */
L8c61f2da: r[0]=4u;goto L8c61f2dc; /* mov #0x4,r0 */
L8c61f2dc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f2de; /* fmac fr0,fr2,fr3 */
L8c61f2de: fcopy(f,2u,6u);goto L8c61f2e0; /* fmov fr6,fr2 */
L8c61f2e0: fcopy(f,0u,6u);goto L8c61f2e2; /* fmov fr6,fr0 */
L8c61f2e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f2e4; /* fmac fr0,fr3,fr2 */
L8c61f2e4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f2e6; /* fmov.s fr2,@-r8 */
L8c61f2e6: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f2e8; /* mov.l @r9,r3 */
L8c61f2e8: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f2ea; /* fmov.s @(r0,r7),fr0 */
L8c61f2ea: r[0]=16u;goto L8c61f2ec; /* mov #0x10,r0 */
L8c61f2ec: r[1]=r[3];goto L8c61f2ee; /* mov r3,r1 */
L8c61f2ee: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f2f0; /* fmov.s @(r0,r1),fr3 */
L8c61f2f0: r[2]=r[3];goto L8c61f2f2; /* mov r3,r2 */
L8c61f2f2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f2f4; /* fmul fr0,fr3 */
L8c61f2f4: fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c61f2f6; /* fmov.s @r7,fr0 */
L8c61f2f6: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f2f8; /* fmov.s @r2,fr2 */
L8c61f2f8: r[0]=8u;goto L8c61f2fa; /* mov #0x8,r0 */
L8c61f2fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f2fc; /* fmac fr0,fr2,fr3 */
L8c61f2fc: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f2fe; /* fmov.s @(r0,r7),fr0 */
L8c61f2fe: r[0]=32u;goto L8c61f300; /* mov #0x20,r0 */
L8c61f300: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f302; /* fmov.s @(r0,r3),fr2 */
L8c61f302: r[0]=24u;goto L8c61f304; /* mov #0x18,r0 */
L8c61f304: r[3]=rd(m,bus,&io_status,0x8c61f35cu,4);if(io_status)return io_status;goto L8c61f306; /* mov.l 0x8c61f35c,r3 */
L8c61f306: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f308; /* fmac fr0,fr2,fr3 */
L8c61f308: fcopy(f,0u,6u);goto L8c61f30a; /* fmov fr6,fr0 */
L8c61f30a: fcopy(f,2u,6u);goto L8c61f30c; /* fmov fr6,fr2 */
L8c61f30c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f30e; /* fmac fr0,fr3,fr2 */
L8c61f30e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f310; /* fmov.s fr2,@-r8 */
L8c61f310: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f312; /* fmov.s @(r0,r7),fr3 */
L8c61f312: r[0]=20u;goto L8c61f314; /* mov #0x14,r0 */
L8c61f314: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f316; /* fmov.s fr3,@-r8 */
L8c61f316: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f318; /* fmov.s @(r0,r7),fr3 */
L8c61f318: r[0]=16u;goto L8c61f31a; /* mov #0x10,r0 */
L8c61f31a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f31c; /* fmov.s fr3,@-r8 */
L8c61f31c: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f31e; /* fmov.s @(r0,r7),fr3 */
L8c61f31e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f320; /* fmov.s fr3,@-r8 */
L8c61f320: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f322; /* mov.l r3,@-r8 */
L8c61f322: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f324; /* pref @r8 */
L8c61f324: r[2]=rd(m,bus,&io_status,0x8c61f360u,4);if(io_status)return io_status;goto L8c61f326; /* mov.l 0x8c61f360,r2 */
L8c61f326: r[8]+=32u;goto L8c61f328; /* add #0x20,r8 */
L8c61f328: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61f32a; /* mov.l @r2,r3 */
L8c61f32a: r[3]+=3u;goto L8c61f32c; /* add #0x3,r3 */
L8c61f32c: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61f32e; /* mov.l r3,@r2 */
L8c61f32e: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61f330; /* mov.w @(0x2,r4),r0 */
L8c61f330: r[0]=r[0]&65535u;goto L8c61f332; /* extu.w r0,r0 */
L8c61f332: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c61f334; /* cmp/ge r0,r14 */
L8c61f334: if((*sr&1u))goto L8c61f33a;goto L8c61f336; /* bt 0x8c61f33a */
L8c61f336: goto L8c61f180; /* bra 0x8c61f180 */
L8c61f33a: goto L8c61f79c; /* bra 0x8c61f79c */
L8c61f33e: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61f340; /* mov.w @(0x2,r4),r0 */
L8c61f340: r[2]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c61f342; /* mov.l @r11,r2 */
L8c61f342: r[0]=r[0]&65535u;goto L8c61f344; /* extu.w r0,r0 */
L8c61f344: r[0]<<=2;goto L8c61f346; /* shll2 r0 */
L8c61f346: r[2]+=r[0];goto L8c61f348; /* add r0,r2 */
L8c61f348: wr(m,bus,&io_status,r[11],r[2],4);if(io_status)return io_status;goto L8c61f34a; /* mov.l r2,@r11 */
L8c61f34a: r[11]=r[12];goto L8c61f58c; /* bra 0x8c61f58c */
L8c61f364: r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f366; /* mov.w @r13+,r6 */
L8c61f366: r[0]=16u;goto L8c61f368; /* mov #0x10,r0 */
L8c61f368: r[3]=rd(m,bus,&io_status,0x8c61f604u,4);if(io_status)return io_status;goto L8c61f36a; /* mov.l 0x8c61f604,r3 */
L8c61f36a: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f36c; /* mov.w @r13+,r5 */
L8c61f36c: r[6]=r[6]&65535u;goto L8c61f36e; /* extu.w r6,r6 */
L8c61f36e: r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f370; /* mov.l @r3,r12 */
L8c61f370: r[6]<<=2;goto L8c61f372; /* shll2 r6 */
L8c61f372: r[5]=r[5]&65535u;goto L8c61f374; /* extu.w r5,r5 */
L8c61f374: r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f376; /* mov.w @r13+,r7 */
L8c61f376: r[5]<<=2;goto L8c61f378; /* shll2 r5 */
L8c61f378: r[5]<<=2;goto L8c61f37a; /* shll2 r5 */
L8c61f37a: r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f37c; /* mov.w @r13+,r14 */
L8c61f37c: r[6]<<=2;goto L8c61f37e; /* shll2 r6 */
L8c61f37e: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61f380; /* shll r6 */
L8c61f380: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61f382; /* shll r5 */
L8c61f382: r[5]+=r[12];goto L8c61f384; /* add r12,r5 */
L8c61f384: r[6]+=r[12];goto L8c61f386; /* add r12,r6 */
L8c61f386: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f388; /* fmov.s @(r0,r5),fr4 */
L8c61f388: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f38a; /* fmov.s @(r0,r6),fr3 */
L8c61f38a: r[7]=r[7]&65535u;goto L8c61f38c; /* extu.w r7,r7 */
L8c61f38c: fcopy(f,2u,4u);goto L8c61f38e; /* fmov fr4,fr2 */
L8c61f38e: r[7]<<=2;goto L8c61f390; /* shll2 r7 */
L8c61f390: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61f392; /* fsub fr3,fr2 */
L8c61f392: r[14]=r[14]&65535u;goto L8c61f394; /* extu.w r14,r14 */
L8c61f394: r[7]<<=2;goto L8c61f396; /* shll2 r7 */
L8c61f396: r[14]<<=2;goto L8c61f398; /* shll2 r14 */
L8c61f398: r[0]=20u;goto L8c61f39a; /* mov #0x14,r0 */
L8c61f39a: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f39c; /* fmov.s @(r0,r5),fr5 */
L8c61f39c: r[14]<<=2;goto L8c61f39e; /* shll2 r14 */
L8c61f39e: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61f3a0; /* shll r7 */
L8c61f3a0: r[7]+=r[12];goto L8c61f3a2; /* add r12,r7 */
L8c61f3a2: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61f3a4; /* shll r14 */
L8c61f3a4: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f3a6; /* fmov.s @(r0,r7),fr3 */
L8c61f3a6: fcopy(f,1u,5u);goto L8c61f3a8; /* fmov fr5,fr1 */
L8c61f3a8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f3aa; /* fsub fr5,fr3 */
L8c61f3aa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f3ac; /* fmul fr3,fr2 */
L8c61f3ac: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f3ae; /* fmov.s @(r0,r6),fr3 */
L8c61f3ae: r[0]=16u;goto L8c61f3b0; /* mov #0x10,r0 */
L8c61f3b0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61f3b2; /* fsub fr3,fr1 */
L8c61f3b2: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f3b4; /* fmov.s @(r0,r7),fr3 */
L8c61f3b4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f3b6; /* fsub fr4,fr3 */
L8c61f3b6: fcopy(f,4u,2u);goto L8c61f3b8; /* fmov fr2,fr4 */
L8c61f3b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61f3ba; /* fmul fr3,fr1 */
L8c61f3ba: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61f3bc; /* fldi0 fr3 */
L8c61f3bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61f3be; /* fsub fr1,fr4 */
L8c61f3be: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61f3c0; /* fcmp/gt fr3,fr4 */
L8c61f3c0: take=!(*sr&1u);r[14]+=r[12];if(take)goto L8c61f3c8;goto L8c61f3c4; /* bf/s 0x8c61f3c8 */
L8c61f3c4: goto L8c61f58a; /* bra 0x8c61f58a */
L8c61f3c8: r[0]=4u;goto L8c61f3ca; /* mov #0x4,r0 */
L8c61f3ca: r[8]+=28u;goto L8c61f3cc; /* add #0x1c,r8 */
L8c61f3cc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f3ce; /* fmov.s fr7,@-r8 */
L8c61f3ce: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f3d0; /* mov.l @r9,r3 */
L8c61f3d0: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f3d2; /* fmov.s @(r0,r6),fr0 */
L8c61f3d2: r[0]=20u;goto L8c61f3d4; /* mov #0x14,r0 */
L8c61f3d4: r[1]=r[3];goto L8c61f3d6; /* mov r3,r1 */
L8c61f3d6: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f3d8; /* fmov.s @(r0,r1),fr3 */
L8c61f3d8: r[0]=4u;goto L8c61f3da; /* mov #0x4,r0 */
L8c61f3da: r[2]=r[3];goto L8c61f3dc; /* mov r3,r2 */
L8c61f3dc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f3de; /* fmul fr0,fr3 */
L8c61f3de: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f3e0; /* fmov.s @(r0,r2),fr2 */
L8c61f3e0: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61f3e2; /* fmov.s @r6,fr0 */
L8c61f3e2: r[0]=8u;goto L8c61f3e4; /* mov #0x8,r0 */
L8c61f3e4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f3e6; /* fmac fr0,fr2,fr3 */
L8c61f3e6: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f3e8; /* fmov.s @(r0,r6),fr0 */
L8c61f3e8: r[0]=36u;goto L8c61f3ea; /* mov #0x24,r0 */
L8c61f3ea: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f3ec; /* fmov.s @(r0,r3),fr2 */
L8c61f3ec: r[0]=4u;goto L8c61f3ee; /* mov #0x4,r0 */
L8c61f3ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f3f0; /* fmac fr0,fr2,fr3 */
L8c61f3f0: fcopy(f,2u,6u);goto L8c61f3f2; /* fmov fr6,fr2 */
L8c61f3f2: fcopy(f,0u,6u);goto L8c61f3f4; /* fmov fr6,fr0 */
L8c61f3f4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f3f6; /* fmac fr0,fr3,fr2 */
L8c61f3f6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f3f8; /* fmov.s fr2,@-r8 */
L8c61f3f8: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f3fa; /* mov.l @r9,r3 */
L8c61f3fa: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f3fc; /* fmov.s @(r0,r6),fr0 */
L8c61f3fc: r[0]=16u;goto L8c61f3fe; /* mov #0x10,r0 */
L8c61f3fe: r[1]=r[3];goto L8c61f400; /* mov r3,r1 */
L8c61f400: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f402; /* fmov.s @(r0,r1),fr3 */
L8c61f402: r[2]=r[3];goto L8c61f404; /* mov r3,r2 */
L8c61f404: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f406; /* fmul fr0,fr3 */
L8c61f406: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61f408; /* fmov.s @r6,fr0 */
L8c61f408: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f40a; /* fmov.s @r2,fr2 */
L8c61f40a: r[0]=8u;goto L8c61f40c; /* mov #0x8,r0 */
L8c61f40c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f40e; /* fmac fr0,fr2,fr3 */
L8c61f40e: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f410; /* fmov.s @(r0,r6),fr0 */
L8c61f410: r[0]=32u;goto L8c61f412; /* mov #0x20,r0 */
L8c61f412: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f414; /* fmov.s @(r0,r3),fr2 */
L8c61f414: r[0]=24u;goto L8c61f416; /* mov #0x18,r0 */
L8c61f416: r[3]=rd(m,bus,&io_status,0x8c61f608u,4);if(io_status)return io_status;goto L8c61f418; /* mov.l 0x8c61f608,r3 */
L8c61f418: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f41a; /* fmac fr0,fr2,fr3 */
L8c61f41a: fcopy(f,0u,6u);goto L8c61f41c; /* fmov fr6,fr0 */
L8c61f41c: fcopy(f,2u,6u);goto L8c61f41e; /* fmov fr6,fr2 */
L8c61f41e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f420; /* fmac fr0,fr3,fr2 */
L8c61f420: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f422; /* fmov.s fr2,@-r8 */
L8c61f422: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f424; /* fmov.s @(r0,r6),fr3 */
L8c61f424: r[0]=20u;goto L8c61f426; /* mov #0x14,r0 */
L8c61f426: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f428; /* fmov.s fr3,@-r8 */
L8c61f428: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f42a; /* fmov.s @(r0,r6),fr3 */
L8c61f42a: r[0]=16u;goto L8c61f42c; /* mov #0x10,r0 */
L8c61f42c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f42e; /* fmov.s fr3,@-r8 */
L8c61f42e: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f430; /* fmov.s @(r0,r6),fr3 */
L8c61f430: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f432; /* fmov.s fr3,@-r8 */
L8c61f432: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f434; /* mov.l r3,@-r8 */
L8c61f434: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f436; /* pref @r8 */
L8c61f436: r[0]=4u;goto L8c61f438; /* mov #0x4,r0 */
L8c61f438: r[8]+=60u;goto L8c61f43a; /* add #0x3c,r8 */
L8c61f43a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f43c; /* fmov.s fr7,@-r8 */
L8c61f43c: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f43e; /* mov.l @r9,r3 */
L8c61f43e: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f440; /* fmov.s @(r0,r5),fr0 */
L8c61f440: r[0]=20u;goto L8c61f442; /* mov #0x14,r0 */
L8c61f442: r[1]=r[3];goto L8c61f444; /* mov r3,r1 */
L8c61f444: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f446; /* fmov.s @(r0,r1),fr3 */
L8c61f446: r[0]=4u;goto L8c61f448; /* mov #0x4,r0 */
L8c61f448: r[2]=r[3];goto L8c61f44a; /* mov r3,r2 */
L8c61f44a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f44c; /* fmul fr0,fr3 */
L8c61f44c: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f44e; /* fmov.s @(r0,r2),fr2 */
L8c61f44e: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f450; /* fmov.s @r5,fr0 */
L8c61f450: r[0]=8u;goto L8c61f452; /* mov #0x8,r0 */
L8c61f452: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f454; /* fmac fr0,fr2,fr3 */
L8c61f454: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f456; /* fmov.s @(r0,r5),fr0 */
L8c61f456: r[0]=36u;goto L8c61f458; /* mov #0x24,r0 */
L8c61f458: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f45a; /* fmov.s @(r0,r3),fr2 */
L8c61f45a: r[0]=4u;goto L8c61f45c; /* mov #0x4,r0 */
L8c61f45c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f45e; /* fmac fr0,fr2,fr3 */
L8c61f45e: fcopy(f,2u,6u);goto L8c61f460; /* fmov fr6,fr2 */
L8c61f460: fcopy(f,0u,6u);goto L8c61f462; /* fmov fr6,fr0 */
L8c61f462: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f464; /* fmac fr0,fr3,fr2 */
L8c61f464: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f466; /* fmov.s fr2,@-r8 */
L8c61f466: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f468; /* mov.l @r9,r3 */
L8c61f468: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f46a; /* fmov.s @(r0,r5),fr0 */
L8c61f46a: r[0]=16u;goto L8c61f46c; /* mov #0x10,r0 */
L8c61f46c: r[1]=r[3];goto L8c61f46e; /* mov r3,r1 */
L8c61f46e: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f470; /* fmov.s @(r0,r1),fr3 */
L8c61f470: r[2]=r[3];goto L8c61f472; /* mov r3,r2 */
L8c61f472: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f474; /* fmul fr0,fr3 */
L8c61f474: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f476; /* fmov.s @r5,fr0 */
L8c61f476: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f478; /* fmov.s @r2,fr2 */
L8c61f478: r[0]=8u;goto L8c61f47a; /* mov #0x8,r0 */
L8c61f47a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f47c; /* fmac fr0,fr2,fr3 */
L8c61f47c: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f47e; /* fmov.s @(r0,r5),fr0 */
L8c61f47e: r[0]=32u;goto L8c61f480; /* mov #0x20,r0 */
L8c61f480: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f482; /* fmov.s @(r0,r3),fr2 */
L8c61f482: r[0]=24u;goto L8c61f484; /* mov #0x18,r0 */
L8c61f484: r[3]=rd(m,bus,&io_status,0x8c61f608u,4);if(io_status)return io_status;goto L8c61f486; /* mov.l 0x8c61f608,r3 */
L8c61f486: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f488; /* fmac fr0,fr2,fr3 */
L8c61f488: fcopy(f,0u,6u);goto L8c61f48a; /* fmov fr6,fr0 */
L8c61f48a: fcopy(f,2u,6u);goto L8c61f48c; /* fmov fr6,fr2 */
L8c61f48c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f48e; /* fmac fr0,fr3,fr2 */
L8c61f48e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f490; /* fmov.s fr2,@-r8 */
L8c61f490: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f492; /* fmov.s @(r0,r5),fr3 */
L8c61f492: r[0]=20u;goto L8c61f494; /* mov #0x14,r0 */
L8c61f494: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f496; /* fmov.s fr3,@-r8 */
L8c61f496: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f498; /* fmov.s @(r0,r5),fr3 */
L8c61f498: r[0]=16u;goto L8c61f49a; /* mov #0x10,r0 */
L8c61f49a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f49c; /* fmov.s fr3,@-r8 */
L8c61f49c: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f49e; /* fmov.s @(r0,r5),fr3 */
L8c61f49e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f4a0; /* fmov.s fr3,@-r8 */
L8c61f4a0: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f4a2; /* mov.l r3,@-r8 */
L8c61f4a2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f4a4; /* pref @r8 */
L8c61f4a4: r[0]=4u;goto L8c61f4a6; /* mov #0x4,r0 */
L8c61f4a6: r[8]+=60u;goto L8c61f4a8; /* add #0x3c,r8 */
L8c61f4a8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f4aa; /* fmov.s fr7,@-r8 */
L8c61f4aa: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f4ac; /* mov.l @r9,r3 */
L8c61f4ac: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f4ae; /* fmov.s @(r0,r7),fr0 */
L8c61f4ae: r[0]=20u;goto L8c61f4b0; /* mov #0x14,r0 */
L8c61f4b0: r[1]=r[3];goto L8c61f4b2; /* mov r3,r1 */
L8c61f4b2: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f4b4; /* fmov.s @(r0,r1),fr3 */
L8c61f4b4: r[0]=4u;goto L8c61f4b6; /* mov #0x4,r0 */
L8c61f4b6: r[2]=r[3];goto L8c61f4b8; /* mov r3,r2 */
L8c61f4b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f4ba; /* fmul fr0,fr3 */
L8c61f4ba: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f4bc; /* fmov.s @(r0,r2),fr2 */
L8c61f4bc: fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c61f4be; /* fmov.s @r7,fr0 */
L8c61f4be: r[0]=8u;goto L8c61f4c0; /* mov #0x8,r0 */
L8c61f4c0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f4c2; /* fmac fr0,fr2,fr3 */
L8c61f4c2: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f4c4; /* fmov.s @(r0,r7),fr0 */
L8c61f4c4: r[0]=36u;goto L8c61f4c6; /* mov #0x24,r0 */
L8c61f4c6: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f4c8; /* fmov.s @(r0,r3),fr2 */
L8c61f4c8: r[0]=4u;goto L8c61f4ca; /* mov #0x4,r0 */
L8c61f4ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f4cc; /* fmac fr0,fr2,fr3 */
L8c61f4cc: fcopy(f,2u,6u);goto L8c61f4ce; /* fmov fr6,fr2 */
L8c61f4ce: fcopy(f,0u,6u);goto L8c61f4d0; /* fmov fr6,fr0 */
L8c61f4d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f4d2; /* fmac fr0,fr3,fr2 */
L8c61f4d2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f4d4; /* fmov.s fr2,@-r8 */
L8c61f4d4: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f4d6; /* mov.l @r9,r3 */
L8c61f4d6: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f4d8; /* fmov.s @(r0,r7),fr0 */
L8c61f4d8: r[0]=16u;goto L8c61f4da; /* mov #0x10,r0 */
L8c61f4da: r[1]=r[3];goto L8c61f4dc; /* mov r3,r1 */
L8c61f4dc: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f4de; /* fmov.s @(r0,r1),fr3 */
L8c61f4de: r[2]=r[3];goto L8c61f4e0; /* mov r3,r2 */
L8c61f4e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f4e2; /* fmul fr0,fr3 */
L8c61f4e2: fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c61f4e4; /* fmov.s @r7,fr0 */
L8c61f4e4: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f4e6; /* fmov.s @r2,fr2 */
L8c61f4e6: r[0]=8u;goto L8c61f4e8; /* mov #0x8,r0 */
L8c61f4e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f4ea; /* fmac fr0,fr2,fr3 */
L8c61f4ea: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f4ec; /* fmov.s @(r0,r7),fr0 */
L8c61f4ec: r[0]=32u;goto L8c61f4ee; /* mov #0x20,r0 */
L8c61f4ee: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f4f0; /* fmov.s @(r0,r3),fr2 */
L8c61f4f0: r[0]=24u;goto L8c61f4f2; /* mov #0x18,r0 */
L8c61f4f2: r[3]=rd(m,bus,&io_status,0x8c61f608u,4);if(io_status)return io_status;goto L8c61f4f4; /* mov.l 0x8c61f608,r3 */
L8c61f4f4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f4f6; /* fmac fr0,fr2,fr3 */
L8c61f4f6: fcopy(f,0u,6u);goto L8c61f4f8; /* fmov fr6,fr0 */
L8c61f4f8: fcopy(f,2u,6u);goto L8c61f4fa; /* fmov fr6,fr2 */
L8c61f4fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f4fc; /* fmac fr0,fr3,fr2 */
L8c61f4fc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f4fe; /* fmov.s fr2,@-r8 */
L8c61f4fe: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f500; /* fmov.s @(r0,r7),fr3 */
L8c61f500: r[0]=20u;goto L8c61f502; /* mov #0x14,r0 */
L8c61f502: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f504; /* fmov.s fr3,@-r8 */
L8c61f504: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f506; /* fmov.s @(r0,r7),fr3 */
L8c61f506: r[0]=16u;goto L8c61f508; /* mov #0x10,r0 */
L8c61f508: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f50a; /* fmov.s fr3,@-r8 */
L8c61f50a: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f50c; /* fmov.s @(r0,r7),fr3 */
L8c61f50c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f50e; /* fmov.s fr3,@-r8 */
L8c61f50e: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f510; /* mov.l r3,@-r8 */
L8c61f510: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f512; /* pref @r8 */
L8c61f512: r[0]=4u;goto L8c61f514; /* mov #0x4,r0 */
L8c61f514: r[8]+=60u;goto L8c61f516; /* add #0x3c,r8 */
L8c61f516: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f518; /* fmov.s fr7,@-r8 */
L8c61f518: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f51a; /* mov.l @r9,r3 */
L8c61f51a: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61f51c; /* fmov.s @(r0,r14),fr0 */
L8c61f51c: r[0]=20u;goto L8c61f51e; /* mov #0x14,r0 */
L8c61f51e: r[1]=r[3];goto L8c61f520; /* mov r3,r1 */
L8c61f520: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f522; /* fmov.s @(r0,r1),fr3 */
L8c61f522: r[0]=4u;goto L8c61f524; /* mov #0x4,r0 */
L8c61f524: r[2]=r[3];goto L8c61f526; /* mov r3,r2 */
L8c61f526: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f528; /* fmul fr0,fr3 */
L8c61f528: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f52a; /* fmov.s @(r0,r2),fr2 */
L8c61f52a: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61f52c; /* fmov.s @r14,fr0 */
L8c61f52c: r[0]=8u;goto L8c61f52e; /* mov #0x8,r0 */
L8c61f52e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f530; /* fmac fr0,fr2,fr3 */
L8c61f530: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61f532; /* fmov.s @(r0,r14),fr0 */
L8c61f532: r[0]=36u;goto L8c61f534; /* mov #0x24,r0 */
L8c61f534: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f536; /* fmov.s @(r0,r3),fr2 */
L8c61f536: r[0]=4u;goto L8c61f538; /* mov #0x4,r0 */
L8c61f538: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f53a; /* fmac fr0,fr2,fr3 */
L8c61f53a: fcopy(f,2u,6u);goto L8c61f53c; /* fmov fr6,fr2 */
L8c61f53c: fcopy(f,0u,6u);goto L8c61f53e; /* fmov fr6,fr0 */
L8c61f53e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f540; /* fmac fr0,fr3,fr2 */
L8c61f540: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f542; /* fmov.s fr2,@-r8 */
L8c61f542: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f544; /* mov.l @r9,r3 */
L8c61f544: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61f546; /* fmov.s @(r0,r14),fr0 */
L8c61f546: r[0]=16u;goto L8c61f548; /* mov #0x10,r0 */
L8c61f548: r[1]=r[3];goto L8c61f54a; /* mov r3,r1 */
L8c61f54a: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f54c; /* fmov.s @(r0,r1),fr3 */
L8c61f54c: r[2]=r[3];goto L8c61f54e; /* mov r3,r2 */
L8c61f54e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f550; /* fmul fr0,fr3 */
L8c61f550: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61f552; /* fmov.s @r14,fr0 */
L8c61f552: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f554; /* fmov.s @r2,fr2 */
L8c61f554: r[0]=8u;goto L8c61f556; /* mov #0x8,r0 */
L8c61f556: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f558; /* fmac fr0,fr2,fr3 */
L8c61f558: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61f55a; /* fmov.s @(r0,r14),fr0 */
L8c61f55a: r[0]=32u;goto L8c61f55c; /* mov #0x20,r0 */
L8c61f55c: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f55e; /* fmov.s @(r0,r3),fr2 */
L8c61f55e: r[0]=24u;goto L8c61f560; /* mov #0x18,r0 */
L8c61f560: r[3]=rd(m,bus,&io_status,0x8c61f60cu,4);if(io_status)return io_status;goto L8c61f562; /* mov.l 0x8c61f60c,r3 */
L8c61f562: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f564; /* fmac fr0,fr2,fr3 */
L8c61f564: fcopy(f,0u,6u);goto L8c61f566; /* fmov fr6,fr0 */
L8c61f566: fcopy(f,2u,6u);goto L8c61f568; /* fmov fr6,fr2 */
L8c61f568: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f56a; /* fmac fr0,fr3,fr2 */
L8c61f56a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f56c; /* fmov.s fr2,@-r8 */
L8c61f56c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61f56e; /* fmov.s @(r0,r14),fr3 */
L8c61f56e: r[0]=20u;goto L8c61f570; /* mov #0x14,r0 */
L8c61f570: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f572; /* fmov.s fr3,@-r8 */
L8c61f572: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61f574; /* fmov.s @(r0,r14),fr3 */
L8c61f574: r[0]=16u;goto L8c61f576; /* mov #0x10,r0 */
L8c61f576: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f578; /* fmov.s fr3,@-r8 */
L8c61f578: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61f57a; /* fmov.s @(r0,r14),fr3 */
L8c61f57a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f57c; /* fmov.s fr3,@-r8 */
L8c61f57c: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f57e; /* mov.l r3,@-r8 */
L8c61f57e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f580; /* pref @r8 */
L8c61f580: r[2]=rd(m,bus,&io_status,0x8c61f610u,4);if(io_status)return io_status;goto L8c61f582; /* mov.l 0x8c61f610,r2 */
L8c61f582: r[8]+=32u;goto L8c61f584; /* add #0x20,r8 */
L8c61f584: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61f586; /* mov.l @r2,r3 */
L8c61f586: r[3]+=4u;goto L8c61f588; /* add #0x4,r3 */
L8c61f588: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61f58a; /* mov.l r3,@r2 */
L8c61f58a: r[11]+=1u;goto L8c61f58c; /* add #0x1,r11 */
L8c61f58c: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61f58e; /* mov.w @(0x2,r4),r0 */
L8c61f58e: r[0]=r[0]&65535u;goto L8c61f590; /* extu.w r0,r0 */
L8c61f590: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61f592; /* cmp/ge r0,r11 */
L8c61f592: if((*sr&1u))goto L8c61f598;goto L8c61f594; /* bt 0x8c61f598 */
L8c61f594: goto L8c61f364; /* bra 0x8c61f364 */
L8c61f598: goto L8c61f79c; /* bra 0x8c61f79c */
L8c61f59c: r[10]=r[12];goto L8c61f72e; /* bra 0x8c61f72e */
L8c61f5a0: r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f5a2; /* mov.w @r13+,r14 */
L8c61f5a2: r[3]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c61f5a4; /* mov.l @r11,r3 */
L8c61f5a4: r[14]=r[14]&65535u;goto L8c61f5a6; /* extu.w r14,r14 */
L8c61f5a6: r[2]=rd(m,bus,&io_status,0x8c61f604u,4);if(io_status)return io_status;goto L8c61f5a8; /* mov.l 0x8c61f604,r2 */
L8c61f5a8: r[3]+=r[14];goto L8c61f5aa; /* add r14,r3 */
L8c61f5aa: wr(m,bus,&io_status,r[11],r[3],4);if(io_status)return io_status;goto L8c61f5ac; /* mov.l r3,@r11 */
L8c61f5ac: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61f5ae; /* mov.w @(0x2,r13),r0 */
L8c61f5ae: r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61f5b0; /* mov.w @r13,r7 */
L8c61f5b0: r[5]=r[0]&65535u;goto L8c61f5b2; /* extu.w r0,r5 */
L8c61f5b2: r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c61f5b4; /* mov.w @(0x4,r13),r0 */
L8c61f5b4: r[5]<<=2;goto L8c61f5b6; /* shll2 r5 */
L8c61f5b6: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61f5b8; /* mov.l @r2,r3 */
L8c61f5b8: r[7]=r[7]&65535u;goto L8c61f5ba; /* extu.w r7,r7 */
L8c61f5ba: r[7]<<=2;goto L8c61f5bc; /* shll2 r7 */
L8c61f5bc: r[5]<<=2;goto L8c61f5be; /* shll2 r5 */
L8c61f5be: r[6]=r[0]&65535u;goto L8c61f5c0; /* extu.w r0,r6 */
L8c61f5c0: r[7]<<=2;goto L8c61f5c2; /* shll2 r7 */
L8c61f5c2: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61f5c4; /* shll r7 */
L8c61f5c4: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61f5c6; /* shll r5 */
L8c61f5c6: r[0]=16u;goto L8c61f5c8; /* mov #0x10,r0 */
L8c61f5c8: r[5]+=r[3];goto L8c61f5ca; /* add r3,r5 */
L8c61f5ca: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f5cc; /* fmov.s @(r0,r5),fr5 */
L8c61f5cc: r[7]+=r[3];goto L8c61f5ce; /* add r3,r7 */
L8c61f5ce: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f5d0; /* fmov.s @(r0,r7),fr3 */
L8c61f5d0: r[6]<<=2;goto L8c61f5d2; /* shll2 r6 */
L8c61f5d2: fcopy(f,2u,5u);goto L8c61f5d4; /* fmov fr5,fr2 */
L8c61f5d4: r[6]<<=2;goto L8c61f5d6; /* shll2 r6 */
L8c61f5d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61f5d8; /* fsub fr3,fr2 */
L8c61f5d8: r[0]=20u;goto L8c61f5da; /* mov #0x14,r0 */
L8c61f5da: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61f5dc; /* shll r6 */
L8c61f5dc: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f5de; /* fmov.s @(r0,r5),fr4 */
L8c61f5de: r[6]+=r[3];goto L8c61f5e0; /* add r3,r6 */
L8c61f5e0: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f5e2; /* fmov.s @(r0,r6),fr3 */
L8c61f5e2: fcopy(f,1u,4u);goto L8c61f5e4; /* fmov fr4,fr1 */
L8c61f5e4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f5e6; /* fsub fr4,fr3 */
L8c61f5e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f5e8; /* fmul fr3,fr2 */
L8c61f5e8: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f5ea; /* fmov.s @(r0,r7),fr3 */
L8c61f5ea: r[0]=16u;goto L8c61f5ec; /* mov #0x10,r0 */
L8c61f5ec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61f5ee; /* fsub fr3,fr1 */
L8c61f5ee: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f5f0; /* fmov.s @(r0,r6),fr3 */
L8c61f5f0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f5f2; /* fsub fr5,fr3 */
L8c61f5f2: fcopy(f,4u,2u);goto L8c61f5f4; /* fmov fr2,fr4 */
L8c61f5f4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61f5f6; /* fmul fr3,fr1 */
L8c61f5f6: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61f5f8; /* fldi0 fr3 */
L8c61f5f8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61f5fa; /* fsub fr1,fr4 */
L8c61f5fa: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61f5fc; /* fcmp/gt fr3,fr4 */
L8c61f5fc: if(!(*sr&1u))goto L8c61f614;goto L8c61f5fe; /* bf 0x8c61f614 */
L8c61f5fe: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61f600; /* shll r14 */
L8c61f600: r[13]+=r[14];goto L8c61f72c; /* bra 0x8c61f72c */
L8c61f614: r[3]=rd(m,bus,&io_status,0x8c61f7b0u,4);if(io_status)return io_status;goto L8c61f616; /* mov.l 0x8c61f7b0,r3 */
L8c61f616: r[7]=r[14];goto L8c61f618; /* mov r14,r7 */
L8c61f618: r[7]+=4294967295u;goto L8c61f61a; /* add #-0x1,r7 */
L8c61f61a: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f61c; /* mov.l @r3,r2 */
L8c61f61c: *sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c61f61e; /* cmp/pl r7 */
L8c61f61e: r[2]+=r[14];goto L8c61f620; /* add r14,r2 */
L8c61f620: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61f622; /* mov.l r2,@r3 */
L8c61f622: take=!(*sr&1u);r[6]=r[12];if(take)goto L8c61f6ac;goto L8c61f626; /* bf/s 0x8c61f6ac */
L8c61f626: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f628; /* mov.w @r13+,r5 */
L8c61f628: r[0]=4u;goto L8c61f62a; /* mov #0x4,r0 */
L8c61f62a: r[2]=rd(m,bus,&io_status,0x8c61f7b4u,4);if(io_status)return io_status;goto L8c61f62c; /* mov.l 0x8c61f7b4,r2 */
L8c61f62c: r[8]+=28u;goto L8c61f62e; /* add #0x1c,r8 */
L8c61f62e: r[5]=r[5]&65535u;goto L8c61f630; /* extu.w r5,r5 */
L8c61f630: r[5]<<=2;goto L8c61f632; /* shll2 r5 */
L8c61f632: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61f634; /* mov.l @r2,r3 */
L8c61f634: r[5]<<=2;goto L8c61f636; /* shll2 r5 */
L8c61f636: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f638; /* fmov.s fr7,@-r8 */
L8c61f638: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61f63a; /* shll r5 */
L8c61f63a: r[5]+=r[3];goto L8c61f63c; /* add r3,r5 */
L8c61f63c: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f63e; /* mov.l @r9,r3 */
L8c61f63e: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f640; /* fmov.s @(r0,r5),fr0 */
L8c61f640: r[0]=20u;goto L8c61f642; /* mov #0x14,r0 */
L8c61f642: r[2]=r[3];goto L8c61f644; /* mov r3,r2 */
L8c61f644: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f646; /* fmov.s @(r0,r2),fr3 */
L8c61f646: r[0]=4u;goto L8c61f648; /* mov #0x4,r0 */
L8c61f648: r[1]=r[3];goto L8c61f64a; /* mov r3,r1 */
L8c61f64a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f64c; /* fmul fr0,fr3 */
L8c61f64c: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f64e; /* fmov.s @r5,fr0 */
L8c61f64e: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f650; /* fmov.s @(r0,r1),fr2 */
L8c61f650: r[0]=8u;goto L8c61f652; /* mov #0x8,r0 */
L8c61f652: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f654; /* fmac fr0,fr2,fr3 */
L8c61f654: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f656; /* fmov.s @(r0,r5),fr0 */
L8c61f656: r[0]=36u;goto L8c61f658; /* mov #0x24,r0 */
L8c61f658: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f65a; /* fmov.s @(r0,r3),fr2 */
L8c61f65a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f65c; /* fmac fr0,fr2,fr3 */
L8c61f65c: fcopy(f,2u,6u);goto L8c61f65e; /* fmov fr6,fr2 */
L8c61f65e: fcopy(f,0u,6u);goto L8c61f660; /* fmov fr6,fr0 */
L8c61f660: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f662; /* fmac fr0,fr3,fr2 */
L8c61f662: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f664; /* fmov.s fr2,@-r8 */
L8c61f664: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f666; /* mov.l @r9,r3 */
L8c61f666: r[0]=4u;goto L8c61f668; /* mov #0x4,r0 */
L8c61f668: r[1]=r[3];goto L8c61f66a; /* mov r3,r1 */
L8c61f66a: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f66c; /* fmov.s @(r0,r5),fr0 */
L8c61f66c: r[0]=16u;goto L8c61f66e; /* mov #0x10,r0 */
L8c61f66e: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f670; /* fmov.s @(r0,r1),fr3 */
L8c61f670: r[2]=r[3];goto L8c61f672; /* mov r3,r2 */
L8c61f672: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f674; /* fmov.s @r2,fr2 */
L8c61f674: r[0]=8u;goto L8c61f676; /* mov #0x8,r0 */
L8c61f676: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f678; /* fmul fr0,fr3 */
L8c61f678: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f67a; /* fmov.s @r5,fr0 */
L8c61f67a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f67c; /* fmac fr0,fr2,fr3 */
L8c61f67c: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f67e; /* fmov.s @(r0,r5),fr0 */
L8c61f67e: r[0]=32u;goto L8c61f680; /* mov #0x20,r0 */
L8c61f680: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f682; /* fmov.s @(r0,r3),fr2 */
L8c61f682: r[0]=24u;goto L8c61f684; /* mov #0x18,r0 */
L8c61f684: r[3]=rd(m,bus,&io_status,0x8c61f7b8u,4);if(io_status)return io_status;goto L8c61f686; /* mov.l 0x8c61f7b8,r3 */
L8c61f686: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f688; /* fmac fr0,fr2,fr3 */
L8c61f688: fcopy(f,0u,6u);goto L8c61f68a; /* fmov fr6,fr0 */
L8c61f68a: fcopy(f,2u,6u);goto L8c61f68c; /* fmov fr6,fr2 */
L8c61f68c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f68e; /* fmac fr0,fr3,fr2 */
L8c61f68e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f690; /* fmov.s fr2,@-r8 */
L8c61f690: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f692; /* fmov.s @(r0,r5),fr3 */
L8c61f692: r[0]=20u;goto L8c61f694; /* mov #0x14,r0 */
L8c61f694: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f696; /* fmov.s fr3,@-r8 */
L8c61f696: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f698; /* fmov.s @(r0,r5),fr3 */
L8c61f698: r[0]=16u;goto L8c61f69a; /* mov #0x10,r0 */
L8c61f69a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f69c; /* fmov.s fr3,@-r8 */
L8c61f69c: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f69e; /* fmov.s @(r0,r5),fr3 */
L8c61f69e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f6a0; /* fmov.s fr3,@-r8 */
L8c61f6a0: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f6a2; /* mov.l r3,@-r8 */
L8c61f6a2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f6a4; /* pref @r8 */
L8c61f6a4: r[6]+=1u;goto L8c61f6a6; /* add #0x1,r6 */
L8c61f6a6: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c61f6a8; /* cmp/ge r7,r6 */
L8c61f6a8: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61f626;goto L8c61f6ac; /* bf/s 0x8c61f626 */
L8c61f6ac: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61f6ae; /* mov.w @r13+,r5 */
L8c61f6ae: r[0]=4u;goto L8c61f6b0; /* mov #0x4,r0 */
L8c61f6b0: r[2]=rd(m,bus,&io_status,0x8c61f7b4u,4);if(io_status)return io_status;goto L8c61f6b2; /* mov.l 0x8c61f7b4,r2 */
L8c61f6b2: r[8]+=28u;goto L8c61f6b4; /* add #0x1c,r8 */
L8c61f6b4: r[5]=r[5]&65535u;goto L8c61f6b6; /* extu.w r5,r5 */
L8c61f6b6: r[5]<<=2;goto L8c61f6b8; /* shll2 r5 */
L8c61f6b8: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61f6ba; /* mov.l @r2,r3 */
L8c61f6ba: r[5]<<=2;goto L8c61f6bc; /* shll2 r5 */
L8c61f6bc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c61f6be; /* fmov.s fr7,@-r8 */
L8c61f6be: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61f6c0; /* shll r5 */
L8c61f6c0: r[5]+=r[3];goto L8c61f6c2; /* add r3,r5 */
L8c61f6c2: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f6c4; /* mov.l @r9,r3 */
L8c61f6c4: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f6c6; /* fmov.s @(r0,r5),fr0 */
L8c61f6c6: r[0]=20u;goto L8c61f6c8; /* mov #0x14,r0 */
L8c61f6c8: r[2]=r[3];goto L8c61f6ca; /* mov r3,r2 */
L8c61f6ca: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f6cc; /* fmov.s @(r0,r2),fr3 */
L8c61f6cc: r[0]=4u;goto L8c61f6ce; /* mov #0x4,r0 */
L8c61f6ce: r[1]=r[3];goto L8c61f6d0; /* mov r3,r1 */
L8c61f6d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f6d2; /* fmul fr0,fr3 */
L8c61f6d2: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f6d4; /* fmov.s @r5,fr0 */
L8c61f6d4: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f6d6; /* fmov.s @(r0,r1),fr2 */
L8c61f6d6: r[0]=8u;goto L8c61f6d8; /* mov #0x8,r0 */
L8c61f6d8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f6da; /* fmac fr0,fr2,fr3 */
L8c61f6da: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f6dc; /* fmov.s @(r0,r5),fr0 */
L8c61f6dc: r[0]=36u;goto L8c61f6de; /* mov #0x24,r0 */
L8c61f6de: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f6e0; /* fmov.s @(r0,r3),fr2 */
L8c61f6e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f6e2; /* fmac fr0,fr2,fr3 */
L8c61f6e2: fcopy(f,2u,6u);goto L8c61f6e4; /* fmov fr6,fr2 */
L8c61f6e4: fcopy(f,0u,6u);goto L8c61f6e6; /* fmov fr6,fr0 */
L8c61f6e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f6e8; /* fmac fr0,fr3,fr2 */
L8c61f6e8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f6ea; /* fmov.s fr2,@-r8 */
L8c61f6ea: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61f6ec; /* mov.l @r9,r3 */
L8c61f6ec: r[0]=4u;goto L8c61f6ee; /* mov #0x4,r0 */
L8c61f6ee: r[1]=r[3];goto L8c61f6f0; /* mov r3,r1 */
L8c61f6f0: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f6f2; /* fmov.s @(r0,r5),fr0 */
L8c61f6f2: r[0]=16u;goto L8c61f6f4; /* mov #0x10,r0 */
L8c61f6f4: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f6f6; /* fmov.s @(r0,r1),fr3 */
L8c61f6f6: r[2]=r[3];goto L8c61f6f8; /* mov r3,r2 */
L8c61f6f8: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61f6fa; /* fmov.s @r2,fr2 */
L8c61f6fa: r[0]=8u;goto L8c61f6fc; /* mov #0x8,r0 */
L8c61f6fc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f6fe; /* fmul fr0,fr3 */
L8c61f6fe: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f700; /* fmov.s @r5,fr0 */
L8c61f700: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f702; /* fmac fr0,fr2,fr3 */
L8c61f702: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f704; /* fmov.s @(r0,r5),fr0 */
L8c61f704: r[0]=32u;goto L8c61f706; /* mov #0x20,r0 */
L8c61f706: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f708; /* fmov.s @(r0,r3),fr2 */
L8c61f708: r[0]=24u;goto L8c61f70a; /* mov #0x18,r0 */
L8c61f70a: r[3]=rd(m,bus,&io_status,0x8c61f7bcu,4);if(io_status)return io_status;goto L8c61f70c; /* mov.l 0x8c61f7bc,r3 */
L8c61f70c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f70e; /* fmac fr0,fr2,fr3 */
L8c61f70e: fcopy(f,0u,6u);goto L8c61f710; /* fmov fr6,fr0 */
L8c61f710: fcopy(f,2u,6u);goto L8c61f712; /* fmov fr6,fr2 */
L8c61f712: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f714; /* fmac fr0,fr3,fr2 */
L8c61f714: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f716; /* fmov.s fr2,@-r8 */
L8c61f716: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f718; /* fmov.s @(r0,r5),fr3 */
L8c61f718: r[0]=20u;goto L8c61f71a; /* mov #0x14,r0 */
L8c61f71a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f71c; /* fmov.s fr3,@-r8 */
L8c61f71c: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f71e; /* fmov.s @(r0,r5),fr3 */
L8c61f71e: r[0]=16u;goto L8c61f720; /* mov #0x10,r0 */
L8c61f720: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f722; /* fmov.s fr3,@-r8 */
L8c61f722: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f724; /* fmov.s @(r0,r5),fr3 */
L8c61f724: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f726; /* fmov.s fr3,@-r8 */
L8c61f726: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61f728; /* mov.l r3,@-r8 */
L8c61f728: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f72a; /* pref @r8 */
L8c61f72a: r[8]+=32u;goto L8c61f72c; /* add #0x20,r8 */
L8c61f72c: r[10]+=1u;goto L8c61f72e; /* add #0x1,r10 */
L8c61f72e: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61f730; /* mov.w @(0x2,r4),r0 */
L8c61f730: r[0]=r[0]&65535u;goto L8c61f732; /* extu.w r0,r0 */
L8c61f732: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c61f734; /* cmp/ge r0,r10 */
L8c61f734: if((*sr&1u))goto L8c61f73a;goto L8c61f736; /* bt 0x8c61f73a */
L8c61f736: goto L8c61f5a0; /* bra 0x8c61f5a0 */
L8c61f73a: goto L8c61f79c; /* bra 0x8c61f79c */
L8c61f73e: r[1]=rd(m,bus,&io_status,0x8c61f7c0u,4);if(io_status)return io_status;goto L8c61f740; /* mov.l 0x8c61f7c0,r1 */
L8c61f740: wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c61f742; /* mov.l r4,@r1 */
L8c61f742: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f744; /* frchg */
L8c61f744: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61f746; /* fmov.s fr0,@-r15 */
L8c61f746: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61f748; /* fmov.s fr1,@-r15 */
L8c61f748: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61f74a; /* fmov.s fr2,@-r15 */
L8c61f74a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61f74c; /* fmov.s fr3,@-r15 */
L8c61f74c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61f74e; /* fmov.s fr4,@-r15 */
L8c61f74e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61f750; /* fmov.s fr5,@-r15 */
L8c61f750: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61f752; /* fmov.s fr6,@-r15 */
L8c61f752: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61f754; /* fmov.s fr7,@-r15 */
L8c61f754: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61f756; /* fmov.s fr8,@-r15 */
L8c61f756: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61f758; /* fmov.s fr9,@-r15 */
L8c61f758: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c61f75a; /* fmov.s fr10,@-r15 */
L8c61f75a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c61f75c; /* fmov.s fr11,@-r15 */
L8c61f75c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c61f75e; /* fmov.s fr12,@-r15 */
L8c61f75e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61f760; /* fmov.s fr13,@-r15 */
L8c61f760: r[0]=0x8c61f7acu;goto L8c61f762; /* mova 0x8c61f7ac,r0 */
L8c61f762: fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c61f764; /* fmov.s @r0,fr12 */
L8c61f764: fcopy(f,13u,12u);goto L8c61f766; /* fmov fr12,fr13 */
L8c61f766: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[12],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61f768; /* fmul fr12,fr0 */
L8c61f768: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61f76a; /* fmul fr12,fr4 */
L8c61f76a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[12],&f->fpscr,&fr[8]);if(status)return status;}goto L8c61f76c; /* fmul fr12,fr8 */
L8c61f76c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[12],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61f76e; /* fmul fr12,fr1 */
L8c61f76e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[12],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61f770; /* fmul fr12,fr5 */
L8c61f770: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[12],&f->fpscr,&fr[9]);if(status)return status;}goto L8c61f772; /* fmul fr12,fr9 */
L8c61f772: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f774; /* frchg */
L8c61f774: r[2]=rd(m,bus,&io_status,0x8c61f7c4u,4);if(io_status)return io_status;goto L8c61f776; /* mov.l 0x8c61f7c4,r2 */
L8c61f776: r[3]=rd(m,bus,&io_status,0x8c61f7c0u,4);if(io_status)return io_status;goto L8c61f778; /* mov.l 0x8c61f7c0,r3 */
L8c61f778: target=r[2];*pr=0x8c61f77cu;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61f77c; /* jsr @r2 */
L8c61f77c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f77e; /* frchg */
L8c61f77e: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f780; /* fmov.s @r15+,fr13 */
L8c61f780: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f782; /* fmov.s @r15+,fr12 */
L8c61f782: fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f784; /* fmov.s @r15+,fr11 */
L8c61f784: fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f786; /* fmov.s @r15+,fr10 */
L8c61f786: fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f788; /* fmov.s @r15+,fr9 */
L8c61f788: fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f78a; /* fmov.s @r15+,fr8 */
L8c61f78a: fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f78c; /* fmov.s @r15+,fr7 */
L8c61f78c: fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f78e; /* fmov.s @r15+,fr6 */
L8c61f78e: fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f790; /* fmov.s @r15+,fr5 */
L8c61f790: fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f792; /* fmov.s @r15+,fr4 */
L8c61f792: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f794; /* fmov.s @r15+,fr3 */
L8c61f794: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f796; /* fmov.s @r15+,fr2 */
L8c61f796: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f798; /* fmov.s @r15+,fr1 */
L8c61f798: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f79a; /* fmov.s @r15+,fr0 */
L8c61f79a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f79c; /* frchg */
L8c61f79c: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f79e; /* lds.l @r15+,PR */
L8c61f79e: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f7a0; /* mov.l @r15+,r9 */
L8c61f7a0: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f7a2; /* mov.l @r15+,r10 */
L8c61f7a2: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f7a4; /* mov.l @r15+,r11 */
L8c61f7a4: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f7a6; /* mov.l @r15+,r12 */
L8c61f7a6: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f7a8; /* mov.l @r15+,r13 */
L8c61f7a8: if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
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
L8c61f7c8: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61f7ca; /* mov.l r14,@-r15 */
L8c61f7ca: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61f7cc; /* mov.l r13,@-r15 */
L8c61f7cc: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61f7ce; /* mov.l r12,@-r15 */
L8c61f7ce: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61f7d0; /* mov.l r11,@-r15 */
L8c61f7d0: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61f7d2; /* mov.l r10,@-r15 */
L8c61f7d2: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61f7d4; /* mov.l r9,@-r15 */
L8c61f7d4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61f7d6; /* fmov.s fr15,@-r15 */
L8c61f7d6: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61f7d8; /* fmov.s fr14,@-r15 */
L8c61f7d8: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61f7da; /* sts.l PR,@-r15 */
L8c61f7da: r[15]+=4294967288u;goto L8c61f7dc; /* add #-0x8,r15 */
L8c61f7dc: r[0]=0x8c61f834u;goto L8c61f7de; /* mova 0x8c61f834,r0 */
L8c61f7de: r[3]=rd(m,bus,&io_status,0x8c61f838u,4);if(io_status)return io_status;goto L8c61f7e0; /* mov.l 0x8c61f838,r3 */
L8c61f7e0: fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c61f7e2; /* fmov.s @r0,fr15 */
L8c61f7e2: r[0]=r[4];goto L8c61f7e4; /* mov r4,r0 */
L8c61f7e4: r[11]=r[4];goto L8c61f7e6; /* mov r4,r11 */
L8c61f7e6: wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61f7e8; /* mov.l r4,@r15 */
L8c61f7e8: r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61f7ea; /* mov.w @r0,r0 */
L8c61f7ea: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][14]=0x3f800000u;goto L8c61f7ec; /* fldi1 fr14 */
L8c61f7ec: r[0]=r[0]&65535u;goto L8c61f7ee; /* extu.w r0,r0 */
L8c61f7ee: r[0]&=r[3];goto L8c61f7f0; /* and r3,r0 */
L8c61f7f0: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61f7f2; /* cmp/eq #0x0,r0 */
L8c61f7f2: take=(*sr&1u);r[11]=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;if(take)goto L8c61f818;goto L8c61f7f6; /* bt/s 0x8c61f818 */
L8c61f7f6: r[1]=sx(m,bus,&io_status,0x8c61f830u,2);if(io_status)return io_status;goto L8c61f7f8; /* mov.w 0x8c61f830,r1 */
L8c61f7f8: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61f7fa; /* cmp/eq r1,r0 */
L8c61f7fa: if(!(*sr&1u))goto L8c61f800;goto L8c61f7fc; /* bf 0x8c61f800 */
L8c61f7fc: goto L8c61fa26; /* bra 0x8c61fa26 */
L8c61f800: r[1]=rd(m,bus,&io_status,0x8c61f83cu,4);if(io_status)return io_status;goto L8c61f802; /* mov.l 0x8c61f83c,r1 */
L8c61f802: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61f804; /* cmp/eq r1,r0 */
L8c61f804: if(!(*sr&1u))goto L8c61f80a;goto L8c61f806; /* bf 0x8c61f80a */
L8c61f806: goto L8c61fcfc; /* bra 0x8c61fcfc */
L8c61f80a: r[1]=rd(m,bus,&io_status,0x8c61f838u,4);if(io_status)return io_status;goto L8c61f80c; /* mov.l 0x8c61f838,r1 */
L8c61f80c: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61f80e; /* cmp/eq r1,r0 */
L8c61f80e: if(!(*sr&1u))goto L8c61f814;goto L8c61f810; /* bf 0x8c61f814 */
L8c61f810: goto L8c61ff3e; /* bra 0x8c61ff3e */
L8c61f814: goto L8c61ff9e; /* bra 0x8c61ff9e */
L8c61f818: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61f81a; /* mov.l @r15,r2 */
L8c61f81a: r[1]=rd(m,bus,&io_status,0x8c61f840u,4);if(io_status)return io_status;goto L8c61f81c; /* mov.l 0x8c61f840,r1 */
L8c61f81c: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61f81e; /* mov.w @(0x2,r2),r0 */
L8c61f81e: r[0]=r[0]&65535u;goto L8c61f820; /* extu.w r0,r0 */
L8c61f820: r[2]=r[0];goto L8c61f822; /* mov r0,r2 */
L8c61f822: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61f824; /* shll r0 */
L8c61f824: r[0]+=r[2];goto L8c61f826; /* add r2,r0 */
L8c61f826: r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61f828; /* mov.l @r1,r2 */
L8c61f828: r[2]+=r[0];goto L8c61f82a; /* add r0,r2 */
L8c61f82a: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61f82c; /* mov.l r2,@r1 */
L8c61f82c: r[14]=0u;goto L8c61fa14; /* bra 0x8c61fa14 */
L8c61f844: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61f846; /* mov.w @r11+,r4 */
L8c61f846: r[3]=rd(m,bus,&io_status,0x8c61fa40u,4);if(io_status)return io_status;goto L8c61f848; /* mov.l 0x8c61fa40,r3 */
L8c61f848: r[5]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61f84a; /* mov.w @r11+,r5 */
L8c61f84a: r[4]=r[4]&65535u;goto L8c61f84c; /* extu.w r4,r4 */
L8c61f84c: r[4]<<=2;goto L8c61f84e; /* shll2 r4 */
L8c61f84e: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f850; /* mov.l @r3,r13 */
L8c61f850: r[6]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61f852; /* mov.w @r11+,r6 */
L8c61f852: r[5]=r[5]&65535u;goto L8c61f854; /* extu.w r5,r5 */
L8c61f854: r[5]<<=2;goto L8c61f856; /* shll2 r5 */
L8c61f856: r[6]=r[6]&65535u;goto L8c61f858; /* extu.w r6,r6 */
L8c61f858: r[5]<<=2;goto L8c61f85a; /* shll2 r5 */
L8c61f85a: r[6]<<=2;goto L8c61f85c; /* shll2 r6 */
L8c61f85c: r[4]<<=2;goto L8c61f85e; /* shll2 r4 */
L8c61f85e: r[6]<<=2;goto L8c61f860; /* shll2 r6 */
L8c61f860: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61f862; /* shll r4 */
L8c61f862: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61f864; /* shll r5 */
L8c61f864: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61f866; /* shll r6 */
L8c61f866: r[4]+=r[13];goto L8c61f868; /* add r13,r4 */
L8c61f868: r[5]+=r[13];goto L8c61f86a; /* add r13,r5 */
L8c61f86a: r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61f86c; /* mov.l @(0xc,r4),r7 */
L8c61f86c: r[2]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c61f86e; /* mov.l @(0xc,r5),r2 */
L8c61f86e: r[6]+=r[13];goto L8c61f870; /* add r13,r6 */
L8c61f870: r[1]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c61f872; /* mov.l @(0xc,r6),r1 */
L8c61f872: r[7]&=r[2];goto L8c61f874; /* and r2,r7 */
L8c61f874: r[2]=63u;goto L8c61f876; /* mov #0x3f,r2 */
L8c61f876: r[7]&=r[1];goto L8c61f878; /* and r1,r7 */
L8c61f878: *sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c61f87a; /* tst r2,r7 */
L8c61f87a: if((*sr&1u))goto L8c61f880;goto L8c61f87c; /* bt 0x8c61f880 */
L8c61f87c: goto L8c61fa12; /* bra 0x8c61fa12 */
L8c61f880: r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61f882; /* mov.l @(0xc,r4),r7 */
L8c61f882: r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c61f884; /* mov.l @(0xc,r5),r3 */
L8c61f884: r[2]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c61f886; /* mov.l @(0xc,r6),r2 */
L8c61f886: r[7]|=r[3];goto L8c61f888; /* or r3,r7 */
L8c61f888: r[3]=1u;goto L8c61f88a; /* mov #0x1,r3 */
L8c61f88a: r[7]|=r[2];goto L8c61f88c; /* or r2,r7 */
L8c61f88c: *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c61f88e; /* tst r3,r7 */
L8c61f88e: if((*sr&1u))goto L8c61f894;goto L8c61f890; /* bt 0x8c61f894 */
L8c61f890: goto L8c61fa08; /* bra 0x8c61fa08 */
L8c61f894: r[0]=16u;goto L8c61f896; /* mov #0x10,r0 */
L8c61f896: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f898; /* fmov.s @(r0,r5),fr5 */
L8c61f898: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f89a; /* fmov.s @(r0,r4),fr3 */
L8c61f89a: r[0]=20u;goto L8c61f89c; /* mov #0x14,r0 */
L8c61f89c: fcopy(f,2u,5u);goto L8c61f89e; /* fmov fr5,fr2 */
L8c61f89e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61f8a0; /* fsub fr3,fr2 */
L8c61f8a0: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f8a2; /* fmov.s @(r0,r6),fr3 */
L8c61f8a2: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f8a4; /* fmov.s @(r0,r5),fr4 */
L8c61f8a4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f8a6; /* fsub fr4,fr3 */
L8c61f8a6: fcopy(f,1u,4u);goto L8c61f8a8; /* fmov fr4,fr1 */
L8c61f8a8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f8aa; /* fmul fr3,fr2 */
L8c61f8aa: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f8ac; /* fmov.s @(r0,r4),fr3 */
L8c61f8ac: r[0]=16u;goto L8c61f8ae; /* mov #0x10,r0 */
L8c61f8ae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61f8b0; /* fsub fr3,fr1 */
L8c61f8b0: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f8b2; /* fmov.s @(r0,r6),fr3 */
L8c61f8b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61f8b4; /* fsub fr5,fr3 */
L8c61f8b4: fcopy(f,4u,2u);goto L8c61f8b6; /* fmov fr2,fr4 */
L8c61f8b6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61f8b8; /* fmul fr3,fr1 */
L8c61f8b8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61f8ba; /* fldi0 fr3 */
L8c61f8ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61f8bc; /* fsub fr1,fr4 */
L8c61f8bc: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61f8be; /* fcmp/gt fr3,fr4 */
L8c61f8be: if(!(*sr&1u))goto L8c61f8c4;goto L8c61f8c0; /* bf 0x8c61f8c4 */
L8c61f8c0: goto L8c61fa12; /* bra 0x8c61fa12 */
L8c61f8c4: r[2]=rd(m,bus,&io_status,0x8c61fa44u,4);if(io_status)return io_status;goto L8c61f8c6; /* mov.l 0x8c61fa44,r2 */
L8c61f8c6: r[0]=4u;goto L8c61f8c8; /* mov #0x4,r0 */
L8c61f8c8: r[8]+=28u;goto L8c61f8ca; /* add #0x1c,r8 */
L8c61f8ca: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61f8cc; /* fmov.s fr14,@-r8 */
L8c61f8cc: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61f8ce; /* mov.l @r2,r3 */
L8c61f8ce: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f8d0; /* fmov.s @(r0,r4),fr0 */
L8c61f8d0: r[0]=20u;goto L8c61f8d2; /* mov #0x14,r0 */
L8c61f8d2: r[2]=r[3];goto L8c61f8d4; /* mov r3,r2 */
L8c61f8d4: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61f8d6; /* fmov.s @(r0,r2),fr3 */
L8c61f8d6: r[1]=r[3];goto L8c61f8d8; /* mov r3,r1 */
L8c61f8d8: r[0]=4u;goto L8c61f8da; /* mov #0x4,r0 */
L8c61f8da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f8dc; /* fmul fr0,fr3 */
L8c61f8dc: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61f8de; /* fmov.s @(r0,r1),fr2 */
L8c61f8de: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61f8e0; /* fmov.s @r4,fr0 */
L8c61f8e0: r[0]=8u;goto L8c61f8e2; /* mov #0x8,r0 */
L8c61f8e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f8e4; /* fmac fr0,fr2,fr3 */
L8c61f8e4: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f8e6; /* fmov.s @(r0,r4),fr0 */
L8c61f8e6: r[0]=36u;goto L8c61f8e8; /* mov #0x24,r0 */
L8c61f8e8: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61f8ea; /* fmov.s @(r0,r3),fr2 */
L8c61f8ea: r[0]=4u;goto L8c61f8ec; /* mov #0x4,r0 */
L8c61f8ec: r[3]=rd(m,bus,&io_status,0x8c61fa44u,4);if(io_status)return io_status;goto L8c61f8ee; /* mov.l 0x8c61fa44,r3 */
L8c61f8ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f8f0; /* fmac fr0,fr2,fr3 */
L8c61f8f0: fcopy(f,2u,15u);goto L8c61f8f2; /* fmov fr15,fr2 */
L8c61f8f2: fcopy(f,0u,15u);goto L8c61f8f4; /* fmov fr15,fr0 */
L8c61f8f4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f8f6; /* fmac fr0,fr3,fr2 */
L8c61f8f6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f8f8; /* fmov.s fr2,@-r8 */
L8c61f8f8: r[9]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f8fa; /* mov.l @r3,r9 */
L8c61f8fa: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f8fc; /* fmov.s @(r0,r4),fr0 */
L8c61f8fc: r[0]=16u;goto L8c61f8fe; /* mov #0x10,r0 */
L8c61f8fe: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61f900; /* fmov.s @(r0,r9),fr3 */
L8c61f900: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f902; /* fmul fr0,fr3 */
L8c61f902: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61f904; /* fmov.s @r4,fr0 */
L8c61f904: fload(m,bus,&io_status,f,2u,r[9]);if(io_status)return io_status;goto L8c61f906; /* fmov.s @r9,fr2 */
L8c61f906: r[0]=8u;goto L8c61f908; /* mov #0x8,r0 */
L8c61f908: r[2]=rd(m,bus,&io_status,0x8c61fa48u,4);if(io_status)return io_status;goto L8c61f90a; /* mov.l 0x8c61fa48,r2 */
L8c61f90a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f90c; /* fmac fr0,fr2,fr3 */
L8c61f90c: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f90e; /* fmov.s @(r0,r4),fr0 */
L8c61f90e: r[0]=32u;goto L8c61f910; /* mov #0x20,r0 */
L8c61f910: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61f912; /* fmov.s @(r0,r9),fr2 */
L8c61f912: r[0]=24u;goto L8c61f914; /* mov #0x18,r0 */
L8c61f914: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f916; /* fmac fr0,fr2,fr3 */
L8c61f916: fcopy(f,0u,15u);goto L8c61f918; /* fmov fr15,fr0 */
L8c61f918: fcopy(f,2u,15u);goto L8c61f91a; /* fmov fr15,fr2 */
L8c61f91a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f91c; /* fmac fr0,fr3,fr2 */
L8c61f91c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f91e; /* fmov.s fr2,@-r8 */
L8c61f91e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f920; /* fmov.s @(r0,r4),fr3 */
L8c61f920: r[0]=20u;goto L8c61f922; /* mov #0x14,r0 */
L8c61f922: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f924; /* fmov.s fr3,@-r8 */
L8c61f924: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f926; /* fmov.s @(r0,r4),fr3 */
L8c61f926: r[0]=16u;goto L8c61f928; /* mov #0x10,r0 */
L8c61f928: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f92a; /* fmov.s fr3,@-r8 */
L8c61f92a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61f92c; /* fmov.s @(r0,r4),fr3 */
L8c61f92c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f92e; /* fmov.s fr3,@-r8 */
L8c61f92e: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61f930; /* mov.l r2,@-r8 */
L8c61f930: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f932; /* pref @r8 */
L8c61f932: r[0]=4u;goto L8c61f934; /* mov #0x4,r0 */
L8c61f934: r[8]+=60u;goto L8c61f936; /* add #0x3c,r8 */
L8c61f936: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61f938; /* fmov.s fr14,@-r8 */
L8c61f938: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f93a; /* fmov.s @(r0,r5),fr0 */
L8c61f93a: r[0]=20u;goto L8c61f93c; /* mov #0x14,r0 */
L8c61f93c: r[10]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f93e; /* mov.l @r3,r10 */
L8c61f93e: fload(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c61f940; /* fmov.s @(r0,r10),fr3 */
L8c61f940: r[0]=4u;goto L8c61f942; /* mov #0x4,r0 */
L8c61f942: fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c61f944; /* fmov.s @(r0,r10),fr2 */
L8c61f944: r[0]=8u;goto L8c61f946; /* mov #0x8,r0 */
L8c61f946: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f948; /* fmul fr0,fr3 */
L8c61f948: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f94a; /* fmov.s @r5,fr0 */
L8c61f94a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f94c; /* fmac fr0,fr2,fr3 */
L8c61f94c: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f94e; /* fmov.s @(r0,r5),fr0 */
L8c61f94e: r[0]=36u;goto L8c61f950; /* mov #0x24,r0 */
L8c61f950: fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c61f952; /* fmov.s @(r0,r10),fr2 */
L8c61f952: r[0]=4u;goto L8c61f954; /* mov #0x4,r0 */
L8c61f954: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f956; /* fmac fr0,fr2,fr3 */
L8c61f956: fcopy(f,2u,15u);goto L8c61f958; /* fmov fr15,fr2 */
L8c61f958: fcopy(f,0u,15u);goto L8c61f95a; /* fmov fr15,fr0 */
L8c61f95a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f95c; /* fmac fr0,fr3,fr2 */
L8c61f95c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f95e; /* fmov.s fr2,@-r8 */
L8c61f95e: r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f960; /* mov.l @r3,r12 */
L8c61f960: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f962; /* fmov.s @(r0,r5),fr0 */
L8c61f962: r[0]=16u;goto L8c61f964; /* mov #0x10,r0 */
L8c61f964: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61f966; /* fmov.s @(r0,r12),fr3 */
L8c61f966: r[0]=8u;goto L8c61f968; /* mov #0x8,r0 */
L8c61f968: fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61f96a; /* fmov.s @r12,fr2 */
L8c61f96a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f96c; /* fmul fr0,fr3 */
L8c61f96c: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61f96e; /* fmov.s @r5,fr0 */
L8c61f96e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f970; /* fmac fr0,fr2,fr3 */
L8c61f970: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f972; /* fmov.s @(r0,r5),fr0 */
L8c61f972: r[0]=32u;goto L8c61f974; /* mov #0x20,r0 */
L8c61f974: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61f976; /* fmov.s @(r0,r12),fr2 */
L8c61f976: r[0]=24u;goto L8c61f978; /* mov #0x18,r0 */
L8c61f978: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f97a; /* fmac fr0,fr2,fr3 */
L8c61f97a: fcopy(f,0u,15u);goto L8c61f97c; /* fmov fr15,fr0 */
L8c61f97c: fcopy(f,2u,15u);goto L8c61f97e; /* fmov fr15,fr2 */
L8c61f97e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f980; /* fmac fr0,fr3,fr2 */
L8c61f980: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f982; /* fmov.s fr2,@-r8 */
L8c61f982: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f984; /* fmov.s @(r0,r5),fr3 */
L8c61f984: r[0]=20u;goto L8c61f986; /* mov #0x14,r0 */
L8c61f986: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f988; /* fmov.s fr3,@-r8 */
L8c61f988: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f98a; /* fmov.s @(r0,r5),fr3 */
L8c61f98a: r[0]=16u;goto L8c61f98c; /* mov #0x10,r0 */
L8c61f98c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f98e; /* fmov.s fr3,@-r8 */
L8c61f98e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61f990; /* fmov.s @(r0,r5),fr3 */
L8c61f990: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f992; /* fmov.s fr3,@-r8 */
L8c61f992: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61f994; /* mov.l r2,@-r8 */
L8c61f994: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f996; /* pref @r8 */
L8c61f996: r[0]=4u;goto L8c61f998; /* mov #0x4,r0 */
L8c61f998: r[8]+=60u;goto L8c61f99a; /* add #0x3c,r8 */
L8c61f99a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61f99c; /* fmov.s fr14,@-r8 */
L8c61f99c: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f99e; /* fmov.s @(r0,r6),fr0 */
L8c61f99e: r[0]=20u;goto L8c61f9a0; /* mov #0x14,r0 */
L8c61f9a0: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f9a2; /* mov.l @r3,r13 */
L8c61f9a2: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61f9a4; /* fmov.s @(r0,r13),fr3 */
L8c61f9a4: r[0]=4u;goto L8c61f9a6; /* mov #0x4,r0 */
L8c61f9a6: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61f9a8; /* fmov.s @(r0,r13),fr2 */
L8c61f9a8: r[0]=8u;goto L8c61f9aa; /* mov #0x8,r0 */
L8c61f9aa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f9ac; /* fmul fr0,fr3 */
L8c61f9ac: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61f9ae; /* fmov.s @r6,fr0 */
L8c61f9ae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f9b0; /* fmac fr0,fr2,fr3 */
L8c61f9b0: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f9b2; /* fmov.s @(r0,r6),fr0 */
L8c61f9b2: r[0]=36u;goto L8c61f9b4; /* mov #0x24,r0 */
L8c61f9b4: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61f9b6; /* fmov.s @(r0,r13),fr2 */
L8c61f9b6: r[0]=4u;goto L8c61f9b8; /* mov #0x4,r0 */
L8c61f9b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f9ba; /* fmac fr0,fr2,fr3 */
L8c61f9ba: fcopy(f,2u,15u);goto L8c61f9bc; /* fmov fr15,fr2 */
L8c61f9bc: fcopy(f,0u,15u);goto L8c61f9be; /* fmov fr15,fr0 */
L8c61f9be: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f9c0; /* fmac fr0,fr3,fr2 */
L8c61f9c0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f9c2; /* fmov.s fr2,@-r8 */
L8c61f9c2: r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61f9c4; /* mov.l @r3,r7 */
L8c61f9c4: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f9c6; /* fmov.s @(r0,r6),fr0 */
L8c61f9c6: r[0]=16u;goto L8c61f9c8; /* mov #0x10,r0 */
L8c61f9c8: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f9ca; /* fmov.s @(r0,r7),fr3 */
L8c61f9ca: r[0]=8u;goto L8c61f9cc; /* mov #0x8,r0 */
L8c61f9cc: fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61f9ce; /* fmov.s @r7,fr2 */
L8c61f9ce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f9d0; /* fmul fr0,fr3 */
L8c61f9d0: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61f9d2; /* fmov.s @r6,fr0 */
L8c61f9d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f9d4; /* fmac fr0,fr2,fr3 */
L8c61f9d4: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f9d6; /* fmov.s @(r0,r6),fr0 */
L8c61f9d6: r[0]=32u;goto L8c61f9d8; /* mov #0x20,r0 */
L8c61f9d8: r[2]=rd(m,bus,&io_status,0x8c61fa4cu,4);if(io_status)return io_status;goto L8c61f9da; /* mov.l 0x8c61fa4c,r2 */
L8c61f9da: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61f9dc; /* fmov.s @(r0,r7),fr2 */
L8c61f9dc: r[0]=24u;goto L8c61f9de; /* mov #0x18,r0 */
L8c61f9de: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61f9e0; /* fmac fr0,fr2,fr3 */
L8c61f9e0: fcopy(f,0u,15u);goto L8c61f9e2; /* fmov fr15,fr0 */
L8c61f9e2: fcopy(f,2u,15u);goto L8c61f9e4; /* fmov fr15,fr2 */
L8c61f9e4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61f9e6; /* fmac fr0,fr3,fr2 */
L8c61f9e6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61f9e8; /* fmov.s fr2,@-r8 */
L8c61f9e8: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f9ea; /* fmov.s @(r0,r6),fr3 */
L8c61f9ea: r[0]=20u;goto L8c61f9ec; /* mov #0x14,r0 */
L8c61f9ec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f9ee; /* fmov.s fr3,@-r8 */
L8c61f9ee: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f9f0; /* fmov.s @(r0,r6),fr3 */
L8c61f9f0: r[0]=16u;goto L8c61f9f2; /* mov #0x10,r0 */
L8c61f9f2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f9f4; /* fmov.s fr3,@-r8 */
L8c61f9f4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61f9f6; /* fmov.s @(r0,r6),fr3 */
L8c61f9f6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61f9f8; /* fmov.s fr3,@-r8 */
L8c61f9f8: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61f9fa; /* mov.l r2,@-r8 */
L8c61f9fa: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61f9fc; /* pref @r8 */
L8c61f9fc: r[1]=rd(m,bus,&io_status,0x8c61fa50u,4);if(io_status)return io_status;goto L8c61f9fe; /* mov.l 0x8c61fa50,r1 */
L8c61f9fe: r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61fa00; /* mov.l @r1,r2 */
L8c61fa00: r[2]+=3u;goto L8c61fa02; /* add #0x3,r2 */
L8c61fa02: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61fa04; /* mov.l r2,@r1 */
L8c61fa04: r[8]+=32u;goto L8c61fa12; /* bra 0x8c61fa12 */
L8c61fa08: r[0]=rd(m,bus,&io_status,0x8c61fa54u,4);if(io_status)return io_status;goto L8c61fa0a; /* mov.l 0x8c61fa54,r0 */
L8c61fa0a: wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61fa0c; /* mov.l r3,@r0 */
L8c61fa0c: r[3]=rd(m,bus,&io_status,0x8c61fa58u,4);if(io_status)return io_status;goto L8c61fa0e; /* mov.l 0x8c61fa58,r3 */
L8c61fa0e: target=r[3];*pr=0x8c61fa12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61fa12; /* jsr @r3 */
L8c61fa12: r[14]+=1u;goto L8c61fa14; /* add #0x1,r14 */
L8c61fa14: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61fa16; /* mov.l @r15,r2 */
L8c61fa16: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61fa18; /* mov.w @(0x2,r2),r0 */
L8c61fa18: r[0]=r[0]&65535u;goto L8c61fa1a; /* extu.w r0,r0 */
L8c61fa1a: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c61fa1c; /* cmp/ge r0,r14 */
L8c61fa1c: if((*sr&1u))goto L8c61fa22;goto L8c61fa1e; /* bt 0x8c61fa22 */
L8c61fa1e: goto L8c61f844; /* bra 0x8c61f844 */
L8c61fa22: goto L8c61ff9e; /* bra 0x8c61ff9e */
L8c61fa26: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61fa28; /* mov.l @r15,r1 */
L8c61fa28: r[2]=rd(m,bus,&io_status,0x8c61fa5cu,4);if(io_status)return io_status;goto L8c61fa2a; /* mov.l 0x8c61fa5c,r2 */
L8c61fa2a: r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61fa2c; /* mov.w @(0x2,r1),r0 */
L8c61fa2c: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fa2e; /* mov.l @r2,r1 */
L8c61fa2e: r[0]=r[0]&65535u;goto L8c61fa30; /* extu.w r0,r0 */
L8c61fa30: r[0]<<=2;goto L8c61fa32; /* shll2 r0 */
L8c61fa32: r[1]+=r[0];goto L8c61fa34; /* add r0,r1 */
L8c61fa34: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61fa36; /* mov.l r1,@r2 */
L8c61fa36: r[10]=0u;goto L8c61fcea; /* bra 0x8c61fcea */
L8c61fa60: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fa62; /* mov.w @r11+,r4 */
L8c61fa62: r[3]=rd(m,bus,&io_status,0x8c61fd00u,4);if(io_status)return io_status;goto L8c61fa64; /* mov.l 0x8c61fd00,r3 */
L8c61fa64: r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fa66; /* mov.w @r11+,r13 */
L8c61fa66: r[4]=r[4]&65535u;goto L8c61fa68; /* extu.w r4,r4 */
L8c61fa68: r[4]<<=2;goto L8c61fa6a; /* shll2 r4 */
L8c61fa6a: r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fa6c; /* mov.l @r3,r6 */
L8c61fa6c: r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fa6e; /* mov.w @r11+,r14 */
L8c61fa6e: r[13]=r[13]&65535u;goto L8c61fa70; /* extu.w r13,r13 */
L8c61fa70: r[13]<<=2;goto L8c61fa72; /* shll2 r13 */
L8c61fa72: r[12]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fa74; /* mov.w @r11+,r12 */
L8c61fa74: r[14]=r[14]&65535u;goto L8c61fa76; /* extu.w r14,r14 */
L8c61fa76: r[13]<<=2;goto L8c61fa78; /* shll2 r13 */
L8c61fa78: r[4]<<=2;goto L8c61fa7a; /* shll2 r4 */
L8c61fa7a: r[12]=r[12]&65535u;goto L8c61fa7c; /* extu.w r12,r12 */
L8c61fa7c: r[14]<<=2;goto L8c61fa7e; /* shll2 r14 */
L8c61fa7e: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61fa80; /* shll r4 */
L8c61fa80: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61fa82; /* shll r13 */
L8c61fa82: r[4]+=r[6];goto L8c61fa84; /* add r6,r4 */
L8c61fa84: r[12]<<=2;goto L8c61fa86; /* shll2 r12 */
L8c61fa86: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61fa88; /* mov.l @(0xc,r4),r5 */
L8c61fa88: r[13]+=r[6];goto L8c61fa8a; /* add r6,r13 */
L8c61fa8a: r[14]<<=2;goto L8c61fa8c; /* shll2 r14 */
L8c61fa8c: r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61fa8e; /* mov.l @(0xc,r13),r2 */
L8c61fa8e: r[12]<<=2;goto L8c61fa90; /* shll2 r12 */
L8c61fa90: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61fa92; /* shll r14 */
L8c61fa92: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61fa94; /* shll r12 */
L8c61fa94: r[14]+=r[6];goto L8c61fa96; /* add r6,r14 */
L8c61fa96: r[1]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61fa98; /* mov.l @(0xc,r14),r1 */
L8c61fa98: r[5]&=r[2];goto L8c61fa9a; /* and r2,r5 */
L8c61fa9a: r[12]+=r[6];goto L8c61fa9c; /* add r6,r12 */
L8c61fa9c: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61fa9e; /* mov.l @(0xc,r12),r2 */
L8c61fa9e: r[5]&=r[1];goto L8c61faa0; /* and r1,r5 */
L8c61faa0: r[1]=63u;goto L8c61faa2; /* mov #0x3f,r1 */
L8c61faa2: r[5]&=r[2];goto L8c61faa4; /* and r2,r5 */
L8c61faa4: *sr=(*sr&~1u)|!(r[1]&r[5]);goto L8c61faa6; /* tst r1,r5 */
L8c61faa6: if((*sr&1u))goto L8c61faac;goto L8c61faa8; /* bt 0x8c61faac */
L8c61faa8: goto L8c61fce8; /* bra 0x8c61fce8 */
L8c61faac: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61faae; /* mov.l @(0xc,r4),r5 */
L8c61faae: r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61fab0; /* mov.l @(0xc,r13),r3 */
L8c61fab0: r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61fab2; /* mov.l @(0xc,r14),r2 */
L8c61fab2: r[5]|=r[3];goto L8c61fab4; /* or r3,r5 */
L8c61fab4: r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61fab6; /* mov.l @(0xc,r12),r3 */
L8c61fab6: r[5]|=r[2];goto L8c61fab8; /* or r2,r5 */
L8c61fab8: r[2]=1u;goto L8c61faba; /* mov #0x1,r2 */
L8c61faba: r[5]|=r[3];goto L8c61fabc; /* or r3,r5 */
L8c61fabc: *sr=(*sr&~1u)|!(r[2]&r[5]);goto L8c61fabe; /* tst r2,r5 */
L8c61fabe: if((*sr&1u))goto L8c61fac4;goto L8c61fac0; /* bt 0x8c61fac4 */
L8c61fac0: goto L8c61fcb2; /* bra 0x8c61fcb2 */
L8c61fac4: r[0]=16u;goto L8c61fac6; /* mov #0x10,r0 */
L8c61fac6: fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fac8; /* fmov.s @(r0,r13),fr4 */
L8c61fac8: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61faca; /* fmov.s @(r0,r4),fr3 */
L8c61faca: r[0]=20u;goto L8c61facc; /* mov #0x14,r0 */
L8c61facc: fcopy(f,2u,4u);goto L8c61face; /* fmov fr4,fr2 */
L8c61face: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61fad0; /* fsub fr3,fr2 */
L8c61fad0: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fad2; /* fmov.s @(r0,r14),fr3 */
L8c61fad2: fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fad4; /* fmov.s @(r0,r13),fr5 */
L8c61fad4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61fad6; /* fsub fr5,fr3 */
L8c61fad6: fcopy(f,1u,5u);goto L8c61fad8; /* fmov fr5,fr1 */
L8c61fad8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fada; /* fmul fr3,fr2 */
L8c61fada: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fadc; /* fmov.s @(r0,r4),fr3 */
L8c61fadc: r[0]=16u;goto L8c61fade; /* mov #0x10,r0 */
L8c61fade: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61fae0; /* fsub fr3,fr1 */
L8c61fae0: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fae2; /* fmov.s @(r0,r14),fr3 */
L8c61fae2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61fae4; /* fsub fr4,fr3 */
L8c61fae4: fcopy(f,4u,2u);goto L8c61fae6; /* fmov fr2,fr4 */
L8c61fae6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61fae8; /* fmul fr3,fr1 */
L8c61fae8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61faea; /* fldi0 fr3 */
L8c61faea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61faec; /* fsub fr1,fr4 */
L8c61faec: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61faee; /* fcmp/gt fr3,fr4 */
L8c61faee: if(!(*sr&1u))goto L8c61faf4;goto L8c61faf0; /* bf 0x8c61faf4 */
L8c61faf0: goto L8c61fce8; /* bra 0x8c61fce8 */
L8c61faf4: r[2]=rd(m,bus,&io_status,0x8c61fd04u,4);if(io_status)return io_status;goto L8c61faf6; /* mov.l 0x8c61fd04,r2 */
L8c61faf6: r[0]=4u;goto L8c61faf8; /* mov #0x4,r0 */
L8c61faf8: r[8]+=28u;goto L8c61fafa; /* add #0x1c,r8 */
L8c61fafa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61fafc; /* fmov.s fr14,@-r8 */
L8c61fafc: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fafe; /* mov.l @r2,r3 */
L8c61fafe: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fb00; /* fmov.s @(r0,r4),fr0 */
L8c61fb00: r[0]=20u;goto L8c61fb02; /* mov #0x14,r0 */
L8c61fb02: r[2]=r[3];goto L8c61fb04; /* mov r3,r2 */
L8c61fb04: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61fb06; /* fmov.s @(r0,r2),fr3 */
L8c61fb06: r[1]=r[3];goto L8c61fb08; /* mov r3,r1 */
L8c61fb08: r[0]=4u;goto L8c61fb0a; /* mov #0x4,r0 */
L8c61fb0a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb0c; /* fmul fr0,fr3 */
L8c61fb0c: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61fb0e; /* fmov.s @(r0,r1),fr2 */
L8c61fb0e: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61fb10; /* fmov.s @r4,fr0 */
L8c61fb10: r[0]=8u;goto L8c61fb12; /* mov #0x8,r0 */
L8c61fb12: r[2]=rd(m,bus,&io_status,0x8c61fd04u,4);if(io_status)return io_status;goto L8c61fb14; /* mov.l 0x8c61fd04,r2 */
L8c61fb14: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb16; /* fmac fr0,fr2,fr3 */
L8c61fb16: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fb18; /* fmov.s @(r0,r4),fr0 */
L8c61fb18: r[0]=36u;goto L8c61fb1a; /* mov #0x24,r0 */
L8c61fb1a: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61fb1c; /* fmov.s @(r0,r3),fr2 */
L8c61fb1c: r[0]=4u;goto L8c61fb1e; /* mov #0x4,r0 */
L8c61fb1e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb20; /* fmac fr0,fr2,fr3 */
L8c61fb20: fcopy(f,0u,15u);goto L8c61fb22; /* fmov fr15,fr0 */
L8c61fb22: fcopy(f,2u,15u);goto L8c61fb24; /* fmov fr15,fr2 */
L8c61fb24: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fb26; /* fmac fr0,fr3,fr2 */
L8c61fb26: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fb28; /* fmov.s fr2,@-r8 */
L8c61fb28: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fb2a; /* mov.l @r2,r3 */
L8c61fb2a: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fb2c; /* fmov.s @(r0,r4),fr0 */
L8c61fb2c: r[0]=16u;goto L8c61fb2e; /* mov #0x10,r0 */
L8c61fb2e: r[2]=r[3];goto L8c61fb30; /* mov r3,r2 */
L8c61fb30: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61fb32; /* fmov.s @(r0,r2),fr3 */
L8c61fb32: r[1]=r[3];goto L8c61fb34; /* mov r3,r1 */
L8c61fb34: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb36; /* fmul fr0,fr3 */
L8c61fb36: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61fb38; /* fmov.s @r1,fr2 */
L8c61fb38: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61fb3a; /* fmov.s @r4,fr0 */
L8c61fb3a: r[0]=8u;goto L8c61fb3c; /* mov #0x8,r0 */
L8c61fb3c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb3e; /* fmac fr0,fr2,fr3 */
L8c61fb3e: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fb40; /* fmov.s @(r0,r4),fr0 */
L8c61fb40: r[0]=32u;goto L8c61fb42; /* mov #0x20,r0 */
L8c61fb42: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61fb44; /* fmov.s @(r0,r3),fr2 */
L8c61fb44: r[0]=24u;goto L8c61fb46; /* mov #0x18,r0 */
L8c61fb46: r[3]=rd(m,bus,&io_status,0x8c61fd08u,4);if(io_status)return io_status;goto L8c61fb48; /* mov.l 0x8c61fd08,r3 */
L8c61fb48: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb4a; /* fmac fr0,fr2,fr3 */
L8c61fb4a: fcopy(f,0u,15u);goto L8c61fb4c; /* fmov fr15,fr0 */
L8c61fb4c: fcopy(f,2u,15u);goto L8c61fb4e; /* fmov fr15,fr2 */
L8c61fb4e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fb50; /* fmac fr0,fr3,fr2 */
L8c61fb50: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fb52; /* fmov.s fr2,@-r8 */
L8c61fb52: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fb54; /* fmov.s @(r0,r4),fr3 */
L8c61fb54: r[0]=20u;goto L8c61fb56; /* mov #0x14,r0 */
L8c61fb56: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fb58; /* fmov.s fr3,@-r8 */
L8c61fb58: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fb5a; /* fmov.s @(r0,r4),fr3 */
L8c61fb5a: r[0]=16u;goto L8c61fb5c; /* mov #0x10,r0 */
L8c61fb5c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fb5e; /* fmov.s fr3,@-r8 */
L8c61fb5e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fb60; /* fmov.s @(r0,r4),fr3 */
L8c61fb60: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fb62; /* fmov.s fr3,@-r8 */
L8c61fb62: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61fb64; /* mov.l r3,@-r8 */
L8c61fb64: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61fb66; /* pref @r8 */
L8c61fb66: r[2]=rd(m,bus,&io_status,0x8c61fd04u,4);if(io_status)return io_status;goto L8c61fb68; /* mov.l 0x8c61fd04,r2 */
L8c61fb68: r[0]=4u;goto L8c61fb6a; /* mov #0x4,r0 */
L8c61fb6a: r[8]+=60u;goto L8c61fb6c; /* add #0x3c,r8 */
L8c61fb6c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61fb6e; /* fmov.s fr14,@-r8 */
L8c61fb6e: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fb70; /* mov.l @r2,r3 */
L8c61fb70: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fb72; /* fmov.s @(r0,r13),fr0 */
L8c61fb72: r[0]=20u;goto L8c61fb74; /* mov #0x14,r0 */
L8c61fb74: r[2]=r[3];goto L8c61fb76; /* mov r3,r2 */
L8c61fb76: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61fb78; /* fmov.s @(r0,r2),fr3 */
L8c61fb78: r[1]=r[3];goto L8c61fb7a; /* mov r3,r1 */
L8c61fb7a: r[0]=4u;goto L8c61fb7c; /* mov #0x4,r0 */
L8c61fb7c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb7e; /* fmul fr0,fr3 */
L8c61fb7e: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61fb80; /* fmov.s @(r0,r1),fr2 */
L8c61fb80: fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61fb82; /* fmov.s @r13,fr0 */
L8c61fb82: r[0]=8u;goto L8c61fb84; /* mov #0x8,r0 */
L8c61fb84: r[2]=rd(m,bus,&io_status,0x8c61fd04u,4);if(io_status)return io_status;goto L8c61fb86; /* mov.l 0x8c61fd04,r2 */
L8c61fb86: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb88; /* fmac fr0,fr2,fr3 */
L8c61fb88: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fb8a; /* fmov.s @(r0,r13),fr0 */
L8c61fb8a: r[0]=36u;goto L8c61fb8c; /* mov #0x24,r0 */
L8c61fb8c: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61fb8e; /* fmov.s @(r0,r3),fr2 */
L8c61fb8e: r[0]=4u;goto L8c61fb90; /* mov #0x4,r0 */
L8c61fb90: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fb92; /* fmac fr0,fr2,fr3 */
L8c61fb92: fcopy(f,0u,15u);goto L8c61fb94; /* fmov fr15,fr0 */
L8c61fb94: fcopy(f,2u,15u);goto L8c61fb96; /* fmov fr15,fr2 */
L8c61fb96: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fb98; /* fmac fr0,fr3,fr2 */
L8c61fb98: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fb9a; /* fmov.s fr2,@-r8 */
L8c61fb9a: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fb9c; /* mov.l @r2,r3 */
L8c61fb9c: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fb9e; /* fmov.s @(r0,r13),fr0 */
L8c61fb9e: r[0]=16u;goto L8c61fba0; /* mov #0x10,r0 */
L8c61fba0: r[2]=r[3];goto L8c61fba2; /* mov r3,r2 */
L8c61fba2: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61fba4; /* fmov.s @(r0,r2),fr3 */
L8c61fba4: r[1]=r[3];goto L8c61fba6; /* mov r3,r1 */
L8c61fba6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fba8; /* fmul fr0,fr3 */
L8c61fba8: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61fbaa; /* fmov.s @r1,fr2 */
L8c61fbaa: fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61fbac; /* fmov.s @r13,fr0 */
L8c61fbac: r[0]=8u;goto L8c61fbae; /* mov #0x8,r0 */
L8c61fbae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fbb0; /* fmac fr0,fr2,fr3 */
L8c61fbb0: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fbb2; /* fmov.s @(r0,r13),fr0 */
L8c61fbb2: r[0]=32u;goto L8c61fbb4; /* mov #0x20,r0 */
L8c61fbb4: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61fbb6; /* fmov.s @(r0,r3),fr2 */
L8c61fbb6: r[0]=24u;goto L8c61fbb8; /* mov #0x18,r0 */
L8c61fbb8: r[3]=rd(m,bus,&io_status,0x8c61fd08u,4);if(io_status)return io_status;goto L8c61fbba; /* mov.l 0x8c61fd08,r3 */
L8c61fbba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fbbc; /* fmac fr0,fr2,fr3 */
L8c61fbbc: fcopy(f,0u,15u);goto L8c61fbbe; /* fmov fr15,fr0 */
L8c61fbbe: fcopy(f,2u,15u);goto L8c61fbc0; /* fmov fr15,fr2 */
L8c61fbc0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fbc2; /* fmac fr0,fr3,fr2 */
L8c61fbc2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fbc4; /* fmov.s fr2,@-r8 */
L8c61fbc4: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fbc6; /* fmov.s @(r0,r13),fr3 */
L8c61fbc6: r[0]=20u;goto L8c61fbc8; /* mov #0x14,r0 */
L8c61fbc8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fbca; /* fmov.s fr3,@-r8 */
L8c61fbca: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fbcc; /* fmov.s @(r0,r13),fr3 */
L8c61fbcc: r[0]=16u;goto L8c61fbce; /* mov #0x10,r0 */
L8c61fbce: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fbd0; /* fmov.s fr3,@-r8 */
L8c61fbd0: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61fbd2; /* fmov.s @(r0,r13),fr3 */
L8c61fbd2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fbd4; /* fmov.s fr3,@-r8 */
L8c61fbd4: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61fbd6; /* mov.l r3,@-r8 */
L8c61fbd6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61fbd8; /* pref @r8 */
L8c61fbd8: r[3]=rd(m,bus,&io_status,0x8c61fd04u,4);if(io_status)return io_status;goto L8c61fbda; /* mov.l 0x8c61fd04,r3 */
L8c61fbda: r[0]=4u;goto L8c61fbdc; /* mov #0x4,r0 */
L8c61fbdc: r[8]+=60u;goto L8c61fbde; /* add #0x3c,r8 */
L8c61fbde: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61fbe0; /* fmov.s fr14,@-r8 */
L8c61fbe0: r[9]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fbe2; /* mov.l @r3,r9 */
L8c61fbe2: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fbe4; /* fmov.s @(r0,r14),fr0 */
L8c61fbe4: r[0]=20u;goto L8c61fbe6; /* mov #0x14,r0 */
L8c61fbe6: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61fbe8; /* fmov.s @(r0,r9),fr3 */
L8c61fbe8: r[0]=4u;goto L8c61fbea; /* mov #0x4,r0 */
L8c61fbea: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61fbec; /* fmov.s @(r0,r9),fr2 */
L8c61fbec: r[0]=8u;goto L8c61fbee; /* mov #0x8,r0 */
L8c61fbee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fbf0; /* fmul fr0,fr3 */
L8c61fbf0: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61fbf2; /* fmov.s @r14,fr0 */
L8c61fbf2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fbf4; /* fmac fr0,fr2,fr3 */
L8c61fbf4: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fbf6; /* fmov.s @(r0,r14),fr0 */
L8c61fbf6: r[0]=36u;goto L8c61fbf8; /* mov #0x24,r0 */
L8c61fbf8: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61fbfa; /* fmov.s @(r0,r9),fr2 */
L8c61fbfa: r[0]=4u;goto L8c61fbfc; /* mov #0x4,r0 */
L8c61fbfc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fbfe; /* fmac fr0,fr2,fr3 */
L8c61fbfe: fcopy(f,2u,15u);goto L8c61fc00; /* fmov fr15,fr2 */
L8c61fc00: fcopy(f,0u,15u);goto L8c61fc02; /* fmov fr15,fr0 */
L8c61fc02: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fc04; /* fmac fr0,fr3,fr2 */
L8c61fc04: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fc06; /* fmov.s fr2,@-r8 */
L8c61fc06: r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fc08; /* mov.l @r3,r7 */
L8c61fc08: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fc0a; /* fmov.s @(r0,r14),fr0 */
L8c61fc0a: r[0]=16u;goto L8c61fc0c; /* mov #0x10,r0 */
L8c61fc0c: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61fc0e; /* fmov.s @(r0,r7),fr3 */
L8c61fc0e: r[0]=8u;goto L8c61fc10; /* mov #0x8,r0 */
L8c61fc10: fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61fc12; /* fmov.s @r7,fr2 */
L8c61fc12: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc14; /* fmul fr0,fr3 */
L8c61fc14: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61fc16; /* fmov.s @r14,fr0 */
L8c61fc16: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc18; /* fmac fr0,fr2,fr3 */
L8c61fc18: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fc1a; /* fmov.s @(r0,r14),fr0 */
L8c61fc1a: r[0]=32u;goto L8c61fc1c; /* mov #0x20,r0 */
L8c61fc1c: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61fc1e; /* fmov.s @(r0,r7),fr2 */
L8c61fc1e: r[0]=24u;goto L8c61fc20; /* mov #0x18,r0 */
L8c61fc20: r[2]=rd(m,bus,&io_status,0x8c61fd08u,4);if(io_status)return io_status;goto L8c61fc22; /* mov.l 0x8c61fd08,r2 */
L8c61fc22: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc24; /* fmac fr0,fr2,fr3 */
L8c61fc24: fcopy(f,0u,15u);goto L8c61fc26; /* fmov fr15,fr0 */
L8c61fc26: fcopy(f,2u,15u);goto L8c61fc28; /* fmov fr15,fr2 */
L8c61fc28: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fc2a; /* fmac fr0,fr3,fr2 */
L8c61fc2a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fc2c; /* fmov.s fr2,@-r8 */
L8c61fc2c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fc2e; /* fmov.s @(r0,r14),fr3 */
L8c61fc2e: r[0]=20u;goto L8c61fc30; /* mov #0x14,r0 */
L8c61fc30: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fc32; /* fmov.s fr3,@-r8 */
L8c61fc32: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fc34; /* fmov.s @(r0,r14),fr3 */
L8c61fc34: r[0]=16u;goto L8c61fc36; /* mov #0x10,r0 */
L8c61fc36: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fc38; /* fmov.s fr3,@-r8 */
L8c61fc38: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fc3a; /* fmov.s @(r0,r14),fr3 */
L8c61fc3a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fc3c; /* fmov.s fr3,@-r8 */
L8c61fc3c: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61fc3e; /* mov.l r2,@-r8 */
L8c61fc3e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61fc40; /* pref @r8 */
L8c61fc40: r[0]=4u;goto L8c61fc42; /* mov #0x4,r0 */
L8c61fc42: r[8]+=60u;goto L8c61fc44; /* add #0x3c,r8 */
L8c61fc44: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61fc46; /* fmov.s fr14,@-r8 */
L8c61fc46: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61fc48; /* fmov.s @(r0,r12),fr0 */
L8c61fc48: r[0]=20u;goto L8c61fc4a; /* mov #0x14,r0 */
L8c61fc4a: r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fc4c; /* mov.l @r3,r6 */
L8c61fc4c: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fc4e; /* fmov.s @(r0,r6),fr3 */
L8c61fc4e: r[0]=4u;goto L8c61fc50; /* mov #0x4,r0 */
L8c61fc50: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fc52; /* fmov.s @(r0,r6),fr2 */
L8c61fc52: r[0]=8u;goto L8c61fc54; /* mov #0x8,r0 */
L8c61fc54: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc56; /* fmul fr0,fr3 */
L8c61fc56: fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61fc58; /* fmov.s @r12,fr0 */
L8c61fc58: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc5a; /* fmac fr0,fr2,fr3 */
L8c61fc5a: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61fc5c; /* fmov.s @(r0,r12),fr0 */
L8c61fc5c: r[0]=36u;goto L8c61fc5e; /* mov #0x24,r0 */
L8c61fc5e: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fc60; /* fmov.s @(r0,r6),fr2 */
L8c61fc60: r[0]=4u;goto L8c61fc62; /* mov #0x4,r0 */
L8c61fc62: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc64; /* fmac fr0,fr2,fr3 */
L8c61fc64: fcopy(f,2u,15u);goto L8c61fc66; /* fmov fr15,fr2 */
L8c61fc66: fcopy(f,0u,15u);goto L8c61fc68; /* fmov fr15,fr0 */
L8c61fc68: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fc6a; /* fmac fr0,fr3,fr2 */
L8c61fc6a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fc6c; /* fmov.s fr2,@-r8 */
L8c61fc6c: r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fc6e; /* mov.l @r3,r5 */
L8c61fc6e: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61fc70; /* fmov.s @(r0,r12),fr0 */
L8c61fc70: r[0]=16u;goto L8c61fc72; /* mov #0x10,r0 */
L8c61fc72: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61fc74; /* fmov.s @(r0,r5),fr3 */
L8c61fc74: r[0]=8u;goto L8c61fc76; /* mov #0x8,r0 */
L8c61fc76: fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c61fc78; /* fmov.s @r5,fr2 */
L8c61fc78: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc7a; /* fmul fr0,fr3 */
L8c61fc7a: fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61fc7c; /* fmov.s @r12,fr0 */
L8c61fc7c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc7e; /* fmac fr0,fr2,fr3 */
L8c61fc7e: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61fc80; /* fmov.s @(r0,r12),fr0 */
L8c61fc80: r[0]=32u;goto L8c61fc82; /* mov #0x20,r0 */
L8c61fc82: r[2]=rd(m,bus,&io_status,0x8c61fd0cu,4);if(io_status)return io_status;goto L8c61fc84; /* mov.l 0x8c61fd0c,r2 */
L8c61fc84: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61fc86; /* fmov.s @(r0,r5),fr2 */
L8c61fc86: r[0]=24u;goto L8c61fc88; /* mov #0x18,r0 */
L8c61fc88: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fc8a; /* fmac fr0,fr2,fr3 */
L8c61fc8a: fcopy(f,0u,15u);goto L8c61fc8c; /* fmov fr15,fr0 */
L8c61fc8c: fcopy(f,2u,15u);goto L8c61fc8e; /* fmov fr15,fr2 */
L8c61fc8e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fc90; /* fmac fr0,fr3,fr2 */
L8c61fc90: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fc92; /* fmov.s fr2,@-r8 */
L8c61fc92: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61fc94; /* fmov.s @(r0,r12),fr3 */
L8c61fc94: r[0]=20u;goto L8c61fc96; /* mov #0x14,r0 */
L8c61fc96: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fc98; /* fmov.s fr3,@-r8 */
L8c61fc98: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61fc9a; /* fmov.s @(r0,r12),fr3 */
L8c61fc9a: r[0]=16u;goto L8c61fc9c; /* mov #0x10,r0 */
L8c61fc9c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fc9e; /* fmov.s fr3,@-r8 */
L8c61fc9e: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61fca0; /* fmov.s @(r0,r12),fr3 */
L8c61fca0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fca2; /* fmov.s fr3,@-r8 */
L8c61fca2: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61fca4; /* mov.l r2,@-r8 */
L8c61fca4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61fca6; /* pref @r8 */
L8c61fca6: r[1]=rd(m,bus,&io_status,0x8c61fd10u,4);if(io_status)return io_status;goto L8c61fca8; /* mov.l 0x8c61fd10,r1 */
L8c61fca8: r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61fcaa; /* mov.l @r1,r2 */
L8c61fcaa: r[2]+=4u;goto L8c61fcac; /* add #0x4,r2 */
L8c61fcac: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61fcae; /* mov.l r2,@r1 */
L8c61fcae: r[8]+=32u;goto L8c61fce8; /* bra 0x8c61fce8 */
L8c61fcb2: r[3]=rd(m,bus,&io_status,0x8c61fd14u,4);if(io_status)return io_status;goto L8c61fcb4; /* mov.l 0x8c61fd14,r3 */
L8c61fcb4: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61fcb6; /* mov.l r2,@r3 */
L8c61fcb6: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61fcb8; /* mov.l @(0xc,r4),r5 */
L8c61fcb8: r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61fcba; /* mov.l @(0xc,r13),r3 */
L8c61fcba: r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61fcbc; /* mov.l @(0xc,r14),r2 */
L8c61fcbc: r[5]&=r[3];goto L8c61fcbe; /* and r3,r5 */
L8c61fcbe: r[3]=1u;goto L8c61fcc0; /* mov #0x1,r3 */
L8c61fcc0: r[5]&=r[2];goto L8c61fcc2; /* and r2,r5 */
L8c61fcc2: *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61fcc4; /* tst r3,r5 */
L8c61fcc4: if(!(*sr&1u))goto L8c61fcce;goto L8c61fcc6; /* bf 0x8c61fcce */
L8c61fcc6: r[1]=rd(m,bus,&io_status,0x8c61fd18u,4);if(io_status)return io_status;goto L8c61fcc8; /* mov.l 0x8c61fd18,r1 */
L8c61fcc8: r[6]=r[14];goto L8c61fcca; /* mov r14,r6 */
L8c61fcca: target=r[1];*pr=0x8c61fcceu;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61fcce; /* jsr @r1 */
L8c61fcce: r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61fcd0; /* mov.l @(0xc,r13),r4 */
L8c61fcd0: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61fcd2; /* mov.l @(0xc,r14),r3 */
L8c61fcd2: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61fcd4; /* mov.l @(0xc,r12),r2 */
L8c61fcd4: r[4]&=r[3];goto L8c61fcd6; /* and r3,r4 */
L8c61fcd6: r[3]=1u;goto L8c61fcd8; /* mov #0x1,r3 */
L8c61fcd8: r[4]&=r[2];goto L8c61fcda; /* and r2,r4 */
L8c61fcda: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61fcdc; /* tst r3,r4 */
L8c61fcdc: if(!(*sr&1u))goto L8c61fce8;goto L8c61fcde; /* bf 0x8c61fce8 */
L8c61fcde: r[1]=rd(m,bus,&io_status,0x8c61fd18u,4);if(io_status)return io_status;goto L8c61fce0; /* mov.l 0x8c61fd18,r1 */
L8c61fce0: r[5]=r[14];goto L8c61fce2; /* mov r14,r5 */
L8c61fce2: r[6]=r[12];goto L8c61fce4; /* mov r12,r6 */
L8c61fce4: target=r[1];*pr=0x8c61fce8u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61fce8; /* jsr @r1 */
L8c61fce8: r[10]+=1u;goto L8c61fcea; /* add #0x1,r10 */
L8c61fcea: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61fcec; /* mov.l @r15,r3 */
L8c61fcec: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61fcee; /* mov.w @(0x2,r3),r0 */
L8c61fcee: r[0]=r[0]&65535u;goto L8c61fcf0; /* extu.w r0,r0 */
L8c61fcf0: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c61fcf2; /* cmp/ge r0,r10 */
L8c61fcf2: if((*sr&1u))goto L8c61fcf8;goto L8c61fcf4; /* bt 0x8c61fcf8 */
L8c61fcf4: goto L8c61fa60; /* bra 0x8c61fa60 */
L8c61fcf8: goto L8c61ff9e; /* bra 0x8c61ff9e */
L8c61fcfc: r[9]=0u;goto L8c61ff2c; /* bra 0x8c61ff2c */
L8c61fd1c: r[3]=rd(m,bus,&io_status,0x8c61ffbcu,4);if(io_status)return io_status;goto L8c61fd1e; /* mov.l 0x8c61ffbc,r3 */
L8c61fd1e: r[6]=0u;goto L8c61fd20; /* mov #0x0,r6 */
L8c61fd20: r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fd22; /* mov.w @r11+,r13 */
L8c61fd22: r[4]=0u;goto L8c61fd24; /* mov #0x0,r4 */
L8c61fd24: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fd26; /* mov.l @r3,r2 */
L8c61fd26: r[5]=r[6];goto L8c61fd28; /* mov r6,r5 */
L8c61fd28: r[13]=r[13]&65535u;goto L8c61fd2a; /* extu.w r13,r13 */
L8c61fd2a: r[2]+=r[13];goto L8c61fd2c; /* add r13,r2 */
L8c61fd2c: *sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c61fd2e; /* cmp/pl r13 */
L8c61fd2e: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61fd30; /* mov.l r2,@r3 */
L8c61fd30: r[7]=4294967295u;goto L8c61fd32; /* mov #-0x1,r7 */
L8c61fd32: take=!(*sr&1u);r[4]+=r[11];if(take)goto L8c61fd54;goto L8c61fd36; /* bf/s 0x8c61fd54 */
L8c61fd36: r[14]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c61fd38; /* mov.w @r4+,r14 */
L8c61fd38: r[5]+=1u;goto L8c61fd3a; /* add #0x1,r5 */
L8c61fd3a: r[2]=rd(m,bus,&io_status,0x8c61ffc0u,4);if(io_status)return io_status;goto L8c61fd3c; /* mov.l 0x8c61ffc0,r2 */
L8c61fd3c: r[14]=r[14]&65535u;goto L8c61fd3e; /* extu.w r14,r14 */
L8c61fd3e: r[14]<<=2;goto L8c61fd40; /* shll2 r14 */
L8c61fd40: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fd42; /* mov.l @r2,r3 */
L8c61fd42: r[14]<<=2;goto L8c61fd44; /* shll2 r14 */
L8c61fd44: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61fd46; /* shll r14 */
L8c61fd46: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[13]);goto L8c61fd48; /* cmp/ge r13,r5 */
L8c61fd48: r[14]+=r[3];goto L8c61fd4a; /* add r3,r14 */
L8c61fd4a: wr(m,bus,&io_status,r[15]+0x4u,r[14],4);if(io_status)return io_status;goto L8c61fd4c; /* mov.l r14,@(0x4,r15) */
L8c61fd4c: r[14]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61fd4e; /* mov.l @(0xc,r14),r14 */
L8c61fd4e: r[7]&=r[14];goto L8c61fd50; /* and r14,r7 */
L8c61fd50: take=!(*sr&1u);r[6]|=r[14];if(take)goto L8c61fd36;goto L8c61fd54; /* bf/s 0x8c61fd36 */
L8c61fd54: r[2]=63u;goto L8c61fd56; /* mov #0x3f,r2 */
L8c61fd56: *sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c61fd58; /* tst r2,r7 */
L8c61fd58: if(!(*sr&1u))goto L8c61fdb8;goto L8c61fd5a; /* bf 0x8c61fdb8 */
L8c61fd5a: r[1]=1u;goto L8c61fd5c; /* mov #0x1,r1 */
L8c61fd5c: *sr=(*sr&~1u)|!(r[1]&r[6]);goto L8c61fd5e; /* tst r1,r6 */
L8c61fd5e: if((*sr&1u))goto L8c61fd64;goto L8c61fd60; /* bt 0x8c61fd64 */
L8c61fd60: goto L8c61fecc; /* bra 0x8c61fecc */
L8c61fd64: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61fd66; /* mov.w @(0x2,r11),r0 */
L8c61fd66: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61fd68; /* mov.w @r11,r4 */
L8c61fd68: r[3]=rd(m,bus,&io_status,0x8c61ffc0u,4);if(io_status)return io_status;goto L8c61fd6a; /* mov.l 0x8c61ffc0,r3 */
L8c61fd6a: r[5]=r[0]&65535u;goto L8c61fd6c; /* extu.w r0,r5 */
L8c61fd6c: r[5]<<=2;goto L8c61fd6e; /* shll2 r5 */
L8c61fd6e: r[0]=sx(m,bus,&io_status,r[11]+0x4u,2);if(io_status)return io_status;goto L8c61fd70; /* mov.w @(0x4,r11),r0 */
L8c61fd70: r[4]=r[4]&65535u;goto L8c61fd72; /* extu.w r4,r4 */
L8c61fd72: r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fd74; /* mov.l @r3,r7 */
L8c61fd74: r[4]<<=2;goto L8c61fd76; /* shll2 r4 */
L8c61fd76: r[5]<<=2;goto L8c61fd78; /* shll2 r5 */
L8c61fd78: r[6]=r[0]&65535u;goto L8c61fd7a; /* extu.w r0,r6 */
L8c61fd7a: r[4]<<=2;goto L8c61fd7c; /* shll2 r4 */
L8c61fd7c: r[0]=16u;goto L8c61fd7e; /* mov #0x10,r0 */
L8c61fd7e: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61fd80; /* shll r4 */
L8c61fd80: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61fd82; /* shll r5 */
L8c61fd82: r[5]+=r[7];goto L8c61fd84; /* add r7,r5 */
L8c61fd84: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61fd86; /* fmov.s @(r0,r5),fr5 */
L8c61fd86: r[6]<<=2;goto L8c61fd88; /* shll2 r6 */
L8c61fd88: r[4]+=r[7];goto L8c61fd8a; /* add r7,r4 */
L8c61fd8a: r[6]<<=2;goto L8c61fd8c; /* shll2 r6 */
L8c61fd8c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fd8e; /* fmov.s @(r0,r4),fr3 */
L8c61fd8e: fcopy(f,2u,5u);goto L8c61fd90; /* fmov fr5,fr2 */
L8c61fd90: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61fd92; /* shll r6 */
L8c61fd92: r[0]=20u;goto L8c61fd94; /* mov #0x14,r0 */
L8c61fd94: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61fd96; /* fsub fr3,fr2 */
L8c61fd96: r[6]+=r[7];goto L8c61fd98; /* add r7,r6 */
L8c61fd98: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61fd9a; /* fmov.s @(r0,r5),fr4 */
L8c61fd9a: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fd9c; /* fmov.s @(r0,r6),fr3 */
L8c61fd9c: fcopy(f,1u,4u);goto L8c61fd9e; /* fmov fr4,fr1 */
L8c61fd9e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61fda0; /* fsub fr4,fr3 */
L8c61fda0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fda2; /* fmul fr3,fr2 */
L8c61fda2: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fda4; /* fmov.s @(r0,r4),fr3 */
L8c61fda4: r[0]=16u;goto L8c61fda6; /* mov #0x10,r0 */
L8c61fda6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61fda8; /* fsub fr3,fr1 */
L8c61fda8: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fdaa; /* fmov.s @(r0,r6),fr3 */
L8c61fdaa: fcopy(f,4u,2u);goto L8c61fdac; /* fmov fr2,fr4 */
L8c61fdac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61fdae; /* fsub fr5,fr3 */
L8c61fdae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61fdb0; /* fmul fr3,fr1 */
L8c61fdb0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61fdb2; /* fldi0 fr3 */
L8c61fdb2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61fdb4; /* fsub fr1,fr4 */
L8c61fdb4: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61fdb6; /* fcmp/gt fr3,fr4 */
L8c61fdb6: if(!(*sr&1u))goto L8c61fdbe;goto L8c61fdb8; /* bf 0x8c61fdbe */
L8c61fdb8: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61fdba; /* shll r13 */
L8c61fdba: r[11]+=r[13];goto L8c61ff2a; /* bra 0x8c61ff2a */
L8c61fdbe: r[2]=rd(m,bus,&io_status,0x8c61ffc4u,4);if(io_status)return io_status;goto L8c61fdc0; /* mov.l 0x8c61ffc4,r2 */
L8c61fdc0: r[6]=r[13];goto L8c61fdc2; /* mov r13,r6 */
L8c61fdc2: r[6]+=4294967295u;goto L8c61fdc4; /* add #-0x1,r6 */
L8c61fdc4: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fdc6; /* mov.l @r2,r3 */
L8c61fdc6: *sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c61fdc8; /* cmp/pl r6 */
L8c61fdc8: r[3]+=r[13];goto L8c61fdca; /* add r13,r3 */
L8c61fdca: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61fdcc; /* mov.l r3,@r2 */
L8c61fdcc: take=!(*sr&1u);r[5]=0u;if(take)goto L8c61fe50;goto L8c61fdd0; /* bf/s 0x8c61fe50 */
L8c61fdd0: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fdd2; /* mov.w @r11+,r4 */
L8c61fdd2: r[0]=4u;goto L8c61fdd4; /* mov #0x4,r0 */
L8c61fdd4: r[2]=rd(m,bus,&io_status,0x8c61ffc0u,4);if(io_status)return io_status;goto L8c61fdd6; /* mov.l 0x8c61ffc0,r2 */
L8c61fdd6: r[8]+=28u;goto L8c61fdd8; /* add #0x1c,r8 */
L8c61fdd8: r[4]=r[4]&65535u;goto L8c61fdda; /* extu.w r4,r4 */
L8c61fdda: r[4]<<=2;goto L8c61fddc; /* shll2 r4 */
L8c61fddc: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fdde; /* mov.l @r2,r3 */
L8c61fdde: r[4]<<=2;goto L8c61fde0; /* shll2 r4 */
L8c61fde0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61fde2; /* fmov.s fr14,@-r8 */
L8c61fde2: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61fde4; /* shll r4 */
L8c61fde4: r[4]+=r[3];goto L8c61fde6; /* add r3,r4 */
L8c61fde6: r[3]=rd(m,bus,&io_status,0x8c61ffc8u,4);if(io_status)return io_status;goto L8c61fde8; /* mov.l 0x8c61ffc8,r3 */
L8c61fde8: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fdea; /* fmov.s @(r0,r4),fr0 */
L8c61fdea: r[0]=20u;goto L8c61fdec; /* mov #0x14,r0 */
L8c61fdec: r[14]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fdee; /* mov.l @r3,r14 */
L8c61fdee: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fdf0; /* fmov.s @(r0,r14),fr3 */
L8c61fdf0: r[0]=4u;goto L8c61fdf2; /* mov #0x4,r0 */
L8c61fdf2: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fdf4; /* fmov.s @(r0,r14),fr2 */
L8c61fdf4: r[0]=8u;goto L8c61fdf6; /* mov #0x8,r0 */
L8c61fdf6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fdf8; /* fmul fr0,fr3 */
L8c61fdf8: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61fdfa; /* fmov.s @r4,fr0 */
L8c61fdfa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fdfc; /* fmac fr0,fr2,fr3 */
L8c61fdfc: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fdfe; /* fmov.s @(r0,r4),fr0 */
L8c61fdfe: r[0]=36u;goto L8c61fe00; /* mov #0x24,r0 */
L8c61fe00: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61fe02; /* fmov.s @(r0,r14),fr2 */
L8c61fe02: r[0]=4u;goto L8c61fe04; /* mov #0x4,r0 */
L8c61fe04: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe06; /* fmac fr0,fr2,fr3 */
L8c61fe06: fcopy(f,2u,15u);goto L8c61fe08; /* fmov fr15,fr2 */
L8c61fe08: fcopy(f,0u,15u);goto L8c61fe0a; /* fmov fr15,fr0 */
L8c61fe0a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fe0c; /* fmac fr0,fr3,fr2 */
L8c61fe0c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fe0e; /* fmov.s fr2,@-r8 */
L8c61fe0e: r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fe10; /* mov.l @r3,r7 */
L8c61fe10: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe12; /* fmov.s @(r0,r4),fr0 */
L8c61fe12: r[0]=16u;goto L8c61fe14; /* mov #0x10,r0 */
L8c61fe14: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61fe16; /* fmov.s @(r0,r7),fr3 */
L8c61fe16: r[0]=8u;goto L8c61fe18; /* mov #0x8,r0 */
L8c61fe18: fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61fe1a; /* fmov.s @r7,fr2 */
L8c61fe1a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe1c; /* fmul fr0,fr3 */
L8c61fe1c: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61fe1e; /* fmov.s @r4,fr0 */
L8c61fe1e: r[1]=rd(m,bus,&io_status,0x8c61ffccu,4);if(io_status)return io_status;goto L8c61fe20; /* mov.l 0x8c61ffcc,r1 */
L8c61fe20: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe22; /* fmac fr0,fr2,fr3 */
L8c61fe22: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe24; /* fmov.s @(r0,r4),fr0 */
L8c61fe24: r[0]=32u;goto L8c61fe26; /* mov #0x20,r0 */
L8c61fe26: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61fe28; /* fmov.s @(r0,r7),fr2 */
L8c61fe28: r[0]=24u;goto L8c61fe2a; /* mov #0x18,r0 */
L8c61fe2a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe2c; /* fmac fr0,fr2,fr3 */
L8c61fe2c: fcopy(f,0u,15u);goto L8c61fe2e; /* fmov fr15,fr0 */
L8c61fe2e: fcopy(f,2u,15u);goto L8c61fe30; /* fmov fr15,fr2 */
L8c61fe30: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fe32; /* fmac fr0,fr3,fr2 */
L8c61fe32: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fe34; /* fmov.s fr2,@-r8 */
L8c61fe34: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe36; /* fmov.s @(r0,r4),fr3 */
L8c61fe36: r[0]=20u;goto L8c61fe38; /* mov #0x14,r0 */
L8c61fe38: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fe3a; /* fmov.s fr3,@-r8 */
L8c61fe3a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe3c; /* fmov.s @(r0,r4),fr3 */
L8c61fe3c: r[0]=16u;goto L8c61fe3e; /* mov #0x10,r0 */
L8c61fe3e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fe40; /* fmov.s fr3,@-r8 */
L8c61fe40: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe42; /* fmov.s @(r0,r4),fr3 */
L8c61fe42: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fe44; /* fmov.s fr3,@-r8 */
L8c61fe44: r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c61fe46; /* mov.l r1,@-r8 */
L8c61fe46: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61fe48; /* pref @r8 */
L8c61fe48: r[5]+=1u;goto L8c61fe4a; /* add #0x1,r5 */
L8c61fe4a: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61fe4c; /* cmp/ge r6,r5 */
L8c61fe4c: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61fdd0;goto L8c61fe50; /* bf/s 0x8c61fdd0 */
L8c61fe50: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fe52; /* mov.w @r11+,r4 */
L8c61fe52: r[0]=4u;goto L8c61fe54; /* mov #0x4,r0 */
L8c61fe54: r[2]=rd(m,bus,&io_status,0x8c61ffc0u,4);if(io_status)return io_status;goto L8c61fe56; /* mov.l 0x8c61ffc0,r2 */
L8c61fe56: r[8]+=28u;goto L8c61fe58; /* add #0x1c,r8 */
L8c61fe58: r[4]=r[4]&65535u;goto L8c61fe5a; /* extu.w r4,r4 */
L8c61fe5a: r[4]<<=2;goto L8c61fe5c; /* shll2 r4 */
L8c61fe5c: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61fe5e; /* mov.l @r2,r3 */
L8c61fe5e: r[4]<<=2;goto L8c61fe60; /* shll2 r4 */
L8c61fe60: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[8]);if(io_status)return io_status;goto L8c61fe62; /* fmov.s fr14,@-r8 */
L8c61fe62: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61fe64; /* shll r4 */
L8c61fe64: r[4]+=r[3];goto L8c61fe66; /* add r3,r4 */
L8c61fe66: r[3]=rd(m,bus,&io_status,0x8c61ffc8u,4);if(io_status)return io_status;goto L8c61fe68; /* mov.l 0x8c61ffc8,r3 */
L8c61fe68: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe6a; /* fmov.s @(r0,r4),fr0 */
L8c61fe6a: r[0]=20u;goto L8c61fe6c; /* mov #0x14,r0 */
L8c61fe6c: r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fe6e; /* mov.l @r3,r6 */
L8c61fe6e: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fe70; /* fmov.s @(r0,r6),fr3 */
L8c61fe70: r[0]=4u;goto L8c61fe72; /* mov #0x4,r0 */
L8c61fe72: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fe74; /* fmov.s @(r0,r6),fr2 */
L8c61fe74: r[0]=8u;goto L8c61fe76; /* mov #0x8,r0 */
L8c61fe76: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe78; /* fmul fr0,fr3 */
L8c61fe78: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61fe7a; /* fmov.s @r4,fr0 */
L8c61fe7a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe7c; /* fmac fr0,fr2,fr3 */
L8c61fe7c: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe7e; /* fmov.s @(r0,r4),fr0 */
L8c61fe7e: r[0]=36u;goto L8c61fe80; /* mov #0x24,r0 */
L8c61fe80: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61fe82; /* fmov.s @(r0,r6),fr2 */
L8c61fe82: r[0]=4u;goto L8c61fe84; /* mov #0x4,r0 */
L8c61fe84: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe86; /* fmac fr0,fr2,fr3 */
L8c61fe86: fcopy(f,2u,15u);goto L8c61fe88; /* fmov fr15,fr2 */
L8c61fe88: fcopy(f,0u,15u);goto L8c61fe8a; /* fmov fr15,fr0 */
L8c61fe8a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61fe8c; /* fmac fr0,fr3,fr2 */
L8c61fe8c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61fe8e; /* fmov.s fr2,@-r8 */
L8c61fe8e: r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61fe90; /* mov.l @r3,r5 */
L8c61fe90: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fe92; /* fmov.s @(r0,r4),fr0 */
L8c61fe92: r[0]=16u;goto L8c61fe94; /* mov #0x10,r0 */
L8c61fe94: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61fe96; /* fmov.s @(r0,r5),fr3 */
L8c61fe96: r[0]=8u;goto L8c61fe98; /* mov #0x8,r0 */
L8c61fe98: fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c61fe9a; /* fmov.s @r5,fr2 */
L8c61fe9a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fe9c; /* fmul fr0,fr3 */
L8c61fe9c: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61fe9e; /* fmov.s @r4,fr0 */
L8c61fe9e: r[1]=rd(m,bus,&io_status,0x8c61ffd0u,4);if(io_status)return io_status;goto L8c61fea0; /* mov.l 0x8c61ffd0,r1 */
L8c61fea0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61fea2; /* fmac fr0,fr2,fr3 */
L8c61fea2: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fea4; /* fmov.s @(r0,r4),fr0 */
L8c61fea4: r[0]=32u;goto L8c61fea6; /* mov #0x20,r0 */
L8c61fea6: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61fea8; /* fmov.s @(r0,r5),fr2 */
L8c61fea8: r[0]=24u;goto L8c61feaa; /* mov #0x18,r0 */
L8c61feaa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61feac; /* fmac fr0,fr2,fr3 */
L8c61feac: fcopy(f,0u,15u);goto L8c61feae; /* fmov fr15,fr0 */
L8c61feae: fcopy(f,2u,15u);goto L8c61feb0; /* fmov fr15,fr2 */
L8c61feb0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61feb2; /* fmac fr0,fr3,fr2 */
L8c61feb2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61feb4; /* fmov.s fr2,@-r8 */
L8c61feb4: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61feb6; /* fmov.s @(r0,r4),fr3 */
L8c61feb6: r[0]=20u;goto L8c61feb8; /* mov #0x14,r0 */
L8c61feb8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61feba; /* fmov.s fr3,@-r8 */
L8c61feba: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61febc; /* fmov.s @(r0,r4),fr3 */
L8c61febc: r[0]=16u;goto L8c61febe; /* mov #0x10,r0 */
L8c61febe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fec0; /* fmov.s fr3,@-r8 */
L8c61fec0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61fec2; /* fmov.s @(r0,r4),fr3 */
L8c61fec2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61fec4; /* fmov.s fr3,@-r8 */
L8c61fec4: r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c61fec6; /* mov.l r1,@-r8 */
L8c61fec6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61fec8; /* pref @r8 */
L8c61fec8: r[8]+=32u;goto L8c61ff2a; /* bra 0x8c61ff2a */
L8c61fecc: r[2]=rd(m,bus,&io_status,0x8c61ffd4u,4);if(io_status)return io_status;goto L8c61fece; /* mov.l 0x8c61ffd4,r2 */
L8c61fece: r[3]=0u;goto L8c61fed0; /* mov #0x0,r3 */
L8c61fed0: r[0]=rd(m,bus,&io_status,0x8c61ffc0u,4);if(io_status)return io_status;goto L8c61fed2; /* mov.l 0x8c61ffc0,r0 */
L8c61fed2: r[10]=r[13];goto L8c61fed4; /* mov r13,r10 */
L8c61fed4: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61fed6; /* mov.l r3,@r2 */
L8c61fed6: r[10]+=4294967294u;goto L8c61fed8; /* add #-0x2,r10 */
L8c61fed8: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61feda; /* mov.w @r11+,r4 */
L8c61feda: r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61fedc; /* mov.l @r0,r3 */
L8c61fedc: r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fede; /* mov.w @r11+,r14 */
L8c61fede: r[4]=r[4]&65535u;goto L8c61fee0; /* extu.w r4,r4 */
L8c61fee0: r[4]<<=2;goto L8c61fee2; /* shll2 r4 */
L8c61fee2: r[14]=r[14]&65535u;goto L8c61fee4; /* extu.w r14,r14 */
L8c61fee4: r[14]<<=2;goto L8c61fee6; /* shll2 r14 */
L8c61fee6: r[14]<<=2;goto L8c61fee8; /* shll2 r14 */
L8c61fee8: r[4]<<=2;goto L8c61feea; /* shll2 r4 */
L8c61feea: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61feec; /* shll r4 */
L8c61feec: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61feee; /* shll r14 */
L8c61feee: *sr=(*sr&~1u)|((int32_t)r[10]>0);goto L8c61fef0; /* cmp/pl r10 */
L8c61fef0: r[4]+=r[3];goto L8c61fef2; /* add r3,r4 */
L8c61fef2: r[14]+=r[3];goto L8c61fef4; /* add r3,r14 */
L8c61fef4: take=!(*sr&1u);r[12]=0u;if(take)goto L8c61ff2a;goto L8c61fef8; /* bf/s 0x8c61ff2a */
L8c61fef8: r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61fefa; /* mov.w @r11+,r13 */
L8c61fefa: r[2]=rd(m,bus,&io_status,0x8c61ffc0u,4);if(io_status)return io_status;goto L8c61fefc; /* mov.l 0x8c61ffc0,r2 */
L8c61fefc: r[13]=r[13]&65535u;goto L8c61fefe; /* extu.w r13,r13 */
L8c61fefe: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61ff00; /* mov.l @(0xc,r4),r5 */
L8c61ff00: r[13]<<=2;goto L8c61ff02; /* shll2 r13 */
L8c61ff02: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ff04; /* mov.l @r2,r3 */
L8c61ff04: r[13]<<=2;goto L8c61ff06; /* shll2 r13 */
L8c61ff06: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61ff08; /* shll r13 */
L8c61ff08: r[13]+=r[3];goto L8c61ff0a; /* add r3,r13 */
L8c61ff0a: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61ff0c; /* mov.l @(0xc,r14),r3 */
L8c61ff0c: r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61ff0e; /* mov.l @(0xc,r13),r1 */
L8c61ff0e: r[5]&=r[3];goto L8c61ff10; /* and r3,r5 */
L8c61ff10: r[3]=1u;goto L8c61ff12; /* mov #0x1,r3 */
L8c61ff12: r[5]&=r[1];goto L8c61ff14; /* and r1,r5 */
L8c61ff14: *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61ff16; /* tst r3,r5 */
L8c61ff16: if(!(*sr&1u))goto L8c61ff20;goto L8c61ff18; /* bf 0x8c61ff20 */
L8c61ff18: r[1]=rd(m,bus,&io_status,0x8c61ffd8u,4);if(io_status)return io_status;goto L8c61ff1a; /* mov.l 0x8c61ffd8,r1 */
L8c61ff1a: r[6]=r[13];goto L8c61ff1c; /* mov r13,r6 */
L8c61ff1c: target=r[1];*pr=0x8c61ff20u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ff20; /* jsr @r1 */
L8c61ff20: r[12]+=1u;goto L8c61ff22; /* add #0x1,r12 */
L8c61ff22: r[4]=r[14];goto L8c61ff24; /* mov r14,r4 */
L8c61ff24: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[10]);goto L8c61ff26; /* cmp/ge r10,r12 */
L8c61ff26: take=!(*sr&1u);r[14]=r[13];if(take)goto L8c61fef8;goto L8c61ff2a; /* bf/s 0x8c61fef8 */
L8c61ff2a: r[9]+=1u;goto L8c61ff2c; /* add #0x1,r9 */
L8c61ff2c: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ff2e; /* mov.l @r15,r2 */
L8c61ff2e: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61ff30; /* mov.w @(0x2,r2),r0 */
L8c61ff30: r[0]=r[0]&65535u;goto L8c61ff32; /* extu.w r0,r0 */
L8c61ff32: *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61ff34; /* cmp/ge r0,r9 */
L8c61ff34: if((*sr&1u))goto L8c61ff3a;goto L8c61ff36; /* bt 0x8c61ff3a */
L8c61ff36: goto L8c61fd1c; /* bra 0x8c61fd1c */
L8c61ff3a: goto L8c61ff9e; /* bra 0x8c61ff9e */
L8c61ff3e: r[2]=rd(m,bus,&io_status,0x8c61ffdcu,4);if(io_status)return io_status;goto L8c61ff40; /* mov.l 0x8c61ffdc,r2 */
L8c61ff40: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ff42; /* mov.l @r15,r1 */
L8c61ff42: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61ff44; /* mov.l r1,@r2 */
L8c61ff44: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61ff46; /* frchg */
L8c61ff46: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61ff48; /* fmov.s fr0,@-r15 */
L8c61ff48: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61ff4a; /* fmov.s fr1,@-r15 */
L8c61ff4a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61ff4c; /* fmov.s fr2,@-r15 */
L8c61ff4c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61ff4e; /* fmov.s fr3,@-r15 */
L8c61ff4e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61ff50; /* fmov.s fr4,@-r15 */
L8c61ff50: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61ff52; /* fmov.s fr5,@-r15 */
L8c61ff52: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61ff54; /* fmov.s fr6,@-r15 */
L8c61ff54: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61ff56; /* fmov.s fr7,@-r15 */
L8c61ff56: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61ff58; /* fmov.s fr8,@-r15 */
L8c61ff58: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61ff5a; /* fmov.s fr9,@-r15 */
L8c61ff5a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c61ff5c; /* fmov.s fr10,@-r15 */
L8c61ff5c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c61ff5e; /* fmov.s fr11,@-r15 */
L8c61ff5e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c61ff60; /* fmov.s fr12,@-r15 */
L8c61ff60: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61ff62; /* fmov.s fr13,@-r15 */
L8c61ff62: r[0]=0x8c61ffb4u;goto L8c61ff64; /* mova 0x8c61ffb4,r0 */
L8c61ff64: fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c61ff66; /* fmov.s @r0,fr12 */
L8c61ff66: fcopy(f,13u,12u);goto L8c61ff68; /* fmov fr12,fr13 */
L8c61ff68: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[12],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61ff6a; /* fmul fr12,fr0 */
L8c61ff6a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61ff6c; /* fmul fr12,fr4 */
L8c61ff6c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[12],&f->fpscr,&fr[8]);if(status)return status;}goto L8c61ff6e; /* fmul fr12,fr8 */
L8c61ff6e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[12],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ff70; /* fmul fr12,fr1 */
L8c61ff70: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[12],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61ff72; /* fmul fr12,fr5 */
L8c61ff72: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[12],&f->fpscr,&fr[9]);if(status)return status;}goto L8c61ff74; /* fmul fr12,fr9 */
L8c61ff74: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61ff76; /* frchg */
L8c61ff76: r[2]=rd(m,bus,&io_status,0x8c61ffe0u,4);if(io_status)return io_status;goto L8c61ff78; /* mov.l 0x8c61ffe0,r2 */
L8c61ff78: r[3]=rd(m,bus,&io_status,0x8c61ffdcu,4);if(io_status)return io_status;goto L8c61ff7a; /* mov.l 0x8c61ffdc,r3 */
L8c61ff7a: target=r[2];*pr=0x8c61ff7eu;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ff7e; /* jsr @r2 */
L8c61ff7e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61ff80; /* frchg */
L8c61ff80: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff82; /* fmov.s @r15+,fr13 */
L8c61ff82: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff84; /* fmov.s @r15+,fr12 */
L8c61ff84: fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff86; /* fmov.s @r15+,fr11 */
L8c61ff86: fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff88; /* fmov.s @r15+,fr10 */
L8c61ff88: fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff8a; /* fmov.s @r15+,fr9 */
L8c61ff8a: fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff8c; /* fmov.s @r15+,fr8 */
L8c61ff8c: fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff8e; /* fmov.s @r15+,fr7 */
L8c61ff8e: fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff90; /* fmov.s @r15+,fr6 */
L8c61ff90: fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff92; /* fmov.s @r15+,fr5 */
L8c61ff92: fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff94; /* fmov.s @r15+,fr4 */
L8c61ff94: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff96; /* fmov.s @r15+,fr3 */
L8c61ff96: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff98; /* fmov.s @r15+,fr2 */
L8c61ff98: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff9a; /* fmov.s @r15+,fr1 */
L8c61ff9a: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ff9c; /* fmov.s @r15+,fr0 */
L8c61ff9c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61ff9e; /* frchg */
L8c61ff9e: r[15]+=8u;goto L8c61ffa0; /* add #0x8,r15 */
L8c61ffa0: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61ffa2; /* lds.l @r15+,PR */
L8c61ffa2: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ffa4; /* fmov.s @r15+,fr14 */
L8c61ffa4: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ffa6; /* fmov.s @r15+,fr15 */
L8c61ffa6: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61ffa8; /* mov.l @r15+,r9 */
L8c61ffa8: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61ffaa; /* mov.l @r15+,r10 */
L8c61ffaa: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61ffac; /* mov.l @r15+,r11 */
L8c61ffac: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61ffae; /* mov.l @r15+,r12 */
L8c61ffae: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61ffb0; /* mov.l @r15+,r13 */
L8c61ffb0: if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
