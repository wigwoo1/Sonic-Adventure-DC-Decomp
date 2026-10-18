#include "sa_draw_motion_stop.h"
/* Environment-map triangle near-plane clipper with portable queue interface; name inferred.
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
SA_STAGE_EXPORT int sa_environment_triangle_near_clip_8c611f24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c611f24;
L8c611f24: sa_motion_publish_stop(ctx,0x8c611f24u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c611f26; /* mov.l r14,@-r15 */
L8c611f26: sa_motion_publish_stop(ctx,0x8c611f26u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c611f28; /* mov.l r13,@-r15 */
L8c611f28: sa_motion_publish_stop(ctx,0x8c611f28u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c611f2a; /* mov.l r12,@-r15 */
L8c611f2a: sa_motion_publish_stop(ctx,0x8c611f2au); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c611f2c; /* mov.l r11,@-r15 */
L8c611f2c: sa_motion_publish_stop(ctx,0x8c611f2cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c611f2e; /* mov.l r10,@-r15 */
L8c611f2e: sa_motion_publish_stop(ctx,0x8c611f2eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c611f30; /* mov.l r9,@-r15 */
L8c611f30: sa_motion_publish_stop(ctx,0x8c611f30u); r[15]+=4294967212u;goto L8c611f32; /* add #-0x54,r15 */
L8c611f32: sa_motion_publish_stop(ctx,0x8c611f32u); r[3]=rd(m,bus,&io_status,0x8c61205cu,4);if(io_status)return io_status;goto L8c611f34; /* mov.l 0x8c61205c,r3 */
L8c611f34: sa_motion_publish_stop(ctx,0x8c611f34u); r[7]=r[15];goto L8c611f36; /* mov r15,r7 */
L8c611f36: sa_motion_publish_stop(ctx,0x8c611f36u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c611f38; /* fldi1 fr7 */
L8c611f38: sa_motion_publish_stop(ctx,0x8c611f38u); r[7]+=72u;goto L8c611f3a; /* add #0x48,r7 */
L8c611f3a: sa_motion_publish_stop(ctx,0x8c611f3au); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c611f3c; /* fmov.s @r3,fr3 */
L8c611f3c: sa_motion_publish_stop(ctx,0x8c611f3cu); r[9]=r[15];goto L8c611f3e; /* mov r15,r9 */
L8c611f3e: sa_motion_publish_stop(ctx,0x8c611f3eu); fcopy(f,5u,7u);goto L8c611f40; /* fmov fr7,fr5 */
L8c611f40: sa_motion_publish_stop(ctx,0x8c611f40u); r[2]=r[7];goto L8c611f42; /* mov r7,r2 */
L8c611f42: sa_motion_publish_stop(ctx,0x8c611f42u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c611f44; /* fdiv fr3,fr5 */
L8c611f44: sa_motion_publish_stop(ctx,0x8c611f44u); r[9]+=36u;goto L8c611f46; /* add #0x24,r9 */
L8c611f46: sa_motion_publish_stop(ctx,0x8c611f46u); r[0]=0x8c612064u;goto L8c611f48; /* mova 0x8c612064,r0 */
L8c611f48: sa_motion_publish_stop(ctx,0x8c611f48u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c611f4a; /* mov.l r2,@(0x4,r15) */
L8c611f4a: sa_motion_publish_stop(ctx,0x8c611f4au); wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c611f4c; /* mov.l r4,@r2 */
L8c611f4c: sa_motion_publish_stop(ctx,0x8c611f4cu); r[11]=r[15];goto L8c611f4e; /* mov r15,r11 */
L8c611f4e: sa_motion_publish_stop(ctx,0x8c611f4eu); wr(m,bus,&io_status,r[7]+0x4u,r[5],4);if(io_status)return io_status;goto L8c611f50; /* mov.l r5,@(0x4,r7) */
L8c611f50: sa_motion_publish_stop(ctx,0x8c611f50u); r[14]=r[9];goto L8c611f52; /* mov r9,r14 */
L8c611f52: sa_motion_publish_stop(ctx,0x8c611f52u); r[12]=rd(m,bus,&io_status,0x8c612060u,4);if(io_status)return io_status;goto L8c611f54; /* mov.l 0x8c612060,r12 */
L8c611f54: sa_motion_publish_stop(ctx,0x8c611f54u); r[2]=0u;goto L8c611f56; /* mov #0x0,r2 */
L8c611f56: sa_motion_publish_stop(ctx,0x8c611f56u); r[5]=rd(m,bus,&io_status,0x8c612068u,4);if(io_status)return io_status;goto L8c611f58; /* mov.l 0x8c612068,r5 */
L8c611f58: sa_motion_publish_stop(ctx,0x8c611f58u); r[4]=1u;goto L8c611f5a; /* mov #0x1,r4 */
L8c611f5a: sa_motion_publish_stop(ctx,0x8c611f5au); wr(m,bus,&io_status,r[7]+0x8u,r[6],4);if(io_status)return io_status;goto L8c611f5c; /* mov.l r6,@(0x8,r7) */
L8c611f5c: sa_motion_publish_stop(ctx,0x8c611f5cu); r[11]+=24u;goto L8c611f5e; /* add #0x18,r11 */
L8c611f5e: sa_motion_publish_stop(ctx,0x8c611f5eu); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c611f60; /* mov.l r2,@r15 */
L8c611f60: sa_motion_publish_stop(ctx,0x8c611f60u); r[13]=r[14];goto L8c611f62; /* mov r14,r13 */
L8c611f62: sa_motion_publish_stop(ctx,0x8c611f62u); r[10]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611f64; /* mov.l @(0x4,r15),r10 */
L8c611f64: sa_motion_publish_stop(ctx,0x8c611f64u); wr(m,bus,&io_status,r[15]+0x8u,r[14],4);if(io_status)return io_status;goto L8c611f66; /* mov.l r14,@(0x8,r15) */
L8c611f66: sa_motion_publish_stop(ctx,0x8c611f66u); fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c611f68; /* fmov.s @r0,fr4 */
L8c611f68: sa_motion_publish_stop(ctx,0x8c611f68u); r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611f6a; /* mov.l @r15,r6 */
L8c611f6a: sa_motion_publish_stop(ctx,0x8c611f6au); r[0]=4u;goto L8c611f6c; /* mov #0x4,r0 */
L8c611f6c: sa_motion_publish_stop(ctx,0x8c611f6cu); r[3]=rd(m,bus,&io_status,0x8c61206cu,4);if(io_status)return io_status;goto L8c611f6e; /* mov.l 0x8c61206c,r3 */
L8c611f6e: sa_motion_publish_stop(ctx,0x8c611f6eu); r[6]<<=2;goto L8c611f70; /* shll2 r6 */
L8c611f70: sa_motion_publish_stop(ctx,0x8c611f70u); r[2]=rd(m,bus,&io_status,0x8c612070u,4);if(io_status)return io_status;goto L8c611f72; /* mov.l 0x8c612070,r2 */
L8c611f72: sa_motion_publish_stop(ctx,0x8c611f72u); wr(m,bus,&io_status,r[15]+0x10u,r[6],4);if(io_status)return io_status;goto L8c611f74; /* mov.l r6,@(0x10,r15) */
L8c611f74: sa_motion_publish_stop(ctx,0x8c611f74u); r[6]+=r[7];goto L8c611f76; /* add r7,r6 */
L8c611f76: sa_motion_publish_stop(ctx,0x8c611f76u); wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c611f78; /* mov.l r6,@(0x4,r15) */
L8c611f78: sa_motion_publish_stop(ctx,0x8c611f78u); r[6]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c611f7a; /* mov.l @r6,r6 */
L8c611f7a: sa_motion_publish_stop(ctx,0x8c611f7au); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c611f7c; /* fmov.s @r3,fr2 */
L8c611f7c: sa_motion_publish_stop(ctx,0x8c611f7cu); r[3]+=8u;goto L8c611f7e; /* add #0x8,r3 */
L8c611f7e: sa_motion_publish_stop(ctx,0x8c611f7eu); fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c611f80; /* fmov.s @r6,fr3 */
L8c611f80: sa_motion_publish_stop(ctx,0x8c611f80u); r[1]=rd(m,bus,&io_status,0x8c612074u,4);if(io_status)return io_status;goto L8c611f82; /* mov.l 0x8c612074,r1 */
L8c611f82: sa_motion_publish_stop(ctx,0x8c611f82u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611f84; /* fmul fr3,fr2 */
L8c611f84: sa_motion_publish_stop(ctx,0x8c611f84u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c611f86; /* fmov.s @r2,fr3 */
L8c611f86: sa_motion_publish_stop(ctx,0x8c611f86u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c611f88; /* fmov.s @r1,fr1 */
L8c611f88: sa_motion_publish_stop(ctx,0x8c611f88u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611f8a; /* fsub fr2,fr3 */
L8c611f8a: sa_motion_publish_stop(ctx,0x8c611f8au); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c611f8c; /* fmov.s @(r0,r6),fr2 */
L8c611f8c: sa_motion_publish_stop(ctx,0x8c611f8cu); r[0]=8u;goto L8c611f8e; /* mov #0x8,r0 */
L8c611f8e: sa_motion_publish_stop(ctx,0x8c611f8eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c611f90; /* fmul fr2,fr1 */
L8c611f90: sa_motion_publish_stop(ctx,0x8c611f90u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c611f92; /* fmov.s @(r0,r6),fr2 */
L8c611f92: sa_motion_publish_stop(ctx,0x8c611f92u); r[0]=28u;goto L8c611f94; /* mov #0x1c,r0 */
L8c611f94: sa_motion_publish_stop(ctx,0x8c611f94u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611f96; /* fsub fr1,fr3 */
L8c611f96: sa_motion_publish_stop(ctx,0x8c611f96u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c611f98; /* fmov.s @r3,fr1 */
L8c611f98: sa_motion_publish_stop(ctx,0x8c611f98u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c611f9a; /* fmul fr2,fr1 */
L8c611f9a: sa_motion_publish_stop(ctx,0x8c611f9au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611f9c; /* fsub fr1,fr3 */
L8c611f9c: sa_motion_publish_stop(ctx,0x8c611f9cu); fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c611f9e; /* fmov fr3,@(r0,r6) */
L8c611f9e: sa_motion_publish_stop(ctx,0x8c611f9eu); r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c611fa0; /* mov.l @(0x10,r15),r2 */
L8c611fa0: sa_motion_publish_stop(ctx,0x8c611fa0u); r[6]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611fa2; /* mov.l @(0x4,r15),r6 */
L8c611fa2: sa_motion_publish_stop(ctx,0x8c611fa2u); r[2]+=r[11];goto L8c611fa4; /* add r11,r2 */
L8c611fa4: sa_motion_publish_stop(ctx,0x8c611fa4u); r[6]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c611fa6; /* mov.l @r6,r6 */
L8c611fa6: sa_motion_publish_stop(ctx,0x8c611fa6u); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c611fa8; /* mov.l r2,@(0xc,r15) */
L8c611fa8: sa_motion_publish_stop(ctx,0x8c611fa8u); r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c611faa; /* mov.l @r5,r3 */
L8c611faa: sa_motion_publish_stop(ctx,0x8c611faau); r[0]=r[3];goto L8c611fac; /* mov r3,r0 */
L8c611fac: sa_motion_publish_stop(ctx,0x8c611facu); fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c611fae; /* fmov.s @r0,fr2 */
L8c611fae: sa_motion_publish_stop(ctx,0x8c611faeu); r[0]=4u;goto L8c611fb0; /* mov #0x4,r0 */
L8c611fb0: sa_motion_publish_stop(ctx,0x8c611fb0u); r[1]=r[3];goto L8c611fb2; /* mov r3,r1 */
L8c611fb2: sa_motion_publish_stop(ctx,0x8c611fb2u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c611fb4; /* fmov.s @(r0,r6),fr0 */
L8c611fb4: sa_motion_publish_stop(ctx,0x8c611fb4u); r[0]=16u;goto L8c611fb6; /* mov #0x10,r0 */
L8c611fb6: sa_motion_publish_stop(ctx,0x8c611fb6u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c611fb8; /* fmov.s @(r0,r1),fr3 */
L8c611fb8: sa_motion_publish_stop(ctx,0x8c611fb8u); r[0]=8u;goto L8c611fba; /* mov #0x8,r0 */
L8c611fba: sa_motion_publish_stop(ctx,0x8c611fbau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611fbc; /* fmul fr0,fr3 */
L8c611fbc: sa_motion_publish_stop(ctx,0x8c611fbcu); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c611fbe; /* fmov.s @r6,fr0 */
L8c611fbe: sa_motion_publish_stop(ctx,0x8c611fbeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611fc0; /* fmac fr0,fr2,fr3 */
L8c611fc0: sa_motion_publish_stop(ctx,0x8c611fc0u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c611fc2; /* fmov.s @(r0,r6),fr0 */
L8c611fc2: sa_motion_publish_stop(ctx,0x8c611fc2u); r[0]=32u;goto L8c611fc4; /* mov #0x20,r0 */
L8c611fc4: sa_motion_publish_stop(ctx,0x8c611fc4u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c611fc6; /* fmov.s @(r0,r3),fr2 */
L8c611fc6: sa_motion_publish_stop(ctx,0x8c611fc6u); r[0]=36u;goto L8c611fc8; /* mov #0x24,r0 */
L8c611fc8: sa_motion_publish_stop(ctx,0x8c611fc8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611fca; /* fmac fr0,fr2,fr3 */
L8c611fca: sa_motion_publish_stop(ctx,0x8c611fcau); fcopy(f,0u,4u);goto L8c611fcc; /* fmov fr4,fr0 */
L8c611fcc: sa_motion_publish_stop(ctx,0x8c611fccu); fcopy(f,2u,4u);goto L8c611fce; /* fmov fr4,fr2 */
L8c611fce: sa_motion_publish_stop(ctx,0x8c611fceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611fd0; /* fmac fr0,fr3,fr2 */
L8c611fd0: sa_motion_publish_stop(ctx,0x8c611fd0u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c611fd2; /* ftrc fr2,FPUL */
L8c611fd2: sa_motion_publish_stop(ctx,0x8c611fd2u); r[3]=*fpul;goto L8c611fd4; /* sts FPUL,r3 */
L8c611fd4: sa_motion_publish_stop(ctx,0x8c611fd4u); wr(m,bus,&io_status,r[2],r[3],2);if(io_status)return io_status;goto L8c611fd6; /* mov.w r3,@r2 */
L8c611fd6: sa_motion_publish_stop(ctx,0x8c611fd6u); r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c611fd8; /* mov.l @r5,r3 */
L8c611fd8: sa_motion_publish_stop(ctx,0x8c611fd8u); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c611fda; /* mov.l @(0xc,r15),r2 */
L8c611fda: sa_motion_publish_stop(ctx,0x8c611fdau); fload(m,bus,&io_status,f,1u,r[0]+r[3]);if(io_status)return io_status;goto L8c611fdc; /* fmov.s @(r0,r3),fr1 */
L8c611fdc: sa_motion_publish_stop(ctx,0x8c611fdcu); r[0]=4u;goto L8c611fde; /* mov #0x4,r0 */
L8c611fde: sa_motion_publish_stop(ctx,0x8c611fdeu); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c611fe0; /* fmov.s @(r0,r6),fr0 */
L8c611fe0: sa_motion_publish_stop(ctx,0x8c611fe0u); r[0]=20u;goto L8c611fe2; /* mov #0x14,r0 */
L8c611fe2: sa_motion_publish_stop(ctx,0x8c611fe2u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c611fe4; /* fmov.s @(r0,r3),fr3 */
L8c611fe4: sa_motion_publish_stop(ctx,0x8c611fe4u); r[1]=r[3];goto L8c611fe6; /* mov r3,r1 */
L8c611fe6: sa_motion_publish_stop(ctx,0x8c611fe6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611fe8; /* fmul fr0,fr3 */
L8c611fe8: sa_motion_publish_stop(ctx,0x8c611fe8u); r[0]=4u;goto L8c611fea; /* mov #0x4,r0 */
L8c611fea: sa_motion_publish_stop(ctx,0x8c611feau); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c611fec; /* fmov.s @r6,fr0 */
L8c611fec: sa_motion_publish_stop(ctx,0x8c611fecu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c611fee; /* fmov.s @(r0,r1),fr2 */
L8c611fee: sa_motion_publish_stop(ctx,0x8c611feeu); r[0]=8u;goto L8c611ff0; /* mov #0x8,r0 */
L8c611ff0: sa_motion_publish_stop(ctx,0x8c611ff0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611ff2; /* fmac fr0,fr2,fr3 */
L8c611ff2: sa_motion_publish_stop(ctx,0x8c611ff2u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c611ff4; /* fmov.s @(r0,r6),fr0 */
L8c611ff4: sa_motion_publish_stop(ctx,0x8c611ff4u); fcopy(f,2u,4u);goto L8c611ff6; /* fmov fr4,fr2 */
L8c611ff6: sa_motion_publish_stop(ctx,0x8c611ff6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611ff8; /* fmac fr0,fr1,fr3 */
L8c611ff8: sa_motion_publish_stop(ctx,0x8c611ff8u); fcopy(f,0u,4u);goto L8c611ffa; /* fmov fr4,fr0 */
L8c611ffa: sa_motion_publish_stop(ctx,0x8c611ffau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611ffc; /* fmac fr0,fr3,fr2 */
L8c611ffc: sa_motion_publish_stop(ctx,0x8c611ffcu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c611ffe; /* ftrc fr2,FPUL */
L8c611ffe: sa_motion_publish_stop(ctx,0x8c611ffeu); r[0]=*fpul;goto L8c612000; /* sts FPUL,r0 */
L8c612000: sa_motion_publish_stop(ctx,0x8c612000u); wr(m,bus,&io_status,r[2]+0x2u,r[0],2);if(io_status)return io_status;goto L8c612002; /* mov.w r0,@(0x2,r2) */
L8c612002: sa_motion_publish_stop(ctx,0x8c612002u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612004; /* mov.l @(0x4,r15),r3 */
L8c612004: sa_motion_publish_stop(ctx,0x8c612004u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c612006; /* mov.l @r3,r2 */
L8c612006: sa_motion_publish_stop(ctx,0x8c612006u); r[1]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612008; /* mov.l @(0xc,r2),r1 */
L8c612008: sa_motion_publish_stop(ctx,0x8c612008u); *sr=(*sr&~1u)|!(r[4]&r[1]);goto L8c61200a; /* tst r4,r1 */
L8c61200a: sa_motion_publish_stop(ctx,0x8c61200au); if(!(*sr&1u))goto L8c612080;goto L8c61200c; /* bf 0x8c612080 */
L8c61200c: sa_motion_publish_stop(ctx,0x8c61200cu); r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61200e; /* mov.l @r15,r6 */
L8c61200e: sa_motion_publish_stop(ctx,0x8c61200eu); r[0]=24u;goto L8c612010; /* mov #0x18,r0 */
L8c612010: sa_motion_publish_stop(ctx,0x8c612010u); fcopy(f,6u,7u);goto L8c612012; /* fmov fr7,fr6 */
L8c612012: sa_motion_publish_stop(ctx,0x8c612012u); r[6]<<=2;goto L8c612014; /* shll2 r6 */
L8c612014: sa_motion_publish_stop(ctx,0x8c612014u); r[3]=rd(m,bus,&io_status,0x8c612078u,4);if(io_status)return io_status;goto L8c612016; /* mov.l 0x8c612078,r3 */
L8c612016: sa_motion_publish_stop(ctx,0x8c612016u); r[6]+=r[7];goto L8c612018; /* add r7,r6 */
L8c612018: sa_motion_publish_stop(ctx,0x8c612018u); wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c61201a; /* mov.l r6,@(0x4,r15) */
L8c61201a: sa_motion_publish_stop(ctx,0x8c61201au); r[6]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c61201c; /* mov.l @r6,r6 */
L8c61201c: sa_motion_publish_stop(ctx,0x8c61201cu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61201e; /* fmov.s @(r0,r6),fr3 */
L8c61201e: sa_motion_publish_stop(ctx,0x8c61201eu); r[0]=12u;goto L8c612020; /* mov #0xc,r0 */
L8c612020: sa_motion_publish_stop(ctx,0x8c612020u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[3],&f->fpscr,&fr[6]);if(status)return status;}goto L8c612022; /* fdiv fr3,fr6 */
L8c612022: sa_motion_publish_stop(ctx,0x8c612022u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c612024; /* fmov.s @(r0,r12),fr3 */
L8c612024: sa_motion_publish_stop(ctx,0x8c612024u); r[0]=16u;goto L8c612026; /* mov #0x10,r0 */
L8c612026: sa_motion_publish_stop(ctx,0x8c612026u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c612028; /* fmov.s @(r0,r6),fr2 */
L8c612028: sa_motion_publish_stop(ctx,0x8c612028u); r[0]=16u;goto L8c61202a; /* mov #0x10,r0 */
L8c61202a: sa_motion_publish_stop(ctx,0x8c61202au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61202c; /* fsub fr3,fr2 */
L8c61202c: sa_motion_publish_stop(ctx,0x8c61202cu); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61202e; /* fmov.s @r3,fr3 */
L8c61202e: sa_motion_publish_stop(ctx,0x8c61202eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612030; /* fmul fr6,fr2 */
L8c612030: sa_motion_publish_stop(ctx,0x8c612030u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612032; /* fdiv fr3,fr2 */
L8c612032: sa_motion_publish_stop(ctx,0x8c612032u); fstore(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c612034; /* fmov.s fr2,@r13 */
L8c612034: sa_motion_publish_stop(ctx,0x8c612034u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612036; /* mov.l @(0x4,r15),r2 */
L8c612036: sa_motion_publish_stop(ctx,0x8c612036u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c612038; /* fmov.s @(r0,r12),fr3 */
L8c612038: sa_motion_publish_stop(ctx,0x8c612038u); r[0]=20u;goto L8c61203a; /* mov #0x14,r0 */
L8c61203a: sa_motion_publish_stop(ctx,0x8c61203au); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61203c; /* mov.l @r2,r1 */
L8c61203c: sa_motion_publish_stop(ctx,0x8c61203cu); r[2]=rd(m,bus,&io_status,0x8c61207cu,4);if(io_status)return io_status;goto L8c61203e; /* mov.l 0x8c61207c,r2 */
L8c61203e: sa_motion_publish_stop(ctx,0x8c61203eu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c612040; /* fmov.s @(r0,r1),fr2 */
L8c612040: sa_motion_publish_stop(ctx,0x8c612040u); r[0]=4u;goto L8c612042; /* mov #0x4,r0 */
L8c612042: sa_motion_publish_stop(ctx,0x8c612042u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612044; /* fsub fr3,fr2 */
L8c612044: sa_motion_publish_stop(ctx,0x8c612044u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c612046; /* fmov.s @r2,fr3 */
L8c612046: sa_motion_publish_stop(ctx,0x8c612046u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612048; /* fmul fr6,fr2 */
L8c612048: sa_motion_publish_stop(ctx,0x8c612048u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61204a; /* fdiv fr3,fr2 */
L8c61204a: sa_motion_publish_stop(ctx,0x8c61204au); fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61204c; /* fmov fr2,@(r0,r13) */
L8c61204c: sa_motion_publish_stop(ctx,0x8c61204cu); r[0]=8u;goto L8c61204e; /* mov #0x8,r0 */
L8c61204e: sa_motion_publish_stop(ctx,0x8c612050u); fstore(m,bus,&io_status,f,6u,r[0]+r[13]);if(io_status)return io_status;goto L8c61209c; /* bra 0x8c61209c */
L8c612080: sa_motion_publish_stop(ctx,0x8c612080u); r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c612082; /* mov.l @r10,r3 */
L8c612082: sa_motion_publish_stop(ctx,0x8c612082u); r[0]=16u;goto L8c612084; /* mov #0x10,r0 */
L8c612084: sa_motion_publish_stop(ctx,0x8c612084u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612086; /* fmov.s @(r0,r3),fr3 */
L8c612086: sa_motion_publish_stop(ctx,0x8c612086u); r[0]=20u;goto L8c612088; /* mov #0x14,r0 */
L8c612088: sa_motion_publish_stop(ctx,0x8c612088u); fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61208a; /* fmov.s fr3,@r14 */
L8c61208a: sa_motion_publish_stop(ctx,0x8c61208au); r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c61208c; /* mov.l @r10,r3 */
L8c61208c: sa_motion_publish_stop(ctx,0x8c61208cu); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61208e; /* fmov.s @(r0,r3),fr3 */
L8c61208e: sa_motion_publish_stop(ctx,0x8c61208eu); r[0]=4u;goto L8c612090; /* mov #0x4,r0 */
L8c612090: sa_motion_publish_stop(ctx,0x8c612090u); fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c612092; /* fmov fr3,@(r0,r14) */
L8c612092: sa_motion_publish_stop(ctx,0x8c612092u); r[0]=24u;goto L8c612094; /* mov #0x18,r0 */
L8c612094: sa_motion_publish_stop(ctx,0x8c612094u); r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c612096; /* mov.l @r10,r3 */
L8c612096: sa_motion_publish_stop(ctx,0x8c612096u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612098; /* fmov.s @(r0,r3),fr3 */
L8c612098: sa_motion_publish_stop(ctx,0x8c612098u); r[0]=8u;goto L8c61209a; /* mov #0x8,r0 */
L8c61209a: sa_motion_publish_stop(ctx,0x8c61209au); fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61209c; /* fmov fr3,@(r0,r14) */
L8c61209c: sa_motion_publish_stop(ctx,0x8c61209cu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61209e; /* mov.l @r15,r3 */
L8c61209e: sa_motion_publish_stop(ctx,0x8c61209eu); r[2]=3u;goto L8c6120a0; /* mov #0x3,r2 */
L8c6120a0: sa_motion_publish_stop(ctx,0x8c6120a0u); r[14]+=12u;goto L8c6120a2; /* add #0xc,r14 */
L8c6120a2: sa_motion_publish_stop(ctx,0x8c6120a2u); r[3]+=1u;goto L8c6120a4; /* add #0x1,r3 */
L8c6120a4: sa_motion_publish_stop(ctx,0x8c6120a4u); *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c6120a6; /* cmp/ge r2,r3 */
L8c6120a6: sa_motion_publish_stop(ctx,0x8c6120a6u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6120a8; /* mov.l r3,@r15 */
L8c6120a8: sa_motion_publish_stop(ctx,0x8c6120a8u); r[10]+=4u;goto L8c6120aa; /* add #0x4,r10 */
L8c6120aa: sa_motion_publish_stop(ctx,0x8c6120acu); take=(*sr&1u);r[13]+=12u;if(take)goto L8c6120b2;goto L8c6120ae; /* bt/s 0x8c6120b2 */
L8c6120ae: sa_motion_publish_stop(ctx,0x8c6120b0u); goto L8c611f68; /* bra 0x8c611f68 */
L8c6120b2: sa_motion_publish_stop(ctx,0x8c6120b2u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6120b4; /* mov.l @(0x8,r15),r3 */
L8c6120b4: sa_motion_publish_stop(ctx,0x8c6120b4u); r[0]=8u;goto L8c6120b6; /* mov #0x8,r0 */
L8c6120b6: sa_motion_publish_stop(ctx,0x8c6120b6u); r[6]=r[9];goto L8c6120b8; /* mov r9,r6 */
L8c6120b8: sa_motion_publish_stop(ctx,0x8c6120b8u); r[6]+=12u;goto L8c6120ba; /* add #0xc,r6 */
L8c6120ba: sa_motion_publish_stop(ctx,0x8c6120bau); fload(m,bus,&io_status,f,7u,r[0]+r[3]);if(io_status)return io_status;goto L8c6120bc; /* fmov.s @(r0,r3),fr7 */
L8c6120bc: sa_motion_publish_stop(ctx,0x8c6120bcu); r[0]=4u;goto L8c6120be; /* mov #0x4,r0 */
L8c6120be: sa_motion_publish_stop(ctx,0x8c6120beu); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c6120c0; /* fmov.s @(r0,r6),fr4 */
L8c6120c0: sa_motion_publish_stop(ctx,0x8c6120c0u); r[5]=r[9];goto L8c6120c2; /* mov r9,r5 */
L8c6120c2: sa_motion_publish_stop(ctx,0x8c6120c2u); fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c6120c4; /* fmov.s @(r0,r3),fr9 */
L8c6120c4: sa_motion_publish_stop(ctx,0x8c6120c4u); r[0]=8u;goto L8c6120c6; /* mov #0x8,r0 */
L8c6120c6: sa_motion_publish_stop(ctx,0x8c6120c6u); fload(m,bus,&io_status,f,6u,r[0]+r[6]);if(io_status)return io_status;goto L8c6120c8; /* fmov.s @(r0,r6),fr6 */
L8c6120c8: sa_motion_publish_stop(ctx,0x8c6120c8u); r[0]=4u;goto L8c6120ca; /* mov #0x4,r0 */
L8c6120ca: sa_motion_publish_stop(ctx,0x8c6120cau); fcopy(f,3u,9u);goto L8c6120cc; /* fmov fr9,fr3 */
L8c6120cc: sa_motion_publish_stop(ctx,0x8c6120ccu); r[5]+=24u;goto L8c6120ce; /* add #0x18,r5 */
L8c6120ce: sa_motion_publish_stop(ctx,0x8c6120ceu); fcopy(f,0u,7u);goto L8c6120d0; /* fmov fr7,fr0 */
L8c6120d0: sa_motion_publish_stop(ctx,0x8c6120d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6120d2; /* fmul fr6,fr3 */
L8c6120d2: sa_motion_publish_stop(ctx,0x8c6120d2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6120d4; /* fmul fr4,fr0 */
L8c6120d4: sa_motion_publish_stop(ctx,0x8c6120d4u); fload(m,bus,&io_status,f,10u,r[3]);if(io_status)return io_status;goto L8c6120d6; /* fmov.s @r3,fr10 */
L8c6120d6: sa_motion_publish_stop(ctx,0x8c6120d6u); fload(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;goto L8c6120d8; /* fmov.s @r6,fr8 */
L8c6120d8: sa_motion_publish_stop(ctx,0x8c6120d8u); fcopy(f,2u,7u);goto L8c6120da; /* fmov fr7,fr2 */
L8c6120da: sa_motion_publish_stop(ctx,0x8c6120dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6120dc; /* fmul fr8,fr2 */
L8c6120dc: sa_motion_publish_stop(ctx,0x8c6120dcu); fcopy(f,1u,8u);goto L8c6120de; /* fmov fr8,fr1 */
L8c6120de: sa_motion_publish_stop(ctx,0x8c6120deu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6120e0; /* fsub fr3,fr0 */
L8c6120e0: sa_motion_publish_stop(ctx,0x8c6120e0u); fcopy(f,3u,10u);goto L8c6120e2; /* fmov fr10,fr3 */
L8c6120e2: sa_motion_publish_stop(ctx,0x8c6120e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6120e4; /* fmul fr6,fr3 */
L8c6120e4: sa_motion_publish_stop(ctx,0x8c6120e4u); fcopy(f,11u,10u);goto L8c6120e6; /* fmov fr10,fr11 */
L8c6120e6: sa_motion_publish_stop(ctx,0x8c6120e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[9],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6120e8; /* fmul fr9,fr1 */
L8c6120e8: sa_motion_publish_stop(ctx,0x8c6120e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6120ea; /* fsub fr2,fr3 */
L8c6120ea: sa_motion_publish_stop(ctx,0x8c6120eau); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c6120ec; /* fmov.s @(r0,r5),fr2 */
L8c6120ec: sa_motion_publish_stop(ctx,0x8c6120ecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6120ee; /* fmul fr3,fr2 */
L8c6120ee: sa_motion_publish_stop(ctx,0x8c6120eeu); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c6120f0; /* fmov.s @r5,fr3 */
L8c6120f0: sa_motion_publish_stop(ctx,0x8c6120f0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6120f2; /* fmac fr0,fr3,fr2 */
L8c6120f2: sa_motion_publish_stop(ctx,0x8c6120f2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[4],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6120f4; /* fmul fr4,fr11 */
L8c6120f4: sa_motion_publish_stop(ctx,0x8c6120f4u); r[0]=8u;goto L8c6120f6; /* mov #0x8,r0 */
L8c6120f6: sa_motion_publish_stop(ctx,0x8c6120f6u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6120f8; /* fmov.s @(r0,r5),fr3 */
L8c6120f8: sa_motion_publish_stop(ctx,0x8c6120f8u); r[5]=rd(m,bus,&io_status,0x8c612388u,4);if(io_status)return io_status;goto L8c6120fa; /* mov.l 0x8c612388,r5 */
L8c6120fa: sa_motion_publish_stop(ctx,0x8c6120fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[11],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6120fc; /* fsub fr11,fr1 */
L8c6120fc: sa_motion_publish_stop(ctx,0x8c6120fcu); r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c6120fe; /* mov.l @r5,r3 */
L8c6120fe: sa_motion_publish_stop(ctx,0x8c6120feu); fcopy(f,0u,1u);goto L8c612100; /* fmov fr1,fr0 */
L8c612100: sa_motion_publish_stop(ctx,0x8c612100u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612102; /* fmac fr0,fr3,fr2 */
L8c612102: sa_motion_publish_stop(ctx,0x8c612102u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c612104; /* fldi0 fr3 */
L8c612104: sa_motion_publish_stop(ctx,0x8c612104u); fcopy(f,4u,2u);goto L8c612106; /* fmov fr2,fr4 */
L8c612106: sa_motion_publish_stop(ctx,0x8c612106u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c612108; /* fcmp/gt fr4,fr3 */
L8c612108: sa_motion_publish_stop(ctx,0x8c612108u); r[0]=*sr&1u;goto L8c61210a; /* movt r0 */
L8c61210a: sa_motion_publish_stop(ctx,0x8c61210au); *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c61210c; /* tst r3,r0 */
L8c61210c: sa_motion_publish_stop(ctx,0x8c61210cu); if((*sr&1u))goto L8c612112;goto L8c61210e; /* bt 0x8c612112 */
L8c61210e: sa_motion_publish_stop(ctx,0x8c612110u); goto L8c6124b0; /* bra 0x8c6124b0 */
L8c612112: sa_motion_publish_stop(ctx,0x8c612112u); r[1]=0u;goto L8c612114; /* mov #0x0,r1 */
L8c612114: sa_motion_publish_stop(ctx,0x8c612114u); r[13]=rd(m,bus,&io_status,0x8c61238cu,4);if(io_status)return io_status;goto L8c612116; /* mov.l 0x8c61238c,r13 */
L8c612116: sa_motion_publish_stop(ctx,0x8c612116u); wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c612118; /* mov.l r1,@r5 */
L8c612118: sa_motion_publish_stop(ctx,0x8c612118u); r[3]=2u;goto L8c61211a; /* mov #0x2,r3 */
L8c61211a: sa_motion_publish_stop(ctx,0x8c61211au); r[5]=rd(m,bus,&io_status,0x8c612390u,4);if(io_status)return io_status;goto L8c61211c; /* mov.l 0x8c612390,r5 */
L8c61211c: sa_motion_publish_stop(ctx,0x8c61211cu); r[6]=r[1];goto L8c61211e; /* mov r1,r6 */
L8c61211e: sa_motion_publish_stop(ctx,0x8c61211eu); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c612120; /* mov.l r3,@(0x4,r15) */
L8c612120: sa_motion_publish_stop(ctx,0x8c612120u); r[10]=r[6];goto L8c612122; /* mov r6,r10 */
L8c612122: sa_motion_publish_stop(ctx,0x8c612122u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612124; /* mov.l @(0x4,r15),r3 */
L8c612124: sa_motion_publish_stop(ctx,0x8c612124u); r[0]=r[7];goto L8c612126; /* mov r7,r0 */
L8c612126: sa_motion_publish_stop(ctx,0x8c612126u); r[14]=r[10];goto L8c612128; /* mov r10,r14 */
L8c612128: sa_motion_publish_stop(ctx,0x8c612128u); r[14]<<=2;goto L8c61212a; /* shll2 r14 */
L8c61212a: sa_motion_publish_stop(ctx,0x8c61212au); r[3]<<=2;goto L8c61212c; /* shll2 r3 */
L8c61212c: sa_motion_publish_stop(ctx,0x8c61212cu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61212e; /* mov.l r3,@r15 */
L8c61212e: sa_motion_publish_stop(ctx,0x8c61212eu); r[2]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c612130; /* mov.l @(r0,r3),r2 */
L8c612130: sa_motion_publish_stop(ctx,0x8c612130u); r[3]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612132; /* mov.l @(0xc,r2),r3 */
L8c612132: sa_motion_publish_stop(ctx,0x8c612132u); r[2]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c612134; /* mov.l @(r0,r14),r2 */
L8c612134: sa_motion_publish_stop(ctx,0x8c612134u); r[3]&=r[4];goto L8c612136; /* and r4,r3 */
L8c612136: sa_motion_publish_stop(ctx,0x8c612136u); if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[2]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612138; /* mov.l @(0xc,r2),r2 */
L8c612138: sa_motion_publish_stop(ctx,0x8c612138u); r[2]&=r[4];goto L8c61213a; /* and r4,r2 */
L8c61213a: sa_motion_publish_stop(ctx,0x8c61213au); r[3]^=r[2];goto L8c61213c; /* xor r2,r3 */
L8c61213c: sa_motion_publish_stop(ctx,0x8c61213cu); r[0]=r[3];goto L8c61213e; /* mov r3,r0 */
L8c61213e: sa_motion_publish_stop(ctx,0x8c61213eu); *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c612140; /* cmp/eq #0x1,r0 */
L8c612140: sa_motion_publish_stop(ctx,0x8c612140u); if((*sr&1u))goto L8c612146;goto L8c612142; /* bt 0x8c612146 */
L8c612142: sa_motion_publish_stop(ctx,0x8c612144u); goto L8c612248; /* bra 0x8c612248 */
L8c612146: sa_motion_publish_stop(ctx,0x8c612146u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612148; /* mov.l @(0x4,r15),r2 */
L8c612148: sa_motion_publish_stop(ctx,0x8c612148u); r[0]=8u;goto L8c61214a; /* mov #0x8,r0 */
L8c61214a: sa_motion_publish_stop(ctx,0x8c61214au); r[1]=r[10];goto L8c61214c; /* mov r10,r1 */
L8c61214c: sa_motion_publish_stop(ctx,0x8c61214cu); r[3]=r[2];goto L8c61214e; /* mov r2,r3 */
L8c61214e: sa_motion_publish_stop(ctx,0x8c61214eu); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612150; /* shll r2 */
L8c612150: sa_motion_publish_stop(ctx,0x8c612150u); r[2]+=r[3];goto L8c612152; /* add r3,r2 */
L8c612152: sa_motion_publish_stop(ctx,0x8c612152u); r[3]=rd(m,bus,&io_status,0x8c612394u,4);if(io_status)return io_status;goto L8c612154; /* mov.l 0x8c612394,r3 */
L8c612154: sa_motion_publish_stop(ctx,0x8c612154u); r[2]<<=2;goto L8c612156; /* shll2 r2 */
L8c612156: sa_motion_publish_stop(ctx,0x8c612156u); r[2]+=r[9];goto L8c612158; /* add r9,r2 */
L8c612158: sa_motion_publish_stop(ctx,0x8c612158u); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c61215a; /* mov.l r2,@(0xc,r15) */
L8c61215a: sa_motion_publish_stop(ctx,0x8c61215au); fload(m,bus,&io_status,f,4u,r[0]+r[2]);if(io_status)return io_status;goto L8c61215c; /* fmov.s @(r0,r2),fr4 */
L8c61215c: sa_motion_publish_stop(ctx,0x8c61215cu); r[2]=r[10];goto L8c61215e; /* mov r10,r2 */
L8c61215e: sa_motion_publish_stop(ctx,0x8c61215eu); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612160; /* shll r2 */
L8c612160: sa_motion_publish_stop(ctx,0x8c612160u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c612162; /* fmov.s @r3,fr3 */
L8c612162: sa_motion_publish_stop(ctx,0x8c612162u); r[2]+=r[1];goto L8c612164; /* add r1,r2 */
L8c612164: sa_motion_publish_stop(ctx,0x8c612164u); r[2]<<=2;goto L8c612166; /* shll2 r2 */
L8c612166: sa_motion_publish_stop(ctx,0x8c612166u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612168; /* fsub fr4,fr3 */
L8c612168: sa_motion_publish_stop(ctx,0x8c612168u); r[0]=8u;goto L8c61216a; /* mov #0x8,r0 */
L8c61216a: sa_motion_publish_stop(ctx,0x8c61216au); r[2]+=r[9];goto L8c61216c; /* add r9,r2 */
L8c61216c: sa_motion_publish_stop(ctx,0x8c61216cu); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c61216e; /* mov.l r2,@(0x8,r15) */
L8c61216e: sa_motion_publish_stop(ctx,0x8c61216eu); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c612170; /* fmov.s @(r0,r2),fr2 */
L8c612170: sa_motion_publish_stop(ctx,0x8c612170u); r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612172; /* mov.l @(0x8,r15),r1 */
L8c612172: sa_motion_publish_stop(ctx,0x8c612172u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612174; /* fsub fr4,fr2 */
L8c612174: sa_motion_publish_stop(ctx,0x8c612174u); fcopy(f,4u,3u);goto L8c612176; /* fmov fr3,fr4 */
L8c612176: sa_motion_publish_stop(ctx,0x8c612176u); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c612178; /* mov.l @(0xc,r15),r2 */
L8c612178: sa_motion_publish_stop(ctx,0x8c612178u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61217a; /* fmov.s @r1,fr3 */
L8c61217a: sa_motion_publish_stop(ctx,0x8c61217au); fload(m,bus,&io_status,f,6u,r[2]);if(io_status)return io_status;goto L8c61217c; /* fmov.s @r2,fr6 */
L8c61217c: sa_motion_publish_stop(ctx,0x8c61217cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61217e; /* fdiv fr2,fr4 */
L8c61217e: sa_motion_publish_stop(ctx,0x8c61217eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612180; /* fsub fr6,fr3 */
L8c612180: sa_motion_publish_stop(ctx,0x8c612180u); fcopy(f,2u,6u);goto L8c612182; /* fmov fr6,fr2 */
L8c612182: sa_motion_publish_stop(ctx,0x8c612182u); fcopy(f,0u,4u);goto L8c612184; /* fmov fr4,fr0 */
L8c612184: sa_motion_publish_stop(ctx,0x8c612184u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612186; /* fmac fr0,fr3,fr2 */
L8c612186: sa_motion_publish_stop(ctx,0x8c612186u); r[1]=rd(m,bus,&io_status,0x8c612398u,4);if(io_status)return io_status;goto L8c612188; /* mov.l 0x8c612398,r1 */
L8c612188: sa_motion_publish_stop(ctx,0x8c612188u); r[0]=12u;goto L8c61218a; /* mov #0xc,r0 */
L8c61218a: sa_motion_publish_stop(ctx,0x8c61218au); fcopy(f,6u,2u);goto L8c61218c; /* fmov fr2,fr6 */
L8c61218c: sa_motion_publish_stop(ctx,0x8c61218cu); r[2]=r[6];goto L8c61218e; /* mov r6,r2 */
L8c61218e: sa_motion_publish_stop(ctx,0x8c61218eu); fcopy(f,0u,5u);goto L8c612190; /* fmov fr5,fr0 */
L8c612190: sa_motion_publish_stop(ctx,0x8c612190u); r[2]<<=2;goto L8c612192; /* shll2 r2 */
L8c612192: sa_motion_publish_stop(ctx,0x8c612192u); r[2]<<=2;goto L8c612194; /* shll2 r2 */
L8c612194: sa_motion_publish_stop(ctx,0x8c612194u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612196; /* shll r2 */
L8c612196: sa_motion_publish_stop(ctx,0x8c612196u); r[2]+=r[13];goto L8c612198; /* add r13,r2 */
L8c612198: sa_motion_publish_stop(ctx,0x8c612198u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c61219a; /* mov.l r2,@(0x4,r15) */
L8c61219a: sa_motion_publish_stop(ctx,0x8c61219au); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61219c; /* fmov.s @r1,fr3 */
L8c61219c: sa_motion_publish_stop(ctx,0x8c61219cu); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61219e; /* fmov.s @(r0,r12),fr2 */
L8c61219e: sa_motion_publish_stop(ctx,0x8c61219eu); r[0]=16u;goto L8c6121a0; /* mov #0x10,r0 */
L8c6121a0: sa_motion_publish_stop(ctx,0x8c6121a0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6121a2; /* fmul fr6,fr3 */
L8c6121a2: sa_motion_publish_stop(ctx,0x8c6121a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6121a4; /* fmac fr0,fr3,fr2 */
L8c6121a4: sa_motion_publish_stop(ctx,0x8c6121a4u); fcopy(f,0u,4u);goto L8c6121a6; /* fmov fr4,fr0 */
L8c6121a6: sa_motion_publish_stop(ctx,0x8c6121a6u); fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6121a8; /* fmov fr2,@(r0,r2) */
L8c6121a8: sa_motion_publish_stop(ctx,0x8c6121a8u); r[0]=4u;goto L8c6121aa; /* mov #0x4,r0 */
L8c6121aa: sa_motion_publish_stop(ctx,0x8c6121aau); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6121ac; /* mov.l @(0xc,r15),r2 */
L8c6121ac: sa_motion_publish_stop(ctx,0x8c6121acu); fload(m,bus,&io_status,f,6u,r[0]+r[2]);if(io_status)return io_status;goto L8c6121ae; /* fmov.s @(r0,r2),fr6 */
L8c6121ae: sa_motion_publish_stop(ctx,0x8c6121aeu); r[0]=4u;goto L8c6121b0; /* mov #0x4,r0 */
L8c6121b0: sa_motion_publish_stop(ctx,0x8c6121b0u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6121b2; /* mov.l @(0x8,r15),r2 */
L8c6121b2: sa_motion_publish_stop(ctx,0x8c6121b2u); fcopy(f,2u,6u);goto L8c6121b4; /* fmov fr6,fr2 */
L8c6121b4: sa_motion_publish_stop(ctx,0x8c6121b4u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c6121b6; /* fmov.s @(r0,r2),fr3 */
L8c6121b6: sa_motion_publish_stop(ctx,0x8c6121b6u); r[0]=rd(m,bus,&io_status,0x8c61239cu,4);if(io_status)return io_status;goto L8c6121b8; /* mov.l 0x8c61239c,r0 */
L8c6121b8: sa_motion_publish_stop(ctx,0x8c6121b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6121ba; /* fsub fr6,fr3 */
L8c6121ba: sa_motion_publish_stop(ctx,0x8c6121bau); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6121bc; /* mov.l @(0x4,r15),r2 */
L8c6121bc: sa_motion_publish_stop(ctx,0x8c6121bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6121be; /* fmac fr0,fr3,fr2 */
L8c6121be: sa_motion_publish_stop(ctx,0x8c6121beu); fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c6121c0; /* fmov.s @r0,fr3 */
L8c6121c0: sa_motion_publish_stop(ctx,0x8c6121c0u); r[0]=16u;goto L8c6121c2; /* mov #0x10,r0 */
L8c6121c2: sa_motion_publish_stop(ctx,0x8c6121c2u); fcopy(f,6u,2u);goto L8c6121c4; /* fmov fr2,fr6 */
L8c6121c4: sa_motion_publish_stop(ctx,0x8c6121c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6121c6; /* fmul fr6,fr3 */
L8c6121c6: sa_motion_publish_stop(ctx,0x8c6121c6u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6121c8; /* fmov.s @(r0,r12),fr2 */
L8c6121c8: sa_motion_publish_stop(ctx,0x8c6121c8u); r[0]=20u;goto L8c6121ca; /* mov #0x14,r0 */
L8c6121ca: sa_motion_publish_stop(ctx,0x8c6121cau); fcopy(f,0u,5u);goto L8c6121cc; /* fmov fr5,fr0 */
L8c6121cc: sa_motion_publish_stop(ctx,0x8c6121ccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6121ce; /* fmac fr0,fr3,fr2 */
L8c6121ce: sa_motion_publish_stop(ctx,0x8c6121ceu); fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6121d0; /* fmov fr2,@(r0,r2) */
L8c6121d0: sa_motion_publish_stop(ctx,0x8c6121d0u); r[0]=24u;goto L8c6121d2; /* mov #0x18,r0 */
L8c6121d2: sa_motion_publish_stop(ctx,0x8c6121d2u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6121d4; /* mov.l @(0x4,r15),r3 */
L8c6121d4: sa_motion_publish_stop(ctx,0x8c6121d4u); fstore(m,bus,&io_status,f,0u,r[0]+r[3]);if(io_status)return io_status;goto L8c6121d6; /* fmov fr0,@(r0,r3) */
L8c6121d6: sa_motion_publish_stop(ctx,0x8c6121d6u); r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6121d8; /* mov.l @r15,r0 */
L8c6121d8: sa_motion_publish_stop(ctx,0x8c6121d8u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6121da; /* mov.l @(0x4,r15),r3 */
L8c6121da: sa_motion_publish_stop(ctx,0x8c6121dau); r[2]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c6121dc; /* mov.l @(r0,r7),r2 */
L8c6121dc: sa_motion_publish_stop(ctx,0x8c6121dcu); r[0]=28u;goto L8c6121de; /* mov #0x1c,r0 */
L8c6121de: sa_motion_publish_stop(ctx,0x8c6121deu); fcopy(f,0u,4u);goto L8c6121e0; /* fmov fr4,fr0 */
L8c6121e0: sa_motion_publish_stop(ctx,0x8c6121e0u); wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c6121e2; /* mov.l r2,@(0x14,r15) */
L8c6121e2: sa_motion_publish_stop(ctx,0x8c6121e2u); fload(m,bus,&io_status,f,6u,r[0]+r[2]);if(io_status)return io_status;goto L8c6121e4; /* fmov.s @(r0,r2),fr6 */
L8c6121e4: sa_motion_publish_stop(ctx,0x8c6121e4u); r[0]=r[7];goto L8c6121e6; /* mov r7,r0 */
L8c6121e6: sa_motion_publish_stop(ctx,0x8c6121e6u); r[2]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c6121e8; /* mov.l @(r0,r14),r2 */
L8c6121e8: sa_motion_publish_stop(ctx,0x8c6121e8u); r[0]=28u;goto L8c6121ea; /* mov #0x1c,r0 */
L8c6121ea: sa_motion_publish_stop(ctx,0x8c6121eau); fcopy(f,2u,6u);goto L8c6121ec; /* fmov fr6,fr2 */
L8c6121ec: sa_motion_publish_stop(ctx,0x8c6121ecu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c6121ee; /* fmov.s @(r0,r2),fr3 */
L8c6121ee: sa_motion_publish_stop(ctx,0x8c6121eeu); r[2]=r[0];goto L8c6121f0; /* mov r0,r2 */
L8c6121f0: sa_motion_publish_stop(ctx,0x8c6121f0u); r[2]+=r[3];goto L8c6121f2; /* add r3,r2 */
L8c6121f2: sa_motion_publish_stop(ctx,0x8c6121f2u); r[3]=r[6];goto L8c6121f4; /* mov r6,r3 */
L8c6121f4: sa_motion_publish_stop(ctx,0x8c6121f4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6121f6; /* fsub fr6,fr3 */
L8c6121f6: sa_motion_publish_stop(ctx,0x8c6121f6u); r[3]<<=2;goto L8c6121f8; /* shll2 r3 */
L8c6121f8: sa_motion_publish_stop(ctx,0x8c6121f8u); r[3]+=r[5];goto L8c6121fa; /* add r5,r3 */
L8c6121fa: sa_motion_publish_stop(ctx,0x8c6121fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6121fc; /* fmac fr0,fr3,fr2 */
L8c6121fc: sa_motion_publish_stop(ctx,0x8c6121fcu); fstore(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c6121fe; /* fmov.s fr2,@r2 */
L8c6121fe: sa_motion_publish_stop(ctx,0x8c6121feu); wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c612200; /* mov.l r3,@(0xc,r15) */
L8c612200: sa_motion_publish_stop(ctx,0x8c612200u); r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612202; /* mov.l @r15,r0 */
L8c612202: sa_motion_publish_stop(ctx,0x8c612202u); r[0]+=r[11];goto L8c612204; /* add r11,r0 */
L8c612204: sa_motion_publish_stop(ctx,0x8c612204u); wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;goto L8c612206; /* mov.l r0,@(0x8,r15) */
L8c612206: sa_motion_publish_stop(ctx,0x8c612206u); r[2]=r[14];goto L8c612208; /* mov r14,r2 */
L8c612208: sa_motion_publish_stop(ctx,0x8c612208u); r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61220a; /* mov.w @r0,r0 */
L8c61220a: sa_motion_publish_stop(ctx,0x8c61220au); r[2]+=r[11];goto L8c61220c; /* add r11,r2 */
L8c61220c: sa_motion_publish_stop(ctx,0x8c61220cu); wr(m,bus,&io_status,r[15]+0x10u,r[0],2);if(io_status)return io_status;goto L8c61220e; /* mov.w r0,@(0x10,r15) */
L8c61220e: sa_motion_publish_stop(ctx,0x8c61220eu); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c612210; /* mov.l r2,@(0x4,r15) */
L8c612210: sa_motion_publish_stop(ctx,0x8c612210u); *fpul=r[0];goto L8c612212; /* lds r0,FPUL */
L8c612212: sa_motion_publish_stop(ctx,0x8c612212u); r[0]=sx(m,bus,&io_status,r[15]+0x10u,2);if(io_status)return io_status;goto L8c612214; /* mov.w @(0x10,r15),r0 */
L8c612214: sa_motion_publish_stop(ctx,0x8c612214u); r[2]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c612216; /* mov.w @r2,r2 */
L8c612216: sa_motion_publish_stop(ctx,0x8c612216u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612218; /* float FPUL,fr3 */
L8c612218: sa_motion_publish_stop(ctx,0x8c612218u); r[2]-=r[0];goto L8c61221a; /* sub r0,r2 */
L8c61221a: sa_motion_publish_stop(ctx,0x8c61221au); *fpul=r[2];goto L8c61221c; /* lds r2,FPUL */
L8c61221c: sa_motion_publish_stop(ctx,0x8c61221cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c61221e; /* float FPUL,fr2 */
L8c61221e: sa_motion_publish_stop(ctx,0x8c61221eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612220; /* fmac fr0,fr2,fr3 */
L8c612220: sa_motion_publish_stop(ctx,0x8c612220u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c612222; /* ftrc fr3,FPUL */
L8c612222: sa_motion_publish_stop(ctx,0x8c612222u); r[2]=*fpul;goto L8c612224; /* sts FPUL,r2 */
L8c612224: sa_motion_publish_stop(ctx,0x8c612224u); wr(m,bus,&io_status,r[3],r[2],2);if(io_status)return io_status;goto L8c612226; /* mov.w r2,@r3 */
L8c612226: sa_motion_publish_stop(ctx,0x8c612226u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612228; /* mov.l @(0x8,r15),r2 */
L8c612228: sa_motion_publish_stop(ctx,0x8c612228u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61222a; /* mov.l @(0xc,r15),r3 */
L8c61222a: sa_motion_publish_stop(ctx,0x8c61222au); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61222c; /* mov.w @(0x2,r2),r0 */
L8c61222c: sa_motion_publish_stop(ctx,0x8c61222cu); *fpul=r[0];goto L8c61222e; /* lds r0,FPUL */
L8c61222e: sa_motion_publish_stop(ctx,0x8c61222eu); wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c612230; /* mov.w r0,@r15 */
L8c612230: sa_motion_publish_stop(ctx,0x8c612230u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612232; /* mov.l @(0x4,r15),r2 */
L8c612232: sa_motion_publish_stop(ctx,0x8c612232u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612234; /* float FPUL,fr3 */
L8c612234: sa_motion_publish_stop(ctx,0x8c612234u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c612236; /* mov.w @(0x2,r2),r0 */
L8c612236: sa_motion_publish_stop(ctx,0x8c612236u); r[2]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c612238; /* mov.w @r15,r2 */
L8c612238: sa_motion_publish_stop(ctx,0x8c612238u); r[0]-=r[2];goto L8c61223a; /* sub r2,r0 */
L8c61223a: sa_motion_publish_stop(ctx,0x8c61223au); *fpul=r[0];goto L8c61223c; /* lds r0,FPUL */
L8c61223c: sa_motion_publish_stop(ctx,0x8c61223cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c61223e; /* float FPUL,fr2 */
L8c61223e: sa_motion_publish_stop(ctx,0x8c61223eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612240; /* fmac fr0,fr2,fr3 */
L8c612240: sa_motion_publish_stop(ctx,0x8c612240u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c612242; /* ftrc fr3,FPUL */
L8c612242: sa_motion_publish_stop(ctx,0x8c612242u); r[0]=*fpul;goto L8c612244; /* sts FPUL,r0 */
L8c612244: sa_motion_publish_stop(ctx,0x8c612244u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c612246; /* mov.w r0,@(0x2,r3) */
L8c612246: sa_motion_publish_stop(ctx,0x8c612246u); r[6]+=1u;goto L8c612248; /* add #0x1,r6 */
L8c612248: sa_motion_publish_stop(ctx,0x8c612248u); r[0]=r[14];goto L8c61224a; /* mov r14,r0 */
L8c61224a: sa_motion_publish_stop(ctx,0x8c61224au); r[3]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c61224c; /* mov.l @(r0,r7),r3 */
L8c61224c: sa_motion_publish_stop(ctx,0x8c61224cu); r[3]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c61224e; /* mov.l @(0xc,r3),r3 */
L8c61224e: sa_motion_publish_stop(ctx,0x8c61224eu); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c612250; /* tst r4,r3 */
L8c612250: sa_motion_publish_stop(ctx,0x8c612250u); if(!(*sr&1u))goto L8c6122ac;goto L8c612252; /* bf 0x8c6122ac */
L8c612252: sa_motion_publish_stop(ctx,0x8c612252u); r[3]=r[14];goto L8c612254; /* mov r14,r3 */
L8c612254: sa_motion_publish_stop(ctx,0x8c612254u); r[2]=r[6];goto L8c612256; /* mov r6,r2 */
L8c612256: sa_motion_publish_stop(ctx,0x8c612256u); r[3]+=r[7];goto L8c612258; /* add r7,r3 */
L8c612258: sa_motion_publish_stop(ctx,0x8c612258u); r[2]<<=2;goto L8c61225a; /* shll2 r2 */
L8c61225a: sa_motion_publish_stop(ctx,0x8c61225au); r[0]=16u;goto L8c61225c; /* mov #0x10,r0 */
L8c61225c: sa_motion_publish_stop(ctx,0x8c61225cu); r[2]<<=2;goto L8c61225e; /* shll2 r2 */
L8c61225e: sa_motion_publish_stop(ctx,0x8c61225eu); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612260; /* shll r2 */
L8c612260: sa_motion_publish_stop(ctx,0x8c612260u); r[2]+=r[13];goto L8c612262; /* add r13,r2 */
L8c612262: sa_motion_publish_stop(ctx,0x8c612262u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c612264; /* mov.l r2,@(0x4,r15) */
L8c612264: sa_motion_publish_stop(ctx,0x8c612264u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c612266; /* mov.l r3,@r15 */
L8c612266: sa_motion_publish_stop(ctx,0x8c612266u); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c612268; /* mov.l @r3,r1 */
L8c612268: sa_motion_publish_stop(ctx,0x8c612268u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61226a; /* fmov.s @(r0,r1),fr3 */
L8c61226a: sa_motion_publish_stop(ctx,0x8c61226au); fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61226c; /* fmov fr3,@(r0,r2) */
L8c61226c: sa_motion_publish_stop(ctx,0x8c61226cu); r[0]=20u;goto L8c61226e; /* mov #0x14,r0 */
L8c61226e: sa_motion_publish_stop(ctx,0x8c61226eu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612270; /* mov.l @r15,r2 */
L8c612270: sa_motion_publish_stop(ctx,0x8c612270u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612272; /* mov.l @(0x4,r15),r3 */
L8c612272: sa_motion_publish_stop(ctx,0x8c612272u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c612274; /* mov.l @r2,r1 */
L8c612274: sa_motion_publish_stop(ctx,0x8c612274u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612276; /* fmov.s @(r0,r1),fr3 */
L8c612276: sa_motion_publish_stop(ctx,0x8c612276u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612278; /* fmov fr3,@(r0,r3) */
L8c612278: sa_motion_publish_stop(ctx,0x8c612278u); r[0]=24u;goto L8c61227a; /* mov #0x18,r0 */
L8c61227a: sa_motion_publish_stop(ctx,0x8c61227au); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61227c; /* mov.l @r15,r2 */
L8c61227c: sa_motion_publish_stop(ctx,0x8c61227cu); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61227e; /* mov.l @(0x4,r15),r3 */
L8c61227e: sa_motion_publish_stop(ctx,0x8c61227eu); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c612280; /* mov.l @r2,r1 */
L8c612280: sa_motion_publish_stop(ctx,0x8c612280u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612282; /* fmov.s @(r0,r1),fr3 */
L8c612282: sa_motion_publish_stop(ctx,0x8c612282u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612284; /* fmov fr3,@(r0,r3) */
L8c612284: sa_motion_publish_stop(ctx,0x8c612284u); r[0]=28u;goto L8c612286; /* mov #0x1c,r0 */
L8c612286: sa_motion_publish_stop(ctx,0x8c612286u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612288; /* mov.l @r15,r2 */
L8c612288: sa_motion_publish_stop(ctx,0x8c612288u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61228a; /* mov.l @(0x4,r15),r3 */
L8c61228a: sa_motion_publish_stop(ctx,0x8c61228au); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61228c; /* mov.l @r2,r1 */
L8c61228c: sa_motion_publish_stop(ctx,0x8c61228cu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61228e; /* fmov.s @(r0,r1),fr3 */
L8c61228e: sa_motion_publish_stop(ctx,0x8c61228eu); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612290; /* fmov fr3,@(r0,r3) */
L8c612290: sa_motion_publish_stop(ctx,0x8c612290u); r[3]=r[6];goto L8c612292; /* mov r6,r3 */
L8c612292: sa_motion_publish_stop(ctx,0x8c612292u); r[2]=r[14];goto L8c612294; /* mov r14,r2 */
L8c612294: sa_motion_publish_stop(ctx,0x8c612294u); r[2]+=r[11];goto L8c612296; /* add r11,r2 */
L8c612296: sa_motion_publish_stop(ctx,0x8c612296u); r[3]<<=2;goto L8c612298; /* shll2 r3 */
L8c612298: sa_motion_publish_stop(ctx,0x8c612298u); r[3]+=r[5];goto L8c61229a; /* add r5,r3 */
L8c61229a: sa_motion_publish_stop(ctx,0x8c61229au); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61229c; /* mov.l r3,@(0x4,r15) */
L8c61229c: sa_motion_publish_stop(ctx,0x8c61229cu); r[6]+=1u;goto L8c61229e; /* add #0x1,r6 */
L8c61229e: sa_motion_publish_stop(ctx,0x8c61229eu); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6122a0; /* mov.l r2,@r15 */
L8c6122a0: sa_motion_publish_stop(ctx,0x8c6122a0u); r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c6122a2; /* mov.w @r2,r1 */
L8c6122a2: sa_motion_publish_stop(ctx,0x8c6122a2u); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c6122a4; /* mov.w r1,@r3 */
L8c6122a4: sa_motion_publish_stop(ctx,0x8c6122a4u); r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6122a6; /* mov.l @r15,r0 */
L8c6122a6: sa_motion_publish_stop(ctx,0x8c6122a6u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6122a8; /* mov.l @(0x4,r15),r3 */
L8c6122a8: sa_motion_publish_stop(ctx,0x8c6122a8u); r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c6122aa; /* mov.w @(0x2,r0),r0 */
L8c6122aa: sa_motion_publish_stop(ctx,0x8c6122aau); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6122ac; /* mov.w r0,@(0x2,r3) */
L8c6122ac: sa_motion_publish_stop(ctx,0x8c6122acu); wr(m,bus,&io_status,r[15]+0x4u,r[10],4);if(io_status)return io_status;goto L8c6122ae; /* mov.l r10,@(0x4,r15) */
L8c6122ae: sa_motion_publish_stop(ctx,0x8c6122aeu); r[3]=3u;goto L8c6122b0; /* mov #0x3,r3 */
L8c6122b0: sa_motion_publish_stop(ctx,0x8c6122b0u); r[10]+=1u;goto L8c6122b2; /* add #0x1,r10 */
L8c6122b2: sa_motion_publish_stop(ctx,0x8c6122b2u); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[3]);goto L8c6122b4; /* cmp/ge r3,r10 */
L8c6122b4: sa_motion_publish_stop(ctx,0x8c6122b4u); if((*sr&1u))goto L8c6122ba;goto L8c6122b6; /* bt 0x8c6122ba */
L8c6122b6: sa_motion_publish_stop(ctx,0x8c6122b8u); goto L8c612122; /* bra 0x8c612122 */
L8c6122ba: sa_motion_publish_stop(ctx,0x8c6122bau); r[0]=0x8c6123a8u;goto L8c6122bc; /* mova 0x8c6123a8,r0 */
L8c6122bc: sa_motion_publish_stop(ctx,0x8c6122bcu); r[4]=rd(m,bus,&io_status,0x8c6123a4u,4);if(io_status)return io_status;goto L8c6122be; /* mov.l 0x8c6123a4,r4 */
L8c6122be: sa_motion_publish_stop(ctx,0x8c6122beu); fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c6122c0; /* fmov.s @r0,fr4 */
L8c6122c0: sa_motion_publish_stop(ctx,0x8c6122c0u); r[0]=r[6];goto L8c6122c2; /* mov r6,r0 */
L8c6122c2: sa_motion_publish_stop(ctx,0x8c6122c2u); r[7]=rd(m,bus,&io_status,0x8c6123a0u,4);if(io_status)return io_status;goto L8c6122c4; /* mov.l 0x8c6123a0,r7 */
L8c6122c4: sa_motion_publish_stop(ctx,0x8c6122c4u); *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c6122c6; /* cmp/eq #0x3,r0 */
L8c6122c6: sa_motion_publish_stop(ctx,0x8c6122c6u); if(!(*sr&1u))goto L8c6123b0;goto L8c6122c8; /* bf 0x8c6123b0 */
L8c6122c8: sa_motion_publish_stop(ctx,0x8c6122c8u); r[0]=28u;goto L8c6122ca; /* mov #0x1c,r0 */
L8c6122ca: sa_motion_publish_stop(ctx,0x8c6122cau); r[6]=r[13];goto L8c6122cc; /* mov r13,r6 */
L8c6122cc: sa_motion_publish_stop(ctx,0x8c6122ccu); r[3]=r[5];goto L8c6122ce; /* mov r5,r3 */
L8c6122ce: sa_motion_publish_stop(ctx,0x8c6122ceu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6122d0; /* fmov.s @(r0,r6),fr3 */
L8c6122d0: sa_motion_publish_stop(ctx,0x8c6122d0u); r[8]+=28u;goto L8c6122d2; /* add #0x1c,r8 */
L8c6122d2: sa_motion_publish_stop(ctx,0x8c6122d2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6122d4; /* fmov.s fr3,@-r8 */
L8c6122d4: sa_motion_publish_stop(ctx,0x8c6122d4u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6122d6; /* mov.l r3,@r15 */
L8c6122d6: sa_motion_publish_stop(ctx,0x8c6122d6u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6122d8; /* mov.w @(0x2,r3),r0 */
L8c6122d8: sa_motion_publish_stop(ctx,0x8c6122d8u); r[3]=r[0];goto L8c6122da; /* mov r0,r3 */
L8c6122da: sa_motion_publish_stop(ctx,0x8c6122dau); *fpul=r[3];goto L8c6122dc; /* lds r3,FPUL */
L8c6122dc: sa_motion_publish_stop(ctx,0x8c6122dcu); r[0]=24u;goto L8c6122de; /* mov #0x18,r0 */
L8c6122de: sa_motion_publish_stop(ctx,0x8c6122deu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6122e0; /* float FPUL,fr3 */
L8c6122e0: sa_motion_publish_stop(ctx,0x8c6122e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6122e2; /* fmul fr4,fr3 */
L8c6122e2: sa_motion_publish_stop(ctx,0x8c6122e2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6122e4; /* fmov.s fr3,@-r8 */
L8c6122e4: sa_motion_publish_stop(ctx,0x8c6122e4u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6122e6; /* mov.l @r15,r3 */
L8c6122e6: sa_motion_publish_stop(ctx,0x8c6122e6u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6122e8; /* mov.w @r3,r3 */
L8c6122e8: sa_motion_publish_stop(ctx,0x8c6122e8u); *fpul=r[3];goto L8c6122ea; /* lds r3,FPUL */
L8c6122ea: sa_motion_publish_stop(ctx,0x8c6122eau); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6122ec; /* float FPUL,fr3 */
L8c6122ec: sa_motion_publish_stop(ctx,0x8c6122ecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6122ee; /* fmul fr4,fr3 */
L8c6122ee: sa_motion_publish_stop(ctx,0x8c6122eeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6122f0; /* fmov.s fr3,@-r8 */
L8c6122f0: sa_motion_publish_stop(ctx,0x8c6122f0u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6122f2; /* fmov.s @(r0,r6),fr3 */
L8c6122f2: sa_motion_publish_stop(ctx,0x8c6122f2u); r[0]=20u;goto L8c6122f4; /* mov #0x14,r0 */
L8c6122f4: sa_motion_publish_stop(ctx,0x8c6122f4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6122f6; /* fmov.s fr3,@-r8 */
L8c6122f6: sa_motion_publish_stop(ctx,0x8c6122f6u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6122f8; /* fmov.s @(r0,r6),fr3 */
L8c6122f8: sa_motion_publish_stop(ctx,0x8c6122f8u); r[0]=16u;goto L8c6122fa; /* mov #0x10,r0 */
L8c6122fa: sa_motion_publish_stop(ctx,0x8c6122fau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6122fc; /* fmov.s fr3,@-r8 */
L8c6122fc: sa_motion_publish_stop(ctx,0x8c6122fcu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6122fe; /* fmov.s @(r0,r6),fr3 */
L8c6122fe: sa_motion_publish_stop(ctx,0x8c6122feu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612300; /* fmov.s fr3,@-r8 */
L8c612300: sa_motion_publish_stop(ctx,0x8c612300u); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612302; /* mov.l r4,@-r8 */
L8c612302: sa_motion_publish_stop(ctx,0x8c612302u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612304; /* pref @r8 */
L8c612304: sa_motion_publish_stop(ctx,0x8c612304u); r[0]=28u;goto L8c612306; /* mov #0x1c,r0 */
L8c612306: sa_motion_publish_stop(ctx,0x8c612306u); r[3]=r[5];goto L8c612308; /* mov r5,r3 */
L8c612308: sa_motion_publish_stop(ctx,0x8c612308u); r[6]+=32u;goto L8c61230a; /* add #0x20,r6 */
L8c61230a: sa_motion_publish_stop(ctx,0x8c61230au); r[3]+=4u;goto L8c61230c; /* add #0x4,r3 */
L8c61230c: sa_motion_publish_stop(ctx,0x8c61230cu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61230e; /* fmov.s @(r0,r6),fr3 */
L8c61230e: sa_motion_publish_stop(ctx,0x8c61230eu); r[8]+=60u;goto L8c612310; /* add #0x3c,r8 */
L8c612310: sa_motion_publish_stop(ctx,0x8c612310u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612312; /* fmov.s fr3,@-r8 */
L8c612312: sa_motion_publish_stop(ctx,0x8c612312u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c612314; /* mov.l r3,@r15 */
L8c612314: sa_motion_publish_stop(ctx,0x8c612314u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c612316; /* mov.w @(0x2,r3),r0 */
L8c612316: sa_motion_publish_stop(ctx,0x8c612316u); r[3]=r[0];goto L8c612318; /* mov r0,r3 */
L8c612318: sa_motion_publish_stop(ctx,0x8c612318u); *fpul=r[3];goto L8c61231a; /* lds r3,FPUL */
L8c61231a: sa_motion_publish_stop(ctx,0x8c61231au); r[0]=24u;goto L8c61231c; /* mov #0x18,r0 */
L8c61231c: sa_motion_publish_stop(ctx,0x8c61231cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61231e; /* float FPUL,fr3 */
L8c61231e: sa_motion_publish_stop(ctx,0x8c61231eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612320; /* fmul fr4,fr3 */
L8c612320: sa_motion_publish_stop(ctx,0x8c612320u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612322; /* fmov.s fr3,@-r8 */
L8c612322: sa_motion_publish_stop(ctx,0x8c612322u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612324; /* mov.l @r15,r3 */
L8c612324: sa_motion_publish_stop(ctx,0x8c612324u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c612326; /* mov.w @r3,r3 */
L8c612326: sa_motion_publish_stop(ctx,0x8c612326u); *fpul=r[3];goto L8c612328; /* lds r3,FPUL */
L8c612328: sa_motion_publish_stop(ctx,0x8c612328u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61232a; /* float FPUL,fr3 */
L8c61232a: sa_motion_publish_stop(ctx,0x8c61232au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61232c; /* fmul fr4,fr3 */
L8c61232c: sa_motion_publish_stop(ctx,0x8c61232cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61232e; /* fmov.s fr3,@-r8 */
L8c61232e: sa_motion_publish_stop(ctx,0x8c61232eu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612330; /* fmov.s @(r0,r6),fr3 */
L8c612330: sa_motion_publish_stop(ctx,0x8c612330u); r[0]=20u;goto L8c612332; /* mov #0x14,r0 */
L8c612332: sa_motion_publish_stop(ctx,0x8c612332u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612334; /* fmov.s fr3,@-r8 */
L8c612334: sa_motion_publish_stop(ctx,0x8c612334u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612336; /* fmov.s @(r0,r6),fr3 */
L8c612336: sa_motion_publish_stop(ctx,0x8c612336u); r[0]=16u;goto L8c612338; /* mov #0x10,r0 */
L8c612338: sa_motion_publish_stop(ctx,0x8c612338u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61233a; /* fmov.s fr3,@-r8 */
L8c61233a: sa_motion_publish_stop(ctx,0x8c61233au); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61233c; /* fmov.s @(r0,r6),fr3 */
L8c61233c: sa_motion_publish_stop(ctx,0x8c61233cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61233e; /* fmov.s fr3,@-r8 */
L8c61233e: sa_motion_publish_stop(ctx,0x8c61233eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612340; /* mov.l r4,@-r8 */
L8c612340: sa_motion_publish_stop(ctx,0x8c612340u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612342; /* pref @r8 */
L8c612342: sa_motion_publish_stop(ctx,0x8c612342u); r[4]=r[13];goto L8c612344; /* mov r13,r4 */
L8c612344: sa_motion_publish_stop(ctx,0x8c612344u); r[0]=28u;goto L8c612346; /* mov #0x1c,r0 */
L8c612346: sa_motion_publish_stop(ctx,0x8c612346u); r[6]=r[5];goto L8c612348; /* mov r5,r6 */
L8c612348: sa_motion_publish_stop(ctx,0x8c612348u); r[4]+=64u;goto L8c61234a; /* add #0x40,r4 */
L8c61234a: sa_motion_publish_stop(ctx,0x8c61234au); r[6]+=8u;goto L8c61234c; /* add #0x8,r6 */
L8c61234c: sa_motion_publish_stop(ctx,0x8c61234cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61234e; /* fmov.s @(r0,r4),fr3 */
L8c61234e: sa_motion_publish_stop(ctx,0x8c61234eu); r[8]+=60u;goto L8c612350; /* add #0x3c,r8 */
L8c612350: sa_motion_publish_stop(ctx,0x8c612350u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612352; /* fmov.s fr3,@-r8 */
L8c612352: sa_motion_publish_stop(ctx,0x8c612352u); r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c612354; /* mov.w @(0x2,r6),r0 */
L8c612354: sa_motion_publish_stop(ctx,0x8c612354u); r[3]=r[0];goto L8c612356; /* mov r0,r3 */
L8c612356: sa_motion_publish_stop(ctx,0x8c612356u); *fpul=r[3];goto L8c612358; /* lds r3,FPUL */
L8c612358: sa_motion_publish_stop(ctx,0x8c612358u); r[0]=24u;goto L8c61235a; /* mov #0x18,r0 */
L8c61235a: sa_motion_publish_stop(ctx,0x8c61235au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61235c; /* float FPUL,fr3 */
L8c61235c: sa_motion_publish_stop(ctx,0x8c61235cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61235e; /* fmul fr4,fr3 */
L8c61235e: sa_motion_publish_stop(ctx,0x8c61235eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612360; /* fmov.s fr3,@-r8 */
L8c612360: sa_motion_publish_stop(ctx,0x8c612360u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c612362; /* mov.w @r6,r3 */
L8c612362: sa_motion_publish_stop(ctx,0x8c612362u); *fpul=r[3];goto L8c612364; /* lds r3,FPUL */
L8c612364: sa_motion_publish_stop(ctx,0x8c612364u); r[3]=rd(m,bus,&io_status,0x8c6123acu,4);if(io_status)return io_status;goto L8c612366; /* mov.l 0x8c6123ac,r3 */
L8c612366: sa_motion_publish_stop(ctx,0x8c612366u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612368; /* float FPUL,fr3 */
L8c612368: sa_motion_publish_stop(ctx,0x8c612368u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61236a; /* fmul fr4,fr3 */
L8c61236a: sa_motion_publish_stop(ctx,0x8c61236au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61236c; /* fmov.s fr3,@-r8 */
L8c61236c: sa_motion_publish_stop(ctx,0x8c61236cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61236e; /* fmov.s @(r0,r4),fr3 */
L8c61236e: sa_motion_publish_stop(ctx,0x8c61236eu); r[0]=20u;goto L8c612370; /* mov #0x14,r0 */
L8c612370: sa_motion_publish_stop(ctx,0x8c612370u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612372; /* fmov.s fr3,@-r8 */
L8c612372: sa_motion_publish_stop(ctx,0x8c612372u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612374; /* fmov.s @(r0,r4),fr3 */
L8c612374: sa_motion_publish_stop(ctx,0x8c612374u); r[0]=16u;goto L8c612376; /* mov #0x10,r0 */
L8c612376: sa_motion_publish_stop(ctx,0x8c612376u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612378; /* fmov.s fr3,@-r8 */
L8c612378: sa_motion_publish_stop(ctx,0x8c612378u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61237a; /* fmov.s @(r0,r4),fr3 */
L8c61237a: sa_motion_publish_stop(ctx,0x8c61237au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61237c; /* fmov.s fr3,@-r8 */
L8c61237c: sa_motion_publish_stop(ctx,0x8c61237cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61237e; /* mov.l r3,@-r8 */
L8c61237e: sa_motion_publish_stop(ctx,0x8c61237eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612380; /* pref @r8 */
L8c612380: sa_motion_publish_stop(ctx,0x8c612380u); r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c612382; /* mov.l @r7,r2 */
L8c612382: sa_motion_publish_stop(ctx,0x8c612382u); r[8]+=32u;goto L8c612384; /* add #0x20,r8 */
L8c612384: sa_motion_publish_stop(ctx,0x8c612386u); r[2]+=3u;goto L8c6124ae; /* bra 0x8c6124ae */
L8c6123b0: sa_motion_publish_stop(ctx,0x8c6123b0u); r[0]=28u;goto L8c6123b2; /* mov #0x1c,r0 */
L8c6123b2: sa_motion_publish_stop(ctx,0x8c6123b2u); r[6]=r[13];goto L8c6123b4; /* mov r13,r6 */
L8c6123b4: sa_motion_publish_stop(ctx,0x8c6123b4u); r[3]=r[5];goto L8c6123b6; /* mov r5,r3 */
L8c6123b6: sa_motion_publish_stop(ctx,0x8c6123b6u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6123b8; /* fmov.s @(r0,r6),fr3 */
L8c6123b8: sa_motion_publish_stop(ctx,0x8c6123b8u); r[8]+=28u;goto L8c6123ba; /* add #0x1c,r8 */
L8c6123ba: sa_motion_publish_stop(ctx,0x8c6123bau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6123bc; /* fmov.s fr3,@-r8 */
L8c6123bc: sa_motion_publish_stop(ctx,0x8c6123bcu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6123be; /* mov.l r3,@r15 */
L8c6123be: sa_motion_publish_stop(ctx,0x8c6123beu); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6123c0; /* mov.w @(0x2,r3),r0 */
L8c6123c0: sa_motion_publish_stop(ctx,0x8c6123c0u); r[3]=r[0];goto L8c6123c2; /* mov r0,r3 */
L8c6123c2: sa_motion_publish_stop(ctx,0x8c6123c2u); *fpul=r[3];goto L8c6123c4; /* lds r3,FPUL */
L8c6123c4: sa_motion_publish_stop(ctx,0x8c6123c4u); r[0]=24u;goto L8c6123c6; /* mov #0x18,r0 */
L8c6123c6: sa_motion_publish_stop(ctx,0x8c6123c6u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6123c8; /* float FPUL,fr3 */
L8c6123c8: sa_motion_publish_stop(ctx,0x8c6123c8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6123ca; /* fmul fr4,fr3 */
L8c6123ca: sa_motion_publish_stop(ctx,0x8c6123cau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6123cc; /* fmov.s fr3,@-r8 */
L8c6123cc: sa_motion_publish_stop(ctx,0x8c6123ccu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6123ce; /* mov.l @r15,r3 */
L8c6123ce: sa_motion_publish_stop(ctx,0x8c6123ceu); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6123d0; /* mov.w @r3,r3 */
L8c6123d0: sa_motion_publish_stop(ctx,0x8c6123d0u); *fpul=r[3];goto L8c6123d2; /* lds r3,FPUL */
L8c6123d2: sa_motion_publish_stop(ctx,0x8c6123d2u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6123d4; /* float FPUL,fr3 */
L8c6123d4: sa_motion_publish_stop(ctx,0x8c6123d4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6123d6; /* fmul fr4,fr3 */
L8c6123d6: sa_motion_publish_stop(ctx,0x8c6123d6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6123d8; /* fmov.s fr3,@-r8 */
L8c6123d8: sa_motion_publish_stop(ctx,0x8c6123d8u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6123da; /* fmov.s @(r0,r6),fr3 */
L8c6123da: sa_motion_publish_stop(ctx,0x8c6123dau); r[0]=20u;goto L8c6123dc; /* mov #0x14,r0 */
L8c6123dc: sa_motion_publish_stop(ctx,0x8c6123dcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6123de; /* fmov.s fr3,@-r8 */
L8c6123de: sa_motion_publish_stop(ctx,0x8c6123deu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6123e0; /* fmov.s @(r0,r6),fr3 */
L8c6123e0: sa_motion_publish_stop(ctx,0x8c6123e0u); r[0]=16u;goto L8c6123e2; /* mov #0x10,r0 */
L8c6123e2: sa_motion_publish_stop(ctx,0x8c6123e2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6123e4; /* fmov.s fr3,@-r8 */
L8c6123e4: sa_motion_publish_stop(ctx,0x8c6123e4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6123e6; /* fmov.s @(r0,r6),fr3 */
L8c6123e6: sa_motion_publish_stop(ctx,0x8c6123e6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6123e8; /* fmov.s fr3,@-r8 */
L8c6123e8: sa_motion_publish_stop(ctx,0x8c6123e8u); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c6123ea; /* mov.l r4,@-r8 */
L8c6123ea: sa_motion_publish_stop(ctx,0x8c6123eau); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6123ec; /* pref @r8 */
L8c6123ec: sa_motion_publish_stop(ctx,0x8c6123ecu); r[0]=28u;goto L8c6123ee; /* mov #0x1c,r0 */
L8c6123ee: sa_motion_publish_stop(ctx,0x8c6123eeu); r[3]=r[5];goto L8c6123f0; /* mov r5,r3 */
L8c6123f0: sa_motion_publish_stop(ctx,0x8c6123f0u); r[6]+=32u;goto L8c6123f2; /* add #0x20,r6 */
L8c6123f2: sa_motion_publish_stop(ctx,0x8c6123f2u); r[3]+=4u;goto L8c6123f4; /* add #0x4,r3 */
L8c6123f4: sa_motion_publish_stop(ctx,0x8c6123f4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6123f6; /* fmov.s @(r0,r6),fr3 */
L8c6123f6: sa_motion_publish_stop(ctx,0x8c6123f6u); r[8]+=60u;goto L8c6123f8; /* add #0x3c,r8 */
L8c6123f8: sa_motion_publish_stop(ctx,0x8c6123f8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6123fa; /* fmov.s fr3,@-r8 */
L8c6123fa: sa_motion_publish_stop(ctx,0x8c6123fau); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6123fc; /* mov.l r3,@r15 */
L8c6123fc: sa_motion_publish_stop(ctx,0x8c6123fcu); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6123fe; /* mov.w @(0x2,r3),r0 */
L8c6123fe: sa_motion_publish_stop(ctx,0x8c6123feu); r[3]=r[0];goto L8c612400; /* mov r0,r3 */
L8c612400: sa_motion_publish_stop(ctx,0x8c612400u); *fpul=r[3];goto L8c612402; /* lds r3,FPUL */
L8c612402: sa_motion_publish_stop(ctx,0x8c612402u); r[0]=24u;goto L8c612404; /* mov #0x18,r0 */
L8c612404: sa_motion_publish_stop(ctx,0x8c612404u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612406; /* float FPUL,fr3 */
L8c612406: sa_motion_publish_stop(ctx,0x8c612406u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612408; /* fmul fr4,fr3 */
L8c612408: sa_motion_publish_stop(ctx,0x8c612408u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61240a; /* fmov.s fr3,@-r8 */
L8c61240a: sa_motion_publish_stop(ctx,0x8c61240au); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61240c; /* mov.l @r15,r3 */
L8c61240c: sa_motion_publish_stop(ctx,0x8c61240cu); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c61240e; /* mov.w @r3,r3 */
L8c61240e: sa_motion_publish_stop(ctx,0x8c61240eu); *fpul=r[3];goto L8c612410; /* lds r3,FPUL */
L8c612410: sa_motion_publish_stop(ctx,0x8c612410u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612412; /* float FPUL,fr3 */
L8c612412: sa_motion_publish_stop(ctx,0x8c612412u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612414; /* fmul fr4,fr3 */
L8c612414: sa_motion_publish_stop(ctx,0x8c612414u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612416; /* fmov.s fr3,@-r8 */
L8c612416: sa_motion_publish_stop(ctx,0x8c612416u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612418; /* fmov.s @(r0,r6),fr3 */
L8c612418: sa_motion_publish_stop(ctx,0x8c612418u); r[0]=20u;goto L8c61241a; /* mov #0x14,r0 */
L8c61241a: sa_motion_publish_stop(ctx,0x8c61241au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61241c; /* fmov.s fr3,@-r8 */
L8c61241c: sa_motion_publish_stop(ctx,0x8c61241cu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61241e; /* fmov.s @(r0,r6),fr3 */
L8c61241e: sa_motion_publish_stop(ctx,0x8c61241eu); r[0]=16u;goto L8c612420; /* mov #0x10,r0 */
L8c612420: sa_motion_publish_stop(ctx,0x8c612420u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612422; /* fmov.s fr3,@-r8 */
L8c612422: sa_motion_publish_stop(ctx,0x8c612422u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612424; /* fmov.s @(r0,r6),fr3 */
L8c612424: sa_motion_publish_stop(ctx,0x8c612424u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612426; /* fmov.s fr3,@-r8 */
L8c612426: sa_motion_publish_stop(ctx,0x8c612426u); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612428; /* mov.l r4,@-r8 */
L8c612428: sa_motion_publish_stop(ctx,0x8c612428u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61242a; /* pref @r8 */
L8c61242a: sa_motion_publish_stop(ctx,0x8c61242au); r[6]=r[13];goto L8c61242c; /* mov r13,r6 */
L8c61242c: sa_motion_publish_stop(ctx,0x8c61242cu); r[0]=28u;goto L8c61242e; /* mov #0x1c,r0 */
L8c61242e: sa_motion_publish_stop(ctx,0x8c61242eu); r[3]=r[5];goto L8c612430; /* mov r5,r3 */
L8c612430: sa_motion_publish_stop(ctx,0x8c612430u); r[6]+=96u;goto L8c612432; /* add #0x60,r6 */
L8c612432: sa_motion_publish_stop(ctx,0x8c612432u); r[3]+=12u;goto L8c612434; /* add #0xc,r3 */
L8c612434: sa_motion_publish_stop(ctx,0x8c612434u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612436; /* fmov.s @(r0,r6),fr3 */
L8c612436: sa_motion_publish_stop(ctx,0x8c612436u); r[8]+=60u;goto L8c612438; /* add #0x3c,r8 */
L8c612438: sa_motion_publish_stop(ctx,0x8c612438u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61243a; /* fmov.s fr3,@-r8 */
L8c61243a: sa_motion_publish_stop(ctx,0x8c61243au); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61243c; /* mov.l r3,@r15 */
L8c61243c: sa_motion_publish_stop(ctx,0x8c61243cu); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61243e; /* mov.w @(0x2,r3),r0 */
L8c61243e: sa_motion_publish_stop(ctx,0x8c61243eu); r[3]=r[0];goto L8c612440; /* mov r0,r3 */
L8c612440: sa_motion_publish_stop(ctx,0x8c612440u); *fpul=r[3];goto L8c612442; /* lds r3,FPUL */
L8c612442: sa_motion_publish_stop(ctx,0x8c612442u); r[0]=24u;goto L8c612444; /* mov #0x18,r0 */
L8c612444: sa_motion_publish_stop(ctx,0x8c612444u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612446; /* float FPUL,fr3 */
L8c612446: sa_motion_publish_stop(ctx,0x8c612446u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612448; /* fmul fr4,fr3 */
L8c612448: sa_motion_publish_stop(ctx,0x8c612448u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61244a; /* fmov.s fr3,@-r8 */
L8c61244a: sa_motion_publish_stop(ctx,0x8c61244au); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61244c; /* mov.l @r15,r3 */
L8c61244c: sa_motion_publish_stop(ctx,0x8c61244cu); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c61244e; /* mov.w @r3,r3 */
L8c61244e: sa_motion_publish_stop(ctx,0x8c61244eu); *fpul=r[3];goto L8c612450; /* lds r3,FPUL */
L8c612450: sa_motion_publish_stop(ctx,0x8c612450u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612452; /* float FPUL,fr3 */
L8c612452: sa_motion_publish_stop(ctx,0x8c612452u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612454; /* fmul fr4,fr3 */
L8c612454: sa_motion_publish_stop(ctx,0x8c612454u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612456; /* fmov.s fr3,@-r8 */
L8c612456: sa_motion_publish_stop(ctx,0x8c612456u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612458; /* fmov.s @(r0,r6),fr3 */
L8c612458: sa_motion_publish_stop(ctx,0x8c612458u); r[0]=20u;goto L8c61245a; /* mov #0x14,r0 */
L8c61245a: sa_motion_publish_stop(ctx,0x8c61245au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61245c; /* fmov.s fr3,@-r8 */
L8c61245c: sa_motion_publish_stop(ctx,0x8c61245cu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61245e; /* fmov.s @(r0,r6),fr3 */
L8c61245e: sa_motion_publish_stop(ctx,0x8c61245eu); r[0]=16u;goto L8c612460; /* mov #0x10,r0 */
L8c612460: sa_motion_publish_stop(ctx,0x8c612460u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612462; /* fmov.s fr3,@-r8 */
L8c612462: sa_motion_publish_stop(ctx,0x8c612462u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612464; /* fmov.s @(r0,r6),fr3 */
L8c612464: sa_motion_publish_stop(ctx,0x8c612464u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612466; /* fmov.s fr3,@-r8 */
L8c612466: sa_motion_publish_stop(ctx,0x8c612466u); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612468; /* mov.l r4,@-r8 */
L8c612468: sa_motion_publish_stop(ctx,0x8c612468u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61246a; /* pref @r8 */
L8c61246a: sa_motion_publish_stop(ctx,0x8c61246au); r[4]=r[13];goto L8c61246c; /* mov r13,r4 */
L8c61246c: sa_motion_publish_stop(ctx,0x8c61246cu); r[0]=28u;goto L8c61246e; /* mov #0x1c,r0 */
L8c61246e: sa_motion_publish_stop(ctx,0x8c61246eu); r[6]=r[5];goto L8c612470; /* mov r5,r6 */
L8c612470: sa_motion_publish_stop(ctx,0x8c612470u); r[4]+=64u;goto L8c612472; /* add #0x40,r4 */
L8c612472: sa_motion_publish_stop(ctx,0x8c612472u); r[6]+=8u;goto L8c612474; /* add #0x8,r6 */
L8c612474: sa_motion_publish_stop(ctx,0x8c612474u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612476; /* fmov.s @(r0,r4),fr3 */
L8c612476: sa_motion_publish_stop(ctx,0x8c612476u); r[8]+=60u;goto L8c612478; /* add #0x3c,r8 */
L8c612478: sa_motion_publish_stop(ctx,0x8c612478u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61247a; /* fmov.s fr3,@-r8 */
L8c61247a: sa_motion_publish_stop(ctx,0x8c61247au); r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c61247c; /* mov.w @(0x2,r6),r0 */
L8c61247c: sa_motion_publish_stop(ctx,0x8c61247cu); r[3]=r[0];goto L8c61247e; /* mov r0,r3 */
L8c61247e: sa_motion_publish_stop(ctx,0x8c61247eu); *fpul=r[3];goto L8c612480; /* lds r3,FPUL */
L8c612480: sa_motion_publish_stop(ctx,0x8c612480u); r[0]=24u;goto L8c612482; /* mov #0x18,r0 */
L8c612482: sa_motion_publish_stop(ctx,0x8c612482u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612484; /* float FPUL,fr3 */
L8c612484: sa_motion_publish_stop(ctx,0x8c612484u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612486; /* fmul fr4,fr3 */
L8c612486: sa_motion_publish_stop(ctx,0x8c612486u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612488; /* fmov.s fr3,@-r8 */
L8c612488: sa_motion_publish_stop(ctx,0x8c612488u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c61248a; /* mov.w @r6,r3 */
L8c61248a: sa_motion_publish_stop(ctx,0x8c61248au); *fpul=r[3];goto L8c61248c; /* lds r3,FPUL */
L8c61248c: sa_motion_publish_stop(ctx,0x8c61248cu); r[3]=rd(m,bus,&io_status,0x8c612554u,4);if(io_status)return io_status;goto L8c61248e; /* mov.l 0x8c612554,r3 */
L8c61248e: sa_motion_publish_stop(ctx,0x8c61248eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612490; /* float FPUL,fr3 */
L8c612490: sa_motion_publish_stop(ctx,0x8c612490u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612492; /* fmul fr4,fr3 */
L8c612492: sa_motion_publish_stop(ctx,0x8c612492u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612494; /* fmov.s fr3,@-r8 */
L8c612494: sa_motion_publish_stop(ctx,0x8c612494u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612496; /* fmov.s @(r0,r4),fr3 */
L8c612496: sa_motion_publish_stop(ctx,0x8c612496u); r[0]=20u;goto L8c612498; /* mov #0x14,r0 */
L8c612498: sa_motion_publish_stop(ctx,0x8c612498u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61249a; /* fmov.s fr3,@-r8 */
L8c61249a: sa_motion_publish_stop(ctx,0x8c61249au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61249c; /* fmov.s @(r0,r4),fr3 */
L8c61249c: sa_motion_publish_stop(ctx,0x8c61249cu); r[0]=16u;goto L8c61249e; /* mov #0x10,r0 */
L8c61249e: sa_motion_publish_stop(ctx,0x8c61249eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6124a0; /* fmov.s fr3,@-r8 */
L8c6124a0: sa_motion_publish_stop(ctx,0x8c6124a0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6124a2; /* fmov.s @(r0,r4),fr3 */
L8c6124a2: sa_motion_publish_stop(ctx,0x8c6124a2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6124a4; /* fmov.s fr3,@-r8 */
L8c6124a4: sa_motion_publish_stop(ctx,0x8c6124a4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6124a6; /* mov.l r3,@-r8 */
L8c6124a6: sa_motion_publish_stop(ctx,0x8c6124a6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6124a8; /* pref @r8 */
L8c6124a8: sa_motion_publish_stop(ctx,0x8c6124a8u); r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c6124aa; /* mov.l @r7,r2 */
L8c6124aa: sa_motion_publish_stop(ctx,0x8c6124aau); r[8]+=32u;goto L8c6124ac; /* add #0x20,r8 */
L8c6124ac: sa_motion_publish_stop(ctx,0x8c6124acu); r[2]+=4u;goto L8c6124ae; /* add #0x4,r2 */
L8c6124ae: sa_motion_publish_stop(ctx,0x8c6124aeu); wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c6124b0; /* mov.l r2,@r7 */
L8c6124b0: sa_motion_publish_stop(ctx,0x8c6124b0u); r[15]+=84u;goto L8c6124b2; /* add #0x54,r15 */
L8c6124b2: sa_motion_publish_stop(ctx,0x8c6124b2u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6124b4; /* mov.l @r15+,r9 */
L8c6124b4: sa_motion_publish_stop(ctx,0x8c6124b4u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6124b6; /* mov.l @r15+,r10 */
L8c6124b6: sa_motion_publish_stop(ctx,0x8c6124b6u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6124b8; /* mov.l @r15+,r11 */
L8c6124b8: sa_motion_publish_stop(ctx,0x8c6124b8u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6124ba; /* mov.l @r15+,r12 */
L8c6124ba: sa_motion_publish_stop(ctx,0x8c6124bau); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6124bc; /* mov.l @r15+,r13 */
L8c6124bc: sa_motion_publish_stop(ctx,0x8c6124beu); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
