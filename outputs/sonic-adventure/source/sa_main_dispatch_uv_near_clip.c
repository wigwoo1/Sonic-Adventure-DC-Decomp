/* UV/lit triangle near-plane clipper with portable queue interface; name inferred.
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
SA_STAGE_EXPORT int sa_uv_triangle_near_clip_8c611a14(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c611a14;
L8c611a14: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c611a16; /* mov.l r14,@-r15 */
L8c611a16: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c611a18; /* mov.l r13,@-r15 */
L8c611a18: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c611a1a; /* mov.l r12,@-r15 */
L8c611a1a: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c611a1c; /* mov.l r11,@-r15 */
L8c611a1c: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c611a1e; /* mov.l r10,@-r15 */
L8c611a1e: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c611a20; /* mov.l r9,@-r15 */
L8c611a20: r[15]+=4294967224u;goto L8c611a22; /* add #-0x48,r15 */
L8c611a22: r[3]=rd(m,bus,&io_status,0x8c611ad4u,4);if(io_status)return io_status;goto L8c611a24; /* mov.l 0x8c611ad4,r3 */
L8c611a24: r[14]=r[15];goto L8c611a26; /* mov r15,r14 */
L8c611a26: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c611a28; /* fldi1 fr6 */
L8c611a28: r[9]=r[15];goto L8c611a2a; /* mov r15,r9 */
L8c611a2a: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c611a2c; /* fmov.s @r3,fr3 */
L8c611a2c: r[14]+=60u;goto L8c611a2e; /* add #0x3c,r14 */
L8c611a2e: fcopy(f,5u,6u);goto L8c611a30; /* fmov fr6,fr5 */
L8c611a30: r[9]+=24u;goto L8c611a32; /* add #0x18,r9 */
L8c611a32: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c611a34; /* fdiv fr3,fr5 */
L8c611a34: r[2]=r[14];goto L8c611a36; /* mov r14,r2 */
L8c611a36: r[11]=rd(m,bus,&io_status,0x8c611adcu,4);if(io_status)return io_status;goto L8c611a38; /* mov.l 0x8c611adc,r11 */
L8c611a38: r[13]=r[9];goto L8c611a3a; /* mov r9,r13 */
L8c611a3a: r[10]=rd(m,bus,&io_status,0x8c611ad8u,4);if(io_status)return io_status;goto L8c611a3c; /* mov.l 0x8c611ad8,r10 */
L8c611a3c: r[12]=0u;goto L8c611a3e; /* mov #0x0,r12 */
L8c611a3e: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c611a40; /* mov.l r2,@r15 */
L8c611a40: wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c611a42; /* mov.l r4,@r2 */
L8c611a42: r[4]=1u;goto L8c611a44; /* mov #0x1,r4 */
L8c611a44: wr(m,bus,&io_status,r[14]+0x4u,r[5],4);if(io_status)return io_status;goto L8c611a46; /* mov.l r5,@(0x4,r14) */
L8c611a46: r[5]=r[13];goto L8c611a48; /* mov r13,r5 */
L8c611a48: wr(m,bus,&io_status,r[14]+0x8u,r[6],4);if(io_status)return io_status;goto L8c611a4a; /* mov.l r6,@(0x8,r14) */
L8c611a4a: r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611a4c; /* mov.l @r15,r6 */
L8c611a4c: wr(m,bus,&io_status,r[15]+0x4u,r[13],4);if(io_status)return io_status;goto L8c611a4e; /* mov.l r13,@(0x4,r15) */
L8c611a4e: r[3]=r[12];goto L8c611a50; /* mov r12,r3 */
L8c611a50: r[3]<<=2;goto L8c611a52; /* shll2 r3 */
L8c611a52: r[3]+=r[14];goto L8c611a54; /* add r14,r3 */
L8c611a54: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c611a56; /* mov.l r3,@(0x8,r15) */
L8c611a56: r[0]=4u;goto L8c611a58; /* mov #0x4,r0 */
L8c611a58: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c611a5a; /* mov.l @r3,r2 */
L8c611a5a: r[3]=r[2];goto L8c611a5c; /* mov r2,r3 */
L8c611a5c: r[1]=r[2];goto L8c611a5e; /* mov r2,r1 */
L8c611a5e: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c611a60; /* mov.l r2,@r15 */
L8c611a60: fload(m,bus,&io_status,f,2u,r[10]);if(io_status)return io_status;goto L8c611a62; /* fmov.s @r10,fr2 */
L8c611a62: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c611a64; /* fmov.s @r3,fr3 */
L8c611a64: r[3]=rd(m,bus,&io_status,0x8c611ae0u,4);if(io_status)return io_status;goto L8c611a66; /* mov.l 0x8c611ae0,r3 */
L8c611a66: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611a68; /* fmul fr3,fr2 */
L8c611a68: fload(m,bus,&io_status,f,1u,r[0]+r[10]);if(io_status)return io_status;goto L8c611a6a; /* fmov.s @(r0,r10),fr1 */
L8c611a6a: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c611a6c; /* fmov.s @r3,fr3 */
L8c611a6c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611a6e; /* fsub fr2,fr3 */
L8c611a6e: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c611a70; /* fmov.s @(r0,r1),fr2 */
L8c611a70: r[0]=8u;goto L8c611a72; /* mov #0x8,r0 */
L8c611a72: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c611a74; /* fmul fr2,fr1 */
L8c611a74: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c611a76; /* fmov.s @(r0,r1),fr2 */
L8c611a76: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611a78; /* fsub fr1,fr3 */
L8c611a78: fload(m,bus,&io_status,f,1u,r[0]+r[10]);if(io_status)return io_status;goto L8c611a7a; /* fmov.s @(r0,r10),fr1 */
L8c611a7a: r[0]=28u;goto L8c611a7c; /* mov #0x1c,r0 */
L8c611a7c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c611a7e; /* fmul fr2,fr1 */
L8c611a7e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611a80; /* fsub fr1,fr3 */
L8c611a80: fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611a82; /* fmov fr3,@(r0,r2) */
L8c611a82: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c611a84; /* mov.l @(0x8,r15),r2 */
L8c611a84: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c611a86; /* mov.l @r2,r1 */
L8c611a86: r[0]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c611a88; /* mov.l @(0xc,r1),r0 */
L8c611a88: *sr=(*sr&~1u)|!(r[4]&r[0]);goto L8c611a8a; /* tst r4,r0 */
L8c611a8a: if(!(*sr&1u))goto L8c611aec;goto L8c611a8c; /* bf 0x8c611aec */
L8c611a8c: r[1]=r[12];goto L8c611a8e; /* mov r12,r1 */
L8c611a8e: r[1]<<=2;goto L8c611a90; /* shll2 r1 */
L8c611a90: r[1]+=r[14];goto L8c611a92; /* add r14,r1 */
L8c611a92: fcopy(f,4u,6u);goto L8c611a94; /* fmov fr6,fr4 */
L8c611a94: r[0]=24u;goto L8c611a96; /* mov #0x18,r0 */
L8c611a96: wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c611a98; /* mov.l r1,@r15 */
L8c611a98: r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c611a9a; /* mov.l @r1,r2 */
L8c611a9a: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c611a9c; /* mov.l r2,@(0x8,r15) */
L8c611a9c: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611a9e; /* fmov.s @(r0,r2),fr3 */
L8c611a9e: r[0]=12u;goto L8c611aa0; /* mov #0xc,r0 */
L8c611aa0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c611aa2; /* fdiv fr3,fr4 */
L8c611aa2: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c611aa4; /* fmov.s @(r0,r11),fr3 */
L8c611aa4: r[0]=16u;goto L8c611aa6; /* mov #0x10,r0 */
L8c611aa6: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c611aa8; /* fmov.s @(r0,r2),fr2 */
L8c611aa8: r[0]=16u;goto L8c611aaa; /* mov #0x10,r0 */
L8c611aaa: r[2]=rd(m,bus,&io_status,0x8c611ae4u,4);if(io_status)return io_status;goto L8c611aac; /* mov.l 0x8c611ae4,r2 */
L8c611aac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c611aae; /* fsub fr3,fr2 */
L8c611aae: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c611ab0; /* fmov.s @r2,fr3 */
L8c611ab0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611ab2; /* fmul fr4,fr2 */
L8c611ab2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611ab4; /* fdiv fr3,fr2 */
L8c611ab4: fstore(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c611ab6; /* fmov.s fr2,@r5 */
L8c611ab6: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611ab8; /* mov.l @r15,r1 */
L8c611ab8: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c611aba; /* fmov.s @(r0,r11),fr3 */
L8c611aba: r[0]=20u;goto L8c611abc; /* mov #0x14,r0 */
L8c611abc: r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c611abe; /* mov.l @r1,r1 */
L8c611abe: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c611ac0; /* fmov.s @(r0,r1),fr2 */
L8c611ac0: r[0]=4u;goto L8c611ac2; /* mov #0x4,r0 */
L8c611ac2: r[1]=rd(m,bus,&io_status,0x8c611ae8u,4);if(io_status)return io_status;goto L8c611ac4; /* mov.l 0x8c611ae8,r1 */
L8c611ac4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c611ac6; /* fsub fr3,fr2 */
L8c611ac6: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c611ac8; /* fmov.s @r1,fr3 */
L8c611ac8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611aca; /* fmul fr4,fr2 */
L8c611aca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611acc; /* fdiv fr3,fr2 */
L8c611acc: fstore(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c611ace; /* fmov fr2,@(r0,r5) */
L8c611ace: r[0]=8u;goto L8c611ad0; /* mov #0x8,r0 */
L8c611ad0: fstore(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c611b08; /* bra 0x8c611b08 */
L8c611aec: r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c611aee; /* mov.l @r6,r2 */
L8c611aee: r[0]=16u;goto L8c611af0; /* mov #0x10,r0 */
L8c611af0: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611af2; /* fmov.s @(r0,r2),fr3 */
L8c611af2: r[0]=20u;goto L8c611af4; /* mov #0x14,r0 */
L8c611af4: fstore(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c611af6; /* fmov.s fr3,@r13 */
L8c611af6: r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c611af8; /* mov.l @r6,r2 */
L8c611af8: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611afa; /* fmov.s @(r0,r2),fr3 */
L8c611afa: r[0]=4u;goto L8c611afc; /* mov #0x4,r0 */
L8c611afc: fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c611afe; /* fmov fr3,@(r0,r13) */
L8c611afe: r[0]=24u;goto L8c611b00; /* mov #0x18,r0 */
L8c611b00: r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c611b02; /* mov.l @r6,r2 */
L8c611b02: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611b04; /* fmov.s @(r0,r2),fr3 */
L8c611b04: r[0]=8u;goto L8c611b06; /* mov #0x8,r0 */
L8c611b06: fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c611b08; /* fmov fr3,@(r0,r13) */
L8c611b08: r[3]=3u;goto L8c611b0a; /* mov #0x3,r3 */
L8c611b0a: r[12]+=1u;goto L8c611b0c; /* add #0x1,r12 */
L8c611b0c: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[3]);goto L8c611b0e; /* cmp/ge r3,r12 */
L8c611b0e: r[6]+=4u;goto L8c611b10; /* add #0x4,r6 */
L8c611b10: r[13]+=12u;goto L8c611b12; /* add #0xc,r13 */
L8c611b12: take=!(*sr&1u);r[5]+=12u;if(take)goto L8c611a4e;goto L8c611b16; /* bf/s 0x8c611a4e */
L8c611b16: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611b18; /* mov.l @(0x4,r15),r3 */
L8c611b18: r[0]=8u;goto L8c611b1a; /* mov #0x8,r0 */
L8c611b1a: r[6]=r[9];goto L8c611b1c; /* mov r9,r6 */
L8c611b1c: r[6]+=12u;goto L8c611b1e; /* add #0xc,r6 */
L8c611b1e: fload(m,bus,&io_status,f,7u,r[0]+r[3]);if(io_status)return io_status;goto L8c611b20; /* fmov.s @(r0,r3),fr7 */
L8c611b20: r[0]=4u;goto L8c611b22; /* mov #0x4,r0 */
L8c611b22: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c611b24; /* fmov.s @(r0,r6),fr4 */
L8c611b24: r[5]=r[9];goto L8c611b26; /* mov r9,r5 */
L8c611b26: fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c611b28; /* fmov.s @(r0,r3),fr9 */
L8c611b28: r[0]=8u;goto L8c611b2a; /* mov #0x8,r0 */
L8c611b2a: fload(m,bus,&io_status,f,6u,r[0]+r[6]);if(io_status)return io_status;goto L8c611b2c; /* fmov.s @(r0,r6),fr6 */
L8c611b2c: r[0]=4u;goto L8c611b2e; /* mov #0x4,r0 */
L8c611b2e: fcopy(f,3u,9u);goto L8c611b30; /* fmov fr9,fr3 */
L8c611b30: r[5]+=24u;goto L8c611b32; /* add #0x18,r5 */
L8c611b32: fcopy(f,0u,7u);goto L8c611b34; /* fmov fr7,fr0 */
L8c611b34: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611b36; /* fmul fr6,fr3 */
L8c611b36: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c611b38; /* fmul fr4,fr0 */
L8c611b38: fload(m,bus,&io_status,f,10u,r[3]);if(io_status)return io_status;goto L8c611b3a; /* fmov.s @r3,fr10 */
L8c611b3a: fload(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;goto L8c611b3c; /* fmov.s @r6,fr8 */
L8c611b3c: fcopy(f,2u,7u);goto L8c611b3e; /* fmov fr7,fr2 */
L8c611b3e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611b40; /* fmul fr8,fr2 */
L8c611b40: fcopy(f,1u,8u);goto L8c611b42; /* fmov fr8,fr1 */
L8c611b42: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c611b44; /* fsub fr3,fr0 */
L8c611b44: fcopy(f,3u,10u);goto L8c611b46; /* fmov fr10,fr3 */
L8c611b46: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611b48; /* fmul fr6,fr3 */
L8c611b48: fcopy(f,11u,10u);goto L8c611b4a; /* fmov fr10,fr11 */
L8c611b4a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[9],&f->fpscr,&fr[1]);if(status)return status;}goto L8c611b4c; /* fmul fr9,fr1 */
L8c611b4c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611b4e; /* fsub fr2,fr3 */
L8c611b4e: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c611b50; /* fmov.s @(r0,r5),fr2 */
L8c611b50: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611b52; /* fmul fr3,fr2 */
L8c611b52: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c611b54; /* fmov.s @r5,fr3 */
L8c611b54: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611b56; /* fmac fr0,fr3,fr2 */
L8c611b56: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[4],&f->fpscr,&fr[11]);if(status)return status;}goto L8c611b58; /* fmul fr4,fr11 */
L8c611b58: r[0]=8u;goto L8c611b5a; /* mov #0x8,r0 */
L8c611b5a: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611b5c; /* fmov.s @(r0,r5),fr3 */
L8c611b5c: r[5]=rd(m,bus,&io_status,0x8c611decu,4);if(io_status)return io_status;goto L8c611b5e; /* mov.l 0x8c611dec,r5 */
L8c611b5e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[11],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c611b60; /* fsub fr11,fr1 */
L8c611b60: r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c611b62; /* mov.l @r5,r3 */
L8c611b62: fcopy(f,0u,1u);goto L8c611b64; /* fmov fr1,fr0 */
L8c611b64: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611b66; /* fmac fr0,fr3,fr2 */
L8c611b66: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c611b68; /* fldi0 fr3 */
L8c611b68: fcopy(f,4u,2u);goto L8c611b6a; /* fmov fr2,fr4 */
L8c611b6a: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c611b6c; /* fcmp/gt fr4,fr3 */
L8c611b6c: r[0]=*sr&1u;goto L8c611b6e; /* movt r0 */
L8c611b6e: *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c611b70; /* tst r3,r0 */
L8c611b70: if((*sr&1u))goto L8c611b76;goto L8c611b72; /* bt 0x8c611b76 */
L8c611b72: goto L8c611f14; /* bra 0x8c611f14 */
L8c611b76: r[1]=0u;goto L8c611b78; /* mov #0x0,r1 */
L8c611b78: r[12]=rd(m,bus,&io_status,0x8c611df0u,4);if(io_status)return io_status;goto L8c611b7a; /* mov.l 0x8c611df0,r12 */
L8c611b7a: wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c611b7c; /* mov.l r1,@r5 */
L8c611b7c: r[3]=2u;goto L8c611b7e; /* mov #0x2,r3 */
L8c611b7e: r[6]=rd(m,bus,&io_status,0x8c611df4u,4);if(io_status)return io_status;goto L8c611b80; /* mov.l 0x8c611df4,r6 */
L8c611b80: r[5]=r[1];goto L8c611b82; /* mov r1,r5 */
L8c611b82: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c611b84; /* mov.l r3,@(0x4,r15) */
L8c611b84: r[10]=r[5];goto L8c611b86; /* mov r5,r10 */
L8c611b86: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611b88; /* mov.l @(0x4,r15),r3 */
L8c611b88: r[0]=r[14];goto L8c611b8a; /* mov r14,r0 */
L8c611b8a: r[13]=r[10];goto L8c611b8c; /* mov r10,r13 */
L8c611b8c: r[13]<<=2;goto L8c611b8e; /* shll2 r13 */
L8c611b8e: r[3]<<=2;goto L8c611b90; /* shll2 r3 */
L8c611b90: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c611b92; /* mov.l r3,@r15 */
L8c611b92: r[2]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c611b94; /* mov.l @(r0,r3),r2 */
L8c611b94: r[3]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c611b96; /* mov.l @(0xc,r2),r3 */
L8c611b96: r[2]=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;goto L8c611b98; /* mov.l @(r0,r13),r2 */
L8c611b98: r[3]&=r[4];goto L8c611b9a; /* and r4,r3 */
L8c611b9a: if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[2]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c611b9c; /* mov.l @(0xc,r2),r2 */
L8c611b9c: r[2]&=r[4];goto L8c611b9e; /* and r4,r2 */
L8c611b9e: r[3]^=r[2];goto L8c611ba0; /* xor r2,r3 */
L8c611ba0: r[0]=r[3];goto L8c611ba2; /* mov r3,r0 */
L8c611ba2: *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c611ba4; /* cmp/eq #0x1,r0 */
L8c611ba4: if((*sr&1u))goto L8c611baa;goto L8c611ba6; /* bt 0x8c611baa */
L8c611ba6: goto L8c611cac; /* bra 0x8c611cac */
L8c611baa: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611bac; /* mov.l @(0x4,r15),r2 */
L8c611bac: r[0]=8u;goto L8c611bae; /* mov #0x8,r0 */
L8c611bae: r[1]=r[10];goto L8c611bb0; /* mov r10,r1 */
L8c611bb0: r[3]=r[2];goto L8c611bb2; /* mov r2,r3 */
L8c611bb2: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c611bb4; /* shll r2 */
L8c611bb4: r[2]+=r[3];goto L8c611bb6; /* add r3,r2 */
L8c611bb6: r[3]=rd(m,bus,&io_status,0x8c611df8u,4);if(io_status)return io_status;goto L8c611bb8; /* mov.l 0x8c611df8,r3 */
L8c611bb8: r[2]<<=2;goto L8c611bba; /* shll2 r2 */
L8c611bba: r[2]+=r[9];goto L8c611bbc; /* add r9,r2 */
L8c611bbc: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c611bbe; /* mov.l r2,@(0xc,r15) */
L8c611bbe: fload(m,bus,&io_status,f,4u,r[0]+r[2]);if(io_status)return io_status;goto L8c611bc0; /* fmov.s @(r0,r2),fr4 */
L8c611bc0: r[2]=r[10];goto L8c611bc2; /* mov r10,r2 */
L8c611bc2: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c611bc4; /* shll r2 */
L8c611bc4: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c611bc6; /* fmov.s @r3,fr3 */
L8c611bc6: r[2]+=r[1];goto L8c611bc8; /* add r1,r2 */
L8c611bc8: r[2]<<=2;goto L8c611bca; /* shll2 r2 */
L8c611bca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611bcc; /* fsub fr4,fr3 */
L8c611bcc: r[0]=8u;goto L8c611bce; /* mov #0x8,r0 */
L8c611bce: r[2]+=r[9];goto L8c611bd0; /* add r9,r2 */
L8c611bd0: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c611bd2; /* mov.l r2,@(0x8,r15) */
L8c611bd2: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c611bd4; /* fmov.s @(r0,r2),fr2 */
L8c611bd4: r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c611bd6; /* mov.l @(0x8,r15),r1 */
L8c611bd6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c611bd8; /* fsub fr4,fr2 */
L8c611bd8: fcopy(f,4u,3u);goto L8c611bda; /* fmov fr3,fr4 */
L8c611bda: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c611bdc; /* mov.l @(0xc,r15),r2 */
L8c611bdc: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c611bde; /* fmov.s @r1,fr3 */
L8c611bde: fload(m,bus,&io_status,f,6u,r[2]);if(io_status)return io_status;goto L8c611be0; /* fmov.s @r2,fr6 */
L8c611be0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c611be2; /* fdiv fr2,fr4 */
L8c611be2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611be4; /* fsub fr6,fr3 */
L8c611be4: fcopy(f,2u,6u);goto L8c611be6; /* fmov fr6,fr2 */
L8c611be6: fcopy(f,0u,4u);goto L8c611be8; /* fmov fr4,fr0 */
L8c611be8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611bea; /* fmac fr0,fr3,fr2 */
L8c611bea: r[1]=rd(m,bus,&io_status,0x8c611dfcu,4);if(io_status)return io_status;goto L8c611bec; /* mov.l 0x8c611dfc,r1 */
L8c611bec: r[0]=12u;goto L8c611bee; /* mov #0xc,r0 */
L8c611bee: fcopy(f,6u,2u);goto L8c611bf0; /* fmov fr2,fr6 */
L8c611bf0: r[2]=r[5];goto L8c611bf2; /* mov r5,r2 */
L8c611bf2: fcopy(f,0u,5u);goto L8c611bf4; /* fmov fr5,fr0 */
L8c611bf4: r[2]<<=2;goto L8c611bf6; /* shll2 r2 */
L8c611bf6: r[2]<<=2;goto L8c611bf8; /* shll2 r2 */
L8c611bf8: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c611bfa; /* shll r2 */
L8c611bfa: r[2]+=r[12];goto L8c611bfc; /* add r12,r2 */
L8c611bfc: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c611bfe; /* mov.l r2,@(0x4,r15) */
L8c611bfe: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c611c00; /* fmov.s @r1,fr3 */
L8c611c00: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c611c02; /* fmov.s @(r0,r11),fr2 */
L8c611c02: r[0]=16u;goto L8c611c04; /* mov #0x10,r0 */
L8c611c04: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611c06; /* fmul fr6,fr3 */
L8c611c06: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611c08; /* fmac fr0,fr3,fr2 */
L8c611c08: fcopy(f,0u,4u);goto L8c611c0a; /* fmov fr4,fr0 */
L8c611c0a: fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c611c0c; /* fmov fr2,@(r0,r2) */
L8c611c0c: r[0]=4u;goto L8c611c0e; /* mov #0x4,r0 */
L8c611c0e: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c611c10; /* mov.l @(0xc,r15),r2 */
L8c611c10: fload(m,bus,&io_status,f,6u,r[0]+r[2]);if(io_status)return io_status;goto L8c611c12; /* fmov.s @(r0,r2),fr6 */
L8c611c12: r[0]=4u;goto L8c611c14; /* mov #0x4,r0 */
L8c611c14: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c611c16; /* mov.l @(0x8,r15),r2 */
L8c611c16: fcopy(f,2u,6u);goto L8c611c18; /* fmov fr6,fr2 */
L8c611c18: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611c1a; /* fmov.s @(r0,r2),fr3 */
L8c611c1a: r[0]=rd(m,bus,&io_status,0x8c611e00u,4);if(io_status)return io_status;goto L8c611c1c; /* mov.l 0x8c611e00,r0 */
L8c611c1c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611c1e; /* fsub fr6,fr3 */
L8c611c1e: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611c20; /* mov.l @(0x4,r15),r2 */
L8c611c20: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611c22; /* fmac fr0,fr3,fr2 */
L8c611c22: fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c611c24; /* fmov.s @r0,fr3 */
L8c611c24: r[0]=16u;goto L8c611c26; /* mov #0x10,r0 */
L8c611c26: fcopy(f,6u,2u);goto L8c611c28; /* fmov fr2,fr6 */
L8c611c28: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611c2a; /* fmul fr6,fr3 */
L8c611c2a: fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c611c2c; /* fmov.s @(r0,r11),fr2 */
L8c611c2c: r[0]=20u;goto L8c611c2e; /* mov #0x14,r0 */
L8c611c2e: fcopy(f,0u,5u);goto L8c611c30; /* fmov fr5,fr0 */
L8c611c30: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611c32; /* fmac fr0,fr3,fr2 */
L8c611c32: fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c611c34; /* fmov fr2,@(r0,r2) */
L8c611c34: r[0]=24u;goto L8c611c36; /* mov #0x18,r0 */
L8c611c36: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611c38; /* mov.l @(0x4,r15),r3 */
L8c611c38: fstore(m,bus,&io_status,f,0u,r[0]+r[3]);if(io_status)return io_status;goto L8c611c3a; /* fmov fr0,@(r0,r3) */
L8c611c3a: r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611c3c; /* mov.l @r15,r0 */
L8c611c3c: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611c3e; /* mov.l @(0x4,r15),r3 */
L8c611c3e: r[2]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c611c40; /* mov.l @(r0,r14),r2 */
L8c611c40: r[0]=28u;goto L8c611c42; /* mov #0x1c,r0 */
L8c611c42: fcopy(f,0u,4u);goto L8c611c44; /* fmov fr4,fr0 */
L8c611c44: wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c611c46; /* mov.l r2,@(0x10,r15) */
L8c611c46: fload(m,bus,&io_status,f,6u,r[0]+r[2]);if(io_status)return io_status;goto L8c611c48; /* fmov.s @(r0,r2),fr6 */
L8c611c48: r[0]=r[14];goto L8c611c4a; /* mov r14,r0 */
L8c611c4a: r[2]=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;goto L8c611c4c; /* mov.l @(r0,r13),r2 */
L8c611c4c: r[0]=28u;goto L8c611c4e; /* mov #0x1c,r0 */
L8c611c4e: fcopy(f,2u,6u);goto L8c611c50; /* fmov fr6,fr2 */
L8c611c50: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611c52; /* fmov.s @(r0,r2),fr3 */
L8c611c52: r[2]=r[0];goto L8c611c54; /* mov r0,r2 */
L8c611c54: r[2]+=r[3];goto L8c611c56; /* add r3,r2 */
L8c611c56: r[3]=r[5];goto L8c611c58; /* mov r5,r3 */
L8c611c58: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611c5a; /* fsub fr6,fr3 */
L8c611c5a: r[3]<<=2;goto L8c611c5c; /* shll2 r3 */
L8c611c5c: r[3]+=r[6];goto L8c611c5e; /* add r6,r3 */
L8c611c5e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611c60; /* fmac fr0,fr3,fr2 */
L8c611c60: fstore(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c611c62; /* fmov.s fr2,@r2 */
L8c611c62: wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c611c64; /* mov.l r3,@(0xc,r15) */
L8c611c64: r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611c66; /* mov.l @r15,r0 */
L8c611c66: r[0]+=r[7];goto L8c611c68; /* add r7,r0 */
L8c611c68: wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;goto L8c611c6a; /* mov.l r0,@(0x8,r15) */
L8c611c6a: r[2]=r[13];goto L8c611c6c; /* mov r13,r2 */
L8c611c6c: r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c611c6e; /* mov.w @r0,r0 */
L8c611c6e: r[2]+=r[7];goto L8c611c70; /* add r7,r2 */
L8c611c70: wr(m,bus,&io_status,r[15]+0x14u,r[0],2);if(io_status)return io_status;goto L8c611c72; /* mov.w r0,@(0x14,r15) */
L8c611c72: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c611c74; /* mov.l r2,@(0x4,r15) */
L8c611c74: *fpul=r[0];goto L8c611c76; /* lds r0,FPUL */
L8c611c76: r[0]=sx(m,bus,&io_status,r[15]+0x14u,2);if(io_status)return io_status;goto L8c611c78; /* mov.w @(0x14,r15),r0 */
L8c611c78: r[2]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c611c7a; /* mov.w @r2,r2 */
L8c611c7a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611c7c; /* float FPUL,fr3 */
L8c611c7c: r[2]-=r[0];goto L8c611c7e; /* sub r0,r2 */
L8c611c7e: *fpul=r[2];goto L8c611c80; /* lds r2,FPUL */
L8c611c80: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c611c82; /* float FPUL,fr2 */
L8c611c82: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611c84; /* fmac fr0,fr2,fr3 */
L8c611c84: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c611c86; /* ftrc fr3,FPUL */
L8c611c86: r[2]=*fpul;goto L8c611c88; /* sts FPUL,r2 */
L8c611c88: wr(m,bus,&io_status,r[3],r[2],2);if(io_status)return io_status;goto L8c611c8a; /* mov.w r2,@r3 */
L8c611c8a: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c611c8c; /* mov.l @(0x8,r15),r2 */
L8c611c8c: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c611c8e; /* mov.l @(0xc,r15),r3 */
L8c611c8e: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c611c90; /* mov.w @(0x2,r2),r0 */
L8c611c90: *fpul=r[0];goto L8c611c92; /* lds r0,FPUL */
L8c611c92: wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c611c94; /* mov.w r0,@r15 */
L8c611c94: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611c96; /* mov.l @(0x4,r15),r2 */
L8c611c96: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611c98; /* float FPUL,fr3 */
L8c611c98: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c611c9a; /* mov.w @(0x2,r2),r0 */
L8c611c9a: r[2]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c611c9c; /* mov.w @r15,r2 */
L8c611c9c: r[0]-=r[2];goto L8c611c9e; /* sub r2,r0 */
L8c611c9e: *fpul=r[0];goto L8c611ca0; /* lds r0,FPUL */
L8c611ca0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c611ca2; /* float FPUL,fr2 */
L8c611ca2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611ca4; /* fmac fr0,fr2,fr3 */
L8c611ca4: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c611ca6; /* ftrc fr3,FPUL */
L8c611ca6: r[0]=*fpul;goto L8c611ca8; /* sts FPUL,r0 */
L8c611ca8: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c611caa; /* mov.w r0,@(0x2,r3) */
L8c611caa: r[5]+=1u;goto L8c611cac; /* add #0x1,r5 */
L8c611cac: r[0]=r[13];goto L8c611cae; /* mov r13,r0 */
L8c611cae: r[3]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c611cb0; /* mov.l @(r0,r14),r3 */
L8c611cb0: r[3]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c611cb2; /* mov.l @(0xc,r3),r3 */
L8c611cb2: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c611cb4; /* tst r4,r3 */
L8c611cb4: if(!(*sr&1u))goto L8c611d10;goto L8c611cb6; /* bf 0x8c611d10 */
L8c611cb6: r[3]=r[13];goto L8c611cb8; /* mov r13,r3 */
L8c611cb8: r[2]=r[5];goto L8c611cba; /* mov r5,r2 */
L8c611cba: r[3]+=r[14];goto L8c611cbc; /* add r14,r3 */
L8c611cbc: r[2]<<=2;goto L8c611cbe; /* shll2 r2 */
L8c611cbe: r[0]=16u;goto L8c611cc0; /* mov #0x10,r0 */
L8c611cc0: r[2]<<=2;goto L8c611cc2; /* shll2 r2 */
L8c611cc2: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c611cc4; /* shll r2 */
L8c611cc4: r[2]+=r[12];goto L8c611cc6; /* add r12,r2 */
L8c611cc6: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c611cc8; /* mov.l r2,@r15 */
L8c611cc8: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c611cca; /* mov.l r3,@(0x4,r15) */
L8c611cca: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c611ccc; /* mov.l @r3,r1 */
L8c611ccc: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c611cce; /* fmov.s @(r0,r1),fr3 */
L8c611cce: fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611cd0; /* fmov fr3,@(r0,r2) */
L8c611cd0: r[0]=20u;goto L8c611cd2; /* mov #0x14,r0 */
L8c611cd2: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611cd4; /* mov.l @(0x4,r15),r2 */
L8c611cd4: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611cd6; /* mov.l @r15,r3 */
L8c611cd6: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c611cd8; /* mov.l @r2,r1 */
L8c611cd8: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c611cda; /* fmov.s @(r0,r1),fr3 */
L8c611cda: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c611cdc; /* fmov fr3,@(r0,r3) */
L8c611cdc: r[0]=24u;goto L8c611cde; /* mov #0x18,r0 */
L8c611cde: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611ce0; /* mov.l @(0x4,r15),r2 */
L8c611ce0: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611ce2; /* mov.l @r15,r3 */
L8c611ce2: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c611ce4; /* mov.l @r2,r1 */
L8c611ce4: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c611ce6; /* fmov.s @(r0,r1),fr3 */
L8c611ce6: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c611ce8; /* fmov fr3,@(r0,r3) */
L8c611ce8: r[0]=28u;goto L8c611cea; /* mov #0x1c,r0 */
L8c611cea: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611cec; /* mov.l @(0x4,r15),r2 */
L8c611cec: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611cee; /* mov.l @r15,r3 */
L8c611cee: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c611cf0; /* mov.l @r2,r1 */
L8c611cf0: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c611cf2; /* fmov.s @(r0,r1),fr3 */
L8c611cf2: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c611cf4; /* fmov fr3,@(r0,r3) */
L8c611cf4: r[3]=r[5];goto L8c611cf6; /* mov r5,r3 */
L8c611cf6: r[2]=r[13];goto L8c611cf8; /* mov r13,r2 */
L8c611cf8: r[2]+=r[7];goto L8c611cfa; /* add r7,r2 */
L8c611cfa: r[3]<<=2;goto L8c611cfc; /* shll2 r3 */
L8c611cfc: r[3]+=r[6];goto L8c611cfe; /* add r6,r3 */
L8c611cfe: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c611d00; /* mov.l r3,@(0x4,r15) */
L8c611d00: r[5]+=1u;goto L8c611d02; /* add #0x1,r5 */
L8c611d02: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c611d04; /* mov.l r2,@r15 */
L8c611d04: r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c611d06; /* mov.w @r2,r1 */
L8c611d06: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c611d08; /* mov.w r1,@r3 */
L8c611d08: r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611d0a; /* mov.l @r15,r0 */
L8c611d0a: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611d0c; /* mov.l @(0x4,r15),r3 */
L8c611d0c: r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c611d0e; /* mov.w @(0x2,r0),r0 */
L8c611d0e: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c611d10; /* mov.w r0,@(0x2,r3) */
L8c611d10: wr(m,bus,&io_status,r[15]+0x4u,r[10],4);if(io_status)return io_status;goto L8c611d12; /* mov.l r10,@(0x4,r15) */
L8c611d12: r[3]=3u;goto L8c611d14; /* mov #0x3,r3 */
L8c611d14: r[10]+=1u;goto L8c611d16; /* add #0x1,r10 */
L8c611d16: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[3]);goto L8c611d18; /* cmp/ge r3,r10 */
L8c611d18: if((*sr&1u))goto L8c611d1e;goto L8c611d1a; /* bt 0x8c611d1e */
L8c611d1a: goto L8c611b86; /* bra 0x8c611b86 */
L8c611d1e: r[0]=0x8c611e0cu;goto L8c611d20; /* mova 0x8c611e0c,r0 */
L8c611d20: r[4]=rd(m,bus,&io_status,0x8c611e08u,4);if(io_status)return io_status;goto L8c611d22; /* mov.l 0x8c611e08,r4 */
L8c611d22: fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c611d24; /* fmov.s @r0,fr4 */
L8c611d24: r[0]=r[5];goto L8c611d26; /* mov r5,r0 */
L8c611d26: r[7]=rd(m,bus,&io_status,0x8c611e04u,4);if(io_status)return io_status;goto L8c611d28; /* mov.l 0x8c611e04,r7 */
L8c611d28: *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c611d2a; /* cmp/eq #0x3,r0 */
L8c611d2a: if(!(*sr&1u))goto L8c611e14;goto L8c611d2c; /* bf 0x8c611e14 */
L8c611d2c: r[0]=28u;goto L8c611d2e; /* mov #0x1c,r0 */
L8c611d2e: r[5]=r[12];goto L8c611d30; /* mov r12,r5 */
L8c611d30: r[3]=r[6];goto L8c611d32; /* mov r6,r3 */
L8c611d32: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611d34; /* fmov.s @(r0,r5),fr3 */
L8c611d34: r[8]+=28u;goto L8c611d36; /* add #0x1c,r8 */
L8c611d36: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d38; /* fmov.s fr3,@-r8 */
L8c611d38: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c611d3a; /* mov.l r3,@r15 */
L8c611d3a: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c611d3c; /* mov.w @(0x2,r3),r0 */
L8c611d3c: r[3]=r[0];goto L8c611d3e; /* mov r0,r3 */
L8c611d3e: *fpul=r[3];goto L8c611d40; /* lds r3,FPUL */
L8c611d40: r[0]=24u;goto L8c611d42; /* mov #0x18,r0 */
L8c611d42: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611d44; /* float FPUL,fr3 */
L8c611d44: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611d46; /* fmul fr4,fr3 */
L8c611d46: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d48; /* fmov.s fr3,@-r8 */
L8c611d48: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611d4a; /* mov.l @r15,r3 */
L8c611d4a: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c611d4c; /* mov.w @r3,r3 */
L8c611d4c: *fpul=r[3];goto L8c611d4e; /* lds r3,FPUL */
L8c611d4e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611d50; /* float FPUL,fr3 */
L8c611d50: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611d52; /* fmul fr4,fr3 */
L8c611d52: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d54; /* fmov.s fr3,@-r8 */
L8c611d54: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611d56; /* fmov.s @(r0,r5),fr3 */
L8c611d56: r[0]=20u;goto L8c611d58; /* mov #0x14,r0 */
L8c611d58: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d5a; /* fmov.s fr3,@-r8 */
L8c611d5a: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611d5c; /* fmov.s @(r0,r5),fr3 */
L8c611d5c: r[0]=16u;goto L8c611d5e; /* mov #0x10,r0 */
L8c611d5e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d60; /* fmov.s fr3,@-r8 */
L8c611d60: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611d62; /* fmov.s @(r0,r5),fr3 */
L8c611d62: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d64; /* fmov.s fr3,@-r8 */
L8c611d64: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c611d66; /* mov.l r4,@-r8 */
L8c611d66: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c611d68; /* pref @r8 */
L8c611d68: r[0]=28u;goto L8c611d6a; /* mov #0x1c,r0 */
L8c611d6a: r[3]=r[6];goto L8c611d6c; /* mov r6,r3 */
L8c611d6c: r[5]+=32u;goto L8c611d6e; /* add #0x20,r5 */
L8c611d6e: r[3]+=4u;goto L8c611d70; /* add #0x4,r3 */
L8c611d70: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611d72; /* fmov.s @(r0,r5),fr3 */
L8c611d72: r[8]+=60u;goto L8c611d74; /* add #0x3c,r8 */
L8c611d74: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d76; /* fmov.s fr3,@-r8 */
L8c611d76: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c611d78; /* mov.l r3,@r15 */
L8c611d78: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c611d7a; /* mov.w @(0x2,r3),r0 */
L8c611d7a: r[3]=r[0];goto L8c611d7c; /* mov r0,r3 */
L8c611d7c: *fpul=r[3];goto L8c611d7e; /* lds r3,FPUL */
L8c611d7e: r[0]=24u;goto L8c611d80; /* mov #0x18,r0 */
L8c611d80: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611d82; /* float FPUL,fr3 */
L8c611d82: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611d84; /* fmul fr4,fr3 */
L8c611d84: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d86; /* fmov.s fr3,@-r8 */
L8c611d86: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611d88; /* mov.l @r15,r3 */
L8c611d88: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c611d8a; /* mov.w @r3,r3 */
L8c611d8a: *fpul=r[3];goto L8c611d8c; /* lds r3,FPUL */
L8c611d8c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611d8e; /* float FPUL,fr3 */
L8c611d8e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611d90; /* fmul fr4,fr3 */
L8c611d90: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d92; /* fmov.s fr3,@-r8 */
L8c611d92: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611d94; /* fmov.s @(r0,r5),fr3 */
L8c611d94: r[0]=20u;goto L8c611d96; /* mov #0x14,r0 */
L8c611d96: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d98; /* fmov.s fr3,@-r8 */
L8c611d98: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611d9a; /* fmov.s @(r0,r5),fr3 */
L8c611d9a: r[0]=16u;goto L8c611d9c; /* mov #0x10,r0 */
L8c611d9c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d9e; /* fmov.s fr3,@-r8 */
L8c611d9e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611da0; /* fmov.s @(r0,r5),fr3 */
L8c611da0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611da2; /* fmov.s fr3,@-r8 */
L8c611da2: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c611da4; /* mov.l r4,@-r8 */
L8c611da4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c611da6; /* pref @r8 */
L8c611da6: r[4]=r[12];goto L8c611da8; /* mov r12,r4 */
L8c611da8: r[0]=28u;goto L8c611daa; /* mov #0x1c,r0 */
L8c611daa: r[5]=r[6];goto L8c611dac; /* mov r6,r5 */
L8c611dac: r[4]+=64u;goto L8c611dae; /* add #0x40,r4 */
L8c611dae: r[5]+=8u;goto L8c611db0; /* add #0x8,r5 */
L8c611db0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611db2; /* fmov.s @(r0,r4),fr3 */
L8c611db2: r[8]+=60u;goto L8c611db4; /* add #0x3c,r8 */
L8c611db4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611db6; /* fmov.s fr3,@-r8 */
L8c611db6: r[0]=sx(m,bus,&io_status,r[5]+0x2u,2);if(io_status)return io_status;goto L8c611db8; /* mov.w @(0x2,r5),r0 */
L8c611db8: r[3]=r[0];goto L8c611dba; /* mov r0,r3 */
L8c611dba: *fpul=r[3];goto L8c611dbc; /* lds r3,FPUL */
L8c611dbc: r[0]=24u;goto L8c611dbe; /* mov #0x18,r0 */
L8c611dbe: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611dc0; /* float FPUL,fr3 */
L8c611dc0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611dc2; /* fmul fr4,fr3 */
L8c611dc2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611dc4; /* fmov.s fr3,@-r8 */
L8c611dc4: r[3]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;goto L8c611dc6; /* mov.w @r5,r3 */
L8c611dc6: *fpul=r[3];goto L8c611dc8; /* lds r3,FPUL */
L8c611dc8: r[3]=rd(m,bus,&io_status,0x8c611e10u,4);if(io_status)return io_status;goto L8c611dca; /* mov.l 0x8c611e10,r3 */
L8c611dca: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611dcc; /* float FPUL,fr3 */
L8c611dcc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611dce; /* fmul fr4,fr3 */
L8c611dce: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611dd0; /* fmov.s fr3,@-r8 */
L8c611dd0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611dd2; /* fmov.s @(r0,r4),fr3 */
L8c611dd2: r[0]=20u;goto L8c611dd4; /* mov #0x14,r0 */
L8c611dd4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611dd6; /* fmov.s fr3,@-r8 */
L8c611dd6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611dd8; /* fmov.s @(r0,r4),fr3 */
L8c611dd8: r[0]=16u;goto L8c611dda; /* mov #0x10,r0 */
L8c611dda: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611ddc; /* fmov.s fr3,@-r8 */
L8c611ddc: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611dde; /* fmov.s @(r0,r4),fr3 */
L8c611dde: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611de0; /* fmov.s fr3,@-r8 */
L8c611de0: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c611de2; /* mov.l r3,@-r8 */
L8c611de2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c611de4; /* pref @r8 */
L8c611de4: r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c611de6; /* mov.l @r7,r2 */
L8c611de6: r[8]+=32u;goto L8c611de8; /* add #0x20,r8 */
L8c611de8: r[2]+=3u;goto L8c611f12; /* bra 0x8c611f12 */
L8c611e14: r[0]=28u;goto L8c611e16; /* mov #0x1c,r0 */
L8c611e16: r[5]=r[12];goto L8c611e18; /* mov r12,r5 */
L8c611e18: r[3]=r[6];goto L8c611e1a; /* mov r6,r3 */
L8c611e1a: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e1c; /* fmov.s @(r0,r5),fr3 */
L8c611e1c: r[8]+=28u;goto L8c611e1e; /* add #0x1c,r8 */
L8c611e1e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e20; /* fmov.s fr3,@-r8 */
L8c611e20: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c611e22; /* mov.l r3,@r15 */
L8c611e22: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c611e24; /* mov.w @(0x2,r3),r0 */
L8c611e24: r[3]=r[0];goto L8c611e26; /* mov r0,r3 */
L8c611e26: *fpul=r[3];goto L8c611e28; /* lds r3,FPUL */
L8c611e28: r[0]=24u;goto L8c611e2a; /* mov #0x18,r0 */
L8c611e2a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611e2c; /* float FPUL,fr3 */
L8c611e2c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611e2e; /* fmul fr4,fr3 */
L8c611e2e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e30; /* fmov.s fr3,@-r8 */
L8c611e30: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611e32; /* mov.l @r15,r3 */
L8c611e32: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c611e34; /* mov.w @r3,r3 */
L8c611e34: *fpul=r[3];goto L8c611e36; /* lds r3,FPUL */
L8c611e36: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611e38; /* float FPUL,fr3 */
L8c611e38: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611e3a; /* fmul fr4,fr3 */
L8c611e3a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e3c; /* fmov.s fr3,@-r8 */
L8c611e3c: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e3e; /* fmov.s @(r0,r5),fr3 */
L8c611e3e: r[0]=20u;goto L8c611e40; /* mov #0x14,r0 */
L8c611e40: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e42; /* fmov.s fr3,@-r8 */
L8c611e42: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e44; /* fmov.s @(r0,r5),fr3 */
L8c611e44: r[0]=16u;goto L8c611e46; /* mov #0x10,r0 */
L8c611e46: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e48; /* fmov.s fr3,@-r8 */
L8c611e48: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e4a; /* fmov.s @(r0,r5),fr3 */
L8c611e4a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e4c; /* fmov.s fr3,@-r8 */
L8c611e4c: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c611e4e; /* mov.l r4,@-r8 */
L8c611e4e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c611e50; /* pref @r8 */
L8c611e50: r[0]=28u;goto L8c611e52; /* mov #0x1c,r0 */
L8c611e52: r[3]=r[6];goto L8c611e54; /* mov r6,r3 */
L8c611e54: r[5]+=32u;goto L8c611e56; /* add #0x20,r5 */
L8c611e56: r[3]+=4u;goto L8c611e58; /* add #0x4,r3 */
L8c611e58: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e5a; /* fmov.s @(r0,r5),fr3 */
L8c611e5a: r[8]+=60u;goto L8c611e5c; /* add #0x3c,r8 */
L8c611e5c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e5e; /* fmov.s fr3,@-r8 */
L8c611e5e: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c611e60; /* mov.l r3,@r15 */
L8c611e60: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c611e62; /* mov.w @(0x2,r3),r0 */
L8c611e62: r[3]=r[0];goto L8c611e64; /* mov r0,r3 */
L8c611e64: *fpul=r[3];goto L8c611e66; /* lds r3,FPUL */
L8c611e66: r[0]=24u;goto L8c611e68; /* mov #0x18,r0 */
L8c611e68: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611e6a; /* float FPUL,fr3 */
L8c611e6a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611e6c; /* fmul fr4,fr3 */
L8c611e6c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e6e; /* fmov.s fr3,@-r8 */
L8c611e6e: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611e70; /* mov.l @r15,r3 */
L8c611e70: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c611e72; /* mov.w @r3,r3 */
L8c611e72: *fpul=r[3];goto L8c611e74; /* lds r3,FPUL */
L8c611e74: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611e76; /* float FPUL,fr3 */
L8c611e76: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611e78; /* fmul fr4,fr3 */
L8c611e78: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e7a; /* fmov.s fr3,@-r8 */
L8c611e7a: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e7c; /* fmov.s @(r0,r5),fr3 */
L8c611e7c: r[0]=20u;goto L8c611e7e; /* mov #0x14,r0 */
L8c611e7e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e80; /* fmov.s fr3,@-r8 */
L8c611e80: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e82; /* fmov.s @(r0,r5),fr3 */
L8c611e82: r[0]=16u;goto L8c611e84; /* mov #0x10,r0 */
L8c611e84: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e86; /* fmov.s fr3,@-r8 */
L8c611e86: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e88; /* fmov.s @(r0,r5),fr3 */
L8c611e88: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e8a; /* fmov.s fr3,@-r8 */
L8c611e8a: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c611e8c; /* mov.l r4,@-r8 */
L8c611e8c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c611e8e; /* pref @r8 */
L8c611e8e: r[5]=r[12];goto L8c611e90; /* mov r12,r5 */
L8c611e90: r[0]=28u;goto L8c611e92; /* mov #0x1c,r0 */
L8c611e92: r[3]=r[6];goto L8c611e94; /* mov r6,r3 */
L8c611e94: r[5]+=96u;goto L8c611e96; /* add #0x60,r5 */
L8c611e96: r[3]+=12u;goto L8c611e98; /* add #0xc,r3 */
L8c611e98: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e9a; /* fmov.s @(r0,r5),fr3 */
L8c611e9a: r[8]+=60u;goto L8c611e9c; /* add #0x3c,r8 */
L8c611e9c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e9e; /* fmov.s fr3,@-r8 */
L8c611e9e: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c611ea0; /* mov.l r3,@r15 */
L8c611ea0: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c611ea2; /* mov.w @(0x2,r3),r0 */
L8c611ea2: r[3]=r[0];goto L8c611ea4; /* mov r0,r3 */
L8c611ea4: *fpul=r[3];goto L8c611ea6; /* lds r3,FPUL */
L8c611ea6: r[0]=24u;goto L8c611ea8; /* mov #0x18,r0 */
L8c611ea8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611eaa; /* float FPUL,fr3 */
L8c611eaa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611eac; /* fmul fr4,fr3 */
L8c611eac: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611eae; /* fmov.s fr3,@-r8 */
L8c611eae: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611eb0; /* mov.l @r15,r3 */
L8c611eb0: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c611eb2; /* mov.w @r3,r3 */
L8c611eb2: *fpul=r[3];goto L8c611eb4; /* lds r3,FPUL */
L8c611eb4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611eb6; /* float FPUL,fr3 */
L8c611eb6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611eb8; /* fmul fr4,fr3 */
L8c611eb8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611eba; /* fmov.s fr3,@-r8 */
L8c611eba: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611ebc; /* fmov.s @(r0,r5),fr3 */
L8c611ebc: r[0]=20u;goto L8c611ebe; /* mov #0x14,r0 */
L8c611ebe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611ec0; /* fmov.s fr3,@-r8 */
L8c611ec0: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611ec2; /* fmov.s @(r0,r5),fr3 */
L8c611ec2: r[0]=16u;goto L8c611ec4; /* mov #0x10,r0 */
L8c611ec4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611ec6; /* fmov.s fr3,@-r8 */
L8c611ec6: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611ec8; /* fmov.s @(r0,r5),fr3 */
L8c611ec8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611eca; /* fmov.s fr3,@-r8 */
L8c611eca: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c611ecc; /* mov.l r4,@-r8 */
L8c611ecc: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c611ece; /* pref @r8 */
L8c611ece: r[4]=r[12];goto L8c611ed0; /* mov r12,r4 */
L8c611ed0: r[0]=28u;goto L8c611ed2; /* mov #0x1c,r0 */
L8c611ed2: r[5]=r[6];goto L8c611ed4; /* mov r6,r5 */
L8c611ed4: r[4]+=64u;goto L8c611ed6; /* add #0x40,r4 */
L8c611ed6: r[5]+=8u;goto L8c611ed8; /* add #0x8,r5 */
L8c611ed8: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611eda; /* fmov.s @(r0,r4),fr3 */
L8c611eda: r[8]+=60u;goto L8c611edc; /* add #0x3c,r8 */
L8c611edc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611ede; /* fmov.s fr3,@-r8 */
L8c611ede: r[0]=sx(m,bus,&io_status,r[5]+0x2u,2);if(io_status)return io_status;goto L8c611ee0; /* mov.w @(0x2,r5),r0 */
L8c611ee0: r[3]=r[0];goto L8c611ee2; /* mov r0,r3 */
L8c611ee2: *fpul=r[3];goto L8c611ee4; /* lds r3,FPUL */
L8c611ee4: r[0]=24u;goto L8c611ee6; /* mov #0x18,r0 */
L8c611ee6: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611ee8; /* float FPUL,fr3 */
L8c611ee8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611eea; /* fmul fr4,fr3 */
L8c611eea: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611eec; /* fmov.s fr3,@-r8 */
L8c611eec: r[3]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;goto L8c611eee; /* mov.w @r5,r3 */
L8c611eee: *fpul=r[3];goto L8c611ef0; /* lds r3,FPUL */
L8c611ef0: r[3]=rd(m,bus,&io_status,0x8c612058u,4);if(io_status)return io_status;goto L8c611ef2; /* mov.l 0x8c612058,r3 */
L8c611ef2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611ef4; /* float FPUL,fr3 */
L8c611ef4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611ef6; /* fmul fr4,fr3 */
L8c611ef6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611ef8; /* fmov.s fr3,@-r8 */
L8c611ef8: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611efa; /* fmov.s @(r0,r4),fr3 */
L8c611efa: r[0]=20u;goto L8c611efc; /* mov #0x14,r0 */
L8c611efc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611efe; /* fmov.s fr3,@-r8 */
L8c611efe: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611f00; /* fmov.s @(r0,r4),fr3 */
L8c611f00: r[0]=16u;goto L8c611f02; /* mov #0x10,r0 */
L8c611f02: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611f04; /* fmov.s fr3,@-r8 */
L8c611f04: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611f06; /* fmov.s @(r0,r4),fr3 */
L8c611f06: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611f08; /* fmov.s fr3,@-r8 */
L8c611f08: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c611f0a; /* mov.l r3,@-r8 */
L8c611f0a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c611f0c; /* pref @r8 */
L8c611f0c: r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c611f0e; /* mov.l @r7,r2 */
L8c611f0e: r[8]+=32u;goto L8c611f10; /* add #0x20,r8 */
L8c611f10: r[2]+=4u;goto L8c611f12; /* add #0x4,r2 */
L8c611f12: wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c611f14; /* mov.l r2,@r7 */
L8c611f14: r[15]+=72u;goto L8c611f16; /* add #0x48,r15 */
L8c611f16: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c611f18; /* mov.l @r15+,r9 */
L8c611f18: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c611f1a; /* mov.l @r15+,r10 */
L8c611f1a: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c611f1c; /* mov.l @r15+,r11 */
L8c611f1c: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c611f1e; /* mov.l @r15+,r12 */
L8c611f1e: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c611f20; /* mov.l @r15+,r13 */
L8c611f20: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
