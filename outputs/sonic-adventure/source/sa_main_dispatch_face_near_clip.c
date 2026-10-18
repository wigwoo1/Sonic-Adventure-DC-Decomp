/* Face-colour near-plane clipper with portable queue interface; name inferred.
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
SA_STAGE_EXPORT int sa_face_triangle_near_clip_8c6178c6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6178c6;
L8c6178c6: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6178c8; /* mov.l r14,@-r15 */
L8c6178c8: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6178ca; /* mov.l r13,@-r15 */
L8c6178ca: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6178cc; /* mov.l r12,@-r15 */
L8c6178cc: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6178ce; /* mov.l r11,@-r15 */
L8c6178ce: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6178d0; /* mov.l r10,@-r15 */
L8c6178d0: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6178d2; /* mov.l r9,@-r15 */
L8c6178d2: r[15]+=4294967236u;goto L8c6178d4; /* add #-0x3c,r15 */
L8c6178d4: r[3]=rd(m,bus,&io_status,0x8c617958u,4);if(io_status)return io_status;goto L8c6178d6; /* mov.l 0x8c617958,r3 */
L8c6178d6: r[14]=r[15];goto L8c6178d8; /* mov r15,r14 */
L8c6178d8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c6178da; /* fldi1 fr6 */
L8c6178da: r[11]=r[15];goto L8c6178dc; /* mov r15,r11 */
L8c6178dc: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6178de; /* fmov.s @r3,fr3 */
L8c6178de: r[14]+=48u;goto L8c6178e0; /* add #0x30,r14 */
L8c6178e0: fcopy(f,4u,6u);goto L8c6178e2; /* fmov fr6,fr4 */
L8c6178e2: r[11]+=12u;goto L8c6178e4; /* add #0xc,r11 */
L8c6178e4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6178e6; /* fdiv fr3,fr4 */
L8c6178e6: r[2]=r[14];goto L8c6178e8; /* mov r14,r2 */
L8c6178e8: r[10]=3u;goto L8c6178ea; /* mov #0x3,r10 */
L8c6178ea: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6178ec; /* mov.l r2,@r15 */
L8c6178ec: wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c6178ee; /* mov.l r4,@r2 */
L8c6178ee: r[4]=1u;goto L8c6178f0; /* mov #0x1,r4 */
L8c6178f0: wr(m,bus,&io_status,r[14]+0x4u,r[5],4);if(io_status)return io_status;goto L8c6178f2; /* mov.l r5,@(0x4,r14) */
L8c6178f2: r[5]=r[11];goto L8c6178f4; /* mov r11,r5 */
L8c6178f4: wr(m,bus,&io_status,r[14]+0x8u,r[6],4);if(io_status)return io_status;goto L8c6178f6; /* mov.l r6,@(0x8,r14) */
L8c6178f6: r[12]=r[5];goto L8c6178f8; /* mov r5,r12 */
L8c6178f8: r[6]=rd(m,bus,&io_status,0x8c61795cu,4);if(io_status)return io_status;goto L8c6178fa; /* mov.l 0x8c61795c,r6 */
L8c6178fa: r[9]=0u;goto L8c6178fc; /* mov #0x0,r9 */
L8c6178fc: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6178fe; /* mov.l @r15,r13 */
L8c6178fe: wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c617900; /* mov.l r5,@r15 */
L8c617900: r[0]=r[9];goto L8c617902; /* mov r9,r0 */
L8c617902: r[0]<<=2;goto L8c617904; /* shll2 r0 */
L8c617904: r[3]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c617906; /* mov.l @(r0,r14),r3 */
L8c617906: r[2]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c617908; /* mov.l @(0xc,r3),r2 */
L8c617908: *sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c61790a; /* tst r4,r2 */
L8c61790a: if(!(*sr&1u))goto L8c617968;goto L8c61790c; /* bf 0x8c617968 */
L8c61790c: r[1]=r[9];goto L8c61790e; /* mov r9,r1 */
L8c61790e: r[1]<<=2;goto L8c617910; /* shll2 r1 */
L8c617910: r[1]+=r[14];goto L8c617912; /* add r14,r1 */
L8c617912: fcopy(f,5u,6u);goto L8c617914; /* fmov fr6,fr5 */
L8c617914: r[0]=24u;goto L8c617916; /* mov #0x18,r0 */
L8c617916: wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c617918; /* mov.l r1,@(0x8,r15) */
L8c617918: r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61791a; /* mov.l @r1,r3 */
L8c61791a: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61791c; /* mov.l r3,@(0x4,r15) */
L8c61791c: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61791e; /* fmov.s @(r0,r3),fr3 */
L8c61791e: r[0]=12u;goto L8c617920; /* mov #0xc,r0 */
L8c617920: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c617922; /* fdiv fr3,fr5 */
L8c617922: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c617924; /* fmov.s @(r0,r6),fr3 */
L8c617924: r[0]=16u;goto L8c617926; /* mov #0x10,r0 */
L8c617926: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c617928; /* fmov.s @(r0,r3),fr2 */
L8c617928: r[0]=16u;goto L8c61792a; /* mov #0x10,r0 */
L8c61792a: r[3]=rd(m,bus,&io_status,0x8c617960u,4);if(io_status)return io_status;goto L8c61792c; /* mov.l 0x8c617960,r3 */
L8c61792c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61792e; /* fsub fr3,fr2 */
L8c61792e: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c617930; /* fmov.s @r3,fr3 */
L8c617930: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617932; /* fmul fr5,fr2 */
L8c617932: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617934; /* fdiv fr3,fr2 */
L8c617934: fstore(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c617936; /* fmov.s fr2,@r12 */
L8c617936: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c617938; /* mov.l @(0x8,r15),r2 */
L8c617938: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61793a; /* fmov.s @(r0,r6),fr3 */
L8c61793a: r[0]=20u;goto L8c61793c; /* mov #0x14,r0 */
L8c61793c: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61793e; /* mov.l @r2,r1 */
L8c61793e: r[2]=rd(m,bus,&io_status,0x8c617964u,4);if(io_status)return io_status;goto L8c617940; /* mov.l 0x8c617964,r2 */
L8c617940: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c617942; /* fmov.s @(r0,r1),fr2 */
L8c617942: r[0]=4u;goto L8c617944; /* mov #0x4,r0 */
L8c617944: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c617946; /* fsub fr3,fr2 */
L8c617946: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c617948; /* fmov.s @r2,fr3 */
L8c617948: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61794a; /* fmul fr5,fr2 */
L8c61794a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61794c; /* fdiv fr3,fr2 */
L8c61794c: fstore(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61794e; /* fmov fr2,@(r0,r12) */
L8c61794e: r[0]=8u;goto L8c617950; /* mov #0x8,r0 */
L8c617950: fstore(m,bus,&io_status,f,5u,r[0]+r[12]);if(io_status)return io_status;goto L8c617984; /* bra 0x8c617984 */
L8c617968: r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61796a; /* mov.l @r13,r3 */
L8c61796a: r[0]=16u;goto L8c61796c; /* mov #0x10,r0 */
L8c61796c: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61796e; /* fmov.s @(r0,r3),fr3 */
L8c61796e: r[0]=20u;goto L8c617970; /* mov #0x14,r0 */
L8c617970: fstore(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c617972; /* fmov.s fr3,@r5 */
L8c617972: r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c617974; /* mov.l @r13,r3 */
L8c617974: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c617976; /* fmov.s @(r0,r3),fr3 */
L8c617976: r[0]=4u;goto L8c617978; /* mov #0x4,r0 */
L8c617978: fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61797a; /* fmov fr3,@(r0,r5) */
L8c61797a: r[0]=24u;goto L8c61797c; /* mov #0x18,r0 */
L8c61797c: r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61797e; /* mov.l @r13,r3 */
L8c61797e: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c617980; /* fmov.s @(r0,r3),fr3 */
L8c617980: r[0]=8u;goto L8c617982; /* mov #0x8,r0 */
L8c617982: fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c617984; /* fmov fr3,@(r0,r5) */
L8c617984: r[9]+=1u;goto L8c617986; /* add #0x1,r9 */
L8c617986: *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[10]);goto L8c617988; /* cmp/ge r10,r9 */
L8c617988: r[5]+=12u;goto L8c61798a; /* add #0xc,r5 */
L8c61798a: r[13]+=4u;goto L8c61798c; /* add #0x4,r13 */
L8c61798c: take=!(*sr&1u);r[12]+=12u;if(take)goto L8c617900;goto L8c617990; /* bf/s 0x8c617900 */
L8c617990: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617992; /* mov.l @r15,r2 */
L8c617992: r[0]=8u;goto L8c617994; /* mov #0x8,r0 */
L8c617994: r[5]=r[11];goto L8c617996; /* mov r11,r5 */
L8c617996: r[5]+=12u;goto L8c617998; /* add #0xc,r5 */
L8c617998: fload(m,bus,&io_status,f,8u,r[0]+r[2]);if(io_status)return io_status;goto L8c61799a; /* fmov.s @(r0,r2),fr8 */
L8c61799a: r[0]=4u;goto L8c61799c; /* mov #0x4,r0 */
L8c61799c: r[3]=r[2];goto L8c61799e; /* mov r2,r3 */
L8c61799e: fload(m,bus,&io_status,f,9u,r[0]+r[5]);if(io_status)return io_status;goto L8c6179a0; /* fmov.s @(r0,r5),fr9 */
L8c6179a0: fload(m,bus,&io_status,f,5u,r[0]+r[3]);if(io_status)return io_status;goto L8c6179a2; /* fmov.s @(r0,r3),fr5 */
L8c6179a2: r[0]=8u;goto L8c6179a4; /* mov #0x8,r0 */
L8c6179a4: fload(m,bus,&io_status,f,10u,r[0]+r[5]);if(io_status)return io_status;goto L8c6179a6; /* fmov.s @(r0,r5),fr10 */
L8c6179a6: r[13]=r[11];goto L8c6179a8; /* mov r11,r13 */
L8c6179a8: fcopy(f,3u,5u);goto L8c6179aa; /* fmov fr5,fr3 */
L8c6179aa: r[0]=4u;goto L8c6179ac; /* mov #0x4,r0 */
L8c6179ac: fcopy(f,0u,8u);goto L8c6179ae; /* fmov fr8,fr0 */
L8c6179ae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[10],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6179b0; /* fmul fr10,fr3 */
L8c6179b0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[9],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6179b2; /* fmul fr9,fr0 */
L8c6179b2: fload(m,bus,&io_status,f,6u,r[3]);if(io_status)return io_status;goto L8c6179b4; /* fmov.s @r3,fr6 */
L8c6179b4: fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;goto L8c6179b6; /* fmov.s @r5,fr7 */
L8c6179b6: r[13]+=24u;goto L8c6179b8; /* add #0x18,r13 */
L8c6179b8: fcopy(f,2u,8u);goto L8c6179ba; /* fmov fr8,fr2 */
L8c6179ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6179bc; /* fmul fr7,fr2 */
L8c6179bc: fcopy(f,1u,7u);goto L8c6179be; /* fmov fr7,fr1 */
L8c6179be: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6179c0; /* fsub fr3,fr0 */
L8c6179c0: fcopy(f,3u,6u);goto L8c6179c2; /* fmov fr6,fr3 */
L8c6179c2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[10],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6179c4; /* fmul fr10,fr3 */
L8c6179c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[5],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6179c6; /* fmul fr5,fr1 */
L8c6179c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6179c8; /* fsub fr2,fr3 */
L8c6179c8: fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c6179ca; /* fmov.s @(r0,r13),fr2 */
L8c6179ca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6179cc; /* fmul fr3,fr2 */
L8c6179cc: fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c6179ce; /* fmov.s @r13,fr3 */
L8c6179ce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6179d0; /* fmac fr0,fr3,fr2 */
L8c6179d0: fcopy(f,11u,6u);goto L8c6179d2; /* fmov fr6,fr11 */
L8c6179d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[9],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6179d4; /* fmul fr9,fr11 */
L8c6179d4: r[0]=8u;goto L8c6179d6; /* mov #0x8,r0 */
L8c6179d6: r[5]=rd(m,bus,&io_status,0x8c617c14u,4);if(io_status)return io_status;goto L8c6179d8; /* mov.l 0x8c617c14,r5 */
L8c6179d8: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6179da; /* fmov.s @(r0,r13),fr3 */
L8c6179da: r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c6179dc; /* mov.l @r5,r3 */
L8c6179dc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[11],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6179de; /* fsub fr11,fr1 */
L8c6179de: fcopy(f,0u,1u);goto L8c6179e0; /* fmov fr1,fr0 */
L8c6179e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6179e2; /* fmac fr0,fr3,fr2 */
L8c6179e2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6179e4; /* fldi0 fr3 */
L8c6179e4: fcopy(f,5u,2u);goto L8c6179e6; /* fmov fr2,fr5 */
L8c6179e6: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c6179e8; /* fcmp/gt fr5,fr3 */
L8c6179e8: r[0]=*sr&1u;goto L8c6179ea; /* movt r0 */
L8c6179ea: *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c6179ec; /* tst r3,r0 */
L8c6179ec: if((*sr&1u))goto L8c6179f2;goto L8c6179ee; /* bt 0x8c6179f2 */
L8c6179ee: goto L8c617bfe; /* bra 0x8c617bfe */
L8c6179f2: r[1]=0u;goto L8c6179f4; /* mov #0x0,r1 */
L8c6179f4: wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c6179f6; /* mov.l r1,@r5 */
L8c6179f6: r[13]=r[1];goto L8c6179f8; /* mov r1,r13 */
L8c6179f8: r[5]=rd(m,bus,&io_status,0x8c617c18u,4);if(io_status)return io_status;goto L8c6179fa; /* mov.l 0x8c617c18,r5 */
L8c6179fa: r[12]=r[1];goto L8c6179fc; /* mov r1,r12 */
L8c6179fc: r[9]=2u;goto L8c6179fe; /* mov #0x2,r9 */
L8c6179fe: r[3]=r[13];goto L8c617a00; /* mov r13,r3 */
L8c617a00: r[3]<<=2;goto L8c617a02; /* shll2 r3 */
L8c617a02: r[0]=r[9];goto L8c617a04; /* mov r9,r0 */
L8c617a04: r[0]<<=2;goto L8c617a06; /* shll2 r0 */
L8c617a06: r[3]+=r[14];goto L8c617a08; /* add r14,r3 */
L8c617a08: r[0]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c617a0a; /* mov.l @(r0,r14),r0 */
L8c617a0a: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c617a0c; /* mov.l @r3,r2 */
L8c617a0c: r[0]=rd(m,bus,&io_status,r[0]+0xcu,4);if(io_status)return io_status;goto L8c617a0e; /* mov.l @(0xc,r0),r0 */
L8c617a0e: if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[1]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c617a10; /* mov.l @(0xc,r2),r1 */
L8c617a10: r[0]&=r[4];goto L8c617a12; /* and r4,r0 */
L8c617a12: r[1]&=r[4];goto L8c617a14; /* and r4,r1 */
L8c617a14: r[0]^=r[1];goto L8c617a16; /* xor r1,r0 */
L8c617a16: *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c617a18; /* cmp/eq #0x1,r0 */
L8c617a18: if(!(*sr&1u))goto L8c617aa2;goto L8c617a1a; /* bf 0x8c617aa2 */
L8c617a1a: r[3]=r[9];goto L8c617a1c; /* mov r9,r3 */
L8c617a1c: *sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c617a1e; /* shll r9 */
L8c617a1e: r[2]=r[13];goto L8c617a20; /* mov r13,r2 */
L8c617a20: r[9]+=r[3];goto L8c617a22; /* add r3,r9 */
L8c617a22: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c617a24; /* shll r2 */
L8c617a24: r[1]=r[13];goto L8c617a26; /* mov r13,r1 */
L8c617a26: r[9]<<=2;goto L8c617a28; /* shll2 r9 */
L8c617a28: r[3]=rd(m,bus,&io_status,0x8c617c1cu,4);if(io_status)return io_status;goto L8c617a2a; /* mov.l 0x8c617c1c,r3 */
L8c617a2a: r[9]+=r[11];goto L8c617a2c; /* add r11,r9 */
L8c617a2c: r[0]=8u;goto L8c617a2e; /* mov #0x8,r0 */
L8c617a2e: wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c617a30; /* mov.l r9,@r15 */
L8c617a30: r[2]+=r[1];goto L8c617a32; /* add r1,r2 */
L8c617a32: fload(m,bus,&io_status,f,5u,r[0]+r[9]);if(io_status)return io_status;goto L8c617a34; /* fmov.s @(r0,r9),fr5 */
L8c617a34: r[2]<<=2;goto L8c617a36; /* shll2 r2 */
L8c617a36: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c617a38; /* fmov.s @r3,fr3 */
L8c617a38: r[2]+=r[11];goto L8c617a3a; /* add r11,r2 */
L8c617a3a: r[0]=8u;goto L8c617a3c; /* mov #0x8,r0 */
L8c617a3c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617a3e; /* fsub fr5,fr3 */
L8c617a3e: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c617a40; /* mov.l r2,@(0x4,r15) */
L8c617a40: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c617a42; /* fmov.s @(r0,r2),fr2 */
L8c617a42: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c617a44; /* mov.l @(0x4,r15),r1 */
L8c617a44: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c617a46; /* fsub fr5,fr2 */
L8c617a46: fcopy(f,5u,3u);goto L8c617a48; /* fmov fr3,fr5 */
L8c617a48: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617a4a; /* mov.l @r15,r2 */
L8c617a4a: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c617a4c; /* fmov.s @r1,fr3 */
L8c617a4c: fload(m,bus,&io_status,f,6u,r[2]);if(io_status)return io_status;goto L8c617a4e; /* fmov.s @r2,fr6 */
L8c617a4e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[2],&f->fpscr,&fr[5]);if(status)return status;}goto L8c617a50; /* fdiv fr2,fr5 */
L8c617a50: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617a52; /* fsub fr6,fr3 */
L8c617a52: fcopy(f,2u,6u);goto L8c617a54; /* fmov fr6,fr2 */
L8c617a54: fcopy(f,0u,5u);goto L8c617a56; /* fmov fr5,fr0 */
L8c617a56: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617a58; /* fmac fr0,fr3,fr2 */
L8c617a58: fcopy(f,6u,2u);goto L8c617a5a; /* fmov fr2,fr6 */
L8c617a5a: r[2]=rd(m,bus,&io_status,0x8c617c20u,4);if(io_status)return io_status;goto L8c617a5c; /* mov.l 0x8c617c20,r2 */
L8c617a5c: r[0]=12u;goto L8c617a5e; /* mov #0xc,r0 */
L8c617a5e: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c617a60; /* fmov.s @(r0,r6),fr2 */
L8c617a60: r[9]=r[12];goto L8c617a62; /* mov r12,r9 */
L8c617a62: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c617a64; /* fmov.s @r2,fr3 */
L8c617a64: r[9]<<=2;goto L8c617a66; /* shll2 r9 */
L8c617a66: fcopy(f,0u,4u);goto L8c617a68; /* fmov fr4,fr0 */
L8c617a68: r[9]<<=2;goto L8c617a6a; /* shll2 r9 */
L8c617a6a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617a6c; /* fmul fr6,fr3 */
L8c617a6c: *sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c617a6e; /* shll r9 */
L8c617a6e: r[0]=16u;goto L8c617a70; /* mov #0x10,r0 */
L8c617a70: r[9]+=r[5];goto L8c617a72; /* add r5,r9 */
L8c617a72: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617a74; /* fmac fr0,fr3,fr2 */
L8c617a74: fcopy(f,0u,5u);goto L8c617a76; /* fmov fr5,fr0 */
L8c617a76: fstore(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c617a78; /* fmov fr2,@(r0,r9) */
L8c617a78: r[0]=4u;goto L8c617a7a; /* mov #0x4,r0 */
L8c617a7a: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617a7c; /* mov.l @r15,r1 */
L8c617a7c: fload(m,bus,&io_status,f,6u,r[0]+r[1]);if(io_status)return io_status;goto L8c617a7e; /* fmov.s @(r0,r1),fr6 */
L8c617a7e: r[0]=4u;goto L8c617a80; /* mov #0x4,r0 */
L8c617a80: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c617a82; /* mov.l @(0x4,r15),r1 */
L8c617a82: fcopy(f,5u,6u);goto L8c617a84; /* fmov fr6,fr5 */
L8c617a84: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c617a86; /* fmov.s @(r0,r1),fr3 */
L8c617a86: r[0]=16u;goto L8c617a88; /* mov #0x10,r0 */
L8c617a88: r[1]=rd(m,bus,&io_status,0x8c617c24u,4);if(io_status)return io_status;goto L8c617a8a; /* mov.l 0x8c617c24,r1 */
L8c617a8a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c617a8c; /* fsub fr6,fr3 */
L8c617a8c: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c617a8e; /* fmov.s @(r0,r6),fr2 */
L8c617a8e: r[0]=20u;goto L8c617a90; /* mov #0x14,r0 */
L8c617a90: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[5],&f->fpscr,&fr[5]);if(status)return status;}goto L8c617a92; /* fmac fr0,fr3,fr5 */
L8c617a92: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c617a94; /* fmov.s @r1,fr3 */
L8c617a94: fcopy(f,0u,4u);goto L8c617a96; /* fmov fr4,fr0 */
L8c617a96: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c617a98; /* fmul fr5,fr3 */
L8c617a98: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c617a9a; /* fmac fr0,fr3,fr2 */
L8c617a9a: fstore(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c617a9c; /* fmov fr2,@(r0,r9) */
L8c617a9c: r[0]=24u;goto L8c617a9e; /* mov #0x18,r0 */
L8c617a9e: r[12]+=1u;goto L8c617aa0; /* add #0x1,r12 */
L8c617aa0: fstore(m,bus,&io_status,f,0u,r[0]+r[9]);if(io_status)return io_status;goto L8c617aa2; /* fmov fr0,@(r0,r9) */
L8c617aa2: r[9]=r[13];goto L8c617aa4; /* mov r13,r9 */
L8c617aa4: r[9]<<=2;goto L8c617aa6; /* shll2 r9 */
L8c617aa6: r[9]+=r[14];goto L8c617aa8; /* add r14,r9 */
L8c617aa8: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c617aaa; /* mov.l @r9,r3 */
L8c617aaa: r[2]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c617aac; /* mov.l @(0xc,r3),r2 */
L8c617aac: *sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c617aae; /* tst r4,r2 */
L8c617aae: if(!(*sr&1u))goto L8c617ada;goto L8c617ab0; /* bf 0x8c617ada */
L8c617ab0: r[1]=r[12];goto L8c617ab2; /* mov r12,r1 */
L8c617ab2: r[1]<<=2;goto L8c617ab4; /* shll2 r1 */
L8c617ab4: r[0]=16u;goto L8c617ab6; /* mov #0x10,r0 */
L8c617ab6: r[1]<<=2;goto L8c617ab8; /* shll2 r1 */
L8c617ab8: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c617aba; /* shll r1 */
L8c617aba: r[1]+=r[5];goto L8c617abc; /* add r5,r1 */
L8c617abc: r[12]+=1u;goto L8c617abe; /* add #0x1,r12 */
L8c617abe: wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c617ac0; /* mov.l r1,@r15 */
L8c617ac0: r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c617ac2; /* mov.l @r9,r3 */
L8c617ac2: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c617ac4; /* fmov.s @(r0,r3),fr3 */
L8c617ac4: fstore(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c617ac6; /* fmov fr3,@(r0,r1) */
L8c617ac6: r[0]=20u;goto L8c617ac8; /* mov #0x14,r0 */
L8c617ac8: r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c617aca; /* mov.l @r9,r2 */
L8c617aca: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617acc; /* mov.l @r15,r3 */
L8c617acc: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c617ace; /* fmov.s @(r0,r2),fr3 */
L8c617ace: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c617ad0; /* fmov fr3,@(r0,r3) */
L8c617ad0: r[0]=24u;goto L8c617ad2; /* mov #0x18,r0 */
L8c617ad2: r[2]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c617ad4; /* mov.l @r9,r2 */
L8c617ad4: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c617ad6; /* mov.l @r15,r3 */
L8c617ad6: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c617ad8; /* fmov.s @(r0,r2),fr3 */
L8c617ad8: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c617ada; /* fmov fr3,@(r0,r3) */
L8c617ada: r[9]=r[13];goto L8c617adc; /* mov r13,r9 */
L8c617adc: r[13]+=1u;goto L8c617ade; /* add #0x1,r13 */
L8c617ade: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[10]);goto L8c617ae0; /* cmp/ge r10,r13 */
L8c617ae0: if(!(*sr&1u))goto L8c6179fe;goto L8c617ae2; /* bf 0x8c6179fe */
L8c617ae2: r[0]=r[12];goto L8c617ae4; /* mov r12,r0 */
L8c617ae4: r[4]=rd(m,bus,&io_status,0x8c617c2cu,4);if(io_status)return io_status;goto L8c617ae6; /* mov.l 0x8c617c2c,r4 */
L8c617ae6: r[6]=rd(m,bus,&io_status,0x8c617c28u,4);if(io_status)return io_status;goto L8c617ae8; /* mov.l 0x8c617c28,r6 */
L8c617ae8: *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c617aea; /* cmp/eq #0x3,r0 */
L8c617aea: if(!(*sr&1u))goto L8c617b5c;goto L8c617aec; /* bf 0x8c617b5c */
L8c617aec: r[0]=24u;goto L8c617aee; /* mov #0x18,r0 */
L8c617aee: r[14]=r[5];goto L8c617af0; /* mov r5,r14 */
L8c617af0: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617af2; /* fmov.s @(r0,r14),fr3 */
L8c617af2: r[8]+=16u;goto L8c617af4; /* add #0x10,r8 */
L8c617af4: r[0]=20u;goto L8c617af6; /* mov #0x14,r0 */
L8c617af6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617af8; /* fmov.s fr3,@-r8 */
L8c617af8: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617afa; /* fmov.s @(r0,r14),fr3 */
L8c617afa: r[0]=16u;goto L8c617afc; /* mov #0x10,r0 */
L8c617afc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617afe; /* fmov.s fr3,@-r8 */
L8c617afe: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617b00; /* fmov.s @(r0,r14),fr3 */
L8c617b00: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b02; /* fmov.s fr3,@-r8 */
L8c617b02: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c617b04; /* mov.l r4,@-r8 */
L8c617b04: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617b06; /* pref @r8 */
L8c617b06: r[0]=24u;goto L8c617b08; /* mov #0x18,r0 */
L8c617b08: r[14]+=32u;goto L8c617b0a; /* add #0x20,r14 */
L8c617b0a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617b0c; /* fmov.s @(r0,r14),fr3 */
L8c617b0c: r[8]+=48u;goto L8c617b0e; /* add #0x30,r8 */
L8c617b0e: r[0]=20u;goto L8c617b10; /* mov #0x14,r0 */
L8c617b10: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b12; /* fmov.s fr3,@-r8 */
L8c617b12: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617b14; /* fmov.s @(r0,r14),fr3 */
L8c617b14: r[0]=16u;goto L8c617b16; /* mov #0x10,r0 */
L8c617b16: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b18; /* fmov.s fr3,@-r8 */
L8c617b18: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617b1a; /* fmov.s @(r0,r14),fr3 */
L8c617b1a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b1c; /* fmov.s fr3,@-r8 */
L8c617b1c: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c617b1e; /* mov.l r4,@-r8 */
L8c617b1e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617b20; /* pref @r8 */
L8c617b20: r[0]=12u;goto L8c617b22; /* mov #0xc,r0 */
L8c617b22: r[4]=r[5];goto L8c617b24; /* mov r5,r4 */
L8c617b24: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c617b26; /* fmov.s @(r0,r7),fr3 */
L8c617b26: r[8]+=64u;goto L8c617b28; /* add #0x40,r8 */
L8c617b28: r[0]=8u;goto L8c617b2a; /* mov #0x8,r0 */
L8c617b2a: r[3]=rd(m,bus,&io_status,0x8c617c30u,4);if(io_status)return io_status;goto L8c617b2c; /* mov.l 0x8c617c30,r3 */
L8c617b2c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b2e; /* fmov.s fr3,@-r8 */
L8c617b2e: r[4]+=64u;goto L8c617b30; /* add #0x40,r4 */
L8c617b30: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c617b32; /* fmov.s @(r0,r7),fr3 */
L8c617b32: r[0]=4u;goto L8c617b34; /* mov #0x4,r0 */
L8c617b34: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b36; /* fmov.s fr3,@-r8 */
L8c617b36: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c617b38; /* fmov.s @(r0,r7),fr3 */
L8c617b38: r[0]=24u;goto L8c617b3a; /* mov #0x18,r0 */
L8c617b3a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b3c; /* fmov.s fr3,@-r8 */
L8c617b3c: fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c617b3e; /* fmov.s @r7,fr3 */
L8c617b3e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b40; /* fmov.s fr3,@-r8 */
L8c617b40: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c617b42; /* fmov.s @(r0,r4),fr3 */
L8c617b42: r[0]=20u;goto L8c617b44; /* mov #0x14,r0 */
L8c617b44: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b46; /* fmov.s fr3,@-r8 */
L8c617b46: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c617b48; /* fmov.s @(r0,r4),fr3 */
L8c617b48: r[0]=16u;goto L8c617b4a; /* mov #0x10,r0 */
L8c617b4a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b4c; /* fmov.s fr3,@-r8 */
L8c617b4c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c617b4e; /* fmov.s @(r0,r4),fr3 */
L8c617b4e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b50; /* fmov.s fr3,@-r8 */
L8c617b50: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c617b52; /* mov.l r3,@-r8 */
L8c617b52: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617b54; /* pref @r8 */
L8c617b54: r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c617b56; /* mov.l @r6,r2 */
L8c617b56: r[8]+=32u;goto L8c617b58; /* add #0x20,r8 */
L8c617b58: r[2]+=3u;goto L8c617bfc; /* bra 0x8c617bfc */
L8c617b5c: r[0]=24u;goto L8c617b5e; /* mov #0x18,r0 */
L8c617b5e: r[14]=r[5];goto L8c617b60; /* mov r5,r14 */
L8c617b60: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617b62; /* fmov.s @(r0,r14),fr3 */
L8c617b62: r[8]+=16u;goto L8c617b64; /* add #0x10,r8 */
L8c617b64: r[0]=20u;goto L8c617b66; /* mov #0x14,r0 */
L8c617b66: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b68; /* fmov.s fr3,@-r8 */
L8c617b68: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617b6a; /* fmov.s @(r0,r14),fr3 */
L8c617b6a: r[0]=16u;goto L8c617b6c; /* mov #0x10,r0 */
L8c617b6c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b6e; /* fmov.s fr3,@-r8 */
L8c617b6e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617b70; /* fmov.s @(r0,r14),fr3 */
L8c617b70: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b72; /* fmov.s fr3,@-r8 */
L8c617b72: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c617b74; /* mov.l r4,@-r8 */
L8c617b74: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617b76; /* pref @r8 */
L8c617b76: r[0]=24u;goto L8c617b78; /* mov #0x18,r0 */
L8c617b78: r[14]+=32u;goto L8c617b7a; /* add #0x20,r14 */
L8c617b7a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617b7c; /* fmov.s @(r0,r14),fr3 */
L8c617b7c: r[8]+=48u;goto L8c617b7e; /* add #0x30,r8 */
L8c617b7e: r[0]=20u;goto L8c617b80; /* mov #0x14,r0 */
L8c617b80: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b82; /* fmov.s fr3,@-r8 */
L8c617b82: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617b84; /* fmov.s @(r0,r14),fr3 */
L8c617b84: r[0]=16u;goto L8c617b86; /* mov #0x10,r0 */
L8c617b86: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b88; /* fmov.s fr3,@-r8 */
L8c617b88: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617b8a; /* fmov.s @(r0,r14),fr3 */
L8c617b8a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b8c; /* fmov.s fr3,@-r8 */
L8c617b8c: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c617b8e; /* mov.l r4,@-r8 */
L8c617b8e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617b90; /* pref @r8 */
L8c617b90: r[0]=12u;goto L8c617b92; /* mov #0xc,r0 */
L8c617b92: r[14]=r[5];goto L8c617b94; /* mov r5,r14 */
L8c617b94: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c617b96; /* fmov.s @(r0,r7),fr3 */
L8c617b96: r[8]+=64u;goto L8c617b98; /* add #0x40,r8 */
L8c617b98: r[0]=8u;goto L8c617b9a; /* mov #0x8,r0 */
L8c617b9a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617b9c; /* fmov.s fr3,@-r8 */
L8c617b9c: r[14]+=96u;goto L8c617b9e; /* add #0x60,r14 */
L8c617b9e: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c617ba0; /* fmov.s @(r0,r7),fr3 */
L8c617ba0: r[0]=4u;goto L8c617ba2; /* mov #0x4,r0 */
L8c617ba2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617ba4; /* fmov.s fr3,@-r8 */
L8c617ba4: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c617ba6; /* fmov.s @(r0,r7),fr3 */
L8c617ba6: r[0]=24u;goto L8c617ba8; /* mov #0x18,r0 */
L8c617ba8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617baa; /* fmov.s fr3,@-r8 */
L8c617baa: fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c617bac; /* fmov.s @r7,fr3 */
L8c617bac: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617bae; /* fmov.s fr3,@-r8 */
L8c617bae: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617bb0; /* fmov.s @(r0,r14),fr3 */
L8c617bb0: r[0]=20u;goto L8c617bb2; /* mov #0x14,r0 */
L8c617bb2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617bb4; /* fmov.s fr3,@-r8 */
L8c617bb4: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617bb6; /* fmov.s @(r0,r14),fr3 */
L8c617bb6: r[0]=16u;goto L8c617bb8; /* mov #0x10,r0 */
L8c617bb8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617bba; /* fmov.s fr3,@-r8 */
L8c617bba: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c617bbc; /* fmov.s @(r0,r14),fr3 */
L8c617bbc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617bbe; /* fmov.s fr3,@-r8 */
L8c617bbe: r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c617bc0; /* mov.l r4,@-r8 */
L8c617bc0: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617bc2; /* pref @r8 */
L8c617bc2: r[0]=12u;goto L8c617bc4; /* mov #0xc,r0 */
L8c617bc4: r[4]=r[5];goto L8c617bc6; /* mov r5,r4 */
L8c617bc6: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c617bc8; /* fmov.s @(r0,r7),fr3 */
L8c617bc8: r[8]+=64u;goto L8c617bca; /* add #0x40,r8 */
L8c617bca: r[0]=8u;goto L8c617bcc; /* mov #0x8,r0 */
L8c617bcc: r[3]=rd(m,bus,&io_status,0x8c617c30u,4);if(io_status)return io_status;goto L8c617bce; /* mov.l 0x8c617c30,r3 */
L8c617bce: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617bd0; /* fmov.s fr3,@-r8 */
L8c617bd0: r[4]+=64u;goto L8c617bd2; /* add #0x40,r4 */
L8c617bd2: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c617bd4; /* fmov.s @(r0,r7),fr3 */
L8c617bd4: r[0]=4u;goto L8c617bd6; /* mov #0x4,r0 */
L8c617bd6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617bd8; /* fmov.s fr3,@-r8 */
L8c617bd8: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c617bda; /* fmov.s @(r0,r7),fr3 */
L8c617bda: r[0]=24u;goto L8c617bdc; /* mov #0x18,r0 */
L8c617bdc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617bde; /* fmov.s fr3,@-r8 */
L8c617bde: fload(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c617be0; /* fmov.s @r7,fr3 */
L8c617be0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617be2; /* fmov.s fr3,@-r8 */
L8c617be2: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c617be4; /* fmov.s @(r0,r4),fr3 */
L8c617be4: r[0]=20u;goto L8c617be6; /* mov #0x14,r0 */
L8c617be6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617be8; /* fmov.s fr3,@-r8 */
L8c617be8: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c617bea; /* fmov.s @(r0,r4),fr3 */
L8c617bea: r[0]=16u;goto L8c617bec; /* mov #0x10,r0 */
L8c617bec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617bee; /* fmov.s fr3,@-r8 */
L8c617bee: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c617bf0; /* fmov.s @(r0,r4),fr3 */
L8c617bf0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c617bf2; /* fmov.s fr3,@-r8 */
L8c617bf2: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c617bf4; /* mov.l r3,@-r8 */
L8c617bf4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c617bf6; /* pref @r8 */
L8c617bf6: r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c617bf8; /* mov.l @r6,r2 */
L8c617bf8: r[8]+=32u;goto L8c617bfa; /* add #0x20,r8 */
L8c617bfa: r[2]+=4u;goto L8c617bfc; /* add #0x4,r2 */
L8c617bfc: wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c617bfe; /* mov.l r2,@r6 */
L8c617bfe: r[15]+=60u;goto L8c617c00; /* add #0x3c,r15 */
L8c617c00: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c617c02; /* mov.l @r15+,r9 */
L8c617c02: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c617c04; /* mov.l @r15+,r10 */
L8c617c04: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c617c06; /* mov.l @r15+,r11 */
L8c617c06: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c617c08; /* mov.l @r15+,r12 */
L8c617c08: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c617c0a; /* mov.l @r15+,r13 */
L8c617c0a: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
