/* Environment UV/face-factor primitive family with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_environment_face_primitive_emit_8c61db98(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 uint32_t entry_pr=*pr;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61db98;
L8c61db98: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61db9a; /* mov.l r14,@-r15 */
L8c61db9a: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61db9c; /* mov.l r13,@-r15 */
L8c61db9c: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61db9e; /* mov.l r12,@-r15 */
L8c61db9e: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61dba0; /* mov.l r11,@-r15 */
L8c61dba0: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61dba2; /* mov.l r10,@-r15 */
L8c61dba2: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61dba4; /* mov.l r9,@-r15 */
L8c61dba4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61dba6; /* fmov.s fr15,@-r15 */
L8c61dba6: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61dba8; /* sts.l PR,@-r15 */
L8c61dba8: r[15]+=4294967280u;goto L8c61dbaa; /* add #-0x10,r15 */
L8c61dbaa: r[0]=0x8c61dc0cu;goto L8c61dbac; /* mova 0x8c61dc0c,r0 */
L8c61dbac: r[3]=rd(m,bus,&io_status,0x8c61dc14u,4);if(io_status)return io_status;goto L8c61dbae; /* mov.l 0x8c61dc14,r3 */
L8c61dbae: r[10]=r[4];goto L8c61dbb0; /* mov r4,r10 */
L8c61dbb0: r[13]=rd(m,bus,&io_status,0x8c61dc10u,4);if(io_status)return io_status;goto L8c61dbb2; /* mov.l 0x8c61dc10,r13 */
L8c61dbb2: wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61dbb4; /* mov.l r4,@r15 */
L8c61dbb4: fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c61dbb6; /* fmov.s @r0,fr15 */
L8c61dbb6: r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61dbb8; /* mov.l @r15,r0 */
L8c61dbb8: r[10]=rd(m,bus,&io_status,r[10]+0x4u,4);if(io_status)return io_status;goto L8c61dbba; /* mov.l @(0x4,r10),r10 */
L8c61dbba: r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61dbbc; /* mov.w @r0,r0 */
L8c61dbbc: r[0]=r[0]&65535u;goto L8c61dbbe; /* extu.w r0,r0 */
L8c61dbbe: r[0]&=r[3];goto L8c61dbc0; /* and r3,r0 */
L8c61dbc0: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61dbc2; /* cmp/eq #0x0,r0 */
L8c61dbc2: take=(*sr&1u);r[4]=0u;if(take)goto L8c61dbe8;goto L8c61dbc6; /* bt/s 0x8c61dbe8 */
L8c61dbc6: r[1]=sx(m,bus,&io_status,0x8c61dc00u,2);if(io_status)return io_status;goto L8c61dbc8; /* mov.w 0x8c61dc00,r1 */
L8c61dbc8: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61dbca; /* cmp/eq r1,r0 */
L8c61dbca: if(!(*sr&1u))goto L8c61dbd0;goto L8c61dbcc; /* bf 0x8c61dbd0 */
L8c61dbcc: goto L8c61de40; /* bra 0x8c61de40 */
L8c61dbd0: r[1]=rd(m,bus,&io_status,0x8c61dc18u,4);if(io_status)return io_status;goto L8c61dbd2; /* mov.l 0x8c61dc18,r1 */
L8c61dbd2: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61dbd4; /* cmp/eq r1,r0 */
L8c61dbd4: if(!(*sr&1u))goto L8c61dbda;goto L8c61dbd6; /* bf 0x8c61dbda */
L8c61dbd6: goto L8c61e122; /* bra 0x8c61e122 */
L8c61dbda: r[1]=rd(m,bus,&io_status,0x8c61dc14u,4);if(io_status)return io_status;goto L8c61dbdc; /* mov.l 0x8c61dc14,r1 */
L8c61dbdc: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61dbde; /* cmp/eq r1,r0 */
L8c61dbde: if(!(*sr&1u))goto L8c61dbe4;goto L8c61dbe0; /* bf 0x8c61dbe4 */
L8c61dbe0: goto L8c61e42e; /* bra 0x8c61e42e */
L8c61dbe4: goto L8c61e4a8; /* bra 0x8c61e4a8 */
L8c61dbe8: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61dbea; /* mov.l @r15,r2 */
L8c61dbea: r[1]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61dbec; /* mov.l @r13,r1 */
L8c61dbec: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61dbee; /* mov.w @(0x2,r2),r0 */
L8c61dbee: r[0]=r[0]&65535u;goto L8c61dbf0; /* extu.w r0,r0 */
L8c61dbf0: r[2]=r[0];goto L8c61dbf2; /* mov r0,r2 */
L8c61dbf2: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61dbf4; /* shll r0 */
L8c61dbf4: r[0]+=r[2];goto L8c61dbf6; /* add r2,r0 */
L8c61dbf6: r[1]+=r[0];goto L8c61dbf8; /* add r0,r1 */
L8c61dbf8: wr(m,bus,&io_status,r[13],r[1],4);if(io_status)return io_status;goto L8c61dbfa; /* mov.l r1,@r13 */
L8c61dbfa: r[11]=r[4];goto L8c61de2e; /* bra 0x8c61de2e */
L8c61dc1c: r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61dc1e; /* mov.w @r10+,r12 */
L8c61dc1e: r[0]=16u;goto L8c61dc20; /* mov #0x10,r0 */
L8c61dc20: r[3]=rd(m,bus,&io_status,0x8c61de58u,4);if(io_status)return io_status;goto L8c61dc22; /* mov.l 0x8c61de58,r3 */
L8c61dc22: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61dc24; /* mov.w @r10+,r13 */
L8c61dc24: r[12]=r[12]&65535u;goto L8c61dc26; /* extu.w r12,r12 */
L8c61dc26: r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61dc28; /* mov.l @r3,r4 */
L8c61dc28: r[12]<<=2;goto L8c61dc2a; /* shll2 r12 */
L8c61dc2a: r[13]=r[13]&65535u;goto L8c61dc2c; /* extu.w r13,r13 */
L8c61dc2c: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61dc2e; /* mov.w @r10+,r14 */
L8c61dc2e: r[13]<<=2;goto L8c61dc30; /* shll2 r13 */
L8c61dc30: r[13]<<=2;goto L8c61dc32; /* shll2 r13 */
L8c61dc32: r[12]<<=2;goto L8c61dc34; /* shll2 r12 */
L8c61dc34: r[14]=r[14]&65535u;goto L8c61dc36; /* extu.w r14,r14 */
L8c61dc36: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61dc38; /* shll r12 */
L8c61dc38: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61dc3a; /* shll r13 */
L8c61dc3a: r[13]+=r[4];goto L8c61dc3c; /* add r4,r13 */
L8c61dc3c: fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dc3e; /* fmov.s @(r0,r13),fr5 */
L8c61dc3e: r[12]+=r[4];goto L8c61dc40; /* add r4,r12 */
L8c61dc40: r[14]<<=2;goto L8c61dc42; /* shll2 r14 */
L8c61dc42: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dc44; /* fmov.s @(r0,r12),fr3 */
L8c61dc44: r[14]<<=2;goto L8c61dc46; /* shll2 r14 */
L8c61dc46: fcopy(f,2u,5u);goto L8c61dc48; /* fmov fr5,fr2 */
L8c61dc48: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61dc4a; /* shll r14 */
L8c61dc4a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61dc4c; /* fsub fr3,fr2 */
L8c61dc4c: r[0]=20u;goto L8c61dc4e; /* mov #0x14,r0 */
L8c61dc4e: r[14]+=r[4];goto L8c61dc50; /* add r4,r14 */
L8c61dc50: fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dc52; /* fmov.s @(r0,r13),fr4 */
L8c61dc52: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dc54; /* fmov.s @(r0,r14),fr3 */
L8c61dc54: fcopy(f,1u,4u);goto L8c61dc56; /* fmov fr4,fr1 */
L8c61dc56: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61dc58; /* fsub fr4,fr3 */
L8c61dc58: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dc5a; /* fmul fr3,fr2 */
L8c61dc5a: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dc5c; /* fmov.s @(r0,r12),fr3 */
L8c61dc5c: r[0]=16u;goto L8c61dc5e; /* mov #0x10,r0 */
L8c61dc5e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61dc60; /* fsub fr3,fr1 */
L8c61dc60: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dc62; /* fmov.s @(r0,r14),fr3 */
L8c61dc62: fcopy(f,4u,2u);goto L8c61dc64; /* fmov fr2,fr4 */
L8c61dc64: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61dc66; /* fsub fr5,fr3 */
L8c61dc66: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61dc68; /* fmul fr3,fr1 */
L8c61dc68: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61dc6a; /* fldi0 fr3 */
L8c61dc6a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61dc6c; /* fsub fr1,fr4 */
L8c61dc6c: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61dc6e; /* fcmp/gt fr3,fr4 */
L8c61dc6e: if(!(*sr&1u))goto L8c61dc74;goto L8c61dc70; /* bf 0x8c61dc74 */
L8c61dc70: goto L8c61de2c; /* bra 0x8c61de2c */
L8c61dc74: fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61dc76; /* fmov.s @r12,fr2 */
L8c61dc76: r[0]=4u;goto L8c61dc78; /* mov #0x4,r0 */
L8c61dc78: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61dc7a; /* fmov.s @r13,fr3 */
L8c61dc7a: r[4]=r[15];goto L8c61dc7c; /* mov r15,r4 */
L8c61dc7c: r[3]=rd(m,bus,&io_status,0x8c61de5cu,4);if(io_status)return io_status;goto L8c61dc7e; /* mov.l 0x8c61de5c,r3 */
L8c61dc7e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61dc80; /* fadd fr3,fr2 */
L8c61dc80: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61dc82; /* fmov.s @r14,fr3 */
L8c61dc82: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61dc84; /* fadd fr3,fr2 */
L8c61dc84: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61dc86; /* fmov fr2,@(r0,r15) */
L8c61dc86: r[0]=4u;goto L8c61dc88; /* mov #0x4,r0 */
L8c61dc88: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dc8a; /* fmov.s @(r0,r12),fr2 */
L8c61dc8a: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dc8c; /* fmov.s @(r0,r13),fr3 */
L8c61dc8c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61dc8e; /* fadd fr3,fr2 */
L8c61dc8e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dc90; /* fmov.s @(r0,r14),fr3 */
L8c61dc90: r[0]=8u;goto L8c61dc92; /* mov #0x8,r0 */
L8c61dc92: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61dc94; /* fadd fr3,fr2 */
L8c61dc94: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61dc96; /* fmov fr2,@(r0,r15) */
L8c61dc96: r[0]=8u;goto L8c61dc98; /* mov #0x8,r0 */
L8c61dc98: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dc9a; /* fmov.s @(r0,r12),fr2 */
L8c61dc9a: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dc9c; /* fmov.s @(r0,r13),fr3 */
L8c61dc9c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61dc9e; /* fadd fr3,fr2 */
L8c61dc9e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dca0; /* fmov.s @(r0,r14),fr3 */
L8c61dca0: r[0]=12u;goto L8c61dca2; /* mov #0xc,r0 */
L8c61dca2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61dca4; /* fadd fr3,fr2 */
L8c61dca4: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61dca6; /* fmov fr2,@(r0,r15) */
L8c61dca6: target=r[3];*pr=0x8c61dcaau;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61dcaa; /* jsr @r3 */
L8c61dcaa: r[3]=rd(m,bus,&io_status,0x8c61de60u,4);if(io_status)return io_status;goto L8c61dcac; /* mov.l 0x8c61de60,r3 */
L8c61dcac: r[0]=4u;goto L8c61dcae; /* mov #0x4,r0 */
L8c61dcae: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61dcb0; /* fmov.s @(r0,r15),fr3 */
L8c61dcb0: r[0]=8u;goto L8c61dcb2; /* mov #0x8,r0 */
L8c61dcb2: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61dcb4; /* fmov.s @r3,fr2 */
L8c61dcb4: r[3]+=8u;goto L8c61dcb6; /* add #0x8,r3 */
L8c61dcb6: r[2]=rd(m,bus,&io_status,0x8c61de64u,4);if(io_status)return io_status;goto L8c61dcb8; /* mov.l 0x8c61de64,r2 */
L8c61dcb8: r[8]+=24u;goto L8c61dcba; /* add #0x18,r8 */
L8c61dcba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dcbc; /* fmul fr3,fr2 */
L8c61dcbc: r[1]=rd(m,bus,&io_status,0x8c61de68u,4);if(io_status)return io_status;goto L8c61dcbe; /* mov.l 0x8c61de68,r1 */
L8c61dcbe: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61dcc0; /* fmov.s @r2,fr3 */
L8c61dcc0: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61dcc2; /* fmov.s @r1,fr1 */
L8c61dcc2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61dcc4; /* fsub fr2,fr3 */
L8c61dcc4: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61dcc6; /* fmov.s @(r0,r15),fr2 */
L8c61dcc6: r[0]=12u;goto L8c61dcc8; /* mov #0xc,r0 */
L8c61dcc8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61dcca; /* fmul fr2,fr1 */
L8c61dcca: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61dccc; /* fmov.s @(r0,r15),fr2 */
L8c61dccc: r[0]=rd(m,bus,&io_status,0x8c61de6cu,4);if(io_status)return io_status;goto L8c61dcce; /* mov.l 0x8c61de6c,r0 */
L8c61dcce: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61dcd0; /* mov.l @r0,r2 */
L8c61dcd0: r[0]=4u;goto L8c61dcd2; /* mov #0x4,r0 */
L8c61dcd2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61dcd4; /* fsub fr1,fr3 */
L8c61dcd4: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61dcd6; /* fmov.s @r3,fr1 */
L8c61dcd6: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dcd8; /* fmov.s @(r0,r12),fr0 */
L8c61dcd8: r[1]=r[2];goto L8c61dcda; /* mov r2,r1 */
L8c61dcda: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61dcdc; /* fmul fr2,fr1 */
L8c61dcdc: r[0]=20u;goto L8c61dcde; /* mov #0x14,r0 */
L8c61dcde: r[3]=r[2];goto L8c61dce0; /* mov r2,r3 */
L8c61dce0: fcopy(f,4u,3u);goto L8c61dce2; /* fmov fr3,fr4 */
L8c61dce2: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61dce4; /* fmov.s @(r0,r1),fr3 */
L8c61dce4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61dce6; /* fsub fr1,fr4 */
L8c61dce6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dce8; /* fmul fr0,fr3 */
L8c61dce8: fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61dcea; /* fmov.s @r12,fr0 */
L8c61dcea: r[0]=4u;goto L8c61dcec; /* mov #0x4,r0 */
L8c61dcec: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61dcee; /* fmov.s @(r0,r3),fr2 */
L8c61dcee: r[0]=8u;goto L8c61dcf0; /* mov #0x8,r0 */
L8c61dcf0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dcf2; /* fmac fr0,fr2,fr3 */
L8c61dcf2: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dcf4; /* fmov.s @(r0,r12),fr0 */
L8c61dcf4: r[0]=36u;goto L8c61dcf6; /* mov #0x24,r0 */
L8c61dcf6: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61dcf8; /* fmov.s @(r0,r2),fr2 */
L8c61dcf8: r[0]=4u;goto L8c61dcfa; /* mov #0x4,r0 */
L8c61dcfa: r[2]=rd(m,bus,&io_status,0x8c61de6cu,4);if(io_status)return io_status;goto L8c61dcfc; /* mov.l 0x8c61de6c,r2 */
L8c61dcfc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dcfe; /* fmac fr0,fr2,fr3 */
L8c61dcfe: fcopy(f,0u,15u);goto L8c61dd00; /* fmov fr15,fr0 */
L8c61dd00: fcopy(f,2u,15u);goto L8c61dd02; /* fmov fr15,fr2 */
L8c61dd02: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dd04; /* fmac fr0,fr3,fr2 */
L8c61dd04: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61dd06; /* fmov.s fr2,@-r8 */
L8c61dd06: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61dd08; /* mov.l @r2,r3 */
L8c61dd08: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dd0a; /* fmov.s @(r0,r12),fr0 */
L8c61dd0a: r[0]=16u;goto L8c61dd0c; /* mov #0x10,r0 */
L8c61dd0c: r[2]=r[3];goto L8c61dd0e; /* mov r3,r2 */
L8c61dd0e: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61dd10; /* fmov.s @(r0,r2),fr3 */
L8c61dd10: r[1]=r[3];goto L8c61dd12; /* mov r3,r1 */
L8c61dd12: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61dd14; /* fmov.s @r1,fr2 */
L8c61dd14: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd16; /* fmul fr0,fr3 */
L8c61dd16: fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61dd18; /* fmov.s @r12,fr0 */
L8c61dd18: r[0]=8u;goto L8c61dd1a; /* mov #0x8,r0 */
L8c61dd1a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd1c; /* fmac fr0,fr2,fr3 */
L8c61dd1c: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dd1e; /* fmov.s @(r0,r12),fr0 */
L8c61dd1e: r[0]=32u;goto L8c61dd20; /* mov #0x20,r0 */
L8c61dd20: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61dd22; /* fmov.s @(r0,r3),fr2 */
L8c61dd22: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd24; /* fmac fr0,fr2,fr3 */
L8c61dd24: fcopy(f,0u,15u);goto L8c61dd26; /* fmov fr15,fr0 */
L8c61dd26: fcopy(f,2u,15u);goto L8c61dd28; /* fmov fr15,fr2 */
L8c61dd28: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dd2a; /* fmac fr0,fr3,fr2 */
L8c61dd2a: r[0]=24u;goto L8c61dd2c; /* mov #0x18,r0 */
L8c61dd2c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61dd2e; /* fmov.s fr2,@-r8 */
L8c61dd2e: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dd30; /* fmov.s @(r0,r12),fr3 */
L8c61dd30: r[0]=20u;goto L8c61dd32; /* mov #0x14,r0 */
L8c61dd32: r[3]=rd(m,bus,&io_status,0x8c61de70u,4);if(io_status)return io_status;goto L8c61dd34; /* mov.l 0x8c61de70,r3 */
L8c61dd34: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61dd36; /* fmov.s fr3,@-r8 */
L8c61dd36: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dd38; /* fmov.s @(r0,r12),fr3 */
L8c61dd38: r[0]=16u;goto L8c61dd3a; /* mov #0x10,r0 */
L8c61dd3a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61dd3c; /* fmov.s fr3,@-r8 */
L8c61dd3c: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dd3e; /* fmov.s @(r0,r12),fr3 */
L8c61dd3e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61dd40; /* fmov.s fr3,@-r8 */
L8c61dd40: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61dd42; /* mov.l r3,@-r8 */
L8c61dd42: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61dd44; /* pref @r8 */
L8c61dd44: r[2]=rd(m,bus,&io_status,0x8c61de6cu,4);if(io_status)return io_status;goto L8c61dd46; /* mov.l 0x8c61de6c,r2 */
L8c61dd46: r[0]=4u;goto L8c61dd48; /* mov #0x4,r0 */
L8c61dd48: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dd4a; /* fmov.s @(r0,r13),fr0 */
L8c61dd4a: r[0]=20u;goto L8c61dd4c; /* mov #0x14,r0 */
L8c61dd4c: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61dd4e; /* mov.l @r2,r3 */
L8c61dd4e: r[8]+=56u;goto L8c61dd50; /* add #0x38,r8 */
L8c61dd50: r[2]=r[3];goto L8c61dd52; /* mov r3,r2 */
L8c61dd52: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61dd54; /* fmov.s @(r0,r2),fr3 */
L8c61dd54: r[0]=4u;goto L8c61dd56; /* mov #0x4,r0 */
L8c61dd56: r[1]=r[3];goto L8c61dd58; /* mov r3,r1 */
L8c61dd58: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd5a; /* fmul fr0,fr3 */
L8c61dd5a: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61dd5c; /* fmov.s @(r0,r1),fr2 */
L8c61dd5c: fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61dd5e; /* fmov.s @r13,fr0 */
L8c61dd5e: r[0]=8u;goto L8c61dd60; /* mov #0x8,r0 */
L8c61dd60: r[2]=rd(m,bus,&io_status,0x8c61de6cu,4);if(io_status)return io_status;goto L8c61dd62; /* mov.l 0x8c61de6c,r2 */
L8c61dd62: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd64; /* fmac fr0,fr2,fr3 */
L8c61dd64: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dd66; /* fmov.s @(r0,r13),fr0 */
L8c61dd66: r[0]=36u;goto L8c61dd68; /* mov #0x24,r0 */
L8c61dd68: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61dd6a; /* fmov.s @(r0,r3),fr2 */
L8c61dd6a: r[0]=4u;goto L8c61dd6c; /* mov #0x4,r0 */
L8c61dd6c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd6e; /* fmac fr0,fr2,fr3 */
L8c61dd6e: fcopy(f,0u,15u);goto L8c61dd70; /* fmov fr15,fr0 */
L8c61dd70: fcopy(f,2u,15u);goto L8c61dd72; /* fmov fr15,fr2 */
L8c61dd72: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dd74; /* fmac fr0,fr3,fr2 */
L8c61dd74: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61dd76; /* fmov.s fr2,@-r8 */
L8c61dd76: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61dd78; /* mov.l @r2,r3 */
L8c61dd78: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dd7a; /* fmov.s @(r0,r13),fr0 */
L8c61dd7a: r[0]=16u;goto L8c61dd7c; /* mov #0x10,r0 */
L8c61dd7c: r[2]=r[3];goto L8c61dd7e; /* mov r3,r2 */
L8c61dd7e: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61dd80; /* fmov.s @(r0,r2),fr3 */
L8c61dd80: r[1]=r[3];goto L8c61dd82; /* mov r3,r1 */
L8c61dd82: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd84; /* fmul fr0,fr3 */
L8c61dd84: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61dd86; /* fmov.s @r1,fr2 */
L8c61dd86: r[0]=8u;goto L8c61dd88; /* mov #0x8,r0 */
L8c61dd88: fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61dd8a; /* fmov.s @r13,fr0 */
L8c61dd8a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd8c; /* fmac fr0,fr2,fr3 */
L8c61dd8c: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dd8e; /* fmov.s @(r0,r13),fr0 */
L8c61dd8e: r[0]=32u;goto L8c61dd90; /* mov #0x20,r0 */
L8c61dd90: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61dd92; /* fmov.s @(r0,r3),fr2 */
L8c61dd92: r[0]=24u;goto L8c61dd94; /* mov #0x18,r0 */
L8c61dd94: r[3]=rd(m,bus,&io_status,0x8c61de70u,4);if(io_status)return io_status;goto L8c61dd96; /* mov.l 0x8c61de70,r3 */
L8c61dd96: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd98; /* fmac fr0,fr2,fr3 */
L8c61dd98: fcopy(f,0u,15u);goto L8c61dd9a; /* fmov fr15,fr0 */
L8c61dd9a: fcopy(f,2u,15u);goto L8c61dd9c; /* fmov fr15,fr2 */
L8c61dd9c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dd9e; /* fmac fr0,fr3,fr2 */
L8c61dd9e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61dda0; /* fmov.s fr2,@-r8 */
L8c61dda0: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dda2; /* fmov.s @(r0,r13),fr3 */
L8c61dda2: r[0]=20u;goto L8c61dda4; /* mov #0x14,r0 */
L8c61dda4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61dda6; /* fmov.s fr3,@-r8 */
L8c61dda6: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dda8; /* fmov.s @(r0,r13),fr3 */
L8c61dda8: r[0]=16u;goto L8c61ddaa; /* mov #0x10,r0 */
L8c61ddaa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ddac; /* fmov.s fr3,@-r8 */
L8c61ddac: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ddae; /* fmov.s @(r0,r13),fr3 */
L8c61ddae: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ddb0; /* fmov.s fr3,@-r8 */
L8c61ddb0: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61ddb2; /* mov.l r3,@-r8 */
L8c61ddb2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61ddb4; /* pref @r8 */
L8c61ddb4: r[2]=rd(m,bus,&io_status,0x8c61de6cu,4);if(io_status)return io_status;goto L8c61ddb6; /* mov.l 0x8c61de6c,r2 */
L8c61ddb6: r[0]=4u;goto L8c61ddb8; /* mov #0x4,r0 */
L8c61ddb8: r[8]+=60u;goto L8c61ddba; /* add #0x3c,r8 */
L8c61ddba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61ddbc; /* fmov.s fr4,@-r8 */
L8c61ddbc: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ddbe; /* mov.l @r2,r3 */
L8c61ddbe: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ddc0; /* fmov.s @(r0,r14),fr0 */
L8c61ddc0: r[0]=20u;goto L8c61ddc2; /* mov #0x14,r0 */
L8c61ddc2: r[2]=r[3];goto L8c61ddc4; /* mov r3,r2 */
L8c61ddc4: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61ddc6; /* fmov.s @(r0,r2),fr3 */
L8c61ddc6: r[1]=r[3];goto L8c61ddc8; /* mov r3,r1 */
L8c61ddc8: r[0]=4u;goto L8c61ddca; /* mov #0x4,r0 */
L8c61ddca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ddcc; /* fmul fr0,fr3 */
L8c61ddcc: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61ddce; /* fmov.s @(r0,r1),fr2 */
L8c61ddce: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61ddd0; /* fmov.s @r14,fr0 */
L8c61ddd0: r[0]=8u;goto L8c61ddd2; /* mov #0x8,r0 */
L8c61ddd2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ddd4; /* fmac fr0,fr2,fr3 */
L8c61ddd4: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ddd6; /* fmov.s @(r0,r14),fr0 */
L8c61ddd6: r[0]=36u;goto L8c61ddd8; /* mov #0x24,r0 */
L8c61ddd8: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61ddda; /* fmov.s @(r0,r3),fr2 */
L8c61ddda: r[0]=4u;goto L8c61dddc; /* mov #0x4,r0 */
L8c61dddc: r[3]=rd(m,bus,&io_status,0x8c61de6cu,4);if(io_status)return io_status;goto L8c61ddde; /* mov.l 0x8c61de6c,r3 */
L8c61ddde: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dde0; /* fmac fr0,fr2,fr3 */
L8c61dde0: fcopy(f,2u,15u);goto L8c61dde2; /* fmov fr15,fr2 */
L8c61dde2: fcopy(f,0u,15u);goto L8c61dde4; /* fmov fr15,fr0 */
L8c61dde4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dde6; /* fmac fr0,fr3,fr2 */
L8c61dde6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61dde8; /* fmov.s fr2,@-r8 */
L8c61dde8: r[9]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ddea; /* mov.l @r3,r9 */
L8c61ddea: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ddec; /* fmov.s @(r0,r14),fr0 */
L8c61ddec: r[0]=16u;goto L8c61ddee; /* mov #0x10,r0 */
L8c61ddee: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61ddf0; /* fmov.s @(r0,r9),fr3 */
L8c61ddf0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ddf2; /* fmul fr0,fr3 */
L8c61ddf2: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61ddf4; /* fmov.s @r14,fr0 */
L8c61ddf4: fload(m,bus,&io_status,f,2u,r[9]);if(io_status)return io_status;goto L8c61ddf6; /* fmov.s @r9,fr2 */
L8c61ddf6: r[0]=8u;goto L8c61ddf8; /* mov #0x8,r0 */
L8c61ddf8: r[2]=rd(m,bus,&io_status,0x8c61de74u,4);if(io_status)return io_status;goto L8c61ddfa; /* mov.l 0x8c61de74,r2 */
L8c61ddfa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ddfc; /* fmac fr0,fr2,fr3 */
L8c61ddfc: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ddfe; /* fmov.s @(r0,r14),fr0 */
L8c61ddfe: r[0]=32u;goto L8c61de00; /* mov #0x20,r0 */
L8c61de00: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61de02; /* fmov.s @(r0,r9),fr2 */
L8c61de02: r[0]=24u;goto L8c61de04; /* mov #0x18,r0 */
L8c61de04: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61de06; /* fmac fr0,fr2,fr3 */
L8c61de06: fcopy(f,0u,15u);goto L8c61de08; /* fmov fr15,fr0 */
L8c61de08: fcopy(f,2u,15u);goto L8c61de0a; /* fmov fr15,fr2 */
L8c61de0a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61de0c; /* fmac fr0,fr3,fr2 */
L8c61de0c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61de0e; /* fmov.s fr2,@-r8 */
L8c61de0e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61de10; /* fmov.s @(r0,r14),fr3 */
L8c61de10: r[0]=20u;goto L8c61de12; /* mov #0x14,r0 */
L8c61de12: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61de14; /* fmov.s fr3,@-r8 */
L8c61de14: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61de16; /* fmov.s @(r0,r14),fr3 */
L8c61de16: r[0]=16u;goto L8c61de18; /* mov #0x10,r0 */
L8c61de18: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61de1a; /* fmov.s fr3,@-r8 */
L8c61de1a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61de1c; /* fmov.s @(r0,r14),fr3 */
L8c61de1c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61de1e; /* fmov.s fr3,@-r8 */
L8c61de1e: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61de20; /* mov.l r2,@-r8 */
L8c61de20: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61de22; /* pref @r8 */
L8c61de22: r[1]=rd(m,bus,&io_status,0x8c61de78u,4);if(io_status)return io_status;goto L8c61de24; /* mov.l 0x8c61de78,r1 */
L8c61de24: r[8]+=32u;goto L8c61de26; /* add #0x20,r8 */
L8c61de26: r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61de28; /* mov.l @r1,r2 */
L8c61de28: r[2]+=3u;goto L8c61de2a; /* add #0x3,r2 */
L8c61de2a: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61de2c; /* mov.l r2,@r1 */
L8c61de2c: r[11]+=1u;goto L8c61de2e; /* add #0x1,r11 */
L8c61de2e: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61de30; /* mov.l @r15,r3 */
L8c61de30: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61de32; /* mov.w @(0x2,r3),r0 */
L8c61de32: r[0]=r[0]&65535u;goto L8c61de34; /* extu.w r0,r0 */
L8c61de34: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61de36; /* cmp/ge r0,r11 */
L8c61de36: if((*sr&1u))goto L8c61de3c;goto L8c61de38; /* bt 0x8c61de3c */
L8c61de38: goto L8c61dc1c; /* bra 0x8c61dc1c */
L8c61de3c: goto L8c61e4a8; /* bra 0x8c61e4a8 */
L8c61de40: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61de42; /* mov.l @r15,r1 */
L8c61de42: r[2]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61de44; /* mov.l @r13,r2 */
L8c61de44: r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61de46; /* mov.w @(0x2,r1),r0 */
L8c61de46: r[0]=r[0]&65535u;goto L8c61de48; /* extu.w r0,r0 */
L8c61de48: r[0]<<=2;goto L8c61de4a; /* shll2 r0 */
L8c61de4a: r[2]+=r[0];goto L8c61de4c; /* add r0,r2 */
L8c61de4c: wr(m,bus,&io_status,r[13],r[2],4);if(io_status)return io_status;goto L8c61de4e; /* mov.l r2,@r13 */
L8c61de4e: r[9]=r[4];goto L8c61e110; /* bra 0x8c61e110 */
L8c61de7c: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61de7e; /* mov.w @r10+,r14 */
L8c61de7e: r[0]=16u;goto L8c61de80; /* mov #0x10,r0 */
L8c61de80: r[3]=rd(m,bus,&io_status,0x8c61e18cu,4);if(io_status)return io_status;goto L8c61de82; /* mov.l 0x8c61e18c,r3 */
L8c61de82: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61de84; /* mov.w @r10+,r13 */
L8c61de84: r[14]=r[14]&65535u;goto L8c61de86; /* extu.w r14,r14 */
L8c61de86: r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61de88; /* mov.l @r3,r4 */
L8c61de88: r[14]<<=2;goto L8c61de8a; /* shll2 r14 */
L8c61de8a: r[13]=r[13]&65535u;goto L8c61de8c; /* extu.w r13,r13 */
L8c61de8c: r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61de8e; /* mov.w @r10+,r12 */
L8c61de8e: r[13]<<=2;goto L8c61de90; /* shll2 r13 */
L8c61de90: r[13]<<=2;goto L8c61de92; /* shll2 r13 */
L8c61de92: r[11]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61de94; /* mov.w @r10+,r11 */
L8c61de94: r[14]<<=2;goto L8c61de96; /* shll2 r14 */
L8c61de96: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61de98; /* shll r14 */
L8c61de98: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61de9a; /* shll r13 */
L8c61de9a: r[13]+=r[4];goto L8c61de9c; /* add r4,r13 */
L8c61de9c: r[14]+=r[4];goto L8c61de9e; /* add r4,r14 */
L8c61de9e: fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dea0; /* fmov.s @(r0,r13),fr5 */
L8c61dea0: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dea2; /* fmov.s @(r0,r14),fr3 */
L8c61dea2: r[12]=r[12]&65535u;goto L8c61dea4; /* extu.w r12,r12 */
L8c61dea4: fcopy(f,2u,5u);goto L8c61dea6; /* fmov fr5,fr2 */
L8c61dea6: r[12]<<=2;goto L8c61dea8; /* shll2 r12 */
L8c61dea8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61deaa; /* fsub fr3,fr2 */
L8c61deaa: r[11]=r[11]&65535u;goto L8c61deac; /* extu.w r11,r11 */
L8c61deac: r[12]<<=2;goto L8c61deae; /* shll2 r12 */
L8c61deae: r[11]<<=2;goto L8c61deb0; /* shll2 r11 */
L8c61deb0: r[0]=20u;goto L8c61deb2; /* mov #0x14,r0 */
L8c61deb2: fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61deb4; /* fmov.s @(r0,r13),fr4 */
L8c61deb4: r[11]<<=2;goto L8c61deb6; /* shll2 r11 */
L8c61deb6: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61deb8; /* shll r12 */
L8c61deb8: r[12]+=r[4];goto L8c61deba; /* add r4,r12 */
L8c61deba: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61debc; /* shll r11 */
L8c61debc: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61debe; /* fmov.s @(r0,r12),fr3 */
L8c61debe: fcopy(f,1u,4u);goto L8c61dec0; /* fmov fr4,fr1 */
L8c61dec0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61dec2; /* fsub fr4,fr3 */
L8c61dec2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dec4; /* fmul fr3,fr2 */
L8c61dec4: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dec6; /* fmov.s @(r0,r14),fr3 */
L8c61dec6: r[0]=16u;goto L8c61dec8; /* mov #0x10,r0 */
L8c61dec8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61deca; /* fsub fr3,fr1 */
L8c61deca: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61decc; /* fmov.s @(r0,r12),fr3 */
L8c61decc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61dece; /* fsub fr5,fr3 */
L8c61dece: fcopy(f,4u,2u);goto L8c61ded0; /* fmov fr2,fr4 */
L8c61ded0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ded2; /* fmul fr3,fr1 */
L8c61ded2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61ded4; /* fldi0 fr3 */
L8c61ded4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61ded6; /* fsub fr1,fr4 */
L8c61ded6: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61ded8; /* fcmp/gt fr3,fr4 */
L8c61ded8: take=!(*sr&1u);r[11]+=r[4];if(take)goto L8c61dee0;goto L8c61dedc; /* bf/s 0x8c61dee0 */
L8c61dedc: goto L8c61e10e; /* bra 0x8c61e10e */
L8c61dee0: fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c61dee2; /* fmov.s @r14,fr2 */
L8c61dee2: r[0]=4u;goto L8c61dee4; /* mov #0x4,r0 */
L8c61dee4: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61dee6; /* fmov.s @r13,fr3 */
L8c61dee6: r[4]=r[15];goto L8c61dee8; /* mov r15,r4 */
L8c61dee8: r[3]=rd(m,bus,&io_status,0x8c61e190u,4);if(io_status)return io_status;goto L8c61deea; /* mov.l 0x8c61e190,r3 */
L8c61deea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61deec; /* fadd fr3,fr2 */
L8c61deec: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61deee; /* fmov.s @r12,fr3 */
L8c61deee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61def0; /* fadd fr3,fr2 */
L8c61def0: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61def2; /* fmov fr2,@(r0,r15) */
L8c61def2: r[0]=4u;goto L8c61def4; /* mov #0x4,r0 */
L8c61def4: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61def6; /* fmov.s @(r0,r14),fr2 */
L8c61def6: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61def8; /* fmov.s @(r0,r13),fr3 */
L8c61def8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61defa; /* fadd fr3,fr2 */
L8c61defa: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61defc; /* fmov.s @(r0,r12),fr3 */
L8c61defc: r[0]=8u;goto L8c61defe; /* mov #0x8,r0 */
L8c61defe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61df00; /* fadd fr3,fr2 */
L8c61df00: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61df02; /* fmov fr2,@(r0,r15) */
L8c61df02: r[0]=8u;goto L8c61df04; /* mov #0x8,r0 */
L8c61df04: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61df06; /* fmov.s @(r0,r14),fr2 */
L8c61df06: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61df08; /* fmov.s @(r0,r13),fr3 */
L8c61df08: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61df0a; /* fadd fr3,fr2 */
L8c61df0a: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61df0c; /* fmov.s @(r0,r12),fr3 */
L8c61df0c: r[0]=12u;goto L8c61df0e; /* mov #0xc,r0 */
L8c61df0e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61df10; /* fadd fr3,fr2 */
L8c61df10: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61df12; /* fmov fr2,@(r0,r15) */
L8c61df12: target=r[3];*pr=0x8c61df16u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61df16; /* jsr @r3 */
L8c61df16: r[3]=rd(m,bus,&io_status,0x8c61e194u,4);if(io_status)return io_status;goto L8c61df18; /* mov.l 0x8c61e194,r3 */
L8c61df18: r[0]=4u;goto L8c61df1a; /* mov #0x4,r0 */
L8c61df1a: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61df1c; /* fmov.s @(r0,r15),fr3 */
L8c61df1c: r[0]=8u;goto L8c61df1e; /* mov #0x8,r0 */
L8c61df1e: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61df20; /* fmov.s @r3,fr2 */
L8c61df20: r[3]+=8u;goto L8c61df22; /* add #0x8,r3 */
L8c61df22: r[2]=rd(m,bus,&io_status,0x8c61e198u,4);if(io_status)return io_status;goto L8c61df24; /* mov.l 0x8c61e198,r2 */
L8c61df24: r[8]+=24u;goto L8c61df26; /* add #0x18,r8 */
L8c61df26: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61df28; /* fmul fr3,fr2 */
L8c61df28: r[1]=rd(m,bus,&io_status,0x8c61e19cu,4);if(io_status)return io_status;goto L8c61df2a; /* mov.l 0x8c61e19c,r1 */
L8c61df2a: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61df2c; /* fmov.s @r2,fr3 */
L8c61df2c: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61df2e; /* fmov.s @r1,fr1 */
L8c61df2e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61df30; /* fsub fr2,fr3 */
L8c61df30: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61df32; /* fmov.s @(r0,r15),fr2 */
L8c61df32: r[0]=12u;goto L8c61df34; /* mov #0xc,r0 */
L8c61df34: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61df36; /* fmul fr2,fr1 */
L8c61df36: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61df38; /* fmov.s @(r0,r15),fr2 */
L8c61df38: r[0]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61df3a; /* mov.l 0x8c61e1a0,r0 */
L8c61df3a: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61df3c; /* mov.l @r0,r2 */
L8c61df3c: r[0]=4u;goto L8c61df3e; /* mov #0x4,r0 */
L8c61df3e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61df40; /* fsub fr1,fr3 */
L8c61df40: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61df42; /* fmov.s @r3,fr1 */
L8c61df42: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61df44; /* fmov.s @(r0,r14),fr0 */
L8c61df44: r[1]=r[2];goto L8c61df46; /* mov r2,r1 */
L8c61df46: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61df48; /* fmul fr2,fr1 */
L8c61df48: r[0]=20u;goto L8c61df4a; /* mov #0x14,r0 */
L8c61df4a: r[3]=r[2];goto L8c61df4c; /* mov r2,r3 */
L8c61df4c: fcopy(f,4u,3u);goto L8c61df4e; /* fmov fr3,fr4 */
L8c61df4e: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61df50; /* fmov.s @(r0,r1),fr3 */
L8c61df50: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61df52; /* fsub fr1,fr4 */
L8c61df52: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61df54; /* fmul fr0,fr3 */
L8c61df54: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61df56; /* fmov.s @r14,fr0 */
L8c61df56: r[0]=4u;goto L8c61df58; /* mov #0x4,r0 */
L8c61df58: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61df5a; /* fmov.s @(r0,r3),fr2 */
L8c61df5a: r[0]=8u;goto L8c61df5c; /* mov #0x8,r0 */
L8c61df5c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61df5e; /* fmac fr0,fr2,fr3 */
L8c61df5e: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61df60; /* fmov.s @(r0,r14),fr0 */
L8c61df60: r[0]=36u;goto L8c61df62; /* mov #0x24,r0 */
L8c61df62: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61df64; /* fmov.s @(r0,r2),fr2 */
L8c61df64: r[0]=4u;goto L8c61df66; /* mov #0x4,r0 */
L8c61df66: r[2]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61df68; /* mov.l 0x8c61e1a0,r2 */
L8c61df68: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61df6a; /* fmac fr0,fr2,fr3 */
L8c61df6a: fcopy(f,0u,15u);goto L8c61df6c; /* fmov fr15,fr0 */
L8c61df6c: fcopy(f,2u,15u);goto L8c61df6e; /* fmov fr15,fr2 */
L8c61df6e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61df70; /* fmac fr0,fr3,fr2 */
L8c61df70: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61df72; /* fmov.s fr2,@-r8 */
L8c61df72: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61df74; /* mov.l @r2,r3 */
L8c61df74: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61df76; /* fmov.s @(r0,r14),fr0 */
L8c61df76: r[0]=16u;goto L8c61df78; /* mov #0x10,r0 */
L8c61df78: r[2]=r[3];goto L8c61df7a; /* mov r3,r2 */
L8c61df7a: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61df7c; /* fmov.s @(r0,r2),fr3 */
L8c61df7c: r[1]=r[3];goto L8c61df7e; /* mov r3,r1 */
L8c61df7e: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61df80; /* fmov.s @r1,fr2 */
L8c61df80: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61df82; /* fmul fr0,fr3 */
L8c61df82: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61df84; /* fmov.s @r14,fr0 */
L8c61df84: r[0]=8u;goto L8c61df86; /* mov #0x8,r0 */
L8c61df86: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61df88; /* fmac fr0,fr2,fr3 */
L8c61df88: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61df8a; /* fmov.s @(r0,r14),fr0 */
L8c61df8a: r[0]=32u;goto L8c61df8c; /* mov #0x20,r0 */
L8c61df8c: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61df8e; /* fmov.s @(r0,r3),fr2 */
L8c61df8e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61df90; /* fmac fr0,fr2,fr3 */
L8c61df90: fcopy(f,0u,15u);goto L8c61df92; /* fmov fr15,fr0 */
L8c61df92: fcopy(f,2u,15u);goto L8c61df94; /* fmov fr15,fr2 */
L8c61df94: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61df96; /* fmac fr0,fr3,fr2 */
L8c61df96: r[0]=24u;goto L8c61df98; /* mov #0x18,r0 */
L8c61df98: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61df9a; /* fmov.s fr2,@-r8 */
L8c61df9a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61df9c; /* fmov.s @(r0,r14),fr3 */
L8c61df9c: r[0]=20u;goto L8c61df9e; /* mov #0x14,r0 */
L8c61df9e: r[3]=rd(m,bus,&io_status,0x8c61e1a4u,4);if(io_status)return io_status;goto L8c61dfa0; /* mov.l 0x8c61e1a4,r3 */
L8c61dfa0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61dfa2; /* fmov.s fr3,@-r8 */
L8c61dfa2: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dfa4; /* fmov.s @(r0,r14),fr3 */
L8c61dfa4: r[0]=16u;goto L8c61dfa6; /* mov #0x10,r0 */
L8c61dfa6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61dfa8; /* fmov.s fr3,@-r8 */
L8c61dfa8: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dfaa; /* fmov.s @(r0,r14),fr3 */
L8c61dfaa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61dfac; /* fmov.s fr3,@-r8 */
L8c61dfac: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61dfae; /* mov.l r3,@-r8 */
L8c61dfae: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61dfb0; /* pref @r8 */
L8c61dfb0: r[2]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61dfb2; /* mov.l 0x8c61e1a0,r2 */
L8c61dfb2: r[0]=4u;goto L8c61dfb4; /* mov #0x4,r0 */
L8c61dfb4: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dfb6; /* fmov.s @(r0,r13),fr0 */
L8c61dfb6: r[0]=20u;goto L8c61dfb8; /* mov #0x14,r0 */
L8c61dfb8: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61dfba; /* mov.l @r2,r3 */
L8c61dfba: r[8]+=56u;goto L8c61dfbc; /* add #0x38,r8 */
L8c61dfbc: r[2]=r[3];goto L8c61dfbe; /* mov r3,r2 */
L8c61dfbe: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61dfc0; /* fmov.s @(r0,r2),fr3 */
L8c61dfc0: r[0]=4u;goto L8c61dfc2; /* mov #0x4,r0 */
L8c61dfc2: r[1]=r[3];goto L8c61dfc4; /* mov r3,r1 */
L8c61dfc4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dfc6; /* fmul fr0,fr3 */
L8c61dfc6: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61dfc8; /* fmov.s @(r0,r1),fr2 */
L8c61dfc8: fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61dfca; /* fmov.s @r13,fr0 */
L8c61dfca: r[0]=8u;goto L8c61dfcc; /* mov #0x8,r0 */
L8c61dfcc: r[2]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61dfce; /* mov.l 0x8c61e1a0,r2 */
L8c61dfce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dfd0; /* fmac fr0,fr2,fr3 */
L8c61dfd0: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dfd2; /* fmov.s @(r0,r13),fr0 */
L8c61dfd2: r[0]=36u;goto L8c61dfd4; /* mov #0x24,r0 */
L8c61dfd4: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61dfd6; /* fmov.s @(r0,r3),fr2 */
L8c61dfd6: r[0]=4u;goto L8c61dfd8; /* mov #0x4,r0 */
L8c61dfd8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dfda; /* fmac fr0,fr2,fr3 */
L8c61dfda: fcopy(f,0u,15u);goto L8c61dfdc; /* fmov fr15,fr0 */
L8c61dfdc: fcopy(f,2u,15u);goto L8c61dfde; /* fmov fr15,fr2 */
L8c61dfde: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dfe0; /* fmac fr0,fr3,fr2 */
L8c61dfe0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61dfe2; /* fmov.s fr2,@-r8 */
L8c61dfe2: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61dfe4; /* mov.l @r2,r3 */
L8c61dfe4: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dfe6; /* fmov.s @(r0,r13),fr0 */
L8c61dfe6: r[0]=16u;goto L8c61dfe8; /* mov #0x10,r0 */
L8c61dfe8: r[2]=r[3];goto L8c61dfea; /* mov r3,r2 */
L8c61dfea: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61dfec; /* fmov.s @(r0,r2),fr3 */
L8c61dfec: r[1]=r[3];goto L8c61dfee; /* mov r3,r1 */
L8c61dfee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dff0; /* fmul fr0,fr3 */
L8c61dff0: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61dff2; /* fmov.s @r1,fr2 */
L8c61dff2: r[0]=8u;goto L8c61dff4; /* mov #0x8,r0 */
L8c61dff4: fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61dff6; /* fmov.s @r13,fr0 */
L8c61dff6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dff8; /* fmac fr0,fr2,fr3 */
L8c61dff8: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dffa; /* fmov.s @(r0,r13),fr0 */
L8c61dffa: r[0]=32u;goto L8c61dffc; /* mov #0x20,r0 */
L8c61dffc: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61dffe; /* fmov.s @(r0,r3),fr2 */
L8c61dffe: r[0]=24u;goto L8c61e000; /* mov #0x18,r0 */
L8c61e000: r[3]=rd(m,bus,&io_status,0x8c61e1a4u,4);if(io_status)return io_status;goto L8c61e002; /* mov.l 0x8c61e1a4,r3 */
L8c61e002: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e004; /* fmac fr0,fr2,fr3 */
L8c61e004: fcopy(f,0u,15u);goto L8c61e006; /* fmov fr15,fr0 */
L8c61e006: fcopy(f,2u,15u);goto L8c61e008; /* fmov fr15,fr2 */
L8c61e008: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e00a; /* fmac fr0,fr3,fr2 */
L8c61e00a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e00c; /* fmov.s fr2,@-r8 */
L8c61e00c: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e00e; /* fmov.s @(r0,r13),fr3 */
L8c61e00e: r[0]=20u;goto L8c61e010; /* mov #0x14,r0 */
L8c61e010: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e012; /* fmov.s fr3,@-r8 */
L8c61e012: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e014; /* fmov.s @(r0,r13),fr3 */
L8c61e014: r[0]=16u;goto L8c61e016; /* mov #0x10,r0 */
L8c61e016: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e018; /* fmov.s fr3,@-r8 */
L8c61e018: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e01a; /* fmov.s @(r0,r13),fr3 */
L8c61e01a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e01c; /* fmov.s fr3,@-r8 */
L8c61e01c: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61e01e; /* mov.l r3,@-r8 */
L8c61e01e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e020; /* pref @r8 */
L8c61e020: r[2]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61e022; /* mov.l 0x8c61e1a0,r2 */
L8c61e022: r[0]=4u;goto L8c61e024; /* mov #0x4,r0 */
L8c61e024: r[8]+=60u;goto L8c61e026; /* add #0x3c,r8 */
L8c61e026: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61e028; /* fmov.s fr4,@-r8 */
L8c61e028: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e02a; /* mov.l @r2,r3 */
L8c61e02a: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e02c; /* fmov.s @(r0,r12),fr0 */
L8c61e02c: r[0]=20u;goto L8c61e02e; /* mov #0x14,r0 */
L8c61e02e: r[2]=r[3];goto L8c61e030; /* mov r3,r2 */
L8c61e030: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e032; /* fmov.s @(r0,r2),fr3 */
L8c61e032: r[1]=r[3];goto L8c61e034; /* mov r3,r1 */
L8c61e034: r[0]=4u;goto L8c61e036; /* mov #0x4,r0 */
L8c61e036: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e038; /* fmul fr0,fr3 */
L8c61e038: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e03a; /* fmov.s @(r0,r1),fr2 */
L8c61e03a: fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61e03c; /* fmov.s @r12,fr0 */
L8c61e03c: r[0]=8u;goto L8c61e03e; /* mov #0x8,r0 */
L8c61e03e: r[2]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61e040; /* mov.l 0x8c61e1a0,r2 */
L8c61e040: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e042; /* fmac fr0,fr2,fr3 */
L8c61e042: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e044; /* fmov.s @(r0,r12),fr0 */
L8c61e044: r[0]=36u;goto L8c61e046; /* mov #0x24,r0 */
L8c61e046: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e048; /* fmov.s @(r0,r3),fr2 */
L8c61e048: r[0]=4u;goto L8c61e04a; /* mov #0x4,r0 */
L8c61e04a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e04c; /* fmac fr0,fr2,fr3 */
L8c61e04c: fcopy(f,0u,15u);goto L8c61e04e; /* fmov fr15,fr0 */
L8c61e04e: fcopy(f,2u,15u);goto L8c61e050; /* fmov fr15,fr2 */
L8c61e050: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e052; /* fmac fr0,fr3,fr2 */
L8c61e052: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e054; /* fmov.s fr2,@-r8 */
L8c61e054: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e056; /* mov.l @r2,r3 */
L8c61e056: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e058; /* fmov.s @(r0,r12),fr0 */
L8c61e058: r[0]=16u;goto L8c61e05a; /* mov #0x10,r0 */
L8c61e05a: r[2]=r[3];goto L8c61e05c; /* mov r3,r2 */
L8c61e05c: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e05e; /* fmov.s @(r0,r2),fr3 */
L8c61e05e: r[1]=r[3];goto L8c61e060; /* mov r3,r1 */
L8c61e060: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e062; /* fmul fr0,fr3 */
L8c61e062: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61e064; /* fmov.s @r1,fr2 */
L8c61e064: fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61e066; /* fmov.s @r12,fr0 */
L8c61e066: r[0]=8u;goto L8c61e068; /* mov #0x8,r0 */
L8c61e068: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e06a; /* fmac fr0,fr2,fr3 */
L8c61e06a: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e06c; /* fmov.s @(r0,r12),fr0 */
L8c61e06c: r[0]=32u;goto L8c61e06e; /* mov #0x20,r0 */
L8c61e06e: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e070; /* fmov.s @(r0,r3),fr2 */
L8c61e070: r[0]=24u;goto L8c61e072; /* mov #0x18,r0 */
L8c61e072: r[3]=rd(m,bus,&io_status,0x8c61e1a4u,4);if(io_status)return io_status;goto L8c61e074; /* mov.l 0x8c61e1a4,r3 */
L8c61e074: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e076; /* fmac fr0,fr2,fr3 */
L8c61e076: fcopy(f,0u,15u);goto L8c61e078; /* fmov fr15,fr0 */
L8c61e078: fcopy(f,2u,15u);goto L8c61e07a; /* fmov fr15,fr2 */
L8c61e07a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e07c; /* fmac fr0,fr3,fr2 */
L8c61e07c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e07e; /* fmov.s fr2,@-r8 */
L8c61e07e: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e080; /* fmov.s @(r0,r12),fr3 */
L8c61e080: r[0]=20u;goto L8c61e082; /* mov #0x14,r0 */
L8c61e082: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e084; /* fmov.s fr3,@-r8 */
L8c61e084: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e086; /* fmov.s @(r0,r12),fr3 */
L8c61e086: r[0]=16u;goto L8c61e088; /* mov #0x10,r0 */
L8c61e088: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e08a; /* fmov.s fr3,@-r8 */
L8c61e08a: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e08c; /* fmov.s @(r0,r12),fr3 */
L8c61e08c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e08e; /* fmov.s fr3,@-r8 */
L8c61e08e: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61e090; /* mov.l r3,@-r8 */
L8c61e090: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e092; /* pref @r8 */
L8c61e092: r[2]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61e094; /* mov.l 0x8c61e1a0,r2 */
L8c61e094: r[0]=4u;goto L8c61e096; /* mov #0x4,r0 */
L8c61e096: r[8]+=60u;goto L8c61e098; /* add #0x3c,r8 */
L8c61e098: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61e09a; /* fmov.s fr4,@-r8 */
L8c61e09a: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e09c; /* mov.l @r2,r3 */
L8c61e09c: fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e09e; /* fmov.s @(r0,r11),fr0 */
L8c61e09e: r[0]=20u;goto L8c61e0a0; /* mov #0x14,r0 */
L8c61e0a0: r[2]=r[3];goto L8c61e0a2; /* mov r3,r2 */
L8c61e0a2: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e0a4; /* fmov.s @(r0,r2),fr3 */
L8c61e0a4: r[1]=r[3];goto L8c61e0a6; /* mov r3,r1 */
L8c61e0a6: r[0]=4u;goto L8c61e0a8; /* mov #0x4,r0 */
L8c61e0a8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e0aa; /* fmul fr0,fr3 */
L8c61e0aa: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e0ac; /* fmov.s @(r0,r1),fr2 */
L8c61e0ac: fload(m,bus,&io_status,f,0u,r[11]);if(io_status)return io_status;goto L8c61e0ae; /* fmov.s @r11,fr0 */
L8c61e0ae: r[0]=8u;goto L8c61e0b0; /* mov #0x8,r0 */
L8c61e0b0: r[2]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61e0b2; /* mov.l 0x8c61e1a0,r2 */
L8c61e0b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e0b4; /* fmac fr0,fr2,fr3 */
L8c61e0b4: fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e0b6; /* fmov.s @(r0,r11),fr0 */
L8c61e0b6: r[0]=36u;goto L8c61e0b8; /* mov #0x24,r0 */
L8c61e0b8: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e0ba; /* fmov.s @(r0,r3),fr2 */
L8c61e0ba: r[0]=4u;goto L8c61e0bc; /* mov #0x4,r0 */
L8c61e0bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e0be; /* fmac fr0,fr2,fr3 */
L8c61e0be: fcopy(f,0u,15u);goto L8c61e0c0; /* fmov fr15,fr0 */
L8c61e0c0: fcopy(f,2u,15u);goto L8c61e0c2; /* fmov fr15,fr2 */
L8c61e0c2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e0c4; /* fmac fr0,fr3,fr2 */
L8c61e0c4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e0c6; /* fmov.s fr2,@-r8 */
L8c61e0c6: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e0c8; /* mov.l @r2,r3 */
L8c61e0c8: fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e0ca; /* fmov.s @(r0,r11),fr0 */
L8c61e0ca: r[0]=16u;goto L8c61e0cc; /* mov #0x10,r0 */
L8c61e0cc: r[2]=r[3];goto L8c61e0ce; /* mov r3,r2 */
L8c61e0ce: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e0d0; /* fmov.s @(r0,r2),fr3 */
L8c61e0d0: r[1]=r[3];goto L8c61e0d2; /* mov r3,r1 */
L8c61e0d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e0d4; /* fmul fr0,fr3 */
L8c61e0d4: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61e0d6; /* fmov.s @r1,fr2 */
L8c61e0d6: fload(m,bus,&io_status,f,0u,r[11]);if(io_status)return io_status;goto L8c61e0d8; /* fmov.s @r11,fr0 */
L8c61e0d8: r[0]=8u;goto L8c61e0da; /* mov #0x8,r0 */
L8c61e0da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e0dc; /* fmac fr0,fr2,fr3 */
L8c61e0dc: fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e0de; /* fmov.s @(r0,r11),fr0 */
L8c61e0de: r[0]=32u;goto L8c61e0e0; /* mov #0x20,r0 */
L8c61e0e0: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e0e2; /* fmov.s @(r0,r3),fr2 */
L8c61e0e2: r[0]=24u;goto L8c61e0e4; /* mov #0x18,r0 */
L8c61e0e4: r[3]=rd(m,bus,&io_status,0x8c61e1a8u,4);if(io_status)return io_status;goto L8c61e0e6; /* mov.l 0x8c61e1a8,r3 */
L8c61e0e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e0e8; /* fmac fr0,fr2,fr3 */
L8c61e0e8: fcopy(f,0u,15u);goto L8c61e0ea; /* fmov fr15,fr0 */
L8c61e0ea: fcopy(f,2u,15u);goto L8c61e0ec; /* fmov fr15,fr2 */
L8c61e0ec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e0ee; /* fmac fr0,fr3,fr2 */
L8c61e0ee: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e0f0; /* fmov.s fr2,@-r8 */
L8c61e0f0: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e0f2; /* fmov.s @(r0,r11),fr3 */
L8c61e0f2: r[0]=20u;goto L8c61e0f4; /* mov #0x14,r0 */
L8c61e0f4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e0f6; /* fmov.s fr3,@-r8 */
L8c61e0f6: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e0f8; /* fmov.s @(r0,r11),fr3 */
L8c61e0f8: r[0]=16u;goto L8c61e0fa; /* mov #0x10,r0 */
L8c61e0fa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e0fc; /* fmov.s fr3,@-r8 */
L8c61e0fc: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e0fe; /* fmov.s @(r0,r11),fr3 */
L8c61e0fe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e100; /* fmov.s fr3,@-r8 */
L8c61e100: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61e102; /* mov.l r3,@-r8 */
L8c61e102: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e104; /* pref @r8 */
L8c61e104: r[2]=rd(m,bus,&io_status,0x8c61e1acu,4);if(io_status)return io_status;goto L8c61e106; /* mov.l 0x8c61e1ac,r2 */
L8c61e106: r[8]+=32u;goto L8c61e108; /* add #0x20,r8 */
L8c61e108: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e10a; /* mov.l @r2,r3 */
L8c61e10a: r[3]+=4u;goto L8c61e10c; /* add #0x4,r3 */
L8c61e10c: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61e10e; /* mov.l r3,@r2 */
L8c61e10e: r[9]+=1u;goto L8c61e110; /* add #0x1,r9 */
L8c61e110: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61e112; /* mov.l @r15,r2 */
L8c61e112: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61e114; /* mov.w @(0x2,r2),r0 */
L8c61e114: r[0]=r[0]&65535u;goto L8c61e116; /* extu.w r0,r0 */
L8c61e116: *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61e118; /* cmp/ge r0,r9 */
L8c61e118: if((*sr&1u))goto L8c61e11e;goto L8c61e11a; /* bt 0x8c61e11e */
L8c61e11a: goto L8c61de7c; /* bra 0x8c61de7c */
L8c61e11e: goto L8c61e4a8; /* bra 0x8c61e4a8 */
L8c61e122: r[11]=2u;goto L8c61e124; /* mov #0x2,r11 */
L8c61e124: r[12]=r[4];goto L8c61e41c; /* bra 0x8c61e41c */
L8c61e128: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e12a; /* mov.w @r10+,r14 */
L8c61e12a: r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61e12c; /* mov.l @r13,r3 */
L8c61e12c: r[14]=r[14]&65535u;goto L8c61e12e; /* extu.w r14,r14 */
L8c61e12e: r[3]+=r[14];goto L8c61e130; /* add r14,r3 */
L8c61e130: wr(m,bus,&io_status,r[13],r[3],4);if(io_status)return io_status;goto L8c61e132; /* mov.l r3,@r13 */
L8c61e132: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61e134; /* mov.w @(0x2,r10),r0 */
L8c61e134: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61e136; /* mov.w @r10,r4 */
L8c61e136: r[3]=rd(m,bus,&io_status,0x8c61e18cu,4);if(io_status)return io_status;goto L8c61e138; /* mov.l 0x8c61e18c,r3 */
L8c61e138: r[6]=r[0]&65535u;goto L8c61e13a; /* extu.w r0,r6 */
L8c61e13a: r[0]=sx(m,bus,&io_status,r[10]+0x4u,2);if(io_status)return io_status;goto L8c61e13c; /* mov.w @(0x4,r10),r0 */
L8c61e13c: r[6]<<=2;goto L8c61e13e; /* shll2 r6 */
L8c61e13e: r[4]=r[4]&65535u;goto L8c61e140; /* extu.w r4,r4 */
L8c61e140: r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e142; /* mov.l @r3,r7 */
L8c61e142: r[6]<<=2;goto L8c61e144; /* shll2 r6 */
L8c61e144: r[4]<<=2;goto L8c61e146; /* shll2 r4 */
L8c61e146: r[5]=r[0]&65535u;goto L8c61e148; /* extu.w r0,r5 */
L8c61e148: r[4]<<=2;goto L8c61e14a; /* shll2 r4 */
L8c61e14a: r[0]=16u;goto L8c61e14c; /* mov #0x10,r0 */
L8c61e14c: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61e14e; /* shll r4 */
L8c61e14e: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61e150; /* shll r6 */
L8c61e150: r[6]+=r[7];goto L8c61e152; /* add r7,r6 */
L8c61e152: fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c61e154; /* fmov.s @(r0,r6),fr5 */
L8c61e154: r[4]+=r[7];goto L8c61e156; /* add r7,r4 */
L8c61e156: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e158; /* fmov.s @(r0,r4),fr3 */
L8c61e158: r[5]<<=2;goto L8c61e15a; /* shll2 r5 */
L8c61e15a: fcopy(f,2u,5u);goto L8c61e15c; /* fmov fr5,fr2 */
L8c61e15c: r[0]=20u;goto L8c61e15e; /* mov #0x14,r0 */
L8c61e15e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61e160; /* fsub fr3,fr2 */
L8c61e160: r[5]<<=2;goto L8c61e162; /* shll2 r5 */
L8c61e162: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c61e164; /* fmov.s @(r0,r6),fr4 */
L8c61e164: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61e166; /* shll r5 */
L8c61e166: r[5]+=r[7];goto L8c61e168; /* add r7,r5 */
L8c61e168: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61e16a; /* fmov.s @(r0,r5),fr3 */
L8c61e16a: fcopy(f,1u,4u);goto L8c61e16c; /* fmov fr4,fr1 */
L8c61e16c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e16e; /* fsub fr4,fr3 */
L8c61e16e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e170; /* fmul fr3,fr2 */
L8c61e170: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e172; /* fmov.s @(r0,r4),fr3 */
L8c61e172: r[0]=16u;goto L8c61e174; /* mov #0x10,r0 */
L8c61e174: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61e176; /* fsub fr3,fr1 */
L8c61e176: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61e178; /* fmov.s @(r0,r5),fr3 */
L8c61e178: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e17a; /* fsub fr5,fr3 */
L8c61e17a: fcopy(f,4u,2u);goto L8c61e17c; /* fmov fr2,fr4 */
L8c61e17c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e17e; /* fmul fr3,fr1 */
L8c61e17e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61e180; /* fldi0 fr3 */
L8c61e180: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61e182; /* fsub fr1,fr4 */
L8c61e182: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61e184; /* fcmp/gt fr3,fr4 */
L8c61e184: if(!(*sr&1u))goto L8c61e1b0;goto L8c61e186; /* bf 0x8c61e1b0 */
L8c61e186: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61e188; /* shll r14 */
L8c61e188: r[10]+=r[14];goto L8c61e41a; /* bra 0x8c61e41a */
L8c61e1b0: r[0]=4u;goto L8c61e1b2; /* mov #0x4,r0 */
L8c61e1b2: r[3]=rd(m,bus,&io_status,0x8c61e4c8u,4);if(io_status)return io_status;goto L8c61e1b4; /* mov.l 0x8c61e4c8,r3 */
L8c61e1b4: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e1b6; /* mov.l @r3,r2 */
L8c61e1b6: r[2]+=r[14];goto L8c61e1b8; /* add r14,r2 */
L8c61e1b8: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61e1ba; /* mov.l r2,@r3 */
L8c61e1ba: fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c61e1bc; /* fmov.s @r6,fr3 */
L8c61e1bc: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c61e1be; /* fmov.s @r4,fr2 */
L8c61e1be: r[3]=rd(m,bus,&io_status,0x8c61e4ccu,4);if(io_status)return io_status;goto L8c61e1c0; /* mov.l 0x8c61e4cc,r3 */
L8c61e1c0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e1c2; /* fadd fr3,fr2 */
L8c61e1c2: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61e1c4; /* fmov.s @r5,fr3 */
L8c61e1c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e1c6; /* fadd fr3,fr2 */
L8c61e1c6: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e1c8; /* fmov fr2,@(r0,r15) */
L8c61e1c8: r[0]=4u;goto L8c61e1ca; /* mov #0x4,r0 */
L8c61e1ca: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e1cc; /* fmov.s @(r0,r4),fr2 */
L8c61e1cc: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61e1ce; /* fmov.s @(r0,r6),fr3 */
L8c61e1ce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e1d0; /* fadd fr3,fr2 */
L8c61e1d0: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61e1d2; /* fmov.s @(r0,r5),fr3 */
L8c61e1d2: r[0]=8u;goto L8c61e1d4; /* mov #0x8,r0 */
L8c61e1d4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e1d6; /* fadd fr3,fr2 */
L8c61e1d6: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e1d8; /* fmov fr2,@(r0,r15) */
L8c61e1d8: r[0]=8u;goto L8c61e1da; /* mov #0x8,r0 */
L8c61e1da: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61e1dc; /* fmov.s @(r0,r6),fr3 */
L8c61e1dc: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e1de; /* fmov.s @(r0,r4),fr2 */
L8c61e1de: r[4]=r[15];goto L8c61e1e0; /* mov r15,r4 */
L8c61e1e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e1e2; /* fadd fr3,fr2 */
L8c61e1e2: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61e1e4; /* fmov.s @(r0,r5),fr3 */
L8c61e1e4: r[0]=12u;goto L8c61e1e6; /* mov #0xc,r0 */
L8c61e1e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e1e8; /* fadd fr3,fr2 */
L8c61e1e8: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e1ea; /* fmov fr2,@(r0,r15) */
L8c61e1ea: target=r[3];*pr=0x8c61e1eeu;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61e1ee; /* jsr @r3 */
L8c61e1ee: r[3]=rd(m,bus,&io_status,0x8c61e4d0u,4);if(io_status)return io_status;goto L8c61e1f0; /* mov.l 0x8c61e4d0,r3 */
L8c61e1f0: r[0]=4u;goto L8c61e1f2; /* mov #0x4,r0 */
L8c61e1f2: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e1f4; /* fmov.s @(r0,r15),fr3 */
L8c61e1f4: r[0]=8u;goto L8c61e1f6; /* mov #0x8,r0 */
L8c61e1f6: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61e1f8; /* fmov.s @r3,fr2 */
L8c61e1f8: r[3]+=8u;goto L8c61e1fa; /* add #0x8,r3 */
L8c61e1fa: r[2]=rd(m,bus,&io_status,0x8c61e4d4u,4);if(io_status)return io_status;goto L8c61e1fc; /* mov.l 0x8c61e4d4,r2 */
L8c61e1fc: r[8]+=24u;goto L8c61e1fe; /* add #0x18,r8 */
L8c61e1fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e200; /* fmul fr3,fr2 */
L8c61e200: r[1]=rd(m,bus,&io_status,0x8c61e4d8u,4);if(io_status)return io_status;goto L8c61e202; /* mov.l 0x8c61e4d8,r1 */
L8c61e202: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61e204; /* fmov.s @r2,fr3 */
L8c61e204: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61e206; /* fmov.s @r1,fr1 */
L8c61e206: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e208; /* mov.w @r10+,r4 */
L8c61e208: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e20a; /* fsub fr2,fr3 */
L8c61e20a: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e20c; /* fmov.s @(r0,r15),fr2 */
L8c61e20c: r[0]=12u;goto L8c61e20e; /* mov #0xc,r0 */
L8c61e20e: r[1]=rd(m,bus,&io_status,0x8c61e4e0u,4);if(io_status)return io_status;goto L8c61e210; /* mov.l 0x8c61e4e0,r1 */
L8c61e210: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e212; /* fmul fr2,fr1 */
L8c61e212: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e214; /* fmov.s @(r0,r15),fr2 */
L8c61e214: r[4]=r[4]&65535u;goto L8c61e216; /* extu.w r4,r4 */
L8c61e216: r[0]=rd(m,bus,&io_status,0x8c61e4dcu,4);if(io_status)return io_status;goto L8c61e218; /* mov.l 0x8c61e4dc,r0 */
L8c61e218: r[4]<<=2;goto L8c61e21a; /* shll2 r4 */
L8c61e21a: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61e21c; /* mov.l @r0,r2 */
L8c61e21c: r[4]<<=2;goto L8c61e21e; /* shll2 r4 */
L8c61e21e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e220; /* fsub fr1,fr3 */
L8c61e220: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61e222; /* fmov.s @r3,fr1 */
L8c61e222: r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61e224; /* mov.l @r1,r3 */
L8c61e224: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61e226; /* shll r4 */
L8c61e226: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e228; /* fmul fr2,fr1 */
L8c61e228: r[4]+=r[2];goto L8c61e22a; /* add r2,r4 */
L8c61e22a: fcopy(f,4u,3u);goto L8c61e22c; /* fmov fr3,fr4 */
L8c61e22c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61e22e; /* fsub fr1,fr4 */
L8c61e22e: r[0]=4u;goto L8c61e230; /* mov #0x4,r0 */
L8c61e230: r[1]=r[3];goto L8c61e232; /* mov r3,r1 */
L8c61e232: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e234; /* fmov.s @(r0,r4),fr0 */
L8c61e234: r[0]=20u;goto L8c61e236; /* mov #0x14,r0 */
L8c61e236: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e238; /* fmov.s @(r0,r1),fr3 */
L8c61e238: r[0]=4u;goto L8c61e23a; /* mov #0x4,r0 */
L8c61e23a: r[2]=r[3];goto L8c61e23c; /* mov r3,r2 */
L8c61e23c: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e23e; /* fmov.s @(r0,r2),fr2 */
L8c61e23e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e240; /* fmul fr0,fr3 */
L8c61e240: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e242; /* fmov.s @r4,fr0 */
L8c61e242: r[0]=8u;goto L8c61e244; /* mov #0x8,r0 */
L8c61e244: r[2]=rd(m,bus,&io_status,0x8c61e4e0u,4);if(io_status)return io_status;goto L8c61e246; /* mov.l 0x8c61e4e0,r2 */
L8c61e246: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e248; /* fmac fr0,fr2,fr3 */
L8c61e248: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e24a; /* fmov.s @(r0,r4),fr0 */
L8c61e24a: r[0]=36u;goto L8c61e24c; /* mov #0x24,r0 */
L8c61e24c: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e24e; /* fmov.s @(r0,r3),fr2 */
L8c61e24e: r[0]=4u;goto L8c61e250; /* mov #0x4,r0 */
L8c61e250: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e252; /* fmac fr0,fr2,fr3 */
L8c61e252: fcopy(f,0u,15u);goto L8c61e254; /* fmov fr15,fr0 */
L8c61e254: fcopy(f,2u,15u);goto L8c61e256; /* fmov fr15,fr2 */
L8c61e256: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e258; /* fmac fr0,fr3,fr2 */
L8c61e258: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e25a; /* fmov.s fr2,@-r8 */
L8c61e25a: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e25c; /* mov.l @r2,r3 */
L8c61e25c: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e25e; /* fmov.s @(r0,r4),fr0 */
L8c61e25e: r[0]=16u;goto L8c61e260; /* mov #0x10,r0 */
L8c61e260: r[2]=r[3];goto L8c61e262; /* mov r3,r2 */
L8c61e262: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e264; /* fmov.s @(r0,r2),fr3 */
L8c61e264: r[1]=r[3];goto L8c61e266; /* mov r3,r1 */
L8c61e266: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61e268; /* fmov.s @r1,fr2 */
L8c61e268: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e26a; /* fmul fr0,fr3 */
L8c61e26a: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e26c; /* fmov.s @r4,fr0 */
L8c61e26c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e26e; /* fmac fr0,fr2,fr3 */
L8c61e26e: r[0]=8u;goto L8c61e270; /* mov #0x8,r0 */
L8c61e270: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e272; /* fmov.s @(r0,r4),fr0 */
L8c61e272: r[0]=32u;goto L8c61e274; /* mov #0x20,r0 */
L8c61e274: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e276; /* fmov.s @(r0,r3),fr2 */
L8c61e276: r[0]=24u;goto L8c61e278; /* mov #0x18,r0 */
L8c61e278: r[3]=rd(m,bus,&io_status,0x8c61e4e4u,4);if(io_status)return io_status;goto L8c61e27a; /* mov.l 0x8c61e4e4,r3 */
L8c61e27a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e27c; /* fmac fr0,fr2,fr3 */
L8c61e27c: fcopy(f,0u,15u);goto L8c61e27e; /* fmov fr15,fr0 */
L8c61e27e: fcopy(f,2u,15u);goto L8c61e280; /* fmov fr15,fr2 */
L8c61e280: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e282; /* fmac fr0,fr3,fr2 */
L8c61e282: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e284; /* fmov.s fr2,@-r8 */
L8c61e284: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e286; /* fmov.s @(r0,r4),fr3 */
L8c61e286: r[0]=20u;goto L8c61e288; /* mov #0x14,r0 */
L8c61e288: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e28a; /* fmov.s fr3,@-r8 */
L8c61e28a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e28c; /* fmov.s @(r0,r4),fr3 */
L8c61e28c: r[0]=16u;goto L8c61e28e; /* mov #0x10,r0 */
L8c61e28e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e290; /* fmov.s fr3,@-r8 */
L8c61e290: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e292; /* fmov.s @(r0,r4),fr3 */
L8c61e292: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e294; /* fmov.s fr3,@-r8 */
L8c61e294: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61e296; /* mov.l r3,@-r8 */
L8c61e296: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e298; /* pref @r8 */
L8c61e298: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e29a; /* mov.w @r10+,r4 */
L8c61e29a: r[8]+=56u;goto L8c61e29c; /* add #0x38,r8 */
L8c61e29c: r[2]=rd(m,bus,&io_status,0x8c61e4dcu,4);if(io_status)return io_status;goto L8c61e29e; /* mov.l 0x8c61e4dc,r2 */
L8c61e29e: r[4]=r[4]&65535u;goto L8c61e2a0; /* extu.w r4,r4 */
L8c61e2a0: r[0]=rd(m,bus,&io_status,0x8c61e4e0u,4);if(io_status)return io_status;goto L8c61e2a2; /* mov.l 0x8c61e4e0,r0 */
L8c61e2a2: r[4]<<=2;goto L8c61e2a4; /* shll2 r4 */
L8c61e2a4: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e2a6; /* mov.l @r2,r3 */
L8c61e2a6: r[4]<<=2;goto L8c61e2a8; /* shll2 r4 */
L8c61e2a8: r[1]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61e2aa; /* mov.l @r0,r1 */
L8c61e2aa: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61e2ac; /* shll r4 */
L8c61e2ac: r[0]=4u;goto L8c61e2ae; /* mov #0x4,r0 */
L8c61e2ae: r[2]=r[1];goto L8c61e2b0; /* mov r1,r2 */
L8c61e2b0: r[4]+=r[3];goto L8c61e2b2; /* add r3,r4 */
L8c61e2b2: r[3]=r[1];goto L8c61e2b4; /* mov r1,r3 */
L8c61e2b4: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e2b6; /* fmov.s @(r0,r4),fr0 */
L8c61e2b6: r[0]=20u;goto L8c61e2b8; /* mov #0x14,r0 */
L8c61e2b8: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e2ba; /* fmov.s @(r0,r2),fr3 */
L8c61e2ba: r[0]=4u;goto L8c61e2bc; /* mov #0x4,r0 */
L8c61e2bc: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e2be; /* fmov.s @(r0,r3),fr2 */
L8c61e2be: r[0]=8u;goto L8c61e2c0; /* mov #0x8,r0 */
L8c61e2c0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e2c2; /* fmul fr0,fr3 */
L8c61e2c2: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e2c4; /* fmov.s @r4,fr0 */
L8c61e2c4: r[3]=rd(m,bus,&io_status,0x8c61e4e0u,4);if(io_status)return io_status;goto L8c61e2c6; /* mov.l 0x8c61e4e0,r3 */
L8c61e2c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e2c8; /* fmac fr0,fr2,fr3 */
L8c61e2c8: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e2ca; /* fmov.s @(r0,r4),fr0 */
L8c61e2ca: r[0]=36u;goto L8c61e2cc; /* mov #0x24,r0 */
L8c61e2cc: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e2ce; /* fmov.s @(r0,r1),fr2 */
L8c61e2ce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e2d0; /* fmac fr0,fr2,fr3 */
L8c61e2d0: fcopy(f,0u,15u);goto L8c61e2d2; /* fmov fr15,fr0 */
L8c61e2d2: fcopy(f,2u,15u);goto L8c61e2d4; /* fmov fr15,fr2 */
L8c61e2d4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e2d6; /* fmac fr0,fr3,fr2 */
L8c61e2d6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e2d8; /* fmov.s fr2,@-r8 */
L8c61e2d8: r[0]=4u;goto L8c61e2da; /* mov #0x4,r0 */
L8c61e2da: r[9]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e2dc; /* mov.l @r3,r9 */
L8c61e2dc: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e2de; /* fmov.s @(r0,r4),fr0 */
L8c61e2de: r[0]=16u;goto L8c61e2e0; /* mov #0x10,r0 */
L8c61e2e0: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61e2e2; /* fmov.s @(r0,r9),fr3 */
L8c61e2e2: r[0]=8u;goto L8c61e2e4; /* mov #0x8,r0 */
L8c61e2e4: fload(m,bus,&io_status,f,2u,r[9]);if(io_status)return io_status;goto L8c61e2e6; /* fmov.s @r9,fr2 */
L8c61e2e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e2e8; /* fmul fr0,fr3 */
L8c61e2e8: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e2ea; /* fmov.s @r4,fr0 */
L8c61e2ea: r[2]=rd(m,bus,&io_status,0x8c61e4e4u,4);if(io_status)return io_status;goto L8c61e2ec; /* mov.l 0x8c61e4e4,r2 */
L8c61e2ec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e2ee; /* fmac fr0,fr2,fr3 */
L8c61e2ee: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e2f0; /* fmov.s @(r0,r4),fr0 */
L8c61e2f0: r[0]=32u;goto L8c61e2f2; /* mov #0x20,r0 */
L8c61e2f2: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61e2f4; /* fmov.s @(r0,r9),fr2 */
L8c61e2f4: r[0]=24u;goto L8c61e2f6; /* mov #0x18,r0 */
L8c61e2f6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e2f8; /* fmac fr0,fr2,fr3 */
L8c61e2f8: fcopy(f,0u,15u);goto L8c61e2fa; /* fmov fr15,fr0 */
L8c61e2fa: fcopy(f,2u,15u);goto L8c61e2fc; /* fmov fr15,fr2 */
L8c61e2fc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e2fe; /* fmac fr0,fr3,fr2 */
L8c61e2fe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e300; /* fmov.s fr2,@-r8 */
L8c61e300: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e302; /* fmov.s @(r0,r4),fr3 */
L8c61e302: r[0]=20u;goto L8c61e304; /* mov #0x14,r0 */
L8c61e304: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e306; /* fmov.s fr3,@-r8 */
L8c61e306: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e308; /* fmov.s @(r0,r4),fr3 */
L8c61e308: r[0]=16u;goto L8c61e30a; /* mov #0x10,r0 */
L8c61e30a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e30c; /* fmov.s fr3,@-r8 */
L8c61e30c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e30e; /* fmov.s @(r0,r4),fr3 */
L8c61e30e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e310; /* fmov.s fr3,@-r8 */
L8c61e310: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61e312; /* mov.l r2,@-r8 */
L8c61e312: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e314; /* pref @r8 */
L8c61e314: r[6]=r[14];goto L8c61e316; /* mov r14,r6 */
L8c61e316: r[6]+=4294967295u;goto L8c61e318; /* add #-0x1,r6 */
L8c61e318: r[5]=r[11];goto L8c61e31a; /* mov r11,r5 */
L8c61e31a: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61e31c; /* cmp/ge r6,r5 */
L8c61e31c: take=(*sr&1u);r[8]+=32u;if(take)goto L8c61e3a0;goto L8c61e320; /* bt/s 0x8c61e3a0 */
L8c61e320: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e322; /* mov.w @r10+,r4 */
L8c61e322: r[0]=4u;goto L8c61e324; /* mov #0x4,r0 */
L8c61e324: r[2]=rd(m,bus,&io_status,0x8c61e4dcu,4);if(io_status)return io_status;goto L8c61e326; /* mov.l 0x8c61e4dc,r2 */
L8c61e326: r[8]+=28u;goto L8c61e328; /* add #0x1c,r8 */
L8c61e328: r[4]=r[4]&65535u;goto L8c61e32a; /* extu.w r4,r4 */
L8c61e32a: r[4]<<=2;goto L8c61e32c; /* shll2 r4 */
L8c61e32c: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e32e; /* mov.l @r2,r3 */
L8c61e32e: r[4]<<=2;goto L8c61e330; /* shll2 r4 */
L8c61e330: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61e332; /* fmov.s fr4,@-r8 */
L8c61e332: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61e334; /* shll r4 */
L8c61e334: r[4]+=r[3];goto L8c61e336; /* add r3,r4 */
L8c61e336: r[3]=rd(m,bus,&io_status,0x8c61e4e0u,4);if(io_status)return io_status;goto L8c61e338; /* mov.l 0x8c61e4e0,r3 */
L8c61e338: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e33a; /* fmov.s @(r0,r4),fr0 */
L8c61e33a: r[0]=20u;goto L8c61e33c; /* mov #0x14,r0 */
L8c61e33c: r[14]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e33e; /* mov.l @r3,r14 */
L8c61e33e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e340; /* fmov.s @(r0,r14),fr3 */
L8c61e340: r[0]=4u;goto L8c61e342; /* mov #0x4,r0 */
L8c61e342: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e344; /* fmov.s @(r0,r14),fr2 */
L8c61e344: r[0]=8u;goto L8c61e346; /* mov #0x8,r0 */
L8c61e346: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e348; /* fmul fr0,fr3 */
L8c61e348: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e34a; /* fmov.s @r4,fr0 */
L8c61e34a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e34c; /* fmac fr0,fr2,fr3 */
L8c61e34c: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e34e; /* fmov.s @(r0,r4),fr0 */
L8c61e34e: r[0]=36u;goto L8c61e350; /* mov #0x24,r0 */
L8c61e350: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e352; /* fmov.s @(r0,r14),fr2 */
L8c61e352: r[0]=4u;goto L8c61e354; /* mov #0x4,r0 */
L8c61e354: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e356; /* fmac fr0,fr2,fr3 */
L8c61e356: fcopy(f,2u,15u);goto L8c61e358; /* fmov fr15,fr2 */
L8c61e358: fcopy(f,0u,15u);goto L8c61e35a; /* fmov fr15,fr0 */
L8c61e35a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e35c; /* fmac fr0,fr3,fr2 */
L8c61e35c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e35e; /* fmov.s fr2,@-r8 */
L8c61e35e: r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e360; /* mov.l @r3,r7 */
L8c61e360: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e362; /* fmov.s @(r0,r4),fr0 */
L8c61e362: r[0]=16u;goto L8c61e364; /* mov #0x10,r0 */
L8c61e364: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61e366; /* fmov.s @(r0,r7),fr3 */
L8c61e366: r[0]=8u;goto L8c61e368; /* mov #0x8,r0 */
L8c61e368: fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61e36a; /* fmov.s @r7,fr2 */
L8c61e36a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e36c; /* fmul fr0,fr3 */
L8c61e36c: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e36e; /* fmov.s @r4,fr0 */
L8c61e36e: r[1]=rd(m,bus,&io_status,0x8c61e4e4u,4);if(io_status)return io_status;goto L8c61e370; /* mov.l 0x8c61e4e4,r1 */
L8c61e370: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e372; /* fmac fr0,fr2,fr3 */
L8c61e372: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e374; /* fmov.s @(r0,r4),fr0 */
L8c61e374: r[0]=32u;goto L8c61e376; /* mov #0x20,r0 */
L8c61e376: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61e378; /* fmov.s @(r0,r7),fr2 */
L8c61e378: r[0]=24u;goto L8c61e37a; /* mov #0x18,r0 */
L8c61e37a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e37c; /* fmac fr0,fr2,fr3 */
L8c61e37c: fcopy(f,0u,15u);goto L8c61e37e; /* fmov fr15,fr0 */
L8c61e37e: fcopy(f,2u,15u);goto L8c61e380; /* fmov fr15,fr2 */
L8c61e380: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e382; /* fmac fr0,fr3,fr2 */
L8c61e382: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e384; /* fmov.s fr2,@-r8 */
L8c61e384: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e386; /* fmov.s @(r0,r4),fr3 */
L8c61e386: r[0]=20u;goto L8c61e388; /* mov #0x14,r0 */
L8c61e388: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e38a; /* fmov.s fr3,@-r8 */
L8c61e38a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e38c; /* fmov.s @(r0,r4),fr3 */
L8c61e38c: r[0]=16u;goto L8c61e38e; /* mov #0x10,r0 */
L8c61e38e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e390; /* fmov.s fr3,@-r8 */
L8c61e390: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e392; /* fmov.s @(r0,r4),fr3 */
L8c61e392: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e394; /* fmov.s fr3,@-r8 */
L8c61e394: r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c61e396; /* mov.l r1,@-r8 */
L8c61e396: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e398; /* pref @r8 */
L8c61e398: r[5]+=1u;goto L8c61e39a; /* add #0x1,r5 */
L8c61e39a: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61e39c; /* cmp/ge r6,r5 */
L8c61e39c: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61e320;goto L8c61e3a0; /* bf/s 0x8c61e320 */
L8c61e3a0: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e3a2; /* mov.w @r10+,r4 */
L8c61e3a2: r[0]=4u;goto L8c61e3a4; /* mov #0x4,r0 */
L8c61e3a4: r[2]=rd(m,bus,&io_status,0x8c61e4dcu,4);if(io_status)return io_status;goto L8c61e3a6; /* mov.l 0x8c61e4dc,r2 */
L8c61e3a6: r[8]+=28u;goto L8c61e3a8; /* add #0x1c,r8 */
L8c61e3a8: r[4]=r[4]&65535u;goto L8c61e3aa; /* extu.w r4,r4 */
L8c61e3aa: r[4]<<=2;goto L8c61e3ac; /* shll2 r4 */
L8c61e3ac: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e3ae; /* mov.l @r2,r3 */
L8c61e3ae: r[4]<<=2;goto L8c61e3b0; /* shll2 r4 */
L8c61e3b0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61e3b2; /* fmov.s fr4,@-r8 */
L8c61e3b2: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61e3b4; /* shll r4 */
L8c61e3b4: r[4]+=r[3];goto L8c61e3b6; /* add r3,r4 */
L8c61e3b6: r[3]=rd(m,bus,&io_status,0x8c61e4e0u,4);if(io_status)return io_status;goto L8c61e3b8; /* mov.l 0x8c61e4e0,r3 */
L8c61e3b8: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e3ba; /* fmov.s @(r0,r4),fr0 */
L8c61e3ba: r[0]=20u;goto L8c61e3bc; /* mov #0x14,r0 */
L8c61e3bc: r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e3be; /* mov.l @r3,r6 */
L8c61e3be: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61e3c0; /* fmov.s @(r0,r6),fr3 */
L8c61e3c0: r[0]=4u;goto L8c61e3c2; /* mov #0x4,r0 */
L8c61e3c2: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61e3c4; /* fmov.s @(r0,r6),fr2 */
L8c61e3c4: r[0]=8u;goto L8c61e3c6; /* mov #0x8,r0 */
L8c61e3c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e3c8; /* fmul fr0,fr3 */
L8c61e3c8: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e3ca; /* fmov.s @r4,fr0 */
L8c61e3ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e3cc; /* fmac fr0,fr2,fr3 */
L8c61e3cc: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e3ce; /* fmov.s @(r0,r4),fr0 */
L8c61e3ce: r[0]=36u;goto L8c61e3d0; /* mov #0x24,r0 */
L8c61e3d0: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61e3d2; /* fmov.s @(r0,r6),fr2 */
L8c61e3d2: r[0]=4u;goto L8c61e3d4; /* mov #0x4,r0 */
L8c61e3d4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e3d6; /* fmac fr0,fr2,fr3 */
L8c61e3d6: fcopy(f,2u,15u);goto L8c61e3d8; /* fmov fr15,fr2 */
L8c61e3d8: fcopy(f,0u,15u);goto L8c61e3da; /* fmov fr15,fr0 */
L8c61e3da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e3dc; /* fmac fr0,fr3,fr2 */
L8c61e3dc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e3de; /* fmov.s fr2,@-r8 */
L8c61e3de: r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e3e0; /* mov.l @r3,r5 */
L8c61e3e0: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e3e2; /* fmov.s @(r0,r4),fr0 */
L8c61e3e2: r[0]=16u;goto L8c61e3e4; /* mov #0x10,r0 */
L8c61e3e4: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61e3e6; /* fmov.s @(r0,r5),fr3 */
L8c61e3e6: r[0]=8u;goto L8c61e3e8; /* mov #0x8,r0 */
L8c61e3e8: fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c61e3ea; /* fmov.s @r5,fr2 */
L8c61e3ea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e3ec; /* fmul fr0,fr3 */
L8c61e3ec: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e3ee; /* fmov.s @r4,fr0 */
L8c61e3ee: r[1]=rd(m,bus,&io_status,0x8c61e4e8u,4);if(io_status)return io_status;goto L8c61e3f0; /* mov.l 0x8c61e4e8,r1 */
L8c61e3f0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e3f2; /* fmac fr0,fr2,fr3 */
L8c61e3f2: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e3f4; /* fmov.s @(r0,r4),fr0 */
L8c61e3f4: r[0]=32u;goto L8c61e3f6; /* mov #0x20,r0 */
L8c61e3f6: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61e3f8; /* fmov.s @(r0,r5),fr2 */
L8c61e3f8: r[0]=24u;goto L8c61e3fa; /* mov #0x18,r0 */
L8c61e3fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e3fc; /* fmac fr0,fr2,fr3 */
L8c61e3fc: fcopy(f,0u,15u);goto L8c61e3fe; /* fmov fr15,fr0 */
L8c61e3fe: fcopy(f,2u,15u);goto L8c61e400; /* fmov fr15,fr2 */
L8c61e400: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e402; /* fmac fr0,fr3,fr2 */
L8c61e402: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e404; /* fmov.s fr2,@-r8 */
L8c61e404: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e406; /* fmov.s @(r0,r4),fr3 */
L8c61e406: r[0]=20u;goto L8c61e408; /* mov #0x14,r0 */
L8c61e408: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e40a; /* fmov.s fr3,@-r8 */
L8c61e40a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e40c; /* fmov.s @(r0,r4),fr3 */
L8c61e40c: r[0]=16u;goto L8c61e40e; /* mov #0x10,r0 */
L8c61e40e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e410; /* fmov.s fr3,@-r8 */
L8c61e410: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e412; /* fmov.s @(r0,r4),fr3 */
L8c61e412: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e414; /* fmov.s fr3,@-r8 */
L8c61e414: r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c61e416; /* mov.l r1,@-r8 */
L8c61e416: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e418; /* pref @r8 */
L8c61e418: r[8]+=32u;goto L8c61e41a; /* add #0x20,r8 */
L8c61e41a: r[12]+=1u;goto L8c61e41c; /* add #0x1,r12 */
L8c61e41c: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61e41e; /* mov.l @r15,r3 */
L8c61e41e: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61e420; /* mov.w @(0x2,r3),r0 */
L8c61e420: r[0]=r[0]&65535u;goto L8c61e422; /* extu.w r0,r0 */
L8c61e422: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[0]);goto L8c61e424; /* cmp/ge r0,r12 */
L8c61e424: if((*sr&1u))goto L8c61e42a;goto L8c61e426; /* bt 0x8c61e42a */
L8c61e426: goto L8c61e128; /* bra 0x8c61e128 */
L8c61e42a: goto L8c61e4a8; /* bra 0x8c61e4a8 */
L8c61e42e: r[2]=rd(m,bus,&io_status,0x8c61e4ecu,4);if(io_status)return io_status;goto L8c61e430; /* mov.l 0x8c61e4ec,r2 */
L8c61e430: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61e432; /* mov.l @r15,r1 */
L8c61e432: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61e434; /* mov.l r1,@r2 */
L8c61e434: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61e436; /* frchg */
L8c61e436: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61e438; /* fmov.s fr0,@-r15 */
L8c61e438: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61e43a; /* fmov.s fr1,@-r15 */
L8c61e43a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61e43c; /* fmov.s fr2,@-r15 */
L8c61e43c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61e43e; /* fmov.s fr3,@-r15 */
L8c61e43e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61e440; /* fmov.s fr4,@-r15 */
L8c61e440: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61e442; /* fmov.s fr5,@-r15 */
L8c61e442: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61e444; /* fmov.s fr6,@-r15 */
L8c61e444: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61e446; /* fmov.s fr7,@-r15 */
L8c61e446: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61e448; /* fmov.s fr8,@-r15 */
L8c61e448: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61e44a; /* fmov.s fr9,@-r15 */
L8c61e44a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c61e44c; /* fmov.s fr10,@-r15 */
L8c61e44c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c61e44e; /* fmov.s fr11,@-r15 */
L8c61e44e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c61e450; /* fmov.s fr12,@-r15 */
L8c61e450: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61e452; /* fmov.s fr13,@-r15 */
L8c61e452: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61e454; /* fmov.s fr14,@-r15 */
L8c61e454: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61e456; /* fmov.s fr15,@-r15 */
L8c61e456: r[0]=0x8c61e4bcu;goto L8c61e458; /* mova 0x8c61e4bc,r0 */
L8c61e458: fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c61e45a; /* fmov.s @r0,fr12 */
L8c61e45a: fcopy(f,13u,12u);goto L8c61e45c; /* fmov fr12,fr13 */
L8c61e45c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[12],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61e45e; /* fmul fr12,fr0 */
L8c61e45e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61e460; /* fmul fr12,fr4 */
L8c61e460: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[12],&f->fpscr,&fr[8]);if(status)return status;}goto L8c61e462; /* fmul fr12,fr8 */
L8c61e462: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[12],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e464; /* fmul fr12,fr1 */
L8c61e464: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[12],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61e466; /* fmul fr12,fr5 */
L8c61e466: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[12],&f->fpscr,&fr[9]);if(status)return status;}goto L8c61e468; /* fmul fr12,fr9 */
L8c61e468: r[0]=rd(m,bus,&io_status,0x8c61e4c0u,4);if(io_status)return io_status;goto L8c61e46a; /* mov.l 0x8c61e4c0,r0 */
L8c61e46a: fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e46c; /* fmov.s @r0+,fr2 */
L8c61e46c: fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e46e; /* fmov.s @r0+,fr6 */
L8c61e46e: fload(m,bus,&io_status,f,10u,r[0]);if(io_status)return io_status;goto L8c61e470; /* fmov.s @r0,fr10 */
L8c61e470: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c61e472; /* fneg fr2 */
L8c61e472: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]^=0x80000000u;goto L8c61e474; /* fneg fr6 */
L8c61e474: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]^=0x80000000u;goto L8c61e476; /* fneg fr10 */
L8c61e476: r[0]=rd(m,bus,&io_status,0x8c61e4c4u,4);if(io_status)return io_status;goto L8c61e478; /* mov.l 0x8c61e4c4,r0 */
L8c61e478: fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c61e47a; /* fmov.s @r0,fr14 */
L8c61e47a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61e47c; /* frchg */
L8c61e47c: r[2]=rd(m,bus,&io_status,0x8c61e4f0u,4);if(io_status)return io_status;goto L8c61e47e; /* mov.l 0x8c61e4f0,r2 */
L8c61e47e: r[3]=rd(m,bus,&io_status,0x8c61e4ecu,4);if(io_status)return io_status;goto L8c61e480; /* mov.l 0x8c61e4ec,r3 */
L8c61e480: target=r[2];*pr=0x8c61e484u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61e484; /* jsr @r2 */
L8c61e484: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61e486; /* frchg */
L8c61e486: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e488; /* fmov.s @r15+,fr15 */
L8c61e488: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e48a; /* fmov.s @r15+,fr14 */
L8c61e48a: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e48c; /* fmov.s @r15+,fr13 */
L8c61e48c: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e48e; /* fmov.s @r15+,fr12 */
L8c61e48e: fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e490; /* fmov.s @r15+,fr11 */
L8c61e490: fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e492; /* fmov.s @r15+,fr10 */
L8c61e492: fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e494; /* fmov.s @r15+,fr9 */
L8c61e494: fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e496; /* fmov.s @r15+,fr8 */
L8c61e496: fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e498; /* fmov.s @r15+,fr7 */
L8c61e498: fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e49a; /* fmov.s @r15+,fr6 */
L8c61e49a: fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e49c; /* fmov.s @r15+,fr5 */
L8c61e49c: fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e49e; /* fmov.s @r15+,fr4 */
L8c61e49e: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e4a0; /* fmov.s @r15+,fr3 */
L8c61e4a0: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e4a2; /* fmov.s @r15+,fr2 */
L8c61e4a2: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e4a4; /* fmov.s @r15+,fr1 */
L8c61e4a4: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e4a6; /* fmov.s @r15+,fr0 */
L8c61e4a6: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61e4a8; /* frchg */
L8c61e4a8: r[15]+=16u;goto L8c61e4aa; /* add #0x10,r15 */
L8c61e4aa: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61e4ac; /* lds.l @r15+,PR */
L8c61e4ac: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e4ae; /* fmov.s @r15+,fr15 */
L8c61e4ae: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61e4b0; /* mov.l @r15+,r9 */
L8c61e4b0: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61e4b2; /* mov.l @r15+,r10 */
L8c61e4b2: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61e4b4; /* mov.l @r15+,r11 */
L8c61e4b4: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61e4b6; /* mov.l @r15+,r12 */
L8c61e4b6: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61e4b8; /* mov.l @r15+,r13 */
L8c61e4b8: if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_environment_face_primitive_clipped_8c61e4f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 uint32_t entry_pr=*pr;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61e4f4;
L8c61e4f4: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61e4f6; /* mov.l r14,@-r15 */
L8c61e4f6: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61e4f8; /* mov.l r13,@-r15 */
L8c61e4f8: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61e4fa; /* mov.l r12,@-r15 */
L8c61e4fa: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61e4fc; /* mov.l r11,@-r15 */
L8c61e4fc: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61e4fe; /* mov.l r10,@-r15 */
L8c61e4fe: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61e500; /* mov.l r9,@-r15 */
L8c61e500: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61e502; /* fmov.s fr15,@-r15 */
L8c61e502: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61e504; /* fmov.s fr14,@-r15 */
L8c61e504: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61e506; /* sts.l PR,@-r15 */
L8c61e506: r[15]+=4294967272u;goto L8c61e508; /* add #-0x18,r15 */
L8c61e508: r[0]=0x8c61e560u;goto L8c61e50a; /* mova 0x8c61e560,r0 */
L8c61e50a: r[3]=rd(m,bus,&io_status,0x8c61e564u,4);if(io_status)return io_status;goto L8c61e50c; /* mov.l 0x8c61e564,r3 */
L8c61e50c: fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c61e50e; /* fmov.s @r0,fr14 */
L8c61e50e: r[0]=r[4];goto L8c61e510; /* mov r4,r0 */
L8c61e510: wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61e512; /* mov.l r4,@r15 */
L8c61e512: r[10]=r[4];goto L8c61e514; /* mov r4,r10 */
L8c61e514: r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61e516; /* mov.w @r0,r0 */
L8c61e516: r[0]=r[0]&65535u;goto L8c61e518; /* extu.w r0,r0 */
L8c61e518: r[0]&=r[3];goto L8c61e51a; /* and r3,r0 */
L8c61e51a: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61e51c; /* cmp/eq #0x0,r0 */
L8c61e51c: take=(*sr&1u);r[10]=rd(m,bus,&io_status,r[10]+0x4u,4);if(io_status)return io_status;if(take)goto L8c61e542;goto L8c61e520; /* bt/s 0x8c61e542 */
L8c61e520: r[1]=sx(m,bus,&io_status,0x8c61e55eu,2);if(io_status)return io_status;goto L8c61e522; /* mov.w 0x8c61e55e,r1 */
L8c61e522: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61e524; /* cmp/eq r1,r0 */
L8c61e524: if(!(*sr&1u))goto L8c61e52a;goto L8c61e526; /* bf 0x8c61e52a */
L8c61e526: goto L8c61e82c; /* bra 0x8c61e82c */
L8c61e52a: r[1]=rd(m,bus,&io_status,0x8c61e568u,4);if(io_status)return io_status;goto L8c61e52c; /* mov.l 0x8c61e568,r1 */
L8c61e52c: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61e52e; /* cmp/eq r1,r0 */
L8c61e52e: if(!(*sr&1u))goto L8c61e534;goto L8c61e530; /* bf 0x8c61e534 */
L8c61e530: goto L8c61ec08; /* bra 0x8c61ec08 */
L8c61e534: r[1]=rd(m,bus,&io_status,0x8c61e564u,4);if(io_status)return io_status;goto L8c61e536; /* mov.l 0x8c61e564,r1 */
L8c61e536: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61e538; /* cmp/eq r1,r0 */
L8c61e538: if(!(*sr&1u))goto L8c61e53e;goto L8c61e53a; /* bf 0x8c61e53e */
L8c61e53a: goto L8c61f042; /* bra 0x8c61f042 */
L8c61e53e: goto L8c61f0bc; /* bra 0x8c61f0bc */
L8c61e542: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61e544; /* mov.l @r15,r2 */
L8c61e544: r[1]=rd(m,bus,&io_status,0x8c61e56cu,4);if(io_status)return io_status;goto L8c61e546; /* mov.l 0x8c61e56c,r1 */
L8c61e546: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61e548; /* mov.w @(0x2,r2),r0 */
L8c61e548: r[0]=r[0]&65535u;goto L8c61e54a; /* extu.w r0,r0 */
L8c61e54a: r[2]=r[0];goto L8c61e54c; /* mov r0,r2 */
L8c61e54c: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61e54e; /* shll r0 */
L8c61e54e: r[0]+=r[2];goto L8c61e550; /* add r2,r0 */
L8c61e550: r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61e552; /* mov.l @r1,r2 */
L8c61e552: r[2]+=r[0];goto L8c61e554; /* add r0,r2 */
L8c61e554: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61e556; /* mov.l r2,@r1 */
L8c61e556: r[11]=0u;goto L8c61e81a; /* bra 0x8c61e81a */
L8c61e570: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e572; /* mov.w @r10+,r14 */
L8c61e572: r[3]=rd(m,bus,&io_status,0x8c61e840u,4);if(io_status)return io_status;goto L8c61e574; /* mov.l 0x8c61e840,r3 */
L8c61e574: r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e576; /* mov.w @r10+,r12 */
L8c61e576: r[14]=r[14]&65535u;goto L8c61e578; /* extu.w r14,r14 */
L8c61e578: r[14]<<=2;goto L8c61e57a; /* shll2 r14 */
L8c61e57a: r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e57c; /* mov.l @r3,r5 */
L8c61e57c: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e57e; /* mov.w @r10+,r13 */
L8c61e57e: r[12]=r[12]&65535u;goto L8c61e580; /* extu.w r12,r12 */
L8c61e580: r[12]<<=2;goto L8c61e582; /* shll2 r12 */
L8c61e582: r[13]=r[13]&65535u;goto L8c61e584; /* extu.w r13,r13 */
L8c61e584: r[12]<<=2;goto L8c61e586; /* shll2 r12 */
L8c61e586: r[13]<<=2;goto L8c61e588; /* shll2 r13 */
L8c61e588: r[14]<<=2;goto L8c61e58a; /* shll2 r14 */
L8c61e58a: r[13]<<=2;goto L8c61e58c; /* shll2 r13 */
L8c61e58c: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61e58e; /* shll r14 */
L8c61e58e: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61e590; /* shll r12 */
L8c61e590: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61e592; /* shll r13 */
L8c61e592: r[14]+=r[5];goto L8c61e594; /* add r5,r14 */
L8c61e594: r[12]+=r[5];goto L8c61e596; /* add r5,r12 */
L8c61e596: r[4]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61e598; /* mov.l @(0xc,r14),r4 */
L8c61e598: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61e59a; /* mov.l @(0xc,r12),r2 */
L8c61e59a: r[13]+=r[5];goto L8c61e59c; /* add r5,r13 */
L8c61e59c: r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61e59e; /* mov.l @(0xc,r13),r1 */
L8c61e59e: r[4]&=r[2];goto L8c61e5a0; /* and r2,r4 */
L8c61e5a0: r[2]=63u;goto L8c61e5a2; /* mov #0x3f,r2 */
L8c61e5a2: r[4]&=r[1];goto L8c61e5a4; /* and r1,r4 */
L8c61e5a4: *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61e5a6; /* tst r2,r4 */
L8c61e5a6: if((*sr&1u))goto L8c61e5ac;goto L8c61e5a8; /* bt 0x8c61e5ac */
L8c61e5a8: goto L8c61e818; /* bra 0x8c61e818 */
L8c61e5ac: r[4]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61e5ae; /* mov.l @(0xc,r14),r4 */
L8c61e5ae: r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61e5b0; /* mov.l @(0xc,r12),r3 */
L8c61e5b0: r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61e5b2; /* mov.l @(0xc,r13),r2 */
L8c61e5b2: r[4]|=r[3];goto L8c61e5b4; /* or r3,r4 */
L8c61e5b4: r[3]=1u;goto L8c61e5b6; /* mov #0x1,r3 */
L8c61e5b6: r[4]|=r[2];goto L8c61e5b8; /* or r2,r4 */
L8c61e5b8: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61e5ba; /* tst r3,r4 */
L8c61e5ba: if((*sr&1u))goto L8c61e5c0;goto L8c61e5bc; /* bt 0x8c61e5c0 */
L8c61e5bc: goto L8c61e7aa; /* bra 0x8c61e7aa */
L8c61e5c0: r[0]=16u;goto L8c61e5c2; /* mov #0x10,r0 */
L8c61e5c2: fload(m,bus,&io_status,f,4u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e5c4; /* fmov.s @(r0,r12),fr4 */
L8c61e5c4: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e5c6; /* fmov.s @(r0,r14),fr3 */
L8c61e5c6: r[0]=20u;goto L8c61e5c8; /* mov #0x14,r0 */
L8c61e5c8: fcopy(f,2u,4u);goto L8c61e5ca; /* fmov fr4,fr2 */
L8c61e5ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61e5cc; /* fsub fr3,fr2 */
L8c61e5cc: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e5ce; /* fmov.s @(r0,r13),fr3 */
L8c61e5ce: fload(m,bus,&io_status,f,5u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e5d0; /* fmov.s @(r0,r12),fr5 */
L8c61e5d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e5d2; /* fsub fr5,fr3 */
L8c61e5d2: fcopy(f,1u,5u);goto L8c61e5d4; /* fmov fr5,fr1 */
L8c61e5d4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e5d6; /* fmul fr3,fr2 */
L8c61e5d6: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e5d8; /* fmov.s @(r0,r14),fr3 */
L8c61e5d8: r[0]=16u;goto L8c61e5da; /* mov #0x10,r0 */
L8c61e5da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61e5dc; /* fsub fr3,fr1 */
L8c61e5dc: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e5de; /* fmov.s @(r0,r13),fr3 */
L8c61e5de: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e5e0; /* fsub fr4,fr3 */
L8c61e5e0: fcopy(f,4u,2u);goto L8c61e5e2; /* fmov fr2,fr4 */
L8c61e5e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e5e4; /* fmul fr3,fr1 */
L8c61e5e4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61e5e6; /* fldi0 fr3 */
L8c61e5e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61e5e8; /* fsub fr1,fr4 */
L8c61e5e8: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61e5ea; /* fcmp/gt fr3,fr4 */
L8c61e5ea: if(!(*sr&1u))goto L8c61e5f0;goto L8c61e5ec; /* bf 0x8c61e5f0 */
L8c61e5ec: goto L8c61e818; /* bra 0x8c61e818 */
L8c61e5f0: fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c61e5f2; /* fmov.s @r14,fr2 */
L8c61e5f2: r[0]=12u;goto L8c61e5f4; /* mov #0xc,r0 */
L8c61e5f4: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61e5f6; /* fmov.s @r12,fr3 */
L8c61e5f6: r[4]=r[15];goto L8c61e5f8; /* mov r15,r4 */
L8c61e5f8: r[3]=rd(m,bus,&io_status,0x8c61e844u,4);if(io_status)return io_status;goto L8c61e5fa; /* mov.l 0x8c61e844,r3 */
L8c61e5fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e5fc; /* fadd fr3,fr2 */
L8c61e5fc: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61e5fe; /* fmov.s @r13,fr3 */
L8c61e5fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e600; /* fadd fr3,fr2 */
L8c61e600: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e602; /* fmov fr2,@(r0,r15) */
L8c61e602: r[0]=4u;goto L8c61e604; /* mov #0x4,r0 */
L8c61e604: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e606; /* fmov.s @(r0,r14),fr2 */
L8c61e606: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e608; /* fmov.s @(r0,r12),fr3 */
L8c61e608: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e60a; /* fadd fr3,fr2 */
L8c61e60a: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e60c; /* fmov.s @(r0,r13),fr3 */
L8c61e60c: r[0]=16u;goto L8c61e60e; /* mov #0x10,r0 */
L8c61e60e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e610; /* fadd fr3,fr2 */
L8c61e610: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e612; /* fmov fr2,@(r0,r15) */
L8c61e612: r[0]=8u;goto L8c61e614; /* mov #0x8,r0 */
L8c61e614: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e616; /* fmov.s @(r0,r14),fr2 */
L8c61e616: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e618; /* fmov.s @(r0,r12),fr3 */
L8c61e618: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e61a; /* fadd fr3,fr2 */
L8c61e61a: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e61c; /* fmov.s @(r0,r13),fr3 */
L8c61e61c: r[0]=20u;goto L8c61e61e; /* mov #0x14,r0 */
L8c61e61e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e620; /* fadd fr3,fr2 */
L8c61e620: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e622; /* fmov fr2,@(r0,r15) */
L8c61e622: target=r[3];*pr=0x8c61e626u;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61e626; /* jsr @r3 */
L8c61e626: r[3]=rd(m,bus,&io_status,0x8c61e848u,4);if(io_status)return io_status;goto L8c61e628; /* mov.l 0x8c61e848,r3 */
L8c61e628: r[0]=12u;goto L8c61e62a; /* mov #0xc,r0 */
L8c61e62a: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e62c; /* fmov.s @(r0,r15),fr3 */
L8c61e62c: r[0]=16u;goto L8c61e62e; /* mov #0x10,r0 */
L8c61e62e: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61e630; /* fmov.s @r3,fr2 */
L8c61e630: r[3]+=8u;goto L8c61e632; /* add #0x8,r3 */
L8c61e632: r[2]=rd(m,bus,&io_status,0x8c61e84cu,4);if(io_status)return io_status;goto L8c61e634; /* mov.l 0x8c61e84c,r2 */
L8c61e634: r[8]+=24u;goto L8c61e636; /* add #0x18,r8 */
L8c61e636: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e638; /* fmul fr3,fr2 */
L8c61e638: r[1]=rd(m,bus,&io_status,0x8c61e850u,4);if(io_status)return io_status;goto L8c61e63a; /* mov.l 0x8c61e850,r1 */
L8c61e63a: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61e63c; /* fmov.s @r2,fr3 */
L8c61e63c: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61e63e; /* fmov.s @r1,fr1 */
L8c61e63e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e640; /* fsub fr2,fr3 */
L8c61e640: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e642; /* fmov.s @(r0,r15),fr2 */
L8c61e642: r[0]=20u;goto L8c61e644; /* mov #0x14,r0 */
L8c61e644: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e646; /* fmul fr2,fr1 */
L8c61e646: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e648; /* fmov.s @(r0,r15),fr2 */
L8c61e648: r[0]=rd(m,bus,&io_status,0x8c61e854u,4);if(io_status)return io_status;goto L8c61e64a; /* mov.l 0x8c61e854,r0 */
L8c61e64a: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61e64c; /* mov.l @r0,r2 */
L8c61e64c: r[0]=4u;goto L8c61e64e; /* mov #0x4,r0 */
L8c61e64e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e650; /* fsub fr1,fr3 */
L8c61e650: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61e652; /* fmov.s @r3,fr1 */
L8c61e652: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e654; /* fmov.s @(r0,r14),fr0 */
L8c61e654: r[1]=r[2];goto L8c61e656; /* mov r2,r1 */
L8c61e656: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e658; /* fmul fr2,fr1 */
L8c61e658: r[0]=20u;goto L8c61e65a; /* mov #0x14,r0 */
L8c61e65a: r[3]=r[2];goto L8c61e65c; /* mov r2,r3 */
L8c61e65c: fcopy(f,4u,3u);goto L8c61e65e; /* fmov fr3,fr4 */
L8c61e65e: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e660; /* fmov.s @(r0,r1),fr3 */
L8c61e660: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61e662; /* fsub fr1,fr4 */
L8c61e662: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e664; /* fmul fr0,fr3 */
L8c61e664: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61e666; /* fmov.s @r14,fr0 */
L8c61e666: r[0]=4u;goto L8c61e668; /* mov #0x4,r0 */
L8c61e668: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e66a; /* fmov.s @(r0,r3),fr2 */
L8c61e66a: r[0]=8u;goto L8c61e66c; /* mov #0x8,r0 */
L8c61e66c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e66e; /* fmac fr0,fr2,fr3 */
L8c61e66e: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e670; /* fmov.s @(r0,r14),fr0 */
L8c61e670: r[0]=36u;goto L8c61e672; /* mov #0x24,r0 */
L8c61e672: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e674; /* fmov.s @(r0,r2),fr2 */
L8c61e674: r[0]=4u;goto L8c61e676; /* mov #0x4,r0 */
L8c61e676: r[2]=rd(m,bus,&io_status,0x8c61e854u,4);if(io_status)return io_status;goto L8c61e678; /* mov.l 0x8c61e854,r2 */
L8c61e678: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e67a; /* fmac fr0,fr2,fr3 */
L8c61e67a: fcopy(f,0u,14u);goto L8c61e67c; /* fmov fr14,fr0 */
L8c61e67c: fcopy(f,2u,14u);goto L8c61e67e; /* fmov fr14,fr2 */
L8c61e67e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e680; /* fmac fr0,fr3,fr2 */
L8c61e680: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e682; /* fmov.s fr2,@-r8 */
L8c61e682: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e684; /* mov.l @r2,r3 */
L8c61e684: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e686; /* fmov.s @(r0,r14),fr0 */
L8c61e686: r[0]=16u;goto L8c61e688; /* mov #0x10,r0 */
L8c61e688: r[2]=r[3];goto L8c61e68a; /* mov r3,r2 */
L8c61e68a: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e68c; /* fmov.s @(r0,r2),fr3 */
L8c61e68c: r[1]=r[3];goto L8c61e68e; /* mov r3,r1 */
L8c61e68e: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61e690; /* fmov.s @r1,fr2 */
L8c61e690: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e692; /* fmul fr0,fr3 */
L8c61e692: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61e694; /* fmov.s @r14,fr0 */
L8c61e694: r[0]=8u;goto L8c61e696; /* mov #0x8,r0 */
L8c61e696: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e698; /* fmac fr0,fr2,fr3 */
L8c61e698: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e69a; /* fmov.s @(r0,r14),fr0 */
L8c61e69a: r[0]=32u;goto L8c61e69c; /* mov #0x20,r0 */
L8c61e69c: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e69e; /* fmov.s @(r0,r3),fr2 */
L8c61e69e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e6a0; /* fmac fr0,fr2,fr3 */
L8c61e6a0: fcopy(f,0u,14u);goto L8c61e6a2; /* fmov fr14,fr0 */
L8c61e6a2: fcopy(f,2u,14u);goto L8c61e6a4; /* fmov fr14,fr2 */
L8c61e6a4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e6a6; /* fmac fr0,fr3,fr2 */
L8c61e6a6: r[0]=24u;goto L8c61e6a8; /* mov #0x18,r0 */
L8c61e6a8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e6aa; /* fmov.s fr2,@-r8 */
L8c61e6aa: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e6ac; /* fmov.s @(r0,r14),fr3 */
L8c61e6ac: r[0]=20u;goto L8c61e6ae; /* mov #0x14,r0 */
L8c61e6ae: r[3]=rd(m,bus,&io_status,0x8c61e858u,4);if(io_status)return io_status;goto L8c61e6b0; /* mov.l 0x8c61e858,r3 */
L8c61e6b0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e6b2; /* fmov.s fr3,@-r8 */
L8c61e6b2: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e6b4; /* fmov.s @(r0,r14),fr3 */
L8c61e6b4: r[0]=16u;goto L8c61e6b6; /* mov #0x10,r0 */
L8c61e6b6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e6b8; /* fmov.s fr3,@-r8 */
L8c61e6b8: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e6ba; /* fmov.s @(r0,r14),fr3 */
L8c61e6ba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e6bc; /* fmov.s fr3,@-r8 */
L8c61e6bc: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61e6be; /* mov.l r3,@-r8 */
L8c61e6be: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e6c0; /* pref @r8 */
L8c61e6c0: r[2]=rd(m,bus,&io_status,0x8c61e854u,4);if(io_status)return io_status;goto L8c61e6c2; /* mov.l 0x8c61e854,r2 */
L8c61e6c2: r[0]=4u;goto L8c61e6c4; /* mov #0x4,r0 */
L8c61e6c4: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e6c6; /* fmov.s @(r0,r12),fr0 */
L8c61e6c6: r[0]=20u;goto L8c61e6c8; /* mov #0x14,r0 */
L8c61e6c8: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e6ca; /* mov.l @r2,r3 */
L8c61e6ca: r[8]+=56u;goto L8c61e6cc; /* add #0x38,r8 */
L8c61e6cc: r[2]=r[3];goto L8c61e6ce; /* mov r3,r2 */
L8c61e6ce: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e6d0; /* fmov.s @(r0,r2),fr3 */
L8c61e6d0: r[0]=4u;goto L8c61e6d2; /* mov #0x4,r0 */
L8c61e6d2: r[1]=r[3];goto L8c61e6d4; /* mov r3,r1 */
L8c61e6d4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e6d6; /* fmul fr0,fr3 */
L8c61e6d6: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e6d8; /* fmov.s @(r0,r1),fr2 */
L8c61e6d8: fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61e6da; /* fmov.s @r12,fr0 */
L8c61e6da: r[0]=8u;goto L8c61e6dc; /* mov #0x8,r0 */
L8c61e6dc: r[2]=rd(m,bus,&io_status,0x8c61e854u,4);if(io_status)return io_status;goto L8c61e6de; /* mov.l 0x8c61e854,r2 */
L8c61e6de: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e6e0; /* fmac fr0,fr2,fr3 */
L8c61e6e0: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e6e2; /* fmov.s @(r0,r12),fr0 */
L8c61e6e2: r[0]=36u;goto L8c61e6e4; /* mov #0x24,r0 */
L8c61e6e4: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e6e6; /* fmov.s @(r0,r3),fr2 */
L8c61e6e6: r[0]=4u;goto L8c61e6e8; /* mov #0x4,r0 */
L8c61e6e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e6ea; /* fmac fr0,fr2,fr3 */
L8c61e6ea: fcopy(f,0u,14u);goto L8c61e6ec; /* fmov fr14,fr0 */
L8c61e6ec: fcopy(f,2u,14u);goto L8c61e6ee; /* fmov fr14,fr2 */
L8c61e6ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e6f0; /* fmac fr0,fr3,fr2 */
L8c61e6f0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e6f2; /* fmov.s fr2,@-r8 */
L8c61e6f2: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e6f4; /* mov.l @r2,r3 */
L8c61e6f4: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e6f6; /* fmov.s @(r0,r12),fr0 */
L8c61e6f6: r[0]=16u;goto L8c61e6f8; /* mov #0x10,r0 */
L8c61e6f8: r[2]=r[3];goto L8c61e6fa; /* mov r3,r2 */
L8c61e6fa: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e6fc; /* fmov.s @(r0,r2),fr3 */
L8c61e6fc: r[1]=r[3];goto L8c61e6fe; /* mov r3,r1 */
L8c61e6fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e700; /* fmul fr0,fr3 */
L8c61e700: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61e702; /* fmov.s @r1,fr2 */
L8c61e702: r[0]=8u;goto L8c61e704; /* mov #0x8,r0 */
L8c61e704: fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61e706; /* fmov.s @r12,fr0 */
L8c61e706: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e708; /* fmac fr0,fr2,fr3 */
L8c61e708: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e70a; /* fmov.s @(r0,r12),fr0 */
L8c61e70a: r[0]=32u;goto L8c61e70c; /* mov #0x20,r0 */
L8c61e70c: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e70e; /* fmov.s @(r0,r3),fr2 */
L8c61e70e: r[0]=24u;goto L8c61e710; /* mov #0x18,r0 */
L8c61e710: r[3]=rd(m,bus,&io_status,0x8c61e858u,4);if(io_status)return io_status;goto L8c61e712; /* mov.l 0x8c61e858,r3 */
L8c61e712: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e714; /* fmac fr0,fr2,fr3 */
L8c61e714: fcopy(f,0u,14u);goto L8c61e716; /* fmov fr14,fr0 */
L8c61e716: fcopy(f,2u,14u);goto L8c61e718; /* fmov fr14,fr2 */
L8c61e718: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e71a; /* fmac fr0,fr3,fr2 */
L8c61e71a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e71c; /* fmov.s fr2,@-r8 */
L8c61e71c: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e71e; /* fmov.s @(r0,r12),fr3 */
L8c61e71e: r[0]=20u;goto L8c61e720; /* mov #0x14,r0 */
L8c61e720: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e722; /* fmov.s fr3,@-r8 */
L8c61e722: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e724; /* fmov.s @(r0,r12),fr3 */
L8c61e724: r[0]=16u;goto L8c61e726; /* mov #0x10,r0 */
L8c61e726: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e728; /* fmov.s fr3,@-r8 */
L8c61e728: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e72a; /* fmov.s @(r0,r12),fr3 */
L8c61e72a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e72c; /* fmov.s fr3,@-r8 */
L8c61e72c: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61e72e; /* mov.l r3,@-r8 */
L8c61e72e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e730; /* pref @r8 */
L8c61e730: r[2]=rd(m,bus,&io_status,0x8c61e854u,4);if(io_status)return io_status;goto L8c61e732; /* mov.l 0x8c61e854,r2 */
L8c61e732: r[0]=4u;goto L8c61e734; /* mov #0x4,r0 */
L8c61e734: r[8]+=60u;goto L8c61e736; /* add #0x3c,r8 */
L8c61e736: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61e738; /* fmov.s fr4,@-r8 */
L8c61e738: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e73a; /* mov.l @r2,r3 */
L8c61e73a: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e73c; /* fmov.s @(r0,r13),fr0 */
L8c61e73c: r[0]=20u;goto L8c61e73e; /* mov #0x14,r0 */
L8c61e73e: r[2]=r[3];goto L8c61e740; /* mov r3,r2 */
L8c61e740: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e742; /* fmov.s @(r0,r2),fr3 */
L8c61e742: r[1]=r[3];goto L8c61e744; /* mov r3,r1 */
L8c61e744: r[0]=4u;goto L8c61e746; /* mov #0x4,r0 */
L8c61e746: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e748; /* fmul fr0,fr3 */
L8c61e748: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e74a; /* fmov.s @(r0,r1),fr2 */
L8c61e74a: fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61e74c; /* fmov.s @r13,fr0 */
L8c61e74c: r[0]=8u;goto L8c61e74e; /* mov #0x8,r0 */
L8c61e74e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e750; /* fmac fr0,fr2,fr3 */
L8c61e750: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e752; /* fmov.s @(r0,r13),fr0 */
L8c61e752: r[0]=36u;goto L8c61e754; /* mov #0x24,r0 */
L8c61e754: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e756; /* fmov.s @(r0,r3),fr2 */
L8c61e756: r[0]=4u;goto L8c61e758; /* mov #0x4,r0 */
L8c61e758: r[3]=rd(m,bus,&io_status,0x8c61e854u,4);if(io_status)return io_status;goto L8c61e75a; /* mov.l 0x8c61e854,r3 */
L8c61e75a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e75c; /* fmac fr0,fr2,fr3 */
L8c61e75c: fcopy(f,2u,14u);goto L8c61e75e; /* fmov fr14,fr2 */
L8c61e75e: fcopy(f,0u,14u);goto L8c61e760; /* fmov fr14,fr0 */
L8c61e760: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e762; /* fmac fr0,fr3,fr2 */
L8c61e762: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e764; /* fmov.s fr2,@-r8 */
L8c61e764: r[9]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e766; /* mov.l @r3,r9 */
L8c61e766: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e768; /* fmov.s @(r0,r13),fr0 */
L8c61e768: r[0]=16u;goto L8c61e76a; /* mov #0x10,r0 */
L8c61e76a: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61e76c; /* fmov.s @(r0,r9),fr3 */
L8c61e76c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e76e; /* fmul fr0,fr3 */
L8c61e76e: fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61e770; /* fmov.s @r13,fr0 */
L8c61e770: fload(m,bus,&io_status,f,2u,r[9]);if(io_status)return io_status;goto L8c61e772; /* fmov.s @r9,fr2 */
L8c61e772: r[0]=8u;goto L8c61e774; /* mov #0x8,r0 */
L8c61e774: r[2]=rd(m,bus,&io_status,0x8c61e85cu,4);if(io_status)return io_status;goto L8c61e776; /* mov.l 0x8c61e85c,r2 */
L8c61e776: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e778; /* fmac fr0,fr2,fr3 */
L8c61e778: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e77a; /* fmov.s @(r0,r13),fr0 */
L8c61e77a: r[0]=32u;goto L8c61e77c; /* mov #0x20,r0 */
L8c61e77c: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61e77e; /* fmov.s @(r0,r9),fr2 */
L8c61e77e: r[0]=24u;goto L8c61e780; /* mov #0x18,r0 */
L8c61e780: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e782; /* fmac fr0,fr2,fr3 */
L8c61e782: fcopy(f,0u,14u);goto L8c61e784; /* fmov fr14,fr0 */
L8c61e784: fcopy(f,2u,14u);goto L8c61e786; /* fmov fr14,fr2 */
L8c61e786: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e788; /* fmac fr0,fr3,fr2 */
L8c61e788: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e78a; /* fmov.s fr2,@-r8 */
L8c61e78a: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e78c; /* fmov.s @(r0,r13),fr3 */
L8c61e78c: r[0]=20u;goto L8c61e78e; /* mov #0x14,r0 */
L8c61e78e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e790; /* fmov.s fr3,@-r8 */
L8c61e790: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e792; /* fmov.s @(r0,r13),fr3 */
L8c61e792: r[0]=16u;goto L8c61e794; /* mov #0x10,r0 */
L8c61e794: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e796; /* fmov.s fr3,@-r8 */
L8c61e796: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e798; /* fmov.s @(r0,r13),fr3 */
L8c61e798: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e79a; /* fmov.s fr3,@-r8 */
L8c61e79a: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61e79c; /* mov.l r2,@-r8 */
L8c61e79c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e79e; /* pref @r8 */
L8c61e79e: r[1]=rd(m,bus,&io_status,0x8c61e860u,4);if(io_status)return io_status;goto L8c61e7a0; /* mov.l 0x8c61e860,r1 */
L8c61e7a0: r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61e7a2; /* mov.l @r1,r2 */
L8c61e7a2: r[2]+=3u;goto L8c61e7a4; /* add #0x3,r2 */
L8c61e7a4: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61e7a6; /* mov.l r2,@r1 */
L8c61e7a6: r[8]+=32u;goto L8c61e818; /* bra 0x8c61e818 */
L8c61e7aa: r[0]=rd(m,bus,&io_status,0x8c61e864u,4);if(io_status)return io_status;goto L8c61e7ac; /* mov.l 0x8c61e864,r0 */
L8c61e7ac: r[4]=r[15];goto L8c61e7ae; /* mov r15,r4 */
L8c61e7ae: wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61e7b0; /* mov.l r3,@r0 */
L8c61e7b0: r[0]=12u;goto L8c61e7b2; /* mov #0xc,r0 */
L8c61e7b2: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61e7b4; /* fmov.s @r12,fr3 */
L8c61e7b4: fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c61e7b6; /* fmov.s @r14,fr2 */
L8c61e7b6: r[3]=rd(m,bus,&io_status,0x8c61e844u,4);if(io_status)return io_status;goto L8c61e7b8; /* mov.l 0x8c61e844,r3 */
L8c61e7b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e7ba; /* fadd fr3,fr2 */
L8c61e7ba: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61e7bc; /* fmov.s @r13,fr3 */
L8c61e7bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e7be; /* fadd fr3,fr2 */
L8c61e7be: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e7c0; /* fmov fr2,@(r0,r15) */
L8c61e7c0: r[0]=4u;goto L8c61e7c2; /* mov #0x4,r0 */
L8c61e7c2: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e7c4; /* fmov.s @(r0,r14),fr2 */
L8c61e7c4: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e7c6; /* fmov.s @(r0,r12),fr3 */
L8c61e7c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e7c8; /* fadd fr3,fr2 */
L8c61e7c8: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e7ca; /* fmov.s @(r0,r13),fr3 */
L8c61e7ca: r[0]=16u;goto L8c61e7cc; /* mov #0x10,r0 */
L8c61e7cc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e7ce; /* fadd fr3,fr2 */
L8c61e7ce: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e7d0; /* fmov fr2,@(r0,r15) */
L8c61e7d0: r[0]=8u;goto L8c61e7d2; /* mov #0x8,r0 */
L8c61e7d2: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e7d4; /* fmov.s @(r0,r14),fr2 */
L8c61e7d4: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e7d6; /* fmov.s @(r0,r12),fr3 */
L8c61e7d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e7d8; /* fadd fr3,fr2 */
L8c61e7d8: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e7da; /* fmov.s @(r0,r13),fr3 */
L8c61e7da: r[0]=20u;goto L8c61e7dc; /* mov #0x14,r0 */
L8c61e7dc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e7de; /* fadd fr3,fr2 */
L8c61e7de: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e7e0; /* fmov fr2,@(r0,r15) */
L8c61e7e0: target=r[3];*pr=0x8c61e7e4u;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61e7e4; /* jsr @r3 */
L8c61e7e4: r[3]=rd(m,bus,&io_status,0x8c61e848u,4);if(io_status)return io_status;goto L8c61e7e6; /* mov.l 0x8c61e848,r3 */
L8c61e7e6: r[0]=12u;goto L8c61e7e8; /* mov #0xc,r0 */
L8c61e7e8: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e7ea; /* fmov.s @(r0,r15),fr3 */
L8c61e7ea: r[0]=16u;goto L8c61e7ec; /* mov #0x10,r0 */
L8c61e7ec: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61e7ee; /* fmov.s @r3,fr2 */
L8c61e7ee: r[3]+=8u;goto L8c61e7f0; /* add #0x8,r3 */
L8c61e7f0: r[2]=rd(m,bus,&io_status,0x8c61e84cu,4);if(io_status)return io_status;goto L8c61e7f2; /* mov.l 0x8c61e84c,r2 */
L8c61e7f2: r[5]=r[12];goto L8c61e7f4; /* mov r12,r5 */
L8c61e7f4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e7f6; /* fmul fr3,fr2 */
L8c61e7f6: r[1]=rd(m,bus,&io_status,0x8c61e850u,4);if(io_status)return io_status;goto L8c61e7f8; /* mov.l 0x8c61e850,r1 */
L8c61e7f8: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61e7fa; /* fmov.s @r2,fr3 */
L8c61e7fa: r[6]=r[13];goto L8c61e7fc; /* mov r13,r6 */
L8c61e7fc: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61e7fe; /* fmov.s @r1,fr1 */
L8c61e7fe: r[2]=rd(m,bus,&io_status,0x8c61e868u,4);if(io_status)return io_status;goto L8c61e800; /* mov.l 0x8c61e868,r2 */
L8c61e800: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e802; /* fsub fr2,fr3 */
L8c61e802: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e804; /* fmov.s @(r0,r15),fr2 */
L8c61e804: r[0]=20u;goto L8c61e806; /* mov #0x14,r0 */
L8c61e806: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e808; /* fmul fr2,fr1 */
L8c61e808: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e80a; /* fmov.s @(r0,r15),fr2 */
L8c61e80a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e80c; /* fsub fr1,fr3 */
L8c61e80c: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61e80e; /* fmov.s @r3,fr1 */
L8c61e80e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e810; /* fmul fr2,fr1 */
L8c61e810: fcopy(f,4u,3u);goto L8c61e812; /* fmov fr3,fr4 */
L8c61e812: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61e814; /* fsub fr1,fr4 */
L8c61e814: target=r[2];*pr=0x8c61e818u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61e818; /* jsr @r2 */
L8c61e818: r[11]+=1u;goto L8c61e81a; /* add #0x1,r11 */
L8c61e81a: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61e81c; /* mov.l @r15,r3 */
L8c61e81c: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61e81e; /* mov.w @(0x2,r3),r0 */
L8c61e81e: r[0]=r[0]&65535u;goto L8c61e820; /* extu.w r0,r0 */
L8c61e820: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61e822; /* cmp/ge r0,r11 */
L8c61e822: if((*sr&1u))goto L8c61e828;goto L8c61e824; /* bt 0x8c61e828 */
L8c61e824: goto L8c61e570; /* bra 0x8c61e570 */
L8c61e828: goto L8c61f0bc; /* bra 0x8c61f0bc */
L8c61e82c: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61e82e; /* mov.l @r15,r1 */
L8c61e82e: r[2]=rd(m,bus,&io_status,0x8c61e86cu,4);if(io_status)return io_status;goto L8c61e830; /* mov.l 0x8c61e86c,r2 */
L8c61e830: r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61e832; /* mov.w @(0x2,r1),r0 */
L8c61e832: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e834; /* mov.l @r2,r1 */
L8c61e834: r[0]=r[0]&65535u;goto L8c61e836; /* extu.w r0,r0 */
L8c61e836: r[0]<<=2;goto L8c61e838; /* shll2 r0 */
L8c61e838: r[1]+=r[0];goto L8c61e83a; /* add r0,r1 */
L8c61e83a: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61e83c; /* mov.l r1,@r2 */
L8c61e83c: r[9]=0u;goto L8c61ebf6; /* bra 0x8c61ebf6 */
L8c61e870: r[11]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e872; /* mov.w @r10+,r11 */
L8c61e872: r[3]=rd(m,bus,&io_status,0x8c61eb34u,4);if(io_status)return io_status;goto L8c61e874; /* mov.l 0x8c61eb34,r3 */
L8c61e874: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e876; /* mov.w @r10+,r14 */
L8c61e876: r[11]=r[11]&65535u;goto L8c61e878; /* extu.w r11,r11 */
L8c61e878: r[11]<<=2;goto L8c61e87a; /* shll2 r11 */
L8c61e87a: r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e87c; /* mov.l @r3,r5 */
L8c61e87c: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e87e; /* mov.w @r10+,r13 */
L8c61e87e: r[14]=r[14]&65535u;goto L8c61e880; /* extu.w r14,r14 */
L8c61e880: r[14]<<=2;goto L8c61e882; /* shll2 r14 */
L8c61e882: r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e884; /* mov.w @r10+,r12 */
L8c61e884: r[13]=r[13]&65535u;goto L8c61e886; /* extu.w r13,r13 */
L8c61e886: r[14]<<=2;goto L8c61e888; /* shll2 r14 */
L8c61e888: r[11]<<=2;goto L8c61e88a; /* shll2 r11 */
L8c61e88a: r[12]=r[12]&65535u;goto L8c61e88c; /* extu.w r12,r12 */
L8c61e88c: r[13]<<=2;goto L8c61e88e; /* shll2 r13 */
L8c61e88e: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61e890; /* shll r11 */
L8c61e890: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61e892; /* shll r14 */
L8c61e892: r[11]+=r[5];goto L8c61e894; /* add r5,r11 */
L8c61e894: r[12]<<=2;goto L8c61e896; /* shll2 r12 */
L8c61e896: r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61e898; /* mov.l @(0xc,r11),r4 */
L8c61e898: r[14]+=r[5];goto L8c61e89a; /* add r5,r14 */
L8c61e89a: r[13]<<=2;goto L8c61e89c; /* shll2 r13 */
L8c61e89c: r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61e89e; /* mov.l @(0xc,r14),r2 */
L8c61e89e: r[12]<<=2;goto L8c61e8a0; /* shll2 r12 */
L8c61e8a0: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61e8a2; /* shll r13 */
L8c61e8a2: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61e8a4; /* shll r12 */
L8c61e8a4: r[13]+=r[5];goto L8c61e8a6; /* add r5,r13 */
L8c61e8a6: r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61e8a8; /* mov.l @(0xc,r13),r1 */
L8c61e8a8: r[4]&=r[2];goto L8c61e8aa; /* and r2,r4 */
L8c61e8aa: r[12]+=r[5];goto L8c61e8ac; /* add r5,r12 */
L8c61e8ac: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61e8ae; /* mov.l @(0xc,r12),r2 */
L8c61e8ae: r[4]&=r[1];goto L8c61e8b0; /* and r1,r4 */
L8c61e8b0: r[1]=63u;goto L8c61e8b2; /* mov #0x3f,r1 */
L8c61e8b2: r[4]&=r[2];goto L8c61e8b4; /* and r2,r4 */
L8c61e8b4: *sr=(*sr&~1u)|!(r[1]&r[4]);goto L8c61e8b6; /* tst r1,r4 */
L8c61e8b6: if((*sr&1u))goto L8c61e8bc;goto L8c61e8b8; /* bt 0x8c61e8bc */
L8c61e8b8: goto L8c61ebf4; /* bra 0x8c61ebf4 */
L8c61e8bc: r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61e8be; /* mov.l @(0xc,r11),r4 */
L8c61e8be: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61e8c0; /* mov.l @(0xc,r14),r3 */
L8c61e8c0: r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61e8c2; /* mov.l @(0xc,r13),r2 */
L8c61e8c2: r[4]|=r[3];goto L8c61e8c4; /* or r3,r4 */
L8c61e8c4: r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61e8c6; /* mov.l @(0xc,r12),r3 */
L8c61e8c6: r[4]|=r[2];goto L8c61e8c8; /* or r2,r4 */
L8c61e8c8: r[2]=1u;goto L8c61e8ca; /* mov #0x1,r2 */
L8c61e8ca: r[4]|=r[3];goto L8c61e8cc; /* or r3,r4 */
L8c61e8cc: *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61e8ce; /* tst r2,r4 */
L8c61e8ce: if((*sr&1u))goto L8c61e8d4;goto L8c61e8d0; /* bt 0x8c61e8d4 */
L8c61e8d0: goto L8c61eb58; /* bra 0x8c61eb58 */
L8c61e8d4: r[0]=16u;goto L8c61e8d6; /* mov #0x10,r0 */
L8c61e8d6: fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e8d8; /* fmov.s @(r0,r14),fr5 */
L8c61e8d8: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e8da; /* fmov.s @(r0,r11),fr3 */
L8c61e8da: r[0]=20u;goto L8c61e8dc; /* mov #0x14,r0 */
L8c61e8dc: fcopy(f,2u,5u);goto L8c61e8de; /* fmov fr5,fr2 */
L8c61e8de: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61e8e0; /* fsub fr3,fr2 */
L8c61e8e0: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e8e2; /* fmov.s @(r0,r13),fr3 */
L8c61e8e2: fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e8e4; /* fmov.s @(r0,r14),fr4 */
L8c61e8e4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e8e6; /* fsub fr4,fr3 */
L8c61e8e6: fcopy(f,1u,4u);goto L8c61e8e8; /* fmov fr4,fr1 */
L8c61e8e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e8ea; /* fmul fr3,fr2 */
L8c61e8ea: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e8ec; /* fmov.s @(r0,r11),fr3 */
L8c61e8ec: r[0]=16u;goto L8c61e8ee; /* mov #0x10,r0 */
L8c61e8ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61e8f0; /* fsub fr3,fr1 */
L8c61e8f0: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e8f2; /* fmov.s @(r0,r13),fr3 */
L8c61e8f2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e8f4; /* fsub fr5,fr3 */
L8c61e8f4: fcopy(f,4u,2u);goto L8c61e8f6; /* fmov fr2,fr4 */
L8c61e8f6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e8f8; /* fmul fr3,fr1 */
L8c61e8f8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61e8fa; /* fldi0 fr3 */
L8c61e8fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61e8fc; /* fsub fr1,fr4 */
L8c61e8fc: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61e8fe; /* fcmp/gt fr3,fr4 */
L8c61e8fe: if(!(*sr&1u))goto L8c61e904;goto L8c61e900; /* bf 0x8c61e904 */
L8c61e900: goto L8c61ebf4; /* bra 0x8c61ebf4 */
L8c61e904: fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c61e906; /* fmov.s @r11,fr2 */
L8c61e906: r[0]=12u;goto L8c61e908; /* mov #0xc,r0 */
L8c61e908: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61e90a; /* fmov.s @r14,fr3 */
L8c61e90a: r[4]=r[15];goto L8c61e90c; /* mov r15,r4 */
L8c61e90c: r[3]=rd(m,bus,&io_status,0x8c61eb38u,4);if(io_status)return io_status;goto L8c61e90e; /* mov.l 0x8c61eb38,r3 */
L8c61e90e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e910; /* fadd fr3,fr2 */
L8c61e910: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61e912; /* fmov.s @r13,fr3 */
L8c61e912: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e914; /* fadd fr3,fr2 */
L8c61e914: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e916; /* fmov fr2,@(r0,r15) */
L8c61e916: r[0]=4u;goto L8c61e918; /* mov #0x4,r0 */
L8c61e918: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e91a; /* fmov.s @(r0,r11),fr2 */
L8c61e91a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e91c; /* fmov.s @(r0,r14),fr3 */
L8c61e91c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e91e; /* fadd fr3,fr2 */
L8c61e91e: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e920; /* fmov.s @(r0,r13),fr3 */
L8c61e920: r[0]=16u;goto L8c61e922; /* mov #0x10,r0 */
L8c61e922: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e924; /* fadd fr3,fr2 */
L8c61e924: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e926; /* fmov fr2,@(r0,r15) */
L8c61e926: r[0]=8u;goto L8c61e928; /* mov #0x8,r0 */
L8c61e928: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e92a; /* fmov.s @(r0,r11),fr2 */
L8c61e92a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e92c; /* fmov.s @(r0,r14),fr3 */
L8c61e92c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e92e; /* fadd fr3,fr2 */
L8c61e92e: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e930; /* fmov.s @(r0,r13),fr3 */
L8c61e930: r[0]=20u;goto L8c61e932; /* mov #0x14,r0 */
L8c61e932: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e934; /* fadd fr3,fr2 */
L8c61e934: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e936; /* fmov fr2,@(r0,r15) */
L8c61e936: target=r[3];*pr=0x8c61e93au;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61e93a; /* jsr @r3 */
L8c61e93a: r[3]=rd(m,bus,&io_status,0x8c61eb3cu,4);if(io_status)return io_status;goto L8c61e93c; /* mov.l 0x8c61eb3c,r3 */
L8c61e93c: r[0]=12u;goto L8c61e93e; /* mov #0xc,r0 */
L8c61e93e: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e940; /* fmov.s @(r0,r15),fr3 */
L8c61e940: r[0]=16u;goto L8c61e942; /* mov #0x10,r0 */
L8c61e942: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61e944; /* fmov.s @r3,fr2 */
L8c61e944: r[3]+=8u;goto L8c61e946; /* add #0x8,r3 */
L8c61e946: r[2]=rd(m,bus,&io_status,0x8c61eb40u,4);if(io_status)return io_status;goto L8c61e948; /* mov.l 0x8c61eb40,r2 */
L8c61e948: r[8]+=24u;goto L8c61e94a; /* add #0x18,r8 */
L8c61e94a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e94c; /* fmul fr3,fr2 */
L8c61e94c: r[1]=rd(m,bus,&io_status,0x8c61eb44u,4);if(io_status)return io_status;goto L8c61e94e; /* mov.l 0x8c61eb44,r1 */
L8c61e94e: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61e950; /* fmov.s @r2,fr3 */
L8c61e950: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61e952; /* fmov.s @r1,fr1 */
L8c61e952: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e954; /* fsub fr2,fr3 */
L8c61e954: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e956; /* fmov.s @(r0,r15),fr2 */
L8c61e956: r[0]=20u;goto L8c61e958; /* mov #0x14,r0 */
L8c61e958: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e95a; /* fmul fr2,fr1 */
L8c61e95a: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e95c; /* fmov.s @(r0,r15),fr2 */
L8c61e95c: r[0]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61e95e; /* mov.l 0x8c61eb48,r0 */
L8c61e95e: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61e960; /* mov.l @r0,r2 */
L8c61e960: r[0]=4u;goto L8c61e962; /* mov #0x4,r0 */
L8c61e962: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e964; /* fsub fr1,fr3 */
L8c61e964: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61e966; /* fmov.s @r3,fr1 */
L8c61e966: fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e968; /* fmov.s @(r0,r11),fr0 */
L8c61e968: r[1]=r[2];goto L8c61e96a; /* mov r2,r1 */
L8c61e96a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e96c; /* fmul fr2,fr1 */
L8c61e96c: r[0]=20u;goto L8c61e96e; /* mov #0x14,r0 */
L8c61e96e: r[3]=r[2];goto L8c61e970; /* mov r2,r3 */
L8c61e970: fcopy(f,4u,3u);goto L8c61e972; /* fmov fr3,fr4 */
L8c61e972: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e974; /* fmov.s @(r0,r1),fr3 */
L8c61e974: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61e976; /* fsub fr1,fr4 */
L8c61e976: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e978; /* fmul fr0,fr3 */
L8c61e978: fload(m,bus,&io_status,f,0u,r[11]);if(io_status)return io_status;goto L8c61e97a; /* fmov.s @r11,fr0 */
L8c61e97a: r[0]=4u;goto L8c61e97c; /* mov #0x4,r0 */
L8c61e97c: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e97e; /* fmov.s @(r0,r3),fr2 */
L8c61e97e: r[0]=8u;goto L8c61e980; /* mov #0x8,r0 */
L8c61e980: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e982; /* fmac fr0,fr2,fr3 */
L8c61e982: fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e984; /* fmov.s @(r0,r11),fr0 */
L8c61e984: r[0]=36u;goto L8c61e986; /* mov #0x24,r0 */
L8c61e986: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e988; /* fmov.s @(r0,r2),fr2 */
L8c61e988: r[0]=4u;goto L8c61e98a; /* mov #0x4,r0 */
L8c61e98a: r[2]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61e98c; /* mov.l 0x8c61eb48,r2 */
L8c61e98c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e98e; /* fmac fr0,fr2,fr3 */
L8c61e98e: fcopy(f,0u,14u);goto L8c61e990; /* fmov fr14,fr0 */
L8c61e990: fcopy(f,2u,14u);goto L8c61e992; /* fmov fr14,fr2 */
L8c61e992: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e994; /* fmac fr0,fr3,fr2 */
L8c61e994: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e996; /* fmov.s fr2,@-r8 */
L8c61e996: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e998; /* mov.l @r2,r3 */
L8c61e998: fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e99a; /* fmov.s @(r0,r11),fr0 */
L8c61e99a: r[0]=16u;goto L8c61e99c; /* mov #0x10,r0 */
L8c61e99c: r[2]=r[3];goto L8c61e99e; /* mov r3,r2 */
L8c61e99e: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e9a0; /* fmov.s @(r0,r2),fr3 */
L8c61e9a0: r[1]=r[3];goto L8c61e9a2; /* mov r3,r1 */
L8c61e9a2: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61e9a4; /* fmov.s @r1,fr2 */
L8c61e9a4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e9a6; /* fmul fr0,fr3 */
L8c61e9a6: fload(m,bus,&io_status,f,0u,r[11]);if(io_status)return io_status;goto L8c61e9a8; /* fmov.s @r11,fr0 */
L8c61e9a8: r[0]=8u;goto L8c61e9aa; /* mov #0x8,r0 */
L8c61e9aa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e9ac; /* fmac fr0,fr2,fr3 */
L8c61e9ac: fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e9ae; /* fmov.s @(r0,r11),fr0 */
L8c61e9ae: r[0]=32u;goto L8c61e9b0; /* mov #0x20,r0 */
L8c61e9b0: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e9b2; /* fmov.s @(r0,r3),fr2 */
L8c61e9b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e9b4; /* fmac fr0,fr2,fr3 */
L8c61e9b4: fcopy(f,0u,14u);goto L8c61e9b6; /* fmov fr14,fr0 */
L8c61e9b6: fcopy(f,2u,14u);goto L8c61e9b8; /* fmov fr14,fr2 */
L8c61e9b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e9ba; /* fmac fr0,fr3,fr2 */
L8c61e9ba: r[0]=24u;goto L8c61e9bc; /* mov #0x18,r0 */
L8c61e9bc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e9be; /* fmov.s fr2,@-r8 */
L8c61e9be: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e9c0; /* fmov.s @(r0,r11),fr3 */
L8c61e9c0: r[0]=20u;goto L8c61e9c2; /* mov #0x14,r0 */
L8c61e9c2: r[3]=rd(m,bus,&io_status,0x8c61eb4cu,4);if(io_status)return io_status;goto L8c61e9c4; /* mov.l 0x8c61eb4c,r3 */
L8c61e9c4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e9c6; /* fmov.s fr3,@-r8 */
L8c61e9c6: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e9c8; /* fmov.s @(r0,r11),fr3 */
L8c61e9c8: r[0]=16u;goto L8c61e9ca; /* mov #0x10,r0 */
L8c61e9ca: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e9cc; /* fmov.s fr3,@-r8 */
L8c61e9cc: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e9ce; /* fmov.s @(r0,r11),fr3 */
L8c61e9ce: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e9d0; /* fmov.s fr3,@-r8 */
L8c61e9d0: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61e9d2; /* mov.l r3,@-r8 */
L8c61e9d2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e9d4; /* pref @r8 */
L8c61e9d4: r[2]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61e9d6; /* mov.l 0x8c61eb48,r2 */
L8c61e9d6: r[0]=4u;goto L8c61e9d8; /* mov #0x4,r0 */
L8c61e9d8: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e9da; /* fmov.s @(r0,r14),fr0 */
L8c61e9da: r[0]=20u;goto L8c61e9dc; /* mov #0x14,r0 */
L8c61e9dc: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e9de; /* mov.l @r2,r3 */
L8c61e9de: r[8]+=56u;goto L8c61e9e0; /* add #0x38,r8 */
L8c61e9e0: r[2]=r[3];goto L8c61e9e2; /* mov r3,r2 */
L8c61e9e2: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e9e4; /* fmov.s @(r0,r2),fr3 */
L8c61e9e4: r[0]=4u;goto L8c61e9e6; /* mov #0x4,r0 */
L8c61e9e6: r[1]=r[3];goto L8c61e9e8; /* mov r3,r1 */
L8c61e9e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e9ea; /* fmul fr0,fr3 */
L8c61e9ea: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e9ec; /* fmov.s @(r0,r1),fr2 */
L8c61e9ec: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61e9ee; /* fmov.s @r14,fr0 */
L8c61e9ee: r[0]=8u;goto L8c61e9f0; /* mov #0x8,r0 */
L8c61e9f0: r[2]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61e9f2; /* mov.l 0x8c61eb48,r2 */
L8c61e9f2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e9f4; /* fmac fr0,fr2,fr3 */
L8c61e9f4: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e9f6; /* fmov.s @(r0,r14),fr0 */
L8c61e9f6: r[0]=36u;goto L8c61e9f8; /* mov #0x24,r0 */
L8c61e9f8: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e9fa; /* fmov.s @(r0,r3),fr2 */
L8c61e9fa: r[0]=4u;goto L8c61e9fc; /* mov #0x4,r0 */
L8c61e9fc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e9fe; /* fmac fr0,fr2,fr3 */
L8c61e9fe: fcopy(f,0u,14u);goto L8c61ea00; /* fmov fr14,fr0 */
L8c61ea00: fcopy(f,2u,14u);goto L8c61ea02; /* fmov fr14,fr2 */
L8c61ea02: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ea04; /* fmac fr0,fr3,fr2 */
L8c61ea04: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ea06; /* fmov.s fr2,@-r8 */
L8c61ea06: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ea08; /* mov.l @r2,r3 */
L8c61ea08: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ea0a; /* fmov.s @(r0,r14),fr0 */
L8c61ea0a: r[0]=16u;goto L8c61ea0c; /* mov #0x10,r0 */
L8c61ea0c: r[2]=r[3];goto L8c61ea0e; /* mov r3,r2 */
L8c61ea0e: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61ea10; /* fmov.s @(r0,r2),fr3 */
L8c61ea10: r[1]=r[3];goto L8c61ea12; /* mov r3,r1 */
L8c61ea12: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea14; /* fmul fr0,fr3 */
L8c61ea14: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61ea16; /* fmov.s @r1,fr2 */
L8c61ea16: r[0]=8u;goto L8c61ea18; /* mov #0x8,r0 */
L8c61ea18: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61ea1a; /* fmov.s @r14,fr0 */
L8c61ea1a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea1c; /* fmac fr0,fr2,fr3 */
L8c61ea1c: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ea1e; /* fmov.s @(r0,r14),fr0 */
L8c61ea1e: r[0]=32u;goto L8c61ea20; /* mov #0x20,r0 */
L8c61ea20: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61ea22; /* fmov.s @(r0,r3),fr2 */
L8c61ea22: r[0]=24u;goto L8c61ea24; /* mov #0x18,r0 */
L8c61ea24: r[3]=rd(m,bus,&io_status,0x8c61eb4cu,4);if(io_status)return io_status;goto L8c61ea26; /* mov.l 0x8c61eb4c,r3 */
L8c61ea26: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea28; /* fmac fr0,fr2,fr3 */
L8c61ea28: fcopy(f,0u,14u);goto L8c61ea2a; /* fmov fr14,fr0 */
L8c61ea2a: fcopy(f,2u,14u);goto L8c61ea2c; /* fmov fr14,fr2 */
L8c61ea2c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ea2e; /* fmac fr0,fr3,fr2 */
L8c61ea2e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ea30; /* fmov.s fr2,@-r8 */
L8c61ea30: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ea32; /* fmov.s @(r0,r14),fr3 */
L8c61ea32: r[0]=20u;goto L8c61ea34; /* mov #0x14,r0 */
L8c61ea34: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ea36; /* fmov.s fr3,@-r8 */
L8c61ea36: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ea38; /* fmov.s @(r0,r14),fr3 */
L8c61ea38: r[0]=16u;goto L8c61ea3a; /* mov #0x10,r0 */
L8c61ea3a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ea3c; /* fmov.s fr3,@-r8 */
L8c61ea3c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ea3e; /* fmov.s @(r0,r14),fr3 */
L8c61ea3e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ea40; /* fmov.s fr3,@-r8 */
L8c61ea40: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61ea42; /* mov.l r3,@-r8 */
L8c61ea42: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61ea44; /* pref @r8 */
L8c61ea44: r[2]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61ea46; /* mov.l 0x8c61eb48,r2 */
L8c61ea46: r[0]=4u;goto L8c61ea48; /* mov #0x4,r0 */
L8c61ea48: r[8]+=60u;goto L8c61ea4a; /* add #0x3c,r8 */
L8c61ea4a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61ea4c; /* fmov.s fr4,@-r8 */
L8c61ea4c: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ea4e; /* mov.l @r2,r3 */
L8c61ea4e: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ea50; /* fmov.s @(r0,r13),fr0 */
L8c61ea50: r[0]=20u;goto L8c61ea52; /* mov #0x14,r0 */
L8c61ea52: r[2]=r[3];goto L8c61ea54; /* mov r3,r2 */
L8c61ea54: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61ea56; /* fmov.s @(r0,r2),fr3 */
L8c61ea56: r[1]=r[3];goto L8c61ea58; /* mov r3,r1 */
L8c61ea58: r[0]=4u;goto L8c61ea5a; /* mov #0x4,r0 */
L8c61ea5a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea5c; /* fmul fr0,fr3 */
L8c61ea5c: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61ea5e; /* fmov.s @(r0,r1),fr2 */
L8c61ea5e: fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61ea60; /* fmov.s @r13,fr0 */
L8c61ea60: r[0]=8u;goto L8c61ea62; /* mov #0x8,r0 */
L8c61ea62: r[2]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61ea64; /* mov.l 0x8c61eb48,r2 */
L8c61ea64: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea66; /* fmac fr0,fr2,fr3 */
L8c61ea66: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ea68; /* fmov.s @(r0,r13),fr0 */
L8c61ea68: r[0]=36u;goto L8c61ea6a; /* mov #0x24,r0 */
L8c61ea6a: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61ea6c; /* fmov.s @(r0,r3),fr2 */
L8c61ea6c: r[0]=4u;goto L8c61ea6e; /* mov #0x4,r0 */
L8c61ea6e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea70; /* fmac fr0,fr2,fr3 */
L8c61ea70: fcopy(f,0u,14u);goto L8c61ea72; /* fmov fr14,fr0 */
L8c61ea72: fcopy(f,2u,14u);goto L8c61ea74; /* fmov fr14,fr2 */
L8c61ea74: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ea76; /* fmac fr0,fr3,fr2 */
L8c61ea76: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ea78; /* fmov.s fr2,@-r8 */
L8c61ea78: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ea7a; /* mov.l @r2,r3 */
L8c61ea7a: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ea7c; /* fmov.s @(r0,r13),fr0 */
L8c61ea7c: r[0]=16u;goto L8c61ea7e; /* mov #0x10,r0 */
L8c61ea7e: r[2]=r[3];goto L8c61ea80; /* mov r3,r2 */
L8c61ea80: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61ea82; /* fmov.s @(r0,r2),fr3 */
L8c61ea82: r[1]=r[3];goto L8c61ea84; /* mov r3,r1 */
L8c61ea84: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea86; /* fmul fr0,fr3 */
L8c61ea86: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61ea88; /* fmov.s @r1,fr2 */
L8c61ea88: fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61ea8a; /* fmov.s @r13,fr0 */
L8c61ea8a: r[0]=8u;goto L8c61ea8c; /* mov #0x8,r0 */
L8c61ea8c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea8e; /* fmac fr0,fr2,fr3 */
L8c61ea8e: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ea90; /* fmov.s @(r0,r13),fr0 */
L8c61ea90: r[0]=32u;goto L8c61ea92; /* mov #0x20,r0 */
L8c61ea92: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61ea94; /* fmov.s @(r0,r3),fr2 */
L8c61ea94: r[0]=24u;goto L8c61ea96; /* mov #0x18,r0 */
L8c61ea96: r[3]=rd(m,bus,&io_status,0x8c61eb4cu,4);if(io_status)return io_status;goto L8c61ea98; /* mov.l 0x8c61eb4c,r3 */
L8c61ea98: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea9a; /* fmac fr0,fr2,fr3 */
L8c61ea9a: fcopy(f,0u,14u);goto L8c61ea9c; /* fmov fr14,fr0 */
L8c61ea9c: fcopy(f,2u,14u);goto L8c61ea9e; /* fmov fr14,fr2 */
L8c61ea9e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61eaa0; /* fmac fr0,fr3,fr2 */
L8c61eaa0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61eaa2; /* fmov.s fr2,@-r8 */
L8c61eaa2: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61eaa4; /* fmov.s @(r0,r13),fr3 */
L8c61eaa4: r[0]=20u;goto L8c61eaa6; /* mov #0x14,r0 */
L8c61eaa6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eaa8; /* fmov.s fr3,@-r8 */
L8c61eaa8: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61eaaa; /* fmov.s @(r0,r13),fr3 */
L8c61eaaa: r[0]=16u;goto L8c61eaac; /* mov #0x10,r0 */
L8c61eaac: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eaae; /* fmov.s fr3,@-r8 */
L8c61eaae: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61eab0; /* fmov.s @(r0,r13),fr3 */
L8c61eab0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eab2; /* fmov.s fr3,@-r8 */
L8c61eab2: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61eab4; /* mov.l r3,@-r8 */
L8c61eab4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61eab6; /* pref @r8 */
L8c61eab6: r[2]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61eab8; /* mov.l 0x8c61eb48,r2 */
L8c61eab8: r[0]=4u;goto L8c61eaba; /* mov #0x4,r0 */
L8c61eaba: r[8]+=60u;goto L8c61eabc; /* add #0x3c,r8 */
L8c61eabc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61eabe; /* fmov.s fr4,@-r8 */
L8c61eabe: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61eac0; /* mov.l @r2,r3 */
L8c61eac0: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61eac2; /* fmov.s @(r0,r12),fr0 */
L8c61eac2: r[0]=20u;goto L8c61eac4; /* mov #0x14,r0 */
L8c61eac4: r[2]=r[3];goto L8c61eac6; /* mov r3,r2 */
L8c61eac6: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61eac8; /* fmov.s @(r0,r2),fr3 */
L8c61eac8: r[1]=r[3];goto L8c61eaca; /* mov r3,r1 */
L8c61eaca: r[0]=4u;goto L8c61eacc; /* mov #0x4,r0 */
L8c61eacc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eace; /* fmul fr0,fr3 */
L8c61eace: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61ead0; /* fmov.s @(r0,r1),fr2 */
L8c61ead0: fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61ead2; /* fmov.s @r12,fr0 */
L8c61ead2: r[0]=8u;goto L8c61ead4; /* mov #0x8,r0 */
L8c61ead4: r[2]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61ead6; /* mov.l 0x8c61eb48,r2 */
L8c61ead6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ead8; /* fmac fr0,fr2,fr3 */
L8c61ead8: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61eada; /* fmov.s @(r0,r12),fr0 */
L8c61eada: r[0]=36u;goto L8c61eadc; /* mov #0x24,r0 */
L8c61eadc: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61eade; /* fmov.s @(r0,r3),fr2 */
L8c61eade: r[0]=4u;goto L8c61eae0; /* mov #0x4,r0 */
L8c61eae0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eae2; /* fmac fr0,fr2,fr3 */
L8c61eae2: fcopy(f,0u,14u);goto L8c61eae4; /* fmov fr14,fr0 */
L8c61eae4: fcopy(f,2u,14u);goto L8c61eae6; /* fmov fr14,fr2 */
L8c61eae6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61eae8; /* fmac fr0,fr3,fr2 */
L8c61eae8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61eaea; /* fmov.s fr2,@-r8 */
L8c61eaea: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61eaec; /* mov.l @r2,r3 */
L8c61eaec: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61eaee; /* fmov.s @(r0,r12),fr0 */
L8c61eaee: r[0]=16u;goto L8c61eaf0; /* mov #0x10,r0 */
L8c61eaf0: r[2]=r[3];goto L8c61eaf2; /* mov r3,r2 */
L8c61eaf2: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61eaf4; /* fmov.s @(r0,r2),fr3 */
L8c61eaf4: r[1]=r[3];goto L8c61eaf6; /* mov r3,r1 */
L8c61eaf6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eaf8; /* fmul fr0,fr3 */
L8c61eaf8: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61eafa; /* fmov.s @r1,fr2 */
L8c61eafa: fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61eafc; /* fmov.s @r12,fr0 */
L8c61eafc: r[0]=8u;goto L8c61eafe; /* mov #0x8,r0 */
L8c61eafe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eb00; /* fmac fr0,fr2,fr3 */
L8c61eb00: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61eb02; /* fmov.s @(r0,r12),fr0 */
L8c61eb02: r[0]=32u;goto L8c61eb04; /* mov #0x20,r0 */
L8c61eb04: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61eb06; /* fmov.s @(r0,r3),fr2 */
L8c61eb06: r[0]=24u;goto L8c61eb08; /* mov #0x18,r0 */
L8c61eb08: r[3]=rd(m,bus,&io_status,0x8c61eb50u,4);if(io_status)return io_status;goto L8c61eb0a; /* mov.l 0x8c61eb50,r3 */
L8c61eb0a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eb0c; /* fmac fr0,fr2,fr3 */
L8c61eb0c: fcopy(f,0u,14u);goto L8c61eb0e; /* fmov fr14,fr0 */
L8c61eb0e: fcopy(f,2u,14u);goto L8c61eb10; /* fmov fr14,fr2 */
L8c61eb10: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61eb12; /* fmac fr0,fr3,fr2 */
L8c61eb12: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61eb14; /* fmov.s fr2,@-r8 */
L8c61eb14: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61eb16; /* fmov.s @(r0,r12),fr3 */
L8c61eb16: r[0]=20u;goto L8c61eb18; /* mov #0x14,r0 */
L8c61eb18: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eb1a; /* fmov.s fr3,@-r8 */
L8c61eb1a: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61eb1c; /* fmov.s @(r0,r12),fr3 */
L8c61eb1c: r[0]=16u;goto L8c61eb1e; /* mov #0x10,r0 */
L8c61eb1e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eb20; /* fmov.s fr3,@-r8 */
L8c61eb20: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61eb22; /* fmov.s @(r0,r12),fr3 */
L8c61eb22: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eb24; /* fmov.s fr3,@-r8 */
L8c61eb24: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61eb26; /* mov.l r3,@-r8 */
L8c61eb26: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61eb28; /* pref @r8 */
L8c61eb28: r[2]=rd(m,bus,&io_status,0x8c61eb54u,4);if(io_status)return io_status;goto L8c61eb2a; /* mov.l 0x8c61eb54,r2 */
L8c61eb2a: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61eb2c; /* mov.l @r2,r3 */
L8c61eb2c: r[3]+=4u;goto L8c61eb2e; /* add #0x4,r3 */
L8c61eb2e: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61eb30; /* mov.l r3,@r2 */
L8c61eb30: r[8]+=32u;goto L8c61ebf4; /* bra 0x8c61ebf4 */
L8c61eb58: r[0]=12u;goto L8c61eb5a; /* mov #0xc,r0 */
L8c61eb5a: r[1]=rd(m,bus,&io_status,0x8c61ecb0u,4);if(io_status)return io_status;goto L8c61eb5c; /* mov.l 0x8c61ecb0,r1 */
L8c61eb5c: r[3]=rd(m,bus,&io_status,0x8c61ecb4u,4);if(io_status)return io_status;goto L8c61eb5e; /* mov.l 0x8c61ecb4,r3 */
L8c61eb5e: r[4]=r[15];goto L8c61eb60; /* mov r15,r4 */
L8c61eb60: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61eb62; /* mov.l r2,@r1 */
L8c61eb62: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61eb64; /* fmov.s @r14,fr3 */
L8c61eb64: fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c61eb66; /* fmov.s @r11,fr2 */
L8c61eb66: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61eb68; /* fadd fr3,fr2 */
L8c61eb68: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61eb6a; /* fmov.s @r13,fr3 */
L8c61eb6a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61eb6c; /* fadd fr3,fr2 */
L8c61eb6c: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61eb6e; /* fmov fr2,@(r0,r15) */
L8c61eb6e: r[0]=4u;goto L8c61eb70; /* mov #0x4,r0 */
L8c61eb70: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61eb72; /* fmov.s @(r0,r14),fr3 */
L8c61eb72: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61eb74; /* fmov.s @(r0,r11),fr2 */
L8c61eb74: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61eb76; /* fadd fr3,fr2 */
L8c61eb76: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61eb78; /* fmov.s @(r0,r13),fr3 */
L8c61eb78: r[0]=16u;goto L8c61eb7a; /* mov #0x10,r0 */
L8c61eb7a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61eb7c; /* fadd fr3,fr2 */
L8c61eb7c: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61eb7e; /* fmov fr2,@(r0,r15) */
L8c61eb7e: r[0]=8u;goto L8c61eb80; /* mov #0x8,r0 */
L8c61eb80: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61eb82; /* fmov.s @(r0,r11),fr2 */
L8c61eb82: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61eb84; /* fmov.s @(r0,r14),fr3 */
L8c61eb84: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61eb86; /* fadd fr3,fr2 */
L8c61eb86: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61eb88; /* fmov.s @(r0,r13),fr3 */
L8c61eb88: r[0]=20u;goto L8c61eb8a; /* mov #0x14,r0 */
L8c61eb8a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61eb8c; /* fadd fr3,fr2 */
L8c61eb8c: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61eb8e; /* fmov fr2,@(r0,r15) */
L8c61eb8e: target=r[3];*pr=0x8c61eb92u;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61eb92; /* jsr @r3 */
L8c61eb92: r[3]=rd(m,bus,&io_status,0x8c61ecb8u,4);if(io_status)return io_status;goto L8c61eb94; /* mov.l 0x8c61ecb8,r3 */
L8c61eb94: r[0]=12u;goto L8c61eb96; /* mov #0xc,r0 */
L8c61eb96: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61eb98; /* fmov.s @(r0,r15),fr3 */
L8c61eb98: r[0]=16u;goto L8c61eb9a; /* mov #0x10,r0 */
L8c61eb9a: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61eb9c; /* fmov.s @r3,fr2 */
L8c61eb9c: r[3]+=8u;goto L8c61eb9e; /* add #0x8,r3 */
L8c61eb9e: r[2]=rd(m,bus,&io_status,0x8c61ecbcu,4);if(io_status)return io_status;goto L8c61eba0; /* mov.l 0x8c61ecbc,r2 */
L8c61eba0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61eba2; /* fmul fr3,fr2 */
L8c61eba2: r[1]=rd(m,bus,&io_status,0x8c61ecc0u,4);if(io_status)return io_status;goto L8c61eba4; /* mov.l 0x8c61ecc0,r1 */
L8c61eba4: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61eba6; /* fmov.s @r2,fr3 */
L8c61eba6: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61eba8; /* fmov.s @r1,fr1 */
L8c61eba8: r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61ebaa; /* mov.l @(0xc,r14),r2 */
L8c61ebaa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ebac; /* fsub fr2,fr3 */
L8c61ebac: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ebae; /* fmov.s @(r0,r15),fr2 */
L8c61ebae: r[0]=20u;goto L8c61ebb0; /* mov #0x14,r0 */
L8c61ebb0: r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61ebb2; /* mov.l @(0xc,r11),r4 */
L8c61ebb2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ebb4; /* fmul fr2,fr1 */
L8c61ebb4: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ebb6; /* fmov.s @(r0,r15),fr2 */
L8c61ebb6: r[0]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61ebb8; /* mov.l @(0xc,r13),r0 */
L8c61ebb8: r[4]&=r[2];goto L8c61ebba; /* and r2,r4 */
L8c61ebba: r[2]=1u;goto L8c61ebbc; /* mov #0x1,r2 */
L8c61ebbc: r[4]&=r[0];goto L8c61ebbe; /* and r0,r4 */
L8c61ebbe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ebc0; /* fsub fr1,fr3 */
L8c61ebc0: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61ebc2; /* fmov.s @r3,fr1 */
L8c61ebc2: *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61ebc4; /* tst r2,r4 */
L8c61ebc4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ebc6; /* fmul fr2,fr1 */
L8c61ebc6: fcopy(f,15u,3u);goto L8c61ebc8; /* fmov fr3,fr15 */
L8c61ebc8: take=!(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[1],&f->fpscr,&fr[15],1);if(status)return status;}if(take)goto L8c61ebd8;goto L8c61ebcc; /* bf/s 0x8c61ebd8 */
L8c61ebcc: r[3]=rd(m,bus,&io_status,0x8c61ecc4u,4);if(io_status)return io_status;goto L8c61ebce; /* mov.l 0x8c61ecc4,r3 */
L8c61ebce: r[5]=r[14];goto L8c61ebd0; /* mov r14,r5 */
L8c61ebd0: fcopy(f,4u,15u);goto L8c61ebd2; /* fmov fr15,fr4 */
L8c61ebd2: r[6]=r[13];goto L8c61ebd4; /* mov r13,r6 */
L8c61ebd4: target=r[3];*pr=0x8c61ebd8u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ebd8; /* jsr @r3 */
L8c61ebd8: r[4]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61ebda; /* mov.l @(0xc,r14),r4 */
L8c61ebda: r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61ebdc; /* mov.l @(0xc,r13),r3 */
L8c61ebdc: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61ebde; /* mov.l @(0xc,r12),r2 */
L8c61ebde: r[4]&=r[3];goto L8c61ebe0; /* and r3,r4 */
L8c61ebe0: r[3]=1u;goto L8c61ebe2; /* mov #0x1,r3 */
L8c61ebe2: r[4]&=r[2];goto L8c61ebe4; /* and r2,r4 */
L8c61ebe4: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61ebe6; /* tst r3,r4 */
L8c61ebe6: if(!(*sr&1u))goto L8c61ebf4;goto L8c61ebe8; /* bf 0x8c61ebf4 */
L8c61ebe8: r[1]=rd(m,bus,&io_status,0x8c61ecc4u,4);if(io_status)return io_status;goto L8c61ebea; /* mov.l 0x8c61ecc4,r1 */
L8c61ebea: r[5]=r[13];goto L8c61ebec; /* mov r13,r5 */
L8c61ebec: fcopy(f,4u,15u);goto L8c61ebee; /* fmov fr15,fr4 */
L8c61ebee: r[6]=r[12];goto L8c61ebf0; /* mov r12,r6 */
L8c61ebf0: target=r[1];*pr=0x8c61ebf4u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ebf4; /* jsr @r1 */
L8c61ebf4: r[9]+=1u;goto L8c61ebf6; /* add #0x1,r9 */
L8c61ebf6: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ebf8; /* mov.l @r15,r3 */
L8c61ebf8: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61ebfa; /* mov.w @(0x2,r3),r0 */
L8c61ebfa: r[0]=r[0]&65535u;goto L8c61ebfc; /* extu.w r0,r0 */
L8c61ebfc: *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61ebfe; /* cmp/ge r0,r9 */
L8c61ebfe: if((*sr&1u))goto L8c61ec04;goto L8c61ec00; /* bt 0x8c61ec04 */
L8c61ec00: goto L8c61e870; /* bra 0x8c61e870 */
L8c61ec04: goto L8c61f0bc; /* bra 0x8c61f0bc */
L8c61ec08: r[1]=0u;goto L8c61ec0a; /* mov #0x0,r1 */
L8c61ec0a: wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c61f02e; /* bra 0x8c61f02e */
L8c61ec0e: r[3]=rd(m,bus,&io_status,0x8c61ecc8u,4);if(io_status)return io_status;goto L8c61ec10; /* mov.l 0x8c61ecc8,r3 */
L8c61ec10: r[7]=0u;goto L8c61ec12; /* mov #0x0,r7 */
L8c61ec12: r[9]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61ec14; /* mov.w @r10+,r9 */
L8c61ec14: r[4]=0u;goto L8c61ec16; /* mov #0x0,r4 */
L8c61ec16: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ec18; /* mov.l @r3,r2 */
L8c61ec18: r[5]=r[7];goto L8c61ec1a; /* mov r7,r5 */
L8c61ec1a: r[9]=r[9]&65535u;goto L8c61ec1c; /* extu.w r9,r9 */
L8c61ec1c: r[2]+=r[9];goto L8c61ec1e; /* add r9,r2 */
L8c61ec1e: *sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c61ec20; /* cmp/pl r9 */
L8c61ec20: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61ec22; /* mov.l r2,@r3 */
L8c61ec22: r[6]=4294967295u;goto L8c61ec24; /* mov #-0x1,r6 */
L8c61ec24: take=!(*sr&1u);r[4]+=r[10];if(take)goto L8c61ec46;goto L8c61ec28; /* bf/s 0x8c61ec46 */
L8c61ec28: r[11]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c61ec2a; /* mov.w @r4+,r11 */
L8c61ec2a: r[5]+=1u;goto L8c61ec2c; /* add #0x1,r5 */
L8c61ec2c: r[2]=rd(m,bus,&io_status,0x8c61ecccu,4);if(io_status)return io_status;goto L8c61ec2e; /* mov.l 0x8c61eccc,r2 */
L8c61ec2e: r[11]=r[11]&65535u;goto L8c61ec30; /* extu.w r11,r11 */
L8c61ec30: r[11]<<=2;goto L8c61ec32; /* shll2 r11 */
L8c61ec32: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ec34; /* mov.l @r2,r3 */
L8c61ec34: r[11]<<=2;goto L8c61ec36; /* shll2 r11 */
L8c61ec36: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61ec38; /* shll r11 */
L8c61ec38: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[9]);goto L8c61ec3a; /* cmp/ge r9,r5 */
L8c61ec3a: r[11]+=r[3];goto L8c61ec3c; /* add r3,r11 */
L8c61ec3c: wr(m,bus,&io_status,r[15]+0x8u,r[11],4);if(io_status)return io_status;goto L8c61ec3e; /* mov.l r11,@(0x8,r15) */
L8c61ec3e: r[11]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61ec40; /* mov.l @(0xc,r11),r11 */
L8c61ec40: r[6]&=r[11];goto L8c61ec42; /* and r11,r6 */
L8c61ec42: take=!(*sr&1u);r[7]|=r[11];if(take)goto L8c61ec28;goto L8c61ec46; /* bf/s 0x8c61ec28 */
L8c61ec46: r[2]=63u;goto L8c61ec48; /* mov #0x3f,r2 */
L8c61ec48: *sr=(*sr&~1u)|!(r[2]&r[6]);goto L8c61ec4a; /* tst r2,r6 */
L8c61ec4a: if(!(*sr&1u))goto L8c61ecaa;goto L8c61ec4c; /* bf 0x8c61ecaa */
L8c61ec4c: r[1]=1u;goto L8c61ec4e; /* mov #0x1,r1 */
L8c61ec4e: *sr=(*sr&~1u)|!(r[1]&r[7]);goto L8c61ec50; /* tst r1,r7 */
L8c61ec50: if((*sr&1u))goto L8c61ec56;goto L8c61ec52; /* bt 0x8c61ec56 */
L8c61ec52: goto L8c61ef68; /* bra 0x8c61ef68 */
L8c61ec56: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ec58; /* mov.w @(0x2,r10),r0 */
L8c61ec58: r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ec5a; /* mov.w @r10,r12 */
L8c61ec5a: r[3]=rd(m,bus,&io_status,0x8c61ecccu,4);if(io_status)return io_status;goto L8c61ec5c; /* mov.l 0x8c61eccc,r3 */
L8c61ec5c: r[14]=r[0]&65535u;goto L8c61ec5e; /* extu.w r0,r14 */
L8c61ec5e: r[14]<<=2;goto L8c61ec60; /* shll2 r14 */
L8c61ec60: r[0]=sx(m,bus,&io_status,r[10]+0x4u,2);if(io_status)return io_status;goto L8c61ec62; /* mov.w @(0x4,r10),r0 */
L8c61ec62: r[12]=r[12]&65535u;goto L8c61ec64; /* extu.w r12,r12 */
L8c61ec64: r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ec66; /* mov.l @r3,r4 */
L8c61ec66: r[12]<<=2;goto L8c61ec68; /* shll2 r12 */
L8c61ec68: r[14]<<=2;goto L8c61ec6a; /* shll2 r14 */
L8c61ec6a: r[13]=r[0]&65535u;goto L8c61ec6c; /* extu.w r0,r13 */
L8c61ec6c: r[12]<<=2;goto L8c61ec6e; /* shll2 r12 */
L8c61ec6e: r[0]=16u;goto L8c61ec70; /* mov #0x10,r0 */
L8c61ec70: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61ec72; /* shll r12 */
L8c61ec72: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61ec74; /* shll r14 */
L8c61ec74: r[14]+=r[4];goto L8c61ec76; /* add r4,r14 */
L8c61ec76: fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ec78; /* fmov.s @(r0,r14),fr5 */
L8c61ec78: r[13]<<=2;goto L8c61ec7a; /* shll2 r13 */
L8c61ec7a: r[12]+=r[4];goto L8c61ec7c; /* add r4,r12 */
L8c61ec7c: r[13]<<=2;goto L8c61ec7e; /* shll2 r13 */
L8c61ec7e: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61ec80; /* fmov.s @(r0,r12),fr3 */
L8c61ec80: fcopy(f,2u,5u);goto L8c61ec82; /* fmov fr5,fr2 */
L8c61ec82: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61ec84; /* shll r13 */
L8c61ec84: r[0]=20u;goto L8c61ec86; /* mov #0x14,r0 */
L8c61ec86: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61ec88; /* fsub fr3,fr2 */
L8c61ec88: r[13]+=r[4];goto L8c61ec8a; /* add r4,r13 */
L8c61ec8a: fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ec8c; /* fmov.s @(r0,r14),fr4 */
L8c61ec8c: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ec8e; /* fmov.s @(r0,r13),fr3 */
L8c61ec8e: fcopy(f,1u,4u);goto L8c61ec90; /* fmov fr4,fr1 */
L8c61ec90: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ec92; /* fsub fr4,fr3 */
L8c61ec92: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ec94; /* fmul fr3,fr2 */
L8c61ec94: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61ec96; /* fmov.s @(r0,r12),fr3 */
L8c61ec96: r[0]=16u;goto L8c61ec98; /* mov #0x10,r0 */
L8c61ec98: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61ec9a; /* fsub fr3,fr1 */
L8c61ec9a: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ec9c; /* fmov.s @(r0,r13),fr3 */
L8c61ec9c: fcopy(f,4u,2u);goto L8c61ec9e; /* fmov fr2,fr4 */
L8c61ec9e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61eca0; /* fsub fr5,fr3 */
L8c61eca0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61eca2; /* fmul fr3,fr1 */
L8c61eca2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61eca4; /* fldi0 fr3 */
L8c61eca4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61eca6; /* fsub fr1,fr4 */
L8c61eca6: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61eca8; /* fcmp/gt fr3,fr4 */
L8c61eca8: if(!(*sr&1u))goto L8c61ecd0;goto L8c61ecaa; /* bf 0x8c61ecd0 */
L8c61ecaa: *sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c61ecac; /* shll r9 */
L8c61ecac: r[10]+=r[9];goto L8c61f028; /* bra 0x8c61f028 */
L8c61ecd0: r[0]=12u;goto L8c61ecd2; /* mov #0xc,r0 */
L8c61ecd2: r[2]=rd(m,bus,&io_status,0x8c61ef44u,4);if(io_status)return io_status;goto L8c61ecd4; /* mov.l 0x8c61ef44,r2 */
L8c61ecd4: r[4]=r[15];goto L8c61ecd6; /* mov r15,r4 */
L8c61ecd6: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ecd8; /* mov.l @r2,r3 */
L8c61ecd8: r[3]+=r[9];goto L8c61ecda; /* add r9,r3 */
L8c61ecda: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61ecdc; /* mov.l r3,@r2 */
L8c61ecdc: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61ecde; /* fmov.s @r14,fr3 */
L8c61ecde: fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61ece0; /* fmov.s @r12,fr2 */
L8c61ece0: r[3]=rd(m,bus,&io_status,0x8c61ef48u,4);if(io_status)return io_status;goto L8c61ece2; /* mov.l 0x8c61ef48,r3 */
L8c61ece2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ece4; /* fadd fr3,fr2 */
L8c61ece4: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61ece6; /* fmov.s @r13,fr3 */
L8c61ece6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ece8; /* fadd fr3,fr2 */
L8c61ece8: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ecea; /* fmov fr2,@(r0,r15) */
L8c61ecea: r[0]=4u;goto L8c61ecec; /* mov #0x4,r0 */
L8c61ecec: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61ecee; /* fmov.s @(r0,r12),fr2 */
L8c61ecee: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ecf0; /* fmov.s @(r0,r14),fr3 */
L8c61ecf0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ecf2; /* fadd fr3,fr2 */
L8c61ecf2: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ecf4; /* fmov.s @(r0,r13),fr3 */
L8c61ecf4: r[0]=16u;goto L8c61ecf6; /* mov #0x10,r0 */
L8c61ecf6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ecf8; /* fadd fr3,fr2 */
L8c61ecf8: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ecfa; /* fmov fr2,@(r0,r15) */
L8c61ecfa: r[0]=8u;goto L8c61ecfc; /* mov #0x8,r0 */
L8c61ecfc: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ecfe; /* fmov.s @(r0,r14),fr3 */
L8c61ecfe: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61ed00; /* fmov.s @(r0,r12),fr2 */
L8c61ed00: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ed02; /* fadd fr3,fr2 */
L8c61ed02: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ed04; /* fmov.s @(r0,r13),fr3 */
L8c61ed04: r[0]=20u;goto L8c61ed06; /* mov #0x14,r0 */
L8c61ed06: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ed08; /* fadd fr3,fr2 */
L8c61ed08: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ed0a; /* fmov fr2,@(r0,r15) */
L8c61ed0a: target=r[3];*pr=0x8c61ed0eu;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ed0e; /* jsr @r3 */
L8c61ed0e: r[3]=rd(m,bus,&io_status,0x8c61ef4cu,4);if(io_status)return io_status;goto L8c61ed10; /* mov.l 0x8c61ef4c,r3 */
L8c61ed10: r[0]=12u;goto L8c61ed12; /* mov #0xc,r0 */
L8c61ed12: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ed14; /* fmov.s @(r0,r15),fr3 */
L8c61ed14: r[0]=16u;goto L8c61ed16; /* mov #0x10,r0 */
L8c61ed16: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61ed18; /* fmov.s @r3,fr2 */
L8c61ed18: r[3]+=8u;goto L8c61ed1a; /* add #0x8,r3 */
L8c61ed1a: r[2]=rd(m,bus,&io_status,0x8c61ef50u,4);if(io_status)return io_status;goto L8c61ed1c; /* mov.l 0x8c61ef50,r2 */
L8c61ed1c: r[8]+=24u;goto L8c61ed1e; /* add #0x18,r8 */
L8c61ed1e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ed20; /* fmul fr3,fr2 */
L8c61ed20: r[1]=rd(m,bus,&io_status,0x8c61ef54u,4);if(io_status)return io_status;goto L8c61ed22; /* mov.l 0x8c61ef54,r1 */
L8c61ed22: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61ed24; /* fmov.s @r2,fr3 */
L8c61ed24: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61ed26; /* fmov.s @r1,fr1 */
L8c61ed26: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61ed28; /* mov.w @r10+,r4 */
L8c61ed28: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ed2a; /* fsub fr2,fr3 */
L8c61ed2a: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ed2c; /* fmov.s @(r0,r15),fr2 */
L8c61ed2c: r[0]=20u;goto L8c61ed2e; /* mov #0x14,r0 */
L8c61ed2e: r[1]=rd(m,bus,&io_status,0x8c61ef5cu,4);if(io_status)return io_status;goto L8c61ed30; /* mov.l 0x8c61ef5c,r1 */
L8c61ed30: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ed32; /* fmul fr2,fr1 */
L8c61ed32: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ed34; /* fmov.s @(r0,r15),fr2 */
L8c61ed34: r[4]=r[4]&65535u;goto L8c61ed36; /* extu.w r4,r4 */
L8c61ed36: r[0]=rd(m,bus,&io_status,0x8c61ef58u,4);if(io_status)return io_status;goto L8c61ed38; /* mov.l 0x8c61ef58,r0 */
L8c61ed38: r[4]<<=2;goto L8c61ed3a; /* shll2 r4 */
L8c61ed3a: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61ed3c; /* mov.l @r0,r2 */
L8c61ed3c: r[4]<<=2;goto L8c61ed3e; /* shll2 r4 */
L8c61ed3e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ed40; /* fsub fr1,fr3 */
L8c61ed40: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61ed42; /* fmov.s @r3,fr1 */
L8c61ed42: r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61ed44; /* mov.l @r1,r3 */
L8c61ed44: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61ed46; /* shll r4 */
L8c61ed46: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ed48; /* fmul fr2,fr1 */
L8c61ed48: r[4]+=r[2];goto L8c61ed4a; /* add r2,r4 */
L8c61ed4a: fcopy(f,4u,3u);goto L8c61ed4c; /* fmov fr3,fr4 */
L8c61ed4c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61ed4e; /* fsub fr1,fr4 */
L8c61ed4e: r[0]=4u;goto L8c61ed50; /* mov #0x4,r0 */
L8c61ed50: r[1]=r[3];goto L8c61ed52; /* mov r3,r1 */
L8c61ed52: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ed54; /* fmov.s @(r0,r4),fr0 */
L8c61ed54: r[0]=20u;goto L8c61ed56; /* mov #0x14,r0 */
L8c61ed56: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61ed58; /* fmov.s @(r0,r1),fr3 */
L8c61ed58: r[0]=4u;goto L8c61ed5a; /* mov #0x4,r0 */
L8c61ed5a: r[2]=r[3];goto L8c61ed5c; /* mov r3,r2 */
L8c61ed5c: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61ed5e; /* fmov.s @(r0,r2),fr2 */
L8c61ed5e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ed60; /* fmul fr0,fr3 */
L8c61ed60: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ed62; /* fmov.s @r4,fr0 */
L8c61ed62: r[0]=8u;goto L8c61ed64; /* mov #0x8,r0 */
L8c61ed64: r[2]=rd(m,bus,&io_status,0x8c61ef5cu,4);if(io_status)return io_status;goto L8c61ed66; /* mov.l 0x8c61ef5c,r2 */
L8c61ed66: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ed68; /* fmac fr0,fr2,fr3 */
L8c61ed68: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ed6a; /* fmov.s @(r0,r4),fr0 */
L8c61ed6a: r[0]=36u;goto L8c61ed6c; /* mov #0x24,r0 */
L8c61ed6c: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61ed6e; /* fmov.s @(r0,r3),fr2 */
L8c61ed6e: r[0]=4u;goto L8c61ed70; /* mov #0x4,r0 */
L8c61ed70: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ed72; /* fmac fr0,fr2,fr3 */
L8c61ed72: fcopy(f,0u,14u);goto L8c61ed74; /* fmov fr14,fr0 */
L8c61ed74: fcopy(f,2u,14u);goto L8c61ed76; /* fmov fr14,fr2 */
L8c61ed76: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ed78; /* fmac fr0,fr3,fr2 */
L8c61ed78: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ed7a; /* fmov.s fr2,@-r8 */
L8c61ed7a: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ed7c; /* mov.l @r2,r3 */
L8c61ed7c: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ed7e; /* fmov.s @(r0,r4),fr0 */
L8c61ed7e: r[0]=16u;goto L8c61ed80; /* mov #0x10,r0 */
L8c61ed80: r[2]=r[3];goto L8c61ed82; /* mov r3,r2 */
L8c61ed82: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61ed84; /* fmov.s @(r0,r2),fr3 */
L8c61ed84: r[1]=r[3];goto L8c61ed86; /* mov r3,r1 */
L8c61ed86: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61ed88; /* fmov.s @r1,fr2 */
L8c61ed88: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ed8a; /* fmul fr0,fr3 */
L8c61ed8a: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ed8c; /* fmov.s @r4,fr0 */
L8c61ed8c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ed8e; /* fmac fr0,fr2,fr3 */
L8c61ed8e: r[0]=8u;goto L8c61ed90; /* mov #0x8,r0 */
L8c61ed90: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ed92; /* fmov.s @(r0,r4),fr0 */
L8c61ed92: r[0]=32u;goto L8c61ed94; /* mov #0x20,r0 */
L8c61ed94: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61ed96; /* fmov.s @(r0,r3),fr2 */
L8c61ed96: r[0]=24u;goto L8c61ed98; /* mov #0x18,r0 */
L8c61ed98: r[3]=rd(m,bus,&io_status,0x8c61ef60u,4);if(io_status)return io_status;goto L8c61ed9a; /* mov.l 0x8c61ef60,r3 */
L8c61ed9a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ed9c; /* fmac fr0,fr2,fr3 */
L8c61ed9c: fcopy(f,0u,14u);goto L8c61ed9e; /* fmov fr14,fr0 */
L8c61ed9e: fcopy(f,2u,14u);goto L8c61eda0; /* fmov fr14,fr2 */
L8c61eda0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61eda2; /* fmac fr0,fr3,fr2 */
L8c61eda2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61eda4; /* fmov.s fr2,@-r8 */
L8c61eda4: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61eda6; /* fmov.s @(r0,r4),fr3 */
L8c61eda6: r[0]=20u;goto L8c61eda8; /* mov #0x14,r0 */
L8c61eda8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61edaa; /* fmov.s fr3,@-r8 */
L8c61edaa: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61edac; /* fmov.s @(r0,r4),fr3 */
L8c61edac: r[0]=16u;goto L8c61edae; /* mov #0x10,r0 */
L8c61edae: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61edb0; /* fmov.s fr3,@-r8 */
L8c61edb0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61edb2; /* fmov.s @(r0,r4),fr3 */
L8c61edb2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61edb4; /* fmov.s fr3,@-r8 */
L8c61edb4: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61edb6; /* mov.l r3,@-r8 */
L8c61edb6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61edb8; /* pref @r8 */
L8c61edb8: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61edba; /* mov.w @r10+,r4 */
L8c61edba: r[8]+=56u;goto L8c61edbc; /* add #0x38,r8 */
L8c61edbc: r[2]=rd(m,bus,&io_status,0x8c61ef58u,4);if(io_status)return io_status;goto L8c61edbe; /* mov.l 0x8c61ef58,r2 */
L8c61edbe: r[4]=r[4]&65535u;goto L8c61edc0; /* extu.w r4,r4 */
L8c61edc0: r[0]=rd(m,bus,&io_status,0x8c61ef5cu,4);if(io_status)return io_status;goto L8c61edc2; /* mov.l 0x8c61ef5c,r0 */
L8c61edc2: r[4]<<=2;goto L8c61edc4; /* shll2 r4 */
L8c61edc4: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61edc6; /* mov.l @r2,r3 */
L8c61edc6: r[4]<<=2;goto L8c61edc8; /* shll2 r4 */
L8c61edc8: r[1]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61edca; /* mov.l @r0,r1 */
L8c61edca: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61edcc; /* shll r4 */
L8c61edcc: r[0]=4u;goto L8c61edce; /* mov #0x4,r0 */
L8c61edce: r[2]=r[1];goto L8c61edd0; /* mov r1,r2 */
L8c61edd0: r[4]+=r[3];goto L8c61edd2; /* add r3,r4 */
L8c61edd2: r[3]=r[1];goto L8c61edd4; /* mov r1,r3 */
L8c61edd4: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61edd6; /* fmov.s @(r0,r4),fr0 */
L8c61edd6: r[0]=20u;goto L8c61edd8; /* mov #0x14,r0 */
L8c61edd8: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61edda; /* fmov.s @(r0,r2),fr3 */
L8c61edda: r[0]=4u;goto L8c61eddc; /* mov #0x4,r0 */
L8c61eddc: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61edde; /* fmov.s @(r0,r3),fr2 */
L8c61edde: r[0]=8u;goto L8c61ede0; /* mov #0x8,r0 */
L8c61ede0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ede2; /* fmul fr0,fr3 */
L8c61ede2: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ede4; /* fmov.s @r4,fr0 */
L8c61ede4: r[3]=rd(m,bus,&io_status,0x8c61ef5cu,4);if(io_status)return io_status;goto L8c61ede6; /* mov.l 0x8c61ef5c,r3 */
L8c61ede6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ede8; /* fmac fr0,fr2,fr3 */
L8c61ede8: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61edea; /* fmov.s @(r0,r4),fr0 */
L8c61edea: r[0]=36u;goto L8c61edec; /* mov #0x24,r0 */
L8c61edec: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61edee; /* fmov.s @(r0,r1),fr2 */
L8c61edee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61edf0; /* fmac fr0,fr2,fr3 */
L8c61edf0: fcopy(f,0u,14u);goto L8c61edf2; /* fmov fr14,fr0 */
L8c61edf2: fcopy(f,2u,14u);goto L8c61edf4; /* fmov fr14,fr2 */
L8c61edf4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61edf6; /* fmac fr0,fr3,fr2 */
L8c61edf6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61edf8; /* fmov.s fr2,@-r8 */
L8c61edf8: r[0]=4u;goto L8c61edfa; /* mov #0x4,r0 */
L8c61edfa: r[11]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61edfc; /* mov.l @r3,r11 */
L8c61edfc: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61edfe; /* fmov.s @(r0,r4),fr0 */
L8c61edfe: r[0]=16u;goto L8c61ee00; /* mov #0x10,r0 */
L8c61ee00: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61ee02; /* fmov.s @(r0,r11),fr3 */
L8c61ee02: r[0]=8u;goto L8c61ee04; /* mov #0x8,r0 */
L8c61ee04: fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c61ee06; /* fmov.s @r11,fr2 */
L8c61ee06: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee08; /* fmul fr0,fr3 */
L8c61ee08: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ee0a; /* fmov.s @r4,fr0 */
L8c61ee0a: r[2]=rd(m,bus,&io_status,0x8c61ef60u,4);if(io_status)return io_status;goto L8c61ee0c; /* mov.l 0x8c61ef60,r2 */
L8c61ee0c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee0e; /* fmac fr0,fr2,fr3 */
L8c61ee0e: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee10; /* fmov.s @(r0,r4),fr0 */
L8c61ee10: r[0]=32u;goto L8c61ee12; /* mov #0x20,r0 */
L8c61ee12: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61ee14; /* fmov.s @(r0,r11),fr2 */
L8c61ee14: r[0]=24u;goto L8c61ee16; /* mov #0x18,r0 */
L8c61ee16: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee18; /* fmac fr0,fr2,fr3 */
L8c61ee18: fcopy(f,0u,14u);goto L8c61ee1a; /* fmov fr14,fr0 */
L8c61ee1a: fcopy(f,2u,14u);goto L8c61ee1c; /* fmov fr14,fr2 */
L8c61ee1c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ee1e; /* fmac fr0,fr3,fr2 */
L8c61ee1e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ee20; /* fmov.s fr2,@-r8 */
L8c61ee20: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee22; /* fmov.s @(r0,r4),fr3 */
L8c61ee22: r[0]=20u;goto L8c61ee24; /* mov #0x14,r0 */
L8c61ee24: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ee26; /* fmov.s fr3,@-r8 */
L8c61ee26: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee28; /* fmov.s @(r0,r4),fr3 */
L8c61ee28: r[0]=16u;goto L8c61ee2a; /* mov #0x10,r0 */
L8c61ee2a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ee2c; /* fmov.s fr3,@-r8 */
L8c61ee2c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee2e; /* fmov.s @(r0,r4),fr3 */
L8c61ee2e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ee30; /* fmov.s fr3,@-r8 */
L8c61ee30: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61ee32; /* mov.l r2,@-r8 */
L8c61ee32: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61ee34; /* pref @r8 */
L8c61ee34: r[6]=r[9];goto L8c61ee36; /* mov r9,r6 */
L8c61ee36: r[4]=2u;goto L8c61ee38; /* mov #0x2,r4 */
L8c61ee38: r[6]+=4294967295u;goto L8c61ee3a; /* add #-0x1,r6 */
L8c61ee3a: r[5]=r[4];goto L8c61ee3c; /* mov r4,r5 */
L8c61ee3c: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61ee3e; /* cmp/ge r6,r5 */
L8c61ee3e: take=(*sr&1u);r[8]+=32u;if(take)goto L8c61eec2;goto L8c61ee42; /* bt/s 0x8c61eec2 */
L8c61ee42: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61ee44; /* mov.w @r10+,r4 */
L8c61ee44: r[0]=4u;goto L8c61ee46; /* mov #0x4,r0 */
L8c61ee46: r[2]=rd(m,bus,&io_status,0x8c61ef58u,4);if(io_status)return io_status;goto L8c61ee48; /* mov.l 0x8c61ef58,r2 */
L8c61ee48: r[8]+=28u;goto L8c61ee4a; /* add #0x1c,r8 */
L8c61ee4a: r[4]=r[4]&65535u;goto L8c61ee4c; /* extu.w r4,r4 */
L8c61ee4c: r[4]<<=2;goto L8c61ee4e; /* shll2 r4 */
L8c61ee4e: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ee50; /* mov.l @r2,r3 */
L8c61ee50: r[4]<<=2;goto L8c61ee52; /* shll2 r4 */
L8c61ee52: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61ee54; /* fmov.s fr4,@-r8 */
L8c61ee54: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61ee56; /* shll r4 */
L8c61ee56: r[4]+=r[3];goto L8c61ee58; /* add r3,r4 */
L8c61ee58: r[3]=rd(m,bus,&io_status,0x8c61ef5cu,4);if(io_status)return io_status;goto L8c61ee5a; /* mov.l 0x8c61ef5c,r3 */
L8c61ee5a: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee5c; /* fmov.s @(r0,r4),fr0 */
L8c61ee5c: r[0]=20u;goto L8c61ee5e; /* mov #0x14,r0 */
L8c61ee5e: r[11]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ee60; /* mov.l @r3,r11 */
L8c61ee60: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61ee62; /* fmov.s @(r0,r11),fr3 */
L8c61ee62: r[0]=4u;goto L8c61ee64; /* mov #0x4,r0 */
L8c61ee64: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61ee66; /* fmov.s @(r0,r11),fr2 */
L8c61ee66: r[0]=8u;goto L8c61ee68; /* mov #0x8,r0 */
L8c61ee68: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee6a; /* fmul fr0,fr3 */
L8c61ee6a: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ee6c; /* fmov.s @r4,fr0 */
L8c61ee6c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee6e; /* fmac fr0,fr2,fr3 */
L8c61ee6e: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee70; /* fmov.s @(r0,r4),fr0 */
L8c61ee70: r[0]=36u;goto L8c61ee72; /* mov #0x24,r0 */
L8c61ee72: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61ee74; /* fmov.s @(r0,r11),fr2 */
L8c61ee74: r[0]=4u;goto L8c61ee76; /* mov #0x4,r0 */
L8c61ee76: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee78; /* fmac fr0,fr2,fr3 */
L8c61ee78: fcopy(f,2u,14u);goto L8c61ee7a; /* fmov fr14,fr2 */
L8c61ee7a: fcopy(f,0u,14u);goto L8c61ee7c; /* fmov fr14,fr0 */
L8c61ee7c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ee7e; /* fmac fr0,fr3,fr2 */
L8c61ee7e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ee80; /* fmov.s fr2,@-r8 */
L8c61ee80: r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ee82; /* mov.l @r3,r7 */
L8c61ee82: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee84; /* fmov.s @(r0,r4),fr0 */
L8c61ee84: r[0]=16u;goto L8c61ee86; /* mov #0x10,r0 */
L8c61ee86: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61ee88; /* fmov.s @(r0,r7),fr3 */
L8c61ee88: r[0]=8u;goto L8c61ee8a; /* mov #0x8,r0 */
L8c61ee8a: fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61ee8c; /* fmov.s @r7,fr2 */
L8c61ee8c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee8e; /* fmul fr0,fr3 */
L8c61ee8e: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ee90; /* fmov.s @r4,fr0 */
L8c61ee90: r[1]=rd(m,bus,&io_status,0x8c61ef60u,4);if(io_status)return io_status;goto L8c61ee92; /* mov.l 0x8c61ef60,r1 */
L8c61ee92: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee94; /* fmac fr0,fr2,fr3 */
L8c61ee94: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee96; /* fmov.s @(r0,r4),fr0 */
L8c61ee96: r[0]=32u;goto L8c61ee98; /* mov #0x20,r0 */
L8c61ee98: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61ee9a; /* fmov.s @(r0,r7),fr2 */
L8c61ee9a: r[0]=24u;goto L8c61ee9c; /* mov #0x18,r0 */
L8c61ee9c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee9e; /* fmac fr0,fr2,fr3 */
L8c61ee9e: fcopy(f,0u,14u);goto L8c61eea0; /* fmov fr14,fr0 */
L8c61eea0: fcopy(f,2u,14u);goto L8c61eea2; /* fmov fr14,fr2 */
L8c61eea2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61eea4; /* fmac fr0,fr3,fr2 */
L8c61eea4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61eea6; /* fmov.s fr2,@-r8 */
L8c61eea6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61eea8; /* fmov.s @(r0,r4),fr3 */
L8c61eea8: r[0]=20u;goto L8c61eeaa; /* mov #0x14,r0 */
L8c61eeaa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eeac; /* fmov.s fr3,@-r8 */
L8c61eeac: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61eeae; /* fmov.s @(r0,r4),fr3 */
L8c61eeae: r[0]=16u;goto L8c61eeb0; /* mov #0x10,r0 */
L8c61eeb0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eeb2; /* fmov.s fr3,@-r8 */
L8c61eeb2: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61eeb4; /* fmov.s @(r0,r4),fr3 */
L8c61eeb4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eeb6; /* fmov.s fr3,@-r8 */
L8c61eeb6: r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c61eeb8; /* mov.l r1,@-r8 */
L8c61eeb8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61eeba; /* pref @r8 */
L8c61eeba: r[5]+=1u;goto L8c61eebc; /* add #0x1,r5 */
L8c61eebc: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61eebe; /* cmp/ge r6,r5 */
L8c61eebe: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61ee42;goto L8c61eec2; /* bf/s 0x8c61ee42 */
L8c61eec2: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61eec4; /* mov.w @r10+,r4 */
L8c61eec4: r[0]=4u;goto L8c61eec6; /* mov #0x4,r0 */
L8c61eec6: r[2]=rd(m,bus,&io_status,0x8c61ef58u,4);if(io_status)return io_status;goto L8c61eec8; /* mov.l 0x8c61ef58,r2 */
L8c61eec8: r[8]+=28u;goto L8c61eeca; /* add #0x1c,r8 */
L8c61eeca: r[4]=r[4]&65535u;goto L8c61eecc; /* extu.w r4,r4 */
L8c61eecc: r[4]<<=2;goto L8c61eece; /* shll2 r4 */
L8c61eece: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61eed0; /* mov.l @r2,r3 */
L8c61eed0: r[4]<<=2;goto L8c61eed2; /* shll2 r4 */
L8c61eed2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61eed4; /* fmov.s fr4,@-r8 */
L8c61eed4: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61eed6; /* shll r4 */
L8c61eed6: r[4]+=r[3];goto L8c61eed8; /* add r3,r4 */
L8c61eed8: r[3]=rd(m,bus,&io_status,0x8c61ef5cu,4);if(io_status)return io_status;goto L8c61eeda; /* mov.l 0x8c61ef5c,r3 */
L8c61eeda: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61eedc; /* fmov.s @(r0,r4),fr0 */
L8c61eedc: r[0]=20u;goto L8c61eede; /* mov #0x14,r0 */
L8c61eede: r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61eee0; /* mov.l @r3,r6 */
L8c61eee0: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61eee2; /* fmov.s @(r0,r6),fr3 */
L8c61eee2: r[0]=4u;goto L8c61eee4; /* mov #0x4,r0 */
L8c61eee4: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61eee6; /* fmov.s @(r0,r6),fr2 */
L8c61eee6: r[0]=8u;goto L8c61eee8; /* mov #0x8,r0 */
L8c61eee8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eeea; /* fmul fr0,fr3 */
L8c61eeea: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61eeec; /* fmov.s @r4,fr0 */
L8c61eeec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eeee; /* fmac fr0,fr2,fr3 */
L8c61eeee: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61eef0; /* fmov.s @(r0,r4),fr0 */
L8c61eef0: r[0]=36u;goto L8c61eef2; /* mov #0x24,r0 */
L8c61eef2: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61eef4; /* fmov.s @(r0,r6),fr2 */
L8c61eef4: r[0]=4u;goto L8c61eef6; /* mov #0x4,r0 */
L8c61eef6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eef8; /* fmac fr0,fr2,fr3 */
L8c61eef8: fcopy(f,2u,14u);goto L8c61eefa; /* fmov fr14,fr2 */
L8c61eefa: fcopy(f,0u,14u);goto L8c61eefc; /* fmov fr14,fr0 */
L8c61eefc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61eefe; /* fmac fr0,fr3,fr2 */
L8c61eefe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ef00; /* fmov.s fr2,@-r8 */
L8c61ef00: r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ef02; /* mov.l @r3,r5 */
L8c61ef02: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ef04; /* fmov.s @(r0,r4),fr0 */
L8c61ef04: r[0]=16u;goto L8c61ef06; /* mov #0x10,r0 */
L8c61ef06: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61ef08; /* fmov.s @(r0,r5),fr3 */
L8c61ef08: r[0]=8u;goto L8c61ef0a; /* mov #0x8,r0 */
L8c61ef0a: fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c61ef0c; /* fmov.s @r5,fr2 */
L8c61ef0c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ef0e; /* fmul fr0,fr3 */
L8c61ef0e: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ef10; /* fmov.s @r4,fr0 */
L8c61ef10: r[1]=rd(m,bus,&io_status,0x8c61ef64u,4);if(io_status)return io_status;goto L8c61ef12; /* mov.l 0x8c61ef64,r1 */
L8c61ef12: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ef14; /* fmac fr0,fr2,fr3 */
L8c61ef14: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ef16; /* fmov.s @(r0,r4),fr0 */
L8c61ef16: r[0]=32u;goto L8c61ef18; /* mov #0x20,r0 */
L8c61ef18: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61ef1a; /* fmov.s @(r0,r5),fr2 */
L8c61ef1a: r[0]=24u;goto L8c61ef1c; /* mov #0x18,r0 */
L8c61ef1c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ef1e; /* fmac fr0,fr2,fr3 */
L8c61ef1e: fcopy(f,0u,14u);goto L8c61ef20; /* fmov fr14,fr0 */
L8c61ef20: fcopy(f,2u,14u);goto L8c61ef22; /* fmov fr14,fr2 */
L8c61ef22: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ef24; /* fmac fr0,fr3,fr2 */
L8c61ef24: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ef26; /* fmov.s fr2,@-r8 */
L8c61ef26: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ef28; /* fmov.s @(r0,r4),fr3 */
L8c61ef28: r[0]=20u;goto L8c61ef2a; /* mov #0x14,r0 */
L8c61ef2a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ef2c; /* fmov.s fr3,@-r8 */
L8c61ef2c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ef2e; /* fmov.s @(r0,r4),fr3 */
L8c61ef2e: r[0]=16u;goto L8c61ef30; /* mov #0x10,r0 */
L8c61ef30: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ef32; /* fmov.s fr3,@-r8 */
L8c61ef32: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ef34; /* fmov.s @(r0,r4),fr3 */
L8c61ef34: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ef36; /* fmov.s fr3,@-r8 */
L8c61ef36: r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c61ef38; /* mov.l r1,@-r8 */
L8c61ef38: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61ef3a; /* pref @r8 */
L8c61ef3a: r[8]+=32u;goto L8c61f028; /* bra 0x8c61f028 */
L8c61ef68: r[0]=12u;goto L8c61ef6a; /* mov #0xc,r0 */
L8c61ef6a: r[2]=rd(m,bus,&io_status,0x8c61f0e4u,4);if(io_status)return io_status;goto L8c61ef6c; /* mov.l 0x8c61f0e4,r2 */
L8c61ef6c: r[3]=0u;goto L8c61ef6e; /* mov #0x0,r3 */
L8c61ef6e: r[4]=r[15];goto L8c61ef70; /* mov r15,r4 */
L8c61ef70: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61ef72; /* mov.l r3,@r2 */
L8c61ef72: if((r[14]&0x1f000000u)!=0x0c000000u)return -3;fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61ef74; /* fmov.s @r14,fr3 */
L8c61ef74: fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61ef76; /* fmov.s @r12,fr2 */
L8c61ef76: r[3]=rd(m,bus,&io_status,0x8c61f0e8u,4);if(io_status)return io_status;goto L8c61ef78; /* mov.l 0x8c61f0e8,r3 */
L8c61ef78: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ef7a; /* fadd fr3,fr2 */
L8c61ef7a: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61ef7c; /* fmov.s @r13,fr3 */
L8c61ef7c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ef7e; /* fadd fr3,fr2 */
L8c61ef7e: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ef80; /* fmov fr2,@(r0,r15) */
L8c61ef80: r[0]=4u;goto L8c61ef82; /* mov #0x4,r0 */
L8c61ef82: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ef84; /* fmov.s @(r0,r14),fr3 */
L8c61ef84: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61ef86; /* fmov.s @(r0,r12),fr2 */
L8c61ef86: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ef88; /* fadd fr3,fr2 */
L8c61ef88: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ef8a; /* fmov.s @(r0,r13),fr3 */
L8c61ef8a: r[0]=16u;goto L8c61ef8c; /* mov #0x10,r0 */
L8c61ef8c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ef8e; /* fadd fr3,fr2 */
L8c61ef8e: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ef90; /* fmov fr2,@(r0,r15) */
L8c61ef90: r[0]=8u;goto L8c61ef92; /* mov #0x8,r0 */
L8c61ef92: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ef94; /* fmov.s @(r0,r14),fr3 */
L8c61ef94: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61ef96; /* fmov.s @(r0,r12),fr2 */
L8c61ef96: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ef98; /* fadd fr3,fr2 */
L8c61ef98: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ef9a; /* fmov.s @(r0,r13),fr3 */
L8c61ef9a: r[0]=20u;goto L8c61ef9c; /* mov #0x14,r0 */
L8c61ef9c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ef9e; /* fadd fr3,fr2 */
L8c61ef9e: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61efa0; /* fmov fr2,@(r0,r15) */
L8c61efa0: target=r[3];*pr=0x8c61efa4u;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61efa4; /* jsr @r3 */
L8c61efa4: r[3]=rd(m,bus,&io_status,0x8c61f0ecu,4);if(io_status)return io_status;goto L8c61efa6; /* mov.l 0x8c61f0ec,r3 */
L8c61efa6: r[0]=12u;goto L8c61efa8; /* mov #0xc,r0 */
L8c61efa8: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61efaa; /* fmov.s @(r0,r15),fr3 */
L8c61efaa: r[0]=16u;goto L8c61efac; /* mov #0x10,r0 */
L8c61efac: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61efae; /* fmov.s @r3,fr2 */
L8c61efae: r[3]+=8u;goto L8c61efb0; /* add #0x8,r3 */
L8c61efb0: r[2]=rd(m,bus,&io_status,0x8c61f0f0u,4);if(io_status)return io_status;goto L8c61efb2; /* mov.l 0x8c61f0f0,r2 */
L8c61efb2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61efb4; /* fmul fr3,fr2 */
L8c61efb4: r[1]=rd(m,bus,&io_status,0x8c61f0f4u,4);if(io_status)return io_status;goto L8c61efb6; /* mov.l 0x8c61f0f4,r1 */
L8c61efb6: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61efb8; /* fmov.s @r2,fr3 */
L8c61efb8: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61efba; /* fmov.s @r1,fr1 */
L8c61efba: r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61efbc; /* mov.w @r10+,r12 */
L8c61efbc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61efbe; /* fsub fr2,fr3 */
L8c61efbe: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61efc0; /* fmov.s @(r0,r15),fr2 */
L8c61efc0: r[0]=20u;goto L8c61efc2; /* mov #0x14,r0 */
L8c61efc2: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61efc4; /* mov.w @r10+,r14 */
L8c61efc4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61efc6; /* fmul fr2,fr1 */
L8c61efc6: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61efc8; /* fmov.s @(r0,r15),fr2 */
L8c61efc8: r[12]=r[12]&65535u;goto L8c61efca; /* extu.w r12,r12 */
L8c61efca: r[0]=rd(m,bus,&io_status,0x8c61f0f8u,4);if(io_status)return io_status;goto L8c61efcc; /* mov.l 0x8c61f0f8,r0 */
L8c61efcc: r[12]<<=2;goto L8c61efce; /* shll2 r12 */
L8c61efce: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61efd0; /* mov.l @r0,r2 */
L8c61efd0: r[12]<<=2;goto L8c61efd2; /* shll2 r12 */
L8c61efd2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61efd4; /* fsub fr1,fr3 */
L8c61efd4: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61efd6; /* fmov.s @r3,fr1 */
L8c61efd6: r[14]=r[14]&65535u;goto L8c61efd8; /* extu.w r14,r14 */
L8c61efd8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61efda; /* fmul fr2,fr1 */
L8c61efda: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61efdc; /* shll r12 */
L8c61efdc: r[14]<<=2;goto L8c61efde; /* shll2 r14 */
L8c61efde: fcopy(f,15u,3u);goto L8c61efe0; /* fmov fr3,fr15 */
L8c61efe0: r[12]+=r[2];goto L8c61efe2; /* add r2,r12 */
L8c61efe2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[1],&f->fpscr,&fr[15],1);if(status)return status;}goto L8c61efe4; /* fsub fr1,fr15 */
L8c61efe4: r[14]<<=2;goto L8c61efe6; /* shll2 r14 */
L8c61efe6: r[9]+=4294967294u;goto L8c61efe8; /* add #-0x2,r9 */
L8c61efe8: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61efea; /* shll r14 */
L8c61efea: *sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c61efec; /* cmp/pl r9 */
L8c61efec: r[14]+=r[2];goto L8c61efee; /* add r2,r14 */
L8c61efee: take=!(*sr&1u);r[11]=0u;if(take)goto L8c61f028;goto L8c61eff2; /* bf/s 0x8c61f028 */
L8c61eff2: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61eff4; /* mov.w @r10+,r13 */
L8c61eff4: r[2]=rd(m,bus,&io_status,0x8c61f0f8u,4);if(io_status)return io_status;goto L8c61eff6; /* mov.l 0x8c61f0f8,r2 */
L8c61eff6: r[13]=r[13]&65535u;goto L8c61eff8; /* extu.w r13,r13 */
L8c61eff8: r[4]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61effa; /* mov.l @(0xc,r12),r4 */
L8c61effa: r[13]<<=2;goto L8c61effc; /* shll2 r13 */
L8c61effc: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61effe; /* mov.l @r2,r3 */
L8c61effe: r[13]<<=2;goto L8c61f000; /* shll2 r13 */
L8c61f000: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61f002; /* shll r13 */
L8c61f002: r[13]+=r[3];goto L8c61f004; /* add r3,r13 */
L8c61f004: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61f006; /* mov.l @(0xc,r14),r3 */
L8c61f006: r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61f008; /* mov.l @(0xc,r13),r1 */
L8c61f008: r[4]&=r[3];goto L8c61f00a; /* and r3,r4 */
L8c61f00a: r[3]=1u;goto L8c61f00c; /* mov #0x1,r3 */
L8c61f00c: r[4]&=r[1];goto L8c61f00e; /* and r1,r4 */
L8c61f00e: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61f010; /* tst r3,r4 */
L8c61f010: if(!(*sr&1u))goto L8c61f01e;goto L8c61f012; /* bf 0x8c61f01e */
L8c61f012: r[1]=rd(m,bus,&io_status,0x8c61f0fcu,4);if(io_status)return io_status;goto L8c61f014; /* mov.l 0x8c61f0fc,r1 */
L8c61f014: r[5]=r[14];goto L8c61f016; /* mov r14,r5 */
L8c61f016: fcopy(f,4u,15u);goto L8c61f018; /* fmov fr15,fr4 */
L8c61f018: r[6]=r[13];goto L8c61f01a; /* mov r13,r6 */
L8c61f01a: target=r[1];*pr=0x8c61f01eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61f01e; /* jsr @r1 */
L8c61f01e: r[11]+=1u;goto L8c61f020; /* add #0x1,r11 */
L8c61f020: r[12]=r[14];goto L8c61f022; /* mov r14,r12 */
L8c61f022: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[9]);goto L8c61f024; /* cmp/ge r9,r11 */
L8c61f024: take=!(*sr&1u);r[14]=r[13];if(take)goto L8c61eff2;goto L8c61f028; /* bf/s 0x8c61eff2 */
L8c61f028: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61f02a; /* mov.l @(0x4,r15),r2 */
L8c61f02a: r[2]+=1u;goto L8c61f02c; /* add #0x1,r2 */
L8c61f02c: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c61f02e; /* mov.l r2,@(0x4,r15) */
L8c61f02e: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61f030; /* mov.l @r15,r3 */
L8c61f030: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61f032; /* mov.w @(0x2,r3),r0 */
L8c61f032: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61f034; /* mov.l @(0x4,r15),r3 */
L8c61f034: r[0]=r[0]&65535u;goto L8c61f036; /* extu.w r0,r0 */
L8c61f036: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c61f038; /* cmp/ge r0,r3 */
L8c61f038: if((*sr&1u))goto L8c61f03e;goto L8c61f03a; /* bt 0x8c61f03e */
L8c61f03a: goto L8c61ec0e; /* bra 0x8c61ec0e */
L8c61f03e: goto L8c61f0bc; /* bra 0x8c61f0bc */
L8c61f042: r[1]=rd(m,bus,&io_status,0x8c61f100u,4);if(io_status)return io_status;goto L8c61f044; /* mov.l 0x8c61f100,r1 */
L8c61f044: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61f046; /* mov.l @r15,r2 */
L8c61f046: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61f048; /* mov.l r2,@r1 */
L8c61f048: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f04a; /* frchg */
L8c61f04a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61f04c; /* fmov.s fr0,@-r15 */
L8c61f04c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61f04e; /* fmov.s fr1,@-r15 */
L8c61f04e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61f050; /* fmov.s fr2,@-r15 */
L8c61f050: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61f052; /* fmov.s fr3,@-r15 */
L8c61f052: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61f054; /* fmov.s fr4,@-r15 */
L8c61f054: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61f056; /* fmov.s fr5,@-r15 */
L8c61f056: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61f058; /* fmov.s fr6,@-r15 */
L8c61f058: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61f05a; /* fmov.s fr7,@-r15 */
L8c61f05a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61f05c; /* fmov.s fr8,@-r15 */
L8c61f05c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61f05e; /* fmov.s fr9,@-r15 */
L8c61f05e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c61f060; /* fmov.s fr10,@-r15 */
L8c61f060: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c61f062; /* fmov.s fr11,@-r15 */
L8c61f062: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c61f064; /* fmov.s fr12,@-r15 */
L8c61f064: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61f066; /* fmov.s fr13,@-r15 */
L8c61f066: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61f068; /* fmov.s fr14,@-r15 */
L8c61f068: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61f06a; /* fmov.s fr15,@-r15 */
L8c61f06a: r[0]=0x8c61f0d4u;goto L8c61f06c; /* mova 0x8c61f0d4,r0 */
L8c61f06c: fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c61f06e; /* fmov.s @r0,fr12 */
L8c61f06e: fcopy(f,13u,12u);goto L8c61f070; /* fmov fr12,fr13 */
L8c61f070: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[12],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61f072; /* fmul fr12,fr0 */
L8c61f072: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61f074; /* fmul fr12,fr4 */
L8c61f074: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[12],&f->fpscr,&fr[8]);if(status)return status;}goto L8c61f076; /* fmul fr12,fr8 */
L8c61f076: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[12],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61f078; /* fmul fr12,fr1 */
L8c61f078: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[12],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61f07a; /* fmul fr12,fr5 */
L8c61f07a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[12],&f->fpscr,&fr[9]);if(status)return status;}goto L8c61f07c; /* fmul fr12,fr9 */
L8c61f07c: r[0]=rd(m,bus,&io_status,0x8c61f0d8u,4);if(io_status)return io_status;goto L8c61f07e; /* mov.l 0x8c61f0d8,r0 */
L8c61f07e: fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f080; /* fmov.s @r0+,fr2 */
L8c61f080: fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f082; /* fmov.s @r0+,fr6 */
L8c61f082: fload(m,bus,&io_status,f,10u,r[0]);if(io_status)return io_status;goto L8c61f084; /* fmov.s @r0,fr10 */
L8c61f084: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c61f086; /* fneg fr2 */
L8c61f086: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]^=0x80000000u;goto L8c61f088; /* fneg fr6 */
L8c61f088: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]^=0x80000000u;goto L8c61f08a; /* fneg fr10 */
L8c61f08a: r[0]=rd(m,bus,&io_status,0x8c61f0dcu,4);if(io_status)return io_status;goto L8c61f08c; /* mov.l 0x8c61f0dc,r0 */
L8c61f08c: fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c61f08e; /* fmov.s @r0,fr14 */
L8c61f08e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f090; /* frchg */
L8c61f090: r[2]=rd(m,bus,&io_status,0x8c61f104u,4);if(io_status)return io_status;goto L8c61f092; /* mov.l 0x8c61f104,r2 */
L8c61f092: r[3]=rd(m,bus,&io_status,0x8c61f100u,4);if(io_status)return io_status;goto L8c61f094; /* mov.l 0x8c61f100,r3 */
L8c61f094: target=r[2];*pr=0x8c61f098u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61f098; /* jsr @r2 */
L8c61f098: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f09a; /* frchg */
L8c61f09a: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f09c; /* fmov.s @r15+,fr15 */
L8c61f09c: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f09e; /* fmov.s @r15+,fr14 */
L8c61f09e: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0a0; /* fmov.s @r15+,fr13 */
L8c61f0a0: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0a2; /* fmov.s @r15+,fr12 */
L8c61f0a2: fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0a4; /* fmov.s @r15+,fr11 */
L8c61f0a4: fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0a6; /* fmov.s @r15+,fr10 */
L8c61f0a6: fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0a8; /* fmov.s @r15+,fr9 */
L8c61f0a8: fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0aa; /* fmov.s @r15+,fr8 */
L8c61f0aa: fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0ac; /* fmov.s @r15+,fr7 */
L8c61f0ac: fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0ae; /* fmov.s @r15+,fr6 */
L8c61f0ae: fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0b0; /* fmov.s @r15+,fr5 */
L8c61f0b0: fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0b2; /* fmov.s @r15+,fr4 */
L8c61f0b2: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0b4; /* fmov.s @r15+,fr3 */
L8c61f0b4: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0b6; /* fmov.s @r15+,fr2 */
L8c61f0b6: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0b8; /* fmov.s @r15+,fr1 */
L8c61f0b8: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0ba; /* fmov.s @r15+,fr0 */
L8c61f0ba: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f0bc; /* frchg */
L8c61f0bc: r[15]+=24u;goto L8c61f0be; /* add #0x18,r15 */
L8c61f0be: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f0c0; /* lds.l @r15+,PR */
L8c61f0c0: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0c2; /* fmov.s @r15+,fr14 */
L8c61f0c2: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0c4; /* fmov.s @r15+,fr15 */
L8c61f0c4: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f0c6; /* mov.l @r15+,r9 */
L8c61f0c6: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f0c8; /* mov.l @r15+,r10 */
L8c61f0c8: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f0ca; /* mov.l @r15+,r11 */
L8c61f0ca: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f0cc; /* mov.l @r15+,r12 */
L8c61f0cc: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f0ce; /* mov.l @r15+,r13 */
L8c61f0ce: if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
