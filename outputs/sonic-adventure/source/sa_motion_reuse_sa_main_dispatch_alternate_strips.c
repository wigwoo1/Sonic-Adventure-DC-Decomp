#include "sa_draw_motion_stop.h"
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
L8c6149b4: sa_motion_publish_stop(ctx,0x8c6149b4u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6149b6; /* mov.l r14,@-r15 */
L8c6149b6: sa_motion_publish_stop(ctx,0x8c6149b6u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6149b8; /* mov.l r13,@-r15 */
L8c6149b8: sa_motion_publish_stop(ctx,0x8c6149b8u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6149ba; /* mov.l r12,@-r15 */
L8c6149ba: sa_motion_publish_stop(ctx,0x8c6149bau); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6149bc; /* mov.l r11,@-r15 */
L8c6149bc: sa_motion_publish_stop(ctx,0x8c6149bcu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6149be; /* mov.l r10,@-r15 */
L8c6149be: sa_motion_publish_stop(ctx,0x8c6149beu); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6149c0; /* mov.l r9,@-r15 */
L8c6149c0: sa_motion_publish_stop(ctx,0x8c6149c0u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c6149c2; /* fmov.s fr15,@-r15 */
L8c6149c2: sa_motion_publish_stop(ctx,0x8c6149c2u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c6149c4; /* fmov.s fr14,@-r15 */
L8c6149c4: sa_motion_publish_stop(ctx,0x8c6149c4u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c6149c6; /* fmov.s fr13,@-r15 */
L8c6149c6: sa_motion_publish_stop(ctx,0x8c6149c6u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6149c8; /* sts.l PR,@-r15 */
L8c6149c8: sa_motion_publish_stop(ctx,0x8c6149c8u); r[15]+=4294967280u;goto L8c6149ca; /* add #-0x10,r15 */
L8c6149ca: sa_motion_publish_stop(ctx,0x8c6149cau); r[3]=rd(m,bus,&io_status,0x8c6149fcu,4);if(io_status)return io_status;goto L8c6149cc; /* mov.l 0x8c6149fc,r3 */
L8c6149cc: sa_motion_publish_stop(ctx,0x8c6149ccu); r[9]=rd(m,bus,&io_status,0x8c6149f8u,4);if(io_status)return io_status;goto L8c6149ce; /* mov.l 0x8c6149f8,r9 */
L8c6149ce: sa_motion_publish_stop(ctx,0x8c6149d0u); target=r[3];*pr=0x8c6149d2u;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6149d2; /* jsr @r3 */
L8c6149d2: sa_motion_publish_stop(ctx,0x8c6149d2u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6149d4; /* mov.l @(0x4,r15),r3 */
L8c6149d4: sa_motion_publish_stop(ctx,0x8c6149d4u); r[2]=0u;goto L8c6149d6; /* mov #0x0,r2 */
L8c6149d6: sa_motion_publish_stop(ctx,0x8c6149d6u); r[14]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6149d8; /* mov.l @(0x4,r15),r14 */
L8c6149d8: sa_motion_publish_stop(ctx,0x8c6149d8u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6149da; /* mov.w @(0x2,r3),r0 */
L8c6149da: sa_motion_publish_stop(ctx,0x8c6149dau); r[3]=rd(m,bus,&io_status,0x8c614a00u,4);if(io_status)return io_status;goto L8c6149dc; /* mov.l 0x8c614a00,r3 */
L8c6149dc: sa_motion_publish_stop(ctx,0x8c6149dcu); r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c6149de; /* mov.l @(0x4,r14),r14 */
L8c6149de: sa_motion_publish_stop(ctx,0x8c6149deu); r[0]=r[0]&65535u;goto L8c6149e0; /* extu.w r0,r0 */
L8c6149e0: sa_motion_publish_stop(ctx,0x8c6149e0u); wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c6149e2; /* mov.l r0,@(0xc,r15) */
L8c6149e2: sa_motion_publish_stop(ctx,0x8c6149e2u); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6149e4; /* mov.l @r3,r13 */
L8c6149e4: sa_motion_publish_stop(ctx,0x8c6149e4u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6149e6; /* mov.l r2,@(0x8,r15) */
L8c6149e6: sa_motion_publish_stop(ctx,0x8c6149e6u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6149e8; /* mov.l r2,@(0x4,r15) */
L8c6149e8: sa_motion_publish_stop(ctx,0x8c6149eau); goto L8c614a04; /* bra 0x8c614a04 */
L8c614a04: sa_motion_publish_stop(ctx,0x8c614a04u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614a06; /* mov.w @r14+,r3 */
L8c614a06: sa_motion_publish_stop(ctx,0x8c614a06u); r[4]=rd(m,bus,&io_status,0x8c614ad0u,4);if(io_status)return io_status;goto L8c614a08; /* mov.l 0x8c614ad0,r4 */
L8c614a08: sa_motion_publish_stop(ctx,0x8c614a08u); r[3]=r[3]&65535u;goto L8c614a0a; /* extu.w r3,r3 */
L8c614a0a: sa_motion_publish_stop(ctx,0x8c614a0au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c614a0c; /* fldi1 fr4 */
L8c614a0c: sa_motion_publish_stop(ctx,0x8c614a0cu); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c614a0e; /* tst r4,r3 */
L8c614a0e: sa_motion_publish_stop(ctx,0x8c614a0eu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c614a10; /* mov.l r3,@r15 */
L8c614a10: sa_motion_publish_stop(ctx,0x8c614a12u); take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c614a22;goto L8c614a14; /* bt/s 0x8c614a22 */
L8c614a14: sa_motion_publish_stop(ctx,0x8c614a14u); r[4]=sx(m,bus,&io_status,0x8c614accu,2);if(io_status)return io_status;goto L8c614a16; /* mov.w 0x8c614acc,r4 */
L8c614a16: sa_motion_publish_stop(ctx,0x8c614a16u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614a18; /* mov.l @r15,r3 */
L8c614a18: sa_motion_publish_stop(ctx,0x8c614a18u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c614a1a; /* fldi1 fr4 */
L8c614a1a: sa_motion_publish_stop(ctx,0x8c614a1au); r[3]&=r[4];goto L8c614a1c; /* and r4,r3 */
L8c614a1c: sa_motion_publish_stop(ctx,0x8c614a1cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c614a1e; /* fneg fr4 */
L8c614a1e: sa_motion_publish_stop(ctx,0x8c614a1eu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c614a20; /* mov.l r3,@r15 */
L8c614a20: sa_motion_publish_stop(ctx,0x8c614a20u); fcopy(f,15u,4u);goto L8c614a22; /* fmov fr4,fr15 */
L8c614a22: sa_motion_publish_stop(ctx,0x8c614a22u); r[0]=16u;goto L8c614a24; /* mov #0x10,r0 */
L8c614a24: sa_motion_publish_stop(ctx,0x8c614a24u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614a26; /* mov.l @r15,r3 */
L8c614a26: sa_motion_publish_stop(ctx,0x8c614a26u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614a28; /* mov.l @(0x4,r15),r2 */
L8c614a28: sa_motion_publish_stop(ctx,0x8c614a28u); r[10]=r[8];goto L8c614a2a; /* mov r8,r10 */
L8c614a2a: sa_motion_publish_stop(ctx,0x8c614a2au); r[2]+=r[3];goto L8c614a2c; /* add r3,r2 */
L8c614a2c: sa_motion_publish_stop(ctx,0x8c614a2cu); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c614a2e; /* mov.l r2,@(0x4,r15) */
L8c614a2e: sa_motion_publish_stop(ctx,0x8c614a2eu); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614a30; /* mov.l @r15,r1 */
L8c614a30: sa_motion_publish_stop(ctx,0x8c614a30u); r[1]+=4294967294u;goto L8c614a32; /* add #-0x2,r1 */
L8c614a32: sa_motion_publish_stop(ctx,0x8c614a32u); wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c614a34; /* mov.l r1,@r15 */
L8c614a34: sa_motion_publish_stop(ctx,0x8c614a34u); r[4]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614a36; /* mov.w @r14+,r4 */
L8c614a36: sa_motion_publish_stop(ctx,0x8c614a36u); r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614a38; /* mov.w @r14+,r11 */
L8c614a38: sa_motion_publish_stop(ctx,0x8c614a38u); r[4]=r[4]&65535u;goto L8c614a3a; /* extu.w r4,r4 */
L8c614a3a: sa_motion_publish_stop(ctx,0x8c614a3au); r[4]<<=2;goto L8c614a3c; /* shll2 r4 */
L8c614a3c: sa_motion_publish_stop(ctx,0x8c614a3cu); r[11]=r[11]&65535u;goto L8c614a3e; /* extu.w r11,r11 */
L8c614a3e: sa_motion_publish_stop(ctx,0x8c614a3eu); r[11]<<=2;goto L8c614a40; /* shll2 r11 */
L8c614a40: sa_motion_publish_stop(ctx,0x8c614a40u); r[4]<<=2;goto L8c614a42; /* shll2 r4 */
L8c614a42: sa_motion_publish_stop(ctx,0x8c614a42u); r[11]<<=2;goto L8c614a44; /* shll2 r11 */
L8c614a44: sa_motion_publish_stop(ctx,0x8c614a44u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c614a46; /* shll r4 */
L8c614a46: sa_motion_publish_stop(ctx,0x8c614a46u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c614a48; /* shll r11 */
L8c614a48: sa_motion_publish_stop(ctx,0x8c614a48u); r[11]+=r[13];goto L8c614a4a; /* add r13,r11 */
L8c614a4a: sa_motion_publish_stop(ctx,0x8c614a4au); r[4]+=r[13];goto L8c614a4c; /* add r13,r4 */
L8c614a4c: sa_motion_publish_stop(ctx,0x8c614a4cu); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c614a4e; /* fmov.s @(r0,r11),fr14 */
L8c614a4e: sa_motion_publish_stop(ctx,0x8c614a4eu); fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c614a50; /* fmov.s @(r0,r4),fr4 */
L8c614a50: sa_motion_publish_stop(ctx,0x8c614a50u); r[0]=20u;goto L8c614a52; /* mov #0x14,r0 */
L8c614a52: sa_motion_publish_stop(ctx,0x8c614a52u); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c614a54; /* fmov.s @(r0,r11),fr13 */
L8c614a54: sa_motion_publish_stop(ctx,0x8c614a54u); fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c614a56; /* fmov.s @(r0,r4),fr5 */
L8c614a56: sa_motion_publish_stop(ctx,0x8c614a56u); r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614a58; /* mov.w @r14+,r12 */
L8c614a58: sa_motion_publish_stop(ctx,0x8c614a58u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c614a5a; /* mov.w @r14,r3 */
L8c614a5a: sa_motion_publish_stop(ctx,0x8c614a5au); r[12]=r[12]&65535u;goto L8c614a5c; /* extu.w r12,r12 */
L8c614a5c: sa_motion_publish_stop(ctx,0x8c614a5cu); r[12]<<=2;goto L8c614a5e; /* shll2 r12 */
L8c614a5e: sa_motion_publish_stop(ctx,0x8c614a5eu); r[3]=r[3]&65535u;goto L8c614a60; /* extu.w r3,r3 */
L8c614a60: sa_motion_publish_stop(ctx,0x8c614a60u); r[3]<<=2;goto L8c614a62; /* shll2 r3 */
L8c614a62: sa_motion_publish_stop(ctx,0x8c614a62u); r[12]<<=2;goto L8c614a64; /* shll2 r12 */
L8c614a64: sa_motion_publish_stop(ctx,0x8c614a64u); r[3]<<=2;goto L8c614a66; /* shll2 r3 */
L8c614a66: sa_motion_publish_stop(ctx,0x8c614a66u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c614a68; /* shll r12 */
L8c614a68: sa_motion_publish_stop(ctx,0x8c614a68u); *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c614a6a; /* shll r3 */
L8c614a6a: sa_motion_publish_stop(ctx,0x8c614a6au); r[3]+=r[13];goto L8c614a6c; /* add r13,r3 */
L8c614a6c: sa_motion_publish_stop(ctx,0x8c614a6cu); r[12]+=r[13];goto L8c614a6e; /* add r13,r12 */
L8c614a6e: sa_motion_publish_stop(ctx,0x8c614a6eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c614a70; /* pref @r3 */
L8c614a70: sa_motion_publish_stop(ctx,0x8c614a70u); r[0]=20u;goto L8c614a72; /* mov #0x14,r0 */
L8c614a72: sa_motion_publish_stop(ctx,0x8c614a72u); fcopy(f,3u,14u);goto L8c614a74; /* fmov fr14,fr3 */
L8c614a74: sa_motion_publish_stop(ctx,0x8c614a74u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c614a76; /* fmov.s @(r0,r12),fr2 */
L8c614a76: sa_motion_publish_stop(ctx,0x8c614a76u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614a78; /* fsub fr4,fr3 */
L8c614a78: sa_motion_publish_stop(ctx,0x8c614a78u); r[0]=16u;goto L8c614a7a; /* mov #0x10,r0 */
L8c614a7a: sa_motion_publish_stop(ctx,0x8c614a7au); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c614a7c; /* cmp/hs r8,r10 */
L8c614a7c: sa_motion_publish_stop(ctx,0x8c614a7cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614a7e; /* fsub fr13,fr2 */
L8c614a7e: sa_motion_publish_stop(ctx,0x8c614a7eu); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c614a80; /* fmov.s @(r0,r12),fr1 */
L8c614a80: sa_motion_publish_stop(ctx,0x8c614a80u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c614a82; /* fsub fr14,fr1 */
L8c614a82: sa_motion_publish_stop(ctx,0x8c614a82u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614a84; /* fmul fr2,fr3 */
L8c614a84: sa_motion_publish_stop(ctx,0x8c614a84u); fcopy(f,2u,13u);goto L8c614a86; /* fmov fr13,fr2 */
L8c614a86: sa_motion_publish_stop(ctx,0x8c614a86u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614a88; /* fsub fr5,fr2 */
L8c614a88: sa_motion_publish_stop(ctx,0x8c614a88u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614a8a; /* fmul fr1,fr2 */
L8c614a8a: sa_motion_publish_stop(ctx,0x8c614a8au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614a8c; /* fsub fr2,fr3 */
L8c614a8c: sa_motion_publish_stop(ctx,0x8c614a8cu); fcopy(f,4u,3u);goto L8c614a8e; /* fmov fr3,fr4 */
L8c614a8e: sa_motion_publish_stop(ctx,0x8c614a90u); take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c614ad4;goto L8c614a92; /* bt/s 0x8c614ad4 */
L8c614a92: sa_motion_publish_stop(ctx,0x8c614a92u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614a94; /* fldi0 fr3 */
L8c614a94: sa_motion_publish_stop(ctx,0x8c614a94u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c614a96; /* fcmp/gt fr4,fr3 */
L8c614a96: sa_motion_publish_stop(ctx,0x8c614a96u); if((*sr&1u))goto L8c614afc;goto L8c614a98; /* bt 0x8c614afc */
L8c614a98: sa_motion_publish_stop(ctx,0x8c614a98u); r[4]=r[11];goto L8c614a9a; /* mov r11,r4 */
L8c614a9a: sa_motion_publish_stop(ctx,0x8c614a9au); r[5]=r[9];goto L8c614a9c; /* mov r9,r5 */
L8c614a9c: sa_motion_publish_stop(ctx,0x8c614a9cu); r[8]+=32u;goto L8c614a9e; /* add #0x20,r8 */
L8c614a9e: sa_motion_publish_stop(ctx,0x8c614a9eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614aa0; /* fschg */
L8c614aa0: sa_motion_publish_stop(ctx,0x8c614aa0u); r[4]+=16u;goto L8c614aa2; /* add #0x10,r4 */
L8c614aa2: sa_motion_publish_stop(ctx,0x8c614aa2u); fcopy(f,0u,1u);goto L8c614aa4; /* fmov fr1,fr0 */
L8c614aa4: sa_motion_publish_stop(ctx,0x8c614aa4u); fcopy(f,2u,3u);goto L8c614aa6; /* fmov fr3,fr2 */
L8c614aa6: sa_motion_publish_stop(ctx,0x8c614aa6u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614aa8; /* fmov.s @r4+,fr4 */
L8c614aa8: sa_motion_publish_stop(ctx,0x8c614aa8u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614aaa; /* fmov.s @r4,fr6 */
L8c614aaa: sa_motion_publish_stop(ctx,0x8c614aaau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614aac; /* fmov.s fr2,@-r8 */
L8c614aac: sa_motion_publish_stop(ctx,0x8c614aacu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c614aae; /* fmov.s fr0,@-r8 */
L8c614aae: sa_motion_publish_stop(ctx,0x8c614aaeu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614ab0; /* fschg */
L8c614ab0: sa_motion_publish_stop(ctx,0x8c614ab0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614ab2; /* fmov.s fr6,@-r8 */
L8c614ab2: sa_motion_publish_stop(ctx,0x8c614ab2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614ab4; /* fmov.s fr5,@-r8 */
L8c614ab4: sa_motion_publish_stop(ctx,0x8c614ab4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614ab6; /* fmov.s fr4,@-r8 */
L8c614ab6: sa_motion_publish_stop(ctx,0x8c614ab6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614ab8; /* mov.l r5,@-r8 */
L8c614ab8: sa_motion_publish_stop(ctx,0x8c614ab8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614aba; /* pref @r8 */
L8c614aba: sa_motion_publish_stop(ctx,0x8c614abau); r[8]+=32u;goto L8c614abc; /* add #0x20,r8 */
L8c614abc: sa_motion_publish_stop(ctx,0x8c614abcu); r[3]=r[8];goto L8c614abe; /* mov r8,r3 */
L8c614abe: sa_motion_publish_stop(ctx,0x8c614abeu); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614ac0; /* mov.l @(0x8,r15),r2 */
L8c614ac0: sa_motion_publish_stop(ctx,0x8c614ac0u); r[3]-=r[10];goto L8c614ac2; /* sub r10,r3 */
L8c614ac2: sa_motion_publish_stop(ctx,0x8c614ac2u); r[3]+=32u;goto L8c614ac4; /* add #0x20,r3 */
L8c614ac4: sa_motion_publish_stop(ctx,0x8c614ac4u); r[2]+=r[3];goto L8c614ac6; /* add r3,r2 */
L8c614ac6: sa_motion_publish_stop(ctx,0x8c614ac6u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c614ac8; /* mov.l r2,@(0x8,r15) */
L8c614ac8: sa_motion_publish_stop(ctx,0x8c614acau); r[10]=r[8];goto L8c614b20; /* bra 0x8c614b20 */
L8c614ad4: sa_motion_publish_stop(ctx,0x8c614ad4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614ad6; /* fldi0 fr3 */
L8c614ad6: sa_motion_publish_stop(ctx,0x8c614ad6u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c614ad8; /* fcmp/gt fr4,fr3 */
L8c614ad8: sa_motion_publish_stop(ctx,0x8c614ad8u); if(!(*sr&1u))goto L8c614b20;goto L8c614ada; /* bf 0x8c614b20 */
L8c614ada: sa_motion_publish_stop(ctx,0x8c614adau); r[5]=rd(m,bus,&io_status,0x8c614be8u,4);if(io_status)return io_status;goto L8c614adc; /* mov.l 0x8c614be8,r5 */
L8c614adc: sa_motion_publish_stop(ctx,0x8c614adcu); r[8]+=32u;goto L8c614ade; /* add #0x20,r8 */
L8c614ade: sa_motion_publish_stop(ctx,0x8c614adeu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614ae0; /* fschg */
L8c614ae0: sa_motion_publish_stop(ctx,0x8c614ae0u); r[4]+=16u;goto L8c614ae2; /* add #0x10,r4 */
L8c614ae2: sa_motion_publish_stop(ctx,0x8c614ae2u); fcopy(f,0u,1u);goto L8c614ae4; /* fmov fr1,fr0 */
L8c614ae4: sa_motion_publish_stop(ctx,0x8c614ae4u); fcopy(f,2u,3u);goto L8c614ae6; /* fmov fr3,fr2 */
L8c614ae6: sa_motion_publish_stop(ctx,0x8c614ae6u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614ae8; /* fmov.s @r4+,fr4 */
L8c614ae8: sa_motion_publish_stop(ctx,0x8c614ae8u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614aea; /* fmov.s @r4,fr6 */
L8c614aea: sa_motion_publish_stop(ctx,0x8c614aeau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614aec; /* fmov.s fr2,@-r8 */
L8c614aec: sa_motion_publish_stop(ctx,0x8c614aecu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c614aee; /* fmov.s fr0,@-r8 */
L8c614aee: sa_motion_publish_stop(ctx,0x8c614aeeu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614af0; /* fschg */
L8c614af0: sa_motion_publish_stop(ctx,0x8c614af0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614af2; /* fmov.s fr6,@-r8 */
L8c614af2: sa_motion_publish_stop(ctx,0x8c614af2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614af4; /* fmov.s fr5,@-r8 */
L8c614af4: sa_motion_publish_stop(ctx,0x8c614af4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614af6; /* fmov.s fr4,@-r8 */
L8c614af6: sa_motion_publish_stop(ctx,0x8c614af6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614af8; /* mov.l r5,@-r8 */
L8c614af8: sa_motion_publish_stop(ctx,0x8c614af8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614afa; /* pref @r8 */
L8c614afa: sa_motion_publish_stop(ctx,0x8c614afau); r[8]+=32u;goto L8c614afc; /* add #0x20,r8 */
L8c614afc: sa_motion_publish_stop(ctx,0x8c614afcu); r[5]=rd(m,bus,&io_status,0x8c614be8u,4);if(io_status)return io_status;goto L8c614afe; /* mov.l 0x8c614be8,r5 */
L8c614afe: sa_motion_publish_stop(ctx,0x8c614afeu); r[4]=r[11];goto L8c614b00; /* mov r11,r4 */
L8c614b00: sa_motion_publish_stop(ctx,0x8c614b00u); r[8]+=32u;goto L8c614b02; /* add #0x20,r8 */
L8c614b02: sa_motion_publish_stop(ctx,0x8c614b02u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614b04; /* fschg */
L8c614b04: sa_motion_publish_stop(ctx,0x8c614b04u); r[4]+=16u;goto L8c614b06; /* add #0x10,r4 */
L8c614b06: sa_motion_publish_stop(ctx,0x8c614b06u); fcopy(f,0u,1u);goto L8c614b08; /* fmov fr1,fr0 */
L8c614b08: sa_motion_publish_stop(ctx,0x8c614b08u); fcopy(f,2u,3u);goto L8c614b0a; /* fmov fr3,fr2 */
L8c614b0a: sa_motion_publish_stop(ctx,0x8c614b0au); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614b0c; /* fmov.s @r4+,fr4 */
L8c614b0c: sa_motion_publish_stop(ctx,0x8c614b0cu); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614b0e; /* fmov.s @r4,fr6 */
L8c614b0e: sa_motion_publish_stop(ctx,0x8c614b0eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614b10; /* fmov.s fr2,@-r8 */
L8c614b10: sa_motion_publish_stop(ctx,0x8c614b10u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c614b12; /* fmov.s fr0,@-r8 */
L8c614b12: sa_motion_publish_stop(ctx,0x8c614b12u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614b14; /* fschg */
L8c614b14: sa_motion_publish_stop(ctx,0x8c614b14u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614b16; /* fmov.s fr6,@-r8 */
L8c614b16: sa_motion_publish_stop(ctx,0x8c614b16u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614b18; /* fmov.s fr5,@-r8 */
L8c614b18: sa_motion_publish_stop(ctx,0x8c614b18u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614b1a; /* fmov.s fr4,@-r8 */
L8c614b1a: sa_motion_publish_stop(ctx,0x8c614b1au); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614b1c; /* mov.l r5,@-r8 */
L8c614b1c: sa_motion_publish_stop(ctx,0x8c614b1cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614b1e; /* pref @r8 */
L8c614b1e: sa_motion_publish_stop(ctx,0x8c614b1eu); r[8]+=32u;goto L8c614b20; /* add #0x20,r8 */
L8c614b20: sa_motion_publish_stop(ctx,0x8c614b20u); fcopy(f,4u,14u);goto L8c614b22; /* fmov fr14,fr4 */
L8c614b22: sa_motion_publish_stop(ctx,0x8c614b22u); r[0]=16u;goto L8c614b24; /* mov #0x10,r0 */
L8c614b24: sa_motion_publish_stop(ctx,0x8c614b24u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614b26; /* mov.l @r15,r3 */
L8c614b26: sa_motion_publish_stop(ctx,0x8c614b26u); r[4]=r[11];goto L8c614b28; /* mov r11,r4 */
L8c614b28: sa_motion_publish_stop(ctx,0x8c614b28u); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c614b2a; /* fmov.s @(r0,r12),fr14 */
L8c614b2a: sa_motion_publish_stop(ctx,0x8c614b2au); r[0]=20u;goto L8c614b2c; /* mov #0x14,r0 */
L8c614b2c: sa_motion_publish_stop(ctx,0x8c614b2cu); fcopy(f,5u,13u);goto L8c614b2e; /* fmov fr13,fr5 */
L8c614b2e: sa_motion_publish_stop(ctx,0x8c614b2eu); r[3]+=4294967295u;goto L8c614b30; /* add #-0x1,r3 */
L8c614b30: sa_motion_publish_stop(ctx,0x8c614b30u); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c614b32; /* fmov.s @(r0,r12),fr13 */
L8c614b32: sa_motion_publish_stop(ctx,0x8c614b32u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614b34; /* tst r3,r3 */
L8c614b34: sa_motion_publish_stop(ctx,0x8c614b34u); r[11]=r[12];goto L8c614b36; /* mov r12,r11 */
L8c614b36: sa_motion_publish_stop(ctx,0x8c614b36u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c614b38; /* fneg fr15 */
L8c614b38: sa_motion_publish_stop(ctx,0x8c614b3au); take=!(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c614a56;goto L8c614b3c; /* bf/s 0x8c614a56 */
L8c614b3c: sa_motion_publish_stop(ctx,0x8c614b3cu); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c614b3e; /* cmp/hs r8,r10 */
L8c614b3e: sa_motion_publish_stop(ctx,0x8c614b3eu); if((*sr&1u))goto L8c614b70;goto L8c614b40; /* bt 0x8c614b70 */
L8c614b40: sa_motion_publish_stop(ctx,0x8c614b40u); r[4]=r[12];goto L8c614b42; /* mov r12,r4 */
L8c614b42: sa_motion_publish_stop(ctx,0x8c614b42u); r[5]=r[9];goto L8c614b44; /* mov r9,r5 */
L8c614b44: sa_motion_publish_stop(ctx,0x8c614b44u); r[8]+=32u;goto L8c614b46; /* add #0x20,r8 */
L8c614b46: sa_motion_publish_stop(ctx,0x8c614b46u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614b48; /* fschg */
L8c614b48: sa_motion_publish_stop(ctx,0x8c614b48u); r[4]+=16u;goto L8c614b4a; /* add #0x10,r4 */
L8c614b4a: sa_motion_publish_stop(ctx,0x8c614b4au); fcopy(f,0u,1u);goto L8c614b4c; /* fmov fr1,fr0 */
L8c614b4c: sa_motion_publish_stop(ctx,0x8c614b4cu); fcopy(f,2u,3u);goto L8c614b4e; /* fmov fr3,fr2 */
L8c614b4e: sa_motion_publish_stop(ctx,0x8c614b4eu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614b50; /* fmov.s @r4+,fr4 */
L8c614b50: sa_motion_publish_stop(ctx,0x8c614b50u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614b52; /* fmov.s @r4,fr6 */
L8c614b52: sa_motion_publish_stop(ctx,0x8c614b52u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614b54; /* fmov.s fr2,@-r8 */
L8c614b54: sa_motion_publish_stop(ctx,0x8c614b54u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c614b56; /* fmov.s fr0,@-r8 */
L8c614b56: sa_motion_publish_stop(ctx,0x8c614b56u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614b58; /* fschg */
L8c614b58: sa_motion_publish_stop(ctx,0x8c614b58u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614b5a; /* fmov.s fr6,@-r8 */
L8c614b5a: sa_motion_publish_stop(ctx,0x8c614b5au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614b5c; /* fmov.s fr5,@-r8 */
L8c614b5c: sa_motion_publish_stop(ctx,0x8c614b5cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614b5e; /* fmov.s fr4,@-r8 */
L8c614b5e: sa_motion_publish_stop(ctx,0x8c614b5eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614b60; /* mov.l r5,@-r8 */
L8c614b60: sa_motion_publish_stop(ctx,0x8c614b60u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614b62; /* pref @r8 */
L8c614b62: sa_motion_publish_stop(ctx,0x8c614b62u); r[8]+=32u;goto L8c614b64; /* add #0x20,r8 */
L8c614b64: sa_motion_publish_stop(ctx,0x8c614b64u); r[3]=r[8];goto L8c614b66; /* mov r8,r3 */
L8c614b66: sa_motion_publish_stop(ctx,0x8c614b66u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614b68; /* mov.l @(0x8,r15),r2 */
L8c614b68: sa_motion_publish_stop(ctx,0x8c614b68u); r[3]-=r[10];goto L8c614b6a; /* sub r10,r3 */
L8c614b6a: sa_motion_publish_stop(ctx,0x8c614b6au); r[3]+=32u;goto L8c614b6c; /* add #0x20,r3 */
L8c614b6c: sa_motion_publish_stop(ctx,0x8c614b6cu); r[2]+=r[3];goto L8c614b6e; /* add r3,r2 */
L8c614b6e: sa_motion_publish_stop(ctx,0x8c614b6eu); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c614b70; /* mov.l r2,@(0x8,r15) */
L8c614b70: sa_motion_publish_stop(ctx,0x8c614b70u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614b72; /* mov.l @(0xc,r15),r3 */
L8c614b72: sa_motion_publish_stop(ctx,0x8c614b72u); r[3]+=4294967295u;goto L8c614b74; /* add #-0x1,r3 */
L8c614b74: sa_motion_publish_stop(ctx,0x8c614b74u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614b76; /* tst r3,r3 */
L8c614b76: sa_motion_publish_stop(ctx,0x8c614b78u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c614b7e;goto L8c614b7a; /* bt/s 0x8c614b7e */
L8c614b7a: sa_motion_publish_stop(ctx,0x8c614b7cu); goto L8c614a04; /* bra 0x8c614a04 */
L8c614b7e: sa_motion_publish_stop(ctx,0x8c614b7eu); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614b80; /* mov.l @(0x8,r15),r3 */
L8c614b80: sa_motion_publish_stop(ctx,0x8c614b80u); r[4]=rd(m,bus,&io_status,0x8c614becu,4);if(io_status)return io_status;goto L8c614b82; /* mov.l 0x8c614bec,r4 */
L8c614b82: sa_motion_publish_stop(ctx,0x8c614b82u); r[3]>>=2;goto L8c614b84; /* shlr2 r3 */
L8c614b84: sa_motion_publish_stop(ctx,0x8c614b84u); r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c614b86; /* mov.l @r4,r1 */
L8c614b86: sa_motion_publish_stop(ctx,0x8c614b86u); r[3]>>=2;goto L8c614b88; /* shlr2 r3 */
L8c614b88: sa_motion_publish_stop(ctx,0x8c614b88u); *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c614b8a; /* shlr r3 */
L8c614b8a: sa_motion_publish_stop(ctx,0x8c614b8au); r[1]+=r[3];goto L8c614b8c; /* add r3,r1 */
L8c614b8c: sa_motion_publish_stop(ctx,0x8c614b8cu); wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c614b8e; /* mov.l r1,@r4 */
L8c614b8e: sa_motion_publish_stop(ctx,0x8c614b8eu); r[4]=rd(m,bus,&io_status,0x8c614bf0u,4);if(io_status)return io_status;goto L8c614b90; /* mov.l 0x8c614bf0,r4 */
L8c614b90: sa_motion_publish_stop(ctx,0x8c614b90u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614b92; /* mov.l @(0x4,r15),r3 */
L8c614b92: sa_motion_publish_stop(ctx,0x8c614b92u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c614b94; /* mov.l @r4,r2 */
L8c614b94: sa_motion_publish_stop(ctx,0x8c614b94u); r[2]+=r[3];goto L8c614b96; /* add r3,r2 */
L8c614b96: sa_motion_publish_stop(ctx,0x8c614b96u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c614b98; /* mov.l r2,@r4 */
L8c614b98: sa_motion_publish_stop(ctx,0x8c614b98u); r[15]+=16u;goto L8c614b9a; /* add #0x10,r15 */
L8c614b9a: sa_motion_publish_stop(ctx,0x8c614b9au); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614b9c; /* lds.l @r15+,PR */
L8c614b9c: sa_motion_publish_stop(ctx,0x8c614b9cu); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614b9e; /* fmov.s @r15+,fr13 */
L8c614b9e: sa_motion_publish_stop(ctx,0x8c614b9eu); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614ba0; /* fmov.s @r15+,fr14 */
L8c614ba0: sa_motion_publish_stop(ctx,0x8c614ba0u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614ba2; /* fmov.s @r15+,fr15 */
L8c614ba2: sa_motion_publish_stop(ctx,0x8c614ba2u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614ba4; /* mov.l @r15+,r9 */
L8c614ba4: sa_motion_publish_stop(ctx,0x8c614ba4u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614ba6; /* mov.l @r15+,r10 */
L8c614ba6: sa_motion_publish_stop(ctx,0x8c614ba6u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614ba8; /* mov.l @r15+,r11 */
L8c614ba8: sa_motion_publish_stop(ctx,0x8c614ba8u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614baa; /* mov.l @r15+,r12 */
L8c614baa: sa_motion_publish_stop(ctx,0x8c614baau); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614bac; /* mov.l @r15+,r13 */
L8c614bac: sa_motion_publish_stop(ctx,0x8c614baeu); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_alternate_strip_clipped_8c616bc4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c616bc4;
L8c616bc4: sa_motion_publish_stop(ctx,0x8c616bc4u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c616bc6; /* mov.l r14,@-r15 */
L8c616bc6: sa_motion_publish_stop(ctx,0x8c616bc6u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c616bc8; /* mov.l r13,@-r15 */
L8c616bc8: sa_motion_publish_stop(ctx,0x8c616bc8u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c616bca; /* mov.l r12,@-r15 */
L8c616bca: sa_motion_publish_stop(ctx,0x8c616bcau); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c616bcc; /* mov.l r11,@-r15 */
L8c616bcc: sa_motion_publish_stop(ctx,0x8c616bccu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c616bce; /* mov.l r10,@-r15 */
L8c616bce: sa_motion_publish_stop(ctx,0x8c616bceu); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c616bd0; /* mov.l r9,@-r15 */
L8c616bd0: sa_motion_publish_stop(ctx,0x8c616bd0u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c616bd2; /* fmov.s fr15,@-r15 */
L8c616bd2: sa_motion_publish_stop(ctx,0x8c616bd2u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c616bd4; /* fmov.s fr14,@-r15 */
L8c616bd4: sa_motion_publish_stop(ctx,0x8c616bd4u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c616bd6; /* fmov.s fr13,@-r15 */
L8c616bd6: sa_motion_publish_stop(ctx,0x8c616bd6u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c616bd8; /* fmov.s fr12,@-r15 */
L8c616bd8: sa_motion_publish_stop(ctx,0x8c616bd8u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c616bda; /* sts.l PR,@-r15 */
L8c616bda: sa_motion_publish_stop(ctx,0x8c616bdau); r[15]+=4294967220u;goto L8c616bdc; /* add #-0x4c,r15 */
L8c616bdc: sa_motion_publish_stop(ctx,0x8c616bdcu); r[3]=rd(m,bus,&io_status,0x8c616c14u,4);if(io_status)return io_status;goto L8c616bde; /* mov.l 0x8c616c14,r3 */
L8c616bde: sa_motion_publish_stop(ctx,0x8c616bdeu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c616be0; /* fldi1 fr12 */
L8c616be0: sa_motion_publish_stop(ctx,0x8c616be2u); target=r[3];*pr=0x8c616be4u;wr(m,bus,&io_status,r[15]+0xcu,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c616be4; /* jsr @r3 */
L8c616be4: sa_motion_publish_stop(ctx,0x8c616be4u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616be6; /* mov.l @(0xc,r15),r3 */
L8c616be6: sa_motion_publish_stop(ctx,0x8c616be6u); r[2]=0u;goto L8c616be8; /* mov #0x0,r2 */
L8c616be8: sa_motion_publish_stop(ctx,0x8c616be8u); r[14]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616bea; /* mov.l @(0xc,r15),r14 */
L8c616bea: sa_motion_publish_stop(ctx,0x8c616beau); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c616bec; /* mov.w @(0x2,r3),r0 */
L8c616bec: sa_motion_publish_stop(ctx,0x8c616becu); r[3]=rd(m,bus,&io_status,0x8c616c18u,4);if(io_status)return io_status;goto L8c616bee; /* mov.l 0x8c616c18,r3 */
L8c616bee: sa_motion_publish_stop(ctx,0x8c616beeu); r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c616bf0; /* mov.l @(0x4,r14),r14 */
L8c616bf0: sa_motion_publish_stop(ctx,0x8c616bf0u); r[0]=r[0]&65535u;goto L8c616bf2; /* extu.w r0,r0 */
L8c616bf2: sa_motion_publish_stop(ctx,0x8c616bf2u); wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c616bf4; /* mov.l r0,@(0x18,r15) */
L8c616bf4: sa_motion_publish_stop(ctx,0x8c616bf4u); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c616bf6; /* mov.l @r3,r13 */
L8c616bf6: sa_motion_publish_stop(ctx,0x8c616bf6u); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c616bf8; /* mov.l r2,@(0xc,r15) */
L8c616bf8: sa_motion_publish_stop(ctx,0x8c616bfau); goto L8c616c1c; /* bra 0x8c616c1c */
L8c616c1c: sa_motion_publish_stop(ctx,0x8c616c1cu); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c616c1e; /* mov.w @r14+,r3 */
L8c616c1e: sa_motion_publish_stop(ctx,0x8c616c1eu); r[4]=rd(m,bus,&io_status,0x8c616d38u,4);if(io_status)return io_status;goto L8c616c20; /* mov.l 0x8c616d38,r4 */
L8c616c20: sa_motion_publish_stop(ctx,0x8c616c20u); r[3]=r[3]&65535u;goto L8c616c22; /* extu.w r3,r3 */
L8c616c22: sa_motion_publish_stop(ctx,0x8c616c22u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c616c24; /* tst r4,r3 */
L8c616c24: sa_motion_publish_stop(ctx,0x8c616c24u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c616c26; /* mov.l r3,@(0x8,r15) */
L8c616c26: sa_motion_publish_stop(ctx,0x8c616c28u); take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c616c38;goto L8c616c2a; /* bt/s 0x8c616c38 */
L8c616c2a: sa_motion_publish_stop(ctx,0x8c616c2au); r[4]=sx(m,bus,&io_status,0x8c616d34u,2);if(io_status)return io_status;goto L8c616c2c; /* mov.w 0x8c616d34,r4 */
L8c616c2c: sa_motion_publish_stop(ctx,0x8c616c2cu); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c616c2e; /* mov.l @(0x8,r15),r3 */
L8c616c2e: sa_motion_publish_stop(ctx,0x8c616c2eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c616c30; /* fldi1 fr4 */
L8c616c30: sa_motion_publish_stop(ctx,0x8c616c30u); r[3]&=r[4];goto L8c616c32; /* and r4,r3 */
L8c616c32: sa_motion_publish_stop(ctx,0x8c616c32u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c616c34; /* fneg fr4 */
L8c616c34: sa_motion_publish_stop(ctx,0x8c616c34u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c616c36; /* mov.l r3,@(0x8,r15) */
L8c616c36: sa_motion_publish_stop(ctx,0x8c616c36u); fcopy(f,15u,4u);goto L8c616c38; /* fmov fr4,fr15 */
L8c616c38: sa_motion_publish_stop(ctx,0x8c616c38u); r[0]=16u;goto L8c616c3a; /* mov #0x10,r0 */
L8c616c3a: sa_motion_publish_stop(ctx,0x8c616c3au); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c616c3c; /* mov.l @(0x8,r15),r3 */
L8c616c3c: sa_motion_publish_stop(ctx,0x8c616c3cu); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616c3e; /* mov.l @(0xc,r15),r2 */
L8c616c3e: sa_motion_publish_stop(ctx,0x8c616c3eu); r[9]=r[8];goto L8c616c40; /* mov r8,r9 */
L8c616c40: sa_motion_publish_stop(ctx,0x8c616c40u); r[2]+=r[3];goto L8c616c42; /* add r3,r2 */
L8c616c42: sa_motion_publish_stop(ctx,0x8c616c42u); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c616c44; /* mov.l r2,@(0xc,r15) */
L8c616c44: sa_motion_publish_stop(ctx,0x8c616c44u); r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c616c46; /* mov.l @(0x8,r15),r1 */
L8c616c46: sa_motion_publish_stop(ctx,0x8c616c46u); r[1]+=4294967294u;goto L8c616c48; /* add #-0x2,r1 */
L8c616c48: sa_motion_publish_stop(ctx,0x8c616c48u); wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c616c4a; /* mov.l r1,@(0x8,r15) */
L8c616c4a: sa_motion_publish_stop(ctx,0x8c616c4au); r[10]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c616c4c; /* mov.w @r14+,r10 */
L8c616c4c: sa_motion_publish_stop(ctx,0x8c616c4cu); r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c616c4e; /* mov.w @r14+,r11 */
L8c616c4e: sa_motion_publish_stop(ctx,0x8c616c4eu); r[10]=r[10]&65535u;goto L8c616c50; /* extu.w r10,r10 */
L8c616c50: sa_motion_publish_stop(ctx,0x8c616c50u); r[10]<<=2;goto L8c616c52; /* shll2 r10 */
L8c616c52: sa_motion_publish_stop(ctx,0x8c616c52u); r[11]=r[11]&65535u;goto L8c616c54; /* extu.w r11,r11 */
L8c616c54: sa_motion_publish_stop(ctx,0x8c616c54u); r[11]<<=2;goto L8c616c56; /* shll2 r11 */
L8c616c56: sa_motion_publish_stop(ctx,0x8c616c56u); r[10]<<=2;goto L8c616c58; /* shll2 r10 */
L8c616c58: sa_motion_publish_stop(ctx,0x8c616c58u); r[11]<<=2;goto L8c616c5a; /* shll2 r11 */
L8c616c5a: sa_motion_publish_stop(ctx,0x8c616c5au); *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c616c5c; /* shll r10 */
L8c616c5c: sa_motion_publish_stop(ctx,0x8c616c5cu); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c616c5e; /* shll r11 */
L8c616c5e: sa_motion_publish_stop(ctx,0x8c616c5eu); r[11]+=r[13];goto L8c616c60; /* add r13,r11 */
L8c616c60: sa_motion_publish_stop(ctx,0x8c616c60u); r[10]+=r[13];goto L8c616c62; /* add r13,r10 */
L8c616c62: sa_motion_publish_stop(ctx,0x8c616c62u); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c616c64; /* fmov.s @(r0,r11),fr13 */
L8c616c64: sa_motion_publish_stop(ctx,0x8c616c64u); fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c616c66; /* fmov.s @(r0,r10),fr6 */
L8c616c66: sa_motion_publish_stop(ctx,0x8c616c66u); r[0]=20u;goto L8c616c68; /* mov #0x14,r0 */
L8c616c68: sa_motion_publish_stop(ctx,0x8c616c68u); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c616c6a; /* fmov.s @(r0,r11),fr14 */
L8c616c6a: sa_motion_publish_stop(ctx,0x8c616c6au); fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c616c6c; /* fmov.s @(r0,r10),fr5 */
L8c616c6c: sa_motion_publish_stop(ctx,0x8c616c6cu); r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c616c6e; /* mov.w @r14+,r12 */
L8c616c6e: sa_motion_publish_stop(ctx,0x8c616c6eu); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c616c70; /* mov.w @r14,r3 */
L8c616c70: sa_motion_publish_stop(ctx,0x8c616c70u); r[12]=r[12]&65535u;goto L8c616c72; /* extu.w r12,r12 */
L8c616c72: sa_motion_publish_stop(ctx,0x8c616c72u); r[12]<<=2;goto L8c616c74; /* shll2 r12 */
L8c616c74: sa_motion_publish_stop(ctx,0x8c616c74u); r[3]=r[3]&65535u;goto L8c616c76; /* extu.w r3,r3 */
L8c616c76: sa_motion_publish_stop(ctx,0x8c616c76u); r[3]<<=2;goto L8c616c78; /* shll2 r3 */
L8c616c78: sa_motion_publish_stop(ctx,0x8c616c78u); r[12]<<=2;goto L8c616c7a; /* shll2 r12 */
L8c616c7a: sa_motion_publish_stop(ctx,0x8c616c7au); r[3]<<=2;goto L8c616c7c; /* shll2 r3 */
L8c616c7c: sa_motion_publish_stop(ctx,0x8c616c7cu); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c616c7e; /* shll r12 */
L8c616c7e: sa_motion_publish_stop(ctx,0x8c616c7eu); *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c616c80; /* shll r3 */
L8c616c80: sa_motion_publish_stop(ctx,0x8c616c80u); r[3]+=r[13];goto L8c616c82; /* add r13,r3 */
L8c616c82: sa_motion_publish_stop(ctx,0x8c616c82u); r[12]+=r[13];goto L8c616c84; /* add r13,r12 */
L8c616c84: sa_motion_publish_stop(ctx,0x8c616c84u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c616c86; /* pref @r3 */
L8c616c86: sa_motion_publish_stop(ctx,0x8c616c86u); r[2]=0u;goto L8c616c88; /* mov #0x0,r2 */
L8c616c88: sa_motion_publish_stop(ctx,0x8c616c88u); wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c616c8a; /* mov.l r2,@(0x14,r15) */
L8c616c8a: sa_motion_publish_stop(ctx,0x8c616c8au); r[4]=63u;goto L8c616c8c; /* mov #0x3f,r4 */
L8c616c8c: sa_motion_publish_stop(ctx,0x8c616c8cu); r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c616c8e; /* mov.l @(0xc,r10),r2 */
L8c616c8e: sa_motion_publish_stop(ctx,0x8c616c8eu); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c616c90; /* mov.l @(0xc,r11),r3 */
L8c616c90: sa_motion_publish_stop(ctx,0x8c616c90u); r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c616c92; /* mov.l @(0xc,r12),r1 */
L8c616c92: sa_motion_publish_stop(ctx,0x8c616c92u); r[2]&=r[3];goto L8c616c94; /* and r3,r2 */
L8c616c94: sa_motion_publish_stop(ctx,0x8c616c94u); r[2]&=r[1];goto L8c616c96; /* and r1,r2 */
L8c616c96: sa_motion_publish_stop(ctx,0x8c616c96u); r[4]&=r[2];goto L8c616c98; /* and r2,r4 */
L8c616c98: sa_motion_publish_stop(ctx,0x8c616c98u); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c616c9a; /* tst r4,r4 */
L8c616c9a: sa_motion_publish_stop(ctx,0x8c616c9cu); take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c616ca2;goto L8c616c9e; /* bt/s 0x8c616ca2 */
L8c616c9e: sa_motion_publish_stop(ctx,0x8c616ca0u); goto L8c616dfc; /* bra 0x8c616dfc */
L8c616ca2: sa_motion_publish_stop(ctx,0x8c616ca2u); r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c616ca4; /* mov.l @(0xc,r10),r4 */
L8c616ca4: sa_motion_publish_stop(ctx,0x8c616ca4u); r[5]=1u;goto L8c616ca6; /* mov #0x1,r5 */
L8c616ca6: sa_motion_publish_stop(ctx,0x8c616ca6u); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c616ca8; /* mov.l @(0xc,r11),r3 */
L8c616ca8: sa_motion_publish_stop(ctx,0x8c616ca8u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c616caa; /* mov.l @(0xc,r12),r2 */
L8c616caa: sa_motion_publish_stop(ctx,0x8c616caau); r[4]|=r[3];goto L8c616cac; /* or r3,r4 */
L8c616cac: sa_motion_publish_stop(ctx,0x8c616cacu); r[4]|=r[2];goto L8c616cae; /* or r2,r4 */
L8c616cae: sa_motion_publish_stop(ctx,0x8c616caeu); r[4]&=r[5];goto L8c616cb0; /* and r5,r4 */
L8c616cb0: sa_motion_publish_stop(ctx,0x8c616cb0u); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c616cb2; /* tst r4,r4 */
L8c616cb2: sa_motion_publish_stop(ctx,0x8c616cb2u); if(!(*sr&1u))goto L8c616cb8;goto L8c616cb4; /* bf 0x8c616cb8 */
L8c616cb4: sa_motion_publish_stop(ctx,0x8c616cb6u); goto L8c616dde; /* bra 0x8c616dde */
L8c616cb8: sa_motion_publish_stop(ctx,0x8c616cb8u); r[7]=r[15];goto L8c616cba; /* mov r15,r7 */
L8c616cba: sa_motion_publish_stop(ctx,0x8c616cbau); r[7]+=64u;goto L8c616cbc; /* add #0x40,r7 */
L8c616cbc: sa_motion_publish_stop(ctx,0x8c616cbcu); wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c616cbe; /* mov.l r10,@r7 */
L8c616cbe: sa_motion_publish_stop(ctx,0x8c616cbeu); wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c616cc0; /* mov.l r11,@(0x4,r7) */
L8c616cc0: sa_motion_publish_stop(ctx,0x8c616cc0u); wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c616cc2; /* mov.l r12,@(0x8,r7) */
L8c616cc2: sa_motion_publish_stop(ctx,0x8c616cc4u); r[6]=0u;goto L8c616d70; /* bra 0x8c616d70 */
L8c616cc6: sa_motion_publish_stop(ctx,0x8c616cc6u); r[2]=r[6];goto L8c616cc8; /* mov r6,r2 */
L8c616cc8: sa_motion_publish_stop(ctx,0x8c616cc8u); r[1]=r[6];goto L8c616cca; /* mov r6,r1 */
L8c616cca: sa_motion_publish_stop(ctx,0x8c616ccau); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c616ccc; /* shll r2 */
L8c616ccc: sa_motion_publish_stop(ctx,0x8c616cccu); r[3]=r[6];goto L8c616cce; /* mov r6,r3 */
L8c616cce: sa_motion_publish_stop(ctx,0x8c616cceu); r[3]<<=2;goto L8c616cd0; /* shll2 r3 */
L8c616cd0: sa_motion_publish_stop(ctx,0x8c616cd0u); r[2]+=r[1];goto L8c616cd2; /* add r1,r2 */
L8c616cd2: sa_motion_publish_stop(ctx,0x8c616cd2u); r[2]<<=2;goto L8c616cd4; /* shll2 r2 */
L8c616cd4: sa_motion_publish_stop(ctx,0x8c616cd4u); r[3]+=r[7];goto L8c616cd6; /* add r7,r3 */
L8c616cd6: sa_motion_publish_stop(ctx,0x8c616cd6u); r[2]+=r[4];goto L8c616cd8; /* add r4,r2 */
L8c616cd8: sa_motion_publish_stop(ctx,0x8c616cd8u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c616cda; /* mov.l r3,@(0x4,r15) */
L8c616cda: sa_motion_publish_stop(ctx,0x8c616cdau); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c616cdc; /* mov.l r2,@r15 */
L8c616cdc: sa_motion_publish_stop(ctx,0x8c616cdcu); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c616cde; /* mov.l @r3,r1 */
L8c616cde: sa_motion_publish_stop(ctx,0x8c616cdeu); r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c616ce0; /* mov.l @(0xc,r1),r2 */
L8c616ce0: sa_motion_publish_stop(ctx,0x8c616ce0u); *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c616ce2; /* tst r5,r2 */
L8c616ce2: sa_motion_publish_stop(ctx,0x8c616ce4u); take=!(*sr&1u);r[6]+=1u;if(take)goto L8c616d48;goto L8c616ce6; /* bf/s 0x8c616d48 */
L8c616ce6: sa_motion_publish_stop(ctx,0x8c616ce6u); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616ce8; /* mov.l @(0x4,r15),r1 */
L8c616ce8: sa_motion_publish_stop(ctx,0x8c616ce8u); r[0]=24u;goto L8c616cea; /* mov #0x18,r0 */
L8c616cea: sa_motion_publish_stop(ctx,0x8c616ceau); fcopy(f,4u,12u);goto L8c616cec; /* fmov fr12,fr4 */
L8c616cec: sa_motion_publish_stop(ctx,0x8c616cecu); r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c616cee; /* mov.l @r1,r3 */
L8c616cee: sa_motion_publish_stop(ctx,0x8c616ceeu); r[4]=rd(m,bus,&io_status,0x8c616d3cu,4);if(io_status)return io_status;goto L8c616cf0; /* mov.l 0x8c616d3c,r4 */
L8c616cf0: sa_motion_publish_stop(ctx,0x8c616cf0u); wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c616cf2; /* mov.l r3,@(0x10,r15) */
L8c616cf2: sa_motion_publish_stop(ctx,0x8c616cf2u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616cf4; /* fmov.s @(r0,r3),fr3 */
L8c616cf4: sa_motion_publish_stop(ctx,0x8c616cf4u); r[0]=12u;goto L8c616cf6; /* mov #0xc,r0 */
L8c616cf6: sa_motion_publish_stop(ctx,0x8c616cf6u); r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c616cf8; /* mov.l @(0x10,r15),r2 */
L8c616cf8: sa_motion_publish_stop(ctx,0x8c616cf8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616cfa; /* fdiv fr3,fr4 */
L8c616cfa: sa_motion_publish_stop(ctx,0x8c616cfau); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c616cfc; /* fmov.s @(r0,r4),fr3 */
L8c616cfc: sa_motion_publish_stop(ctx,0x8c616cfcu); r[0]=16u;goto L8c616cfe; /* mov #0x10,r0 */
L8c616cfe: sa_motion_publish_stop(ctx,0x8c616cfeu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616d00; /* mov.l @r15,r3 */
L8c616d00: sa_motion_publish_stop(ctx,0x8c616d00u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c616d02; /* fmov.s @(r0,r2),fr2 */
L8c616d02: sa_motion_publish_stop(ctx,0x8c616d02u); r[0]=16u;goto L8c616d04; /* mov #0x10,r0 */
L8c616d04: sa_motion_publish_stop(ctx,0x8c616d04u); r[2]=rd(m,bus,&io_status,0x8c616d40u,4);if(io_status)return io_status;goto L8c616d06; /* mov.l 0x8c616d40,r2 */
L8c616d06: sa_motion_publish_stop(ctx,0x8c616d06u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616d08; /* fsub fr3,fr2 */
L8c616d08: sa_motion_publish_stop(ctx,0x8c616d08u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c616d0a; /* fmov.s @r2,fr3 */
L8c616d0a: sa_motion_publish_stop(ctx,0x8c616d0au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616d0c; /* fmul fr4,fr2 */
L8c616d0c: sa_motion_publish_stop(ctx,0x8c616d0cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616d0e; /* fdiv fr3,fr2 */
L8c616d0e: sa_motion_publish_stop(ctx,0x8c616d0eu); fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c616d10; /* fmov.s fr2,@r3 */
L8c616d10: sa_motion_publish_stop(ctx,0x8c616d10u); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616d12; /* mov.l @(0x4,r15),r1 */
L8c616d12: sa_motion_publish_stop(ctx,0x8c616d12u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c616d14; /* fmov.s @(r0,r4),fr3 */
L8c616d14: sa_motion_publish_stop(ctx,0x8c616d14u); r[0]=20u;goto L8c616d16; /* mov #0x14,r0 */
L8c616d16: sa_motion_publish_stop(ctx,0x8c616d16u); r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c616d18; /* mov.l @r1,r1 */
L8c616d18: sa_motion_publish_stop(ctx,0x8c616d18u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616d1a; /* mov.l @r15,r3 */
L8c616d1a: sa_motion_publish_stop(ctx,0x8c616d1au); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c616d1c; /* fmov.s @(r0,r1),fr2 */
L8c616d1c: sa_motion_publish_stop(ctx,0x8c616d1cu); r[1]=rd(m,bus,&io_status,0x8c616d44u,4);if(io_status)return io_status;goto L8c616d1e; /* mov.l 0x8c616d44,r1 */
L8c616d1e: sa_motion_publish_stop(ctx,0x8c616d1eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616d20; /* fsub fr3,fr2 */
L8c616d20: sa_motion_publish_stop(ctx,0x8c616d20u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c616d22; /* fmov.s @r1,fr3 */
L8c616d22: sa_motion_publish_stop(ctx,0x8c616d22u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616d24; /* fmul fr4,fr2 */
L8c616d24: sa_motion_publish_stop(ctx,0x8c616d24u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616d26; /* fdiv fr3,fr2 */
L8c616d26: sa_motion_publish_stop(ctx,0x8c616d26u); r[0]=4u;goto L8c616d28; /* mov #0x4,r0 */
L8c616d28: sa_motion_publish_stop(ctx,0x8c616d28u); fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c616d2a; /* fmov fr2,@(r0,r3) */
L8c616d2a: sa_motion_publish_stop(ctx,0x8c616d2au); r[0]=8u;goto L8c616d2c; /* mov #0x8,r0 */
L8c616d2c: sa_motion_publish_stop(ctx,0x8c616d2cu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616d2e; /* mov.l @r15,r3 */
L8c616d2e: sa_motion_publish_stop(ctx,0x8c616d2eu); fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c616d30; /* fmov fr4,@(r0,r3) */
L8c616d30: sa_motion_publish_stop(ctx,0x8c616d32u); goto L8c616d70; /* bra 0x8c616d70 */
L8c616d48: sa_motion_publish_stop(ctx,0x8c616d48u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616d4a; /* mov.l @(0x4,r15),r3 */
L8c616d4a: sa_motion_publish_stop(ctx,0x8c616d4au); r[0]=16u;goto L8c616d4c; /* mov #0x10,r0 */
L8c616d4c: sa_motion_publish_stop(ctx,0x8c616d4cu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616d4e; /* mov.l @r15,r2 */
L8c616d4e: sa_motion_publish_stop(ctx,0x8c616d4eu); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c616d50; /* mov.l @r3,r1 */
L8c616d50: sa_motion_publish_stop(ctx,0x8c616d50u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c616d52; /* fmov.s @(r0,r1),fr3 */
L8c616d52: sa_motion_publish_stop(ctx,0x8c616d52u); r[0]=20u;goto L8c616d54; /* mov #0x14,r0 */
L8c616d54: sa_motion_publish_stop(ctx,0x8c616d54u); fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c616d56; /* fmov.s fr3,@r2 */
L8c616d56: sa_motion_publish_stop(ctx,0x8c616d56u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616d58; /* mov.l @(0x4,r15),r2 */
L8c616d58: sa_motion_publish_stop(ctx,0x8c616d58u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616d5a; /* mov.l @r15,r3 */
L8c616d5a: sa_motion_publish_stop(ctx,0x8c616d5au); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c616d5c; /* mov.l @r2,r1 */
L8c616d5c: sa_motion_publish_stop(ctx,0x8c616d5cu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c616d5e; /* fmov.s @(r0,r1),fr3 */
L8c616d5e: sa_motion_publish_stop(ctx,0x8c616d5eu); r[0]=4u;goto L8c616d60; /* mov #0x4,r0 */
L8c616d60: sa_motion_publish_stop(ctx,0x8c616d60u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616d62; /* fmov fr3,@(r0,r3) */
L8c616d62: sa_motion_publish_stop(ctx,0x8c616d62u); r[0]=24u;goto L8c616d64; /* mov #0x18,r0 */
L8c616d64: sa_motion_publish_stop(ctx,0x8c616d64u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616d66; /* mov.l @(0x4,r15),r2 */
L8c616d66: sa_motion_publish_stop(ctx,0x8c616d66u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616d68; /* mov.l @r15,r3 */
L8c616d68: sa_motion_publish_stop(ctx,0x8c616d68u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c616d6a; /* mov.l @r2,r1 */
L8c616d6a: sa_motion_publish_stop(ctx,0x8c616d6au); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c616d6c; /* fmov.s @(r0,r1),fr3 */
L8c616d6c: sa_motion_publish_stop(ctx,0x8c616d6cu); r[0]=8u;goto L8c616d6e; /* mov #0x8,r0 */
L8c616d6e: sa_motion_publish_stop(ctx,0x8c616d6eu); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616d70; /* fmov fr3,@(r0,r3) */
L8c616d70: sa_motion_publish_stop(ctx,0x8c616d70u); r[3]=3u;goto L8c616d72; /* mov #0x3,r3 */
L8c616d72: sa_motion_publish_stop(ctx,0x8c616d72u); r[4]=r[15];goto L8c616d74; /* mov r15,r4 */
L8c616d74: sa_motion_publish_stop(ctx,0x8c616d74u); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c616d76; /* cmp/ge r3,r6 */
L8c616d76: sa_motion_publish_stop(ctx,0x8c616d78u); take=!(*sr&1u);r[4]+=28u;if(take)goto L8c616cc6;goto L8c616d7a; /* bf/s 0x8c616cc6 */
L8c616d7a: sa_motion_publish_stop(ctx,0x8c616d7au); r[6]=r[4];goto L8c616d7c; /* mov r4,r6 */
L8c616d7c: sa_motion_publish_stop(ctx,0x8c616d7cu); r[0]=8u;goto L8c616d7e; /* mov #0x8,r0 */
L8c616d7e: sa_motion_publish_stop(ctx,0x8c616d7eu); r[3]=r[4];goto L8c616d80; /* mov r4,r3 */
L8c616d80: sa_motion_publish_stop(ctx,0x8c616d80u); fload(m,bus,&io_status,f,6u,r[0]+r[6]);if(io_status)return io_status;goto L8c616d82; /* fmov.s @(r0,r6),fr6 */
L8c616d82: sa_motion_publish_stop(ctx,0x8c616d82u); r[3]+=12u;goto L8c616d84; /* add #0xc,r3 */
L8c616d84: sa_motion_publish_stop(ctx,0x8c616d84u); r[7]=r[4];goto L8c616d86; /* mov r4,r7 */
L8c616d86: sa_motion_publish_stop(ctx,0x8c616d86u); r[0]=4u;goto L8c616d88; /* mov #0x4,r0 */
L8c616d88: sa_motion_publish_stop(ctx,0x8c616d88u); fcopy(f,0u,6u);goto L8c616d8a; /* fmov fr6,fr0 */
L8c616d8a: sa_motion_publish_stop(ctx,0x8c616d8au); r[7]+=24u;goto L8c616d8c; /* add #0x18,r7 */
L8c616d8c: sa_motion_publish_stop(ctx,0x8c616d8cu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c616d8e; /* mov.l r3,@r15 */
L8c616d8e: sa_motion_publish_stop(ctx,0x8c616d8eu); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c616d90; /* fmov.s @(r0,r6),fr4 */
L8c616d90: sa_motion_publish_stop(ctx,0x8c616d90u); fload(m,bus,&io_status,f,5u,r[0]+r[3]);if(io_status)return io_status;goto L8c616d92; /* fmov.s @(r0,r3),fr5 */
L8c616d92: sa_motion_publish_stop(ctx,0x8c616d92u); r[0]=8u;goto L8c616d94; /* mov #0x8,r0 */
L8c616d94: sa_motion_publish_stop(ctx,0x8c616d94u); fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c616d96; /* fmov.s @(r0,r3),fr9 */
L8c616d96: sa_motion_publish_stop(ctx,0x8c616d96u); r[0]=4u;goto L8c616d98; /* mov #0x4,r0 */
L8c616d98: sa_motion_publish_stop(ctx,0x8c616d98u); fcopy(f,3u,4u);goto L8c616d9a; /* fmov fr4,fr3 */
L8c616d9a: sa_motion_publish_stop(ctx,0x8c616d9au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c616d9c; /* fmul fr5,fr0 */
L8c616d9c: sa_motion_publish_stop(ctx,0x8c616d9cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616d9e; /* fmul fr9,fr3 */
L8c616d9e: sa_motion_publish_stop(ctx,0x8c616d9eu); fload(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;goto L8c616da0; /* fmov.s @r6,fr8 */
L8c616da0: sa_motion_publish_stop(ctx,0x8c616da0u); fload(m,bus,&io_status,f,7u,r[3]);if(io_status)return io_status;goto L8c616da2; /* fmov.s @r3,fr7 */
L8c616da2: sa_motion_publish_stop(ctx,0x8c616da2u); fcopy(f,2u,8u);goto L8c616da4; /* fmov fr8,fr2 */
L8c616da4: sa_motion_publish_stop(ctx,0x8c616da4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[9],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616da6; /* fmul fr9,fr2 */
L8c616da6: sa_motion_publish_stop(ctx,0x8c616da6u); fcopy(f,1u,4u);goto L8c616da8; /* fmov fr4,fr1 */
L8c616da8: sa_motion_publish_stop(ctx,0x8c616da8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c616daa; /* fsub fr3,fr0 */
L8c616daa: sa_motion_publish_stop(ctx,0x8c616daau); fcopy(f,3u,6u);goto L8c616dac; /* fmov fr6,fr3 */
L8c616dac: sa_motion_publish_stop(ctx,0x8c616dacu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616dae; /* fmul fr7,fr3 */
L8c616dae: sa_motion_publish_stop(ctx,0x8c616daeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616db0; /* fsub fr3,fr2 */
L8c616db0: sa_motion_publish_stop(ctx,0x8c616db0u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c616db2; /* fmov.s @(r0,r7),fr3 */
L8c616db2: sa_motion_publish_stop(ctx,0x8c616db2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616db4; /* fmul fr2,fr3 */
L8c616db4: sa_motion_publish_stop(ctx,0x8c616db4u); fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c616db6; /* fmov.s @r7,fr2 */
L8c616db6: sa_motion_publish_stop(ctx,0x8c616db6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616db8; /* fmac fr0,fr2,fr3 */
L8c616db8: sa_motion_publish_stop(ctx,0x8c616db8u); fcopy(f,2u,3u);goto L8c616dba; /* fmov fr3,fr2 */
L8c616dba: sa_motion_publish_stop(ctx,0x8c616dbau); fcopy(f,3u,8u);goto L8c616dbc; /* fmov fr8,fr3 */
L8c616dbc: sa_motion_publish_stop(ctx,0x8c616dbcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616dbe; /* fmul fr5,fr3 */
L8c616dbe: sa_motion_publish_stop(ctx,0x8c616dbeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c616dc0; /* fmul fr7,fr1 */
L8c616dc0: sa_motion_publish_stop(ctx,0x8c616dc0u); r[0]=8u;goto L8c616dc2; /* mov #0x8,r0 */
L8c616dc2: sa_motion_publish_stop(ctx,0x8c616dc2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c616dc4; /* fsub fr3,fr1 */
L8c616dc4: sa_motion_publish_stop(ctx,0x8c616dc4u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c616dc6; /* fmov.s @(r0,r7),fr3 */
L8c616dc6: sa_motion_publish_stop(ctx,0x8c616dc6u); fcopy(f,0u,1u);goto L8c616dc8; /* fmov fr1,fr0 */
L8c616dc8: sa_motion_publish_stop(ctx,0x8c616dc8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616dca; /* fmac fr0,fr3,fr2 */
L8c616dca: sa_motion_publish_stop(ctx,0x8c616dcau); fcopy(f,3u,15u);goto L8c616dcc; /* fmov fr15,fr3 */
L8c616dcc: sa_motion_publish_stop(ctx,0x8c616dccu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c616dce; /* fneg fr3 */
L8c616dce: sa_motion_publish_stop(ctx,0x8c616dceu); fcopy(f,4u,2u);goto L8c616dd0; /* fmov fr2,fr4 */
L8c616dd0: sa_motion_publish_stop(ctx,0x8c616dd0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616dd2; /* fmul fr3,fr4 */
L8c616dd2: sa_motion_publish_stop(ctx,0x8c616dd2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c616dd4; /* fldi0 fr2 */
L8c616dd4: sa_motion_publish_stop(ctx,0x8c616dd4u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c616dd6; /* fcmp/gt fr2,fr4 */
L8c616dd6: sa_motion_publish_stop(ctx,0x8c616dd6u); if((*sr&1u))goto L8c616dfc;goto L8c616dd8; /* bt 0x8c616dfc */
L8c616dd8: sa_motion_publish_stop(ctx,0x8c616dd8u); wr(m,bus,&io_status,r[15]+0x14u,r[5],4);if(io_status)return io_status;goto L8c616dda; /* mov.l r5,@(0x14,r15) */
L8c616dda: sa_motion_publish_stop(ctx,0x8c616ddcu); fcopy(f,4u,12u);goto L8c616dfc; /* bra 0x8c616dfc */
L8c616dde: sa_motion_publish_stop(ctx,0x8c616ddeu); r[0]=20u;goto L8c616de0; /* mov #0x14,r0 */
L8c616de0: sa_motion_publish_stop(ctx,0x8c616de0u); fcopy(f,3u,13u);goto L8c616de2; /* fmov fr13,fr3 */
L8c616de2: sa_motion_publish_stop(ctx,0x8c616de2u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c616de4; /* fmov.s @(r0,r12),fr2 */
L8c616de4: sa_motion_publish_stop(ctx,0x8c616de4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c616de6; /* fsub fr6,fr3 */
L8c616de6: sa_motion_publish_stop(ctx,0x8c616de6u); r[0]=16u;goto L8c616de8; /* mov #0x10,r0 */
L8c616de8: sa_motion_publish_stop(ctx,0x8c616de8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616dea; /* fsub fr14,fr2 */
L8c616dea: sa_motion_publish_stop(ctx,0x8c616deau); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c616dec; /* fmov.s @(r0,r12),fr1 */
L8c616dec: sa_motion_publish_stop(ctx,0x8c616decu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c616dee; /* fsub fr13,fr1 */
L8c616dee: sa_motion_publish_stop(ctx,0x8c616deeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616df0; /* fmul fr2,fr3 */
L8c616df0: sa_motion_publish_stop(ctx,0x8c616df0u); fcopy(f,2u,14u);goto L8c616df2; /* fmov fr14,fr2 */
L8c616df2: sa_motion_publish_stop(ctx,0x8c616df2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616df4; /* fsub fr5,fr2 */
L8c616df4: sa_motion_publish_stop(ctx,0x8c616df4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616df6; /* fmul fr1,fr2 */
L8c616df6: sa_motion_publish_stop(ctx,0x8c616df6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c616df8; /* fsub fr2,fr3 */
L8c616df8: sa_motion_publish_stop(ctx,0x8c616df8u); fcopy(f,4u,3u);goto L8c616dfa; /* fmov fr3,fr4 */
L8c616dfa: sa_motion_publish_stop(ctx,0x8c616dfau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616dfc; /* fmul fr15,fr4 */
L8c616dfc: sa_motion_publish_stop(ctx,0x8c616dfcu); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c616dfe; /* cmp/hs r8,r9 */
L8c616dfe: sa_motion_publish_stop(ctx,0x8c616dfeu); if((*sr&1u))goto L8c616e2e;goto L8c616e00; /* bt 0x8c616e2e */
L8c616e00: sa_motion_publish_stop(ctx,0x8c616e00u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c616e02; /* fldi0 fr3 */
L8c616e02: sa_motion_publish_stop(ctx,0x8c616e02u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c616e04; /* fcmp/gt fr4,fr3 */
L8c616e04: sa_motion_publish_stop(ctx,0x8c616e04u); if((*sr&1u))goto L8c616e58;goto L8c616e06; /* bt 0x8c616e58 */
L8c616e06: sa_motion_publish_stop(ctx,0x8c616e06u); r[5]=rd(m,bus,&io_status,0x8c616facu,4);if(io_status)return io_status;goto L8c616e08; /* mov.l 0x8c616fac,r5 */
L8c616e08: sa_motion_publish_stop(ctx,0x8c616e08u); r[4]=r[11];goto L8c616e0a; /* mov r11,r4 */
L8c616e0a: sa_motion_publish_stop(ctx,0x8c616e0au); r[8]+=32u;goto L8c616e0c; /* add #0x20,r8 */
L8c616e0c: sa_motion_publish_stop(ctx,0x8c616e0cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616e0e; /* fschg */
L8c616e0e: sa_motion_publish_stop(ctx,0x8c616e0eu); r[4]+=16u;goto L8c616e10; /* add #0x10,r4 */
L8c616e10: sa_motion_publish_stop(ctx,0x8c616e10u); fcopy(f,0u,1u);goto L8c616e12; /* fmov fr1,fr0 */
L8c616e12: sa_motion_publish_stop(ctx,0x8c616e12u); fcopy(f,2u,3u);goto L8c616e14; /* fmov fr3,fr2 */
L8c616e14: sa_motion_publish_stop(ctx,0x8c616e14u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616e16; /* fmov.s @r4+,fr4 */
L8c616e16: sa_motion_publish_stop(ctx,0x8c616e16u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616e18; /* fmov.s @r4,fr6 */
L8c616e18: sa_motion_publish_stop(ctx,0x8c616e18u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616e1a; /* fmov.s fr2,@-r8 */
L8c616e1a: sa_motion_publish_stop(ctx,0x8c616e1au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616e1c; /* fmov.s fr0,@-r8 */
L8c616e1c: sa_motion_publish_stop(ctx,0x8c616e1cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616e1e; /* fschg */
L8c616e1e: sa_motion_publish_stop(ctx,0x8c616e1eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616e20; /* fmov.s fr6,@-r8 */
L8c616e20: sa_motion_publish_stop(ctx,0x8c616e20u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616e22; /* fmov.s fr5,@-r8 */
L8c616e22: sa_motion_publish_stop(ctx,0x8c616e22u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616e24; /* fmov.s fr4,@-r8 */
L8c616e24: sa_motion_publish_stop(ctx,0x8c616e24u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616e26; /* mov.l r5,@-r8 */
L8c616e26: sa_motion_publish_stop(ctx,0x8c616e26u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616e28; /* pref @r8 */
L8c616e28: sa_motion_publish_stop(ctx,0x8c616e28u); r[8]+=32u;goto L8c616e2a; /* add #0x20,r8 */
L8c616e2a: sa_motion_publish_stop(ctx,0x8c616e2cu); r[9]=r[8];goto L8c616e7c; /* bra 0x8c616e7c */
L8c616e2e: sa_motion_publish_stop(ctx,0x8c616e2eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c616e30; /* fldi0 fr3 */
L8c616e30: sa_motion_publish_stop(ctx,0x8c616e30u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c616e32; /* fcmp/gt fr4,fr3 */
L8c616e32: sa_motion_publish_stop(ctx,0x8c616e32u); if(!(*sr&1u))goto L8c616e7c;goto L8c616e34; /* bf 0x8c616e7c */
L8c616e34: sa_motion_publish_stop(ctx,0x8c616e34u); r[5]=rd(m,bus,&io_status,0x8c616fb0u,4);if(io_status)return io_status;goto L8c616e36; /* mov.l 0x8c616fb0,r5 */
L8c616e36: sa_motion_publish_stop(ctx,0x8c616e36u); r[4]=r[10];goto L8c616e38; /* mov r10,r4 */
L8c616e38: sa_motion_publish_stop(ctx,0x8c616e38u); r[8]+=32u;goto L8c616e3a; /* add #0x20,r8 */
L8c616e3a: sa_motion_publish_stop(ctx,0x8c616e3au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616e3c; /* fschg */
L8c616e3c: sa_motion_publish_stop(ctx,0x8c616e3cu); r[4]+=16u;goto L8c616e3e; /* add #0x10,r4 */
L8c616e3e: sa_motion_publish_stop(ctx,0x8c616e3eu); fcopy(f,0u,1u);goto L8c616e40; /* fmov fr1,fr0 */
L8c616e40: sa_motion_publish_stop(ctx,0x8c616e40u); fcopy(f,2u,3u);goto L8c616e42; /* fmov fr3,fr2 */
L8c616e42: sa_motion_publish_stop(ctx,0x8c616e42u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616e44; /* fmov.s @r4+,fr4 */
L8c616e44: sa_motion_publish_stop(ctx,0x8c616e44u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616e46; /* fmov.s @r4,fr6 */
L8c616e46: sa_motion_publish_stop(ctx,0x8c616e46u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616e48; /* fmov.s fr2,@-r8 */
L8c616e48: sa_motion_publish_stop(ctx,0x8c616e48u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616e4a; /* fmov.s fr0,@-r8 */
L8c616e4a: sa_motion_publish_stop(ctx,0x8c616e4au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616e4c; /* fschg */
L8c616e4c: sa_motion_publish_stop(ctx,0x8c616e4cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616e4e; /* fmov.s fr6,@-r8 */
L8c616e4e: sa_motion_publish_stop(ctx,0x8c616e4eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616e50; /* fmov.s fr5,@-r8 */
L8c616e50: sa_motion_publish_stop(ctx,0x8c616e50u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616e52; /* fmov.s fr4,@-r8 */
L8c616e52: sa_motion_publish_stop(ctx,0x8c616e52u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616e54; /* mov.l r5,@-r8 */
L8c616e54: sa_motion_publish_stop(ctx,0x8c616e54u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616e56; /* pref @r8 */
L8c616e56: sa_motion_publish_stop(ctx,0x8c616e56u); r[8]+=32u;goto L8c616e58; /* add #0x20,r8 */
L8c616e58: sa_motion_publish_stop(ctx,0x8c616e58u); r[5]=rd(m,bus,&io_status,0x8c616fb0u,4);if(io_status)return io_status;goto L8c616e5a; /* mov.l 0x8c616fb0,r5 */
L8c616e5a: sa_motion_publish_stop(ctx,0x8c616e5au); r[4]=r[11];goto L8c616e5c; /* mov r11,r4 */
L8c616e5c: sa_motion_publish_stop(ctx,0x8c616e5cu); r[8]+=32u;goto L8c616e5e; /* add #0x20,r8 */
L8c616e5e: sa_motion_publish_stop(ctx,0x8c616e5eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616e60; /* fschg */
L8c616e60: sa_motion_publish_stop(ctx,0x8c616e60u); r[4]+=16u;goto L8c616e62; /* add #0x10,r4 */
L8c616e62: sa_motion_publish_stop(ctx,0x8c616e62u); fcopy(f,0u,1u);goto L8c616e64; /* fmov fr1,fr0 */
L8c616e64: sa_motion_publish_stop(ctx,0x8c616e64u); fcopy(f,2u,3u);goto L8c616e66; /* fmov fr3,fr2 */
L8c616e66: sa_motion_publish_stop(ctx,0x8c616e66u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616e68; /* fmov.s @r4+,fr4 */
L8c616e68: sa_motion_publish_stop(ctx,0x8c616e68u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616e6a; /* fmov.s @r4,fr6 */
L8c616e6a: sa_motion_publish_stop(ctx,0x8c616e6au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616e6c; /* fmov.s fr2,@-r8 */
L8c616e6c: sa_motion_publish_stop(ctx,0x8c616e6cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616e6e; /* fmov.s fr0,@-r8 */
L8c616e6e: sa_motion_publish_stop(ctx,0x8c616e6eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616e70; /* fschg */
L8c616e70: sa_motion_publish_stop(ctx,0x8c616e70u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616e72; /* fmov.s fr6,@-r8 */
L8c616e72: sa_motion_publish_stop(ctx,0x8c616e72u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616e74; /* fmov.s fr5,@-r8 */
L8c616e74: sa_motion_publish_stop(ctx,0x8c616e74u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616e76; /* fmov.s fr4,@-r8 */
L8c616e76: sa_motion_publish_stop(ctx,0x8c616e76u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616e78; /* mov.l r5,@-r8 */
L8c616e78: sa_motion_publish_stop(ctx,0x8c616e78u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616e7a; /* pref @r8 */
L8c616e7a: sa_motion_publish_stop(ctx,0x8c616e7au); r[8]+=32u;goto L8c616e7c; /* add #0x20,r8 */
L8c616e7c: sa_motion_publish_stop(ctx,0x8c616e7cu); r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c616e7e; /* mov.l @(0x14,r15),r2 */
L8c616e7e: sa_motion_publish_stop(ctx,0x8c616e7eu); *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c616e80; /* tst r2,r2 */
L8c616e80: sa_motion_publish_stop(ctx,0x8c616e80u); if((*sr&1u))goto L8c616e94;goto L8c616e82; /* bt 0x8c616e94 */
L8c616e82: sa_motion_publish_stop(ctx,0x8c616e82u); r[4]=rd(m,bus,&io_status,0x8c616fb4u,4);if(io_status)return io_status;goto L8c616e84; /* mov.l 0x8c616fb4,r4 */
L8c616e84: sa_motion_publish_stop(ctx,0x8c616e84u); r[3]=0u;goto L8c616e86; /* mov #0x0,r3 */
L8c616e86: sa_motion_publish_stop(ctx,0x8c616e86u); r[2]=rd(m,bus,&io_status,0x8c616fb8u,4);if(io_status)return io_status;goto L8c616e88; /* mov.l 0x8c616fb8,r2 */
L8c616e88: sa_motion_publish_stop(ctx,0x8c616e88u); r[5]=r[11];goto L8c616e8a; /* mov r11,r5 */
L8c616e8a: sa_motion_publish_stop(ctx,0x8c616e8au); wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c616e8c; /* mov.l r3,@r4 */
L8c616e8c: sa_motion_publish_stop(ctx,0x8c616e8cu); r[6]=r[12];goto L8c616e8e; /* mov r12,r6 */
L8c616e8e: sa_motion_publish_stop(ctx,0x8c616e90u); target=r[2];*pr=0x8c616e92u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c616e92; /* jsr @r2 */
L8c616e92: sa_motion_publish_stop(ctx,0x8c616e92u); r[9]=r[8];goto L8c616e94; /* mov r8,r9 */
L8c616e94: sa_motion_publish_stop(ctx,0x8c616e94u); fcopy(f,6u,13u);goto L8c616e96; /* fmov fr13,fr6 */
L8c616e96: sa_motion_publish_stop(ctx,0x8c616e96u); r[0]=16u;goto L8c616e98; /* mov #0x10,r0 */
L8c616e98: sa_motion_publish_stop(ctx,0x8c616e98u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c616e9a; /* mov.l @(0x8,r15),r3 */
L8c616e9a: sa_motion_publish_stop(ctx,0x8c616e9au); r[10]=r[11];goto L8c616e9c; /* mov r11,r10 */
L8c616e9c: sa_motion_publish_stop(ctx,0x8c616e9cu); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c616e9e; /* fmov.s @(r0,r12),fr13 */
L8c616e9e: sa_motion_publish_stop(ctx,0x8c616e9eu); r[0]=20u;goto L8c616ea0; /* mov #0x14,r0 */
L8c616ea0: sa_motion_publish_stop(ctx,0x8c616ea0u); fcopy(f,5u,14u);goto L8c616ea2; /* fmov fr14,fr5 */
L8c616ea2: sa_motion_publish_stop(ctx,0x8c616ea2u); r[3]+=4294967295u;goto L8c616ea4; /* add #-0x1,r3 */
L8c616ea4: sa_motion_publish_stop(ctx,0x8c616ea4u); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c616ea6; /* fmov.s @(r0,r12),fr14 */
L8c616ea6: sa_motion_publish_stop(ctx,0x8c616ea6u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c616ea8; /* tst r3,r3 */
L8c616ea8: sa_motion_publish_stop(ctx,0x8c616ea8u); r[11]=r[12];goto L8c616eaa; /* mov r12,r11 */
L8c616eaa: sa_motion_publish_stop(ctx,0x8c616eaau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c616eac; /* fneg fr15 */
L8c616eac: sa_motion_publish_stop(ctx,0x8c616eaeu); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c616eb4;goto L8c616eb0; /* bt/s 0x8c616eb4 */
L8c616eb0: sa_motion_publish_stop(ctx,0x8c616eb2u); goto L8c616c6c; /* bra 0x8c616c6c */
L8c616eb4: sa_motion_publish_stop(ctx,0x8c616eb4u); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c616eb6; /* cmp/hs r8,r9 */
L8c616eb6: sa_motion_publish_stop(ctx,0x8c616eb6u); if((*sr&1u))goto L8c616edc;goto L8c616eb8; /* bt 0x8c616edc */
L8c616eb8: sa_motion_publish_stop(ctx,0x8c616eb8u); r[5]=rd(m,bus,&io_status,0x8c616facu,4);if(io_status)return io_status;goto L8c616eba; /* mov.l 0x8c616fac,r5 */
L8c616eba: sa_motion_publish_stop(ctx,0x8c616ebau); r[4]=r[12];goto L8c616ebc; /* mov r12,r4 */
L8c616ebc: sa_motion_publish_stop(ctx,0x8c616ebcu); r[8]+=32u;goto L8c616ebe; /* add #0x20,r8 */
L8c616ebe: sa_motion_publish_stop(ctx,0x8c616ebeu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616ec0; /* fschg */
L8c616ec0: sa_motion_publish_stop(ctx,0x8c616ec0u); r[4]+=16u;goto L8c616ec2; /* add #0x10,r4 */
L8c616ec2: sa_motion_publish_stop(ctx,0x8c616ec2u); fcopy(f,0u,1u);goto L8c616ec4; /* fmov fr1,fr0 */
L8c616ec4: sa_motion_publish_stop(ctx,0x8c616ec4u); fcopy(f,2u,3u);goto L8c616ec6; /* fmov fr3,fr2 */
L8c616ec6: sa_motion_publish_stop(ctx,0x8c616ec6u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616ec8; /* fmov.s @r4+,fr4 */
L8c616ec8: sa_motion_publish_stop(ctx,0x8c616ec8u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616eca; /* fmov.s @r4,fr6 */
L8c616eca: sa_motion_publish_stop(ctx,0x8c616ecau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616ecc; /* fmov.s fr2,@-r8 */
L8c616ecc: sa_motion_publish_stop(ctx,0x8c616eccu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616ece; /* fmov.s fr0,@-r8 */
L8c616ece: sa_motion_publish_stop(ctx,0x8c616eceu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616ed0; /* fschg */
L8c616ed0: sa_motion_publish_stop(ctx,0x8c616ed0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616ed2; /* fmov.s fr6,@-r8 */
L8c616ed2: sa_motion_publish_stop(ctx,0x8c616ed2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616ed4; /* fmov.s fr5,@-r8 */
L8c616ed4: sa_motion_publish_stop(ctx,0x8c616ed4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616ed6; /* fmov.s fr4,@-r8 */
L8c616ed6: sa_motion_publish_stop(ctx,0x8c616ed6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616ed8; /* mov.l r5,@-r8 */
L8c616ed8: sa_motion_publish_stop(ctx,0x8c616ed8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616eda; /* pref @r8 */
L8c616eda: sa_motion_publish_stop(ctx,0x8c616edau); r[8]+=32u;goto L8c616edc; /* add #0x20,r8 */
L8c616edc: sa_motion_publish_stop(ctx,0x8c616edcu); r[3]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c616ede; /* mov.l @(0x18,r15),r3 */
L8c616ede: sa_motion_publish_stop(ctx,0x8c616edeu); r[3]+=4294967295u;goto L8c616ee0; /* add #-0x1,r3 */
L8c616ee0: sa_motion_publish_stop(ctx,0x8c616ee0u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c616ee2; /* tst r3,r3 */
L8c616ee2: sa_motion_publish_stop(ctx,0x8c616ee4u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;if(take)goto L8c616eea;goto L8c616ee6; /* bt/s 0x8c616eea */
L8c616ee6: sa_motion_publish_stop(ctx,0x8c616ee8u); goto L8c616c1c; /* bra 0x8c616c1c */
L8c616eea: sa_motion_publish_stop(ctx,0x8c616eeau); r[4]=rd(m,bus,&io_status,0x8c616fbcu,4);if(io_status)return io_status;goto L8c616eec; /* mov.l 0x8c616fbc,r4 */
L8c616eec: sa_motion_publish_stop(ctx,0x8c616eecu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616eee; /* mov.l @(0xc,r15),r3 */
L8c616eee: sa_motion_publish_stop(ctx,0x8c616eeeu); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616ef0; /* mov.l @r4,r2 */
L8c616ef0: sa_motion_publish_stop(ctx,0x8c616ef0u); r[2]+=r[3];goto L8c616ef2; /* add r3,r2 */
L8c616ef2: sa_motion_publish_stop(ctx,0x8c616ef2u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c616ef4; /* mov.l r2,@r4 */
L8c616ef4: sa_motion_publish_stop(ctx,0x8c616ef4u); r[4]=rd(m,bus,&io_status,0x8c616fc0u,4);if(io_status)return io_status;goto L8c616ef6; /* mov.l 0x8c616fc0,r4 */
L8c616ef6: sa_motion_publish_stop(ctx,0x8c616ef6u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616ef8; /* mov.l @(0xc,r15),r3 */
L8c616ef8: sa_motion_publish_stop(ctx,0x8c616ef8u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616efa; /* mov.l @r4,r2 */
L8c616efa: sa_motion_publish_stop(ctx,0x8c616efau); r[2]+=r[3];goto L8c616efc; /* add r3,r2 */
L8c616efc: sa_motion_publish_stop(ctx,0x8c616efcu); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c616efe; /* mov.l r2,@r4 */
L8c616efe: sa_motion_publish_stop(ctx,0x8c616efeu); r[15]+=76u;goto L8c616f00; /* add #0x4c,r15 */
L8c616f00: sa_motion_publish_stop(ctx,0x8c616f00u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616f02; /* lds.l @r15+,PR */
L8c616f02: sa_motion_publish_stop(ctx,0x8c616f02u); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616f04; /* fmov.s @r15+,fr12 */
L8c616f04: sa_motion_publish_stop(ctx,0x8c616f04u); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616f06; /* fmov.s @r15+,fr13 */
L8c616f06: sa_motion_publish_stop(ctx,0x8c616f06u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616f08; /* fmov.s @r15+,fr14 */
L8c616f08: sa_motion_publish_stop(ctx,0x8c616f08u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616f0a; /* fmov.s @r15+,fr15 */
L8c616f0a: sa_motion_publish_stop(ctx,0x8c616f0au); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616f0c; /* mov.l @r15+,r9 */
L8c616f0c: sa_motion_publish_stop(ctx,0x8c616f0cu); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616f0e; /* mov.l @r15+,r10 */
L8c616f0e: sa_motion_publish_stop(ctx,0x8c616f0eu); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616f10; /* mov.l @r15+,r11 */
L8c616f10: sa_motion_publish_stop(ctx,0x8c616f10u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616f12; /* mov.l @r15+,r12 */
L8c616f12: sa_motion_publish_stop(ctx,0x8c616f12u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616f14; /* mov.l @r15+,r13 */
L8c616f14: sa_motion_publish_stop(ctx,0x8c616f16u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
