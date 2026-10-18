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
L8c6173ba: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6173bc; /* mov.l r14,@-r15 */
L8c6173bc: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6173be; /* mov.l r13,@-r15 */
L8c6173be: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6173c0; /* mov.l r12,@-r15 */
L8c6173c0: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6173c2; /* mov.l r11,@-r15 */
L8c6173c2: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6173c4; /* mov.l r10,@-r15 */
L8c6173c4: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6173c6; /* mov.l r9,@-r15 */
L8c6173c6: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c6173c8; /* fmov.s fr12,@-r15 */
L8c6173c8: r[15]+=4294967216u;goto L8c6173ca; /* add #-0x50,r15 */
L8c6173ca: r[3]=rd(m,bus,&io_status,0x8c6174c0u,4);if(io_status)return io_status;goto L8c6173cc; /* mov.l 0x8c6174c0,r3 */
L8c6173cc: r[7]=r[15];goto L8c6173ce; /* mov r15,r7 */
L8c6173ce: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][8]=0x3f800000u;goto L8c6173d0; /* fldi1 fr8 */
L8c6173d0: r[7]+=68u;goto L8c6173d2; /* add #0x44,r7 */
L8c6173d2: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6173d4; /* fmov.s @r3,fr3 */
L8c6173d4: r[9]=r[15];goto L8c6173d6; /* mov r15,r9 */
L8c6173d6: fcopy(f,6u,8u);goto L8c6173d8; /* fmov fr8,fr6 */
L8c6173d8: r[2]=r[7];goto L8c6173da; /* mov r7,r2 */
L8c6173da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[3],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6173dc; /* fdiv fr3,fr6 */
L8c6173dc: r[0]=0x8c6174c8u;goto L8c6173de; /* mova 0x8c6174c8,r0 */
L8c6173de: r[9]+=32u;goto L8c6173e0; /* add #0x20,r9 */
L8c6173e0: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6173e2; /* mov.l r2,@(0x4,r15) */
L8c6173e2: wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c6173e4; /* mov.l r4,@r2 */
L8c6173e4: r[10]=r[9];goto L8c6173e6; /* mov r9,r10 */
L8c6173e6: r[12]=r[15];goto L8c6173e8; /* mov r15,r12 */
L8c6173e8: r[13]=rd(m,bus,&io_status,0x8c6174c4u,4);if(io_status)return io_status;goto L8c6173ea; /* mov.l 0x8c6174c4,r13 */
L8c6173ea: r[4]=rd(m,bus,&io_status,0x8c6174ccu,4);if(io_status)return io_status;goto L8c6173ec; /* mov.l 0x8c6174cc,r4 */
L8c6173ec: r[2]=0u;goto L8c6173ee; /* mov #0x0,r2 */
L8c6173ee: wr(m,bus,&io_status,r[7]+0x4u,r[5],4);if(io_status)return io_status;goto L8c6173f0; /* mov.l r5,@(0x4,r7) */
L8c6173f0: r[5]=1u;goto L8c6173f2; /* mov #0x1,r5 */
L8c6173f2: wr(m,bus,&io_status,r[7]+0x8u,r[6],4);if(io_status)return io_status;goto L8c6173f4; /* mov.l r6,@(0x8,r7) */
L8c6173f4: r[12]+=20u;goto L8c6173f6; /* add #0x14,r12 */
L8c6173f6: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6173f8; /* mov.l r2,@r15 */
L8c6173f8: r[11]=r[10];goto L8c6173fa; /* mov r10,r11 */
L8c6173fa: r[14]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6173fc; /* mov.l @(0x4,r15),r14 */
L8c6173fc: wr(m,bus,&io_status,r[15]+0x4u,r[10],4);if(io_status)return io_status;goto L8c6173fe; /* mov.l r10,@(0x4,r15) */
L8c6173fe: fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c617400; /* fmov.s @r0,fr5 */
L8c617400: r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617402; /* mov.l @r15,r6 */
L8c617402: r[0]=4u;goto L8c617404; /* mov #0x4,r0 */
L8c617404: r[6]<<=2;goto L8c617406; /* shll2 r6 */
L8c617406: wr(m,bus,&io_status,r[15]+0xcu,r[6],4);if(io_status)return io_status;goto L8c617408; /* mov.l r6,@(0xc,r15) */
L8c617408: r[6]+=r[7];goto L8c61740a; /* add r7,r6 */
L8c61740a: wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c61740c; /* mov.l r6,@(0x8,r15) */
L8c61740c: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61740e; /* mov.l @(0xc,r15),r3 */
L8c61740e: r[6]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c617410; /* mov.l @r6,r6 */
L8c617410: r[3]+=r[12];goto L8c617412; /* add r12,r3 */
L8c617412: wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c617414; /* mov.l r3,@(0x10,r15) */
L8c617414: r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c617416; /* mov.l r3,@-r15 */
L8c617416: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c617418; /* mov.l @r4,r2 */
L8c617418: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61741a; /* fmov.s @(r0,r6),fr0 */
L8c61741a: r[0]=16u;goto L8c61741c; /* mov #0x10,r0 */
L8c61741c: r[3]=r[2];goto L8c61741e; /* mov r2,r3 */
L8c61741e: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c617420; /* fmov.s @(r0,r3),fr3 */
L8c617420: r[1]=r[2];goto L8c617422; /* mov r2,r1 */
L8c617422: fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c617424; /* fmov.s @r1,fr2 */
L8c617424: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617426; /* fmul fr0,fr3 */
L8c617426: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c617428; /* fmov.s @r6,fr0 */
L8c617428: r[0]=8u;goto L8c61742a; /* mov #0x8,r0 */
L8c61742a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61742c; /* fmac fr0,fr2,fr3 */
L8c61742c: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61742e; /* fmov.s @(r0,r6),fr0 */
L8c61742e: r[0]=32u;goto L8c617430; /* mov #0x20,r0 */
L8c617430: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c617432; /* fmov.s @(r0,r2),fr2 */
L8c617432: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c617434; /* mov.l @r15+,r2 */
L8c617434: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617436; /* fmac fr0,fr2,fr3 */
L8c617436: fcopy(f,0u,5u);goto L8c617438; /* fmov fr5,fr0 */
L8c617438: fcopy(f,2u,5u);goto L8c61743a; /* fmov fr5,fr2 */
L8c61743a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61743c; /* fmac fr0,fr3,fr2 */
L8c61743c: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c61743e; /* ftrc fr2,FPUL */
L8c61743e: r[3]=*fpul;goto L8c617440; /* sts FPUL,r3 */
L8c617440: wr(m,bus,&io_status,r[2],r[3],2);if(io_status)return io_status;goto L8c617442; /* mov.w r3,@r2 */
L8c617442: r[0]=36u;goto L8c617444; /* mov #0x24,r0 */
L8c617444: r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c617446; /* mov.l @r4,r3 */
L8c617446: r[1]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c617448; /* mov.l @(0x10,r15),r1 */
L8c617448: fload(m,bus,&io_status,f,1u,r[0]+r[3]);if(io_status)return io_status;goto L8c61744a; /* fmov.s @(r0,r3),fr1 */
L8c61744a: r[0]=4u;goto L8c61744c; /* mov #0x4,r0 */
L8c61744c: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c61744e; /* fmov.s @(r0,r6),fr0 */
L8c61744e: r[0]=20u;goto L8c617450; /* mov #0x14,r0 */
L8c617450: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c617452; /* fmov.s @(r0,r3),fr3 */
L8c617452: r[0]=4u;goto L8c617454; /* mov #0x4,r0 */
L8c617454: r[2]=r[3];goto L8c617456; /* mov r3,r2 */
L8c617456: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c617458; /* fmov.s @(r0,r2),fr2 */
L8c617458: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61745a; /* fmul fr0,fr3 */
L8c61745a: fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c61745c; /* fmov.s @r6,fr0 */
L8c61745c: r[0]=8u;goto L8c61745e; /* mov #0x8,r0 */
L8c61745e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617460; /* fmac fr0,fr2,fr3 */
L8c617460: fload(m,bus,&io_status,f,0u,r[0]+r[6]);if(io_status)return io_status;goto L8c617462; /* fmov.s @(r0,r6),fr0 */
L8c617462: fcopy(f,2u,5u);goto L8c617464; /* fmov fr5,fr2 */
L8c617464: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617466; /* fmac fr0,fr1,fr3 */
L8c617466: fcopy(f,0u,5u);goto L8c617468; /* fmov fr5,fr0 */
L8c617468: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61746a; /* fmac fr0,fr3,fr2 */
L8c61746a: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c61746c; /* ftrc fr2,FPUL */
L8c61746c: r[0]=*fpul;goto L8c61746e; /* sts FPUL,r0 */
L8c61746e: wr(m,bus,&io_status,r[1]+0x2u,r[0],2);if(io_status)return io_status;goto L8c617470; /* mov.w r0,@(0x2,r1) */
L8c617470: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c617472; /* mov.l @(0x8,r15),r3 */
L8c617472: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c617474; /* mov.l @r3,r2 */
L8c617474: r[1]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c617476; /* mov.l @(0xc,r2),r1 */
L8c617476: *sr=(*sr&~1u)|!(r[5]&r[1]);goto L8c617478; /* tst r5,r1 */
L8c617478: if(!(*sr&1u))goto L8c6174d8;goto L8c61747a; /* bf 0x8c6174d8 */
L8c61747a: r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61747c; /* mov.l @r15,r6 */
L8c61747c: r[0]=24u;goto L8c61747e; /* mov #0x18,r0 */
L8c61747e: fcopy(f,7u,8u);goto L8c617480; /* fmov fr8,fr7 */
L8c617480: r[6]<<=2;goto L8c617482; /* shll2 r6 */
L8c617482: r[6]+=r[7];goto L8c617484; /* add r7,r6 */
L8c617484: r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c617486; /* mov.l @r6,r3 */
L8c617486: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c617488; /* mov.l r3,@(0x8,r15) */
L8c617488: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61748a; /* fmov.s @(r0,r3),fr3 */
L8c61748a: r[0]=12u;goto L8c61748c; /* mov #0xc,r0 */
L8c61748c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[7],&fr[3],&f->fpscr,&fr[7]);if(status)return status;}goto L8c61748e; /* fdiv fr3,fr7 */
L8c61748e: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c617490; /* fmov.s @(r0,r13),fr3 */
L8c617490: r[0]=16u;goto L8c617492; /* mov #0x10,r0 */
L8c617492: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c617494; /* fmov.s @(r0,r3),fr2 */
L8c617494: r[3]=rd(m,bus,&io_status,0x8c6174d0u,4);if(io_status)return io_status;goto L8c617496; /* mov.l 0x8c6174d0,r3 */
L8c617496: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c617498; /* fsub fr3,fr2 */
L8c617498: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61749a; /* fmov.s @r3,fr3 */
L8c61749a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61749c; /* fmul fr7,fr2 */
L8c61749c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61749e; /* fdiv fr3,fr2 */
L8c61749e: fstore(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c6174a0; /* fmov.s fr2,@r11 */
L8c6174a0: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6174a2; /* fmov.s @(r0,r13),fr3 */
L8c6174a2: r[0]=20u;goto L8c6174a4; /* mov #0x14,r0 */
L8c6174a4: r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6174a6; /* mov.l @r6,r2 */
L8c6174a6: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6174a8; /* fmov.s @(r0,r2),fr2 */
L8c6174a8: r[0]=4u;goto L8c6174aa; /* mov #0x4,r0 */
L8c6174aa: r[2]=rd(m,bus,&io_status,0x8c6174d4u,4);if(io_status)return io_status;goto L8c6174ac; /* mov.l 0x8c6174d4,r2 */
L8c6174ac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6174ae; /* fsub fr3,fr2 */
L8c6174ae: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6174b0; /* fmov.s @r2,fr3 */
L8c6174b0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6174b2; /* fmul fr7,fr2 */
L8c6174b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6174b4; /* fdiv fr3,fr2 */
L8c6174b4: fstore(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c6174b6; /* fmov fr2,@(r0,r11) */
L8c6174b6: r[0]=8u;goto L8c6174b8; /* mov #0x8,r0 */
L8c6174b8: fstore(m,bus,&io_status,f,7u,r[0]+r[11]);if(io_status)return io_status;goto L8c6174f4; /* bra 0x8c6174f4 */
L8c6174d8: r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c6174da; /* mov.l @r14,r3 */
L8c6174da: r[0]=16u;goto L8c6174dc; /* mov #0x10,r0 */
L8c6174dc: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6174de; /* fmov.s @(r0,r3),fr3 */
L8c6174de: r[0]=20u;goto L8c6174e0; /* mov #0x14,r0 */
L8c6174e0: fstore(m,bus,&io_status,f,3u,r[10]);if(io_status)return io_status;goto L8c6174e2; /* fmov.s fr3,@r10 */
L8c6174e2: r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c6174e4; /* mov.l @r14,r3 */
L8c6174e4: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6174e6; /* fmov.s @(r0,r3),fr3 */
L8c6174e6: r[0]=4u;goto L8c6174e8; /* mov #0x4,r0 */
L8c6174e8: fstore(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c6174ea; /* fmov fr3,@(r0,r10) */
L8c6174ea: r[0]=24u;goto L8c6174ec; /* mov #0x18,r0 */
L8c6174ec: r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c6174ee; /* mov.l @r14,r3 */
L8c6174ee: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6174f0; /* fmov.s @(r0,r3),fr3 */
L8c6174f0: r[0]=8u;goto L8c6174f2; /* mov #0x8,r0 */
L8c6174f2: fstore(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c6174f4; /* fmov fr3,@(r0,r10) */
L8c6174f4: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6174f6; /* mov.l @r15,r3 */
L8c6174f6: r[2]=3u;goto L8c6174f8; /* mov #0x3,r2 */
L8c6174f8: r[10]+=12u;goto L8c6174fa; /* add #0xc,r10 */
L8c6174fa: r[3]+=1u;goto L8c6174fc; /* add #0x1,r3 */
L8c6174fc: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c6174fe; /* cmp/ge r2,r3 */
L8c6174fe: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c617500; /* mov.l r3,@r15 */
L8c617500: r[14]+=4u;goto L8c617502; /* add #0x4,r14 */
L8c617502: take=(*sr&1u);r[11]+=12u;if(take)goto L8c61750a;goto L8c617506; /* bt/s 0x8c61750a */
L8c617506: goto L8c617400; /* bra 0x8c617400 */
L8c61750a: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61750c; /* mov.l @(0x4,r15),r3 */
L8c61750c: r[0]=8u;goto L8c61750e; /* mov #0x8,r0 */
L8c61750e: r[4]=r[9];goto L8c617510; /* mov r9,r4 */
L8c617510: r[4]+=12u;goto L8c617512; /* add #0xc,r4 */
L8c617512: fload(m,bus,&io_status,f,7u,r[0]+r[3]);if(io_status)return io_status;goto L8c617514; /* fmov.s @(r0,r3),fr7 */
L8c617514: r[0]=4u;goto L8c617516; /* mov #0x4,r0 */
L8c617516: fload(m,bus,&io_status,f,11u,r[0]+r[4]);if(io_status)return io_status;goto L8c617518; /* fmov.s @(r0,r4),fr11 */
L8c617518: r[6]=r[9];goto L8c61751a; /* mov r9,r6 */
L8c61751a: fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c61751c; /* fmov.s @(r0,r3),fr9 */
L8c61751c: r[0]=8u;goto L8c61751e; /* mov #0x8,r0 */
L8c61751e: fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c617520; /* fmov.s @(r0,r4),fr5 */
L8c617520: r[0]=4u;goto L8c617522; /* mov #0x4,r0 */
L8c617522: fcopy(f,3u,9u);goto L8c617524; /* fmov fr9,fr3 */
L8c617524: r[6]+=24u;goto L8c617526; /* add #0x18,r6 */
L8c617526: fcopy(f,0u,7u);goto L8c617528; /* fmov fr7,fr0 */
L8c617528: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61752a; /* fmul fr5,fr3 */
L8c61752a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[11],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61752c; /* fmul fr11,fr0 */
L8c61752c: fload(m,bus,&io_status,f,10u,r[3]);if(io_status)return io_status;goto L8c61752e; /* fmov.s @r3,fr10 */
L8c61752e: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;goto L8c617530; /* fmov.s @r4,fr8 */
L8c617530: fcopy(f,2u,7u);goto L8c617532; /* fmov fr7,fr2 */
L8c617532: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617534; /* fmul fr8,fr2 */
L8c617534: fcopy(f,1u,8u);goto L8c617536; /* fmov fr8,fr1 */
L8c617536: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c617538; /* fsub fr3,fr0 */
L8c617538: fcopy(f,3u,10u);goto L8c61753a; /* fmov fr10,fr3 */
L8c61753a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61753c; /* fmul fr5,fr3 */
L8c61753c: fcopy(f,12u,10u);goto L8c61753e; /* fmov fr10,fr12 */
L8c61753e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[9],&f->fpscr,&fr[1]);if(status)return status;}goto L8c617540; /* fmul fr9,fr1 */
L8c617540: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617542; /* fsub fr2,fr3 */
L8c617542: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c617544; /* fmov.s @(r0,r6),fr2 */
L8c617544: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617546; /* fmul fr3,fr2 */
L8c617546: fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c617548; /* fmov.s @r6,fr3 */
L8c617548: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61754a; /* fmac fr0,fr3,fr2 */
L8c61754a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[11],&f->fpscr,&fr[12]);if(status)return status;}goto L8c61754c; /* fmul fr11,fr12 */
L8c61754c: r[0]=8u;goto L8c61754e; /* mov #0x8,r0 */
L8c61754e: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c617550; /* fmov.s @(r0,r6),fr3 */
L8c617550: r[4]=rd(m,bus,&io_status,0x8c6177a0u,4);if(io_status)return io_status;goto L8c617552; /* mov.l 0x8c6177a0,r4 */
L8c617552: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[12],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c617554; /* fsub fr12,fr1 */
L8c617554: r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c617556; /* mov.l @r4,r3 */
L8c617556: fcopy(f,0u,1u);goto L8c617558; /* fmov fr1,fr0 */
L8c617558: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61755a; /* fmac fr0,fr3,fr2 */
L8c61755a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61755c; /* fldi0 fr3 */
L8c61755c: fcopy(f,5u,2u);goto L8c61755e; /* fmov fr2,fr5 */
L8c61755e: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c617560; /* fcmp/gt fr5,fr3 */
L8c617560: r[0]=*sr&1u;goto L8c617562; /* movt r0 */
L8c617562: *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c617564; /* tst r3,r0 */
L8c617564: if((*sr&1u))goto L8c61756a;goto L8c617566; /* bt 0x8c61756a */
L8c617566: goto L8c6178b4; /* bra 0x8c6178b4 */
L8c61756a: r[1]=0u;goto L8c61756c; /* mov #0x0,r1 */
L8c61756c: r[6]=rd(m,bus,&io_status,0x8c6177a4u,4);if(io_status)return io_status;goto L8c61756e; /* mov.l 0x8c6177a4,r6 */
L8c61756e: wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c617570; /* mov.l r1,@r4 */
L8c617570: r[3]=2u;goto L8c617572; /* mov #0x2,r3 */
L8c617572: r[4]=rd(m,bus,&io_status,0x8c6177a8u,4);if(io_status)return io_status;goto L8c617574; /* mov.l 0x8c6177a8,r4 */
L8c617574: r[14]=r[1];goto L8c617576; /* mov r1,r14 */
L8c617576: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c617578; /* mov.l r3,@(0x4,r15) */
L8c617578: r[10]=r[14];goto L8c61757a; /* mov r14,r10 */
L8c61757a: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61757c; /* mov.l @(0x4,r15),r3 */
L8c61757c: r[0]=r[7];goto L8c61757e; /* mov r7,r0 */
L8c61757e: r[11]=r[10];goto L8c617580; /* mov r10,r11 */
L8c617580: r[11]<<=2;goto L8c617582; /* shll2 r11 */
L8c617582: r[3]<<=2;goto L8c617584; /* shll2 r3 */
L8c617584: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c617586; /* mov.l r3,@r15 */
L8c617586: r[2]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c617588; /* mov.l @(r0,r3),r2 */
L8c617588: r[3]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c61758a; /* mov.l @(0xc,r2),r3 */
L8c61758a: r[2]=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;goto L8c61758c; /* mov.l @(r0,r11),r2 */
L8c61758c: r[3]&=r[5];goto L8c61758e; /* and r5,r3 */
L8c61758e: if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[2]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c617590; /* mov.l @(0xc,r2),r2 */
L8c617590: r[2]&=r[5];goto L8c617592; /* and r5,r2 */
L8c617592: r[3]^=r[2];goto L8c617594; /* xor r2,r3 */
L8c617594: r[0]=r[3];goto L8c617596; /* mov r3,r0 */
L8c617596: *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c617598; /* cmp/eq #0x1,r0 */
L8c617598: if(!(*sr&1u))goto L8c61767c;goto L8c61759a; /* bf 0x8c61767c */
L8c61759a: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61759c; /* mov.l @(0x4,r15),r2 */
L8c61759c: r[0]=8u;goto L8c61759e; /* mov #0x8,r0 */
L8c61759e: r[1]=r[10];goto L8c6175a0; /* mov r10,r1 */
L8c6175a0: r[3]=r[2];goto L8c6175a2; /* mov r2,r3 */
L8c6175a2: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6175a4; /* shll r2 */
L8c6175a4: r[2]+=r[3];goto L8c6175a6; /* add r3,r2 */
L8c6175a6: r[3]=rd(m,bus,&io_status,0x8c6177acu,4);if(io_status)return io_status;goto L8c6175a8; /* mov.l 0x8c6177ac,r3 */
L8c6175a8: r[2]<<=2;goto L8c6175aa; /* shll2 r2 */
L8c6175aa: r[2]+=r[9];goto L8c6175ac; /* add r9,r2 */
L8c6175ac: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6175ae; /* mov.l r2,@(0x8,r15) */
L8c6175ae: fload(m,bus,&io_status,f,5u,r[0]+r[2]);if(io_status)return io_status;goto L8c6175b0; /* fmov.s @(r0,r2),fr5 */
L8c6175b0: r[2]=r[10];goto L8c6175b2; /* mov r10,r2 */
L8c6175b2: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6175b4; /* shll r2 */
L8c6175b4: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6175b6; /* fmov.s @r3,fr3 */
L8c6175b6: r[2]+=r[1];goto L8c6175b8; /* add r1,r2 */
L8c6175b8: r[2]<<=2;goto L8c6175ba; /* shll2 r2 */
L8c6175ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6175bc; /* fsub fr5,fr3 */
L8c6175bc: r[0]=8u;goto L8c6175be; /* mov #0x8,r0 */
L8c6175be: r[2]+=r[9];goto L8c6175c0; /* add r9,r2 */
L8c6175c0: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6175c2; /* mov.l r2,@(0xc,r15) */
L8c6175c2: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6175c4; /* fmov.s @(r0,r2),fr2 */
L8c6175c4: r[1]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6175c6; /* mov.l @(0xc,r15),r1 */
L8c6175c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6175c8; /* fsub fr5,fr2 */
L8c6175c8: fcopy(f,5u,3u);goto L8c6175ca; /* fmov fr3,fr5 */
L8c6175ca: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6175cc; /* mov.l @(0x8,r15),r2 */
L8c6175cc: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6175ce; /* fmov.s @r1,fr3 */
L8c6175ce: fload(m,bus,&io_status,f,7u,r[2]);if(io_status)return io_status;goto L8c6175d0; /* fmov.s @r2,fr7 */
L8c6175d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[2],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6175d2; /* fdiv fr2,fr5 */
L8c6175d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6175d4; /* fsub fr7,fr3 */
L8c6175d4: fcopy(f,2u,7u);goto L8c6175d6; /* fmov fr7,fr2 */
L8c6175d6: fcopy(f,0u,5u);goto L8c6175d8; /* fmov fr5,fr0 */
L8c6175d8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6175da; /* fmac fr0,fr3,fr2 */
L8c6175da: r[1]=rd(m,bus,&io_status,0x8c6177b0u,4);if(io_status)return io_status;goto L8c6175dc; /* mov.l 0x8c6177b0,r1 */
L8c6175dc: r[0]=12u;goto L8c6175de; /* mov #0xc,r0 */
L8c6175de: fcopy(f,7u,2u);goto L8c6175e0; /* fmov fr2,fr7 */
L8c6175e0: r[2]=r[14];goto L8c6175e2; /* mov r14,r2 */
L8c6175e2: fcopy(f,0u,6u);goto L8c6175e4; /* fmov fr6,fr0 */
L8c6175e4: r[2]<<=2;goto L8c6175e6; /* shll2 r2 */
L8c6175e6: r[2]<<=2;goto L8c6175e8; /* shll2 r2 */
L8c6175e8: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6175ea; /* shll r2 */
L8c6175ea: r[2]+=r[4];goto L8c6175ec; /* add r4,r2 */
L8c6175ec: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6175ee; /* mov.l r2,@(0x4,r15) */
L8c6175ee: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6175f0; /* fmov.s @r1,fr3 */
L8c6175f0: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c6175f2; /* fmov.s @(r0,r13),fr2 */
L8c6175f2: r[0]=16u;goto L8c6175f4; /* mov #0x10,r0 */
L8c6175f4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6175f6; /* fmul fr7,fr3 */
L8c6175f6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6175f8; /* fmac fr0,fr3,fr2 */
L8c6175f8: fcopy(f,0u,5u);goto L8c6175fa; /* fmov fr5,fr0 */
L8c6175fa: fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6175fc; /* fmov fr2,@(r0,r2) */
L8c6175fc: r[0]=4u;goto L8c6175fe; /* mov #0x4,r0 */
L8c6175fe: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c617600; /* mov.l @(0x8,r15),r2 */
L8c617600: fload(m,bus,&io_status,f,7u,r[0]+r[2]);if(io_status)return io_status;goto L8c617602; /* fmov.s @(r0,r2),fr7 */
L8c617602: r[0]=4u;goto L8c617604; /* mov #0x4,r0 */
L8c617604: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c617606; /* mov.l @(0xc,r15),r2 */
L8c617606: fcopy(f,2u,7u);goto L8c617608; /* fmov fr7,fr2 */
L8c617608: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61760a; /* fmov.s @(r0,r2),fr3 */
L8c61760a: r[0]=rd(m,bus,&io_status,0x8c6177b4u,4);if(io_status)return io_status;goto L8c61760c; /* mov.l 0x8c6177b4,r0 */
L8c61760c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61760e; /* fsub fr7,fr3 */
L8c61760e: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c617610; /* mov.l @(0x4,r15),r2 */
L8c617610: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617612; /* fmac fr0,fr3,fr2 */
L8c617612: fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c617614; /* fmov.s @r0,fr3 */
L8c617614: r[0]=16u;goto L8c617616; /* mov #0x10,r0 */
L8c617616: fcopy(f,7u,2u);goto L8c617618; /* fmov fr2,fr7 */
L8c617618: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61761a; /* fmul fr7,fr3 */
L8c61761a: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61761c; /* fmov.s @(r0,r13),fr2 */
L8c61761c: r[0]=20u;goto L8c61761e; /* mov #0x14,r0 */
L8c61761e: fcopy(f,0u,6u);goto L8c617620; /* fmov fr6,fr0 */
L8c617620: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617622; /* fmac fr0,fr3,fr2 */
L8c617622: fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c617624; /* fmov fr2,@(r0,r2) */
L8c617624: r[0]=24u;goto L8c617626; /* mov #0x18,r0 */
L8c617626: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c617628; /* mov.l @(0x4,r15),r3 */
L8c617628: r[2]=r[11];goto L8c61762a; /* mov r11,r2 */
L8c61762a: r[2]+=r[12];goto L8c61762c; /* add r12,r2 */
L8c61762c: fstore(m,bus,&io_status,f,0u,r[0]+r[3]);if(io_status)return io_status;goto L8c61762e; /* fmov fr0,@(r0,r3) */
L8c61762e: r[3]=r[14];goto L8c617630; /* mov r14,r3 */
L8c617630: fcopy(f,0u,5u);goto L8c617632; /* fmov fr5,fr0 */
L8c617632: r[3]<<=2;goto L8c617634; /* shll2 r3 */
L8c617634: r[3]+=r[6];goto L8c617636; /* add r6,r3 */
L8c617636: wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c617638; /* mov.l r3,@(0x10,r15) */
L8c617638: r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61763a; /* mov.l @r15,r0 */
L8c61763a: r[0]+=r[12];goto L8c61763c; /* add r12,r0 */
L8c61763c: wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;goto L8c61763e; /* mov.l r0,@(0x8,r15) */
L8c61763e: r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c617640; /* mov.w @r0,r0 */
L8c617640: wr(m,bus,&io_status,r[15]+0xcu,r[0],2);if(io_status)return io_status;goto L8c617642; /* mov.w r0,@(0xc,r15) */
L8c617642: *fpul=r[0];goto L8c617644; /* lds r0,FPUL */
L8c617644: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c617646; /* mov.l r2,@(0x4,r15) */
L8c617646: r[0]=sx(m,bus,&io_status,r[15]+0xcu,2);if(io_status)return io_status;goto L8c617648; /* mov.w @(0xc,r15),r0 */
L8c617648: r[2]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c61764a; /* mov.w @r2,r2 */
L8c61764a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61764c; /* float FPUL,fr3 */
L8c61764c: r[2]-=r[0];goto L8c61764e; /* sub r0,r2 */
L8c61764e: *fpul=r[2];goto L8c617650; /* lds r2,FPUL */
L8c617650: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c617652; /* float FPUL,fr2 */
L8c617652: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617654; /* fmac fr0,fr2,fr3 */
L8c617654: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c617656; /* ftrc fr3,FPUL */
L8c617656: r[2]=*fpul;goto L8c617658; /* sts FPUL,r2 */
L8c617658: wr(m,bus,&io_status,r[3],r[2],2);if(io_status)return io_status;goto L8c61765a; /* mov.w r2,@r3 */
L8c61765a: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61765c; /* mov.l @(0x8,r15),r2 */
L8c61765c: r[14]+=1u;goto L8c61765e; /* add #0x1,r14 */
L8c61765e: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c617660; /* mov.l @(0x10,r15),r3 */
L8c617660: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c617662; /* mov.w @(0x2,r2),r0 */
L8c617662: *fpul=r[0];goto L8c617664; /* lds r0,FPUL */
L8c617664: wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c617666; /* mov.w r0,@r15 */
L8c617666: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c617668; /* mov.l @(0x4,r15),r2 */
L8c617668: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61766a; /* float FPUL,fr3 */
L8c61766a: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61766c; /* mov.w @(0x2,r2),r0 */
L8c61766c: r[2]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c61766e; /* mov.w @r15,r2 */
L8c61766e: r[0]-=r[2];goto L8c617670; /* sub r2,r0 */
L8c617670: *fpul=r[0];goto L8c617672; /* lds r0,FPUL */
L8c617672: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c617674; /* float FPUL,fr2 */
L8c617674: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617676; /* fmac fr0,fr2,fr3 */
L8c617676: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c617678; /* ftrc fr3,FPUL */
L8c617678: r[0]=*fpul;goto L8c61767a; /* sts FPUL,r0 */
L8c61767a: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61767c; /* mov.w r0,@(0x2,r3) */
L8c61767c: r[0]=r[11];goto L8c61767e; /* mov r11,r0 */
L8c61767e: r[3]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c617680; /* mov.l @(r0,r7),r3 */
L8c617680: r[3]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c617682; /* mov.l @(0xc,r3),r3 */
L8c617682: *sr=(*sr&~1u)|!(r[5]&r[3]);goto L8c617684; /* tst r5,r3 */
L8c617684: if(!(*sr&1u))goto L8c6176d4;goto L8c617686; /* bf 0x8c6176d4 */
L8c617686: r[3]=r[11];goto L8c617688; /* mov r11,r3 */
L8c617688: r[2]=r[14];goto L8c61768a; /* mov r14,r2 */
L8c61768a: r[3]+=r[7];goto L8c61768c; /* add r7,r3 */
L8c61768c: r[2]<<=2;goto L8c61768e; /* shll2 r2 */
L8c61768e: r[0]=16u;goto L8c617690; /* mov #0x10,r0 */
L8c617690: r[2]<<=2;goto L8c617692; /* shll2 r2 */
L8c617692: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c617694; /* shll r2 */
L8c617694: r[2]+=r[4];goto L8c617696; /* add r4,r2 */
L8c617696: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c617698; /* mov.l r2,@r15 */
L8c617698: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61769a; /* mov.l r3,@(0x4,r15) */
L8c61769a: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61769c; /* mov.l @r3,r1 */
L8c61769c: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61769e; /* fmov.s @(r0,r1),fr3 */
L8c61769e: fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c6176a0; /* fmov fr3,@(r0,r2) */
L8c6176a0: r[0]=20u;goto L8c6176a2; /* mov #0x14,r0 */
L8c6176a2: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6176a4; /* mov.l @(0x4,r15),r2 */
L8c6176a4: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6176a6; /* mov.l @r15,r3 */
L8c6176a6: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6176a8; /* mov.l @r2,r1 */
L8c6176a8: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6176aa; /* fmov.s @(r0,r1),fr3 */
L8c6176aa: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6176ac; /* fmov fr3,@(r0,r3) */
L8c6176ac: r[0]=24u;goto L8c6176ae; /* mov #0x18,r0 */
L8c6176ae: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6176b0; /* mov.l @(0x4,r15),r2 */
L8c6176b0: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6176b2; /* mov.l @r15,r3 */
L8c6176b2: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6176b4; /* mov.l @r2,r1 */
L8c6176b4: r[2]=r[11];goto L8c6176b6; /* mov r11,r2 */
L8c6176b6: r[2]+=r[12];goto L8c6176b8; /* add r12,r2 */
L8c6176b8: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6176ba; /* fmov.s @(r0,r1),fr3 */
L8c6176ba: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6176bc; /* fmov fr3,@(r0,r3) */
L8c6176bc: r[3]=r[14];goto L8c6176be; /* mov r14,r3 */
L8c6176be: r[3]<<=2;goto L8c6176c0; /* shll2 r3 */
L8c6176c0: r[3]+=r[6];goto L8c6176c2; /* add r6,r3 */
L8c6176c2: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6176c4; /* mov.l r3,@r15 */
L8c6176c4: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6176c6; /* mov.l r2,@(0x4,r15) */
L8c6176c6: r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c6176c8; /* mov.w @r2,r1 */
L8c6176c8: r[14]+=1u;goto L8c6176ca; /* add #0x1,r14 */
L8c6176ca: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c6176cc; /* mov.w r1,@r3 */
L8c6176cc: r[0]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6176ce; /* mov.l @(0x4,r15),r0 */
L8c6176ce: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6176d0; /* mov.l @r15,r3 */
L8c6176d0: r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c6176d2; /* mov.w @(0x2,r0),r0 */
L8c6176d2: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6176d4; /* mov.w r0,@(0x2,r3) */
L8c6176d4: wr(m,bus,&io_status,r[15]+0x4u,r[10],4);if(io_status)return io_status;goto L8c6176d6; /* mov.l r10,@(0x4,r15) */
L8c6176d6: r[3]=3u;goto L8c6176d8; /* mov #0x3,r3 */
L8c6176d8: r[10]+=1u;goto L8c6176da; /* add #0x1,r10 */
L8c6176da: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[3]);goto L8c6176dc; /* cmp/ge r3,r10 */
L8c6176dc: if((*sr&1u))goto L8c6176e2;goto L8c6176de; /* bt 0x8c6176e2 */
L8c6176de: goto L8c61757a; /* bra 0x8c61757a */
L8c6176e2: r[0]=0x8c6177c0u;goto L8c6176e4; /* mova 0x8c6177c0,r0 */
L8c6176e4: r[5]=rd(m,bus,&io_status,0x8c6177bcu,4);if(io_status)return io_status;goto L8c6176e6; /* mov.l 0x8c6177bc,r5 */
L8c6176e6: fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c6176e8; /* fmov.s @r0,fr5 */
L8c6176e8: r[0]=r[14];goto L8c6176ea; /* mov r14,r0 */
L8c6176ea: r[7]=rd(m,bus,&io_status,0x8c6177b8u,4);if(io_status)return io_status;goto L8c6176ec; /* mov.l 0x8c6177b8,r7 */
L8c6176ec: *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c6176ee; /* cmp/eq #0x3,r0 */
L8c6176ee: if(!(*sr&1u))goto L8c6177c8;goto L8c6176f0; /* bf 0x8c6177c8 */
L8c6176f0: r[3]=r[6];goto L8c6176f2; /* mov r6,r3 */
L8c6176f2: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6176f4; /* mov.l r3,@r15 */
L8c6176f4: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6176f6; /* mov.w @(0x2,r3),r0 */
L8c6176f6: r[8]+=24u;goto L8c6176f8; /* add #0x18,r8 */
L8c6176f8: r[14]=r[4];goto L8c6176fa; /* mov r4,r14 */
L8c6176fa: r[3]=r[0];goto L8c6176fc; /* mov r0,r3 */
L8c6176fc: *fpul=r[3];goto L8c6176fe; /* lds r3,FPUL */
L8c6176fe: r[0]=24u;goto L8c617700; /* mov #0x18,r0 */
L8c617700: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617702; /* float FPUL,fr3 */
L8c617702: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617704; /* fmul fr5,fr3 */
L8c617704: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617706; /* fmov.s fr3,@-r8 */
L8c617706: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617708; /* mov.l @r15,r3 */
L8c617708: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c61770a; /* mov.w @r3,r3 */
L8c61770a: *fpul=r[3];goto L8c61770c; /* lds r3,FPUL */
L8c61770c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61770e; /* float FPUL,fr3 */
L8c61770e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617710; /* fmul fr5,fr3 */
L8c617710: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617712; /* fmov.s fr3,@-r8 */
L8c617712: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617714; /* fmov.s @(r0,r14),fr3 */
L8c617714: r[0]=20u;goto L8c617716; /* mov #0x14,r0 */
L8c617716: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617718; /* fmov.s fr3,@-r8 */
L8c617718: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61771a; /* fmov.s @(r0,r14),fr3 */
L8c61771a: r[0]=16u;goto L8c61771c; /* mov #0x10,r0 */
L8c61771c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61771e; /* fmov.s fr3,@-r8 */
L8c61771e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617720; /* fmov.s @(r0,r14),fr3 */
L8c617720: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617722; /* fmov.s fr3,@-r8 */
L8c617722: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c617724; /* mov.l r5,@-r8 */
L8c617724: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617726; /* pref @r8 */
L8c617726: r[3]=r[6];goto L8c617728; /* mov r6,r3 */
L8c617728: r[3]+=4u;goto L8c61772a; /* add #0x4,r3 */
L8c61772a: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61772c; /* mov.l r3,@r15 */
L8c61772c: r[8]+=56u;goto L8c61772e; /* add #0x38,r8 */
L8c61772e: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c617730; /* mov.w @(0x2,r3),r0 */
L8c617730: r[14]+=32u;goto L8c617732; /* add #0x20,r14 */
L8c617732: r[3]=r[0];goto L8c617734; /* mov r0,r3 */
L8c617734: *fpul=r[3];goto L8c617736; /* lds r3,FPUL */
L8c617736: r[0]=24u;goto L8c617738; /* mov #0x18,r0 */
L8c617738: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61773a; /* float FPUL,fr3 */
L8c61773a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61773c; /* fmul fr5,fr3 */
L8c61773c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61773e; /* fmov.s fr3,@-r8 */
L8c61773e: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617740; /* mov.l @r15,r3 */
L8c617740: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c617742; /* mov.w @r3,r3 */
L8c617742: *fpul=r[3];goto L8c617744; /* lds r3,FPUL */
L8c617744: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617746; /* float FPUL,fr3 */
L8c617746: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617748; /* fmul fr5,fr3 */
L8c617748: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61774a; /* fmov.s fr3,@-r8 */
L8c61774a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61774c; /* fmov.s @(r0,r14),fr3 */
L8c61774c: r[0]=20u;goto L8c61774e; /* mov #0x14,r0 */
L8c61774e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617750; /* fmov.s fr3,@-r8 */
L8c617750: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617752; /* fmov.s @(r0,r14),fr3 */
L8c617752: r[0]=16u;goto L8c617754; /* mov #0x10,r0 */
L8c617754: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617756; /* fmov.s fr3,@-r8 */
L8c617756: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617758; /* fmov.s @(r0,r14),fr3 */
L8c617758: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61775a; /* fmov.s fr3,@-r8 */
L8c61775a: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61775c; /* mov.l r5,@-r8 */
L8c61775c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61775e; /* pref @r8 */
L8c61775e: r[5]=r[6];goto L8c617760; /* mov r6,r5 */
L8c617760: r[5]+=8u;goto L8c617762; /* add #0x8,r5 */
L8c617762: r[8]+=60u;goto L8c617764; /* add #0x3c,r8 */
L8c617764: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c617766; /* fmov.s fr4,@-r8 */
L8c617766: r[0]=sx(m,bus,&io_status,r[5]+0x2u,2);if(io_status)return io_status;goto L8c617768; /* mov.w @(0x2,r5),r0 */
L8c617768: r[3]=r[0];goto L8c61776a; /* mov r0,r3 */
L8c61776a: *fpul=r[3];goto L8c61776c; /* lds r3,FPUL */
L8c61776c: r[0]=24u;goto L8c61776e; /* mov #0x18,r0 */
L8c61776e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617770; /* float FPUL,fr3 */
L8c617770: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617772; /* fmul fr5,fr3 */
L8c617772: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617774; /* fmov.s fr3,@-r8 */
L8c617774: r[3]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;goto L8c617776; /* mov.w @r5,r3 */
L8c617776: r[5]=r[4];goto L8c617778; /* mov r4,r5 */
L8c617778: r[5]+=64u;goto L8c61777a; /* add #0x40,r5 */
L8c61777a: *fpul=r[3];goto L8c61777c; /* lds r3,FPUL */
L8c61777c: r[3]=rd(m,bus,&io_status,0x8c6177c4u,4);if(io_status)return io_status;goto L8c61777e; /* mov.l 0x8c6177c4,r3 */
L8c61777e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617780; /* float FPUL,fr3 */
L8c617780: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617782; /* fmul fr5,fr3 */
L8c617782: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617784; /* fmov.s fr3,@-r8 */
L8c617784: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617786; /* fmov.s @(r0,r5),fr3 */
L8c617786: r[0]=20u;goto L8c617788; /* mov #0x14,r0 */
L8c617788: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61778a; /* fmov.s fr3,@-r8 */
L8c61778a: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61778c; /* fmov.s @(r0,r5),fr3 */
L8c61778c: r[0]=16u;goto L8c61778e; /* mov #0x10,r0 */
L8c61778e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617790; /* fmov.s fr3,@-r8 */
L8c617790: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617792; /* fmov.s @(r0,r5),fr3 */
L8c617792: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617794; /* fmov.s fr3,@-r8 */
L8c617794: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c617796; /* mov.l r3,@-r8 */
L8c617796: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617798; /* pref @r8 */
L8c617798: r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c61779a; /* mov.l @r7,r2 */
L8c61779a: r[8]+=32u;goto L8c61779c; /* add #0x20,r8 */
L8c61779c: r[2]+=3u;goto L8c6178b2; /* bra 0x8c6178b2 */
L8c6177c8: r[3]=r[6];goto L8c6177ca; /* mov r6,r3 */
L8c6177ca: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6177cc; /* mov.l r3,@r15 */
L8c6177cc: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6177ce; /* mov.w @(0x2,r3),r0 */
L8c6177ce: r[8]+=24u;goto L8c6177d0; /* add #0x18,r8 */
L8c6177d0: r[14]=r[4];goto L8c6177d2; /* mov r4,r14 */
L8c6177d2: r[3]=r[0];goto L8c6177d4; /* mov r0,r3 */
L8c6177d4: *fpul=r[3];goto L8c6177d6; /* lds r3,FPUL */
L8c6177d6: r[0]=24u;goto L8c6177d8; /* mov #0x18,r0 */
L8c6177d8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6177da; /* float FPUL,fr3 */
L8c6177da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6177dc; /* fmul fr5,fr3 */
L8c6177dc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6177de; /* fmov.s fr3,@-r8 */
L8c6177de: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6177e0; /* mov.l @r15,r3 */
L8c6177e0: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6177e2; /* mov.w @r3,r3 */
L8c6177e2: *fpul=r[3];goto L8c6177e4; /* lds r3,FPUL */
L8c6177e4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6177e6; /* float FPUL,fr3 */
L8c6177e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6177e8; /* fmul fr5,fr3 */
L8c6177e8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6177ea; /* fmov.s fr3,@-r8 */
L8c6177ea: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6177ec; /* fmov.s @(r0,r14),fr3 */
L8c6177ec: r[0]=20u;goto L8c6177ee; /* mov #0x14,r0 */
L8c6177ee: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6177f0; /* fmov.s fr3,@-r8 */
L8c6177f0: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6177f2; /* fmov.s @(r0,r14),fr3 */
L8c6177f2: r[0]=16u;goto L8c6177f4; /* mov #0x10,r0 */
L8c6177f4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6177f6; /* fmov.s fr3,@-r8 */
L8c6177f6: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6177f8; /* fmov.s @(r0,r14),fr3 */
L8c6177f8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6177fa; /* fmov.s fr3,@-r8 */
L8c6177fa: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6177fc; /* mov.l r5,@-r8 */
L8c6177fc: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6177fe; /* pref @r8 */
L8c6177fe: r[3]=r[6];goto L8c617800; /* mov r6,r3 */
L8c617800: r[3]+=4u;goto L8c617802; /* add #0x4,r3 */
L8c617802: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c617804; /* mov.l r3,@r15 */
L8c617804: r[8]+=56u;goto L8c617806; /* add #0x38,r8 */
L8c617806: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c617808; /* mov.w @(0x2,r3),r0 */
L8c617808: r[14]+=32u;goto L8c61780a; /* add #0x20,r14 */
L8c61780a: r[3]=r[0];goto L8c61780c; /* mov r0,r3 */
L8c61780c: *fpul=r[3];goto L8c61780e; /* lds r3,FPUL */
L8c61780e: r[0]=24u;goto L8c617810; /* mov #0x18,r0 */
L8c617810: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617812; /* float FPUL,fr3 */
L8c617812: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617814; /* fmul fr5,fr3 */
L8c617814: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617816; /* fmov.s fr3,@-r8 */
L8c617816: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617818; /* mov.l @r15,r3 */
L8c617818: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c61781a; /* mov.w @r3,r3 */
L8c61781a: *fpul=r[3];goto L8c61781c; /* lds r3,FPUL */
L8c61781c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61781e; /* float FPUL,fr3 */
L8c61781e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617820; /* fmul fr5,fr3 */
L8c617820: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617822; /* fmov.s fr3,@-r8 */
L8c617822: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617824; /* fmov.s @(r0,r14),fr3 */
L8c617824: r[0]=20u;goto L8c617826; /* mov #0x14,r0 */
L8c617826: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617828; /* fmov.s fr3,@-r8 */
L8c617828: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61782a; /* fmov.s @(r0,r14),fr3 */
L8c61782a: r[0]=16u;goto L8c61782c; /* mov #0x10,r0 */
L8c61782c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61782e; /* fmov.s fr3,@-r8 */
L8c61782e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617830; /* fmov.s @(r0,r14),fr3 */
L8c617830: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617832; /* fmov.s fr3,@-r8 */
L8c617832: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c617834; /* mov.l r5,@-r8 */
L8c617834: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617836; /* pref @r8 */
L8c617836: r[3]=r[6];goto L8c617838; /* mov r6,r3 */
L8c617838: r[3]+=12u;goto L8c61783a; /* add #0xc,r3 */
L8c61783a: r[8]+=60u;goto L8c61783c; /* add #0x3c,r8 */
L8c61783c: r[14]=r[4];goto L8c61783e; /* mov r4,r14 */
L8c61783e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c617840; /* fmov.s fr4,@-r8 */
L8c617840: r[14]+=96u;goto L8c617842; /* add #0x60,r14 */
L8c617842: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c617844; /* mov.l r3,@r15 */
L8c617844: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c617846; /* mov.w @(0x2,r3),r0 */
L8c617846: r[3]=r[0];goto L8c617848; /* mov r0,r3 */
L8c617848: *fpul=r[3];goto L8c61784a; /* lds r3,FPUL */
L8c61784a: r[0]=24u;goto L8c61784c; /* mov #0x18,r0 */
L8c61784c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61784e; /* float FPUL,fr3 */
L8c61784e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617850; /* fmul fr5,fr3 */
L8c617850: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617852; /* fmov.s fr3,@-r8 */
L8c617852: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617854; /* mov.l @r15,r3 */
L8c617854: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c617856; /* mov.w @r3,r3 */
L8c617856: *fpul=r[3];goto L8c617858; /* lds r3,FPUL */
L8c617858: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61785a; /* float FPUL,fr3 */
L8c61785a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61785c; /* fmul fr5,fr3 */
L8c61785c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61785e; /* fmov.s fr3,@-r8 */
L8c61785e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617860; /* fmov.s @(r0,r14),fr3 */
L8c617860: r[0]=20u;goto L8c617862; /* mov #0x14,r0 */
L8c617862: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617864; /* fmov.s fr3,@-r8 */
L8c617864: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617866; /* fmov.s @(r0,r14),fr3 */
L8c617866: r[0]=16u;goto L8c617868; /* mov #0x10,r0 */
L8c617868: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61786a; /* fmov.s fr3,@-r8 */
L8c61786a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61786c; /* fmov.s @(r0,r14),fr3 */
L8c61786c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61786e; /* fmov.s fr3,@-r8 */
L8c61786e: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c617870; /* mov.l r5,@-r8 */
L8c617870: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617872; /* pref @r8 */
L8c617872: r[5]=r[6];goto L8c617874; /* mov r6,r5 */
L8c617874: r[5]+=8u;goto L8c617876; /* add #0x8,r5 */
L8c617876: r[8]+=60u;goto L8c617878; /* add #0x3c,r8 */
L8c617878: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61787a; /* fmov.s fr4,@-r8 */
L8c61787a: r[0]=sx(m,bus,&io_status,r[5]+0x2u,2);if(io_status)return io_status;goto L8c61787c; /* mov.w @(0x2,r5),r0 */
L8c61787c: r[3]=r[0];goto L8c61787e; /* mov r0,r3 */
L8c61787e: *fpul=r[3];goto L8c617880; /* lds r3,FPUL */
L8c617880: r[0]=24u;goto L8c617882; /* mov #0x18,r0 */
L8c617882: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617884; /* float FPUL,fr3 */
L8c617884: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617886; /* fmul fr5,fr3 */
L8c617886: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617888; /* fmov.s fr3,@-r8 */
L8c617888: r[3]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;goto L8c61788a; /* mov.w @r5,r3 */
L8c61788a: r[5]=r[4];goto L8c61788c; /* mov r4,r5 */
L8c61788c: r[5]+=64u;goto L8c61788e; /* add #0x40,r5 */
L8c61788e: *fpul=r[3];goto L8c617890; /* lds r3,FPUL */
L8c617890: r[3]=rd(m,bus,&io_status,0x8c617954u,4);if(io_status)return io_status;goto L8c617892; /* mov.l 0x8c617954,r3 */
L8c617892: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c617894; /* float FPUL,fr3 */
L8c617894: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617896; /* fmul fr5,fr3 */
L8c617896: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617898; /* fmov.s fr3,@-r8 */
L8c617898: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61789a; /* fmov.s @(r0,r5),fr3 */
L8c61789a: r[0]=20u;goto L8c61789c; /* mov #0x14,r0 */
L8c61789c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61789e; /* fmov.s fr3,@-r8 */
L8c61789e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6178a0; /* fmov.s @(r0,r5),fr3 */
L8c6178a0: r[0]=16u;goto L8c6178a2; /* mov #0x10,r0 */
L8c6178a2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6178a4; /* fmov.s fr3,@-r8 */
L8c6178a4: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6178a6; /* fmov.s @(r0,r5),fr3 */
L8c6178a6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6178a8; /* fmov.s fr3,@-r8 */
L8c6178a8: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6178aa; /* mov.l r3,@-r8 */
L8c6178aa: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6178ac; /* pref @r8 */
L8c6178ac: r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c6178ae; /* mov.l @r7,r2 */
L8c6178ae: r[8]+=32u;goto L8c6178b0; /* add #0x20,r8 */
L8c6178b0: r[2]+=4u;goto L8c6178b2; /* add #0x4,r2 */
L8c6178b2: wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c6178b4; /* mov.l r2,@r7 */
L8c6178b4: r[15]+=80u;goto L8c6178b6; /* add #0x50,r15 */
L8c6178b6: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6178b8; /* fmov.s @r15+,fr12 */
L8c6178b8: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6178ba; /* mov.l @r15+,r9 */
L8c6178ba: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6178bc; /* mov.l @r15+,r10 */
L8c6178bc: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6178be; /* mov.l @r15+,r11 */
L8c6178be: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6178c0; /* mov.l @r15+,r12 */
L8c6178c0: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6178c2; /* mov.l @r15+,r13 */
L8c6178c2: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
