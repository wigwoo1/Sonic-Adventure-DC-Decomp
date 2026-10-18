#include "sa_draw_motion_stop.h"
/* Environment alternate strip emitters with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_environment_alternate_strip_8c613be2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c613be2;
L8c613be2: sa_motion_publish_stop(ctx,0x8c613be2u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c613be4; /* mov.l r14,@-r15 */
L8c613be4: sa_motion_publish_stop(ctx,0x8c613be4u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c613be6; /* mov.l r13,@-r15 */
L8c613be6: sa_motion_publish_stop(ctx,0x8c613be6u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c613be8; /* mov.l r12,@-r15 */
L8c613be8: sa_motion_publish_stop(ctx,0x8c613be8u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c613bea; /* mov.l r11,@-r15 */
L8c613bea: sa_motion_publish_stop(ctx,0x8c613beau); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c613bec; /* mov.l r10,@-r15 */
L8c613bec: sa_motion_publish_stop(ctx,0x8c613becu); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c613bee; /* mov.l r9,@-r15 */
L8c613bee: sa_motion_publish_stop(ctx,0x8c613beeu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c613bf0; /* fmov.s fr15,@-r15 */
L8c613bf0: sa_motion_publish_stop(ctx,0x8c613bf0u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c613bf2; /* fmov.s fr14,@-r15 */
L8c613bf2: sa_motion_publish_stop(ctx,0x8c613bf2u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c613bf4; /* fmov.s fr13,@-r15 */
L8c613bf4: sa_motion_publish_stop(ctx,0x8c613bf4u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c613bf6; /* sts.l PR,@-r15 */
L8c613bf6: sa_motion_publish_stop(ctx,0x8c613bf6u); r[15]+=4294967280u;goto L8c613bf8; /* add #-0x10,r15 */
L8c613bf8: sa_motion_publish_stop(ctx,0x8c613bf8u); r[3]=rd(m,bus,&io_status,0x8c613c2cu,4);if(io_status)return io_status;goto L8c613bfa; /* mov.l 0x8c613c2c,r3 */
L8c613bfa: sa_motion_publish_stop(ctx,0x8c613bfau); r[9]=rd(m,bus,&io_status,0x8c613c28u,4);if(io_status)return io_status;goto L8c613bfc; /* mov.l 0x8c613c28,r9 */
L8c613bfc: sa_motion_publish_stop(ctx,0x8c613bfeu); target=r[3];*pr=0x8c613c00u;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c613c00; /* jsr @r3 */
L8c613c00: sa_motion_publish_stop(ctx,0x8c613c00u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613c02; /* mov.l @(0x4,r15),r3 */
L8c613c02: sa_motion_publish_stop(ctx,0x8c613c02u); r[2]=0u;goto L8c613c04; /* mov #0x0,r2 */
L8c613c04: sa_motion_publish_stop(ctx,0x8c613c04u); r[14]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613c06; /* mov.l @(0x4,r15),r14 */
L8c613c06: sa_motion_publish_stop(ctx,0x8c613c06u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c613c08; /* mov.w @(0x2,r3),r0 */
L8c613c08: sa_motion_publish_stop(ctx,0x8c613c08u); r[3]=rd(m,bus,&io_status,0x8c613c30u,4);if(io_status)return io_status;goto L8c613c0a; /* mov.l 0x8c613c30,r3 */
L8c613c0a: sa_motion_publish_stop(ctx,0x8c613c0au); r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c613c0c; /* mov.l @(0x4,r14),r14 */
L8c613c0c: sa_motion_publish_stop(ctx,0x8c613c0cu); r[0]=r[0]&65535u;goto L8c613c0e; /* extu.w r0,r0 */
L8c613c0e: sa_motion_publish_stop(ctx,0x8c613c0eu); wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c613c10; /* mov.l r0,@(0xc,r15) */
L8c613c10: sa_motion_publish_stop(ctx,0x8c613c10u); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c613c12; /* mov.l @r3,r13 */
L8c613c12: sa_motion_publish_stop(ctx,0x8c613c12u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613c14; /* mov.l r2,@(0x8,r15) */
L8c613c14: sa_motion_publish_stop(ctx,0x8c613c14u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c613c16; /* mov.l r2,@(0x4,r15) */
L8c613c16: sa_motion_publish_stop(ctx,0x8c613c18u); goto L8c613c34; /* bra 0x8c613c34 */
L8c613c34: sa_motion_publish_stop(ctx,0x8c613c34u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613c36; /* mov.w @r14+,r3 */
L8c613c36: sa_motion_publish_stop(ctx,0x8c613c36u); r[4]=rd(m,bus,&io_status,0x8c613d08u,4);if(io_status)return io_status;goto L8c613c38; /* mov.l 0x8c613d08,r4 */
L8c613c38: sa_motion_publish_stop(ctx,0x8c613c38u); r[3]=r[3]&65535u;goto L8c613c3a; /* extu.w r3,r3 */
L8c613c3a: sa_motion_publish_stop(ctx,0x8c613c3au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c613c3c; /* fldi1 fr4 */
L8c613c3c: sa_motion_publish_stop(ctx,0x8c613c3cu); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c613c3e; /* tst r4,r3 */
L8c613c3e: sa_motion_publish_stop(ctx,0x8c613c3eu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c613c40; /* mov.l r3,@r15 */
L8c613c40: sa_motion_publish_stop(ctx,0x8c613c42u); take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c613c52;goto L8c613c44; /* bt/s 0x8c613c52 */
L8c613c44: sa_motion_publish_stop(ctx,0x8c613c44u); r[4]=sx(m,bus,&io_status,0x8c613d04u,2);if(io_status)return io_status;goto L8c613c46; /* mov.w 0x8c613d04,r4 */
L8c613c46: sa_motion_publish_stop(ctx,0x8c613c46u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613c48; /* mov.l @r15,r3 */
L8c613c48: sa_motion_publish_stop(ctx,0x8c613c48u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c613c4a; /* fldi1 fr4 */
L8c613c4a: sa_motion_publish_stop(ctx,0x8c613c4au); r[3]&=r[4];goto L8c613c4c; /* and r4,r3 */
L8c613c4c: sa_motion_publish_stop(ctx,0x8c613c4cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c613c4e; /* fneg fr4 */
L8c613c4e: sa_motion_publish_stop(ctx,0x8c613c4eu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c613c50; /* mov.l r3,@r15 */
L8c613c50: sa_motion_publish_stop(ctx,0x8c613c50u); fcopy(f,15u,4u);goto L8c613c52; /* fmov fr4,fr15 */
L8c613c52: sa_motion_publish_stop(ctx,0x8c613c52u); r[0]=16u;goto L8c613c54; /* mov #0x10,r0 */
L8c613c54: sa_motion_publish_stop(ctx,0x8c613c54u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613c56; /* mov.l @r15,r3 */
L8c613c56: sa_motion_publish_stop(ctx,0x8c613c56u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613c58; /* mov.l @(0x4,r15),r2 */
L8c613c58: sa_motion_publish_stop(ctx,0x8c613c58u); r[10]=r[8];goto L8c613c5a; /* mov r8,r10 */
L8c613c5a: sa_motion_publish_stop(ctx,0x8c613c5au); r[2]+=r[3];goto L8c613c5c; /* add r3,r2 */
L8c613c5c: sa_motion_publish_stop(ctx,0x8c613c5cu); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c613c5e; /* mov.l r2,@(0x4,r15) */
L8c613c5e: sa_motion_publish_stop(ctx,0x8c613c5eu); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613c60; /* mov.l @r15,r1 */
L8c613c60: sa_motion_publish_stop(ctx,0x8c613c60u); r[1]+=4294967294u;goto L8c613c62; /* add #-0x2,r1 */
L8c613c62: sa_motion_publish_stop(ctx,0x8c613c62u); wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c613c64; /* mov.l r1,@r15 */
L8c613c64: sa_motion_publish_stop(ctx,0x8c613c64u); r[4]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613c66; /* mov.w @r14+,r4 */
L8c613c66: sa_motion_publish_stop(ctx,0x8c613c66u); r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613c68; /* mov.w @r14+,r11 */
L8c613c68: sa_motion_publish_stop(ctx,0x8c613c68u); r[4]=r[4]&65535u;goto L8c613c6a; /* extu.w r4,r4 */
L8c613c6a: sa_motion_publish_stop(ctx,0x8c613c6au); r[4]<<=2;goto L8c613c6c; /* shll2 r4 */
L8c613c6c: sa_motion_publish_stop(ctx,0x8c613c6cu); r[11]=r[11]&65535u;goto L8c613c6e; /* extu.w r11,r11 */
L8c613c6e: sa_motion_publish_stop(ctx,0x8c613c6eu); r[11]<<=2;goto L8c613c70; /* shll2 r11 */
L8c613c70: sa_motion_publish_stop(ctx,0x8c613c70u); r[4]<<=2;goto L8c613c72; /* shll2 r4 */
L8c613c72: sa_motion_publish_stop(ctx,0x8c613c72u); r[11]<<=2;goto L8c613c74; /* shll2 r11 */
L8c613c74: sa_motion_publish_stop(ctx,0x8c613c74u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c613c76; /* shll r4 */
L8c613c76: sa_motion_publish_stop(ctx,0x8c613c76u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c613c78; /* shll r11 */
L8c613c78: sa_motion_publish_stop(ctx,0x8c613c78u); r[11]+=r[13];goto L8c613c7a; /* add r13,r11 */
L8c613c7a: sa_motion_publish_stop(ctx,0x8c613c7au); r[4]+=r[13];goto L8c613c7c; /* add r13,r4 */
L8c613c7c: sa_motion_publish_stop(ctx,0x8c613c7cu); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c613c7e; /* fmov.s @(r0,r11),fr14 */
L8c613c7e: sa_motion_publish_stop(ctx,0x8c613c7eu); fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c613c80; /* fmov.s @(r0,r4),fr4 */
L8c613c80: sa_motion_publish_stop(ctx,0x8c613c80u); r[0]=20u;goto L8c613c82; /* mov #0x14,r0 */
L8c613c82: sa_motion_publish_stop(ctx,0x8c613c82u); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c613c84; /* fmov.s @(r0,r11),fr13 */
L8c613c84: sa_motion_publish_stop(ctx,0x8c613c84u); fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c613c86; /* fmov.s @(r0,r4),fr5 */
L8c613c86: sa_motion_publish_stop(ctx,0x8c613c86u); r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613c88; /* mov.w @r14+,r12 */
L8c613c88: sa_motion_publish_stop(ctx,0x8c613c88u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c613c8a; /* mov.w @r14,r3 */
L8c613c8a: sa_motion_publish_stop(ctx,0x8c613c8au); r[12]=r[12]&65535u;goto L8c613c8c; /* extu.w r12,r12 */
L8c613c8c: sa_motion_publish_stop(ctx,0x8c613c8cu); r[12]<<=2;goto L8c613c8e; /* shll2 r12 */
L8c613c8e: sa_motion_publish_stop(ctx,0x8c613c8eu); r[3]=r[3]&65535u;goto L8c613c90; /* extu.w r3,r3 */
L8c613c90: sa_motion_publish_stop(ctx,0x8c613c90u); r[3]<<=2;goto L8c613c92; /* shll2 r3 */
L8c613c92: sa_motion_publish_stop(ctx,0x8c613c92u); r[12]<<=2;goto L8c613c94; /* shll2 r12 */
L8c613c94: sa_motion_publish_stop(ctx,0x8c613c94u); r[3]<<=2;goto L8c613c96; /* shll2 r3 */
L8c613c96: sa_motion_publish_stop(ctx,0x8c613c96u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c613c98; /* shll r12 */
L8c613c98: sa_motion_publish_stop(ctx,0x8c613c98u); *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c613c9a; /* shll r3 */
L8c613c9a: sa_motion_publish_stop(ctx,0x8c613c9au); r[3]+=r[13];goto L8c613c9c; /* add r13,r3 */
L8c613c9c: sa_motion_publish_stop(ctx,0x8c613c9cu); r[12]+=r[13];goto L8c613c9e; /* add r13,r12 */
L8c613c9e: sa_motion_publish_stop(ctx,0x8c613c9eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c613ca0; /* pref @r3 */
L8c613ca0: sa_motion_publish_stop(ctx,0x8c613ca0u); r[0]=20u;goto L8c613ca2; /* mov #0x14,r0 */
L8c613ca2: sa_motion_publish_stop(ctx,0x8c613ca2u); fcopy(f,3u,14u);goto L8c613ca4; /* fmov fr14,fr3 */
L8c613ca4: sa_motion_publish_stop(ctx,0x8c613ca4u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c613ca6; /* fmov.s @(r0,r12),fr2 */
L8c613ca6: sa_motion_publish_stop(ctx,0x8c613ca6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613ca8; /* fsub fr4,fr3 */
L8c613ca8: sa_motion_publish_stop(ctx,0x8c613ca8u); r[0]=16u;goto L8c613caa; /* mov #0x10,r0 */
L8c613caa: sa_motion_publish_stop(ctx,0x8c613caau); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c613cac; /* cmp/hs r8,r10 */
L8c613cac: sa_motion_publish_stop(ctx,0x8c613cacu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613cae; /* fsub fr13,fr2 */
L8c613cae: sa_motion_publish_stop(ctx,0x8c613caeu); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c613cb0; /* fmov.s @(r0,r12),fr1 */
L8c613cb0: sa_motion_publish_stop(ctx,0x8c613cb0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c613cb2; /* fsub fr14,fr1 */
L8c613cb2: sa_motion_publish_stop(ctx,0x8c613cb2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c613cb4; /* fmul fr2,fr3 */
L8c613cb4: sa_motion_publish_stop(ctx,0x8c613cb4u); fcopy(f,2u,13u);goto L8c613cb6; /* fmov fr13,fr2 */
L8c613cb6: sa_motion_publish_stop(ctx,0x8c613cb6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613cb8; /* fsub fr5,fr2 */
L8c613cb8: sa_motion_publish_stop(ctx,0x8c613cb8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613cba; /* fmul fr1,fr2 */
L8c613cba: sa_motion_publish_stop(ctx,0x8c613cbau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613cbc; /* fsub fr2,fr3 */
L8c613cbc: sa_motion_publish_stop(ctx,0x8c613cbcu); fcopy(f,4u,3u);goto L8c613cbe; /* fmov fr3,fr4 */
L8c613cbe: sa_motion_publish_stop(ctx,0x8c613cc0u); take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c613d0c;goto L8c613cc2; /* bt/s 0x8c613d0c */
L8c613cc2: sa_motion_publish_stop(ctx,0x8c613cc2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613cc4; /* fldi0 fr3 */
L8c613cc4: sa_motion_publish_stop(ctx,0x8c613cc4u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c613cc6; /* fcmp/gt fr4,fr3 */
L8c613cc6: sa_motion_publish_stop(ctx,0x8c613cc6u); if((*sr&1u))goto L8c613d3a;goto L8c613cc8; /* bt 0x8c613d3a */
L8c613cc8: sa_motion_publish_stop(ctx,0x8c613cc8u); r[4]=r[11];goto L8c613cca; /* mov r11,r4 */
L8c613cca: sa_motion_publish_stop(ctx,0x8c613ccau); r[5]=r[9];goto L8c613ccc; /* mov r9,r5 */
L8c613ccc: sa_motion_publish_stop(ctx,0x8c613cccu); r[8]+=16u;goto L8c613cce; /* add #0x10,r8 */
L8c613cce: sa_motion_publish_stop(ctx,0x8c613cceu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613cd0; /* fschg */
L8c613cd0: sa_motion_publish_stop(ctx,0x8c613cd0u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613cd2; /* fmov.s @r4+,fr0 */
L8c613cd2: sa_motion_publish_stop(ctx,0x8c613cd2u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613cd4; /* fmov.s @r4+,fr2 */
L8c613cd4: sa_motion_publish_stop(ctx,0x8c613cd4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c613cd6; /* fldi1 fr3 */
L8c613cd6: sa_motion_publish_stop(ctx,0x8c613cd6u); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c613cd8; /* ftrv xmtrx,fv0 */
L8c613cd8: sa_motion_publish_stop(ctx,0x8c613cd8u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613cda; /* fmov.s @r4+,fr4 */
L8c613cda: sa_motion_publish_stop(ctx,0x8c613cdau); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613cdc; /* fmov.s @r4,fr6 */
L8c613cdc: sa_motion_publish_stop(ctx,0x8c613cdcu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613cde; /* fschg */
L8c613cde: sa_motion_publish_stop(ctx,0x8c613cdeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613ce0; /* fmov.s fr6,@-r8 */
L8c613ce0: sa_motion_publish_stop(ctx,0x8c613ce0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613ce2; /* fmov.s fr5,@-r8 */
L8c613ce2: sa_motion_publish_stop(ctx,0x8c613ce2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613ce4; /* fmov.s fr4,@-r8 */
L8c613ce4: sa_motion_publish_stop(ctx,0x8c613ce4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613ce6; /* mov.l r5,@-r8 */
L8c613ce6: sa_motion_publish_stop(ctx,0x8c613ce6u); r[8]+=28u;goto L8c613ce8; /* add #0x1c,r8 */
L8c613ce8: sa_motion_publish_stop(ctx,0x8c613ce8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613cea; /* fmov.s fr2,@-r8 */
L8c613cea: sa_motion_publish_stop(ctx,0x8c613ceau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613cec; /* fmov.s fr1,@-r8 */
L8c613cec: sa_motion_publish_stop(ctx,0x8c613cecu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c613cee; /* fmov.s fr0,@-r8 */
L8c613cee: sa_motion_publish_stop(ctx,0x8c613ceeu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613cf0; /* pref @r8 */
L8c613cf0: sa_motion_publish_stop(ctx,0x8c613cf0u); r[8]+=16u;goto L8c613cf2; /* add #0x10,r8 */
L8c613cf2: sa_motion_publish_stop(ctx,0x8c613cf2u); r[3]=r[8];goto L8c613cf4; /* mov r8,r3 */
L8c613cf4: sa_motion_publish_stop(ctx,0x8c613cf4u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613cf6; /* mov.l @(0x8,r15),r2 */
L8c613cf6: sa_motion_publish_stop(ctx,0x8c613cf6u); r[3]-=r[10];goto L8c613cf8; /* sub r10,r3 */
L8c613cf8: sa_motion_publish_stop(ctx,0x8c613cf8u); r[3]+=32u;goto L8c613cfa; /* add #0x20,r3 */
L8c613cfa: sa_motion_publish_stop(ctx,0x8c613cfau); r[2]+=r[3];goto L8c613cfc; /* add r3,r2 */
L8c613cfc: sa_motion_publish_stop(ctx,0x8c613cfcu); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613cfe; /* mov.l r2,@(0x8,r15) */
L8c613cfe: sa_motion_publish_stop(ctx,0x8c613d00u); r[10]=r[8];goto L8c613d64; /* bra 0x8c613d64 */
L8c613d0c: sa_motion_publish_stop(ctx,0x8c613d0cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613d0e; /* fldi0 fr3 */
L8c613d0e: sa_motion_publish_stop(ctx,0x8c613d0eu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c613d10; /* fcmp/gt fr4,fr3 */
L8c613d10: sa_motion_publish_stop(ctx,0x8c613d10u); if(!(*sr&1u))goto L8c613d64;goto L8c613d12; /* bf 0x8c613d64 */
L8c613d12: sa_motion_publish_stop(ctx,0x8c613d12u); r[5]=rd(m,bus,&io_status,0x8c613e34u,4);if(io_status)return io_status;goto L8c613d14; /* mov.l 0x8c613e34,r5 */
L8c613d14: sa_motion_publish_stop(ctx,0x8c613d14u); r[8]+=16u;goto L8c613d16; /* add #0x10,r8 */
L8c613d16: sa_motion_publish_stop(ctx,0x8c613d16u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613d18; /* fschg */
L8c613d18: sa_motion_publish_stop(ctx,0x8c613d18u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d1a; /* fmov.s @r4+,fr0 */
L8c613d1a: sa_motion_publish_stop(ctx,0x8c613d1au); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d1c; /* fmov.s @r4+,fr2 */
L8c613d1c: sa_motion_publish_stop(ctx,0x8c613d1cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c613d1e; /* fldi1 fr3 */
L8c613d1e: sa_motion_publish_stop(ctx,0x8c613d1eu); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c613d20; /* ftrv xmtrx,fv0 */
L8c613d20: sa_motion_publish_stop(ctx,0x8c613d20u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d22; /* fmov.s @r4+,fr4 */
L8c613d22: sa_motion_publish_stop(ctx,0x8c613d22u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613d24; /* fmov.s @r4,fr6 */
L8c613d24: sa_motion_publish_stop(ctx,0x8c613d24u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613d26; /* fschg */
L8c613d26: sa_motion_publish_stop(ctx,0x8c613d26u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613d28; /* fmov.s fr6,@-r8 */
L8c613d28: sa_motion_publish_stop(ctx,0x8c613d28u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613d2a; /* fmov.s fr5,@-r8 */
L8c613d2a: sa_motion_publish_stop(ctx,0x8c613d2au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613d2c; /* fmov.s fr4,@-r8 */
L8c613d2c: sa_motion_publish_stop(ctx,0x8c613d2cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613d2e; /* mov.l r5,@-r8 */
L8c613d2e: sa_motion_publish_stop(ctx,0x8c613d2eu); r[8]+=28u;goto L8c613d30; /* add #0x1c,r8 */
L8c613d30: sa_motion_publish_stop(ctx,0x8c613d30u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613d32; /* fmov.s fr2,@-r8 */
L8c613d32: sa_motion_publish_stop(ctx,0x8c613d32u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613d34; /* fmov.s fr1,@-r8 */
L8c613d34: sa_motion_publish_stop(ctx,0x8c613d34u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c613d36; /* fmov.s fr0,@-r8 */
L8c613d36: sa_motion_publish_stop(ctx,0x8c613d36u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613d38; /* pref @r8 */
L8c613d38: sa_motion_publish_stop(ctx,0x8c613d38u); r[8]+=16u;goto L8c613d3a; /* add #0x10,r8 */
L8c613d3a: sa_motion_publish_stop(ctx,0x8c613d3au); r[5]=rd(m,bus,&io_status,0x8c613e34u,4);if(io_status)return io_status;goto L8c613d3c; /* mov.l 0x8c613e34,r5 */
L8c613d3c: sa_motion_publish_stop(ctx,0x8c613d3cu); r[4]=r[11];goto L8c613d3e; /* mov r11,r4 */
L8c613d3e: sa_motion_publish_stop(ctx,0x8c613d3eu); r[8]+=16u;goto L8c613d40; /* add #0x10,r8 */
L8c613d40: sa_motion_publish_stop(ctx,0x8c613d40u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613d42; /* fschg */
L8c613d42: sa_motion_publish_stop(ctx,0x8c613d42u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d44; /* fmov.s @r4+,fr0 */
L8c613d44: sa_motion_publish_stop(ctx,0x8c613d44u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d46; /* fmov.s @r4+,fr2 */
L8c613d46: sa_motion_publish_stop(ctx,0x8c613d46u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c613d48; /* fldi1 fr3 */
L8c613d48: sa_motion_publish_stop(ctx,0x8c613d48u); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c613d4a; /* ftrv xmtrx,fv0 */
L8c613d4a: sa_motion_publish_stop(ctx,0x8c613d4au); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d4c; /* fmov.s @r4+,fr4 */
L8c613d4c: sa_motion_publish_stop(ctx,0x8c613d4cu); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613d4e; /* fmov.s @r4,fr6 */
L8c613d4e: sa_motion_publish_stop(ctx,0x8c613d4eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613d50; /* fschg */
L8c613d50: sa_motion_publish_stop(ctx,0x8c613d50u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613d52; /* fmov.s fr6,@-r8 */
L8c613d52: sa_motion_publish_stop(ctx,0x8c613d52u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613d54; /* fmov.s fr5,@-r8 */
L8c613d54: sa_motion_publish_stop(ctx,0x8c613d54u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613d56; /* fmov.s fr4,@-r8 */
L8c613d56: sa_motion_publish_stop(ctx,0x8c613d56u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613d58; /* mov.l r5,@-r8 */
L8c613d58: sa_motion_publish_stop(ctx,0x8c613d58u); r[8]+=28u;goto L8c613d5a; /* add #0x1c,r8 */
L8c613d5a: sa_motion_publish_stop(ctx,0x8c613d5au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613d5c; /* fmov.s fr2,@-r8 */
L8c613d5c: sa_motion_publish_stop(ctx,0x8c613d5cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613d5e; /* fmov.s fr1,@-r8 */
L8c613d5e: sa_motion_publish_stop(ctx,0x8c613d5eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c613d60; /* fmov.s fr0,@-r8 */
L8c613d60: sa_motion_publish_stop(ctx,0x8c613d60u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613d62; /* pref @r8 */
L8c613d62: sa_motion_publish_stop(ctx,0x8c613d62u); r[8]+=16u;goto L8c613d64; /* add #0x10,r8 */
L8c613d64: sa_motion_publish_stop(ctx,0x8c613d64u); fcopy(f,4u,14u);goto L8c613d66; /* fmov fr14,fr4 */
L8c613d66: sa_motion_publish_stop(ctx,0x8c613d66u); r[0]=16u;goto L8c613d68; /* mov #0x10,r0 */
L8c613d68: sa_motion_publish_stop(ctx,0x8c613d68u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613d6a; /* mov.l @r15,r3 */
L8c613d6a: sa_motion_publish_stop(ctx,0x8c613d6au); r[4]=r[11];goto L8c613d6c; /* mov r11,r4 */
L8c613d6c: sa_motion_publish_stop(ctx,0x8c613d6cu); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c613d6e; /* fmov.s @(r0,r12),fr14 */
L8c613d6e: sa_motion_publish_stop(ctx,0x8c613d6eu); r[0]=20u;goto L8c613d70; /* mov #0x14,r0 */
L8c613d70: sa_motion_publish_stop(ctx,0x8c613d70u); fcopy(f,5u,13u);goto L8c613d72; /* fmov fr13,fr5 */
L8c613d72: sa_motion_publish_stop(ctx,0x8c613d72u); r[3]+=4294967295u;goto L8c613d74; /* add #-0x1,r3 */
L8c613d74: sa_motion_publish_stop(ctx,0x8c613d74u); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c613d76; /* fmov.s @(r0,r12),fr13 */
L8c613d76: sa_motion_publish_stop(ctx,0x8c613d76u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c613d78; /* tst r3,r3 */
L8c613d78: sa_motion_publish_stop(ctx,0x8c613d78u); r[11]=r[12];goto L8c613d7a; /* mov r12,r11 */
L8c613d7a: sa_motion_publish_stop(ctx,0x8c613d7au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c613d7c; /* fneg fr15 */
L8c613d7c: sa_motion_publish_stop(ctx,0x8c613d7eu); take=!(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c613c86;goto L8c613d80; /* bf/s 0x8c613c86 */
L8c613d80: sa_motion_publish_stop(ctx,0x8c613d80u); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c613d82; /* cmp/hs r8,r10 */
L8c613d82: sa_motion_publish_stop(ctx,0x8c613d82u); if((*sr&1u))goto L8c613dba;goto L8c613d84; /* bt 0x8c613dba */
L8c613d84: sa_motion_publish_stop(ctx,0x8c613d84u); r[4]=r[12];goto L8c613d86; /* mov r12,r4 */
L8c613d86: sa_motion_publish_stop(ctx,0x8c613d86u); r[5]=r[9];goto L8c613d88; /* mov r9,r5 */
L8c613d88: sa_motion_publish_stop(ctx,0x8c613d88u); r[8]+=16u;goto L8c613d8a; /* add #0x10,r8 */
L8c613d8a: sa_motion_publish_stop(ctx,0x8c613d8au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613d8c; /* fschg */
L8c613d8c: sa_motion_publish_stop(ctx,0x8c613d8cu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d8e; /* fmov.s @r4+,fr0 */
L8c613d8e: sa_motion_publish_stop(ctx,0x8c613d8eu); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d90; /* fmov.s @r4+,fr2 */
L8c613d90: sa_motion_publish_stop(ctx,0x8c613d90u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c613d92; /* fldi1 fr3 */
L8c613d92: sa_motion_publish_stop(ctx,0x8c613d92u); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c613d94; /* ftrv xmtrx,fv0 */
L8c613d94: sa_motion_publish_stop(ctx,0x8c613d94u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d96; /* fmov.s @r4+,fr4 */
L8c613d96: sa_motion_publish_stop(ctx,0x8c613d96u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613d98; /* fmov.s @r4,fr6 */
L8c613d98: sa_motion_publish_stop(ctx,0x8c613d98u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613d9a; /* fschg */
L8c613d9a: sa_motion_publish_stop(ctx,0x8c613d9au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613d9c; /* fmov.s fr6,@-r8 */
L8c613d9c: sa_motion_publish_stop(ctx,0x8c613d9cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613d9e; /* fmov.s fr5,@-r8 */
L8c613d9e: sa_motion_publish_stop(ctx,0x8c613d9eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613da0; /* fmov.s fr4,@-r8 */
L8c613da0: sa_motion_publish_stop(ctx,0x8c613da0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613da2; /* mov.l r5,@-r8 */
L8c613da2: sa_motion_publish_stop(ctx,0x8c613da2u); r[8]+=28u;goto L8c613da4; /* add #0x1c,r8 */
L8c613da4: sa_motion_publish_stop(ctx,0x8c613da4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613da6; /* fmov.s fr2,@-r8 */
L8c613da6: sa_motion_publish_stop(ctx,0x8c613da6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613da8; /* fmov.s fr1,@-r8 */
L8c613da8: sa_motion_publish_stop(ctx,0x8c613da8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c613daa; /* fmov.s fr0,@-r8 */
L8c613daa: sa_motion_publish_stop(ctx,0x8c613daau); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613dac; /* pref @r8 */
L8c613dac: sa_motion_publish_stop(ctx,0x8c613dacu); r[8]+=16u;goto L8c613dae; /* add #0x10,r8 */
L8c613dae: sa_motion_publish_stop(ctx,0x8c613daeu); r[3]=r[8];goto L8c613db0; /* mov r8,r3 */
L8c613db0: sa_motion_publish_stop(ctx,0x8c613db0u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613db2; /* mov.l @(0x8,r15),r2 */
L8c613db2: sa_motion_publish_stop(ctx,0x8c613db2u); r[3]-=r[10];goto L8c613db4; /* sub r10,r3 */
L8c613db4: sa_motion_publish_stop(ctx,0x8c613db4u); r[3]+=32u;goto L8c613db6; /* add #0x20,r3 */
L8c613db6: sa_motion_publish_stop(ctx,0x8c613db6u); r[2]+=r[3];goto L8c613db8; /* add r3,r2 */
L8c613db8: sa_motion_publish_stop(ctx,0x8c613db8u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613dba; /* mov.l r2,@(0x8,r15) */
L8c613dba: sa_motion_publish_stop(ctx,0x8c613dbau); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c613dbc; /* mov.l @(0xc,r15),r3 */
L8c613dbc: sa_motion_publish_stop(ctx,0x8c613dbcu); r[3]+=4294967295u;goto L8c613dbe; /* add #-0x1,r3 */
L8c613dbe: sa_motion_publish_stop(ctx,0x8c613dbeu); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c613dc0; /* tst r3,r3 */
L8c613dc0: sa_motion_publish_stop(ctx,0x8c613dc2u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c613dc8;goto L8c613dc4; /* bt/s 0x8c613dc8 */
L8c613dc4: sa_motion_publish_stop(ctx,0x8c613dc6u); goto L8c613c34; /* bra 0x8c613c34 */
L8c613dc8: sa_motion_publish_stop(ctx,0x8c613dc8u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613dca; /* mov.l @(0x8,r15),r3 */
L8c613dca: sa_motion_publish_stop(ctx,0x8c613dcau); r[4]=rd(m,bus,&io_status,0x8c613e38u,4);if(io_status)return io_status;goto L8c613dcc; /* mov.l 0x8c613e38,r4 */
L8c613dcc: sa_motion_publish_stop(ctx,0x8c613dccu); r[3]>>=2;goto L8c613dce; /* shlr2 r3 */
L8c613dce: sa_motion_publish_stop(ctx,0x8c613dceu); r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c613dd0; /* mov.l @r4,r1 */
L8c613dd0: sa_motion_publish_stop(ctx,0x8c613dd0u); r[3]>>=2;goto L8c613dd2; /* shlr2 r3 */
L8c613dd2: sa_motion_publish_stop(ctx,0x8c613dd2u); *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c613dd4; /* shlr r3 */
L8c613dd4: sa_motion_publish_stop(ctx,0x8c613dd4u); r[1]+=r[3];goto L8c613dd6; /* add r3,r1 */
L8c613dd6: sa_motion_publish_stop(ctx,0x8c613dd6u); wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c613dd8; /* mov.l r1,@r4 */
L8c613dd8: sa_motion_publish_stop(ctx,0x8c613dd8u); r[4]=rd(m,bus,&io_status,0x8c613e3cu,4);if(io_status)return io_status;goto L8c613dda; /* mov.l 0x8c613e3c,r4 */
L8c613dda: sa_motion_publish_stop(ctx,0x8c613ddau); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613ddc; /* mov.l @(0x4,r15),r3 */
L8c613ddc: sa_motion_publish_stop(ctx,0x8c613ddcu); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c613dde; /* mov.l @r4,r2 */
L8c613dde: sa_motion_publish_stop(ctx,0x8c613ddeu); r[2]+=r[3];goto L8c613de0; /* add r3,r2 */
L8c613de0: sa_motion_publish_stop(ctx,0x8c613de0u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c613de2; /* mov.l r2,@r4 */
L8c613de2: sa_motion_publish_stop(ctx,0x8c613de2u); r[15]+=16u;goto L8c613de4; /* add #0x10,r15 */
L8c613de4: sa_motion_publish_stop(ctx,0x8c613de4u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613de6; /* lds.l @r15+,PR */
L8c613de6: sa_motion_publish_stop(ctx,0x8c613de6u); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613de8; /* fmov.s @r15+,fr13 */
L8c613de8: sa_motion_publish_stop(ctx,0x8c613de8u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613dea; /* fmov.s @r15+,fr14 */
L8c613dea: sa_motion_publish_stop(ctx,0x8c613deau); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613dec; /* fmov.s @r15+,fr15 */
L8c613dec: sa_motion_publish_stop(ctx,0x8c613decu); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613dee; /* mov.l @r15+,r9 */
L8c613dee: sa_motion_publish_stop(ctx,0x8c613deeu); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613df0; /* mov.l @r15+,r10 */
L8c613df0: sa_motion_publish_stop(ctx,0x8c613df0u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613df2; /* mov.l @r15+,r11 */
L8c613df2: sa_motion_publish_stop(ctx,0x8c613df2u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613df4; /* mov.l @r15+,r12 */
L8c613df4: sa_motion_publish_stop(ctx,0x8c613df4u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613df6; /* mov.l @r15+,r13 */
L8c613df6: sa_motion_publish_stop(ctx,0x8c613df8u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_environment_alternate_strip_clipped_8c6153bc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6153bc;
L8c6153bc: sa_motion_publish_stop(ctx,0x8c6153bcu); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6153be; /* mov.l r14,@-r15 */
L8c6153be: sa_motion_publish_stop(ctx,0x8c6153beu); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6153c0; /* mov.l r13,@-r15 */
L8c6153c0: sa_motion_publish_stop(ctx,0x8c6153c0u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6153c2; /* mov.l r12,@-r15 */
L8c6153c2: sa_motion_publish_stop(ctx,0x8c6153c2u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6153c4; /* mov.l r11,@-r15 */
L8c6153c4: sa_motion_publish_stop(ctx,0x8c6153c4u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6153c6; /* mov.l r10,@-r15 */
L8c6153c6: sa_motion_publish_stop(ctx,0x8c6153c6u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6153c8; /* mov.l r9,@-r15 */
L8c6153c8: sa_motion_publish_stop(ctx,0x8c6153c8u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c6153ca; /* fmov.s fr15,@-r15 */
L8c6153ca: sa_motion_publish_stop(ctx,0x8c6153cau); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c6153cc; /* fmov.s fr14,@-r15 */
L8c6153cc: sa_motion_publish_stop(ctx,0x8c6153ccu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c6153ce; /* fmov.s fr13,@-r15 */
L8c6153ce: sa_motion_publish_stop(ctx,0x8c6153ceu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c6153d0; /* fmov.s fr12,@-r15 */
L8c6153d0: sa_motion_publish_stop(ctx,0x8c6153d0u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6153d2; /* sts.l PR,@-r15 */
L8c6153d2: sa_motion_publish_stop(ctx,0x8c6153d2u); r[15]+=4294967220u;goto L8c6153d4; /* add #-0x4c,r15 */
L8c6153d4: sa_motion_publish_stop(ctx,0x8c6153d4u); r[3]=rd(m,bus,&io_status,0x8c615410u,4);if(io_status)return io_status;goto L8c6153d6; /* mov.l 0x8c615410,r3 */
L8c6153d6: sa_motion_publish_stop(ctx,0x8c6153d6u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c6153d8; /* fldi1 fr12 */
L8c6153d8: sa_motion_publish_stop(ctx,0x8c6153dau); target=r[3];*pr=0x8c6153dcu;wr(m,bus,&io_status,r[15]+0xcu,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6153dc; /* jsr @r3 */
L8c6153dc: sa_motion_publish_stop(ctx,0x8c6153dcu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6153de; /* mov.l @(0xc,r15),r3 */
L8c6153de: sa_motion_publish_stop(ctx,0x8c6153deu); r[2]=0u;goto L8c6153e0; /* mov #0x0,r2 */
L8c6153e0: sa_motion_publish_stop(ctx,0x8c6153e0u); r[14]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6153e2; /* mov.l @(0xc,r15),r14 */
L8c6153e2: sa_motion_publish_stop(ctx,0x8c6153e2u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6153e4; /* mov.w @(0x2,r3),r0 */
L8c6153e4: sa_motion_publish_stop(ctx,0x8c6153e4u); r[3]=rd(m,bus,&io_status,0x8c615414u,4);if(io_status)return io_status;goto L8c6153e6; /* mov.l 0x8c615414,r3 */
L8c6153e6: sa_motion_publish_stop(ctx,0x8c6153e6u); r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c6153e8; /* mov.l @(0x4,r14),r14 */
L8c6153e8: sa_motion_publish_stop(ctx,0x8c6153e8u); r[0]=r[0]&65535u;goto L8c6153ea; /* extu.w r0,r0 */
L8c6153ea: sa_motion_publish_stop(ctx,0x8c6153eau); wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c6153ec; /* mov.l r0,@(0x18,r15) */
L8c6153ec: sa_motion_publish_stop(ctx,0x8c6153ecu); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6153ee; /* mov.l @r3,r13 */
L8c6153ee: sa_motion_publish_stop(ctx,0x8c6153eeu); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6153f0; /* mov.l r2,@(0xc,r15) */
L8c6153f0: sa_motion_publish_stop(ctx,0x8c6153f2u); goto L8c615418; /* bra 0x8c615418 */
L8c615418: sa_motion_publish_stop(ctx,0x8c615418u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c61541a; /* mov.w @r14+,r3 */
L8c61541a: sa_motion_publish_stop(ctx,0x8c61541au); r[4]=rd(m,bus,&io_status,0x8c615534u,4);if(io_status)return io_status;goto L8c61541c; /* mov.l 0x8c615534,r4 */
L8c61541c: sa_motion_publish_stop(ctx,0x8c61541cu); r[3]=r[3]&65535u;goto L8c61541e; /* extu.w r3,r3 */
L8c61541e: sa_motion_publish_stop(ctx,0x8c61541eu); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c615420; /* tst r4,r3 */
L8c615420: sa_motion_publish_stop(ctx,0x8c615420u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c615422; /* mov.l r3,@(0x8,r15) */
L8c615422: sa_motion_publish_stop(ctx,0x8c615424u); take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c615434;goto L8c615426; /* bt/s 0x8c615434 */
L8c615426: sa_motion_publish_stop(ctx,0x8c615426u); r[4]=sx(m,bus,&io_status,0x8c615530u,2);if(io_status)return io_status;goto L8c615428; /* mov.w 0x8c615530,r4 */
L8c615428: sa_motion_publish_stop(ctx,0x8c615428u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61542a; /* mov.l @(0x8,r15),r3 */
L8c61542a: sa_motion_publish_stop(ctx,0x8c61542au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c61542c; /* fldi1 fr4 */
L8c61542c: sa_motion_publish_stop(ctx,0x8c61542cu); r[3]&=r[4];goto L8c61542e; /* and r4,r3 */
L8c61542e: sa_motion_publish_stop(ctx,0x8c61542eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c615430; /* fneg fr4 */
L8c615430: sa_motion_publish_stop(ctx,0x8c615430u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c615432; /* mov.l r3,@(0x8,r15) */
L8c615432: sa_motion_publish_stop(ctx,0x8c615432u); fcopy(f,15u,4u);goto L8c615434; /* fmov fr4,fr15 */
L8c615434: sa_motion_publish_stop(ctx,0x8c615434u); r[0]=16u;goto L8c615436; /* mov #0x10,r0 */
L8c615436: sa_motion_publish_stop(ctx,0x8c615436u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615438; /* mov.l @(0x8,r15),r3 */
L8c615438: sa_motion_publish_stop(ctx,0x8c615438u); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61543a; /* mov.l @(0xc,r15),r2 */
L8c61543a: sa_motion_publish_stop(ctx,0x8c61543au); r[9]=r[8];goto L8c61543c; /* mov r8,r9 */
L8c61543c: sa_motion_publish_stop(ctx,0x8c61543cu); r[2]+=r[3];goto L8c61543e; /* add r3,r2 */
L8c61543e: sa_motion_publish_stop(ctx,0x8c61543eu); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c615440; /* mov.l r2,@(0xc,r15) */
L8c615440: sa_motion_publish_stop(ctx,0x8c615440u); r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615442; /* mov.l @(0x8,r15),r1 */
L8c615442: sa_motion_publish_stop(ctx,0x8c615442u); r[1]+=4294967294u;goto L8c615444; /* add #-0x2,r1 */
L8c615444: sa_motion_publish_stop(ctx,0x8c615444u); wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c615446; /* mov.l r1,@(0x8,r15) */
L8c615446: sa_motion_publish_stop(ctx,0x8c615446u); r[10]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c615448; /* mov.w @r14+,r10 */
L8c615448: sa_motion_publish_stop(ctx,0x8c615448u); r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c61544a; /* mov.w @r14+,r11 */
L8c61544a: sa_motion_publish_stop(ctx,0x8c61544au); r[10]=r[10]&65535u;goto L8c61544c; /* extu.w r10,r10 */
L8c61544c: sa_motion_publish_stop(ctx,0x8c61544cu); r[10]<<=2;goto L8c61544e; /* shll2 r10 */
L8c61544e: sa_motion_publish_stop(ctx,0x8c61544eu); r[11]=r[11]&65535u;goto L8c615450; /* extu.w r11,r11 */
L8c615450: sa_motion_publish_stop(ctx,0x8c615450u); r[11]<<=2;goto L8c615452; /* shll2 r11 */
L8c615452: sa_motion_publish_stop(ctx,0x8c615452u); r[10]<<=2;goto L8c615454; /* shll2 r10 */
L8c615454: sa_motion_publish_stop(ctx,0x8c615454u); r[11]<<=2;goto L8c615456; /* shll2 r11 */
L8c615456: sa_motion_publish_stop(ctx,0x8c615456u); *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c615458; /* shll r10 */
L8c615458: sa_motion_publish_stop(ctx,0x8c615458u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61545a; /* shll r11 */
L8c61545a: sa_motion_publish_stop(ctx,0x8c61545au); r[11]+=r[13];goto L8c61545c; /* add r13,r11 */
L8c61545c: sa_motion_publish_stop(ctx,0x8c61545cu); r[10]+=r[13];goto L8c61545e; /* add r13,r10 */
L8c61545e: sa_motion_publish_stop(ctx,0x8c61545eu); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c615460; /* fmov.s @(r0,r11),fr13 */
L8c615460: sa_motion_publish_stop(ctx,0x8c615460u); fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c615462; /* fmov.s @(r0,r10),fr6 */
L8c615462: sa_motion_publish_stop(ctx,0x8c615462u); r[0]=20u;goto L8c615464; /* mov #0x14,r0 */
L8c615464: sa_motion_publish_stop(ctx,0x8c615464u); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c615466; /* fmov.s @(r0,r11),fr14 */
L8c615466: sa_motion_publish_stop(ctx,0x8c615466u); fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c615468; /* fmov.s @(r0,r10),fr5 */
L8c615468: sa_motion_publish_stop(ctx,0x8c615468u); r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c61546a; /* mov.w @r14+,r12 */
L8c61546a: sa_motion_publish_stop(ctx,0x8c61546au); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c61546c; /* mov.w @r14,r3 */
L8c61546c: sa_motion_publish_stop(ctx,0x8c61546cu); r[12]=r[12]&65535u;goto L8c61546e; /* extu.w r12,r12 */
L8c61546e: sa_motion_publish_stop(ctx,0x8c61546eu); r[12]<<=2;goto L8c615470; /* shll2 r12 */
L8c615470: sa_motion_publish_stop(ctx,0x8c615470u); r[3]=r[3]&65535u;goto L8c615472; /* extu.w r3,r3 */
L8c615472: sa_motion_publish_stop(ctx,0x8c615472u); r[3]<<=2;goto L8c615474; /* shll2 r3 */
L8c615474: sa_motion_publish_stop(ctx,0x8c615474u); r[12]<<=2;goto L8c615476; /* shll2 r12 */
L8c615476: sa_motion_publish_stop(ctx,0x8c615476u); r[3]<<=2;goto L8c615478; /* shll2 r3 */
L8c615478: sa_motion_publish_stop(ctx,0x8c615478u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61547a; /* shll r12 */
L8c61547a: sa_motion_publish_stop(ctx,0x8c61547au); *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c61547c; /* shll r3 */
L8c61547c: sa_motion_publish_stop(ctx,0x8c61547cu); r[3]+=r[13];goto L8c61547e; /* add r13,r3 */
L8c61547e: sa_motion_publish_stop(ctx,0x8c61547eu); r[12]+=r[13];goto L8c615480; /* add r13,r12 */
L8c615480: sa_motion_publish_stop(ctx,0x8c615480u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c615482; /* pref @r3 */
L8c615482: sa_motion_publish_stop(ctx,0x8c615482u); r[2]=0u;goto L8c615484; /* mov #0x0,r2 */
L8c615484: sa_motion_publish_stop(ctx,0x8c615484u); wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c615486; /* mov.l r2,@(0x14,r15) */
L8c615486: sa_motion_publish_stop(ctx,0x8c615486u); r[4]=63u;goto L8c615488; /* mov #0x3f,r4 */
L8c615488: sa_motion_publish_stop(ctx,0x8c615488u); r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c61548a; /* mov.l @(0xc,r10),r2 */
L8c61548a: sa_motion_publish_stop(ctx,0x8c61548au); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61548c; /* mov.l @(0xc,r11),r3 */
L8c61548c: sa_motion_publish_stop(ctx,0x8c61548cu); r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61548e; /* mov.l @(0xc,r12),r1 */
L8c61548e: sa_motion_publish_stop(ctx,0x8c61548eu); r[2]&=r[3];goto L8c615490; /* and r3,r2 */
L8c615490: sa_motion_publish_stop(ctx,0x8c615490u); r[2]&=r[1];goto L8c615492; /* and r1,r2 */
L8c615492: sa_motion_publish_stop(ctx,0x8c615492u); r[4]&=r[2];goto L8c615494; /* and r2,r4 */
L8c615494: sa_motion_publish_stop(ctx,0x8c615494u); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c615496; /* tst r4,r4 */
L8c615496: sa_motion_publish_stop(ctx,0x8c615498u); take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c61549e;goto L8c61549a; /* bt/s 0x8c61549e */
L8c61549a: sa_motion_publish_stop(ctx,0x8c61549cu); goto L8c6155f8; /* bra 0x8c6155f8 */
L8c61549e: sa_motion_publish_stop(ctx,0x8c61549eu); r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c6154a0; /* mov.l @(0xc,r10),r4 */
L8c6154a0: sa_motion_publish_stop(ctx,0x8c6154a0u); r[5]=1u;goto L8c6154a2; /* mov #0x1,r5 */
L8c6154a2: sa_motion_publish_stop(ctx,0x8c6154a2u); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c6154a4; /* mov.l @(0xc,r11),r3 */
L8c6154a4: sa_motion_publish_stop(ctx,0x8c6154a4u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c6154a6; /* mov.l @(0xc,r12),r2 */
L8c6154a6: sa_motion_publish_stop(ctx,0x8c6154a6u); r[4]|=r[3];goto L8c6154a8; /* or r3,r4 */
L8c6154a8: sa_motion_publish_stop(ctx,0x8c6154a8u); r[4]|=r[2];goto L8c6154aa; /* or r2,r4 */
L8c6154aa: sa_motion_publish_stop(ctx,0x8c6154aau); r[4]&=r[5];goto L8c6154ac; /* and r5,r4 */
L8c6154ac: sa_motion_publish_stop(ctx,0x8c6154acu); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6154ae; /* tst r4,r4 */
L8c6154ae: sa_motion_publish_stop(ctx,0x8c6154aeu); if(!(*sr&1u))goto L8c6154b4;goto L8c6154b0; /* bf 0x8c6154b4 */
L8c6154b0: sa_motion_publish_stop(ctx,0x8c6154b2u); goto L8c6155da; /* bra 0x8c6155da */
L8c6154b4: sa_motion_publish_stop(ctx,0x8c6154b4u); r[7]=r[15];goto L8c6154b6; /* mov r15,r7 */
L8c6154b6: sa_motion_publish_stop(ctx,0x8c6154b6u); r[7]+=64u;goto L8c6154b8; /* add #0x40,r7 */
L8c6154b8: sa_motion_publish_stop(ctx,0x8c6154b8u); wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c6154ba; /* mov.l r10,@r7 */
L8c6154ba: sa_motion_publish_stop(ctx,0x8c6154bau); wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c6154bc; /* mov.l r11,@(0x4,r7) */
L8c6154bc: sa_motion_publish_stop(ctx,0x8c6154bcu); wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c6154be; /* mov.l r12,@(0x8,r7) */
L8c6154be: sa_motion_publish_stop(ctx,0x8c6154c0u); r[6]=0u;goto L8c61556c; /* bra 0x8c61556c */
L8c6154c2: sa_motion_publish_stop(ctx,0x8c6154c2u); r[2]=r[6];goto L8c6154c4; /* mov r6,r2 */
L8c6154c4: sa_motion_publish_stop(ctx,0x8c6154c4u); r[1]=r[6];goto L8c6154c6; /* mov r6,r1 */
L8c6154c6: sa_motion_publish_stop(ctx,0x8c6154c6u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6154c8; /* shll r2 */
L8c6154c8: sa_motion_publish_stop(ctx,0x8c6154c8u); r[3]=r[6];goto L8c6154ca; /* mov r6,r3 */
L8c6154ca: sa_motion_publish_stop(ctx,0x8c6154cau); r[3]<<=2;goto L8c6154cc; /* shll2 r3 */
L8c6154cc: sa_motion_publish_stop(ctx,0x8c6154ccu); r[2]+=r[1];goto L8c6154ce; /* add r1,r2 */
L8c6154ce: sa_motion_publish_stop(ctx,0x8c6154ceu); r[2]<<=2;goto L8c6154d0; /* shll2 r2 */
L8c6154d0: sa_motion_publish_stop(ctx,0x8c6154d0u); r[3]+=r[7];goto L8c6154d2; /* add r7,r3 */
L8c6154d2: sa_motion_publish_stop(ctx,0x8c6154d2u); r[2]+=r[4];goto L8c6154d4; /* add r4,r2 */
L8c6154d4: sa_motion_publish_stop(ctx,0x8c6154d4u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6154d6; /* mov.l r3,@(0x4,r15) */
L8c6154d6: sa_motion_publish_stop(ctx,0x8c6154d6u); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6154d8; /* mov.l r2,@r15 */
L8c6154d8: sa_motion_publish_stop(ctx,0x8c6154d8u); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6154da; /* mov.l @r3,r1 */
L8c6154da: sa_motion_publish_stop(ctx,0x8c6154dau); r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c6154dc; /* mov.l @(0xc,r1),r2 */
L8c6154dc: sa_motion_publish_stop(ctx,0x8c6154dcu); *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c6154de; /* tst r5,r2 */
L8c6154de: sa_motion_publish_stop(ctx,0x8c6154e0u); take=!(*sr&1u);r[6]+=1u;if(take)goto L8c615544;goto L8c6154e2; /* bf/s 0x8c615544 */
L8c6154e2: sa_motion_publish_stop(ctx,0x8c6154e2u); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6154e4; /* mov.l @(0x4,r15),r1 */
L8c6154e4: sa_motion_publish_stop(ctx,0x8c6154e4u); r[0]=24u;goto L8c6154e6; /* mov #0x18,r0 */
L8c6154e6: sa_motion_publish_stop(ctx,0x8c6154e6u); fcopy(f,4u,12u);goto L8c6154e8; /* fmov fr12,fr4 */
L8c6154e8: sa_motion_publish_stop(ctx,0x8c6154e8u); r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c6154ea; /* mov.l @r1,r3 */
L8c6154ea: sa_motion_publish_stop(ctx,0x8c6154eau); r[4]=rd(m,bus,&io_status,0x8c615538u,4);if(io_status)return io_status;goto L8c6154ec; /* mov.l 0x8c615538,r4 */
L8c6154ec: sa_motion_publish_stop(ctx,0x8c6154ecu); wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6154ee; /* mov.l r3,@(0x10,r15) */
L8c6154ee: sa_motion_publish_stop(ctx,0x8c6154eeu); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6154f0; /* fmov.s @(r0,r3),fr3 */
L8c6154f0: sa_motion_publish_stop(ctx,0x8c6154f0u); r[0]=12u;goto L8c6154f2; /* mov #0xc,r0 */
L8c6154f2: sa_motion_publish_stop(ctx,0x8c6154f2u); r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c6154f4; /* mov.l @(0x10,r15),r2 */
L8c6154f4: sa_motion_publish_stop(ctx,0x8c6154f4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6154f6; /* fdiv fr3,fr4 */
L8c6154f6: sa_motion_publish_stop(ctx,0x8c6154f6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6154f8; /* fmov.s @(r0,r4),fr3 */
L8c6154f8: sa_motion_publish_stop(ctx,0x8c6154f8u); r[0]=16u;goto L8c6154fa; /* mov #0x10,r0 */
L8c6154fa: sa_motion_publish_stop(ctx,0x8c6154fau); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6154fc; /* mov.l @r15,r3 */
L8c6154fc: sa_motion_publish_stop(ctx,0x8c6154fcu); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6154fe; /* fmov.s @(r0,r2),fr2 */
L8c6154fe: sa_motion_publish_stop(ctx,0x8c6154feu); r[0]=16u;goto L8c615500; /* mov #0x10,r0 */
L8c615500: sa_motion_publish_stop(ctx,0x8c615500u); r[2]=rd(m,bus,&io_status,0x8c61553cu,4);if(io_status)return io_status;goto L8c615502; /* mov.l 0x8c61553c,r2 */
L8c615502: sa_motion_publish_stop(ctx,0x8c615502u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615504; /* fsub fr3,fr2 */
L8c615504: sa_motion_publish_stop(ctx,0x8c615504u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c615506; /* fmov.s @r2,fr3 */
L8c615506: sa_motion_publish_stop(ctx,0x8c615506u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615508; /* fmul fr4,fr2 */
L8c615508: sa_motion_publish_stop(ctx,0x8c615508u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61550a; /* fdiv fr3,fr2 */
L8c61550a: sa_motion_publish_stop(ctx,0x8c61550au); fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61550c; /* fmov.s fr2,@r3 */
L8c61550c: sa_motion_publish_stop(ctx,0x8c61550cu); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61550e; /* mov.l @(0x4,r15),r1 */
L8c61550e: sa_motion_publish_stop(ctx,0x8c61550eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615510; /* fmov.s @(r0,r4),fr3 */
L8c615510: sa_motion_publish_stop(ctx,0x8c615510u); r[0]=20u;goto L8c615512; /* mov #0x14,r0 */
L8c615512: sa_motion_publish_stop(ctx,0x8c615512u); r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c615514; /* mov.l @r1,r1 */
L8c615514: sa_motion_publish_stop(ctx,0x8c615514u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615516; /* mov.l @r15,r3 */
L8c615516: sa_motion_publish_stop(ctx,0x8c615516u); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c615518; /* fmov.s @(r0,r1),fr2 */
L8c615518: sa_motion_publish_stop(ctx,0x8c615518u); r[1]=rd(m,bus,&io_status,0x8c615540u,4);if(io_status)return io_status;goto L8c61551a; /* mov.l 0x8c615540,r1 */
L8c61551a: sa_motion_publish_stop(ctx,0x8c61551au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61551c; /* fsub fr3,fr2 */
L8c61551c: sa_motion_publish_stop(ctx,0x8c61551cu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61551e; /* fmov.s @r1,fr3 */
L8c61551e: sa_motion_publish_stop(ctx,0x8c61551eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615520; /* fmul fr4,fr2 */
L8c615520: sa_motion_publish_stop(ctx,0x8c615520u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615522; /* fdiv fr3,fr2 */
L8c615522: sa_motion_publish_stop(ctx,0x8c615522u); r[0]=4u;goto L8c615524; /* mov #0x4,r0 */
L8c615524: sa_motion_publish_stop(ctx,0x8c615524u); fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c615526; /* fmov fr2,@(r0,r3) */
L8c615526: sa_motion_publish_stop(ctx,0x8c615526u); r[0]=8u;goto L8c615528; /* mov #0x8,r0 */
L8c615528: sa_motion_publish_stop(ctx,0x8c615528u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61552a; /* mov.l @r15,r3 */
L8c61552a: sa_motion_publish_stop(ctx,0x8c61552au); fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c61552c; /* fmov fr4,@(r0,r3) */
L8c61552c: sa_motion_publish_stop(ctx,0x8c61552eu); goto L8c61556c; /* bra 0x8c61556c */
L8c615544: sa_motion_publish_stop(ctx,0x8c615544u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615546; /* mov.l @(0x4,r15),r3 */
L8c615546: sa_motion_publish_stop(ctx,0x8c615546u); r[0]=16u;goto L8c615548; /* mov #0x10,r0 */
L8c615548: sa_motion_publish_stop(ctx,0x8c615548u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61554a; /* mov.l @r15,r2 */
L8c61554a: sa_motion_publish_stop(ctx,0x8c61554au); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61554c; /* mov.l @r3,r1 */
L8c61554c: sa_motion_publish_stop(ctx,0x8c61554cu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61554e; /* fmov.s @(r0,r1),fr3 */
L8c61554e: sa_motion_publish_stop(ctx,0x8c61554eu); r[0]=20u;goto L8c615550; /* mov #0x14,r0 */
L8c615550: sa_motion_publish_stop(ctx,0x8c615550u); fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c615552; /* fmov.s fr3,@r2 */
L8c615552: sa_motion_publish_stop(ctx,0x8c615552u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615554; /* mov.l @(0x4,r15),r2 */
L8c615554: sa_motion_publish_stop(ctx,0x8c615554u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615556; /* mov.l @r15,r3 */
L8c615556: sa_motion_publish_stop(ctx,0x8c615556u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c615558; /* mov.l @r2,r1 */
L8c615558: sa_motion_publish_stop(ctx,0x8c615558u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61555a; /* fmov.s @(r0,r1),fr3 */
L8c61555a: sa_motion_publish_stop(ctx,0x8c61555au); r[0]=4u;goto L8c61555c; /* mov #0x4,r0 */
L8c61555c: sa_motion_publish_stop(ctx,0x8c61555cu); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61555e; /* fmov fr3,@(r0,r3) */
L8c61555e: sa_motion_publish_stop(ctx,0x8c61555eu); r[0]=24u;goto L8c615560; /* mov #0x18,r0 */
L8c615560: sa_motion_publish_stop(ctx,0x8c615560u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615562; /* mov.l @(0x4,r15),r2 */
L8c615562: sa_motion_publish_stop(ctx,0x8c615562u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615564; /* mov.l @r15,r3 */
L8c615564: sa_motion_publish_stop(ctx,0x8c615564u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c615566; /* mov.l @r2,r1 */
L8c615566: sa_motion_publish_stop(ctx,0x8c615566u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c615568; /* fmov.s @(r0,r1),fr3 */
L8c615568: sa_motion_publish_stop(ctx,0x8c615568u); r[0]=8u;goto L8c61556a; /* mov #0x8,r0 */
L8c61556a: sa_motion_publish_stop(ctx,0x8c61556au); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61556c; /* fmov fr3,@(r0,r3) */
L8c61556c: sa_motion_publish_stop(ctx,0x8c61556cu); r[3]=3u;goto L8c61556e; /* mov #0x3,r3 */
L8c61556e: sa_motion_publish_stop(ctx,0x8c61556eu); r[4]=r[15];goto L8c615570; /* mov r15,r4 */
L8c615570: sa_motion_publish_stop(ctx,0x8c615570u); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c615572; /* cmp/ge r3,r6 */
L8c615572: sa_motion_publish_stop(ctx,0x8c615574u); take=!(*sr&1u);r[4]+=28u;if(take)goto L8c6154c2;goto L8c615576; /* bf/s 0x8c6154c2 */
L8c615576: sa_motion_publish_stop(ctx,0x8c615576u); r[6]=r[4];goto L8c615578; /* mov r4,r6 */
L8c615578: sa_motion_publish_stop(ctx,0x8c615578u); r[0]=8u;goto L8c61557a; /* mov #0x8,r0 */
L8c61557a: sa_motion_publish_stop(ctx,0x8c61557au); r[3]=r[4];goto L8c61557c; /* mov r4,r3 */
L8c61557c: sa_motion_publish_stop(ctx,0x8c61557cu); fload(m,bus,&io_status,f,6u,r[0]+r[6]);if(io_status)return io_status;goto L8c61557e; /* fmov.s @(r0,r6),fr6 */
L8c61557e: sa_motion_publish_stop(ctx,0x8c61557eu); r[3]+=12u;goto L8c615580; /* add #0xc,r3 */
L8c615580: sa_motion_publish_stop(ctx,0x8c615580u); r[7]=r[4];goto L8c615582; /* mov r4,r7 */
L8c615582: sa_motion_publish_stop(ctx,0x8c615582u); r[0]=4u;goto L8c615584; /* mov #0x4,r0 */
L8c615584: sa_motion_publish_stop(ctx,0x8c615584u); fcopy(f,0u,6u);goto L8c615586; /* fmov fr6,fr0 */
L8c615586: sa_motion_publish_stop(ctx,0x8c615586u); r[7]+=24u;goto L8c615588; /* add #0x18,r7 */
L8c615588: sa_motion_publish_stop(ctx,0x8c615588u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61558a; /* mov.l r3,@r15 */
L8c61558a: sa_motion_publish_stop(ctx,0x8c61558au); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c61558c; /* fmov.s @(r0,r6),fr4 */
L8c61558c: sa_motion_publish_stop(ctx,0x8c61558cu); fload(m,bus,&io_status,f,5u,r[0]+r[3]);if(io_status)return io_status;goto L8c61558e; /* fmov.s @(r0,r3),fr5 */
L8c61558e: sa_motion_publish_stop(ctx,0x8c61558eu); r[0]=8u;goto L8c615590; /* mov #0x8,r0 */
L8c615590: sa_motion_publish_stop(ctx,0x8c615590u); fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c615592; /* fmov.s @(r0,r3),fr9 */
L8c615592: sa_motion_publish_stop(ctx,0x8c615592u); r[0]=4u;goto L8c615594; /* mov #0x4,r0 */
L8c615594: sa_motion_publish_stop(ctx,0x8c615594u); fcopy(f,3u,4u);goto L8c615596; /* fmov fr4,fr3 */
L8c615596: sa_motion_publish_stop(ctx,0x8c615596u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c615598; /* fmul fr5,fr0 */
L8c615598: sa_motion_publish_stop(ctx,0x8c615598u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61559a; /* fmul fr9,fr3 */
L8c61559a: sa_motion_publish_stop(ctx,0x8c61559au); fload(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;goto L8c61559c; /* fmov.s @r6,fr8 */
L8c61559c: sa_motion_publish_stop(ctx,0x8c61559cu); fload(m,bus,&io_status,f,7u,r[3]);if(io_status)return io_status;goto L8c61559e; /* fmov.s @r3,fr7 */
L8c61559e: sa_motion_publish_stop(ctx,0x8c61559eu); fcopy(f,2u,8u);goto L8c6155a0; /* fmov fr8,fr2 */
L8c6155a0: sa_motion_publish_stop(ctx,0x8c6155a0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[9],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6155a2; /* fmul fr9,fr2 */
L8c6155a2: sa_motion_publish_stop(ctx,0x8c6155a2u); fcopy(f,1u,4u);goto L8c6155a4; /* fmov fr4,fr1 */
L8c6155a4: sa_motion_publish_stop(ctx,0x8c6155a4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6155a6; /* fsub fr3,fr0 */
L8c6155a6: sa_motion_publish_stop(ctx,0x8c6155a6u); fcopy(f,3u,6u);goto L8c6155a8; /* fmov fr6,fr3 */
L8c6155a8: sa_motion_publish_stop(ctx,0x8c6155a8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6155aa; /* fmul fr7,fr3 */
L8c6155aa: sa_motion_publish_stop(ctx,0x8c6155aau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6155ac; /* fsub fr3,fr2 */
L8c6155ac: sa_motion_publish_stop(ctx,0x8c6155acu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6155ae; /* fmov.s @(r0,r7),fr3 */
L8c6155ae: sa_motion_publish_stop(ctx,0x8c6155aeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6155b0; /* fmul fr2,fr3 */
L8c6155b0: sa_motion_publish_stop(ctx,0x8c6155b0u); fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c6155b2; /* fmov.s @r7,fr2 */
L8c6155b2: sa_motion_publish_stop(ctx,0x8c6155b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6155b4; /* fmac fr0,fr2,fr3 */
L8c6155b4: sa_motion_publish_stop(ctx,0x8c6155b4u); fcopy(f,2u,3u);goto L8c6155b6; /* fmov fr3,fr2 */
L8c6155b6: sa_motion_publish_stop(ctx,0x8c6155b6u); fcopy(f,3u,8u);goto L8c6155b8; /* fmov fr8,fr3 */
L8c6155b8: sa_motion_publish_stop(ctx,0x8c6155b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6155ba; /* fmul fr5,fr3 */
L8c6155ba: sa_motion_publish_stop(ctx,0x8c6155bau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6155bc; /* fmul fr7,fr1 */
L8c6155bc: sa_motion_publish_stop(ctx,0x8c6155bcu); r[0]=8u;goto L8c6155be; /* mov #0x8,r0 */
L8c6155be: sa_motion_publish_stop(ctx,0x8c6155beu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6155c0; /* fsub fr3,fr1 */
L8c6155c0: sa_motion_publish_stop(ctx,0x8c6155c0u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6155c2; /* fmov.s @(r0,r7),fr3 */
L8c6155c2: sa_motion_publish_stop(ctx,0x8c6155c2u); fcopy(f,0u,1u);goto L8c6155c4; /* fmov fr1,fr0 */
L8c6155c4: sa_motion_publish_stop(ctx,0x8c6155c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6155c6; /* fmac fr0,fr3,fr2 */
L8c6155c6: sa_motion_publish_stop(ctx,0x8c6155c6u); fcopy(f,3u,15u);goto L8c6155c8; /* fmov fr15,fr3 */
L8c6155c8: sa_motion_publish_stop(ctx,0x8c6155c8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c6155ca; /* fneg fr3 */
L8c6155ca: sa_motion_publish_stop(ctx,0x8c6155cau); fcopy(f,4u,2u);goto L8c6155cc; /* fmov fr2,fr4 */
L8c6155cc: sa_motion_publish_stop(ctx,0x8c6155ccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6155ce; /* fmul fr3,fr4 */
L8c6155ce: sa_motion_publish_stop(ctx,0x8c6155ceu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c6155d0; /* fldi0 fr2 */
L8c6155d0: sa_motion_publish_stop(ctx,0x8c6155d0u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c6155d2; /* fcmp/gt fr2,fr4 */
L8c6155d2: sa_motion_publish_stop(ctx,0x8c6155d2u); if((*sr&1u))goto L8c6155f8;goto L8c6155d4; /* bt 0x8c6155f8 */
L8c6155d4: sa_motion_publish_stop(ctx,0x8c6155d4u); wr(m,bus,&io_status,r[15]+0x14u,r[5],4);if(io_status)return io_status;goto L8c6155d6; /* mov.l r5,@(0x14,r15) */
L8c6155d6: sa_motion_publish_stop(ctx,0x8c6155d8u); fcopy(f,4u,12u);goto L8c6155f8; /* bra 0x8c6155f8 */
L8c6155da: sa_motion_publish_stop(ctx,0x8c6155dau); r[0]=20u;goto L8c6155dc; /* mov #0x14,r0 */
L8c6155dc: sa_motion_publish_stop(ctx,0x8c6155dcu); fcopy(f,3u,13u);goto L8c6155de; /* fmov fr13,fr3 */
L8c6155de: sa_motion_publish_stop(ctx,0x8c6155deu); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6155e0; /* fmov.s @(r0,r12),fr2 */
L8c6155e0: sa_motion_publish_stop(ctx,0x8c6155e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6155e2; /* fsub fr6,fr3 */
L8c6155e2: sa_motion_publish_stop(ctx,0x8c6155e2u); r[0]=16u;goto L8c6155e4; /* mov #0x10,r0 */
L8c6155e4: sa_motion_publish_stop(ctx,0x8c6155e4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6155e6; /* fsub fr14,fr2 */
L8c6155e6: sa_motion_publish_stop(ctx,0x8c6155e6u); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c6155e8; /* fmov.s @(r0,r12),fr1 */
L8c6155e8: sa_motion_publish_stop(ctx,0x8c6155e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6155ea; /* fsub fr13,fr1 */
L8c6155ea: sa_motion_publish_stop(ctx,0x8c6155eau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6155ec; /* fmul fr2,fr3 */
L8c6155ec: sa_motion_publish_stop(ctx,0x8c6155ecu); fcopy(f,2u,14u);goto L8c6155ee; /* fmov fr14,fr2 */
L8c6155ee: sa_motion_publish_stop(ctx,0x8c6155eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6155f0; /* fsub fr5,fr2 */
L8c6155f0: sa_motion_publish_stop(ctx,0x8c6155f0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6155f2; /* fmul fr1,fr2 */
L8c6155f2: sa_motion_publish_stop(ctx,0x8c6155f2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6155f4; /* fsub fr2,fr3 */
L8c6155f4: sa_motion_publish_stop(ctx,0x8c6155f4u); fcopy(f,4u,3u);goto L8c6155f6; /* fmov fr3,fr4 */
L8c6155f6: sa_motion_publish_stop(ctx,0x8c6155f6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6155f8; /* fmul fr15,fr4 */
L8c6155f8: sa_motion_publish_stop(ctx,0x8c6155f8u); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c6155fa; /* cmp/hs r8,r9 */
L8c6155fa: sa_motion_publish_stop(ctx,0x8c6155fau); if((*sr&1u))goto L8c615630;goto L8c6155fc; /* bt 0x8c615630 */
L8c6155fc: sa_motion_publish_stop(ctx,0x8c6155fcu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6155fe; /* fldi0 fr3 */
L8c6155fe: sa_motion_publish_stop(ctx,0x8c6155feu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c615600; /* fcmp/gt fr4,fr3 */
L8c615600: sa_motion_publish_stop(ctx,0x8c615600u); if((*sr&1u))goto L8c615660;goto L8c615602; /* bt 0x8c615660 */
L8c615602: sa_motion_publish_stop(ctx,0x8c615602u); r[5]=rd(m,bus,&io_status,0x8c615764u,4);if(io_status)return io_status;goto L8c615604; /* mov.l 0x8c615764,r5 */
L8c615604: sa_motion_publish_stop(ctx,0x8c615604u); r[4]=r[11];goto L8c615606; /* mov r11,r4 */
L8c615606: sa_motion_publish_stop(ctx,0x8c615606u); r[8]+=16u;goto L8c615608; /* add #0x10,r8 */
L8c615608: sa_motion_publish_stop(ctx,0x8c615608u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61560a; /* fschg */
L8c61560a: sa_motion_publish_stop(ctx,0x8c61560au); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61560c; /* fmov.s @r4+,fr0 */
L8c61560c: sa_motion_publish_stop(ctx,0x8c61560cu); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61560e; /* fmov.s @r4+,fr2 */
L8c61560e: sa_motion_publish_stop(ctx,0x8c61560eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c615610; /* fldi1 fr3 */
L8c615610: sa_motion_publish_stop(ctx,0x8c615610u); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c615612; /* ftrv xmtrx,fv0 */
L8c615612: sa_motion_publish_stop(ctx,0x8c615612u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615614; /* fmov.s @r4+,fr4 */
L8c615614: sa_motion_publish_stop(ctx,0x8c615614u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615616; /* fmov.s @r4,fr6 */
L8c615616: sa_motion_publish_stop(ctx,0x8c615616u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615618; /* fschg */
L8c615618: sa_motion_publish_stop(ctx,0x8c615618u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c61561a; /* fmov.s fr6,@-r8 */
L8c61561a: sa_motion_publish_stop(ctx,0x8c61561au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61561c; /* fmov.s fr5,@-r8 */
L8c61561c: sa_motion_publish_stop(ctx,0x8c61561cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61561e; /* fmov.s fr4,@-r8 */
L8c61561e: sa_motion_publish_stop(ctx,0x8c61561eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615620; /* mov.l r5,@-r8 */
L8c615620: sa_motion_publish_stop(ctx,0x8c615620u); r[8]+=28u;goto L8c615622; /* add #0x1c,r8 */
L8c615622: sa_motion_publish_stop(ctx,0x8c615622u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c615624; /* fmov.s fr2,@-r8 */
L8c615624: sa_motion_publish_stop(ctx,0x8c615624u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c615626; /* fmov.s fr1,@-r8 */
L8c615626: sa_motion_publish_stop(ctx,0x8c615626u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c615628; /* fmov.s fr0,@-r8 */
L8c615628: sa_motion_publish_stop(ctx,0x8c615628u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61562a; /* pref @r8 */
L8c61562a: sa_motion_publish_stop(ctx,0x8c61562au); r[8]+=16u;goto L8c61562c; /* add #0x10,r8 */
L8c61562c: sa_motion_publish_stop(ctx,0x8c61562eu); r[9]=r[8];goto L8c61568a; /* bra 0x8c61568a */
L8c615630: sa_motion_publish_stop(ctx,0x8c615630u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615632; /* fldi0 fr3 */
L8c615632: sa_motion_publish_stop(ctx,0x8c615632u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c615634; /* fcmp/gt fr4,fr3 */
L8c615634: sa_motion_publish_stop(ctx,0x8c615634u); if(!(*sr&1u))goto L8c61568a;goto L8c615636; /* bf 0x8c61568a */
L8c615636: sa_motion_publish_stop(ctx,0x8c615636u); r[5]=rd(m,bus,&io_status,0x8c615768u,4);if(io_status)return io_status;goto L8c615638; /* mov.l 0x8c615768,r5 */
L8c615638: sa_motion_publish_stop(ctx,0x8c615638u); r[4]=r[10];goto L8c61563a; /* mov r10,r4 */
L8c61563a: sa_motion_publish_stop(ctx,0x8c61563au); r[8]+=16u;goto L8c61563c; /* add #0x10,r8 */
L8c61563c: sa_motion_publish_stop(ctx,0x8c61563cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61563e; /* fschg */
L8c61563e: sa_motion_publish_stop(ctx,0x8c61563eu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615640; /* fmov.s @r4+,fr0 */
L8c615640: sa_motion_publish_stop(ctx,0x8c615640u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615642; /* fmov.s @r4+,fr2 */
L8c615642: sa_motion_publish_stop(ctx,0x8c615642u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c615644; /* fldi1 fr3 */
L8c615644: sa_motion_publish_stop(ctx,0x8c615644u); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c615646; /* ftrv xmtrx,fv0 */
L8c615646: sa_motion_publish_stop(ctx,0x8c615646u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615648; /* fmov.s @r4+,fr4 */
L8c615648: sa_motion_publish_stop(ctx,0x8c615648u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61564a; /* fmov.s @r4,fr6 */
L8c61564a: sa_motion_publish_stop(ctx,0x8c61564au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61564c; /* fschg */
L8c61564c: sa_motion_publish_stop(ctx,0x8c61564cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c61564e; /* fmov.s fr6,@-r8 */
L8c61564e: sa_motion_publish_stop(ctx,0x8c61564eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615650; /* fmov.s fr5,@-r8 */
L8c615650: sa_motion_publish_stop(ctx,0x8c615650u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615652; /* fmov.s fr4,@-r8 */
L8c615652: sa_motion_publish_stop(ctx,0x8c615652u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615654; /* mov.l r5,@-r8 */
L8c615654: sa_motion_publish_stop(ctx,0x8c615654u); r[8]+=28u;goto L8c615656; /* add #0x1c,r8 */
L8c615656: sa_motion_publish_stop(ctx,0x8c615656u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c615658; /* fmov.s fr2,@-r8 */
L8c615658: sa_motion_publish_stop(ctx,0x8c615658u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61565a; /* fmov.s fr1,@-r8 */
L8c61565a: sa_motion_publish_stop(ctx,0x8c61565au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c61565c; /* fmov.s fr0,@-r8 */
L8c61565c: sa_motion_publish_stop(ctx,0x8c61565cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61565e; /* pref @r8 */
L8c61565e: sa_motion_publish_stop(ctx,0x8c61565eu); r[8]+=16u;goto L8c615660; /* add #0x10,r8 */
L8c615660: sa_motion_publish_stop(ctx,0x8c615660u); r[5]=rd(m,bus,&io_status,0x8c615768u,4);if(io_status)return io_status;goto L8c615662; /* mov.l 0x8c615768,r5 */
L8c615662: sa_motion_publish_stop(ctx,0x8c615662u); r[4]=r[11];goto L8c615664; /* mov r11,r4 */
L8c615664: sa_motion_publish_stop(ctx,0x8c615664u); r[8]+=16u;goto L8c615666; /* add #0x10,r8 */
L8c615666: sa_motion_publish_stop(ctx,0x8c615666u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615668; /* fschg */
L8c615668: sa_motion_publish_stop(ctx,0x8c615668u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61566a; /* fmov.s @r4+,fr0 */
L8c61566a: sa_motion_publish_stop(ctx,0x8c61566au); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61566c; /* fmov.s @r4+,fr2 */
L8c61566c: sa_motion_publish_stop(ctx,0x8c61566cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c61566e; /* fldi1 fr3 */
L8c61566e: sa_motion_publish_stop(ctx,0x8c61566eu); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c615670; /* ftrv xmtrx,fv0 */
L8c615670: sa_motion_publish_stop(ctx,0x8c615670u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615672; /* fmov.s @r4+,fr4 */
L8c615672: sa_motion_publish_stop(ctx,0x8c615672u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615674; /* fmov.s @r4,fr6 */
L8c615674: sa_motion_publish_stop(ctx,0x8c615674u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615676; /* fschg */
L8c615676: sa_motion_publish_stop(ctx,0x8c615676u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615678; /* fmov.s fr6,@-r8 */
L8c615678: sa_motion_publish_stop(ctx,0x8c615678u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61567a; /* fmov.s fr5,@-r8 */
L8c61567a: sa_motion_publish_stop(ctx,0x8c61567au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61567c; /* fmov.s fr4,@-r8 */
L8c61567c: sa_motion_publish_stop(ctx,0x8c61567cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61567e; /* mov.l r5,@-r8 */
L8c61567e: sa_motion_publish_stop(ctx,0x8c61567eu); r[8]+=28u;goto L8c615680; /* add #0x1c,r8 */
L8c615680: sa_motion_publish_stop(ctx,0x8c615680u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c615682; /* fmov.s fr2,@-r8 */
L8c615682: sa_motion_publish_stop(ctx,0x8c615682u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c615684; /* fmov.s fr1,@-r8 */
L8c615684: sa_motion_publish_stop(ctx,0x8c615684u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c615686; /* fmov.s fr0,@-r8 */
L8c615686: sa_motion_publish_stop(ctx,0x8c615686u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615688; /* pref @r8 */
L8c615688: sa_motion_publish_stop(ctx,0x8c615688u); r[8]+=16u;goto L8c61568a; /* add #0x10,r8 */
L8c61568a: sa_motion_publish_stop(ctx,0x8c61568au); r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c61568c; /* mov.l @(0x14,r15),r2 */
L8c61568c: sa_motion_publish_stop(ctx,0x8c61568cu); *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c61568e; /* tst r2,r2 */
L8c61568e: sa_motion_publish_stop(ctx,0x8c61568eu); if((*sr&1u))goto L8c6156a2;goto L8c615690; /* bt 0x8c6156a2 */
L8c615690: sa_motion_publish_stop(ctx,0x8c615690u); r[4]=rd(m,bus,&io_status,0x8c61576cu,4);if(io_status)return io_status;goto L8c615692; /* mov.l 0x8c61576c,r4 */
L8c615692: sa_motion_publish_stop(ctx,0x8c615692u); r[3]=0u;goto L8c615694; /* mov #0x0,r3 */
L8c615694: sa_motion_publish_stop(ctx,0x8c615694u); r[2]=rd(m,bus,&io_status,0x8c615770u,4);if(io_status)return io_status;goto L8c615696; /* mov.l 0x8c615770,r2 */
L8c615696: sa_motion_publish_stop(ctx,0x8c615696u); r[5]=r[11];goto L8c615698; /* mov r11,r5 */
L8c615698: sa_motion_publish_stop(ctx,0x8c615698u); wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c61569a; /* mov.l r3,@r4 */
L8c61569a: sa_motion_publish_stop(ctx,0x8c61569au); r[6]=r[12];goto L8c61569c; /* mov r12,r6 */
L8c61569c: sa_motion_publish_stop(ctx,0x8c61569eu); target=r[2];*pr=0x8c6156a0u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6156a0; /* jsr @r2 */
L8c6156a0: sa_motion_publish_stop(ctx,0x8c6156a0u); r[9]=r[8];goto L8c6156a2; /* mov r8,r9 */
L8c6156a2: sa_motion_publish_stop(ctx,0x8c6156a2u); fcopy(f,6u,13u);goto L8c6156a4; /* fmov fr13,fr6 */
L8c6156a4: sa_motion_publish_stop(ctx,0x8c6156a4u); r[0]=16u;goto L8c6156a6; /* mov #0x10,r0 */
L8c6156a6: sa_motion_publish_stop(ctx,0x8c6156a6u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6156a8; /* mov.l @(0x8,r15),r3 */
L8c6156a8: sa_motion_publish_stop(ctx,0x8c6156a8u); r[10]=r[11];goto L8c6156aa; /* mov r11,r10 */
L8c6156aa: sa_motion_publish_stop(ctx,0x8c6156aau); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c6156ac; /* fmov.s @(r0,r12),fr13 */
L8c6156ac: sa_motion_publish_stop(ctx,0x8c6156acu); r[0]=20u;goto L8c6156ae; /* mov #0x14,r0 */
L8c6156ae: sa_motion_publish_stop(ctx,0x8c6156aeu); fcopy(f,5u,14u);goto L8c6156b0; /* fmov fr14,fr5 */
L8c6156b0: sa_motion_publish_stop(ctx,0x8c6156b0u); r[3]+=4294967295u;goto L8c6156b2; /* add #-0x1,r3 */
L8c6156b2: sa_motion_publish_stop(ctx,0x8c6156b2u); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c6156b4; /* fmov.s @(r0,r12),fr14 */
L8c6156b4: sa_motion_publish_stop(ctx,0x8c6156b4u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6156b6; /* tst r3,r3 */
L8c6156b6: sa_motion_publish_stop(ctx,0x8c6156b6u); r[11]=r[12];goto L8c6156b8; /* mov r12,r11 */
L8c6156b8: sa_motion_publish_stop(ctx,0x8c6156b8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c6156ba; /* fneg fr15 */
L8c6156ba: sa_motion_publish_stop(ctx,0x8c6156bcu); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c6156c2;goto L8c6156be; /* bt/s 0x8c6156c2 */
L8c6156be: sa_motion_publish_stop(ctx,0x8c6156c0u); goto L8c615468; /* bra 0x8c615468 */
L8c6156c2: sa_motion_publish_stop(ctx,0x8c6156c2u); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c6156c4; /* cmp/hs r8,r9 */
L8c6156c4: sa_motion_publish_stop(ctx,0x8c6156c4u); if((*sr&1u))goto L8c6156f0;goto L8c6156c6; /* bt 0x8c6156f0 */
L8c6156c6: sa_motion_publish_stop(ctx,0x8c6156c6u); r[5]=rd(m,bus,&io_status,0x8c615764u,4);if(io_status)return io_status;goto L8c6156c8; /* mov.l 0x8c615764,r5 */
L8c6156c8: sa_motion_publish_stop(ctx,0x8c6156c8u); r[4]=r[12];goto L8c6156ca; /* mov r12,r4 */
L8c6156ca: sa_motion_publish_stop(ctx,0x8c6156cau); r[8]+=16u;goto L8c6156cc; /* add #0x10,r8 */
L8c6156cc: sa_motion_publish_stop(ctx,0x8c6156ccu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6156ce; /* fschg */
L8c6156ce: sa_motion_publish_stop(ctx,0x8c6156ceu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6156d0; /* fmov.s @r4+,fr0 */
L8c6156d0: sa_motion_publish_stop(ctx,0x8c6156d0u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6156d2; /* fmov.s @r4+,fr2 */
L8c6156d2: sa_motion_publish_stop(ctx,0x8c6156d2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6156d4; /* fldi1 fr3 */
L8c6156d4: sa_motion_publish_stop(ctx,0x8c6156d4u); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6156d6; /* ftrv xmtrx,fv0 */
L8c6156d6: sa_motion_publish_stop(ctx,0x8c6156d6u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6156d8; /* fmov.s @r4+,fr4 */
L8c6156d8: sa_motion_publish_stop(ctx,0x8c6156d8u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6156da; /* fmov.s @r4,fr6 */
L8c6156da: sa_motion_publish_stop(ctx,0x8c6156dau); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6156dc; /* fschg */
L8c6156dc: sa_motion_publish_stop(ctx,0x8c6156dcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6156de; /* fmov.s fr6,@-r8 */
L8c6156de: sa_motion_publish_stop(ctx,0x8c6156deu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6156e0; /* fmov.s fr5,@-r8 */
L8c6156e0: sa_motion_publish_stop(ctx,0x8c6156e0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6156e2; /* fmov.s fr4,@-r8 */
L8c6156e2: sa_motion_publish_stop(ctx,0x8c6156e2u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6156e4; /* mov.l r5,@-r8 */
L8c6156e4: sa_motion_publish_stop(ctx,0x8c6156e4u); r[8]+=28u;goto L8c6156e6; /* add #0x1c,r8 */
L8c6156e6: sa_motion_publish_stop(ctx,0x8c6156e6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6156e8; /* fmov.s fr2,@-r8 */
L8c6156e8: sa_motion_publish_stop(ctx,0x8c6156e8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6156ea; /* fmov.s fr1,@-r8 */
L8c6156ea: sa_motion_publish_stop(ctx,0x8c6156eau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c6156ec; /* fmov.s fr0,@-r8 */
L8c6156ec: sa_motion_publish_stop(ctx,0x8c6156ecu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6156ee; /* pref @r8 */
L8c6156ee: sa_motion_publish_stop(ctx,0x8c6156eeu); r[8]+=16u;goto L8c6156f0; /* add #0x10,r8 */
L8c6156f0: sa_motion_publish_stop(ctx,0x8c6156f0u); r[3]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c6156f2; /* mov.l @(0x18,r15),r3 */
L8c6156f2: sa_motion_publish_stop(ctx,0x8c6156f2u); r[3]+=4294967295u;goto L8c6156f4; /* add #-0x1,r3 */
L8c6156f4: sa_motion_publish_stop(ctx,0x8c6156f4u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6156f6; /* tst r3,r3 */
L8c6156f6: sa_motion_publish_stop(ctx,0x8c6156f8u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;if(take)goto L8c6156fe;goto L8c6156fa; /* bt/s 0x8c6156fe */
L8c6156fa: sa_motion_publish_stop(ctx,0x8c6156fcu); goto L8c615418; /* bra 0x8c615418 */
L8c6156fe: sa_motion_publish_stop(ctx,0x8c6156feu); r[4]=rd(m,bus,&io_status,0x8c615774u,4);if(io_status)return io_status;goto L8c615700; /* mov.l 0x8c615774,r4 */
L8c615700: sa_motion_publish_stop(ctx,0x8c615700u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c615702; /* mov.l @(0xc,r15),r3 */
L8c615702: sa_motion_publish_stop(ctx,0x8c615702u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c615704; /* mov.l @r4,r2 */
L8c615704: sa_motion_publish_stop(ctx,0x8c615704u); r[2]+=r[3];goto L8c615706; /* add r3,r2 */
L8c615706: sa_motion_publish_stop(ctx,0x8c615706u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c615708; /* mov.l r2,@r4 */
L8c615708: sa_motion_publish_stop(ctx,0x8c615708u); r[4]=rd(m,bus,&io_status,0x8c615778u,4);if(io_status)return io_status;goto L8c61570a; /* mov.l 0x8c615778,r4 */
L8c61570a: sa_motion_publish_stop(ctx,0x8c61570au); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61570c; /* mov.l @(0xc,r15),r3 */
L8c61570c: sa_motion_publish_stop(ctx,0x8c61570cu); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61570e; /* mov.l @r4,r2 */
L8c61570e: sa_motion_publish_stop(ctx,0x8c61570eu); r[2]+=r[3];goto L8c615710; /* add r3,r2 */
L8c615710: sa_motion_publish_stop(ctx,0x8c615710u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c615712; /* mov.l r2,@r4 */
L8c615712: sa_motion_publish_stop(ctx,0x8c615712u); r[15]+=76u;goto L8c615714; /* add #0x4c,r15 */
L8c615714: sa_motion_publish_stop(ctx,0x8c615714u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615716; /* lds.l @r15+,PR */
L8c615716: sa_motion_publish_stop(ctx,0x8c615716u); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615718; /* fmov.s @r15+,fr12 */
L8c615718: sa_motion_publish_stop(ctx,0x8c615718u); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61571a; /* fmov.s @r15+,fr13 */
L8c61571a: sa_motion_publish_stop(ctx,0x8c61571au); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61571c; /* fmov.s @r15+,fr14 */
L8c61571c: sa_motion_publish_stop(ctx,0x8c61571cu); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61571e; /* fmov.s @r15+,fr15 */
L8c61571e: sa_motion_publish_stop(ctx,0x8c61571eu); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615720; /* mov.l @r15+,r9 */
L8c615720: sa_motion_publish_stop(ctx,0x8c615720u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615722; /* mov.l @r15+,r10 */
L8c615722: sa_motion_publish_stop(ctx,0x8c615722u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615724; /* mov.l @r15+,r11 */
L8c615724: sa_motion_publish_stop(ctx,0x8c615724u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615726; /* mov.l @r15+,r12 */
L8c615726: sa_motion_publish_stop(ctx,0x8c615726u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615728; /* mov.l @r15+,r13 */
L8c615728: sa_motion_publish_stop(ctx,0x8c61572au); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
