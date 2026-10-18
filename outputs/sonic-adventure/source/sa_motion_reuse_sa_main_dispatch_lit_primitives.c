#include "sa_draw_motion_stop.h"
/* Normal-lit primitive family with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_lit_primitive_emit_8c617e46(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c617e46;
L8c617e46: sa_motion_publish_stop(ctx,0x8c617e46u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c617e48; /* mov.l r14,@-r15 */
L8c617e48: sa_motion_publish_stop(ctx,0x8c617e48u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c617e4a; /* mov.l r13,@-r15 */
L8c617e4a: sa_motion_publish_stop(ctx,0x8c617e4au); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c617e4c; /* mov.l r12,@-r15 */
L8c617e4c: sa_motion_publish_stop(ctx,0x8c617e4cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c617e4e; /* mov.l r11,@-r15 */
L8c617e4e: sa_motion_publish_stop(ctx,0x8c617e4eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c617e50; /* mov.l r10,@-r15 */
L8c617e50: sa_motion_publish_stop(ctx,0x8c617e50u); r[3]=rd(m,bus,&io_status,0x8c617ebcu,4);if(io_status)return io_status;goto L8c617e52; /* mov.l 0x8c617ebc,r3 */
L8c617e52: sa_motion_publish_stop(ctx,0x8c617e52u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c617e54; /* mov.l r9,@-r15 */
L8c617e54: sa_motion_publish_stop(ctx,0x8c617e54u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c617e56; /* sts.l PR,@-r15 */
L8c617e56: sa_motion_publish_stop(ctx,0x8c617e56u); r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c617e58; /* mov.w @r4,r0 */
L8c617e58: sa_motion_publish_stop(ctx,0x8c617e58u); r[13]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c617e5a; /* mov.l @(0x4,r4),r13 */
L8c617e5a: sa_motion_publish_stop(ctx,0x8c617e5au); r[0]=r[0]&65535u;goto L8c617e5c; /* extu.w r0,r0 */
L8c617e5c: sa_motion_publish_stop(ctx,0x8c617e5cu); r[9]=rd(m,bus,&io_status,0x8c617eb4u,4);if(io_status)return io_status;goto L8c617e5e; /* mov.l 0x8c617eb4,r9 */
L8c617e5e: sa_motion_publish_stop(ctx,0x8c617e5eu); r[0]&=r[3];goto L8c617e60; /* and r3,r0 */
L8c617e60: sa_motion_publish_stop(ctx,0x8c617e60u); r[11]=rd(m,bus,&io_status,0x8c617eb8u,4);if(io_status)return io_status;goto L8c617e62; /* mov.l 0x8c617eb8,r11 */
L8c617e62: sa_motion_publish_stop(ctx,0x8c617e62u); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c617e64; /* cmp/eq #0x0,r0 */
L8c617e64: sa_motion_publish_stop(ctx,0x8c617e66u); take=(*sr&1u);r[12]=0u;if(take)goto L8c617e8a;goto L8c617e68; /* bt/s 0x8c617e8a */
L8c617e68: sa_motion_publish_stop(ctx,0x8c617e68u); r[1]=sx(m,bus,&io_status,0x8c617ea4u,2);if(io_status)return io_status;goto L8c617e6a; /* mov.w 0x8c617ea4,r1 */
L8c617e6a: sa_motion_publish_stop(ctx,0x8c617e6au); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c617e6c; /* cmp/eq r1,r0 */
L8c617e6c: sa_motion_publish_stop(ctx,0x8c617e6cu); if(!(*sr&1u))goto L8c617e72;goto L8c617e6e; /* bf 0x8c617e72 */
L8c617e6e: sa_motion_publish_stop(ctx,0x8c617e70u); goto L8c618066; /* bra 0x8c618066 */
L8c617e72: sa_motion_publish_stop(ctx,0x8c617e72u); r[1]=rd(m,bus,&io_status,0x8c617ec0u,4);if(io_status)return io_status;goto L8c617e74; /* mov.l 0x8c617ec0,r1 */
L8c617e74: sa_motion_publish_stop(ctx,0x8c617e74u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c617e76; /* cmp/eq r1,r0 */
L8c617e76: sa_motion_publish_stop(ctx,0x8c617e76u); if(!(*sr&1u))goto L8c617e7c;goto L8c617e78; /* bf 0x8c617e7c */
L8c617e78: sa_motion_publish_stop(ctx,0x8c617e7au); goto L8c6182ae; /* bra 0x8c6182ae */
L8c617e7c: sa_motion_publish_stop(ctx,0x8c617e7cu); r[1]=rd(m,bus,&io_status,0x8c617ebcu,4);if(io_status)return io_status;goto L8c617e7e; /* mov.l 0x8c617ebc,r1 */
L8c617e7e: sa_motion_publish_stop(ctx,0x8c617e7eu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c617e80; /* cmp/eq r1,r0 */
L8c617e80: sa_motion_publish_stop(ctx,0x8c617e80u); if(!(*sr&1u))goto L8c617e86;goto L8c617e82; /* bf 0x8c617e86 */
L8c617e82: sa_motion_publish_stop(ctx,0x8c617e84u); goto L8c618456; /* bra 0x8c618456 */
L8c617e86: sa_motion_publish_stop(ctx,0x8c617e88u); goto L8c6184aa; /* bra 0x8c6184aa */
L8c617e8a: sa_motion_publish_stop(ctx,0x8c617e8au); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c617e8c; /* mov.w @(0x2,r4),r0 */
L8c617e8c: sa_motion_publish_stop(ctx,0x8c617e8cu); r[1]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c617e8e; /* mov.l @r11,r1 */
L8c617e8e: sa_motion_publish_stop(ctx,0x8c617e8eu); r[0]=r[0]&65535u;goto L8c617e90; /* extu.w r0,r0 */
L8c617e90: sa_motion_publish_stop(ctx,0x8c617e90u); r[2]=r[0];goto L8c617e92; /* mov r0,r2 */
L8c617e92: sa_motion_publish_stop(ctx,0x8c617e92u); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c617e94; /* shll r0 */
L8c617e94: sa_motion_publish_stop(ctx,0x8c617e94u); r[0]+=r[2];goto L8c617e96; /* add r2,r0 */
L8c617e96: sa_motion_publish_stop(ctx,0x8c617e96u); r[1]+=r[0];goto L8c617e98; /* add r0,r1 */
L8c617e98: sa_motion_publish_stop(ctx,0x8c617e98u); wr(m,bus,&io_status,r[11],r[1],4);if(io_status)return io_status;goto L8c617e9a; /* mov.l r1,@r11 */
L8c617e9a: sa_motion_publish_stop(ctx,0x8c617e9cu); r[14]=r[12];goto L8c618056; /* bra 0x8c618056 */
L8c617ec4: sa_motion_publish_stop(ctx,0x8c617ec4u); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c617ec6; /* mov.w @r13+,r5 */
L8c617ec6: sa_motion_publish_stop(ctx,0x8c617ec6u); r[0]=16u;goto L8c617ec8; /* mov #0x10,r0 */
L8c617ec8: sa_motion_publish_stop(ctx,0x8c617ec8u); r[3]=rd(m,bus,&io_status,0x8c61807cu,4);if(io_status)return io_status;goto L8c617eca; /* mov.l 0x8c61807c,r3 */
L8c617eca: sa_motion_publish_stop(ctx,0x8c617ecau); r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c617ecc; /* mov.w @r13+,r6 */
L8c617ecc: sa_motion_publish_stop(ctx,0x8c617eccu); r[5]=r[5]&65535u;goto L8c617ece; /* extu.w r5,r5 */
L8c617ece: sa_motion_publish_stop(ctx,0x8c617eceu); r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c617ed0; /* mov.l @r3,r12 */
L8c617ed0: sa_motion_publish_stop(ctx,0x8c617ed0u); r[5]<<=2;goto L8c617ed2; /* shll2 r5 */
L8c617ed2: sa_motion_publish_stop(ctx,0x8c617ed2u); r[6]=r[6]&65535u;goto L8c617ed4; /* extu.w r6,r6 */
L8c617ed4: sa_motion_publish_stop(ctx,0x8c617ed4u); r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c617ed6; /* mov.w @r13+,r7 */
L8c617ed6: sa_motion_publish_stop(ctx,0x8c617ed6u); r[6]<<=2;goto L8c617ed8; /* shll2 r6 */
L8c617ed8: sa_motion_publish_stop(ctx,0x8c617ed8u); r[6]<<=2;goto L8c617eda; /* shll2 r6 */
L8c617eda: sa_motion_publish_stop(ctx,0x8c617edau); r[5]<<=2;goto L8c617edc; /* shll2 r5 */
L8c617edc: sa_motion_publish_stop(ctx,0x8c617edcu); r[7]=r[7]&65535u;goto L8c617ede; /* extu.w r7,r7 */
L8c617ede: sa_motion_publish_stop(ctx,0x8c617edeu); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c617ee0; /* shll r5 */
L8c617ee0: sa_motion_publish_stop(ctx,0x8c617ee0u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c617ee2; /* shll r6 */
L8c617ee2: sa_motion_publish_stop(ctx,0x8c617ee2u); r[6]+=r[12];goto L8c617ee4; /* add r12,r6 */
L8c617ee4: sa_motion_publish_stop(ctx,0x8c617ee4u); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c617ee6; /* fmov.s @(r0,r6),fr4 */
L8c617ee6: sa_motion_publish_stop(ctx,0x8c617ee6u); r[5]+=r[12];goto L8c617ee8; /* add r12,r5 */
L8c617ee8: sa_motion_publish_stop(ctx,0x8c617ee8u); r[7]<<=2;goto L8c617eea; /* shll2 r7 */
L8c617eea: sa_motion_publish_stop(ctx,0x8c617eeau); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617eec; /* fmov.s @(r0,r5),fr3 */
L8c617eec: sa_motion_publish_stop(ctx,0x8c617eecu); r[7]<<=2;goto L8c617eee; /* shll2 r7 */
L8c617eee: sa_motion_publish_stop(ctx,0x8c617eeeu); fcopy(f,2u,4u);goto L8c617ef0; /* fmov fr4,fr2 */
L8c617ef0: sa_motion_publish_stop(ctx,0x8c617ef0u); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c617ef2; /* shll r7 */
L8c617ef2: sa_motion_publish_stop(ctx,0x8c617ef2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c617ef4; /* fsub fr3,fr2 */
L8c617ef4: sa_motion_publish_stop(ctx,0x8c617ef4u); r[0]=20u;goto L8c617ef6; /* mov #0x14,r0 */
L8c617ef6: sa_motion_publish_stop(ctx,0x8c617ef6u); r[7]+=r[12];goto L8c617ef8; /* add r12,r7 */
L8c617ef8: sa_motion_publish_stop(ctx,0x8c617ef8u); fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c617efa; /* fmov.s @(r0,r6),fr5 */
L8c617efa: sa_motion_publish_stop(ctx,0x8c617efau); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c617efc; /* fmov.s @(r0,r7),fr3 */
L8c617efc: sa_motion_publish_stop(ctx,0x8c617efcu); fcopy(f,1u,5u);goto L8c617efe; /* fmov fr5,fr1 */
L8c617efe: sa_motion_publish_stop(ctx,0x8c617efeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617f00; /* fsub fr5,fr3 */
L8c617f00: sa_motion_publish_stop(ctx,0x8c617f00u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617f02; /* fmul fr3,fr2 */
L8c617f02: sa_motion_publish_stop(ctx,0x8c617f02u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f04; /* fmov.s @(r0,r5),fr3 */
L8c617f04: sa_motion_publish_stop(ctx,0x8c617f04u); r[0]=16u;goto L8c617f06; /* mov #0x10,r0 */
L8c617f06: sa_motion_publish_stop(ctx,0x8c617f06u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c617f08; /* fsub fr3,fr1 */
L8c617f08: sa_motion_publish_stop(ctx,0x8c617f08u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c617f0a; /* fmov.s @(r0,r7),fr3 */
L8c617f0a: sa_motion_publish_stop(ctx,0x8c617f0au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617f0c; /* fsub fr4,fr3 */
L8c617f0c: sa_motion_publish_stop(ctx,0x8c617f0cu); fcopy(f,4u,2u);goto L8c617f0e; /* fmov fr2,fr4 */
L8c617f0e: sa_motion_publish_stop(ctx,0x8c617f0eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c617f10; /* fmul fr3,fr1 */
L8c617f10: sa_motion_publish_stop(ctx,0x8c617f10u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c617f12; /* fldi0 fr3 */
L8c617f12: sa_motion_publish_stop(ctx,0x8c617f12u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c617f14; /* fsub fr1,fr4 */
L8c617f14: sa_motion_publish_stop(ctx,0x8c617f14u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c617f16; /* fcmp/gt fr3,fr4 */
L8c617f16: sa_motion_publish_stop(ctx,0x8c617f18u); take=!(*sr&1u);r[14]+=1u;if(take)goto L8c617f1e;goto L8c617f1a; /* bf/s 0x8c617f1e */
L8c617f1a: sa_motion_publish_stop(ctx,0x8c617f1cu); goto L8c618056; /* bra 0x8c618056 */
L8c617f1e: sa_motion_publish_stop(ctx,0x8c617f1eu); r[3]=rd(m,bus,&io_status,0x8c618080u,4);if(io_status)return io_status;goto L8c617f20; /* mov.l 0x8c618080,r3 */
L8c617f20: sa_motion_publish_stop(ctx,0x8c617f20u); r[0]=4u;goto L8c617f22; /* mov #0x4,r0 */
L8c617f22: sa_motion_publish_stop(ctx,0x8c617f22u); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c617f24; /* fmov.s @r5,fr3 */
L8c617f24: sa_motion_publish_stop(ctx,0x8c617f24u); r[8]+=32u;goto L8c617f26; /* add #0x20,r8 */
L8c617f26: sa_motion_publish_stop(ctx,0x8c617f26u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c617f28; /* fmov.s @r3,fr2 */
L8c617f28: sa_motion_publish_stop(ctx,0x8c617f28u); r[3]+=8u;goto L8c617f2a; /* add #0x8,r3 */
L8c617f2a: sa_motion_publish_stop(ctx,0x8c617f2au); r[2]=rd(m,bus,&io_status,0x8c618084u,4);if(io_status)return io_status;goto L8c617f2c; /* mov.l 0x8c618084,r2 */
L8c617f2c: sa_motion_publish_stop(ctx,0x8c617f2cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617f2e; /* fmul fr3,fr2 */
L8c617f2e: sa_motion_publish_stop(ctx,0x8c617f2eu); r[1]=rd(m,bus,&io_status,0x8c618088u,4);if(io_status)return io_status;goto L8c617f30; /* mov.l 0x8c618088,r1 */
L8c617f30: sa_motion_publish_stop(ctx,0x8c617f30u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c617f32; /* fmov.s @r2,fr3 */
L8c617f32: sa_motion_publish_stop(ctx,0x8c617f32u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c617f34; /* fmov.s @r1,fr1 */
L8c617f34: sa_motion_publish_stop(ctx,0x8c617f34u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617f36; /* fsub fr2,fr3 */
L8c617f36: sa_motion_publish_stop(ctx,0x8c617f36u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f38; /* fmov.s @(r0,r5),fr2 */
L8c617f38: sa_motion_publish_stop(ctx,0x8c617f38u); r[0]=8u;goto L8c617f3a; /* mov #0x8,r0 */
L8c617f3a: sa_motion_publish_stop(ctx,0x8c617f3au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c617f3c; /* fmul fr2,fr1 */
L8c617f3c: sa_motion_publish_stop(ctx,0x8c617f3cu); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f3e; /* fmov.s @(r0,r5),fr2 */
L8c617f3e: sa_motion_publish_stop(ctx,0x8c617f3eu); r[0]=28u;goto L8c617f40; /* mov #0x1c,r0 */
L8c617f40: sa_motion_publish_stop(ctx,0x8c617f40u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617f42; /* fsub fr1,fr3 */
L8c617f42: sa_motion_publish_stop(ctx,0x8c617f42u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c617f44; /* fmov.s @r3,fr1 */
L8c617f44: sa_motion_publish_stop(ctx,0x8c617f44u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c617f46; /* fmul fr2,fr1 */
L8c617f46: sa_motion_publish_stop(ctx,0x8c617f46u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617f48; /* fsub fr1,fr3 */
L8c617f48: sa_motion_publish_stop(ctx,0x8c617f48u); fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f4a; /* fmov fr3,@(r0,r5) */
L8c617f4a: sa_motion_publish_stop(ctx,0x8c617f4au); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c617f4c; /* fmov.s @(r0,r9),fr2 */
L8c617f4c: sa_motion_publish_stop(ctx,0x8c617f4cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617f4e; /* fmul fr3,fr2 */
L8c617f4e: sa_motion_publish_stop(ctx,0x8c617f4eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c617f50; /* fmov.s fr2,@-r8 */
L8c617f50: sa_motion_publish_stop(ctx,0x8c617f50u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f52; /* fmov.s @(r0,r5),fr3 */
L8c617f52: sa_motion_publish_stop(ctx,0x8c617f52u); r[0]=24u;goto L8c617f54; /* mov #0x18,r0 */
L8c617f54: sa_motion_publish_stop(ctx,0x8c617f54u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c617f56; /* fmov.s @(r0,r9),fr2 */
L8c617f56: sa_motion_publish_stop(ctx,0x8c617f56u); r[0]=28u;goto L8c617f58; /* mov #0x1c,r0 */
L8c617f58: sa_motion_publish_stop(ctx,0x8c617f58u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617f5a; /* fmul fr3,fr2 */
L8c617f5a: sa_motion_publish_stop(ctx,0x8c617f5au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c617f5c; /* fmov.s fr2,@-r8 */
L8c617f5c: sa_motion_publish_stop(ctx,0x8c617f5cu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f5e; /* fmov.s @(r0,r5),fr3 */
L8c617f5e: sa_motion_publish_stop(ctx,0x8c617f5eu); r[0]=20u;goto L8c617f60; /* mov #0x14,r0 */
L8c617f60: sa_motion_publish_stop(ctx,0x8c617f60u); r[2]=rd(m,bus,&io_status,0x8c61808cu,4);if(io_status)return io_status;goto L8c617f62; /* mov.l 0x8c61808c,r2 */
L8c617f62: sa_motion_publish_stop(ctx,0x8c617f62u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c617f64; /* fmov.s @(r0,r9),fr2 */
L8c617f64: sa_motion_publish_stop(ctx,0x8c617f64u); r[0]=16u;goto L8c617f66; /* mov #0x10,r0 */
L8c617f66: sa_motion_publish_stop(ctx,0x8c617f66u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617f68; /* fmul fr3,fr2 */
L8c617f68: sa_motion_publish_stop(ctx,0x8c617f68u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c617f6a; /* fmov.s fr2,@-r8 */
L8c617f6a: sa_motion_publish_stop(ctx,0x8c617f6au); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c617f6c; /* fmov.s @(r0,r9),fr3 */
L8c617f6c: sa_motion_publish_stop(ctx,0x8c617f6cu); r[0]=24u;goto L8c617f6e; /* mov #0x18,r0 */
L8c617f6e: sa_motion_publish_stop(ctx,0x8c617f6eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617f70; /* fmov.s fr3,@-r8 */
L8c617f70: sa_motion_publish_stop(ctx,0x8c617f70u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f72; /* fmov.s @(r0,r5),fr3 */
L8c617f72: sa_motion_publish_stop(ctx,0x8c617f72u); r[0]=20u;goto L8c617f74; /* mov #0x14,r0 */
L8c617f74: sa_motion_publish_stop(ctx,0x8c617f74u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617f76; /* fmov.s fr3,@-r8 */
L8c617f76: sa_motion_publish_stop(ctx,0x8c617f76u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f78; /* fmov.s @(r0,r5),fr3 */
L8c617f78: sa_motion_publish_stop(ctx,0x8c617f78u); r[0]=16u;goto L8c617f7a; /* mov #0x10,r0 */
L8c617f7a: sa_motion_publish_stop(ctx,0x8c617f7au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617f7c; /* fmov.s fr3,@-r8 */
L8c617f7c: sa_motion_publish_stop(ctx,0x8c617f7cu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617f7e; /* fmov.s @(r0,r5),fr3 */
L8c617f7e: sa_motion_publish_stop(ctx,0x8c617f7eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617f80; /* fmov.s fr3,@-r8 */
L8c617f80: sa_motion_publish_stop(ctx,0x8c617f80u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c617f82; /* mov.l r2,@-r8 */
L8c617f82: sa_motion_publish_stop(ctx,0x8c617f82u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617f84; /* pref @r8 */
L8c617f84: sa_motion_publish_stop(ctx,0x8c617f84u); r[2]=rd(m,bus,&io_status,0x8c618080u,4);if(io_status)return io_status;goto L8c617f86; /* mov.l 0x8c618080,r2 */
L8c617f86: sa_motion_publish_stop(ctx,0x8c617f86u); r[0]=4u;goto L8c617f88; /* mov #0x4,r0 */
L8c617f88: sa_motion_publish_stop(ctx,0x8c617f88u); fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c617f8a; /* fmov.s @r6,fr3 */
L8c617f8a: sa_motion_publish_stop(ctx,0x8c617f8au); r[8]+=64u;goto L8c617f8c; /* add #0x40,r8 */
L8c617f8c: sa_motion_publish_stop(ctx,0x8c617f8cu); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c617f8e; /* fmov.s @r2,fr2 */
L8c617f8e: sa_motion_publish_stop(ctx,0x8c617f8eu); r[3]=rd(m,bus,&io_status,0x8c618084u,4);if(io_status)return io_status;goto L8c617f90; /* mov.l 0x8c618084,r3 */
L8c617f90: sa_motion_publish_stop(ctx,0x8c617f90u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617f92; /* fmul fr3,fr2 */
L8c617f92: sa_motion_publish_stop(ctx,0x8c617f92u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c617f94; /* fmov.s @r1,fr1 */
L8c617f94: sa_motion_publish_stop(ctx,0x8c617f94u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c617f96; /* fmov.s @r3,fr3 */
L8c617f96: sa_motion_publish_stop(ctx,0x8c617f96u); r[3]=rd(m,bus,&io_status,0x8c618090u,4);if(io_status)return io_status;goto L8c617f98; /* mov.l 0x8c618090,r3 */
L8c617f98: sa_motion_publish_stop(ctx,0x8c617f98u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617f9a; /* fsub fr2,fr3 */
L8c617f9a: sa_motion_publish_stop(ctx,0x8c617f9au); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c617f9c; /* fmov.s @(r0,r6),fr2 */
L8c617f9c: sa_motion_publish_stop(ctx,0x8c617f9cu); r[0]=8u;goto L8c617f9e; /* mov #0x8,r0 */
L8c617f9e: sa_motion_publish_stop(ctx,0x8c617f9eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c617fa0; /* fmul fr2,fr1 */
L8c617fa0: sa_motion_publish_stop(ctx,0x8c617fa0u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c617fa2; /* fmov.s @(r0,r6),fr2 */
L8c617fa2: sa_motion_publish_stop(ctx,0x8c617fa2u); r[0]=28u;goto L8c617fa4; /* mov #0x1c,r0 */
L8c617fa4: sa_motion_publish_stop(ctx,0x8c617fa4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617fa6; /* fsub fr1,fr3 */
L8c617fa6: sa_motion_publish_stop(ctx,0x8c617fa6u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c617fa8; /* fmov.s @r3,fr1 */
L8c617fa8: sa_motion_publish_stop(ctx,0x8c617fa8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c617faa; /* fmul fr2,fr1 */
L8c617faa: sa_motion_publish_stop(ctx,0x8c617faau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617fac; /* fsub fr1,fr3 */
L8c617fac: sa_motion_publish_stop(ctx,0x8c617facu); fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c617fae; /* fmov fr3,@(r0,r6) */
L8c617fae: sa_motion_publish_stop(ctx,0x8c617faeu); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c617fb0; /* fmov.s @(r0,r9),fr2 */
L8c617fb0: sa_motion_publish_stop(ctx,0x8c617fb0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617fb2; /* fmul fr3,fr2 */
L8c617fb2: sa_motion_publish_stop(ctx,0x8c617fb2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c617fb4; /* fmov.s fr2,@-r8 */
L8c617fb4: sa_motion_publish_stop(ctx,0x8c617fb4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c617fb6; /* fmov.s @(r0,r6),fr3 */
L8c617fb6: sa_motion_publish_stop(ctx,0x8c617fb6u); r[0]=24u;goto L8c617fb8; /* mov #0x18,r0 */
L8c617fb8: sa_motion_publish_stop(ctx,0x8c617fb8u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c617fba; /* fmov.s @(r0,r9),fr2 */
L8c617fba: sa_motion_publish_stop(ctx,0x8c617fbau); r[0]=28u;goto L8c617fbc; /* mov #0x1c,r0 */
L8c617fbc: sa_motion_publish_stop(ctx,0x8c617fbcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617fbe; /* fmul fr3,fr2 */
L8c617fbe: sa_motion_publish_stop(ctx,0x8c617fbeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c617fc0; /* fmov.s fr2,@-r8 */
L8c617fc0: sa_motion_publish_stop(ctx,0x8c617fc0u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c617fc2; /* fmov.s @(r0,r6),fr3 */
L8c617fc2: sa_motion_publish_stop(ctx,0x8c617fc2u); r[0]=20u;goto L8c617fc4; /* mov #0x14,r0 */
L8c617fc4: sa_motion_publish_stop(ctx,0x8c617fc4u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c617fc6; /* fmov.s @(r0,r9),fr2 */
L8c617fc6: sa_motion_publish_stop(ctx,0x8c617fc6u); r[0]=16u;goto L8c617fc8; /* mov #0x10,r0 */
L8c617fc8: sa_motion_publish_stop(ctx,0x8c617fc8u); r[2]=rd(m,bus,&io_status,0x8c61808cu,4);if(io_status)return io_status;goto L8c617fca; /* mov.l 0x8c61808c,r2 */
L8c617fca: sa_motion_publish_stop(ctx,0x8c617fcau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617fcc; /* fmul fr3,fr2 */
L8c617fcc: sa_motion_publish_stop(ctx,0x8c617fccu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c617fce; /* fmov.s fr2,@-r8 */
L8c617fce: sa_motion_publish_stop(ctx,0x8c617fceu); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c617fd0; /* fmov.s @(r0,r9),fr3 */
L8c617fd0: sa_motion_publish_stop(ctx,0x8c617fd0u); r[0]=24u;goto L8c617fd2; /* mov #0x18,r0 */
L8c617fd2: sa_motion_publish_stop(ctx,0x8c617fd2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617fd4; /* fmov.s fr3,@-r8 */
L8c617fd4: sa_motion_publish_stop(ctx,0x8c617fd4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c617fd6; /* fmov.s @(r0,r6),fr3 */
L8c617fd6: sa_motion_publish_stop(ctx,0x8c617fd6u); r[0]=20u;goto L8c617fd8; /* mov #0x14,r0 */
L8c617fd8: sa_motion_publish_stop(ctx,0x8c617fd8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617fda; /* fmov.s fr3,@-r8 */
L8c617fda: sa_motion_publish_stop(ctx,0x8c617fdau); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c617fdc; /* fmov.s @(r0,r6),fr3 */
L8c617fdc: sa_motion_publish_stop(ctx,0x8c617fdcu); r[0]=16u;goto L8c617fde; /* mov #0x10,r0 */
L8c617fde: sa_motion_publish_stop(ctx,0x8c617fdeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617fe0; /* fmov.s fr3,@-r8 */
L8c617fe0: sa_motion_publish_stop(ctx,0x8c617fe0u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c617fe2; /* fmov.s @(r0,r6),fr3 */
L8c617fe2: sa_motion_publish_stop(ctx,0x8c617fe2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617fe4; /* fmov.s fr3,@-r8 */
L8c617fe4: sa_motion_publish_stop(ctx,0x8c617fe4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c617fe6; /* mov.l r2,@-r8 */
L8c617fe6: sa_motion_publish_stop(ctx,0x8c617fe6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617fe8; /* pref @r8 */
L8c617fe8: sa_motion_publish_stop(ctx,0x8c617fe8u); r[2]=rd(m,bus,&io_status,0x8c618080u,4);if(io_status)return io_status;goto L8c617fea; /* mov.l 0x8c618080,r2 */
L8c617fea: sa_motion_publish_stop(ctx,0x8c617feau); r[0]=4u;goto L8c617fec; /* mov #0x4,r0 */
L8c617fec: sa_motion_publish_stop(ctx,0x8c617fecu); fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c617fee; /* fmov.s @r7,fr3 */
L8c617fee: sa_motion_publish_stop(ctx,0x8c617feeu); r[8]+=64u;goto L8c617ff0; /* add #0x40,r8 */
L8c617ff0: sa_motion_publish_stop(ctx,0x8c617ff0u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c617ff2; /* fmov.s @r2,fr2 */
L8c617ff2: sa_motion_publish_stop(ctx,0x8c617ff2u); r[3]=rd(m,bus,&io_status,0x8c618084u,4);if(io_status)return io_status;goto L8c617ff4; /* mov.l 0x8c618084,r3 */
L8c617ff4: sa_motion_publish_stop(ctx,0x8c617ff4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617ff6; /* fmul fr3,fr2 */
L8c617ff6: sa_motion_publish_stop(ctx,0x8c617ff6u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c617ff8; /* fmov.s @r1,fr1 */
L8c617ff8: sa_motion_publish_stop(ctx,0x8c617ff8u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c617ffa; /* fmov.s @r3,fr3 */
L8c617ffa: sa_motion_publish_stop(ctx,0x8c617ffau); r[3]=rd(m,bus,&io_status,0x8c618090u,4);if(io_status)return io_status;goto L8c617ffc; /* mov.l 0x8c618090,r3 */
L8c617ffc: sa_motion_publish_stop(ctx,0x8c617ffcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617ffe; /* fsub fr2,fr3 */
L8c617ffe: sa_motion_publish_stop(ctx,0x8c617ffeu); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c618000; /* fmov.s @(r0,r7),fr2 */
L8c618000: sa_motion_publish_stop(ctx,0x8c618000u); r[0]=8u;goto L8c618002; /* mov #0x8,r0 */
L8c618002: sa_motion_publish_stop(ctx,0x8c618002u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618004; /* fmul fr2,fr1 */
L8c618004: sa_motion_publish_stop(ctx,0x8c618004u); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c618006; /* fmov.s @(r0,r7),fr2 */
L8c618006: sa_motion_publish_stop(ctx,0x8c618006u); r[0]=28u;goto L8c618008; /* mov #0x1c,r0 */
L8c618008: sa_motion_publish_stop(ctx,0x8c618008u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61800a; /* fsub fr1,fr3 */
L8c61800a: sa_motion_publish_stop(ctx,0x8c61800au); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61800c; /* fmov.s @r3,fr1 */
L8c61800c: sa_motion_publish_stop(ctx,0x8c61800cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61800e; /* fmul fr2,fr1 */
L8c61800e: sa_motion_publish_stop(ctx,0x8c61800eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618010; /* fsub fr1,fr3 */
L8c618010: sa_motion_publish_stop(ctx,0x8c618010u); fstore(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618012; /* fmov fr3,@(r0,r7) */
L8c618012: sa_motion_publish_stop(ctx,0x8c618012u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618014; /* fmov.s @(r0,r9),fr2 */
L8c618014: sa_motion_publish_stop(ctx,0x8c618014u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618016; /* fmul fr3,fr2 */
L8c618016: sa_motion_publish_stop(ctx,0x8c618016u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618018; /* fmov.s fr2,@-r8 */
L8c618018: sa_motion_publish_stop(ctx,0x8c618018u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61801a; /* fmov.s @(r0,r7),fr3 */
L8c61801a: sa_motion_publish_stop(ctx,0x8c61801au); r[0]=24u;goto L8c61801c; /* mov #0x18,r0 */
L8c61801c: sa_motion_publish_stop(ctx,0x8c61801cu); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61801e; /* fmov.s @(r0,r9),fr2 */
L8c61801e: sa_motion_publish_stop(ctx,0x8c61801eu); r[0]=28u;goto L8c618020; /* mov #0x1c,r0 */
L8c618020: sa_motion_publish_stop(ctx,0x8c618020u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618022; /* fmul fr3,fr2 */
L8c618022: sa_motion_publish_stop(ctx,0x8c618022u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618024; /* fmov.s fr2,@-r8 */
L8c618024: sa_motion_publish_stop(ctx,0x8c618024u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618026; /* fmov.s @(r0,r7),fr3 */
L8c618026: sa_motion_publish_stop(ctx,0x8c618026u); r[0]=20u;goto L8c618028; /* mov #0x14,r0 */
L8c618028: sa_motion_publish_stop(ctx,0x8c618028u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61802a; /* fmov.s @(r0,r9),fr2 */
L8c61802a: sa_motion_publish_stop(ctx,0x8c61802au); r[0]=16u;goto L8c61802c; /* mov #0x10,r0 */
L8c61802c: sa_motion_publish_stop(ctx,0x8c61802cu); r[2]=rd(m,bus,&io_status,0x8c618094u,4);if(io_status)return io_status;goto L8c61802e; /* mov.l 0x8c618094,r2 */
L8c61802e: sa_motion_publish_stop(ctx,0x8c61802eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618030; /* fmul fr3,fr2 */
L8c618030: sa_motion_publish_stop(ctx,0x8c618030u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618032; /* fmov.s fr2,@-r8 */
L8c618032: sa_motion_publish_stop(ctx,0x8c618032u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c618034; /* fmov.s @(r0,r9),fr3 */
L8c618034: sa_motion_publish_stop(ctx,0x8c618034u); r[0]=24u;goto L8c618036; /* mov #0x18,r0 */
L8c618036: sa_motion_publish_stop(ctx,0x8c618036u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618038; /* fmov.s fr3,@-r8 */
L8c618038: sa_motion_publish_stop(ctx,0x8c618038u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61803a; /* fmov.s @(r0,r7),fr3 */
L8c61803a: sa_motion_publish_stop(ctx,0x8c61803au); r[0]=20u;goto L8c61803c; /* mov #0x14,r0 */
L8c61803c: sa_motion_publish_stop(ctx,0x8c61803cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61803e; /* fmov.s fr3,@-r8 */
L8c61803e: sa_motion_publish_stop(ctx,0x8c61803eu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618040; /* fmov.s @(r0,r7),fr3 */
L8c618040: sa_motion_publish_stop(ctx,0x8c618040u); r[0]=16u;goto L8c618042; /* mov #0x10,r0 */
L8c618042: sa_motion_publish_stop(ctx,0x8c618042u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618044; /* fmov.s fr3,@-r8 */
L8c618044: sa_motion_publish_stop(ctx,0x8c618044u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618046; /* fmov.s @(r0,r7),fr3 */
L8c618046: sa_motion_publish_stop(ctx,0x8c618046u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618048; /* fmov.s fr3,@-r8 */
L8c618048: sa_motion_publish_stop(ctx,0x8c618048u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61804a; /* mov.l r2,@-r8 */
L8c61804a: sa_motion_publish_stop(ctx,0x8c61804au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61804c; /* pref @r8 */
L8c61804c: sa_motion_publish_stop(ctx,0x8c61804cu); r[0]=rd(m,bus,&io_status,0x8c618098u,4);if(io_status)return io_status;goto L8c61804e; /* mov.l 0x8c618098,r0 */
L8c61804e: sa_motion_publish_stop(ctx,0x8c61804eu); r[8]+=32u;goto L8c618050; /* add #0x20,r8 */
L8c618050: sa_motion_publish_stop(ctx,0x8c618050u); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c618052; /* mov.l @r0,r2 */
L8c618052: sa_motion_publish_stop(ctx,0x8c618052u); r[2]+=3u;goto L8c618054; /* add #0x3,r2 */
L8c618054: sa_motion_publish_stop(ctx,0x8c618054u); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c618056; /* mov.l r2,@r0 */
L8c618056: sa_motion_publish_stop(ctx,0x8c618056u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c618058; /* mov.w @(0x2,r4),r0 */
L8c618058: sa_motion_publish_stop(ctx,0x8c618058u); r[0]=r[0]&65535u;goto L8c61805a; /* extu.w r0,r0 */
L8c61805a: sa_motion_publish_stop(ctx,0x8c61805au); *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c61805c; /* cmp/ge r0,r14 */
L8c61805c: sa_motion_publish_stop(ctx,0x8c61805cu); if((*sr&1u))goto L8c618062;goto L8c61805e; /* bt 0x8c618062 */
L8c61805e: sa_motion_publish_stop(ctx,0x8c618060u); goto L8c617ec4; /* bra 0x8c617ec4 */
L8c618062: sa_motion_publish_stop(ctx,0x8c618064u); goto L8c6184aa; /* bra 0x8c6184aa */
L8c618066: sa_motion_publish_stop(ctx,0x8c618066u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c618068; /* mov.w @(0x2,r4),r0 */
L8c618068: sa_motion_publish_stop(ctx,0x8c618068u); r[2]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c61806a; /* mov.l @r11,r2 */
L8c61806a: sa_motion_publish_stop(ctx,0x8c61806au); r[0]=r[0]&65535u;goto L8c61806c; /* extu.w r0,r0 */
L8c61806c: sa_motion_publish_stop(ctx,0x8c61806cu); r[0]<<=2;goto L8c61806e; /* shll2 r0 */
L8c61806e: sa_motion_publish_stop(ctx,0x8c61806eu); r[2]+=r[0];goto L8c618070; /* add r0,r2 */
L8c618070: sa_motion_publish_stop(ctx,0x8c618070u); wr(m,bus,&io_status,r[11],r[2],4);if(io_status)return io_status;goto L8c618072; /* mov.l r2,@r11 */
L8c618072: sa_motion_publish_stop(ctx,0x8c618074u); r[11]=r[12];goto L8c61829e; /* bra 0x8c61829e */
L8c61809c: sa_motion_publish_stop(ctx,0x8c61809cu); r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61809e; /* mov.w @r13+,r6 */
L8c61809e: sa_motion_publish_stop(ctx,0x8c61809eu); r[0]=16u;goto L8c6180a0; /* mov #0x10,r0 */
L8c6180a0: sa_motion_publish_stop(ctx,0x8c6180a0u); r[3]=rd(m,bus,&io_status,0x8c61831cu,4);if(io_status)return io_status;goto L8c6180a2; /* mov.l 0x8c61831c,r3 */
L8c6180a2: sa_motion_publish_stop(ctx,0x8c6180a2u); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6180a4; /* mov.w @r13+,r5 */
L8c6180a4: sa_motion_publish_stop(ctx,0x8c6180a4u); r[6]=r[6]&65535u;goto L8c6180a6; /* extu.w r6,r6 */
L8c6180a6: sa_motion_publish_stop(ctx,0x8c6180a6u); r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6180a8; /* mov.l @r3,r12 */
L8c6180a8: sa_motion_publish_stop(ctx,0x8c6180a8u); r[6]<<=2;goto L8c6180aa; /* shll2 r6 */
L8c6180aa: sa_motion_publish_stop(ctx,0x8c6180aau); r[5]=r[5]&65535u;goto L8c6180ac; /* extu.w r5,r5 */
L8c6180ac: sa_motion_publish_stop(ctx,0x8c6180acu); r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6180ae; /* mov.w @r13+,r7 */
L8c6180ae: sa_motion_publish_stop(ctx,0x8c6180aeu); r[5]<<=2;goto L8c6180b0; /* shll2 r5 */
L8c6180b0: sa_motion_publish_stop(ctx,0x8c6180b0u); r[5]<<=2;goto L8c6180b2; /* shll2 r5 */
L8c6180b2: sa_motion_publish_stop(ctx,0x8c6180b2u); r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6180b4; /* mov.w @r13+,r14 */
L8c6180b4: sa_motion_publish_stop(ctx,0x8c6180b4u); r[6]<<=2;goto L8c6180b6; /* shll2 r6 */
L8c6180b6: sa_motion_publish_stop(ctx,0x8c6180b6u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c6180b8; /* shll r6 */
L8c6180b8: sa_motion_publish_stop(ctx,0x8c6180b8u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6180ba; /* shll r5 */
L8c6180ba: sa_motion_publish_stop(ctx,0x8c6180bau); r[5]+=r[12];goto L8c6180bc; /* add r12,r5 */
L8c6180bc: sa_motion_publish_stop(ctx,0x8c6180bcu); r[6]+=r[12];goto L8c6180be; /* add r12,r6 */
L8c6180be: sa_motion_publish_stop(ctx,0x8c6180beu); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c6180c0; /* fmov.s @(r0,r5),fr4 */
L8c6180c0: sa_motion_publish_stop(ctx,0x8c6180c0u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6180c2; /* fmov.s @(r0,r6),fr3 */
L8c6180c2: sa_motion_publish_stop(ctx,0x8c6180c2u); r[7]=r[7]&65535u;goto L8c6180c4; /* extu.w r7,r7 */
L8c6180c4: sa_motion_publish_stop(ctx,0x8c6180c4u); fcopy(f,2u,4u);goto L8c6180c6; /* fmov fr4,fr2 */
L8c6180c6: sa_motion_publish_stop(ctx,0x8c6180c6u); r[7]<<=2;goto L8c6180c8; /* shll2 r7 */
L8c6180c8: sa_motion_publish_stop(ctx,0x8c6180c8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6180ca; /* fsub fr3,fr2 */
L8c6180ca: sa_motion_publish_stop(ctx,0x8c6180cau); r[14]=r[14]&65535u;goto L8c6180cc; /* extu.w r14,r14 */
L8c6180cc: sa_motion_publish_stop(ctx,0x8c6180ccu); r[7]<<=2;goto L8c6180ce; /* shll2 r7 */
L8c6180ce: sa_motion_publish_stop(ctx,0x8c6180ceu); r[14]<<=2;goto L8c6180d0; /* shll2 r14 */
L8c6180d0: sa_motion_publish_stop(ctx,0x8c6180d0u); r[0]=20u;goto L8c6180d2; /* mov #0x14,r0 */
L8c6180d2: sa_motion_publish_stop(ctx,0x8c6180d2u); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c6180d4; /* fmov.s @(r0,r5),fr5 */
L8c6180d4: sa_motion_publish_stop(ctx,0x8c6180d4u); r[14]<<=2;goto L8c6180d6; /* shll2 r14 */
L8c6180d6: sa_motion_publish_stop(ctx,0x8c6180d6u); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c6180d8; /* shll r7 */
L8c6180d8: sa_motion_publish_stop(ctx,0x8c6180d8u); r[7]+=r[12];goto L8c6180da; /* add r12,r7 */
L8c6180da: sa_motion_publish_stop(ctx,0x8c6180dau); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c6180dc; /* shll r14 */
L8c6180dc: sa_motion_publish_stop(ctx,0x8c6180dcu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6180de; /* fmov.s @(r0,r7),fr3 */
L8c6180de: sa_motion_publish_stop(ctx,0x8c6180deu); fcopy(f,1u,5u);goto L8c6180e0; /* fmov fr5,fr1 */
L8c6180e0: sa_motion_publish_stop(ctx,0x8c6180e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6180e2; /* fsub fr5,fr3 */
L8c6180e2: sa_motion_publish_stop(ctx,0x8c6180e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6180e4; /* fmul fr3,fr2 */
L8c6180e4: sa_motion_publish_stop(ctx,0x8c6180e4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6180e6; /* fmov.s @(r0,r6),fr3 */
L8c6180e6: sa_motion_publish_stop(ctx,0x8c6180e6u); r[0]=16u;goto L8c6180e8; /* mov #0x10,r0 */
L8c6180e8: sa_motion_publish_stop(ctx,0x8c6180e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6180ea; /* fsub fr3,fr1 */
L8c6180ea: sa_motion_publish_stop(ctx,0x8c6180eau); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6180ec; /* fmov.s @(r0,r7),fr3 */
L8c6180ec: sa_motion_publish_stop(ctx,0x8c6180ecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6180ee; /* fsub fr4,fr3 */
L8c6180ee: sa_motion_publish_stop(ctx,0x8c6180eeu); fcopy(f,4u,2u);goto L8c6180f0; /* fmov fr2,fr4 */
L8c6180f0: sa_motion_publish_stop(ctx,0x8c6180f0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6180f2; /* fmul fr3,fr1 */
L8c6180f2: sa_motion_publish_stop(ctx,0x8c6180f2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6180f4; /* fldi0 fr3 */
L8c6180f4: sa_motion_publish_stop(ctx,0x8c6180f4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c6180f6; /* fsub fr1,fr4 */
L8c6180f6: sa_motion_publish_stop(ctx,0x8c6180f6u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c6180f8; /* fcmp/gt fr3,fr4 */
L8c6180f8: sa_motion_publish_stop(ctx,0x8c6180fau); take=!(*sr&1u);r[14]+=r[12];if(take)goto L8c618100;goto L8c6180fc; /* bf/s 0x8c618100 */
L8c6180fc: sa_motion_publish_stop(ctx,0x8c6180feu); goto L8c61829c; /* bra 0x8c61829c */
L8c618100: sa_motion_publish_stop(ctx,0x8c618100u); r[3]=rd(m,bus,&io_status,0x8c618320u,4);if(io_status)return io_status;goto L8c618102; /* mov.l 0x8c618320,r3 */
L8c618102: sa_motion_publish_stop(ctx,0x8c618102u); r[0]=4u;goto L8c618104; /* mov #0x4,r0 */
L8c618104: sa_motion_publish_stop(ctx,0x8c618104u); fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c618106; /* fmov.s @r6,fr3 */
L8c618106: sa_motion_publish_stop(ctx,0x8c618106u); r[8]+=32u;goto L8c618108; /* add #0x20,r8 */
L8c618108: sa_motion_publish_stop(ctx,0x8c618108u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61810a; /* fmov.s @r3,fr2 */
L8c61810a: sa_motion_publish_stop(ctx,0x8c61810au); r[3]+=8u;goto L8c61810c; /* add #0x8,r3 */
L8c61810c: sa_motion_publish_stop(ctx,0x8c61810cu); r[2]=rd(m,bus,&io_status,0x8c618324u,4);if(io_status)return io_status;goto L8c61810e; /* mov.l 0x8c618324,r2 */
L8c61810e: sa_motion_publish_stop(ctx,0x8c61810eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618110; /* fmul fr3,fr2 */
L8c618110: sa_motion_publish_stop(ctx,0x8c618110u); r[1]=rd(m,bus,&io_status,0x8c618328u,4);if(io_status)return io_status;goto L8c618112; /* mov.l 0x8c618328,r1 */
L8c618112: sa_motion_publish_stop(ctx,0x8c618112u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618114; /* fmov.s @r2,fr3 */
L8c618114: sa_motion_publish_stop(ctx,0x8c618114u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c618116; /* fmov.s @r1,fr1 */
L8c618116: sa_motion_publish_stop(ctx,0x8c618116u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618118; /* fsub fr2,fr3 */
L8c618118: sa_motion_publish_stop(ctx,0x8c618118u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61811a; /* fmov.s @(r0,r6),fr2 */
L8c61811a: sa_motion_publish_stop(ctx,0x8c61811au); r[0]=8u;goto L8c61811c; /* mov #0x8,r0 */
L8c61811c: sa_motion_publish_stop(ctx,0x8c61811cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61811e; /* fmul fr2,fr1 */
L8c61811e: sa_motion_publish_stop(ctx,0x8c61811eu); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c618120; /* fmov.s @(r0,r6),fr2 */
L8c618120: sa_motion_publish_stop(ctx,0x8c618120u); r[0]=28u;goto L8c618122; /* mov #0x1c,r0 */
L8c618122: sa_motion_publish_stop(ctx,0x8c618122u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618124; /* fsub fr1,fr3 */
L8c618124: sa_motion_publish_stop(ctx,0x8c618124u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618126; /* fmov.s @r3,fr1 */
L8c618126: sa_motion_publish_stop(ctx,0x8c618126u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618128; /* fmul fr2,fr1 */
L8c618128: sa_motion_publish_stop(ctx,0x8c618128u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61812a; /* fsub fr1,fr3 */
L8c61812a: sa_motion_publish_stop(ctx,0x8c61812au); fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61812c; /* fmov fr3,@(r0,r6) */
L8c61812c: sa_motion_publish_stop(ctx,0x8c61812cu); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61812e; /* fmov.s @(r0,r9),fr2 */
L8c61812e: sa_motion_publish_stop(ctx,0x8c61812eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618130; /* fmul fr3,fr2 */
L8c618130: sa_motion_publish_stop(ctx,0x8c618130u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618132; /* fmov.s fr2,@-r8 */
L8c618132: sa_motion_publish_stop(ctx,0x8c618132u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618134; /* fmov.s @(r0,r6),fr3 */
L8c618134: sa_motion_publish_stop(ctx,0x8c618134u); r[0]=24u;goto L8c618136; /* mov #0x18,r0 */
L8c618136: sa_motion_publish_stop(ctx,0x8c618136u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618138; /* fmov.s @(r0,r9),fr2 */
L8c618138: sa_motion_publish_stop(ctx,0x8c618138u); r[0]=28u;goto L8c61813a; /* mov #0x1c,r0 */
L8c61813a: sa_motion_publish_stop(ctx,0x8c61813au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61813c; /* fmul fr3,fr2 */
L8c61813c: sa_motion_publish_stop(ctx,0x8c61813cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61813e; /* fmov.s fr2,@-r8 */
L8c61813e: sa_motion_publish_stop(ctx,0x8c61813eu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618140; /* fmov.s @(r0,r6),fr3 */
L8c618140: sa_motion_publish_stop(ctx,0x8c618140u); r[0]=20u;goto L8c618142; /* mov #0x14,r0 */
L8c618142: sa_motion_publish_stop(ctx,0x8c618142u); r[2]=rd(m,bus,&io_status,0x8c61832cu,4);if(io_status)return io_status;goto L8c618144; /* mov.l 0x8c61832c,r2 */
L8c618144: sa_motion_publish_stop(ctx,0x8c618144u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618146; /* fmov.s @(r0,r9),fr2 */
L8c618146: sa_motion_publish_stop(ctx,0x8c618146u); r[0]=16u;goto L8c618148; /* mov #0x10,r0 */
L8c618148: sa_motion_publish_stop(ctx,0x8c618148u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61814a; /* fmul fr3,fr2 */
L8c61814a: sa_motion_publish_stop(ctx,0x8c61814au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61814c; /* fmov.s fr2,@-r8 */
L8c61814c: sa_motion_publish_stop(ctx,0x8c61814cu); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61814e; /* fmov.s @(r0,r9),fr3 */
L8c61814e: sa_motion_publish_stop(ctx,0x8c61814eu); r[0]=24u;goto L8c618150; /* mov #0x18,r0 */
L8c618150: sa_motion_publish_stop(ctx,0x8c618150u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618152; /* fmov.s fr3,@-r8 */
L8c618152: sa_motion_publish_stop(ctx,0x8c618152u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618154; /* fmov.s @(r0,r6),fr3 */
L8c618154: sa_motion_publish_stop(ctx,0x8c618154u); r[0]=20u;goto L8c618156; /* mov #0x14,r0 */
L8c618156: sa_motion_publish_stop(ctx,0x8c618156u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618158; /* fmov.s fr3,@-r8 */
L8c618158: sa_motion_publish_stop(ctx,0x8c618158u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61815a; /* fmov.s @(r0,r6),fr3 */
L8c61815a: sa_motion_publish_stop(ctx,0x8c61815au); r[0]=16u;goto L8c61815c; /* mov #0x10,r0 */
L8c61815c: sa_motion_publish_stop(ctx,0x8c61815cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61815e; /* fmov.s fr3,@-r8 */
L8c61815e: sa_motion_publish_stop(ctx,0x8c61815eu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618160; /* fmov.s @(r0,r6),fr3 */
L8c618160: sa_motion_publish_stop(ctx,0x8c618160u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618162; /* fmov.s fr3,@-r8 */
L8c618162: sa_motion_publish_stop(ctx,0x8c618162u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c618164; /* mov.l r2,@-r8 */
L8c618164: sa_motion_publish_stop(ctx,0x8c618164u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618166; /* pref @r8 */
L8c618166: sa_motion_publish_stop(ctx,0x8c618166u); r[2]=rd(m,bus,&io_status,0x8c618320u,4);if(io_status)return io_status;goto L8c618168; /* mov.l 0x8c618320,r2 */
L8c618168: sa_motion_publish_stop(ctx,0x8c618168u); r[0]=4u;goto L8c61816a; /* mov #0x4,r0 */
L8c61816a: sa_motion_publish_stop(ctx,0x8c61816au); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61816c; /* fmov.s @r5,fr3 */
L8c61816c: sa_motion_publish_stop(ctx,0x8c61816cu); r[8]+=64u;goto L8c61816e; /* add #0x40,r8 */
L8c61816e: sa_motion_publish_stop(ctx,0x8c61816eu); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618170; /* fmov.s @r2,fr2 */
L8c618170: sa_motion_publish_stop(ctx,0x8c618170u); r[3]=rd(m,bus,&io_status,0x8c618324u,4);if(io_status)return io_status;goto L8c618172; /* mov.l 0x8c618324,r3 */
L8c618172: sa_motion_publish_stop(ctx,0x8c618172u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618174; /* fmul fr3,fr2 */
L8c618174: sa_motion_publish_stop(ctx,0x8c618174u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c618176; /* fmov.s @r1,fr1 */
L8c618176: sa_motion_publish_stop(ctx,0x8c618176u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618178; /* fmov.s @r3,fr3 */
L8c618178: sa_motion_publish_stop(ctx,0x8c618178u); r[3]=rd(m,bus,&io_status,0x8c618330u,4);if(io_status)return io_status;goto L8c61817a; /* mov.l 0x8c618330,r3 */
L8c61817a: sa_motion_publish_stop(ctx,0x8c61817au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61817c; /* fsub fr2,fr3 */
L8c61817c: sa_motion_publish_stop(ctx,0x8c61817cu); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61817e; /* fmov.s @(r0,r5),fr2 */
L8c61817e: sa_motion_publish_stop(ctx,0x8c61817eu); r[0]=8u;goto L8c618180; /* mov #0x8,r0 */
L8c618180: sa_motion_publish_stop(ctx,0x8c618180u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618182; /* fmul fr2,fr1 */
L8c618182: sa_motion_publish_stop(ctx,0x8c618182u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c618184; /* fmov.s @(r0,r5),fr2 */
L8c618184: sa_motion_publish_stop(ctx,0x8c618184u); r[0]=28u;goto L8c618186; /* mov #0x1c,r0 */
L8c618186: sa_motion_publish_stop(ctx,0x8c618186u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618188; /* fsub fr1,fr3 */
L8c618188: sa_motion_publish_stop(ctx,0x8c618188u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61818a; /* fmov.s @r3,fr1 */
L8c61818a: sa_motion_publish_stop(ctx,0x8c61818au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61818c; /* fmul fr2,fr1 */
L8c61818c: sa_motion_publish_stop(ctx,0x8c61818cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61818e; /* fsub fr1,fr3 */
L8c61818e: sa_motion_publish_stop(ctx,0x8c61818eu); fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618190; /* fmov fr3,@(r0,r5) */
L8c618190: sa_motion_publish_stop(ctx,0x8c618190u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618192; /* fmov.s @(r0,r9),fr2 */
L8c618192: sa_motion_publish_stop(ctx,0x8c618192u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618194; /* fmul fr3,fr2 */
L8c618194: sa_motion_publish_stop(ctx,0x8c618194u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618196; /* fmov.s fr2,@-r8 */
L8c618196: sa_motion_publish_stop(ctx,0x8c618196u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618198; /* fmov.s @(r0,r5),fr3 */
L8c618198: sa_motion_publish_stop(ctx,0x8c618198u); r[0]=24u;goto L8c61819a; /* mov #0x18,r0 */
L8c61819a: sa_motion_publish_stop(ctx,0x8c61819au); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61819c; /* fmov.s @(r0,r9),fr2 */
L8c61819c: sa_motion_publish_stop(ctx,0x8c61819cu); r[0]=28u;goto L8c61819e; /* mov #0x1c,r0 */
L8c61819e: sa_motion_publish_stop(ctx,0x8c61819eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6181a0; /* fmul fr3,fr2 */
L8c6181a0: sa_motion_publish_stop(ctx,0x8c6181a0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6181a2; /* fmov.s fr2,@-r8 */
L8c6181a2: sa_motion_publish_stop(ctx,0x8c6181a2u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6181a4; /* fmov.s @(r0,r5),fr3 */
L8c6181a4: sa_motion_publish_stop(ctx,0x8c6181a4u); r[0]=20u;goto L8c6181a6; /* mov #0x14,r0 */
L8c6181a6: sa_motion_publish_stop(ctx,0x8c6181a6u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c6181a8; /* fmov.s @(r0,r9),fr2 */
L8c6181a8: sa_motion_publish_stop(ctx,0x8c6181a8u); r[0]=16u;goto L8c6181aa; /* mov #0x10,r0 */
L8c6181aa: sa_motion_publish_stop(ctx,0x8c6181aau); r[2]=rd(m,bus,&io_status,0x8c61832cu,4);if(io_status)return io_status;goto L8c6181ac; /* mov.l 0x8c61832c,r2 */
L8c6181ac: sa_motion_publish_stop(ctx,0x8c6181acu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6181ae; /* fmul fr3,fr2 */
L8c6181ae: sa_motion_publish_stop(ctx,0x8c6181aeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6181b0; /* fmov.s fr2,@-r8 */
L8c6181b0: sa_motion_publish_stop(ctx,0x8c6181b0u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6181b2; /* fmov.s @(r0,r9),fr3 */
L8c6181b2: sa_motion_publish_stop(ctx,0x8c6181b2u); r[0]=24u;goto L8c6181b4; /* mov #0x18,r0 */
L8c6181b4: sa_motion_publish_stop(ctx,0x8c6181b4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6181b6; /* fmov.s fr3,@-r8 */
L8c6181b6: sa_motion_publish_stop(ctx,0x8c6181b6u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6181b8; /* fmov.s @(r0,r5),fr3 */
L8c6181b8: sa_motion_publish_stop(ctx,0x8c6181b8u); r[0]=20u;goto L8c6181ba; /* mov #0x14,r0 */
L8c6181ba: sa_motion_publish_stop(ctx,0x8c6181bau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6181bc; /* fmov.s fr3,@-r8 */
L8c6181bc: sa_motion_publish_stop(ctx,0x8c6181bcu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6181be; /* fmov.s @(r0,r5),fr3 */
L8c6181be: sa_motion_publish_stop(ctx,0x8c6181beu); r[0]=16u;goto L8c6181c0; /* mov #0x10,r0 */
L8c6181c0: sa_motion_publish_stop(ctx,0x8c6181c0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6181c2; /* fmov.s fr3,@-r8 */
L8c6181c2: sa_motion_publish_stop(ctx,0x8c6181c2u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6181c4; /* fmov.s @(r0,r5),fr3 */
L8c6181c4: sa_motion_publish_stop(ctx,0x8c6181c4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6181c6; /* fmov.s fr3,@-r8 */
L8c6181c6: sa_motion_publish_stop(ctx,0x8c6181c6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c6181c8; /* mov.l r2,@-r8 */
L8c6181c8: sa_motion_publish_stop(ctx,0x8c6181c8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6181ca; /* pref @r8 */
L8c6181ca: sa_motion_publish_stop(ctx,0x8c6181cau); r[2]=rd(m,bus,&io_status,0x8c618320u,4);if(io_status)return io_status;goto L8c6181cc; /* mov.l 0x8c618320,r2 */
L8c6181cc: sa_motion_publish_stop(ctx,0x8c6181ccu); r[0]=4u;goto L8c6181ce; /* mov #0x4,r0 */
L8c6181ce: sa_motion_publish_stop(ctx,0x8c6181ceu); fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c6181d0; /* fmov.s @r7,fr3 */
L8c6181d0: sa_motion_publish_stop(ctx,0x8c6181d0u); r[8]+=64u;goto L8c6181d2; /* add #0x40,r8 */
L8c6181d2: sa_motion_publish_stop(ctx,0x8c6181d2u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c6181d4; /* fmov.s @r2,fr2 */
L8c6181d4: sa_motion_publish_stop(ctx,0x8c6181d4u); r[3]=rd(m,bus,&io_status,0x8c618324u,4);if(io_status)return io_status;goto L8c6181d6; /* mov.l 0x8c618324,r3 */
L8c6181d6: sa_motion_publish_stop(ctx,0x8c6181d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6181d8; /* fmul fr3,fr2 */
L8c6181d8: sa_motion_publish_stop(ctx,0x8c6181d8u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c6181da; /* fmov.s @r1,fr1 */
L8c6181da: sa_motion_publish_stop(ctx,0x8c6181dau); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6181dc; /* fmov.s @r3,fr3 */
L8c6181dc: sa_motion_publish_stop(ctx,0x8c6181dcu); r[3]=rd(m,bus,&io_status,0x8c618330u,4);if(io_status)return io_status;goto L8c6181de; /* mov.l 0x8c618330,r3 */
L8c6181de: sa_motion_publish_stop(ctx,0x8c6181deu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6181e0; /* fsub fr2,fr3 */
L8c6181e0: sa_motion_publish_stop(ctx,0x8c6181e0u); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c6181e2; /* fmov.s @(r0,r7),fr2 */
L8c6181e2: sa_motion_publish_stop(ctx,0x8c6181e2u); r[0]=8u;goto L8c6181e4; /* mov #0x8,r0 */
L8c6181e4: sa_motion_publish_stop(ctx,0x8c6181e4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6181e6; /* fmul fr2,fr1 */
L8c6181e6: sa_motion_publish_stop(ctx,0x8c6181e6u); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c6181e8; /* fmov.s @(r0,r7),fr2 */
L8c6181e8: sa_motion_publish_stop(ctx,0x8c6181e8u); r[0]=28u;goto L8c6181ea; /* mov #0x1c,r0 */
L8c6181ea: sa_motion_publish_stop(ctx,0x8c6181eau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6181ec; /* fsub fr1,fr3 */
L8c6181ec: sa_motion_publish_stop(ctx,0x8c6181ecu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6181ee; /* fmov.s @r3,fr1 */
L8c6181ee: sa_motion_publish_stop(ctx,0x8c6181eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6181f0; /* fmul fr2,fr1 */
L8c6181f0: sa_motion_publish_stop(ctx,0x8c6181f0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6181f2; /* fsub fr1,fr3 */
L8c6181f2: sa_motion_publish_stop(ctx,0x8c6181f2u); fstore(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6181f4; /* fmov fr3,@(r0,r7) */
L8c6181f4: sa_motion_publish_stop(ctx,0x8c6181f4u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c6181f6; /* fmov.s @(r0,r9),fr2 */
L8c6181f6: sa_motion_publish_stop(ctx,0x8c6181f6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6181f8; /* fmul fr3,fr2 */
L8c6181f8: sa_motion_publish_stop(ctx,0x8c6181f8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6181fa; /* fmov.s fr2,@-r8 */
L8c6181fa: sa_motion_publish_stop(ctx,0x8c6181fau); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6181fc; /* fmov.s @(r0,r7),fr3 */
L8c6181fc: sa_motion_publish_stop(ctx,0x8c6181fcu); r[0]=24u;goto L8c6181fe; /* mov #0x18,r0 */
L8c6181fe: sa_motion_publish_stop(ctx,0x8c6181feu); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618200; /* fmov.s @(r0,r9),fr2 */
L8c618200: sa_motion_publish_stop(ctx,0x8c618200u); r[0]=28u;goto L8c618202; /* mov #0x1c,r0 */
L8c618202: sa_motion_publish_stop(ctx,0x8c618202u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618204; /* fmul fr3,fr2 */
L8c618204: sa_motion_publish_stop(ctx,0x8c618204u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618206; /* fmov.s fr2,@-r8 */
L8c618206: sa_motion_publish_stop(ctx,0x8c618206u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618208; /* fmov.s @(r0,r7),fr3 */
L8c618208: sa_motion_publish_stop(ctx,0x8c618208u); r[0]=20u;goto L8c61820a; /* mov #0x14,r0 */
L8c61820a: sa_motion_publish_stop(ctx,0x8c61820au); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61820c; /* fmov.s @(r0,r9),fr2 */
L8c61820c: sa_motion_publish_stop(ctx,0x8c61820cu); r[0]=16u;goto L8c61820e; /* mov #0x10,r0 */
L8c61820e: sa_motion_publish_stop(ctx,0x8c61820eu); r[2]=rd(m,bus,&io_status,0x8c61832cu,4);if(io_status)return io_status;goto L8c618210; /* mov.l 0x8c61832c,r2 */
L8c618210: sa_motion_publish_stop(ctx,0x8c618210u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618212; /* fmul fr3,fr2 */
L8c618212: sa_motion_publish_stop(ctx,0x8c618212u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618214; /* fmov.s fr2,@-r8 */
L8c618214: sa_motion_publish_stop(ctx,0x8c618214u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c618216; /* fmov.s @(r0,r9),fr3 */
L8c618216: sa_motion_publish_stop(ctx,0x8c618216u); r[0]=24u;goto L8c618218; /* mov #0x18,r0 */
L8c618218: sa_motion_publish_stop(ctx,0x8c618218u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61821a; /* fmov.s fr3,@-r8 */
L8c61821a: sa_motion_publish_stop(ctx,0x8c61821au); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61821c; /* fmov.s @(r0,r7),fr3 */
L8c61821c: sa_motion_publish_stop(ctx,0x8c61821cu); r[0]=20u;goto L8c61821e; /* mov #0x14,r0 */
L8c61821e: sa_motion_publish_stop(ctx,0x8c61821eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618220; /* fmov.s fr3,@-r8 */
L8c618220: sa_motion_publish_stop(ctx,0x8c618220u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618222; /* fmov.s @(r0,r7),fr3 */
L8c618222: sa_motion_publish_stop(ctx,0x8c618222u); r[0]=16u;goto L8c618224; /* mov #0x10,r0 */
L8c618224: sa_motion_publish_stop(ctx,0x8c618224u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618226; /* fmov.s fr3,@-r8 */
L8c618226: sa_motion_publish_stop(ctx,0x8c618226u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c618228; /* fmov.s @(r0,r7),fr3 */
L8c618228: sa_motion_publish_stop(ctx,0x8c618228u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61822a; /* fmov.s fr3,@-r8 */
L8c61822a: sa_motion_publish_stop(ctx,0x8c61822au); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61822c; /* mov.l r2,@-r8 */
L8c61822c: sa_motion_publish_stop(ctx,0x8c61822cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61822e; /* pref @r8 */
L8c61822e: sa_motion_publish_stop(ctx,0x8c61822eu); r[2]=rd(m,bus,&io_status,0x8c618320u,4);if(io_status)return io_status;goto L8c618230; /* mov.l 0x8c618320,r2 */
L8c618230: sa_motion_publish_stop(ctx,0x8c618230u); r[0]=4u;goto L8c618232; /* mov #0x4,r0 */
L8c618232: sa_motion_publish_stop(ctx,0x8c618232u); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c618234; /* fmov.s @r14,fr3 */
L8c618234: sa_motion_publish_stop(ctx,0x8c618234u); r[8]+=64u;goto L8c618236; /* add #0x40,r8 */
L8c618236: sa_motion_publish_stop(ctx,0x8c618236u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618238; /* fmov.s @r2,fr2 */
L8c618238: sa_motion_publish_stop(ctx,0x8c618238u); r[3]=rd(m,bus,&io_status,0x8c618324u,4);if(io_status)return io_status;goto L8c61823a; /* mov.l 0x8c618324,r3 */
L8c61823a: sa_motion_publish_stop(ctx,0x8c61823au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61823c; /* fmul fr3,fr2 */
L8c61823c: sa_motion_publish_stop(ctx,0x8c61823cu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61823e; /* fmov.s @r1,fr1 */
L8c61823e: sa_motion_publish_stop(ctx,0x8c61823eu); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618240; /* fmov.s @r3,fr3 */
L8c618240: sa_motion_publish_stop(ctx,0x8c618240u); r[3]=rd(m,bus,&io_status,0x8c618330u,4);if(io_status)return io_status;goto L8c618242; /* mov.l 0x8c618330,r3 */
L8c618242: sa_motion_publish_stop(ctx,0x8c618242u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618244; /* fsub fr2,fr3 */
L8c618244: sa_motion_publish_stop(ctx,0x8c618244u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c618246; /* fmov.s @(r0,r14),fr2 */
L8c618246: sa_motion_publish_stop(ctx,0x8c618246u); r[0]=8u;goto L8c618248; /* mov #0x8,r0 */
L8c618248: sa_motion_publish_stop(ctx,0x8c618248u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61824a; /* fmul fr2,fr1 */
L8c61824a: sa_motion_publish_stop(ctx,0x8c61824au); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61824c; /* fmov.s @(r0,r14),fr2 */
L8c61824c: sa_motion_publish_stop(ctx,0x8c61824cu); r[0]=28u;goto L8c61824e; /* mov #0x1c,r0 */
L8c61824e: sa_motion_publish_stop(ctx,0x8c61824eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618250; /* fsub fr1,fr3 */
L8c618250: sa_motion_publish_stop(ctx,0x8c618250u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618252; /* fmov.s @r3,fr1 */
L8c618252: sa_motion_publish_stop(ctx,0x8c618252u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618254; /* fmul fr2,fr1 */
L8c618254: sa_motion_publish_stop(ctx,0x8c618254u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618256; /* fsub fr1,fr3 */
L8c618256: sa_motion_publish_stop(ctx,0x8c618256u); fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618258; /* fmov fr3,@(r0,r14) */
L8c618258: sa_motion_publish_stop(ctx,0x8c618258u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61825a; /* fmov.s @(r0,r9),fr2 */
L8c61825a: sa_motion_publish_stop(ctx,0x8c61825au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61825c; /* fmul fr3,fr2 */
L8c61825c: sa_motion_publish_stop(ctx,0x8c61825cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61825e; /* fmov.s fr2,@-r8 */
L8c61825e: sa_motion_publish_stop(ctx,0x8c61825eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618260; /* fmov.s @(r0,r14),fr3 */
L8c618260: sa_motion_publish_stop(ctx,0x8c618260u); r[0]=24u;goto L8c618262; /* mov #0x18,r0 */
L8c618262: sa_motion_publish_stop(ctx,0x8c618262u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618264; /* fmov.s @(r0,r9),fr2 */
L8c618264: sa_motion_publish_stop(ctx,0x8c618264u); r[0]=28u;goto L8c618266; /* mov #0x1c,r0 */
L8c618266: sa_motion_publish_stop(ctx,0x8c618266u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618268; /* fmul fr3,fr2 */
L8c618268: sa_motion_publish_stop(ctx,0x8c618268u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61826a; /* fmov.s fr2,@-r8 */
L8c61826a: sa_motion_publish_stop(ctx,0x8c61826au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61826c; /* fmov.s @(r0,r14),fr3 */
L8c61826c: sa_motion_publish_stop(ctx,0x8c61826cu); r[0]=20u;goto L8c61826e; /* mov #0x14,r0 */
L8c61826e: sa_motion_publish_stop(ctx,0x8c61826eu); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618270; /* fmov.s @(r0,r9),fr2 */
L8c618270: sa_motion_publish_stop(ctx,0x8c618270u); r[0]=16u;goto L8c618272; /* mov #0x10,r0 */
L8c618272: sa_motion_publish_stop(ctx,0x8c618272u); r[2]=rd(m,bus,&io_status,0x8c618334u,4);if(io_status)return io_status;goto L8c618274; /* mov.l 0x8c618334,r2 */
L8c618274: sa_motion_publish_stop(ctx,0x8c618274u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618276; /* fmul fr3,fr2 */
L8c618276: sa_motion_publish_stop(ctx,0x8c618276u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618278; /* fmov.s fr2,@-r8 */
L8c618278: sa_motion_publish_stop(ctx,0x8c618278u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61827a; /* fmov.s @(r0,r9),fr3 */
L8c61827a: sa_motion_publish_stop(ctx,0x8c61827au); r[0]=24u;goto L8c61827c; /* mov #0x18,r0 */
L8c61827c: sa_motion_publish_stop(ctx,0x8c61827cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61827e; /* fmov.s fr3,@-r8 */
L8c61827e: sa_motion_publish_stop(ctx,0x8c61827eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618280; /* fmov.s @(r0,r14),fr3 */
L8c618280: sa_motion_publish_stop(ctx,0x8c618280u); r[0]=20u;goto L8c618282; /* mov #0x14,r0 */
L8c618282: sa_motion_publish_stop(ctx,0x8c618282u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618284; /* fmov.s fr3,@-r8 */
L8c618284: sa_motion_publish_stop(ctx,0x8c618284u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618286; /* fmov.s @(r0,r14),fr3 */
L8c618286: sa_motion_publish_stop(ctx,0x8c618286u); r[0]=16u;goto L8c618288; /* mov #0x10,r0 */
L8c618288: sa_motion_publish_stop(ctx,0x8c618288u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61828a; /* fmov.s fr3,@-r8 */
L8c61828a: sa_motion_publish_stop(ctx,0x8c61828au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61828c; /* fmov.s @(r0,r14),fr3 */
L8c61828c: sa_motion_publish_stop(ctx,0x8c61828cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61828e; /* fmov.s fr3,@-r8 */
L8c61828e: sa_motion_publish_stop(ctx,0x8c61828eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c618290; /* mov.l r2,@-r8 */
L8c618290: sa_motion_publish_stop(ctx,0x8c618290u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618292; /* pref @r8 */
L8c618292: sa_motion_publish_stop(ctx,0x8c618292u); r[0]=rd(m,bus,&io_status,0x8c618338u,4);if(io_status)return io_status;goto L8c618294; /* mov.l 0x8c618338,r0 */
L8c618294: sa_motion_publish_stop(ctx,0x8c618294u); r[8]+=32u;goto L8c618296; /* add #0x20,r8 */
L8c618296: sa_motion_publish_stop(ctx,0x8c618296u); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c618298; /* mov.l @r0,r2 */
L8c618298: sa_motion_publish_stop(ctx,0x8c618298u); r[2]+=4u;goto L8c61829a; /* add #0x4,r2 */
L8c61829a: sa_motion_publish_stop(ctx,0x8c61829au); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61829c; /* mov.l r2,@r0 */
L8c61829c: sa_motion_publish_stop(ctx,0x8c61829cu); r[11]+=1u;goto L8c61829e; /* add #0x1,r11 */
L8c61829e: sa_motion_publish_stop(ctx,0x8c61829eu); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c6182a0; /* mov.w @(0x2,r4),r0 */
L8c6182a0: sa_motion_publish_stop(ctx,0x8c6182a0u); r[0]=r[0]&65535u;goto L8c6182a2; /* extu.w r0,r0 */
L8c6182a2: sa_motion_publish_stop(ctx,0x8c6182a2u); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c6182a4; /* cmp/ge r0,r11 */
L8c6182a4: sa_motion_publish_stop(ctx,0x8c6182a4u); if((*sr&1u))goto L8c6182aa;goto L8c6182a6; /* bt 0x8c6182aa */
L8c6182a6: sa_motion_publish_stop(ctx,0x8c6182a8u); goto L8c61809c; /* bra 0x8c61809c */
L8c6182aa: sa_motion_publish_stop(ctx,0x8c6182acu); goto L8c6184aa; /* bra 0x8c6184aa */
L8c6182ae: sa_motion_publish_stop(ctx,0x8c6182b0u); r[10]=r[12];goto L8c618446; /* bra 0x8c618446 */
L8c6182b2: sa_motion_publish_stop(ctx,0x8c6182b2u); r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6182b4; /* mov.w @r13+,r14 */
L8c6182b4: sa_motion_publish_stop(ctx,0x8c6182b4u); r[3]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c6182b6; /* mov.l @r11,r3 */
L8c6182b6: sa_motion_publish_stop(ctx,0x8c6182b6u); r[14]=r[14]&65535u;goto L8c6182b8; /* extu.w r14,r14 */
L8c6182b8: sa_motion_publish_stop(ctx,0x8c6182b8u); r[2]=rd(m,bus,&io_status,0x8c61831cu,4);if(io_status)return io_status;goto L8c6182ba; /* mov.l 0x8c61831c,r2 */
L8c6182ba: sa_motion_publish_stop(ctx,0x8c6182bau); r[3]+=r[14];goto L8c6182bc; /* add r14,r3 */
L8c6182bc: sa_motion_publish_stop(ctx,0x8c6182bcu); wr(m,bus,&io_status,r[11],r[3],4);if(io_status)return io_status;goto L8c6182be; /* mov.l r3,@r11 */
L8c6182be: sa_motion_publish_stop(ctx,0x8c6182beu); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c6182c0; /* mov.w @(0x2,r13),r0 */
L8c6182c0: sa_motion_publish_stop(ctx,0x8c6182c0u); r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c6182c2; /* mov.w @r13,r7 */
L8c6182c2: sa_motion_publish_stop(ctx,0x8c6182c2u); r[5]=r[0]&65535u;goto L8c6182c4; /* extu.w r0,r5 */
L8c6182c4: sa_motion_publish_stop(ctx,0x8c6182c4u); r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c6182c6; /* mov.w @(0x4,r13),r0 */
L8c6182c6: sa_motion_publish_stop(ctx,0x8c6182c6u); r[5]<<=2;goto L8c6182c8; /* shll2 r5 */
L8c6182c8: sa_motion_publish_stop(ctx,0x8c6182c8u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6182ca; /* mov.l @r2,r3 */
L8c6182ca: sa_motion_publish_stop(ctx,0x8c6182cau); r[7]=r[7]&65535u;goto L8c6182cc; /* extu.w r7,r7 */
L8c6182cc: sa_motion_publish_stop(ctx,0x8c6182ccu); r[7]<<=2;goto L8c6182ce; /* shll2 r7 */
L8c6182ce: sa_motion_publish_stop(ctx,0x8c6182ceu); r[5]<<=2;goto L8c6182d0; /* shll2 r5 */
L8c6182d0: sa_motion_publish_stop(ctx,0x8c6182d0u); r[6]=r[0]&65535u;goto L8c6182d2; /* extu.w r0,r6 */
L8c6182d2: sa_motion_publish_stop(ctx,0x8c6182d2u); r[7]<<=2;goto L8c6182d4; /* shll2 r7 */
L8c6182d4: sa_motion_publish_stop(ctx,0x8c6182d4u); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c6182d6; /* shll r7 */
L8c6182d6: sa_motion_publish_stop(ctx,0x8c6182d6u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6182d8; /* shll r5 */
L8c6182d8: sa_motion_publish_stop(ctx,0x8c6182d8u); r[0]=16u;goto L8c6182da; /* mov #0x10,r0 */
L8c6182da: sa_motion_publish_stop(ctx,0x8c6182dau); r[5]+=r[3];goto L8c6182dc; /* add r3,r5 */
L8c6182dc: sa_motion_publish_stop(ctx,0x8c6182dcu); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c6182de; /* fmov.s @(r0,r5),fr5 */
L8c6182de: sa_motion_publish_stop(ctx,0x8c6182deu); r[7]+=r[3];goto L8c6182e0; /* add r3,r7 */
L8c6182e0: sa_motion_publish_stop(ctx,0x8c6182e0u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6182e2; /* fmov.s @(r0,r7),fr3 */
L8c6182e2: sa_motion_publish_stop(ctx,0x8c6182e2u); r[6]<<=2;goto L8c6182e4; /* shll2 r6 */
L8c6182e4: sa_motion_publish_stop(ctx,0x8c6182e4u); fcopy(f,2u,5u);goto L8c6182e6; /* fmov fr5,fr2 */
L8c6182e6: sa_motion_publish_stop(ctx,0x8c6182e6u); r[6]<<=2;goto L8c6182e8; /* shll2 r6 */
L8c6182e8: sa_motion_publish_stop(ctx,0x8c6182e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6182ea; /* fsub fr3,fr2 */
L8c6182ea: sa_motion_publish_stop(ctx,0x8c6182eau); r[0]=20u;goto L8c6182ec; /* mov #0x14,r0 */
L8c6182ec: sa_motion_publish_stop(ctx,0x8c6182ecu); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c6182ee; /* shll r6 */
L8c6182ee: sa_motion_publish_stop(ctx,0x8c6182eeu); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c6182f0; /* fmov.s @(r0,r5),fr4 */
L8c6182f0: sa_motion_publish_stop(ctx,0x8c6182f0u); r[6]+=r[3];goto L8c6182f2; /* add r3,r6 */
L8c6182f2: sa_motion_publish_stop(ctx,0x8c6182f2u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6182f4; /* fmov.s @(r0,r6),fr3 */
L8c6182f4: sa_motion_publish_stop(ctx,0x8c6182f4u); fcopy(f,1u,4u);goto L8c6182f6; /* fmov fr4,fr1 */
L8c6182f6: sa_motion_publish_stop(ctx,0x8c6182f6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6182f8; /* fsub fr4,fr3 */
L8c6182f8: sa_motion_publish_stop(ctx,0x8c6182f8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6182fa; /* fmul fr3,fr2 */
L8c6182fa: sa_motion_publish_stop(ctx,0x8c6182fau); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6182fc; /* fmov.s @(r0,r7),fr3 */
L8c6182fc: sa_motion_publish_stop(ctx,0x8c6182fcu); r[0]=16u;goto L8c6182fe; /* mov #0x10,r0 */
L8c6182fe: sa_motion_publish_stop(ctx,0x8c6182feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c618300; /* fsub fr3,fr1 */
L8c618300: sa_motion_publish_stop(ctx,0x8c618300u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618302; /* fmov.s @(r0,r6),fr3 */
L8c618302: sa_motion_publish_stop(ctx,0x8c618302u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618304; /* fsub fr5,fr3 */
L8c618304: sa_motion_publish_stop(ctx,0x8c618304u); fcopy(f,4u,2u);goto L8c618306; /* fmov fr2,fr4 */
L8c618306: sa_motion_publish_stop(ctx,0x8c618306u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618308; /* fmul fr3,fr1 */
L8c618308: sa_motion_publish_stop(ctx,0x8c618308u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61830a; /* fldi0 fr3 */
L8c61830a: sa_motion_publish_stop(ctx,0x8c61830au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61830c; /* fsub fr1,fr4 */
L8c61830c: sa_motion_publish_stop(ctx,0x8c61830cu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61830e; /* fcmp/gt fr3,fr4 */
L8c61830e: sa_motion_publish_stop(ctx,0x8c61830eu); if(!(*sr&1u))goto L8c61833c;goto L8c618310; /* bf 0x8c61833c */
L8c618310: sa_motion_publish_stop(ctx,0x8c618310u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c618312; /* shll r14 */
L8c618312: sa_motion_publish_stop(ctx,0x8c618314u); r[13]+=r[14];goto L8c618444; /* bra 0x8c618444 */
L8c61833c: sa_motion_publish_stop(ctx,0x8c61833cu); r[3]=rd(m,bus,&io_status,0x8c6184c8u,4);if(io_status)return io_status;goto L8c61833e; /* mov.l 0x8c6184c8,r3 */
L8c61833e: sa_motion_publish_stop(ctx,0x8c61833eu); r[7]=r[14];goto L8c618340; /* mov r14,r7 */
L8c618340: sa_motion_publish_stop(ctx,0x8c618340u); r[7]+=4294967295u;goto L8c618342; /* add #-0x1,r7 */
L8c618342: sa_motion_publish_stop(ctx,0x8c618342u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c618344; /* mov.l @r3,r2 */
L8c618344: sa_motion_publish_stop(ctx,0x8c618344u); *sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c618346; /* cmp/pl r7 */
L8c618346: sa_motion_publish_stop(ctx,0x8c618346u); r[2]+=r[14];goto L8c618348; /* add r14,r2 */
L8c618348: sa_motion_publish_stop(ctx,0x8c618348u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61834a; /* mov.l r2,@r3 */
L8c61834a: sa_motion_publish_stop(ctx,0x8c61834cu); take=!(*sr&1u);r[6]=r[12];if(take)goto L8c6183cc;goto L8c61834e; /* bf/s 0x8c6183cc */
L8c61834e: sa_motion_publish_stop(ctx,0x8c61834eu); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c618350; /* mov.w @r13+,r5 */
L8c618350: sa_motion_publish_stop(ctx,0x8c618350u); r[0]=4u;goto L8c618352; /* mov #0x4,r0 */
L8c618352: sa_motion_publish_stop(ctx,0x8c618352u); r[2]=rd(m,bus,&io_status,0x8c6184ccu,4);if(io_status)return io_status;goto L8c618354; /* mov.l 0x8c6184cc,r2 */
L8c618354: sa_motion_publish_stop(ctx,0x8c618354u); r[8]+=32u;goto L8c618356; /* add #0x20,r8 */
L8c618356: sa_motion_publish_stop(ctx,0x8c618356u); r[5]=r[5]&65535u;goto L8c618358; /* extu.w r5,r5 */
L8c618358: sa_motion_publish_stop(ctx,0x8c618358u); r[1]=rd(m,bus,&io_status,0x8c6184d0u,4);if(io_status)return io_status;goto L8c61835a; /* mov.l 0x8c6184d0,r1 */
L8c61835a: sa_motion_publish_stop(ctx,0x8c61835au); r[5]<<=2;goto L8c61835c; /* shll2 r5 */
L8c61835c: sa_motion_publish_stop(ctx,0x8c61835cu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61835e; /* mov.l @r2,r3 */
L8c61835e: sa_motion_publish_stop(ctx,0x8c61835eu); r[5]<<=2;goto L8c618360; /* shll2 r5 */
L8c618360: sa_motion_publish_stop(ctx,0x8c618360u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618362; /* fmov.s @r1,fr2 */
L8c618362: sa_motion_publish_stop(ctx,0x8c618362u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c618364; /* shll r5 */
L8c618364: sa_motion_publish_stop(ctx,0x8c618364u); r[2]=rd(m,bus,&io_status,0x8c6184d8u,4);if(io_status)return io_status;goto L8c618366; /* mov.l 0x8c6184d8,r2 */
L8c618366: sa_motion_publish_stop(ctx,0x8c618366u); r[5]+=r[3];goto L8c618368; /* add r3,r5 */
L8c618368: sa_motion_publish_stop(ctx,0x8c618368u); r[3]=rd(m,bus,&io_status,0x8c6184d4u,4);if(io_status)return io_status;goto L8c61836a; /* mov.l 0x8c6184d4,r3 */
L8c61836a: sa_motion_publish_stop(ctx,0x8c61836au); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61836c; /* fmov.s @r5,fr3 */
L8c61836c: sa_motion_publish_stop(ctx,0x8c61836cu); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61836e; /* fmov.s @r2,fr1 */
L8c61836e: sa_motion_publish_stop(ctx,0x8c61836eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618370; /* fmul fr3,fr2 */
L8c618370: sa_motion_publish_stop(ctx,0x8c618370u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618372; /* fmov.s @r3,fr3 */
L8c618372: sa_motion_publish_stop(ctx,0x8c618372u); r[3]=rd(m,bus,&io_status,0x8c6184dcu,4);if(io_status)return io_status;goto L8c618374; /* mov.l 0x8c6184dc,r3 */
L8c618374: sa_motion_publish_stop(ctx,0x8c618374u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618376; /* fsub fr2,fr3 */
L8c618376: sa_motion_publish_stop(ctx,0x8c618376u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c618378; /* fmov.s @(r0,r5),fr2 */
L8c618378: sa_motion_publish_stop(ctx,0x8c618378u); r[0]=8u;goto L8c61837a; /* mov #0x8,r0 */
L8c61837a: sa_motion_publish_stop(ctx,0x8c61837au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61837c; /* fmul fr2,fr1 */
L8c61837c: sa_motion_publish_stop(ctx,0x8c61837cu); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61837e; /* fmov.s @(r0,r5),fr2 */
L8c61837e: sa_motion_publish_stop(ctx,0x8c61837eu); r[0]=28u;goto L8c618380; /* mov #0x1c,r0 */
L8c618380: sa_motion_publish_stop(ctx,0x8c618380u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618382; /* fsub fr1,fr3 */
L8c618382: sa_motion_publish_stop(ctx,0x8c618382u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618384; /* fmov.s @r3,fr1 */
L8c618384: sa_motion_publish_stop(ctx,0x8c618384u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618386; /* fmul fr2,fr1 */
L8c618386: sa_motion_publish_stop(ctx,0x8c618386u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618388; /* fsub fr1,fr3 */
L8c618388: sa_motion_publish_stop(ctx,0x8c618388u); fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61838a; /* fmov fr3,@(r0,r5) */
L8c61838a: sa_motion_publish_stop(ctx,0x8c61838au); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61838c; /* fmov.s @(r0,r9),fr2 */
L8c61838c: sa_motion_publish_stop(ctx,0x8c61838cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61838e; /* fmul fr3,fr2 */
L8c61838e: sa_motion_publish_stop(ctx,0x8c61838eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618390; /* fmov.s fr2,@-r8 */
L8c618390: sa_motion_publish_stop(ctx,0x8c618390u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618392; /* fmov.s @(r0,r5),fr3 */
L8c618392: sa_motion_publish_stop(ctx,0x8c618392u); r[0]=24u;goto L8c618394; /* mov #0x18,r0 */
L8c618394: sa_motion_publish_stop(ctx,0x8c618394u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618396; /* fmov.s @(r0,r9),fr2 */
L8c618396: sa_motion_publish_stop(ctx,0x8c618396u); r[0]=28u;goto L8c618398; /* mov #0x1c,r0 */
L8c618398: sa_motion_publish_stop(ctx,0x8c618398u); r[2]=rd(m,bus,&io_status,0x8c6184e0u,4);if(io_status)return io_status;goto L8c61839a; /* mov.l 0x8c6184e0,r2 */
L8c61839a: sa_motion_publish_stop(ctx,0x8c61839au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61839c; /* fmul fr3,fr2 */
L8c61839c: sa_motion_publish_stop(ctx,0x8c61839cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61839e; /* fmov.s fr2,@-r8 */
L8c61839e: sa_motion_publish_stop(ctx,0x8c61839eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6183a0; /* fmov.s @(r0,r5),fr3 */
L8c6183a0: sa_motion_publish_stop(ctx,0x8c6183a0u); r[0]=20u;goto L8c6183a2; /* mov #0x14,r0 */
L8c6183a2: sa_motion_publish_stop(ctx,0x8c6183a2u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c6183a4; /* fmov.s @(r0,r9),fr2 */
L8c6183a4: sa_motion_publish_stop(ctx,0x8c6183a4u); r[0]=16u;goto L8c6183a6; /* mov #0x10,r0 */
L8c6183a6: sa_motion_publish_stop(ctx,0x8c6183a6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6183a8; /* fmul fr3,fr2 */
L8c6183a8: sa_motion_publish_stop(ctx,0x8c6183a8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6183aa; /* fmov.s fr2,@-r8 */
L8c6183aa: sa_motion_publish_stop(ctx,0x8c6183aau); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6183ac; /* fmov.s @(r0,r9),fr3 */
L8c6183ac: sa_motion_publish_stop(ctx,0x8c6183acu); r[0]=24u;goto L8c6183ae; /* mov #0x18,r0 */
L8c6183ae: sa_motion_publish_stop(ctx,0x8c6183aeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6183b0; /* fmov.s fr3,@-r8 */
L8c6183b0: sa_motion_publish_stop(ctx,0x8c6183b0u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6183b2; /* fmov.s @(r0,r5),fr3 */
L8c6183b2: sa_motion_publish_stop(ctx,0x8c6183b2u); r[0]=20u;goto L8c6183b4; /* mov #0x14,r0 */
L8c6183b4: sa_motion_publish_stop(ctx,0x8c6183b4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6183b6; /* fmov.s fr3,@-r8 */
L8c6183b6: sa_motion_publish_stop(ctx,0x8c6183b6u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6183b8; /* fmov.s @(r0,r5),fr3 */
L8c6183b8: sa_motion_publish_stop(ctx,0x8c6183b8u); r[0]=16u;goto L8c6183ba; /* mov #0x10,r0 */
L8c6183ba: sa_motion_publish_stop(ctx,0x8c6183bau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6183bc; /* fmov.s fr3,@-r8 */
L8c6183bc: sa_motion_publish_stop(ctx,0x8c6183bcu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6183be; /* fmov.s @(r0,r5),fr3 */
L8c6183be: sa_motion_publish_stop(ctx,0x8c6183beu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6183c0; /* fmov.s fr3,@-r8 */
L8c6183c0: sa_motion_publish_stop(ctx,0x8c6183c0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c6183c2; /* mov.l r2,@-r8 */
L8c6183c2: sa_motion_publish_stop(ctx,0x8c6183c2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6183c4; /* pref @r8 */
L8c6183c4: sa_motion_publish_stop(ctx,0x8c6183c4u); r[6]+=1u;goto L8c6183c6; /* add #0x1,r6 */
L8c6183c6: sa_motion_publish_stop(ctx,0x8c6183c6u); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c6183c8; /* cmp/ge r7,r6 */
L8c6183c8: sa_motion_publish_stop(ctx,0x8c6183cau); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61834e;goto L8c6183cc; /* bf/s 0x8c61834e */
L8c6183cc: sa_motion_publish_stop(ctx,0x8c6183ccu); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6183ce; /* mov.w @r13+,r5 */
L8c6183ce: sa_motion_publish_stop(ctx,0x8c6183ceu); r[0]=4u;goto L8c6183d0; /* mov #0x4,r0 */
L8c6183d0: sa_motion_publish_stop(ctx,0x8c6183d0u); r[2]=rd(m,bus,&io_status,0x8c6184ccu,4);if(io_status)return io_status;goto L8c6183d2; /* mov.l 0x8c6184cc,r2 */
L8c6183d2: sa_motion_publish_stop(ctx,0x8c6183d2u); r[8]+=32u;goto L8c6183d4; /* add #0x20,r8 */
L8c6183d4: sa_motion_publish_stop(ctx,0x8c6183d4u); r[5]=r[5]&65535u;goto L8c6183d6; /* extu.w r5,r5 */
L8c6183d6: sa_motion_publish_stop(ctx,0x8c6183d6u); r[1]=rd(m,bus,&io_status,0x8c6184d0u,4);if(io_status)return io_status;goto L8c6183d8; /* mov.l 0x8c6184d0,r1 */
L8c6183d8: sa_motion_publish_stop(ctx,0x8c6183d8u); r[5]<<=2;goto L8c6183da; /* shll2 r5 */
L8c6183da: sa_motion_publish_stop(ctx,0x8c6183dau); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6183dc; /* mov.l @r2,r3 */
L8c6183dc: sa_motion_publish_stop(ctx,0x8c6183dcu); r[5]<<=2;goto L8c6183de; /* shll2 r5 */
L8c6183de: sa_motion_publish_stop(ctx,0x8c6183deu); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c6183e0; /* fmov.s @r1,fr2 */
L8c6183e0: sa_motion_publish_stop(ctx,0x8c6183e0u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6183e2; /* shll r5 */
L8c6183e2: sa_motion_publish_stop(ctx,0x8c6183e2u); r[2]=rd(m,bus,&io_status,0x8c6184d8u,4);if(io_status)return io_status;goto L8c6183e4; /* mov.l 0x8c6184d8,r2 */
L8c6183e4: sa_motion_publish_stop(ctx,0x8c6183e4u); r[5]+=r[3];goto L8c6183e6; /* add r3,r5 */
L8c6183e6: sa_motion_publish_stop(ctx,0x8c6183e6u); r[3]=rd(m,bus,&io_status,0x8c6184d4u,4);if(io_status)return io_status;goto L8c6183e8; /* mov.l 0x8c6184d4,r3 */
L8c6183e8: sa_motion_publish_stop(ctx,0x8c6183e8u); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c6183ea; /* fmov.s @r5,fr3 */
L8c6183ea: sa_motion_publish_stop(ctx,0x8c6183eau); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c6183ec; /* fmov.s @r2,fr1 */
L8c6183ec: sa_motion_publish_stop(ctx,0x8c6183ecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6183ee; /* fmul fr3,fr2 */
L8c6183ee: sa_motion_publish_stop(ctx,0x8c6183eeu); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6183f0; /* fmov.s @r3,fr3 */
L8c6183f0: sa_motion_publish_stop(ctx,0x8c6183f0u); r[3]=rd(m,bus,&io_status,0x8c6184dcu,4);if(io_status)return io_status;goto L8c6183f2; /* mov.l 0x8c6184dc,r3 */
L8c6183f2: sa_motion_publish_stop(ctx,0x8c6183f2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6183f4; /* fsub fr2,fr3 */
L8c6183f4: sa_motion_publish_stop(ctx,0x8c6183f4u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c6183f6; /* fmov.s @(r0,r5),fr2 */
L8c6183f6: sa_motion_publish_stop(ctx,0x8c6183f6u); r[0]=8u;goto L8c6183f8; /* mov #0x8,r0 */
L8c6183f8: sa_motion_publish_stop(ctx,0x8c6183f8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6183fa; /* fmul fr2,fr1 */
L8c6183fa: sa_motion_publish_stop(ctx,0x8c6183fau); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c6183fc; /* fmov.s @(r0,r5),fr2 */
L8c6183fc: sa_motion_publish_stop(ctx,0x8c6183fcu); r[0]=28u;goto L8c6183fe; /* mov #0x1c,r0 */
L8c6183fe: sa_motion_publish_stop(ctx,0x8c6183feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618400; /* fsub fr1,fr3 */
L8c618400: sa_motion_publish_stop(ctx,0x8c618400u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618402; /* fmov.s @r3,fr1 */
L8c618402: sa_motion_publish_stop(ctx,0x8c618402u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618404; /* fmul fr2,fr1 */
L8c618404: sa_motion_publish_stop(ctx,0x8c618404u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618406; /* fsub fr1,fr3 */
L8c618406: sa_motion_publish_stop(ctx,0x8c618406u); fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618408; /* fmov fr3,@(r0,r5) */
L8c618408: sa_motion_publish_stop(ctx,0x8c618408u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61840a; /* fmov.s @(r0,r9),fr2 */
L8c61840a: sa_motion_publish_stop(ctx,0x8c61840au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61840c; /* fmul fr3,fr2 */
L8c61840c: sa_motion_publish_stop(ctx,0x8c61840cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61840e; /* fmov.s fr2,@-r8 */
L8c61840e: sa_motion_publish_stop(ctx,0x8c61840eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618410; /* fmov.s @(r0,r5),fr3 */
L8c618410: sa_motion_publish_stop(ctx,0x8c618410u); r[0]=24u;goto L8c618412; /* mov #0x18,r0 */
L8c618412: sa_motion_publish_stop(ctx,0x8c618412u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618414; /* fmov.s @(r0,r9),fr2 */
L8c618414: sa_motion_publish_stop(ctx,0x8c618414u); r[0]=28u;goto L8c618416; /* mov #0x1c,r0 */
L8c618416: sa_motion_publish_stop(ctx,0x8c618416u); r[2]=rd(m,bus,&io_status,0x8c6184e4u,4);if(io_status)return io_status;goto L8c618418; /* mov.l 0x8c6184e4,r2 */
L8c618418: sa_motion_publish_stop(ctx,0x8c618418u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61841a; /* fmul fr3,fr2 */
L8c61841a: sa_motion_publish_stop(ctx,0x8c61841au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61841c; /* fmov.s fr2,@-r8 */
L8c61841c: sa_motion_publish_stop(ctx,0x8c61841cu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61841e; /* fmov.s @(r0,r5),fr3 */
L8c61841e: sa_motion_publish_stop(ctx,0x8c61841eu); r[0]=20u;goto L8c618420; /* mov #0x14,r0 */
L8c618420: sa_motion_publish_stop(ctx,0x8c618420u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c618422; /* fmov.s @(r0,r9),fr2 */
L8c618422: sa_motion_publish_stop(ctx,0x8c618422u); r[0]=16u;goto L8c618424; /* mov #0x10,r0 */
L8c618424: sa_motion_publish_stop(ctx,0x8c618424u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618426; /* fmul fr3,fr2 */
L8c618426: sa_motion_publish_stop(ctx,0x8c618426u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618428; /* fmov.s fr2,@-r8 */
L8c618428: sa_motion_publish_stop(ctx,0x8c618428u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61842a; /* fmov.s @(r0,r9),fr3 */
L8c61842a: sa_motion_publish_stop(ctx,0x8c61842au); r[0]=24u;goto L8c61842c; /* mov #0x18,r0 */
L8c61842c: sa_motion_publish_stop(ctx,0x8c61842cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61842e; /* fmov.s fr3,@-r8 */
L8c61842e: sa_motion_publish_stop(ctx,0x8c61842eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618430; /* fmov.s @(r0,r5),fr3 */
L8c618430: sa_motion_publish_stop(ctx,0x8c618430u); r[0]=20u;goto L8c618432; /* mov #0x14,r0 */
L8c618432: sa_motion_publish_stop(ctx,0x8c618432u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618434; /* fmov.s fr3,@-r8 */
L8c618434: sa_motion_publish_stop(ctx,0x8c618434u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618436; /* fmov.s @(r0,r5),fr3 */
L8c618436: sa_motion_publish_stop(ctx,0x8c618436u); r[0]=16u;goto L8c618438; /* mov #0x10,r0 */
L8c618438: sa_motion_publish_stop(ctx,0x8c618438u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61843a; /* fmov.s fr3,@-r8 */
L8c61843a: sa_motion_publish_stop(ctx,0x8c61843au); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61843c; /* fmov.s @(r0,r5),fr3 */
L8c61843c: sa_motion_publish_stop(ctx,0x8c61843cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61843e; /* fmov.s fr3,@-r8 */
L8c61843e: sa_motion_publish_stop(ctx,0x8c61843eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c618440; /* mov.l r2,@-r8 */
L8c618440: sa_motion_publish_stop(ctx,0x8c618440u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618442; /* pref @r8 */
L8c618442: sa_motion_publish_stop(ctx,0x8c618442u); r[8]+=32u;goto L8c618444; /* add #0x20,r8 */
L8c618444: sa_motion_publish_stop(ctx,0x8c618444u); r[10]+=1u;goto L8c618446; /* add #0x1,r10 */
L8c618446: sa_motion_publish_stop(ctx,0x8c618446u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c618448; /* mov.w @(0x2,r4),r0 */
L8c618448: sa_motion_publish_stop(ctx,0x8c618448u); r[0]=r[0]&65535u;goto L8c61844a; /* extu.w r0,r0 */
L8c61844a: sa_motion_publish_stop(ctx,0x8c61844au); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c61844c; /* cmp/ge r0,r10 */
L8c61844c: sa_motion_publish_stop(ctx,0x8c61844cu); if((*sr&1u))goto L8c618452;goto L8c61844e; /* bt 0x8c618452 */
L8c61844e: sa_motion_publish_stop(ctx,0x8c618450u); goto L8c6182b2; /* bra 0x8c6182b2 */
L8c618452: sa_motion_publish_stop(ctx,0x8c618454u); goto L8c6184aa; /* bra 0x8c6184aa */
L8c618456: sa_motion_publish_stop(ctx,0x8c618456u); r[1]=rd(m,bus,&io_status,0x8c6184e8u,4);if(io_status)return io_status;goto L8c618458; /* mov.l 0x8c6184e8,r1 */
L8c618458: sa_motion_publish_stop(ctx,0x8c618458u); wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c61845a; /* mov.l r4,@r1 */
L8c61845a: sa_motion_publish_stop(ctx,0x8c61845au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61845c; /* frchg */
L8c61845c: sa_motion_publish_stop(ctx,0x8c61845cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61845e; /* fmov.s fr0,@-r15 */
L8c61845e: sa_motion_publish_stop(ctx,0x8c61845eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c618460; /* fmov.s fr1,@-r15 */
L8c618460: sa_motion_publish_stop(ctx,0x8c618460u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c618462; /* fmov.s fr2,@-r15 */
L8c618462: sa_motion_publish_stop(ctx,0x8c618462u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c618464; /* fmov.s fr3,@-r15 */
L8c618464: sa_motion_publish_stop(ctx,0x8c618464u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c618466; /* fmov.s fr4,@-r15 */
L8c618466: sa_motion_publish_stop(ctx,0x8c618466u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c618468; /* fmov.s fr5,@-r15 */
L8c618468: sa_motion_publish_stop(ctx,0x8c618468u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61846a; /* fmov.s fr6,@-r15 */
L8c61846a: sa_motion_publish_stop(ctx,0x8c61846au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61846c; /* fmov.s fr7,@-r15 */
L8c61846c: sa_motion_publish_stop(ctx,0x8c61846cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61846e; /* fmov.s fr8,@-r15 */
L8c61846e: sa_motion_publish_stop(ctx,0x8c61846eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c618470; /* fmov.s fr9,@-r15 */
L8c618470: sa_motion_publish_stop(ctx,0x8c618470u); r[0]=rd(m,bus,&io_status,0x8c6184bcu,4);if(io_status)return io_status;goto L8c618472; /* mov.l 0x8c6184bc,r0 */
L8c618472: sa_motion_publish_stop(ctx,0x8c618472u); fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618474; /* fmov.s @r0+,fr0 */
L8c618474: sa_motion_publish_stop(ctx,0x8c618474u); fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618476; /* fmov.s @r0+,fr1 */
L8c618476: sa_motion_publish_stop(ctx,0x8c618476u); fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c618478; /* fmov.s @r0,fr2 */
L8c618478: sa_motion_publish_stop(ctx,0x8c618478u); r[0]=rd(m,bus,&io_status,0x8c6184c0u,4);if(io_status)return io_status;goto L8c61847a; /* mov.l 0x8c6184c0,r0 */
L8c61847a: sa_motion_publish_stop(ctx,0x8c61847au); r[0]+=16u;goto L8c61847c; /* add #0x10,r0 */
L8c61847c: sa_motion_publish_stop(ctx,0x8c61847cu); fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61847e; /* fmov.s @r0+,fr4 */
L8c61847e: sa_motion_publish_stop(ctx,0x8c61847eu); fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618480; /* fmov.s @r0+,fr5 */
L8c618480: sa_motion_publish_stop(ctx,0x8c618480u); fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618482; /* fmov.s @r0+,fr6 */
L8c618482: sa_motion_publish_stop(ctx,0x8c618482u); fload(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;goto L8c618484; /* fmov.s @r0,fr7 */
L8c618484: sa_motion_publish_stop(ctx,0x8c618484u); r[0]=rd(m,bus,&io_status,0x8c6184c4u,4);if(io_status)return io_status;goto L8c618486; /* mov.l 0x8c6184c4,r0 */
L8c618486: sa_motion_publish_stop(ctx,0x8c618486u); fload(m,bus,&io_status,f,8u,r[0]);if(io_status)return io_status;goto L8c618488; /* fmov.s @r0,fr8 */
L8c618488: sa_motion_publish_stop(ctx,0x8c618488u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61848a; /* frchg */
L8c61848a: sa_motion_publish_stop(ctx,0x8c61848au); r[2]=rd(m,bus,&io_status,0x8c6184ecu,4);if(io_status)return io_status;goto L8c61848c; /* mov.l 0x8c6184ec,r2 */
L8c61848c: sa_motion_publish_stop(ctx,0x8c61848cu); r[3]=rd(m,bus,&io_status,0x8c6184e8u,4);if(io_status)return io_status;goto L8c61848e; /* mov.l 0x8c6184e8,r3 */
L8c61848e: sa_motion_publish_stop(ctx,0x8c618490u); target=r[2];*pr=0x8c618492u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c618492; /* jsr @r2 */
L8c618492: sa_motion_publish_stop(ctx,0x8c618492u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c618494; /* frchg */
L8c618494: sa_motion_publish_stop(ctx,0x8c618494u); fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618496; /* fmov.s @r15+,fr9 */
L8c618496: sa_motion_publish_stop(ctx,0x8c618496u); fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618498; /* fmov.s @r15+,fr8 */
L8c618498: sa_motion_publish_stop(ctx,0x8c618498u); fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61849a; /* fmov.s @r15+,fr7 */
L8c61849a: sa_motion_publish_stop(ctx,0x8c61849au); fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61849c; /* fmov.s @r15+,fr6 */
L8c61849c: sa_motion_publish_stop(ctx,0x8c61849cu); fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61849e; /* fmov.s @r15+,fr5 */
L8c61849e: sa_motion_publish_stop(ctx,0x8c61849eu); fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6184a0; /* fmov.s @r15+,fr4 */
L8c6184a0: sa_motion_publish_stop(ctx,0x8c6184a0u); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6184a2; /* fmov.s @r15+,fr3 */
L8c6184a2: sa_motion_publish_stop(ctx,0x8c6184a2u); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6184a4; /* fmov.s @r15+,fr2 */
L8c6184a4: sa_motion_publish_stop(ctx,0x8c6184a4u); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6184a6; /* fmov.s @r15+,fr1 */
L8c6184a6: sa_motion_publish_stop(ctx,0x8c6184a6u); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6184a8; /* fmov.s @r15+,fr0 */
L8c6184a8: sa_motion_publish_stop(ctx,0x8c6184a8u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6184aa; /* frchg */
L8c6184aa: sa_motion_publish_stop(ctx,0x8c6184aau); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6184ac; /* lds.l @r15+,PR */
L8c6184ac: sa_motion_publish_stop(ctx,0x8c6184acu); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6184ae; /* mov.l @r15+,r9 */
L8c6184ae: sa_motion_publish_stop(ctx,0x8c6184aeu); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6184b0; /* mov.l @r15+,r10 */
L8c6184b0: sa_motion_publish_stop(ctx,0x8c6184b0u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6184b2; /* mov.l @r15+,r11 */
L8c6184b2: sa_motion_publish_stop(ctx,0x8c6184b2u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6184b4; /* mov.l @r15+,r12 */
L8c6184b4: sa_motion_publish_stop(ctx,0x8c6184b4u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6184b6; /* mov.l @r15+,r13 */
L8c6184b6: sa_motion_publish_stop(ctx,0x8c6184b8u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_lit_primitive_clipped_8c6184f0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6184f0;
L8c6184f0: sa_motion_publish_stop(ctx,0x8c6184f0u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6184f2; /* mov.l r14,@-r15 */
L8c6184f2: sa_motion_publish_stop(ctx,0x8c6184f2u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6184f4; /* mov.l r13,@-r15 */
L8c6184f4: sa_motion_publish_stop(ctx,0x8c6184f4u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6184f6; /* mov.l r12,@-r15 */
L8c6184f6: sa_motion_publish_stop(ctx,0x8c6184f6u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6184f8; /* mov.l r11,@-r15 */
L8c6184f8: sa_motion_publish_stop(ctx,0x8c6184f8u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6184fa; /* mov.l r10,@-r15 */
L8c6184fa: sa_motion_publish_stop(ctx,0x8c6184fau); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6184fc; /* mov.l r9,@-r15 */
L8c6184fc: sa_motion_publish_stop(ctx,0x8c6184fcu); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6184fe; /* sts.l PR,@-r15 */
L8c6184fe: sa_motion_publish_stop(ctx,0x8c6184feu); r[15]+=4294967288u;goto L8c618500; /* add #-0x8,r15 */
L8c618500: sa_motion_publish_stop(ctx,0x8c618500u); r[0]=r[4];goto L8c618502; /* mov r4,r0 */
L8c618502: sa_motion_publish_stop(ctx,0x8c618502u); r[3]=rd(m,bus,&io_status,0x8c618554u,4);if(io_status)return io_status;goto L8c618504; /* mov.l 0x8c618554,r3 */
L8c618504: sa_motion_publish_stop(ctx,0x8c618504u); wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c618506; /* mov.l r4,@r15 */
L8c618506: sa_motion_publish_stop(ctx,0x8c618506u); r[11]=r[4];goto L8c618508; /* mov r4,r11 */
L8c618508: sa_motion_publish_stop(ctx,0x8c618508u); r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61850a; /* mov.w @r0,r0 */
L8c61850a: sa_motion_publish_stop(ctx,0x8c61850au); r[0]=r[0]&65535u;goto L8c61850c; /* extu.w r0,r0 */
L8c61850c: sa_motion_publish_stop(ctx,0x8c61850cu); r[0]&=r[3];goto L8c61850e; /* and r3,r0 */
L8c61850e: sa_motion_publish_stop(ctx,0x8c61850eu); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c618510; /* cmp/eq #0x0,r0 */
L8c618510: sa_motion_publish_stop(ctx,0x8c618512u); take=(*sr&1u);r[11]=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;if(take)goto L8c618536;goto L8c618514; /* bt/s 0x8c618536 */
L8c618514: sa_motion_publish_stop(ctx,0x8c618514u); r[1]=sx(m,bus,&io_status,0x8c618552u,2);if(io_status)return io_status;goto L8c618516; /* mov.w 0x8c618552,r1 */
L8c618516: sa_motion_publish_stop(ctx,0x8c618516u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c618518; /* cmp/eq r1,r0 */
L8c618518: sa_motion_publish_stop(ctx,0x8c618518u); if(!(*sr&1u))goto L8c61851e;goto L8c61851a; /* bf 0x8c61851e */
L8c61851a: sa_motion_publish_stop(ctx,0x8c61851cu); goto L8c61873c; /* bra 0x8c61873c */
L8c61851e: sa_motion_publish_stop(ctx,0x8c61851eu); r[1]=rd(m,bus,&io_status,0x8c618558u,4);if(io_status)return io_status;goto L8c618520; /* mov.l 0x8c618558,r1 */
L8c618520: sa_motion_publish_stop(ctx,0x8c618520u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c618522; /* cmp/eq r1,r0 */
L8c618522: sa_motion_publish_stop(ctx,0x8c618522u); if(!(*sr&1u))goto L8c618528;goto L8c618524; /* bf 0x8c618528 */
L8c618524: sa_motion_publish_stop(ctx,0x8c618526u); goto L8c618a06; /* bra 0x8c618a06 */
L8c618528: sa_motion_publish_stop(ctx,0x8c618528u); r[1]=rd(m,bus,&io_status,0x8c618554u,4);if(io_status)return io_status;goto L8c61852a; /* mov.l 0x8c618554,r1 */
L8c61852a: sa_motion_publish_stop(ctx,0x8c61852au); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61852c; /* cmp/eq r1,r0 */
L8c61852c: sa_motion_publish_stop(ctx,0x8c61852cu); if(!(*sr&1u))goto L8c618532;goto L8c61852e; /* bf 0x8c618532 */
L8c61852e: sa_motion_publish_stop(ctx,0x8c618530u); goto L8c618c5e; /* bra 0x8c618c5e */
L8c618532: sa_motion_publish_stop(ctx,0x8c618534u); goto L8c618cb4; /* bra 0x8c618cb4 */
L8c618536: sa_motion_publish_stop(ctx,0x8c618536u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c618538; /* mov.l @r15,r2 */
L8c618538: sa_motion_publish_stop(ctx,0x8c618538u); r[1]=rd(m,bus,&io_status,0x8c61855cu,4);if(io_status)return io_status;goto L8c61853a; /* mov.l 0x8c61855c,r1 */
L8c61853a: sa_motion_publish_stop(ctx,0x8c61853au); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61853c; /* mov.w @(0x2,r2),r0 */
L8c61853c: sa_motion_publish_stop(ctx,0x8c61853cu); r[0]=r[0]&65535u;goto L8c61853e; /* extu.w r0,r0 */
L8c61853e: sa_motion_publish_stop(ctx,0x8c61853eu); r[2]=r[0];goto L8c618540; /* mov r0,r2 */
L8c618540: sa_motion_publish_stop(ctx,0x8c618540u); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c618542; /* shll r0 */
L8c618542: sa_motion_publish_stop(ctx,0x8c618542u); r[0]+=r[2];goto L8c618544; /* add r2,r0 */
L8c618544: sa_motion_publish_stop(ctx,0x8c618544u); r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c618546; /* mov.l @r1,r2 */
L8c618546: sa_motion_publish_stop(ctx,0x8c618546u); r[2]+=r[0];goto L8c618548; /* add r0,r2 */
L8c618548: sa_motion_publish_stop(ctx,0x8c618548u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61854a; /* mov.l r2,@r1 */
L8c61854a: sa_motion_publish_stop(ctx,0x8c61854cu); r[14]=0u;goto L8c61872a; /* bra 0x8c61872a */
L8c618560: sa_motion_publish_stop(ctx,0x8c618560u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618562; /* mov.w @r11+,r4 */
L8c618562: sa_motion_publish_stop(ctx,0x8c618562u); r[3]=rd(m,bus,&io_status,0x8c618750u,4);if(io_status)return io_status;goto L8c618564; /* mov.l 0x8c618750,r3 */
L8c618564: sa_motion_publish_stop(ctx,0x8c618564u); r[5]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618566; /* mov.w @r11+,r5 */
L8c618566: sa_motion_publish_stop(ctx,0x8c618566u); r[4]=r[4]&65535u;goto L8c618568; /* extu.w r4,r4 */
L8c618568: sa_motion_publish_stop(ctx,0x8c618568u); r[4]<<=2;goto L8c61856a; /* shll2 r4 */
L8c61856a: sa_motion_publish_stop(ctx,0x8c61856au); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61856c; /* mov.l @r3,r13 */
L8c61856c: sa_motion_publish_stop(ctx,0x8c61856cu); r[6]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61856e; /* mov.w @r11+,r6 */
L8c61856e: sa_motion_publish_stop(ctx,0x8c61856eu); r[5]=r[5]&65535u;goto L8c618570; /* extu.w r5,r5 */
L8c618570: sa_motion_publish_stop(ctx,0x8c618570u); r[5]<<=2;goto L8c618572; /* shll2 r5 */
L8c618572: sa_motion_publish_stop(ctx,0x8c618572u); r[6]=r[6]&65535u;goto L8c618574; /* extu.w r6,r6 */
L8c618574: sa_motion_publish_stop(ctx,0x8c618574u); r[5]<<=2;goto L8c618576; /* shll2 r5 */
L8c618576: sa_motion_publish_stop(ctx,0x8c618576u); r[6]<<=2;goto L8c618578; /* shll2 r6 */
L8c618578: sa_motion_publish_stop(ctx,0x8c618578u); r[4]<<=2;goto L8c61857a; /* shll2 r4 */
L8c61857a: sa_motion_publish_stop(ctx,0x8c61857au); r[6]<<=2;goto L8c61857c; /* shll2 r6 */
L8c61857c: sa_motion_publish_stop(ctx,0x8c61857cu); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61857e; /* shll r4 */
L8c61857e: sa_motion_publish_stop(ctx,0x8c61857eu); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c618580; /* shll r5 */
L8c618580: sa_motion_publish_stop(ctx,0x8c618580u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c618582; /* shll r6 */
L8c618582: sa_motion_publish_stop(ctx,0x8c618582u); r[4]+=r[13];goto L8c618584; /* add r13,r4 */
L8c618584: sa_motion_publish_stop(ctx,0x8c618584u); r[5]+=r[13];goto L8c618586; /* add r13,r5 */
L8c618586: sa_motion_publish_stop(ctx,0x8c618586u); r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c618588; /* mov.l @(0xc,r4),r7 */
L8c618588: sa_motion_publish_stop(ctx,0x8c618588u); r[2]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c61858a; /* mov.l @(0xc,r5),r2 */
L8c61858a: sa_motion_publish_stop(ctx,0x8c61858au); r[6]+=r[13];goto L8c61858c; /* add r13,r6 */
L8c61858c: sa_motion_publish_stop(ctx,0x8c61858cu); r[1]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c61858e; /* mov.l @(0xc,r6),r1 */
L8c61858e: sa_motion_publish_stop(ctx,0x8c61858eu); r[7]&=r[2];goto L8c618590; /* and r2,r7 */
L8c618590: sa_motion_publish_stop(ctx,0x8c618590u); r[2]=63u;goto L8c618592; /* mov #0x3f,r2 */
L8c618592: sa_motion_publish_stop(ctx,0x8c618592u); r[7]&=r[1];goto L8c618594; /* and r1,r7 */
L8c618594: sa_motion_publish_stop(ctx,0x8c618594u); *sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c618596; /* tst r2,r7 */
L8c618596: sa_motion_publish_stop(ctx,0x8c618596u); if((*sr&1u))goto L8c61859c;goto L8c618598; /* bt 0x8c61859c */
L8c618598: sa_motion_publish_stop(ctx,0x8c61859au); goto L8c618728; /* bra 0x8c618728 */
L8c61859c: sa_motion_publish_stop(ctx,0x8c61859cu); r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61859e; /* mov.l @(0xc,r4),r7 */
L8c61859e: sa_motion_publish_stop(ctx,0x8c61859eu); r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c6185a0; /* mov.l @(0xc,r5),r3 */
L8c6185a0: sa_motion_publish_stop(ctx,0x8c6185a0u); r[2]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c6185a2; /* mov.l @(0xc,r6),r2 */
L8c6185a2: sa_motion_publish_stop(ctx,0x8c6185a2u); r[7]|=r[3];goto L8c6185a4; /* or r3,r7 */
L8c6185a4: sa_motion_publish_stop(ctx,0x8c6185a4u); r[3]=1u;goto L8c6185a6; /* mov #0x1,r3 */
L8c6185a6: sa_motion_publish_stop(ctx,0x8c6185a6u); r[7]|=r[2];goto L8c6185a8; /* or r2,r7 */
L8c6185a8: sa_motion_publish_stop(ctx,0x8c6185a8u); *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c6185aa; /* tst r3,r7 */
L8c6185aa: sa_motion_publish_stop(ctx,0x8c6185aau); if((*sr&1u))goto L8c6185b0;goto L8c6185ac; /* bt 0x8c6185b0 */
L8c6185ac: sa_motion_publish_stop(ctx,0x8c6185aeu); goto L8c61871e; /* bra 0x8c61871e */
L8c6185b0: sa_motion_publish_stop(ctx,0x8c6185b0u); r[0]=16u;goto L8c6185b2; /* mov #0x10,r0 */
L8c6185b2: sa_motion_publish_stop(ctx,0x8c6185b2u); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c6185b4; /* fmov.s @(r0,r5),fr5 */
L8c6185b4: sa_motion_publish_stop(ctx,0x8c6185b4u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6185b6; /* fmov.s @(r0,r4),fr3 */
L8c6185b6: sa_motion_publish_stop(ctx,0x8c6185b6u); r[0]=20u;goto L8c6185b8; /* mov #0x14,r0 */
L8c6185b8: sa_motion_publish_stop(ctx,0x8c6185b8u); fcopy(f,2u,5u);goto L8c6185ba; /* fmov fr5,fr2 */
L8c6185ba: sa_motion_publish_stop(ctx,0x8c6185bau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6185bc; /* fsub fr3,fr2 */
L8c6185bc: sa_motion_publish_stop(ctx,0x8c6185bcu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6185be; /* fmov.s @(r0,r6),fr3 */
L8c6185be: sa_motion_publish_stop(ctx,0x8c6185beu); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c6185c0; /* fmov.s @(r0,r5),fr4 */
L8c6185c0: sa_motion_publish_stop(ctx,0x8c6185c0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6185c2; /* fsub fr4,fr3 */
L8c6185c2: sa_motion_publish_stop(ctx,0x8c6185c2u); fcopy(f,1u,4u);goto L8c6185c4; /* fmov fr4,fr1 */
L8c6185c4: sa_motion_publish_stop(ctx,0x8c6185c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6185c6; /* fmul fr3,fr2 */
L8c6185c6: sa_motion_publish_stop(ctx,0x8c6185c6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6185c8; /* fmov.s @(r0,r4),fr3 */
L8c6185c8: sa_motion_publish_stop(ctx,0x8c6185c8u); r[0]=16u;goto L8c6185ca; /* mov #0x10,r0 */
L8c6185ca: sa_motion_publish_stop(ctx,0x8c6185cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6185cc; /* fsub fr3,fr1 */
L8c6185cc: sa_motion_publish_stop(ctx,0x8c6185ccu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6185ce; /* fmov.s @(r0,r6),fr3 */
L8c6185ce: sa_motion_publish_stop(ctx,0x8c6185ceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6185d0; /* fsub fr5,fr3 */
L8c6185d0: sa_motion_publish_stop(ctx,0x8c6185d0u); fcopy(f,4u,2u);goto L8c6185d2; /* fmov fr2,fr4 */
L8c6185d2: sa_motion_publish_stop(ctx,0x8c6185d2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6185d4; /* fmul fr3,fr1 */
L8c6185d4: sa_motion_publish_stop(ctx,0x8c6185d4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6185d6; /* fldi0 fr3 */
L8c6185d6: sa_motion_publish_stop(ctx,0x8c6185d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c6185d8; /* fsub fr1,fr4 */
L8c6185d8: sa_motion_publish_stop(ctx,0x8c6185d8u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c6185da; /* fcmp/gt fr3,fr4 */
L8c6185da: sa_motion_publish_stop(ctx,0x8c6185dau); if(!(*sr&1u))goto L8c6185e0;goto L8c6185dc; /* bf 0x8c6185e0 */
L8c6185dc: sa_motion_publish_stop(ctx,0x8c6185deu); goto L8c618728; /* bra 0x8c618728 */
L8c6185e0: sa_motion_publish_stop(ctx,0x8c6185e0u); r[3]=rd(m,bus,&io_status,0x8c618754u,4);if(io_status)return io_status;goto L8c6185e2; /* mov.l 0x8c618754,r3 */
L8c6185e2: sa_motion_publish_stop(ctx,0x8c6185e2u); r[0]=4u;goto L8c6185e4; /* mov #0x4,r0 */
L8c6185e4: sa_motion_publish_stop(ctx,0x8c6185e4u); fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c6185e6; /* fmov.s @r4,fr3 */
L8c6185e6: sa_motion_publish_stop(ctx,0x8c6185e6u); r[8]+=32u;goto L8c6185e8; /* add #0x20,r8 */
L8c6185e8: sa_motion_publish_stop(ctx,0x8c6185e8u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6185ea; /* fmov.s @r3,fr2 */
L8c6185ea: sa_motion_publish_stop(ctx,0x8c6185eau); r[3]+=8u;goto L8c6185ec; /* add #0x8,r3 */
L8c6185ec: sa_motion_publish_stop(ctx,0x8c6185ecu); r[2]=rd(m,bus,&io_status,0x8c618758u,4);if(io_status)return io_status;goto L8c6185ee; /* mov.l 0x8c618758,r2 */
L8c6185ee: sa_motion_publish_stop(ctx,0x8c6185eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6185f0; /* fmul fr3,fr2 */
L8c6185f0: sa_motion_publish_stop(ctx,0x8c6185f0u); r[1]=rd(m,bus,&io_status,0x8c61875cu,4);if(io_status)return io_status;goto L8c6185f2; /* mov.l 0x8c61875c,r1 */
L8c6185f2: sa_motion_publish_stop(ctx,0x8c6185f2u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6185f4; /* fmov.s @r2,fr3 */
L8c6185f4: sa_motion_publish_stop(ctx,0x8c6185f4u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c6185f6; /* fmov.s @r1,fr1 */
L8c6185f6: sa_motion_publish_stop(ctx,0x8c6185f6u); r[2]=rd(m,bus,&io_status,0x8c618760u,4);if(io_status)return io_status;goto L8c6185f8; /* mov.l 0x8c618760,r2 */
L8c6185f8: sa_motion_publish_stop(ctx,0x8c6185f8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6185fa; /* fsub fr2,fr3 */
L8c6185fa: sa_motion_publish_stop(ctx,0x8c6185fau); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c6185fc; /* fmov.s @(r0,r4),fr2 */
L8c6185fc: sa_motion_publish_stop(ctx,0x8c6185fcu); r[0]=8u;goto L8c6185fe; /* mov #0x8,r0 */
L8c6185fe: sa_motion_publish_stop(ctx,0x8c6185feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618600; /* fmul fr2,fr1 */
L8c618600: sa_motion_publish_stop(ctx,0x8c618600u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c618602; /* fmov.s @(r0,r4),fr2 */
L8c618602: sa_motion_publish_stop(ctx,0x8c618602u); r[0]=28u;goto L8c618604; /* mov #0x1c,r0 */
L8c618604: sa_motion_publish_stop(ctx,0x8c618604u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618606; /* fsub fr1,fr3 */
L8c618606: sa_motion_publish_stop(ctx,0x8c618606u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618608; /* fmov.s @r3,fr1 */
L8c618608: sa_motion_publish_stop(ctx,0x8c618608u); r[3]=rd(m,bus,&io_status,0x8c618764u,4);if(io_status)return io_status;goto L8c61860a; /* mov.l 0x8c618764,r3 */
L8c61860a: sa_motion_publish_stop(ctx,0x8c61860au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61860c; /* fmul fr2,fr1 */
L8c61860c: sa_motion_publish_stop(ctx,0x8c61860cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61860e; /* fsub fr1,fr3 */
L8c61860e: sa_motion_publish_stop(ctx,0x8c61860eu); fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618610; /* fmov fr3,@(r0,r4) */
L8c618610: sa_motion_publish_stop(ctx,0x8c618610u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618612; /* fmov.s @r2,fr2 */
L8c618612: sa_motion_publish_stop(ctx,0x8c618612u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618614; /* fmul fr3,fr2 */
L8c618614: sa_motion_publish_stop(ctx,0x8c618614u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618616; /* fmov.s fr2,@-r8 */
L8c618616: sa_motion_publish_stop(ctx,0x8c618616u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618618; /* fmov.s @r3,fr2 */
L8c618618: sa_motion_publish_stop(ctx,0x8c618618u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61861a; /* fmov.s @(r0,r4),fr3 */
L8c61861a: sa_motion_publish_stop(ctx,0x8c61861au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61861c; /* fmul fr3,fr2 */
L8c61861c: sa_motion_publish_stop(ctx,0x8c61861cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61861e; /* fmov.s fr2,@-r8 */
L8c61861e: sa_motion_publish_stop(ctx,0x8c61861eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618620; /* fmov.s @(r0,r4),fr3 */
L8c618620: sa_motion_publish_stop(ctx,0x8c618620u); r[1]=rd(m,bus,&io_status,0x8c618768u,4);if(io_status)return io_status;goto L8c618622; /* mov.l 0x8c618768,r1 */
L8c618622: sa_motion_publish_stop(ctx,0x8c618622u); r[2]+=4294967284u;goto L8c618624; /* add #-0xc,r2 */
L8c618624: sa_motion_publish_stop(ctx,0x8c618624u); r[0]=24u;goto L8c618626; /* mov #0x18,r0 */
L8c618626: sa_motion_publish_stop(ctx,0x8c618626u); r[3]=rd(m,bus,&io_status,0x8c61876cu,4);if(io_status)return io_status;goto L8c618628; /* mov.l 0x8c61876c,r3 */
L8c618628: sa_motion_publish_stop(ctx,0x8c618628u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61862a; /* fmov.s @r1,fr2 */
L8c61862a: sa_motion_publish_stop(ctx,0x8c61862au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61862c; /* fmul fr3,fr2 */
L8c61862c: sa_motion_publish_stop(ctx,0x8c61862cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61862e; /* fmov.s fr2,@-r8 */
L8c61862e: sa_motion_publish_stop(ctx,0x8c61862eu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618630; /* fmov.s @r2,fr3 */
L8c618630: sa_motion_publish_stop(ctx,0x8c618630u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618632; /* fmov.s fr3,@-r8 */
L8c618632: sa_motion_publish_stop(ctx,0x8c618632u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618634; /* fmov.s @(r0,r4),fr3 */
L8c618634: sa_motion_publish_stop(ctx,0x8c618634u); r[0]=20u;goto L8c618636; /* mov #0x14,r0 */
L8c618636: sa_motion_publish_stop(ctx,0x8c618636u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618638; /* fmov.s fr3,@-r8 */
L8c618638: sa_motion_publish_stop(ctx,0x8c618638u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61863a; /* fmov.s @(r0,r4),fr3 */
L8c61863a: sa_motion_publish_stop(ctx,0x8c61863au); r[0]=16u;goto L8c61863c; /* mov #0x10,r0 */
L8c61863c: sa_motion_publish_stop(ctx,0x8c61863cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61863e; /* fmov.s fr3,@-r8 */
L8c61863e: sa_motion_publish_stop(ctx,0x8c61863eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618640; /* fmov.s @(r0,r4),fr3 */
L8c618640: sa_motion_publish_stop(ctx,0x8c618640u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618642; /* fmov.s fr3,@-r8 */
L8c618642: sa_motion_publish_stop(ctx,0x8c618642u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c618644; /* mov.l r3,@-r8 */
L8c618644: sa_motion_publish_stop(ctx,0x8c618644u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618646; /* pref @r8 */
L8c618646: sa_motion_publish_stop(ctx,0x8c618646u); r[3]=rd(m,bus,&io_status,0x8c618754u,4);if(io_status)return io_status;goto L8c618648; /* mov.l 0x8c618754,r3 */
L8c618648: sa_motion_publish_stop(ctx,0x8c618648u); r[0]=4u;goto L8c61864a; /* mov #0x4,r0 */
L8c61864a: sa_motion_publish_stop(ctx,0x8c61864au); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61864c; /* fmov.s @r5,fr3 */
L8c61864c: sa_motion_publish_stop(ctx,0x8c61864cu); r[8]+=64u;goto L8c61864e; /* add #0x40,r8 */
L8c61864e: sa_motion_publish_stop(ctx,0x8c61864eu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618650; /* fmov.s @r3,fr2 */
L8c618650: sa_motion_publish_stop(ctx,0x8c618650u); r[3]+=8u;goto L8c618652; /* add #0x8,r3 */
L8c618652: sa_motion_publish_stop(ctx,0x8c618652u); r[1]=rd(m,bus,&io_status,0x8c618758u,4);if(io_status)return io_status;goto L8c618654; /* mov.l 0x8c618758,r1 */
L8c618654: sa_motion_publish_stop(ctx,0x8c618654u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618656; /* fmul fr3,fr2 */
L8c618656: sa_motion_publish_stop(ctx,0x8c618656u); r[2]=rd(m,bus,&io_status,0x8c61875cu,4);if(io_status)return io_status;goto L8c618658; /* mov.l 0x8c61875c,r2 */
L8c618658: sa_motion_publish_stop(ctx,0x8c618658u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61865a; /* fmov.s @r1,fr3 */
L8c61865a: sa_motion_publish_stop(ctx,0x8c61865au); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61865c; /* fmov.s @r2,fr1 */
L8c61865c: sa_motion_publish_stop(ctx,0x8c61865cu); r[2]=rd(m,bus,&io_status,0x8c618760u,4);if(io_status)return io_status;goto L8c61865e; /* mov.l 0x8c618760,r2 */
L8c61865e: sa_motion_publish_stop(ctx,0x8c61865eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618660; /* fsub fr2,fr3 */
L8c618660: sa_motion_publish_stop(ctx,0x8c618660u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c618662; /* fmov.s @(r0,r5),fr2 */
L8c618662: sa_motion_publish_stop(ctx,0x8c618662u); r[0]=8u;goto L8c618664; /* mov #0x8,r0 */
L8c618664: sa_motion_publish_stop(ctx,0x8c618664u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618666; /* fmul fr2,fr1 */
L8c618666: sa_motion_publish_stop(ctx,0x8c618666u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c618668; /* fmov.s @(r0,r5),fr2 */
L8c618668: sa_motion_publish_stop(ctx,0x8c618668u); r[0]=28u;goto L8c61866a; /* mov #0x1c,r0 */
L8c61866a: sa_motion_publish_stop(ctx,0x8c61866au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61866c; /* fsub fr1,fr3 */
L8c61866c: sa_motion_publish_stop(ctx,0x8c61866cu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61866e; /* fmov.s @r3,fr1 */
L8c61866e: sa_motion_publish_stop(ctx,0x8c61866eu); r[3]=rd(m,bus,&io_status,0x8c618764u,4);if(io_status)return io_status;goto L8c618670; /* mov.l 0x8c618764,r3 */
L8c618670: sa_motion_publish_stop(ctx,0x8c618670u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618672; /* fmul fr2,fr1 */
L8c618672: sa_motion_publish_stop(ctx,0x8c618672u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618674; /* fsub fr1,fr3 */
L8c618674: sa_motion_publish_stop(ctx,0x8c618674u); fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618676; /* fmov fr3,@(r0,r5) */
L8c618676: sa_motion_publish_stop(ctx,0x8c618676u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618678; /* fmov.s @r2,fr2 */
L8c618678: sa_motion_publish_stop(ctx,0x8c618678u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61867a; /* fmul fr3,fr2 */
L8c61867a: sa_motion_publish_stop(ctx,0x8c61867au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61867c; /* fmov.s fr2,@-r8 */
L8c61867c: sa_motion_publish_stop(ctx,0x8c61867cu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61867e; /* fmov.s @r3,fr2 */
L8c61867e: sa_motion_publish_stop(ctx,0x8c61867eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618680; /* fmov.s @(r0,r5),fr3 */
L8c618680: sa_motion_publish_stop(ctx,0x8c618680u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618682; /* fmul fr3,fr2 */
L8c618682: sa_motion_publish_stop(ctx,0x8c618682u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618684; /* fmov.s fr2,@-r8 */
L8c618684: sa_motion_publish_stop(ctx,0x8c618684u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c618686; /* fmov.s @(r0,r5),fr3 */
L8c618686: sa_motion_publish_stop(ctx,0x8c618686u); r[1]=rd(m,bus,&io_status,0x8c618768u,4);if(io_status)return io_status;goto L8c618688; /* mov.l 0x8c618768,r1 */
L8c618688: sa_motion_publish_stop(ctx,0x8c618688u); r[2]+=4294967284u;goto L8c61868a; /* add #-0xc,r2 */
L8c61868a: sa_motion_publish_stop(ctx,0x8c61868au); r[0]=24u;goto L8c61868c; /* mov #0x18,r0 */
L8c61868c: sa_motion_publish_stop(ctx,0x8c61868cu); r[3]=rd(m,bus,&io_status,0x8c61876cu,4);if(io_status)return io_status;goto L8c61868e; /* mov.l 0x8c61876c,r3 */
L8c61868e: sa_motion_publish_stop(ctx,0x8c61868eu); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618690; /* fmov.s @r1,fr2 */
L8c618690: sa_motion_publish_stop(ctx,0x8c618690u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618692; /* fmul fr3,fr2 */
L8c618692: sa_motion_publish_stop(ctx,0x8c618692u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618694; /* fmov.s fr2,@-r8 */
L8c618694: sa_motion_publish_stop(ctx,0x8c618694u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618696; /* fmov.s @r2,fr3 */
L8c618696: sa_motion_publish_stop(ctx,0x8c618696u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618698; /* fmov.s fr3,@-r8 */
L8c618698: sa_motion_publish_stop(ctx,0x8c618698u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61869a; /* fmov.s @(r0,r5),fr3 */
L8c61869a: sa_motion_publish_stop(ctx,0x8c61869au); r[0]=20u;goto L8c61869c; /* mov #0x14,r0 */
L8c61869c: sa_motion_publish_stop(ctx,0x8c61869cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61869e; /* fmov.s fr3,@-r8 */
L8c61869e: sa_motion_publish_stop(ctx,0x8c61869eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6186a0; /* fmov.s @(r0,r5),fr3 */
L8c6186a0: sa_motion_publish_stop(ctx,0x8c6186a0u); r[0]=16u;goto L8c6186a2; /* mov #0x10,r0 */
L8c6186a2: sa_motion_publish_stop(ctx,0x8c6186a2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6186a4; /* fmov.s fr3,@-r8 */
L8c6186a4: sa_motion_publish_stop(ctx,0x8c6186a4u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6186a6; /* fmov.s @(r0,r5),fr3 */
L8c6186a6: sa_motion_publish_stop(ctx,0x8c6186a6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6186a8; /* fmov.s fr3,@-r8 */
L8c6186a8: sa_motion_publish_stop(ctx,0x8c6186a8u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6186aa; /* mov.l r3,@-r8 */
L8c6186aa: sa_motion_publish_stop(ctx,0x8c6186aau); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6186ac; /* pref @r8 */
L8c6186ac: sa_motion_publish_stop(ctx,0x8c6186acu); r[3]=rd(m,bus,&io_status,0x8c618754u,4);if(io_status)return io_status;goto L8c6186ae; /* mov.l 0x8c618754,r3 */
L8c6186ae: sa_motion_publish_stop(ctx,0x8c6186aeu); r[0]=4u;goto L8c6186b0; /* mov #0x4,r0 */
L8c6186b0: sa_motion_publish_stop(ctx,0x8c6186b0u); fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c6186b2; /* fmov.s @r6,fr3 */
L8c6186b2: sa_motion_publish_stop(ctx,0x8c6186b2u); r[8]+=64u;goto L8c6186b4; /* add #0x40,r8 */
L8c6186b4: sa_motion_publish_stop(ctx,0x8c6186b4u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6186b6; /* fmov.s @r3,fr2 */
L8c6186b6: sa_motion_publish_stop(ctx,0x8c6186b6u); r[3]+=8u;goto L8c6186b8; /* add #0x8,r3 */
L8c6186b8: sa_motion_publish_stop(ctx,0x8c6186b8u); r[1]=rd(m,bus,&io_status,0x8c618758u,4);if(io_status)return io_status;goto L8c6186ba; /* mov.l 0x8c618758,r1 */
L8c6186ba: sa_motion_publish_stop(ctx,0x8c6186bau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6186bc; /* fmul fr3,fr2 */
L8c6186bc: sa_motion_publish_stop(ctx,0x8c6186bcu); r[2]=rd(m,bus,&io_status,0x8c61875cu,4);if(io_status)return io_status;goto L8c6186be; /* mov.l 0x8c61875c,r2 */
L8c6186be: sa_motion_publish_stop(ctx,0x8c6186beu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6186c0; /* fmov.s @r1,fr3 */
L8c6186c0: sa_motion_publish_stop(ctx,0x8c6186c0u); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c6186c2; /* fmov.s @r2,fr1 */
L8c6186c2: sa_motion_publish_stop(ctx,0x8c6186c2u); r[2]=rd(m,bus,&io_status,0x8c618760u,4);if(io_status)return io_status;goto L8c6186c4; /* mov.l 0x8c618760,r2 */
L8c6186c4: sa_motion_publish_stop(ctx,0x8c6186c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6186c6; /* fsub fr2,fr3 */
L8c6186c6: sa_motion_publish_stop(ctx,0x8c6186c6u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c6186c8; /* fmov.s @(r0,r6),fr2 */
L8c6186c8: sa_motion_publish_stop(ctx,0x8c6186c8u); r[0]=8u;goto L8c6186ca; /* mov #0x8,r0 */
L8c6186ca: sa_motion_publish_stop(ctx,0x8c6186cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6186cc; /* fmul fr2,fr1 */
L8c6186cc: sa_motion_publish_stop(ctx,0x8c6186ccu); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c6186ce; /* fmov.s @(r0,r6),fr2 */
L8c6186ce: sa_motion_publish_stop(ctx,0x8c6186ceu); r[0]=28u;goto L8c6186d0; /* mov #0x1c,r0 */
L8c6186d0: sa_motion_publish_stop(ctx,0x8c6186d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6186d2; /* fsub fr1,fr3 */
L8c6186d2: sa_motion_publish_stop(ctx,0x8c6186d2u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6186d4; /* fmov.s @r3,fr1 */
L8c6186d4: sa_motion_publish_stop(ctx,0x8c6186d4u); r[3]=rd(m,bus,&io_status,0x8c618764u,4);if(io_status)return io_status;goto L8c6186d6; /* mov.l 0x8c618764,r3 */
L8c6186d6: sa_motion_publish_stop(ctx,0x8c6186d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6186d8; /* fmul fr2,fr1 */
L8c6186d8: sa_motion_publish_stop(ctx,0x8c6186d8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6186da; /* fsub fr1,fr3 */
L8c6186da: sa_motion_publish_stop(ctx,0x8c6186dau); fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6186dc; /* fmov fr3,@(r0,r6) */
L8c6186dc: sa_motion_publish_stop(ctx,0x8c6186dcu); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c6186de; /* fmov.s @r2,fr2 */
L8c6186de: sa_motion_publish_stop(ctx,0x8c6186deu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6186e0; /* fmul fr3,fr2 */
L8c6186e0: sa_motion_publish_stop(ctx,0x8c6186e0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6186e2; /* fmov.s fr2,@-r8 */
L8c6186e2: sa_motion_publish_stop(ctx,0x8c6186e2u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6186e4; /* fmov.s @r3,fr2 */
L8c6186e4: sa_motion_publish_stop(ctx,0x8c6186e4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6186e6; /* fmov.s @(r0,r6),fr3 */
L8c6186e6: sa_motion_publish_stop(ctx,0x8c6186e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6186e8; /* fmul fr3,fr2 */
L8c6186e8: sa_motion_publish_stop(ctx,0x8c6186e8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6186ea; /* fmov.s fr2,@-r8 */
L8c6186ea: sa_motion_publish_stop(ctx,0x8c6186eau); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6186ec; /* fmov.s @(r0,r6),fr3 */
L8c6186ec: sa_motion_publish_stop(ctx,0x8c6186ecu); r[1]=rd(m,bus,&io_status,0x8c618768u,4);if(io_status)return io_status;goto L8c6186ee; /* mov.l 0x8c618768,r1 */
L8c6186ee: sa_motion_publish_stop(ctx,0x8c6186eeu); r[2]+=4294967284u;goto L8c6186f0; /* add #-0xc,r2 */
L8c6186f0: sa_motion_publish_stop(ctx,0x8c6186f0u); r[0]=24u;goto L8c6186f2; /* mov #0x18,r0 */
L8c6186f2: sa_motion_publish_stop(ctx,0x8c6186f2u); r[3]=rd(m,bus,&io_status,0x8c618770u,4);if(io_status)return io_status;goto L8c6186f4; /* mov.l 0x8c618770,r3 */
L8c6186f4: sa_motion_publish_stop(ctx,0x8c6186f4u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c6186f6; /* fmov.s @r1,fr2 */
L8c6186f6: sa_motion_publish_stop(ctx,0x8c6186f6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6186f8; /* fmul fr3,fr2 */
L8c6186f8: sa_motion_publish_stop(ctx,0x8c6186f8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6186fa; /* fmov.s fr2,@-r8 */
L8c6186fa: sa_motion_publish_stop(ctx,0x8c6186fau); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6186fc; /* fmov.s @r2,fr3 */
L8c6186fc: sa_motion_publish_stop(ctx,0x8c6186fcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6186fe; /* fmov.s fr3,@-r8 */
L8c6186fe: sa_motion_publish_stop(ctx,0x8c6186feu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618700; /* fmov.s @(r0,r6),fr3 */
L8c618700: sa_motion_publish_stop(ctx,0x8c618700u); r[0]=20u;goto L8c618702; /* mov #0x14,r0 */
L8c618702: sa_motion_publish_stop(ctx,0x8c618702u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618704; /* fmov.s fr3,@-r8 */
L8c618704: sa_motion_publish_stop(ctx,0x8c618704u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618706; /* fmov.s @(r0,r6),fr3 */
L8c618706: sa_motion_publish_stop(ctx,0x8c618706u); r[0]=16u;goto L8c618708; /* mov #0x10,r0 */
L8c618708: sa_motion_publish_stop(ctx,0x8c618708u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61870a; /* fmov.s fr3,@-r8 */
L8c61870a: sa_motion_publish_stop(ctx,0x8c61870au); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61870c; /* fmov.s @(r0,r6),fr3 */
L8c61870c: sa_motion_publish_stop(ctx,0x8c61870cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61870e; /* fmov.s fr3,@-r8 */
L8c61870e: sa_motion_publish_stop(ctx,0x8c61870eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c618710; /* mov.l r3,@-r8 */
L8c618710: sa_motion_publish_stop(ctx,0x8c618710u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618712; /* pref @r8 */
L8c618712: sa_motion_publish_stop(ctx,0x8c618712u); r[0]=rd(m,bus,&io_status,0x8c618774u,4);if(io_status)return io_status;goto L8c618714; /* mov.l 0x8c618774,r0 */
L8c618714: sa_motion_publish_stop(ctx,0x8c618714u); r[8]+=32u;goto L8c618716; /* add #0x20,r8 */
L8c618716: sa_motion_publish_stop(ctx,0x8c618716u); r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c618718; /* mov.l @r0,r3 */
L8c618718: sa_motion_publish_stop(ctx,0x8c618718u); r[3]+=3u;goto L8c61871a; /* add #0x3,r3 */
L8c61871a: sa_motion_publish_stop(ctx,0x8c61871cu); wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c618728; /* bra 0x8c618728 */
L8c61871e: sa_motion_publish_stop(ctx,0x8c61871eu); r[2]=rd(m,bus,&io_status,0x8c618778u,4);if(io_status)return io_status;goto L8c618720; /* mov.l 0x8c618778,r2 */
L8c618720: sa_motion_publish_stop(ctx,0x8c618720u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c618722; /* mov.l r3,@r2 */
L8c618722: sa_motion_publish_stop(ctx,0x8c618722u); r[3]=rd(m,bus,&io_status,0x8c61877cu,4);if(io_status)return io_status;goto L8c618724; /* mov.l 0x8c61877c,r3 */
L8c618724: sa_motion_publish_stop(ctx,0x8c618726u); target=r[3];*pr=0x8c618728u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c618728; /* jsr @r3 */
L8c618728: sa_motion_publish_stop(ctx,0x8c618728u); r[14]+=1u;goto L8c61872a; /* add #0x1,r14 */
L8c61872a: sa_motion_publish_stop(ctx,0x8c61872au); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61872c; /* mov.l @r15,r2 */
L8c61872c: sa_motion_publish_stop(ctx,0x8c61872cu); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61872e; /* mov.w @(0x2,r2),r0 */
L8c61872e: sa_motion_publish_stop(ctx,0x8c61872eu); r[0]=r[0]&65535u;goto L8c618730; /* extu.w r0,r0 */
L8c618730: sa_motion_publish_stop(ctx,0x8c618730u); *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c618732; /* cmp/ge r0,r14 */
L8c618732: sa_motion_publish_stop(ctx,0x8c618732u); if((*sr&1u))goto L8c618738;goto L8c618734; /* bt 0x8c618738 */
L8c618734: sa_motion_publish_stop(ctx,0x8c618736u); goto L8c618560; /* bra 0x8c618560 */
L8c618738: sa_motion_publish_stop(ctx,0x8c61873au); goto L8c618cb4; /* bra 0x8c618cb4 */
L8c61873c: sa_motion_publish_stop(ctx,0x8c61873cu); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61873e; /* mov.l @r15,r1 */
L8c61873e: sa_motion_publish_stop(ctx,0x8c61873eu); r[2]=rd(m,bus,&io_status,0x8c618780u,4);if(io_status)return io_status;goto L8c618740; /* mov.l 0x8c618780,r2 */
L8c618740: sa_motion_publish_stop(ctx,0x8c618740u); r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c618742; /* mov.w @(0x2,r1),r0 */
L8c618742: sa_motion_publish_stop(ctx,0x8c618742u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c618744; /* mov.l @r2,r1 */
L8c618744: sa_motion_publish_stop(ctx,0x8c618744u); r[0]=r[0]&65535u;goto L8c618746; /* extu.w r0,r0 */
L8c618746: sa_motion_publish_stop(ctx,0x8c618746u); r[0]<<=2;goto L8c618748; /* shll2 r0 */
L8c618748: sa_motion_publish_stop(ctx,0x8c618748u); r[1]+=r[0];goto L8c61874a; /* add r0,r1 */
L8c61874a: sa_motion_publish_stop(ctx,0x8c61874au); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61874c; /* mov.l r1,@r2 */
L8c61874c: sa_motion_publish_stop(ctx,0x8c61874eu); r[10]=0u;goto L8c6189f4; /* bra 0x8c6189f4 */
L8c618784: sa_motion_publish_stop(ctx,0x8c618784u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618786; /* mov.w @r11+,r4 */
L8c618786: sa_motion_publish_stop(ctx,0x8c618786u); r[3]=rd(m,bus,&io_status,0x8c618a10u,4);if(io_status)return io_status;goto L8c618788; /* mov.l 0x8c618a10,r3 */
L8c618788: sa_motion_publish_stop(ctx,0x8c618788u); r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61878a; /* mov.w @r11+,r13 */
L8c61878a: sa_motion_publish_stop(ctx,0x8c61878au); r[4]=r[4]&65535u;goto L8c61878c; /* extu.w r4,r4 */
L8c61878c: sa_motion_publish_stop(ctx,0x8c61878cu); r[4]<<=2;goto L8c61878e; /* shll2 r4 */
L8c61878e: sa_motion_publish_stop(ctx,0x8c61878eu); r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c618790; /* mov.l @r3,r6 */
L8c618790: sa_motion_publish_stop(ctx,0x8c618790u); r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618792; /* mov.w @r11+,r14 */
L8c618792: sa_motion_publish_stop(ctx,0x8c618792u); r[13]=r[13]&65535u;goto L8c618794; /* extu.w r13,r13 */
L8c618794: sa_motion_publish_stop(ctx,0x8c618794u); r[13]<<=2;goto L8c618796; /* shll2 r13 */
L8c618796: sa_motion_publish_stop(ctx,0x8c618796u); r[12]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618798; /* mov.w @r11+,r12 */
L8c618798: sa_motion_publish_stop(ctx,0x8c618798u); r[14]=r[14]&65535u;goto L8c61879a; /* extu.w r14,r14 */
L8c61879a: sa_motion_publish_stop(ctx,0x8c61879au); r[13]<<=2;goto L8c61879c; /* shll2 r13 */
L8c61879c: sa_motion_publish_stop(ctx,0x8c61879cu); r[4]<<=2;goto L8c61879e; /* shll2 r4 */
L8c61879e: sa_motion_publish_stop(ctx,0x8c61879eu); r[12]=r[12]&65535u;goto L8c6187a0; /* extu.w r12,r12 */
L8c6187a0: sa_motion_publish_stop(ctx,0x8c6187a0u); r[14]<<=2;goto L8c6187a2; /* shll2 r14 */
L8c6187a2: sa_motion_publish_stop(ctx,0x8c6187a2u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6187a4; /* shll r4 */
L8c6187a4: sa_motion_publish_stop(ctx,0x8c6187a4u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c6187a6; /* shll r13 */
L8c6187a6: sa_motion_publish_stop(ctx,0x8c6187a6u); r[4]+=r[6];goto L8c6187a8; /* add r6,r4 */
L8c6187a8: sa_motion_publish_stop(ctx,0x8c6187a8u); r[12]<<=2;goto L8c6187aa; /* shll2 r12 */
L8c6187aa: sa_motion_publish_stop(ctx,0x8c6187aau); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6187ac; /* mov.l @(0xc,r4),r5 */
L8c6187ac: sa_motion_publish_stop(ctx,0x8c6187acu); r[13]+=r[6];goto L8c6187ae; /* add r6,r13 */
L8c6187ae: sa_motion_publish_stop(ctx,0x8c6187aeu); r[14]<<=2;goto L8c6187b0; /* shll2 r14 */
L8c6187b0: sa_motion_publish_stop(ctx,0x8c6187b0u); r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c6187b2; /* mov.l @(0xc,r13),r2 */
L8c6187b2: sa_motion_publish_stop(ctx,0x8c6187b2u); r[12]<<=2;goto L8c6187b4; /* shll2 r12 */
L8c6187b4: sa_motion_publish_stop(ctx,0x8c6187b4u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c6187b6; /* shll r14 */
L8c6187b6: sa_motion_publish_stop(ctx,0x8c6187b6u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c6187b8; /* shll r12 */
L8c6187b8: sa_motion_publish_stop(ctx,0x8c6187b8u); r[14]+=r[6];goto L8c6187ba; /* add r6,r14 */
L8c6187ba: sa_motion_publish_stop(ctx,0x8c6187bau); r[1]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c6187bc; /* mov.l @(0xc,r14),r1 */
L8c6187bc: sa_motion_publish_stop(ctx,0x8c6187bcu); r[5]&=r[2];goto L8c6187be; /* and r2,r5 */
L8c6187be: sa_motion_publish_stop(ctx,0x8c6187beu); r[12]+=r[6];goto L8c6187c0; /* add r6,r12 */
L8c6187c0: sa_motion_publish_stop(ctx,0x8c6187c0u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c6187c2; /* mov.l @(0xc,r12),r2 */
L8c6187c2: sa_motion_publish_stop(ctx,0x8c6187c2u); r[5]&=r[1];goto L8c6187c4; /* and r1,r5 */
L8c6187c4: sa_motion_publish_stop(ctx,0x8c6187c4u); r[1]=63u;goto L8c6187c6; /* mov #0x3f,r1 */
L8c6187c6: sa_motion_publish_stop(ctx,0x8c6187c6u); r[5]&=r[2];goto L8c6187c8; /* and r2,r5 */
L8c6187c8: sa_motion_publish_stop(ctx,0x8c6187c8u); *sr=(*sr&~1u)|!(r[1]&r[5]);goto L8c6187ca; /* tst r1,r5 */
L8c6187ca: sa_motion_publish_stop(ctx,0x8c6187cau); if((*sr&1u))goto L8c6187d0;goto L8c6187cc; /* bt 0x8c6187d0 */
L8c6187cc: sa_motion_publish_stop(ctx,0x8c6187ceu); goto L8c6189f2; /* bra 0x8c6189f2 */
L8c6187d0: sa_motion_publish_stop(ctx,0x8c6187d0u); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6187d2; /* mov.l @(0xc,r4),r5 */
L8c6187d2: sa_motion_publish_stop(ctx,0x8c6187d2u); r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c6187d4; /* mov.l @(0xc,r13),r3 */
L8c6187d4: sa_motion_publish_stop(ctx,0x8c6187d4u); r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c6187d6; /* mov.l @(0xc,r14),r2 */
L8c6187d6: sa_motion_publish_stop(ctx,0x8c6187d6u); r[5]|=r[3];goto L8c6187d8; /* or r3,r5 */
L8c6187d8: sa_motion_publish_stop(ctx,0x8c6187d8u); r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c6187da; /* mov.l @(0xc,r12),r3 */
L8c6187da: sa_motion_publish_stop(ctx,0x8c6187dau); r[5]|=r[2];goto L8c6187dc; /* or r2,r5 */
L8c6187dc: sa_motion_publish_stop(ctx,0x8c6187dcu); r[2]=1u;goto L8c6187de; /* mov #0x1,r2 */
L8c6187de: sa_motion_publish_stop(ctx,0x8c6187deu); r[5]|=r[3];goto L8c6187e0; /* or r3,r5 */
L8c6187e0: sa_motion_publish_stop(ctx,0x8c6187e0u); *sr=(*sr&~1u)|!(r[2]&r[5]);goto L8c6187e2; /* tst r2,r5 */
L8c6187e2: sa_motion_publish_stop(ctx,0x8c6187e2u); if((*sr&1u))goto L8c6187e8;goto L8c6187e4; /* bt 0x8c6187e8 */
L8c6187e4: sa_motion_publish_stop(ctx,0x8c6187e6u); goto L8c6189bc; /* bra 0x8c6189bc */
L8c6187e8: sa_motion_publish_stop(ctx,0x8c6187e8u); r[0]=16u;goto L8c6187ea; /* mov #0x10,r0 */
L8c6187ea: sa_motion_publish_stop(ctx,0x8c6187eau); fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c6187ec; /* fmov.s @(r0,r13),fr4 */
L8c6187ec: sa_motion_publish_stop(ctx,0x8c6187ecu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6187ee; /* fmov.s @(r0,r4),fr3 */
L8c6187ee: sa_motion_publish_stop(ctx,0x8c6187eeu); r[0]=20u;goto L8c6187f0; /* mov #0x14,r0 */
L8c6187f0: sa_motion_publish_stop(ctx,0x8c6187f0u); fcopy(f,2u,4u);goto L8c6187f2; /* fmov fr4,fr2 */
L8c6187f2: sa_motion_publish_stop(ctx,0x8c6187f2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6187f4; /* fsub fr3,fr2 */
L8c6187f4: sa_motion_publish_stop(ctx,0x8c6187f4u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6187f6; /* fmov.s @(r0,r14),fr3 */
L8c6187f6: sa_motion_publish_stop(ctx,0x8c6187f6u); fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c6187f8; /* fmov.s @(r0,r13),fr5 */
L8c6187f8: sa_motion_publish_stop(ctx,0x8c6187f8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6187fa; /* fsub fr5,fr3 */
L8c6187fa: sa_motion_publish_stop(ctx,0x8c6187fau); fcopy(f,1u,5u);goto L8c6187fc; /* fmov fr5,fr1 */
L8c6187fc: sa_motion_publish_stop(ctx,0x8c6187fcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6187fe; /* fmul fr3,fr2 */
L8c6187fe: sa_motion_publish_stop(ctx,0x8c6187feu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618800; /* fmov.s @(r0,r4),fr3 */
L8c618800: sa_motion_publish_stop(ctx,0x8c618800u); r[0]=16u;goto L8c618802; /* mov #0x10,r0 */
L8c618802: sa_motion_publish_stop(ctx,0x8c618802u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c618804; /* fsub fr3,fr1 */
L8c618804: sa_motion_publish_stop(ctx,0x8c618804u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618806; /* fmov.s @(r0,r14),fr3 */
L8c618806: sa_motion_publish_stop(ctx,0x8c618806u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618808; /* fsub fr4,fr3 */
L8c618808: sa_motion_publish_stop(ctx,0x8c618808u); fcopy(f,4u,2u);goto L8c61880a; /* fmov fr2,fr4 */
L8c61880a: sa_motion_publish_stop(ctx,0x8c61880au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61880c; /* fmul fr3,fr1 */
L8c61880c: sa_motion_publish_stop(ctx,0x8c61880cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61880e; /* fldi0 fr3 */
L8c61880e: sa_motion_publish_stop(ctx,0x8c61880eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c618810; /* fsub fr1,fr4 */
L8c618810: sa_motion_publish_stop(ctx,0x8c618810u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c618812; /* fcmp/gt fr3,fr4 */
L8c618812: sa_motion_publish_stop(ctx,0x8c618812u); if(!(*sr&1u))goto L8c618818;goto L8c618814; /* bf 0x8c618818 */
L8c618814: sa_motion_publish_stop(ctx,0x8c618816u); goto L8c6189f2; /* bra 0x8c6189f2 */
L8c618818: sa_motion_publish_stop(ctx,0x8c618818u); r[3]=rd(m,bus,&io_status,0x8c618a14u,4);if(io_status)return io_status;goto L8c61881a; /* mov.l 0x8c618a14,r3 */
L8c61881a: sa_motion_publish_stop(ctx,0x8c61881au); r[0]=4u;goto L8c61881c; /* mov #0x4,r0 */
L8c61881c: sa_motion_publish_stop(ctx,0x8c61881cu); fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61881e; /* fmov.s @r4,fr3 */
L8c61881e: sa_motion_publish_stop(ctx,0x8c61881eu); r[8]+=32u;goto L8c618820; /* add #0x20,r8 */
L8c618820: sa_motion_publish_stop(ctx,0x8c618820u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618822; /* fmov.s @r3,fr2 */
L8c618822: sa_motion_publish_stop(ctx,0x8c618822u); r[3]+=8u;goto L8c618824; /* add #0x8,r3 */
L8c618824: sa_motion_publish_stop(ctx,0x8c618824u); r[2]=rd(m,bus,&io_status,0x8c618a18u,4);if(io_status)return io_status;goto L8c618826; /* mov.l 0x8c618a18,r2 */
L8c618826: sa_motion_publish_stop(ctx,0x8c618826u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618828; /* fmul fr3,fr2 */
L8c618828: sa_motion_publish_stop(ctx,0x8c618828u); r[1]=rd(m,bus,&io_status,0x8c618a1cu,4);if(io_status)return io_status;goto L8c61882a; /* mov.l 0x8c618a1c,r1 */
L8c61882a: sa_motion_publish_stop(ctx,0x8c61882au); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61882c; /* fmov.s @r2,fr3 */
L8c61882c: sa_motion_publish_stop(ctx,0x8c61882cu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61882e; /* fmov.s @r1,fr1 */
L8c61882e: sa_motion_publish_stop(ctx,0x8c61882eu); r[2]=rd(m,bus,&io_status,0x8c618a20u,4);if(io_status)return io_status;goto L8c618830; /* mov.l 0x8c618a20,r2 */
L8c618830: sa_motion_publish_stop(ctx,0x8c618830u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618832; /* fsub fr2,fr3 */
L8c618832: sa_motion_publish_stop(ctx,0x8c618832u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c618834; /* fmov.s @(r0,r4),fr2 */
L8c618834: sa_motion_publish_stop(ctx,0x8c618834u); r[0]=8u;goto L8c618836; /* mov #0x8,r0 */
L8c618836: sa_motion_publish_stop(ctx,0x8c618836u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618838; /* fmul fr2,fr1 */
L8c618838: sa_motion_publish_stop(ctx,0x8c618838u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61883a; /* fmov.s @(r0,r4),fr2 */
L8c61883a: sa_motion_publish_stop(ctx,0x8c61883au); r[0]=28u;goto L8c61883c; /* mov #0x1c,r0 */
L8c61883c: sa_motion_publish_stop(ctx,0x8c61883cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61883e; /* fsub fr1,fr3 */
L8c61883e: sa_motion_publish_stop(ctx,0x8c61883eu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618840; /* fmov.s @r3,fr1 */
L8c618840: sa_motion_publish_stop(ctx,0x8c618840u); r[3]=rd(m,bus,&io_status,0x8c618a24u,4);if(io_status)return io_status;goto L8c618842; /* mov.l 0x8c618a24,r3 */
L8c618842: sa_motion_publish_stop(ctx,0x8c618842u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618844; /* fmul fr2,fr1 */
L8c618844: sa_motion_publish_stop(ctx,0x8c618844u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618846; /* fsub fr1,fr3 */
L8c618846: sa_motion_publish_stop(ctx,0x8c618846u); fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618848; /* fmov fr3,@(r0,r4) */
L8c618848: sa_motion_publish_stop(ctx,0x8c618848u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61884a; /* fmov.s @r2,fr2 */
L8c61884a: sa_motion_publish_stop(ctx,0x8c61884au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61884c; /* fmul fr3,fr2 */
L8c61884c: sa_motion_publish_stop(ctx,0x8c61884cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61884e; /* fmov.s fr2,@-r8 */
L8c61884e: sa_motion_publish_stop(ctx,0x8c61884eu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618850; /* fmov.s @r3,fr2 */
L8c618850: sa_motion_publish_stop(ctx,0x8c618850u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618852; /* fmov.s @(r0,r4),fr3 */
L8c618852: sa_motion_publish_stop(ctx,0x8c618852u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618854; /* fmul fr3,fr2 */
L8c618854: sa_motion_publish_stop(ctx,0x8c618854u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618856; /* fmov.s fr2,@-r8 */
L8c618856: sa_motion_publish_stop(ctx,0x8c618856u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618858; /* fmov.s @(r0,r4),fr3 */
L8c618858: sa_motion_publish_stop(ctx,0x8c618858u); r[1]=rd(m,bus,&io_status,0x8c618a28u,4);if(io_status)return io_status;goto L8c61885a; /* mov.l 0x8c618a28,r1 */
L8c61885a: sa_motion_publish_stop(ctx,0x8c61885au); r[2]+=4294967284u;goto L8c61885c; /* add #-0xc,r2 */
L8c61885c: sa_motion_publish_stop(ctx,0x8c61885cu); r[0]=24u;goto L8c61885e; /* mov #0x18,r0 */
L8c61885e: sa_motion_publish_stop(ctx,0x8c61885eu); r[3]=rd(m,bus,&io_status,0x8c618a2cu,4);if(io_status)return io_status;goto L8c618860; /* mov.l 0x8c618a2c,r3 */
L8c618860: sa_motion_publish_stop(ctx,0x8c618860u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618862; /* fmov.s @r1,fr2 */
L8c618862: sa_motion_publish_stop(ctx,0x8c618862u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618864; /* fmul fr3,fr2 */
L8c618864: sa_motion_publish_stop(ctx,0x8c618864u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618866; /* fmov.s fr2,@-r8 */
L8c618866: sa_motion_publish_stop(ctx,0x8c618866u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618868; /* fmov.s @r2,fr3 */
L8c618868: sa_motion_publish_stop(ctx,0x8c618868u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61886a; /* fmov.s fr3,@-r8 */
L8c61886a: sa_motion_publish_stop(ctx,0x8c61886au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61886c; /* fmov.s @(r0,r4),fr3 */
L8c61886c: sa_motion_publish_stop(ctx,0x8c61886cu); r[0]=20u;goto L8c61886e; /* mov #0x14,r0 */
L8c61886e: sa_motion_publish_stop(ctx,0x8c61886eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618870; /* fmov.s fr3,@-r8 */
L8c618870: sa_motion_publish_stop(ctx,0x8c618870u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618872; /* fmov.s @(r0,r4),fr3 */
L8c618872: sa_motion_publish_stop(ctx,0x8c618872u); r[0]=16u;goto L8c618874; /* mov #0x10,r0 */
L8c618874: sa_motion_publish_stop(ctx,0x8c618874u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618876; /* fmov.s fr3,@-r8 */
L8c618876: sa_motion_publish_stop(ctx,0x8c618876u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618878; /* fmov.s @(r0,r4),fr3 */
L8c618878: sa_motion_publish_stop(ctx,0x8c618878u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61887a; /* fmov.s fr3,@-r8 */
L8c61887a: sa_motion_publish_stop(ctx,0x8c61887au); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61887c; /* mov.l r3,@-r8 */
L8c61887c: sa_motion_publish_stop(ctx,0x8c61887cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61887e; /* pref @r8 */
L8c61887e: sa_motion_publish_stop(ctx,0x8c61887eu); r[3]=rd(m,bus,&io_status,0x8c618a14u,4);if(io_status)return io_status;goto L8c618880; /* mov.l 0x8c618a14,r3 */
L8c618880: sa_motion_publish_stop(ctx,0x8c618880u); r[0]=4u;goto L8c618882; /* mov #0x4,r0 */
L8c618882: sa_motion_publish_stop(ctx,0x8c618882u); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c618884; /* fmov.s @r13,fr3 */
L8c618884: sa_motion_publish_stop(ctx,0x8c618884u); r[8]+=64u;goto L8c618886; /* add #0x40,r8 */
L8c618886: sa_motion_publish_stop(ctx,0x8c618886u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618888; /* fmov.s @r3,fr2 */
L8c618888: sa_motion_publish_stop(ctx,0x8c618888u); r[3]+=8u;goto L8c61888a; /* add #0x8,r3 */
L8c61888a: sa_motion_publish_stop(ctx,0x8c61888au); r[1]=rd(m,bus,&io_status,0x8c618a18u,4);if(io_status)return io_status;goto L8c61888c; /* mov.l 0x8c618a18,r1 */
L8c61888c: sa_motion_publish_stop(ctx,0x8c61888cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61888e; /* fmul fr3,fr2 */
L8c61888e: sa_motion_publish_stop(ctx,0x8c61888eu); r[2]=rd(m,bus,&io_status,0x8c618a1cu,4);if(io_status)return io_status;goto L8c618890; /* mov.l 0x8c618a1c,r2 */
L8c618890: sa_motion_publish_stop(ctx,0x8c618890u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c618892; /* fmov.s @r1,fr3 */
L8c618892: sa_motion_publish_stop(ctx,0x8c618892u); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c618894; /* fmov.s @r2,fr1 */
L8c618894: sa_motion_publish_stop(ctx,0x8c618894u); r[2]=rd(m,bus,&io_status,0x8c618a20u,4);if(io_status)return io_status;goto L8c618896; /* mov.l 0x8c618a20,r2 */
L8c618896: sa_motion_publish_stop(ctx,0x8c618896u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618898; /* fsub fr2,fr3 */
L8c618898: sa_motion_publish_stop(ctx,0x8c618898u); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61889a; /* fmov.s @(r0,r13),fr2 */
L8c61889a: sa_motion_publish_stop(ctx,0x8c61889au); r[0]=8u;goto L8c61889c; /* mov #0x8,r0 */
L8c61889c: sa_motion_publish_stop(ctx,0x8c61889cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61889e; /* fmul fr2,fr1 */
L8c61889e: sa_motion_publish_stop(ctx,0x8c61889eu); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c6188a0; /* fmov.s @(r0,r13),fr2 */
L8c6188a0: sa_motion_publish_stop(ctx,0x8c6188a0u); r[0]=28u;goto L8c6188a2; /* mov #0x1c,r0 */
L8c6188a2: sa_motion_publish_stop(ctx,0x8c6188a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6188a4; /* fsub fr1,fr3 */
L8c6188a4: sa_motion_publish_stop(ctx,0x8c6188a4u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c6188a6; /* fmov.s @r3,fr1 */
L8c6188a6: sa_motion_publish_stop(ctx,0x8c6188a6u); r[3]=rd(m,bus,&io_status,0x8c618a24u,4);if(io_status)return io_status;goto L8c6188a8; /* mov.l 0x8c618a24,r3 */
L8c6188a8: sa_motion_publish_stop(ctx,0x8c6188a8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6188aa; /* fmul fr2,fr1 */
L8c6188aa: sa_motion_publish_stop(ctx,0x8c6188aau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6188ac; /* fsub fr1,fr3 */
L8c6188ac: sa_motion_publish_stop(ctx,0x8c6188acu); fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6188ae; /* fmov fr3,@(r0,r13) */
L8c6188ae: sa_motion_publish_stop(ctx,0x8c6188aeu); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c6188b0; /* fmov.s @r2,fr2 */
L8c6188b0: sa_motion_publish_stop(ctx,0x8c6188b0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6188b2; /* fmul fr3,fr2 */
L8c6188b2: sa_motion_publish_stop(ctx,0x8c6188b2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6188b4; /* fmov.s fr2,@-r8 */
L8c6188b4: sa_motion_publish_stop(ctx,0x8c6188b4u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6188b6; /* fmov.s @r3,fr2 */
L8c6188b6: sa_motion_publish_stop(ctx,0x8c6188b6u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6188b8; /* fmov.s @(r0,r13),fr3 */
L8c6188b8: sa_motion_publish_stop(ctx,0x8c6188b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6188ba; /* fmul fr3,fr2 */
L8c6188ba: sa_motion_publish_stop(ctx,0x8c6188bau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6188bc; /* fmov.s fr2,@-r8 */
L8c6188bc: sa_motion_publish_stop(ctx,0x8c6188bcu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6188be; /* fmov.s @(r0,r13),fr3 */
L8c6188be: sa_motion_publish_stop(ctx,0x8c6188beu); r[1]=rd(m,bus,&io_status,0x8c618a28u,4);if(io_status)return io_status;goto L8c6188c0; /* mov.l 0x8c618a28,r1 */
L8c6188c0: sa_motion_publish_stop(ctx,0x8c6188c0u); r[2]+=4294967284u;goto L8c6188c2; /* add #-0xc,r2 */
L8c6188c2: sa_motion_publish_stop(ctx,0x8c6188c2u); r[0]=24u;goto L8c6188c4; /* mov #0x18,r0 */
L8c6188c4: sa_motion_publish_stop(ctx,0x8c6188c4u); r[3]=rd(m,bus,&io_status,0x8c618a2cu,4);if(io_status)return io_status;goto L8c6188c6; /* mov.l 0x8c618a2c,r3 */
L8c6188c6: sa_motion_publish_stop(ctx,0x8c6188c6u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c6188c8; /* fmov.s @r1,fr2 */
L8c6188c8: sa_motion_publish_stop(ctx,0x8c6188c8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6188ca; /* fmul fr3,fr2 */
L8c6188ca: sa_motion_publish_stop(ctx,0x8c6188cau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6188cc; /* fmov.s fr2,@-r8 */
L8c6188cc: sa_motion_publish_stop(ctx,0x8c6188ccu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6188ce; /* fmov.s @r2,fr3 */
L8c6188ce: sa_motion_publish_stop(ctx,0x8c6188ceu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6188d0; /* fmov.s fr3,@-r8 */
L8c6188d0: sa_motion_publish_stop(ctx,0x8c6188d0u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6188d2; /* fmov.s @(r0,r13),fr3 */
L8c6188d2: sa_motion_publish_stop(ctx,0x8c6188d2u); r[0]=20u;goto L8c6188d4; /* mov #0x14,r0 */
L8c6188d4: sa_motion_publish_stop(ctx,0x8c6188d4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6188d6; /* fmov.s fr3,@-r8 */
L8c6188d6: sa_motion_publish_stop(ctx,0x8c6188d6u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6188d8; /* fmov.s @(r0,r13),fr3 */
L8c6188d8: sa_motion_publish_stop(ctx,0x8c6188d8u); r[0]=16u;goto L8c6188da; /* mov #0x10,r0 */
L8c6188da: sa_motion_publish_stop(ctx,0x8c6188dau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6188dc; /* fmov.s fr3,@-r8 */
L8c6188dc: sa_motion_publish_stop(ctx,0x8c6188dcu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6188de; /* fmov.s @(r0,r13),fr3 */
L8c6188de: sa_motion_publish_stop(ctx,0x8c6188deu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6188e0; /* fmov.s fr3,@-r8 */
L8c6188e0: sa_motion_publish_stop(ctx,0x8c6188e0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6188e2; /* mov.l r3,@-r8 */
L8c6188e2: sa_motion_publish_stop(ctx,0x8c6188e2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6188e4; /* pref @r8 */
L8c6188e4: sa_motion_publish_stop(ctx,0x8c6188e4u); r[3]=rd(m,bus,&io_status,0x8c618a14u,4);if(io_status)return io_status;goto L8c6188e6; /* mov.l 0x8c618a14,r3 */
L8c6188e6: sa_motion_publish_stop(ctx,0x8c6188e6u); r[0]=4u;goto L8c6188e8; /* mov #0x4,r0 */
L8c6188e8: sa_motion_publish_stop(ctx,0x8c6188e8u); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c6188ea; /* fmov.s @r14,fr3 */
L8c6188ea: sa_motion_publish_stop(ctx,0x8c6188eau); r[8]+=64u;goto L8c6188ec; /* add #0x40,r8 */
L8c6188ec: sa_motion_publish_stop(ctx,0x8c6188ecu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6188ee; /* fmov.s @r3,fr2 */
L8c6188ee: sa_motion_publish_stop(ctx,0x8c6188eeu); r[3]+=8u;goto L8c6188f0; /* add #0x8,r3 */
L8c6188f0: sa_motion_publish_stop(ctx,0x8c6188f0u); r[1]=rd(m,bus,&io_status,0x8c618a18u,4);if(io_status)return io_status;goto L8c6188f2; /* mov.l 0x8c618a18,r1 */
L8c6188f2: sa_motion_publish_stop(ctx,0x8c6188f2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6188f4; /* fmul fr3,fr2 */
L8c6188f4: sa_motion_publish_stop(ctx,0x8c6188f4u); r[2]=rd(m,bus,&io_status,0x8c618a1cu,4);if(io_status)return io_status;goto L8c6188f6; /* mov.l 0x8c618a1c,r2 */
L8c6188f6: sa_motion_publish_stop(ctx,0x8c6188f6u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6188f8; /* fmov.s @r1,fr3 */
L8c6188f8: sa_motion_publish_stop(ctx,0x8c6188f8u); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c6188fa; /* fmov.s @r2,fr1 */
L8c6188fa: sa_motion_publish_stop(ctx,0x8c6188fau); r[2]=rd(m,bus,&io_status,0x8c618a20u,4);if(io_status)return io_status;goto L8c6188fc; /* mov.l 0x8c618a20,r2 */
L8c6188fc: sa_motion_publish_stop(ctx,0x8c6188fcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6188fe; /* fsub fr2,fr3 */
L8c6188fe: sa_motion_publish_stop(ctx,0x8c6188feu); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c618900; /* fmov.s @(r0,r14),fr2 */
L8c618900: sa_motion_publish_stop(ctx,0x8c618900u); r[0]=8u;goto L8c618902; /* mov #0x8,r0 */
L8c618902: sa_motion_publish_stop(ctx,0x8c618902u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618904; /* fmul fr2,fr1 */
L8c618904: sa_motion_publish_stop(ctx,0x8c618904u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c618906; /* fmov.s @(r0,r14),fr2 */
L8c618906: sa_motion_publish_stop(ctx,0x8c618906u); r[0]=28u;goto L8c618908; /* mov #0x1c,r0 */
L8c618908: sa_motion_publish_stop(ctx,0x8c618908u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61890a; /* fsub fr1,fr3 */
L8c61890a: sa_motion_publish_stop(ctx,0x8c61890au); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61890c; /* fmov.s @r3,fr1 */
L8c61890c: sa_motion_publish_stop(ctx,0x8c61890cu); r[3]=rd(m,bus,&io_status,0x8c618a24u,4);if(io_status)return io_status;goto L8c61890e; /* mov.l 0x8c618a24,r3 */
L8c61890e: sa_motion_publish_stop(ctx,0x8c61890eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618910; /* fmul fr2,fr1 */
L8c618910: sa_motion_publish_stop(ctx,0x8c618910u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618912; /* fsub fr1,fr3 */
L8c618912: sa_motion_publish_stop(ctx,0x8c618912u); fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618914; /* fmov fr3,@(r0,r14) */
L8c618914: sa_motion_publish_stop(ctx,0x8c618914u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618916; /* fmov.s @r2,fr2 */
L8c618916: sa_motion_publish_stop(ctx,0x8c618916u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618918; /* fmul fr3,fr2 */
L8c618918: sa_motion_publish_stop(ctx,0x8c618918u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61891a; /* fmov.s fr2,@-r8 */
L8c61891a: sa_motion_publish_stop(ctx,0x8c61891au); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61891c; /* fmov.s @r3,fr2 */
L8c61891c: sa_motion_publish_stop(ctx,0x8c61891cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61891e; /* fmov.s @(r0,r14),fr3 */
L8c61891e: sa_motion_publish_stop(ctx,0x8c61891eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618920; /* fmul fr3,fr2 */
L8c618920: sa_motion_publish_stop(ctx,0x8c618920u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618922; /* fmov.s fr2,@-r8 */
L8c618922: sa_motion_publish_stop(ctx,0x8c618922u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618924; /* fmov.s @(r0,r14),fr3 */
L8c618924: sa_motion_publish_stop(ctx,0x8c618924u); r[1]=rd(m,bus,&io_status,0x8c618a28u,4);if(io_status)return io_status;goto L8c618926; /* mov.l 0x8c618a28,r1 */
L8c618926: sa_motion_publish_stop(ctx,0x8c618926u); r[2]+=4294967284u;goto L8c618928; /* add #-0xc,r2 */
L8c618928: sa_motion_publish_stop(ctx,0x8c618928u); r[0]=24u;goto L8c61892a; /* mov #0x18,r0 */
L8c61892a: sa_motion_publish_stop(ctx,0x8c61892au); r[3]=rd(m,bus,&io_status,0x8c618a2cu,4);if(io_status)return io_status;goto L8c61892c; /* mov.l 0x8c618a2c,r3 */
L8c61892c: sa_motion_publish_stop(ctx,0x8c61892cu); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61892e; /* fmov.s @r1,fr2 */
L8c61892e: sa_motion_publish_stop(ctx,0x8c61892eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618930; /* fmul fr3,fr2 */
L8c618930: sa_motion_publish_stop(ctx,0x8c618930u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618932; /* fmov.s fr2,@-r8 */
L8c618932: sa_motion_publish_stop(ctx,0x8c618932u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618934; /* fmov.s @r2,fr3 */
L8c618934: sa_motion_publish_stop(ctx,0x8c618934u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618936; /* fmov.s fr3,@-r8 */
L8c618936: sa_motion_publish_stop(ctx,0x8c618936u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618938; /* fmov.s @(r0,r14),fr3 */
L8c618938: sa_motion_publish_stop(ctx,0x8c618938u); r[0]=20u;goto L8c61893a; /* mov #0x14,r0 */
L8c61893a: sa_motion_publish_stop(ctx,0x8c61893au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61893c; /* fmov.s fr3,@-r8 */
L8c61893c: sa_motion_publish_stop(ctx,0x8c61893cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61893e; /* fmov.s @(r0,r14),fr3 */
L8c61893e: sa_motion_publish_stop(ctx,0x8c61893eu); r[0]=16u;goto L8c618940; /* mov #0x10,r0 */
L8c618940: sa_motion_publish_stop(ctx,0x8c618940u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618942; /* fmov.s fr3,@-r8 */
L8c618942: sa_motion_publish_stop(ctx,0x8c618942u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c618944; /* fmov.s @(r0,r14),fr3 */
L8c618944: sa_motion_publish_stop(ctx,0x8c618944u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618946; /* fmov.s fr3,@-r8 */
L8c618946: sa_motion_publish_stop(ctx,0x8c618946u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c618948; /* mov.l r3,@-r8 */
L8c618948: sa_motion_publish_stop(ctx,0x8c618948u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61894a; /* pref @r8 */
L8c61894a: sa_motion_publish_stop(ctx,0x8c61894au); r[3]=rd(m,bus,&io_status,0x8c618a14u,4);if(io_status)return io_status;goto L8c61894c; /* mov.l 0x8c618a14,r3 */
L8c61894c: sa_motion_publish_stop(ctx,0x8c61894cu); r[0]=4u;goto L8c61894e; /* mov #0x4,r0 */
L8c61894e: sa_motion_publish_stop(ctx,0x8c61894eu); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c618950; /* fmov.s @r12,fr3 */
L8c618950: sa_motion_publish_stop(ctx,0x8c618950u); r[8]+=64u;goto L8c618952; /* add #0x40,r8 */
L8c618952: sa_motion_publish_stop(ctx,0x8c618952u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618954; /* fmov.s @r3,fr2 */
L8c618954: sa_motion_publish_stop(ctx,0x8c618954u); r[3]+=8u;goto L8c618956; /* add #0x8,r3 */
L8c618956: sa_motion_publish_stop(ctx,0x8c618956u); r[1]=rd(m,bus,&io_status,0x8c618a18u,4);if(io_status)return io_status;goto L8c618958; /* mov.l 0x8c618a18,r1 */
L8c618958: sa_motion_publish_stop(ctx,0x8c618958u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61895a; /* fmul fr3,fr2 */
L8c61895a: sa_motion_publish_stop(ctx,0x8c61895au); r[2]=rd(m,bus,&io_status,0x8c618a1cu,4);if(io_status)return io_status;goto L8c61895c; /* mov.l 0x8c618a1c,r2 */
L8c61895c: sa_motion_publish_stop(ctx,0x8c61895cu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61895e; /* fmov.s @r1,fr3 */
L8c61895e: sa_motion_publish_stop(ctx,0x8c61895eu); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c618960; /* fmov.s @r2,fr1 */
L8c618960: sa_motion_publish_stop(ctx,0x8c618960u); r[2]=rd(m,bus,&io_status,0x8c618a20u,4);if(io_status)return io_status;goto L8c618962; /* mov.l 0x8c618a20,r2 */
L8c618962: sa_motion_publish_stop(ctx,0x8c618962u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618964; /* fsub fr2,fr3 */
L8c618964: sa_motion_publish_stop(ctx,0x8c618964u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c618966; /* fmov.s @(r0,r12),fr2 */
L8c618966: sa_motion_publish_stop(ctx,0x8c618966u); r[0]=8u;goto L8c618968; /* mov #0x8,r0 */
L8c618968: sa_motion_publish_stop(ctx,0x8c618968u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61896a; /* fmul fr2,fr1 */
L8c61896a: sa_motion_publish_stop(ctx,0x8c61896au); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61896c; /* fmov.s @(r0,r12),fr2 */
L8c61896c: sa_motion_publish_stop(ctx,0x8c61896cu); r[0]=28u;goto L8c61896e; /* mov #0x1c,r0 */
L8c61896e: sa_motion_publish_stop(ctx,0x8c61896eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618970; /* fsub fr1,fr3 */
L8c618970: sa_motion_publish_stop(ctx,0x8c618970u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618972; /* fmov.s @r3,fr1 */
L8c618972: sa_motion_publish_stop(ctx,0x8c618972u); r[3]=rd(m,bus,&io_status,0x8c618a24u,4);if(io_status)return io_status;goto L8c618974; /* mov.l 0x8c618a24,r3 */
L8c618974: sa_motion_publish_stop(ctx,0x8c618974u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618976; /* fmul fr2,fr1 */
L8c618976: sa_motion_publish_stop(ctx,0x8c618976u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618978; /* fsub fr1,fr3 */
L8c618978: sa_motion_publish_stop(ctx,0x8c618978u); fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61897a; /* fmov fr3,@(r0,r12) */
L8c61897a: sa_motion_publish_stop(ctx,0x8c61897au); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61897c; /* fmov.s @r2,fr2 */
L8c61897c: sa_motion_publish_stop(ctx,0x8c61897cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61897e; /* fmul fr3,fr2 */
L8c61897e: sa_motion_publish_stop(ctx,0x8c61897eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618980; /* fmov.s fr2,@-r8 */
L8c618980: sa_motion_publish_stop(ctx,0x8c618980u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618982; /* fmov.s @r3,fr2 */
L8c618982: sa_motion_publish_stop(ctx,0x8c618982u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c618984; /* fmov.s @(r0,r12),fr3 */
L8c618984: sa_motion_publish_stop(ctx,0x8c618984u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618986; /* fmul fr3,fr2 */
L8c618986: sa_motion_publish_stop(ctx,0x8c618986u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618988; /* fmov.s fr2,@-r8 */
L8c618988: sa_motion_publish_stop(ctx,0x8c618988u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61898a; /* fmov.s @(r0,r12),fr3 */
L8c61898a: sa_motion_publish_stop(ctx,0x8c61898au); r[1]=rd(m,bus,&io_status,0x8c618a28u,4);if(io_status)return io_status;goto L8c61898c; /* mov.l 0x8c618a28,r1 */
L8c61898c: sa_motion_publish_stop(ctx,0x8c61898cu); r[2]+=4294967284u;goto L8c61898e; /* add #-0xc,r2 */
L8c61898e: sa_motion_publish_stop(ctx,0x8c61898eu); r[0]=24u;goto L8c618990; /* mov #0x18,r0 */
L8c618990: sa_motion_publish_stop(ctx,0x8c618990u); r[3]=rd(m,bus,&io_status,0x8c618a30u,4);if(io_status)return io_status;goto L8c618992; /* mov.l 0x8c618a30,r3 */
L8c618992: sa_motion_publish_stop(ctx,0x8c618992u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618994; /* fmov.s @r1,fr2 */
L8c618994: sa_motion_publish_stop(ctx,0x8c618994u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618996; /* fmul fr3,fr2 */
L8c618996: sa_motion_publish_stop(ctx,0x8c618996u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618998; /* fmov.s fr2,@-r8 */
L8c618998: sa_motion_publish_stop(ctx,0x8c618998u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61899a; /* fmov.s @r2,fr3 */
L8c61899a: sa_motion_publish_stop(ctx,0x8c61899au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61899c; /* fmov.s fr3,@-r8 */
L8c61899c: sa_motion_publish_stop(ctx,0x8c61899cu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61899e; /* fmov.s @(r0,r12),fr3 */
L8c61899e: sa_motion_publish_stop(ctx,0x8c61899eu); r[0]=20u;goto L8c6189a0; /* mov #0x14,r0 */
L8c6189a0: sa_motion_publish_stop(ctx,0x8c6189a0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6189a2; /* fmov.s fr3,@-r8 */
L8c6189a2: sa_motion_publish_stop(ctx,0x8c6189a2u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c6189a4; /* fmov.s @(r0,r12),fr3 */
L8c6189a4: sa_motion_publish_stop(ctx,0x8c6189a4u); r[0]=16u;goto L8c6189a6; /* mov #0x10,r0 */
L8c6189a6: sa_motion_publish_stop(ctx,0x8c6189a6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6189a8; /* fmov.s fr3,@-r8 */
L8c6189a8: sa_motion_publish_stop(ctx,0x8c6189a8u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c6189aa; /* fmov.s @(r0,r12),fr3 */
L8c6189aa: sa_motion_publish_stop(ctx,0x8c6189aau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6189ac; /* fmov.s fr3,@-r8 */
L8c6189ac: sa_motion_publish_stop(ctx,0x8c6189acu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6189ae; /* mov.l r3,@-r8 */
L8c6189ae: sa_motion_publish_stop(ctx,0x8c6189aeu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6189b0; /* pref @r8 */
L8c6189b0: sa_motion_publish_stop(ctx,0x8c6189b0u); r[0]=rd(m,bus,&io_status,0x8c618a34u,4);if(io_status)return io_status;goto L8c6189b2; /* mov.l 0x8c618a34,r0 */
L8c6189b2: sa_motion_publish_stop(ctx,0x8c6189b2u); r[8]+=32u;goto L8c6189b4; /* add #0x20,r8 */
L8c6189b4: sa_motion_publish_stop(ctx,0x8c6189b4u); r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c6189b6; /* mov.l @r0,r3 */
L8c6189b6: sa_motion_publish_stop(ctx,0x8c6189b6u); r[3]+=4u;goto L8c6189b8; /* add #0x4,r3 */
L8c6189b8: sa_motion_publish_stop(ctx,0x8c6189bau); wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c6189f2; /* bra 0x8c6189f2 */
L8c6189bc: sa_motion_publish_stop(ctx,0x8c6189bcu); r[0]=rd(m,bus,&io_status,0x8c618a38u,4);if(io_status)return io_status;goto L8c6189be; /* mov.l 0x8c618a38,r0 */
L8c6189be: sa_motion_publish_stop(ctx,0x8c6189beu); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c6189c0; /* mov.l r2,@r0 */
L8c6189c0: sa_motion_publish_stop(ctx,0x8c6189c0u); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6189c2; /* mov.l @(0xc,r4),r5 */
L8c6189c2: sa_motion_publish_stop(ctx,0x8c6189c2u); r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c6189c4; /* mov.l @(0xc,r13),r3 */
L8c6189c4: sa_motion_publish_stop(ctx,0x8c6189c4u); r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c6189c6; /* mov.l @(0xc,r14),r2 */
L8c6189c6: sa_motion_publish_stop(ctx,0x8c6189c6u); r[5]&=r[3];goto L8c6189c8; /* and r3,r5 */
L8c6189c8: sa_motion_publish_stop(ctx,0x8c6189c8u); r[3]=1u;goto L8c6189ca; /* mov #0x1,r3 */
L8c6189ca: sa_motion_publish_stop(ctx,0x8c6189cau); r[5]&=r[2];goto L8c6189cc; /* and r2,r5 */
L8c6189cc: sa_motion_publish_stop(ctx,0x8c6189ccu); *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c6189ce; /* tst r3,r5 */
L8c6189ce: sa_motion_publish_stop(ctx,0x8c6189ceu); if(!(*sr&1u))goto L8c6189d8;goto L8c6189d0; /* bf 0x8c6189d8 */
L8c6189d0: sa_motion_publish_stop(ctx,0x8c6189d0u); r[1]=rd(m,bus,&io_status,0x8c618a3cu,4);if(io_status)return io_status;goto L8c6189d2; /* mov.l 0x8c618a3c,r1 */
L8c6189d2: sa_motion_publish_stop(ctx,0x8c6189d2u); r[6]=r[14];goto L8c6189d4; /* mov r14,r6 */
L8c6189d4: sa_motion_publish_stop(ctx,0x8c6189d6u); target=r[1];*pr=0x8c6189d8u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6189d8; /* jsr @r1 */
L8c6189d8: sa_motion_publish_stop(ctx,0x8c6189d8u); r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c6189da; /* mov.l @(0xc,r13),r4 */
L8c6189da: sa_motion_publish_stop(ctx,0x8c6189dau); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c6189dc; /* mov.l @(0xc,r14),r3 */
L8c6189dc: sa_motion_publish_stop(ctx,0x8c6189dcu); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c6189de; /* mov.l @(0xc,r12),r2 */
L8c6189de: sa_motion_publish_stop(ctx,0x8c6189deu); r[4]&=r[3];goto L8c6189e0; /* and r3,r4 */
L8c6189e0: sa_motion_publish_stop(ctx,0x8c6189e0u); r[3]=1u;goto L8c6189e2; /* mov #0x1,r3 */
L8c6189e2: sa_motion_publish_stop(ctx,0x8c6189e2u); r[4]&=r[2];goto L8c6189e4; /* and r2,r4 */
L8c6189e4: sa_motion_publish_stop(ctx,0x8c6189e4u); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c6189e6; /* tst r3,r4 */
L8c6189e6: sa_motion_publish_stop(ctx,0x8c6189e6u); if(!(*sr&1u))goto L8c6189f2;goto L8c6189e8; /* bf 0x8c6189f2 */
L8c6189e8: sa_motion_publish_stop(ctx,0x8c6189e8u); r[1]=rd(m,bus,&io_status,0x8c618a3cu,4);if(io_status)return io_status;goto L8c6189ea; /* mov.l 0x8c618a3c,r1 */
L8c6189ea: sa_motion_publish_stop(ctx,0x8c6189eau); r[5]=r[14];goto L8c6189ec; /* mov r14,r5 */
L8c6189ec: sa_motion_publish_stop(ctx,0x8c6189ecu); r[6]=r[12];goto L8c6189ee; /* mov r12,r6 */
L8c6189ee: sa_motion_publish_stop(ctx,0x8c6189f0u); target=r[1];*pr=0x8c6189f2u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6189f2; /* jsr @r1 */
L8c6189f2: sa_motion_publish_stop(ctx,0x8c6189f2u); r[10]+=1u;goto L8c6189f4; /* add #0x1,r10 */
L8c6189f4: sa_motion_publish_stop(ctx,0x8c6189f4u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6189f6; /* mov.l @r15,r3 */
L8c6189f6: sa_motion_publish_stop(ctx,0x8c6189f6u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6189f8; /* mov.w @(0x2,r3),r0 */
L8c6189f8: sa_motion_publish_stop(ctx,0x8c6189f8u); r[0]=r[0]&65535u;goto L8c6189fa; /* extu.w r0,r0 */
L8c6189fa: sa_motion_publish_stop(ctx,0x8c6189fau); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c6189fc; /* cmp/ge r0,r10 */
L8c6189fc: sa_motion_publish_stop(ctx,0x8c6189fcu); if((*sr&1u))goto L8c618a02;goto L8c6189fe; /* bt 0x8c618a02 */
L8c6189fe: sa_motion_publish_stop(ctx,0x8c618a00u); goto L8c618784; /* bra 0x8c618784 */
L8c618a02: sa_motion_publish_stop(ctx,0x8c618a04u); goto L8c618cb4; /* bra 0x8c618cb4 */
L8c618a06: sa_motion_publish_stop(ctx,0x8c618a08u); r[9]=0u;goto L8c618c4c; /* bra 0x8c618c4c */
L8c618a40: sa_motion_publish_stop(ctx,0x8c618a40u); r[3]=rd(m,bus,&io_status,0x8c618cd8u,4);if(io_status)return io_status;goto L8c618a42; /* mov.l 0x8c618cd8,r3 */
L8c618a42: sa_motion_publish_stop(ctx,0x8c618a42u); r[6]=0u;goto L8c618a44; /* mov #0x0,r6 */
L8c618a44: sa_motion_publish_stop(ctx,0x8c618a44u); r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618a46; /* mov.w @r11+,r13 */
L8c618a46: sa_motion_publish_stop(ctx,0x8c618a46u); r[4]=0u;goto L8c618a48; /* mov #0x0,r4 */
L8c618a48: sa_motion_publish_stop(ctx,0x8c618a48u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c618a4a; /* mov.l @r3,r2 */
L8c618a4a: sa_motion_publish_stop(ctx,0x8c618a4au); r[5]=r[6];goto L8c618a4c; /* mov r6,r5 */
L8c618a4c: sa_motion_publish_stop(ctx,0x8c618a4cu); r[13]=r[13]&65535u;goto L8c618a4e; /* extu.w r13,r13 */
L8c618a4e: sa_motion_publish_stop(ctx,0x8c618a4eu); r[2]+=r[13];goto L8c618a50; /* add r13,r2 */
L8c618a50: sa_motion_publish_stop(ctx,0x8c618a50u); *sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c618a52; /* cmp/pl r13 */
L8c618a52: sa_motion_publish_stop(ctx,0x8c618a52u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c618a54; /* mov.l r2,@r3 */
L8c618a54: sa_motion_publish_stop(ctx,0x8c618a54u); r[7]=4294967295u;goto L8c618a56; /* mov #-0x1,r7 */
L8c618a56: sa_motion_publish_stop(ctx,0x8c618a58u); take=!(*sr&1u);r[4]+=r[11];if(take)goto L8c618a78;goto L8c618a5a; /* bf/s 0x8c618a78 */
L8c618a5a: sa_motion_publish_stop(ctx,0x8c618a5au); r[14]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c618a5c; /* mov.w @r4+,r14 */
L8c618a5c: sa_motion_publish_stop(ctx,0x8c618a5cu); r[5]+=1u;goto L8c618a5e; /* add #0x1,r5 */
L8c618a5e: sa_motion_publish_stop(ctx,0x8c618a5eu); r[2]=rd(m,bus,&io_status,0x8c618cdcu,4);if(io_status)return io_status;goto L8c618a60; /* mov.l 0x8c618cdc,r2 */
L8c618a60: sa_motion_publish_stop(ctx,0x8c618a60u); r[14]=r[14]&65535u;goto L8c618a62; /* extu.w r14,r14 */
L8c618a62: sa_motion_publish_stop(ctx,0x8c618a62u); r[14]<<=2;goto L8c618a64; /* shll2 r14 */
L8c618a64: sa_motion_publish_stop(ctx,0x8c618a64u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c618a66; /* mov.l @r2,r3 */
L8c618a66: sa_motion_publish_stop(ctx,0x8c618a66u); r[14]<<=2;goto L8c618a68; /* shll2 r14 */
L8c618a68: sa_motion_publish_stop(ctx,0x8c618a68u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c618a6a; /* shll r14 */
L8c618a6a: sa_motion_publish_stop(ctx,0x8c618a6au); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[13]);goto L8c618a6c; /* cmp/ge r13,r5 */
L8c618a6c: sa_motion_publish_stop(ctx,0x8c618a6cu); r[14]+=r[3];goto L8c618a6e; /* add r3,r14 */
L8c618a6e: sa_motion_publish_stop(ctx,0x8c618a6eu); wr(m,bus,&io_status,r[15]+0x4u,r[14],4);if(io_status)return io_status;goto L8c618a70; /* mov.l r14,@(0x4,r15) */
L8c618a70: sa_motion_publish_stop(ctx,0x8c618a70u); r[14]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c618a72; /* mov.l @(0xc,r14),r14 */
L8c618a72: sa_motion_publish_stop(ctx,0x8c618a72u); r[7]&=r[14];goto L8c618a74; /* and r14,r7 */
L8c618a74: sa_motion_publish_stop(ctx,0x8c618a76u); take=!(*sr&1u);r[6]|=r[14];if(take)goto L8c618a5a;goto L8c618a78; /* bf/s 0x8c618a5a */
L8c618a78: sa_motion_publish_stop(ctx,0x8c618a78u); r[2]=63u;goto L8c618a7a; /* mov #0x3f,r2 */
L8c618a7a: sa_motion_publish_stop(ctx,0x8c618a7au); *sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c618a7c; /* tst r2,r7 */
L8c618a7c: sa_motion_publish_stop(ctx,0x8c618a7cu); if(!(*sr&1u))goto L8c618adc;goto L8c618a7e; /* bf 0x8c618adc */
L8c618a7e: sa_motion_publish_stop(ctx,0x8c618a7eu); r[1]=1u;goto L8c618a80; /* mov #0x1,r1 */
L8c618a80: sa_motion_publish_stop(ctx,0x8c618a80u); *sr=(*sr&~1u)|!(r[1]&r[6]);goto L8c618a82; /* tst r1,r6 */
L8c618a82: sa_motion_publish_stop(ctx,0x8c618a82u); if((*sr&1u))goto L8c618a88;goto L8c618a84; /* bt 0x8c618a88 */
L8c618a84: sa_motion_publish_stop(ctx,0x8c618a86u); goto L8c618bec; /* bra 0x8c618bec */
L8c618a88: sa_motion_publish_stop(ctx,0x8c618a88u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c618a8a; /* mov.w @(0x2,r11),r0 */
L8c618a8a: sa_motion_publish_stop(ctx,0x8c618a8au); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c618a8c; /* mov.w @r11,r4 */
L8c618a8c: sa_motion_publish_stop(ctx,0x8c618a8cu); r[3]=rd(m,bus,&io_status,0x8c618cdcu,4);if(io_status)return io_status;goto L8c618a8e; /* mov.l 0x8c618cdc,r3 */
L8c618a8e: sa_motion_publish_stop(ctx,0x8c618a8eu); r[5]=r[0]&65535u;goto L8c618a90; /* extu.w r0,r5 */
L8c618a90: sa_motion_publish_stop(ctx,0x8c618a90u); r[5]<<=2;goto L8c618a92; /* shll2 r5 */
L8c618a92: sa_motion_publish_stop(ctx,0x8c618a92u); r[0]=sx(m,bus,&io_status,r[11]+0x4u,2);if(io_status)return io_status;goto L8c618a94; /* mov.w @(0x4,r11),r0 */
L8c618a94: sa_motion_publish_stop(ctx,0x8c618a94u); r[4]=r[4]&65535u;goto L8c618a96; /* extu.w r4,r4 */
L8c618a96: sa_motion_publish_stop(ctx,0x8c618a96u); r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c618a98; /* mov.l @r3,r7 */
L8c618a98: sa_motion_publish_stop(ctx,0x8c618a98u); r[4]<<=2;goto L8c618a9a; /* shll2 r4 */
L8c618a9a: sa_motion_publish_stop(ctx,0x8c618a9au); r[5]<<=2;goto L8c618a9c; /* shll2 r5 */
L8c618a9c: sa_motion_publish_stop(ctx,0x8c618a9cu); r[6]=r[0]&65535u;goto L8c618a9e; /* extu.w r0,r6 */
L8c618a9e: sa_motion_publish_stop(ctx,0x8c618a9eu); r[4]<<=2;goto L8c618aa0; /* shll2 r4 */
L8c618aa0: sa_motion_publish_stop(ctx,0x8c618aa0u); r[0]=16u;goto L8c618aa2; /* mov #0x10,r0 */
L8c618aa2: sa_motion_publish_stop(ctx,0x8c618aa2u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c618aa4; /* shll r4 */
L8c618aa4: sa_motion_publish_stop(ctx,0x8c618aa4u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c618aa6; /* shll r5 */
L8c618aa6: sa_motion_publish_stop(ctx,0x8c618aa6u); r[5]+=r[7];goto L8c618aa8; /* add r7,r5 */
L8c618aa8: sa_motion_publish_stop(ctx,0x8c618aa8u); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c618aaa; /* fmov.s @(r0,r5),fr5 */
L8c618aaa: sa_motion_publish_stop(ctx,0x8c618aaau); r[6]<<=2;goto L8c618aac; /* shll2 r6 */
L8c618aac: sa_motion_publish_stop(ctx,0x8c618aacu); r[4]+=r[7];goto L8c618aae; /* add r7,r4 */
L8c618aae: sa_motion_publish_stop(ctx,0x8c618aaeu); r[6]<<=2;goto L8c618ab0; /* shll2 r6 */
L8c618ab0: sa_motion_publish_stop(ctx,0x8c618ab0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618ab2; /* fmov.s @(r0,r4),fr3 */
L8c618ab2: sa_motion_publish_stop(ctx,0x8c618ab2u); fcopy(f,2u,5u);goto L8c618ab4; /* fmov fr5,fr2 */
L8c618ab4: sa_motion_publish_stop(ctx,0x8c618ab4u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c618ab6; /* shll r6 */
L8c618ab6: sa_motion_publish_stop(ctx,0x8c618ab6u); r[0]=20u;goto L8c618ab8; /* mov #0x14,r0 */
L8c618ab8: sa_motion_publish_stop(ctx,0x8c618ab8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c618aba; /* fsub fr3,fr2 */
L8c618aba: sa_motion_publish_stop(ctx,0x8c618abau); r[6]+=r[7];goto L8c618abc; /* add r7,r6 */
L8c618abc: sa_motion_publish_stop(ctx,0x8c618abcu); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c618abe; /* fmov.s @(r0,r5),fr4 */
L8c618abe: sa_motion_publish_stop(ctx,0x8c618abeu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618ac0; /* fmov.s @(r0,r6),fr3 */
L8c618ac0: sa_motion_publish_stop(ctx,0x8c618ac0u); fcopy(f,1u,4u);goto L8c618ac2; /* fmov fr4,fr1 */
L8c618ac2: sa_motion_publish_stop(ctx,0x8c618ac2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618ac4; /* fsub fr4,fr3 */
L8c618ac4: sa_motion_publish_stop(ctx,0x8c618ac4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618ac6; /* fmul fr3,fr2 */
L8c618ac6: sa_motion_publish_stop(ctx,0x8c618ac6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618ac8; /* fmov.s @(r0,r4),fr3 */
L8c618ac8: sa_motion_publish_stop(ctx,0x8c618ac8u); r[0]=16u;goto L8c618aca; /* mov #0x10,r0 */
L8c618aca: sa_motion_publish_stop(ctx,0x8c618acau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c618acc; /* fsub fr3,fr1 */
L8c618acc: sa_motion_publish_stop(ctx,0x8c618accu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c618ace; /* fmov.s @(r0,r6),fr3 */
L8c618ace: sa_motion_publish_stop(ctx,0x8c618aceu); fcopy(f,4u,2u);goto L8c618ad0; /* fmov fr2,fr4 */
L8c618ad0: sa_motion_publish_stop(ctx,0x8c618ad0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618ad2; /* fsub fr5,fr3 */
L8c618ad2: sa_motion_publish_stop(ctx,0x8c618ad2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618ad4; /* fmul fr3,fr1 */
L8c618ad4: sa_motion_publish_stop(ctx,0x8c618ad4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c618ad6; /* fldi0 fr3 */
L8c618ad6: sa_motion_publish_stop(ctx,0x8c618ad6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c618ad8; /* fsub fr1,fr4 */
L8c618ad8: sa_motion_publish_stop(ctx,0x8c618ad8u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c618ada; /* fcmp/gt fr3,fr4 */
L8c618ada: sa_motion_publish_stop(ctx,0x8c618adau); if(!(*sr&1u))goto L8c618ae2;goto L8c618adc; /* bf 0x8c618ae2 */
L8c618adc: sa_motion_publish_stop(ctx,0x8c618adcu); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c618ade; /* shll r13 */
L8c618ade: sa_motion_publish_stop(ctx,0x8c618ae0u); r[11]+=r[13];goto L8c618c4a; /* bra 0x8c618c4a */
L8c618ae2: sa_motion_publish_stop(ctx,0x8c618ae2u); r[2]=rd(m,bus,&io_status,0x8c618ce0u,4);if(io_status)return io_status;goto L8c618ae4; /* mov.l 0x8c618ce0,r2 */
L8c618ae4: sa_motion_publish_stop(ctx,0x8c618ae4u); r[6]=r[13];goto L8c618ae6; /* mov r13,r6 */
L8c618ae6: sa_motion_publish_stop(ctx,0x8c618ae6u); r[6]+=4294967295u;goto L8c618ae8; /* add #-0x1,r6 */
L8c618ae8: sa_motion_publish_stop(ctx,0x8c618ae8u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c618aea; /* mov.l @r2,r3 */
L8c618aea: sa_motion_publish_stop(ctx,0x8c618aeau); *sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c618aec; /* cmp/pl r6 */
L8c618aec: sa_motion_publish_stop(ctx,0x8c618aecu); r[3]+=r[13];goto L8c618aee; /* add r13,r3 */
L8c618aee: sa_motion_publish_stop(ctx,0x8c618aeeu); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c618af0; /* mov.l r3,@r2 */
L8c618af0: sa_motion_publish_stop(ctx,0x8c618af2u); take=!(*sr&1u);r[5]=0u;if(take)goto L8c618b72;goto L8c618af4; /* bf/s 0x8c618b72 */
L8c618af4: sa_motion_publish_stop(ctx,0x8c618af4u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618af6; /* mov.w @r11+,r4 */
L8c618af6: sa_motion_publish_stop(ctx,0x8c618af6u); r[0]=4u;goto L8c618af8; /* mov #0x4,r0 */
L8c618af8: sa_motion_publish_stop(ctx,0x8c618af8u); r[2]=rd(m,bus,&io_status,0x8c618cdcu,4);if(io_status)return io_status;goto L8c618afa; /* mov.l 0x8c618cdc,r2 */
L8c618afa: sa_motion_publish_stop(ctx,0x8c618afau); r[8]+=32u;goto L8c618afc; /* add #0x20,r8 */
L8c618afc: sa_motion_publish_stop(ctx,0x8c618afcu); r[4]=r[4]&65535u;goto L8c618afe; /* extu.w r4,r4 */
L8c618afe: sa_motion_publish_stop(ctx,0x8c618afeu); r[1]=rd(m,bus,&io_status,0x8c618ce4u,4);if(io_status)return io_status;goto L8c618b00; /* mov.l 0x8c618ce4,r1 */
L8c618b00: sa_motion_publish_stop(ctx,0x8c618b00u); r[4]<<=2;goto L8c618b02; /* shll2 r4 */
L8c618b02: sa_motion_publish_stop(ctx,0x8c618b02u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c618b04; /* mov.l @r2,r3 */
L8c618b04: sa_motion_publish_stop(ctx,0x8c618b04u); r[4]<<=2;goto L8c618b06; /* shll2 r4 */
L8c618b06: sa_motion_publish_stop(ctx,0x8c618b06u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618b08; /* fmov.s @r1,fr2 */
L8c618b08: sa_motion_publish_stop(ctx,0x8c618b08u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c618b0a; /* shll r4 */
L8c618b0a: sa_motion_publish_stop(ctx,0x8c618b0au); r[2]=rd(m,bus,&io_status,0x8c618cecu,4);if(io_status)return io_status;goto L8c618b0c; /* mov.l 0x8c618cec,r2 */
L8c618b0c: sa_motion_publish_stop(ctx,0x8c618b0cu); r[4]+=r[3];goto L8c618b0e; /* add r3,r4 */
L8c618b0e: sa_motion_publish_stop(ctx,0x8c618b0eu); r[3]=rd(m,bus,&io_status,0x8c618ce8u,4);if(io_status)return io_status;goto L8c618b10; /* mov.l 0x8c618ce8,r3 */
L8c618b10: sa_motion_publish_stop(ctx,0x8c618b10u); fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c618b12; /* fmov.s @r4,fr3 */
L8c618b12: sa_motion_publish_stop(ctx,0x8c618b12u); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c618b14; /* fmov.s @r2,fr1 */
L8c618b14: sa_motion_publish_stop(ctx,0x8c618b14u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618b16; /* fmul fr3,fr2 */
L8c618b16: sa_motion_publish_stop(ctx,0x8c618b16u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618b18; /* fmov.s @r3,fr3 */
L8c618b18: sa_motion_publish_stop(ctx,0x8c618b18u); r[3]=rd(m,bus,&io_status,0x8c618cf0u,4);if(io_status)return io_status;goto L8c618b1a; /* mov.l 0x8c618cf0,r3 */
L8c618b1a: sa_motion_publish_stop(ctx,0x8c618b1au); r[2]=rd(m,bus,&io_status,0x8c618cf4u,4);if(io_status)return io_status;goto L8c618b1c; /* mov.l 0x8c618cf4,r2 */
L8c618b1c: sa_motion_publish_stop(ctx,0x8c618b1cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618b1e; /* fsub fr2,fr3 */
L8c618b1e: sa_motion_publish_stop(ctx,0x8c618b1eu); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b20; /* fmov.s @(r0,r4),fr2 */
L8c618b20: sa_motion_publish_stop(ctx,0x8c618b20u); r[0]=8u;goto L8c618b22; /* mov #0x8,r0 */
L8c618b22: sa_motion_publish_stop(ctx,0x8c618b22u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618b24; /* fmul fr2,fr1 */
L8c618b24: sa_motion_publish_stop(ctx,0x8c618b24u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b26; /* fmov.s @(r0,r4),fr2 */
L8c618b26: sa_motion_publish_stop(ctx,0x8c618b26u); r[0]=28u;goto L8c618b28; /* mov #0x1c,r0 */
L8c618b28: sa_motion_publish_stop(ctx,0x8c618b28u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618b2a; /* fsub fr1,fr3 */
L8c618b2a: sa_motion_publish_stop(ctx,0x8c618b2au); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618b2c; /* fmov.s @r3,fr1 */
L8c618b2c: sa_motion_publish_stop(ctx,0x8c618b2cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618b2e; /* fmul fr2,fr1 */
L8c618b2e: sa_motion_publish_stop(ctx,0x8c618b2eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618b30; /* fsub fr1,fr3 */
L8c618b30: sa_motion_publish_stop(ctx,0x8c618b30u); fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b32; /* fmov fr3,@(r0,r4) */
L8c618b32: sa_motion_publish_stop(ctx,0x8c618b32u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618b34; /* fmov.s @r2,fr2 */
L8c618b34: sa_motion_publish_stop(ctx,0x8c618b34u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618b36; /* fmul fr3,fr2 */
L8c618b36: sa_motion_publish_stop(ctx,0x8c618b36u); r[3]=rd(m,bus,&io_status,0x8c618cf8u,4);if(io_status)return io_status;goto L8c618b38; /* mov.l 0x8c618cf8,r3 */
L8c618b38: sa_motion_publish_stop(ctx,0x8c618b38u); r[1]=rd(m,bus,&io_status,0x8c618cfcu,4);if(io_status)return io_status;goto L8c618b3a; /* mov.l 0x8c618cfc,r1 */
L8c618b3a: sa_motion_publish_stop(ctx,0x8c618b3au); r[2]+=4294967284u;goto L8c618b3c; /* add #-0xc,r2 */
L8c618b3c: sa_motion_publish_stop(ctx,0x8c618b3cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618b3e; /* fmov.s fr2,@-r8 */
L8c618b3e: sa_motion_publish_stop(ctx,0x8c618b3eu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618b40; /* fmov.s @r3,fr2 */
L8c618b40: sa_motion_publish_stop(ctx,0x8c618b40u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b42; /* fmov.s @(r0,r4),fr3 */
L8c618b42: sa_motion_publish_stop(ctx,0x8c618b42u); r[3]=rd(m,bus,&io_status,0x8c618d00u,4);if(io_status)return io_status;goto L8c618b44; /* mov.l 0x8c618d00,r3 */
L8c618b44: sa_motion_publish_stop(ctx,0x8c618b44u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618b46; /* fmul fr3,fr2 */
L8c618b46: sa_motion_publish_stop(ctx,0x8c618b46u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618b48; /* fmov.s fr2,@-r8 */
L8c618b48: sa_motion_publish_stop(ctx,0x8c618b48u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618b4a; /* fmov.s @r1,fr2 */
L8c618b4a: sa_motion_publish_stop(ctx,0x8c618b4au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b4c; /* fmov.s @(r0,r4),fr3 */
L8c618b4c: sa_motion_publish_stop(ctx,0x8c618b4cu); r[0]=24u;goto L8c618b4e; /* mov #0x18,r0 */
L8c618b4e: sa_motion_publish_stop(ctx,0x8c618b4eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618b50; /* fmul fr3,fr2 */
L8c618b50: sa_motion_publish_stop(ctx,0x8c618b50u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618b52; /* fmov.s fr2,@-r8 */
L8c618b52: sa_motion_publish_stop(ctx,0x8c618b52u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618b54; /* fmov.s @r2,fr3 */
L8c618b54: sa_motion_publish_stop(ctx,0x8c618b54u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618b56; /* fmov.s fr3,@-r8 */
L8c618b56: sa_motion_publish_stop(ctx,0x8c618b56u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b58; /* fmov.s @(r0,r4),fr3 */
L8c618b58: sa_motion_publish_stop(ctx,0x8c618b58u); r[0]=20u;goto L8c618b5a; /* mov #0x14,r0 */
L8c618b5a: sa_motion_publish_stop(ctx,0x8c618b5au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618b5c; /* fmov.s fr3,@-r8 */
L8c618b5c: sa_motion_publish_stop(ctx,0x8c618b5cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b5e; /* fmov.s @(r0,r4),fr3 */
L8c618b5e: sa_motion_publish_stop(ctx,0x8c618b5eu); r[0]=16u;goto L8c618b60; /* mov #0x10,r0 */
L8c618b60: sa_motion_publish_stop(ctx,0x8c618b60u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618b62; /* fmov.s fr3,@-r8 */
L8c618b62: sa_motion_publish_stop(ctx,0x8c618b62u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b64; /* fmov.s @(r0,r4),fr3 */
L8c618b64: sa_motion_publish_stop(ctx,0x8c618b64u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618b66; /* fmov.s fr3,@-r8 */
L8c618b66: sa_motion_publish_stop(ctx,0x8c618b66u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c618b68; /* mov.l r3,@-r8 */
L8c618b68: sa_motion_publish_stop(ctx,0x8c618b68u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618b6a; /* pref @r8 */
L8c618b6a: sa_motion_publish_stop(ctx,0x8c618b6au); r[5]+=1u;goto L8c618b6c; /* add #0x1,r5 */
L8c618b6c: sa_motion_publish_stop(ctx,0x8c618b6cu); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c618b6e; /* cmp/ge r6,r5 */
L8c618b6e: sa_motion_publish_stop(ctx,0x8c618b70u); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c618af4;goto L8c618b72; /* bf/s 0x8c618af4 */
L8c618b72: sa_motion_publish_stop(ctx,0x8c618b72u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618b74; /* mov.w @r11+,r4 */
L8c618b74: sa_motion_publish_stop(ctx,0x8c618b74u); r[0]=4u;goto L8c618b76; /* mov #0x4,r0 */
L8c618b76: sa_motion_publish_stop(ctx,0x8c618b76u); r[2]=rd(m,bus,&io_status,0x8c618cdcu,4);if(io_status)return io_status;goto L8c618b78; /* mov.l 0x8c618cdc,r2 */
L8c618b78: sa_motion_publish_stop(ctx,0x8c618b78u); r[8]+=32u;goto L8c618b7a; /* add #0x20,r8 */
L8c618b7a: sa_motion_publish_stop(ctx,0x8c618b7au); r[4]=r[4]&65535u;goto L8c618b7c; /* extu.w r4,r4 */
L8c618b7c: sa_motion_publish_stop(ctx,0x8c618b7cu); r[1]=rd(m,bus,&io_status,0x8c618ce4u,4);if(io_status)return io_status;goto L8c618b7e; /* mov.l 0x8c618ce4,r1 */
L8c618b7e: sa_motion_publish_stop(ctx,0x8c618b7eu); r[4]<<=2;goto L8c618b80; /* shll2 r4 */
L8c618b80: sa_motion_publish_stop(ctx,0x8c618b80u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c618b82; /* mov.l @r2,r3 */
L8c618b82: sa_motion_publish_stop(ctx,0x8c618b82u); r[4]<<=2;goto L8c618b84; /* shll2 r4 */
L8c618b84: sa_motion_publish_stop(ctx,0x8c618b84u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618b86; /* fmov.s @r1,fr2 */
L8c618b86: sa_motion_publish_stop(ctx,0x8c618b86u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c618b88; /* shll r4 */
L8c618b88: sa_motion_publish_stop(ctx,0x8c618b88u); r[2]=rd(m,bus,&io_status,0x8c618cecu,4);if(io_status)return io_status;goto L8c618b8a; /* mov.l 0x8c618cec,r2 */
L8c618b8a: sa_motion_publish_stop(ctx,0x8c618b8au); r[4]+=r[3];goto L8c618b8c; /* add r3,r4 */
L8c618b8c: sa_motion_publish_stop(ctx,0x8c618b8cu); r[3]=rd(m,bus,&io_status,0x8c618ce8u,4);if(io_status)return io_status;goto L8c618b8e; /* mov.l 0x8c618ce8,r3 */
L8c618b8e: sa_motion_publish_stop(ctx,0x8c618b8eu); fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c618b90; /* fmov.s @r4,fr3 */
L8c618b90: sa_motion_publish_stop(ctx,0x8c618b90u); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c618b92; /* fmov.s @r2,fr1 */
L8c618b92: sa_motion_publish_stop(ctx,0x8c618b92u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618b94; /* fmul fr3,fr2 */
L8c618b94: sa_motion_publish_stop(ctx,0x8c618b94u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c618b96; /* fmov.s @r3,fr3 */
L8c618b96: sa_motion_publish_stop(ctx,0x8c618b96u); r[3]=rd(m,bus,&io_status,0x8c618cf0u,4);if(io_status)return io_status;goto L8c618b98; /* mov.l 0x8c618cf0,r3 */
L8c618b98: sa_motion_publish_stop(ctx,0x8c618b98u); r[2]=rd(m,bus,&io_status,0x8c618cf4u,4);if(io_status)return io_status;goto L8c618b9a; /* mov.l 0x8c618cf4,r2 */
L8c618b9a: sa_motion_publish_stop(ctx,0x8c618b9au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618b9c; /* fsub fr2,fr3 */
L8c618b9c: sa_motion_publish_stop(ctx,0x8c618b9cu); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c618b9e; /* fmov.s @(r0,r4),fr2 */
L8c618b9e: sa_motion_publish_stop(ctx,0x8c618b9eu); r[0]=8u;goto L8c618ba0; /* mov #0x8,r0 */
L8c618ba0: sa_motion_publish_stop(ctx,0x8c618ba0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618ba2; /* fmul fr2,fr1 */
L8c618ba2: sa_motion_publish_stop(ctx,0x8c618ba2u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c618ba4; /* fmov.s @(r0,r4),fr2 */
L8c618ba4: sa_motion_publish_stop(ctx,0x8c618ba4u); r[0]=28u;goto L8c618ba6; /* mov #0x1c,r0 */
L8c618ba6: sa_motion_publish_stop(ctx,0x8c618ba6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618ba8; /* fsub fr1,fr3 */
L8c618ba8: sa_motion_publish_stop(ctx,0x8c618ba8u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c618baa; /* fmov.s @r3,fr1 */
L8c618baa: sa_motion_publish_stop(ctx,0x8c618baau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c618bac; /* fmul fr2,fr1 */
L8c618bac: sa_motion_publish_stop(ctx,0x8c618bacu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c618bae; /* fsub fr1,fr3 */
L8c618bae: sa_motion_publish_stop(ctx,0x8c618baeu); fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618bb0; /* fmov fr3,@(r0,r4) */
L8c618bb0: sa_motion_publish_stop(ctx,0x8c618bb0u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c618bb2; /* fmov.s @r2,fr2 */
L8c618bb2: sa_motion_publish_stop(ctx,0x8c618bb2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618bb4; /* fmul fr3,fr2 */
L8c618bb4: sa_motion_publish_stop(ctx,0x8c618bb4u); r[3]=rd(m,bus,&io_status,0x8c618cf8u,4);if(io_status)return io_status;goto L8c618bb6; /* mov.l 0x8c618cf8,r3 */
L8c618bb6: sa_motion_publish_stop(ctx,0x8c618bb6u); r[1]=rd(m,bus,&io_status,0x8c618cfcu,4);if(io_status)return io_status;goto L8c618bb8; /* mov.l 0x8c618cfc,r1 */
L8c618bb8: sa_motion_publish_stop(ctx,0x8c618bb8u); r[2]+=4294967284u;goto L8c618bba; /* add #-0xc,r2 */
L8c618bba: sa_motion_publish_stop(ctx,0x8c618bbau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618bbc; /* fmov.s fr2,@-r8 */
L8c618bbc: sa_motion_publish_stop(ctx,0x8c618bbcu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c618bbe; /* fmov.s @r3,fr2 */
L8c618bbe: sa_motion_publish_stop(ctx,0x8c618bbeu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618bc0; /* fmov.s @(r0,r4),fr3 */
L8c618bc0: sa_motion_publish_stop(ctx,0x8c618bc0u); r[3]=rd(m,bus,&io_status,0x8c618d04u,4);if(io_status)return io_status;goto L8c618bc2; /* mov.l 0x8c618d04,r3 */
L8c618bc2: sa_motion_publish_stop(ctx,0x8c618bc2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618bc4; /* fmul fr3,fr2 */
L8c618bc4: sa_motion_publish_stop(ctx,0x8c618bc4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618bc6; /* fmov.s fr2,@-r8 */
L8c618bc6: sa_motion_publish_stop(ctx,0x8c618bc6u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c618bc8; /* fmov.s @r1,fr2 */
L8c618bc8: sa_motion_publish_stop(ctx,0x8c618bc8u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618bca; /* fmov.s @(r0,r4),fr3 */
L8c618bca: sa_motion_publish_stop(ctx,0x8c618bcau); r[0]=24u;goto L8c618bcc; /* mov #0x18,r0 */
L8c618bcc: sa_motion_publish_stop(ctx,0x8c618bccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c618bce; /* fmul fr3,fr2 */
L8c618bce: sa_motion_publish_stop(ctx,0x8c618bceu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c618bd0; /* fmov.s fr2,@-r8 */
L8c618bd0: sa_motion_publish_stop(ctx,0x8c618bd0u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c618bd2; /* fmov.s @r2,fr3 */
L8c618bd2: sa_motion_publish_stop(ctx,0x8c618bd2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618bd4; /* fmov.s fr3,@-r8 */
L8c618bd4: sa_motion_publish_stop(ctx,0x8c618bd4u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618bd6; /* fmov.s @(r0,r4),fr3 */
L8c618bd6: sa_motion_publish_stop(ctx,0x8c618bd6u); r[0]=20u;goto L8c618bd8; /* mov #0x14,r0 */
L8c618bd8: sa_motion_publish_stop(ctx,0x8c618bd8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618bda; /* fmov.s fr3,@-r8 */
L8c618bda: sa_motion_publish_stop(ctx,0x8c618bdau); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618bdc; /* fmov.s @(r0,r4),fr3 */
L8c618bdc: sa_motion_publish_stop(ctx,0x8c618bdcu); r[0]=16u;goto L8c618bde; /* mov #0x10,r0 */
L8c618bde: sa_motion_publish_stop(ctx,0x8c618bdeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618be0; /* fmov.s fr3,@-r8 */
L8c618be0: sa_motion_publish_stop(ctx,0x8c618be0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c618be2; /* fmov.s @(r0,r4),fr3 */
L8c618be2: sa_motion_publish_stop(ctx,0x8c618be2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c618be4; /* fmov.s fr3,@-r8 */
L8c618be4: sa_motion_publish_stop(ctx,0x8c618be4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c618be6; /* mov.l r3,@-r8 */
L8c618be6: sa_motion_publish_stop(ctx,0x8c618be6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c618be8; /* pref @r8 */
L8c618be8: sa_motion_publish_stop(ctx,0x8c618beau); r[8]+=32u;goto L8c618c4a; /* bra 0x8c618c4a */
L8c618bec: sa_motion_publish_stop(ctx,0x8c618becu); r[2]=rd(m,bus,&io_status,0x8c618d08u,4);if(io_status)return io_status;goto L8c618bee; /* mov.l 0x8c618d08,r2 */
L8c618bee: sa_motion_publish_stop(ctx,0x8c618beeu); r[3]=0u;goto L8c618bf0; /* mov #0x0,r3 */
L8c618bf0: sa_motion_publish_stop(ctx,0x8c618bf0u); r[0]=rd(m,bus,&io_status,0x8c618cdcu,4);if(io_status)return io_status;goto L8c618bf2; /* mov.l 0x8c618cdc,r0 */
L8c618bf2: sa_motion_publish_stop(ctx,0x8c618bf2u); r[10]=r[13];goto L8c618bf4; /* mov r13,r10 */
L8c618bf4: sa_motion_publish_stop(ctx,0x8c618bf4u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c618bf6; /* mov.l r3,@r2 */
L8c618bf6: sa_motion_publish_stop(ctx,0x8c618bf6u); r[10]+=4294967294u;goto L8c618bf8; /* add #-0x2,r10 */
L8c618bf8: sa_motion_publish_stop(ctx,0x8c618bf8u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618bfa; /* mov.w @r11+,r4 */
L8c618bfa: sa_motion_publish_stop(ctx,0x8c618bfau); r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c618bfc; /* mov.l @r0,r3 */
L8c618bfc: sa_motion_publish_stop(ctx,0x8c618bfcu); r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618bfe; /* mov.w @r11+,r14 */
L8c618bfe: sa_motion_publish_stop(ctx,0x8c618bfeu); r[4]=r[4]&65535u;goto L8c618c00; /* extu.w r4,r4 */
L8c618c00: sa_motion_publish_stop(ctx,0x8c618c00u); r[4]<<=2;goto L8c618c02; /* shll2 r4 */
L8c618c02: sa_motion_publish_stop(ctx,0x8c618c02u); r[14]=r[14]&65535u;goto L8c618c04; /* extu.w r14,r14 */
L8c618c04: sa_motion_publish_stop(ctx,0x8c618c04u); r[14]<<=2;goto L8c618c06; /* shll2 r14 */
L8c618c06: sa_motion_publish_stop(ctx,0x8c618c06u); r[14]<<=2;goto L8c618c08; /* shll2 r14 */
L8c618c08: sa_motion_publish_stop(ctx,0x8c618c08u); r[4]<<=2;goto L8c618c0a; /* shll2 r4 */
L8c618c0a: sa_motion_publish_stop(ctx,0x8c618c0au); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c618c0c; /* shll r4 */
L8c618c0c: sa_motion_publish_stop(ctx,0x8c618c0cu); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c618c0e; /* shll r14 */
L8c618c0e: sa_motion_publish_stop(ctx,0x8c618c0eu); *sr=(*sr&~1u)|((int32_t)r[10]>0);goto L8c618c10; /* cmp/pl r10 */
L8c618c10: sa_motion_publish_stop(ctx,0x8c618c10u); r[4]+=r[3];goto L8c618c12; /* add r3,r4 */
L8c618c12: sa_motion_publish_stop(ctx,0x8c618c12u); r[14]+=r[3];goto L8c618c14; /* add r3,r14 */
L8c618c14: sa_motion_publish_stop(ctx,0x8c618c16u); take=!(*sr&1u);r[12]=0u;if(take)goto L8c618c4a;goto L8c618c18; /* bf/s 0x8c618c4a */
L8c618c18: sa_motion_publish_stop(ctx,0x8c618c18u); r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c618c1a; /* mov.w @r11+,r13 */
L8c618c1a: sa_motion_publish_stop(ctx,0x8c618c1au); r[2]=rd(m,bus,&io_status,0x8c618cdcu,4);if(io_status)return io_status;goto L8c618c1c; /* mov.l 0x8c618cdc,r2 */
L8c618c1c: sa_motion_publish_stop(ctx,0x8c618c1cu); r[13]=r[13]&65535u;goto L8c618c1e; /* extu.w r13,r13 */
L8c618c1e: sa_motion_publish_stop(ctx,0x8c618c1eu); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c618c20; /* mov.l @(0xc,r4),r5 */
L8c618c20: sa_motion_publish_stop(ctx,0x8c618c20u); r[13]<<=2;goto L8c618c22; /* shll2 r13 */
L8c618c22: sa_motion_publish_stop(ctx,0x8c618c22u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c618c24; /* mov.l @r2,r3 */
L8c618c24: sa_motion_publish_stop(ctx,0x8c618c24u); r[13]<<=2;goto L8c618c26; /* shll2 r13 */
L8c618c26: sa_motion_publish_stop(ctx,0x8c618c26u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c618c28; /* shll r13 */
L8c618c28: sa_motion_publish_stop(ctx,0x8c618c28u); r[13]+=r[3];goto L8c618c2a; /* add r3,r13 */
L8c618c2a: sa_motion_publish_stop(ctx,0x8c618c2au); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c618c2c; /* mov.l @(0xc,r14),r3 */
L8c618c2c: sa_motion_publish_stop(ctx,0x8c618c2cu); r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c618c2e; /* mov.l @(0xc,r13),r1 */
L8c618c2e: sa_motion_publish_stop(ctx,0x8c618c2eu); r[5]&=r[3];goto L8c618c30; /* and r3,r5 */
L8c618c30: sa_motion_publish_stop(ctx,0x8c618c30u); r[3]=1u;goto L8c618c32; /* mov #0x1,r3 */
L8c618c32: sa_motion_publish_stop(ctx,0x8c618c32u); r[5]&=r[1];goto L8c618c34; /* and r1,r5 */
L8c618c34: sa_motion_publish_stop(ctx,0x8c618c34u); *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c618c36; /* tst r3,r5 */
L8c618c36: sa_motion_publish_stop(ctx,0x8c618c36u); if(!(*sr&1u))goto L8c618c40;goto L8c618c38; /* bf 0x8c618c40 */
L8c618c38: sa_motion_publish_stop(ctx,0x8c618c38u); r[1]=rd(m,bus,&io_status,0x8c618d0cu,4);if(io_status)return io_status;goto L8c618c3a; /* mov.l 0x8c618d0c,r1 */
L8c618c3a: sa_motion_publish_stop(ctx,0x8c618c3au); r[6]=r[13];goto L8c618c3c; /* mov r13,r6 */
L8c618c3c: sa_motion_publish_stop(ctx,0x8c618c3eu); target=r[1];*pr=0x8c618c40u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c618c40; /* jsr @r1 */
L8c618c40: sa_motion_publish_stop(ctx,0x8c618c40u); r[12]+=1u;goto L8c618c42; /* add #0x1,r12 */
L8c618c42: sa_motion_publish_stop(ctx,0x8c618c42u); r[4]=r[14];goto L8c618c44; /* mov r14,r4 */
L8c618c44: sa_motion_publish_stop(ctx,0x8c618c44u); *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[10]);goto L8c618c46; /* cmp/ge r10,r12 */
L8c618c46: sa_motion_publish_stop(ctx,0x8c618c48u); take=!(*sr&1u);r[14]=r[13];if(take)goto L8c618c18;goto L8c618c4a; /* bf/s 0x8c618c18 */
L8c618c4a: sa_motion_publish_stop(ctx,0x8c618c4au); r[9]+=1u;goto L8c618c4c; /* add #0x1,r9 */
L8c618c4c: sa_motion_publish_stop(ctx,0x8c618c4cu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c618c4e; /* mov.l @r15,r2 */
L8c618c4e: sa_motion_publish_stop(ctx,0x8c618c4eu); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c618c50; /* mov.w @(0x2,r2),r0 */
L8c618c50: sa_motion_publish_stop(ctx,0x8c618c50u); r[0]=r[0]&65535u;goto L8c618c52; /* extu.w r0,r0 */
L8c618c52: sa_motion_publish_stop(ctx,0x8c618c52u); *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c618c54; /* cmp/ge r0,r9 */
L8c618c54: sa_motion_publish_stop(ctx,0x8c618c54u); if((*sr&1u))goto L8c618c5a;goto L8c618c56; /* bt 0x8c618c5a */
L8c618c56: sa_motion_publish_stop(ctx,0x8c618c58u); goto L8c618a40; /* bra 0x8c618a40 */
L8c618c5a: sa_motion_publish_stop(ctx,0x8c618c5cu); goto L8c618cb4; /* bra 0x8c618cb4 */
L8c618c5e: sa_motion_publish_stop(ctx,0x8c618c5eu); r[2]=rd(m,bus,&io_status,0x8c618d10u,4);if(io_status)return io_status;goto L8c618c60; /* mov.l 0x8c618d10,r2 */
L8c618c60: sa_motion_publish_stop(ctx,0x8c618c60u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c618c62; /* mov.l @r15,r1 */
L8c618c62: sa_motion_publish_stop(ctx,0x8c618c62u); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c618c64; /* mov.l r1,@r2 */
L8c618c64: sa_motion_publish_stop(ctx,0x8c618c64u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c618c66; /* frchg */
L8c618c66: sa_motion_publish_stop(ctx,0x8c618c66u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c618c68; /* fmov.s fr0,@-r15 */
L8c618c68: sa_motion_publish_stop(ctx,0x8c618c68u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c618c6a; /* fmov.s fr1,@-r15 */
L8c618c6a: sa_motion_publish_stop(ctx,0x8c618c6au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c618c6c; /* fmov.s fr2,@-r15 */
L8c618c6c: sa_motion_publish_stop(ctx,0x8c618c6cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c618c6e; /* fmov.s fr3,@-r15 */
L8c618c6e: sa_motion_publish_stop(ctx,0x8c618c6eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c618c70; /* fmov.s fr4,@-r15 */
L8c618c70: sa_motion_publish_stop(ctx,0x8c618c70u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c618c72; /* fmov.s fr5,@-r15 */
L8c618c72: sa_motion_publish_stop(ctx,0x8c618c72u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c618c74; /* fmov.s fr6,@-r15 */
L8c618c74: sa_motion_publish_stop(ctx,0x8c618c74u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c618c76; /* fmov.s fr7,@-r15 */
L8c618c76: sa_motion_publish_stop(ctx,0x8c618c76u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c618c78; /* fmov.s fr8,@-r15 */
L8c618c78: sa_motion_publish_stop(ctx,0x8c618c78u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c618c7a; /* fmov.s fr9,@-r15 */
L8c618c7a: sa_motion_publish_stop(ctx,0x8c618c7au); r[0]=rd(m,bus,&io_status,0x8c618cc8u,4);if(io_status)return io_status;goto L8c618c7c; /* mov.l 0x8c618cc8,r0 */
L8c618c7c: sa_motion_publish_stop(ctx,0x8c618c7cu); fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618c7e; /* fmov.s @r0+,fr0 */
L8c618c7e: sa_motion_publish_stop(ctx,0x8c618c7eu); fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618c80; /* fmov.s @r0+,fr1 */
L8c618c80: sa_motion_publish_stop(ctx,0x8c618c80u); fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c618c82; /* fmov.s @r0,fr2 */
L8c618c82: sa_motion_publish_stop(ctx,0x8c618c82u); r[0]=rd(m,bus,&io_status,0x8c618cccu,4);if(io_status)return io_status;goto L8c618c84; /* mov.l 0x8c618ccc,r0 */
L8c618c84: sa_motion_publish_stop(ctx,0x8c618c84u); r[0]+=16u;goto L8c618c86; /* add #0x10,r0 */
L8c618c86: sa_motion_publish_stop(ctx,0x8c618c86u); fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618c88; /* fmov.s @r0+,fr4 */
L8c618c88: sa_motion_publish_stop(ctx,0x8c618c88u); fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618c8a; /* fmov.s @r0+,fr5 */
L8c618c8a: sa_motion_publish_stop(ctx,0x8c618c8au); fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618c8c; /* fmov.s @r0+,fr6 */
L8c618c8c: sa_motion_publish_stop(ctx,0x8c618c8cu); fload(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;goto L8c618c8e; /* fmov.s @r0,fr7 */
L8c618c8e: sa_motion_publish_stop(ctx,0x8c618c8eu); r[0]=rd(m,bus,&io_status,0x8c618cd0u,4);if(io_status)return io_status;goto L8c618c90; /* mov.l 0x8c618cd0,r0 */
L8c618c90: sa_motion_publish_stop(ctx,0x8c618c90u); fload(m,bus,&io_status,f,8u,r[0]);if(io_status)return io_status;goto L8c618c92; /* fmov.s @r0,fr8 */
L8c618c92: sa_motion_publish_stop(ctx,0x8c618c92u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c618c94; /* frchg */
L8c618c94: sa_motion_publish_stop(ctx,0x8c618c94u); r[2]=rd(m,bus,&io_status,0x8c618d14u,4);if(io_status)return io_status;goto L8c618c96; /* mov.l 0x8c618d14,r2 */
L8c618c96: sa_motion_publish_stop(ctx,0x8c618c96u); r[3]=rd(m,bus,&io_status,0x8c618d10u,4);if(io_status)return io_status;goto L8c618c98; /* mov.l 0x8c618d10,r3 */
L8c618c98: sa_motion_publish_stop(ctx,0x8c618c9au); target=r[2];*pr=0x8c618c9cu;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c618c9c; /* jsr @r2 */
L8c618c9c: sa_motion_publish_stop(ctx,0x8c618c9cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c618c9e; /* frchg */
L8c618c9e: sa_motion_publish_stop(ctx,0x8c618c9eu); fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618ca0; /* fmov.s @r15+,fr9 */
L8c618ca0: sa_motion_publish_stop(ctx,0x8c618ca0u); fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618ca2; /* fmov.s @r15+,fr8 */
L8c618ca2: sa_motion_publish_stop(ctx,0x8c618ca2u); fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618ca4; /* fmov.s @r15+,fr7 */
L8c618ca4: sa_motion_publish_stop(ctx,0x8c618ca4u); fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618ca6; /* fmov.s @r15+,fr6 */
L8c618ca6: sa_motion_publish_stop(ctx,0x8c618ca6u); fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618ca8; /* fmov.s @r15+,fr5 */
L8c618ca8: sa_motion_publish_stop(ctx,0x8c618ca8u); fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618caa; /* fmov.s @r15+,fr4 */
L8c618caa: sa_motion_publish_stop(ctx,0x8c618caau); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618cac; /* fmov.s @r15+,fr3 */
L8c618cac: sa_motion_publish_stop(ctx,0x8c618cacu); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618cae; /* fmov.s @r15+,fr2 */
L8c618cae: sa_motion_publish_stop(ctx,0x8c618caeu); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618cb0; /* fmov.s @r15+,fr1 */
L8c618cb0: sa_motion_publish_stop(ctx,0x8c618cb0u); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c618cb2; /* fmov.s @r15+,fr0 */
L8c618cb2: sa_motion_publish_stop(ctx,0x8c618cb2u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c618cb4; /* frchg */
L8c618cb4: sa_motion_publish_stop(ctx,0x8c618cb4u); r[15]+=8u;goto L8c618cb6; /* add #0x8,r15 */
L8c618cb6: sa_motion_publish_stop(ctx,0x8c618cb6u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c618cb8; /* lds.l @r15+,PR */
L8c618cb8: sa_motion_publish_stop(ctx,0x8c618cb8u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c618cba; /* mov.l @r15+,r9 */
L8c618cba: sa_motion_publish_stop(ctx,0x8c618cbau); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c618cbc; /* mov.l @r15+,r10 */
L8c618cbc: sa_motion_publish_stop(ctx,0x8c618cbcu); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c618cbe; /* mov.l @r15+,r11 */
L8c618cbe: sa_motion_publish_stop(ctx,0x8c618cbeu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c618cc0; /* mov.l @r15+,r12 */
L8c618cc0: sa_motion_publish_stop(ctx,0x8c618cc0u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c618cc2; /* mov.l @r15+,r13 */
L8c618cc2: sa_motion_publish_stop(ctx,0x8c618cc4u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
