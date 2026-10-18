#include "sa_draw_motion_stop.h"
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
L8c611a14: sa_motion_publish_stop(ctx,0x8c611a14u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c611a16; /* mov.l r14,@-r15 */
L8c611a16: sa_motion_publish_stop(ctx,0x8c611a16u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c611a18; /* mov.l r13,@-r15 */
L8c611a18: sa_motion_publish_stop(ctx,0x8c611a18u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c611a1a; /* mov.l r12,@-r15 */
L8c611a1a: sa_motion_publish_stop(ctx,0x8c611a1au); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c611a1c; /* mov.l r11,@-r15 */
L8c611a1c: sa_motion_publish_stop(ctx,0x8c611a1cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c611a1e; /* mov.l r10,@-r15 */
L8c611a1e: sa_motion_publish_stop(ctx,0x8c611a1eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c611a20; /* mov.l r9,@-r15 */
L8c611a20: sa_motion_publish_stop(ctx,0x8c611a20u); r[15]+=4294967224u;goto L8c611a22; /* add #-0x48,r15 */
L8c611a22: sa_motion_publish_stop(ctx,0x8c611a22u); r[3]=rd(m,bus,&io_status,0x8c611ad4u,4);if(io_status)return io_status;goto L8c611a24; /* mov.l 0x8c611ad4,r3 */
L8c611a24: sa_motion_publish_stop(ctx,0x8c611a24u); r[14]=r[15];goto L8c611a26; /* mov r15,r14 */
L8c611a26: sa_motion_publish_stop(ctx,0x8c611a26u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c611a28; /* fldi1 fr6 */
L8c611a28: sa_motion_publish_stop(ctx,0x8c611a28u); r[9]=r[15];goto L8c611a2a; /* mov r15,r9 */
L8c611a2a: sa_motion_publish_stop(ctx,0x8c611a2au); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c611a2c; /* fmov.s @r3,fr3 */
L8c611a2c: sa_motion_publish_stop(ctx,0x8c611a2cu); r[14]+=60u;goto L8c611a2e; /* add #0x3c,r14 */
L8c611a2e: sa_motion_publish_stop(ctx,0x8c611a2eu); fcopy(f,5u,6u);goto L8c611a30; /* fmov fr6,fr5 */
L8c611a30: sa_motion_publish_stop(ctx,0x8c611a30u); r[9]+=24u;goto L8c611a32; /* add #0x18,r9 */
L8c611a32: sa_motion_publish_stop(ctx,0x8c611a32u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c611a34; /* fdiv fr3,fr5 */
L8c611a34: sa_motion_publish_stop(ctx,0x8c611a34u); r[2]=r[14];goto L8c611a36; /* mov r14,r2 */
L8c611a36: sa_motion_publish_stop(ctx,0x8c611a36u); r[11]=rd(m,bus,&io_status,0x8c611adcu,4);if(io_status)return io_status;goto L8c611a38; /* mov.l 0x8c611adc,r11 */
L8c611a38: sa_motion_publish_stop(ctx,0x8c611a38u); r[13]=r[9];goto L8c611a3a; /* mov r9,r13 */
L8c611a3a: sa_motion_publish_stop(ctx,0x8c611a3au); r[10]=rd(m,bus,&io_status,0x8c611ad8u,4);if(io_status)return io_status;goto L8c611a3c; /* mov.l 0x8c611ad8,r10 */
L8c611a3c: sa_motion_publish_stop(ctx,0x8c611a3cu); r[12]=0u;goto L8c611a3e; /* mov #0x0,r12 */
L8c611a3e: sa_motion_publish_stop(ctx,0x8c611a3eu); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c611a40; /* mov.l r2,@r15 */
L8c611a40: sa_motion_publish_stop(ctx,0x8c611a40u); wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c611a42; /* mov.l r4,@r2 */
L8c611a42: sa_motion_publish_stop(ctx,0x8c611a42u); r[4]=1u;goto L8c611a44; /* mov #0x1,r4 */
L8c611a44: sa_motion_publish_stop(ctx,0x8c611a44u); wr(m,bus,&io_status,r[14]+0x4u,r[5],4);if(io_status)return io_status;goto L8c611a46; /* mov.l r5,@(0x4,r14) */
L8c611a46: sa_motion_publish_stop(ctx,0x8c611a46u); r[5]=r[13];goto L8c611a48; /* mov r13,r5 */
L8c611a48: sa_motion_publish_stop(ctx,0x8c611a48u); wr(m,bus,&io_status,r[14]+0x8u,r[6],4);if(io_status)return io_status;goto L8c611a4a; /* mov.l r6,@(0x8,r14) */
L8c611a4a: sa_motion_publish_stop(ctx,0x8c611a4au); r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611a4c; /* mov.l @r15,r6 */
L8c611a4c: sa_motion_publish_stop(ctx,0x8c611a4cu); wr(m,bus,&io_status,r[15]+0x4u,r[13],4);if(io_status)return io_status;goto L8c611a4e; /* mov.l r13,@(0x4,r15) */
L8c611a4e: sa_motion_publish_stop(ctx,0x8c611a4eu); r[3]=r[12];goto L8c611a50; /* mov r12,r3 */
L8c611a50: sa_motion_publish_stop(ctx,0x8c611a50u); r[3]<<=2;goto L8c611a52; /* shll2 r3 */
L8c611a52: sa_motion_publish_stop(ctx,0x8c611a52u); r[3]+=r[14];goto L8c611a54; /* add r14,r3 */
L8c611a54: sa_motion_publish_stop(ctx,0x8c611a54u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c611a56; /* mov.l r3,@(0x8,r15) */
L8c611a56: sa_motion_publish_stop(ctx,0x8c611a56u); r[0]=4u;goto L8c611a58; /* mov #0x4,r0 */
L8c611a58: sa_motion_publish_stop(ctx,0x8c611a58u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c611a5a; /* mov.l @r3,r2 */
L8c611a5a: sa_motion_publish_stop(ctx,0x8c611a5au); r[3]=r[2];goto L8c611a5c; /* mov r2,r3 */
L8c611a5c: sa_motion_publish_stop(ctx,0x8c611a5cu); r[1]=r[2];goto L8c611a5e; /* mov r2,r1 */
L8c611a5e: sa_motion_publish_stop(ctx,0x8c611a5eu); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c611a60; /* mov.l r2,@r15 */
L8c611a60: sa_motion_publish_stop(ctx,0x8c611a60u); fload(m,bus,&io_status,f,2u,r[10]);if(io_status)return io_status;goto L8c611a62; /* fmov.s @r10,fr2 */
L8c611a62: sa_motion_publish_stop(ctx,0x8c611a62u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c611a64; /* fmov.s @r3,fr3 */
L8c611a64: sa_motion_publish_stop(ctx,0x8c611a64u); r[3]=rd(m,bus,&io_status,0x8c611ae0u,4);if(io_status)return io_status;goto L8c611a66; /* mov.l 0x8c611ae0,r3 */
L8c611a66: sa_motion_publish_stop(ctx,0x8c611a66u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611a68; /* fmul fr3,fr2 */
L8c611a68: sa_motion_publish_stop(ctx,0x8c611a68u); fload(m,bus,&io_status,f,1u,r[0]+r[10]);if(io_status)return io_status;goto L8c611a6a; /* fmov.s @(r0,r10),fr1 */
L8c611a6a: sa_motion_publish_stop(ctx,0x8c611a6au); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c611a6c; /* fmov.s @r3,fr3 */
L8c611a6c: sa_motion_publish_stop(ctx,0x8c611a6cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611a6e; /* fsub fr2,fr3 */
L8c611a6e: sa_motion_publish_stop(ctx,0x8c611a6eu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c611a70; /* fmov.s @(r0,r1),fr2 */
L8c611a70: sa_motion_publish_stop(ctx,0x8c611a70u); r[0]=8u;goto L8c611a72; /* mov #0x8,r0 */
L8c611a72: sa_motion_publish_stop(ctx,0x8c611a72u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c611a74; /* fmul fr2,fr1 */
L8c611a74: sa_motion_publish_stop(ctx,0x8c611a74u); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c611a76; /* fmov.s @(r0,r1),fr2 */
L8c611a76: sa_motion_publish_stop(ctx,0x8c611a76u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611a78; /* fsub fr1,fr3 */
L8c611a78: sa_motion_publish_stop(ctx,0x8c611a78u); fload(m,bus,&io_status,f,1u,r[0]+r[10]);if(io_status)return io_status;goto L8c611a7a; /* fmov.s @(r0,r10),fr1 */
L8c611a7a: sa_motion_publish_stop(ctx,0x8c611a7au); r[0]=28u;goto L8c611a7c; /* mov #0x1c,r0 */
L8c611a7c: sa_motion_publish_stop(ctx,0x8c611a7cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c611a7e; /* fmul fr2,fr1 */
L8c611a7e: sa_motion_publish_stop(ctx,0x8c611a7eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611a80; /* fsub fr1,fr3 */
L8c611a80: sa_motion_publish_stop(ctx,0x8c611a80u); fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611a82; /* fmov fr3,@(r0,r2) */
L8c611a82: sa_motion_publish_stop(ctx,0x8c611a82u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c611a84; /* mov.l @(0x8,r15),r2 */
L8c611a84: sa_motion_publish_stop(ctx,0x8c611a84u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c611a86; /* mov.l @r2,r1 */
L8c611a86: sa_motion_publish_stop(ctx,0x8c611a86u); r[0]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c611a88; /* mov.l @(0xc,r1),r0 */
L8c611a88: sa_motion_publish_stop(ctx,0x8c611a88u); *sr=(*sr&~1u)|!(r[4]&r[0]);goto L8c611a8a; /* tst r4,r0 */
L8c611a8a: sa_motion_publish_stop(ctx,0x8c611a8au); if(!(*sr&1u))goto L8c611aec;goto L8c611a8c; /* bf 0x8c611aec */
L8c611a8c: sa_motion_publish_stop(ctx,0x8c611a8cu); r[1]=r[12];goto L8c611a8e; /* mov r12,r1 */
L8c611a8e: sa_motion_publish_stop(ctx,0x8c611a8eu); r[1]<<=2;goto L8c611a90; /* shll2 r1 */
L8c611a90: sa_motion_publish_stop(ctx,0x8c611a90u); r[1]+=r[14];goto L8c611a92; /* add r14,r1 */
L8c611a92: sa_motion_publish_stop(ctx,0x8c611a92u); fcopy(f,4u,6u);goto L8c611a94; /* fmov fr6,fr4 */
L8c611a94: sa_motion_publish_stop(ctx,0x8c611a94u); r[0]=24u;goto L8c611a96; /* mov #0x18,r0 */
L8c611a96: sa_motion_publish_stop(ctx,0x8c611a96u); wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c611a98; /* mov.l r1,@r15 */
L8c611a98: sa_motion_publish_stop(ctx,0x8c611a98u); r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c611a9a; /* mov.l @r1,r2 */
L8c611a9a: sa_motion_publish_stop(ctx,0x8c611a9au); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c611a9c; /* mov.l r2,@(0x8,r15) */
L8c611a9c: sa_motion_publish_stop(ctx,0x8c611a9cu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611a9e; /* fmov.s @(r0,r2),fr3 */
L8c611a9e: sa_motion_publish_stop(ctx,0x8c611a9eu); r[0]=12u;goto L8c611aa0; /* mov #0xc,r0 */
L8c611aa0: sa_motion_publish_stop(ctx,0x8c611aa0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c611aa2; /* fdiv fr3,fr4 */
L8c611aa2: sa_motion_publish_stop(ctx,0x8c611aa2u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c611aa4; /* fmov.s @(r0,r11),fr3 */
L8c611aa4: sa_motion_publish_stop(ctx,0x8c611aa4u); r[0]=16u;goto L8c611aa6; /* mov #0x10,r0 */
L8c611aa6: sa_motion_publish_stop(ctx,0x8c611aa6u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c611aa8; /* fmov.s @(r0,r2),fr2 */
L8c611aa8: sa_motion_publish_stop(ctx,0x8c611aa8u); r[0]=16u;goto L8c611aaa; /* mov #0x10,r0 */
L8c611aaa: sa_motion_publish_stop(ctx,0x8c611aaau); r[2]=rd(m,bus,&io_status,0x8c611ae4u,4);if(io_status)return io_status;goto L8c611aac; /* mov.l 0x8c611ae4,r2 */
L8c611aac: sa_motion_publish_stop(ctx,0x8c611aacu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c611aae; /* fsub fr3,fr2 */
L8c611aae: sa_motion_publish_stop(ctx,0x8c611aaeu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c611ab0; /* fmov.s @r2,fr3 */
L8c611ab0: sa_motion_publish_stop(ctx,0x8c611ab0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611ab2; /* fmul fr4,fr2 */
L8c611ab2: sa_motion_publish_stop(ctx,0x8c611ab2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611ab4; /* fdiv fr3,fr2 */
L8c611ab4: sa_motion_publish_stop(ctx,0x8c611ab4u); fstore(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c611ab6; /* fmov.s fr2,@r5 */
L8c611ab6: sa_motion_publish_stop(ctx,0x8c611ab6u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611ab8; /* mov.l @r15,r1 */
L8c611ab8: sa_motion_publish_stop(ctx,0x8c611ab8u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c611aba; /* fmov.s @(r0,r11),fr3 */
L8c611aba: sa_motion_publish_stop(ctx,0x8c611abau); r[0]=20u;goto L8c611abc; /* mov #0x14,r0 */
L8c611abc: sa_motion_publish_stop(ctx,0x8c611abcu); r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c611abe; /* mov.l @r1,r1 */
L8c611abe: sa_motion_publish_stop(ctx,0x8c611abeu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c611ac0; /* fmov.s @(r0,r1),fr2 */
L8c611ac0: sa_motion_publish_stop(ctx,0x8c611ac0u); r[0]=4u;goto L8c611ac2; /* mov #0x4,r0 */
L8c611ac2: sa_motion_publish_stop(ctx,0x8c611ac2u); r[1]=rd(m,bus,&io_status,0x8c611ae8u,4);if(io_status)return io_status;goto L8c611ac4; /* mov.l 0x8c611ae8,r1 */
L8c611ac4: sa_motion_publish_stop(ctx,0x8c611ac4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c611ac6; /* fsub fr3,fr2 */
L8c611ac6: sa_motion_publish_stop(ctx,0x8c611ac6u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c611ac8; /* fmov.s @r1,fr3 */
L8c611ac8: sa_motion_publish_stop(ctx,0x8c611ac8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611aca; /* fmul fr4,fr2 */
L8c611aca: sa_motion_publish_stop(ctx,0x8c611acau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611acc; /* fdiv fr3,fr2 */
L8c611acc: sa_motion_publish_stop(ctx,0x8c611accu); fstore(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c611ace; /* fmov fr2,@(r0,r5) */
L8c611ace: sa_motion_publish_stop(ctx,0x8c611aceu); r[0]=8u;goto L8c611ad0; /* mov #0x8,r0 */
L8c611ad0: sa_motion_publish_stop(ctx,0x8c611ad2u); fstore(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c611b08; /* bra 0x8c611b08 */
L8c611aec: sa_motion_publish_stop(ctx,0x8c611aecu); r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c611aee; /* mov.l @r6,r2 */
L8c611aee: sa_motion_publish_stop(ctx,0x8c611aeeu); r[0]=16u;goto L8c611af0; /* mov #0x10,r0 */
L8c611af0: sa_motion_publish_stop(ctx,0x8c611af0u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611af2; /* fmov.s @(r0,r2),fr3 */
L8c611af2: sa_motion_publish_stop(ctx,0x8c611af2u); r[0]=20u;goto L8c611af4; /* mov #0x14,r0 */
L8c611af4: sa_motion_publish_stop(ctx,0x8c611af4u); fstore(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c611af6; /* fmov.s fr3,@r13 */
L8c611af6: sa_motion_publish_stop(ctx,0x8c611af6u); r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c611af8; /* mov.l @r6,r2 */
L8c611af8: sa_motion_publish_stop(ctx,0x8c611af8u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611afa; /* fmov.s @(r0,r2),fr3 */
L8c611afa: sa_motion_publish_stop(ctx,0x8c611afau); r[0]=4u;goto L8c611afc; /* mov #0x4,r0 */
L8c611afc: sa_motion_publish_stop(ctx,0x8c611afcu); fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c611afe; /* fmov fr3,@(r0,r13) */
L8c611afe: sa_motion_publish_stop(ctx,0x8c611afeu); r[0]=24u;goto L8c611b00; /* mov #0x18,r0 */
L8c611b00: sa_motion_publish_stop(ctx,0x8c611b00u); r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c611b02; /* mov.l @r6,r2 */
L8c611b02: sa_motion_publish_stop(ctx,0x8c611b02u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611b04; /* fmov.s @(r0,r2),fr3 */
L8c611b04: sa_motion_publish_stop(ctx,0x8c611b04u); r[0]=8u;goto L8c611b06; /* mov #0x8,r0 */
L8c611b06: sa_motion_publish_stop(ctx,0x8c611b06u); fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c611b08; /* fmov fr3,@(r0,r13) */
L8c611b08: sa_motion_publish_stop(ctx,0x8c611b08u); r[3]=3u;goto L8c611b0a; /* mov #0x3,r3 */
L8c611b0a: sa_motion_publish_stop(ctx,0x8c611b0au); r[12]+=1u;goto L8c611b0c; /* add #0x1,r12 */
L8c611b0c: sa_motion_publish_stop(ctx,0x8c611b0cu); *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[3]);goto L8c611b0e; /* cmp/ge r3,r12 */
L8c611b0e: sa_motion_publish_stop(ctx,0x8c611b0eu); r[6]+=4u;goto L8c611b10; /* add #0x4,r6 */
L8c611b10: sa_motion_publish_stop(ctx,0x8c611b10u); r[13]+=12u;goto L8c611b12; /* add #0xc,r13 */
L8c611b12: sa_motion_publish_stop(ctx,0x8c611b14u); take=!(*sr&1u);r[5]+=12u;if(take)goto L8c611a4e;goto L8c611b16; /* bf/s 0x8c611a4e */
L8c611b16: sa_motion_publish_stop(ctx,0x8c611b16u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611b18; /* mov.l @(0x4,r15),r3 */
L8c611b18: sa_motion_publish_stop(ctx,0x8c611b18u); r[0]=8u;goto L8c611b1a; /* mov #0x8,r0 */
L8c611b1a: sa_motion_publish_stop(ctx,0x8c611b1au); r[6]=r[9];goto L8c611b1c; /* mov r9,r6 */
L8c611b1c: sa_motion_publish_stop(ctx,0x8c611b1cu); r[6]+=12u;goto L8c611b1e; /* add #0xc,r6 */
L8c611b1e: sa_motion_publish_stop(ctx,0x8c611b1eu); fload(m,bus,&io_status,f,7u,r[0]+r[3]);if(io_status)return io_status;goto L8c611b20; /* fmov.s @(r0,r3),fr7 */
L8c611b20: sa_motion_publish_stop(ctx,0x8c611b20u); r[0]=4u;goto L8c611b22; /* mov #0x4,r0 */
L8c611b22: sa_motion_publish_stop(ctx,0x8c611b22u); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c611b24; /* fmov.s @(r0,r6),fr4 */
L8c611b24: sa_motion_publish_stop(ctx,0x8c611b24u); r[5]=r[9];goto L8c611b26; /* mov r9,r5 */
L8c611b26: sa_motion_publish_stop(ctx,0x8c611b26u); fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c611b28; /* fmov.s @(r0,r3),fr9 */
L8c611b28: sa_motion_publish_stop(ctx,0x8c611b28u); r[0]=8u;goto L8c611b2a; /* mov #0x8,r0 */
L8c611b2a: sa_motion_publish_stop(ctx,0x8c611b2au); fload(m,bus,&io_status,f,6u,r[0]+r[6]);if(io_status)return io_status;goto L8c611b2c; /* fmov.s @(r0,r6),fr6 */
L8c611b2c: sa_motion_publish_stop(ctx,0x8c611b2cu); r[0]=4u;goto L8c611b2e; /* mov #0x4,r0 */
L8c611b2e: sa_motion_publish_stop(ctx,0x8c611b2eu); fcopy(f,3u,9u);goto L8c611b30; /* fmov fr9,fr3 */
L8c611b30: sa_motion_publish_stop(ctx,0x8c611b30u); r[5]+=24u;goto L8c611b32; /* add #0x18,r5 */
L8c611b32: sa_motion_publish_stop(ctx,0x8c611b32u); fcopy(f,0u,7u);goto L8c611b34; /* fmov fr7,fr0 */
L8c611b34: sa_motion_publish_stop(ctx,0x8c611b34u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611b36; /* fmul fr6,fr3 */
L8c611b36: sa_motion_publish_stop(ctx,0x8c611b36u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c611b38; /* fmul fr4,fr0 */
L8c611b38: sa_motion_publish_stop(ctx,0x8c611b38u); fload(m,bus,&io_status,f,10u,r[3]);if(io_status)return io_status;goto L8c611b3a; /* fmov.s @r3,fr10 */
L8c611b3a: sa_motion_publish_stop(ctx,0x8c611b3au); fload(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;goto L8c611b3c; /* fmov.s @r6,fr8 */
L8c611b3c: sa_motion_publish_stop(ctx,0x8c611b3cu); fcopy(f,2u,7u);goto L8c611b3e; /* fmov fr7,fr2 */
L8c611b3e: sa_motion_publish_stop(ctx,0x8c611b3eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611b40; /* fmul fr8,fr2 */
L8c611b40: sa_motion_publish_stop(ctx,0x8c611b40u); fcopy(f,1u,8u);goto L8c611b42; /* fmov fr8,fr1 */
L8c611b42: sa_motion_publish_stop(ctx,0x8c611b42u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c611b44; /* fsub fr3,fr0 */
L8c611b44: sa_motion_publish_stop(ctx,0x8c611b44u); fcopy(f,3u,10u);goto L8c611b46; /* fmov fr10,fr3 */
L8c611b46: sa_motion_publish_stop(ctx,0x8c611b46u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611b48; /* fmul fr6,fr3 */
L8c611b48: sa_motion_publish_stop(ctx,0x8c611b48u); fcopy(f,11u,10u);goto L8c611b4a; /* fmov fr10,fr11 */
L8c611b4a: sa_motion_publish_stop(ctx,0x8c611b4au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[9],&f->fpscr,&fr[1]);if(status)return status;}goto L8c611b4c; /* fmul fr9,fr1 */
L8c611b4c: sa_motion_publish_stop(ctx,0x8c611b4cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611b4e; /* fsub fr2,fr3 */
L8c611b4e: sa_motion_publish_stop(ctx,0x8c611b4eu); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c611b50; /* fmov.s @(r0,r5),fr2 */
L8c611b50: sa_motion_publish_stop(ctx,0x8c611b50u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611b52; /* fmul fr3,fr2 */
L8c611b52: sa_motion_publish_stop(ctx,0x8c611b52u); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c611b54; /* fmov.s @r5,fr3 */
L8c611b54: sa_motion_publish_stop(ctx,0x8c611b54u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611b56; /* fmac fr0,fr3,fr2 */
L8c611b56: sa_motion_publish_stop(ctx,0x8c611b56u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[4],&f->fpscr,&fr[11]);if(status)return status;}goto L8c611b58; /* fmul fr4,fr11 */
L8c611b58: sa_motion_publish_stop(ctx,0x8c611b58u); r[0]=8u;goto L8c611b5a; /* mov #0x8,r0 */
L8c611b5a: sa_motion_publish_stop(ctx,0x8c611b5au); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611b5c; /* fmov.s @(r0,r5),fr3 */
L8c611b5c: sa_motion_publish_stop(ctx,0x8c611b5cu); r[5]=rd(m,bus,&io_status,0x8c611decu,4);if(io_status)return io_status;goto L8c611b5e; /* mov.l 0x8c611dec,r5 */
L8c611b5e: sa_motion_publish_stop(ctx,0x8c611b5eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[11],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c611b60; /* fsub fr11,fr1 */
L8c611b60: sa_motion_publish_stop(ctx,0x8c611b60u); r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c611b62; /* mov.l @r5,r3 */
L8c611b62: sa_motion_publish_stop(ctx,0x8c611b62u); fcopy(f,0u,1u);goto L8c611b64; /* fmov fr1,fr0 */
L8c611b64: sa_motion_publish_stop(ctx,0x8c611b64u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611b66; /* fmac fr0,fr3,fr2 */
L8c611b66: sa_motion_publish_stop(ctx,0x8c611b66u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c611b68; /* fldi0 fr3 */
L8c611b68: sa_motion_publish_stop(ctx,0x8c611b68u); fcopy(f,4u,2u);goto L8c611b6a; /* fmov fr2,fr4 */
L8c611b6a: sa_motion_publish_stop(ctx,0x8c611b6au); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c611b6c; /* fcmp/gt fr4,fr3 */
L8c611b6c: sa_motion_publish_stop(ctx,0x8c611b6cu); r[0]=*sr&1u;goto L8c611b6e; /* movt r0 */
L8c611b6e: sa_motion_publish_stop(ctx,0x8c611b6eu); *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c611b70; /* tst r3,r0 */
L8c611b70: sa_motion_publish_stop(ctx,0x8c611b70u); if((*sr&1u))goto L8c611b76;goto L8c611b72; /* bt 0x8c611b76 */
L8c611b72: sa_motion_publish_stop(ctx,0x8c611b74u); goto L8c611f14; /* bra 0x8c611f14 */
L8c611b76: sa_motion_publish_stop(ctx,0x8c611b76u); r[1]=0u;goto L8c611b78; /* mov #0x0,r1 */
L8c611b78: sa_motion_publish_stop(ctx,0x8c611b78u); r[12]=rd(m,bus,&io_status,0x8c611df0u,4);if(io_status)return io_status;goto L8c611b7a; /* mov.l 0x8c611df0,r12 */
L8c611b7a: sa_motion_publish_stop(ctx,0x8c611b7au); wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c611b7c; /* mov.l r1,@r5 */
L8c611b7c: sa_motion_publish_stop(ctx,0x8c611b7cu); r[3]=2u;goto L8c611b7e; /* mov #0x2,r3 */
L8c611b7e: sa_motion_publish_stop(ctx,0x8c611b7eu); r[6]=rd(m,bus,&io_status,0x8c611df4u,4);if(io_status)return io_status;goto L8c611b80; /* mov.l 0x8c611df4,r6 */
L8c611b80: sa_motion_publish_stop(ctx,0x8c611b80u); r[5]=r[1];goto L8c611b82; /* mov r1,r5 */
L8c611b82: sa_motion_publish_stop(ctx,0x8c611b82u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c611b84; /* mov.l r3,@(0x4,r15) */
L8c611b84: sa_motion_publish_stop(ctx,0x8c611b84u); r[10]=r[5];goto L8c611b86; /* mov r5,r10 */
L8c611b86: sa_motion_publish_stop(ctx,0x8c611b86u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611b88; /* mov.l @(0x4,r15),r3 */
L8c611b88: sa_motion_publish_stop(ctx,0x8c611b88u); r[0]=r[14];goto L8c611b8a; /* mov r14,r0 */
L8c611b8a: sa_motion_publish_stop(ctx,0x8c611b8au); r[13]=r[10];goto L8c611b8c; /* mov r10,r13 */
L8c611b8c: sa_motion_publish_stop(ctx,0x8c611b8cu); r[13]<<=2;goto L8c611b8e; /* shll2 r13 */
L8c611b8e: sa_motion_publish_stop(ctx,0x8c611b8eu); r[3]<<=2;goto L8c611b90; /* shll2 r3 */
L8c611b90: sa_motion_publish_stop(ctx,0x8c611b90u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c611b92; /* mov.l r3,@r15 */
L8c611b92: sa_motion_publish_stop(ctx,0x8c611b92u); r[2]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c611b94; /* mov.l @(r0,r3),r2 */
L8c611b94: sa_motion_publish_stop(ctx,0x8c611b94u); r[3]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c611b96; /* mov.l @(0xc,r2),r3 */
L8c611b96: sa_motion_publish_stop(ctx,0x8c611b96u); r[2]=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;goto L8c611b98; /* mov.l @(r0,r13),r2 */
L8c611b98: sa_motion_publish_stop(ctx,0x8c611b98u); r[3]&=r[4];goto L8c611b9a; /* and r4,r3 */
L8c611b9a: sa_motion_publish_stop(ctx,0x8c611b9au); if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[2]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c611b9c; /* mov.l @(0xc,r2),r2 */
L8c611b9c: sa_motion_publish_stop(ctx,0x8c611b9cu); r[2]&=r[4];goto L8c611b9e; /* and r4,r2 */
L8c611b9e: sa_motion_publish_stop(ctx,0x8c611b9eu); r[3]^=r[2];goto L8c611ba0; /* xor r2,r3 */
L8c611ba0: sa_motion_publish_stop(ctx,0x8c611ba0u); r[0]=r[3];goto L8c611ba2; /* mov r3,r0 */
L8c611ba2: sa_motion_publish_stop(ctx,0x8c611ba2u); *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c611ba4; /* cmp/eq #0x1,r0 */
L8c611ba4: sa_motion_publish_stop(ctx,0x8c611ba4u); if((*sr&1u))goto L8c611baa;goto L8c611ba6; /* bt 0x8c611baa */
L8c611ba6: sa_motion_publish_stop(ctx,0x8c611ba8u); goto L8c611cac; /* bra 0x8c611cac */
L8c611baa: sa_motion_publish_stop(ctx,0x8c611baau); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611bac; /* mov.l @(0x4,r15),r2 */
L8c611bac: sa_motion_publish_stop(ctx,0x8c611bacu); r[0]=8u;goto L8c611bae; /* mov #0x8,r0 */
L8c611bae: sa_motion_publish_stop(ctx,0x8c611baeu); r[1]=r[10];goto L8c611bb0; /* mov r10,r1 */
L8c611bb0: sa_motion_publish_stop(ctx,0x8c611bb0u); r[3]=r[2];goto L8c611bb2; /* mov r2,r3 */
L8c611bb2: sa_motion_publish_stop(ctx,0x8c611bb2u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c611bb4; /* shll r2 */
L8c611bb4: sa_motion_publish_stop(ctx,0x8c611bb4u); r[2]+=r[3];goto L8c611bb6; /* add r3,r2 */
L8c611bb6: sa_motion_publish_stop(ctx,0x8c611bb6u); r[3]=rd(m,bus,&io_status,0x8c611df8u,4);if(io_status)return io_status;goto L8c611bb8; /* mov.l 0x8c611df8,r3 */
L8c611bb8: sa_motion_publish_stop(ctx,0x8c611bb8u); r[2]<<=2;goto L8c611bba; /* shll2 r2 */
L8c611bba: sa_motion_publish_stop(ctx,0x8c611bbau); r[2]+=r[9];goto L8c611bbc; /* add r9,r2 */
L8c611bbc: sa_motion_publish_stop(ctx,0x8c611bbcu); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c611bbe; /* mov.l r2,@(0xc,r15) */
L8c611bbe: sa_motion_publish_stop(ctx,0x8c611bbeu); fload(m,bus,&io_status,f,4u,r[0]+r[2]);if(io_status)return io_status;goto L8c611bc0; /* fmov.s @(r0,r2),fr4 */
L8c611bc0: sa_motion_publish_stop(ctx,0x8c611bc0u); r[2]=r[10];goto L8c611bc2; /* mov r10,r2 */
L8c611bc2: sa_motion_publish_stop(ctx,0x8c611bc2u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c611bc4; /* shll r2 */
L8c611bc4: sa_motion_publish_stop(ctx,0x8c611bc4u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c611bc6; /* fmov.s @r3,fr3 */
L8c611bc6: sa_motion_publish_stop(ctx,0x8c611bc6u); r[2]+=r[1];goto L8c611bc8; /* add r1,r2 */
L8c611bc8: sa_motion_publish_stop(ctx,0x8c611bc8u); r[2]<<=2;goto L8c611bca; /* shll2 r2 */
L8c611bca: sa_motion_publish_stop(ctx,0x8c611bcau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611bcc; /* fsub fr4,fr3 */
L8c611bcc: sa_motion_publish_stop(ctx,0x8c611bccu); r[0]=8u;goto L8c611bce; /* mov #0x8,r0 */
L8c611bce: sa_motion_publish_stop(ctx,0x8c611bceu); r[2]+=r[9];goto L8c611bd0; /* add r9,r2 */
L8c611bd0: sa_motion_publish_stop(ctx,0x8c611bd0u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c611bd2; /* mov.l r2,@(0x8,r15) */
L8c611bd2: sa_motion_publish_stop(ctx,0x8c611bd2u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c611bd4; /* fmov.s @(r0,r2),fr2 */
L8c611bd4: sa_motion_publish_stop(ctx,0x8c611bd4u); r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c611bd6; /* mov.l @(0x8,r15),r1 */
L8c611bd6: sa_motion_publish_stop(ctx,0x8c611bd6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c611bd8; /* fsub fr4,fr2 */
L8c611bd8: sa_motion_publish_stop(ctx,0x8c611bd8u); fcopy(f,4u,3u);goto L8c611bda; /* fmov fr3,fr4 */
L8c611bda: sa_motion_publish_stop(ctx,0x8c611bdau); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c611bdc; /* mov.l @(0xc,r15),r2 */
L8c611bdc: sa_motion_publish_stop(ctx,0x8c611bdcu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c611bde; /* fmov.s @r1,fr3 */
L8c611bde: sa_motion_publish_stop(ctx,0x8c611bdeu); fload(m,bus,&io_status,f,6u,r[2]);if(io_status)return io_status;goto L8c611be0; /* fmov.s @r2,fr6 */
L8c611be0: sa_motion_publish_stop(ctx,0x8c611be0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c611be2; /* fdiv fr2,fr4 */
L8c611be2: sa_motion_publish_stop(ctx,0x8c611be2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611be4; /* fsub fr6,fr3 */
L8c611be4: sa_motion_publish_stop(ctx,0x8c611be4u); fcopy(f,2u,6u);goto L8c611be6; /* fmov fr6,fr2 */
L8c611be6: sa_motion_publish_stop(ctx,0x8c611be6u); fcopy(f,0u,4u);goto L8c611be8; /* fmov fr4,fr0 */
L8c611be8: sa_motion_publish_stop(ctx,0x8c611be8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611bea; /* fmac fr0,fr3,fr2 */
L8c611bea: sa_motion_publish_stop(ctx,0x8c611beau); r[1]=rd(m,bus,&io_status,0x8c611dfcu,4);if(io_status)return io_status;goto L8c611bec; /* mov.l 0x8c611dfc,r1 */
L8c611bec: sa_motion_publish_stop(ctx,0x8c611becu); r[0]=12u;goto L8c611bee; /* mov #0xc,r0 */
L8c611bee: sa_motion_publish_stop(ctx,0x8c611beeu); fcopy(f,6u,2u);goto L8c611bf0; /* fmov fr2,fr6 */
L8c611bf0: sa_motion_publish_stop(ctx,0x8c611bf0u); r[2]=r[5];goto L8c611bf2; /* mov r5,r2 */
L8c611bf2: sa_motion_publish_stop(ctx,0x8c611bf2u); fcopy(f,0u,5u);goto L8c611bf4; /* fmov fr5,fr0 */
L8c611bf4: sa_motion_publish_stop(ctx,0x8c611bf4u); r[2]<<=2;goto L8c611bf6; /* shll2 r2 */
L8c611bf6: sa_motion_publish_stop(ctx,0x8c611bf6u); r[2]<<=2;goto L8c611bf8; /* shll2 r2 */
L8c611bf8: sa_motion_publish_stop(ctx,0x8c611bf8u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c611bfa; /* shll r2 */
L8c611bfa: sa_motion_publish_stop(ctx,0x8c611bfau); r[2]+=r[12];goto L8c611bfc; /* add r12,r2 */
L8c611bfc: sa_motion_publish_stop(ctx,0x8c611bfcu); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c611bfe; /* mov.l r2,@(0x4,r15) */
L8c611bfe: sa_motion_publish_stop(ctx,0x8c611bfeu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c611c00; /* fmov.s @r1,fr3 */
L8c611c00: sa_motion_publish_stop(ctx,0x8c611c00u); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c611c02; /* fmov.s @(r0,r11),fr2 */
L8c611c02: sa_motion_publish_stop(ctx,0x8c611c02u); r[0]=16u;goto L8c611c04; /* mov #0x10,r0 */
L8c611c04: sa_motion_publish_stop(ctx,0x8c611c04u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611c06; /* fmul fr6,fr3 */
L8c611c06: sa_motion_publish_stop(ctx,0x8c611c06u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611c08; /* fmac fr0,fr3,fr2 */
L8c611c08: sa_motion_publish_stop(ctx,0x8c611c08u); fcopy(f,0u,4u);goto L8c611c0a; /* fmov fr4,fr0 */
L8c611c0a: sa_motion_publish_stop(ctx,0x8c611c0au); fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c611c0c; /* fmov fr2,@(r0,r2) */
L8c611c0c: sa_motion_publish_stop(ctx,0x8c611c0cu); r[0]=4u;goto L8c611c0e; /* mov #0x4,r0 */
L8c611c0e: sa_motion_publish_stop(ctx,0x8c611c0eu); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c611c10; /* mov.l @(0xc,r15),r2 */
L8c611c10: sa_motion_publish_stop(ctx,0x8c611c10u); fload(m,bus,&io_status,f,6u,r[0]+r[2]);if(io_status)return io_status;goto L8c611c12; /* fmov.s @(r0,r2),fr6 */
L8c611c12: sa_motion_publish_stop(ctx,0x8c611c12u); r[0]=4u;goto L8c611c14; /* mov #0x4,r0 */
L8c611c14: sa_motion_publish_stop(ctx,0x8c611c14u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c611c16; /* mov.l @(0x8,r15),r2 */
L8c611c16: sa_motion_publish_stop(ctx,0x8c611c16u); fcopy(f,2u,6u);goto L8c611c18; /* fmov fr6,fr2 */
L8c611c18: sa_motion_publish_stop(ctx,0x8c611c18u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611c1a; /* fmov.s @(r0,r2),fr3 */
L8c611c1a: sa_motion_publish_stop(ctx,0x8c611c1au); r[0]=rd(m,bus,&io_status,0x8c611e00u,4);if(io_status)return io_status;goto L8c611c1c; /* mov.l 0x8c611e00,r0 */
L8c611c1c: sa_motion_publish_stop(ctx,0x8c611c1cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611c1e; /* fsub fr6,fr3 */
L8c611c1e: sa_motion_publish_stop(ctx,0x8c611c1eu); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611c20; /* mov.l @(0x4,r15),r2 */
L8c611c20: sa_motion_publish_stop(ctx,0x8c611c20u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611c22; /* fmac fr0,fr3,fr2 */
L8c611c22: sa_motion_publish_stop(ctx,0x8c611c22u); fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c611c24; /* fmov.s @r0,fr3 */
L8c611c24: sa_motion_publish_stop(ctx,0x8c611c24u); r[0]=16u;goto L8c611c26; /* mov #0x10,r0 */
L8c611c26: sa_motion_publish_stop(ctx,0x8c611c26u); fcopy(f,6u,2u);goto L8c611c28; /* fmov fr2,fr6 */
L8c611c28: sa_motion_publish_stop(ctx,0x8c611c28u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611c2a; /* fmul fr6,fr3 */
L8c611c2a: sa_motion_publish_stop(ctx,0x8c611c2au); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c611c2c; /* fmov.s @(r0,r11),fr2 */
L8c611c2c: sa_motion_publish_stop(ctx,0x8c611c2cu); r[0]=20u;goto L8c611c2e; /* mov #0x14,r0 */
L8c611c2e: sa_motion_publish_stop(ctx,0x8c611c2eu); fcopy(f,0u,5u);goto L8c611c30; /* fmov fr5,fr0 */
L8c611c30: sa_motion_publish_stop(ctx,0x8c611c30u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611c32; /* fmac fr0,fr3,fr2 */
L8c611c32: sa_motion_publish_stop(ctx,0x8c611c32u); fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c611c34; /* fmov fr2,@(r0,r2) */
L8c611c34: sa_motion_publish_stop(ctx,0x8c611c34u); r[0]=24u;goto L8c611c36; /* mov #0x18,r0 */
L8c611c36: sa_motion_publish_stop(ctx,0x8c611c36u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611c38; /* mov.l @(0x4,r15),r3 */
L8c611c38: sa_motion_publish_stop(ctx,0x8c611c38u); fstore(m,bus,&io_status,f,0u,r[0]+r[3]);if(io_status)return io_status;goto L8c611c3a; /* fmov fr0,@(r0,r3) */
L8c611c3a: sa_motion_publish_stop(ctx,0x8c611c3au); r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611c3c; /* mov.l @r15,r0 */
L8c611c3c: sa_motion_publish_stop(ctx,0x8c611c3cu); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611c3e; /* mov.l @(0x4,r15),r3 */
L8c611c3e: sa_motion_publish_stop(ctx,0x8c611c3eu); r[2]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c611c40; /* mov.l @(r0,r14),r2 */
L8c611c40: sa_motion_publish_stop(ctx,0x8c611c40u); r[0]=28u;goto L8c611c42; /* mov #0x1c,r0 */
L8c611c42: sa_motion_publish_stop(ctx,0x8c611c42u); fcopy(f,0u,4u);goto L8c611c44; /* fmov fr4,fr0 */
L8c611c44: sa_motion_publish_stop(ctx,0x8c611c44u); wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c611c46; /* mov.l r2,@(0x10,r15) */
L8c611c46: sa_motion_publish_stop(ctx,0x8c611c46u); fload(m,bus,&io_status,f,6u,r[0]+r[2]);if(io_status)return io_status;goto L8c611c48; /* fmov.s @(r0,r2),fr6 */
L8c611c48: sa_motion_publish_stop(ctx,0x8c611c48u); r[0]=r[14];goto L8c611c4a; /* mov r14,r0 */
L8c611c4a: sa_motion_publish_stop(ctx,0x8c611c4au); r[2]=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;goto L8c611c4c; /* mov.l @(r0,r13),r2 */
L8c611c4c: sa_motion_publish_stop(ctx,0x8c611c4cu); r[0]=28u;goto L8c611c4e; /* mov #0x1c,r0 */
L8c611c4e: sa_motion_publish_stop(ctx,0x8c611c4eu); fcopy(f,2u,6u);goto L8c611c50; /* fmov fr6,fr2 */
L8c611c50: sa_motion_publish_stop(ctx,0x8c611c50u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611c52; /* fmov.s @(r0,r2),fr3 */
L8c611c52: sa_motion_publish_stop(ctx,0x8c611c52u); r[2]=r[0];goto L8c611c54; /* mov r0,r2 */
L8c611c54: sa_motion_publish_stop(ctx,0x8c611c54u); r[2]+=r[3];goto L8c611c56; /* add r3,r2 */
L8c611c56: sa_motion_publish_stop(ctx,0x8c611c56u); r[3]=r[5];goto L8c611c58; /* mov r5,r3 */
L8c611c58: sa_motion_publish_stop(ctx,0x8c611c58u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c611c5a; /* fsub fr6,fr3 */
L8c611c5a: sa_motion_publish_stop(ctx,0x8c611c5au); r[3]<<=2;goto L8c611c5c; /* shll2 r3 */
L8c611c5c: sa_motion_publish_stop(ctx,0x8c611c5cu); r[3]+=r[6];goto L8c611c5e; /* add r6,r3 */
L8c611c5e: sa_motion_publish_stop(ctx,0x8c611c5eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611c60; /* fmac fr0,fr3,fr2 */
L8c611c60: sa_motion_publish_stop(ctx,0x8c611c60u); fstore(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c611c62; /* fmov.s fr2,@r2 */
L8c611c62: sa_motion_publish_stop(ctx,0x8c611c62u); wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c611c64; /* mov.l r3,@(0xc,r15) */
L8c611c64: sa_motion_publish_stop(ctx,0x8c611c64u); r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611c66; /* mov.l @r15,r0 */
L8c611c66: sa_motion_publish_stop(ctx,0x8c611c66u); r[0]+=r[7];goto L8c611c68; /* add r7,r0 */
L8c611c68: sa_motion_publish_stop(ctx,0x8c611c68u); wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;goto L8c611c6a; /* mov.l r0,@(0x8,r15) */
L8c611c6a: sa_motion_publish_stop(ctx,0x8c611c6au); r[2]=r[13];goto L8c611c6c; /* mov r13,r2 */
L8c611c6c: sa_motion_publish_stop(ctx,0x8c611c6cu); r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c611c6e; /* mov.w @r0,r0 */
L8c611c6e: sa_motion_publish_stop(ctx,0x8c611c6eu); r[2]+=r[7];goto L8c611c70; /* add r7,r2 */
L8c611c70: sa_motion_publish_stop(ctx,0x8c611c70u); wr(m,bus,&io_status,r[15]+0x14u,r[0],2);if(io_status)return io_status;goto L8c611c72; /* mov.w r0,@(0x14,r15) */
L8c611c72: sa_motion_publish_stop(ctx,0x8c611c72u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c611c74; /* mov.l r2,@(0x4,r15) */
L8c611c74: sa_motion_publish_stop(ctx,0x8c611c74u); *fpul=r[0];goto L8c611c76; /* lds r0,FPUL */
L8c611c76: sa_motion_publish_stop(ctx,0x8c611c76u); r[0]=sx(m,bus,&io_status,r[15]+0x14u,2);if(io_status)return io_status;goto L8c611c78; /* mov.w @(0x14,r15),r0 */
L8c611c78: sa_motion_publish_stop(ctx,0x8c611c78u); r[2]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c611c7a; /* mov.w @r2,r2 */
L8c611c7a: sa_motion_publish_stop(ctx,0x8c611c7au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611c7c; /* float FPUL,fr3 */
L8c611c7c: sa_motion_publish_stop(ctx,0x8c611c7cu); r[2]-=r[0];goto L8c611c7e; /* sub r0,r2 */
L8c611c7e: sa_motion_publish_stop(ctx,0x8c611c7eu); *fpul=r[2];goto L8c611c80; /* lds r2,FPUL */
L8c611c80: sa_motion_publish_stop(ctx,0x8c611c80u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c611c82; /* float FPUL,fr2 */
L8c611c82: sa_motion_publish_stop(ctx,0x8c611c82u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611c84; /* fmac fr0,fr2,fr3 */
L8c611c84: sa_motion_publish_stop(ctx,0x8c611c84u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c611c86; /* ftrc fr3,FPUL */
L8c611c86: sa_motion_publish_stop(ctx,0x8c611c86u); r[2]=*fpul;goto L8c611c88; /* sts FPUL,r2 */
L8c611c88: sa_motion_publish_stop(ctx,0x8c611c88u); wr(m,bus,&io_status,r[3],r[2],2);if(io_status)return io_status;goto L8c611c8a; /* mov.w r2,@r3 */
L8c611c8a: sa_motion_publish_stop(ctx,0x8c611c8au); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c611c8c; /* mov.l @(0x8,r15),r2 */
L8c611c8c: sa_motion_publish_stop(ctx,0x8c611c8cu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c611c8e; /* mov.l @(0xc,r15),r3 */
L8c611c8e: sa_motion_publish_stop(ctx,0x8c611c8eu); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c611c90; /* mov.w @(0x2,r2),r0 */
L8c611c90: sa_motion_publish_stop(ctx,0x8c611c90u); *fpul=r[0];goto L8c611c92; /* lds r0,FPUL */
L8c611c92: sa_motion_publish_stop(ctx,0x8c611c92u); wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c611c94; /* mov.w r0,@r15 */
L8c611c94: sa_motion_publish_stop(ctx,0x8c611c94u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611c96; /* mov.l @(0x4,r15),r2 */
L8c611c96: sa_motion_publish_stop(ctx,0x8c611c96u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611c98; /* float FPUL,fr3 */
L8c611c98: sa_motion_publish_stop(ctx,0x8c611c98u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c611c9a; /* mov.w @(0x2,r2),r0 */
L8c611c9a: sa_motion_publish_stop(ctx,0x8c611c9au); r[2]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c611c9c; /* mov.w @r15,r2 */
L8c611c9c: sa_motion_publish_stop(ctx,0x8c611c9cu); r[0]-=r[2];goto L8c611c9e; /* sub r2,r0 */
L8c611c9e: sa_motion_publish_stop(ctx,0x8c611c9eu); *fpul=r[0];goto L8c611ca0; /* lds r0,FPUL */
L8c611ca0: sa_motion_publish_stop(ctx,0x8c611ca0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c611ca2; /* float FPUL,fr2 */
L8c611ca2: sa_motion_publish_stop(ctx,0x8c611ca2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611ca4; /* fmac fr0,fr2,fr3 */
L8c611ca4: sa_motion_publish_stop(ctx,0x8c611ca4u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c611ca6; /* ftrc fr3,FPUL */
L8c611ca6: sa_motion_publish_stop(ctx,0x8c611ca6u); r[0]=*fpul;goto L8c611ca8; /* sts FPUL,r0 */
L8c611ca8: sa_motion_publish_stop(ctx,0x8c611ca8u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c611caa; /* mov.w r0,@(0x2,r3) */
L8c611caa: sa_motion_publish_stop(ctx,0x8c611caau); r[5]+=1u;goto L8c611cac; /* add #0x1,r5 */
L8c611cac: sa_motion_publish_stop(ctx,0x8c611cacu); r[0]=r[13];goto L8c611cae; /* mov r13,r0 */
L8c611cae: sa_motion_publish_stop(ctx,0x8c611caeu); r[3]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c611cb0; /* mov.l @(r0,r14),r3 */
L8c611cb0: sa_motion_publish_stop(ctx,0x8c611cb0u); r[3]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c611cb2; /* mov.l @(0xc,r3),r3 */
L8c611cb2: sa_motion_publish_stop(ctx,0x8c611cb2u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c611cb4; /* tst r4,r3 */
L8c611cb4: sa_motion_publish_stop(ctx,0x8c611cb4u); if(!(*sr&1u))goto L8c611d10;goto L8c611cb6; /* bf 0x8c611d10 */
L8c611cb6: sa_motion_publish_stop(ctx,0x8c611cb6u); r[3]=r[13];goto L8c611cb8; /* mov r13,r3 */
L8c611cb8: sa_motion_publish_stop(ctx,0x8c611cb8u); r[2]=r[5];goto L8c611cba; /* mov r5,r2 */
L8c611cba: sa_motion_publish_stop(ctx,0x8c611cbau); r[3]+=r[14];goto L8c611cbc; /* add r14,r3 */
L8c611cbc: sa_motion_publish_stop(ctx,0x8c611cbcu); r[2]<<=2;goto L8c611cbe; /* shll2 r2 */
L8c611cbe: sa_motion_publish_stop(ctx,0x8c611cbeu); r[0]=16u;goto L8c611cc0; /* mov #0x10,r0 */
L8c611cc0: sa_motion_publish_stop(ctx,0x8c611cc0u); r[2]<<=2;goto L8c611cc2; /* shll2 r2 */
L8c611cc2: sa_motion_publish_stop(ctx,0x8c611cc2u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c611cc4; /* shll r2 */
L8c611cc4: sa_motion_publish_stop(ctx,0x8c611cc4u); r[2]+=r[12];goto L8c611cc6; /* add r12,r2 */
L8c611cc6: sa_motion_publish_stop(ctx,0x8c611cc6u); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c611cc8; /* mov.l r2,@r15 */
L8c611cc8: sa_motion_publish_stop(ctx,0x8c611cc8u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c611cca; /* mov.l r3,@(0x4,r15) */
L8c611cca: sa_motion_publish_stop(ctx,0x8c611ccau); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c611ccc; /* mov.l @r3,r1 */
L8c611ccc: sa_motion_publish_stop(ctx,0x8c611cccu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c611cce; /* fmov.s @(r0,r1),fr3 */
L8c611cce: sa_motion_publish_stop(ctx,0x8c611cceu); fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c611cd0; /* fmov fr3,@(r0,r2) */
L8c611cd0: sa_motion_publish_stop(ctx,0x8c611cd0u); r[0]=20u;goto L8c611cd2; /* mov #0x14,r0 */
L8c611cd2: sa_motion_publish_stop(ctx,0x8c611cd2u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611cd4; /* mov.l @(0x4,r15),r2 */
L8c611cd4: sa_motion_publish_stop(ctx,0x8c611cd4u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611cd6; /* mov.l @r15,r3 */
L8c611cd6: sa_motion_publish_stop(ctx,0x8c611cd6u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c611cd8; /* mov.l @r2,r1 */
L8c611cd8: sa_motion_publish_stop(ctx,0x8c611cd8u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c611cda; /* fmov.s @(r0,r1),fr3 */
L8c611cda: sa_motion_publish_stop(ctx,0x8c611cdau); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c611cdc; /* fmov fr3,@(r0,r3) */
L8c611cdc: sa_motion_publish_stop(ctx,0x8c611cdcu); r[0]=24u;goto L8c611cde; /* mov #0x18,r0 */
L8c611cde: sa_motion_publish_stop(ctx,0x8c611cdeu); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611ce0; /* mov.l @(0x4,r15),r2 */
L8c611ce0: sa_motion_publish_stop(ctx,0x8c611ce0u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611ce2; /* mov.l @r15,r3 */
L8c611ce2: sa_motion_publish_stop(ctx,0x8c611ce2u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c611ce4; /* mov.l @r2,r1 */
L8c611ce4: sa_motion_publish_stop(ctx,0x8c611ce4u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c611ce6; /* fmov.s @(r0,r1),fr3 */
L8c611ce6: sa_motion_publish_stop(ctx,0x8c611ce6u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c611ce8; /* fmov fr3,@(r0,r3) */
L8c611ce8: sa_motion_publish_stop(ctx,0x8c611ce8u); r[0]=28u;goto L8c611cea; /* mov #0x1c,r0 */
L8c611cea: sa_motion_publish_stop(ctx,0x8c611ceau); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611cec; /* mov.l @(0x4,r15),r2 */
L8c611cec: sa_motion_publish_stop(ctx,0x8c611cecu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611cee; /* mov.l @r15,r3 */
L8c611cee: sa_motion_publish_stop(ctx,0x8c611ceeu); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c611cf0; /* mov.l @r2,r1 */
L8c611cf0: sa_motion_publish_stop(ctx,0x8c611cf0u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c611cf2; /* fmov.s @(r0,r1),fr3 */
L8c611cf2: sa_motion_publish_stop(ctx,0x8c611cf2u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c611cf4; /* fmov fr3,@(r0,r3) */
L8c611cf4: sa_motion_publish_stop(ctx,0x8c611cf4u); r[3]=r[5];goto L8c611cf6; /* mov r5,r3 */
L8c611cf6: sa_motion_publish_stop(ctx,0x8c611cf6u); r[2]=r[13];goto L8c611cf8; /* mov r13,r2 */
L8c611cf8: sa_motion_publish_stop(ctx,0x8c611cf8u); r[2]+=r[7];goto L8c611cfa; /* add r7,r2 */
L8c611cfa: sa_motion_publish_stop(ctx,0x8c611cfau); r[3]<<=2;goto L8c611cfc; /* shll2 r3 */
L8c611cfc: sa_motion_publish_stop(ctx,0x8c611cfcu); r[3]+=r[6];goto L8c611cfe; /* add r6,r3 */
L8c611cfe: sa_motion_publish_stop(ctx,0x8c611cfeu); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c611d00; /* mov.l r3,@(0x4,r15) */
L8c611d00: sa_motion_publish_stop(ctx,0x8c611d00u); r[5]+=1u;goto L8c611d02; /* add #0x1,r5 */
L8c611d02: sa_motion_publish_stop(ctx,0x8c611d02u); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c611d04; /* mov.l r2,@r15 */
L8c611d04: sa_motion_publish_stop(ctx,0x8c611d04u); r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c611d06; /* mov.w @r2,r1 */
L8c611d06: sa_motion_publish_stop(ctx,0x8c611d06u); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c611d08; /* mov.w r1,@r3 */
L8c611d08: sa_motion_publish_stop(ctx,0x8c611d08u); r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611d0a; /* mov.l @r15,r0 */
L8c611d0a: sa_motion_publish_stop(ctx,0x8c611d0au); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c611d0c; /* mov.l @(0x4,r15),r3 */
L8c611d0c: sa_motion_publish_stop(ctx,0x8c611d0cu); r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c611d0e; /* mov.w @(0x2,r0),r0 */
L8c611d0e: sa_motion_publish_stop(ctx,0x8c611d0eu); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c611d10; /* mov.w r0,@(0x2,r3) */
L8c611d10: sa_motion_publish_stop(ctx,0x8c611d10u); wr(m,bus,&io_status,r[15]+0x4u,r[10],4);if(io_status)return io_status;goto L8c611d12; /* mov.l r10,@(0x4,r15) */
L8c611d12: sa_motion_publish_stop(ctx,0x8c611d12u); r[3]=3u;goto L8c611d14; /* mov #0x3,r3 */
L8c611d14: sa_motion_publish_stop(ctx,0x8c611d14u); r[10]+=1u;goto L8c611d16; /* add #0x1,r10 */
L8c611d16: sa_motion_publish_stop(ctx,0x8c611d16u); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[3]);goto L8c611d18; /* cmp/ge r3,r10 */
L8c611d18: sa_motion_publish_stop(ctx,0x8c611d18u); if((*sr&1u))goto L8c611d1e;goto L8c611d1a; /* bt 0x8c611d1e */
L8c611d1a: sa_motion_publish_stop(ctx,0x8c611d1cu); goto L8c611b86; /* bra 0x8c611b86 */
L8c611d1e: sa_motion_publish_stop(ctx,0x8c611d1eu); r[0]=0x8c611e0cu;goto L8c611d20; /* mova 0x8c611e0c,r0 */
L8c611d20: sa_motion_publish_stop(ctx,0x8c611d20u); r[4]=rd(m,bus,&io_status,0x8c611e08u,4);if(io_status)return io_status;goto L8c611d22; /* mov.l 0x8c611e08,r4 */
L8c611d22: sa_motion_publish_stop(ctx,0x8c611d22u); fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c611d24; /* fmov.s @r0,fr4 */
L8c611d24: sa_motion_publish_stop(ctx,0x8c611d24u); r[0]=r[5];goto L8c611d26; /* mov r5,r0 */
L8c611d26: sa_motion_publish_stop(ctx,0x8c611d26u); r[7]=rd(m,bus,&io_status,0x8c611e04u,4);if(io_status)return io_status;goto L8c611d28; /* mov.l 0x8c611e04,r7 */
L8c611d28: sa_motion_publish_stop(ctx,0x8c611d28u); *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c611d2a; /* cmp/eq #0x3,r0 */
L8c611d2a: sa_motion_publish_stop(ctx,0x8c611d2au); if(!(*sr&1u))goto L8c611e14;goto L8c611d2c; /* bf 0x8c611e14 */
L8c611d2c: sa_motion_publish_stop(ctx,0x8c611d2cu); r[0]=28u;goto L8c611d2e; /* mov #0x1c,r0 */
L8c611d2e: sa_motion_publish_stop(ctx,0x8c611d2eu); r[5]=r[12];goto L8c611d30; /* mov r12,r5 */
L8c611d30: sa_motion_publish_stop(ctx,0x8c611d30u); r[3]=r[6];goto L8c611d32; /* mov r6,r3 */
L8c611d32: sa_motion_publish_stop(ctx,0x8c611d32u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611d34; /* fmov.s @(r0,r5),fr3 */
L8c611d34: sa_motion_publish_stop(ctx,0x8c611d34u); r[8]+=28u;goto L8c611d36; /* add #0x1c,r8 */
L8c611d36: sa_motion_publish_stop(ctx,0x8c611d36u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d38; /* fmov.s fr3,@-r8 */
L8c611d38: sa_motion_publish_stop(ctx,0x8c611d38u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c611d3a; /* mov.l r3,@r15 */
L8c611d3a: sa_motion_publish_stop(ctx,0x8c611d3au); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c611d3c; /* mov.w @(0x2,r3),r0 */
L8c611d3c: sa_motion_publish_stop(ctx,0x8c611d3cu); r[3]=r[0];goto L8c611d3e; /* mov r0,r3 */
L8c611d3e: sa_motion_publish_stop(ctx,0x8c611d3eu); *fpul=r[3];goto L8c611d40; /* lds r3,FPUL */
L8c611d40: sa_motion_publish_stop(ctx,0x8c611d40u); r[0]=24u;goto L8c611d42; /* mov #0x18,r0 */
L8c611d42: sa_motion_publish_stop(ctx,0x8c611d42u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611d44; /* float FPUL,fr3 */
L8c611d44: sa_motion_publish_stop(ctx,0x8c611d44u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611d46; /* fmul fr4,fr3 */
L8c611d46: sa_motion_publish_stop(ctx,0x8c611d46u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d48; /* fmov.s fr3,@-r8 */
L8c611d48: sa_motion_publish_stop(ctx,0x8c611d48u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611d4a; /* mov.l @r15,r3 */
L8c611d4a: sa_motion_publish_stop(ctx,0x8c611d4au); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c611d4c; /* mov.w @r3,r3 */
L8c611d4c: sa_motion_publish_stop(ctx,0x8c611d4cu); *fpul=r[3];goto L8c611d4e; /* lds r3,FPUL */
L8c611d4e: sa_motion_publish_stop(ctx,0x8c611d4eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611d50; /* float FPUL,fr3 */
L8c611d50: sa_motion_publish_stop(ctx,0x8c611d50u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611d52; /* fmul fr4,fr3 */
L8c611d52: sa_motion_publish_stop(ctx,0x8c611d52u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d54; /* fmov.s fr3,@-r8 */
L8c611d54: sa_motion_publish_stop(ctx,0x8c611d54u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611d56; /* fmov.s @(r0,r5),fr3 */
L8c611d56: sa_motion_publish_stop(ctx,0x8c611d56u); r[0]=20u;goto L8c611d58; /* mov #0x14,r0 */
L8c611d58: sa_motion_publish_stop(ctx,0x8c611d58u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d5a; /* fmov.s fr3,@-r8 */
L8c611d5a: sa_motion_publish_stop(ctx,0x8c611d5au); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611d5c; /* fmov.s @(r0,r5),fr3 */
L8c611d5c: sa_motion_publish_stop(ctx,0x8c611d5cu); r[0]=16u;goto L8c611d5e; /* mov #0x10,r0 */
L8c611d5e: sa_motion_publish_stop(ctx,0x8c611d5eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d60; /* fmov.s fr3,@-r8 */
L8c611d60: sa_motion_publish_stop(ctx,0x8c611d60u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611d62; /* fmov.s @(r0,r5),fr3 */
L8c611d62: sa_motion_publish_stop(ctx,0x8c611d62u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d64; /* fmov.s fr3,@-r8 */
L8c611d64: sa_motion_publish_stop(ctx,0x8c611d64u); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c611d66; /* mov.l r4,@-r8 */
L8c611d66: sa_motion_publish_stop(ctx,0x8c611d66u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c611d68; /* pref @r8 */
L8c611d68: sa_motion_publish_stop(ctx,0x8c611d68u); r[0]=28u;goto L8c611d6a; /* mov #0x1c,r0 */
L8c611d6a: sa_motion_publish_stop(ctx,0x8c611d6au); r[3]=r[6];goto L8c611d6c; /* mov r6,r3 */
L8c611d6c: sa_motion_publish_stop(ctx,0x8c611d6cu); r[5]+=32u;goto L8c611d6e; /* add #0x20,r5 */
L8c611d6e: sa_motion_publish_stop(ctx,0x8c611d6eu); r[3]+=4u;goto L8c611d70; /* add #0x4,r3 */
L8c611d70: sa_motion_publish_stop(ctx,0x8c611d70u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611d72; /* fmov.s @(r0,r5),fr3 */
L8c611d72: sa_motion_publish_stop(ctx,0x8c611d72u); r[8]+=60u;goto L8c611d74; /* add #0x3c,r8 */
L8c611d74: sa_motion_publish_stop(ctx,0x8c611d74u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d76; /* fmov.s fr3,@-r8 */
L8c611d76: sa_motion_publish_stop(ctx,0x8c611d76u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c611d78; /* mov.l r3,@r15 */
L8c611d78: sa_motion_publish_stop(ctx,0x8c611d78u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c611d7a; /* mov.w @(0x2,r3),r0 */
L8c611d7a: sa_motion_publish_stop(ctx,0x8c611d7au); r[3]=r[0];goto L8c611d7c; /* mov r0,r3 */
L8c611d7c: sa_motion_publish_stop(ctx,0x8c611d7cu); *fpul=r[3];goto L8c611d7e; /* lds r3,FPUL */
L8c611d7e: sa_motion_publish_stop(ctx,0x8c611d7eu); r[0]=24u;goto L8c611d80; /* mov #0x18,r0 */
L8c611d80: sa_motion_publish_stop(ctx,0x8c611d80u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611d82; /* float FPUL,fr3 */
L8c611d82: sa_motion_publish_stop(ctx,0x8c611d82u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611d84; /* fmul fr4,fr3 */
L8c611d84: sa_motion_publish_stop(ctx,0x8c611d84u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d86; /* fmov.s fr3,@-r8 */
L8c611d86: sa_motion_publish_stop(ctx,0x8c611d86u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611d88; /* mov.l @r15,r3 */
L8c611d88: sa_motion_publish_stop(ctx,0x8c611d88u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c611d8a; /* mov.w @r3,r3 */
L8c611d8a: sa_motion_publish_stop(ctx,0x8c611d8au); *fpul=r[3];goto L8c611d8c; /* lds r3,FPUL */
L8c611d8c: sa_motion_publish_stop(ctx,0x8c611d8cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611d8e; /* float FPUL,fr3 */
L8c611d8e: sa_motion_publish_stop(ctx,0x8c611d8eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611d90; /* fmul fr4,fr3 */
L8c611d90: sa_motion_publish_stop(ctx,0x8c611d90u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d92; /* fmov.s fr3,@-r8 */
L8c611d92: sa_motion_publish_stop(ctx,0x8c611d92u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611d94; /* fmov.s @(r0,r5),fr3 */
L8c611d94: sa_motion_publish_stop(ctx,0x8c611d94u); r[0]=20u;goto L8c611d96; /* mov #0x14,r0 */
L8c611d96: sa_motion_publish_stop(ctx,0x8c611d96u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d98; /* fmov.s fr3,@-r8 */
L8c611d98: sa_motion_publish_stop(ctx,0x8c611d98u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611d9a; /* fmov.s @(r0,r5),fr3 */
L8c611d9a: sa_motion_publish_stop(ctx,0x8c611d9au); r[0]=16u;goto L8c611d9c; /* mov #0x10,r0 */
L8c611d9c: sa_motion_publish_stop(ctx,0x8c611d9cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611d9e; /* fmov.s fr3,@-r8 */
L8c611d9e: sa_motion_publish_stop(ctx,0x8c611d9eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611da0; /* fmov.s @(r0,r5),fr3 */
L8c611da0: sa_motion_publish_stop(ctx,0x8c611da0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611da2; /* fmov.s fr3,@-r8 */
L8c611da2: sa_motion_publish_stop(ctx,0x8c611da2u); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c611da4; /* mov.l r4,@-r8 */
L8c611da4: sa_motion_publish_stop(ctx,0x8c611da4u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c611da6; /* pref @r8 */
L8c611da6: sa_motion_publish_stop(ctx,0x8c611da6u); r[4]=r[12];goto L8c611da8; /* mov r12,r4 */
L8c611da8: sa_motion_publish_stop(ctx,0x8c611da8u); r[0]=28u;goto L8c611daa; /* mov #0x1c,r0 */
L8c611daa: sa_motion_publish_stop(ctx,0x8c611daau); r[5]=r[6];goto L8c611dac; /* mov r6,r5 */
L8c611dac: sa_motion_publish_stop(ctx,0x8c611dacu); r[4]+=64u;goto L8c611dae; /* add #0x40,r4 */
L8c611dae: sa_motion_publish_stop(ctx,0x8c611daeu); r[5]+=8u;goto L8c611db0; /* add #0x8,r5 */
L8c611db0: sa_motion_publish_stop(ctx,0x8c611db0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611db2; /* fmov.s @(r0,r4),fr3 */
L8c611db2: sa_motion_publish_stop(ctx,0x8c611db2u); r[8]+=60u;goto L8c611db4; /* add #0x3c,r8 */
L8c611db4: sa_motion_publish_stop(ctx,0x8c611db4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611db6; /* fmov.s fr3,@-r8 */
L8c611db6: sa_motion_publish_stop(ctx,0x8c611db6u); r[0]=sx(m,bus,&io_status,r[5]+0x2u,2);if(io_status)return io_status;goto L8c611db8; /* mov.w @(0x2,r5),r0 */
L8c611db8: sa_motion_publish_stop(ctx,0x8c611db8u); r[3]=r[0];goto L8c611dba; /* mov r0,r3 */
L8c611dba: sa_motion_publish_stop(ctx,0x8c611dbau); *fpul=r[3];goto L8c611dbc; /* lds r3,FPUL */
L8c611dbc: sa_motion_publish_stop(ctx,0x8c611dbcu); r[0]=24u;goto L8c611dbe; /* mov #0x18,r0 */
L8c611dbe: sa_motion_publish_stop(ctx,0x8c611dbeu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611dc0; /* float FPUL,fr3 */
L8c611dc0: sa_motion_publish_stop(ctx,0x8c611dc0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611dc2; /* fmul fr4,fr3 */
L8c611dc2: sa_motion_publish_stop(ctx,0x8c611dc2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611dc4; /* fmov.s fr3,@-r8 */
L8c611dc4: sa_motion_publish_stop(ctx,0x8c611dc4u); r[3]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;goto L8c611dc6; /* mov.w @r5,r3 */
L8c611dc6: sa_motion_publish_stop(ctx,0x8c611dc6u); *fpul=r[3];goto L8c611dc8; /* lds r3,FPUL */
L8c611dc8: sa_motion_publish_stop(ctx,0x8c611dc8u); r[3]=rd(m,bus,&io_status,0x8c611e10u,4);if(io_status)return io_status;goto L8c611dca; /* mov.l 0x8c611e10,r3 */
L8c611dca: sa_motion_publish_stop(ctx,0x8c611dcau); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611dcc; /* float FPUL,fr3 */
L8c611dcc: sa_motion_publish_stop(ctx,0x8c611dccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611dce; /* fmul fr4,fr3 */
L8c611dce: sa_motion_publish_stop(ctx,0x8c611dceu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611dd0; /* fmov.s fr3,@-r8 */
L8c611dd0: sa_motion_publish_stop(ctx,0x8c611dd0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611dd2; /* fmov.s @(r0,r4),fr3 */
L8c611dd2: sa_motion_publish_stop(ctx,0x8c611dd2u); r[0]=20u;goto L8c611dd4; /* mov #0x14,r0 */
L8c611dd4: sa_motion_publish_stop(ctx,0x8c611dd4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611dd6; /* fmov.s fr3,@-r8 */
L8c611dd6: sa_motion_publish_stop(ctx,0x8c611dd6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611dd8; /* fmov.s @(r0,r4),fr3 */
L8c611dd8: sa_motion_publish_stop(ctx,0x8c611dd8u); r[0]=16u;goto L8c611dda; /* mov #0x10,r0 */
L8c611dda: sa_motion_publish_stop(ctx,0x8c611ddau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611ddc; /* fmov.s fr3,@-r8 */
L8c611ddc: sa_motion_publish_stop(ctx,0x8c611ddcu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611dde; /* fmov.s @(r0,r4),fr3 */
L8c611dde: sa_motion_publish_stop(ctx,0x8c611ddeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611de0; /* fmov.s fr3,@-r8 */
L8c611de0: sa_motion_publish_stop(ctx,0x8c611de0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c611de2; /* mov.l r3,@-r8 */
L8c611de2: sa_motion_publish_stop(ctx,0x8c611de2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c611de4; /* pref @r8 */
L8c611de4: sa_motion_publish_stop(ctx,0x8c611de4u); r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c611de6; /* mov.l @r7,r2 */
L8c611de6: sa_motion_publish_stop(ctx,0x8c611de6u); r[8]+=32u;goto L8c611de8; /* add #0x20,r8 */
L8c611de8: sa_motion_publish_stop(ctx,0x8c611deau); r[2]+=3u;goto L8c611f12; /* bra 0x8c611f12 */
L8c611e14: sa_motion_publish_stop(ctx,0x8c611e14u); r[0]=28u;goto L8c611e16; /* mov #0x1c,r0 */
L8c611e16: sa_motion_publish_stop(ctx,0x8c611e16u); r[5]=r[12];goto L8c611e18; /* mov r12,r5 */
L8c611e18: sa_motion_publish_stop(ctx,0x8c611e18u); r[3]=r[6];goto L8c611e1a; /* mov r6,r3 */
L8c611e1a: sa_motion_publish_stop(ctx,0x8c611e1au); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e1c; /* fmov.s @(r0,r5),fr3 */
L8c611e1c: sa_motion_publish_stop(ctx,0x8c611e1cu); r[8]+=28u;goto L8c611e1e; /* add #0x1c,r8 */
L8c611e1e: sa_motion_publish_stop(ctx,0x8c611e1eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e20; /* fmov.s fr3,@-r8 */
L8c611e20: sa_motion_publish_stop(ctx,0x8c611e20u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c611e22; /* mov.l r3,@r15 */
L8c611e22: sa_motion_publish_stop(ctx,0x8c611e22u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c611e24; /* mov.w @(0x2,r3),r0 */
L8c611e24: sa_motion_publish_stop(ctx,0x8c611e24u); r[3]=r[0];goto L8c611e26; /* mov r0,r3 */
L8c611e26: sa_motion_publish_stop(ctx,0x8c611e26u); *fpul=r[3];goto L8c611e28; /* lds r3,FPUL */
L8c611e28: sa_motion_publish_stop(ctx,0x8c611e28u); r[0]=24u;goto L8c611e2a; /* mov #0x18,r0 */
L8c611e2a: sa_motion_publish_stop(ctx,0x8c611e2au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611e2c; /* float FPUL,fr3 */
L8c611e2c: sa_motion_publish_stop(ctx,0x8c611e2cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611e2e; /* fmul fr4,fr3 */
L8c611e2e: sa_motion_publish_stop(ctx,0x8c611e2eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e30; /* fmov.s fr3,@-r8 */
L8c611e30: sa_motion_publish_stop(ctx,0x8c611e30u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611e32; /* mov.l @r15,r3 */
L8c611e32: sa_motion_publish_stop(ctx,0x8c611e32u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c611e34; /* mov.w @r3,r3 */
L8c611e34: sa_motion_publish_stop(ctx,0x8c611e34u); *fpul=r[3];goto L8c611e36; /* lds r3,FPUL */
L8c611e36: sa_motion_publish_stop(ctx,0x8c611e36u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611e38; /* float FPUL,fr3 */
L8c611e38: sa_motion_publish_stop(ctx,0x8c611e38u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611e3a; /* fmul fr4,fr3 */
L8c611e3a: sa_motion_publish_stop(ctx,0x8c611e3au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e3c; /* fmov.s fr3,@-r8 */
L8c611e3c: sa_motion_publish_stop(ctx,0x8c611e3cu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e3e; /* fmov.s @(r0,r5),fr3 */
L8c611e3e: sa_motion_publish_stop(ctx,0x8c611e3eu); r[0]=20u;goto L8c611e40; /* mov #0x14,r0 */
L8c611e40: sa_motion_publish_stop(ctx,0x8c611e40u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e42; /* fmov.s fr3,@-r8 */
L8c611e42: sa_motion_publish_stop(ctx,0x8c611e42u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e44; /* fmov.s @(r0,r5),fr3 */
L8c611e44: sa_motion_publish_stop(ctx,0x8c611e44u); r[0]=16u;goto L8c611e46; /* mov #0x10,r0 */
L8c611e46: sa_motion_publish_stop(ctx,0x8c611e46u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e48; /* fmov.s fr3,@-r8 */
L8c611e48: sa_motion_publish_stop(ctx,0x8c611e48u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e4a; /* fmov.s @(r0,r5),fr3 */
L8c611e4a: sa_motion_publish_stop(ctx,0x8c611e4au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e4c; /* fmov.s fr3,@-r8 */
L8c611e4c: sa_motion_publish_stop(ctx,0x8c611e4cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c611e4e; /* mov.l r4,@-r8 */
L8c611e4e: sa_motion_publish_stop(ctx,0x8c611e4eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c611e50; /* pref @r8 */
L8c611e50: sa_motion_publish_stop(ctx,0x8c611e50u); r[0]=28u;goto L8c611e52; /* mov #0x1c,r0 */
L8c611e52: sa_motion_publish_stop(ctx,0x8c611e52u); r[3]=r[6];goto L8c611e54; /* mov r6,r3 */
L8c611e54: sa_motion_publish_stop(ctx,0x8c611e54u); r[5]+=32u;goto L8c611e56; /* add #0x20,r5 */
L8c611e56: sa_motion_publish_stop(ctx,0x8c611e56u); r[3]+=4u;goto L8c611e58; /* add #0x4,r3 */
L8c611e58: sa_motion_publish_stop(ctx,0x8c611e58u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e5a; /* fmov.s @(r0,r5),fr3 */
L8c611e5a: sa_motion_publish_stop(ctx,0x8c611e5au); r[8]+=60u;goto L8c611e5c; /* add #0x3c,r8 */
L8c611e5c: sa_motion_publish_stop(ctx,0x8c611e5cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e5e; /* fmov.s fr3,@-r8 */
L8c611e5e: sa_motion_publish_stop(ctx,0x8c611e5eu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c611e60; /* mov.l r3,@r15 */
L8c611e60: sa_motion_publish_stop(ctx,0x8c611e60u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c611e62; /* mov.w @(0x2,r3),r0 */
L8c611e62: sa_motion_publish_stop(ctx,0x8c611e62u); r[3]=r[0];goto L8c611e64; /* mov r0,r3 */
L8c611e64: sa_motion_publish_stop(ctx,0x8c611e64u); *fpul=r[3];goto L8c611e66; /* lds r3,FPUL */
L8c611e66: sa_motion_publish_stop(ctx,0x8c611e66u); r[0]=24u;goto L8c611e68; /* mov #0x18,r0 */
L8c611e68: sa_motion_publish_stop(ctx,0x8c611e68u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611e6a; /* float FPUL,fr3 */
L8c611e6a: sa_motion_publish_stop(ctx,0x8c611e6au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611e6c; /* fmul fr4,fr3 */
L8c611e6c: sa_motion_publish_stop(ctx,0x8c611e6cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e6e; /* fmov.s fr3,@-r8 */
L8c611e6e: sa_motion_publish_stop(ctx,0x8c611e6eu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611e70; /* mov.l @r15,r3 */
L8c611e70: sa_motion_publish_stop(ctx,0x8c611e70u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c611e72; /* mov.w @r3,r3 */
L8c611e72: sa_motion_publish_stop(ctx,0x8c611e72u); *fpul=r[3];goto L8c611e74; /* lds r3,FPUL */
L8c611e74: sa_motion_publish_stop(ctx,0x8c611e74u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611e76; /* float FPUL,fr3 */
L8c611e76: sa_motion_publish_stop(ctx,0x8c611e76u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611e78; /* fmul fr4,fr3 */
L8c611e78: sa_motion_publish_stop(ctx,0x8c611e78u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e7a; /* fmov.s fr3,@-r8 */
L8c611e7a: sa_motion_publish_stop(ctx,0x8c611e7au); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e7c; /* fmov.s @(r0,r5),fr3 */
L8c611e7c: sa_motion_publish_stop(ctx,0x8c611e7cu); r[0]=20u;goto L8c611e7e; /* mov #0x14,r0 */
L8c611e7e: sa_motion_publish_stop(ctx,0x8c611e7eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e80; /* fmov.s fr3,@-r8 */
L8c611e80: sa_motion_publish_stop(ctx,0x8c611e80u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e82; /* fmov.s @(r0,r5),fr3 */
L8c611e82: sa_motion_publish_stop(ctx,0x8c611e82u); r[0]=16u;goto L8c611e84; /* mov #0x10,r0 */
L8c611e84: sa_motion_publish_stop(ctx,0x8c611e84u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e86; /* fmov.s fr3,@-r8 */
L8c611e86: sa_motion_publish_stop(ctx,0x8c611e86u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e88; /* fmov.s @(r0,r5),fr3 */
L8c611e88: sa_motion_publish_stop(ctx,0x8c611e88u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e8a; /* fmov.s fr3,@-r8 */
L8c611e8a: sa_motion_publish_stop(ctx,0x8c611e8au); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c611e8c; /* mov.l r4,@-r8 */
L8c611e8c: sa_motion_publish_stop(ctx,0x8c611e8cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c611e8e; /* pref @r8 */
L8c611e8e: sa_motion_publish_stop(ctx,0x8c611e8eu); r[5]=r[12];goto L8c611e90; /* mov r12,r5 */
L8c611e90: sa_motion_publish_stop(ctx,0x8c611e90u); r[0]=28u;goto L8c611e92; /* mov #0x1c,r0 */
L8c611e92: sa_motion_publish_stop(ctx,0x8c611e92u); r[3]=r[6];goto L8c611e94; /* mov r6,r3 */
L8c611e94: sa_motion_publish_stop(ctx,0x8c611e94u); r[5]+=96u;goto L8c611e96; /* add #0x60,r5 */
L8c611e96: sa_motion_publish_stop(ctx,0x8c611e96u); r[3]+=12u;goto L8c611e98; /* add #0xc,r3 */
L8c611e98: sa_motion_publish_stop(ctx,0x8c611e98u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611e9a; /* fmov.s @(r0,r5),fr3 */
L8c611e9a: sa_motion_publish_stop(ctx,0x8c611e9au); r[8]+=60u;goto L8c611e9c; /* add #0x3c,r8 */
L8c611e9c: sa_motion_publish_stop(ctx,0x8c611e9cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611e9e; /* fmov.s fr3,@-r8 */
L8c611e9e: sa_motion_publish_stop(ctx,0x8c611e9eu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c611ea0; /* mov.l r3,@r15 */
L8c611ea0: sa_motion_publish_stop(ctx,0x8c611ea0u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c611ea2; /* mov.w @(0x2,r3),r0 */
L8c611ea2: sa_motion_publish_stop(ctx,0x8c611ea2u); r[3]=r[0];goto L8c611ea4; /* mov r0,r3 */
L8c611ea4: sa_motion_publish_stop(ctx,0x8c611ea4u); *fpul=r[3];goto L8c611ea6; /* lds r3,FPUL */
L8c611ea6: sa_motion_publish_stop(ctx,0x8c611ea6u); r[0]=24u;goto L8c611ea8; /* mov #0x18,r0 */
L8c611ea8: sa_motion_publish_stop(ctx,0x8c611ea8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611eaa; /* float FPUL,fr3 */
L8c611eaa: sa_motion_publish_stop(ctx,0x8c611eaau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611eac; /* fmul fr4,fr3 */
L8c611eac: sa_motion_publish_stop(ctx,0x8c611eacu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611eae; /* fmov.s fr3,@-r8 */
L8c611eae: sa_motion_publish_stop(ctx,0x8c611eaeu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c611eb0; /* mov.l @r15,r3 */
L8c611eb0: sa_motion_publish_stop(ctx,0x8c611eb0u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c611eb2; /* mov.w @r3,r3 */
L8c611eb2: sa_motion_publish_stop(ctx,0x8c611eb2u); *fpul=r[3];goto L8c611eb4; /* lds r3,FPUL */
L8c611eb4: sa_motion_publish_stop(ctx,0x8c611eb4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611eb6; /* float FPUL,fr3 */
L8c611eb6: sa_motion_publish_stop(ctx,0x8c611eb6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611eb8; /* fmul fr4,fr3 */
L8c611eb8: sa_motion_publish_stop(ctx,0x8c611eb8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611eba; /* fmov.s fr3,@-r8 */
L8c611eba: sa_motion_publish_stop(ctx,0x8c611ebau); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611ebc; /* fmov.s @(r0,r5),fr3 */
L8c611ebc: sa_motion_publish_stop(ctx,0x8c611ebcu); r[0]=20u;goto L8c611ebe; /* mov #0x14,r0 */
L8c611ebe: sa_motion_publish_stop(ctx,0x8c611ebeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611ec0; /* fmov.s fr3,@-r8 */
L8c611ec0: sa_motion_publish_stop(ctx,0x8c611ec0u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611ec2; /* fmov.s @(r0,r5),fr3 */
L8c611ec2: sa_motion_publish_stop(ctx,0x8c611ec2u); r[0]=16u;goto L8c611ec4; /* mov #0x10,r0 */
L8c611ec4: sa_motion_publish_stop(ctx,0x8c611ec4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611ec6; /* fmov.s fr3,@-r8 */
L8c611ec6: sa_motion_publish_stop(ctx,0x8c611ec6u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c611ec8; /* fmov.s @(r0,r5),fr3 */
L8c611ec8: sa_motion_publish_stop(ctx,0x8c611ec8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611eca; /* fmov.s fr3,@-r8 */
L8c611eca: sa_motion_publish_stop(ctx,0x8c611ecau); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c611ecc; /* mov.l r4,@-r8 */
L8c611ecc: sa_motion_publish_stop(ctx,0x8c611eccu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c611ece; /* pref @r8 */
L8c611ece: sa_motion_publish_stop(ctx,0x8c611eceu); r[4]=r[12];goto L8c611ed0; /* mov r12,r4 */
L8c611ed0: sa_motion_publish_stop(ctx,0x8c611ed0u); r[0]=28u;goto L8c611ed2; /* mov #0x1c,r0 */
L8c611ed2: sa_motion_publish_stop(ctx,0x8c611ed2u); r[5]=r[6];goto L8c611ed4; /* mov r6,r5 */
L8c611ed4: sa_motion_publish_stop(ctx,0x8c611ed4u); r[4]+=64u;goto L8c611ed6; /* add #0x40,r4 */
L8c611ed6: sa_motion_publish_stop(ctx,0x8c611ed6u); r[5]+=8u;goto L8c611ed8; /* add #0x8,r5 */
L8c611ed8: sa_motion_publish_stop(ctx,0x8c611ed8u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611eda; /* fmov.s @(r0,r4),fr3 */
L8c611eda: sa_motion_publish_stop(ctx,0x8c611edau); r[8]+=60u;goto L8c611edc; /* add #0x3c,r8 */
L8c611edc: sa_motion_publish_stop(ctx,0x8c611edcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611ede; /* fmov.s fr3,@-r8 */
L8c611ede: sa_motion_publish_stop(ctx,0x8c611edeu); r[0]=sx(m,bus,&io_status,r[5]+0x2u,2);if(io_status)return io_status;goto L8c611ee0; /* mov.w @(0x2,r5),r0 */
L8c611ee0: sa_motion_publish_stop(ctx,0x8c611ee0u); r[3]=r[0];goto L8c611ee2; /* mov r0,r3 */
L8c611ee2: sa_motion_publish_stop(ctx,0x8c611ee2u); *fpul=r[3];goto L8c611ee4; /* lds r3,FPUL */
L8c611ee4: sa_motion_publish_stop(ctx,0x8c611ee4u); r[0]=24u;goto L8c611ee6; /* mov #0x18,r0 */
L8c611ee6: sa_motion_publish_stop(ctx,0x8c611ee6u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611ee8; /* float FPUL,fr3 */
L8c611ee8: sa_motion_publish_stop(ctx,0x8c611ee8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611eea; /* fmul fr4,fr3 */
L8c611eea: sa_motion_publish_stop(ctx,0x8c611eeau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611eec; /* fmov.s fr3,@-r8 */
L8c611eec: sa_motion_publish_stop(ctx,0x8c611eecu); r[3]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;goto L8c611eee; /* mov.w @r5,r3 */
L8c611eee: sa_motion_publish_stop(ctx,0x8c611eeeu); *fpul=r[3];goto L8c611ef0; /* lds r3,FPUL */
L8c611ef0: sa_motion_publish_stop(ctx,0x8c611ef0u); r[3]=rd(m,bus,&io_status,0x8c612058u,4);if(io_status)return io_status;goto L8c611ef2; /* mov.l 0x8c612058,r3 */
L8c611ef2: sa_motion_publish_stop(ctx,0x8c611ef2u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c611ef4; /* float FPUL,fr3 */
L8c611ef4: sa_motion_publish_stop(ctx,0x8c611ef4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c611ef6; /* fmul fr4,fr3 */
L8c611ef6: sa_motion_publish_stop(ctx,0x8c611ef6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611ef8; /* fmov.s fr3,@-r8 */
L8c611ef8: sa_motion_publish_stop(ctx,0x8c611ef8u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611efa; /* fmov.s @(r0,r4),fr3 */
L8c611efa: sa_motion_publish_stop(ctx,0x8c611efau); r[0]=20u;goto L8c611efc; /* mov #0x14,r0 */
L8c611efc: sa_motion_publish_stop(ctx,0x8c611efcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611efe; /* fmov.s fr3,@-r8 */
L8c611efe: sa_motion_publish_stop(ctx,0x8c611efeu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611f00; /* fmov.s @(r0,r4),fr3 */
L8c611f00: sa_motion_publish_stop(ctx,0x8c611f00u); r[0]=16u;goto L8c611f02; /* mov #0x10,r0 */
L8c611f02: sa_motion_publish_stop(ctx,0x8c611f02u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611f04; /* fmov.s fr3,@-r8 */
L8c611f04: sa_motion_publish_stop(ctx,0x8c611f04u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c611f06; /* fmov.s @(r0,r4),fr3 */
L8c611f06: sa_motion_publish_stop(ctx,0x8c611f06u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c611f08; /* fmov.s fr3,@-r8 */
L8c611f08: sa_motion_publish_stop(ctx,0x8c611f08u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c611f0a; /* mov.l r3,@-r8 */
L8c611f0a: sa_motion_publish_stop(ctx,0x8c611f0au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c611f0c; /* pref @r8 */
L8c611f0c: sa_motion_publish_stop(ctx,0x8c611f0cu); r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c611f0e; /* mov.l @r7,r2 */
L8c611f0e: sa_motion_publish_stop(ctx,0x8c611f0eu); r[8]+=32u;goto L8c611f10; /* add #0x20,r8 */
L8c611f10: sa_motion_publish_stop(ctx,0x8c611f10u); r[2]+=4u;goto L8c611f12; /* add #0x4,r2 */
L8c611f12: sa_motion_publish_stop(ctx,0x8c611f12u); wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c611f14; /* mov.l r2,@r7 */
L8c611f14: sa_motion_publish_stop(ctx,0x8c611f14u); r[15]+=72u;goto L8c611f16; /* add #0x48,r15 */
L8c611f16: sa_motion_publish_stop(ctx,0x8c611f16u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c611f18; /* mov.l @r15+,r9 */
L8c611f18: sa_motion_publish_stop(ctx,0x8c611f18u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c611f1a; /* mov.l @r15+,r10 */
L8c611f1a: sa_motion_publish_stop(ctx,0x8c611f1au); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c611f1c; /* mov.l @r15+,r11 */
L8c611f1c: sa_motion_publish_stop(ctx,0x8c611f1cu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c611f1e; /* mov.l @r15+,r12 */
L8c611f1e: sa_motion_publish_stop(ctx,0x8c611f1eu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c611f20; /* mov.l @r15+,r13 */
L8c611f20: sa_motion_publish_stop(ctx,0x8c611f22u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
