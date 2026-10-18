/* UV/constant-factor triangle near-plane clipper with portable queue interface; name inferred.
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
SA_STAGE_EXPORT int sa_uv_constant_triangle_near_clip_8c6124c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6124c0;
L8c6124c0: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6124c2; /* mov.l r14,@-r15 */
L8c6124c2: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6124c4; /* mov.l r13,@-r15 */
L8c6124c4: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6124c6; /* mov.l r12,@-r15 */
L8c6124c6: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6124c8; /* mov.l r11,@-r15 */
L8c6124c8: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6124ca; /* mov.l r10,@-r15 */
L8c6124ca: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6124cc; /* mov.l r9,@-r15 */
L8c6124cc: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c6124ce; /* fmov.s fr12,@-r15 */
L8c6124ce: r[15]+=4294967228u;goto L8c6124d0; /* add #-0x44,r15 */
L8c6124d0: r[3]=rd(m,bus,&io_status,0x8c612558u,4);if(io_status)return io_status;goto L8c6124d2; /* mov.l 0x8c612558,r3 */
L8c6124d2: r[14]=r[15];goto L8c6124d4; /* mov r15,r14 */
L8c6124d4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c6124d6; /* fldi1 fr5 */
L8c6124d6: r[14]+=56u;goto L8c6124d8; /* add #0x38,r14 */
L8c6124d8: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6124da; /* fmov.s @r3,fr3 */
L8c6124da: r[9]=r[15];goto L8c6124dc; /* mov r15,r9 */
L8c6124dc: fcopy(f,6u,5u);goto L8c6124de; /* fmov fr5,fr6 */
L8c6124de: r[2]=r[14];goto L8c6124e0; /* mov r14,r2 */
L8c6124e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[3],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6124e2; /* fdiv fr3,fr6 */
L8c6124e2: r[12]=rd(m,bus,&io_status,0x8c61255cu,4);if(io_status)return io_status;goto L8c6124e4; /* mov.l 0x8c61255c,r12 */
L8c6124e4: r[9]+=20u;goto L8c6124e6; /* add #0x14,r9 */
L8c6124e6: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6124e8; /* mov.l r2,@r15 */
L8c6124e8: wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c6124ea; /* mov.l r4,@r2 */
L8c6124ea: r[4]=1u;goto L8c6124ec; /* mov #0x1,r4 */
L8c6124ec: wr(m,bus,&io_status,r[14]+0x4u,r[5],4);if(io_status)return io_status;goto L8c6124ee; /* mov.l r5,@(0x4,r14) */
L8c6124ee: r[11]=0u;goto L8c6124f0; /* mov #0x0,r11 */
L8c6124f0: wr(m,bus,&io_status,r[14]+0x8u,r[6],4);if(io_status)return io_status;goto L8c6124f2; /* mov.l r6,@(0x8,r14) */
L8c6124f2: r[6]=r[9];goto L8c6124f4; /* mov r9,r6 */
L8c6124f4: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6124f6; /* mov.l @r15,r13 */
L8c6124f6: r[10]=r[9];goto L8c6124f8; /* mov r9,r10 */
L8c6124f8: r[5]=r[9];goto L8c6124fa; /* mov r9,r5 */
L8c6124fa: r[0]=r[11];goto L8c6124fc; /* mov r11,r0 */
L8c6124fc: r[0]<<=2;goto L8c6124fe; /* shll2 r0 */
L8c6124fe: r[3]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c612500; /* mov.l @(r0,r14),r3 */
L8c612500: r[2]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c612502; /* mov.l @(0xc,r3),r2 */
L8c612502: *sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c612504; /* tst r4,r2 */
L8c612504: if(!(*sr&1u))goto L8c612568;goto L8c612506; /* bf 0x8c612568 */
L8c612506: r[1]=r[11];goto L8c612508; /* mov r11,r1 */
L8c612508: r[1]<<=2;goto L8c61250a; /* shll2 r1 */
L8c61250a: r[1]+=r[14];goto L8c61250c; /* add r14,r1 */
L8c61250c: fcopy(f,4u,5u);goto L8c61250e; /* fmov fr5,fr4 */
L8c61250e: r[0]=24u;goto L8c612510; /* mov #0x18,r0 */
L8c612510: wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c612512; /* mov.l r1,@r15 */
L8c612512: r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c612514; /* mov.l @r1,r3 */
L8c612514: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c612516; /* mov.l r3,@(0x4,r15) */
L8c612516: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612518; /* fmov.s @(r0,r3),fr3 */
L8c612518: r[0]=12u;goto L8c61251a; /* mov #0xc,r0 */
L8c61251a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61251c; /* fdiv fr3,fr4 */
L8c61251c: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61251e; /* fmov.s @(r0,r12),fr3 */
L8c61251e: r[0]=16u;goto L8c612520; /* mov #0x10,r0 */
L8c612520: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c612522; /* fmov.s @(r0,r3),fr2 */
L8c612522: r[0]=16u;goto L8c612524; /* mov #0x10,r0 */
L8c612524: r[3]=rd(m,bus,&io_status,0x8c612560u,4);if(io_status)return io_status;goto L8c612526; /* mov.l 0x8c612560,r3 */
L8c612526: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612528; /* fsub fr3,fr2 */
L8c612528: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61252a; /* fmov.s @r3,fr3 */
L8c61252a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61252c; /* fmul fr4,fr2 */
L8c61252c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61252e; /* fdiv fr3,fr2 */
L8c61252e: fstore(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c612530; /* fmov.s fr2,@r6 */
L8c612530: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612532; /* mov.l @r15,r2 */
L8c612532: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c612534; /* fmov.s @(r0,r12),fr3 */
L8c612534: r[0]=20u;goto L8c612536; /* mov #0x14,r0 */
L8c612536: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c612538; /* mov.l @r2,r1 */
L8c612538: r[2]=rd(m,bus,&io_status,0x8c612564u,4);if(io_status)return io_status;goto L8c61253a; /* mov.l 0x8c612564,r2 */
L8c61253a: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61253c; /* fmov.s @(r0,r1),fr2 */
L8c61253c: r[0]=4u;goto L8c61253e; /* mov #0x4,r0 */
L8c61253e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612540; /* fsub fr3,fr2 */
L8c612540: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c612542; /* fmov.s @r2,fr3 */
L8c612542: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612544; /* fmul fr4,fr2 */
L8c612544: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612546; /* fdiv fr3,fr2 */
L8c612546: fstore(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c612548; /* fmov fr2,@(r0,r6) */
L8c612548: r[0]=8u;goto L8c61254a; /* mov #0x8,r0 */
L8c61254a: fstore(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c612584; /* bra 0x8c612584 */
L8c612568: r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61256a; /* mov.l @r13,r3 */
L8c61256a: r[0]=16u;goto L8c61256c; /* mov #0x10,r0 */
L8c61256c: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61256e; /* fmov.s @(r0,r3),fr3 */
L8c61256e: r[0]=20u;goto L8c612570; /* mov #0x14,r0 */
L8c612570: fstore(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c612572; /* fmov.s fr3,@r5 */
L8c612572: r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c612574; /* mov.l @r13,r3 */
L8c612574: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612576; /* fmov.s @(r0,r3),fr3 */
L8c612576: r[0]=4u;goto L8c612578; /* mov #0x4,r0 */
L8c612578: fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61257a; /* fmov fr3,@(r0,r5) */
L8c61257a: r[0]=24u;goto L8c61257c; /* mov #0x18,r0 */
L8c61257c: r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61257e; /* mov.l @r13,r3 */
L8c61257e: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612580; /* fmov.s @(r0,r3),fr3 */
L8c612580: r[0]=8u;goto L8c612582; /* mov #0x8,r0 */
L8c612582: fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c612584; /* fmov fr3,@(r0,r5) */
L8c612584: r[3]=3u;goto L8c612586; /* mov #0x3,r3 */
L8c612586: r[11]+=1u;goto L8c612588; /* add #0x1,r11 */
L8c612588: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[3]);goto L8c61258a; /* cmp/ge r3,r11 */
L8c61258a: r[13]+=4u;goto L8c61258c; /* add #0x4,r13 */
L8c61258c: r[5]+=12u;goto L8c61258e; /* add #0xc,r5 */
L8c61258e: take=!(*sr&1u);r[6]+=12u;if(take)goto L8c6124fa;goto L8c612592; /* bf/s 0x8c6124fa */
L8c612592: r[0]=8u;goto L8c612594; /* mov #0x8,r0 */
L8c612594: r[6]=r[9];goto L8c612596; /* mov r9,r6 */
L8c612596: fload(m,bus,&io_status,f,8u,r[0]+r[10]);if(io_status)return io_status;goto L8c612598; /* fmov.s @(r0,r10),fr8 */
L8c612598: r[6]+=12u;goto L8c61259a; /* add #0xc,r6 */
L8c61259a: r[0]=4u;goto L8c61259c; /* mov #0x4,r0 */
L8c61259c: fload(m,bus,&io_status,f,11u,r[10]);if(io_status)return io_status;goto L8c61259e; /* fmov.s @r10,fr11 */
L8c61259e: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c6125a0; /* fmov.s @(r0,r6),fr4 */
L8c6125a0: r[5]=r[9];goto L8c6125a2; /* mov r9,r5 */
L8c6125a2: fload(m,bus,&io_status,f,10u,r[0]+r[10]);if(io_status)return io_status;goto L8c6125a4; /* fmov.s @(r0,r10),fr10 */
L8c6125a4: r[0]=8u;goto L8c6125a6; /* mov #0x8,r0 */
L8c6125a6: fload(m,bus,&io_status,f,7u,r[0]+r[6]);if(io_status)return io_status;goto L8c6125a8; /* fmov.s @(r0,r6),fr7 */
L8c6125a8: r[0]=4u;goto L8c6125aa; /* mov #0x4,r0 */
L8c6125aa: fcopy(f,3u,10u);goto L8c6125ac; /* fmov fr10,fr3 */
L8c6125ac: r[5]+=24u;goto L8c6125ae; /* add #0x18,r5 */
L8c6125ae: fcopy(f,0u,8u);goto L8c6125b0; /* fmov fr8,fr0 */
L8c6125b0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6125b2; /* fmul fr4,fr0 */
L8c6125b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6125b4; /* fmul fr7,fr3 */
L8c6125b4: fload(m,bus,&io_status,f,9u,r[6]);if(io_status)return io_status;goto L8c6125b6; /* fmov.s @r6,fr9 */
L8c6125b6: fcopy(f,2u,11u);goto L8c6125b8; /* fmov fr11,fr2 */
L8c6125b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6125ba; /* fmul fr7,fr2 */
L8c6125ba: fcopy(f,12u,11u);goto L8c6125bc; /* fmov fr11,fr12 */
L8c6125bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[4],&f->fpscr,&fr[12]);if(status)return status;}goto L8c6125be; /* fmul fr4,fr12 */
L8c6125be: fcopy(f,1u,9u);goto L8c6125c0; /* fmov fr9,fr1 */
L8c6125c0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[10],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6125c2; /* fmul fr10,fr1 */
L8c6125c2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6125c4; /* fsub fr3,fr0 */
L8c6125c4: fcopy(f,3u,8u);goto L8c6125c6; /* fmov fr8,fr3 */
L8c6125c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6125c8; /* fmul fr9,fr3 */
L8c6125c8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6125ca; /* fsub fr3,fr2 */
L8c6125ca: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6125cc; /* fmov.s @(r0,r5),fr3 */
L8c6125cc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6125ce; /* fmul fr2,fr3 */
L8c6125ce: fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c6125d0; /* fmov.s @r5,fr2 */
L8c6125d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6125d2; /* fmac fr0,fr2,fr3 */
L8c6125d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[12],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6125d4; /* fsub fr12,fr1 */
L8c6125d4: r[0]=8u;goto L8c6125d6; /* mov #0x8,r0 */
L8c6125d6: fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c6125d8; /* fmov.s @(r0,r5),fr2 */
L8c6125d8: r[5]=rd(m,bus,&io_status,0x8c612830u,4);if(io_status)return io_status;goto L8c6125da; /* mov.l 0x8c612830,r5 */
L8c6125da: fcopy(f,0u,1u);goto L8c6125dc; /* fmov fr1,fr0 */
L8c6125dc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6125de; /* fmac fr0,fr2,fr3 */
L8c6125de: r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c6125e0; /* mov.l @r5,r3 */
L8c6125e0: fcopy(f,4u,3u);goto L8c6125e2; /* fmov fr3,fr4 */
L8c6125e2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6125e4; /* fldi0 fr3 */
L8c6125e4: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6125e6; /* fcmp/gt fr4,fr3 */
L8c6125e6: r[0]=*sr&1u;goto L8c6125e8; /* movt r0 */
L8c6125e8: *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c6125ea; /* tst r3,r0 */
L8c6125ea: if((*sr&1u))goto L8c6125f0;goto L8c6125ec; /* bt 0x8c6125f0 */
L8c6125ec: goto L8c612948; /* bra 0x8c612948 */
L8c6125f0: r[1]=0u;goto L8c6125f2; /* mov #0x0,r1 */
L8c6125f2: r[6]=rd(m,bus,&io_status,0x8c612834u,4);if(io_status)return io_status;goto L8c6125f4; /* mov.l 0x8c612834,r6 */
L8c6125f4: wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c6125f6; /* mov.l r1,@r5 */
L8c6125f6: r[3]=2u;goto L8c6125f8; /* mov #0x2,r3 */
L8c6125f8: r[5]=rd(m,bus,&io_status,0x8c612838u,4);if(io_status)return io_status;goto L8c6125fa; /* mov.l 0x8c612838,r5 */
L8c6125fa: r[13]=r[1];goto L8c6125fc; /* mov r1,r13 */
L8c6125fc: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6125fe; /* mov.l r3,@(0x8,r15) */
L8c6125fe: r[10]=r[13];goto L8c612600; /* mov r13,r10 */
L8c612600: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612602; /* mov.l @(0x8,r15),r3 */
L8c612602: r[0]=r[14];goto L8c612604; /* mov r14,r0 */
L8c612604: r[11]=r[10];goto L8c612606; /* mov r10,r11 */
L8c612606: r[11]<<=2;goto L8c612608; /* shll2 r11 */
L8c612608: r[3]<<=2;goto L8c61260a; /* shll2 r3 */
L8c61260a: wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c61260c; /* mov.l r3,@(0xc,r15) */
L8c61260c: r[2]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c61260e; /* mov.l @(r0,r3),r2 */
L8c61260e: r[3]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612610; /* mov.l @(0xc,r2),r3 */
L8c612610: r[2]=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;goto L8c612612; /* mov.l @(r0,r11),r2 */
L8c612612: r[3]&=r[4];goto L8c612614; /* and r4,r3 */
L8c612614: if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[2]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612616; /* mov.l @(0xc,r2),r2 */
L8c612616: r[2]&=r[4];goto L8c612618; /* and r4,r2 */
L8c612618: r[3]^=r[2];goto L8c61261a; /* xor r2,r3 */
L8c61261a: r[0]=r[3];goto L8c61261c; /* mov r3,r0 */
L8c61261c: *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c61261e; /* cmp/eq #0x1,r0 */
L8c61261e: if(!(*sr&1u))goto L8c612702;goto L8c612620; /* bf 0x8c612702 */
L8c612620: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612622; /* mov.l @(0x8,r15),r2 */
L8c612622: r[0]=8u;goto L8c612624; /* mov #0x8,r0 */
L8c612624: r[1]=r[10];goto L8c612626; /* mov r10,r1 */
L8c612626: r[3]=r[2];goto L8c612628; /* mov r2,r3 */
L8c612628: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c61262a; /* shll r2 */
L8c61262a: r[2]+=r[3];goto L8c61262c; /* add r3,r2 */
L8c61262c: r[3]=rd(m,bus,&io_status,0x8c61283cu,4);if(io_status)return io_status;goto L8c61262e; /* mov.l 0x8c61283c,r3 */
L8c61262e: r[2]<<=2;goto L8c612630; /* shll2 r2 */
L8c612630: r[2]+=r[9];goto L8c612632; /* add r9,r2 */
L8c612632: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c612634; /* mov.l r2,@r15 */
L8c612634: fload(m,bus,&io_status,f,4u,r[0]+r[2]);if(io_status)return io_status;goto L8c612636; /* fmov.s @(r0,r2),fr4 */
L8c612636: r[2]=r[10];goto L8c612638; /* mov r10,r2 */
L8c612638: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c61263a; /* shll r2 */
L8c61263a: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61263c; /* fmov.s @r3,fr3 */
L8c61263c: r[2]+=r[1];goto L8c61263e; /* add r1,r2 */
L8c61263e: r[2]<<=2;goto L8c612640; /* shll2 r2 */
L8c612640: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612642; /* fsub fr4,fr3 */
L8c612642: r[0]=8u;goto L8c612644; /* mov #0x8,r0 */
L8c612644: r[2]+=r[9];goto L8c612646; /* add r9,r2 */
L8c612646: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c612648; /* mov.l r2,@(0x4,r15) */
L8c612648: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61264a; /* fmov.s @(r0,r2),fr2 */
L8c61264a: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61264c; /* mov.l @(0x4,r15),r1 */
L8c61264c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61264e; /* fsub fr4,fr2 */
L8c61264e: fcopy(f,4u,3u);goto L8c612650; /* fmov fr3,fr4 */
L8c612650: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612652; /* mov.l @r15,r2 */
L8c612652: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c612654; /* fmov.s @r1,fr3 */
L8c612654: fload(m,bus,&io_status,f,7u,r[2]);if(io_status)return io_status;goto L8c612656; /* fmov.s @r2,fr7 */
L8c612656: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c612658; /* fdiv fr2,fr4 */
L8c612658: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61265a; /* fsub fr7,fr3 */
L8c61265a: fcopy(f,2u,7u);goto L8c61265c; /* fmov fr7,fr2 */
L8c61265c: fcopy(f,0u,4u);goto L8c61265e; /* fmov fr4,fr0 */
L8c61265e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612660; /* fmac fr0,fr3,fr2 */
L8c612660: r[1]=rd(m,bus,&io_status,0x8c612840u,4);if(io_status)return io_status;goto L8c612662; /* mov.l 0x8c612840,r1 */
L8c612662: r[0]=12u;goto L8c612664; /* mov #0xc,r0 */
L8c612664: fcopy(f,7u,2u);goto L8c612666; /* fmov fr2,fr7 */
L8c612666: r[2]=r[13];goto L8c612668; /* mov r13,r2 */
L8c612668: fcopy(f,0u,6u);goto L8c61266a; /* fmov fr6,fr0 */
L8c61266a: r[2]<<=2;goto L8c61266c; /* shll2 r2 */
L8c61266c: r[2]<<=2;goto L8c61266e; /* shll2 r2 */
L8c61266e: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612670; /* shll r2 */
L8c612670: r[2]+=r[5];goto L8c612672; /* add r5,r2 */
L8c612672: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c612674; /* mov.l r2,@(0x8,r15) */
L8c612674: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c612676; /* fmov.s @r1,fr3 */
L8c612676: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c612678; /* fmov.s @(r0,r12),fr2 */
L8c612678: r[0]=16u;goto L8c61267a; /* mov #0x10,r0 */
L8c61267a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61267c; /* fmul fr7,fr3 */
L8c61267c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61267e; /* fmac fr0,fr3,fr2 */
L8c61267e: fcopy(f,0u,4u);goto L8c612680; /* fmov fr4,fr0 */
L8c612680: fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c612682; /* fmov fr2,@(r0,r2) */
L8c612682: r[0]=4u;goto L8c612684; /* mov #0x4,r0 */
L8c612684: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612686; /* mov.l @r15,r2 */
L8c612686: fload(m,bus,&io_status,f,7u,r[0]+r[2]);if(io_status)return io_status;goto L8c612688; /* fmov.s @(r0,r2),fr7 */
L8c612688: r[0]=4u;goto L8c61268a; /* mov #0x4,r0 */
L8c61268a: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61268c; /* mov.l @(0x4,r15),r2 */
L8c61268c: fcopy(f,2u,7u);goto L8c61268e; /* fmov fr7,fr2 */
L8c61268e: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612690; /* fmov.s @(r0,r2),fr3 */
L8c612690: r[0]=rd(m,bus,&io_status,0x8c612844u,4);if(io_status)return io_status;goto L8c612692; /* mov.l 0x8c612844,r0 */
L8c612692: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612694; /* fsub fr7,fr3 */
L8c612694: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612696; /* mov.l @(0x8,r15),r2 */
L8c612696: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612698; /* fmac fr0,fr3,fr2 */
L8c612698: fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c61269a; /* fmov.s @r0,fr3 */
L8c61269a: r[0]=16u;goto L8c61269c; /* mov #0x10,r0 */
L8c61269c: fcopy(f,7u,2u);goto L8c61269e; /* fmov fr2,fr7 */
L8c61269e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6126a0; /* fmul fr7,fr3 */
L8c6126a0: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6126a2; /* fmov.s @(r0,r12),fr2 */
L8c6126a2: r[0]=20u;goto L8c6126a4; /* mov #0x14,r0 */
L8c6126a4: fcopy(f,0u,6u);goto L8c6126a6; /* fmov fr6,fr0 */
L8c6126a6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6126a8; /* fmac fr0,fr3,fr2 */
L8c6126a8: fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6126aa; /* fmov fr2,@(r0,r2) */
L8c6126aa: r[0]=24u;goto L8c6126ac; /* mov #0x18,r0 */
L8c6126ac: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6126ae; /* mov.l @(0x8,r15),r3 */
L8c6126ae: fstore(m,bus,&io_status,f,0u,r[0]+r[3]);if(io_status)return io_status;goto L8c6126b0; /* fmov fr0,@(r0,r3) */
L8c6126b0: r[0]=r[11];goto L8c6126b2; /* mov r11,r0 */
L8c6126b2: r[0]+=r[7];goto L8c6126b4; /* add r7,r0 */
L8c6126b4: fcopy(f,0u,4u);goto L8c6126b6; /* fmov fr4,fr0 */
L8c6126b6: r[3]=r[13];goto L8c6126b8; /* mov r13,r3 */
L8c6126b8: r[3]<<=2;goto L8c6126ba; /* shll2 r3 */
L8c6126ba: r[3]+=r[6];goto L8c6126bc; /* add r6,r3 */
L8c6126bc: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6126be; /* mov.l r3,@(0x4,r15) */
L8c6126be: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6126c0; /* mov.l @(0xc,r15),r2 */
L8c6126c0: r[2]+=r[7];goto L8c6126c2; /* add r7,r2 */
L8c6126c2: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6126c4; /* mov.l r2,@(0x8,r15) */
L8c6126c4: r[2]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c6126c6; /* mov.w @r2,r2 */
L8c6126c6: wr(m,bus,&io_status,r[15],r[2],2);if(io_status)return io_status;goto L8c6126c8; /* mov.w r2,@r15 */
L8c6126c8: *fpul=r[2];goto L8c6126ca; /* lds r2,FPUL */
L8c6126ca: wr(m,bus,&io_status,r[15]+0x10u,r[0],4);if(io_status)return io_status;goto L8c6126cc; /* mov.l r0,@(0x10,r15) */
L8c6126cc: r[2]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c6126ce; /* mov.w @r0,r2 */
L8c6126ce: r[0]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c6126d0; /* mov.w @r15,r0 */
L8c6126d0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6126d2; /* float FPUL,fr3 */
L8c6126d2: r[2]-=r[0];goto L8c6126d4; /* sub r0,r2 */
L8c6126d4: *fpul=r[2];goto L8c6126d6; /* lds r2,FPUL */
L8c6126d6: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c6126d8; /* float FPUL,fr2 */
L8c6126d8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6126da; /* fmac fr0,fr2,fr3 */
L8c6126da: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6126dc; /* ftrc fr3,FPUL */
L8c6126dc: r[2]=*fpul;goto L8c6126de; /* sts FPUL,r2 */
L8c6126de: wr(m,bus,&io_status,r[3],r[2],2);if(io_status)return io_status;goto L8c6126e0; /* mov.w r2,@r3 */
L8c6126e0: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6126e2; /* mov.l @(0x8,r15),r2 */
L8c6126e2: r[13]+=1u;goto L8c6126e4; /* add #0x1,r13 */
L8c6126e4: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6126e6; /* mov.l @(0x4,r15),r3 */
L8c6126e6: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c6126e8; /* mov.w @(0x2,r2),r0 */
L8c6126e8: *fpul=r[0];goto L8c6126ea; /* lds r0,FPUL */
L8c6126ea: wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c6126ec; /* mov.w r0,@r15 */
L8c6126ec: r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c6126ee; /* mov.l @(0x10,r15),r2 */
L8c6126ee: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6126f0; /* float FPUL,fr3 */
L8c6126f0: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c6126f2; /* mov.w @(0x2,r2),r0 */
L8c6126f2: r[2]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c6126f4; /* mov.w @r15,r2 */
L8c6126f4: r[0]-=r[2];goto L8c6126f6; /* sub r2,r0 */
L8c6126f6: *fpul=r[0];goto L8c6126f8; /* lds r0,FPUL */
L8c6126f8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c6126fa; /* float FPUL,fr2 */
L8c6126fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6126fc; /* fmac fr0,fr2,fr3 */
L8c6126fc: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6126fe; /* ftrc fr3,FPUL */
L8c6126fe: r[0]=*fpul;goto L8c612700; /* sts FPUL,r0 */
L8c612700: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c612702; /* mov.w r0,@(0x2,r3) */
L8c612702: r[0]=r[11];goto L8c612704; /* mov r11,r0 */
L8c612704: r[3]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c612706; /* mov.l @(r0,r14),r3 */
L8c612706: r[3]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c612708; /* mov.l @(0xc,r3),r3 */
L8c612708: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c61270a; /* tst r4,r3 */
L8c61270a: if(!(*sr&1u))goto L8c61275a;goto L8c61270c; /* bf 0x8c61275a */
L8c61270c: r[3]=r[11];goto L8c61270e; /* mov r11,r3 */
L8c61270e: r[2]=r[13];goto L8c612710; /* mov r13,r2 */
L8c612710: r[3]+=r[14];goto L8c612712; /* add r14,r3 */
L8c612712: r[2]<<=2;goto L8c612714; /* shll2 r2 */
L8c612714: r[0]=16u;goto L8c612716; /* mov #0x10,r0 */
L8c612716: r[2]<<=2;goto L8c612718; /* shll2 r2 */
L8c612718: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c61271a; /* shll r2 */
L8c61271a: r[2]+=r[5];goto L8c61271c; /* add r5,r2 */
L8c61271c: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61271e; /* mov.l r2,@r15 */
L8c61271e: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c612720; /* mov.l r3,@(0x4,r15) */
L8c612720: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c612722; /* mov.l @r3,r1 */
L8c612722: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612724; /* fmov.s @(r0,r1),fr3 */
L8c612724: fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612726; /* fmov fr3,@(r0,r2) */
L8c612726: r[0]=20u;goto L8c612728; /* mov #0x14,r0 */
L8c612728: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61272a; /* mov.l @(0x4,r15),r2 */
L8c61272a: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61272c; /* mov.l @r15,r3 */
L8c61272c: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61272e; /* mov.l @r2,r1 */
L8c61272e: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612730; /* fmov.s @(r0,r1),fr3 */
L8c612730: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612732; /* fmov fr3,@(r0,r3) */
L8c612732: r[0]=24u;goto L8c612734; /* mov #0x18,r0 */
L8c612734: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612736; /* mov.l @(0x4,r15),r2 */
L8c612736: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612738; /* mov.l @r15,r3 */
L8c612738: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61273a; /* mov.l @r2,r1 */
L8c61273a: r[2]=r[11];goto L8c61273c; /* mov r11,r2 */
L8c61273c: r[2]+=r[7];goto L8c61273e; /* add r7,r2 */
L8c61273e: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612740; /* fmov.s @(r0,r1),fr3 */
L8c612740: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612742; /* fmov fr3,@(r0,r3) */
L8c612742: r[3]=r[13];goto L8c612744; /* mov r13,r3 */
L8c612744: r[3]<<=2;goto L8c612746; /* shll2 r3 */
L8c612746: r[3]+=r[6];goto L8c612748; /* add r6,r3 */
L8c612748: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61274a; /* mov.l r3,@(0x4,r15) */
L8c61274a: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61274c; /* mov.l r2,@r15 */
L8c61274c: r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c61274e; /* mov.w @r2,r1 */
L8c61274e: r[13]+=1u;goto L8c612750; /* add #0x1,r13 */
L8c612750: wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c612752; /* mov.w r1,@r3 */
L8c612752: r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612754; /* mov.l @r15,r0 */
L8c612754: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612756; /* mov.l @(0x4,r15),r3 */
L8c612756: r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c612758; /* mov.w @(0x2,r0),r0 */
L8c612758: wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61275a; /* mov.w r0,@(0x2,r3) */
L8c61275a: wr(m,bus,&io_status,r[15]+0x8u,r[10],4);if(io_status)return io_status;goto L8c61275c; /* mov.l r10,@(0x8,r15) */
L8c61275c: r[3]=3u;goto L8c61275e; /* mov #0x3,r3 */
L8c61275e: r[10]+=1u;goto L8c612760; /* add #0x1,r10 */
L8c612760: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[3]);goto L8c612762; /* cmp/ge r3,r10 */
L8c612762: if((*sr&1u))goto L8c612768;goto L8c612764; /* bt 0x8c612768 */
L8c612764: goto L8c612600; /* bra 0x8c612600 */
L8c612768: r[0]=0x8c612850u;goto L8c61276a; /* mova 0x8c612850,r0 */
L8c61276a: r[4]=rd(m,bus,&io_status,0x8c61284cu,4);if(io_status)return io_status;goto L8c61276c; /* mov.l 0x8c61284c,r4 */
L8c61276c: fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c61276e; /* fmov.s @r0,fr4 */
L8c61276e: r[0]=r[13];goto L8c612770; /* mov r13,r0 */
L8c612770: r[7]=rd(m,bus,&io_status,0x8c612848u,4);if(io_status)return io_status;goto L8c612772; /* mov.l 0x8c612848,r7 */
L8c612772: *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c612774; /* cmp/eq #0x3,r0 */
L8c612774: if(!(*sr&1u))goto L8c612858;goto L8c612776; /* bf 0x8c612858 */
L8c612776: r[3]=r[6];goto L8c612778; /* mov r6,r3 */
L8c612778: r[8]+=28u;goto L8c61277a; /* add #0x1c,r8 */
L8c61277a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61277c; /* fmov.s fr5,@-r8 */
L8c61277c: r[14]=r[5];goto L8c61277e; /* mov r5,r14 */
L8c61277e: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c612780; /* mov.l r3,@r15 */
L8c612780: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c612782; /* mov.w @(0x2,r3),r0 */
L8c612782: r[3]=r[0];goto L8c612784; /* mov r0,r3 */
L8c612784: *fpul=r[3];goto L8c612786; /* lds r3,FPUL */
L8c612786: r[0]=24u;goto L8c612788; /* mov #0x18,r0 */
L8c612788: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61278a; /* float FPUL,fr3 */
L8c61278a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61278c; /* fmul fr4,fr3 */
L8c61278c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61278e; /* fmov.s fr3,@-r8 */
L8c61278e: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612790; /* mov.l @r15,r3 */
L8c612790: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c612792; /* mov.w @r3,r3 */
L8c612792: *fpul=r[3];goto L8c612794; /* lds r3,FPUL */
L8c612794: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612796; /* float FPUL,fr3 */
L8c612796: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612798; /* fmul fr4,fr3 */
L8c612798: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61279a; /* fmov.s fr3,@-r8 */
L8c61279a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61279c; /* fmov.s @(r0,r14),fr3 */
L8c61279c: r[0]=20u;goto L8c61279e; /* mov #0x14,r0 */
L8c61279e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127a0; /* fmov.s fr3,@-r8 */
L8c6127a0: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6127a2; /* fmov.s @(r0,r14),fr3 */
L8c6127a2: r[0]=16u;goto L8c6127a4; /* mov #0x10,r0 */
L8c6127a4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127a6; /* fmov.s fr3,@-r8 */
L8c6127a6: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6127a8; /* fmov.s @(r0,r14),fr3 */
L8c6127a8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127aa; /* fmov.s fr3,@-r8 */
L8c6127aa: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c6127ac; /* mov.l r4,@-r8 */
L8c6127ac: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6127ae; /* pref @r8 */
L8c6127ae: r[3]=r[6];goto L8c6127b0; /* mov r6,r3 */
L8c6127b0: r[3]+=4u;goto L8c6127b2; /* add #0x4,r3 */
L8c6127b2: r[8]+=60u;goto L8c6127b4; /* add #0x3c,r8 */
L8c6127b4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6127b6; /* fmov.s fr5,@-r8 */
L8c6127b6: r[14]+=32u;goto L8c6127b8; /* add #0x20,r14 */
L8c6127b8: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6127ba; /* mov.l r3,@r15 */
L8c6127ba: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6127bc; /* mov.w @(0x2,r3),r0 */
L8c6127bc: r[3]=r[0];goto L8c6127be; /* mov r0,r3 */
L8c6127be: *fpul=r[3];goto L8c6127c0; /* lds r3,FPUL */
L8c6127c0: r[0]=24u;goto L8c6127c2; /* mov #0x18,r0 */
L8c6127c2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6127c4; /* float FPUL,fr3 */
L8c6127c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6127c6; /* fmul fr4,fr3 */
L8c6127c6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127c8; /* fmov.s fr3,@-r8 */
L8c6127c8: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6127ca; /* mov.l @r15,r3 */
L8c6127ca: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6127cc; /* mov.w @r3,r3 */
L8c6127cc: *fpul=r[3];goto L8c6127ce; /* lds r3,FPUL */
L8c6127ce: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6127d0; /* float FPUL,fr3 */
L8c6127d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6127d2; /* fmul fr4,fr3 */
L8c6127d2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127d4; /* fmov.s fr3,@-r8 */
L8c6127d4: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6127d6; /* fmov.s @(r0,r14),fr3 */
L8c6127d6: r[0]=20u;goto L8c6127d8; /* mov #0x14,r0 */
L8c6127d8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127da; /* fmov.s fr3,@-r8 */
L8c6127da: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6127dc; /* fmov.s @(r0,r14),fr3 */
L8c6127dc: r[0]=16u;goto L8c6127de; /* mov #0x10,r0 */
L8c6127de: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127e0; /* fmov.s fr3,@-r8 */
L8c6127e0: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6127e2; /* fmov.s @(r0,r14),fr3 */
L8c6127e2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127e4; /* fmov.s fr3,@-r8 */
L8c6127e4: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c6127e6; /* mov.l r4,@-r8 */
L8c6127e6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6127e8; /* pref @r8 */
L8c6127e8: r[4]=r[6];goto L8c6127ea; /* mov r6,r4 */
L8c6127ea: r[4]+=8u;goto L8c6127ec; /* add #0x8,r4 */
L8c6127ec: r[8]+=60u;goto L8c6127ee; /* add #0x3c,r8 */
L8c6127ee: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6127f0; /* fmov.s fr5,@-r8 */
L8c6127f0: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c6127f2; /* mov.w @(0x2,r4),r0 */
L8c6127f2: r[3]=r[0];goto L8c6127f4; /* mov r0,r3 */
L8c6127f4: *fpul=r[3];goto L8c6127f6; /* lds r3,FPUL */
L8c6127f6: r[0]=24u;goto L8c6127f8; /* mov #0x18,r0 */
L8c6127f8: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6127fa; /* float FPUL,fr3 */
L8c6127fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6127fc; /* fmul fr4,fr3 */
L8c6127fc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127fe; /* fmov.s fr3,@-r8 */
L8c6127fe: r[3]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c612800; /* mov.w @r4,r3 */
L8c612800: r[4]=r[5];goto L8c612802; /* mov r5,r4 */
L8c612802: r[4]+=64u;goto L8c612804; /* add #0x40,r4 */
L8c612804: *fpul=r[3];goto L8c612806; /* lds r3,FPUL */
L8c612806: r[3]=rd(m,bus,&io_status,0x8c612854u,4);if(io_status)return io_status;goto L8c612808; /* mov.l 0x8c612854,r3 */
L8c612808: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61280a; /* float FPUL,fr3 */
L8c61280a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61280c; /* fmul fr4,fr3 */
L8c61280c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61280e; /* fmov.s fr3,@-r8 */
L8c61280e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612810; /* fmov.s @(r0,r4),fr3 */
L8c612810: r[0]=20u;goto L8c612812; /* mov #0x14,r0 */
L8c612812: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612814; /* fmov.s fr3,@-r8 */
L8c612814: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612816; /* fmov.s @(r0,r4),fr3 */
L8c612816: r[0]=16u;goto L8c612818; /* mov #0x10,r0 */
L8c612818: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61281a; /* fmov.s fr3,@-r8 */
L8c61281a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61281c; /* fmov.s @(r0,r4),fr3 */
L8c61281c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61281e; /* fmov.s fr3,@-r8 */
L8c61281e: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c612820; /* mov.l r3,@-r8 */
L8c612820: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612822; /* pref @r8 */
L8c612822: r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c612824; /* mov.l @r7,r2 */
L8c612824: r[8]+=32u;goto L8c612826; /* add #0x20,r8 */
L8c612826: r[2]+=3u;goto L8c612946; /* bra 0x8c612946 */
L8c612858: r[3]=r[6];goto L8c61285a; /* mov r6,r3 */
L8c61285a: r[8]+=28u;goto L8c61285c; /* add #0x1c,r8 */
L8c61285c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61285e; /* fmov.s fr5,@-r8 */
L8c61285e: r[14]=r[5];goto L8c612860; /* mov r5,r14 */
L8c612860: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c612862; /* mov.l r3,@r15 */
L8c612862: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c612864; /* mov.w @(0x2,r3),r0 */
L8c612864: r[3]=r[0];goto L8c612866; /* mov r0,r3 */
L8c612866: *fpul=r[3];goto L8c612868; /* lds r3,FPUL */
L8c612868: r[0]=24u;goto L8c61286a; /* mov #0x18,r0 */
L8c61286a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61286c; /* float FPUL,fr3 */
L8c61286c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61286e; /* fmul fr4,fr3 */
L8c61286e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612870; /* fmov.s fr3,@-r8 */
L8c612870: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612872; /* mov.l @r15,r3 */
L8c612872: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c612874; /* mov.w @r3,r3 */
L8c612874: *fpul=r[3];goto L8c612876; /* lds r3,FPUL */
L8c612876: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612878; /* float FPUL,fr3 */
L8c612878: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61287a; /* fmul fr4,fr3 */
L8c61287a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61287c; /* fmov.s fr3,@-r8 */
L8c61287c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61287e; /* fmov.s @(r0,r14),fr3 */
L8c61287e: r[0]=20u;goto L8c612880; /* mov #0x14,r0 */
L8c612880: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612882; /* fmov.s fr3,@-r8 */
L8c612882: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c612884; /* fmov.s @(r0,r14),fr3 */
L8c612884: r[0]=16u;goto L8c612886; /* mov #0x10,r0 */
L8c612886: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612888; /* fmov.s fr3,@-r8 */
L8c612888: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61288a; /* fmov.s @(r0,r14),fr3 */
L8c61288a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61288c; /* fmov.s fr3,@-r8 */
L8c61288c: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c61288e; /* mov.l r4,@-r8 */
L8c61288e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612890; /* pref @r8 */
L8c612890: r[3]=r[6];goto L8c612892; /* mov r6,r3 */
L8c612892: r[3]+=4u;goto L8c612894; /* add #0x4,r3 */
L8c612894: r[8]+=60u;goto L8c612896; /* add #0x3c,r8 */
L8c612896: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612898; /* fmov.s fr5,@-r8 */
L8c612898: r[14]+=32u;goto L8c61289a; /* add #0x20,r14 */
L8c61289a: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61289c; /* mov.l r3,@r15 */
L8c61289c: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61289e; /* mov.w @(0x2,r3),r0 */
L8c61289e: r[3]=r[0];goto L8c6128a0; /* mov r0,r3 */
L8c6128a0: *fpul=r[3];goto L8c6128a2; /* lds r3,FPUL */
L8c6128a2: r[0]=24u;goto L8c6128a4; /* mov #0x18,r0 */
L8c6128a4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6128a6; /* float FPUL,fr3 */
L8c6128a6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6128a8; /* fmul fr4,fr3 */
L8c6128a8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128aa; /* fmov.s fr3,@-r8 */
L8c6128aa: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6128ac; /* mov.l @r15,r3 */
L8c6128ac: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6128ae; /* mov.w @r3,r3 */
L8c6128ae: *fpul=r[3];goto L8c6128b0; /* lds r3,FPUL */
L8c6128b0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6128b2; /* float FPUL,fr3 */
L8c6128b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6128b4; /* fmul fr4,fr3 */
L8c6128b4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128b6; /* fmov.s fr3,@-r8 */
L8c6128b6: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6128b8; /* fmov.s @(r0,r14),fr3 */
L8c6128b8: r[0]=20u;goto L8c6128ba; /* mov #0x14,r0 */
L8c6128ba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128bc; /* fmov.s fr3,@-r8 */
L8c6128bc: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6128be; /* fmov.s @(r0,r14),fr3 */
L8c6128be: r[0]=16u;goto L8c6128c0; /* mov #0x10,r0 */
L8c6128c0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128c2; /* fmov.s fr3,@-r8 */
L8c6128c2: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6128c4; /* fmov.s @(r0,r14),fr3 */
L8c6128c4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128c6; /* fmov.s fr3,@-r8 */
L8c6128c6: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c6128c8; /* mov.l r4,@-r8 */
L8c6128c8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6128ca; /* pref @r8 */
L8c6128ca: r[3]=r[6];goto L8c6128cc; /* mov r6,r3 */
L8c6128cc: r[3]+=12u;goto L8c6128ce; /* add #0xc,r3 */
L8c6128ce: r[8]+=60u;goto L8c6128d0; /* add #0x3c,r8 */
L8c6128d0: r[14]=r[5];goto L8c6128d2; /* mov r5,r14 */
L8c6128d2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6128d4; /* fmov.s fr5,@-r8 */
L8c6128d4: r[14]+=96u;goto L8c6128d6; /* add #0x60,r14 */
L8c6128d6: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6128d8; /* mov.l r3,@r15 */
L8c6128d8: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6128da; /* mov.w @(0x2,r3),r0 */
L8c6128da: r[3]=r[0];goto L8c6128dc; /* mov r0,r3 */
L8c6128dc: *fpul=r[3];goto L8c6128de; /* lds r3,FPUL */
L8c6128de: r[0]=24u;goto L8c6128e0; /* mov #0x18,r0 */
L8c6128e0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6128e2; /* float FPUL,fr3 */
L8c6128e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6128e4; /* fmul fr4,fr3 */
L8c6128e4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128e6; /* fmov.s fr3,@-r8 */
L8c6128e6: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6128e8; /* mov.l @r15,r3 */
L8c6128e8: r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6128ea; /* mov.w @r3,r3 */
L8c6128ea: *fpul=r[3];goto L8c6128ec; /* lds r3,FPUL */
L8c6128ec: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6128ee; /* float FPUL,fr3 */
L8c6128ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6128f0; /* fmul fr4,fr3 */
L8c6128f0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128f2; /* fmov.s fr3,@-r8 */
L8c6128f2: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6128f4; /* fmov.s @(r0,r14),fr3 */
L8c6128f4: r[0]=20u;goto L8c6128f6; /* mov #0x14,r0 */
L8c6128f6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128f8; /* fmov.s fr3,@-r8 */
L8c6128f8: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6128fa; /* fmov.s @(r0,r14),fr3 */
L8c6128fa: r[0]=16u;goto L8c6128fc; /* mov #0x10,r0 */
L8c6128fc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128fe; /* fmov.s fr3,@-r8 */
L8c6128fe: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c612900; /* fmov.s @(r0,r14),fr3 */
L8c612900: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612902; /* fmov.s fr3,@-r8 */
L8c612902: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612904; /* mov.l r4,@-r8 */
L8c612904: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612906; /* pref @r8 */
L8c612906: r[4]=r[6];goto L8c612908; /* mov r6,r4 */
L8c612908: r[4]+=8u;goto L8c61290a; /* add #0x8,r4 */
L8c61290a: r[8]+=60u;goto L8c61290c; /* add #0x3c,r8 */
L8c61290c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61290e; /* fmov.s fr5,@-r8 */
L8c61290e: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c612910; /* mov.w @(0x2,r4),r0 */
L8c612910: r[3]=r[0];goto L8c612912; /* mov r0,r3 */
L8c612912: *fpul=r[3];goto L8c612914; /* lds r3,FPUL */
L8c612914: r[0]=24u;goto L8c612916; /* mov #0x18,r0 */
L8c612916: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612918; /* float FPUL,fr3 */
L8c612918: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61291a; /* fmul fr4,fr3 */
L8c61291a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61291c; /* fmov.s fr3,@-r8 */
L8c61291c: r[3]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61291e; /* mov.w @r4,r3 */
L8c61291e: r[4]=r[5];goto L8c612920; /* mov r5,r4 */
L8c612920: r[4]+=64u;goto L8c612922; /* add #0x40,r4 */
L8c612922: *fpul=r[3];goto L8c612924; /* lds r3,FPUL */
L8c612924: r[3]=rd(m,bus,&io_status,0x8c612a60u,4);if(io_status)return io_status;goto L8c612926; /* mov.l 0x8c612a60,r3 */
L8c612926: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612928; /* float FPUL,fr3 */
L8c612928: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61292a; /* fmul fr4,fr3 */
L8c61292a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61292c; /* fmov.s fr3,@-r8 */
L8c61292c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61292e; /* fmov.s @(r0,r4),fr3 */
L8c61292e: r[0]=20u;goto L8c612930; /* mov #0x14,r0 */
L8c612930: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612932; /* fmov.s fr3,@-r8 */
L8c612932: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612934; /* fmov.s @(r0,r4),fr3 */
L8c612934: r[0]=16u;goto L8c612936; /* mov #0x10,r0 */
L8c612936: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612938; /* fmov.s fr3,@-r8 */
L8c612938: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61293a; /* fmov.s @(r0,r4),fr3 */
L8c61293a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61293c; /* fmov.s fr3,@-r8 */
L8c61293c: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61293e; /* mov.l r3,@-r8 */
L8c61293e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612940; /* pref @r8 */
L8c612940: r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c612942; /* mov.l @r7,r2 */
L8c612942: r[8]+=32u;goto L8c612944; /* add #0x20,r8 */
L8c612944: r[2]+=4u;goto L8c612946; /* add #0x4,r2 */
L8c612946: wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c612948; /* mov.l r2,@r7 */
L8c612948: r[15]+=68u;goto L8c61294a; /* add #0x44,r15 */
L8c61294a: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61294c; /* fmov.s @r15+,fr12 */
L8c61294c: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61294e; /* mov.l @r15+,r9 */
L8c61294e: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612950; /* mov.l @r15+,r10 */
L8c612950: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612952; /* mov.l @r15+,r11 */
L8c612952: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612954; /* mov.l @r15+,r12 */
L8c612954: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612956; /* mov.l @r15+,r13 */
L8c612956: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
