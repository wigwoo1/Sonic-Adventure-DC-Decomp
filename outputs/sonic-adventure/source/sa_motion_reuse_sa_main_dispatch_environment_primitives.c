#include "sa_draw_motion_stop.h"
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
L8c619c34: sa_motion_publish_stop(ctx,0x8c619c34u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c619c36; /* mov.l r14,@-r15 */
L8c619c36: sa_motion_publish_stop(ctx,0x8c619c36u); r[0]=0x8c619c94u;goto L8c619c38; /* mova 0x8c619c94,r0 */
L8c619c38: sa_motion_publish_stop(ctx,0x8c619c38u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c619c3a; /* mov.l r13,@-r15 */
L8c619c3a: sa_motion_publish_stop(ctx,0x8c619c3au); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c619c3c; /* mov.l r12,@-r15 */
L8c619c3c: sa_motion_publish_stop(ctx,0x8c619c3cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c619c3e; /* mov.l r11,@-r15 */
L8c619c3e: sa_motion_publish_stop(ctx,0x8c619c3eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c619c40; /* mov.l r10,@-r15 */
L8c619c40: sa_motion_publish_stop(ctx,0x8c619c40u); r[3]=rd(m,bus,&io_status,0x8c619ca0u,4);if(io_status)return io_status;goto L8c619c42; /* mov.l 0x8c619ca0,r3 */
L8c619c42: sa_motion_publish_stop(ctx,0x8c619c42u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c619c44; /* mov.l r9,@-r15 */
L8c619c44: sa_motion_publish_stop(ctx,0x8c619c44u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c619c46; /* sts.l PR,@-r15 */
L8c619c46: sa_motion_publish_stop(ctx,0x8c619c46u); fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c619c48; /* fmov.s @r0,fr6 */
L8c619c48: sa_motion_publish_stop(ctx,0x8c619c48u); r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c619c4a; /* mov.w @r4,r0 */
L8c619c4a: sa_motion_publish_stop(ctx,0x8c619c4au); r[13]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c619c4c; /* mov.l @(0x4,r4),r13 */
L8c619c4c: sa_motion_publish_stop(ctx,0x8c619c4cu); r[0]=r[0]&65535u;goto L8c619c4e; /* extu.w r0,r0 */
L8c619c4e: sa_motion_publish_stop(ctx,0x8c619c4eu); r[9]=rd(m,bus,&io_status,0x8c619c98u,4);if(io_status)return io_status;goto L8c619c50; /* mov.l 0x8c619c98,r9 */
L8c619c50: sa_motion_publish_stop(ctx,0x8c619c50u); r[0]&=r[3];goto L8c619c52; /* and r3,r0 */
L8c619c52: sa_motion_publish_stop(ctx,0x8c619c52u); r[11]=rd(m,bus,&io_status,0x8c619c9cu,4);if(io_status)return io_status;goto L8c619c54; /* mov.l 0x8c619c9c,r11 */
L8c619c54: sa_motion_publish_stop(ctx,0x8c619c54u); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c619c56; /* cmp/eq #0x0,r0 */
L8c619c56: sa_motion_publish_stop(ctx,0x8c619c58u); take=(*sr&1u);r[12]=0u;if(take)goto L8c619c7c;goto L8c619c5a; /* bt/s 0x8c619c7c */
L8c619c5a: sa_motion_publish_stop(ctx,0x8c619c5au); r[1]=sx(m,bus,&io_status,0x8c619c90u,2);if(io_status)return io_status;goto L8c619c5c; /* mov.w 0x8c619c90,r1 */
L8c619c5c: sa_motion_publish_stop(ctx,0x8c619c5cu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c619c5e; /* cmp/eq r1,r0 */
L8c619c5e: sa_motion_publish_stop(ctx,0x8c619c5eu); if(!(*sr&1u))goto L8c619c64;goto L8c619c60; /* bf 0x8c619c64 */
L8c619c60: sa_motion_publish_stop(ctx,0x8c619c62u); goto L8c619ed8; /* bra 0x8c619ed8 */
L8c619c64: sa_motion_publish_stop(ctx,0x8c619c64u); r[1]=rd(m,bus,&io_status,0x8c619ca4u,4);if(io_status)return io_status;goto L8c619c66; /* mov.l 0x8c619ca4,r1 */
L8c619c66: sa_motion_publish_stop(ctx,0x8c619c66u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c619c68; /* cmp/eq r1,r0 */
L8c619c68: sa_motion_publish_stop(ctx,0x8c619c68u); if(!(*sr&1u))goto L8c619c6e;goto L8c619c6a; /* bf 0x8c619c6e */
L8c619c6a: sa_motion_publish_stop(ctx,0x8c619c6cu); goto L8c61a1d4; /* bra 0x8c61a1d4 */
L8c619c6e: sa_motion_publish_stop(ctx,0x8c619c6eu); r[1]=rd(m,bus,&io_status,0x8c619ca0u,4);if(io_status)return io_status;goto L8c619c70; /* mov.l 0x8c619ca0,r1 */
L8c619c70: sa_motion_publish_stop(ctx,0x8c619c70u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c619c72; /* cmp/eq r1,r0 */
L8c619c72: sa_motion_publish_stop(ctx,0x8c619c72u); if(!(*sr&1u))goto L8c619c78;goto L8c619c74; /* bf 0x8c619c78 */
L8c619c74: sa_motion_publish_stop(ctx,0x8c619c76u); goto L8c61a3ce; /* bra 0x8c61a3ce */
L8c619c78: sa_motion_publish_stop(ctx,0x8c619c7au); goto L8c61a446; /* bra 0x8c61a446 */
L8c619c7c: sa_motion_publish_stop(ctx,0x8c619c7cu); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c619c7e; /* mov.w @(0x2,r4),r0 */
L8c619c7e: sa_motion_publish_stop(ctx,0x8c619c7eu); r[1]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c619c80; /* mov.l @r11,r1 */
L8c619c80: sa_motion_publish_stop(ctx,0x8c619c80u); r[0]=r[0]&65535u;goto L8c619c82; /* extu.w r0,r0 */
L8c619c82: sa_motion_publish_stop(ctx,0x8c619c82u); r[2]=r[0];goto L8c619c84; /* mov r0,r2 */
L8c619c84: sa_motion_publish_stop(ctx,0x8c619c84u); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c619c86; /* shll r0 */
L8c619c86: sa_motion_publish_stop(ctx,0x8c619c86u); r[0]+=r[2];goto L8c619c88; /* add r2,r0 */
L8c619c88: sa_motion_publish_stop(ctx,0x8c619c88u); r[1]+=r[0];goto L8c619c8a; /* add r0,r1 */
L8c619c8a: sa_motion_publish_stop(ctx,0x8c619c8au); wr(m,bus,&io_status,r[11],r[1],4);if(io_status)return io_status;goto L8c619c8c; /* mov.l r1,@r11 */
L8c619c8c: sa_motion_publish_stop(ctx,0x8c619c8eu); r[14]=r[12];goto L8c619ec8; /* bra 0x8c619ec8 */
L8c619ca8: sa_motion_publish_stop(ctx,0x8c619ca8u); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c619caa; /* mov.w @r13+,r5 */
L8c619caa: sa_motion_publish_stop(ctx,0x8c619caau); r[0]=16u;goto L8c619cac; /* mov #0x10,r0 */
L8c619cac: sa_motion_publish_stop(ctx,0x8c619cacu); r[3]=rd(m,bus,&io_status,0x8c619ee8u,4);if(io_status)return io_status;goto L8c619cae; /* mov.l 0x8c619ee8,r3 */
L8c619cae: sa_motion_publish_stop(ctx,0x8c619caeu); r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c619cb0; /* mov.w @r13+,r6 */
L8c619cb0: sa_motion_publish_stop(ctx,0x8c619cb0u); r[5]=r[5]&65535u;goto L8c619cb2; /* extu.w r5,r5 */
L8c619cb2: sa_motion_publish_stop(ctx,0x8c619cb2u); r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c619cb4; /* mov.l @r3,r12 */
L8c619cb4: sa_motion_publish_stop(ctx,0x8c619cb4u); r[5]<<=2;goto L8c619cb6; /* shll2 r5 */
L8c619cb6: sa_motion_publish_stop(ctx,0x8c619cb6u); r[6]=r[6]&65535u;goto L8c619cb8; /* extu.w r6,r6 */
L8c619cb8: sa_motion_publish_stop(ctx,0x8c619cb8u); r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c619cba; /* mov.w @r13+,r7 */
L8c619cba: sa_motion_publish_stop(ctx,0x8c619cbau); r[6]<<=2;goto L8c619cbc; /* shll2 r6 */
L8c619cbc: sa_motion_publish_stop(ctx,0x8c619cbcu); r[6]<<=2;goto L8c619cbe; /* shll2 r6 */
L8c619cbe: sa_motion_publish_stop(ctx,0x8c619cbeu); r[5]<<=2;goto L8c619cc0; /* shll2 r5 */
L8c619cc0: sa_motion_publish_stop(ctx,0x8c619cc0u); r[7]=r[7]&65535u;goto L8c619cc2; /* extu.w r7,r7 */
L8c619cc2: sa_motion_publish_stop(ctx,0x8c619cc2u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c619cc4; /* shll r5 */
L8c619cc4: sa_motion_publish_stop(ctx,0x8c619cc4u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c619cc6; /* shll r6 */
L8c619cc6: sa_motion_publish_stop(ctx,0x8c619cc6u); r[6]+=r[12];goto L8c619cc8; /* add r12,r6 */
L8c619cc8: sa_motion_publish_stop(ctx,0x8c619cc8u); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c619cca; /* fmov.s @(r0,r6),fr4 */
L8c619cca: sa_motion_publish_stop(ctx,0x8c619ccau); r[5]+=r[12];goto L8c619ccc; /* add r12,r5 */
L8c619ccc: sa_motion_publish_stop(ctx,0x8c619cccu); r[7]<<=2;goto L8c619cce; /* shll2 r7 */
L8c619cce: sa_motion_publish_stop(ctx,0x8c619cceu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619cd0; /* fmov.s @(r0,r5),fr3 */
L8c619cd0: sa_motion_publish_stop(ctx,0x8c619cd0u); r[7]<<=2;goto L8c619cd2; /* shll2 r7 */
L8c619cd2: sa_motion_publish_stop(ctx,0x8c619cd2u); fcopy(f,2u,4u);goto L8c619cd4; /* fmov fr4,fr2 */
L8c619cd4: sa_motion_publish_stop(ctx,0x8c619cd4u); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c619cd6; /* shll r7 */
L8c619cd6: sa_motion_publish_stop(ctx,0x8c619cd6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c619cd8; /* fsub fr3,fr2 */
L8c619cd8: sa_motion_publish_stop(ctx,0x8c619cd8u); r[0]=20u;goto L8c619cda; /* mov #0x14,r0 */
L8c619cda: sa_motion_publish_stop(ctx,0x8c619cdau); r[7]+=r[12];goto L8c619cdc; /* add r12,r7 */
L8c619cdc: sa_motion_publish_stop(ctx,0x8c619cdcu); fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c619cde; /* fmov.s @(r0,r6),fr5 */
L8c619cde: sa_motion_publish_stop(ctx,0x8c619cdeu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619ce0; /* fmov.s @(r0,r7),fr3 */
L8c619ce0: sa_motion_publish_stop(ctx,0x8c619ce0u); fcopy(f,1u,5u);goto L8c619ce2; /* fmov fr5,fr1 */
L8c619ce2: sa_motion_publish_stop(ctx,0x8c619ce2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619ce4; /* fsub fr5,fr3 */
L8c619ce4: sa_motion_publish_stop(ctx,0x8c619ce4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619ce6; /* fmul fr3,fr2 */
L8c619ce6: sa_motion_publish_stop(ctx,0x8c619ce6u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619ce8; /* fmov.s @(r0,r5),fr3 */
L8c619ce8: sa_motion_publish_stop(ctx,0x8c619ce8u); r[0]=16u;goto L8c619cea; /* mov #0x10,r0 */
L8c619cea: sa_motion_publish_stop(ctx,0x8c619ceau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c619cec; /* fsub fr3,fr1 */
L8c619cec: sa_motion_publish_stop(ctx,0x8c619cecu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619cee; /* fmov.s @(r0,r7),fr3 */
L8c619cee: sa_motion_publish_stop(ctx,0x8c619ceeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619cf0; /* fsub fr4,fr3 */
L8c619cf0: sa_motion_publish_stop(ctx,0x8c619cf0u); fcopy(f,4u,2u);goto L8c619cf2; /* fmov fr2,fr4 */
L8c619cf2: sa_motion_publish_stop(ctx,0x8c619cf2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619cf4; /* fmul fr3,fr1 */
L8c619cf4: sa_motion_publish_stop(ctx,0x8c619cf4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c619cf6; /* fldi0 fr3 */
L8c619cf6: sa_motion_publish_stop(ctx,0x8c619cf6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c619cf8; /* fsub fr1,fr4 */
L8c619cf8: sa_motion_publish_stop(ctx,0x8c619cf8u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c619cfa; /* fcmp/gt fr3,fr4 */
L8c619cfa: sa_motion_publish_stop(ctx,0x8c619cfcu); take=!(*sr&1u);r[14]+=1u;if(take)goto L8c619d02;goto L8c619cfe; /* bf/s 0x8c619d02 */
L8c619cfe: sa_motion_publish_stop(ctx,0x8c619d00u); goto L8c619ec8; /* bra 0x8c619ec8 */
L8c619d02: sa_motion_publish_stop(ctx,0x8c619d02u); r[3]=rd(m,bus,&io_status,0x8c619eecu,4);if(io_status)return io_status;goto L8c619d04; /* mov.l 0x8c619eec,r3 */
L8c619d04: sa_motion_publish_stop(ctx,0x8c619d04u); r[0]=4u;goto L8c619d06; /* mov #0x4,r0 */
L8c619d06: sa_motion_publish_stop(ctx,0x8c619d06u); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c619d08; /* fmov.s @r5,fr3 */
L8c619d08: sa_motion_publish_stop(ctx,0x8c619d08u); r[8]+=28u;goto L8c619d0a; /* add #0x1c,r8 */
L8c619d0a: sa_motion_publish_stop(ctx,0x8c619d0au); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c619d0c; /* fmov.s @r3,fr2 */
L8c619d0c: sa_motion_publish_stop(ctx,0x8c619d0cu); r[3]+=8u;goto L8c619d0e; /* add #0x8,r3 */
L8c619d0e: sa_motion_publish_stop(ctx,0x8c619d0eu); r[2]=rd(m,bus,&io_status,0x8c619ef0u,4);if(io_status)return io_status;goto L8c619d10; /* mov.l 0x8c619ef0,r2 */
L8c619d10: sa_motion_publish_stop(ctx,0x8c619d10u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619d12; /* fmul fr3,fr2 */
L8c619d12: sa_motion_publish_stop(ctx,0x8c619d12u); r[1]=rd(m,bus,&io_status,0x8c619ef4u,4);if(io_status)return io_status;goto L8c619d14; /* mov.l 0x8c619ef4,r1 */
L8c619d14: sa_motion_publish_stop(ctx,0x8c619d14u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c619d16; /* fmov.s @r2,fr3 */
L8c619d16: sa_motion_publish_stop(ctx,0x8c619d16u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c619d18; /* fmov.s @r1,fr1 */
L8c619d18: sa_motion_publish_stop(ctx,0x8c619d18u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619d1a; /* fsub fr2,fr3 */
L8c619d1a: sa_motion_publish_stop(ctx,0x8c619d1au); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d1c; /* fmov.s @(r0,r5),fr2 */
L8c619d1c: sa_motion_publish_stop(ctx,0x8c619d1cu); r[0]=8u;goto L8c619d1e; /* mov #0x8,r0 */
L8c619d1e: sa_motion_publish_stop(ctx,0x8c619d1eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619d20; /* fmul fr2,fr1 */
L8c619d20: sa_motion_publish_stop(ctx,0x8c619d20u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d22; /* fmov.s @(r0,r5),fr2 */
L8c619d22: sa_motion_publish_stop(ctx,0x8c619d22u); r[0]=4u;goto L8c619d24; /* mov #0x4,r0 */
L8c619d24: sa_motion_publish_stop(ctx,0x8c619d24u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619d26; /* fsub fr1,fr3 */
L8c619d26: sa_motion_publish_stop(ctx,0x8c619d26u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619d28; /* fmov.s @r3,fr1 */
L8c619d28: sa_motion_publish_stop(ctx,0x8c619d28u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619d2a; /* fmul fr2,fr1 */
L8c619d2a: sa_motion_publish_stop(ctx,0x8c619d2au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619d2c; /* fsub fr1,fr3 */
L8c619d2c: sa_motion_publish_stop(ctx,0x8c619d2cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619d2e; /* fmov.s fr3,@-r8 */
L8c619d2e: sa_motion_publish_stop(ctx,0x8c619d2eu); r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619d30; /* mov.l @r9,r2 */
L8c619d30: sa_motion_publish_stop(ctx,0x8c619d30u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d32; /* fmov.s @(r0,r5),fr0 */
L8c619d32: sa_motion_publish_stop(ctx,0x8c619d32u); r[0]=20u;goto L8c619d34; /* mov #0x14,r0 */
L8c619d34: sa_motion_publish_stop(ctx,0x8c619d34u); r[1]=r[2];goto L8c619d36; /* mov r2,r1 */
L8c619d36: sa_motion_publish_stop(ctx,0x8c619d36u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619d38; /* fmov.s @(r0,r1),fr3 */
L8c619d38: sa_motion_publish_stop(ctx,0x8c619d38u); r[0]=4u;goto L8c619d3a; /* mov #0x4,r0 */
L8c619d3a: sa_motion_publish_stop(ctx,0x8c619d3au); r[3]=r[2];goto L8c619d3c; /* mov r2,r3 */
L8c619d3c: sa_motion_publish_stop(ctx,0x8c619d3cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619d3e; /* fmul fr0,fr3 */
L8c619d3e: sa_motion_publish_stop(ctx,0x8c619d3eu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619d40; /* fmov.s @(r0,r3),fr2 */
L8c619d40: sa_motion_publish_stop(ctx,0x8c619d40u); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c619d42; /* fmov.s @r5,fr0 */
L8c619d42: sa_motion_publish_stop(ctx,0x8c619d42u); r[0]=8u;goto L8c619d44; /* mov #0x8,r0 */
L8c619d44: sa_motion_publish_stop(ctx,0x8c619d44u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619d46; /* fmac fr0,fr2,fr3 */
L8c619d46: sa_motion_publish_stop(ctx,0x8c619d46u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d48; /* fmov.s @(r0,r5),fr0 */
L8c619d48: sa_motion_publish_stop(ctx,0x8c619d48u); r[0]=36u;goto L8c619d4a; /* mov #0x24,r0 */
L8c619d4a: sa_motion_publish_stop(ctx,0x8c619d4au); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c619d4c; /* fmov.s @(r0,r2),fr2 */
L8c619d4c: sa_motion_publish_stop(ctx,0x8c619d4cu); r[0]=4u;goto L8c619d4e; /* mov #0x4,r0 */
L8c619d4e: sa_motion_publish_stop(ctx,0x8c619d4eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619d50; /* fmac fr0,fr2,fr3 */
L8c619d50: sa_motion_publish_stop(ctx,0x8c619d50u); fcopy(f,0u,6u);goto L8c619d52; /* fmov fr6,fr0 */
L8c619d52: sa_motion_publish_stop(ctx,0x8c619d52u); fcopy(f,2u,6u);goto L8c619d54; /* fmov fr6,fr2 */
L8c619d54: sa_motion_publish_stop(ctx,0x8c619d54u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619d56; /* fmac fr0,fr3,fr2 */
L8c619d56: sa_motion_publish_stop(ctx,0x8c619d56u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619d58; /* fmov.s fr2,@-r8 */
L8c619d58: sa_motion_publish_stop(ctx,0x8c619d58u); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619d5a; /* mov.l @r9,r3 */
L8c619d5a: sa_motion_publish_stop(ctx,0x8c619d5au); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d5c; /* fmov.s @(r0,r5),fr0 */
L8c619d5c: sa_motion_publish_stop(ctx,0x8c619d5cu); r[0]=16u;goto L8c619d5e; /* mov #0x10,r0 */
L8c619d5e: sa_motion_publish_stop(ctx,0x8c619d5eu); r[1]=r[3];goto L8c619d60; /* mov r3,r1 */
L8c619d60: sa_motion_publish_stop(ctx,0x8c619d60u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619d62; /* fmov.s @(r0,r1),fr3 */
L8c619d62: sa_motion_publish_stop(ctx,0x8c619d62u); r[2]=r[3];goto L8c619d64; /* mov r3,r2 */
L8c619d64: sa_motion_publish_stop(ctx,0x8c619d64u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c619d66; /* fmov.s @r2,fr2 */
L8c619d66: sa_motion_publish_stop(ctx,0x8c619d66u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619d68; /* fmul fr0,fr3 */
L8c619d68: sa_motion_publish_stop(ctx,0x8c619d68u); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c619d6a; /* fmov.s @r5,fr0 */
L8c619d6a: sa_motion_publish_stop(ctx,0x8c619d6au); r[0]=8u;goto L8c619d6c; /* mov #0x8,r0 */
L8c619d6c: sa_motion_publish_stop(ctx,0x8c619d6cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619d6e; /* fmac fr0,fr2,fr3 */
L8c619d6e: sa_motion_publish_stop(ctx,0x8c619d6eu); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d70; /* fmov.s @(r0,r5),fr0 */
L8c619d70: sa_motion_publish_stop(ctx,0x8c619d70u); r[0]=32u;goto L8c619d72; /* mov #0x20,r0 */
L8c619d72: sa_motion_publish_stop(ctx,0x8c619d72u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619d74; /* fmov.s @(r0,r3),fr2 */
L8c619d74: sa_motion_publish_stop(ctx,0x8c619d74u); r[0]=24u;goto L8c619d76; /* mov #0x18,r0 */
L8c619d76: sa_motion_publish_stop(ctx,0x8c619d76u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619d78; /* fmac fr0,fr2,fr3 */
L8c619d78: sa_motion_publish_stop(ctx,0x8c619d78u); fcopy(f,0u,6u);goto L8c619d7a; /* fmov fr6,fr0 */
L8c619d7a: sa_motion_publish_stop(ctx,0x8c619d7au); fcopy(f,2u,6u);goto L8c619d7c; /* fmov fr6,fr2 */
L8c619d7c: sa_motion_publish_stop(ctx,0x8c619d7cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619d7e; /* fmac fr0,fr3,fr2 */
L8c619d7e: sa_motion_publish_stop(ctx,0x8c619d7eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619d80; /* fmov.s fr2,@-r8 */
L8c619d80: sa_motion_publish_stop(ctx,0x8c619d80u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d82; /* fmov.s @(r0,r5),fr3 */
L8c619d82: sa_motion_publish_stop(ctx,0x8c619d82u); r[0]=20u;goto L8c619d84; /* mov #0x14,r0 */
L8c619d84: sa_motion_publish_stop(ctx,0x8c619d84u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619d86; /* fmov.s fr3,@-r8 */
L8c619d86: sa_motion_publish_stop(ctx,0x8c619d86u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d88; /* fmov.s @(r0,r5),fr3 */
L8c619d88: sa_motion_publish_stop(ctx,0x8c619d88u); r[0]=16u;goto L8c619d8a; /* mov #0x10,r0 */
L8c619d8a: sa_motion_publish_stop(ctx,0x8c619d8au); r[3]=rd(m,bus,&io_status,0x8c619ef8u,4);if(io_status)return io_status;goto L8c619d8c; /* mov.l 0x8c619ef8,r3 */
L8c619d8c: sa_motion_publish_stop(ctx,0x8c619d8cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619d8e; /* fmov.s fr3,@-r8 */
L8c619d8e: sa_motion_publish_stop(ctx,0x8c619d8eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c619d90; /* fmov.s @(r0,r5),fr3 */
L8c619d90: sa_motion_publish_stop(ctx,0x8c619d90u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619d92; /* fmov.s fr3,@-r8 */
L8c619d92: sa_motion_publish_stop(ctx,0x8c619d92u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c619d94; /* mov.l r3,@-r8 */
L8c619d94: sa_motion_publish_stop(ctx,0x8c619d94u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619d96; /* pref @r8 */
L8c619d96: sa_motion_publish_stop(ctx,0x8c619d96u); r[3]=rd(m,bus,&io_status,0x8c619eecu,4);if(io_status)return io_status;goto L8c619d98; /* mov.l 0x8c619eec,r3 */
L8c619d98: sa_motion_publish_stop(ctx,0x8c619d98u); r[0]=4u;goto L8c619d9a; /* mov #0x4,r0 */
L8c619d9a: sa_motion_publish_stop(ctx,0x8c619d9au); fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c619d9c; /* fmov.s @r6,fr3 */
L8c619d9c: sa_motion_publish_stop(ctx,0x8c619d9cu); r[8]+=60u;goto L8c619d9e; /* add #0x3c,r8 */
L8c619d9e: sa_motion_publish_stop(ctx,0x8c619d9eu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c619da0; /* fmov.s @r3,fr2 */
L8c619da0: sa_motion_publish_stop(ctx,0x8c619da0u); r[3]+=8u;goto L8c619da2; /* add #0x8,r3 */
L8c619da2: sa_motion_publish_stop(ctx,0x8c619da2u); r[2]=rd(m,bus,&io_status,0x8c619ef0u,4);if(io_status)return io_status;goto L8c619da4; /* mov.l 0x8c619ef0,r2 */
L8c619da4: sa_motion_publish_stop(ctx,0x8c619da4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619da6; /* fmul fr3,fr2 */
L8c619da6: sa_motion_publish_stop(ctx,0x8c619da6u); r[1]=rd(m,bus,&io_status,0x8c619ef4u,4);if(io_status)return io_status;goto L8c619da8; /* mov.l 0x8c619ef4,r1 */
L8c619da8: sa_motion_publish_stop(ctx,0x8c619da8u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c619daa; /* fmov.s @r2,fr3 */
L8c619daa: sa_motion_publish_stop(ctx,0x8c619daau); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c619dac; /* fmov.s @r1,fr1 */
L8c619dac: sa_motion_publish_stop(ctx,0x8c619dacu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619dae; /* fsub fr2,fr3 */
L8c619dae: sa_motion_publish_stop(ctx,0x8c619daeu); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c619db0; /* fmov.s @(r0,r6),fr2 */
L8c619db0: sa_motion_publish_stop(ctx,0x8c619db0u); r[0]=8u;goto L8c619db2; /* mov #0x8,r0 */
L8c619db2: sa_motion_publish_stop(ctx,0x8c619db2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619db4; /* fmul fr2,fr1 */
L8c619db4: sa_motion_publish_stop(ctx,0x8c619db4u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c619db6; /* fmov.s @(r0,r6),fr2 */
L8c619db6: sa_motion_publish_stop(ctx,0x8c619db6u); r[0]=4u;goto L8c619db8; /* mov #0x4,r0 */
L8c619db8: sa_motion_publish_stop(ctx,0x8c619db8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619dba; /* fsub fr1,fr3 */
L8c619dba: sa_motion_publish_stop(ctx,0x8c619dbau); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619dbc; /* fmov.s @r3,fr1 */
L8c619dbc: sa_motion_publish_stop(ctx,0x8c619dbcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619dbe; /* fmul fr2,fr1 */
L8c619dbe: sa_motion_publish_stop(ctx,0x8c619dbeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619dc0; /* fsub fr1,fr3 */
L8c619dc0: sa_motion_publish_stop(ctx,0x8c619dc0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619dc2; /* fmov.s fr3,@-r8 */
L8c619dc2: sa_motion_publish_stop(ctx,0x8c619dc2u); r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619dc4; /* mov.l @r9,r2 */
L8c619dc4: sa_motion_publish_stop(ctx,0x8c619dc4u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619dc6; /* fmov.s @(r0,r6),fr0 */
L8c619dc6: sa_motion_publish_stop(ctx,0x8c619dc6u); r[0]=20u;goto L8c619dc8; /* mov #0x14,r0 */
L8c619dc8: sa_motion_publish_stop(ctx,0x8c619dc8u); r[1]=r[2];goto L8c619dca; /* mov r2,r1 */
L8c619dca: sa_motion_publish_stop(ctx,0x8c619dcau); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619dcc; /* fmov.s @(r0,r1),fr3 */
L8c619dcc: sa_motion_publish_stop(ctx,0x8c619dccu); r[0]=4u;goto L8c619dce; /* mov #0x4,r0 */
L8c619dce: sa_motion_publish_stop(ctx,0x8c619dceu); r[3]=r[2];goto L8c619dd0; /* mov r2,r3 */
L8c619dd0: sa_motion_publish_stop(ctx,0x8c619dd0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619dd2; /* fmul fr0,fr3 */
L8c619dd2: sa_motion_publish_stop(ctx,0x8c619dd2u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619dd4; /* fmov.s @(r0,r3),fr2 */
L8c619dd4: sa_motion_publish_stop(ctx,0x8c619dd4u); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c619dd6; /* fmov.s @r6,fr0 */
L8c619dd6: sa_motion_publish_stop(ctx,0x8c619dd6u); r[0]=8u;goto L8c619dd8; /* mov #0x8,r0 */
L8c619dd8: sa_motion_publish_stop(ctx,0x8c619dd8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619dda; /* fmac fr0,fr2,fr3 */
L8c619dda: sa_motion_publish_stop(ctx,0x8c619ddau); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619ddc; /* fmov.s @(r0,r6),fr0 */
L8c619ddc: sa_motion_publish_stop(ctx,0x8c619ddcu); r[0]=36u;goto L8c619dde; /* mov #0x24,r0 */
L8c619dde: sa_motion_publish_stop(ctx,0x8c619ddeu); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c619de0; /* fmov.s @(r0,r2),fr2 */
L8c619de0: sa_motion_publish_stop(ctx,0x8c619de0u); r[0]=4u;goto L8c619de2; /* mov #0x4,r0 */
L8c619de2: sa_motion_publish_stop(ctx,0x8c619de2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619de4; /* fmac fr0,fr2,fr3 */
L8c619de4: sa_motion_publish_stop(ctx,0x8c619de4u); fcopy(f,0u,6u);goto L8c619de6; /* fmov fr6,fr0 */
L8c619de6: sa_motion_publish_stop(ctx,0x8c619de6u); fcopy(f,2u,6u);goto L8c619de8; /* fmov fr6,fr2 */
L8c619de8: sa_motion_publish_stop(ctx,0x8c619de8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619dea; /* fmac fr0,fr3,fr2 */
L8c619dea: sa_motion_publish_stop(ctx,0x8c619deau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619dec; /* fmov.s fr2,@-r8 */
L8c619dec: sa_motion_publish_stop(ctx,0x8c619decu); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619dee; /* mov.l @r9,r3 */
L8c619dee: sa_motion_publish_stop(ctx,0x8c619deeu); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619df0; /* fmov.s @(r0,r6),fr0 */
L8c619df0: sa_motion_publish_stop(ctx,0x8c619df0u); r[0]=16u;goto L8c619df2; /* mov #0x10,r0 */
L8c619df2: sa_motion_publish_stop(ctx,0x8c619df2u); r[1]=r[3];goto L8c619df4; /* mov r3,r1 */
L8c619df4: sa_motion_publish_stop(ctx,0x8c619df4u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619df6; /* fmov.s @(r0,r1),fr3 */
L8c619df6: sa_motion_publish_stop(ctx,0x8c619df6u); r[2]=r[3];goto L8c619df8; /* mov r3,r2 */
L8c619df8: sa_motion_publish_stop(ctx,0x8c619df8u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c619dfa; /* fmov.s @r2,fr2 */
L8c619dfa: sa_motion_publish_stop(ctx,0x8c619dfau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619dfc; /* fmul fr0,fr3 */
L8c619dfc: sa_motion_publish_stop(ctx,0x8c619dfcu); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c619dfe; /* fmov.s @r6,fr0 */
L8c619dfe: sa_motion_publish_stop(ctx,0x8c619dfeu); r[0]=8u;goto L8c619e00; /* mov #0x8,r0 */
L8c619e00: sa_motion_publish_stop(ctx,0x8c619e00u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619e02; /* fmac fr0,fr2,fr3 */
L8c619e02: sa_motion_publish_stop(ctx,0x8c619e02u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619e04; /* fmov.s @(r0,r6),fr0 */
L8c619e04: sa_motion_publish_stop(ctx,0x8c619e04u); r[0]=32u;goto L8c619e06; /* mov #0x20,r0 */
L8c619e06: sa_motion_publish_stop(ctx,0x8c619e06u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619e08; /* fmov.s @(r0,r3),fr2 */
L8c619e08: sa_motion_publish_stop(ctx,0x8c619e08u); r[0]=24u;goto L8c619e0a; /* mov #0x18,r0 */
L8c619e0a: sa_motion_publish_stop(ctx,0x8c619e0au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619e0c; /* fmac fr0,fr2,fr3 */
L8c619e0c: sa_motion_publish_stop(ctx,0x8c619e0cu); fcopy(f,0u,6u);goto L8c619e0e; /* fmov fr6,fr0 */
L8c619e0e: sa_motion_publish_stop(ctx,0x8c619e0eu); fcopy(f,2u,6u);goto L8c619e10; /* fmov fr6,fr2 */
L8c619e10: sa_motion_publish_stop(ctx,0x8c619e10u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619e12; /* fmac fr0,fr3,fr2 */
L8c619e12: sa_motion_publish_stop(ctx,0x8c619e12u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619e14; /* fmov.s fr2,@-r8 */
L8c619e14: sa_motion_publish_stop(ctx,0x8c619e14u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619e16; /* fmov.s @(r0,r6),fr3 */
L8c619e16: sa_motion_publish_stop(ctx,0x8c619e16u); r[0]=20u;goto L8c619e18; /* mov #0x14,r0 */
L8c619e18: sa_motion_publish_stop(ctx,0x8c619e18u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619e1a; /* fmov.s fr3,@-r8 */
L8c619e1a: sa_motion_publish_stop(ctx,0x8c619e1au); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619e1c; /* fmov.s @(r0,r6),fr3 */
L8c619e1c: sa_motion_publish_stop(ctx,0x8c619e1cu); r[0]=16u;goto L8c619e1e; /* mov #0x10,r0 */
L8c619e1e: sa_motion_publish_stop(ctx,0x8c619e1eu); r[3]=rd(m,bus,&io_status,0x8c619ef8u,4);if(io_status)return io_status;goto L8c619e20; /* mov.l 0x8c619ef8,r3 */
L8c619e20: sa_motion_publish_stop(ctx,0x8c619e20u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619e22; /* fmov.s fr3,@-r8 */
L8c619e22: sa_motion_publish_stop(ctx,0x8c619e22u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619e24; /* fmov.s @(r0,r6),fr3 */
L8c619e24: sa_motion_publish_stop(ctx,0x8c619e24u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619e26; /* fmov.s fr3,@-r8 */
L8c619e26: sa_motion_publish_stop(ctx,0x8c619e26u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c619e28; /* mov.l r3,@-r8 */
L8c619e28: sa_motion_publish_stop(ctx,0x8c619e28u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619e2a; /* pref @r8 */
L8c619e2a: sa_motion_publish_stop(ctx,0x8c619e2au); r[3]=rd(m,bus,&io_status,0x8c619eecu,4);if(io_status)return io_status;goto L8c619e2c; /* mov.l 0x8c619eec,r3 */
L8c619e2c: sa_motion_publish_stop(ctx,0x8c619e2cu); r[0]=4u;goto L8c619e2e; /* mov #0x4,r0 */
L8c619e2e: sa_motion_publish_stop(ctx,0x8c619e2eu); fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c619e30; /* fmov.s @r7,fr3 */
L8c619e30: sa_motion_publish_stop(ctx,0x8c619e30u); r[8]+=60u;goto L8c619e32; /* add #0x3c,r8 */
L8c619e32: sa_motion_publish_stop(ctx,0x8c619e32u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c619e34; /* fmov.s @r3,fr2 */
L8c619e34: sa_motion_publish_stop(ctx,0x8c619e34u); r[3]+=8u;goto L8c619e36; /* add #0x8,r3 */
L8c619e36: sa_motion_publish_stop(ctx,0x8c619e36u); r[2]=rd(m,bus,&io_status,0x8c619ef0u,4);if(io_status)return io_status;goto L8c619e38; /* mov.l 0x8c619ef0,r2 */
L8c619e38: sa_motion_publish_stop(ctx,0x8c619e38u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619e3a; /* fmul fr3,fr2 */
L8c619e3a: sa_motion_publish_stop(ctx,0x8c619e3au); r[1]=rd(m,bus,&io_status,0x8c619ef4u,4);if(io_status)return io_status;goto L8c619e3c; /* mov.l 0x8c619ef4,r1 */
L8c619e3c: sa_motion_publish_stop(ctx,0x8c619e3cu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c619e3e; /* fmov.s @r2,fr3 */
L8c619e3e: sa_motion_publish_stop(ctx,0x8c619e3eu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c619e40; /* fmov.s @r1,fr1 */
L8c619e40: sa_motion_publish_stop(ctx,0x8c619e40u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619e42; /* fsub fr2,fr3 */
L8c619e42: sa_motion_publish_stop(ctx,0x8c619e42u); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c619e44; /* fmov.s @(r0,r7),fr2 */
L8c619e44: sa_motion_publish_stop(ctx,0x8c619e44u); r[0]=8u;goto L8c619e46; /* mov #0x8,r0 */
L8c619e46: sa_motion_publish_stop(ctx,0x8c619e46u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619e48; /* fmul fr2,fr1 */
L8c619e48: sa_motion_publish_stop(ctx,0x8c619e48u); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c619e4a; /* fmov.s @(r0,r7),fr2 */
L8c619e4a: sa_motion_publish_stop(ctx,0x8c619e4au); r[0]=4u;goto L8c619e4c; /* mov #0x4,r0 */
L8c619e4c: sa_motion_publish_stop(ctx,0x8c619e4cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619e4e; /* fsub fr1,fr3 */
L8c619e4e: sa_motion_publish_stop(ctx,0x8c619e4eu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619e50; /* fmov.s @r3,fr1 */
L8c619e50: sa_motion_publish_stop(ctx,0x8c619e50u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619e52; /* fmul fr2,fr1 */
L8c619e52: sa_motion_publish_stop(ctx,0x8c619e52u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619e54; /* fsub fr1,fr3 */
L8c619e54: sa_motion_publish_stop(ctx,0x8c619e54u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619e56; /* fmov.s fr3,@-r8 */
L8c619e56: sa_motion_publish_stop(ctx,0x8c619e56u); r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619e58; /* mov.l @r9,r2 */
L8c619e58: sa_motion_publish_stop(ctx,0x8c619e58u); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c619e5a; /* fmov.s @(r0,r7),fr0 */
L8c619e5a: sa_motion_publish_stop(ctx,0x8c619e5au); r[0]=20u;goto L8c619e5c; /* mov #0x14,r0 */
L8c619e5c: sa_motion_publish_stop(ctx,0x8c619e5cu); r[1]=r[2];goto L8c619e5e; /* mov r2,r1 */
L8c619e5e: sa_motion_publish_stop(ctx,0x8c619e5eu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619e60; /* fmov.s @(r0,r1),fr3 */
L8c619e60: sa_motion_publish_stop(ctx,0x8c619e60u); r[0]=4u;goto L8c619e62; /* mov #0x4,r0 */
L8c619e62: sa_motion_publish_stop(ctx,0x8c619e62u); r[3]=r[2];goto L8c619e64; /* mov r2,r3 */
L8c619e64: sa_motion_publish_stop(ctx,0x8c619e64u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619e66; /* fmul fr0,fr3 */
L8c619e66: sa_motion_publish_stop(ctx,0x8c619e66u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619e68; /* fmov.s @(r0,r3),fr2 */
L8c619e68: sa_motion_publish_stop(ctx,0x8c619e68u); fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c619e6a; /* fmov.s @r7,fr0 */
L8c619e6a: sa_motion_publish_stop(ctx,0x8c619e6au); r[0]=8u;goto L8c619e6c; /* mov #0x8,r0 */
L8c619e6c: sa_motion_publish_stop(ctx,0x8c619e6cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619e6e; /* fmac fr0,fr2,fr3 */
L8c619e6e: sa_motion_publish_stop(ctx,0x8c619e6eu); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c619e70; /* fmov.s @(r0,r7),fr0 */
L8c619e70: sa_motion_publish_stop(ctx,0x8c619e70u); r[0]=36u;goto L8c619e72; /* mov #0x24,r0 */
L8c619e72: sa_motion_publish_stop(ctx,0x8c619e72u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c619e74; /* fmov.s @(r0,r2),fr2 */
L8c619e74: sa_motion_publish_stop(ctx,0x8c619e74u); r[0]=4u;goto L8c619e76; /* mov #0x4,r0 */
L8c619e76: sa_motion_publish_stop(ctx,0x8c619e76u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619e78; /* fmac fr0,fr2,fr3 */
L8c619e78: sa_motion_publish_stop(ctx,0x8c619e78u); fcopy(f,0u,6u);goto L8c619e7a; /* fmov fr6,fr0 */
L8c619e7a: sa_motion_publish_stop(ctx,0x8c619e7au); fcopy(f,2u,6u);goto L8c619e7c; /* fmov fr6,fr2 */
L8c619e7c: sa_motion_publish_stop(ctx,0x8c619e7cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619e7e; /* fmac fr0,fr3,fr2 */
L8c619e7e: sa_motion_publish_stop(ctx,0x8c619e7eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619e80; /* fmov.s fr2,@-r8 */
L8c619e80: sa_motion_publish_stop(ctx,0x8c619e80u); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619e82; /* mov.l @r9,r3 */
L8c619e82: sa_motion_publish_stop(ctx,0x8c619e82u); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c619e84; /* fmov.s @(r0,r7),fr0 */
L8c619e84: sa_motion_publish_stop(ctx,0x8c619e84u); r[0]=16u;goto L8c619e86; /* mov #0x10,r0 */
L8c619e86: sa_motion_publish_stop(ctx,0x8c619e86u); r[1]=r[3];goto L8c619e88; /* mov r3,r1 */
L8c619e88: sa_motion_publish_stop(ctx,0x8c619e88u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619e8a; /* fmov.s @(r0,r1),fr3 */
L8c619e8a: sa_motion_publish_stop(ctx,0x8c619e8au); r[2]=r[3];goto L8c619e8c; /* mov r3,r2 */
L8c619e8c: sa_motion_publish_stop(ctx,0x8c619e8cu); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c619e8e; /* fmov.s @r2,fr2 */
L8c619e8e: sa_motion_publish_stop(ctx,0x8c619e8eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619e90; /* fmul fr0,fr3 */
L8c619e90: sa_motion_publish_stop(ctx,0x8c619e90u); fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c619e92; /* fmov.s @r7,fr0 */
L8c619e92: sa_motion_publish_stop(ctx,0x8c619e92u); r[0]=8u;goto L8c619e94; /* mov #0x8,r0 */
L8c619e94: sa_motion_publish_stop(ctx,0x8c619e94u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619e96; /* fmac fr0,fr2,fr3 */
L8c619e96: sa_motion_publish_stop(ctx,0x8c619e96u); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c619e98; /* fmov.s @(r0,r7),fr0 */
L8c619e98: sa_motion_publish_stop(ctx,0x8c619e98u); r[0]=32u;goto L8c619e9a; /* mov #0x20,r0 */
L8c619e9a: sa_motion_publish_stop(ctx,0x8c619e9au); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619e9c; /* fmov.s @(r0,r3),fr2 */
L8c619e9c: sa_motion_publish_stop(ctx,0x8c619e9cu); r[0]=24u;goto L8c619e9e; /* mov #0x18,r0 */
L8c619e9e: sa_motion_publish_stop(ctx,0x8c619e9eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619ea0; /* fmac fr0,fr2,fr3 */
L8c619ea0: sa_motion_publish_stop(ctx,0x8c619ea0u); fcopy(f,0u,6u);goto L8c619ea2; /* fmov fr6,fr0 */
L8c619ea2: sa_motion_publish_stop(ctx,0x8c619ea2u); fcopy(f,2u,6u);goto L8c619ea4; /* fmov fr6,fr2 */
L8c619ea4: sa_motion_publish_stop(ctx,0x8c619ea4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619ea6; /* fmac fr0,fr3,fr2 */
L8c619ea6: sa_motion_publish_stop(ctx,0x8c619ea6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619ea8; /* fmov.s fr2,@-r8 */
L8c619ea8: sa_motion_publish_stop(ctx,0x8c619ea8u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619eaa; /* fmov.s @(r0,r7),fr3 */
L8c619eaa: sa_motion_publish_stop(ctx,0x8c619eaau); r[0]=20u;goto L8c619eac; /* mov #0x14,r0 */
L8c619eac: sa_motion_publish_stop(ctx,0x8c619eacu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619eae; /* fmov.s fr3,@-r8 */
L8c619eae: sa_motion_publish_stop(ctx,0x8c619eaeu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619eb0; /* fmov.s @(r0,r7),fr3 */
L8c619eb0: sa_motion_publish_stop(ctx,0x8c619eb0u); r[0]=16u;goto L8c619eb2; /* mov #0x10,r0 */
L8c619eb2: sa_motion_publish_stop(ctx,0x8c619eb2u); r[3]=rd(m,bus,&io_status,0x8c619efcu,4);if(io_status)return io_status;goto L8c619eb4; /* mov.l 0x8c619efc,r3 */
L8c619eb4: sa_motion_publish_stop(ctx,0x8c619eb4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619eb6; /* fmov.s fr3,@-r8 */
L8c619eb6: sa_motion_publish_stop(ctx,0x8c619eb6u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619eb8; /* fmov.s @(r0,r7),fr3 */
L8c619eb8: sa_motion_publish_stop(ctx,0x8c619eb8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619eba; /* fmov.s fr3,@-r8 */
L8c619eba: sa_motion_publish_stop(ctx,0x8c619ebau); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c619ebc; /* mov.l r3,@-r8 */
L8c619ebc: sa_motion_publish_stop(ctx,0x8c619ebcu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619ebe; /* pref @r8 */
L8c619ebe: sa_motion_publish_stop(ctx,0x8c619ebeu); r[2]=rd(m,bus,&io_status,0x8c619f00u,4);if(io_status)return io_status;goto L8c619ec0; /* mov.l 0x8c619f00,r2 */
L8c619ec0: sa_motion_publish_stop(ctx,0x8c619ec0u); r[8]+=32u;goto L8c619ec2; /* add #0x20,r8 */
L8c619ec2: sa_motion_publish_stop(ctx,0x8c619ec2u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c619ec4; /* mov.l @r2,r3 */
L8c619ec4: sa_motion_publish_stop(ctx,0x8c619ec4u); r[3]+=3u;goto L8c619ec6; /* add #0x3,r3 */
L8c619ec6: sa_motion_publish_stop(ctx,0x8c619ec6u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c619ec8; /* mov.l r3,@r2 */
L8c619ec8: sa_motion_publish_stop(ctx,0x8c619ec8u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c619eca; /* mov.w @(0x2,r4),r0 */
L8c619eca: sa_motion_publish_stop(ctx,0x8c619ecau); r[0]=r[0]&65535u;goto L8c619ecc; /* extu.w r0,r0 */
L8c619ecc: sa_motion_publish_stop(ctx,0x8c619eccu); *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c619ece; /* cmp/ge r0,r14 */
L8c619ece: sa_motion_publish_stop(ctx,0x8c619eceu); if((*sr&1u))goto L8c619ed4;goto L8c619ed0; /* bt 0x8c619ed4 */
L8c619ed0: sa_motion_publish_stop(ctx,0x8c619ed2u); goto L8c619ca8; /* bra 0x8c619ca8 */
L8c619ed4: sa_motion_publish_stop(ctx,0x8c619ed6u); goto L8c61a446; /* bra 0x8c61a446 */
L8c619ed8: sa_motion_publish_stop(ctx,0x8c619ed8u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c619eda; /* mov.w @(0x2,r4),r0 */
L8c619eda: sa_motion_publish_stop(ctx,0x8c619edau); r[2]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c619edc; /* mov.l @r11,r2 */
L8c619edc: sa_motion_publish_stop(ctx,0x8c619edcu); r[0]=r[0]&65535u;goto L8c619ede; /* extu.w r0,r0 */
L8c619ede: sa_motion_publish_stop(ctx,0x8c619edeu); r[0]<<=2;goto L8c619ee0; /* shll2 r0 */
L8c619ee0: sa_motion_publish_stop(ctx,0x8c619ee0u); r[2]+=r[0];goto L8c619ee2; /* add r0,r2 */
L8c619ee2: sa_motion_publish_stop(ctx,0x8c619ee2u); wr(m,bus,&io_status,r[11],r[2],4);if(io_status)return io_status;goto L8c619ee4; /* mov.l r2,@r11 */
L8c619ee4: sa_motion_publish_stop(ctx,0x8c619ee6u); r[11]=r[12];goto L8c61a1c4; /* bra 0x8c61a1c4 */
L8c619f04: sa_motion_publish_stop(ctx,0x8c619f04u); r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c619f06; /* mov.w @r13+,r6 */
L8c619f06: sa_motion_publish_stop(ctx,0x8c619f06u); r[0]=16u;goto L8c619f08; /* mov #0x10,r0 */
L8c619f08: sa_motion_publish_stop(ctx,0x8c619f08u); r[3]=rd(m,bus,&io_status,0x8c61a23cu,4);if(io_status)return io_status;goto L8c619f0a; /* mov.l 0x8c61a23c,r3 */
L8c619f0a: sa_motion_publish_stop(ctx,0x8c619f0au); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c619f0c; /* mov.w @r13+,r5 */
L8c619f0c: sa_motion_publish_stop(ctx,0x8c619f0cu); r[6]=r[6]&65535u;goto L8c619f0e; /* extu.w r6,r6 */
L8c619f0e: sa_motion_publish_stop(ctx,0x8c619f0eu); r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c619f10; /* mov.l @r3,r12 */
L8c619f10: sa_motion_publish_stop(ctx,0x8c619f10u); r[6]<<=2;goto L8c619f12; /* shll2 r6 */
L8c619f12: sa_motion_publish_stop(ctx,0x8c619f12u); r[5]=r[5]&65535u;goto L8c619f14; /* extu.w r5,r5 */
L8c619f14: sa_motion_publish_stop(ctx,0x8c619f14u); r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c619f16; /* mov.w @r13+,r7 */
L8c619f16: sa_motion_publish_stop(ctx,0x8c619f16u); r[5]<<=2;goto L8c619f18; /* shll2 r5 */
L8c619f18: sa_motion_publish_stop(ctx,0x8c619f18u); r[5]<<=2;goto L8c619f1a; /* shll2 r5 */
L8c619f1a: sa_motion_publish_stop(ctx,0x8c619f1au); r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c619f1c; /* mov.w @r13+,r14 */
L8c619f1c: sa_motion_publish_stop(ctx,0x8c619f1cu); r[6]<<=2;goto L8c619f1e; /* shll2 r6 */
L8c619f1e: sa_motion_publish_stop(ctx,0x8c619f1eu); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c619f20; /* shll r6 */
L8c619f20: sa_motion_publish_stop(ctx,0x8c619f20u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c619f22; /* shll r5 */
L8c619f22: sa_motion_publish_stop(ctx,0x8c619f22u); r[5]+=r[12];goto L8c619f24; /* add r12,r5 */
L8c619f24: sa_motion_publish_stop(ctx,0x8c619f24u); r[6]+=r[12];goto L8c619f26; /* add r12,r6 */
L8c619f26: sa_motion_publish_stop(ctx,0x8c619f26u); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c619f28; /* fmov.s @(r0,r5),fr4 */
L8c619f28: sa_motion_publish_stop(ctx,0x8c619f28u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619f2a; /* fmov.s @(r0,r6),fr3 */
L8c619f2a: sa_motion_publish_stop(ctx,0x8c619f2au); r[7]=r[7]&65535u;goto L8c619f2c; /* extu.w r7,r7 */
L8c619f2c: sa_motion_publish_stop(ctx,0x8c619f2cu); fcopy(f,2u,4u);goto L8c619f2e; /* fmov fr4,fr2 */
L8c619f2e: sa_motion_publish_stop(ctx,0x8c619f2eu); r[7]<<=2;goto L8c619f30; /* shll2 r7 */
L8c619f30: sa_motion_publish_stop(ctx,0x8c619f30u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c619f32; /* fsub fr3,fr2 */
L8c619f32: sa_motion_publish_stop(ctx,0x8c619f32u); r[14]=r[14]&65535u;goto L8c619f34; /* extu.w r14,r14 */
L8c619f34: sa_motion_publish_stop(ctx,0x8c619f34u); r[7]<<=2;goto L8c619f36; /* shll2 r7 */
L8c619f36: sa_motion_publish_stop(ctx,0x8c619f36u); r[14]<<=2;goto L8c619f38; /* shll2 r14 */
L8c619f38: sa_motion_publish_stop(ctx,0x8c619f38u); r[0]=20u;goto L8c619f3a; /* mov #0x14,r0 */
L8c619f3a: sa_motion_publish_stop(ctx,0x8c619f3au); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c619f3c; /* fmov.s @(r0,r5),fr5 */
L8c619f3c: sa_motion_publish_stop(ctx,0x8c619f3cu); r[14]<<=2;goto L8c619f3e; /* shll2 r14 */
L8c619f3e: sa_motion_publish_stop(ctx,0x8c619f3eu); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c619f40; /* shll r7 */
L8c619f40: sa_motion_publish_stop(ctx,0x8c619f40u); r[7]+=r[12];goto L8c619f42; /* add r12,r7 */
L8c619f42: sa_motion_publish_stop(ctx,0x8c619f42u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c619f44; /* shll r14 */
L8c619f44: sa_motion_publish_stop(ctx,0x8c619f44u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619f46; /* fmov.s @(r0,r7),fr3 */
L8c619f46: sa_motion_publish_stop(ctx,0x8c619f46u); fcopy(f,1u,5u);goto L8c619f48; /* fmov fr5,fr1 */
L8c619f48: sa_motion_publish_stop(ctx,0x8c619f48u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619f4a; /* fsub fr5,fr3 */
L8c619f4a: sa_motion_publish_stop(ctx,0x8c619f4au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619f4c; /* fmul fr3,fr2 */
L8c619f4c: sa_motion_publish_stop(ctx,0x8c619f4cu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619f4e; /* fmov.s @(r0,r6),fr3 */
L8c619f4e: sa_motion_publish_stop(ctx,0x8c619f4eu); r[0]=16u;goto L8c619f50; /* mov #0x10,r0 */
L8c619f50: sa_motion_publish_stop(ctx,0x8c619f50u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c619f52; /* fsub fr3,fr1 */
L8c619f52: sa_motion_publish_stop(ctx,0x8c619f52u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c619f54; /* fmov.s @(r0,r7),fr3 */
L8c619f54: sa_motion_publish_stop(ctx,0x8c619f54u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619f56; /* fsub fr4,fr3 */
L8c619f56: sa_motion_publish_stop(ctx,0x8c619f56u); fcopy(f,4u,2u);goto L8c619f58; /* fmov fr2,fr4 */
L8c619f58: sa_motion_publish_stop(ctx,0x8c619f58u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619f5a; /* fmul fr3,fr1 */
L8c619f5a: sa_motion_publish_stop(ctx,0x8c619f5au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c619f5c; /* fldi0 fr3 */
L8c619f5c: sa_motion_publish_stop(ctx,0x8c619f5cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c619f5e; /* fsub fr1,fr4 */
L8c619f5e: sa_motion_publish_stop(ctx,0x8c619f5eu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c619f60; /* fcmp/gt fr3,fr4 */
L8c619f60: sa_motion_publish_stop(ctx,0x8c619f62u); take=!(*sr&1u);r[14]+=r[12];if(take)goto L8c619f68;goto L8c619f64; /* bf/s 0x8c619f68 */
L8c619f64: sa_motion_publish_stop(ctx,0x8c619f66u); goto L8c61a1c2; /* bra 0x8c61a1c2 */
L8c619f68: sa_motion_publish_stop(ctx,0x8c619f68u); r[3]=rd(m,bus,&io_status,0x8c61a240u,4);if(io_status)return io_status;goto L8c619f6a; /* mov.l 0x8c61a240,r3 */
L8c619f6a: sa_motion_publish_stop(ctx,0x8c619f6au); r[0]=4u;goto L8c619f6c; /* mov #0x4,r0 */
L8c619f6c: sa_motion_publish_stop(ctx,0x8c619f6cu); fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c619f6e; /* fmov.s @r6,fr3 */
L8c619f6e: sa_motion_publish_stop(ctx,0x8c619f6eu); r[8]+=28u;goto L8c619f70; /* add #0x1c,r8 */
L8c619f70: sa_motion_publish_stop(ctx,0x8c619f70u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c619f72; /* fmov.s @r3,fr2 */
L8c619f72: sa_motion_publish_stop(ctx,0x8c619f72u); r[3]+=8u;goto L8c619f74; /* add #0x8,r3 */
L8c619f74: sa_motion_publish_stop(ctx,0x8c619f74u); r[2]=rd(m,bus,&io_status,0x8c61a244u,4);if(io_status)return io_status;goto L8c619f76; /* mov.l 0x8c61a244,r2 */
L8c619f76: sa_motion_publish_stop(ctx,0x8c619f76u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619f78; /* fmul fr3,fr2 */
L8c619f78: sa_motion_publish_stop(ctx,0x8c619f78u); r[1]=rd(m,bus,&io_status,0x8c61a248u,4);if(io_status)return io_status;goto L8c619f7a; /* mov.l 0x8c61a248,r1 */
L8c619f7a: sa_motion_publish_stop(ctx,0x8c619f7au); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c619f7c; /* fmov.s @r2,fr3 */
L8c619f7c: sa_motion_publish_stop(ctx,0x8c619f7cu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c619f7e; /* fmov.s @r1,fr1 */
L8c619f7e: sa_motion_publish_stop(ctx,0x8c619f7eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619f80; /* fsub fr2,fr3 */
L8c619f80: sa_motion_publish_stop(ctx,0x8c619f80u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c619f82; /* fmov.s @(r0,r6),fr2 */
L8c619f82: sa_motion_publish_stop(ctx,0x8c619f82u); r[0]=8u;goto L8c619f84; /* mov #0x8,r0 */
L8c619f84: sa_motion_publish_stop(ctx,0x8c619f84u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619f86; /* fmul fr2,fr1 */
L8c619f86: sa_motion_publish_stop(ctx,0x8c619f86u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c619f88; /* fmov.s @(r0,r6),fr2 */
L8c619f88: sa_motion_publish_stop(ctx,0x8c619f88u); r[0]=4u;goto L8c619f8a; /* mov #0x4,r0 */
L8c619f8a: sa_motion_publish_stop(ctx,0x8c619f8au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619f8c; /* fsub fr1,fr3 */
L8c619f8c: sa_motion_publish_stop(ctx,0x8c619f8cu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c619f8e; /* fmov.s @r3,fr1 */
L8c619f8e: sa_motion_publish_stop(ctx,0x8c619f8eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c619f90; /* fmul fr2,fr1 */
L8c619f90: sa_motion_publish_stop(ctx,0x8c619f90u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c619f92; /* fsub fr1,fr3 */
L8c619f92: sa_motion_publish_stop(ctx,0x8c619f92u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619f94; /* fmov.s fr3,@-r8 */
L8c619f94: sa_motion_publish_stop(ctx,0x8c619f94u); r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619f96; /* mov.l @r9,r2 */
L8c619f96: sa_motion_publish_stop(ctx,0x8c619f96u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619f98; /* fmov.s @(r0,r6),fr0 */
L8c619f98: sa_motion_publish_stop(ctx,0x8c619f98u); r[0]=20u;goto L8c619f9a; /* mov #0x14,r0 */
L8c619f9a: sa_motion_publish_stop(ctx,0x8c619f9au); r[1]=r[2];goto L8c619f9c; /* mov r2,r1 */
L8c619f9c: sa_motion_publish_stop(ctx,0x8c619f9cu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619f9e; /* fmov.s @(r0,r1),fr3 */
L8c619f9e: sa_motion_publish_stop(ctx,0x8c619f9eu); r[0]=4u;goto L8c619fa0; /* mov #0x4,r0 */
L8c619fa0: sa_motion_publish_stop(ctx,0x8c619fa0u); r[3]=r[2];goto L8c619fa2; /* mov r2,r3 */
L8c619fa2: sa_motion_publish_stop(ctx,0x8c619fa2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619fa4; /* fmul fr0,fr3 */
L8c619fa4: sa_motion_publish_stop(ctx,0x8c619fa4u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619fa6; /* fmov.s @(r0,r3),fr2 */
L8c619fa6: sa_motion_publish_stop(ctx,0x8c619fa6u); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c619fa8; /* fmov.s @r6,fr0 */
L8c619fa8: sa_motion_publish_stop(ctx,0x8c619fa8u); r[0]=8u;goto L8c619faa; /* mov #0x8,r0 */
L8c619faa: sa_motion_publish_stop(ctx,0x8c619faau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619fac; /* fmac fr0,fr2,fr3 */
L8c619fac: sa_motion_publish_stop(ctx,0x8c619facu); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619fae; /* fmov.s @(r0,r6),fr0 */
L8c619fae: sa_motion_publish_stop(ctx,0x8c619faeu); r[0]=36u;goto L8c619fb0; /* mov #0x24,r0 */
L8c619fb0: sa_motion_publish_stop(ctx,0x8c619fb0u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c619fb2; /* fmov.s @(r0,r2),fr2 */
L8c619fb2: sa_motion_publish_stop(ctx,0x8c619fb2u); r[0]=4u;goto L8c619fb4; /* mov #0x4,r0 */
L8c619fb4: sa_motion_publish_stop(ctx,0x8c619fb4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619fb6; /* fmac fr0,fr2,fr3 */
L8c619fb6: sa_motion_publish_stop(ctx,0x8c619fb6u); fcopy(f,0u,6u);goto L8c619fb8; /* fmov fr6,fr0 */
L8c619fb8: sa_motion_publish_stop(ctx,0x8c619fb8u); fcopy(f,2u,6u);goto L8c619fba; /* fmov fr6,fr2 */
L8c619fba: sa_motion_publish_stop(ctx,0x8c619fbau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619fbc; /* fmac fr0,fr3,fr2 */
L8c619fbc: sa_motion_publish_stop(ctx,0x8c619fbcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619fbe; /* fmov.s fr2,@-r8 */
L8c619fbe: sa_motion_publish_stop(ctx,0x8c619fbeu); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c619fc0; /* mov.l @r9,r3 */
L8c619fc0: sa_motion_publish_stop(ctx,0x8c619fc0u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619fc2; /* fmov.s @(r0,r6),fr0 */
L8c619fc2: sa_motion_publish_stop(ctx,0x8c619fc2u); r[0]=16u;goto L8c619fc4; /* mov #0x10,r0 */
L8c619fc4: sa_motion_publish_stop(ctx,0x8c619fc4u); r[1]=r[3];goto L8c619fc6; /* mov r3,r1 */
L8c619fc6: sa_motion_publish_stop(ctx,0x8c619fc6u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c619fc8; /* fmov.s @(r0,r1),fr3 */
L8c619fc8: sa_motion_publish_stop(ctx,0x8c619fc8u); r[2]=r[3];goto L8c619fca; /* mov r3,r2 */
L8c619fca: sa_motion_publish_stop(ctx,0x8c619fcau); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c619fcc; /* fmov.s @r2,fr2 */
L8c619fcc: sa_motion_publish_stop(ctx,0x8c619fccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619fce; /* fmul fr0,fr3 */
L8c619fce: sa_motion_publish_stop(ctx,0x8c619fceu); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c619fd0; /* fmov.s @r6,fr0 */
L8c619fd0: sa_motion_publish_stop(ctx,0x8c619fd0u); r[0]=8u;goto L8c619fd2; /* mov #0x8,r0 */
L8c619fd2: sa_motion_publish_stop(ctx,0x8c619fd2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619fd4; /* fmac fr0,fr2,fr3 */
L8c619fd4: sa_motion_publish_stop(ctx,0x8c619fd4u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c619fd6; /* fmov.s @(r0,r6),fr0 */
L8c619fd6: sa_motion_publish_stop(ctx,0x8c619fd6u); r[0]=32u;goto L8c619fd8; /* mov #0x20,r0 */
L8c619fd8: sa_motion_publish_stop(ctx,0x8c619fd8u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c619fda; /* fmov.s @(r0,r3),fr2 */
L8c619fda: sa_motion_publish_stop(ctx,0x8c619fdau); r[0]=24u;goto L8c619fdc; /* mov #0x18,r0 */
L8c619fdc: sa_motion_publish_stop(ctx,0x8c619fdcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c619fde; /* fmac fr0,fr2,fr3 */
L8c619fde: sa_motion_publish_stop(ctx,0x8c619fdeu); fcopy(f,0u,6u);goto L8c619fe0; /* fmov fr6,fr0 */
L8c619fe0: sa_motion_publish_stop(ctx,0x8c619fe0u); fcopy(f,2u,6u);goto L8c619fe2; /* fmov fr6,fr2 */
L8c619fe2: sa_motion_publish_stop(ctx,0x8c619fe2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c619fe4; /* fmac fr0,fr3,fr2 */
L8c619fe4: sa_motion_publish_stop(ctx,0x8c619fe4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c619fe6; /* fmov.s fr2,@-r8 */
L8c619fe6: sa_motion_publish_stop(ctx,0x8c619fe6u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619fe8; /* fmov.s @(r0,r6),fr3 */
L8c619fe8: sa_motion_publish_stop(ctx,0x8c619fe8u); r[0]=20u;goto L8c619fea; /* mov #0x14,r0 */
L8c619fea: sa_motion_publish_stop(ctx,0x8c619feau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619fec; /* fmov.s fr3,@-r8 */
L8c619fec: sa_motion_publish_stop(ctx,0x8c619fecu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619fee; /* fmov.s @(r0,r6),fr3 */
L8c619fee: sa_motion_publish_stop(ctx,0x8c619feeu); r[0]=16u;goto L8c619ff0; /* mov #0x10,r0 */
L8c619ff0: sa_motion_publish_stop(ctx,0x8c619ff0u); r[3]=rd(m,bus,&io_status,0x8c61a24cu,4);if(io_status)return io_status;goto L8c619ff2; /* mov.l 0x8c61a24c,r3 */
L8c619ff2: sa_motion_publish_stop(ctx,0x8c619ff2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619ff4; /* fmov.s fr3,@-r8 */
L8c619ff4: sa_motion_publish_stop(ctx,0x8c619ff4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c619ff6; /* fmov.s @(r0,r6),fr3 */
L8c619ff6: sa_motion_publish_stop(ctx,0x8c619ff6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c619ff8; /* fmov.s fr3,@-r8 */
L8c619ff8: sa_motion_publish_stop(ctx,0x8c619ff8u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c619ffa; /* mov.l r3,@-r8 */
L8c619ffa: sa_motion_publish_stop(ctx,0x8c619ffau); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c619ffc; /* pref @r8 */
L8c619ffc: sa_motion_publish_stop(ctx,0x8c619ffcu); r[3]=rd(m,bus,&io_status,0x8c61a240u,4);if(io_status)return io_status;goto L8c619ffe; /* mov.l 0x8c61a240,r3 */
L8c619ffe: sa_motion_publish_stop(ctx,0x8c619ffeu); r[0]=4u;goto L8c61a000; /* mov #0x4,r0 */
L8c61a000: sa_motion_publish_stop(ctx,0x8c61a000u); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61a002; /* fmov.s @r5,fr3 */
L8c61a002: sa_motion_publish_stop(ctx,0x8c61a002u); r[8]+=60u;goto L8c61a004; /* add #0x3c,r8 */
L8c61a004: sa_motion_publish_stop(ctx,0x8c61a004u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a006; /* fmov.s @r3,fr2 */
L8c61a006: sa_motion_publish_stop(ctx,0x8c61a006u); r[3]+=8u;goto L8c61a008; /* add #0x8,r3 */
L8c61a008: sa_motion_publish_stop(ctx,0x8c61a008u); r[2]=rd(m,bus,&io_status,0x8c61a244u,4);if(io_status)return io_status;goto L8c61a00a; /* mov.l 0x8c61a244,r2 */
L8c61a00a: sa_motion_publish_stop(ctx,0x8c61a00au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a00c; /* fmul fr3,fr2 */
L8c61a00c: sa_motion_publish_stop(ctx,0x8c61a00cu); r[1]=rd(m,bus,&io_status,0x8c61a248u,4);if(io_status)return io_status;goto L8c61a00e; /* mov.l 0x8c61a248,r1 */
L8c61a00e: sa_motion_publish_stop(ctx,0x8c61a00eu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61a010; /* fmov.s @r2,fr3 */
L8c61a010: sa_motion_publish_stop(ctx,0x8c61a010u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61a012; /* fmov.s @r1,fr1 */
L8c61a012: sa_motion_publish_stop(ctx,0x8c61a012u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a014; /* fsub fr2,fr3 */
L8c61a014: sa_motion_publish_stop(ctx,0x8c61a014u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a016; /* fmov.s @(r0,r5),fr2 */
L8c61a016: sa_motion_publish_stop(ctx,0x8c61a016u); r[0]=8u;goto L8c61a018; /* mov #0x8,r0 */
L8c61a018: sa_motion_publish_stop(ctx,0x8c61a018u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a01a; /* fmul fr2,fr1 */
L8c61a01a: sa_motion_publish_stop(ctx,0x8c61a01au); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a01c; /* fmov.s @(r0,r5),fr2 */
L8c61a01c: sa_motion_publish_stop(ctx,0x8c61a01cu); r[0]=4u;goto L8c61a01e; /* mov #0x4,r0 */
L8c61a01e: sa_motion_publish_stop(ctx,0x8c61a01eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a020; /* fsub fr1,fr3 */
L8c61a020: sa_motion_publish_stop(ctx,0x8c61a020u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a022; /* fmov.s @r3,fr1 */
L8c61a022: sa_motion_publish_stop(ctx,0x8c61a022u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a024; /* fmul fr2,fr1 */
L8c61a024: sa_motion_publish_stop(ctx,0x8c61a024u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a026; /* fsub fr1,fr3 */
L8c61a026: sa_motion_publish_stop(ctx,0x8c61a026u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a028; /* fmov.s fr3,@-r8 */
L8c61a028: sa_motion_publish_stop(ctx,0x8c61a028u); r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a02a; /* mov.l @r9,r2 */
L8c61a02a: sa_motion_publish_stop(ctx,0x8c61a02au); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a02c; /* fmov.s @(r0,r5),fr0 */
L8c61a02c: sa_motion_publish_stop(ctx,0x8c61a02cu); r[0]=20u;goto L8c61a02e; /* mov #0x14,r0 */
L8c61a02e: sa_motion_publish_stop(ctx,0x8c61a02eu); r[1]=r[2];goto L8c61a030; /* mov r2,r1 */
L8c61a030: sa_motion_publish_stop(ctx,0x8c61a030u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a032; /* fmov.s @(r0,r1),fr3 */
L8c61a032: sa_motion_publish_stop(ctx,0x8c61a032u); r[0]=4u;goto L8c61a034; /* mov #0x4,r0 */
L8c61a034: sa_motion_publish_stop(ctx,0x8c61a034u); r[3]=r[2];goto L8c61a036; /* mov r2,r3 */
L8c61a036: sa_motion_publish_stop(ctx,0x8c61a036u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a038; /* fmul fr0,fr3 */
L8c61a038: sa_motion_publish_stop(ctx,0x8c61a038u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a03a; /* fmov.s @(r0,r3),fr2 */
L8c61a03a: sa_motion_publish_stop(ctx,0x8c61a03au); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a03c; /* fmov.s @r5,fr0 */
L8c61a03c: sa_motion_publish_stop(ctx,0x8c61a03cu); r[0]=8u;goto L8c61a03e; /* mov #0x8,r0 */
L8c61a03e: sa_motion_publish_stop(ctx,0x8c61a03eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a040; /* fmac fr0,fr2,fr3 */
L8c61a040: sa_motion_publish_stop(ctx,0x8c61a040u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a042; /* fmov.s @(r0,r5),fr0 */
L8c61a042: sa_motion_publish_stop(ctx,0x8c61a042u); r[0]=36u;goto L8c61a044; /* mov #0x24,r0 */
L8c61a044: sa_motion_publish_stop(ctx,0x8c61a044u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a046; /* fmov.s @(r0,r2),fr2 */
L8c61a046: sa_motion_publish_stop(ctx,0x8c61a046u); r[0]=4u;goto L8c61a048; /* mov #0x4,r0 */
L8c61a048: sa_motion_publish_stop(ctx,0x8c61a048u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a04a; /* fmac fr0,fr2,fr3 */
L8c61a04a: sa_motion_publish_stop(ctx,0x8c61a04au); fcopy(f,0u,6u);goto L8c61a04c; /* fmov fr6,fr0 */
L8c61a04c: sa_motion_publish_stop(ctx,0x8c61a04cu); fcopy(f,2u,6u);goto L8c61a04e; /* fmov fr6,fr2 */
L8c61a04e: sa_motion_publish_stop(ctx,0x8c61a04eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a050; /* fmac fr0,fr3,fr2 */
L8c61a050: sa_motion_publish_stop(ctx,0x8c61a050u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a052; /* fmov.s fr2,@-r8 */
L8c61a052: sa_motion_publish_stop(ctx,0x8c61a052u); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a054; /* mov.l @r9,r3 */
L8c61a054: sa_motion_publish_stop(ctx,0x8c61a054u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a056; /* fmov.s @(r0,r5),fr0 */
L8c61a056: sa_motion_publish_stop(ctx,0x8c61a056u); r[0]=16u;goto L8c61a058; /* mov #0x10,r0 */
L8c61a058: sa_motion_publish_stop(ctx,0x8c61a058u); r[1]=r[3];goto L8c61a05a; /* mov r3,r1 */
L8c61a05a: sa_motion_publish_stop(ctx,0x8c61a05au); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a05c; /* fmov.s @(r0,r1),fr3 */
L8c61a05c: sa_motion_publish_stop(ctx,0x8c61a05cu); r[2]=r[3];goto L8c61a05e; /* mov r3,r2 */
L8c61a05e: sa_motion_publish_stop(ctx,0x8c61a05eu); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61a060; /* fmov.s @r2,fr2 */
L8c61a060: sa_motion_publish_stop(ctx,0x8c61a060u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a062; /* fmul fr0,fr3 */
L8c61a062: sa_motion_publish_stop(ctx,0x8c61a062u); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a064; /* fmov.s @r5,fr0 */
L8c61a064: sa_motion_publish_stop(ctx,0x8c61a064u); r[0]=8u;goto L8c61a066; /* mov #0x8,r0 */
L8c61a066: sa_motion_publish_stop(ctx,0x8c61a066u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a068; /* fmac fr0,fr2,fr3 */
L8c61a068: sa_motion_publish_stop(ctx,0x8c61a068u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a06a; /* fmov.s @(r0,r5),fr0 */
L8c61a06a: sa_motion_publish_stop(ctx,0x8c61a06au); r[0]=32u;goto L8c61a06c; /* mov #0x20,r0 */
L8c61a06c: sa_motion_publish_stop(ctx,0x8c61a06cu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a06e; /* fmov.s @(r0,r3),fr2 */
L8c61a06e: sa_motion_publish_stop(ctx,0x8c61a06eu); r[0]=24u;goto L8c61a070; /* mov #0x18,r0 */
L8c61a070: sa_motion_publish_stop(ctx,0x8c61a070u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a072; /* fmac fr0,fr2,fr3 */
L8c61a072: sa_motion_publish_stop(ctx,0x8c61a072u); fcopy(f,0u,6u);goto L8c61a074; /* fmov fr6,fr0 */
L8c61a074: sa_motion_publish_stop(ctx,0x8c61a074u); fcopy(f,2u,6u);goto L8c61a076; /* fmov fr6,fr2 */
L8c61a076: sa_motion_publish_stop(ctx,0x8c61a076u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a078; /* fmac fr0,fr3,fr2 */
L8c61a078: sa_motion_publish_stop(ctx,0x8c61a078u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a07a; /* fmov.s fr2,@-r8 */
L8c61a07a: sa_motion_publish_stop(ctx,0x8c61a07au); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a07c; /* fmov.s @(r0,r5),fr3 */
L8c61a07c: sa_motion_publish_stop(ctx,0x8c61a07cu); r[0]=20u;goto L8c61a07e; /* mov #0x14,r0 */
L8c61a07e: sa_motion_publish_stop(ctx,0x8c61a07eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a080; /* fmov.s fr3,@-r8 */
L8c61a080: sa_motion_publish_stop(ctx,0x8c61a080u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a082; /* fmov.s @(r0,r5),fr3 */
L8c61a082: sa_motion_publish_stop(ctx,0x8c61a082u); r[0]=16u;goto L8c61a084; /* mov #0x10,r0 */
L8c61a084: sa_motion_publish_stop(ctx,0x8c61a084u); r[3]=rd(m,bus,&io_status,0x8c61a24cu,4);if(io_status)return io_status;goto L8c61a086; /* mov.l 0x8c61a24c,r3 */
L8c61a086: sa_motion_publish_stop(ctx,0x8c61a086u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a088; /* fmov.s fr3,@-r8 */
L8c61a088: sa_motion_publish_stop(ctx,0x8c61a088u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a08a; /* fmov.s @(r0,r5),fr3 */
L8c61a08a: sa_motion_publish_stop(ctx,0x8c61a08au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a08c; /* fmov.s fr3,@-r8 */
L8c61a08c: sa_motion_publish_stop(ctx,0x8c61a08cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a08e; /* mov.l r3,@-r8 */
L8c61a08e: sa_motion_publish_stop(ctx,0x8c61a08eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a090; /* pref @r8 */
L8c61a090: sa_motion_publish_stop(ctx,0x8c61a090u); r[3]=rd(m,bus,&io_status,0x8c61a240u,4);if(io_status)return io_status;goto L8c61a092; /* mov.l 0x8c61a240,r3 */
L8c61a092: sa_motion_publish_stop(ctx,0x8c61a092u); r[0]=4u;goto L8c61a094; /* mov #0x4,r0 */
L8c61a094: sa_motion_publish_stop(ctx,0x8c61a094u); fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c61a096; /* fmov.s @r7,fr3 */
L8c61a096: sa_motion_publish_stop(ctx,0x8c61a096u); r[8]+=60u;goto L8c61a098; /* add #0x3c,r8 */
L8c61a098: sa_motion_publish_stop(ctx,0x8c61a098u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a09a; /* fmov.s @r3,fr2 */
L8c61a09a: sa_motion_publish_stop(ctx,0x8c61a09au); r[3]+=8u;goto L8c61a09c; /* add #0x8,r3 */
L8c61a09c: sa_motion_publish_stop(ctx,0x8c61a09cu); r[2]=rd(m,bus,&io_status,0x8c61a244u,4);if(io_status)return io_status;goto L8c61a09e; /* mov.l 0x8c61a244,r2 */
L8c61a09e: sa_motion_publish_stop(ctx,0x8c61a09eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a0a0; /* fmul fr3,fr2 */
L8c61a0a0: sa_motion_publish_stop(ctx,0x8c61a0a0u); r[1]=rd(m,bus,&io_status,0x8c61a248u,4);if(io_status)return io_status;goto L8c61a0a2; /* mov.l 0x8c61a248,r1 */
L8c61a0a2: sa_motion_publish_stop(ctx,0x8c61a0a2u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61a0a4; /* fmov.s @r2,fr3 */
L8c61a0a4: sa_motion_publish_stop(ctx,0x8c61a0a4u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61a0a6; /* fmov.s @r1,fr1 */
L8c61a0a6: sa_motion_publish_stop(ctx,0x8c61a0a6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a0a8; /* fsub fr2,fr3 */
L8c61a0a8: sa_motion_publish_stop(ctx,0x8c61a0a8u); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a0aa; /* fmov.s @(r0,r7),fr2 */
L8c61a0aa: sa_motion_publish_stop(ctx,0x8c61a0aau); r[0]=8u;goto L8c61a0ac; /* mov #0x8,r0 */
L8c61a0ac: sa_motion_publish_stop(ctx,0x8c61a0acu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a0ae; /* fmul fr2,fr1 */
L8c61a0ae: sa_motion_publish_stop(ctx,0x8c61a0aeu); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a0b0; /* fmov.s @(r0,r7),fr2 */
L8c61a0b0: sa_motion_publish_stop(ctx,0x8c61a0b0u); r[0]=4u;goto L8c61a0b2; /* mov #0x4,r0 */
L8c61a0b2: sa_motion_publish_stop(ctx,0x8c61a0b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a0b4; /* fsub fr1,fr3 */
L8c61a0b4: sa_motion_publish_stop(ctx,0x8c61a0b4u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a0b6; /* fmov.s @r3,fr1 */
L8c61a0b6: sa_motion_publish_stop(ctx,0x8c61a0b6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a0b8; /* fmul fr2,fr1 */
L8c61a0b8: sa_motion_publish_stop(ctx,0x8c61a0b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a0ba; /* fsub fr1,fr3 */
L8c61a0ba: sa_motion_publish_stop(ctx,0x8c61a0bau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a0bc; /* fmov.s fr3,@-r8 */
L8c61a0bc: sa_motion_publish_stop(ctx,0x8c61a0bcu); r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a0be; /* mov.l @r9,r2 */
L8c61a0be: sa_motion_publish_stop(ctx,0x8c61a0beu); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a0c0; /* fmov.s @(r0,r7),fr0 */
L8c61a0c0: sa_motion_publish_stop(ctx,0x8c61a0c0u); r[0]=20u;goto L8c61a0c2; /* mov #0x14,r0 */
L8c61a0c2: sa_motion_publish_stop(ctx,0x8c61a0c2u); r[1]=r[2];goto L8c61a0c4; /* mov r2,r1 */
L8c61a0c4: sa_motion_publish_stop(ctx,0x8c61a0c4u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a0c6; /* fmov.s @(r0,r1),fr3 */
L8c61a0c6: sa_motion_publish_stop(ctx,0x8c61a0c6u); r[0]=4u;goto L8c61a0c8; /* mov #0x4,r0 */
L8c61a0c8: sa_motion_publish_stop(ctx,0x8c61a0c8u); r[3]=r[2];goto L8c61a0ca; /* mov r2,r3 */
L8c61a0ca: sa_motion_publish_stop(ctx,0x8c61a0cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a0cc; /* fmul fr0,fr3 */
L8c61a0cc: sa_motion_publish_stop(ctx,0x8c61a0ccu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a0ce; /* fmov.s @(r0,r3),fr2 */
L8c61a0ce: sa_motion_publish_stop(ctx,0x8c61a0ceu); fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c61a0d0; /* fmov.s @r7,fr0 */
L8c61a0d0: sa_motion_publish_stop(ctx,0x8c61a0d0u); r[0]=8u;goto L8c61a0d2; /* mov #0x8,r0 */
L8c61a0d2: sa_motion_publish_stop(ctx,0x8c61a0d2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a0d4; /* fmac fr0,fr2,fr3 */
L8c61a0d4: sa_motion_publish_stop(ctx,0x8c61a0d4u); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a0d6; /* fmov.s @(r0,r7),fr0 */
L8c61a0d6: sa_motion_publish_stop(ctx,0x8c61a0d6u); r[0]=36u;goto L8c61a0d8; /* mov #0x24,r0 */
L8c61a0d8: sa_motion_publish_stop(ctx,0x8c61a0d8u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a0da; /* fmov.s @(r0,r2),fr2 */
L8c61a0da: sa_motion_publish_stop(ctx,0x8c61a0dau); r[0]=4u;goto L8c61a0dc; /* mov #0x4,r0 */
L8c61a0dc: sa_motion_publish_stop(ctx,0x8c61a0dcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a0de; /* fmac fr0,fr2,fr3 */
L8c61a0de: sa_motion_publish_stop(ctx,0x8c61a0deu); fcopy(f,0u,6u);goto L8c61a0e0; /* fmov fr6,fr0 */
L8c61a0e0: sa_motion_publish_stop(ctx,0x8c61a0e0u); fcopy(f,2u,6u);goto L8c61a0e2; /* fmov fr6,fr2 */
L8c61a0e2: sa_motion_publish_stop(ctx,0x8c61a0e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a0e4; /* fmac fr0,fr3,fr2 */
L8c61a0e4: sa_motion_publish_stop(ctx,0x8c61a0e4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a0e6; /* fmov.s fr2,@-r8 */
L8c61a0e6: sa_motion_publish_stop(ctx,0x8c61a0e6u); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a0e8; /* mov.l @r9,r3 */
L8c61a0e8: sa_motion_publish_stop(ctx,0x8c61a0e8u); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a0ea; /* fmov.s @(r0,r7),fr0 */
L8c61a0ea: sa_motion_publish_stop(ctx,0x8c61a0eau); r[0]=16u;goto L8c61a0ec; /* mov #0x10,r0 */
L8c61a0ec: sa_motion_publish_stop(ctx,0x8c61a0ecu); r[1]=r[3];goto L8c61a0ee; /* mov r3,r1 */
L8c61a0ee: sa_motion_publish_stop(ctx,0x8c61a0eeu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a0f0; /* fmov.s @(r0,r1),fr3 */
L8c61a0f0: sa_motion_publish_stop(ctx,0x8c61a0f0u); r[2]=r[3];goto L8c61a0f2; /* mov r3,r2 */
L8c61a0f2: sa_motion_publish_stop(ctx,0x8c61a0f2u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61a0f4; /* fmov.s @r2,fr2 */
L8c61a0f4: sa_motion_publish_stop(ctx,0x8c61a0f4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a0f6; /* fmul fr0,fr3 */
L8c61a0f6: sa_motion_publish_stop(ctx,0x8c61a0f6u); fload(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c61a0f8; /* fmov.s @r7,fr0 */
L8c61a0f8: sa_motion_publish_stop(ctx,0x8c61a0f8u); r[0]=8u;goto L8c61a0fa; /* mov #0x8,r0 */
L8c61a0fa: sa_motion_publish_stop(ctx,0x8c61a0fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a0fc; /* fmac fr0,fr2,fr3 */
L8c61a0fc: sa_motion_publish_stop(ctx,0x8c61a0fcu); fload(m,bus,&io_status,f,0u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a0fe; /* fmov.s @(r0,r7),fr0 */
L8c61a0fe: sa_motion_publish_stop(ctx,0x8c61a0feu); r[0]=32u;goto L8c61a100; /* mov #0x20,r0 */
L8c61a100: sa_motion_publish_stop(ctx,0x8c61a100u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a102; /* fmov.s @(r0,r3),fr2 */
L8c61a102: sa_motion_publish_stop(ctx,0x8c61a102u); r[0]=24u;goto L8c61a104; /* mov #0x18,r0 */
L8c61a104: sa_motion_publish_stop(ctx,0x8c61a104u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a106; /* fmac fr0,fr2,fr3 */
L8c61a106: sa_motion_publish_stop(ctx,0x8c61a106u); fcopy(f,0u,6u);goto L8c61a108; /* fmov fr6,fr0 */
L8c61a108: sa_motion_publish_stop(ctx,0x8c61a108u); fcopy(f,2u,6u);goto L8c61a10a; /* fmov fr6,fr2 */
L8c61a10a: sa_motion_publish_stop(ctx,0x8c61a10au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a10c; /* fmac fr0,fr3,fr2 */
L8c61a10c: sa_motion_publish_stop(ctx,0x8c61a10cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a10e; /* fmov.s fr2,@-r8 */
L8c61a10e: sa_motion_publish_stop(ctx,0x8c61a10eu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a110; /* fmov.s @(r0,r7),fr3 */
L8c61a110: sa_motion_publish_stop(ctx,0x8c61a110u); r[0]=20u;goto L8c61a112; /* mov #0x14,r0 */
L8c61a112: sa_motion_publish_stop(ctx,0x8c61a112u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a114; /* fmov.s fr3,@-r8 */
L8c61a114: sa_motion_publish_stop(ctx,0x8c61a114u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a116; /* fmov.s @(r0,r7),fr3 */
L8c61a116: sa_motion_publish_stop(ctx,0x8c61a116u); r[0]=16u;goto L8c61a118; /* mov #0x10,r0 */
L8c61a118: sa_motion_publish_stop(ctx,0x8c61a118u); r[3]=rd(m,bus,&io_status,0x8c61a24cu,4);if(io_status)return io_status;goto L8c61a11a; /* mov.l 0x8c61a24c,r3 */
L8c61a11a: sa_motion_publish_stop(ctx,0x8c61a11au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a11c; /* fmov.s fr3,@-r8 */
L8c61a11c: sa_motion_publish_stop(ctx,0x8c61a11cu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a11e; /* fmov.s @(r0,r7),fr3 */
L8c61a11e: sa_motion_publish_stop(ctx,0x8c61a11eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a120; /* fmov.s fr3,@-r8 */
L8c61a120: sa_motion_publish_stop(ctx,0x8c61a120u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a122; /* mov.l r3,@-r8 */
L8c61a122: sa_motion_publish_stop(ctx,0x8c61a122u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a124; /* pref @r8 */
L8c61a124: sa_motion_publish_stop(ctx,0x8c61a124u); r[3]=rd(m,bus,&io_status,0x8c61a240u,4);if(io_status)return io_status;goto L8c61a126; /* mov.l 0x8c61a240,r3 */
L8c61a126: sa_motion_publish_stop(ctx,0x8c61a126u); r[0]=4u;goto L8c61a128; /* mov #0x4,r0 */
L8c61a128: sa_motion_publish_stop(ctx,0x8c61a128u); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61a12a; /* fmov.s @r14,fr3 */
L8c61a12a: sa_motion_publish_stop(ctx,0x8c61a12au); r[8]+=60u;goto L8c61a12c; /* add #0x3c,r8 */
L8c61a12c: sa_motion_publish_stop(ctx,0x8c61a12cu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a12e; /* fmov.s @r3,fr2 */
L8c61a12e: sa_motion_publish_stop(ctx,0x8c61a12eu); r[3]+=8u;goto L8c61a130; /* add #0x8,r3 */
L8c61a130: sa_motion_publish_stop(ctx,0x8c61a130u); r[2]=rd(m,bus,&io_status,0x8c61a244u,4);if(io_status)return io_status;goto L8c61a132; /* mov.l 0x8c61a244,r2 */
L8c61a132: sa_motion_publish_stop(ctx,0x8c61a132u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a134; /* fmul fr3,fr2 */
L8c61a134: sa_motion_publish_stop(ctx,0x8c61a134u); r[1]=rd(m,bus,&io_status,0x8c61a248u,4);if(io_status)return io_status;goto L8c61a136; /* mov.l 0x8c61a248,r1 */
L8c61a136: sa_motion_publish_stop(ctx,0x8c61a136u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61a138; /* fmov.s @r2,fr3 */
L8c61a138: sa_motion_publish_stop(ctx,0x8c61a138u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61a13a; /* fmov.s @r1,fr1 */
L8c61a13a: sa_motion_publish_stop(ctx,0x8c61a13au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a13c; /* fsub fr2,fr3 */
L8c61a13c: sa_motion_publish_stop(ctx,0x8c61a13cu); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a13e; /* fmov.s @(r0,r14),fr2 */
L8c61a13e: sa_motion_publish_stop(ctx,0x8c61a13eu); r[0]=8u;goto L8c61a140; /* mov #0x8,r0 */
L8c61a140: sa_motion_publish_stop(ctx,0x8c61a140u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a142; /* fmul fr2,fr1 */
L8c61a142: sa_motion_publish_stop(ctx,0x8c61a142u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a144; /* fmov.s @(r0,r14),fr2 */
L8c61a144: sa_motion_publish_stop(ctx,0x8c61a144u); r[0]=4u;goto L8c61a146; /* mov #0x4,r0 */
L8c61a146: sa_motion_publish_stop(ctx,0x8c61a146u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a148; /* fsub fr1,fr3 */
L8c61a148: sa_motion_publish_stop(ctx,0x8c61a148u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a14a; /* fmov.s @r3,fr1 */
L8c61a14a: sa_motion_publish_stop(ctx,0x8c61a14au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a14c; /* fmul fr2,fr1 */
L8c61a14c: sa_motion_publish_stop(ctx,0x8c61a14cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a14e; /* fsub fr1,fr3 */
L8c61a14e: sa_motion_publish_stop(ctx,0x8c61a14eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a150; /* fmov.s fr3,@-r8 */
L8c61a150: sa_motion_publish_stop(ctx,0x8c61a150u); r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a152; /* mov.l @r9,r2 */
L8c61a152: sa_motion_publish_stop(ctx,0x8c61a152u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a154; /* fmov.s @(r0,r14),fr0 */
L8c61a154: sa_motion_publish_stop(ctx,0x8c61a154u); r[0]=20u;goto L8c61a156; /* mov #0x14,r0 */
L8c61a156: sa_motion_publish_stop(ctx,0x8c61a156u); r[1]=r[2];goto L8c61a158; /* mov r2,r1 */
L8c61a158: sa_motion_publish_stop(ctx,0x8c61a158u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a15a; /* fmov.s @(r0,r1),fr3 */
L8c61a15a: sa_motion_publish_stop(ctx,0x8c61a15au); r[0]=4u;goto L8c61a15c; /* mov #0x4,r0 */
L8c61a15c: sa_motion_publish_stop(ctx,0x8c61a15cu); r[3]=r[2];goto L8c61a15e; /* mov r2,r3 */
L8c61a15e: sa_motion_publish_stop(ctx,0x8c61a15eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a160; /* fmul fr0,fr3 */
L8c61a160: sa_motion_publish_stop(ctx,0x8c61a160u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a162; /* fmov.s @(r0,r3),fr2 */
L8c61a162: sa_motion_publish_stop(ctx,0x8c61a162u); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61a164; /* fmov.s @r14,fr0 */
L8c61a164: sa_motion_publish_stop(ctx,0x8c61a164u); r[0]=8u;goto L8c61a166; /* mov #0x8,r0 */
L8c61a166: sa_motion_publish_stop(ctx,0x8c61a166u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a168; /* fmac fr0,fr2,fr3 */
L8c61a168: sa_motion_publish_stop(ctx,0x8c61a168u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a16a; /* fmov.s @(r0,r14),fr0 */
L8c61a16a: sa_motion_publish_stop(ctx,0x8c61a16au); r[0]=36u;goto L8c61a16c; /* mov #0x24,r0 */
L8c61a16c: sa_motion_publish_stop(ctx,0x8c61a16cu); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a16e; /* fmov.s @(r0,r2),fr2 */
L8c61a16e: sa_motion_publish_stop(ctx,0x8c61a16eu); r[0]=4u;goto L8c61a170; /* mov #0x4,r0 */
L8c61a170: sa_motion_publish_stop(ctx,0x8c61a170u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a172; /* fmac fr0,fr2,fr3 */
L8c61a172: sa_motion_publish_stop(ctx,0x8c61a172u); fcopy(f,0u,6u);goto L8c61a174; /* fmov fr6,fr0 */
L8c61a174: sa_motion_publish_stop(ctx,0x8c61a174u); fcopy(f,2u,6u);goto L8c61a176; /* fmov fr6,fr2 */
L8c61a176: sa_motion_publish_stop(ctx,0x8c61a176u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a178; /* fmac fr0,fr3,fr2 */
L8c61a178: sa_motion_publish_stop(ctx,0x8c61a178u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a17a; /* fmov.s fr2,@-r8 */
L8c61a17a: sa_motion_publish_stop(ctx,0x8c61a17au); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a17c; /* mov.l @r9,r3 */
L8c61a17c: sa_motion_publish_stop(ctx,0x8c61a17cu); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a17e; /* fmov.s @(r0,r14),fr0 */
L8c61a17e: sa_motion_publish_stop(ctx,0x8c61a17eu); r[0]=16u;goto L8c61a180; /* mov #0x10,r0 */
L8c61a180: sa_motion_publish_stop(ctx,0x8c61a180u); r[1]=r[3];goto L8c61a182; /* mov r3,r1 */
L8c61a182: sa_motion_publish_stop(ctx,0x8c61a182u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a184; /* fmov.s @(r0,r1),fr3 */
L8c61a184: sa_motion_publish_stop(ctx,0x8c61a184u); r[2]=r[3];goto L8c61a186; /* mov r3,r2 */
L8c61a186: sa_motion_publish_stop(ctx,0x8c61a186u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61a188; /* fmov.s @r2,fr2 */
L8c61a188: sa_motion_publish_stop(ctx,0x8c61a188u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a18a; /* fmul fr0,fr3 */
L8c61a18a: sa_motion_publish_stop(ctx,0x8c61a18au); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61a18c; /* fmov.s @r14,fr0 */
L8c61a18c: sa_motion_publish_stop(ctx,0x8c61a18cu); r[0]=8u;goto L8c61a18e; /* mov #0x8,r0 */
L8c61a18e: sa_motion_publish_stop(ctx,0x8c61a18eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a190; /* fmac fr0,fr2,fr3 */
L8c61a190: sa_motion_publish_stop(ctx,0x8c61a190u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a192; /* fmov.s @(r0,r14),fr0 */
L8c61a192: sa_motion_publish_stop(ctx,0x8c61a192u); r[0]=32u;goto L8c61a194; /* mov #0x20,r0 */
L8c61a194: sa_motion_publish_stop(ctx,0x8c61a194u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a196; /* fmov.s @(r0,r3),fr2 */
L8c61a196: sa_motion_publish_stop(ctx,0x8c61a196u); r[0]=24u;goto L8c61a198; /* mov #0x18,r0 */
L8c61a198: sa_motion_publish_stop(ctx,0x8c61a198u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a19a; /* fmac fr0,fr2,fr3 */
L8c61a19a: sa_motion_publish_stop(ctx,0x8c61a19au); fcopy(f,0u,6u);goto L8c61a19c; /* fmov fr6,fr0 */
L8c61a19c: sa_motion_publish_stop(ctx,0x8c61a19cu); fcopy(f,2u,6u);goto L8c61a19e; /* fmov fr6,fr2 */
L8c61a19e: sa_motion_publish_stop(ctx,0x8c61a19eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a1a0; /* fmac fr0,fr3,fr2 */
L8c61a1a0: sa_motion_publish_stop(ctx,0x8c61a1a0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a1a2; /* fmov.s fr2,@-r8 */
L8c61a1a2: sa_motion_publish_stop(ctx,0x8c61a1a2u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a1a4; /* fmov.s @(r0,r14),fr3 */
L8c61a1a4: sa_motion_publish_stop(ctx,0x8c61a1a4u); r[0]=20u;goto L8c61a1a6; /* mov #0x14,r0 */
L8c61a1a6: sa_motion_publish_stop(ctx,0x8c61a1a6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a1a8; /* fmov.s fr3,@-r8 */
L8c61a1a8: sa_motion_publish_stop(ctx,0x8c61a1a8u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a1aa; /* fmov.s @(r0,r14),fr3 */
L8c61a1aa: sa_motion_publish_stop(ctx,0x8c61a1aau); r[0]=16u;goto L8c61a1ac; /* mov #0x10,r0 */
L8c61a1ac: sa_motion_publish_stop(ctx,0x8c61a1acu); r[3]=rd(m,bus,&io_status,0x8c61a250u,4);if(io_status)return io_status;goto L8c61a1ae; /* mov.l 0x8c61a250,r3 */
L8c61a1ae: sa_motion_publish_stop(ctx,0x8c61a1aeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a1b0; /* fmov.s fr3,@-r8 */
L8c61a1b0: sa_motion_publish_stop(ctx,0x8c61a1b0u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a1b2; /* fmov.s @(r0,r14),fr3 */
L8c61a1b2: sa_motion_publish_stop(ctx,0x8c61a1b2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a1b4; /* fmov.s fr3,@-r8 */
L8c61a1b4: sa_motion_publish_stop(ctx,0x8c61a1b4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a1b6; /* mov.l r3,@-r8 */
L8c61a1b6: sa_motion_publish_stop(ctx,0x8c61a1b6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a1b8; /* pref @r8 */
L8c61a1b8: sa_motion_publish_stop(ctx,0x8c61a1b8u); r[2]=rd(m,bus,&io_status,0x8c61a254u,4);if(io_status)return io_status;goto L8c61a1ba; /* mov.l 0x8c61a254,r2 */
L8c61a1ba: sa_motion_publish_stop(ctx,0x8c61a1bau); r[8]+=32u;goto L8c61a1bc; /* add #0x20,r8 */
L8c61a1bc: sa_motion_publish_stop(ctx,0x8c61a1bcu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a1be; /* mov.l @r2,r3 */
L8c61a1be: sa_motion_publish_stop(ctx,0x8c61a1beu); r[3]+=4u;goto L8c61a1c0; /* add #0x4,r3 */
L8c61a1c0: sa_motion_publish_stop(ctx,0x8c61a1c0u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61a1c2; /* mov.l r3,@r2 */
L8c61a1c2: sa_motion_publish_stop(ctx,0x8c61a1c2u); r[11]+=1u;goto L8c61a1c4; /* add #0x1,r11 */
L8c61a1c4: sa_motion_publish_stop(ctx,0x8c61a1c4u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61a1c6; /* mov.w @(0x2,r4),r0 */
L8c61a1c6: sa_motion_publish_stop(ctx,0x8c61a1c6u); r[0]=r[0]&65535u;goto L8c61a1c8; /* extu.w r0,r0 */
L8c61a1c8: sa_motion_publish_stop(ctx,0x8c61a1c8u); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61a1ca; /* cmp/ge r0,r11 */
L8c61a1ca: sa_motion_publish_stop(ctx,0x8c61a1cau); if((*sr&1u))goto L8c61a1d0;goto L8c61a1cc; /* bt 0x8c61a1d0 */
L8c61a1cc: sa_motion_publish_stop(ctx,0x8c61a1ceu); goto L8c619f04; /* bra 0x8c619f04 */
L8c61a1d0: sa_motion_publish_stop(ctx,0x8c61a1d2u); goto L8c61a446; /* bra 0x8c61a446 */
L8c61a1d4: sa_motion_publish_stop(ctx,0x8c61a1d6u); r[10]=r[12];goto L8c61a3be; /* bra 0x8c61a3be */
L8c61a1d8: sa_motion_publish_stop(ctx,0x8c61a1d8u); r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61a1da; /* mov.w @r13+,r14 */
L8c61a1da: sa_motion_publish_stop(ctx,0x8c61a1dau); r[3]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c61a1dc; /* mov.l @r11,r3 */
L8c61a1dc: sa_motion_publish_stop(ctx,0x8c61a1dcu); r[14]=r[14]&65535u;goto L8c61a1de; /* extu.w r14,r14 */
L8c61a1de: sa_motion_publish_stop(ctx,0x8c61a1deu); r[2]=rd(m,bus,&io_status,0x8c61a23cu,4);if(io_status)return io_status;goto L8c61a1e0; /* mov.l 0x8c61a23c,r2 */
L8c61a1e0: sa_motion_publish_stop(ctx,0x8c61a1e0u); r[3]+=r[14];goto L8c61a1e2; /* add r14,r3 */
L8c61a1e2: sa_motion_publish_stop(ctx,0x8c61a1e2u); wr(m,bus,&io_status,r[11],r[3],4);if(io_status)return io_status;goto L8c61a1e4; /* mov.l r3,@r11 */
L8c61a1e4: sa_motion_publish_stop(ctx,0x8c61a1e4u); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c61a1e6; /* mov.w @(0x2,r13),r0 */
L8c61a1e6: sa_motion_publish_stop(ctx,0x8c61a1e6u); r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c61a1e8; /* mov.w @r13,r7 */
L8c61a1e8: sa_motion_publish_stop(ctx,0x8c61a1e8u); r[5]=r[0]&65535u;goto L8c61a1ea; /* extu.w r0,r5 */
L8c61a1ea: sa_motion_publish_stop(ctx,0x8c61a1eau); r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c61a1ec; /* mov.w @(0x4,r13),r0 */
L8c61a1ec: sa_motion_publish_stop(ctx,0x8c61a1ecu); r[5]<<=2;goto L8c61a1ee; /* shll2 r5 */
L8c61a1ee: sa_motion_publish_stop(ctx,0x8c61a1eeu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a1f0; /* mov.l @r2,r3 */
L8c61a1f0: sa_motion_publish_stop(ctx,0x8c61a1f0u); r[7]=r[7]&65535u;goto L8c61a1f2; /* extu.w r7,r7 */
L8c61a1f2: sa_motion_publish_stop(ctx,0x8c61a1f2u); r[7]<<=2;goto L8c61a1f4; /* shll2 r7 */
L8c61a1f4: sa_motion_publish_stop(ctx,0x8c61a1f4u); r[5]<<=2;goto L8c61a1f6; /* shll2 r5 */
L8c61a1f6: sa_motion_publish_stop(ctx,0x8c61a1f6u); r[6]=r[0]&65535u;goto L8c61a1f8; /* extu.w r0,r6 */
L8c61a1f8: sa_motion_publish_stop(ctx,0x8c61a1f8u); r[7]<<=2;goto L8c61a1fa; /* shll2 r7 */
L8c61a1fa: sa_motion_publish_stop(ctx,0x8c61a1fau); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c61a1fc; /* shll r7 */
L8c61a1fc: sa_motion_publish_stop(ctx,0x8c61a1fcu); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61a1fe; /* shll r5 */
L8c61a1fe: sa_motion_publish_stop(ctx,0x8c61a1feu); r[0]=16u;goto L8c61a200; /* mov #0x10,r0 */
L8c61a200: sa_motion_publish_stop(ctx,0x8c61a200u); r[5]+=r[3];goto L8c61a202; /* add r3,r5 */
L8c61a202: sa_motion_publish_stop(ctx,0x8c61a202u); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a204; /* fmov.s @(r0,r5),fr5 */
L8c61a204: sa_motion_publish_stop(ctx,0x8c61a204u); r[7]+=r[3];goto L8c61a206; /* add r3,r7 */
L8c61a206: sa_motion_publish_stop(ctx,0x8c61a206u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a208; /* fmov.s @(r0,r7),fr3 */
L8c61a208: sa_motion_publish_stop(ctx,0x8c61a208u); r[6]<<=2;goto L8c61a20a; /* shll2 r6 */
L8c61a20a: sa_motion_publish_stop(ctx,0x8c61a20au); fcopy(f,2u,5u);goto L8c61a20c; /* fmov fr5,fr2 */
L8c61a20c: sa_motion_publish_stop(ctx,0x8c61a20cu); r[6]<<=2;goto L8c61a20e; /* shll2 r6 */
L8c61a20e: sa_motion_publish_stop(ctx,0x8c61a20eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61a210; /* fsub fr3,fr2 */
L8c61a210: sa_motion_publish_stop(ctx,0x8c61a210u); r[0]=20u;goto L8c61a212; /* mov #0x14,r0 */
L8c61a212: sa_motion_publish_stop(ctx,0x8c61a212u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61a214; /* shll r6 */
L8c61a214: sa_motion_publish_stop(ctx,0x8c61a214u); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a216; /* fmov.s @(r0,r5),fr4 */
L8c61a216: sa_motion_publish_stop(ctx,0x8c61a216u); r[6]+=r[3];goto L8c61a218; /* add r3,r6 */
L8c61a218: sa_motion_publish_stop(ctx,0x8c61a218u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a21a; /* fmov.s @(r0,r6),fr3 */
L8c61a21a: sa_motion_publish_stop(ctx,0x8c61a21au); fcopy(f,1u,4u);goto L8c61a21c; /* fmov fr4,fr1 */
L8c61a21c: sa_motion_publish_stop(ctx,0x8c61a21cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a21e; /* fsub fr4,fr3 */
L8c61a21e: sa_motion_publish_stop(ctx,0x8c61a21eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a220; /* fmul fr3,fr2 */
L8c61a220: sa_motion_publish_stop(ctx,0x8c61a220u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a222; /* fmov.s @(r0,r7),fr3 */
L8c61a222: sa_motion_publish_stop(ctx,0x8c61a222u); r[0]=16u;goto L8c61a224; /* mov #0x10,r0 */
L8c61a224: sa_motion_publish_stop(ctx,0x8c61a224u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61a226; /* fsub fr3,fr1 */
L8c61a226: sa_motion_publish_stop(ctx,0x8c61a226u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a228; /* fmov.s @(r0,r6),fr3 */
L8c61a228: sa_motion_publish_stop(ctx,0x8c61a228u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a22a; /* fsub fr5,fr3 */
L8c61a22a: sa_motion_publish_stop(ctx,0x8c61a22au); fcopy(f,4u,2u);goto L8c61a22c; /* fmov fr2,fr4 */
L8c61a22c: sa_motion_publish_stop(ctx,0x8c61a22cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a22e; /* fmul fr3,fr1 */
L8c61a22e: sa_motion_publish_stop(ctx,0x8c61a22eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61a230; /* fldi0 fr3 */
L8c61a230: sa_motion_publish_stop(ctx,0x8c61a230u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61a232; /* fsub fr1,fr4 */
L8c61a232: sa_motion_publish_stop(ctx,0x8c61a232u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61a234; /* fcmp/gt fr3,fr4 */
L8c61a234: sa_motion_publish_stop(ctx,0x8c61a234u); if(!(*sr&1u))goto L8c61a258;goto L8c61a236; /* bf 0x8c61a258 */
L8c61a236: sa_motion_publish_stop(ctx,0x8c61a236u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61a238; /* shll r14 */
L8c61a238: sa_motion_publish_stop(ctx,0x8c61a23au); r[13]+=r[14];goto L8c61a3bc; /* bra 0x8c61a3bc */
L8c61a258: sa_motion_publish_stop(ctx,0x8c61a258u); r[3]=rd(m,bus,&io_status,0x8c61a464u,4);if(io_status)return io_status;goto L8c61a25a; /* mov.l 0x8c61a464,r3 */
L8c61a25a: sa_motion_publish_stop(ctx,0x8c61a25au); r[7]=r[14];goto L8c61a25c; /* mov r14,r7 */
L8c61a25c: sa_motion_publish_stop(ctx,0x8c61a25cu); r[7]+=4294967295u;goto L8c61a25e; /* add #-0x1,r7 */
L8c61a25e: sa_motion_publish_stop(ctx,0x8c61a25eu); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a260; /* mov.l @r3,r2 */
L8c61a260: sa_motion_publish_stop(ctx,0x8c61a260u); *sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c61a262; /* cmp/pl r7 */
L8c61a262: sa_motion_publish_stop(ctx,0x8c61a262u); r[2]+=r[14];goto L8c61a264; /* add r14,r2 */
L8c61a264: sa_motion_publish_stop(ctx,0x8c61a264u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61a266; /* mov.l r2,@r3 */
L8c61a266: sa_motion_publish_stop(ctx,0x8c61a268u); take=!(*sr&1u);r[6]=r[12];if(take)goto L8c61a316;goto L8c61a26a; /* bf/s 0x8c61a316 */
L8c61a26a: sa_motion_publish_stop(ctx,0x8c61a26au); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61a26c; /* mov.w @r13+,r5 */
L8c61a26c: sa_motion_publish_stop(ctx,0x8c61a26cu); r[0]=4u;goto L8c61a26e; /* mov #0x4,r0 */
L8c61a26e: sa_motion_publish_stop(ctx,0x8c61a26eu); r[2]=rd(m,bus,&io_status,0x8c61a468u,4);if(io_status)return io_status;goto L8c61a270; /* mov.l 0x8c61a468,r2 */
L8c61a270: sa_motion_publish_stop(ctx,0x8c61a270u); r[8]+=28u;goto L8c61a272; /* add #0x1c,r8 */
L8c61a272: sa_motion_publish_stop(ctx,0x8c61a272u); r[5]=r[5]&65535u;goto L8c61a274; /* extu.w r5,r5 */
L8c61a274: sa_motion_publish_stop(ctx,0x8c61a274u); r[1]=rd(m,bus,&io_status,0x8c61a470u,4);if(io_status)return io_status;goto L8c61a276; /* mov.l 0x8c61a470,r1 */
L8c61a276: sa_motion_publish_stop(ctx,0x8c61a276u); r[5]<<=2;goto L8c61a278; /* shll2 r5 */
L8c61a278: sa_motion_publish_stop(ctx,0x8c61a278u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a27a; /* mov.l @r2,r3 */
L8c61a27a: sa_motion_publish_stop(ctx,0x8c61a27au); r[5]<<=2;goto L8c61a27c; /* shll2 r5 */
L8c61a27c: sa_motion_publish_stop(ctx,0x8c61a27cu); r[2]=rd(m,bus,&io_status,0x8c61a474u,4);if(io_status)return io_status;goto L8c61a27e; /* mov.l 0x8c61a474,r2 */
L8c61a27e: sa_motion_publish_stop(ctx,0x8c61a27eu); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61a280; /* shll r5 */
L8c61a280: sa_motion_publish_stop(ctx,0x8c61a280u); r[5]+=r[3];goto L8c61a282; /* add r3,r5 */
L8c61a282: sa_motion_publish_stop(ctx,0x8c61a282u); r[3]=rd(m,bus,&io_status,0x8c61a46cu,4);if(io_status)return io_status;goto L8c61a284; /* mov.l 0x8c61a46c,r3 */
L8c61a284: sa_motion_publish_stop(ctx,0x8c61a284u); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61a286; /* fmov.s @r5,fr3 */
L8c61a286: sa_motion_publish_stop(ctx,0x8c61a286u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a288; /* fmov.s @r3,fr2 */
L8c61a288: sa_motion_publish_stop(ctx,0x8c61a288u); r[3]+=8u;goto L8c61a28a; /* add #0x8,r3 */
L8c61a28a: sa_motion_publish_stop(ctx,0x8c61a28au); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61a28c; /* fmov.s @r2,fr1 */
L8c61a28c: sa_motion_publish_stop(ctx,0x8c61a28cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a28e; /* fmul fr3,fr2 */
L8c61a28e: sa_motion_publish_stop(ctx,0x8c61a28eu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61a290; /* fmov.s @r1,fr3 */
L8c61a290: sa_motion_publish_stop(ctx,0x8c61a290u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a292; /* fsub fr2,fr3 */
L8c61a292: sa_motion_publish_stop(ctx,0x8c61a292u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a294; /* fmov.s @(r0,r5),fr2 */
L8c61a294: sa_motion_publish_stop(ctx,0x8c61a294u); r[0]=8u;goto L8c61a296; /* mov #0x8,r0 */
L8c61a296: sa_motion_publish_stop(ctx,0x8c61a296u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a298; /* fmul fr2,fr1 */
L8c61a298: sa_motion_publish_stop(ctx,0x8c61a298u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a29a; /* fmov.s @(r0,r5),fr2 */
L8c61a29a: sa_motion_publish_stop(ctx,0x8c61a29au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a29c; /* fsub fr1,fr3 */
L8c61a29c: sa_motion_publish_stop(ctx,0x8c61a29cu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a29e; /* fmov.s @r3,fr1 */
L8c61a29e: sa_motion_publish_stop(ctx,0x8c61a29eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a2a0; /* fmul fr2,fr1 */
L8c61a2a0: sa_motion_publish_stop(ctx,0x8c61a2a0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a2a2; /* fsub fr1,fr3 */
L8c61a2a2: sa_motion_publish_stop(ctx,0x8c61a2a2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a2a4; /* fmov.s fr3,@-r8 */
L8c61a2a4: sa_motion_publish_stop(ctx,0x8c61a2a4u); r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a2a6; /* mov.l @r9,r2 */
L8c61a2a6: sa_motion_publish_stop(ctx,0x8c61a2a6u); r[1]=r[2];goto L8c61a2a8; /* mov r2,r1 */
L8c61a2a8: sa_motion_publish_stop(ctx,0x8c61a2a8u); r[3]=r[2];goto L8c61a2aa; /* mov r2,r3 */
L8c61a2aa: sa_motion_publish_stop(ctx,0x8c61a2aau); r[0]=4u;goto L8c61a2ac; /* mov #0x4,r0 */
L8c61a2ac: sa_motion_publish_stop(ctx,0x8c61a2acu); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a2ae; /* fmov.s @(r0,r5),fr0 */
L8c61a2ae: sa_motion_publish_stop(ctx,0x8c61a2aeu); r[0]=20u;goto L8c61a2b0; /* mov #0x14,r0 */
L8c61a2b0: sa_motion_publish_stop(ctx,0x8c61a2b0u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a2b2; /* fmov.s @(r0,r1),fr3 */
L8c61a2b2: sa_motion_publish_stop(ctx,0x8c61a2b2u); r[0]=4u;goto L8c61a2b4; /* mov #0x4,r0 */
L8c61a2b4: sa_motion_publish_stop(ctx,0x8c61a2b4u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a2b6; /* fmov.s @(r0,r3),fr2 */
L8c61a2b6: sa_motion_publish_stop(ctx,0x8c61a2b6u); r[0]=8u;goto L8c61a2b8; /* mov #0x8,r0 */
L8c61a2b8: sa_motion_publish_stop(ctx,0x8c61a2b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a2ba; /* fmul fr0,fr3 */
L8c61a2ba: sa_motion_publish_stop(ctx,0x8c61a2bau); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a2bc; /* fmov.s @r5,fr0 */
L8c61a2bc: sa_motion_publish_stop(ctx,0x8c61a2bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a2be; /* fmac fr0,fr2,fr3 */
L8c61a2be: sa_motion_publish_stop(ctx,0x8c61a2beu); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a2c0; /* fmov.s @(r0,r5),fr0 */
L8c61a2c0: sa_motion_publish_stop(ctx,0x8c61a2c0u); r[0]=36u;goto L8c61a2c2; /* mov #0x24,r0 */
L8c61a2c2: sa_motion_publish_stop(ctx,0x8c61a2c2u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a2c4; /* fmov.s @(r0,r2),fr2 */
L8c61a2c4: sa_motion_publish_stop(ctx,0x8c61a2c4u); r[0]=4u;goto L8c61a2c6; /* mov #0x4,r0 */
L8c61a2c6: sa_motion_publish_stop(ctx,0x8c61a2c6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a2c8; /* fmac fr0,fr2,fr3 */
L8c61a2c8: sa_motion_publish_stop(ctx,0x8c61a2c8u); fcopy(f,0u,6u);goto L8c61a2ca; /* fmov fr6,fr0 */
L8c61a2ca: sa_motion_publish_stop(ctx,0x8c61a2cau); fcopy(f,2u,6u);goto L8c61a2cc; /* fmov fr6,fr2 */
L8c61a2cc: sa_motion_publish_stop(ctx,0x8c61a2ccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a2ce; /* fmac fr0,fr3,fr2 */
L8c61a2ce: sa_motion_publish_stop(ctx,0x8c61a2ceu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a2d0; /* fmov.s fr2,@-r8 */
L8c61a2d0: sa_motion_publish_stop(ctx,0x8c61a2d0u); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a2d2; /* mov.l @r9,r3 */
L8c61a2d2: sa_motion_publish_stop(ctx,0x8c61a2d2u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a2d4; /* fmov.s @(r0,r5),fr0 */
L8c61a2d4: sa_motion_publish_stop(ctx,0x8c61a2d4u); r[0]=16u;goto L8c61a2d6; /* mov #0x10,r0 */
L8c61a2d6: sa_motion_publish_stop(ctx,0x8c61a2d6u); r[1]=r[3];goto L8c61a2d8; /* mov r3,r1 */
L8c61a2d8: sa_motion_publish_stop(ctx,0x8c61a2d8u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a2da; /* fmov.s @(r0,r1),fr3 */
L8c61a2da: sa_motion_publish_stop(ctx,0x8c61a2dau); r[2]=r[3];goto L8c61a2dc; /* mov r3,r2 */
L8c61a2dc: sa_motion_publish_stop(ctx,0x8c61a2dcu); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61a2de; /* fmov.s @r2,fr2 */
L8c61a2de: sa_motion_publish_stop(ctx,0x8c61a2deu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a2e0; /* fmul fr0,fr3 */
L8c61a2e0: sa_motion_publish_stop(ctx,0x8c61a2e0u); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a2e2; /* fmov.s @r5,fr0 */
L8c61a2e2: sa_motion_publish_stop(ctx,0x8c61a2e2u); r[0]=8u;goto L8c61a2e4; /* mov #0x8,r0 */
L8c61a2e4: sa_motion_publish_stop(ctx,0x8c61a2e4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a2e6; /* fmac fr0,fr2,fr3 */
L8c61a2e6: sa_motion_publish_stop(ctx,0x8c61a2e6u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a2e8; /* fmov.s @(r0,r5),fr0 */
L8c61a2e8: sa_motion_publish_stop(ctx,0x8c61a2e8u); r[0]=32u;goto L8c61a2ea; /* mov #0x20,r0 */
L8c61a2ea: sa_motion_publish_stop(ctx,0x8c61a2eau); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a2ec; /* fmov.s @(r0,r3),fr2 */
L8c61a2ec: sa_motion_publish_stop(ctx,0x8c61a2ecu); r[0]=24u;goto L8c61a2ee; /* mov #0x18,r0 */
L8c61a2ee: sa_motion_publish_stop(ctx,0x8c61a2eeu); r[3]=rd(m,bus,&io_status,0x8c61a478u,4);if(io_status)return io_status;goto L8c61a2f0; /* mov.l 0x8c61a478,r3 */
L8c61a2f0: sa_motion_publish_stop(ctx,0x8c61a2f0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a2f2; /* fmac fr0,fr2,fr3 */
L8c61a2f2: sa_motion_publish_stop(ctx,0x8c61a2f2u); fcopy(f,0u,6u);goto L8c61a2f4; /* fmov fr6,fr0 */
L8c61a2f4: sa_motion_publish_stop(ctx,0x8c61a2f4u); fcopy(f,2u,6u);goto L8c61a2f6; /* fmov fr6,fr2 */
L8c61a2f6: sa_motion_publish_stop(ctx,0x8c61a2f6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a2f8; /* fmac fr0,fr3,fr2 */
L8c61a2f8: sa_motion_publish_stop(ctx,0x8c61a2f8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a2fa; /* fmov.s fr2,@-r8 */
L8c61a2fa: sa_motion_publish_stop(ctx,0x8c61a2fau); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a2fc; /* fmov.s @(r0,r5),fr3 */
L8c61a2fc: sa_motion_publish_stop(ctx,0x8c61a2fcu); r[0]=20u;goto L8c61a2fe; /* mov #0x14,r0 */
L8c61a2fe: sa_motion_publish_stop(ctx,0x8c61a2feu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a300; /* fmov.s fr3,@-r8 */
L8c61a300: sa_motion_publish_stop(ctx,0x8c61a300u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a302; /* fmov.s @(r0,r5),fr3 */
L8c61a302: sa_motion_publish_stop(ctx,0x8c61a302u); r[0]=16u;goto L8c61a304; /* mov #0x10,r0 */
L8c61a304: sa_motion_publish_stop(ctx,0x8c61a304u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a306; /* fmov.s fr3,@-r8 */
L8c61a306: sa_motion_publish_stop(ctx,0x8c61a306u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a308; /* fmov.s @(r0,r5),fr3 */
L8c61a308: sa_motion_publish_stop(ctx,0x8c61a308u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a30a; /* fmov.s fr3,@-r8 */
L8c61a30a: sa_motion_publish_stop(ctx,0x8c61a30au); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a30c; /* mov.l r3,@-r8 */
L8c61a30c: sa_motion_publish_stop(ctx,0x8c61a30cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a30e; /* pref @r8 */
L8c61a30e: sa_motion_publish_stop(ctx,0x8c61a30eu); r[6]+=1u;goto L8c61a310; /* add #0x1,r6 */
L8c61a310: sa_motion_publish_stop(ctx,0x8c61a310u); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c61a312; /* cmp/ge r7,r6 */
L8c61a312: sa_motion_publish_stop(ctx,0x8c61a314u); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61a26a;goto L8c61a316; /* bf/s 0x8c61a26a */
L8c61a316: sa_motion_publish_stop(ctx,0x8c61a316u); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61a318; /* mov.w @r13+,r5 */
L8c61a318: sa_motion_publish_stop(ctx,0x8c61a318u); r[0]=4u;goto L8c61a31a; /* mov #0x4,r0 */
L8c61a31a: sa_motion_publish_stop(ctx,0x8c61a31au); r[2]=rd(m,bus,&io_status,0x8c61a468u,4);if(io_status)return io_status;goto L8c61a31c; /* mov.l 0x8c61a468,r2 */
L8c61a31c: sa_motion_publish_stop(ctx,0x8c61a31cu); r[8]+=28u;goto L8c61a31e; /* add #0x1c,r8 */
L8c61a31e: sa_motion_publish_stop(ctx,0x8c61a31eu); r[5]=r[5]&65535u;goto L8c61a320; /* extu.w r5,r5 */
L8c61a320: sa_motion_publish_stop(ctx,0x8c61a320u); r[1]=rd(m,bus,&io_status,0x8c61a470u,4);if(io_status)return io_status;goto L8c61a322; /* mov.l 0x8c61a470,r1 */
L8c61a322: sa_motion_publish_stop(ctx,0x8c61a322u); r[5]<<=2;goto L8c61a324; /* shll2 r5 */
L8c61a324: sa_motion_publish_stop(ctx,0x8c61a324u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a326; /* mov.l @r2,r3 */
L8c61a326: sa_motion_publish_stop(ctx,0x8c61a326u); r[5]<<=2;goto L8c61a328; /* shll2 r5 */
L8c61a328: sa_motion_publish_stop(ctx,0x8c61a328u); r[2]=rd(m,bus,&io_status,0x8c61a474u,4);if(io_status)return io_status;goto L8c61a32a; /* mov.l 0x8c61a474,r2 */
L8c61a32a: sa_motion_publish_stop(ctx,0x8c61a32au); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61a32c; /* shll r5 */
L8c61a32c: sa_motion_publish_stop(ctx,0x8c61a32cu); r[5]+=r[3];goto L8c61a32e; /* add r3,r5 */
L8c61a32e: sa_motion_publish_stop(ctx,0x8c61a32eu); r[3]=rd(m,bus,&io_status,0x8c61a46cu,4);if(io_status)return io_status;goto L8c61a330; /* mov.l 0x8c61a46c,r3 */
L8c61a330: sa_motion_publish_stop(ctx,0x8c61a330u); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61a332; /* fmov.s @r5,fr3 */
L8c61a332: sa_motion_publish_stop(ctx,0x8c61a332u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a334; /* fmov.s @r3,fr2 */
L8c61a334: sa_motion_publish_stop(ctx,0x8c61a334u); r[3]+=8u;goto L8c61a336; /* add #0x8,r3 */
L8c61a336: sa_motion_publish_stop(ctx,0x8c61a336u); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61a338; /* fmov.s @r2,fr1 */
L8c61a338: sa_motion_publish_stop(ctx,0x8c61a338u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a33a; /* fmul fr3,fr2 */
L8c61a33a: sa_motion_publish_stop(ctx,0x8c61a33au); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61a33c; /* fmov.s @r1,fr3 */
L8c61a33c: sa_motion_publish_stop(ctx,0x8c61a33cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a33e; /* fsub fr2,fr3 */
L8c61a33e: sa_motion_publish_stop(ctx,0x8c61a33eu); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a340; /* fmov.s @(r0,r5),fr2 */
L8c61a340: sa_motion_publish_stop(ctx,0x8c61a340u); r[0]=8u;goto L8c61a342; /* mov #0x8,r0 */
L8c61a342: sa_motion_publish_stop(ctx,0x8c61a342u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a344; /* fmul fr2,fr1 */
L8c61a344: sa_motion_publish_stop(ctx,0x8c61a344u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a346; /* fmov.s @(r0,r5),fr2 */
L8c61a346: sa_motion_publish_stop(ctx,0x8c61a346u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a348; /* fsub fr1,fr3 */
L8c61a348: sa_motion_publish_stop(ctx,0x8c61a348u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a34a; /* fmov.s @r3,fr1 */
L8c61a34a: sa_motion_publish_stop(ctx,0x8c61a34au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a34c; /* fmul fr2,fr1 */
L8c61a34c: sa_motion_publish_stop(ctx,0x8c61a34cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a34e; /* fsub fr1,fr3 */
L8c61a34e: sa_motion_publish_stop(ctx,0x8c61a34eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a350; /* fmov.s fr3,@-r8 */
L8c61a350: sa_motion_publish_stop(ctx,0x8c61a350u); r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a352; /* mov.l @r9,r2 */
L8c61a352: sa_motion_publish_stop(ctx,0x8c61a352u); r[1]=r[2];goto L8c61a354; /* mov r2,r1 */
L8c61a354: sa_motion_publish_stop(ctx,0x8c61a354u); r[3]=r[2];goto L8c61a356; /* mov r2,r3 */
L8c61a356: sa_motion_publish_stop(ctx,0x8c61a356u); r[0]=4u;goto L8c61a358; /* mov #0x4,r0 */
L8c61a358: sa_motion_publish_stop(ctx,0x8c61a358u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a35a; /* fmov.s @(r0,r5),fr0 */
L8c61a35a: sa_motion_publish_stop(ctx,0x8c61a35au); r[0]=20u;goto L8c61a35c; /* mov #0x14,r0 */
L8c61a35c: sa_motion_publish_stop(ctx,0x8c61a35cu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a35e; /* fmov.s @(r0,r1),fr3 */
L8c61a35e: sa_motion_publish_stop(ctx,0x8c61a35eu); r[0]=4u;goto L8c61a360; /* mov #0x4,r0 */
L8c61a360: sa_motion_publish_stop(ctx,0x8c61a360u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a362; /* fmov.s @(r0,r3),fr2 */
L8c61a362: sa_motion_publish_stop(ctx,0x8c61a362u); r[0]=8u;goto L8c61a364; /* mov #0x8,r0 */
L8c61a364: sa_motion_publish_stop(ctx,0x8c61a364u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a366; /* fmul fr0,fr3 */
L8c61a366: sa_motion_publish_stop(ctx,0x8c61a366u); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a368; /* fmov.s @r5,fr0 */
L8c61a368: sa_motion_publish_stop(ctx,0x8c61a368u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a36a; /* fmac fr0,fr2,fr3 */
L8c61a36a: sa_motion_publish_stop(ctx,0x8c61a36au); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a36c; /* fmov.s @(r0,r5),fr0 */
L8c61a36c: sa_motion_publish_stop(ctx,0x8c61a36cu); r[0]=36u;goto L8c61a36e; /* mov #0x24,r0 */
L8c61a36e: sa_motion_publish_stop(ctx,0x8c61a36eu); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a370; /* fmov.s @(r0,r2),fr2 */
L8c61a370: sa_motion_publish_stop(ctx,0x8c61a370u); r[0]=4u;goto L8c61a372; /* mov #0x4,r0 */
L8c61a372: sa_motion_publish_stop(ctx,0x8c61a372u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a374; /* fmac fr0,fr2,fr3 */
L8c61a374: sa_motion_publish_stop(ctx,0x8c61a374u); fcopy(f,0u,6u);goto L8c61a376; /* fmov fr6,fr0 */
L8c61a376: sa_motion_publish_stop(ctx,0x8c61a376u); fcopy(f,2u,6u);goto L8c61a378; /* fmov fr6,fr2 */
L8c61a378: sa_motion_publish_stop(ctx,0x8c61a378u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a37a; /* fmac fr0,fr3,fr2 */
L8c61a37a: sa_motion_publish_stop(ctx,0x8c61a37au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a37c; /* fmov.s fr2,@-r8 */
L8c61a37c: sa_motion_publish_stop(ctx,0x8c61a37cu); r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c61a37e; /* mov.l @r9,r3 */
L8c61a37e: sa_motion_publish_stop(ctx,0x8c61a37eu); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a380; /* fmov.s @(r0,r5),fr0 */
L8c61a380: sa_motion_publish_stop(ctx,0x8c61a380u); r[0]=16u;goto L8c61a382; /* mov #0x10,r0 */
L8c61a382: sa_motion_publish_stop(ctx,0x8c61a382u); r[1]=r[3];goto L8c61a384; /* mov r3,r1 */
L8c61a384: sa_motion_publish_stop(ctx,0x8c61a384u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a386; /* fmov.s @(r0,r1),fr3 */
L8c61a386: sa_motion_publish_stop(ctx,0x8c61a386u); r[2]=r[3];goto L8c61a388; /* mov r3,r2 */
L8c61a388: sa_motion_publish_stop(ctx,0x8c61a388u); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61a38a; /* fmov.s @r2,fr2 */
L8c61a38a: sa_motion_publish_stop(ctx,0x8c61a38au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a38c; /* fmul fr0,fr3 */
L8c61a38c: sa_motion_publish_stop(ctx,0x8c61a38cu); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a38e; /* fmov.s @r5,fr0 */
L8c61a38e: sa_motion_publish_stop(ctx,0x8c61a38eu); r[0]=8u;goto L8c61a390; /* mov #0x8,r0 */
L8c61a390: sa_motion_publish_stop(ctx,0x8c61a390u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a392; /* fmac fr0,fr2,fr3 */
L8c61a392: sa_motion_publish_stop(ctx,0x8c61a392u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a394; /* fmov.s @(r0,r5),fr0 */
L8c61a394: sa_motion_publish_stop(ctx,0x8c61a394u); r[0]=32u;goto L8c61a396; /* mov #0x20,r0 */
L8c61a396: sa_motion_publish_stop(ctx,0x8c61a396u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a398; /* fmov.s @(r0,r3),fr2 */
L8c61a398: sa_motion_publish_stop(ctx,0x8c61a398u); r[0]=24u;goto L8c61a39a; /* mov #0x18,r0 */
L8c61a39a: sa_motion_publish_stop(ctx,0x8c61a39au); r[3]=rd(m,bus,&io_status,0x8c61a47cu,4);if(io_status)return io_status;goto L8c61a39c; /* mov.l 0x8c61a47c,r3 */
L8c61a39c: sa_motion_publish_stop(ctx,0x8c61a39cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a39e; /* fmac fr0,fr2,fr3 */
L8c61a39e: sa_motion_publish_stop(ctx,0x8c61a39eu); fcopy(f,0u,6u);goto L8c61a3a0; /* fmov fr6,fr0 */
L8c61a3a0: sa_motion_publish_stop(ctx,0x8c61a3a0u); fcopy(f,2u,6u);goto L8c61a3a2; /* fmov fr6,fr2 */
L8c61a3a2: sa_motion_publish_stop(ctx,0x8c61a3a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a3a4; /* fmac fr0,fr3,fr2 */
L8c61a3a4: sa_motion_publish_stop(ctx,0x8c61a3a4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a3a6; /* fmov.s fr2,@-r8 */
L8c61a3a6: sa_motion_publish_stop(ctx,0x8c61a3a6u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a3a8; /* fmov.s @(r0,r5),fr3 */
L8c61a3a8: sa_motion_publish_stop(ctx,0x8c61a3a8u); r[0]=20u;goto L8c61a3aa; /* mov #0x14,r0 */
L8c61a3aa: sa_motion_publish_stop(ctx,0x8c61a3aau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a3ac; /* fmov.s fr3,@-r8 */
L8c61a3ac: sa_motion_publish_stop(ctx,0x8c61a3acu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a3ae; /* fmov.s @(r0,r5),fr3 */
L8c61a3ae: sa_motion_publish_stop(ctx,0x8c61a3aeu); r[0]=16u;goto L8c61a3b0; /* mov #0x10,r0 */
L8c61a3b0: sa_motion_publish_stop(ctx,0x8c61a3b0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a3b2; /* fmov.s fr3,@-r8 */
L8c61a3b2: sa_motion_publish_stop(ctx,0x8c61a3b2u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a3b4; /* fmov.s @(r0,r5),fr3 */
L8c61a3b4: sa_motion_publish_stop(ctx,0x8c61a3b4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a3b6; /* fmov.s fr3,@-r8 */
L8c61a3b6: sa_motion_publish_stop(ctx,0x8c61a3b6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a3b8; /* mov.l r3,@-r8 */
L8c61a3b8: sa_motion_publish_stop(ctx,0x8c61a3b8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a3ba; /* pref @r8 */
L8c61a3ba: sa_motion_publish_stop(ctx,0x8c61a3bau); r[8]+=32u;goto L8c61a3bc; /* add #0x20,r8 */
L8c61a3bc: sa_motion_publish_stop(ctx,0x8c61a3bcu); r[10]+=1u;goto L8c61a3be; /* add #0x1,r10 */
L8c61a3be: sa_motion_publish_stop(ctx,0x8c61a3beu); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c61a3c0; /* mov.w @(0x2,r4),r0 */
L8c61a3c0: sa_motion_publish_stop(ctx,0x8c61a3c0u); r[0]=r[0]&65535u;goto L8c61a3c2; /* extu.w r0,r0 */
L8c61a3c2: sa_motion_publish_stop(ctx,0x8c61a3c2u); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c61a3c4; /* cmp/ge r0,r10 */
L8c61a3c4: sa_motion_publish_stop(ctx,0x8c61a3c4u); if((*sr&1u))goto L8c61a3ca;goto L8c61a3c6; /* bt 0x8c61a3ca */
L8c61a3c6: sa_motion_publish_stop(ctx,0x8c61a3c8u); goto L8c61a1d8; /* bra 0x8c61a1d8 */
L8c61a3ca: sa_motion_publish_stop(ctx,0x8c61a3ccu); goto L8c61a446; /* bra 0x8c61a446 */
L8c61a3ce: sa_motion_publish_stop(ctx,0x8c61a3ceu); r[1]=rd(m,bus,&io_status,0x8c61a480u,4);if(io_status)return io_status;goto L8c61a3d0; /* mov.l 0x8c61a480,r1 */
L8c61a3d0: sa_motion_publish_stop(ctx,0x8c61a3d0u); wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c61a3d2; /* mov.l r4,@r1 */
L8c61a3d2: sa_motion_publish_stop(ctx,0x8c61a3d2u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61a3d4; /* frchg */
L8c61a3d4: sa_motion_publish_stop(ctx,0x8c61a3d4u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61a3d6; /* fmov.s fr0,@-r15 */
L8c61a3d6: sa_motion_publish_stop(ctx,0x8c61a3d6u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61a3d8; /* fmov.s fr1,@-r15 */
L8c61a3d8: sa_motion_publish_stop(ctx,0x8c61a3d8u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61a3da; /* fmov.s fr2,@-r15 */
L8c61a3da: sa_motion_publish_stop(ctx,0x8c61a3dau); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61a3dc; /* fmov.s fr3,@-r15 */
L8c61a3dc: sa_motion_publish_stop(ctx,0x8c61a3dcu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61a3de; /* fmov.s fr4,@-r15 */
L8c61a3de: sa_motion_publish_stop(ctx,0x8c61a3deu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61a3e0; /* fmov.s fr5,@-r15 */
L8c61a3e0: sa_motion_publish_stop(ctx,0x8c61a3e0u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61a3e2; /* fmov.s fr6,@-r15 */
L8c61a3e2: sa_motion_publish_stop(ctx,0x8c61a3e2u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61a3e4; /* fmov.s fr7,@-r15 */
L8c61a3e4: sa_motion_publish_stop(ctx,0x8c61a3e4u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61a3e6; /* fmov.s fr8,@-r15 */
L8c61a3e6: sa_motion_publish_stop(ctx,0x8c61a3e6u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61a3e8; /* fmov.s fr9,@-r15 */
L8c61a3e8: sa_motion_publish_stop(ctx,0x8c61a3e8u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c61a3ea; /* fmov.s fr10,@-r15 */
L8c61a3ea: sa_motion_publish_stop(ctx,0x8c61a3eau); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c61a3ec; /* fmov.s fr11,@-r15 */
L8c61a3ec: sa_motion_publish_stop(ctx,0x8c61a3ecu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c61a3ee; /* fmov.s fr12,@-r15 */
L8c61a3ee: sa_motion_publish_stop(ctx,0x8c61a3eeu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61a3f0; /* fmov.s fr13,@-r15 */
L8c61a3f0: sa_motion_publish_stop(ctx,0x8c61a3f0u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61a3f2; /* fmov.s fr14,@-r15 */
L8c61a3f2: sa_motion_publish_stop(ctx,0x8c61a3f2u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61a3f4; /* fmov.s fr15,@-r15 */
L8c61a3f4: sa_motion_publish_stop(ctx,0x8c61a3f4u); r[0]=0x8c61a458u;goto L8c61a3f6; /* mova 0x8c61a458,r0 */
L8c61a3f6: sa_motion_publish_stop(ctx,0x8c61a3f6u); fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c61a3f8; /* fmov.s @r0,fr12 */
L8c61a3f8: sa_motion_publish_stop(ctx,0x8c61a3f8u); fcopy(f,13u,12u);goto L8c61a3fa; /* fmov fr12,fr13 */
L8c61a3fa: sa_motion_publish_stop(ctx,0x8c61a3fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[12],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61a3fc; /* fmul fr12,fr0 */
L8c61a3fc: sa_motion_publish_stop(ctx,0x8c61a3fcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61a3fe; /* fmul fr12,fr4 */
L8c61a3fe: sa_motion_publish_stop(ctx,0x8c61a3feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[12],&f->fpscr,&fr[8]);if(status)return status;}goto L8c61a400; /* fmul fr12,fr8 */
L8c61a400: sa_motion_publish_stop(ctx,0x8c61a400u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[12],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a402; /* fmul fr12,fr1 */
L8c61a402: sa_motion_publish_stop(ctx,0x8c61a402u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[12],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61a404; /* fmul fr12,fr5 */
L8c61a404: sa_motion_publish_stop(ctx,0x8c61a404u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[12],&f->fpscr,&fr[9]);if(status)return status;}goto L8c61a406; /* fmul fr12,fr9 */
L8c61a406: sa_motion_publish_stop(ctx,0x8c61a406u); r[0]=rd(m,bus,&io_status,0x8c61a45cu,4);if(io_status)return io_status;goto L8c61a408; /* mov.l 0x8c61a45c,r0 */
L8c61a408: sa_motion_publish_stop(ctx,0x8c61a408u); fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a40a; /* fmov.s @r0+,fr2 */
L8c61a40a: sa_motion_publish_stop(ctx,0x8c61a40au); fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a40c; /* fmov.s @r0+,fr6 */
L8c61a40c: sa_motion_publish_stop(ctx,0x8c61a40cu); fload(m,bus,&io_status,f,10u,r[0]);if(io_status)return io_status;goto L8c61a40e; /* fmov.s @r0,fr10 */
L8c61a40e: sa_motion_publish_stop(ctx,0x8c61a40eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c61a410; /* fneg fr2 */
L8c61a410: sa_motion_publish_stop(ctx,0x8c61a410u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]^=0x80000000u;goto L8c61a412; /* fneg fr6 */
L8c61a412: sa_motion_publish_stop(ctx,0x8c61a412u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]^=0x80000000u;goto L8c61a414; /* fneg fr10 */
L8c61a414: sa_motion_publish_stop(ctx,0x8c61a414u); r[0]=rd(m,bus,&io_status,0x8c61a460u,4);if(io_status)return io_status;goto L8c61a416; /* mov.l 0x8c61a460,r0 */
L8c61a416: sa_motion_publish_stop(ctx,0x8c61a416u); fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c61a418; /* fmov.s @r0,fr14 */
L8c61a418: sa_motion_publish_stop(ctx,0x8c61a418u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61a41a; /* frchg */
L8c61a41a: sa_motion_publish_stop(ctx,0x8c61a41au); r[2]=rd(m,bus,&io_status,0x8c61a484u,4);if(io_status)return io_status;goto L8c61a41c; /* mov.l 0x8c61a484,r2 */
L8c61a41c: sa_motion_publish_stop(ctx,0x8c61a41cu); r[3]=rd(m,bus,&io_status,0x8c61a480u,4);if(io_status)return io_status;goto L8c61a41e; /* mov.l 0x8c61a480,r3 */
L8c61a41e: sa_motion_publish_stop(ctx,0x8c61a420u); target=r[2];*pr=0x8c61a422u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61a422; /* jsr @r2 */
L8c61a422: sa_motion_publish_stop(ctx,0x8c61a422u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61a424; /* frchg */
L8c61a424: sa_motion_publish_stop(ctx,0x8c61a424u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a426; /* fmov.s @r15+,fr15 */
L8c61a426: sa_motion_publish_stop(ctx,0x8c61a426u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a428; /* fmov.s @r15+,fr14 */
L8c61a428: sa_motion_publish_stop(ctx,0x8c61a428u); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a42a; /* fmov.s @r15+,fr13 */
L8c61a42a: sa_motion_publish_stop(ctx,0x8c61a42au); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a42c; /* fmov.s @r15+,fr12 */
L8c61a42c: sa_motion_publish_stop(ctx,0x8c61a42cu); fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a42e; /* fmov.s @r15+,fr11 */
L8c61a42e: sa_motion_publish_stop(ctx,0x8c61a42eu); fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a430; /* fmov.s @r15+,fr10 */
L8c61a430: sa_motion_publish_stop(ctx,0x8c61a430u); fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a432; /* fmov.s @r15+,fr9 */
L8c61a432: sa_motion_publish_stop(ctx,0x8c61a432u); fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a434; /* fmov.s @r15+,fr8 */
L8c61a434: sa_motion_publish_stop(ctx,0x8c61a434u); fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a436; /* fmov.s @r15+,fr7 */
L8c61a436: sa_motion_publish_stop(ctx,0x8c61a436u); fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a438; /* fmov.s @r15+,fr6 */
L8c61a438: sa_motion_publish_stop(ctx,0x8c61a438u); fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a43a; /* fmov.s @r15+,fr5 */
L8c61a43a: sa_motion_publish_stop(ctx,0x8c61a43au); fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a43c; /* fmov.s @r15+,fr4 */
L8c61a43c: sa_motion_publish_stop(ctx,0x8c61a43cu); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a43e; /* fmov.s @r15+,fr3 */
L8c61a43e: sa_motion_publish_stop(ctx,0x8c61a43eu); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a440; /* fmov.s @r15+,fr2 */
L8c61a440: sa_motion_publish_stop(ctx,0x8c61a440u); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a442; /* fmov.s @r15+,fr1 */
L8c61a442: sa_motion_publish_stop(ctx,0x8c61a442u); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a444; /* fmov.s @r15+,fr0 */
L8c61a444: sa_motion_publish_stop(ctx,0x8c61a444u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61a446; /* frchg */
L8c61a446: sa_motion_publish_stop(ctx,0x8c61a446u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61a448; /* lds.l @r15+,PR */
L8c61a448: sa_motion_publish_stop(ctx,0x8c61a448u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61a44a; /* mov.l @r15+,r9 */
L8c61a44a: sa_motion_publish_stop(ctx,0x8c61a44au); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61a44c; /* mov.l @r15+,r10 */
L8c61a44c: sa_motion_publish_stop(ctx,0x8c61a44cu); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61a44e; /* mov.l @r15+,r11 */
L8c61a44e: sa_motion_publish_stop(ctx,0x8c61a44eu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61a450; /* mov.l @r15+,r12 */
L8c61a450: sa_motion_publish_stop(ctx,0x8c61a450u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61a452; /* mov.l @r15+,r13 */
L8c61a452: sa_motion_publish_stop(ctx,0x8c61a454u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_environment_primitive_clipped_8c61a488(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61a488;
L8c61a488: sa_motion_publish_stop(ctx,0x8c61a488u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61a48a; /* mov.l r14,@-r15 */
L8c61a48a: sa_motion_publish_stop(ctx,0x8c61a48au); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61a48c; /* mov.l r13,@-r15 */
L8c61a48c: sa_motion_publish_stop(ctx,0x8c61a48cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61a48e; /* mov.l r12,@-r15 */
L8c61a48e: sa_motion_publish_stop(ctx,0x8c61a48eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61a490; /* mov.l r11,@-r15 */
L8c61a490: sa_motion_publish_stop(ctx,0x8c61a490u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61a492; /* mov.l r10,@-r15 */
L8c61a492: sa_motion_publish_stop(ctx,0x8c61a492u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61a494; /* mov.l r9,@-r15 */
L8c61a494: sa_motion_publish_stop(ctx,0x8c61a494u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61a496; /* fmov.s fr15,@-r15 */
L8c61a496: sa_motion_publish_stop(ctx,0x8c61a496u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61a498; /* sts.l PR,@-r15 */
L8c61a498: sa_motion_publish_stop(ctx,0x8c61a498u); r[15]+=4294967288u;goto L8c61a49a; /* add #-0x8,r15 */
L8c61a49a: sa_motion_publish_stop(ctx,0x8c61a49au); r[0]=0x8c61a4f0u;goto L8c61a49c; /* mova 0x8c61a4f0,r0 */
L8c61a49c: sa_motion_publish_stop(ctx,0x8c61a49cu); r[3]=rd(m,bus,&io_status,0x8c61a4f4u,4);if(io_status)return io_status;goto L8c61a49e; /* mov.l 0x8c61a4f4,r3 */
L8c61a49e: sa_motion_publish_stop(ctx,0x8c61a49eu); fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c61a4a0; /* fmov.s @r0,fr15 */
L8c61a4a0: sa_motion_publish_stop(ctx,0x8c61a4a0u); r[0]=r[4];goto L8c61a4a2; /* mov r4,r0 */
L8c61a4a2: sa_motion_publish_stop(ctx,0x8c61a4a2u); wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61a4a4; /* mov.l r4,@r15 */
L8c61a4a4: sa_motion_publish_stop(ctx,0x8c61a4a4u); r[11]=r[4];goto L8c61a4a6; /* mov r4,r11 */
L8c61a4a6: sa_motion_publish_stop(ctx,0x8c61a4a6u); r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61a4a8; /* mov.w @r0,r0 */
L8c61a4a8: sa_motion_publish_stop(ctx,0x8c61a4a8u); r[0]=r[0]&65535u;goto L8c61a4aa; /* extu.w r0,r0 */
L8c61a4aa: sa_motion_publish_stop(ctx,0x8c61a4aau); r[0]&=r[3];goto L8c61a4ac; /* and r3,r0 */
L8c61a4ac: sa_motion_publish_stop(ctx,0x8c61a4acu); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61a4ae; /* cmp/eq #0x0,r0 */
L8c61a4ae: sa_motion_publish_stop(ctx,0x8c61a4b0u); take=(*sr&1u);r[11]=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;if(take)goto L8c61a4d4;goto L8c61a4b2; /* bt/s 0x8c61a4d4 */
L8c61a4b2: sa_motion_publish_stop(ctx,0x8c61a4b2u); r[1]=sx(m,bus,&io_status,0x8c61a4ecu,2);if(io_status)return io_status;goto L8c61a4b4; /* mov.w 0x8c61a4ec,r1 */
L8c61a4b4: sa_motion_publish_stop(ctx,0x8c61a4b4u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61a4b6; /* cmp/eq r1,r0 */
L8c61a4b6: sa_motion_publish_stop(ctx,0x8c61a4b6u); if(!(*sr&1u))goto L8c61a4bc;goto L8c61a4b8; /* bf 0x8c61a4bc */
L8c61a4b8: sa_motion_publish_stop(ctx,0x8c61a4bau); goto L8c61a752; /* bra 0x8c61a752 */
L8c61a4bc: sa_motion_publish_stop(ctx,0x8c61a4bcu); r[1]=rd(m,bus,&io_status,0x8c61a4f8u,4);if(io_status)return io_status;goto L8c61a4be; /* mov.l 0x8c61a4f8,r1 */
L8c61a4be: sa_motion_publish_stop(ctx,0x8c61a4beu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61a4c0; /* cmp/eq r1,r0 */
L8c61a4c0: sa_motion_publish_stop(ctx,0x8c61a4c0u); if(!(*sr&1u))goto L8c61a4c6;goto L8c61a4c2; /* bf 0x8c61a4c6 */
L8c61a4c2: sa_motion_publish_stop(ctx,0x8c61a4c4u); goto L8c61aace; /* bra 0x8c61aace */
L8c61a4c6: sa_motion_publish_stop(ctx,0x8c61a4c6u); r[1]=rd(m,bus,&io_status,0x8c61a4f4u,4);if(io_status)return io_status;goto L8c61a4c8; /* mov.l 0x8c61a4f4,r1 */
L8c61a4c8: sa_motion_publish_stop(ctx,0x8c61a4c8u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61a4ca; /* cmp/eq r1,r0 */
L8c61a4ca: sa_motion_publish_stop(ctx,0x8c61a4cau); if(!(*sr&1u))goto L8c61a4d0;goto L8c61a4cc; /* bf 0x8c61a4d0 */
L8c61a4cc: sa_motion_publish_stop(ctx,0x8c61a4ceu); goto L8c61ad6e; /* bra 0x8c61ad6e */
L8c61a4d0: sa_motion_publish_stop(ctx,0x8c61a4d2u); goto L8c61ade8; /* bra 0x8c61ade8 */
L8c61a4d4: sa_motion_publish_stop(ctx,0x8c61a4d4u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61a4d6; /* mov.l @r15,r2 */
L8c61a4d6: sa_motion_publish_stop(ctx,0x8c61a4d6u); r[1]=rd(m,bus,&io_status,0x8c61a4fcu,4);if(io_status)return io_status;goto L8c61a4d8; /* mov.l 0x8c61a4fc,r1 */
L8c61a4d8: sa_motion_publish_stop(ctx,0x8c61a4d8u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61a4da; /* mov.w @(0x2,r2),r0 */
L8c61a4da: sa_motion_publish_stop(ctx,0x8c61a4dau); r[0]=r[0]&65535u;goto L8c61a4dc; /* extu.w r0,r0 */
L8c61a4dc: sa_motion_publish_stop(ctx,0x8c61a4dcu); r[2]=r[0];goto L8c61a4de; /* mov r0,r2 */
L8c61a4de: sa_motion_publish_stop(ctx,0x8c61a4deu); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61a4e0; /* shll r0 */
L8c61a4e0: sa_motion_publish_stop(ctx,0x8c61a4e0u); r[0]+=r[2];goto L8c61a4e2; /* add r2,r0 */
L8c61a4e2: sa_motion_publish_stop(ctx,0x8c61a4e2u); r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61a4e4; /* mov.l @r1,r2 */
L8c61a4e4: sa_motion_publish_stop(ctx,0x8c61a4e4u); r[2]+=r[0];goto L8c61a4e6; /* add r0,r2 */
L8c61a4e6: sa_motion_publish_stop(ctx,0x8c61a4e6u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61a4e8; /* mov.l r2,@r1 */
L8c61a4e8: sa_motion_publish_stop(ctx,0x8c61a4eau); r[14]=0u;goto L8c61a740; /* bra 0x8c61a740 */
L8c61a500: sa_motion_publish_stop(ctx,0x8c61a500u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61a502; /* mov.w @r11+,r4 */
L8c61a502: sa_motion_publish_stop(ctx,0x8c61a502u); r[3]=rd(m,bus,&io_status,0x8c61a76cu,4);if(io_status)return io_status;goto L8c61a504; /* mov.l 0x8c61a76c,r3 */
L8c61a504: sa_motion_publish_stop(ctx,0x8c61a504u); r[5]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61a506; /* mov.w @r11+,r5 */
L8c61a506: sa_motion_publish_stop(ctx,0x8c61a506u); r[4]=r[4]&65535u;goto L8c61a508; /* extu.w r4,r4 */
L8c61a508: sa_motion_publish_stop(ctx,0x8c61a508u); r[4]<<=2;goto L8c61a50a; /* shll2 r4 */
L8c61a50a: sa_motion_publish_stop(ctx,0x8c61a50au); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a50c; /* mov.l @r3,r13 */
L8c61a50c: sa_motion_publish_stop(ctx,0x8c61a50cu); r[6]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61a50e; /* mov.w @r11+,r6 */
L8c61a50e: sa_motion_publish_stop(ctx,0x8c61a50eu); r[5]=r[5]&65535u;goto L8c61a510; /* extu.w r5,r5 */
L8c61a510: sa_motion_publish_stop(ctx,0x8c61a510u); r[5]<<=2;goto L8c61a512; /* shll2 r5 */
L8c61a512: sa_motion_publish_stop(ctx,0x8c61a512u); r[6]=r[6]&65535u;goto L8c61a514; /* extu.w r6,r6 */
L8c61a514: sa_motion_publish_stop(ctx,0x8c61a514u); r[5]<<=2;goto L8c61a516; /* shll2 r5 */
L8c61a516: sa_motion_publish_stop(ctx,0x8c61a516u); r[6]<<=2;goto L8c61a518; /* shll2 r6 */
L8c61a518: sa_motion_publish_stop(ctx,0x8c61a518u); r[4]<<=2;goto L8c61a51a; /* shll2 r4 */
L8c61a51a: sa_motion_publish_stop(ctx,0x8c61a51au); r[6]<<=2;goto L8c61a51c; /* shll2 r6 */
L8c61a51c: sa_motion_publish_stop(ctx,0x8c61a51cu); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61a51e; /* shll r4 */
L8c61a51e: sa_motion_publish_stop(ctx,0x8c61a51eu); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61a520; /* shll r5 */
L8c61a520: sa_motion_publish_stop(ctx,0x8c61a520u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61a522; /* shll r6 */
L8c61a522: sa_motion_publish_stop(ctx,0x8c61a522u); r[4]+=r[13];goto L8c61a524; /* add r13,r4 */
L8c61a524: sa_motion_publish_stop(ctx,0x8c61a524u); r[5]+=r[13];goto L8c61a526; /* add r13,r5 */
L8c61a526: sa_motion_publish_stop(ctx,0x8c61a526u); r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61a528; /* mov.l @(0xc,r4),r7 */
L8c61a528: sa_motion_publish_stop(ctx,0x8c61a528u); r[2]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c61a52a; /* mov.l @(0xc,r5),r2 */
L8c61a52a: sa_motion_publish_stop(ctx,0x8c61a52au); r[6]+=r[13];goto L8c61a52c; /* add r13,r6 */
L8c61a52c: sa_motion_publish_stop(ctx,0x8c61a52cu); r[1]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c61a52e; /* mov.l @(0xc,r6),r1 */
L8c61a52e: sa_motion_publish_stop(ctx,0x8c61a52eu); r[7]&=r[2];goto L8c61a530; /* and r2,r7 */
L8c61a530: sa_motion_publish_stop(ctx,0x8c61a530u); r[2]=63u;goto L8c61a532; /* mov #0x3f,r2 */
L8c61a532: sa_motion_publish_stop(ctx,0x8c61a532u); r[7]&=r[1];goto L8c61a534; /* and r1,r7 */
L8c61a534: sa_motion_publish_stop(ctx,0x8c61a534u); *sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c61a536; /* tst r2,r7 */
L8c61a536: sa_motion_publish_stop(ctx,0x8c61a536u); if((*sr&1u))goto L8c61a53c;goto L8c61a538; /* bt 0x8c61a53c */
L8c61a538: sa_motion_publish_stop(ctx,0x8c61a53au); goto L8c61a73e; /* bra 0x8c61a73e */
L8c61a53c: sa_motion_publish_stop(ctx,0x8c61a53cu); r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61a53e; /* mov.l @(0xc,r4),r7 */
L8c61a53e: sa_motion_publish_stop(ctx,0x8c61a53eu); r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c61a540; /* mov.l @(0xc,r5),r3 */
L8c61a540: sa_motion_publish_stop(ctx,0x8c61a540u); r[2]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c61a542; /* mov.l @(0xc,r6),r2 */
L8c61a542: sa_motion_publish_stop(ctx,0x8c61a542u); r[7]|=r[3];goto L8c61a544; /* or r3,r7 */
L8c61a544: sa_motion_publish_stop(ctx,0x8c61a544u); r[3]=1u;goto L8c61a546; /* mov #0x1,r3 */
L8c61a546: sa_motion_publish_stop(ctx,0x8c61a546u); r[7]|=r[2];goto L8c61a548; /* or r2,r7 */
L8c61a548: sa_motion_publish_stop(ctx,0x8c61a548u); *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c61a54a; /* tst r3,r7 */
L8c61a54a: sa_motion_publish_stop(ctx,0x8c61a54au); if((*sr&1u))goto L8c61a550;goto L8c61a54c; /* bt 0x8c61a550 */
L8c61a54c: sa_motion_publish_stop(ctx,0x8c61a54eu); goto L8c61a734; /* bra 0x8c61a734 */
L8c61a550: sa_motion_publish_stop(ctx,0x8c61a550u); r[0]=16u;goto L8c61a552; /* mov #0x10,r0 */
L8c61a552: sa_motion_publish_stop(ctx,0x8c61a552u); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a554; /* fmov.s @(r0,r5),fr5 */
L8c61a554: sa_motion_publish_stop(ctx,0x8c61a554u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a556; /* fmov.s @(r0,r4),fr3 */
L8c61a556: sa_motion_publish_stop(ctx,0x8c61a556u); r[0]=20u;goto L8c61a558; /* mov #0x14,r0 */
L8c61a558: sa_motion_publish_stop(ctx,0x8c61a558u); fcopy(f,2u,5u);goto L8c61a55a; /* fmov fr5,fr2 */
L8c61a55a: sa_motion_publish_stop(ctx,0x8c61a55au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61a55c; /* fsub fr3,fr2 */
L8c61a55c: sa_motion_publish_stop(ctx,0x8c61a55cu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a55e; /* fmov.s @(r0,r6),fr3 */
L8c61a55e: sa_motion_publish_stop(ctx,0x8c61a55eu); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a560; /* fmov.s @(r0,r5),fr4 */
L8c61a560: sa_motion_publish_stop(ctx,0x8c61a560u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a562; /* fsub fr4,fr3 */
L8c61a562: sa_motion_publish_stop(ctx,0x8c61a562u); fcopy(f,1u,4u);goto L8c61a564; /* fmov fr4,fr1 */
L8c61a564: sa_motion_publish_stop(ctx,0x8c61a564u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a566; /* fmul fr3,fr2 */
L8c61a566: sa_motion_publish_stop(ctx,0x8c61a566u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a568; /* fmov.s @(r0,r4),fr3 */
L8c61a568: sa_motion_publish_stop(ctx,0x8c61a568u); r[0]=16u;goto L8c61a56a; /* mov #0x10,r0 */
L8c61a56a: sa_motion_publish_stop(ctx,0x8c61a56au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61a56c; /* fsub fr3,fr1 */
L8c61a56c: sa_motion_publish_stop(ctx,0x8c61a56cu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a56e; /* fmov.s @(r0,r6),fr3 */
L8c61a56e: sa_motion_publish_stop(ctx,0x8c61a56eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a570; /* fsub fr5,fr3 */
L8c61a570: sa_motion_publish_stop(ctx,0x8c61a570u); fcopy(f,4u,2u);goto L8c61a572; /* fmov fr2,fr4 */
L8c61a572: sa_motion_publish_stop(ctx,0x8c61a572u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a574; /* fmul fr3,fr1 */
L8c61a574: sa_motion_publish_stop(ctx,0x8c61a574u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61a576; /* fldi0 fr3 */
L8c61a576: sa_motion_publish_stop(ctx,0x8c61a576u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61a578; /* fsub fr1,fr4 */
L8c61a578: sa_motion_publish_stop(ctx,0x8c61a578u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61a57a; /* fcmp/gt fr3,fr4 */
L8c61a57a: sa_motion_publish_stop(ctx,0x8c61a57au); if(!(*sr&1u))goto L8c61a580;goto L8c61a57c; /* bf 0x8c61a580 */
L8c61a57c: sa_motion_publish_stop(ctx,0x8c61a57eu); goto L8c61a73e; /* bra 0x8c61a73e */
L8c61a580: sa_motion_publish_stop(ctx,0x8c61a580u); r[3]=rd(m,bus,&io_status,0x8c61a770u,4);if(io_status)return io_status;goto L8c61a582; /* mov.l 0x8c61a770,r3 */
L8c61a582: sa_motion_publish_stop(ctx,0x8c61a582u); r[0]=4u;goto L8c61a584; /* mov #0x4,r0 */
L8c61a584: sa_motion_publish_stop(ctx,0x8c61a584u); fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61a586; /* fmov.s @r4,fr3 */
L8c61a586: sa_motion_publish_stop(ctx,0x8c61a586u); r[8]+=28u;goto L8c61a588; /* add #0x1c,r8 */
L8c61a588: sa_motion_publish_stop(ctx,0x8c61a588u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a58a; /* fmov.s @r3,fr2 */
L8c61a58a: sa_motion_publish_stop(ctx,0x8c61a58au); r[3]+=8u;goto L8c61a58c; /* add #0x8,r3 */
L8c61a58c: sa_motion_publish_stop(ctx,0x8c61a58cu); r[2]=rd(m,bus,&io_status,0x8c61a774u,4);if(io_status)return io_status;goto L8c61a58e; /* mov.l 0x8c61a774,r2 */
L8c61a58e: sa_motion_publish_stop(ctx,0x8c61a58eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a590; /* fmul fr3,fr2 */
L8c61a590: sa_motion_publish_stop(ctx,0x8c61a590u); r[1]=rd(m,bus,&io_status,0x8c61a778u,4);if(io_status)return io_status;goto L8c61a592; /* mov.l 0x8c61a778,r1 */
L8c61a592: sa_motion_publish_stop(ctx,0x8c61a592u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61a594; /* fmov.s @r2,fr3 */
L8c61a594: sa_motion_publish_stop(ctx,0x8c61a594u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61a596; /* fmov.s @r1,fr1 */
L8c61a596: sa_motion_publish_stop(ctx,0x8c61a596u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a598; /* fsub fr2,fr3 */
L8c61a598: sa_motion_publish_stop(ctx,0x8c61a598u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a59a; /* fmov.s @(r0,r4),fr2 */
L8c61a59a: sa_motion_publish_stop(ctx,0x8c61a59au); r[0]=8u;goto L8c61a59c; /* mov #0x8,r0 */
L8c61a59c: sa_motion_publish_stop(ctx,0x8c61a59cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a59e; /* fmul fr2,fr1 */
L8c61a59e: sa_motion_publish_stop(ctx,0x8c61a59eu); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a5a0; /* fmov.s @(r0,r4),fr2 */
L8c61a5a0: sa_motion_publish_stop(ctx,0x8c61a5a0u); r[0]=rd(m,bus,&io_status,0x8c61a77cu,4);if(io_status)return io_status;goto L8c61a5a2; /* mov.l 0x8c61a77c,r0 */
L8c61a5a2: sa_motion_publish_stop(ctx,0x8c61a5a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a5a4; /* fsub fr1,fr3 */
L8c61a5a4: sa_motion_publish_stop(ctx,0x8c61a5a4u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a5a6; /* fmov.s @r3,fr1 */
L8c61a5a6: sa_motion_publish_stop(ctx,0x8c61a5a6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a5a8; /* fmul fr2,fr1 */
L8c61a5a8: sa_motion_publish_stop(ctx,0x8c61a5a8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a5aa; /* fsub fr1,fr3 */
L8c61a5aa: sa_motion_publish_stop(ctx,0x8c61a5aau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a5ac; /* fmov.s fr3,@-r8 */
L8c61a5ac: sa_motion_publish_stop(ctx,0x8c61a5acu); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61a5ae; /* mov.l @r0,r2 */
L8c61a5ae: sa_motion_publish_stop(ctx,0x8c61a5aeu); r[0]=4u;goto L8c61a5b0; /* mov #0x4,r0 */
L8c61a5b0: sa_motion_publish_stop(ctx,0x8c61a5b0u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a5b2; /* fmov.s @(r0,r4),fr0 */
L8c61a5b2: sa_motion_publish_stop(ctx,0x8c61a5b2u); r[0]=20u;goto L8c61a5b4; /* mov #0x14,r0 */
L8c61a5b4: sa_motion_publish_stop(ctx,0x8c61a5b4u); r[1]=r[2];goto L8c61a5b6; /* mov r2,r1 */
L8c61a5b6: sa_motion_publish_stop(ctx,0x8c61a5b6u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a5b8; /* fmov.s @(r0,r1),fr3 */
L8c61a5b8: sa_motion_publish_stop(ctx,0x8c61a5b8u); r[0]=4u;goto L8c61a5ba; /* mov #0x4,r0 */
L8c61a5ba: sa_motion_publish_stop(ctx,0x8c61a5bau); r[3]=r[2];goto L8c61a5bc; /* mov r2,r3 */
L8c61a5bc: sa_motion_publish_stop(ctx,0x8c61a5bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a5be; /* fmul fr0,fr3 */
L8c61a5be: sa_motion_publish_stop(ctx,0x8c61a5beu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61a5c0; /* fmov.s @r4,fr0 */
L8c61a5c0: sa_motion_publish_stop(ctx,0x8c61a5c0u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a5c2; /* fmov.s @(r0,r3),fr2 */
L8c61a5c2: sa_motion_publish_stop(ctx,0x8c61a5c2u); r[0]=8u;goto L8c61a5c4; /* mov #0x8,r0 */
L8c61a5c4: sa_motion_publish_stop(ctx,0x8c61a5c4u); r[3]=rd(m,bus,&io_status,0x8c61a77cu,4);if(io_status)return io_status;goto L8c61a5c6; /* mov.l 0x8c61a77c,r3 */
L8c61a5c6: sa_motion_publish_stop(ctx,0x8c61a5c6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a5c8; /* fmac fr0,fr2,fr3 */
L8c61a5c8: sa_motion_publish_stop(ctx,0x8c61a5c8u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a5ca; /* fmov.s @(r0,r4),fr0 */
L8c61a5ca: sa_motion_publish_stop(ctx,0x8c61a5cau); r[0]=36u;goto L8c61a5cc; /* mov #0x24,r0 */
L8c61a5cc: sa_motion_publish_stop(ctx,0x8c61a5ccu); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a5ce; /* fmov.s @(r0,r2),fr2 */
L8c61a5ce: sa_motion_publish_stop(ctx,0x8c61a5ceu); r[0]=4u;goto L8c61a5d0; /* mov #0x4,r0 */
L8c61a5d0: sa_motion_publish_stop(ctx,0x8c61a5d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a5d2; /* fmac fr0,fr2,fr3 */
L8c61a5d2: sa_motion_publish_stop(ctx,0x8c61a5d2u); fcopy(f,0u,15u);goto L8c61a5d4; /* fmov fr15,fr0 */
L8c61a5d4: sa_motion_publish_stop(ctx,0x8c61a5d4u); fcopy(f,2u,15u);goto L8c61a5d6; /* fmov fr15,fr2 */
L8c61a5d6: sa_motion_publish_stop(ctx,0x8c61a5d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a5d8; /* fmac fr0,fr3,fr2 */
L8c61a5d8: sa_motion_publish_stop(ctx,0x8c61a5d8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a5da; /* fmov.s fr2,@-r8 */
L8c61a5da: sa_motion_publish_stop(ctx,0x8c61a5dau); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a5dc; /* fmov.s @(r0,r4),fr0 */
L8c61a5dc: sa_motion_publish_stop(ctx,0x8c61a5dcu); r[0]=16u;goto L8c61a5de; /* mov #0x10,r0 */
L8c61a5de: sa_motion_publish_stop(ctx,0x8c61a5deu); r[9]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a5e0; /* mov.l @r3,r9 */
L8c61a5e0: sa_motion_publish_stop(ctx,0x8c61a5e0u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61a5e2; /* fmov.s @(r0,r9),fr3 */
L8c61a5e2: sa_motion_publish_stop(ctx,0x8c61a5e2u); r[0]=8u;goto L8c61a5e4; /* mov #0x8,r0 */
L8c61a5e4: sa_motion_publish_stop(ctx,0x8c61a5e4u); fload(m,bus,&io_status,f,2u,r[9]);if(io_status)return io_status;goto L8c61a5e6; /* fmov.s @r9,fr2 */
L8c61a5e6: sa_motion_publish_stop(ctx,0x8c61a5e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a5e8; /* fmul fr0,fr3 */
L8c61a5e8: sa_motion_publish_stop(ctx,0x8c61a5e8u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61a5ea; /* fmov.s @r4,fr0 */
L8c61a5ea: sa_motion_publish_stop(ctx,0x8c61a5eau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a5ec; /* fmac fr0,fr2,fr3 */
L8c61a5ec: sa_motion_publish_stop(ctx,0x8c61a5ecu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a5ee; /* fmov.s @(r0,r4),fr0 */
L8c61a5ee: sa_motion_publish_stop(ctx,0x8c61a5eeu); r[0]=32u;goto L8c61a5f0; /* mov #0x20,r0 */
L8c61a5f0: sa_motion_publish_stop(ctx,0x8c61a5f0u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61a5f2; /* fmov.s @(r0,r9),fr2 */
L8c61a5f2: sa_motion_publish_stop(ctx,0x8c61a5f2u); r[0]=24u;goto L8c61a5f4; /* mov #0x18,r0 */
L8c61a5f4: sa_motion_publish_stop(ctx,0x8c61a5f4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a5f6; /* fmac fr0,fr2,fr3 */
L8c61a5f6: sa_motion_publish_stop(ctx,0x8c61a5f6u); fcopy(f,0u,15u);goto L8c61a5f8; /* fmov fr15,fr0 */
L8c61a5f8: sa_motion_publish_stop(ctx,0x8c61a5f8u); fcopy(f,2u,15u);goto L8c61a5fa; /* fmov fr15,fr2 */
L8c61a5fa: sa_motion_publish_stop(ctx,0x8c61a5fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a5fc; /* fmac fr0,fr3,fr2 */
L8c61a5fc: sa_motion_publish_stop(ctx,0x8c61a5fcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a5fe; /* fmov.s fr2,@-r8 */
L8c61a5fe: sa_motion_publish_stop(ctx,0x8c61a5feu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a600; /* fmov.s @(r0,r4),fr3 */
L8c61a600: sa_motion_publish_stop(ctx,0x8c61a600u); r[0]=20u;goto L8c61a602; /* mov #0x14,r0 */
L8c61a602: sa_motion_publish_stop(ctx,0x8c61a602u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a604; /* fmov.s fr3,@-r8 */
L8c61a604: sa_motion_publish_stop(ctx,0x8c61a604u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a606; /* fmov.s @(r0,r4),fr3 */
L8c61a606: sa_motion_publish_stop(ctx,0x8c61a606u); r[0]=16u;goto L8c61a608; /* mov #0x10,r0 */
L8c61a608: sa_motion_publish_stop(ctx,0x8c61a608u); r[2]=rd(m,bus,&io_status,0x8c61a780u,4);if(io_status)return io_status;goto L8c61a60a; /* mov.l 0x8c61a780,r2 */
L8c61a60a: sa_motion_publish_stop(ctx,0x8c61a60au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a60c; /* fmov.s fr3,@-r8 */
L8c61a60c: sa_motion_publish_stop(ctx,0x8c61a60cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a60e; /* fmov.s @(r0,r4),fr3 */
L8c61a60e: sa_motion_publish_stop(ctx,0x8c61a60eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a610; /* fmov.s fr3,@-r8 */
L8c61a610: sa_motion_publish_stop(ctx,0x8c61a610u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61a612; /* mov.l r2,@-r8 */
L8c61a612: sa_motion_publish_stop(ctx,0x8c61a612u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a614; /* pref @r8 */
L8c61a614: sa_motion_publish_stop(ctx,0x8c61a614u); r[2]=rd(m,bus,&io_status,0x8c61a770u,4);if(io_status)return io_status;goto L8c61a616; /* mov.l 0x8c61a770,r2 */
L8c61a616: sa_motion_publish_stop(ctx,0x8c61a616u); r[0]=4u;goto L8c61a618; /* mov #0x4,r0 */
L8c61a618: sa_motion_publish_stop(ctx,0x8c61a618u); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61a61a; /* fmov.s @r5,fr3 */
L8c61a61a: sa_motion_publish_stop(ctx,0x8c61a61au); r[8]+=60u;goto L8c61a61c; /* add #0x3c,r8 */
L8c61a61c: sa_motion_publish_stop(ctx,0x8c61a61cu); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61a61e; /* fmov.s @r2,fr2 */
L8c61a61e: sa_motion_publish_stop(ctx,0x8c61a61eu); r[2]+=8u;goto L8c61a620; /* add #0x8,r2 */
L8c61a620: sa_motion_publish_stop(ctx,0x8c61a620u); r[1]=rd(m,bus,&io_status,0x8c61a774u,4);if(io_status)return io_status;goto L8c61a622; /* mov.l 0x8c61a774,r1 */
L8c61a622: sa_motion_publish_stop(ctx,0x8c61a622u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a624; /* fmul fr3,fr2 */
L8c61a624: sa_motion_publish_stop(ctx,0x8c61a624u); r[3]=rd(m,bus,&io_status,0x8c61a778u,4);if(io_status)return io_status;goto L8c61a626; /* mov.l 0x8c61a778,r3 */
L8c61a626: sa_motion_publish_stop(ctx,0x8c61a626u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61a628; /* fmov.s @r1,fr3 */
L8c61a628: sa_motion_publish_stop(ctx,0x8c61a628u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a62a; /* fmov.s @r3,fr1 */
L8c61a62a: sa_motion_publish_stop(ctx,0x8c61a62au); r[3]=rd(m,bus,&io_status,0x8c61a77cu,4);if(io_status)return io_status;goto L8c61a62c; /* mov.l 0x8c61a77c,r3 */
L8c61a62c: sa_motion_publish_stop(ctx,0x8c61a62cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a62e; /* fsub fr2,fr3 */
L8c61a62e: sa_motion_publish_stop(ctx,0x8c61a62eu); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a630; /* fmov.s @(r0,r5),fr2 */
L8c61a630: sa_motion_publish_stop(ctx,0x8c61a630u); r[0]=8u;goto L8c61a632; /* mov #0x8,r0 */
L8c61a632: sa_motion_publish_stop(ctx,0x8c61a632u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a634; /* fmul fr2,fr1 */
L8c61a634: sa_motion_publish_stop(ctx,0x8c61a634u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a636; /* fmov.s @(r0,r5),fr2 */
L8c61a636: sa_motion_publish_stop(ctx,0x8c61a636u); r[0]=4u;goto L8c61a638; /* mov #0x4,r0 */
L8c61a638: sa_motion_publish_stop(ctx,0x8c61a638u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a63a; /* fsub fr1,fr3 */
L8c61a63a: sa_motion_publish_stop(ctx,0x8c61a63au); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61a63c; /* fmov.s @r2,fr1 */
L8c61a63c: sa_motion_publish_stop(ctx,0x8c61a63cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a63e; /* fmul fr2,fr1 */
L8c61a63e: sa_motion_publish_stop(ctx,0x8c61a63eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a640; /* fsub fr1,fr3 */
L8c61a640: sa_motion_publish_stop(ctx,0x8c61a640u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a642; /* fmov.s fr3,@-r8 */
L8c61a642: sa_motion_publish_stop(ctx,0x8c61a642u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a644; /* fmov.s @(r0,r5),fr0 */
L8c61a644: sa_motion_publish_stop(ctx,0x8c61a644u); r[0]=20u;goto L8c61a646; /* mov #0x14,r0 */
L8c61a646: sa_motion_publish_stop(ctx,0x8c61a646u); r[10]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a648; /* mov.l @r3,r10 */
L8c61a648: sa_motion_publish_stop(ctx,0x8c61a648u); fload(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c61a64a; /* fmov.s @(r0,r10),fr3 */
L8c61a64a: sa_motion_publish_stop(ctx,0x8c61a64au); r[0]=4u;goto L8c61a64c; /* mov #0x4,r0 */
L8c61a64c: sa_motion_publish_stop(ctx,0x8c61a64cu); fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c61a64e; /* fmov.s @(r0,r10),fr2 */
L8c61a64e: sa_motion_publish_stop(ctx,0x8c61a64eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a650; /* fmul fr0,fr3 */
L8c61a650: sa_motion_publish_stop(ctx,0x8c61a650u); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a652; /* fmov.s @r5,fr0 */
L8c61a652: sa_motion_publish_stop(ctx,0x8c61a652u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a654; /* fmac fr0,fr2,fr3 */
L8c61a654: sa_motion_publish_stop(ctx,0x8c61a654u); r[0]=8u;goto L8c61a656; /* mov #0x8,r0 */
L8c61a656: sa_motion_publish_stop(ctx,0x8c61a656u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a658; /* fmov.s @(r0,r5),fr0 */
L8c61a658: sa_motion_publish_stop(ctx,0x8c61a658u); r[0]=36u;goto L8c61a65a; /* mov #0x24,r0 */
L8c61a65a: sa_motion_publish_stop(ctx,0x8c61a65au); fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c61a65c; /* fmov.s @(r0,r10),fr2 */
L8c61a65c: sa_motion_publish_stop(ctx,0x8c61a65cu); r[0]=4u;goto L8c61a65e; /* mov #0x4,r0 */
L8c61a65e: sa_motion_publish_stop(ctx,0x8c61a65eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a660; /* fmac fr0,fr2,fr3 */
L8c61a660: sa_motion_publish_stop(ctx,0x8c61a660u); fcopy(f,0u,15u);goto L8c61a662; /* fmov fr15,fr0 */
L8c61a662: sa_motion_publish_stop(ctx,0x8c61a662u); fcopy(f,2u,15u);goto L8c61a664; /* fmov fr15,fr2 */
L8c61a664: sa_motion_publish_stop(ctx,0x8c61a664u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a666; /* fmac fr0,fr3,fr2 */
L8c61a666: sa_motion_publish_stop(ctx,0x8c61a666u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a668; /* fmov.s fr2,@-r8 */
L8c61a668: sa_motion_publish_stop(ctx,0x8c61a668u); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a66a; /* fmov.s @(r0,r5),fr0 */
L8c61a66a: sa_motion_publish_stop(ctx,0x8c61a66au); r[0]=16u;goto L8c61a66c; /* mov #0x10,r0 */
L8c61a66c: sa_motion_publish_stop(ctx,0x8c61a66cu); r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a66e; /* mov.l @r3,r12 */
L8c61a66e: sa_motion_publish_stop(ctx,0x8c61a66eu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61a670; /* fmov.s @(r0,r12),fr3 */
L8c61a670: sa_motion_publish_stop(ctx,0x8c61a670u); r[0]=8u;goto L8c61a672; /* mov #0x8,r0 */
L8c61a672: sa_motion_publish_stop(ctx,0x8c61a672u); fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61a674; /* fmov.s @r12,fr2 */
L8c61a674: sa_motion_publish_stop(ctx,0x8c61a674u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a676; /* fmul fr0,fr3 */
L8c61a676: sa_motion_publish_stop(ctx,0x8c61a676u); fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c61a678; /* fmov.s @r5,fr0 */
L8c61a678: sa_motion_publish_stop(ctx,0x8c61a678u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a67a; /* fmac fr0,fr2,fr3 */
L8c61a67a: sa_motion_publish_stop(ctx,0x8c61a67au); fload(m,bus,&io_status,f,0u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a67c; /* fmov.s @(r0,r5),fr0 */
L8c61a67c: sa_motion_publish_stop(ctx,0x8c61a67cu); r[0]=32u;goto L8c61a67e; /* mov #0x20,r0 */
L8c61a67e: sa_motion_publish_stop(ctx,0x8c61a67eu); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61a680; /* fmov.s @(r0,r12),fr2 */
L8c61a680: sa_motion_publish_stop(ctx,0x8c61a680u); r[0]=24u;goto L8c61a682; /* mov #0x18,r0 */
L8c61a682: sa_motion_publish_stop(ctx,0x8c61a682u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a684; /* fmac fr0,fr2,fr3 */
L8c61a684: sa_motion_publish_stop(ctx,0x8c61a684u); fcopy(f,0u,15u);goto L8c61a686; /* fmov fr15,fr0 */
L8c61a686: sa_motion_publish_stop(ctx,0x8c61a686u); fcopy(f,2u,15u);goto L8c61a688; /* fmov fr15,fr2 */
L8c61a688: sa_motion_publish_stop(ctx,0x8c61a688u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a68a; /* fmac fr0,fr3,fr2 */
L8c61a68a: sa_motion_publish_stop(ctx,0x8c61a68au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a68c; /* fmov.s fr2,@-r8 */
L8c61a68c: sa_motion_publish_stop(ctx,0x8c61a68cu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a68e; /* fmov.s @(r0,r5),fr3 */
L8c61a68e: sa_motion_publish_stop(ctx,0x8c61a68eu); r[0]=20u;goto L8c61a690; /* mov #0x14,r0 */
L8c61a690: sa_motion_publish_stop(ctx,0x8c61a690u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a692; /* fmov.s fr3,@-r8 */
L8c61a692: sa_motion_publish_stop(ctx,0x8c61a692u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a694; /* fmov.s @(r0,r5),fr3 */
L8c61a694: sa_motion_publish_stop(ctx,0x8c61a694u); r[0]=16u;goto L8c61a696; /* mov #0x10,r0 */
L8c61a696: sa_motion_publish_stop(ctx,0x8c61a696u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a698; /* fmov.s fr3,@-r8 */
L8c61a698: sa_motion_publish_stop(ctx,0x8c61a698u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61a69a; /* fmov.s @(r0,r5),fr3 */
L8c61a69a: sa_motion_publish_stop(ctx,0x8c61a69au); r[2]=rd(m,bus,&io_status,0x8c61a780u,4);if(io_status)return io_status;goto L8c61a69c; /* mov.l 0x8c61a780,r2 */
L8c61a69c: sa_motion_publish_stop(ctx,0x8c61a69cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a69e; /* fmov.s fr3,@-r8 */
L8c61a69e: sa_motion_publish_stop(ctx,0x8c61a69eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61a6a0; /* mov.l r2,@-r8 */
L8c61a6a0: sa_motion_publish_stop(ctx,0x8c61a6a0u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a6a2; /* pref @r8 */
L8c61a6a2: sa_motion_publish_stop(ctx,0x8c61a6a2u); r[2]=rd(m,bus,&io_status,0x8c61a770u,4);if(io_status)return io_status;goto L8c61a6a4; /* mov.l 0x8c61a770,r2 */
L8c61a6a4: sa_motion_publish_stop(ctx,0x8c61a6a4u); r[0]=4u;goto L8c61a6a6; /* mov #0x4,r0 */
L8c61a6a6: sa_motion_publish_stop(ctx,0x8c61a6a6u); fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c61a6a8; /* fmov.s @r6,fr3 */
L8c61a6a8: sa_motion_publish_stop(ctx,0x8c61a6a8u); r[8]+=60u;goto L8c61a6aa; /* add #0x3c,r8 */
L8c61a6aa: sa_motion_publish_stop(ctx,0x8c61a6aau); fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a6ac; /* fmov.s @r2+,fr2 */
L8c61a6ac: sa_motion_publish_stop(ctx,0x8c61a6acu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a6ae; /* fmul fr3,fr2 */
L8c61a6ae: sa_motion_publish_stop(ctx,0x8c61a6aeu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61a6b0; /* fmov.s @r1,fr3 */
L8c61a6b0: sa_motion_publish_stop(ctx,0x8c61a6b0u); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61a6b2; /* fmov.s @r2+,fr1 */
L8c61a6b2: sa_motion_publish_stop(ctx,0x8c61a6b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a6b4; /* fsub fr2,fr3 */
L8c61a6b4: sa_motion_publish_stop(ctx,0x8c61a6b4u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a6b6; /* fmov.s @(r0,r6),fr2 */
L8c61a6b6: sa_motion_publish_stop(ctx,0x8c61a6b6u); r[0]=8u;goto L8c61a6b8; /* mov #0x8,r0 */
L8c61a6b8: sa_motion_publish_stop(ctx,0x8c61a6b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a6ba; /* fmul fr2,fr1 */
L8c61a6ba: sa_motion_publish_stop(ctx,0x8c61a6bau); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a6bc; /* fmov.s @(r0,r6),fr2 */
L8c61a6bc: sa_motion_publish_stop(ctx,0x8c61a6bcu); r[0]=4u;goto L8c61a6be; /* mov #0x4,r0 */
L8c61a6be: sa_motion_publish_stop(ctx,0x8c61a6beu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a6c0; /* fsub fr1,fr3 */
L8c61a6c0: sa_motion_publish_stop(ctx,0x8c61a6c0u); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61a6c2; /* fmov.s @r2,fr1 */
L8c61a6c2: sa_motion_publish_stop(ctx,0x8c61a6c2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a6c4; /* fmul fr2,fr1 */
L8c61a6c4: sa_motion_publish_stop(ctx,0x8c61a6c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a6c6; /* fsub fr1,fr3 */
L8c61a6c6: sa_motion_publish_stop(ctx,0x8c61a6c6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a6c8; /* fmov.s fr3,@-r8 */
L8c61a6c8: sa_motion_publish_stop(ctx,0x8c61a6c8u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a6ca; /* fmov.s @(r0,r6),fr0 */
L8c61a6ca: sa_motion_publish_stop(ctx,0x8c61a6cau); r[0]=20u;goto L8c61a6cc; /* mov #0x14,r0 */
L8c61a6cc: sa_motion_publish_stop(ctx,0x8c61a6ccu); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a6ce; /* mov.l @r3,r13 */
L8c61a6ce: sa_motion_publish_stop(ctx,0x8c61a6ceu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a6d0; /* fmov.s @(r0,r13),fr3 */
L8c61a6d0: sa_motion_publish_stop(ctx,0x8c61a6d0u); r[0]=4u;goto L8c61a6d2; /* mov #0x4,r0 */
L8c61a6d2: sa_motion_publish_stop(ctx,0x8c61a6d2u); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a6d4; /* fmov.s @(r0,r13),fr2 */
L8c61a6d4: sa_motion_publish_stop(ctx,0x8c61a6d4u); r[0]=8u;goto L8c61a6d6; /* mov #0x8,r0 */
L8c61a6d6: sa_motion_publish_stop(ctx,0x8c61a6d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a6d8; /* fmul fr0,fr3 */
L8c61a6d8: sa_motion_publish_stop(ctx,0x8c61a6d8u); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61a6da; /* fmov.s @r6,fr0 */
L8c61a6da: sa_motion_publish_stop(ctx,0x8c61a6dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a6dc; /* fmac fr0,fr2,fr3 */
L8c61a6dc: sa_motion_publish_stop(ctx,0x8c61a6dcu); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a6de; /* fmov.s @(r0,r6),fr0 */
L8c61a6de: sa_motion_publish_stop(ctx,0x8c61a6deu); r[0]=36u;goto L8c61a6e0; /* mov #0x24,r0 */
L8c61a6e0: sa_motion_publish_stop(ctx,0x8c61a6e0u); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a6e2; /* fmov.s @(r0,r13),fr2 */
L8c61a6e2: sa_motion_publish_stop(ctx,0x8c61a6e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a6e4; /* fmac fr0,fr2,fr3 */
L8c61a6e4: sa_motion_publish_stop(ctx,0x8c61a6e4u); fcopy(f,0u,15u);goto L8c61a6e6; /* fmov fr15,fr0 */
L8c61a6e6: sa_motion_publish_stop(ctx,0x8c61a6e6u); fcopy(f,2u,15u);goto L8c61a6e8; /* fmov fr15,fr2 */
L8c61a6e8: sa_motion_publish_stop(ctx,0x8c61a6e8u); r[0]=4u;goto L8c61a6ea; /* mov #0x4,r0 */
L8c61a6ea: sa_motion_publish_stop(ctx,0x8c61a6eau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a6ec; /* fmac fr0,fr3,fr2 */
L8c61a6ec: sa_motion_publish_stop(ctx,0x8c61a6ecu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a6ee; /* fmov.s fr2,@-r8 */
L8c61a6ee: sa_motion_publish_stop(ctx,0x8c61a6eeu); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a6f0; /* fmov.s @(r0,r6),fr0 */
L8c61a6f0: sa_motion_publish_stop(ctx,0x8c61a6f0u); r[0]=16u;goto L8c61a6f2; /* mov #0x10,r0 */
L8c61a6f2: sa_motion_publish_stop(ctx,0x8c61a6f2u); r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a6f4; /* mov.l @r3,r7 */
L8c61a6f4: sa_motion_publish_stop(ctx,0x8c61a6f4u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a6f6; /* fmov.s @(r0,r7),fr3 */
L8c61a6f6: sa_motion_publish_stop(ctx,0x8c61a6f6u); r[0]=8u;goto L8c61a6f8; /* mov #0x8,r0 */
L8c61a6f8: sa_motion_publish_stop(ctx,0x8c61a6f8u); fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61a6fa; /* fmov.s @r7,fr2 */
L8c61a6fa: sa_motion_publish_stop(ctx,0x8c61a6fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a6fc; /* fmul fr0,fr3 */
L8c61a6fc: sa_motion_publish_stop(ctx,0x8c61a6fcu); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61a6fe; /* fmov.s @r6,fr0 */
L8c61a6fe: sa_motion_publish_stop(ctx,0x8c61a6feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a700; /* fmac fr0,fr2,fr3 */
L8c61a700: sa_motion_publish_stop(ctx,0x8c61a700u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a702; /* fmov.s @(r0,r6),fr0 */
L8c61a702: sa_motion_publish_stop(ctx,0x8c61a702u); r[0]=32u;goto L8c61a704; /* mov #0x20,r0 */
L8c61a704: sa_motion_publish_stop(ctx,0x8c61a704u); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a706; /* fmov.s @(r0,r7),fr2 */
L8c61a706: sa_motion_publish_stop(ctx,0x8c61a706u); r[0]=24u;goto L8c61a708; /* mov #0x18,r0 */
L8c61a708: sa_motion_publish_stop(ctx,0x8c61a708u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a70a; /* fmac fr0,fr2,fr3 */
L8c61a70a: sa_motion_publish_stop(ctx,0x8c61a70au); fcopy(f,0u,15u);goto L8c61a70c; /* fmov fr15,fr0 */
L8c61a70c: sa_motion_publish_stop(ctx,0x8c61a70cu); fcopy(f,2u,15u);goto L8c61a70e; /* fmov fr15,fr2 */
L8c61a70e: sa_motion_publish_stop(ctx,0x8c61a70eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a710; /* fmac fr0,fr3,fr2 */
L8c61a710: sa_motion_publish_stop(ctx,0x8c61a710u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a712; /* fmov.s fr2,@-r8 */
L8c61a712: sa_motion_publish_stop(ctx,0x8c61a712u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a714; /* fmov.s @(r0,r6),fr3 */
L8c61a714: sa_motion_publish_stop(ctx,0x8c61a714u); r[0]=20u;goto L8c61a716; /* mov #0x14,r0 */
L8c61a716: sa_motion_publish_stop(ctx,0x8c61a716u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a718; /* fmov.s fr3,@-r8 */
L8c61a718: sa_motion_publish_stop(ctx,0x8c61a718u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a71a; /* fmov.s @(r0,r6),fr3 */
L8c61a71a: sa_motion_publish_stop(ctx,0x8c61a71au); r[0]=16u;goto L8c61a71c; /* mov #0x10,r0 */
L8c61a71c: sa_motion_publish_stop(ctx,0x8c61a71cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a71e; /* fmov.s fr3,@-r8 */
L8c61a71e: sa_motion_publish_stop(ctx,0x8c61a71eu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61a720; /* fmov.s @(r0,r6),fr3 */
L8c61a720: sa_motion_publish_stop(ctx,0x8c61a720u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a722; /* fmov.s fr3,@-r8 */
L8c61a722: sa_motion_publish_stop(ctx,0x8c61a722u); r[2]=rd(m,bus,&io_status,0x8c61a784u,4);if(io_status)return io_status;goto L8c61a724; /* mov.l 0x8c61a784,r2 */
L8c61a724: sa_motion_publish_stop(ctx,0x8c61a724u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61a726; /* mov.l r2,@-r8 */
L8c61a726: sa_motion_publish_stop(ctx,0x8c61a726u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a728; /* pref @r8 */
L8c61a728: sa_motion_publish_stop(ctx,0x8c61a728u); r[0]=rd(m,bus,&io_status,0x8c61a788u,4);if(io_status)return io_status;goto L8c61a72a; /* mov.l 0x8c61a788,r0 */
L8c61a72a: sa_motion_publish_stop(ctx,0x8c61a72au); r[8]+=32u;goto L8c61a72c; /* add #0x20,r8 */
L8c61a72c: sa_motion_publish_stop(ctx,0x8c61a72cu); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61a72e; /* mov.l @r0,r2 */
L8c61a72e: sa_motion_publish_stop(ctx,0x8c61a72eu); r[2]+=3u;goto L8c61a730; /* add #0x3,r2 */
L8c61a730: sa_motion_publish_stop(ctx,0x8c61a732u); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61a73e; /* bra 0x8c61a73e */
L8c61a734: sa_motion_publish_stop(ctx,0x8c61a734u); r[0]=rd(m,bus,&io_status,0x8c61a78cu,4);if(io_status)return io_status;goto L8c61a736; /* mov.l 0x8c61a78c,r0 */
L8c61a736: sa_motion_publish_stop(ctx,0x8c61a736u); wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61a738; /* mov.l r3,@r0 */
L8c61a738: sa_motion_publish_stop(ctx,0x8c61a738u); r[3]=rd(m,bus,&io_status,0x8c61a790u,4);if(io_status)return io_status;goto L8c61a73a; /* mov.l 0x8c61a790,r3 */
L8c61a73a: sa_motion_publish_stop(ctx,0x8c61a73cu); target=r[3];*pr=0x8c61a73eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61a73e; /* jsr @r3 */
L8c61a73e: sa_motion_publish_stop(ctx,0x8c61a73eu); r[14]+=1u;goto L8c61a740; /* add #0x1,r14 */
L8c61a740: sa_motion_publish_stop(ctx,0x8c61a740u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61a742; /* mov.l @r15,r2 */
L8c61a742: sa_motion_publish_stop(ctx,0x8c61a742u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61a744; /* mov.w @(0x2,r2),r0 */
L8c61a744: sa_motion_publish_stop(ctx,0x8c61a744u); r[0]=r[0]&65535u;goto L8c61a746; /* extu.w r0,r0 */
L8c61a746: sa_motion_publish_stop(ctx,0x8c61a746u); *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c61a748; /* cmp/ge r0,r14 */
L8c61a748: sa_motion_publish_stop(ctx,0x8c61a748u); if((*sr&1u))goto L8c61a74e;goto L8c61a74a; /* bt 0x8c61a74e */
L8c61a74a: sa_motion_publish_stop(ctx,0x8c61a74cu); goto L8c61a500; /* bra 0x8c61a500 */
L8c61a74e: sa_motion_publish_stop(ctx,0x8c61a750u); goto L8c61ade8; /* bra 0x8c61ade8 */
L8c61a752: sa_motion_publish_stop(ctx,0x8c61a752u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61a754; /* mov.l @r15,r1 */
L8c61a754: sa_motion_publish_stop(ctx,0x8c61a754u); r[2]=rd(m,bus,&io_status,0x8c61a794u,4);if(io_status)return io_status;goto L8c61a756; /* mov.l 0x8c61a794,r2 */
L8c61a756: sa_motion_publish_stop(ctx,0x8c61a756u); r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61a758; /* mov.w @(0x2,r1),r0 */
L8c61a758: sa_motion_publish_stop(ctx,0x8c61a758u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a75a; /* mov.l @r2,r1 */
L8c61a75a: sa_motion_publish_stop(ctx,0x8c61a75au); r[0]=r[0]&65535u;goto L8c61a75c; /* extu.w r0,r0 */
L8c61a75c: sa_motion_publish_stop(ctx,0x8c61a75cu); r[0]<<=2;goto L8c61a75e; /* shll2 r0 */
L8c61a75e: sa_motion_publish_stop(ctx,0x8c61a75eu); r[1]+=r[0];goto L8c61a760; /* add r0,r1 */
L8c61a760: sa_motion_publish_stop(ctx,0x8c61a760u); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61a762; /* mov.l r1,@r2 */
L8c61a762: sa_motion_publish_stop(ctx,0x8c61a764u); r[10]=0u;goto L8c61aabc; /* bra 0x8c61aabc */
L8c61a798: sa_motion_publish_stop(ctx,0x8c61a798u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61a79a; /* mov.w @r11+,r4 */
L8c61a79a: sa_motion_publish_stop(ctx,0x8c61a79au); r[3]=rd(m,bus,&io_status,0x8c61aad8u,4);if(io_status)return io_status;goto L8c61a79c; /* mov.l 0x8c61aad8,r3 */
L8c61a79c: sa_motion_publish_stop(ctx,0x8c61a79cu); r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61a79e; /* mov.w @r11+,r13 */
L8c61a79e: sa_motion_publish_stop(ctx,0x8c61a79eu); r[4]=r[4]&65535u;goto L8c61a7a0; /* extu.w r4,r4 */
L8c61a7a0: sa_motion_publish_stop(ctx,0x8c61a7a0u); r[4]<<=2;goto L8c61a7a2; /* shll2 r4 */
L8c61a7a2: sa_motion_publish_stop(ctx,0x8c61a7a2u); r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61a7a4; /* mov.l @r3,r6 */
L8c61a7a4: sa_motion_publish_stop(ctx,0x8c61a7a4u); r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61a7a6; /* mov.w @r11+,r14 */
L8c61a7a6: sa_motion_publish_stop(ctx,0x8c61a7a6u); r[13]=r[13]&65535u;goto L8c61a7a8; /* extu.w r13,r13 */
L8c61a7a8: sa_motion_publish_stop(ctx,0x8c61a7a8u); r[13]<<=2;goto L8c61a7aa; /* shll2 r13 */
L8c61a7aa: sa_motion_publish_stop(ctx,0x8c61a7aau); r[12]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61a7ac; /* mov.w @r11+,r12 */
L8c61a7ac: sa_motion_publish_stop(ctx,0x8c61a7acu); r[14]=r[14]&65535u;goto L8c61a7ae; /* extu.w r14,r14 */
L8c61a7ae: sa_motion_publish_stop(ctx,0x8c61a7aeu); r[13]<<=2;goto L8c61a7b0; /* shll2 r13 */
L8c61a7b0: sa_motion_publish_stop(ctx,0x8c61a7b0u); r[4]<<=2;goto L8c61a7b2; /* shll2 r4 */
L8c61a7b2: sa_motion_publish_stop(ctx,0x8c61a7b2u); r[12]=r[12]&65535u;goto L8c61a7b4; /* extu.w r12,r12 */
L8c61a7b4: sa_motion_publish_stop(ctx,0x8c61a7b4u); r[14]<<=2;goto L8c61a7b6; /* shll2 r14 */
L8c61a7b6: sa_motion_publish_stop(ctx,0x8c61a7b6u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61a7b8; /* shll r4 */
L8c61a7b8: sa_motion_publish_stop(ctx,0x8c61a7b8u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61a7ba; /* shll r13 */
L8c61a7ba: sa_motion_publish_stop(ctx,0x8c61a7bau); r[4]+=r[6];goto L8c61a7bc; /* add r6,r4 */
L8c61a7bc: sa_motion_publish_stop(ctx,0x8c61a7bcu); r[12]<<=2;goto L8c61a7be; /* shll2 r12 */
L8c61a7be: sa_motion_publish_stop(ctx,0x8c61a7beu); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61a7c0; /* mov.l @(0xc,r4),r5 */
L8c61a7c0: sa_motion_publish_stop(ctx,0x8c61a7c0u); r[13]+=r[6];goto L8c61a7c2; /* add r6,r13 */
L8c61a7c2: sa_motion_publish_stop(ctx,0x8c61a7c2u); r[14]<<=2;goto L8c61a7c4; /* shll2 r14 */
L8c61a7c4: sa_motion_publish_stop(ctx,0x8c61a7c4u); r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61a7c6; /* mov.l @(0xc,r13),r2 */
L8c61a7c6: sa_motion_publish_stop(ctx,0x8c61a7c6u); r[12]<<=2;goto L8c61a7c8; /* shll2 r12 */
L8c61a7c8: sa_motion_publish_stop(ctx,0x8c61a7c8u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61a7ca; /* shll r14 */
L8c61a7ca: sa_motion_publish_stop(ctx,0x8c61a7cau); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61a7cc; /* shll r12 */
L8c61a7cc: sa_motion_publish_stop(ctx,0x8c61a7ccu); r[14]+=r[6];goto L8c61a7ce; /* add r6,r14 */
L8c61a7ce: sa_motion_publish_stop(ctx,0x8c61a7ceu); r[1]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61a7d0; /* mov.l @(0xc,r14),r1 */
L8c61a7d0: sa_motion_publish_stop(ctx,0x8c61a7d0u); r[5]&=r[2];goto L8c61a7d2; /* and r2,r5 */
L8c61a7d2: sa_motion_publish_stop(ctx,0x8c61a7d2u); r[12]+=r[6];goto L8c61a7d4; /* add r6,r12 */
L8c61a7d4: sa_motion_publish_stop(ctx,0x8c61a7d4u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61a7d6; /* mov.l @(0xc,r12),r2 */
L8c61a7d6: sa_motion_publish_stop(ctx,0x8c61a7d6u); r[5]&=r[1];goto L8c61a7d8; /* and r1,r5 */
L8c61a7d8: sa_motion_publish_stop(ctx,0x8c61a7d8u); r[1]=63u;goto L8c61a7da; /* mov #0x3f,r1 */
L8c61a7da: sa_motion_publish_stop(ctx,0x8c61a7dau); r[5]&=r[2];goto L8c61a7dc; /* and r2,r5 */
L8c61a7dc: sa_motion_publish_stop(ctx,0x8c61a7dcu); *sr=(*sr&~1u)|!(r[1]&r[5]);goto L8c61a7de; /* tst r1,r5 */
L8c61a7de: sa_motion_publish_stop(ctx,0x8c61a7deu); if((*sr&1u))goto L8c61a7e4;goto L8c61a7e0; /* bt 0x8c61a7e4 */
L8c61a7e0: sa_motion_publish_stop(ctx,0x8c61a7e2u); goto L8c61aaba; /* bra 0x8c61aaba */
L8c61a7e4: sa_motion_publish_stop(ctx,0x8c61a7e4u); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61a7e6; /* mov.l @(0xc,r4),r5 */
L8c61a7e6: sa_motion_publish_stop(ctx,0x8c61a7e6u); r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61a7e8; /* mov.l @(0xc,r13),r3 */
L8c61a7e8: sa_motion_publish_stop(ctx,0x8c61a7e8u); r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61a7ea; /* mov.l @(0xc,r14),r2 */
L8c61a7ea: sa_motion_publish_stop(ctx,0x8c61a7eau); r[5]|=r[3];goto L8c61a7ec; /* or r3,r5 */
L8c61a7ec: sa_motion_publish_stop(ctx,0x8c61a7ecu); r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61a7ee; /* mov.l @(0xc,r12),r3 */
L8c61a7ee: sa_motion_publish_stop(ctx,0x8c61a7eeu); r[5]|=r[2];goto L8c61a7f0; /* or r2,r5 */
L8c61a7f0: sa_motion_publish_stop(ctx,0x8c61a7f0u); r[2]=1u;goto L8c61a7f2; /* mov #0x1,r2 */
L8c61a7f2: sa_motion_publish_stop(ctx,0x8c61a7f2u); r[5]|=r[3];goto L8c61a7f4; /* or r3,r5 */
L8c61a7f4: sa_motion_publish_stop(ctx,0x8c61a7f4u); *sr=(*sr&~1u)|!(r[2]&r[5]);goto L8c61a7f6; /* tst r2,r5 */
L8c61a7f6: sa_motion_publish_stop(ctx,0x8c61a7f6u); if((*sr&1u))goto L8c61a7fc;goto L8c61a7f8; /* bt 0x8c61a7fc */
L8c61a7f8: sa_motion_publish_stop(ctx,0x8c61a7fau); goto L8c61aa84; /* bra 0x8c61aa84 */
L8c61a7fc: sa_motion_publish_stop(ctx,0x8c61a7fcu); r[0]=16u;goto L8c61a7fe; /* mov #0x10,r0 */
L8c61a7fe: sa_motion_publish_stop(ctx,0x8c61a7feu); fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a800; /* fmov.s @(r0,r13),fr4 */
L8c61a800: sa_motion_publish_stop(ctx,0x8c61a800u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a802; /* fmov.s @(r0,r4),fr3 */
L8c61a802: sa_motion_publish_stop(ctx,0x8c61a802u); r[0]=20u;goto L8c61a804; /* mov #0x14,r0 */
L8c61a804: sa_motion_publish_stop(ctx,0x8c61a804u); fcopy(f,2u,4u);goto L8c61a806; /* fmov fr4,fr2 */
L8c61a806: sa_motion_publish_stop(ctx,0x8c61a806u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61a808; /* fsub fr3,fr2 */
L8c61a808: sa_motion_publish_stop(ctx,0x8c61a808u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a80a; /* fmov.s @(r0,r14),fr3 */
L8c61a80a: sa_motion_publish_stop(ctx,0x8c61a80au); fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a80c; /* fmov.s @(r0,r13),fr5 */
L8c61a80c: sa_motion_publish_stop(ctx,0x8c61a80cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a80e; /* fsub fr5,fr3 */
L8c61a80e: sa_motion_publish_stop(ctx,0x8c61a80eu); fcopy(f,1u,5u);goto L8c61a810; /* fmov fr5,fr1 */
L8c61a810: sa_motion_publish_stop(ctx,0x8c61a810u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a812; /* fmul fr3,fr2 */
L8c61a812: sa_motion_publish_stop(ctx,0x8c61a812u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a814; /* fmov.s @(r0,r4),fr3 */
L8c61a814: sa_motion_publish_stop(ctx,0x8c61a814u); r[0]=16u;goto L8c61a816; /* mov #0x10,r0 */
L8c61a816: sa_motion_publish_stop(ctx,0x8c61a816u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61a818; /* fsub fr3,fr1 */
L8c61a818: sa_motion_publish_stop(ctx,0x8c61a818u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a81a; /* fmov.s @(r0,r14),fr3 */
L8c61a81a: sa_motion_publish_stop(ctx,0x8c61a81au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a81c; /* fsub fr4,fr3 */
L8c61a81c: sa_motion_publish_stop(ctx,0x8c61a81cu); fcopy(f,4u,2u);goto L8c61a81e; /* fmov fr2,fr4 */
L8c61a81e: sa_motion_publish_stop(ctx,0x8c61a81eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a820; /* fmul fr3,fr1 */
L8c61a820: sa_motion_publish_stop(ctx,0x8c61a820u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61a822; /* fldi0 fr3 */
L8c61a822: sa_motion_publish_stop(ctx,0x8c61a822u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61a824; /* fsub fr1,fr4 */
L8c61a824: sa_motion_publish_stop(ctx,0x8c61a824u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61a826; /* fcmp/gt fr3,fr4 */
L8c61a826: sa_motion_publish_stop(ctx,0x8c61a826u); if(!(*sr&1u))goto L8c61a82c;goto L8c61a828; /* bf 0x8c61a82c */
L8c61a828: sa_motion_publish_stop(ctx,0x8c61a82au); goto L8c61aaba; /* bra 0x8c61aaba */
L8c61a82c: sa_motion_publish_stop(ctx,0x8c61a82cu); r[3]=rd(m,bus,&io_status,0x8c61aadcu,4);if(io_status)return io_status;goto L8c61a82e; /* mov.l 0x8c61aadc,r3 */
L8c61a82e: sa_motion_publish_stop(ctx,0x8c61a82eu); r[0]=4u;goto L8c61a830; /* mov #0x4,r0 */
L8c61a830: sa_motion_publish_stop(ctx,0x8c61a830u); fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61a832; /* fmov.s @r4,fr3 */
L8c61a832: sa_motion_publish_stop(ctx,0x8c61a832u); r[8]+=28u;goto L8c61a834; /* add #0x1c,r8 */
L8c61a834: sa_motion_publish_stop(ctx,0x8c61a834u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a836; /* fmov.s @r3,fr2 */
L8c61a836: sa_motion_publish_stop(ctx,0x8c61a836u); r[3]+=8u;goto L8c61a838; /* add #0x8,r3 */
L8c61a838: sa_motion_publish_stop(ctx,0x8c61a838u); r[2]=rd(m,bus,&io_status,0x8c61aae0u,4);if(io_status)return io_status;goto L8c61a83a; /* mov.l 0x8c61aae0,r2 */
L8c61a83a: sa_motion_publish_stop(ctx,0x8c61a83au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a83c; /* fmul fr3,fr2 */
L8c61a83c: sa_motion_publish_stop(ctx,0x8c61a83cu); r[1]=rd(m,bus,&io_status,0x8c61aae4u,4);if(io_status)return io_status;goto L8c61a83e; /* mov.l 0x8c61aae4,r1 */
L8c61a83e: sa_motion_publish_stop(ctx,0x8c61a83eu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61a840; /* fmov.s @r2,fr3 */
L8c61a840: sa_motion_publish_stop(ctx,0x8c61a840u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61a842; /* fmov.s @r1,fr1 */
L8c61a842: sa_motion_publish_stop(ctx,0x8c61a842u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a844; /* fsub fr2,fr3 */
L8c61a844: sa_motion_publish_stop(ctx,0x8c61a844u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a846; /* fmov.s @(r0,r4),fr2 */
L8c61a846: sa_motion_publish_stop(ctx,0x8c61a846u); r[0]=8u;goto L8c61a848; /* mov #0x8,r0 */
L8c61a848: sa_motion_publish_stop(ctx,0x8c61a848u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a84a; /* fmul fr2,fr1 */
L8c61a84a: sa_motion_publish_stop(ctx,0x8c61a84au); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a84c; /* fmov.s @(r0,r4),fr2 */
L8c61a84c: sa_motion_publish_stop(ctx,0x8c61a84cu); r[0]=rd(m,bus,&io_status,0x8c61aae8u,4);if(io_status)return io_status;goto L8c61a84e; /* mov.l 0x8c61aae8,r0 */
L8c61a84e: sa_motion_publish_stop(ctx,0x8c61a84eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a850; /* fsub fr1,fr3 */
L8c61a850: sa_motion_publish_stop(ctx,0x8c61a850u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a852; /* fmov.s @r3,fr1 */
L8c61a852: sa_motion_publish_stop(ctx,0x8c61a852u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a854; /* fmul fr2,fr1 */
L8c61a854: sa_motion_publish_stop(ctx,0x8c61a854u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a856; /* fsub fr1,fr3 */
L8c61a856: sa_motion_publish_stop(ctx,0x8c61a856u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a858; /* fmov.s fr3,@-r8 */
L8c61a858: sa_motion_publish_stop(ctx,0x8c61a858u); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61a85a; /* mov.l @r0,r2 */
L8c61a85a: sa_motion_publish_stop(ctx,0x8c61a85au); r[0]=4u;goto L8c61a85c; /* mov #0x4,r0 */
L8c61a85c: sa_motion_publish_stop(ctx,0x8c61a85cu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a85e; /* fmov.s @(r0,r4),fr0 */
L8c61a85e: sa_motion_publish_stop(ctx,0x8c61a85eu); r[0]=20u;goto L8c61a860; /* mov #0x14,r0 */
L8c61a860: sa_motion_publish_stop(ctx,0x8c61a860u); r[1]=r[2];goto L8c61a862; /* mov r2,r1 */
L8c61a862: sa_motion_publish_stop(ctx,0x8c61a862u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a864; /* fmov.s @(r0,r1),fr3 */
L8c61a864: sa_motion_publish_stop(ctx,0x8c61a864u); r[0]=4u;goto L8c61a866; /* mov #0x4,r0 */
L8c61a866: sa_motion_publish_stop(ctx,0x8c61a866u); r[3]=r[2];goto L8c61a868; /* mov r2,r3 */
L8c61a868: sa_motion_publish_stop(ctx,0x8c61a868u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a86a; /* fmul fr0,fr3 */
L8c61a86a: sa_motion_publish_stop(ctx,0x8c61a86au); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61a86c; /* fmov.s @r4,fr0 */
L8c61a86c: sa_motion_publish_stop(ctx,0x8c61a86cu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a86e; /* fmov.s @(r0,r3),fr2 */
L8c61a86e: sa_motion_publish_stop(ctx,0x8c61a86eu); r[0]=8u;goto L8c61a870; /* mov #0x8,r0 */
L8c61a870: sa_motion_publish_stop(ctx,0x8c61a870u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a872; /* fmac fr0,fr2,fr3 */
L8c61a872: sa_motion_publish_stop(ctx,0x8c61a872u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a874; /* fmov.s @(r0,r4),fr0 */
L8c61a874: sa_motion_publish_stop(ctx,0x8c61a874u); r[0]=36u;goto L8c61a876; /* mov #0x24,r0 */
L8c61a876: sa_motion_publish_stop(ctx,0x8c61a876u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a878; /* fmov.s @(r0,r2),fr2 */
L8c61a878: sa_motion_publish_stop(ctx,0x8c61a878u); r[0]=4u;goto L8c61a87a; /* mov #0x4,r0 */
L8c61a87a: sa_motion_publish_stop(ctx,0x8c61a87au); r[2]=rd(m,bus,&io_status,0x8c61aae8u,4);if(io_status)return io_status;goto L8c61a87c; /* mov.l 0x8c61aae8,r2 */
L8c61a87c: sa_motion_publish_stop(ctx,0x8c61a87cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a87e; /* fmac fr0,fr2,fr3 */
L8c61a87e: sa_motion_publish_stop(ctx,0x8c61a87eu); fcopy(f,0u,15u);goto L8c61a880; /* fmov fr15,fr0 */
L8c61a880: sa_motion_publish_stop(ctx,0x8c61a880u); fcopy(f,2u,15u);goto L8c61a882; /* fmov fr15,fr2 */
L8c61a882: sa_motion_publish_stop(ctx,0x8c61a882u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a884; /* fmac fr0,fr3,fr2 */
L8c61a884: sa_motion_publish_stop(ctx,0x8c61a884u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a886; /* fmov.s fr2,@-r8 */
L8c61a886: sa_motion_publish_stop(ctx,0x8c61a886u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a888; /* mov.l @r2,r3 */
L8c61a888: sa_motion_publish_stop(ctx,0x8c61a888u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a88a; /* fmov.s @(r0,r4),fr0 */
L8c61a88a: sa_motion_publish_stop(ctx,0x8c61a88au); r[0]=16u;goto L8c61a88c; /* mov #0x10,r0 */
L8c61a88c: sa_motion_publish_stop(ctx,0x8c61a88cu); r[2]=r[3];goto L8c61a88e; /* mov r3,r2 */
L8c61a88e: sa_motion_publish_stop(ctx,0x8c61a88eu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a890; /* fmov.s @(r0,r2),fr3 */
L8c61a890: sa_motion_publish_stop(ctx,0x8c61a890u); r[1]=r[3];goto L8c61a892; /* mov r3,r1 */
L8c61a892: sa_motion_publish_stop(ctx,0x8c61a892u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61a894; /* fmov.s @r1,fr2 */
L8c61a894: sa_motion_publish_stop(ctx,0x8c61a894u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a896; /* fmul fr0,fr3 */
L8c61a896: sa_motion_publish_stop(ctx,0x8c61a896u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61a898; /* fmov.s @r4,fr0 */
L8c61a898: sa_motion_publish_stop(ctx,0x8c61a898u); r[0]=8u;goto L8c61a89a; /* mov #0x8,r0 */
L8c61a89a: sa_motion_publish_stop(ctx,0x8c61a89au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a89c; /* fmac fr0,fr2,fr3 */
L8c61a89c: sa_motion_publish_stop(ctx,0x8c61a89cu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a89e; /* fmov.s @(r0,r4),fr0 */
L8c61a89e: sa_motion_publish_stop(ctx,0x8c61a89eu); r[0]=32u;goto L8c61a8a0; /* mov #0x20,r0 */
L8c61a8a0: sa_motion_publish_stop(ctx,0x8c61a8a0u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a8a2; /* fmov.s @(r0,r3),fr2 */
L8c61a8a2: sa_motion_publish_stop(ctx,0x8c61a8a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a8a4; /* fmac fr0,fr2,fr3 */
L8c61a8a4: sa_motion_publish_stop(ctx,0x8c61a8a4u); fcopy(f,0u,15u);goto L8c61a8a6; /* fmov fr15,fr0 */
L8c61a8a6: sa_motion_publish_stop(ctx,0x8c61a8a6u); fcopy(f,2u,15u);goto L8c61a8a8; /* fmov fr15,fr2 */
L8c61a8a8: sa_motion_publish_stop(ctx,0x8c61a8a8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a8aa; /* fmac fr0,fr3,fr2 */
L8c61a8aa: sa_motion_publish_stop(ctx,0x8c61a8aau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a8ac; /* fmov.s fr2,@-r8 */
L8c61a8ac: sa_motion_publish_stop(ctx,0x8c61a8acu); r[0]=24u;goto L8c61a8ae; /* mov #0x18,r0 */
L8c61a8ae: sa_motion_publish_stop(ctx,0x8c61a8aeu); r[3]=rd(m,bus,&io_status,0x8c61aaecu,4);if(io_status)return io_status;goto L8c61a8b0; /* mov.l 0x8c61aaec,r3 */
L8c61a8b0: sa_motion_publish_stop(ctx,0x8c61a8b0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a8b2; /* fmov.s @(r0,r4),fr3 */
L8c61a8b2: sa_motion_publish_stop(ctx,0x8c61a8b2u); r[0]=20u;goto L8c61a8b4; /* mov #0x14,r0 */
L8c61a8b4: sa_motion_publish_stop(ctx,0x8c61a8b4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a8b6; /* fmov.s fr3,@-r8 */
L8c61a8b6: sa_motion_publish_stop(ctx,0x8c61a8b6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a8b8; /* fmov.s @(r0,r4),fr3 */
L8c61a8b8: sa_motion_publish_stop(ctx,0x8c61a8b8u); r[0]=16u;goto L8c61a8ba; /* mov #0x10,r0 */
L8c61a8ba: sa_motion_publish_stop(ctx,0x8c61a8bau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a8bc; /* fmov.s fr3,@-r8 */
L8c61a8bc: sa_motion_publish_stop(ctx,0x8c61a8bcu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61a8be; /* fmov.s @(r0,r4),fr3 */
L8c61a8be: sa_motion_publish_stop(ctx,0x8c61a8beu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a8c0; /* fmov.s fr3,@-r8 */
L8c61a8c0: sa_motion_publish_stop(ctx,0x8c61a8c0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a8c2; /* mov.l r3,@-r8 */
L8c61a8c2: sa_motion_publish_stop(ctx,0x8c61a8c2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a8c4; /* pref @r8 */
L8c61a8c4: sa_motion_publish_stop(ctx,0x8c61a8c4u); r[3]=rd(m,bus,&io_status,0x8c61aadcu,4);if(io_status)return io_status;goto L8c61a8c6; /* mov.l 0x8c61aadc,r3 */
L8c61a8c6: sa_motion_publish_stop(ctx,0x8c61a8c6u); r[0]=4u;goto L8c61a8c8; /* mov #0x4,r0 */
L8c61a8c8: sa_motion_publish_stop(ctx,0x8c61a8c8u); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61a8ca; /* fmov.s @r13,fr3 */
L8c61a8ca: sa_motion_publish_stop(ctx,0x8c61a8cau); r[8]+=60u;goto L8c61a8cc; /* add #0x3c,r8 */
L8c61a8cc: sa_motion_publish_stop(ctx,0x8c61a8ccu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a8ce; /* fmov.s @r3,fr2 */
L8c61a8ce: sa_motion_publish_stop(ctx,0x8c61a8ceu); r[3]+=8u;goto L8c61a8d0; /* add #0x8,r3 */
L8c61a8d0: sa_motion_publish_stop(ctx,0x8c61a8d0u); r[2]=rd(m,bus,&io_status,0x8c61aae0u,4);if(io_status)return io_status;goto L8c61a8d2; /* mov.l 0x8c61aae0,r2 */
L8c61a8d2: sa_motion_publish_stop(ctx,0x8c61a8d2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a8d4; /* fmul fr3,fr2 */
L8c61a8d4: sa_motion_publish_stop(ctx,0x8c61a8d4u); r[1]=rd(m,bus,&io_status,0x8c61aae4u,4);if(io_status)return io_status;goto L8c61a8d6; /* mov.l 0x8c61aae4,r1 */
L8c61a8d6: sa_motion_publish_stop(ctx,0x8c61a8d6u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61a8d8; /* fmov.s @r2,fr3 */
L8c61a8d8: sa_motion_publish_stop(ctx,0x8c61a8d8u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61a8da; /* fmov.s @r1,fr1 */
L8c61a8da: sa_motion_publish_stop(ctx,0x8c61a8dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a8dc; /* fsub fr2,fr3 */
L8c61a8dc: sa_motion_publish_stop(ctx,0x8c61a8dcu); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a8de; /* fmov.s @(r0,r13),fr2 */
L8c61a8de: sa_motion_publish_stop(ctx,0x8c61a8deu); r[0]=8u;goto L8c61a8e0; /* mov #0x8,r0 */
L8c61a8e0: sa_motion_publish_stop(ctx,0x8c61a8e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a8e2; /* fmul fr2,fr1 */
L8c61a8e2: sa_motion_publish_stop(ctx,0x8c61a8e2u); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a8e4; /* fmov.s @(r0,r13),fr2 */
L8c61a8e4: sa_motion_publish_stop(ctx,0x8c61a8e4u); r[0]=rd(m,bus,&io_status,0x8c61aae8u,4);if(io_status)return io_status;goto L8c61a8e6; /* mov.l 0x8c61aae8,r0 */
L8c61a8e6: sa_motion_publish_stop(ctx,0x8c61a8e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a8e8; /* fsub fr1,fr3 */
L8c61a8e8: sa_motion_publish_stop(ctx,0x8c61a8e8u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a8ea; /* fmov.s @r3,fr1 */
L8c61a8ea: sa_motion_publish_stop(ctx,0x8c61a8eau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a8ec; /* fmul fr2,fr1 */
L8c61a8ec: sa_motion_publish_stop(ctx,0x8c61a8ecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a8ee; /* fsub fr1,fr3 */
L8c61a8ee: sa_motion_publish_stop(ctx,0x8c61a8eeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a8f0; /* fmov.s fr3,@-r8 */
L8c61a8f0: sa_motion_publish_stop(ctx,0x8c61a8f0u); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61a8f2; /* mov.l @r0,r2 */
L8c61a8f2: sa_motion_publish_stop(ctx,0x8c61a8f2u); r[0]=4u;goto L8c61a8f4; /* mov #0x4,r0 */
L8c61a8f4: sa_motion_publish_stop(ctx,0x8c61a8f4u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a8f6; /* fmov.s @(r0,r13),fr0 */
L8c61a8f6: sa_motion_publish_stop(ctx,0x8c61a8f6u); r[0]=20u;goto L8c61a8f8; /* mov #0x14,r0 */
L8c61a8f8: sa_motion_publish_stop(ctx,0x8c61a8f8u); r[1]=r[2];goto L8c61a8fa; /* mov r2,r1 */
L8c61a8fa: sa_motion_publish_stop(ctx,0x8c61a8fau); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61a8fc; /* fmov.s @(r0,r1),fr3 */
L8c61a8fc: sa_motion_publish_stop(ctx,0x8c61a8fcu); r[0]=4u;goto L8c61a8fe; /* mov #0x4,r0 */
L8c61a8fe: sa_motion_publish_stop(ctx,0x8c61a8feu); r[3]=r[2];goto L8c61a900; /* mov r2,r3 */
L8c61a900: sa_motion_publish_stop(ctx,0x8c61a900u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a902; /* fmul fr0,fr3 */
L8c61a902: sa_motion_publish_stop(ctx,0x8c61a902u); fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61a904; /* fmov.s @r13,fr0 */
L8c61a904: sa_motion_publish_stop(ctx,0x8c61a904u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a906; /* fmov.s @(r0,r3),fr2 */
L8c61a906: sa_motion_publish_stop(ctx,0x8c61a906u); r[0]=8u;goto L8c61a908; /* mov #0x8,r0 */
L8c61a908: sa_motion_publish_stop(ctx,0x8c61a908u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a90a; /* fmac fr0,fr2,fr3 */
L8c61a90a: sa_motion_publish_stop(ctx,0x8c61a90au); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a90c; /* fmov.s @(r0,r13),fr0 */
L8c61a90c: sa_motion_publish_stop(ctx,0x8c61a90cu); r[0]=36u;goto L8c61a90e; /* mov #0x24,r0 */
L8c61a90e: sa_motion_publish_stop(ctx,0x8c61a90eu); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a910; /* fmov.s @(r0,r2),fr2 */
L8c61a910: sa_motion_publish_stop(ctx,0x8c61a910u); r[0]=4u;goto L8c61a912; /* mov #0x4,r0 */
L8c61a912: sa_motion_publish_stop(ctx,0x8c61a912u); r[2]=rd(m,bus,&io_status,0x8c61aae8u,4);if(io_status)return io_status;goto L8c61a914; /* mov.l 0x8c61aae8,r2 */
L8c61a914: sa_motion_publish_stop(ctx,0x8c61a914u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a916; /* fmac fr0,fr2,fr3 */
L8c61a916: sa_motion_publish_stop(ctx,0x8c61a916u); fcopy(f,0u,15u);goto L8c61a918; /* fmov fr15,fr0 */
L8c61a918: sa_motion_publish_stop(ctx,0x8c61a918u); fcopy(f,2u,15u);goto L8c61a91a; /* fmov fr15,fr2 */
L8c61a91a: sa_motion_publish_stop(ctx,0x8c61a91au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a91c; /* fmac fr0,fr3,fr2 */
L8c61a91c: sa_motion_publish_stop(ctx,0x8c61a91cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a91e; /* fmov.s fr2,@-r8 */
L8c61a91e: sa_motion_publish_stop(ctx,0x8c61a91eu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a920; /* mov.l @r2,r3 */
L8c61a920: sa_motion_publish_stop(ctx,0x8c61a920u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a922; /* fmov.s @(r0,r13),fr0 */
L8c61a922: sa_motion_publish_stop(ctx,0x8c61a922u); r[0]=16u;goto L8c61a924; /* mov #0x10,r0 */
L8c61a924: sa_motion_publish_stop(ctx,0x8c61a924u); r[2]=r[3];goto L8c61a926; /* mov r3,r2 */
L8c61a926: sa_motion_publish_stop(ctx,0x8c61a926u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61a928; /* fmov.s @(r0,r2),fr3 */
L8c61a928: sa_motion_publish_stop(ctx,0x8c61a928u); r[1]=r[3];goto L8c61a92a; /* mov r3,r1 */
L8c61a92a: sa_motion_publish_stop(ctx,0x8c61a92au); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61a92c; /* fmov.s @r1,fr2 */
L8c61a92c: sa_motion_publish_stop(ctx,0x8c61a92cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a92e; /* fmul fr0,fr3 */
L8c61a92e: sa_motion_publish_stop(ctx,0x8c61a92eu); fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61a930; /* fmov.s @r13,fr0 */
L8c61a930: sa_motion_publish_stop(ctx,0x8c61a930u); r[0]=8u;goto L8c61a932; /* mov #0x8,r0 */
L8c61a932: sa_motion_publish_stop(ctx,0x8c61a932u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a934; /* fmac fr0,fr2,fr3 */
L8c61a934: sa_motion_publish_stop(ctx,0x8c61a934u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a936; /* fmov.s @(r0,r13),fr0 */
L8c61a936: sa_motion_publish_stop(ctx,0x8c61a936u); r[0]=32u;goto L8c61a938; /* mov #0x20,r0 */
L8c61a938: sa_motion_publish_stop(ctx,0x8c61a938u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61a93a; /* fmov.s @(r0,r3),fr2 */
L8c61a93a: sa_motion_publish_stop(ctx,0x8c61a93au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a93c; /* fmac fr0,fr2,fr3 */
L8c61a93c: sa_motion_publish_stop(ctx,0x8c61a93cu); fcopy(f,0u,15u);goto L8c61a93e; /* fmov fr15,fr0 */
L8c61a93e: sa_motion_publish_stop(ctx,0x8c61a93eu); fcopy(f,2u,15u);goto L8c61a940; /* fmov fr15,fr2 */
L8c61a940: sa_motion_publish_stop(ctx,0x8c61a940u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a942; /* fmac fr0,fr3,fr2 */
L8c61a942: sa_motion_publish_stop(ctx,0x8c61a942u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a944; /* fmov.s fr2,@-r8 */
L8c61a944: sa_motion_publish_stop(ctx,0x8c61a944u); r[0]=24u;goto L8c61a946; /* mov #0x18,r0 */
L8c61a946: sa_motion_publish_stop(ctx,0x8c61a946u); r[3]=rd(m,bus,&io_status,0x8c61aaecu,4);if(io_status)return io_status;goto L8c61a948; /* mov.l 0x8c61aaec,r3 */
L8c61a948: sa_motion_publish_stop(ctx,0x8c61a948u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a94a; /* fmov.s @(r0,r13),fr3 */
L8c61a94a: sa_motion_publish_stop(ctx,0x8c61a94au); r[0]=20u;goto L8c61a94c; /* mov #0x14,r0 */
L8c61a94c: sa_motion_publish_stop(ctx,0x8c61a94cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a94e; /* fmov.s fr3,@-r8 */
L8c61a94e: sa_motion_publish_stop(ctx,0x8c61a94eu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a950; /* fmov.s @(r0,r13),fr3 */
L8c61a950: sa_motion_publish_stop(ctx,0x8c61a950u); r[0]=16u;goto L8c61a952; /* mov #0x10,r0 */
L8c61a952: sa_motion_publish_stop(ctx,0x8c61a952u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a954; /* fmov.s fr3,@-r8 */
L8c61a954: sa_motion_publish_stop(ctx,0x8c61a954u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61a956; /* fmov.s @(r0,r13),fr3 */
L8c61a956: sa_motion_publish_stop(ctx,0x8c61a956u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a958; /* fmov.s fr3,@-r8 */
L8c61a958: sa_motion_publish_stop(ctx,0x8c61a958u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a95a; /* mov.l r3,@-r8 */
L8c61a95a: sa_motion_publish_stop(ctx,0x8c61a95au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a95c; /* pref @r8 */
L8c61a95c: sa_motion_publish_stop(ctx,0x8c61a95cu); r[3]=rd(m,bus,&io_status,0x8c61aadcu,4);if(io_status)return io_status;goto L8c61a95e; /* mov.l 0x8c61aadc,r3 */
L8c61a95e: sa_motion_publish_stop(ctx,0x8c61a95eu); r[0]=4u;goto L8c61a960; /* mov #0x4,r0 */
L8c61a960: sa_motion_publish_stop(ctx,0x8c61a960u); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61a962; /* fmov.s @r14,fr3 */
L8c61a962: sa_motion_publish_stop(ctx,0x8c61a962u); r[8]+=60u;goto L8c61a964; /* add #0x3c,r8 */
L8c61a964: sa_motion_publish_stop(ctx,0x8c61a964u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a966; /* fmov.s @r3,fr2 */
L8c61a966: sa_motion_publish_stop(ctx,0x8c61a966u); r[3]+=8u;goto L8c61a968; /* add #0x8,r3 */
L8c61a968: sa_motion_publish_stop(ctx,0x8c61a968u); r[2]=rd(m,bus,&io_status,0x8c61aae0u,4);if(io_status)return io_status;goto L8c61a96a; /* mov.l 0x8c61aae0,r2 */
L8c61a96a: sa_motion_publish_stop(ctx,0x8c61a96au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a96c; /* fmul fr3,fr2 */
L8c61a96c: sa_motion_publish_stop(ctx,0x8c61a96cu); r[1]=rd(m,bus,&io_status,0x8c61aae4u,4);if(io_status)return io_status;goto L8c61a96e; /* mov.l 0x8c61aae4,r1 */
L8c61a96e: sa_motion_publish_stop(ctx,0x8c61a96eu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61a970; /* fmov.s @r2,fr3 */
L8c61a970: sa_motion_publish_stop(ctx,0x8c61a970u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61a972; /* fmov.s @r1,fr1 */
L8c61a972: sa_motion_publish_stop(ctx,0x8c61a972u); r[2]=rd(m,bus,&io_status,0x8c61aae8u,4);if(io_status)return io_status;goto L8c61a974; /* mov.l 0x8c61aae8,r2 */
L8c61a974: sa_motion_publish_stop(ctx,0x8c61a974u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a976; /* fsub fr2,fr3 */
L8c61a976: sa_motion_publish_stop(ctx,0x8c61a976u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a978; /* fmov.s @(r0,r14),fr2 */
L8c61a978: sa_motion_publish_stop(ctx,0x8c61a978u); r[0]=8u;goto L8c61a97a; /* mov #0x8,r0 */
L8c61a97a: sa_motion_publish_stop(ctx,0x8c61a97au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a97c; /* fmul fr2,fr1 */
L8c61a97c: sa_motion_publish_stop(ctx,0x8c61a97cu); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a97e; /* fmov.s @(r0,r14),fr2 */
L8c61a97e: sa_motion_publish_stop(ctx,0x8c61a97eu); r[0]=4u;goto L8c61a980; /* mov #0x4,r0 */
L8c61a980: sa_motion_publish_stop(ctx,0x8c61a980u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a982; /* fsub fr1,fr3 */
L8c61a982: sa_motion_publish_stop(ctx,0x8c61a982u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61a984; /* fmov.s @r3,fr1 */
L8c61a984: sa_motion_publish_stop(ctx,0x8c61a984u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61a986; /* fmul fr2,fr1 */
L8c61a986: sa_motion_publish_stop(ctx,0x8c61a986u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61a988; /* fsub fr1,fr3 */
L8c61a988: sa_motion_publish_stop(ctx,0x8c61a988u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a98a; /* fmov.s fr3,@-r8 */
L8c61a98a: sa_motion_publish_stop(ctx,0x8c61a98au); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a98c; /* fmov.s @(r0,r14),fr0 */
L8c61a98c: sa_motion_publish_stop(ctx,0x8c61a98cu); r[0]=20u;goto L8c61a98e; /* mov #0x14,r0 */
L8c61a98e: sa_motion_publish_stop(ctx,0x8c61a98eu); r[9]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a990; /* mov.l @r2,r9 */
L8c61a990: sa_motion_publish_stop(ctx,0x8c61a990u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61a992; /* fmov.s @(r0,r9),fr3 */
L8c61a992: sa_motion_publish_stop(ctx,0x8c61a992u); r[0]=4u;goto L8c61a994; /* mov #0x4,r0 */
L8c61a994: sa_motion_publish_stop(ctx,0x8c61a994u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61a996; /* fmov.s @(r0,r9),fr2 */
L8c61a996: sa_motion_publish_stop(ctx,0x8c61a996u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a998; /* fmul fr0,fr3 */
L8c61a998: sa_motion_publish_stop(ctx,0x8c61a998u); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61a99a; /* fmov.s @r14,fr0 */
L8c61a99a: sa_motion_publish_stop(ctx,0x8c61a99au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a99c; /* fmac fr0,fr2,fr3 */
L8c61a99c: sa_motion_publish_stop(ctx,0x8c61a99cu); r[0]=8u;goto L8c61a99e; /* mov #0x8,r0 */
L8c61a99e: sa_motion_publish_stop(ctx,0x8c61a99eu); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a9a0; /* fmov.s @(r0,r14),fr0 */
L8c61a9a0: sa_motion_publish_stop(ctx,0x8c61a9a0u); r[0]=36u;goto L8c61a9a2; /* mov #0x24,r0 */
L8c61a9a2: sa_motion_publish_stop(ctx,0x8c61a9a2u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61a9a4; /* fmov.s @(r0,r9),fr2 */
L8c61a9a4: sa_motion_publish_stop(ctx,0x8c61a9a4u); r[0]=4u;goto L8c61a9a6; /* mov #0x4,r0 */
L8c61a9a6: sa_motion_publish_stop(ctx,0x8c61a9a6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a9a8; /* fmac fr0,fr2,fr3 */
L8c61a9a8: sa_motion_publish_stop(ctx,0x8c61a9a8u); fcopy(f,0u,15u);goto L8c61a9aa; /* fmov fr15,fr0 */
L8c61a9aa: sa_motion_publish_stop(ctx,0x8c61a9aau); fcopy(f,2u,15u);goto L8c61a9ac; /* fmov fr15,fr2 */
L8c61a9ac: sa_motion_publish_stop(ctx,0x8c61a9acu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a9ae; /* fmac fr0,fr3,fr2 */
L8c61a9ae: sa_motion_publish_stop(ctx,0x8c61a9aeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a9b0; /* fmov.s fr2,@-r8 */
L8c61a9b0: sa_motion_publish_stop(ctx,0x8c61a9b0u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a9b2; /* fmov.s @(r0,r14),fr0 */
L8c61a9b2: sa_motion_publish_stop(ctx,0x8c61a9b2u); r[0]=16u;goto L8c61a9b4; /* mov #0x10,r0 */
L8c61a9b4: sa_motion_publish_stop(ctx,0x8c61a9b4u); r[7]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61a9b6; /* mov.l @r2,r7 */
L8c61a9b6: sa_motion_publish_stop(ctx,0x8c61a9b6u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a9b8; /* fmov.s @(r0,r7),fr3 */
L8c61a9b8: sa_motion_publish_stop(ctx,0x8c61a9b8u); r[0]=8u;goto L8c61a9ba; /* mov #0x8,r0 */
L8c61a9ba: sa_motion_publish_stop(ctx,0x8c61a9bau); fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61a9bc; /* fmov.s @r7,fr2 */
L8c61a9bc: sa_motion_publish_stop(ctx,0x8c61a9bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a9be; /* fmul fr0,fr3 */
L8c61a9be: sa_motion_publish_stop(ctx,0x8c61a9beu); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61a9c0; /* fmov.s @r14,fr0 */
L8c61a9c0: sa_motion_publish_stop(ctx,0x8c61a9c0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a9c2; /* fmac fr0,fr2,fr3 */
L8c61a9c2: sa_motion_publish_stop(ctx,0x8c61a9c2u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a9c4; /* fmov.s @(r0,r14),fr0 */
L8c61a9c4: sa_motion_publish_stop(ctx,0x8c61a9c4u); r[0]=32u;goto L8c61a9c6; /* mov #0x20,r0 */
L8c61a9c6: sa_motion_publish_stop(ctx,0x8c61a9c6u); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61a9c8; /* fmov.s @(r0,r7),fr2 */
L8c61a9c8: sa_motion_publish_stop(ctx,0x8c61a9c8u); r[0]=24u;goto L8c61a9ca; /* mov #0x18,r0 */
L8c61a9ca: sa_motion_publish_stop(ctx,0x8c61a9cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61a9cc; /* fmac fr0,fr2,fr3 */
L8c61a9cc: sa_motion_publish_stop(ctx,0x8c61a9ccu); fcopy(f,0u,15u);goto L8c61a9ce; /* fmov fr15,fr0 */
L8c61a9ce: sa_motion_publish_stop(ctx,0x8c61a9ceu); fcopy(f,2u,15u);goto L8c61a9d0; /* fmov fr15,fr2 */
L8c61a9d0: sa_motion_publish_stop(ctx,0x8c61a9d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a9d2; /* fmac fr0,fr3,fr2 */
L8c61a9d2: sa_motion_publish_stop(ctx,0x8c61a9d2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61a9d4; /* fmov.s fr2,@-r8 */
L8c61a9d4: sa_motion_publish_stop(ctx,0x8c61a9d4u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a9d6; /* fmov.s @(r0,r14),fr3 */
L8c61a9d6: sa_motion_publish_stop(ctx,0x8c61a9d6u); r[0]=20u;goto L8c61a9d8; /* mov #0x14,r0 */
L8c61a9d8: sa_motion_publish_stop(ctx,0x8c61a9d8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a9da; /* fmov.s fr3,@-r8 */
L8c61a9da: sa_motion_publish_stop(ctx,0x8c61a9dau); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a9dc; /* fmov.s @(r0,r14),fr3 */
L8c61a9dc: sa_motion_publish_stop(ctx,0x8c61a9dcu); r[0]=16u;goto L8c61a9de; /* mov #0x10,r0 */
L8c61a9de: sa_motion_publish_stop(ctx,0x8c61a9deu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a9e0; /* fmov.s fr3,@-r8 */
L8c61a9e0: sa_motion_publish_stop(ctx,0x8c61a9e0u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61a9e2; /* fmov.s @(r0,r14),fr3 */
L8c61a9e2: sa_motion_publish_stop(ctx,0x8c61a9e2u); r[3]=rd(m,bus,&io_status,0x8c61aaecu,4);if(io_status)return io_status;goto L8c61a9e4; /* mov.l 0x8c61aaec,r3 */
L8c61a9e4: sa_motion_publish_stop(ctx,0x8c61a9e4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61a9e6; /* fmov.s fr3,@-r8 */
L8c61a9e6: sa_motion_publish_stop(ctx,0x8c61a9e6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61a9e8; /* mov.l r3,@-r8 */
L8c61a9e8: sa_motion_publish_stop(ctx,0x8c61a9e8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61a9ea; /* pref @r8 */
L8c61a9ea: sa_motion_publish_stop(ctx,0x8c61a9eau); r[3]=rd(m,bus,&io_status,0x8c61aadcu,4);if(io_status)return io_status;goto L8c61a9ec; /* mov.l 0x8c61aadc,r3 */
L8c61a9ec: sa_motion_publish_stop(ctx,0x8c61a9ecu); r[0]=4u;goto L8c61a9ee; /* mov #0x4,r0 */
L8c61a9ee: sa_motion_publish_stop(ctx,0x8c61a9eeu); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61a9f0; /* fmov.s @r12,fr3 */
L8c61a9f0: sa_motion_publish_stop(ctx,0x8c61a9f0u); r[8]+=60u;goto L8c61a9f2; /* add #0x3c,r8 */
L8c61a9f2: sa_motion_publish_stop(ctx,0x8c61a9f2u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61a9f4; /* fmov.s @r3,fr2 */
L8c61a9f4: sa_motion_publish_stop(ctx,0x8c61a9f4u); r[3]+=8u;goto L8c61a9f6; /* add #0x8,r3 */
L8c61a9f6: sa_motion_publish_stop(ctx,0x8c61a9f6u); r[1]=rd(m,bus,&io_status,0x8c61aae0u,4);if(io_status)return io_status;goto L8c61a9f8; /* mov.l 0x8c61aae0,r1 */
L8c61a9f8: sa_motion_publish_stop(ctx,0x8c61a9f8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61a9fa; /* fmul fr3,fr2 */
L8c61a9fa: sa_motion_publish_stop(ctx,0x8c61a9fau); r[2]=rd(m,bus,&io_status,0x8c61aae4u,4);if(io_status)return io_status;goto L8c61a9fc; /* mov.l 0x8c61aae4,r2 */
L8c61a9fc: sa_motion_publish_stop(ctx,0x8c61a9fcu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61a9fe; /* fmov.s @r1,fr3 */
L8c61a9fe: sa_motion_publish_stop(ctx,0x8c61a9feu); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61aa00; /* fmov.s @r2,fr1 */
L8c61aa00: sa_motion_publish_stop(ctx,0x8c61aa00u); r[2]=rd(m,bus,&io_status,0x8c61aae8u,4);if(io_status)return io_status;goto L8c61aa02; /* mov.l 0x8c61aae8,r2 */
L8c61aa02: sa_motion_publish_stop(ctx,0x8c61aa02u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61aa04; /* fsub fr2,fr3 */
L8c61aa04: sa_motion_publish_stop(ctx,0x8c61aa04u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa06; /* fmov.s @(r0,r12),fr2 */
L8c61aa06: sa_motion_publish_stop(ctx,0x8c61aa06u); r[0]=8u;goto L8c61aa08; /* mov #0x8,r0 */
L8c61aa08: sa_motion_publish_stop(ctx,0x8c61aa08u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61aa0a; /* fmul fr2,fr1 */
L8c61aa0a: sa_motion_publish_stop(ctx,0x8c61aa0au); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa0c; /* fmov.s @(r0,r12),fr2 */
L8c61aa0c: sa_motion_publish_stop(ctx,0x8c61aa0cu); r[0]=4u;goto L8c61aa0e; /* mov #0x4,r0 */
L8c61aa0e: sa_motion_publish_stop(ctx,0x8c61aa0eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61aa10; /* fsub fr1,fr3 */
L8c61aa10: sa_motion_publish_stop(ctx,0x8c61aa10u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61aa12; /* fmov.s @r3,fr1 */
L8c61aa12: sa_motion_publish_stop(ctx,0x8c61aa12u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61aa14; /* fmul fr2,fr1 */
L8c61aa14: sa_motion_publish_stop(ctx,0x8c61aa14u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61aa16; /* fsub fr1,fr3 */
L8c61aa16: sa_motion_publish_stop(ctx,0x8c61aa16u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61aa18; /* fmov.s fr3,@-r8 */
L8c61aa18: sa_motion_publish_stop(ctx,0x8c61aa18u); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa1a; /* fmov.s @(r0,r12),fr0 */
L8c61aa1a: sa_motion_publish_stop(ctx,0x8c61aa1au); r[0]=20u;goto L8c61aa1c; /* mov #0x14,r0 */
L8c61aa1c: sa_motion_publish_stop(ctx,0x8c61aa1cu); r[6]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61aa1e; /* mov.l @r2,r6 */
L8c61aa1e: sa_motion_publish_stop(ctx,0x8c61aa1eu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61aa20; /* fmov.s @(r0,r6),fr3 */
L8c61aa20: sa_motion_publish_stop(ctx,0x8c61aa20u); r[0]=4u;goto L8c61aa22; /* mov #0x4,r0 */
L8c61aa22: sa_motion_publish_stop(ctx,0x8c61aa22u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61aa24; /* fmov.s @(r0,r6),fr2 */
L8c61aa24: sa_motion_publish_stop(ctx,0x8c61aa24u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61aa26; /* fmul fr0,fr3 */
L8c61aa26: sa_motion_publish_stop(ctx,0x8c61aa26u); fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61aa28; /* fmov.s @r12,fr0 */
L8c61aa28: sa_motion_publish_stop(ctx,0x8c61aa28u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61aa2a; /* fmac fr0,fr2,fr3 */
L8c61aa2a: sa_motion_publish_stop(ctx,0x8c61aa2au); r[0]=8u;goto L8c61aa2c; /* mov #0x8,r0 */
L8c61aa2c: sa_motion_publish_stop(ctx,0x8c61aa2cu); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa2e; /* fmov.s @(r0,r12),fr0 */
L8c61aa2e: sa_motion_publish_stop(ctx,0x8c61aa2eu); r[0]=36u;goto L8c61aa30; /* mov #0x24,r0 */
L8c61aa30: sa_motion_publish_stop(ctx,0x8c61aa30u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61aa32; /* fmov.s @(r0,r6),fr2 */
L8c61aa32: sa_motion_publish_stop(ctx,0x8c61aa32u); r[0]=4u;goto L8c61aa34; /* mov #0x4,r0 */
L8c61aa34: sa_motion_publish_stop(ctx,0x8c61aa34u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61aa36; /* fmac fr0,fr2,fr3 */
L8c61aa36: sa_motion_publish_stop(ctx,0x8c61aa36u); fcopy(f,0u,15u);goto L8c61aa38; /* fmov fr15,fr0 */
L8c61aa38: sa_motion_publish_stop(ctx,0x8c61aa38u); fcopy(f,2u,15u);goto L8c61aa3a; /* fmov fr15,fr2 */
L8c61aa3a: sa_motion_publish_stop(ctx,0x8c61aa3au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61aa3c; /* fmac fr0,fr3,fr2 */
L8c61aa3c: sa_motion_publish_stop(ctx,0x8c61aa3cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61aa3e; /* fmov.s fr2,@-r8 */
L8c61aa3e: sa_motion_publish_stop(ctx,0x8c61aa3eu); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa40; /* fmov.s @(r0,r12),fr0 */
L8c61aa40: sa_motion_publish_stop(ctx,0x8c61aa40u); r[0]=16u;goto L8c61aa42; /* mov #0x10,r0 */
L8c61aa42: sa_motion_publish_stop(ctx,0x8c61aa42u); r[5]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61aa44; /* mov.l @r2,r5 */
L8c61aa44: sa_motion_publish_stop(ctx,0x8c61aa44u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61aa46; /* fmov.s @(r0,r5),fr3 */
L8c61aa46: sa_motion_publish_stop(ctx,0x8c61aa46u); r[0]=8u;goto L8c61aa48; /* mov #0x8,r0 */
L8c61aa48: sa_motion_publish_stop(ctx,0x8c61aa48u); fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c61aa4a; /* fmov.s @r5,fr2 */
L8c61aa4a: sa_motion_publish_stop(ctx,0x8c61aa4au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61aa4c; /* fmul fr0,fr3 */
L8c61aa4c: sa_motion_publish_stop(ctx,0x8c61aa4cu); fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61aa4e; /* fmov.s @r12,fr0 */
L8c61aa4e: sa_motion_publish_stop(ctx,0x8c61aa4eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61aa50; /* fmac fr0,fr2,fr3 */
L8c61aa50: sa_motion_publish_stop(ctx,0x8c61aa50u); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa52; /* fmov.s @(r0,r12),fr0 */
L8c61aa52: sa_motion_publish_stop(ctx,0x8c61aa52u); r[0]=32u;goto L8c61aa54; /* mov #0x20,r0 */
L8c61aa54: sa_motion_publish_stop(ctx,0x8c61aa54u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61aa56; /* fmov.s @(r0,r5),fr2 */
L8c61aa56: sa_motion_publish_stop(ctx,0x8c61aa56u); r[0]=24u;goto L8c61aa58; /* mov #0x18,r0 */
L8c61aa58: sa_motion_publish_stop(ctx,0x8c61aa58u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61aa5a; /* fmac fr0,fr2,fr3 */
L8c61aa5a: sa_motion_publish_stop(ctx,0x8c61aa5au); fcopy(f,0u,15u);goto L8c61aa5c; /* fmov fr15,fr0 */
L8c61aa5c: sa_motion_publish_stop(ctx,0x8c61aa5cu); fcopy(f,2u,15u);goto L8c61aa5e; /* fmov fr15,fr2 */
L8c61aa5e: sa_motion_publish_stop(ctx,0x8c61aa5eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61aa60; /* fmac fr0,fr3,fr2 */
L8c61aa60: sa_motion_publish_stop(ctx,0x8c61aa60u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61aa62; /* fmov.s fr2,@-r8 */
L8c61aa62: sa_motion_publish_stop(ctx,0x8c61aa62u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa64; /* fmov.s @(r0,r12),fr3 */
L8c61aa64: sa_motion_publish_stop(ctx,0x8c61aa64u); r[0]=20u;goto L8c61aa66; /* mov #0x14,r0 */
L8c61aa66: sa_motion_publish_stop(ctx,0x8c61aa66u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61aa68; /* fmov.s fr3,@-r8 */
L8c61aa68: sa_motion_publish_stop(ctx,0x8c61aa68u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa6a; /* fmov.s @(r0,r12),fr3 */
L8c61aa6a: sa_motion_publish_stop(ctx,0x8c61aa6au); r[0]=16u;goto L8c61aa6c; /* mov #0x10,r0 */
L8c61aa6c: sa_motion_publish_stop(ctx,0x8c61aa6cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61aa6e; /* fmov.s fr3,@-r8 */
L8c61aa6e: sa_motion_publish_stop(ctx,0x8c61aa6eu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61aa70; /* fmov.s @(r0,r12),fr3 */
L8c61aa70: sa_motion_publish_stop(ctx,0x8c61aa70u); r[3]=rd(m,bus,&io_status,0x8c61aaf0u,4);if(io_status)return io_status;goto L8c61aa72; /* mov.l 0x8c61aaf0,r3 */
L8c61aa72: sa_motion_publish_stop(ctx,0x8c61aa72u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61aa74; /* fmov.s fr3,@-r8 */
L8c61aa74: sa_motion_publish_stop(ctx,0x8c61aa74u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61aa76; /* mov.l r3,@-r8 */
L8c61aa76: sa_motion_publish_stop(ctx,0x8c61aa76u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61aa78; /* pref @r8 */
L8c61aa78: sa_motion_publish_stop(ctx,0x8c61aa78u); r[0]=rd(m,bus,&io_status,0x8c61aaf4u,4);if(io_status)return io_status;goto L8c61aa7a; /* mov.l 0x8c61aaf4,r0 */
L8c61aa7a: sa_motion_publish_stop(ctx,0x8c61aa7au); r[8]+=32u;goto L8c61aa7c; /* add #0x20,r8 */
L8c61aa7c: sa_motion_publish_stop(ctx,0x8c61aa7cu); r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61aa7e; /* mov.l @r0,r3 */
L8c61aa7e: sa_motion_publish_stop(ctx,0x8c61aa7eu); r[3]+=4u;goto L8c61aa80; /* add #0x4,r3 */
L8c61aa80: sa_motion_publish_stop(ctx,0x8c61aa82u); wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61aaba; /* bra 0x8c61aaba */
L8c61aa84: sa_motion_publish_stop(ctx,0x8c61aa84u); r[0]=rd(m,bus,&io_status,0x8c61aaf8u,4);if(io_status)return io_status;goto L8c61aa86; /* mov.l 0x8c61aaf8,r0 */
L8c61aa86: sa_motion_publish_stop(ctx,0x8c61aa86u); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c61aa88; /* mov.l r2,@r0 */
L8c61aa88: sa_motion_publish_stop(ctx,0x8c61aa88u); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61aa8a; /* mov.l @(0xc,r4),r5 */
L8c61aa8a: sa_motion_publish_stop(ctx,0x8c61aa8au); r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61aa8c; /* mov.l @(0xc,r13),r3 */
L8c61aa8c: sa_motion_publish_stop(ctx,0x8c61aa8cu); r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61aa8e; /* mov.l @(0xc,r14),r2 */
L8c61aa8e: sa_motion_publish_stop(ctx,0x8c61aa8eu); r[5]&=r[3];goto L8c61aa90; /* and r3,r5 */
L8c61aa90: sa_motion_publish_stop(ctx,0x8c61aa90u); r[3]=1u;goto L8c61aa92; /* mov #0x1,r3 */
L8c61aa92: sa_motion_publish_stop(ctx,0x8c61aa92u); r[5]&=r[2];goto L8c61aa94; /* and r2,r5 */
L8c61aa94: sa_motion_publish_stop(ctx,0x8c61aa94u); *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61aa96; /* tst r3,r5 */
L8c61aa96: sa_motion_publish_stop(ctx,0x8c61aa96u); if(!(*sr&1u))goto L8c61aaa0;goto L8c61aa98; /* bf 0x8c61aaa0 */
L8c61aa98: sa_motion_publish_stop(ctx,0x8c61aa98u); r[1]=rd(m,bus,&io_status,0x8c61aafcu,4);if(io_status)return io_status;goto L8c61aa9a; /* mov.l 0x8c61aafc,r1 */
L8c61aa9a: sa_motion_publish_stop(ctx,0x8c61aa9au); r[6]=r[14];goto L8c61aa9c; /* mov r14,r6 */
L8c61aa9c: sa_motion_publish_stop(ctx,0x8c61aa9eu); target=r[1];*pr=0x8c61aaa0u;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61aaa0; /* jsr @r1 */
L8c61aaa0: sa_motion_publish_stop(ctx,0x8c61aaa0u); r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61aaa2; /* mov.l @(0xc,r13),r4 */
L8c61aaa2: sa_motion_publish_stop(ctx,0x8c61aaa2u); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61aaa4; /* mov.l @(0xc,r14),r3 */
L8c61aaa4: sa_motion_publish_stop(ctx,0x8c61aaa4u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61aaa6; /* mov.l @(0xc,r12),r2 */
L8c61aaa6: sa_motion_publish_stop(ctx,0x8c61aaa6u); r[4]&=r[3];goto L8c61aaa8; /* and r3,r4 */
L8c61aaa8: sa_motion_publish_stop(ctx,0x8c61aaa8u); r[3]=1u;goto L8c61aaaa; /* mov #0x1,r3 */
L8c61aaaa: sa_motion_publish_stop(ctx,0x8c61aaaau); r[4]&=r[2];goto L8c61aaac; /* and r2,r4 */
L8c61aaac: sa_motion_publish_stop(ctx,0x8c61aaacu); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61aaae; /* tst r3,r4 */
L8c61aaae: sa_motion_publish_stop(ctx,0x8c61aaaeu); if(!(*sr&1u))goto L8c61aaba;goto L8c61aab0; /* bf 0x8c61aaba */
L8c61aab0: sa_motion_publish_stop(ctx,0x8c61aab0u); r[1]=rd(m,bus,&io_status,0x8c61aafcu,4);if(io_status)return io_status;goto L8c61aab2; /* mov.l 0x8c61aafc,r1 */
L8c61aab2: sa_motion_publish_stop(ctx,0x8c61aab2u); r[5]=r[14];goto L8c61aab4; /* mov r14,r5 */
L8c61aab4: sa_motion_publish_stop(ctx,0x8c61aab4u); r[6]=r[12];goto L8c61aab6; /* mov r12,r6 */
L8c61aab6: sa_motion_publish_stop(ctx,0x8c61aab8u); target=r[1];*pr=0x8c61aabau;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61aaba; /* jsr @r1 */
L8c61aaba: sa_motion_publish_stop(ctx,0x8c61aabau); r[10]+=1u;goto L8c61aabc; /* add #0x1,r10 */
L8c61aabc: sa_motion_publish_stop(ctx,0x8c61aabcu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61aabe; /* mov.l @r15,r3 */
L8c61aabe: sa_motion_publish_stop(ctx,0x8c61aabeu); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61aac0; /* mov.w @(0x2,r3),r0 */
L8c61aac0: sa_motion_publish_stop(ctx,0x8c61aac0u); r[0]=r[0]&65535u;goto L8c61aac2; /* extu.w r0,r0 */
L8c61aac2: sa_motion_publish_stop(ctx,0x8c61aac2u); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c61aac4; /* cmp/ge r0,r10 */
L8c61aac4: sa_motion_publish_stop(ctx,0x8c61aac4u); if((*sr&1u))goto L8c61aaca;goto L8c61aac6; /* bt 0x8c61aaca */
L8c61aac6: sa_motion_publish_stop(ctx,0x8c61aac8u); goto L8c61a798; /* bra 0x8c61a798 */
L8c61aaca: sa_motion_publish_stop(ctx,0x8c61aaccu); goto L8c61ade8; /* bra 0x8c61ade8 */
L8c61aace: sa_motion_publish_stop(ctx,0x8c61aad0u); r[9]=0u;goto L8c61ad5c; /* bra 0x8c61ad5c */
L8c61ab00: sa_motion_publish_stop(ctx,0x8c61ab00u); r[3]=rd(m,bus,&io_status,0x8c61ae08u,4);if(io_status)return io_status;goto L8c61ab02; /* mov.l 0x8c61ae08,r3 */
L8c61ab02: sa_motion_publish_stop(ctx,0x8c61ab02u); r[6]=0u;goto L8c61ab04; /* mov #0x0,r6 */
L8c61ab04: sa_motion_publish_stop(ctx,0x8c61ab04u); r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61ab06; /* mov.w @r11+,r13 */
L8c61ab06: sa_motion_publish_stop(ctx,0x8c61ab06u); r[4]=0u;goto L8c61ab08; /* mov #0x0,r4 */
L8c61ab08: sa_motion_publish_stop(ctx,0x8c61ab08u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ab0a; /* mov.l @r3,r2 */
L8c61ab0a: sa_motion_publish_stop(ctx,0x8c61ab0au); r[5]=r[6];goto L8c61ab0c; /* mov r6,r5 */
L8c61ab0c: sa_motion_publish_stop(ctx,0x8c61ab0cu); r[13]=r[13]&65535u;goto L8c61ab0e; /* extu.w r13,r13 */
L8c61ab0e: sa_motion_publish_stop(ctx,0x8c61ab0eu); r[2]+=r[13];goto L8c61ab10; /* add r13,r2 */
L8c61ab10: sa_motion_publish_stop(ctx,0x8c61ab10u); *sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c61ab12; /* cmp/pl r13 */
L8c61ab12: sa_motion_publish_stop(ctx,0x8c61ab12u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61ab14; /* mov.l r2,@r3 */
L8c61ab14: sa_motion_publish_stop(ctx,0x8c61ab14u); r[7]=4294967295u;goto L8c61ab16; /* mov #-0x1,r7 */
L8c61ab16: sa_motion_publish_stop(ctx,0x8c61ab18u); take=!(*sr&1u);r[4]+=r[11];if(take)goto L8c61ab38;goto L8c61ab1a; /* bf/s 0x8c61ab38 */
L8c61ab1a: sa_motion_publish_stop(ctx,0x8c61ab1au); r[14]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c61ab1c; /* mov.w @r4+,r14 */
L8c61ab1c: sa_motion_publish_stop(ctx,0x8c61ab1cu); r[5]+=1u;goto L8c61ab1e; /* add #0x1,r5 */
L8c61ab1e: sa_motion_publish_stop(ctx,0x8c61ab1eu); r[2]=rd(m,bus,&io_status,0x8c61ae0cu,4);if(io_status)return io_status;goto L8c61ab20; /* mov.l 0x8c61ae0c,r2 */
L8c61ab20: sa_motion_publish_stop(ctx,0x8c61ab20u); r[14]=r[14]&65535u;goto L8c61ab22; /* extu.w r14,r14 */
L8c61ab22: sa_motion_publish_stop(ctx,0x8c61ab22u); r[14]<<=2;goto L8c61ab24; /* shll2 r14 */
L8c61ab24: sa_motion_publish_stop(ctx,0x8c61ab24u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ab26; /* mov.l @r2,r3 */
L8c61ab26: sa_motion_publish_stop(ctx,0x8c61ab26u); r[14]<<=2;goto L8c61ab28; /* shll2 r14 */
L8c61ab28: sa_motion_publish_stop(ctx,0x8c61ab28u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61ab2a; /* shll r14 */
L8c61ab2a: sa_motion_publish_stop(ctx,0x8c61ab2au); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[13]);goto L8c61ab2c; /* cmp/ge r13,r5 */
L8c61ab2c: sa_motion_publish_stop(ctx,0x8c61ab2cu); r[14]+=r[3];goto L8c61ab2e; /* add r3,r14 */
L8c61ab2e: sa_motion_publish_stop(ctx,0x8c61ab2eu); wr(m,bus,&io_status,r[15]+0x4u,r[14],4);if(io_status)return io_status;goto L8c61ab30; /* mov.l r14,@(0x4,r15) */
L8c61ab30: sa_motion_publish_stop(ctx,0x8c61ab30u); r[14]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61ab32; /* mov.l @(0xc,r14),r14 */
L8c61ab32: sa_motion_publish_stop(ctx,0x8c61ab32u); r[7]&=r[14];goto L8c61ab34; /* and r14,r7 */
L8c61ab34: sa_motion_publish_stop(ctx,0x8c61ab36u); take=!(*sr&1u);r[6]|=r[14];if(take)goto L8c61ab1a;goto L8c61ab38; /* bf/s 0x8c61ab1a */
L8c61ab38: sa_motion_publish_stop(ctx,0x8c61ab38u); r[2]=63u;goto L8c61ab3a; /* mov #0x3f,r2 */
L8c61ab3a: sa_motion_publish_stop(ctx,0x8c61ab3au); *sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c61ab3c; /* tst r2,r7 */
L8c61ab3c: sa_motion_publish_stop(ctx,0x8c61ab3cu); if(!(*sr&1u))goto L8c61ab9c;goto L8c61ab3e; /* bf 0x8c61ab9c */
L8c61ab3e: sa_motion_publish_stop(ctx,0x8c61ab3eu); r[1]=1u;goto L8c61ab40; /* mov #0x1,r1 */
L8c61ab40: sa_motion_publish_stop(ctx,0x8c61ab40u); *sr=(*sr&~1u)|!(r[1]&r[6]);goto L8c61ab42; /* tst r1,r6 */
L8c61ab42: sa_motion_publish_stop(ctx,0x8c61ab42u); if((*sr&1u))goto L8c61ab48;goto L8c61ab44; /* bt 0x8c61ab48 */
L8c61ab44: sa_motion_publish_stop(ctx,0x8c61ab46u); goto L8c61acfc; /* bra 0x8c61acfc */
L8c61ab48: sa_motion_publish_stop(ctx,0x8c61ab48u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c61ab4a; /* mov.w @(0x2,r11),r0 */
L8c61ab4a: sa_motion_publish_stop(ctx,0x8c61ab4au); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c61ab4c; /* mov.w @r11,r4 */
L8c61ab4c: sa_motion_publish_stop(ctx,0x8c61ab4cu); r[3]=rd(m,bus,&io_status,0x8c61ae0cu,4);if(io_status)return io_status;goto L8c61ab4e; /* mov.l 0x8c61ae0c,r3 */
L8c61ab4e: sa_motion_publish_stop(ctx,0x8c61ab4eu); r[5]=r[0]&65535u;goto L8c61ab50; /* extu.w r0,r5 */
L8c61ab50: sa_motion_publish_stop(ctx,0x8c61ab50u); r[5]<<=2;goto L8c61ab52; /* shll2 r5 */
L8c61ab52: sa_motion_publish_stop(ctx,0x8c61ab52u); r[0]=sx(m,bus,&io_status,r[11]+0x4u,2);if(io_status)return io_status;goto L8c61ab54; /* mov.w @(0x4,r11),r0 */
L8c61ab54: sa_motion_publish_stop(ctx,0x8c61ab54u); r[4]=r[4]&65535u;goto L8c61ab56; /* extu.w r4,r4 */
L8c61ab56: sa_motion_publish_stop(ctx,0x8c61ab56u); r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ab58; /* mov.l @r3,r7 */
L8c61ab58: sa_motion_publish_stop(ctx,0x8c61ab58u); r[4]<<=2;goto L8c61ab5a; /* shll2 r4 */
L8c61ab5a: sa_motion_publish_stop(ctx,0x8c61ab5au); r[5]<<=2;goto L8c61ab5c; /* shll2 r5 */
L8c61ab5c: sa_motion_publish_stop(ctx,0x8c61ab5cu); r[6]=r[0]&65535u;goto L8c61ab5e; /* extu.w r0,r6 */
L8c61ab5e: sa_motion_publish_stop(ctx,0x8c61ab5eu); r[4]<<=2;goto L8c61ab60; /* shll2 r4 */
L8c61ab60: sa_motion_publish_stop(ctx,0x8c61ab60u); r[0]=16u;goto L8c61ab62; /* mov #0x10,r0 */
L8c61ab62: sa_motion_publish_stop(ctx,0x8c61ab62u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61ab64; /* shll r4 */
L8c61ab64: sa_motion_publish_stop(ctx,0x8c61ab64u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61ab66; /* shll r5 */
L8c61ab66: sa_motion_publish_stop(ctx,0x8c61ab66u); r[5]+=r[7];goto L8c61ab68; /* add r7,r5 */
L8c61ab68: sa_motion_publish_stop(ctx,0x8c61ab68u); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c61ab6a; /* fmov.s @(r0,r5),fr5 */
L8c61ab6a: sa_motion_publish_stop(ctx,0x8c61ab6au); r[6]<<=2;goto L8c61ab6c; /* shll2 r6 */
L8c61ab6c: sa_motion_publish_stop(ctx,0x8c61ab6cu); r[4]+=r[7];goto L8c61ab6e; /* add r7,r4 */
L8c61ab6e: sa_motion_publish_stop(ctx,0x8c61ab6eu); r[6]<<=2;goto L8c61ab70; /* shll2 r6 */
L8c61ab70: sa_motion_publish_stop(ctx,0x8c61ab70u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ab72; /* fmov.s @(r0,r4),fr3 */
L8c61ab72: sa_motion_publish_stop(ctx,0x8c61ab72u); fcopy(f,2u,5u);goto L8c61ab74; /* fmov fr5,fr2 */
L8c61ab74: sa_motion_publish_stop(ctx,0x8c61ab74u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61ab76; /* shll r6 */
L8c61ab76: sa_motion_publish_stop(ctx,0x8c61ab76u); r[0]=20u;goto L8c61ab78; /* mov #0x14,r0 */
L8c61ab78: sa_motion_publish_stop(ctx,0x8c61ab78u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61ab7a; /* fsub fr3,fr2 */
L8c61ab7a: sa_motion_publish_stop(ctx,0x8c61ab7au); r[6]+=r[7];goto L8c61ab7c; /* add r7,r6 */
L8c61ab7c: sa_motion_publish_stop(ctx,0x8c61ab7cu); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c61ab7e; /* fmov.s @(r0,r5),fr4 */
L8c61ab7e: sa_motion_publish_stop(ctx,0x8c61ab7eu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61ab80; /* fmov.s @(r0,r6),fr3 */
L8c61ab80: sa_motion_publish_stop(ctx,0x8c61ab80u); fcopy(f,1u,4u);goto L8c61ab82; /* fmov fr4,fr1 */
L8c61ab82: sa_motion_publish_stop(ctx,0x8c61ab82u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ab84; /* fsub fr4,fr3 */
L8c61ab84: sa_motion_publish_stop(ctx,0x8c61ab84u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ab86; /* fmul fr3,fr2 */
L8c61ab86: sa_motion_publish_stop(ctx,0x8c61ab86u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ab88; /* fmov.s @(r0,r4),fr3 */
L8c61ab88: sa_motion_publish_stop(ctx,0x8c61ab88u); r[0]=16u;goto L8c61ab8a; /* mov #0x10,r0 */
L8c61ab8a: sa_motion_publish_stop(ctx,0x8c61ab8au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61ab8c; /* fsub fr3,fr1 */
L8c61ab8c: sa_motion_publish_stop(ctx,0x8c61ab8cu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61ab8e; /* fmov.s @(r0,r6),fr3 */
L8c61ab8e: sa_motion_publish_stop(ctx,0x8c61ab8eu); fcopy(f,4u,2u);goto L8c61ab90; /* fmov fr2,fr4 */
L8c61ab90: sa_motion_publish_stop(ctx,0x8c61ab90u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ab92; /* fsub fr5,fr3 */
L8c61ab92: sa_motion_publish_stop(ctx,0x8c61ab92u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ab94; /* fmul fr3,fr1 */
L8c61ab94: sa_motion_publish_stop(ctx,0x8c61ab94u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61ab96; /* fldi0 fr3 */
L8c61ab96: sa_motion_publish_stop(ctx,0x8c61ab96u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61ab98; /* fsub fr1,fr4 */
L8c61ab98: sa_motion_publish_stop(ctx,0x8c61ab98u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61ab9a; /* fcmp/gt fr3,fr4 */
L8c61ab9a: sa_motion_publish_stop(ctx,0x8c61ab9au); if(!(*sr&1u))goto L8c61aba2;goto L8c61ab9c; /* bf 0x8c61aba2 */
L8c61ab9c: sa_motion_publish_stop(ctx,0x8c61ab9cu); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61ab9e; /* shll r13 */
L8c61ab9e: sa_motion_publish_stop(ctx,0x8c61aba0u); r[11]+=r[13];goto L8c61ad5a; /* bra 0x8c61ad5a */
L8c61aba2: sa_motion_publish_stop(ctx,0x8c61aba2u); r[2]=rd(m,bus,&io_status,0x8c61ae10u,4);if(io_status)return io_status;goto L8c61aba4; /* mov.l 0x8c61ae10,r2 */
L8c61aba4: sa_motion_publish_stop(ctx,0x8c61aba4u); r[6]=r[13];goto L8c61aba6; /* mov r13,r6 */
L8c61aba6: sa_motion_publish_stop(ctx,0x8c61aba6u); r[6]+=4294967295u;goto L8c61aba8; /* add #-0x1,r6 */
L8c61aba8: sa_motion_publish_stop(ctx,0x8c61aba8u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61abaa; /* mov.l @r2,r3 */
L8c61abaa: sa_motion_publish_stop(ctx,0x8c61abaau); *sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c61abac; /* cmp/pl r6 */
L8c61abac: sa_motion_publish_stop(ctx,0x8c61abacu); r[3]+=r[13];goto L8c61abae; /* add r13,r3 */
L8c61abae: sa_motion_publish_stop(ctx,0x8c61abaeu); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61abb0; /* mov.l r3,@r2 */
L8c61abb0: sa_motion_publish_stop(ctx,0x8c61abb2u); take=!(*sr&1u);r[5]=0u;if(take)goto L8c61ac5a;goto L8c61abb4; /* bf/s 0x8c61ac5a */
L8c61abb4: sa_motion_publish_stop(ctx,0x8c61abb4u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61abb6; /* mov.w @r11+,r4 */
L8c61abb6: sa_motion_publish_stop(ctx,0x8c61abb6u); r[0]=4u;goto L8c61abb8; /* mov #0x4,r0 */
L8c61abb8: sa_motion_publish_stop(ctx,0x8c61abb8u); r[2]=rd(m,bus,&io_status,0x8c61ae0cu,4);if(io_status)return io_status;goto L8c61abba; /* mov.l 0x8c61ae0c,r2 */
L8c61abba: sa_motion_publish_stop(ctx,0x8c61abbau); r[8]+=28u;goto L8c61abbc; /* add #0x1c,r8 */
L8c61abbc: sa_motion_publish_stop(ctx,0x8c61abbcu); r[4]=r[4]&65535u;goto L8c61abbe; /* extu.w r4,r4 */
L8c61abbe: sa_motion_publish_stop(ctx,0x8c61abbeu); r[1]=rd(m,bus,&io_status,0x8c61ae18u,4);if(io_status)return io_status;goto L8c61abc0; /* mov.l 0x8c61ae18,r1 */
L8c61abc0: sa_motion_publish_stop(ctx,0x8c61abc0u); r[4]<<=2;goto L8c61abc2; /* shll2 r4 */
L8c61abc2: sa_motion_publish_stop(ctx,0x8c61abc2u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61abc4; /* mov.l @r2,r3 */
L8c61abc4: sa_motion_publish_stop(ctx,0x8c61abc4u); r[4]<<=2;goto L8c61abc6; /* shll2 r4 */
L8c61abc6: sa_motion_publish_stop(ctx,0x8c61abc6u); r[2]=rd(m,bus,&io_status,0x8c61ae1cu,4);if(io_status)return io_status;goto L8c61abc8; /* mov.l 0x8c61ae1c,r2 */
L8c61abc8: sa_motion_publish_stop(ctx,0x8c61abc8u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61abca; /* shll r4 */
L8c61abca: sa_motion_publish_stop(ctx,0x8c61abcau); r[4]+=r[3];goto L8c61abcc; /* add r3,r4 */
L8c61abcc: sa_motion_publish_stop(ctx,0x8c61abccu); r[3]=rd(m,bus,&io_status,0x8c61ae14u,4);if(io_status)return io_status;goto L8c61abce; /* mov.l 0x8c61ae14,r3 */
L8c61abce: sa_motion_publish_stop(ctx,0x8c61abceu); fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61abd0; /* fmov.s @r4,fr3 */
L8c61abd0: sa_motion_publish_stop(ctx,0x8c61abd0u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61abd2; /* fmov.s @r3,fr2 */
L8c61abd2: sa_motion_publish_stop(ctx,0x8c61abd2u); r[3]+=8u;goto L8c61abd4; /* add #0x8,r3 */
L8c61abd4: sa_motion_publish_stop(ctx,0x8c61abd4u); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61abd6; /* fmov.s @r2,fr1 */
L8c61abd6: sa_motion_publish_stop(ctx,0x8c61abd6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61abd8; /* fmul fr3,fr2 */
L8c61abd8: sa_motion_publish_stop(ctx,0x8c61abd8u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61abda; /* fmov.s @r1,fr3 */
L8c61abda: sa_motion_publish_stop(ctx,0x8c61abdau); r[2]=rd(m,bus,&io_status,0x8c61ae20u,4);if(io_status)return io_status;goto L8c61abdc; /* mov.l 0x8c61ae20,r2 */
L8c61abdc: sa_motion_publish_stop(ctx,0x8c61abdcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61abde; /* fsub fr2,fr3 */
L8c61abde: sa_motion_publish_stop(ctx,0x8c61abdeu); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61abe0; /* fmov.s @(r0,r4),fr2 */
L8c61abe0: sa_motion_publish_stop(ctx,0x8c61abe0u); r[0]=8u;goto L8c61abe2; /* mov #0x8,r0 */
L8c61abe2: sa_motion_publish_stop(ctx,0x8c61abe2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61abe4; /* fmul fr2,fr1 */
L8c61abe4: sa_motion_publish_stop(ctx,0x8c61abe4u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61abe6; /* fmov.s @(r0,r4),fr2 */
L8c61abe6: sa_motion_publish_stop(ctx,0x8c61abe6u); r[0]=4u;goto L8c61abe8; /* mov #0x4,r0 */
L8c61abe8: sa_motion_publish_stop(ctx,0x8c61abe8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61abea; /* fsub fr1,fr3 */
L8c61abea: sa_motion_publish_stop(ctx,0x8c61abeau); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61abec; /* fmov.s @r3,fr1 */
L8c61abec: sa_motion_publish_stop(ctx,0x8c61abecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61abee; /* fmul fr2,fr1 */
L8c61abee: sa_motion_publish_stop(ctx,0x8c61abeeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61abf0; /* fsub fr1,fr3 */
L8c61abf0: sa_motion_publish_stop(ctx,0x8c61abf0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61abf2; /* fmov.s fr3,@-r8 */
L8c61abf2: sa_motion_publish_stop(ctx,0x8c61abf2u); r[14]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61abf4; /* mov.l @r2,r14 */
L8c61abf4: sa_motion_publish_stop(ctx,0x8c61abf4u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61abf6; /* fmov.s @(r0,r4),fr0 */
L8c61abf6: sa_motion_publish_stop(ctx,0x8c61abf6u); r[0]=20u;goto L8c61abf8; /* mov #0x14,r0 */
L8c61abf8: sa_motion_publish_stop(ctx,0x8c61abf8u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61abfa; /* fmov.s @(r0,r14),fr3 */
L8c61abfa: sa_motion_publish_stop(ctx,0x8c61abfau); r[0]=4u;goto L8c61abfc; /* mov #0x4,r0 */
L8c61abfc: sa_motion_publish_stop(ctx,0x8c61abfcu); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61abfe; /* fmov.s @(r0,r14),fr2 */
L8c61abfe: sa_motion_publish_stop(ctx,0x8c61abfeu); r[0]=8u;goto L8c61ac00; /* mov #0x8,r0 */
L8c61ac00: sa_motion_publish_stop(ctx,0x8c61ac00u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ac02; /* fmul fr0,fr3 */
L8c61ac02: sa_motion_publish_stop(ctx,0x8c61ac02u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ac04; /* fmov.s @r4,fr0 */
L8c61ac04: sa_motion_publish_stop(ctx,0x8c61ac04u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ac06; /* fmac fr0,fr2,fr3 */
L8c61ac06: sa_motion_publish_stop(ctx,0x8c61ac06u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac08; /* fmov.s @(r0,r4),fr0 */
L8c61ac08: sa_motion_publish_stop(ctx,0x8c61ac08u); r[0]=36u;goto L8c61ac0a; /* mov #0x24,r0 */
L8c61ac0a: sa_motion_publish_stop(ctx,0x8c61ac0au); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ac0c; /* fmov.s @(r0,r14),fr2 */
L8c61ac0c: sa_motion_publish_stop(ctx,0x8c61ac0cu); r[0]=4u;goto L8c61ac0e; /* mov #0x4,r0 */
L8c61ac0e: sa_motion_publish_stop(ctx,0x8c61ac0eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ac10; /* fmac fr0,fr2,fr3 */
L8c61ac10: sa_motion_publish_stop(ctx,0x8c61ac10u); fcopy(f,0u,15u);goto L8c61ac12; /* fmov fr15,fr0 */
L8c61ac12: sa_motion_publish_stop(ctx,0x8c61ac12u); fcopy(f,2u,15u);goto L8c61ac14; /* fmov fr15,fr2 */
L8c61ac14: sa_motion_publish_stop(ctx,0x8c61ac14u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ac16; /* fmac fr0,fr3,fr2 */
L8c61ac16: sa_motion_publish_stop(ctx,0x8c61ac16u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ac18; /* fmov.s fr2,@-r8 */
L8c61ac18: sa_motion_publish_stop(ctx,0x8c61ac18u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac1a; /* fmov.s @(r0,r4),fr0 */
L8c61ac1a: sa_motion_publish_stop(ctx,0x8c61ac1au); r[0]=16u;goto L8c61ac1c; /* mov #0x10,r0 */
L8c61ac1c: sa_motion_publish_stop(ctx,0x8c61ac1cu); r[7]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ac1e; /* mov.l @r2,r7 */
L8c61ac1e: sa_motion_publish_stop(ctx,0x8c61ac1eu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61ac20; /* fmov.s @(r0,r7),fr3 */
L8c61ac20: sa_motion_publish_stop(ctx,0x8c61ac20u); r[0]=8u;goto L8c61ac22; /* mov #0x8,r0 */
L8c61ac22: sa_motion_publish_stop(ctx,0x8c61ac22u); fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61ac24; /* fmov.s @r7,fr2 */
L8c61ac24: sa_motion_publish_stop(ctx,0x8c61ac24u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ac26; /* fmul fr0,fr3 */
L8c61ac26: sa_motion_publish_stop(ctx,0x8c61ac26u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ac28; /* fmov.s @r4,fr0 */
L8c61ac28: sa_motion_publish_stop(ctx,0x8c61ac28u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ac2a; /* fmac fr0,fr2,fr3 */
L8c61ac2a: sa_motion_publish_stop(ctx,0x8c61ac2au); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac2c; /* fmov.s @(r0,r4),fr0 */
L8c61ac2c: sa_motion_publish_stop(ctx,0x8c61ac2cu); r[0]=32u;goto L8c61ac2e; /* mov #0x20,r0 */
L8c61ac2e: sa_motion_publish_stop(ctx,0x8c61ac2eu); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61ac30; /* fmov.s @(r0,r7),fr2 */
L8c61ac30: sa_motion_publish_stop(ctx,0x8c61ac30u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ac32; /* fmac fr0,fr2,fr3 */
L8c61ac32: sa_motion_publish_stop(ctx,0x8c61ac32u); fcopy(f,2u,15u);goto L8c61ac34; /* fmov fr15,fr2 */
L8c61ac34: sa_motion_publish_stop(ctx,0x8c61ac34u); fcopy(f,0u,15u);goto L8c61ac36; /* fmov fr15,fr0 */
L8c61ac36: sa_motion_publish_stop(ctx,0x8c61ac36u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ac38; /* fmac fr0,fr3,fr2 */
L8c61ac38: sa_motion_publish_stop(ctx,0x8c61ac38u); r[0]=24u;goto L8c61ac3a; /* mov #0x18,r0 */
L8c61ac3a: sa_motion_publish_stop(ctx,0x8c61ac3au); r[3]=rd(m,bus,&io_status,0x8c61ae24u,4);if(io_status)return io_status;goto L8c61ac3c; /* mov.l 0x8c61ae24,r3 */
L8c61ac3c: sa_motion_publish_stop(ctx,0x8c61ac3cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ac3e; /* fmov.s fr2,@-r8 */
L8c61ac3e: sa_motion_publish_stop(ctx,0x8c61ac3eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac40; /* fmov.s @(r0,r4),fr3 */
L8c61ac40: sa_motion_publish_stop(ctx,0x8c61ac40u); r[0]=20u;goto L8c61ac42; /* mov #0x14,r0 */
L8c61ac42: sa_motion_publish_stop(ctx,0x8c61ac42u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ac44; /* fmov.s fr3,@-r8 */
L8c61ac44: sa_motion_publish_stop(ctx,0x8c61ac44u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac46; /* fmov.s @(r0,r4),fr3 */
L8c61ac46: sa_motion_publish_stop(ctx,0x8c61ac46u); r[0]=16u;goto L8c61ac48; /* mov #0x10,r0 */
L8c61ac48: sa_motion_publish_stop(ctx,0x8c61ac48u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ac4a; /* fmov.s fr3,@-r8 */
L8c61ac4a: sa_motion_publish_stop(ctx,0x8c61ac4au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac4c; /* fmov.s @(r0,r4),fr3 */
L8c61ac4c: sa_motion_publish_stop(ctx,0x8c61ac4cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ac4e; /* fmov.s fr3,@-r8 */
L8c61ac4e: sa_motion_publish_stop(ctx,0x8c61ac4eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61ac50; /* mov.l r3,@-r8 */
L8c61ac50: sa_motion_publish_stop(ctx,0x8c61ac50u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61ac52; /* pref @r8 */
L8c61ac52: sa_motion_publish_stop(ctx,0x8c61ac52u); r[5]+=1u;goto L8c61ac54; /* add #0x1,r5 */
L8c61ac54: sa_motion_publish_stop(ctx,0x8c61ac54u); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61ac56; /* cmp/ge r6,r5 */
L8c61ac56: sa_motion_publish_stop(ctx,0x8c61ac58u); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61abb4;goto L8c61ac5a; /* bf/s 0x8c61abb4 */
L8c61ac5a: sa_motion_publish_stop(ctx,0x8c61ac5au); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61ac5c; /* mov.w @r11+,r4 */
L8c61ac5c: sa_motion_publish_stop(ctx,0x8c61ac5cu); r[0]=4u;goto L8c61ac5e; /* mov #0x4,r0 */
L8c61ac5e: sa_motion_publish_stop(ctx,0x8c61ac5eu); r[2]=rd(m,bus,&io_status,0x8c61ae0cu,4);if(io_status)return io_status;goto L8c61ac60; /* mov.l 0x8c61ae0c,r2 */
L8c61ac60: sa_motion_publish_stop(ctx,0x8c61ac60u); r[8]+=28u;goto L8c61ac62; /* add #0x1c,r8 */
L8c61ac62: sa_motion_publish_stop(ctx,0x8c61ac62u); r[4]=r[4]&65535u;goto L8c61ac64; /* extu.w r4,r4 */
L8c61ac64: sa_motion_publish_stop(ctx,0x8c61ac64u); r[1]=rd(m,bus,&io_status,0x8c61ae18u,4);if(io_status)return io_status;goto L8c61ac66; /* mov.l 0x8c61ae18,r1 */
L8c61ac66: sa_motion_publish_stop(ctx,0x8c61ac66u); r[4]<<=2;goto L8c61ac68; /* shll2 r4 */
L8c61ac68: sa_motion_publish_stop(ctx,0x8c61ac68u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ac6a; /* mov.l @r2,r3 */
L8c61ac6a: sa_motion_publish_stop(ctx,0x8c61ac6au); r[4]<<=2;goto L8c61ac6c; /* shll2 r4 */
L8c61ac6c: sa_motion_publish_stop(ctx,0x8c61ac6cu); r[2]=rd(m,bus,&io_status,0x8c61ae1cu,4);if(io_status)return io_status;goto L8c61ac6e; /* mov.l 0x8c61ae1c,r2 */
L8c61ac6e: sa_motion_publish_stop(ctx,0x8c61ac6eu); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61ac70; /* shll r4 */
L8c61ac70: sa_motion_publish_stop(ctx,0x8c61ac70u); r[4]+=r[3];goto L8c61ac72; /* add r3,r4 */
L8c61ac72: sa_motion_publish_stop(ctx,0x8c61ac72u); r[3]=rd(m,bus,&io_status,0x8c61ae14u,4);if(io_status)return io_status;goto L8c61ac74; /* mov.l 0x8c61ae14,r3 */
L8c61ac74: sa_motion_publish_stop(ctx,0x8c61ac74u); fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c61ac76; /* fmov.s @r4,fr3 */
L8c61ac76: sa_motion_publish_stop(ctx,0x8c61ac76u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61ac78; /* fmov.s @r3,fr2 */
L8c61ac78: sa_motion_publish_stop(ctx,0x8c61ac78u); r[3]+=8u;goto L8c61ac7a; /* add #0x8,r3 */
L8c61ac7a: sa_motion_publish_stop(ctx,0x8c61ac7au); fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61ac7c; /* fmov.s @r2,fr1 */
L8c61ac7c: sa_motion_publish_stop(ctx,0x8c61ac7cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ac7e; /* fmul fr3,fr2 */
L8c61ac7e: sa_motion_publish_stop(ctx,0x8c61ac7eu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61ac80; /* fmov.s @r1,fr3 */
L8c61ac80: sa_motion_publish_stop(ctx,0x8c61ac80u); r[2]=rd(m,bus,&io_status,0x8c61ae20u,4);if(io_status)return io_status;goto L8c61ac82; /* mov.l 0x8c61ae20,r2 */
L8c61ac82: sa_motion_publish_stop(ctx,0x8c61ac82u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ac84; /* fsub fr2,fr3 */
L8c61ac84: sa_motion_publish_stop(ctx,0x8c61ac84u); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac86; /* fmov.s @(r0,r4),fr2 */
L8c61ac86: sa_motion_publish_stop(ctx,0x8c61ac86u); r[0]=8u;goto L8c61ac88; /* mov #0x8,r0 */
L8c61ac88: sa_motion_publish_stop(ctx,0x8c61ac88u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ac8a; /* fmul fr2,fr1 */
L8c61ac8a: sa_motion_publish_stop(ctx,0x8c61ac8au); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac8c; /* fmov.s @(r0,r4),fr2 */
L8c61ac8c: sa_motion_publish_stop(ctx,0x8c61ac8cu); r[0]=4u;goto L8c61ac8e; /* mov #0x4,r0 */
L8c61ac8e: sa_motion_publish_stop(ctx,0x8c61ac8eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ac90; /* fsub fr1,fr3 */
L8c61ac90: sa_motion_publish_stop(ctx,0x8c61ac90u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61ac92; /* fmov.s @r3,fr1 */
L8c61ac92: sa_motion_publish_stop(ctx,0x8c61ac92u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ac94; /* fmul fr2,fr1 */
L8c61ac94: sa_motion_publish_stop(ctx,0x8c61ac94u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ac96; /* fsub fr1,fr3 */
L8c61ac96: sa_motion_publish_stop(ctx,0x8c61ac96u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ac98; /* fmov.s fr3,@-r8 */
L8c61ac98: sa_motion_publish_stop(ctx,0x8c61ac98u); r[6]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ac9a; /* mov.l @r2,r6 */
L8c61ac9a: sa_motion_publish_stop(ctx,0x8c61ac9au); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ac9c; /* fmov.s @(r0,r4),fr0 */
L8c61ac9c: sa_motion_publish_stop(ctx,0x8c61ac9cu); r[0]=20u;goto L8c61ac9e; /* mov #0x14,r0 */
L8c61ac9e: sa_motion_publish_stop(ctx,0x8c61ac9eu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61aca0; /* fmov.s @(r0,r6),fr3 */
L8c61aca0: sa_motion_publish_stop(ctx,0x8c61aca0u); r[0]=4u;goto L8c61aca2; /* mov #0x4,r0 */
L8c61aca2: sa_motion_publish_stop(ctx,0x8c61aca2u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61aca4; /* fmov.s @(r0,r6),fr2 */
L8c61aca4: sa_motion_publish_stop(ctx,0x8c61aca4u); r[0]=8u;goto L8c61aca6; /* mov #0x8,r0 */
L8c61aca6: sa_motion_publish_stop(ctx,0x8c61aca6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61aca8; /* fmul fr0,fr3 */
L8c61aca8: sa_motion_publish_stop(ctx,0x8c61aca8u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61acaa; /* fmov.s @r4,fr0 */
L8c61acaa: sa_motion_publish_stop(ctx,0x8c61acaau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61acac; /* fmac fr0,fr2,fr3 */
L8c61acac: sa_motion_publish_stop(ctx,0x8c61acacu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61acae; /* fmov.s @(r0,r4),fr0 */
L8c61acae: sa_motion_publish_stop(ctx,0x8c61acaeu); r[0]=36u;goto L8c61acb0; /* mov #0x24,r0 */
L8c61acb0: sa_motion_publish_stop(ctx,0x8c61acb0u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61acb2; /* fmov.s @(r0,r6),fr2 */
L8c61acb2: sa_motion_publish_stop(ctx,0x8c61acb2u); r[0]=4u;goto L8c61acb4; /* mov #0x4,r0 */
L8c61acb4: sa_motion_publish_stop(ctx,0x8c61acb4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61acb6; /* fmac fr0,fr2,fr3 */
L8c61acb6: sa_motion_publish_stop(ctx,0x8c61acb6u); fcopy(f,0u,15u);goto L8c61acb8; /* fmov fr15,fr0 */
L8c61acb8: sa_motion_publish_stop(ctx,0x8c61acb8u); fcopy(f,2u,15u);goto L8c61acba; /* fmov fr15,fr2 */
L8c61acba: sa_motion_publish_stop(ctx,0x8c61acbau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61acbc; /* fmac fr0,fr3,fr2 */
L8c61acbc: sa_motion_publish_stop(ctx,0x8c61acbcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61acbe; /* fmov.s fr2,@-r8 */
L8c61acbe: sa_motion_publish_stop(ctx,0x8c61acbeu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61acc0; /* fmov.s @(r0,r4),fr0 */
L8c61acc0: sa_motion_publish_stop(ctx,0x8c61acc0u); r[0]=16u;goto L8c61acc2; /* mov #0x10,r0 */
L8c61acc2: sa_motion_publish_stop(ctx,0x8c61acc2u); r[5]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61acc4; /* mov.l @r2,r5 */
L8c61acc4: sa_motion_publish_stop(ctx,0x8c61acc4u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61acc6; /* fmov.s @(r0,r5),fr3 */
L8c61acc6: sa_motion_publish_stop(ctx,0x8c61acc6u); r[0]=8u;goto L8c61acc8; /* mov #0x8,r0 */
L8c61acc8: sa_motion_publish_stop(ctx,0x8c61acc8u); fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c61acca; /* fmov.s @r5,fr2 */
L8c61acca: sa_motion_publish_stop(ctx,0x8c61accau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61accc; /* fmul fr0,fr3 */
L8c61accc: sa_motion_publish_stop(ctx,0x8c61acccu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61acce; /* fmov.s @r4,fr0 */
L8c61acce: sa_motion_publish_stop(ctx,0x8c61acceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61acd0; /* fmac fr0,fr2,fr3 */
L8c61acd0: sa_motion_publish_stop(ctx,0x8c61acd0u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61acd2; /* fmov.s @(r0,r4),fr0 */
L8c61acd2: sa_motion_publish_stop(ctx,0x8c61acd2u); r[0]=32u;goto L8c61acd4; /* mov #0x20,r0 */
L8c61acd4: sa_motion_publish_stop(ctx,0x8c61acd4u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61acd6; /* fmov.s @(r0,r5),fr2 */
L8c61acd6: sa_motion_publish_stop(ctx,0x8c61acd6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61acd8; /* fmac fr0,fr2,fr3 */
L8c61acd8: sa_motion_publish_stop(ctx,0x8c61acd8u); fcopy(f,2u,15u);goto L8c61acda; /* fmov fr15,fr2 */
L8c61acda: sa_motion_publish_stop(ctx,0x8c61acdau); fcopy(f,0u,15u);goto L8c61acdc; /* fmov fr15,fr0 */
L8c61acdc: sa_motion_publish_stop(ctx,0x8c61acdcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61acde; /* fmac fr0,fr3,fr2 */
L8c61acde: sa_motion_publish_stop(ctx,0x8c61acdeu); r[0]=24u;goto L8c61ace0; /* mov #0x18,r0 */
L8c61ace0: sa_motion_publish_stop(ctx,0x8c61ace0u); r[3]=rd(m,bus,&io_status,0x8c61ae28u,4);if(io_status)return io_status;goto L8c61ace2; /* mov.l 0x8c61ae28,r3 */
L8c61ace2: sa_motion_publish_stop(ctx,0x8c61ace2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ace4; /* fmov.s fr2,@-r8 */
L8c61ace4: sa_motion_publish_stop(ctx,0x8c61ace4u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ace6; /* fmov.s @(r0,r4),fr3 */
L8c61ace6: sa_motion_publish_stop(ctx,0x8c61ace6u); r[0]=20u;goto L8c61ace8; /* mov #0x14,r0 */
L8c61ace8: sa_motion_publish_stop(ctx,0x8c61ace8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61acea; /* fmov.s fr3,@-r8 */
L8c61acea: sa_motion_publish_stop(ctx,0x8c61aceau); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61acec; /* fmov.s @(r0,r4),fr3 */
L8c61acec: sa_motion_publish_stop(ctx,0x8c61acecu); r[0]=16u;goto L8c61acee; /* mov #0x10,r0 */
L8c61acee: sa_motion_publish_stop(ctx,0x8c61aceeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61acf0; /* fmov.s fr3,@-r8 */
L8c61acf0: sa_motion_publish_stop(ctx,0x8c61acf0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61acf2; /* fmov.s @(r0,r4),fr3 */
L8c61acf2: sa_motion_publish_stop(ctx,0x8c61acf2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61acf4; /* fmov.s fr3,@-r8 */
L8c61acf4: sa_motion_publish_stop(ctx,0x8c61acf4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61acf6; /* mov.l r3,@-r8 */
L8c61acf6: sa_motion_publish_stop(ctx,0x8c61acf6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61acf8; /* pref @r8 */
L8c61acf8: sa_motion_publish_stop(ctx,0x8c61acfau); r[8]+=32u;goto L8c61ad5a; /* bra 0x8c61ad5a */
L8c61acfc: sa_motion_publish_stop(ctx,0x8c61acfcu); r[2]=rd(m,bus,&io_status,0x8c61ae2cu,4);if(io_status)return io_status;goto L8c61acfe; /* mov.l 0x8c61ae2c,r2 */
L8c61acfe: sa_motion_publish_stop(ctx,0x8c61acfeu); r[3]=0u;goto L8c61ad00; /* mov #0x0,r3 */
L8c61ad00: sa_motion_publish_stop(ctx,0x8c61ad00u); r[0]=rd(m,bus,&io_status,0x8c61ae0cu,4);if(io_status)return io_status;goto L8c61ad02; /* mov.l 0x8c61ae0c,r0 */
L8c61ad02: sa_motion_publish_stop(ctx,0x8c61ad02u); r[10]=r[13];goto L8c61ad04; /* mov r13,r10 */
L8c61ad04: sa_motion_publish_stop(ctx,0x8c61ad04u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61ad06; /* mov.l r3,@r2 */
L8c61ad06: sa_motion_publish_stop(ctx,0x8c61ad06u); r[10]+=4294967294u;goto L8c61ad08; /* add #-0x2,r10 */
L8c61ad08: sa_motion_publish_stop(ctx,0x8c61ad08u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61ad0a; /* mov.w @r11+,r4 */
L8c61ad0a: sa_motion_publish_stop(ctx,0x8c61ad0au); r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61ad0c; /* mov.l @r0,r3 */
L8c61ad0c: sa_motion_publish_stop(ctx,0x8c61ad0cu); r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61ad0e; /* mov.w @r11+,r14 */
L8c61ad0e: sa_motion_publish_stop(ctx,0x8c61ad0eu); r[4]=r[4]&65535u;goto L8c61ad10; /* extu.w r4,r4 */
L8c61ad10: sa_motion_publish_stop(ctx,0x8c61ad10u); r[4]<<=2;goto L8c61ad12; /* shll2 r4 */
L8c61ad12: sa_motion_publish_stop(ctx,0x8c61ad12u); r[14]=r[14]&65535u;goto L8c61ad14; /* extu.w r14,r14 */
L8c61ad14: sa_motion_publish_stop(ctx,0x8c61ad14u); r[14]<<=2;goto L8c61ad16; /* shll2 r14 */
L8c61ad16: sa_motion_publish_stop(ctx,0x8c61ad16u); r[14]<<=2;goto L8c61ad18; /* shll2 r14 */
L8c61ad18: sa_motion_publish_stop(ctx,0x8c61ad18u); r[4]<<=2;goto L8c61ad1a; /* shll2 r4 */
L8c61ad1a: sa_motion_publish_stop(ctx,0x8c61ad1au); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61ad1c; /* shll r4 */
L8c61ad1c: sa_motion_publish_stop(ctx,0x8c61ad1cu); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61ad1e; /* shll r14 */
L8c61ad1e: sa_motion_publish_stop(ctx,0x8c61ad1eu); *sr=(*sr&~1u)|((int32_t)r[10]>0);goto L8c61ad20; /* cmp/pl r10 */
L8c61ad20: sa_motion_publish_stop(ctx,0x8c61ad20u); r[4]+=r[3];goto L8c61ad22; /* add r3,r4 */
L8c61ad22: sa_motion_publish_stop(ctx,0x8c61ad22u); r[14]+=r[3];goto L8c61ad24; /* add r3,r14 */
L8c61ad24: sa_motion_publish_stop(ctx,0x8c61ad26u); take=!(*sr&1u);r[12]=0u;if(take)goto L8c61ad5a;goto L8c61ad28; /* bf/s 0x8c61ad5a */
L8c61ad28: sa_motion_publish_stop(ctx,0x8c61ad28u); r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c61ad2a; /* mov.w @r11+,r13 */
L8c61ad2a: sa_motion_publish_stop(ctx,0x8c61ad2au); r[2]=rd(m,bus,&io_status,0x8c61ae0cu,4);if(io_status)return io_status;goto L8c61ad2c; /* mov.l 0x8c61ae0c,r2 */
L8c61ad2c: sa_motion_publish_stop(ctx,0x8c61ad2cu); r[13]=r[13]&65535u;goto L8c61ad2e; /* extu.w r13,r13 */
L8c61ad2e: sa_motion_publish_stop(ctx,0x8c61ad2eu); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c61ad30; /* mov.l @(0xc,r4),r5 */
L8c61ad30: sa_motion_publish_stop(ctx,0x8c61ad30u); r[13]<<=2;goto L8c61ad32; /* shll2 r13 */
L8c61ad32: sa_motion_publish_stop(ctx,0x8c61ad32u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ad34; /* mov.l @r2,r3 */
L8c61ad34: sa_motion_publish_stop(ctx,0x8c61ad34u); r[13]<<=2;goto L8c61ad36; /* shll2 r13 */
L8c61ad36: sa_motion_publish_stop(ctx,0x8c61ad36u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61ad38; /* shll r13 */
L8c61ad38: sa_motion_publish_stop(ctx,0x8c61ad38u); r[13]+=r[3];goto L8c61ad3a; /* add r3,r13 */
L8c61ad3a: sa_motion_publish_stop(ctx,0x8c61ad3au); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61ad3c; /* mov.l @(0xc,r14),r3 */
L8c61ad3c: sa_motion_publish_stop(ctx,0x8c61ad3cu); r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61ad3e; /* mov.l @(0xc,r13),r1 */
L8c61ad3e: sa_motion_publish_stop(ctx,0x8c61ad3eu); r[5]&=r[3];goto L8c61ad40; /* and r3,r5 */
L8c61ad40: sa_motion_publish_stop(ctx,0x8c61ad40u); r[3]=1u;goto L8c61ad42; /* mov #0x1,r3 */
L8c61ad42: sa_motion_publish_stop(ctx,0x8c61ad42u); r[5]&=r[1];goto L8c61ad44; /* and r1,r5 */
L8c61ad44: sa_motion_publish_stop(ctx,0x8c61ad44u); *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c61ad46; /* tst r3,r5 */
L8c61ad46: sa_motion_publish_stop(ctx,0x8c61ad46u); if(!(*sr&1u))goto L8c61ad50;goto L8c61ad48; /* bf 0x8c61ad50 */
L8c61ad48: sa_motion_publish_stop(ctx,0x8c61ad48u); r[1]=rd(m,bus,&io_status,0x8c61ae30u,4);if(io_status)return io_status;goto L8c61ad4a; /* mov.l 0x8c61ae30,r1 */
L8c61ad4a: sa_motion_publish_stop(ctx,0x8c61ad4au); r[6]=r[13];goto L8c61ad4c; /* mov r13,r6 */
L8c61ad4c: sa_motion_publish_stop(ctx,0x8c61ad4eu); target=r[1];*pr=0x8c61ad50u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ad50; /* jsr @r1 */
L8c61ad50: sa_motion_publish_stop(ctx,0x8c61ad50u); r[12]+=1u;goto L8c61ad52; /* add #0x1,r12 */
L8c61ad52: sa_motion_publish_stop(ctx,0x8c61ad52u); r[4]=r[14];goto L8c61ad54; /* mov r14,r4 */
L8c61ad54: sa_motion_publish_stop(ctx,0x8c61ad54u); *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[10]);goto L8c61ad56; /* cmp/ge r10,r12 */
L8c61ad56: sa_motion_publish_stop(ctx,0x8c61ad58u); take=!(*sr&1u);r[14]=r[13];if(take)goto L8c61ad28;goto L8c61ad5a; /* bf/s 0x8c61ad28 */
L8c61ad5a: sa_motion_publish_stop(ctx,0x8c61ad5au); r[9]+=1u;goto L8c61ad5c; /* add #0x1,r9 */
L8c61ad5c: sa_motion_publish_stop(ctx,0x8c61ad5cu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ad5e; /* mov.l @r15,r2 */
L8c61ad5e: sa_motion_publish_stop(ctx,0x8c61ad5eu); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61ad60; /* mov.w @(0x2,r2),r0 */
L8c61ad60: sa_motion_publish_stop(ctx,0x8c61ad60u); r[0]=r[0]&65535u;goto L8c61ad62; /* extu.w r0,r0 */
L8c61ad62: sa_motion_publish_stop(ctx,0x8c61ad62u); *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61ad64; /* cmp/ge r0,r9 */
L8c61ad64: sa_motion_publish_stop(ctx,0x8c61ad64u); if((*sr&1u))goto L8c61ad6a;goto L8c61ad66; /* bt 0x8c61ad6a */
L8c61ad66: sa_motion_publish_stop(ctx,0x8c61ad68u); goto L8c61ab00; /* bra 0x8c61ab00 */
L8c61ad6a: sa_motion_publish_stop(ctx,0x8c61ad6cu); goto L8c61ade8; /* bra 0x8c61ade8 */
L8c61ad6e: sa_motion_publish_stop(ctx,0x8c61ad6eu); r[2]=rd(m,bus,&io_status,0x8c61ae34u,4);if(io_status)return io_status;goto L8c61ad70; /* mov.l 0x8c61ae34,r2 */
L8c61ad70: sa_motion_publish_stop(ctx,0x8c61ad70u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ad72; /* mov.l @r15,r1 */
L8c61ad72: sa_motion_publish_stop(ctx,0x8c61ad72u); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61ad74; /* mov.l r1,@r2 */
L8c61ad74: sa_motion_publish_stop(ctx,0x8c61ad74u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61ad76; /* frchg */
L8c61ad76: sa_motion_publish_stop(ctx,0x8c61ad76u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61ad78; /* fmov.s fr0,@-r15 */
L8c61ad78: sa_motion_publish_stop(ctx,0x8c61ad78u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61ad7a; /* fmov.s fr1,@-r15 */
L8c61ad7a: sa_motion_publish_stop(ctx,0x8c61ad7au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61ad7c; /* fmov.s fr2,@-r15 */
L8c61ad7c: sa_motion_publish_stop(ctx,0x8c61ad7cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61ad7e; /* fmov.s fr3,@-r15 */
L8c61ad7e: sa_motion_publish_stop(ctx,0x8c61ad7eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61ad80; /* fmov.s fr4,@-r15 */
L8c61ad80: sa_motion_publish_stop(ctx,0x8c61ad80u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61ad82; /* fmov.s fr5,@-r15 */
L8c61ad82: sa_motion_publish_stop(ctx,0x8c61ad82u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61ad84; /* fmov.s fr6,@-r15 */
L8c61ad84: sa_motion_publish_stop(ctx,0x8c61ad84u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61ad86; /* fmov.s fr7,@-r15 */
L8c61ad86: sa_motion_publish_stop(ctx,0x8c61ad86u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61ad88; /* fmov.s fr8,@-r15 */
L8c61ad88: sa_motion_publish_stop(ctx,0x8c61ad88u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61ad8a; /* fmov.s fr9,@-r15 */
L8c61ad8a: sa_motion_publish_stop(ctx,0x8c61ad8au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c61ad8c; /* fmov.s fr10,@-r15 */
L8c61ad8c: sa_motion_publish_stop(ctx,0x8c61ad8cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c61ad8e; /* fmov.s fr11,@-r15 */
L8c61ad8e: sa_motion_publish_stop(ctx,0x8c61ad8eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c61ad90; /* fmov.s fr12,@-r15 */
L8c61ad90: sa_motion_publish_stop(ctx,0x8c61ad90u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61ad92; /* fmov.s fr13,@-r15 */
L8c61ad92: sa_motion_publish_stop(ctx,0x8c61ad92u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61ad94; /* fmov.s fr14,@-r15 */
L8c61ad94: sa_motion_publish_stop(ctx,0x8c61ad94u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61ad96; /* fmov.s fr15,@-r15 */
L8c61ad96: sa_motion_publish_stop(ctx,0x8c61ad96u); r[0]=0x8c61adfcu;goto L8c61ad98; /* mova 0x8c61adfc,r0 */
L8c61ad98: sa_motion_publish_stop(ctx,0x8c61ad98u); fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c61ad9a; /* fmov.s @r0,fr12 */
L8c61ad9a: sa_motion_publish_stop(ctx,0x8c61ad9au); fcopy(f,13u,12u);goto L8c61ad9c; /* fmov fr12,fr13 */
L8c61ad9c: sa_motion_publish_stop(ctx,0x8c61ad9cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[12],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61ad9e; /* fmul fr12,fr0 */
L8c61ad9e: sa_motion_publish_stop(ctx,0x8c61ad9eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61ada0; /* fmul fr12,fr4 */
L8c61ada0: sa_motion_publish_stop(ctx,0x8c61ada0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[12],&f->fpscr,&fr[8]);if(status)return status;}goto L8c61ada2; /* fmul fr12,fr8 */
L8c61ada2: sa_motion_publish_stop(ctx,0x8c61ada2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[12],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ada4; /* fmul fr12,fr1 */
L8c61ada4: sa_motion_publish_stop(ctx,0x8c61ada4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[12],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61ada6; /* fmul fr12,fr5 */
L8c61ada6: sa_motion_publish_stop(ctx,0x8c61ada6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[12],&f->fpscr,&fr[9]);if(status)return status;}goto L8c61ada8; /* fmul fr12,fr9 */
L8c61ada8: sa_motion_publish_stop(ctx,0x8c61ada8u); r[0]=rd(m,bus,&io_status,0x8c61ae00u,4);if(io_status)return io_status;goto L8c61adaa; /* mov.l 0x8c61ae00,r0 */
L8c61adaa: sa_motion_publish_stop(ctx,0x8c61adaau); fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adac; /* fmov.s @r0+,fr2 */
L8c61adac: sa_motion_publish_stop(ctx,0x8c61adacu); fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adae; /* fmov.s @r0+,fr6 */
L8c61adae: sa_motion_publish_stop(ctx,0x8c61adaeu); fload(m,bus,&io_status,f,10u,r[0]);if(io_status)return io_status;goto L8c61adb0; /* fmov.s @r0,fr10 */
L8c61adb0: sa_motion_publish_stop(ctx,0x8c61adb0u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c61adb2; /* fneg fr2 */
L8c61adb2: sa_motion_publish_stop(ctx,0x8c61adb2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]^=0x80000000u;goto L8c61adb4; /* fneg fr6 */
L8c61adb4: sa_motion_publish_stop(ctx,0x8c61adb4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]^=0x80000000u;goto L8c61adb6; /* fneg fr10 */
L8c61adb6: sa_motion_publish_stop(ctx,0x8c61adb6u); r[0]=rd(m,bus,&io_status,0x8c61ae04u,4);if(io_status)return io_status;goto L8c61adb8; /* mov.l 0x8c61ae04,r0 */
L8c61adb8: sa_motion_publish_stop(ctx,0x8c61adb8u); fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c61adba; /* fmov.s @r0,fr14 */
L8c61adba: sa_motion_publish_stop(ctx,0x8c61adbau); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61adbc; /* frchg */
L8c61adbc: sa_motion_publish_stop(ctx,0x8c61adbcu); r[2]=rd(m,bus,&io_status,0x8c61ae38u,4);if(io_status)return io_status;goto L8c61adbe; /* mov.l 0x8c61ae38,r2 */
L8c61adbe: sa_motion_publish_stop(ctx,0x8c61adbeu); r[3]=rd(m,bus,&io_status,0x8c61ae34u,4);if(io_status)return io_status;goto L8c61adc0; /* mov.l 0x8c61ae34,r3 */
L8c61adc0: sa_motion_publish_stop(ctx,0x8c61adc2u); target=r[2];*pr=0x8c61adc4u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61adc4; /* jsr @r2 */
L8c61adc4: sa_motion_publish_stop(ctx,0x8c61adc4u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61adc6; /* frchg */
L8c61adc6: sa_motion_publish_stop(ctx,0x8c61adc6u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adc8; /* fmov.s @r15+,fr15 */
L8c61adc8: sa_motion_publish_stop(ctx,0x8c61adc8u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adca; /* fmov.s @r15+,fr14 */
L8c61adca: sa_motion_publish_stop(ctx,0x8c61adcau); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adcc; /* fmov.s @r15+,fr13 */
L8c61adcc: sa_motion_publish_stop(ctx,0x8c61adccu); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adce; /* fmov.s @r15+,fr12 */
L8c61adce: sa_motion_publish_stop(ctx,0x8c61adceu); fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61add0; /* fmov.s @r15+,fr11 */
L8c61add0: sa_motion_publish_stop(ctx,0x8c61add0u); fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61add2; /* fmov.s @r15+,fr10 */
L8c61add2: sa_motion_publish_stop(ctx,0x8c61add2u); fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61add4; /* fmov.s @r15+,fr9 */
L8c61add4: sa_motion_publish_stop(ctx,0x8c61add4u); fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61add6; /* fmov.s @r15+,fr8 */
L8c61add6: sa_motion_publish_stop(ctx,0x8c61add6u); fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61add8; /* fmov.s @r15+,fr7 */
L8c61add8: sa_motion_publish_stop(ctx,0x8c61add8u); fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adda; /* fmov.s @r15+,fr6 */
L8c61adda: sa_motion_publish_stop(ctx,0x8c61addau); fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61addc; /* fmov.s @r15+,fr5 */
L8c61addc: sa_motion_publish_stop(ctx,0x8c61addcu); fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adde; /* fmov.s @r15+,fr4 */
L8c61adde: sa_motion_publish_stop(ctx,0x8c61addeu); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ade0; /* fmov.s @r15+,fr3 */
L8c61ade0: sa_motion_publish_stop(ctx,0x8c61ade0u); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ade2; /* fmov.s @r15+,fr2 */
L8c61ade2: sa_motion_publish_stop(ctx,0x8c61ade2u); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ade4; /* fmov.s @r15+,fr1 */
L8c61ade4: sa_motion_publish_stop(ctx,0x8c61ade4u); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61ade6; /* fmov.s @r15+,fr0 */
L8c61ade6: sa_motion_publish_stop(ctx,0x8c61ade6u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61ade8; /* frchg */
L8c61ade8: sa_motion_publish_stop(ctx,0x8c61ade8u); r[15]+=8u;goto L8c61adea; /* add #0x8,r15 */
L8c61adea: sa_motion_publish_stop(ctx,0x8c61adeau); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61adec; /* lds.l @r15+,PR */
L8c61adec: sa_motion_publish_stop(ctx,0x8c61adecu); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61adee; /* fmov.s @r15+,fr15 */
L8c61adee: sa_motion_publish_stop(ctx,0x8c61adeeu); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61adf0; /* mov.l @r15+,r9 */
L8c61adf0: sa_motion_publish_stop(ctx,0x8c61adf0u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61adf2; /* mov.l @r15+,r10 */
L8c61adf2: sa_motion_publish_stop(ctx,0x8c61adf2u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61adf4; /* mov.l @r15+,r11 */
L8c61adf4: sa_motion_publish_stop(ctx,0x8c61adf4u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61adf6; /* mov.l @r15+,r12 */
L8c61adf6: sa_motion_publish_stop(ctx,0x8c61adf6u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61adf8; /* mov.l @r15+,r13 */
L8c61adf8: sa_motion_publish_stop(ctx,0x8c61adfau); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
