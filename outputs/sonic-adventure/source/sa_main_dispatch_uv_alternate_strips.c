/* UV alternate strip emitters with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_uv_alternate_strip_8c61394a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61394a;
L8c61394a: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61394c; /* mov.l r14,@-r15 */
L8c61394c: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61394e; /* mov.l r13,@-r15 */
L8c61394e: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c613950; /* mov.l r12,@-r15 */
L8c613950: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c613952; /* mov.l r11,@-r15 */
L8c613952: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c613954; /* mov.l r10,@-r15 */
L8c613954: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c613956; /* mov.l r9,@-r15 */
L8c613956: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c613958; /* fmov.s fr15,@-r15 */
L8c613958: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61395a; /* fmov.s fr14,@-r15 */
L8c61395a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61395c; /* fmov.s fr13,@-r15 */
L8c61395c: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61395e; /* sts.l PR,@-r15 */
L8c61395e: r[15]+=4294967276u;goto L8c613960; /* add #-0x14,r15 */
L8c613960: r[3]=rd(m,bus,&io_status,0x8c613994u,4);if(io_status)return io_status;goto L8c613962; /* mov.l 0x8c613994,r3 */
L8c613962: r[9]=rd(m,bus,&io_status,0x8c613990u,4);if(io_status)return io_status;goto L8c613964; /* mov.l 0x8c613990,r9 */
L8c613964: target=r[3];*pr=0x8c613968u;r[12]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c613968; /* jsr @r3 */
L8c613968: r[2]=rd(m,bus,&io_status,0x8c613998u,4);if(io_status)return io_status;goto L8c61396a; /* mov.l 0x8c613998,r2 */
L8c61396a: r[1]=0u;goto L8c61396c; /* mov #0x0,r1 */
L8c61396c: r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c61396e; /* mov.w @(0x2,r12),r0 */
L8c61396e: r[14]=rd(m,bus,&io_status,r[12]+0x14u,4);if(io_status)return io_status;goto L8c613970; /* mov.l @(0x14,r12),r14 */
L8c613970: r[13]=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;goto L8c613972; /* mov.l @(0x4,r12),r13 */
L8c613972: r[0]=r[0]&65535u;goto L8c613974; /* extu.w r0,r0 */
L8c613974: wr(m,bus,&io_status,r[15]+0x10u,r[0],4);if(io_status)return io_status;goto L8c613976; /* mov.l r0,@(0x10,r15) */
L8c613976: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c613978; /* mov.l @r2,r3 */
L8c613978: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61397a; /* mov.l r3,@(0x4,r15) */
L8c61397a: wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c61397c; /* mov.l r1,@(0x8,r15) */
L8c61397c: wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c61397e; /* mov.l r1,@(0xc,r15) */
L8c61397e: goto L8c61399c; /* bra 0x8c61399c */
L8c61399c: r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61399e; /* mov.w @r13+,r3 */
L8c61399e: r[4]=rd(m,bus,&io_status,0x8c613a90u,4);if(io_status)return io_status;goto L8c6139a0; /* mov.l 0x8c613a90,r4 */
L8c6139a0: r[3]=r[3]&65535u;goto L8c6139a2; /* extu.w r3,r3 */
L8c6139a2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6139a4; /* fldi1 fr4 */
L8c6139a4: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6139a6; /* tst r4,r3 */
L8c6139a6: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6139a8; /* mov.l r3,@r15 */
L8c6139a8: take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c6139ba;goto L8c6139ac; /* bt/s 0x8c6139ba */
L8c6139ac: r[4]=sx(m,bus,&io_status,0x8c613a8eu,2);if(io_status)return io_status;goto L8c6139ae; /* mov.w 0x8c613a8e,r4 */
L8c6139ae: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6139b0; /* mov.l @r15,r3 */
L8c6139b0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6139b2; /* fldi1 fr4 */
L8c6139b2: r[3]&=r[4];goto L8c6139b4; /* and r4,r3 */
L8c6139b4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c6139b6; /* fneg fr4 */
L8c6139b6: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6139b8; /* mov.l r3,@r15 */
L8c6139b8: fcopy(f,15u,4u);goto L8c6139ba; /* fmov fr4,fr15 */
L8c6139ba: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6139bc; /* mov.l @r15,r3 */
L8c6139bc: r[0]=16u;goto L8c6139be; /* mov #0x10,r0 */
L8c6139be: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6139c0; /* mov.l @(0xc,r15),r2 */
L8c6139c0: r[10]=r[8];goto L8c6139c2; /* mov r8,r10 */
L8c6139c2: r[2]+=r[3];goto L8c6139c4; /* add r3,r2 */
L8c6139c4: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6139c6; /* mov.l r2,@(0xc,r15) */
L8c6139c6: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6139c8; /* mov.l @r15,r1 */
L8c6139c8: r[1]+=4294967294u;goto L8c6139ca; /* add #-0x2,r1 */
L8c6139ca: wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c6139cc; /* mov.l r1,@r15 */
L8c6139cc: r[4]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6139ce; /* mov.w @r13+,r4 */
L8c6139ce: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6139d0; /* mov.l @(0x4,r15),r3 */
L8c6139d0: r[11]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6139d2; /* mov.w @r13+,r11 */
L8c6139d2: r[4]=r[4]&65535u;goto L8c6139d4; /* extu.w r4,r4 */
L8c6139d4: r[4]<<=2;goto L8c6139d6; /* shll2 r4 */
L8c6139d6: r[11]=r[11]&65535u;goto L8c6139d8; /* extu.w r11,r11 */
L8c6139d8: r[11]<<=2;goto L8c6139da; /* shll2 r11 */
L8c6139da: r[4]<<=2;goto L8c6139dc; /* shll2 r4 */
L8c6139dc: r[11]<<=2;goto L8c6139de; /* shll2 r11 */
L8c6139de: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6139e0; /* shll r4 */
L8c6139e0: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c6139e2; /* shll r11 */
L8c6139e2: r[11]+=r[3];goto L8c6139e4; /* add r3,r11 */
L8c6139e4: r[4]+=r[3];goto L8c6139e6; /* add r3,r4 */
L8c6139e6: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c6139e8; /* fmov.s @(r0,r11),fr14 */
L8c6139e8: fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c6139ea; /* fmov.s @(r0,r4),fr4 */
L8c6139ea: r[0]=20u;goto L8c6139ec; /* mov #0x14,r0 */
L8c6139ec: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c6139ee; /* fmov.s @(r0,r11),fr13 */
L8c6139ee: fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c6139f0; /* fmov.s @(r0,r4),fr5 */
L8c6139f0: r[12]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6139f2; /* mov.w @r13+,r12 */
L8c6139f2: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6139f4; /* mov.l @(0x4,r15),r3 */
L8c6139f4: r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c6139f6; /* mov.w @r13,r2 */
L8c6139f6: r[12]=r[12]&65535u;goto L8c6139f8; /* extu.w r12,r12 */
L8c6139f8: r[12]<<=2;goto L8c6139fa; /* shll2 r12 */
L8c6139fa: r[2]=r[2]&65535u;goto L8c6139fc; /* extu.w r2,r2 */
L8c6139fc: r[2]<<=2;goto L8c6139fe; /* shll2 r2 */
L8c6139fe: r[2]<<=2;goto L8c613a00; /* shll2 r2 */
L8c613a00: r[12]<<=2;goto L8c613a02; /* shll2 r12 */
L8c613a02: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c613a04; /* shll r12 */
L8c613a04: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c613a06; /* shll r2 */
L8c613a06: r[2]+=r[3];goto L8c613a08; /* add r3,r2 */
L8c613a08: r[12]+=r[3];goto L8c613a0a; /* add r3,r12 */
L8c613a0a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c613a0c; /* pref @r2 */
L8c613a0c: r[0]=20u;goto L8c613a0e; /* mov #0x14,r0 */
L8c613a0e: fcopy(f,3u,14u);goto L8c613a10; /* fmov fr14,fr3 */
L8c613a10: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c613a12; /* fmov.s @(r0,r12),fr2 */
L8c613a12: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613a14; /* fsub fr4,fr3 */
L8c613a14: r[0]=16u;goto L8c613a16; /* mov #0x10,r0 */
L8c613a16: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c613a18; /* cmp/hs r8,r10 */
L8c613a18: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613a1a; /* fsub fr13,fr2 */
L8c613a1a: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c613a1c; /* fmov.s @(r0,r12),fr1 */
L8c613a1c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c613a1e; /* fsub fr14,fr1 */
L8c613a1e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c613a20; /* fmul fr2,fr3 */
L8c613a20: fcopy(f,2u,13u);goto L8c613a22; /* fmov fr13,fr2 */
L8c613a22: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613a24; /* fsub fr5,fr2 */
L8c613a24: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613a26; /* fmul fr1,fr2 */
L8c613a26: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613a28; /* fsub fr2,fr3 */
L8c613a28: fcopy(f,4u,3u);goto L8c613a2a; /* fmov fr3,fr4 */
L8c613a2a: take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c613a94;goto L8c613a2e; /* bt/s 0x8c613a94 */
L8c613a2e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613a30; /* fldi0 fr3 */
L8c613a30: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c613a32; /* fcmp/gt fr4,fr3 */
L8c613a32: if((*sr&1u))goto L8c613ae2;goto L8c613a34; /* bt 0x8c613ae2 */
L8c613a34: r[6]=r[14];goto L8c613a36; /* mov r14,r6 */
L8c613a36: r[4]=r[11];goto L8c613a38; /* mov r11,r4 */
L8c613a38: r[5]=r[9];goto L8c613a3a; /* mov r9,r5 */
L8c613a3a: r[14]+=4u;goto L8c613a3c; /* add #0x4,r14 */
L8c613a3c: r[8]+=16u;goto L8c613a3e; /* add #0x10,r8 */
L8c613a3e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613a40; /* fschg */
L8c613a40: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613a42; /* fmov.s @r4+,fr0 */
L8c613a42: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613a44; /* fmov.s @r4+,fr2 */
L8c613a44: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613a46; /* fldi0 fr3 */
L8c613a46: fcopy(f,8u,1u);goto L8c613a48; /* fmov fr1,fr8 */
L8c613a48: fcopy(f,10u,3u);goto L8c613a4a; /* fmov fr3,fr10 */
L8c613a4a: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c613a4c; /* fipr fv8,fv0 */
L8c613a4c: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613a4e; /* fmov.s @r4+,fr4 */
L8c613a4e: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613a50; /* fmov.s @r4,fr6 */
L8c613a50: fcopy(f,8u,5u);goto L8c613a52; /* fmov fr5,fr8 */
L8c613a52: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613a54; /* fschg */
L8c613a54: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c613a56; /* mov.w @r6+,r2 */
L8c613a56: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c613a58; /* mov.w @r6,r3 */
L8c613a58: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613a5a; /* fmov.s fr6,@-r8 */
L8c613a5a: *fpul=r[3];goto L8c613a5c; /* lds r3,FPUL */
L8c613a5c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613a5e; /* fmov.s fr5,@-r8 */
L8c613a5e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c613a60; /* fsub fr3,fr11 */
L8c613a60: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613a62; /* fmov.s fr4,@-r8 */
L8c613a62: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c613a64; /* float FPUL,fr1 */
L8c613a64: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c613a66; /* fmul fr8,fr1 */
L8c613a66: *fpul=r[2];goto L8c613a68; /* lds r2,FPUL */
L8c613a68: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613a6a; /* mov.l r5,@-r8 */
L8c613a6a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c613a6c; /* float FPUL,fr2 */
L8c613a6c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613a6e; /* fmul fr8,fr2 */
L8c613a6e: r[8]+=28u;goto L8c613a70; /* add #0x1c,r8 */
L8c613a70: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c613a72; /* fmov.s fr11,@-r8 */
L8c613a72: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613a74; /* fmov.s fr1,@-r8 */
L8c613a74: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613a76; /* fmov.s fr2,@-r8 */
L8c613a76: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613a78; /* pref @r8 */
L8c613a78: r[8]+=16u;goto L8c613a7a; /* add #0x10,r8 */
L8c613a7a: r[3]=r[8];goto L8c613a7c; /* mov r8,r3 */
L8c613a7c: r[3]-=r[10];goto L8c613a7e; /* sub r10,r3 */
L8c613a7e: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613a80; /* mov.l @(0x8,r15),r2 */
L8c613a80: r[3]+=32u;goto L8c613a82; /* add #0x20,r3 */
L8c613a82: r[14]+=4294967292u;goto L8c613a84; /* add #-0x4,r14 */
L8c613a84: r[2]+=r[3];goto L8c613a86; /* add r3,r2 */
L8c613a86: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613a88; /* mov.l r2,@(0x8,r15) */
L8c613a88: r[10]=r[8];goto L8c613b28; /* bra 0x8c613b28 */
L8c613a94: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613a96; /* fldi0 fr3 */
L8c613a96: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c613a98; /* fcmp/gt fr4,fr3 */
L8c613a98: take=!(*sr&1u);r[14]+=4u;if(take)goto L8c613b28;goto L8c613a9c; /* bf/s 0x8c613b28 */
L8c613a9c: r[14]+=4294967292u;goto L8c613a9e; /* add #-0x4,r14 */
L8c613a9e: r[5]=rd(m,bus,&io_status,0x8c613c1cu,4);if(io_status)return io_status;goto L8c613aa0; /* mov.l 0x8c613c1c,r5 */
L8c613aa0: r[6]=r[14];goto L8c613aa2; /* mov r14,r6 */
L8c613aa2: r[14]+=4u;goto L8c613aa4; /* add #0x4,r14 */
L8c613aa4: r[8]+=16u;goto L8c613aa6; /* add #0x10,r8 */
L8c613aa6: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613aa8; /* fschg */
L8c613aa8: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613aaa; /* fmov.s @r4+,fr0 */
L8c613aaa: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613aac; /* fmov.s @r4+,fr2 */
L8c613aac: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613aae; /* fldi0 fr3 */
L8c613aae: fcopy(f,8u,1u);goto L8c613ab0; /* fmov fr1,fr8 */
L8c613ab0: fcopy(f,10u,3u);goto L8c613ab2; /* fmov fr3,fr10 */
L8c613ab2: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c613ab4; /* fipr fv8,fv0 */
L8c613ab4: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613ab6; /* fmov.s @r4+,fr4 */
L8c613ab6: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613ab8; /* fmov.s @r4,fr6 */
L8c613ab8: fcopy(f,8u,5u);goto L8c613aba; /* fmov fr5,fr8 */
L8c613aba: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613abc; /* fschg */
L8c613abc: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c613abe; /* mov.w @r6+,r2 */
L8c613abe: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c613ac0; /* mov.w @r6,r3 */
L8c613ac0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613ac2; /* fmov.s fr6,@-r8 */
L8c613ac2: *fpul=r[3];goto L8c613ac4; /* lds r3,FPUL */
L8c613ac4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613ac6; /* fmov.s fr5,@-r8 */
L8c613ac6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c613ac8; /* fsub fr3,fr11 */
L8c613ac8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613aca; /* fmov.s fr4,@-r8 */
L8c613aca: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c613acc; /* float FPUL,fr1 */
L8c613acc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c613ace; /* fmul fr8,fr1 */
L8c613ace: *fpul=r[2];goto L8c613ad0; /* lds r2,FPUL */
L8c613ad0: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613ad2; /* mov.l r5,@-r8 */
L8c613ad2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c613ad4; /* float FPUL,fr2 */
L8c613ad4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613ad6; /* fmul fr8,fr2 */
L8c613ad6: r[8]+=28u;goto L8c613ad8; /* add #0x1c,r8 */
L8c613ad8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c613ada; /* fmov.s fr11,@-r8 */
L8c613ada: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613adc; /* fmov.s fr1,@-r8 */
L8c613adc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613ade; /* fmov.s fr2,@-r8 */
L8c613ade: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613ae0; /* pref @r8 */
L8c613ae0: r[8]+=16u;goto L8c613ae2; /* add #0x10,r8 */
L8c613ae2: r[5]=rd(m,bus,&io_status,0x8c613c1cu,4);if(io_status)return io_status;goto L8c613ae4; /* mov.l 0x8c613c1c,r5 */
L8c613ae4: r[6]=r[14];goto L8c613ae6; /* mov r14,r6 */
L8c613ae6: r[4]=r[11];goto L8c613ae8; /* mov r11,r4 */
L8c613ae8: r[14]+=4u;goto L8c613aea; /* add #0x4,r14 */
L8c613aea: r[8]+=16u;goto L8c613aec; /* add #0x10,r8 */
L8c613aec: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613aee; /* fschg */
L8c613aee: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613af0; /* fmov.s @r4+,fr0 */
L8c613af0: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613af2; /* fmov.s @r4+,fr2 */
L8c613af2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613af4; /* fldi0 fr3 */
L8c613af4: fcopy(f,8u,1u);goto L8c613af6; /* fmov fr1,fr8 */
L8c613af6: fcopy(f,10u,3u);goto L8c613af8; /* fmov fr3,fr10 */
L8c613af8: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c613afa; /* fipr fv8,fv0 */
L8c613afa: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613afc; /* fmov.s @r4+,fr4 */
L8c613afc: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613afe; /* fmov.s @r4,fr6 */
L8c613afe: fcopy(f,8u,5u);goto L8c613b00; /* fmov fr5,fr8 */
L8c613b00: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613b02; /* fschg */
L8c613b02: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c613b04; /* mov.w @r6+,r2 */
L8c613b04: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c613b06; /* mov.w @r6,r3 */
L8c613b06: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613b08; /* fmov.s fr6,@-r8 */
L8c613b08: *fpul=r[3];goto L8c613b0a; /* lds r3,FPUL */
L8c613b0a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613b0c; /* fmov.s fr5,@-r8 */
L8c613b0c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c613b0e; /* fsub fr3,fr11 */
L8c613b0e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613b10; /* fmov.s fr4,@-r8 */
L8c613b10: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c613b12; /* float FPUL,fr1 */
L8c613b12: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c613b14; /* fmul fr8,fr1 */
L8c613b14: *fpul=r[2];goto L8c613b16; /* lds r2,FPUL */
L8c613b16: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613b18; /* mov.l r5,@-r8 */
L8c613b18: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c613b1a; /* float FPUL,fr2 */
L8c613b1a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613b1c; /* fmul fr8,fr2 */
L8c613b1c: r[8]+=28u;goto L8c613b1e; /* add #0x1c,r8 */
L8c613b1e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c613b20; /* fmov.s fr11,@-r8 */
L8c613b20: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613b22; /* fmov.s fr1,@-r8 */
L8c613b22: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613b24; /* fmov.s fr2,@-r8 */
L8c613b24: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613b26; /* pref @r8 */
L8c613b26: r[8]+=16u;goto L8c613b28; /* add #0x10,r8 */
L8c613b28: fcopy(f,4u,14u);goto L8c613b2a; /* fmov fr14,fr4 */
L8c613b2a: r[0]=16u;goto L8c613b2c; /* mov #0x10,r0 */
L8c613b2c: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613b2e; /* mov.l @r15,r3 */
L8c613b2e: r[4]=r[11];goto L8c613b30; /* mov r11,r4 */
L8c613b30: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c613b32; /* fmov.s @(r0,r12),fr14 */
L8c613b32: r[0]=20u;goto L8c613b34; /* mov #0x14,r0 */
L8c613b34: fcopy(f,5u,13u);goto L8c613b36; /* fmov fr13,fr5 */
L8c613b36: r[3]+=4294967295u;goto L8c613b38; /* add #-0x1,r3 */
L8c613b38: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c613b3a; /* fmov.s @(r0,r12),fr13 */
L8c613b3a: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c613b3c; /* tst r3,r3 */
L8c613b3c: r[11]=r[12];goto L8c613b3e; /* mov r12,r11 */
L8c613b3e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c613b40; /* fneg fr15 */
L8c613b40: take=(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c613b48;goto L8c613b44; /* bt/s 0x8c613b48 */
L8c613b44: goto L8c6139f0; /* bra 0x8c6139f0 */
L8c613b48: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c613b4a; /* cmp/hs r8,r10 */
L8c613b4a: take=(*sr&1u);r[14]+=8u;if(take)goto L8c613ba2;goto L8c613b4e; /* bt/s 0x8c613ba2 */
L8c613b4e: r[14]+=4294967288u;goto L8c613b50; /* add #-0x8,r14 */
L8c613b50: r[4]=r[12];goto L8c613b52; /* mov r12,r4 */
L8c613b52: r[6]=r[14];goto L8c613b54; /* mov r14,r6 */
L8c613b54: r[5]=r[9];goto L8c613b56; /* mov r9,r5 */
L8c613b56: r[14]+=4u;goto L8c613b58; /* add #0x4,r14 */
L8c613b58: r[8]+=16u;goto L8c613b5a; /* add #0x10,r8 */
L8c613b5a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613b5c; /* fschg */
L8c613b5c: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613b5e; /* fmov.s @r4+,fr0 */
L8c613b5e: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613b60; /* fmov.s @r4+,fr2 */
L8c613b60: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613b62; /* fldi0 fr3 */
L8c613b62: fcopy(f,8u,1u);goto L8c613b64; /* fmov fr1,fr8 */
L8c613b64: fcopy(f,10u,3u);goto L8c613b66; /* fmov fr3,fr10 */
L8c613b66: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c613b68; /* fipr fv8,fv0 */
L8c613b68: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613b6a; /* fmov.s @r4+,fr4 */
L8c613b6a: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613b6c; /* fmov.s @r4,fr6 */
L8c613b6c: fcopy(f,8u,5u);goto L8c613b6e; /* fmov fr5,fr8 */
L8c613b6e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613b70; /* fschg */
L8c613b70: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c613b72; /* mov.w @r6+,r2 */
L8c613b72: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c613b74; /* mov.w @r6,r3 */
L8c613b74: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613b76; /* fmov.s fr6,@-r8 */
L8c613b76: *fpul=r[3];goto L8c613b78; /* lds r3,FPUL */
L8c613b78: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613b7a; /* fmov.s fr5,@-r8 */
L8c613b7a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c613b7c; /* fsub fr3,fr11 */
L8c613b7c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613b7e; /* fmov.s fr4,@-r8 */
L8c613b7e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c613b80; /* float FPUL,fr1 */
L8c613b80: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c613b82; /* fmul fr8,fr1 */
L8c613b82: *fpul=r[2];goto L8c613b84; /* lds r2,FPUL */
L8c613b84: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613b86; /* mov.l r5,@-r8 */
L8c613b86: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c613b88; /* float FPUL,fr2 */
L8c613b88: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613b8a; /* fmul fr8,fr2 */
L8c613b8a: r[8]+=28u;goto L8c613b8c; /* add #0x1c,r8 */
L8c613b8c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c613b8e; /* fmov.s fr11,@-r8 */
L8c613b8e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613b90; /* fmov.s fr1,@-r8 */
L8c613b90: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613b92; /* fmov.s fr2,@-r8 */
L8c613b92: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613b94; /* pref @r8 */
L8c613b94: r[8]+=16u;goto L8c613b96; /* add #0x10,r8 */
L8c613b96: r[3]=r[8];goto L8c613b98; /* mov r8,r3 */
L8c613b98: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613b9a; /* mov.l @(0x8,r15),r2 */
L8c613b9a: r[3]-=r[10];goto L8c613b9c; /* sub r10,r3 */
L8c613b9c: r[3]+=32u;goto L8c613b9e; /* add #0x20,r3 */
L8c613b9e: r[2]+=r[3];goto L8c613ba0; /* add r3,r2 */
L8c613ba0: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613ba2; /* mov.l r2,@(0x8,r15) */
L8c613ba2: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c613ba4; /* mov.l @(0x10,r15),r3 */
L8c613ba4: r[3]+=4294967295u;goto L8c613ba6; /* add #-0x1,r3 */
L8c613ba6: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c613ba8; /* tst r3,r3 */
L8c613ba8: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;if(take)goto L8c613bb0;goto L8c613bac; /* bt/s 0x8c613bb0 */
L8c613bac: goto L8c61399c; /* bra 0x8c61399c */
L8c613bb0: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613bb2; /* mov.l @(0x8,r15),r3 */
L8c613bb2: r[4]=rd(m,bus,&io_status,0x8c613c20u,4);if(io_status)return io_status;goto L8c613bb4; /* mov.l 0x8c613c20,r4 */
L8c613bb4: r[3]>>=2;goto L8c613bb6; /* shlr2 r3 */
L8c613bb6: r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c613bb8; /* mov.l @r4,r1 */
L8c613bb8: r[3]>>=2;goto L8c613bba; /* shlr2 r3 */
L8c613bba: *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c613bbc; /* shlr r3 */
L8c613bbc: r[1]+=r[3];goto L8c613bbe; /* add r3,r1 */
L8c613bbe: wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c613bc0; /* mov.l r1,@r4 */
L8c613bc0: r[4]=rd(m,bus,&io_status,0x8c613c24u,4);if(io_status)return io_status;goto L8c613bc2; /* mov.l 0x8c613c24,r4 */
L8c613bc2: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c613bc4; /* mov.l @(0xc,r15),r3 */
L8c613bc4: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c613bc6; /* mov.l @r4,r2 */
L8c613bc6: r[2]+=r[3];goto L8c613bc8; /* add r3,r2 */
L8c613bc8: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c613bca; /* mov.l r2,@r4 */
L8c613bca: r[15]+=20u;goto L8c613bcc; /* add #0x14,r15 */
L8c613bcc: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613bce; /* lds.l @r15+,PR */
L8c613bce: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613bd0; /* fmov.s @r15+,fr13 */
L8c613bd0: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613bd2; /* fmov.s @r15+,fr14 */
L8c613bd2: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613bd4; /* fmov.s @r15+,fr15 */
L8c613bd4: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613bd6; /* mov.l @r15+,r9 */
L8c613bd6: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613bd8; /* mov.l @r15+,r10 */
L8c613bd8: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613bda; /* mov.l @r15+,r11 */
L8c613bda: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613bdc; /* mov.l @r15+,r12 */
L8c613bdc: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613bde; /* mov.l @r15+,r13 */
L8c613bde: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_uv_alternate_strip_clipped_8c614f70(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c614f70;
L8c614f70: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c614f72; /* mov.l r14,@-r15 */
L8c614f72: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c614f74; /* mov.l r13,@-r15 */
L8c614f74: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c614f76; /* mov.l r12,@-r15 */
L8c614f76: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c614f78; /* mov.l r11,@-r15 */
L8c614f78: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c614f7a; /* mov.l r10,@-r15 */
L8c614f7a: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c614f7c; /* mov.l r9,@-r15 */
L8c614f7c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c614f7e; /* fmov.s fr15,@-r15 */
L8c614f7e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c614f80; /* fmov.s fr14,@-r15 */
L8c614f80: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c614f82; /* fmov.s fr13,@-r15 */
L8c614f82: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c614f84; /* fmov.s fr12,@-r15 */
L8c614f84: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c614f86; /* sts.l PR,@-r15 */
L8c614f86: r[15]+=4294967216u;goto L8c614f88; /* add #-0x50,r15 */
L8c614f88: r[3]=rd(m,bus,&io_status,0x8c614fc0u,4);if(io_status)return io_status;goto L8c614f8a; /* mov.l 0x8c614fc0,r3 */
L8c614f8a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c614f8c; /* fldi1 fr12 */
L8c614f8c: target=r[3];*pr=0x8c614f90u;r[12]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c614f90; /* jsr @r3 */
L8c614f90: r[2]=rd(m,bus,&io_status,0x8c614fc4u,4);if(io_status)return io_status;goto L8c614f92; /* mov.l 0x8c614fc4,r2 */
L8c614f92: r[1]=0u;goto L8c614f94; /* mov #0x0,r1 */
L8c614f94: r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c614f96; /* mov.w @(0x2,r12),r0 */
L8c614f96: r[13]=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;goto L8c614f98; /* mov.l @(0x4,r12),r13 */
L8c614f98: r[14]=rd(m,bus,&io_status,r[12]+0x14u,4);if(io_status)return io_status;goto L8c614f9a; /* mov.l @(0x14,r12),r14 */
L8c614f9a: r[0]=r[0]&65535u;goto L8c614f9c; /* extu.w r0,r0 */
L8c614f9c: wr(m,bus,&io_status,r[15]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c614f9e; /* mov.l r0,@(0x1c,r15) */
L8c614f9e: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c614fa0; /* mov.l @r2,r3 */
L8c614fa0: wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c614fa2; /* mov.l r3,@(0xc,r15) */
L8c614fa2: wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c614fa4; /* mov.l r1,@(0x10,r15) */
L8c614fa4: goto L8c614fc8; /* bra 0x8c614fc8 */
L8c614fc8: r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c614fca; /* mov.w @r13+,r3 */
L8c614fca: r[4]=rd(m,bus,&io_status,0x8c6150e8u,4);if(io_status)return io_status;goto L8c614fcc; /* mov.l 0x8c6150e8,r4 */
L8c614fcc: r[3]=r[3]&65535u;goto L8c614fce; /* extu.w r3,r3 */
L8c614fce: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c614fd0; /* tst r4,r3 */
L8c614fd0: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c614fd2; /* mov.l r3,@(0x8,r15) */
L8c614fd2: take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c614fe4;goto L8c614fd6; /* bt/s 0x8c614fe4 */
L8c614fd6: r[4]=sx(m,bus,&io_status,0x8c6150e4u,2);if(io_status)return io_status;goto L8c614fd8; /* mov.w 0x8c6150e4,r4 */
L8c614fd8: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614fda; /* mov.l @(0x8,r15),r3 */
L8c614fda: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c614fdc; /* fldi1 fr4 */
L8c614fdc: r[3]&=r[4];goto L8c614fde; /* and r4,r3 */
L8c614fde: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c614fe0; /* fneg fr4 */
L8c614fe0: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c614fe2; /* mov.l r3,@(0x8,r15) */
L8c614fe2: fcopy(f,15u,4u);goto L8c614fe4; /* fmov fr4,fr15 */
L8c614fe4: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614fe6; /* mov.l @(0x8,r15),r3 */
L8c614fe6: r[0]=16u;goto L8c614fe8; /* mov #0x10,r0 */
L8c614fe8: r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c614fea; /* mov.l @(0x10,r15),r2 */
L8c614fea: r[9]=r[8];goto L8c614fec; /* mov r8,r9 */
L8c614fec: r[2]+=r[3];goto L8c614fee; /* add r3,r2 */
L8c614fee: wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c614ff0; /* mov.l r2,@(0x10,r15) */
L8c614ff0: r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614ff2; /* mov.l @(0x8,r15),r1 */
L8c614ff2: r[1]+=4294967294u;goto L8c614ff4; /* add #-0x2,r1 */
L8c614ff4: wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c614ff6; /* mov.l r1,@(0x8,r15) */
L8c614ff6: r[10]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c614ff8; /* mov.w @r13+,r10 */
L8c614ff8: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614ffa; /* mov.l @(0xc,r15),r3 */
L8c614ffa: r[11]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c614ffc; /* mov.w @r13+,r11 */
L8c614ffc: r[10]=r[10]&65535u;goto L8c614ffe; /* extu.w r10,r10 */
L8c614ffe: r[10]<<=2;goto L8c615000; /* shll2 r10 */
L8c615000: r[11]=r[11]&65535u;goto L8c615002; /* extu.w r11,r11 */
L8c615002: r[11]<<=2;goto L8c615004; /* shll2 r11 */
L8c615004: r[10]<<=2;goto L8c615006; /* shll2 r10 */
L8c615006: r[11]<<=2;goto L8c615008; /* shll2 r11 */
L8c615008: *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c61500a; /* shll r10 */
L8c61500a: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61500c; /* shll r11 */
L8c61500c: r[11]+=r[3];goto L8c61500e; /* add r3,r11 */
L8c61500e: r[10]+=r[3];goto L8c615010; /* add r3,r10 */
L8c615010: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c615012; /* fmov.s @(r0,r11),fr13 */
L8c615012: fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c615014; /* fmov.s @(r0,r10),fr5 */
L8c615014: r[0]=20u;goto L8c615016; /* mov #0x14,r0 */
L8c615016: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c615018; /* fmov.s @(r0,r11),fr14 */
L8c615018: fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c61501a; /* fmov.s @(r0,r10),fr6 */
L8c61501a: r[12]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61501c; /* mov.w @r13+,r12 */
L8c61501c: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61501e; /* mov.l @(0xc,r15),r3 */
L8c61501e: r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c615020; /* mov.w @r13,r2 */
L8c615020: r[12]=r[12]&65535u;goto L8c615022; /* extu.w r12,r12 */
L8c615022: r[12]<<=2;goto L8c615024; /* shll2 r12 */
L8c615024: r[2]=r[2]&65535u;goto L8c615026; /* extu.w r2,r2 */
L8c615026: r[2]<<=2;goto L8c615028; /* shll2 r2 */
L8c615028: r[2]<<=2;goto L8c61502a; /* shll2 r2 */
L8c61502a: r[12]<<=2;goto L8c61502c; /* shll2 r12 */
L8c61502c: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61502e; /* shll r12 */
L8c61502e: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c615030; /* shll r2 */
L8c615030: r[2]+=r[3];goto L8c615032; /* add r3,r2 */
L8c615032: r[12]+=r[3];goto L8c615034; /* add r3,r12 */
L8c615034: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c615036; /* pref @r2 */
L8c615036: r[3]=0u;goto L8c615038; /* mov #0x0,r3 */
L8c615038: wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;goto L8c61503a; /* mov.l r3,@(0x18,r15) */
L8c61503a: r[4]=63u;goto L8c61503c; /* mov #0x3f,r4 */
L8c61503c: r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c61503e; /* mov.l @(0xc,r10),r2 */
L8c61503e: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c615040; /* mov.l @(0xc,r11),r3 */
L8c615040: r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c615042; /* mov.l @(0xc,r12),r1 */
L8c615042: r[2]&=r[3];goto L8c615044; /* and r3,r2 */
L8c615044: r[2]&=r[1];goto L8c615046; /* and r1,r2 */
L8c615046: r[4]&=r[2];goto L8c615048; /* and r2,r4 */
L8c615048: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c61504a; /* tst r4,r4 */
L8c61504a: take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c615052;goto L8c61504e; /* bt/s 0x8c615052 */
L8c61504e: goto L8c6151ac; /* bra 0x8c6151ac */
L8c615052: r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c615054; /* mov.l @(0xc,r10),r4 */
L8c615054: r[5]=1u;goto L8c615056; /* mov #0x1,r5 */
L8c615056: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c615058; /* mov.l @(0xc,r11),r3 */
L8c615058: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61505a; /* mov.l @(0xc,r12),r2 */
L8c61505a: r[4]|=r[3];goto L8c61505c; /* or r3,r4 */
L8c61505c: r[4]|=r[2];goto L8c61505e; /* or r2,r4 */
L8c61505e: r[4]&=r[5];goto L8c615060; /* and r5,r4 */
L8c615060: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c615062; /* tst r4,r4 */
L8c615062: if(!(*sr&1u))goto L8c615068;goto L8c615064; /* bf 0x8c615068 */
L8c615064: goto L8c61518e; /* bra 0x8c61518e */
L8c615068: r[7]=r[15];goto L8c61506a; /* mov r15,r7 */
L8c61506a: r[7]+=68u;goto L8c61506c; /* add #0x44,r7 */
L8c61506c: wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c61506e; /* mov.l r10,@r7 */
L8c61506e: wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c615070; /* mov.l r11,@(0x4,r7) */
L8c615070: wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c615072; /* mov.l r12,@(0x8,r7) */
L8c615072: r[6]=0u;goto L8c615120; /* bra 0x8c615120 */
L8c615076: r[2]=r[6];goto L8c615078; /* mov r6,r2 */
L8c615078: r[1]=r[6];goto L8c61507a; /* mov r6,r1 */
L8c61507a: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c61507c; /* shll r2 */
L8c61507c: r[3]=r[6];goto L8c61507e; /* mov r6,r3 */
L8c61507e: r[3]<<=2;goto L8c615080; /* shll2 r3 */
L8c615080: r[2]+=r[1];goto L8c615082; /* add r1,r2 */
L8c615082: r[2]<<=2;goto L8c615084; /* shll2 r2 */
L8c615084: r[3]+=r[7];goto L8c615086; /* add r7,r3 */
L8c615086: r[2]+=r[4];goto L8c615088; /* add r4,r2 */
L8c615088: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61508a; /* mov.l r3,@(0x4,r15) */
L8c61508a: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61508c; /* mov.l r2,@r15 */
L8c61508c: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61508e; /* mov.l @r3,r1 */
L8c61508e: r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c615090; /* mov.l @(0xc,r1),r2 */
L8c615090: *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c615092; /* tst r5,r2 */
L8c615092: take=!(*sr&1u);r[6]+=1u;if(take)goto L8c6150f8;goto L8c615096; /* bf/s 0x8c6150f8 */
L8c615096: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615098; /* mov.l @(0x4,r15),r1 */
L8c615098: r[0]=24u;goto L8c61509a; /* mov #0x18,r0 */
L8c61509a: fcopy(f,4u,12u);goto L8c61509c; /* fmov fr12,fr4 */
L8c61509c: r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61509e; /* mov.l @r1,r3 */
L8c61509e: r[4]=rd(m,bus,&io_status,0x8c6150ecu,4);if(io_status)return io_status;goto L8c6150a0; /* mov.l 0x8c6150ec,r4 */
L8c6150a0: wr(m,bus,&io_status,r[15]+0x14u,r[3],4);if(io_status)return io_status;goto L8c6150a2; /* mov.l r3,@(0x14,r15) */
L8c6150a2: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6150a4; /* fmov.s @(r0,r3),fr3 */
L8c6150a4: r[0]=12u;goto L8c6150a6; /* mov #0xc,r0 */
L8c6150a6: r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c6150a8; /* mov.l @(0x14,r15),r2 */
L8c6150a8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6150aa; /* fdiv fr3,fr4 */
L8c6150aa: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6150ac; /* fmov.s @(r0,r4),fr3 */
L8c6150ac: r[0]=16u;goto L8c6150ae; /* mov #0x10,r0 */
L8c6150ae: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6150b0; /* mov.l @r15,r3 */
L8c6150b0: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6150b2; /* fmov.s @(r0,r2),fr2 */
L8c6150b2: r[0]=16u;goto L8c6150b4; /* mov #0x10,r0 */
L8c6150b4: r[2]=rd(m,bus,&io_status,0x8c6150f0u,4);if(io_status)return io_status;goto L8c6150b6; /* mov.l 0x8c6150f0,r2 */
L8c6150b6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6150b8; /* fsub fr3,fr2 */
L8c6150b8: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6150ba; /* fmov.s @r2,fr3 */
L8c6150ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6150bc; /* fmul fr4,fr2 */
L8c6150bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6150be; /* fdiv fr3,fr2 */
L8c6150be: fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6150c0; /* fmov.s fr2,@r3 */
L8c6150c0: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6150c2; /* mov.l @(0x4,r15),r1 */
L8c6150c2: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6150c4; /* fmov.s @(r0,r4),fr3 */
L8c6150c4: r[0]=20u;goto L8c6150c6; /* mov #0x14,r0 */
L8c6150c6: r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c6150c8; /* mov.l @r1,r1 */
L8c6150c8: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6150ca; /* mov.l @r15,r3 */
L8c6150ca: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c6150cc; /* fmov.s @(r0,r1),fr2 */
L8c6150cc: r[1]=rd(m,bus,&io_status,0x8c6150f4u,4);if(io_status)return io_status;goto L8c6150ce; /* mov.l 0x8c6150f4,r1 */
L8c6150ce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6150d0; /* fsub fr3,fr2 */
L8c6150d0: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6150d2; /* fmov.s @r1,fr3 */
L8c6150d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6150d4; /* fmul fr4,fr2 */
L8c6150d4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6150d6; /* fdiv fr3,fr2 */
L8c6150d6: r[0]=4u;goto L8c6150d8; /* mov #0x4,r0 */
L8c6150d8: fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c6150da; /* fmov fr2,@(r0,r3) */
L8c6150da: r[0]=8u;goto L8c6150dc; /* mov #0x8,r0 */
L8c6150dc: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6150de; /* mov.l @r15,r3 */
L8c6150de: fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c6150e0; /* fmov fr4,@(r0,r3) */
L8c6150e0: goto L8c615120; /* bra 0x8c615120 */
L8c6150f8: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6150fa; /* mov.l @(0x4,r15),r3 */
L8c6150fa: r[0]=16u;goto L8c6150fc; /* mov #0x10,r0 */
L8c6150fc: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6150fe; /* mov.l @r15,r2 */
L8c6150fe: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c615100; /* mov.l @r3,r1 */
L8c615100: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c615102; /* fmov.s @(r0,r1),fr3 */
L8c615102: r[0]=20u;goto L8c615104; /* mov #0x14,r0 */
L8c615104: fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c615106; /* fmov.s fr3,@r2 */
L8c615106: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615108; /* mov.l @(0x4,r15),r2 */
L8c615108: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61510a; /* mov.l @r15,r3 */
L8c61510a: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61510c; /* mov.l @r2,r1 */
L8c61510c: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61510e; /* fmov.s @(r0,r1),fr3 */
L8c61510e: r[0]=4u;goto L8c615110; /* mov #0x4,r0 */
L8c615110: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c615112; /* fmov fr3,@(r0,r3) */
L8c615112: r[0]=24u;goto L8c615114; /* mov #0x18,r0 */
L8c615114: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615116; /* mov.l @(0x4,r15),r2 */
L8c615116: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615118; /* mov.l @r15,r3 */
L8c615118: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61511a; /* mov.l @r2,r1 */
L8c61511a: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61511c; /* fmov.s @(r0,r1),fr3 */
L8c61511c: r[0]=8u;goto L8c61511e; /* mov #0x8,r0 */
L8c61511e: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c615120; /* fmov fr3,@(r0,r3) */
L8c615120: r[3]=3u;goto L8c615122; /* mov #0x3,r3 */
L8c615122: r[4]=r[15];goto L8c615124; /* mov r15,r4 */
L8c615124: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c615126; /* cmp/ge r3,r6 */
L8c615126: take=!(*sr&1u);r[4]+=32u;if(take)goto L8c615076;goto L8c61512a; /* bf/s 0x8c615076 */
L8c61512a: r[7]=r[4];goto L8c61512c; /* mov r4,r7 */
L8c61512c: r[0]=8u;goto L8c61512e; /* mov #0x8,r0 */
L8c61512e: r[3]=r[4];goto L8c615130; /* mov r4,r3 */
L8c615130: fload(m,bus,&io_status,f,8u,r[0]+r[7]);if(io_status)return io_status;goto L8c615132; /* fmov.s @(r0,r7),fr8 */
L8c615132: r[3]+=12u;goto L8c615134; /* add #0xc,r3 */
L8c615134: r[6]=r[4];goto L8c615136; /* mov r4,r6 */
L8c615136: r[0]=4u;goto L8c615138; /* mov #0x4,r0 */
L8c615138: fcopy(f,0u,8u);goto L8c61513a; /* fmov fr8,fr0 */
L8c61513a: r[6]+=24u;goto L8c61513c; /* add #0x18,r6 */
L8c61513c: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61513e; /* mov.l r3,@r15 */
L8c61513e: fload(m,bus,&io_status,f,9u,r[0]+r[7]);if(io_status)return io_status;goto L8c615140; /* fmov.s @(r0,r7),fr9 */
L8c615140: fload(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c615142; /* fmov.s @(r0,r3),fr4 */
L8c615142: r[0]=8u;goto L8c615144; /* mov #0x8,r0 */
L8c615144: fload(m,bus,&io_status,f,6u,r[0]+r[3]);if(io_status)return io_status;goto L8c615146; /* fmov.s @(r0,r3),fr6 */
L8c615146: r[0]=4u;goto L8c615148; /* mov #0x4,r0 */
L8c615148: fcopy(f,3u,9u);goto L8c61514a; /* fmov fr9,fr3 */
L8c61514a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61514c; /* fmul fr4,fr0 */
L8c61514c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61514e; /* fmul fr6,fr3 */
L8c61514e: fload(m,bus,&io_status,f,7u,r[7]);if(io_status)return io_status;goto L8c615150; /* fmov.s @r7,fr7 */
L8c615150: fload(m,bus,&io_status,f,5u,r[3]);if(io_status)return io_status;goto L8c615152; /* fmov.s @r3,fr5 */
L8c615152: fcopy(f,2u,7u);goto L8c615154; /* fmov fr7,fr2 */
L8c615154: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615156; /* fmul fr6,fr2 */
L8c615156: fcopy(f,1u,9u);goto L8c615158; /* fmov fr9,fr1 */
L8c615158: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c61515a; /* fsub fr3,fr0 */
L8c61515a: fcopy(f,3u,8u);goto L8c61515c; /* fmov fr8,fr3 */
L8c61515c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61515e; /* fmul fr5,fr3 */
L8c61515e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615160; /* fsub fr3,fr2 */
L8c615160: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c615162; /* fmov.s @(r0,r6),fr3 */
L8c615162: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615164; /* fmul fr2,fr3 */
L8c615164: fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c615166; /* fmov.s @r6,fr2 */
L8c615166: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615168; /* fmac fr0,fr2,fr3 */
L8c615168: fcopy(f,2u,3u);goto L8c61516a; /* fmov fr3,fr2 */
L8c61516a: fcopy(f,3u,7u);goto L8c61516c; /* fmov fr7,fr3 */
L8c61516c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61516e; /* fmul fr4,fr3 */
L8c61516e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[5],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615170; /* fmul fr5,fr1 */
L8c615170: r[0]=8u;goto L8c615172; /* mov #0x8,r0 */
L8c615172: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c615174; /* fsub fr3,fr1 */
L8c615174: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c615176; /* fmov.s @(r0,r6),fr3 */
L8c615176: fcopy(f,0u,1u);goto L8c615178; /* fmov fr1,fr0 */
L8c615178: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61517a; /* fmac fr0,fr3,fr2 */
L8c61517a: fcopy(f,3u,15u);goto L8c61517c; /* fmov fr15,fr3 */
L8c61517c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c61517e; /* fneg fr3 */
L8c61517e: fcopy(f,4u,2u);goto L8c615180; /* fmov fr2,fr4 */
L8c615180: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c615182; /* fmul fr3,fr4 */
L8c615182: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c615184; /* fldi0 fr2 */
L8c615184: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c615186; /* fcmp/gt fr2,fr4 */
L8c615186: if((*sr&1u))goto L8c6151ac;goto L8c615188; /* bt 0x8c6151ac */
L8c615188: wr(m,bus,&io_status,r[15]+0x18u,r[5],4);if(io_status)return io_status;goto L8c61518a; /* mov.l r5,@(0x18,r15) */
L8c61518a: fcopy(f,4u,12u);goto L8c6151ac; /* bra 0x8c6151ac */
L8c61518e: r[0]=20u;goto L8c615190; /* mov #0x14,r0 */
L8c615190: fcopy(f,3u,13u);goto L8c615192; /* fmov fr13,fr3 */
L8c615192: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c615194; /* fmov.s @(r0,r12),fr2 */
L8c615194: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615196; /* fsub fr5,fr3 */
L8c615196: r[0]=16u;goto L8c615198; /* mov #0x10,r0 */
L8c615198: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61519a; /* fsub fr14,fr2 */
L8c61519a: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c61519c; /* fmov.s @(r0,r12),fr1 */
L8c61519c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61519e; /* fsub fr13,fr1 */
L8c61519e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6151a0; /* fmul fr2,fr3 */
L8c6151a0: fcopy(f,2u,14u);goto L8c6151a2; /* fmov fr14,fr2 */
L8c6151a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[6],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6151a4; /* fsub fr6,fr2 */
L8c6151a4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6151a6; /* fmul fr1,fr2 */
L8c6151a6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6151a8; /* fsub fr2,fr3 */
L8c6151a8: fcopy(f,4u,3u);goto L8c6151aa; /* fmov fr3,fr4 */
L8c6151aa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6151ac; /* fmul fr15,fr4 */
L8c6151ac: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c6151ae; /* cmp/hs r8,r9 */
L8c6151ae: if((*sr&1u))goto L8c615202;goto L8c6151b0; /* bt 0x8c615202 */
L8c6151b0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6151b2; /* fldi0 fr3 */
L8c6151b2: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6151b4; /* fcmp/gt fr4,fr3 */
L8c6151b4: if((*sr&1u))goto L8c615252;goto L8c6151b6; /* bt 0x8c615252 */
L8c6151b6: r[5]=rd(m,bus,&io_status,0x8c6153f4u,4);if(io_status)return io_status;goto L8c6151b8; /* mov.l 0x8c6153f4,r5 */
L8c6151b8: r[6]=r[14];goto L8c6151ba; /* mov r14,r6 */
L8c6151ba: r[4]=r[11];goto L8c6151bc; /* mov r11,r4 */
L8c6151bc: r[14]+=4u;goto L8c6151be; /* add #0x4,r14 */
L8c6151be: r[8]+=16u;goto L8c6151c0; /* add #0x10,r8 */
L8c6151c0: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6151c2; /* fschg */
L8c6151c2: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6151c4; /* fmov.s @r4+,fr0 */
L8c6151c4: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6151c6; /* fmov.s @r4+,fr2 */
L8c6151c6: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6151c8; /* fldi0 fr3 */
L8c6151c8: fcopy(f,8u,1u);goto L8c6151ca; /* fmov fr1,fr8 */
L8c6151ca: fcopy(f,10u,3u);goto L8c6151cc; /* fmov fr3,fr10 */
L8c6151cc: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c6151ce; /* fipr fv8,fv0 */
L8c6151ce: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6151d0; /* fmov.s @r4+,fr4 */
L8c6151d0: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6151d2; /* fmov.s @r4,fr6 */
L8c6151d2: fcopy(f,8u,5u);goto L8c6151d4; /* fmov fr5,fr8 */
L8c6151d4: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6151d6; /* fschg */
L8c6151d6: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c6151d8; /* mov.w @r6+,r2 */
L8c6151d8: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c6151da; /* mov.w @r6,r3 */
L8c6151da: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6151dc; /* fmov.s fr6,@-r8 */
L8c6151dc: *fpul=r[3];goto L8c6151de; /* lds r3,FPUL */
L8c6151de: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6151e0; /* fmov.s fr5,@-r8 */
L8c6151e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c6151e2; /* fsub fr3,fr11 */
L8c6151e2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6151e4; /* fmov.s fr4,@-r8 */
L8c6151e4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c6151e6; /* float FPUL,fr1 */
L8c6151e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6151e8; /* fmul fr8,fr1 */
L8c6151e8: *fpul=r[2];goto L8c6151ea; /* lds r2,FPUL */
L8c6151ea: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6151ec; /* mov.l r5,@-r8 */
L8c6151ec: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c6151ee; /* float FPUL,fr2 */
L8c6151ee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6151f0; /* fmul fr8,fr2 */
L8c6151f0: r[8]+=28u;goto L8c6151f2; /* add #0x1c,r8 */
L8c6151f2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c6151f4; /* fmov.s fr11,@-r8 */
L8c6151f4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6151f6; /* fmov.s fr1,@-r8 */
L8c6151f6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6151f8; /* fmov.s fr2,@-r8 */
L8c6151f8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6151fa; /* pref @r8 */
L8c6151fa: r[8]+=16u;goto L8c6151fc; /* add #0x10,r8 */
L8c6151fc: r[14]+=4294967292u;goto L8c6151fe; /* add #-0x4,r14 */
L8c6151fe: r[9]=r[8];goto L8c615298; /* bra 0x8c615298 */
L8c615202: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615204; /* fldi0 fr3 */
L8c615204: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c615206; /* fcmp/gt fr4,fr3 */
L8c615206: take=!(*sr&1u);r[14]+=4u;if(take)goto L8c615298;goto L8c61520a; /* bf/s 0x8c615298 */
L8c61520a: r[14]+=4294967292u;goto L8c61520c; /* add #-0x4,r14 */
L8c61520c: r[5]=rd(m,bus,&io_status,0x8c6153f8u,4);if(io_status)return io_status;goto L8c61520e; /* mov.l 0x8c6153f8,r5 */
L8c61520e: r[6]=r[14];goto L8c615210; /* mov r14,r6 */
L8c615210: r[4]=r[10];goto L8c615212; /* mov r10,r4 */
L8c615212: r[14]+=4u;goto L8c615214; /* add #0x4,r14 */
L8c615214: r[8]+=16u;goto L8c615216; /* add #0x10,r8 */
L8c615216: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615218; /* fschg */
L8c615218: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61521a; /* fmov.s @r4+,fr0 */
L8c61521a: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61521c; /* fmov.s @r4+,fr2 */
L8c61521c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61521e; /* fldi0 fr3 */
L8c61521e: fcopy(f,8u,1u);goto L8c615220; /* fmov fr1,fr8 */
L8c615220: fcopy(f,10u,3u);goto L8c615222; /* fmov fr3,fr10 */
L8c615222: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c615224; /* fipr fv8,fv0 */
L8c615224: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615226; /* fmov.s @r4+,fr4 */
L8c615226: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615228; /* fmov.s @r4,fr6 */
L8c615228: fcopy(f,8u,5u);goto L8c61522a; /* fmov fr5,fr8 */
L8c61522a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61522c; /* fschg */
L8c61522c: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c61522e; /* mov.w @r6+,r2 */
L8c61522e: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c615230; /* mov.w @r6,r3 */
L8c615230: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615232; /* fmov.s fr6,@-r8 */
L8c615232: *fpul=r[3];goto L8c615234; /* lds r3,FPUL */
L8c615234: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615236; /* fmov.s fr5,@-r8 */
L8c615236: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c615238; /* fsub fr3,fr11 */
L8c615238: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61523a; /* fmov.s fr4,@-r8 */
L8c61523a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c61523c; /* float FPUL,fr1 */
L8c61523c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61523e; /* fmul fr8,fr1 */
L8c61523e: *fpul=r[2];goto L8c615240; /* lds r2,FPUL */
L8c615240: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615242; /* mov.l r5,@-r8 */
L8c615242: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c615244; /* float FPUL,fr2 */
L8c615244: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615246; /* fmul fr8,fr2 */
L8c615246: r[8]+=28u;goto L8c615248; /* add #0x1c,r8 */
L8c615248: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c61524a; /* fmov.s fr11,@-r8 */
L8c61524a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61524c; /* fmov.s fr1,@-r8 */
L8c61524c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61524e; /* fmov.s fr2,@-r8 */
L8c61524e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615250; /* pref @r8 */
L8c615250: r[8]+=16u;goto L8c615252; /* add #0x10,r8 */
L8c615252: r[5]=rd(m,bus,&io_status,0x8c6153f8u,4);if(io_status)return io_status;goto L8c615254; /* mov.l 0x8c6153f8,r5 */
L8c615254: r[6]=r[14];goto L8c615256; /* mov r14,r6 */
L8c615256: r[4]=r[11];goto L8c615258; /* mov r11,r4 */
L8c615258: r[14]+=4u;goto L8c61525a; /* add #0x4,r14 */
L8c61525a: r[8]+=16u;goto L8c61525c; /* add #0x10,r8 */
L8c61525c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61525e; /* fschg */
L8c61525e: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615260; /* fmov.s @r4+,fr0 */
L8c615260: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615262; /* fmov.s @r4+,fr2 */
L8c615262: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615264; /* fldi0 fr3 */
L8c615264: fcopy(f,8u,1u);goto L8c615266; /* fmov fr1,fr8 */
L8c615266: fcopy(f,10u,3u);goto L8c615268; /* fmov fr3,fr10 */
L8c615268: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c61526a; /* fipr fv8,fv0 */
L8c61526a: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61526c; /* fmov.s @r4+,fr4 */
L8c61526c: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61526e; /* fmov.s @r4,fr6 */
L8c61526e: fcopy(f,8u,5u);goto L8c615270; /* fmov fr5,fr8 */
L8c615270: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615272; /* fschg */
L8c615272: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c615274; /* mov.w @r6+,r2 */
L8c615274: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c615276; /* mov.w @r6,r3 */
L8c615276: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615278; /* fmov.s fr6,@-r8 */
L8c615278: *fpul=r[3];goto L8c61527a; /* lds r3,FPUL */
L8c61527a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61527c; /* fmov.s fr5,@-r8 */
L8c61527c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c61527e; /* fsub fr3,fr11 */
L8c61527e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615280; /* fmov.s fr4,@-r8 */
L8c615280: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c615282; /* float FPUL,fr1 */
L8c615282: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615284; /* fmul fr8,fr1 */
L8c615284: *fpul=r[2];goto L8c615286; /* lds r2,FPUL */
L8c615286: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615288; /* mov.l r5,@-r8 */
L8c615288: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c61528a; /* float FPUL,fr2 */
L8c61528a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61528c; /* fmul fr8,fr2 */
L8c61528c: r[8]+=28u;goto L8c61528e; /* add #0x1c,r8 */
L8c61528e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c615290; /* fmov.s fr11,@-r8 */
L8c615290: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c615292; /* fmov.s fr1,@-r8 */
L8c615292: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c615294; /* fmov.s fr2,@-r8 */
L8c615294: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615296; /* pref @r8 */
L8c615296: r[8]+=16u;goto L8c615298; /* add #0x10,r8 */
L8c615298: r[2]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c61529a; /* mov.l @(0x18,r15),r2 */
L8c61529a: *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c61529c; /* tst r2,r2 */
L8c61529c: if((*sr&1u))goto L8c615312;goto L8c61529e; /* bt 0x8c615312 */
L8c61529e: r[4]=rd(m,bus,&io_status,0x8c6153fcu,4);if(io_status)return io_status;goto L8c6152a0; /* mov.l 0x8c6153fc,r4 */
L8c6152a0: r[3]=0u;goto L8c6152a2; /* mov #0x0,r3 */
L8c6152a2: r[14]+=4294967292u;goto L8c6152a4; /* add #-0x4,r14 */
L8c6152a4: r[9]=rd(m,bus,&io_status,0x8c615400u,4);if(io_status)return io_status;goto L8c6152a6; /* mov.l 0x8c615400,r9 */
L8c6152a6: wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c6152a8; /* mov.l r3,@r4 */
L8c6152a8: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c6152aa; /* mov.w @r14,r3 */
L8c6152aa: r[4]=r[9];goto L8c6152ac; /* mov r9,r4 */
L8c6152ac: *fpul=r[3];goto L8c6152ae; /* lds r3,FPUL */
L8c6152ae: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6152b0; /* float FPUL,fr3 */
L8c6152b0: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6152b2; /* ftrc fr3,FPUL */
L8c6152b2: r[2]=*fpul;goto L8c6152b4; /* sts FPUL,r2 */
L8c6152b4: wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c6152b6; /* mov.w r2,@r4 */
L8c6152b6: r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c6152b8; /* mov.w @(0x2,r14),r0 */
L8c6152b8: r[14]+=4u;goto L8c6152ba; /* add #0x4,r14 */
L8c6152ba: r[3]=r[0];goto L8c6152bc; /* mov r0,r3 */
L8c6152bc: *fpul=r[3];goto L8c6152be; /* lds r3,FPUL */
L8c6152be: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6152c0; /* float FPUL,fr3 */
L8c6152c0: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6152c2; /* ftrc fr3,FPUL */
L8c6152c2: r[0]=*fpul;goto L8c6152c4; /* sts FPUL,r0 */
L8c6152c4: wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6152c6; /* mov.w r0,@(0x2,r4) */
L8c6152c6: r[4]+=4u;goto L8c6152c8; /* add #0x4,r4 */
L8c6152c8: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c6152ca; /* mov.w @r14,r3 */
L8c6152ca: *fpul=r[3];goto L8c6152cc; /* lds r3,FPUL */
L8c6152cc: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6152ce; /* float FPUL,fr3 */
L8c6152ce: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6152d0; /* ftrc fr3,FPUL */
L8c6152d0: r[2]=*fpul;goto L8c6152d2; /* sts FPUL,r2 */
L8c6152d2: wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c6152d4; /* mov.w r2,@r4 */
L8c6152d4: r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c6152d6; /* mov.w @(0x2,r14),r0 */
L8c6152d6: r[3]=r[0];goto L8c6152d8; /* mov r0,r3 */
L8c6152d8: *fpul=r[3];goto L8c6152da; /* lds r3,FPUL */
L8c6152da: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6152dc; /* float FPUL,fr3 */
L8c6152dc: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6152de; /* ftrc fr3,FPUL */
L8c6152de: r[14]+=4u;goto L8c6152e0; /* add #0x4,r14 */
L8c6152e0: r[0]=*fpul;goto L8c6152e2; /* sts FPUL,r0 */
L8c6152e2: r[6]=r[12];goto L8c6152e4; /* mov r12,r6 */
L8c6152e4: r[5]=r[11];goto L8c6152e6; /* mov r11,r5 */
L8c6152e6: r[7]=r[9];goto L8c6152e8; /* mov r9,r7 */
L8c6152e8: wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6152ea; /* mov.w r0,@(0x2,r4) */
L8c6152ea: r[4]=r[9];goto L8c6152ec; /* mov r9,r4 */
L8c6152ec: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c6152ee; /* mov.w @r14,r3 */
L8c6152ee: r[4]+=8u;goto L8c6152f0; /* add #0x8,r4 */
L8c6152f0: *fpul=r[3];goto L8c6152f2; /* lds r3,FPUL */
L8c6152f2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6152f4; /* float FPUL,fr3 */
L8c6152f4: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6152f6; /* ftrc fr3,FPUL */
L8c6152f6: r[2]=*fpul;goto L8c6152f8; /* sts FPUL,r2 */
L8c6152f8: wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c6152fa; /* mov.w r2,@r4 */
L8c6152fa: r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c6152fc; /* mov.w @(0x2,r14),r0 */
L8c6152fc: r[3]=r[0];goto L8c6152fe; /* mov r0,r3 */
L8c6152fe: *fpul=r[3];goto L8c615300; /* lds r3,FPUL */
L8c615300: r[3]=rd(m,bus,&io_status,0x8c615404u,4);if(io_status)return io_status;goto L8c615302; /* mov.l 0x8c615404,r3 */
L8c615302: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c615304; /* float FPUL,fr3 */
L8c615304: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c615306; /* ftrc fr3,FPUL */
L8c615306: r[0]=*fpul;goto L8c615308; /* sts FPUL,r0 */
L8c615308: wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61530a; /* mov.w r0,@(0x2,r4) */
L8c61530a: target=r[3];*pr=0x8c61530eu;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61530e; /* jsr @r3 */
L8c61530e: r[9]=r[8];goto L8c615310; /* mov r8,r9 */
L8c615310: r[14]+=4294967292u;goto L8c615312; /* add #-0x4,r14 */
L8c615312: fcopy(f,5u,13u);goto L8c615314; /* fmov fr13,fr5 */
L8c615314: r[0]=16u;goto L8c615316; /* mov #0x10,r0 */
L8c615316: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615318; /* mov.l @(0x8,r15),r3 */
L8c615318: r[10]=r[11];goto L8c61531a; /* mov r11,r10 */
L8c61531a: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c61531c; /* fmov.s @(r0,r12),fr13 */
L8c61531c: r[0]=20u;goto L8c61531e; /* mov #0x14,r0 */
L8c61531e: fcopy(f,6u,14u);goto L8c615320; /* fmov fr14,fr6 */
L8c615320: r[3]+=4294967295u;goto L8c615322; /* add #-0x1,r3 */
L8c615322: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c615324; /* fmov.s @(r0,r12),fr14 */
L8c615324: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c615326; /* tst r3,r3 */
L8c615326: r[11]=r[12];goto L8c615328; /* mov r12,r11 */
L8c615328: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c61532a; /* fneg fr15 */
L8c61532a: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c615332;goto L8c61532e; /* bt/s 0x8c615332 */
L8c61532e: goto L8c61501a; /* bra 0x8c61501a */
L8c615332: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c615334; /* cmp/hs r8,r9 */
L8c615334: take=(*sr&1u);r[14]+=8u;if(take)goto L8c615380;goto L8c615338; /* bt/s 0x8c615380 */
L8c615338: r[14]+=4294967288u;goto L8c61533a; /* add #-0x8,r14 */
L8c61533a: r[5]=rd(m,bus,&io_status,0x8c6153f4u,4);if(io_status)return io_status;goto L8c61533c; /* mov.l 0x8c6153f4,r5 */
L8c61533c: r[6]=r[14];goto L8c61533e; /* mov r14,r6 */
L8c61533e: r[4]=r[12];goto L8c615340; /* mov r12,r4 */
L8c615340: r[14]+=4u;goto L8c615342; /* add #0x4,r14 */
L8c615342: r[8]+=16u;goto L8c615344; /* add #0x10,r8 */
L8c615344: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615346; /* fschg */
L8c615346: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615348; /* fmov.s @r4+,fr0 */
L8c615348: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61534a; /* fmov.s @r4+,fr2 */
L8c61534a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61534c; /* fldi0 fr3 */
L8c61534c: fcopy(f,8u,1u);goto L8c61534e; /* fmov fr1,fr8 */
L8c61534e: fcopy(f,10u,3u);goto L8c615350; /* fmov fr3,fr10 */
L8c615350: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c615352; /* fipr fv8,fv0 */
L8c615352: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615354; /* fmov.s @r4+,fr4 */
L8c615354: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615356; /* fmov.s @r4,fr6 */
L8c615356: fcopy(f,8u,5u);goto L8c615358; /* fmov fr5,fr8 */
L8c615358: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61535a; /* fschg */
L8c61535a: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c61535c; /* mov.w @r6+,r2 */
L8c61535c: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c61535e; /* mov.w @r6,r3 */
L8c61535e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615360; /* fmov.s fr6,@-r8 */
L8c615360: *fpul=r[3];goto L8c615362; /* lds r3,FPUL */
L8c615362: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615364; /* fmov.s fr5,@-r8 */
L8c615364: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c615366; /* fsub fr3,fr11 */
L8c615366: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615368; /* fmov.s fr4,@-r8 */
L8c615368: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c61536a; /* float FPUL,fr1 */
L8c61536a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61536c; /* fmul fr8,fr1 */
L8c61536c: *fpul=r[2];goto L8c61536e; /* lds r2,FPUL */
L8c61536e: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615370; /* mov.l r5,@-r8 */
L8c615370: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c615372; /* float FPUL,fr2 */
L8c615372: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615374; /* fmul fr8,fr2 */
L8c615374: r[8]+=28u;goto L8c615376; /* add #0x1c,r8 */
L8c615376: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c615378; /* fmov.s fr11,@-r8 */
L8c615378: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61537a; /* fmov.s fr1,@-r8 */
L8c61537a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61537c; /* fmov.s fr2,@-r8 */
L8c61537c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61537e; /* pref @r8 */
L8c61537e: r[8]+=16u;goto L8c615380; /* add #0x10,r8 */
L8c615380: r[3]=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;goto L8c615382; /* mov.l @(0x1c,r15),r3 */
L8c615382: r[3]+=4294967295u;goto L8c615384; /* add #-0x1,r3 */
L8c615384: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c615386; /* tst r3,r3 */
L8c615386: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x1cu,r[3],4);if(io_status)return io_status;if(take)goto L8c61538e;goto L8c61538a; /* bt/s 0x8c61538e */
L8c61538a: goto L8c614fc8; /* bra 0x8c614fc8 */
L8c61538e: r[4]=rd(m,bus,&io_status,0x8c615408u,4);if(io_status)return io_status;goto L8c615390; /* mov.l 0x8c615408,r4 */
L8c615390: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c615392; /* mov.l @(0x10,r15),r3 */
L8c615392: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c615394; /* mov.l @r4,r2 */
L8c615394: r[2]+=r[3];goto L8c615396; /* add r3,r2 */
L8c615396: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c615398; /* mov.l r2,@r4 */
L8c615398: r[4]=rd(m,bus,&io_status,0x8c61540cu,4);if(io_status)return io_status;goto L8c61539a; /* mov.l 0x8c61540c,r4 */
L8c61539a: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61539c; /* mov.l @(0x10,r15),r3 */
L8c61539c: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61539e; /* mov.l @r4,r2 */
L8c61539e: r[2]+=r[3];goto L8c6153a0; /* add r3,r2 */
L8c6153a0: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c6153a2; /* mov.l r2,@r4 */
L8c6153a2: r[15]+=80u;goto L8c6153a4; /* add #0x50,r15 */
L8c6153a4: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6153a6; /* lds.l @r15+,PR */
L8c6153a6: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6153a8; /* fmov.s @r15+,fr12 */
L8c6153a8: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6153aa; /* fmov.s @r15+,fr13 */
L8c6153aa: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6153ac; /* fmov.s @r15+,fr14 */
L8c6153ac: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6153ae; /* fmov.s @r15+,fr15 */
L8c6153ae: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6153b0; /* mov.l @r15+,r9 */
L8c6153b0: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6153b2; /* mov.l @r15+,r10 */
L8c6153b2: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6153b4; /* mov.l @r15+,r11 */
L8c6153b4: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6153b6; /* mov.l @r15+,r12 */
L8c6153b6: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6153b8; /* mov.l @r15+,r13 */
L8c6153b8: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
