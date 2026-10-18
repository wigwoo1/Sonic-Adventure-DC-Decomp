#include "sa_draw_motion_stop.h"
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
L8c618d18: sa_motion_publish_stop(ctx,0x8c618d18u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c618d1a; /* mov.l r14,@-r15 */
L8c618d1a: sa_motion_publish_stop(ctx,0x8c618d1au); r[0]=0x8c618dd4u;goto L8c618d1c; /* mova 0x8c618dd4,r0 */
L8c618d1c: sa_motion_publish_stop(ctx,0x8c618d1cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c618d1e; /* mov.l r13,@-r15 */
L8c618d1e: sa_motion_publish_stop(ctx,0x8c618d1eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c618d20; /* mov.l r12,@-r15 */
L8c618d20: sa_motion_publish_stop(ctx,0x8c618d20u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c618d22; /* mov.l r11,@-r15 */
L8c618d22: sa_motion_publish_stop(ctx,0x8c618d22u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c618d24; /* mov.l r10,@-r15 */
L8c618d24: sa_motion_publish_stop(ctx,0x8c618d24u); r[3]=rd(m,bus,&io_status,0x8c618ddcu,4);if(io_status)return io_status;goto L8c618d26; /* mov.l 0x8c618ddc,r3 */
L8c618d26: sa_motion_publish_stop(ctx,0x8c618d26u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c618d28; /* mov.l r9,@-r15 */
L8c618d28: sa_motion_publish_stop(ctx,0x8c618d28u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c618d2a; /* sts.l PR,@-r15 */
L8c618d2a: sa_motion_publish_stop(ctx,0x8c618d2au); fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c618d2c; /* fmov.s @r0,fr6 */
L8c618d2c: sa_motion_publish_stop(ctx,0x8c618d2cu); r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c618d2e; /* mov.w @r4,r0 */
L8c618d2e: sa_motion_publish_stop(ctx,0x8c618d2eu); r[13]=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;goto L8c618d30; /* mov.l @(0x14,r4),r13 */
L8c618d30: sa_motion_publish_stop(ctx,0x8c618d30u); r[0]=r[0]&65535u;goto L8c618d32; /* extu.w r0,r0 */
L8c618d32: sa_motion_publish_stop(ctx,0x8c618d32u); r[12]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c618d34; /* mov.l @(0x4,r4),r12 */
L8c618d34: sa_motion_publish_stop(ctx,0x8c618d34u); r[0]&=r[3];goto L8c618d36; /* and r3,r0 */
L8c618d36: sa_motion_publish_stop(ctx,0x8c618d36u); r[10]=rd(m,bus,&io_status,0x8c618dd8u,4);if(io_status)return io_status;goto L8c618d38; /* mov.l 0x8c618dd8,r10 */
L8c618d38: sa_motion_publish_stop(ctx,0x8c618d38u); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c618d3a; /* cmp/eq #0x0,r0 */
L8c618d3a: sa_motion_publish_stop(ctx,0x8c618d3cu); take=(*sr&1u);r[11]=0u;if(take)goto L8c618d60;goto L8c618d3e; /* bt/s 0x8c618d60 */
L8c618d3e: sa_motion_publish_stop(ctx,0x8c618d3eu); r[1]=sx(m,bus,&io_status,0x8c618dd2u,2);if(io_status)return io_status;goto L8c618d40; /* mov.w 0x8c618dd2,r1 */
L8c618d40: sa_motion_publish_stop(ctx,0x8c618d40u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c618d42; /* cmp/eq r1,r0 */
L8c618d42: sa_motion_publish_stop(ctx,0x8c618d42u); if(!(*sr&1u))goto L8c618d48;goto L8c618d44; /* bf 0x8c618d48 */
L8c618d44: sa_motion_publish_stop(ctx,0x8c618d46u); goto L8c618f0c; /* bra 0x8c618f0c */
L8c618d48: sa_motion_publish_stop(ctx,0x8c618d48u); r[1]=rd(m,bus,&io_status,0x8c618de0u,4);if(io_status)return io_status;goto L8c618d4a; /* mov.l 0x8c618de0,r1 */
L8c618d4a: sa_motion_publish_stop(ctx,0x8c618d4au); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c618d4c; /* cmp/eq r1,r0 */
L8c618d4c: sa_motion_publish_stop(ctx,0x8c618d4cu); if(!(*sr&1u))goto L8c618d52;goto L8c618d4e; /* bf 0x8c618d52 */
L8c618d4e: sa_motion_publish_stop(ctx,0x8c618d50u); goto L8c6190fe; /* bra 0x8c6190fe */
L8c618d52: sa_motion_publish_stop(ctx,0x8c618d52u); r[1]=rd(m,bus,&io_status,0x8c618ddcu,4);if(io_status)return io_status;goto L8c618d54; /* mov.l 0x8c618ddc,r1 */
L8c618d54: sa_motion_publish_stop(ctx,0x8c618d54u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c618d56; /* cmp/eq r1,r0 */
L8c618d56: sa_motion_publish_stop(ctx,0x8c618d56u); if(!(*sr&1u))goto L8c618d5c;goto L8c618d58; /* bf 0x8c618d5c */
L8c618d58: sa_motion_publish_stop(ctx,0x8c618d5au); goto L8c619294; /* bra 0x8c619294 */
L8c618d5c: sa_motion_publish_stop(ctx,0x8c618d5eu); goto L8c6192d0; /* bra 0x8c6192d0 */
L8c618d60: sa_motion_publish_stop(ctx,0x8c618d60u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c618d62; /* mov.w @(0x2,r4),r0 */
L8c618d62: sa_motion_publish_stop(ctx,0x8c618d62u); r[1]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c618d64; /* mov.l @r10,r1 */
L8c618d64: sa_motion_publish_stop(ctx,0x8c618d64u); r[0]=r[0]&65535u;goto L8c618d66; /* extu.w r0,r0 */
L8c618d66: sa_motion_publish_stop(ctx,0x8c618d66u); r[2]=r[0];goto L8c618d68; /* mov r0,r2 */
L8c618d68: sa_motion_publish_stop(ctx,0x8c618d68u); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c618d6a; /* shll r0 */
L8c618d6a: sa_motion_publish_stop(ctx,0x8c618d6au); r[0]+=r[2];goto L8c618d6c; /* add r2,r0 */
L8c618d6c: sa_motion_publish_stop(ctx,0x8c618d6cu); r[1]+=r[0];goto L8c618d6e; /* add r0,r1 */
L8c618d6e: sa_motion_publish_stop(ctx,0x8c618d6eu); wr(m,bus,&io_status,r[10],r[1],4);if(io_status)return io_status;goto L8c618d70; /* mov.l r1,@r10 */
L8c618d70: sa_motion_publish_stop(ctx,0x8c618d72u); r[14]=r[11];goto L8c618efc; /* bra 0x8c618efc */
L8c618d74: sa_motion_publish_stop(ctx,0x8c618d74u); r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c618d76; /* mov.w @r12+,r5 */
L8c618d76: sa_motion_publish_stop(ctx,0x8c618d76u); r[0]=16u;goto L8c618d78; /* mov #0x10,r0 */
L8c618d78: sa_motion_publish_stop(ctx,0x8c618d78u); r[3]=rd(m,bus,&io_status,0x8c618de4u,4);if(io_status)return io_status;goto L8c618d7a; /* mov.l 0x8c618de4,r3 */
L8c618d7a: sa_motion_publish_stop(ctx,0x8c618d7au); r[6]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c618d7c; /* mov.w @r12+,r6 */
L8c618d7c: sa_motion_publish_stop(ctx,0x8c618d7cu); r[5]=r[5]&65535u;goto L8c618d7e; /* extu.w r5,r5 */
L8c618d7e: sa_motion_publish_stop(ctx,0x8c618d7eu); r[11]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c618d80; /* mov.l @r3,r11 */
L8c618d80: sa_motion_publish_stop(ctx,0x8c618d80u); r[5]<<=2;goto L8c618d82; /* shll2 r5 */
L8c618d82: sa_motion_publish_stop(ctx,0x8c618d82u); r[6]=r[6]&65535u;goto L8c618d84; /* extu.w r6,r6 */
L8c618d84: sa_motion_publish_stop(ctx,0x8c618d84u); r[7]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c618d86; /* mov.w @r12+,r7 */
L8c618d86: sa_motion_publish_stop(ctx,0x8c618d86u); r[6]<<=2;goto L8c618d88; /* shll2 r6 */
L8c618d88: sa_motion_publish_stop(ctx,0x8c618d88u); r[6]<<=2;goto L8c618d8a; /* shll2 r6 */
L8c618d8a: sa_motion_publish_stop(ctx,0x8c618d8au); r[5]<<=2;goto L8c618d8c; /* shll2 r5 */
L8c618d8c: sa_motion_publish_stop(ctx,0x8c618d8cu); r[7]=r[7]&65535u;goto L8c618d8e; /* extu.w r7,r7 */
L8c618d8e: sa_motion_publish_stop(ctx,0x8c618d8eu); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c618d90; /* shll r5 */
L8c618d90: sa_motion_publish_stop(ctx,0x8c618d90u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c618d92; /* shll r6 */
L8c618d92: sa_motion_publish_stop(ctx,0x8c618d92u); r[6]+=r[11];goto L8c618d94; /* add r11,r6 */
L8c618d94: sa_motion_publish_stop(ctx,0x8c618d94u); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c618d96; /* fmov.s @(r0,r6),fr4 */
L8c618d96: sa_motion_publish_stop(ctx,0x8c618d96u); r[5]+=r[11];goto L8c618d98; /* add r11,r5 */
L8c618d98: sa_motion_publish_stop(ctx,0x8c618d98u); r[7]<<=2;goto L8c618d9a; /* shll2 r7 */
L8c618d9a: sa_motion_publish_stop(ctx,0x8c618d9au); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618d9c; /* fmov.s @(r0,r5),fr3 */
L8c618d9c: sa_motion_publish_stop(ctx,0x8c618d9cu); r[7]<<=2;goto L8c618d9e; /* shll2 r7 */
L8c618d9e: sa_motion_publish_stop(ctx,0x8c618d9eu); fcopy(f,2u,4u);goto L8c618da0; /* fmov fr4,fr2 */
L8c618da0: sa_motion_publish_stop(ctx,0x8c618da0u); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c618da2; /* shll r7 */
L8c618da2: sa_motion_publish_stop(ctx,0x8c618da2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c618da4; /* fsub fr3,fr2 */
L8c618da4: sa_motion_publish_stop(ctx,0x8c618da4u); r[0]=20u;goto L8c618da6; /* mov #0x14,r0 */
L8c618da6: sa_motion_publish_stop(ctx,0x8c618da6u); r[7]+=r[11];goto L8c618da8; /* add r11,r7 */
L8c618da8: sa_motion_publish_stop(ctx,0x8c618da8u); fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c618daa; /* fmov.s @(r0,r6),fr5 */
L8c618daa: sa_motion_publish_stop(ctx,0x8c618daau); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618dac; /* fmov.s @(r0,r7),fr3 */
L8c618dac: sa_motion_publish_stop(ctx,0x8c618dacu); fcopy(f,1u,5u);goto L8c618dae; /* fmov fr5,fr1 */
L8c618dae: sa_motion_publish_stop(ctx,0x8c618daeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618db0; /* fsub fr5,fr3 */
L8c618db0: sa_motion_publish_stop(ctx,0x8c618db0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618db2; /* fmul fr3,fr2 */
L8c618db2: sa_motion_publish_stop(ctx,0x8c618db2u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618db4; /* fmov.s @(r0,r5),fr3 */
L8c618db4: sa_motion_publish_stop(ctx,0x8c618db4u); r[0]=16u;goto L8c618db6; /* mov #0x10,r0 */
L8c618db6: sa_motion_publish_stop(ctx,0x8c618db6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c618db8; /* fsub fr3,fr1 */
L8c618db8: sa_motion_publish_stop(ctx,0x8c618db8u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618dba; /* fmov.s @(r0,r7),fr3 */
L8c618dba: sa_motion_publish_stop(ctx,0x8c618dbau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618dbc; /* fsub fr4,fr3 */
L8c618dbc: sa_motion_publish_stop(ctx,0x8c618dbcu); fcopy(f,4u,2u);goto L8c618dbe; /* fmov fr2,fr4 */
L8c618dbe: sa_motion_publish_stop(ctx,0x8c618dbeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618dc0; /* fmul fr3,fr1 */
L8c618dc0: sa_motion_publish_stop(ctx,0x8c618dc0u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c618dc2; /* fldi0 fr3 */
L8c618dc2: sa_motion_publish_stop(ctx,0x8c618dc2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c618dc4; /* fsub fr1,fr4 */
L8c618dc4: sa_motion_publish_stop(ctx,0x8c618dc4u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c618dc6; /* fcmp/gt fr3,fr4 */
L8c618dc6: sa_motion_publish_stop(ctx,0x8c618dc8u); take=!(*sr&1u);r[14]+=1u;if(take)goto L8c618de8;goto L8c618dca; /* bf/s 0x8c618de8 */
L8c618dca: sa_motion_publish_stop(ctx,0x8c618dccu); r[13]+=12u;goto L8c618efc; /* bra 0x8c618efc */
L8c618de8: sa_motion_publish_stop(ctx,0x8c618de8u); r[3]=rd(m,bus,&io_status,0x8c619108u,4);if(io_status)return io_status;goto L8c618dea; /* mov.l 0x8c619108,r3 */
L8c618dea: sa_motion_publish_stop(ctx,0x8c618deau); r[0]=4u;goto L8c618dec; /* mov #0x4,r0 */
L8c618dec: sa_motion_publish_stop(ctx,0x8c618decu); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c618dee; /* fmov.s @r5,fr3 */
L8c618dee: sa_motion_publish_stop(ctx,0x8c618deeu); r[8]+=28u;goto L8c618df0; /* add #0x1c,r8 */
L8c618df0: sa_motion_publish_stop(ctx,0x8c618df0u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618df2; /* fmov.s @r3,fr2 */
L8c618df2: sa_motion_publish_stop(ctx,0x8c618df2u); r[3]+=8u;goto L8c618df4; /* add #0x8,r3 */
L8c618df4: sa_motion_publish_stop(ctx,0x8c618df4u); r[2]=rd(m,bus,&io_status,0x8c61910cu,4);if(io_status)return io_status;goto L8c618df6; /* mov.l 0x8c61910c,r2 */
L8c618df6: sa_motion_publish_stop(ctx,0x8c618df6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618df8; /* fmul fr3,fr2 */
L8c618df8: sa_motion_publish_stop(ctx,0x8c618df8u); r[1]=rd(m,bus,&io_status,0x8c619110u,4);if(io_status)return io_status;goto L8c618dfa; /* mov.l 0x8c619110,r1 */
L8c618dfa: sa_motion_publish_stop(ctx,0x8c618dfau); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618dfc; /* fmov.s @r2,fr3 */
L8c618dfc: sa_motion_publish_stop(ctx,0x8c618dfcu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c618dfe; /* fmov.s @r1,fr1 */
L8c618dfe: sa_motion_publish_stop(ctx,0x8c618dfeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618e00; /* fsub fr2,fr3 */
L8c618e00: sa_motion_publish_stop(ctx,0x8c618e00u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c618e02; /* fmov.s @(r0,r5),fr2 */
L8c618e02: sa_motion_publish_stop(ctx,0x8c618e02u); r[0]=8u;goto L8c618e04; /* mov #0x8,r0 */
L8c618e04: sa_motion_publish_stop(ctx,0x8c618e04u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618e06; /* fmul fr2,fr1 */
L8c618e06: sa_motion_publish_stop(ctx,0x8c618e06u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c618e08; /* fmov.s @(r0,r5),fr2 */
L8c618e08: sa_motion_publish_stop(ctx,0x8c618e08u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618e0a; /* fsub fr1,fr3 */
L8c618e0a: sa_motion_publish_stop(ctx,0x8c618e0au); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618e0c; /* fmov.s @r3,fr1 */
L8c618e0c: sa_motion_publish_stop(ctx,0x8c618e0cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618e0e; /* fmul fr2,fr1 */
L8c618e0e: sa_motion_publish_stop(ctx,0x8c618e0eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618e10; /* fsub fr1,fr3 */
L8c618e10: sa_motion_publish_stop(ctx,0x8c618e10u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e12; /* fmov.s fr3,@-r8 */
L8c618e12: sa_motion_publish_stop(ctx,0x8c618e12u); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c618e14; /* mov.w @(0x2,r13),r0 */
L8c618e14: sa_motion_publish_stop(ctx,0x8c618e14u); r[2]=r[0];goto L8c618e16; /* mov r0,r2 */
L8c618e16: sa_motion_publish_stop(ctx,0x8c618e16u); *fpul=r[2];goto L8c618e18; /* lds r2,FPUL */
L8c618e18: sa_motion_publish_stop(ctx,0x8c618e18u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618e1a; /* float FPUL,fr3 */
L8c618e1a: sa_motion_publish_stop(ctx,0x8c618e1au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618e1c; /* fmul fr6,fr3 */
L8c618e1c: sa_motion_publish_stop(ctx,0x8c618e1cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e1e; /* fmov.s fr3,@-r8 */
L8c618e1e: sa_motion_publish_stop(ctx,0x8c618e1eu); r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c618e20; /* mov.w @r13,r2 */
L8c618e20: sa_motion_publish_stop(ctx,0x8c618e20u); *fpul=r[2];goto L8c618e22; /* lds r2,FPUL */
L8c618e22: sa_motion_publish_stop(ctx,0x8c618e22u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618e24; /* float FPUL,fr3 */
L8c618e24: sa_motion_publish_stop(ctx,0x8c618e24u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618e26; /* fmul fr6,fr3 */
L8c618e26: sa_motion_publish_stop(ctx,0x8c618e26u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e28; /* fmov.s fr3,@-r8 */
L8c618e28: sa_motion_publish_stop(ctx,0x8c618e28u); r[0]=24u;goto L8c618e2a; /* mov #0x18,r0 */
L8c618e2a: sa_motion_publish_stop(ctx,0x8c618e2au); r[2]=rd(m,bus,&io_status,0x8c619114u,4);if(io_status)return io_status;goto L8c618e2c; /* mov.l 0x8c619114,r2 */
L8c618e2c: sa_motion_publish_stop(ctx,0x8c618e2cu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618e2e; /* fmov.s @(r0,r5),fr3 */
L8c618e2e: sa_motion_publish_stop(ctx,0x8c618e2eu); r[0]=20u;goto L8c618e30; /* mov #0x14,r0 */
L8c618e30: sa_motion_publish_stop(ctx,0x8c618e30u); r[13]+=4u;goto L8c618e32; /* add #0x4,r13 */
L8c618e32: sa_motion_publish_stop(ctx,0x8c618e32u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e34; /* fmov.s fr3,@-r8 */
L8c618e34: sa_motion_publish_stop(ctx,0x8c618e34u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618e36; /* fmov.s @(r0,r5),fr3 */
L8c618e36: sa_motion_publish_stop(ctx,0x8c618e36u); r[0]=16u;goto L8c618e38; /* mov #0x10,r0 */
L8c618e38: sa_motion_publish_stop(ctx,0x8c618e38u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e3a; /* fmov.s fr3,@-r8 */
L8c618e3a: sa_motion_publish_stop(ctx,0x8c618e3au); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618e3c; /* fmov.s @(r0,r5),fr3 */
L8c618e3c: sa_motion_publish_stop(ctx,0x8c618e3cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e3e; /* fmov.s fr3,@-r8 */
L8c618e3e: sa_motion_publish_stop(ctx,0x8c618e3eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c618e40; /* mov.l r2,@-r8 */
L8c618e40: sa_motion_publish_stop(ctx,0x8c618e40u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618e42; /* pref @r8 */
L8c618e42: sa_motion_publish_stop(ctx,0x8c618e42u); r[2]=rd(m,bus,&io_status,0x8c619108u,4);if(io_status)return io_status;goto L8c618e44; /* mov.l 0x8c619108,r2 */
L8c618e44: sa_motion_publish_stop(ctx,0x8c618e44u); r[0]=4u;goto L8c618e46; /* mov #0x4,r0 */
L8c618e46: sa_motion_publish_stop(ctx,0x8c618e46u); fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c618e48; /* fmov.s @r6,fr3 */
L8c618e48: sa_motion_publish_stop(ctx,0x8c618e48u); r[8]+=60u;goto L8c618e4a; /* add #0x3c,r8 */
L8c618e4a: sa_motion_publish_stop(ctx,0x8c618e4au); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618e4c; /* fmov.s @r2,fr2 */
L8c618e4c: sa_motion_publish_stop(ctx,0x8c618e4cu); r[3]=rd(m,bus,&io_status,0x8c61910cu,4);if(io_status)return io_status;goto L8c618e4e; /* mov.l 0x8c61910c,r3 */
L8c618e4e: sa_motion_publish_stop(ctx,0x8c618e4eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618e50; /* fmul fr3,fr2 */
L8c618e50: sa_motion_publish_stop(ctx,0x8c618e50u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c618e52; /* fmov.s @r1,fr1 */
L8c618e52: sa_motion_publish_stop(ctx,0x8c618e52u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618e54; /* fmov.s @r3,fr3 */
L8c618e54: sa_motion_publish_stop(ctx,0x8c618e54u); r[3]=rd(m,bus,&io_status,0x8c619118u,4);if(io_status)return io_status;goto L8c618e56; /* mov.l 0x8c619118,r3 */
L8c618e56: sa_motion_publish_stop(ctx,0x8c618e56u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618e58; /* fsub fr2,fr3 */
L8c618e58: sa_motion_publish_stop(ctx,0x8c618e58u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c618e5a; /* fmov.s @(r0,r6),fr2 */
L8c618e5a: sa_motion_publish_stop(ctx,0x8c618e5au); r[0]=8u;goto L8c618e5c; /* mov #0x8,r0 */
L8c618e5c: sa_motion_publish_stop(ctx,0x8c618e5cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618e5e; /* fmul fr2,fr1 */
L8c618e5e: sa_motion_publish_stop(ctx,0x8c618e5eu); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c618e60; /* fmov.s @(r0,r6),fr2 */
L8c618e60: sa_motion_publish_stop(ctx,0x8c618e60u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618e62; /* fsub fr1,fr3 */
L8c618e62: sa_motion_publish_stop(ctx,0x8c618e62u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618e64; /* fmov.s @r3,fr1 */
L8c618e64: sa_motion_publish_stop(ctx,0x8c618e64u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618e66; /* fmul fr2,fr1 */
L8c618e66: sa_motion_publish_stop(ctx,0x8c618e66u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618e68; /* fsub fr1,fr3 */
L8c618e68: sa_motion_publish_stop(ctx,0x8c618e68u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e6a; /* fmov.s fr3,@-r8 */
L8c618e6a: sa_motion_publish_stop(ctx,0x8c618e6au); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c618e6c; /* mov.w @(0x2,r13),r0 */
L8c618e6c: sa_motion_publish_stop(ctx,0x8c618e6cu); r[2]=r[0];goto L8c618e6e; /* mov r0,r2 */
L8c618e6e: sa_motion_publish_stop(ctx,0x8c618e6eu); *fpul=r[2];goto L8c618e70; /* lds r2,FPUL */
L8c618e70: sa_motion_publish_stop(ctx,0x8c618e70u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618e72; /* float FPUL,fr3 */
L8c618e72: sa_motion_publish_stop(ctx,0x8c618e72u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618e74; /* fmul fr6,fr3 */
L8c618e74: sa_motion_publish_stop(ctx,0x8c618e74u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e76; /* fmov.s fr3,@-r8 */
L8c618e76: sa_motion_publish_stop(ctx,0x8c618e76u); r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c618e78; /* mov.w @r13,r2 */
L8c618e78: sa_motion_publish_stop(ctx,0x8c618e78u); r[13]+=4u;goto L8c618e7a; /* add #0x4,r13 */
L8c618e7a: sa_motion_publish_stop(ctx,0x8c618e7au); *fpul=r[2];goto L8c618e7c; /* lds r2,FPUL */
L8c618e7c: sa_motion_publish_stop(ctx,0x8c618e7cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618e7e; /* float FPUL,fr3 */
L8c618e7e: sa_motion_publish_stop(ctx,0x8c618e7eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618e80; /* fmul fr6,fr3 */
L8c618e80: sa_motion_publish_stop(ctx,0x8c618e80u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e82; /* fmov.s fr3,@-r8 */
L8c618e82: sa_motion_publish_stop(ctx,0x8c618e82u); r[0]=24u;goto L8c618e84; /* mov #0x18,r0 */
L8c618e84: sa_motion_publish_stop(ctx,0x8c618e84u); r[2]=rd(m,bus,&io_status,0x8c619114u,4);if(io_status)return io_status;goto L8c618e86; /* mov.l 0x8c619114,r2 */
L8c618e86: sa_motion_publish_stop(ctx,0x8c618e86u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618e88; /* fmov.s @(r0,r6),fr3 */
L8c618e88: sa_motion_publish_stop(ctx,0x8c618e88u); r[0]=20u;goto L8c618e8a; /* mov #0x14,r0 */
L8c618e8a: sa_motion_publish_stop(ctx,0x8c618e8au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e8c; /* fmov.s fr3,@-r8 */
L8c618e8c: sa_motion_publish_stop(ctx,0x8c618e8cu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618e8e; /* fmov.s @(r0,r6),fr3 */
L8c618e8e: sa_motion_publish_stop(ctx,0x8c618e8eu); r[0]=16u;goto L8c618e90; /* mov #0x10,r0 */
L8c618e90: sa_motion_publish_stop(ctx,0x8c618e90u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e92; /* fmov.s fr3,@-r8 */
L8c618e92: sa_motion_publish_stop(ctx,0x8c618e92u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618e94; /* fmov.s @(r0,r6),fr3 */
L8c618e94: sa_motion_publish_stop(ctx,0x8c618e94u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618e96; /* fmov.s fr3,@-r8 */
L8c618e96: sa_motion_publish_stop(ctx,0x8c618e96u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c618e98; /* mov.l r2,@-r8 */
L8c618e98: sa_motion_publish_stop(ctx,0x8c618e98u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618e9a; /* pref @r8 */
L8c618e9a: sa_motion_publish_stop(ctx,0x8c618e9au); r[2]=rd(m,bus,&io_status,0x8c619108u,4);if(io_status)return io_status;goto L8c618e9c; /* mov.l 0x8c619108,r2 */
L8c618e9c: sa_motion_publish_stop(ctx,0x8c618e9cu); r[0]=4u;goto L8c618e9e; /* mov #0x4,r0 */
L8c618e9e: sa_motion_publish_stop(ctx,0x8c618e9eu); fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c618ea0; /* fmov.s @r7,fr3 */
L8c618ea0: sa_motion_publish_stop(ctx,0x8c618ea0u); r[8]+=60u;goto L8c618ea2; /* add #0x3c,r8 */
L8c618ea2: sa_motion_publish_stop(ctx,0x8c618ea2u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618ea4; /* fmov.s @r2,fr2 */
L8c618ea4: sa_motion_publish_stop(ctx,0x8c618ea4u); r[3]=rd(m,bus,&io_status,0x8c61910cu,4);if(io_status)return io_status;goto L8c618ea6; /* mov.l 0x8c61910c,r3 */
L8c618ea6: sa_motion_publish_stop(ctx,0x8c618ea6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618ea8; /* fmul fr3,fr2 */
L8c618ea8: sa_motion_publish_stop(ctx,0x8c618ea8u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c618eaa; /* fmov.s @r1,fr1 */
L8c618eaa: sa_motion_publish_stop(ctx,0x8c618eaau); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618eac; /* fmov.s @r3,fr3 */
L8c618eac: sa_motion_publish_stop(ctx,0x8c618eacu); r[3]=rd(m,bus,&io_status,0x8c619118u,4);if(io_status)return io_status;goto L8c618eae; /* mov.l 0x8c619118,r3 */
L8c618eae: sa_motion_publish_stop(ctx,0x8c618eaeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618eb0; /* fsub fr2,fr3 */
L8c618eb0: sa_motion_publish_stop(ctx,0x8c618eb0u); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c618eb2; /* fmov.s @(r0,r7),fr2 */
L8c618eb2: sa_motion_publish_stop(ctx,0x8c618eb2u); r[0]=8u;goto L8c618eb4; /* mov #0x8,r0 */
L8c618eb4: sa_motion_publish_stop(ctx,0x8c618eb4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618eb6; /* fmul fr2,fr1 */
L8c618eb6: sa_motion_publish_stop(ctx,0x8c618eb6u); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c618eb8; /* fmov.s @(r0,r7),fr2 */
L8c618eb8: sa_motion_publish_stop(ctx,0x8c618eb8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618eba; /* fsub fr1,fr3 */
L8c618eba: sa_motion_publish_stop(ctx,0x8c618ebau); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618ebc; /* fmov.s @r3,fr1 */
L8c618ebc: sa_motion_publish_stop(ctx,0x8c618ebcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618ebe; /* fmul fr2,fr1 */
L8c618ebe: sa_motion_publish_stop(ctx,0x8c618ebeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618ec0; /* fsub fr1,fr3 */
L8c618ec0: sa_motion_publish_stop(ctx,0x8c618ec0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618ec2; /* fmov.s fr3,@-r8 */
L8c618ec2: sa_motion_publish_stop(ctx,0x8c618ec2u); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c618ec4; /* mov.w @(0x2,r13),r0 */
L8c618ec4: sa_motion_publish_stop(ctx,0x8c618ec4u); r[2]=r[0];goto L8c618ec6; /* mov r0,r2 */
L8c618ec6: sa_motion_publish_stop(ctx,0x8c618ec6u); *fpul=r[2];goto L8c618ec8; /* lds r2,FPUL */
L8c618ec8: sa_motion_publish_stop(ctx,0x8c618ec8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618eca; /* float FPUL,fr3 */
L8c618eca: sa_motion_publish_stop(ctx,0x8c618ecau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618ecc; /* fmul fr6,fr3 */
L8c618ecc: sa_motion_publish_stop(ctx,0x8c618eccu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618ece; /* fmov.s fr3,@-r8 */
L8c618ece: sa_motion_publish_stop(ctx,0x8c618eceu); r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c618ed0; /* mov.w @r13,r2 */
L8c618ed0: sa_motion_publish_stop(ctx,0x8c618ed0u); r[13]+=4u;goto L8c618ed2; /* add #0x4,r13 */
L8c618ed2: sa_motion_publish_stop(ctx,0x8c618ed2u); *fpul=r[2];goto L8c618ed4; /* lds r2,FPUL */
L8c618ed4: sa_motion_publish_stop(ctx,0x8c618ed4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618ed6; /* float FPUL,fr3 */
L8c618ed6: sa_motion_publish_stop(ctx,0x8c618ed6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618ed8; /* fmul fr6,fr3 */
L8c618ed8: sa_motion_publish_stop(ctx,0x8c618ed8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618eda; /* fmov.s fr3,@-r8 */
L8c618eda: sa_motion_publish_stop(ctx,0x8c618edau); r[0]=24u;goto L8c618edc; /* mov #0x18,r0 */
L8c618edc: sa_motion_publish_stop(ctx,0x8c618edcu); r[2]=rd(m,bus,&io_status,0x8c61911cu,4);if(io_status)return io_status;goto L8c618ede; /* mov.l 0x8c61911c,r2 */
L8c618ede: sa_motion_publish_stop(ctx,0x8c618edeu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618ee0; /* fmov.s @(r0,r7),fr3 */
L8c618ee0: sa_motion_publish_stop(ctx,0x8c618ee0u); r[0]=20u;goto L8c618ee2; /* mov #0x14,r0 */
L8c618ee2: sa_motion_publish_stop(ctx,0x8c618ee2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618ee4; /* fmov.s fr3,@-r8 */
L8c618ee4: sa_motion_publish_stop(ctx,0x8c618ee4u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618ee6; /* fmov.s @(r0,r7),fr3 */
L8c618ee6: sa_motion_publish_stop(ctx,0x8c618ee6u); r[0]=16u;goto L8c618ee8; /* mov #0x10,r0 */
L8c618ee8: sa_motion_publish_stop(ctx,0x8c618ee8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618eea; /* fmov.s fr3,@-r8 */
L8c618eea: sa_motion_publish_stop(ctx,0x8c618eeau); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618eec; /* fmov.s @(r0,r7),fr3 */
L8c618eec: sa_motion_publish_stop(ctx,0x8c618eecu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618eee; /* fmov.s fr3,@-r8 */
L8c618eee: sa_motion_publish_stop(ctx,0x8c618eeeu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c618ef0; /* mov.l r2,@-r8 */
L8c618ef0: sa_motion_publish_stop(ctx,0x8c618ef0u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618ef2; /* pref @r8 */
L8c618ef2: sa_motion_publish_stop(ctx,0x8c618ef2u); r[0]=rd(m,bus,&io_status,0x8c619120u,4);if(io_status)return io_status;goto L8c618ef4; /* mov.l 0x8c619120,r0 */
L8c618ef4: sa_motion_publish_stop(ctx,0x8c618ef4u); r[8]+=32u;goto L8c618ef6; /* add #0x20,r8 */
L8c618ef6: sa_motion_publish_stop(ctx,0x8c618ef6u); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c618ef8; /* mov.l @r0,r2 */
L8c618ef8: sa_motion_publish_stop(ctx,0x8c618ef8u); r[2]+=3u;goto L8c618efa; /* add #0x3,r2 */
L8c618efa: sa_motion_publish_stop(ctx,0x8c618efau); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c618efc; /* mov.l r2,@r0 */
L8c618efc: sa_motion_publish_stop(ctx,0x8c618efcu); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c618efe; /* mov.w @(0x2,r4),r0 */
L8c618efe: sa_motion_publish_stop(ctx,0x8c618efeu); r[0]=r[0]&65535u;goto L8c618f00; /* extu.w r0,r0 */
L8c618f00: sa_motion_publish_stop(ctx,0x8c618f00u); *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c618f02; /* cmp/ge r0,r14 */
L8c618f02: sa_motion_publish_stop(ctx,0x8c618f02u); if((*sr&1u))goto L8c618f08;goto L8c618f04; /* bt 0x8c618f08 */
L8c618f04: sa_motion_publish_stop(ctx,0x8c618f06u); goto L8c618d74; /* bra 0x8c618d74 */
L8c618f08: sa_motion_publish_stop(ctx,0x8c618f0au); goto L8c6192d0; /* bra 0x8c6192d0 */
L8c618f0c: sa_motion_publish_stop(ctx,0x8c618f0cu); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c618f0e; /* mov.w @(0x2,r4),r0 */
L8c618f0e: sa_motion_publish_stop(ctx,0x8c618f0eu); r[2]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c618f10; /* mov.l @r10,r2 */
L8c618f10: sa_motion_publish_stop(ctx,0x8c618f10u); r[0]=r[0]&65535u;goto L8c618f12; /* extu.w r0,r0 */
L8c618f12: sa_motion_publish_stop(ctx,0x8c618f12u); r[0]<<=2;goto L8c618f14; /* shll2 r0 */
L8c618f14: sa_motion_publish_stop(ctx,0x8c618f14u); r[2]+=r[0];goto L8c618f16; /* add r0,r2 */
L8c618f16: sa_motion_publish_stop(ctx,0x8c618f16u); wr(m,bus,&io_status,r[10],r[2],4);if(io_status)return io_status;goto L8c618f18; /* mov.l r2,@r10 */
L8c618f18: sa_motion_publish_stop(ctx,0x8c618f1au); r[10]=r[11];goto L8c6190ee; /* bra 0x8c6190ee */
L8c618f1c: sa_motion_publish_stop(ctx,0x8c618f1cu); r[6]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c618f1e; /* mov.w @r12+,r6 */
L8c618f1e: sa_motion_publish_stop(ctx,0x8c618f1eu); r[0]=16u;goto L8c618f20; /* mov #0x10,r0 */
L8c618f20: sa_motion_publish_stop(ctx,0x8c618f20u); r[3]=rd(m,bus,&io_status,0x8c619124u,4);if(io_status)return io_status;goto L8c618f22; /* mov.l 0x8c619124,r3 */
L8c618f22: sa_motion_publish_stop(ctx,0x8c618f22u); r[7]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c618f24; /* mov.w @r12+,r7 */
L8c618f24: sa_motion_publish_stop(ctx,0x8c618f24u); r[6]=r[6]&65535u;goto L8c618f26; /* extu.w r6,r6 */
L8c618f26: sa_motion_publish_stop(ctx,0x8c618f26u); r[11]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c618f28; /* mov.l @r3,r11 */
L8c618f28: sa_motion_publish_stop(ctx,0x8c618f28u); r[6]<<=2;goto L8c618f2a; /* shll2 r6 */
L8c618f2a: sa_motion_publish_stop(ctx,0x8c618f2au); r[7]=r[7]&65535u;goto L8c618f2c; /* extu.w r7,r7 */
L8c618f2c: sa_motion_publish_stop(ctx,0x8c618f2cu); r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c618f2e; /* mov.w @r12+,r5 */
L8c618f2e: sa_motion_publish_stop(ctx,0x8c618f2eu); r[7]<<=2;goto L8c618f30; /* shll2 r7 */
L8c618f30: sa_motion_publish_stop(ctx,0x8c618f30u); r[7]<<=2;goto L8c618f32; /* shll2 r7 */
L8c618f32: sa_motion_publish_stop(ctx,0x8c618f32u); r[14]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c618f34; /* mov.w @r12+,r14 */
L8c618f34: sa_motion_publish_stop(ctx,0x8c618f34u); r[6]<<=2;goto L8c618f36; /* shll2 r6 */
L8c618f36: sa_motion_publish_stop(ctx,0x8c618f36u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c618f38; /* shll r6 */
L8c618f38: sa_motion_publish_stop(ctx,0x8c618f38u); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c618f3a; /* shll r7 */
L8c618f3a: sa_motion_publish_stop(ctx,0x8c618f3au); r[7]+=r[11];goto L8c618f3c; /* add r11,r7 */
L8c618f3c: sa_motion_publish_stop(ctx,0x8c618f3cu); r[6]+=r[11];goto L8c618f3e; /* add r11,r6 */
L8c618f3e: sa_motion_publish_stop(ctx,0x8c618f3eu); fload(m,bus,&io_status,f,4u,r[0]+r[7]);if(io_status)return io_status;goto L8c618f40; /* fmov.s @(r0,r7),fr4 */
L8c618f40: sa_motion_publish_stop(ctx,0x8c618f40u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618f42; /* fmov.s @(r0,r6),fr3 */
L8c618f42: sa_motion_publish_stop(ctx,0x8c618f42u); r[5]=r[5]&65535u;goto L8c618f44; /* extu.w r5,r5 */
L8c618f44: sa_motion_publish_stop(ctx,0x8c618f44u); fcopy(f,2u,4u);goto L8c618f46; /* fmov fr4,fr2 */
L8c618f46: sa_motion_publish_stop(ctx,0x8c618f46u); r[5]<<=2;goto L8c618f48; /* shll2 r5 */
L8c618f48: sa_motion_publish_stop(ctx,0x8c618f48u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c618f4a; /* fsub fr3,fr2 */
L8c618f4a: sa_motion_publish_stop(ctx,0x8c618f4au); r[14]=r[14]&65535u;goto L8c618f4c; /* extu.w r14,r14 */
L8c618f4c: sa_motion_publish_stop(ctx,0x8c618f4cu); r[5]<<=2;goto L8c618f4e; /* shll2 r5 */
L8c618f4e: sa_motion_publish_stop(ctx,0x8c618f4eu); r[14]<<=2;goto L8c618f50; /* shll2 r14 */
L8c618f50: sa_motion_publish_stop(ctx,0x8c618f50u); r[0]=20u;goto L8c618f52; /* mov #0x14,r0 */
L8c618f52: sa_motion_publish_stop(ctx,0x8c618f52u); fload(m,bus,&io_status,f,5u,r[0]+r[7]);if(io_status)return io_status;goto L8c618f54; /* fmov.s @(r0,r7),fr5 */
L8c618f54: sa_motion_publish_stop(ctx,0x8c618f54u); r[14]<<=2;goto L8c618f56; /* shll2 r14 */
L8c618f56: sa_motion_publish_stop(ctx,0x8c618f56u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c618f58; /* shll r5 */
L8c618f58: sa_motion_publish_stop(ctx,0x8c618f58u); r[5]+=r[11];goto L8c618f5a; /* add r11,r5 */
L8c618f5a: sa_motion_publish_stop(ctx,0x8c618f5au); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c618f5c; /* shll r14 */
L8c618f5c: sa_motion_publish_stop(ctx,0x8c618f5cu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618f5e; /* fmov.s @(r0,r5),fr3 */
L8c618f5e: sa_motion_publish_stop(ctx,0x8c618f5eu); fcopy(f,1u,5u);goto L8c618f60; /* fmov fr5,fr1 */
L8c618f60: sa_motion_publish_stop(ctx,0x8c618f60u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618f62; /* fsub fr5,fr3 */
L8c618f62: sa_motion_publish_stop(ctx,0x8c618f62u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618f64; /* fmul fr3,fr2 */
L8c618f64: sa_motion_publish_stop(ctx,0x8c618f64u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618f66; /* fmov.s @(r0,r6),fr3 */
L8c618f66: sa_motion_publish_stop(ctx,0x8c618f66u); r[0]=16u;goto L8c618f68; /* mov #0x10,r0 */
L8c618f68: sa_motion_publish_stop(ctx,0x8c618f68u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c618f6a; /* fsub fr3,fr1 */
L8c618f6a: sa_motion_publish_stop(ctx,0x8c618f6au); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618f6c; /* fmov.s @(r0,r5),fr3 */
L8c618f6c: sa_motion_publish_stop(ctx,0x8c618f6cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618f6e; /* fsub fr4,fr3 */
L8c618f6e: sa_motion_publish_stop(ctx,0x8c618f6eu); fcopy(f,4u,2u);goto L8c618f70; /* fmov fr2,fr4 */
L8c618f70: sa_motion_publish_stop(ctx,0x8c618f70u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618f72; /* fmul fr3,fr1 */
L8c618f72: sa_motion_publish_stop(ctx,0x8c618f72u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c618f74; /* fldi0 fr3 */
L8c618f74: sa_motion_publish_stop(ctx,0x8c618f74u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c618f76; /* fsub fr1,fr4 */
L8c618f76: sa_motion_publish_stop(ctx,0x8c618f76u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c618f78; /* fcmp/gt fr3,fr4 */
L8c618f78: sa_motion_publish_stop(ctx,0x8c618f7au); take=!(*sr&1u);r[14]+=r[11];if(take)goto L8c618f80;goto L8c618f7c; /* bf/s 0x8c618f80 */
L8c618f7c: sa_motion_publish_stop(ctx,0x8c618f7eu); r[13]+=16u;goto L8c6190ec; /* bra 0x8c6190ec */
L8c618f80: sa_motion_publish_stop(ctx,0x8c618f80u); r[3]=rd(m,bus,&io_status,0x8c619108u,4);if(io_status)return io_status;goto L8c618f82; /* mov.l 0x8c619108,r3 */
L8c618f82: sa_motion_publish_stop(ctx,0x8c618f82u); r[0]=4u;goto L8c618f84; /* mov #0x4,r0 */
L8c618f84: sa_motion_publish_stop(ctx,0x8c618f84u); fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c618f86; /* fmov.s @r6,fr3 */
L8c618f86: sa_motion_publish_stop(ctx,0x8c618f86u); r[8]+=28u;goto L8c618f88; /* add #0x1c,r8 */
L8c618f88: sa_motion_publish_stop(ctx,0x8c618f88u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618f8a; /* fmov.s @r3,fr2 */
L8c618f8a: sa_motion_publish_stop(ctx,0x8c618f8au); r[3]+=8u;goto L8c618f8c; /* add #0x8,r3 */
L8c618f8c: sa_motion_publish_stop(ctx,0x8c618f8cu); r[2]=rd(m,bus,&io_status,0x8c61910cu,4);if(io_status)return io_status;goto L8c618f8e; /* mov.l 0x8c61910c,r2 */
L8c618f8e: sa_motion_publish_stop(ctx,0x8c618f8eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618f90; /* fmul fr3,fr2 */
L8c618f90: sa_motion_publish_stop(ctx,0x8c618f90u); r[1]=rd(m,bus,&io_status,0x8c619110u,4);if(io_status)return io_status;goto L8c618f92; /* mov.l 0x8c619110,r1 */
L8c618f92: sa_motion_publish_stop(ctx,0x8c618f92u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618f94; /* fmov.s @r2,fr3 */
L8c618f94: sa_motion_publish_stop(ctx,0x8c618f94u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c618f96; /* fmov.s @r1,fr1 */
L8c618f96: sa_motion_publish_stop(ctx,0x8c618f96u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618f98; /* fsub fr2,fr3 */
L8c618f98: sa_motion_publish_stop(ctx,0x8c618f98u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c618f9a; /* fmov.s @(r0,r6),fr2 */
L8c618f9a: sa_motion_publish_stop(ctx,0x8c618f9au); r[0]=8u;goto L8c618f9c; /* mov #0x8,r0 */
L8c618f9c: sa_motion_publish_stop(ctx,0x8c618f9cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618f9e; /* fmul fr2,fr1 */
L8c618f9e: sa_motion_publish_stop(ctx,0x8c618f9eu); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c618fa0; /* fmov.s @(r0,r6),fr2 */
L8c618fa0: sa_motion_publish_stop(ctx,0x8c618fa0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618fa2; /* fsub fr1,fr3 */
L8c618fa2: sa_motion_publish_stop(ctx,0x8c618fa2u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618fa4; /* fmov.s @r3,fr1 */
L8c618fa4: sa_motion_publish_stop(ctx,0x8c618fa4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618fa6; /* fmul fr2,fr1 */
L8c618fa6: sa_motion_publish_stop(ctx,0x8c618fa6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618fa8; /* fsub fr1,fr3 */
L8c618fa8: sa_motion_publish_stop(ctx,0x8c618fa8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618faa; /* fmov.s fr3,@-r8 */
L8c618faa: sa_motion_publish_stop(ctx,0x8c618faau); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c618fac; /* mov.w @(0x2,r13),r0 */
L8c618fac: sa_motion_publish_stop(ctx,0x8c618facu); r[2]=r[0];goto L8c618fae; /* mov r0,r2 */
L8c618fae: sa_motion_publish_stop(ctx,0x8c618faeu); *fpul=r[2];goto L8c618fb0; /* lds r2,FPUL */
L8c618fb0: sa_motion_publish_stop(ctx,0x8c618fb0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618fb2; /* float FPUL,fr3 */
L8c618fb2: sa_motion_publish_stop(ctx,0x8c618fb2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618fb4; /* fmul fr6,fr3 */
L8c618fb4: sa_motion_publish_stop(ctx,0x8c618fb4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618fb6; /* fmov.s fr3,@-r8 */
L8c618fb6: sa_motion_publish_stop(ctx,0x8c618fb6u); r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c618fb8; /* mov.w @r13,r2 */
L8c618fb8: sa_motion_publish_stop(ctx,0x8c618fb8u); *fpul=r[2];goto L8c618fba; /* lds r2,FPUL */
L8c618fba: sa_motion_publish_stop(ctx,0x8c618fbau); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c618fbc; /* float FPUL,fr3 */
L8c618fbc: sa_motion_publish_stop(ctx,0x8c618fbcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c618fbe; /* fmul fr6,fr3 */
L8c618fbe: sa_motion_publish_stop(ctx,0x8c618fbeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618fc0; /* fmov.s fr3,@-r8 */
L8c618fc0: sa_motion_publish_stop(ctx,0x8c618fc0u); r[0]=24u;goto L8c618fc2; /* mov #0x18,r0 */
L8c618fc2: sa_motion_publish_stop(ctx,0x8c618fc2u); r[2]=rd(m,bus,&io_status,0x8c619114u,4);if(io_status)return io_status;goto L8c618fc4; /* mov.l 0x8c619114,r2 */
L8c618fc4: sa_motion_publish_stop(ctx,0x8c618fc4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618fc6; /* fmov.s @(r0,r6),fr3 */
L8c618fc6: sa_motion_publish_stop(ctx,0x8c618fc6u); r[0]=20u;goto L8c618fc8; /* mov #0x14,r0 */
L8c618fc8: sa_motion_publish_stop(ctx,0x8c618fc8u); r[13]+=4u;goto L8c618fca; /* add #0x4,r13 */
L8c618fca: sa_motion_publish_stop(ctx,0x8c618fcau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618fcc; /* fmov.s fr3,@-r8 */
L8c618fcc: sa_motion_publish_stop(ctx,0x8c618fccu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618fce; /* fmov.s @(r0,r6),fr3 */
L8c618fce: sa_motion_publish_stop(ctx,0x8c618fceu); r[0]=16u;goto L8c618fd0; /* mov #0x10,r0 */
L8c618fd0: sa_motion_publish_stop(ctx,0x8c618fd0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618fd2; /* fmov.s fr3,@-r8 */
L8c618fd2: sa_motion_publish_stop(ctx,0x8c618fd2u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618fd4; /* fmov.s @(r0,r6),fr3 */
L8c618fd4: sa_motion_publish_stop(ctx,0x8c618fd4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618fd6; /* fmov.s fr3,@-r8 */
L8c618fd6: sa_motion_publish_stop(ctx,0x8c618fd6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c618fd8; /* mov.l r2,@-r8 */
L8c618fd8: sa_motion_publish_stop(ctx,0x8c618fd8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618fda; /* pref @r8 */
L8c618fda: sa_motion_publish_stop(ctx,0x8c618fdau); r[2]=rd(m,bus,&io_status,0x8c619108u,4);if(io_status)return io_status;goto L8c618fdc; /* mov.l 0x8c619108,r2 */
L8c618fdc: sa_motion_publish_stop(ctx,0x8c618fdcu); r[0]=4u;goto L8c618fde; /* mov #0x4,r0 */
L8c618fde: sa_motion_publish_stop(ctx,0x8c618fdeu); fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c618fe0; /* fmov.s @r7,fr3 */
L8c618fe0: sa_motion_publish_stop(ctx,0x8c618fe0u); r[8]+=60u;goto L8c618fe2; /* add #0x3c,r8 */
L8c618fe2: sa_motion_publish_stop(ctx,0x8c618fe2u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618fe4; /* fmov.s @r2,fr2 */
L8c618fe4: sa_motion_publish_stop(ctx,0x8c618fe4u); r[3]=rd(m,bus,&io_status,0x8c61910cu,4);if(io_status)return io_status;goto L8c618fe6; /* mov.l 0x8c61910c,r3 */
L8c618fe6: sa_motion_publish_stop(ctx,0x8c618fe6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618fe8; /* fmul fr3,fr2 */
L8c618fe8: sa_motion_publish_stop(ctx,0x8c618fe8u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c618fea; /* fmov.s @r1,fr1 */
L8c618fea: sa_motion_publish_stop(ctx,0x8c618feau); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618fec; /* fmov.s @r3,fr3 */
L8c618fec: sa_motion_publish_stop(ctx,0x8c618fecu); r[3]=rd(m,bus,&io_status,0x8c619118u,4);if(io_status)return io_status;goto L8c618fee; /* mov.l 0x8c619118,r3 */
L8c618fee: sa_motion_publish_stop(ctx,0x8c618feeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618ff0; /* fsub fr2,fr3 */
L8c618ff0: sa_motion_publish_stop(ctx,0x8c618ff0u); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c618ff2; /* fmov.s @(r0,r7),fr2 */
L8c618ff2: sa_motion_publish_stop(ctx,0x8c618ff2u); r[0]=8u;goto L8c618ff4; /* mov #0x8,r0 */
L8c618ff4: sa_motion_publish_stop(ctx,0x8c618ff4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618ff6; /* fmul fr2,fr1 */
L8c618ff6: sa_motion_publish_stop(ctx,0x8c618ff6u); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c618ff8; /* fmov.s @(r0,r7),fr2 */
L8c618ff8: sa_motion_publish_stop(ctx,0x8c618ff8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618ffa; /* fsub fr1,fr3 */
L8c618ffa: sa_motion_publish_stop(ctx,0x8c618ffau); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618ffc; /* fmov.s @r3,fr1 */
L8c618ffc: sa_motion_publish_stop(ctx,0x8c618ffcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618ffe; /* fmul fr2,fr1 */
L8c618ffe: sa_motion_publish_stop(ctx,0x8c618ffeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619000; /* fsub fr1,fr3 */
L8c619000: sa_motion_publish_stop(ctx,0x8c619000u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619002; /* fmov.s fr3,@-r8 */
L8c619002: sa_motion_publish_stop(ctx,0x8c619002u); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c619004; /* mov.w @(0x2,r13),r0 */
L8c619004: sa_motion_publish_stop(ctx,0x8c619004u); r[2]=r[0];goto L8c619006; /* mov r0,r2 */
L8c619006: sa_motion_publish_stop(ctx,0x8c619006u); *fpul=r[2];goto L8c619008; /* lds r2,FPUL */
L8c619008: sa_motion_publish_stop(ctx,0x8c619008u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61900a; /* float FPUL,fr3 */
L8c61900a: sa_motion_publish_stop(ctx,0x8c61900au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61900c; /* fmul fr6,fr3 */
L8c61900c: sa_motion_publish_stop(ctx,0x8c61900cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61900e; /* fmov.s fr3,@-r8 */
L8c61900e: sa_motion_publish_stop(ctx,0x8c61900eu); r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c619010; /* mov.w @r13,r2 */
L8c619010: sa_motion_publish_stop(ctx,0x8c619010u); r[13]+=4u;goto L8c619012; /* add #0x4,r13 */
L8c619012: sa_motion_publish_stop(ctx,0x8c619012u); *fpul=r[2];goto L8c619014; /* lds r2,FPUL */
L8c619014: sa_motion_publish_stop(ctx,0x8c619014u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619016; /* float FPUL,fr3 */
L8c619016: sa_motion_publish_stop(ctx,0x8c619016u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619018; /* fmul fr6,fr3 */
L8c619018: sa_motion_publish_stop(ctx,0x8c619018u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61901a; /* fmov.s fr3,@-r8 */
L8c61901a: sa_motion_publish_stop(ctx,0x8c61901au); r[0]=24u;goto L8c61901c; /* mov #0x18,r0 */
L8c61901c: sa_motion_publish_stop(ctx,0x8c61901cu); r[2]=rd(m,bus,&io_status,0x8c619114u,4);if(io_status)return io_status;goto L8c61901e; /* mov.l 0x8c619114,r2 */
L8c61901e: sa_motion_publish_stop(ctx,0x8c61901eu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619020; /* fmov.s @(r0,r7),fr3 */
L8c619020: sa_motion_publish_stop(ctx,0x8c619020u); r[0]=20u;goto L8c619022; /* mov #0x14,r0 */
L8c619022: sa_motion_publish_stop(ctx,0x8c619022u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619024; /* fmov.s fr3,@-r8 */
L8c619024: sa_motion_publish_stop(ctx,0x8c619024u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619026; /* fmov.s @(r0,r7),fr3 */
L8c619026: sa_motion_publish_stop(ctx,0x8c619026u); r[0]=16u;goto L8c619028; /* mov #0x10,r0 */
L8c619028: sa_motion_publish_stop(ctx,0x8c619028u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61902a; /* fmov.s fr3,@-r8 */
L8c61902a: sa_motion_publish_stop(ctx,0x8c61902au); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61902c; /* fmov.s @(r0,r7),fr3 */
L8c61902c: sa_motion_publish_stop(ctx,0x8c61902cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61902e; /* fmov.s fr3,@-r8 */
L8c61902e: sa_motion_publish_stop(ctx,0x8c61902eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c619030; /* mov.l r2,@-r8 */
L8c619030: sa_motion_publish_stop(ctx,0x8c619030u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619032; /* pref @r8 */
L8c619032: sa_motion_publish_stop(ctx,0x8c619032u); r[2]=rd(m,bus,&io_status,0x8c619108u,4);if(io_status)return io_status;goto L8c619034; /* mov.l 0x8c619108,r2 */
L8c619034: sa_motion_publish_stop(ctx,0x8c619034u); r[0]=4u;goto L8c619036; /* mov #0x4,r0 */
L8c619036: sa_motion_publish_stop(ctx,0x8c619036u); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c619038; /* fmov.s @r5,fr3 */
L8c619038: sa_motion_publish_stop(ctx,0x8c619038u); r[8]+=60u;goto L8c61903a; /* add #0x3c,r8 */
L8c61903a: sa_motion_publish_stop(ctx,0x8c61903au); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61903c; /* fmov.s @r2,fr2 */
L8c61903c: sa_motion_publish_stop(ctx,0x8c61903cu); r[3]=rd(m,bus,&io_status,0x8c61910cu,4);if(io_status)return io_status;goto L8c61903e; /* mov.l 0x8c61910c,r3 */
L8c61903e: sa_motion_publish_stop(ctx,0x8c61903eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619040; /* fmul fr3,fr2 */
L8c619040: sa_motion_publish_stop(ctx,0x8c619040u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c619042; /* fmov.s @r1,fr1 */
L8c619042: sa_motion_publish_stop(ctx,0x8c619042u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c619044; /* fmov.s @r3,fr3 */
L8c619044: sa_motion_publish_stop(ctx,0x8c619044u); r[3]=rd(m,bus,&io_status,0x8c619118u,4);if(io_status)return io_status;goto L8c619046; /* mov.l 0x8c619118,r3 */
L8c619046: sa_motion_publish_stop(ctx,0x8c619046u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619048; /* fsub fr2,fr3 */
L8c619048: sa_motion_publish_stop(ctx,0x8c619048u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61904a; /* fmov.s @(r0,r5),fr2 */
L8c61904a: sa_motion_publish_stop(ctx,0x8c61904au); r[0]=8u;goto L8c61904c; /* mov #0x8,r0 */
L8c61904c: sa_motion_publish_stop(ctx,0x8c61904cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61904e; /* fmul fr2,fr1 */
L8c61904e: sa_motion_publish_stop(ctx,0x8c61904eu); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c619050; /* fmov.s @(r0,r5),fr2 */
L8c619050: sa_motion_publish_stop(ctx,0x8c619050u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619052; /* fsub fr1,fr3 */
L8c619052: sa_motion_publish_stop(ctx,0x8c619052u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619054; /* fmov.s @r3,fr1 */
L8c619054: sa_motion_publish_stop(ctx,0x8c619054u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619056; /* fmul fr2,fr1 */
L8c619056: sa_motion_publish_stop(ctx,0x8c619056u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619058; /* fsub fr1,fr3 */
L8c619058: sa_motion_publish_stop(ctx,0x8c619058u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61905a; /* fmov.s fr3,@-r8 */
L8c61905a: sa_motion_publish_stop(ctx,0x8c61905au); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61905c; /* mov.w @(0x2,r13),r0 */
L8c61905c: sa_motion_publish_stop(ctx,0x8c61905cu); r[2]=r[0];goto L8c61905e; /* mov r0,r2 */
L8c61905e: sa_motion_publish_stop(ctx,0x8c61905eu); *fpul=r[2];goto L8c619060; /* lds r2,FPUL */
L8c619060: sa_motion_publish_stop(ctx,0x8c619060u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619062; /* float FPUL,fr3 */
L8c619062: sa_motion_publish_stop(ctx,0x8c619062u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619064; /* fmul fr6,fr3 */
L8c619064: sa_motion_publish_stop(ctx,0x8c619064u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619066; /* fmov.s fr3,@-r8 */
L8c619066: sa_motion_publish_stop(ctx,0x8c619066u); r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c619068; /* mov.w @r13,r2 */
L8c619068: sa_motion_publish_stop(ctx,0x8c619068u); r[13]+=4u;goto L8c61906a; /* add #0x4,r13 */
L8c61906a: sa_motion_publish_stop(ctx,0x8c61906au); *fpul=r[2];goto L8c61906c; /* lds r2,FPUL */
L8c61906c: sa_motion_publish_stop(ctx,0x8c61906cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61906e; /* float FPUL,fr3 */
L8c61906e: sa_motion_publish_stop(ctx,0x8c61906eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619070; /* fmul fr6,fr3 */
L8c619070: sa_motion_publish_stop(ctx,0x8c619070u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619072; /* fmov.s fr3,@-r8 */
L8c619072: sa_motion_publish_stop(ctx,0x8c619072u); r[0]=24u;goto L8c619074; /* mov #0x18,r0 */
L8c619074: sa_motion_publish_stop(ctx,0x8c619074u); r[2]=rd(m,bus,&io_status,0x8c619114u,4);if(io_status)return io_status;goto L8c619076; /* mov.l 0x8c619114,r2 */
L8c619076: sa_motion_publish_stop(ctx,0x8c619076u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619078; /* fmov.s @(r0,r5),fr3 */
L8c619078: sa_motion_publish_stop(ctx,0x8c619078u); r[0]=20u;goto L8c61907a; /* mov #0x14,r0 */
L8c61907a: sa_motion_publish_stop(ctx,0x8c61907au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61907c; /* fmov.s fr3,@-r8 */
L8c61907c: sa_motion_publish_stop(ctx,0x8c61907cu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61907e; /* fmov.s @(r0,r5),fr3 */
L8c61907e: sa_motion_publish_stop(ctx,0x8c61907eu); r[0]=16u;goto L8c619080; /* mov #0x10,r0 */
L8c619080: sa_motion_publish_stop(ctx,0x8c619080u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619082; /* fmov.s fr3,@-r8 */
L8c619082: sa_motion_publish_stop(ctx,0x8c619082u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619084; /* fmov.s @(r0,r5),fr3 */
L8c619084: sa_motion_publish_stop(ctx,0x8c619084u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619086; /* fmov.s fr3,@-r8 */
L8c619086: sa_motion_publish_stop(ctx,0x8c619086u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c619088; /* mov.l r2,@-r8 */
L8c619088: sa_motion_publish_stop(ctx,0x8c619088u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61908a; /* pref @r8 */
L8c61908a: sa_motion_publish_stop(ctx,0x8c61908au); r[2]=rd(m,bus,&io_status,0x8c619108u,4);if(io_status)return io_status;goto L8c61908c; /* mov.l 0x8c619108,r2 */
L8c61908c: sa_motion_publish_stop(ctx,0x8c61908cu); r[0]=4u;goto L8c61908e; /* mov #0x4,r0 */
L8c61908e: sa_motion_publish_stop(ctx,0x8c61908eu); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c619090; /* fmov.s @r14,fr3 */
L8c619090: sa_motion_publish_stop(ctx,0x8c619090u); r[8]+=60u;goto L8c619092; /* add #0x3c,r8 */
L8c619092: sa_motion_publish_stop(ctx,0x8c619092u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c619094; /* fmov.s @r2,fr2 */
L8c619094: sa_motion_publish_stop(ctx,0x8c619094u); r[3]=rd(m,bus,&io_status,0x8c61910cu,4);if(io_status)return io_status;goto L8c619096; /* mov.l 0x8c61910c,r3 */
L8c619096: sa_motion_publish_stop(ctx,0x8c619096u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619098; /* fmul fr3,fr2 */
L8c619098: sa_motion_publish_stop(ctx,0x8c619098u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61909a; /* fmov.s @r1,fr1 */
L8c61909a: sa_motion_publish_stop(ctx,0x8c61909au); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61909c; /* fmov.s @r3,fr3 */
L8c61909c: sa_motion_publish_stop(ctx,0x8c61909cu); r[3]=rd(m,bus,&io_status,0x8c619118u,4);if(io_status)return io_status;goto L8c61909e; /* mov.l 0x8c619118,r3 */
L8c61909e: sa_motion_publish_stop(ctx,0x8c61909eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6190a0; /* fsub fr2,fr3 */
L8c6190a0: sa_motion_publish_stop(ctx,0x8c6190a0u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c6190a2; /* fmov.s @(r0,r14),fr2 */
L8c6190a2: sa_motion_publish_stop(ctx,0x8c6190a2u); r[0]=8u;goto L8c6190a4; /* mov #0x8,r0 */
L8c6190a4: sa_motion_publish_stop(ctx,0x8c6190a4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6190a6; /* fmul fr2,fr1 */
L8c6190a6: sa_motion_publish_stop(ctx,0x8c6190a6u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c6190a8; /* fmov.s @(r0,r14),fr2 */
L8c6190a8: sa_motion_publish_stop(ctx,0x8c6190a8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6190aa; /* fsub fr1,fr3 */
L8c6190aa: sa_motion_publish_stop(ctx,0x8c6190aau); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6190ac; /* fmov.s @r3,fr1 */
L8c6190ac: sa_motion_publish_stop(ctx,0x8c6190acu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6190ae; /* fmul fr2,fr1 */
L8c6190ae: sa_motion_publish_stop(ctx,0x8c6190aeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6190b0; /* fsub fr1,fr3 */
L8c6190b0: sa_motion_publish_stop(ctx,0x8c6190b0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6190b2; /* fmov.s fr3,@-r8 */
L8c6190b2: sa_motion_publish_stop(ctx,0x8c6190b2u); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c6190b4; /* mov.w @(0x2,r13),r0 */
L8c6190b4: sa_motion_publish_stop(ctx,0x8c6190b4u); r[2]=r[0];goto L8c6190b6; /* mov r0,r2 */
L8c6190b6: sa_motion_publish_stop(ctx,0x8c6190b6u); *fpul=r[2];goto L8c6190b8; /* lds r2,FPUL */
L8c6190b8: sa_motion_publish_stop(ctx,0x8c6190b8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6190ba; /* float FPUL,fr3 */
L8c6190ba: sa_motion_publish_stop(ctx,0x8c6190bau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6190bc; /* fmul fr6,fr3 */
L8c6190bc: sa_motion_publish_stop(ctx,0x8c6190bcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6190be; /* fmov.s fr3,@-r8 */
L8c6190be: sa_motion_publish_stop(ctx,0x8c6190beu); r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c6190c0; /* mov.w @r13,r2 */
L8c6190c0: sa_motion_publish_stop(ctx,0x8c6190c0u); r[13]+=4u;goto L8c6190c2; /* add #0x4,r13 */
L8c6190c2: sa_motion_publish_stop(ctx,0x8c6190c2u); *fpul=r[2];goto L8c6190c4; /* lds r2,FPUL */
L8c6190c4: sa_motion_publish_stop(ctx,0x8c6190c4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6190c6; /* float FPUL,fr3 */
L8c6190c6: sa_motion_publish_stop(ctx,0x8c6190c6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6190c8; /* fmul fr6,fr3 */
L8c6190c8: sa_motion_publish_stop(ctx,0x8c6190c8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6190ca; /* fmov.s fr3,@-r8 */
L8c6190ca: sa_motion_publish_stop(ctx,0x8c6190cau); r[0]=24u;goto L8c6190cc; /* mov #0x18,r0 */
L8c6190cc: sa_motion_publish_stop(ctx,0x8c6190ccu); r[2]=rd(m,bus,&io_status,0x8c61911cu,4);if(io_status)return io_status;goto L8c6190ce; /* mov.l 0x8c61911c,r2 */
L8c6190ce: sa_motion_publish_stop(ctx,0x8c6190ceu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6190d0; /* fmov.s @(r0,r14),fr3 */
L8c6190d0: sa_motion_publish_stop(ctx,0x8c6190d0u); r[0]=20u;goto L8c6190d2; /* mov #0x14,r0 */
L8c6190d2: sa_motion_publish_stop(ctx,0x8c6190d2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6190d4; /* fmov.s fr3,@-r8 */
L8c6190d4: sa_motion_publish_stop(ctx,0x8c6190d4u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6190d6; /* fmov.s @(r0,r14),fr3 */
L8c6190d6: sa_motion_publish_stop(ctx,0x8c6190d6u); r[0]=16u;goto L8c6190d8; /* mov #0x10,r0 */
L8c6190d8: sa_motion_publish_stop(ctx,0x8c6190d8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6190da; /* fmov.s fr3,@-r8 */
L8c6190da: sa_motion_publish_stop(ctx,0x8c6190dau); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6190dc; /* fmov.s @(r0,r14),fr3 */
L8c6190dc: sa_motion_publish_stop(ctx,0x8c6190dcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6190de; /* fmov.s fr3,@-r8 */
L8c6190de: sa_motion_publish_stop(ctx,0x8c6190deu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c6190e0; /* mov.l r2,@-r8 */
L8c6190e0: sa_motion_publish_stop(ctx,0x8c6190e0u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6190e2; /* pref @r8 */
L8c6190e2: sa_motion_publish_stop(ctx,0x8c6190e2u); r[0]=rd(m,bus,&io_status,0x8c619120u,4);if(io_status)return io_status;goto L8c6190e4; /* mov.l 0x8c619120,r0 */
L8c6190e4: sa_motion_publish_stop(ctx,0x8c6190e4u); r[8]+=32u;goto L8c6190e6; /* add #0x20,r8 */
L8c6190e6: sa_motion_publish_stop(ctx,0x8c6190e6u); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c6190e8; /* mov.l @r0,r2 */
L8c6190e8: sa_motion_publish_stop(ctx,0x8c6190e8u); r[2]+=4u;goto L8c6190ea; /* add #0x4,r2 */
L8c6190ea: sa_motion_publish_stop(ctx,0x8c6190eau); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c6190ec; /* mov.l r2,@r0 */
L8c6190ec: sa_motion_publish_stop(ctx,0x8c6190ecu); r[10]+=1u;goto L8c6190ee; /* add #0x1,r10 */
L8c6190ee: sa_motion_publish_stop(ctx,0x8c6190eeu); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c6190f0; /* mov.w @(0x2,r4),r0 */
L8c6190f0: sa_motion_publish_stop(ctx,0x8c6190f0u); r[0]=r[0]&65535u;goto L8c6190f2; /* extu.w r0,r0 */
L8c6190f2: sa_motion_publish_stop(ctx,0x8c6190f2u); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c6190f4; /* cmp/ge r0,r10 */
L8c6190f4: sa_motion_publish_stop(ctx,0x8c6190f4u); if((*sr&1u))goto L8c6190fa;goto L8c6190f6; /* bt 0x8c6190fa */
L8c6190f6: sa_motion_publish_stop(ctx,0x8c6190f8u); goto L8c618f1c; /* bra 0x8c618f1c */
L8c6190fa: sa_motion_publish_stop(ctx,0x8c6190fcu); goto L8c6192d0; /* bra 0x8c6192d0 */
L8c6190fe: sa_motion_publish_stop(ctx,0x8c619100u); r[9]=r[11];goto L8c619284; /* bra 0x8c619284 */
L8c619128: sa_motion_publish_stop(ctx,0x8c619128u); r[14]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c61912a; /* mov.w @r12+,r14 */
L8c61912a: sa_motion_publish_stop(ctx,0x8c61912au); r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c61912c; /* mov.l @r10,r3 */
L8c61912c: sa_motion_publish_stop(ctx,0x8c61912cu); r[14]=r[14]&65535u;goto L8c61912e; /* extu.w r14,r14 */
L8c61912e: sa_motion_publish_stop(ctx,0x8c61912eu); r[2]=rd(m,bus,&io_status,0x8c6192ecu,4);if(io_status)return io_status;goto L8c619130; /* mov.l 0x8c6192ec,r2 */
L8c619130: sa_motion_publish_stop(ctx,0x8c619130u); r[3]+=r[14];goto L8c619132; /* add r14,r3 */
L8c619132: sa_motion_publish_stop(ctx,0x8c619132u); wr(m,bus,&io_status,r[10],r[3],4);if(io_status)return io_status;goto L8c619134; /* mov.l r3,@r10 */
L8c619134: sa_motion_publish_stop(ctx,0x8c619134u); r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c619136; /* mov.w @(0x2,r12),r0 */
L8c619136: sa_motion_publish_stop(ctx,0x8c619136u); r[7]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;goto L8c619138; /* mov.w @r12,r7 */
L8c619138: sa_motion_publish_stop(ctx,0x8c619138u); r[5]=r[0]&65535u;goto L8c61913a; /* extu.w r0,r5 */
L8c61913a: sa_motion_publish_stop(ctx,0x8c61913au); r[0]=sx(m,bus,&io_status,r[12]+0x4u,2);if(io_status)return io_status;goto L8c61913c; /* mov.w @(0x4,r12),r0 */
L8c61913c: sa_motion_publish_stop(ctx,0x8c61913cu); r[5]<<=2;goto L8c61913e; /* shll2 r5 */
L8c61913e: sa_motion_publish_stop(ctx,0x8c61913eu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c619140; /* mov.l @r2,r3 */
L8c619140: sa_motion_publish_stop(ctx,0x8c619140u); r[7]=r[7]&65535u;goto L8c619142; /* extu.w r7,r7 */
L8c619142: sa_motion_publish_stop(ctx,0x8c619142u); r[7]<<=2;goto L8c619144; /* shll2 r7 */
L8c619144: sa_motion_publish_stop(ctx,0x8c619144u); r[5]<<=2;goto L8c619146; /* shll2 r5 */
L8c619146: sa_motion_publish_stop(ctx,0x8c619146u); r[6]=r[0]&65535u;goto L8c619148; /* extu.w r0,r6 */
L8c619148: sa_motion_publish_stop(ctx,0x8c619148u); r[7]<<=2;goto L8c61914a; /* shll2 r7 */
L8c61914a: sa_motion_publish_stop(ctx,0x8c61914au); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61914c; /* shll r7 */
L8c61914c: sa_motion_publish_stop(ctx,0x8c61914cu); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61914e; /* shll r5 */
L8c61914e: sa_motion_publish_stop(ctx,0x8c61914eu); r[0]=16u;goto L8c619150; /* mov #0x10,r0 */
L8c619150: sa_motion_publish_stop(ctx,0x8c619150u); r[5]+=r[3];goto L8c619152; /* add r3,r5 */
L8c619152: sa_motion_publish_stop(ctx,0x8c619152u); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c619154; /* fmov.s @(r0,r5),fr5 */
L8c619154: sa_motion_publish_stop(ctx,0x8c619154u); r[7]+=r[3];goto L8c619156; /* add r3,r7 */
L8c619156: sa_motion_publish_stop(ctx,0x8c619156u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619158; /* fmov.s @(r0,r7),fr3 */
L8c619158: sa_motion_publish_stop(ctx,0x8c619158u); r[6]<<=2;goto L8c61915a; /* shll2 r6 */
L8c61915a: sa_motion_publish_stop(ctx,0x8c61915au); fcopy(f,2u,5u);goto L8c61915c; /* fmov fr5,fr2 */
L8c61915c: sa_motion_publish_stop(ctx,0x8c61915cu); r[6]<<=2;goto L8c61915e; /* shll2 r6 */
L8c61915e: sa_motion_publish_stop(ctx,0x8c61915eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c619160; /* fsub fr3,fr2 */
L8c619160: sa_motion_publish_stop(ctx,0x8c619160u); r[0]=20u;goto L8c619162; /* mov #0x14,r0 */
L8c619162: sa_motion_publish_stop(ctx,0x8c619162u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c619164; /* shll r6 */
L8c619164: sa_motion_publish_stop(ctx,0x8c619164u); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c619166; /* fmov.s @(r0,r5),fr4 */
L8c619166: sa_motion_publish_stop(ctx,0x8c619166u); r[6]+=r[3];goto L8c619168; /* add r3,r6 */
L8c619168: sa_motion_publish_stop(ctx,0x8c619168u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61916a; /* fmov.s @(r0,r6),fr3 */
L8c61916a: sa_motion_publish_stop(ctx,0x8c61916au); fcopy(f,1u,4u);goto L8c61916c; /* fmov fr4,fr1 */
L8c61916c: sa_motion_publish_stop(ctx,0x8c61916cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61916e; /* fsub fr4,fr3 */
L8c61916e: sa_motion_publish_stop(ctx,0x8c61916eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619170; /* fmul fr3,fr2 */
L8c619170: sa_motion_publish_stop(ctx,0x8c619170u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619172; /* fmov.s @(r0,r7),fr3 */
L8c619172: sa_motion_publish_stop(ctx,0x8c619172u); r[0]=16u;goto L8c619174; /* mov #0x10,r0 */
L8c619174: sa_motion_publish_stop(ctx,0x8c619174u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c619176; /* fsub fr3,fr1 */
L8c619176: sa_motion_publish_stop(ctx,0x8c619176u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619178; /* fmov.s @(r0,r6),fr3 */
L8c619178: sa_motion_publish_stop(ctx,0x8c619178u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61917a; /* fsub fr5,fr3 */
L8c61917a: sa_motion_publish_stop(ctx,0x8c61917au); fcopy(f,4u,2u);goto L8c61917c; /* fmov fr2,fr4 */
L8c61917c: sa_motion_publish_stop(ctx,0x8c61917cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61917e; /* fmul fr3,fr1 */
L8c61917e: sa_motion_publish_stop(ctx,0x8c61917eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c619180; /* fldi0 fr3 */
L8c619180: sa_motion_publish_stop(ctx,0x8c619180u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c619182; /* fsub fr1,fr4 */
L8c619182: sa_motion_publish_stop(ctx,0x8c619182u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c619184; /* fcmp/gt fr3,fr4 */
L8c619184: sa_motion_publish_stop(ctx,0x8c619184u); if(!(*sr&1u))goto L8c619192;goto L8c619186; /* bf 0x8c619192 */
L8c619186: sa_motion_publish_stop(ctx,0x8c619186u); r[1]=r[14];goto L8c619188; /* mov r14,r1 */
L8c619188: sa_motion_publish_stop(ctx,0x8c619188u); *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c61918a; /* shll r1 */
L8c61918a: sa_motion_publish_stop(ctx,0x8c61918au); r[14]<<=2;goto L8c61918c; /* shll2 r14 */
L8c61918c: sa_motion_publish_stop(ctx,0x8c61918cu); r[12]+=r[1];goto L8c61918e; /* add r1,r12 */
L8c61918e: sa_motion_publish_stop(ctx,0x8c619190u); r[13]+=r[14];goto L8c619282; /* bra 0x8c619282 */
L8c619192: sa_motion_publish_stop(ctx,0x8c619192u); r[2]=rd(m,bus,&io_status,0x8c6192f0u,4);if(io_status)return io_status;goto L8c619194; /* mov.l 0x8c6192f0,r2 */
L8c619194: sa_motion_publish_stop(ctx,0x8c619194u); r[7]=r[14];goto L8c619196; /* mov r14,r7 */
L8c619196: sa_motion_publish_stop(ctx,0x8c619196u); r[7]+=4294967295u;goto L8c619198; /* add #-0x1,r7 */
L8c619198: sa_motion_publish_stop(ctx,0x8c619198u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61919a; /* mov.l @r2,r3 */
L8c61919a: sa_motion_publish_stop(ctx,0x8c61919au); *sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c61919c; /* cmp/pl r7 */
L8c61919c: sa_motion_publish_stop(ctx,0x8c61919cu); r[3]+=r[14];goto L8c61919e; /* add r14,r3 */
L8c61919e: sa_motion_publish_stop(ctx,0x8c61919eu); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6191a0; /* mov.l r3,@r2 */
L8c6191a0: sa_motion_publish_stop(ctx,0x8c6191a2u); take=!(*sr&1u);r[6]=r[11];if(take)goto L8c619216;goto L8c6191a4; /* bf/s 0x8c619216 */
L8c6191a4: sa_motion_publish_stop(ctx,0x8c6191a4u); r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c6191a6; /* mov.w @r12+,r5 */
L8c6191a6: sa_motion_publish_stop(ctx,0x8c6191a6u); r[0]=4u;goto L8c6191a8; /* mov #0x4,r0 */
L8c6191a8: sa_motion_publish_stop(ctx,0x8c6191a8u); r[2]=rd(m,bus,&io_status,0x8c6192ecu,4);if(io_status)return io_status;goto L8c6191aa; /* mov.l 0x8c6192ec,r2 */
L8c6191aa: sa_motion_publish_stop(ctx,0x8c6191aau); r[8]+=28u;goto L8c6191ac; /* add #0x1c,r8 */
L8c6191ac: sa_motion_publish_stop(ctx,0x8c6191acu); r[5]=r[5]&65535u;goto L8c6191ae; /* extu.w r5,r5 */
L8c6191ae: sa_motion_publish_stop(ctx,0x8c6191aeu); r[1]=rd(m,bus,&io_status,0x8c6192f8u,4);if(io_status)return io_status;goto L8c6191b0; /* mov.l 0x8c6192f8,r1 */
L8c6191b0: sa_motion_publish_stop(ctx,0x8c6191b0u); r[5]<<=2;goto L8c6191b2; /* shll2 r5 */
L8c6191b2: sa_motion_publish_stop(ctx,0x8c6191b2u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6191b4; /* mov.l @r2,r3 */
L8c6191b4: sa_motion_publish_stop(ctx,0x8c6191b4u); r[5]<<=2;goto L8c6191b6; /* shll2 r5 */
L8c6191b6: sa_motion_publish_stop(ctx,0x8c6191b6u); r[2]=rd(m,bus,&io_status,0x8c6192fcu,4);if(io_status)return io_status;goto L8c6191b8; /* mov.l 0x8c6192fc,r2 */
L8c6191b8: sa_motion_publish_stop(ctx,0x8c6191b8u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6191ba; /* shll r5 */
L8c6191ba: sa_motion_publish_stop(ctx,0x8c6191bau); r[5]+=r[3];goto L8c6191bc; /* add r3,r5 */
L8c6191bc: sa_motion_publish_stop(ctx,0x8c6191bcu); r[3]=rd(m,bus,&io_status,0x8c6192f4u,4);if(io_status)return io_status;goto L8c6191be; /* mov.l 0x8c6192f4,r3 */
L8c6191be: sa_motion_publish_stop(ctx,0x8c6191beu); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c6191c0; /* fmov.s @r5,fr3 */
L8c6191c0: sa_motion_publish_stop(ctx,0x8c6191c0u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6191c2; /* fmov.s @r3,fr2 */
L8c6191c2: sa_motion_publish_stop(ctx,0x8c6191c2u); r[3]+=8u;goto L8c6191c4; /* add #0x8,r3 */
L8c6191c4: sa_motion_publish_stop(ctx,0x8c6191c4u); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c6191c6; /* fmov.s @r2,fr1 */
L8c6191c6: sa_motion_publish_stop(ctx,0x8c6191c6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6191c8; /* fmul fr3,fr2 */
L8c6191c8: sa_motion_publish_stop(ctx,0x8c6191c8u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6191ca; /* fmov.s @r1,fr3 */
L8c6191ca: sa_motion_publish_stop(ctx,0x8c6191cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6191cc; /* fsub fr2,fr3 */
L8c6191cc: sa_motion_publish_stop(ctx,0x8c6191ccu); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c6191ce; /* fmov.s @(r0,r5),fr2 */
L8c6191ce: sa_motion_publish_stop(ctx,0x8c6191ceu); r[0]=8u;goto L8c6191d0; /* mov #0x8,r0 */
L8c6191d0: sa_motion_publish_stop(ctx,0x8c6191d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6191d2; /* fmul fr2,fr1 */
L8c6191d2: sa_motion_publish_stop(ctx,0x8c6191d2u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c6191d4; /* fmov.s @(r0,r5),fr2 */
L8c6191d4: sa_motion_publish_stop(ctx,0x8c6191d4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6191d6; /* fsub fr1,fr3 */
L8c6191d6: sa_motion_publish_stop(ctx,0x8c6191d6u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6191d8; /* fmov.s @r3,fr1 */
L8c6191d8: sa_motion_publish_stop(ctx,0x8c6191d8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6191da; /* fmul fr2,fr1 */
L8c6191da: sa_motion_publish_stop(ctx,0x8c6191dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6191dc; /* fsub fr1,fr3 */
L8c6191dc: sa_motion_publish_stop(ctx,0x8c6191dcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6191de; /* fmov.s fr3,@-r8 */
L8c6191de: sa_motion_publish_stop(ctx,0x8c6191deu); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c6191e0; /* mov.w @(0x2,r13),r0 */
L8c6191e0: sa_motion_publish_stop(ctx,0x8c6191e0u); r[2]=r[0];goto L8c6191e2; /* mov r0,r2 */
L8c6191e2: sa_motion_publish_stop(ctx,0x8c6191e2u); *fpul=r[2];goto L8c6191e4; /* lds r2,FPUL */
L8c6191e4: sa_motion_publish_stop(ctx,0x8c6191e4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6191e6; /* float FPUL,fr3 */
L8c6191e6: sa_motion_publish_stop(ctx,0x8c6191e6u); r[0]=24u;goto L8c6191e8; /* mov #0x18,r0 */
L8c6191e8: sa_motion_publish_stop(ctx,0x8c6191e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6191ea; /* fmul fr6,fr3 */
L8c6191ea: sa_motion_publish_stop(ctx,0x8c6191eau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6191ec; /* fmov.s fr3,@-r8 */
L8c6191ec: sa_motion_publish_stop(ctx,0x8c6191ecu); r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c6191ee; /* mov.w @r13,r2 */
L8c6191ee: sa_motion_publish_stop(ctx,0x8c6191eeu); r[13]+=4u;goto L8c6191f0; /* add #0x4,r13 */
L8c6191f0: sa_motion_publish_stop(ctx,0x8c6191f0u); *fpul=r[2];goto L8c6191f2; /* lds r2,FPUL */
L8c6191f2: sa_motion_publish_stop(ctx,0x8c6191f2u); r[2]=rd(m,bus,&io_status,0x8c619300u,4);if(io_status)return io_status;goto L8c6191f4; /* mov.l 0x8c619300,r2 */
L8c6191f4: sa_motion_publish_stop(ctx,0x8c6191f4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6191f6; /* float FPUL,fr3 */
L8c6191f6: sa_motion_publish_stop(ctx,0x8c6191f6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6191f8; /* fmul fr6,fr3 */
L8c6191f8: sa_motion_publish_stop(ctx,0x8c6191f8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6191fa; /* fmov.s fr3,@-r8 */
L8c6191fa: sa_motion_publish_stop(ctx,0x8c6191fau); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6191fc; /* fmov.s @(r0,r5),fr3 */
L8c6191fc: sa_motion_publish_stop(ctx,0x8c6191fcu); r[0]=20u;goto L8c6191fe; /* mov #0x14,r0 */
L8c6191fe: sa_motion_publish_stop(ctx,0x8c6191feu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619200; /* fmov.s fr3,@-r8 */
L8c619200: sa_motion_publish_stop(ctx,0x8c619200u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619202; /* fmov.s @(r0,r5),fr3 */
L8c619202: sa_motion_publish_stop(ctx,0x8c619202u); r[0]=16u;goto L8c619204; /* mov #0x10,r0 */
L8c619204: sa_motion_publish_stop(ctx,0x8c619204u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619206; /* fmov.s fr3,@-r8 */
L8c619206: sa_motion_publish_stop(ctx,0x8c619206u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619208; /* fmov.s @(r0,r5),fr3 */
L8c619208: sa_motion_publish_stop(ctx,0x8c619208u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61920a; /* fmov.s fr3,@-r8 */
L8c61920a: sa_motion_publish_stop(ctx,0x8c61920au); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61920c; /* mov.l r2,@-r8 */
L8c61920c: sa_motion_publish_stop(ctx,0x8c61920cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61920e; /* pref @r8 */
L8c61920e: sa_motion_publish_stop(ctx,0x8c61920eu); r[6]+=1u;goto L8c619210; /* add #0x1,r6 */
L8c619210: sa_motion_publish_stop(ctx,0x8c619210u); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c619212; /* cmp/ge r7,r6 */
L8c619212: sa_motion_publish_stop(ctx,0x8c619214u); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c6191a4;goto L8c619216; /* bf/s 0x8c6191a4 */
L8c619216: sa_motion_publish_stop(ctx,0x8c619216u); r[5]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;r[12]+=2;goto L8c619218; /* mov.w @r12+,r5 */
L8c619218: sa_motion_publish_stop(ctx,0x8c619218u); r[0]=4u;goto L8c61921a; /* mov #0x4,r0 */
L8c61921a: sa_motion_publish_stop(ctx,0x8c61921au); r[2]=rd(m,bus,&io_status,0x8c6192ecu,4);if(io_status)return io_status;goto L8c61921c; /* mov.l 0x8c6192ec,r2 */
L8c61921c: sa_motion_publish_stop(ctx,0x8c61921cu); r[8]+=28u;goto L8c61921e; /* add #0x1c,r8 */
L8c61921e: sa_motion_publish_stop(ctx,0x8c61921eu); r[5]=r[5]&65535u;goto L8c619220; /* extu.w r5,r5 */
L8c619220: sa_motion_publish_stop(ctx,0x8c619220u); r[1]=rd(m,bus,&io_status,0x8c6192f8u,4);if(io_status)return io_status;goto L8c619222; /* mov.l 0x8c6192f8,r1 */
L8c619222: sa_motion_publish_stop(ctx,0x8c619222u); r[5]<<=2;goto L8c619224; /* shll2 r5 */
L8c619224: sa_motion_publish_stop(ctx,0x8c619224u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c619226; /* mov.l @r2,r3 */
L8c619226: sa_motion_publish_stop(ctx,0x8c619226u); r[5]<<=2;goto L8c619228; /* shll2 r5 */
L8c619228: sa_motion_publish_stop(ctx,0x8c619228u); r[2]=rd(m,bus,&io_status,0x8c6192fcu,4);if(io_status)return io_status;goto L8c61922a; /* mov.l 0x8c6192fc,r2 */
L8c61922a: sa_motion_publish_stop(ctx,0x8c61922au); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61922c; /* shll r5 */
L8c61922c: sa_motion_publish_stop(ctx,0x8c61922cu); r[5]+=r[3];goto L8c61922e; /* add r3,r5 */
L8c61922e: sa_motion_publish_stop(ctx,0x8c61922eu); r[3]=rd(m,bus,&io_status,0x8c6192f4u,4);if(io_status)return io_status;goto L8c619230; /* mov.l 0x8c6192f4,r3 */
L8c619230: sa_motion_publish_stop(ctx,0x8c619230u); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c619232; /* fmov.s @r5,fr3 */
L8c619232: sa_motion_publish_stop(ctx,0x8c619232u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c619234; /* fmov.s @r3,fr2 */
L8c619234: sa_motion_publish_stop(ctx,0x8c619234u); r[3]+=8u;goto L8c619236; /* add #0x8,r3 */
L8c619236: sa_motion_publish_stop(ctx,0x8c619236u); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c619238; /* fmov.s @r2,fr1 */
L8c619238: sa_motion_publish_stop(ctx,0x8c619238u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61923a; /* fmul fr3,fr2 */
L8c61923a: sa_motion_publish_stop(ctx,0x8c61923au); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61923c; /* fmov.s @r1,fr3 */
L8c61923c: sa_motion_publish_stop(ctx,0x8c61923cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61923e; /* fsub fr2,fr3 */
L8c61923e: sa_motion_publish_stop(ctx,0x8c61923eu); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c619240; /* fmov.s @(r0,r5),fr2 */
L8c619240: sa_motion_publish_stop(ctx,0x8c619240u); r[0]=8u;goto L8c619242; /* mov #0x8,r0 */
L8c619242: sa_motion_publish_stop(ctx,0x8c619242u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619244; /* fmul fr2,fr1 */
L8c619244: sa_motion_publish_stop(ctx,0x8c619244u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c619246; /* fmov.s @(r0,r5),fr2 */
L8c619246: sa_motion_publish_stop(ctx,0x8c619246u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619248; /* fsub fr1,fr3 */
L8c619248: sa_motion_publish_stop(ctx,0x8c619248u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61924a; /* fmov.s @r3,fr1 */
L8c61924a: sa_motion_publish_stop(ctx,0x8c61924au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61924c; /* fmul fr2,fr1 */
L8c61924c: sa_motion_publish_stop(ctx,0x8c61924cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61924e; /* fsub fr1,fr3 */
L8c61924e: sa_motion_publish_stop(ctx,0x8c61924eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619250; /* fmov.s fr3,@-r8 */
L8c619250: sa_motion_publish_stop(ctx,0x8c619250u); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c619252; /* mov.w @(0x2,r13),r0 */
L8c619252: sa_motion_publish_stop(ctx,0x8c619252u); r[2]=r[0];goto L8c619254; /* mov r0,r2 */
L8c619254: sa_motion_publish_stop(ctx,0x8c619254u); *fpul=r[2];goto L8c619256; /* lds r2,FPUL */
L8c619256: sa_motion_publish_stop(ctx,0x8c619256u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619258; /* float FPUL,fr3 */
L8c619258: sa_motion_publish_stop(ctx,0x8c619258u); r[0]=24u;goto L8c61925a; /* mov #0x18,r0 */
L8c61925a: sa_motion_publish_stop(ctx,0x8c61925au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61925c; /* fmul fr6,fr3 */
L8c61925c: sa_motion_publish_stop(ctx,0x8c61925cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61925e; /* fmov.s fr3,@-r8 */
L8c61925e: sa_motion_publish_stop(ctx,0x8c61925eu); r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c619260; /* mov.w @r13,r2 */
L8c619260: sa_motion_publish_stop(ctx,0x8c619260u); r[13]+=4u;goto L8c619262; /* add #0x4,r13 */
L8c619262: sa_motion_publish_stop(ctx,0x8c619262u); *fpul=r[2];goto L8c619264; /* lds r2,FPUL */
L8c619264: sa_motion_publish_stop(ctx,0x8c619264u); r[2]=rd(m,bus,&io_status,0x8c619304u,4);if(io_status)return io_status;goto L8c619266; /* mov.l 0x8c619304,r2 */
L8c619266: sa_motion_publish_stop(ctx,0x8c619266u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619268; /* float FPUL,fr3 */
L8c619268: sa_motion_publish_stop(ctx,0x8c619268u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61926a; /* fmul fr6,fr3 */
L8c61926a: sa_motion_publish_stop(ctx,0x8c61926au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61926c; /* fmov.s fr3,@-r8 */
L8c61926c: sa_motion_publish_stop(ctx,0x8c61926cu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61926e; /* fmov.s @(r0,r5),fr3 */
L8c61926e: sa_motion_publish_stop(ctx,0x8c61926eu); r[0]=20u;goto L8c619270; /* mov #0x14,r0 */
L8c619270: sa_motion_publish_stop(ctx,0x8c619270u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619272; /* fmov.s fr3,@-r8 */
L8c619272: sa_motion_publish_stop(ctx,0x8c619272u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619274; /* fmov.s @(r0,r5),fr3 */
L8c619274: sa_motion_publish_stop(ctx,0x8c619274u); r[0]=16u;goto L8c619276; /* mov #0x10,r0 */
L8c619276: sa_motion_publish_stop(ctx,0x8c619276u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619278; /* fmov.s fr3,@-r8 */
L8c619278: sa_motion_publish_stop(ctx,0x8c619278u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61927a; /* fmov.s @(r0,r5),fr3 */
L8c61927a: sa_motion_publish_stop(ctx,0x8c61927au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61927c; /* fmov.s fr3,@-r8 */
L8c61927c: sa_motion_publish_stop(ctx,0x8c61927cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61927e; /* mov.l r2,@-r8 */
L8c61927e: sa_motion_publish_stop(ctx,0x8c61927eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619280; /* pref @r8 */
L8c619280: sa_motion_publish_stop(ctx,0x8c619280u); r[8]+=32u;goto L8c619282; /* add #0x20,r8 */
L8c619282: sa_motion_publish_stop(ctx,0x8c619282u); r[9]+=1u;goto L8c619284; /* add #0x1,r9 */
L8c619284: sa_motion_publish_stop(ctx,0x8c619284u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c619286; /* mov.w @(0x2,r4),r0 */
L8c619286: sa_motion_publish_stop(ctx,0x8c619286u); r[0]=r[0]&65535u;goto L8c619288; /* extu.w r0,r0 */
L8c619288: sa_motion_publish_stop(ctx,0x8c619288u); *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61928a; /* cmp/ge r0,r9 */
L8c61928a: sa_motion_publish_stop(ctx,0x8c61928au); if((*sr&1u))goto L8c619290;goto L8c61928c; /* bt 0x8c619290 */
L8c61928c: sa_motion_publish_stop(ctx,0x8c61928eu); goto L8c619128; /* bra 0x8c619128 */
L8c619290: sa_motion_publish_stop(ctx,0x8c619292u); goto L8c6192d0; /* bra 0x8c6192d0 */
L8c619294: sa_motion_publish_stop(ctx,0x8c619294u); r[1]=rd(m,bus,&io_status,0x8c619308u,4);if(io_status)return io_status;goto L8c619296; /* mov.l 0x8c619308,r1 */
L8c619296: sa_motion_publish_stop(ctx,0x8c619296u); wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c619298; /* mov.l r4,@r1 */
L8c619298: sa_motion_publish_stop(ctx,0x8c619298u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61929a; /* frchg */
L8c61929a: sa_motion_publish_stop(ctx,0x8c61929au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61929c; /* fmov.s fr0,@-r15 */
L8c61929c: sa_motion_publish_stop(ctx,0x8c61929cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61929e; /* fmov.s fr1,@-r15 */
L8c61929e: sa_motion_publish_stop(ctx,0x8c61929eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c6192a0; /* fmov.s fr2,@-r15 */
L8c6192a0: sa_motion_publish_stop(ctx,0x8c6192a0u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c6192a2; /* fmov.s fr3,@-r15 */
L8c6192a2: sa_motion_publish_stop(ctx,0x8c6192a2u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c6192a4; /* fmov.s fr4,@-r15 */
L8c6192a4: sa_motion_publish_stop(ctx,0x8c6192a4u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c6192a6; /* fmov.s fr5,@-r15 */
L8c6192a6: sa_motion_publish_stop(ctx,0x8c6192a6u); r[0]=rd(m,bus,&io_status,0x8c6192e0u,4);if(io_status)return io_status;goto L8c6192a8; /* mov.l 0x8c6192e0,r0 */
L8c6192a8: sa_motion_publish_stop(ctx,0x8c6192a8u); fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192aa; /* fmov.s @r0+,fr0 */
L8c6192aa: sa_motion_publish_stop(ctx,0x8c6192aau); fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192ac; /* fmov.s @r0+,fr1 */
L8c6192ac: sa_motion_publish_stop(ctx,0x8c6192acu); fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c6192ae; /* fmov.s @r0,fr2 */
L8c6192ae: sa_motion_publish_stop(ctx,0x8c6192aeu); r[0]=rd(m,bus,&io_status,0x8c6192e4u,4);if(io_status)return io_status;goto L8c6192b0; /* mov.l 0x8c6192e4,r0 */
L8c6192b0: sa_motion_publish_stop(ctx,0x8c6192b0u); fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c6192b2; /* fmov.s @r0,fr3 */
L8c6192b2: sa_motion_publish_stop(ctx,0x8c6192b2u); r[0]=0x8c6192e8u;goto L8c6192b4; /* mova 0x8c6192e8,r0 */
L8c6192b4: sa_motion_publish_stop(ctx,0x8c6192b4u); fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c6192b6; /* fmov.s @r0,fr4 */
L8c6192b6: sa_motion_publish_stop(ctx,0x8c6192b6u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6192b8; /* frchg */
L8c6192b8: sa_motion_publish_stop(ctx,0x8c6192b8u); r[2]=rd(m,bus,&io_status,0x8c61930cu,4);if(io_status)return io_status;goto L8c6192ba; /* mov.l 0x8c61930c,r2 */
L8c6192ba: sa_motion_publish_stop(ctx,0x8c6192bau); r[3]=rd(m,bus,&io_status,0x8c619308u,4);if(io_status)return io_status;goto L8c6192bc; /* mov.l 0x8c619308,r3 */
L8c6192bc: sa_motion_publish_stop(ctx,0x8c6192beu); target=r[2];*pr=0x8c6192c0u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6192c0; /* jsr @r2 */
L8c6192c0: sa_motion_publish_stop(ctx,0x8c6192c0u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6192c2; /* frchg */
L8c6192c2: sa_motion_publish_stop(ctx,0x8c6192c2u); fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192c4; /* fmov.s @r15+,fr5 */
L8c6192c4: sa_motion_publish_stop(ctx,0x8c6192c4u); fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192c6; /* fmov.s @r15+,fr4 */
L8c6192c6: sa_motion_publish_stop(ctx,0x8c6192c6u); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192c8; /* fmov.s @r15+,fr3 */
L8c6192c8: sa_motion_publish_stop(ctx,0x8c6192c8u); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192ca; /* fmov.s @r15+,fr2 */
L8c6192ca: sa_motion_publish_stop(ctx,0x8c6192cau); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192cc; /* fmov.s @r15+,fr1 */
L8c6192cc: sa_motion_publish_stop(ctx,0x8c6192ccu); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6192ce; /* fmov.s @r15+,fr0 */
L8c6192ce: sa_motion_publish_stop(ctx,0x8c6192ceu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6192d0; /* frchg */
L8c6192d0: sa_motion_publish_stop(ctx,0x8c6192d0u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6192d2; /* lds.l @r15+,PR */
L8c6192d2: sa_motion_publish_stop(ctx,0x8c6192d2u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6192d4; /* mov.l @r15+,r9 */
L8c6192d4: sa_motion_publish_stop(ctx,0x8c6192d4u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6192d6; /* mov.l @r15+,r10 */
L8c6192d6: sa_motion_publish_stop(ctx,0x8c6192d6u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6192d8; /* mov.l @r15+,r11 */
L8c6192d8: sa_motion_publish_stop(ctx,0x8c6192d8u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6192da; /* mov.l @r15+,r12 */
L8c6192da: sa_motion_publish_stop(ctx,0x8c6192dau); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6192dc; /* mov.l @r15+,r13 */
L8c6192dc: sa_motion_publish_stop(ctx,0x8c6192deu); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_uv_lit_primitive_clipped_8c619310(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c619310;
L8c619310: sa_motion_publish_stop(ctx,0x8c619310u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c619312; /* mov.l r14,@-r15 */
L8c619312: sa_motion_publish_stop(ctx,0x8c619312u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c619314; /* mov.l r13,@-r15 */
L8c619314: sa_motion_publish_stop(ctx,0x8c619314u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c619316; /* mov.l r12,@-r15 */
L8c619316: sa_motion_publish_stop(ctx,0x8c619316u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c619318; /* mov.l r11,@-r15 */
L8c619318: sa_motion_publish_stop(ctx,0x8c619318u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61931a; /* mov.l r10,@-r15 */
L8c61931a: sa_motion_publish_stop(ctx,0x8c61931au); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61931c; /* mov.l r9,@-r15 */
L8c61931c: sa_motion_publish_stop(ctx,0x8c61931cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61931e; /* fmov.s fr15,@-r15 */
L8c61931e: sa_motion_publish_stop(ctx,0x8c61931eu); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c619320; /* sts.l PR,@-r15 */
L8c619320: sa_motion_publish_stop(ctx,0x8c619320u); r[15]+=4294967272u;goto L8c619322; /* add #-0x18,r15 */
L8c619322: sa_motion_publish_stop(ctx,0x8c619322u); r[0]=0x8c6193fcu;goto L8c619324; /* mova 0x8c6193fc,r0 */
L8c619324: sa_motion_publish_stop(ctx,0x8c619324u); r[3]=rd(m,bus,&io_status,0x8c619400u,4);if(io_status)return io_status;goto L8c619326; /* mov.l 0x8c619400,r3 */
L8c619326: sa_motion_publish_stop(ctx,0x8c619326u); fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c619328; /* fmov.s @r0,fr15 */
L8c619328: sa_motion_publish_stop(ctx,0x8c619328u); wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c61932a; /* mov.l r4,@(0x8,r15) */
L8c61932a: sa_motion_publish_stop(ctx,0x8c61932au); r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61932c; /* mov.w @r4,r0 */
L8c61932c: sa_motion_publish_stop(ctx,0x8c61932cu); r[11]=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;goto L8c61932e; /* mov.l @(0x14,r4),r11 */
L8c61932e: sa_motion_publish_stop(ctx,0x8c61932eu); r[0]=r[0]&65535u;goto L8c619330; /* extu.w r0,r0 */
L8c619330: sa_motion_publish_stop(ctx,0x8c619330u); r[0]&=r[3];goto L8c619332; /* and r3,r0 */
L8c619332: sa_motion_publish_stop(ctx,0x8c619332u); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c619334; /* cmp/eq #0x0,r0 */
L8c619334: sa_motion_publish_stop(ctx,0x8c619336u); take=(*sr&1u);r[10]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;if(take)goto L8c61935a;goto L8c619338; /* bt/s 0x8c61935a */
L8c619338: sa_motion_publish_stop(ctx,0x8c619338u); r[1]=sx(m,bus,&io_status,0x8c6193fau,2);if(io_status)return io_status;goto L8c61933a; /* mov.w 0x8c6193fa,r1 */
L8c61933a: sa_motion_publish_stop(ctx,0x8c61933au); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61933c; /* cmp/eq r1,r0 */
L8c61933c: sa_motion_publish_stop(ctx,0x8c61933cu); if(!(*sr&1u))goto L8c619342;goto L8c61933e; /* bf 0x8c619342 */
L8c61933e: sa_motion_publish_stop(ctx,0x8c619340u); goto L8c6195a0; /* bra 0x8c6195a0 */
L8c619342: sa_motion_publish_stop(ctx,0x8c619342u); r[1]=rd(m,bus,&io_status,0x8c619404u,4);if(io_status)return io_status;goto L8c619344; /* mov.l 0x8c619404,r1 */
L8c619344: sa_motion_publish_stop(ctx,0x8c619344u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c619346; /* cmp/eq r1,r0 */
L8c619346: sa_motion_publish_stop(ctx,0x8c619346u); if(!(*sr&1u))goto L8c61934c;goto L8c619348; /* bf 0x8c61934c */
L8c619348: sa_motion_publish_stop(ctx,0x8c61934au); goto L8c6198d2; /* bra 0x8c6198d2 */
L8c61934c: sa_motion_publish_stop(ctx,0x8c61934cu); r[1]=rd(m,bus,&io_status,0x8c619400u,4);if(io_status)return io_status;goto L8c61934e; /* mov.l 0x8c619400,r1 */
L8c61934e: sa_motion_publish_stop(ctx,0x8c61934eu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c619350; /* cmp/eq r1,r0 */
L8c619350: sa_motion_publish_stop(ctx,0x8c619350u); if(!(*sr&1u))goto L8c619356;goto L8c619352; /* bf 0x8c619356 */
L8c619352: sa_motion_publish_stop(ctx,0x8c619354u); goto L8c619ba4; /* bra 0x8c619ba4 */
L8c619356: sa_motion_publish_stop(ctx,0x8c619358u); goto L8c619be2; /* bra 0x8c619be2 */
L8c61935a: sa_motion_publish_stop(ctx,0x8c61935au); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61935c; /* mov.l @(0x8,r15),r2 */
L8c61935c: sa_motion_publish_stop(ctx,0x8c61935cu); r[13]=0u;goto L8c61935e; /* mov #0x0,r13 */
L8c61935e: sa_motion_publish_stop(ctx,0x8c61935eu); r[1]=rd(m,bus,&io_status,0x8c619408u,4);if(io_status)return io_status;goto L8c619360; /* mov.l 0x8c619408,r1 */
L8c619360: sa_motion_publish_stop(ctx,0x8c619360u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c619362; /* mov.w @(0x2,r2),r0 */
L8c619362: sa_motion_publish_stop(ctx,0x8c619362u); r[12]=rd(m,bus,&io_status,0x8c61940cu,4);if(io_status)return io_status;goto L8c619364; /* mov.l 0x8c61940c,r12 */
L8c619364: sa_motion_publish_stop(ctx,0x8c619364u); r[0]=r[0]&65535u;goto L8c619366; /* extu.w r0,r0 */
L8c619366: sa_motion_publish_stop(ctx,0x8c619366u); r[14]=rd(m,bus,&io_status,0x8c619410u,4);if(io_status)return io_status;goto L8c619368; /* mov.l 0x8c619410,r14 */
L8c619368: sa_motion_publish_stop(ctx,0x8c619368u); r[2]=r[0];goto L8c61936a; /* mov r0,r2 */
L8c61936a: sa_motion_publish_stop(ctx,0x8c61936au); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61936c; /* shll r0 */
L8c61936c: sa_motion_publish_stop(ctx,0x8c61936cu); r[0]+=r[2];goto L8c61936e; /* add r2,r0 */
L8c61936e: sa_motion_publish_stop(ctx,0x8c61936eu); r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c619370; /* mov.l @r1,r2 */
L8c619370: sa_motion_publish_stop(ctx,0x8c619370u); r[9]=r[12];goto L8c619372; /* mov r12,r9 */
L8c619372: sa_motion_publish_stop(ctx,0x8c619372u); r[2]+=r[0];goto L8c619374; /* add r0,r2 */
L8c619374: sa_motion_publish_stop(ctx,0x8c619374u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c619376; /* mov.l r2,@r1 */
L8c619376: sa_motion_publish_stop(ctx,0x8c619378u); r[9]+=4u;goto L8c61958e; /* bra 0x8c61958e */
L8c61937a: sa_motion_publish_stop(ctx,0x8c61937au); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61937c; /* mov.w @r10+,r4 */
L8c61937c: sa_motion_publish_stop(ctx,0x8c61937cu); r[2]=rd(m,bus,&io_status,0x8c619414u,4);if(io_status)return io_status;goto L8c61937e; /* mov.l 0x8c619414,r2 */
L8c61937e: sa_motion_publish_stop(ctx,0x8c61937eu); r[5]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c619380; /* mov.w @r10+,r5 */
L8c619380: sa_motion_publish_stop(ctx,0x8c619380u); r[4]=r[4]&65535u;goto L8c619382; /* extu.w r4,r4 */
L8c619382: sa_motion_publish_stop(ctx,0x8c619382u); r[4]<<=2;goto L8c619384; /* shll2 r4 */
L8c619384: sa_motion_publish_stop(ctx,0x8c619384u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c619386; /* mov.l @r2,r3 */
L8c619386: sa_motion_publish_stop(ctx,0x8c619386u); r[6]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c619388; /* mov.w @r10+,r6 */
L8c619388: sa_motion_publish_stop(ctx,0x8c619388u); r[5]=r[5]&65535u;goto L8c61938a; /* extu.w r5,r5 */
L8c61938a: sa_motion_publish_stop(ctx,0x8c61938au); r[5]<<=2;goto L8c61938c; /* shll2 r5 */
L8c61938c: sa_motion_publish_stop(ctx,0x8c61938cu); r[6]=r[6]&65535u;goto L8c61938e; /* extu.w r6,r6 */
L8c61938e: sa_motion_publish_stop(ctx,0x8c61938eu); r[6]<<=2;goto L8c619390; /* shll2 r6 */
L8c619390: sa_motion_publish_stop(ctx,0x8c619390u); r[4]<<=2;goto L8c619392; /* shll2 r4 */
L8c619392: sa_motion_publish_stop(ctx,0x8c619392u); r[6]<<=2;goto L8c619394; /* shll2 r6 */
L8c619394: sa_motion_publish_stop(ctx,0x8c619394u); r[5]<<=2;goto L8c619396; /* shll2 r5 */
L8c619396: sa_motion_publish_stop(ctx,0x8c619396u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c619398; /* shll r4 */
L8c619398: sa_motion_publish_stop(ctx,0x8c619398u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61939a; /* shll r5 */
L8c61939a: sa_motion_publish_stop(ctx,0x8c61939au); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61939c; /* shll r6 */
L8c61939c: sa_motion_publish_stop(ctx,0x8c61939cu); r[6]+=r[3];goto L8c61939e; /* add r3,r6 */
L8c61939e: sa_motion_publish_stop(ctx,0x8c61939eu); r[4]+=r[3];goto L8c6193a0; /* add r3,r4 */
L8c6193a0: sa_motion_publish_stop(ctx,0x8c6193a0u); r[1]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c6193a2; /* mov.l @(0xc,r6),r1 */
L8c6193a2: sa_motion_publish_stop(ctx,0x8c6193a2u); r[5]+=r[3];goto L8c6193a4; /* add r3,r5 */
L8c6193a4: sa_motion_publish_stop(ctx,0x8c6193a4u); r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6193a6; /* mov.l @(0xc,r4),r7 */
L8c6193a6: sa_motion_publish_stop(ctx,0x8c6193a6u); r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c6193a8; /* mov.l @(0xc,r5),r3 */
L8c6193a8: sa_motion_publish_stop(ctx,0x8c6193a8u); r[7]&=r[3];goto L8c6193aa; /* and r3,r7 */
L8c6193aa: sa_motion_publish_stop(ctx,0x8c6193aau); r[3]=63u;goto L8c6193ac; /* mov #0x3f,r3 */
L8c6193ac: sa_motion_publish_stop(ctx,0x8c6193acu); r[7]&=r[1];goto L8c6193ae; /* and r1,r7 */
L8c6193ae: sa_motion_publish_stop(ctx,0x8c6193aeu); *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c6193b0; /* tst r3,r7 */
L8c6193b0: sa_motion_publish_stop(ctx,0x8c6193b0u); if(!(*sr&1u))goto L8c6193f2;goto L8c6193b2; /* bf 0x8c6193f2 */
L8c6193b2: sa_motion_publish_stop(ctx,0x8c6193b2u); r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6193b4; /* mov.l @(0xc,r4),r7 */
L8c6193b4: sa_motion_publish_stop(ctx,0x8c6193b4u); r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c6193b6; /* mov.l @(0xc,r5),r3 */
L8c6193b6: sa_motion_publish_stop(ctx,0x8c6193b6u); r[2]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c6193b8; /* mov.l @(0xc,r6),r2 */
L8c6193b8: sa_motion_publish_stop(ctx,0x8c6193b8u); r[7]|=r[3];goto L8c6193ba; /* or r3,r7 */
L8c6193ba: sa_motion_publish_stop(ctx,0x8c6193bau); r[3]=1u;goto L8c6193bc; /* mov #0x1,r3 */
L8c6193bc: sa_motion_publish_stop(ctx,0x8c6193bcu); r[7]|=r[2];goto L8c6193be; /* or r2,r7 */
L8c6193be: sa_motion_publish_stop(ctx,0x8c6193beu); *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c6193c0; /* tst r3,r7 */
L8c6193c0: sa_motion_publish_stop(ctx,0x8c6193c0u); if((*sr&1u))goto L8c6193c6;goto L8c6193c2; /* bt 0x8c6193c6 */
L8c6193c2: sa_motion_publish_stop(ctx,0x8c6193c4u); goto L8c61952e; /* bra 0x8c61952e */
L8c6193c6: sa_motion_publish_stop(ctx,0x8c6193c6u); r[0]=16u;goto L8c6193c8; /* mov #0x10,r0 */
L8c6193c8: sa_motion_publish_stop(ctx,0x8c6193c8u); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c6193ca; /* fmov.s @(r0,r5),fr5 */
L8c6193ca: sa_motion_publish_stop(ctx,0x8c6193cau); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6193cc; /* fmov.s @(r0,r4),fr3 */
L8c6193cc: sa_motion_publish_stop(ctx,0x8c6193ccu); r[0]=20u;goto L8c6193ce; /* mov #0x14,r0 */
L8c6193ce: sa_motion_publish_stop(ctx,0x8c6193ceu); fcopy(f,2u,5u);goto L8c6193d0; /* fmov fr5,fr2 */
L8c6193d0: sa_motion_publish_stop(ctx,0x8c6193d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6193d2; /* fsub fr3,fr2 */
L8c6193d2: sa_motion_publish_stop(ctx,0x8c6193d2u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6193d4; /* fmov.s @(r0,r6),fr3 */
L8c6193d4: sa_motion_publish_stop(ctx,0x8c6193d4u); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c6193d6; /* fmov.s @(r0,r5),fr4 */
L8c6193d6: sa_motion_publish_stop(ctx,0x8c6193d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6193d8; /* fsub fr4,fr3 */
L8c6193d8: sa_motion_publish_stop(ctx,0x8c6193d8u); fcopy(f,1u,4u);goto L8c6193da; /* fmov fr4,fr1 */
L8c6193da: sa_motion_publish_stop(ctx,0x8c6193dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6193dc; /* fmul fr3,fr2 */
L8c6193dc: sa_motion_publish_stop(ctx,0x8c6193dcu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6193de; /* fmov.s @(r0,r4),fr3 */
L8c6193de: sa_motion_publish_stop(ctx,0x8c6193deu); r[0]=16u;goto L8c6193e0; /* mov #0x10,r0 */
L8c6193e0: sa_motion_publish_stop(ctx,0x8c6193e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6193e2; /* fsub fr3,fr1 */
L8c6193e2: sa_motion_publish_stop(ctx,0x8c6193e2u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6193e4; /* fmov.s @(r0,r6),fr3 */
L8c6193e4: sa_motion_publish_stop(ctx,0x8c6193e4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6193e6; /* fsub fr5,fr3 */
L8c6193e6: sa_motion_publish_stop(ctx,0x8c6193e6u); fcopy(f,4u,2u);goto L8c6193e8; /* fmov fr2,fr4 */
L8c6193e8: sa_motion_publish_stop(ctx,0x8c6193e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6193ea; /* fmul fr3,fr1 */
L8c6193ea: sa_motion_publish_stop(ctx,0x8c6193eau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6193ec; /* fldi0 fr3 */
L8c6193ec: sa_motion_publish_stop(ctx,0x8c6193ecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c6193ee; /* fsub fr1,fr4 */
L8c6193ee: sa_motion_publish_stop(ctx,0x8c6193eeu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c6193f0; /* fcmp/gt fr3,fr4 */
L8c6193f0: sa_motion_publish_stop(ctx,0x8c6193f0u); if(!(*sr&1u))goto L8c619418;goto L8c6193f2; /* bf 0x8c619418 */
L8c6193f2: sa_motion_publish_stop(ctx,0x8c6193f4u); r[11]+=12u;goto L8c61958c; /* bra 0x8c61958c */
L8c619418: sa_motion_publish_stop(ctx,0x8c619418u); r[3]=rd(m,bus,&io_status,0x8c61965cu,4);if(io_status)return io_status;goto L8c61941a; /* mov.l 0x8c61965c,r3 */
L8c61941a: sa_motion_publish_stop(ctx,0x8c61941au); r[0]=4u;goto L8c61941c; /* mov #0x4,r0 */
L8c61941c: sa_motion_publish_stop(ctx,0x8c61941cu); fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61941e; /* fmov.s @r4,fr3 */
L8c61941e: sa_motion_publish_stop(ctx,0x8c61941eu); r[8]+=28u;goto L8c619420; /* add #0x1c,r8 */
L8c619420: sa_motion_publish_stop(ctx,0x8c619420u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c619422; /* fmov.s @r3,fr2 */
L8c619422: sa_motion_publish_stop(ctx,0x8c619422u); r[3]+=8u;goto L8c619424; /* add #0x8,r3 */
L8c619424: sa_motion_publish_stop(ctx,0x8c619424u); r[2]=rd(m,bus,&io_status,0x8c619660u,4);if(io_status)return io_status;goto L8c619426; /* mov.l 0x8c619660,r2 */
L8c619426: sa_motion_publish_stop(ctx,0x8c619426u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619428; /* fmul fr3,fr2 */
L8c619428: sa_motion_publish_stop(ctx,0x8c619428u); r[1]=rd(m,bus,&io_status,0x8c619664u,4);if(io_status)return io_status;goto L8c61942a; /* mov.l 0x8c619664,r1 */
L8c61942a: sa_motion_publish_stop(ctx,0x8c61942au); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61942c; /* fmov.s @r2,fr3 */
L8c61942c: sa_motion_publish_stop(ctx,0x8c61942cu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61942e; /* fmov.s @r1,fr1 */
L8c61942e: sa_motion_publish_stop(ctx,0x8c61942eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619430; /* fsub fr2,fr3 */
L8c619430: sa_motion_publish_stop(ctx,0x8c619430u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c619432; /* fmov.s @(r0,r4),fr2 */
L8c619432: sa_motion_publish_stop(ctx,0x8c619432u); r[0]=8u;goto L8c619434; /* mov #0x8,r0 */
L8c619434: sa_motion_publish_stop(ctx,0x8c619434u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619436; /* fmul fr2,fr1 */
L8c619436: sa_motion_publish_stop(ctx,0x8c619436u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c619438; /* fmov.s @(r0,r4),fr2 */
L8c619438: sa_motion_publish_stop(ctx,0x8c619438u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61943a; /* fsub fr1,fr3 */
L8c61943a: sa_motion_publish_stop(ctx,0x8c61943au); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61943c; /* fmov.s @r3,fr1 */
L8c61943c: sa_motion_publish_stop(ctx,0x8c61943cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61943e; /* fmul fr2,fr1 */
L8c61943e: sa_motion_publish_stop(ctx,0x8c61943eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619440; /* fsub fr1,fr3 */
L8c619440: sa_motion_publish_stop(ctx,0x8c619440u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619442; /* fmov.s fr3,@-r8 */
L8c619442: sa_motion_publish_stop(ctx,0x8c619442u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619444; /* mov.w @(0x2,r11),r0 */
L8c619444: sa_motion_publish_stop(ctx,0x8c619444u); r[2]=r[0];goto L8c619446; /* mov r0,r2 */
L8c619446: sa_motion_publish_stop(ctx,0x8c619446u); *fpul=r[2];goto L8c619448; /* lds r2,FPUL */
L8c619448: sa_motion_publish_stop(ctx,0x8c619448u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61944a; /* float FPUL,fr3 */
L8c61944a: sa_motion_publish_stop(ctx,0x8c61944au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61944c; /* fmul fr15,fr3 */
L8c61944c: sa_motion_publish_stop(ctx,0x8c61944cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61944e; /* fmov.s fr3,@-r8 */
L8c61944e: sa_motion_publish_stop(ctx,0x8c61944eu); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619450; /* mov.w @r11,r2 */
L8c619450: sa_motion_publish_stop(ctx,0x8c619450u); *fpul=r[2];goto L8c619452; /* lds r2,FPUL */
L8c619452: sa_motion_publish_stop(ctx,0x8c619452u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619454; /* float FPUL,fr3 */
L8c619454: sa_motion_publish_stop(ctx,0x8c619454u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619456; /* fmul fr15,fr3 */
L8c619456: sa_motion_publish_stop(ctx,0x8c619456u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619458; /* fmov.s fr3,@-r8 */
L8c619458: sa_motion_publish_stop(ctx,0x8c619458u); r[0]=24u;goto L8c61945a; /* mov #0x18,r0 */
L8c61945a: sa_motion_publish_stop(ctx,0x8c61945au); r[2]=rd(m,bus,&io_status,0x8c619668u,4);if(io_status)return io_status;goto L8c61945c; /* mov.l 0x8c619668,r2 */
L8c61945c: sa_motion_publish_stop(ctx,0x8c61945cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61945e; /* fmov.s @(r0,r4),fr3 */
L8c61945e: sa_motion_publish_stop(ctx,0x8c61945eu); r[0]=20u;goto L8c619460; /* mov #0x14,r0 */
L8c619460: sa_motion_publish_stop(ctx,0x8c619460u); r[11]+=4u;goto L8c619462; /* add #0x4,r11 */
L8c619462: sa_motion_publish_stop(ctx,0x8c619462u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619464; /* fmov.s fr3,@-r8 */
L8c619464: sa_motion_publish_stop(ctx,0x8c619464u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619466; /* fmov.s @(r0,r4),fr3 */
L8c619466: sa_motion_publish_stop(ctx,0x8c619466u); r[0]=16u;goto L8c619468; /* mov #0x10,r0 */
L8c619468: sa_motion_publish_stop(ctx,0x8c619468u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61946a; /* fmov.s fr3,@-r8 */
L8c61946a: sa_motion_publish_stop(ctx,0x8c61946au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61946c; /* fmov.s @(r0,r4),fr3 */
L8c61946c: sa_motion_publish_stop(ctx,0x8c61946cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61946e; /* fmov.s fr3,@-r8 */
L8c61946e: sa_motion_publish_stop(ctx,0x8c61946eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c619470; /* mov.l r2,@-r8 */
L8c619470: sa_motion_publish_stop(ctx,0x8c619470u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619472; /* pref @r8 */
L8c619472: sa_motion_publish_stop(ctx,0x8c619472u); r[2]=rd(m,bus,&io_status,0x8c61965cu,4);if(io_status)return io_status;goto L8c619474; /* mov.l 0x8c61965c,r2 */
L8c619474: sa_motion_publish_stop(ctx,0x8c619474u); r[0]=4u;goto L8c619476; /* mov #0x4,r0 */
L8c619476: sa_motion_publish_stop(ctx,0x8c619476u); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c619478; /* fmov.s @r5,fr3 */
L8c619478: sa_motion_publish_stop(ctx,0x8c619478u); r[8]+=60u;goto L8c61947a; /* add #0x3c,r8 */
L8c61947a: sa_motion_publish_stop(ctx,0x8c61947au); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61947c; /* fmov.s @r2,fr2 */
L8c61947c: sa_motion_publish_stop(ctx,0x8c61947cu); r[3]=rd(m,bus,&io_status,0x8c619660u,4);if(io_status)return io_status;goto L8c61947e; /* mov.l 0x8c619660,r3 */
L8c61947e: sa_motion_publish_stop(ctx,0x8c61947eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619480; /* fmul fr3,fr2 */
L8c619480: sa_motion_publish_stop(ctx,0x8c619480u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c619482; /* fmov.s @r1,fr1 */
L8c619482: sa_motion_publish_stop(ctx,0x8c619482u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c619484; /* fmov.s @r3,fr3 */
L8c619484: sa_motion_publish_stop(ctx,0x8c619484u); r[3]=rd(m,bus,&io_status,0x8c61966cu,4);if(io_status)return io_status;goto L8c619486; /* mov.l 0x8c61966c,r3 */
L8c619486: sa_motion_publish_stop(ctx,0x8c619486u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619488; /* fsub fr2,fr3 */
L8c619488: sa_motion_publish_stop(ctx,0x8c619488u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61948a; /* fmov.s @(r0,r5),fr2 */
L8c61948a: sa_motion_publish_stop(ctx,0x8c61948au); r[0]=8u;goto L8c61948c; /* mov #0x8,r0 */
L8c61948c: sa_motion_publish_stop(ctx,0x8c61948cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61948e; /* fmul fr2,fr1 */
L8c61948e: sa_motion_publish_stop(ctx,0x8c61948eu); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c619490; /* fmov.s @(r0,r5),fr2 */
L8c619490: sa_motion_publish_stop(ctx,0x8c619490u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619492; /* fsub fr1,fr3 */
L8c619492: sa_motion_publish_stop(ctx,0x8c619492u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619494; /* fmov.s @r3,fr1 */
L8c619494: sa_motion_publish_stop(ctx,0x8c619494u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619496; /* fmul fr2,fr1 */
L8c619496: sa_motion_publish_stop(ctx,0x8c619496u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619498; /* fsub fr1,fr3 */
L8c619498: sa_motion_publish_stop(ctx,0x8c619498u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61949a; /* fmov.s fr3,@-r8 */
L8c61949a: sa_motion_publish_stop(ctx,0x8c61949au); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61949c; /* mov.w @(0x2,r11),r0 */
L8c61949c: sa_motion_publish_stop(ctx,0x8c61949cu); r[2]=r[0];goto L8c61949e; /* mov r0,r2 */
L8c61949e: sa_motion_publish_stop(ctx,0x8c61949eu); *fpul=r[2];goto L8c6194a0; /* lds r2,FPUL */
L8c6194a0: sa_motion_publish_stop(ctx,0x8c6194a0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6194a2; /* float FPUL,fr3 */
L8c6194a2: sa_motion_publish_stop(ctx,0x8c6194a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6194a4; /* fmul fr15,fr3 */
L8c6194a4: sa_motion_publish_stop(ctx,0x8c6194a4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6194a6; /* fmov.s fr3,@-r8 */
L8c6194a6: sa_motion_publish_stop(ctx,0x8c6194a6u); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c6194a8; /* mov.w @r11,r2 */
L8c6194a8: sa_motion_publish_stop(ctx,0x8c6194a8u); r[11]+=4u;goto L8c6194aa; /* add #0x4,r11 */
L8c6194aa: sa_motion_publish_stop(ctx,0x8c6194aau); *fpul=r[2];goto L8c6194ac; /* lds r2,FPUL */
L8c6194ac: sa_motion_publish_stop(ctx,0x8c6194acu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6194ae; /* float FPUL,fr3 */
L8c6194ae: sa_motion_publish_stop(ctx,0x8c6194aeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6194b0; /* fmul fr15,fr3 */
L8c6194b0: sa_motion_publish_stop(ctx,0x8c6194b0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6194b2; /* fmov.s fr3,@-r8 */
L8c6194b2: sa_motion_publish_stop(ctx,0x8c6194b2u); r[0]=24u;goto L8c6194b4; /* mov #0x18,r0 */
L8c6194b4: sa_motion_publish_stop(ctx,0x8c6194b4u); r[2]=rd(m,bus,&io_status,0x8c619668u,4);if(io_status)return io_status;goto L8c6194b6; /* mov.l 0x8c619668,r2 */
L8c6194b6: sa_motion_publish_stop(ctx,0x8c6194b6u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6194b8; /* fmov.s @(r0,r5),fr3 */
L8c6194b8: sa_motion_publish_stop(ctx,0x8c6194b8u); r[0]=20u;goto L8c6194ba; /* mov #0x14,r0 */
L8c6194ba: sa_motion_publish_stop(ctx,0x8c6194bau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6194bc; /* fmov.s fr3,@-r8 */
L8c6194bc: sa_motion_publish_stop(ctx,0x8c6194bcu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6194be; /* fmov.s @(r0,r5),fr3 */
L8c6194be: sa_motion_publish_stop(ctx,0x8c6194beu); r[0]=16u;goto L8c6194c0; /* mov #0x10,r0 */
L8c6194c0: sa_motion_publish_stop(ctx,0x8c6194c0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6194c2; /* fmov.s fr3,@-r8 */
L8c6194c2: sa_motion_publish_stop(ctx,0x8c6194c2u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6194c4; /* fmov.s @(r0,r5),fr3 */
L8c6194c4: sa_motion_publish_stop(ctx,0x8c6194c4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6194c6; /* fmov.s fr3,@-r8 */
L8c6194c6: sa_motion_publish_stop(ctx,0x8c6194c6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c6194c8; /* mov.l r2,@-r8 */
L8c6194c8: sa_motion_publish_stop(ctx,0x8c6194c8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6194ca; /* pref @r8 */
L8c6194ca: sa_motion_publish_stop(ctx,0x8c6194cau); r[2]=rd(m,bus,&io_status,0x8c61965cu,4);if(io_status)return io_status;goto L8c6194cc; /* mov.l 0x8c61965c,r2 */
L8c6194cc: sa_motion_publish_stop(ctx,0x8c6194ccu); r[0]=4u;goto L8c6194ce; /* mov #0x4,r0 */
L8c6194ce: sa_motion_publish_stop(ctx,0x8c6194ceu); fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c6194d0; /* fmov.s @r6,fr3 */
L8c6194d0: sa_motion_publish_stop(ctx,0x8c6194d0u); r[8]+=60u;goto L8c6194d2; /* add #0x3c,r8 */
L8c6194d2: sa_motion_publish_stop(ctx,0x8c6194d2u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c6194d4; /* fmov.s @r2,fr2 */
L8c6194d4: sa_motion_publish_stop(ctx,0x8c6194d4u); r[3]=rd(m,bus,&io_status,0x8c619660u,4);if(io_status)return io_status;goto L8c6194d6; /* mov.l 0x8c619660,r3 */
L8c6194d6: sa_motion_publish_stop(ctx,0x8c6194d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6194d8; /* fmul fr3,fr2 */
L8c6194d8: sa_motion_publish_stop(ctx,0x8c6194d8u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c6194da; /* fmov.s @r1,fr1 */
L8c6194da: sa_motion_publish_stop(ctx,0x8c6194dau); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6194dc; /* fmov.s @r3,fr3 */
L8c6194dc: sa_motion_publish_stop(ctx,0x8c6194dcu); r[3]=rd(m,bus,&io_status,0x8c61966cu,4);if(io_status)return io_status;goto L8c6194de; /* mov.l 0x8c61966c,r3 */
L8c6194de: sa_motion_publish_stop(ctx,0x8c6194deu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6194e0; /* fsub fr2,fr3 */
L8c6194e0: sa_motion_publish_stop(ctx,0x8c6194e0u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c6194e2; /* fmov.s @(r0,r6),fr2 */
L8c6194e2: sa_motion_publish_stop(ctx,0x8c6194e2u); r[0]=8u;goto L8c6194e4; /* mov #0x8,r0 */
L8c6194e4: sa_motion_publish_stop(ctx,0x8c6194e4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6194e6; /* fmul fr2,fr1 */
L8c6194e6: sa_motion_publish_stop(ctx,0x8c6194e6u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c6194e8; /* fmov.s @(r0,r6),fr2 */
L8c6194e8: sa_motion_publish_stop(ctx,0x8c6194e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6194ea; /* fsub fr1,fr3 */
L8c6194ea: sa_motion_publish_stop(ctx,0x8c6194eau); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6194ec; /* fmov.s @r3,fr1 */
L8c6194ec: sa_motion_publish_stop(ctx,0x8c6194ecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6194ee; /* fmul fr2,fr1 */
L8c6194ee: sa_motion_publish_stop(ctx,0x8c6194eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6194f0; /* fsub fr1,fr3 */
L8c6194f0: sa_motion_publish_stop(ctx,0x8c6194f0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6194f2; /* fmov.s fr3,@-r8 */
L8c6194f2: sa_motion_publish_stop(ctx,0x8c6194f2u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c6194f4; /* mov.w @(0x2,r11),r0 */
L8c6194f4: sa_motion_publish_stop(ctx,0x8c6194f4u); r[2]=r[0];goto L8c6194f6; /* mov r0,r2 */
L8c6194f6: sa_motion_publish_stop(ctx,0x8c6194f6u); *fpul=r[2];goto L8c6194f8; /* lds r2,FPUL */
L8c6194f8: sa_motion_publish_stop(ctx,0x8c6194f8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6194fa; /* float FPUL,fr3 */
L8c6194fa: sa_motion_publish_stop(ctx,0x8c6194fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6194fc; /* fmul fr15,fr3 */
L8c6194fc: sa_motion_publish_stop(ctx,0x8c6194fcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6194fe; /* fmov.s fr3,@-r8 */
L8c6194fe: sa_motion_publish_stop(ctx,0x8c6194feu); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619500; /* mov.w @r11,r2 */
L8c619500: sa_motion_publish_stop(ctx,0x8c619500u); r[11]+=4u;goto L8c619502; /* add #0x4,r11 */
L8c619502: sa_motion_publish_stop(ctx,0x8c619502u); *fpul=r[2];goto L8c619504; /* lds r2,FPUL */
L8c619504: sa_motion_publish_stop(ctx,0x8c619504u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619506; /* float FPUL,fr3 */
L8c619506: sa_motion_publish_stop(ctx,0x8c619506u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619508; /* fmul fr15,fr3 */
L8c619508: sa_motion_publish_stop(ctx,0x8c619508u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61950a; /* fmov.s fr3,@-r8 */
L8c61950a: sa_motion_publish_stop(ctx,0x8c61950au); r[0]=24u;goto L8c61950c; /* mov #0x18,r0 */
L8c61950c: sa_motion_publish_stop(ctx,0x8c61950cu); r[2]=rd(m,bus,&io_status,0x8c619670u,4);if(io_status)return io_status;goto L8c61950e; /* mov.l 0x8c619670,r2 */
L8c61950e: sa_motion_publish_stop(ctx,0x8c61950eu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619510; /* fmov.s @(r0,r6),fr3 */
L8c619510: sa_motion_publish_stop(ctx,0x8c619510u); r[0]=20u;goto L8c619512; /* mov #0x14,r0 */
L8c619512: sa_motion_publish_stop(ctx,0x8c619512u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619514; /* fmov.s fr3,@-r8 */
L8c619514: sa_motion_publish_stop(ctx,0x8c619514u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619516; /* fmov.s @(r0,r6),fr3 */
L8c619516: sa_motion_publish_stop(ctx,0x8c619516u); r[0]=16u;goto L8c619518; /* mov #0x10,r0 */
L8c619518: sa_motion_publish_stop(ctx,0x8c619518u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61951a; /* fmov.s fr3,@-r8 */
L8c61951a: sa_motion_publish_stop(ctx,0x8c61951au); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61951c; /* fmov.s @(r0,r6),fr3 */
L8c61951c: sa_motion_publish_stop(ctx,0x8c61951cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61951e; /* fmov.s fr3,@-r8 */
L8c61951e: sa_motion_publish_stop(ctx,0x8c61951eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c619520; /* mov.l r2,@-r8 */
L8c619520: sa_motion_publish_stop(ctx,0x8c619520u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619522; /* pref @r8 */
L8c619522: sa_motion_publish_stop(ctx,0x8c619522u); r[0]=rd(m,bus,&io_status,0x8c619674u,4);if(io_status)return io_status;goto L8c619524; /* mov.l 0x8c619674,r0 */
L8c619524: sa_motion_publish_stop(ctx,0x8c619524u); r[8]+=32u;goto L8c619526; /* add #0x20,r8 */
L8c619526: sa_motion_publish_stop(ctx,0x8c619526u); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c619528; /* mov.l @r0,r2 */
L8c619528: sa_motion_publish_stop(ctx,0x8c619528u); r[2]+=3u;goto L8c61952a; /* add #0x3,r2 */
L8c61952a: sa_motion_publish_stop(ctx,0x8c61952cu); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61958c; /* bra 0x8c61958c */
L8c61952e: sa_motion_publish_stop(ctx,0x8c61952eu); r[1]=rd(m,bus,&io_status,0x8c619678u,4);if(io_status)return io_status;goto L8c619530; /* mov.l 0x8c619678,r1 */
L8c619530: sa_motion_publish_stop(ctx,0x8c619530u); wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c619532; /* mov.l r3,@r1 */
L8c619532: sa_motion_publish_stop(ctx,0x8c619532u); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619534; /* mov.w @r11,r3 */
L8c619534: sa_motion_publish_stop(ctx,0x8c619534u); *fpul=r[3];goto L8c619536; /* lds r3,FPUL */
L8c619536: sa_motion_publish_stop(ctx,0x8c619536u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619538; /* float FPUL,fr3 */
L8c619538: sa_motion_publish_stop(ctx,0x8c619538u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61953a; /* ftrc fr3,FPUL */
L8c61953a: sa_motion_publish_stop(ctx,0x8c61953au); r[2]=*fpul;goto L8c61953c; /* sts FPUL,r2 */
L8c61953c: sa_motion_publish_stop(ctx,0x8c61953cu); wr(m,bus,&io_status,r[12],r[2],2);if(io_status)return io_status;goto L8c61953e; /* mov.w r2,@r12 */
L8c61953e: sa_motion_publish_stop(ctx,0x8c61953eu); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619540; /* mov.w @(0x2,r11),r0 */
L8c619540: sa_motion_publish_stop(ctx,0x8c619540u); r[11]+=4u;goto L8c619542; /* add #0x4,r11 */
L8c619542: sa_motion_publish_stop(ctx,0x8c619542u); r[3]=r[0];goto L8c619544; /* mov r0,r3 */
L8c619544: sa_motion_publish_stop(ctx,0x8c619544u); *fpul=r[3];goto L8c619546; /* lds r3,FPUL */
L8c619546: sa_motion_publish_stop(ctx,0x8c619546u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619548; /* float FPUL,fr3 */
L8c619548: sa_motion_publish_stop(ctx,0x8c619548u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61954a; /* ftrc fr3,FPUL */
L8c61954a: sa_motion_publish_stop(ctx,0x8c61954au); r[0]=*fpul;goto L8c61954c; /* sts FPUL,r0 */
L8c61954c: sa_motion_publish_stop(ctx,0x8c61954cu); wr(m,bus,&io_status,r[12]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61954e; /* mov.w r0,@(0x2,r12) */
L8c61954e: sa_motion_publish_stop(ctx,0x8c61954eu); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619550; /* mov.w @r11,r3 */
L8c619550: sa_motion_publish_stop(ctx,0x8c619550u); *fpul=r[3];goto L8c619552; /* lds r3,FPUL */
L8c619552: sa_motion_publish_stop(ctx,0x8c619552u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619554; /* float FPUL,fr3 */
L8c619554: sa_motion_publish_stop(ctx,0x8c619554u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619556; /* ftrc fr3,FPUL */
L8c619556: sa_motion_publish_stop(ctx,0x8c619556u); r[2]=*fpul;goto L8c619558; /* sts FPUL,r2 */
L8c619558: sa_motion_publish_stop(ctx,0x8c619558u); wr(m,bus,&io_status,r[9],r[2],2);if(io_status)return io_status;goto L8c61955a; /* mov.w r2,@r9 */
L8c61955a: sa_motion_publish_stop(ctx,0x8c61955au); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61955c; /* mov.w @(0x2,r11),r0 */
L8c61955c: sa_motion_publish_stop(ctx,0x8c61955cu); r[11]+=4u;goto L8c61955e; /* add #0x4,r11 */
L8c61955e: sa_motion_publish_stop(ctx,0x8c61955eu); r[3]=r[0];goto L8c619560; /* mov r0,r3 */
L8c619560: sa_motion_publish_stop(ctx,0x8c619560u); *fpul=r[3];goto L8c619562; /* lds r3,FPUL */
L8c619562: sa_motion_publish_stop(ctx,0x8c619562u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619564; /* float FPUL,fr3 */
L8c619564: sa_motion_publish_stop(ctx,0x8c619564u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619566; /* ftrc fr3,FPUL */
L8c619566: sa_motion_publish_stop(ctx,0x8c619566u); r[0]=*fpul;goto L8c619568; /* sts FPUL,r0 */
L8c619568: sa_motion_publish_stop(ctx,0x8c619568u); wr(m,bus,&io_status,r[9]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61956a; /* mov.w r0,@(0x2,r9) */
L8c61956a: sa_motion_publish_stop(ctx,0x8c61956au); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61956c; /* mov.w @r11,r3 */
L8c61956c: sa_motion_publish_stop(ctx,0x8c61956cu); *fpul=r[3];goto L8c61956e; /* lds r3,FPUL */
L8c61956e: sa_motion_publish_stop(ctx,0x8c61956eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619570; /* float FPUL,fr3 */
L8c619570: sa_motion_publish_stop(ctx,0x8c619570u); r[7]=rd(m,bus,&io_status,0x8c61967cu,4);if(io_status)return io_status;goto L8c619572; /* mov.l 0x8c61967c,r7 */
L8c619572: sa_motion_publish_stop(ctx,0x8c619572u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619574; /* ftrc fr3,FPUL */
L8c619574: sa_motion_publish_stop(ctx,0x8c619574u); r[2]=*fpul;goto L8c619576; /* sts FPUL,r2 */
L8c619576: sa_motion_publish_stop(ctx,0x8c619576u); wr(m,bus,&io_status,r[14],r[2],2);if(io_status)return io_status;goto L8c619578; /* mov.w r2,@r14 */
L8c619578: sa_motion_publish_stop(ctx,0x8c619578u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61957a; /* mov.w @(0x2,r11),r0 */
L8c61957a: sa_motion_publish_stop(ctx,0x8c61957au); r[3]=r[0];goto L8c61957c; /* mov r0,r3 */
L8c61957c: sa_motion_publish_stop(ctx,0x8c61957cu); *fpul=r[3];goto L8c61957e; /* lds r3,FPUL */
L8c61957e: sa_motion_publish_stop(ctx,0x8c61957eu); r[3]=rd(m,bus,&io_status,0x8c619680u,4);if(io_status)return io_status;goto L8c619580; /* mov.l 0x8c619680,r3 */
L8c619580: sa_motion_publish_stop(ctx,0x8c619580u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619582; /* float FPUL,fr3 */
L8c619582: sa_motion_publish_stop(ctx,0x8c619582u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619584; /* ftrc fr3,FPUL */
L8c619584: sa_motion_publish_stop(ctx,0x8c619584u); r[0]=*fpul;goto L8c619586; /* sts FPUL,r0 */
L8c619586: sa_motion_publish_stop(ctx,0x8c619586u); wr(m,bus,&io_status,r[14]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619588; /* mov.w r0,@(0x2,r14) */
L8c619588: sa_motion_publish_stop(ctx,0x8c61958au); target=r[3];*pr=0x8c61958cu;r[11]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61958c; /* jsr @r3 */
L8c61958c: sa_motion_publish_stop(ctx,0x8c61958cu); r[13]+=1u;goto L8c61958e; /* add #0x1,r13 */
L8c61958e: sa_motion_publish_stop(ctx,0x8c61958eu); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c619590; /* mov.l @(0x8,r15),r2 */
L8c619590: sa_motion_publish_stop(ctx,0x8c619590u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c619592; /* mov.w @(0x2,r2),r0 */
L8c619592: sa_motion_publish_stop(ctx,0x8c619592u); r[0]=r[0]&65535u;goto L8c619594; /* extu.w r0,r0 */
L8c619594: sa_motion_publish_stop(ctx,0x8c619594u); *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[0]);goto L8c619596; /* cmp/ge r0,r13 */
L8c619596: sa_motion_publish_stop(ctx,0x8c619596u); if((*sr&1u))goto L8c61959c;goto L8c619598; /* bt 0x8c61959c */
L8c619598: sa_motion_publish_stop(ctx,0x8c61959au); goto L8c61937a; /* bra 0x8c61937a */
L8c61959c: sa_motion_publish_stop(ctx,0x8c61959eu); goto L8c619be2; /* bra 0x8c619be2 */
L8c6195a0: sa_motion_publish_stop(ctx,0x8c6195a0u); r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6195a2; /* mov.l @(0x8,r15),r1 */
L8c6195a2: sa_motion_publish_stop(ctx,0x8c6195a2u); r[2]=rd(m,bus,&io_status,0x8c619684u,4);if(io_status)return io_status;goto L8c6195a4; /* mov.l 0x8c619684,r2 */
L8c6195a4: sa_motion_publish_stop(ctx,0x8c6195a4u); r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c6195a6; /* mov.w @(0x2,r1),r0 */
L8c6195a6: sa_motion_publish_stop(ctx,0x8c6195a6u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6195a8; /* mov.l @r2,r1 */
L8c6195a8: sa_motion_publish_stop(ctx,0x8c6195a8u); r[0]=r[0]&65535u;goto L8c6195aa; /* extu.w r0,r0 */
L8c6195aa: sa_motion_publish_stop(ctx,0x8c6195aau); r[9]=rd(m,bus,&io_status,0x8c61967cu,4);if(io_status)return io_status;goto L8c6195ac; /* mov.l 0x8c61967c,r9 */
L8c6195ac: sa_motion_publish_stop(ctx,0x8c6195acu); r[0]<<=2;goto L8c6195ae; /* shll2 r0 */
L8c6195ae: sa_motion_publish_stop(ctx,0x8c6195aeu); r[3]=rd(m,bus,&io_status,0x8c619688u,4);if(io_status)return io_status;goto L8c6195b0; /* mov.l 0x8c619688,r3 */
L8c6195b0: sa_motion_publish_stop(ctx,0x8c6195b0u); r[1]+=r[0];goto L8c6195b2; /* add r0,r1 */
L8c6195b2: sa_motion_publish_stop(ctx,0x8c6195b2u); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c6195b4; /* mov.l r1,@r2 */
L8c6195b4: sa_motion_publish_stop(ctx,0x8c6195b4u); r[2]=0u;goto L8c6195b6; /* mov #0x0,r2 */
L8c6195b6: sa_motion_publish_stop(ctx,0x8c6195b6u); wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c6195b8; /* mov.l r2,@(0x10,r15) */
L8c6195b8: sa_motion_publish_stop(ctx,0x8c6195b8u); r[2]=r[9];goto L8c6195ba; /* mov r9,r2 */
L8c6195ba: sa_motion_publish_stop(ctx,0x8c6195bau); r[1]=rd(m,bus,&io_status,0x8c61968cu,4);if(io_status)return io_status;goto L8c6195bc; /* mov.l 0x8c61968c,r1 */
L8c6195bc: sa_motion_publish_stop(ctx,0x8c6195bcu); r[2]+=4u;goto L8c6195be; /* add #0x4,r2 */
L8c6195be: sa_motion_publish_stop(ctx,0x8c6195beu); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6195c0; /* mov.l r2,@r15 */
L8c6195c0: sa_motion_publish_stop(ctx,0x8c6195c0u); wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6195c2; /* mov.l r3,@(0xc,r15) */
L8c6195c2: sa_motion_publish_stop(ctx,0x8c6195c4u); wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c6198be; /* bra 0x8c6198be */
L8c6195c6: sa_motion_publish_stop(ctx,0x8c6195c6u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c6195c8; /* mov.w @r10+,r4 */
L8c6195c8: sa_motion_publish_stop(ctx,0x8c6195c8u); r[2]=rd(m,bus,&io_status,0x8c619690u,4);if(io_status)return io_status;goto L8c6195ca; /* mov.l 0x8c619690,r2 */
L8c6195ca: sa_motion_publish_stop(ctx,0x8c6195cau); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c6195cc; /* mov.w @r10+,r14 */
L8c6195cc: sa_motion_publish_stop(ctx,0x8c6195ccu); r[4]=r[4]&65535u;goto L8c6195ce; /* extu.w r4,r4 */
L8c6195ce: sa_motion_publish_stop(ctx,0x8c6195ceu); r[4]<<=2;goto L8c6195d0; /* shll2 r4 */
L8c6195d0: sa_motion_publish_stop(ctx,0x8c6195d0u); r[6]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6195d2; /* mov.l @r2,r6 */
L8c6195d2: sa_motion_publish_stop(ctx,0x8c6195d2u); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c6195d4; /* mov.w @r10+,r13 */
L8c6195d4: sa_motion_publish_stop(ctx,0x8c6195d4u); r[14]=r[14]&65535u;goto L8c6195d6; /* extu.w r14,r14 */
L8c6195d6: sa_motion_publish_stop(ctx,0x8c6195d6u); r[14]<<=2;goto L8c6195d8; /* shll2 r14 */
L8c6195d8: sa_motion_publish_stop(ctx,0x8c6195d8u); r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c6195da; /* mov.w @r10+,r12 */
L8c6195da: sa_motion_publish_stop(ctx,0x8c6195dau); r[13]=r[13]&65535u;goto L8c6195dc; /* extu.w r13,r13 */
L8c6195dc: sa_motion_publish_stop(ctx,0x8c6195dcu); r[14]<<=2;goto L8c6195de; /* shll2 r14 */
L8c6195de: sa_motion_publish_stop(ctx,0x8c6195deu); r[4]<<=2;goto L8c6195e0; /* shll2 r4 */
L8c6195e0: sa_motion_publish_stop(ctx,0x8c6195e0u); r[12]=r[12]&65535u;goto L8c6195e2; /* extu.w r12,r12 */
L8c6195e2: sa_motion_publish_stop(ctx,0x8c6195e2u); r[13]<<=2;goto L8c6195e4; /* shll2 r13 */
L8c6195e4: sa_motion_publish_stop(ctx,0x8c6195e4u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6195e6; /* shll r4 */
L8c6195e6: sa_motion_publish_stop(ctx,0x8c6195e6u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c6195e8; /* shll r14 */
L8c6195e8: sa_motion_publish_stop(ctx,0x8c6195e8u); r[4]+=r[6];goto L8c6195ea; /* add r6,r4 */
L8c6195ea: sa_motion_publish_stop(ctx,0x8c6195eau); r[12]<<=2;goto L8c6195ec; /* shll2 r12 */
L8c6195ec: sa_motion_publish_stop(ctx,0x8c6195ecu); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6195ee; /* mov.l @(0xc,r4),r5 */
L8c6195ee: sa_motion_publish_stop(ctx,0x8c6195eeu); r[14]+=r[6];goto L8c6195f0; /* add r6,r14 */
L8c6195f0: sa_motion_publish_stop(ctx,0x8c6195f0u); r[13]<<=2;goto L8c6195f2; /* shll2 r13 */
L8c6195f2: sa_motion_publish_stop(ctx,0x8c6195f2u); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c6195f4; /* mov.l @(0xc,r14),r3 */
L8c6195f4: sa_motion_publish_stop(ctx,0x8c6195f4u); r[12]<<=2;goto L8c6195f6; /* shll2 r12 */
L8c6195f6: sa_motion_publish_stop(ctx,0x8c6195f6u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c6195f8; /* shll r13 */
L8c6195f8: sa_motion_publish_stop(ctx,0x8c6195f8u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c6195fa; /* shll r12 */
L8c6195fa: sa_motion_publish_stop(ctx,0x8c6195fau); r[13]+=r[6];goto L8c6195fc; /* add r6,r13 */
L8c6195fc: sa_motion_publish_stop(ctx,0x8c6195fcu); r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c6195fe; /* mov.l @(0xc,r13),r1 */
L8c6195fe: sa_motion_publish_stop(ctx,0x8c6195feu); r[5]&=r[3];goto L8c619600; /* and r3,r5 */
L8c619600: sa_motion_publish_stop(ctx,0x8c619600u); r[12]+=r[6];goto L8c619602; /* add r6,r12 */
L8c619602: sa_motion_publish_stop(ctx,0x8c619602u); r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c619604; /* mov.l @(0xc,r12),r3 */
L8c619604: sa_motion_publish_stop(ctx,0x8c619604u); r[5]&=r[1];goto L8c619606; /* and r1,r5 */
L8c619606: sa_motion_publish_stop(ctx,0x8c619606u); r[1]=63u;goto L8c619608; /* mov #0x3f,r1 */
L8c619608: sa_motion_publish_stop(ctx,0x8c619608u); r[5]&=r[3];goto L8c61960a; /* and r3,r5 */
L8c61960a: sa_motion_publish_stop(ctx,0x8c61960au); *sr=(*sr&~1u)|!(r[1]&r[5]);goto L8c61960c; /* tst r1,r5 */
L8c61960c: sa_motion_publish_stop(ctx,0x8c61960cu); if(!(*sr&1u))goto L8c619652;goto L8c61960e; /* bf 0x8c619652 */
L8c61960e: sa_motion_publish_stop(ctx,0x8c61960eu); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c619610; /* mov.l @(0xc,r4),r5 */
L8c619610: sa_motion_publish_stop(ctx,0x8c619610u); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c619612; /* mov.l @(0xc,r14),r3 */
L8c619612: sa_motion_publish_stop(ctx,0x8c619612u); r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c619614; /* mov.l @(0xc,r13),r2 */
L8c619614: sa_motion_publish_stop(ctx,0x8c619614u); r[5]|=r[3];goto L8c619616; /* or r3,r5 */
L8c619616: sa_motion_publish_stop(ctx,0x8c619616u); r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c619618; /* mov.l @(0xc,r12),r3 */
L8c619618: sa_motion_publish_stop(ctx,0x8c619618u); r[5]|=r[2];goto L8c61961a; /* or r2,r5 */
L8c61961a: sa_motion_publish_stop(ctx,0x8c61961au); r[2]=1u;goto L8c61961c; /* mov #0x1,r2 */
L8c61961c: sa_motion_publish_stop(ctx,0x8c61961cu); r[5]|=r[3];goto L8c61961e; /* or r3,r5 */
L8c61961e: sa_motion_publish_stop(ctx,0x8c61961eu); *sr=(*sr&~1u)|!(r[2]&r[5]);goto L8c619620; /* tst r2,r5 */
L8c619620: sa_motion_publish_stop(ctx,0x8c619620u); if((*sr&1u))goto L8c619626;goto L8c619622; /* bt 0x8c619626 */
L8c619622: sa_motion_publish_stop(ctx,0x8c619624u); goto L8c619802; /* bra 0x8c619802 */
L8c619626: sa_motion_publish_stop(ctx,0x8c619626u); r[0]=16u;goto L8c619628; /* mov #0x10,r0 */
L8c619628: sa_motion_publish_stop(ctx,0x8c619628u); fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c61962a; /* fmov.s @(r0,r14),fr4 */
L8c61962a: sa_motion_publish_stop(ctx,0x8c61962au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61962c; /* fmov.s @(r0,r4),fr3 */
L8c61962c: sa_motion_publish_stop(ctx,0x8c61962cu); r[0]=20u;goto L8c61962e; /* mov #0x14,r0 */
L8c61962e: sa_motion_publish_stop(ctx,0x8c61962eu); fcopy(f,2u,4u);goto L8c619630; /* fmov fr4,fr2 */
L8c619630: sa_motion_publish_stop(ctx,0x8c619630u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c619632; /* fsub fr3,fr2 */
L8c619632: sa_motion_publish_stop(ctx,0x8c619632u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c619634; /* fmov.s @(r0,r13),fr3 */
L8c619634: sa_motion_publish_stop(ctx,0x8c619634u); fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c619636; /* fmov.s @(r0,r14),fr5 */
L8c619636: sa_motion_publish_stop(ctx,0x8c619636u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619638; /* fsub fr5,fr3 */
L8c619638: sa_motion_publish_stop(ctx,0x8c619638u); fcopy(f,1u,5u);goto L8c61963a; /* fmov fr5,fr1 */
L8c61963a: sa_motion_publish_stop(ctx,0x8c61963au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61963c; /* fmul fr3,fr2 */
L8c61963c: sa_motion_publish_stop(ctx,0x8c61963cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61963e; /* fmov.s @(r0,r4),fr3 */
L8c61963e: sa_motion_publish_stop(ctx,0x8c61963eu); r[0]=16u;goto L8c619640; /* mov #0x10,r0 */
L8c619640: sa_motion_publish_stop(ctx,0x8c619640u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c619642; /* fsub fr3,fr1 */
L8c619642: sa_motion_publish_stop(ctx,0x8c619642u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c619644; /* fmov.s @(r0,r13),fr3 */
L8c619644: sa_motion_publish_stop(ctx,0x8c619644u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619646; /* fsub fr4,fr3 */
L8c619646: sa_motion_publish_stop(ctx,0x8c619646u); fcopy(f,4u,2u);goto L8c619648; /* fmov fr2,fr4 */
L8c619648: sa_motion_publish_stop(ctx,0x8c619648u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61964a; /* fmul fr3,fr1 */
L8c61964a: sa_motion_publish_stop(ctx,0x8c61964au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61964c; /* fldi0 fr3 */
L8c61964c: sa_motion_publish_stop(ctx,0x8c61964cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61964e; /* fsub fr1,fr4 */
L8c61964e: sa_motion_publish_stop(ctx,0x8c61964eu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c619650; /* fcmp/gt fr3,fr4 */
L8c619650: sa_motion_publish_stop(ctx,0x8c619650u); if(!(*sr&1u))goto L8c619694;goto L8c619652; /* bf 0x8c619694 */
L8c619652: sa_motion_publish_stop(ctx,0x8c619654u); r[11]+=16u;goto L8c6198b8; /* bra 0x8c6198b8 */
L8c619694: sa_motion_publish_stop(ctx,0x8c619694u); r[3]=rd(m,bus,&io_status,0x8c619998u,4);if(io_status)return io_status;goto L8c619696; /* mov.l 0x8c619998,r3 */
L8c619696: sa_motion_publish_stop(ctx,0x8c619696u); r[0]=4u;goto L8c619698; /* mov #0x4,r0 */
L8c619698: sa_motion_publish_stop(ctx,0x8c619698u); fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61969a; /* fmov.s @r4,fr3 */
L8c61969a: sa_motion_publish_stop(ctx,0x8c61969au); r[8]+=28u;goto L8c61969c; /* add #0x1c,r8 */
L8c61969c: sa_motion_publish_stop(ctx,0x8c61969cu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61969e; /* fmov.s @r3,fr2 */
L8c61969e: sa_motion_publish_stop(ctx,0x8c61969eu); r[3]+=8u;goto L8c6196a0; /* add #0x8,r3 */
L8c6196a0: sa_motion_publish_stop(ctx,0x8c6196a0u); r[2]=rd(m,bus,&io_status,0x8c61999cu,4);if(io_status)return io_status;goto L8c6196a2; /* mov.l 0x8c61999c,r2 */
L8c6196a2: sa_motion_publish_stop(ctx,0x8c6196a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6196a4; /* fmul fr3,fr2 */
L8c6196a4: sa_motion_publish_stop(ctx,0x8c6196a4u); r[1]=rd(m,bus,&io_status,0x8c6199a0u,4);if(io_status)return io_status;goto L8c6196a6; /* mov.l 0x8c6199a0,r1 */
L8c6196a6: sa_motion_publish_stop(ctx,0x8c6196a6u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6196a8; /* fmov.s @r2,fr3 */
L8c6196a8: sa_motion_publish_stop(ctx,0x8c6196a8u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c6196aa; /* fmov.s @r1,fr1 */
L8c6196aa: sa_motion_publish_stop(ctx,0x8c6196aau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6196ac; /* fsub fr2,fr3 */
L8c6196ac: sa_motion_publish_stop(ctx,0x8c6196acu); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6196ae; /* fmov.s @(r0,r4),fr2 */
L8c6196ae: sa_motion_publish_stop(ctx,0x8c6196aeu); r[0]=8u;goto L8c6196b0; /* mov #0x8,r0 */
L8c6196b0: sa_motion_publish_stop(ctx,0x8c6196b0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6196b2; /* fmul fr2,fr1 */
L8c6196b2: sa_motion_publish_stop(ctx,0x8c6196b2u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6196b4; /* fmov.s @(r0,r4),fr2 */
L8c6196b4: sa_motion_publish_stop(ctx,0x8c6196b4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6196b6; /* fsub fr1,fr3 */
L8c6196b6: sa_motion_publish_stop(ctx,0x8c6196b6u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6196b8; /* fmov.s @r3,fr1 */
L8c6196b8: sa_motion_publish_stop(ctx,0x8c6196b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6196ba; /* fmul fr2,fr1 */
L8c6196ba: sa_motion_publish_stop(ctx,0x8c6196bau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6196bc; /* fsub fr1,fr3 */
L8c6196bc: sa_motion_publish_stop(ctx,0x8c6196bcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6196be; /* fmov.s fr3,@-r8 */
L8c6196be: sa_motion_publish_stop(ctx,0x8c6196beu); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c6196c0; /* mov.w @(0x2,r11),r0 */
L8c6196c0: sa_motion_publish_stop(ctx,0x8c6196c0u); r[2]=r[0];goto L8c6196c2; /* mov r0,r2 */
L8c6196c2: sa_motion_publish_stop(ctx,0x8c6196c2u); *fpul=r[2];goto L8c6196c4; /* lds r2,FPUL */
L8c6196c4: sa_motion_publish_stop(ctx,0x8c6196c4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6196c6; /* float FPUL,fr3 */
L8c6196c6: sa_motion_publish_stop(ctx,0x8c6196c6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6196c8; /* fmul fr15,fr3 */
L8c6196c8: sa_motion_publish_stop(ctx,0x8c6196c8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6196ca; /* fmov.s fr3,@-r8 */
L8c6196ca: sa_motion_publish_stop(ctx,0x8c6196cau); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c6196cc; /* mov.w @r11,r2 */
L8c6196cc: sa_motion_publish_stop(ctx,0x8c6196ccu); *fpul=r[2];goto L8c6196ce; /* lds r2,FPUL */
L8c6196ce: sa_motion_publish_stop(ctx,0x8c6196ceu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6196d0; /* float FPUL,fr3 */
L8c6196d0: sa_motion_publish_stop(ctx,0x8c6196d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6196d2; /* fmul fr15,fr3 */
L8c6196d2: sa_motion_publish_stop(ctx,0x8c6196d2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6196d4; /* fmov.s fr3,@-r8 */
L8c6196d4: sa_motion_publish_stop(ctx,0x8c6196d4u); r[0]=24u;goto L8c6196d6; /* mov #0x18,r0 */
L8c6196d6: sa_motion_publish_stop(ctx,0x8c6196d6u); r[2]=rd(m,bus,&io_status,0x8c6199a4u,4);if(io_status)return io_status;goto L8c6196d8; /* mov.l 0x8c6199a4,r2 */
L8c6196d8: sa_motion_publish_stop(ctx,0x8c6196d8u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6196da; /* fmov.s @(r0,r4),fr3 */
L8c6196da: sa_motion_publish_stop(ctx,0x8c6196dau); r[0]=20u;goto L8c6196dc; /* mov #0x14,r0 */
L8c6196dc: sa_motion_publish_stop(ctx,0x8c6196dcu); r[11]+=4u;goto L8c6196de; /* add #0x4,r11 */
L8c6196de: sa_motion_publish_stop(ctx,0x8c6196deu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6196e0; /* fmov.s fr3,@-r8 */
L8c6196e0: sa_motion_publish_stop(ctx,0x8c6196e0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6196e2; /* fmov.s @(r0,r4),fr3 */
L8c6196e2: sa_motion_publish_stop(ctx,0x8c6196e2u); r[0]=16u;goto L8c6196e4; /* mov #0x10,r0 */
L8c6196e4: sa_motion_publish_stop(ctx,0x8c6196e4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6196e6; /* fmov.s fr3,@-r8 */
L8c6196e6: sa_motion_publish_stop(ctx,0x8c6196e6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6196e8; /* fmov.s @(r0,r4),fr3 */
L8c6196e8: sa_motion_publish_stop(ctx,0x8c6196e8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6196ea; /* fmov.s fr3,@-r8 */
L8c6196ea: sa_motion_publish_stop(ctx,0x8c6196eau); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c6196ec; /* mov.l r2,@-r8 */
L8c6196ec: sa_motion_publish_stop(ctx,0x8c6196ecu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6196ee; /* pref @r8 */
L8c6196ee: sa_motion_publish_stop(ctx,0x8c6196eeu); r[2]=rd(m,bus,&io_status,0x8c619998u,4);if(io_status)return io_status;goto L8c6196f0; /* mov.l 0x8c619998,r2 */
L8c6196f0: sa_motion_publish_stop(ctx,0x8c6196f0u); r[0]=4u;goto L8c6196f2; /* mov #0x4,r0 */
L8c6196f2: sa_motion_publish_stop(ctx,0x8c6196f2u); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c6196f4; /* fmov.s @r14,fr3 */
L8c6196f4: sa_motion_publish_stop(ctx,0x8c6196f4u); r[8]+=60u;goto L8c6196f6; /* add #0x3c,r8 */
L8c6196f6: sa_motion_publish_stop(ctx,0x8c6196f6u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c6196f8; /* fmov.s @r2,fr2 */
L8c6196f8: sa_motion_publish_stop(ctx,0x8c6196f8u); r[3]=rd(m,bus,&io_status,0x8c61999cu,4);if(io_status)return io_status;goto L8c6196fa; /* mov.l 0x8c61999c,r3 */
L8c6196fa: sa_motion_publish_stop(ctx,0x8c6196fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6196fc; /* fmul fr3,fr2 */
L8c6196fc: sa_motion_publish_stop(ctx,0x8c6196fcu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c6196fe; /* fmov.s @r1,fr1 */
L8c6196fe: sa_motion_publish_stop(ctx,0x8c6196feu); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c619700; /* fmov.s @r3,fr3 */
L8c619700: sa_motion_publish_stop(ctx,0x8c619700u); r[3]=rd(m,bus,&io_status,0x8c6199a8u,4);if(io_status)return io_status;goto L8c619702; /* mov.l 0x8c6199a8,r3 */
L8c619702: sa_motion_publish_stop(ctx,0x8c619702u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619704; /* fsub fr2,fr3 */
L8c619704: sa_motion_publish_stop(ctx,0x8c619704u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c619706; /* fmov.s @(r0,r14),fr2 */
L8c619706: sa_motion_publish_stop(ctx,0x8c619706u); r[0]=8u;goto L8c619708; /* mov #0x8,r0 */
L8c619708: sa_motion_publish_stop(ctx,0x8c619708u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61970a; /* fmul fr2,fr1 */
L8c61970a: sa_motion_publish_stop(ctx,0x8c61970au); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61970c; /* fmov.s @(r0,r14),fr2 */
L8c61970c: sa_motion_publish_stop(ctx,0x8c61970cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61970e; /* fsub fr1,fr3 */
L8c61970e: sa_motion_publish_stop(ctx,0x8c61970eu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619710; /* fmov.s @r3,fr1 */
L8c619710: sa_motion_publish_stop(ctx,0x8c619710u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619712; /* fmul fr2,fr1 */
L8c619712: sa_motion_publish_stop(ctx,0x8c619712u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619714; /* fsub fr1,fr3 */
L8c619714: sa_motion_publish_stop(ctx,0x8c619714u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619716; /* fmov.s fr3,@-r8 */
L8c619716: sa_motion_publish_stop(ctx,0x8c619716u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619718; /* mov.w @(0x2,r11),r0 */
L8c619718: sa_motion_publish_stop(ctx,0x8c619718u); r[2]=r[0];goto L8c61971a; /* mov r0,r2 */
L8c61971a: sa_motion_publish_stop(ctx,0x8c61971au); *fpul=r[2];goto L8c61971c; /* lds r2,FPUL */
L8c61971c: sa_motion_publish_stop(ctx,0x8c61971cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61971e; /* float FPUL,fr3 */
L8c61971e: sa_motion_publish_stop(ctx,0x8c61971eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619720; /* fmul fr15,fr3 */
L8c619720: sa_motion_publish_stop(ctx,0x8c619720u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619722; /* fmov.s fr3,@-r8 */
L8c619722: sa_motion_publish_stop(ctx,0x8c619722u); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619724; /* mov.w @r11,r2 */
L8c619724: sa_motion_publish_stop(ctx,0x8c619724u); r[11]+=4u;goto L8c619726; /* add #0x4,r11 */
L8c619726: sa_motion_publish_stop(ctx,0x8c619726u); *fpul=r[2];goto L8c619728; /* lds r2,FPUL */
L8c619728: sa_motion_publish_stop(ctx,0x8c619728u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61972a; /* float FPUL,fr3 */
L8c61972a: sa_motion_publish_stop(ctx,0x8c61972au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61972c; /* fmul fr15,fr3 */
L8c61972c: sa_motion_publish_stop(ctx,0x8c61972cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61972e; /* fmov.s fr3,@-r8 */
L8c61972e: sa_motion_publish_stop(ctx,0x8c61972eu); r[0]=24u;goto L8c619730; /* mov #0x18,r0 */
L8c619730: sa_motion_publish_stop(ctx,0x8c619730u); r[2]=rd(m,bus,&io_status,0x8c6199a4u,4);if(io_status)return io_status;goto L8c619732; /* mov.l 0x8c6199a4,r2 */
L8c619732: sa_motion_publish_stop(ctx,0x8c619732u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c619734; /* fmov.s @(r0,r14),fr3 */
L8c619734: sa_motion_publish_stop(ctx,0x8c619734u); r[0]=20u;goto L8c619736; /* mov #0x14,r0 */
L8c619736: sa_motion_publish_stop(ctx,0x8c619736u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619738; /* fmov.s fr3,@-r8 */
L8c619738: sa_motion_publish_stop(ctx,0x8c619738u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61973a; /* fmov.s @(r0,r14),fr3 */
L8c61973a: sa_motion_publish_stop(ctx,0x8c61973au); r[0]=16u;goto L8c61973c; /* mov #0x10,r0 */
L8c61973c: sa_motion_publish_stop(ctx,0x8c61973cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61973e; /* fmov.s fr3,@-r8 */
L8c61973e: sa_motion_publish_stop(ctx,0x8c61973eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c619740; /* fmov.s @(r0,r14),fr3 */
L8c619740: sa_motion_publish_stop(ctx,0x8c619740u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619742; /* fmov.s fr3,@-r8 */
L8c619742: sa_motion_publish_stop(ctx,0x8c619742u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c619744; /* mov.l r2,@-r8 */
L8c619744: sa_motion_publish_stop(ctx,0x8c619744u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619746; /* pref @r8 */
L8c619746: sa_motion_publish_stop(ctx,0x8c619746u); r[2]=rd(m,bus,&io_status,0x8c619998u,4);if(io_status)return io_status;goto L8c619748; /* mov.l 0x8c619998,r2 */
L8c619748: sa_motion_publish_stop(ctx,0x8c619748u); r[0]=4u;goto L8c61974a; /* mov #0x4,r0 */
L8c61974a: sa_motion_publish_stop(ctx,0x8c61974au); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61974c; /* fmov.s @r13,fr3 */
L8c61974c: sa_motion_publish_stop(ctx,0x8c61974cu); r[8]+=60u;goto L8c61974e; /* add #0x3c,r8 */
L8c61974e: sa_motion_publish_stop(ctx,0x8c61974eu); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c619750; /* fmov.s @r2,fr2 */
L8c619750: sa_motion_publish_stop(ctx,0x8c619750u); r[3]=rd(m,bus,&io_status,0x8c61999cu,4);if(io_status)return io_status;goto L8c619752; /* mov.l 0x8c61999c,r3 */
L8c619752: sa_motion_publish_stop(ctx,0x8c619752u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619754; /* fmul fr3,fr2 */
L8c619754: sa_motion_publish_stop(ctx,0x8c619754u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c619756; /* fmov.s @r1,fr1 */
L8c619756: sa_motion_publish_stop(ctx,0x8c619756u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c619758; /* fmov.s @r3,fr3 */
L8c619758: sa_motion_publish_stop(ctx,0x8c619758u); r[3]=rd(m,bus,&io_status,0x8c6199a8u,4);if(io_status)return io_status;goto L8c61975a; /* mov.l 0x8c6199a8,r3 */
L8c61975a: sa_motion_publish_stop(ctx,0x8c61975au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61975c; /* fsub fr2,fr3 */
L8c61975c: sa_motion_publish_stop(ctx,0x8c61975cu); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61975e; /* fmov.s @(r0,r13),fr2 */
L8c61975e: sa_motion_publish_stop(ctx,0x8c61975eu); r[0]=8u;goto L8c619760; /* mov #0x8,r0 */
L8c619760: sa_motion_publish_stop(ctx,0x8c619760u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619762; /* fmul fr2,fr1 */
L8c619762: sa_motion_publish_stop(ctx,0x8c619762u); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c619764; /* fmov.s @(r0,r13),fr2 */
L8c619764: sa_motion_publish_stop(ctx,0x8c619764u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619766; /* fsub fr1,fr3 */
L8c619766: sa_motion_publish_stop(ctx,0x8c619766u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619768; /* fmov.s @r3,fr1 */
L8c619768: sa_motion_publish_stop(ctx,0x8c619768u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61976a; /* fmul fr2,fr1 */
L8c61976a: sa_motion_publish_stop(ctx,0x8c61976au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61976c; /* fsub fr1,fr3 */
L8c61976c: sa_motion_publish_stop(ctx,0x8c61976cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61976e; /* fmov.s fr3,@-r8 */
L8c61976e: sa_motion_publish_stop(ctx,0x8c61976eu); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619770; /* mov.w @(0x2,r11),r0 */
L8c619770: sa_motion_publish_stop(ctx,0x8c619770u); r[2]=r[0];goto L8c619772; /* mov r0,r2 */
L8c619772: sa_motion_publish_stop(ctx,0x8c619772u); *fpul=r[2];goto L8c619774; /* lds r2,FPUL */
L8c619774: sa_motion_publish_stop(ctx,0x8c619774u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619776; /* float FPUL,fr3 */
L8c619776: sa_motion_publish_stop(ctx,0x8c619776u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619778; /* fmul fr15,fr3 */
L8c619778: sa_motion_publish_stop(ctx,0x8c619778u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61977a; /* fmov.s fr3,@-r8 */
L8c61977a: sa_motion_publish_stop(ctx,0x8c61977au); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61977c; /* mov.w @r11,r2 */
L8c61977c: sa_motion_publish_stop(ctx,0x8c61977cu); r[11]+=4u;goto L8c61977e; /* add #0x4,r11 */
L8c61977e: sa_motion_publish_stop(ctx,0x8c61977eu); *fpul=r[2];goto L8c619780; /* lds r2,FPUL */
L8c619780: sa_motion_publish_stop(ctx,0x8c619780u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619782; /* float FPUL,fr3 */
L8c619782: sa_motion_publish_stop(ctx,0x8c619782u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619784; /* fmul fr15,fr3 */
L8c619784: sa_motion_publish_stop(ctx,0x8c619784u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619786; /* fmov.s fr3,@-r8 */
L8c619786: sa_motion_publish_stop(ctx,0x8c619786u); r[0]=24u;goto L8c619788; /* mov #0x18,r0 */
L8c619788: sa_motion_publish_stop(ctx,0x8c619788u); r[2]=rd(m,bus,&io_status,0x8c6199a4u,4);if(io_status)return io_status;goto L8c61978a; /* mov.l 0x8c6199a4,r2 */
L8c61978a: sa_motion_publish_stop(ctx,0x8c61978au); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61978c; /* fmov.s @(r0,r13),fr3 */
L8c61978c: sa_motion_publish_stop(ctx,0x8c61978cu); r[0]=20u;goto L8c61978e; /* mov #0x14,r0 */
L8c61978e: sa_motion_publish_stop(ctx,0x8c61978eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619790; /* fmov.s fr3,@-r8 */
L8c619790: sa_motion_publish_stop(ctx,0x8c619790u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c619792; /* fmov.s @(r0,r13),fr3 */
L8c619792: sa_motion_publish_stop(ctx,0x8c619792u); r[0]=16u;goto L8c619794; /* mov #0x10,r0 */
L8c619794: sa_motion_publish_stop(ctx,0x8c619794u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619796; /* fmov.s fr3,@-r8 */
L8c619796: sa_motion_publish_stop(ctx,0x8c619796u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c619798; /* fmov.s @(r0,r13),fr3 */
L8c619798: sa_motion_publish_stop(ctx,0x8c619798u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61979a; /* fmov.s fr3,@-r8 */
L8c61979a: sa_motion_publish_stop(ctx,0x8c61979au); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61979c; /* mov.l r2,@-r8 */
L8c61979c: sa_motion_publish_stop(ctx,0x8c61979cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61979e; /* pref @r8 */
L8c61979e: sa_motion_publish_stop(ctx,0x8c61979eu); r[2]=rd(m,bus,&io_status,0x8c619998u,4);if(io_status)return io_status;goto L8c6197a0; /* mov.l 0x8c619998,r2 */
L8c6197a0: sa_motion_publish_stop(ctx,0x8c6197a0u); r[0]=4u;goto L8c6197a2; /* mov #0x4,r0 */
L8c6197a2: sa_motion_publish_stop(ctx,0x8c6197a2u); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c6197a4; /* fmov.s @r12,fr3 */
L8c6197a4: sa_motion_publish_stop(ctx,0x8c6197a4u); r[8]+=60u;goto L8c6197a6; /* add #0x3c,r8 */
L8c6197a6: sa_motion_publish_stop(ctx,0x8c6197a6u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c6197a8; /* fmov.s @r2,fr2 */
L8c6197a8: sa_motion_publish_stop(ctx,0x8c6197a8u); r[3]=rd(m,bus,&io_status,0x8c61999cu,4);if(io_status)return io_status;goto L8c6197aa; /* mov.l 0x8c61999c,r3 */
L8c6197aa: sa_motion_publish_stop(ctx,0x8c6197aau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6197ac; /* fmul fr3,fr2 */
L8c6197ac: sa_motion_publish_stop(ctx,0x8c6197acu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c6197ae; /* fmov.s @r1,fr1 */
L8c6197ae: sa_motion_publish_stop(ctx,0x8c6197aeu); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6197b0; /* fmov.s @r3,fr3 */
L8c6197b0: sa_motion_publish_stop(ctx,0x8c6197b0u); r[3]=rd(m,bus,&io_status,0x8c6199a8u,4);if(io_status)return io_status;goto L8c6197b2; /* mov.l 0x8c6199a8,r3 */
L8c6197b2: sa_motion_publish_stop(ctx,0x8c6197b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6197b4; /* fsub fr2,fr3 */
L8c6197b4: sa_motion_publish_stop(ctx,0x8c6197b4u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6197b6; /* fmov.s @(r0,r12),fr2 */
L8c6197b6: sa_motion_publish_stop(ctx,0x8c6197b6u); r[0]=8u;goto L8c6197b8; /* mov #0x8,r0 */
L8c6197b8: sa_motion_publish_stop(ctx,0x8c6197b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6197ba; /* fmul fr2,fr1 */
L8c6197ba: sa_motion_publish_stop(ctx,0x8c6197bau); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6197bc; /* fmov.s @(r0,r12),fr2 */
L8c6197bc: sa_motion_publish_stop(ctx,0x8c6197bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6197be; /* fsub fr1,fr3 */
L8c6197be: sa_motion_publish_stop(ctx,0x8c6197beu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6197c0; /* fmov.s @r3,fr1 */
L8c6197c0: sa_motion_publish_stop(ctx,0x8c6197c0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6197c2; /* fmul fr2,fr1 */
L8c6197c2: sa_motion_publish_stop(ctx,0x8c6197c2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6197c4; /* fsub fr1,fr3 */
L8c6197c4: sa_motion_publish_stop(ctx,0x8c6197c4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6197c6; /* fmov.s fr3,@-r8 */
L8c6197c6: sa_motion_publish_stop(ctx,0x8c6197c6u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c6197c8; /* mov.w @(0x2,r11),r0 */
L8c6197c8: sa_motion_publish_stop(ctx,0x8c6197c8u); r[2]=r[0];goto L8c6197ca; /* mov r0,r2 */
L8c6197ca: sa_motion_publish_stop(ctx,0x8c6197cau); *fpul=r[2];goto L8c6197cc; /* lds r2,FPUL */
L8c6197cc: sa_motion_publish_stop(ctx,0x8c6197ccu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6197ce; /* float FPUL,fr3 */
L8c6197ce: sa_motion_publish_stop(ctx,0x8c6197ceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6197d0; /* fmul fr15,fr3 */
L8c6197d0: sa_motion_publish_stop(ctx,0x8c6197d0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6197d2; /* fmov.s fr3,@-r8 */
L8c6197d2: sa_motion_publish_stop(ctx,0x8c6197d2u); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c6197d4; /* mov.w @r11,r2 */
L8c6197d4: sa_motion_publish_stop(ctx,0x8c6197d4u); r[11]+=4u;goto L8c6197d6; /* add #0x4,r11 */
L8c6197d6: sa_motion_publish_stop(ctx,0x8c6197d6u); *fpul=r[2];goto L8c6197d8; /* lds r2,FPUL */
L8c6197d8: sa_motion_publish_stop(ctx,0x8c6197d8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6197da; /* float FPUL,fr3 */
L8c6197da: sa_motion_publish_stop(ctx,0x8c6197dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6197dc; /* fmul fr15,fr3 */
L8c6197dc: sa_motion_publish_stop(ctx,0x8c6197dcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6197de; /* fmov.s fr3,@-r8 */
L8c6197de: sa_motion_publish_stop(ctx,0x8c6197deu); r[0]=24u;goto L8c6197e0; /* mov #0x18,r0 */
L8c6197e0: sa_motion_publish_stop(ctx,0x8c6197e0u); r[2]=rd(m,bus,&io_status,0x8c6199acu,4);if(io_status)return io_status;goto L8c6197e2; /* mov.l 0x8c6199ac,r2 */
L8c6197e2: sa_motion_publish_stop(ctx,0x8c6197e2u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c6197e4; /* fmov.s @(r0,r12),fr3 */
L8c6197e4: sa_motion_publish_stop(ctx,0x8c6197e4u); r[0]=20u;goto L8c6197e6; /* mov #0x14,r0 */
L8c6197e6: sa_motion_publish_stop(ctx,0x8c6197e6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6197e8; /* fmov.s fr3,@-r8 */
L8c6197e8: sa_motion_publish_stop(ctx,0x8c6197e8u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c6197ea; /* fmov.s @(r0,r12),fr3 */
L8c6197ea: sa_motion_publish_stop(ctx,0x8c6197eau); r[0]=16u;goto L8c6197ec; /* mov #0x10,r0 */
L8c6197ec: sa_motion_publish_stop(ctx,0x8c6197ecu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6197ee; /* fmov.s fr3,@-r8 */
L8c6197ee: sa_motion_publish_stop(ctx,0x8c6197eeu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c6197f0; /* fmov.s @(r0,r12),fr3 */
L8c6197f0: sa_motion_publish_stop(ctx,0x8c6197f0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6197f2; /* fmov.s fr3,@-r8 */
L8c6197f2: sa_motion_publish_stop(ctx,0x8c6197f2u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c6197f4; /* mov.l r2,@-r8 */
L8c6197f4: sa_motion_publish_stop(ctx,0x8c6197f4u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6197f6; /* pref @r8 */
L8c6197f6: sa_motion_publish_stop(ctx,0x8c6197f6u); r[0]=rd(m,bus,&io_status,0x8c6199b0u,4);if(io_status)return io_status;goto L8c6197f8; /* mov.l 0x8c6199b0,r0 */
L8c6197f8: sa_motion_publish_stop(ctx,0x8c6197f8u); r[8]+=32u;goto L8c6197fa; /* add #0x20,r8 */
L8c6197fa: sa_motion_publish_stop(ctx,0x8c6197fau); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c6197fc; /* mov.l @r0,r2 */
L8c6197fc: sa_motion_publish_stop(ctx,0x8c6197fcu); r[2]+=4u;goto L8c6197fe; /* add #0x4,r2 */
L8c6197fe: sa_motion_publish_stop(ctx,0x8c619800u); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c6198b8; /* bra 0x8c6198b8 */
L8c619802: sa_motion_publish_stop(ctx,0x8c619802u); r[1]=rd(m,bus,&io_status,0x8c6199b4u,4);if(io_status)return io_status;goto L8c619804; /* mov.l 0x8c6199b4,r1 */
L8c619804: sa_motion_publish_stop(ctx,0x8c619804u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c619806; /* mov.l r2,@r1 */
L8c619806: sa_motion_publish_stop(ctx,0x8c619806u); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619808; /* mov.w @r11,r3 */
L8c619808: sa_motion_publish_stop(ctx,0x8c619808u); *fpul=r[3];goto L8c61980a; /* lds r3,FPUL */
L8c61980a: sa_motion_publish_stop(ctx,0x8c61980au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61980c; /* float FPUL,fr3 */
L8c61980c: sa_motion_publish_stop(ctx,0x8c61980cu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61980e; /* ftrc fr3,FPUL */
L8c61980e: sa_motion_publish_stop(ctx,0x8c61980eu); r[2]=*fpul;goto L8c619810; /* sts FPUL,r2 */
L8c619810: sa_motion_publish_stop(ctx,0x8c619810u); wr(m,bus,&io_status,r[9],r[2],2);if(io_status)return io_status;goto L8c619812; /* mov.w r2,@r9 */
L8c619812: sa_motion_publish_stop(ctx,0x8c619812u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619814; /* mov.w @(0x2,r11),r0 */
L8c619814: sa_motion_publish_stop(ctx,0x8c619814u); r[11]+=4u;goto L8c619816; /* add #0x4,r11 */
L8c619816: sa_motion_publish_stop(ctx,0x8c619816u); r[3]=r[0];goto L8c619818; /* mov r0,r3 */
L8c619818: sa_motion_publish_stop(ctx,0x8c619818u); *fpul=r[3];goto L8c61981a; /* lds r3,FPUL */
L8c61981a: sa_motion_publish_stop(ctx,0x8c61981au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61981c; /* float FPUL,fr3 */
L8c61981c: sa_motion_publish_stop(ctx,0x8c61981cu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61981e; /* ftrc fr3,FPUL */
L8c61981e: sa_motion_publish_stop(ctx,0x8c61981eu); r[0]=*fpul;goto L8c619820; /* sts FPUL,r0 */
L8c619820: sa_motion_publish_stop(ctx,0x8c619820u); wr(m,bus,&io_status,r[9]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619822; /* mov.w r0,@(0x2,r9) */
L8c619822: sa_motion_publish_stop(ctx,0x8c619822u); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619824; /* mov.w @r11,r2 */
L8c619824: sa_motion_publish_stop(ctx,0x8c619824u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c619826; /* mov.l @r15,r3 */
L8c619826: sa_motion_publish_stop(ctx,0x8c619826u); *fpul=r[2];goto L8c619828; /* lds r2,FPUL */
L8c619828: sa_motion_publish_stop(ctx,0x8c619828u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61982a; /* float FPUL,fr3 */
L8c61982a: sa_motion_publish_stop(ctx,0x8c61982au); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61982c; /* ftrc fr3,FPUL */
L8c61982c: sa_motion_publish_stop(ctx,0x8c61982cu); r[1]=*fpul;goto L8c61982e; /* sts FPUL,r1 */
L8c61982e: sa_motion_publish_stop(ctx,0x8c61982eu); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c619830; /* mov.w r1,@r3 */
L8c619830: sa_motion_publish_stop(ctx,0x8c619830u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619832; /* mov.w @(0x2,r11),r0 */
L8c619832: sa_motion_publish_stop(ctx,0x8c619832u); r[11]+=4u;goto L8c619834; /* add #0x4,r11 */
L8c619834: sa_motion_publish_stop(ctx,0x8c619834u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c619836; /* mov.l @r15,r3 */
L8c619836: sa_motion_publish_stop(ctx,0x8c619836u); r[2]=r[0];goto L8c619838; /* mov r0,r2 */
L8c619838: sa_motion_publish_stop(ctx,0x8c619838u); *fpul=r[2];goto L8c61983a; /* lds r2,FPUL */
L8c61983a: sa_motion_publish_stop(ctx,0x8c61983au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61983c; /* float FPUL,fr3 */
L8c61983c: sa_motion_publish_stop(ctx,0x8c61983cu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61983e; /* ftrc fr3,FPUL */
L8c61983e: sa_motion_publish_stop(ctx,0x8c61983eu); r[0]=*fpul;goto L8c619840; /* sts FPUL,r0 */
L8c619840: sa_motion_publish_stop(ctx,0x8c619840u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619842; /* mov.w r0,@(0x2,r3) */
L8c619842: sa_motion_publish_stop(ctx,0x8c619842u); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619844; /* mov.w @r11,r2 */
L8c619844: sa_motion_publish_stop(ctx,0x8c619844u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c619846; /* mov.l @(0xc,r15),r3 */
L8c619846: sa_motion_publish_stop(ctx,0x8c619846u); *fpul=r[2];goto L8c619848; /* lds r2,FPUL */
L8c619848: sa_motion_publish_stop(ctx,0x8c619848u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61984a; /* float FPUL,fr3 */
L8c61984a: sa_motion_publish_stop(ctx,0x8c61984au); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61984c; /* ftrc fr3,FPUL */
L8c61984c: sa_motion_publish_stop(ctx,0x8c61984cu); r[1]=*fpul;goto L8c61984e; /* sts FPUL,r1 */
L8c61984e: sa_motion_publish_stop(ctx,0x8c61984eu); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c619850; /* mov.w r1,@r3 */
L8c619850: sa_motion_publish_stop(ctx,0x8c619850u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619852; /* mov.w @(0x2,r11),r0 */
L8c619852: sa_motion_publish_stop(ctx,0x8c619852u); r[11]+=4u;goto L8c619854; /* add #0x4,r11 */
L8c619854: sa_motion_publish_stop(ctx,0x8c619854u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c619856; /* mov.l @(0xc,r15),r3 */
L8c619856: sa_motion_publish_stop(ctx,0x8c619856u); r[2]=r[0];goto L8c619858; /* mov r0,r2 */
L8c619858: sa_motion_publish_stop(ctx,0x8c619858u); *fpul=r[2];goto L8c61985a; /* lds r2,FPUL */
L8c61985a: sa_motion_publish_stop(ctx,0x8c61985au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61985c; /* float FPUL,fr3 */
L8c61985c: sa_motion_publish_stop(ctx,0x8c61985cu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61985e; /* ftrc fr3,FPUL */
L8c61985e: sa_motion_publish_stop(ctx,0x8c61985eu); r[0]=*fpul;goto L8c619860; /* sts FPUL,r0 */
L8c619860: sa_motion_publish_stop(ctx,0x8c619860u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619862; /* mov.w r0,@(0x2,r3) */
L8c619862: sa_motion_publish_stop(ctx,0x8c619862u); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619864; /* mov.w @r11,r2 */
L8c619864: sa_motion_publish_stop(ctx,0x8c619864u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c619866; /* mov.l @(0x4,r15),r3 */
L8c619866: sa_motion_publish_stop(ctx,0x8c619866u); *fpul=r[2];goto L8c619868; /* lds r2,FPUL */
L8c619868: sa_motion_publish_stop(ctx,0x8c619868u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61986a; /* float FPUL,fr3 */
L8c61986a: sa_motion_publish_stop(ctx,0x8c61986au); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61986c; /* ftrc fr3,FPUL */
L8c61986c: sa_motion_publish_stop(ctx,0x8c61986cu); r[1]=*fpul;goto L8c61986e; /* sts FPUL,r1 */
L8c61986e: sa_motion_publish_stop(ctx,0x8c61986eu); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c619870; /* mov.w r1,@r3 */
L8c619870: sa_motion_publish_stop(ctx,0x8c619870u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619872; /* mov.w @(0x2,r11),r0 */
L8c619872: sa_motion_publish_stop(ctx,0x8c619872u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c619874; /* mov.l @(0x4,r15),r3 */
L8c619874: sa_motion_publish_stop(ctx,0x8c619874u); r[2]=r[0];goto L8c619876; /* mov r0,r2 */
L8c619876: sa_motion_publish_stop(ctx,0x8c619876u); *fpul=r[2];goto L8c619878; /* lds r2,FPUL */
L8c619878: sa_motion_publish_stop(ctx,0x8c619878u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61987a; /* float FPUL,fr3 */
L8c61987a: sa_motion_publish_stop(ctx,0x8c61987au); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61987c; /* ftrc fr3,FPUL */
L8c61987c: sa_motion_publish_stop(ctx,0x8c61987cu); r[0]=*fpul;goto L8c61987e; /* sts FPUL,r0 */
L8c61987e: sa_motion_publish_stop(ctx,0x8c61987eu); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619880; /* mov.w r0,@(0x2,r3) */
L8c619880: sa_motion_publish_stop(ctx,0x8c619880u); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c619882; /* mov.l @(0xc,r14),r3 */
L8c619882: sa_motion_publish_stop(ctx,0x8c619882u); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c619884; /* mov.l @(0xc,r4),r5 */
L8c619884: sa_motion_publish_stop(ctx,0x8c619884u); r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c619886; /* mov.l @(0xc,r13),r2 */
L8c619886: sa_motion_publish_stop(ctx,0x8c619886u); r[5]&=r[3];goto L8c619888; /* and r3,r5 */
L8c619888: sa_motion_publish_stop(ctx,0x8c619888u); r[3]=1u;goto L8c61988a; /* mov #0x1,r3 */
L8c61988a: sa_motion_publish_stop(ctx,0x8c61988au); r[5]&=r[2];goto L8c61988c; /* and r2,r5 */
L8c61988c: sa_motion_publish_stop(ctx,0x8c61988cu); *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61988e; /* tst r3,r5 */
L8c61988e: sa_motion_publish_stop(ctx,0x8c619890u); take=!(*sr&1u);r[11]+=4u;if(take)goto L8c61989c;goto L8c619892; /* bf/s 0x8c61989c */
L8c619892: sa_motion_publish_stop(ctx,0x8c619892u); r[1]=rd(m,bus,&io_status,0x8c6199b8u,4);if(io_status)return io_status;goto L8c619894; /* mov.l 0x8c6199b8,r1 */
L8c619894: sa_motion_publish_stop(ctx,0x8c619894u); r[6]=r[13];goto L8c619896; /* mov r13,r6 */
L8c619896: sa_motion_publish_stop(ctx,0x8c619896u); r[7]=r[9];goto L8c619898; /* mov r9,r7 */
L8c619898: sa_motion_publish_stop(ctx,0x8c61989au); target=r[1];*pr=0x8c61989cu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61989c; /* jsr @r1 */
L8c61989c: sa_motion_publish_stop(ctx,0x8c61989cu); r[4]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61989e; /* mov.l @(0xc,r14),r4 */
L8c61989e: sa_motion_publish_stop(ctx,0x8c61989eu); r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c6198a0; /* mov.l @(0xc,r13),r3 */
L8c6198a0: sa_motion_publish_stop(ctx,0x8c6198a0u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c6198a2; /* mov.l @(0xc,r12),r2 */
L8c6198a2: sa_motion_publish_stop(ctx,0x8c6198a2u); r[4]&=r[3];goto L8c6198a4; /* and r3,r4 */
L8c6198a4: sa_motion_publish_stop(ctx,0x8c6198a4u); r[3]=1u;goto L8c6198a6; /* mov #0x1,r3 */
L8c6198a6: sa_motion_publish_stop(ctx,0x8c6198a6u); r[4]&=r[2];goto L8c6198a8; /* and r2,r4 */
L8c6198a8: sa_motion_publish_stop(ctx,0x8c6198a8u); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c6198aa; /* tst r3,r4 */
L8c6198aa: sa_motion_publish_stop(ctx,0x8c6198aau); if(!(*sr&1u))goto L8c6198b8;goto L8c6198ac; /* bf 0x8c6198b8 */
L8c6198ac: sa_motion_publish_stop(ctx,0x8c6198acu); r[2]=rd(m,bus,&io_status,0x8c6199b8u,4);if(io_status)return io_status;goto L8c6198ae; /* mov.l 0x8c6199b8,r2 */
L8c6198ae: sa_motion_publish_stop(ctx,0x8c6198aeu); r[5]=r[13];goto L8c6198b0; /* mov r13,r5 */
L8c6198b0: sa_motion_publish_stop(ctx,0x8c6198b0u); r[7]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6198b2; /* mov.l @r15,r7 */
L8c6198b2: sa_motion_publish_stop(ctx,0x8c6198b2u); r[6]=r[12];goto L8c6198b4; /* mov r12,r6 */
L8c6198b4: sa_motion_publish_stop(ctx,0x8c6198b6u); target=r[2];*pr=0x8c6198b8u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6198b8; /* jsr @r2 */
L8c6198b8: sa_motion_publish_stop(ctx,0x8c6198b8u); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c6198ba; /* mov.l @(0x10,r15),r3 */
L8c6198ba: sa_motion_publish_stop(ctx,0x8c6198bau); r[3]+=1u;goto L8c6198bc; /* add #0x1,r3 */
L8c6198bc: sa_motion_publish_stop(ctx,0x8c6198bcu); wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6198be; /* mov.l r3,@(0x10,r15) */
L8c6198be: sa_motion_publish_stop(ctx,0x8c6198beu); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6198c0; /* mov.l @(0x8,r15),r2 */
L8c6198c0: sa_motion_publish_stop(ctx,0x8c6198c0u); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c6198c2; /* mov.l @(0x10,r15),r3 */
L8c6198c2: sa_motion_publish_stop(ctx,0x8c6198c2u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c6198c4; /* mov.w @(0x2,r2),r0 */
L8c6198c4: sa_motion_publish_stop(ctx,0x8c6198c4u); r[0]=r[0]&65535u;goto L8c6198c6; /* extu.w r0,r0 */
L8c6198c6: sa_motion_publish_stop(ctx,0x8c6198c6u); *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c6198c8; /* cmp/ge r0,r3 */
L8c6198c8: sa_motion_publish_stop(ctx,0x8c6198c8u); if((*sr&1u))goto L8c6198ce;goto L8c6198ca; /* bt 0x8c6198ce */
L8c6198ca: sa_motion_publish_stop(ctx,0x8c6198ccu); goto L8c6195c6; /* bra 0x8c6195c6 */
L8c6198ce: sa_motion_publish_stop(ctx,0x8c6198d0u); goto L8c619be2; /* bra 0x8c619be2 */
L8c6198d2: sa_motion_publish_stop(ctx,0x8c6198d2u); r[4]=rd(m,bus,&io_status,0x8c6199bcu,4);if(io_status)return io_status;goto L8c6198d4; /* mov.l 0x8c6199bc,r4 */
L8c6198d4: sa_motion_publish_stop(ctx,0x8c6198d4u); r[2]=0u;goto L8c6198d6; /* mov #0x0,r2 */
L8c6198d6: sa_motion_publish_stop(ctx,0x8c6198d6u); wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c6198d8; /* mov.l r2,@(0x10,r15) */
L8c6198d8: sa_motion_publish_stop(ctx,0x8c6198d8u); r[2]=r[4];goto L8c6198da; /* mov r4,r2 */
L8c6198da: sa_motion_publish_stop(ctx,0x8c6198dau); r[1]=r[4];goto L8c6198dc; /* mov r4,r1 */
L8c6198dc: sa_motion_publish_stop(ctx,0x8c6198dcu); r[2]+=8u;goto L8c6198de; /* add #0x8,r2 */
L8c6198de: sa_motion_publish_stop(ctx,0x8c6198deu); r[4]+=4u;goto L8c6198e0; /* add #0x4,r4 */
L8c6198e0: sa_motion_publish_stop(ctx,0x8c6198e0u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6198e2; /* mov.l r2,@(0x4,r15) */
L8c6198e2: sa_motion_publish_stop(ctx,0x8c6198e2u); wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c6198e4; /* mov.l r1,@(0xc,r15) */
L8c6198e4: sa_motion_publish_stop(ctx,0x8c6198e6u); wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c619b90; /* bra 0x8c619b90 */
L8c6198e8: sa_motion_publish_stop(ctx,0x8c6198e8u); r[3]=rd(m,bus,&io_status,0x8c6199c0u,4);if(io_status)return io_status;goto L8c6198ea; /* mov.l 0x8c6199c0,r3 */
L8c6198ea: sa_motion_publish_stop(ctx,0x8c6198eau); r[5]=0u;goto L8c6198ec; /* mov #0x0,r5 */
L8c6198ec: sa_motion_publish_stop(ctx,0x8c6198ecu); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c6198ee; /* mov.w @r10+,r13 */
L8c6198ee: sa_motion_publish_stop(ctx,0x8c6198eeu); r[6]=0u;goto L8c6198f0; /* mov #0x0,r6 */
L8c6198f0: sa_motion_publish_stop(ctx,0x8c6198f0u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6198f2; /* mov.l @r3,r2 */
L8c6198f2: sa_motion_publish_stop(ctx,0x8c6198f2u); r[7]=r[5];goto L8c6198f4; /* mov r5,r7 */
L8c6198f4: sa_motion_publish_stop(ctx,0x8c6198f4u); r[13]=r[13]&65535u;goto L8c6198f6; /* extu.w r13,r13 */
L8c6198f6: sa_motion_publish_stop(ctx,0x8c6198f6u); r[2]+=r[13];goto L8c6198f8; /* add r13,r2 */
L8c6198f8: sa_motion_publish_stop(ctx,0x8c6198f8u); *sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c6198fa; /* cmp/pl r13 */
L8c6198fa: sa_motion_publish_stop(ctx,0x8c6198fau); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6198fc; /* mov.l r2,@r3 */
L8c6198fc: sa_motion_publish_stop(ctx,0x8c6198fcu); r[4]=4294967295u;goto L8c6198fe; /* mov #-0x1,r4 */
L8c6198fe: sa_motion_publish_stop(ctx,0x8c619900u); take=!(*sr&1u);r[6]+=r[10];if(take)goto L8c619920;goto L8c619902; /* bf/s 0x8c619920 */
L8c619902: sa_motion_publish_stop(ctx,0x8c619902u); r[14]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c619904; /* mov.w @r6+,r14 */
L8c619904: sa_motion_publish_stop(ctx,0x8c619904u); r[7]+=1u;goto L8c619906; /* add #0x1,r7 */
L8c619906: sa_motion_publish_stop(ctx,0x8c619906u); r[2]=rd(m,bus,&io_status,0x8c6199c4u,4);if(io_status)return io_status;goto L8c619908; /* mov.l 0x8c6199c4,r2 */
L8c619908: sa_motion_publish_stop(ctx,0x8c619908u); r[14]=r[14]&65535u;goto L8c61990a; /* extu.w r14,r14 */
L8c61990a: sa_motion_publish_stop(ctx,0x8c61990au); r[14]<<=2;goto L8c61990c; /* shll2 r14 */
L8c61990c: sa_motion_publish_stop(ctx,0x8c61990cu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61990e; /* mov.l @r2,r3 */
L8c61990e: sa_motion_publish_stop(ctx,0x8c61990eu); r[14]<<=2;goto L8c619910; /* shll2 r14 */
L8c619910: sa_motion_publish_stop(ctx,0x8c619910u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c619912; /* shll r14 */
L8c619912: sa_motion_publish_stop(ctx,0x8c619912u); *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[13]);goto L8c619914; /* cmp/ge r13,r7 */
L8c619914: sa_motion_publish_stop(ctx,0x8c619914u); r[14]+=r[3];goto L8c619916; /* add r3,r14 */
L8c619916: sa_motion_publish_stop(ctx,0x8c619916u); wr(m,bus,&io_status,r[15]+0x14u,r[14],4);if(io_status)return io_status;goto L8c619918; /* mov.l r14,@(0x14,r15) */
L8c619918: sa_motion_publish_stop(ctx,0x8c619918u); r[14]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61991a; /* mov.l @(0xc,r14),r14 */
L8c61991a: sa_motion_publish_stop(ctx,0x8c61991au); r[4]&=r[14];goto L8c61991c; /* and r14,r4 */
L8c61991c: sa_motion_publish_stop(ctx,0x8c61991eu); take=!(*sr&1u);r[5]|=r[14];if(take)goto L8c619902;goto L8c619920; /* bf/s 0x8c619902 */
L8c619920: sa_motion_publish_stop(ctx,0x8c619920u); r[2]=63u;goto L8c619922; /* mov #0x3f,r2 */
L8c619922: sa_motion_publish_stop(ctx,0x8c619922u); r[7]=r[13];goto L8c619924; /* mov r13,r7 */
L8c619924: sa_motion_publish_stop(ctx,0x8c619924u); r[14]=r[13];goto L8c619926; /* mov r13,r14 */
L8c619926: sa_motion_publish_stop(ctx,0x8c619926u); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c619928; /* shll r7 */
L8c619928: sa_motion_publish_stop(ctx,0x8c619928u); *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61992a; /* tst r2,r4 */
L8c61992a: sa_motion_publish_stop(ctx,0x8c61992cu); take=(*sr&1u);r[14]<<=2;if(take)goto L8c619934;goto L8c61992e; /* bt/s 0x8c619934 */
L8c61992e: sa_motion_publish_stop(ctx,0x8c61992eu); r[11]+=r[14];goto L8c619930; /* add r14,r11 */
L8c619930: sa_motion_publish_stop(ctx,0x8c619932u); r[10]+=r[7];goto L8c619b8a; /* bra 0x8c619b8a */
L8c619934: sa_motion_publish_stop(ctx,0x8c619934u); r[3]=1u;goto L8c619936; /* mov #0x1,r3 */
L8c619936: sa_motion_publish_stop(ctx,0x8c619936u); *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c619938; /* tst r3,r5 */
L8c619938: sa_motion_publish_stop(ctx,0x8c619938u); if((*sr&1u))goto L8c61993e;goto L8c61993a; /* bt 0x8c61993e */
L8c61993a: sa_motion_publish_stop(ctx,0x8c61993cu); goto L8c619aba; /* bra 0x8c619aba */
L8c61993e: sa_motion_publish_stop(ctx,0x8c61993eu); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c619940; /* mov.w @(0x2,r10),r0 */
L8c619940: sa_motion_publish_stop(ctx,0x8c619940u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c619942; /* mov.w @r10,r4 */
L8c619942: sa_motion_publish_stop(ctx,0x8c619942u); r[2]=rd(m,bus,&io_status,0x8c6199c4u,4);if(io_status)return io_status;goto L8c619944; /* mov.l 0x8c6199c4,r2 */
L8c619944: sa_motion_publish_stop(ctx,0x8c619944u); r[5]=r[0]&65535u;goto L8c619946; /* extu.w r0,r5 */
L8c619946: sa_motion_publish_stop(ctx,0x8c619946u); r[5]<<=2;goto L8c619948; /* shll2 r5 */
L8c619948: sa_motion_publish_stop(ctx,0x8c619948u); r[0]=sx(m,bus,&io_status,r[10]+0x4u,2);if(io_status)return io_status;goto L8c61994a; /* mov.w @(0x4,r10),r0 */
L8c61994a: sa_motion_publish_stop(ctx,0x8c61994au); r[4]=r[4]&65535u;goto L8c61994c; /* extu.w r4,r4 */
L8c61994c: sa_motion_publish_stop(ctx,0x8c61994cu); r[12]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61994e; /* mov.l @r2,r12 */
L8c61994e: sa_motion_publish_stop(ctx,0x8c61994eu); r[4]<<=2;goto L8c619950; /* shll2 r4 */
L8c619950: sa_motion_publish_stop(ctx,0x8c619950u); r[5]<<=2;goto L8c619952; /* shll2 r5 */
L8c619952: sa_motion_publish_stop(ctx,0x8c619952u); r[6]=r[0]&65535u;goto L8c619954; /* extu.w r0,r6 */
L8c619954: sa_motion_publish_stop(ctx,0x8c619954u); r[4]<<=2;goto L8c619956; /* shll2 r4 */
L8c619956: sa_motion_publish_stop(ctx,0x8c619956u); r[0]=16u;goto L8c619958; /* mov #0x10,r0 */
L8c619958: sa_motion_publish_stop(ctx,0x8c619958u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61995a; /* shll r4 */
L8c61995a: sa_motion_publish_stop(ctx,0x8c61995au); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61995c; /* shll r5 */
L8c61995c: sa_motion_publish_stop(ctx,0x8c61995cu); r[5]+=r[12];goto L8c61995e; /* add r12,r5 */
L8c61995e: sa_motion_publish_stop(ctx,0x8c61995eu); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c619960; /* fmov.s @(r0,r5),fr5 */
L8c619960: sa_motion_publish_stop(ctx,0x8c619960u); r[6]<<=2;goto L8c619962; /* shll2 r6 */
L8c619962: sa_motion_publish_stop(ctx,0x8c619962u); r[4]+=r[12];goto L8c619964; /* add r12,r4 */
L8c619964: sa_motion_publish_stop(ctx,0x8c619964u); r[6]<<=2;goto L8c619966; /* shll2 r6 */
L8c619966: sa_motion_publish_stop(ctx,0x8c619966u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619968; /* fmov.s @(r0,r4),fr3 */
L8c619968: sa_motion_publish_stop(ctx,0x8c619968u); fcopy(f,2u,5u);goto L8c61996a; /* fmov fr5,fr2 */
L8c61996a: sa_motion_publish_stop(ctx,0x8c61996au); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61996c; /* shll r6 */
L8c61996c: sa_motion_publish_stop(ctx,0x8c61996cu); r[0]=20u;goto L8c61996e; /* mov #0x14,r0 */
L8c61996e: sa_motion_publish_stop(ctx,0x8c61996eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c619970; /* fsub fr3,fr2 */
L8c619970: sa_motion_publish_stop(ctx,0x8c619970u); r[6]+=r[12];goto L8c619972; /* add r12,r6 */
L8c619972: sa_motion_publish_stop(ctx,0x8c619972u); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c619974; /* fmov.s @(r0,r5),fr4 */
L8c619974: sa_motion_publish_stop(ctx,0x8c619974u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619976; /* fmov.s @(r0,r6),fr3 */
L8c619976: sa_motion_publish_stop(ctx,0x8c619976u); fcopy(f,1u,4u);goto L8c619978; /* fmov fr4,fr1 */
L8c619978: sa_motion_publish_stop(ctx,0x8c619978u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61997a; /* fsub fr4,fr3 */
L8c61997a: sa_motion_publish_stop(ctx,0x8c61997au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61997c; /* fmul fr3,fr2 */
L8c61997c: sa_motion_publish_stop(ctx,0x8c61997cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61997e; /* fmov.s @(r0,r4),fr3 */
L8c61997e: sa_motion_publish_stop(ctx,0x8c61997eu); r[0]=16u;goto L8c619980; /* mov #0x10,r0 */
L8c619980: sa_motion_publish_stop(ctx,0x8c619980u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c619982; /* fsub fr3,fr1 */
L8c619982: sa_motion_publish_stop(ctx,0x8c619982u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619984; /* fmov.s @(r0,r6),fr3 */
L8c619984: sa_motion_publish_stop(ctx,0x8c619984u); fcopy(f,4u,2u);goto L8c619986; /* fmov fr2,fr4 */
L8c619986: sa_motion_publish_stop(ctx,0x8c619986u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619988; /* fsub fr5,fr3 */
L8c619988: sa_motion_publish_stop(ctx,0x8c619988u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61998a; /* fmul fr3,fr1 */
L8c61998a: sa_motion_publish_stop(ctx,0x8c61998au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61998c; /* fldi0 fr3 */
L8c61998c: sa_motion_publish_stop(ctx,0x8c61998cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61998e; /* fsub fr1,fr4 */
L8c61998e: sa_motion_publish_stop(ctx,0x8c61998eu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c619990; /* fcmp/gt fr3,fr4 */
L8c619990: sa_motion_publish_stop(ctx,0x8c619990u); if(!(*sr&1u))goto L8c6199c8;goto L8c619992; /* bf 0x8c6199c8 */
L8c619992: sa_motion_publish_stop(ctx,0x8c619992u); r[10]+=r[7];goto L8c619994; /* add r7,r10 */
L8c619994: sa_motion_publish_stop(ctx,0x8c619996u); r[11]+=r[14];goto L8c619b8a; /* bra 0x8c619b8a */
L8c6199c8: sa_motion_publish_stop(ctx,0x8c6199c8u); r[3]=rd(m,bus,&io_status,0x8c619c04u,4);if(io_status)return io_status;goto L8c6199ca; /* mov.l 0x8c619c04,r3 */
L8c6199ca: sa_motion_publish_stop(ctx,0x8c6199cau); r[6]=r[13];goto L8c6199cc; /* mov r13,r6 */
L8c6199cc: sa_motion_publish_stop(ctx,0x8c6199ccu); r[6]+=4294967295u;goto L8c6199ce; /* add #-0x1,r6 */
L8c6199ce: sa_motion_publish_stop(ctx,0x8c6199ceu); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6199d0; /* mov.l @r3,r2 */
L8c6199d0: sa_motion_publish_stop(ctx,0x8c6199d0u); *sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c6199d2; /* cmp/pl r6 */
L8c6199d2: sa_motion_publish_stop(ctx,0x8c6199d2u); r[2]+=r[13];goto L8c6199d4; /* add r13,r2 */
L8c6199d4: sa_motion_publish_stop(ctx,0x8c6199d4u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6199d6; /* mov.l r2,@r3 */
L8c6199d6: sa_motion_publish_stop(ctx,0x8c6199d8u); take=!(*sr&1u);r[5]=0u;if(take)goto L8c619a4c;goto L8c6199da; /* bf/s 0x8c619a4c */
L8c6199da: sa_motion_publish_stop(ctx,0x8c6199dau); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c6199dc; /* mov.w @r10+,r4 */
L8c6199dc: sa_motion_publish_stop(ctx,0x8c6199dcu); r[0]=4u;goto L8c6199de; /* mov #0x4,r0 */
L8c6199de: sa_motion_publish_stop(ctx,0x8c6199deu); r[2]=rd(m,bus,&io_status,0x8c619c08u,4);if(io_status)return io_status;goto L8c6199e0; /* mov.l 0x8c619c08,r2 */
L8c6199e0: sa_motion_publish_stop(ctx,0x8c6199e0u); r[8]+=28u;goto L8c6199e2; /* add #0x1c,r8 */
L8c6199e2: sa_motion_publish_stop(ctx,0x8c6199e2u); r[4]=r[4]&65535u;goto L8c6199e4; /* extu.w r4,r4 */
L8c6199e4: sa_motion_publish_stop(ctx,0x8c6199e4u); r[1]=rd(m,bus,&io_status,0x8c619c10u,4);if(io_status)return io_status;goto L8c6199e6; /* mov.l 0x8c619c10,r1 */
L8c6199e6: sa_motion_publish_stop(ctx,0x8c6199e6u); r[4]<<=2;goto L8c6199e8; /* shll2 r4 */
L8c6199e8: sa_motion_publish_stop(ctx,0x8c6199e8u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6199ea; /* mov.l @r2,r3 */
L8c6199ea: sa_motion_publish_stop(ctx,0x8c6199eau); r[4]<<=2;goto L8c6199ec; /* shll2 r4 */
L8c6199ec: sa_motion_publish_stop(ctx,0x8c6199ecu); r[2]=rd(m,bus,&io_status,0x8c619c14u,4);if(io_status)return io_status;goto L8c6199ee; /* mov.l 0x8c619c14,r2 */
L8c6199ee: sa_motion_publish_stop(ctx,0x8c6199eeu); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6199f0; /* shll r4 */
L8c6199f0: sa_motion_publish_stop(ctx,0x8c6199f0u); r[4]+=r[3];goto L8c6199f2; /* add r3,r4 */
L8c6199f2: sa_motion_publish_stop(ctx,0x8c6199f2u); r[3]=rd(m,bus,&io_status,0x8c619c0cu,4);if(io_status)return io_status;goto L8c6199f4; /* mov.l 0x8c619c0c,r3 */
L8c6199f4: sa_motion_publish_stop(ctx,0x8c6199f4u); fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c6199f6; /* fmov.s @r4,fr3 */
L8c6199f6: sa_motion_publish_stop(ctx,0x8c6199f6u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6199f8; /* fmov.s @r3,fr2 */
L8c6199f8: sa_motion_publish_stop(ctx,0x8c6199f8u); r[3]+=8u;goto L8c6199fa; /* add #0x8,r3 */
L8c6199fa: sa_motion_publish_stop(ctx,0x8c6199fau); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c6199fc; /* fmov.s @r2,fr1 */
L8c6199fc: sa_motion_publish_stop(ctx,0x8c6199fcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6199fe; /* fmul fr3,fr2 */
L8c6199fe: sa_motion_publish_stop(ctx,0x8c6199feu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c619a00; /* fmov.s @r1,fr3 */
L8c619a00: sa_motion_publish_stop(ctx,0x8c619a00u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619a02; /* fsub fr2,fr3 */
L8c619a02: sa_motion_publish_stop(ctx,0x8c619a02u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c619a04; /* fmov.s @(r0,r4),fr2 */
L8c619a04: sa_motion_publish_stop(ctx,0x8c619a04u); r[0]=8u;goto L8c619a06; /* mov #0x8,r0 */
L8c619a06: sa_motion_publish_stop(ctx,0x8c619a06u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619a08; /* fmul fr2,fr1 */
L8c619a08: sa_motion_publish_stop(ctx,0x8c619a08u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c619a0a; /* fmov.s @(r0,r4),fr2 */
L8c619a0a: sa_motion_publish_stop(ctx,0x8c619a0au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619a0c; /* fsub fr1,fr3 */
L8c619a0c: sa_motion_publish_stop(ctx,0x8c619a0cu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619a0e; /* fmov.s @r3,fr1 */
L8c619a0e: sa_motion_publish_stop(ctx,0x8c619a0eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619a10; /* fmul fr2,fr1 */
L8c619a10: sa_motion_publish_stop(ctx,0x8c619a10u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619a12; /* fsub fr1,fr3 */
L8c619a12: sa_motion_publish_stop(ctx,0x8c619a12u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a14; /* fmov.s fr3,@-r8 */
L8c619a14: sa_motion_publish_stop(ctx,0x8c619a14u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619a16; /* mov.w @(0x2,r11),r0 */
L8c619a16: sa_motion_publish_stop(ctx,0x8c619a16u); r[2]=r[0];goto L8c619a18; /* mov r0,r2 */
L8c619a18: sa_motion_publish_stop(ctx,0x8c619a18u); *fpul=r[2];goto L8c619a1a; /* lds r2,FPUL */
L8c619a1a: sa_motion_publish_stop(ctx,0x8c619a1au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619a1c; /* float FPUL,fr3 */
L8c619a1c: sa_motion_publish_stop(ctx,0x8c619a1cu); r[0]=24u;goto L8c619a1e; /* mov #0x18,r0 */
L8c619a1e: sa_motion_publish_stop(ctx,0x8c619a1eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619a20; /* fmul fr15,fr3 */
L8c619a20: sa_motion_publish_stop(ctx,0x8c619a20u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a22; /* fmov.s fr3,@-r8 */
L8c619a22: sa_motion_publish_stop(ctx,0x8c619a22u); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619a24; /* mov.w @r11,r2 */
L8c619a24: sa_motion_publish_stop(ctx,0x8c619a24u); r[11]+=4u;goto L8c619a26; /* add #0x4,r11 */
L8c619a26: sa_motion_publish_stop(ctx,0x8c619a26u); *fpul=r[2];goto L8c619a28; /* lds r2,FPUL */
L8c619a28: sa_motion_publish_stop(ctx,0x8c619a28u); r[2]=rd(m,bus,&io_status,0x8c619c18u,4);if(io_status)return io_status;goto L8c619a2a; /* mov.l 0x8c619c18,r2 */
L8c619a2a: sa_motion_publish_stop(ctx,0x8c619a2au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619a2c; /* float FPUL,fr3 */
L8c619a2c: sa_motion_publish_stop(ctx,0x8c619a2cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619a2e; /* fmul fr15,fr3 */
L8c619a2e: sa_motion_publish_stop(ctx,0x8c619a2eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a30; /* fmov.s fr3,@-r8 */
L8c619a30: sa_motion_publish_stop(ctx,0x8c619a30u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619a32; /* fmov.s @(r0,r4),fr3 */
L8c619a32: sa_motion_publish_stop(ctx,0x8c619a32u); r[0]=20u;goto L8c619a34; /* mov #0x14,r0 */
L8c619a34: sa_motion_publish_stop(ctx,0x8c619a34u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a36; /* fmov.s fr3,@-r8 */
L8c619a36: sa_motion_publish_stop(ctx,0x8c619a36u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619a38; /* fmov.s @(r0,r4),fr3 */
L8c619a38: sa_motion_publish_stop(ctx,0x8c619a38u); r[0]=16u;goto L8c619a3a; /* mov #0x10,r0 */
L8c619a3a: sa_motion_publish_stop(ctx,0x8c619a3au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a3c; /* fmov.s fr3,@-r8 */
L8c619a3c: sa_motion_publish_stop(ctx,0x8c619a3cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619a3e; /* fmov.s @(r0,r4),fr3 */
L8c619a3e: sa_motion_publish_stop(ctx,0x8c619a3eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a40; /* fmov.s fr3,@-r8 */
L8c619a40: sa_motion_publish_stop(ctx,0x8c619a40u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c619a42; /* mov.l r2,@-r8 */
L8c619a42: sa_motion_publish_stop(ctx,0x8c619a42u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619a44; /* pref @r8 */
L8c619a44: sa_motion_publish_stop(ctx,0x8c619a44u); r[5]+=1u;goto L8c619a46; /* add #0x1,r5 */
L8c619a46: sa_motion_publish_stop(ctx,0x8c619a46u); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c619a48; /* cmp/ge r6,r5 */
L8c619a48: sa_motion_publish_stop(ctx,0x8c619a4au); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c6199da;goto L8c619a4c; /* bf/s 0x8c6199da */
L8c619a4c: sa_motion_publish_stop(ctx,0x8c619a4cu); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c619a4e; /* mov.w @r10+,r4 */
L8c619a4e: sa_motion_publish_stop(ctx,0x8c619a4eu); r[0]=4u;goto L8c619a50; /* mov #0x4,r0 */
L8c619a50: sa_motion_publish_stop(ctx,0x8c619a50u); r[2]=rd(m,bus,&io_status,0x8c619c08u,4);if(io_status)return io_status;goto L8c619a52; /* mov.l 0x8c619c08,r2 */
L8c619a52: sa_motion_publish_stop(ctx,0x8c619a52u); r[8]+=28u;goto L8c619a54; /* add #0x1c,r8 */
L8c619a54: sa_motion_publish_stop(ctx,0x8c619a54u); r[4]=r[4]&65535u;goto L8c619a56; /* extu.w r4,r4 */
L8c619a56: sa_motion_publish_stop(ctx,0x8c619a56u); r[1]=rd(m,bus,&io_status,0x8c619c10u,4);if(io_status)return io_status;goto L8c619a58; /* mov.l 0x8c619c10,r1 */
L8c619a58: sa_motion_publish_stop(ctx,0x8c619a58u); r[4]<<=2;goto L8c619a5a; /* shll2 r4 */
L8c619a5a: sa_motion_publish_stop(ctx,0x8c619a5au); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c619a5c; /* mov.l @r2,r3 */
L8c619a5c: sa_motion_publish_stop(ctx,0x8c619a5cu); r[4]<<=2;goto L8c619a5e; /* shll2 r4 */
L8c619a5e: sa_motion_publish_stop(ctx,0x8c619a5eu); r[2]=rd(m,bus,&io_status,0x8c619c14u,4);if(io_status)return io_status;goto L8c619a60; /* mov.l 0x8c619c14,r2 */
L8c619a60: sa_motion_publish_stop(ctx,0x8c619a60u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c619a62; /* shll r4 */
L8c619a62: sa_motion_publish_stop(ctx,0x8c619a62u); r[4]+=r[3];goto L8c619a64; /* add r3,r4 */
L8c619a64: sa_motion_publish_stop(ctx,0x8c619a64u); r[3]=rd(m,bus,&io_status,0x8c619c0cu,4);if(io_status)return io_status;goto L8c619a66; /* mov.l 0x8c619c0c,r3 */
L8c619a66: sa_motion_publish_stop(ctx,0x8c619a66u); fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c619a68; /* fmov.s @r4,fr3 */
L8c619a68: sa_motion_publish_stop(ctx,0x8c619a68u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c619a6a; /* fmov.s @r3,fr2 */
L8c619a6a: sa_motion_publish_stop(ctx,0x8c619a6au); r[3]+=8u;goto L8c619a6c; /* add #0x8,r3 */
L8c619a6c: sa_motion_publish_stop(ctx,0x8c619a6cu); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c619a6e; /* fmov.s @r2,fr1 */
L8c619a6e: sa_motion_publish_stop(ctx,0x8c619a6eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619a70; /* fmul fr3,fr2 */
L8c619a70: sa_motion_publish_stop(ctx,0x8c619a70u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c619a72; /* fmov.s @r1,fr3 */
L8c619a72: sa_motion_publish_stop(ctx,0x8c619a72u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619a74; /* fsub fr2,fr3 */
L8c619a74: sa_motion_publish_stop(ctx,0x8c619a74u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c619a76; /* fmov.s @(r0,r4),fr2 */
L8c619a76: sa_motion_publish_stop(ctx,0x8c619a76u); r[0]=8u;goto L8c619a78; /* mov #0x8,r0 */
L8c619a78: sa_motion_publish_stop(ctx,0x8c619a78u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619a7a; /* fmul fr2,fr1 */
L8c619a7a: sa_motion_publish_stop(ctx,0x8c619a7au); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c619a7c; /* fmov.s @(r0,r4),fr2 */
L8c619a7c: sa_motion_publish_stop(ctx,0x8c619a7cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619a7e; /* fsub fr1,fr3 */
L8c619a7e: sa_motion_publish_stop(ctx,0x8c619a7eu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619a80; /* fmov.s @r3,fr1 */
L8c619a80: sa_motion_publish_stop(ctx,0x8c619a80u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619a82; /* fmul fr2,fr1 */
L8c619a82: sa_motion_publish_stop(ctx,0x8c619a82u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619a84; /* fsub fr1,fr3 */
L8c619a84: sa_motion_publish_stop(ctx,0x8c619a84u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a86; /* fmov.s fr3,@-r8 */
L8c619a86: sa_motion_publish_stop(ctx,0x8c619a86u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619a88; /* mov.w @(0x2,r11),r0 */
L8c619a88: sa_motion_publish_stop(ctx,0x8c619a88u); r[2]=r[0];goto L8c619a8a; /* mov r0,r2 */
L8c619a8a: sa_motion_publish_stop(ctx,0x8c619a8au); *fpul=r[2];goto L8c619a8c; /* lds r2,FPUL */
L8c619a8c: sa_motion_publish_stop(ctx,0x8c619a8cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619a8e; /* float FPUL,fr3 */
L8c619a8e: sa_motion_publish_stop(ctx,0x8c619a8eu); r[0]=24u;goto L8c619a90; /* mov #0x18,r0 */
L8c619a90: sa_motion_publish_stop(ctx,0x8c619a90u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619a92; /* fmul fr15,fr3 */
L8c619a92: sa_motion_publish_stop(ctx,0x8c619a92u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619a94; /* fmov.s fr3,@-r8 */
L8c619a94: sa_motion_publish_stop(ctx,0x8c619a94u); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619a96; /* mov.w @r11,r2 */
L8c619a96: sa_motion_publish_stop(ctx,0x8c619a96u); r[11]+=4u;goto L8c619a98; /* add #0x4,r11 */
L8c619a98: sa_motion_publish_stop(ctx,0x8c619a98u); *fpul=r[2];goto L8c619a9a; /* lds r2,FPUL */
L8c619a9a: sa_motion_publish_stop(ctx,0x8c619a9au); r[2]=rd(m,bus,&io_status,0x8c619c1cu,4);if(io_status)return io_status;goto L8c619a9c; /* mov.l 0x8c619c1c,r2 */
L8c619a9c: sa_motion_publish_stop(ctx,0x8c619a9cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619a9e; /* float FPUL,fr3 */
L8c619a9e: sa_motion_publish_stop(ctx,0x8c619a9eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619aa0; /* fmul fr15,fr3 */
L8c619aa0: sa_motion_publish_stop(ctx,0x8c619aa0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619aa2; /* fmov.s fr3,@-r8 */
L8c619aa2: sa_motion_publish_stop(ctx,0x8c619aa2u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619aa4; /* fmov.s @(r0,r4),fr3 */
L8c619aa4: sa_motion_publish_stop(ctx,0x8c619aa4u); r[0]=20u;goto L8c619aa6; /* mov #0x14,r0 */
L8c619aa6: sa_motion_publish_stop(ctx,0x8c619aa6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619aa8; /* fmov.s fr3,@-r8 */
L8c619aa8: sa_motion_publish_stop(ctx,0x8c619aa8u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619aaa; /* fmov.s @(r0,r4),fr3 */
L8c619aaa: sa_motion_publish_stop(ctx,0x8c619aaau); r[0]=16u;goto L8c619aac; /* mov #0x10,r0 */
L8c619aac: sa_motion_publish_stop(ctx,0x8c619aacu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619aae; /* fmov.s fr3,@-r8 */
L8c619aae: sa_motion_publish_stop(ctx,0x8c619aaeu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c619ab0; /* fmov.s @(r0,r4),fr3 */
L8c619ab0: sa_motion_publish_stop(ctx,0x8c619ab0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619ab2; /* fmov.s fr3,@-r8 */
L8c619ab2: sa_motion_publish_stop(ctx,0x8c619ab2u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c619ab4; /* mov.l r2,@-r8 */
L8c619ab4: sa_motion_publish_stop(ctx,0x8c619ab4u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619ab6; /* pref @r8 */
L8c619ab6: sa_motion_publish_stop(ctx,0x8c619ab8u); r[8]+=32u;goto L8c619b8a; /* bra 0x8c619b8a */
L8c619aba: sa_motion_publish_stop(ctx,0x8c619abau); r[1]=rd(m,bus,&io_status,0x8c619c20u,4);if(io_status)return io_status;goto L8c619abc; /* mov.l 0x8c619c20,r1 */
L8c619abc: sa_motion_publish_stop(ctx,0x8c619abcu); r[2]=0u;goto L8c619abe; /* mov #0x0,r2 */
L8c619abe: sa_motion_publish_stop(ctx,0x8c619abeu); r[0]=rd(m,bus,&io_status,0x8c619c08u,4);if(io_status)return io_status;goto L8c619ac0; /* mov.l 0x8c619c08,r0 */
L8c619ac0: sa_motion_publish_stop(ctx,0x8c619ac0u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c619ac2; /* mov.l r2,@r1 */
L8c619ac2: sa_motion_publish_stop(ctx,0x8c619ac2u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c619ac4; /* mov.w @r10+,r4 */
L8c619ac4: sa_motion_publish_stop(ctx,0x8c619ac4u); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c619ac6; /* mov.l @r0,r2 */
L8c619ac6: sa_motion_publish_stop(ctx,0x8c619ac6u); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c619ac8; /* mov.w @r10+,r14 */
L8c619ac8: sa_motion_publish_stop(ctx,0x8c619ac8u); r[4]=r[4]&65535u;goto L8c619aca; /* extu.w r4,r4 */
L8c619aca: sa_motion_publish_stop(ctx,0x8c619acau); r[4]<<=2;goto L8c619acc; /* shll2 r4 */
L8c619acc: sa_motion_publish_stop(ctx,0x8c619accu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c619ace; /* mov.l @(0xc,r15),r3 */
L8c619ace: sa_motion_publish_stop(ctx,0x8c619aceu); r[14]=r[14]&65535u;goto L8c619ad0; /* extu.w r14,r14 */
L8c619ad0: sa_motion_publish_stop(ctx,0x8c619ad0u); r[14]<<=2;goto L8c619ad2; /* shll2 r14 */
L8c619ad2: sa_motion_publish_stop(ctx,0x8c619ad2u); r[14]<<=2;goto L8c619ad4; /* shll2 r14 */
L8c619ad4: sa_motion_publish_stop(ctx,0x8c619ad4u); r[4]<<=2;goto L8c619ad6; /* shll2 r4 */
L8c619ad6: sa_motion_publish_stop(ctx,0x8c619ad6u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c619ad8; /* shll r4 */
L8c619ad8: sa_motion_publish_stop(ctx,0x8c619ad8u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c619ada; /* shll r14 */
L8c619ada: sa_motion_publish_stop(ctx,0x8c619adau); r[14]+=r[2];goto L8c619adc; /* add r2,r14 */
L8c619adc: sa_motion_publish_stop(ctx,0x8c619adcu); r[4]+=r[2];goto L8c619ade; /* add r2,r4 */
L8c619ade: sa_motion_publish_stop(ctx,0x8c619adeu); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619ae0; /* mov.w @r11,r2 */
L8c619ae0: sa_motion_publish_stop(ctx,0x8c619ae0u); *fpul=r[2];goto L8c619ae2; /* lds r2,FPUL */
L8c619ae2: sa_motion_publish_stop(ctx,0x8c619ae2u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619ae4; /* float FPUL,fr3 */
L8c619ae4: sa_motion_publish_stop(ctx,0x8c619ae4u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619ae6; /* ftrc fr3,FPUL */
L8c619ae6: sa_motion_publish_stop(ctx,0x8c619ae6u); r[1]=*fpul;goto L8c619ae8; /* sts FPUL,r1 */
L8c619ae8: sa_motion_publish_stop(ctx,0x8c619ae8u); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c619aea; /* mov.w r1,@r3 */
L8c619aea: sa_motion_publish_stop(ctx,0x8c619aeau); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619aec; /* mov.w @(0x2,r11),r0 */
L8c619aec: sa_motion_publish_stop(ctx,0x8c619aecu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c619aee; /* mov.l @(0xc,r15),r3 */
L8c619aee: sa_motion_publish_stop(ctx,0x8c619aeeu); r[2]=r[0];goto L8c619af0; /* mov r0,r2 */
L8c619af0: sa_motion_publish_stop(ctx,0x8c619af0u); *fpul=r[2];goto L8c619af2; /* lds r2,FPUL */
L8c619af2: sa_motion_publish_stop(ctx,0x8c619af2u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619af4; /* float FPUL,fr3 */
L8c619af4: sa_motion_publish_stop(ctx,0x8c619af4u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619af6; /* ftrc fr3,FPUL */
L8c619af6: sa_motion_publish_stop(ctx,0x8c619af6u); r[0]=*fpul;goto L8c619af8; /* sts FPUL,r0 */
L8c619af8: sa_motion_publish_stop(ctx,0x8c619af8u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619afa; /* mov.w r0,@(0x2,r3) */
L8c619afa: sa_motion_publish_stop(ctx,0x8c619afau); r[11]+=4u;goto L8c619afc; /* add #0x4,r11 */
L8c619afc: sa_motion_publish_stop(ctx,0x8c619afcu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c619afe; /* mov.l @r15,r3 */
L8c619afe: sa_motion_publish_stop(ctx,0x8c619afeu); r[2]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619b00; /* mov.w @r11,r2 */
L8c619b00: sa_motion_publish_stop(ctx,0x8c619b00u); r[9]=r[13];goto L8c619b02; /* mov r13,r9 */
L8c619b02: sa_motion_publish_stop(ctx,0x8c619b02u); r[9]+=4294967294u;goto L8c619b04; /* add #-0x2,r9 */
L8c619b04: sa_motion_publish_stop(ctx,0x8c619b04u); *fpul=r[2];goto L8c619b06; /* lds r2,FPUL */
L8c619b06: sa_motion_publish_stop(ctx,0x8c619b06u); *sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c619b08; /* cmp/pl r9 */
L8c619b08: sa_motion_publish_stop(ctx,0x8c619b08u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619b0a; /* float FPUL,fr3 */
L8c619b0a: sa_motion_publish_stop(ctx,0x8c619b0au); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619b0c; /* ftrc fr3,FPUL */
L8c619b0c: sa_motion_publish_stop(ctx,0x8c619b0cu); r[1]=*fpul;goto L8c619b0e; /* sts FPUL,r1 */
L8c619b0e: sa_motion_publish_stop(ctx,0x8c619b0eu); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c619b10; /* mov.w r1,@r3 */
L8c619b10: sa_motion_publish_stop(ctx,0x8c619b10u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619b12; /* mov.w @(0x2,r11),r0 */
L8c619b12: sa_motion_publish_stop(ctx,0x8c619b12u); r[11]+=4u;goto L8c619b14; /* add #0x4,r11 */
L8c619b14: sa_motion_publish_stop(ctx,0x8c619b14u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c619b16; /* mov.l @r15,r3 */
L8c619b16: sa_motion_publish_stop(ctx,0x8c619b16u); r[2]=r[0];goto L8c619b18; /* mov r0,r2 */
L8c619b18: sa_motion_publish_stop(ctx,0x8c619b18u); *fpul=r[2];goto L8c619b1a; /* lds r2,FPUL */
L8c619b1a: sa_motion_publish_stop(ctx,0x8c619b1au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619b1c; /* float FPUL,fr3 */
L8c619b1c: sa_motion_publish_stop(ctx,0x8c619b1cu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619b1e; /* ftrc fr3,FPUL */
L8c619b1e: sa_motion_publish_stop(ctx,0x8c619b1eu); r[0]=*fpul;goto L8c619b20; /* sts FPUL,r0 */
L8c619b20: sa_motion_publish_stop(ctx,0x8c619b20u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619b22; /* mov.w r0,@(0x2,r3) */
L8c619b22: sa_motion_publish_stop(ctx,0x8c619b24u); take=!(*sr&1u);r[12]=0u;if(take)goto L8c619b8a;goto L8c619b26; /* bf/s 0x8c619b8a */
L8c619b26: sa_motion_publish_stop(ctx,0x8c619b26u); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c619b28; /* mov.w @r10+,r13 */
L8c619b28: sa_motion_publish_stop(ctx,0x8c619b28u); r[2]=rd(m,bus,&io_status,0x8c619c08u,4);if(io_status)return io_status;goto L8c619b2a; /* mov.l 0x8c619c08,r2 */
L8c619b2a: sa_motion_publish_stop(ctx,0x8c619b2au); r[13]=r[13]&65535u;goto L8c619b2c; /* extu.w r13,r13 */
L8c619b2c: sa_motion_publish_stop(ctx,0x8c619b2cu); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c619b2e; /* mov.l @(0x4,r15),r1 */
L8c619b2e: sa_motion_publish_stop(ctx,0x8c619b2eu); r[13]<<=2;goto L8c619b30; /* shll2 r13 */
L8c619b30: sa_motion_publish_stop(ctx,0x8c619b30u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c619b32; /* mov.l @r2,r3 */
L8c619b32: sa_motion_publish_stop(ctx,0x8c619b32u); r[13]<<=2;goto L8c619b34; /* shll2 r13 */
L8c619b34: sa_motion_publish_stop(ctx,0x8c619b34u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c619b36; /* shll r13 */
L8c619b36: sa_motion_publish_stop(ctx,0x8c619b36u); r[13]+=r[3];goto L8c619b38; /* add r3,r13 */
L8c619b38: sa_motion_publish_stop(ctx,0x8c619b38u); r[3]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c619b3a; /* mov.w @r11,r3 */
L8c619b3a: sa_motion_publish_stop(ctx,0x8c619b3au); *fpul=r[3];goto L8c619b3c; /* lds r3,FPUL */
L8c619b3c: sa_motion_publish_stop(ctx,0x8c619b3cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619b3e; /* float FPUL,fr3 */
L8c619b3e: sa_motion_publish_stop(ctx,0x8c619b3eu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619b40; /* ftrc fr3,FPUL */
L8c619b40: sa_motion_publish_stop(ctx,0x8c619b40u); r[0]=*fpul;goto L8c619b42; /* sts FPUL,r0 */
L8c619b42: sa_motion_publish_stop(ctx,0x8c619b42u); wr(m,bus,&io_status,r[1],r[0],2);if(io_status)return io_status;goto L8c619b44; /* mov.w r0,@r1 */
L8c619b44: sa_motion_publish_stop(ctx,0x8c619b44u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c619b46; /* mov.w @(0x2,r11),r0 */
L8c619b46: sa_motion_publish_stop(ctx,0x8c619b46u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c619b48; /* mov.l @(0x4,r15),r3 */
L8c619b48: sa_motion_publish_stop(ctx,0x8c619b48u); r[1]=r[0];goto L8c619b4a; /* mov r0,r1 */
L8c619b4a: sa_motion_publish_stop(ctx,0x8c619b4au); *fpul=r[1];goto L8c619b4c; /* lds r1,FPUL */
L8c619b4c: sa_motion_publish_stop(ctx,0x8c619b4cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c619b4e; /* float FPUL,fr3 */
L8c619b4e: sa_motion_publish_stop(ctx,0x8c619b4eu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c619b50; /* ftrc fr3,FPUL */
L8c619b50: sa_motion_publish_stop(ctx,0x8c619b50u); r[0]=*fpul;goto L8c619b52; /* sts FPUL,r0 */
L8c619b52: sa_motion_publish_stop(ctx,0x8c619b52u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c619b54; /* mov.w r0,@(0x2,r3) */
L8c619b54: sa_motion_publish_stop(ctx,0x8c619b54u); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c619b56; /* mov.l @(0xc,r4),r5 */
L8c619b56: sa_motion_publish_stop(ctx,0x8c619b56u); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c619b58; /* mov.l @(0xc,r14),r3 */
L8c619b58: sa_motion_publish_stop(ctx,0x8c619b58u); r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c619b5a; /* mov.l @(0xc,r13),r1 */
L8c619b5a: sa_motion_publish_stop(ctx,0x8c619b5au); r[5]&=r[3];goto L8c619b5c; /* and r3,r5 */
L8c619b5c: sa_motion_publish_stop(ctx,0x8c619b5cu); r[3]=1u;goto L8c619b5e; /* mov #0x1,r3 */
L8c619b5e: sa_motion_publish_stop(ctx,0x8c619b5eu); r[5]&=r[1];goto L8c619b60; /* and r1,r5 */
L8c619b60: sa_motion_publish_stop(ctx,0x8c619b60u); *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c619b62; /* tst r3,r5 */
L8c619b62: sa_motion_publish_stop(ctx,0x8c619b64u); take=!(*sr&1u);r[11]+=4u;if(take)goto L8c619b70;goto L8c619b66; /* bf/s 0x8c619b70 */
L8c619b66: sa_motion_publish_stop(ctx,0x8c619b66u); r[1]=rd(m,bus,&io_status,0x8c619c28u,4);if(io_status)return io_status;goto L8c619b68; /* mov.l 0x8c619c28,r1 */
L8c619b68: sa_motion_publish_stop(ctx,0x8c619b68u); r[6]=r[13];goto L8c619b6a; /* mov r13,r6 */
L8c619b6a: sa_motion_publish_stop(ctx,0x8c619b6au); r[7]=rd(m,bus,&io_status,0x8c619c24u,4);if(io_status)return io_status;goto L8c619b6c; /* mov.l 0x8c619c24,r7 */
L8c619b6c: sa_motion_publish_stop(ctx,0x8c619b6eu); target=r[1];*pr=0x8c619b70u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c619b70; /* jsr @r1 */
L8c619b70: sa_motion_publish_stop(ctx,0x8c619b70u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c619b72; /* mov.l @r15,r2 */
L8c619b72: sa_motion_publish_stop(ctx,0x8c619b72u); r[12]+=1u;goto L8c619b74; /* add #0x1,r12 */
L8c619b74: sa_motion_publish_stop(ctx,0x8c619b74u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c619b76; /* mov.l @(0xc,r15),r3 */
L8c619b76: sa_motion_publish_stop(ctx,0x8c619b76u); *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[9]);goto L8c619b78; /* cmp/ge r9,r12 */
L8c619b78: sa_motion_publish_stop(ctx,0x8c619b78u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c619b7a; /* mov.l @r2,r1 */
L8c619b7a: sa_motion_publish_stop(ctx,0x8c619b7au); r[4]=r[14];goto L8c619b7c; /* mov r14,r4 */
L8c619b7c: sa_motion_publish_stop(ctx,0x8c619b7cu); wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c619b7e; /* mov.l r1,@r3 */
L8c619b7e: sa_motion_publish_stop(ctx,0x8c619b7eu); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c619b80; /* mov.l @(0x4,r15),r3 */
L8c619b80: sa_motion_publish_stop(ctx,0x8c619b80u); r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c619b82; /* mov.l @r15,r0 */
L8c619b82: sa_motion_publish_stop(ctx,0x8c619b82u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c619b84; /* mov.l @r3,r2 */
L8c619b84: sa_motion_publish_stop(ctx,0x8c619b84u); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c619b86; /* mov.l r2,@r0 */
L8c619b86: sa_motion_publish_stop(ctx,0x8c619b88u); take=!(*sr&1u);r[14]=r[13];if(take)goto L8c619b26;goto L8c619b8a; /* bf/s 0x8c619b26 */
L8c619b8a: sa_motion_publish_stop(ctx,0x8c619b8au); r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c619b8c; /* mov.l @(0x10,r15),r2 */
L8c619b8c: sa_motion_publish_stop(ctx,0x8c619b8cu); r[2]+=1u;goto L8c619b8e; /* add #0x1,r2 */
L8c619b8e: sa_motion_publish_stop(ctx,0x8c619b8eu); wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c619b90; /* mov.l r2,@(0x10,r15) */
L8c619b90: sa_motion_publish_stop(ctx,0x8c619b90u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c619b92; /* mov.l @(0x8,r15),r3 */
L8c619b92: sa_motion_publish_stop(ctx,0x8c619b92u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c619b94; /* mov.w @(0x2,r3),r0 */
L8c619b94: sa_motion_publish_stop(ctx,0x8c619b94u); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c619b96; /* mov.l @(0x10,r15),r3 */
L8c619b96: sa_motion_publish_stop(ctx,0x8c619b96u); r[0]=r[0]&65535u;goto L8c619b98; /* extu.w r0,r0 */
L8c619b98: sa_motion_publish_stop(ctx,0x8c619b98u); *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c619b9a; /* cmp/ge r0,r3 */
L8c619b9a: sa_motion_publish_stop(ctx,0x8c619b9au); if((*sr&1u))goto L8c619ba0;goto L8c619b9c; /* bt 0x8c619ba0 */
L8c619b9c: sa_motion_publish_stop(ctx,0x8c619b9eu); goto L8c6198e8; /* bra 0x8c6198e8 */
L8c619ba0: sa_motion_publish_stop(ctx,0x8c619ba2u); goto L8c619be2; /* bra 0x8c619be2 */
L8c619ba4: sa_motion_publish_stop(ctx,0x8c619ba4u); r[1]=rd(m,bus,&io_status,0x8c619c2cu,4);if(io_status)return io_status;goto L8c619ba6; /* mov.l 0x8c619c2c,r1 */
L8c619ba6: sa_motion_publish_stop(ctx,0x8c619ba6u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c619ba8; /* mov.l @(0x8,r15),r2 */
L8c619ba8: sa_motion_publish_stop(ctx,0x8c619ba8u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c619baa; /* mov.l r2,@r1 */
L8c619baa: sa_motion_publish_stop(ctx,0x8c619baau); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c619bac; /* frchg */
L8c619bac: sa_motion_publish_stop(ctx,0x8c619bacu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c619bae; /* fmov.s fr0,@-r15 */
L8c619bae: sa_motion_publish_stop(ctx,0x8c619baeu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c619bb0; /* fmov.s fr1,@-r15 */
L8c619bb0: sa_motion_publish_stop(ctx,0x8c619bb0u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c619bb2; /* fmov.s fr2,@-r15 */
L8c619bb2: sa_motion_publish_stop(ctx,0x8c619bb2u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c619bb4; /* fmov.s fr3,@-r15 */
L8c619bb4: sa_motion_publish_stop(ctx,0x8c619bb4u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c619bb6; /* fmov.s fr4,@-r15 */
L8c619bb6: sa_motion_publish_stop(ctx,0x8c619bb6u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c619bb8; /* fmov.s fr5,@-r15 */
L8c619bb8: sa_motion_publish_stop(ctx,0x8c619bb8u); r[0]=rd(m,bus,&io_status,0x8c619bf8u,4);if(io_status)return io_status;goto L8c619bba; /* mov.l 0x8c619bf8,r0 */
L8c619bba: sa_motion_publish_stop(ctx,0x8c619bbau); fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619bbc; /* fmov.s @r0+,fr0 */
L8c619bbc: sa_motion_publish_stop(ctx,0x8c619bbcu); fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619bbe; /* fmov.s @r0+,fr1 */
L8c619bbe: sa_motion_publish_stop(ctx,0x8c619bbeu); fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c619bc0; /* fmov.s @r0,fr2 */
L8c619bc0: sa_motion_publish_stop(ctx,0x8c619bc0u); r[0]=rd(m,bus,&io_status,0x8c619bfcu,4);if(io_status)return io_status;goto L8c619bc2; /* mov.l 0x8c619bfc,r0 */
L8c619bc2: sa_motion_publish_stop(ctx,0x8c619bc2u); fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c619bc4; /* fmov.s @r0,fr3 */
L8c619bc4: sa_motion_publish_stop(ctx,0x8c619bc4u); r[0]=0x8c619c00u;goto L8c619bc6; /* mova 0x8c619c00,r0 */
L8c619bc6: sa_motion_publish_stop(ctx,0x8c619bc6u); fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c619bc8; /* fmov.s @r0,fr4 */
L8c619bc8: sa_motion_publish_stop(ctx,0x8c619bc8u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c619bca; /* frchg */
L8c619bca: sa_motion_publish_stop(ctx,0x8c619bcau); r[2]=rd(m,bus,&io_status,0x8c619c30u,4);if(io_status)return io_status;goto L8c619bcc; /* mov.l 0x8c619c30,r2 */
L8c619bcc: sa_motion_publish_stop(ctx,0x8c619bccu); r[3]=rd(m,bus,&io_status,0x8c619c2cu,4);if(io_status)return io_status;goto L8c619bce; /* mov.l 0x8c619c2c,r3 */
L8c619bce: sa_motion_publish_stop(ctx,0x8c619bd0u); target=r[2];*pr=0x8c619bd2u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c619bd2; /* jsr @r2 */
L8c619bd2: sa_motion_publish_stop(ctx,0x8c619bd2u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c619bd4; /* frchg */
L8c619bd4: sa_motion_publish_stop(ctx,0x8c619bd4u); fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619bd6; /* fmov.s @r15+,fr5 */
L8c619bd6: sa_motion_publish_stop(ctx,0x8c619bd6u); fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619bd8; /* fmov.s @r15+,fr4 */
L8c619bd8: sa_motion_publish_stop(ctx,0x8c619bd8u); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619bda; /* fmov.s @r15+,fr3 */
L8c619bda: sa_motion_publish_stop(ctx,0x8c619bdau); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619bdc; /* fmov.s @r15+,fr2 */
L8c619bdc: sa_motion_publish_stop(ctx,0x8c619bdcu); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619bde; /* fmov.s @r15+,fr1 */
L8c619bde: sa_motion_publish_stop(ctx,0x8c619bdeu); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619be0; /* fmov.s @r15+,fr0 */
L8c619be0: sa_motion_publish_stop(ctx,0x8c619be0u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c619be2; /* frchg */
L8c619be2: sa_motion_publish_stop(ctx,0x8c619be2u); r[15]+=24u;goto L8c619be4; /* add #0x18,r15 */
L8c619be4: sa_motion_publish_stop(ctx,0x8c619be4u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c619be6; /* lds.l @r15+,PR */
L8c619be6: sa_motion_publish_stop(ctx,0x8c619be6u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c619be8; /* fmov.s @r15+,fr15 */
L8c619be8: sa_motion_publish_stop(ctx,0x8c619be8u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c619bea; /* mov.l @r15+,r9 */
L8c619bea: sa_motion_publish_stop(ctx,0x8c619beau); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c619bec; /* mov.l @r15+,r10 */
L8c619bec: sa_motion_publish_stop(ctx,0x8c619becu); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c619bee; /* mov.l @r15+,r11 */
L8c619bee: sa_motion_publish_stop(ctx,0x8c619beeu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c619bf0; /* mov.l @r15+,r12 */
L8c619bf0: sa_motion_publish_stop(ctx,0x8c619bf0u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c619bf2; /* mov.l @r15+,r13 */
L8c619bf2: sa_motion_publish_stop(ctx,0x8c619bf4u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
