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
L8c613be2: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c613be4; /* mov.l r14,@-r15 */
L8c613be4: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c613be6; /* mov.l r13,@-r15 */
L8c613be6: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c613be8; /* mov.l r12,@-r15 */
L8c613be8: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c613bea; /* mov.l r11,@-r15 */
L8c613bea: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c613bec; /* mov.l r10,@-r15 */
L8c613bec: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c613bee; /* mov.l r9,@-r15 */
L8c613bee: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c613bf0; /* fmov.s fr15,@-r15 */
L8c613bf0: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c613bf2; /* fmov.s fr14,@-r15 */
L8c613bf2: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c613bf4; /* fmov.s fr13,@-r15 */
L8c613bf4: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c613bf6; /* sts.l PR,@-r15 */
L8c613bf6: r[15]+=4294967280u;goto L8c613bf8; /* add #-0x10,r15 */
L8c613bf8: r[3]=rd(m,bus,&io_status,0x8c613c2cu,4);if(io_status)return io_status;goto L8c613bfa; /* mov.l 0x8c613c2c,r3 */
L8c613bfa: r[9]=rd(m,bus,&io_status,0x8c613c28u,4);if(io_status)return io_status;goto L8c613bfc; /* mov.l 0x8c613c28,r9 */
L8c613bfc: target=r[3];*pr=0x8c613c00u;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c613c00; /* jsr @r3 */
L8c613c00: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613c02; /* mov.l @(0x4,r15),r3 */
L8c613c02: r[2]=0u;goto L8c613c04; /* mov #0x0,r2 */
L8c613c04: r[14]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613c06; /* mov.l @(0x4,r15),r14 */
L8c613c06: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c613c08; /* mov.w @(0x2,r3),r0 */
L8c613c08: r[3]=rd(m,bus,&io_status,0x8c613c30u,4);if(io_status)return io_status;goto L8c613c0a; /* mov.l 0x8c613c30,r3 */
L8c613c0a: r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c613c0c; /* mov.l @(0x4,r14),r14 */
L8c613c0c: r[0]=r[0]&65535u;goto L8c613c0e; /* extu.w r0,r0 */
L8c613c0e: wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c613c10; /* mov.l r0,@(0xc,r15) */
L8c613c10: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c613c12; /* mov.l @r3,r13 */
L8c613c12: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613c14; /* mov.l r2,@(0x8,r15) */
L8c613c14: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c613c16; /* mov.l r2,@(0x4,r15) */
L8c613c16: goto L8c613c34; /* bra 0x8c613c34 */
L8c613c34: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613c36; /* mov.w @r14+,r3 */
L8c613c36: r[4]=rd(m,bus,&io_status,0x8c613d08u,4);if(io_status)return io_status;goto L8c613c38; /* mov.l 0x8c613d08,r4 */
L8c613c38: r[3]=r[3]&65535u;goto L8c613c3a; /* extu.w r3,r3 */
L8c613c3a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c613c3c; /* fldi1 fr4 */
L8c613c3c: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c613c3e; /* tst r4,r3 */
L8c613c3e: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c613c40; /* mov.l r3,@r15 */
L8c613c40: take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c613c52;goto L8c613c44; /* bt/s 0x8c613c52 */
L8c613c44: r[4]=sx(m,bus,&io_status,0x8c613d04u,2);if(io_status)return io_status;goto L8c613c46; /* mov.w 0x8c613d04,r4 */
L8c613c46: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613c48; /* mov.l @r15,r3 */
L8c613c48: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c613c4a; /* fldi1 fr4 */
L8c613c4a: r[3]&=r[4];goto L8c613c4c; /* and r4,r3 */
L8c613c4c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c613c4e; /* fneg fr4 */
L8c613c4e: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c613c50; /* mov.l r3,@r15 */
L8c613c50: fcopy(f,15u,4u);goto L8c613c52; /* fmov fr4,fr15 */
L8c613c52: r[0]=16u;goto L8c613c54; /* mov #0x10,r0 */
L8c613c54: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613c56; /* mov.l @r15,r3 */
L8c613c56: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613c58; /* mov.l @(0x4,r15),r2 */
L8c613c58: r[10]=r[8];goto L8c613c5a; /* mov r8,r10 */
L8c613c5a: r[2]+=r[3];goto L8c613c5c; /* add r3,r2 */
L8c613c5c: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c613c5e; /* mov.l r2,@(0x4,r15) */
L8c613c5e: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613c60; /* mov.l @r15,r1 */
L8c613c60: r[1]+=4294967294u;goto L8c613c62; /* add #-0x2,r1 */
L8c613c62: wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c613c64; /* mov.l r1,@r15 */
L8c613c64: r[4]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613c66; /* mov.w @r14+,r4 */
L8c613c66: r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613c68; /* mov.w @r14+,r11 */
L8c613c68: r[4]=r[4]&65535u;goto L8c613c6a; /* extu.w r4,r4 */
L8c613c6a: r[4]<<=2;goto L8c613c6c; /* shll2 r4 */
L8c613c6c: r[11]=r[11]&65535u;goto L8c613c6e; /* extu.w r11,r11 */
L8c613c6e: r[11]<<=2;goto L8c613c70; /* shll2 r11 */
L8c613c70: r[4]<<=2;goto L8c613c72; /* shll2 r4 */
L8c613c72: r[11]<<=2;goto L8c613c74; /* shll2 r11 */
L8c613c74: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c613c76; /* shll r4 */
L8c613c76: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c613c78; /* shll r11 */
L8c613c78: r[11]+=r[13];goto L8c613c7a; /* add r13,r11 */
L8c613c7a: r[4]+=r[13];goto L8c613c7c; /* add r13,r4 */
L8c613c7c: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c613c7e; /* fmov.s @(r0,r11),fr14 */
L8c613c7e: fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c613c80; /* fmov.s @(r0,r4),fr4 */
L8c613c80: r[0]=20u;goto L8c613c82; /* mov #0x14,r0 */
L8c613c82: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c613c84; /* fmov.s @(r0,r11),fr13 */
L8c613c84: fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c613c86; /* fmov.s @(r0,r4),fr5 */
L8c613c86: r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c613c88; /* mov.w @r14+,r12 */
L8c613c88: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c613c8a; /* mov.w @r14,r3 */
L8c613c8a: r[12]=r[12]&65535u;goto L8c613c8c; /* extu.w r12,r12 */
L8c613c8c: r[12]<<=2;goto L8c613c8e; /* shll2 r12 */
L8c613c8e: r[3]=r[3]&65535u;goto L8c613c90; /* extu.w r3,r3 */
L8c613c90: r[3]<<=2;goto L8c613c92; /* shll2 r3 */
L8c613c92: r[12]<<=2;goto L8c613c94; /* shll2 r12 */
L8c613c94: r[3]<<=2;goto L8c613c96; /* shll2 r3 */
L8c613c96: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c613c98; /* shll r12 */
L8c613c98: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c613c9a; /* shll r3 */
L8c613c9a: r[3]+=r[13];goto L8c613c9c; /* add r13,r3 */
L8c613c9c: r[12]+=r[13];goto L8c613c9e; /* add r13,r12 */
L8c613c9e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c613ca0; /* pref @r3 */
L8c613ca0: r[0]=20u;goto L8c613ca2; /* mov #0x14,r0 */
L8c613ca2: fcopy(f,3u,14u);goto L8c613ca4; /* fmov fr14,fr3 */
L8c613ca4: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c613ca6; /* fmov.s @(r0,r12),fr2 */
L8c613ca6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613ca8; /* fsub fr4,fr3 */
L8c613ca8: r[0]=16u;goto L8c613caa; /* mov #0x10,r0 */
L8c613caa: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c613cac; /* cmp/hs r8,r10 */
L8c613cac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613cae; /* fsub fr13,fr2 */
L8c613cae: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c613cb0; /* fmov.s @(r0,r12),fr1 */
L8c613cb0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c613cb2; /* fsub fr14,fr1 */
L8c613cb2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c613cb4; /* fmul fr2,fr3 */
L8c613cb4: fcopy(f,2u,13u);goto L8c613cb6; /* fmov fr13,fr2 */
L8c613cb6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613cb8; /* fsub fr5,fr2 */
L8c613cb8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613cba; /* fmul fr1,fr2 */
L8c613cba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613cbc; /* fsub fr2,fr3 */
L8c613cbc: fcopy(f,4u,3u);goto L8c613cbe; /* fmov fr3,fr4 */
L8c613cbe: take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c613d0c;goto L8c613cc2; /* bt/s 0x8c613d0c */
L8c613cc2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613cc4; /* fldi0 fr3 */
L8c613cc4: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c613cc6; /* fcmp/gt fr4,fr3 */
L8c613cc6: if((*sr&1u))goto L8c613d3a;goto L8c613cc8; /* bt 0x8c613d3a */
L8c613cc8: r[4]=r[11];goto L8c613cca; /* mov r11,r4 */
L8c613cca: r[5]=r[9];goto L8c613ccc; /* mov r9,r5 */
L8c613ccc: r[8]+=16u;goto L8c613cce; /* add #0x10,r8 */
L8c613cce: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613cd0; /* fschg */
L8c613cd0: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613cd2; /* fmov.s @r4+,fr0 */
L8c613cd2: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613cd4; /* fmov.s @r4+,fr2 */
L8c613cd4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c613cd6; /* fldi1 fr3 */
L8c613cd6: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c613cd8; /* ftrv xmtrx,fv0 */
L8c613cd8: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613cda; /* fmov.s @r4+,fr4 */
L8c613cda: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613cdc; /* fmov.s @r4,fr6 */
L8c613cdc: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613cde; /* fschg */
L8c613cde: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613ce0; /* fmov.s fr6,@-r8 */
L8c613ce0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613ce2; /* fmov.s fr5,@-r8 */
L8c613ce2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613ce4; /* fmov.s fr4,@-r8 */
L8c613ce4: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613ce6; /* mov.l r5,@-r8 */
L8c613ce6: r[8]+=28u;goto L8c613ce8; /* add #0x1c,r8 */
L8c613ce8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613cea; /* fmov.s fr2,@-r8 */
L8c613cea: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613cec; /* fmov.s fr1,@-r8 */
L8c613cec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c613cee; /* fmov.s fr0,@-r8 */
L8c613cee: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613cf0; /* pref @r8 */
L8c613cf0: r[8]+=16u;goto L8c613cf2; /* add #0x10,r8 */
L8c613cf2: r[3]=r[8];goto L8c613cf4; /* mov r8,r3 */
L8c613cf4: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613cf6; /* mov.l @(0x8,r15),r2 */
L8c613cf6: r[3]-=r[10];goto L8c613cf8; /* sub r10,r3 */
L8c613cf8: r[3]+=32u;goto L8c613cfa; /* add #0x20,r3 */
L8c613cfa: r[2]+=r[3];goto L8c613cfc; /* add r3,r2 */
L8c613cfc: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613cfe; /* mov.l r2,@(0x8,r15) */
L8c613cfe: r[10]=r[8];goto L8c613d64; /* bra 0x8c613d64 */
L8c613d0c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613d0e; /* fldi0 fr3 */
L8c613d0e: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c613d10; /* fcmp/gt fr4,fr3 */
L8c613d10: if(!(*sr&1u))goto L8c613d64;goto L8c613d12; /* bf 0x8c613d64 */
L8c613d12: r[5]=rd(m,bus,&io_status,0x8c613e34u,4);if(io_status)return io_status;goto L8c613d14; /* mov.l 0x8c613e34,r5 */
L8c613d14: r[8]+=16u;goto L8c613d16; /* add #0x10,r8 */
L8c613d16: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613d18; /* fschg */
L8c613d18: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d1a; /* fmov.s @r4+,fr0 */
L8c613d1a: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d1c; /* fmov.s @r4+,fr2 */
L8c613d1c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c613d1e; /* fldi1 fr3 */
L8c613d1e: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c613d20; /* ftrv xmtrx,fv0 */
L8c613d20: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d22; /* fmov.s @r4+,fr4 */
L8c613d22: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613d24; /* fmov.s @r4,fr6 */
L8c613d24: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613d26; /* fschg */
L8c613d26: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613d28; /* fmov.s fr6,@-r8 */
L8c613d28: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613d2a; /* fmov.s fr5,@-r8 */
L8c613d2a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613d2c; /* fmov.s fr4,@-r8 */
L8c613d2c: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613d2e; /* mov.l r5,@-r8 */
L8c613d2e: r[8]+=28u;goto L8c613d30; /* add #0x1c,r8 */
L8c613d30: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613d32; /* fmov.s fr2,@-r8 */
L8c613d32: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613d34; /* fmov.s fr1,@-r8 */
L8c613d34: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c613d36; /* fmov.s fr0,@-r8 */
L8c613d36: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613d38; /* pref @r8 */
L8c613d38: r[8]+=16u;goto L8c613d3a; /* add #0x10,r8 */
L8c613d3a: r[5]=rd(m,bus,&io_status,0x8c613e34u,4);if(io_status)return io_status;goto L8c613d3c; /* mov.l 0x8c613e34,r5 */
L8c613d3c: r[4]=r[11];goto L8c613d3e; /* mov r11,r4 */
L8c613d3e: r[8]+=16u;goto L8c613d40; /* add #0x10,r8 */
L8c613d40: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613d42; /* fschg */
L8c613d42: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d44; /* fmov.s @r4+,fr0 */
L8c613d44: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d46; /* fmov.s @r4+,fr2 */
L8c613d46: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c613d48; /* fldi1 fr3 */
L8c613d48: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c613d4a; /* ftrv xmtrx,fv0 */
L8c613d4a: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d4c; /* fmov.s @r4+,fr4 */
L8c613d4c: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613d4e; /* fmov.s @r4,fr6 */
L8c613d4e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613d50; /* fschg */
L8c613d50: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613d52; /* fmov.s fr6,@-r8 */
L8c613d52: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613d54; /* fmov.s fr5,@-r8 */
L8c613d54: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613d56; /* fmov.s fr4,@-r8 */
L8c613d56: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613d58; /* mov.l r5,@-r8 */
L8c613d58: r[8]+=28u;goto L8c613d5a; /* add #0x1c,r8 */
L8c613d5a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613d5c; /* fmov.s fr2,@-r8 */
L8c613d5c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613d5e; /* fmov.s fr1,@-r8 */
L8c613d5e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c613d60; /* fmov.s fr0,@-r8 */
L8c613d60: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613d62; /* pref @r8 */
L8c613d62: r[8]+=16u;goto L8c613d64; /* add #0x10,r8 */
L8c613d64: fcopy(f,4u,14u);goto L8c613d66; /* fmov fr14,fr4 */
L8c613d66: r[0]=16u;goto L8c613d68; /* mov #0x10,r0 */
L8c613d68: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613d6a; /* mov.l @r15,r3 */
L8c613d6a: r[4]=r[11];goto L8c613d6c; /* mov r11,r4 */
L8c613d6c: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c613d6e; /* fmov.s @(r0,r12),fr14 */
L8c613d6e: r[0]=20u;goto L8c613d70; /* mov #0x14,r0 */
L8c613d70: fcopy(f,5u,13u);goto L8c613d72; /* fmov fr13,fr5 */
L8c613d72: r[3]+=4294967295u;goto L8c613d74; /* add #-0x1,r3 */
L8c613d74: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c613d76; /* fmov.s @(r0,r12),fr13 */
L8c613d76: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c613d78; /* tst r3,r3 */
L8c613d78: r[11]=r[12];goto L8c613d7a; /* mov r12,r11 */
L8c613d7a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c613d7c; /* fneg fr15 */
L8c613d7c: take=!(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c613c86;goto L8c613d80; /* bf/s 0x8c613c86 */
L8c613d80: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c613d82; /* cmp/hs r8,r10 */
L8c613d82: if((*sr&1u))goto L8c613dba;goto L8c613d84; /* bt 0x8c613dba */
L8c613d84: r[4]=r[12];goto L8c613d86; /* mov r12,r4 */
L8c613d86: r[5]=r[9];goto L8c613d88; /* mov r9,r5 */
L8c613d88: r[8]+=16u;goto L8c613d8a; /* add #0x10,r8 */
L8c613d8a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613d8c; /* fschg */
L8c613d8c: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d8e; /* fmov.s @r4+,fr0 */
L8c613d8e: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d90; /* fmov.s @r4+,fr2 */
L8c613d90: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c613d92; /* fldi1 fr3 */
L8c613d92: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c613d94; /* ftrv xmtrx,fv0 */
L8c613d94: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613d96; /* fmov.s @r4+,fr4 */
L8c613d96: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613d98; /* fmov.s @r4,fr6 */
L8c613d98: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613d9a; /* fschg */
L8c613d9a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613d9c; /* fmov.s fr6,@-r8 */
L8c613d9c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613d9e; /* fmov.s fr5,@-r8 */
L8c613d9e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613da0; /* fmov.s fr4,@-r8 */
L8c613da0: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613da2; /* mov.l r5,@-r8 */
L8c613da2: r[8]+=28u;goto L8c613da4; /* add #0x1c,r8 */
L8c613da4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613da6; /* fmov.s fr2,@-r8 */
L8c613da6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613da8; /* fmov.s fr1,@-r8 */
L8c613da8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c613daa; /* fmov.s fr0,@-r8 */
L8c613daa: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613dac; /* pref @r8 */
L8c613dac: r[8]+=16u;goto L8c613dae; /* add #0x10,r8 */
L8c613dae: r[3]=r[8];goto L8c613db0; /* mov r8,r3 */
L8c613db0: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613db2; /* mov.l @(0x8,r15),r2 */
L8c613db2: r[3]-=r[10];goto L8c613db4; /* sub r10,r3 */
L8c613db4: r[3]+=32u;goto L8c613db6; /* add #0x20,r3 */
L8c613db6: r[2]+=r[3];goto L8c613db8; /* add r3,r2 */
L8c613db8: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613dba; /* mov.l r2,@(0x8,r15) */
L8c613dba: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c613dbc; /* mov.l @(0xc,r15),r3 */
L8c613dbc: r[3]+=4294967295u;goto L8c613dbe; /* add #-0x1,r3 */
L8c613dbe: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c613dc0; /* tst r3,r3 */
L8c613dc0: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c613dc8;goto L8c613dc4; /* bt/s 0x8c613dc8 */
L8c613dc4: goto L8c613c34; /* bra 0x8c613c34 */
L8c613dc8: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613dca; /* mov.l @(0x8,r15),r3 */
L8c613dca: r[4]=rd(m,bus,&io_status,0x8c613e38u,4);if(io_status)return io_status;goto L8c613dcc; /* mov.l 0x8c613e38,r4 */
L8c613dcc: r[3]>>=2;goto L8c613dce; /* shlr2 r3 */
L8c613dce: r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c613dd0; /* mov.l @r4,r1 */
L8c613dd0: r[3]>>=2;goto L8c613dd2; /* shlr2 r3 */
L8c613dd2: *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c613dd4; /* shlr r3 */
L8c613dd4: r[1]+=r[3];goto L8c613dd6; /* add r3,r1 */
L8c613dd6: wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c613dd8; /* mov.l r1,@r4 */
L8c613dd8: r[4]=rd(m,bus,&io_status,0x8c613e3cu,4);if(io_status)return io_status;goto L8c613dda; /* mov.l 0x8c613e3c,r4 */
L8c613dda: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613ddc; /* mov.l @(0x4,r15),r3 */
L8c613ddc: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c613dde; /* mov.l @r4,r2 */
L8c613dde: r[2]+=r[3];goto L8c613de0; /* add r3,r2 */
L8c613de0: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c613de2; /* mov.l r2,@r4 */
L8c613de2: r[15]+=16u;goto L8c613de4; /* add #0x10,r15 */
L8c613de4: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613de6; /* lds.l @r15+,PR */
L8c613de6: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613de8; /* fmov.s @r15+,fr13 */
L8c613de8: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613dea; /* fmov.s @r15+,fr14 */
L8c613dea: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613dec; /* fmov.s @r15+,fr15 */
L8c613dec: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613dee; /* mov.l @r15+,r9 */
L8c613dee: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613df0; /* mov.l @r15+,r10 */
L8c613df0: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613df2; /* mov.l @r15+,r11 */
L8c613df2: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613df4; /* mov.l @r15+,r12 */
L8c613df4: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613df6; /* mov.l @r15+,r13 */
L8c613df6: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_environment_alternate_strip_clipped_8c6153bc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6153bc;
L8c6153bc: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6153be; /* mov.l r14,@-r15 */
L8c6153be: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6153c0; /* mov.l r13,@-r15 */
L8c6153c0: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6153c2; /* mov.l r12,@-r15 */
L8c6153c2: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6153c4; /* mov.l r11,@-r15 */
L8c6153c4: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6153c6; /* mov.l r10,@-r15 */
L8c6153c6: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6153c8; /* mov.l r9,@-r15 */
L8c6153c8: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c6153ca; /* fmov.s fr15,@-r15 */
L8c6153ca: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c6153cc; /* fmov.s fr14,@-r15 */
L8c6153cc: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c6153ce; /* fmov.s fr13,@-r15 */
L8c6153ce: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c6153d0; /* fmov.s fr12,@-r15 */
L8c6153d0: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6153d2; /* sts.l PR,@-r15 */
L8c6153d2: r[15]+=4294967220u;goto L8c6153d4; /* add #-0x4c,r15 */
L8c6153d4: r[3]=rd(m,bus,&io_status,0x8c615410u,4);if(io_status)return io_status;goto L8c6153d6; /* mov.l 0x8c615410,r3 */
L8c6153d6: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c6153d8; /* fldi1 fr12 */
L8c6153d8: target=r[3];*pr=0x8c6153dcu;wr(m,bus,&io_status,r[15]+0xcu,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6153dc; /* jsr @r3 */
L8c6153dc: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6153de; /* mov.l @(0xc,r15),r3 */
L8c6153de: r[2]=0u;goto L8c6153e0; /* mov #0x0,r2 */
L8c6153e0: r[14]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6153e2; /* mov.l @(0xc,r15),r14 */
L8c6153e2: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6153e4; /* mov.w @(0x2,r3),r0 */
L8c6153e4: r[3]=rd(m,bus,&io_status,0x8c615414u,4);if(io_status)return io_status;goto L8c6153e6; /* mov.l 0x8c615414,r3 */
L8c6153e6: r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c6153e8; /* mov.l @(0x4,r14),r14 */
L8c6153e8: r[0]=r[0]&65535u;goto L8c6153ea; /* extu.w r0,r0 */
L8c6153ea: wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c6153ec; /* mov.l r0,@(0x18,r15) */
L8c6153ec: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6153ee; /* mov.l @r3,r13 */
L8c6153ee: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6153f0; /* mov.l r2,@(0xc,r15) */
L8c6153f0: goto L8c615418; /* bra 0x8c615418 */
L8c615418: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c61541a; /* mov.w @r14+,r3 */
L8c61541a: r[4]=rd(m,bus,&io_status,0x8c615534u,4);if(io_status)return io_status;goto L8c61541c; /* mov.l 0x8c615534,r4 */
L8c61541c: r[3]=r[3]&65535u;goto L8c61541e; /* extu.w r3,r3 */
L8c61541e: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c615420; /* tst r4,r3 */
L8c615420: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c615422; /* mov.l r3,@(0x8,r15) */
L8c615422: take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c615434;goto L8c615426; /* bt/s 0x8c615434 */
L8c615426: r[4]=sx(m,bus,&io_status,0x8c615530u,2);if(io_status)return io_status;goto L8c615428; /* mov.w 0x8c615530,r4 */
L8c615428: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61542a; /* mov.l @(0x8,r15),r3 */
L8c61542a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c61542c; /* fldi1 fr4 */
L8c61542c: r[3]&=r[4];goto L8c61542e; /* and r4,r3 */
L8c61542e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c615430; /* fneg fr4 */
L8c615430: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c615432; /* mov.l r3,@(0x8,r15) */
L8c615432: fcopy(f,15u,4u);goto L8c615434; /* fmov fr4,fr15 */
L8c615434: r[0]=16u;goto L8c615436; /* mov #0x10,r0 */
L8c615436: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615438; /* mov.l @(0x8,r15),r3 */
L8c615438: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61543a; /* mov.l @(0xc,r15),r2 */
L8c61543a: r[9]=r[8];goto L8c61543c; /* mov r8,r9 */
L8c61543c: r[2]+=r[3];goto L8c61543e; /* add r3,r2 */
L8c61543e: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c615440; /* mov.l r2,@(0xc,r15) */
L8c615440: r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615442; /* mov.l @(0x8,r15),r1 */
L8c615442: r[1]+=4294967294u;goto L8c615444; /* add #-0x2,r1 */
L8c615444: wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c615446; /* mov.l r1,@(0x8,r15) */
L8c615446: r[10]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c615448; /* mov.w @r14+,r10 */
L8c615448: r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c61544a; /* mov.w @r14+,r11 */
L8c61544a: r[10]=r[10]&65535u;goto L8c61544c; /* extu.w r10,r10 */
L8c61544c: r[10]<<=2;goto L8c61544e; /* shll2 r10 */
L8c61544e: r[11]=r[11]&65535u;goto L8c615450; /* extu.w r11,r11 */
L8c615450: r[11]<<=2;goto L8c615452; /* shll2 r11 */
L8c615452: r[10]<<=2;goto L8c615454; /* shll2 r10 */
L8c615454: r[11]<<=2;goto L8c615456; /* shll2 r11 */
L8c615456: *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c615458; /* shll r10 */
L8c615458: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61545a; /* shll r11 */
L8c61545a: r[11]+=r[13];goto L8c61545c; /* add r13,r11 */
L8c61545c: r[10]+=r[13];goto L8c61545e; /* add r13,r10 */
L8c61545e: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c615460; /* fmov.s @(r0,r11),fr13 */
L8c615460: fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c615462; /* fmov.s @(r0,r10),fr6 */
L8c615462: r[0]=20u;goto L8c615464; /* mov #0x14,r0 */
L8c615464: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c615466; /* fmov.s @(r0,r11),fr14 */
L8c615466: fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c615468; /* fmov.s @(r0,r10),fr5 */
L8c615468: r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c61546a; /* mov.w @r14+,r12 */
L8c61546a: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c61546c; /* mov.w @r14,r3 */
L8c61546c: r[12]=r[12]&65535u;goto L8c61546e; /* extu.w r12,r12 */
L8c61546e: r[12]<<=2;goto L8c615470; /* shll2 r12 */
L8c615470: r[3]=r[3]&65535u;goto L8c615472; /* extu.w r3,r3 */
L8c615472: r[3]<<=2;goto L8c615474; /* shll2 r3 */
L8c615474: r[12]<<=2;goto L8c615476; /* shll2 r12 */
L8c615476: r[3]<<=2;goto L8c615478; /* shll2 r3 */
L8c615478: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61547a; /* shll r12 */
L8c61547a: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c61547c; /* shll r3 */
L8c61547c: r[3]+=r[13];goto L8c61547e; /* add r13,r3 */
L8c61547e: r[12]+=r[13];goto L8c615480; /* add r13,r12 */
L8c615480: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c615482; /* pref @r3 */
L8c615482: r[2]=0u;goto L8c615484; /* mov #0x0,r2 */
L8c615484: wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c615486; /* mov.l r2,@(0x14,r15) */
L8c615486: r[4]=63u;goto L8c615488; /* mov #0x3f,r4 */
L8c615488: r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c61548a; /* mov.l @(0xc,r10),r2 */
L8c61548a: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61548c; /* mov.l @(0xc,r11),r3 */
L8c61548c: r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61548e; /* mov.l @(0xc,r12),r1 */
L8c61548e: r[2]&=r[3];goto L8c615490; /* and r3,r2 */
L8c615490: r[2]&=r[1];goto L8c615492; /* and r1,r2 */
L8c615492: r[4]&=r[2];goto L8c615494; /* and r2,r4 */
L8c615494: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c615496; /* tst r4,r4 */
L8c615496: take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c61549e;goto L8c61549a; /* bt/s 0x8c61549e */
L8c61549a: goto L8c6155f8; /* bra 0x8c6155f8 */
L8c61549e: r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c6154a0; /* mov.l @(0xc,r10),r4 */
L8c6154a0: r[5]=1u;goto L8c6154a2; /* mov #0x1,r5 */
L8c6154a2: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c6154a4; /* mov.l @(0xc,r11),r3 */
L8c6154a4: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c6154a6; /* mov.l @(0xc,r12),r2 */
L8c6154a6: r[4]|=r[3];goto L8c6154a8; /* or r3,r4 */
L8c6154a8: r[4]|=r[2];goto L8c6154aa; /* or r2,r4 */
L8c6154aa: r[4]&=r[5];goto L8c6154ac; /* and r5,r4 */
L8c6154ac: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6154ae; /* tst r4,r4 */
L8c6154ae: if(!(*sr&1u))goto L8c6154b4;goto L8c6154b0; /* bf 0x8c6154b4 */
L8c6154b0: goto L8c6155da; /* bra 0x8c6155da */
L8c6154b4: r[7]=r[15];goto L8c6154b6; /* mov r15,r7 */
L8c6154b6: r[7]+=64u;goto L8c6154b8; /* add #0x40,r7 */
L8c6154b8: wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c6154ba; /* mov.l r10,@r7 */
L8c6154ba: wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c6154bc; /* mov.l r11,@(0x4,r7) */
L8c6154bc: wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c6154be; /* mov.l r12,@(0x8,r7) */
L8c6154be: r[6]=0u;goto L8c61556c; /* bra 0x8c61556c */
L8c6154c2: r[2]=r[6];goto L8c6154c4; /* mov r6,r2 */
L8c6154c4: r[1]=r[6];goto L8c6154c6; /* mov r6,r1 */
L8c6154c6: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6154c8; /* shll r2 */
L8c6154c8: r[3]=r[6];goto L8c6154ca; /* mov r6,r3 */
L8c6154ca: r[3]<<=2;goto L8c6154cc; /* shll2 r3 */
L8c6154cc: r[2]+=r[1];goto L8c6154ce; /* add r1,r2 */
L8c6154ce: r[2]<<=2;goto L8c6154d0; /* shll2 r2 */
L8c6154d0: r[3]+=r[7];goto L8c6154d2; /* add r7,r3 */
L8c6154d2: r[2]+=r[4];goto L8c6154d4; /* add r4,r2 */
L8c6154d4: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6154d6; /* mov.l r3,@(0x4,r15) */
L8c6154d6: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6154d8; /* mov.l r2,@r15 */
L8c6154d8: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6154da; /* mov.l @r3,r1 */
L8c6154da: r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c6154dc; /* mov.l @(0xc,r1),r2 */
L8c6154dc: *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c6154de; /* tst r5,r2 */
L8c6154de: take=!(*sr&1u);r[6]+=1u;if(take)goto L8c615544;goto L8c6154e2; /* bf/s 0x8c615544 */
L8c6154e2: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6154e4; /* mov.l @(0x4,r15),r1 */
L8c6154e4: r[0]=24u;goto L8c6154e6; /* mov #0x18,r0 */
L8c6154e6: fcopy(f,4u,12u);goto L8c6154e8; /* fmov fr12,fr4 */
L8c6154e8: r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c6154ea; /* mov.l @r1,r3 */
L8c6154ea: r[4]=rd(m,bus,&io_status,0x8c615538u,4);if(io_status)return io_status;goto L8c6154ec; /* mov.l 0x8c615538,r4 */
L8c6154ec: wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6154ee; /* mov.l r3,@(0x10,r15) */
L8c6154ee: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6154f0; /* fmov.s @(r0,r3),fr3 */
L8c6154f0: r[0]=12u;goto L8c6154f2; /* mov #0xc,r0 */
L8c6154f2: r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c6154f4; /* mov.l @(0x10,r15),r2 */
L8c6154f4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6154f6; /* fdiv fr3,fr4 */
L8c6154f6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6154f8; /* fmov.s @(r0,r4),fr3 */
L8c6154f8: r[0]=16u;goto L8c6154fa; /* mov #0x10,r0 */
L8c6154fa: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6154fc; /* mov.l @r15,r3 */
L8c6154fc: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6154fe; /* fmov.s @(r0,r2),fr2 */
L8c6154fe: r[0]=16u;goto L8c615500; /* mov #0x10,r0 */
L8c615500: r[2]=rd(m,bus,&io_status,0x8c61553cu,4);if(io_status)return io_status;goto L8c615502; /* mov.l 0x8c61553c,r2 */
L8c615502: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615504; /* fsub fr3,fr2 */
L8c615504: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c615506; /* fmov.s @r2,fr3 */
L8c615506: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615508; /* fmul fr4,fr2 */
L8c615508: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61550a; /* fdiv fr3,fr2 */
L8c61550a: fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61550c; /* fmov.s fr2,@r3 */
L8c61550c: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61550e; /* mov.l @(0x4,r15),r1 */
L8c61550e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615510; /* fmov.s @(r0,r4),fr3 */
L8c615510: r[0]=20u;goto L8c615512; /* mov #0x14,r0 */
L8c615512: r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c615514; /* mov.l @r1,r1 */
L8c615514: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615516; /* mov.l @r15,r3 */
L8c615516: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c615518; /* fmov.s @(r0,r1),fr2 */
L8c615518: r[1]=rd(m,bus,&io_status,0x8c615540u,4);if(io_status)return io_status;goto L8c61551a; /* mov.l 0x8c615540,r1 */
L8c61551a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61551c; /* fsub fr3,fr2 */
L8c61551c: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c61551e; /* fmov.s @r1,fr3 */
L8c61551e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615520; /* fmul fr4,fr2 */
L8c615520: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615522; /* fdiv fr3,fr2 */
L8c615522: r[0]=4u;goto L8c615524; /* mov #0x4,r0 */
L8c615524: fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c615526; /* fmov fr2,@(r0,r3) */
L8c615526: r[0]=8u;goto L8c615528; /* mov #0x8,r0 */
L8c615528: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61552a; /* mov.l @r15,r3 */
L8c61552a: fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c61552c; /* fmov fr4,@(r0,r3) */
L8c61552c: goto L8c61556c; /* bra 0x8c61556c */
L8c615544: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615546; /* mov.l @(0x4,r15),r3 */
L8c615546: r[0]=16u;goto L8c615548; /* mov #0x10,r0 */
L8c615548: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61554a; /* mov.l @r15,r2 */
L8c61554a: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61554c; /* mov.l @r3,r1 */
L8c61554c: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61554e; /* fmov.s @(r0,r1),fr3 */
L8c61554e: r[0]=20u;goto L8c615550; /* mov #0x14,r0 */
L8c615550: fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c615552; /* fmov.s fr3,@r2 */
L8c615552: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615554; /* mov.l @(0x4,r15),r2 */
L8c615554: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615556; /* mov.l @r15,r3 */
L8c615556: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c615558; /* mov.l @r2,r1 */
L8c615558: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61555a; /* fmov.s @(r0,r1),fr3 */
L8c61555a: r[0]=4u;goto L8c61555c; /* mov #0x4,r0 */
L8c61555c: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61555e; /* fmov fr3,@(r0,r3) */
L8c61555e: r[0]=24u;goto L8c615560; /* mov #0x18,r0 */
L8c615560: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615562; /* mov.l @(0x4,r15),r2 */
L8c615562: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615564; /* mov.l @r15,r3 */
L8c615564: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c615566; /* mov.l @r2,r1 */
L8c615566: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c615568; /* fmov.s @(r0,r1),fr3 */
L8c615568: r[0]=8u;goto L8c61556a; /* mov #0x8,r0 */
L8c61556a: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61556c; /* fmov fr3,@(r0,r3) */
L8c61556c: r[3]=3u;goto L8c61556e; /* mov #0x3,r3 */
L8c61556e: r[4]=r[15];goto L8c615570; /* mov r15,r4 */
L8c615570: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c615572; /* cmp/ge r3,r6 */
L8c615572: take=!(*sr&1u);r[4]+=28u;if(take)goto L8c6154c2;goto L8c615576; /* bf/s 0x8c6154c2 */
L8c615576: r[6]=r[4];goto L8c615578; /* mov r4,r6 */
L8c615578: r[0]=8u;goto L8c61557a; /* mov #0x8,r0 */
L8c61557a: r[3]=r[4];goto L8c61557c; /* mov r4,r3 */
L8c61557c: fload(m,bus,&io_status,f,6u,r[0]+r[6]);if(io_status)return io_status;goto L8c61557e; /* fmov.s @(r0,r6),fr6 */
L8c61557e: r[3]+=12u;goto L8c615580; /* add #0xc,r3 */
L8c615580: r[7]=r[4];goto L8c615582; /* mov r4,r7 */
L8c615582: r[0]=4u;goto L8c615584; /* mov #0x4,r0 */
L8c615584: fcopy(f,0u,6u);goto L8c615586; /* fmov fr6,fr0 */
L8c615586: r[7]+=24u;goto L8c615588; /* add #0x18,r7 */
L8c615588: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61558a; /* mov.l r3,@r15 */
L8c61558a: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c61558c; /* fmov.s @(r0,r6),fr4 */
L8c61558c: fload(m,bus,&io_status,f,5u,r[0]+r[3]);if(io_status)return io_status;goto L8c61558e; /* fmov.s @(r0,r3),fr5 */
L8c61558e: r[0]=8u;goto L8c615590; /* mov #0x8,r0 */
L8c615590: fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c615592; /* fmov.s @(r0,r3),fr9 */
L8c615592: r[0]=4u;goto L8c615594; /* mov #0x4,r0 */
L8c615594: fcopy(f,3u,4u);goto L8c615596; /* fmov fr4,fr3 */
L8c615596: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c615598; /* fmul fr5,fr0 */
L8c615598: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61559a; /* fmul fr9,fr3 */
L8c61559a: fload(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;goto L8c61559c; /* fmov.s @r6,fr8 */
L8c61559c: fload(m,bus,&io_status,f,7u,r[3]);if(io_status)return io_status;goto L8c61559e; /* fmov.s @r3,fr7 */
L8c61559e: fcopy(f,2u,8u);goto L8c6155a0; /* fmov fr8,fr2 */
L8c6155a0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[9],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6155a2; /* fmul fr9,fr2 */
L8c6155a2: fcopy(f,1u,4u);goto L8c6155a4; /* fmov fr4,fr1 */
L8c6155a4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6155a6; /* fsub fr3,fr0 */
L8c6155a6: fcopy(f,3u,6u);goto L8c6155a8; /* fmov fr6,fr3 */
L8c6155a8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6155aa; /* fmul fr7,fr3 */
L8c6155aa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6155ac; /* fsub fr3,fr2 */
L8c6155ac: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6155ae; /* fmov.s @(r0,r7),fr3 */
L8c6155ae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6155b0; /* fmul fr2,fr3 */
L8c6155b0: fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c6155b2; /* fmov.s @r7,fr2 */
L8c6155b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6155b4; /* fmac fr0,fr2,fr3 */
L8c6155b4: fcopy(f,2u,3u);goto L8c6155b6; /* fmov fr3,fr2 */
L8c6155b6: fcopy(f,3u,8u);goto L8c6155b8; /* fmov fr8,fr3 */
L8c6155b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6155ba; /* fmul fr5,fr3 */
L8c6155ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6155bc; /* fmul fr7,fr1 */
L8c6155bc: r[0]=8u;goto L8c6155be; /* mov #0x8,r0 */
L8c6155be: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6155c0; /* fsub fr3,fr1 */
L8c6155c0: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6155c2; /* fmov.s @(r0,r7),fr3 */
L8c6155c2: fcopy(f,0u,1u);goto L8c6155c4; /* fmov fr1,fr0 */
L8c6155c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6155c6; /* fmac fr0,fr3,fr2 */
L8c6155c6: fcopy(f,3u,15u);goto L8c6155c8; /* fmov fr15,fr3 */
L8c6155c8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c6155ca; /* fneg fr3 */
L8c6155ca: fcopy(f,4u,2u);goto L8c6155cc; /* fmov fr2,fr4 */
L8c6155cc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6155ce; /* fmul fr3,fr4 */
L8c6155ce: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c6155d0; /* fldi0 fr2 */
L8c6155d0: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c6155d2; /* fcmp/gt fr2,fr4 */
L8c6155d2: if((*sr&1u))goto L8c6155f8;goto L8c6155d4; /* bt 0x8c6155f8 */
L8c6155d4: wr(m,bus,&io_status,r[15]+0x14u,r[5],4);if(io_status)return io_status;goto L8c6155d6; /* mov.l r5,@(0x14,r15) */
L8c6155d6: fcopy(f,4u,12u);goto L8c6155f8; /* bra 0x8c6155f8 */
L8c6155da: r[0]=20u;goto L8c6155dc; /* mov #0x14,r0 */
L8c6155dc: fcopy(f,3u,13u);goto L8c6155de; /* fmov fr13,fr3 */
L8c6155de: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6155e0; /* fmov.s @(r0,r12),fr2 */
L8c6155e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6155e2; /* fsub fr6,fr3 */
L8c6155e2: r[0]=16u;goto L8c6155e4; /* mov #0x10,r0 */
L8c6155e4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6155e6; /* fsub fr14,fr2 */
L8c6155e6: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c6155e8; /* fmov.s @(r0,r12),fr1 */
L8c6155e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6155ea; /* fsub fr13,fr1 */
L8c6155ea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6155ec; /* fmul fr2,fr3 */
L8c6155ec: fcopy(f,2u,14u);goto L8c6155ee; /* fmov fr14,fr2 */
L8c6155ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6155f0; /* fsub fr5,fr2 */
L8c6155f0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6155f2; /* fmul fr1,fr2 */
L8c6155f2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6155f4; /* fsub fr2,fr3 */
L8c6155f4: fcopy(f,4u,3u);goto L8c6155f6; /* fmov fr3,fr4 */
L8c6155f6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6155f8; /* fmul fr15,fr4 */
L8c6155f8: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c6155fa; /* cmp/hs r8,r9 */
L8c6155fa: if((*sr&1u))goto L8c615630;goto L8c6155fc; /* bt 0x8c615630 */
L8c6155fc: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6155fe; /* fldi0 fr3 */
L8c6155fe: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c615600; /* fcmp/gt fr4,fr3 */
L8c615600: if((*sr&1u))goto L8c615660;goto L8c615602; /* bt 0x8c615660 */
L8c615602: r[5]=rd(m,bus,&io_status,0x8c615764u,4);if(io_status)return io_status;goto L8c615604; /* mov.l 0x8c615764,r5 */
L8c615604: r[4]=r[11];goto L8c615606; /* mov r11,r4 */
L8c615606: r[8]+=16u;goto L8c615608; /* add #0x10,r8 */
L8c615608: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61560a; /* fschg */
L8c61560a: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61560c; /* fmov.s @r4+,fr0 */
L8c61560c: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61560e; /* fmov.s @r4+,fr2 */
L8c61560e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c615610; /* fldi1 fr3 */
L8c615610: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c615612; /* ftrv xmtrx,fv0 */
L8c615612: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615614; /* fmov.s @r4+,fr4 */
L8c615614: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615616; /* fmov.s @r4,fr6 */
L8c615616: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615618; /* fschg */
L8c615618: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c61561a; /* fmov.s fr6,@-r8 */
L8c61561a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61561c; /* fmov.s fr5,@-r8 */
L8c61561c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61561e; /* fmov.s fr4,@-r8 */
L8c61561e: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615620; /* mov.l r5,@-r8 */
L8c615620: r[8]+=28u;goto L8c615622; /* add #0x1c,r8 */
L8c615622: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c615624; /* fmov.s fr2,@-r8 */
L8c615624: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c615626; /* fmov.s fr1,@-r8 */
L8c615626: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c615628; /* fmov.s fr0,@-r8 */
L8c615628: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61562a; /* pref @r8 */
L8c61562a: r[8]+=16u;goto L8c61562c; /* add #0x10,r8 */
L8c61562c: r[9]=r[8];goto L8c61568a; /* bra 0x8c61568a */
L8c615630: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615632; /* fldi0 fr3 */
L8c615632: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c615634; /* fcmp/gt fr4,fr3 */
L8c615634: if(!(*sr&1u))goto L8c61568a;goto L8c615636; /* bf 0x8c61568a */
L8c615636: r[5]=rd(m,bus,&io_status,0x8c615768u,4);if(io_status)return io_status;goto L8c615638; /* mov.l 0x8c615768,r5 */
L8c615638: r[4]=r[10];goto L8c61563a; /* mov r10,r4 */
L8c61563a: r[8]+=16u;goto L8c61563c; /* add #0x10,r8 */
L8c61563c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61563e; /* fschg */
L8c61563e: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615640; /* fmov.s @r4+,fr0 */
L8c615640: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615642; /* fmov.s @r4+,fr2 */
L8c615642: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c615644; /* fldi1 fr3 */
L8c615644: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c615646; /* ftrv xmtrx,fv0 */
L8c615646: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615648; /* fmov.s @r4+,fr4 */
L8c615648: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61564a; /* fmov.s @r4,fr6 */
L8c61564a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61564c; /* fschg */
L8c61564c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c61564e; /* fmov.s fr6,@-r8 */
L8c61564e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615650; /* fmov.s fr5,@-r8 */
L8c615650: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615652; /* fmov.s fr4,@-r8 */
L8c615652: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615654; /* mov.l r5,@-r8 */
L8c615654: r[8]+=28u;goto L8c615656; /* add #0x1c,r8 */
L8c615656: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c615658; /* fmov.s fr2,@-r8 */
L8c615658: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61565a; /* fmov.s fr1,@-r8 */
L8c61565a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c61565c; /* fmov.s fr0,@-r8 */
L8c61565c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61565e; /* pref @r8 */
L8c61565e: r[8]+=16u;goto L8c615660; /* add #0x10,r8 */
L8c615660: r[5]=rd(m,bus,&io_status,0x8c615768u,4);if(io_status)return io_status;goto L8c615662; /* mov.l 0x8c615768,r5 */
L8c615662: r[4]=r[11];goto L8c615664; /* mov r11,r4 */
L8c615664: r[8]+=16u;goto L8c615666; /* add #0x10,r8 */
L8c615666: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615668; /* fschg */
L8c615668: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61566a; /* fmov.s @r4+,fr0 */
L8c61566a: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61566c; /* fmov.s @r4+,fr2 */
L8c61566c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c61566e; /* fldi1 fr3 */
L8c61566e: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c615670; /* ftrv xmtrx,fv0 */
L8c615670: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615672; /* fmov.s @r4+,fr4 */
L8c615672: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615674; /* fmov.s @r4,fr6 */
L8c615674: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615676; /* fschg */
L8c615676: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615678; /* fmov.s fr6,@-r8 */
L8c615678: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61567a; /* fmov.s fr5,@-r8 */
L8c61567a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61567c; /* fmov.s fr4,@-r8 */
L8c61567c: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61567e; /* mov.l r5,@-r8 */
L8c61567e: r[8]+=28u;goto L8c615680; /* add #0x1c,r8 */
L8c615680: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c615682; /* fmov.s fr2,@-r8 */
L8c615682: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c615684; /* fmov.s fr1,@-r8 */
L8c615684: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c615686; /* fmov.s fr0,@-r8 */
L8c615686: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615688; /* pref @r8 */
L8c615688: r[8]+=16u;goto L8c61568a; /* add #0x10,r8 */
L8c61568a: r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c61568c; /* mov.l @(0x14,r15),r2 */
L8c61568c: *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c61568e; /* tst r2,r2 */
L8c61568e: if((*sr&1u))goto L8c6156a2;goto L8c615690; /* bt 0x8c6156a2 */
L8c615690: r[4]=rd(m,bus,&io_status,0x8c61576cu,4);if(io_status)return io_status;goto L8c615692; /* mov.l 0x8c61576c,r4 */
L8c615692: r[3]=0u;goto L8c615694; /* mov #0x0,r3 */
L8c615694: r[2]=rd(m,bus,&io_status,0x8c615770u,4);if(io_status)return io_status;goto L8c615696; /* mov.l 0x8c615770,r2 */
L8c615696: r[5]=r[11];goto L8c615698; /* mov r11,r5 */
L8c615698: wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c61569a; /* mov.l r3,@r4 */
L8c61569a: r[6]=r[12];goto L8c61569c; /* mov r12,r6 */
L8c61569c: target=r[2];*pr=0x8c6156a0u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6156a0; /* jsr @r2 */
L8c6156a0: r[9]=r[8];goto L8c6156a2; /* mov r8,r9 */
L8c6156a2: fcopy(f,6u,13u);goto L8c6156a4; /* fmov fr13,fr6 */
L8c6156a4: r[0]=16u;goto L8c6156a6; /* mov #0x10,r0 */
L8c6156a6: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6156a8; /* mov.l @(0x8,r15),r3 */
L8c6156a8: r[10]=r[11];goto L8c6156aa; /* mov r11,r10 */
L8c6156aa: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c6156ac; /* fmov.s @(r0,r12),fr13 */
L8c6156ac: r[0]=20u;goto L8c6156ae; /* mov #0x14,r0 */
L8c6156ae: fcopy(f,5u,14u);goto L8c6156b0; /* fmov fr14,fr5 */
L8c6156b0: r[3]+=4294967295u;goto L8c6156b2; /* add #-0x1,r3 */
L8c6156b2: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c6156b4; /* fmov.s @(r0,r12),fr14 */
L8c6156b4: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6156b6; /* tst r3,r3 */
L8c6156b6: r[11]=r[12];goto L8c6156b8; /* mov r12,r11 */
L8c6156b8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c6156ba; /* fneg fr15 */
L8c6156ba: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c6156c2;goto L8c6156be; /* bt/s 0x8c6156c2 */
L8c6156be: goto L8c615468; /* bra 0x8c615468 */
L8c6156c2: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c6156c4; /* cmp/hs r8,r9 */
L8c6156c4: if((*sr&1u))goto L8c6156f0;goto L8c6156c6; /* bt 0x8c6156f0 */
L8c6156c6: r[5]=rd(m,bus,&io_status,0x8c615764u,4);if(io_status)return io_status;goto L8c6156c8; /* mov.l 0x8c615764,r5 */
L8c6156c8: r[4]=r[12];goto L8c6156ca; /* mov r12,r4 */
L8c6156ca: r[8]+=16u;goto L8c6156cc; /* add #0x10,r8 */
L8c6156cc: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6156ce; /* fschg */
L8c6156ce: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6156d0; /* fmov.s @r4+,fr0 */
L8c6156d0: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6156d2; /* fmov.s @r4+,fr2 */
L8c6156d2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6156d4; /* fldi1 fr3 */
L8c6156d4: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6156d6; /* ftrv xmtrx,fv0 */
L8c6156d6: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6156d8; /* fmov.s @r4+,fr4 */
L8c6156d8: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6156da; /* fmov.s @r4,fr6 */
L8c6156da: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6156dc; /* fschg */
L8c6156dc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6156de; /* fmov.s fr6,@-r8 */
L8c6156de: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6156e0; /* fmov.s fr5,@-r8 */
L8c6156e0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6156e2; /* fmov.s fr4,@-r8 */
L8c6156e2: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6156e4; /* mov.l r5,@-r8 */
L8c6156e4: r[8]+=28u;goto L8c6156e6; /* add #0x1c,r8 */
L8c6156e6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6156e8; /* fmov.s fr2,@-r8 */
L8c6156e8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6156ea; /* fmov.s fr1,@-r8 */
L8c6156ea: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c6156ec; /* fmov.s fr0,@-r8 */
L8c6156ec: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6156ee; /* pref @r8 */
L8c6156ee: r[8]+=16u;goto L8c6156f0; /* add #0x10,r8 */
L8c6156f0: r[3]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c6156f2; /* mov.l @(0x18,r15),r3 */
L8c6156f2: r[3]+=4294967295u;goto L8c6156f4; /* add #-0x1,r3 */
L8c6156f4: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6156f6; /* tst r3,r3 */
L8c6156f6: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;if(take)goto L8c6156fe;goto L8c6156fa; /* bt/s 0x8c6156fe */
L8c6156fa: goto L8c615418; /* bra 0x8c615418 */
L8c6156fe: r[4]=rd(m,bus,&io_status,0x8c615774u,4);if(io_status)return io_status;goto L8c615700; /* mov.l 0x8c615774,r4 */
L8c615700: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c615702; /* mov.l @(0xc,r15),r3 */
L8c615702: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c615704; /* mov.l @r4,r2 */
L8c615704: r[2]+=r[3];goto L8c615706; /* add r3,r2 */
L8c615706: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c615708; /* mov.l r2,@r4 */
L8c615708: r[4]=rd(m,bus,&io_status,0x8c615778u,4);if(io_status)return io_status;goto L8c61570a; /* mov.l 0x8c615778,r4 */
L8c61570a: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61570c; /* mov.l @(0xc,r15),r3 */
L8c61570c: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61570e; /* mov.l @r4,r2 */
L8c61570e: r[2]+=r[3];goto L8c615710; /* add r3,r2 */
L8c615710: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c615712; /* mov.l r2,@r4 */
L8c615712: r[15]+=76u;goto L8c615714; /* add #0x4c,r15 */
L8c615714: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615716; /* lds.l @r15+,PR */
L8c615716: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615718; /* fmov.s @r15+,fr12 */
L8c615718: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61571a; /* fmov.s @r15+,fr13 */
L8c61571a: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61571c; /* fmov.s @r15+,fr14 */
L8c61571c: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61571e; /* fmov.s @r15+,fr15 */
L8c61571e: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615720; /* mov.l @r15+,r9 */
L8c615720: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615722; /* mov.l @r15+,r10 */
L8c615722: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615724; /* mov.l @r15+,r11 */
L8c615724: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615726; /* mov.l @r15+,r12 */
L8c615726: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c615728; /* mov.l @r15+,r13 */
L8c615728: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
