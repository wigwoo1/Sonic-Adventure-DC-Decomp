/* UV/lit primitive family with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_uv_lit_primitive_emit_8c618d18(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c618d18;
L8c618d18: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c618d1a; /* mov.l r14,@-r15 */
L8c618d1a: r[0]=0x8c618dd4u;goto L8c618d1c; /* mova 0x8c618dd4,r0 */
L8c618d1c: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c618d1e; /* mov.l r13,@-r15 */
L8c618d1e: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c618d20; /* mov.l r12,@-r15 */
L8c618d20: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c618d22; /* mov.l r11,@-r15 */
L8c618d22: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c618d24; /* mov.l r10,@-r15 */
L8c618d24: r[3]=rd(m,bus,&io_status,0x8c618ddcu,4);if(io_status)return io_status;goto L8c618d26; /* mov.l 0x8c618ddc,r3 */
L8c618d26: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c618d28; /* mov.l r9,@-r15 */
L8c618d28: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c618d2a; /* sts.l PR,@-r15 */
L8c618d2a: fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c618d2c; /* fmov.s @r0,fr6 */
L8c618d2c: r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c618d2e; /* mov.w @r4,r0 */
L8c618d2e: r[13]=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;goto L8c618d30; /* mov.l @(0x14,r4),r13 */
L8c618d30: r[0]=r[0]&65535u;goto L8c618d32; /* extu.w r0,r0 */
L8c618d32: r[12]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c618d34; /* mov.l @(0x4,r4),r12 */
L8c618d34: r[0]&=r[3];goto L8c618d36; /* and r3,r0 */
L8c618d36: r[10]=rd(m,bus,&io_status,0x8c618dd8u,4);if(io_status)return io_status;goto L8c618d38; /* mov.l 0x8c618dd8,r10 */
L8c618d38: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c618d3a; /* cmp/eq #0x0,r0 */
L8c618d3a: take=(*sr&1u);r[11]=0u;if(take)goto L8c618d60;goto L8c618d3e; /* bt/s 0x8c618d60 */
L8c618d3e: r[1]=sx(m,bus,&io_status,0x8c618dd2u,2);if(io_status)return io_status;goto L8c618d40; /* mov.w 0x8c618dd2,r1 */
L8c618d40: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c618d42; /* cmp/eq r1,r0 */
L8c618d42: if(!(*sr&1u))goto L8c618d48;goto L8c618d44; /* bf 0x8c618d48 */
L8c618d44: goto L8c618f0c; /* bra 0x8c618f0c */
L8c618d48: r[1]=rd(m,bus,&io_status,0x8c618de0u,4);if(io_status)return io_status;goto L8c618d4a; /* mov.l 0x8c618de0,r1 */
L8c618d4a: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c618d4c; /* cmp/eq r1,r0 */
L8c618d4c: if(!(*sr&1u))goto L8c618d52;goto L8c618d4e; /* bf 0x8c618d52 */
L8c618d4e: goto L8c6190fe; /* bra 0x8c6190fe */
L8c618d52: r[1]=rd(m,bus,&io_status,0x8c618ddcu,4);if(io_status)return io_status;goto L8c618d54; /* mov.l 0x8c618ddc,r1 */
L8c618d54: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c618d56; /* cmp/eq r1,r0 */
L8c618d56: if(!(*sr&1u))goto L8c618d5c;goto L8c618d58; /* bf 0x8c618d5c */
L8c618d58: goto L8c619294; /* bra 0x8c619294 */
L8c618d5c: goto L8c6192d0; /* bra 0x8c6192d0 */
L8c618d60: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c618d62; /* mov.w @(0x2,r4),r0 */
L8c618d62: r[1]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c618d64; /* mov.l @r10,r1 */
L8c618d64: r[0]=r[0]&65535u;goto L8c618d66; /* extu.w r0,r0 */
L8c618d66: r[2]=r[0];goto L8c618d68; /* mov r0,r2 */
L8c618d68: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c618d6a; /* shll r0 */
L8c618d6a: r[0]+=r[2];goto L8c618d6c; /* add r2,r0 */
L8c618d6c: r[1]+=r[0];goto L8c618d6e; /* add r0,r1 */
L8c618d6e: wr(m,bus,&io_status,r[10],r[1],4);if(io_status)return io_status;goto L8c618d70; /* mov.l r1,@r10 */
L8c618d70: r[14]=r[11];goto L8c618efc; /* bra 0x8c618efc */
L8c618d74: r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c618d76; /* mov.w @r12+,r5 */
L8c618d76: r[0]=16u;goto L8c618d78; /* mov #0x10,r0 */
L8c618d78: r[3]=rd(m,bus,&io_status,0x8c618de4u,4);if(io_status)return io_status;goto L8c618d7a; /* mov.l 0x8c618de4,r3 */
L8c618d7a: r[6]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c618d7c; /* mov.w @r12+,r6 */
L8c618d7c: r[5]=r[5]&65535u;goto L8c618d7e; /* extu.w r5,r5 */
L8c618d7e: r[11]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c618d80; /* mov.l @r3,r11 */
L8c618d80: r[5]<<=2;goto L8c618d82; /* shll2 r5 */
L8c618d82: r[6]=r[6]&65535u;goto L8c618d84; /* extu.w r6,r6 */
L8c618d84: r[7]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c618d86; /* mov.w @r12+,r7 */
L8c618d86: r[6]<<=2;goto L8c618d88; /* shll2 r6 */
L8c618d88: r[6]<<=2;goto L8c618d8a; /* shll2 r6 */
L8c618d8a: r[5]<<=2;goto L8c618d8c; /* shll2 r5 */
L8c618d8c: r[7]=r[7]&65535u;goto L8c618d8e; /* extu.w r7,r7 */
L8c618d8e: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c618d90; /* shll r5 */
L8c618d90: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c618d92; /* shll r6 */
L8c618d92: r[6]+=r[11];goto L8c618d94; /* add r11,r6 */
L8c618d94: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c618d96; /* fmov.s @(r0,r6),fr4 */
L8c618d96: r[5]+=r[11];goto L8c618d98; /* add r11,r5 */
L8c618d98: r[7]<<=2;goto L8c618d9a; /* shll2 r7 */
L8c618d9a: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618d9c; /* fmov.s @(r0,r5),fr3 */
L8c618d9c: r[7]<<=2;goto L8c618d9e; /* shll2 r7 */
L8c618d9e: fcopy(f,2u,4u);goto L8c618da0; /* fmov fr4,fr2 */
L8c618da0: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c618da2; /* shll r7 */
L8c618da2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c618da4; /* fsub fr3,fr2 */
L8c618da4: r[0]=20u;goto L8c618da6; /* mov #0x14,r0 */
L8c618da6: r[7]+=r[11];goto L8c618da8; /* add r11,r7 */
L8c618da8: fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c618daa; /* fmov.s @(r0,r6),fr5 */
L8c618daa: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618dac; /* fmov.s @(r0,r7),fr3 */
L8c618dac: fcopy(f,1u,5u);goto L8c618dae; /* fmov fr5,fr1 */
L8c618dae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618db0; /* fsub fr5,fr3 */
L8c618db0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618db2; /* fmul fr3,fr2 */
L8c618db2: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618db4; /* fmov.s @(r0,r5),fr3 */
L8c618db4: r[0]=16u;goto L8c618db6; /* mov #0x10,r0 */
L8c618db6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c618db8; /* fsub fr3,fr1 */
L8c618db8: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618dba; /* fmov.s @(r0,r7),fr3 */
L8c618dba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618dbc; /* fsub fr4,fr3 */
L8c618dbc: fcopy(f,4u,2u);goto L8c618dbe; /* fmov fr2,fr4 */
L8c618dbe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618dc0; /* fmul fr3,fr1 */
L8c618dc0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c618dc2; /* fldi0 fr3 */
L8c618dc2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c618dc4; /* fsub fr1,fr4 */
L8c618dc4: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c618dc6; /* fcmp/gt fr3,fr4 */
L8c618dc6: take=!(*sr&1u);r[14]+=1u;if(take)goto L8c618de8;goto L8c618dca; /* bf/s 0x8c618de8 */
L8c618dca: r[13]+=12u;goto L8c618efc; /* bra 0x8c618efc */
L8c618de8: r[3]=rd(m,bus,&io_status,0x8c619108u,4);if(io_status)return io_status;goto L8c618dea; /* mov.l 0x8c619108,r3 */
L8c618dea: r[0]=4u;goto L8c618dec; /* mov #0x4,r0 */
L8c618dec: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c618dee; /* fmov.s @r5,fr3 */
L8c618dee: r[8]+=28u;goto L8c618df0; /* add #0x1c,r8 */
L8c618df0: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618df2; /* fmov.s @r3,fr2 */
L8c618df2: r[3]+=8u;goto L8c618df4; /* add #0x8,r3 */
L8c618df4: r[2]=rd(m,bus,&io_status,0x8c61910cu,4);if(io_status)return io_status;goto L8c618df6; /* mov.l 0x8c61910c,r2 */
L8c618df6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618df8; /* fmul fr3,fr2 */
L8c618df8: r[1]=rd(m,bus,&io_status,0x8c619110u,4);if(io_status)return io_status;goto L8c618dfa; /* mov.l 0x8c619110,r1 */
L8c618dfa: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618dfc; /* fmov.s @r2,fr3 */
L8c618dfc: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c618dfe; /* fmov.s @r1,fr1 */
L8c618dfe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618e00; /* fsub fr2,fr3 */
L8c618e00: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c618e02; /* fmov.s @(r0,r5),fr2 */
L8c618e02: r[0]=8u;goto L8c618e04; /* mov #0x8,r0 */
L8c618e04: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618e06; /* fmul fr2,fr1 */
L8c618e06: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c618e08; /* fmov.s @(r0,r5),fr2 */
L8c618e08: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618e0a; /* fsub fr1,fr3 */
L8c618e0a: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618e0c; /* fmov.s @r3,fr1 */
L8c618e0c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618e0e; /* fmul fr2,fr1 */
L8c618e0e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618e10; /* fsub fr1,fr3 */
L8c618e10: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e12; /* fmov.s fr3,@-r8 */
L8c618e12: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c618e14; /* mov.w @(0x2,r13),r0 */
L8c618e14: r[2]=r[0];goto L8c618e16; /* mov r0,r2 */
L8c618e16: *fpul=r[2];goto L8c618e18; /* lds r2,FPUL */
L8c618e18: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618e1a; /* float FPUL,fr3 */
L8c618e1a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618e1c; /* fmul fr6,fr3 */
L8c618e1c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e1e; /* fmov.s fr3,@-r8 */
L8c618e1e: r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c618e20; /* mov.w @r13,r2 */
L8c618e20: *fpul=r[2];goto L8c618e22; /* lds r2,FPUL */
L8c618e22: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618e24; /* float FPUL,fr3 */
L8c618e24: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618e26; /* fmul fr6,fr3 */
L8c618e26: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e28; /* fmov.s fr3,@-r8 */
L8c618e28: r[0]=24u;goto L8c618e2a; /* mov #0x18,r0 */
L8c618e2a: r[2]=rd(m,bus,&io_status,0x8c619114u,4);if(io_status)return io_status;goto L8c618e2c; /* mov.l 0x8c619114,r2 */
L8c618e2c: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618e2e; /* fmov.s @(r0,r5),fr3 */
L8c618e2e: r[0]=20u;goto L8c618e30; /* mov #0x14,r0 */
L8c618e30: r[13]+=4u;goto L8c618e32; /* add #0x4,r13 */
L8c618e32: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e34; /* fmov.s fr3,@-r8 */
L8c618e34: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618e36; /* fmov.s @(r0,r5),fr3 */
L8c618e36: r[0]=16u;goto L8c618e38; /* mov #0x10,r0 */
L8c618e38: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e3a; /* fmov.s fr3,@-r8 */
L8c618e3a: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618e3c; /* fmov.s @(r0,r5),fr3 */
L8c618e3c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e3e; /* fmov.s fr3,@-r8 */
L8c618e3e: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c618e40; /* mov.l r2,@-r8 */
L8c618e40: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618e42; /* pref @r8 */
L8c618e42: r[2]=rd(m,bus,&io_status,0x8c619108u,4);if(io_status)return io_status;goto L8c618e44; /* mov.l 0x8c619108,r2 */
L8c618e44: r[0]=4u;goto L8c618e46; /* mov #0x4,r0 */
L8c618e46: fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c618e48; /* fmov.s @r6,fr3 */
L8c618e48: r[8]+=60u;goto L8c618e4a; /* add #0x3c,r8 */
L8c618e4a: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618e4c; /* fmov.s @r2,fr2 */
L8c618e4c: r[3]=rd(m,bus,&io_status,0x8c61910cu,4);if(io_status)return io_status;goto L8c618e4e; /* mov.l 0x8c61910c,r3 */
L8c618e4e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618e50; /* fmul fr3,fr2 */
L8c618e50: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c618e52; /* fmov.s @r1,fr1 */
L8c618e52: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618e54; /* fmov.s @r3,fr3 */
L8c618e54: r[3]=rd(m,bus,&io_status,0x8c619118u,4);if(io_status)return io_status;goto L8c618e56; /* mov.l 0x8c619118,r3 */
L8c618e56: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618e58; /* fsub fr2,fr3 */
L8c618e58: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c618e5a; /* fmov.s @(r0,r6),fr2 */
L8c618e5a: r[0]=8u;goto L8c618e5c; /* mov #0x8,r0 */
L8c618e5c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618e5e; /* fmul fr2,fr1 */
L8c618e5e: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c618e60; /* fmov.s @(r0,r6),fr2 */
L8c618e60: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618e62; /* fsub fr1,fr3 */
L8c618e62: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618e64; /* fmov.s @r3,fr1 */
L8c618e64: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618e66; /* fmul fr2,fr1 */
L8c618e66: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618e68; /* fsub fr1,fr3 */
L8c618e68: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e6a; /* fmov.s fr3,@-r8 */
L8c618e6a: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c618e6c; /* mov.w @(0x2,r13),r0 */
L8c618e6c: r[2]=r[0];goto L8c618e6e; /* mov r0,r2 */
L8c618e6e: *fpul=r[2];goto L8c618e70; /* lds r2,FPUL */
L8c618e70: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618e72; /* float FPUL,fr3 */
L8c618e72: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618e74; /* fmul fr6,fr3 */
L8c618e74: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e76; /* fmov.s fr3,@-r8 */
L8c618e76: r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c618e78; /* mov.w @r13,r2 */
L8c618e78: r[13]+=4u;goto L8c618e7a; /* add #0x4,r13 */
L8c618e7a: *fpul=r[2];goto L8c618e7c; /* lds r2,FPUL */
L8c618e7c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618e7e; /* float FPUL,fr3 */
L8c618e7e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618e80; /* fmul fr6,fr3 */
L8c618e80: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e82; /* fmov.s fr3,@-r8 */
L8c618e82: r[0]=24u;goto L8c618e84; /* mov #0x18,r0 */
L8c618e84: r[2]=rd(m,bus,&io_status,0x8c619114u,4);if(io_status)return io_status;goto L8c618e86; /* mov.l 0x8c619114,r2 */
L8c618e86: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618e88; /* fmov.s @(r0,r6),fr3 */
L8c618e88: r[0]=20u;goto L8c618e8a; /* mov #0x14,r0 */
L8c618e8a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e8c; /* fmov.s fr3,@-r8 */
L8c618e8c: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618e8e; /* fmov.s @(r0,r6),fr3 */
L8c618e8e: r[0]=16u;goto L8c618e90; /* mov #0x10,r0 */
L8c618e90: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e92; /* fmov.s fr3,@-r8 */
L8c618e92: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618e94; /* fmov.s @(r0,r6),fr3 */
L8c618e94: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e96; /* fmov.s fr3,@-r8 */
L8c618e96: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c618e98; /* mov.l r2,@-r8 */
L8c618e98: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618e9a; /* pref @r8 */
L8c618e9a: r[2]=rd(m,bus,&io_status,0x8c619108u,4);if(io_status)return io_status;goto L8c618e9c; /* mov.l 0x8c619108,r2 */
L8c618e9c: r[0]=4u;goto L8c618e9e; /* mov #0x4,r0 */
L8c618e9e: fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c618ea0; /* fmov.s @r7,fr3 */
L8c618ea0: r[8]+=60u;goto L8c618ea2; /* add #0x3c,r8 */
L8c618ea2: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618ea4; /* fmov.s @r2,fr2 */
L8c618ea4: r[3]=rd(m,bus,&io_status,0x8c61910cu,4);if(io_status)return io_status;goto L8c618ea6; /* mov.l 0x8c61910c,r3 */
L8c618ea6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618ea8; /* fmul fr3,fr2 */
L8c618ea8: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c618eaa; /* fmov.s @r1,fr1 */
L8c618eaa: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618eac; /* fmov.s @r3,fr3 */
L8c618eac: r[3]=rd(m,bus,&io_status,0x8c619118u,4);if(io_status)return io_status;goto L8c618eae; /* mov.l 0x8c619118,r3 */
L8c618eae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618eb0; /* fsub fr2,fr3 */
L8c618eb0: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c618eb2; /* fmov.s @(r0,r7),fr2 */
L8c618eb2: r[0]=8u;goto L8c618eb4; /* mov #0x8,r0 */
L8c618eb4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618eb6; /* fmul fr2,fr1 */
L8c618eb6: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c618eb8; /* fmov.s @(r0,r7),fr2 */
L8c618eb8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618eba; /* fsub fr1,fr3 */
L8c618eba: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618ebc; /* fmov.s @r3,fr1 */
L8c618ebc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618ebe; /* fmul fr2,fr1 */
L8c618ebe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618ec0; /* fsub fr1,fr3 */
L8c618ec0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618ec2; /* fmov.s fr3,@-r8 */
L8c618ec2: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c618ec4; /* mov.w @(0x2,r13),r0 */
L8c618ec4: r[2]=r[0];goto L8c618ec6; /* mov r0,r2 */
L8c618ec6: *fpul=r[2];goto L8c618ec8; /* lds r2,FPUL */
L8c618ec8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618eca; /* float FPUL,fr3 */
L8c618eca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618ecc; /* fmul fr6,fr3 */
L8c618ecc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618ece; /* fmov.s fr3,@-r8 */
L8c618ece: r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c618ed0; /* mov.w @r13,r2 */
L8c618ed0: r[13]+=4u;goto L8c618ed2; /* add #0x4,r13 */
L8c618ed2: *fpul=r[2];goto L8c618ed4; /* lds r2,FPUL */
L8c618ed4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618ed6; /* float FPUL,fr3 */
L8c618ed6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618ed8; /* fmul fr6,fr3 */
L8c618ed8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618eda; /* fmov.s fr3,@-r8 */
L8c618eda: r[0]=24u;goto L8c618edc; /* mov #0x18,r0 */
L8c618edc: r[2]=rd(m,bus,&io_status,0x8c61911cu,4);if(io_status)return io_status;goto L8c618ede; /* mov.l 0x8c61911c,r2 */
L8c618ede: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618ee0; /* fmov.s @(r0,r7),fr3 */
L8c618ee0: r[0]=20u;goto L8c618ee2; /* mov #0x14,r0 */
L8c618ee2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618ee4; /* fmov.s fr3,@-r8 */
L8c618ee4: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618ee6; /* fmov.s @(r0,r7),fr3 */
L8c618ee6: r[0]=16u;goto L8c618ee8; /* mov #0x10,r0 */
L8c618ee8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618eea; /* fmov.s fr3,@-r8 */
L8c618eea: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618eec; /* fmov.s @(r0,r7),fr3 */
L8c618eec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618eee; /* fmov.s fr3,@-r8 */
L8c618eee: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c618ef0; /* mov.l r2,@-r8 */
L8c618ef0: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618ef2; /* pref @r8 */
L8c618ef2: r[0]=rd(m,bus,&io_status,0x8c619120u,4);if(io_status)return io_status;goto L8c618ef4; /* mov.l 0x8c619120,r0 */
L8c618ef4: r[8]+=32u;goto L8c618ef6; /* add #0x20,r8 */
L8c618ef6: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c618ef8; /* mov.l @r0,r2 */
L8c618ef8: r[2]+=3u;goto L8c618efa; /* add #0x3,r2 */
L8c618efa: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c618efc; /* mov.l r2,@r0 */
L8c618efc: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c618efe; /* mov.w @(0x2,r4),r0 */
L8c618efe: r[0]=r[0]&65535u;goto L8c618f00; /* extu.w r0,r0 */
L8c618f00: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c618f02; /* cmp/ge r0,r14 */
L8c618f02: if((*sr&1u))goto L8c618f08;goto L8c618f04; /* bt 0x8c618f08 */
L8c618f04: goto L8c618d74; /* bra 0x8c618d74 */
L8c618f08: goto L8c6192d0; /* bra 0x8c6192d0 */
L8c618f0c: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c618f0e; /* mov.w @(0x2,r4),r0 */
L8c618f0e: r[2]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c618f10; /* mov.l @r10,r2 */
L8c618f10: r[0]=r[0]&65535u;goto L8c618f12; /* extu.w r0,r0 */
L8c618f12: r[0]<<=2;goto L8c618f14; /* shll2 r0 */
L8c618f14: r[2]+=r[0];goto L8c618f16; /* add r0,r2 */
L8c618f16: wr(m,bus,&io_status,r[10],r[2],4);if(io_status)return io_status;goto L8c618f18; /* mov.l r2,@r10 */
L8c618f18: r[10]=r[11];goto L8c6190ee; /* bra 0x8c6190ee */
L8c618f1c: r[6]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c618f1e; /* mov.w @r12+,r6 */
L8c618f1e: r[0]=16u;goto L8c618f20; /* mov #0x10,r0 */
L8c618f20: r[3]=rd(m,bus,&io_status,0x8c619124u,4);if(io_status)return io_status;goto L8c618f22; /* mov.l 0x8c619124,r3 */
L8c618f22: r[7]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c618f24; /* mov.w @r12+,r7 */
L8c618f24: r[6]=r[6]&65535u;goto L8c618f26; /* extu.w r6,r6 */
L8c618f26: r[11]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c618f28; /* mov.l @r3,r11 */
L8c618f28: r[6]<<=2;goto L8c618f2a; /* shll2 r6 */
L8c618f2a: r[7]=r[7]&65535u;goto L8c618f2c; /* extu.w r7,r7 */
L8c618f2c: r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c618f2e; /* mov.w @r12+,r5 */
L8c618f2e: r[7]<<=2;goto L8c618f30; /* shll2 r7 */
L8c618f30: r[7]<<=2;goto L8c618f32; /* shll2 r7 */
L8c618f32: r[14]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c618f34; /* mov.w @r12+,r14 */
L8c618f34: r[6]<<=2;goto L8c618f36; /* shll2 r6 */
L8c618f36: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c618f38; /* shll r6 */
L8c618f38: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c618f3a; /* shll r7 */
L8c618f3a: r[7]+=r[11];goto L8c618f3c; /* add r11,r7 */
L8c618f3c: r[6]+=r[11];goto L8c618f3e; /* add r11,r6 */
L8c618f3e: fload(m,bus,&io_status,f,4u,r[0]+r[7]);if(io_status)return io_status;goto L8c618f40; /* fmov.s @(r0,r7),fr4 */
L8c618f40: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618f42; /* fmov.s @(r0,r6),fr3 */
L8c618f42: r[5]=r[5]&65535u;goto L8c618f44; /* extu.w r5,r5 */
L8c618f44: fcopy(f,2u,4u);goto L8c618f46; /* fmov fr4,fr2 */
L8c618f46: r[5]<<=2;goto L8c618f48; /* shll2 r5 */
L8c618f48: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c618f4a; /* fsub fr3,fr2 */
L8c618f4a: r[14]=r[14]&65535u;goto L8c618f4c; /* extu.w r14,r14 */
L8c618f4c: r[5]<<=2;goto L8c618f4e; /* shll2 r5 */
L8c618f4e: r[14]<<=2;goto L8c618f50; /* shll2 r14 */
L8c618f50: r[0]=20u;goto L8c618f52; /* mov #0x14,r0 */
L8c618f52: fload(m,bus,&io_status,f,5u,r[0]+r[7]);if(io_status)return io_status;goto L8c618f54; /* fmov.s @(r0,r7),fr5 */
L8c618f54: r[14]<<=2;goto L8c618f56; /* shll2 r14 */
L8c618f56: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c618f58; /* shll r5 */
L8c618f58: r[5]+=r[11];goto L8c618f5a; /* add r11,r5 */
L8c618f5a: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c618f5c; /* shll r14 */
L8c618f5c: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618f5e; /* fmov.s @(r0,r5),fr3 */
L8c618f5e: fcopy(f,1u,5u);goto L8c618f60; /* fmov fr5,fr1 */
L8c618f60: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618f62; /* fsub fr5,fr3 */
L8c618f62: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618f64; /* fmul fr3,fr2 */
L8c618f64: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618f66; /* fmov.s @(r0,r6),fr3 */
L8c618f66: r[0]=16u;goto L8c618f68; /* mov #0x10,r0 */
L8c618f68: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c618f6a; /* fsub fr3,fr1 */
L8c618f6a: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618f6c; /* fmov.s @(r0,r5),fr3 */
L8c618f6c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618f6e; /* fsub fr4,fr3 */
L8c618f6e: fcopy(f,4u,2u);goto L8c618f70; /* fmov fr2,fr4 */
L8c618f70: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618f72; /* fmul fr3,fr1 */
L8c618f72: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c618f74; /* fldi0 fr3 */
L8c618f74: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c618f76; /* fsub fr1,fr4 */
L8c618f76: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c618f78; /* fcmp/gt fr3,fr4 */
L8c618f78: take=!(*sr&1u);r[14]+=r[11];if(take)goto L8c618f80;goto L8c618f7c; /* bf/s 0x8c618f80 */
L8c618f7c: r[13]+=16u;goto L8c6190ec; /* bra 0x8c6190ec */
L8c618f80: r[3]=rd(m,bus,&io_status,0x8c619108u,4);if(io_status)return io_status;goto L8c618f82; /* mov.l 0x8c619108,r3 */
L8c618f82: r[0]=4u;goto L8c618f84; /* mov #0x4,r0 */
L8c618f84: fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c618f86; /* fmov.s @r6,fr3 */
L8c618f86: r[8]+=28u;goto L8c618f88; /* add #0x1c,r8 */
L8c618f88: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618f8a; /* fmov.s @r3,fr2 */
L8c618f8a: r[3]+=8u;goto L8c618f8c; /* add #0x8,r3 */
L8c618f8c: r[2]=rd(m,bus,&io_status,0x8c61910cu,4);if(io_status)return io_status;goto L8c618f8e; /* mov.l 0x8c61910c,r2 */
L8c618f8e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618f90; /* fmul fr3,fr2 */
L8c618f90: r[1]=rd(m,bus,&io_status,0x8c619110u,4);if(io_status)return io_status;goto L8c618f92; /* mov.l 0x8c619110,r1 */
L8c618f92: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618f94; /* fmov.s @r2,fr3 */
L8c618f94: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c618f96; /* fmov.s @r1,fr1 */
L8c618f96: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618f98; /* fsub fr2,fr3 */
L8c618f98: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c618f9a; /* fmov.s @(r0,r6),fr2 */
L8c618f9a: r[0]=8u;goto L8c618f9c; /* mov #0x8,r0 */
L8c618f9c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618f9e; /* fmul fr2,fr1 */
L8c618f9e: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c618fa0; /* fmov.s @(r0,r6),fr2 */
L8c618fa0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618fa2; /* fsub fr1,fr3 */
L8c618fa2: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618fa4; /* fmov.s @r3,fr1 */
L8c618fa4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618fa6; /* fmul fr2,fr1 */
L8c618fa6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618fa8; /* fsub fr1,fr3 */
L8c618fa8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618faa; /* fmov.s fr3,@-r8 */
L8c618faa: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c618fac; /* mov.w @(0x2,r13),r0 */
L8c618fac: r[2]=r[0];goto L8c618fae; /* mov r0,r2 */
L8c618fae: *fpul=r[2];goto L8c618fb0; /* lds r2,FPUL */
L8c618fb0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618fb2; /* float FPUL,fr3 */
L8c618fb2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618fb4; /* fmul fr6,fr3 */
L8c618fb4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618fb6; /* fmov.s fr3,@-r8 */
L8c618fb6: r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c618fb8; /* mov.w @r13,r2 */
L8c618fb8: *fpul=r[2];goto L8c618fba; /* lds r2,FPUL */
L8c618fba: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618fbc; /* float FPUL,fr3 */
L8c618fbc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618fbe; /* fmul fr6,fr3 */
L8c618fbe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618fc0; /* fmov.s fr3,@-r8 */
L8c618fc0: r[0]=24u;goto L8c618fc2; /* mov #0x18,r0 */
L8c618fc2: r[2]=rd(m,bus,&io_status,0x8c619114u,4);if(io_status)return io_status;goto L8c618fc4; /* mov.l 0x8c619114,r2 */
L8c618fc4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618fc6; /* fmov.s @(r0,r6),fr3 */
L8c618fc6: r[0]=20u;goto L8c618fc8; /* mov #0x14,r0 */
L8c618fc8: r[13]+=4u;goto L8c618fca; /* add #0x4,r13 */
L8c618fca: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618fcc; /* fmov.s fr3,@-r8 */
L8c618fcc: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618fce; /* fmov.s @(r0,r6),fr3 */
L8c618fce: r[0]=16u;goto L8c618fd0; /* mov #0x10,r0 */
L8c618fd0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618fd2; /* fmov.s fr3,@-r8 */
L8c618fd2: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618fd4; /* fmov.s @(r0,r6),fr3 */
L8c618fd4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618fd6; /* fmov.s fr3,@-r8 */
L8c618fd6: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c618fd8; /* mov.l r2,@-r8 */
L8c618fd8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618fda; /* pref @r8 */
L8c618fda: r[2]=rd(m,bus,&io_status,0x8c619108u,4);if(io_status)return io_status;goto L8c618fdc; /* mov.l 0x8c619108,r2 */
L8c618fdc: r[0]=4u;goto L8c618fde; /* mov #0x4,r0 */
L8c618fde: fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c618fe0; /* fmov.s @r7,fr3 */
L8c618fe0: r[8]+=60u;goto L8c618fe2; /* add #0x3c,r8 */
L8c618fe2: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618fe4; /* fmov.s @r2,fr2 */
L8c618fe4: r[3]=rd(m,bus,&io_status,0x8c61910cu,4);if(io_status)return io_status;goto L8c618fe6; /* mov.l 0x8c61910c,r3 */
L8c618fe6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618fe8; /* fmul fr3,fr2 */
L8c618fe8: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c618fea; /* fmov.s @r1,fr1 */
L8c618fea: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618fec; /* fmov.s @r3,fr3 */
L8c618fec: r[3]=rd(m,bus,&io_status,0x8c619118u,4);if(io_status)return io_status;goto L8c618fee; /* mov.l 0x8c619118,r3 */
L8c618fee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618ff0; /* fsub fr2,fr3 */
L8c618ff0: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c618ff2; /* fmov.s @(r0,r7),fr2 */
L8c618ff2: r[0]=8u;goto L8c618ff4; /* mov #0x8,r0 */
L8c618ff4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618ff6; /* fmul fr2,fr1 */
L8c618ff6: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c618ff8; /* fmov.s @(r0,r7),fr2 */
L8c618ff8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618ffa; /* fsub fr1,fr3 */
L8c618ffa: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618ffc; /* fmov.s @r3,fr1 */
L8c618ffc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618ffe; /* fmul fr2,fr1 */
L8c618ffe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619000; /* fsub fr1,fr3 */
L8c619000: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619002; /* fmov.s fr3,@-r8 */
L8c619002: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c619004; /* mov.w @(0x2,r13),r0 */
L8c619004: r[2]=r[0];goto L8c619006; /* mov r0,r2 */
L8c619006: *fpul=r[2];goto L8c619008; /* lds r2,FPUL */
L8c619008: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61900a; /* float FPUL,fr3 */
L8c61900a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61900c; /* fmul fr6,fr3 */
L8c61900c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61900e; /* fmov.s fr3,@-r8 */
L8c61900e: r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c619010; /* mov.w @r13,r2 */
L8c619010: r[13]+=4u;goto L8c619012; /* add #0x4,r13 */
L8c619012: *fpul=r[2];goto L8c619014; /* lds r2,FPUL */
L8c619014: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619016; /* float FPUL,fr3 */
L8c619016: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619018; /* fmul fr6,fr3 */
L8c619018: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61901a; /* fmov.s fr3,@-r8 */
L8c61901a: r[0]=24u;goto L8c61901c; /* mov #0x18,r0 */
L8c61901c: r[2]=rd(m,bus,&io_status,0x8c619114u,4);if(io_status)return io_status;goto L8c61901e; /* mov.l 0x8c619114,r2 */
L8c61901e: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619020; /* fmov.s @(r0,r7),fr3 */
L8c619020: r[0]=20u;goto L8c619022; /* mov #0x14,r0 */
L8c619022: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619024; /* fmov.s fr3,@-r8 */
L8c619024: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619026; /* fmov.s @(r0,r7),fr3 */
L8c619026: r[0]=16u;goto L8c619028; /* mov #0x10,r0 */
L8c619028: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61902a; /* fmov.s fr3,@-r8 */
L8c61902a: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61902c; /* fmov.s @(r0,r7),fr3 */
L8c61902c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61902e; /* fmov.s fr3,@-r8 */
L8c61902e: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c619030; /* mov.l r2,@-r8 */
L8c619030: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619032; /* pref @r8 */
L8c619032: r[2]=rd(m,bus,&io_status,0x8c619108u,4);if(io_status)return io_status;goto L8c619034; /* mov.l 0x8c619108,r2 */
L8c619034: r[0]=4u;goto L8c619036; /* mov #0x4,r0 */
L8c619036: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c619038; /* fmov.s @r5,fr3 */
L8c619038: r[8]+=60u;goto L8c61903a; /* add #0x3c,r8 */
L8c61903a: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61903c; /* fmov.s @r2,fr2 */
L8c61903c: r[3]=rd(m,bus,&io_status,0x8c61910cu,4);if(io_status)return io_status;goto L8c61903e; /* mov.l 0x8c61910c,r3 */
L8c61903e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619040; /* fmul fr3,fr2 */
L8c619040: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c619042; /* fmov.s @r1,fr1 */
L8c619042: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c619044; /* fmov.s @r3,fr3 */
L8c619044: r[3]=rd(m,bus,&io_status,0x8c619118u,4);if(io_status)return io_status;goto L8c619046; /* mov.l 0x8c619118,r3 */
L8c619046: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619048; /* fsub fr2,fr3 */
L8c619048: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61904a; /* fmov.s @(r0,r5),fr2 */
L8c61904a: r[0]=8u;goto L8c61904c; /* mov #0x8,r0 */
L8c61904c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61904e; /* fmul fr2,fr1 */
L8c61904e: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c619050; /* fmov.s @(r0,r5),fr2 */
L8c619050: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619052; /* fsub fr1,fr3 */
L8c619052: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619054; /* fmov.s @r3,fr1 */
L8c619054: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619056; /* fmul fr2,fr1 */
L8c619056: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619058; /* fsub fr1,fr3 */
L8c619058: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61905a; /* fmov.s fr3,@-r8 */
L8c61905a: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61905c; /* mov.w @(0x2,r13),r0 */
L8c61905c: r[2]=r[0];goto L8c61905e; /* mov r0,r2 */
L8c61905e: *fpul=r[2];goto L8c619060; /* lds r2,FPUL */
L8c619060: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619062; /* float FPUL,fr3 */
L8c619062: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619064; /* fmul fr6,fr3 */
L8c619064: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619066; /* fmov.s fr3,@-r8 */
L8c619066: r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c619068; /* mov.w @r13,r2 */
L8c619068: r[13]+=4u;goto L8c61906a; /* add #0x4,r13 */
L8c61906a: *fpul=r[2];goto L8c61906c; /* lds r2,FPUL */
L8c61906c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61906e; /* float FPUL,fr3 */
L8c61906e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619070; /* fmul fr6,fr3 */
L8c619070: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619072; /* fmov.s fr3,@-r8 */
L8c619072: r[0]=24u;goto L8c619074; /* mov #0x18,r0 */
L8c619074: r[2]=rd(m,bus,&io_status,0x8c619114u,4);if(io_status)return io_status;goto L8c619076; /* mov.l 0x8c619114,r2 */
L8c619076: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619078; /* fmov.s @(r0,r5),fr3 */
L8c619078: r[0]=20u;goto L8c61907a; /* mov #0x14,r0 */
L8c61907a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61907c; /* fmov.s fr3,@-r8 */
L8c61907c: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61907e; /* fmov.s @(r0,r5),fr3 */
L8c61907e: r[0]=16u;goto L8c619080; /* mov #0x10,r0 */
L8c619080: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619082; /* fmov.s fr3,@-r8 */
L8c619082: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619084; /* fmov.s @(r0,r5),fr3 */
L8c619084: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619086; /* fmov.s fr3,@-r8 */
L8c619086: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c619088; /* mov.l r2,@-r8 */
L8c619088: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61908a; /* pref @r8 */
L8c61908a: r[2]=rd(m,bus,&io_status,0x8c619108u,4);if(io_status)return io_status;goto L8c61908c; /* mov.l 0x8c619108,r2 */
L8c61908c: r[0]=4u;goto L8c61908e; /* mov #0x4,r0 */
L8c61908e: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c619090; /* fmov.s @r14,fr3 */
L8c619090: r[8]+=60u;goto L8c619092; /* add #0x3c,r8 */
L8c619092: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c619094; /* fmov.s @r2,fr2 */
L8c619094: r[3]=rd(m,bus,&io_status,0x8c61910cu,4);if(io_status)return io_status;goto L8c619096; /* mov.l 0x8c61910c,r3 */
L8c619096: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619098; /* fmul fr3,fr2 */
L8c619098: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61909a; /* fmov.s @r1,fr1 */
L8c61909a: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61909c; /* fmov.s @r3,fr3 */
L8c61909c: r[3]=rd(m,bus,&io_status,0x8c619118u,4);if(io_status)return io_status;goto L8c61909e; /* mov.l 0x8c619118,r3 */
L8c61909e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6190a0; /* fsub fr2,fr3 */
L8c6190a0: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c6190a2; /* fmov.s @(r0,r14),fr2 */
L8c6190a2: r[0]=8u;goto L8c6190a4; /* mov #0x8,r0 */
L8c6190a4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6190a6; /* fmul fr2,fr1 */
L8c6190a6: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c6190a8; /* fmov.s @(r0,r14),fr2 */
L8c6190a8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6190aa; /* fsub fr1,fr3 */
L8c6190aa: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6190ac; /* fmov.s @r3,fr1 */
L8c6190ac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6190ae; /* fmul fr2,fr1 */
L8c6190ae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6190b0; /* fsub fr1,fr3 */
L8c6190b0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6190b2; /* fmov.s fr3,@-r8 */
L8c6190b2: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c6190b4; /* mov.w @(0x2,r13),r0 */
L8c6190b4: r[2]=r[0];goto L8c6190b6; /* mov r0,r2 */
L8c6190b6: *fpul=r[2];goto L8c6190b8; /* lds r2,FPUL */
L8c6190b8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6190ba; /* float FPUL,fr3 */
L8c6190ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6190bc; /* fmul fr6,fr3 */
L8c6190bc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6190be; /* fmov.s fr3,@-r8 */
L8c6190be: r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c6190c0; /* mov.w @r13,r2 */
L8c6190c0: r[13]+=4u;goto L8c6190c2; /* add #0x4,r13 */
L8c6190c2: *fpul=r[2];goto L8c6190c4; /* lds r2,FPUL */
L8c6190c4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6190c6; /* float FPUL,fr3 */
L8c6190c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6190c8; /* fmul fr6,fr3 */
L8c6190c8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6190ca; /* fmov.s fr3,@-r8 */
L8c6190ca: r[0]=24u;goto L8c6190cc; /* mov #0x18,r0 */
L8c6190cc: r[2]=rd(m,bus,&io_status,0x8c61911cu,4);if(io_status)return io_status;goto L8c6190ce; /* mov.l 0x8c61911c,r2 */
L8c6190ce: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6190d0; /* fmov.s @(r0,r14),fr3 */
L8c6190d0: r[0]=20u;goto L8c6190d2; /* mov #0x14,r0 */
L8c6190d2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6190d4; /* fmov.s fr3,@-r8 */
L8c6190d4: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6190d6; /* fmov.s @(r0,r14),fr3 */
L8c6190d6: r[0]=16u;goto L8c6190d8; /* mov #0x10,r0 */
L8c6190d8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6190da; /* fmov.s fr3,@-r8 */
L8c6190da: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6190dc; /* fmov.s @(r0,r14),fr3 */
L8c6190dc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6190de; /* fmov.s fr3,@-r8 */
L8c6190de: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c6190e0; /* mov.l r2,@-r8 */
L8c6190e0: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6190e2; /* pref @r8 */
L8c6190e2: r[0]=rd(m,bus,&io_status,0x8c619120u,4);if(io_status)return io_status;goto L8c6190e4; /* mov.l 0x8c619120,r0 */
L8c6190e4: r[8]+=32u;goto L8c6190e6; /* add #0x20,r8 */
L8c6190e6: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c6190e8; /* mov.l @r0,r2 */
L8c6190e8: r[2]+=4u;goto L8c6190ea; /* add #0x4,r2 */
L8c6190ea: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c6190ec; /* mov.l r2,@r0 */
L8c6190ec: r[10]+=1u;goto L8c6190ee; /* add #0x1,r10 */
L8c6190ee: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c6190f0; /* mov.w @(0x2,r4),r0 */
L8c6190f0: r[0]=r[0]&65535u;goto L8c6190f2; /* extu.w r0,r0 */
L8c6190f2: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c6190f4; /* cmp/ge r0,r10 */
L8c6190f4: if((*sr&1u))goto L8c6190fa;goto L8c6190f6; /* bt 0x8c6190fa */
L8c6190f6: goto L8c618f1c; /* bra 0x8c618f1c */
L8c6190fa: goto L8c6192d0; /* bra 0x8c6192d0 */
L8c6190fe: r[9]=r[11];goto L8c619284; /* bra 0x8c619284 */
L8c619128: r[14]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61912a; /* mov.w @r12+,r14 */
L8c61912a: r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c61912c; /* mov.l @r10,r3 */
L8c61912c: r[14]=r[14]&65535u;goto L8c61912e; /* extu.w r14,r14 */
L8c61912e: r[2]=rd(m,bus,&io_status,0x8c6192ecu,4);if(io_status)return io_status;goto L8c619130; /* mov.l 0x8c6192ec,r2 */
L8c619130: r[3]+=r[14];goto L8c619132; /* add r14,r3 */
L8c619132: wr(m,bus,&io_status,r[10],r[3],4);if(io_status)return io_status;goto L8c619134; /* mov.l r3,@r10 */
L8c619134: r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c619136; /* mov.w @(0x2,r12),r0 */
L8c619136: r[7]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;goto L8c619138; /* mov.w @r12,r7 */
L8c619138: r[5]=r[0]&65535u;goto L8c61913a; /* extu.w r0,r5 */
L8c61913a: r[0]=sx(m,bus,&io_status,r[12]+0x4u,2);if(io_status)return io_status;goto L8c61913c; /* mov.w @(0x4,r12),r0 */
L8c61913c: r[5]<<=2;goto L8c61913e; /* shll2 r5 */
L8c61913e: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c619140; /* mov.l @r2,r3 */
L8c619140: r[7]=r[7]&65535u;goto L8c619142; /* extu.w r7,r7 */
L8c619142: r[7]<<=2;goto L8c619144; /* shll2 r7 */
L8c619144: r[5]<<=2;goto L8c619146; /* shll2 r5 */
L8c619146: r[6]=r[0]&65535u;goto L8c619148; /* extu.w r0,r6 */
L8c619148: r[7]<<=2;goto L8c61914a; /* shll2 r7 */
L8c61914a: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61914c; /* shll r7 */
L8c61914c: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61914e; /* shll r5 */
L8c61914e: r[0]=16u;goto L8c619150; /* mov #0x10,r0 */
L8c619150: r[5]+=r[3];goto L8c619152; /* add r3,r5 */
L8c619152: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c619154; /* fmov.s @(r0,r5),fr5 */
L8c619154: r[7]+=r[3];goto L8c619156; /* add r3,r7 */
L8c619156: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619158; /* fmov.s @(r0,r7),fr3 */
L8c619158: r[6]<<=2;goto L8c61915a; /* shll2 r6 */
L8c61915a: fcopy(f,2u,5u);goto L8c61915c; /* fmov fr5,fr2 */
L8c61915c: r[6]<<=2;goto L8c61915e; /* shll2 r6 */
L8c61915e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c619160; /* fsub fr3,fr2 */
L8c619160: r[0]=20u;goto L8c619162; /* mov #0x14,r0 */
L8c619162: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c619164; /* shll r6 */
L8c619164: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c619166; /* fmov.s @(r0,r5),fr4 */
L8c619166: r[6]+=r[3];goto L8c619168; /* add r3,r6 */
L8c619168: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61916a; /* fmov.s @(r0,r6),fr3 */
L8c61916a: fcopy(f,1u,4u);goto L8c61916c; /* fmov fr4,fr1 */
L8c61916c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61916e; /* fsub fr4,fr3 */
L8c61916e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619170; /* fmul fr3,fr2 */
L8c619170: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619172; /* fmov.s @(r0,r7),fr3 */
L8c619172: r[0]=16u;goto L8c619174; /* mov #0x10,r0 */
L8c619174: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c619176; /* fsub fr3,fr1 */
L8c619176: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619178; /* fmov.s @(r0,r6),fr3 */
L8c619178: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61917a; /* fsub fr5,fr3 */
L8c61917a: fcopy(f,4u,2u);goto L8c61917c; /* fmov fr2,fr4 */
L8c61917c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61917e; /* fmul fr3,fr1 */
L8c61917e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c619180; /* fldi0 fr3 */
L8c619180: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c619182; /* fsub fr1,fr4 */
L8c619182: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c619184; /* fcmp/gt fr3,fr4 */
L8c619184: if(!(*sr&1u))goto L8c619192;goto L8c619186; /* bf 0x8c619192 */
L8c619186: r[1]=r[14];goto L8c619188; /* mov r14,r1 */
L8c619188: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c61918a; /* shll r1 */
L8c61918a: r[14]<<=2;goto L8c61918c; /* shll2 r14 */
L8c61918c: r[12]+=r[1];goto L8c61918e; /* add r1,r12 */
L8c61918e: r[13]+=r[14];goto L8c619282; /* bra 0x8c619282 */
L8c619192: r[2]=rd(m,bus,&io_status,0x8c6192f0u,4);if(io_status)return io_status;goto L8c619194; /* mov.l 0x8c6192f0,r2 */
L8c619194: r[7]=r[14];goto L8c619196; /* mov r14,r7 */
L8c619196: r[7]+=4294967295u;goto L8c619198; /* add #-0x1,r7 */
L8c619198: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61919a; /* mov.l @r2,r3 */
L8c61919a: *sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c61919c; /* cmp/pl r7 */
L8c61919c: r[3]+=r[14];goto L8c61919e; /* add r14,r3 */
L8c61919e: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6191a0; /* mov.l r3,@r2 */
L8c6191a0: take=!(*sr&1u);r[6]=r[11];if(take)goto L8c619216;goto L8c6191a4; /* bf/s 0x8c619216 */
L8c6191a4: r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c6191a6; /* mov.w @r12+,r5 */
L8c6191a6: r[0]=4u;goto L8c6191a8; /* mov #0x4,r0 */
L8c6191a8: r[2]=rd(m,bus,&io_status,0x8c6192ecu,4);if(io_status)return io_status;goto L8c6191aa; /* mov.l 0x8c6192ec,r2 */
L8c6191aa: r[8]+=28u;goto L8c6191ac; /* add #0x1c,r8 */
L8c6191ac: r[5]=r[5]&65535u;goto L8c6191ae; /* extu.w r5,r5 */
L8c6191ae: r[1]=rd(m,bus,&io_status,0x8c6192f8u,4);if(io_status)return io_status;goto L8c6191b0; /* mov.l 0x8c6192f8,r1 */
L8c6191b0: r[5]<<=2;goto L8c6191b2; /* shll2 r5 */
L8c6191b2: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6191b4; /* mov.l @r2,r3 */
L8c6191b4: r[5]<<=2;goto L8c6191b6; /* shll2 r5 */
L8c6191b6: r[2]=rd(m,bus,&io_status,0x8c6192fcu,4);if(io_status)return io_status;goto L8c6191b8; /* mov.l 0x8c6192fc,r2 */
L8c6191b8: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6191ba; /* shll r5 */
L8c6191ba: r[5]+=r[3];goto L8c6191bc; /* add r3,r5 */
L8c6191bc: r[3]=rd(m,bus,&io_status,0x8c6192f4u,4);if(io_status)return io_status;goto L8c6191be; /* mov.l 0x8c6192f4,r3 */
L8c6191be: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c6191c0; /* fmov.s @r5,fr3 */
L8c6191c0: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6191c2; /* fmov.s @r3,fr2 */
L8c6191c2: r[3]+=8u;goto L8c6191c4; /* add #0x8,r3 */
L8c6191c4: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c6191c6; /* fmov.s @r2,fr1 */
L8c6191c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6191c8; /* fmul fr3,fr2 */
L8c6191c8: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6191ca; /* fmov.s @r1,fr3 */
L8c6191ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6191cc; /* fsub fr2,fr3 */
L8c6191cc: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c6191ce; /* fmov.s @(r0,r5),fr2 */
L8c6191ce: r[0]=8u;goto L8c6191d0; /* mov #0x8,r0 */
L8c6191d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6191d2; /* fmul fr2,fr1 */
L8c6191d2: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c6191d4; /* fmov.s @(r0,r5),fr2 */
L8c6191d4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6191d6; /* fsub fr1,fr3 */
L8c6191d6: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6191d8; /* fmov.s @r3,fr1 */
L8c6191d8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6191da; /* fmul fr2,fr1 */
L8c6191da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6191dc; /* fsub fr1,fr3 */
L8c6191dc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6191de; /* fmov.s fr3,@-r8 */
L8c6191de: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c6191e0; /* mov.w @(0x2,r13),r0 */
L8c6191e0: r[2]=r[0];goto L8c6191e2; /* mov r0,r2 */
L8c6191e2: *fpul=r[2];goto L8c6191e4; /* lds r2,FPUL */
L8c6191e4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6191e6; /* float FPUL,fr3 */
L8c6191e6: r[0]=24u;goto L8c6191e8; /* mov #0x18,r0 */
L8c6191e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6191ea; /* fmul fr6,fr3 */
L8c6191ea: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6191ec; /* fmov.s fr3,@-r8 */
L8c6191ec: r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c6191ee; /* mov.w @r13,r2 */
L8c6191ee: r[13]+=4u;goto L8c6191f0; /* add #0x4,r13 */
L8c6191f0: *fpul=r[2];goto L8c6191f2; /* lds r2,FPUL */
L8c6191f2: r[2]=rd(m,bus,&io_status,0x8c619300u,4);if(io_status)return io_status;goto L8c6191f4; /* mov.l 0x8c619300,r2 */
L8c6191f4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6191f6; /* float FPUL,fr3 */
L8c6191f6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6191f8; /* fmul fr6,fr3 */
L8c6191f8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6191fa; /* fmov.s fr3,@-r8 */
L8c6191fa: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6191fc; /* fmov.s @(r0,r5),fr3 */
L8c6191fc: r[0]=20u;goto L8c6191fe; /* mov #0x14,r0 */
L8c6191fe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619200; /* fmov.s fr3,@-r8 */
L8c619200: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619202; /* fmov.s @(r0,r5),fr3 */
L8c619202: r[0]=16u;goto L8c619204; /* mov #0x10,r0 */
L8c619204: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619206; /* fmov.s fr3,@-r8 */
L8c619206: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619208; /* fmov.s @(r0,r5),fr3 */
L8c619208: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61920a; /* fmov.s fr3,@-r8 */
L8c61920a: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61920c; /* mov.l r2,@-r8 */
L8c61920c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61920e; /* pref @r8 */
L8c61920e: r[6]+=1u;goto L8c619210; /* add #0x1,r6 */
L8c619210: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c619212; /* cmp/ge r7,r6 */
L8c619212: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c6191a4;goto L8c619216; /* bf/s 0x8c6191a4 */
L8c619216: r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c619218; /* mov.w @r12+,r5 */
L8c619218: r[0]=4u;goto L8c61921a; /* mov #0x4,r0 */
L8c61921a: r[2]=rd(m,bus,&io_status,0x8c6192ecu,4);if(io_status)return io_status;goto L8c61921c; /* mov.l 0x8c6192ec,r2 */
L8c61921c: r[8]+=28u;goto L8c61921e; /* add #0x1c,r8 */
L8c61921e: r[5]=r[5]&65535u;goto L8c619220; /* extu.w r5,r5 */
L8c619220: r[1]=rd(m,bus,&io_status,0x8c6192f8u,4);if(io_status)return io_status;goto L8c619222; /* mov.l 0x8c6192f8,r1 */
L8c619222: r[5]<<=2;goto L8c619224; /* shll2 r5 */
L8c619224: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c619226; /* mov.l @r2,r3 */
L8c619226: r[5]<<=2;goto L8c619228; /* shll2 r5 */
L8c619228: r[2]=rd(m,bus,&io_status,0x8c6192fcu,4);if(io_status)return io_status;goto L8c61922a; /* mov.l 0x8c6192fc,r2 */
L8c61922a: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61922c; /* shll r5 */
L8c61922c: r[5]+=r[3];goto L8c61922e; /* add r3,r5 */
L8c61922e: r[3]=rd(m,bus,&io_status,0x8c6192f4u,4);if(io_status)return io_status;goto L8c619230; /* mov.l 0x8c6192f4,r3 */
L8c619230: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c619232; /* fmov.s @r5,fr3 */
L8c619232: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c619234; /* fmov.s @r3,fr2 */
L8c619234: r[3]+=8u;goto L8c619236; /* add #0x8,r3 */
L8c619236: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c619238; /* fmov.s @r2,fr1 */
L8c619238: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61923a; /* fmul fr3,fr2 */
L8c61923a: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61923c; /* fmov.s @r1,fr3 */
L8c61923c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61923e; /* fsub fr2,fr3 */
L8c61923e: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c619240; /* fmov.s @(r0,r5),fr2 */
L8c619240: r[0]=8u;goto L8c619242; /* mov #0x8,r0 */
L8c619242: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619244; /* fmul fr2,fr1 */
L8c619244: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c619246; /* fmov.s @(r0,r5),fr2 */
L8c619246: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619248; /* fsub fr1,fr3 */
L8c619248: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61924a; /* fmov.s @r3,fr1 */
L8c61924a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61924c; /* fmul fr2,fr1 */
L8c61924c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61924e; /* fsub fr1,fr3 */
L8c61924e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619250; /* fmov.s fr3,@-r8 */
L8c619250: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c619252; /* mov.w @(0x2,r13),r0 */
L8c619252: r[2]=r[0];goto L8c619254; /* mov r0,r2 */
L8c619254: *fpul=r[2];goto L8c619256; /* lds r2,FPUL */
L8c619256: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619258; /* float FPUL,fr3 */
L8c619258: r[0]=24u;goto L8c61925a; /* mov #0x18,r0 */
L8c61925a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61925c; /* fmul fr6,fr3 */
L8c61925c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61925e; /* fmov.s fr3,@-r8 */
L8c61925e: r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c619260; /* mov.w @r13,r2 */
L8c619260: r[13]+=4u;goto L8c619262; /* add #0x4,r13 */
L8c619262: *fpul=r[2];goto L8c619264; /* lds r2,FPUL */
L8c619264: r[2]=rd(m,bus,&io_status,0x8c619304u,4);if(io_status)return io_status;goto L8c619266; /* mov.l 0x8c619304,r2 */
L8c619266: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619268; /* float FPUL,fr3 */
L8c619268: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61926a; /* fmul fr6,fr3 */
L8c61926a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61926c; /* fmov.s fr3,@-r8 */
L8c61926c: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61926e; /* fmov.s @(r0,r5),fr3 */
L8c61926e: r[0]=20u;goto L8c619270; /* mov #0x14,r0 */
L8c619270: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619272; /* fmov.s fr3,@-r8 */
L8c619272: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619274; /* fmov.s @(r0,r5),fr3 */
L8c619274: r[0]=16u;goto L8c619276; /* mov #0x10,r0 */
L8c619276: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619278; /* fmov.s fr3,@-r8 */
L8c619278: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61927a; /* fmov.s @(r0,r5),fr3 */
L8c61927a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61927c; /* fmov.s fr3,@-r8 */
L8c61927c: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61927e; /* mov.l r2,@-r8 */
L8c61927e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619280; /* pref @r8 */
L8c619280: r[8]+=32u;goto L8c619282; /* add #0x20,r8 */
L8c619282: r[9]+=1u;goto L8c619284; /* add #0x1,r9 */
L8c619284: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c619286; /* mov.w @(0x2,r4),r0 */
L8c619286: r[0]=r[0]&65535u;goto L8c619288; /* extu.w r0,r0 */
L8c619288: *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61928a; /* cmp/ge r0,r9 */
L8c61928a: if((*sr&1u))goto L8c619290;goto L8c61928c; /* bt 0x8c619290 */
L8c61928c: goto L8c619128; /* bra 0x8c619128 */
L8c619290: goto L8c6192d0; /* bra 0x8c6192d0 */
L8c619294: r[1]=rd(m,bus,&io_status,0x8c619308u,4);if(io_status)return io_status;goto L8c619296; /* mov.l 0x8c619308,r1 */
L8c619296: wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c619298; /* mov.l r4,@r1 */
L8c619298: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61929a; /* frchg */
L8c61929a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61929c; /* fmov.s fr0,@-r15 */
L8c61929c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61929e; /* fmov.s fr1,@-r15 */
L8c61929e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c6192a0; /* fmov.s fr2,@-r15 */
L8c6192a0: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c6192a2; /* fmov.s fr3,@-r15 */
L8c6192a2: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c6192a4; /* fmov.s fr4,@-r15 */
L8c6192a4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c6192a6; /* fmov.s fr5,@-r15 */
L8c6192a6: r[0]=rd(m,bus,&io_status,0x8c6192e0u,4);if(io_status)return io_status;goto L8c6192a8; /* mov.l 0x8c6192e0,r0 */
L8c6192a8: fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192aa; /* fmov.s @r0+,fr0 */
L8c6192aa: fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192ac; /* fmov.s @r0+,fr1 */
L8c6192ac: fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c6192ae; /* fmov.s @r0,fr2 */
L8c6192ae: r[0]=rd(m,bus,&io_status,0x8c6192e4u,4);if(io_status)return io_status;goto L8c6192b0; /* mov.l 0x8c6192e4,r0 */
L8c6192b0: fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c6192b2; /* fmov.s @r0,fr3 */
L8c6192b2: r[0]=0x8c6192e8u;goto L8c6192b4; /* mova 0x8c6192e8,r0 */
L8c6192b4: fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c6192b6; /* fmov.s @r0,fr4 */
L8c6192b6: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6192b8; /* frchg */
L8c6192b8: r[2]=rd(m,bus,&io_status,0x8c61930cu,4);if(io_status)return io_status;goto L8c6192ba; /* mov.l 0x8c61930c,r2 */
L8c6192ba: r[3]=rd(m,bus,&io_status,0x8c619308u,4);if(io_status)return io_status;goto L8c6192bc; /* mov.l 0x8c619308,r3 */
L8c6192bc: target=r[2];*pr=0x8c6192c0u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6192c0; /* jsr @r2 */
L8c6192c0: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6192c2; /* frchg */
L8c6192c2: fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192c4; /* fmov.s @r15+,fr5 */
L8c6192c4: fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192c6; /* fmov.s @r15+,fr4 */
L8c6192c6: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192c8; /* fmov.s @r15+,fr3 */
L8c6192c8: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192ca; /* fmov.s @r15+,fr2 */
L8c6192ca: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192cc; /* fmov.s @r15+,fr1 */
L8c6192cc: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192ce; /* fmov.s @r15+,fr0 */
L8c6192ce: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6192d0; /* frchg */
L8c6192d0: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6192d2; /* lds.l @r15+,PR */
L8c6192d2: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6192d4; /* mov.l @r15+,r9 */
L8c6192d4: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6192d6; /* mov.l @r15+,r10 */
L8c6192d6: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6192d8; /* mov.l @r15+,r11 */
L8c6192d8: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6192da; /* mov.l @r15+,r12 */
L8c6192da: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6192dc; /* mov.l @r15+,r13 */
L8c6192dc: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_uv_lit_primitive_clipped_8c619310(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c619310;
L8c619310: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c619312; /* mov.l r14,@-r15 */
L8c619312: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c619314; /* mov.l r13,@-r15 */
L8c619314: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c619316; /* mov.l r12,@-r15 */
L8c619316: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c619318; /* mov.l r11,@-r15 */
L8c619318: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61931a; /* mov.l r10,@-r15 */
L8c61931a: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61931c; /* mov.l r9,@-r15 */
L8c61931c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61931e; /* fmov.s fr15,@-r15 */
L8c61931e: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c619320; /* sts.l PR,@-r15 */
L8c619320: r[15]+=4294967272u;goto L8c619322; /* add #-0x18,r15 */
L8c619322: r[0]=0x8c6193fcu;goto L8c619324; /* mova 0x8c6193fc,r0 */
L8c619324: r[3]=rd(m,bus,&io_status,0x8c619400u,4);if(io_status)return io_status;goto L8c619326; /* mov.l 0x8c619400,r3 */
L8c619326: fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c619328; /* fmov.s @r0,fr15 */
L8c619328: wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c61932a; /* mov.l r4,@(0x8,r15) */
L8c61932a: r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61932c; /* mov.w @r4,r0 */
L8c61932c: r[11]=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;goto L8c61932e; /* mov.l @(0x14,r4),r11 */
L8c61932e: r[0]=r[0]&65535u;goto L8c619330; /* extu.w r0,r0 */
L8c619330: r[0]&=r[3];goto L8c619332; /* and r3,r0 */
L8c619332: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c619334; /* cmp/eq #0x0,r0 */
L8c619334: take=(*sr&1u);r[10]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;if(take)goto L8c61935a;goto L8c619338; /* bt/s 0x8c61935a */
L8c619338: r[1]=sx(m,bus,&io_status,0x8c6193fau,2);if(io_status)return io_status;goto L8c61933a; /* mov.w 0x8c6193fa,r1 */
L8c61933a: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61933c; /* cmp/eq r1,r0 */
L8c61933c: if(!(*sr&1u))goto L8c619342;goto L8c61933e; /* bf 0x8c619342 */
L8c61933e: goto L8c6195a0; /* bra 0x8c6195a0 */
L8c619342: r[1]=rd(m,bus,&io_status,0x8c619404u,4);if(io_status)return io_status;goto L8c619344; /* mov.l 0x8c619404,r1 */
L8c619344: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c619346; /* cmp/eq r1,r0 */
L8c619346: if(!(*sr&1u))goto L8c61934c;goto L8c619348; /* bf 0x8c61934c */
L8c619348: goto L8c6198d2; /* bra 0x8c6198d2 */
L8c61934c: r[1]=rd(m,bus,&io_status,0x8c619400u,4);if(io_status)return io_status;goto L8c61934e; /* mov.l 0x8c619400,r1 */
L8c61934e: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c619350; /* cmp/eq r1,r0 */
L8c619350: if(!(*sr&1u))goto L8c619356;goto L8c619352; /* bf 0x8c619356 */
L8c619352: goto L8c619ba4; /* bra 0x8c619ba4 */
L8c619356: goto L8c619be2; /* bra 0x8c619be2 */
L8c61935a: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61935c; /* mov.l @(0x8,r15),r2 */
L8c61935c: r[13]=0u;goto L8c61935e; /* mov #0x0,r13 */
L8c61935e: r[1]=rd(m,bus,&io_status,0x8c619408u,4);if(io_status)return io_status;goto L8c619360; /* mov.l 0x8c619408,r1 */
L8c619360: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c619362; /* mov.w @(0x2,r2),r0 */
L8c619362: r[12]=rd(m,bus,&io_status,0x8c61940cu,4);if(io_status)return io_status;goto L8c619364; /* mov.l 0x8c61940c,r12 */
L8c619364: r[0]=r[0]&65535u;goto L8c619366; /* extu.w r0,r0 */
L8c619366: r[14]=rd(m,bus,&io_status,0x8c619410u,4);if(io_status)return io_status;goto L8c619368; /* mov.l 0x8c619410,r14 */
L8c619368: r[2]=r[0];goto L8c61936a; /* mov r0,r2 */
L8c61936a: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61936c; /* shll r0 */
L8c61936c: r[0]+=r[2];goto L8c61936e; /* add r2,r0 */
L8c61936e: r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c619370; /* mov.l @r1,r2 */
L8c619370: r[9]=r[12];goto L8c619372; /* mov r12,r9 */
L8c619372: r[2]+=r[0];goto L8c619374; /* add r0,r2 */
L8c619374: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c619376; /* mov.l r2,@r1 */
L8c619376: r[9]+=4u;goto L8c61958e; /* bra 0x8c61958e */
L8c61937a: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61937c; /* mov.w @r10+,r4 */
L8c61937c: r[2]=rd(m,bus,&io_status,0x8c619414u,4);if(io_status)return io_status;goto L8c61937e; /* mov.l 0x8c619414,r2 */
L8c61937e: r[5]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c619380; /* mov.w @r10+,r5 */
L8c619380: r[4]=r[4]&65535u;goto L8c619382; /* extu.w r4,r4 */
L8c619382: r[4]<<=2;goto L8c619384; /* shll2 r4 */
L8c619384: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c619386; /* mov.l @r2,r3 */
L8c619386: r[6]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c619388; /* mov.w @r10+,r6 */
L8c619388: r[5]=r[5]&65535u;goto L8c61938a; /* extu.w r5,r5 */
L8c61938a: r[5]<<=2;goto L8c61938c; /* shll2 r5 */
L8c61938c: r[6]=r[6]&65535u;goto L8c61938e; /* extu.w r6,r6 */
L8c61938e: r[6]<<=2;goto L8c619390; /* shll2 r6 */
L8c619390: r[4]<<=2;goto L8c619392; /* shll2 r4 */
L8c619392: r[6]<<=2;goto L8c619394; /* shll2 r6 */
L8c619394: r[5]<<=2;goto L8c619396; /* shll2 r5 */
L8c619396: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c619398; /* shll r4 */
L8c619398: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61939a; /* shll r5 */
L8c61939a: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61939c; /* shll r6 */
L8c61939c: r[6]+=r[3];goto L8c61939e; /* add r3,r6 */
L8c61939e: r[4]+=r[3];goto L8c6193a0; /* add r3,r4 */
L8c6193a0: r[1]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c6193a2; /* mov.l @(0xc,r6),r1 */
L8c6193a2: r[5]+=r[3];goto L8c6193a4; /* add r3,r5 */
L8c6193a4: r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6193a6; /* mov.l @(0xc,r4),r7 */
L8c6193a6: r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c6193a8; /* mov.l @(0xc,r5),r3 */
L8c6193a8: r[7]&=r[3];goto L8c6193aa; /* and r3,r7 */
L8c6193aa: r[3]=63u;goto L8c6193ac; /* mov #0x3f,r3 */
L8c6193ac: r[7]&=r[1];goto L8c6193ae; /* and r1,r7 */
L8c6193ae: *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c6193b0; /* tst r3,r7 */
L8c6193b0: if(!(*sr&1u))goto L8c6193f2;goto L8c6193b2; /* bf 0x8c6193f2 */
L8c6193b2: r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6193b4; /* mov.l @(0xc,r4),r7 */
L8c6193b4: r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c6193b6; /* mov.l @(0xc,r5),r3 */
L8c6193b6: r[2]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c6193b8; /* mov.l @(0xc,r6),r2 */
L8c6193b8: r[7]|=r[3];goto L8c6193ba; /* or r3,r7 */
L8c6193ba: r[3]=1u;goto L8c6193bc; /* mov #0x1,r3 */
L8c6193bc: r[7]|=r[2];goto L8c6193be; /* or r2,r7 */
L8c6193be: *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c6193c0; /* tst r3,r7 */
L8c6193c0: if((*sr&1u))goto L8c6193c6;goto L8c6193c2; /* bt 0x8c6193c6 */
L8c6193c2: goto L8c61952e; /* bra 0x8c61952e */
L8c6193c6: r[0]=16u;goto L8c6193c8; /* mov #0x10,r0 */
L8c6193c8: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c6193ca; /* fmov.s @(r0,r5),fr5 */
L8c6193ca: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6193cc; /* fmov.s @(r0,r4),fr3 */
L8c6193cc: r[0]=20u;goto L8c6193ce; /* mov #0x14,r0 */
L8c6193ce: fcopy(f,2u,5u);goto L8c6193d0; /* fmov fr5,fr2 */
L8c6193d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6193d2; /* fsub fr3,fr2 */
L8c6193d2: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6193d4; /* fmov.s @(r0,r6),fr3 */
L8c6193d4: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c6193d6; /* fmov.s @(r0,r5),fr4 */
L8c6193d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6193d8; /* fsub fr4,fr3 */
L8c6193d8: fcopy(f,1u,4u);goto L8c6193da; /* fmov fr4,fr1 */
L8c6193da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6193dc; /* fmul fr3,fr2 */
L8c6193dc: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6193de; /* fmov.s @(r0,r4),fr3 */
L8c6193de: r[0]=16u;goto L8c6193e0; /* mov #0x10,r0 */
L8c6193e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6193e2; /* fsub fr3,fr1 */
L8c6193e2: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6193e4; /* fmov.s @(r0,r6),fr3 */
L8c6193e4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6193e6; /* fsub fr5,fr3 */
L8c6193e6: fcopy(f,4u,2u);goto L8c6193e8; /* fmov fr2,fr4 */
L8c6193e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6193ea; /* fmul fr3,fr1 */
L8c6193ea: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6193ec; /* fldi0 fr3 */
L8c6193ec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c6193ee; /* fsub fr1,fr4 */
L8c6193ee: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c6193f0; /* fcmp/gt fr3,fr4 */
L8c6193f0: if(!(*sr&1u))goto L8c619418;goto L8c6193f2; /* bf 0x8c619418 */
L8c6193f2: r[11]+=12u;goto L8c61958c; /* bra 0x8c61958c */
L8c619418: r[3]=rd(m,bus,&io_status,0x8c61965cu,4);if(io_status)return io_status;goto L8c61941a; /* mov.l 0x8c61965c,r3 */
L8c61941a: r[0]=4u;goto L8c61941c; /* mov #0x4,r0 */
L8c61941c: fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61941e; /* fmov.s @r4,fr3 */
L8c61941e: r[8]+=28u;goto L8c619420; /* add #0x1c,r8 */
L8c619420: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c619422; /* fmov.s @r3,fr2 */
L8c619422: r[3]+=8u;goto L8c619424; /* add #0x8,r3 */
L8c619424: r[2]=rd(m,bus,&io_status,0x8c619660u,4);if(io_status)return io_status;goto L8c619426; /* mov.l 0x8c619660,r2 */
L8c619426: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619428; /* fmul fr3,fr2 */
L8c619428: r[1]=rd(m,bus,&io_status,0x8c619664u,4);if(io_status)return io_status;goto L8c61942a; /* mov.l 0x8c619664,r1 */
L8c61942a: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61942c; /* fmov.s @r2,fr3 */
L8c61942c: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61942e; /* fmov.s @r1,fr1 */
L8c61942e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619430; /* fsub fr2,fr3 */
L8c619430: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c619432; /* fmov.s @(r0,r4),fr2 */
L8c619432: r[0]=8u;goto L8c619434; /* mov #0x8,r0 */
L8c619434: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619436; /* fmul fr2,fr1 */
L8c619436: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c619438; /* fmov.s @(r0,r4),fr2 */
L8c619438: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61943a; /* fsub fr1,fr3 */
L8c61943a: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61943c; /* fmov.s @r3,fr1 */
L8c61943c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61943e; /* fmul fr2,fr1 */
L8c61943e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619440; /* fsub fr1,fr3 */
L8c619440: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619442; /* fmov.s fr3,@-r8 */
L8c619442: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619444; /* mov.w @(0x2,r11),r0 */
L8c619444: r[2]=r[0];goto L8c619446; /* mov r0,r2 */
L8c619446: *fpul=r[2];goto L8c619448; /* lds r2,FPUL */
L8c619448: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61944a; /* float FPUL,fr3 */
L8c61944a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61944c; /* fmul fr15,fr3 */
L8c61944c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61944e; /* fmov.s fr3,@-r8 */
L8c61944e: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619450; /* mov.w @r11,r2 */
L8c619450: *fpul=r[2];goto L8c619452; /* lds r2,FPUL */
L8c619452: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619454; /* float FPUL,fr3 */
L8c619454: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619456; /* fmul fr15,fr3 */
L8c619456: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619458; /* fmov.s fr3,@-r8 */
L8c619458: r[0]=24u;goto L8c61945a; /* mov #0x18,r0 */
L8c61945a: r[2]=rd(m,bus,&io_status,0x8c619668u,4);if(io_status)return io_status;goto L8c61945c; /* mov.l 0x8c619668,r2 */
L8c61945c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61945e; /* fmov.s @(r0,r4),fr3 */
L8c61945e: r[0]=20u;goto L8c619460; /* mov #0x14,r0 */
L8c619460: r[11]+=4u;goto L8c619462; /* add #0x4,r11 */
L8c619462: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619464; /* fmov.s fr3,@-r8 */
L8c619464: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619466; /* fmov.s @(r0,r4),fr3 */
L8c619466: r[0]=16u;goto L8c619468; /* mov #0x10,r0 */
L8c619468: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61946a; /* fmov.s fr3,@-r8 */
L8c61946a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61946c; /* fmov.s @(r0,r4),fr3 */
L8c61946c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61946e; /* fmov.s fr3,@-r8 */
L8c61946e: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c619470; /* mov.l r2,@-r8 */
L8c619470: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619472; /* pref @r8 */
L8c619472: r[2]=rd(m,bus,&io_status,0x8c61965cu,4);if(io_status)return io_status;goto L8c619474; /* mov.l 0x8c61965c,r2 */
L8c619474: r[0]=4u;goto L8c619476; /* mov #0x4,r0 */
L8c619476: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c619478; /* fmov.s @r5,fr3 */
L8c619478: r[8]+=60u;goto L8c61947a; /* add #0x3c,r8 */
L8c61947a: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61947c; /* fmov.s @r2,fr2 */
L8c61947c: r[3]=rd(m,bus,&io_status,0x8c619660u,4);if(io_status)return io_status;goto L8c61947e; /* mov.l 0x8c619660,r3 */
L8c61947e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619480; /* fmul fr3,fr2 */
L8c619480: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c619482; /* fmov.s @r1,fr1 */
L8c619482: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c619484; /* fmov.s @r3,fr3 */
L8c619484: r[3]=rd(m,bus,&io_status,0x8c61966cu,4);if(io_status)return io_status;goto L8c619486; /* mov.l 0x8c61966c,r3 */
L8c619486: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619488; /* fsub fr2,fr3 */
L8c619488: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61948a; /* fmov.s @(r0,r5),fr2 */
L8c61948a: r[0]=8u;goto L8c61948c; /* mov #0x8,r0 */
L8c61948c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61948e; /* fmul fr2,fr1 */
L8c61948e: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c619490; /* fmov.s @(r0,r5),fr2 */
L8c619490: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619492; /* fsub fr1,fr3 */
L8c619492: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619494; /* fmov.s @r3,fr1 */
L8c619494: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619496; /* fmul fr2,fr1 */
L8c619496: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619498; /* fsub fr1,fr3 */
L8c619498: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61949a; /* fmov.s fr3,@-r8 */
L8c61949a: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61949c; /* mov.w @(0x2,r11),r0 */
L8c61949c: r[2]=r[0];goto L8c61949e; /* mov r0,r2 */
L8c61949e: *fpul=r[2];goto L8c6194a0; /* lds r2,FPUL */
L8c6194a0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6194a2; /* float FPUL,fr3 */
L8c6194a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6194a4; /* fmul fr15,fr3 */
L8c6194a4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6194a6; /* fmov.s fr3,@-r8 */
L8c6194a6: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c6194a8; /* mov.w @r11,r2 */
L8c6194a8: r[11]+=4u;goto L8c6194aa; /* add #0x4,r11 */
L8c6194aa: *fpul=r[2];goto L8c6194ac; /* lds r2,FPUL */
L8c6194ac: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6194ae; /* float FPUL,fr3 */
L8c6194ae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6194b0; /* fmul fr15,fr3 */
L8c6194b0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6194b2; /* fmov.s fr3,@-r8 */
L8c6194b2: r[0]=24u;goto L8c6194b4; /* mov #0x18,r0 */
L8c6194b4: r[2]=rd(m,bus,&io_status,0x8c619668u,4);if(io_status)return io_status;goto L8c6194b6; /* mov.l 0x8c619668,r2 */
L8c6194b6: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6194b8; /* fmov.s @(r0,r5),fr3 */
L8c6194b8: r[0]=20u;goto L8c6194ba; /* mov #0x14,r0 */
L8c6194ba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6194bc; /* fmov.s fr3,@-r8 */
L8c6194bc: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6194be; /* fmov.s @(r0,r5),fr3 */
L8c6194be: r[0]=16u;goto L8c6194c0; /* mov #0x10,r0 */
L8c6194c0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6194c2; /* fmov.s fr3,@-r8 */
L8c6194c2: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6194c4; /* fmov.s @(r0,r5),fr3 */
L8c6194c4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6194c6; /* fmov.s fr3,@-r8 */
L8c6194c6: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c6194c8; /* mov.l r2,@-r8 */
L8c6194c8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6194ca; /* pref @r8 */
L8c6194ca: r[2]=rd(m,bus,&io_status,0x8c61965cu,4);if(io_status)return io_status;goto L8c6194cc; /* mov.l 0x8c61965c,r2 */
L8c6194cc: r[0]=4u;goto L8c6194ce; /* mov #0x4,r0 */
L8c6194ce: fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c6194d0; /* fmov.s @r6,fr3 */
L8c6194d0: r[8]+=60u;goto L8c6194d2; /* add #0x3c,r8 */
L8c6194d2: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c6194d4; /* fmov.s @r2,fr2 */
L8c6194d4: r[3]=rd(m,bus,&io_status,0x8c619660u,4);if(io_status)return io_status;goto L8c6194d6; /* mov.l 0x8c619660,r3 */
L8c6194d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6194d8; /* fmul fr3,fr2 */
L8c6194d8: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c6194da; /* fmov.s @r1,fr1 */
L8c6194da: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6194dc; /* fmov.s @r3,fr3 */
L8c6194dc: r[3]=rd(m,bus,&io_status,0x8c61966cu,4);if(io_status)return io_status;goto L8c6194de; /* mov.l 0x8c61966c,r3 */
L8c6194de: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6194e0; /* fsub fr2,fr3 */
L8c6194e0: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c6194e2; /* fmov.s @(r0,r6),fr2 */
L8c6194e2: r[0]=8u;goto L8c6194e4; /* mov #0x8,r0 */
L8c6194e4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6194e6; /* fmul fr2,fr1 */
L8c6194e6: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c6194e8; /* fmov.s @(r0,r6),fr2 */
L8c6194e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6194ea; /* fsub fr1,fr3 */
L8c6194ea: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6194ec; /* fmov.s @r3,fr1 */
L8c6194ec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6194ee; /* fmul fr2,fr1 */
L8c6194ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6194f0; /* fsub fr1,fr3 */
L8c6194f0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6194f2; /* fmov.s fr3,@-r8 */
L8c6194f2: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c6194f4; /* mov.w @(0x2,r11),r0 */
L8c6194f4: r[2]=r[0];goto L8c6194f6; /* mov r0,r2 */
L8c6194f6: *fpul=r[2];goto L8c6194f8; /* lds r2,FPUL */
L8c6194f8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6194fa; /* float FPUL,fr3 */
L8c6194fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6194fc; /* fmul fr15,fr3 */
L8c6194fc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6194fe; /* fmov.s fr3,@-r8 */
L8c6194fe: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619500; /* mov.w @r11,r2 */
L8c619500: r[11]+=4u;goto L8c619502; /* add #0x4,r11 */
L8c619502: *fpul=r[2];goto L8c619504; /* lds r2,FPUL */
L8c619504: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619506; /* float FPUL,fr3 */
L8c619506: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619508; /* fmul fr15,fr3 */
L8c619508: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61950a; /* fmov.s fr3,@-r8 */
L8c61950a: r[0]=24u;goto L8c61950c; /* mov #0x18,r0 */
L8c61950c: r[2]=rd(m,bus,&io_status,0x8c619670u,4);if(io_status)return io_status;goto L8c61950e; /* mov.l 0x8c619670,r2 */
L8c61950e: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619510; /* fmov.s @(r0,r6),fr3 */
L8c619510: r[0]=20u;goto L8c619512; /* mov #0x14,r0 */
L8c619512: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619514; /* fmov.s fr3,@-r8 */
L8c619514: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619516; /* fmov.s @(r0,r6),fr3 */
L8c619516: r[0]=16u;goto L8c619518; /* mov #0x10,r0 */
L8c619518: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61951a; /* fmov.s fr3,@-r8 */
L8c61951a: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61951c; /* fmov.s @(r0,r6),fr3 */
L8c61951c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61951e; /* fmov.s fr3,@-r8 */
L8c61951e: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c619520; /* mov.l r2,@-r8 */
L8c619520: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619522; /* pref @r8 */
L8c619522: r[0]=rd(m,bus,&io_status,0x8c619674u,4);if(io_status)return io_status;goto L8c619524; /* mov.l 0x8c619674,r0 */
L8c619524: r[8]+=32u;goto L8c619526; /* add #0x20,r8 */
L8c619526: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c619528; /* mov.l @r0,r2 */
L8c619528: r[2]+=3u;goto L8c61952a; /* add #0x3,r2 */
L8c61952a: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61958c; /* bra 0x8c61958c */
L8c61952e: r[1]=rd(m,bus,&io_status,0x8c619678u,4);if(io_status)return io_status;goto L8c619530; /* mov.l 0x8c619678,r1 */
L8c619530: wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c619532; /* mov.l r3,@r1 */
L8c619532: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619534; /* mov.w @r11,r3 */
L8c619534: *fpul=r[3];goto L8c619536; /* lds r3,FPUL */
L8c619536: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619538; /* float FPUL,fr3 */
L8c619538: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61953a; /* ftrc fr3,FPUL */
L8c61953a: r[2]=*fpul;goto L8c61953c; /* sts FPUL,r2 */
L8c61953c: wr(m,bus,&io_status,r[12],r[2],2);if(io_status)return io_status;goto L8c61953e; /* mov.w r2,@r12 */
L8c61953e: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619540; /* mov.w @(0x2,r11),r0 */
L8c619540: r[11]+=4u;goto L8c619542; /* add #0x4,r11 */
L8c619542: r[3]=r[0];goto L8c619544; /* mov r0,r3 */
L8c619544: *fpul=r[3];goto L8c619546; /* lds r3,FPUL */
L8c619546: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619548; /* float FPUL,fr3 */
L8c619548: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61954a; /* ftrc fr3,FPUL */
L8c61954a: r[0]=*fpul;goto L8c61954c; /* sts FPUL,r0 */
L8c61954c: wr(m,bus,&io_status,r[12]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61954e; /* mov.w r0,@(0x2,r12) */
L8c61954e: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619550; /* mov.w @r11,r3 */
L8c619550: *fpul=r[3];goto L8c619552; /* lds r3,FPUL */
L8c619552: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619554; /* float FPUL,fr3 */
L8c619554: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619556; /* ftrc fr3,FPUL */
L8c619556: r[2]=*fpul;goto L8c619558; /* sts FPUL,r2 */
L8c619558: wr(m,bus,&io_status,r[9],r[2],2);if(io_status)return io_status;goto L8c61955a; /* mov.w r2,@r9 */
L8c61955a: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61955c; /* mov.w @(0x2,r11),r0 */
L8c61955c: r[11]+=4u;goto L8c61955e; /* add #0x4,r11 */
L8c61955e: r[3]=r[0];goto L8c619560; /* mov r0,r3 */
L8c619560: *fpul=r[3];goto L8c619562; /* lds r3,FPUL */
L8c619562: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619564; /* float FPUL,fr3 */
L8c619564: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619566; /* ftrc fr3,FPUL */
L8c619566: r[0]=*fpul;goto L8c619568; /* sts FPUL,r0 */
L8c619568: wr(m,bus,&io_status,r[9]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61956a; /* mov.w r0,@(0x2,r9) */
L8c61956a: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61956c; /* mov.w @r11,r3 */
L8c61956c: *fpul=r[3];goto L8c61956e; /* lds r3,FPUL */
L8c61956e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619570; /* float FPUL,fr3 */
L8c619570: r[7]=rd(m,bus,&io_status,0x8c61967cu,4);if(io_status)return io_status;goto L8c619572; /* mov.l 0x8c61967c,r7 */
L8c619572: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619574; /* ftrc fr3,FPUL */
L8c619574: r[2]=*fpul;goto L8c619576; /* sts FPUL,r2 */
L8c619576: wr(m,bus,&io_status,r[14],r[2],2);if(io_status)return io_status;goto L8c619578; /* mov.w r2,@r14 */
L8c619578: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61957a; /* mov.w @(0x2,r11),r0 */
L8c61957a: r[3]=r[0];goto L8c61957c; /* mov r0,r3 */
L8c61957c: *fpul=r[3];goto L8c61957e; /* lds r3,FPUL */
L8c61957e: r[3]=rd(m,bus,&io_status,0x8c619680u,4);if(io_status)return io_status;goto L8c619580; /* mov.l 0x8c619680,r3 */
L8c619580: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619582; /* float FPUL,fr3 */
L8c619582: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619584; /* ftrc fr3,FPUL */
L8c619584: r[0]=*fpul;goto L8c619586; /* sts FPUL,r0 */
L8c619586: wr(m,bus,&io_status,r[14]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619588; /* mov.w r0,@(0x2,r14) */
L8c619588: target=r[3];*pr=0x8c61958cu;r[11]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61958c; /* jsr @r3 */
L8c61958c: r[13]+=1u;goto L8c61958e; /* add #0x1,r13 */
L8c61958e: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c619590; /* mov.l @(0x8,r15),r2 */
L8c619590: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c619592; /* mov.w @(0x2,r2),r0 */
L8c619592: r[0]=r[0]&65535u;goto L8c619594; /* extu.w r0,r0 */
L8c619594: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[0]);goto L8c619596; /* cmp/ge r0,r13 */
L8c619596: if((*sr&1u))goto L8c61959c;goto L8c619598; /* bt 0x8c61959c */
L8c619598: goto L8c61937a; /* bra 0x8c61937a */
L8c61959c: goto L8c619be2; /* bra 0x8c619be2 */
L8c6195a0: r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6195a2; /* mov.l @(0x8,r15),r1 */
L8c6195a2: r[2]=rd(m,bus,&io_status,0x8c619684u,4);if(io_status)return io_status;goto L8c6195a4; /* mov.l 0x8c619684,r2 */
L8c6195a4: r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c6195a6; /* mov.w @(0x2,r1),r0 */
L8c6195a6: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6195a8; /* mov.l @r2,r1 */
L8c6195a8: r[0]=r[0]&65535u;goto L8c6195aa; /* extu.w r0,r0 */
L8c6195aa: r[9]=rd(m,bus,&io_status,0x8c61967cu,4);if(io_status)return io_status;goto L8c6195ac; /* mov.l 0x8c61967c,r9 */
L8c6195ac: r[0]<<=2;goto L8c6195ae; /* shll2 r0 */
L8c6195ae: r[3]=rd(m,bus,&io_status,0x8c619688u,4);if(io_status)return io_status;goto L8c6195b0; /* mov.l 0x8c619688,r3 */
L8c6195b0: r[1]+=r[0];goto L8c6195b2; /* add r0,r1 */
L8c6195b2: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c6195b4; /* mov.l r1,@r2 */
L8c6195b4: r[2]=0u;goto L8c6195b6; /* mov #0x0,r2 */
L8c6195b6: wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c6195b8; /* mov.l r2,@(0x10,r15) */
L8c6195b8: r[2]=r[9];goto L8c6195ba; /* mov r9,r2 */
L8c6195ba: r[1]=rd(m,bus,&io_status,0x8c61968cu,4);if(io_status)return io_status;goto L8c6195bc; /* mov.l 0x8c61968c,r1 */
L8c6195bc: r[2]+=4u;goto L8c6195be; /* add #0x4,r2 */
L8c6195be: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6195c0; /* mov.l r2,@r15 */
L8c6195c0: wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6195c2; /* mov.l r3,@(0xc,r15) */
L8c6195c2: wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c6198be; /* bra 0x8c6198be */
L8c6195c6: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c6195c8; /* mov.w @r10+,r4 */
L8c6195c8: r[2]=rd(m,bus,&io_status,0x8c619690u,4);if(io_status)return io_status;goto L8c6195ca; /* mov.l 0x8c619690,r2 */
L8c6195ca: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c6195cc; /* mov.w @r10+,r14 */
L8c6195cc: r[4]=r[4]&65535u;goto L8c6195ce; /* extu.w r4,r4 */
L8c6195ce: r[4]<<=2;goto L8c6195d0; /* shll2 r4 */
L8c6195d0: r[6]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6195d2; /* mov.l @r2,r6 */
L8c6195d2: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c6195d4; /* mov.w @r10+,r13 */
L8c6195d4: r[14]=r[14]&65535u;goto L8c6195d6; /* extu.w r14,r14 */
L8c6195d6: r[14]<<=2;goto L8c6195d8; /* shll2 r14 */
L8c6195d8: r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c6195da; /* mov.w @r10+,r12 */
L8c6195da: r[13]=r[13]&65535u;goto L8c6195dc; /* extu.w r13,r13 */
L8c6195dc: r[14]<<=2;goto L8c6195de; /* shll2 r14 */
L8c6195de: r[4]<<=2;goto L8c6195e0; /* shll2 r4 */
L8c6195e0: r[12]=r[12]&65535u;goto L8c6195e2; /* extu.w r12,r12 */
L8c6195e2: r[13]<<=2;goto L8c6195e4; /* shll2 r13 */
L8c6195e4: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6195e6; /* shll r4 */
L8c6195e6: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c6195e8; /* shll r14 */
L8c6195e8: r[4]+=r[6];goto L8c6195ea; /* add r6,r4 */
L8c6195ea: r[12]<<=2;goto L8c6195ec; /* shll2 r12 */
L8c6195ec: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6195ee; /* mov.l @(0xc,r4),r5 */
L8c6195ee: r[14]+=r[6];goto L8c6195f0; /* add r6,r14 */
L8c6195f0: r[13]<<=2;goto L8c6195f2; /* shll2 r13 */
L8c6195f2: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c6195f4; /* mov.l @(0xc,r14),r3 */
L8c6195f4: r[12]<<=2;goto L8c6195f6; /* shll2 r12 */
L8c6195f6: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c6195f8; /* shll r13 */
L8c6195f8: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c6195fa; /* shll r12 */
L8c6195fa: r[13]+=r[6];goto L8c6195fc; /* add r6,r13 */
L8c6195fc: r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c6195fe; /* mov.l @(0xc,r13),r1 */
L8c6195fe: r[5]&=r[3];goto L8c619600; /* and r3,r5 */
L8c619600: r[12]+=r[6];goto L8c619602; /* add r6,r12 */
L8c619602: r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c619604; /* mov.l @(0xc,r12),r3 */
L8c619604: r[5]&=r[1];goto L8c619606; /* and r1,r5 */
L8c619606: r[1]=63u;goto L8c619608; /* mov #0x3f,r1 */
L8c619608: r[5]&=r[3];goto L8c61960a; /* and r3,r5 */
L8c61960a: *sr=(*sr&~1u)|!(r[1]&r[5]);goto L8c61960c; /* tst r1,r5 */
L8c61960c: if(!(*sr&1u))goto L8c619652;goto L8c61960e; /* bf 0x8c619652 */
L8c61960e: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c619610; /* mov.l @(0xc,r4),r5 */
L8c619610: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c619612; /* mov.l @(0xc,r14),r3 */
L8c619612: r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c619614; /* mov.l @(0xc,r13),r2 */
L8c619614: r[5]|=r[3];goto L8c619616; /* or r3,r5 */
L8c619616: r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c619618; /* mov.l @(0xc,r12),r3 */
L8c619618: r[5]|=r[2];goto L8c61961a; /* or r2,r5 */
L8c61961a: r[2]=1u;goto L8c61961c; /* mov #0x1,r2 */
L8c61961c: r[5]|=r[3];goto L8c61961e; /* or r3,r5 */
L8c61961e: *sr=(*sr&~1u)|!(r[2]&r[5]);goto L8c619620; /* tst r2,r5 */
L8c619620: if((*sr&1u))goto L8c619626;goto L8c619622; /* bt 0x8c619626 */
L8c619622: goto L8c619802; /* bra 0x8c619802 */
L8c619626: r[0]=16u;goto L8c619628; /* mov #0x10,r0 */
L8c619628: fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c61962a; /* fmov.s @(r0,r14),fr4 */
L8c61962a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61962c; /* fmov.s @(r0,r4),fr3 */
L8c61962c: r[0]=20u;goto L8c61962e; /* mov #0x14,r0 */
L8c61962e: fcopy(f,2u,4u);goto L8c619630; /* fmov fr4,fr2 */
L8c619630: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c619632; /* fsub fr3,fr2 */
L8c619632: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c619634; /* fmov.s @(r0,r13),fr3 */
L8c619634: fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c619636; /* fmov.s @(r0,r14),fr5 */
L8c619636: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619638; /* fsub fr5,fr3 */
L8c619638: fcopy(f,1u,5u);goto L8c61963a; /* fmov fr5,fr1 */
L8c61963a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61963c; /* fmul fr3,fr2 */
L8c61963c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61963e; /* fmov.s @(r0,r4),fr3 */
L8c61963e: r[0]=16u;goto L8c619640; /* mov #0x10,r0 */
L8c619640: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c619642; /* fsub fr3,fr1 */
L8c619642: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c619644; /* fmov.s @(r0,r13),fr3 */
L8c619644: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619646; /* fsub fr4,fr3 */
L8c619646: fcopy(f,4u,2u);goto L8c619648; /* fmov fr2,fr4 */
L8c619648: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61964a; /* fmul fr3,fr1 */
L8c61964a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61964c; /* fldi0 fr3 */
L8c61964c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61964e; /* fsub fr1,fr4 */
L8c61964e: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c619650; /* fcmp/gt fr3,fr4 */
L8c619650: if(!(*sr&1u))goto L8c619694;goto L8c619652; /* bf 0x8c619694 */
L8c619652: r[11]+=16u;goto L8c6198b8; /* bra 0x8c6198b8 */
L8c619694: r[3]=rd(m,bus,&io_status,0x8c619998u,4);if(io_status)return io_status;goto L8c619696; /* mov.l 0x8c619998,r3 */
L8c619696: r[0]=4u;goto L8c619698; /* mov #0x4,r0 */
L8c619698: fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61969a; /* fmov.s @r4,fr3 */
L8c61969a: r[8]+=28u;goto L8c61969c; /* add #0x1c,r8 */
L8c61969c: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61969e; /* fmov.s @r3,fr2 */
L8c61969e: r[3]+=8u;goto L8c6196a0; /* add #0x8,r3 */
L8c6196a0: r[2]=rd(m,bus,&io_status,0x8c61999cu,4);if(io_status)return io_status;goto L8c6196a2; /* mov.l 0x8c61999c,r2 */
L8c6196a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6196a4; /* fmul fr3,fr2 */
L8c6196a4: r[1]=rd(m,bus,&io_status,0x8c6199a0u,4);if(io_status)return io_status;goto L8c6196a6; /* mov.l 0x8c6199a0,r1 */
L8c6196a6: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6196a8; /* fmov.s @r2,fr3 */
L8c6196a8: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c6196aa; /* fmov.s @r1,fr1 */
L8c6196aa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6196ac; /* fsub fr2,fr3 */
L8c6196ac: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6196ae; /* fmov.s @(r0,r4),fr2 */
L8c6196ae: r[0]=8u;goto L8c6196b0; /* mov #0x8,r0 */
L8c6196b0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6196b2; /* fmul fr2,fr1 */
L8c6196b2: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6196b4; /* fmov.s @(r0,r4),fr2 */
L8c6196b4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6196b6; /* fsub fr1,fr3 */
L8c6196b6: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6196b8; /* fmov.s @r3,fr1 */
L8c6196b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6196ba; /* fmul fr2,fr1 */
L8c6196ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6196bc; /* fsub fr1,fr3 */
L8c6196bc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6196be; /* fmov.s fr3,@-r8 */
L8c6196be: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c6196c0; /* mov.w @(0x2,r11),r0 */
L8c6196c0: r[2]=r[0];goto L8c6196c2; /* mov r0,r2 */
L8c6196c2: *fpul=r[2];goto L8c6196c4; /* lds r2,FPUL */
L8c6196c4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6196c6; /* float FPUL,fr3 */
L8c6196c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6196c8; /* fmul fr15,fr3 */
L8c6196c8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6196ca; /* fmov.s fr3,@-r8 */
L8c6196ca: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c6196cc; /* mov.w @r11,r2 */
L8c6196cc: *fpul=r[2];goto L8c6196ce; /* lds r2,FPUL */
L8c6196ce: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6196d0; /* float FPUL,fr3 */
L8c6196d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6196d2; /* fmul fr15,fr3 */
L8c6196d2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6196d4; /* fmov.s fr3,@-r8 */
L8c6196d4: r[0]=24u;goto L8c6196d6; /* mov #0x18,r0 */
L8c6196d6: r[2]=rd(m,bus,&io_status,0x8c6199a4u,4);if(io_status)return io_status;goto L8c6196d8; /* mov.l 0x8c6199a4,r2 */
L8c6196d8: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6196da; /* fmov.s @(r0,r4),fr3 */
L8c6196da: r[0]=20u;goto L8c6196dc; /* mov #0x14,r0 */
L8c6196dc: r[11]+=4u;goto L8c6196de; /* add #0x4,r11 */
L8c6196de: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6196e0; /* fmov.s fr3,@-r8 */
L8c6196e0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6196e2; /* fmov.s @(r0,r4),fr3 */
L8c6196e2: r[0]=16u;goto L8c6196e4; /* mov #0x10,r0 */
L8c6196e4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6196e6; /* fmov.s fr3,@-r8 */
L8c6196e6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6196e8; /* fmov.s @(r0,r4),fr3 */
L8c6196e8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6196ea; /* fmov.s fr3,@-r8 */
L8c6196ea: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c6196ec; /* mov.l r2,@-r8 */
L8c6196ec: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6196ee; /* pref @r8 */
L8c6196ee: r[2]=rd(m,bus,&io_status,0x8c619998u,4);if(io_status)return io_status;goto L8c6196f0; /* mov.l 0x8c619998,r2 */
L8c6196f0: r[0]=4u;goto L8c6196f2; /* mov #0x4,r0 */
L8c6196f2: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c6196f4; /* fmov.s @r14,fr3 */
L8c6196f4: r[8]+=60u;goto L8c6196f6; /* add #0x3c,r8 */
L8c6196f6: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c6196f8; /* fmov.s @r2,fr2 */
L8c6196f8: r[3]=rd(m,bus,&io_status,0x8c61999cu,4);if(io_status)return io_status;goto L8c6196fa; /* mov.l 0x8c61999c,r3 */
L8c6196fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6196fc; /* fmul fr3,fr2 */
L8c6196fc: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c6196fe; /* fmov.s @r1,fr1 */
L8c6196fe: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c619700; /* fmov.s @r3,fr3 */
L8c619700: r[3]=rd(m,bus,&io_status,0x8c6199a8u,4);if(io_status)return io_status;goto L8c619702; /* mov.l 0x8c6199a8,r3 */
L8c619702: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619704; /* fsub fr2,fr3 */
L8c619704: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c619706; /* fmov.s @(r0,r14),fr2 */
L8c619706: r[0]=8u;goto L8c619708; /* mov #0x8,r0 */
L8c619708: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61970a; /* fmul fr2,fr1 */
L8c61970a: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61970c; /* fmov.s @(r0,r14),fr2 */
L8c61970c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61970e; /* fsub fr1,fr3 */
L8c61970e: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619710; /* fmov.s @r3,fr1 */
L8c619710: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619712; /* fmul fr2,fr1 */
L8c619712: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619714; /* fsub fr1,fr3 */
L8c619714: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619716; /* fmov.s fr3,@-r8 */
L8c619716: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619718; /* mov.w @(0x2,r11),r0 */
L8c619718: r[2]=r[0];goto L8c61971a; /* mov r0,r2 */
L8c61971a: *fpul=r[2];goto L8c61971c; /* lds r2,FPUL */
L8c61971c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61971e; /* float FPUL,fr3 */
L8c61971e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619720; /* fmul fr15,fr3 */
L8c619720: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619722; /* fmov.s fr3,@-r8 */
L8c619722: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619724; /* mov.w @r11,r2 */
L8c619724: r[11]+=4u;goto L8c619726; /* add #0x4,r11 */
L8c619726: *fpul=r[2];goto L8c619728; /* lds r2,FPUL */
L8c619728: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61972a; /* float FPUL,fr3 */
L8c61972a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61972c; /* fmul fr15,fr3 */
L8c61972c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61972e; /* fmov.s fr3,@-r8 */
L8c61972e: r[0]=24u;goto L8c619730; /* mov #0x18,r0 */
L8c619730: r[2]=rd(m,bus,&io_status,0x8c6199a4u,4);if(io_status)return io_status;goto L8c619732; /* mov.l 0x8c6199a4,r2 */
L8c619732: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c619734; /* fmov.s @(r0,r14),fr3 */
L8c619734: r[0]=20u;goto L8c619736; /* mov #0x14,r0 */
L8c619736: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619738; /* fmov.s fr3,@-r8 */
L8c619738: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61973a; /* fmov.s @(r0,r14),fr3 */
L8c61973a: r[0]=16u;goto L8c61973c; /* mov #0x10,r0 */
L8c61973c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61973e; /* fmov.s fr3,@-r8 */
L8c61973e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c619740; /* fmov.s @(r0,r14),fr3 */
L8c619740: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619742; /* fmov.s fr3,@-r8 */
L8c619742: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c619744; /* mov.l r2,@-r8 */
L8c619744: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619746; /* pref @r8 */
L8c619746: r[2]=rd(m,bus,&io_status,0x8c619998u,4);if(io_status)return io_status;goto L8c619748; /* mov.l 0x8c619998,r2 */
L8c619748: r[0]=4u;goto L8c61974a; /* mov #0x4,r0 */
L8c61974a: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61974c; /* fmov.s @r13,fr3 */
L8c61974c: r[8]+=60u;goto L8c61974e; /* add #0x3c,r8 */
L8c61974e: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c619750; /* fmov.s @r2,fr2 */
L8c619750: r[3]=rd(m,bus,&io_status,0x8c61999cu,4);if(io_status)return io_status;goto L8c619752; /* mov.l 0x8c61999c,r3 */
L8c619752: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619754; /* fmul fr3,fr2 */
L8c619754: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c619756; /* fmov.s @r1,fr1 */
L8c619756: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c619758; /* fmov.s @r3,fr3 */
L8c619758: r[3]=rd(m,bus,&io_status,0x8c6199a8u,4);if(io_status)return io_status;goto L8c61975a; /* mov.l 0x8c6199a8,r3 */
L8c61975a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61975c; /* fsub fr2,fr3 */
L8c61975c: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61975e; /* fmov.s @(r0,r13),fr2 */
L8c61975e: r[0]=8u;goto L8c619760; /* mov #0x8,r0 */
L8c619760: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619762; /* fmul fr2,fr1 */
L8c619762: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c619764; /* fmov.s @(r0,r13),fr2 */
L8c619764: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619766; /* fsub fr1,fr3 */
L8c619766: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619768; /* fmov.s @r3,fr1 */
L8c619768: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61976a; /* fmul fr2,fr1 */
L8c61976a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61976c; /* fsub fr1,fr3 */
L8c61976c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61976e; /* fmov.s fr3,@-r8 */
L8c61976e: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619770; /* mov.w @(0x2,r11),r0 */
L8c619770: r[2]=r[0];goto L8c619772; /* mov r0,r2 */
L8c619772: *fpul=r[2];goto L8c619774; /* lds r2,FPUL */
L8c619774: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619776; /* float FPUL,fr3 */
L8c619776: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619778; /* fmul fr15,fr3 */
L8c619778: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61977a; /* fmov.s fr3,@-r8 */
L8c61977a: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61977c; /* mov.w @r11,r2 */
L8c61977c: r[11]+=4u;goto L8c61977e; /* add #0x4,r11 */
L8c61977e: *fpul=r[2];goto L8c619780; /* lds r2,FPUL */
L8c619780: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619782; /* float FPUL,fr3 */
L8c619782: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619784; /* fmul fr15,fr3 */
L8c619784: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619786; /* fmov.s fr3,@-r8 */
L8c619786: r[0]=24u;goto L8c619788; /* mov #0x18,r0 */
L8c619788: r[2]=rd(m,bus,&io_status,0x8c6199a4u,4);if(io_status)return io_status;goto L8c61978a; /* mov.l 0x8c6199a4,r2 */
L8c61978a: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61978c; /* fmov.s @(r0,r13),fr3 */
L8c61978c: r[0]=20u;goto L8c61978e; /* mov #0x14,r0 */
L8c61978e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619790; /* fmov.s fr3,@-r8 */
L8c619790: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c619792; /* fmov.s @(r0,r13),fr3 */
L8c619792: r[0]=16u;goto L8c619794; /* mov #0x10,r0 */
L8c619794: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619796; /* fmov.s fr3,@-r8 */
L8c619796: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c619798; /* fmov.s @(r0,r13),fr3 */
L8c619798: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61979a; /* fmov.s fr3,@-r8 */
L8c61979a: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61979c; /* mov.l r2,@-r8 */
L8c61979c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61979e; /* pref @r8 */
L8c61979e: r[2]=rd(m,bus,&io_status,0x8c619998u,4);if(io_status)return io_status;goto L8c6197a0; /* mov.l 0x8c619998,r2 */
L8c6197a0: r[0]=4u;goto L8c6197a2; /* mov #0x4,r0 */
L8c6197a2: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c6197a4; /* fmov.s @r12,fr3 */
L8c6197a4: r[8]+=60u;goto L8c6197a6; /* add #0x3c,r8 */
L8c6197a6: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c6197a8; /* fmov.s @r2,fr2 */
L8c6197a8: r[3]=rd(m,bus,&io_status,0x8c61999cu,4);if(io_status)return io_status;goto L8c6197aa; /* mov.l 0x8c61999c,r3 */
L8c6197aa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6197ac; /* fmul fr3,fr2 */
L8c6197ac: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c6197ae; /* fmov.s @r1,fr1 */
L8c6197ae: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6197b0; /* fmov.s @r3,fr3 */
L8c6197b0: r[3]=rd(m,bus,&io_status,0x8c6199a8u,4);if(io_status)return io_status;goto L8c6197b2; /* mov.l 0x8c6199a8,r3 */
L8c6197b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6197b4; /* fsub fr2,fr3 */
L8c6197b4: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6197b6; /* fmov.s @(r0,r12),fr2 */
L8c6197b6: r[0]=8u;goto L8c6197b8; /* mov #0x8,r0 */
L8c6197b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6197ba; /* fmul fr2,fr1 */
L8c6197ba: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6197bc; /* fmov.s @(r0,r12),fr2 */
L8c6197bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6197be; /* fsub fr1,fr3 */
L8c6197be: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6197c0; /* fmov.s @r3,fr1 */
L8c6197c0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6197c2; /* fmul fr2,fr1 */
L8c6197c2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6197c4; /* fsub fr1,fr3 */
L8c6197c4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6197c6; /* fmov.s fr3,@-r8 */
L8c6197c6: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c6197c8; /* mov.w @(0x2,r11),r0 */
L8c6197c8: r[2]=r[0];goto L8c6197ca; /* mov r0,r2 */
L8c6197ca: *fpul=r[2];goto L8c6197cc; /* lds r2,FPUL */
L8c6197cc: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6197ce; /* float FPUL,fr3 */
L8c6197ce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6197d0; /* fmul fr15,fr3 */
L8c6197d0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6197d2; /* fmov.s fr3,@-r8 */
L8c6197d2: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c6197d4; /* mov.w @r11,r2 */
L8c6197d4: r[11]+=4u;goto L8c6197d6; /* add #0x4,r11 */
L8c6197d6: *fpul=r[2];goto L8c6197d8; /* lds r2,FPUL */
L8c6197d8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6197da; /* float FPUL,fr3 */
L8c6197da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6197dc; /* fmul fr15,fr3 */
L8c6197dc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6197de; /* fmov.s fr3,@-r8 */
L8c6197de: r[0]=24u;goto L8c6197e0; /* mov #0x18,r0 */
L8c6197e0: r[2]=rd(m,bus,&io_status,0x8c6199acu,4);if(io_status)return io_status;goto L8c6197e2; /* mov.l 0x8c6199ac,r2 */
L8c6197e2: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c6197e4; /* fmov.s @(r0,r12),fr3 */
L8c6197e4: r[0]=20u;goto L8c6197e6; /* mov #0x14,r0 */
L8c6197e6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6197e8; /* fmov.s fr3,@-r8 */
L8c6197e8: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c6197ea; /* fmov.s @(r0,r12),fr3 */
L8c6197ea: r[0]=16u;goto L8c6197ec; /* mov #0x10,r0 */
L8c6197ec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6197ee; /* fmov.s fr3,@-r8 */
L8c6197ee: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c6197f0; /* fmov.s @(r0,r12),fr3 */
L8c6197f0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6197f2; /* fmov.s fr3,@-r8 */
L8c6197f2: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c6197f4; /* mov.l r2,@-r8 */
L8c6197f4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6197f6; /* pref @r8 */
L8c6197f6: r[0]=rd(m,bus,&io_status,0x8c6199b0u,4);if(io_status)return io_status;goto L8c6197f8; /* mov.l 0x8c6199b0,r0 */
L8c6197f8: r[8]+=32u;goto L8c6197fa; /* add #0x20,r8 */
L8c6197fa: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c6197fc; /* mov.l @r0,r2 */
L8c6197fc: r[2]+=4u;goto L8c6197fe; /* add #0x4,r2 */
L8c6197fe: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c6198b8; /* bra 0x8c6198b8 */
L8c619802: r[1]=rd(m,bus,&io_status,0x8c6199b4u,4);if(io_status)return io_status;goto L8c619804; /* mov.l 0x8c6199b4,r1 */
L8c619804: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c619806; /* mov.l r2,@r1 */
L8c619806: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619808; /* mov.w @r11,r3 */
L8c619808: *fpul=r[3];goto L8c61980a; /* lds r3,FPUL */
L8c61980a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61980c; /* float FPUL,fr3 */
L8c61980c: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61980e; /* ftrc fr3,FPUL */
L8c61980e: r[2]=*fpul;goto L8c619810; /* sts FPUL,r2 */
L8c619810: wr(m,bus,&io_status,r[9],r[2],2);if(io_status)return io_status;goto L8c619812; /* mov.w r2,@r9 */
L8c619812: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619814; /* mov.w @(0x2,r11),r0 */
L8c619814: r[11]+=4u;goto L8c619816; /* add #0x4,r11 */
L8c619816: r[3]=r[0];goto L8c619818; /* mov r0,r3 */
L8c619818: *fpul=r[3];goto L8c61981a; /* lds r3,FPUL */
L8c61981a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61981c; /* float FPUL,fr3 */
L8c61981c: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61981e; /* ftrc fr3,FPUL */
L8c61981e: r[0]=*fpul;goto L8c619820; /* sts FPUL,r0 */
L8c619820: wr(m,bus,&io_status,r[9]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619822; /* mov.w r0,@(0x2,r9) */
L8c619822: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619824; /* mov.w @r11,r2 */
L8c619824: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c619826; /* mov.l @r15,r3 */
L8c619826: *fpul=r[2];goto L8c619828; /* lds r2,FPUL */
L8c619828: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61982a; /* float FPUL,fr3 */
L8c61982a: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61982c; /* ftrc fr3,FPUL */
L8c61982c: r[1]=*fpul;goto L8c61982e; /* sts FPUL,r1 */
L8c61982e: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c619830; /* mov.w r1,@r3 */
L8c619830: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619832; /* mov.w @(0x2,r11),r0 */
L8c619832: r[11]+=4u;goto L8c619834; /* add #0x4,r11 */
L8c619834: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c619836; /* mov.l @r15,r3 */
L8c619836: r[2]=r[0];goto L8c619838; /* mov r0,r2 */
L8c619838: *fpul=r[2];goto L8c61983a; /* lds r2,FPUL */
L8c61983a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61983c; /* float FPUL,fr3 */
L8c61983c: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61983e; /* ftrc fr3,FPUL */
L8c61983e: r[0]=*fpul;goto L8c619840; /* sts FPUL,r0 */
L8c619840: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619842; /* mov.w r0,@(0x2,r3) */
L8c619842: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619844; /* mov.w @r11,r2 */
L8c619844: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c619846; /* mov.l @(0xc,r15),r3 */
L8c619846: *fpul=r[2];goto L8c619848; /* lds r2,FPUL */
L8c619848: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61984a; /* float FPUL,fr3 */
L8c61984a: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61984c; /* ftrc fr3,FPUL */
L8c61984c: r[1]=*fpul;goto L8c61984e; /* sts FPUL,r1 */
L8c61984e: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c619850; /* mov.w r1,@r3 */
L8c619850: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619852; /* mov.w @(0x2,r11),r0 */
L8c619852: r[11]+=4u;goto L8c619854; /* add #0x4,r11 */
L8c619854: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c619856; /* mov.l @(0xc,r15),r3 */
L8c619856: r[2]=r[0];goto L8c619858; /* mov r0,r2 */
L8c619858: *fpul=r[2];goto L8c61985a; /* lds r2,FPUL */
L8c61985a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61985c; /* float FPUL,fr3 */
L8c61985c: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61985e; /* ftrc fr3,FPUL */
L8c61985e: r[0]=*fpul;goto L8c619860; /* sts FPUL,r0 */
L8c619860: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619862; /* mov.w r0,@(0x2,r3) */
L8c619862: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619864; /* mov.w @r11,r2 */
L8c619864: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c619866; /* mov.l @(0x4,r15),r3 */
L8c619866: *fpul=r[2];goto L8c619868; /* lds r2,FPUL */
L8c619868: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61986a; /* float FPUL,fr3 */
L8c61986a: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61986c; /* ftrc fr3,FPUL */
L8c61986c: r[1]=*fpul;goto L8c61986e; /* sts FPUL,r1 */
L8c61986e: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c619870; /* mov.w r1,@r3 */
L8c619870: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619872; /* mov.w @(0x2,r11),r0 */
L8c619872: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c619874; /* mov.l @(0x4,r15),r3 */
L8c619874: r[2]=r[0];goto L8c619876; /* mov r0,r2 */
L8c619876: *fpul=r[2];goto L8c619878; /* lds r2,FPUL */
L8c619878: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61987a; /* float FPUL,fr3 */
L8c61987a: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61987c; /* ftrc fr3,FPUL */
L8c61987c: r[0]=*fpul;goto L8c61987e; /* sts FPUL,r0 */
L8c61987e: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619880; /* mov.w r0,@(0x2,r3) */
L8c619880: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c619882; /* mov.l @(0xc,r14),r3 */
L8c619882: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c619884; /* mov.l @(0xc,r4),r5 */
L8c619884: r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c619886; /* mov.l @(0xc,r13),r2 */
L8c619886: r[5]&=r[3];goto L8c619888; /* and r3,r5 */
L8c619888: r[3]=1u;goto L8c61988a; /* mov #0x1,r3 */
L8c61988a: r[5]&=r[2];goto L8c61988c; /* and r2,r5 */
L8c61988c: *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61988e; /* tst r3,r5 */
L8c61988e: take=!(*sr&1u);r[11]+=4u;if(take)goto L8c61989c;goto L8c619892; /* bf/s 0x8c61989c */
L8c619892: r[1]=rd(m,bus,&io_status,0x8c6199b8u,4);if(io_status)return io_status;goto L8c619894; /* mov.l 0x8c6199b8,r1 */
L8c619894: r[6]=r[13];goto L8c619896; /* mov r13,r6 */
L8c619896: r[7]=r[9];goto L8c619898; /* mov r9,r7 */
L8c619898: target=r[1];*pr=0x8c61989cu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61989c; /* jsr @r1 */
L8c61989c: r[4]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61989e; /* mov.l @(0xc,r14),r4 */
L8c61989e: r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c6198a0; /* mov.l @(0xc,r13),r3 */
L8c6198a0: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c6198a2; /* mov.l @(0xc,r12),r2 */
L8c6198a2: r[4]&=r[3];goto L8c6198a4; /* and r3,r4 */
L8c6198a4: r[3]=1u;goto L8c6198a6; /* mov #0x1,r3 */
L8c6198a6: r[4]&=r[2];goto L8c6198a8; /* and r2,r4 */
L8c6198a8: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c6198aa; /* tst r3,r4 */
L8c6198aa: if(!(*sr&1u))goto L8c6198b8;goto L8c6198ac; /* bf 0x8c6198b8 */
L8c6198ac: r[2]=rd(m,bus,&io_status,0x8c6199b8u,4);if(io_status)return io_status;goto L8c6198ae; /* mov.l 0x8c6199b8,r2 */
L8c6198ae: r[5]=r[13];goto L8c6198b0; /* mov r13,r5 */
L8c6198b0: r[7]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6198b2; /* mov.l @r15,r7 */
L8c6198b2: r[6]=r[12];goto L8c6198b4; /* mov r12,r6 */
L8c6198b4: target=r[2];*pr=0x8c6198b8u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6198b8; /* jsr @r2 */
L8c6198b8: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c6198ba; /* mov.l @(0x10,r15),r3 */
L8c6198ba: r[3]+=1u;goto L8c6198bc; /* add #0x1,r3 */
L8c6198bc: wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6198be; /* mov.l r3,@(0x10,r15) */
L8c6198be: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6198c0; /* mov.l @(0x8,r15),r2 */
L8c6198c0: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c6198c2; /* mov.l @(0x10,r15),r3 */
L8c6198c2: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c6198c4; /* mov.w @(0x2,r2),r0 */
L8c6198c4: r[0]=r[0]&65535u;goto L8c6198c6; /* extu.w r0,r0 */
L8c6198c6: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c6198c8; /* cmp/ge r0,r3 */
L8c6198c8: if((*sr&1u))goto L8c6198ce;goto L8c6198ca; /* bt 0x8c6198ce */
L8c6198ca: goto L8c6195c6; /* bra 0x8c6195c6 */
L8c6198ce: goto L8c619be2; /* bra 0x8c619be2 */
L8c6198d2: r[4]=rd(m,bus,&io_status,0x8c6199bcu,4);if(io_status)return io_status;goto L8c6198d4; /* mov.l 0x8c6199bc,r4 */
L8c6198d4: r[2]=0u;goto L8c6198d6; /* mov #0x0,r2 */
L8c6198d6: wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c6198d8; /* mov.l r2,@(0x10,r15) */
L8c6198d8: r[2]=r[4];goto L8c6198da; /* mov r4,r2 */
L8c6198da: r[1]=r[4];goto L8c6198dc; /* mov r4,r1 */
L8c6198dc: r[2]+=8u;goto L8c6198de; /* add #0x8,r2 */
L8c6198de: r[4]+=4u;goto L8c6198e0; /* add #0x4,r4 */
L8c6198e0: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6198e2; /* mov.l r2,@(0x4,r15) */
L8c6198e2: wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c6198e4; /* mov.l r1,@(0xc,r15) */
L8c6198e4: wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c619b90; /* bra 0x8c619b90 */
L8c6198e8: r[3]=rd(m,bus,&io_status,0x8c6199c0u,4);if(io_status)return io_status;goto L8c6198ea; /* mov.l 0x8c6199c0,r3 */
L8c6198ea: r[5]=0u;goto L8c6198ec; /* mov #0x0,r5 */
L8c6198ec: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c6198ee; /* mov.w @r10+,r13 */
L8c6198ee: r[6]=0u;goto L8c6198f0; /* mov #0x0,r6 */
L8c6198f0: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6198f2; /* mov.l @r3,r2 */
L8c6198f2: r[7]=r[5];goto L8c6198f4; /* mov r5,r7 */
L8c6198f4: r[13]=r[13]&65535u;goto L8c6198f6; /* extu.w r13,r13 */
L8c6198f6: r[2]+=r[13];goto L8c6198f8; /* add r13,r2 */
L8c6198f8: *sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c6198fa; /* cmp/pl r13 */
L8c6198fa: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6198fc; /* mov.l r2,@r3 */
L8c6198fc: r[4]=4294967295u;goto L8c6198fe; /* mov #-0x1,r4 */
L8c6198fe: take=!(*sr&1u);r[6]+=r[10];if(take)goto L8c619920;goto L8c619902; /* bf/s 0x8c619920 */
L8c619902: r[14]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c619904; /* mov.w @r6+,r14 */
L8c619904: r[7]+=1u;goto L8c619906; /* add #0x1,r7 */
L8c619906: r[2]=rd(m,bus,&io_status,0x8c6199c4u,4);if(io_status)return io_status;goto L8c619908; /* mov.l 0x8c6199c4,r2 */
L8c619908: r[14]=r[14]&65535u;goto L8c61990a; /* extu.w r14,r14 */
L8c61990a: r[14]<<=2;goto L8c61990c; /* shll2 r14 */
L8c61990c: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61990e; /* mov.l @r2,r3 */
L8c61990e: r[14]<<=2;goto L8c619910; /* shll2 r14 */
L8c619910: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c619912; /* shll r14 */
L8c619912: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[13]);goto L8c619914; /* cmp/ge r13,r7 */
L8c619914: r[14]+=r[3];goto L8c619916; /* add r3,r14 */
L8c619916: wr(m,bus,&io_status,r[15]+0x14u,r[14],4);if(io_status)return io_status;goto L8c619918; /* mov.l r14,@(0x14,r15) */
L8c619918: r[14]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61991a; /* mov.l @(0xc,r14),r14 */
L8c61991a: r[4]&=r[14];goto L8c61991c; /* and r14,r4 */
L8c61991c: take=!(*sr&1u);r[5]|=r[14];if(take)goto L8c619902;goto L8c619920; /* bf/s 0x8c619902 */
L8c619920: r[2]=63u;goto L8c619922; /* mov #0x3f,r2 */
L8c619922: r[7]=r[13];goto L8c619924; /* mov r13,r7 */
L8c619924: r[14]=r[13];goto L8c619926; /* mov r13,r14 */
L8c619926: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c619928; /* shll r7 */
L8c619928: *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61992a; /* tst r2,r4 */
L8c61992a: take=(*sr&1u);r[14]<<=2;if(take)goto L8c619934;goto L8c61992e; /* bt/s 0x8c619934 */
L8c61992e: r[11]+=r[14];goto L8c619930; /* add r14,r11 */
L8c619930: r[10]+=r[7];goto L8c619b8a; /* bra 0x8c619b8a */
L8c619934: r[3]=1u;goto L8c619936; /* mov #0x1,r3 */
L8c619936: *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c619938; /* tst r3,r5 */
L8c619938: if((*sr&1u))goto L8c61993e;goto L8c61993a; /* bt 0x8c61993e */
L8c61993a: goto L8c619aba; /* bra 0x8c619aba */
L8c61993e: r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c619940; /* mov.w @(0x2,r10),r0 */
L8c619940: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c619942; /* mov.w @r10,r4 */
L8c619942: r[2]=rd(m,bus,&io_status,0x8c6199c4u,4);if(io_status)return io_status;goto L8c619944; /* mov.l 0x8c6199c4,r2 */
L8c619944: r[5]=r[0]&65535u;goto L8c619946; /* extu.w r0,r5 */
L8c619946: r[5]<<=2;goto L8c619948; /* shll2 r5 */
L8c619948: r[0]=sx(m,bus,&io_status,r[10]+0x4u,2);if(io_status)return io_status;goto L8c61994a; /* mov.w @(0x4,r10),r0 */
L8c61994a: r[4]=r[4]&65535u;goto L8c61994c; /* extu.w r4,r4 */
L8c61994c: r[12]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61994e; /* mov.l @r2,r12 */
L8c61994e: r[4]<<=2;goto L8c619950; /* shll2 r4 */
L8c619950: r[5]<<=2;goto L8c619952; /* shll2 r5 */
L8c619952: r[6]=r[0]&65535u;goto L8c619954; /* extu.w r0,r6 */
L8c619954: r[4]<<=2;goto L8c619956; /* shll2 r4 */
L8c619956: r[0]=16u;goto L8c619958; /* mov #0x10,r0 */
L8c619958: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61995a; /* shll r4 */
L8c61995a: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61995c; /* shll r5 */
L8c61995c: r[5]+=r[12];goto L8c61995e; /* add r12,r5 */
L8c61995e: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c619960; /* fmov.s @(r0,r5),fr5 */
L8c619960: r[6]<<=2;goto L8c619962; /* shll2 r6 */
L8c619962: r[4]+=r[12];goto L8c619964; /* add r12,r4 */
L8c619964: r[6]<<=2;goto L8c619966; /* shll2 r6 */
L8c619966: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619968; /* fmov.s @(r0,r4),fr3 */
L8c619968: fcopy(f,2u,5u);goto L8c61996a; /* fmov fr5,fr2 */
L8c61996a: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61996c; /* shll r6 */
L8c61996c: r[0]=20u;goto L8c61996e; /* mov #0x14,r0 */
L8c61996e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c619970; /* fsub fr3,fr2 */
L8c619970: r[6]+=r[12];goto L8c619972; /* add r12,r6 */
L8c619972: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c619974; /* fmov.s @(r0,r5),fr4 */
L8c619974: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619976; /* fmov.s @(r0,r6),fr3 */
L8c619976: fcopy(f,1u,4u);goto L8c619978; /* fmov fr4,fr1 */
L8c619978: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61997a; /* fsub fr4,fr3 */
L8c61997a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61997c; /* fmul fr3,fr2 */
L8c61997c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61997e; /* fmov.s @(r0,r4),fr3 */
L8c61997e: r[0]=16u;goto L8c619980; /* mov #0x10,r0 */
L8c619980: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c619982; /* fsub fr3,fr1 */
L8c619982: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619984; /* fmov.s @(r0,r6),fr3 */
L8c619984: fcopy(f,4u,2u);goto L8c619986; /* fmov fr2,fr4 */
L8c619986: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619988; /* fsub fr5,fr3 */
L8c619988: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61998a; /* fmul fr3,fr1 */
L8c61998a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61998c; /* fldi0 fr3 */
L8c61998c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61998e; /* fsub fr1,fr4 */
L8c61998e: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c619990; /* fcmp/gt fr3,fr4 */
L8c619990: if(!(*sr&1u))goto L8c6199c8;goto L8c619992; /* bf 0x8c6199c8 */
L8c619992: r[10]+=r[7];goto L8c619994; /* add r7,r10 */
L8c619994: r[11]+=r[14];goto L8c619b8a; /* bra 0x8c619b8a */
L8c6199c8: r[3]=rd(m,bus,&io_status,0x8c619c04u,4);if(io_status)return io_status;goto L8c6199ca; /* mov.l 0x8c619c04,r3 */
L8c6199ca: r[6]=r[13];goto L8c6199cc; /* mov r13,r6 */
L8c6199cc: r[6]+=4294967295u;goto L8c6199ce; /* add #-0x1,r6 */
L8c6199ce: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6199d0; /* mov.l @r3,r2 */
L8c6199d0: *sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c6199d2; /* cmp/pl r6 */
L8c6199d2: r[2]+=r[13];goto L8c6199d4; /* add r13,r2 */
L8c6199d4: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6199d6; /* mov.l r2,@r3 */
L8c6199d6: take=!(*sr&1u);r[5]=0u;if(take)goto L8c619a4c;goto L8c6199da; /* bf/s 0x8c619a4c */
L8c6199da: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c6199dc; /* mov.w @r10+,r4 */
L8c6199dc: r[0]=4u;goto L8c6199de; /* mov #0x4,r0 */
L8c6199de: r[2]=rd(m,bus,&io_status,0x8c619c08u,4);if(io_status)return io_status;goto L8c6199e0; /* mov.l 0x8c619c08,r2 */
L8c6199e0: r[8]+=28u;goto L8c6199e2; /* add #0x1c,r8 */
L8c6199e2: r[4]=r[4]&65535u;goto L8c6199e4; /* extu.w r4,r4 */
L8c6199e4: r[1]=rd(m,bus,&io_status,0x8c619c10u,4);if(io_status)return io_status;goto L8c6199e6; /* mov.l 0x8c619c10,r1 */
L8c6199e6: r[4]<<=2;goto L8c6199e8; /* shll2 r4 */
L8c6199e8: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6199ea; /* mov.l @r2,r3 */
L8c6199ea: r[4]<<=2;goto L8c6199ec; /* shll2 r4 */
L8c6199ec: r[2]=rd(m,bus,&io_status,0x8c619c14u,4);if(io_status)return io_status;goto L8c6199ee; /* mov.l 0x8c619c14,r2 */
L8c6199ee: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6199f0; /* shll r4 */
L8c6199f0: r[4]+=r[3];goto L8c6199f2; /* add r3,r4 */
L8c6199f2: r[3]=rd(m,bus,&io_status,0x8c619c0cu,4);if(io_status)return io_status;goto L8c6199f4; /* mov.l 0x8c619c0c,r3 */
L8c6199f4: fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c6199f6; /* fmov.s @r4,fr3 */
L8c6199f6: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6199f8; /* fmov.s @r3,fr2 */
L8c6199f8: r[3]+=8u;goto L8c6199fa; /* add #0x8,r3 */
L8c6199fa: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c6199fc; /* fmov.s @r2,fr1 */
L8c6199fc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6199fe; /* fmul fr3,fr2 */
L8c6199fe: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c619a00; /* fmov.s @r1,fr3 */
L8c619a00: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619a02; /* fsub fr2,fr3 */
L8c619a02: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c619a04; /* fmov.s @(r0,r4),fr2 */
L8c619a04: r[0]=8u;goto L8c619a06; /* mov #0x8,r0 */
L8c619a06: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619a08; /* fmul fr2,fr1 */
L8c619a08: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c619a0a; /* fmov.s @(r0,r4),fr2 */
L8c619a0a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619a0c; /* fsub fr1,fr3 */
L8c619a0c: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619a0e; /* fmov.s @r3,fr1 */
L8c619a0e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619a10; /* fmul fr2,fr1 */
L8c619a10: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619a12; /* fsub fr1,fr3 */
L8c619a12: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a14; /* fmov.s fr3,@-r8 */
L8c619a14: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619a16; /* mov.w @(0x2,r11),r0 */
L8c619a16: r[2]=r[0];goto L8c619a18; /* mov r0,r2 */
L8c619a18: *fpul=r[2];goto L8c619a1a; /* lds r2,FPUL */
L8c619a1a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619a1c; /* float FPUL,fr3 */
L8c619a1c: r[0]=24u;goto L8c619a1e; /* mov #0x18,r0 */
L8c619a1e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619a20; /* fmul fr15,fr3 */
L8c619a20: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a22; /* fmov.s fr3,@-r8 */
L8c619a22: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619a24; /* mov.w @r11,r2 */
L8c619a24: r[11]+=4u;goto L8c619a26; /* add #0x4,r11 */
L8c619a26: *fpul=r[2];goto L8c619a28; /* lds r2,FPUL */
L8c619a28: r[2]=rd(m,bus,&io_status,0x8c619c18u,4);if(io_status)return io_status;goto L8c619a2a; /* mov.l 0x8c619c18,r2 */
L8c619a2a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619a2c; /* float FPUL,fr3 */
L8c619a2c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619a2e; /* fmul fr15,fr3 */
L8c619a2e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a30; /* fmov.s fr3,@-r8 */
L8c619a30: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619a32; /* fmov.s @(r0,r4),fr3 */
L8c619a32: r[0]=20u;goto L8c619a34; /* mov #0x14,r0 */
L8c619a34: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a36; /* fmov.s fr3,@-r8 */
L8c619a36: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619a38; /* fmov.s @(r0,r4),fr3 */
L8c619a38: r[0]=16u;goto L8c619a3a; /* mov #0x10,r0 */
L8c619a3a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a3c; /* fmov.s fr3,@-r8 */
L8c619a3c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619a3e; /* fmov.s @(r0,r4),fr3 */
L8c619a3e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a40; /* fmov.s fr3,@-r8 */
L8c619a40: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c619a42; /* mov.l r2,@-r8 */
L8c619a42: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619a44; /* pref @r8 */
L8c619a44: r[5]+=1u;goto L8c619a46; /* add #0x1,r5 */
L8c619a46: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c619a48; /* cmp/ge r6,r5 */
L8c619a48: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c6199da;goto L8c619a4c; /* bf/s 0x8c6199da */
L8c619a4c: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c619a4e; /* mov.w @r10+,r4 */
L8c619a4e: r[0]=4u;goto L8c619a50; /* mov #0x4,r0 */
L8c619a50: r[2]=rd(m,bus,&io_status,0x8c619c08u,4);if(io_status)return io_status;goto L8c619a52; /* mov.l 0x8c619c08,r2 */
L8c619a52: r[8]+=28u;goto L8c619a54; /* add #0x1c,r8 */
L8c619a54: r[4]=r[4]&65535u;goto L8c619a56; /* extu.w r4,r4 */
L8c619a56: r[1]=rd(m,bus,&io_status,0x8c619c10u,4);if(io_status)return io_status;goto L8c619a58; /* mov.l 0x8c619c10,r1 */
L8c619a58: r[4]<<=2;goto L8c619a5a; /* shll2 r4 */
L8c619a5a: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c619a5c; /* mov.l @r2,r3 */
L8c619a5c: r[4]<<=2;goto L8c619a5e; /* shll2 r4 */
L8c619a5e: r[2]=rd(m,bus,&io_status,0x8c619c14u,4);if(io_status)return io_status;goto L8c619a60; /* mov.l 0x8c619c14,r2 */
L8c619a60: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c619a62; /* shll r4 */
L8c619a62: r[4]+=r[3];goto L8c619a64; /* add r3,r4 */
L8c619a64: r[3]=rd(m,bus,&io_status,0x8c619c0cu,4);if(io_status)return io_status;goto L8c619a66; /* mov.l 0x8c619c0c,r3 */
L8c619a66: fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c619a68; /* fmov.s @r4,fr3 */
L8c619a68: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c619a6a; /* fmov.s @r3,fr2 */
L8c619a6a: r[3]+=8u;goto L8c619a6c; /* add #0x8,r3 */
L8c619a6c: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c619a6e; /* fmov.s @r2,fr1 */
L8c619a6e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619a70; /* fmul fr3,fr2 */
L8c619a70: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c619a72; /* fmov.s @r1,fr3 */
L8c619a72: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619a74; /* fsub fr2,fr3 */
L8c619a74: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c619a76; /* fmov.s @(r0,r4),fr2 */
L8c619a76: r[0]=8u;goto L8c619a78; /* mov #0x8,r0 */
L8c619a78: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619a7a; /* fmul fr2,fr1 */
L8c619a7a: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c619a7c; /* fmov.s @(r0,r4),fr2 */
L8c619a7c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619a7e; /* fsub fr1,fr3 */
L8c619a7e: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619a80; /* fmov.s @r3,fr1 */
L8c619a80: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619a82; /* fmul fr2,fr1 */
L8c619a82: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619a84; /* fsub fr1,fr3 */
L8c619a84: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a86; /* fmov.s fr3,@-r8 */
L8c619a86: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619a88; /* mov.w @(0x2,r11),r0 */
L8c619a88: r[2]=r[0];goto L8c619a8a; /* mov r0,r2 */
L8c619a8a: *fpul=r[2];goto L8c619a8c; /* lds r2,FPUL */
L8c619a8c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619a8e; /* float FPUL,fr3 */
L8c619a8e: r[0]=24u;goto L8c619a90; /* mov #0x18,r0 */
L8c619a90: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619a92; /* fmul fr15,fr3 */
L8c619a92: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a94; /* fmov.s fr3,@-r8 */
L8c619a94: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619a96; /* mov.w @r11,r2 */
L8c619a96: r[11]+=4u;goto L8c619a98; /* add #0x4,r11 */
L8c619a98: *fpul=r[2];goto L8c619a9a; /* lds r2,FPUL */
L8c619a9a: r[2]=rd(m,bus,&io_status,0x8c619c1cu,4);if(io_status)return io_status;goto L8c619a9c; /* mov.l 0x8c619c1c,r2 */
L8c619a9c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619a9e; /* float FPUL,fr3 */
L8c619a9e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619aa0; /* fmul fr15,fr3 */
L8c619aa0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619aa2; /* fmov.s fr3,@-r8 */
L8c619aa2: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619aa4; /* fmov.s @(r0,r4),fr3 */
L8c619aa4: r[0]=20u;goto L8c619aa6; /* mov #0x14,r0 */
L8c619aa6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619aa8; /* fmov.s fr3,@-r8 */
L8c619aa8: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619aaa; /* fmov.s @(r0,r4),fr3 */
L8c619aaa: r[0]=16u;goto L8c619aac; /* mov #0x10,r0 */
L8c619aac: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619aae; /* fmov.s fr3,@-r8 */
L8c619aae: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619ab0; /* fmov.s @(r0,r4),fr3 */
L8c619ab0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619ab2; /* fmov.s fr3,@-r8 */
L8c619ab2: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c619ab4; /* mov.l r2,@-r8 */
L8c619ab4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619ab6; /* pref @r8 */
L8c619ab6: r[8]+=32u;goto L8c619b8a; /* bra 0x8c619b8a */
L8c619aba: r[1]=rd(m,bus,&io_status,0x8c619c20u,4);if(io_status)return io_status;goto L8c619abc; /* mov.l 0x8c619c20,r1 */
L8c619abc: r[2]=0u;goto L8c619abe; /* mov #0x0,r2 */
L8c619abe: r[0]=rd(m,bus,&io_status,0x8c619c08u,4);if(io_status)return io_status;goto L8c619ac0; /* mov.l 0x8c619c08,r0 */
L8c619ac0: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c619ac2; /* mov.l r2,@r1 */
L8c619ac2: r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c619ac4; /* mov.w @r10+,r4 */
L8c619ac4: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c619ac6; /* mov.l @r0,r2 */
L8c619ac6: r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c619ac8; /* mov.w @r10+,r14 */
L8c619ac8: r[4]=r[4]&65535u;goto L8c619aca; /* extu.w r4,r4 */
L8c619aca: r[4]<<=2;goto L8c619acc; /* shll2 r4 */
L8c619acc: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c619ace; /* mov.l @(0xc,r15),r3 */
L8c619ace: r[14]=r[14]&65535u;goto L8c619ad0; /* extu.w r14,r14 */
L8c619ad0: r[14]<<=2;goto L8c619ad2; /* shll2 r14 */
L8c619ad2: r[14]<<=2;goto L8c619ad4; /* shll2 r14 */
L8c619ad4: r[4]<<=2;goto L8c619ad6; /* shll2 r4 */
L8c619ad6: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c619ad8; /* shll r4 */
L8c619ad8: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c619ada; /* shll r14 */
L8c619ada: r[14]+=r[2];goto L8c619adc; /* add r2,r14 */
L8c619adc: r[4]+=r[2];goto L8c619ade; /* add r2,r4 */
L8c619ade: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619ae0; /* mov.w @r11,r2 */
L8c619ae0: *fpul=r[2];goto L8c619ae2; /* lds r2,FPUL */
L8c619ae2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619ae4; /* float FPUL,fr3 */
L8c619ae4: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619ae6; /* ftrc fr3,FPUL */
L8c619ae6: r[1]=*fpul;goto L8c619ae8; /* sts FPUL,r1 */
L8c619ae8: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c619aea; /* mov.w r1,@r3 */
L8c619aea: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619aec; /* mov.w @(0x2,r11),r0 */
L8c619aec: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c619aee; /* mov.l @(0xc,r15),r3 */
L8c619aee: r[2]=r[0];goto L8c619af0; /* mov r0,r2 */
L8c619af0: *fpul=r[2];goto L8c619af2; /* lds r2,FPUL */
L8c619af2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619af4; /* float FPUL,fr3 */
L8c619af4: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619af6; /* ftrc fr3,FPUL */
L8c619af6: r[0]=*fpul;goto L8c619af8; /* sts FPUL,r0 */
L8c619af8: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619afa; /* mov.w r0,@(0x2,r3) */
L8c619afa: r[11]+=4u;goto L8c619afc; /* add #0x4,r11 */
L8c619afc: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c619afe; /* mov.l @r15,r3 */
L8c619afe: r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619b00; /* mov.w @r11,r2 */
L8c619b00: r[9]=r[13];goto L8c619b02; /* mov r13,r9 */
L8c619b02: r[9]+=4294967294u;goto L8c619b04; /* add #-0x2,r9 */
L8c619b04: *fpul=r[2];goto L8c619b06; /* lds r2,FPUL */
L8c619b06: *sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c619b08; /* cmp/pl r9 */
L8c619b08: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619b0a; /* float FPUL,fr3 */
L8c619b0a: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619b0c; /* ftrc fr3,FPUL */
L8c619b0c: r[1]=*fpul;goto L8c619b0e; /* sts FPUL,r1 */
L8c619b0e: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c619b10; /* mov.w r1,@r3 */
L8c619b10: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619b12; /* mov.w @(0x2,r11),r0 */
L8c619b12: r[11]+=4u;goto L8c619b14; /* add #0x4,r11 */
L8c619b14: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c619b16; /* mov.l @r15,r3 */
L8c619b16: r[2]=r[0];goto L8c619b18; /* mov r0,r2 */
L8c619b18: *fpul=r[2];goto L8c619b1a; /* lds r2,FPUL */
L8c619b1a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619b1c; /* float FPUL,fr3 */
L8c619b1c: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619b1e; /* ftrc fr3,FPUL */
L8c619b1e: r[0]=*fpul;goto L8c619b20; /* sts FPUL,r0 */
L8c619b20: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619b22; /* mov.w r0,@(0x2,r3) */
L8c619b22: take=!(*sr&1u);r[12]=0u;if(take)goto L8c619b8a;goto L8c619b26; /* bf/s 0x8c619b8a */
L8c619b26: r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c619b28; /* mov.w @r10+,r13 */
L8c619b28: r[2]=rd(m,bus,&io_status,0x8c619c08u,4);if(io_status)return io_status;goto L8c619b2a; /* mov.l 0x8c619c08,r2 */
L8c619b2a: r[13]=r[13]&65535u;goto L8c619b2c; /* extu.w r13,r13 */
L8c619b2c: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c619b2e; /* mov.l @(0x4,r15),r1 */
L8c619b2e: r[13]<<=2;goto L8c619b30; /* shll2 r13 */
L8c619b30: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c619b32; /* mov.l @r2,r3 */
L8c619b32: r[13]<<=2;goto L8c619b34; /* shll2 r13 */
L8c619b34: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c619b36; /* shll r13 */
L8c619b36: r[13]+=r[3];goto L8c619b38; /* add r3,r13 */
L8c619b38: r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619b3a; /* mov.w @r11,r3 */
L8c619b3a: *fpul=r[3];goto L8c619b3c; /* lds r3,FPUL */
L8c619b3c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619b3e; /* float FPUL,fr3 */
L8c619b3e: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619b40; /* ftrc fr3,FPUL */
L8c619b40: r[0]=*fpul;goto L8c619b42; /* sts FPUL,r0 */
L8c619b42: wr(m,bus,&io_status,r[1],r[0],2);if(io_status)return io_status;goto L8c619b44; /* mov.w r0,@r1 */
L8c619b44: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619b46; /* mov.w @(0x2,r11),r0 */
L8c619b46: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c619b48; /* mov.l @(0x4,r15),r3 */
L8c619b48: r[1]=r[0];goto L8c619b4a; /* mov r0,r1 */
L8c619b4a: *fpul=r[1];goto L8c619b4c; /* lds r1,FPUL */
L8c619b4c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619b4e; /* float FPUL,fr3 */
L8c619b4e: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619b50; /* ftrc fr3,FPUL */
L8c619b50: r[0]=*fpul;goto L8c619b52; /* sts FPUL,r0 */
L8c619b52: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619b54; /* mov.w r0,@(0x2,r3) */
L8c619b54: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c619b56; /* mov.l @(0xc,r4),r5 */
L8c619b56: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c619b58; /* mov.l @(0xc,r14),r3 */
L8c619b58: r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c619b5a; /* mov.l @(0xc,r13),r1 */
L8c619b5a: r[5]&=r[3];goto L8c619b5c; /* and r3,r5 */
L8c619b5c: r[3]=1u;goto L8c619b5e; /* mov #0x1,r3 */
L8c619b5e: r[5]&=r[1];goto L8c619b60; /* and r1,r5 */
L8c619b60: *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c619b62; /* tst r3,r5 */
L8c619b62: take=!(*sr&1u);r[11]+=4u;if(take)goto L8c619b70;goto L8c619b66; /* bf/s 0x8c619b70 */
L8c619b66: r[1]=rd(m,bus,&io_status,0x8c619c28u,4);if(io_status)return io_status;goto L8c619b68; /* mov.l 0x8c619c28,r1 */
L8c619b68: r[6]=r[13];goto L8c619b6a; /* mov r13,r6 */
L8c619b6a: r[7]=rd(m,bus,&io_status,0x8c619c24u,4);if(io_status)return io_status;goto L8c619b6c; /* mov.l 0x8c619c24,r7 */
L8c619b6c: target=r[1];*pr=0x8c619b70u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c619b70; /* jsr @r1 */
L8c619b70: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c619b72; /* mov.l @r15,r2 */
L8c619b72: r[12]+=1u;goto L8c619b74; /* add #0x1,r12 */
L8c619b74: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c619b76; /* mov.l @(0xc,r15),r3 */
L8c619b76: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[9]);goto L8c619b78; /* cmp/ge r9,r12 */
L8c619b78: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c619b7a; /* mov.l @r2,r1 */
L8c619b7a: r[4]=r[14];goto L8c619b7c; /* mov r14,r4 */
L8c619b7c: wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c619b7e; /* mov.l r1,@r3 */
L8c619b7e: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c619b80; /* mov.l @(0x4,r15),r3 */
L8c619b80: r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c619b82; /* mov.l @r15,r0 */
L8c619b82: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c619b84; /* mov.l @r3,r2 */
L8c619b84: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c619b86; /* mov.l r2,@r0 */
L8c619b86: take=!(*sr&1u);r[14]=r[13];if(take)goto L8c619b26;goto L8c619b8a; /* bf/s 0x8c619b26 */
L8c619b8a: r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c619b8c; /* mov.l @(0x10,r15),r2 */
L8c619b8c: r[2]+=1u;goto L8c619b8e; /* add #0x1,r2 */
L8c619b8e: wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c619b90; /* mov.l r2,@(0x10,r15) */
L8c619b90: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c619b92; /* mov.l @(0x8,r15),r3 */
L8c619b92: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c619b94; /* mov.w @(0x2,r3),r0 */
L8c619b94: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c619b96; /* mov.l @(0x10,r15),r3 */
L8c619b96: r[0]=r[0]&65535u;goto L8c619b98; /* extu.w r0,r0 */
L8c619b98: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c619b9a; /* cmp/ge r0,r3 */
L8c619b9a: if((*sr&1u))goto L8c619ba0;goto L8c619b9c; /* bt 0x8c619ba0 */
L8c619b9c: goto L8c6198e8; /* bra 0x8c6198e8 */
L8c619ba0: goto L8c619be2; /* bra 0x8c619be2 */
L8c619ba4: r[1]=rd(m,bus,&io_status,0x8c619c2cu,4);if(io_status)return io_status;goto L8c619ba6; /* mov.l 0x8c619c2c,r1 */
L8c619ba6: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c619ba8; /* mov.l @(0x8,r15),r2 */
L8c619ba8: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c619baa; /* mov.l r2,@r1 */
L8c619baa: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c619bac; /* frchg */
L8c619bac: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c619bae; /* fmov.s fr0,@-r15 */
L8c619bae: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c619bb0; /* fmov.s fr1,@-r15 */
L8c619bb0: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c619bb2; /* fmov.s fr2,@-r15 */
L8c619bb2: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c619bb4; /* fmov.s fr3,@-r15 */
L8c619bb4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c619bb6; /* fmov.s fr4,@-r15 */
L8c619bb6: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c619bb8; /* fmov.s fr5,@-r15 */
L8c619bb8: r[0]=rd(m,bus,&io_status,0x8c619bf8u,4);if(io_status)return io_status;goto L8c619bba; /* mov.l 0x8c619bf8,r0 */
L8c619bba: fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619bbc; /* fmov.s @r0+,fr0 */
L8c619bbc: fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619bbe; /* fmov.s @r0+,fr1 */
L8c619bbe: fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c619bc0; /* fmov.s @r0,fr2 */
L8c619bc0: r[0]=rd(m,bus,&io_status,0x8c619bfcu,4);if(io_status)return io_status;goto L8c619bc2; /* mov.l 0x8c619bfc,r0 */
L8c619bc2: fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c619bc4; /* fmov.s @r0,fr3 */
L8c619bc4: r[0]=0x8c619c00u;goto L8c619bc6; /* mova 0x8c619c00,r0 */
L8c619bc6: fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c619bc8; /* fmov.s @r0,fr4 */
L8c619bc8: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c619bca; /* frchg */
L8c619bca: r[2]=rd(m,bus,&io_status,0x8c619c30u,4);if(io_status)return io_status;goto L8c619bcc; /* mov.l 0x8c619c30,r2 */
L8c619bcc: r[3]=rd(m,bus,&io_status,0x8c619c2cu,4);if(io_status)return io_status;goto L8c619bce; /* mov.l 0x8c619c2c,r3 */
L8c619bce: target=r[2];*pr=0x8c619bd2u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c619bd2; /* jsr @r2 */
L8c619bd2: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c619bd4; /* frchg */
L8c619bd4: fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619bd6; /* fmov.s @r15+,fr5 */
L8c619bd6: fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619bd8; /* fmov.s @r15+,fr4 */
L8c619bd8: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619bda; /* fmov.s @r15+,fr3 */
L8c619bda: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619bdc; /* fmov.s @r15+,fr2 */
L8c619bdc: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619bde; /* fmov.s @r15+,fr1 */
L8c619bde: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619be0; /* fmov.s @r15+,fr0 */
L8c619be0: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c619be2; /* frchg */
L8c619be2: r[15]+=24u;goto L8c619be4; /* add #0x18,r15 */
L8c619be4: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c619be6; /* lds.l @r15+,PR */
L8c619be6: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619be8; /* fmov.s @r15+,fr15 */
L8c619be8: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c619bea; /* mov.l @r15+,r9 */
L8c619bea: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c619bec; /* mov.l @r15+,r10 */
L8c619bec: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c619bee; /* mov.l @r15+,r11 */
L8c619bee: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c619bf0; /* mov.l @r15+,r12 */
L8c619bf0: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c619bf2; /* mov.l @r15+,r13 */
L8c619bf2: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
