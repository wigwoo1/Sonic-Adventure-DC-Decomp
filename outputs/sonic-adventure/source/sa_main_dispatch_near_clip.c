/* Triangle near-plane clipper with portable queue interface; name inferred.
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
SA_STAGE_EXPORT int sa_triangle_near_clip_8c613312(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c613312;
L8c613312: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c613314; /* mov.l r14,@-r15 */
L8c613314: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c613316; /* mov.l r13,@-r15 */
L8c613316: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c613318; /* mov.l r12,@-r15 */
L8c613318: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61331a; /* mov.l r11,@-r15 */
L8c61331a: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61331c; /* mov.l r10,@-r15 */
L8c61331c: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61331e; /* mov.l r9,@-r15 */
L8c61331e: r[15]+=4294967236u;goto L8c613320; /* add #-0x3c,r15 */
L8c613320: r[3]=rd(m,bus,&io_status,0x8c6133a4u,4);if(io_status)return io_status;goto L8c613322; /* mov.l 0x8c6133a4,r3 */
L8c613322: r[7]=r[15];goto L8c613324; /* mov r15,r7 */
L8c613324: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c613326; /* fldi1 fr6 */
L8c613326: r[12]=r[15];goto L8c613328; /* mov r15,r12 */
L8c613328: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61332a; /* fmov.s @r3,fr3 */
L8c61332a: r[7]+=48u;goto L8c61332c; /* add #0x30,r7 */
L8c61332c: fcopy(f,4u,6u);goto L8c61332e; /* fmov fr6,fr4 */
L8c61332e: r[12]+=12u;goto L8c613330; /* add #0xc,r12 */
L8c613330: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c613332; /* fdiv fr3,fr4 */
L8c613332: r[2]=r[7];goto L8c613334; /* mov r7,r2 */
L8c613334: r[14]=rd(m,bus,&io_status,0x8c6133acu,4);if(io_status)return io_status;goto L8c613336; /* mov.l 0x8c6133ac,r14 */
L8c613336: r[10]=3u;goto L8c613338; /* mov #0x3,r10 */
L8c613338: r[9]=rd(m,bus,&io_status,0x8c6133a8u,4);if(io_status)return io_status;goto L8c61333a; /* mov.l 0x8c6133a8,r9 */
L8c61333a: r[11]=0u;goto L8c61333c; /* mov #0x0,r11 */
L8c61333c: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61333e; /* mov.l r2,@r15 */
L8c61333e: wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c613340; /* mov.l r4,@r2 */
L8c613340: r[4]=1u;goto L8c613342; /* mov #0x1,r4 */
L8c613342: wr(m,bus,&io_status,r[7]+0x4u,r[5],4);if(io_status)return io_status;goto L8c613344; /* mov.l r5,@(0x4,r7) */
L8c613344: r[5]=r[12];goto L8c613346; /* mov r12,r5 */
L8c613346: wr(m,bus,&io_status,r[7]+0x8u,r[6],4);if(io_status)return io_status;goto L8c613348; /* mov.l r6,@(0x8,r7) */
L8c613348: r[13]=r[5];goto L8c61334a; /* mov r5,r13 */
L8c61334a: r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61334c; /* mov.l @r15,r6 */
L8c61334c: wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c61334e; /* mov.l r5,@r15 */
L8c61334e: r[0]=r[11];goto L8c613350; /* mov r11,r0 */
L8c613350: r[0]<<=2;goto L8c613352; /* shll2 r0 */
L8c613352: r[3]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c613354; /* mov.l @(r0,r7),r3 */
L8c613354: r[2]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c613356; /* mov.l @(0xc,r3),r2 */
L8c613356: *sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c613358; /* tst r4,r2 */
L8c613358: if(!(*sr&1u))goto L8c6133b4;goto L8c61335a; /* bf 0x8c6133b4 */
L8c61335a: r[1]=r[11];goto L8c61335c; /* mov r11,r1 */
L8c61335c: r[1]<<=2;goto L8c61335e; /* shll2 r1 */
L8c61335e: r[1]+=r[7];goto L8c613360; /* add r7,r1 */
L8c613360: fcopy(f,5u,6u);goto L8c613362; /* fmov fr6,fr5 */
L8c613362: r[0]=24u;goto L8c613364; /* mov #0x18,r0 */
L8c613364: wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c613366; /* mov.l r1,@(0x4,r15) */
L8c613366: r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c613368; /* mov.l @r1,r3 */
L8c613368: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c61336a; /* mov.l r3,@(0x8,r15) */
L8c61336a: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61336c; /* fmov.s @(r0,r3),fr3 */
L8c61336c: r[0]=12u;goto L8c61336e; /* mov #0xc,r0 */
L8c61336e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c613370; /* fdiv fr3,fr5 */
L8c613370: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613372; /* fmov.s @(r0,r14),fr3 */
L8c613372: r[0]=16u;goto L8c613374; /* mov #0x10,r0 */
L8c613374: fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c613376; /* fmov.s @(r0,r3),fr2 */
L8c613376: r[0]=16u;goto L8c613378; /* mov #0x10,r0 */
L8c613378: r[3]=rd(m,bus,&io_status,0x8c6133b0u,4);if(io_status)return io_status;goto L8c61337a; /* mov.l 0x8c6133b0,r3 */
L8c61337a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61337c; /* fsub fr3,fr2 */
L8c61337c: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61337e; /* fmov.s @r3,fr3 */
L8c61337e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613380; /* fmul fr5,fr2 */
L8c613380: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613382; /* fdiv fr3,fr2 */
L8c613382: fstore(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c613384; /* fmov.s fr2,@r13 */
L8c613384: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613386; /* mov.l @(0x4,r15),r2 */
L8c613386: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613388; /* fmov.s @(r0,r14),fr3 */
L8c613388: r[0]=20u;goto L8c61338a; /* mov #0x14,r0 */
L8c61338a: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61338c; /* mov.l @r2,r1 */
L8c61338c: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61338e; /* fmov.s @(r0,r1),fr2 */
L8c61338e: r[0]=4u;goto L8c613390; /* mov #0x4,r0 */
L8c613390: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613392; /* fsub fr3,fr2 */
L8c613392: fload(m,bus,&io_status,f,3u,r[9]);if(io_status)return io_status;goto L8c613394; /* fmov.s @r9,fr3 */
L8c613394: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613396; /* fmul fr5,fr2 */
L8c613396: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613398; /* fdiv fr3,fr2 */
L8c613398: fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61339a; /* fmov fr2,@(r0,r13) */
L8c61339a: r[0]=8u;goto L8c61339c; /* mov #0x8,r0 */
L8c61339c: fstore(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c6133d0; /* bra 0x8c6133d0 */
L8c6133b4: r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6133b6; /* mov.l @r6,r3 */
L8c6133b6: r[0]=16u;goto L8c6133b8; /* mov #0x10,r0 */
L8c6133b8: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6133ba; /* fmov.s @(r0,r3),fr3 */
L8c6133ba: r[0]=20u;goto L8c6133bc; /* mov #0x14,r0 */
L8c6133bc: fstore(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c6133be; /* fmov.s fr3,@r5 */
L8c6133be: r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6133c0; /* mov.l @r6,r3 */
L8c6133c0: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6133c2; /* fmov.s @(r0,r3),fr3 */
L8c6133c2: r[0]=4u;goto L8c6133c4; /* mov #0x4,r0 */
L8c6133c4: fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6133c6; /* fmov fr3,@(r0,r5) */
L8c6133c6: r[0]=24u;goto L8c6133c8; /* mov #0x18,r0 */
L8c6133c8: r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6133ca; /* mov.l @r6,r3 */
L8c6133ca: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6133cc; /* fmov.s @(r0,r3),fr3 */
L8c6133cc: r[0]=8u;goto L8c6133ce; /* mov #0x8,r0 */
L8c6133ce: fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6133d0; /* fmov fr3,@(r0,r5) */
L8c6133d0: r[11]+=1u;goto L8c6133d2; /* add #0x1,r11 */
L8c6133d2: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[10]);goto L8c6133d4; /* cmp/ge r10,r11 */
L8c6133d4: r[5]+=12u;goto L8c6133d6; /* add #0xc,r5 */
L8c6133d6: r[6]+=4u;goto L8c6133d8; /* add #0x4,r6 */
L8c6133d8: take=!(*sr&1u);r[13]+=12u;if(take)goto L8c61334e;goto L8c6133dc; /* bf/s 0x8c61334e */
L8c6133dc: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6133de; /* mov.l @r15,r2 */
L8c6133de: r[0]=8u;goto L8c6133e0; /* mov #0x8,r0 */
L8c6133e0: r[5]=r[12];goto L8c6133e2; /* mov r12,r5 */
L8c6133e2: r[5]+=12u;goto L8c6133e4; /* add #0xc,r5 */
L8c6133e4: fload(m,bus,&io_status,f,7u,r[0]+r[2]);if(io_status)return io_status;goto L8c6133e6; /* fmov.s @(r0,r2),fr7 */
L8c6133e6: r[0]=4u;goto L8c6133e8; /* mov #0x4,r0 */
L8c6133e8: r[3]=r[2];goto L8c6133ea; /* mov r2,r3 */
L8c6133ea: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c6133ec; /* fmov.s @(r0,r5),fr5 */
L8c6133ec: fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c6133ee; /* fmov.s @(r0,r3),fr9 */
L8c6133ee: r[0]=8u;goto L8c6133f0; /* mov #0x8,r0 */
L8c6133f0: fload(m,bus,&io_status,f,6u,r[0]+r[5]);if(io_status)return io_status;goto L8c6133f2; /* fmov.s @(r0,r5),fr6 */
L8c6133f2: r[6]=r[12];goto L8c6133f4; /* mov r12,r6 */
L8c6133f4: fcopy(f,3u,9u);goto L8c6133f6; /* fmov fr9,fr3 */
L8c6133f6: r[0]=4u;goto L8c6133f8; /* mov #0x4,r0 */
L8c6133f8: fcopy(f,0u,7u);goto L8c6133fa; /* fmov fr7,fr0 */
L8c6133fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6133fc; /* fmul fr6,fr3 */
L8c6133fc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6133fe; /* fmul fr5,fr0 */
L8c6133fe: fload(m,bus,&io_status,f,10u,r[3]);if(io_status)return io_status;goto L8c613400; /* fmov.s @r3,fr10 */
L8c613400: fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;goto L8c613402; /* fmov.s @r5,fr8 */
L8c613402: r[6]+=24u;goto L8c613404; /* add #0x18,r6 */
L8c613404: fcopy(f,2u,7u);goto L8c613406; /* fmov fr7,fr2 */
L8c613406: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613408; /* fmul fr8,fr2 */
L8c613408: fcopy(f,1u,8u);goto L8c61340a; /* fmov fr8,fr1 */
L8c61340a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c61340c; /* fsub fr3,fr0 */
L8c61340c: fcopy(f,3u,10u);goto L8c61340e; /* fmov fr10,fr3 */
L8c61340e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c613410; /* fmul fr6,fr3 */
L8c613410: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[9],&f->fpscr,&fr[1]);if(status)return status;}goto L8c613412; /* fmul fr9,fr1 */
L8c613412: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613414; /* fsub fr2,fr3 */
L8c613414: fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c613416; /* fmov.s @(r0,r6),fr2 */
L8c613416: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613418; /* fmul fr3,fr2 */
L8c613418: fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c61341a; /* fmov.s @r6,fr3 */
L8c61341a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61341c; /* fmac fr0,fr3,fr2 */
L8c61341c: fcopy(f,11u,10u);goto L8c61341e; /* fmov fr10,fr11 */
L8c61341e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[5],&f->fpscr,&fr[11]);if(status)return status;}goto L8c613420; /* fmul fr5,fr11 */
L8c613420: r[0]=8u;goto L8c613422; /* mov #0x8,r0 */
L8c613422: r[5]=rd(m,bus,&io_status,0x8c6136f8u,4);if(io_status)return io_status;goto L8c613424; /* mov.l 0x8c6136f8,r5 */
L8c613424: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c613426; /* fmov.s @(r0,r6),fr3 */
L8c613426: r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c613428; /* mov.l @r5,r3 */
L8c613428: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[11],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61342a; /* fsub fr11,fr1 */
L8c61342a: fcopy(f,0u,1u);goto L8c61342c; /* fmov fr1,fr0 */
L8c61342c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61342e; /* fmac fr0,fr3,fr2 */
L8c61342e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613430; /* fldi0 fr3 */
L8c613430: fcopy(f,5u,2u);goto L8c613432; /* fmov fr2,fr5 */
L8c613432: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c613434; /* fcmp/gt fr5,fr3 */
L8c613434: r[0]=*sr&1u;goto L8c613436; /* movt r0 */
L8c613436: *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c613438; /* tst r3,r0 */
L8c613438: if((*sr&1u))goto L8c61343e;goto L8c61343a; /* bt 0x8c61343e */
L8c61343a: goto L8c6136b0; /* bra 0x8c6136b0 */
L8c61343e: r[1]=0u;goto L8c613440; /* mov #0x0,r1 */
L8c613440: wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c613442; /* mov.l r1,@r5 */
L8c613442: r[6]=r[1];goto L8c613444; /* mov r1,r6 */
L8c613444: r[5]=rd(m,bus,&io_status,0x8c6136fcu,4);if(io_status)return io_status;goto L8c613446; /* mov.l 0x8c6136fc,r5 */
L8c613446: r[13]=r[1];goto L8c613448; /* mov r1,r13 */
L8c613448: r[11]=2u;goto L8c61344a; /* mov #0x2,r11 */
L8c61344a: r[3]=r[6];goto L8c61344c; /* mov r6,r3 */
L8c61344c: r[3]<<=2;goto L8c61344e; /* shll2 r3 */
L8c61344e: r[0]=r[11];goto L8c613450; /* mov r11,r0 */
L8c613450: r[0]<<=2;goto L8c613452; /* shll2 r0 */
L8c613452: r[3]+=r[7];goto L8c613454; /* add r7,r3 */
L8c613454: r[0]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c613456; /* mov.l @(r0,r7),r0 */
L8c613456: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c613458; /* mov.l @r3,r2 */
L8c613458: r[0]=rd(m,bus,&io_status,r[0]+0xcu,4);if(io_status)return io_status;goto L8c61345a; /* mov.l @(0xc,r0),r0 */
L8c61345a: if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[1]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c61345c; /* mov.l @(0xc,r2),r1 */
L8c61345c: r[0]&=r[4];goto L8c61345e; /* and r4,r0 */
L8c61345e: r[1]&=r[4];goto L8c613460; /* and r4,r1 */
L8c613460: r[0]^=r[1];goto L8c613462; /* xor r1,r0 */
L8c613462: *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c613464; /* cmp/eq #0x1,r0 */
L8c613464: if(!(*sr&1u))goto L8c6134ec;goto L8c613466; /* bf 0x8c6134ec */
L8c613466: r[3]=r[11];goto L8c613468; /* mov r11,r3 */
L8c613468: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61346a; /* shll r11 */
L8c61346a: r[2]=r[6];goto L8c61346c; /* mov r6,r2 */
L8c61346c: r[11]+=r[3];goto L8c61346e; /* add r3,r11 */
L8c61346e: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c613470; /* shll r2 */
L8c613470: r[1]=r[6];goto L8c613472; /* mov r6,r1 */
L8c613472: r[11]<<=2;goto L8c613474; /* shll2 r11 */
L8c613474: r[3]=rd(m,bus,&io_status,0x8c613700u,4);if(io_status)return io_status;goto L8c613476; /* mov.l 0x8c613700,r3 */
L8c613476: r[11]+=r[12];goto L8c613478; /* add r12,r11 */
L8c613478: r[0]=8u;goto L8c61347a; /* mov #0x8,r0 */
L8c61347a: wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61347c; /* mov.l r11,@r15 */
L8c61347c: r[2]+=r[1];goto L8c61347e; /* add r1,r2 */
L8c61347e: fload(m,bus,&io_status,f,5u,r[0]+r[11]);if(io_status)return io_status;goto L8c613480; /* fmov.s @(r0,r11),fr5 */
L8c613480: r[2]<<=2;goto L8c613482; /* shll2 r2 */
L8c613482: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c613484; /* fmov.s @r3,fr3 */
L8c613484: r[2]+=r[12];goto L8c613486; /* add r12,r2 */
L8c613486: r[0]=8u;goto L8c613488; /* mov #0x8,r0 */
L8c613488: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61348a; /* fsub fr5,fr3 */
L8c61348a: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c61348c; /* mov.l r2,@(0x4,r15) */
L8c61348c: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61348e; /* fmov.s @(r0,r2),fr2 */
L8c61348e: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613490; /* mov.l @(0x4,r15),r1 */
L8c613490: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613492; /* fsub fr5,fr2 */
L8c613492: fcopy(f,5u,3u);goto L8c613494; /* fmov fr3,fr5 */
L8c613494: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613496; /* mov.l @r15,r2 */
L8c613496: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c613498; /* fmov.s @r1,fr3 */
L8c613498: fload(m,bus,&io_status,f,6u,r[2]);if(io_status)return io_status;goto L8c61349a; /* fmov.s @r2,fr6 */
L8c61349a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[2],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61349c; /* fdiv fr2,fr5 */
L8c61349c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61349e; /* fsub fr6,fr3 */
L8c61349e: fcopy(f,2u,6u);goto L8c6134a0; /* fmov fr6,fr2 */
L8c6134a0: fcopy(f,0u,5u);goto L8c6134a2; /* fmov fr5,fr0 */
L8c6134a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6134a4; /* fmac fr0,fr3,fr2 */
L8c6134a4: fcopy(f,6u,2u);goto L8c6134a6; /* fmov fr2,fr6 */
L8c6134a6: r[2]=rd(m,bus,&io_status,0x8c613704u,4);if(io_status)return io_status;goto L8c6134a8; /* mov.l 0x8c613704,r2 */
L8c6134a8: r[0]=12u;goto L8c6134aa; /* mov #0xc,r0 */
L8c6134aa: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c6134ac; /* fmov.s @(r0,r14),fr2 */
L8c6134ac: r[11]=r[13];goto L8c6134ae; /* mov r13,r11 */
L8c6134ae: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6134b0; /* fmov.s @r2,fr3 */
L8c6134b0: r[11]<<=2;goto L8c6134b2; /* shll2 r11 */
L8c6134b2: fcopy(f,0u,4u);goto L8c6134b4; /* fmov fr4,fr0 */
L8c6134b4: r[11]<<=2;goto L8c6134b6; /* shll2 r11 */
L8c6134b6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6134b8; /* fmul fr6,fr3 */
L8c6134b8: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c6134ba; /* shll r11 */
L8c6134ba: r[0]=16u;goto L8c6134bc; /* mov #0x10,r0 */
L8c6134bc: r[11]+=r[5];goto L8c6134be; /* add r5,r11 */
L8c6134be: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6134c0; /* fmac fr0,fr3,fr2 */
L8c6134c0: fcopy(f,0u,5u);goto L8c6134c2; /* fmov fr5,fr0 */
L8c6134c2: fstore(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c6134c4; /* fmov fr2,@(r0,r11) */
L8c6134c4: r[0]=4u;goto L8c6134c6; /* mov #0x4,r0 */
L8c6134c6: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6134c8; /* mov.l @r15,r1 */
L8c6134c8: fload(m,bus,&io_status,f,6u,r[0]+r[1]);if(io_status)return io_status;goto L8c6134ca; /* fmov.s @(r0,r1),fr6 */
L8c6134ca: r[0]=4u;goto L8c6134cc; /* mov #0x4,r0 */
L8c6134cc: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6134ce; /* mov.l @(0x4,r15),r1 */
L8c6134ce: fcopy(f,5u,6u);goto L8c6134d0; /* fmov fr6,fr5 */
L8c6134d0: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6134d2; /* fmov.s @(r0,r1),fr3 */
L8c6134d2: r[0]=16u;goto L8c6134d4; /* mov #0x10,r0 */
L8c6134d4: fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c6134d6; /* fmov.s @(r0,r14),fr2 */
L8c6134d6: r[0]=20u;goto L8c6134d8; /* mov #0x14,r0 */
L8c6134d8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6134da; /* fsub fr6,fr3 */
L8c6134da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[5],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6134dc; /* fmac fr0,fr3,fr5 */
L8c6134dc: fload(m,bus,&io_status,f,3u,r[9]);if(io_status)return io_status;goto L8c6134de; /* fmov.s @r9,fr3 */
L8c6134de: fcopy(f,0u,4u);goto L8c6134e0; /* fmov fr4,fr0 */
L8c6134e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6134e2; /* fmul fr5,fr3 */
L8c6134e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6134e4; /* fmac fr0,fr3,fr2 */
L8c6134e4: fstore(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c6134e6; /* fmov fr2,@(r0,r11) */
L8c6134e6: r[0]=24u;goto L8c6134e8; /* mov #0x18,r0 */
L8c6134e8: r[13]+=1u;goto L8c6134ea; /* add #0x1,r13 */
L8c6134ea: fstore(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c6134ec; /* fmov fr0,@(r0,r11) */
L8c6134ec: r[11]=r[6];goto L8c6134ee; /* mov r6,r11 */
L8c6134ee: r[11]<<=2;goto L8c6134f0; /* shll2 r11 */
L8c6134f0: r[11]+=r[7];goto L8c6134f2; /* add r7,r11 */
L8c6134f2: r[3]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c6134f4; /* mov.l @r11,r3 */
L8c6134f4: r[2]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c6134f6; /* mov.l @(0xc,r3),r2 */
L8c6134f6: *sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c6134f8; /* tst r4,r2 */
L8c6134f8: if(!(*sr&1u))goto L8c613524;goto L8c6134fa; /* bf 0x8c613524 */
L8c6134fa: r[1]=r[13];goto L8c6134fc; /* mov r13,r1 */
L8c6134fc: r[1]<<=2;goto L8c6134fe; /* shll2 r1 */
L8c6134fe: r[0]=16u;goto L8c613500; /* mov #0x10,r0 */
L8c613500: r[1]<<=2;goto L8c613502; /* shll2 r1 */
L8c613502: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c613504; /* shll r1 */
L8c613504: r[1]+=r[5];goto L8c613506; /* add r5,r1 */
L8c613506: r[13]+=1u;goto L8c613508; /* add #0x1,r13 */
L8c613508: wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c61350a; /* mov.l r1,@r15 */
L8c61350a: r[3]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c61350c; /* mov.l @r11,r3 */
L8c61350c: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61350e; /* fmov.s @(r0,r3),fr3 */
L8c61350e: fstore(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c613510; /* fmov fr3,@(r0,r1) */
L8c613510: r[0]=20u;goto L8c613512; /* mov #0x14,r0 */
L8c613512: r[2]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c613514; /* mov.l @r11,r2 */
L8c613514: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613516; /* mov.l @r15,r3 */
L8c613516: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c613518; /* fmov.s @(r0,r2),fr3 */
L8c613518: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61351a; /* fmov fr3,@(r0,r3) */
L8c61351a: r[0]=24u;goto L8c61351c; /* mov #0x18,r0 */
L8c61351c: r[2]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c61351e; /* mov.l @r11,r2 */
L8c61351e: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613520; /* mov.l @r15,r3 */
L8c613520: fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c613522; /* fmov.s @(r0,r2),fr3 */
L8c613522: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c613524; /* fmov fr3,@(r0,r3) */
L8c613524: r[11]=r[6];goto L8c613526; /* mov r6,r11 */
L8c613526: r[6]+=1u;goto L8c613528; /* add #0x1,r6 */
L8c613528: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[10]);goto L8c61352a; /* cmp/ge r10,r6 */
L8c61352a: if(!(*sr&1u))goto L8c61344a;goto L8c61352c; /* bf 0x8c61344a */
L8c61352c: r[4]=rd(m,bus,&io_status,0x8c613710u,4);if(io_status)return io_status;goto L8c61352e; /* mov.l 0x8c613710,r4 */
L8c61352e: r[0]=r[13];goto L8c613530; /* mov r13,r0 */
L8c613530: r[7]=rd(m,bus,&io_status,0x8c613708u,4);if(io_status)return io_status;goto L8c613532; /* mov.l 0x8c613708,r7 */
L8c613532: *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c613534; /* cmp/eq #0x3,r0 */
L8c613534: r[6]=rd(m,bus,&io_status,0x8c61370cu,4);if(io_status)return io_status;goto L8c613536; /* mov.l 0x8c61370c,r6 */
L8c613536: if(!(*sr&1u))goto L8c6135da;goto L8c613538; /* bf 0x8c6135da */
L8c613538: r[0]=28u;goto L8c61353a; /* mov #0x1c,r0 */
L8c61353a: r[14]=r[5];goto L8c61353c; /* mov r5,r14 */
L8c61353c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61353e; /* fmov.s @(r0,r4),fr3 */
L8c61353e: r[8]+=32u;goto L8c613540; /* add #0x20,r8 */
L8c613540: r[0]=24u;goto L8c613542; /* mov #0x18,r0 */
L8c613542: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613544; /* fmov.s fr3,@-r8 */
L8c613544: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613546; /* fmov.s @(r0,r4),fr3 */
L8c613546: r[0]=20u;goto L8c613548; /* mov #0x14,r0 */
L8c613548: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61354a; /* fmov.s fr3,@-r8 */
L8c61354a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61354c; /* fmov.s @(r0,r4),fr3 */
L8c61354c: r[0]=16u;goto L8c61354e; /* mov #0x10,r0 */
L8c61354e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613550; /* fmov.s fr3,@-r8 */
L8c613550: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613552; /* fmov.s @(r0,r4),fr3 */
L8c613552: r[0]=24u;goto L8c613554; /* mov #0x18,r0 */
L8c613554: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613556; /* fmov.s fr3,@-r8 */
L8c613556: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613558; /* fmov.s @(r0,r14),fr3 */
L8c613558: r[0]=20u;goto L8c61355a; /* mov #0x14,r0 */
L8c61355a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61355c; /* fmov.s fr3,@-r8 */
L8c61355c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61355e; /* fmov.s @(r0,r14),fr3 */
L8c61355e: r[0]=16u;goto L8c613560; /* mov #0x10,r0 */
L8c613560: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613562; /* fmov.s fr3,@-r8 */
L8c613562: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613564; /* fmov.s @(r0,r14),fr3 */
L8c613564: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613566; /* fmov.s fr3,@-r8 */
L8c613566: r[8]-=4;wr(m,bus,&io_status,r[8],r[7],4);if(io_status)return io_status;goto L8c613568; /* mov.l r7,@-r8 */
L8c613568: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61356a; /* pref @r8 */
L8c61356a: r[0]=28u;goto L8c61356c; /* mov #0x1c,r0 */
L8c61356c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61356e; /* fmov.s @(r0,r4),fr3 */
L8c61356e: r[8]+=64u;goto L8c613570; /* add #0x40,r8 */
L8c613570: r[0]=24u;goto L8c613572; /* mov #0x18,r0 */
L8c613572: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613574; /* fmov.s fr3,@-r8 */
L8c613574: r[14]+=32u;goto L8c613576; /* add #0x20,r14 */
L8c613576: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613578; /* fmov.s @(r0,r4),fr3 */
L8c613578: r[0]=20u;goto L8c61357a; /* mov #0x14,r0 */
L8c61357a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61357c; /* fmov.s fr3,@-r8 */
L8c61357c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61357e; /* fmov.s @(r0,r4),fr3 */
L8c61357e: r[0]=16u;goto L8c613580; /* mov #0x10,r0 */
L8c613580: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613582; /* fmov.s fr3,@-r8 */
L8c613582: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613584; /* fmov.s @(r0,r4),fr3 */
L8c613584: r[0]=24u;goto L8c613586; /* mov #0x18,r0 */
L8c613586: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613588; /* fmov.s fr3,@-r8 */
L8c613588: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61358a; /* fmov.s @(r0,r14),fr3 */
L8c61358a: r[0]=20u;goto L8c61358c; /* mov #0x14,r0 */
L8c61358c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61358e; /* fmov.s fr3,@-r8 */
L8c61358e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613590; /* fmov.s @(r0,r14),fr3 */
L8c613590: r[0]=16u;goto L8c613592; /* mov #0x10,r0 */
L8c613592: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613594; /* fmov.s fr3,@-r8 */
L8c613594: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613596; /* fmov.s @(r0,r14),fr3 */
L8c613596: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613598; /* fmov.s fr3,@-r8 */
L8c613598: r[8]-=4;wr(m,bus,&io_status,r[8],r[7],4);if(io_status)return io_status;goto L8c61359a; /* mov.l r7,@-r8 */
L8c61359a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61359c; /* pref @r8 */
L8c61359c: r[0]=28u;goto L8c61359e; /* mov #0x1c,r0 */
L8c61359e: r[3]=rd(m,bus,&io_status,0x8c613714u,4);if(io_status)return io_status;goto L8c6135a0; /* mov.l 0x8c613714,r3 */
L8c6135a0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135a2; /* fmov.s @(r0,r4),fr3 */
L8c6135a2: r[8]+=64u;goto L8c6135a4; /* add #0x40,r8 */
L8c6135a4: r[0]=24u;goto L8c6135a6; /* mov #0x18,r0 */
L8c6135a6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135a8; /* fmov.s fr3,@-r8 */
L8c6135a8: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135aa; /* fmov.s @(r0,r4),fr3 */
L8c6135aa: r[0]=20u;goto L8c6135ac; /* mov #0x14,r0 */
L8c6135ac: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135ae; /* fmov.s fr3,@-r8 */
L8c6135ae: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135b0; /* fmov.s @(r0,r4),fr3 */
L8c6135b0: r[0]=16u;goto L8c6135b2; /* mov #0x10,r0 */
L8c6135b2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135b4; /* fmov.s fr3,@-r8 */
L8c6135b4: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135b6; /* fmov.s @(r0,r4),fr3 */
L8c6135b6: r[4]=r[5];goto L8c6135b8; /* mov r5,r4 */
L8c6135b8: r[0]=24u;goto L8c6135ba; /* mov #0x18,r0 */
L8c6135ba: r[4]+=64u;goto L8c6135bc; /* add #0x40,r4 */
L8c6135bc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135be; /* fmov.s fr3,@-r8 */
L8c6135be: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135c0; /* fmov.s @(r0,r4),fr3 */
L8c6135c0: r[0]=20u;goto L8c6135c2; /* mov #0x14,r0 */
L8c6135c2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135c4; /* fmov.s fr3,@-r8 */
L8c6135c4: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135c6; /* fmov.s @(r0,r4),fr3 */
L8c6135c6: r[0]=16u;goto L8c6135c8; /* mov #0x10,r0 */
L8c6135c8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135ca; /* fmov.s fr3,@-r8 */
L8c6135ca: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135cc; /* fmov.s @(r0,r4),fr3 */
L8c6135cc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135ce; /* fmov.s fr3,@-r8 */
L8c6135ce: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6135d0; /* mov.l r3,@-r8 */
L8c6135d0: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6135d2; /* pref @r8 */
L8c6135d2: r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6135d4; /* mov.l @r6,r2 */
L8c6135d4: r[8]+=32u;goto L8c6135d6; /* add #0x20,r8 */
L8c6135d6: r[2]+=3u;goto L8c6136ae; /* bra 0x8c6136ae */
L8c6135da: r[0]=28u;goto L8c6135dc; /* mov #0x1c,r0 */
L8c6135dc: r[14]=r[5];goto L8c6135de; /* mov r5,r14 */
L8c6135de: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135e0; /* fmov.s @(r0,r4),fr3 */
L8c6135e0: r[8]+=32u;goto L8c6135e2; /* add #0x20,r8 */
L8c6135e2: r[0]=24u;goto L8c6135e4; /* mov #0x18,r0 */
L8c6135e4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135e6; /* fmov.s fr3,@-r8 */
L8c6135e6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135e8; /* fmov.s @(r0,r4),fr3 */
L8c6135e8: r[0]=20u;goto L8c6135ea; /* mov #0x14,r0 */
L8c6135ea: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135ec; /* fmov.s fr3,@-r8 */
L8c6135ec: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135ee; /* fmov.s @(r0,r4),fr3 */
L8c6135ee: r[0]=16u;goto L8c6135f0; /* mov #0x10,r0 */
L8c6135f0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135f2; /* fmov.s fr3,@-r8 */
L8c6135f2: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135f4; /* fmov.s @(r0,r4),fr3 */
L8c6135f4: r[0]=24u;goto L8c6135f6; /* mov #0x18,r0 */
L8c6135f6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135f8; /* fmov.s fr3,@-r8 */
L8c6135f8: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6135fa; /* fmov.s @(r0,r14),fr3 */
L8c6135fa: r[0]=20u;goto L8c6135fc; /* mov #0x14,r0 */
L8c6135fc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135fe; /* fmov.s fr3,@-r8 */
L8c6135fe: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613600; /* fmov.s @(r0,r14),fr3 */
L8c613600: r[0]=16u;goto L8c613602; /* mov #0x10,r0 */
L8c613602: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613604; /* fmov.s fr3,@-r8 */
L8c613604: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613606; /* fmov.s @(r0,r14),fr3 */
L8c613606: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613608; /* fmov.s fr3,@-r8 */
L8c613608: r[8]-=4;wr(m,bus,&io_status,r[8],r[7],4);if(io_status)return io_status;goto L8c61360a; /* mov.l r7,@-r8 */
L8c61360a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61360c; /* pref @r8 */
L8c61360c: r[0]=28u;goto L8c61360e; /* mov #0x1c,r0 */
L8c61360e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613610; /* fmov.s @(r0,r4),fr3 */
L8c613610: r[8]+=64u;goto L8c613612; /* add #0x40,r8 */
L8c613612: r[0]=24u;goto L8c613614; /* mov #0x18,r0 */
L8c613614: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613616; /* fmov.s fr3,@-r8 */
L8c613616: r[14]+=32u;goto L8c613618; /* add #0x20,r14 */
L8c613618: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61361a; /* fmov.s @(r0,r4),fr3 */
L8c61361a: r[0]=20u;goto L8c61361c; /* mov #0x14,r0 */
L8c61361c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61361e; /* fmov.s fr3,@-r8 */
L8c61361e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613620; /* fmov.s @(r0,r4),fr3 */
L8c613620: r[0]=16u;goto L8c613622; /* mov #0x10,r0 */
L8c613622: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613624; /* fmov.s fr3,@-r8 */
L8c613624: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613626; /* fmov.s @(r0,r4),fr3 */
L8c613626: r[0]=24u;goto L8c613628; /* mov #0x18,r0 */
L8c613628: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61362a; /* fmov.s fr3,@-r8 */
L8c61362a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61362c; /* fmov.s @(r0,r14),fr3 */
L8c61362c: r[0]=20u;goto L8c61362e; /* mov #0x14,r0 */
L8c61362e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613630; /* fmov.s fr3,@-r8 */
L8c613630: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613632; /* fmov.s @(r0,r14),fr3 */
L8c613632: r[0]=16u;goto L8c613634; /* mov #0x10,r0 */
L8c613634: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613636; /* fmov.s fr3,@-r8 */
L8c613636: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613638; /* fmov.s @(r0,r14),fr3 */
L8c613638: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61363a; /* fmov.s fr3,@-r8 */
L8c61363a: r[8]-=4;wr(m,bus,&io_status,r[8],r[7],4);if(io_status)return io_status;goto L8c61363c; /* mov.l r7,@-r8 */
L8c61363c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61363e; /* pref @r8 */
L8c61363e: r[0]=28u;goto L8c613640; /* mov #0x1c,r0 */
L8c613640: r[14]=r[5];goto L8c613642; /* mov r5,r14 */
L8c613642: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613644; /* fmov.s @(r0,r4),fr3 */
L8c613644: r[8]+=64u;goto L8c613646; /* add #0x40,r8 */
L8c613646: r[0]=24u;goto L8c613648; /* mov #0x18,r0 */
L8c613648: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61364a; /* fmov.s fr3,@-r8 */
L8c61364a: r[14]+=96u;goto L8c61364c; /* add #0x60,r14 */
L8c61364c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61364e; /* fmov.s @(r0,r4),fr3 */
L8c61364e: r[0]=20u;goto L8c613650; /* mov #0x14,r0 */
L8c613650: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613652; /* fmov.s fr3,@-r8 */
L8c613652: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613654; /* fmov.s @(r0,r4),fr3 */
L8c613654: r[0]=16u;goto L8c613656; /* mov #0x10,r0 */
L8c613656: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613658; /* fmov.s fr3,@-r8 */
L8c613658: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61365a; /* fmov.s @(r0,r4),fr3 */
L8c61365a: r[0]=24u;goto L8c61365c; /* mov #0x18,r0 */
L8c61365c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61365e; /* fmov.s fr3,@-r8 */
L8c61365e: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613660; /* fmov.s @(r0,r14),fr3 */
L8c613660: r[0]=20u;goto L8c613662; /* mov #0x14,r0 */
L8c613662: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613664; /* fmov.s fr3,@-r8 */
L8c613664: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613666; /* fmov.s @(r0,r14),fr3 */
L8c613666: r[0]=16u;goto L8c613668; /* mov #0x10,r0 */
L8c613668: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61366a; /* fmov.s fr3,@-r8 */
L8c61366a: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61366c; /* fmov.s @(r0,r14),fr3 */
L8c61366c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61366e; /* fmov.s fr3,@-r8 */
L8c61366e: r[8]-=4;wr(m,bus,&io_status,r[8],r[7],4);if(io_status)return io_status;goto L8c613670; /* mov.l r7,@-r8 */
L8c613670: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613672; /* pref @r8 */
L8c613672: r[0]=28u;goto L8c613674; /* mov #0x1c,r0 */
L8c613674: r[3]=rd(m,bus,&io_status,0x8c613714u,4);if(io_status)return io_status;goto L8c613676; /* mov.l 0x8c613714,r3 */
L8c613676: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613678; /* fmov.s @(r0,r4),fr3 */
L8c613678: r[8]+=64u;goto L8c61367a; /* add #0x40,r8 */
L8c61367a: r[0]=24u;goto L8c61367c; /* mov #0x18,r0 */
L8c61367c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61367e; /* fmov.s fr3,@-r8 */
L8c61367e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613680; /* fmov.s @(r0,r4),fr3 */
L8c613680: r[0]=20u;goto L8c613682; /* mov #0x14,r0 */
L8c613682: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613684; /* fmov.s fr3,@-r8 */
L8c613684: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613686; /* fmov.s @(r0,r4),fr3 */
L8c613686: r[0]=16u;goto L8c613688; /* mov #0x10,r0 */
L8c613688: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61368a; /* fmov.s fr3,@-r8 */
L8c61368a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61368c; /* fmov.s @(r0,r4),fr3 */
L8c61368c: r[4]=r[5];goto L8c61368e; /* mov r5,r4 */
L8c61368e: r[0]=24u;goto L8c613690; /* mov #0x18,r0 */
L8c613690: r[4]+=64u;goto L8c613692; /* add #0x40,r4 */
L8c613692: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613694; /* fmov.s fr3,@-r8 */
L8c613694: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613696; /* fmov.s @(r0,r4),fr3 */
L8c613696: r[0]=20u;goto L8c613698; /* mov #0x14,r0 */
L8c613698: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61369a; /* fmov.s fr3,@-r8 */
L8c61369a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61369c; /* fmov.s @(r0,r4),fr3 */
L8c61369c: r[0]=16u;goto L8c61369e; /* mov #0x10,r0 */
L8c61369e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6136a0; /* fmov.s fr3,@-r8 */
L8c6136a0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6136a2; /* fmov.s @(r0,r4),fr3 */
L8c6136a2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6136a4; /* fmov.s fr3,@-r8 */
L8c6136a4: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6136a6; /* mov.l r3,@-r8 */
L8c6136a6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6136a8; /* pref @r8 */
L8c6136a8: r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6136aa; /* mov.l @r6,r2 */
L8c6136aa: r[8]+=32u;goto L8c6136ac; /* add #0x20,r8 */
L8c6136ac: r[2]+=4u;goto L8c6136ae; /* add #0x4,r2 */
L8c6136ae: wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c6136b0; /* mov.l r2,@r6 */
L8c6136b0: r[15]+=60u;goto L8c6136b2; /* add #0x3c,r15 */
L8c6136b2: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6136b4; /* mov.l @r15+,r9 */
L8c6136b4: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6136b6; /* mov.l @r15+,r10 */
L8c6136b6: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6136b8; /* mov.l @r15+,r11 */
L8c6136b8: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6136ba; /* mov.l @r15+,r12 */
L8c6136ba: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6136bc; /* mov.l @r15+,r13 */
L8c6136bc: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
