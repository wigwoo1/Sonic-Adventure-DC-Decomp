#include "sa_draw_motion_stop.h"
/* UV/face-factor triangle near-plane clipper with portable queue interface; name inferred.
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
SA_STAGE_EXPORT int sa_uv_face_triangle_near_clip_8c616f18(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c616f18;
L8c616f18: sa_motion_publish_stop(ctx,0x8c616f18u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c616f1a; /* mov.l r14,@-r15 */
L8c616f1a: sa_motion_publish_stop(ctx,0x8c616f1au); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c616f1c; /* mov.l r13,@-r15 */
L8c616f1c: sa_motion_publish_stop(ctx,0x8c616f1cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c616f1e; /* mov.l r12,@-r15 */
L8c616f1e: sa_motion_publish_stop(ctx,0x8c616f1eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c616f20; /* mov.l r11,@-r15 */
L8c616f20: sa_motion_publish_stop(ctx,0x8c616f20u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c616f22; /* mov.l r10,@-r15 */
L8c616f22: sa_motion_publish_stop(ctx,0x8c616f22u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c616f24; /* mov.l r9,@-r15 */
L8c616f24: sa_motion_publish_stop(ctx,0x8c616f24u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c616f26; /* fmov.s fr12,@-r15 */
L8c616f26: sa_motion_publish_stop(ctx,0x8c616f26u); r[15]+=4294967228u;goto L8c616f28; /* add #-0x44,r15 */
L8c616f28: sa_motion_publish_stop(ctx,0x8c616f28u); r[3]=rd(m,bus,&io_status,0x8c616fc4u,4);if(io_status)return io_status;goto L8c616f2a; /* mov.l 0x8c616fc4,r3 */
L8c616f2a: sa_motion_publish_stop(ctx,0x8c616f2au); r[14]=r[15];goto L8c616f2c; /* mov r15,r14 */
L8c616f2c: sa_motion_publish_stop(ctx,0x8c616f2cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c616f2e; /* fldi1 fr7 */
L8c616f2e: sa_motion_publish_stop(ctx,0x8c616f2eu); r[14]+=56u;goto L8c616f30; /* add #0x38,r14 */
L8c616f30: sa_motion_publish_stop(ctx,0x8c616f30u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c616f32; /* fmov.s @r3,fr3 */
L8c616f32: sa_motion_publish_stop(ctx,0x8c616f32u); r[9]=r[15];goto L8c616f34; /* mov r15,r9 */
L8c616f34: sa_motion_publish_stop(ctx,0x8c616f34u); fcopy(f,6u,7u);goto L8c616f36; /* fmov fr7,fr6 */
L8c616f36: sa_motion_publish_stop(ctx,0x8c616f36u); r[2]=r[14];goto L8c616f38; /* mov r14,r2 */
L8c616f38: sa_motion_publish_stop(ctx,0x8c616f38u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[3],&f->fpscr,&fr[6]);if(status)return status;}goto L8c616f3a; /* fdiv fr3,fr6 */
L8c616f3a: sa_motion_publish_stop(ctx,0x8c616f3au); r[12]=rd(m,bus,&io_status,0x8c616fc8u,4);if(io_status)return io_status;goto L8c616f3c; /* mov.l 0x8c616fc8,r12 */
L8c616f3c: sa_motion_publish_stop(ctx,0x8c616f3cu); r[9]+=20u;goto L8c616f3e; /* add #0x14,r9 */
L8c616f3e: sa_motion_publish_stop(ctx,0x8c616f3eu); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c616f40; /* mov.l r2,@r15 */
L8c616f40: sa_motion_publish_stop(ctx,0x8c616f40u); wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c616f42; /* mov.l r4,@r2 */
L8c616f42: sa_motion_publish_stop(ctx,0x8c616f42u); r[4]=1u;goto L8c616f44; /* mov #0x1,r4 */
L8c616f44: sa_motion_publish_stop(ctx,0x8c616f44u); wr(m,bus,&io_status,r[14]+0x4u,r[5],4);if(io_status)return io_status;goto L8c616f46; /* mov.l r5,@(0x4,r14) */
L8c616f46: sa_motion_publish_stop(ctx,0x8c616f46u); r[5]=r[9];goto L8c616f48; /* mov r9,r5 */
L8c616f48: sa_motion_publish_stop(ctx,0x8c616f48u); wr(m,bus,&io_status,r[14]+0x8u,r[6],4);if(io_status)return io_status;goto L8c616f4a; /* mov.l r6,@(0x8,r14) */
L8c616f4a: sa_motion_publish_stop(ctx,0x8c616f4au); r[11]=0u;goto L8c616f4c; /* mov #0x0,r11 */
L8c616f4c: sa_motion_publish_stop(ctx,0x8c616f4cu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616f4e; /* mov.l @r15,r13 */
L8c616f4e: sa_motion_publish_stop(ctx,0x8c616f4eu); r[10]=r[9];goto L8c616f50; /* mov r9,r10 */
L8c616f50: sa_motion_publish_stop(ctx,0x8c616f50u); r[6]=r[9];goto L8c616f52; /* mov r9,r6 */
L8c616f52: sa_motion_publish_stop(ctx,0x8c616f52u); r[0]=r[11];goto L8c616f54; /* mov r11,r0 */
L8c616f54: sa_motion_publish_stop(ctx,0x8c616f54u); r[0]<<=2;goto L8c616f56; /* shll2 r0 */
L8c616f56: sa_motion_publish_stop(ctx,0x8c616f56u); r[3]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c616f58; /* mov.l @(r0,r14),r3 */
L8c616f58: sa_motion_publish_stop(ctx,0x8c616f58u); r[2]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c616f5a; /* mov.l @(0xc,r3),r2 */
L8c616f5a: sa_motion_publish_stop(ctx,0x8c616f5au); *sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c616f5c; /* tst r4,r2 */
L8c616f5c: sa_motion_publish_stop(ctx,0x8c616f5cu); if(!(*sr&1u))goto L8c616fd4;goto L8c616f5e; /* bf 0x8c616fd4 */
L8c616f5e: sa_motion_publish_stop(ctx,0x8c616f5eu); r[1]=r[11];goto L8c616f60; /* mov r11,r1 */
L8c616f60: sa_motion_publish_stop(ctx,0x8c616f60u); r[1]<<=2;goto L8c616f62; /* shll2 r1 */
L8c616f62: sa_motion_publish_stop(ctx,0x8c616f62u); r[1]+=r[14];goto L8c616f64; /* add r14,r1 */
L8c616f64: sa_motion_publish_stop(ctx,0x8c616f64u); fcopy(f,5u,7u);goto L8c616f66; /* fmov fr7,fr5 */
L8c616f66: sa_motion_publish_stop(ctx,0x8c616f66u); r[0]=24u;goto L8c616f68; /* mov #0x18,r0 */
L8c616f68: sa_motion_publish_stop(ctx,0x8c616f68u); wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c616f6a; /* mov.l r1,@(0x4,r15) */
L8c616f6a: sa_motion_publish_stop(ctx,0x8c616f6au); r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c616f6c; /* mov.l @r1,r3 */
L8c616f6c: sa_motion_publish_stop(ctx,0x8c616f6cu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c616f6e; /* mov.l r3,@r15 */
L8c616f6e: sa_motion_publish_stop(ctx,0x8c616f6eu); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616f70; /* fmov.s @(r0,r3),fr3 */
L8c616f70: sa_motion_publish_stop(ctx,0x8c616f70u); r[0]=12u;goto L8c616f72; /* mov #0xc,r0 */
L8c616f72: sa_motion_publish_stop(ctx,0x8c616f72u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c616f74; /* fdiv fr3,fr5 */
L8c616f74: sa_motion_publish_stop(ctx,0x8c616f74u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c616f76; /* fmov.s @(r0,r12),fr3 */
L8c616f76: sa_motion_publish_stop(ctx,0x8c616f76u); r[0]=16u;goto L8c616f78; /* mov #0x10,r0 */
L8c616f78: sa_motion_publish_stop(ctx,0x8c616f78u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c616f7a; /* fmov.s @(r0,r3),fr2 */
L8c616f7a: sa_motion_publish_stop(ctx,0x8c616f7au); r[0]=16u;goto L8c616f7c; /* mov #0x10,r0 */
L8c616f7c: sa_motion_publish_stop(ctx,0x8c616f7cu); r[3]=rd(m,bus,&io_status,0x8c616fccu,4);if(io_status)return io_status;goto L8c616f7e; /* mov.l 0x8c616fcc,r3 */
L8c616f7e: sa_motion_publish_stop(ctx,0x8c616f7eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616f80; /* fsub fr3,fr2 */
L8c616f80: sa_motion_publish_stop(ctx,0x8c616f80u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c616f82; /* fmov.s @r3,fr3 */
L8c616f82: sa_motion_publish_stop(ctx,0x8c616f82u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616f84; /* fmul fr5,fr2 */
L8c616f84: sa_motion_publish_stop(ctx,0x8c616f84u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616f86; /* fdiv fr3,fr2 */
L8c616f86: sa_motion_publish_stop(ctx,0x8c616f86u); fstore(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c616f88; /* fmov.s fr2,@r5 */
L8c616f88: sa_motion_publish_stop(ctx,0x8c616f88u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616f8a; /* mov.l @(0x4,r15),r2 */
L8c616f8a: sa_motion_publish_stop(ctx,0x8c616f8au); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c616f8c; /* fmov.s @(r0,r12),fr3 */
L8c616f8c: sa_motion_publish_stop(ctx,0x8c616f8cu); r[0]=20u;goto L8c616f8e; /* mov #0x14,r0 */
L8c616f8e: sa_motion_publish_stop(ctx,0x8c616f8eu); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c616f90; /* mov.l @r2,r1 */
L8c616f90: sa_motion_publish_stop(ctx,0x8c616f90u); r[2]=rd(m,bus,&io_status,0x8c616fd0u,4);if(io_status)return io_status;goto L8c616f92; /* mov.l 0x8c616fd0,r2 */
L8c616f92: sa_motion_publish_stop(ctx,0x8c616f92u); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c616f94; /* fmov.s @(r0,r1),fr2 */
L8c616f94: sa_motion_publish_stop(ctx,0x8c616f94u); r[0]=4u;goto L8c616f96; /* mov #0x4,r0 */
L8c616f96: sa_motion_publish_stop(ctx,0x8c616f96u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616f98; /* fsub fr3,fr2 */
L8c616f98: sa_motion_publish_stop(ctx,0x8c616f98u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c616f9a; /* fmov.s @r2,fr3 */
L8c616f9a: sa_motion_publish_stop(ctx,0x8c616f9au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616f9c; /* fmul fr5,fr2 */
L8c616f9c: sa_motion_publish_stop(ctx,0x8c616f9cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616f9e; /* fdiv fr3,fr2 */
L8c616f9e: sa_motion_publish_stop(ctx,0x8c616f9eu); fstore(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c616fa0; /* fmov fr2,@(r0,r5) */
L8c616fa0: sa_motion_publish_stop(ctx,0x8c616fa0u); r[0]=8u;goto L8c616fa2; /* mov #0x8,r0 */
L8c616fa2: sa_motion_publish_stop(ctx,0x8c616fa4u); fstore(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c616ff0; /* bra 0x8c616ff0 */
L8c616fd4: sa_motion_publish_stop(ctx,0x8c616fd4u); r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c616fd6; /* mov.l @r13,r3 */
L8c616fd6: sa_motion_publish_stop(ctx,0x8c616fd6u); r[0]=16u;goto L8c616fd8; /* mov #0x10,r0 */
L8c616fd8: sa_motion_publish_stop(ctx,0x8c616fd8u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616fda; /* fmov.s @(r0,r3),fr3 */
L8c616fda: sa_motion_publish_stop(ctx,0x8c616fdau); r[0]=20u;goto L8c616fdc; /* mov #0x14,r0 */
L8c616fdc: sa_motion_publish_stop(ctx,0x8c616fdcu); fstore(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c616fde; /* fmov.s fr3,@r6 */
L8c616fde: sa_motion_publish_stop(ctx,0x8c616fdeu); r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c616fe0; /* mov.l @r13,r3 */
L8c616fe0: sa_motion_publish_stop(ctx,0x8c616fe0u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616fe2; /* fmov.s @(r0,r3),fr3 */
L8c616fe2: sa_motion_publish_stop(ctx,0x8c616fe2u); r[0]=4u;goto L8c616fe4; /* mov #0x4,r0 */
L8c616fe4: sa_motion_publish_stop(ctx,0x8c616fe4u); fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c616fe6; /* fmov fr3,@(r0,r6) */
L8c616fe6: sa_motion_publish_stop(ctx,0x8c616fe6u); r[0]=24u;goto L8c616fe8; /* mov #0x18,r0 */
L8c616fe8: sa_motion_publish_stop(ctx,0x8c616fe8u); r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c616fea; /* mov.l @r13,r3 */
L8c616fea: sa_motion_publish_stop(ctx,0x8c616feau); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616fec; /* fmov.s @(r0,r3),fr3 */
L8c616fec: sa_motion_publish_stop(ctx,0x8c616fecu); r[0]=8u;goto L8c616fee; /* mov #0x8,r0 */
L8c616fee: sa_motion_publish_stop(ctx,0x8c616feeu); fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c616ff0; /* fmov fr3,@(r0,r6) */
L8c616ff0: sa_motion_publish_stop(ctx,0x8c616ff0u); r[3]=3u;goto L8c616ff2; /* mov #0x3,r3 */
L8c616ff2: sa_motion_publish_stop(ctx,0x8c616ff2u); r[11]+=1u;goto L8c616ff4; /* add #0x1,r11 */
L8c616ff4: sa_motion_publish_stop(ctx,0x8c616ff4u); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[3]);goto L8c616ff6; /* cmp/ge r3,r11 */
L8c616ff6: sa_motion_publish_stop(ctx,0x8c616ff6u); r[13]+=4u;goto L8c616ff8; /* add #0x4,r13 */
L8c616ff8: sa_motion_publish_stop(ctx,0x8c616ff8u); r[6]+=12u;goto L8c616ffa; /* add #0xc,r6 */
L8c616ffa: sa_motion_publish_stop(ctx,0x8c616ffcu); take=!(*sr&1u);r[5]+=12u;if(take)goto L8c616f52;goto L8c616ffe; /* bf/s 0x8c616f52 */
L8c616ffe: sa_motion_publish_stop(ctx,0x8c616ffeu); r[0]=8u;goto L8c617000; /* mov #0x8,r0 */
L8c617000: sa_motion_publish_stop(ctx,0x8c617000u); r[6]=r[9];goto L8c617002; /* mov r9,r6 */
L8c617002: sa_motion_publish_stop(ctx,0x8c617002u); fload(m,bus,&io_status,f,8u,r[0]+r[10]);if(io_status)return io_status;goto L8c617004; /* fmov.s @(r0,r10),fr8 */
L8c617004: sa_motion_publish_stop(ctx,0x8c617004u); r[6]+=12u;goto L8c617006; /* add #0xc,r6 */
L8c617006: sa_motion_publish_stop(ctx,0x8c617006u); r[0]=4u;goto L8c617008; /* mov #0x4,r0 */
L8c617008: sa_motion_publish_stop(ctx,0x8c617008u); fload(m,bus,&io_status,f,11u,r[10]);if(io_status)return io_status;goto L8c61700a; /* fmov.s @r10,fr11 */
L8c61700a: sa_motion_publish_stop(ctx,0x8c61700au); fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c61700c; /* fmov.s @(r0,r6),fr5 */
L8c61700c: sa_motion_publish_stop(ctx,0x8c61700cu); r[5]=r[9];goto L8c61700e; /* mov r9,r5 */
L8c61700e: sa_motion_publish_stop(ctx,0x8c61700eu); fload(m,bus,&io_status,f,10u,r[0]+r[10]);if(io_status)return io_status;goto L8c617010; /* fmov.s @(r0,r10),fr10 */
L8c617010: sa_motion_publish_stop(ctx,0x8c617010u); r[0]=8u;goto L8c617012; /* mov #0x8,r0 */
L8c617012: sa_motion_publish_stop(ctx,0x8c617012u); fload(m,bus,&io_status,f,7u,r[0]+r[6]);if(io_status)return io_status;goto L8c617014; /* fmov.s @(r0,r6),fr7 */
L8c617014: sa_motion_publish_stop(ctx,0x8c617014u); r[0]=4u;goto L8c617016; /* mov #0x4,r0 */
L8c617016: sa_motion_publish_stop(ctx,0x8c617016u); fcopy(f,3u,10u);goto L8c617018; /* fmov fr10,fr3 */
L8c617018: sa_motion_publish_stop(ctx,0x8c617018u); r[5]+=24u;goto L8c61701a; /* add #0x18,r5 */
L8c61701a: sa_motion_publish_stop(ctx,0x8c61701au); fcopy(f,0u,8u);goto L8c61701c; /* fmov fr8,fr0 */
L8c61701c: sa_motion_publish_stop(ctx,0x8c61701cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61701e; /* fmul fr5,fr0 */
L8c61701e: sa_motion_publish_stop(ctx,0x8c61701eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617020; /* fmul fr7,fr3 */
L8c617020: sa_motion_publish_stop(ctx,0x8c617020u); fload(m,bus,&io_status,f,9u,r[6]);if(io_status)return io_status;goto L8c617022; /* fmov.s @r6,fr9 */
L8c617022: sa_motion_publish_stop(ctx,0x8c617022u); fcopy(f,2u,11u);goto L8c617024; /* fmov fr11,fr2 */
L8c617024: sa_motion_publish_stop(ctx,0x8c617024u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617026; /* fmul fr7,fr2 */
L8c617026: sa_motion_publish_stop(ctx,0x8c617026u); fcopy(f,12u,11u);goto L8c617028; /* fmov fr11,fr12 */
L8c617028: sa_motion_publish_stop(ctx,0x8c617028u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[5],&f->fpscr,&fr[12]);if(status)return status;}goto L8c61702a; /* fmul fr5,fr12 */
L8c61702a: sa_motion_publish_stop(ctx,0x8c61702au); fcopy(f,1u,9u);goto L8c61702c; /* fmov fr9,fr1 */
L8c61702c: sa_motion_publish_stop(ctx,0x8c61702cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[10],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61702e; /* fmul fr10,fr1 */
L8c61702e: sa_motion_publish_stop(ctx,0x8c61702eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c617030; /* fsub fr3,fr0 */
L8c617030: sa_motion_publish_stop(ctx,0x8c617030u); fcopy(f,3u,8u);goto L8c617032; /* fmov fr8,fr3 */
L8c617032: sa_motion_publish_stop(ctx,0x8c617032u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617034; /* fmul fr9,fr3 */
L8c617034: sa_motion_publish_stop(ctx,0x8c617034u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c617036; /* fsub fr3,fr2 */
L8c617036: sa_motion_publish_stop(ctx,0x8c617036u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617038; /* fmov.s @(r0,r5),fr3 */
L8c617038: sa_motion_publish_stop(ctx,0x8c617038u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61703a; /* fmul fr2,fr3 */
L8c61703a: sa_motion_publish_stop(ctx,0x8c61703au); fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c61703c; /* fmov.s @r5,fr2 */
L8c61703c: sa_motion_publish_stop(ctx,0x8c61703cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61703e; /* fmac fr0,fr2,fr3 */
L8c61703e: sa_motion_publish_stop(ctx,0x8c61703eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[12],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c617040; /* fsub fr12,fr1 */
L8c617040: sa_motion_publish_stop(ctx,0x8c617040u); r[0]=8u;goto L8c617042; /* mov #0x8,r0 */
L8c617042: sa_motion_publish_stop(ctx,0x8c617042u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c617044; /* fmov.s @(r0,r5),fr2 */
L8c617044: sa_motion_publish_stop(ctx,0x8c617044u); r[5]=rd(m,bus,&io_status,0x8c617294u,4);if(io_status)return io_status;goto L8c617046; /* mov.l 0x8c617294,r5 */
L8c617046: sa_motion_publish_stop(ctx,0x8c617046u); fcopy(f,0u,1u);goto L8c617048; /* fmov fr1,fr0 */
L8c617048: sa_motion_publish_stop(ctx,0x8c617048u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61704a; /* fmac fr0,fr2,fr3 */
L8c61704a: sa_motion_publish_stop(ctx,0x8c61704au); r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c61704c; /* mov.l @r5,r3 */
L8c61704c: sa_motion_publish_stop(ctx,0x8c61704cu); fcopy(f,5u,3u);goto L8c61704e; /* fmov fr3,fr5 */
L8c61704e: sa_motion_publish_stop(ctx,0x8c61704eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c617050; /* fldi0 fr3 */
L8c617050: sa_motion_publish_stop(ctx,0x8c617050u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c617052; /* fcmp/gt fr5,fr3 */
L8c617052: sa_motion_publish_stop(ctx,0x8c617052u); r[0]=*sr&1u;goto L8c617054; /* movt r0 */
L8c617054: sa_motion_publish_stop(ctx,0x8c617054u); *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c617056; /* tst r3,r0 */
L8c617056: sa_motion_publish_stop(ctx,0x8c617056u); if((*sr&1u))goto L8c61705c;goto L8c617058; /* bt 0x8c61705c */
L8c617058: sa_motion_publish_stop(ctx,0x8c61705au); goto L8c6173a8; /* bra 0x8c6173a8 */
L8c61705c: sa_motion_publish_stop(ctx,0x8c61705cu); r[1]=0u;goto L8c61705e; /* mov #0x0,r1 */
L8c61705e: sa_motion_publish_stop(ctx,0x8c61705eu); r[6]=rd(m,bus,&io_status,0x8c617298u,4);if(io_status)return io_status;goto L8c617060; /* mov.l 0x8c617298,r6 */
L8c617060: sa_motion_publish_stop(ctx,0x8c617060u); wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c617062; /* mov.l r1,@r5 */
L8c617062: sa_motion_publish_stop(ctx,0x8c617062u); r[3]=2u;goto L8c617064; /* mov #0x2,r3 */
L8c617064: sa_motion_publish_stop(ctx,0x8c617064u); r[5]=rd(m,bus,&io_status,0x8c61729cu,4);if(io_status)return io_status;goto L8c617066; /* mov.l 0x8c61729c,r5 */
L8c617066: sa_motion_publish_stop(ctx,0x8c617066u); r[13]=r[1];goto L8c617068; /* mov r1,r13 */
L8c617068: sa_motion_publish_stop(ctx,0x8c617068u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61706a; /* mov.l r3,@r15 */
L8c61706a: sa_motion_publish_stop(ctx,0x8c61706au); r[10]=r[13];goto L8c61706c; /* mov r13,r10 */
L8c61706c: sa_motion_publish_stop(ctx,0x8c61706cu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61706e; /* mov.l @r15,r3 */
L8c61706e: sa_motion_publish_stop(ctx,0x8c61706eu); r[0]=r[14];goto L8c617070; /* mov r14,r0 */
L8c617070: sa_motion_publish_stop(ctx,0x8c617070u); r[11]=r[10];goto L8c617072; /* mov r10,r11 */
L8c617072: sa_motion_publish_stop(ctx,0x8c617072u); r[11]<<=2;goto L8c617074; /* shll2 r11 */
L8c617074: sa_motion_publish_stop(ctx,0x8c617074u); r[3]<<=2;goto L8c617076; /* shll2 r3 */
L8c617076: sa_motion_publish_stop(ctx,0x8c617076u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c617078; /* mov.l r3,@(0x4,r15) */
L8c617078: sa_motion_publish_stop(ctx,0x8c617078u); r[2]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c61707a; /* mov.l @(r0,r3),r2 */
L8c61707a: sa_motion_publish_stop(ctx,0x8c61707au); r[3]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c61707c; /* mov.l @(0xc,r2),r3 */
L8c61707c: sa_motion_publish_stop(ctx,0x8c61707cu); r[2]=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;goto L8c61707e; /* mov.l @(r0,r11),r2 */
L8c61707e: sa_motion_publish_stop(ctx,0x8c61707eu); r[3]&=r[4];goto L8c617080; /* and r4,r3 */
L8c617080: sa_motion_publish_stop(ctx,0x8c617080u); if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[2]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c617082; /* mov.l @(0xc,r2),r2 */
L8c617082: sa_motion_publish_stop(ctx,0x8c617082u); r[2]&=r[4];goto L8c617084; /* and r4,r2 */
L8c617084: sa_motion_publish_stop(ctx,0x8c617084u); r[3]^=r[2];goto L8c617086; /* xor r2,r3 */
L8c617086: sa_motion_publish_stop(ctx,0x8c617086u); r[0]=r[3];goto L8c617088; /* mov r3,r0 */
L8c617088: sa_motion_publish_stop(ctx,0x8c617088u); *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c61708a; /* cmp/eq #0x1,r0 */
L8c61708a: sa_motion_publish_stop(ctx,0x8c61708au); if(!(*sr&1u))goto L8c617170;goto L8c61708c; /* bf 0x8c617170 */
L8c61708c: sa_motion_publish_stop(ctx,0x8c61708cu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61708e; /* mov.l @r15,r2 */
L8c61708e: sa_motion_publish_stop(ctx,0x8c61708eu); r[0]=8u;goto L8c617090; /* mov #0x8,r0 */
L8c617090: sa_motion_publish_stop(ctx,0x8c617090u); r[1]=r[10];goto L8c617092; /* mov r10,r1 */
L8c617092: sa_motion_publish_stop(ctx,0x8c617092u); r[3]=r[2];goto L8c617094; /* mov r2,r3 */
L8c617094: sa_motion_publish_stop(ctx,0x8c617094u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c617096; /* shll r2 */
L8c617096: sa_motion_publish_stop(ctx,0x8c617096u); r[2]+=r[3];goto L8c617098; /* add r3,r2 */
L8c617098: sa_motion_publish_stop(ctx,0x8c617098u); r[3]=rd(m,bus,&io_status,0x8c6172a0u,4);if(io_status)return io_status;goto L8c61709a; /* mov.l 0x8c6172a0,r3 */
L8c61709a: sa_motion_publish_stop(ctx,0x8c61709au); r[2]<<=2;goto L8c61709c; /* shll2 r2 */
L8c61709c: sa_motion_publish_stop(ctx,0x8c61709cu); r[2]+=r[9];goto L8c61709e; /* add r9,r2 */
L8c61709e: sa_motion_publish_stop(ctx,0x8c61709eu); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6170a0; /* mov.l r2,@(0x8,r15) */
L8c6170a0: sa_motion_publish_stop(ctx,0x8c6170a0u); fload(m,bus,&io_status,f,5u,r[0]+r[2]);if(io_status)return io_status;goto L8c6170a2; /* fmov.s @(r0,r2),fr5 */
L8c6170a2: sa_motion_publish_stop(ctx,0x8c6170a2u); r[2]=r[10];goto L8c6170a4; /* mov r10,r2 */
L8c6170a4: sa_motion_publish_stop(ctx,0x8c6170a4u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6170a6; /* shll r2 */
L8c6170a6: sa_motion_publish_stop(ctx,0x8c6170a6u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6170a8; /* fmov.s @r3,fr3 */
L8c6170a8: sa_motion_publish_stop(ctx,0x8c6170a8u); r[2]+=r[1];goto L8c6170aa; /* add r1,r2 */
L8c6170aa: sa_motion_publish_stop(ctx,0x8c6170aau); r[2]<<=2;goto L8c6170ac; /* shll2 r2 */
L8c6170ac: sa_motion_publish_stop(ctx,0x8c6170acu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6170ae; /* fsub fr5,fr3 */
L8c6170ae: sa_motion_publish_stop(ctx,0x8c6170aeu); r[0]=8u;goto L8c6170b0; /* mov #0x8,r0 */
L8c6170b0: sa_motion_publish_stop(ctx,0x8c6170b0u); r[2]+=r[9];goto L8c6170b2; /* add r9,r2 */
L8c6170b2: sa_motion_publish_stop(ctx,0x8c6170b2u); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6170b4; /* mov.l r2,@(0xc,r15) */
L8c6170b4: sa_motion_publish_stop(ctx,0x8c6170b4u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6170b6; /* fmov.s @(r0,r2),fr2 */
L8c6170b6: sa_motion_publish_stop(ctx,0x8c6170b6u); r[1]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6170b8; /* mov.l @(0xc,r15),r1 */
L8c6170b8: sa_motion_publish_stop(ctx,0x8c6170b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6170ba; /* fsub fr5,fr2 */
L8c6170ba: sa_motion_publish_stop(ctx,0x8c6170bau); fcopy(f,5u,3u);goto L8c6170bc; /* fmov fr3,fr5 */
L8c6170bc: sa_motion_publish_stop(ctx,0x8c6170bcu); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6170be; /* mov.l @(0x8,r15),r2 */
L8c6170be: sa_motion_publish_stop(ctx,0x8c6170beu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6170c0; /* fmov.s @r1,fr3 */
L8c6170c0: sa_motion_publish_stop(ctx,0x8c6170c0u); fload(m,bus,&io_status,f,7u,r[2]);if(io_status)return io_status;goto L8c6170c2; /* fmov.s @r2,fr7 */
L8c6170c2: sa_motion_publish_stop(ctx,0x8c6170c2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[2],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6170c4; /* fdiv fr2,fr5 */
L8c6170c4: sa_motion_publish_stop(ctx,0x8c6170c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6170c6; /* fsub fr7,fr3 */
L8c6170c6: sa_motion_publish_stop(ctx,0x8c6170c6u); fcopy(f,2u,7u);goto L8c6170c8; /* fmov fr7,fr2 */
L8c6170c8: sa_motion_publish_stop(ctx,0x8c6170c8u); fcopy(f,0u,5u);goto L8c6170ca; /* fmov fr5,fr0 */
L8c6170ca: sa_motion_publish_stop(ctx,0x8c6170cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6170cc; /* fmac fr0,fr3,fr2 */
L8c6170cc: sa_motion_publish_stop(ctx,0x8c6170ccu); r[1]=rd(m,bus,&io_status,0x8c6172a4u,4);if(io_status)return io_status;goto L8c6170ce; /* mov.l 0x8c6172a4,r1 */
L8c6170ce: sa_motion_publish_stop(ctx,0x8c6170ceu); r[0]=12u;goto L8c6170d0; /* mov #0xc,r0 */
L8c6170d0: sa_motion_publish_stop(ctx,0x8c6170d0u); fcopy(f,7u,2u);goto L8c6170d2; /* fmov fr2,fr7 */
L8c6170d2: sa_motion_publish_stop(ctx,0x8c6170d2u); r[2]=r[13];goto L8c6170d4; /* mov r13,r2 */
L8c6170d4: sa_motion_publish_stop(ctx,0x8c6170d4u); fcopy(f,0u,6u);goto L8c6170d6; /* fmov fr6,fr0 */
L8c6170d6: sa_motion_publish_stop(ctx,0x8c6170d6u); r[2]<<=2;goto L8c6170d8; /* shll2 r2 */
L8c6170d8: sa_motion_publish_stop(ctx,0x8c6170d8u); r[2]<<=2;goto L8c6170da; /* shll2 r2 */
L8c6170da: sa_motion_publish_stop(ctx,0x8c6170dau); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6170dc; /* shll r2 */
L8c6170dc: sa_motion_publish_stop(ctx,0x8c6170dcu); r[2]+=r[6];goto L8c6170de; /* add r6,r2 */
L8c6170de: sa_motion_publish_stop(ctx,0x8c6170deu); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6170e0; /* mov.l r2,@r15 */
L8c6170e0: sa_motion_publish_stop(ctx,0x8c6170e0u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6170e2; /* fmov.s @r1,fr3 */
L8c6170e2: sa_motion_publish_stop(ctx,0x8c6170e2u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6170e4; /* fmov.s @(r0,r12),fr2 */
L8c6170e4: sa_motion_publish_stop(ctx,0x8c6170e4u); r[0]=16u;goto L8c6170e6; /* mov #0x10,r0 */
L8c6170e6: sa_motion_publish_stop(ctx,0x8c6170e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6170e8; /* fmul fr7,fr3 */
L8c6170e8: sa_motion_publish_stop(ctx,0x8c6170e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6170ea; /* fmac fr0,fr3,fr2 */
L8c6170ea: sa_motion_publish_stop(ctx,0x8c6170eau); fcopy(f,0u,5u);goto L8c6170ec; /* fmov fr5,fr0 */
L8c6170ec: sa_motion_publish_stop(ctx,0x8c6170ecu); fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6170ee; /* fmov fr2,@(r0,r2) */
L8c6170ee: sa_motion_publish_stop(ctx,0x8c6170eeu); r[0]=4u;goto L8c6170f0; /* mov #0x4,r0 */
L8c6170f0: sa_motion_publish_stop(ctx,0x8c6170f0u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6170f2; /* mov.l @(0x8,r15),r2 */
L8c6170f2: sa_motion_publish_stop(ctx,0x8c6170f2u); fload(m,bus,&io_status,f,7u,r[0]+r[2]);if(io_status)return io_status;goto L8c6170f4; /* fmov.s @(r0,r2),fr7 */
L8c6170f4: sa_motion_publish_stop(ctx,0x8c6170f4u); r[0]=4u;goto L8c6170f6; /* mov #0x4,r0 */
L8c6170f6: sa_motion_publish_stop(ctx,0x8c6170f6u); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6170f8; /* mov.l @(0xc,r15),r2 */
L8c6170f8: sa_motion_publish_stop(ctx,0x8c6170f8u); fcopy(f,2u,7u);goto L8c6170fa; /* fmov fr7,fr2 */
L8c6170fa: sa_motion_publish_stop(ctx,0x8c6170fau); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c6170fc; /* fmov.s @(r0,r2),fr3 */
L8c6170fc: sa_motion_publish_stop(ctx,0x8c6170fcu); r[0]=rd(m,bus,&io_status,0x8c6172a8u,4);if(io_status)return io_status;goto L8c6170fe; /* mov.l 0x8c6172a8,r0 */
L8c6170fe: sa_motion_publish_stop(ctx,0x8c6170feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617100; /* fsub fr7,fr3 */
L8c617100: sa_motion_publish_stop(ctx,0x8c617100u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617102; /* mov.l @r15,r2 */
L8c617102: sa_motion_publish_stop(ctx,0x8c617102u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617104; /* fmac fr0,fr3,fr2 */
L8c617104: sa_motion_publish_stop(ctx,0x8c617104u); fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c617106; /* fmov.s @r0,fr3 */
L8c617106: sa_motion_publish_stop(ctx,0x8c617106u); r[0]=16u;goto L8c617108; /* mov #0x10,r0 */
L8c617108: sa_motion_publish_stop(ctx,0x8c617108u); fcopy(f,7u,2u);goto L8c61710a; /* fmov fr2,fr7 */
L8c61710a: sa_motion_publish_stop(ctx,0x8c61710au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61710c; /* fmul fr7,fr3 */
L8c61710c: sa_motion_publish_stop(ctx,0x8c61710cu); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61710e; /* fmov.s @(r0,r12),fr2 */
L8c61710e: sa_motion_publish_stop(ctx,0x8c61710eu); r[0]=20u;goto L8c617110; /* mov #0x14,r0 */
L8c617110: sa_motion_publish_stop(ctx,0x8c617110u); fcopy(f,0u,6u);goto L8c617112; /* fmov fr6,fr0 */
L8c617112: sa_motion_publish_stop(ctx,0x8c617112u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617114; /* fmac fr0,fr3,fr2 */
L8c617114: sa_motion_publish_stop(ctx,0x8c617114u); fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c617116; /* fmov fr2,@(r0,r2) */
L8c617116: sa_motion_publish_stop(ctx,0x8c617116u); r[0]=24u;goto L8c617118; /* mov #0x18,r0 */
L8c617118: sa_motion_publish_stop(ctx,0x8c617118u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61711a; /* mov.l @r15,r3 */
L8c61711a: sa_motion_publish_stop(ctx,0x8c61711au); r[2]=r[11];goto L8c61711c; /* mov r11,r2 */
L8c61711c: sa_motion_publish_stop(ctx,0x8c61711cu); r[2]+=r[7];goto L8c61711e; /* add r7,r2 */
L8c61711e: sa_motion_publish_stop(ctx,0x8c61711eu); fstore(m,bus,&io_status,f,0u,r[0]+r[3]);if(io_status)return io_status;goto L8c617120; /* fmov fr0,@(r0,r3) */
L8c617120: sa_motion_publish_stop(ctx,0x8c617120u); r[3]=r[13];goto L8c617122; /* mov r13,r3 */
L8c617122: sa_motion_publish_stop(ctx,0x8c617122u); fcopy(f,0u,5u);goto L8c617124; /* fmov fr5,fr0 */
L8c617124: sa_motion_publish_stop(ctx,0x8c617124u); r[3]<<=2;goto L8c617126; /* shll2 r3 */
L8c617126: sa_motion_publish_stop(ctx,0x8c617126u); r[3]+=r[5];goto L8c617128; /* add r5,r3 */
L8c617128: sa_motion_publish_stop(ctx,0x8c617128u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c61712a; /* mov.l r3,@(0x8,r15) */
L8c61712a: sa_motion_publish_stop(ctx,0x8c61712au); r[0]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61712c; /* mov.l @(0x4,r15),r0 */
L8c61712c: sa_motion_publish_stop(ctx,0x8c61712cu); r[0]+=r[7];goto L8c61712e; /* add r7,r0 */
L8c61712e: sa_motion_publish_stop(ctx,0x8c61712eu); wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c617130; /* mov.l r0,@(0xc,r15) */
L8c617130: sa_motion_publish_stop(ctx,0x8c617130u); r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c617132; /* mov.w @r0,r0 */
L8c617132: sa_motion_publish_stop(ctx,0x8c617132u); wr(m,bus,&io_status,r[15]+0x10u,r[0],2);if(io_status)return io_status;goto L8c617134; /* mov.w r0,@(0x10,r15) */
L8c617134: sa_motion_publish_stop(ctx,0x8c617134u); *fpul=r[0];goto L8c617136; /* lds r0,FPUL */
L8c617136: sa_motion_publish_stop(ctx,0x8c617136u); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c617138; /* mov.l r2,@r15 */
L8c617138: sa_motion_publish_stop(ctx,0x8c617138u); r[0]=sx(m,bus,&io_status,r[15]+0x10u,2);if(io_status)return io_status;goto L8c61713a; /* mov.w @(0x10,r15),r0 */
L8c61713a: sa_motion_publish_stop(ctx,0x8c61713au); r[2]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c61713c; /* mov.w @r2,r2 */
L8c61713c: sa_motion_publish_stop(ctx,0x8c61713cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61713e; /* float FPUL,fr3 */
L8c61713e: sa_motion_publish_stop(ctx,0x8c61713eu); r[2]-=r[0];goto L8c617140; /* sub r0,r2 */
L8c617140: sa_motion_publish_stop(ctx,0x8c617140u); *fpul=r[2];goto L8c617142; /* lds r2,FPUL */
L8c617142: sa_motion_publish_stop(ctx,0x8c617142u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c617144; /* float FPUL,fr2 */
L8c617144: sa_motion_publish_stop(ctx,0x8c617144u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617146; /* fmac fr0,fr2,fr3 */
L8c617146: sa_motion_publish_stop(ctx,0x8c617146u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c617148; /* ftrc fr3,FPUL */
L8c617148: sa_motion_publish_stop(ctx,0x8c617148u); r[2]=*fpul;goto L8c61714a; /* sts FPUL,r2 */
L8c61714a: sa_motion_publish_stop(ctx,0x8c61714au); wr(m,bus,&io_status,r[3],r[2],2);if(io_status)return io_status;goto L8c61714c; /* mov.w r2,@r3 */
L8c61714c: sa_motion_publish_stop(ctx,0x8c61714cu); r[0]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61714e; /* mov.l @(0xc,r15),r0 */
L8c61714e: sa_motion_publish_stop(ctx,0x8c61714eu); r[13]+=1u;goto L8c617150; /* add #0x1,r13 */
L8c617150: sa_motion_publish_stop(ctx,0x8c617150u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c617152; /* mov.l @(0x8,r15),r3 */
L8c617152: sa_motion_publish_stop(ctx,0x8c617152u); r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c617154; /* mov.w @(0x2,r0),r0 */
L8c617154: sa_motion_publish_stop(ctx,0x8c617154u); *fpul=r[0];goto L8c617156; /* lds r0,FPUL */
L8c617156: sa_motion_publish_stop(ctx,0x8c617156u); wr(m,bus,&io_status,r[15]+0x4u,r[0],2);if(io_status)return io_status;goto L8c617158; /* mov.w r0,@(0x4,r15) */
L8c617158: sa_motion_publish_stop(ctx,0x8c617158u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61715a; /* mov.l @r15,r2 */
L8c61715a: sa_motion_publish_stop(ctx,0x8c61715au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61715c; /* float FPUL,fr3 */
L8c61715c: sa_motion_publish_stop(ctx,0x8c61715cu); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61715e; /* mov.w @(0x2,r2),r0 */
L8c61715e: sa_motion_publish_stop(ctx,0x8c61715eu); r[2]=r[0];goto L8c617160; /* mov r0,r2 */
L8c617160: sa_motion_publish_stop(ctx,0x8c617160u); r[0]=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;goto L8c617162; /* mov.w @(0x4,r15),r0 */
L8c617162: sa_motion_publish_stop(ctx,0x8c617162u); r[2]-=r[0];goto L8c617164; /* sub r0,r2 */
L8c617164: sa_motion_publish_stop(ctx,0x8c617164u); *fpul=r[2];goto L8c617166; /* lds r2,FPUL */
L8c617166: sa_motion_publish_stop(ctx,0x8c617166u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c617168; /* float FPUL,fr2 */
L8c617168: sa_motion_publish_stop(ctx,0x8c617168u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61716a; /* fmac fr0,fr2,fr3 */
L8c61716a: sa_motion_publish_stop(ctx,0x8c61716au); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c61716c; /* ftrc fr3,FPUL */
L8c61716c: sa_motion_publish_stop(ctx,0x8c61716cu); r[0]=*fpul;goto L8c61716e; /* sts FPUL,r0 */
L8c61716e: sa_motion_publish_stop(ctx,0x8c61716eu); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c617170; /* mov.w r0,@(0x2,r3) */
L8c617170: sa_motion_publish_stop(ctx,0x8c617170u); r[0]=r[11];goto L8c617172; /* mov r11,r0 */
L8c617172: sa_motion_publish_stop(ctx,0x8c617172u); r[3]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c617174; /* mov.l @(r0,r14),r3 */
L8c617174: sa_motion_publish_stop(ctx,0x8c617174u); r[3]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c617176; /* mov.l @(0xc,r3),r3 */
L8c617176: sa_motion_publish_stop(ctx,0x8c617176u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c617178; /* tst r4,r3 */
L8c617178: sa_motion_publish_stop(ctx,0x8c617178u); if(!(*sr&1u))goto L8c6171c8;goto L8c61717a; /* bf 0x8c6171c8 */
L8c61717a: sa_motion_publish_stop(ctx,0x8c61717au); r[3]=r[11];goto L8c61717c; /* mov r11,r3 */
L8c61717c: sa_motion_publish_stop(ctx,0x8c61717cu); r[2]=r[13];goto L8c61717e; /* mov r13,r2 */
L8c61717e: sa_motion_publish_stop(ctx,0x8c61717eu); r[3]+=r[14];goto L8c617180; /* add r14,r3 */
L8c617180: sa_motion_publish_stop(ctx,0x8c617180u); r[2]<<=2;goto L8c617182; /* shll2 r2 */
L8c617182: sa_motion_publish_stop(ctx,0x8c617182u); r[0]=16u;goto L8c617184; /* mov #0x10,r0 */
L8c617184: sa_motion_publish_stop(ctx,0x8c617184u); r[2]<<=2;goto L8c617186; /* shll2 r2 */
L8c617186: sa_motion_publish_stop(ctx,0x8c617186u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c617188; /* shll r2 */
L8c617188: sa_motion_publish_stop(ctx,0x8c617188u); r[2]+=r[6];goto L8c61718a; /* add r6,r2 */
L8c61718a: sa_motion_publish_stop(ctx,0x8c61718au); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61718c; /* mov.l r2,@r15 */
L8c61718c: sa_motion_publish_stop(ctx,0x8c61718cu); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61718e; /* mov.l r3,@(0x4,r15) */
L8c61718e: sa_motion_publish_stop(ctx,0x8c61718eu); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c617190; /* mov.l @r3,r1 */
L8c617190: sa_motion_publish_stop(ctx,0x8c617190u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c617192; /* fmov.s @(r0,r1),fr3 */
L8c617192: sa_motion_publish_stop(ctx,0x8c617192u); fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c617194; /* fmov fr3,@(r0,r2) */
L8c617194: sa_motion_publish_stop(ctx,0x8c617194u); r[0]=20u;goto L8c617196; /* mov #0x14,r0 */
L8c617196: sa_motion_publish_stop(ctx,0x8c617196u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c617198; /* mov.l @(0x4,r15),r2 */
L8c617198: sa_motion_publish_stop(ctx,0x8c617198u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61719a; /* mov.l @r15,r3 */
L8c61719a: sa_motion_publish_stop(ctx,0x8c61719au); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61719c; /* mov.l @r2,r1 */
L8c61719c: sa_motion_publish_stop(ctx,0x8c61719cu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61719e; /* fmov.s @(r0,r1),fr3 */
L8c61719e: sa_motion_publish_stop(ctx,0x8c61719eu); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6171a0; /* fmov fr3,@(r0,r3) */
L8c6171a0: sa_motion_publish_stop(ctx,0x8c6171a0u); r[0]=24u;goto L8c6171a2; /* mov #0x18,r0 */
L8c6171a2: sa_motion_publish_stop(ctx,0x8c6171a2u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6171a4; /* mov.l @(0x4,r15),r2 */
L8c6171a4: sa_motion_publish_stop(ctx,0x8c6171a4u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6171a6; /* mov.l @r15,r3 */
L8c6171a6: sa_motion_publish_stop(ctx,0x8c6171a6u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6171a8; /* mov.l @r2,r1 */
L8c6171a8: sa_motion_publish_stop(ctx,0x8c6171a8u); r[2]=r[11];goto L8c6171aa; /* mov r11,r2 */
L8c6171aa: sa_motion_publish_stop(ctx,0x8c6171aau); r[2]+=r[7];goto L8c6171ac; /* add r7,r2 */
L8c6171ac: sa_motion_publish_stop(ctx,0x8c6171acu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6171ae; /* fmov.s @(r0,r1),fr3 */
L8c6171ae: sa_motion_publish_stop(ctx,0x8c6171aeu); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6171b0; /* fmov fr3,@(r0,r3) */
L8c6171b0: sa_motion_publish_stop(ctx,0x8c6171b0u); r[3]=r[13];goto L8c6171b2; /* mov r13,r3 */
L8c6171b2: sa_motion_publish_stop(ctx,0x8c6171b2u); r[3]<<=2;goto L8c6171b4; /* shll2 r3 */
L8c6171b4: sa_motion_publish_stop(ctx,0x8c6171b4u); r[3]+=r[5];goto L8c6171b6; /* add r5,r3 */
L8c6171b6: sa_motion_publish_stop(ctx,0x8c6171b6u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6171b8; /* mov.l r3,@r15 */
L8c6171b8: sa_motion_publish_stop(ctx,0x8c6171b8u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6171ba; /* mov.l r2,@(0x4,r15) */
L8c6171ba: sa_motion_publish_stop(ctx,0x8c6171bau); r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c6171bc; /* mov.w @r2,r1 */
L8c6171bc: sa_motion_publish_stop(ctx,0x8c6171bcu); r[13]+=1u;goto L8c6171be; /* add #0x1,r13 */
L8c6171be: sa_motion_publish_stop(ctx,0x8c6171beu); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c6171c0; /* mov.w r1,@r3 */
L8c6171c0: sa_motion_publish_stop(ctx,0x8c6171c0u); r[0]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6171c2; /* mov.l @(0x4,r15),r0 */
L8c6171c2: sa_motion_publish_stop(ctx,0x8c6171c2u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6171c4; /* mov.l @r15,r3 */
L8c6171c4: sa_motion_publish_stop(ctx,0x8c6171c4u); r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c6171c6; /* mov.w @(0x2,r0),r0 */
L8c6171c6: sa_motion_publish_stop(ctx,0x8c6171c6u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6171c8; /* mov.w r0,@(0x2,r3) */
L8c6171c8: sa_motion_publish_stop(ctx,0x8c6171c8u); wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6171ca; /* mov.l r10,@r15 */
L8c6171ca: sa_motion_publish_stop(ctx,0x8c6171cau); r[3]=3u;goto L8c6171cc; /* mov #0x3,r3 */
L8c6171cc: sa_motion_publish_stop(ctx,0x8c6171ccu); r[10]+=1u;goto L8c6171ce; /* add #0x1,r10 */
L8c6171ce: sa_motion_publish_stop(ctx,0x8c6171ceu); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[3]);goto L8c6171d0; /* cmp/ge r3,r10 */
L8c6171d0: sa_motion_publish_stop(ctx,0x8c6171d0u); if((*sr&1u))goto L8c6171d6;goto L8c6171d2; /* bt 0x8c6171d6 */
L8c6171d2: sa_motion_publish_stop(ctx,0x8c6171d4u); goto L8c61706c; /* bra 0x8c61706c */
L8c6171d6: sa_motion_publish_stop(ctx,0x8c6171d6u); r[0]=0x8c6172b4u;goto L8c6171d8; /* mova 0x8c6172b4,r0 */
L8c6171d8: sa_motion_publish_stop(ctx,0x8c6171d8u); r[4]=rd(m,bus,&io_status,0x8c6172b0u,4);if(io_status)return io_status;goto L8c6171da; /* mov.l 0x8c6172b0,r4 */
L8c6171da: sa_motion_publish_stop(ctx,0x8c6171dau); fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c6171dc; /* fmov.s @r0,fr5 */
L8c6171dc: sa_motion_publish_stop(ctx,0x8c6171dcu); r[0]=r[13];goto L8c6171de; /* mov r13,r0 */
L8c6171de: sa_motion_publish_stop(ctx,0x8c6171deu); r[7]=rd(m,bus,&io_status,0x8c6172acu,4);if(io_status)return io_status;goto L8c6171e0; /* mov.l 0x8c6172ac,r7 */
L8c6171e0: sa_motion_publish_stop(ctx,0x8c6171e0u); *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c6171e2; /* cmp/eq #0x3,r0 */
L8c6171e2: sa_motion_publish_stop(ctx,0x8c6171e2u); if(!(*sr&1u))goto L8c6172bc;goto L8c6171e4; /* bf 0x8c6172bc */
L8c6171e4: sa_motion_publish_stop(ctx,0x8c6171e4u); r[3]=r[5];goto L8c6171e6; /* mov r5,r3 */
L8c6171e6: sa_motion_publish_stop(ctx,0x8c6171e6u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6171e8; /* mov.l r3,@r15 */
L8c6171e8: sa_motion_publish_stop(ctx,0x8c6171e8u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6171ea; /* mov.w @(0x2,r3),r0 */
L8c6171ea: sa_motion_publish_stop(ctx,0x8c6171eau); r[8]+=24u;goto L8c6171ec; /* add #0x18,r8 */
L8c6171ec: sa_motion_publish_stop(ctx,0x8c6171ecu); r[14]=r[6];goto L8c6171ee; /* mov r6,r14 */
L8c6171ee: sa_motion_publish_stop(ctx,0x8c6171eeu); r[3]=r[0];goto L8c6171f0; /* mov r0,r3 */
L8c6171f0: sa_motion_publish_stop(ctx,0x8c6171f0u); *fpul=r[3];goto L8c6171f2; /* lds r3,FPUL */
L8c6171f2: sa_motion_publish_stop(ctx,0x8c6171f2u); r[0]=24u;goto L8c6171f4; /* mov #0x18,r0 */
L8c6171f4: sa_motion_publish_stop(ctx,0x8c6171f4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6171f6; /* float FPUL,fr3 */
L8c6171f6: sa_motion_publish_stop(ctx,0x8c6171f6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6171f8; /* fmul fr5,fr3 */
L8c6171f8: sa_motion_publish_stop(ctx,0x8c6171f8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6171fa; /* fmov.s fr3,@-r8 */
L8c6171fa: sa_motion_publish_stop(ctx,0x8c6171fau); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6171fc; /* mov.l @r15,r3 */
L8c6171fc: sa_motion_publish_stop(ctx,0x8c6171fcu); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6171fe; /* mov.w @r3,r3 */
L8c6171fe: sa_motion_publish_stop(ctx,0x8c6171feu); *fpul=r[3];goto L8c617200; /* lds r3,FPUL */
L8c617200: sa_motion_publish_stop(ctx,0x8c617200u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617202; /* float FPUL,fr3 */
L8c617202: sa_motion_publish_stop(ctx,0x8c617202u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617204; /* fmul fr5,fr3 */
L8c617204: sa_motion_publish_stop(ctx,0x8c617204u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617206; /* fmov.s fr3,@-r8 */
L8c617206: sa_motion_publish_stop(ctx,0x8c617206u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617208; /* fmov.s @(r0,r14),fr3 */
L8c617208: sa_motion_publish_stop(ctx,0x8c617208u); r[0]=20u;goto L8c61720a; /* mov #0x14,r0 */
L8c61720a: sa_motion_publish_stop(ctx,0x8c61720au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61720c; /* fmov.s fr3,@-r8 */
L8c61720c: sa_motion_publish_stop(ctx,0x8c61720cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61720e; /* fmov.s @(r0,r14),fr3 */
L8c61720e: sa_motion_publish_stop(ctx,0x8c61720eu); r[0]=16u;goto L8c617210; /* mov #0x10,r0 */
L8c617210: sa_motion_publish_stop(ctx,0x8c617210u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617212; /* fmov.s fr3,@-r8 */
L8c617212: sa_motion_publish_stop(ctx,0x8c617212u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617214; /* fmov.s @(r0,r14),fr3 */
L8c617214: sa_motion_publish_stop(ctx,0x8c617214u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617216; /* fmov.s fr3,@-r8 */
L8c617216: sa_motion_publish_stop(ctx,0x8c617216u); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c617218; /* mov.l r4,@-r8 */
L8c617218: sa_motion_publish_stop(ctx,0x8c617218u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61721a; /* pref @r8 */
L8c61721a: sa_motion_publish_stop(ctx,0x8c61721au); r[3]=r[5];goto L8c61721c; /* mov r5,r3 */
L8c61721c: sa_motion_publish_stop(ctx,0x8c61721cu); r[3]+=4u;goto L8c61721e; /* add #0x4,r3 */
L8c61721e: sa_motion_publish_stop(ctx,0x8c61721eu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c617220; /* mov.l r3,@r15 */
L8c617220: sa_motion_publish_stop(ctx,0x8c617220u); r[8]+=56u;goto L8c617222; /* add #0x38,r8 */
L8c617222: sa_motion_publish_stop(ctx,0x8c617222u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c617224; /* mov.w @(0x2,r3),r0 */
L8c617224: sa_motion_publish_stop(ctx,0x8c617224u); r[14]+=32u;goto L8c617226; /* add #0x20,r14 */
L8c617226: sa_motion_publish_stop(ctx,0x8c617226u); r[3]=r[0];goto L8c617228; /* mov r0,r3 */
L8c617228: sa_motion_publish_stop(ctx,0x8c617228u); *fpul=r[3];goto L8c61722a; /* lds r3,FPUL */
L8c61722a: sa_motion_publish_stop(ctx,0x8c61722au); r[0]=24u;goto L8c61722c; /* mov #0x18,r0 */
L8c61722c: sa_motion_publish_stop(ctx,0x8c61722cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61722e; /* float FPUL,fr3 */
L8c61722e: sa_motion_publish_stop(ctx,0x8c61722eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617230; /* fmul fr5,fr3 */
L8c617230: sa_motion_publish_stop(ctx,0x8c617230u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617232; /* fmov.s fr3,@-r8 */
L8c617232: sa_motion_publish_stop(ctx,0x8c617232u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617234; /* mov.l @r15,r3 */
L8c617234: sa_motion_publish_stop(ctx,0x8c617234u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c617236; /* mov.w @r3,r3 */
L8c617236: sa_motion_publish_stop(ctx,0x8c617236u); *fpul=r[3];goto L8c617238; /* lds r3,FPUL */
L8c617238: sa_motion_publish_stop(ctx,0x8c617238u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61723a; /* float FPUL,fr3 */
L8c61723a: sa_motion_publish_stop(ctx,0x8c61723au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61723c; /* fmul fr5,fr3 */
L8c61723c: sa_motion_publish_stop(ctx,0x8c61723cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61723e; /* fmov.s fr3,@-r8 */
L8c61723e: sa_motion_publish_stop(ctx,0x8c61723eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617240; /* fmov.s @(r0,r14),fr3 */
L8c617240: sa_motion_publish_stop(ctx,0x8c617240u); r[0]=20u;goto L8c617242; /* mov #0x14,r0 */
L8c617242: sa_motion_publish_stop(ctx,0x8c617242u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617244; /* fmov.s fr3,@-r8 */
L8c617244: sa_motion_publish_stop(ctx,0x8c617244u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617246; /* fmov.s @(r0,r14),fr3 */
L8c617246: sa_motion_publish_stop(ctx,0x8c617246u); r[0]=16u;goto L8c617248; /* mov #0x10,r0 */
L8c617248: sa_motion_publish_stop(ctx,0x8c617248u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61724a; /* fmov.s fr3,@-r8 */
L8c61724a: sa_motion_publish_stop(ctx,0x8c61724au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61724c; /* fmov.s @(r0,r14),fr3 */
L8c61724c: sa_motion_publish_stop(ctx,0x8c61724cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61724e; /* fmov.s fr3,@-r8 */
L8c61724e: sa_motion_publish_stop(ctx,0x8c61724eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c617250; /* mov.l r4,@-r8 */
L8c617250: sa_motion_publish_stop(ctx,0x8c617250u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617252; /* pref @r8 */
L8c617252: sa_motion_publish_stop(ctx,0x8c617252u); r[4]=r[5];goto L8c617254; /* mov r5,r4 */
L8c617254: sa_motion_publish_stop(ctx,0x8c617254u); r[4]+=8u;goto L8c617256; /* add #0x8,r4 */
L8c617256: sa_motion_publish_stop(ctx,0x8c617256u); r[8]+=60u;goto L8c617258; /* add #0x3c,r8 */
L8c617258: sa_motion_publish_stop(ctx,0x8c617258u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61725a; /* fmov.s fr4,@-r8 */
L8c61725a: sa_motion_publish_stop(ctx,0x8c61725au); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61725c; /* mov.w @(0x2,r4),r0 */
L8c61725c: sa_motion_publish_stop(ctx,0x8c61725cu); r[3]=r[0];goto L8c61725e; /* mov r0,r3 */
L8c61725e: sa_motion_publish_stop(ctx,0x8c61725eu); *fpul=r[3];goto L8c617260; /* lds r3,FPUL */
L8c617260: sa_motion_publish_stop(ctx,0x8c617260u); r[0]=24u;goto L8c617262; /* mov #0x18,r0 */
L8c617262: sa_motion_publish_stop(ctx,0x8c617262u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617264; /* float FPUL,fr3 */
L8c617264: sa_motion_publish_stop(ctx,0x8c617264u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617266; /* fmul fr5,fr3 */
L8c617266: sa_motion_publish_stop(ctx,0x8c617266u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617268; /* fmov.s fr3,@-r8 */
L8c617268: sa_motion_publish_stop(ctx,0x8c617268u); r[3]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61726a; /* mov.w @r4,r3 */
L8c61726a: sa_motion_publish_stop(ctx,0x8c61726au); r[4]=r[6];goto L8c61726c; /* mov r6,r4 */
L8c61726c: sa_motion_publish_stop(ctx,0x8c61726cu); r[4]+=64u;goto L8c61726e; /* add #0x40,r4 */
L8c61726e: sa_motion_publish_stop(ctx,0x8c61726eu); *fpul=r[3];goto L8c617270; /* lds r3,FPUL */
L8c617270: sa_motion_publish_stop(ctx,0x8c617270u); r[3]=rd(m,bus,&io_status,0x8c6172b8u,4);if(io_status)return io_status;goto L8c617272; /* mov.l 0x8c6172b8,r3 */
L8c617272: sa_motion_publish_stop(ctx,0x8c617272u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617274; /* float FPUL,fr3 */
L8c617274: sa_motion_publish_stop(ctx,0x8c617274u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617276; /* fmul fr5,fr3 */
L8c617276: sa_motion_publish_stop(ctx,0x8c617276u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617278; /* fmov.s fr3,@-r8 */
L8c617278: sa_motion_publish_stop(ctx,0x8c617278u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61727a; /* fmov.s @(r0,r4),fr3 */
L8c61727a: sa_motion_publish_stop(ctx,0x8c61727au); r[0]=20u;goto L8c61727c; /* mov #0x14,r0 */
L8c61727c: sa_motion_publish_stop(ctx,0x8c61727cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61727e; /* fmov.s fr3,@-r8 */
L8c61727e: sa_motion_publish_stop(ctx,0x8c61727eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c617280; /* fmov.s @(r0,r4),fr3 */
L8c617280: sa_motion_publish_stop(ctx,0x8c617280u); r[0]=16u;goto L8c617282; /* mov #0x10,r0 */
L8c617282: sa_motion_publish_stop(ctx,0x8c617282u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617284; /* fmov.s fr3,@-r8 */
L8c617284: sa_motion_publish_stop(ctx,0x8c617284u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c617286; /* fmov.s @(r0,r4),fr3 */
L8c617286: sa_motion_publish_stop(ctx,0x8c617286u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617288; /* fmov.s fr3,@-r8 */
L8c617288: sa_motion_publish_stop(ctx,0x8c617288u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61728a; /* mov.l r3,@-r8 */
L8c61728a: sa_motion_publish_stop(ctx,0x8c61728au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61728c; /* pref @r8 */
L8c61728c: sa_motion_publish_stop(ctx,0x8c61728cu); r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c61728e; /* mov.l @r7,r2 */
L8c61728e: sa_motion_publish_stop(ctx,0x8c61728eu); r[8]+=32u;goto L8c617290; /* add #0x20,r8 */
L8c617290: sa_motion_publish_stop(ctx,0x8c617292u); r[2]+=3u;goto L8c6173a6; /* bra 0x8c6173a6 */
L8c6172bc: sa_motion_publish_stop(ctx,0x8c6172bcu); r[3]=r[5];goto L8c6172be; /* mov r5,r3 */
L8c6172be: sa_motion_publish_stop(ctx,0x8c6172beu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6172c0; /* mov.l r3,@r15 */
L8c6172c0: sa_motion_publish_stop(ctx,0x8c6172c0u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6172c2; /* mov.w @(0x2,r3),r0 */
L8c6172c2: sa_motion_publish_stop(ctx,0x8c6172c2u); r[8]+=24u;goto L8c6172c4; /* add #0x18,r8 */
L8c6172c4: sa_motion_publish_stop(ctx,0x8c6172c4u); r[14]=r[6];goto L8c6172c6; /* mov r6,r14 */
L8c6172c6: sa_motion_publish_stop(ctx,0x8c6172c6u); r[3]=r[0];goto L8c6172c8; /* mov r0,r3 */
L8c6172c8: sa_motion_publish_stop(ctx,0x8c6172c8u); *fpul=r[3];goto L8c6172ca; /* lds r3,FPUL */
L8c6172ca: sa_motion_publish_stop(ctx,0x8c6172cau); r[0]=24u;goto L8c6172cc; /* mov #0x18,r0 */
L8c6172cc: sa_motion_publish_stop(ctx,0x8c6172ccu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6172ce; /* float FPUL,fr3 */
L8c6172ce: sa_motion_publish_stop(ctx,0x8c6172ceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6172d0; /* fmul fr5,fr3 */
L8c6172d0: sa_motion_publish_stop(ctx,0x8c6172d0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6172d2; /* fmov.s fr3,@-r8 */
L8c6172d2: sa_motion_publish_stop(ctx,0x8c6172d2u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6172d4; /* mov.l @r15,r3 */
L8c6172d4: sa_motion_publish_stop(ctx,0x8c6172d4u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6172d6; /* mov.w @r3,r3 */
L8c6172d6: sa_motion_publish_stop(ctx,0x8c6172d6u); *fpul=r[3];goto L8c6172d8; /* lds r3,FPUL */
L8c6172d8: sa_motion_publish_stop(ctx,0x8c6172d8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6172da; /* float FPUL,fr3 */
L8c6172da: sa_motion_publish_stop(ctx,0x8c6172dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6172dc; /* fmul fr5,fr3 */
L8c6172dc: sa_motion_publish_stop(ctx,0x8c6172dcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6172de; /* fmov.s fr3,@-r8 */
L8c6172de: sa_motion_publish_stop(ctx,0x8c6172deu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6172e0; /* fmov.s @(r0,r14),fr3 */
L8c6172e0: sa_motion_publish_stop(ctx,0x8c6172e0u); r[0]=20u;goto L8c6172e2; /* mov #0x14,r0 */
L8c6172e2: sa_motion_publish_stop(ctx,0x8c6172e2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6172e4; /* fmov.s fr3,@-r8 */
L8c6172e4: sa_motion_publish_stop(ctx,0x8c6172e4u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6172e6; /* fmov.s @(r0,r14),fr3 */
L8c6172e6: sa_motion_publish_stop(ctx,0x8c6172e6u); r[0]=16u;goto L8c6172e8; /* mov #0x10,r0 */
L8c6172e8: sa_motion_publish_stop(ctx,0x8c6172e8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6172ea; /* fmov.s fr3,@-r8 */
L8c6172ea: sa_motion_publish_stop(ctx,0x8c6172eau); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6172ec; /* fmov.s @(r0,r14),fr3 */
L8c6172ec: sa_motion_publish_stop(ctx,0x8c6172ecu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6172ee; /* fmov.s fr3,@-r8 */
L8c6172ee: sa_motion_publish_stop(ctx,0x8c6172eeu); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c6172f0; /* mov.l r4,@-r8 */
L8c6172f0: sa_motion_publish_stop(ctx,0x8c6172f0u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6172f2; /* pref @r8 */
L8c6172f2: sa_motion_publish_stop(ctx,0x8c6172f2u); r[3]=r[5];goto L8c6172f4; /* mov r5,r3 */
L8c6172f4: sa_motion_publish_stop(ctx,0x8c6172f4u); r[3]+=4u;goto L8c6172f6; /* add #0x4,r3 */
L8c6172f6: sa_motion_publish_stop(ctx,0x8c6172f6u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6172f8; /* mov.l r3,@r15 */
L8c6172f8: sa_motion_publish_stop(ctx,0x8c6172f8u); r[8]+=56u;goto L8c6172fa; /* add #0x38,r8 */
L8c6172fa: sa_motion_publish_stop(ctx,0x8c6172fau); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6172fc; /* mov.w @(0x2,r3),r0 */
L8c6172fc: sa_motion_publish_stop(ctx,0x8c6172fcu); r[14]+=32u;goto L8c6172fe; /* add #0x20,r14 */
L8c6172fe: sa_motion_publish_stop(ctx,0x8c6172feu); r[3]=r[0];goto L8c617300; /* mov r0,r3 */
L8c617300: sa_motion_publish_stop(ctx,0x8c617300u); *fpul=r[3];goto L8c617302; /* lds r3,FPUL */
L8c617302: sa_motion_publish_stop(ctx,0x8c617302u); r[0]=24u;goto L8c617304; /* mov #0x18,r0 */
L8c617304: sa_motion_publish_stop(ctx,0x8c617304u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617306; /* float FPUL,fr3 */
L8c617306: sa_motion_publish_stop(ctx,0x8c617306u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617308; /* fmul fr5,fr3 */
L8c617308: sa_motion_publish_stop(ctx,0x8c617308u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61730a; /* fmov.s fr3,@-r8 */
L8c61730a: sa_motion_publish_stop(ctx,0x8c61730au); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61730c; /* mov.l @r15,r3 */
L8c61730c: sa_motion_publish_stop(ctx,0x8c61730cu); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c61730e; /* mov.w @r3,r3 */
L8c61730e: sa_motion_publish_stop(ctx,0x8c61730eu); *fpul=r[3];goto L8c617310; /* lds r3,FPUL */
L8c617310: sa_motion_publish_stop(ctx,0x8c617310u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617312; /* float FPUL,fr3 */
L8c617312: sa_motion_publish_stop(ctx,0x8c617312u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617314; /* fmul fr5,fr3 */
L8c617314: sa_motion_publish_stop(ctx,0x8c617314u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617316; /* fmov.s fr3,@-r8 */
L8c617316: sa_motion_publish_stop(ctx,0x8c617316u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617318; /* fmov.s @(r0,r14),fr3 */
L8c617318: sa_motion_publish_stop(ctx,0x8c617318u); r[0]=20u;goto L8c61731a; /* mov #0x14,r0 */
L8c61731a: sa_motion_publish_stop(ctx,0x8c61731au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61731c; /* fmov.s fr3,@-r8 */
L8c61731c: sa_motion_publish_stop(ctx,0x8c61731cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61731e; /* fmov.s @(r0,r14),fr3 */
L8c61731e: sa_motion_publish_stop(ctx,0x8c61731eu); r[0]=16u;goto L8c617320; /* mov #0x10,r0 */
L8c617320: sa_motion_publish_stop(ctx,0x8c617320u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617322; /* fmov.s fr3,@-r8 */
L8c617322: sa_motion_publish_stop(ctx,0x8c617322u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617324; /* fmov.s @(r0,r14),fr3 */
L8c617324: sa_motion_publish_stop(ctx,0x8c617324u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617326; /* fmov.s fr3,@-r8 */
L8c617326: sa_motion_publish_stop(ctx,0x8c617326u); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c617328; /* mov.l r4,@-r8 */
L8c617328: sa_motion_publish_stop(ctx,0x8c617328u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61732a; /* pref @r8 */
L8c61732a: sa_motion_publish_stop(ctx,0x8c61732au); r[3]=r[5];goto L8c61732c; /* mov r5,r3 */
L8c61732c: sa_motion_publish_stop(ctx,0x8c61732cu); r[3]+=12u;goto L8c61732e; /* add #0xc,r3 */
L8c61732e: sa_motion_publish_stop(ctx,0x8c61732eu); r[8]+=60u;goto L8c617330; /* add #0x3c,r8 */
L8c617330: sa_motion_publish_stop(ctx,0x8c617330u); r[14]=r[6];goto L8c617332; /* mov r6,r14 */
L8c617332: sa_motion_publish_stop(ctx,0x8c617332u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c617334; /* fmov.s fr4,@-r8 */
L8c617334: sa_motion_publish_stop(ctx,0x8c617334u); r[14]+=96u;goto L8c617336; /* add #0x60,r14 */
L8c617336: sa_motion_publish_stop(ctx,0x8c617336u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c617338; /* mov.l r3,@r15 */
L8c617338: sa_motion_publish_stop(ctx,0x8c617338u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61733a; /* mov.w @(0x2,r3),r0 */
L8c61733a: sa_motion_publish_stop(ctx,0x8c61733au); r[3]=r[0];goto L8c61733c; /* mov r0,r3 */
L8c61733c: sa_motion_publish_stop(ctx,0x8c61733cu); *fpul=r[3];goto L8c61733e; /* lds r3,FPUL */
L8c61733e: sa_motion_publish_stop(ctx,0x8c61733eu); r[0]=24u;goto L8c617340; /* mov #0x18,r0 */
L8c617340: sa_motion_publish_stop(ctx,0x8c617340u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617342; /* float FPUL,fr3 */
L8c617342: sa_motion_publish_stop(ctx,0x8c617342u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617344; /* fmul fr5,fr3 */
L8c617344: sa_motion_publish_stop(ctx,0x8c617344u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617346; /* fmov.s fr3,@-r8 */
L8c617346: sa_motion_publish_stop(ctx,0x8c617346u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617348; /* mov.l @r15,r3 */
L8c617348: sa_motion_publish_stop(ctx,0x8c617348u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c61734a; /* mov.w @r3,r3 */
L8c61734a: sa_motion_publish_stop(ctx,0x8c61734au); *fpul=r[3];goto L8c61734c; /* lds r3,FPUL */
L8c61734c: sa_motion_publish_stop(ctx,0x8c61734cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61734e; /* float FPUL,fr3 */
L8c61734e: sa_motion_publish_stop(ctx,0x8c61734eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617350; /* fmul fr5,fr3 */
L8c617350: sa_motion_publish_stop(ctx,0x8c617350u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617352; /* fmov.s fr3,@-r8 */
L8c617352: sa_motion_publish_stop(ctx,0x8c617352u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617354; /* fmov.s @(r0,r14),fr3 */
L8c617354: sa_motion_publish_stop(ctx,0x8c617354u); r[0]=20u;goto L8c617356; /* mov #0x14,r0 */
L8c617356: sa_motion_publish_stop(ctx,0x8c617356u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617358; /* fmov.s fr3,@-r8 */
L8c617358: sa_motion_publish_stop(ctx,0x8c617358u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61735a; /* fmov.s @(r0,r14),fr3 */
L8c61735a: sa_motion_publish_stop(ctx,0x8c61735au); r[0]=16u;goto L8c61735c; /* mov #0x10,r0 */
L8c61735c: sa_motion_publish_stop(ctx,0x8c61735cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61735e; /* fmov.s fr3,@-r8 */
L8c61735e: sa_motion_publish_stop(ctx,0x8c61735eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617360; /* fmov.s @(r0,r14),fr3 */
L8c617360: sa_motion_publish_stop(ctx,0x8c617360u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617362; /* fmov.s fr3,@-r8 */
L8c617362: sa_motion_publish_stop(ctx,0x8c617362u); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c617364; /* mov.l r4,@-r8 */
L8c617364: sa_motion_publish_stop(ctx,0x8c617364u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617366; /* pref @r8 */
L8c617366: sa_motion_publish_stop(ctx,0x8c617366u); r[4]=r[5];goto L8c617368; /* mov r5,r4 */
L8c617368: sa_motion_publish_stop(ctx,0x8c617368u); r[4]+=8u;goto L8c61736a; /* add #0x8,r4 */
L8c61736a: sa_motion_publish_stop(ctx,0x8c61736au); r[8]+=60u;goto L8c61736c; /* add #0x3c,r8 */
L8c61736c: sa_motion_publish_stop(ctx,0x8c61736cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61736e; /* fmov.s fr4,@-r8 */
L8c61736e: sa_motion_publish_stop(ctx,0x8c61736eu); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c617370; /* mov.w @(0x2,r4),r0 */
L8c617370: sa_motion_publish_stop(ctx,0x8c617370u); r[3]=r[0];goto L8c617372; /* mov r0,r3 */
L8c617372: sa_motion_publish_stop(ctx,0x8c617372u); *fpul=r[3];goto L8c617374; /* lds r3,FPUL */
L8c617374: sa_motion_publish_stop(ctx,0x8c617374u); r[0]=24u;goto L8c617376; /* mov #0x18,r0 */
L8c617376: sa_motion_publish_stop(ctx,0x8c617376u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617378; /* float FPUL,fr3 */
L8c617378: sa_motion_publish_stop(ctx,0x8c617378u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61737a; /* fmul fr5,fr3 */
L8c61737a: sa_motion_publish_stop(ctx,0x8c61737au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61737c; /* fmov.s fr3,@-r8 */
L8c61737c: sa_motion_publish_stop(ctx,0x8c61737cu); r[3]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61737e; /* mov.w @r4,r3 */
L8c61737e: sa_motion_publish_stop(ctx,0x8c61737eu); r[4]=r[6];goto L8c617380; /* mov r6,r4 */
L8c617380: sa_motion_publish_stop(ctx,0x8c617380u); r[4]+=64u;goto L8c617382; /* add #0x40,r4 */
L8c617382: sa_motion_publish_stop(ctx,0x8c617382u); *fpul=r[3];goto L8c617384; /* lds r3,FPUL */
L8c617384: sa_motion_publish_stop(ctx,0x8c617384u); r[3]=rd(m,bus,&io_status,0x8c6174bcu,4);if(io_status)return io_status;goto L8c617386; /* mov.l 0x8c6174bc,r3 */
L8c617386: sa_motion_publish_stop(ctx,0x8c617386u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617388; /* float FPUL,fr3 */
L8c617388: sa_motion_publish_stop(ctx,0x8c617388u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61738a; /* fmul fr5,fr3 */
L8c61738a: sa_motion_publish_stop(ctx,0x8c61738au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61738c; /* fmov.s fr3,@-r8 */
L8c61738c: sa_motion_publish_stop(ctx,0x8c61738cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61738e; /* fmov.s @(r0,r4),fr3 */
L8c61738e: sa_motion_publish_stop(ctx,0x8c61738eu); r[0]=20u;goto L8c617390; /* mov #0x14,r0 */
L8c617390: sa_motion_publish_stop(ctx,0x8c617390u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617392; /* fmov.s fr3,@-r8 */
L8c617392: sa_motion_publish_stop(ctx,0x8c617392u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c617394; /* fmov.s @(r0,r4),fr3 */
L8c617394: sa_motion_publish_stop(ctx,0x8c617394u); r[0]=16u;goto L8c617396; /* mov #0x10,r0 */
L8c617396: sa_motion_publish_stop(ctx,0x8c617396u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617398; /* fmov.s fr3,@-r8 */
L8c617398: sa_motion_publish_stop(ctx,0x8c617398u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61739a; /* fmov.s @(r0,r4),fr3 */
L8c61739a: sa_motion_publish_stop(ctx,0x8c61739au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61739c; /* fmov.s fr3,@-r8 */
L8c61739c: sa_motion_publish_stop(ctx,0x8c61739cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61739e; /* mov.l r3,@-r8 */
L8c61739e: sa_motion_publish_stop(ctx,0x8c61739eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6173a0; /* pref @r8 */
L8c6173a0: sa_motion_publish_stop(ctx,0x8c6173a0u); r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c6173a2; /* mov.l @r7,r2 */
L8c6173a2: sa_motion_publish_stop(ctx,0x8c6173a2u); r[8]+=32u;goto L8c6173a4; /* add #0x20,r8 */
L8c6173a4: sa_motion_publish_stop(ctx,0x8c6173a4u); r[2]+=4u;goto L8c6173a6; /* add #0x4,r2 */
L8c6173a6: sa_motion_publish_stop(ctx,0x8c6173a6u); wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c6173a8; /* mov.l r2,@r7 */
L8c6173a8: sa_motion_publish_stop(ctx,0x8c6173a8u); r[15]+=68u;goto L8c6173aa; /* add #0x44,r15 */
L8c6173aa: sa_motion_publish_stop(ctx,0x8c6173aau); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6173ac; /* fmov.s @r15+,fr12 */
L8c6173ac: sa_motion_publish_stop(ctx,0x8c6173acu); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6173ae; /* mov.l @r15+,r9 */
L8c6173ae: sa_motion_publish_stop(ctx,0x8c6173aeu); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6173b0; /* mov.l @r15+,r10 */
L8c6173b0: sa_motion_publish_stop(ctx,0x8c6173b0u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6173b2; /* mov.l @r15+,r11 */
L8c6173b2: sa_motion_publish_stop(ctx,0x8c6173b2u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6173b4; /* mov.l @r15+,r12 */
L8c6173b4: sa_motion_publish_stop(ctx,0x8c6173b4u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6173b6; /* mov.l @r15+,r13 */
L8c6173b6: sa_motion_publish_stop(ctx,0x8c6173b8u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
