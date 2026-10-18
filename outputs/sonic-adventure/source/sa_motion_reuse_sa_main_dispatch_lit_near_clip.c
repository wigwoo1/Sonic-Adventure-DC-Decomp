#include "sa_draw_motion_stop.h"
/* Lit triangle near-plane clipper with portable queue interface; name inferred.
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
SA_STAGE_EXPORT int sa_lit_triangle_near_clip_8c612e64(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c612e64;
L8c612e64: sa_motion_publish_stop(ctx,0x8c612e64u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c612e66; /* mov.l r14,@-r15 */
L8c612e66: sa_motion_publish_stop(ctx,0x8c612e66u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c612e68; /* mov.l r13,@-r15 */
L8c612e68: sa_motion_publish_stop(ctx,0x8c612e68u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c612e6a; /* mov.l r12,@-r15 */
L8c612e6a: sa_motion_publish_stop(ctx,0x8c612e6au); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c612e6c; /* mov.l r11,@-r15 */
L8c612e6c: sa_motion_publish_stop(ctx,0x8c612e6cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c612e6e; /* mov.l r10,@-r15 */
L8c612e6e: sa_motion_publish_stop(ctx,0x8c612e6eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c612e70; /* mov.l r9,@-r15 */
L8c612e70: sa_motion_publish_stop(ctx,0x8c612e70u); r[15]+=4294967232u;goto L8c612e72; /* add #-0x40,r15 */
L8c612e72: sa_motion_publish_stop(ctx,0x8c612e72u); r[3]=rd(m,bus,&io_status,0x8c612f30u,4);if(io_status)return io_status;goto L8c612e74; /* mov.l 0x8c612f30,r3 */
L8c612e74: sa_motion_publish_stop(ctx,0x8c612e74u); r[7]=r[15];goto L8c612e76; /* mov r15,r7 */
L8c612e76: sa_motion_publish_stop(ctx,0x8c612e76u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c612e78; /* fldi1 fr6 */
L8c612e78: sa_motion_publish_stop(ctx,0x8c612e78u); r[13]=r[15];goto L8c612e7a; /* mov r15,r13 */
L8c612e7a: sa_motion_publish_stop(ctx,0x8c612e7au); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c612e7c; /* fmov.s @r3,fr3 */
L8c612e7c: sa_motion_publish_stop(ctx,0x8c612e7cu); r[7]+=52u;goto L8c612e7e; /* add #0x34,r7 */
L8c612e7e: sa_motion_publish_stop(ctx,0x8c612e7eu); fcopy(f,4u,6u);goto L8c612e80; /* fmov fr6,fr4 */
L8c612e80: sa_motion_publish_stop(ctx,0x8c612e80u); r[13]+=16u;goto L8c612e82; /* add #0x10,r13 */
L8c612e82: sa_motion_publish_stop(ctx,0x8c612e82u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c612e84; /* fdiv fr3,fr4 */
L8c612e84: sa_motion_publish_stop(ctx,0x8c612e84u); r[2]=r[7];goto L8c612e86; /* mov r7,r2 */
L8c612e86: sa_motion_publish_stop(ctx,0x8c612e86u); r[9]=rd(m,bus,&io_status,0x8c612f34u,4);if(io_status)return io_status;goto L8c612e88; /* mov.l 0x8c612f34,r9 */
L8c612e88: sa_motion_publish_stop(ctx,0x8c612e88u); r[10]=3u;goto L8c612e8a; /* mov #0x3,r10 */
L8c612e8a: sa_motion_publish_stop(ctx,0x8c612e8au); r[11]=0u;goto L8c612e8c; /* mov #0x0,r11 */
L8c612e8c: sa_motion_publish_stop(ctx,0x8c612e8cu); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c612e8e; /* mov.l r2,@r15 */
L8c612e8e: sa_motion_publish_stop(ctx,0x8c612e8eu); wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c612e90; /* mov.l r4,@r2 */
L8c612e90: sa_motion_publish_stop(ctx,0x8c612e90u); r[4]=1u;goto L8c612e92; /* mov #0x1,r4 */
L8c612e92: sa_motion_publish_stop(ctx,0x8c612e92u); wr(m,bus,&io_status,r[7]+0x4u,r[5],4);if(io_status)return io_status;goto L8c612e94; /* mov.l r5,@(0x4,r7) */
L8c612e94: sa_motion_publish_stop(ctx,0x8c612e94u); r[5]=r[13];goto L8c612e96; /* mov r13,r5 */
L8c612e96: sa_motion_publish_stop(ctx,0x8c612e96u); wr(m,bus,&io_status,r[7]+0x8u,r[6],4);if(io_status)return io_status;goto L8c612e98; /* mov.l r6,@(0x8,r7) */
L8c612e98: sa_motion_publish_stop(ctx,0x8c612e98u); r[14]=r[5];goto L8c612e9a; /* mov r5,r14 */
L8c612e9a: sa_motion_publish_stop(ctx,0x8c612e9au); r[6]=rd(m,bus,&io_status,0x8c612f38u,4);if(io_status)return io_status;goto L8c612e9c; /* mov.l 0x8c612f38,r6 */
L8c612e9c: sa_motion_publish_stop(ctx,0x8c612e9cu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612e9e; /* mov.l @r15,r12 */
L8c612e9e: sa_motion_publish_stop(ctx,0x8c612e9eu); wr(m,bus,&io_status,r[15]+0x8u,r[5],4);if(io_status)return io_status;goto L8c612ea0; /* mov.l r5,@(0x8,r15) */
L8c612ea0: sa_motion_publish_stop(ctx,0x8c612ea0u); r[3]=r[11];goto L8c612ea2; /* mov r11,r3 */
L8c612ea2: sa_motion_publish_stop(ctx,0x8c612ea2u); r[3]<<=2;goto L8c612ea4; /* shll2 r3 */
L8c612ea4: sa_motion_publish_stop(ctx,0x8c612ea4u); r[3]+=r[7];goto L8c612ea6; /* add r7,r3 */
L8c612ea6: sa_motion_publish_stop(ctx,0x8c612ea6u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c612ea8; /* mov.l r3,@(0x4,r15) */
L8c612ea8: sa_motion_publish_stop(ctx,0x8c612ea8u); r[0]=4u;goto L8c612eaa; /* mov #0x4,r0 */
L8c612eaa: sa_motion_publish_stop(ctx,0x8c612eaau); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c612eac; /* mov.l @r3,r2 */
L8c612eac: sa_motion_publish_stop(ctx,0x8c612eacu); r[3]=r[2];goto L8c612eae; /* mov r2,r3 */
L8c612eae: sa_motion_publish_stop(ctx,0x8c612eaeu); r[1]=r[2];goto L8c612eb0; /* mov r2,r1 */
L8c612eb0: sa_motion_publish_stop(ctx,0x8c612eb0u); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c612eb2; /* mov.l r2,@r15 */
L8c612eb2: sa_motion_publish_stop(ctx,0x8c612eb2u); fload(m,bus,&io_status,f,2u,r[9]);if(io_status)return io_status;goto L8c612eb4; /* fmov.s @r9,fr2 */
L8c612eb4: sa_motion_publish_stop(ctx,0x8c612eb4u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c612eb6; /* fmov.s @r3,fr3 */
L8c612eb6: sa_motion_publish_stop(ctx,0x8c612eb6u); r[3]=rd(m,bus,&io_status,0x8c612f3cu,4);if(io_status)return io_status;goto L8c612eb8; /* mov.l 0x8c612f3c,r3 */
L8c612eb8: sa_motion_publish_stop(ctx,0x8c612eb8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612eba; /* fmul fr3,fr2 */
L8c612eba: sa_motion_publish_stop(ctx,0x8c612ebau); fload(m,bus,&io_status,f,1u,r[0]+r[9]);if(io_status)return io_status;goto L8c612ebc; /* fmov.s @(r0,r9),fr1 */
L8c612ebc: sa_motion_publish_stop(ctx,0x8c612ebcu); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c612ebe; /* fmov.s @r3,fr3 */
L8c612ebe: sa_motion_publish_stop(ctx,0x8c612ebeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612ec0; /* fsub fr2,fr3 */
L8c612ec0: sa_motion_publish_stop(ctx,0x8c612ec0u); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c612ec2; /* fmov.s @(r0,r1),fr2 */
L8c612ec2: sa_motion_publish_stop(ctx,0x8c612ec2u); r[0]=8u;goto L8c612ec4; /* mov #0x8,r0 */
L8c612ec4: sa_motion_publish_stop(ctx,0x8c612ec4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c612ec6; /* fmul fr2,fr1 */
L8c612ec6: sa_motion_publish_stop(ctx,0x8c612ec6u); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c612ec8; /* fmov.s @(r0,r1),fr2 */
L8c612ec8: sa_motion_publish_stop(ctx,0x8c612ec8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612eca; /* fsub fr1,fr3 */
L8c612eca: sa_motion_publish_stop(ctx,0x8c612ecau); fload(m,bus,&io_status,f,1u,r[0]+r[9]);if(io_status)return io_status;goto L8c612ecc; /* fmov.s @(r0,r9),fr1 */
L8c612ecc: sa_motion_publish_stop(ctx,0x8c612eccu); r[0]=28u;goto L8c612ece; /* mov #0x1c,r0 */
L8c612ece: sa_motion_publish_stop(ctx,0x8c612eceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c612ed0; /* fmul fr2,fr1 */
L8c612ed0: sa_motion_publish_stop(ctx,0x8c612ed0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612ed2; /* fsub fr1,fr3 */
L8c612ed2: sa_motion_publish_stop(ctx,0x8c612ed2u); fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612ed4; /* fmov fr3,@(r0,r2) */
L8c612ed4: sa_motion_publish_stop(ctx,0x8c612ed4u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612ed6; /* mov.l @(0x4,r15),r2 */
L8c612ed6: sa_motion_publish_stop(ctx,0x8c612ed6u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c612ed8; /* mov.l @r2,r1 */
L8c612ed8: sa_motion_publish_stop(ctx,0x8c612ed8u); r[0]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c612eda; /* mov.l @(0xc,r1),r0 */
L8c612eda: sa_motion_publish_stop(ctx,0x8c612edau); *sr=(*sr&~1u)|!(r[4]&r[0]);goto L8c612edc; /* tst r4,r0 */
L8c612edc: sa_motion_publish_stop(ctx,0x8c612edcu); if(!(*sr&1u))goto L8c612f48;goto L8c612ede; /* bf 0x8c612f48 */
L8c612ede: sa_motion_publish_stop(ctx,0x8c612edeu); r[1]=r[11];goto L8c612ee0; /* mov r11,r1 */
L8c612ee0: sa_motion_publish_stop(ctx,0x8c612ee0u); r[1]<<=2;goto L8c612ee2; /* shll2 r1 */
L8c612ee2: sa_motion_publish_stop(ctx,0x8c612ee2u); r[1]+=r[7];goto L8c612ee4; /* add r7,r1 */
L8c612ee4: sa_motion_publish_stop(ctx,0x8c612ee4u); fcopy(f,5u,6u);goto L8c612ee6; /* fmov fr6,fr5 */
L8c612ee6: sa_motion_publish_stop(ctx,0x8c612ee6u); r[0]=24u;goto L8c612ee8; /* mov #0x18,r0 */
L8c612ee8: sa_motion_publish_stop(ctx,0x8c612ee8u); wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c612eea; /* mov.l r1,@r15 */
L8c612eea: sa_motion_publish_stop(ctx,0x8c612eeau); r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c612eec; /* mov.l @r1,r2 */
L8c612eec: sa_motion_publish_stop(ctx,0x8c612eecu); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c612eee; /* mov.l r2,@(0x4,r15) */
L8c612eee: sa_motion_publish_stop(ctx,0x8c612eeeu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612ef0; /* fmov.s @(r0,r2),fr3 */
L8c612ef0: sa_motion_publish_stop(ctx,0x8c612ef0u); r[0]=12u;goto L8c612ef2; /* mov #0xc,r0 */
L8c612ef2: sa_motion_publish_stop(ctx,0x8c612ef2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c612ef4; /* fdiv fr3,fr5 */
L8c612ef4: sa_motion_publish_stop(ctx,0x8c612ef4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612ef6; /* fmov.s @(r0,r6),fr3 */
L8c612ef6: sa_motion_publish_stop(ctx,0x8c612ef6u); r[0]=16u;goto L8c612ef8; /* mov #0x10,r0 */
L8c612ef8: sa_motion_publish_stop(ctx,0x8c612ef8u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c612efa; /* fmov.s @(r0,r2),fr2 */
L8c612efa: sa_motion_publish_stop(ctx,0x8c612efau); r[0]=16u;goto L8c612efc; /* mov #0x10,r0 */
L8c612efc: sa_motion_publish_stop(ctx,0x8c612efcu); r[2]=rd(m,bus,&io_status,0x8c612f40u,4);if(io_status)return io_status;goto L8c612efe; /* mov.l 0x8c612f40,r2 */
L8c612efe: sa_motion_publish_stop(ctx,0x8c612efeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612f00; /* fsub fr3,fr2 */
L8c612f00: sa_motion_publish_stop(ctx,0x8c612f00u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c612f02; /* fmov.s @r2,fr3 */
L8c612f02: sa_motion_publish_stop(ctx,0x8c612f02u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612f04; /* fmul fr5,fr2 */
L8c612f04: sa_motion_publish_stop(ctx,0x8c612f04u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612f06; /* fdiv fr3,fr2 */
L8c612f06: sa_motion_publish_stop(ctx,0x8c612f06u); fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c612f08; /* fmov.s fr2,@r14 */
L8c612f08: sa_motion_publish_stop(ctx,0x8c612f08u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612f0a; /* mov.l @r15,r1 */
L8c612f0a: sa_motion_publish_stop(ctx,0x8c612f0au); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612f0c; /* fmov.s @(r0,r6),fr3 */
L8c612f0c: sa_motion_publish_stop(ctx,0x8c612f0cu); r[0]=20u;goto L8c612f0e; /* mov #0x14,r0 */
L8c612f0e: sa_motion_publish_stop(ctx,0x8c612f0eu); r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c612f10; /* mov.l @r1,r1 */
L8c612f10: sa_motion_publish_stop(ctx,0x8c612f10u); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c612f12; /* fmov.s @(r0,r1),fr2 */
L8c612f12: sa_motion_publish_stop(ctx,0x8c612f12u); r[0]=4u;goto L8c612f14; /* mov #0x4,r0 */
L8c612f14: sa_motion_publish_stop(ctx,0x8c612f14u); r[1]=rd(m,bus,&io_status,0x8c612f44u,4);if(io_status)return io_status;goto L8c612f16; /* mov.l 0x8c612f44,r1 */
L8c612f16: sa_motion_publish_stop(ctx,0x8c612f16u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612f18; /* fsub fr3,fr2 */
L8c612f18: sa_motion_publish_stop(ctx,0x8c612f18u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c612f1a; /* fmov.s @r1,fr3 */
L8c612f1a: sa_motion_publish_stop(ctx,0x8c612f1au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612f1c; /* fmul fr5,fr2 */
L8c612f1c: sa_motion_publish_stop(ctx,0x8c612f1cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612f1e; /* fdiv fr3,fr2 */
L8c612f1e: sa_motion_publish_stop(ctx,0x8c612f1eu); fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c612f20; /* fmov fr2,@(r0,r14) */
L8c612f20: sa_motion_publish_stop(ctx,0x8c612f20u); r[0]=8u;goto L8c612f22; /* mov #0x8,r0 */
L8c612f22: sa_motion_publish_stop(ctx,0x8c612f24u); fstore(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c612f64; /* bra 0x8c612f64 */
L8c612f48: sa_motion_publish_stop(ctx,0x8c612f48u); r[2]=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;goto L8c612f4a; /* mov.l @r12,r2 */
L8c612f4a: sa_motion_publish_stop(ctx,0x8c612f4au); r[0]=16u;goto L8c612f4c; /* mov #0x10,r0 */
L8c612f4c: sa_motion_publish_stop(ctx,0x8c612f4cu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612f4e; /* fmov.s @(r0,r2),fr3 */
L8c612f4e: sa_motion_publish_stop(ctx,0x8c612f4eu); r[0]=20u;goto L8c612f50; /* mov #0x14,r0 */
L8c612f50: sa_motion_publish_stop(ctx,0x8c612f50u); fstore(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c612f52; /* fmov.s fr3,@r5 */
L8c612f52: sa_motion_publish_stop(ctx,0x8c612f52u); r[2]=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;goto L8c612f54; /* mov.l @r12,r2 */
L8c612f54: sa_motion_publish_stop(ctx,0x8c612f54u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612f56; /* fmov.s @(r0,r2),fr3 */
L8c612f56: sa_motion_publish_stop(ctx,0x8c612f56u); r[0]=4u;goto L8c612f58; /* mov #0x4,r0 */
L8c612f58: sa_motion_publish_stop(ctx,0x8c612f58u); fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c612f5a; /* fmov fr3,@(r0,r5) */
L8c612f5a: sa_motion_publish_stop(ctx,0x8c612f5au); r[0]=24u;goto L8c612f5c; /* mov #0x18,r0 */
L8c612f5c: sa_motion_publish_stop(ctx,0x8c612f5cu); r[2]=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;goto L8c612f5e; /* mov.l @r12,r2 */
L8c612f5e: sa_motion_publish_stop(ctx,0x8c612f5eu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612f60; /* fmov.s @(r0,r2),fr3 */
L8c612f60: sa_motion_publish_stop(ctx,0x8c612f60u); r[0]=8u;goto L8c612f62; /* mov #0x8,r0 */
L8c612f62: sa_motion_publish_stop(ctx,0x8c612f62u); fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c612f64; /* fmov fr3,@(r0,r5) */
L8c612f64: sa_motion_publish_stop(ctx,0x8c612f64u); r[11]+=1u;goto L8c612f66; /* add #0x1,r11 */
L8c612f66: sa_motion_publish_stop(ctx,0x8c612f66u); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[10]);goto L8c612f68; /* cmp/ge r10,r11 */
L8c612f68: sa_motion_publish_stop(ctx,0x8c612f68u); r[5]+=12u;goto L8c612f6a; /* add #0xc,r5 */
L8c612f6a: sa_motion_publish_stop(ctx,0x8c612f6au); r[12]+=4u;goto L8c612f6c; /* add #0x4,r12 */
L8c612f6c: sa_motion_publish_stop(ctx,0x8c612f6eu); take=!(*sr&1u);r[14]+=12u;if(take)goto L8c612ea0;goto L8c612f70; /* bf/s 0x8c612ea0 */
L8c612f70: sa_motion_publish_stop(ctx,0x8c612f70u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612f72; /* mov.l @(0x8,r15),r2 */
L8c612f72: sa_motion_publish_stop(ctx,0x8c612f72u); r[0]=8u;goto L8c612f74; /* mov #0x8,r0 */
L8c612f74: sa_motion_publish_stop(ctx,0x8c612f74u); r[14]=r[13];goto L8c612f76; /* mov r13,r14 */
L8c612f76: sa_motion_publish_stop(ctx,0x8c612f76u); r[14]+=12u;goto L8c612f78; /* add #0xc,r14 */
L8c612f78: sa_motion_publish_stop(ctx,0x8c612f78u); fload(m,bus,&io_status,f,6u,r[0]+r[2]);if(io_status)return io_status;goto L8c612f7a; /* fmov.s @(r0,r2),fr6 */
L8c612f7a: sa_motion_publish_stop(ctx,0x8c612f7au); r[0]=4u;goto L8c612f7c; /* mov #0x4,r0 */
L8c612f7c: sa_motion_publish_stop(ctx,0x8c612f7cu); r[3]=r[2];goto L8c612f7e; /* mov r2,r3 */
L8c612f7e: sa_motion_publish_stop(ctx,0x8c612f7eu); fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c612f80; /* fmov.s @(r0,r14),fr5 */
L8c612f80: sa_motion_publish_stop(ctx,0x8c612f80u); fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c612f82; /* fmov.s @(r0,r3),fr9 */
L8c612f82: sa_motion_publish_stop(ctx,0x8c612f82u); r[0]=8u;goto L8c612f84; /* mov #0x8,r0 */
L8c612f84: sa_motion_publish_stop(ctx,0x8c612f84u); fload(m,bus,&io_status,f,7u,r[0]+r[14]);if(io_status)return io_status;goto L8c612f86; /* fmov.s @(r0,r14),fr7 */
L8c612f86: sa_motion_publish_stop(ctx,0x8c612f86u); r[5]=r[13];goto L8c612f88; /* mov r13,r5 */
L8c612f88: sa_motion_publish_stop(ctx,0x8c612f88u); fcopy(f,3u,9u);goto L8c612f8a; /* fmov fr9,fr3 */
L8c612f8a: sa_motion_publish_stop(ctx,0x8c612f8au); r[0]=4u;goto L8c612f8c; /* mov #0x4,r0 */
L8c612f8c: sa_motion_publish_stop(ctx,0x8c612f8cu); fcopy(f,0u,6u);goto L8c612f8e; /* fmov fr6,fr0 */
L8c612f8e: sa_motion_publish_stop(ctx,0x8c612f8eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612f90; /* fmul fr7,fr3 */
L8c612f90: sa_motion_publish_stop(ctx,0x8c612f90u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c612f92; /* fmul fr5,fr0 */
L8c612f92: sa_motion_publish_stop(ctx,0x8c612f92u); fload(m,bus,&io_status,f,10u,r[3]);if(io_status)return io_status;goto L8c612f94; /* fmov.s @r3,fr10 */
L8c612f94: sa_motion_publish_stop(ctx,0x8c612f94u); fload(m,bus,&io_status,f,8u,r[14]);if(io_status)return io_status;goto L8c612f96; /* fmov.s @r14,fr8 */
L8c612f96: sa_motion_publish_stop(ctx,0x8c612f96u); r[5]+=24u;goto L8c612f98; /* add #0x18,r5 */
L8c612f98: sa_motion_publish_stop(ctx,0x8c612f98u); fcopy(f,2u,6u);goto L8c612f9a; /* fmov fr6,fr2 */
L8c612f9a: sa_motion_publish_stop(ctx,0x8c612f9au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612f9c; /* fmul fr8,fr2 */
L8c612f9c: sa_motion_publish_stop(ctx,0x8c612f9cu); fcopy(f,1u,8u);goto L8c612f9e; /* fmov fr8,fr1 */
L8c612f9e: sa_motion_publish_stop(ctx,0x8c612f9eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c612fa0; /* fsub fr3,fr0 */
L8c612fa0: sa_motion_publish_stop(ctx,0x8c612fa0u); fcopy(f,3u,10u);goto L8c612fa2; /* fmov fr10,fr3 */
L8c612fa2: sa_motion_publish_stop(ctx,0x8c612fa2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612fa4; /* fmul fr7,fr3 */
L8c612fa4: sa_motion_publish_stop(ctx,0x8c612fa4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[9],&f->fpscr,&fr[1]);if(status)return status;}goto L8c612fa6; /* fmul fr9,fr1 */
L8c612fa6: sa_motion_publish_stop(ctx,0x8c612fa6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612fa8; /* fsub fr2,fr3 */
L8c612fa8: sa_motion_publish_stop(ctx,0x8c612fa8u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c612faa; /* fmov.s @(r0,r5),fr2 */
L8c612faa: sa_motion_publish_stop(ctx,0x8c612faau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612fac; /* fmul fr3,fr2 */
L8c612fac: sa_motion_publish_stop(ctx,0x8c612facu); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c612fae; /* fmov.s @r5,fr3 */
L8c612fae: sa_motion_publish_stop(ctx,0x8c612faeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612fb0; /* fmac fr0,fr3,fr2 */
L8c612fb0: sa_motion_publish_stop(ctx,0x8c612fb0u); fcopy(f,11u,10u);goto L8c612fb2; /* fmov fr10,fr11 */
L8c612fb2: sa_motion_publish_stop(ctx,0x8c612fb2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[5],&f->fpscr,&fr[11]);if(status)return status;}goto L8c612fb4; /* fmul fr5,fr11 */
L8c612fb4: sa_motion_publish_stop(ctx,0x8c612fb4u); r[0]=8u;goto L8c612fb6; /* mov #0x8,r0 */
L8c612fb6: sa_motion_publish_stop(ctx,0x8c612fb6u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c612fb8; /* fmov.s @(r0,r5),fr3 */
L8c612fb8: sa_motion_publish_stop(ctx,0x8c612fb8u); r[5]=rd(m,bus,&io_status,0x8c6131d0u,4);if(io_status)return io_status;goto L8c612fba; /* mov.l 0x8c6131d0,r5 */
L8c612fba: sa_motion_publish_stop(ctx,0x8c612fbau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[11],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c612fbc; /* fsub fr11,fr1 */
L8c612fbc: sa_motion_publish_stop(ctx,0x8c612fbcu); r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c612fbe; /* mov.l @r5,r3 */
L8c612fbe: sa_motion_publish_stop(ctx,0x8c612fbeu); fcopy(f,0u,1u);goto L8c612fc0; /* fmov fr1,fr0 */
L8c612fc0: sa_motion_publish_stop(ctx,0x8c612fc0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612fc2; /* fmac fr0,fr3,fr2 */
L8c612fc2: sa_motion_publish_stop(ctx,0x8c612fc2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c612fc4; /* fldi0 fr3 */
L8c612fc4: sa_motion_publish_stop(ctx,0x8c612fc4u); fcopy(f,5u,2u);goto L8c612fc6; /* fmov fr2,fr5 */
L8c612fc6: sa_motion_publish_stop(ctx,0x8c612fc6u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c612fc8; /* fcmp/gt fr5,fr3 */
L8c612fc8: sa_motion_publish_stop(ctx,0x8c612fc8u); r[0]=*sr&1u;goto L8c612fca; /* movt r0 */
L8c612fca: sa_motion_publish_stop(ctx,0x8c612fcau); *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c612fcc; /* tst r3,r0 */
L8c612fcc: sa_motion_publish_stop(ctx,0x8c612fccu); if((*sr&1u))goto L8c612fd2;goto L8c612fce; /* bt 0x8c612fd2 */
L8c612fce: sa_motion_publish_stop(ctx,0x8c612fd0u); goto L8c613302; /* bra 0x8c613302 */
L8c612fd2: sa_motion_publish_stop(ctx,0x8c612fd2u); r[1]=0u;goto L8c612fd4; /* mov #0x0,r1 */
L8c612fd4: sa_motion_publish_stop(ctx,0x8c612fd4u); wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c612fd6; /* mov.l r1,@r5 */
L8c612fd6: sa_motion_publish_stop(ctx,0x8c612fd6u); r[14]=r[1];goto L8c612fd8; /* mov r1,r14 */
L8c612fd8: sa_motion_publish_stop(ctx,0x8c612fd8u); r[5]=rd(m,bus,&io_status,0x8c6131d4u,4);if(io_status)return io_status;goto L8c612fda; /* mov.l 0x8c6131d4,r5 */
L8c612fda: sa_motion_publish_stop(ctx,0x8c612fdau); r[12]=r[1];goto L8c612fdc; /* mov r1,r12 */
L8c612fdc: sa_motion_publish_stop(ctx,0x8c612fdcu); r[9]=2u;goto L8c612fde; /* mov #0x2,r9 */
L8c612fde: sa_motion_publish_stop(ctx,0x8c612fdeu); r[3]=r[9];goto L8c612fe0; /* mov r9,r3 */
L8c612fe0: sa_motion_publish_stop(ctx,0x8c612fe0u); r[0]=r[7];goto L8c612fe2; /* mov r7,r0 */
L8c612fe2: sa_motion_publish_stop(ctx,0x8c612fe2u); r[3]<<=2;goto L8c612fe4; /* shll2 r3 */
L8c612fe4: sa_motion_publish_stop(ctx,0x8c612fe4u); r[11]=r[14];goto L8c612fe6; /* mov r14,r11 */
L8c612fe6: sa_motion_publish_stop(ctx,0x8c612fe6u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c612fe8; /* mov.l r3,@r15 */
L8c612fe8: sa_motion_publish_stop(ctx,0x8c612fe8u); r[11]<<=2;goto L8c612fea; /* shll2 r11 */
L8c612fea: sa_motion_publish_stop(ctx,0x8c612feau); r[2]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c612fec; /* mov.l @(r0,r3),r2 */
L8c612fec: sa_motion_publish_stop(ctx,0x8c612fecu); r[3]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612fee; /* mov.l @(0xc,r2),r3 */
L8c612fee: sa_motion_publish_stop(ctx,0x8c612feeu); r[2]=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;goto L8c612ff0; /* mov.l @(r0,r11),r2 */
L8c612ff0: sa_motion_publish_stop(ctx,0x8c612ff0u); r[3]&=r[4];goto L8c612ff2; /* and r4,r3 */
L8c612ff2: sa_motion_publish_stop(ctx,0x8c612ff2u); if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[2]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612ff4; /* mov.l @(0xc,r2),r2 */
L8c612ff4: sa_motion_publish_stop(ctx,0x8c612ff4u); r[2]&=r[4];goto L8c612ff6; /* and r4,r2 */
L8c612ff6: sa_motion_publish_stop(ctx,0x8c612ff6u); r[3]^=r[2];goto L8c612ff8; /* xor r2,r3 */
L8c612ff8: sa_motion_publish_stop(ctx,0x8c612ff8u); r[0]=r[3];goto L8c612ffa; /* mov r3,r0 */
L8c612ffa: sa_motion_publish_stop(ctx,0x8c612ffau); *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c612ffc; /* cmp/eq #0x1,r0 */
L8c612ffc: sa_motion_publish_stop(ctx,0x8c612ffcu); if(!(*sr&1u))goto L8c6130ac;goto L8c612ffe; /* bf 0x8c6130ac */
L8c612ffe: sa_motion_publish_stop(ctx,0x8c612ffeu); r[3]=r[9];goto L8c613000; /* mov r9,r3 */
L8c613000: sa_motion_publish_stop(ctx,0x8c613000u); *sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c613002; /* shll r9 */
L8c613002: sa_motion_publish_stop(ctx,0x8c613002u); r[2]=r[14];goto L8c613004; /* mov r14,r2 */
L8c613004: sa_motion_publish_stop(ctx,0x8c613004u); r[9]+=r[3];goto L8c613006; /* add r3,r9 */
L8c613006: sa_motion_publish_stop(ctx,0x8c613006u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c613008; /* shll r2 */
L8c613008: sa_motion_publish_stop(ctx,0x8c613008u); r[1]=r[14];goto L8c61300a; /* mov r14,r1 */
L8c61300a: sa_motion_publish_stop(ctx,0x8c61300au); r[9]<<=2;goto L8c61300c; /* shll2 r9 */
L8c61300c: sa_motion_publish_stop(ctx,0x8c61300cu); r[3]=rd(m,bus,&io_status,0x8c6131d8u,4);if(io_status)return io_status;goto L8c61300e; /* mov.l 0x8c6131d8,r3 */
L8c61300e: sa_motion_publish_stop(ctx,0x8c61300eu); r[9]+=r[13];goto L8c613010; /* add r13,r9 */
L8c613010: sa_motion_publish_stop(ctx,0x8c613010u); r[0]=8u;goto L8c613012; /* mov #0x8,r0 */
L8c613012: sa_motion_publish_stop(ctx,0x8c613012u); wr(m,bus,&io_status,r[15]+0x8u,r[9],4);if(io_status)return io_status;goto L8c613014; /* mov.l r9,@(0x8,r15) */
L8c613014: sa_motion_publish_stop(ctx,0x8c613014u); r[2]+=r[1];goto L8c613016; /* add r1,r2 */
L8c613016: sa_motion_publish_stop(ctx,0x8c613016u); fload(m,bus,&io_status,f,5u,r[0]+r[9]);if(io_status)return io_status;goto L8c613018; /* fmov.s @(r0,r9),fr5 */
L8c613018: sa_motion_publish_stop(ctx,0x8c613018u); r[2]<<=2;goto L8c61301a; /* shll2 r2 */
L8c61301a: sa_motion_publish_stop(ctx,0x8c61301au); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61301c; /* fmov.s @r3,fr3 */
L8c61301c: sa_motion_publish_stop(ctx,0x8c61301cu); r[2]+=r[13];goto L8c61301e; /* add r13,r2 */
L8c61301e: sa_motion_publish_stop(ctx,0x8c61301eu); r[0]=8u;goto L8c613020; /* mov #0x8,r0 */
L8c613020: sa_motion_publish_stop(ctx,0x8c613020u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613022; /* fsub fr5,fr3 */
L8c613022: sa_motion_publish_stop(ctx,0x8c613022u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c613024; /* mov.l r2,@(0x4,r15) */
L8c613024: sa_motion_publish_stop(ctx,0x8c613024u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c613026; /* fmov.s @(r0,r2),fr2 */
L8c613026: sa_motion_publish_stop(ctx,0x8c613026u); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613028; /* mov.l @(0x4,r15),r1 */
L8c613028: sa_motion_publish_stop(ctx,0x8c613028u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61302a; /* fsub fr5,fr2 */
L8c61302a: sa_motion_publish_stop(ctx,0x8c61302au); fcopy(f,5u,3u);goto L8c61302c; /* fmov fr3,fr5 */
L8c61302c: sa_motion_publish_stop(ctx,0x8c61302cu); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61302e; /* mov.l @(0x8,r15),r2 */
L8c61302e: sa_motion_publish_stop(ctx,0x8c61302eu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c613030; /* fmov.s @r1,fr3 */
L8c613030: sa_motion_publish_stop(ctx,0x8c613030u); fload(m,bus,&io_status,f,6u,r[2]);if(io_status)return io_status;goto L8c613032; /* fmov.s @r2,fr6 */
L8c613032: sa_motion_publish_stop(ctx,0x8c613032u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[2],&f->fpscr,&fr[5]);if(status)return status;}goto L8c613034; /* fdiv fr2,fr5 */
L8c613034: sa_motion_publish_stop(ctx,0x8c613034u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613036; /* fsub fr6,fr3 */
L8c613036: sa_motion_publish_stop(ctx,0x8c613036u); fcopy(f,2u,6u);goto L8c613038; /* fmov fr6,fr2 */
L8c613038: sa_motion_publish_stop(ctx,0x8c613038u); fcopy(f,0u,5u);goto L8c61303a; /* fmov fr5,fr0 */
L8c61303a: sa_motion_publish_stop(ctx,0x8c61303au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61303c; /* fmac fr0,fr3,fr2 */
L8c61303c: sa_motion_publish_stop(ctx,0x8c61303cu); fcopy(f,6u,2u);goto L8c61303e; /* fmov fr2,fr6 */
L8c61303e: sa_motion_publish_stop(ctx,0x8c61303eu); r[2]=rd(m,bus,&io_status,0x8c6131dcu,4);if(io_status)return io_status;goto L8c613040; /* mov.l 0x8c6131dc,r2 */
L8c613040: sa_motion_publish_stop(ctx,0x8c613040u); r[0]=12u;goto L8c613042; /* mov #0xc,r0 */
L8c613042: sa_motion_publish_stop(ctx,0x8c613042u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c613044; /* fmov.s @(r0,r6),fr2 */
L8c613044: sa_motion_publish_stop(ctx,0x8c613044u); r[9]=r[12];goto L8c613046; /* mov r12,r9 */
L8c613046: sa_motion_publish_stop(ctx,0x8c613046u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c613048; /* fmov.s @r2,fr3 */
L8c613048: sa_motion_publish_stop(ctx,0x8c613048u); r[9]<<=2;goto L8c61304a; /* shll2 r9 */
L8c61304a: sa_motion_publish_stop(ctx,0x8c61304au); fcopy(f,0u,4u);goto L8c61304c; /* fmov fr4,fr0 */
L8c61304c: sa_motion_publish_stop(ctx,0x8c61304cu); r[9]<<=2;goto L8c61304e; /* shll2 r9 */
L8c61304e: sa_motion_publish_stop(ctx,0x8c61304eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c613050; /* fmul fr6,fr3 */
L8c613050: sa_motion_publish_stop(ctx,0x8c613050u); *sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c613052; /* shll r9 */
L8c613052: sa_motion_publish_stop(ctx,0x8c613052u); r[0]=16u;goto L8c613054; /* mov #0x10,r0 */
L8c613054: sa_motion_publish_stop(ctx,0x8c613054u); r[9]+=r[5];goto L8c613056; /* add r5,r9 */
L8c613056: sa_motion_publish_stop(ctx,0x8c613056u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613058; /* fmac fr0,fr3,fr2 */
L8c613058: sa_motion_publish_stop(ctx,0x8c613058u); fcopy(f,0u,5u);goto L8c61305a; /* fmov fr5,fr0 */
L8c61305a: sa_motion_publish_stop(ctx,0x8c61305au); fstore(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61305c; /* fmov fr2,@(r0,r9) */
L8c61305c: sa_motion_publish_stop(ctx,0x8c61305cu); r[0]=4u;goto L8c61305e; /* mov #0x4,r0 */
L8c61305e: sa_motion_publish_stop(ctx,0x8c61305eu); r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613060; /* mov.l @(0x8,r15),r1 */
L8c613060: sa_motion_publish_stop(ctx,0x8c613060u); fload(m,bus,&io_status,f,6u,r[0]+r[1]);if(io_status)return io_status;goto L8c613062; /* fmov.s @(r0,r1),fr6 */
L8c613062: sa_motion_publish_stop(ctx,0x8c613062u); r[0]=4u;goto L8c613064; /* mov #0x4,r0 */
L8c613064: sa_motion_publish_stop(ctx,0x8c613064u); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613066; /* mov.l @(0x4,r15),r1 */
L8c613066: sa_motion_publish_stop(ctx,0x8c613066u); fcopy(f,2u,6u);goto L8c613068; /* fmov fr6,fr2 */
L8c613068: sa_motion_publish_stop(ctx,0x8c613068u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61306a; /* fmov.s @(r0,r1),fr3 */
L8c61306a: sa_motion_publish_stop(ctx,0x8c61306au); r[0]=16u;goto L8c61306c; /* mov #0x10,r0 */
L8c61306c: sa_motion_publish_stop(ctx,0x8c61306cu); r[1]=rd(m,bus,&io_status,0x8c6131e0u,4);if(io_status)return io_status;goto L8c61306e; /* mov.l 0x8c6131e0,r1 */
L8c61306e: sa_motion_publish_stop(ctx,0x8c61306eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613070; /* fsub fr6,fr3 */
L8c613070: sa_motion_publish_stop(ctx,0x8c613070u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613072; /* fmac fr0,fr3,fr2 */
L8c613072: sa_motion_publish_stop(ctx,0x8c613072u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c613074; /* fmov.s @r1,fr3 */
L8c613074: sa_motion_publish_stop(ctx,0x8c613074u); fcopy(f,0u,4u);goto L8c613076; /* fmov fr4,fr0 */
L8c613076: sa_motion_publish_stop(ctx,0x8c613076u); fcopy(f,6u,2u);goto L8c613078; /* fmov fr2,fr6 */
L8c613078: sa_motion_publish_stop(ctx,0x8c613078u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61307a; /* fmul fr6,fr3 */
L8c61307a: sa_motion_publish_stop(ctx,0x8c61307au); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61307c; /* fmov.s @(r0,r6),fr2 */
L8c61307c: sa_motion_publish_stop(ctx,0x8c61307cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61307e; /* fmac fr0,fr3,fr2 */
L8c61307e: sa_motion_publish_stop(ctx,0x8c61307eu); r[0]=20u;goto L8c613080; /* mov #0x14,r0 */
L8c613080: sa_motion_publish_stop(ctx,0x8c613080u); fstore(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c613082; /* fmov fr2,@(r0,r9) */
L8c613082: sa_motion_publish_stop(ctx,0x8c613082u); r[0]=24u;goto L8c613084; /* mov #0x18,r0 */
L8c613084: sa_motion_publish_stop(ctx,0x8c613084u); fstore(m,bus,&io_status,f,0u,r[0]+r[9]);if(io_status)return io_status;goto L8c613086; /* fmov fr0,@(r0,r9) */
L8c613086: sa_motion_publish_stop(ctx,0x8c613086u); r[2]=28u;goto L8c613088; /* mov #0x1c,r2 */
L8c613088: sa_motion_publish_stop(ctx,0x8c613088u); r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61308a; /* mov.l @r15,r0 */
L8c61308a: sa_motion_publish_stop(ctx,0x8c61308au); r[12]+=1u;goto L8c61308c; /* add #0x1,r12 */
L8c61308c: sa_motion_publish_stop(ctx,0x8c61308cu); fcopy(f,0u,5u);goto L8c61308e; /* fmov fr5,fr0 */
L8c61308e: sa_motion_publish_stop(ctx,0x8c61308eu); r[3]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c613090; /* mov.l @(r0,r7),r3 */
L8c613090: sa_motion_publish_stop(ctx,0x8c613090u); r[0]=r[7];goto L8c613092; /* mov r7,r0 */
L8c613092: sa_motion_publish_stop(ctx,0x8c613092u); r[2]+=r[3];goto L8c613094; /* add r3,r2 */
L8c613094: sa_motion_publish_stop(ctx,0x8c613094u); wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c613096; /* mov.l r3,@(0xc,r15) */
L8c613096: sa_motion_publish_stop(ctx,0x8c613096u); r[3]=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;goto L8c613098; /* mov.l @(r0,r11),r3 */
L8c613098: sa_motion_publish_stop(ctx,0x8c613098u); fload(m,bus,&io_status,f,6u,r[2]);if(io_status)return io_status;goto L8c61309a; /* fmov.s @r2,fr6 */
L8c61309a: sa_motion_publish_stop(ctx,0x8c61309au); r[2]=28u;goto L8c61309c; /* mov #0x1c,r2 */
L8c61309c: sa_motion_publish_stop(ctx,0x8c61309cu); r[2]+=r[3];goto L8c61309e; /* add r3,r2 */
L8c61309e: sa_motion_publish_stop(ctx,0x8c61309eu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6130a0; /* fmov.s @r2,fr3 */
L8c6130a0: sa_motion_publish_stop(ctx,0x8c6130a0u); r[3]=28u;goto L8c6130a2; /* mov #0x1c,r3 */
L8c6130a2: sa_motion_publish_stop(ctx,0x8c6130a2u); fcopy(f,2u,6u);goto L8c6130a4; /* fmov fr6,fr2 */
L8c6130a4: sa_motion_publish_stop(ctx,0x8c6130a4u); r[3]+=r[9];goto L8c6130a6; /* add r9,r3 */
L8c6130a6: sa_motion_publish_stop(ctx,0x8c6130a6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6130a8; /* fsub fr6,fr3 */
L8c6130a8: sa_motion_publish_stop(ctx,0x8c6130a8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6130aa; /* fmac fr0,fr3,fr2 */
L8c6130aa: sa_motion_publish_stop(ctx,0x8c6130aau); fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6130ac; /* fmov.s fr2,@r3 */
L8c6130ac: sa_motion_publish_stop(ctx,0x8c6130acu); r[0]=r[11];goto L8c6130ae; /* mov r11,r0 */
L8c6130ae: sa_motion_publish_stop(ctx,0x8c6130aeu); r[3]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c6130b0; /* mov.l @(r0,r7),r3 */
L8c6130b0: sa_motion_publish_stop(ctx,0x8c6130b0u); r[3]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c6130b2; /* mov.l @(0xc,r3),r3 */
L8c6130b2: sa_motion_publish_stop(ctx,0x8c6130b2u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6130b4; /* tst r4,r3 */
L8c6130b4: sa_motion_publish_stop(ctx,0x8c6130b4u); if(!(*sr&1u))goto L8c6130ec;goto L8c6130b6; /* bf 0x8c6130ec */
L8c6130b6: sa_motion_publish_stop(ctx,0x8c6130b6u); r[11]+=r[7];goto L8c6130b8; /* add r7,r11 */
L8c6130b8: sa_motion_publish_stop(ctx,0x8c6130b8u); r[9]=r[12];goto L8c6130ba; /* mov r12,r9 */
L8c6130ba: sa_motion_publish_stop(ctx,0x8c6130bau); r[9]<<=2;goto L8c6130bc; /* shll2 r9 */
L8c6130bc: sa_motion_publish_stop(ctx,0x8c6130bcu); wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6130be; /* mov.l r11,@r15 */
L8c6130be: sa_motion_publish_stop(ctx,0x8c6130beu); r[3]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c6130c0; /* mov.l @r11,r3 */
L8c6130c0: sa_motion_publish_stop(ctx,0x8c6130c0u); r[0]=16u;goto L8c6130c2; /* mov #0x10,r0 */
L8c6130c2: sa_motion_publish_stop(ctx,0x8c6130c2u); r[9]<<=2;goto L8c6130c4; /* shll2 r9 */
L8c6130c4: sa_motion_publish_stop(ctx,0x8c6130c4u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6130c6; /* fmov.s @(r0,r3),fr3 */
L8c6130c6: sa_motion_publish_stop(ctx,0x8c6130c6u); *sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c6130c8; /* shll r9 */
L8c6130c8: sa_motion_publish_stop(ctx,0x8c6130c8u); r[9]+=r[5];goto L8c6130ca; /* add r5,r9 */
L8c6130ca: sa_motion_publish_stop(ctx,0x8c6130cau); fstore(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6130cc; /* fmov fr3,@(r0,r9) */
L8c6130cc: sa_motion_publish_stop(ctx,0x8c6130ccu); r[0]=20u;goto L8c6130ce; /* mov #0x14,r0 */
L8c6130ce: sa_motion_publish_stop(ctx,0x8c6130ceu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6130d0; /* mov.l @r15,r3 */
L8c6130d0: sa_motion_publish_stop(ctx,0x8c6130d0u); r[12]+=1u;goto L8c6130d2; /* add #0x1,r12 */
L8c6130d2: sa_motion_publish_stop(ctx,0x8c6130d2u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6130d4; /* mov.l @r3,r2 */
L8c6130d4: sa_motion_publish_stop(ctx,0x8c6130d4u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c6130d6; /* fmov.s @(r0,r2),fr3 */
L8c6130d6: sa_motion_publish_stop(ctx,0x8c6130d6u); fstore(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6130d8; /* fmov fr3,@(r0,r9) */
L8c6130d8: sa_motion_publish_stop(ctx,0x8c6130d8u); r[0]=24u;goto L8c6130da; /* mov #0x18,r0 */
L8c6130da: sa_motion_publish_stop(ctx,0x8c6130dau); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6130dc; /* mov.l @r15,r3 */
L8c6130dc: sa_motion_publish_stop(ctx,0x8c6130dcu); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6130de; /* mov.l @r3,r2 */
L8c6130de: sa_motion_publish_stop(ctx,0x8c6130deu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c6130e0; /* fmov.s @(r0,r2),fr3 */
L8c6130e0: sa_motion_publish_stop(ctx,0x8c6130e0u); fstore(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6130e2; /* fmov fr3,@(r0,r9) */
L8c6130e2: sa_motion_publish_stop(ctx,0x8c6130e2u); r[0]=28u;goto L8c6130e4; /* mov #0x1c,r0 */
L8c6130e4: sa_motion_publish_stop(ctx,0x8c6130e4u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6130e6; /* mov.l @r15,r3 */
L8c6130e6: sa_motion_publish_stop(ctx,0x8c6130e6u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6130e8; /* mov.l @r3,r2 */
L8c6130e8: sa_motion_publish_stop(ctx,0x8c6130e8u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c6130ea; /* fmov.s @(r0,r2),fr3 */
L8c6130ea: sa_motion_publish_stop(ctx,0x8c6130eau); fstore(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6130ec; /* fmov fr3,@(r0,r9) */
L8c6130ec: sa_motion_publish_stop(ctx,0x8c6130ecu); r[9]=r[14];goto L8c6130ee; /* mov r14,r9 */
L8c6130ee: sa_motion_publish_stop(ctx,0x8c6130eeu); r[14]+=1u;goto L8c6130f0; /* add #0x1,r14 */
L8c6130f0: sa_motion_publish_stop(ctx,0x8c6130f0u); *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[10]);goto L8c6130f2; /* cmp/ge r10,r14 */
L8c6130f2: sa_motion_publish_stop(ctx,0x8c6130f2u); if((*sr&1u))goto L8c6130f8;goto L8c6130f4; /* bt 0x8c6130f8 */
L8c6130f4: sa_motion_publish_stop(ctx,0x8c6130f6u); goto L8c612fde; /* bra 0x8c612fde */
L8c6130f8: sa_motion_publish_stop(ctx,0x8c6130f8u); r[4]=rd(m,bus,&io_status,0x8c6131ecu,4);if(io_status)return io_status;goto L8c6130fa; /* mov.l 0x8c6131ec,r4 */
L8c6130fa: sa_motion_publish_stop(ctx,0x8c6130fau); r[0]=r[12];goto L8c6130fc; /* mov r12,r0 */
L8c6130fc: sa_motion_publish_stop(ctx,0x8c6130fcu); r[14]=rd(m,bus,&io_status,0x8c6131e4u,4);if(io_status)return io_status;goto L8c6130fe; /* mov.l 0x8c6131e4,r14 */
L8c6130fe: sa_motion_publish_stop(ctx,0x8c6130feu); *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c613100; /* cmp/eq #0x3,r0 */
L8c613100: sa_motion_publish_stop(ctx,0x8c613100u); r[6]=rd(m,bus,&io_status,0x8c6131e8u,4);if(io_status)return io_status;goto L8c613102; /* mov.l 0x8c6131e8,r6 */
L8c613102: sa_motion_publish_stop(ctx,0x8c613102u); if(!(*sr&1u))goto L8c6131f4;goto L8c613104; /* bf 0x8c6131f4 */
L8c613104: sa_motion_publish_stop(ctx,0x8c613104u); r[0]=28u;goto L8c613106; /* mov #0x1c,r0 */
L8c613106: sa_motion_publish_stop(ctx,0x8c613106u); r[7]=r[5];goto L8c613108; /* mov r5,r7 */
L8c613108: sa_motion_publish_stop(ctx,0x8c613108u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61310a; /* fmov.s @(r0,r4),fr2 */
L8c61310a: sa_motion_publish_stop(ctx,0x8c61310au); r[8]+=32u;goto L8c61310c; /* add #0x20,r8 */
L8c61310c: sa_motion_publish_stop(ctx,0x8c61310cu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61310e; /* fmov.s @(r0,r7),fr3 */
L8c61310e: sa_motion_publish_stop(ctx,0x8c61310eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613110; /* fmul fr3,fr2 */
L8c613110: sa_motion_publish_stop(ctx,0x8c613110u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613112; /* fmov.s fr2,@-r8 */
L8c613112: sa_motion_publish_stop(ctx,0x8c613112u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613114; /* fmov.s @(r0,r7),fr3 */
L8c613114: sa_motion_publish_stop(ctx,0x8c613114u); r[0]=24u;goto L8c613116; /* mov #0x18,r0 */
L8c613116: sa_motion_publish_stop(ctx,0x8c613116u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613118; /* fmov.s @(r0,r4),fr2 */
L8c613118: sa_motion_publish_stop(ctx,0x8c613118u); r[0]=28u;goto L8c61311a; /* mov #0x1c,r0 */
L8c61311a: sa_motion_publish_stop(ctx,0x8c61311au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61311c; /* fmul fr3,fr2 */
L8c61311c: sa_motion_publish_stop(ctx,0x8c61311cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61311e; /* fmov.s fr2,@-r8 */
L8c61311e: sa_motion_publish_stop(ctx,0x8c61311eu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613120; /* fmov.s @(r0,r7),fr3 */
L8c613120: sa_motion_publish_stop(ctx,0x8c613120u); r[0]=20u;goto L8c613122; /* mov #0x14,r0 */
L8c613122: sa_motion_publish_stop(ctx,0x8c613122u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613124; /* fmov.s @(r0,r4),fr2 */
L8c613124: sa_motion_publish_stop(ctx,0x8c613124u); r[0]=16u;goto L8c613126; /* mov #0x10,r0 */
L8c613126: sa_motion_publish_stop(ctx,0x8c613126u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613128; /* fmul fr3,fr2 */
L8c613128: sa_motion_publish_stop(ctx,0x8c613128u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61312a; /* fmov.s fr2,@-r8 */
L8c61312a: sa_motion_publish_stop(ctx,0x8c61312au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61312c; /* fmov.s @(r0,r4),fr3 */
L8c61312c: sa_motion_publish_stop(ctx,0x8c61312cu); r[0]=24u;goto L8c61312e; /* mov #0x18,r0 */
L8c61312e: sa_motion_publish_stop(ctx,0x8c61312eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613130; /* fmov.s fr3,@-r8 */
L8c613130: sa_motion_publish_stop(ctx,0x8c613130u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613132; /* fmov.s @(r0,r7),fr3 */
L8c613132: sa_motion_publish_stop(ctx,0x8c613132u); r[0]=20u;goto L8c613134; /* mov #0x14,r0 */
L8c613134: sa_motion_publish_stop(ctx,0x8c613134u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613136; /* fmov.s fr3,@-r8 */
L8c613136: sa_motion_publish_stop(ctx,0x8c613136u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613138; /* fmov.s @(r0,r7),fr3 */
L8c613138: sa_motion_publish_stop(ctx,0x8c613138u); r[0]=16u;goto L8c61313a; /* mov #0x10,r0 */
L8c61313a: sa_motion_publish_stop(ctx,0x8c61313au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61313c; /* fmov.s fr3,@-r8 */
L8c61313c: sa_motion_publish_stop(ctx,0x8c61313cu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61313e; /* fmov.s @(r0,r7),fr3 */
L8c61313e: sa_motion_publish_stop(ctx,0x8c61313eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613140; /* fmov.s fr3,@-r8 */
L8c613140: sa_motion_publish_stop(ctx,0x8c613140u); r[8]-=4;wr(m,bus,&io_status,r[8],r[6],4);if(io_status)return io_status;goto L8c613142; /* mov.l r6,@-r8 */
L8c613142: sa_motion_publish_stop(ctx,0x8c613142u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613144; /* pref @r8 */
L8c613144: sa_motion_publish_stop(ctx,0x8c613144u); r[0]=28u;goto L8c613146; /* mov #0x1c,r0 */
L8c613146: sa_motion_publish_stop(ctx,0x8c613146u); r[7]+=32u;goto L8c613148; /* add #0x20,r7 */
L8c613148: sa_motion_publish_stop(ctx,0x8c613148u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61314a; /* fmov.s @(r0,r4),fr2 */
L8c61314a: sa_motion_publish_stop(ctx,0x8c61314au); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61314c; /* fmov.s @(r0,r7),fr3 */
L8c61314c: sa_motion_publish_stop(ctx,0x8c61314cu); r[8]+=64u;goto L8c61314e; /* add #0x40,r8 */
L8c61314e: sa_motion_publish_stop(ctx,0x8c61314eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613150; /* fmul fr3,fr2 */
L8c613150: sa_motion_publish_stop(ctx,0x8c613150u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613152; /* fmov.s fr2,@-r8 */
L8c613152: sa_motion_publish_stop(ctx,0x8c613152u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613154; /* fmov.s @(r0,r7),fr3 */
L8c613154: sa_motion_publish_stop(ctx,0x8c613154u); r[0]=24u;goto L8c613156; /* mov #0x18,r0 */
L8c613156: sa_motion_publish_stop(ctx,0x8c613156u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613158; /* fmov.s @(r0,r4),fr2 */
L8c613158: sa_motion_publish_stop(ctx,0x8c613158u); r[0]=28u;goto L8c61315a; /* mov #0x1c,r0 */
L8c61315a: sa_motion_publish_stop(ctx,0x8c61315au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61315c; /* fmul fr3,fr2 */
L8c61315c: sa_motion_publish_stop(ctx,0x8c61315cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61315e; /* fmov.s fr2,@-r8 */
L8c61315e: sa_motion_publish_stop(ctx,0x8c61315eu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613160; /* fmov.s @(r0,r7),fr3 */
L8c613160: sa_motion_publish_stop(ctx,0x8c613160u); r[0]=20u;goto L8c613162; /* mov #0x14,r0 */
L8c613162: sa_motion_publish_stop(ctx,0x8c613162u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613164; /* fmov.s @(r0,r4),fr2 */
L8c613164: sa_motion_publish_stop(ctx,0x8c613164u); r[0]=16u;goto L8c613166; /* mov #0x10,r0 */
L8c613166: sa_motion_publish_stop(ctx,0x8c613166u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613168; /* fmul fr3,fr2 */
L8c613168: sa_motion_publish_stop(ctx,0x8c613168u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61316a; /* fmov.s fr2,@-r8 */
L8c61316a: sa_motion_publish_stop(ctx,0x8c61316au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61316c; /* fmov.s @(r0,r4),fr3 */
L8c61316c: sa_motion_publish_stop(ctx,0x8c61316cu); r[0]=24u;goto L8c61316e; /* mov #0x18,r0 */
L8c61316e: sa_motion_publish_stop(ctx,0x8c61316eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613170; /* fmov.s fr3,@-r8 */
L8c613170: sa_motion_publish_stop(ctx,0x8c613170u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613172; /* fmov.s @(r0,r7),fr3 */
L8c613172: sa_motion_publish_stop(ctx,0x8c613172u); r[0]=20u;goto L8c613174; /* mov #0x14,r0 */
L8c613174: sa_motion_publish_stop(ctx,0x8c613174u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613176; /* fmov.s fr3,@-r8 */
L8c613176: sa_motion_publish_stop(ctx,0x8c613176u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613178; /* fmov.s @(r0,r7),fr3 */
L8c613178: sa_motion_publish_stop(ctx,0x8c613178u); r[0]=16u;goto L8c61317a; /* mov #0x10,r0 */
L8c61317a: sa_motion_publish_stop(ctx,0x8c61317au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61317c; /* fmov.s fr3,@-r8 */
L8c61317c: sa_motion_publish_stop(ctx,0x8c61317cu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61317e; /* fmov.s @(r0,r7),fr3 */
L8c61317e: sa_motion_publish_stop(ctx,0x8c61317eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613180; /* fmov.s fr3,@-r8 */
L8c613180: sa_motion_publish_stop(ctx,0x8c613180u); r[8]-=4;wr(m,bus,&io_status,r[8],r[6],4);if(io_status)return io_status;goto L8c613182; /* mov.l r6,@-r8 */
L8c613182: sa_motion_publish_stop(ctx,0x8c613182u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613184; /* pref @r8 */
L8c613184: sa_motion_publish_stop(ctx,0x8c613184u); r[6]=r[5];goto L8c613186; /* mov r5,r6 */
L8c613186: sa_motion_publish_stop(ctx,0x8c613186u); r[0]=28u;goto L8c613188; /* mov #0x1c,r0 */
L8c613188: sa_motion_publish_stop(ctx,0x8c613188u); r[6]+=64u;goto L8c61318a; /* add #0x40,r6 */
L8c61318a: sa_motion_publish_stop(ctx,0x8c61318au); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61318c; /* fmov.s @(r0,r4),fr2 */
L8c61318c: sa_motion_publish_stop(ctx,0x8c61318cu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61318e; /* fmov.s @(r0,r6),fr3 */
L8c61318e: sa_motion_publish_stop(ctx,0x8c61318eu); r[8]+=64u;goto L8c613190; /* add #0x40,r8 */
L8c613190: sa_motion_publish_stop(ctx,0x8c613190u); r[3]=rd(m,bus,&io_status,0x8c6131f0u,4);if(io_status)return io_status;goto L8c613192; /* mov.l 0x8c6131f0,r3 */
L8c613192: sa_motion_publish_stop(ctx,0x8c613192u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613194; /* fmul fr3,fr2 */
L8c613194: sa_motion_publish_stop(ctx,0x8c613194u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613196; /* fmov.s fr2,@-r8 */
L8c613196: sa_motion_publish_stop(ctx,0x8c613196u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c613198; /* fmov.s @(r0,r6),fr3 */
L8c613198: sa_motion_publish_stop(ctx,0x8c613198u); r[0]=24u;goto L8c61319a; /* mov #0x18,r0 */
L8c61319a: sa_motion_publish_stop(ctx,0x8c61319au); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61319c; /* fmov.s @(r0,r4),fr2 */
L8c61319c: sa_motion_publish_stop(ctx,0x8c61319cu); r[0]=28u;goto L8c61319e; /* mov #0x1c,r0 */
L8c61319e: sa_motion_publish_stop(ctx,0x8c61319eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6131a0; /* fmul fr3,fr2 */
L8c6131a0: sa_motion_publish_stop(ctx,0x8c6131a0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6131a2; /* fmov.s fr2,@-r8 */
L8c6131a2: sa_motion_publish_stop(ctx,0x8c6131a2u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6131a4; /* fmov.s @(r0,r6),fr3 */
L8c6131a4: sa_motion_publish_stop(ctx,0x8c6131a4u); r[0]=20u;goto L8c6131a6; /* mov #0x14,r0 */
L8c6131a6: sa_motion_publish_stop(ctx,0x8c6131a6u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6131a8; /* fmov.s @(r0,r4),fr2 */
L8c6131a8: sa_motion_publish_stop(ctx,0x8c6131a8u); r[0]=16u;goto L8c6131aa; /* mov #0x10,r0 */
L8c6131aa: sa_motion_publish_stop(ctx,0x8c6131aau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6131ac; /* fmul fr3,fr2 */
L8c6131ac: sa_motion_publish_stop(ctx,0x8c6131acu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6131ae; /* fmov.s fr2,@-r8 */
L8c6131ae: sa_motion_publish_stop(ctx,0x8c6131aeu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6131b0; /* fmov.s @(r0,r4),fr3 */
L8c6131b0: sa_motion_publish_stop(ctx,0x8c6131b0u); r[0]=24u;goto L8c6131b2; /* mov #0x18,r0 */
L8c6131b2: sa_motion_publish_stop(ctx,0x8c6131b2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6131b4; /* fmov.s fr3,@-r8 */
L8c6131b4: sa_motion_publish_stop(ctx,0x8c6131b4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6131b6; /* fmov.s @(r0,r6),fr3 */
L8c6131b6: sa_motion_publish_stop(ctx,0x8c6131b6u); r[0]=20u;goto L8c6131b8; /* mov #0x14,r0 */
L8c6131b8: sa_motion_publish_stop(ctx,0x8c6131b8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6131ba; /* fmov.s fr3,@-r8 */
L8c6131ba: sa_motion_publish_stop(ctx,0x8c6131bau); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6131bc; /* fmov.s @(r0,r6),fr3 */
L8c6131bc: sa_motion_publish_stop(ctx,0x8c6131bcu); r[0]=16u;goto L8c6131be; /* mov #0x10,r0 */
L8c6131be: sa_motion_publish_stop(ctx,0x8c6131beu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6131c0; /* fmov.s fr3,@-r8 */
L8c6131c0: sa_motion_publish_stop(ctx,0x8c6131c0u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6131c2; /* fmov.s @(r0,r6),fr3 */
L8c6131c2: sa_motion_publish_stop(ctx,0x8c6131c2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6131c4; /* fmov.s fr3,@-r8 */
L8c6131c4: sa_motion_publish_stop(ctx,0x8c6131c4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6131c6; /* mov.l r3,@-r8 */
L8c6131c6: sa_motion_publish_stop(ctx,0x8c6131c6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6131c8; /* pref @r8 */
L8c6131c8: sa_motion_publish_stop(ctx,0x8c6131c8u); r[2]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c6131ca; /* mov.l @r14,r2 */
L8c6131ca: sa_motion_publish_stop(ctx,0x8c6131cau); r[8]+=32u;goto L8c6131cc; /* add #0x20,r8 */
L8c6131cc: sa_motion_publish_stop(ctx,0x8c6131ceu); r[2]+=3u;goto L8c613300; /* bra 0x8c613300 */
L8c6131f4: sa_motion_publish_stop(ctx,0x8c6131f4u); r[0]=28u;goto L8c6131f6; /* mov #0x1c,r0 */
L8c6131f6: sa_motion_publish_stop(ctx,0x8c6131f6u); r[7]=r[5];goto L8c6131f8; /* mov r5,r7 */
L8c6131f8: sa_motion_publish_stop(ctx,0x8c6131f8u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6131fa; /* fmov.s @(r0,r4),fr2 */
L8c6131fa: sa_motion_publish_stop(ctx,0x8c6131fau); r[8]+=32u;goto L8c6131fc; /* add #0x20,r8 */
L8c6131fc: sa_motion_publish_stop(ctx,0x8c6131fcu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6131fe; /* fmov.s @(r0,r7),fr3 */
L8c6131fe: sa_motion_publish_stop(ctx,0x8c6131feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613200; /* fmul fr3,fr2 */
L8c613200: sa_motion_publish_stop(ctx,0x8c613200u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613202; /* fmov.s fr2,@-r8 */
L8c613202: sa_motion_publish_stop(ctx,0x8c613202u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613204; /* fmov.s @(r0,r7),fr3 */
L8c613204: sa_motion_publish_stop(ctx,0x8c613204u); r[0]=24u;goto L8c613206; /* mov #0x18,r0 */
L8c613206: sa_motion_publish_stop(ctx,0x8c613206u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613208; /* fmov.s @(r0,r4),fr2 */
L8c613208: sa_motion_publish_stop(ctx,0x8c613208u); r[0]=28u;goto L8c61320a; /* mov #0x1c,r0 */
L8c61320a: sa_motion_publish_stop(ctx,0x8c61320au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61320c; /* fmul fr3,fr2 */
L8c61320c: sa_motion_publish_stop(ctx,0x8c61320cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61320e; /* fmov.s fr2,@-r8 */
L8c61320e: sa_motion_publish_stop(ctx,0x8c61320eu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613210; /* fmov.s @(r0,r7),fr3 */
L8c613210: sa_motion_publish_stop(ctx,0x8c613210u); r[0]=20u;goto L8c613212; /* mov #0x14,r0 */
L8c613212: sa_motion_publish_stop(ctx,0x8c613212u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613214; /* fmov.s @(r0,r4),fr2 */
L8c613214: sa_motion_publish_stop(ctx,0x8c613214u); r[0]=16u;goto L8c613216; /* mov #0x10,r0 */
L8c613216: sa_motion_publish_stop(ctx,0x8c613216u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613218; /* fmul fr3,fr2 */
L8c613218: sa_motion_publish_stop(ctx,0x8c613218u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61321a; /* fmov.s fr2,@-r8 */
L8c61321a: sa_motion_publish_stop(ctx,0x8c61321au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61321c; /* fmov.s @(r0,r4),fr3 */
L8c61321c: sa_motion_publish_stop(ctx,0x8c61321cu); r[0]=24u;goto L8c61321e; /* mov #0x18,r0 */
L8c61321e: sa_motion_publish_stop(ctx,0x8c61321eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613220; /* fmov.s fr3,@-r8 */
L8c613220: sa_motion_publish_stop(ctx,0x8c613220u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613222; /* fmov.s @(r0,r7),fr3 */
L8c613222: sa_motion_publish_stop(ctx,0x8c613222u); r[0]=20u;goto L8c613224; /* mov #0x14,r0 */
L8c613224: sa_motion_publish_stop(ctx,0x8c613224u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613226; /* fmov.s fr3,@-r8 */
L8c613226: sa_motion_publish_stop(ctx,0x8c613226u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613228; /* fmov.s @(r0,r7),fr3 */
L8c613228: sa_motion_publish_stop(ctx,0x8c613228u); r[0]=16u;goto L8c61322a; /* mov #0x10,r0 */
L8c61322a: sa_motion_publish_stop(ctx,0x8c61322au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61322c; /* fmov.s fr3,@-r8 */
L8c61322c: sa_motion_publish_stop(ctx,0x8c61322cu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61322e; /* fmov.s @(r0,r7),fr3 */
L8c61322e: sa_motion_publish_stop(ctx,0x8c61322eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613230; /* fmov.s fr3,@-r8 */
L8c613230: sa_motion_publish_stop(ctx,0x8c613230u); r[8]-=4;wr(m,bus,&io_status,r[8],r[6],4);if(io_status)return io_status;goto L8c613232; /* mov.l r6,@-r8 */
L8c613232: sa_motion_publish_stop(ctx,0x8c613232u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613234; /* pref @r8 */
L8c613234: sa_motion_publish_stop(ctx,0x8c613234u); r[0]=28u;goto L8c613236; /* mov #0x1c,r0 */
L8c613236: sa_motion_publish_stop(ctx,0x8c613236u); r[7]+=32u;goto L8c613238; /* add #0x20,r7 */
L8c613238: sa_motion_publish_stop(ctx,0x8c613238u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61323a; /* fmov.s @(r0,r4),fr2 */
L8c61323a: sa_motion_publish_stop(ctx,0x8c61323au); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61323c; /* fmov.s @(r0,r7),fr3 */
L8c61323c: sa_motion_publish_stop(ctx,0x8c61323cu); r[8]+=64u;goto L8c61323e; /* add #0x40,r8 */
L8c61323e: sa_motion_publish_stop(ctx,0x8c61323eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613240; /* fmul fr3,fr2 */
L8c613240: sa_motion_publish_stop(ctx,0x8c613240u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613242; /* fmov.s fr2,@-r8 */
L8c613242: sa_motion_publish_stop(ctx,0x8c613242u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613244; /* fmov.s @(r0,r7),fr3 */
L8c613244: sa_motion_publish_stop(ctx,0x8c613244u); r[0]=24u;goto L8c613246; /* mov #0x18,r0 */
L8c613246: sa_motion_publish_stop(ctx,0x8c613246u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613248; /* fmov.s @(r0,r4),fr2 */
L8c613248: sa_motion_publish_stop(ctx,0x8c613248u); r[0]=28u;goto L8c61324a; /* mov #0x1c,r0 */
L8c61324a: sa_motion_publish_stop(ctx,0x8c61324au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61324c; /* fmul fr3,fr2 */
L8c61324c: sa_motion_publish_stop(ctx,0x8c61324cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61324e; /* fmov.s fr2,@-r8 */
L8c61324e: sa_motion_publish_stop(ctx,0x8c61324eu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613250; /* fmov.s @(r0,r7),fr3 */
L8c613250: sa_motion_publish_stop(ctx,0x8c613250u); r[0]=20u;goto L8c613252; /* mov #0x14,r0 */
L8c613252: sa_motion_publish_stop(ctx,0x8c613252u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613254; /* fmov.s @(r0,r4),fr2 */
L8c613254: sa_motion_publish_stop(ctx,0x8c613254u); r[0]=16u;goto L8c613256; /* mov #0x10,r0 */
L8c613256: sa_motion_publish_stop(ctx,0x8c613256u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613258; /* fmul fr3,fr2 */
L8c613258: sa_motion_publish_stop(ctx,0x8c613258u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61325a; /* fmov.s fr2,@-r8 */
L8c61325a: sa_motion_publish_stop(ctx,0x8c61325au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61325c; /* fmov.s @(r0,r4),fr3 */
L8c61325c: sa_motion_publish_stop(ctx,0x8c61325cu); r[0]=24u;goto L8c61325e; /* mov #0x18,r0 */
L8c61325e: sa_motion_publish_stop(ctx,0x8c61325eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613260; /* fmov.s fr3,@-r8 */
L8c613260: sa_motion_publish_stop(ctx,0x8c613260u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613262; /* fmov.s @(r0,r7),fr3 */
L8c613262: sa_motion_publish_stop(ctx,0x8c613262u); r[0]=20u;goto L8c613264; /* mov #0x14,r0 */
L8c613264: sa_motion_publish_stop(ctx,0x8c613264u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613266; /* fmov.s fr3,@-r8 */
L8c613266: sa_motion_publish_stop(ctx,0x8c613266u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613268; /* fmov.s @(r0,r7),fr3 */
L8c613268: sa_motion_publish_stop(ctx,0x8c613268u); r[0]=16u;goto L8c61326a; /* mov #0x10,r0 */
L8c61326a: sa_motion_publish_stop(ctx,0x8c61326au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61326c; /* fmov.s fr3,@-r8 */
L8c61326c: sa_motion_publish_stop(ctx,0x8c61326cu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61326e; /* fmov.s @(r0,r7),fr3 */
L8c61326e: sa_motion_publish_stop(ctx,0x8c61326eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613270; /* fmov.s fr3,@-r8 */
L8c613270: sa_motion_publish_stop(ctx,0x8c613270u); r[8]-=4;wr(m,bus,&io_status,r[8],r[6],4);if(io_status)return io_status;goto L8c613272; /* mov.l r6,@-r8 */
L8c613272: sa_motion_publish_stop(ctx,0x8c613272u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613274; /* pref @r8 */
L8c613274: sa_motion_publish_stop(ctx,0x8c613274u); r[7]=r[5];goto L8c613276; /* mov r5,r7 */
L8c613276: sa_motion_publish_stop(ctx,0x8c613276u); r[0]=28u;goto L8c613278; /* mov #0x1c,r0 */
L8c613278: sa_motion_publish_stop(ctx,0x8c613278u); r[7]+=96u;goto L8c61327a; /* add #0x60,r7 */
L8c61327a: sa_motion_publish_stop(ctx,0x8c61327au); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61327c; /* fmov.s @(r0,r4),fr2 */
L8c61327c: sa_motion_publish_stop(ctx,0x8c61327cu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61327e; /* fmov.s @(r0,r7),fr3 */
L8c61327e: sa_motion_publish_stop(ctx,0x8c61327eu); r[8]+=64u;goto L8c613280; /* add #0x40,r8 */
L8c613280: sa_motion_publish_stop(ctx,0x8c613280u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613282; /* fmul fr3,fr2 */
L8c613282: sa_motion_publish_stop(ctx,0x8c613282u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613284; /* fmov.s fr2,@-r8 */
L8c613284: sa_motion_publish_stop(ctx,0x8c613284u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613286; /* fmov.s @(r0,r7),fr3 */
L8c613286: sa_motion_publish_stop(ctx,0x8c613286u); r[0]=24u;goto L8c613288; /* mov #0x18,r0 */
L8c613288: sa_motion_publish_stop(ctx,0x8c613288u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61328a; /* fmov.s @(r0,r4),fr2 */
L8c61328a: sa_motion_publish_stop(ctx,0x8c61328au); r[0]=28u;goto L8c61328c; /* mov #0x1c,r0 */
L8c61328c: sa_motion_publish_stop(ctx,0x8c61328cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61328e; /* fmul fr3,fr2 */
L8c61328e: sa_motion_publish_stop(ctx,0x8c61328eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613290; /* fmov.s fr2,@-r8 */
L8c613290: sa_motion_publish_stop(ctx,0x8c613290u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c613292; /* fmov.s @(r0,r7),fr3 */
L8c613292: sa_motion_publish_stop(ctx,0x8c613292u); r[0]=20u;goto L8c613294; /* mov #0x14,r0 */
L8c613294: sa_motion_publish_stop(ctx,0x8c613294u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c613296; /* fmov.s @(r0,r4),fr2 */
L8c613296: sa_motion_publish_stop(ctx,0x8c613296u); r[0]=16u;goto L8c613298; /* mov #0x10,r0 */
L8c613298: sa_motion_publish_stop(ctx,0x8c613298u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61329a; /* fmul fr3,fr2 */
L8c61329a: sa_motion_publish_stop(ctx,0x8c61329au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61329c; /* fmov.s fr2,@-r8 */
L8c61329c: sa_motion_publish_stop(ctx,0x8c61329cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61329e; /* fmov.s @(r0,r4),fr3 */
L8c61329e: sa_motion_publish_stop(ctx,0x8c61329eu); r[0]=24u;goto L8c6132a0; /* mov #0x18,r0 */
L8c6132a0: sa_motion_publish_stop(ctx,0x8c6132a0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132a2; /* fmov.s fr3,@-r8 */
L8c6132a2: sa_motion_publish_stop(ctx,0x8c6132a2u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6132a4; /* fmov.s @(r0,r7),fr3 */
L8c6132a4: sa_motion_publish_stop(ctx,0x8c6132a4u); r[0]=20u;goto L8c6132a6; /* mov #0x14,r0 */
L8c6132a6: sa_motion_publish_stop(ctx,0x8c6132a6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132a8; /* fmov.s fr3,@-r8 */
L8c6132a8: sa_motion_publish_stop(ctx,0x8c6132a8u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6132aa; /* fmov.s @(r0,r7),fr3 */
L8c6132aa: sa_motion_publish_stop(ctx,0x8c6132aau); r[0]=16u;goto L8c6132ac; /* mov #0x10,r0 */
L8c6132ac: sa_motion_publish_stop(ctx,0x8c6132acu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132ae; /* fmov.s fr3,@-r8 */
L8c6132ae: sa_motion_publish_stop(ctx,0x8c6132aeu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6132b0; /* fmov.s @(r0,r7),fr3 */
L8c6132b0: sa_motion_publish_stop(ctx,0x8c6132b0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132b2; /* fmov.s fr3,@-r8 */
L8c6132b2: sa_motion_publish_stop(ctx,0x8c6132b2u); r[8]-=4;wr(m,bus,&io_status,r[8],r[6],4);if(io_status)return io_status;goto L8c6132b4; /* mov.l r6,@-r8 */
L8c6132b4: sa_motion_publish_stop(ctx,0x8c6132b4u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6132b6; /* pref @r8 */
L8c6132b6: sa_motion_publish_stop(ctx,0x8c6132b6u); r[6]=r[5];goto L8c6132b8; /* mov r5,r6 */
L8c6132b8: sa_motion_publish_stop(ctx,0x8c6132b8u); r[0]=28u;goto L8c6132ba; /* mov #0x1c,r0 */
L8c6132ba: sa_motion_publish_stop(ctx,0x8c6132bau); r[6]+=64u;goto L8c6132bc; /* add #0x40,r6 */
L8c6132bc: sa_motion_publish_stop(ctx,0x8c6132bcu); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6132be; /* fmov.s @(r0,r4),fr2 */
L8c6132be: sa_motion_publish_stop(ctx,0x8c6132beu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6132c0; /* fmov.s @(r0,r6),fr3 */
L8c6132c0: sa_motion_publish_stop(ctx,0x8c6132c0u); r[8]+=64u;goto L8c6132c2; /* add #0x40,r8 */
L8c6132c2: sa_motion_publish_stop(ctx,0x8c6132c2u); r[3]=rd(m,bus,&io_status,0x8c6133a0u,4);if(io_status)return io_status;goto L8c6132c4; /* mov.l 0x8c6133a0,r3 */
L8c6132c4: sa_motion_publish_stop(ctx,0x8c6132c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6132c6; /* fmul fr3,fr2 */
L8c6132c6: sa_motion_publish_stop(ctx,0x8c6132c6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6132c8; /* fmov.s fr2,@-r8 */
L8c6132c8: sa_motion_publish_stop(ctx,0x8c6132c8u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6132ca; /* fmov.s @(r0,r6),fr3 */
L8c6132ca: sa_motion_publish_stop(ctx,0x8c6132cau); r[0]=24u;goto L8c6132cc; /* mov #0x18,r0 */
L8c6132cc: sa_motion_publish_stop(ctx,0x8c6132ccu); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6132ce; /* fmov.s @(r0,r4),fr2 */
L8c6132ce: sa_motion_publish_stop(ctx,0x8c6132ceu); r[0]=28u;goto L8c6132d0; /* mov #0x1c,r0 */
L8c6132d0: sa_motion_publish_stop(ctx,0x8c6132d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6132d2; /* fmul fr3,fr2 */
L8c6132d2: sa_motion_publish_stop(ctx,0x8c6132d2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6132d4; /* fmov.s fr2,@-r8 */
L8c6132d4: sa_motion_publish_stop(ctx,0x8c6132d4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6132d6; /* fmov.s @(r0,r6),fr3 */
L8c6132d6: sa_motion_publish_stop(ctx,0x8c6132d6u); r[0]=20u;goto L8c6132d8; /* mov #0x14,r0 */
L8c6132d8: sa_motion_publish_stop(ctx,0x8c6132d8u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6132da; /* fmov.s @(r0,r4),fr2 */
L8c6132da: sa_motion_publish_stop(ctx,0x8c6132dau); r[0]=16u;goto L8c6132dc; /* mov #0x10,r0 */
L8c6132dc: sa_motion_publish_stop(ctx,0x8c6132dcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6132de; /* fmul fr3,fr2 */
L8c6132de: sa_motion_publish_stop(ctx,0x8c6132deu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6132e0; /* fmov.s fr2,@-r8 */
L8c6132e0: sa_motion_publish_stop(ctx,0x8c6132e0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6132e2; /* fmov.s @(r0,r4),fr3 */
L8c6132e2: sa_motion_publish_stop(ctx,0x8c6132e2u); r[0]=24u;goto L8c6132e4; /* mov #0x18,r0 */
L8c6132e4: sa_motion_publish_stop(ctx,0x8c6132e4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132e6; /* fmov.s fr3,@-r8 */
L8c6132e6: sa_motion_publish_stop(ctx,0x8c6132e6u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6132e8; /* fmov.s @(r0,r6),fr3 */
L8c6132e8: sa_motion_publish_stop(ctx,0x8c6132e8u); r[0]=20u;goto L8c6132ea; /* mov #0x14,r0 */
L8c6132ea: sa_motion_publish_stop(ctx,0x8c6132eau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132ec; /* fmov.s fr3,@-r8 */
L8c6132ec: sa_motion_publish_stop(ctx,0x8c6132ecu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6132ee; /* fmov.s @(r0,r6),fr3 */
L8c6132ee: sa_motion_publish_stop(ctx,0x8c6132eeu); r[0]=16u;goto L8c6132f0; /* mov #0x10,r0 */
L8c6132f0: sa_motion_publish_stop(ctx,0x8c6132f0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132f2; /* fmov.s fr3,@-r8 */
L8c6132f2: sa_motion_publish_stop(ctx,0x8c6132f2u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6132f4; /* fmov.s @(r0,r6),fr3 */
L8c6132f4: sa_motion_publish_stop(ctx,0x8c6132f4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6132f6; /* fmov.s fr3,@-r8 */
L8c6132f6: sa_motion_publish_stop(ctx,0x8c6132f6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6132f8; /* mov.l r3,@-r8 */
L8c6132f8: sa_motion_publish_stop(ctx,0x8c6132f8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6132fa; /* pref @r8 */
L8c6132fa: sa_motion_publish_stop(ctx,0x8c6132fau); r[2]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c6132fc; /* mov.l @r14,r2 */
L8c6132fc: sa_motion_publish_stop(ctx,0x8c6132fcu); r[8]+=32u;goto L8c6132fe; /* add #0x20,r8 */
L8c6132fe: sa_motion_publish_stop(ctx,0x8c6132feu); r[2]+=4u;goto L8c613300; /* add #0x4,r2 */
L8c613300: sa_motion_publish_stop(ctx,0x8c613300u); wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c613302; /* mov.l r2,@r14 */
L8c613302: sa_motion_publish_stop(ctx,0x8c613302u); r[15]+=64u;goto L8c613304; /* add #0x40,r15 */
L8c613304: sa_motion_publish_stop(ctx,0x8c613304u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613306; /* mov.l @r15+,r9 */
L8c613306: sa_motion_publish_stop(ctx,0x8c613306u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613308; /* mov.l @r15+,r10 */
L8c613308: sa_motion_publish_stop(ctx,0x8c613308u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61330a; /* mov.l @r15+,r11 */
L8c61330a: sa_motion_publish_stop(ctx,0x8c61330au); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61330c; /* mov.l @r15+,r12 */
L8c61330c: sa_motion_publish_stop(ctx,0x8c61330cu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61330e; /* mov.l @r15+,r13 */
L8c61330e: sa_motion_publish_stop(ctx,0x8c613310u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
