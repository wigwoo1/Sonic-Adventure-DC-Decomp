#include "sa_draw_motion_stop.h"
/* Environment UV/face-factor triangle near-plane clipper with portable queue interface; name inferred.
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
SA_STAGE_EXPORT int sa_environment_face_triangle_near_clip_8c6173ba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6173ba;
L8c6173ba: sa_motion_publish_stop(ctx,0x8c6173bau); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6173bc; /* mov.l r14,@-r15 */
L8c6173bc: sa_motion_publish_stop(ctx,0x8c6173bcu); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6173be; /* mov.l r13,@-r15 */
L8c6173be: sa_motion_publish_stop(ctx,0x8c6173beu); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6173c0; /* mov.l r12,@-r15 */
L8c6173c0: sa_motion_publish_stop(ctx,0x8c6173c0u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6173c2; /* mov.l r11,@-r15 */
L8c6173c2: sa_motion_publish_stop(ctx,0x8c6173c2u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6173c4; /* mov.l r10,@-r15 */
L8c6173c4: sa_motion_publish_stop(ctx,0x8c6173c4u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6173c6; /* mov.l r9,@-r15 */
L8c6173c6: sa_motion_publish_stop(ctx,0x8c6173c6u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c6173c8; /* fmov.s fr12,@-r15 */
L8c6173c8: sa_motion_publish_stop(ctx,0x8c6173c8u); r[15]+=4294967216u;goto L8c6173ca; /* add #-0x50,r15 */
L8c6173ca: sa_motion_publish_stop(ctx,0x8c6173cau); r[3]=rd(m,bus,&io_status,0x8c6174c0u,4);if(io_status)return io_status;goto L8c6173cc; /* mov.l 0x8c6174c0,r3 */
L8c6173cc: sa_motion_publish_stop(ctx,0x8c6173ccu); r[7]=r[15];goto L8c6173ce; /* mov r15,r7 */
L8c6173ce: sa_motion_publish_stop(ctx,0x8c6173ceu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][8]=0x3f800000u;goto L8c6173d0; /* fldi1 fr8 */
L8c6173d0: sa_motion_publish_stop(ctx,0x8c6173d0u); r[7]+=68u;goto L8c6173d2; /* add #0x44,r7 */
L8c6173d2: sa_motion_publish_stop(ctx,0x8c6173d2u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6173d4; /* fmov.s @r3,fr3 */
L8c6173d4: sa_motion_publish_stop(ctx,0x8c6173d4u); r[9]=r[15];goto L8c6173d6; /* mov r15,r9 */
L8c6173d6: sa_motion_publish_stop(ctx,0x8c6173d6u); fcopy(f,6u,8u);goto L8c6173d8; /* fmov fr8,fr6 */
L8c6173d8: sa_motion_publish_stop(ctx,0x8c6173d8u); r[2]=r[7];goto L8c6173da; /* mov r7,r2 */
L8c6173da: sa_motion_publish_stop(ctx,0x8c6173dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[3],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6173dc; /* fdiv fr3,fr6 */
L8c6173dc: sa_motion_publish_stop(ctx,0x8c6173dcu); r[0]=0x8c6174c8u;goto L8c6173de; /* mova 0x8c6174c8,r0 */
L8c6173de: sa_motion_publish_stop(ctx,0x8c6173deu); r[9]+=32u;goto L8c6173e0; /* add #0x20,r9 */
L8c6173e0: sa_motion_publish_stop(ctx,0x8c6173e0u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6173e2; /* mov.l r2,@(0x4,r15) */
L8c6173e2: sa_motion_publish_stop(ctx,0x8c6173e2u); wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c6173e4; /* mov.l r4,@r2 */
L8c6173e4: sa_motion_publish_stop(ctx,0x8c6173e4u); r[10]=r[9];goto L8c6173e6; /* mov r9,r10 */
L8c6173e6: sa_motion_publish_stop(ctx,0x8c6173e6u); r[12]=r[15];goto L8c6173e8; /* mov r15,r12 */
L8c6173e8: sa_motion_publish_stop(ctx,0x8c6173e8u); r[13]=rd(m,bus,&io_status,0x8c6174c4u,4);if(io_status)return io_status;goto L8c6173ea; /* mov.l 0x8c6174c4,r13 */
L8c6173ea: sa_motion_publish_stop(ctx,0x8c6173eau); r[4]=rd(m,bus,&io_status,0x8c6174ccu,4);if(io_status)return io_status;goto L8c6173ec; /* mov.l 0x8c6174cc,r4 */
L8c6173ec: sa_motion_publish_stop(ctx,0x8c6173ecu); r[2]=0u;goto L8c6173ee; /* mov #0x0,r2 */
L8c6173ee: sa_motion_publish_stop(ctx,0x8c6173eeu); wr(m,bus,&io_status,r[7]+0x4u,r[5],4);if(io_status)return io_status;goto L8c6173f0; /* mov.l r5,@(0x4,r7) */
L8c6173f0: sa_motion_publish_stop(ctx,0x8c6173f0u); r[5]=1u;goto L8c6173f2; /* mov #0x1,r5 */
L8c6173f2: sa_motion_publish_stop(ctx,0x8c6173f2u); wr(m,bus,&io_status,r[7]+0x8u,r[6],4);if(io_status)return io_status;goto L8c6173f4; /* mov.l r6,@(0x8,r7) */
L8c6173f4: sa_motion_publish_stop(ctx,0x8c6173f4u); r[12]+=20u;goto L8c6173f6; /* add #0x14,r12 */
L8c6173f6: sa_motion_publish_stop(ctx,0x8c6173f6u); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6173f8; /* mov.l r2,@r15 */
L8c6173f8: sa_motion_publish_stop(ctx,0x8c6173f8u); r[11]=r[10];goto L8c6173fa; /* mov r10,r11 */
L8c6173fa: sa_motion_publish_stop(ctx,0x8c6173fau); r[14]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6173fc; /* mov.l @(0x4,r15),r14 */
L8c6173fc: sa_motion_publish_stop(ctx,0x8c6173fcu); wr(m,bus,&io_status,r[15]+0x4u,r[10],4);if(io_status)return io_status;goto L8c6173fe; /* mov.l r10,@(0x4,r15) */
L8c6173fe: sa_motion_publish_stop(ctx,0x8c6173feu); fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c617400; /* fmov.s @r0,fr5 */
L8c617400: sa_motion_publish_stop(ctx,0x8c617400u); r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617402; /* mov.l @r15,r6 */
L8c617402: sa_motion_publish_stop(ctx,0x8c617402u); r[0]=4u;goto L8c617404; /* mov #0x4,r0 */
L8c617404: sa_motion_publish_stop(ctx,0x8c617404u); r[6]<<=2;goto L8c617406; /* shll2 r6 */
L8c617406: sa_motion_publish_stop(ctx,0x8c617406u); wr(m,bus,&io_status,r[15]+0xcu,r[6],4);if(io_status)return io_status;goto L8c617408; /* mov.l r6,@(0xc,r15) */
L8c617408: sa_motion_publish_stop(ctx,0x8c617408u); r[6]+=r[7];goto L8c61740a; /* add r7,r6 */
L8c61740a: sa_motion_publish_stop(ctx,0x8c61740au); wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c61740c; /* mov.l r6,@(0x8,r15) */
L8c61740c: sa_motion_publish_stop(ctx,0x8c61740cu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61740e; /* mov.l @(0xc,r15),r3 */
L8c61740e: sa_motion_publish_stop(ctx,0x8c61740eu); r[6]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c617410; /* mov.l @r6,r6 */
L8c617410: sa_motion_publish_stop(ctx,0x8c617410u); r[3]+=r[12];goto L8c617412; /* add r12,r3 */
L8c617412: sa_motion_publish_stop(ctx,0x8c617412u); wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c617414; /* mov.l r3,@(0x10,r15) */
L8c617414: sa_motion_publish_stop(ctx,0x8c617414u); r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c617416; /* mov.l r3,@-r15 */
L8c617416: sa_motion_publish_stop(ctx,0x8c617416u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c617418; /* mov.l @r4,r2 */
L8c617418: sa_motion_publish_stop(ctx,0x8c617418u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61741a; /* fmov.s @(r0,r6),fr0 */
L8c61741a: sa_motion_publish_stop(ctx,0x8c61741au); r[0]=16u;goto L8c61741c; /* mov #0x10,r0 */
L8c61741c: sa_motion_publish_stop(ctx,0x8c61741cu); r[3]=r[2];goto L8c61741e; /* mov r2,r3 */
L8c61741e: sa_motion_publish_stop(ctx,0x8c61741eu); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c617420; /* fmov.s @(r0,r3),fr3 */
L8c617420: sa_motion_publish_stop(ctx,0x8c617420u); r[1]=r[2];goto L8c617422; /* mov r2,r1 */
L8c617422: sa_motion_publish_stop(ctx,0x8c617422u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c617424; /* fmov.s @r1,fr2 */
L8c617424: sa_motion_publish_stop(ctx,0x8c617424u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617426; /* fmul fr0,fr3 */
L8c617426: sa_motion_publish_stop(ctx,0x8c617426u); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c617428; /* fmov.s @r6,fr0 */
L8c617428: sa_motion_publish_stop(ctx,0x8c617428u); r[0]=8u;goto L8c61742a; /* mov #0x8,r0 */
L8c61742a: sa_motion_publish_stop(ctx,0x8c61742au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61742c; /* fmac fr0,fr2,fr3 */
L8c61742c: sa_motion_publish_stop(ctx,0x8c61742cu); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61742e; /* fmov.s @(r0,r6),fr0 */
L8c61742e: sa_motion_publish_stop(ctx,0x8c61742eu); r[0]=32u;goto L8c617430; /* mov #0x20,r0 */
L8c617430: sa_motion_publish_stop(ctx,0x8c617430u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c617432; /* fmov.s @(r0,r2),fr2 */
L8c617432: sa_motion_publish_stop(ctx,0x8c617432u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c617434; /* mov.l @r15+,r2 */
L8c617434: sa_motion_publish_stop(ctx,0x8c617434u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617436; /* fmac fr0,fr2,fr3 */
L8c617436: sa_motion_publish_stop(ctx,0x8c617436u); fcopy(f,0u,5u);goto L8c617438; /* fmov fr5,fr0 */
L8c617438: sa_motion_publish_stop(ctx,0x8c617438u); fcopy(f,2u,5u);goto L8c61743a; /* fmov fr5,fr2 */
L8c61743a: sa_motion_publish_stop(ctx,0x8c61743au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61743c; /* fmac fr0,fr3,fr2 */
L8c61743c: sa_motion_publish_stop(ctx,0x8c61743cu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c61743e; /* ftrc fr2,FPUL */
L8c61743e: sa_motion_publish_stop(ctx,0x8c61743eu); r[3]=*fpul;goto L8c617440; /* sts FPUL,r3 */
L8c617440: sa_motion_publish_stop(ctx,0x8c617440u); wr(m,bus,&io_status,r[2],r[3],2);if(io_status)return io_status;goto L8c617442; /* mov.w r3,@r2 */
L8c617442: sa_motion_publish_stop(ctx,0x8c617442u); r[0]=36u;goto L8c617444; /* mov #0x24,r0 */
L8c617444: sa_motion_publish_stop(ctx,0x8c617444u); r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c617446; /* mov.l @r4,r3 */
L8c617446: sa_motion_publish_stop(ctx,0x8c617446u); r[1]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c617448; /* mov.l @(0x10,r15),r1 */
L8c617448: sa_motion_publish_stop(ctx,0x8c617448u); fload(m,bus,&io_status,f,1u,r[0]+r[3]);if(io_status)return io_status;goto L8c61744a; /* fmov.s @(r0,r3),fr1 */
L8c61744a: sa_motion_publish_stop(ctx,0x8c61744au); r[0]=4u;goto L8c61744c; /* mov #0x4,r0 */
L8c61744c: sa_motion_publish_stop(ctx,0x8c61744cu); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61744e; /* fmov.s @(r0,r6),fr0 */
L8c61744e: sa_motion_publish_stop(ctx,0x8c61744eu); r[0]=20u;goto L8c617450; /* mov #0x14,r0 */
L8c617450: sa_motion_publish_stop(ctx,0x8c617450u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c617452; /* fmov.s @(r0,r3),fr3 */
L8c617452: sa_motion_publish_stop(ctx,0x8c617452u); r[0]=4u;goto L8c617454; /* mov #0x4,r0 */
L8c617454: sa_motion_publish_stop(ctx,0x8c617454u); r[2]=r[3];goto L8c617456; /* mov r3,r2 */
L8c617456: sa_motion_publish_stop(ctx,0x8c617456u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c617458; /* fmov.s @(r0,r2),fr2 */
L8c617458: sa_motion_publish_stop(ctx,0x8c617458u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61745a; /* fmul fr0,fr3 */
L8c61745a: sa_motion_publish_stop(ctx,0x8c61745au); fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61745c; /* fmov.s @r6,fr0 */
L8c61745c: sa_motion_publish_stop(ctx,0x8c61745cu); r[0]=8u;goto L8c61745e; /* mov #0x8,r0 */
L8c61745e: sa_motion_publish_stop(ctx,0x8c61745eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617460; /* fmac fr0,fr2,fr3 */
L8c617460: sa_motion_publish_stop(ctx,0x8c617460u); fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c617462; /* fmov.s @(r0,r6),fr0 */
L8c617462: sa_motion_publish_stop(ctx,0x8c617462u); fcopy(f,2u,5u);goto L8c617464; /* fmov fr5,fr2 */
L8c617464: sa_motion_publish_stop(ctx,0x8c617464u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617466; /* fmac fr0,fr1,fr3 */
L8c617466: sa_motion_publish_stop(ctx,0x8c617466u); fcopy(f,0u,5u);goto L8c617468; /* fmov fr5,fr0 */
L8c617468: sa_motion_publish_stop(ctx,0x8c617468u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61746a; /* fmac fr0,fr3,fr2 */
L8c61746a: sa_motion_publish_stop(ctx,0x8c61746au); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c61746c; /* ftrc fr2,FPUL */
L8c61746c: sa_motion_publish_stop(ctx,0x8c61746cu); r[0]=*fpul;goto L8c61746e; /* sts FPUL,r0 */
L8c61746e: sa_motion_publish_stop(ctx,0x8c61746eu); wr(m,bus,&io_status,r[1]+0x2u,r[0],2);if(io_status)return io_status;goto L8c617470; /* mov.w r0,@(0x2,r1) */
L8c617470: sa_motion_publish_stop(ctx,0x8c617470u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c617472; /* mov.l @(0x8,r15),r3 */
L8c617472: sa_motion_publish_stop(ctx,0x8c617472u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c617474; /* mov.l @r3,r2 */
L8c617474: sa_motion_publish_stop(ctx,0x8c617474u); r[1]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c617476; /* mov.l @(0xc,r2),r1 */
L8c617476: sa_motion_publish_stop(ctx,0x8c617476u); *sr=(*sr&~1u)|!(r[5]&r[1]);goto L8c617478; /* tst r5,r1 */
L8c617478: sa_motion_publish_stop(ctx,0x8c617478u); if(!(*sr&1u))goto L8c6174d8;goto L8c61747a; /* bf 0x8c6174d8 */
L8c61747a: sa_motion_publish_stop(ctx,0x8c61747au); r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61747c; /* mov.l @r15,r6 */
L8c61747c: sa_motion_publish_stop(ctx,0x8c61747cu); r[0]=24u;goto L8c61747e; /* mov #0x18,r0 */
L8c61747e: sa_motion_publish_stop(ctx,0x8c61747eu); fcopy(f,7u,8u);goto L8c617480; /* fmov fr8,fr7 */
L8c617480: sa_motion_publish_stop(ctx,0x8c617480u); r[6]<<=2;goto L8c617482; /* shll2 r6 */
L8c617482: sa_motion_publish_stop(ctx,0x8c617482u); r[6]+=r[7];goto L8c617484; /* add r7,r6 */
L8c617484: sa_motion_publish_stop(ctx,0x8c617484u); r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c617486; /* mov.l @r6,r3 */
L8c617486: sa_motion_publish_stop(ctx,0x8c617486u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c617488; /* mov.l r3,@(0x8,r15) */
L8c617488: sa_motion_publish_stop(ctx,0x8c617488u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61748a; /* fmov.s @(r0,r3),fr3 */
L8c61748a: sa_motion_publish_stop(ctx,0x8c61748au); r[0]=12u;goto L8c61748c; /* mov #0xc,r0 */
L8c61748c: sa_motion_publish_stop(ctx,0x8c61748cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[7],&fr[3],&f->fpscr,&fr[7]);if(status)return status;}goto L8c61748e; /* fdiv fr3,fr7 */
L8c61748e: sa_motion_publish_stop(ctx,0x8c61748eu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c617490; /* fmov.s @(r0,r13),fr3 */
L8c617490: sa_motion_publish_stop(ctx,0x8c617490u); r[0]=16u;goto L8c617492; /* mov #0x10,r0 */
L8c617492: sa_motion_publish_stop(ctx,0x8c617492u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c617494; /* fmov.s @(r0,r3),fr2 */
L8c617494: sa_motion_publish_stop(ctx,0x8c617494u); r[3]=rd(m,bus,&io_status,0x8c6174d0u,4);if(io_status)return io_status;goto L8c617496; /* mov.l 0x8c6174d0,r3 */
L8c617496: sa_motion_publish_stop(ctx,0x8c617496u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c617498; /* fsub fr3,fr2 */
L8c617498: sa_motion_publish_stop(ctx,0x8c617498u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61749a; /* fmov.s @r3,fr3 */
L8c61749a: sa_motion_publish_stop(ctx,0x8c61749au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61749c; /* fmul fr7,fr2 */
L8c61749c: sa_motion_publish_stop(ctx,0x8c61749cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61749e; /* fdiv fr3,fr2 */
L8c61749e: sa_motion_publish_stop(ctx,0x8c61749eu); fstore(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c6174a0; /* fmov.s fr2,@r11 */
L8c6174a0: sa_motion_publish_stop(ctx,0x8c6174a0u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6174a2; /* fmov.s @(r0,r13),fr3 */
L8c6174a2: sa_motion_publish_stop(ctx,0x8c6174a2u); r[0]=20u;goto L8c6174a4; /* mov #0x14,r0 */
L8c6174a4: sa_motion_publish_stop(ctx,0x8c6174a4u); r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6174a6; /* mov.l @r6,r2 */
L8c6174a6: sa_motion_publish_stop(ctx,0x8c6174a6u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6174a8; /* fmov.s @(r0,r2),fr2 */
L8c6174a8: sa_motion_publish_stop(ctx,0x8c6174a8u); r[0]=4u;goto L8c6174aa; /* mov #0x4,r0 */
L8c6174aa: sa_motion_publish_stop(ctx,0x8c6174aau); r[2]=rd(m,bus,&io_status,0x8c6174d4u,4);if(io_status)return io_status;goto L8c6174ac; /* mov.l 0x8c6174d4,r2 */
L8c6174ac: sa_motion_publish_stop(ctx,0x8c6174acu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6174ae; /* fsub fr3,fr2 */
L8c6174ae: sa_motion_publish_stop(ctx,0x8c6174aeu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6174b0; /* fmov.s @r2,fr3 */
L8c6174b0: sa_motion_publish_stop(ctx,0x8c6174b0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6174b2; /* fmul fr7,fr2 */
L8c6174b2: sa_motion_publish_stop(ctx,0x8c6174b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6174b4; /* fdiv fr3,fr2 */
L8c6174b4: sa_motion_publish_stop(ctx,0x8c6174b4u); fstore(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c6174b6; /* fmov fr2,@(r0,r11) */
L8c6174b6: sa_motion_publish_stop(ctx,0x8c6174b6u); r[0]=8u;goto L8c6174b8; /* mov #0x8,r0 */
L8c6174b8: sa_motion_publish_stop(ctx,0x8c6174bau); fstore(m,bus,&io_status,f,7u,r[0]+r[11]);if(io_status)return io_status;goto L8c6174f4; /* bra 0x8c6174f4 */
L8c6174d8: sa_motion_publish_stop(ctx,0x8c6174d8u); r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c6174da; /* mov.l @r14,r3 */
L8c6174da: sa_motion_publish_stop(ctx,0x8c6174dau); r[0]=16u;goto L8c6174dc; /* mov #0x10,r0 */
L8c6174dc: sa_motion_publish_stop(ctx,0x8c6174dcu); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6174de; /* fmov.s @(r0,r3),fr3 */
L8c6174de: sa_motion_publish_stop(ctx,0x8c6174deu); r[0]=20u;goto L8c6174e0; /* mov #0x14,r0 */
L8c6174e0: sa_motion_publish_stop(ctx,0x8c6174e0u); fstore(m,bus,&io_status,f,3u,r[10]);if(io_status)return io_status;goto L8c6174e2; /* fmov.s fr3,@r10 */
L8c6174e2: sa_motion_publish_stop(ctx,0x8c6174e2u); r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c6174e4; /* mov.l @r14,r3 */
L8c6174e4: sa_motion_publish_stop(ctx,0x8c6174e4u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6174e6; /* fmov.s @(r0,r3),fr3 */
L8c6174e6: sa_motion_publish_stop(ctx,0x8c6174e6u); r[0]=4u;goto L8c6174e8; /* mov #0x4,r0 */
L8c6174e8: sa_motion_publish_stop(ctx,0x8c6174e8u); fstore(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c6174ea; /* fmov fr3,@(r0,r10) */
L8c6174ea: sa_motion_publish_stop(ctx,0x8c6174eau); r[0]=24u;goto L8c6174ec; /* mov #0x18,r0 */
L8c6174ec: sa_motion_publish_stop(ctx,0x8c6174ecu); r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c6174ee; /* mov.l @r14,r3 */
L8c6174ee: sa_motion_publish_stop(ctx,0x8c6174eeu); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6174f0; /* fmov.s @(r0,r3),fr3 */
L8c6174f0: sa_motion_publish_stop(ctx,0x8c6174f0u); r[0]=8u;goto L8c6174f2; /* mov #0x8,r0 */
L8c6174f2: sa_motion_publish_stop(ctx,0x8c6174f2u); fstore(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c6174f4; /* fmov fr3,@(r0,r10) */
L8c6174f4: sa_motion_publish_stop(ctx,0x8c6174f4u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6174f6; /* mov.l @r15,r3 */
L8c6174f6: sa_motion_publish_stop(ctx,0x8c6174f6u); r[2]=3u;goto L8c6174f8; /* mov #0x3,r2 */
L8c6174f8: sa_motion_publish_stop(ctx,0x8c6174f8u); r[10]+=12u;goto L8c6174fa; /* add #0xc,r10 */
L8c6174fa: sa_motion_publish_stop(ctx,0x8c6174fau); r[3]+=1u;goto L8c6174fc; /* add #0x1,r3 */
L8c6174fc: sa_motion_publish_stop(ctx,0x8c6174fcu); *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c6174fe; /* cmp/ge r2,r3 */
L8c6174fe: sa_motion_publish_stop(ctx,0x8c6174feu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c617500; /* mov.l r3,@r15 */
L8c617500: sa_motion_publish_stop(ctx,0x8c617500u); r[14]+=4u;goto L8c617502; /* add #0x4,r14 */
L8c617502: sa_motion_publish_stop(ctx,0x8c617504u); take=(*sr&1u);r[11]+=12u;if(take)goto L8c61750a;goto L8c617506; /* bt/s 0x8c61750a */
L8c617506: sa_motion_publish_stop(ctx,0x8c617508u); goto L8c617400; /* bra 0x8c617400 */
L8c61750a: sa_motion_publish_stop(ctx,0x8c61750au); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61750c; /* mov.l @(0x4,r15),r3 */
L8c61750c: sa_motion_publish_stop(ctx,0x8c61750cu); r[0]=8u;goto L8c61750e; /* mov #0x8,r0 */
L8c61750e: sa_motion_publish_stop(ctx,0x8c61750eu); r[4]=r[9];goto L8c617510; /* mov r9,r4 */
L8c617510: sa_motion_publish_stop(ctx,0x8c617510u); r[4]+=12u;goto L8c617512; /* add #0xc,r4 */
L8c617512: sa_motion_publish_stop(ctx,0x8c617512u); fload(m,bus,&io_status,f,7u,r[0]+r[3]);if(io_status)return io_status;goto L8c617514; /* fmov.s @(r0,r3),fr7 */
L8c617514: sa_motion_publish_stop(ctx,0x8c617514u); r[0]=4u;goto L8c617516; /* mov #0x4,r0 */
L8c617516: sa_motion_publish_stop(ctx,0x8c617516u); fload(m,bus,&io_status,f,11u,r[0]+r[4]);if(io_status)return io_status;goto L8c617518; /* fmov.s @(r0,r4),fr11 */
L8c617518: sa_motion_publish_stop(ctx,0x8c617518u); r[6]=r[9];goto L8c61751a; /* mov r9,r6 */
L8c61751a: sa_motion_publish_stop(ctx,0x8c61751au); fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c61751c; /* fmov.s @(r0,r3),fr9 */
L8c61751c: sa_motion_publish_stop(ctx,0x8c61751cu); r[0]=8u;goto L8c61751e; /* mov #0x8,r0 */
L8c61751e: sa_motion_publish_stop(ctx,0x8c61751eu); fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c617520; /* fmov.s @(r0,r4),fr5 */
L8c617520: sa_motion_publish_stop(ctx,0x8c617520u); r[0]=4u;goto L8c617522; /* mov #0x4,r0 */
L8c617522: sa_motion_publish_stop(ctx,0x8c617522u); fcopy(f,3u,9u);goto L8c617524; /* fmov fr9,fr3 */
L8c617524: sa_motion_publish_stop(ctx,0x8c617524u); r[6]+=24u;goto L8c617526; /* add #0x18,r6 */
L8c617526: sa_motion_publish_stop(ctx,0x8c617526u); fcopy(f,0u,7u);goto L8c617528; /* fmov fr7,fr0 */
L8c617528: sa_motion_publish_stop(ctx,0x8c617528u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61752a; /* fmul fr5,fr3 */
L8c61752a: sa_motion_publish_stop(ctx,0x8c61752au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[11],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61752c; /* fmul fr11,fr0 */
L8c61752c: sa_motion_publish_stop(ctx,0x8c61752cu); fload(m,bus,&io_status,f,10u,r[3]);if(io_status)return io_status;goto L8c61752e; /* fmov.s @r3,fr10 */
L8c61752e: sa_motion_publish_stop(ctx,0x8c61752eu); fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;goto L8c617530; /* fmov.s @r4,fr8 */
L8c617530: sa_motion_publish_stop(ctx,0x8c617530u); fcopy(f,2u,7u);goto L8c617532; /* fmov fr7,fr2 */
L8c617532: sa_motion_publish_stop(ctx,0x8c617532u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617534; /* fmul fr8,fr2 */
L8c617534: sa_motion_publish_stop(ctx,0x8c617534u); fcopy(f,1u,8u);goto L8c617536; /* fmov fr8,fr1 */
L8c617536: sa_motion_publish_stop(ctx,0x8c617536u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c617538; /* fsub fr3,fr0 */
L8c617538: sa_motion_publish_stop(ctx,0x8c617538u); fcopy(f,3u,10u);goto L8c61753a; /* fmov fr10,fr3 */
L8c61753a: sa_motion_publish_stop(ctx,0x8c61753au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61753c; /* fmul fr5,fr3 */
L8c61753c: sa_motion_publish_stop(ctx,0x8c61753cu); fcopy(f,12u,10u);goto L8c61753e; /* fmov fr10,fr12 */
L8c61753e: sa_motion_publish_stop(ctx,0x8c61753eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[9],&f->fpscr,&fr[1]);if(status)return status;}goto L8c617540; /* fmul fr9,fr1 */
L8c617540: sa_motion_publish_stop(ctx,0x8c617540u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617542; /* fsub fr2,fr3 */
L8c617542: sa_motion_publish_stop(ctx,0x8c617542u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c617544; /* fmov.s @(r0,r6),fr2 */
L8c617544: sa_motion_publish_stop(ctx,0x8c617544u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617546; /* fmul fr3,fr2 */
L8c617546: sa_motion_publish_stop(ctx,0x8c617546u); fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c617548; /* fmov.s @r6,fr3 */
L8c617548: sa_motion_publish_stop(ctx,0x8c617548u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61754a; /* fmac fr0,fr3,fr2 */
L8c61754a: sa_motion_publish_stop(ctx,0x8c61754au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[11],&f->fpscr,&fr[12]);if(status)return status;}goto L8c61754c; /* fmul fr11,fr12 */
L8c61754c: sa_motion_publish_stop(ctx,0x8c61754cu); r[0]=8u;goto L8c61754e; /* mov #0x8,r0 */
L8c61754e: sa_motion_publish_stop(ctx,0x8c61754eu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c617550; /* fmov.s @(r0,r6),fr3 */
L8c617550: sa_motion_publish_stop(ctx,0x8c617550u); r[4]=rd(m,bus,&io_status,0x8c6177a0u,4);if(io_status)return io_status;goto L8c617552; /* mov.l 0x8c6177a0,r4 */
L8c617552: sa_motion_publish_stop(ctx,0x8c617552u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[12],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c617554; /* fsub fr12,fr1 */
L8c617554: sa_motion_publish_stop(ctx,0x8c617554u); r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c617556; /* mov.l @r4,r3 */
L8c617556: sa_motion_publish_stop(ctx,0x8c617556u); fcopy(f,0u,1u);goto L8c617558; /* fmov fr1,fr0 */
L8c617558: sa_motion_publish_stop(ctx,0x8c617558u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61755a; /* fmac fr0,fr3,fr2 */
L8c61755a: sa_motion_publish_stop(ctx,0x8c61755au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61755c; /* fldi0 fr3 */
L8c61755c: sa_motion_publish_stop(ctx,0x8c61755cu); fcopy(f,5u,2u);goto L8c61755e; /* fmov fr2,fr5 */
L8c61755e: sa_motion_publish_stop(ctx,0x8c61755eu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c617560; /* fcmp/gt fr5,fr3 */
L8c617560: sa_motion_publish_stop(ctx,0x8c617560u); r[0]=*sr&1u;goto L8c617562; /* movt r0 */
L8c617562: sa_motion_publish_stop(ctx,0x8c617562u); *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c617564; /* tst r3,r0 */
L8c617564: sa_motion_publish_stop(ctx,0x8c617564u); if((*sr&1u))goto L8c61756a;goto L8c617566; /* bt 0x8c61756a */
L8c617566: sa_motion_publish_stop(ctx,0x8c617568u); goto L8c6178b4; /* bra 0x8c6178b4 */
L8c61756a: sa_motion_publish_stop(ctx,0x8c61756au); r[1]=0u;goto L8c61756c; /* mov #0x0,r1 */
L8c61756c: sa_motion_publish_stop(ctx,0x8c61756cu); r[6]=rd(m,bus,&io_status,0x8c6177a4u,4);if(io_status)return io_status;goto L8c61756e; /* mov.l 0x8c6177a4,r6 */
L8c61756e: sa_motion_publish_stop(ctx,0x8c61756eu); wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c617570; /* mov.l r1,@r4 */
L8c617570: sa_motion_publish_stop(ctx,0x8c617570u); r[3]=2u;goto L8c617572; /* mov #0x2,r3 */
L8c617572: sa_motion_publish_stop(ctx,0x8c617572u); r[4]=rd(m,bus,&io_status,0x8c6177a8u,4);if(io_status)return io_status;goto L8c617574; /* mov.l 0x8c6177a8,r4 */
L8c617574: sa_motion_publish_stop(ctx,0x8c617574u); r[14]=r[1];goto L8c617576; /* mov r1,r14 */
L8c617576: sa_motion_publish_stop(ctx,0x8c617576u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c617578; /* mov.l r3,@(0x4,r15) */
L8c617578: sa_motion_publish_stop(ctx,0x8c617578u); r[10]=r[14];goto L8c61757a; /* mov r14,r10 */
L8c61757a: sa_motion_publish_stop(ctx,0x8c61757au); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61757c; /* mov.l @(0x4,r15),r3 */
L8c61757c: sa_motion_publish_stop(ctx,0x8c61757cu); r[0]=r[7];goto L8c61757e; /* mov r7,r0 */
L8c61757e: sa_motion_publish_stop(ctx,0x8c61757eu); r[11]=r[10];goto L8c617580; /* mov r10,r11 */
L8c617580: sa_motion_publish_stop(ctx,0x8c617580u); r[11]<<=2;goto L8c617582; /* shll2 r11 */
L8c617582: sa_motion_publish_stop(ctx,0x8c617582u); r[3]<<=2;goto L8c617584; /* shll2 r3 */
L8c617584: sa_motion_publish_stop(ctx,0x8c617584u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c617586; /* mov.l r3,@r15 */
L8c617586: sa_motion_publish_stop(ctx,0x8c617586u); r[2]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c617588; /* mov.l @(r0,r3),r2 */
L8c617588: sa_motion_publish_stop(ctx,0x8c617588u); r[3]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c61758a; /* mov.l @(0xc,r2),r3 */
L8c61758a: sa_motion_publish_stop(ctx,0x8c61758au); r[2]=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;goto L8c61758c; /* mov.l @(r0,r11),r2 */
L8c61758c: sa_motion_publish_stop(ctx,0x8c61758cu); r[3]&=r[5];goto L8c61758e; /* and r5,r3 */
L8c61758e: sa_motion_publish_stop(ctx,0x8c61758eu); if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[2]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c617590; /* mov.l @(0xc,r2),r2 */
L8c617590: sa_motion_publish_stop(ctx,0x8c617590u); r[2]&=r[5];goto L8c617592; /* and r5,r2 */
L8c617592: sa_motion_publish_stop(ctx,0x8c617592u); r[3]^=r[2];goto L8c617594; /* xor r2,r3 */
L8c617594: sa_motion_publish_stop(ctx,0x8c617594u); r[0]=r[3];goto L8c617596; /* mov r3,r0 */
L8c617596: sa_motion_publish_stop(ctx,0x8c617596u); *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c617598; /* cmp/eq #0x1,r0 */
L8c617598: sa_motion_publish_stop(ctx,0x8c617598u); if(!(*sr&1u))goto L8c61767c;goto L8c61759a; /* bf 0x8c61767c */
L8c61759a: sa_motion_publish_stop(ctx,0x8c61759au); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61759c; /* mov.l @(0x4,r15),r2 */
L8c61759c: sa_motion_publish_stop(ctx,0x8c61759cu); r[0]=8u;goto L8c61759e; /* mov #0x8,r0 */
L8c61759e: sa_motion_publish_stop(ctx,0x8c61759eu); r[1]=r[10];goto L8c6175a0; /* mov r10,r1 */
L8c6175a0: sa_motion_publish_stop(ctx,0x8c6175a0u); r[3]=r[2];goto L8c6175a2; /* mov r2,r3 */
L8c6175a2: sa_motion_publish_stop(ctx,0x8c6175a2u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6175a4; /* shll r2 */
L8c6175a4: sa_motion_publish_stop(ctx,0x8c6175a4u); r[2]+=r[3];goto L8c6175a6; /* add r3,r2 */
L8c6175a6: sa_motion_publish_stop(ctx,0x8c6175a6u); r[3]=rd(m,bus,&io_status,0x8c6177acu,4);if(io_status)return io_status;goto L8c6175a8; /* mov.l 0x8c6177ac,r3 */
L8c6175a8: sa_motion_publish_stop(ctx,0x8c6175a8u); r[2]<<=2;goto L8c6175aa; /* shll2 r2 */
L8c6175aa: sa_motion_publish_stop(ctx,0x8c6175aau); r[2]+=r[9];goto L8c6175ac; /* add r9,r2 */
L8c6175ac: sa_motion_publish_stop(ctx,0x8c6175acu); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6175ae; /* mov.l r2,@(0x8,r15) */
L8c6175ae: sa_motion_publish_stop(ctx,0x8c6175aeu); fload(m,bus,&io_status,f,5u,r[0]+r[2]);if(io_status)return io_status;goto L8c6175b0; /* fmov.s @(r0,r2),fr5 */
L8c6175b0: sa_motion_publish_stop(ctx,0x8c6175b0u); r[2]=r[10];goto L8c6175b2; /* mov r10,r2 */
L8c6175b2: sa_motion_publish_stop(ctx,0x8c6175b2u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6175b4; /* shll r2 */
L8c6175b4: sa_motion_publish_stop(ctx,0x8c6175b4u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6175b6; /* fmov.s @r3,fr3 */
L8c6175b6: sa_motion_publish_stop(ctx,0x8c6175b6u); r[2]+=r[1];goto L8c6175b8; /* add r1,r2 */
L8c6175b8: sa_motion_publish_stop(ctx,0x8c6175b8u); r[2]<<=2;goto L8c6175ba; /* shll2 r2 */
L8c6175ba: sa_motion_publish_stop(ctx,0x8c6175bau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6175bc; /* fsub fr5,fr3 */
L8c6175bc: sa_motion_publish_stop(ctx,0x8c6175bcu); r[0]=8u;goto L8c6175be; /* mov #0x8,r0 */
L8c6175be: sa_motion_publish_stop(ctx,0x8c6175beu); r[2]+=r[9];goto L8c6175c0; /* add r9,r2 */
L8c6175c0: sa_motion_publish_stop(ctx,0x8c6175c0u); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6175c2; /* mov.l r2,@(0xc,r15) */
L8c6175c2: sa_motion_publish_stop(ctx,0x8c6175c2u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6175c4; /* fmov.s @(r0,r2),fr2 */
L8c6175c4: sa_motion_publish_stop(ctx,0x8c6175c4u); r[1]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6175c6; /* mov.l @(0xc,r15),r1 */
L8c6175c6: sa_motion_publish_stop(ctx,0x8c6175c6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6175c8; /* fsub fr5,fr2 */
L8c6175c8: sa_motion_publish_stop(ctx,0x8c6175c8u); fcopy(f,5u,3u);goto L8c6175ca; /* fmov fr3,fr5 */
L8c6175ca: sa_motion_publish_stop(ctx,0x8c6175cau); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6175cc; /* mov.l @(0x8,r15),r2 */
L8c6175cc: sa_motion_publish_stop(ctx,0x8c6175ccu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6175ce; /* fmov.s @r1,fr3 */
L8c6175ce: sa_motion_publish_stop(ctx,0x8c6175ceu); fload(m,bus,&io_status,f,7u,r[2]);if(io_status)return io_status;goto L8c6175d0; /* fmov.s @r2,fr7 */
L8c6175d0: sa_motion_publish_stop(ctx,0x8c6175d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[2],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6175d2; /* fdiv fr2,fr5 */
L8c6175d2: sa_motion_publish_stop(ctx,0x8c6175d2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6175d4; /* fsub fr7,fr3 */
L8c6175d4: sa_motion_publish_stop(ctx,0x8c6175d4u); fcopy(f,2u,7u);goto L8c6175d6; /* fmov fr7,fr2 */
L8c6175d6: sa_motion_publish_stop(ctx,0x8c6175d6u); fcopy(f,0u,5u);goto L8c6175d8; /* fmov fr5,fr0 */
L8c6175d8: sa_motion_publish_stop(ctx,0x8c6175d8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6175da; /* fmac fr0,fr3,fr2 */
L8c6175da: sa_motion_publish_stop(ctx,0x8c6175dau); r[1]=rd(m,bus,&io_status,0x8c6177b0u,4);if(io_status)return io_status;goto L8c6175dc; /* mov.l 0x8c6177b0,r1 */
L8c6175dc: sa_motion_publish_stop(ctx,0x8c6175dcu); r[0]=12u;goto L8c6175de; /* mov #0xc,r0 */
L8c6175de: sa_motion_publish_stop(ctx,0x8c6175deu); fcopy(f,7u,2u);goto L8c6175e0; /* fmov fr2,fr7 */
L8c6175e0: sa_motion_publish_stop(ctx,0x8c6175e0u); r[2]=r[14];goto L8c6175e2; /* mov r14,r2 */
L8c6175e2: sa_motion_publish_stop(ctx,0x8c6175e2u); fcopy(f,0u,6u);goto L8c6175e4; /* fmov fr6,fr0 */
L8c6175e4: sa_motion_publish_stop(ctx,0x8c6175e4u); r[2]<<=2;goto L8c6175e6; /* shll2 r2 */
L8c6175e6: sa_motion_publish_stop(ctx,0x8c6175e6u); r[2]<<=2;goto L8c6175e8; /* shll2 r2 */
L8c6175e8: sa_motion_publish_stop(ctx,0x8c6175e8u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6175ea; /* shll r2 */
L8c6175ea: sa_motion_publish_stop(ctx,0x8c6175eau); r[2]+=r[4];goto L8c6175ec; /* add r4,r2 */
L8c6175ec: sa_motion_publish_stop(ctx,0x8c6175ecu); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6175ee; /* mov.l r2,@(0x4,r15) */
L8c6175ee: sa_motion_publish_stop(ctx,0x8c6175eeu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6175f0; /* fmov.s @r1,fr3 */
L8c6175f0: sa_motion_publish_stop(ctx,0x8c6175f0u); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c6175f2; /* fmov.s @(r0,r13),fr2 */
L8c6175f2: sa_motion_publish_stop(ctx,0x8c6175f2u); r[0]=16u;goto L8c6175f4; /* mov #0x10,r0 */
L8c6175f4: sa_motion_publish_stop(ctx,0x8c6175f4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6175f6; /* fmul fr7,fr3 */
L8c6175f6: sa_motion_publish_stop(ctx,0x8c6175f6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6175f8; /* fmac fr0,fr3,fr2 */
L8c6175f8: sa_motion_publish_stop(ctx,0x8c6175f8u); fcopy(f,0u,5u);goto L8c6175fa; /* fmov fr5,fr0 */
L8c6175fa: sa_motion_publish_stop(ctx,0x8c6175fau); fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6175fc; /* fmov fr2,@(r0,r2) */
L8c6175fc: sa_motion_publish_stop(ctx,0x8c6175fcu); r[0]=4u;goto L8c6175fe; /* mov #0x4,r0 */
L8c6175fe: sa_motion_publish_stop(ctx,0x8c6175feu); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c617600; /* mov.l @(0x8,r15),r2 */
L8c617600: sa_motion_publish_stop(ctx,0x8c617600u); fload(m,bus,&io_status,f,7u,r[0]+r[2]);if(io_status)return io_status;goto L8c617602; /* fmov.s @(r0,r2),fr7 */
L8c617602: sa_motion_publish_stop(ctx,0x8c617602u); r[0]=4u;goto L8c617604; /* mov #0x4,r0 */
L8c617604: sa_motion_publish_stop(ctx,0x8c617604u); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c617606; /* mov.l @(0xc,r15),r2 */
L8c617606: sa_motion_publish_stop(ctx,0x8c617606u); fcopy(f,2u,7u);goto L8c617608; /* fmov fr7,fr2 */
L8c617608: sa_motion_publish_stop(ctx,0x8c617608u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61760a; /* fmov.s @(r0,r2),fr3 */
L8c61760a: sa_motion_publish_stop(ctx,0x8c61760au); r[0]=rd(m,bus,&io_status,0x8c6177b4u,4);if(io_status)return io_status;goto L8c61760c; /* mov.l 0x8c6177b4,r0 */
L8c61760c: sa_motion_publish_stop(ctx,0x8c61760cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61760e; /* fsub fr7,fr3 */
L8c61760e: sa_motion_publish_stop(ctx,0x8c61760eu); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c617610; /* mov.l @(0x4,r15),r2 */
L8c617610: sa_motion_publish_stop(ctx,0x8c617610u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617612; /* fmac fr0,fr3,fr2 */
L8c617612: sa_motion_publish_stop(ctx,0x8c617612u); fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c617614; /* fmov.s @r0,fr3 */
L8c617614: sa_motion_publish_stop(ctx,0x8c617614u); r[0]=16u;goto L8c617616; /* mov #0x10,r0 */
L8c617616: sa_motion_publish_stop(ctx,0x8c617616u); fcopy(f,7u,2u);goto L8c617618; /* fmov fr2,fr7 */
L8c617618: sa_motion_publish_stop(ctx,0x8c617618u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61761a; /* fmul fr7,fr3 */
L8c61761a: sa_motion_publish_stop(ctx,0x8c61761au); fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61761c; /* fmov.s @(r0,r13),fr2 */
L8c61761c: sa_motion_publish_stop(ctx,0x8c61761cu); r[0]=20u;goto L8c61761e; /* mov #0x14,r0 */
L8c61761e: sa_motion_publish_stop(ctx,0x8c61761eu); fcopy(f,0u,6u);goto L8c617620; /* fmov fr6,fr0 */
L8c617620: sa_motion_publish_stop(ctx,0x8c617620u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617622; /* fmac fr0,fr3,fr2 */
L8c617622: sa_motion_publish_stop(ctx,0x8c617622u); fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c617624; /* fmov fr2,@(r0,r2) */
L8c617624: sa_motion_publish_stop(ctx,0x8c617624u); r[0]=24u;goto L8c617626; /* mov #0x18,r0 */
L8c617626: sa_motion_publish_stop(ctx,0x8c617626u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c617628; /* mov.l @(0x4,r15),r3 */
L8c617628: sa_motion_publish_stop(ctx,0x8c617628u); r[2]=r[11];goto L8c61762a; /* mov r11,r2 */
L8c61762a: sa_motion_publish_stop(ctx,0x8c61762au); r[2]+=r[12];goto L8c61762c; /* add r12,r2 */
L8c61762c: sa_motion_publish_stop(ctx,0x8c61762cu); fstore(m,bus,&io_status,f,0u,r[0]+r[3]);if(io_status)return io_status;goto L8c61762e; /* fmov fr0,@(r0,r3) */
L8c61762e: sa_motion_publish_stop(ctx,0x8c61762eu); r[3]=r[14];goto L8c617630; /* mov r14,r3 */
L8c617630: sa_motion_publish_stop(ctx,0x8c617630u); fcopy(f,0u,5u);goto L8c617632; /* fmov fr5,fr0 */
L8c617632: sa_motion_publish_stop(ctx,0x8c617632u); r[3]<<=2;goto L8c617634; /* shll2 r3 */
L8c617634: sa_motion_publish_stop(ctx,0x8c617634u); r[3]+=r[6];goto L8c617636; /* add r6,r3 */
L8c617636: sa_motion_publish_stop(ctx,0x8c617636u); wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c617638; /* mov.l r3,@(0x10,r15) */
L8c617638: sa_motion_publish_stop(ctx,0x8c617638u); r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61763a; /* mov.l @r15,r0 */
L8c61763a: sa_motion_publish_stop(ctx,0x8c61763au); r[0]+=r[12];goto L8c61763c; /* add r12,r0 */
L8c61763c: sa_motion_publish_stop(ctx,0x8c61763cu); wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;goto L8c61763e; /* mov.l r0,@(0x8,r15) */
L8c61763e: sa_motion_publish_stop(ctx,0x8c61763eu); r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c617640; /* mov.w @r0,r0 */
L8c617640: sa_motion_publish_stop(ctx,0x8c617640u); wr(m,bus,&io_status,r[15]+0xcu,r[0],2);if(io_status)return io_status;goto L8c617642; /* mov.w r0,@(0xc,r15) */
L8c617642: sa_motion_publish_stop(ctx,0x8c617642u); *fpul=r[0];goto L8c617644; /* lds r0,FPUL */
L8c617644: sa_motion_publish_stop(ctx,0x8c617644u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c617646; /* mov.l r2,@(0x4,r15) */
L8c617646: sa_motion_publish_stop(ctx,0x8c617646u); r[0]=sx(m,bus,&io_status,r[15]+0xcu,2);if(io_status)return io_status;goto L8c617648; /* mov.w @(0xc,r15),r0 */
L8c617648: sa_motion_publish_stop(ctx,0x8c617648u); r[2]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c61764a; /* mov.w @r2,r2 */
L8c61764a: sa_motion_publish_stop(ctx,0x8c61764au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61764c; /* float FPUL,fr3 */
L8c61764c: sa_motion_publish_stop(ctx,0x8c61764cu); r[2]-=r[0];goto L8c61764e; /* sub r0,r2 */
L8c61764e: sa_motion_publish_stop(ctx,0x8c61764eu); *fpul=r[2];goto L8c617650; /* lds r2,FPUL */
L8c617650: sa_motion_publish_stop(ctx,0x8c617650u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c617652; /* float FPUL,fr2 */
L8c617652: sa_motion_publish_stop(ctx,0x8c617652u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617654; /* fmac fr0,fr2,fr3 */
L8c617654: sa_motion_publish_stop(ctx,0x8c617654u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c617656; /* ftrc fr3,FPUL */
L8c617656: sa_motion_publish_stop(ctx,0x8c617656u); r[2]=*fpul;goto L8c617658; /* sts FPUL,r2 */
L8c617658: sa_motion_publish_stop(ctx,0x8c617658u); wr(m,bus,&io_status,r[3],r[2],2);if(io_status)return io_status;goto L8c61765a; /* mov.w r2,@r3 */
L8c61765a: sa_motion_publish_stop(ctx,0x8c61765au); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61765c; /* mov.l @(0x8,r15),r2 */
L8c61765c: sa_motion_publish_stop(ctx,0x8c61765cu); r[14]+=1u;goto L8c61765e; /* add #0x1,r14 */
L8c61765e: sa_motion_publish_stop(ctx,0x8c61765eu); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c617660; /* mov.l @(0x10,r15),r3 */
L8c617660: sa_motion_publish_stop(ctx,0x8c617660u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c617662; /* mov.w @(0x2,r2),r0 */
L8c617662: sa_motion_publish_stop(ctx,0x8c617662u); *fpul=r[0];goto L8c617664; /* lds r0,FPUL */
L8c617664: sa_motion_publish_stop(ctx,0x8c617664u); wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c617666; /* mov.w r0,@r15 */
L8c617666: sa_motion_publish_stop(ctx,0x8c617666u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c617668; /* mov.l @(0x4,r15),r2 */
L8c617668: sa_motion_publish_stop(ctx,0x8c617668u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61766a; /* float FPUL,fr3 */
L8c61766a: sa_motion_publish_stop(ctx,0x8c61766au); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61766c; /* mov.w @(0x2,r2),r0 */
L8c61766c: sa_motion_publish_stop(ctx,0x8c61766cu); r[2]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c61766e; /* mov.w @r15,r2 */
L8c61766e: sa_motion_publish_stop(ctx,0x8c61766eu); r[0]-=r[2];goto L8c617670; /* sub r2,r0 */
L8c617670: sa_motion_publish_stop(ctx,0x8c617670u); *fpul=r[0];goto L8c617672; /* lds r0,FPUL */
L8c617672: sa_motion_publish_stop(ctx,0x8c617672u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c617674; /* float FPUL,fr2 */
L8c617674: sa_motion_publish_stop(ctx,0x8c617674u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617676; /* fmac fr0,fr2,fr3 */
L8c617676: sa_motion_publish_stop(ctx,0x8c617676u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c617678; /* ftrc fr3,FPUL */
L8c617678: sa_motion_publish_stop(ctx,0x8c617678u); r[0]=*fpul;goto L8c61767a; /* sts FPUL,r0 */
L8c61767a: sa_motion_publish_stop(ctx,0x8c61767au); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61767c; /* mov.w r0,@(0x2,r3) */
L8c61767c: sa_motion_publish_stop(ctx,0x8c61767cu); r[0]=r[11];goto L8c61767e; /* mov r11,r0 */
L8c61767e: sa_motion_publish_stop(ctx,0x8c61767eu); r[3]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c617680; /* mov.l @(r0,r7),r3 */
L8c617680: sa_motion_publish_stop(ctx,0x8c617680u); r[3]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c617682; /* mov.l @(0xc,r3),r3 */
L8c617682: sa_motion_publish_stop(ctx,0x8c617682u); *sr=(*sr&~1u)|!(r[5]&r[3]);goto L8c617684; /* tst r5,r3 */
L8c617684: sa_motion_publish_stop(ctx,0x8c617684u); if(!(*sr&1u))goto L8c6176d4;goto L8c617686; /* bf 0x8c6176d4 */
L8c617686: sa_motion_publish_stop(ctx,0x8c617686u); r[3]=r[11];goto L8c617688; /* mov r11,r3 */
L8c617688: sa_motion_publish_stop(ctx,0x8c617688u); r[2]=r[14];goto L8c61768a; /* mov r14,r2 */
L8c61768a: sa_motion_publish_stop(ctx,0x8c61768au); r[3]+=r[7];goto L8c61768c; /* add r7,r3 */
L8c61768c: sa_motion_publish_stop(ctx,0x8c61768cu); r[2]<<=2;goto L8c61768e; /* shll2 r2 */
L8c61768e: sa_motion_publish_stop(ctx,0x8c61768eu); r[0]=16u;goto L8c617690; /* mov #0x10,r0 */
L8c617690: sa_motion_publish_stop(ctx,0x8c617690u); r[2]<<=2;goto L8c617692; /* shll2 r2 */
L8c617692: sa_motion_publish_stop(ctx,0x8c617692u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c617694; /* shll r2 */
L8c617694: sa_motion_publish_stop(ctx,0x8c617694u); r[2]+=r[4];goto L8c617696; /* add r4,r2 */
L8c617696: sa_motion_publish_stop(ctx,0x8c617696u); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c617698; /* mov.l r2,@r15 */
L8c617698: sa_motion_publish_stop(ctx,0x8c617698u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61769a; /* mov.l r3,@(0x4,r15) */
L8c61769a: sa_motion_publish_stop(ctx,0x8c61769au); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61769c; /* mov.l @r3,r1 */
L8c61769c: sa_motion_publish_stop(ctx,0x8c61769cu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61769e; /* fmov.s @(r0,r1),fr3 */
L8c61769e: sa_motion_publish_stop(ctx,0x8c61769eu); fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c6176a0; /* fmov fr3,@(r0,r2) */
L8c6176a0: sa_motion_publish_stop(ctx,0x8c6176a0u); r[0]=20u;goto L8c6176a2; /* mov #0x14,r0 */
L8c6176a2: sa_motion_publish_stop(ctx,0x8c6176a2u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6176a4; /* mov.l @(0x4,r15),r2 */
L8c6176a4: sa_motion_publish_stop(ctx,0x8c6176a4u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6176a6; /* mov.l @r15,r3 */
L8c6176a6: sa_motion_publish_stop(ctx,0x8c6176a6u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6176a8; /* mov.l @r2,r1 */
L8c6176a8: sa_motion_publish_stop(ctx,0x8c6176a8u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6176aa; /* fmov.s @(r0,r1),fr3 */
L8c6176aa: sa_motion_publish_stop(ctx,0x8c6176aau); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6176ac; /* fmov fr3,@(r0,r3) */
L8c6176ac: sa_motion_publish_stop(ctx,0x8c6176acu); r[0]=24u;goto L8c6176ae; /* mov #0x18,r0 */
L8c6176ae: sa_motion_publish_stop(ctx,0x8c6176aeu); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6176b0; /* mov.l @(0x4,r15),r2 */
L8c6176b0: sa_motion_publish_stop(ctx,0x8c6176b0u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6176b2; /* mov.l @r15,r3 */
L8c6176b2: sa_motion_publish_stop(ctx,0x8c6176b2u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6176b4; /* mov.l @r2,r1 */
L8c6176b4: sa_motion_publish_stop(ctx,0x8c6176b4u); r[2]=r[11];goto L8c6176b6; /* mov r11,r2 */
L8c6176b6: sa_motion_publish_stop(ctx,0x8c6176b6u); r[2]+=r[12];goto L8c6176b8; /* add r12,r2 */
L8c6176b8: sa_motion_publish_stop(ctx,0x8c6176b8u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6176ba; /* fmov.s @(r0,r1),fr3 */
L8c6176ba: sa_motion_publish_stop(ctx,0x8c6176bau); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6176bc; /* fmov fr3,@(r0,r3) */
L8c6176bc: sa_motion_publish_stop(ctx,0x8c6176bcu); r[3]=r[14];goto L8c6176be; /* mov r14,r3 */
L8c6176be: sa_motion_publish_stop(ctx,0x8c6176beu); r[3]<<=2;goto L8c6176c0; /* shll2 r3 */
L8c6176c0: sa_motion_publish_stop(ctx,0x8c6176c0u); r[3]+=r[6];goto L8c6176c2; /* add r6,r3 */
L8c6176c2: sa_motion_publish_stop(ctx,0x8c6176c2u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6176c4; /* mov.l r3,@r15 */
L8c6176c4: sa_motion_publish_stop(ctx,0x8c6176c4u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6176c6; /* mov.l r2,@(0x4,r15) */
L8c6176c6: sa_motion_publish_stop(ctx,0x8c6176c6u); r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c6176c8; /* mov.w @r2,r1 */
L8c6176c8: sa_motion_publish_stop(ctx,0x8c6176c8u); r[14]+=1u;goto L8c6176ca; /* add #0x1,r14 */
L8c6176ca: sa_motion_publish_stop(ctx,0x8c6176cau); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c6176cc; /* mov.w r1,@r3 */
L8c6176cc: sa_motion_publish_stop(ctx,0x8c6176ccu); r[0]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6176ce; /* mov.l @(0x4,r15),r0 */
L8c6176ce: sa_motion_publish_stop(ctx,0x8c6176ceu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6176d0; /* mov.l @r15,r3 */
L8c6176d0: sa_motion_publish_stop(ctx,0x8c6176d0u); r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c6176d2; /* mov.w @(0x2,r0),r0 */
L8c6176d2: sa_motion_publish_stop(ctx,0x8c6176d2u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6176d4; /* mov.w r0,@(0x2,r3) */
L8c6176d4: sa_motion_publish_stop(ctx,0x8c6176d4u); wr(m,bus,&io_status,r[15]+0x4u,r[10],4);if(io_status)return io_status;goto L8c6176d6; /* mov.l r10,@(0x4,r15) */
L8c6176d6: sa_motion_publish_stop(ctx,0x8c6176d6u); r[3]=3u;goto L8c6176d8; /* mov #0x3,r3 */
L8c6176d8: sa_motion_publish_stop(ctx,0x8c6176d8u); r[10]+=1u;goto L8c6176da; /* add #0x1,r10 */
L8c6176da: sa_motion_publish_stop(ctx,0x8c6176dau); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[3]);goto L8c6176dc; /* cmp/ge r3,r10 */
L8c6176dc: sa_motion_publish_stop(ctx,0x8c6176dcu); if((*sr&1u))goto L8c6176e2;goto L8c6176de; /* bt 0x8c6176e2 */
L8c6176de: sa_motion_publish_stop(ctx,0x8c6176e0u); goto L8c61757a; /* bra 0x8c61757a */
L8c6176e2: sa_motion_publish_stop(ctx,0x8c6176e2u); r[0]=0x8c6177c0u;goto L8c6176e4; /* mova 0x8c6177c0,r0 */
L8c6176e4: sa_motion_publish_stop(ctx,0x8c6176e4u); r[5]=rd(m,bus,&io_status,0x8c6177bcu,4);if(io_status)return io_status;goto L8c6176e6; /* mov.l 0x8c6177bc,r5 */
L8c6176e6: sa_motion_publish_stop(ctx,0x8c6176e6u); fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c6176e8; /* fmov.s @r0,fr5 */
L8c6176e8: sa_motion_publish_stop(ctx,0x8c6176e8u); r[0]=r[14];goto L8c6176ea; /* mov r14,r0 */
L8c6176ea: sa_motion_publish_stop(ctx,0x8c6176eau); r[7]=rd(m,bus,&io_status,0x8c6177b8u,4);if(io_status)return io_status;goto L8c6176ec; /* mov.l 0x8c6177b8,r7 */
L8c6176ec: sa_motion_publish_stop(ctx,0x8c6176ecu); *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c6176ee; /* cmp/eq #0x3,r0 */
L8c6176ee: sa_motion_publish_stop(ctx,0x8c6176eeu); if(!(*sr&1u))goto L8c6177c8;goto L8c6176f0; /* bf 0x8c6177c8 */
L8c6176f0: sa_motion_publish_stop(ctx,0x8c6176f0u); r[3]=r[6];goto L8c6176f2; /* mov r6,r3 */
L8c6176f2: sa_motion_publish_stop(ctx,0x8c6176f2u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6176f4; /* mov.l r3,@r15 */
L8c6176f4: sa_motion_publish_stop(ctx,0x8c6176f4u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6176f6; /* mov.w @(0x2,r3),r0 */
L8c6176f6: sa_motion_publish_stop(ctx,0x8c6176f6u); r[8]+=24u;goto L8c6176f8; /* add #0x18,r8 */
L8c6176f8: sa_motion_publish_stop(ctx,0x8c6176f8u); r[14]=r[4];goto L8c6176fa; /* mov r4,r14 */
L8c6176fa: sa_motion_publish_stop(ctx,0x8c6176fau); r[3]=r[0];goto L8c6176fc; /* mov r0,r3 */
L8c6176fc: sa_motion_publish_stop(ctx,0x8c6176fcu); *fpul=r[3];goto L8c6176fe; /* lds r3,FPUL */
L8c6176fe: sa_motion_publish_stop(ctx,0x8c6176feu); r[0]=24u;goto L8c617700; /* mov #0x18,r0 */
L8c617700: sa_motion_publish_stop(ctx,0x8c617700u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617702; /* float FPUL,fr3 */
L8c617702: sa_motion_publish_stop(ctx,0x8c617702u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617704; /* fmul fr5,fr3 */
L8c617704: sa_motion_publish_stop(ctx,0x8c617704u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617706; /* fmov.s fr3,@-r8 */
L8c617706: sa_motion_publish_stop(ctx,0x8c617706u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617708; /* mov.l @r15,r3 */
L8c617708: sa_motion_publish_stop(ctx,0x8c617708u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c61770a; /* mov.w @r3,r3 */
L8c61770a: sa_motion_publish_stop(ctx,0x8c61770au); *fpul=r[3];goto L8c61770c; /* lds r3,FPUL */
L8c61770c: sa_motion_publish_stop(ctx,0x8c61770cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61770e; /* float FPUL,fr3 */
L8c61770e: sa_motion_publish_stop(ctx,0x8c61770eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617710; /* fmul fr5,fr3 */
L8c617710: sa_motion_publish_stop(ctx,0x8c617710u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617712; /* fmov.s fr3,@-r8 */
L8c617712: sa_motion_publish_stop(ctx,0x8c617712u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617714; /* fmov.s @(r0,r14),fr3 */
L8c617714: sa_motion_publish_stop(ctx,0x8c617714u); r[0]=20u;goto L8c617716; /* mov #0x14,r0 */
L8c617716: sa_motion_publish_stop(ctx,0x8c617716u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617718; /* fmov.s fr3,@-r8 */
L8c617718: sa_motion_publish_stop(ctx,0x8c617718u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61771a; /* fmov.s @(r0,r14),fr3 */
L8c61771a: sa_motion_publish_stop(ctx,0x8c61771au); r[0]=16u;goto L8c61771c; /* mov #0x10,r0 */
L8c61771c: sa_motion_publish_stop(ctx,0x8c61771cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61771e; /* fmov.s fr3,@-r8 */
L8c61771e: sa_motion_publish_stop(ctx,0x8c61771eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617720; /* fmov.s @(r0,r14),fr3 */
L8c617720: sa_motion_publish_stop(ctx,0x8c617720u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617722; /* fmov.s fr3,@-r8 */
L8c617722: sa_motion_publish_stop(ctx,0x8c617722u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c617724; /* mov.l r5,@-r8 */
L8c617724: sa_motion_publish_stop(ctx,0x8c617724u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617726; /* pref @r8 */
L8c617726: sa_motion_publish_stop(ctx,0x8c617726u); r[3]=r[6];goto L8c617728; /* mov r6,r3 */
L8c617728: sa_motion_publish_stop(ctx,0x8c617728u); r[3]+=4u;goto L8c61772a; /* add #0x4,r3 */
L8c61772a: sa_motion_publish_stop(ctx,0x8c61772au); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61772c; /* mov.l r3,@r15 */
L8c61772c: sa_motion_publish_stop(ctx,0x8c61772cu); r[8]+=56u;goto L8c61772e; /* add #0x38,r8 */
L8c61772e: sa_motion_publish_stop(ctx,0x8c61772eu); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c617730; /* mov.w @(0x2,r3),r0 */
L8c617730: sa_motion_publish_stop(ctx,0x8c617730u); r[14]+=32u;goto L8c617732; /* add #0x20,r14 */
L8c617732: sa_motion_publish_stop(ctx,0x8c617732u); r[3]=r[0];goto L8c617734; /* mov r0,r3 */
L8c617734: sa_motion_publish_stop(ctx,0x8c617734u); *fpul=r[3];goto L8c617736; /* lds r3,FPUL */
L8c617736: sa_motion_publish_stop(ctx,0x8c617736u); r[0]=24u;goto L8c617738; /* mov #0x18,r0 */
L8c617738: sa_motion_publish_stop(ctx,0x8c617738u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61773a; /* float FPUL,fr3 */
L8c61773a: sa_motion_publish_stop(ctx,0x8c61773au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61773c; /* fmul fr5,fr3 */
L8c61773c: sa_motion_publish_stop(ctx,0x8c61773cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61773e; /* fmov.s fr3,@-r8 */
L8c61773e: sa_motion_publish_stop(ctx,0x8c61773eu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617740; /* mov.l @r15,r3 */
L8c617740: sa_motion_publish_stop(ctx,0x8c617740u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c617742; /* mov.w @r3,r3 */
L8c617742: sa_motion_publish_stop(ctx,0x8c617742u); *fpul=r[3];goto L8c617744; /* lds r3,FPUL */
L8c617744: sa_motion_publish_stop(ctx,0x8c617744u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617746; /* float FPUL,fr3 */
L8c617746: sa_motion_publish_stop(ctx,0x8c617746u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617748; /* fmul fr5,fr3 */
L8c617748: sa_motion_publish_stop(ctx,0x8c617748u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61774a; /* fmov.s fr3,@-r8 */
L8c61774a: sa_motion_publish_stop(ctx,0x8c61774au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61774c; /* fmov.s @(r0,r14),fr3 */
L8c61774c: sa_motion_publish_stop(ctx,0x8c61774cu); r[0]=20u;goto L8c61774e; /* mov #0x14,r0 */
L8c61774e: sa_motion_publish_stop(ctx,0x8c61774eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617750; /* fmov.s fr3,@-r8 */
L8c617750: sa_motion_publish_stop(ctx,0x8c617750u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617752; /* fmov.s @(r0,r14),fr3 */
L8c617752: sa_motion_publish_stop(ctx,0x8c617752u); r[0]=16u;goto L8c617754; /* mov #0x10,r0 */
L8c617754: sa_motion_publish_stop(ctx,0x8c617754u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617756; /* fmov.s fr3,@-r8 */
L8c617756: sa_motion_publish_stop(ctx,0x8c617756u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617758; /* fmov.s @(r0,r14),fr3 */
L8c617758: sa_motion_publish_stop(ctx,0x8c617758u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61775a; /* fmov.s fr3,@-r8 */
L8c61775a: sa_motion_publish_stop(ctx,0x8c61775au); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61775c; /* mov.l r5,@-r8 */
L8c61775c: sa_motion_publish_stop(ctx,0x8c61775cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61775e; /* pref @r8 */
L8c61775e: sa_motion_publish_stop(ctx,0x8c61775eu); r[5]=r[6];goto L8c617760; /* mov r6,r5 */
L8c617760: sa_motion_publish_stop(ctx,0x8c617760u); r[5]+=8u;goto L8c617762; /* add #0x8,r5 */
L8c617762: sa_motion_publish_stop(ctx,0x8c617762u); r[8]+=60u;goto L8c617764; /* add #0x3c,r8 */
L8c617764: sa_motion_publish_stop(ctx,0x8c617764u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c617766; /* fmov.s fr4,@-r8 */
L8c617766: sa_motion_publish_stop(ctx,0x8c617766u); r[0]=sx(m,bus,&io_status,r[5]+0x2u,2);if(io_status)return io_status;goto L8c617768; /* mov.w @(0x2,r5),r0 */
L8c617768: sa_motion_publish_stop(ctx,0x8c617768u); r[3]=r[0];goto L8c61776a; /* mov r0,r3 */
L8c61776a: sa_motion_publish_stop(ctx,0x8c61776au); *fpul=r[3];goto L8c61776c; /* lds r3,FPUL */
L8c61776c: sa_motion_publish_stop(ctx,0x8c61776cu); r[0]=24u;goto L8c61776e; /* mov #0x18,r0 */
L8c61776e: sa_motion_publish_stop(ctx,0x8c61776eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617770; /* float FPUL,fr3 */
L8c617770: sa_motion_publish_stop(ctx,0x8c617770u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617772; /* fmul fr5,fr3 */
L8c617772: sa_motion_publish_stop(ctx,0x8c617772u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617774; /* fmov.s fr3,@-r8 */
L8c617774: sa_motion_publish_stop(ctx,0x8c617774u); r[3]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;goto L8c617776; /* mov.w @r5,r3 */
L8c617776: sa_motion_publish_stop(ctx,0x8c617776u); r[5]=r[4];goto L8c617778; /* mov r4,r5 */
L8c617778: sa_motion_publish_stop(ctx,0x8c617778u); r[5]+=64u;goto L8c61777a; /* add #0x40,r5 */
L8c61777a: sa_motion_publish_stop(ctx,0x8c61777au); *fpul=r[3];goto L8c61777c; /* lds r3,FPUL */
L8c61777c: sa_motion_publish_stop(ctx,0x8c61777cu); r[3]=rd(m,bus,&io_status,0x8c6177c4u,4);if(io_status)return io_status;goto L8c61777e; /* mov.l 0x8c6177c4,r3 */
L8c61777e: sa_motion_publish_stop(ctx,0x8c61777eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617780; /* float FPUL,fr3 */
L8c617780: sa_motion_publish_stop(ctx,0x8c617780u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617782; /* fmul fr5,fr3 */
L8c617782: sa_motion_publish_stop(ctx,0x8c617782u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617784; /* fmov.s fr3,@-r8 */
L8c617784: sa_motion_publish_stop(ctx,0x8c617784u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617786; /* fmov.s @(r0,r5),fr3 */
L8c617786: sa_motion_publish_stop(ctx,0x8c617786u); r[0]=20u;goto L8c617788; /* mov #0x14,r0 */
L8c617788: sa_motion_publish_stop(ctx,0x8c617788u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61778a; /* fmov.s fr3,@-r8 */
L8c61778a: sa_motion_publish_stop(ctx,0x8c61778au); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61778c; /* fmov.s @(r0,r5),fr3 */
L8c61778c: sa_motion_publish_stop(ctx,0x8c61778cu); r[0]=16u;goto L8c61778e; /* mov #0x10,r0 */
L8c61778e: sa_motion_publish_stop(ctx,0x8c61778eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617790; /* fmov.s fr3,@-r8 */
L8c617790: sa_motion_publish_stop(ctx,0x8c617790u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617792; /* fmov.s @(r0,r5),fr3 */
L8c617792: sa_motion_publish_stop(ctx,0x8c617792u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617794; /* fmov.s fr3,@-r8 */
L8c617794: sa_motion_publish_stop(ctx,0x8c617794u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c617796; /* mov.l r3,@-r8 */
L8c617796: sa_motion_publish_stop(ctx,0x8c617796u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617798; /* pref @r8 */
L8c617798: sa_motion_publish_stop(ctx,0x8c617798u); r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c61779a; /* mov.l @r7,r2 */
L8c61779a: sa_motion_publish_stop(ctx,0x8c61779au); r[8]+=32u;goto L8c61779c; /* add #0x20,r8 */
L8c61779c: sa_motion_publish_stop(ctx,0x8c61779eu); r[2]+=3u;goto L8c6178b2; /* bra 0x8c6178b2 */
L8c6177c8: sa_motion_publish_stop(ctx,0x8c6177c8u); r[3]=r[6];goto L8c6177ca; /* mov r6,r3 */
L8c6177ca: sa_motion_publish_stop(ctx,0x8c6177cau); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6177cc; /* mov.l r3,@r15 */
L8c6177cc: sa_motion_publish_stop(ctx,0x8c6177ccu); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6177ce; /* mov.w @(0x2,r3),r0 */
L8c6177ce: sa_motion_publish_stop(ctx,0x8c6177ceu); r[8]+=24u;goto L8c6177d0; /* add #0x18,r8 */
L8c6177d0: sa_motion_publish_stop(ctx,0x8c6177d0u); r[14]=r[4];goto L8c6177d2; /* mov r4,r14 */
L8c6177d2: sa_motion_publish_stop(ctx,0x8c6177d2u); r[3]=r[0];goto L8c6177d4; /* mov r0,r3 */
L8c6177d4: sa_motion_publish_stop(ctx,0x8c6177d4u); *fpul=r[3];goto L8c6177d6; /* lds r3,FPUL */
L8c6177d6: sa_motion_publish_stop(ctx,0x8c6177d6u); r[0]=24u;goto L8c6177d8; /* mov #0x18,r0 */
L8c6177d8: sa_motion_publish_stop(ctx,0x8c6177d8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6177da; /* float FPUL,fr3 */
L8c6177da: sa_motion_publish_stop(ctx,0x8c6177dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6177dc; /* fmul fr5,fr3 */
L8c6177dc: sa_motion_publish_stop(ctx,0x8c6177dcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6177de; /* fmov.s fr3,@-r8 */
L8c6177de: sa_motion_publish_stop(ctx,0x8c6177deu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6177e0; /* mov.l @r15,r3 */
L8c6177e0: sa_motion_publish_stop(ctx,0x8c6177e0u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6177e2; /* mov.w @r3,r3 */
L8c6177e2: sa_motion_publish_stop(ctx,0x8c6177e2u); *fpul=r[3];goto L8c6177e4; /* lds r3,FPUL */
L8c6177e4: sa_motion_publish_stop(ctx,0x8c6177e4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6177e6; /* float FPUL,fr3 */
L8c6177e6: sa_motion_publish_stop(ctx,0x8c6177e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6177e8; /* fmul fr5,fr3 */
L8c6177e8: sa_motion_publish_stop(ctx,0x8c6177e8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6177ea; /* fmov.s fr3,@-r8 */
L8c6177ea: sa_motion_publish_stop(ctx,0x8c6177eau); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6177ec; /* fmov.s @(r0,r14),fr3 */
L8c6177ec: sa_motion_publish_stop(ctx,0x8c6177ecu); r[0]=20u;goto L8c6177ee; /* mov #0x14,r0 */
L8c6177ee: sa_motion_publish_stop(ctx,0x8c6177eeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6177f0; /* fmov.s fr3,@-r8 */
L8c6177f0: sa_motion_publish_stop(ctx,0x8c6177f0u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6177f2; /* fmov.s @(r0,r14),fr3 */
L8c6177f2: sa_motion_publish_stop(ctx,0x8c6177f2u); r[0]=16u;goto L8c6177f4; /* mov #0x10,r0 */
L8c6177f4: sa_motion_publish_stop(ctx,0x8c6177f4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6177f6; /* fmov.s fr3,@-r8 */
L8c6177f6: sa_motion_publish_stop(ctx,0x8c6177f6u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6177f8; /* fmov.s @(r0,r14),fr3 */
L8c6177f8: sa_motion_publish_stop(ctx,0x8c6177f8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6177fa; /* fmov.s fr3,@-r8 */
L8c6177fa: sa_motion_publish_stop(ctx,0x8c6177fau); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6177fc; /* mov.l r5,@-r8 */
L8c6177fc: sa_motion_publish_stop(ctx,0x8c6177fcu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6177fe; /* pref @r8 */
L8c6177fe: sa_motion_publish_stop(ctx,0x8c6177feu); r[3]=r[6];goto L8c617800; /* mov r6,r3 */
L8c617800: sa_motion_publish_stop(ctx,0x8c617800u); r[3]+=4u;goto L8c617802; /* add #0x4,r3 */
L8c617802: sa_motion_publish_stop(ctx,0x8c617802u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c617804; /* mov.l r3,@r15 */
L8c617804: sa_motion_publish_stop(ctx,0x8c617804u); r[8]+=56u;goto L8c617806; /* add #0x38,r8 */
L8c617806: sa_motion_publish_stop(ctx,0x8c617806u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c617808; /* mov.w @(0x2,r3),r0 */
L8c617808: sa_motion_publish_stop(ctx,0x8c617808u); r[14]+=32u;goto L8c61780a; /* add #0x20,r14 */
L8c61780a: sa_motion_publish_stop(ctx,0x8c61780au); r[3]=r[0];goto L8c61780c; /* mov r0,r3 */
L8c61780c: sa_motion_publish_stop(ctx,0x8c61780cu); *fpul=r[3];goto L8c61780e; /* lds r3,FPUL */
L8c61780e: sa_motion_publish_stop(ctx,0x8c61780eu); r[0]=24u;goto L8c617810; /* mov #0x18,r0 */
L8c617810: sa_motion_publish_stop(ctx,0x8c617810u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617812; /* float FPUL,fr3 */
L8c617812: sa_motion_publish_stop(ctx,0x8c617812u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617814; /* fmul fr5,fr3 */
L8c617814: sa_motion_publish_stop(ctx,0x8c617814u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617816; /* fmov.s fr3,@-r8 */
L8c617816: sa_motion_publish_stop(ctx,0x8c617816u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617818; /* mov.l @r15,r3 */
L8c617818: sa_motion_publish_stop(ctx,0x8c617818u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c61781a; /* mov.w @r3,r3 */
L8c61781a: sa_motion_publish_stop(ctx,0x8c61781au); *fpul=r[3];goto L8c61781c; /* lds r3,FPUL */
L8c61781c: sa_motion_publish_stop(ctx,0x8c61781cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61781e; /* float FPUL,fr3 */
L8c61781e: sa_motion_publish_stop(ctx,0x8c61781eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617820; /* fmul fr5,fr3 */
L8c617820: sa_motion_publish_stop(ctx,0x8c617820u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617822; /* fmov.s fr3,@-r8 */
L8c617822: sa_motion_publish_stop(ctx,0x8c617822u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617824; /* fmov.s @(r0,r14),fr3 */
L8c617824: sa_motion_publish_stop(ctx,0x8c617824u); r[0]=20u;goto L8c617826; /* mov #0x14,r0 */
L8c617826: sa_motion_publish_stop(ctx,0x8c617826u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617828; /* fmov.s fr3,@-r8 */
L8c617828: sa_motion_publish_stop(ctx,0x8c617828u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61782a; /* fmov.s @(r0,r14),fr3 */
L8c61782a: sa_motion_publish_stop(ctx,0x8c61782au); r[0]=16u;goto L8c61782c; /* mov #0x10,r0 */
L8c61782c: sa_motion_publish_stop(ctx,0x8c61782cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61782e; /* fmov.s fr3,@-r8 */
L8c61782e: sa_motion_publish_stop(ctx,0x8c61782eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617830; /* fmov.s @(r0,r14),fr3 */
L8c617830: sa_motion_publish_stop(ctx,0x8c617830u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617832; /* fmov.s fr3,@-r8 */
L8c617832: sa_motion_publish_stop(ctx,0x8c617832u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c617834; /* mov.l r5,@-r8 */
L8c617834: sa_motion_publish_stop(ctx,0x8c617834u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617836; /* pref @r8 */
L8c617836: sa_motion_publish_stop(ctx,0x8c617836u); r[3]=r[6];goto L8c617838; /* mov r6,r3 */
L8c617838: sa_motion_publish_stop(ctx,0x8c617838u); r[3]+=12u;goto L8c61783a; /* add #0xc,r3 */
L8c61783a: sa_motion_publish_stop(ctx,0x8c61783au); r[8]+=60u;goto L8c61783c; /* add #0x3c,r8 */
L8c61783c: sa_motion_publish_stop(ctx,0x8c61783cu); r[14]=r[4];goto L8c61783e; /* mov r4,r14 */
L8c61783e: sa_motion_publish_stop(ctx,0x8c61783eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c617840; /* fmov.s fr4,@-r8 */
L8c617840: sa_motion_publish_stop(ctx,0x8c617840u); r[14]+=96u;goto L8c617842; /* add #0x60,r14 */
L8c617842: sa_motion_publish_stop(ctx,0x8c617842u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c617844; /* mov.l r3,@r15 */
L8c617844: sa_motion_publish_stop(ctx,0x8c617844u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c617846; /* mov.w @(0x2,r3),r0 */
L8c617846: sa_motion_publish_stop(ctx,0x8c617846u); r[3]=r[0];goto L8c617848; /* mov r0,r3 */
L8c617848: sa_motion_publish_stop(ctx,0x8c617848u); *fpul=r[3];goto L8c61784a; /* lds r3,FPUL */
L8c61784a: sa_motion_publish_stop(ctx,0x8c61784au); r[0]=24u;goto L8c61784c; /* mov #0x18,r0 */
L8c61784c: sa_motion_publish_stop(ctx,0x8c61784cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61784e; /* float FPUL,fr3 */
L8c61784e: sa_motion_publish_stop(ctx,0x8c61784eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617850; /* fmul fr5,fr3 */
L8c617850: sa_motion_publish_stop(ctx,0x8c617850u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617852; /* fmov.s fr3,@-r8 */
L8c617852: sa_motion_publish_stop(ctx,0x8c617852u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617854; /* mov.l @r15,r3 */
L8c617854: sa_motion_publish_stop(ctx,0x8c617854u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c617856; /* mov.w @r3,r3 */
L8c617856: sa_motion_publish_stop(ctx,0x8c617856u); *fpul=r[3];goto L8c617858; /* lds r3,FPUL */
L8c617858: sa_motion_publish_stop(ctx,0x8c617858u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61785a; /* float FPUL,fr3 */
L8c61785a: sa_motion_publish_stop(ctx,0x8c61785au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61785c; /* fmul fr5,fr3 */
L8c61785c: sa_motion_publish_stop(ctx,0x8c61785cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61785e; /* fmov.s fr3,@-r8 */
L8c61785e: sa_motion_publish_stop(ctx,0x8c61785eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617860; /* fmov.s @(r0,r14),fr3 */
L8c617860: sa_motion_publish_stop(ctx,0x8c617860u); r[0]=20u;goto L8c617862; /* mov #0x14,r0 */
L8c617862: sa_motion_publish_stop(ctx,0x8c617862u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617864; /* fmov.s fr3,@-r8 */
L8c617864: sa_motion_publish_stop(ctx,0x8c617864u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617866; /* fmov.s @(r0,r14),fr3 */
L8c617866: sa_motion_publish_stop(ctx,0x8c617866u); r[0]=16u;goto L8c617868; /* mov #0x10,r0 */
L8c617868: sa_motion_publish_stop(ctx,0x8c617868u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61786a; /* fmov.s fr3,@-r8 */
L8c61786a: sa_motion_publish_stop(ctx,0x8c61786au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61786c; /* fmov.s @(r0,r14),fr3 */
L8c61786c: sa_motion_publish_stop(ctx,0x8c61786cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61786e; /* fmov.s fr3,@-r8 */
L8c61786e: sa_motion_publish_stop(ctx,0x8c61786eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c617870; /* mov.l r5,@-r8 */
L8c617870: sa_motion_publish_stop(ctx,0x8c617870u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617872; /* pref @r8 */
L8c617872: sa_motion_publish_stop(ctx,0x8c617872u); r[5]=r[6];goto L8c617874; /* mov r6,r5 */
L8c617874: sa_motion_publish_stop(ctx,0x8c617874u); r[5]+=8u;goto L8c617876; /* add #0x8,r5 */
L8c617876: sa_motion_publish_stop(ctx,0x8c617876u); r[8]+=60u;goto L8c617878; /* add #0x3c,r8 */
L8c617878: sa_motion_publish_stop(ctx,0x8c617878u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61787a; /* fmov.s fr4,@-r8 */
L8c61787a: sa_motion_publish_stop(ctx,0x8c61787au); r[0]=sx(m,bus,&io_status,r[5]+0x2u,2);if(io_status)return io_status;goto L8c61787c; /* mov.w @(0x2,r5),r0 */
L8c61787c: sa_motion_publish_stop(ctx,0x8c61787cu); r[3]=r[0];goto L8c61787e; /* mov r0,r3 */
L8c61787e: sa_motion_publish_stop(ctx,0x8c61787eu); *fpul=r[3];goto L8c617880; /* lds r3,FPUL */
L8c617880: sa_motion_publish_stop(ctx,0x8c617880u); r[0]=24u;goto L8c617882; /* mov #0x18,r0 */
L8c617882: sa_motion_publish_stop(ctx,0x8c617882u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617884; /* float FPUL,fr3 */
L8c617884: sa_motion_publish_stop(ctx,0x8c617884u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617886; /* fmul fr5,fr3 */
L8c617886: sa_motion_publish_stop(ctx,0x8c617886u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617888; /* fmov.s fr3,@-r8 */
L8c617888: sa_motion_publish_stop(ctx,0x8c617888u); r[3]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;goto L8c61788a; /* mov.w @r5,r3 */
L8c61788a: sa_motion_publish_stop(ctx,0x8c61788au); r[5]=r[4];goto L8c61788c; /* mov r4,r5 */
L8c61788c: sa_motion_publish_stop(ctx,0x8c61788cu); r[5]+=64u;goto L8c61788e; /* add #0x40,r5 */
L8c61788e: sa_motion_publish_stop(ctx,0x8c61788eu); *fpul=r[3];goto L8c617890; /* lds r3,FPUL */
L8c617890: sa_motion_publish_stop(ctx,0x8c617890u); r[3]=rd(m,bus,&io_status,0x8c617954u,4);if(io_status)return io_status;goto L8c617892; /* mov.l 0x8c617954,r3 */
L8c617892: sa_motion_publish_stop(ctx,0x8c617892u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617894; /* float FPUL,fr3 */
L8c617894: sa_motion_publish_stop(ctx,0x8c617894u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617896; /* fmul fr5,fr3 */
L8c617896: sa_motion_publish_stop(ctx,0x8c617896u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617898; /* fmov.s fr3,@-r8 */
L8c617898: sa_motion_publish_stop(ctx,0x8c617898u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61789a; /* fmov.s @(r0,r5),fr3 */
L8c61789a: sa_motion_publish_stop(ctx,0x8c61789au); r[0]=20u;goto L8c61789c; /* mov #0x14,r0 */
L8c61789c: sa_motion_publish_stop(ctx,0x8c61789cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61789e; /* fmov.s fr3,@-r8 */
L8c61789e: sa_motion_publish_stop(ctx,0x8c61789eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6178a0; /* fmov.s @(r0,r5),fr3 */
L8c6178a0: sa_motion_publish_stop(ctx,0x8c6178a0u); r[0]=16u;goto L8c6178a2; /* mov #0x10,r0 */
L8c6178a2: sa_motion_publish_stop(ctx,0x8c6178a2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6178a4; /* fmov.s fr3,@-r8 */
L8c6178a4: sa_motion_publish_stop(ctx,0x8c6178a4u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6178a6; /* fmov.s @(r0,r5),fr3 */
L8c6178a6: sa_motion_publish_stop(ctx,0x8c6178a6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6178a8; /* fmov.s fr3,@-r8 */
L8c6178a8: sa_motion_publish_stop(ctx,0x8c6178a8u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6178aa; /* mov.l r3,@-r8 */
L8c6178aa: sa_motion_publish_stop(ctx,0x8c6178aau); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6178ac; /* pref @r8 */
L8c6178ac: sa_motion_publish_stop(ctx,0x8c6178acu); r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c6178ae; /* mov.l @r7,r2 */
L8c6178ae: sa_motion_publish_stop(ctx,0x8c6178aeu); r[8]+=32u;goto L8c6178b0; /* add #0x20,r8 */
L8c6178b0: sa_motion_publish_stop(ctx,0x8c6178b0u); r[2]+=4u;goto L8c6178b2; /* add #0x4,r2 */
L8c6178b2: sa_motion_publish_stop(ctx,0x8c6178b2u); wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c6178b4; /* mov.l r2,@r7 */
L8c6178b4: sa_motion_publish_stop(ctx,0x8c6178b4u); r[15]+=80u;goto L8c6178b6; /* add #0x50,r15 */
L8c6178b6: sa_motion_publish_stop(ctx,0x8c6178b6u); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6178b8; /* fmov.s @r15+,fr12 */
L8c6178b8: sa_motion_publish_stop(ctx,0x8c6178b8u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6178ba; /* mov.l @r15+,r9 */
L8c6178ba: sa_motion_publish_stop(ctx,0x8c6178bau); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6178bc; /* mov.l @r15+,r10 */
L8c6178bc: sa_motion_publish_stop(ctx,0x8c6178bcu); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6178be; /* mov.l @r15+,r11 */
L8c6178be: sa_motion_publish_stop(ctx,0x8c6178beu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6178c0; /* mov.l @r15+,r12 */
L8c6178c0: sa_motion_publish_stop(ctx,0x8c6178c0u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6178c2; /* mov.l @r15+,r13 */
L8c6178c2: sa_motion_publish_stop(ctx,0x8c6178c4u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
