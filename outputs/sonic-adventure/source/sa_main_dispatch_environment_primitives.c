/* Normal-derived environment UV primitives with portable queues; names inferred.
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
SA_STAGE_EXPORT int sa_environment_primitive_emit_8c619c34(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c619c34;
L8c619c34: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c619c36; /* mov.l r14,@-r15 */
L8c619c36: r[0]=0x8c619c94u;goto L8c619c38; /* mova 0x8c619c94,r0 */
L8c619c38: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c619c3a; /* mov.l r13,@-r15 */
L8c619c3a: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c619c3c; /* mov.l r12,@-r15 */
L8c619c3c: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c619c3e; /* mov.l r11,@-r15 */
L8c619c3e: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c619c40; /* mov.l r10,@-r15 */
L8c619c40: r[3]=rd(m,bus,&io_status,0x8c619ca0u,4);if(io_status)return io_status;goto L8c619c42; /* mov.l 0x8c619ca0,r3 */
L8c619c42: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c619c44; /* mov.l r9,@-r15 */
L8c619c44: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c619c46; /* sts.l PR,@-r15 */
L8c619c46: fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c619c48; /* fmov.s @r0,fr6 */
L8c619c48: r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c619c4a; /* mov.w @r4,r0 */
L8c619c4a: r[13]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c619c4c; /* mov.l @(0x4,r4),r13 */
L8c619c4c: r[0]=r[0]&65535u;goto L8c619c4e; /* extu.w r0,r0 */
L8c619c4e: r[9]=rd(m,bus,&io_status,0x8c619c98u,4);if(io_status)return io_status;goto L8c619c50; /* mov.l 0x8c619c98,r9 */
L8c619c50: r[0]&=r[3];goto L8c619c52; /* and r3,r0 */
L8c619c52: r[11]=rd(m,bus,&io_status,0x8c619c9cu,4);if(io_status)return io_status;goto L8c619c54; /* mov.l 0x8c619c9c,r11 */
L8c619c54: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c619c56; /* cmp/eq #0x0,r0 */
L8c619c56: take=(*sr&1u);r[12]=0u;if(take)goto L8c619c7c;goto L8c619c5a; /* bt/s 0x8c619c7c */
L8c619c5a: r[1]=sx(m,bus,&io_status,0x8c619c90u,2);if(io_status)return io_status;goto L8c619c5c; /* mov.w 0x8c619c90,r1 */
L8c619c5c: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c619c5e; /* cmp/eq r1,r0 */
L8c619c5e: if(!(*sr&1u))goto L8c619c64;goto L8c619c60; /* bf 0x8c619c64 */
L8c619c60: goto L8c619ed8; /* bra 0x8c619ed8 */
L8c619c64: r[1]=rd(m,bus,&io_status,0x8c619ca4u,4);if(io_status)return io_status;goto L8c619c66; /* mov.l 0x8c619ca4,r1 */
L8c619c66: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c619c68; /* cmp/eq r1,r0 */
L8c619c68: if(!(*sr&1u))goto L8c619c6e;goto L8c619c6a; /* bf 0x8c619c6e */
L8c619c6a: goto L8c61a1d4; /* bra 0x8c61a1d4 */
L8c619c6e: r[1]=rd(m,bus,&io_status,0x8c619ca0u,4);if(io_status)return io_status;goto L8c619c70; /* mov.l 0x8c619ca0,r1 */
L8c619c70: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c619c72; /* cmp/eq r1,r0 */
L8c619c72: if(!(*sr&1u))goto L8c619c78;goto L8c619c74; /* bf 0x8c619c78 */
L8c619c74: goto L8c61a3ce; /* bra 0x8c61a3ce */
L8c619c78: goto L8c61a446; /* bra 0x8c61a446 */
L8c619c7c: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c619c7e; /* mov.w @(0x2,r4),r0 */
L8c619c7e: r[1]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c619c80; /* mov.l @r11,r1 */
L8c619c80: r[0]=r[0]&65535u;goto L8c619c82; /* extu.w r0,r0 */
L8c619c82: r[2]=r[0];goto L8c619c84; /* mov r0,r2 */
L8c619c84: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c619c86; /* shll r0 */
L8c619c86: r[0]+=r[2];goto L8c619c88; /* add r2,r0 */
L8c619c88: r[1]+=r[0];goto L8c619c8a; /* add r0,r1 */
L8c619c8a: wr(m,bus,&io_status,r[11],r[1],4);if(io_status)return io_status;goto L8c619c8c; /* mov.l r1,@r11 */
L8c619c8c: r[14]=r[12];goto L8c619ec8; /* bra 0x8c619ec8 */
L8c619ca8: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c619caa; /* mov.w @r13+,r5 */
L8c619caa: r[0]=16u;goto L8c619cac; /* mov #0x10,r0 */
L8c619cac: r[3]=rd(m,bus,&io_status,0x8c619ee8u,4);if(io_status)return io_status;goto L8c619cae; /* mov.l 0x8c619ee8,r3 */
L8c619cae: r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c619cb0; /* mov.w @r13+,r6 */
L8c619cb0: r[5]=r[5]&65535u;goto L8c619cb2; /* extu.w r5,r5 */
L8c619cb2: r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c619cb4; /* mov.l @r3,r12 */
L8c619cb4: r[5]<<=2;goto L8c619cb6; /* shll2 r5 */
L8c619cb6: r[6]=r[6]&65535u;goto L8c619cb8; /* extu.w r6,r6 */
L8c619cb8: r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c619cba; /* mov.w @r13+,r7 */
L8c619cba: r[6]<<=2;goto L8c619cbc; /* shll2 r6 */
L8c619cbc: r[6]<<=2;goto L8c619cbe; /* shll2 r6 */
L8c619cbe: r[5]<<=2;goto L8c619cc0; /* shll2 r5 */
L8c619cc0: r[7]=r[7]&65535u;goto L8c619cc2; /* extu.w r7,r7 */
L8c619cc2: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c619cc4; /* shll r5 */
L8c619cc4: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c619cc6; /* shll r6 */
L8c619cc6: r[6]+=r[12];goto L8c619cc8; /* add r12,r6 */
L8c619cc8: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c619cca; /* fmov.s @(r0,r6),fr4 */
L8c619cca: r[5]+=r[12];goto L8c619ccc; /* add r12,r5 */
L8c619ccc: r[7]<<=2;goto L8c619cce; /* shll2 r7 */
L8c619cce: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619cd0; /* fmov.s @(r0,r5),fr3 */
L8c619cd0: r[7]<<=2;goto L8c619cd2; /* shll2 r7 */
L8c619cd2: fcopy(f,2u,4u);goto L8c619cd4; /* fmov fr4,fr2 */
L8c619cd4: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c619cd6; /* shll r7 */
L8c619cd6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c619cd8; /* fsub fr3,fr2 */
L8c619cd8: r[0]=20u;goto L8c619cda; /* mov #0x14,r0 */
L8c619cda: r[7]+=r[12];goto L8c619cdc; /* add r12,r7 */
L8c619cdc: fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c619cde; /* fmov.s @(r0,r6),fr5 */
L8c619cde: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619ce0; /* fmov.s @(r0,r7),fr3 */
L8c619ce0: fcopy(f,1u,5u);goto L8c619ce2; /* fmov fr5,fr1 */
L8c619ce2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619ce4; /* fsub fr5,fr3 */
L8c619ce4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619ce6; /* fmul fr3,fr2 */
L8c619ce6: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619ce8; /* fmov.s @(r0,r5),fr3 */
L8c619ce8: r[0]=16u;goto L8c619cea; /* mov #0x10,r0 */
L8c619cea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c619cec; /* fsub fr3,fr1 */
L8c619cec: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619cee; /* fmov.s @(r0,r7),fr3 */
L8c619cee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619cf0; /* fsub fr4,fr3 */
L8c619cf0: fcopy(f,4u,2u);goto L8c619cf2; /* fmov fr2,fr4 */
L8c619cf2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619cf4; /* fmul fr3,fr1 */
L8c619cf4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c619cf6; /* fldi0 fr3 */
L8c619cf6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c619cf8; /* fsub fr1,fr4 */
L8c619cf8: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c619cfa; /* fcmp/gt fr3,fr4 */
L8c619cfa: take=!(*sr&1u);r[14]+=1u;if(take)goto L8c619d02;goto L8c619cfe; /* bf/s 0x8c619d02 */
L8c619cfe: goto L8c619ec8; /* bra 0x8c619ec8 */
L8c619d02: r[3]=rd(m,bus,&io_status,0x8c619eecu,4);if(io_status)return io_status;goto L8c619d04; /* mov.l 0x8c619eec,r3 */
L8c619d04: r[0]=4u;goto L8c619d06; /* mov #0x4,r0 */
L8c619d06: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c619d08; /* fmov.s @r5,fr3 */
L8c619d08: r[8]+=28u;goto L8c619d0a; /* add #0x1c,r8 */
L8c619d0a: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c619d0c; /* fmov.s @r3,fr2 */
L8c619d0c: r[3]+=8u;goto L8c619d0e; /* add #0x8,r3 */
L8c619d0e: r[2]=rd(m,bus,&io_status,0x8c619ef0u,4);if(io_status)return io_status;goto L8c619d10; /* mov.l 0x8c619ef0,r2 */
L8c619d10: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619d12; /* fmul fr3,fr2 */
L8c619d12: r[1]=rd(m,bus,&io_status,0x8c619ef4u,4);if(io_status)return io_status;goto L8c619d14; /* mov.l 0x8c619ef4,r1 */
L8c619d14: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c619d16; /* fmov.s @r2,fr3 */
L8c619d16: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c619d18; /* fmov.s @r1,fr1 */
L8c619d18: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619d1a; /* fsub fr2,fr3 */
L8c619d1a: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d1c; /* fmov.s @(r0,r5),fr2 */
L8c619d1c: r[0]=8u;goto L8c619d1e; /* mov #0x8,r0 */
L8c619d1e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619d20; /* fmul fr2,fr1 */
L8c619d20: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d22; /* fmov.s @(r0,r5),fr2 */
L8c619d22: r[0]=4u;goto L8c619d24; /* mov #0x4,r0 */
L8c619d24: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619d26; /* fsub fr1,fr3 */
L8c619d26: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619d28; /* fmov.s @r3,fr1 */
L8c619d28: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619d2a; /* fmul fr2,fr1 */
L8c619d2a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619d2c; /* fsub fr1,fr3 */
L8c619d2c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619d2e; /* fmov.s fr3,@-r8 */
L8c619d2e: r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619d30; /* mov.l @r9,r2 */
L8c619d30: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d32; /* fmov.s @(r0,r5),fr0 */
L8c619d32: r[0]=20u;goto L8c619d34; /* mov #0x14,r0 */
L8c619d34: r[1]=r[2];goto L8c619d36; /* mov r2,r1 */
L8c619d36: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619d38; /* fmov.s @(r0,r1),fr3 */
L8c619d38: r[0]=4u;goto L8c619d3a; /* mov #0x4,r0 */
L8c619d3a: r[3]=r[2];goto L8c619d3c; /* mov r2,r3 */
L8c619d3c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619d3e; /* fmul fr0,fr3 */
L8c619d3e: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619d40; /* fmov.s @(r0,r3),fr2 */
L8c619d40: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c619d42; /* fmov.s @r5,fr0 */
L8c619d42: r[0]=8u;goto L8c619d44; /* mov #0x8,r0 */
L8c619d44: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619d46; /* fmac fr0,fr2,fr3 */
L8c619d46: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d48; /* fmov.s @(r0,r5),fr0 */
L8c619d48: r[0]=36u;goto L8c619d4a; /* mov #0x24,r0 */
L8c619d4a: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c619d4c; /* fmov.s @(r0,r2),fr2 */
L8c619d4c: r[0]=4u;goto L8c619d4e; /* mov #0x4,r0 */
L8c619d4e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619d50; /* fmac fr0,fr2,fr3 */
L8c619d50: fcopy(f,0u,6u);goto L8c619d52; /* fmov fr6,fr0 */
L8c619d52: fcopy(f,2u,6u);goto L8c619d54; /* fmov fr6,fr2 */
L8c619d54: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619d56; /* fmac fr0,fr3,fr2 */
L8c619d56: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619d58; /* fmov.s fr2,@-r8 */
L8c619d58: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619d5a; /* mov.l @r9,r3 */
L8c619d5a: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d5c; /* fmov.s @(r0,r5),fr0 */
L8c619d5c: r[0]=16u;goto L8c619d5e; /* mov #0x10,r0 */
L8c619d5e: r[1]=r[3];goto L8c619d60; /* mov r3,r1 */
L8c619d60: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619d62; /* fmov.s @(r0,r1),fr3 */
L8c619d62: r[2]=r[3];goto L8c619d64; /* mov r3,r2 */
L8c619d64: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c619d66; /* fmov.s @r2,fr2 */
L8c619d66: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619d68; /* fmul fr0,fr3 */
L8c619d68: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c619d6a; /* fmov.s @r5,fr0 */
L8c619d6a: r[0]=8u;goto L8c619d6c; /* mov #0x8,r0 */
L8c619d6c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619d6e; /* fmac fr0,fr2,fr3 */
L8c619d6e: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d70; /* fmov.s @(r0,r5),fr0 */
L8c619d70: r[0]=32u;goto L8c619d72; /* mov #0x20,r0 */
L8c619d72: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619d74; /* fmov.s @(r0,r3),fr2 */
L8c619d74: r[0]=24u;goto L8c619d76; /* mov #0x18,r0 */
L8c619d76: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619d78; /* fmac fr0,fr2,fr3 */
L8c619d78: fcopy(f,0u,6u);goto L8c619d7a; /* fmov fr6,fr0 */
L8c619d7a: fcopy(f,2u,6u);goto L8c619d7c; /* fmov fr6,fr2 */
L8c619d7c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619d7e; /* fmac fr0,fr3,fr2 */
L8c619d7e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619d80; /* fmov.s fr2,@-r8 */
L8c619d80: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d82; /* fmov.s @(r0,r5),fr3 */
L8c619d82: r[0]=20u;goto L8c619d84; /* mov #0x14,r0 */
L8c619d84: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619d86; /* fmov.s fr3,@-r8 */
L8c619d86: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d88; /* fmov.s @(r0,r5),fr3 */
L8c619d88: r[0]=16u;goto L8c619d8a; /* mov #0x10,r0 */
L8c619d8a: r[3]=rd(m,bus,&io_status,0x8c619ef8u,4);if(io_status)return io_status;goto L8c619d8c; /* mov.l 0x8c619ef8,r3 */
L8c619d8c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619d8e; /* fmov.s fr3,@-r8 */
L8c619d8e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d90; /* fmov.s @(r0,r5),fr3 */
L8c619d90: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619d92; /* fmov.s fr3,@-r8 */
L8c619d92: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c619d94; /* mov.l r3,@-r8 */
L8c619d94: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619d96; /* pref @r8 */
L8c619d96: r[3]=rd(m,bus,&io_status,0x8c619eecu,4);if(io_status)return io_status;goto L8c619d98; /* mov.l 0x8c619eec,r3 */
L8c619d98: r[0]=4u;goto L8c619d9a; /* mov #0x4,r0 */
L8c619d9a: fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c619d9c; /* fmov.s @r6,fr3 */
L8c619d9c: r[8]+=60u;goto L8c619d9e; /* add #0x3c,r8 */
L8c619d9e: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c619da0; /* fmov.s @r3,fr2 */
L8c619da0: r[3]+=8u;goto L8c619da2; /* add #0x8,r3 */
L8c619da2: r[2]=rd(m,bus,&io_status,0x8c619ef0u,4);if(io_status)return io_status;goto L8c619da4; /* mov.l 0x8c619ef0,r2 */
L8c619da4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619da6; /* fmul fr3,fr2 */
L8c619da6: r[1]=rd(m,bus,&io_status,0x8c619ef4u,4);if(io_status)return io_status;goto L8c619da8; /* mov.l 0x8c619ef4,r1 */
L8c619da8: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c619daa; /* fmov.s @r2,fr3 */
L8c619daa: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c619dac; /* fmov.s @r1,fr1 */
L8c619dac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619dae; /* fsub fr2,fr3 */
L8c619dae: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c619db0; /* fmov.s @(r0,r6),fr2 */
L8c619db0: r[0]=8u;goto L8c619db2; /* mov #0x8,r0 */
L8c619db2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619db4; /* fmul fr2,fr1 */
L8c619db4: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c619db6; /* fmov.s @(r0,r6),fr2 */
L8c619db6: r[0]=4u;goto L8c619db8; /* mov #0x4,r0 */
L8c619db8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619dba; /* fsub fr1,fr3 */
L8c619dba: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619dbc; /* fmov.s @r3,fr1 */
L8c619dbc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619dbe; /* fmul fr2,fr1 */
L8c619dbe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619dc0; /* fsub fr1,fr3 */
L8c619dc0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619dc2; /* fmov.s fr3,@-r8 */
L8c619dc2: r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619dc4; /* mov.l @r9,r2 */
L8c619dc4: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619dc6; /* fmov.s @(r0,r6),fr0 */
L8c619dc6: r[0]=20u;goto L8c619dc8; /* mov #0x14,r0 */
L8c619dc8: r[1]=r[2];goto L8c619dca; /* mov r2,r1 */
L8c619dca: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619dcc; /* fmov.s @(r0,r1),fr3 */
L8c619dcc: r[0]=4u;goto L8c619dce; /* mov #0x4,r0 */
L8c619dce: r[3]=r[2];goto L8c619dd0; /* mov r2,r3 */
L8c619dd0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619dd2; /* fmul fr0,fr3 */
L8c619dd2: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619dd4; /* fmov.s @(r0,r3),fr2 */
L8c619dd4: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c619dd6; /* fmov.s @r6,fr0 */
L8c619dd6: r[0]=8u;goto L8c619dd8; /* mov #0x8,r0 */
L8c619dd8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619dda; /* fmac fr0,fr2,fr3 */
L8c619dda: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619ddc; /* fmov.s @(r0,r6),fr0 */
L8c619ddc: r[0]=36u;goto L8c619dde; /* mov #0x24,r0 */
L8c619dde: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c619de0; /* fmov.s @(r0,r2),fr2 */
L8c619de0: r[0]=4u;goto L8c619de2; /* mov #0x4,r0 */
L8c619de2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619de4; /* fmac fr0,fr2,fr3 */
L8c619de4: fcopy(f,0u,6u);goto L8c619de6; /* fmov fr6,fr0 */
L8c619de6: fcopy(f,2u,6u);goto L8c619de8; /* fmov fr6,fr2 */
L8c619de8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619dea; /* fmac fr0,fr3,fr2 */
L8c619dea: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619dec; /* fmov.s fr2,@-r8 */
L8c619dec: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619dee; /* mov.l @r9,r3 */
L8c619dee: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619df0; /* fmov.s @(r0,r6),fr0 */
L8c619df0: r[0]=16u;goto L8c619df2; /* mov #0x10,r0 */
L8c619df2: r[1]=r[3];goto L8c619df4; /* mov r3,r1 */
L8c619df4: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619df6; /* fmov.s @(r0,r1),fr3 */
L8c619df6: r[2]=r[3];goto L8c619df8; /* mov r3,r2 */
L8c619df8: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c619dfa; /* fmov.s @r2,fr2 */
L8c619dfa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619dfc; /* fmul fr0,fr3 */
L8c619dfc: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c619dfe; /* fmov.s @r6,fr0 */
L8c619dfe: r[0]=8u;goto L8c619e00; /* mov #0x8,r0 */
L8c619e00: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619e02; /* fmac fr0,fr2,fr3 */
L8c619e02: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619e04; /* fmov.s @(r0,r6),fr0 */
L8c619e04: r[0]=32u;goto L8c619e06; /* mov #0x20,r0 */
L8c619e06: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619e08; /* fmov.s @(r0,r3),fr2 */
L8c619e08: r[0]=24u;goto L8c619e0a; /* mov #0x18,r0 */
L8c619e0a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619e0c; /* fmac fr0,fr2,fr3 */
L8c619e0c: fcopy(f,0u,6u);goto L8c619e0e; /* fmov fr6,fr0 */
L8c619e0e: fcopy(f,2u,6u);goto L8c619e10; /* fmov fr6,fr2 */
L8c619e10: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619e12; /* fmac fr0,fr3,fr2 */
L8c619e12: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619e14; /* fmov.s fr2,@-r8 */
L8c619e14: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619e16; /* fmov.s @(r0,r6),fr3 */
L8c619e16: r[0]=20u;goto L8c619e18; /* mov #0x14,r0 */
L8c619e18: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619e1a; /* fmov.s fr3,@-r8 */
L8c619e1a: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619e1c; /* fmov.s @(r0,r6),fr3 */
L8c619e1c: r[0]=16u;goto L8c619e1e; /* mov #0x10,r0 */
L8c619e1e: r[3]=rd(m,bus,&io_status,0x8c619ef8u,4);if(io_status)return io_status;goto L8c619e20; /* mov.l 0x8c619ef8,r3 */
L8c619e20: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619e22; /* fmov.s fr3,@-r8 */
L8c619e22: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619e24; /* fmov.s @(r0,r6),fr3 */
L8c619e24: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619e26; /* fmov.s fr3,@-r8 */
L8c619e26: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c619e28; /* mov.l r3,@-r8 */
L8c619e28: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619e2a; /* pref @r8 */
L8c619e2a: r[3]=rd(m,bus,&io_status,0x8c619eecu,4);if(io_status)return io_status;goto L8c619e2c; /* mov.l 0x8c619eec,r3 */
L8c619e2c: r[0]=4u;goto L8c619e2e; /* mov #0x4,r0 */
L8c619e2e: fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c619e30; /* fmov.s @r7,fr3 */
L8c619e30: r[8]+=60u;goto L8c619e32; /* add #0x3c,r8 */
L8c619e32: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c619e34; /* fmov.s @r3,fr2 */
L8c619e34: r[3]+=8u;goto L8c619e36; /* add #0x8,r3 */
L8c619e36: r[2]=rd(m,bus,&io_status,0x8c619ef0u,4);if(io_status)return io_status;goto L8c619e38; /* mov.l 0x8c619ef0,r2 */
L8c619e38: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619e3a; /* fmul fr3,fr2 */
L8c619e3a: r[1]=rd(m,bus,&io_status,0x8c619ef4u,4);if(io_status)return io_status;goto L8c619e3c; /* mov.l 0x8c619ef4,r1 */
L8c619e3c: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c619e3e; /* fmov.s @r2,fr3 */
L8c619e3e: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c619e40; /* fmov.s @r1,fr1 */
L8c619e40: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619e42; /* fsub fr2,fr3 */
L8c619e42: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c619e44; /* fmov.s @(r0,r7),fr2 */
L8c619e44: r[0]=8u;goto L8c619e46; /* mov #0x8,r0 */
L8c619e46: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619e48; /* fmul fr2,fr1 */
L8c619e48: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c619e4a; /* fmov.s @(r0,r7),fr2 */
L8c619e4a: r[0]=4u;goto L8c619e4c; /* mov #0x4,r0 */
L8c619e4c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619e4e; /* fsub fr1,fr3 */
L8c619e4e: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619e50; /* fmov.s @r3,fr1 */
L8c619e50: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619e52; /* fmul fr2,fr1 */
L8c619e52: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619e54; /* fsub fr1,fr3 */
L8c619e54: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619e56; /* fmov.s fr3,@-r8 */
L8c619e56: r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619e58; /* mov.l @r9,r2 */
L8c619e58: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c619e5a; /* fmov.s @(r0,r7),fr0 */
L8c619e5a: r[0]=20u;goto L8c619e5c; /* mov #0x14,r0 */
L8c619e5c: r[1]=r[2];goto L8c619e5e; /* mov r2,r1 */
L8c619e5e: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619e60; /* fmov.s @(r0,r1),fr3 */
L8c619e60: r[0]=4u;goto L8c619e62; /* mov #0x4,r0 */
L8c619e62: r[3]=r[2];goto L8c619e64; /* mov r2,r3 */
L8c619e64: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619e66; /* fmul fr0,fr3 */
L8c619e66: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619e68; /* fmov.s @(r0,r3),fr2 */
L8c619e68: fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c619e6a; /* fmov.s @r7,fr0 */
L8c619e6a: r[0]=8u;goto L8c619e6c; /* mov #0x8,r0 */
L8c619e6c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619e6e; /* fmac fr0,fr2,fr3 */
L8c619e6e: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c619e70; /* fmov.s @(r0,r7),fr0 */
L8c619e70: r[0]=36u;goto L8c619e72; /* mov #0x24,r0 */
L8c619e72: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c619e74; /* fmov.s @(r0,r2),fr2 */
L8c619e74: r[0]=4u;goto L8c619e76; /* mov #0x4,r0 */
L8c619e76: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619e78; /* fmac fr0,fr2,fr3 */
L8c619e78: fcopy(f,0u,6u);goto L8c619e7a; /* fmov fr6,fr0 */
L8c619e7a: fcopy(f,2u,6u);goto L8c619e7c; /* fmov fr6,fr2 */
L8c619e7c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619e7e; /* fmac fr0,fr3,fr2 */
L8c619e7e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619e80; /* fmov.s fr2,@-r8 */
L8c619e80: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619e82; /* mov.l @r9,r3 */
L8c619e82: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c619e84; /* fmov.s @(r0,r7),fr0 */
L8c619e84: r[0]=16u;goto L8c619e86; /* mov #0x10,r0 */
L8c619e86: r[1]=r[3];goto L8c619e88; /* mov r3,r1 */
L8c619e88: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619e8a; /* fmov.s @(r0,r1),fr3 */
L8c619e8a: r[2]=r[3];goto L8c619e8c; /* mov r3,r2 */
L8c619e8c: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c619e8e; /* fmov.s @r2,fr2 */
L8c619e8e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619e90; /* fmul fr0,fr3 */
L8c619e90: fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c619e92; /* fmov.s @r7,fr0 */
L8c619e92: r[0]=8u;goto L8c619e94; /* mov #0x8,r0 */
L8c619e94: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619e96; /* fmac fr0,fr2,fr3 */
L8c619e96: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c619e98; /* fmov.s @(r0,r7),fr0 */
L8c619e98: r[0]=32u;goto L8c619e9a; /* mov #0x20,r0 */
L8c619e9a: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619e9c; /* fmov.s @(r0,r3),fr2 */
L8c619e9c: r[0]=24u;goto L8c619e9e; /* mov #0x18,r0 */
L8c619e9e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619ea0; /* fmac fr0,fr2,fr3 */
L8c619ea0: fcopy(f,0u,6u);goto L8c619ea2; /* fmov fr6,fr0 */
L8c619ea2: fcopy(f,2u,6u);goto L8c619ea4; /* fmov fr6,fr2 */
L8c619ea4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619ea6; /* fmac fr0,fr3,fr2 */
L8c619ea6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619ea8; /* fmov.s fr2,@-r8 */
L8c619ea8: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619eaa; /* fmov.s @(r0,r7),fr3 */
L8c619eaa: r[0]=20u;goto L8c619eac; /* mov #0x14,r0 */
L8c619eac: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619eae; /* fmov.s fr3,@-r8 */
L8c619eae: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619eb0; /* fmov.s @(r0,r7),fr3 */
L8c619eb0: r[0]=16u;goto L8c619eb2; /* mov #0x10,r0 */
L8c619eb2: r[3]=rd(m,bus,&io_status,0x8c619efcu,4);if(io_status)return io_status;goto L8c619eb4; /* mov.l 0x8c619efc,r3 */
L8c619eb4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619eb6; /* fmov.s fr3,@-r8 */
L8c619eb6: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619eb8; /* fmov.s @(r0,r7),fr3 */
L8c619eb8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619eba; /* fmov.s fr3,@-r8 */
L8c619eba: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c619ebc; /* mov.l r3,@-r8 */
L8c619ebc: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619ebe; /* pref @r8 */
L8c619ebe: r[2]=rd(m,bus,&io_status,0x8c619f00u,4);if(io_status)return io_status;goto L8c619ec0; /* mov.l 0x8c619f00,r2 */
L8c619ec0: r[8]+=32u;goto L8c619ec2; /* add #0x20,r8 */
L8c619ec2: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c619ec4; /* mov.l @r2,r3 */
L8c619ec4: r[3]+=3u;goto L8c619ec6; /* add #0x3,r3 */
L8c619ec6: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c619ec8; /* mov.l r3,@r2 */
L8c619ec8: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c619eca; /* mov.w @(0x2,r4),r0 */
L8c619eca: r[0]=r[0]&65535u;goto L8c619ecc; /* extu.w r0,r0 */
L8c619ecc: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c619ece; /* cmp/ge r0,r14 */
L8c619ece: if((*sr&1u))goto L8c619ed4;goto L8c619ed0; /* bt 0x8c619ed4 */
L8c619ed0: goto L8c619ca8; /* bra 0x8c619ca8 */
L8c619ed4: goto L8c61a446; /* bra 0x8c61a446 */
L8c619ed8: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c619eda; /* mov.w @(0x2,r4),r0 */
L8c619eda: r[2]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c619edc; /* mov.l @r11,r2 */
L8c619edc: r[0]=r[0]&65535u;goto L8c619ede; /* extu.w r0,r0 */
L8c619ede: r[0]<<=2;goto L8c619ee0; /* shll2 r0 */
L8c619ee0: r[2]+=r[0];goto L8c619ee2; /* add r0,r2 */
L8c619ee2: wr(m,bus,&io_status,r[11],r[2],4);if(io_status)return io_status;goto L8c619ee4; /* mov.l r2,@r11 */
L8c619ee4: r[11]=r[12];goto L8c61a1c4; /* bra 0x8c61a1c4 */
L8c619f04: r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c619f06; /* mov.w @r13+,r6 */
L8c619f06: r[0]=16u;goto L8c619f08; /* mov #0x10,r0 */
L8c619f08: r[3]=rd(m,bus,&io_status,0x8c61a23cu,4);if(io_status)return io_status;goto L8c619f0a; /* mov.l 0x8c61a23c,r3 */
L8c619f0a: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c619f0c; /* mov.w @r13+,r5 */
L8c619f0c: r[6]=r[6]&65535u;goto L8c619f0e; /* extu.w r6,r6 */
L8c619f0e: r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c619f10; /* mov.l @r3,r12 */
L8c619f10: r[6]<<=2;goto L8c619f12; /* shll2 r6 */
L8c619f12: r[5]=r[5]&65535u;goto L8c619f14; /* extu.w r5,r5 */
L8c619f14: r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c619f16; /* mov.w @r13+,r7 */
L8c619f16: r[5]<<=2;goto L8c619f18; /* shll2 r5 */
L8c619f18: r[5]<<=2;goto L8c619f1a; /* shll2 r5 */
L8c619f1a: r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c619f1c; /* mov.w @r13+,r14 */
L8c619f1c: r[6]<<=2;goto L8c619f1e; /* shll2 r6 */
L8c619f1e: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c619f20; /* shll r6 */
L8c619f20: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c619f22; /* shll r5 */
L8c619f22: r[5]+=r[12];goto L8c619f24; /* add r12,r5 */
L8c619f24: r[6]+=r[12];goto L8c619f26; /* add r12,r6 */
L8c619f26: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c619f28; /* fmov.s @(r0,r5),fr4 */
L8c619f28: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619f2a; /* fmov.s @(r0,r6),fr3 */
L8c619f2a: r[7]=r[7]&65535u;goto L8c619f2c; /* extu.w r7,r7 */
L8c619f2c: fcopy(f,2u,4u);goto L8c619f2e; /* fmov fr4,fr2 */
L8c619f2e: r[7]<<=2;goto L8c619f30; /* shll2 r7 */
L8c619f30: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c619f32; /* fsub fr3,fr2 */
L8c619f32: r[14]=r[14]&65535u;goto L8c619f34; /* extu.w r14,r14 */
L8c619f34: r[7]<<=2;goto L8c619f36; /* shll2 r7 */
L8c619f36: r[14]<<=2;goto L8c619f38; /* shll2 r14 */
L8c619f38: r[0]=20u;goto L8c619f3a; /* mov #0x14,r0 */
L8c619f3a: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c619f3c; /* fmov.s @(r0,r5),fr5 */
L8c619f3c: r[14]<<=2;goto L8c619f3e; /* shll2 r14 */
L8c619f3e: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c619f40; /* shll r7 */
L8c619f40: r[7]+=r[12];goto L8c619f42; /* add r12,r7 */
L8c619f42: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c619f44; /* shll r14 */
L8c619f44: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619f46; /* fmov.s @(r0,r7),fr3 */
L8c619f46: fcopy(f,1u,5u);goto L8c619f48; /* fmov fr5,fr1 */
L8c619f48: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619f4a; /* fsub fr5,fr3 */
L8c619f4a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619f4c; /* fmul fr3,fr2 */
L8c619f4c: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619f4e; /* fmov.s @(r0,r6),fr3 */
L8c619f4e: r[0]=16u;goto L8c619f50; /* mov #0x10,r0 */
L8c619f50: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c619f52; /* fsub fr3,fr1 */
L8c619f52: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619f54; /* fmov.s @(r0,r7),fr3 */
L8c619f54: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619f56; /* fsub fr4,fr3 */
L8c619f56: fcopy(f,4u,2u);goto L8c619f58; /* fmov fr2,fr4 */
L8c619f58: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619f5a; /* fmul fr3,fr1 */
L8c619f5a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c619f5c; /* fldi0 fr3 */
L8c619f5c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c619f5e; /* fsub fr1,fr4 */
L8c619f5e: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c619f60; /* fcmp/gt fr3,fr4 */
L8c619f60: take=!(*sr&1u);r[14]+=r[12];if(take)goto L8c619f68;goto L8c619f64; /* bf/s 0x8c619f68 */
L8c619f64: goto L8c61a1c2; /* bra 0x8c61a1c2 */
L8c619f68: r[3]=rd(m,bus,&io_status,0x8c61a240u,4);if(io_status)return io_status;goto L8c619f6a; /* mov.l 0x8c61a240,r3 */
L8c619f6a: r[0]=4u;goto L8c619f6c; /* mov #0x4,r0 */
L8c619f6c: fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c619f6e; /* fmov.s @r6,fr3 */
L8c619f6e: r[8]+=28u;goto L8c619f70; /* add #0x1c,r8 */
L8c619f70: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c619f72; /* fmov.s @r3,fr2 */
L8c619f72: r[3]+=8u;goto L8c619f74; /* add #0x8,r3 */
L8c619f74: r[2]=rd(m,bus,&io_status,0x8c61a244u,4);if(io_status)return io_status;goto L8c619f76; /* mov.l 0x8c61a244,r2 */
L8c619f76: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619f78; /* fmul fr3,fr2 */
L8c619f78: r[1]=rd(m,bus,&io_status,0x8c61a248u,4);if(io_status)return io_status;goto L8c619f7a; /* mov.l 0x8c61a248,r1 */
L8c619f7a: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c619f7c; /* fmov.s @r2,fr3 */
L8c619f7c: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c619f7e; /* fmov.s @r1,fr1 */
L8c619f7e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619f80; /* fsub fr2,fr3 */
L8c619f80: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c619f82; /* fmov.s @(r0,r6),fr2 */
L8c619f82: r[0]=8u;goto L8c619f84; /* mov #0x8,r0 */
L8c619f84: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619f86; /* fmul fr2,fr1 */
L8c619f86: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c619f88; /* fmov.s @(r0,r6),fr2 */
L8c619f88: r[0]=4u;goto L8c619f8a; /* mov #0x4,r0 */
L8c619f8a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619f8c; /* fsub fr1,fr3 */
L8c619f8c: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619f8e; /* fmov.s @r3,fr1 */
L8c619f8e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619f90; /* fmul fr2,fr1 */
L8c619f90: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619f92; /* fsub fr1,fr3 */
L8c619f92: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619f94; /* fmov.s fr3,@-r8 */
L8c619f94: r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619f96; /* mov.l @r9,r2 */
L8c619f96: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619f98; /* fmov.s @(r0,r6),fr0 */
L8c619f98: r[0]=20u;goto L8c619f9a; /* mov #0x14,r0 */
L8c619f9a: r[1]=r[2];goto L8c619f9c; /* mov r2,r1 */
L8c619f9c: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619f9e; /* fmov.s @(r0,r1),fr3 */
L8c619f9e: r[0]=4u;goto L8c619fa0; /* mov #0x4,r0 */
L8c619fa0: r[3]=r[2];goto L8c619fa2; /* mov r2,r3 */
L8c619fa2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619fa4; /* fmul fr0,fr3 */
L8c619fa4: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619fa6; /* fmov.s @(r0,r3),fr2 */
L8c619fa6: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c619fa8; /* fmov.s @r6,fr0 */
L8c619fa8: r[0]=8u;goto L8c619faa; /* mov #0x8,r0 */
L8c619faa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619fac; /* fmac fr0,fr2,fr3 */
L8c619fac: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619fae; /* fmov.s @(r0,r6),fr0 */
L8c619fae: r[0]=36u;goto L8c619fb0; /* mov #0x24,r0 */
L8c619fb0: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c619fb2; /* fmov.s @(r0,r2),fr2 */
L8c619fb2: r[0]=4u;goto L8c619fb4; /* mov #0x4,r0 */
L8c619fb4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619fb6; /* fmac fr0,fr2,fr3 */
L8c619fb6: fcopy(f,0u,6u);goto L8c619fb8; /* fmov fr6,fr0 */
L8c619fb8: fcopy(f,2u,6u);goto L8c619fba; /* fmov fr6,fr2 */
L8c619fba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619fbc; /* fmac fr0,fr3,fr2 */
L8c619fbc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619fbe; /* fmov.s fr2,@-r8 */
L8c619fbe: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619fc0; /* mov.l @r9,r3 */
L8c619fc0: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619fc2; /* fmov.s @(r0,r6),fr0 */
L8c619fc2: r[0]=16u;goto L8c619fc4; /* mov #0x10,r0 */
L8c619fc4: r[1]=r[3];goto L8c619fc6; /* mov r3,r1 */
L8c619fc6: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619fc8; /* fmov.s @(r0,r1),fr3 */
L8c619fc8: r[2]=r[3];goto L8c619fca; /* mov r3,r2 */
L8c619fca: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c619fcc; /* fmov.s @r2,fr2 */
L8c619fcc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619fce; /* fmul fr0,fr3 */
L8c619fce: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c619fd0; /* fmov.s @r6,fr0 */
L8c619fd0: r[0]=8u;goto L8c619fd2; /* mov #0x8,r0 */
L8c619fd2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619fd4; /* fmac fr0,fr2,fr3 */
L8c619fd4: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619fd6; /* fmov.s @(r0,r6),fr0 */
L8c619fd6: r[0]=32u;goto L8c619fd8; /* mov #0x20,r0 */
L8c619fd8: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619fda; /* fmov.s @(r0,r3),fr2 */
L8c619fda: r[0]=24u;goto L8c619fdc; /* mov #0x18,r0 */
L8c619fdc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619fde; /* fmac fr0,fr2,fr3 */
L8c619fde: fcopy(f,0u,6u);goto L8c619fe0; /* fmov fr6,fr0 */
L8c619fe0: fcopy(f,2u,6u);goto L8c619fe2; /* fmov fr6,fr2 */
L8c619fe2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619fe4; /* fmac fr0,fr3,fr2 */
L8c619fe4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619fe6; /* fmov.s fr2,@-r8 */
L8c619fe6: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619fe8; /* fmov.s @(r0,r6),fr3 */
L8c619fe8: r[0]=20u;goto L8c619fea; /* mov #0x14,r0 */
L8c619fea: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619fec; /* fmov.s fr3,@-r8 */
L8c619fec: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619fee; /* fmov.s @(r0,r6),fr3 */
L8c619fee: r[0]=16u;goto L8c619ff0; /* mov #0x10,r0 */
L8c619ff0: r[3]=rd(m,bus,&io_status,0x8c61a24cu,4);if(io_status)return io_status;goto L8c619ff2; /* mov.l 0x8c61a24c,r3 */
L8c619ff2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619ff4; /* fmov.s fr3,@-r8 */
L8c619ff4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619ff6; /* fmov.s @(r0,r6),fr3 */
L8c619ff6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619ff8; /* fmov.s fr3,@-r8 */
L8c619ff8: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c619ffa; /* mov.l r3,@-r8 */
L8c619ffa: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619ffc; /* pref @r8 */
L8c619ffc: r[3]=rd(m,bus,&io_status,0x8c61a240u,4);if(io_status)return io_status;goto L8c619ffe; /* mov.l 0x8c61a240,r3 */
L8c619ffe: r[0]=4u;goto L8c61a000; /* mov #0x4,r0 */
L8c61a000: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61a002; /* fmov.s @r5,fr3 */
L8c61a002: r[8]+=60u;goto L8c61a004; /* add #0x3c,r8 */
L8c61a004: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a006; /* fmov.s @r3,fr2 */
L8c61a006: r[3]+=8u;goto L8c61a008; /* add #0x8,r3 */
L8c61a008: r[2]=rd(m,bus,&io_status,0x8c61a244u,4);if(io_status)return io_status;goto L8c61a00a; /* mov.l 0x8c61a244,r2 */
L8c61a00a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a00c; /* fmul fr3,fr2 */
L8c61a00c: r[1]=rd(m,bus,&io_status,0x8c61a248u,4);if(io_status)return io_status;goto L8c61a00e; /* mov.l 0x8c61a248,r1 */
L8c61a00e: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61a010; /* fmov.s @r2,fr3 */
L8c61a010: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61a012; /* fmov.s @r1,fr1 */
L8c61a012: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a014; /* fsub fr2,fr3 */
L8c61a014: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a016; /* fmov.s @(r0,r5),fr2 */
L8c61a016: r[0]=8u;goto L8c61a018; /* mov #0x8,r0 */
L8c61a018: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a01a; /* fmul fr2,fr1 */
L8c61a01a: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a01c; /* fmov.s @(r0,r5),fr2 */
L8c61a01c: r[0]=4u;goto L8c61a01e; /* mov #0x4,r0 */
L8c61a01e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a020; /* fsub fr1,fr3 */
L8c61a020: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a022; /* fmov.s @r3,fr1 */
L8c61a022: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a024; /* fmul fr2,fr1 */
L8c61a024: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a026; /* fsub fr1,fr3 */
L8c61a026: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a028; /* fmov.s fr3,@-r8 */
L8c61a028: r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a02a; /* mov.l @r9,r2 */
L8c61a02a: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a02c; /* fmov.s @(r0,r5),fr0 */
L8c61a02c: r[0]=20u;goto L8c61a02e; /* mov #0x14,r0 */
L8c61a02e: r[1]=r[2];goto L8c61a030; /* mov r2,r1 */
L8c61a030: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a032; /* fmov.s @(r0,r1),fr3 */
L8c61a032: r[0]=4u;goto L8c61a034; /* mov #0x4,r0 */
L8c61a034: r[3]=r[2];goto L8c61a036; /* mov r2,r3 */
L8c61a036: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a038; /* fmul fr0,fr3 */
L8c61a038: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a03a; /* fmov.s @(r0,r3),fr2 */
L8c61a03a: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a03c; /* fmov.s @r5,fr0 */
L8c61a03c: r[0]=8u;goto L8c61a03e; /* mov #0x8,r0 */
L8c61a03e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a040; /* fmac fr0,fr2,fr3 */
L8c61a040: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a042; /* fmov.s @(r0,r5),fr0 */
L8c61a042: r[0]=36u;goto L8c61a044; /* mov #0x24,r0 */
L8c61a044: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a046; /* fmov.s @(r0,r2),fr2 */
L8c61a046: r[0]=4u;goto L8c61a048; /* mov #0x4,r0 */
L8c61a048: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a04a; /* fmac fr0,fr2,fr3 */
L8c61a04a: fcopy(f,0u,6u);goto L8c61a04c; /* fmov fr6,fr0 */
L8c61a04c: fcopy(f,2u,6u);goto L8c61a04e; /* fmov fr6,fr2 */
L8c61a04e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a050; /* fmac fr0,fr3,fr2 */
L8c61a050: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a052; /* fmov.s fr2,@-r8 */
L8c61a052: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a054; /* mov.l @r9,r3 */
L8c61a054: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a056; /* fmov.s @(r0,r5),fr0 */
L8c61a056: r[0]=16u;goto L8c61a058; /* mov #0x10,r0 */
L8c61a058: r[1]=r[3];goto L8c61a05a; /* mov r3,r1 */
L8c61a05a: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a05c; /* fmov.s @(r0,r1),fr3 */
L8c61a05c: r[2]=r[3];goto L8c61a05e; /* mov r3,r2 */
L8c61a05e: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61a060; /* fmov.s @r2,fr2 */
L8c61a060: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a062; /* fmul fr0,fr3 */
L8c61a062: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a064; /* fmov.s @r5,fr0 */
L8c61a064: r[0]=8u;goto L8c61a066; /* mov #0x8,r0 */
L8c61a066: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a068; /* fmac fr0,fr2,fr3 */
L8c61a068: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a06a; /* fmov.s @(r0,r5),fr0 */
L8c61a06a: r[0]=32u;goto L8c61a06c; /* mov #0x20,r0 */
L8c61a06c: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a06e; /* fmov.s @(r0,r3),fr2 */
L8c61a06e: r[0]=24u;goto L8c61a070; /* mov #0x18,r0 */
L8c61a070: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a072; /* fmac fr0,fr2,fr3 */
L8c61a072: fcopy(f,0u,6u);goto L8c61a074; /* fmov fr6,fr0 */
L8c61a074: fcopy(f,2u,6u);goto L8c61a076; /* fmov fr6,fr2 */
L8c61a076: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a078; /* fmac fr0,fr3,fr2 */
L8c61a078: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a07a; /* fmov.s fr2,@-r8 */
L8c61a07a: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a07c; /* fmov.s @(r0,r5),fr3 */
L8c61a07c: r[0]=20u;goto L8c61a07e; /* mov #0x14,r0 */
L8c61a07e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a080; /* fmov.s fr3,@-r8 */
L8c61a080: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a082; /* fmov.s @(r0,r5),fr3 */
L8c61a082: r[0]=16u;goto L8c61a084; /* mov #0x10,r0 */
L8c61a084: r[3]=rd(m,bus,&io_status,0x8c61a24cu,4);if(io_status)return io_status;goto L8c61a086; /* mov.l 0x8c61a24c,r3 */
L8c61a086: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a088; /* fmov.s fr3,@-r8 */
L8c61a088: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a08a; /* fmov.s @(r0,r5),fr3 */
L8c61a08a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a08c; /* fmov.s fr3,@-r8 */
L8c61a08c: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a08e; /* mov.l r3,@-r8 */
L8c61a08e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a090; /* pref @r8 */
L8c61a090: r[3]=rd(m,bus,&io_status,0x8c61a240u,4);if(io_status)return io_status;goto L8c61a092; /* mov.l 0x8c61a240,r3 */
L8c61a092: r[0]=4u;goto L8c61a094; /* mov #0x4,r0 */
L8c61a094: fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c61a096; /* fmov.s @r7,fr3 */
L8c61a096: r[8]+=60u;goto L8c61a098; /* add #0x3c,r8 */
L8c61a098: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a09a; /* fmov.s @r3,fr2 */
L8c61a09a: r[3]+=8u;goto L8c61a09c; /* add #0x8,r3 */
L8c61a09c: r[2]=rd(m,bus,&io_status,0x8c61a244u,4);if(io_status)return io_status;goto L8c61a09e; /* mov.l 0x8c61a244,r2 */
L8c61a09e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a0a0; /* fmul fr3,fr2 */
L8c61a0a0: r[1]=rd(m,bus,&io_status,0x8c61a248u,4);if(io_status)return io_status;goto L8c61a0a2; /* mov.l 0x8c61a248,r1 */
L8c61a0a2: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61a0a4; /* fmov.s @r2,fr3 */
L8c61a0a4: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61a0a6; /* fmov.s @r1,fr1 */
L8c61a0a6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a0a8; /* fsub fr2,fr3 */
L8c61a0a8: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a0aa; /* fmov.s @(r0,r7),fr2 */
L8c61a0aa: r[0]=8u;goto L8c61a0ac; /* mov #0x8,r0 */
L8c61a0ac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a0ae; /* fmul fr2,fr1 */
L8c61a0ae: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a0b0; /* fmov.s @(r0,r7),fr2 */
L8c61a0b0: r[0]=4u;goto L8c61a0b2; /* mov #0x4,r0 */
L8c61a0b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a0b4; /* fsub fr1,fr3 */
L8c61a0b4: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a0b6; /* fmov.s @r3,fr1 */
L8c61a0b6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a0b8; /* fmul fr2,fr1 */
L8c61a0b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a0ba; /* fsub fr1,fr3 */
L8c61a0ba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a0bc; /* fmov.s fr3,@-r8 */
L8c61a0bc: r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a0be; /* mov.l @r9,r2 */
L8c61a0be: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a0c0; /* fmov.s @(r0,r7),fr0 */
L8c61a0c0: r[0]=20u;goto L8c61a0c2; /* mov #0x14,r0 */
L8c61a0c2: r[1]=r[2];goto L8c61a0c4; /* mov r2,r1 */
L8c61a0c4: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a0c6; /* fmov.s @(r0,r1),fr3 */
L8c61a0c6: r[0]=4u;goto L8c61a0c8; /* mov #0x4,r0 */
L8c61a0c8: r[3]=r[2];goto L8c61a0ca; /* mov r2,r3 */
L8c61a0ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a0cc; /* fmul fr0,fr3 */
L8c61a0cc: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a0ce; /* fmov.s @(r0,r3),fr2 */
L8c61a0ce: fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c61a0d0; /* fmov.s @r7,fr0 */
L8c61a0d0: r[0]=8u;goto L8c61a0d2; /* mov #0x8,r0 */
L8c61a0d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a0d4; /* fmac fr0,fr2,fr3 */
L8c61a0d4: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a0d6; /* fmov.s @(r0,r7),fr0 */
L8c61a0d6: r[0]=36u;goto L8c61a0d8; /* mov #0x24,r0 */
L8c61a0d8: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a0da; /* fmov.s @(r0,r2),fr2 */
L8c61a0da: r[0]=4u;goto L8c61a0dc; /* mov #0x4,r0 */
L8c61a0dc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a0de; /* fmac fr0,fr2,fr3 */
L8c61a0de: fcopy(f,0u,6u);goto L8c61a0e0; /* fmov fr6,fr0 */
L8c61a0e0: fcopy(f,2u,6u);goto L8c61a0e2; /* fmov fr6,fr2 */
L8c61a0e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a0e4; /* fmac fr0,fr3,fr2 */
L8c61a0e4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a0e6; /* fmov.s fr2,@-r8 */
L8c61a0e6: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a0e8; /* mov.l @r9,r3 */
L8c61a0e8: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a0ea; /* fmov.s @(r0,r7),fr0 */
L8c61a0ea: r[0]=16u;goto L8c61a0ec; /* mov #0x10,r0 */
L8c61a0ec: r[1]=r[3];goto L8c61a0ee; /* mov r3,r1 */
L8c61a0ee: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a0f0; /* fmov.s @(r0,r1),fr3 */
L8c61a0f0: r[2]=r[3];goto L8c61a0f2; /* mov r3,r2 */
L8c61a0f2: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61a0f4; /* fmov.s @r2,fr2 */
L8c61a0f4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a0f6; /* fmul fr0,fr3 */
L8c61a0f6: fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c61a0f8; /* fmov.s @r7,fr0 */
L8c61a0f8: r[0]=8u;goto L8c61a0fa; /* mov #0x8,r0 */
L8c61a0fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a0fc; /* fmac fr0,fr2,fr3 */
L8c61a0fc: fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a0fe; /* fmov.s @(r0,r7),fr0 */
L8c61a0fe: r[0]=32u;goto L8c61a100; /* mov #0x20,r0 */
L8c61a100: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a102; /* fmov.s @(r0,r3),fr2 */
L8c61a102: r[0]=24u;goto L8c61a104; /* mov #0x18,r0 */
L8c61a104: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a106; /* fmac fr0,fr2,fr3 */
L8c61a106: fcopy(f,0u,6u);goto L8c61a108; /* fmov fr6,fr0 */
L8c61a108: fcopy(f,2u,6u);goto L8c61a10a; /* fmov fr6,fr2 */
L8c61a10a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a10c; /* fmac fr0,fr3,fr2 */
L8c61a10c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a10e; /* fmov.s fr2,@-r8 */
L8c61a10e: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a110; /* fmov.s @(r0,r7),fr3 */
L8c61a110: r[0]=20u;goto L8c61a112; /* mov #0x14,r0 */
L8c61a112: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a114; /* fmov.s fr3,@-r8 */
L8c61a114: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a116; /* fmov.s @(r0,r7),fr3 */
L8c61a116: r[0]=16u;goto L8c61a118; /* mov #0x10,r0 */
L8c61a118: r[3]=rd(m,bus,&io_status,0x8c61a24cu,4);if(io_status)return io_status;goto L8c61a11a; /* mov.l 0x8c61a24c,r3 */
L8c61a11a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a11c; /* fmov.s fr3,@-r8 */
L8c61a11c: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a11e; /* fmov.s @(r0,r7),fr3 */
L8c61a11e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a120; /* fmov.s fr3,@-r8 */
L8c61a120: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a122; /* mov.l r3,@-r8 */
L8c61a122: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a124; /* pref @r8 */
L8c61a124: r[3]=rd(m,bus,&io_status,0x8c61a240u,4);if(io_status)return io_status;goto L8c61a126; /* mov.l 0x8c61a240,r3 */
L8c61a126: r[0]=4u;goto L8c61a128; /* mov #0x4,r0 */
L8c61a128: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61a12a; /* fmov.s @r14,fr3 */
L8c61a12a: r[8]+=60u;goto L8c61a12c; /* add #0x3c,r8 */
L8c61a12c: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a12e; /* fmov.s @r3,fr2 */
L8c61a12e: r[3]+=8u;goto L8c61a130; /* add #0x8,r3 */
L8c61a130: r[2]=rd(m,bus,&io_status,0x8c61a244u,4);if(io_status)return io_status;goto L8c61a132; /* mov.l 0x8c61a244,r2 */
L8c61a132: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a134; /* fmul fr3,fr2 */
L8c61a134: r[1]=rd(m,bus,&io_status,0x8c61a248u,4);if(io_status)return io_status;goto L8c61a136; /* mov.l 0x8c61a248,r1 */
L8c61a136: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61a138; /* fmov.s @r2,fr3 */
L8c61a138: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61a13a; /* fmov.s @r1,fr1 */
L8c61a13a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a13c; /* fsub fr2,fr3 */
L8c61a13c: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a13e; /* fmov.s @(r0,r14),fr2 */
L8c61a13e: r[0]=8u;goto L8c61a140; /* mov #0x8,r0 */
L8c61a140: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a142; /* fmul fr2,fr1 */
L8c61a142: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a144; /* fmov.s @(r0,r14),fr2 */
L8c61a144: r[0]=4u;goto L8c61a146; /* mov #0x4,r0 */
L8c61a146: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a148; /* fsub fr1,fr3 */
L8c61a148: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a14a; /* fmov.s @r3,fr1 */
L8c61a14a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a14c; /* fmul fr2,fr1 */
L8c61a14c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a14e; /* fsub fr1,fr3 */
L8c61a14e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a150; /* fmov.s fr3,@-r8 */
L8c61a150: r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a152; /* mov.l @r9,r2 */
L8c61a152: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a154; /* fmov.s @(r0,r14),fr0 */
L8c61a154: r[0]=20u;goto L8c61a156; /* mov #0x14,r0 */
L8c61a156: r[1]=r[2];goto L8c61a158; /* mov r2,r1 */
L8c61a158: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a15a; /* fmov.s @(r0,r1),fr3 */
L8c61a15a: r[0]=4u;goto L8c61a15c; /* mov #0x4,r0 */
L8c61a15c: r[3]=r[2];goto L8c61a15e; /* mov r2,r3 */
L8c61a15e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a160; /* fmul fr0,fr3 */
L8c61a160: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a162; /* fmov.s @(r0,r3),fr2 */
L8c61a162: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61a164; /* fmov.s @r14,fr0 */
L8c61a164: r[0]=8u;goto L8c61a166; /* mov #0x8,r0 */
L8c61a166: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a168; /* fmac fr0,fr2,fr3 */
L8c61a168: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a16a; /* fmov.s @(r0,r14),fr0 */
L8c61a16a: r[0]=36u;goto L8c61a16c; /* mov #0x24,r0 */
L8c61a16c: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a16e; /* fmov.s @(r0,r2),fr2 */
L8c61a16e: r[0]=4u;goto L8c61a170; /* mov #0x4,r0 */
L8c61a170: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a172; /* fmac fr0,fr2,fr3 */
L8c61a172: fcopy(f,0u,6u);goto L8c61a174; /* fmov fr6,fr0 */
L8c61a174: fcopy(f,2u,6u);goto L8c61a176; /* fmov fr6,fr2 */
L8c61a176: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a178; /* fmac fr0,fr3,fr2 */
L8c61a178: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a17a; /* fmov.s fr2,@-r8 */
L8c61a17a: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a17c; /* mov.l @r9,r3 */
L8c61a17c: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a17e; /* fmov.s @(r0,r14),fr0 */
L8c61a17e: r[0]=16u;goto L8c61a180; /* mov #0x10,r0 */
L8c61a180: r[1]=r[3];goto L8c61a182; /* mov r3,r1 */
L8c61a182: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a184; /* fmov.s @(r0,r1),fr3 */
L8c61a184: r[2]=r[3];goto L8c61a186; /* mov r3,r2 */
L8c61a186: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61a188; /* fmov.s @r2,fr2 */
L8c61a188: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a18a; /* fmul fr0,fr3 */
L8c61a18a: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61a18c; /* fmov.s @r14,fr0 */
L8c61a18c: r[0]=8u;goto L8c61a18e; /* mov #0x8,r0 */
L8c61a18e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a190; /* fmac fr0,fr2,fr3 */
L8c61a190: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a192; /* fmov.s @(r0,r14),fr0 */
L8c61a192: r[0]=32u;goto L8c61a194; /* mov #0x20,r0 */
L8c61a194: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a196; /* fmov.s @(r0,r3),fr2 */
L8c61a196: r[0]=24u;goto L8c61a198; /* mov #0x18,r0 */
L8c61a198: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a19a; /* fmac fr0,fr2,fr3 */
L8c61a19a: fcopy(f,0u,6u);goto L8c61a19c; /* fmov fr6,fr0 */
L8c61a19c: fcopy(f,2u,6u);goto L8c61a19e; /* fmov fr6,fr2 */
L8c61a19e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a1a0; /* fmac fr0,fr3,fr2 */
L8c61a1a0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a1a2; /* fmov.s fr2,@-r8 */
L8c61a1a2: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a1a4; /* fmov.s @(r0,r14),fr3 */
L8c61a1a4: r[0]=20u;goto L8c61a1a6; /* mov #0x14,r0 */
L8c61a1a6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a1a8; /* fmov.s fr3,@-r8 */
L8c61a1a8: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a1aa; /* fmov.s @(r0,r14),fr3 */
L8c61a1aa: r[0]=16u;goto L8c61a1ac; /* mov #0x10,r0 */
L8c61a1ac: r[3]=rd(m,bus,&io_status,0x8c61a250u,4);if(io_status)return io_status;goto L8c61a1ae; /* mov.l 0x8c61a250,r3 */
L8c61a1ae: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a1b0; /* fmov.s fr3,@-r8 */
L8c61a1b0: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a1b2; /* fmov.s @(r0,r14),fr3 */
L8c61a1b2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a1b4; /* fmov.s fr3,@-r8 */
L8c61a1b4: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a1b6; /* mov.l r3,@-r8 */
L8c61a1b6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a1b8; /* pref @r8 */
L8c61a1b8: r[2]=rd(m,bus,&io_status,0x8c61a254u,4);if(io_status)return io_status;goto L8c61a1ba; /* mov.l 0x8c61a254,r2 */
L8c61a1ba: r[8]+=32u;goto L8c61a1bc; /* add #0x20,r8 */
L8c61a1bc: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a1be; /* mov.l @r2,r3 */
L8c61a1be: r[3]+=4u;goto L8c61a1c0; /* add #0x4,r3 */
L8c61a1c0: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61a1c2; /* mov.l r3,@r2 */
L8c61a1c2: r[11]+=1u;goto L8c61a1c4; /* add #0x1,r11 */
L8c61a1c4: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61a1c6; /* mov.w @(0x2,r4),r0 */
L8c61a1c6: r[0]=r[0]&65535u;goto L8c61a1c8; /* extu.w r0,r0 */
L8c61a1c8: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61a1ca; /* cmp/ge r0,r11 */
L8c61a1ca: if((*sr&1u))goto L8c61a1d0;goto L8c61a1cc; /* bt 0x8c61a1d0 */
L8c61a1cc: goto L8c619f04; /* bra 0x8c619f04 */
L8c61a1d0: goto L8c61a446; /* bra 0x8c61a446 */
L8c61a1d4: r[10]=r[12];goto L8c61a3be; /* bra 0x8c61a3be */
L8c61a1d8: r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61a1da; /* mov.w @r13+,r14 */
L8c61a1da: r[3]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c61a1dc; /* mov.l @r11,r3 */
L8c61a1dc: r[14]=r[14]&65535u;goto L8c61a1de; /* extu.w r14,r14 */
L8c61a1de: r[2]=rd(m,bus,&io_status,0x8c61a23cu,4);if(io_status)return io_status;goto L8c61a1e0; /* mov.l 0x8c61a23c,r2 */
L8c61a1e0: r[3]+=r[14];goto L8c61a1e2; /* add r14,r3 */
L8c61a1e2: wr(m,bus,&io_status,r[11],r[3],4);if(io_status)return io_status;goto L8c61a1e4; /* mov.l r3,@r11 */
L8c61a1e4: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61a1e6; /* mov.w @(0x2,r13),r0 */
L8c61a1e6: r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61a1e8; /* mov.w @r13,r7 */
L8c61a1e8: r[5]=r[0]&65535u;goto L8c61a1ea; /* extu.w r0,r5 */
L8c61a1ea: r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c61a1ec; /* mov.w @(0x4,r13),r0 */
L8c61a1ec: r[5]<<=2;goto L8c61a1ee; /* shll2 r5 */
L8c61a1ee: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a1f0; /* mov.l @r2,r3 */
L8c61a1f0: r[7]=r[7]&65535u;goto L8c61a1f2; /* extu.w r7,r7 */
L8c61a1f2: r[7]<<=2;goto L8c61a1f4; /* shll2 r7 */
L8c61a1f4: r[5]<<=2;goto L8c61a1f6; /* shll2 r5 */
L8c61a1f6: r[6]=r[0]&65535u;goto L8c61a1f8; /* extu.w r0,r6 */
L8c61a1f8: r[7]<<=2;goto L8c61a1fa; /* shll2 r7 */
L8c61a1fa: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61a1fc; /* shll r7 */
L8c61a1fc: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61a1fe; /* shll r5 */
L8c61a1fe: r[0]=16u;goto L8c61a200; /* mov #0x10,r0 */
L8c61a200: r[5]+=r[3];goto L8c61a202; /* add r3,r5 */
L8c61a202: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a204; /* fmov.s @(r0,r5),fr5 */
L8c61a204: r[7]+=r[3];goto L8c61a206; /* add r3,r7 */
L8c61a206: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a208; /* fmov.s @(r0,r7),fr3 */
L8c61a208: r[6]<<=2;goto L8c61a20a; /* shll2 r6 */
L8c61a20a: fcopy(f,2u,5u);goto L8c61a20c; /* fmov fr5,fr2 */
L8c61a20c: r[6]<<=2;goto L8c61a20e; /* shll2 r6 */
L8c61a20e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61a210; /* fsub fr3,fr2 */
L8c61a210: r[0]=20u;goto L8c61a212; /* mov #0x14,r0 */
L8c61a212: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61a214; /* shll r6 */
L8c61a214: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a216; /* fmov.s @(r0,r5),fr4 */
L8c61a216: r[6]+=r[3];goto L8c61a218; /* add r3,r6 */
L8c61a218: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a21a; /* fmov.s @(r0,r6),fr3 */
L8c61a21a: fcopy(f,1u,4u);goto L8c61a21c; /* fmov fr4,fr1 */
L8c61a21c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a21e; /* fsub fr4,fr3 */
L8c61a21e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a220; /* fmul fr3,fr2 */
L8c61a220: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a222; /* fmov.s @(r0,r7),fr3 */
L8c61a222: r[0]=16u;goto L8c61a224; /* mov #0x10,r0 */
L8c61a224: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61a226; /* fsub fr3,fr1 */
L8c61a226: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a228; /* fmov.s @(r0,r6),fr3 */
L8c61a228: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a22a; /* fsub fr5,fr3 */
L8c61a22a: fcopy(f,4u,2u);goto L8c61a22c; /* fmov fr2,fr4 */
L8c61a22c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a22e; /* fmul fr3,fr1 */
L8c61a22e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61a230; /* fldi0 fr3 */
L8c61a230: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61a232; /* fsub fr1,fr4 */
L8c61a232: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61a234; /* fcmp/gt fr3,fr4 */
L8c61a234: if(!(*sr&1u))goto L8c61a258;goto L8c61a236; /* bf 0x8c61a258 */
L8c61a236: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61a238; /* shll r14 */
L8c61a238: r[13]+=r[14];goto L8c61a3bc; /* bra 0x8c61a3bc */
L8c61a258: r[3]=rd(m,bus,&io_status,0x8c61a464u,4);if(io_status)return io_status;goto L8c61a25a; /* mov.l 0x8c61a464,r3 */
L8c61a25a: r[7]=r[14];goto L8c61a25c; /* mov r14,r7 */
L8c61a25c: r[7]+=4294967295u;goto L8c61a25e; /* add #-0x1,r7 */
L8c61a25e: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a260; /* mov.l @r3,r2 */
L8c61a260: *sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c61a262; /* cmp/pl r7 */
L8c61a262: r[2]+=r[14];goto L8c61a264; /* add r14,r2 */
L8c61a264: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61a266; /* mov.l r2,@r3 */
L8c61a266: take=!(*sr&1u);r[6]=r[12];if(take)goto L8c61a316;goto L8c61a26a; /* bf/s 0x8c61a316 */
L8c61a26a: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61a26c; /* mov.w @r13+,r5 */
L8c61a26c: r[0]=4u;goto L8c61a26e; /* mov #0x4,r0 */
L8c61a26e: r[2]=rd(m,bus,&io_status,0x8c61a468u,4);if(io_status)return io_status;goto L8c61a270; /* mov.l 0x8c61a468,r2 */
L8c61a270: r[8]+=28u;goto L8c61a272; /* add #0x1c,r8 */
L8c61a272: r[5]=r[5]&65535u;goto L8c61a274; /* extu.w r5,r5 */
L8c61a274: r[1]=rd(m,bus,&io_status,0x8c61a470u,4);if(io_status)return io_status;goto L8c61a276; /* mov.l 0x8c61a470,r1 */
L8c61a276: r[5]<<=2;goto L8c61a278; /* shll2 r5 */
L8c61a278: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a27a; /* mov.l @r2,r3 */
L8c61a27a: r[5]<<=2;goto L8c61a27c; /* shll2 r5 */
L8c61a27c: r[2]=rd(m,bus,&io_status,0x8c61a474u,4);if(io_status)return io_status;goto L8c61a27e; /* mov.l 0x8c61a474,r2 */
L8c61a27e: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61a280; /* shll r5 */
L8c61a280: r[5]+=r[3];goto L8c61a282; /* add r3,r5 */
L8c61a282: r[3]=rd(m,bus,&io_status,0x8c61a46cu,4);if(io_status)return io_status;goto L8c61a284; /* mov.l 0x8c61a46c,r3 */
L8c61a284: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61a286; /* fmov.s @r5,fr3 */
L8c61a286: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a288; /* fmov.s @r3,fr2 */
L8c61a288: r[3]+=8u;goto L8c61a28a; /* add #0x8,r3 */
L8c61a28a: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61a28c; /* fmov.s @r2,fr1 */
L8c61a28c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a28e; /* fmul fr3,fr2 */
L8c61a28e: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61a290; /* fmov.s @r1,fr3 */
L8c61a290: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a292; /* fsub fr2,fr3 */
L8c61a292: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a294; /* fmov.s @(r0,r5),fr2 */
L8c61a294: r[0]=8u;goto L8c61a296; /* mov #0x8,r0 */
L8c61a296: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a298; /* fmul fr2,fr1 */
L8c61a298: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a29a; /* fmov.s @(r0,r5),fr2 */
L8c61a29a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a29c; /* fsub fr1,fr3 */
L8c61a29c: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a29e; /* fmov.s @r3,fr1 */
L8c61a29e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a2a0; /* fmul fr2,fr1 */
L8c61a2a0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a2a2; /* fsub fr1,fr3 */
L8c61a2a2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a2a4; /* fmov.s fr3,@-r8 */
L8c61a2a4: r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a2a6; /* mov.l @r9,r2 */
L8c61a2a6: r[1]=r[2];goto L8c61a2a8; /* mov r2,r1 */
L8c61a2a8: r[3]=r[2];goto L8c61a2aa; /* mov r2,r3 */
L8c61a2aa: r[0]=4u;goto L8c61a2ac; /* mov #0x4,r0 */
L8c61a2ac: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a2ae; /* fmov.s @(r0,r5),fr0 */
L8c61a2ae: r[0]=20u;goto L8c61a2b0; /* mov #0x14,r0 */
L8c61a2b0: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a2b2; /* fmov.s @(r0,r1),fr3 */
L8c61a2b2: r[0]=4u;goto L8c61a2b4; /* mov #0x4,r0 */
L8c61a2b4: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a2b6; /* fmov.s @(r0,r3),fr2 */
L8c61a2b6: r[0]=8u;goto L8c61a2b8; /* mov #0x8,r0 */
L8c61a2b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a2ba; /* fmul fr0,fr3 */
L8c61a2ba: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a2bc; /* fmov.s @r5,fr0 */
L8c61a2bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a2be; /* fmac fr0,fr2,fr3 */
L8c61a2be: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a2c0; /* fmov.s @(r0,r5),fr0 */
L8c61a2c0: r[0]=36u;goto L8c61a2c2; /* mov #0x24,r0 */
L8c61a2c2: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a2c4; /* fmov.s @(r0,r2),fr2 */
L8c61a2c4: r[0]=4u;goto L8c61a2c6; /* mov #0x4,r0 */
L8c61a2c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a2c8; /* fmac fr0,fr2,fr3 */
L8c61a2c8: fcopy(f,0u,6u);goto L8c61a2ca; /* fmov fr6,fr0 */
L8c61a2ca: fcopy(f,2u,6u);goto L8c61a2cc; /* fmov fr6,fr2 */
L8c61a2cc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a2ce; /* fmac fr0,fr3,fr2 */
L8c61a2ce: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a2d0; /* fmov.s fr2,@-r8 */
L8c61a2d0: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a2d2; /* mov.l @r9,r3 */
L8c61a2d2: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a2d4; /* fmov.s @(r0,r5),fr0 */
L8c61a2d4: r[0]=16u;goto L8c61a2d6; /* mov #0x10,r0 */
L8c61a2d6: r[1]=r[3];goto L8c61a2d8; /* mov r3,r1 */
L8c61a2d8: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a2da; /* fmov.s @(r0,r1),fr3 */
L8c61a2da: r[2]=r[3];goto L8c61a2dc; /* mov r3,r2 */
L8c61a2dc: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61a2de; /* fmov.s @r2,fr2 */
L8c61a2de: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a2e0; /* fmul fr0,fr3 */
L8c61a2e0: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a2e2; /* fmov.s @r5,fr0 */
L8c61a2e2: r[0]=8u;goto L8c61a2e4; /* mov #0x8,r0 */
L8c61a2e4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a2e6; /* fmac fr0,fr2,fr3 */
L8c61a2e6: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a2e8; /* fmov.s @(r0,r5),fr0 */
L8c61a2e8: r[0]=32u;goto L8c61a2ea; /* mov #0x20,r0 */
L8c61a2ea: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a2ec; /* fmov.s @(r0,r3),fr2 */
L8c61a2ec: r[0]=24u;goto L8c61a2ee; /* mov #0x18,r0 */
L8c61a2ee: r[3]=rd(m,bus,&io_status,0x8c61a478u,4);if(io_status)return io_status;goto L8c61a2f0; /* mov.l 0x8c61a478,r3 */
L8c61a2f0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a2f2; /* fmac fr0,fr2,fr3 */
L8c61a2f2: fcopy(f,0u,6u);goto L8c61a2f4; /* fmov fr6,fr0 */
L8c61a2f4: fcopy(f,2u,6u);goto L8c61a2f6; /* fmov fr6,fr2 */
L8c61a2f6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a2f8; /* fmac fr0,fr3,fr2 */
L8c61a2f8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a2fa; /* fmov.s fr2,@-r8 */
L8c61a2fa: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a2fc; /* fmov.s @(r0,r5),fr3 */
L8c61a2fc: r[0]=20u;goto L8c61a2fe; /* mov #0x14,r0 */
L8c61a2fe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a300; /* fmov.s fr3,@-r8 */
L8c61a300: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a302; /* fmov.s @(r0,r5),fr3 */
L8c61a302: r[0]=16u;goto L8c61a304; /* mov #0x10,r0 */
L8c61a304: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a306; /* fmov.s fr3,@-r8 */
L8c61a306: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a308; /* fmov.s @(r0,r5),fr3 */
L8c61a308: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a30a; /* fmov.s fr3,@-r8 */
L8c61a30a: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a30c; /* mov.l r3,@-r8 */
L8c61a30c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a30e; /* pref @r8 */
L8c61a30e: r[6]+=1u;goto L8c61a310; /* add #0x1,r6 */
L8c61a310: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c61a312; /* cmp/ge r7,r6 */
L8c61a312: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61a26a;goto L8c61a316; /* bf/s 0x8c61a26a */
L8c61a316: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61a318; /* mov.w @r13+,r5 */
L8c61a318: r[0]=4u;goto L8c61a31a; /* mov #0x4,r0 */
L8c61a31a: r[2]=rd(m,bus,&io_status,0x8c61a468u,4);if(io_status)return io_status;goto L8c61a31c; /* mov.l 0x8c61a468,r2 */
L8c61a31c: r[8]+=28u;goto L8c61a31e; /* add #0x1c,r8 */
L8c61a31e: r[5]=r[5]&65535u;goto L8c61a320; /* extu.w r5,r5 */
L8c61a320: r[1]=rd(m,bus,&io_status,0x8c61a470u,4);if(io_status)return io_status;goto L8c61a322; /* mov.l 0x8c61a470,r1 */
L8c61a322: r[5]<<=2;goto L8c61a324; /* shll2 r5 */
L8c61a324: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a326; /* mov.l @r2,r3 */
L8c61a326: r[5]<<=2;goto L8c61a328; /* shll2 r5 */
L8c61a328: r[2]=rd(m,bus,&io_status,0x8c61a474u,4);if(io_status)return io_status;goto L8c61a32a; /* mov.l 0x8c61a474,r2 */
L8c61a32a: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61a32c; /* shll r5 */
L8c61a32c: r[5]+=r[3];goto L8c61a32e; /* add r3,r5 */
L8c61a32e: r[3]=rd(m,bus,&io_status,0x8c61a46cu,4);if(io_status)return io_status;goto L8c61a330; /* mov.l 0x8c61a46c,r3 */
L8c61a330: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61a332; /* fmov.s @r5,fr3 */
L8c61a332: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a334; /* fmov.s @r3,fr2 */
L8c61a334: r[3]+=8u;goto L8c61a336; /* add #0x8,r3 */
L8c61a336: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61a338; /* fmov.s @r2,fr1 */
L8c61a338: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a33a; /* fmul fr3,fr2 */
L8c61a33a: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61a33c; /* fmov.s @r1,fr3 */
L8c61a33c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a33e; /* fsub fr2,fr3 */
L8c61a33e: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a340; /* fmov.s @(r0,r5),fr2 */
L8c61a340: r[0]=8u;goto L8c61a342; /* mov #0x8,r0 */
L8c61a342: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a344; /* fmul fr2,fr1 */
L8c61a344: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a346; /* fmov.s @(r0,r5),fr2 */
L8c61a346: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a348; /* fsub fr1,fr3 */
L8c61a348: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a34a; /* fmov.s @r3,fr1 */
L8c61a34a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a34c; /* fmul fr2,fr1 */
L8c61a34c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a34e; /* fsub fr1,fr3 */
L8c61a34e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a350; /* fmov.s fr3,@-r8 */
L8c61a350: r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a352; /* mov.l @r9,r2 */
L8c61a352: r[1]=r[2];goto L8c61a354; /* mov r2,r1 */
L8c61a354: r[3]=r[2];goto L8c61a356; /* mov r2,r3 */
L8c61a356: r[0]=4u;goto L8c61a358; /* mov #0x4,r0 */
L8c61a358: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a35a; /* fmov.s @(r0,r5),fr0 */
L8c61a35a: r[0]=20u;goto L8c61a35c; /* mov #0x14,r0 */
L8c61a35c: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a35e; /* fmov.s @(r0,r1),fr3 */
L8c61a35e: r[0]=4u;goto L8c61a360; /* mov #0x4,r0 */
L8c61a360: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a362; /* fmov.s @(r0,r3),fr2 */
L8c61a362: r[0]=8u;goto L8c61a364; /* mov #0x8,r0 */
L8c61a364: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a366; /* fmul fr0,fr3 */
L8c61a366: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a368; /* fmov.s @r5,fr0 */
L8c61a368: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a36a; /* fmac fr0,fr2,fr3 */
L8c61a36a: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a36c; /* fmov.s @(r0,r5),fr0 */
L8c61a36c: r[0]=36u;goto L8c61a36e; /* mov #0x24,r0 */
L8c61a36e: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a370; /* fmov.s @(r0,r2),fr2 */
L8c61a370: r[0]=4u;goto L8c61a372; /* mov #0x4,r0 */
L8c61a372: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a374; /* fmac fr0,fr2,fr3 */
L8c61a374: fcopy(f,0u,6u);goto L8c61a376; /* fmov fr6,fr0 */
L8c61a376: fcopy(f,2u,6u);goto L8c61a378; /* fmov fr6,fr2 */
L8c61a378: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a37a; /* fmac fr0,fr3,fr2 */
L8c61a37a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a37c; /* fmov.s fr2,@-r8 */
L8c61a37c: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a37e; /* mov.l @r9,r3 */
L8c61a37e: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a380; /* fmov.s @(r0,r5),fr0 */
L8c61a380: r[0]=16u;goto L8c61a382; /* mov #0x10,r0 */
L8c61a382: r[1]=r[3];goto L8c61a384; /* mov r3,r1 */
L8c61a384: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a386; /* fmov.s @(r0,r1),fr3 */
L8c61a386: r[2]=r[3];goto L8c61a388; /* mov r3,r2 */
L8c61a388: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61a38a; /* fmov.s @r2,fr2 */
L8c61a38a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a38c; /* fmul fr0,fr3 */
L8c61a38c: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a38e; /* fmov.s @r5,fr0 */
L8c61a38e: r[0]=8u;goto L8c61a390; /* mov #0x8,r0 */
L8c61a390: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a392; /* fmac fr0,fr2,fr3 */
L8c61a392: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a394; /* fmov.s @(r0,r5),fr0 */
L8c61a394: r[0]=32u;goto L8c61a396; /* mov #0x20,r0 */
L8c61a396: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a398; /* fmov.s @(r0,r3),fr2 */
L8c61a398: r[0]=24u;goto L8c61a39a; /* mov #0x18,r0 */
L8c61a39a: r[3]=rd(m,bus,&io_status,0x8c61a47cu,4);if(io_status)return io_status;goto L8c61a39c; /* mov.l 0x8c61a47c,r3 */
L8c61a39c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a39e; /* fmac fr0,fr2,fr3 */
L8c61a39e: fcopy(f,0u,6u);goto L8c61a3a0; /* fmov fr6,fr0 */
L8c61a3a0: fcopy(f,2u,6u);goto L8c61a3a2; /* fmov fr6,fr2 */
L8c61a3a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a3a4; /* fmac fr0,fr3,fr2 */
L8c61a3a4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a3a6; /* fmov.s fr2,@-r8 */
L8c61a3a6: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a3a8; /* fmov.s @(r0,r5),fr3 */
L8c61a3a8: r[0]=20u;goto L8c61a3aa; /* mov #0x14,r0 */
L8c61a3aa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a3ac; /* fmov.s fr3,@-r8 */
L8c61a3ac: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a3ae; /* fmov.s @(r0,r5),fr3 */
L8c61a3ae: r[0]=16u;goto L8c61a3b0; /* mov #0x10,r0 */
L8c61a3b0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a3b2; /* fmov.s fr3,@-r8 */
L8c61a3b2: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a3b4; /* fmov.s @(r0,r5),fr3 */
L8c61a3b4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a3b6; /* fmov.s fr3,@-r8 */
L8c61a3b6: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a3b8; /* mov.l r3,@-r8 */
L8c61a3b8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a3ba; /* pref @r8 */
L8c61a3ba: r[8]+=32u;goto L8c61a3bc; /* add #0x20,r8 */
L8c61a3bc: r[10]+=1u;goto L8c61a3be; /* add #0x1,r10 */
L8c61a3be: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61a3c0; /* mov.w @(0x2,r4),r0 */
L8c61a3c0: r[0]=r[0]&65535u;goto L8c61a3c2; /* extu.w r0,r0 */
L8c61a3c2: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c61a3c4; /* cmp/ge r0,r10 */
L8c61a3c4: if((*sr&1u))goto L8c61a3ca;goto L8c61a3c6; /* bt 0x8c61a3ca */
L8c61a3c6: goto L8c61a1d8; /* bra 0x8c61a1d8 */
L8c61a3ca: goto L8c61a446; /* bra 0x8c61a446 */
L8c61a3ce: r[1]=rd(m,bus,&io_status,0x8c61a480u,4);if(io_status)return io_status;goto L8c61a3d0; /* mov.l 0x8c61a480,r1 */
L8c61a3d0: wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c61a3d2; /* mov.l r4,@r1 */
L8c61a3d2: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61a3d4; /* frchg */
L8c61a3d4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61a3d6; /* fmov.s fr0,@-r15 */
L8c61a3d6: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61a3d8; /* fmov.s fr1,@-r15 */
L8c61a3d8: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61a3da; /* fmov.s fr2,@-r15 */
L8c61a3da: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61a3dc; /* fmov.s fr3,@-r15 */
L8c61a3dc: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61a3de; /* fmov.s fr4,@-r15 */
L8c61a3de: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61a3e0; /* fmov.s fr5,@-r15 */
L8c61a3e0: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61a3e2; /* fmov.s fr6,@-r15 */
L8c61a3e2: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61a3e4; /* fmov.s fr7,@-r15 */
L8c61a3e4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61a3e6; /* fmov.s fr8,@-r15 */
L8c61a3e6: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61a3e8; /* fmov.s fr9,@-r15 */
L8c61a3e8: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c61a3ea; /* fmov.s fr10,@-r15 */
L8c61a3ea: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c61a3ec; /* fmov.s fr11,@-r15 */
L8c61a3ec: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c61a3ee; /* fmov.s fr12,@-r15 */
L8c61a3ee: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61a3f0; /* fmov.s fr13,@-r15 */
L8c61a3f0: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61a3f2; /* fmov.s fr14,@-r15 */
L8c61a3f2: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61a3f4; /* fmov.s fr15,@-r15 */
L8c61a3f4: r[0]=0x8c61a458u;goto L8c61a3f6; /* mova 0x8c61a458,r0 */
L8c61a3f6: fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c61a3f8; /* fmov.s @r0,fr12 */
L8c61a3f8: fcopy(f,13u,12u);goto L8c61a3fa; /* fmov fr12,fr13 */
L8c61a3fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[12],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61a3fc; /* fmul fr12,fr0 */
L8c61a3fc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61a3fe; /* fmul fr12,fr4 */
L8c61a3fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[12],&f->fpscr,&fr[8]);if(status)return status;}goto L8c61a400; /* fmul fr12,fr8 */
L8c61a400: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[12],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a402; /* fmul fr12,fr1 */
L8c61a402: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[12],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61a404; /* fmul fr12,fr5 */
L8c61a404: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[12],&f->fpscr,&fr[9]);if(status)return status;}goto L8c61a406; /* fmul fr12,fr9 */
L8c61a406: r[0]=rd(m,bus,&io_status,0x8c61a45cu,4);if(io_status)return io_status;goto L8c61a408; /* mov.l 0x8c61a45c,r0 */
L8c61a408: fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a40a; /* fmov.s @r0+,fr2 */
L8c61a40a: fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a40c; /* fmov.s @r0+,fr6 */
L8c61a40c: fload(m,bus,&io_status,f,10u,r[0]);if(io_status)return io_status;goto L8c61a40e; /* fmov.s @r0,fr10 */
L8c61a40e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c61a410; /* fneg fr2 */
L8c61a410: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]^=0x80000000u;goto L8c61a412; /* fneg fr6 */
L8c61a412: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]^=0x80000000u;goto L8c61a414; /* fneg fr10 */
L8c61a414: r[0]=rd(m,bus,&io_status,0x8c61a460u,4);if(io_status)return io_status;goto L8c61a416; /* mov.l 0x8c61a460,r0 */
L8c61a416: fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c61a418; /* fmov.s @r0,fr14 */
L8c61a418: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61a41a; /* frchg */
L8c61a41a: r[2]=rd(m,bus,&io_status,0x8c61a484u,4);if(io_status)return io_status;goto L8c61a41c; /* mov.l 0x8c61a484,r2 */
L8c61a41c: r[3]=rd(m,bus,&io_status,0x8c61a480u,4);if(io_status)return io_status;goto L8c61a41e; /* mov.l 0x8c61a480,r3 */
L8c61a41e: target=r[2];*pr=0x8c61a422u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61a422; /* jsr @r2 */
L8c61a422: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61a424; /* frchg */
L8c61a424: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a426; /* fmov.s @r15+,fr15 */
L8c61a426: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a428; /* fmov.s @r15+,fr14 */
L8c61a428: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a42a; /* fmov.s @r15+,fr13 */
L8c61a42a: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a42c; /* fmov.s @r15+,fr12 */
L8c61a42c: fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a42e; /* fmov.s @r15+,fr11 */
L8c61a42e: fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a430; /* fmov.s @r15+,fr10 */
L8c61a430: fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a432; /* fmov.s @r15+,fr9 */
L8c61a432: fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a434; /* fmov.s @r15+,fr8 */
L8c61a434: fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a436; /* fmov.s @r15+,fr7 */
L8c61a436: fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a438; /* fmov.s @r15+,fr6 */
L8c61a438: fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a43a; /* fmov.s @r15+,fr5 */
L8c61a43a: fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a43c; /* fmov.s @r15+,fr4 */
L8c61a43c: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a43e; /* fmov.s @r15+,fr3 */
L8c61a43e: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a440; /* fmov.s @r15+,fr2 */
L8c61a440: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a442; /* fmov.s @r15+,fr1 */
L8c61a442: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a444; /* fmov.s @r15+,fr0 */
L8c61a444: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61a446; /* frchg */
L8c61a446: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61a448; /* lds.l @r15+,PR */
L8c61a448: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61a44a; /* mov.l @r15+,r9 */
L8c61a44a: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61a44c; /* mov.l @r15+,r10 */
L8c61a44c: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61a44e; /* mov.l @r15+,r11 */
L8c61a44e: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61a450; /* mov.l @r15+,r12 */
L8c61a450: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61a452; /* mov.l @r15+,r13 */
L8c61a452: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_environment_primitive_clipped_8c61a488(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61a488;
L8c61a488: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61a48a; /* mov.l r14,@-r15 */
L8c61a48a: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61a48c; /* mov.l r13,@-r15 */
L8c61a48c: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61a48e; /* mov.l r12,@-r15 */
L8c61a48e: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61a490; /* mov.l r11,@-r15 */
L8c61a490: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61a492; /* mov.l r10,@-r15 */
L8c61a492: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61a494; /* mov.l r9,@-r15 */
L8c61a494: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61a496; /* fmov.s fr15,@-r15 */
L8c61a496: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61a498; /* sts.l PR,@-r15 */
L8c61a498: r[15]+=4294967288u;goto L8c61a49a; /* add #-0x8,r15 */
L8c61a49a: r[0]=0x8c61a4f0u;goto L8c61a49c; /* mova 0x8c61a4f0,r0 */
L8c61a49c: r[3]=rd(m,bus,&io_status,0x8c61a4f4u,4);if(io_status)return io_status;goto L8c61a49e; /* mov.l 0x8c61a4f4,r3 */
L8c61a49e: fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c61a4a0; /* fmov.s @r0,fr15 */
L8c61a4a0: r[0]=r[4];goto L8c61a4a2; /* mov r4,r0 */
L8c61a4a2: wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61a4a4; /* mov.l r4,@r15 */
L8c61a4a4: r[11]=r[4];goto L8c61a4a6; /* mov r4,r11 */
L8c61a4a6: r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61a4a8; /* mov.w @r0,r0 */
L8c61a4a8: r[0]=r[0]&65535u;goto L8c61a4aa; /* extu.w r0,r0 */
L8c61a4aa: r[0]&=r[3];goto L8c61a4ac; /* and r3,r0 */
L8c61a4ac: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61a4ae; /* cmp/eq #0x0,r0 */
L8c61a4ae: take=(*sr&1u);r[11]=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;if(take)goto L8c61a4d4;goto L8c61a4b2; /* bt/s 0x8c61a4d4 */
L8c61a4b2: r[1]=sx(m,bus,&io_status,0x8c61a4ecu,2);if(io_status)return io_status;goto L8c61a4b4; /* mov.w 0x8c61a4ec,r1 */
L8c61a4b4: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61a4b6; /* cmp/eq r1,r0 */
L8c61a4b6: if(!(*sr&1u))goto L8c61a4bc;goto L8c61a4b8; /* bf 0x8c61a4bc */
L8c61a4b8: goto L8c61a752; /* bra 0x8c61a752 */
L8c61a4bc: r[1]=rd(m,bus,&io_status,0x8c61a4f8u,4);if(io_status)return io_status;goto L8c61a4be; /* mov.l 0x8c61a4f8,r1 */
L8c61a4be: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61a4c0; /* cmp/eq r1,r0 */
L8c61a4c0: if(!(*sr&1u))goto L8c61a4c6;goto L8c61a4c2; /* bf 0x8c61a4c6 */
L8c61a4c2: goto L8c61aace; /* bra 0x8c61aace */
L8c61a4c6: r[1]=rd(m,bus,&io_status,0x8c61a4f4u,4);if(io_status)return io_status;goto L8c61a4c8; /* mov.l 0x8c61a4f4,r1 */
L8c61a4c8: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61a4ca; /* cmp/eq r1,r0 */
L8c61a4ca: if(!(*sr&1u))goto L8c61a4d0;goto L8c61a4cc; /* bf 0x8c61a4d0 */
L8c61a4cc: goto L8c61ad6e; /* bra 0x8c61ad6e */
L8c61a4d0: goto L8c61ade8; /* bra 0x8c61ade8 */
L8c61a4d4: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61a4d6; /* mov.l @r15,r2 */
L8c61a4d6: r[1]=rd(m,bus,&io_status,0x8c61a4fcu,4);if(io_status)return io_status;goto L8c61a4d8; /* mov.l 0x8c61a4fc,r1 */
L8c61a4d8: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61a4da; /* mov.w @(0x2,r2),r0 */
L8c61a4da: r[0]=r[0]&65535u;goto L8c61a4dc; /* extu.w r0,r0 */
L8c61a4dc: r[2]=r[0];goto L8c61a4de; /* mov r0,r2 */
L8c61a4de: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61a4e0; /* shll r0 */
L8c61a4e0: r[0]+=r[2];goto L8c61a4e2; /* add r2,r0 */
L8c61a4e2: r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61a4e4; /* mov.l @r1,r2 */
L8c61a4e4: r[2]+=r[0];goto L8c61a4e6; /* add r0,r2 */
L8c61a4e6: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61a4e8; /* mov.l r2,@r1 */
L8c61a4e8: r[14]=0u;goto L8c61a740; /* bra 0x8c61a740 */
L8c61a500: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61a502; /* mov.w @r11+,r4 */
L8c61a502: r[3]=rd(m,bus,&io_status,0x8c61a76cu,4);if(io_status)return io_status;goto L8c61a504; /* mov.l 0x8c61a76c,r3 */
L8c61a504: r[5]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61a506; /* mov.w @r11+,r5 */
L8c61a506: r[4]=r[4]&65535u;goto L8c61a508; /* extu.w r4,r4 */
L8c61a508: r[4]<<=2;goto L8c61a50a; /* shll2 r4 */
L8c61a50a: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a50c; /* mov.l @r3,r13 */
L8c61a50c: r[6]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61a50e; /* mov.w @r11+,r6 */
L8c61a50e: r[5]=r[5]&65535u;goto L8c61a510; /* extu.w r5,r5 */
L8c61a510: r[5]<<=2;goto L8c61a512; /* shll2 r5 */
L8c61a512: r[6]=r[6]&65535u;goto L8c61a514; /* extu.w r6,r6 */
L8c61a514: r[5]<<=2;goto L8c61a516; /* shll2 r5 */
L8c61a516: r[6]<<=2;goto L8c61a518; /* shll2 r6 */
L8c61a518: r[4]<<=2;goto L8c61a51a; /* shll2 r4 */
L8c61a51a: r[6]<<=2;goto L8c61a51c; /* shll2 r6 */
L8c61a51c: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61a51e; /* shll r4 */
L8c61a51e: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61a520; /* shll r5 */
L8c61a520: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61a522; /* shll r6 */
L8c61a522: r[4]+=r[13];goto L8c61a524; /* add r13,r4 */
L8c61a524: r[5]+=r[13];goto L8c61a526; /* add r13,r5 */
L8c61a526: r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61a528; /* mov.l @(0xc,r4),r7 */
L8c61a528: r[2]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c61a52a; /* mov.l @(0xc,r5),r2 */
L8c61a52a: r[6]+=r[13];goto L8c61a52c; /* add r13,r6 */
L8c61a52c: r[1]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c61a52e; /* mov.l @(0xc,r6),r1 */
L8c61a52e: r[7]&=r[2];goto L8c61a530; /* and r2,r7 */
L8c61a530: r[2]=63u;goto L8c61a532; /* mov #0x3f,r2 */
L8c61a532: r[7]&=r[1];goto L8c61a534; /* and r1,r7 */
L8c61a534: *sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c61a536; /* tst r2,r7 */
L8c61a536: if((*sr&1u))goto L8c61a53c;goto L8c61a538; /* bt 0x8c61a53c */
L8c61a538: goto L8c61a73e; /* bra 0x8c61a73e */
L8c61a53c: r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61a53e; /* mov.l @(0xc,r4),r7 */
L8c61a53e: r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c61a540; /* mov.l @(0xc,r5),r3 */
L8c61a540: r[2]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c61a542; /* mov.l @(0xc,r6),r2 */
L8c61a542: r[7]|=r[3];goto L8c61a544; /* or r3,r7 */
L8c61a544: r[3]=1u;goto L8c61a546; /* mov #0x1,r3 */
L8c61a546: r[7]|=r[2];goto L8c61a548; /* or r2,r7 */
L8c61a548: *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c61a54a; /* tst r3,r7 */
L8c61a54a: if((*sr&1u))goto L8c61a550;goto L8c61a54c; /* bt 0x8c61a550 */
L8c61a54c: goto L8c61a734; /* bra 0x8c61a734 */
L8c61a550: r[0]=16u;goto L8c61a552; /* mov #0x10,r0 */
L8c61a552: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a554; /* fmov.s @(r0,r5),fr5 */
L8c61a554: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a556; /* fmov.s @(r0,r4),fr3 */
L8c61a556: r[0]=20u;goto L8c61a558; /* mov #0x14,r0 */
L8c61a558: fcopy(f,2u,5u);goto L8c61a55a; /* fmov fr5,fr2 */
L8c61a55a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61a55c; /* fsub fr3,fr2 */
L8c61a55c: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a55e; /* fmov.s @(r0,r6),fr3 */
L8c61a55e: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a560; /* fmov.s @(r0,r5),fr4 */
L8c61a560: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a562; /* fsub fr4,fr3 */
L8c61a562: fcopy(f,1u,4u);goto L8c61a564; /* fmov fr4,fr1 */
L8c61a564: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a566; /* fmul fr3,fr2 */
L8c61a566: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a568; /* fmov.s @(r0,r4),fr3 */
L8c61a568: r[0]=16u;goto L8c61a56a; /* mov #0x10,r0 */
L8c61a56a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61a56c; /* fsub fr3,fr1 */
L8c61a56c: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a56e; /* fmov.s @(r0,r6),fr3 */
L8c61a56e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a570; /* fsub fr5,fr3 */
L8c61a570: fcopy(f,4u,2u);goto L8c61a572; /* fmov fr2,fr4 */
L8c61a572: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a574; /* fmul fr3,fr1 */
L8c61a574: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61a576; /* fldi0 fr3 */
L8c61a576: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61a578; /* fsub fr1,fr4 */
L8c61a578: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61a57a; /* fcmp/gt fr3,fr4 */
L8c61a57a: if(!(*sr&1u))goto L8c61a580;goto L8c61a57c; /* bf 0x8c61a580 */
L8c61a57c: goto L8c61a73e; /* bra 0x8c61a73e */
L8c61a580: r[3]=rd(m,bus,&io_status,0x8c61a770u,4);if(io_status)return io_status;goto L8c61a582; /* mov.l 0x8c61a770,r3 */
L8c61a582: r[0]=4u;goto L8c61a584; /* mov #0x4,r0 */
L8c61a584: fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61a586; /* fmov.s @r4,fr3 */
L8c61a586: r[8]+=28u;goto L8c61a588; /* add #0x1c,r8 */
L8c61a588: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a58a; /* fmov.s @r3,fr2 */
L8c61a58a: r[3]+=8u;goto L8c61a58c; /* add #0x8,r3 */
L8c61a58c: r[2]=rd(m,bus,&io_status,0x8c61a774u,4);if(io_status)return io_status;goto L8c61a58e; /* mov.l 0x8c61a774,r2 */
L8c61a58e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a590; /* fmul fr3,fr2 */
L8c61a590: r[1]=rd(m,bus,&io_status,0x8c61a778u,4);if(io_status)return io_status;goto L8c61a592; /* mov.l 0x8c61a778,r1 */
L8c61a592: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61a594; /* fmov.s @r2,fr3 */
L8c61a594: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61a596; /* fmov.s @r1,fr1 */
L8c61a596: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a598; /* fsub fr2,fr3 */
L8c61a598: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a59a; /* fmov.s @(r0,r4),fr2 */
L8c61a59a: r[0]=8u;goto L8c61a59c; /* mov #0x8,r0 */
L8c61a59c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a59e; /* fmul fr2,fr1 */
L8c61a59e: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a5a0; /* fmov.s @(r0,r4),fr2 */
L8c61a5a0: r[0]=rd(m,bus,&io_status,0x8c61a77cu,4);if(io_status)return io_status;goto L8c61a5a2; /* mov.l 0x8c61a77c,r0 */
L8c61a5a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a5a4; /* fsub fr1,fr3 */
L8c61a5a4: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a5a6; /* fmov.s @r3,fr1 */
L8c61a5a6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a5a8; /* fmul fr2,fr1 */
L8c61a5a8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a5aa; /* fsub fr1,fr3 */
L8c61a5aa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a5ac; /* fmov.s fr3,@-r8 */
L8c61a5ac: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61a5ae; /* mov.l @r0,r2 */
L8c61a5ae: r[0]=4u;goto L8c61a5b0; /* mov #0x4,r0 */
L8c61a5b0: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a5b2; /* fmov.s @(r0,r4),fr0 */
L8c61a5b2: r[0]=20u;goto L8c61a5b4; /* mov #0x14,r0 */
L8c61a5b4: r[1]=r[2];goto L8c61a5b6; /* mov r2,r1 */
L8c61a5b6: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a5b8; /* fmov.s @(r0,r1),fr3 */
L8c61a5b8: r[0]=4u;goto L8c61a5ba; /* mov #0x4,r0 */
L8c61a5ba: r[3]=r[2];goto L8c61a5bc; /* mov r2,r3 */
L8c61a5bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a5be; /* fmul fr0,fr3 */
L8c61a5be: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61a5c0; /* fmov.s @r4,fr0 */
L8c61a5c0: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a5c2; /* fmov.s @(r0,r3),fr2 */
L8c61a5c2: r[0]=8u;goto L8c61a5c4; /* mov #0x8,r0 */
L8c61a5c4: r[3]=rd(m,bus,&io_status,0x8c61a77cu,4);if(io_status)return io_status;goto L8c61a5c6; /* mov.l 0x8c61a77c,r3 */
L8c61a5c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a5c8; /* fmac fr0,fr2,fr3 */
L8c61a5c8: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a5ca; /* fmov.s @(r0,r4),fr0 */
L8c61a5ca: r[0]=36u;goto L8c61a5cc; /* mov #0x24,r0 */
L8c61a5cc: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a5ce; /* fmov.s @(r0,r2),fr2 */
L8c61a5ce: r[0]=4u;goto L8c61a5d0; /* mov #0x4,r0 */
L8c61a5d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a5d2; /* fmac fr0,fr2,fr3 */
L8c61a5d2: fcopy(f,0u,15u);goto L8c61a5d4; /* fmov fr15,fr0 */
L8c61a5d4: fcopy(f,2u,15u);goto L8c61a5d6; /* fmov fr15,fr2 */
L8c61a5d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a5d8; /* fmac fr0,fr3,fr2 */
L8c61a5d8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a5da; /* fmov.s fr2,@-r8 */
L8c61a5da: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a5dc; /* fmov.s @(r0,r4),fr0 */
L8c61a5dc: r[0]=16u;goto L8c61a5de; /* mov #0x10,r0 */
L8c61a5de: r[9]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a5e0; /* mov.l @r3,r9 */
L8c61a5e0: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61a5e2; /* fmov.s @(r0,r9),fr3 */
L8c61a5e2: r[0]=8u;goto L8c61a5e4; /* mov #0x8,r0 */
L8c61a5e4: fload(m,bus,&io_status,f,2u,r[9]);if(io_status)return io_status;goto L8c61a5e6; /* fmov.s @r9,fr2 */
L8c61a5e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a5e8; /* fmul fr0,fr3 */
L8c61a5e8: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61a5ea; /* fmov.s @r4,fr0 */
L8c61a5ea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a5ec; /* fmac fr0,fr2,fr3 */
L8c61a5ec: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a5ee; /* fmov.s @(r0,r4),fr0 */
L8c61a5ee: r[0]=32u;goto L8c61a5f0; /* mov #0x20,r0 */
L8c61a5f0: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61a5f2; /* fmov.s @(r0,r9),fr2 */
L8c61a5f2: r[0]=24u;goto L8c61a5f4; /* mov #0x18,r0 */
L8c61a5f4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a5f6; /* fmac fr0,fr2,fr3 */
L8c61a5f6: fcopy(f,0u,15u);goto L8c61a5f8; /* fmov fr15,fr0 */
L8c61a5f8: fcopy(f,2u,15u);goto L8c61a5fa; /* fmov fr15,fr2 */
L8c61a5fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a5fc; /* fmac fr0,fr3,fr2 */
L8c61a5fc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a5fe; /* fmov.s fr2,@-r8 */
L8c61a5fe: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a600; /* fmov.s @(r0,r4),fr3 */
L8c61a600: r[0]=20u;goto L8c61a602; /* mov #0x14,r0 */
L8c61a602: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a604; /* fmov.s fr3,@-r8 */
L8c61a604: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a606; /* fmov.s @(r0,r4),fr3 */
L8c61a606: r[0]=16u;goto L8c61a608; /* mov #0x10,r0 */
L8c61a608: r[2]=rd(m,bus,&io_status,0x8c61a780u,4);if(io_status)return io_status;goto L8c61a60a; /* mov.l 0x8c61a780,r2 */
L8c61a60a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a60c; /* fmov.s fr3,@-r8 */
L8c61a60c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a60e; /* fmov.s @(r0,r4),fr3 */
L8c61a60e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a610; /* fmov.s fr3,@-r8 */
L8c61a610: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61a612; /* mov.l r2,@-r8 */
L8c61a612: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a614; /* pref @r8 */
L8c61a614: r[2]=rd(m,bus,&io_status,0x8c61a770u,4);if(io_status)return io_status;goto L8c61a616; /* mov.l 0x8c61a770,r2 */
L8c61a616: r[0]=4u;goto L8c61a618; /* mov #0x4,r0 */
L8c61a618: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61a61a; /* fmov.s @r5,fr3 */
L8c61a61a: r[8]+=60u;goto L8c61a61c; /* add #0x3c,r8 */
L8c61a61c: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61a61e; /* fmov.s @r2,fr2 */
L8c61a61e: r[2]+=8u;goto L8c61a620; /* add #0x8,r2 */
L8c61a620: r[1]=rd(m,bus,&io_status,0x8c61a774u,4);if(io_status)return io_status;goto L8c61a622; /* mov.l 0x8c61a774,r1 */
L8c61a622: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a624; /* fmul fr3,fr2 */
L8c61a624: r[3]=rd(m,bus,&io_status,0x8c61a778u,4);if(io_status)return io_status;goto L8c61a626; /* mov.l 0x8c61a778,r3 */
L8c61a626: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61a628; /* fmov.s @r1,fr3 */
L8c61a628: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a62a; /* fmov.s @r3,fr1 */
L8c61a62a: r[3]=rd(m,bus,&io_status,0x8c61a77cu,4);if(io_status)return io_status;goto L8c61a62c; /* mov.l 0x8c61a77c,r3 */
L8c61a62c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a62e; /* fsub fr2,fr3 */
L8c61a62e: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a630; /* fmov.s @(r0,r5),fr2 */
L8c61a630: r[0]=8u;goto L8c61a632; /* mov #0x8,r0 */
L8c61a632: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a634; /* fmul fr2,fr1 */
L8c61a634: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a636; /* fmov.s @(r0,r5),fr2 */
L8c61a636: r[0]=4u;goto L8c61a638; /* mov #0x4,r0 */
L8c61a638: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a63a; /* fsub fr1,fr3 */
L8c61a63a: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61a63c; /* fmov.s @r2,fr1 */
L8c61a63c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a63e; /* fmul fr2,fr1 */
L8c61a63e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a640; /* fsub fr1,fr3 */
L8c61a640: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a642; /* fmov.s fr3,@-r8 */
L8c61a642: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a644; /* fmov.s @(r0,r5),fr0 */
L8c61a644: r[0]=20u;goto L8c61a646; /* mov #0x14,r0 */
L8c61a646: r[10]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a648; /* mov.l @r3,r10 */
L8c61a648: fload(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c61a64a; /* fmov.s @(r0,r10),fr3 */
L8c61a64a: r[0]=4u;goto L8c61a64c; /* mov #0x4,r0 */
L8c61a64c: fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c61a64e; /* fmov.s @(r0,r10),fr2 */
L8c61a64e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a650; /* fmul fr0,fr3 */
L8c61a650: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a652; /* fmov.s @r5,fr0 */
L8c61a652: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a654; /* fmac fr0,fr2,fr3 */
L8c61a654: r[0]=8u;goto L8c61a656; /* mov #0x8,r0 */
L8c61a656: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a658; /* fmov.s @(r0,r5),fr0 */
L8c61a658: r[0]=36u;goto L8c61a65a; /* mov #0x24,r0 */
L8c61a65a: fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c61a65c; /* fmov.s @(r0,r10),fr2 */
L8c61a65c: r[0]=4u;goto L8c61a65e; /* mov #0x4,r0 */
L8c61a65e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a660; /* fmac fr0,fr2,fr3 */
L8c61a660: fcopy(f,0u,15u);goto L8c61a662; /* fmov fr15,fr0 */
L8c61a662: fcopy(f,2u,15u);goto L8c61a664; /* fmov fr15,fr2 */
L8c61a664: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a666; /* fmac fr0,fr3,fr2 */
L8c61a666: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a668; /* fmov.s fr2,@-r8 */
L8c61a668: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a66a; /* fmov.s @(r0,r5),fr0 */
L8c61a66a: r[0]=16u;goto L8c61a66c; /* mov #0x10,r0 */
L8c61a66c: r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a66e; /* mov.l @r3,r12 */
L8c61a66e: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61a670; /* fmov.s @(r0,r12),fr3 */
L8c61a670: r[0]=8u;goto L8c61a672; /* mov #0x8,r0 */
L8c61a672: fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61a674; /* fmov.s @r12,fr2 */
L8c61a674: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a676; /* fmul fr0,fr3 */
L8c61a676: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a678; /* fmov.s @r5,fr0 */
L8c61a678: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a67a; /* fmac fr0,fr2,fr3 */
L8c61a67a: fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a67c; /* fmov.s @(r0,r5),fr0 */
L8c61a67c: r[0]=32u;goto L8c61a67e; /* mov #0x20,r0 */
L8c61a67e: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61a680; /* fmov.s @(r0,r12),fr2 */
L8c61a680: r[0]=24u;goto L8c61a682; /* mov #0x18,r0 */
L8c61a682: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a684; /* fmac fr0,fr2,fr3 */
L8c61a684: fcopy(f,0u,15u);goto L8c61a686; /* fmov fr15,fr0 */
L8c61a686: fcopy(f,2u,15u);goto L8c61a688; /* fmov fr15,fr2 */
L8c61a688: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a68a; /* fmac fr0,fr3,fr2 */
L8c61a68a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a68c; /* fmov.s fr2,@-r8 */
L8c61a68c: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a68e; /* fmov.s @(r0,r5),fr3 */
L8c61a68e: r[0]=20u;goto L8c61a690; /* mov #0x14,r0 */
L8c61a690: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a692; /* fmov.s fr3,@-r8 */
L8c61a692: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a694; /* fmov.s @(r0,r5),fr3 */
L8c61a694: r[0]=16u;goto L8c61a696; /* mov #0x10,r0 */
L8c61a696: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a698; /* fmov.s fr3,@-r8 */
L8c61a698: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a69a; /* fmov.s @(r0,r5),fr3 */
L8c61a69a: r[2]=rd(m,bus,&io_status,0x8c61a780u,4);if(io_status)return io_status;goto L8c61a69c; /* mov.l 0x8c61a780,r2 */
L8c61a69c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a69e; /* fmov.s fr3,@-r8 */
L8c61a69e: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61a6a0; /* mov.l r2,@-r8 */
L8c61a6a0: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a6a2; /* pref @r8 */
L8c61a6a2: r[2]=rd(m,bus,&io_status,0x8c61a770u,4);if(io_status)return io_status;goto L8c61a6a4; /* mov.l 0x8c61a770,r2 */
L8c61a6a4: r[0]=4u;goto L8c61a6a6; /* mov #0x4,r0 */
L8c61a6a6: fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c61a6a8; /* fmov.s @r6,fr3 */
L8c61a6a8: r[8]+=60u;goto L8c61a6aa; /* add #0x3c,r8 */
L8c61a6aa: fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a6ac; /* fmov.s @r2+,fr2 */
L8c61a6ac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a6ae; /* fmul fr3,fr2 */
L8c61a6ae: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61a6b0; /* fmov.s @r1,fr3 */
L8c61a6b0: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a6b2; /* fmov.s @r2+,fr1 */
L8c61a6b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a6b4; /* fsub fr2,fr3 */
L8c61a6b4: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a6b6; /* fmov.s @(r0,r6),fr2 */
L8c61a6b6: r[0]=8u;goto L8c61a6b8; /* mov #0x8,r0 */
L8c61a6b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a6ba; /* fmul fr2,fr1 */
L8c61a6ba: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a6bc; /* fmov.s @(r0,r6),fr2 */
L8c61a6bc: r[0]=4u;goto L8c61a6be; /* mov #0x4,r0 */
L8c61a6be: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a6c0; /* fsub fr1,fr3 */
L8c61a6c0: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61a6c2; /* fmov.s @r2,fr1 */
L8c61a6c2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a6c4; /* fmul fr2,fr1 */
L8c61a6c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a6c6; /* fsub fr1,fr3 */
L8c61a6c6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a6c8; /* fmov.s fr3,@-r8 */
L8c61a6c8: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a6ca; /* fmov.s @(r0,r6),fr0 */
L8c61a6ca: r[0]=20u;goto L8c61a6cc; /* mov #0x14,r0 */
L8c61a6cc: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a6ce; /* mov.l @r3,r13 */
L8c61a6ce: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a6d0; /* fmov.s @(r0,r13),fr3 */
L8c61a6d0: r[0]=4u;goto L8c61a6d2; /* mov #0x4,r0 */
L8c61a6d2: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a6d4; /* fmov.s @(r0,r13),fr2 */
L8c61a6d4: r[0]=8u;goto L8c61a6d6; /* mov #0x8,r0 */
L8c61a6d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a6d8; /* fmul fr0,fr3 */
L8c61a6d8: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61a6da; /* fmov.s @r6,fr0 */
L8c61a6da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a6dc; /* fmac fr0,fr2,fr3 */
L8c61a6dc: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a6de; /* fmov.s @(r0,r6),fr0 */
L8c61a6de: r[0]=36u;goto L8c61a6e0; /* mov #0x24,r0 */
L8c61a6e0: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a6e2; /* fmov.s @(r0,r13),fr2 */
L8c61a6e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a6e4; /* fmac fr0,fr2,fr3 */
L8c61a6e4: fcopy(f,0u,15u);goto L8c61a6e6; /* fmov fr15,fr0 */
L8c61a6e6: fcopy(f,2u,15u);goto L8c61a6e8; /* fmov fr15,fr2 */
L8c61a6e8: r[0]=4u;goto L8c61a6ea; /* mov #0x4,r0 */
L8c61a6ea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a6ec; /* fmac fr0,fr3,fr2 */
L8c61a6ec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a6ee; /* fmov.s fr2,@-r8 */
L8c61a6ee: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a6f0; /* fmov.s @(r0,r6),fr0 */
L8c61a6f0: r[0]=16u;goto L8c61a6f2; /* mov #0x10,r0 */
L8c61a6f2: r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a6f4; /* mov.l @r3,r7 */
L8c61a6f4: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a6f6; /* fmov.s @(r0,r7),fr3 */
L8c61a6f6: r[0]=8u;goto L8c61a6f8; /* mov #0x8,r0 */
L8c61a6f8: fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61a6fa; /* fmov.s @r7,fr2 */
L8c61a6fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a6fc; /* fmul fr0,fr3 */
L8c61a6fc: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61a6fe; /* fmov.s @r6,fr0 */
L8c61a6fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a700; /* fmac fr0,fr2,fr3 */
L8c61a700: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a702; /* fmov.s @(r0,r6),fr0 */
L8c61a702: r[0]=32u;goto L8c61a704; /* mov #0x20,r0 */
L8c61a704: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a706; /* fmov.s @(r0,r7),fr2 */
L8c61a706: r[0]=24u;goto L8c61a708; /* mov #0x18,r0 */
L8c61a708: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a70a; /* fmac fr0,fr2,fr3 */
L8c61a70a: fcopy(f,0u,15u);goto L8c61a70c; /* fmov fr15,fr0 */
L8c61a70c: fcopy(f,2u,15u);goto L8c61a70e; /* fmov fr15,fr2 */
L8c61a70e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a710; /* fmac fr0,fr3,fr2 */
L8c61a710: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a712; /* fmov.s fr2,@-r8 */
L8c61a712: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a714; /* fmov.s @(r0,r6),fr3 */
L8c61a714: r[0]=20u;goto L8c61a716; /* mov #0x14,r0 */
L8c61a716: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a718; /* fmov.s fr3,@-r8 */
L8c61a718: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a71a; /* fmov.s @(r0,r6),fr3 */
L8c61a71a: r[0]=16u;goto L8c61a71c; /* mov #0x10,r0 */
L8c61a71c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a71e; /* fmov.s fr3,@-r8 */
L8c61a71e: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a720; /* fmov.s @(r0,r6),fr3 */
L8c61a720: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a722; /* fmov.s fr3,@-r8 */
L8c61a722: r[2]=rd(m,bus,&io_status,0x8c61a784u,4);if(io_status)return io_status;goto L8c61a724; /* mov.l 0x8c61a784,r2 */
L8c61a724: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61a726; /* mov.l r2,@-r8 */
L8c61a726: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a728; /* pref @r8 */
L8c61a728: r[0]=rd(m,bus,&io_status,0x8c61a788u,4);if(io_status)return io_status;goto L8c61a72a; /* mov.l 0x8c61a788,r0 */
L8c61a72a: r[8]+=32u;goto L8c61a72c; /* add #0x20,r8 */
L8c61a72c: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61a72e; /* mov.l @r0,r2 */
L8c61a72e: r[2]+=3u;goto L8c61a730; /* add #0x3,r2 */
L8c61a730: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61a73e; /* bra 0x8c61a73e */
L8c61a734: r[0]=rd(m,bus,&io_status,0x8c61a78cu,4);if(io_status)return io_status;goto L8c61a736; /* mov.l 0x8c61a78c,r0 */
L8c61a736: wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61a738; /* mov.l r3,@r0 */
L8c61a738: r[3]=rd(m,bus,&io_status,0x8c61a790u,4);if(io_status)return io_status;goto L8c61a73a; /* mov.l 0x8c61a790,r3 */
L8c61a73a: target=r[3];*pr=0x8c61a73eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61a73e; /* jsr @r3 */
L8c61a73e: r[14]+=1u;goto L8c61a740; /* add #0x1,r14 */
L8c61a740: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61a742; /* mov.l @r15,r2 */
L8c61a742: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61a744; /* mov.w @(0x2,r2),r0 */
L8c61a744: r[0]=r[0]&65535u;goto L8c61a746; /* extu.w r0,r0 */
L8c61a746: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c61a748; /* cmp/ge r0,r14 */
L8c61a748: if((*sr&1u))goto L8c61a74e;goto L8c61a74a; /* bt 0x8c61a74e */
L8c61a74a: goto L8c61a500; /* bra 0x8c61a500 */
L8c61a74e: goto L8c61ade8; /* bra 0x8c61ade8 */
L8c61a752: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61a754; /* mov.l @r15,r1 */
L8c61a754: r[2]=rd(m,bus,&io_status,0x8c61a794u,4);if(io_status)return io_status;goto L8c61a756; /* mov.l 0x8c61a794,r2 */
L8c61a756: r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61a758; /* mov.w @(0x2,r1),r0 */
L8c61a758: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a75a; /* mov.l @r2,r1 */
L8c61a75a: r[0]=r[0]&65535u;goto L8c61a75c; /* extu.w r0,r0 */
L8c61a75c: r[0]<<=2;goto L8c61a75e; /* shll2 r0 */
L8c61a75e: r[1]+=r[0];goto L8c61a760; /* add r0,r1 */
L8c61a760: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61a762; /* mov.l r1,@r2 */
L8c61a762: r[10]=0u;goto L8c61aabc; /* bra 0x8c61aabc */
L8c61a798: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61a79a; /* mov.w @r11+,r4 */
L8c61a79a: r[3]=rd(m,bus,&io_status,0x8c61aad8u,4);if(io_status)return io_status;goto L8c61a79c; /* mov.l 0x8c61aad8,r3 */
L8c61a79c: r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61a79e; /* mov.w @r11+,r13 */
L8c61a79e: r[4]=r[4]&65535u;goto L8c61a7a0; /* extu.w r4,r4 */
L8c61a7a0: r[4]<<=2;goto L8c61a7a2; /* shll2 r4 */
L8c61a7a2: r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a7a4; /* mov.l @r3,r6 */
L8c61a7a4: r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61a7a6; /* mov.w @r11+,r14 */
L8c61a7a6: r[13]=r[13]&65535u;goto L8c61a7a8; /* extu.w r13,r13 */
L8c61a7a8: r[13]<<=2;goto L8c61a7aa; /* shll2 r13 */
L8c61a7aa: r[12]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61a7ac; /* mov.w @r11+,r12 */
L8c61a7ac: r[14]=r[14]&65535u;goto L8c61a7ae; /* extu.w r14,r14 */
L8c61a7ae: r[13]<<=2;goto L8c61a7b0; /* shll2 r13 */
L8c61a7b0: r[4]<<=2;goto L8c61a7b2; /* shll2 r4 */
L8c61a7b2: r[12]=r[12]&65535u;goto L8c61a7b4; /* extu.w r12,r12 */
L8c61a7b4: r[14]<<=2;goto L8c61a7b6; /* shll2 r14 */
L8c61a7b6: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61a7b8; /* shll r4 */
L8c61a7b8: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61a7ba; /* shll r13 */
L8c61a7ba: r[4]+=r[6];goto L8c61a7bc; /* add r6,r4 */
L8c61a7bc: r[12]<<=2;goto L8c61a7be; /* shll2 r12 */
L8c61a7be: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61a7c0; /* mov.l @(0xc,r4),r5 */
L8c61a7c0: r[13]+=r[6];goto L8c61a7c2; /* add r6,r13 */
L8c61a7c2: r[14]<<=2;goto L8c61a7c4; /* shll2 r14 */
L8c61a7c4: r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61a7c6; /* mov.l @(0xc,r13),r2 */
L8c61a7c6: r[12]<<=2;goto L8c61a7c8; /* shll2 r12 */
L8c61a7c8: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61a7ca; /* shll r14 */
L8c61a7ca: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61a7cc; /* shll r12 */
L8c61a7cc: r[14]+=r[6];goto L8c61a7ce; /* add r6,r14 */
L8c61a7ce: r[1]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61a7d0; /* mov.l @(0xc,r14),r1 */
L8c61a7d0: r[5]&=r[2];goto L8c61a7d2; /* and r2,r5 */
L8c61a7d2: r[12]+=r[6];goto L8c61a7d4; /* add r6,r12 */
L8c61a7d4: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61a7d6; /* mov.l @(0xc,r12),r2 */
L8c61a7d6: r[5]&=r[1];goto L8c61a7d8; /* and r1,r5 */
L8c61a7d8: r[1]=63u;goto L8c61a7da; /* mov #0x3f,r1 */
L8c61a7da: r[5]&=r[2];goto L8c61a7dc; /* and r2,r5 */
L8c61a7dc: *sr=(*sr&~1u)|!(r[1]&r[5]);goto L8c61a7de; /* tst r1,r5 */
L8c61a7de: if((*sr&1u))goto L8c61a7e4;goto L8c61a7e0; /* bt 0x8c61a7e4 */
L8c61a7e0: goto L8c61aaba; /* bra 0x8c61aaba */
L8c61a7e4: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61a7e6; /* mov.l @(0xc,r4),r5 */
L8c61a7e6: r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61a7e8; /* mov.l @(0xc,r13),r3 */
L8c61a7e8: r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61a7ea; /* mov.l @(0xc,r14),r2 */
L8c61a7ea: r[5]|=r[3];goto L8c61a7ec; /* or r3,r5 */
L8c61a7ec: r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61a7ee; /* mov.l @(0xc,r12),r3 */
L8c61a7ee: r[5]|=r[2];goto L8c61a7f0; /* or r2,r5 */
L8c61a7f0: r[2]=1u;goto L8c61a7f2; /* mov #0x1,r2 */
L8c61a7f2: r[5]|=r[3];goto L8c61a7f4; /* or r3,r5 */
L8c61a7f4: *sr=(*sr&~1u)|!(r[2]&r[5]);goto L8c61a7f6; /* tst r2,r5 */
L8c61a7f6: if((*sr&1u))goto L8c61a7fc;goto L8c61a7f8; /* bt 0x8c61a7fc */
L8c61a7f8: goto L8c61aa84; /* bra 0x8c61aa84 */
L8c61a7fc: r[0]=16u;goto L8c61a7fe; /* mov #0x10,r0 */
L8c61a7fe: fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a800; /* fmov.s @(r0,r13),fr4 */
L8c61a800: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a802; /* fmov.s @(r0,r4),fr3 */
L8c61a802: r[0]=20u;goto L8c61a804; /* mov #0x14,r0 */
L8c61a804: fcopy(f,2u,4u);goto L8c61a806; /* fmov fr4,fr2 */
L8c61a806: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61a808; /* fsub fr3,fr2 */
L8c61a808: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a80a; /* fmov.s @(r0,r14),fr3 */
L8c61a80a: fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a80c; /* fmov.s @(r0,r13),fr5 */
L8c61a80c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a80e; /* fsub fr5,fr3 */
L8c61a80e: fcopy(f,1u,5u);goto L8c61a810; /* fmov fr5,fr1 */
L8c61a810: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a812; /* fmul fr3,fr2 */
L8c61a812: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a814; /* fmov.s @(r0,r4),fr3 */
L8c61a814: r[0]=16u;goto L8c61a816; /* mov #0x10,r0 */
L8c61a816: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61a818; /* fsub fr3,fr1 */
L8c61a818: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a81a; /* fmov.s @(r0,r14),fr3 */
L8c61a81a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a81c; /* fsub fr4,fr3 */
L8c61a81c: fcopy(f,4u,2u);goto L8c61a81e; /* fmov fr2,fr4 */
L8c61a81e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a820; /* fmul fr3,fr1 */
L8c61a820: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61a822; /* fldi0 fr3 */
L8c61a822: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61a824; /* fsub fr1,fr4 */
L8c61a824: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61a826; /* fcmp/gt fr3,fr4 */
L8c61a826: if(!(*sr&1u))goto L8c61a82c;goto L8c61a828; /* bf 0x8c61a82c */
L8c61a828: goto L8c61aaba; /* bra 0x8c61aaba */
L8c61a82c: r[3]=rd(m,bus,&io_status,0x8c61aadcu,4);if(io_status)return io_status;goto L8c61a82e; /* mov.l 0x8c61aadc,r3 */
L8c61a82e: r[0]=4u;goto L8c61a830; /* mov #0x4,r0 */
L8c61a830: fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61a832; /* fmov.s @r4,fr3 */
L8c61a832: r[8]+=28u;goto L8c61a834; /* add #0x1c,r8 */
L8c61a834: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a836; /* fmov.s @r3,fr2 */
L8c61a836: r[3]+=8u;goto L8c61a838; /* add #0x8,r3 */
L8c61a838: r[2]=rd(m,bus,&io_status,0x8c61aae0u,4);if(io_status)return io_status;goto L8c61a83a; /* mov.l 0x8c61aae0,r2 */
L8c61a83a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a83c; /* fmul fr3,fr2 */
L8c61a83c: r[1]=rd(m,bus,&io_status,0x8c61aae4u,4);if(io_status)return io_status;goto L8c61a83e; /* mov.l 0x8c61aae4,r1 */
L8c61a83e: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61a840; /* fmov.s @r2,fr3 */
L8c61a840: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61a842; /* fmov.s @r1,fr1 */
L8c61a842: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a844; /* fsub fr2,fr3 */
L8c61a844: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a846; /* fmov.s @(r0,r4),fr2 */
L8c61a846: r[0]=8u;goto L8c61a848; /* mov #0x8,r0 */
L8c61a848: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a84a; /* fmul fr2,fr1 */
L8c61a84a: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a84c; /* fmov.s @(r0,r4),fr2 */
L8c61a84c: r[0]=rd(m,bus,&io_status,0x8c61aae8u,4);if(io_status)return io_status;goto L8c61a84e; /* mov.l 0x8c61aae8,r0 */
L8c61a84e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a850; /* fsub fr1,fr3 */
L8c61a850: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a852; /* fmov.s @r3,fr1 */
L8c61a852: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a854; /* fmul fr2,fr1 */
L8c61a854: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a856; /* fsub fr1,fr3 */
L8c61a856: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a858; /* fmov.s fr3,@-r8 */
L8c61a858: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61a85a; /* mov.l @r0,r2 */
L8c61a85a: r[0]=4u;goto L8c61a85c; /* mov #0x4,r0 */
L8c61a85c: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a85e; /* fmov.s @(r0,r4),fr0 */
L8c61a85e: r[0]=20u;goto L8c61a860; /* mov #0x14,r0 */
L8c61a860: r[1]=r[2];goto L8c61a862; /* mov r2,r1 */
L8c61a862: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a864; /* fmov.s @(r0,r1),fr3 */
L8c61a864: r[0]=4u;goto L8c61a866; /* mov #0x4,r0 */
L8c61a866: r[3]=r[2];goto L8c61a868; /* mov r2,r3 */
L8c61a868: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a86a; /* fmul fr0,fr3 */
L8c61a86a: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61a86c; /* fmov.s @r4,fr0 */
L8c61a86c: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a86e; /* fmov.s @(r0,r3),fr2 */
L8c61a86e: r[0]=8u;goto L8c61a870; /* mov #0x8,r0 */
L8c61a870: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a872; /* fmac fr0,fr2,fr3 */
L8c61a872: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a874; /* fmov.s @(r0,r4),fr0 */
L8c61a874: r[0]=36u;goto L8c61a876; /* mov #0x24,r0 */
L8c61a876: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a878; /* fmov.s @(r0,r2),fr2 */
L8c61a878: r[0]=4u;goto L8c61a87a; /* mov #0x4,r0 */
L8c61a87a: r[2]=rd(m,bus,&io_status,0x8c61aae8u,4);if(io_status)return io_status;goto L8c61a87c; /* mov.l 0x8c61aae8,r2 */
L8c61a87c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a87e; /* fmac fr0,fr2,fr3 */
L8c61a87e: fcopy(f,0u,15u);goto L8c61a880; /* fmov fr15,fr0 */
L8c61a880: fcopy(f,2u,15u);goto L8c61a882; /* fmov fr15,fr2 */
L8c61a882: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a884; /* fmac fr0,fr3,fr2 */
L8c61a884: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a886; /* fmov.s fr2,@-r8 */
L8c61a886: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a888; /* mov.l @r2,r3 */
L8c61a888: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a88a; /* fmov.s @(r0,r4),fr0 */
L8c61a88a: r[0]=16u;goto L8c61a88c; /* mov #0x10,r0 */
L8c61a88c: r[2]=r[3];goto L8c61a88e; /* mov r3,r2 */
L8c61a88e: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a890; /* fmov.s @(r0,r2),fr3 */
L8c61a890: r[1]=r[3];goto L8c61a892; /* mov r3,r1 */
L8c61a892: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61a894; /* fmov.s @r1,fr2 */
L8c61a894: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a896; /* fmul fr0,fr3 */
L8c61a896: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61a898; /* fmov.s @r4,fr0 */
L8c61a898: r[0]=8u;goto L8c61a89a; /* mov #0x8,r0 */
L8c61a89a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a89c; /* fmac fr0,fr2,fr3 */
L8c61a89c: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a89e; /* fmov.s @(r0,r4),fr0 */
L8c61a89e: r[0]=32u;goto L8c61a8a0; /* mov #0x20,r0 */
L8c61a8a0: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a8a2; /* fmov.s @(r0,r3),fr2 */
L8c61a8a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a8a4; /* fmac fr0,fr2,fr3 */
L8c61a8a4: fcopy(f,0u,15u);goto L8c61a8a6; /* fmov fr15,fr0 */
L8c61a8a6: fcopy(f,2u,15u);goto L8c61a8a8; /* fmov fr15,fr2 */
L8c61a8a8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a8aa; /* fmac fr0,fr3,fr2 */
L8c61a8aa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a8ac; /* fmov.s fr2,@-r8 */
L8c61a8ac: r[0]=24u;goto L8c61a8ae; /* mov #0x18,r0 */
L8c61a8ae: r[3]=rd(m,bus,&io_status,0x8c61aaecu,4);if(io_status)return io_status;goto L8c61a8b0; /* mov.l 0x8c61aaec,r3 */
L8c61a8b0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a8b2; /* fmov.s @(r0,r4),fr3 */
L8c61a8b2: r[0]=20u;goto L8c61a8b4; /* mov #0x14,r0 */
L8c61a8b4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a8b6; /* fmov.s fr3,@-r8 */
L8c61a8b6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a8b8; /* fmov.s @(r0,r4),fr3 */
L8c61a8b8: r[0]=16u;goto L8c61a8ba; /* mov #0x10,r0 */
L8c61a8ba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a8bc; /* fmov.s fr3,@-r8 */
L8c61a8bc: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a8be; /* fmov.s @(r0,r4),fr3 */
L8c61a8be: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a8c0; /* fmov.s fr3,@-r8 */
L8c61a8c0: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a8c2; /* mov.l r3,@-r8 */
L8c61a8c2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a8c4; /* pref @r8 */
L8c61a8c4: r[3]=rd(m,bus,&io_status,0x8c61aadcu,4);if(io_status)return io_status;goto L8c61a8c6; /* mov.l 0x8c61aadc,r3 */
L8c61a8c6: r[0]=4u;goto L8c61a8c8; /* mov #0x4,r0 */
L8c61a8c8: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61a8ca; /* fmov.s @r13,fr3 */
L8c61a8ca: r[8]+=60u;goto L8c61a8cc; /* add #0x3c,r8 */
L8c61a8cc: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a8ce; /* fmov.s @r3,fr2 */
L8c61a8ce: r[3]+=8u;goto L8c61a8d0; /* add #0x8,r3 */
L8c61a8d0: r[2]=rd(m,bus,&io_status,0x8c61aae0u,4);if(io_status)return io_status;goto L8c61a8d2; /* mov.l 0x8c61aae0,r2 */
L8c61a8d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a8d4; /* fmul fr3,fr2 */
L8c61a8d4: r[1]=rd(m,bus,&io_status,0x8c61aae4u,4);if(io_status)return io_status;goto L8c61a8d6; /* mov.l 0x8c61aae4,r1 */
L8c61a8d6: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61a8d8; /* fmov.s @r2,fr3 */
L8c61a8d8: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61a8da; /* fmov.s @r1,fr1 */
L8c61a8da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a8dc; /* fsub fr2,fr3 */
L8c61a8dc: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a8de; /* fmov.s @(r0,r13),fr2 */
L8c61a8de: r[0]=8u;goto L8c61a8e0; /* mov #0x8,r0 */
L8c61a8e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a8e2; /* fmul fr2,fr1 */
L8c61a8e2: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a8e4; /* fmov.s @(r0,r13),fr2 */
L8c61a8e4: r[0]=rd(m,bus,&io_status,0x8c61aae8u,4);if(io_status)return io_status;goto L8c61a8e6; /* mov.l 0x8c61aae8,r0 */
L8c61a8e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a8e8; /* fsub fr1,fr3 */
L8c61a8e8: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a8ea; /* fmov.s @r3,fr1 */
L8c61a8ea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a8ec; /* fmul fr2,fr1 */
L8c61a8ec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a8ee; /* fsub fr1,fr3 */
L8c61a8ee: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a8f0; /* fmov.s fr3,@-r8 */
L8c61a8f0: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61a8f2; /* mov.l @r0,r2 */
L8c61a8f2: r[0]=4u;goto L8c61a8f4; /* mov #0x4,r0 */
L8c61a8f4: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a8f6; /* fmov.s @(r0,r13),fr0 */
L8c61a8f6: r[0]=20u;goto L8c61a8f8; /* mov #0x14,r0 */
L8c61a8f8: r[1]=r[2];goto L8c61a8fa; /* mov r2,r1 */
L8c61a8fa: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a8fc; /* fmov.s @(r0,r1),fr3 */
L8c61a8fc: r[0]=4u;goto L8c61a8fe; /* mov #0x4,r0 */
L8c61a8fe: r[3]=r[2];goto L8c61a900; /* mov r2,r3 */
L8c61a900: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a902; /* fmul fr0,fr3 */
L8c61a902: fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61a904; /* fmov.s @r13,fr0 */
L8c61a904: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a906; /* fmov.s @(r0,r3),fr2 */
L8c61a906: r[0]=8u;goto L8c61a908; /* mov #0x8,r0 */
L8c61a908: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a90a; /* fmac fr0,fr2,fr3 */
L8c61a90a: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a90c; /* fmov.s @(r0,r13),fr0 */
L8c61a90c: r[0]=36u;goto L8c61a90e; /* mov #0x24,r0 */
L8c61a90e: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a910; /* fmov.s @(r0,r2),fr2 */
L8c61a910: r[0]=4u;goto L8c61a912; /* mov #0x4,r0 */
L8c61a912: r[2]=rd(m,bus,&io_status,0x8c61aae8u,4);if(io_status)return io_status;goto L8c61a914; /* mov.l 0x8c61aae8,r2 */
L8c61a914: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a916; /* fmac fr0,fr2,fr3 */
L8c61a916: fcopy(f,0u,15u);goto L8c61a918; /* fmov fr15,fr0 */
L8c61a918: fcopy(f,2u,15u);goto L8c61a91a; /* fmov fr15,fr2 */
L8c61a91a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a91c; /* fmac fr0,fr3,fr2 */
L8c61a91c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a91e; /* fmov.s fr2,@-r8 */
L8c61a91e: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a920; /* mov.l @r2,r3 */
L8c61a920: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a922; /* fmov.s @(r0,r13),fr0 */
L8c61a922: r[0]=16u;goto L8c61a924; /* mov #0x10,r0 */
L8c61a924: r[2]=r[3];goto L8c61a926; /* mov r3,r2 */
L8c61a926: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a928; /* fmov.s @(r0,r2),fr3 */
L8c61a928: r[1]=r[3];goto L8c61a92a; /* mov r3,r1 */
L8c61a92a: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61a92c; /* fmov.s @r1,fr2 */
L8c61a92c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a92e; /* fmul fr0,fr3 */
L8c61a92e: fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61a930; /* fmov.s @r13,fr0 */
L8c61a930: r[0]=8u;goto L8c61a932; /* mov #0x8,r0 */
L8c61a932: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a934; /* fmac fr0,fr2,fr3 */
L8c61a934: fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a936; /* fmov.s @(r0,r13),fr0 */
L8c61a936: r[0]=32u;goto L8c61a938; /* mov #0x20,r0 */
L8c61a938: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a93a; /* fmov.s @(r0,r3),fr2 */
L8c61a93a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a93c; /* fmac fr0,fr2,fr3 */
L8c61a93c: fcopy(f,0u,15u);goto L8c61a93e; /* fmov fr15,fr0 */
L8c61a93e: fcopy(f,2u,15u);goto L8c61a940; /* fmov fr15,fr2 */
L8c61a940: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a942; /* fmac fr0,fr3,fr2 */
L8c61a942: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a944; /* fmov.s fr2,@-r8 */
L8c61a944: r[0]=24u;goto L8c61a946; /* mov #0x18,r0 */
L8c61a946: r[3]=rd(m,bus,&io_status,0x8c61aaecu,4);if(io_status)return io_status;goto L8c61a948; /* mov.l 0x8c61aaec,r3 */
L8c61a948: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a94a; /* fmov.s @(r0,r13),fr3 */
L8c61a94a: r[0]=20u;goto L8c61a94c; /* mov #0x14,r0 */
L8c61a94c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a94e; /* fmov.s fr3,@-r8 */
L8c61a94e: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a950; /* fmov.s @(r0,r13),fr3 */
L8c61a950: r[0]=16u;goto L8c61a952; /* mov #0x10,r0 */
L8c61a952: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a954; /* fmov.s fr3,@-r8 */
L8c61a954: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a956; /* fmov.s @(r0,r13),fr3 */
L8c61a956: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a958; /* fmov.s fr3,@-r8 */
L8c61a958: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a95a; /* mov.l r3,@-r8 */
L8c61a95a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a95c; /* pref @r8 */
L8c61a95c: r[3]=rd(m,bus,&io_status,0x8c61aadcu,4);if(io_status)return io_status;goto L8c61a95e; /* mov.l 0x8c61aadc,r3 */
L8c61a95e: r[0]=4u;goto L8c61a960; /* mov #0x4,r0 */
L8c61a960: fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61a962; /* fmov.s @r14,fr3 */
L8c61a962: r[8]+=60u;goto L8c61a964; /* add #0x3c,r8 */
L8c61a964: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a966; /* fmov.s @r3,fr2 */
L8c61a966: r[3]+=8u;goto L8c61a968; /* add #0x8,r3 */
L8c61a968: r[2]=rd(m,bus,&io_status,0x8c61aae0u,4);if(io_status)return io_status;goto L8c61a96a; /* mov.l 0x8c61aae0,r2 */
L8c61a96a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a96c; /* fmul fr3,fr2 */
L8c61a96c: r[1]=rd(m,bus,&io_status,0x8c61aae4u,4);if(io_status)return io_status;goto L8c61a96e; /* mov.l 0x8c61aae4,r1 */
L8c61a96e: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61a970; /* fmov.s @r2,fr3 */
L8c61a970: fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61a972; /* fmov.s @r1,fr1 */
L8c61a972: r[2]=rd(m,bus,&io_status,0x8c61aae8u,4);if(io_status)return io_status;goto L8c61a974; /* mov.l 0x8c61aae8,r2 */
L8c61a974: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a976; /* fsub fr2,fr3 */
L8c61a976: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a978; /* fmov.s @(r0,r14),fr2 */
L8c61a978: r[0]=8u;goto L8c61a97a; /* mov #0x8,r0 */
L8c61a97a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a97c; /* fmul fr2,fr1 */
L8c61a97c: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a97e; /* fmov.s @(r0,r14),fr2 */
L8c61a97e: r[0]=4u;goto L8c61a980; /* mov #0x4,r0 */
L8c61a980: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a982; /* fsub fr1,fr3 */
L8c61a982: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a984; /* fmov.s @r3,fr1 */
L8c61a984: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a986; /* fmul fr2,fr1 */
L8c61a986: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a988; /* fsub fr1,fr3 */
L8c61a988: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a98a; /* fmov.s fr3,@-r8 */
L8c61a98a: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a98c; /* fmov.s @(r0,r14),fr0 */
L8c61a98c: r[0]=20u;goto L8c61a98e; /* mov #0x14,r0 */
L8c61a98e: r[9]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a990; /* mov.l @r2,r9 */
L8c61a990: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61a992; /* fmov.s @(r0,r9),fr3 */
L8c61a992: r[0]=4u;goto L8c61a994; /* mov #0x4,r0 */
L8c61a994: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61a996; /* fmov.s @(r0,r9),fr2 */
L8c61a996: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a998; /* fmul fr0,fr3 */
L8c61a998: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61a99a; /* fmov.s @r14,fr0 */
L8c61a99a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a99c; /* fmac fr0,fr2,fr3 */
L8c61a99c: r[0]=8u;goto L8c61a99e; /* mov #0x8,r0 */
L8c61a99e: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a9a0; /* fmov.s @(r0,r14),fr0 */
L8c61a9a0: r[0]=36u;goto L8c61a9a2; /* mov #0x24,r0 */
L8c61a9a2: fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61a9a4; /* fmov.s @(r0,r9),fr2 */
L8c61a9a4: r[0]=4u;goto L8c61a9a6; /* mov #0x4,r0 */
L8c61a9a6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a9a8; /* fmac fr0,fr2,fr3 */
L8c61a9a8: fcopy(f,0u,15u);goto L8c61a9aa; /* fmov fr15,fr0 */
L8c61a9aa: fcopy(f,2u,15u);goto L8c61a9ac; /* fmov fr15,fr2 */
L8c61a9ac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a9ae; /* fmac fr0,fr3,fr2 */
L8c61a9ae: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a9b0; /* fmov.s fr2,@-r8 */
L8c61a9b0: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a9b2; /* fmov.s @(r0,r14),fr0 */
L8c61a9b2: r[0]=16u;goto L8c61a9b4; /* mov #0x10,r0 */
L8c61a9b4: r[7]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a9b6; /* mov.l @r2,r7 */
L8c61a9b6: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a9b8; /* fmov.s @(r0,r7),fr3 */
L8c61a9b8: r[0]=8u;goto L8c61a9ba; /* mov #0x8,r0 */
L8c61a9ba: fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61a9bc; /* fmov.s @r7,fr2 */
L8c61a9bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a9be; /* fmul fr0,fr3 */
L8c61a9be: fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61a9c0; /* fmov.s @r14,fr0 */
L8c61a9c0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a9c2; /* fmac fr0,fr2,fr3 */
L8c61a9c2: fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a9c4; /* fmov.s @(r0,r14),fr0 */
L8c61a9c4: r[0]=32u;goto L8c61a9c6; /* mov #0x20,r0 */
L8c61a9c6: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a9c8; /* fmov.s @(r0,r7),fr2 */
L8c61a9c8: r[0]=24u;goto L8c61a9ca; /* mov #0x18,r0 */
L8c61a9ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a9cc; /* fmac fr0,fr2,fr3 */
L8c61a9cc: fcopy(f,0u,15u);goto L8c61a9ce; /* fmov fr15,fr0 */
L8c61a9ce: fcopy(f,2u,15u);goto L8c61a9d0; /* fmov fr15,fr2 */
L8c61a9d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a9d2; /* fmac fr0,fr3,fr2 */
L8c61a9d2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a9d4; /* fmov.s fr2,@-r8 */
L8c61a9d4: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a9d6; /* fmov.s @(r0,r14),fr3 */
L8c61a9d6: r[0]=20u;goto L8c61a9d8; /* mov #0x14,r0 */
L8c61a9d8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a9da; /* fmov.s fr3,@-r8 */
L8c61a9da: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a9dc; /* fmov.s @(r0,r14),fr3 */
L8c61a9dc: r[0]=16u;goto L8c61a9de; /* mov #0x10,r0 */
L8c61a9de: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a9e0; /* fmov.s fr3,@-r8 */
L8c61a9e0: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a9e2; /* fmov.s @(r0,r14),fr3 */
L8c61a9e2: r[3]=rd(m,bus,&io_status,0x8c61aaecu,4);if(io_status)return io_status;goto L8c61a9e4; /* mov.l 0x8c61aaec,r3 */
L8c61a9e4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a9e6; /* fmov.s fr3,@-r8 */
L8c61a9e6: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a9e8; /* mov.l r3,@-r8 */
L8c61a9e8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a9ea; /* pref @r8 */
L8c61a9ea: r[3]=rd(m,bus,&io_status,0x8c61aadcu,4);if(io_status)return io_status;goto L8c61a9ec; /* mov.l 0x8c61aadc,r3 */
L8c61a9ec: r[0]=4u;goto L8c61a9ee; /* mov #0x4,r0 */
L8c61a9ee: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61a9f0; /* fmov.s @r12,fr3 */
L8c61a9f0: r[8]+=60u;goto L8c61a9f2; /* add #0x3c,r8 */
L8c61a9f2: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a9f4; /* fmov.s @r3,fr2 */
L8c61a9f4: r[3]+=8u;goto L8c61a9f6; /* add #0x8,r3 */
L8c61a9f6: r[1]=rd(m,bus,&io_status,0x8c61aae0u,4);if(io_status)return io_status;goto L8c61a9f8; /* mov.l 0x8c61aae0,r1 */
L8c61a9f8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a9fa; /* fmul fr3,fr2 */
L8c61a9fa: r[2]=rd(m,bus,&io_status,0x8c61aae4u,4);if(io_status)return io_status;goto L8c61a9fc; /* mov.l 0x8c61aae4,r2 */
L8c61a9fc: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61a9fe; /* fmov.s @r1,fr3 */
L8c61a9fe: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61aa00; /* fmov.s @r2,fr1 */
L8c61aa00: r[2]=rd(m,bus,&io_status,0x8c61aae8u,4);if(io_status)return io_status;goto L8c61aa02; /* mov.l 0x8c61aae8,r2 */
L8c61aa02: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61aa04; /* fsub fr2,fr3 */
L8c61aa04: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa06; /* fmov.s @(r0,r12),fr2 */
L8c61aa06: r[0]=8u;goto L8c61aa08; /* mov #0x8,r0 */
L8c61aa08: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61aa0a; /* fmul fr2,fr1 */
L8c61aa0a: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa0c; /* fmov.s @(r0,r12),fr2 */
L8c61aa0c: r[0]=4u;goto L8c61aa0e; /* mov #0x4,r0 */
L8c61aa0e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61aa10; /* fsub fr1,fr3 */
L8c61aa10: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61aa12; /* fmov.s @r3,fr1 */
L8c61aa12: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61aa14; /* fmul fr2,fr1 */
L8c61aa14: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61aa16; /* fsub fr1,fr3 */
L8c61aa16: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61aa18; /* fmov.s fr3,@-r8 */
L8c61aa18: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa1a; /* fmov.s @(r0,r12),fr0 */
L8c61aa1a: r[0]=20u;goto L8c61aa1c; /* mov #0x14,r0 */
L8c61aa1c: r[6]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61aa1e; /* mov.l @r2,r6 */
L8c61aa1e: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61aa20; /* fmov.s @(r0,r6),fr3 */
L8c61aa20: r[0]=4u;goto L8c61aa22; /* mov #0x4,r0 */
L8c61aa22: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61aa24; /* fmov.s @(r0,r6),fr2 */
L8c61aa24: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61aa26; /* fmul fr0,fr3 */
L8c61aa26: fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61aa28; /* fmov.s @r12,fr0 */
L8c61aa28: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61aa2a; /* fmac fr0,fr2,fr3 */
L8c61aa2a: r[0]=8u;goto L8c61aa2c; /* mov #0x8,r0 */
L8c61aa2c: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa2e; /* fmov.s @(r0,r12),fr0 */
L8c61aa2e: r[0]=36u;goto L8c61aa30; /* mov #0x24,r0 */
L8c61aa30: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61aa32; /* fmov.s @(r0,r6),fr2 */
L8c61aa32: r[0]=4u;goto L8c61aa34; /* mov #0x4,r0 */
L8c61aa34: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61aa36; /* fmac fr0,fr2,fr3 */
L8c61aa36: fcopy(f,0u,15u);goto L8c61aa38; /* fmov fr15,fr0 */
L8c61aa38: fcopy(f,2u,15u);goto L8c61aa3a; /* fmov fr15,fr2 */
L8c61aa3a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61aa3c; /* fmac fr0,fr3,fr2 */
L8c61aa3c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61aa3e; /* fmov.s fr2,@-r8 */
L8c61aa3e: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa40; /* fmov.s @(r0,r12),fr0 */
L8c61aa40: r[0]=16u;goto L8c61aa42; /* mov #0x10,r0 */
L8c61aa42: r[5]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61aa44; /* mov.l @r2,r5 */
L8c61aa44: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61aa46; /* fmov.s @(r0,r5),fr3 */
L8c61aa46: r[0]=8u;goto L8c61aa48; /* mov #0x8,r0 */
L8c61aa48: fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c61aa4a; /* fmov.s @r5,fr2 */
L8c61aa4a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61aa4c; /* fmul fr0,fr3 */
L8c61aa4c: fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61aa4e; /* fmov.s @r12,fr0 */
L8c61aa4e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61aa50; /* fmac fr0,fr2,fr3 */
L8c61aa50: fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa52; /* fmov.s @(r0,r12),fr0 */
L8c61aa52: r[0]=32u;goto L8c61aa54; /* mov #0x20,r0 */
L8c61aa54: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61aa56; /* fmov.s @(r0,r5),fr2 */
L8c61aa56: r[0]=24u;goto L8c61aa58; /* mov #0x18,r0 */
L8c61aa58: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61aa5a; /* fmac fr0,fr2,fr3 */
L8c61aa5a: fcopy(f,0u,15u);goto L8c61aa5c; /* fmov fr15,fr0 */
L8c61aa5c: fcopy(f,2u,15u);goto L8c61aa5e; /* fmov fr15,fr2 */
L8c61aa5e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61aa60; /* fmac fr0,fr3,fr2 */
L8c61aa60: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61aa62; /* fmov.s fr2,@-r8 */
L8c61aa62: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa64; /* fmov.s @(r0,r12),fr3 */
L8c61aa64: r[0]=20u;goto L8c61aa66; /* mov #0x14,r0 */
L8c61aa66: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61aa68; /* fmov.s fr3,@-r8 */
L8c61aa68: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa6a; /* fmov.s @(r0,r12),fr3 */
L8c61aa6a: r[0]=16u;goto L8c61aa6c; /* mov #0x10,r0 */
L8c61aa6c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61aa6e; /* fmov.s fr3,@-r8 */
L8c61aa6e: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa70; /* fmov.s @(r0,r12),fr3 */
L8c61aa70: r[3]=rd(m,bus,&io_status,0x8c61aaf0u,4);if(io_status)return io_status;goto L8c61aa72; /* mov.l 0x8c61aaf0,r3 */
L8c61aa72: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61aa74; /* fmov.s fr3,@-r8 */
L8c61aa74: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61aa76; /* mov.l r3,@-r8 */
L8c61aa76: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61aa78; /* pref @r8 */
L8c61aa78: r[0]=rd(m,bus,&io_status,0x8c61aaf4u,4);if(io_status)return io_status;goto L8c61aa7a; /* mov.l 0x8c61aaf4,r0 */
L8c61aa7a: r[8]+=32u;goto L8c61aa7c; /* add #0x20,r8 */
L8c61aa7c: r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61aa7e; /* mov.l @r0,r3 */
L8c61aa7e: r[3]+=4u;goto L8c61aa80; /* add #0x4,r3 */
L8c61aa80: wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61aaba; /* bra 0x8c61aaba */
L8c61aa84: r[0]=rd(m,bus,&io_status,0x8c61aaf8u,4);if(io_status)return io_status;goto L8c61aa86; /* mov.l 0x8c61aaf8,r0 */
L8c61aa86: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61aa88; /* mov.l r2,@r0 */
L8c61aa88: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61aa8a; /* mov.l @(0xc,r4),r5 */
L8c61aa8a: r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61aa8c; /* mov.l @(0xc,r13),r3 */
L8c61aa8c: r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61aa8e; /* mov.l @(0xc,r14),r2 */
L8c61aa8e: r[5]&=r[3];goto L8c61aa90; /* and r3,r5 */
L8c61aa90: r[3]=1u;goto L8c61aa92; /* mov #0x1,r3 */
L8c61aa92: r[5]&=r[2];goto L8c61aa94; /* and r2,r5 */
L8c61aa94: *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61aa96; /* tst r3,r5 */
L8c61aa96: if(!(*sr&1u))goto L8c61aaa0;goto L8c61aa98; /* bf 0x8c61aaa0 */
L8c61aa98: r[1]=rd(m,bus,&io_status,0x8c61aafcu,4);if(io_status)return io_status;goto L8c61aa9a; /* mov.l 0x8c61aafc,r1 */
L8c61aa9a: r[6]=r[14];goto L8c61aa9c; /* mov r14,r6 */
L8c61aa9c: target=r[1];*pr=0x8c61aaa0u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61aaa0; /* jsr @r1 */
L8c61aaa0: r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61aaa2; /* mov.l @(0xc,r13),r4 */
L8c61aaa2: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61aaa4; /* mov.l @(0xc,r14),r3 */
L8c61aaa4: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61aaa6; /* mov.l @(0xc,r12),r2 */
L8c61aaa6: r[4]&=r[3];goto L8c61aaa8; /* and r3,r4 */
L8c61aaa8: r[3]=1u;goto L8c61aaaa; /* mov #0x1,r3 */
L8c61aaaa: r[4]&=r[2];goto L8c61aaac; /* and r2,r4 */
L8c61aaac: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61aaae; /* tst r3,r4 */
L8c61aaae: if(!(*sr&1u))goto L8c61aaba;goto L8c61aab0; /* bf 0x8c61aaba */
L8c61aab0: r[1]=rd(m,bus,&io_status,0x8c61aafcu,4);if(io_status)return io_status;goto L8c61aab2; /* mov.l 0x8c61aafc,r1 */
L8c61aab2: r[5]=r[14];goto L8c61aab4; /* mov r14,r5 */
L8c61aab4: r[6]=r[12];goto L8c61aab6; /* mov r12,r6 */
L8c61aab6: target=r[1];*pr=0x8c61aabau;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61aaba; /* jsr @r1 */
L8c61aaba: r[10]+=1u;goto L8c61aabc; /* add #0x1,r10 */
L8c61aabc: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61aabe; /* mov.l @r15,r3 */
L8c61aabe: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61aac0; /* mov.w @(0x2,r3),r0 */
L8c61aac0: r[0]=r[0]&65535u;goto L8c61aac2; /* extu.w r0,r0 */
L8c61aac2: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c61aac4; /* cmp/ge r0,r10 */
L8c61aac4: if((*sr&1u))goto L8c61aaca;goto L8c61aac6; /* bt 0x8c61aaca */
L8c61aac6: goto L8c61a798; /* bra 0x8c61a798 */
L8c61aaca: goto L8c61ade8; /* bra 0x8c61ade8 */
L8c61aace: r[9]=0u;goto L8c61ad5c; /* bra 0x8c61ad5c */
L8c61ab00: r[3]=rd(m,bus,&io_status,0x8c61ae08u,4);if(io_status)return io_status;goto L8c61ab02; /* mov.l 0x8c61ae08,r3 */
L8c61ab02: r[6]=0u;goto L8c61ab04; /* mov #0x0,r6 */
L8c61ab04: r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61ab06; /* mov.w @r11+,r13 */
L8c61ab06: r[4]=0u;goto L8c61ab08; /* mov #0x0,r4 */
L8c61ab08: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ab0a; /* mov.l @r3,r2 */
L8c61ab0a: r[5]=r[6];goto L8c61ab0c; /* mov r6,r5 */
L8c61ab0c: r[13]=r[13]&65535u;goto L8c61ab0e; /* extu.w r13,r13 */
L8c61ab0e: r[2]+=r[13];goto L8c61ab10; /* add r13,r2 */
L8c61ab10: *sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c61ab12; /* cmp/pl r13 */
L8c61ab12: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61ab14; /* mov.l r2,@r3 */
L8c61ab14: r[7]=4294967295u;goto L8c61ab16; /* mov #-0x1,r7 */
L8c61ab16: take=!(*sr&1u);r[4]+=r[11];if(take)goto L8c61ab38;goto L8c61ab1a; /* bf/s 0x8c61ab38 */
L8c61ab1a: r[14]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c61ab1c; /* mov.w @r4+,r14 */
L8c61ab1c: r[5]+=1u;goto L8c61ab1e; /* add #0x1,r5 */
L8c61ab1e: r[2]=rd(m,bus,&io_status,0x8c61ae0cu,4);if(io_status)return io_status;goto L8c61ab20; /* mov.l 0x8c61ae0c,r2 */
L8c61ab20: r[14]=r[14]&65535u;goto L8c61ab22; /* extu.w r14,r14 */
L8c61ab22: r[14]<<=2;goto L8c61ab24; /* shll2 r14 */
L8c61ab24: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ab26; /* mov.l @r2,r3 */
L8c61ab26: r[14]<<=2;goto L8c61ab28; /* shll2 r14 */
L8c61ab28: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61ab2a; /* shll r14 */
L8c61ab2a: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[13]);goto L8c61ab2c; /* cmp/ge r13,r5 */
L8c61ab2c: r[14]+=r[3];goto L8c61ab2e; /* add r3,r14 */
L8c61ab2e: wr(m,bus,&io_status,r[15]+0x4u,r[14],4);if(io_status)return io_status;goto L8c61ab30; /* mov.l r14,@(0x4,r15) */
L8c61ab30: r[14]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61ab32; /* mov.l @(0xc,r14),r14 */
L8c61ab32: r[7]&=r[14];goto L8c61ab34; /* and r14,r7 */
L8c61ab34: take=!(*sr&1u);r[6]|=r[14];if(take)goto L8c61ab1a;goto L8c61ab38; /* bf/s 0x8c61ab1a */
L8c61ab38: r[2]=63u;goto L8c61ab3a; /* mov #0x3f,r2 */
L8c61ab3a: *sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c61ab3c; /* tst r2,r7 */
L8c61ab3c: if(!(*sr&1u))goto L8c61ab9c;goto L8c61ab3e; /* bf 0x8c61ab9c */
L8c61ab3e: r[1]=1u;goto L8c61ab40; /* mov #0x1,r1 */
L8c61ab40: *sr=(*sr&~1u)|!(r[1]&r[6]);goto L8c61ab42; /* tst r1,r6 */
L8c61ab42: if((*sr&1u))goto L8c61ab48;goto L8c61ab44; /* bt 0x8c61ab48 */
L8c61ab44: goto L8c61acfc; /* bra 0x8c61acfc */
L8c61ab48: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61ab4a; /* mov.w @(0x2,r11),r0 */
L8c61ab4a: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61ab4c; /* mov.w @r11,r4 */
L8c61ab4c: r[3]=rd(m,bus,&io_status,0x8c61ae0cu,4);if(io_status)return io_status;goto L8c61ab4e; /* mov.l 0x8c61ae0c,r3 */
L8c61ab4e: r[5]=r[0]&65535u;goto L8c61ab50; /* extu.w r0,r5 */
L8c61ab50: r[5]<<=2;goto L8c61ab52; /* shll2 r5 */
L8c61ab52: r[0]=sx(m,bus,&io_status,r[11]+0x4u,2);if(io_status)return io_status;goto L8c61ab54; /* mov.w @(0x4,r11),r0 */
L8c61ab54: r[4]=r[4]&65535u;goto L8c61ab56; /* extu.w r4,r4 */
L8c61ab56: r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ab58; /* mov.l @r3,r7 */
L8c61ab58: r[4]<<=2;goto L8c61ab5a; /* shll2 r4 */
L8c61ab5a: r[5]<<=2;goto L8c61ab5c; /* shll2 r5 */
L8c61ab5c: r[6]=r[0]&65535u;goto L8c61ab5e; /* extu.w r0,r6 */
L8c61ab5e: r[4]<<=2;goto L8c61ab60; /* shll2 r4 */
L8c61ab60: r[0]=16u;goto L8c61ab62; /* mov #0x10,r0 */
L8c61ab62: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61ab64; /* shll r4 */
L8c61ab64: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61ab66; /* shll r5 */
L8c61ab66: r[5]+=r[7];goto L8c61ab68; /* add r7,r5 */
L8c61ab68: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61ab6a; /* fmov.s @(r0,r5),fr5 */
L8c61ab6a: r[6]<<=2;goto L8c61ab6c; /* shll2 r6 */
L8c61ab6c: r[4]+=r[7];goto L8c61ab6e; /* add r7,r4 */
L8c61ab6e: r[6]<<=2;goto L8c61ab70; /* shll2 r6 */
L8c61ab70: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ab72; /* fmov.s @(r0,r4),fr3 */
L8c61ab72: fcopy(f,2u,5u);goto L8c61ab74; /* fmov fr5,fr2 */
L8c61ab74: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61ab76; /* shll r6 */
L8c61ab76: r[0]=20u;goto L8c61ab78; /* mov #0x14,r0 */
L8c61ab78: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61ab7a; /* fsub fr3,fr2 */
L8c61ab7a: r[6]+=r[7];goto L8c61ab7c; /* add r7,r6 */
L8c61ab7c: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61ab7e; /* fmov.s @(r0,r5),fr4 */
L8c61ab7e: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61ab80; /* fmov.s @(r0,r6),fr3 */
L8c61ab80: fcopy(f,1u,4u);goto L8c61ab82; /* fmov fr4,fr1 */
L8c61ab82: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ab84; /* fsub fr4,fr3 */
L8c61ab84: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ab86; /* fmul fr3,fr2 */
L8c61ab86: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ab88; /* fmov.s @(r0,r4),fr3 */
L8c61ab88: r[0]=16u;goto L8c61ab8a; /* mov #0x10,r0 */
L8c61ab8a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61ab8c; /* fsub fr3,fr1 */
L8c61ab8c: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61ab8e; /* fmov.s @(r0,r6),fr3 */
L8c61ab8e: fcopy(f,4u,2u);goto L8c61ab90; /* fmov fr2,fr4 */
L8c61ab90: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ab92; /* fsub fr5,fr3 */
L8c61ab92: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ab94; /* fmul fr3,fr1 */
L8c61ab94: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61ab96; /* fldi0 fr3 */
L8c61ab96: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61ab98; /* fsub fr1,fr4 */
L8c61ab98: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61ab9a; /* fcmp/gt fr3,fr4 */
L8c61ab9a: if(!(*sr&1u))goto L8c61aba2;goto L8c61ab9c; /* bf 0x8c61aba2 */
L8c61ab9c: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61ab9e; /* shll r13 */
L8c61ab9e: r[11]+=r[13];goto L8c61ad5a; /* bra 0x8c61ad5a */
L8c61aba2: r[2]=rd(m,bus,&io_status,0x8c61ae10u,4);if(io_status)return io_status;goto L8c61aba4; /* mov.l 0x8c61ae10,r2 */
L8c61aba4: r[6]=r[13];goto L8c61aba6; /* mov r13,r6 */
L8c61aba6: r[6]+=4294967295u;goto L8c61aba8; /* add #-0x1,r6 */
L8c61aba8: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61abaa; /* mov.l @r2,r3 */
L8c61abaa: *sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c61abac; /* cmp/pl r6 */
L8c61abac: r[3]+=r[13];goto L8c61abae; /* add r13,r3 */
L8c61abae: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61abb0; /* mov.l r3,@r2 */
L8c61abb0: take=!(*sr&1u);r[5]=0u;if(take)goto L8c61ac5a;goto L8c61abb4; /* bf/s 0x8c61ac5a */
L8c61abb4: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61abb6; /* mov.w @r11+,r4 */
L8c61abb6: r[0]=4u;goto L8c61abb8; /* mov #0x4,r0 */
L8c61abb8: r[2]=rd(m,bus,&io_status,0x8c61ae0cu,4);if(io_status)return io_status;goto L8c61abba; /* mov.l 0x8c61ae0c,r2 */
L8c61abba: r[8]+=28u;goto L8c61abbc; /* add #0x1c,r8 */
L8c61abbc: r[4]=r[4]&65535u;goto L8c61abbe; /* extu.w r4,r4 */
L8c61abbe: r[1]=rd(m,bus,&io_status,0x8c61ae18u,4);if(io_status)return io_status;goto L8c61abc0; /* mov.l 0x8c61ae18,r1 */
L8c61abc0: r[4]<<=2;goto L8c61abc2; /* shll2 r4 */
L8c61abc2: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61abc4; /* mov.l @r2,r3 */
L8c61abc4: r[4]<<=2;goto L8c61abc6; /* shll2 r4 */
L8c61abc6: r[2]=rd(m,bus,&io_status,0x8c61ae1cu,4);if(io_status)return io_status;goto L8c61abc8; /* mov.l 0x8c61ae1c,r2 */
L8c61abc8: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61abca; /* shll r4 */
L8c61abca: r[4]+=r[3];goto L8c61abcc; /* add r3,r4 */
L8c61abcc: r[3]=rd(m,bus,&io_status,0x8c61ae14u,4);if(io_status)return io_status;goto L8c61abce; /* mov.l 0x8c61ae14,r3 */
L8c61abce: fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61abd0; /* fmov.s @r4,fr3 */
L8c61abd0: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61abd2; /* fmov.s @r3,fr2 */
L8c61abd2: r[3]+=8u;goto L8c61abd4; /* add #0x8,r3 */
L8c61abd4: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61abd6; /* fmov.s @r2,fr1 */
L8c61abd6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61abd8; /* fmul fr3,fr2 */
L8c61abd8: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61abda; /* fmov.s @r1,fr3 */
L8c61abda: r[2]=rd(m,bus,&io_status,0x8c61ae20u,4);if(io_status)return io_status;goto L8c61abdc; /* mov.l 0x8c61ae20,r2 */
L8c61abdc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61abde; /* fsub fr2,fr3 */
L8c61abde: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61abe0; /* fmov.s @(r0,r4),fr2 */
L8c61abe0: r[0]=8u;goto L8c61abe2; /* mov #0x8,r0 */
L8c61abe2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61abe4; /* fmul fr2,fr1 */
L8c61abe4: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61abe6; /* fmov.s @(r0,r4),fr2 */
L8c61abe6: r[0]=4u;goto L8c61abe8; /* mov #0x4,r0 */
L8c61abe8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61abea; /* fsub fr1,fr3 */
L8c61abea: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61abec; /* fmov.s @r3,fr1 */
L8c61abec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61abee; /* fmul fr2,fr1 */
L8c61abee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61abf0; /* fsub fr1,fr3 */
L8c61abf0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61abf2; /* fmov.s fr3,@-r8 */
L8c61abf2: r[14]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61abf4; /* mov.l @r2,r14 */
L8c61abf4: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61abf6; /* fmov.s @(r0,r4),fr0 */
L8c61abf6: r[0]=20u;goto L8c61abf8; /* mov #0x14,r0 */
L8c61abf8: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61abfa; /* fmov.s @(r0,r14),fr3 */
L8c61abfa: r[0]=4u;goto L8c61abfc; /* mov #0x4,r0 */
L8c61abfc: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61abfe; /* fmov.s @(r0,r14),fr2 */
L8c61abfe: r[0]=8u;goto L8c61ac00; /* mov #0x8,r0 */
L8c61ac00: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ac02; /* fmul fr0,fr3 */
L8c61ac02: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ac04; /* fmov.s @r4,fr0 */
L8c61ac04: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ac06; /* fmac fr0,fr2,fr3 */
L8c61ac06: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac08; /* fmov.s @(r0,r4),fr0 */
L8c61ac08: r[0]=36u;goto L8c61ac0a; /* mov #0x24,r0 */
L8c61ac0a: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ac0c; /* fmov.s @(r0,r14),fr2 */
L8c61ac0c: r[0]=4u;goto L8c61ac0e; /* mov #0x4,r0 */
L8c61ac0e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ac10; /* fmac fr0,fr2,fr3 */
L8c61ac10: fcopy(f,0u,15u);goto L8c61ac12; /* fmov fr15,fr0 */
L8c61ac12: fcopy(f,2u,15u);goto L8c61ac14; /* fmov fr15,fr2 */
L8c61ac14: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ac16; /* fmac fr0,fr3,fr2 */
L8c61ac16: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ac18; /* fmov.s fr2,@-r8 */
L8c61ac18: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac1a; /* fmov.s @(r0,r4),fr0 */
L8c61ac1a: r[0]=16u;goto L8c61ac1c; /* mov #0x10,r0 */
L8c61ac1c: r[7]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ac1e; /* mov.l @r2,r7 */
L8c61ac1e: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61ac20; /* fmov.s @(r0,r7),fr3 */
L8c61ac20: r[0]=8u;goto L8c61ac22; /* mov #0x8,r0 */
L8c61ac22: fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61ac24; /* fmov.s @r7,fr2 */
L8c61ac24: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ac26; /* fmul fr0,fr3 */
L8c61ac26: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ac28; /* fmov.s @r4,fr0 */
L8c61ac28: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ac2a; /* fmac fr0,fr2,fr3 */
L8c61ac2a: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac2c; /* fmov.s @(r0,r4),fr0 */
L8c61ac2c: r[0]=32u;goto L8c61ac2e; /* mov #0x20,r0 */
L8c61ac2e: fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61ac30; /* fmov.s @(r0,r7),fr2 */
L8c61ac30: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ac32; /* fmac fr0,fr2,fr3 */
L8c61ac32: fcopy(f,2u,15u);goto L8c61ac34; /* fmov fr15,fr2 */
L8c61ac34: fcopy(f,0u,15u);goto L8c61ac36; /* fmov fr15,fr0 */
L8c61ac36: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ac38; /* fmac fr0,fr3,fr2 */
L8c61ac38: r[0]=24u;goto L8c61ac3a; /* mov #0x18,r0 */
L8c61ac3a: r[3]=rd(m,bus,&io_status,0x8c61ae24u,4);if(io_status)return io_status;goto L8c61ac3c; /* mov.l 0x8c61ae24,r3 */
L8c61ac3c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ac3e; /* fmov.s fr2,@-r8 */
L8c61ac3e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac40; /* fmov.s @(r0,r4),fr3 */
L8c61ac40: r[0]=20u;goto L8c61ac42; /* mov #0x14,r0 */
L8c61ac42: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ac44; /* fmov.s fr3,@-r8 */
L8c61ac44: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac46; /* fmov.s @(r0,r4),fr3 */
L8c61ac46: r[0]=16u;goto L8c61ac48; /* mov #0x10,r0 */
L8c61ac48: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ac4a; /* fmov.s fr3,@-r8 */
L8c61ac4a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac4c; /* fmov.s @(r0,r4),fr3 */
L8c61ac4c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ac4e; /* fmov.s fr3,@-r8 */
L8c61ac4e: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61ac50; /* mov.l r3,@-r8 */
L8c61ac50: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61ac52; /* pref @r8 */
L8c61ac52: r[5]+=1u;goto L8c61ac54; /* add #0x1,r5 */
L8c61ac54: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61ac56; /* cmp/ge r6,r5 */
L8c61ac56: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61abb4;goto L8c61ac5a; /* bf/s 0x8c61abb4 */
L8c61ac5a: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61ac5c; /* mov.w @r11+,r4 */
L8c61ac5c: r[0]=4u;goto L8c61ac5e; /* mov #0x4,r0 */
L8c61ac5e: r[2]=rd(m,bus,&io_status,0x8c61ae0cu,4);if(io_status)return io_status;goto L8c61ac60; /* mov.l 0x8c61ae0c,r2 */
L8c61ac60: r[8]+=28u;goto L8c61ac62; /* add #0x1c,r8 */
L8c61ac62: r[4]=r[4]&65535u;goto L8c61ac64; /* extu.w r4,r4 */
L8c61ac64: r[1]=rd(m,bus,&io_status,0x8c61ae18u,4);if(io_status)return io_status;goto L8c61ac66; /* mov.l 0x8c61ae18,r1 */
L8c61ac66: r[4]<<=2;goto L8c61ac68; /* shll2 r4 */
L8c61ac68: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ac6a; /* mov.l @r2,r3 */
L8c61ac6a: r[4]<<=2;goto L8c61ac6c; /* shll2 r4 */
L8c61ac6c: r[2]=rd(m,bus,&io_status,0x8c61ae1cu,4);if(io_status)return io_status;goto L8c61ac6e; /* mov.l 0x8c61ae1c,r2 */
L8c61ac6e: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61ac70; /* shll r4 */
L8c61ac70: r[4]+=r[3];goto L8c61ac72; /* add r3,r4 */
L8c61ac72: r[3]=rd(m,bus,&io_status,0x8c61ae14u,4);if(io_status)return io_status;goto L8c61ac74; /* mov.l 0x8c61ae14,r3 */
L8c61ac74: fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61ac76; /* fmov.s @r4,fr3 */
L8c61ac76: fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61ac78; /* fmov.s @r3,fr2 */
L8c61ac78: r[3]+=8u;goto L8c61ac7a; /* add #0x8,r3 */
L8c61ac7a: fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61ac7c; /* fmov.s @r2,fr1 */
L8c61ac7c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ac7e; /* fmul fr3,fr2 */
L8c61ac7e: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61ac80; /* fmov.s @r1,fr3 */
L8c61ac80: r[2]=rd(m,bus,&io_status,0x8c61ae20u,4);if(io_status)return io_status;goto L8c61ac82; /* mov.l 0x8c61ae20,r2 */
L8c61ac82: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ac84; /* fsub fr2,fr3 */
L8c61ac84: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac86; /* fmov.s @(r0,r4),fr2 */
L8c61ac86: r[0]=8u;goto L8c61ac88; /* mov #0x8,r0 */
L8c61ac88: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ac8a; /* fmul fr2,fr1 */
L8c61ac8a: fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac8c; /* fmov.s @(r0,r4),fr2 */
L8c61ac8c: r[0]=4u;goto L8c61ac8e; /* mov #0x4,r0 */
L8c61ac8e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ac90; /* fsub fr1,fr3 */
L8c61ac90: fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61ac92; /* fmov.s @r3,fr1 */
L8c61ac92: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ac94; /* fmul fr2,fr1 */
L8c61ac94: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ac96; /* fsub fr1,fr3 */
L8c61ac96: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ac98; /* fmov.s fr3,@-r8 */
L8c61ac98: r[6]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ac9a; /* mov.l @r2,r6 */
L8c61ac9a: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac9c; /* fmov.s @(r0,r4),fr0 */
L8c61ac9c: r[0]=20u;goto L8c61ac9e; /* mov #0x14,r0 */
L8c61ac9e: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61aca0; /* fmov.s @(r0,r6),fr3 */
L8c61aca0: r[0]=4u;goto L8c61aca2; /* mov #0x4,r0 */
L8c61aca2: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61aca4; /* fmov.s @(r0,r6),fr2 */
L8c61aca4: r[0]=8u;goto L8c61aca6; /* mov #0x8,r0 */
L8c61aca6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61aca8; /* fmul fr0,fr3 */
L8c61aca8: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61acaa; /* fmov.s @r4,fr0 */
L8c61acaa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61acac; /* fmac fr0,fr2,fr3 */
L8c61acac: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61acae; /* fmov.s @(r0,r4),fr0 */
L8c61acae: r[0]=36u;goto L8c61acb0; /* mov #0x24,r0 */
L8c61acb0: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61acb2; /* fmov.s @(r0,r6),fr2 */
L8c61acb2: r[0]=4u;goto L8c61acb4; /* mov #0x4,r0 */
L8c61acb4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61acb6; /* fmac fr0,fr2,fr3 */
L8c61acb6: fcopy(f,0u,15u);goto L8c61acb8; /* fmov fr15,fr0 */
L8c61acb8: fcopy(f,2u,15u);goto L8c61acba; /* fmov fr15,fr2 */
L8c61acba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61acbc; /* fmac fr0,fr3,fr2 */
L8c61acbc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61acbe; /* fmov.s fr2,@-r8 */
L8c61acbe: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61acc0; /* fmov.s @(r0,r4),fr0 */
L8c61acc0: r[0]=16u;goto L8c61acc2; /* mov #0x10,r0 */
L8c61acc2: r[5]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61acc4; /* mov.l @r2,r5 */
L8c61acc4: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61acc6; /* fmov.s @(r0,r5),fr3 */
L8c61acc6: r[0]=8u;goto L8c61acc8; /* mov #0x8,r0 */
L8c61acc8: fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c61acca; /* fmov.s @r5,fr2 */
L8c61acca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61accc; /* fmul fr0,fr3 */
L8c61accc: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61acce; /* fmov.s @r4,fr0 */
L8c61acce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61acd0; /* fmac fr0,fr2,fr3 */
L8c61acd0: fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61acd2; /* fmov.s @(r0,r4),fr0 */
L8c61acd2: r[0]=32u;goto L8c61acd4; /* mov #0x20,r0 */
L8c61acd4: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61acd6; /* fmov.s @(r0,r5),fr2 */
L8c61acd6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61acd8; /* fmac fr0,fr2,fr3 */
L8c61acd8: fcopy(f,2u,15u);goto L8c61acda; /* fmov fr15,fr2 */
L8c61acda: fcopy(f,0u,15u);goto L8c61acdc; /* fmov fr15,fr0 */
L8c61acdc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61acde; /* fmac fr0,fr3,fr2 */
L8c61acde: r[0]=24u;goto L8c61ace0; /* mov #0x18,r0 */
L8c61ace0: r[3]=rd(m,bus,&io_status,0x8c61ae28u,4);if(io_status)return io_status;goto L8c61ace2; /* mov.l 0x8c61ae28,r3 */
L8c61ace2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ace4; /* fmov.s fr2,@-r8 */
L8c61ace4: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ace6; /* fmov.s @(r0,r4),fr3 */
L8c61ace6: r[0]=20u;goto L8c61ace8; /* mov #0x14,r0 */
L8c61ace8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61acea; /* fmov.s fr3,@-r8 */
L8c61acea: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61acec; /* fmov.s @(r0,r4),fr3 */
L8c61acec: r[0]=16u;goto L8c61acee; /* mov #0x10,r0 */
L8c61acee: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61acf0; /* fmov.s fr3,@-r8 */
L8c61acf0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61acf2; /* fmov.s @(r0,r4),fr3 */
L8c61acf2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61acf4; /* fmov.s fr3,@-r8 */
L8c61acf4: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61acf6; /* mov.l r3,@-r8 */
L8c61acf6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61acf8; /* pref @r8 */
L8c61acf8: r[8]+=32u;goto L8c61ad5a; /* bra 0x8c61ad5a */
L8c61acfc: r[2]=rd(m,bus,&io_status,0x8c61ae2cu,4);if(io_status)return io_status;goto L8c61acfe; /* mov.l 0x8c61ae2c,r2 */
L8c61acfe: r[3]=0u;goto L8c61ad00; /* mov #0x0,r3 */
L8c61ad00: r[0]=rd(m,bus,&io_status,0x8c61ae0cu,4);if(io_status)return io_status;goto L8c61ad02; /* mov.l 0x8c61ae0c,r0 */
L8c61ad02: r[10]=r[13];goto L8c61ad04; /* mov r13,r10 */
L8c61ad04: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61ad06; /* mov.l r3,@r2 */
L8c61ad06: r[10]+=4294967294u;goto L8c61ad08; /* add #-0x2,r10 */
L8c61ad08: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61ad0a; /* mov.w @r11+,r4 */
L8c61ad0a: r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61ad0c; /* mov.l @r0,r3 */
L8c61ad0c: r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61ad0e; /* mov.w @r11+,r14 */
L8c61ad0e: r[4]=r[4]&65535u;goto L8c61ad10; /* extu.w r4,r4 */
L8c61ad10: r[4]<<=2;goto L8c61ad12; /* shll2 r4 */
L8c61ad12: r[14]=r[14]&65535u;goto L8c61ad14; /* extu.w r14,r14 */
L8c61ad14: r[14]<<=2;goto L8c61ad16; /* shll2 r14 */
L8c61ad16: r[14]<<=2;goto L8c61ad18; /* shll2 r14 */
L8c61ad18: r[4]<<=2;goto L8c61ad1a; /* shll2 r4 */
L8c61ad1a: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61ad1c; /* shll r4 */
L8c61ad1c: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61ad1e; /* shll r14 */
L8c61ad1e: *sr=(*sr&~1u)|((int32_t)r[10]>0);goto L8c61ad20; /* cmp/pl r10 */
L8c61ad20: r[4]+=r[3];goto L8c61ad22; /* add r3,r4 */
L8c61ad22: r[14]+=r[3];goto L8c61ad24; /* add r3,r14 */
L8c61ad24: take=!(*sr&1u);r[12]=0u;if(take)goto L8c61ad5a;goto L8c61ad28; /* bf/s 0x8c61ad5a */
L8c61ad28: r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61ad2a; /* mov.w @r11+,r13 */
L8c61ad2a: r[2]=rd(m,bus,&io_status,0x8c61ae0cu,4);if(io_status)return io_status;goto L8c61ad2c; /* mov.l 0x8c61ae0c,r2 */
L8c61ad2c: r[13]=r[13]&65535u;goto L8c61ad2e; /* extu.w r13,r13 */
L8c61ad2e: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61ad30; /* mov.l @(0xc,r4),r5 */
L8c61ad30: r[13]<<=2;goto L8c61ad32; /* shll2 r13 */
L8c61ad32: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ad34; /* mov.l @r2,r3 */
L8c61ad34: r[13]<<=2;goto L8c61ad36; /* shll2 r13 */
L8c61ad36: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61ad38; /* shll r13 */
L8c61ad38: r[13]+=r[3];goto L8c61ad3a; /* add r3,r13 */
L8c61ad3a: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61ad3c; /* mov.l @(0xc,r14),r3 */
L8c61ad3c: r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61ad3e; /* mov.l @(0xc,r13),r1 */
L8c61ad3e: r[5]&=r[3];goto L8c61ad40; /* and r3,r5 */
L8c61ad40: r[3]=1u;goto L8c61ad42; /* mov #0x1,r3 */
L8c61ad42: r[5]&=r[1];goto L8c61ad44; /* and r1,r5 */
L8c61ad44: *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61ad46; /* tst r3,r5 */
L8c61ad46: if(!(*sr&1u))goto L8c61ad50;goto L8c61ad48; /* bf 0x8c61ad50 */
L8c61ad48: r[1]=rd(m,bus,&io_status,0x8c61ae30u,4);if(io_status)return io_status;goto L8c61ad4a; /* mov.l 0x8c61ae30,r1 */
L8c61ad4a: r[6]=r[13];goto L8c61ad4c; /* mov r13,r6 */
L8c61ad4c: target=r[1];*pr=0x8c61ad50u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ad50; /* jsr @r1 */
L8c61ad50: r[12]+=1u;goto L8c61ad52; /* add #0x1,r12 */
L8c61ad52: r[4]=r[14];goto L8c61ad54; /* mov r14,r4 */
L8c61ad54: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[10]);goto L8c61ad56; /* cmp/ge r10,r12 */
L8c61ad56: take=!(*sr&1u);r[14]=r[13];if(take)goto L8c61ad28;goto L8c61ad5a; /* bf/s 0x8c61ad28 */
L8c61ad5a: r[9]+=1u;goto L8c61ad5c; /* add #0x1,r9 */
L8c61ad5c: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ad5e; /* mov.l @r15,r2 */
L8c61ad5e: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61ad60; /* mov.w @(0x2,r2),r0 */
L8c61ad60: r[0]=r[0]&65535u;goto L8c61ad62; /* extu.w r0,r0 */
L8c61ad62: *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61ad64; /* cmp/ge r0,r9 */
L8c61ad64: if((*sr&1u))goto L8c61ad6a;goto L8c61ad66; /* bt 0x8c61ad6a */
L8c61ad66: goto L8c61ab00; /* bra 0x8c61ab00 */
L8c61ad6a: goto L8c61ade8; /* bra 0x8c61ade8 */
L8c61ad6e: r[2]=rd(m,bus,&io_status,0x8c61ae34u,4);if(io_status)return io_status;goto L8c61ad70; /* mov.l 0x8c61ae34,r2 */
L8c61ad70: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ad72; /* mov.l @r15,r1 */
L8c61ad72: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61ad74; /* mov.l r1,@r2 */
L8c61ad74: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61ad76; /* frchg */
L8c61ad76: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61ad78; /* fmov.s fr0,@-r15 */
L8c61ad78: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61ad7a; /* fmov.s fr1,@-r15 */
L8c61ad7a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61ad7c; /* fmov.s fr2,@-r15 */
L8c61ad7c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61ad7e; /* fmov.s fr3,@-r15 */
L8c61ad7e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61ad80; /* fmov.s fr4,@-r15 */
L8c61ad80: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61ad82; /* fmov.s fr5,@-r15 */
L8c61ad82: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61ad84; /* fmov.s fr6,@-r15 */
L8c61ad84: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61ad86; /* fmov.s fr7,@-r15 */
L8c61ad86: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61ad88; /* fmov.s fr8,@-r15 */
L8c61ad88: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61ad8a; /* fmov.s fr9,@-r15 */
L8c61ad8a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c61ad8c; /* fmov.s fr10,@-r15 */
L8c61ad8c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c61ad8e; /* fmov.s fr11,@-r15 */
L8c61ad8e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c61ad90; /* fmov.s fr12,@-r15 */
L8c61ad90: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61ad92; /* fmov.s fr13,@-r15 */
L8c61ad92: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61ad94; /* fmov.s fr14,@-r15 */
L8c61ad94: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61ad96; /* fmov.s fr15,@-r15 */
L8c61ad96: r[0]=0x8c61adfcu;goto L8c61ad98; /* mova 0x8c61adfc,r0 */
L8c61ad98: fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c61ad9a; /* fmov.s @r0,fr12 */
L8c61ad9a: fcopy(f,13u,12u);goto L8c61ad9c; /* fmov fr12,fr13 */
L8c61ad9c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[12],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61ad9e; /* fmul fr12,fr0 */
L8c61ad9e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61ada0; /* fmul fr12,fr4 */
L8c61ada0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[12],&f->fpscr,&fr[8]);if(status)return status;}goto L8c61ada2; /* fmul fr12,fr8 */
L8c61ada2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[12],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ada4; /* fmul fr12,fr1 */
L8c61ada4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[12],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61ada6; /* fmul fr12,fr5 */
L8c61ada6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[12],&f->fpscr,&fr[9]);if(status)return status;}goto L8c61ada8; /* fmul fr12,fr9 */
L8c61ada8: r[0]=rd(m,bus,&io_status,0x8c61ae00u,4);if(io_status)return io_status;goto L8c61adaa; /* mov.l 0x8c61ae00,r0 */
L8c61adaa: fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adac; /* fmov.s @r0+,fr2 */
L8c61adac: fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adae; /* fmov.s @r0+,fr6 */
L8c61adae: fload(m,bus,&io_status,f,10u,r[0]);if(io_status)return io_status;goto L8c61adb0; /* fmov.s @r0,fr10 */
L8c61adb0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c61adb2; /* fneg fr2 */
L8c61adb2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]^=0x80000000u;goto L8c61adb4; /* fneg fr6 */
L8c61adb4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]^=0x80000000u;goto L8c61adb6; /* fneg fr10 */
L8c61adb6: r[0]=rd(m,bus,&io_status,0x8c61ae04u,4);if(io_status)return io_status;goto L8c61adb8; /* mov.l 0x8c61ae04,r0 */
L8c61adb8: fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c61adba; /* fmov.s @r0,fr14 */
L8c61adba: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61adbc; /* frchg */
L8c61adbc: r[2]=rd(m,bus,&io_status,0x8c61ae38u,4);if(io_status)return io_status;goto L8c61adbe; /* mov.l 0x8c61ae38,r2 */
L8c61adbe: r[3]=rd(m,bus,&io_status,0x8c61ae34u,4);if(io_status)return io_status;goto L8c61adc0; /* mov.l 0x8c61ae34,r3 */
L8c61adc0: target=r[2];*pr=0x8c61adc4u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61adc4; /* jsr @r2 */
L8c61adc4: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61adc6; /* frchg */
L8c61adc6: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adc8; /* fmov.s @r15+,fr15 */
L8c61adc8: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adca; /* fmov.s @r15+,fr14 */
L8c61adca: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adcc; /* fmov.s @r15+,fr13 */
L8c61adcc: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adce; /* fmov.s @r15+,fr12 */
L8c61adce: fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61add0; /* fmov.s @r15+,fr11 */
L8c61add0: fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61add2; /* fmov.s @r15+,fr10 */
L8c61add2: fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61add4; /* fmov.s @r15+,fr9 */
L8c61add4: fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61add6; /* fmov.s @r15+,fr8 */
L8c61add6: fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61add8; /* fmov.s @r15+,fr7 */
L8c61add8: fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adda; /* fmov.s @r15+,fr6 */
L8c61adda: fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61addc; /* fmov.s @r15+,fr5 */
L8c61addc: fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adde; /* fmov.s @r15+,fr4 */
L8c61adde: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ade0; /* fmov.s @r15+,fr3 */
L8c61ade0: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ade2; /* fmov.s @r15+,fr2 */
L8c61ade2: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ade4; /* fmov.s @r15+,fr1 */
L8c61ade4: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ade6; /* fmov.s @r15+,fr0 */
L8c61ade6: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61ade8; /* frchg */
L8c61ade8: r[15]+=8u;goto L8c61adea; /* add #0x8,r15 */
L8c61adea: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61adec; /* lds.l @r15+,PR */
L8c61adec: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adee; /* fmov.s @r15+,fr15 */
L8c61adee: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61adf0; /* mov.l @r15+,r9 */
L8c61adf0: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61adf2; /* mov.l @r15+,r10 */
L8c61adf2: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61adf4; /* mov.l @r15+,r11 */
L8c61adf4: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61adf6; /* mov.l @r15+,r12 */
L8c61adf6: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61adf8; /* mov.l @r15+,r13 */
L8c61adf8: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
