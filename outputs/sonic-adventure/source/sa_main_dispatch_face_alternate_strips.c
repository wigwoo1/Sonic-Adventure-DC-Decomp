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
L8c613dfa: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c613dfc; /* mov.l r14,@-r15 */
L8c613dfc: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c613dfe; /* mov.l r13,@-r15 */
L8c613dfe: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c613e00; /* mov.l r12,@-r15 */
L8c613e00: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c613e02; /* mov.l r11,@-r15 */
L8c613e02: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c613e04; /* mov.l r10,@-r15 */
L8c613e04: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c613e06; /* mov.l r9,@-r15 */
L8c613e06: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c613e08; /* fmov.s fr15,@-r15 */
L8c613e08: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c613e0a; /* fmov.s fr14,@-r15 */
L8c613e0a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c613e0c; /* fmov.s fr13,@-r15 */
L8c613e0c: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c613e0e; /* sts.l PR,@-r15 */
L8c613e0e: r[15]+=4294967280u;goto L8c613e10; /* add #-0x10,r15 */
L8c613e10: r[3]=rd(m,bus,&io_status,0x8c613e44u,4);if(io_status)return io_status;goto L8c613e12; /* mov.l 0x8c613e44,r3 */
L8c613e12: r[9]=rd(m,bus,&io_status,0x8c613e40u,4);if(io_status)return io_status;goto L8c613e14; /* mov.l 0x8c613e40,r9 */
L8c613e14: target=r[3];*pr=0x8c613e18u;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c613e18; /* jsr @r3 */
L8c613e18: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613e1a; /* mov.l @(0x4,r15),r3 */
L8c613e1a: r[2]=0u;goto L8c613e1c; /* mov #0x0,r2 */
L8c613e1c: r[14]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613e1e; /* mov.l @(0x4,r15),r14 */
L8c613e1e: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c613e20; /* mov.w @(0x2,r3),r0 */
L8c613e20: r[3]=rd(m,bus,&io_status,0x8c613e48u,4);if(io_status)return io_status;goto L8c613e22; /* mov.l 0x8c613e48,r3 */
L8c613e22: r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c613e24; /* mov.l @(0x4,r14),r14 */
L8c613e24: r[0]=r[0]&65535u;goto L8c613e26; /* extu.w r0,r0 */
L8c613e26: wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c613e28; /* mov.l r0,@(0xc,r15) */
L8c613e28: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c613e2a; /* mov.l @r3,r13 */
L8c613e2a: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613e2c; /* mov.l r2,@(0x8,r15) */
L8c613e2c: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c613e2e; /* mov.l r2,@(0x4,r15) */
L8c613e2e: goto L8c613e4c; /* bra 0x8c613e4c */
L8c613e4c: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613e4e; /* mov.w @r14+,r3 */
L8c613e4e: r[4]=rd(m,bus,&io_status,0x8c613f38u,4);if(io_status)return io_status;goto L8c613e50; /* mov.l 0x8c613f38,r4 */
L8c613e50: r[3]=r[3]&65535u;goto L8c613e52; /* extu.w r3,r3 */
L8c613e52: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c613e54; /* fldi1 fr4 */
L8c613e54: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c613e56; /* tst r4,r3 */
L8c613e56: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c613e58; /* mov.l r3,@r15 */
L8c613e58: take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c613e6a;goto L8c613e5c; /* bt/s 0x8c613e6a */
L8c613e5c: r[4]=sx(m,bus,&io_status,0x8c613f34u,2);if(io_status)return io_status;goto L8c613e5e; /* mov.w 0x8c613f34,r4 */
L8c613e5e: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613e60; /* mov.l @r15,r3 */
L8c613e60: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c613e62; /* fldi1 fr4 */
L8c613e62: r[3]&=r[4];goto L8c613e64; /* and r4,r3 */
L8c613e64: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c613e66; /* fneg fr4 */
L8c613e66: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c613e68; /* mov.l r3,@r15 */
L8c613e68: fcopy(f,15u,4u);goto L8c613e6a; /* fmov fr4,fr15 */
L8c613e6a: r[0]=16u;goto L8c613e6c; /* mov #0x10,r0 */
L8c613e6c: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613e6e; /* mov.l @r15,r3 */
L8c613e6e: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613e70; /* mov.l @(0x4,r15),r2 */
L8c613e70: r[10]=r[8];goto L8c613e72; /* mov r8,r10 */
L8c613e72: r[2]+=r[3];goto L8c613e74; /* add r3,r2 */
L8c613e74: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c613e76; /* mov.l r2,@(0x4,r15) */
L8c613e76: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613e78; /* mov.l @r15,r1 */
L8c613e78: r[1]+=4294967294u;goto L8c613e7a; /* add #-0x2,r1 */
L8c613e7a: wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c613e7c; /* mov.l r1,@r15 */
L8c613e7c: r[4]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613e7e; /* mov.w @r14+,r4 */
L8c613e7e: r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613e80; /* mov.w @r14+,r11 */
L8c613e80: r[4]=r[4]&65535u;goto L8c613e82; /* extu.w r4,r4 */
L8c613e82: r[4]<<=2;goto L8c613e84; /* shll2 r4 */
L8c613e84: r[11]=r[11]&65535u;goto L8c613e86; /* extu.w r11,r11 */
L8c613e86: r[11]<<=2;goto L8c613e88; /* shll2 r11 */
L8c613e88: r[4]<<=2;goto L8c613e8a; /* shll2 r4 */
L8c613e8a: r[11]<<=2;goto L8c613e8c; /* shll2 r11 */
L8c613e8c: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c613e8e; /* shll r4 */
L8c613e8e: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c613e90; /* shll r11 */
L8c613e90: r[11]+=r[13];goto L8c613e92; /* add r13,r11 */
L8c613e92: r[4]+=r[13];goto L8c613e94; /* add r13,r4 */
L8c613e94: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c613e96; /* fmov.s @(r0,r11),fr14 */
L8c613e96: fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c613e98; /* fmov.s @(r0,r4),fr4 */
L8c613e98: r[0]=20u;goto L8c613e9a; /* mov #0x14,r0 */
L8c613e9a: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c613e9c; /* fmov.s @(r0,r11),fr13 */
L8c613e9c: fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c613e9e; /* fmov.s @(r0,r4),fr5 */
L8c613e9e: r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613ea0; /* mov.w @r14+,r12 */
L8c613ea0: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c613ea2; /* mov.w @r14,r3 */
L8c613ea2: r[12]=r[12]&65535u;goto L8c613ea4; /* extu.w r12,r12 */
L8c613ea4: r[12]<<=2;goto L8c613ea6; /* shll2 r12 */
L8c613ea6: r[3]=r[3]&65535u;goto L8c613ea8; /* extu.w r3,r3 */
L8c613ea8: r[3]<<=2;goto L8c613eaa; /* shll2 r3 */
L8c613eaa: r[12]<<=2;goto L8c613eac; /* shll2 r12 */
L8c613eac: r[3]<<=2;goto L8c613eae; /* shll2 r3 */
L8c613eae: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c613eb0; /* shll r12 */
L8c613eb0: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c613eb2; /* shll r3 */
L8c613eb2: r[3]+=r[13];goto L8c613eb4; /* add r13,r3 */
L8c613eb4: r[12]+=r[13];goto L8c613eb6; /* add r13,r12 */
L8c613eb6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c613eb8; /* pref @r3 */
L8c613eb8: r[0]=20u;goto L8c613eba; /* mov #0x14,r0 */
L8c613eba: fcopy(f,3u,14u);goto L8c613ebc; /* fmov fr14,fr3 */
L8c613ebc: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c613ebe; /* fmov.s @(r0,r12),fr2 */
L8c613ebe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613ec0; /* fsub fr4,fr3 */
L8c613ec0: r[0]=16u;goto L8c613ec2; /* mov #0x10,r0 */
L8c613ec2: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c613ec4; /* cmp/hs r8,r10 */
L8c613ec4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613ec6; /* fsub fr13,fr2 */
L8c613ec6: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c613ec8; /* fmov.s @(r0,r12),fr1 */
L8c613ec8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c613eca; /* fsub fr14,fr1 */
L8c613eca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c613ecc; /* fmul fr2,fr3 */
L8c613ecc: fcopy(f,2u,13u);goto L8c613ece; /* fmov fr13,fr2 */
L8c613ece: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613ed0; /* fsub fr5,fr2 */
L8c613ed0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613ed2; /* fmul fr1,fr2 */
L8c613ed2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613ed4; /* fsub fr2,fr3 */
L8c613ed4: fcopy(f,4u,3u);goto L8c613ed6; /* fmov fr3,fr4 */
L8c613ed6: take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c613f3c;goto L8c613eda; /* bt/s 0x8c613f3c */
L8c613eda: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613edc; /* fldi0 fr3 */
L8c613edc: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c613ede; /* fcmp/gt fr4,fr3 */
L8c613ede: if((*sr&1u))goto L8c613f82;goto L8c613ee0; /* bt 0x8c613f82 */
L8c613ee0: r[4]=r[11];goto L8c613ee2; /* mov r11,r4 */
L8c613ee2: r[5]=r[9];goto L8c613ee4; /* mov r9,r5 */
L8c613ee4: r[8]+=16u;goto L8c613ee6; /* add #0x10,r8 */
L8c613ee6: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613ee8; /* fschg */
L8c613ee8: fcopy(f,0u,1u);goto L8c613eea; /* fmov fr1,fr0 */
L8c613eea: fcopy(f,2u,3u);goto L8c613eec; /* fmov fr3,fr2 */
L8c613eec: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613eee; /* fmov.s @r4+,fr4 */
L8c613eee: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613ef0; /* fmov.s @r4+,fr6 */
L8c613ef0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613ef2; /* fldi0 fr3 */
L8c613ef2: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c613ef4; /* fipr fv4,fv0 */
L8c613ef4: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613ef6; /* fmov.s @r4+,fr8 */
L8c613ef6: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613ef8; /* fmov.s @r4+,fr10 */
L8c613ef8: fcopy(f,0u,9u);goto L8c613efa; /* fmov fr9,fr0 */
L8c613efa: fcopy(f,4u,5u);goto L8c613efc; /* fmov fr5,fr4 */
L8c613efc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c613efe; /* fsub fr3,fr0 */
L8c613efe: fcopy(f,6u,7u);goto L8c613f00; /* fmov fr7,fr6 */
L8c613f00: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613f02; /* fschg */
L8c613f02: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c613f04; /* fmov.s fr10,@-r8 */
L8c613f04: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c613f06; /* fmov.s fr0,@-r4 */
L8c613f06: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c613f08; /* ocbwb @r4 */
L8c613f08: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c613f0a; /* fmul fr0,fr7 */
L8c613f0a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c613f0c; /* fmov.s fr9,@-r8 */
L8c613f0c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c613f0e; /* fmul fr0,fr6 */
L8c613f0e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c613f10; /* fmov.s fr8,@-r8 */
L8c613f10: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c613f12; /* fmul fr0,fr5 */
L8c613f12: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613f14; /* mov.l r5,@-r8 */
L8c613f14: r[8]+=32u;goto L8c613f16; /* add #0x20,r8 */
L8c613f16: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c613f18; /* fmov.s fr7,@-r8 */
L8c613f18: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613f1a; /* fmov.s fr6,@-r8 */
L8c613f1a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613f1c; /* fmov.s fr5,@-r8 */
L8c613f1c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613f1e; /* fmov.s fr4,@-r8 */
L8c613f1e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613f20; /* pref @r8 */
L8c613f20: r[8]+=16u;goto L8c613f22; /* add #0x10,r8 */
L8c613f22: r[3]=r[8];goto L8c613f24; /* mov r8,r3 */
L8c613f24: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613f26; /* mov.l @(0x8,r15),r2 */
L8c613f26: r[3]-=r[10];goto L8c613f28; /* sub r10,r3 */
L8c613f28: r[3]+=32u;goto L8c613f2a; /* add #0x20,r3 */
L8c613f2a: r[2]+=r[3];goto L8c613f2c; /* add r3,r2 */
L8c613f2c: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613f2e; /* mov.l r2,@(0x8,r15) */
L8c613f2e: r[10]=r[8];goto L8c613fc4; /* bra 0x8c613fc4 */
L8c613f3c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613f3e; /* fldi0 fr3 */
L8c613f3e: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c613f40; /* fcmp/gt fr4,fr3 */
L8c613f40: if(!(*sr&1u))goto L8c613fc4;goto L8c613f42; /* bf 0x8c613fc4 */
L8c613f42: r[5]=rd(m,bus,&io_status,0x8c6140b0u,4);if(io_status)return io_status;goto L8c613f44; /* mov.l 0x8c6140b0,r5 */
L8c613f44: r[8]+=16u;goto L8c613f46; /* add #0x10,r8 */
L8c613f46: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613f48; /* fschg */
L8c613f48: fcopy(f,0u,1u);goto L8c613f4a; /* fmov fr1,fr0 */
L8c613f4a: fcopy(f,2u,3u);goto L8c613f4c; /* fmov fr3,fr2 */
L8c613f4c: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f4e; /* fmov.s @r4+,fr4 */
L8c613f4e: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f50; /* fmov.s @r4+,fr6 */
L8c613f50: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613f52; /* fldi0 fr3 */
L8c613f52: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c613f54; /* fipr fv4,fv0 */
L8c613f54: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f56; /* fmov.s @r4+,fr8 */
L8c613f56: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f58; /* fmov.s @r4+,fr10 */
L8c613f58: fcopy(f,0u,9u);goto L8c613f5a; /* fmov fr9,fr0 */
L8c613f5a: fcopy(f,4u,5u);goto L8c613f5c; /* fmov fr5,fr4 */
L8c613f5c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c613f5e; /* fsub fr3,fr0 */
L8c613f5e: fcopy(f,6u,7u);goto L8c613f60; /* fmov fr7,fr6 */
L8c613f60: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613f62; /* fschg */
L8c613f62: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c613f64; /* fmov.s fr10,@-r8 */
L8c613f64: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c613f66; /* fmov.s fr0,@-r4 */
L8c613f66: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c613f68; /* ocbwb @r4 */
L8c613f68: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c613f6a; /* fmul fr0,fr7 */
L8c613f6a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c613f6c; /* fmov.s fr9,@-r8 */
L8c613f6c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c613f6e; /* fmul fr0,fr6 */
L8c613f6e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c613f70; /* fmov.s fr8,@-r8 */
L8c613f70: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c613f72; /* fmul fr0,fr5 */
L8c613f72: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613f74; /* mov.l r5,@-r8 */
L8c613f74: r[8]+=32u;goto L8c613f76; /* add #0x20,r8 */
L8c613f76: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c613f78; /* fmov.s fr7,@-r8 */
L8c613f78: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613f7a; /* fmov.s fr6,@-r8 */
L8c613f7a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613f7c; /* fmov.s fr5,@-r8 */
L8c613f7c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613f7e; /* fmov.s fr4,@-r8 */
L8c613f7e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613f80; /* pref @r8 */
L8c613f80: r[8]+=16u;goto L8c613f82; /* add #0x10,r8 */
L8c613f82: r[5]=rd(m,bus,&io_status,0x8c6140b0u,4);if(io_status)return io_status;goto L8c613f84; /* mov.l 0x8c6140b0,r5 */
L8c613f84: r[4]=r[11];goto L8c613f86; /* mov r11,r4 */
L8c613f86: r[8]+=16u;goto L8c613f88; /* add #0x10,r8 */
L8c613f88: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613f8a; /* fschg */
L8c613f8a: fcopy(f,0u,1u);goto L8c613f8c; /* fmov fr1,fr0 */
L8c613f8c: fcopy(f,2u,3u);goto L8c613f8e; /* fmov fr3,fr2 */
L8c613f8e: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f90; /* fmov.s @r4+,fr4 */
L8c613f90: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f92; /* fmov.s @r4+,fr6 */
L8c613f92: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613f94; /* fldi0 fr3 */
L8c613f94: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c613f96; /* fipr fv4,fv0 */
L8c613f96: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f98; /* fmov.s @r4+,fr8 */
L8c613f98: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613f9a; /* fmov.s @r4+,fr10 */
L8c613f9a: fcopy(f,0u,9u);goto L8c613f9c; /* fmov fr9,fr0 */
L8c613f9c: fcopy(f,4u,5u);goto L8c613f9e; /* fmov fr5,fr4 */
L8c613f9e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c613fa0; /* fsub fr3,fr0 */
L8c613fa0: fcopy(f,6u,7u);goto L8c613fa2; /* fmov fr7,fr6 */
L8c613fa2: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613fa4; /* fschg */
L8c613fa4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c613fa6; /* fmov.s fr10,@-r8 */
L8c613fa6: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c613fa8; /* fmov.s fr0,@-r4 */
L8c613fa8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c613faa; /* ocbwb @r4 */
L8c613faa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c613fac; /* fmul fr0,fr7 */
L8c613fac: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c613fae; /* fmov.s fr9,@-r8 */
L8c613fae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c613fb0; /* fmul fr0,fr6 */
L8c613fb0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c613fb2; /* fmov.s fr8,@-r8 */
L8c613fb2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c613fb4; /* fmul fr0,fr5 */
L8c613fb4: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613fb6; /* mov.l r5,@-r8 */
L8c613fb6: r[8]+=32u;goto L8c613fb8; /* add #0x20,r8 */
L8c613fb8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c613fba; /* fmov.s fr7,@-r8 */
L8c613fba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613fbc; /* fmov.s fr6,@-r8 */
L8c613fbc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613fbe; /* fmov.s fr5,@-r8 */
L8c613fbe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613fc0; /* fmov.s fr4,@-r8 */
L8c613fc0: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613fc2; /* pref @r8 */
L8c613fc2: r[8]+=16u;goto L8c613fc4; /* add #0x10,r8 */
L8c613fc4: fcopy(f,4u,14u);goto L8c613fc6; /* fmov fr14,fr4 */
L8c613fc6: r[0]=16u;goto L8c613fc8; /* mov #0x10,r0 */
L8c613fc8: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613fca; /* mov.l @r15,r3 */
L8c613fca: r[4]=r[11];goto L8c613fcc; /* mov r11,r4 */
L8c613fcc: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c613fce; /* fmov.s @(r0,r12),fr14 */
L8c613fce: r[0]=20u;goto L8c613fd0; /* mov #0x14,r0 */
L8c613fd0: fcopy(f,5u,13u);goto L8c613fd2; /* fmov fr13,fr5 */
L8c613fd2: r[3]+=4294967295u;goto L8c613fd4; /* add #-0x1,r3 */
L8c613fd4: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c613fd6; /* fmov.s @(r0,r12),fr13 */
L8c613fd6: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c613fd8; /* tst r3,r3 */
L8c613fd8: r[11]=r[12];goto L8c613fda; /* mov r12,r11 */
L8c613fda: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c613fdc; /* fneg fr15 */
L8c613fdc: take=(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c613fe4;goto L8c613fe0; /* bt/s 0x8c613fe4 */
L8c613fe0: goto L8c613e9e; /* bra 0x8c613e9e */
L8c613fe4: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c613fe6; /* cmp/hs r8,r10 */
L8c613fe6: if((*sr&1u))goto L8c614036;goto L8c613fe8; /* bt 0x8c614036 */
L8c613fe8: r[4]=r[12];goto L8c613fea; /* mov r12,r4 */
L8c613fea: r[5]=r[9];goto L8c613fec; /* mov r9,r5 */
L8c613fec: r[8]+=16u;goto L8c613fee; /* add #0x10,r8 */
L8c613fee: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613ff0; /* fschg */
L8c613ff0: fcopy(f,0u,1u);goto L8c613ff2; /* fmov fr1,fr0 */
L8c613ff2: fcopy(f,2u,3u);goto L8c613ff4; /* fmov fr3,fr2 */
L8c613ff4: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613ff6; /* fmov.s @r4+,fr4 */
L8c613ff6: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613ff8; /* fmov.s @r4+,fr6 */
L8c613ff8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613ffa; /* fldi0 fr3 */
L8c613ffa: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c613ffc; /* fipr fv4,fv0 */
L8c613ffc: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613ffe; /* fmov.s @r4+,fr8 */
L8c613ffe: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614000; /* fmov.s @r4+,fr10 */
L8c614000: fcopy(f,0u,9u);goto L8c614002; /* fmov fr9,fr0 */
L8c614002: fcopy(f,4u,5u);goto L8c614004; /* fmov fr5,fr4 */
L8c614004: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c614006; /* fsub fr3,fr0 */
L8c614006: fcopy(f,6u,7u);goto L8c614008; /* fmov fr7,fr6 */
L8c614008: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61400a; /* fschg */
L8c61400a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c61400c; /* fmov.s fr10,@-r8 */
L8c61400c: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61400e; /* fmov.s fr0,@-r4 */
L8c61400e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c614010; /* ocbwb @r4 */
L8c614010: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c614012; /* fmul fr0,fr7 */
L8c614012: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c614014; /* fmov.s fr9,@-r8 */
L8c614014: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c614016; /* fmul fr0,fr6 */
L8c614016: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c614018; /* fmov.s fr8,@-r8 */
L8c614018: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61401a; /* fmul fr0,fr5 */
L8c61401a: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61401c; /* mov.l r5,@-r8 */
L8c61401c: r[8]+=32u;goto L8c61401e; /* add #0x20,r8 */
L8c61401e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c614020; /* fmov.s fr7,@-r8 */
L8c614020: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614022; /* fmov.s fr6,@-r8 */
L8c614022: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614024; /* fmov.s fr5,@-r8 */
L8c614024: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614026; /* fmov.s fr4,@-r8 */
L8c614026: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614028; /* pref @r8 */
L8c614028: r[8]+=16u;goto L8c61402a; /* add #0x10,r8 */
L8c61402a: r[3]=r[8];goto L8c61402c; /* mov r8,r3 */
L8c61402c: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61402e; /* mov.l @(0x8,r15),r2 */
L8c61402e: r[3]-=r[10];goto L8c614030; /* sub r10,r3 */
L8c614030: r[3]+=32u;goto L8c614032; /* add #0x20,r3 */
L8c614032: r[2]+=r[3];goto L8c614034; /* add r3,r2 */
L8c614034: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c614036; /* mov.l r2,@(0x8,r15) */
L8c614036: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614038; /* mov.l @(0xc,r15),r3 */
L8c614038: r[3]+=4294967295u;goto L8c61403a; /* add #-0x1,r3 */
L8c61403a: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c61403c; /* tst r3,r3 */
L8c61403c: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c614044;goto L8c614040; /* bt/s 0x8c614044 */
L8c614040: goto L8c613e4c; /* bra 0x8c613e4c */
L8c614044: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614046; /* mov.l @(0x8,r15),r3 */
L8c614046: r[4]=rd(m,bus,&io_status,0x8c6140b4u,4);if(io_status)return io_status;goto L8c614048; /* mov.l 0x8c6140b4,r4 */
L8c614048: r[3]>>=2;goto L8c61404a; /* shlr2 r3 */
L8c61404a: r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61404c; /* mov.l @r4,r1 */
L8c61404c: r[3]>>=2;goto L8c61404e; /* shlr2 r3 */
L8c61404e: *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c614050; /* shlr r3 */
L8c614050: r[1]+=r[3];goto L8c614052; /* add r3,r1 */
L8c614052: wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c614054; /* mov.l r1,@r4 */
L8c614054: r[4]=rd(m,bus,&io_status,0x8c6140b8u,4);if(io_status)return io_status;goto L8c614056; /* mov.l 0x8c6140b8,r4 */
L8c614056: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614058; /* mov.l @(0x4,r15),r3 */
L8c614058: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61405a; /* mov.l @r4,r2 */
L8c61405a: r[2]+=r[3];goto L8c61405c; /* add r3,r2 */
L8c61405c: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c61405e; /* mov.l r2,@r4 */
L8c61405e: r[15]+=16u;goto L8c614060; /* add #0x10,r15 */
L8c614060: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614062; /* lds.l @r15+,PR */
L8c614062: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614064; /* fmov.s @r15+,fr13 */
L8c614064: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614066; /* fmov.s @r15+,fr14 */
L8c614066: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614068; /* fmov.s @r15+,fr15 */
L8c614068: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61406a; /* mov.l @r15+,r9 */
L8c61406a: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61406c; /* mov.l @r15+,r10 */
L8c61406c: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61406e; /* mov.l @r15+,r11 */
L8c61406e: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614070; /* mov.l @r15+,r12 */
L8c614070: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614072; /* mov.l @r15+,r13 */
L8c614072: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_face_alternate_strip_clipped_8c61572c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61572c;
L8c61572c: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61572e; /* mov.l r14,@-r15 */
L8c61572e: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c615730; /* mov.l r13,@-r15 */
L8c615730: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c615732; /* mov.l r12,@-r15 */
L8c615732: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c615734; /* mov.l r11,@-r15 */
L8c615734: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c615736; /* mov.l r10,@-r15 */
L8c615736: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c615738; /* mov.l r9,@-r15 */
L8c615738: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61573a; /* fmov.s fr15,@-r15 */
L8c61573a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61573c; /* fmov.s fr14,@-r15 */
L8c61573c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61573e; /* fmov.s fr13,@-r15 */
L8c61573e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c615740; /* fmov.s fr12,@-r15 */
L8c615740: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c615742; /* sts.l PR,@-r15 */
L8c615742: r[15]+=4294967220u;goto L8c615744; /* add #-0x4c,r15 */
L8c615744: r[3]=rd(m,bus,&io_status,0x8c61577cu,4);if(io_status)return io_status;goto L8c615746; /* mov.l 0x8c61577c,r3 */
L8c615746: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c615748; /* fldi1 fr12 */
L8c615748: target=r[3];*pr=0x8c61574cu;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61574c; /* jsr @r3 */
L8c61574c: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61574e; /* mov.l @(0x8,r15),r3 */
L8c61574e: r[2]=0u;goto L8c615750; /* mov #0x0,r2 */
L8c615750: r[14]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615752; /* mov.l @(0x8,r15),r14 */
L8c615752: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c615754; /* mov.w @(0x2,r3),r0 */
L8c615754: r[3]=rd(m,bus,&io_status,0x8c615780u,4);if(io_status)return io_status;goto L8c615756; /* mov.l 0x8c615780,r3 */
L8c615756: r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c615758; /* mov.l @(0x4,r14),r14 */
L8c615758: r[0]=r[0]&65535u;goto L8c61575a; /* extu.w r0,r0 */
L8c61575a: wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c61575c; /* mov.l r0,@(0x18,r15) */
L8c61575c: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61575e; /* mov.l @r3,r13 */
L8c61575e: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c615760; /* mov.l r2,@(0x8,r15) */
L8c615760: goto L8c615784; /* bra 0x8c615784 */
L8c615784: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c615786; /* mov.w @r14+,r3 */
L8c615786: r[4]=rd(m,bus,&io_status,0x8c6158a0u,4);if(io_status)return io_status;goto L8c615788; /* mov.l 0x8c6158a0,r4 */
L8c615788: r[3]=r[3]&65535u;goto L8c61578a; /* extu.w r3,r3 */
L8c61578a: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c61578c; /* tst r4,r3 */
L8c61578c: wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c61578e; /* mov.l r3,@(0xc,r15) */
L8c61578e: take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c6157a0;goto L8c615792; /* bt/s 0x8c6157a0 */
L8c615792: r[4]=sx(m,bus,&io_status,0x8c61589cu,2);if(io_status)return io_status;goto L8c615794; /* mov.w 0x8c61589c,r4 */
L8c615794: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c615796; /* mov.l @(0xc,r15),r3 */
L8c615796: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c615798; /* fldi1 fr4 */
L8c615798: r[3]&=r[4];goto L8c61579a; /* and r4,r3 */
L8c61579a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c61579c; /* fneg fr4 */
L8c61579c: wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c61579e; /* mov.l r3,@(0xc,r15) */
L8c61579e: fcopy(f,15u,4u);goto L8c6157a0; /* fmov fr4,fr15 */
L8c6157a0: r[0]=16u;goto L8c6157a2; /* mov #0x10,r0 */
L8c6157a2: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6157a4; /* mov.l @(0xc,r15),r3 */
L8c6157a4: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6157a6; /* mov.l @(0x8,r15),r2 */
L8c6157a6: r[9]=r[8];goto L8c6157a8; /* mov r8,r9 */
L8c6157a8: r[2]+=r[3];goto L8c6157aa; /* add r3,r2 */
L8c6157aa: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6157ac; /* mov.l r2,@(0x8,r15) */
L8c6157ac: r[1]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6157ae; /* mov.l @(0xc,r15),r1 */
L8c6157ae: r[1]+=4294967294u;goto L8c6157b0; /* add #-0x2,r1 */
L8c6157b0: wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c6157b2; /* mov.l r1,@(0xc,r15) */
L8c6157b2: r[10]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6157b4; /* mov.w @r14+,r10 */
L8c6157b4: r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6157b6; /* mov.w @r14+,r11 */
L8c6157b6: r[10]=r[10]&65535u;goto L8c6157b8; /* extu.w r10,r10 */
L8c6157b8: r[10]<<=2;goto L8c6157ba; /* shll2 r10 */
L8c6157ba: r[11]=r[11]&65535u;goto L8c6157bc; /* extu.w r11,r11 */
L8c6157bc: r[11]<<=2;goto L8c6157be; /* shll2 r11 */
L8c6157be: r[10]<<=2;goto L8c6157c0; /* shll2 r10 */
L8c6157c0: r[11]<<=2;goto L8c6157c2; /* shll2 r11 */
L8c6157c2: *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c6157c4; /* shll r10 */
L8c6157c4: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c6157c6; /* shll r11 */
L8c6157c6: r[11]+=r[13];goto L8c6157c8; /* add r13,r11 */
L8c6157c8: r[10]+=r[13];goto L8c6157ca; /* add r13,r10 */
L8c6157ca: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c6157cc; /* fmov.s @(r0,r11),fr13 */
L8c6157cc: fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c6157ce; /* fmov.s @(r0,r10),fr6 */
L8c6157ce: r[0]=20u;goto L8c6157d0; /* mov #0x14,r0 */
L8c6157d0: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c6157d2; /* fmov.s @(r0,r11),fr14 */
L8c6157d2: fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c6157d4; /* fmov.s @(r0,r10),fr5 */
L8c6157d4: r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6157d6; /* mov.w @r14+,r12 */
L8c6157d6: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c6157d8; /* mov.w @r14,r3 */
L8c6157d8: r[12]=r[12]&65535u;goto L8c6157da; /* extu.w r12,r12 */
L8c6157da: r[12]<<=2;goto L8c6157dc; /* shll2 r12 */
L8c6157dc: r[3]=r[3]&65535u;goto L8c6157de; /* extu.w r3,r3 */
L8c6157de: r[3]<<=2;goto L8c6157e0; /* shll2 r3 */
L8c6157e0: r[12]<<=2;goto L8c6157e2; /* shll2 r12 */
L8c6157e2: r[3]<<=2;goto L8c6157e4; /* shll2 r3 */
L8c6157e4: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c6157e6; /* shll r12 */
L8c6157e6: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c6157e8; /* shll r3 */
L8c6157e8: r[3]+=r[13];goto L8c6157ea; /* add r13,r3 */
L8c6157ea: r[12]+=r[13];goto L8c6157ec; /* add r13,r12 */
L8c6157ec: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c6157ee; /* pref @r3 */
L8c6157ee: r[2]=0u;goto L8c6157f0; /* mov #0x0,r2 */
L8c6157f0: wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c6157f2; /* mov.l r2,@(0x14,r15) */
L8c6157f2: r[4]=63u;goto L8c6157f4; /* mov #0x3f,r4 */
L8c6157f4: r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c6157f6; /* mov.l @(0xc,r10),r2 */
L8c6157f6: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c6157f8; /* mov.l @(0xc,r11),r3 */
L8c6157f8: r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c6157fa; /* mov.l @(0xc,r12),r1 */
L8c6157fa: r[2]&=r[3];goto L8c6157fc; /* and r3,r2 */
L8c6157fc: r[2]&=r[1];goto L8c6157fe; /* and r1,r2 */
L8c6157fe: r[4]&=r[2];goto L8c615800; /* and r2,r4 */
L8c615800: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c615802; /* tst r4,r4 */
L8c615802: take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c61580a;goto L8c615806; /* bt/s 0x8c61580a */
L8c615806: goto L8c615964; /* bra 0x8c615964 */
L8c61580a: r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c61580c; /* mov.l @(0xc,r10),r4 */
L8c61580c: r[5]=1u;goto L8c61580e; /* mov #0x1,r5 */
L8c61580e: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c615810; /* mov.l @(0xc,r11),r3 */
L8c615810: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c615812; /* mov.l @(0xc,r12),r2 */
L8c615812: r[4]|=r[3];goto L8c615814; /* or r3,r4 */
L8c615814: r[4]|=r[2];goto L8c615816; /* or r2,r4 */
L8c615816: r[4]&=r[5];goto L8c615818; /* and r5,r4 */
L8c615818: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c61581a; /* tst r4,r4 */
L8c61581a: if(!(*sr&1u))goto L8c615820;goto L8c61581c; /* bf 0x8c615820 */
L8c61581c: goto L8c615946; /* bra 0x8c615946 */
L8c615820: r[7]=r[15];goto L8c615822; /* mov r15,r7 */
L8c615822: r[7]+=64u;goto L8c615824; /* add #0x40,r7 */
L8c615824: wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c615826; /* mov.l r10,@r7 */
L8c615826: wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c615828; /* mov.l r11,@(0x4,r7) */
L8c615828: wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c61582a; /* mov.l r12,@(0x8,r7) */
L8c61582a: r[6]=0u;goto L8c6158d8; /* bra 0x8c6158d8 */
L8c61582e: r[2]=r[6];goto L8c615830; /* mov r6,r2 */
L8c615830: r[1]=r[6];goto L8c615832; /* mov r6,r1 */
L8c615832: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c615834; /* shll r2 */
L8c615834: r[3]=r[6];goto L8c615836; /* mov r6,r3 */
L8c615836: r[3]<<=2;goto L8c615838; /* shll2 r3 */
L8c615838: r[2]+=r[1];goto L8c61583a; /* add r1,r2 */
L8c61583a: r[2]<<=2;goto L8c61583c; /* shll2 r2 */
L8c61583c: r[3]+=r[7];goto L8c61583e; /* add r7,r3 */
L8c61583e: r[2]+=r[4];goto L8c615840; /* add r4,r2 */
L8c615840: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c615842; /* mov.l r3,@(0x4,r15) */
L8c615842: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c615844; /* mov.l r2,@r15 */
L8c615844: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c615846; /* mov.l @r3,r1 */
L8c615846: r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c615848; /* mov.l @(0xc,r1),r2 */
L8c615848: *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c61584a; /* tst r5,r2 */
L8c61584a: take=!(*sr&1u);r[6]+=1u;if(take)goto L8c6158b0;goto L8c61584e; /* bf/s 0x8c6158b0 */
L8c61584e: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615850; /* mov.l @(0x4,r15),r1 */
L8c615850: r[0]=24u;goto L8c615852; /* mov #0x18,r0 */
L8c615852: fcopy(f,4u,12u);goto L8c615854; /* fmov fr12,fr4 */
L8c615854: r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c615856; /* mov.l @r1,r3 */
L8c615856: r[4]=rd(m,bus,&io_status,0x8c6158a4u,4);if(io_status)return io_status;goto L8c615858; /* mov.l 0x8c6158a4,r4 */
L8c615858: wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c61585a; /* mov.l r3,@(0x10,r15) */
L8c61585a: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61585c; /* fmov.s @(r0,r3),fr3 */
L8c61585c: r[0]=12u;goto L8c61585e; /* mov #0xc,r0 */
L8c61585e: r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c615860; /* mov.l @(0x10,r15),r2 */
L8c615860: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c615862; /* fdiv fr3,fr4 */
L8c615862: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615864; /* fmov.s @(r0,r4),fr3 */
L8c615864: r[0]=16u;goto L8c615866; /* mov #0x10,r0 */
L8c615866: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615868; /* mov.l @r15,r3 */
L8c615868: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61586a; /* fmov.s @(r0,r2),fr2 */
L8c61586a: r[0]=16u;goto L8c61586c; /* mov #0x10,r0 */
L8c61586c: r[2]=rd(m,bus,&io_status,0x8c6158a8u,4);if(io_status)return io_status;goto L8c61586e; /* mov.l 0x8c6158a8,r2 */
L8c61586e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615870; /* fsub fr3,fr2 */
L8c615870: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c615872; /* fmov.s @r2,fr3 */
L8c615872: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615874; /* fmul fr4,fr2 */
L8c615874: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615876; /* fdiv fr3,fr2 */
L8c615876: fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c615878; /* fmov.s fr2,@r3 */
L8c615878: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61587a; /* mov.l @(0x4,r15),r1 */
L8c61587a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61587c; /* fmov.s @(r0,r4),fr3 */
L8c61587c: r[0]=20u;goto L8c61587e; /* mov #0x14,r0 */
L8c61587e: r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c615880; /* mov.l @r1,r1 */
L8c615880: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615882; /* mov.l @r15,r3 */
L8c615882: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c615884; /* fmov.s @(r0,r1),fr2 */
L8c615884: r[1]=rd(m,bus,&io_status,0x8c6158acu,4);if(io_status)return io_status;goto L8c615886; /* mov.l 0x8c6158ac,r1 */
L8c615886: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615888; /* fsub fr3,fr2 */
L8c615888: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61588a; /* fmov.s @r1,fr3 */
L8c61588a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61588c; /* fmul fr4,fr2 */
L8c61588c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61588e; /* fdiv fr3,fr2 */
L8c61588e: r[0]=4u;goto L8c615890; /* mov #0x4,r0 */
L8c615890: fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c615892; /* fmov fr2,@(r0,r3) */
L8c615892: r[0]=8u;goto L8c615894; /* mov #0x8,r0 */
L8c615894: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615896; /* mov.l @r15,r3 */
L8c615896: fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c615898; /* fmov fr4,@(r0,r3) */
L8c615898: goto L8c6158d8; /* bra 0x8c6158d8 */
L8c6158b0: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6158b2; /* mov.l @(0x4,r15),r3 */
L8c6158b2: r[0]=16u;goto L8c6158b4; /* mov #0x10,r0 */
L8c6158b4: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6158b6; /* mov.l @r15,r2 */
L8c6158b6: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6158b8; /* mov.l @r3,r1 */
L8c6158b8: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6158ba; /* fmov.s @(r0,r1),fr3 */
L8c6158ba: r[0]=20u;goto L8c6158bc; /* mov #0x14,r0 */
L8c6158bc: fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6158be; /* fmov.s fr3,@r2 */
L8c6158be: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6158c0; /* mov.l @(0x4,r15),r2 */
L8c6158c0: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6158c2; /* mov.l @r15,r3 */
L8c6158c2: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6158c4; /* mov.l @r2,r1 */
L8c6158c4: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6158c6; /* fmov.s @(r0,r1),fr3 */
L8c6158c6: r[0]=4u;goto L8c6158c8; /* mov #0x4,r0 */
L8c6158c8: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6158ca; /* fmov fr3,@(r0,r3) */
L8c6158ca: r[0]=24u;goto L8c6158cc; /* mov #0x18,r0 */
L8c6158cc: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6158ce; /* mov.l @(0x4,r15),r2 */
L8c6158ce: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6158d0; /* mov.l @r15,r3 */
L8c6158d0: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6158d2; /* mov.l @r2,r1 */
L8c6158d2: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6158d4; /* fmov.s @(r0,r1),fr3 */
L8c6158d4: r[0]=8u;goto L8c6158d6; /* mov #0x8,r0 */
L8c6158d6: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6158d8; /* fmov fr3,@(r0,r3) */
L8c6158d8: r[3]=3u;goto L8c6158da; /* mov #0x3,r3 */
L8c6158da: r[4]=r[15];goto L8c6158dc; /* mov r15,r4 */
L8c6158dc: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c6158de; /* cmp/ge r3,r6 */
L8c6158de: take=!(*sr&1u);r[4]+=28u;if(take)goto L8c61582e;goto L8c6158e2; /* bf/s 0x8c61582e */
L8c6158e2: r[7]=r[4];goto L8c6158e4; /* mov r4,r7 */
L8c6158e4: r[0]=8u;goto L8c6158e6; /* mov #0x8,r0 */
L8c6158e6: r[3]=r[4];goto L8c6158e8; /* mov r4,r3 */
L8c6158e8: fload(m,bus,&io_status,f,6u,r[0]+r[7]);if(io_status)return io_status;goto L8c6158ea; /* fmov.s @(r0,r7),fr6 */
L8c6158ea: r[3]+=12u;goto L8c6158ec; /* add #0xc,r3 */
L8c6158ec: r[6]=r[4];goto L8c6158ee; /* mov r4,r6 */
L8c6158ee: r[0]=4u;goto L8c6158f0; /* mov #0x4,r0 */
L8c6158f0: fcopy(f,0u,6u);goto L8c6158f2; /* fmov fr6,fr0 */
L8c6158f2: r[6]+=24u;goto L8c6158f4; /* add #0x18,r6 */
L8c6158f4: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6158f6; /* mov.l r3,@r15 */
L8c6158f6: fload(m,bus,&io_status,f,5u,r[0]+r[7]);if(io_status)return io_status;goto L8c6158f8; /* fmov.s @(r0,r7),fr5 */
L8c6158f8: fload(m,bus,&io_status,f,7u,r[0]+r[3]);if(io_status)return io_status;goto L8c6158fa; /* fmov.s @(r0,r3),fr7 */
L8c6158fa: r[0]=8u;goto L8c6158fc; /* mov #0x8,r0 */
L8c6158fc: fload(m,bus,&io_status,f,8u,r[0]+r[3]);if(io_status)return io_status;goto L8c6158fe; /* fmov.s @(r0,r3),fr8 */
L8c6158fe: r[0]=4u;goto L8c615900; /* mov #0x4,r0 */
L8c615900: fcopy(f,3u,5u);goto L8c615902; /* fmov fr5,fr3 */
L8c615902: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[7],&f->fpscr,&fr[0]);if(status)return status;}goto L8c615904; /* fmul fr7,fr0 */
L8c615904: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[8],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615906; /* fmul fr8,fr3 */
L8c615906: fload(m,bus,&io_status,f,4u,r[7]);if(io_status)return io_status;goto L8c615908; /* fmov.s @r7,fr4 */
L8c615908: fload(m,bus,&io_status,f,9u,r[3]);if(io_status)return io_status;goto L8c61590a; /* fmov.s @r3,fr9 */
L8c61590a: fcopy(f,2u,4u);goto L8c61590c; /* fmov fr4,fr2 */
L8c61590c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61590e; /* fmul fr8,fr2 */
L8c61590e: fcopy(f,1u,5u);goto L8c615910; /* fmov fr5,fr1 */
L8c615910: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c615912; /* fsub fr3,fr0 */
L8c615912: fcopy(f,3u,6u);goto L8c615914; /* fmov fr6,fr3 */
L8c615914: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615916; /* fmul fr9,fr3 */
L8c615916: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615918; /* fsub fr3,fr2 */
L8c615918: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61591a; /* fmov.s @(r0,r6),fr3 */
L8c61591a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61591c; /* fmul fr2,fr3 */
L8c61591c: fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c61591e; /* fmov.s @r6,fr2 */
L8c61591e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615920; /* fmac fr0,fr2,fr3 */
L8c615920: fcopy(f,2u,3u);goto L8c615922; /* fmov fr3,fr2 */
L8c615922: fcopy(f,3u,4u);goto L8c615924; /* fmov fr4,fr3 */
L8c615924: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615926; /* fmul fr7,fr3 */
L8c615926: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[9],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615928; /* fmul fr9,fr1 */
L8c615928: r[0]=8u;goto L8c61592a; /* mov #0x8,r0 */
L8c61592a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61592c; /* fsub fr3,fr1 */
L8c61592c: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61592e; /* fmov.s @(r0,r6),fr3 */
L8c61592e: fcopy(f,0u,1u);goto L8c615930; /* fmov fr1,fr0 */
L8c615930: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615932; /* fmac fr0,fr3,fr2 */
L8c615932: fcopy(f,3u,15u);goto L8c615934; /* fmov fr15,fr3 */
L8c615934: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c615936; /* fneg fr3 */
L8c615936: fcopy(f,4u,2u);goto L8c615938; /* fmov fr2,fr4 */
L8c615938: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61593a; /* fmul fr3,fr4 */
L8c61593a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c61593c; /* fldi0 fr2 */
L8c61593c: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c61593e; /* fcmp/gt fr2,fr4 */
L8c61593e: if((*sr&1u))goto L8c615964;goto L8c615940; /* bt 0x8c615964 */
L8c615940: wr(m,bus,&io_status,r[15]+0x14u,r[5],4);if(io_status)return io_status;goto L8c615942; /* mov.l r5,@(0x14,r15) */
L8c615942: fcopy(f,4u,12u);goto L8c615964; /* bra 0x8c615964 */
L8c615946: r[0]=20u;goto L8c615948; /* mov #0x14,r0 */
L8c615948: fcopy(f,3u,13u);goto L8c61594a; /* fmov fr13,fr3 */
L8c61594a: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61594c; /* fmov.s @(r0,r12),fr2 */
L8c61594c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61594e; /* fsub fr6,fr3 */
L8c61594e: r[0]=16u;goto L8c615950; /* mov #0x10,r0 */
L8c615950: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615952; /* fsub fr14,fr2 */
L8c615952: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c615954; /* fmov.s @(r0,r12),fr1 */
L8c615954: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c615956; /* fsub fr13,fr1 */
L8c615956: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615958; /* fmul fr2,fr3 */
L8c615958: fcopy(f,2u,14u);goto L8c61595a; /* fmov fr14,fr2 */
L8c61595a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61595c; /* fsub fr5,fr2 */
L8c61595c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61595e; /* fmul fr1,fr2 */
L8c61595e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615960; /* fsub fr2,fr3 */
L8c615960: fcopy(f,4u,3u);goto L8c615962; /* fmov fr3,fr4 */
L8c615962: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c615964; /* fmul fr15,fr4 */
L8c615964: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c615966; /* cmp/hs r8,r9 */
L8c615966: if((*sr&1u))goto L8c6159b4;goto L8c615968; /* bt 0x8c6159b4 */
L8c615968: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61596a; /* fldi0 fr3 */
L8c61596a: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c61596c; /* fcmp/gt fr4,fr3 */
L8c61596c: if((*sr&1u))goto L8c6159fc;goto L8c61596e; /* bt 0x8c6159fc */
L8c61596e: r[5]=rd(m,bus,&io_status,0x8c615bbcu,4);if(io_status)return io_status;goto L8c615970; /* mov.l 0x8c615bbc,r5 */
L8c615970: r[4]=r[11];goto L8c615972; /* mov r11,r4 */
L8c615972: r[8]+=16u;goto L8c615974; /* add #0x10,r8 */
L8c615974: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615976; /* fschg */
L8c615976: fcopy(f,0u,1u);goto L8c615978; /* fmov fr1,fr0 */
L8c615978: fcopy(f,2u,3u);goto L8c61597a; /* fmov fr3,fr2 */
L8c61597a: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61597c; /* fmov.s @r4+,fr4 */
L8c61597c: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61597e; /* fmov.s @r4+,fr6 */
L8c61597e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615980; /* fldi0 fr3 */
L8c615980: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c615982; /* fipr fv4,fv0 */
L8c615982: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615984; /* fmov.s @r4+,fr8 */
L8c615984: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615986; /* fmov.s @r4+,fr10 */
L8c615986: fcopy(f,0u,9u);goto L8c615988; /* fmov fr9,fr0 */
L8c615988: fcopy(f,4u,5u);goto L8c61598a; /* fmov fr5,fr4 */
L8c61598a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c61598c; /* fsub fr3,fr0 */
L8c61598c: fcopy(f,6u,7u);goto L8c61598e; /* fmov fr7,fr6 */
L8c61598e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615990; /* fschg */
L8c615990: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c615992; /* fmov.s fr10,@-r8 */
L8c615992: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c615994; /* fmov.s fr0,@-r4 */
L8c615994: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c615996; /* ocbwb @r4 */
L8c615996: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c615998; /* fmul fr0,fr7 */
L8c615998: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c61599a; /* fmov.s fr9,@-r8 */
L8c61599a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c61599c; /* fmul fr0,fr6 */
L8c61599c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c61599e; /* fmov.s fr8,@-r8 */
L8c61599e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6159a0; /* fmul fr0,fr5 */
L8c6159a0: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6159a2; /* mov.l r5,@-r8 */
L8c6159a2: r[8]+=32u;goto L8c6159a4; /* add #0x20,r8 */
L8c6159a4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c6159a6; /* fmov.s fr7,@-r8 */
L8c6159a6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6159a8; /* fmov.s fr6,@-r8 */
L8c6159a8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6159aa; /* fmov.s fr5,@-r8 */
L8c6159aa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6159ac; /* fmov.s fr4,@-r8 */
L8c6159ac: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6159ae; /* pref @r8 */
L8c6159ae: r[8]+=16u;goto L8c6159b0; /* add #0x10,r8 */
L8c6159b0: r[9]=r[8];goto L8c615a3e; /* bra 0x8c615a3e */
L8c6159b4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6159b6; /* fldi0 fr3 */
L8c6159b6: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6159b8; /* fcmp/gt fr4,fr3 */
L8c6159b8: if(!(*sr&1u))goto L8c615a3e;goto L8c6159ba; /* bf 0x8c615a3e */
L8c6159ba: r[5]=rd(m,bus,&io_status,0x8c615bc0u,4);if(io_status)return io_status;goto L8c6159bc; /* mov.l 0x8c615bc0,r5 */
L8c6159bc: r[4]=r[10];goto L8c6159be; /* mov r10,r4 */
L8c6159be: r[8]+=16u;goto L8c6159c0; /* add #0x10,r8 */
L8c6159c0: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6159c2; /* fschg */
L8c6159c2: fcopy(f,0u,1u);goto L8c6159c4; /* fmov fr1,fr0 */
L8c6159c4: fcopy(f,2u,3u);goto L8c6159c6; /* fmov fr3,fr2 */
L8c6159c6: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6159c8; /* fmov.s @r4+,fr4 */
L8c6159c8: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6159ca; /* fmov.s @r4+,fr6 */
L8c6159ca: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6159cc; /* fldi0 fr3 */
L8c6159cc: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c6159ce; /* fipr fv4,fv0 */
L8c6159ce: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6159d0; /* fmov.s @r4+,fr8 */
L8c6159d0: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6159d2; /* fmov.s @r4+,fr10 */
L8c6159d2: fcopy(f,0u,9u);goto L8c6159d4; /* fmov fr9,fr0 */
L8c6159d4: fcopy(f,4u,5u);goto L8c6159d6; /* fmov fr5,fr4 */
L8c6159d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6159d8; /* fsub fr3,fr0 */
L8c6159d8: fcopy(f,6u,7u);goto L8c6159da; /* fmov fr7,fr6 */
L8c6159da: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6159dc; /* fschg */
L8c6159dc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c6159de; /* fmov.s fr10,@-r8 */
L8c6159de: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6159e0; /* fmov.s fr0,@-r4 */
L8c6159e0: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c6159e2; /* ocbwb @r4 */
L8c6159e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6159e4; /* fmul fr0,fr7 */
L8c6159e4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c6159e6; /* fmov.s fr9,@-r8 */
L8c6159e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6159e8; /* fmul fr0,fr6 */
L8c6159e8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c6159ea; /* fmov.s fr8,@-r8 */
L8c6159ea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6159ec; /* fmul fr0,fr5 */
L8c6159ec: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6159ee; /* mov.l r5,@-r8 */
L8c6159ee: r[8]+=32u;goto L8c6159f0; /* add #0x20,r8 */
L8c6159f0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c6159f2; /* fmov.s fr7,@-r8 */
L8c6159f2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6159f4; /* fmov.s fr6,@-r8 */
L8c6159f4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6159f6; /* fmov.s fr5,@-r8 */
L8c6159f6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6159f8; /* fmov.s fr4,@-r8 */
L8c6159f8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6159fa; /* pref @r8 */
L8c6159fa: r[8]+=16u;goto L8c6159fc; /* add #0x10,r8 */
L8c6159fc: r[5]=rd(m,bus,&io_status,0x8c615bc0u,4);if(io_status)return io_status;goto L8c6159fe; /* mov.l 0x8c615bc0,r5 */
L8c6159fe: r[4]=r[11];goto L8c615a00; /* mov r11,r4 */
L8c615a00: r[8]+=16u;goto L8c615a02; /* add #0x10,r8 */
L8c615a02: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615a04; /* fschg */
L8c615a04: fcopy(f,0u,1u);goto L8c615a06; /* fmov fr1,fr0 */
L8c615a06: fcopy(f,2u,3u);goto L8c615a08; /* fmov fr3,fr2 */
L8c615a08: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615a0a; /* fmov.s @r4+,fr4 */
L8c615a0a: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615a0c; /* fmov.s @r4+,fr6 */
L8c615a0c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615a0e; /* fldi0 fr3 */
L8c615a0e: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c615a10; /* fipr fv4,fv0 */
L8c615a10: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615a12; /* fmov.s @r4+,fr8 */
L8c615a12: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615a14; /* fmov.s @r4+,fr10 */
L8c615a14: fcopy(f,0u,9u);goto L8c615a16; /* fmov fr9,fr0 */
L8c615a16: fcopy(f,4u,5u);goto L8c615a18; /* fmov fr5,fr4 */
L8c615a18: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c615a1a; /* fsub fr3,fr0 */
L8c615a1a: fcopy(f,6u,7u);goto L8c615a1c; /* fmov fr7,fr6 */
L8c615a1c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615a1e; /* fschg */
L8c615a1e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c615a20; /* fmov.s fr10,@-r8 */
L8c615a20: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c615a22; /* fmov.s fr0,@-r4 */
L8c615a22: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c615a24; /* ocbwb @r4 */
L8c615a24: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c615a26; /* fmul fr0,fr7 */
L8c615a26: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c615a28; /* fmov.s fr9,@-r8 */
L8c615a28: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c615a2a; /* fmul fr0,fr6 */
L8c615a2a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c615a2c; /* fmov.s fr8,@-r8 */
L8c615a2c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c615a2e; /* fmul fr0,fr5 */
L8c615a2e: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615a30; /* mov.l r5,@-r8 */
L8c615a30: r[8]+=32u;goto L8c615a32; /* add #0x20,r8 */
L8c615a32: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c615a34; /* fmov.s fr7,@-r8 */
L8c615a34: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615a36; /* fmov.s fr6,@-r8 */
L8c615a36: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615a38; /* fmov.s fr5,@-r8 */
L8c615a38: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615a3a; /* fmov.s fr4,@-r8 */
L8c615a3a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615a3c; /* pref @r8 */
L8c615a3c: r[8]+=16u;goto L8c615a3e; /* add #0x10,r8 */
L8c615a3e: r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c615a40; /* mov.l @(0x14,r15),r2 */
L8c615a40: *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c615a42; /* tst r2,r2 */
L8c615a42: if((*sr&1u))goto L8c615ae2;goto L8c615a44; /* bt 0x8c615ae2 */
L8c615a44: r[0]=64u;goto L8c615a46; /* mov #0x40,r0 */
L8c615a46: r[4]=rd(m,bus,&io_status,0x8c615bc4u,4);if(io_status)return io_status;goto L8c615a48; /* mov.l 0x8c615bc4,r4 */
L8c615a48: r[3]=0u;goto L8c615a4a; /* mov #0x0,r3 */
L8c615a4a: r[9]=rd(m,bus,&io_status,0x8c615bc8u,4);if(io_status)return io_status;goto L8c615a4c; /* mov.l 0x8c615bc8,r9 */
L8c615a4c: wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c615a4e; /* mov.l r3,@r4 */
L8c615a4e: r[4]=r[15];goto L8c615a50; /* mov r15,r4 */
L8c615a50: fload(m,bus,&io_status,f,3u,r[11]);if(io_status)return io_status;goto L8c615a52; /* fmov.s @r11,fr3 */
L8c615a52: fload(m,bus,&io_status,f,2u,r[10]);if(io_status)return io_status;goto L8c615a54; /* fmov.s @r10,fr2 */
L8c615a54: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615a56; /* fadd fr3,fr2 */
L8c615a56: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c615a58; /* fmov.s @r12,fr3 */
L8c615a58: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615a5a; /* fadd fr3,fr2 */
L8c615a5a: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615a5c; /* fmov fr2,@(r0,r15) */
L8c615a5c: r[0]=4u;goto L8c615a5e; /* mov #0x4,r0 */
L8c615a5e: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c615a60; /* fmov.s @(r0,r11),fr3 */
L8c615a60: fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c615a62; /* fmov.s @(r0,r10),fr2 */
L8c615a62: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615a64; /* fadd fr3,fr2 */
L8c615a64: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c615a66; /* fmov.s @(r0,r12),fr3 */
L8c615a66: r[0]=68u;goto L8c615a68; /* mov #0x44,r0 */
L8c615a68: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615a6a; /* fadd fr3,fr2 */
L8c615a6a: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615a6c; /* fmov fr2,@(r0,r15) */
L8c615a6c: r[0]=8u;goto L8c615a6e; /* mov #0x8,r0 */
L8c615a6e: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c615a70; /* fmov.s @(r0,r11),fr3 */
L8c615a70: fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c615a72; /* fmov.s @(r0,r10),fr2 */
L8c615a72: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615a74; /* fadd fr3,fr2 */
L8c615a74: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c615a76; /* fmov.s @(r0,r12),fr3 */
L8c615a76: r[0]=72u;goto L8c615a78; /* mov #0x48,r0 */
L8c615a78: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615a7a; /* fadd fr3,fr2 */
L8c615a7a: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615a7c; /* fmov fr2,@(r0,r15) */
L8c615a7c: target=r[9];*pr=0x8c615a80u;r[4]+=64u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c615a80; /* jsr @r9 */
L8c615a80: r[4]=rd(m,bus,&io_status,0x8c615bd0u,4);if(io_status)return io_status;goto L8c615a82; /* mov.l 0x8c615bd0,r4 */
L8c615a82: r[0]=64u;goto L8c615a84; /* mov #0x40,r0 */
L8c615a84: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c615a86; /* fmov.s @(r0,r15),fr3 */
L8c615a86: r[0]=68u;goto L8c615a88; /* mov #0x44,r0 */
L8c615a88: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c615a8a; /* fmov.s @r4,fr2 */
L8c615a8a: r[5]=rd(m,bus,&io_status,0x8c615bccu,4);if(io_status)return io_status;goto L8c615a8c; /* mov.l 0x8c615bcc,r5 */
L8c615a8c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615a8e; /* fmul fr3,fr2 */
L8c615a8e: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c615a90; /* fmov.s @r5,fr3 */
L8c615a90: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615a92; /* fsub fr2,fr3 */
L8c615a92: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615a94; /* fmov.s @(r0,r15),fr2 */
L8c615a94: r[0]=4u;goto L8c615a96; /* mov #0x4,r0 */
L8c615a96: fload(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c615a98; /* fmov.s @(r0,r4),fr1 */
L8c615a98: r[0]=72u;goto L8c615a9a; /* mov #0x48,r0 */
L8c615a9a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615a9c; /* fmul fr2,fr1 */
L8c615a9c: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615a9e; /* fmov.s @(r0,r15),fr2 */
L8c615a9e: r[0]=8u;goto L8c615aa0; /* mov #0x8,r0 */
L8c615aa0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615aa2; /* fsub fr1,fr3 */
L8c615aa2: fload(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c615aa4; /* fmov.s @(r0,r4),fr1 */
L8c615aa4: r[4]=rd(m,bus,&io_status,0x8c615bd4u,4);if(io_status)return io_status;goto L8c615aa6; /* mov.l 0x8c615bd4,r4 */
L8c615aa6: r[0]=16u;goto L8c615aa8; /* mov #0x10,r0 */
L8c615aa8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615aaa; /* fmul fr2,fr1 */
L8c615aaa: fcopy(f,4u,3u);goto L8c615aac; /* fmov fr3,fr4 */
L8c615aac: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615aae; /* fmov.s @(r0,r4),fr3 */
L8c615aae: r[0]=48u;goto L8c615ab0; /* mov #0x30,r0 */
L8c615ab0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c615ab2; /* fsub fr1,fr4 */
L8c615ab2: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c615ab4; /* fmov fr3,@(r0,r15) */
L8c615ab4: r[0]=20u;goto L8c615ab6; /* mov #0x14,r0 */
L8c615ab6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615ab8; /* fmov.s @(r0,r4),fr3 */
L8c615ab8: r[0]=52u;goto L8c615aba; /* mov #0x34,r0 */
L8c615aba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615abc; /* fmul fr4,fr3 */
L8c615abc: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c615abe; /* fmov fr3,@(r0,r15) */
L8c615abe: r[0]=24u;goto L8c615ac0; /* mov #0x18,r0 */
L8c615ac0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615ac2; /* fmov.s @(r0,r4),fr3 */
L8c615ac2: r[0]=56u;goto L8c615ac4; /* mov #0x38,r0 */
L8c615ac4: r[7]=r[15];goto L8c615ac6; /* mov r15,r7 */
L8c615ac6: r[3]=rd(m,bus,&io_status,0x8c615bd8u,4);if(io_status)return io_status;goto L8c615ac8; /* mov.l 0x8c615bd8,r3 */
L8c615ac8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615aca; /* fmul fr4,fr3 */
L8c615aca: r[5]=r[11];goto L8c615acc; /* mov r11,r5 */
L8c615acc: r[6]=r[12];goto L8c615ace; /* mov r12,r6 */
L8c615ace: r[7]+=48u;goto L8c615ad0; /* add #0x30,r7 */
L8c615ad0: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c615ad2; /* fmov fr3,@(r0,r15) */
L8c615ad2: r[0]=28u;goto L8c615ad4; /* mov #0x1c,r0 */
L8c615ad4: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615ad6; /* fmov.s @(r0,r4),fr3 */
L8c615ad6: r[0]=60u;goto L8c615ad8; /* mov #0x3c,r0 */
L8c615ad8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615ada; /* fmul fr4,fr3 */
L8c615ada: fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c615adc; /* fmov fr3,@(r0,r15) */
L8c615adc: target=r[3];*pr=0x8c615ae0u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c615ae0; /* jsr @r3 */
L8c615ae0: r[9]=r[8];goto L8c615ae2; /* mov r8,r9 */
L8c615ae2: fcopy(f,6u,13u);goto L8c615ae4; /* fmov fr13,fr6 */
L8c615ae4: r[0]=16u;goto L8c615ae6; /* mov #0x10,r0 */
L8c615ae6: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c615ae8; /* mov.l @(0xc,r15),r3 */
L8c615ae8: r[10]=r[11];goto L8c615aea; /* mov r11,r10 */
L8c615aea: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c615aec; /* fmov.s @(r0,r12),fr13 */
L8c615aec: r[0]=20u;goto L8c615aee; /* mov #0x14,r0 */
L8c615aee: fcopy(f,5u,14u);goto L8c615af0; /* fmov fr14,fr5 */
L8c615af0: r[3]+=4294967295u;goto L8c615af2; /* add #-0x1,r3 */
L8c615af2: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c615af4; /* fmov.s @(r0,r12),fr14 */
L8c615af4: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c615af6; /* tst r3,r3 */
L8c615af6: r[11]=r[12];goto L8c615af8; /* mov r12,r11 */
L8c615af8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c615afa; /* fneg fr15 */
L8c615afa: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c615b02;goto L8c615afe; /* bt/s 0x8c615b02 */
L8c615afe: goto L8c6157d4; /* bra 0x8c6157d4 */
L8c615b02: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c615b04; /* cmp/hs r8,r9 */
L8c615b04: if((*sr&1u))goto L8c615b48;goto L8c615b06; /* bt 0x8c615b48 */
L8c615b06: r[5]=rd(m,bus,&io_status,0x8c615bbcu,4);if(io_status)return io_status;goto L8c615b08; /* mov.l 0x8c615bbc,r5 */
L8c615b08: r[4]=r[12];goto L8c615b0a; /* mov r12,r4 */
L8c615b0a: r[8]+=16u;goto L8c615b0c; /* add #0x10,r8 */
L8c615b0c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615b0e; /* fschg */
L8c615b0e: fcopy(f,0u,1u);goto L8c615b10; /* fmov fr1,fr0 */
L8c615b10: fcopy(f,2u,3u);goto L8c615b12; /* fmov fr3,fr2 */
L8c615b12: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b14; /* fmov.s @r4+,fr4 */
L8c615b14: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b16; /* fmov.s @r4+,fr6 */
L8c615b16: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615b18; /* fldi0 fr3 */
L8c615b18: status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c615b1a; /* fipr fv4,fv0 */
L8c615b1a: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b1c; /* fmov.s @r4+,fr8 */
L8c615b1c: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b1e; /* fmov.s @r4+,fr10 */
L8c615b1e: fcopy(f,0u,9u);goto L8c615b20; /* fmov fr9,fr0 */
L8c615b20: fcopy(f,4u,5u);goto L8c615b22; /* fmov fr5,fr4 */
L8c615b22: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c615b24; /* fsub fr3,fr0 */
L8c615b24: fcopy(f,6u,7u);goto L8c615b26; /* fmov fr7,fr6 */
L8c615b26: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615b28; /* fschg */
L8c615b28: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[8]);if(io_status)return io_status;goto L8c615b2a; /* fmov.s fr10,@-r8 */
L8c615b2a: r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c615b2c; /* fmov.s fr0,@-r4 */
L8c615b2c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],2u);if(status)return status;goto L8c615b2e; /* ocbwb @r4 */
L8c615b2e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c615b30; /* fmul fr0,fr7 */
L8c615b30: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c615b32; /* fmov.s fr9,@-r8 */
L8c615b32: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c615b34; /* fmul fr0,fr6 */
L8c615b34: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[8]);if(io_status)return io_status;goto L8c615b36; /* fmov.s fr8,@-r8 */
L8c615b36: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c615b38; /* fmul fr0,fr5 */
L8c615b38: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615b3a; /* mov.l r5,@-r8 */
L8c615b3a: r[8]+=32u;goto L8c615b3c; /* add #0x20,r8 */
L8c615b3c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c615b3e; /* fmov.s fr7,@-r8 */
L8c615b3e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615b40; /* fmov.s fr6,@-r8 */
L8c615b40: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615b42; /* fmov.s fr5,@-r8 */
L8c615b42: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615b44; /* fmov.s fr4,@-r8 */
L8c615b44: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615b46; /* pref @r8 */
L8c615b46: r[8]+=16u;goto L8c615b48; /* add #0x10,r8 */
L8c615b48: r[3]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c615b4a; /* mov.l @(0x18,r15),r3 */
L8c615b4a: r[3]+=4294967295u;goto L8c615b4c; /* add #-0x1,r3 */
L8c615b4c: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c615b4e; /* tst r3,r3 */
L8c615b4e: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;if(take)goto L8c615b56;goto L8c615b52; /* bt/s 0x8c615b56 */
L8c615b52: goto L8c615784; /* bra 0x8c615784 */
L8c615b56: r[4]=rd(m,bus,&io_status,0x8c615bdcu,4);if(io_status)return io_status;goto L8c615b58; /* mov.l 0x8c615bdc,r4 */
L8c615b58: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615b5a; /* mov.l @(0x8,r15),r3 */
L8c615b5a: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c615b5c; /* mov.l @r4,r2 */
L8c615b5c: r[2]+=r[3];goto L8c615b5e; /* add r3,r2 */
L8c615b5e: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c615b60; /* mov.l r2,@r4 */
L8c615b60: r[4]=rd(m,bus,&io_status,0x8c615be0u,4);if(io_status)return io_status;goto L8c615b62; /* mov.l 0x8c615be0,r4 */
L8c615b62: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615b64; /* mov.l @(0x8,r15),r3 */
L8c615b64: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c615b66; /* mov.l @r4,r2 */
L8c615b66: r[2]+=r[3];goto L8c615b68; /* add r3,r2 */
L8c615b68: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c615b6a; /* mov.l r2,@r4 */
L8c615b6a: r[15]+=76u;goto L8c615b6c; /* add #0x4c,r15 */
L8c615b6c: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615b6e; /* lds.l @r15+,PR */
L8c615b6e: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b70; /* fmov.s @r15+,fr12 */
L8c615b70: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b72; /* fmov.s @r15+,fr13 */
L8c615b72: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b74; /* fmov.s @r15+,fr14 */
L8c615b74: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615b76; /* fmov.s @r15+,fr15 */
L8c615b76: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615b78; /* mov.l @r15+,r9 */
L8c615b78: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615b7a; /* mov.l @r15+,r10 */
L8c615b7a: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615b7c; /* mov.l @r15+,r11 */
L8c615b7c: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615b7e; /* mov.l @r15+,r12 */
L8c615b7e: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615b80; /* mov.l @r15+,r13 */
L8c615b80: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
