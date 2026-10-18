#include "sa_draw_motion_stop.h"
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
L8c61295a: sa_motion_publish_stop(ctx,0x8c61295au); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61295c; /* mov.l r14,@-r15 */
L8c61295c: sa_motion_publish_stop(ctx,0x8c61295cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61295e; /* mov.l r13,@-r15 */
L8c61295e: sa_motion_publish_stop(ctx,0x8c61295eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c612960; /* mov.l r12,@-r15 */
L8c612960: sa_motion_publish_stop(ctx,0x8c612960u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c612962; /* mov.l r11,@-r15 */
L8c612962: sa_motion_publish_stop(ctx,0x8c612962u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c612964; /* mov.l r10,@-r15 */
L8c612964: sa_motion_publish_stop(ctx,0x8c612964u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c612966; /* mov.l r9,@-r15 */
L8c612966: sa_motion_publish_stop(ctx,0x8c612966u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c612968; /* fmov.s fr12,@-r15 */
L8c612968: sa_motion_publish_stop(ctx,0x8c612968u); r[15]+=4294967216u;goto L8c61296a; /* add #-0x50,r15 */
L8c61296a: sa_motion_publish_stop(ctx,0x8c61296au); r[3]=rd(m,bus,&io_status,0x8c612a64u,4);if(io_status)return io_status;goto L8c61296c; /* mov.l 0x8c612a64,r3 */
L8c61296c: sa_motion_publish_stop(ctx,0x8c61296cu); r[7]=r[15];goto L8c61296e; /* mov r15,r7 */
L8c61296e: sa_motion_publish_stop(ctx,0x8c61296eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c612970; /* fldi1 fr5 */
L8c612970: sa_motion_publish_stop(ctx,0x8c612970u); r[7]+=68u;goto L8c612972; /* add #0x44,r7 */
L8c612972: sa_motion_publish_stop(ctx,0x8c612972u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c612974; /* fmov.s @r3,fr3 */
L8c612974: sa_motion_publish_stop(ctx,0x8c612974u); r[9]=r[15];goto L8c612976; /* mov r15,r9 */
L8c612976: sa_motion_publish_stop(ctx,0x8c612976u); fcopy(f,6u,5u);goto L8c612978; /* fmov fr5,fr6 */
L8c612978: sa_motion_publish_stop(ctx,0x8c612978u); r[2]=r[7];goto L8c61297a; /* mov r7,r2 */
L8c61297a: sa_motion_publish_stop(ctx,0x8c61297au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[3],&f->fpscr,&fr[6]);if(status)return status;}goto L8c61297c; /* fdiv fr3,fr6 */
L8c61297c: sa_motion_publish_stop(ctx,0x8c61297cu); r[9]+=32u;goto L8c61297e; /* add #0x20,r9 */
L8c61297e: sa_motion_publish_stop(ctx,0x8c61297eu); r[0]=0x8c612a6cu;goto L8c612980; /* mova 0x8c612a6c,r0 */
L8c612980: sa_motion_publish_stop(ctx,0x8c612980u); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c612982; /* mov.l r2,@r15 */
L8c612982: sa_motion_publish_stop(ctx,0x8c612982u); wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c612984; /* mov.l r4,@r2 */
L8c612984: sa_motion_publish_stop(ctx,0x8c612984u); r[12]=r[15];goto L8c612986; /* mov r15,r12 */
L8c612986: sa_motion_publish_stop(ctx,0x8c612986u); wr(m,bus,&io_status,r[7]+0x4u,r[5],4);if(io_status)return io_status;goto L8c612988; /* mov.l r5,@(0x4,r7) */
L8c612988: sa_motion_publish_stop(ctx,0x8c612988u); r[11]=r[9];goto L8c61298a; /* mov r9,r11 */
L8c61298a: sa_motion_publish_stop(ctx,0x8c61298au); r[13]=rd(m,bus,&io_status,0x8c612a68u,4);if(io_status)return io_status;goto L8c61298c; /* mov.l 0x8c612a68,r13 */
L8c61298c: sa_motion_publish_stop(ctx,0x8c61298cu); r[2]=0u;goto L8c61298e; /* mov #0x0,r2 */
L8c61298e: sa_motion_publish_stop(ctx,0x8c61298eu); r[5]=rd(m,bus,&io_status,0x8c612a70u,4);if(io_status)return io_status;goto L8c612990; /* mov.l 0x8c612a70,r5 */
L8c612990: sa_motion_publish_stop(ctx,0x8c612990u); r[4]=1u;goto L8c612992; /* mov #0x1,r4 */
L8c612992: sa_motion_publish_stop(ctx,0x8c612992u); wr(m,bus,&io_status,r[7]+0x8u,r[6],4);if(io_status)return io_status;goto L8c612994; /* mov.l r6,@(0x8,r7) */
L8c612994: sa_motion_publish_stop(ctx,0x8c612994u); r[12]+=20u;goto L8c612996; /* add #0x14,r12 */
L8c612996: sa_motion_publish_stop(ctx,0x8c612996u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c612998; /* mov.l r2,@(0x4,r15) */
L8c612998: sa_motion_publish_stop(ctx,0x8c612998u); r[14]=r[11];goto L8c61299a; /* mov r11,r14 */
L8c61299a: sa_motion_publish_stop(ctx,0x8c61299au); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61299c; /* mov.l @r15,r10 */
L8c61299c: sa_motion_publish_stop(ctx,0x8c61299cu); wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61299e; /* mov.l r11,@r15 */
L8c61299e: sa_motion_publish_stop(ctx,0x8c61299eu); fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c6129a0; /* fmov.s @r0,fr4 */
L8c6129a0: sa_motion_publish_stop(ctx,0x8c6129a0u); r[6]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6129a2; /* mov.l @(0x4,r15),r6 */
L8c6129a2: sa_motion_publish_stop(ctx,0x8c6129a2u); r[0]=4u;goto L8c6129a4; /* mov #0x4,r0 */
L8c6129a4: sa_motion_publish_stop(ctx,0x8c6129a4u); r[6]<<=2;goto L8c6129a6; /* shll2 r6 */
L8c6129a6: sa_motion_publish_stop(ctx,0x8c6129a6u); wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c6129a8; /* mov.l r6,@(0x8,r15) */
L8c6129a8: sa_motion_publish_stop(ctx,0x8c6129a8u); r[6]+=r[7];goto L8c6129aa; /* add r7,r6 */
L8c6129aa: sa_motion_publish_stop(ctx,0x8c6129aau); wr(m,bus,&io_status,r[15]+0x10u,r[6],4);if(io_status)return io_status;goto L8c6129ac; /* mov.l r6,@(0x10,r15) */
L8c6129ac: sa_motion_publish_stop(ctx,0x8c6129acu); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6129ae; /* mov.l @(0x8,r15),r3 */
L8c6129ae: sa_motion_publish_stop(ctx,0x8c6129aeu); r[6]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6129b0; /* mov.l @r6,r6 */
L8c6129b0: sa_motion_publish_stop(ctx,0x8c6129b0u); r[3]+=r[12];goto L8c6129b2; /* add r12,r3 */
L8c6129b2: sa_motion_publish_stop(ctx,0x8c6129b2u); wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6129b4; /* mov.l r3,@(0xc,r15) */
L8c6129b4: sa_motion_publish_stop(ctx,0x8c6129b4u); r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6129b6; /* mov.l r3,@-r15 */
L8c6129b6: sa_motion_publish_stop(ctx,0x8c6129b6u); r[2]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c6129b8; /* mov.l @r5,r2 */
L8c6129b8: sa_motion_publish_stop(ctx,0x8c6129b8u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c6129ba; /* fmov.s @(r0,r6),fr0 */
L8c6129ba: sa_motion_publish_stop(ctx,0x8c6129bau); r[0]=16u;goto L8c6129bc; /* mov #0x10,r0 */
L8c6129bc: sa_motion_publish_stop(ctx,0x8c6129bcu); r[3]=r[2];goto L8c6129be; /* mov r2,r3 */
L8c6129be: sa_motion_publish_stop(ctx,0x8c6129beu); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6129c0; /* fmov.s @(r0,r3),fr3 */
L8c6129c0: sa_motion_publish_stop(ctx,0x8c6129c0u); r[1]=r[2];goto L8c6129c2; /* mov r2,r1 */
L8c6129c2: sa_motion_publish_stop(ctx,0x8c6129c2u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c6129c4; /* fmov.s @r1,fr2 */
L8c6129c4: sa_motion_publish_stop(ctx,0x8c6129c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6129c6; /* fmul fr0,fr3 */
L8c6129c6: sa_motion_publish_stop(ctx,0x8c6129c6u); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c6129c8; /* fmov.s @r6,fr0 */
L8c6129c8: sa_motion_publish_stop(ctx,0x8c6129c8u); r[0]=8u;goto L8c6129ca; /* mov #0x8,r0 */
L8c6129ca: sa_motion_publish_stop(ctx,0x8c6129cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6129cc; /* fmac fr0,fr2,fr3 */
L8c6129cc: sa_motion_publish_stop(ctx,0x8c6129ccu); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c6129ce; /* fmov.s @(r0,r6),fr0 */
L8c6129ce: sa_motion_publish_stop(ctx,0x8c6129ceu); r[0]=32u;goto L8c6129d0; /* mov #0x20,r0 */
L8c6129d0: sa_motion_publish_stop(ctx,0x8c6129d0u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6129d2; /* fmov.s @(r0,r2),fr2 */
L8c6129d2: sa_motion_publish_stop(ctx,0x8c6129d2u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6129d4; /* mov.l @r15+,r2 */
L8c6129d4: sa_motion_publish_stop(ctx,0x8c6129d4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6129d6; /* fmac fr0,fr2,fr3 */
L8c6129d6: sa_motion_publish_stop(ctx,0x8c6129d6u); fcopy(f,0u,4u);goto L8c6129d8; /* fmov fr4,fr0 */
L8c6129d8: sa_motion_publish_stop(ctx,0x8c6129d8u); fcopy(f,2u,4u);goto L8c6129da; /* fmov fr4,fr2 */
L8c6129da: sa_motion_publish_stop(ctx,0x8c6129dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6129dc; /* fmac fr0,fr3,fr2 */
L8c6129dc: sa_motion_publish_stop(ctx,0x8c6129dcu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c6129de; /* ftrc fr2,FPUL */
L8c6129de: sa_motion_publish_stop(ctx,0x8c6129deu); r[3]=*fpul;goto L8c6129e0; /* sts FPUL,r3 */
L8c6129e0: sa_motion_publish_stop(ctx,0x8c6129e0u); wr(m,bus,&io_status,r[2],r[3],2);if(io_status)return io_status;goto L8c6129e2; /* mov.w r3,@r2 */
L8c6129e2: sa_motion_publish_stop(ctx,0x8c6129e2u); r[0]=36u;goto L8c6129e4; /* mov #0x24,r0 */
L8c6129e4: sa_motion_publish_stop(ctx,0x8c6129e4u); r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c6129e6; /* mov.l @r5,r3 */
L8c6129e6: sa_motion_publish_stop(ctx,0x8c6129e6u); r[1]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6129e8; /* mov.l @(0xc,r15),r1 */
L8c6129e8: sa_motion_publish_stop(ctx,0x8c6129e8u); fload(m,bus,&io_status,f,1u,r[0]+r[3]);if(io_status)return io_status;goto L8c6129ea; /* fmov.s @(r0,r3),fr1 */
L8c6129ea: sa_motion_publish_stop(ctx,0x8c6129eau); r[0]=4u;goto L8c6129ec; /* mov #0x4,r0 */
L8c6129ec: sa_motion_publish_stop(ctx,0x8c6129ecu); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c6129ee; /* fmov.s @(r0,r6),fr0 */
L8c6129ee: sa_motion_publish_stop(ctx,0x8c6129eeu); r[0]=20u;goto L8c6129f0; /* mov #0x14,r0 */
L8c6129f0: sa_motion_publish_stop(ctx,0x8c6129f0u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6129f2; /* fmov.s @(r0,r3),fr3 */
L8c6129f2: sa_motion_publish_stop(ctx,0x8c6129f2u); r[0]=4u;goto L8c6129f4; /* mov #0x4,r0 */
L8c6129f4: sa_motion_publish_stop(ctx,0x8c6129f4u); r[2]=r[3];goto L8c6129f6; /* mov r3,r2 */
L8c6129f6: sa_motion_publish_stop(ctx,0x8c6129f6u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6129f8; /* fmov.s @(r0,r2),fr2 */
L8c6129f8: sa_motion_publish_stop(ctx,0x8c6129f8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6129fa; /* fmul fr0,fr3 */
L8c6129fa: sa_motion_publish_stop(ctx,0x8c6129fau); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c6129fc; /* fmov.s @r6,fr0 */
L8c6129fc: sa_motion_publish_stop(ctx,0x8c6129fcu); r[0]=8u;goto L8c6129fe; /* mov #0x8,r0 */
L8c6129fe: sa_motion_publish_stop(ctx,0x8c6129feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612a00; /* fmac fr0,fr2,fr3 */
L8c612a00: sa_motion_publish_stop(ctx,0x8c612a00u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c612a02; /* fmov.s @(r0,r6),fr0 */
L8c612a02: sa_motion_publish_stop(ctx,0x8c612a02u); fcopy(f,2u,4u);goto L8c612a04; /* fmov fr4,fr2 */
L8c612a04: sa_motion_publish_stop(ctx,0x8c612a04u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612a06; /* fmac fr0,fr1,fr3 */
L8c612a06: sa_motion_publish_stop(ctx,0x8c612a06u); fcopy(f,0u,4u);goto L8c612a08; /* fmov fr4,fr0 */
L8c612a08: sa_motion_publish_stop(ctx,0x8c612a08u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612a0a; /* fmac fr0,fr3,fr2 */
L8c612a0a: sa_motion_publish_stop(ctx,0x8c612a0au); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c612a0c; /* ftrc fr2,FPUL */
L8c612a0c: sa_motion_publish_stop(ctx,0x8c612a0cu); r[0]=*fpul;goto L8c612a0e; /* sts FPUL,r0 */
L8c612a0e: sa_motion_publish_stop(ctx,0x8c612a0eu); wr(m,bus,&io_status,r[1]+0x2u,r[0],2);if(io_status)return io_status;goto L8c612a10; /* mov.w r0,@(0x2,r1) */
L8c612a10: sa_motion_publish_stop(ctx,0x8c612a10u); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c612a12; /* mov.l @(0x10,r15),r3 */
L8c612a12: sa_motion_publish_stop(ctx,0x8c612a12u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c612a14; /* mov.l @r3,r2 */
L8c612a14: sa_motion_publish_stop(ctx,0x8c612a14u); r[1]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612a16; /* mov.l @(0xc,r2),r1 */
L8c612a16: sa_motion_publish_stop(ctx,0x8c612a16u); *sr=(*sr&~1u)|!(r[4]&r[1]);goto L8c612a18; /* tst r4,r1 */
L8c612a18: sa_motion_publish_stop(ctx,0x8c612a18u); if(!(*sr&1u))goto L8c612a7c;goto L8c612a1a; /* bf 0x8c612a7c */
L8c612a1a: sa_motion_publish_stop(ctx,0x8c612a1au); r[6]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612a1c; /* mov.l @(0x4,r15),r6 */
L8c612a1c: sa_motion_publish_stop(ctx,0x8c612a1cu); r[0]=24u;goto L8c612a1e; /* mov #0x18,r0 */
L8c612a1e: sa_motion_publish_stop(ctx,0x8c612a1eu); fcopy(f,7u,5u);goto L8c612a20; /* fmov fr5,fr7 */
L8c612a20: sa_motion_publish_stop(ctx,0x8c612a20u); r[6]<<=2;goto L8c612a22; /* shll2 r6 */
L8c612a22: sa_motion_publish_stop(ctx,0x8c612a22u); r[3]=rd(m,bus,&io_status,0x8c612a74u,4);if(io_status)return io_status;goto L8c612a24; /* mov.l 0x8c612a74,r3 */
L8c612a24: sa_motion_publish_stop(ctx,0x8c612a24u); r[6]+=r[7];goto L8c612a26; /* add r7,r6 */
L8c612a26: sa_motion_publish_stop(ctx,0x8c612a26u); wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c612a28; /* mov.l r6,@(0x8,r15) */
L8c612a28: sa_motion_publish_stop(ctx,0x8c612a28u); r[6]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c612a2a; /* mov.l @r6,r6 */
L8c612a2a: sa_motion_publish_stop(ctx,0x8c612a2au); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612a2c; /* fmov.s @(r0,r6),fr3 */
L8c612a2c: sa_motion_publish_stop(ctx,0x8c612a2cu); r[0]=12u;goto L8c612a2e; /* mov #0xc,r0 */
L8c612a2e: sa_motion_publish_stop(ctx,0x8c612a2eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[7],&fr[3],&f->fpscr,&fr[7]);if(status)return status;}goto L8c612a30; /* fdiv fr3,fr7 */
L8c612a30: sa_motion_publish_stop(ctx,0x8c612a30u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c612a32; /* fmov.s @(r0,r13),fr3 */
L8c612a32: sa_motion_publish_stop(ctx,0x8c612a32u); r[0]=16u;goto L8c612a34; /* mov #0x10,r0 */
L8c612a34: sa_motion_publish_stop(ctx,0x8c612a34u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c612a36; /* fmov.s @(r0,r6),fr2 */
L8c612a36: sa_motion_publish_stop(ctx,0x8c612a36u); r[0]=16u;goto L8c612a38; /* mov #0x10,r0 */
L8c612a38: sa_motion_publish_stop(ctx,0x8c612a38u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612a3a; /* fsub fr3,fr2 */
L8c612a3a: sa_motion_publish_stop(ctx,0x8c612a3au); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c612a3c; /* fmov.s @r3,fr3 */
L8c612a3c: sa_motion_publish_stop(ctx,0x8c612a3cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612a3e; /* fmul fr7,fr2 */
L8c612a3e: sa_motion_publish_stop(ctx,0x8c612a3eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612a40; /* fdiv fr3,fr2 */
L8c612a40: sa_motion_publish_stop(ctx,0x8c612a40u); fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c612a42; /* fmov.s fr2,@r14 */
L8c612a42: sa_motion_publish_stop(ctx,0x8c612a42u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612a44; /* mov.l @(0x8,r15),r2 */
L8c612a44: sa_motion_publish_stop(ctx,0x8c612a44u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c612a46; /* fmov.s @(r0,r13),fr3 */
L8c612a46: sa_motion_publish_stop(ctx,0x8c612a46u); r[0]=20u;goto L8c612a48; /* mov #0x14,r0 */
L8c612a48: sa_motion_publish_stop(ctx,0x8c612a48u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c612a4a; /* mov.l @r2,r1 */
L8c612a4a: sa_motion_publish_stop(ctx,0x8c612a4au); r[2]=rd(m,bus,&io_status,0x8c612a78u,4);if(io_status)return io_status;goto L8c612a4c; /* mov.l 0x8c612a78,r2 */
L8c612a4c: sa_motion_publish_stop(ctx,0x8c612a4cu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c612a4e; /* fmov.s @(r0,r1),fr2 */
L8c612a4e: sa_motion_publish_stop(ctx,0x8c612a4eu); r[0]=4u;goto L8c612a50; /* mov #0x4,r0 */
L8c612a50: sa_motion_publish_stop(ctx,0x8c612a50u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612a52; /* fsub fr3,fr2 */
L8c612a52: sa_motion_publish_stop(ctx,0x8c612a52u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c612a54; /* fmov.s @r2,fr3 */
L8c612a54: sa_motion_publish_stop(ctx,0x8c612a54u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612a56; /* fmul fr7,fr2 */
L8c612a56: sa_motion_publish_stop(ctx,0x8c612a56u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612a58; /* fdiv fr3,fr2 */
L8c612a58: sa_motion_publish_stop(ctx,0x8c612a58u); fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c612a5a; /* fmov fr2,@(r0,r14) */
L8c612a5a: sa_motion_publish_stop(ctx,0x8c612a5au); r[0]=8u;goto L8c612a5c; /* mov #0x8,r0 */
L8c612a5c: sa_motion_publish_stop(ctx,0x8c612a5eu); fstore(m,bus,&io_status,f,7u,r[0]+r[14]);if(io_status)return io_status;goto L8c612a98; /* bra 0x8c612a98 */
L8c612a7c: sa_motion_publish_stop(ctx,0x8c612a7cu); r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c612a7e; /* mov.l @r10,r3 */
L8c612a7e: sa_motion_publish_stop(ctx,0x8c612a7eu); r[0]=16u;goto L8c612a80; /* mov #0x10,r0 */
L8c612a80: sa_motion_publish_stop(ctx,0x8c612a80u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612a82; /* fmov.s @(r0,r3),fr3 */
L8c612a82: sa_motion_publish_stop(ctx,0x8c612a82u); r[0]=20u;goto L8c612a84; /* mov #0x14,r0 */
L8c612a84: sa_motion_publish_stop(ctx,0x8c612a84u); fstore(m,bus,&io_status,f,3u,r[11]);if(io_status)return io_status;goto L8c612a86; /* fmov.s fr3,@r11 */
L8c612a86: sa_motion_publish_stop(ctx,0x8c612a86u); r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c612a88; /* mov.l @r10,r3 */
L8c612a88: sa_motion_publish_stop(ctx,0x8c612a88u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612a8a; /* fmov.s @(r0,r3),fr3 */
L8c612a8a: sa_motion_publish_stop(ctx,0x8c612a8au); r[0]=4u;goto L8c612a8c; /* mov #0x4,r0 */
L8c612a8c: sa_motion_publish_stop(ctx,0x8c612a8cu); fstore(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c612a8e; /* fmov fr3,@(r0,r11) */
L8c612a8e: sa_motion_publish_stop(ctx,0x8c612a8eu); r[0]=24u;goto L8c612a90; /* mov #0x18,r0 */
L8c612a90: sa_motion_publish_stop(ctx,0x8c612a90u); r[3]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;goto L8c612a92; /* mov.l @r10,r3 */
L8c612a92: sa_motion_publish_stop(ctx,0x8c612a92u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612a94; /* fmov.s @(r0,r3),fr3 */
L8c612a94: sa_motion_publish_stop(ctx,0x8c612a94u); r[0]=8u;goto L8c612a96; /* mov #0x8,r0 */
L8c612a96: sa_motion_publish_stop(ctx,0x8c612a96u); fstore(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c612a98; /* fmov fr3,@(r0,r11) */
L8c612a98: sa_motion_publish_stop(ctx,0x8c612a98u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612a9a; /* mov.l @(0x4,r15),r3 */
L8c612a9a: sa_motion_publish_stop(ctx,0x8c612a9au); r[2]=3u;goto L8c612a9c; /* mov #0x3,r2 */
L8c612a9c: sa_motion_publish_stop(ctx,0x8c612a9cu); r[11]+=12u;goto L8c612a9e; /* add #0xc,r11 */
L8c612a9e: sa_motion_publish_stop(ctx,0x8c612a9eu); r[3]+=1u;goto L8c612aa0; /* add #0x1,r3 */
L8c612aa0: sa_motion_publish_stop(ctx,0x8c612aa0u); *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c612aa2; /* cmp/ge r2,r3 */
L8c612aa2: sa_motion_publish_stop(ctx,0x8c612aa2u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c612aa4; /* mov.l r3,@(0x4,r15) */
L8c612aa4: sa_motion_publish_stop(ctx,0x8c612aa4u); r[10]+=4u;goto L8c612aa6; /* add #0x4,r10 */
L8c612aa6: sa_motion_publish_stop(ctx,0x8c612aa8u); take=(*sr&1u);r[14]+=12u;if(take)goto L8c612aae;goto L8c612aaa; /* bt/s 0x8c612aae */
L8c612aaa: sa_motion_publish_stop(ctx,0x8c612aacu); goto L8c6129a0; /* bra 0x8c6129a0 */
L8c612aae: sa_motion_publish_stop(ctx,0x8c612aaeu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612ab0; /* mov.l @r15,r3 */
L8c612ab0: sa_motion_publish_stop(ctx,0x8c612ab0u); r[0]=8u;goto L8c612ab2; /* mov #0x8,r0 */
L8c612ab2: sa_motion_publish_stop(ctx,0x8c612ab2u); r[6]=r[9];goto L8c612ab4; /* mov r9,r6 */
L8c612ab4: sa_motion_publish_stop(ctx,0x8c612ab4u); r[6]+=12u;goto L8c612ab6; /* add #0xc,r6 */
L8c612ab6: sa_motion_publish_stop(ctx,0x8c612ab6u); fload(m,bus,&io_status,f,8u,r[0]+r[3]);if(io_status)return io_status;goto L8c612ab8; /* fmov.s @(r0,r3),fr8 */
L8c612ab8: sa_motion_publish_stop(ctx,0x8c612ab8u); r[0]=4u;goto L8c612aba; /* mov #0x4,r0 */
L8c612aba: sa_motion_publish_stop(ctx,0x8c612abau); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c612abc; /* fmov.s @(r0,r6),fr4 */
L8c612abc: sa_motion_publish_stop(ctx,0x8c612abcu); r[5]=r[9];goto L8c612abe; /* mov r9,r5 */
L8c612abe: sa_motion_publish_stop(ctx,0x8c612abeu); fload(m,bus,&io_status,f,10u,r[0]+r[3]);if(io_status)return io_status;goto L8c612ac0; /* fmov.s @(r0,r3),fr10 */
L8c612ac0: sa_motion_publish_stop(ctx,0x8c612ac0u); r[0]=8u;goto L8c612ac2; /* mov #0x8,r0 */
L8c612ac2: sa_motion_publish_stop(ctx,0x8c612ac2u); fload(m,bus,&io_status,f,7u,r[0]+r[6]);if(io_status)return io_status;goto L8c612ac4; /* fmov.s @(r0,r6),fr7 */
L8c612ac4: sa_motion_publish_stop(ctx,0x8c612ac4u); r[0]=4u;goto L8c612ac6; /* mov #0x4,r0 */
L8c612ac6: sa_motion_publish_stop(ctx,0x8c612ac6u); fcopy(f,3u,10u);goto L8c612ac8; /* fmov fr10,fr3 */
L8c612ac8: sa_motion_publish_stop(ctx,0x8c612ac8u); r[5]+=24u;goto L8c612aca; /* add #0x18,r5 */
L8c612aca: sa_motion_publish_stop(ctx,0x8c612acau); fcopy(f,0u,8u);goto L8c612acc; /* fmov fr8,fr0 */
L8c612acc: sa_motion_publish_stop(ctx,0x8c612accu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612ace; /* fmul fr7,fr3 */
L8c612ace: sa_motion_publish_stop(ctx,0x8c612aceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c612ad0; /* fmul fr4,fr0 */
L8c612ad0: sa_motion_publish_stop(ctx,0x8c612ad0u); fload(m,bus,&io_status,f,11u,r[3]);if(io_status)return io_status;goto L8c612ad2; /* fmov.s @r3,fr11 */
L8c612ad2: sa_motion_publish_stop(ctx,0x8c612ad2u); fload(m,bus,&io_status,f,9u,r[6]);if(io_status)return io_status;goto L8c612ad4; /* fmov.s @r6,fr9 */
L8c612ad4: sa_motion_publish_stop(ctx,0x8c612ad4u); fcopy(f,2u,8u);goto L8c612ad6; /* fmov fr8,fr2 */
L8c612ad6: sa_motion_publish_stop(ctx,0x8c612ad6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[9],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612ad8; /* fmul fr9,fr2 */
L8c612ad8: sa_motion_publish_stop(ctx,0x8c612ad8u); fcopy(f,1u,9u);goto L8c612ada; /* fmov fr9,fr1 */
L8c612ada: sa_motion_publish_stop(ctx,0x8c612adau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c612adc; /* fsub fr3,fr0 */
L8c612adc: sa_motion_publish_stop(ctx,0x8c612adcu); fcopy(f,3u,11u);goto L8c612ade; /* fmov fr11,fr3 */
L8c612ade: sa_motion_publish_stop(ctx,0x8c612adeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612ae0; /* fmul fr7,fr3 */
L8c612ae0: sa_motion_publish_stop(ctx,0x8c612ae0u); fcopy(f,12u,11u);goto L8c612ae2; /* fmov fr11,fr12 */
L8c612ae2: sa_motion_publish_stop(ctx,0x8c612ae2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[10],&f->fpscr,&fr[1]);if(status)return status;}goto L8c612ae4; /* fmul fr10,fr1 */
L8c612ae4: sa_motion_publish_stop(ctx,0x8c612ae4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612ae6; /* fsub fr2,fr3 */
L8c612ae6: sa_motion_publish_stop(ctx,0x8c612ae6u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c612ae8; /* fmov.s @(r0,r5),fr2 */
L8c612ae8: sa_motion_publish_stop(ctx,0x8c612ae8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612aea; /* fmul fr3,fr2 */
L8c612aea: sa_motion_publish_stop(ctx,0x8c612aeau); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c612aec; /* fmov.s @r5,fr3 */
L8c612aec: sa_motion_publish_stop(ctx,0x8c612aecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612aee; /* fmac fr0,fr3,fr2 */
L8c612aee: sa_motion_publish_stop(ctx,0x8c612aeeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[4],&f->fpscr,&fr[12]);if(status)return status;}goto L8c612af0; /* fmul fr4,fr12 */
L8c612af0: sa_motion_publish_stop(ctx,0x8c612af0u); r[0]=8u;goto L8c612af2; /* mov #0x8,r0 */
L8c612af2: sa_motion_publish_stop(ctx,0x8c612af2u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c612af4; /* fmov.s @(r0,r5),fr3 */
L8c612af4: sa_motion_publish_stop(ctx,0x8c612af4u); r[5]=rd(m,bus,&io_status,0x8c612d44u,4);if(io_status)return io_status;goto L8c612af6; /* mov.l 0x8c612d44,r5 */
L8c612af6: sa_motion_publish_stop(ctx,0x8c612af6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[12],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c612af8; /* fsub fr12,fr1 */
L8c612af8: sa_motion_publish_stop(ctx,0x8c612af8u); r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c612afa; /* mov.l @r5,r3 */
L8c612afa: sa_motion_publish_stop(ctx,0x8c612afau); fcopy(f,0u,1u);goto L8c612afc; /* fmov fr1,fr0 */
L8c612afc: sa_motion_publish_stop(ctx,0x8c612afcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612afe; /* fmac fr0,fr3,fr2 */
L8c612afe: sa_motion_publish_stop(ctx,0x8c612afeu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c612b00; /* fldi0 fr3 */
L8c612b00: sa_motion_publish_stop(ctx,0x8c612b00u); fcopy(f,4u,2u);goto L8c612b02; /* fmov fr2,fr4 */
L8c612b02: sa_motion_publish_stop(ctx,0x8c612b02u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c612b04; /* fcmp/gt fr4,fr3 */
L8c612b04: sa_motion_publish_stop(ctx,0x8c612b04u); r[0]=*sr&1u;goto L8c612b06; /* movt r0 */
L8c612b06: sa_motion_publish_stop(ctx,0x8c612b06u); *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c612b08; /* tst r3,r0 */
L8c612b08: sa_motion_publish_stop(ctx,0x8c612b08u); if((*sr&1u))goto L8c612b0e;goto L8c612b0a; /* bt 0x8c612b0e */
L8c612b0a: sa_motion_publish_stop(ctx,0x8c612b0cu); goto L8c612e52; /* bra 0x8c612e52 */
L8c612b0e: sa_motion_publish_stop(ctx,0x8c612b0eu); r[1]=0u;goto L8c612b10; /* mov #0x0,r1 */
L8c612b10: sa_motion_publish_stop(ctx,0x8c612b10u); r[14]=rd(m,bus,&io_status,0x8c612d48u,4);if(io_status)return io_status;goto L8c612b12; /* mov.l 0x8c612d48,r14 */
L8c612b12: sa_motion_publish_stop(ctx,0x8c612b12u); wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c612b14; /* mov.l r1,@r5 */
L8c612b14: sa_motion_publish_stop(ctx,0x8c612b14u); r[3]=2u;goto L8c612b16; /* mov #0x2,r3 */
L8c612b16: sa_motion_publish_stop(ctx,0x8c612b16u); r[5]=rd(m,bus,&io_status,0x8c612d4cu,4);if(io_status)return io_status;goto L8c612b18; /* mov.l 0x8c612d4c,r5 */
L8c612b18: sa_motion_publish_stop(ctx,0x8c612b18u); r[6]=r[1];goto L8c612b1a; /* mov r1,r6 */
L8c612b1a: sa_motion_publish_stop(ctx,0x8c612b1au); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c612b1c; /* mov.l r3,@r15 */
L8c612b1c: sa_motion_publish_stop(ctx,0x8c612b1cu); r[10]=r[6];goto L8c612b1e; /* mov r6,r10 */
L8c612b1e: sa_motion_publish_stop(ctx,0x8c612b1eu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612b20; /* mov.l @r15,r3 */
L8c612b20: sa_motion_publish_stop(ctx,0x8c612b20u); r[0]=r[7];goto L8c612b22; /* mov r7,r0 */
L8c612b22: sa_motion_publish_stop(ctx,0x8c612b22u); r[11]=r[10];goto L8c612b24; /* mov r10,r11 */
L8c612b24: sa_motion_publish_stop(ctx,0x8c612b24u); r[11]<<=2;goto L8c612b26; /* shll2 r11 */
L8c612b26: sa_motion_publish_stop(ctx,0x8c612b26u); r[3]<<=2;goto L8c612b28; /* shll2 r3 */
L8c612b28: sa_motion_publish_stop(ctx,0x8c612b28u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c612b2a; /* mov.l r3,@(0x4,r15) */
L8c612b2a: sa_motion_publish_stop(ctx,0x8c612b2au); r[2]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c612b2c; /* mov.l @(r0,r3),r2 */
L8c612b2c: sa_motion_publish_stop(ctx,0x8c612b2cu); r[3]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612b2e; /* mov.l @(0xc,r2),r3 */
L8c612b2e: sa_motion_publish_stop(ctx,0x8c612b2eu); r[2]=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;goto L8c612b30; /* mov.l @(r0,r11),r2 */
L8c612b30: sa_motion_publish_stop(ctx,0x8c612b30u); r[3]&=r[4];goto L8c612b32; /* and r4,r3 */
L8c612b32: sa_motion_publish_stop(ctx,0x8c612b32u); if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[2]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612b34; /* mov.l @(0xc,r2),r2 */
L8c612b34: sa_motion_publish_stop(ctx,0x8c612b34u); r[2]&=r[4];goto L8c612b36; /* and r4,r2 */
L8c612b36: sa_motion_publish_stop(ctx,0x8c612b36u); r[3]^=r[2];goto L8c612b38; /* xor r2,r3 */
L8c612b38: sa_motion_publish_stop(ctx,0x8c612b38u); r[0]=r[3];goto L8c612b3a; /* mov r3,r0 */
L8c612b3a: sa_motion_publish_stop(ctx,0x8c612b3au); *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c612b3c; /* cmp/eq #0x1,r0 */
L8c612b3c: sa_motion_publish_stop(ctx,0x8c612b3cu); if(!(*sr&1u))goto L8c612c22;goto L8c612b3e; /* bf 0x8c612c22 */
L8c612b3e: sa_motion_publish_stop(ctx,0x8c612b3eu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612b40; /* mov.l @r15,r2 */
L8c612b40: sa_motion_publish_stop(ctx,0x8c612b40u); r[0]=8u;goto L8c612b42; /* mov #0x8,r0 */
L8c612b42: sa_motion_publish_stop(ctx,0x8c612b42u); r[1]=r[10];goto L8c612b44; /* mov r10,r1 */
L8c612b44: sa_motion_publish_stop(ctx,0x8c612b44u); r[3]=r[2];goto L8c612b46; /* mov r2,r3 */
L8c612b46: sa_motion_publish_stop(ctx,0x8c612b46u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612b48; /* shll r2 */
L8c612b48: sa_motion_publish_stop(ctx,0x8c612b48u); r[2]+=r[3];goto L8c612b4a; /* add r3,r2 */
L8c612b4a: sa_motion_publish_stop(ctx,0x8c612b4au); r[3]=rd(m,bus,&io_status,0x8c612d50u,4);if(io_status)return io_status;goto L8c612b4c; /* mov.l 0x8c612d50,r3 */
L8c612b4c: sa_motion_publish_stop(ctx,0x8c612b4cu); r[2]<<=2;goto L8c612b4e; /* shll2 r2 */
L8c612b4e: sa_motion_publish_stop(ctx,0x8c612b4eu); r[2]+=r[9];goto L8c612b50; /* add r9,r2 */
L8c612b50: sa_motion_publish_stop(ctx,0x8c612b50u); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c612b52; /* mov.l r2,@(0xc,r15) */
L8c612b52: sa_motion_publish_stop(ctx,0x8c612b52u); fload(m,bus,&io_status,f,4u,r[0]+r[2]);if(io_status)return io_status;goto L8c612b54; /* fmov.s @(r0,r2),fr4 */
L8c612b54: sa_motion_publish_stop(ctx,0x8c612b54u); r[2]=r[10];goto L8c612b56; /* mov r10,r2 */
L8c612b56: sa_motion_publish_stop(ctx,0x8c612b56u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612b58; /* shll r2 */
L8c612b58: sa_motion_publish_stop(ctx,0x8c612b58u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c612b5a; /* fmov.s @r3,fr3 */
L8c612b5a: sa_motion_publish_stop(ctx,0x8c612b5au); r[2]+=r[1];goto L8c612b5c; /* add r1,r2 */
L8c612b5c: sa_motion_publish_stop(ctx,0x8c612b5cu); r[2]<<=2;goto L8c612b5e; /* shll2 r2 */
L8c612b5e: sa_motion_publish_stop(ctx,0x8c612b5eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612b60; /* fsub fr4,fr3 */
L8c612b60: sa_motion_publish_stop(ctx,0x8c612b60u); r[0]=8u;goto L8c612b62; /* mov #0x8,r0 */
L8c612b62: sa_motion_publish_stop(ctx,0x8c612b62u); r[2]+=r[9];goto L8c612b64; /* add r9,r2 */
L8c612b64: sa_motion_publish_stop(ctx,0x8c612b64u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c612b66; /* mov.l r2,@(0x8,r15) */
L8c612b66: sa_motion_publish_stop(ctx,0x8c612b66u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c612b68; /* fmov.s @(r0,r2),fr2 */
L8c612b68: sa_motion_publish_stop(ctx,0x8c612b68u); r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612b6a; /* mov.l @(0x8,r15),r1 */
L8c612b6a: sa_motion_publish_stop(ctx,0x8c612b6au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612b6c; /* fsub fr4,fr2 */
L8c612b6c: sa_motion_publish_stop(ctx,0x8c612b6cu); fcopy(f,4u,3u);goto L8c612b6e; /* fmov fr3,fr4 */
L8c612b6e: sa_motion_publish_stop(ctx,0x8c612b6eu); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c612b70; /* mov.l @(0xc,r15),r2 */
L8c612b70: sa_motion_publish_stop(ctx,0x8c612b70u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c612b72; /* fmov.s @r1,fr3 */
L8c612b72: sa_motion_publish_stop(ctx,0x8c612b72u); fload(m,bus,&io_status,f,7u,r[2]);if(io_status)return io_status;goto L8c612b74; /* fmov.s @r2,fr7 */
L8c612b74: sa_motion_publish_stop(ctx,0x8c612b74u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c612b76; /* fdiv fr2,fr4 */
L8c612b76: sa_motion_publish_stop(ctx,0x8c612b76u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612b78; /* fsub fr7,fr3 */
L8c612b78: sa_motion_publish_stop(ctx,0x8c612b78u); fcopy(f,2u,7u);goto L8c612b7a; /* fmov fr7,fr2 */
L8c612b7a: sa_motion_publish_stop(ctx,0x8c612b7au); fcopy(f,0u,4u);goto L8c612b7c; /* fmov fr4,fr0 */
L8c612b7c: sa_motion_publish_stop(ctx,0x8c612b7cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612b7e; /* fmac fr0,fr3,fr2 */
L8c612b7e: sa_motion_publish_stop(ctx,0x8c612b7eu); r[1]=rd(m,bus,&io_status,0x8c612d54u,4);if(io_status)return io_status;goto L8c612b80; /* mov.l 0x8c612d54,r1 */
L8c612b80: sa_motion_publish_stop(ctx,0x8c612b80u); r[0]=12u;goto L8c612b82; /* mov #0xc,r0 */
L8c612b82: sa_motion_publish_stop(ctx,0x8c612b82u); fcopy(f,7u,2u);goto L8c612b84; /* fmov fr2,fr7 */
L8c612b84: sa_motion_publish_stop(ctx,0x8c612b84u); r[2]=r[6];goto L8c612b86; /* mov r6,r2 */
L8c612b86: sa_motion_publish_stop(ctx,0x8c612b86u); fcopy(f,0u,6u);goto L8c612b88; /* fmov fr6,fr0 */
L8c612b88: sa_motion_publish_stop(ctx,0x8c612b88u); r[2]<<=2;goto L8c612b8a; /* shll2 r2 */
L8c612b8a: sa_motion_publish_stop(ctx,0x8c612b8au); r[2]<<=2;goto L8c612b8c; /* shll2 r2 */
L8c612b8c: sa_motion_publish_stop(ctx,0x8c612b8cu); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612b8e; /* shll r2 */
L8c612b8e: sa_motion_publish_stop(ctx,0x8c612b8eu); r[2]+=r[5];goto L8c612b90; /* add r5,r2 */
L8c612b90: sa_motion_publish_stop(ctx,0x8c612b90u); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c612b92; /* mov.l r2,@r15 */
L8c612b92: sa_motion_publish_stop(ctx,0x8c612b92u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c612b94; /* fmov.s @r1,fr3 */
L8c612b94: sa_motion_publish_stop(ctx,0x8c612b94u); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c612b96; /* fmov.s @(r0,r13),fr2 */
L8c612b96: sa_motion_publish_stop(ctx,0x8c612b96u); r[0]=16u;goto L8c612b98; /* mov #0x10,r0 */
L8c612b98: sa_motion_publish_stop(ctx,0x8c612b98u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612b9a; /* fmul fr7,fr3 */
L8c612b9a: sa_motion_publish_stop(ctx,0x8c612b9au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612b9c; /* fmac fr0,fr3,fr2 */
L8c612b9c: sa_motion_publish_stop(ctx,0x8c612b9cu); fcopy(f,0u,4u);goto L8c612b9e; /* fmov fr4,fr0 */
L8c612b9e: sa_motion_publish_stop(ctx,0x8c612b9eu); fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c612ba0; /* fmov fr2,@(r0,r2) */
L8c612ba0: sa_motion_publish_stop(ctx,0x8c612ba0u); r[0]=4u;goto L8c612ba2; /* mov #0x4,r0 */
L8c612ba2: sa_motion_publish_stop(ctx,0x8c612ba2u); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c612ba4; /* mov.l @(0xc,r15),r2 */
L8c612ba4: sa_motion_publish_stop(ctx,0x8c612ba4u); fload(m,bus,&io_status,f,7u,r[0]+r[2]);if(io_status)return io_status;goto L8c612ba6; /* fmov.s @(r0,r2),fr7 */
L8c612ba6: sa_motion_publish_stop(ctx,0x8c612ba6u); r[0]=4u;goto L8c612ba8; /* mov #0x4,r0 */
L8c612ba8: sa_motion_publish_stop(ctx,0x8c612ba8u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612baa; /* mov.l @(0x8,r15),r2 */
L8c612baa: sa_motion_publish_stop(ctx,0x8c612baau); fcopy(f,2u,7u);goto L8c612bac; /* fmov fr7,fr2 */
L8c612bac: sa_motion_publish_stop(ctx,0x8c612bacu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612bae; /* fmov.s @(r0,r2),fr3 */
L8c612bae: sa_motion_publish_stop(ctx,0x8c612baeu); r[0]=rd(m,bus,&io_status,0x8c612d58u,4);if(io_status)return io_status;goto L8c612bb0; /* mov.l 0x8c612d58,r0 */
L8c612bb0: sa_motion_publish_stop(ctx,0x8c612bb0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612bb2; /* fsub fr7,fr3 */
L8c612bb2: sa_motion_publish_stop(ctx,0x8c612bb2u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612bb4; /* mov.l @r15,r2 */
L8c612bb4: sa_motion_publish_stop(ctx,0x8c612bb4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612bb6; /* fmac fr0,fr3,fr2 */
L8c612bb6: sa_motion_publish_stop(ctx,0x8c612bb6u); fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c612bb8; /* fmov.s @r0,fr3 */
L8c612bb8: sa_motion_publish_stop(ctx,0x8c612bb8u); r[0]=16u;goto L8c612bba; /* mov #0x10,r0 */
L8c612bba: sa_motion_publish_stop(ctx,0x8c612bbau); fcopy(f,7u,2u);goto L8c612bbc; /* fmov fr2,fr7 */
L8c612bbc: sa_motion_publish_stop(ctx,0x8c612bbcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612bbe; /* fmul fr7,fr3 */
L8c612bbe: sa_motion_publish_stop(ctx,0x8c612bbeu); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c612bc0; /* fmov.s @(r0,r13),fr2 */
L8c612bc0: sa_motion_publish_stop(ctx,0x8c612bc0u); r[0]=20u;goto L8c612bc2; /* mov #0x14,r0 */
L8c612bc2: sa_motion_publish_stop(ctx,0x8c612bc2u); fcopy(f,0u,6u);goto L8c612bc4; /* fmov fr6,fr0 */
L8c612bc4: sa_motion_publish_stop(ctx,0x8c612bc4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612bc6; /* fmac fr0,fr3,fr2 */
L8c612bc6: sa_motion_publish_stop(ctx,0x8c612bc6u); fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c612bc8; /* fmov fr2,@(r0,r2) */
L8c612bc8: sa_motion_publish_stop(ctx,0x8c612bc8u); r[0]=24u;goto L8c612bca; /* mov #0x18,r0 */
L8c612bca: sa_motion_publish_stop(ctx,0x8c612bcau); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612bcc; /* mov.l @r15,r3 */
L8c612bcc: sa_motion_publish_stop(ctx,0x8c612bccu); fstore(m,bus,&io_status,f,0u,r[0]+r[3]);if(io_status)return io_status;goto L8c612bce; /* fmov fr0,@(r0,r3) */
L8c612bce: sa_motion_publish_stop(ctx,0x8c612bceu); r[0]=r[11];goto L8c612bd0; /* mov r11,r0 */
L8c612bd0: sa_motion_publish_stop(ctx,0x8c612bd0u); r[0]+=r[12];goto L8c612bd2; /* add r12,r0 */
L8c612bd2: sa_motion_publish_stop(ctx,0x8c612bd2u); fcopy(f,0u,4u);goto L8c612bd4; /* fmov fr4,fr0 */
L8c612bd4: sa_motion_publish_stop(ctx,0x8c612bd4u); r[3]=r[6];goto L8c612bd6; /* mov r6,r3 */
L8c612bd6: sa_motion_publish_stop(ctx,0x8c612bd6u); r[3]<<=2;goto L8c612bd8; /* shll2 r3 */
L8c612bd8: sa_motion_publish_stop(ctx,0x8c612bd8u); r[3]+=r[14];goto L8c612bda; /* add r14,r3 */
L8c612bda: sa_motion_publish_stop(ctx,0x8c612bdau); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c612bdc; /* mov.l r3,@(0x8,r15) */
L8c612bdc: sa_motion_publish_stop(ctx,0x8c612bdcu); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612bde; /* mov.l @(0x4,r15),r2 */
L8c612bde: sa_motion_publish_stop(ctx,0x8c612bdeu); r[2]+=r[12];goto L8c612be0; /* add r12,r2 */
L8c612be0: sa_motion_publish_stop(ctx,0x8c612be0u); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c612be2; /* mov.l r2,@(0xc,r15) */
L8c612be2: sa_motion_publish_stop(ctx,0x8c612be2u); r[2]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c612be4; /* mov.w @r2,r2 */
L8c612be4: sa_motion_publish_stop(ctx,0x8c612be4u); wr(m,bus,&io_status,r[15],r[2],2);if(io_status)return io_status;goto L8c612be6; /* mov.w r2,@r15 */
L8c612be6: sa_motion_publish_stop(ctx,0x8c612be6u); *fpul=r[2];goto L8c612be8; /* lds r2,FPUL */
L8c612be8: sa_motion_publish_stop(ctx,0x8c612be8u); wr(m,bus,&io_status,r[15]+0x10u,r[0],4);if(io_status)return io_status;goto L8c612bea; /* mov.l r0,@(0x10,r15) */
L8c612bea: sa_motion_publish_stop(ctx,0x8c612beau); r[2]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c612bec; /* mov.w @r0,r2 */
L8c612bec: sa_motion_publish_stop(ctx,0x8c612becu); r[0]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c612bee; /* mov.w @r15,r0 */
L8c612bee: sa_motion_publish_stop(ctx,0x8c612beeu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612bf0; /* float FPUL,fr3 */
L8c612bf0: sa_motion_publish_stop(ctx,0x8c612bf0u); r[2]-=r[0];goto L8c612bf2; /* sub r0,r2 */
L8c612bf2: sa_motion_publish_stop(ctx,0x8c612bf2u); *fpul=r[2];goto L8c612bf4; /* lds r2,FPUL */
L8c612bf4: sa_motion_publish_stop(ctx,0x8c612bf4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c612bf6; /* float FPUL,fr2 */
L8c612bf6: sa_motion_publish_stop(ctx,0x8c612bf6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612bf8; /* fmac fr0,fr2,fr3 */
L8c612bf8: sa_motion_publish_stop(ctx,0x8c612bf8u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c612bfa; /* ftrc fr3,FPUL */
L8c612bfa: sa_motion_publish_stop(ctx,0x8c612bfau); r[2]=*fpul;goto L8c612bfc; /* sts FPUL,r2 */
L8c612bfc: sa_motion_publish_stop(ctx,0x8c612bfcu); wr(m,bus,&io_status,r[3],r[2],2);if(io_status)return io_status;goto L8c612bfe; /* mov.w r2,@r3 */
L8c612bfe: sa_motion_publish_stop(ctx,0x8c612bfeu); r[0]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c612c00; /* mov.l @(0xc,r15),r0 */
L8c612c00: sa_motion_publish_stop(ctx,0x8c612c00u); r[6]+=1u;goto L8c612c02; /* add #0x1,r6 */
L8c612c02: sa_motion_publish_stop(ctx,0x8c612c02u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612c04; /* mov.l @(0x8,r15),r3 */
L8c612c04: sa_motion_publish_stop(ctx,0x8c612c04u); r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c612c06; /* mov.w @(0x2,r0),r0 */
L8c612c06: sa_motion_publish_stop(ctx,0x8c612c06u); *fpul=r[0];goto L8c612c08; /* lds r0,FPUL */
L8c612c08: sa_motion_publish_stop(ctx,0x8c612c08u); wr(m,bus,&io_status,r[15]+0x4u,r[0],2);if(io_status)return io_status;goto L8c612c0a; /* mov.w r0,@(0x4,r15) */
L8c612c0a: sa_motion_publish_stop(ctx,0x8c612c0au); r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c612c0c; /* mov.l @(0x10,r15),r2 */
L8c612c0c: sa_motion_publish_stop(ctx,0x8c612c0cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612c0e; /* float FPUL,fr3 */
L8c612c0e: sa_motion_publish_stop(ctx,0x8c612c0eu); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c612c10; /* mov.w @(0x2,r2),r0 */
L8c612c10: sa_motion_publish_stop(ctx,0x8c612c10u); r[2]=r[0];goto L8c612c12; /* mov r0,r2 */
L8c612c12: sa_motion_publish_stop(ctx,0x8c612c12u); r[0]=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;goto L8c612c14; /* mov.w @(0x4,r15),r0 */
L8c612c14: sa_motion_publish_stop(ctx,0x8c612c14u); r[2]-=r[0];goto L8c612c16; /* sub r0,r2 */
L8c612c16: sa_motion_publish_stop(ctx,0x8c612c16u); *fpul=r[2];goto L8c612c18; /* lds r2,FPUL */
L8c612c18: sa_motion_publish_stop(ctx,0x8c612c18u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c612c1a; /* float FPUL,fr2 */
L8c612c1a: sa_motion_publish_stop(ctx,0x8c612c1au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612c1c; /* fmac fr0,fr2,fr3 */
L8c612c1c: sa_motion_publish_stop(ctx,0x8c612c1cu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c612c1e; /* ftrc fr3,FPUL */
L8c612c1e: sa_motion_publish_stop(ctx,0x8c612c1eu); r[0]=*fpul;goto L8c612c20; /* sts FPUL,r0 */
L8c612c20: sa_motion_publish_stop(ctx,0x8c612c20u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c612c22; /* mov.w r0,@(0x2,r3) */
L8c612c22: sa_motion_publish_stop(ctx,0x8c612c22u); r[0]=r[11];goto L8c612c24; /* mov r11,r0 */
L8c612c24: sa_motion_publish_stop(ctx,0x8c612c24u); r[3]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c612c26; /* mov.l @(r0,r7),r3 */
L8c612c26: sa_motion_publish_stop(ctx,0x8c612c26u); r[3]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c612c28; /* mov.l @(0xc,r3),r3 */
L8c612c28: sa_motion_publish_stop(ctx,0x8c612c28u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c612c2a; /* tst r4,r3 */
L8c612c2a: sa_motion_publish_stop(ctx,0x8c612c2au); if(!(*sr&1u))goto L8c612c7a;goto L8c612c2c; /* bf 0x8c612c7a */
L8c612c2c: sa_motion_publish_stop(ctx,0x8c612c2cu); r[3]=r[11];goto L8c612c2e; /* mov r11,r3 */
L8c612c2e: sa_motion_publish_stop(ctx,0x8c612c2eu); r[2]=r[6];goto L8c612c30; /* mov r6,r2 */
L8c612c30: sa_motion_publish_stop(ctx,0x8c612c30u); r[3]+=r[7];goto L8c612c32; /* add r7,r3 */
L8c612c32: sa_motion_publish_stop(ctx,0x8c612c32u); r[2]<<=2;goto L8c612c34; /* shll2 r2 */
L8c612c34: sa_motion_publish_stop(ctx,0x8c612c34u); r[0]=16u;goto L8c612c36; /* mov #0x10,r0 */
L8c612c36: sa_motion_publish_stop(ctx,0x8c612c36u); r[2]<<=2;goto L8c612c38; /* shll2 r2 */
L8c612c38: sa_motion_publish_stop(ctx,0x8c612c38u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612c3a; /* shll r2 */
L8c612c3a: sa_motion_publish_stop(ctx,0x8c612c3au); r[2]+=r[5];goto L8c612c3c; /* add r5,r2 */
L8c612c3c: sa_motion_publish_stop(ctx,0x8c612c3cu); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c612c3e; /* mov.l r2,@(0x4,r15) */
L8c612c3e: sa_motion_publish_stop(ctx,0x8c612c3eu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c612c40; /* mov.l r3,@r15 */
L8c612c40: sa_motion_publish_stop(ctx,0x8c612c40u); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c612c42; /* mov.l @r3,r1 */
L8c612c42: sa_motion_publish_stop(ctx,0x8c612c42u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612c44; /* fmov.s @(r0,r1),fr3 */
L8c612c44: sa_motion_publish_stop(ctx,0x8c612c44u); fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612c46; /* fmov fr3,@(r0,r2) */
L8c612c46: sa_motion_publish_stop(ctx,0x8c612c46u); r[0]=20u;goto L8c612c48; /* mov #0x14,r0 */
L8c612c48: sa_motion_publish_stop(ctx,0x8c612c48u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612c4a; /* mov.l @r15,r2 */
L8c612c4a: sa_motion_publish_stop(ctx,0x8c612c4au); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612c4c; /* mov.l @(0x4,r15),r3 */
L8c612c4c: sa_motion_publish_stop(ctx,0x8c612c4cu); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c612c4e; /* mov.l @r2,r1 */
L8c612c4e: sa_motion_publish_stop(ctx,0x8c612c4eu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612c50; /* fmov.s @(r0,r1),fr3 */
L8c612c50: sa_motion_publish_stop(ctx,0x8c612c50u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612c52; /* fmov fr3,@(r0,r3) */
L8c612c52: sa_motion_publish_stop(ctx,0x8c612c52u); r[0]=24u;goto L8c612c54; /* mov #0x18,r0 */
L8c612c54: sa_motion_publish_stop(ctx,0x8c612c54u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612c56; /* mov.l @r15,r2 */
L8c612c56: sa_motion_publish_stop(ctx,0x8c612c56u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612c58; /* mov.l @(0x4,r15),r3 */
L8c612c58: sa_motion_publish_stop(ctx,0x8c612c58u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c612c5a; /* mov.l @r2,r1 */
L8c612c5a: sa_motion_publish_stop(ctx,0x8c612c5au); r[2]=r[11];goto L8c612c5c; /* mov r11,r2 */
L8c612c5c: sa_motion_publish_stop(ctx,0x8c612c5cu); r[2]+=r[12];goto L8c612c5e; /* add r12,r2 */
L8c612c5e: sa_motion_publish_stop(ctx,0x8c612c5eu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612c60; /* fmov.s @(r0,r1),fr3 */
L8c612c60: sa_motion_publish_stop(ctx,0x8c612c60u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612c62; /* fmov fr3,@(r0,r3) */
L8c612c62: sa_motion_publish_stop(ctx,0x8c612c62u); r[3]=r[6];goto L8c612c64; /* mov r6,r3 */
L8c612c64: sa_motion_publish_stop(ctx,0x8c612c64u); r[3]<<=2;goto L8c612c66; /* shll2 r3 */
L8c612c66: sa_motion_publish_stop(ctx,0x8c612c66u); r[3]+=r[14];goto L8c612c68; /* add r14,r3 */
L8c612c68: sa_motion_publish_stop(ctx,0x8c612c68u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c612c6a; /* mov.l r3,@(0x4,r15) */
L8c612c6a: sa_motion_publish_stop(ctx,0x8c612c6au); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c612c6c; /* mov.l r2,@r15 */
L8c612c6c: sa_motion_publish_stop(ctx,0x8c612c6cu); r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c612c6e; /* mov.w @r2,r1 */
L8c612c6e: sa_motion_publish_stop(ctx,0x8c612c6eu); r[6]+=1u;goto L8c612c70; /* add #0x1,r6 */
L8c612c70: sa_motion_publish_stop(ctx,0x8c612c70u); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c612c72; /* mov.w r1,@r3 */
L8c612c72: sa_motion_publish_stop(ctx,0x8c612c72u); r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612c74; /* mov.l @r15,r0 */
L8c612c74: sa_motion_publish_stop(ctx,0x8c612c74u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612c76; /* mov.l @(0x4,r15),r3 */
L8c612c76: sa_motion_publish_stop(ctx,0x8c612c76u); r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c612c78; /* mov.w @(0x2,r0),r0 */
L8c612c78: sa_motion_publish_stop(ctx,0x8c612c78u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c612c7a; /* mov.w r0,@(0x2,r3) */
L8c612c7a: sa_motion_publish_stop(ctx,0x8c612c7au); wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c612c7c; /* mov.l r10,@r15 */
L8c612c7c: sa_motion_publish_stop(ctx,0x8c612c7cu); r[3]=3u;goto L8c612c7e; /* mov #0x3,r3 */
L8c612c7e: sa_motion_publish_stop(ctx,0x8c612c7eu); r[10]+=1u;goto L8c612c80; /* add #0x1,r10 */
L8c612c80: sa_motion_publish_stop(ctx,0x8c612c80u); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[3]);goto L8c612c82; /* cmp/ge r3,r10 */
L8c612c82: sa_motion_publish_stop(ctx,0x8c612c82u); if((*sr&1u))goto L8c612c88;goto L8c612c84; /* bt 0x8c612c88 */
L8c612c84: sa_motion_publish_stop(ctx,0x8c612c86u); goto L8c612b1e; /* bra 0x8c612b1e */
L8c612c88: sa_motion_publish_stop(ctx,0x8c612c88u); r[0]=0x8c612d64u;goto L8c612c8a; /* mova 0x8c612d64,r0 */
L8c612c8a: sa_motion_publish_stop(ctx,0x8c612c8au); r[4]=rd(m,bus,&io_status,0x8c612d60u,4);if(io_status)return io_status;goto L8c612c8c; /* mov.l 0x8c612d60,r4 */
L8c612c8c: sa_motion_publish_stop(ctx,0x8c612c8cu); fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c612c8e; /* fmov.s @r0,fr4 */
L8c612c8e: sa_motion_publish_stop(ctx,0x8c612c8eu); r[0]=r[6];goto L8c612c90; /* mov r6,r0 */
L8c612c90: sa_motion_publish_stop(ctx,0x8c612c90u); r[7]=rd(m,bus,&io_status,0x8c612d5cu,4);if(io_status)return io_status;goto L8c612c92; /* mov.l 0x8c612d5c,r7 */
L8c612c92: sa_motion_publish_stop(ctx,0x8c612c92u); *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c612c94; /* cmp/eq #0x3,r0 */
L8c612c94: sa_motion_publish_stop(ctx,0x8c612c94u); if(!(*sr&1u))goto L8c612d6c;goto L8c612c96; /* bf 0x8c612d6c */
L8c612c96: sa_motion_publish_stop(ctx,0x8c612c96u); r[6]=r[14];goto L8c612c98; /* mov r14,r6 */
L8c612c98: sa_motion_publish_stop(ctx,0x8c612c98u); r[8]+=28u;goto L8c612c9a; /* add #0x1c,r8 */
L8c612c9a: sa_motion_publish_stop(ctx,0x8c612c9au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612c9c; /* fmov.s fr5,@-r8 */
L8c612c9c: sa_motion_publish_stop(ctx,0x8c612c9cu); r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c612c9e; /* mov.w @(0x2,r6),r0 */
L8c612c9e: sa_motion_publish_stop(ctx,0x8c612c9eu); r[3]=r[0];goto L8c612ca0; /* mov r0,r3 */
L8c612ca0: sa_motion_publish_stop(ctx,0x8c612ca0u); *fpul=r[3];goto L8c612ca2; /* lds r3,FPUL */
L8c612ca2: sa_motion_publish_stop(ctx,0x8c612ca2u); r[0]=24u;goto L8c612ca4; /* mov #0x18,r0 */
L8c612ca4: sa_motion_publish_stop(ctx,0x8c612ca4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612ca6; /* float FPUL,fr3 */
L8c612ca6: sa_motion_publish_stop(ctx,0x8c612ca6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612ca8; /* fmul fr4,fr3 */
L8c612ca8: sa_motion_publish_stop(ctx,0x8c612ca8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612caa; /* fmov.s fr3,@-r8 */
L8c612caa: sa_motion_publish_stop(ctx,0x8c612caau); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c612cac; /* mov.w @r6,r3 */
L8c612cac: sa_motion_publish_stop(ctx,0x8c612cacu); r[6]=r[5];goto L8c612cae; /* mov r5,r6 */
L8c612cae: sa_motion_publish_stop(ctx,0x8c612caeu); *fpul=r[3];goto L8c612cb0; /* lds r3,FPUL */
L8c612cb0: sa_motion_publish_stop(ctx,0x8c612cb0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612cb2; /* float FPUL,fr3 */
L8c612cb2: sa_motion_publish_stop(ctx,0x8c612cb2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612cb4; /* fmul fr4,fr3 */
L8c612cb4: sa_motion_publish_stop(ctx,0x8c612cb4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cb6; /* fmov.s fr3,@-r8 */
L8c612cb6: sa_motion_publish_stop(ctx,0x8c612cb6u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612cb8; /* fmov.s @(r0,r6),fr3 */
L8c612cb8: sa_motion_publish_stop(ctx,0x8c612cb8u); r[0]=20u;goto L8c612cba; /* mov #0x14,r0 */
L8c612cba: sa_motion_publish_stop(ctx,0x8c612cbau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cbc; /* fmov.s fr3,@-r8 */
L8c612cbc: sa_motion_publish_stop(ctx,0x8c612cbcu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612cbe; /* fmov.s @(r0,r6),fr3 */
L8c612cbe: sa_motion_publish_stop(ctx,0x8c612cbeu); r[0]=16u;goto L8c612cc0; /* mov #0x10,r0 */
L8c612cc0: sa_motion_publish_stop(ctx,0x8c612cc0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cc2; /* fmov.s fr3,@-r8 */
L8c612cc2: sa_motion_publish_stop(ctx,0x8c612cc2u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612cc4; /* fmov.s @(r0,r6),fr3 */
L8c612cc4: sa_motion_publish_stop(ctx,0x8c612cc4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cc6; /* fmov.s fr3,@-r8 */
L8c612cc6: sa_motion_publish_stop(ctx,0x8c612cc6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612cc8; /* mov.l r4,@-r8 */
L8c612cc8: sa_motion_publish_stop(ctx,0x8c612cc8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612cca; /* pref @r8 */
L8c612cca: sa_motion_publish_stop(ctx,0x8c612ccau); r[6]=r[14];goto L8c612ccc; /* mov r14,r6 */
L8c612ccc: sa_motion_publish_stop(ctx,0x8c612cccu); r[6]+=4u;goto L8c612cce; /* add #0x4,r6 */
L8c612cce: sa_motion_publish_stop(ctx,0x8c612cceu); r[8]+=60u;goto L8c612cd0; /* add #0x3c,r8 */
L8c612cd0: sa_motion_publish_stop(ctx,0x8c612cd0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612cd2; /* fmov.s fr5,@-r8 */
L8c612cd2: sa_motion_publish_stop(ctx,0x8c612cd2u); r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c612cd4; /* mov.w @(0x2,r6),r0 */
L8c612cd4: sa_motion_publish_stop(ctx,0x8c612cd4u); r[3]=r[0];goto L8c612cd6; /* mov r0,r3 */
L8c612cd6: sa_motion_publish_stop(ctx,0x8c612cd6u); *fpul=r[3];goto L8c612cd8; /* lds r3,FPUL */
L8c612cd8: sa_motion_publish_stop(ctx,0x8c612cd8u); r[0]=24u;goto L8c612cda; /* mov #0x18,r0 */
L8c612cda: sa_motion_publish_stop(ctx,0x8c612cdau); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612cdc; /* float FPUL,fr3 */
L8c612cdc: sa_motion_publish_stop(ctx,0x8c612cdcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612cde; /* fmul fr4,fr3 */
L8c612cde: sa_motion_publish_stop(ctx,0x8c612cdeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612ce0; /* fmov.s fr3,@-r8 */
L8c612ce0: sa_motion_publish_stop(ctx,0x8c612ce0u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c612ce2; /* mov.w @r6,r3 */
L8c612ce2: sa_motion_publish_stop(ctx,0x8c612ce2u); r[6]=r[5];goto L8c612ce4; /* mov r5,r6 */
L8c612ce4: sa_motion_publish_stop(ctx,0x8c612ce4u); r[6]+=32u;goto L8c612ce6; /* add #0x20,r6 */
L8c612ce6: sa_motion_publish_stop(ctx,0x8c612ce6u); *fpul=r[3];goto L8c612ce8; /* lds r3,FPUL */
L8c612ce8: sa_motion_publish_stop(ctx,0x8c612ce8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612cea; /* float FPUL,fr3 */
L8c612cea: sa_motion_publish_stop(ctx,0x8c612ceau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612cec; /* fmul fr4,fr3 */
L8c612cec: sa_motion_publish_stop(ctx,0x8c612cecu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cee; /* fmov.s fr3,@-r8 */
L8c612cee: sa_motion_publish_stop(ctx,0x8c612ceeu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612cf0; /* fmov.s @(r0,r6),fr3 */
L8c612cf0: sa_motion_publish_stop(ctx,0x8c612cf0u); r[0]=20u;goto L8c612cf2; /* mov #0x14,r0 */
L8c612cf2: sa_motion_publish_stop(ctx,0x8c612cf2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cf4; /* fmov.s fr3,@-r8 */
L8c612cf4: sa_motion_publish_stop(ctx,0x8c612cf4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612cf6; /* fmov.s @(r0,r6),fr3 */
L8c612cf6: sa_motion_publish_stop(ctx,0x8c612cf6u); r[0]=16u;goto L8c612cf8; /* mov #0x10,r0 */
L8c612cf8: sa_motion_publish_stop(ctx,0x8c612cf8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cfa; /* fmov.s fr3,@-r8 */
L8c612cfa: sa_motion_publish_stop(ctx,0x8c612cfau); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612cfc; /* fmov.s @(r0,r6),fr3 */
L8c612cfc: sa_motion_publish_stop(ctx,0x8c612cfcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612cfe; /* fmov.s fr3,@-r8 */
L8c612cfe: sa_motion_publish_stop(ctx,0x8c612cfeu); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612d00; /* mov.l r4,@-r8 */
L8c612d00: sa_motion_publish_stop(ctx,0x8c612d00u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612d02; /* pref @r8 */
L8c612d02: sa_motion_publish_stop(ctx,0x8c612d02u); r[4]=r[14];goto L8c612d04; /* mov r14,r4 */
L8c612d04: sa_motion_publish_stop(ctx,0x8c612d04u); r[4]+=8u;goto L8c612d06; /* add #0x8,r4 */
L8c612d06: sa_motion_publish_stop(ctx,0x8c612d06u); r[8]+=60u;goto L8c612d08; /* add #0x3c,r8 */
L8c612d08: sa_motion_publish_stop(ctx,0x8c612d08u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612d0a; /* fmov.s fr5,@-r8 */
L8c612d0a: sa_motion_publish_stop(ctx,0x8c612d0au); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c612d0c; /* mov.w @(0x2,r4),r0 */
L8c612d0c: sa_motion_publish_stop(ctx,0x8c612d0cu); r[3]=r[0];goto L8c612d0e; /* mov r0,r3 */
L8c612d0e: sa_motion_publish_stop(ctx,0x8c612d0eu); *fpul=r[3];goto L8c612d10; /* lds r3,FPUL */
L8c612d10: sa_motion_publish_stop(ctx,0x8c612d10u); r[0]=24u;goto L8c612d12; /* mov #0x18,r0 */
L8c612d12: sa_motion_publish_stop(ctx,0x8c612d12u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612d14; /* float FPUL,fr3 */
L8c612d14: sa_motion_publish_stop(ctx,0x8c612d14u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612d16; /* fmul fr4,fr3 */
L8c612d16: sa_motion_publish_stop(ctx,0x8c612d16u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d18; /* fmov.s fr3,@-r8 */
L8c612d18: sa_motion_publish_stop(ctx,0x8c612d18u); r[3]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c612d1a; /* mov.w @r4,r3 */
L8c612d1a: sa_motion_publish_stop(ctx,0x8c612d1au); r[4]=r[5];goto L8c612d1c; /* mov r5,r4 */
L8c612d1c: sa_motion_publish_stop(ctx,0x8c612d1cu); r[4]+=64u;goto L8c612d1e; /* add #0x40,r4 */
L8c612d1e: sa_motion_publish_stop(ctx,0x8c612d1eu); *fpul=r[3];goto L8c612d20; /* lds r3,FPUL */
L8c612d20: sa_motion_publish_stop(ctx,0x8c612d20u); r[3]=rd(m,bus,&io_status,0x8c612d68u,4);if(io_status)return io_status;goto L8c612d22; /* mov.l 0x8c612d68,r3 */
L8c612d22: sa_motion_publish_stop(ctx,0x8c612d22u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612d24; /* float FPUL,fr3 */
L8c612d24: sa_motion_publish_stop(ctx,0x8c612d24u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612d26; /* fmul fr4,fr3 */
L8c612d26: sa_motion_publish_stop(ctx,0x8c612d26u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d28; /* fmov.s fr3,@-r8 */
L8c612d28: sa_motion_publish_stop(ctx,0x8c612d28u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612d2a; /* fmov.s @(r0,r4),fr3 */
L8c612d2a: sa_motion_publish_stop(ctx,0x8c612d2au); r[0]=20u;goto L8c612d2c; /* mov #0x14,r0 */
L8c612d2c: sa_motion_publish_stop(ctx,0x8c612d2cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d2e; /* fmov.s fr3,@-r8 */
L8c612d2e: sa_motion_publish_stop(ctx,0x8c612d2eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612d30; /* fmov.s @(r0,r4),fr3 */
L8c612d30: sa_motion_publish_stop(ctx,0x8c612d30u); r[0]=16u;goto L8c612d32; /* mov #0x10,r0 */
L8c612d32: sa_motion_publish_stop(ctx,0x8c612d32u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d34; /* fmov.s fr3,@-r8 */
L8c612d34: sa_motion_publish_stop(ctx,0x8c612d34u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612d36; /* fmov.s @(r0,r4),fr3 */
L8c612d36: sa_motion_publish_stop(ctx,0x8c612d36u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d38; /* fmov.s fr3,@-r8 */
L8c612d38: sa_motion_publish_stop(ctx,0x8c612d38u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c612d3a; /* mov.l r3,@-r8 */
L8c612d3a: sa_motion_publish_stop(ctx,0x8c612d3au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612d3c; /* pref @r8 */
L8c612d3c: sa_motion_publish_stop(ctx,0x8c612d3cu); r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c612d3e; /* mov.l @r7,r2 */
L8c612d3e: sa_motion_publish_stop(ctx,0x8c612d3eu); r[8]+=32u;goto L8c612d40; /* add #0x20,r8 */
L8c612d40: sa_motion_publish_stop(ctx,0x8c612d42u); r[2]+=3u;goto L8c612e50; /* bra 0x8c612e50 */
L8c612d6c: sa_motion_publish_stop(ctx,0x8c612d6cu); r[6]=r[14];goto L8c612d6e; /* mov r14,r6 */
L8c612d6e: sa_motion_publish_stop(ctx,0x8c612d6eu); r[8]+=28u;goto L8c612d70; /* add #0x1c,r8 */
L8c612d70: sa_motion_publish_stop(ctx,0x8c612d70u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612d72; /* fmov.s fr5,@-r8 */
L8c612d72: sa_motion_publish_stop(ctx,0x8c612d72u); r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c612d74; /* mov.w @(0x2,r6),r0 */
L8c612d74: sa_motion_publish_stop(ctx,0x8c612d74u); r[3]=r[0];goto L8c612d76; /* mov r0,r3 */
L8c612d76: sa_motion_publish_stop(ctx,0x8c612d76u); *fpul=r[3];goto L8c612d78; /* lds r3,FPUL */
L8c612d78: sa_motion_publish_stop(ctx,0x8c612d78u); r[0]=24u;goto L8c612d7a; /* mov #0x18,r0 */
L8c612d7a: sa_motion_publish_stop(ctx,0x8c612d7au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612d7c; /* float FPUL,fr3 */
L8c612d7c: sa_motion_publish_stop(ctx,0x8c612d7cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612d7e; /* fmul fr4,fr3 */
L8c612d7e: sa_motion_publish_stop(ctx,0x8c612d7eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d80; /* fmov.s fr3,@-r8 */
L8c612d80: sa_motion_publish_stop(ctx,0x8c612d80u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c612d82; /* mov.w @r6,r3 */
L8c612d82: sa_motion_publish_stop(ctx,0x8c612d82u); r[6]=r[5];goto L8c612d84; /* mov r5,r6 */
L8c612d84: sa_motion_publish_stop(ctx,0x8c612d84u); *fpul=r[3];goto L8c612d86; /* lds r3,FPUL */
L8c612d86: sa_motion_publish_stop(ctx,0x8c612d86u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612d88; /* float FPUL,fr3 */
L8c612d88: sa_motion_publish_stop(ctx,0x8c612d88u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612d8a; /* fmul fr4,fr3 */
L8c612d8a: sa_motion_publish_stop(ctx,0x8c612d8au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d8c; /* fmov.s fr3,@-r8 */
L8c612d8c: sa_motion_publish_stop(ctx,0x8c612d8cu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612d8e; /* fmov.s @(r0,r6),fr3 */
L8c612d8e: sa_motion_publish_stop(ctx,0x8c612d8eu); r[0]=20u;goto L8c612d90; /* mov #0x14,r0 */
L8c612d90: sa_motion_publish_stop(ctx,0x8c612d90u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d92; /* fmov.s fr3,@-r8 */
L8c612d92: sa_motion_publish_stop(ctx,0x8c612d92u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612d94; /* fmov.s @(r0,r6),fr3 */
L8c612d94: sa_motion_publish_stop(ctx,0x8c612d94u); r[0]=16u;goto L8c612d96; /* mov #0x10,r0 */
L8c612d96: sa_motion_publish_stop(ctx,0x8c612d96u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d98; /* fmov.s fr3,@-r8 */
L8c612d98: sa_motion_publish_stop(ctx,0x8c612d98u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612d9a; /* fmov.s @(r0,r6),fr3 */
L8c612d9a: sa_motion_publish_stop(ctx,0x8c612d9au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612d9c; /* fmov.s fr3,@-r8 */
L8c612d9c: sa_motion_publish_stop(ctx,0x8c612d9cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612d9e; /* mov.l r4,@-r8 */
L8c612d9e: sa_motion_publish_stop(ctx,0x8c612d9eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612da0; /* pref @r8 */
L8c612da0: sa_motion_publish_stop(ctx,0x8c612da0u); r[6]=r[14];goto L8c612da2; /* mov r14,r6 */
L8c612da2: sa_motion_publish_stop(ctx,0x8c612da2u); r[6]+=4u;goto L8c612da4; /* add #0x4,r6 */
L8c612da4: sa_motion_publish_stop(ctx,0x8c612da4u); r[8]+=60u;goto L8c612da6; /* add #0x3c,r8 */
L8c612da6: sa_motion_publish_stop(ctx,0x8c612da6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612da8; /* fmov.s fr5,@-r8 */
L8c612da8: sa_motion_publish_stop(ctx,0x8c612da8u); r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c612daa; /* mov.w @(0x2,r6),r0 */
L8c612daa: sa_motion_publish_stop(ctx,0x8c612daau); r[3]=r[0];goto L8c612dac; /* mov r0,r3 */
L8c612dac: sa_motion_publish_stop(ctx,0x8c612dacu); *fpul=r[3];goto L8c612dae; /* lds r3,FPUL */
L8c612dae: sa_motion_publish_stop(ctx,0x8c612daeu); r[0]=24u;goto L8c612db0; /* mov #0x18,r0 */
L8c612db0: sa_motion_publish_stop(ctx,0x8c612db0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612db2; /* float FPUL,fr3 */
L8c612db2: sa_motion_publish_stop(ctx,0x8c612db2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612db4; /* fmul fr4,fr3 */
L8c612db4: sa_motion_publish_stop(ctx,0x8c612db4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612db6; /* fmov.s fr3,@-r8 */
L8c612db6: sa_motion_publish_stop(ctx,0x8c612db6u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c612db8; /* mov.w @r6,r3 */
L8c612db8: sa_motion_publish_stop(ctx,0x8c612db8u); r[6]=r[5];goto L8c612dba; /* mov r5,r6 */
L8c612dba: sa_motion_publish_stop(ctx,0x8c612dbau); r[6]+=32u;goto L8c612dbc; /* add #0x20,r6 */
L8c612dbc: sa_motion_publish_stop(ctx,0x8c612dbcu); *fpul=r[3];goto L8c612dbe; /* lds r3,FPUL */
L8c612dbe: sa_motion_publish_stop(ctx,0x8c612dbeu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612dc0; /* float FPUL,fr3 */
L8c612dc0: sa_motion_publish_stop(ctx,0x8c612dc0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612dc2; /* fmul fr4,fr3 */
L8c612dc2: sa_motion_publish_stop(ctx,0x8c612dc2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612dc4; /* fmov.s fr3,@-r8 */
L8c612dc4: sa_motion_publish_stop(ctx,0x8c612dc4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612dc6; /* fmov.s @(r0,r6),fr3 */
L8c612dc6: sa_motion_publish_stop(ctx,0x8c612dc6u); r[0]=20u;goto L8c612dc8; /* mov #0x14,r0 */
L8c612dc8: sa_motion_publish_stop(ctx,0x8c612dc8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612dca; /* fmov.s fr3,@-r8 */
L8c612dca: sa_motion_publish_stop(ctx,0x8c612dcau); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612dcc; /* fmov.s @(r0,r6),fr3 */
L8c612dcc: sa_motion_publish_stop(ctx,0x8c612dccu); r[0]=16u;goto L8c612dce; /* mov #0x10,r0 */
L8c612dce: sa_motion_publish_stop(ctx,0x8c612dceu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612dd0; /* fmov.s fr3,@-r8 */
L8c612dd0: sa_motion_publish_stop(ctx,0x8c612dd0u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612dd2; /* fmov.s @(r0,r6),fr3 */
L8c612dd2: sa_motion_publish_stop(ctx,0x8c612dd2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612dd4; /* fmov.s fr3,@-r8 */
L8c612dd4: sa_motion_publish_stop(ctx,0x8c612dd4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612dd6; /* mov.l r4,@-r8 */
L8c612dd6: sa_motion_publish_stop(ctx,0x8c612dd6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612dd8; /* pref @r8 */
L8c612dd8: sa_motion_publish_stop(ctx,0x8c612dd8u); r[6]=r[14];goto L8c612dda; /* mov r14,r6 */
L8c612dda: sa_motion_publish_stop(ctx,0x8c612ddau); r[6]+=12u;goto L8c612ddc; /* add #0xc,r6 */
L8c612ddc: sa_motion_publish_stop(ctx,0x8c612ddcu); r[8]+=60u;goto L8c612dde; /* add #0x3c,r8 */
L8c612dde: sa_motion_publish_stop(ctx,0x8c612ddeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612de0; /* fmov.s fr5,@-r8 */
L8c612de0: sa_motion_publish_stop(ctx,0x8c612de0u); r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c612de2; /* mov.w @(0x2,r6),r0 */
L8c612de2: sa_motion_publish_stop(ctx,0x8c612de2u); r[3]=r[0];goto L8c612de4; /* mov r0,r3 */
L8c612de4: sa_motion_publish_stop(ctx,0x8c612de4u); *fpul=r[3];goto L8c612de6; /* lds r3,FPUL */
L8c612de6: sa_motion_publish_stop(ctx,0x8c612de6u); r[0]=24u;goto L8c612de8; /* mov #0x18,r0 */
L8c612de8: sa_motion_publish_stop(ctx,0x8c612de8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612dea; /* float FPUL,fr3 */
L8c612dea: sa_motion_publish_stop(ctx,0x8c612deau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612dec; /* fmul fr4,fr3 */
L8c612dec: sa_motion_publish_stop(ctx,0x8c612decu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612dee; /* fmov.s fr3,@-r8 */
L8c612dee: sa_motion_publish_stop(ctx,0x8c612deeu); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c612df0; /* mov.w @r6,r3 */
L8c612df0: sa_motion_publish_stop(ctx,0x8c612df0u); r[6]=r[5];goto L8c612df2; /* mov r5,r6 */
L8c612df2: sa_motion_publish_stop(ctx,0x8c612df2u); r[6]+=96u;goto L8c612df4; /* add #0x60,r6 */
L8c612df4: sa_motion_publish_stop(ctx,0x8c612df4u); *fpul=r[3];goto L8c612df6; /* lds r3,FPUL */
L8c612df6: sa_motion_publish_stop(ctx,0x8c612df6u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612df8; /* float FPUL,fr3 */
L8c612df8: sa_motion_publish_stop(ctx,0x8c612df8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612dfa; /* fmul fr4,fr3 */
L8c612dfa: sa_motion_publish_stop(ctx,0x8c612dfau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612dfc; /* fmov.s fr3,@-r8 */
L8c612dfc: sa_motion_publish_stop(ctx,0x8c612dfcu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612dfe; /* fmov.s @(r0,r6),fr3 */
L8c612dfe: sa_motion_publish_stop(ctx,0x8c612dfeu); r[0]=20u;goto L8c612e00; /* mov #0x14,r0 */
L8c612e00: sa_motion_publish_stop(ctx,0x8c612e00u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e02; /* fmov.s fr3,@-r8 */
L8c612e02: sa_motion_publish_stop(ctx,0x8c612e02u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612e04; /* fmov.s @(r0,r6),fr3 */
L8c612e04: sa_motion_publish_stop(ctx,0x8c612e04u); r[0]=16u;goto L8c612e06; /* mov #0x10,r0 */
L8c612e06: sa_motion_publish_stop(ctx,0x8c612e06u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e08; /* fmov.s fr3,@-r8 */
L8c612e08: sa_motion_publish_stop(ctx,0x8c612e08u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c612e0a; /* fmov.s @(r0,r6),fr3 */
L8c612e0a: sa_motion_publish_stop(ctx,0x8c612e0au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e0c; /* fmov.s fr3,@-r8 */
L8c612e0c: sa_motion_publish_stop(ctx,0x8c612e0cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612e0e; /* mov.l r4,@-r8 */
L8c612e0e: sa_motion_publish_stop(ctx,0x8c612e0eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612e10; /* pref @r8 */
L8c612e10: sa_motion_publish_stop(ctx,0x8c612e10u); r[4]=r[14];goto L8c612e12; /* mov r14,r4 */
L8c612e12: sa_motion_publish_stop(ctx,0x8c612e12u); r[4]+=8u;goto L8c612e14; /* add #0x8,r4 */
L8c612e14: sa_motion_publish_stop(ctx,0x8c612e14u); r[8]+=60u;goto L8c612e16; /* add #0x3c,r8 */
L8c612e16: sa_motion_publish_stop(ctx,0x8c612e16u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612e18; /* fmov.s fr5,@-r8 */
L8c612e18: sa_motion_publish_stop(ctx,0x8c612e18u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c612e1a; /* mov.w @(0x2,r4),r0 */
L8c612e1a: sa_motion_publish_stop(ctx,0x8c612e1au); r[3]=r[0];goto L8c612e1c; /* mov r0,r3 */
L8c612e1c: sa_motion_publish_stop(ctx,0x8c612e1cu); *fpul=r[3];goto L8c612e1e; /* lds r3,FPUL */
L8c612e1e: sa_motion_publish_stop(ctx,0x8c612e1eu); r[0]=24u;goto L8c612e20; /* mov #0x18,r0 */
L8c612e20: sa_motion_publish_stop(ctx,0x8c612e20u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612e22; /* float FPUL,fr3 */
L8c612e22: sa_motion_publish_stop(ctx,0x8c612e22u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612e24; /* fmul fr4,fr3 */
L8c612e24: sa_motion_publish_stop(ctx,0x8c612e24u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e26; /* fmov.s fr3,@-r8 */
L8c612e26: sa_motion_publish_stop(ctx,0x8c612e26u); r[3]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c612e28; /* mov.w @r4,r3 */
L8c612e28: sa_motion_publish_stop(ctx,0x8c612e28u); r[4]=r[5];goto L8c612e2a; /* mov r5,r4 */
L8c612e2a: sa_motion_publish_stop(ctx,0x8c612e2au); r[4]+=64u;goto L8c612e2c; /* add #0x40,r4 */
L8c612e2c: sa_motion_publish_stop(ctx,0x8c612e2cu); *fpul=r[3];goto L8c612e2e; /* lds r3,FPUL */
L8c612e2e: sa_motion_publish_stop(ctx,0x8c612e2eu); r[3]=rd(m,bus,&io_status,0x8c612f2cu,4);if(io_status)return io_status;goto L8c612e30; /* mov.l 0x8c612f2c,r3 */
L8c612e30: sa_motion_publish_stop(ctx,0x8c612e30u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612e32; /* float FPUL,fr3 */
L8c612e32: sa_motion_publish_stop(ctx,0x8c612e32u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612e34; /* fmul fr4,fr3 */
L8c612e34: sa_motion_publish_stop(ctx,0x8c612e34u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e36; /* fmov.s fr3,@-r8 */
L8c612e36: sa_motion_publish_stop(ctx,0x8c612e36u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612e38; /* fmov.s @(r0,r4),fr3 */
L8c612e38: sa_motion_publish_stop(ctx,0x8c612e38u); r[0]=20u;goto L8c612e3a; /* mov #0x14,r0 */
L8c612e3a: sa_motion_publish_stop(ctx,0x8c612e3au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e3c; /* fmov.s fr3,@-r8 */
L8c612e3c: sa_motion_publish_stop(ctx,0x8c612e3cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612e3e; /* fmov.s @(r0,r4),fr3 */
L8c612e3e: sa_motion_publish_stop(ctx,0x8c612e3eu); r[0]=16u;goto L8c612e40; /* mov #0x10,r0 */
L8c612e40: sa_motion_publish_stop(ctx,0x8c612e40u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e42; /* fmov.s fr3,@-r8 */
L8c612e42: sa_motion_publish_stop(ctx,0x8c612e42u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612e44; /* fmov.s @(r0,r4),fr3 */
L8c612e44: sa_motion_publish_stop(ctx,0x8c612e44u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612e46; /* fmov.s fr3,@-r8 */
L8c612e46: sa_motion_publish_stop(ctx,0x8c612e46u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c612e48; /* mov.l r3,@-r8 */
L8c612e48: sa_motion_publish_stop(ctx,0x8c612e48u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612e4a; /* pref @r8 */
L8c612e4a: sa_motion_publish_stop(ctx,0x8c612e4au); r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c612e4c; /* mov.l @r7,r2 */
L8c612e4c: sa_motion_publish_stop(ctx,0x8c612e4cu); r[8]+=32u;goto L8c612e4e; /* add #0x20,r8 */
L8c612e4e: sa_motion_publish_stop(ctx,0x8c612e4eu); r[2]+=4u;goto L8c612e50; /* add #0x4,r2 */
L8c612e50: sa_motion_publish_stop(ctx,0x8c612e50u); wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c612e52; /* mov.l r2,@r7 */
L8c612e52: sa_motion_publish_stop(ctx,0x8c612e52u); r[15]+=80u;goto L8c612e54; /* add #0x50,r15 */
L8c612e54: sa_motion_publish_stop(ctx,0x8c612e54u); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c612e56; /* fmov.s @r15+,fr12 */
L8c612e56: sa_motion_publish_stop(ctx,0x8c612e56u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612e58; /* mov.l @r15+,r9 */
L8c612e58: sa_motion_publish_stop(ctx,0x8c612e58u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612e5a; /* mov.l @r15+,r10 */
L8c612e5a: sa_motion_publish_stop(ctx,0x8c612e5au); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612e5c; /* mov.l @r15+,r11 */
L8c612e5c: sa_motion_publish_stop(ctx,0x8c612e5cu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612e5e; /* mov.l @r15+,r12 */
L8c612e5e: sa_motion_publish_stop(ctx,0x8c612e5eu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612e60; /* mov.l @r15+,r13 */
L8c612e60: sa_motion_publish_stop(ctx,0x8c612e62u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
