/* Environment UV/constant-factor triangle near-plane clipper with portable queue interface; name inferred.
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
SA_STAGE_EXPORT int sa_environment_constant_triangle_near_clip_8c61295a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61295a;
L8c61295a: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61295c; /* mov.l r14,@-r15 */
L8c61295c: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61295e; /* mov.l r13,@-r15 */
L8c61295e: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c612960; /* mov.l r12,@-r15 */
L8c612960: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c612962; /* mov.l r11,@-r15 */
L8c612962: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c612964; /* mov.l r10,@-r15 */
L8c612964: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c612966; /* mov.l r9,@-r15 */
L8c612966: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c612968; /* fmov.s fr12,@-r15 */
L8c612968: r[15]+=4294967216u;goto L8c61296a; /* add #-0x50,r15 */
L8c61296a: r[3]=rd(m,bus,&io_status,0x8c612a64u,4);if(io_status)return io_status;goto L8c61296c; /* mov.l 0x8c612a64,r3 */
L8c61296c: r[7]=r[15];goto L8c61296e; /* mov r15,r7 */
L8c61296e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c612970; /* fldi1 fr5 */
L8c612970: r[7]+=68u;goto L8c612972; /* add #0x44,r7 */
L8c612972: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c612974; /* fmov.s @r3,fr3 */
L8c612974: r[9]=r[15];goto L8c612976; /* mov r15,r9 */
L8c612976: fcopy(f,6u,5u);goto L8c612978; /* fmov fr5,fr6 */
L8c612978: r[2]=r[7];goto L8c61297a; /* mov r7,r2 */
L8c61297a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[3],&f->fpscr,&fr[6]);if(status)return status;}goto L8c61297c; /* fdiv fr3,fr6 */
L8c61297c: r[9]+=32u;goto L8c61297e; /* add #0x20,r9 */
L8c61297e: r[0]=0x8c612a6cu;goto L8c612980; /* mova 0x8c612a6c,r0 */
L8c612980: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c612982; /* mov.l r2,@r15 */
L8c612982: wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c612984; /* mov.l r4,@r2 */
L8c612984: r[12]=r[15];goto L8c612986; /* mov r15,r12 */
L8c612986: wr(m,bus,&io_status,r[7]+0x4u,r[5],4);if(io_status)return io_status;goto L8c612988; /* mov.l r5,@(0x4,r7) */
L8c612988: r[11]=r[9];goto L8c61298a; /* mov r9,r11 */
L8c61298a: r[13]=rd(m,bus,&io_status,0x8c612a68u,4);if(io_status)return io_status;goto L8c61298c; /* mov.l 0x8c612a68,r13 */
L8c61298c: r[2]=0u;goto L8c61298e; /* mov #0x0,r2 */
L8c61298e: r[5]=rd(m,bus,&io_status,0x8c612a70u,4);if(io_status)return io_status;goto L8c612990; /* mov.l 0x8c612a70,r5 */
L8c612990: r[4]=1u;goto L8c612992; /* mov #0x1,r4 */
L8c612992: wr(m,bus,&io_status,r[7]+0x8u,r[6],4);if(io_status)return io_status;goto L8c612994; /* mov.l r6,@(0x8,r7) */
L8c612994: r[12]+=20u;goto L8c612996; /* add #0x14,r12 */
L8c612996: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c612998; /* mov.l r2,@(0x4,r15) */
L8c612998: r[14]=r[11];goto L8c61299a; /* mov r11,r14 */
L8c61299a: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61299c; /* mov.l @r15,r10 */
L8c61299c: wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61299e; /* mov.l r11,@r15 */
L8c61299e: fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c6129a0; /* fmov.s @r0,fr4 */
L8c6129a0: r[6]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6129a2; /* mov.l @(0x4,r15),r6 */
L8c6129a2: r[0]=4u;goto L8c6129a4; /* mov #0x4,r0 */
L8c6129a4: r[6]<<=2;goto L8c6129a6; /* shll2 r6 */
L8c6129a6: wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c6129a8; /* mov.l r6,@(0x8,r15) */
L8c6129a8: r[6]+=r[7];goto L8c6129aa; /* add r7,r6 */
L8c6129aa: wr(m,bus,&io_status,r[15]+0x10u,r[6],4);if(io_status)return io_status;goto L8c6129ac; /* mov.l r6,@(0x10,r15) */
L8c6129ac: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6129ae; /* mov.l @(0x8,r15),r3 */
L8c6129ae: r[6]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6129b0; /* mov.l @r6,r6 */
L8c6129b0: r[3]+=r[12];goto L8c6129b2; /* add r12,r3 */
L8c6129b2: wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6129b4; /* mov.l r3,@(0xc,r15) */
L8c6129b4: r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6129b6; /* mov.l r3,@-r15 */
L8c6129b6: r[2]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c6129b8; /* mov.l @r5,r2 */
L8c6129b8: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c6129ba; /* fmov.s @(r0,r6),fr0 */
L8c6129ba: r[0]=16u;goto L8c6129bc; /* mov #0x10,r0 */
L8c6129bc: r[3]=r[2];goto L8c6129be; /* mov r2,r3 */
L8c6129be: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6129c0; /* fmov.s @(r0,r3),fr3 */
L8c6129c0: r[1]=r[2];goto L8c6129c2; /* mov r2,r1 */
L8c6129c2: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c6129c4; /* fmov.s @r1,fr2 */
L8c6129c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6129c6; /* fmul fr0,fr3 */
L8c6129c6: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c6129c8; /* fmov.s @r6,fr0 */
L8c6129c8: r[0]=8u;goto L8c6129ca; /* mov #0x8,r0 */
L8c6129ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6129cc; /* fmac fr0,fr2,fr3 */
L8c6129cc: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c6129ce; /* fmov.s @(r0,r6),fr0 */
L8c6129ce: r[0]=32u;goto L8c6129d0; /* mov #0x20,r0 */
L8c6129d0: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6129d2; /* fmov.s @(r0,r2),fr2 */
L8c6129d2: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6129d4; /* mov.l @r15+,r2 */
L8c6129d4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6129d6; /* fmac fr0,fr2,fr3 */
L8c6129d6: fcopy(f,0u,4u);goto L8c6129d8; /* fmov fr4,fr0 */
L8c6129d8: fcopy(f,2u,4u);goto L8c6129da; /* fmov fr4,fr2 */
L8c6129da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6129dc; /* fmac fr0,fr3,fr2 */
L8c6129dc: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c6129de; /* ftrc fr2,FPUL */
L8c6129de: r[3]=*fpul;goto L8c6129e0; /* sts FPUL,r3 */
L8c6129e0: wr(m,bus,&io_status,r[2],r[3],2);if(io_status)return io_status;goto L8c6129e2; /* mov.w r3,@r2 */
L8c6129e2: r[0]=36u;goto L8c6129e4; /* mov #0x24,r0 */
L8c6129e4: r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c6129e6; /* mov.l @r5,r3 */
L8c6129e6: r[1]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6129e8; /* mov.l @(0xc,r15),r1 */
L8c6129e8: fload(m,bus,&io_status,f,1u,r[0]+r[3]);if(io_status)return io_status;goto L8c6129ea; /* fmov.s @(r0,r3),fr1 */
L8c6129ea: r[0]=4u;goto L8c6129ec; /* mov #0x4,r0 */
L8c6129ec: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c6129ee; /* fmov.s @(r0,r6),fr0 */
L8c6129ee: r[0]=20u;goto L8c6129f0; /* mov #0x14,r0 */
L8c6129f0: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6129f2; /* fmov.s @(r0,r3),fr3 */
L8c6129f2: r[0]=4u;goto L8c6129f4; /* mov #0x4,r0 */
L8c6129f4: r[2]=r[3];goto L8c6129f6; /* mov r3,r2 */
L8c6129f6: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6129f8; /* fmov.s @(r0,r2),fr2 */
L8c6129f8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6129fa; /* fmul fr0,fr3 */
L8c6129fa: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c6129fc; /* fmov.s @r6,fr0 */
L8c6129fc: r[0]=8u;goto L8c6129fe; /* mov #0x8,r0 */
L8c6129fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612a00; /* fmac fr0,fr2,fr3 */
L8c612a00: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c612a02; /* fmov.s @(r0,r6),fr0 */
L8c612a02: fcopy(f,2u,4u);goto L8c612a04; /* fmov fr4,fr2 */
L8c612a04: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612a06; /* fmac fr0,fr1,fr3 */
L8c612a06: fcopy(f,0u,4u);goto L8c612a08; /* fmov fr4,fr0 */
L8c612a08: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612a0a; /* fmac fr0,fr3,fr2 */
L8c612a0a: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c612a0c; /* ftrc fr2,FPUL */
L8c612a0c: r[0]=*fpul;goto L8c612a0e; /* sts FPUL,r0 */
L8c612a0e: wr(m,bus,&io_status,r[1]+0x2u,r[0],2);if(io_status)return io_status;goto L8c612a10; /* mov.w r0,@(0x2,r1) */
L8c612a10: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c612a12; /* mov.l @(0x10,r15),r3 */
L8c612a12: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c612a14; /* mov.l @r3,r2 */
L8c612a14: r[1]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612a16; /* mov.l @(0xc,r2),r1 */
L8c612a16: *sr=(*sr&~1u)|!(r[4]&r[1]);goto L8c612a18; /* tst r4,r1 */
L8c612a18: if(!(*sr&1u))goto L8c612a7c;goto L8c612a1a; /* bf 0x8c612a7c */
L8c612a1a: r[6]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612a1c; /* mov.l @(0x4,r15),r6 */
L8c612a1c: r[0]=24u;goto L8c612a1e; /* mov #0x18,r0 */
L8c612a1e: fcopy(f,7u,5u);goto L8c612a20; /* fmov fr5,fr7 */
L8c612a20: r[6]<<=2;goto L8c612a22; /* shll2 r6 */
L8c612a22: r[3]=rd(m,bus,&io_status,0x8c612a74u,4);if(io_status)return io_status;goto L8c612a24; /* mov.l 0x8c612a74,r3 */
L8c612a24: r[6]+=r[7];goto L8c612a26; /* add r7,r6 */
L8c612a26: wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c612a28; /* mov.l r6,@(0x8,r15) */
L8c612a28: r[6]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c612a2a; /* mov.l @r6,r6 */
L8c612a2a: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612a2c; /* fmov.s @(r0,r6),fr3 */
L8c612a2c: r[0]=12u;goto L8c612a2e; /* mov #0xc,r0 */
L8c612a2e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[7],&fr[3],&f->fpscr,&fr[7]);if(status)return status;}goto L8c612a30; /* fdiv fr3,fr7 */
L8c612a30: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c612a32; /* fmov.s @(r0,r13),fr3 */
L8c612a32: r[0]=16u;goto L8c612a34; /* mov #0x10,r0 */
L8c612a34: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c612a36; /* fmov.s @(r0,r6),fr2 */
L8c612a36: r[0]=16u;goto L8c612a38; /* mov #0x10,r0 */
L8c612a38: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612a3a; /* fsub fr3,fr2 */
L8c612a3a: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c612a3c; /* fmov.s @r3,fr3 */
L8c612a3c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612a3e; /* fmul fr7,fr2 */
L8c612a3e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612a40; /* fdiv fr3,fr2 */
L8c612a40: fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c612a42; /* fmov.s fr2,@r14 */
L8c612a42: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612a44; /* mov.l @(0x8,r15),r2 */
L8c612a44: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c612a46; /* fmov.s @(r0,r13),fr3 */
L8c612a46: r[0]=20u;goto L8c612a48; /* mov #0x14,r0 */
L8c612a48: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c612a4a; /* mov.l @r2,r1 */
L8c612a4a: r[2]=rd(m,bus,&io_status,0x8c612a78u,4);if(io_status)return io_status;goto L8c612a4c; /* mov.l 0x8c612a78,r2 */
L8c612a4c: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c612a4e; /* fmov.s @(r0,r1),fr2 */
L8c612a4e: r[0]=4u;goto L8c612a50; /* mov #0x4,r0 */
L8c612a50: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612a52; /* fsub fr3,fr2 */
L8c612a52: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c612a54; /* fmov.s @r2,fr3 */
L8c612a54: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612a56; /* fmul fr7,fr2 */
L8c612a56: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612a58; /* fdiv fr3,fr2 */
L8c612a58: fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c612a5a; /* fmov fr2,@(r0,r14) */
L8c612a5a: r[0]=8u;goto L8c612a5c; /* mov #0x8,r0 */
L8c612a5c: fstore(m,bus,&io_status,f,7u,r[0]+r[14]);if(io_status)return io_status;goto L8c612a98; /* bra 0x8c612a98 */
L8c612a7c: r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c612a7e; /* mov.l @r10,r3 */
L8c612a7e: r[0]=16u;goto L8c612a80; /* mov #0x10,r0 */
L8c612a80: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612a82; /* fmov.s @(r0,r3),fr3 */
L8c612a82: r[0]=20u;goto L8c612a84; /* mov #0x14,r0 */
L8c612a84: fstore(m,bus,&io_status,f,3u,r[11]);if(io_status)return io_status;goto L8c612a86; /* fmov.s fr3,@r11 */
L8c612a86: r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c612a88; /* mov.l @r10,r3 */
L8c612a88: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612a8a; /* fmov.s @(r0,r3),fr3 */
L8c612a8a: r[0]=4u;goto L8c612a8c; /* mov #0x4,r0 */
L8c612a8c: fstore(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c612a8e; /* fmov fr3,@(r0,r11) */
L8c612a8e: r[0]=24u;goto L8c612a90; /* mov #0x18,r0 */
L8c612a90: r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c612a92; /* mov.l @r10,r3 */
L8c612a92: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612a94; /* fmov.s @(r0,r3),fr3 */
L8c612a94: r[0]=8u;goto L8c612a96; /* mov #0x8,r0 */
L8c612a96: fstore(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c612a98; /* fmov fr3,@(r0,r11) */
L8c612a98: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612a9a; /* mov.l @(0x4,r15),r3 */
L8c612a9a: r[2]=3u;goto L8c612a9c; /* mov #0x3,r2 */
L8c612a9c: r[11]+=12u;goto L8c612a9e; /* add #0xc,r11 */
L8c612a9e: r[3]+=1u;goto L8c612aa0; /* add #0x1,r3 */
L8c612aa0: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c612aa2; /* cmp/ge r2,r3 */
L8c612aa2: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c612aa4; /* mov.l r3,@(0x4,r15) */
L8c612aa4: r[10]+=4u;goto L8c612aa6; /* add #0x4,r10 */
L8c612aa6: take=(*sr&1u);r[14]+=12u;if(take)goto L8c612aae;goto L8c612aaa; /* bt/s 0x8c612aae */
L8c612aaa: goto L8c6129a0; /* bra 0x8c6129a0 */
L8c612aae: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612ab0; /* mov.l @r15,r3 */
L8c612ab0: r[0]=8u;goto L8c612ab2; /* mov #0x8,r0 */
L8c612ab2: r[6]=r[9];goto L8c612ab4; /* mov r9,r6 */
L8c612ab4: r[6]+=12u;goto L8c612ab6; /* add #0xc,r6 */
L8c612ab6: fload(m,bus,&io_status,f,8u,r[0]+r[3]);if(io_status)return io_status;goto L8c612ab8; /* fmov.s @(r0,r3),fr8 */
L8c612ab8: r[0]=4u;goto L8c612aba; /* mov #0x4,r0 */
L8c612aba: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c612abc; /* fmov.s @(r0,r6),fr4 */
L8c612abc: r[5]=r[9];goto L8c612abe; /* mov r9,r5 */
L8c612abe: fload(m,bus,&io_status,f,10u,r[0]+r[3]);if(io_status)return io_status;goto L8c612ac0; /* fmov.s @(r0,r3),fr10 */
L8c612ac0: r[0]=8u;goto L8c612ac2; /* mov #0x8,r0 */
L8c612ac2: fload(m,bus,&io_status,f,7u,r[0]+r[6]);if(io_status)return io_status;goto L8c612ac4; /* fmov.s @(r0,r6),fr7 */
L8c612ac4: r[0]=4u;goto L8c612ac6; /* mov #0x4,r0 */
L8c612ac6: fcopy(f,3u,10u);goto L8c612ac8; /* fmov fr10,fr3 */
L8c612ac8: r[5]+=24u;goto L8c612aca; /* add #0x18,r5 */
L8c612aca: fcopy(f,0u,8u);goto L8c612acc; /* fmov fr8,fr0 */
L8c612acc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612ace; /* fmul fr7,fr3 */
L8c612ace: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c612ad0; /* fmul fr4,fr0 */
L8c612ad0: fload(m,bus,&io_status,f,11u,r[3]);if(io_status)return io_status;goto L8c612ad2; /* fmov.s @r3,fr11 */
L8c612ad2: fload(m,bus,&io_status,f,9u,r[6]);if(io_status)return io_status;goto L8c612ad4; /* fmov.s @r6,fr9 */
L8c612ad4: fcopy(f,2u,8u);goto L8c612ad6; /* fmov fr8,fr2 */
L8c612ad6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[9],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612ad8; /* fmul fr9,fr2 */
L8c612ad8: fcopy(f,1u,9u);goto L8c612ada; /* fmov fr9,fr1 */
L8c612ada: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c612adc; /* fsub fr3,fr0 */
L8c612adc: fcopy(f,3u,11u);goto L8c612ade; /* fmov fr11,fr3 */
L8c612ade: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612ae0; /* fmul fr7,fr3 */
L8c612ae0: fcopy(f,12u,11u);goto L8c612ae2; /* fmov fr11,fr12 */
L8c612ae2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[10],&f->fpscr,&fr[1]);if(status)return status;}goto L8c612ae4; /* fmul fr10,fr1 */
L8c612ae4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612ae6; /* fsub fr2,fr3 */
L8c612ae6: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c612ae8; /* fmov.s @(r0,r5),fr2 */
L8c612ae8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612aea; /* fmul fr3,fr2 */
L8c612aea: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c612aec; /* fmov.s @r5,fr3 */
L8c612aec: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612aee; /* fmac fr0,fr3,fr2 */
L8c612aee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[4],&f->fpscr,&fr[12]);if(status)return status;}goto L8c612af0; /* fmul fr4,fr12 */
L8c612af0: r[0]=8u;goto L8c612af2; /* mov #0x8,r0 */
L8c612af2: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c612af4; /* fmov.s @(r0,r5),fr3 */
L8c612af4: r[5]=rd(m,bus,&io_status,0x8c612d44u,4);if(io_status)return io_status;goto L8c612af6; /* mov.l 0x8c612d44,r5 */
L8c612af6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[12],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c612af8; /* fsub fr12,fr1 */
L8c612af8: r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c612afa; /* mov.l @r5,r3 */
L8c612afa: fcopy(f,0u,1u);goto L8c612afc; /* fmov fr1,fr0 */
L8c612afc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612afe; /* fmac fr0,fr3,fr2 */
L8c612afe: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c612b00; /* fldi0 fr3 */
L8c612b00: fcopy(f,4u,2u);goto L8c612b02; /* fmov fr2,fr4 */
L8c612b02: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c612b04; /* fcmp/gt fr4,fr3 */
L8c612b04: r[0]=*sr&1u;goto L8c612b06; /* movt r0 */
L8c612b06: *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c612b08; /* tst r3,r0 */
L8c612b08: if((*sr&1u))goto L8c612b0e;goto L8c612b0a; /* bt 0x8c612b0e */
L8c612b0a: goto L8c612e52; /* bra 0x8c612e52 */
L8c612b0e: r[1]=0u;goto L8c612b10; /* mov #0x0,r1 */
L8c612b10: r[14]=rd(m,bus,&io_status,0x8c612d48u,4);if(io_status)return io_status;goto L8c612b12; /* mov.l 0x8c612d48,r14 */
L8c612b12: wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c612b14; /* mov.l r1,@r5 */
L8c612b14: r[3]=2u;goto L8c612b16; /* mov #0x2,r3 */
L8c612b16: r[5]=rd(m,bus,&io_status,0x8c612d4cu,4);if(io_status)return io_status;goto L8c612b18; /* mov.l 0x8c612d4c,r5 */
L8c612b18: r[6]=r[1];goto L8c612b1a; /* mov r1,r6 */
L8c612b1a: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c612b1c; /* mov.l r3,@r15 */
L8c612b1c: r[10]=r[6];goto L8c612b1e; /* mov r6,r10 */
L8c612b1e: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612b20; /* mov.l @r15,r3 */
L8c612b20: r[0]=r[7];goto L8c612b22; /* mov r7,r0 */
L8c612b22: r[11]=r[10];goto L8c612b24; /* mov r10,r11 */
L8c612b24: r[11]<<=2;goto L8c612b26; /* shll2 r11 */
L8c612b26: r[3]<<=2;goto L8c612b28; /* shll2 r3 */
L8c612b28: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c612b2a; /* mov.l r3,@(0x4,r15) */
L8c612b2a: r[2]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c612b2c; /* mov.l @(r0,r3),r2 */
L8c612b2c: r[3]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612b2e; /* mov.l @(0xc,r2),r3 */
L8c612b2e: r[2]=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;goto L8c612b30; /* mov.l @(r0,r11),r2 */
L8c612b30: r[3]&=r[4];goto L8c612b32; /* and r4,r3 */
L8c612b32: if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[2]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612b34; /* mov.l @(0xc,r2),r2 */
L8c612b34: r[2]&=r[4];goto L8c612b36; /* and r4,r2 */
L8c612b36: r[3]^=r[2];goto L8c612b38; /* xor r2,r3 */
L8c612b38: r[0]=r[3];goto L8c612b3a; /* mov r3,r0 */
L8c612b3a: *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c612b3c; /* cmp/eq #0x1,r0 */
L8c612b3c: if(!(*sr&1u))goto L8c612c22;goto L8c612b3e; /* bf 0x8c612c22 */
L8c612b3e: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612b40; /* mov.l @r15,r2 */
L8c612b40: r[0]=8u;goto L8c612b42; /* mov #0x8,r0 */
L8c612b42: r[1]=r[10];goto L8c612b44; /* mov r10,r1 */
L8c612b44: r[3]=r[2];goto L8c612b46; /* mov r2,r3 */
L8c612b46: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612b48; /* shll r2 */
L8c612b48: r[2]+=r[3];goto L8c612b4a; /* add r3,r2 */
L8c612b4a: r[3]=rd(m,bus,&io_status,0x8c612d50u,4);if(io_status)return io_status;goto L8c612b4c; /* mov.l 0x8c612d50,r3 */
L8c612b4c: r[2]<<=2;goto L8c612b4e; /* shll2 r2 */
L8c612b4e: r[2]+=r[9];goto L8c612b50; /* add r9,r2 */
L8c612b50: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c612b52; /* mov.l r2,@(0xc,r15) */
L8c612b52: fload(m,bus,&io_status,f,4u,r[0]+r[2]);if(io_status)return io_status;goto L8c612b54; /* fmov.s @(r0,r2),fr4 */
L8c612b54: r[2]=r[10];goto L8c612b56; /* mov r10,r2 */
L8c612b56: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612b58; /* shll r2 */
L8c612b58: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c612b5a; /* fmov.s @r3,fr3 */
L8c612b5a: r[2]+=r[1];goto L8c612b5c; /* add r1,r2 */
L8c612b5c: r[2]<<=2;goto L8c612b5e; /* shll2 r2 */
L8c612b5e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612b60; /* fsub fr4,fr3 */
L8c612b60: r[0]=8u;goto L8c612b62; /* mov #0x8,r0 */
L8c612b62: r[2]+=r[9];goto L8c612b64; /* add r9,r2 */
L8c612b64: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c612b66; /* mov.l r2,@(0x8,r15) */
L8c612b66: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c612b68; /* fmov.s @(r0,r2),fr2 */
L8c612b68: r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612b6a; /* mov.l @(0x8,r15),r1 */
L8c612b6a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612b6c; /* fsub fr4,fr2 */
L8c612b6c: fcopy(f,4u,3u);goto L8c612b6e; /* fmov fr3,fr4 */
L8c612b6e: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c612b70; /* mov.l @(0xc,r15),r2 */
L8c612b70: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c612b72; /* fmov.s @r1,fr3 */
L8c612b72: fload(m,bus,&io_status,f,7u,r[2]);if(io_status)return io_status;goto L8c612b74; /* fmov.s @r2,fr7 */
L8c612b74: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c612b76; /* fdiv fr2,fr4 */
L8c612b76: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612b78; /* fsub fr7,fr3 */
L8c612b78: fcopy(f,2u,7u);goto L8c612b7a; /* fmov fr7,fr2 */
L8c612b7a: fcopy(f,0u,4u);goto L8c612b7c; /* fmov fr4,fr0 */
L8c612b7c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612b7e; /* fmac fr0,fr3,fr2 */
L8c612b7e: r[1]=rd(m,bus,&io_status,0x8c612d54u,4);if(io_status)return io_status;goto L8c612b80; /* mov.l 0x8c612d54,r1 */
L8c612b80: r[0]=12u;goto L8c612b82; /* mov #0xc,r0 */
L8c612b82: fcopy(f,7u,2u);goto L8c612b84; /* fmov fr2,fr7 */
L8c612b84: r[2]=r[6];goto L8c612b86; /* mov r6,r2 */
L8c612b86: fcopy(f,0u,6u);goto L8c612b88; /* fmov fr6,fr0 */
L8c612b88: r[2]<<=2;goto L8c612b8a; /* shll2 r2 */
L8c612b8a: r[2]<<=2;goto L8c612b8c; /* shll2 r2 */
L8c612b8c: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612b8e; /* shll r2 */
L8c612b8e: r[2]+=r[5];goto L8c612b90; /* add r5,r2 */
L8c612b90: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c612b92; /* mov.l r2,@r15 */
L8c612b92: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c612b94; /* fmov.s @r1,fr3 */
L8c612b94: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c612b96; /* fmov.s @(r0,r13),fr2 */
L8c612b96: r[0]=16u;goto L8c612b98; /* mov #0x10,r0 */
L8c612b98: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612b9a; /* fmul fr7,fr3 */
L8c612b9a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612b9c; /* fmac fr0,fr3,fr2 */
L8c612b9c: fcopy(f,0u,4u);goto L8c612b9e; /* fmov fr4,fr0 */
L8c612b9e: fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c612ba0; /* fmov fr2,@(r0,r2) */
L8c612ba0: r[0]=4u;goto L8c612ba2; /* mov #0x4,r0 */
L8c612ba2: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c612ba4; /* mov.l @(0xc,r15),r2 */
L8c612ba4: fload(m,bus,&io_status,f,7u,r[0]+r[2]);if(io_status)return io_status;goto L8c612ba6; /* fmov.s @(r0,r2),fr7 */
L8c612ba6: r[0]=4u;goto L8c612ba8; /* mov #0x4,r0 */
L8c612ba8: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612baa; /* mov.l @(0x8,r15),r2 */
L8c612baa: fcopy(f,2u,7u);goto L8c612bac; /* fmov fr7,fr2 */
L8c612bac: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612bae; /* fmov.s @(r0,r2),fr3 */
L8c612bae: r[0]=rd(m,bus,&io_status,0x8c612d58u,4);if(io_status)return io_status;goto L8c612bb0; /* mov.l 0x8c612d58,r0 */
L8c612bb0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612bb2; /* fsub fr7,fr3 */
L8c612bb2: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612bb4; /* mov.l @r15,r2 */
L8c612bb4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612bb6; /* fmac fr0,fr3,fr2 */
L8c612bb6: fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c612bb8; /* fmov.s @r0,fr3 */
L8c612bb8: r[0]=16u;goto L8c612bba; /* mov #0x10,r0 */
L8c612bba: fcopy(f,7u,2u);goto L8c612bbc; /* fmov fr2,fr7 */
L8c612bbc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612bbe; /* fmul fr7,fr3 */
L8c612bbe: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c612bc0; /* fmov.s @(r0,r13),fr2 */
L8c612bc0: r[0]=20u;goto L8c612bc2; /* mov #0x14,r0 */
L8c612bc2: fcopy(f,0u,6u);goto L8c612bc4; /* fmov fr6,fr0 */
L8c612bc4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612bc6; /* fmac fr0,fr3,fr2 */
L8c612bc6: fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c612bc8; /* fmov fr2,@(r0,r2) */
L8c612bc8: r[0]=24u;goto L8c612bca; /* mov #0x18,r0 */
L8c612bca: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612bcc; /* mov.l @r15,r3 */
L8c612bcc: fstore(m,bus,&io_status,f,0u,r[0]+r[3]);if(io_status)return io_status;goto L8c612bce; /* fmov fr0,@(r0,r3) */
L8c612bce: r[0]=r[11];goto L8c612bd0; /* mov r11,r0 */
L8c612bd0: r[0]+=r[12];goto L8c612bd2; /* add r12,r0 */
L8c612bd2: fcopy(f,0u,4u);goto L8c612bd4; /* fmov fr4,fr0 */
L8c612bd4: r[3]=r[6];goto L8c612bd6; /* mov r6,r3 */
L8c612bd6: r[3]<<=2;goto L8c612bd8; /* shll2 r3 */
L8c612bd8: r[3]+=r[14];goto L8c612bda; /* add r14,r3 */
L8c612bda: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c612bdc; /* mov.l r3,@(0x8,r15) */
L8c612bdc: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612bde; /* mov.l @(0x4,r15),r2 */
L8c612bde: r[2]+=r[12];goto L8c612be0; /* add r12,r2 */
L8c612be0: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c612be2; /* mov.l r2,@(0xc,r15) */
L8c612be2: r[2]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c612be4; /* mov.w @r2,r2 */
L8c612be4: wr(m,bus,&io_status,r[15],r[2],2);if(io_status)return io_status;goto L8c612be6; /* mov.w r2,@r15 */
L8c612be6: *fpul=r[2];goto L8c612be8; /* lds r2,FPUL */
L8c612be8: wr(m,bus,&io_status,r[15]+0x10u,r[0],4);if(io_status)return io_status;goto L8c612bea; /* mov.l r0,@(0x10,r15) */
L8c612bea: r[2]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c612bec; /* mov.w @r0,r2 */
L8c612bec: r[0]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c612bee; /* mov.w @r15,r0 */
L8c612bee: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612bf0; /* float FPUL,fr3 */
L8c612bf0: r[2]-=r[0];goto L8c612bf2; /* sub r0,r2 */
L8c612bf2: *fpul=r[2];goto L8c612bf4; /* lds r2,FPUL */
L8c612bf4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c612bf6; /* float FPUL,fr2 */
L8c612bf6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612bf8; /* fmac fr0,fr2,fr3 */
L8c612bf8: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c612bfa; /* ftrc fr3,FPUL */
L8c612bfa: r[2]=*fpul;goto L8c612bfc; /* sts FPUL,r2 */
L8c612bfc: wr(m,bus,&io_status,r[3],r[2],2);if(io_status)return io_status;goto L8c612bfe; /* mov.w r2,@r3 */
L8c612bfe: r[0]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c612c00; /* mov.l @(0xc,r15),r0 */
L8c612c00: r[6]+=1u;goto L8c612c02; /* add #0x1,r6 */
L8c612c02: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612c04; /* mov.l @(0x8,r15),r3 */
L8c612c04: r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c612c06; /* mov.w @(0x2,r0),r0 */
L8c612c06: *fpul=r[0];goto L8c612c08; /* lds r0,FPUL */
L8c612c08: wr(m,bus,&io_status,r[15]+0x4u,r[0],2);if(io_status)return io_status;goto L8c612c0a; /* mov.w r0,@(0x4,r15) */
L8c612c0a: r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c612c0c; /* mov.l @(0x10,r15),r2 */
L8c612c0c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612c0e; /* float FPUL,fr3 */
L8c612c0e: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c612c10; /* mov.w @(0x2,r2),r0 */
L8c612c10: r[2]=r[0];goto L8c612c12; /* mov r0,r2 */
L8c612c12: r[0]=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;goto L8c612c14; /* mov.w @(0x4,r15),r0 */
L8c612c14: r[2]-=r[0];goto L8c612c16; /* sub r0,r2 */
L8c612c16: *fpul=r[2];goto L8c612c18; /* lds r2,FPUL */
L8c612c18: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c612c1a; /* float FPUL,fr2 */
L8c612c1a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612c1c; /* fmac fr0,fr2,fr3 */
L8c612c1c: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c612c1e; /* ftrc fr3,FPUL */
L8c612c1e: r[0]=*fpul;goto L8c612c20; /* sts FPUL,r0 */
L8c612c20: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c612c22; /* mov.w r0,@(0x2,r3) */
L8c612c22: r[0]=r[11];goto L8c612c24; /* mov r11,r0 */
L8c612c24: r[3]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c612c26; /* mov.l @(r0,r7),r3 */
L8c612c26: r[3]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c612c28; /* mov.l @(0xc,r3),r3 */
L8c612c28: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c612c2a; /* tst r4,r3 */
L8c612c2a: if(!(*sr&1u))goto L8c612c7a;goto L8c612c2c; /* bf 0x8c612c7a */
L8c612c2c: r[3]=r[11];goto L8c612c2e; /* mov r11,r3 */
L8c612c2e: r[2]=r[6];goto L8c612c30; /* mov r6,r2 */
L8c612c30: r[3]+=r[7];goto L8c612c32; /* add r7,r3 */
L8c612c32: r[2]<<=2;goto L8c612c34; /* shll2 r2 */
L8c612c34: r[0]=16u;goto L8c612c36; /* mov #0x10,r0 */
L8c612c36: r[2]<<=2;goto L8c612c38; /* shll2 r2 */
L8c612c38: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612c3a; /* shll r2 */
L8c612c3a: r[2]+=r[5];goto L8c612c3c; /* add r5,r2 */
L8c612c3c: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c612c3e; /* mov.l r2,@(0x4,r15) */
L8c612c3e: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c612c40; /* mov.l r3,@r15 */
L8c612c40: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c612c42; /* mov.l @r3,r1 */
L8c612c42: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612c44; /* fmov.s @(r0,r1),fr3 */
L8c612c44: fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612c46; /* fmov fr3,@(r0,r2) */
L8c612c46: r[0]=20u;goto L8c612c48; /* mov #0x14,r0 */
L8c612c48: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612c4a; /* mov.l @r15,r2 */
L8c612c4a: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612c4c; /* mov.l @(0x4,r15),r3 */
L8c612c4c: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c612c4e; /* mov.l @r2,r1 */
L8c612c4e: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612c50; /* fmov.s @(r0,r1),fr3 */
L8c612c50: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612c52; /* fmov fr3,@(r0,r3) */
L8c612c52: r[0]=24u;goto L8c612c54; /* mov #0x18,r0 */
L8c612c54: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612c56; /* mov.l @r15,r2 */
L8c612c56: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612c58; /* mov.l @(0x4,r15),r3 */
L8c612c58: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c612c5a; /* mov.l @r2,r1 */
L8c612c5a: r[2]=r[11];goto L8c612c5c; /* mov r11,r2 */
L8c612c5c: r[2]+=r[12];goto L8c612c5e; /* add r12,r2 */
L8c612c5e: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612c60; /* fmov.s @(r0,r1),fr3 */
L8c612c60: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612c62; /* fmov fr3,@(r0,r3) */
L8c612c62: r[3]=r[6];goto L8c612c64; /* mov r6,r3 */
L8c612c64: r[3]<<=2;goto L8c612c66; /* shll2 r3 */
L8c612c66: r[3]+=r[14];goto L8c612c68; /* add r14,r3 */
L8c612c68: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c612c6a; /* mov.l r3,@(0x4,r15) */
L8c612c6a: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c612c6c; /* mov.l r2,@r15 */
L8c612c6c: r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c612c6e; /* mov.w @r2,r1 */
L8c612c6e: r[6]+=1u;goto L8c612c70; /* add #0x1,r6 */
L8c612c70: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c612c72; /* mov.w r1,@r3 */
L8c612c72: r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612c74; /* mov.l @r15,r0 */
L8c612c74: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612c76; /* mov.l @(0x4,r15),r3 */
L8c612c76: r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c612c78; /* mov.w @(0x2,r0),r0 */
L8c612c78: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c612c7a; /* mov.w r0,@(0x2,r3) */
L8c612c7a: wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c612c7c; /* mov.l r10,@r15 */
L8c612c7c: r[3]=3u;goto L8c612c7e; /* mov #0x3,r3 */
L8c612c7e: r[10]+=1u;goto L8c612c80; /* add #0x1,r10 */
L8c612c80: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[3]);goto L8c612c82; /* cmp/ge r3,r10 */
L8c612c82: if((*sr&1u))goto L8c612c88;goto L8c612c84; /* bt 0x8c612c88 */
L8c612c84: goto L8c612b1e; /* bra 0x8c612b1e */
L8c612c88: r[0]=0x8c612d64u;goto L8c612c8a; /* mova 0x8c612d64,r0 */
L8c612c8a: r[4]=rd(m,bus,&io_status,0x8c612d60u,4);if(io_status)return io_status;goto L8c612c8c; /* mov.l 0x8c612d60,r4 */
L8c612c8c: fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c612c8e; /* fmov.s @r0,fr4 */
L8c612c8e: r[0]=r[6];goto L8c612c90; /* mov r6,r0 */
L8c612c90: r[7]=rd(m,bus,&io_status,0x8c612d5cu,4);if(io_status)return io_status;goto L8c612c92; /* mov.l 0x8c612d5c,r7 */
L8c612c92: *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c612c94; /* cmp/eq #0x3,r0 */
L8c612c94: if(!(*sr&1u))goto L8c612d6c;goto L8c612c96; /* bf 0x8c612d6c */
L8c612c96: r[6]=r[14];goto L8c612c98; /* mov r14,r6 */
L8c612c98: r[8]+=28u;goto L8c612c9a; /* add #0x1c,r8 */
L8c612c9a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612c9c; /* fmov.s fr5,@-r8 */
L8c612c9c: r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c612c9e; /* mov.w @(0x2,r6),r0 */
L8c612c9e: r[3]=r[0];goto L8c612ca0; /* mov r0,r3 */
L8c612ca0: *fpul=r[3];goto L8c612ca2; /* lds r3,FPUL */
L8c612ca2: r[0]=24u;goto L8c612ca4; /* mov #0x18,r0 */
L8c612ca4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612ca6; /* float FPUL,fr3 */
L8c612ca6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612ca8; /* fmul fr4,fr3 */
L8c612ca8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612caa; /* fmov.s fr3,@-r8 */
L8c612caa: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c612cac; /* mov.w @r6,r3 */
L8c612cac: r[6]=r[5];goto L8c612cae; /* mov r5,r6 */
L8c612cae: *fpul=r[3];goto L8c612cb0; /* lds r3,FPUL */
L8c612cb0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612cb2; /* float FPUL,fr3 */
L8c612cb2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612cb4; /* fmul fr4,fr3 */
L8c612cb4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cb6; /* fmov.s fr3,@-r8 */
L8c612cb6: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612cb8; /* fmov.s @(r0,r6),fr3 */
L8c612cb8: r[0]=20u;goto L8c612cba; /* mov #0x14,r0 */
L8c612cba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cbc; /* fmov.s fr3,@-r8 */
L8c612cbc: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612cbe; /* fmov.s @(r0,r6),fr3 */
L8c612cbe: r[0]=16u;goto L8c612cc0; /* mov #0x10,r0 */
L8c612cc0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cc2; /* fmov.s fr3,@-r8 */
L8c612cc2: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612cc4; /* fmov.s @(r0,r6),fr3 */
L8c612cc4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cc6; /* fmov.s fr3,@-r8 */
L8c612cc6: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612cc8; /* mov.l r4,@-r8 */
L8c612cc8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612cca; /* pref @r8 */
L8c612cca: r[6]=r[14];goto L8c612ccc; /* mov r14,r6 */
L8c612ccc: r[6]+=4u;goto L8c612cce; /* add #0x4,r6 */
L8c612cce: r[8]+=60u;goto L8c612cd0; /* add #0x3c,r8 */
L8c612cd0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612cd2; /* fmov.s fr5,@-r8 */
L8c612cd2: r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c612cd4; /* mov.w @(0x2,r6),r0 */
L8c612cd4: r[3]=r[0];goto L8c612cd6; /* mov r0,r3 */
L8c612cd6: *fpul=r[3];goto L8c612cd8; /* lds r3,FPUL */
L8c612cd8: r[0]=24u;goto L8c612cda; /* mov #0x18,r0 */
L8c612cda: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612cdc; /* float FPUL,fr3 */
L8c612cdc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612cde; /* fmul fr4,fr3 */
L8c612cde: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612ce0; /* fmov.s fr3,@-r8 */
L8c612ce0: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c612ce2; /* mov.w @r6,r3 */
L8c612ce2: r[6]=r[5];goto L8c612ce4; /* mov r5,r6 */
L8c612ce4: r[6]+=32u;goto L8c612ce6; /* add #0x20,r6 */
L8c612ce6: *fpul=r[3];goto L8c612ce8; /* lds r3,FPUL */
L8c612ce8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612cea; /* float FPUL,fr3 */
L8c612cea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612cec; /* fmul fr4,fr3 */
L8c612cec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cee; /* fmov.s fr3,@-r8 */
L8c612cee: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612cf0; /* fmov.s @(r0,r6),fr3 */
L8c612cf0: r[0]=20u;goto L8c612cf2; /* mov #0x14,r0 */
L8c612cf2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cf4; /* fmov.s fr3,@-r8 */
L8c612cf4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612cf6; /* fmov.s @(r0,r6),fr3 */
L8c612cf6: r[0]=16u;goto L8c612cf8; /* mov #0x10,r0 */
L8c612cf8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cfa; /* fmov.s fr3,@-r8 */
L8c612cfa: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612cfc; /* fmov.s @(r0,r6),fr3 */
L8c612cfc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cfe; /* fmov.s fr3,@-r8 */
L8c612cfe: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612d00; /* mov.l r4,@-r8 */
L8c612d00: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612d02; /* pref @r8 */
L8c612d02: r[4]=r[14];goto L8c612d04; /* mov r14,r4 */
L8c612d04: r[4]+=8u;goto L8c612d06; /* add #0x8,r4 */
L8c612d06: r[8]+=60u;goto L8c612d08; /* add #0x3c,r8 */
L8c612d08: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612d0a; /* fmov.s fr5,@-r8 */
L8c612d0a: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c612d0c; /* mov.w @(0x2,r4),r0 */
L8c612d0c: r[3]=r[0];goto L8c612d0e; /* mov r0,r3 */
L8c612d0e: *fpul=r[3];goto L8c612d10; /* lds r3,FPUL */
L8c612d10: r[0]=24u;goto L8c612d12; /* mov #0x18,r0 */
L8c612d12: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612d14; /* float FPUL,fr3 */
L8c612d14: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612d16; /* fmul fr4,fr3 */
L8c612d16: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d18; /* fmov.s fr3,@-r8 */
L8c612d18: r[3]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c612d1a; /* mov.w @r4,r3 */
L8c612d1a: r[4]=r[5];goto L8c612d1c; /* mov r5,r4 */
L8c612d1c: r[4]+=64u;goto L8c612d1e; /* add #0x40,r4 */
L8c612d1e: *fpul=r[3];goto L8c612d20; /* lds r3,FPUL */
L8c612d20: r[3]=rd(m,bus,&io_status,0x8c612d68u,4);if(io_status)return io_status;goto L8c612d22; /* mov.l 0x8c612d68,r3 */
L8c612d22: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612d24; /* float FPUL,fr3 */
L8c612d24: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612d26; /* fmul fr4,fr3 */
L8c612d26: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d28; /* fmov.s fr3,@-r8 */
L8c612d28: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612d2a; /* fmov.s @(r0,r4),fr3 */
L8c612d2a: r[0]=20u;goto L8c612d2c; /* mov #0x14,r0 */
L8c612d2c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d2e; /* fmov.s fr3,@-r8 */
L8c612d2e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612d30; /* fmov.s @(r0,r4),fr3 */
L8c612d30: r[0]=16u;goto L8c612d32; /* mov #0x10,r0 */
L8c612d32: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d34; /* fmov.s fr3,@-r8 */
L8c612d34: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612d36; /* fmov.s @(r0,r4),fr3 */
L8c612d36: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d38; /* fmov.s fr3,@-r8 */
L8c612d38: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c612d3a; /* mov.l r3,@-r8 */
L8c612d3a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612d3c; /* pref @r8 */
L8c612d3c: r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c612d3e; /* mov.l @r7,r2 */
L8c612d3e: r[8]+=32u;goto L8c612d40; /* add #0x20,r8 */
L8c612d40: r[2]+=3u;goto L8c612e50; /* bra 0x8c612e50 */
L8c612d6c: r[6]=r[14];goto L8c612d6e; /* mov r14,r6 */
L8c612d6e: r[8]+=28u;goto L8c612d70; /* add #0x1c,r8 */
L8c612d70: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612d72; /* fmov.s fr5,@-r8 */
L8c612d72: r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c612d74; /* mov.w @(0x2,r6),r0 */
L8c612d74: r[3]=r[0];goto L8c612d76; /* mov r0,r3 */
L8c612d76: *fpul=r[3];goto L8c612d78; /* lds r3,FPUL */
L8c612d78: r[0]=24u;goto L8c612d7a; /* mov #0x18,r0 */
L8c612d7a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612d7c; /* float FPUL,fr3 */
L8c612d7c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612d7e; /* fmul fr4,fr3 */
L8c612d7e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d80; /* fmov.s fr3,@-r8 */
L8c612d80: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c612d82; /* mov.w @r6,r3 */
L8c612d82: r[6]=r[5];goto L8c612d84; /* mov r5,r6 */
L8c612d84: *fpul=r[3];goto L8c612d86; /* lds r3,FPUL */
L8c612d86: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612d88; /* float FPUL,fr3 */
L8c612d88: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612d8a; /* fmul fr4,fr3 */
L8c612d8a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d8c; /* fmov.s fr3,@-r8 */
L8c612d8c: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612d8e; /* fmov.s @(r0,r6),fr3 */
L8c612d8e: r[0]=20u;goto L8c612d90; /* mov #0x14,r0 */
L8c612d90: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d92; /* fmov.s fr3,@-r8 */
L8c612d92: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612d94; /* fmov.s @(r0,r6),fr3 */
L8c612d94: r[0]=16u;goto L8c612d96; /* mov #0x10,r0 */
L8c612d96: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d98; /* fmov.s fr3,@-r8 */
L8c612d98: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612d9a; /* fmov.s @(r0,r6),fr3 */
L8c612d9a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d9c; /* fmov.s fr3,@-r8 */
L8c612d9c: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612d9e; /* mov.l r4,@-r8 */
L8c612d9e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612da0; /* pref @r8 */
L8c612da0: r[6]=r[14];goto L8c612da2; /* mov r14,r6 */
L8c612da2: r[6]+=4u;goto L8c612da4; /* add #0x4,r6 */
L8c612da4: r[8]+=60u;goto L8c612da6; /* add #0x3c,r8 */
L8c612da6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612da8; /* fmov.s fr5,@-r8 */
L8c612da8: r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c612daa; /* mov.w @(0x2,r6),r0 */
L8c612daa: r[3]=r[0];goto L8c612dac; /* mov r0,r3 */
L8c612dac: *fpul=r[3];goto L8c612dae; /* lds r3,FPUL */
L8c612dae: r[0]=24u;goto L8c612db0; /* mov #0x18,r0 */
L8c612db0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612db2; /* float FPUL,fr3 */
L8c612db2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612db4; /* fmul fr4,fr3 */
L8c612db4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612db6; /* fmov.s fr3,@-r8 */
L8c612db6: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c612db8; /* mov.w @r6,r3 */
L8c612db8: r[6]=r[5];goto L8c612dba; /* mov r5,r6 */
L8c612dba: r[6]+=32u;goto L8c612dbc; /* add #0x20,r6 */
L8c612dbc: *fpul=r[3];goto L8c612dbe; /* lds r3,FPUL */
L8c612dbe: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612dc0; /* float FPUL,fr3 */
L8c612dc0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612dc2; /* fmul fr4,fr3 */
L8c612dc2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612dc4; /* fmov.s fr3,@-r8 */
L8c612dc4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612dc6; /* fmov.s @(r0,r6),fr3 */
L8c612dc6: r[0]=20u;goto L8c612dc8; /* mov #0x14,r0 */
L8c612dc8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612dca; /* fmov.s fr3,@-r8 */
L8c612dca: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612dcc; /* fmov.s @(r0,r6),fr3 */
L8c612dcc: r[0]=16u;goto L8c612dce; /* mov #0x10,r0 */
L8c612dce: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612dd0; /* fmov.s fr3,@-r8 */
L8c612dd0: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612dd2; /* fmov.s @(r0,r6),fr3 */
L8c612dd2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612dd4; /* fmov.s fr3,@-r8 */
L8c612dd4: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612dd6; /* mov.l r4,@-r8 */
L8c612dd6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612dd8; /* pref @r8 */
L8c612dd8: r[6]=r[14];goto L8c612dda; /* mov r14,r6 */
L8c612dda: r[6]+=12u;goto L8c612ddc; /* add #0xc,r6 */
L8c612ddc: r[8]+=60u;goto L8c612dde; /* add #0x3c,r8 */
L8c612dde: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612de0; /* fmov.s fr5,@-r8 */
L8c612de0: r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c612de2; /* mov.w @(0x2,r6),r0 */
L8c612de2: r[3]=r[0];goto L8c612de4; /* mov r0,r3 */
L8c612de4: *fpul=r[3];goto L8c612de6; /* lds r3,FPUL */
L8c612de6: r[0]=24u;goto L8c612de8; /* mov #0x18,r0 */
L8c612de8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612dea; /* float FPUL,fr3 */
L8c612dea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612dec; /* fmul fr4,fr3 */
L8c612dec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612dee; /* fmov.s fr3,@-r8 */
L8c612dee: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c612df0; /* mov.w @r6,r3 */
L8c612df0: r[6]=r[5];goto L8c612df2; /* mov r5,r6 */
L8c612df2: r[6]+=96u;goto L8c612df4; /* add #0x60,r6 */
L8c612df4: *fpul=r[3];goto L8c612df6; /* lds r3,FPUL */
L8c612df6: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612df8; /* float FPUL,fr3 */
L8c612df8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612dfa; /* fmul fr4,fr3 */
L8c612dfa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612dfc; /* fmov.s fr3,@-r8 */
L8c612dfc: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612dfe; /* fmov.s @(r0,r6),fr3 */
L8c612dfe: r[0]=20u;goto L8c612e00; /* mov #0x14,r0 */
L8c612e00: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e02; /* fmov.s fr3,@-r8 */
L8c612e02: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612e04; /* fmov.s @(r0,r6),fr3 */
L8c612e04: r[0]=16u;goto L8c612e06; /* mov #0x10,r0 */
L8c612e06: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e08; /* fmov.s fr3,@-r8 */
L8c612e08: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612e0a; /* fmov.s @(r0,r6),fr3 */
L8c612e0a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e0c; /* fmov.s fr3,@-r8 */
L8c612e0c: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612e0e; /* mov.l r4,@-r8 */
L8c612e0e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612e10; /* pref @r8 */
L8c612e10: r[4]=r[14];goto L8c612e12; /* mov r14,r4 */
L8c612e12: r[4]+=8u;goto L8c612e14; /* add #0x8,r4 */
L8c612e14: r[8]+=60u;goto L8c612e16; /* add #0x3c,r8 */
L8c612e16: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612e18; /* fmov.s fr5,@-r8 */
L8c612e18: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c612e1a; /* mov.w @(0x2,r4),r0 */
L8c612e1a: r[3]=r[0];goto L8c612e1c; /* mov r0,r3 */
L8c612e1c: *fpul=r[3];goto L8c612e1e; /* lds r3,FPUL */
L8c612e1e: r[0]=24u;goto L8c612e20; /* mov #0x18,r0 */
L8c612e20: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612e22; /* float FPUL,fr3 */
L8c612e22: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612e24; /* fmul fr4,fr3 */
L8c612e24: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e26; /* fmov.s fr3,@-r8 */
L8c612e26: r[3]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c612e28; /* mov.w @r4,r3 */
L8c612e28: r[4]=r[5];goto L8c612e2a; /* mov r5,r4 */
L8c612e2a: r[4]+=64u;goto L8c612e2c; /* add #0x40,r4 */
L8c612e2c: *fpul=r[3];goto L8c612e2e; /* lds r3,FPUL */
L8c612e2e: r[3]=rd(m,bus,&io_status,0x8c612f2cu,4);if(io_status)return io_status;goto L8c612e30; /* mov.l 0x8c612f2c,r3 */
L8c612e30: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612e32; /* float FPUL,fr3 */
L8c612e32: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612e34; /* fmul fr4,fr3 */
L8c612e34: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e36; /* fmov.s fr3,@-r8 */
L8c612e36: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612e38; /* fmov.s @(r0,r4),fr3 */
L8c612e38: r[0]=20u;goto L8c612e3a; /* mov #0x14,r0 */
L8c612e3a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e3c; /* fmov.s fr3,@-r8 */
L8c612e3c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612e3e; /* fmov.s @(r0,r4),fr3 */
L8c612e3e: r[0]=16u;goto L8c612e40; /* mov #0x10,r0 */
L8c612e40: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e42; /* fmov.s fr3,@-r8 */
L8c612e42: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612e44; /* fmov.s @(r0,r4),fr3 */
L8c612e44: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e46; /* fmov.s fr3,@-r8 */
L8c612e46: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c612e48; /* mov.l r3,@-r8 */
L8c612e48: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612e4a; /* pref @r8 */
L8c612e4a: r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c612e4c; /* mov.l @r7,r2 */
L8c612e4c: r[8]+=32u;goto L8c612e4e; /* add #0x20,r8 */
L8c612e4e: r[2]+=4u;goto L8c612e50; /* add #0x4,r2 */
L8c612e50: wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c612e52; /* mov.l r2,@r7 */
L8c612e52: r[15]+=80u;goto L8c612e54; /* add #0x50,r15 */
L8c612e54: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c612e56; /* fmov.s @r15+,fr12 */
L8c612e56: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612e58; /* mov.l @r15+,r9 */
L8c612e58: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612e5a; /* mov.l @r15+,r10 */
L8c612e5a: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612e5c; /* mov.l @r15+,r11 */
L8c612e5c: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612e5e; /* mov.l @r15+,r12 */
L8c612e5e: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612e60; /* mov.l @r15+,r13 */
L8c612e60: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
