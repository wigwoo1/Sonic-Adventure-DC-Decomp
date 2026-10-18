/* Alternate strip emitters with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_alternate_strip_8c6149b4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6149b4;
L8c6149b4: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6149b6; /* mov.l r14,@-r15 */
L8c6149b6: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6149b8; /* mov.l r13,@-r15 */
L8c6149b8: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6149ba; /* mov.l r12,@-r15 */
L8c6149ba: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6149bc; /* mov.l r11,@-r15 */
L8c6149bc: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6149be; /* mov.l r10,@-r15 */
L8c6149be: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6149c0; /* mov.l r9,@-r15 */
L8c6149c0: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c6149c2; /* fmov.s fr15,@-r15 */
L8c6149c2: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c6149c4; /* fmov.s fr14,@-r15 */
L8c6149c4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c6149c6; /* fmov.s fr13,@-r15 */
L8c6149c6: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6149c8; /* sts.l PR,@-r15 */
L8c6149c8: r[15]+=4294967280u;goto L8c6149ca; /* add #-0x10,r15 */
L8c6149ca: r[3]=rd(m,bus,&io_status,0x8c6149fcu,4);if(io_status)return io_status;goto L8c6149cc; /* mov.l 0x8c6149fc,r3 */
L8c6149cc: r[9]=rd(m,bus,&io_status,0x8c6149f8u,4);if(io_status)return io_status;goto L8c6149ce; /* mov.l 0x8c6149f8,r9 */
L8c6149ce: target=r[3];*pr=0x8c6149d2u;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6149d2; /* jsr @r3 */
L8c6149d2: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6149d4; /* mov.l @(0x4,r15),r3 */
L8c6149d4: r[2]=0u;goto L8c6149d6; /* mov #0x0,r2 */
L8c6149d6: r[14]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6149d8; /* mov.l @(0x4,r15),r14 */
L8c6149d8: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6149da; /* mov.w @(0x2,r3),r0 */
L8c6149da: r[3]=rd(m,bus,&io_status,0x8c614a00u,4);if(io_status)return io_status;goto L8c6149dc; /* mov.l 0x8c614a00,r3 */
L8c6149dc: r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c6149de; /* mov.l @(0x4,r14),r14 */
L8c6149de: r[0]=r[0]&65535u;goto L8c6149e0; /* extu.w r0,r0 */
L8c6149e0: wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c6149e2; /* mov.l r0,@(0xc,r15) */
L8c6149e2: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6149e4; /* mov.l @r3,r13 */
L8c6149e4: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6149e6; /* mov.l r2,@(0x8,r15) */
L8c6149e6: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6149e8; /* mov.l r2,@(0x4,r15) */
L8c6149e8: goto L8c614a04; /* bra 0x8c614a04 */
L8c614a04: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614a06; /* mov.w @r14+,r3 */
L8c614a06: r[4]=rd(m,bus,&io_status,0x8c614ad0u,4);if(io_status)return io_status;goto L8c614a08; /* mov.l 0x8c614ad0,r4 */
L8c614a08: r[3]=r[3]&65535u;goto L8c614a0a; /* extu.w r3,r3 */
L8c614a0a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c614a0c; /* fldi1 fr4 */
L8c614a0c: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c614a0e; /* tst r4,r3 */
L8c614a0e: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c614a10; /* mov.l r3,@r15 */
L8c614a10: take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c614a22;goto L8c614a14; /* bt/s 0x8c614a22 */
L8c614a14: r[4]=sx(m,bus,&io_status,0x8c614accu,2);if(io_status)return io_status;goto L8c614a16; /* mov.w 0x8c614acc,r4 */
L8c614a16: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614a18; /* mov.l @r15,r3 */
L8c614a18: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c614a1a; /* fldi1 fr4 */
L8c614a1a: r[3]&=r[4];goto L8c614a1c; /* and r4,r3 */
L8c614a1c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c614a1e; /* fneg fr4 */
L8c614a1e: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c614a20; /* mov.l r3,@r15 */
L8c614a20: fcopy(f,15u,4u);goto L8c614a22; /* fmov fr4,fr15 */
L8c614a22: r[0]=16u;goto L8c614a24; /* mov #0x10,r0 */
L8c614a24: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614a26; /* mov.l @r15,r3 */
L8c614a26: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614a28; /* mov.l @(0x4,r15),r2 */
L8c614a28: r[10]=r[8];goto L8c614a2a; /* mov r8,r10 */
L8c614a2a: r[2]+=r[3];goto L8c614a2c; /* add r3,r2 */
L8c614a2c: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c614a2e; /* mov.l r2,@(0x4,r15) */
L8c614a2e: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614a30; /* mov.l @r15,r1 */
L8c614a30: r[1]+=4294967294u;goto L8c614a32; /* add #-0x2,r1 */
L8c614a32: wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c614a34; /* mov.l r1,@r15 */
L8c614a34: r[4]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614a36; /* mov.w @r14+,r4 */
L8c614a36: r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614a38; /* mov.w @r14+,r11 */
L8c614a38: r[4]=r[4]&65535u;goto L8c614a3a; /* extu.w r4,r4 */
L8c614a3a: r[4]<<=2;goto L8c614a3c; /* shll2 r4 */
L8c614a3c: r[11]=r[11]&65535u;goto L8c614a3e; /* extu.w r11,r11 */
L8c614a3e: r[11]<<=2;goto L8c614a40; /* shll2 r11 */
L8c614a40: r[4]<<=2;goto L8c614a42; /* shll2 r4 */
L8c614a42: r[11]<<=2;goto L8c614a44; /* shll2 r11 */
L8c614a44: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c614a46; /* shll r4 */
L8c614a46: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c614a48; /* shll r11 */
L8c614a48: r[11]+=r[13];goto L8c614a4a; /* add r13,r11 */
L8c614a4a: r[4]+=r[13];goto L8c614a4c; /* add r13,r4 */
L8c614a4c: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c614a4e; /* fmov.s @(r0,r11),fr14 */
L8c614a4e: fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c614a50; /* fmov.s @(r0,r4),fr4 */
L8c614a50: r[0]=20u;goto L8c614a52; /* mov #0x14,r0 */
L8c614a52: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c614a54; /* fmov.s @(r0,r11),fr13 */
L8c614a54: fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c614a56; /* fmov.s @(r0,r4),fr5 */
L8c614a56: r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614a58; /* mov.w @r14+,r12 */
L8c614a58: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c614a5a; /* mov.w @r14,r3 */
L8c614a5a: r[12]=r[12]&65535u;goto L8c614a5c; /* extu.w r12,r12 */
L8c614a5c: r[12]<<=2;goto L8c614a5e; /* shll2 r12 */
L8c614a5e: r[3]=r[3]&65535u;goto L8c614a60; /* extu.w r3,r3 */
L8c614a60: r[3]<<=2;goto L8c614a62; /* shll2 r3 */
L8c614a62: r[12]<<=2;goto L8c614a64; /* shll2 r12 */
L8c614a64: r[3]<<=2;goto L8c614a66; /* shll2 r3 */
L8c614a66: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c614a68; /* shll r12 */
L8c614a68: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c614a6a; /* shll r3 */
L8c614a6a: r[3]+=r[13];goto L8c614a6c; /* add r13,r3 */
L8c614a6c: r[12]+=r[13];goto L8c614a6e; /* add r13,r12 */
L8c614a6e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c614a70; /* pref @r3 */
L8c614a70: r[0]=20u;goto L8c614a72; /* mov #0x14,r0 */
L8c614a72: fcopy(f,3u,14u);goto L8c614a74; /* fmov fr14,fr3 */
L8c614a74: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c614a76; /* fmov.s @(r0,r12),fr2 */
L8c614a76: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614a78; /* fsub fr4,fr3 */
L8c614a78: r[0]=16u;goto L8c614a7a; /* mov #0x10,r0 */
L8c614a7a: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c614a7c; /* cmp/hs r8,r10 */
L8c614a7c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614a7e; /* fsub fr13,fr2 */
L8c614a7e: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c614a80; /* fmov.s @(r0,r12),fr1 */
L8c614a80: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c614a82; /* fsub fr14,fr1 */
L8c614a82: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614a84; /* fmul fr2,fr3 */
L8c614a84: fcopy(f,2u,13u);goto L8c614a86; /* fmov fr13,fr2 */
L8c614a86: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614a88; /* fsub fr5,fr2 */
L8c614a88: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614a8a; /* fmul fr1,fr2 */
L8c614a8a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614a8c; /* fsub fr2,fr3 */
L8c614a8c: fcopy(f,4u,3u);goto L8c614a8e; /* fmov fr3,fr4 */
L8c614a8e: take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c614ad4;goto L8c614a92; /* bt/s 0x8c614ad4 */
L8c614a92: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614a94; /* fldi0 fr3 */
L8c614a94: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c614a96; /* fcmp/gt fr4,fr3 */
L8c614a96: if((*sr&1u))goto L8c614afc;goto L8c614a98; /* bt 0x8c614afc */
L8c614a98: r[4]=r[11];goto L8c614a9a; /* mov r11,r4 */
L8c614a9a: r[5]=r[9];goto L8c614a9c; /* mov r9,r5 */
L8c614a9c: r[8]+=32u;goto L8c614a9e; /* add #0x20,r8 */
L8c614a9e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614aa0; /* fschg */
L8c614aa0: r[4]+=16u;goto L8c614aa2; /* add #0x10,r4 */
L8c614aa2: fcopy(f,0u,1u);goto L8c614aa4; /* fmov fr1,fr0 */
L8c614aa4: fcopy(f,2u,3u);goto L8c614aa6; /* fmov fr3,fr2 */
L8c614aa6: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614aa8; /* fmov.s @r4+,fr4 */
L8c614aa8: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614aaa; /* fmov.s @r4,fr6 */
L8c614aaa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614aac; /* fmov.s fr2,@-r8 */
L8c614aac: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c614aae; /* fmov.s fr0,@-r8 */
L8c614aae: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614ab0; /* fschg */
L8c614ab0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614ab2; /* fmov.s fr6,@-r8 */
L8c614ab2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614ab4; /* fmov.s fr5,@-r8 */
L8c614ab4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614ab6; /* fmov.s fr4,@-r8 */
L8c614ab6: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614ab8; /* mov.l r5,@-r8 */
L8c614ab8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614aba; /* pref @r8 */
L8c614aba: r[8]+=32u;goto L8c614abc; /* add #0x20,r8 */
L8c614abc: r[3]=r[8];goto L8c614abe; /* mov r8,r3 */
L8c614abe: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614ac0; /* mov.l @(0x8,r15),r2 */
L8c614ac0: r[3]-=r[10];goto L8c614ac2; /* sub r10,r3 */
L8c614ac2: r[3]+=32u;goto L8c614ac4; /* add #0x20,r3 */
L8c614ac4: r[2]+=r[3];goto L8c614ac6; /* add r3,r2 */
L8c614ac6: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c614ac8; /* mov.l r2,@(0x8,r15) */
L8c614ac8: r[10]=r[8];goto L8c614b20; /* bra 0x8c614b20 */
L8c614ad4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614ad6; /* fldi0 fr3 */
L8c614ad6: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c614ad8; /* fcmp/gt fr4,fr3 */
L8c614ad8: if(!(*sr&1u))goto L8c614b20;goto L8c614ada; /* bf 0x8c614b20 */
L8c614ada: r[5]=rd(m,bus,&io_status,0x8c614be8u,4);if(io_status)return io_status;goto L8c614adc; /* mov.l 0x8c614be8,r5 */
L8c614adc: r[8]+=32u;goto L8c614ade; /* add #0x20,r8 */
L8c614ade: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614ae0; /* fschg */
L8c614ae0: r[4]+=16u;goto L8c614ae2; /* add #0x10,r4 */
L8c614ae2: fcopy(f,0u,1u);goto L8c614ae4; /* fmov fr1,fr0 */
L8c614ae4: fcopy(f,2u,3u);goto L8c614ae6; /* fmov fr3,fr2 */
L8c614ae6: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614ae8; /* fmov.s @r4+,fr4 */
L8c614ae8: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614aea; /* fmov.s @r4,fr6 */
L8c614aea: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614aec; /* fmov.s fr2,@-r8 */
L8c614aec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c614aee; /* fmov.s fr0,@-r8 */
L8c614aee: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614af0; /* fschg */
L8c614af0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614af2; /* fmov.s fr6,@-r8 */
L8c614af2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614af4; /* fmov.s fr5,@-r8 */
L8c614af4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614af6; /* fmov.s fr4,@-r8 */
L8c614af6: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614af8; /* mov.l r5,@-r8 */
L8c614af8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614afa; /* pref @r8 */
L8c614afa: r[8]+=32u;goto L8c614afc; /* add #0x20,r8 */
L8c614afc: r[5]=rd(m,bus,&io_status,0x8c614be8u,4);if(io_status)return io_status;goto L8c614afe; /* mov.l 0x8c614be8,r5 */
L8c614afe: r[4]=r[11];goto L8c614b00; /* mov r11,r4 */
L8c614b00: r[8]+=32u;goto L8c614b02; /* add #0x20,r8 */
L8c614b02: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614b04; /* fschg */
L8c614b04: r[4]+=16u;goto L8c614b06; /* add #0x10,r4 */
L8c614b06: fcopy(f,0u,1u);goto L8c614b08; /* fmov fr1,fr0 */
L8c614b08: fcopy(f,2u,3u);goto L8c614b0a; /* fmov fr3,fr2 */
L8c614b0a: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614b0c; /* fmov.s @r4+,fr4 */
L8c614b0c: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614b0e; /* fmov.s @r4,fr6 */
L8c614b0e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614b10; /* fmov.s fr2,@-r8 */
L8c614b10: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c614b12; /* fmov.s fr0,@-r8 */
L8c614b12: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614b14; /* fschg */
L8c614b14: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614b16; /* fmov.s fr6,@-r8 */
L8c614b16: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614b18; /* fmov.s fr5,@-r8 */
L8c614b18: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614b1a; /* fmov.s fr4,@-r8 */
L8c614b1a: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614b1c; /* mov.l r5,@-r8 */
L8c614b1c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614b1e; /* pref @r8 */
L8c614b1e: r[8]+=32u;goto L8c614b20; /* add #0x20,r8 */
L8c614b20: fcopy(f,4u,14u);goto L8c614b22; /* fmov fr14,fr4 */
L8c614b22: r[0]=16u;goto L8c614b24; /* mov #0x10,r0 */
L8c614b24: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614b26; /* mov.l @r15,r3 */
L8c614b26: r[4]=r[11];goto L8c614b28; /* mov r11,r4 */
L8c614b28: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c614b2a; /* fmov.s @(r0,r12),fr14 */
L8c614b2a: r[0]=20u;goto L8c614b2c; /* mov #0x14,r0 */
L8c614b2c: fcopy(f,5u,13u);goto L8c614b2e; /* fmov fr13,fr5 */
L8c614b2e: r[3]+=4294967295u;goto L8c614b30; /* add #-0x1,r3 */
L8c614b30: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c614b32; /* fmov.s @(r0,r12),fr13 */
L8c614b32: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614b34; /* tst r3,r3 */
L8c614b34: r[11]=r[12];goto L8c614b36; /* mov r12,r11 */
L8c614b36: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c614b38; /* fneg fr15 */
L8c614b38: take=!(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c614a56;goto L8c614b3c; /* bf/s 0x8c614a56 */
L8c614b3c: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c614b3e; /* cmp/hs r8,r10 */
L8c614b3e: if((*sr&1u))goto L8c614b70;goto L8c614b40; /* bt 0x8c614b70 */
L8c614b40: r[4]=r[12];goto L8c614b42; /* mov r12,r4 */
L8c614b42: r[5]=r[9];goto L8c614b44; /* mov r9,r5 */
L8c614b44: r[8]+=32u;goto L8c614b46; /* add #0x20,r8 */
L8c614b46: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614b48; /* fschg */
L8c614b48: r[4]+=16u;goto L8c614b4a; /* add #0x10,r4 */
L8c614b4a: fcopy(f,0u,1u);goto L8c614b4c; /* fmov fr1,fr0 */
L8c614b4c: fcopy(f,2u,3u);goto L8c614b4e; /* fmov fr3,fr2 */
L8c614b4e: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614b50; /* fmov.s @r4+,fr4 */
L8c614b50: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614b52; /* fmov.s @r4,fr6 */
L8c614b52: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614b54; /* fmov.s fr2,@-r8 */
L8c614b54: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c614b56; /* fmov.s fr0,@-r8 */
L8c614b56: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614b58; /* fschg */
L8c614b58: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614b5a; /* fmov.s fr6,@-r8 */
L8c614b5a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614b5c; /* fmov.s fr5,@-r8 */
L8c614b5c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614b5e; /* fmov.s fr4,@-r8 */
L8c614b5e: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614b60; /* mov.l r5,@-r8 */
L8c614b60: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614b62; /* pref @r8 */
L8c614b62: r[8]+=32u;goto L8c614b64; /* add #0x20,r8 */
L8c614b64: r[3]=r[8];goto L8c614b66; /* mov r8,r3 */
L8c614b66: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614b68; /* mov.l @(0x8,r15),r2 */
L8c614b68: r[3]-=r[10];goto L8c614b6a; /* sub r10,r3 */
L8c614b6a: r[3]+=32u;goto L8c614b6c; /* add #0x20,r3 */
L8c614b6c: r[2]+=r[3];goto L8c614b6e; /* add r3,r2 */
L8c614b6e: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c614b70; /* mov.l r2,@(0x8,r15) */
L8c614b70: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614b72; /* mov.l @(0xc,r15),r3 */
L8c614b72: r[3]+=4294967295u;goto L8c614b74; /* add #-0x1,r3 */
L8c614b74: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614b76; /* tst r3,r3 */
L8c614b76: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c614b7e;goto L8c614b7a; /* bt/s 0x8c614b7e */
L8c614b7a: goto L8c614a04; /* bra 0x8c614a04 */
L8c614b7e: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614b80; /* mov.l @(0x8,r15),r3 */
L8c614b80: r[4]=rd(m,bus,&io_status,0x8c614becu,4);if(io_status)return io_status;goto L8c614b82; /* mov.l 0x8c614bec,r4 */
L8c614b82: r[3]>>=2;goto L8c614b84; /* shlr2 r3 */
L8c614b84: r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c614b86; /* mov.l @r4,r1 */
L8c614b86: r[3]>>=2;goto L8c614b88; /* shlr2 r3 */
L8c614b88: *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c614b8a; /* shlr r3 */
L8c614b8a: r[1]+=r[3];goto L8c614b8c; /* add r3,r1 */
L8c614b8c: wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c614b8e; /* mov.l r1,@r4 */
L8c614b8e: r[4]=rd(m,bus,&io_status,0x8c614bf0u,4);if(io_status)return io_status;goto L8c614b90; /* mov.l 0x8c614bf0,r4 */
L8c614b90: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614b92; /* mov.l @(0x4,r15),r3 */
L8c614b92: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c614b94; /* mov.l @r4,r2 */
L8c614b94: r[2]+=r[3];goto L8c614b96; /* add r3,r2 */
L8c614b96: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c614b98; /* mov.l r2,@r4 */
L8c614b98: r[15]+=16u;goto L8c614b9a; /* add #0x10,r15 */
L8c614b9a: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614b9c; /* lds.l @r15+,PR */
L8c614b9c: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614b9e; /* fmov.s @r15+,fr13 */
L8c614b9e: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614ba0; /* fmov.s @r15+,fr14 */
L8c614ba0: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614ba2; /* fmov.s @r15+,fr15 */
L8c614ba2: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614ba4; /* mov.l @r15+,r9 */
L8c614ba4: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614ba6; /* mov.l @r15+,r10 */
L8c614ba6: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614ba8; /* mov.l @r15+,r11 */
L8c614ba8: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614baa; /* mov.l @r15+,r12 */
L8c614baa: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614bac; /* mov.l @r15+,r13 */
L8c614bac: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_alternate_strip_clipped_8c616bc4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c616bc4;
L8c616bc4: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c616bc6; /* mov.l r14,@-r15 */
L8c616bc6: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c616bc8; /* mov.l r13,@-r15 */
L8c616bc8: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c616bca; /* mov.l r12,@-r15 */
L8c616bca: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c616bcc; /* mov.l r11,@-r15 */
L8c616bcc: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c616bce; /* mov.l r10,@-r15 */
L8c616bce: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c616bd0; /* mov.l r9,@-r15 */
L8c616bd0: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c616bd2; /* fmov.s fr15,@-r15 */
L8c616bd2: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c616bd4; /* fmov.s fr14,@-r15 */
L8c616bd4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c616bd6; /* fmov.s fr13,@-r15 */
L8c616bd6: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c616bd8; /* fmov.s fr12,@-r15 */
L8c616bd8: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c616bda; /* sts.l PR,@-r15 */
L8c616bda: r[15]+=4294967220u;goto L8c616bdc; /* add #-0x4c,r15 */
L8c616bdc: r[3]=rd(m,bus,&io_status,0x8c616c14u,4);if(io_status)return io_status;goto L8c616bde; /* mov.l 0x8c616c14,r3 */
L8c616bde: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c616be0; /* fldi1 fr12 */
L8c616be0: target=r[3];*pr=0x8c616be4u;wr(m,bus,&io_status,r[15]+0xcu,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c616be4; /* jsr @r3 */
L8c616be4: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616be6; /* mov.l @(0xc,r15),r3 */
L8c616be6: r[2]=0u;goto L8c616be8; /* mov #0x0,r2 */
L8c616be8: r[14]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616bea; /* mov.l @(0xc,r15),r14 */
L8c616bea: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c616bec; /* mov.w @(0x2,r3),r0 */
L8c616bec: r[3]=rd(m,bus,&io_status,0x8c616c18u,4);if(io_status)return io_status;goto L8c616bee; /* mov.l 0x8c616c18,r3 */
L8c616bee: r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c616bf0; /* mov.l @(0x4,r14),r14 */
L8c616bf0: r[0]=r[0]&65535u;goto L8c616bf2; /* extu.w r0,r0 */
L8c616bf2: wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c616bf4; /* mov.l r0,@(0x18,r15) */
L8c616bf4: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c616bf6; /* mov.l @r3,r13 */
L8c616bf6: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c616bf8; /* mov.l r2,@(0xc,r15) */
L8c616bf8: goto L8c616c1c; /* bra 0x8c616c1c */
L8c616c1c: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c616c1e; /* mov.w @r14+,r3 */
L8c616c1e: r[4]=rd(m,bus,&io_status,0x8c616d38u,4);if(io_status)return io_status;goto L8c616c20; /* mov.l 0x8c616d38,r4 */
L8c616c20: r[3]=r[3]&65535u;goto L8c616c22; /* extu.w r3,r3 */
L8c616c22: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c616c24; /* tst r4,r3 */
L8c616c24: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c616c26; /* mov.l r3,@(0x8,r15) */
L8c616c26: take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c616c38;goto L8c616c2a; /* bt/s 0x8c616c38 */
L8c616c2a: r[4]=sx(m,bus,&io_status,0x8c616d34u,2);if(io_status)return io_status;goto L8c616c2c; /* mov.w 0x8c616d34,r4 */
L8c616c2c: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c616c2e; /* mov.l @(0x8,r15),r3 */
L8c616c2e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c616c30; /* fldi1 fr4 */
L8c616c30: r[3]&=r[4];goto L8c616c32; /* and r4,r3 */
L8c616c32: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c616c34; /* fneg fr4 */
L8c616c34: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c616c36; /* mov.l r3,@(0x8,r15) */
L8c616c36: fcopy(f,15u,4u);goto L8c616c38; /* fmov fr4,fr15 */
L8c616c38: r[0]=16u;goto L8c616c3a; /* mov #0x10,r0 */
L8c616c3a: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c616c3c; /* mov.l @(0x8,r15),r3 */
L8c616c3c: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616c3e; /* mov.l @(0xc,r15),r2 */
L8c616c3e: r[9]=r[8];goto L8c616c40; /* mov r8,r9 */
L8c616c40: r[2]+=r[3];goto L8c616c42; /* add r3,r2 */
L8c616c42: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c616c44; /* mov.l r2,@(0xc,r15) */
L8c616c44: r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c616c46; /* mov.l @(0x8,r15),r1 */
L8c616c46: r[1]+=4294967294u;goto L8c616c48; /* add #-0x2,r1 */
L8c616c48: wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c616c4a; /* mov.l r1,@(0x8,r15) */
L8c616c4a: r[10]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c616c4c; /* mov.w @r14+,r10 */
L8c616c4c: r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c616c4e; /* mov.w @r14+,r11 */
L8c616c4e: r[10]=r[10]&65535u;goto L8c616c50; /* extu.w r10,r10 */
L8c616c50: r[10]<<=2;goto L8c616c52; /* shll2 r10 */
L8c616c52: r[11]=r[11]&65535u;goto L8c616c54; /* extu.w r11,r11 */
L8c616c54: r[11]<<=2;goto L8c616c56; /* shll2 r11 */
L8c616c56: r[10]<<=2;goto L8c616c58; /* shll2 r10 */
L8c616c58: r[11]<<=2;goto L8c616c5a; /* shll2 r11 */
L8c616c5a: *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c616c5c; /* shll r10 */
L8c616c5c: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c616c5e; /* shll r11 */
L8c616c5e: r[11]+=r[13];goto L8c616c60; /* add r13,r11 */
L8c616c60: r[10]+=r[13];goto L8c616c62; /* add r13,r10 */
L8c616c62: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c616c64; /* fmov.s @(r0,r11),fr13 */
L8c616c64: fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c616c66; /* fmov.s @(r0,r10),fr6 */
L8c616c66: r[0]=20u;goto L8c616c68; /* mov #0x14,r0 */
L8c616c68: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c616c6a; /* fmov.s @(r0,r11),fr14 */
L8c616c6a: fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c616c6c; /* fmov.s @(r0,r10),fr5 */
L8c616c6c: r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c616c6e; /* mov.w @r14+,r12 */
L8c616c6e: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c616c70; /* mov.w @r14,r3 */
L8c616c70: r[12]=r[12]&65535u;goto L8c616c72; /* extu.w r12,r12 */
L8c616c72: r[12]<<=2;goto L8c616c74; /* shll2 r12 */
L8c616c74: r[3]=r[3]&65535u;goto L8c616c76; /* extu.w r3,r3 */
L8c616c76: r[3]<<=2;goto L8c616c78; /* shll2 r3 */
L8c616c78: r[12]<<=2;goto L8c616c7a; /* shll2 r12 */
L8c616c7a: r[3]<<=2;goto L8c616c7c; /* shll2 r3 */
L8c616c7c: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c616c7e; /* shll r12 */
L8c616c7e: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c616c80; /* shll r3 */
L8c616c80: r[3]+=r[13];goto L8c616c82; /* add r13,r3 */
L8c616c82: r[12]+=r[13];goto L8c616c84; /* add r13,r12 */
L8c616c84: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c616c86; /* pref @r3 */
L8c616c86: r[2]=0u;goto L8c616c88; /* mov #0x0,r2 */
L8c616c88: wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c616c8a; /* mov.l r2,@(0x14,r15) */
L8c616c8a: r[4]=63u;goto L8c616c8c; /* mov #0x3f,r4 */
L8c616c8c: r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c616c8e; /* mov.l @(0xc,r10),r2 */
L8c616c8e: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c616c90; /* mov.l @(0xc,r11),r3 */
L8c616c90: r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c616c92; /* mov.l @(0xc,r12),r1 */
L8c616c92: r[2]&=r[3];goto L8c616c94; /* and r3,r2 */
L8c616c94: r[2]&=r[1];goto L8c616c96; /* and r1,r2 */
L8c616c96: r[4]&=r[2];goto L8c616c98; /* and r2,r4 */
L8c616c98: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c616c9a; /* tst r4,r4 */
L8c616c9a: take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c616ca2;goto L8c616c9e; /* bt/s 0x8c616ca2 */
L8c616c9e: goto L8c616dfc; /* bra 0x8c616dfc */
L8c616ca2: r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c616ca4; /* mov.l @(0xc,r10),r4 */
L8c616ca4: r[5]=1u;goto L8c616ca6; /* mov #0x1,r5 */
L8c616ca6: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c616ca8; /* mov.l @(0xc,r11),r3 */
L8c616ca8: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c616caa; /* mov.l @(0xc,r12),r2 */
L8c616caa: r[4]|=r[3];goto L8c616cac; /* or r3,r4 */
L8c616cac: r[4]|=r[2];goto L8c616cae; /* or r2,r4 */
L8c616cae: r[4]&=r[5];goto L8c616cb0; /* and r5,r4 */
L8c616cb0: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c616cb2; /* tst r4,r4 */
L8c616cb2: if(!(*sr&1u))goto L8c616cb8;goto L8c616cb4; /* bf 0x8c616cb8 */
L8c616cb4: goto L8c616dde; /* bra 0x8c616dde */
L8c616cb8: r[7]=r[15];goto L8c616cba; /* mov r15,r7 */
L8c616cba: r[7]+=64u;goto L8c616cbc; /* add #0x40,r7 */
L8c616cbc: wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c616cbe; /* mov.l r10,@r7 */
L8c616cbe: wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c616cc0; /* mov.l r11,@(0x4,r7) */
L8c616cc0: wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c616cc2; /* mov.l r12,@(0x8,r7) */
L8c616cc2: r[6]=0u;goto L8c616d70; /* bra 0x8c616d70 */
L8c616cc6: r[2]=r[6];goto L8c616cc8; /* mov r6,r2 */
L8c616cc8: r[1]=r[6];goto L8c616cca; /* mov r6,r1 */
L8c616cca: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c616ccc; /* shll r2 */
L8c616ccc: r[3]=r[6];goto L8c616cce; /* mov r6,r3 */
L8c616cce: r[3]<<=2;goto L8c616cd0; /* shll2 r3 */
L8c616cd0: r[2]+=r[1];goto L8c616cd2; /* add r1,r2 */
L8c616cd2: r[2]<<=2;goto L8c616cd4; /* shll2 r2 */
L8c616cd4: r[3]+=r[7];goto L8c616cd6; /* add r7,r3 */
L8c616cd6: r[2]+=r[4];goto L8c616cd8; /* add r4,r2 */
L8c616cd8: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c616cda; /* mov.l r3,@(0x4,r15) */
L8c616cda: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c616cdc; /* mov.l r2,@r15 */
L8c616cdc: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c616cde; /* mov.l @r3,r1 */
L8c616cde: r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c616ce0; /* mov.l @(0xc,r1),r2 */
L8c616ce0: *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c616ce2; /* tst r5,r2 */
L8c616ce2: take=!(*sr&1u);r[6]+=1u;if(take)goto L8c616d48;goto L8c616ce6; /* bf/s 0x8c616d48 */
L8c616ce6: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616ce8; /* mov.l @(0x4,r15),r1 */
L8c616ce8: r[0]=24u;goto L8c616cea; /* mov #0x18,r0 */
L8c616cea: fcopy(f,4u,12u);goto L8c616cec; /* fmov fr12,fr4 */
L8c616cec: r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c616cee; /* mov.l @r1,r3 */
L8c616cee: r[4]=rd(m,bus,&io_status,0x8c616d3cu,4);if(io_status)return io_status;goto L8c616cf0; /* mov.l 0x8c616d3c,r4 */
L8c616cf0: wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c616cf2; /* mov.l r3,@(0x10,r15) */
L8c616cf2: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616cf4; /* fmov.s @(r0,r3),fr3 */
L8c616cf4: r[0]=12u;goto L8c616cf6; /* mov #0xc,r0 */
L8c616cf6: r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c616cf8; /* mov.l @(0x10,r15),r2 */
L8c616cf8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616cfa; /* fdiv fr3,fr4 */
L8c616cfa: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c616cfc; /* fmov.s @(r0,r4),fr3 */
L8c616cfc: r[0]=16u;goto L8c616cfe; /* mov #0x10,r0 */
L8c616cfe: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616d00; /* mov.l @r15,r3 */
L8c616d00: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c616d02; /* fmov.s @(r0,r2),fr2 */
L8c616d02: r[0]=16u;goto L8c616d04; /* mov #0x10,r0 */
L8c616d04: r[2]=rd(m,bus,&io_status,0x8c616d40u,4);if(io_status)return io_status;goto L8c616d06; /* mov.l 0x8c616d40,r2 */
L8c616d06: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616d08; /* fsub fr3,fr2 */
L8c616d08: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c616d0a; /* fmov.s @r2,fr3 */
L8c616d0a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616d0c; /* fmul fr4,fr2 */
L8c616d0c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616d0e; /* fdiv fr3,fr2 */
L8c616d0e: fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c616d10; /* fmov.s fr2,@r3 */
L8c616d10: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616d12; /* mov.l @(0x4,r15),r1 */
L8c616d12: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c616d14; /* fmov.s @(r0,r4),fr3 */
L8c616d14: r[0]=20u;goto L8c616d16; /* mov #0x14,r0 */
L8c616d16: r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c616d18; /* mov.l @r1,r1 */
L8c616d18: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616d1a; /* mov.l @r15,r3 */
L8c616d1a: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c616d1c; /* fmov.s @(r0,r1),fr2 */
L8c616d1c: r[1]=rd(m,bus,&io_status,0x8c616d44u,4);if(io_status)return io_status;goto L8c616d1e; /* mov.l 0x8c616d44,r1 */
L8c616d1e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616d20; /* fsub fr3,fr2 */
L8c616d20: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c616d22; /* fmov.s @r1,fr3 */
L8c616d22: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616d24; /* fmul fr4,fr2 */
L8c616d24: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616d26; /* fdiv fr3,fr2 */
L8c616d26: r[0]=4u;goto L8c616d28; /* mov #0x4,r0 */
L8c616d28: fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c616d2a; /* fmov fr2,@(r0,r3) */
L8c616d2a: r[0]=8u;goto L8c616d2c; /* mov #0x8,r0 */
L8c616d2c: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616d2e; /* mov.l @r15,r3 */
L8c616d2e: fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c616d30; /* fmov fr4,@(r0,r3) */
L8c616d30: goto L8c616d70; /* bra 0x8c616d70 */
L8c616d48: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616d4a; /* mov.l @(0x4,r15),r3 */
L8c616d4a: r[0]=16u;goto L8c616d4c; /* mov #0x10,r0 */
L8c616d4c: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616d4e; /* mov.l @r15,r2 */
L8c616d4e: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c616d50; /* mov.l @r3,r1 */
L8c616d50: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c616d52; /* fmov.s @(r0,r1),fr3 */
L8c616d52: r[0]=20u;goto L8c616d54; /* mov #0x14,r0 */
L8c616d54: fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c616d56; /* fmov.s fr3,@r2 */
L8c616d56: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616d58; /* mov.l @(0x4,r15),r2 */
L8c616d58: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616d5a; /* mov.l @r15,r3 */
L8c616d5a: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c616d5c; /* mov.l @r2,r1 */
L8c616d5c: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c616d5e; /* fmov.s @(r0,r1),fr3 */
L8c616d5e: r[0]=4u;goto L8c616d60; /* mov #0x4,r0 */
L8c616d60: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616d62; /* fmov fr3,@(r0,r3) */
L8c616d62: r[0]=24u;goto L8c616d64; /* mov #0x18,r0 */
L8c616d64: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616d66; /* mov.l @(0x4,r15),r2 */
L8c616d66: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616d68; /* mov.l @r15,r3 */
L8c616d68: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c616d6a; /* mov.l @r2,r1 */
L8c616d6a: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c616d6c; /* fmov.s @(r0,r1),fr3 */
L8c616d6c: r[0]=8u;goto L8c616d6e; /* mov #0x8,r0 */
L8c616d6e: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616d70; /* fmov fr3,@(r0,r3) */
L8c616d70: r[3]=3u;goto L8c616d72; /* mov #0x3,r3 */
L8c616d72: r[4]=r[15];goto L8c616d74; /* mov r15,r4 */
L8c616d74: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c616d76; /* cmp/ge r3,r6 */
L8c616d76: take=!(*sr&1u);r[4]+=28u;if(take)goto L8c616cc6;goto L8c616d7a; /* bf/s 0x8c616cc6 */
L8c616d7a: r[6]=r[4];goto L8c616d7c; /* mov r4,r6 */
L8c616d7c: r[0]=8u;goto L8c616d7e; /* mov #0x8,r0 */
L8c616d7e: r[3]=r[4];goto L8c616d80; /* mov r4,r3 */
L8c616d80: fload(m,bus,&io_status,f,6u,r[0]+r[6]);if(io_status)return io_status;goto L8c616d82; /* fmov.s @(r0,r6),fr6 */
L8c616d82: r[3]+=12u;goto L8c616d84; /* add #0xc,r3 */
L8c616d84: r[7]=r[4];goto L8c616d86; /* mov r4,r7 */
L8c616d86: r[0]=4u;goto L8c616d88; /* mov #0x4,r0 */
L8c616d88: fcopy(f,0u,6u);goto L8c616d8a; /* fmov fr6,fr0 */
L8c616d8a: r[7]+=24u;goto L8c616d8c; /* add #0x18,r7 */
L8c616d8c: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c616d8e; /* mov.l r3,@r15 */
L8c616d8e: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c616d90; /* fmov.s @(r0,r6),fr4 */
L8c616d90: fload(m,bus,&io_status,f,5u,r[0]+r[3]);if(io_status)return io_status;goto L8c616d92; /* fmov.s @(r0,r3),fr5 */
L8c616d92: r[0]=8u;goto L8c616d94; /* mov #0x8,r0 */
L8c616d94: fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c616d96; /* fmov.s @(r0,r3),fr9 */
L8c616d96: r[0]=4u;goto L8c616d98; /* mov #0x4,r0 */
L8c616d98: fcopy(f,3u,4u);goto L8c616d9a; /* fmov fr4,fr3 */
L8c616d9a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c616d9c; /* fmul fr5,fr0 */
L8c616d9c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616d9e; /* fmul fr9,fr3 */
L8c616d9e: fload(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;goto L8c616da0; /* fmov.s @r6,fr8 */
L8c616da0: fload(m,bus,&io_status,f,7u,r[3]);if(io_status)return io_status;goto L8c616da2; /* fmov.s @r3,fr7 */
L8c616da2: fcopy(f,2u,8u);goto L8c616da4; /* fmov fr8,fr2 */
L8c616da4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[9],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616da6; /* fmul fr9,fr2 */
L8c616da6: fcopy(f,1u,4u);goto L8c616da8; /* fmov fr4,fr1 */
L8c616da8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c616daa; /* fsub fr3,fr0 */
L8c616daa: fcopy(f,3u,6u);goto L8c616dac; /* fmov fr6,fr3 */
L8c616dac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616dae; /* fmul fr7,fr3 */
L8c616dae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616db0; /* fsub fr3,fr2 */
L8c616db0: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c616db2; /* fmov.s @(r0,r7),fr3 */
L8c616db2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616db4; /* fmul fr2,fr3 */
L8c616db4: fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c616db6; /* fmov.s @r7,fr2 */
L8c616db6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616db8; /* fmac fr0,fr2,fr3 */
L8c616db8: fcopy(f,2u,3u);goto L8c616dba; /* fmov fr3,fr2 */
L8c616dba: fcopy(f,3u,8u);goto L8c616dbc; /* fmov fr8,fr3 */
L8c616dbc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616dbe; /* fmul fr5,fr3 */
L8c616dbe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c616dc0; /* fmul fr7,fr1 */
L8c616dc0: r[0]=8u;goto L8c616dc2; /* mov #0x8,r0 */
L8c616dc2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c616dc4; /* fsub fr3,fr1 */
L8c616dc4: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c616dc6; /* fmov.s @(r0,r7),fr3 */
L8c616dc6: fcopy(f,0u,1u);goto L8c616dc8; /* fmov fr1,fr0 */
L8c616dc8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616dca; /* fmac fr0,fr3,fr2 */
L8c616dca: fcopy(f,3u,15u);goto L8c616dcc; /* fmov fr15,fr3 */
L8c616dcc: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c616dce; /* fneg fr3 */
L8c616dce: fcopy(f,4u,2u);goto L8c616dd0; /* fmov fr2,fr4 */
L8c616dd0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616dd2; /* fmul fr3,fr4 */
L8c616dd2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c616dd4; /* fldi0 fr2 */
L8c616dd4: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c616dd6; /* fcmp/gt fr2,fr4 */
L8c616dd6: if((*sr&1u))goto L8c616dfc;goto L8c616dd8; /* bt 0x8c616dfc */
L8c616dd8: wr(m,bus,&io_status,r[15]+0x14u,r[5],4);if(io_status)return io_status;goto L8c616dda; /* mov.l r5,@(0x14,r15) */
L8c616dda: fcopy(f,4u,12u);goto L8c616dfc; /* bra 0x8c616dfc */
L8c616dde: r[0]=20u;goto L8c616de0; /* mov #0x14,r0 */
L8c616de0: fcopy(f,3u,13u);goto L8c616de2; /* fmov fr13,fr3 */
L8c616de2: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c616de4; /* fmov.s @(r0,r12),fr2 */
L8c616de4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c616de6; /* fsub fr6,fr3 */
L8c616de6: r[0]=16u;goto L8c616de8; /* mov #0x10,r0 */
L8c616de8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616dea; /* fsub fr14,fr2 */
L8c616dea: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c616dec; /* fmov.s @(r0,r12),fr1 */
L8c616dec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c616dee; /* fsub fr13,fr1 */
L8c616dee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616df0; /* fmul fr2,fr3 */
L8c616df0: fcopy(f,2u,14u);goto L8c616df2; /* fmov fr14,fr2 */
L8c616df2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616df4; /* fsub fr5,fr2 */
L8c616df4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616df6; /* fmul fr1,fr2 */
L8c616df6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c616df8; /* fsub fr2,fr3 */
L8c616df8: fcopy(f,4u,3u);goto L8c616dfa; /* fmov fr3,fr4 */
L8c616dfa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616dfc; /* fmul fr15,fr4 */
L8c616dfc: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c616dfe; /* cmp/hs r8,r9 */
L8c616dfe: if((*sr&1u))goto L8c616e2e;goto L8c616e00; /* bt 0x8c616e2e */
L8c616e00: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c616e02; /* fldi0 fr3 */
L8c616e02: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c616e04; /* fcmp/gt fr4,fr3 */
L8c616e04: if((*sr&1u))goto L8c616e58;goto L8c616e06; /* bt 0x8c616e58 */
L8c616e06: r[5]=rd(m,bus,&io_status,0x8c616facu,4);if(io_status)return io_status;goto L8c616e08; /* mov.l 0x8c616fac,r5 */
L8c616e08: r[4]=r[11];goto L8c616e0a; /* mov r11,r4 */
L8c616e0a: r[8]+=32u;goto L8c616e0c; /* add #0x20,r8 */
L8c616e0c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616e0e; /* fschg */
L8c616e0e: r[4]+=16u;goto L8c616e10; /* add #0x10,r4 */
L8c616e10: fcopy(f,0u,1u);goto L8c616e12; /* fmov fr1,fr0 */
L8c616e12: fcopy(f,2u,3u);goto L8c616e14; /* fmov fr3,fr2 */
L8c616e14: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616e16; /* fmov.s @r4+,fr4 */
L8c616e16: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616e18; /* fmov.s @r4,fr6 */
L8c616e18: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616e1a; /* fmov.s fr2,@-r8 */
L8c616e1a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616e1c; /* fmov.s fr0,@-r8 */
L8c616e1c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616e1e; /* fschg */
L8c616e1e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616e20; /* fmov.s fr6,@-r8 */
L8c616e20: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616e22; /* fmov.s fr5,@-r8 */
L8c616e22: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616e24; /* fmov.s fr4,@-r8 */
L8c616e24: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616e26; /* mov.l r5,@-r8 */
L8c616e26: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616e28; /* pref @r8 */
L8c616e28: r[8]+=32u;goto L8c616e2a; /* add #0x20,r8 */
L8c616e2a: r[9]=r[8];goto L8c616e7c; /* bra 0x8c616e7c */
L8c616e2e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c616e30; /* fldi0 fr3 */
L8c616e30: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c616e32; /* fcmp/gt fr4,fr3 */
L8c616e32: if(!(*sr&1u))goto L8c616e7c;goto L8c616e34; /* bf 0x8c616e7c */
L8c616e34: r[5]=rd(m,bus,&io_status,0x8c616fb0u,4);if(io_status)return io_status;goto L8c616e36; /* mov.l 0x8c616fb0,r5 */
L8c616e36: r[4]=r[10];goto L8c616e38; /* mov r10,r4 */
L8c616e38: r[8]+=32u;goto L8c616e3a; /* add #0x20,r8 */
L8c616e3a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616e3c; /* fschg */
L8c616e3c: r[4]+=16u;goto L8c616e3e; /* add #0x10,r4 */
L8c616e3e: fcopy(f,0u,1u);goto L8c616e40; /* fmov fr1,fr0 */
L8c616e40: fcopy(f,2u,3u);goto L8c616e42; /* fmov fr3,fr2 */
L8c616e42: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616e44; /* fmov.s @r4+,fr4 */
L8c616e44: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616e46; /* fmov.s @r4,fr6 */
L8c616e46: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616e48; /* fmov.s fr2,@-r8 */
L8c616e48: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616e4a; /* fmov.s fr0,@-r8 */
L8c616e4a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616e4c; /* fschg */
L8c616e4c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616e4e; /* fmov.s fr6,@-r8 */
L8c616e4e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616e50; /* fmov.s fr5,@-r8 */
L8c616e50: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616e52; /* fmov.s fr4,@-r8 */
L8c616e52: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616e54; /* mov.l r5,@-r8 */
L8c616e54: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616e56; /* pref @r8 */
L8c616e56: r[8]+=32u;goto L8c616e58; /* add #0x20,r8 */
L8c616e58: r[5]=rd(m,bus,&io_status,0x8c616fb0u,4);if(io_status)return io_status;goto L8c616e5a; /* mov.l 0x8c616fb0,r5 */
L8c616e5a: r[4]=r[11];goto L8c616e5c; /* mov r11,r4 */
L8c616e5c: r[8]+=32u;goto L8c616e5e; /* add #0x20,r8 */
L8c616e5e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616e60; /* fschg */
L8c616e60: r[4]+=16u;goto L8c616e62; /* add #0x10,r4 */
L8c616e62: fcopy(f,0u,1u);goto L8c616e64; /* fmov fr1,fr0 */
L8c616e64: fcopy(f,2u,3u);goto L8c616e66; /* fmov fr3,fr2 */
L8c616e66: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616e68; /* fmov.s @r4+,fr4 */
L8c616e68: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616e6a; /* fmov.s @r4,fr6 */
L8c616e6a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616e6c; /* fmov.s fr2,@-r8 */
L8c616e6c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616e6e; /* fmov.s fr0,@-r8 */
L8c616e6e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616e70; /* fschg */
L8c616e70: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616e72; /* fmov.s fr6,@-r8 */
L8c616e72: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616e74; /* fmov.s fr5,@-r8 */
L8c616e74: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616e76; /* fmov.s fr4,@-r8 */
L8c616e76: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616e78; /* mov.l r5,@-r8 */
L8c616e78: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616e7a; /* pref @r8 */
L8c616e7a: r[8]+=32u;goto L8c616e7c; /* add #0x20,r8 */
L8c616e7c: r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c616e7e; /* mov.l @(0x14,r15),r2 */
L8c616e7e: *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c616e80; /* tst r2,r2 */
L8c616e80: if((*sr&1u))goto L8c616e94;goto L8c616e82; /* bt 0x8c616e94 */
L8c616e82: r[4]=rd(m,bus,&io_status,0x8c616fb4u,4);if(io_status)return io_status;goto L8c616e84; /* mov.l 0x8c616fb4,r4 */
L8c616e84: r[3]=0u;goto L8c616e86; /* mov #0x0,r3 */
L8c616e86: r[2]=rd(m,bus,&io_status,0x8c616fb8u,4);if(io_status)return io_status;goto L8c616e88; /* mov.l 0x8c616fb8,r2 */
L8c616e88: r[5]=r[11];goto L8c616e8a; /* mov r11,r5 */
L8c616e8a: wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c616e8c; /* mov.l r3,@r4 */
L8c616e8c: r[6]=r[12];goto L8c616e8e; /* mov r12,r6 */
L8c616e8e: target=r[2];*pr=0x8c616e92u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c616e92; /* jsr @r2 */
L8c616e92: r[9]=r[8];goto L8c616e94; /* mov r8,r9 */
L8c616e94: fcopy(f,6u,13u);goto L8c616e96; /* fmov fr13,fr6 */
L8c616e96: r[0]=16u;goto L8c616e98; /* mov #0x10,r0 */
L8c616e98: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c616e9a; /* mov.l @(0x8,r15),r3 */
L8c616e9a: r[10]=r[11];goto L8c616e9c; /* mov r11,r10 */
L8c616e9c: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c616e9e; /* fmov.s @(r0,r12),fr13 */
L8c616e9e: r[0]=20u;goto L8c616ea0; /* mov #0x14,r0 */
L8c616ea0: fcopy(f,5u,14u);goto L8c616ea2; /* fmov fr14,fr5 */
L8c616ea2: r[3]+=4294967295u;goto L8c616ea4; /* add #-0x1,r3 */
L8c616ea4: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c616ea6; /* fmov.s @(r0,r12),fr14 */
L8c616ea6: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c616ea8; /* tst r3,r3 */
L8c616ea8: r[11]=r[12];goto L8c616eaa; /* mov r12,r11 */
L8c616eaa: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c616eac; /* fneg fr15 */
L8c616eac: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c616eb4;goto L8c616eb0; /* bt/s 0x8c616eb4 */
L8c616eb0: goto L8c616c6c; /* bra 0x8c616c6c */
L8c616eb4: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c616eb6; /* cmp/hs r8,r9 */
L8c616eb6: if((*sr&1u))goto L8c616edc;goto L8c616eb8; /* bt 0x8c616edc */
L8c616eb8: r[5]=rd(m,bus,&io_status,0x8c616facu,4);if(io_status)return io_status;goto L8c616eba; /* mov.l 0x8c616fac,r5 */
L8c616eba: r[4]=r[12];goto L8c616ebc; /* mov r12,r4 */
L8c616ebc: r[8]+=32u;goto L8c616ebe; /* add #0x20,r8 */
L8c616ebe: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616ec0; /* fschg */
L8c616ec0: r[4]+=16u;goto L8c616ec2; /* add #0x10,r4 */
L8c616ec2: fcopy(f,0u,1u);goto L8c616ec4; /* fmov fr1,fr0 */
L8c616ec4: fcopy(f,2u,3u);goto L8c616ec6; /* fmov fr3,fr2 */
L8c616ec6: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616ec8; /* fmov.s @r4+,fr4 */
L8c616ec8: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616eca; /* fmov.s @r4,fr6 */
L8c616eca: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616ecc; /* fmov.s fr2,@-r8 */
L8c616ecc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616ece; /* fmov.s fr0,@-r8 */
L8c616ece: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616ed0; /* fschg */
L8c616ed0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616ed2; /* fmov.s fr6,@-r8 */
L8c616ed2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616ed4; /* fmov.s fr5,@-r8 */
L8c616ed4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616ed6; /* fmov.s fr4,@-r8 */
L8c616ed6: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616ed8; /* mov.l r5,@-r8 */
L8c616ed8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616eda; /* pref @r8 */
L8c616eda: r[8]+=32u;goto L8c616edc; /* add #0x20,r8 */
L8c616edc: r[3]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c616ede; /* mov.l @(0x18,r15),r3 */
L8c616ede: r[3]+=4294967295u;goto L8c616ee0; /* add #-0x1,r3 */
L8c616ee0: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c616ee2; /* tst r3,r3 */
L8c616ee2: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;if(take)goto L8c616eea;goto L8c616ee6; /* bt/s 0x8c616eea */
L8c616ee6: goto L8c616c1c; /* bra 0x8c616c1c */
L8c616eea: r[4]=rd(m,bus,&io_status,0x8c616fbcu,4);if(io_status)return io_status;goto L8c616eec; /* mov.l 0x8c616fbc,r4 */
L8c616eec: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616eee; /* mov.l @(0xc,r15),r3 */
L8c616eee: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616ef0; /* mov.l @r4,r2 */
L8c616ef0: r[2]+=r[3];goto L8c616ef2; /* add r3,r2 */
L8c616ef2: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c616ef4; /* mov.l r2,@r4 */
L8c616ef4: r[4]=rd(m,bus,&io_status,0x8c616fc0u,4);if(io_status)return io_status;goto L8c616ef6; /* mov.l 0x8c616fc0,r4 */
L8c616ef6: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616ef8; /* mov.l @(0xc,r15),r3 */
L8c616ef8: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616efa; /* mov.l @r4,r2 */
L8c616efa: r[2]+=r[3];goto L8c616efc; /* add r3,r2 */
L8c616efc: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c616efe; /* mov.l r2,@r4 */
L8c616efe: r[15]+=76u;goto L8c616f00; /* add #0x4c,r15 */
L8c616f00: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616f02; /* lds.l @r15+,PR */
L8c616f02: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616f04; /* fmov.s @r15+,fr12 */
L8c616f04: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616f06; /* fmov.s @r15+,fr13 */
L8c616f06: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616f08; /* fmov.s @r15+,fr14 */
L8c616f08: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616f0a; /* fmov.s @r15+,fr15 */
L8c616f0a: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616f0c; /* mov.l @r15+,r9 */
L8c616f0c: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616f0e; /* mov.l @r15+,r10 */
L8c616f0e: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616f10; /* mov.l @r15+,r11 */
L8c616f10: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616f12; /* mov.l @r15+,r12 */
L8c616f12: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616f14; /* mov.l @r15+,r13 */
L8c616f14: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
