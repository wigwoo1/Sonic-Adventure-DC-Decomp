#include "sa_draw_motion_stop.h"
/* Face-selected alternate strip emitters with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_face_alternate_strip_8c613dfa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c613dfa;
L8c613dfa: sa_motion_publish_stop(ctx,0x8c613dfau); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c613dfc; /* mov.l r14,@-r15 */
L8c613dfc: sa_motion_publish_stop(ctx,0x8c613dfcu); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c613dfe; /* mov.l r13,@-r15 */
L8c613dfe: sa_motion_publish_stop(ctx,0x8c613dfeu); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c613e00; /* mov.l r12,@-r15 */
L8c613e00: sa_motion_publish_stop(ctx,0x8c613e00u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c613e02; /* mov.l r11,@-r15 */
L8c613e02: sa_motion_publish_stop(ctx,0x8c613e02u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c613e04; /* mov.l r10,@-r15 */
L8c613e04: sa_motion_publish_stop(ctx,0x8c613e04u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c613e06; /* mov.l r9,@-r15 */
L8c613e06: sa_motion_publish_stop(ctx,0x8c613e06u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c613e08; /* fmov.s fr15,@-r15 */
L8c613e08: sa_motion_publish_stop(ctx,0x8c613e08u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c613e0a; /* fmov.s fr14,@-r15 */
L8c613e0a: sa_motion_publish_stop(ctx,0x8c613e0au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c613e0c; /* fmov.s fr13,@-r15 */
L8c613e0c: sa_motion_publish_stop(ctx,0x8c613e0cu); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c613e0e; /* sts.l PR,@-r15 */
L8c613e0e: sa_motion_publish_stop(ctx,0x8c613e0eu); r[15]+=4294967280u;goto L8c613e10; /* add #-0x10,r15 */
L8c613e10: sa_motion_publish_stop(ctx,0x8c613e10u); r[3]=rd(m,bus,&io_status,0x8c613e44u,4);if(io_status)return io_status;goto L8c613e12; /* mov.l 0x8c613e44,r3 */
L8c613e12: sa_motion_publish_stop(ctx,0x8c613e12u); r[9]=rd(m,bus,&io_status,0x8c613e40u,4);if(io_status)return io_status;goto L8c613e14; /* mov.l 0x8c613e40,r9 */
L8c613e14: sa_motion_publish_stop(ctx,0x8c613e16u); target=r[3];*pr=0x8c613e18u;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c613e18; /* jsr @r3 */
L8c613e18: sa_motion_publish_stop(ctx,0x8c613e18u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613e1a; /* mov.l @(0x4,r15),r3 */
L8c613e1a: sa_motion_publish_stop(ctx,0x8c613e1au); r[2]=0u;goto L8c613e1c; /* mov #0x0,r2 */
L8c613e1c: sa_motion_publish_stop(ctx,0x8c613e1cu); r[14]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613e1e; /* mov.l @(0x4,r15),r14 */
L8c613e1e: sa_motion_publish_stop(ctx,0x8c613e1eu); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c613e20; /* mov.w @(0x2,r3),r0 */
L8c613e20: sa_motion_publish_stop(ctx,0x8c613e20u); r[3]=rd(m,bus,&io_status,0x8c613e48u,4);if(io_status)return io_status;goto L8c613e22; /* mov.l 0x8c613e48,r3 */
L8c613e22: sa_motion_publish_stop(ctx,0x8c613e22u); r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c613e24; /* mov.l @(0x4,r14),r14 */
L8c613e24: sa_motion_publish_stop(ctx,0x8c613e24u); r[0]=r[0]&65535u;goto L8c613e26; /* extu.w r0,r0 */
L8c613e26: sa_motion_publish_stop(ctx,0x8c613e26u); wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c613e28; /* mov.l r0,@(0xc,r15) */
L8c613e28: sa_motion_publish_stop(ctx,0x8c613e28u); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c613e2a; /* mov.l @r3,r13 */
L8c613e2a: sa_motion_publish_stop(ctx,0x8c613e2au); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613e2c; /* mov.l r2,@(0x8,r15) */
L8c613e2c: sa_motion_publish_stop(ctx,0x8c613e2cu); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c613e2e; /* mov.l r2,@(0x4,r15) */
L8c613e2e: sa_motion_publish_stop(ctx,0x8c613e30u); goto L8c613e4c; /* bra 0x8c613e4c */
L8c613e4c: sa_motion_publish_stop(ctx,0x8c613e4cu); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613e4e; /* mov.w @r14+,r3 */
L8c613e4e: sa_motion_publish_stop(ctx,0x8c613e4eu); r[4]=rd(m,bus,&io_status,0x8c613f38u,4);if(io_status)return io_status;goto L8c613e50; /* mov.l 0x8c613f38,r4 */
L8c613e50: sa_motion_publish_stop(ctx,0x8c613e50u); r[3]=r[3]&65535u;goto L8c613e52; /* extu.w r3,r3 */
L8c613e52: sa_motion_publish_stop(ctx,0x8c613e52u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c613e54; /* fldi1 fr4 */
L8c613e54: sa_motion_publish_stop(ctx,0x8c613e54u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c613e56; /* tst r4,r3 */
L8c613e56: sa_motion_publish_stop(ctx,0x8c613e56u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c613e58; /* mov.l r3,@r15 */
L8c613e58: sa_motion_publish_stop(ctx,0x8c613e5au); take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c613e6a;goto L8c613e5c; /* bt/s 0x8c613e6a */
L8c613e5c: sa_motion_publish_stop(ctx,0x8c613e5cu); r[4]=sx(m,bus,&io_status,0x8c613f34u,2);if(io_status)return io_status;goto L8c613e5e; /* mov.w 0x8c613f34,r4 */
L8c613e5e: sa_motion_publish_stop(ctx,0x8c613e5eu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613e60; /* mov.l @r15,r3 */
L8c613e60: sa_motion_publish_stop(ctx,0x8c613e60u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c613e62; /* fldi1 fr4 */
L8c613e62: sa_motion_publish_stop(ctx,0x8c613e62u); r[3]&=r[4];goto L8c613e64; /* and r4,r3 */
L8c613e64: sa_motion_publish_stop(ctx,0x8c613e64u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c613e66; /* fneg fr4 */
L8c613e66: sa_motion_publish_stop(ctx,0x8c613e66u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c613e68; /* mov.l r3,@r15 */
L8c613e68: sa_motion_publish_stop(ctx,0x8c613e68u); fcopy(f,15u,4u);goto L8c613e6a; /* fmov fr4,fr15 */
L8c613e6a: sa_motion_publish_stop(ctx,0x8c613e6au); r[0]=16u;goto L8c613e6c; /* mov #0x10,r0 */
L8c613e6c: sa_motion_publish_stop(ctx,0x8c613e6cu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613e6e; /* mov.l @r15,r3 */
L8c613e6e: sa_motion_publish_stop(ctx,0x8c613e6eu); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613e70; /* mov.l @(0x4,r15),r2 */
L8c613e70: sa_motion_publish_stop(ctx,0x8c613e70u); r[10]=r[8];goto L8c613e72; /* mov r8,r10 */
L8c613e72: sa_motion_publish_stop(ctx,0x8c613e72u); r[2]+=r[3];goto L8c613e74; /* add r3,r2 */
L8c613e74: sa_motion_publish_stop(ctx,0x8c613e74u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c613e76; /* mov.l r2,@(0x4,r15) */
L8c613e76: sa_motion_publish_stop(ctx,0x8c613e76u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613e78; /* mov.l @r15,r1 */
L8c613e78: sa_motion_publish_stop(ctx,0x8c613e78u); r[1]+=4294967294u;goto L8c613e7a; /* add #-0x2,r1 */
L8c613e7a: sa_motion_publish_stop(ctx,0x8c613e7au); wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c613e7c; /* mov.l r1,@r15 */
L8c613e7c: sa_motion_publish_stop(ctx,0x8c613e7cu); r[4]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613e7e; /* mov.w @r14+,r4 */
L8c613e7e: sa_motion_publish_stop(ctx,0x8c613e7eu); r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613e80; /* mov.w @r14+,r11 */
L8c613e80: sa_motion_publish_stop(ctx,0x8c613e80u); r[4]=r[4]&65535u;goto L8c613e82; /* extu.w r4,r4 */
L8c613e82: sa_motion_publish_stop(ctx,0x8c613e82u); r[4]<<=2;goto L8c613e84; /* shll2 r4 */
L8c613e84: sa_motion_publish_stop(ctx,0x8c613e84u); r[11]=r[11]&65535u;goto L8c613e86; /* extu.w r11,r11 */
L8c613e86: sa_motion_publish_stop(ctx,0x8c613e86u); r[11]<<=2;goto L8c613e88; /* shll2 r11 */
L8c613e88: sa_motion_publish_stop(ctx,0x8c613e88u); r[4]<<=2;goto L8c613e8a; /* shll2 r4 */
L8c613e8a: sa_motion_publish_stop(ctx,0x8c613e8au); r[11]<<=2;goto L8c613e8c; /* shll2 r11 */
L8c613e8c: sa_motion_publish_stop(ctx,0x8c613e8cu); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c613e8e; /* shll r4 */
L8c613e8e: sa_motion_publish_stop(ctx,0x8c613e8eu); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c613e90; /* shll r11 */
L8c613e90: sa_motion_publish_stop(ctx,0x8c613e90u); r[11]+=r[13];goto L8c613e92; /* add r13,r11 */
L8c613e92: sa_motion_publish_stop(ctx,0x8c613e92u); r[4]+=r[13];goto L8c613e94; /* add r13,r4 */
L8c613e94: sa_motion_publish_stop(ctx,0x8c613e94u); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c613e96; /* fmov.s @(r0,r11),fr14 */
L8c613e96: sa_motion_publish_stop(ctx,0x8c613e96u); fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c613e98; /* fmov.s @(r0,r4),fr4 */
L8c613e98: sa_motion_publish_stop(ctx,0x8c613e98u); r[0]=20u;goto L8c613e9a; /* mov #0x14,r0 */
L8c613e9a: sa_motion_publish_stop(ctx,0x8c613e9au); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c613e9c; /* fmov.s @(r0,r11),fr13 */
L8c613e9c: sa_motion_publish_stop(ctx,0x8c613e9cu); fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c613e9e; /* fmov.s @(r0,r4),fr5 */
L8c613e9e: sa_motion_publish_stop(ctx,0x8c613e9eu); r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613ea0; /* mov.w @r14+,r12 */
L8c613ea0: sa_motion_publish_stop(ctx,0x8c613ea0u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c613ea2; /* mov.w @r14,r3 */
L8c613ea2: sa_motion_publish_stop(ctx,0x8c613ea2u); r[12]=r[12]&65535u;goto L8c613ea4; /* extu.w r12,r12 */
L8c613ea4: sa_motion_publish_stop(ctx,0x8c613ea4u); r[12]<<=2;goto L8c613ea6; /* shll2 r12 */
L8c613ea6: sa_motion_publish_stop(ctx,0x8c613ea6u); r[3]=r[3]&65535u;goto L8c613ea8; /* extu.w r3,r3 */
L8c613ea8: sa_motion_publish_stop(ctx,0x8c613ea8u); r[3]<<=2;goto L8c613eaa; /* shll2 r3 */
L8c613eaa: sa_motion_publish_stop(ctx,0x8c613eaau); r[12]<<=2;goto L8c613eac; /* shll2 r12 */
L8c613eac: sa_motion_publish_stop(ctx,0x8c613eacu); r[3]<<=2;goto L8c613eae; /* shll2 r3 */
L8c613eae: sa_motion_publish_stop(ctx,0x8c613eaeu); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c613eb0; /* shll r12 */
L8c613eb0: sa_motion_publish_stop(ctx,0x8c613eb0u); *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c613eb2; /* shll r3 */
L8c613eb2: sa_motion_publish_stop(ctx,0x8c613eb2u); r[3]+=r[13];goto L8c613eb4; /* add r13,r3 */
L8c613eb4: sa_motion_publish_stop(ctx,0x8c613eb4u); r[12]+=r[13];goto L8c613eb6; /* add r13,r12 */
L8c613eb6: sa_motion_publish_stop(ctx,0x8c613eb6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c613eb8; /* pref @r3 */
L8c613eb8: sa_motion_publish_stop(ctx,0x8c613eb8u); r[0]=20u;goto L8c613eba; /* mov #0x14,r0 */
L8c613eba: sa_motion_publish_stop(ctx,0x8c613ebau); fcopy(f,3u,14u);goto L8c613ebc; /* fmov fr14,fr3 */
L8c613ebc: sa_motion_publish_stop(ctx,0x8c613ebcu); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c613ebe; /* fmov.s @(r0,r12),fr2 */
L8c613ebe: sa_motion_publish_stop(ctx,0x8c613ebeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613ec0; /* fsub fr4,fr3 */
L8c613ec0: sa_motion_publish_stop(ctx,0x8c613ec0u); r[0]=16u;goto L8c613ec2; /* mov #0x10,r0 */
L8c613ec2: sa_motion_publish_stop(ctx,0x8c613ec2u); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c613ec4; /* cmp/hs r8,r10 */
L8c613ec4: sa_motion_publish_stop(ctx,0x8c613ec4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613ec6; /* fsub fr13,fr2 */
L8c613ec6: sa_motion_publish_stop(ctx,0x8c613ec6u); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c613ec8; /* fmov.s @(r0,r12),fr1 */
L8c613ec8: sa_motion_publish_stop(ctx,0x8c613ec8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c613eca; /* fsub fr14,fr1 */
L8c613eca: sa_motion_publish_stop(ctx,0x8c613ecau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c613ecc; /* fmul fr2,fr3 */
L8c613ecc: sa_motion_publish_stop(ctx,0x8c613eccu); fcopy(f,2u,13u);goto L8c613ece; /* fmov fr13,fr2 */
L8c613ece: sa_motion_publish_stop(ctx,0x8c613eceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613ed0; /* fsub fr5,fr2 */
L8c613ed0: sa_motion_publish_stop(ctx,0x8c613ed0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613ed2; /* fmul fr1,fr2 */
L8c613ed2: sa_motion_publish_stop(ctx,0x8c613ed2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613ed4; /* fsub fr2,fr3 */
L8c613ed4: sa_motion_publish_stop(ctx,0x8c613ed4u); fcopy(f,4u,3u);goto L8c613ed6; /* fmov fr3,fr4 */
L8c613ed6: sa_motion_publish_stop(ctx,0x8c613ed8u); take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c613f3c;goto L8c613eda; /* bt/s 0x8c613f3c */
L8c613eda: sa_motion_publish_stop(ctx,0x8c613edau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613edc; /* fldi0 fr3 */
L8c613edc: sa_motion_publish_stop(ctx,0x8c613edcu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c613ede; /* fcmp/gt fr4,fr3 */
L8c613ede: sa_motion_publish_stop(ctx,0x8c613edeu); if((*sr&1u))goto L8c613f82;goto L8c613ee0; /* bt 0x8c613f82 */
L8c613ee0: sa_motion_publish_stop(ctx,0x8c613ee0u); r[4]=r[11];goto L8c613ee2; /* mov r11,r4 */
L8c613ee2: sa_motion_publish_stop(ctx,0x8c613ee2u); r[5]=r[9];goto L8c613ee4; /* mov r9,r5 */
L8c613ee4: sa_motion_publish_stop(ctx,0x8c613ee4u); r[8]+=16u;goto L8c613ee6; /* add #0x10,r8 */
L8c613ee6: sa_motion_publish_stop(ctx,0x8c613ee6u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613ee8; /* fschg */
L8c613ee8: sa_motion_publish_stop(ctx,0x8c613ee8u); fcopy(f,0u,1u);goto L8c613eea; /* fmov fr1,fr0 */
L8c613eea: sa_motion_publish_stop(ctx,0x8c613eeau); fcopy(f,2u,3u);goto L8c613eec; /* fmov fr3,fr2 */
L8c613eec: sa_motion_publish_stop(ctx,0x8c613eecu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613eee; /* fmov.s @r4+,fr4 */
L8c613eee: sa_motion_publish_stop(ctx,0x8c613eeeu); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613ef0; /* fmov.s @r4+,fr6 */
L8c613ef0: sa_motion_publish_stop(ctx,0x8c613ef0u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613ef2; /* fldi0 fr3 */
L8c613ef2: sa_motion_publish_stop(ctx,0x8c613ef2u); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c613ef4; /* fipr fv4,fv0 */
L8c613ef4: sa_motion_publish_stop(ctx,0x8c613ef4u); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613ef6; /* fmov.s @r4+,fr8 */
L8c613ef6: sa_motion_publish_stop(ctx,0x8c613ef6u); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613ef8; /* fmov.s @r4+,fr10 */
L8c613ef8: sa_motion_publish_stop(ctx,0x8c613ef8u); fcopy(f,0u,9u);goto L8c613efa; /* fmov fr9,fr0 */
L8c613efa: sa_motion_publish_stop(ctx,0x8c613efau); fcopy(f,4u,5u);goto L8c613efc; /* fmov fr5,fr4 */
L8c613efc: sa_motion_publish_stop(ctx,0x8c613efcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c613efe; /* fsub fr3,fr0 */
L8c613efe: sa_motion_publish_stop(ctx,0x8c613efeu); fcopy(f,6u,7u);goto L8c613f00; /* fmov fr7,fr6 */
L8c613f00: sa_motion_publish_stop(ctx,0x8c613f00u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613f02; /* fschg */
L8c613f02: sa_motion_publish_stop(ctx,0x8c613f02u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c613f04; /* fmov.s fr10,@-r8 */
L8c613f04: sa_motion_publish_stop(ctx,0x8c613f04u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c613f06; /* fmov.s fr0,@-r4 */
L8c613f06: sa_motion_publish_stop(ctx,0x8c613f06u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c613f08; /* ocbwb @r4 */
L8c613f08: sa_motion_publish_stop(ctx,0x8c613f08u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c613f0a; /* fmul fr0,fr7 */
L8c613f0a: sa_motion_publish_stop(ctx,0x8c613f0au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c613f0c; /* fmov.s fr9,@-r8 */
L8c613f0c: sa_motion_publish_stop(ctx,0x8c613f0cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c613f0e; /* fmul fr0,fr6 */
L8c613f0e: sa_motion_publish_stop(ctx,0x8c613f0eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c613f10; /* fmov.s fr8,@-r8 */
L8c613f10: sa_motion_publish_stop(ctx,0x8c613f10u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c613f12; /* fmul fr0,fr5 */
L8c613f12: sa_motion_publish_stop(ctx,0x8c613f12u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613f14; /* mov.l r5,@-r8 */
L8c613f14: sa_motion_publish_stop(ctx,0x8c613f14u); r[8]+=32u;goto L8c613f16; /* add #0x20,r8 */
L8c613f16: sa_motion_publish_stop(ctx,0x8c613f16u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c613f18; /* fmov.s fr7,@-r8 */
L8c613f18: sa_motion_publish_stop(ctx,0x8c613f18u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613f1a; /* fmov.s fr6,@-r8 */
L8c613f1a: sa_motion_publish_stop(ctx,0x8c613f1au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613f1c; /* fmov.s fr5,@-r8 */
L8c613f1c: sa_motion_publish_stop(ctx,0x8c613f1cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613f1e; /* fmov.s fr4,@-r8 */
L8c613f1e: sa_motion_publish_stop(ctx,0x8c613f1eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613f20; /* pref @r8 */
L8c613f20: sa_motion_publish_stop(ctx,0x8c613f20u); r[8]+=16u;goto L8c613f22; /* add #0x10,r8 */
L8c613f22: sa_motion_publish_stop(ctx,0x8c613f22u); r[3]=r[8];goto L8c613f24; /* mov r8,r3 */
L8c613f24: sa_motion_publish_stop(ctx,0x8c613f24u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613f26; /* mov.l @(0x8,r15),r2 */
L8c613f26: sa_motion_publish_stop(ctx,0x8c613f26u); r[3]-=r[10];goto L8c613f28; /* sub r10,r3 */
L8c613f28: sa_motion_publish_stop(ctx,0x8c613f28u); r[3]+=32u;goto L8c613f2a; /* add #0x20,r3 */
L8c613f2a: sa_motion_publish_stop(ctx,0x8c613f2au); r[2]+=r[3];goto L8c613f2c; /* add r3,r2 */
L8c613f2c: sa_motion_publish_stop(ctx,0x8c613f2cu); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613f2e; /* mov.l r2,@(0x8,r15) */
L8c613f2e: sa_motion_publish_stop(ctx,0x8c613f30u); r[10]=r[8];goto L8c613fc4; /* bra 0x8c613fc4 */
L8c613f3c: sa_motion_publish_stop(ctx,0x8c613f3cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613f3e; /* fldi0 fr3 */
L8c613f3e: sa_motion_publish_stop(ctx,0x8c613f3eu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c613f40; /* fcmp/gt fr4,fr3 */
L8c613f40: sa_motion_publish_stop(ctx,0x8c613f40u); if(!(*sr&1u))goto L8c613fc4;goto L8c613f42; /* bf 0x8c613fc4 */
L8c613f42: sa_motion_publish_stop(ctx,0x8c613f42u); r[5]=rd(m,bus,&io_status,0x8c6140b0u,4);if(io_status)return io_status;goto L8c613f44; /* mov.l 0x8c6140b0,r5 */
L8c613f44: sa_motion_publish_stop(ctx,0x8c613f44u); r[8]+=16u;goto L8c613f46; /* add #0x10,r8 */
L8c613f46: sa_motion_publish_stop(ctx,0x8c613f46u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613f48; /* fschg */
L8c613f48: sa_motion_publish_stop(ctx,0x8c613f48u); fcopy(f,0u,1u);goto L8c613f4a; /* fmov fr1,fr0 */
L8c613f4a: sa_motion_publish_stop(ctx,0x8c613f4au); fcopy(f,2u,3u);goto L8c613f4c; /* fmov fr3,fr2 */
L8c613f4c: sa_motion_publish_stop(ctx,0x8c613f4cu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f4e; /* fmov.s @r4+,fr4 */
L8c613f4e: sa_motion_publish_stop(ctx,0x8c613f4eu); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f50; /* fmov.s @r4+,fr6 */
L8c613f50: sa_motion_publish_stop(ctx,0x8c613f50u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613f52; /* fldi0 fr3 */
L8c613f52: sa_motion_publish_stop(ctx,0x8c613f52u); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c613f54; /* fipr fv4,fv0 */
L8c613f54: sa_motion_publish_stop(ctx,0x8c613f54u); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f56; /* fmov.s @r4+,fr8 */
L8c613f56: sa_motion_publish_stop(ctx,0x8c613f56u); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f58; /* fmov.s @r4+,fr10 */
L8c613f58: sa_motion_publish_stop(ctx,0x8c613f58u); fcopy(f,0u,9u);goto L8c613f5a; /* fmov fr9,fr0 */
L8c613f5a: sa_motion_publish_stop(ctx,0x8c613f5au); fcopy(f,4u,5u);goto L8c613f5c; /* fmov fr5,fr4 */
L8c613f5c: sa_motion_publish_stop(ctx,0x8c613f5cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c613f5e; /* fsub fr3,fr0 */
L8c613f5e: sa_motion_publish_stop(ctx,0x8c613f5eu); fcopy(f,6u,7u);goto L8c613f60; /* fmov fr7,fr6 */
L8c613f60: sa_motion_publish_stop(ctx,0x8c613f60u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613f62; /* fschg */
L8c613f62: sa_motion_publish_stop(ctx,0x8c613f62u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c613f64; /* fmov.s fr10,@-r8 */
L8c613f64: sa_motion_publish_stop(ctx,0x8c613f64u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c613f66; /* fmov.s fr0,@-r4 */
L8c613f66: sa_motion_publish_stop(ctx,0x8c613f66u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c613f68; /* ocbwb @r4 */
L8c613f68: sa_motion_publish_stop(ctx,0x8c613f68u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c613f6a; /* fmul fr0,fr7 */
L8c613f6a: sa_motion_publish_stop(ctx,0x8c613f6au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c613f6c; /* fmov.s fr9,@-r8 */
L8c613f6c: sa_motion_publish_stop(ctx,0x8c613f6cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c613f6e; /* fmul fr0,fr6 */
L8c613f6e: sa_motion_publish_stop(ctx,0x8c613f6eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c613f70; /* fmov.s fr8,@-r8 */
L8c613f70: sa_motion_publish_stop(ctx,0x8c613f70u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c613f72; /* fmul fr0,fr5 */
L8c613f72: sa_motion_publish_stop(ctx,0x8c613f72u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613f74; /* mov.l r5,@-r8 */
L8c613f74: sa_motion_publish_stop(ctx,0x8c613f74u); r[8]+=32u;goto L8c613f76; /* add #0x20,r8 */
L8c613f76: sa_motion_publish_stop(ctx,0x8c613f76u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c613f78; /* fmov.s fr7,@-r8 */
L8c613f78: sa_motion_publish_stop(ctx,0x8c613f78u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613f7a; /* fmov.s fr6,@-r8 */
L8c613f7a: sa_motion_publish_stop(ctx,0x8c613f7au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613f7c; /* fmov.s fr5,@-r8 */
L8c613f7c: sa_motion_publish_stop(ctx,0x8c613f7cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613f7e; /* fmov.s fr4,@-r8 */
L8c613f7e: sa_motion_publish_stop(ctx,0x8c613f7eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613f80; /* pref @r8 */
L8c613f80: sa_motion_publish_stop(ctx,0x8c613f80u); r[8]+=16u;goto L8c613f82; /* add #0x10,r8 */
L8c613f82: sa_motion_publish_stop(ctx,0x8c613f82u); r[5]=rd(m,bus,&io_status,0x8c6140b0u,4);if(io_status)return io_status;goto L8c613f84; /* mov.l 0x8c6140b0,r5 */
L8c613f84: sa_motion_publish_stop(ctx,0x8c613f84u); r[4]=r[11];goto L8c613f86; /* mov r11,r4 */
L8c613f86: sa_motion_publish_stop(ctx,0x8c613f86u); r[8]+=16u;goto L8c613f88; /* add #0x10,r8 */
L8c613f88: sa_motion_publish_stop(ctx,0x8c613f88u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613f8a; /* fschg */
L8c613f8a: sa_motion_publish_stop(ctx,0x8c613f8au); fcopy(f,0u,1u);goto L8c613f8c; /* fmov fr1,fr0 */
L8c613f8c: sa_motion_publish_stop(ctx,0x8c613f8cu); fcopy(f,2u,3u);goto L8c613f8e; /* fmov fr3,fr2 */
L8c613f8e: sa_motion_publish_stop(ctx,0x8c613f8eu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f90; /* fmov.s @r4+,fr4 */
L8c613f90: sa_motion_publish_stop(ctx,0x8c613f90u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f92; /* fmov.s @r4+,fr6 */
L8c613f92: sa_motion_publish_stop(ctx,0x8c613f92u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613f94; /* fldi0 fr3 */
L8c613f94: sa_motion_publish_stop(ctx,0x8c613f94u); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c613f96; /* fipr fv4,fv0 */
L8c613f96: sa_motion_publish_stop(ctx,0x8c613f96u); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f98; /* fmov.s @r4+,fr8 */
L8c613f98: sa_motion_publish_stop(ctx,0x8c613f98u); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f9a; /* fmov.s @r4+,fr10 */
L8c613f9a: sa_motion_publish_stop(ctx,0x8c613f9au); fcopy(f,0u,9u);goto L8c613f9c; /* fmov fr9,fr0 */
L8c613f9c: sa_motion_publish_stop(ctx,0x8c613f9cu); fcopy(f,4u,5u);goto L8c613f9e; /* fmov fr5,fr4 */
L8c613f9e: sa_motion_publish_stop(ctx,0x8c613f9eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c613fa0; /* fsub fr3,fr0 */
L8c613fa0: sa_motion_publish_stop(ctx,0x8c613fa0u); fcopy(f,6u,7u);goto L8c613fa2; /* fmov fr7,fr6 */
L8c613fa2: sa_motion_publish_stop(ctx,0x8c613fa2u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613fa4; /* fschg */
L8c613fa4: sa_motion_publish_stop(ctx,0x8c613fa4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c613fa6; /* fmov.s fr10,@-r8 */
L8c613fa6: sa_motion_publish_stop(ctx,0x8c613fa6u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c613fa8; /* fmov.s fr0,@-r4 */
L8c613fa8: sa_motion_publish_stop(ctx,0x8c613fa8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c613faa; /* ocbwb @r4 */
L8c613faa: sa_motion_publish_stop(ctx,0x8c613faau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c613fac; /* fmul fr0,fr7 */
L8c613fac: sa_motion_publish_stop(ctx,0x8c613facu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c613fae; /* fmov.s fr9,@-r8 */
L8c613fae: sa_motion_publish_stop(ctx,0x8c613faeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c613fb0; /* fmul fr0,fr6 */
L8c613fb0: sa_motion_publish_stop(ctx,0x8c613fb0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c613fb2; /* fmov.s fr8,@-r8 */
L8c613fb2: sa_motion_publish_stop(ctx,0x8c613fb2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c613fb4; /* fmul fr0,fr5 */
L8c613fb4: sa_motion_publish_stop(ctx,0x8c613fb4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613fb6; /* mov.l r5,@-r8 */
L8c613fb6: sa_motion_publish_stop(ctx,0x8c613fb6u); r[8]+=32u;goto L8c613fb8; /* add #0x20,r8 */
L8c613fb8: sa_motion_publish_stop(ctx,0x8c613fb8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c613fba; /* fmov.s fr7,@-r8 */
L8c613fba: sa_motion_publish_stop(ctx,0x8c613fbau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613fbc; /* fmov.s fr6,@-r8 */
L8c613fbc: sa_motion_publish_stop(ctx,0x8c613fbcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613fbe; /* fmov.s fr5,@-r8 */
L8c613fbe: sa_motion_publish_stop(ctx,0x8c613fbeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613fc0; /* fmov.s fr4,@-r8 */
L8c613fc0: sa_motion_publish_stop(ctx,0x8c613fc0u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613fc2; /* pref @r8 */
L8c613fc2: sa_motion_publish_stop(ctx,0x8c613fc2u); r[8]+=16u;goto L8c613fc4; /* add #0x10,r8 */
L8c613fc4: sa_motion_publish_stop(ctx,0x8c613fc4u); fcopy(f,4u,14u);goto L8c613fc6; /* fmov fr14,fr4 */
L8c613fc6: sa_motion_publish_stop(ctx,0x8c613fc6u); r[0]=16u;goto L8c613fc8; /* mov #0x10,r0 */
L8c613fc8: sa_motion_publish_stop(ctx,0x8c613fc8u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613fca; /* mov.l @r15,r3 */
L8c613fca: sa_motion_publish_stop(ctx,0x8c613fcau); r[4]=r[11];goto L8c613fcc; /* mov r11,r4 */
L8c613fcc: sa_motion_publish_stop(ctx,0x8c613fccu); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c613fce; /* fmov.s @(r0,r12),fr14 */
L8c613fce: sa_motion_publish_stop(ctx,0x8c613fceu); r[0]=20u;goto L8c613fd0; /* mov #0x14,r0 */
L8c613fd0: sa_motion_publish_stop(ctx,0x8c613fd0u); fcopy(f,5u,13u);goto L8c613fd2; /* fmov fr13,fr5 */
L8c613fd2: sa_motion_publish_stop(ctx,0x8c613fd2u); r[3]+=4294967295u;goto L8c613fd4; /* add #-0x1,r3 */
L8c613fd4: sa_motion_publish_stop(ctx,0x8c613fd4u); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c613fd6; /* fmov.s @(r0,r12),fr13 */
L8c613fd6: sa_motion_publish_stop(ctx,0x8c613fd6u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c613fd8; /* tst r3,r3 */
L8c613fd8: sa_motion_publish_stop(ctx,0x8c613fd8u); r[11]=r[12];goto L8c613fda; /* mov r12,r11 */
L8c613fda: sa_motion_publish_stop(ctx,0x8c613fdau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c613fdc; /* fneg fr15 */
L8c613fdc: sa_motion_publish_stop(ctx,0x8c613fdeu); take=(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c613fe4;goto L8c613fe0; /* bt/s 0x8c613fe4 */
L8c613fe0: sa_motion_publish_stop(ctx,0x8c613fe2u); goto L8c613e9e; /* bra 0x8c613e9e */
L8c613fe4: sa_motion_publish_stop(ctx,0x8c613fe4u); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c613fe6; /* cmp/hs r8,r10 */
L8c613fe6: sa_motion_publish_stop(ctx,0x8c613fe6u); if((*sr&1u))goto L8c614036;goto L8c613fe8; /* bt 0x8c614036 */
L8c613fe8: sa_motion_publish_stop(ctx,0x8c613fe8u); r[4]=r[12];goto L8c613fea; /* mov r12,r4 */
L8c613fea: sa_motion_publish_stop(ctx,0x8c613feau); r[5]=r[9];goto L8c613fec; /* mov r9,r5 */
L8c613fec: sa_motion_publish_stop(ctx,0x8c613fecu); r[8]+=16u;goto L8c613fee; /* add #0x10,r8 */
L8c613fee: sa_motion_publish_stop(ctx,0x8c613feeu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613ff0; /* fschg */
L8c613ff0: sa_motion_publish_stop(ctx,0x8c613ff0u); fcopy(f,0u,1u);goto L8c613ff2; /* fmov fr1,fr0 */
L8c613ff2: sa_motion_publish_stop(ctx,0x8c613ff2u); fcopy(f,2u,3u);goto L8c613ff4; /* fmov fr3,fr2 */
L8c613ff4: sa_motion_publish_stop(ctx,0x8c613ff4u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613ff6; /* fmov.s @r4+,fr4 */
L8c613ff6: sa_motion_publish_stop(ctx,0x8c613ff6u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613ff8; /* fmov.s @r4+,fr6 */
L8c613ff8: sa_motion_publish_stop(ctx,0x8c613ff8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613ffa; /* fldi0 fr3 */
L8c613ffa: sa_motion_publish_stop(ctx,0x8c613ffau); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c613ffc; /* fipr fv4,fv0 */
L8c613ffc: sa_motion_publish_stop(ctx,0x8c613ffcu); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613ffe; /* fmov.s @r4+,fr8 */
L8c613ffe: sa_motion_publish_stop(ctx,0x8c613ffeu); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614000; /* fmov.s @r4+,fr10 */
L8c614000: sa_motion_publish_stop(ctx,0x8c614000u); fcopy(f,0u,9u);goto L8c614002; /* fmov fr9,fr0 */
L8c614002: sa_motion_publish_stop(ctx,0x8c614002u); fcopy(f,4u,5u);goto L8c614004; /* fmov fr5,fr4 */
L8c614004: sa_motion_publish_stop(ctx,0x8c614004u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c614006; /* fsub fr3,fr0 */
L8c614006: sa_motion_publish_stop(ctx,0x8c614006u); fcopy(f,6u,7u);goto L8c614008; /* fmov fr7,fr6 */
L8c614008: sa_motion_publish_stop(ctx,0x8c614008u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61400a; /* fschg */
L8c61400a: sa_motion_publish_stop(ctx,0x8c61400au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c61400c; /* fmov.s fr10,@-r8 */
L8c61400c: sa_motion_publish_stop(ctx,0x8c61400cu); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61400e; /* fmov.s fr0,@-r4 */
L8c61400e: sa_motion_publish_stop(ctx,0x8c61400eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c614010; /* ocbwb @r4 */
L8c614010: sa_motion_publish_stop(ctx,0x8c614010u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c614012; /* fmul fr0,fr7 */
L8c614012: sa_motion_publish_stop(ctx,0x8c614012u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c614014; /* fmov.s fr9,@-r8 */
L8c614014: sa_motion_publish_stop(ctx,0x8c614014u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c614016; /* fmul fr0,fr6 */
L8c614016: sa_motion_publish_stop(ctx,0x8c614016u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c614018; /* fmov.s fr8,@-r8 */
L8c614018: sa_motion_publish_stop(ctx,0x8c614018u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61401a; /* fmul fr0,fr5 */
L8c61401a: sa_motion_publish_stop(ctx,0x8c61401au); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61401c; /* mov.l r5,@-r8 */
L8c61401c: sa_motion_publish_stop(ctx,0x8c61401cu); r[8]+=32u;goto L8c61401e; /* add #0x20,r8 */
L8c61401e: sa_motion_publish_stop(ctx,0x8c61401eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c614020; /* fmov.s fr7,@-r8 */
L8c614020: sa_motion_publish_stop(ctx,0x8c614020u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614022; /* fmov.s fr6,@-r8 */
L8c614022: sa_motion_publish_stop(ctx,0x8c614022u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614024; /* fmov.s fr5,@-r8 */
L8c614024: sa_motion_publish_stop(ctx,0x8c614024u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614026; /* fmov.s fr4,@-r8 */
L8c614026: sa_motion_publish_stop(ctx,0x8c614026u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614028; /* pref @r8 */
L8c614028: sa_motion_publish_stop(ctx,0x8c614028u); r[8]+=16u;goto L8c61402a; /* add #0x10,r8 */
L8c61402a: sa_motion_publish_stop(ctx,0x8c61402au); r[3]=r[8];goto L8c61402c; /* mov r8,r3 */
L8c61402c: sa_motion_publish_stop(ctx,0x8c61402cu); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61402e; /* mov.l @(0x8,r15),r2 */
L8c61402e: sa_motion_publish_stop(ctx,0x8c61402eu); r[3]-=r[10];goto L8c614030; /* sub r10,r3 */
L8c614030: sa_motion_publish_stop(ctx,0x8c614030u); r[3]+=32u;goto L8c614032; /* add #0x20,r3 */
L8c614032: sa_motion_publish_stop(ctx,0x8c614032u); r[2]+=r[3];goto L8c614034; /* add r3,r2 */
L8c614034: sa_motion_publish_stop(ctx,0x8c614034u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c614036; /* mov.l r2,@(0x8,r15) */
L8c614036: sa_motion_publish_stop(ctx,0x8c614036u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614038; /* mov.l @(0xc,r15),r3 */
L8c614038: sa_motion_publish_stop(ctx,0x8c614038u); r[3]+=4294967295u;goto L8c61403a; /* add #-0x1,r3 */
L8c61403a: sa_motion_publish_stop(ctx,0x8c61403au); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c61403c; /* tst r3,r3 */
L8c61403c: sa_motion_publish_stop(ctx,0x8c61403eu); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c614044;goto L8c614040; /* bt/s 0x8c614044 */
L8c614040: sa_motion_publish_stop(ctx,0x8c614042u); goto L8c613e4c; /* bra 0x8c613e4c */
L8c614044: sa_motion_publish_stop(ctx,0x8c614044u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614046; /* mov.l @(0x8,r15),r3 */
L8c614046: sa_motion_publish_stop(ctx,0x8c614046u); r[4]=rd(m,bus,&io_status,0x8c6140b4u,4);if(io_status)return io_status;goto L8c614048; /* mov.l 0x8c6140b4,r4 */
L8c614048: sa_motion_publish_stop(ctx,0x8c614048u); r[3]>>=2;goto L8c61404a; /* shlr2 r3 */
L8c61404a: sa_motion_publish_stop(ctx,0x8c61404au); r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61404c; /* mov.l @r4,r1 */
L8c61404c: sa_motion_publish_stop(ctx,0x8c61404cu); r[3]>>=2;goto L8c61404e; /* shlr2 r3 */
L8c61404e: sa_motion_publish_stop(ctx,0x8c61404eu); *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c614050; /* shlr r3 */
L8c614050: sa_motion_publish_stop(ctx,0x8c614050u); r[1]+=r[3];goto L8c614052; /* add r3,r1 */
L8c614052: sa_motion_publish_stop(ctx,0x8c614052u); wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c614054; /* mov.l r1,@r4 */
L8c614054: sa_motion_publish_stop(ctx,0x8c614054u); r[4]=rd(m,bus,&io_status,0x8c6140b8u,4);if(io_status)return io_status;goto L8c614056; /* mov.l 0x8c6140b8,r4 */
L8c614056: sa_motion_publish_stop(ctx,0x8c614056u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614058; /* mov.l @(0x4,r15),r3 */
L8c614058: sa_motion_publish_stop(ctx,0x8c614058u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61405a; /* mov.l @r4,r2 */
L8c61405a: sa_motion_publish_stop(ctx,0x8c61405au); r[2]+=r[3];goto L8c61405c; /* add r3,r2 */
L8c61405c: sa_motion_publish_stop(ctx,0x8c61405cu); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c61405e; /* mov.l r2,@r4 */
L8c61405e: sa_motion_publish_stop(ctx,0x8c61405eu); r[15]+=16u;goto L8c614060; /* add #0x10,r15 */
L8c614060: sa_motion_publish_stop(ctx,0x8c614060u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614062; /* lds.l @r15+,PR */
L8c614062: sa_motion_publish_stop(ctx,0x8c614062u); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614064; /* fmov.s @r15+,fr13 */
L8c614064: sa_motion_publish_stop(ctx,0x8c614064u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614066; /* fmov.s @r15+,fr14 */
L8c614066: sa_motion_publish_stop(ctx,0x8c614066u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614068; /* fmov.s @r15+,fr15 */
L8c614068: sa_motion_publish_stop(ctx,0x8c614068u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61406a; /* mov.l @r15+,r9 */
L8c61406a: sa_motion_publish_stop(ctx,0x8c61406au); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61406c; /* mov.l @r15+,r10 */
L8c61406c: sa_motion_publish_stop(ctx,0x8c61406cu); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61406e; /* mov.l @r15+,r11 */
L8c61406e: sa_motion_publish_stop(ctx,0x8c61406eu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614070; /* mov.l @r15+,r12 */
L8c614070: sa_motion_publish_stop(ctx,0x8c614070u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614072; /* mov.l @r15+,r13 */
L8c614072: sa_motion_publish_stop(ctx,0x8c614074u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_face_alternate_strip_clipped_8c61572c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61572c;
L8c61572c: sa_motion_publish_stop(ctx,0x8c61572cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61572e; /* mov.l r14,@-r15 */
L8c61572e: sa_motion_publish_stop(ctx,0x8c61572eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c615730; /* mov.l r13,@-r15 */
L8c615730: sa_motion_publish_stop(ctx,0x8c615730u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c615732; /* mov.l r12,@-r15 */
L8c615732: sa_motion_publish_stop(ctx,0x8c615732u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c615734; /* mov.l r11,@-r15 */
L8c615734: sa_motion_publish_stop(ctx,0x8c615734u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c615736; /* mov.l r10,@-r15 */
L8c615736: sa_motion_publish_stop(ctx,0x8c615736u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c615738; /* mov.l r9,@-r15 */
L8c615738: sa_motion_publish_stop(ctx,0x8c615738u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61573a; /* fmov.s fr15,@-r15 */
L8c61573a: sa_motion_publish_stop(ctx,0x8c61573au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61573c; /* fmov.s fr14,@-r15 */
L8c61573c: sa_motion_publish_stop(ctx,0x8c61573cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61573e; /* fmov.s fr13,@-r15 */
L8c61573e: sa_motion_publish_stop(ctx,0x8c61573eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c615740; /* fmov.s fr12,@-r15 */
L8c615740: sa_motion_publish_stop(ctx,0x8c615740u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c615742; /* sts.l PR,@-r15 */
L8c615742: sa_motion_publish_stop(ctx,0x8c615742u); r[15]+=4294967220u;goto L8c615744; /* add #-0x4c,r15 */
L8c615744: sa_motion_publish_stop(ctx,0x8c615744u); r[3]=rd(m,bus,&io_status,0x8c61577cu,4);if(io_status)return io_status;goto L8c615746; /* mov.l 0x8c61577c,r3 */
L8c615746: sa_motion_publish_stop(ctx,0x8c615746u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c615748; /* fldi1 fr12 */
L8c615748: sa_motion_publish_stop(ctx,0x8c61574au); target=r[3];*pr=0x8c61574cu;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61574c; /* jsr @r3 */
L8c61574c: sa_motion_publish_stop(ctx,0x8c61574cu); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61574e; /* mov.l @(0x8,r15),r3 */
L8c61574e: sa_motion_publish_stop(ctx,0x8c61574eu); r[2]=0u;goto L8c615750; /* mov #0x0,r2 */
L8c615750: sa_motion_publish_stop(ctx,0x8c615750u); r[14]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615752; /* mov.l @(0x8,r15),r14 */
L8c615752: sa_motion_publish_stop(ctx,0x8c615752u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c615754; /* mov.w @(0x2,r3),r0 */
L8c615754: sa_motion_publish_stop(ctx,0x8c615754u); r[3]=rd(m,bus,&io_status,0x8c615780u,4);if(io_status)return io_status;goto L8c615756; /* mov.l 0x8c615780,r3 */
L8c615756: sa_motion_publish_stop(ctx,0x8c615756u); r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c615758; /* mov.l @(0x4,r14),r14 */
L8c615758: sa_motion_publish_stop(ctx,0x8c615758u); r[0]=r[0]&65535u;goto L8c61575a; /* extu.w r0,r0 */
L8c61575a: sa_motion_publish_stop(ctx,0x8c61575au); wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c61575c; /* mov.l r0,@(0x18,r15) */
L8c61575c: sa_motion_publish_stop(ctx,0x8c61575cu); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61575e; /* mov.l @r3,r13 */
L8c61575e: sa_motion_publish_stop(ctx,0x8c61575eu); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c615760; /* mov.l r2,@(0x8,r15) */
L8c615760: sa_motion_publish_stop(ctx,0x8c615762u); goto L8c615784; /* bra 0x8c615784 */
L8c615784: sa_motion_publish_stop(ctx,0x8c615784u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c615786; /* mov.w @r14+,r3 */
L8c615786: sa_motion_publish_stop(ctx,0x8c615786u); r[4]=rd(m,bus,&io_status,0x8c6158a0u,4);if(io_status)return io_status;goto L8c615788; /* mov.l 0x8c6158a0,r4 */
L8c615788: sa_motion_publish_stop(ctx,0x8c615788u); r[3]=r[3]&65535u;goto L8c61578a; /* extu.w r3,r3 */
L8c61578a: sa_motion_publish_stop(ctx,0x8c61578au); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c61578c; /* tst r4,r3 */
L8c61578c: sa_motion_publish_stop(ctx,0x8c61578cu); wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c61578e; /* mov.l r3,@(0xc,r15) */
L8c61578e: sa_motion_publish_stop(ctx,0x8c615790u); take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c6157a0;goto L8c615792; /* bt/s 0x8c6157a0 */
L8c615792: sa_motion_publish_stop(ctx,0x8c615792u); r[4]=sx(m,bus,&io_status,0x8c61589cu,2);if(io_status)return io_status;goto L8c615794; /* mov.w 0x8c61589c,r4 */
L8c615794: sa_motion_publish_stop(ctx,0x8c615794u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c615796; /* mov.l @(0xc,r15),r3 */
L8c615796: sa_motion_publish_stop(ctx,0x8c615796u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c615798; /* fldi1 fr4 */
L8c615798: sa_motion_publish_stop(ctx,0x8c615798u); r[3]&=r[4];goto L8c61579a; /* and r4,r3 */
L8c61579a: sa_motion_publish_stop(ctx,0x8c61579au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c61579c; /* fneg fr4 */
L8c61579c: sa_motion_publish_stop(ctx,0x8c61579cu); wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c61579e; /* mov.l r3,@(0xc,r15) */
L8c61579e: sa_motion_publish_stop(ctx,0x8c61579eu); fcopy(f,15u,4u);goto L8c6157a0; /* fmov fr4,fr15 */
L8c6157a0: sa_motion_publish_stop(ctx,0x8c6157a0u); r[0]=16u;goto L8c6157a2; /* mov #0x10,r0 */
L8c6157a2: sa_motion_publish_stop(ctx,0x8c6157a2u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6157a4; /* mov.l @(0xc,r15),r3 */
L8c6157a4: sa_motion_publish_stop(ctx,0x8c6157a4u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6157a6; /* mov.l @(0x8,r15),r2 */
L8c6157a6: sa_motion_publish_stop(ctx,0x8c6157a6u); r[9]=r[8];goto L8c6157a8; /* mov r8,r9 */
L8c6157a8: sa_motion_publish_stop(ctx,0x8c6157a8u); r[2]+=r[3];goto L8c6157aa; /* add r3,r2 */
L8c6157aa: sa_motion_publish_stop(ctx,0x8c6157aau); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6157ac; /* mov.l r2,@(0x8,r15) */
L8c6157ac: sa_motion_publish_stop(ctx,0x8c6157acu); r[1]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6157ae; /* mov.l @(0xc,r15),r1 */
L8c6157ae: sa_motion_publish_stop(ctx,0x8c6157aeu); r[1]+=4294967294u;goto L8c6157b0; /* add #-0x2,r1 */
L8c6157b0: sa_motion_publish_stop(ctx,0x8c6157b0u); wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c6157b2; /* mov.l r1,@(0xc,r15) */
L8c6157b2: sa_motion_publish_stop(ctx,0x8c6157b2u); r[10]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6157b4; /* mov.w @r14+,r10 */
L8c6157b4: sa_motion_publish_stop(ctx,0x8c6157b4u); r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6157b6; /* mov.w @r14+,r11 */
L8c6157b6: sa_motion_publish_stop(ctx,0x8c6157b6u); r[10]=r[10]&65535u;goto L8c6157b8; /* extu.w r10,r10 */
L8c6157b8: sa_motion_publish_stop(ctx,0x8c6157b8u); r[10]<<=2;goto L8c6157ba; /* shll2 r10 */
L8c6157ba: sa_motion_publish_stop(ctx,0x8c6157bau); r[11]=r[11]&65535u;goto L8c6157bc; /* extu.w r11,r11 */
L8c6157bc: sa_motion_publish_stop(ctx,0x8c6157bcu); r[11]<<=2;goto L8c6157be; /* shll2 r11 */
L8c6157be: sa_motion_publish_stop(ctx,0x8c6157beu); r[10]<<=2;goto L8c6157c0; /* shll2 r10 */
L8c6157c0: sa_motion_publish_stop(ctx,0x8c6157c0u); r[11]<<=2;goto L8c6157c2; /* shll2 r11 */
L8c6157c2: sa_motion_publish_stop(ctx,0x8c6157c2u); *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c6157c4; /* shll r10 */
L8c6157c4: sa_motion_publish_stop(ctx,0x8c6157c4u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c6157c6; /* shll r11 */
L8c6157c6: sa_motion_publish_stop(ctx,0x8c6157c6u); r[11]+=r[13];goto L8c6157c8; /* add r13,r11 */
L8c6157c8: sa_motion_publish_stop(ctx,0x8c6157c8u); r[10]+=r[13];goto L8c6157ca; /* add r13,r10 */
L8c6157ca: sa_motion_publish_stop(ctx,0x8c6157cau); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c6157cc; /* fmov.s @(r0,r11),fr13 */
L8c6157cc: sa_motion_publish_stop(ctx,0x8c6157ccu); fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c6157ce; /* fmov.s @(r0,r10),fr6 */
L8c6157ce: sa_motion_publish_stop(ctx,0x8c6157ceu); r[0]=20u;goto L8c6157d0; /* mov #0x14,r0 */
L8c6157d0: sa_motion_publish_stop(ctx,0x8c6157d0u); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c6157d2; /* fmov.s @(r0,r11),fr14 */
L8c6157d2: sa_motion_publish_stop(ctx,0x8c6157d2u); fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c6157d4; /* fmov.s @(r0,r10),fr5 */
L8c6157d4: sa_motion_publish_stop(ctx,0x8c6157d4u); r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6157d6; /* mov.w @r14+,r12 */
L8c6157d6: sa_motion_publish_stop(ctx,0x8c6157d6u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c6157d8; /* mov.w @r14,r3 */
L8c6157d8: sa_motion_publish_stop(ctx,0x8c6157d8u); r[12]=r[12]&65535u;goto L8c6157da; /* extu.w r12,r12 */
L8c6157da: sa_motion_publish_stop(ctx,0x8c6157dau); r[12]<<=2;goto L8c6157dc; /* shll2 r12 */
L8c6157dc: sa_motion_publish_stop(ctx,0x8c6157dcu); r[3]=r[3]&65535u;goto L8c6157de; /* extu.w r3,r3 */
L8c6157de: sa_motion_publish_stop(ctx,0x8c6157deu); r[3]<<=2;goto L8c6157e0; /* shll2 r3 */
L8c6157e0: sa_motion_publish_stop(ctx,0x8c6157e0u); r[12]<<=2;goto L8c6157e2; /* shll2 r12 */
L8c6157e2: sa_motion_publish_stop(ctx,0x8c6157e2u); r[3]<<=2;goto L8c6157e4; /* shll2 r3 */
L8c6157e4: sa_motion_publish_stop(ctx,0x8c6157e4u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c6157e6; /* shll r12 */
L8c6157e6: sa_motion_publish_stop(ctx,0x8c6157e6u); *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c6157e8; /* shll r3 */
L8c6157e8: sa_motion_publish_stop(ctx,0x8c6157e8u); r[3]+=r[13];goto L8c6157ea; /* add r13,r3 */
L8c6157ea: sa_motion_publish_stop(ctx,0x8c6157eau); r[12]+=r[13];goto L8c6157ec; /* add r13,r12 */
L8c6157ec: sa_motion_publish_stop(ctx,0x8c6157ecu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c6157ee; /* pref @r3 */
L8c6157ee: sa_motion_publish_stop(ctx,0x8c6157eeu); r[2]=0u;goto L8c6157f0; /* mov #0x0,r2 */
L8c6157f0: sa_motion_publish_stop(ctx,0x8c6157f0u); wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c6157f2; /* mov.l r2,@(0x14,r15) */
L8c6157f2: sa_motion_publish_stop(ctx,0x8c6157f2u); r[4]=63u;goto L8c6157f4; /* mov #0x3f,r4 */
L8c6157f4: sa_motion_publish_stop(ctx,0x8c6157f4u); r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c6157f6; /* mov.l @(0xc,r10),r2 */
L8c6157f6: sa_motion_publish_stop(ctx,0x8c6157f6u); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c6157f8; /* mov.l @(0xc,r11),r3 */
L8c6157f8: sa_motion_publish_stop(ctx,0x8c6157f8u); r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c6157fa; /* mov.l @(0xc,r12),r1 */
L8c6157fa: sa_motion_publish_stop(ctx,0x8c6157fau); r[2]&=r[3];goto L8c6157fc; /* and r3,r2 */
L8c6157fc: sa_motion_publish_stop(ctx,0x8c6157fcu); r[2]&=r[1];goto L8c6157fe; /* and r1,r2 */
L8c6157fe: sa_motion_publish_stop(ctx,0x8c6157feu); r[4]&=r[2];goto L8c615800; /* and r2,r4 */
L8c615800: sa_motion_publish_stop(ctx,0x8c615800u); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c615802; /* tst r4,r4 */
L8c615802: sa_motion_publish_stop(ctx,0x8c615804u); take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c61580a;goto L8c615806; /* bt/s 0x8c61580a */
L8c615806: sa_motion_publish_stop(ctx,0x8c615808u); goto L8c615964; /* bra 0x8c615964 */
L8c61580a: sa_motion_publish_stop(ctx,0x8c61580au); r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c61580c; /* mov.l @(0xc,r10),r4 */
L8c61580c: sa_motion_publish_stop(ctx,0x8c61580cu); r[5]=1u;goto L8c61580e; /* mov #0x1,r5 */
L8c61580e: sa_motion_publish_stop(ctx,0x8c61580eu); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c615810; /* mov.l @(0xc,r11),r3 */
L8c615810: sa_motion_publish_stop(ctx,0x8c615810u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c615812; /* mov.l @(0xc,r12),r2 */
L8c615812: sa_motion_publish_stop(ctx,0x8c615812u); r[4]|=r[3];goto L8c615814; /* or r3,r4 */
L8c615814: sa_motion_publish_stop(ctx,0x8c615814u); r[4]|=r[2];goto L8c615816; /* or r2,r4 */
L8c615816: sa_motion_publish_stop(ctx,0x8c615816u); r[4]&=r[5];goto L8c615818; /* and r5,r4 */
L8c615818: sa_motion_publish_stop(ctx,0x8c615818u); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c61581a; /* tst r4,r4 */
L8c61581a: sa_motion_publish_stop(ctx,0x8c61581au); if(!(*sr&1u))goto L8c615820;goto L8c61581c; /* bf 0x8c615820 */
L8c61581c: sa_motion_publish_stop(ctx,0x8c61581eu); goto L8c615946; /* bra 0x8c615946 */
L8c615820: sa_motion_publish_stop(ctx,0x8c615820u); r[7]=r[15];goto L8c615822; /* mov r15,r7 */
L8c615822: sa_motion_publish_stop(ctx,0x8c615822u); r[7]+=64u;goto L8c615824; /* add #0x40,r7 */
L8c615824: sa_motion_publish_stop(ctx,0x8c615824u); wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c615826; /* mov.l r10,@r7 */
L8c615826: sa_motion_publish_stop(ctx,0x8c615826u); wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c615828; /* mov.l r11,@(0x4,r7) */
L8c615828: sa_motion_publish_stop(ctx,0x8c615828u); wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c61582a; /* mov.l r12,@(0x8,r7) */
L8c61582a: sa_motion_publish_stop(ctx,0x8c61582cu); r[6]=0u;goto L8c6158d8; /* bra 0x8c6158d8 */
L8c61582e: sa_motion_publish_stop(ctx,0x8c61582eu); r[2]=r[6];goto L8c615830; /* mov r6,r2 */
L8c615830: sa_motion_publish_stop(ctx,0x8c615830u); r[1]=r[6];goto L8c615832; /* mov r6,r1 */
L8c615832: sa_motion_publish_stop(ctx,0x8c615832u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c615834; /* shll r2 */
L8c615834: sa_motion_publish_stop(ctx,0x8c615834u); r[3]=r[6];goto L8c615836; /* mov r6,r3 */
L8c615836: sa_motion_publish_stop(ctx,0x8c615836u); r[3]<<=2;goto L8c615838; /* shll2 r3 */
L8c615838: sa_motion_publish_stop(ctx,0x8c615838u); r[2]+=r[1];goto L8c61583a; /* add r1,r2 */
L8c61583a: sa_motion_publish_stop(ctx,0x8c61583au); r[2]<<=2;goto L8c61583c; /* shll2 r2 */
L8c61583c: sa_motion_publish_stop(ctx,0x8c61583cu); r[3]+=r[7];goto L8c61583e; /* add r7,r3 */
L8c61583e: sa_motion_publish_stop(ctx,0x8c61583eu); r[2]+=r[4];goto L8c615840; /* add r4,r2 */
L8c615840: sa_motion_publish_stop(ctx,0x8c615840u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c615842; /* mov.l r3,@(0x4,r15) */
L8c615842: sa_motion_publish_stop(ctx,0x8c615842u); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c615844; /* mov.l r2,@r15 */
L8c615844: sa_motion_publish_stop(ctx,0x8c615844u); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c615846; /* mov.l @r3,r1 */
L8c615846: sa_motion_publish_stop(ctx,0x8c615846u); r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c615848; /* mov.l @(0xc,r1),r2 */
L8c615848: sa_motion_publish_stop(ctx,0x8c615848u); *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c61584a; /* tst r5,r2 */
L8c61584a: sa_motion_publish_stop(ctx,0x8c61584cu); take=!(*sr&1u);r[6]+=1u;if(take)goto L8c6158b0;goto L8c61584e; /* bf/s 0x8c6158b0 */
L8c61584e: sa_motion_publish_stop(ctx,0x8c61584eu); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615850; /* mov.l @(0x4,r15),r1 */
L8c615850: sa_motion_publish_stop(ctx,0x8c615850u); r[0]=24u;goto L8c615852; /* mov #0x18,r0 */
L8c615852: sa_motion_publish_stop(ctx,0x8c615852u); fcopy(f,4u,12u);goto L8c615854; /* fmov fr12,fr4 */
L8c615854: sa_motion_publish_stop(ctx,0x8c615854u); r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c615856; /* mov.l @r1,r3 */
L8c615856: sa_motion_publish_stop(ctx,0x8c615856u); r[4]=rd(m,bus,&io_status,0x8c6158a4u,4);if(io_status)return io_status;goto L8c615858; /* mov.l 0x8c6158a4,r4 */
L8c615858: sa_motion_publish_stop(ctx,0x8c615858u); wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c61585a; /* mov.l r3,@(0x10,r15) */
L8c61585a: sa_motion_publish_stop(ctx,0x8c61585au); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61585c; /* fmov.s @(r0,r3),fr3 */
L8c61585c: sa_motion_publish_stop(ctx,0x8c61585cu); r[0]=12u;goto L8c61585e; /* mov #0xc,r0 */
L8c61585e: sa_motion_publish_stop(ctx,0x8c61585eu); r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c615860; /* mov.l @(0x10,r15),r2 */
L8c615860: sa_motion_publish_stop(ctx,0x8c615860u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c615862; /* fdiv fr3,fr4 */
L8c615862: sa_motion_publish_stop(ctx,0x8c615862u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615864; /* fmov.s @(r0,r4),fr3 */
L8c615864: sa_motion_publish_stop(ctx,0x8c615864u); r[0]=16u;goto L8c615866; /* mov #0x10,r0 */
L8c615866: sa_motion_publish_stop(ctx,0x8c615866u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615868; /* mov.l @r15,r3 */
L8c615868: sa_motion_publish_stop(ctx,0x8c615868u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61586a; /* fmov.s @(r0,r2),fr2 */
L8c61586a: sa_motion_publish_stop(ctx,0x8c61586au); r[0]=16u;goto L8c61586c; /* mov #0x10,r0 */
L8c61586c: sa_motion_publish_stop(ctx,0x8c61586cu); r[2]=rd(m,bus,&io_status,0x8c6158a8u,4);if(io_status)return io_status;goto L8c61586e; /* mov.l 0x8c6158a8,r2 */
L8c61586e: sa_motion_publish_stop(ctx,0x8c61586eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615870; /* fsub fr3,fr2 */
L8c615870: sa_motion_publish_stop(ctx,0x8c615870u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c615872; /* fmov.s @r2,fr3 */
L8c615872: sa_motion_publish_stop(ctx,0x8c615872u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615874; /* fmul fr4,fr2 */
L8c615874: sa_motion_publish_stop(ctx,0x8c615874u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615876; /* fdiv fr3,fr2 */
L8c615876: sa_motion_publish_stop(ctx,0x8c615876u); fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c615878; /* fmov.s fr2,@r3 */
L8c615878: sa_motion_publish_stop(ctx,0x8c615878u); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61587a; /* mov.l @(0x4,r15),r1 */
L8c61587a: sa_motion_publish_stop(ctx,0x8c61587au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61587c; /* fmov.s @(r0,r4),fr3 */
L8c61587c: sa_motion_publish_stop(ctx,0x8c61587cu); r[0]=20u;goto L8c61587e; /* mov #0x14,r0 */
L8c61587e: sa_motion_publish_stop(ctx,0x8c61587eu); r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c615880; /* mov.l @r1,r1 */
L8c615880: sa_motion_publish_stop(ctx,0x8c615880u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615882; /* mov.l @r15,r3 */
L8c615882: sa_motion_publish_stop(ctx,0x8c615882u); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c615884; /* fmov.s @(r0,r1),fr2 */
L8c615884: sa_motion_publish_stop(ctx,0x8c615884u); r[1]=rd(m,bus,&io_status,0x8c6158acu,4);if(io_status)return io_status;goto L8c615886; /* mov.l 0x8c6158ac,r1 */
L8c615886: sa_motion_publish_stop(ctx,0x8c615886u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615888; /* fsub fr3,fr2 */
L8c615888: sa_motion_publish_stop(ctx,0x8c615888u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61588a; /* fmov.s @r1,fr3 */
L8c61588a: sa_motion_publish_stop(ctx,0x8c61588au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61588c; /* fmul fr4,fr2 */
L8c61588c: sa_motion_publish_stop(ctx,0x8c61588cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61588e; /* fdiv fr3,fr2 */
L8c61588e: sa_motion_publish_stop(ctx,0x8c61588eu); r[0]=4u;goto L8c615890; /* mov #0x4,r0 */
L8c615890: sa_motion_publish_stop(ctx,0x8c615890u); fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c615892; /* fmov fr2,@(r0,r3) */
L8c615892: sa_motion_publish_stop(ctx,0x8c615892u); r[0]=8u;goto L8c615894; /* mov #0x8,r0 */
L8c615894: sa_motion_publish_stop(ctx,0x8c615894u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615896; /* mov.l @r15,r3 */
L8c615896: sa_motion_publish_stop(ctx,0x8c615896u); fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c615898; /* fmov fr4,@(r0,r3) */
L8c615898: sa_motion_publish_stop(ctx,0x8c61589au); goto L8c6158d8; /* bra 0x8c6158d8 */
L8c6158b0: sa_motion_publish_stop(ctx,0x8c6158b0u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6158b2; /* mov.l @(0x4,r15),r3 */
L8c6158b2: sa_motion_publish_stop(ctx,0x8c6158b2u); r[0]=16u;goto L8c6158b4; /* mov #0x10,r0 */
L8c6158b4: sa_motion_publish_stop(ctx,0x8c6158b4u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6158b6; /* mov.l @r15,r2 */
L8c6158b6: sa_motion_publish_stop(ctx,0x8c6158b6u); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6158b8; /* mov.l @r3,r1 */
L8c6158b8: sa_motion_publish_stop(ctx,0x8c6158b8u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6158ba; /* fmov.s @(r0,r1),fr3 */
L8c6158ba: sa_motion_publish_stop(ctx,0x8c6158bau); r[0]=20u;goto L8c6158bc; /* mov #0x14,r0 */
L8c6158bc: sa_motion_publish_stop(ctx,0x8c6158bcu); fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6158be; /* fmov.s fr3,@r2 */
L8c6158be: sa_motion_publish_stop(ctx,0x8c6158beu); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6158c0; /* mov.l @(0x4,r15),r2 */
L8c6158c0: sa_motion_publish_stop(ctx,0x8c6158c0u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6158c2; /* mov.l @r15,r3 */
L8c6158c2: sa_motion_publish_stop(ctx,0x8c6158c2u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6158c4; /* mov.l @r2,r1 */
L8c6158c4: sa_motion_publish_stop(ctx,0x8c6158c4u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6158c6; /* fmov.s @(r0,r1),fr3 */
L8c6158c6: sa_motion_publish_stop(ctx,0x8c6158c6u); r[0]=4u;goto L8c6158c8; /* mov #0x4,r0 */
L8c6158c8: sa_motion_publish_stop(ctx,0x8c6158c8u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6158ca; /* fmov fr3,@(r0,r3) */
L8c6158ca: sa_motion_publish_stop(ctx,0x8c6158cau); r[0]=24u;goto L8c6158cc; /* mov #0x18,r0 */
L8c6158cc: sa_motion_publish_stop(ctx,0x8c6158ccu); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6158ce; /* mov.l @(0x4,r15),r2 */
L8c6158ce: sa_motion_publish_stop(ctx,0x8c6158ceu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6158d0; /* mov.l @r15,r3 */
L8c6158d0: sa_motion_publish_stop(ctx,0x8c6158d0u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6158d2; /* mov.l @r2,r1 */
L8c6158d2: sa_motion_publish_stop(ctx,0x8c6158d2u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6158d4; /* fmov.s @(r0,r1),fr3 */
L8c6158d4: sa_motion_publish_stop(ctx,0x8c6158d4u); r[0]=8u;goto L8c6158d6; /* mov #0x8,r0 */
L8c6158d6: sa_motion_publish_stop(ctx,0x8c6158d6u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6158d8; /* fmov fr3,@(r0,r3) */
L8c6158d8: sa_motion_publish_stop(ctx,0x8c6158d8u); r[3]=3u;goto L8c6158da; /* mov #0x3,r3 */
L8c6158da: sa_motion_publish_stop(ctx,0x8c6158dau); r[4]=r[15];goto L8c6158dc; /* mov r15,r4 */
L8c6158dc: sa_motion_publish_stop(ctx,0x8c6158dcu); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c6158de; /* cmp/ge r3,r6 */
L8c6158de: sa_motion_publish_stop(ctx,0x8c6158e0u); take=!(*sr&1u);r[4]+=28u;if(take)goto L8c61582e;goto L8c6158e2; /* bf/s 0x8c61582e */
L8c6158e2: sa_motion_publish_stop(ctx,0x8c6158e2u); r[7]=r[4];goto L8c6158e4; /* mov r4,r7 */
L8c6158e4: sa_motion_publish_stop(ctx,0x8c6158e4u); r[0]=8u;goto L8c6158e6; /* mov #0x8,r0 */
L8c6158e6: sa_motion_publish_stop(ctx,0x8c6158e6u); r[3]=r[4];goto L8c6158e8; /* mov r4,r3 */
L8c6158e8: sa_motion_publish_stop(ctx,0x8c6158e8u); fload(m,bus,&io_status,f,6u,r[0]+r[7]);if(io_status)return io_status;goto L8c6158ea; /* fmov.s @(r0,r7),fr6 */
L8c6158ea: sa_motion_publish_stop(ctx,0x8c6158eau); r[3]+=12u;goto L8c6158ec; /* add #0xc,r3 */
L8c6158ec: sa_motion_publish_stop(ctx,0x8c6158ecu); r[6]=r[4];goto L8c6158ee; /* mov r4,r6 */
L8c6158ee: sa_motion_publish_stop(ctx,0x8c6158eeu); r[0]=4u;goto L8c6158f0; /* mov #0x4,r0 */
L8c6158f0: sa_motion_publish_stop(ctx,0x8c6158f0u); fcopy(f,0u,6u);goto L8c6158f2; /* fmov fr6,fr0 */
L8c6158f2: sa_motion_publish_stop(ctx,0x8c6158f2u); r[6]+=24u;goto L8c6158f4; /* add #0x18,r6 */
L8c6158f4: sa_motion_publish_stop(ctx,0x8c6158f4u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6158f6; /* mov.l r3,@r15 */
L8c6158f6: sa_motion_publish_stop(ctx,0x8c6158f6u); fload(m,bus,&io_status,f,5u,r[0]+r[7]);if(io_status)return io_status;goto L8c6158f8; /* fmov.s @(r0,r7),fr5 */
L8c6158f8: sa_motion_publish_stop(ctx,0x8c6158f8u); fload(m,bus,&io_status,f,7u,r[0]+r[3]);if(io_status)return io_status;goto L8c6158fa; /* fmov.s @(r0,r3),fr7 */
L8c6158fa: sa_motion_publish_stop(ctx,0x8c6158fau); r[0]=8u;goto L8c6158fc; /* mov #0x8,r0 */
L8c6158fc: sa_motion_publish_stop(ctx,0x8c6158fcu); fload(m,bus,&io_status,f,8u,r[0]+r[3]);if(io_status)return io_status;goto L8c6158fe; /* fmov.s @(r0,r3),fr8 */
L8c6158fe: sa_motion_publish_stop(ctx,0x8c6158feu); r[0]=4u;goto L8c615900; /* mov #0x4,r0 */
L8c615900: sa_motion_publish_stop(ctx,0x8c615900u); fcopy(f,3u,5u);goto L8c615902; /* fmov fr5,fr3 */
L8c615902: sa_motion_publish_stop(ctx,0x8c615902u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[7],&f->fpscr,&fr[0]);if(status)return status;}goto L8c615904; /* fmul fr7,fr0 */
L8c615904: sa_motion_publish_stop(ctx,0x8c615904u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[8],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615906; /* fmul fr8,fr3 */
L8c615906: sa_motion_publish_stop(ctx,0x8c615906u); fload(m,bus,&io_status,f,4u,r[7]);if(io_status)return io_status;goto L8c615908; /* fmov.s @r7,fr4 */
L8c615908: sa_motion_publish_stop(ctx,0x8c615908u); fload(m,bus,&io_status,f,9u,r[3]);if(io_status)return io_status;goto L8c61590a; /* fmov.s @r3,fr9 */
L8c61590a: sa_motion_publish_stop(ctx,0x8c61590au); fcopy(f,2u,4u);goto L8c61590c; /* fmov fr4,fr2 */
L8c61590c: sa_motion_publish_stop(ctx,0x8c61590cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61590e; /* fmul fr8,fr2 */
L8c61590e: sa_motion_publish_stop(ctx,0x8c61590eu); fcopy(f,1u,5u);goto L8c615910; /* fmov fr5,fr1 */
L8c615910: sa_motion_publish_stop(ctx,0x8c615910u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c615912; /* fsub fr3,fr0 */
L8c615912: sa_motion_publish_stop(ctx,0x8c615912u); fcopy(f,3u,6u);goto L8c615914; /* fmov fr6,fr3 */
L8c615914: sa_motion_publish_stop(ctx,0x8c615914u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615916; /* fmul fr9,fr3 */
L8c615916: sa_motion_publish_stop(ctx,0x8c615916u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615918; /* fsub fr3,fr2 */
L8c615918: sa_motion_publish_stop(ctx,0x8c615918u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61591a; /* fmov.s @(r0,r6),fr3 */
L8c61591a: sa_motion_publish_stop(ctx,0x8c61591au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61591c; /* fmul fr2,fr3 */
L8c61591c: sa_motion_publish_stop(ctx,0x8c61591cu); fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c61591e; /* fmov.s @r6,fr2 */
L8c61591e: sa_motion_publish_stop(ctx,0x8c61591eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615920; /* fmac fr0,fr2,fr3 */
L8c615920: sa_motion_publish_stop(ctx,0x8c615920u); fcopy(f,2u,3u);goto L8c615922; /* fmov fr3,fr2 */
L8c615922: sa_motion_publish_stop(ctx,0x8c615922u); fcopy(f,3u,4u);goto L8c615924; /* fmov fr4,fr3 */
L8c615924: sa_motion_publish_stop(ctx,0x8c615924u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615926; /* fmul fr7,fr3 */
L8c615926: sa_motion_publish_stop(ctx,0x8c615926u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[9],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615928; /* fmul fr9,fr1 */
L8c615928: sa_motion_publish_stop(ctx,0x8c615928u); r[0]=8u;goto L8c61592a; /* mov #0x8,r0 */
L8c61592a: sa_motion_publish_stop(ctx,0x8c61592au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61592c; /* fsub fr3,fr1 */
L8c61592c: sa_motion_publish_stop(ctx,0x8c61592cu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61592e; /* fmov.s @(r0,r6),fr3 */
L8c61592e: sa_motion_publish_stop(ctx,0x8c61592eu); fcopy(f,0u,1u);goto L8c615930; /* fmov fr1,fr0 */
L8c615930: sa_motion_publish_stop(ctx,0x8c615930u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615932; /* fmac fr0,fr3,fr2 */
L8c615932: sa_motion_publish_stop(ctx,0x8c615932u); fcopy(f,3u,15u);goto L8c615934; /* fmov fr15,fr3 */
L8c615934: sa_motion_publish_stop(ctx,0x8c615934u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c615936; /* fneg fr3 */
L8c615936: sa_motion_publish_stop(ctx,0x8c615936u); fcopy(f,4u,2u);goto L8c615938; /* fmov fr2,fr4 */
L8c615938: sa_motion_publish_stop(ctx,0x8c615938u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61593a; /* fmul fr3,fr4 */
L8c61593a: sa_motion_publish_stop(ctx,0x8c61593au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c61593c; /* fldi0 fr2 */
L8c61593c: sa_motion_publish_stop(ctx,0x8c61593cu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c61593e; /* fcmp/gt fr2,fr4 */
L8c61593e: sa_motion_publish_stop(ctx,0x8c61593eu); if((*sr&1u))goto L8c615964;goto L8c615940; /* bt 0x8c615964 */
L8c615940: sa_motion_publish_stop(ctx,0x8c615940u); wr(m,bus,&io_status,r[15]+0x14u,r[5],4);if(io_status)return io_status;goto L8c615942; /* mov.l r5,@(0x14,r15) */
L8c615942: sa_motion_publish_stop(ctx,0x8c615944u); fcopy(f,4u,12u);goto L8c615964; /* bra 0x8c615964 */
L8c615946: sa_motion_publish_stop(ctx,0x8c615946u); r[0]=20u;goto L8c615948; /* mov #0x14,r0 */
L8c615948: sa_motion_publish_stop(ctx,0x8c615948u); fcopy(f,3u,13u);goto L8c61594a; /* fmov fr13,fr3 */
L8c61594a: sa_motion_publish_stop(ctx,0x8c61594au); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61594c; /* fmov.s @(r0,r12),fr2 */
L8c61594c: sa_motion_publish_stop(ctx,0x8c61594cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61594e; /* fsub fr6,fr3 */
L8c61594e: sa_motion_publish_stop(ctx,0x8c61594eu); r[0]=16u;goto L8c615950; /* mov #0x10,r0 */
L8c615950: sa_motion_publish_stop(ctx,0x8c615950u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615952; /* fsub fr14,fr2 */
L8c615952: sa_motion_publish_stop(ctx,0x8c615952u); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c615954; /* fmov.s @(r0,r12),fr1 */
L8c615954: sa_motion_publish_stop(ctx,0x8c615954u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c615956; /* fsub fr13,fr1 */
L8c615956: sa_motion_publish_stop(ctx,0x8c615956u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615958; /* fmul fr2,fr3 */
L8c615958: sa_motion_publish_stop(ctx,0x8c615958u); fcopy(f,2u,14u);goto L8c61595a; /* fmov fr14,fr2 */
L8c61595a: sa_motion_publish_stop(ctx,0x8c61595au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61595c; /* fsub fr5,fr2 */
L8c61595c: sa_motion_publish_stop(ctx,0x8c61595cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61595e; /* fmul fr1,fr2 */
L8c61595e: sa_motion_publish_stop(ctx,0x8c61595eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615960; /* fsub fr2,fr3 */
L8c615960: sa_motion_publish_stop(ctx,0x8c615960u); fcopy(f,4u,3u);goto L8c615962; /* fmov fr3,fr4 */
L8c615962: sa_motion_publish_stop(ctx,0x8c615962u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c615964; /* fmul fr15,fr4 */
L8c615964: sa_motion_publish_stop(ctx,0x8c615964u); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c615966; /* cmp/hs r8,r9 */
L8c615966: sa_motion_publish_stop(ctx,0x8c615966u); if((*sr&1u))goto L8c6159b4;goto L8c615968; /* bt 0x8c6159b4 */
L8c615968: sa_motion_publish_stop(ctx,0x8c615968u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61596a; /* fldi0 fr3 */
L8c61596a: sa_motion_publish_stop(ctx,0x8c61596au); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c61596c; /* fcmp/gt fr4,fr3 */
L8c61596c: sa_motion_publish_stop(ctx,0x8c61596cu); if((*sr&1u))goto L8c6159fc;goto L8c61596e; /* bt 0x8c6159fc */
L8c61596e: sa_motion_publish_stop(ctx,0x8c61596eu); r[5]=rd(m,bus,&io_status,0x8c615bbcu,4);if(io_status)return io_status;goto L8c615970; /* mov.l 0x8c615bbc,r5 */
L8c615970: sa_motion_publish_stop(ctx,0x8c615970u); r[4]=r[11];goto L8c615972; /* mov r11,r4 */
L8c615972: sa_motion_publish_stop(ctx,0x8c615972u); r[8]+=16u;goto L8c615974; /* add #0x10,r8 */
L8c615974: sa_motion_publish_stop(ctx,0x8c615974u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615976; /* fschg */
L8c615976: sa_motion_publish_stop(ctx,0x8c615976u); fcopy(f,0u,1u);goto L8c615978; /* fmov fr1,fr0 */
L8c615978: sa_motion_publish_stop(ctx,0x8c615978u); fcopy(f,2u,3u);goto L8c61597a; /* fmov fr3,fr2 */
L8c61597a: sa_motion_publish_stop(ctx,0x8c61597au); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61597c; /* fmov.s @r4+,fr4 */
L8c61597c: sa_motion_publish_stop(ctx,0x8c61597cu); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61597e; /* fmov.s @r4+,fr6 */
L8c61597e: sa_motion_publish_stop(ctx,0x8c61597eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615980; /* fldi0 fr3 */
L8c615980: sa_motion_publish_stop(ctx,0x8c615980u); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c615982; /* fipr fv4,fv0 */
L8c615982: sa_motion_publish_stop(ctx,0x8c615982u); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615984; /* fmov.s @r4+,fr8 */
L8c615984: sa_motion_publish_stop(ctx,0x8c615984u); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615986; /* fmov.s @r4+,fr10 */
L8c615986: sa_motion_publish_stop(ctx,0x8c615986u); fcopy(f,0u,9u);goto L8c615988; /* fmov fr9,fr0 */
L8c615988: sa_motion_publish_stop(ctx,0x8c615988u); fcopy(f,4u,5u);goto L8c61598a; /* fmov fr5,fr4 */
L8c61598a: sa_motion_publish_stop(ctx,0x8c61598au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c61598c; /* fsub fr3,fr0 */
L8c61598c: sa_motion_publish_stop(ctx,0x8c61598cu); fcopy(f,6u,7u);goto L8c61598e; /* fmov fr7,fr6 */
L8c61598e: sa_motion_publish_stop(ctx,0x8c61598eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615990; /* fschg */
L8c615990: sa_motion_publish_stop(ctx,0x8c615990u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c615992; /* fmov.s fr10,@-r8 */
L8c615992: sa_motion_publish_stop(ctx,0x8c615992u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c615994; /* fmov.s fr0,@-r4 */
L8c615994: sa_motion_publish_stop(ctx,0x8c615994u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c615996; /* ocbwb @r4 */
L8c615996: sa_motion_publish_stop(ctx,0x8c615996u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c615998; /* fmul fr0,fr7 */
L8c615998: sa_motion_publish_stop(ctx,0x8c615998u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c61599a; /* fmov.s fr9,@-r8 */
L8c61599a: sa_motion_publish_stop(ctx,0x8c61599au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c61599c; /* fmul fr0,fr6 */
L8c61599c: sa_motion_publish_stop(ctx,0x8c61599cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c61599e; /* fmov.s fr8,@-r8 */
L8c61599e: sa_motion_publish_stop(ctx,0x8c61599eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6159a0; /* fmul fr0,fr5 */
L8c6159a0: sa_motion_publish_stop(ctx,0x8c6159a0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6159a2; /* mov.l r5,@-r8 */
L8c6159a2: sa_motion_publish_stop(ctx,0x8c6159a2u); r[8]+=32u;goto L8c6159a4; /* add #0x20,r8 */
L8c6159a4: sa_motion_publish_stop(ctx,0x8c6159a4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c6159a6; /* fmov.s fr7,@-r8 */
L8c6159a6: sa_motion_publish_stop(ctx,0x8c6159a6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6159a8; /* fmov.s fr6,@-r8 */
L8c6159a8: sa_motion_publish_stop(ctx,0x8c6159a8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6159aa; /* fmov.s fr5,@-r8 */
L8c6159aa: sa_motion_publish_stop(ctx,0x8c6159aau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6159ac; /* fmov.s fr4,@-r8 */
L8c6159ac: sa_motion_publish_stop(ctx,0x8c6159acu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6159ae; /* pref @r8 */
L8c6159ae: sa_motion_publish_stop(ctx,0x8c6159aeu); r[8]+=16u;goto L8c6159b0; /* add #0x10,r8 */
L8c6159b0: sa_motion_publish_stop(ctx,0x8c6159b2u); r[9]=r[8];goto L8c615a3e; /* bra 0x8c615a3e */
L8c6159b4: sa_motion_publish_stop(ctx,0x8c6159b4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6159b6; /* fldi0 fr3 */
L8c6159b6: sa_motion_publish_stop(ctx,0x8c6159b6u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6159b8; /* fcmp/gt fr4,fr3 */
L8c6159b8: sa_motion_publish_stop(ctx,0x8c6159b8u); if(!(*sr&1u))goto L8c615a3e;goto L8c6159ba; /* bf 0x8c615a3e */
L8c6159ba: sa_motion_publish_stop(ctx,0x8c6159bau); r[5]=rd(m,bus,&io_status,0x8c615bc0u,4);if(io_status)return io_status;goto L8c6159bc; /* mov.l 0x8c615bc0,r5 */
L8c6159bc: sa_motion_publish_stop(ctx,0x8c6159bcu); r[4]=r[10];goto L8c6159be; /* mov r10,r4 */
L8c6159be: sa_motion_publish_stop(ctx,0x8c6159beu); r[8]+=16u;goto L8c6159c0; /* add #0x10,r8 */
L8c6159c0: sa_motion_publish_stop(ctx,0x8c6159c0u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6159c2; /* fschg */
L8c6159c2: sa_motion_publish_stop(ctx,0x8c6159c2u); fcopy(f,0u,1u);goto L8c6159c4; /* fmov fr1,fr0 */
L8c6159c4: sa_motion_publish_stop(ctx,0x8c6159c4u); fcopy(f,2u,3u);goto L8c6159c6; /* fmov fr3,fr2 */
L8c6159c6: sa_motion_publish_stop(ctx,0x8c6159c6u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6159c8; /* fmov.s @r4+,fr4 */
L8c6159c8: sa_motion_publish_stop(ctx,0x8c6159c8u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6159ca; /* fmov.s @r4+,fr6 */
L8c6159ca: sa_motion_publish_stop(ctx,0x8c6159cau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6159cc; /* fldi0 fr3 */
L8c6159cc: sa_motion_publish_stop(ctx,0x8c6159ccu); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c6159ce; /* fipr fv4,fv0 */
L8c6159ce: sa_motion_publish_stop(ctx,0x8c6159ceu); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6159d0; /* fmov.s @r4+,fr8 */
L8c6159d0: sa_motion_publish_stop(ctx,0x8c6159d0u); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6159d2; /* fmov.s @r4+,fr10 */
L8c6159d2: sa_motion_publish_stop(ctx,0x8c6159d2u); fcopy(f,0u,9u);goto L8c6159d4; /* fmov fr9,fr0 */
L8c6159d4: sa_motion_publish_stop(ctx,0x8c6159d4u); fcopy(f,4u,5u);goto L8c6159d6; /* fmov fr5,fr4 */
L8c6159d6: sa_motion_publish_stop(ctx,0x8c6159d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6159d8; /* fsub fr3,fr0 */
L8c6159d8: sa_motion_publish_stop(ctx,0x8c6159d8u); fcopy(f,6u,7u);goto L8c6159da; /* fmov fr7,fr6 */
L8c6159da: sa_motion_publish_stop(ctx,0x8c6159dau); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6159dc; /* fschg */
L8c6159dc: sa_motion_publish_stop(ctx,0x8c6159dcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c6159de; /* fmov.s fr10,@-r8 */
L8c6159de: sa_motion_publish_stop(ctx,0x8c6159deu); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6159e0; /* fmov.s fr0,@-r4 */
L8c6159e0: sa_motion_publish_stop(ctx,0x8c6159e0u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c6159e2; /* ocbwb @r4 */
L8c6159e2: sa_motion_publish_stop(ctx,0x8c6159e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6159e4; /* fmul fr0,fr7 */
L8c6159e4: sa_motion_publish_stop(ctx,0x8c6159e4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c6159e6; /* fmov.s fr9,@-r8 */
L8c6159e6: sa_motion_publish_stop(ctx,0x8c6159e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6159e8; /* fmul fr0,fr6 */
L8c6159e8: sa_motion_publish_stop(ctx,0x8c6159e8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c6159ea; /* fmov.s fr8,@-r8 */
L8c6159ea: sa_motion_publish_stop(ctx,0x8c6159eau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6159ec; /* fmul fr0,fr5 */
L8c6159ec: sa_motion_publish_stop(ctx,0x8c6159ecu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6159ee; /* mov.l r5,@-r8 */
L8c6159ee: sa_motion_publish_stop(ctx,0x8c6159eeu); r[8]+=32u;goto L8c6159f0; /* add #0x20,r8 */
L8c6159f0: sa_motion_publish_stop(ctx,0x8c6159f0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c6159f2; /* fmov.s fr7,@-r8 */
L8c6159f2: sa_motion_publish_stop(ctx,0x8c6159f2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6159f4; /* fmov.s fr6,@-r8 */
L8c6159f4: sa_motion_publish_stop(ctx,0x8c6159f4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6159f6; /* fmov.s fr5,@-r8 */
L8c6159f6: sa_motion_publish_stop(ctx,0x8c6159f6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6159f8; /* fmov.s fr4,@-r8 */
L8c6159f8: sa_motion_publish_stop(ctx,0x8c6159f8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6159fa; /* pref @r8 */
L8c6159fa: sa_motion_publish_stop(ctx,0x8c6159fau); r[8]+=16u;goto L8c6159fc; /* add #0x10,r8 */
L8c6159fc: sa_motion_publish_stop(ctx,0x8c6159fcu); r[5]=rd(m,bus,&io_status,0x8c615bc0u,4);if(io_status)return io_status;goto L8c6159fe; /* mov.l 0x8c615bc0,r5 */
L8c6159fe: sa_motion_publish_stop(ctx,0x8c6159feu); r[4]=r[11];goto L8c615a00; /* mov r11,r4 */
L8c615a00: sa_motion_publish_stop(ctx,0x8c615a00u); r[8]+=16u;goto L8c615a02; /* add #0x10,r8 */
L8c615a02: sa_motion_publish_stop(ctx,0x8c615a02u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615a04; /* fschg */
L8c615a04: sa_motion_publish_stop(ctx,0x8c615a04u); fcopy(f,0u,1u);goto L8c615a06; /* fmov fr1,fr0 */
L8c615a06: sa_motion_publish_stop(ctx,0x8c615a06u); fcopy(f,2u,3u);goto L8c615a08; /* fmov fr3,fr2 */
L8c615a08: sa_motion_publish_stop(ctx,0x8c615a08u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615a0a; /* fmov.s @r4+,fr4 */
L8c615a0a: sa_motion_publish_stop(ctx,0x8c615a0au); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615a0c; /* fmov.s @r4+,fr6 */
L8c615a0c: sa_motion_publish_stop(ctx,0x8c615a0cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615a0e; /* fldi0 fr3 */
L8c615a0e: sa_motion_publish_stop(ctx,0x8c615a0eu); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c615a10; /* fipr fv4,fv0 */
L8c615a10: sa_motion_publish_stop(ctx,0x8c615a10u); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615a12; /* fmov.s @r4+,fr8 */
L8c615a12: sa_motion_publish_stop(ctx,0x8c615a12u); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615a14; /* fmov.s @r4+,fr10 */
L8c615a14: sa_motion_publish_stop(ctx,0x8c615a14u); fcopy(f,0u,9u);goto L8c615a16; /* fmov fr9,fr0 */
L8c615a16: sa_motion_publish_stop(ctx,0x8c615a16u); fcopy(f,4u,5u);goto L8c615a18; /* fmov fr5,fr4 */
L8c615a18: sa_motion_publish_stop(ctx,0x8c615a18u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c615a1a; /* fsub fr3,fr0 */
L8c615a1a: sa_motion_publish_stop(ctx,0x8c615a1au); fcopy(f,6u,7u);goto L8c615a1c; /* fmov fr7,fr6 */
L8c615a1c: sa_motion_publish_stop(ctx,0x8c615a1cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615a1e; /* fschg */
L8c615a1e: sa_motion_publish_stop(ctx,0x8c615a1eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c615a20; /* fmov.s fr10,@-r8 */
L8c615a20: sa_motion_publish_stop(ctx,0x8c615a20u); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c615a22; /* fmov.s fr0,@-r4 */
L8c615a22: sa_motion_publish_stop(ctx,0x8c615a22u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c615a24; /* ocbwb @r4 */
L8c615a24: sa_motion_publish_stop(ctx,0x8c615a24u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c615a26; /* fmul fr0,fr7 */
L8c615a26: sa_motion_publish_stop(ctx,0x8c615a26u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c615a28; /* fmov.s fr9,@-r8 */
L8c615a28: sa_motion_publish_stop(ctx,0x8c615a28u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c615a2a; /* fmul fr0,fr6 */
L8c615a2a: sa_motion_publish_stop(ctx,0x8c615a2au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c615a2c; /* fmov.s fr8,@-r8 */
L8c615a2c: sa_motion_publish_stop(ctx,0x8c615a2cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c615a2e; /* fmul fr0,fr5 */
L8c615a2e: sa_motion_publish_stop(ctx,0x8c615a2eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615a30; /* mov.l r5,@-r8 */
L8c615a30: sa_motion_publish_stop(ctx,0x8c615a30u); r[8]+=32u;goto L8c615a32; /* add #0x20,r8 */
L8c615a32: sa_motion_publish_stop(ctx,0x8c615a32u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c615a34; /* fmov.s fr7,@-r8 */
L8c615a34: sa_motion_publish_stop(ctx,0x8c615a34u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615a36; /* fmov.s fr6,@-r8 */
L8c615a36: sa_motion_publish_stop(ctx,0x8c615a36u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615a38; /* fmov.s fr5,@-r8 */
L8c615a38: sa_motion_publish_stop(ctx,0x8c615a38u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615a3a; /* fmov.s fr4,@-r8 */
L8c615a3a: sa_motion_publish_stop(ctx,0x8c615a3au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615a3c; /* pref @r8 */
L8c615a3c: sa_motion_publish_stop(ctx,0x8c615a3cu); r[8]+=16u;goto L8c615a3e; /* add #0x10,r8 */
L8c615a3e: sa_motion_publish_stop(ctx,0x8c615a3eu); r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c615a40; /* mov.l @(0x14,r15),r2 */
L8c615a40: sa_motion_publish_stop(ctx,0x8c615a40u); *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c615a42; /* tst r2,r2 */
L8c615a42: sa_motion_publish_stop(ctx,0x8c615a42u); if((*sr&1u))goto L8c615ae2;goto L8c615a44; /* bt 0x8c615ae2 */
L8c615a44: sa_motion_publish_stop(ctx,0x8c615a44u); r[0]=64u;goto L8c615a46; /* mov #0x40,r0 */
L8c615a46: sa_motion_publish_stop(ctx,0x8c615a46u); r[4]=rd(m,bus,&io_status,0x8c615bc4u,4);if(io_status)return io_status;goto L8c615a48; /* mov.l 0x8c615bc4,r4 */
L8c615a48: sa_motion_publish_stop(ctx,0x8c615a48u); r[3]=0u;goto L8c615a4a; /* mov #0x0,r3 */
L8c615a4a: sa_motion_publish_stop(ctx,0x8c615a4au); r[9]=rd(m,bus,&io_status,0x8c615bc8u,4);if(io_status)return io_status;goto L8c615a4c; /* mov.l 0x8c615bc8,r9 */
L8c615a4c: sa_motion_publish_stop(ctx,0x8c615a4cu); wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c615a4e; /* mov.l r3,@r4 */
L8c615a4e: sa_motion_publish_stop(ctx,0x8c615a4eu); r[4]=r[15];goto L8c615a50; /* mov r15,r4 */
L8c615a50: sa_motion_publish_stop(ctx,0x8c615a50u); fload(m,bus,&io_status,f,3u,r[11]);if(io_status)return io_status;goto L8c615a52; /* fmov.s @r11,fr3 */
L8c615a52: sa_motion_publish_stop(ctx,0x8c615a52u); fload(m,bus,&io_status,f,2u,r[10]);if(io_status)return io_status;goto L8c615a54; /* fmov.s @r10,fr2 */
L8c615a54: sa_motion_publish_stop(ctx,0x8c615a54u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615a56; /* fadd fr3,fr2 */
L8c615a56: sa_motion_publish_stop(ctx,0x8c615a56u); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c615a58; /* fmov.s @r12,fr3 */
L8c615a58: sa_motion_publish_stop(ctx,0x8c615a58u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615a5a; /* fadd fr3,fr2 */
L8c615a5a: sa_motion_publish_stop(ctx,0x8c615a5au); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615a5c; /* fmov fr2,@(r0,r15) */
L8c615a5c: sa_motion_publish_stop(ctx,0x8c615a5cu); r[0]=4u;goto L8c615a5e; /* mov #0x4,r0 */
L8c615a5e: sa_motion_publish_stop(ctx,0x8c615a5eu); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c615a60; /* fmov.s @(r0,r11),fr3 */
L8c615a60: sa_motion_publish_stop(ctx,0x8c615a60u); fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c615a62; /* fmov.s @(r0,r10),fr2 */
L8c615a62: sa_motion_publish_stop(ctx,0x8c615a62u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615a64; /* fadd fr3,fr2 */
L8c615a64: sa_motion_publish_stop(ctx,0x8c615a64u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c615a66; /* fmov.s @(r0,r12),fr3 */
L8c615a66: sa_motion_publish_stop(ctx,0x8c615a66u); r[0]=68u;goto L8c615a68; /* mov #0x44,r0 */
L8c615a68: sa_motion_publish_stop(ctx,0x8c615a68u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615a6a; /* fadd fr3,fr2 */
L8c615a6a: sa_motion_publish_stop(ctx,0x8c615a6au); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615a6c; /* fmov fr2,@(r0,r15) */
L8c615a6c: sa_motion_publish_stop(ctx,0x8c615a6cu); r[0]=8u;goto L8c615a6e; /* mov #0x8,r0 */
L8c615a6e: sa_motion_publish_stop(ctx,0x8c615a6eu); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c615a70; /* fmov.s @(r0,r11),fr3 */
L8c615a70: sa_motion_publish_stop(ctx,0x8c615a70u); fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c615a72; /* fmov.s @(r0,r10),fr2 */
L8c615a72: sa_motion_publish_stop(ctx,0x8c615a72u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615a74; /* fadd fr3,fr2 */
L8c615a74: sa_motion_publish_stop(ctx,0x8c615a74u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c615a76; /* fmov.s @(r0,r12),fr3 */
L8c615a76: sa_motion_publish_stop(ctx,0x8c615a76u); r[0]=72u;goto L8c615a78; /* mov #0x48,r0 */
L8c615a78: sa_motion_publish_stop(ctx,0x8c615a78u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615a7a; /* fadd fr3,fr2 */
L8c615a7a: sa_motion_publish_stop(ctx,0x8c615a7au); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615a7c; /* fmov fr2,@(r0,r15) */
L8c615a7c: sa_motion_publish_stop(ctx,0x8c615a7eu); target=r[9];*pr=0x8c615a80u;r[4]+=64u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c615a80; /* jsr @r9 */
L8c615a80: sa_motion_publish_stop(ctx,0x8c615a80u); r[4]=rd(m,bus,&io_status,0x8c615bd0u,4);if(io_status)return io_status;goto L8c615a82; /* mov.l 0x8c615bd0,r4 */
L8c615a82: sa_motion_publish_stop(ctx,0x8c615a82u); r[0]=64u;goto L8c615a84; /* mov #0x40,r0 */
L8c615a84: sa_motion_publish_stop(ctx,0x8c615a84u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c615a86; /* fmov.s @(r0,r15),fr3 */
L8c615a86: sa_motion_publish_stop(ctx,0x8c615a86u); r[0]=68u;goto L8c615a88; /* mov #0x44,r0 */
L8c615a88: sa_motion_publish_stop(ctx,0x8c615a88u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c615a8a; /* fmov.s @r4,fr2 */
L8c615a8a: sa_motion_publish_stop(ctx,0x8c615a8au); r[5]=rd(m,bus,&io_status,0x8c615bccu,4);if(io_status)return io_status;goto L8c615a8c; /* mov.l 0x8c615bcc,r5 */
L8c615a8c: sa_motion_publish_stop(ctx,0x8c615a8cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615a8e; /* fmul fr3,fr2 */
L8c615a8e: sa_motion_publish_stop(ctx,0x8c615a8eu); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c615a90; /* fmov.s @r5,fr3 */
L8c615a90: sa_motion_publish_stop(ctx,0x8c615a90u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615a92; /* fsub fr2,fr3 */
L8c615a92: sa_motion_publish_stop(ctx,0x8c615a92u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615a94; /* fmov.s @(r0,r15),fr2 */
L8c615a94: sa_motion_publish_stop(ctx,0x8c615a94u); r[0]=4u;goto L8c615a96; /* mov #0x4,r0 */
L8c615a96: sa_motion_publish_stop(ctx,0x8c615a96u); fload(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c615a98; /* fmov.s @(r0,r4),fr1 */
L8c615a98: sa_motion_publish_stop(ctx,0x8c615a98u); r[0]=72u;goto L8c615a9a; /* mov #0x48,r0 */
L8c615a9a: sa_motion_publish_stop(ctx,0x8c615a9au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615a9c; /* fmul fr2,fr1 */
L8c615a9c: sa_motion_publish_stop(ctx,0x8c615a9cu); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615a9e; /* fmov.s @(r0,r15),fr2 */
L8c615a9e: sa_motion_publish_stop(ctx,0x8c615a9eu); r[0]=8u;goto L8c615aa0; /* mov #0x8,r0 */
L8c615aa0: sa_motion_publish_stop(ctx,0x8c615aa0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615aa2; /* fsub fr1,fr3 */
L8c615aa2: sa_motion_publish_stop(ctx,0x8c615aa2u); fload(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c615aa4; /* fmov.s @(r0,r4),fr1 */
L8c615aa4: sa_motion_publish_stop(ctx,0x8c615aa4u); r[4]=rd(m,bus,&io_status,0x8c615bd4u,4);if(io_status)return io_status;goto L8c615aa6; /* mov.l 0x8c615bd4,r4 */
L8c615aa6: sa_motion_publish_stop(ctx,0x8c615aa6u); r[0]=16u;goto L8c615aa8; /* mov #0x10,r0 */
L8c615aa8: sa_motion_publish_stop(ctx,0x8c615aa8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615aaa; /* fmul fr2,fr1 */
L8c615aaa: sa_motion_publish_stop(ctx,0x8c615aaau); fcopy(f,4u,3u);goto L8c615aac; /* fmov fr3,fr4 */
L8c615aac: sa_motion_publish_stop(ctx,0x8c615aacu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615aae; /* fmov.s @(r0,r4),fr3 */
L8c615aae: sa_motion_publish_stop(ctx,0x8c615aaeu); r[0]=48u;goto L8c615ab0; /* mov #0x30,r0 */
L8c615ab0: sa_motion_publish_stop(ctx,0x8c615ab0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c615ab2; /* fsub fr1,fr4 */
L8c615ab2: sa_motion_publish_stop(ctx,0x8c615ab2u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c615ab4; /* fmov fr3,@(r0,r15) */
L8c615ab4: sa_motion_publish_stop(ctx,0x8c615ab4u); r[0]=20u;goto L8c615ab6; /* mov #0x14,r0 */
L8c615ab6: sa_motion_publish_stop(ctx,0x8c615ab6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615ab8; /* fmov.s @(r0,r4),fr3 */
L8c615ab8: sa_motion_publish_stop(ctx,0x8c615ab8u); r[0]=52u;goto L8c615aba; /* mov #0x34,r0 */
L8c615aba: sa_motion_publish_stop(ctx,0x8c615abau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615abc; /* fmul fr4,fr3 */
L8c615abc: sa_motion_publish_stop(ctx,0x8c615abcu); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c615abe; /* fmov fr3,@(r0,r15) */
L8c615abe: sa_motion_publish_stop(ctx,0x8c615abeu); r[0]=24u;goto L8c615ac0; /* mov #0x18,r0 */
L8c615ac0: sa_motion_publish_stop(ctx,0x8c615ac0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615ac2; /* fmov.s @(r0,r4),fr3 */
L8c615ac2: sa_motion_publish_stop(ctx,0x8c615ac2u); r[0]=56u;goto L8c615ac4; /* mov #0x38,r0 */
L8c615ac4: sa_motion_publish_stop(ctx,0x8c615ac4u); r[7]=r[15];goto L8c615ac6; /* mov r15,r7 */
L8c615ac6: sa_motion_publish_stop(ctx,0x8c615ac6u); r[3]=rd(m,bus,&io_status,0x8c615bd8u,4);if(io_status)return io_status;goto L8c615ac8; /* mov.l 0x8c615bd8,r3 */
L8c615ac8: sa_motion_publish_stop(ctx,0x8c615ac8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615aca; /* fmul fr4,fr3 */
L8c615aca: sa_motion_publish_stop(ctx,0x8c615acau); r[5]=r[11];goto L8c615acc; /* mov r11,r5 */
L8c615acc: sa_motion_publish_stop(ctx,0x8c615accu); r[6]=r[12];goto L8c615ace; /* mov r12,r6 */
L8c615ace: sa_motion_publish_stop(ctx,0x8c615aceu); r[7]+=48u;goto L8c615ad0; /* add #0x30,r7 */
L8c615ad0: sa_motion_publish_stop(ctx,0x8c615ad0u); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c615ad2; /* fmov fr3,@(r0,r15) */
L8c615ad2: sa_motion_publish_stop(ctx,0x8c615ad2u); r[0]=28u;goto L8c615ad4; /* mov #0x1c,r0 */
L8c615ad4: sa_motion_publish_stop(ctx,0x8c615ad4u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615ad6; /* fmov.s @(r0,r4),fr3 */
L8c615ad6: sa_motion_publish_stop(ctx,0x8c615ad6u); r[0]=60u;goto L8c615ad8; /* mov #0x3c,r0 */
L8c615ad8: sa_motion_publish_stop(ctx,0x8c615ad8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615ada; /* fmul fr4,fr3 */
L8c615ada: sa_motion_publish_stop(ctx,0x8c615adau); fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c615adc; /* fmov fr3,@(r0,r15) */
L8c615adc: sa_motion_publish_stop(ctx,0x8c615adeu); target=r[3];*pr=0x8c615ae0u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c615ae0; /* jsr @r3 */
L8c615ae0: sa_motion_publish_stop(ctx,0x8c615ae0u); r[9]=r[8];goto L8c615ae2; /* mov r8,r9 */
L8c615ae2: sa_motion_publish_stop(ctx,0x8c615ae2u); fcopy(f,6u,13u);goto L8c615ae4; /* fmov fr13,fr6 */
L8c615ae4: sa_motion_publish_stop(ctx,0x8c615ae4u); r[0]=16u;goto L8c615ae6; /* mov #0x10,r0 */
L8c615ae6: sa_motion_publish_stop(ctx,0x8c615ae6u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c615ae8; /* mov.l @(0xc,r15),r3 */
L8c615ae8: sa_motion_publish_stop(ctx,0x8c615ae8u); r[10]=r[11];goto L8c615aea; /* mov r11,r10 */
L8c615aea: sa_motion_publish_stop(ctx,0x8c615aeau); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c615aec; /* fmov.s @(r0,r12),fr13 */
L8c615aec: sa_motion_publish_stop(ctx,0x8c615aecu); r[0]=20u;goto L8c615aee; /* mov #0x14,r0 */
L8c615aee: sa_motion_publish_stop(ctx,0x8c615aeeu); fcopy(f,5u,14u);goto L8c615af0; /* fmov fr14,fr5 */
L8c615af0: sa_motion_publish_stop(ctx,0x8c615af0u); r[3]+=4294967295u;goto L8c615af2; /* add #-0x1,r3 */
L8c615af2: sa_motion_publish_stop(ctx,0x8c615af2u); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c615af4; /* fmov.s @(r0,r12),fr14 */
L8c615af4: sa_motion_publish_stop(ctx,0x8c615af4u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c615af6; /* tst r3,r3 */
L8c615af6: sa_motion_publish_stop(ctx,0x8c615af6u); r[11]=r[12];goto L8c615af8; /* mov r12,r11 */
L8c615af8: sa_motion_publish_stop(ctx,0x8c615af8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c615afa; /* fneg fr15 */
L8c615afa: sa_motion_publish_stop(ctx,0x8c615afcu); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c615b02;goto L8c615afe; /* bt/s 0x8c615b02 */
L8c615afe: sa_motion_publish_stop(ctx,0x8c615b00u); goto L8c6157d4; /* bra 0x8c6157d4 */
L8c615b02: sa_motion_publish_stop(ctx,0x8c615b02u); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c615b04; /* cmp/hs r8,r9 */
L8c615b04: sa_motion_publish_stop(ctx,0x8c615b04u); if((*sr&1u))goto L8c615b48;goto L8c615b06; /* bt 0x8c615b48 */
L8c615b06: sa_motion_publish_stop(ctx,0x8c615b06u); r[5]=rd(m,bus,&io_status,0x8c615bbcu,4);if(io_status)return io_status;goto L8c615b08; /* mov.l 0x8c615bbc,r5 */
L8c615b08: sa_motion_publish_stop(ctx,0x8c615b08u); r[4]=r[12];goto L8c615b0a; /* mov r12,r4 */
L8c615b0a: sa_motion_publish_stop(ctx,0x8c615b0au); r[8]+=16u;goto L8c615b0c; /* add #0x10,r8 */
L8c615b0c: sa_motion_publish_stop(ctx,0x8c615b0cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615b0e; /* fschg */
L8c615b0e: sa_motion_publish_stop(ctx,0x8c615b0eu); fcopy(f,0u,1u);goto L8c615b10; /* fmov fr1,fr0 */
L8c615b10: sa_motion_publish_stop(ctx,0x8c615b10u); fcopy(f,2u,3u);goto L8c615b12; /* fmov fr3,fr2 */
L8c615b12: sa_motion_publish_stop(ctx,0x8c615b12u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b14; /* fmov.s @r4+,fr4 */
L8c615b14: sa_motion_publish_stop(ctx,0x8c615b14u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b16; /* fmov.s @r4+,fr6 */
L8c615b16: sa_motion_publish_stop(ctx,0x8c615b16u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615b18; /* fldi0 fr3 */
L8c615b18: sa_motion_publish_stop(ctx,0x8c615b18u); status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c615b1a; /* fipr fv4,fv0 */
L8c615b1a: sa_motion_publish_stop(ctx,0x8c615b1au); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b1c; /* fmov.s @r4+,fr8 */
L8c615b1c: sa_motion_publish_stop(ctx,0x8c615b1cu); fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b1e; /* fmov.s @r4+,fr10 */
L8c615b1e: sa_motion_publish_stop(ctx,0x8c615b1eu); fcopy(f,0u,9u);goto L8c615b20; /* fmov fr9,fr0 */
L8c615b20: sa_motion_publish_stop(ctx,0x8c615b20u); fcopy(f,4u,5u);goto L8c615b22; /* fmov fr5,fr4 */
L8c615b22: sa_motion_publish_stop(ctx,0x8c615b22u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c615b24; /* fsub fr3,fr0 */
L8c615b24: sa_motion_publish_stop(ctx,0x8c615b24u); fcopy(f,6u,7u);goto L8c615b26; /* fmov fr7,fr6 */
L8c615b26: sa_motion_publish_stop(ctx,0x8c615b26u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615b28; /* fschg */
L8c615b28: sa_motion_publish_stop(ctx,0x8c615b28u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c615b2a; /* fmov.s fr10,@-r8 */
L8c615b2a: sa_motion_publish_stop(ctx,0x8c615b2au); r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c615b2c; /* fmov.s fr0,@-r4 */
L8c615b2c: sa_motion_publish_stop(ctx,0x8c615b2cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c615b2e; /* ocbwb @r4 */
L8c615b2e: sa_motion_publish_stop(ctx,0x8c615b2eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c615b30; /* fmul fr0,fr7 */
L8c615b30: sa_motion_publish_stop(ctx,0x8c615b30u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c615b32; /* fmov.s fr9,@-r8 */
L8c615b32: sa_motion_publish_stop(ctx,0x8c615b32u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c615b34; /* fmul fr0,fr6 */
L8c615b34: sa_motion_publish_stop(ctx,0x8c615b34u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c615b36; /* fmov.s fr8,@-r8 */
L8c615b36: sa_motion_publish_stop(ctx,0x8c615b36u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c615b38; /* fmul fr0,fr5 */
L8c615b38: sa_motion_publish_stop(ctx,0x8c615b38u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615b3a; /* mov.l r5,@-r8 */
L8c615b3a: sa_motion_publish_stop(ctx,0x8c615b3au); r[8]+=32u;goto L8c615b3c; /* add #0x20,r8 */
L8c615b3c: sa_motion_publish_stop(ctx,0x8c615b3cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c615b3e; /* fmov.s fr7,@-r8 */
L8c615b3e: sa_motion_publish_stop(ctx,0x8c615b3eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615b40; /* fmov.s fr6,@-r8 */
L8c615b40: sa_motion_publish_stop(ctx,0x8c615b40u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615b42; /* fmov.s fr5,@-r8 */
L8c615b42: sa_motion_publish_stop(ctx,0x8c615b42u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615b44; /* fmov.s fr4,@-r8 */
L8c615b44: sa_motion_publish_stop(ctx,0x8c615b44u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615b46; /* pref @r8 */
L8c615b46: sa_motion_publish_stop(ctx,0x8c615b46u); r[8]+=16u;goto L8c615b48; /* add #0x10,r8 */
L8c615b48: sa_motion_publish_stop(ctx,0x8c615b48u); r[3]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c615b4a; /* mov.l @(0x18,r15),r3 */
L8c615b4a: sa_motion_publish_stop(ctx,0x8c615b4au); r[3]+=4294967295u;goto L8c615b4c; /* add #-0x1,r3 */
L8c615b4c: sa_motion_publish_stop(ctx,0x8c615b4cu); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c615b4e; /* tst r3,r3 */
L8c615b4e: sa_motion_publish_stop(ctx,0x8c615b50u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;if(take)goto L8c615b56;goto L8c615b52; /* bt/s 0x8c615b56 */
L8c615b52: sa_motion_publish_stop(ctx,0x8c615b54u); goto L8c615784; /* bra 0x8c615784 */
L8c615b56: sa_motion_publish_stop(ctx,0x8c615b56u); r[4]=rd(m,bus,&io_status,0x8c615bdcu,4);if(io_status)return io_status;goto L8c615b58; /* mov.l 0x8c615bdc,r4 */
L8c615b58: sa_motion_publish_stop(ctx,0x8c615b58u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615b5a; /* mov.l @(0x8,r15),r3 */
L8c615b5a: sa_motion_publish_stop(ctx,0x8c615b5au); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c615b5c; /* mov.l @r4,r2 */
L8c615b5c: sa_motion_publish_stop(ctx,0x8c615b5cu); r[2]+=r[3];goto L8c615b5e; /* add r3,r2 */
L8c615b5e: sa_motion_publish_stop(ctx,0x8c615b5eu); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c615b60; /* mov.l r2,@r4 */
L8c615b60: sa_motion_publish_stop(ctx,0x8c615b60u); r[4]=rd(m,bus,&io_status,0x8c615be0u,4);if(io_status)return io_status;goto L8c615b62; /* mov.l 0x8c615be0,r4 */
L8c615b62: sa_motion_publish_stop(ctx,0x8c615b62u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615b64; /* mov.l @(0x8,r15),r3 */
L8c615b64: sa_motion_publish_stop(ctx,0x8c615b64u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c615b66; /* mov.l @r4,r2 */
L8c615b66: sa_motion_publish_stop(ctx,0x8c615b66u); r[2]+=r[3];goto L8c615b68; /* add r3,r2 */
L8c615b68: sa_motion_publish_stop(ctx,0x8c615b68u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c615b6a; /* mov.l r2,@r4 */
L8c615b6a: sa_motion_publish_stop(ctx,0x8c615b6au); r[15]+=76u;goto L8c615b6c; /* add #0x4c,r15 */
L8c615b6c: sa_motion_publish_stop(ctx,0x8c615b6cu); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615b6e; /* lds.l @r15+,PR */
L8c615b6e: sa_motion_publish_stop(ctx,0x8c615b6eu); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b70; /* fmov.s @r15+,fr12 */
L8c615b70: sa_motion_publish_stop(ctx,0x8c615b70u); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b72; /* fmov.s @r15+,fr13 */
L8c615b72: sa_motion_publish_stop(ctx,0x8c615b72u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b74; /* fmov.s @r15+,fr14 */
L8c615b74: sa_motion_publish_stop(ctx,0x8c615b74u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b76; /* fmov.s @r15+,fr15 */
L8c615b76: sa_motion_publish_stop(ctx,0x8c615b76u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615b78; /* mov.l @r15+,r9 */
L8c615b78: sa_motion_publish_stop(ctx,0x8c615b78u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615b7a; /* mov.l @r15+,r10 */
L8c615b7a: sa_motion_publish_stop(ctx,0x8c615b7au); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615b7c; /* mov.l @r15+,r11 */
L8c615b7c: sa_motion_publish_stop(ctx,0x8c615b7cu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615b7e; /* mov.l @r15+,r12 */
L8c615b7e: sa_motion_publish_stop(ctx,0x8c615b7eu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615b80; /* mov.l @r15+,r13 */
L8c615b80: sa_motion_publish_stop(ctx,0x8c615b82u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
