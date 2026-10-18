/* UV-face alternate strip emitters with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_uv_face_alternate_strip_8c614076(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c614076;
L8c614076: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c614078; /* mov.l r14,@-r15 */
L8c614078: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61407a; /* mov.l r13,@-r15 */
L8c61407a: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61407c; /* mov.l r12,@-r15 */
L8c61407c: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61407e; /* mov.l r11,@-r15 */
L8c61407e: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c614080; /* mov.l r10,@-r15 */
L8c614080: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c614082; /* mov.l r9,@-r15 */
L8c614082: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c614084; /* fmov.s fr15,@-r15 */
L8c614084: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c614086; /* fmov.s fr14,@-r15 */
L8c614086: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c614088; /* fmov.s fr13,@-r15 */
L8c614088: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61408a; /* sts.l PR,@-r15 */
L8c61408a: r[15]+=4294967276u;goto L8c61408c; /* add #-0x14,r15 */
L8c61408c: r[3]=rd(m,bus,&io_status,0x8c6140c0u,4);if(io_status)return io_status;goto L8c61408e; /* mov.l 0x8c6140c0,r3 */
L8c61408e: r[9]=rd(m,bus,&io_status,0x8c6140bcu,4);if(io_status)return io_status;goto L8c614090; /* mov.l 0x8c6140bc,r9 */
L8c614090: target=r[3];*pr=0x8c614094u;r[12]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c614094; /* jsr @r3 */
L8c614094: r[2]=rd(m,bus,&io_status,0x8c6140c4u,4);if(io_status)return io_status;goto L8c614096; /* mov.l 0x8c6140c4,r2 */
L8c614096: r[1]=0u;goto L8c614098; /* mov #0x0,r1 */
L8c614098: r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c61409a; /* mov.w @(0x2,r12),r0 */
L8c61409a: r[14]=rd(m,bus,&io_status,r[12]+0x14u,4);if(io_status)return io_status;goto L8c61409c; /* mov.l @(0x14,r12),r14 */
L8c61409c: r[13]=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;goto L8c61409e; /* mov.l @(0x4,r12),r13 */
L8c61409e: r[0]=r[0]&65535u;goto L8c6140a0; /* extu.w r0,r0 */
L8c6140a0: wr(m,bus,&io_status,r[15]+0x10u,r[0],4);if(io_status)return io_status;goto L8c6140a2; /* mov.l r0,@(0x10,r15) */
L8c6140a2: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6140a4; /* mov.l @r2,r3 */
L8c6140a4: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6140a6; /* mov.l r3,@(0x4,r15) */
L8c6140a6: wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c6140a8; /* mov.l r1,@(0x8,r15) */
L8c6140a8: wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c6140aa; /* mov.l r1,@(0xc,r15) */
L8c6140aa: goto L8c6140c8; /* bra 0x8c6140c8 */
L8c6140c8: r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6140ca; /* mov.w @r13+,r3 */
L8c6140ca: r[4]=rd(m,bus,&io_status,0x8c6141bcu,4);if(io_status)return io_status;goto L8c6140cc; /* mov.l 0x8c6141bc,r4 */
L8c6140cc: r[3]=r[3]&65535u;goto L8c6140ce; /* extu.w r3,r3 */
L8c6140ce: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6140d0; /* fldi1 fr4 */
L8c6140d0: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6140d2; /* tst r4,r3 */
L8c6140d2: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6140d4; /* mov.l r3,@r15 */
L8c6140d4: take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c6140e6;goto L8c6140d8; /* bt/s 0x8c6140e6 */
L8c6140d8: r[4]=sx(m,bus,&io_status,0x8c6141bau,2);if(io_status)return io_status;goto L8c6140da; /* mov.w 0x8c6141ba,r4 */
L8c6140da: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6140dc; /* mov.l @r15,r3 */
L8c6140dc: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6140de; /* fldi1 fr4 */
L8c6140de: r[3]&=r[4];goto L8c6140e0; /* and r4,r3 */
L8c6140e0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c6140e2; /* fneg fr4 */
L8c6140e2: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6140e4; /* mov.l r3,@r15 */
L8c6140e4: fcopy(f,15u,4u);goto L8c6140e6; /* fmov fr4,fr15 */
L8c6140e6: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6140e8; /* mov.l @r15,r3 */
L8c6140e8: r[0]=16u;goto L8c6140ea; /* mov #0x10,r0 */
L8c6140ea: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6140ec; /* mov.l @(0xc,r15),r2 */
L8c6140ec: r[10]=r[8];goto L8c6140ee; /* mov r8,r10 */
L8c6140ee: r[2]+=r[3];goto L8c6140f0; /* add r3,r2 */
L8c6140f0: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6140f2; /* mov.l r2,@(0xc,r15) */
L8c6140f2: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6140f4; /* mov.l @r15,r1 */
L8c6140f4: r[1]+=4294967294u;goto L8c6140f6; /* add #-0x2,r1 */
L8c6140f6: wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c6140f8; /* mov.l r1,@r15 */
L8c6140f8: r[4]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6140fa; /* mov.w @r13+,r4 */
L8c6140fa: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6140fc; /* mov.l @(0x4,r15),r3 */
L8c6140fc: r[11]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6140fe; /* mov.w @r13+,r11 */
L8c6140fe: r[4]=r[4]&65535u;goto L8c614100; /* extu.w r4,r4 */
L8c614100: r[4]<<=2;goto L8c614102; /* shll2 r4 */
L8c614102: r[11]=r[11]&65535u;goto L8c614104; /* extu.w r11,r11 */
L8c614104: r[11]<<=2;goto L8c614106; /* shll2 r11 */
L8c614106: r[4]<<=2;goto L8c614108; /* shll2 r4 */
L8c614108: r[11]<<=2;goto L8c61410a; /* shll2 r11 */
L8c61410a: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61410c; /* shll r4 */
L8c61410c: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61410e; /* shll r11 */
L8c61410e: r[11]+=r[3];goto L8c614110; /* add r3,r11 */
L8c614110: r[4]+=r[3];goto L8c614112; /* add r3,r4 */
L8c614112: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c614114; /* fmov.s @(r0,r11),fr14 */
L8c614114: fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c614116; /* fmov.s @(r0,r4),fr4 */
L8c614116: r[0]=20u;goto L8c614118; /* mov #0x14,r0 */
L8c614118: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c61411a; /* fmov.s @(r0,r11),fr13 */
L8c61411a: fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c61411c; /* fmov.s @(r0,r4),fr5 */
L8c61411c: r[12]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61411e; /* mov.w @r13+,r12 */
L8c61411e: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614120; /* mov.l @(0x4,r15),r3 */
L8c614120: r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c614122; /* mov.w @r13,r2 */
L8c614122: r[12]=r[12]&65535u;goto L8c614124; /* extu.w r12,r12 */
L8c614124: r[12]<<=2;goto L8c614126; /* shll2 r12 */
L8c614126: r[2]=r[2]&65535u;goto L8c614128; /* extu.w r2,r2 */
L8c614128: r[2]<<=2;goto L8c61412a; /* shll2 r2 */
L8c61412a: r[2]<<=2;goto L8c61412c; /* shll2 r2 */
L8c61412c: r[12]<<=2;goto L8c61412e; /* shll2 r12 */
L8c61412e: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c614130; /* shll r12 */
L8c614130: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c614132; /* shll r2 */
L8c614132: r[2]+=r[3];goto L8c614134; /* add r3,r2 */
L8c614134: r[12]+=r[3];goto L8c614136; /* add r3,r12 */
L8c614136: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c614138; /* pref @r2 */
L8c614138: r[0]=20u;goto L8c61413a; /* mov #0x14,r0 */
L8c61413a: fcopy(f,3u,14u);goto L8c61413c; /* fmov fr14,fr3 */
L8c61413c: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61413e; /* fmov.s @(r0,r12),fr2 */
L8c61413e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614140; /* fsub fr4,fr3 */
L8c614140: r[0]=16u;goto L8c614142; /* mov #0x10,r0 */
L8c614142: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c614144; /* cmp/hs r8,r10 */
L8c614144: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614146; /* fsub fr13,fr2 */
L8c614146: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c614148; /* fmov.s @(r0,r12),fr1 */
L8c614148: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61414a; /* fsub fr14,fr1 */
L8c61414a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61414c; /* fmul fr2,fr3 */
L8c61414c: fcopy(f,2u,13u);goto L8c61414e; /* fmov fr13,fr2 */
L8c61414e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614150; /* fsub fr5,fr2 */
L8c614150: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614152; /* fmul fr1,fr2 */
L8c614152: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614154; /* fsub fr2,fr3 */
L8c614154: fcopy(f,4u,3u);goto L8c614156; /* fmov fr3,fr4 */
L8c614156: take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c6141c0;goto L8c61415a; /* bt/s 0x8c6141c0 */
L8c61415a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61415c; /* fldi0 fr3 */
L8c61415c: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c61415e; /* fcmp/gt fr4,fr3 */
L8c61415e: if((*sr&1u))goto L8c61420e;goto L8c614160; /* bt 0x8c61420e */
L8c614160: r[6]=r[14];goto L8c614162; /* mov r14,r6 */
L8c614162: r[4]=r[11];goto L8c614164; /* mov r11,r4 */
L8c614164: r[5]=r[9];goto L8c614166; /* mov r9,r5 */
L8c614166: r[14]+=4u;goto L8c614168; /* add #0x4,r14 */
L8c614168: r[8]+=16u;goto L8c61416a; /* add #0x10,r8 */
L8c61416a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61416c; /* fschg */
L8c61416c: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61416e; /* fmov.s @r4+,fr0 */
L8c61416e: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614170; /* fmov.s @r4+,fr2 */
L8c614170: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614172; /* fldi0 fr3 */
L8c614172: fcopy(f,8u,1u);goto L8c614174; /* fmov fr1,fr8 */
L8c614174: fcopy(f,10u,3u);goto L8c614176; /* fmov fr3,fr10 */
L8c614176: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c614178; /* fipr fv8,fv0 */
L8c614178: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61417a; /* fmov.s @r4+,fr4 */
L8c61417a: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61417c; /* fmov.s @r4,fr6 */
L8c61417c: fcopy(f,8u,5u);goto L8c61417e; /* fmov fr5,fr8 */
L8c61417e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614180; /* fschg */
L8c614180: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c614182; /* mov.w @r6+,r2 */
L8c614182: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c614184; /* mov.w @r6,r3 */
L8c614184: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614186; /* fmov.s fr6,@-r8 */
L8c614186: *fpul=r[3];goto L8c614188; /* lds r3,FPUL */
L8c614188: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61418a; /* fmov.s fr5,@-r8 */
L8c61418a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c61418c; /* fsub fr3,fr11 */
L8c61418c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61418e; /* fmov.s fr4,@-r8 */
L8c61418e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c614190; /* float FPUL,fr1 */
L8c614190: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c614192; /* fmul fr8,fr1 */
L8c614192: *fpul=r[2];goto L8c614194; /* lds r2,FPUL */
L8c614194: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614196; /* mov.l r5,@-r8 */
L8c614196: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c614198; /* float FPUL,fr2 */
L8c614198: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61419a; /* fmul fr8,fr2 */
L8c61419a: r[8]+=28u;goto L8c61419c; /* add #0x1c,r8 */
L8c61419c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c61419e; /* fmov.s fr11,@-r8 */
L8c61419e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6141a0; /* fmov.s fr1,@-r8 */
L8c6141a0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6141a2; /* fmov.s fr2,@-r8 */
L8c6141a2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6141a4; /* pref @r8 */
L8c6141a4: r[8]+=16u;goto L8c6141a6; /* add #0x10,r8 */
L8c6141a6: r[3]=r[8];goto L8c6141a8; /* mov r8,r3 */
L8c6141a8: r[3]-=r[10];goto L8c6141aa; /* sub r10,r3 */
L8c6141aa: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6141ac; /* mov.l @(0x8,r15),r2 */
L8c6141ac: r[3]+=32u;goto L8c6141ae; /* add #0x20,r3 */
L8c6141ae: r[14]+=4294967292u;goto L8c6141b0; /* add #-0x4,r14 */
L8c6141b0: r[2]+=r[3];goto L8c6141b2; /* add r3,r2 */
L8c6141b2: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6141b4; /* mov.l r2,@(0x8,r15) */
L8c6141b4: r[10]=r[8];goto L8c614254; /* bra 0x8c614254 */
L8c6141c0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6141c2; /* fldi0 fr3 */
L8c6141c2: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6141c4; /* fcmp/gt fr4,fr3 */
L8c6141c4: take=!(*sr&1u);r[14]+=4u;if(take)goto L8c614254;goto L8c6141c8; /* bf/s 0x8c614254 */
L8c6141c8: r[14]+=4294967292u;goto L8c6141ca; /* add #-0x4,r14 */
L8c6141ca: r[5]=rd(m,bus,&io_status,0x8c614348u,4);if(io_status)return io_status;goto L8c6141cc; /* mov.l 0x8c614348,r5 */
L8c6141cc: r[6]=r[14];goto L8c6141ce; /* mov r14,r6 */
L8c6141ce: r[14]+=4u;goto L8c6141d0; /* add #0x4,r14 */
L8c6141d0: r[8]+=16u;goto L8c6141d2; /* add #0x10,r8 */
L8c6141d2: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6141d4; /* fschg */
L8c6141d4: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6141d6; /* fmov.s @r4+,fr0 */
L8c6141d6: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6141d8; /* fmov.s @r4+,fr2 */
L8c6141d8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6141da; /* fldi0 fr3 */
L8c6141da: fcopy(f,8u,1u);goto L8c6141dc; /* fmov fr1,fr8 */
L8c6141dc: fcopy(f,10u,3u);goto L8c6141de; /* fmov fr3,fr10 */
L8c6141de: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c6141e0; /* fipr fv8,fv0 */
L8c6141e0: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6141e2; /* fmov.s @r4+,fr4 */
L8c6141e2: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6141e4; /* fmov.s @r4,fr6 */
L8c6141e4: fcopy(f,8u,5u);goto L8c6141e6; /* fmov fr5,fr8 */
L8c6141e6: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6141e8; /* fschg */
L8c6141e8: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c6141ea; /* mov.w @r6+,r2 */
L8c6141ea: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c6141ec; /* mov.w @r6,r3 */
L8c6141ec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6141ee; /* fmov.s fr6,@-r8 */
L8c6141ee: *fpul=r[3];goto L8c6141f0; /* lds r3,FPUL */
L8c6141f0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6141f2; /* fmov.s fr5,@-r8 */
L8c6141f2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c6141f4; /* fsub fr3,fr11 */
L8c6141f4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6141f6; /* fmov.s fr4,@-r8 */
L8c6141f6: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c6141f8; /* float FPUL,fr1 */
L8c6141f8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6141fa; /* fmul fr8,fr1 */
L8c6141fa: *fpul=r[2];goto L8c6141fc; /* lds r2,FPUL */
L8c6141fc: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6141fe; /* mov.l r5,@-r8 */
L8c6141fe: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c614200; /* float FPUL,fr2 */
L8c614200: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614202; /* fmul fr8,fr2 */
L8c614202: r[8]+=28u;goto L8c614204; /* add #0x1c,r8 */
L8c614204: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c614206; /* fmov.s fr11,@-r8 */
L8c614206: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c614208; /* fmov.s fr1,@-r8 */
L8c614208: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61420a; /* fmov.s fr2,@-r8 */
L8c61420a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61420c; /* pref @r8 */
L8c61420c: r[8]+=16u;goto L8c61420e; /* add #0x10,r8 */
L8c61420e: r[5]=rd(m,bus,&io_status,0x8c614348u,4);if(io_status)return io_status;goto L8c614210; /* mov.l 0x8c614348,r5 */
L8c614210: r[6]=r[14];goto L8c614212; /* mov r14,r6 */
L8c614212: r[4]=r[11];goto L8c614214; /* mov r11,r4 */
L8c614214: r[14]+=4u;goto L8c614216; /* add #0x4,r14 */
L8c614216: r[8]+=16u;goto L8c614218; /* add #0x10,r8 */
L8c614218: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61421a; /* fschg */
L8c61421a: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61421c; /* fmov.s @r4+,fr0 */
L8c61421c: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61421e; /* fmov.s @r4+,fr2 */
L8c61421e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614220; /* fldi0 fr3 */
L8c614220: fcopy(f,8u,1u);goto L8c614222; /* fmov fr1,fr8 */
L8c614222: fcopy(f,10u,3u);goto L8c614224; /* fmov fr3,fr10 */
L8c614224: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c614226; /* fipr fv8,fv0 */
L8c614226: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614228; /* fmov.s @r4+,fr4 */
L8c614228: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61422a; /* fmov.s @r4,fr6 */
L8c61422a: fcopy(f,8u,5u);goto L8c61422c; /* fmov fr5,fr8 */
L8c61422c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61422e; /* fschg */
L8c61422e: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c614230; /* mov.w @r6+,r2 */
L8c614230: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c614232; /* mov.w @r6,r3 */
L8c614232: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614234; /* fmov.s fr6,@-r8 */
L8c614234: *fpul=r[3];goto L8c614236; /* lds r3,FPUL */
L8c614236: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614238; /* fmov.s fr5,@-r8 */
L8c614238: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c61423a; /* fsub fr3,fr11 */
L8c61423a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61423c; /* fmov.s fr4,@-r8 */
L8c61423c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c61423e; /* float FPUL,fr1 */
L8c61423e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c614240; /* fmul fr8,fr1 */
L8c614240: *fpul=r[2];goto L8c614242; /* lds r2,FPUL */
L8c614242: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614244; /* mov.l r5,@-r8 */
L8c614244: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c614246; /* float FPUL,fr2 */
L8c614246: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614248; /* fmul fr8,fr2 */
L8c614248: r[8]+=28u;goto L8c61424a; /* add #0x1c,r8 */
L8c61424a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c61424c; /* fmov.s fr11,@-r8 */
L8c61424c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61424e; /* fmov.s fr1,@-r8 */
L8c61424e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614250; /* fmov.s fr2,@-r8 */
L8c614250: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614252; /* pref @r8 */
L8c614252: r[8]+=16u;goto L8c614254; /* add #0x10,r8 */
L8c614254: fcopy(f,4u,14u);goto L8c614256; /* fmov fr14,fr4 */
L8c614256: r[0]=16u;goto L8c614258; /* mov #0x10,r0 */
L8c614258: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61425a; /* mov.l @r15,r3 */
L8c61425a: r[4]=r[11];goto L8c61425c; /* mov r11,r4 */
L8c61425c: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c61425e; /* fmov.s @(r0,r12),fr14 */
L8c61425e: r[0]=20u;goto L8c614260; /* mov #0x14,r0 */
L8c614260: fcopy(f,5u,13u);goto L8c614262; /* fmov fr13,fr5 */
L8c614262: r[3]+=4294967295u;goto L8c614264; /* add #-0x1,r3 */
L8c614264: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c614266; /* fmov.s @(r0,r12),fr13 */
L8c614266: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614268; /* tst r3,r3 */
L8c614268: r[11]=r[12];goto L8c61426a; /* mov r12,r11 */
L8c61426a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c61426c; /* fneg fr15 */
L8c61426c: take=(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c614274;goto L8c614270; /* bt/s 0x8c614274 */
L8c614270: goto L8c61411c; /* bra 0x8c61411c */
L8c614274: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c614276; /* cmp/hs r8,r10 */
L8c614276: take=(*sr&1u);r[14]+=8u;if(take)goto L8c6142ce;goto L8c61427a; /* bt/s 0x8c6142ce */
L8c61427a: r[14]+=4294967288u;goto L8c61427c; /* add #-0x8,r14 */
L8c61427c: r[4]=r[12];goto L8c61427e; /* mov r12,r4 */
L8c61427e: r[6]=r[14];goto L8c614280; /* mov r14,r6 */
L8c614280: r[5]=r[9];goto L8c614282; /* mov r9,r5 */
L8c614282: r[14]+=4u;goto L8c614284; /* add #0x4,r14 */
L8c614284: r[8]+=16u;goto L8c614286; /* add #0x10,r8 */
L8c614286: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614288; /* fschg */
L8c614288: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61428a; /* fmov.s @r4+,fr0 */
L8c61428a: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61428c; /* fmov.s @r4+,fr2 */
L8c61428c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61428e; /* fldi0 fr3 */
L8c61428e: fcopy(f,8u,1u);goto L8c614290; /* fmov fr1,fr8 */
L8c614290: fcopy(f,10u,3u);goto L8c614292; /* fmov fr3,fr10 */
L8c614292: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c614294; /* fipr fv8,fv0 */
L8c614294: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614296; /* fmov.s @r4+,fr4 */
L8c614296: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614298; /* fmov.s @r4,fr6 */
L8c614298: fcopy(f,8u,5u);goto L8c61429a; /* fmov fr5,fr8 */
L8c61429a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61429c; /* fschg */
L8c61429c: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c61429e; /* mov.w @r6+,r2 */
L8c61429e: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c6142a0; /* mov.w @r6,r3 */
L8c6142a0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6142a2; /* fmov.s fr6,@-r8 */
L8c6142a2: *fpul=r[3];goto L8c6142a4; /* lds r3,FPUL */
L8c6142a4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6142a6; /* fmov.s fr5,@-r8 */
L8c6142a6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c6142a8; /* fsub fr3,fr11 */
L8c6142a8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6142aa; /* fmov.s fr4,@-r8 */
L8c6142aa: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c6142ac; /* float FPUL,fr1 */
L8c6142ac: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6142ae; /* fmul fr8,fr1 */
L8c6142ae: *fpul=r[2];goto L8c6142b0; /* lds r2,FPUL */
L8c6142b0: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6142b2; /* mov.l r5,@-r8 */
L8c6142b2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c6142b4; /* float FPUL,fr2 */
L8c6142b4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6142b6; /* fmul fr8,fr2 */
L8c6142b6: r[8]+=28u;goto L8c6142b8; /* add #0x1c,r8 */
L8c6142b8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c6142ba; /* fmov.s fr11,@-r8 */
L8c6142ba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6142bc; /* fmov.s fr1,@-r8 */
L8c6142bc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6142be; /* fmov.s fr2,@-r8 */
L8c6142be: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6142c0; /* pref @r8 */
L8c6142c0: r[8]+=16u;goto L8c6142c2; /* add #0x10,r8 */
L8c6142c2: r[3]=r[8];goto L8c6142c4; /* mov r8,r3 */
L8c6142c4: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6142c6; /* mov.l @(0x8,r15),r2 */
L8c6142c6: r[3]-=r[10];goto L8c6142c8; /* sub r10,r3 */
L8c6142c8: r[3]+=32u;goto L8c6142ca; /* add #0x20,r3 */
L8c6142ca: r[2]+=r[3];goto L8c6142cc; /* add r3,r2 */
L8c6142cc: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6142ce; /* mov.l r2,@(0x8,r15) */
L8c6142ce: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c6142d0; /* mov.l @(0x10,r15),r3 */
L8c6142d0: r[3]+=4294967295u;goto L8c6142d2; /* add #-0x1,r3 */
L8c6142d2: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6142d4; /* tst r3,r3 */
L8c6142d4: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;if(take)goto L8c6142dc;goto L8c6142d8; /* bt/s 0x8c6142dc */
L8c6142d8: goto L8c6140c8; /* bra 0x8c6140c8 */
L8c6142dc: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6142de; /* mov.l @(0x8,r15),r3 */
L8c6142de: r[4]=rd(m,bus,&io_status,0x8c61434cu,4);if(io_status)return io_status;goto L8c6142e0; /* mov.l 0x8c61434c,r4 */
L8c6142e0: r[3]>>=2;goto L8c6142e2; /* shlr2 r3 */
L8c6142e2: r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6142e4; /* mov.l @r4,r1 */
L8c6142e4: r[3]>>=2;goto L8c6142e6; /* shlr2 r3 */
L8c6142e6: *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c6142e8; /* shlr r3 */
L8c6142e8: r[1]+=r[3];goto L8c6142ea; /* add r3,r1 */
L8c6142ea: wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c6142ec; /* mov.l r1,@r4 */
L8c6142ec: r[4]=rd(m,bus,&io_status,0x8c614350u,4);if(io_status)return io_status;goto L8c6142ee; /* mov.l 0x8c614350,r4 */
L8c6142ee: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6142f0; /* mov.l @(0xc,r15),r3 */
L8c6142f0: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6142f2; /* mov.l @r4,r2 */
L8c6142f2: r[2]+=r[3];goto L8c6142f4; /* add r3,r2 */
L8c6142f4: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c6142f6; /* mov.l r2,@r4 */
L8c6142f6: r[15]+=20u;goto L8c6142f8; /* add #0x14,r15 */
L8c6142f8: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6142fa; /* lds.l @r15+,PR */
L8c6142fa: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6142fc; /* fmov.s @r15+,fr13 */
L8c6142fc: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6142fe; /* fmov.s @r15+,fr14 */
L8c6142fe: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614300; /* fmov.s @r15+,fr15 */
L8c614300: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614302; /* mov.l @r15+,r9 */
L8c614302: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614304; /* mov.l @r15+,r10 */
L8c614304: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614306; /* mov.l @r15+,r11 */
L8c614306: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614308; /* mov.l @r15+,r12 */
L8c614308: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61430a; /* mov.l @r15+,r13 */
L8c61430a: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_uv_face_alternate_strip_clipped_8c615b84(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c615b84;
L8c615b84: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c615b86; /* mov.l r14,@-r15 */
L8c615b86: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c615b88; /* mov.l r13,@-r15 */
L8c615b88: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c615b8a; /* mov.l r12,@-r15 */
L8c615b8a: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c615b8c; /* mov.l r11,@-r15 */
L8c615b8c: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c615b8e; /* mov.l r10,@-r15 */
L8c615b8e: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c615b90; /* mov.l r9,@-r15 */
L8c615b90: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c615b92; /* fmov.s fr15,@-r15 */
L8c615b92: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c615b94; /* fmov.s fr14,@-r15 */
L8c615b94: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c615b96; /* fmov.s fr13,@-r15 */
L8c615b96: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c615b98; /* fmov.s fr12,@-r15 */
L8c615b98: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c615b9a; /* sts.l PR,@-r15 */
L8c615b9a: r[15]+=4294967216u;goto L8c615b9c; /* add #-0x50,r15 */
L8c615b9c: r[3]=rd(m,bus,&io_status,0x8c615be4u,4);if(io_status)return io_status;goto L8c615b9e; /* mov.l 0x8c615be4,r3 */
L8c615b9e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c615ba0; /* fldi1 fr12 */
L8c615ba0: target=r[3];*pr=0x8c615ba4u;r[12]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c615ba4; /* jsr @r3 */
L8c615ba4: r[2]=rd(m,bus,&io_status,0x8c615be8u,4);if(io_status)return io_status;goto L8c615ba6; /* mov.l 0x8c615be8,r2 */
L8c615ba6: r[1]=0u;goto L8c615ba8; /* mov #0x0,r1 */
L8c615ba8: r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c615baa; /* mov.w @(0x2,r12),r0 */
L8c615baa: r[13]=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;goto L8c615bac; /* mov.l @(0x4,r12),r13 */
L8c615bac: r[14]=rd(m,bus,&io_status,r[12]+0x14u,4);if(io_status)return io_status;goto L8c615bae; /* mov.l @(0x14,r12),r14 */
L8c615bae: r[0]=r[0]&65535u;goto L8c615bb0; /* extu.w r0,r0 */
L8c615bb0: wr(m,bus,&io_status,r[15]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c615bb2; /* mov.l r0,@(0x1c,r15) */
L8c615bb2: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c615bb4; /* mov.l @r2,r3 */
L8c615bb4: wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c615bb6; /* mov.l r3,@(0xc,r15) */
L8c615bb6: wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c615bb8; /* mov.l r1,@(0x10,r15) */
L8c615bb8: goto L8c615bec; /* bra 0x8c615bec */
L8c615bec: r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c615bee; /* mov.w @r13+,r3 */
L8c615bee: r[4]=rd(m,bus,&io_status,0x8c615d0cu,4);if(io_status)return io_status;goto L8c615bf0; /* mov.l 0x8c615d0c,r4 */
L8c615bf0: r[3]=r[3]&65535u;goto L8c615bf2; /* extu.w r3,r3 */
L8c615bf2: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c615bf4; /* tst r4,r3 */
L8c615bf4: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c615bf6; /* mov.l r3,@(0x8,r15) */
L8c615bf6: take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c615c08;goto L8c615bfa; /* bt/s 0x8c615c08 */
L8c615bfa: r[4]=sx(m,bus,&io_status,0x8c615d08u,2);if(io_status)return io_status;goto L8c615bfc; /* mov.w 0x8c615d08,r4 */
L8c615bfc: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615bfe; /* mov.l @(0x8,r15),r3 */
L8c615bfe: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c615c00; /* fldi1 fr4 */
L8c615c00: r[3]&=r[4];goto L8c615c02; /* and r4,r3 */
L8c615c02: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c615c04; /* fneg fr4 */
L8c615c04: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c615c06; /* mov.l r3,@(0x8,r15) */
L8c615c06: fcopy(f,15u,4u);goto L8c615c08; /* fmov fr4,fr15 */
L8c615c08: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615c0a; /* mov.l @(0x8,r15),r3 */
L8c615c0a: r[0]=16u;goto L8c615c0c; /* mov #0x10,r0 */
L8c615c0c: r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c615c0e; /* mov.l @(0x10,r15),r2 */
L8c615c0e: r[9]=r[8];goto L8c615c10; /* mov r8,r9 */
L8c615c10: r[2]+=r[3];goto L8c615c12; /* add r3,r2 */
L8c615c12: wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c615c14; /* mov.l r2,@(0x10,r15) */
L8c615c14: r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615c16; /* mov.l @(0x8,r15),r1 */
L8c615c16: r[1]+=4294967294u;goto L8c615c18; /* add #-0x2,r1 */
L8c615c18: wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c615c1a; /* mov.l r1,@(0x8,r15) */
L8c615c1a: r[10]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c615c1c; /* mov.w @r13+,r10 */
L8c615c1c: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c615c1e; /* mov.l @(0xc,r15),r3 */
L8c615c1e: r[11]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c615c20; /* mov.w @r13+,r11 */
L8c615c20: r[10]=r[10]&65535u;goto L8c615c22; /* extu.w r10,r10 */
L8c615c22: r[10]<<=2;goto L8c615c24; /* shll2 r10 */
L8c615c24: r[11]=r[11]&65535u;goto L8c615c26; /* extu.w r11,r11 */
L8c615c26: r[11]<<=2;goto L8c615c28; /* shll2 r11 */
L8c615c28: r[10]<<=2;goto L8c615c2a; /* shll2 r10 */
L8c615c2a: r[11]<<=2;goto L8c615c2c; /* shll2 r11 */
L8c615c2c: *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c615c2e; /* shll r10 */
L8c615c2e: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c615c30; /* shll r11 */
L8c615c30: r[11]+=r[3];goto L8c615c32; /* add r3,r11 */
L8c615c32: r[10]+=r[3];goto L8c615c34; /* add r3,r10 */
L8c615c34: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c615c36; /* fmov.s @(r0,r11),fr13 */
L8c615c36: fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c615c38; /* fmov.s @(r0,r10),fr5 */
L8c615c38: r[0]=20u;goto L8c615c3a; /* mov #0x14,r0 */
L8c615c3a: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c615c3c; /* fmov.s @(r0,r11),fr14 */
L8c615c3c: fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c615c3e; /* fmov.s @(r0,r10),fr6 */
L8c615c3e: r[12]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c615c40; /* mov.w @r13+,r12 */
L8c615c40: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c615c42; /* mov.l @(0xc,r15),r3 */
L8c615c42: r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c615c44; /* mov.w @r13,r2 */
L8c615c44: r[12]=r[12]&65535u;goto L8c615c46; /* extu.w r12,r12 */
L8c615c46: r[12]<<=2;goto L8c615c48; /* shll2 r12 */
L8c615c48: r[2]=r[2]&65535u;goto L8c615c4a; /* extu.w r2,r2 */
L8c615c4a: r[2]<<=2;goto L8c615c4c; /* shll2 r2 */
L8c615c4c: r[2]<<=2;goto L8c615c4e; /* shll2 r2 */
L8c615c4e: r[12]<<=2;goto L8c615c50; /* shll2 r12 */
L8c615c50: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c615c52; /* shll r12 */
L8c615c52: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c615c54; /* shll r2 */
L8c615c54: r[2]+=r[3];goto L8c615c56; /* add r3,r2 */
L8c615c56: r[12]+=r[3];goto L8c615c58; /* add r3,r12 */
L8c615c58: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c615c5a; /* pref @r2 */
L8c615c5a: r[3]=0u;goto L8c615c5c; /* mov #0x0,r3 */
L8c615c5c: wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;goto L8c615c5e; /* mov.l r3,@(0x18,r15) */
L8c615c5e: r[4]=63u;goto L8c615c60; /* mov #0x3f,r4 */
L8c615c60: r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c615c62; /* mov.l @(0xc,r10),r2 */
L8c615c62: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c615c64; /* mov.l @(0xc,r11),r3 */
L8c615c64: r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c615c66; /* mov.l @(0xc,r12),r1 */
L8c615c66: r[2]&=r[3];goto L8c615c68; /* and r3,r2 */
L8c615c68: r[2]&=r[1];goto L8c615c6a; /* and r1,r2 */
L8c615c6a: r[4]&=r[2];goto L8c615c6c; /* and r2,r4 */
L8c615c6c: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c615c6e; /* tst r4,r4 */
L8c615c6e: take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c615c76;goto L8c615c72; /* bt/s 0x8c615c76 */
L8c615c72: goto L8c615dd0; /* bra 0x8c615dd0 */
L8c615c76: r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c615c78; /* mov.l @(0xc,r10),r4 */
L8c615c78: r[5]=1u;goto L8c615c7a; /* mov #0x1,r5 */
L8c615c7a: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c615c7c; /* mov.l @(0xc,r11),r3 */
L8c615c7c: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c615c7e; /* mov.l @(0xc,r12),r2 */
L8c615c7e: r[4]|=r[3];goto L8c615c80; /* or r3,r4 */
L8c615c80: r[4]|=r[2];goto L8c615c82; /* or r2,r4 */
L8c615c82: r[4]&=r[5];goto L8c615c84; /* and r5,r4 */
L8c615c84: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c615c86; /* tst r4,r4 */
L8c615c86: if(!(*sr&1u))goto L8c615c8c;goto L8c615c88; /* bf 0x8c615c8c */
L8c615c88: goto L8c615db2; /* bra 0x8c615db2 */
L8c615c8c: r[7]=r[15];goto L8c615c8e; /* mov r15,r7 */
L8c615c8e: r[7]+=68u;goto L8c615c90; /* add #0x44,r7 */
L8c615c90: wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c615c92; /* mov.l r10,@r7 */
L8c615c92: wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c615c94; /* mov.l r11,@(0x4,r7) */
L8c615c94: wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c615c96; /* mov.l r12,@(0x8,r7) */
L8c615c96: r[6]=0u;goto L8c615d44; /* bra 0x8c615d44 */
L8c615c9a: r[2]=r[6];goto L8c615c9c; /* mov r6,r2 */
L8c615c9c: r[1]=r[6];goto L8c615c9e; /* mov r6,r1 */
L8c615c9e: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c615ca0; /* shll r2 */
L8c615ca0: r[3]=r[6];goto L8c615ca2; /* mov r6,r3 */
L8c615ca2: r[3]<<=2;goto L8c615ca4; /* shll2 r3 */
L8c615ca4: r[2]+=r[1];goto L8c615ca6; /* add r1,r2 */
L8c615ca6: r[2]<<=2;goto L8c615ca8; /* shll2 r2 */
L8c615ca8: r[3]+=r[7];goto L8c615caa; /* add r7,r3 */
L8c615caa: r[2]+=r[4];goto L8c615cac; /* add r4,r2 */
L8c615cac: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c615cae; /* mov.l r3,@(0x4,r15) */
L8c615cae: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c615cb0; /* mov.l r2,@r15 */
L8c615cb0: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c615cb2; /* mov.l @r3,r1 */
L8c615cb2: r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c615cb4; /* mov.l @(0xc,r1),r2 */
L8c615cb4: *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c615cb6; /* tst r5,r2 */
L8c615cb6: take=!(*sr&1u);r[6]+=1u;if(take)goto L8c615d1c;goto L8c615cba; /* bf/s 0x8c615d1c */
L8c615cba: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615cbc; /* mov.l @(0x4,r15),r1 */
L8c615cbc: r[0]=24u;goto L8c615cbe; /* mov #0x18,r0 */
L8c615cbe: fcopy(f,4u,12u);goto L8c615cc0; /* fmov fr12,fr4 */
L8c615cc0: r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c615cc2; /* mov.l @r1,r3 */
L8c615cc2: r[4]=rd(m,bus,&io_status,0x8c615d10u,4);if(io_status)return io_status;goto L8c615cc4; /* mov.l 0x8c615d10,r4 */
L8c615cc4: wr(m,bus,&io_status,r[15]+0x14u,r[3],4);if(io_status)return io_status;goto L8c615cc6; /* mov.l r3,@(0x14,r15) */
L8c615cc6: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c615cc8; /* fmov.s @(r0,r3),fr3 */
L8c615cc8: r[0]=12u;goto L8c615cca; /* mov #0xc,r0 */
L8c615cca: r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c615ccc; /* mov.l @(0x14,r15),r2 */
L8c615ccc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c615cce; /* fdiv fr3,fr4 */
L8c615cce: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615cd0; /* fmov.s @(r0,r4),fr3 */
L8c615cd0: r[0]=16u;goto L8c615cd2; /* mov #0x10,r0 */
L8c615cd2: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615cd4; /* mov.l @r15,r3 */
L8c615cd4: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c615cd6; /* fmov.s @(r0,r2),fr2 */
L8c615cd6: r[0]=16u;goto L8c615cd8; /* mov #0x10,r0 */
L8c615cd8: r[2]=rd(m,bus,&io_status,0x8c615d14u,4);if(io_status)return io_status;goto L8c615cda; /* mov.l 0x8c615d14,r2 */
L8c615cda: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615cdc; /* fsub fr3,fr2 */
L8c615cdc: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c615cde; /* fmov.s @r2,fr3 */
L8c615cde: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615ce0; /* fmul fr4,fr2 */
L8c615ce0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615ce2; /* fdiv fr3,fr2 */
L8c615ce2: fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c615ce4; /* fmov.s fr2,@r3 */
L8c615ce4: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615ce6; /* mov.l @(0x4,r15),r1 */
L8c615ce6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615ce8; /* fmov.s @(r0,r4),fr3 */
L8c615ce8: r[0]=20u;goto L8c615cea; /* mov #0x14,r0 */
L8c615cea: r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c615cec; /* mov.l @r1,r1 */
L8c615cec: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615cee; /* mov.l @r15,r3 */
L8c615cee: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c615cf0; /* fmov.s @(r0,r1),fr2 */
L8c615cf0: r[1]=rd(m,bus,&io_status,0x8c615d18u,4);if(io_status)return io_status;goto L8c615cf2; /* mov.l 0x8c615d18,r1 */
L8c615cf2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615cf4; /* fsub fr3,fr2 */
L8c615cf4: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c615cf6; /* fmov.s @r1,fr3 */
L8c615cf6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615cf8; /* fmul fr4,fr2 */
L8c615cf8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615cfa; /* fdiv fr3,fr2 */
L8c615cfa: r[0]=4u;goto L8c615cfc; /* mov #0x4,r0 */
L8c615cfc: fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c615cfe; /* fmov fr2,@(r0,r3) */
L8c615cfe: r[0]=8u;goto L8c615d00; /* mov #0x8,r0 */
L8c615d00: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615d02; /* mov.l @r15,r3 */
L8c615d02: fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c615d04; /* fmov fr4,@(r0,r3) */
L8c615d04: goto L8c615d44; /* bra 0x8c615d44 */
L8c615d1c: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615d1e; /* mov.l @(0x4,r15),r3 */
L8c615d1e: r[0]=16u;goto L8c615d20; /* mov #0x10,r0 */
L8c615d20: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615d22; /* mov.l @r15,r2 */
L8c615d22: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c615d24; /* mov.l @r3,r1 */
L8c615d24: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c615d26; /* fmov.s @(r0,r1),fr3 */
L8c615d26: r[0]=20u;goto L8c615d28; /* mov #0x14,r0 */
L8c615d28: fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c615d2a; /* fmov.s fr3,@r2 */
L8c615d2a: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615d2c; /* mov.l @(0x4,r15),r2 */
L8c615d2c: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615d2e; /* mov.l @r15,r3 */
L8c615d2e: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c615d30; /* mov.l @r2,r1 */
L8c615d30: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c615d32; /* fmov.s @(r0,r1),fr3 */
L8c615d32: r[0]=4u;goto L8c615d34; /* mov #0x4,r0 */
L8c615d34: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c615d36; /* fmov fr3,@(r0,r3) */
L8c615d36: r[0]=24u;goto L8c615d38; /* mov #0x18,r0 */
L8c615d38: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615d3a; /* mov.l @(0x4,r15),r2 */
L8c615d3a: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615d3c; /* mov.l @r15,r3 */
L8c615d3c: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c615d3e; /* mov.l @r2,r1 */
L8c615d3e: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c615d40; /* fmov.s @(r0,r1),fr3 */
L8c615d40: r[0]=8u;goto L8c615d42; /* mov #0x8,r0 */
L8c615d42: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c615d44; /* fmov fr3,@(r0,r3) */
L8c615d44: r[3]=3u;goto L8c615d46; /* mov #0x3,r3 */
L8c615d46: r[4]=r[15];goto L8c615d48; /* mov r15,r4 */
L8c615d48: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c615d4a; /* cmp/ge r3,r6 */
L8c615d4a: take=!(*sr&1u);r[4]+=32u;if(take)goto L8c615c9a;goto L8c615d4e; /* bf/s 0x8c615c9a */
L8c615d4e: r[7]=r[4];goto L8c615d50; /* mov r4,r7 */
L8c615d50: r[0]=8u;goto L8c615d52; /* mov #0x8,r0 */
L8c615d52: r[3]=r[4];goto L8c615d54; /* mov r4,r3 */
L8c615d54: fload(m,bus,&io_status,f,9u,r[0]+r[7]);if(io_status)return io_status;goto L8c615d56; /* fmov.s @(r0,r7),fr9 */
L8c615d56: r[3]+=12u;goto L8c615d58; /* add #0xc,r3 */
L8c615d58: r[6]=r[4];goto L8c615d5a; /* mov r4,r6 */
L8c615d5a: r[0]=4u;goto L8c615d5c; /* mov #0x4,r0 */
L8c615d5c: fcopy(f,0u,9u);goto L8c615d5e; /* fmov fr9,fr0 */
L8c615d5e: r[6]+=24u;goto L8c615d60; /* add #0x18,r6 */
L8c615d60: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c615d62; /* mov.l r3,@r15 */
L8c615d62: fload(m,bus,&io_status,f,6u,r[0]+r[7]);if(io_status)return io_status;goto L8c615d64; /* fmov.s @(r0,r7),fr6 */
L8c615d64: fload(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c615d66; /* fmov.s @(r0,r3),fr4 */
L8c615d66: r[0]=8u;goto L8c615d68; /* mov #0x8,r0 */
L8c615d68: fload(m,bus,&io_status,f,7u,r[0]+r[3]);if(io_status)return io_status;goto L8c615d6a; /* fmov.s @(r0,r3),fr7 */
L8c615d6a: r[0]=4u;goto L8c615d6c; /* mov #0x4,r0 */
L8c615d6c: fcopy(f,3u,6u);goto L8c615d6e; /* fmov fr6,fr3 */
L8c615d6e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c615d70; /* fmul fr4,fr0 */
L8c615d70: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615d72; /* fmul fr7,fr3 */
L8c615d72: fload(m,bus,&io_status,f,8u,r[7]);if(io_status)return io_status;goto L8c615d74; /* fmov.s @r7,fr8 */
L8c615d74: fload(m,bus,&io_status,f,5u,r[3]);if(io_status)return io_status;goto L8c615d76; /* fmov.s @r3,fr5 */
L8c615d76: fcopy(f,2u,8u);goto L8c615d78; /* fmov fr8,fr2 */
L8c615d78: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615d7a; /* fmul fr7,fr2 */
L8c615d7a: fcopy(f,1u,6u);goto L8c615d7c; /* fmov fr6,fr1 */
L8c615d7c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c615d7e; /* fsub fr3,fr0 */
L8c615d7e: fcopy(f,3u,9u);goto L8c615d80; /* fmov fr9,fr3 */
L8c615d80: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615d82; /* fmul fr5,fr3 */
L8c615d82: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615d84; /* fsub fr3,fr2 */
L8c615d84: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c615d86; /* fmov.s @(r0,r6),fr3 */
L8c615d86: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615d88; /* fmul fr2,fr3 */
L8c615d88: fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c615d8a; /* fmov.s @r6,fr2 */
L8c615d8a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615d8c; /* fmac fr0,fr2,fr3 */
L8c615d8c: fcopy(f,2u,3u);goto L8c615d8e; /* fmov fr3,fr2 */
L8c615d8e: fcopy(f,3u,8u);goto L8c615d90; /* fmov fr8,fr3 */
L8c615d90: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615d92; /* fmul fr4,fr3 */
L8c615d92: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[5],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615d94; /* fmul fr5,fr1 */
L8c615d94: r[0]=8u;goto L8c615d96; /* mov #0x8,r0 */
L8c615d96: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c615d98; /* fsub fr3,fr1 */
L8c615d98: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c615d9a; /* fmov.s @(r0,r6),fr3 */
L8c615d9a: fcopy(f,0u,1u);goto L8c615d9c; /* fmov fr1,fr0 */
L8c615d9c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615d9e; /* fmac fr0,fr3,fr2 */
L8c615d9e: fcopy(f,3u,15u);goto L8c615da0; /* fmov fr15,fr3 */
L8c615da0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c615da2; /* fneg fr3 */
L8c615da2: fcopy(f,4u,2u);goto L8c615da4; /* fmov fr2,fr4 */
L8c615da4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c615da6; /* fmul fr3,fr4 */
L8c615da6: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c615da8; /* fldi0 fr2 */
L8c615da8: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c615daa; /* fcmp/gt fr2,fr4 */
L8c615daa: if((*sr&1u))goto L8c615dd0;goto L8c615dac; /* bt 0x8c615dd0 */
L8c615dac: wr(m,bus,&io_status,r[15]+0x18u,r[5],4);if(io_status)return io_status;goto L8c615dae; /* mov.l r5,@(0x18,r15) */
L8c615dae: fcopy(f,4u,12u);goto L8c615dd0; /* bra 0x8c615dd0 */
L8c615db2: r[0]=20u;goto L8c615db4; /* mov #0x14,r0 */
L8c615db4: fcopy(f,3u,13u);goto L8c615db6; /* fmov fr13,fr3 */
L8c615db6: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c615db8; /* fmov.s @(r0,r12),fr2 */
L8c615db8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615dba; /* fsub fr5,fr3 */
L8c615dba: r[0]=16u;goto L8c615dbc; /* mov #0x10,r0 */
L8c615dbc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615dbe; /* fsub fr14,fr2 */
L8c615dbe: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c615dc0; /* fmov.s @(r0,r12),fr1 */
L8c615dc0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c615dc2; /* fsub fr13,fr1 */
L8c615dc2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615dc4; /* fmul fr2,fr3 */
L8c615dc4: fcopy(f,2u,14u);goto L8c615dc6; /* fmov fr14,fr2 */
L8c615dc6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[6],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615dc8; /* fsub fr6,fr2 */
L8c615dc8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615dca; /* fmul fr1,fr2 */
L8c615dca: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615dcc; /* fsub fr2,fr3 */
L8c615dcc: fcopy(f,4u,3u);goto L8c615dce; /* fmov fr3,fr4 */
L8c615dce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c615dd0; /* fmul fr15,fr4 */
L8c615dd0: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c615dd2; /* cmp/hs r8,r9 */
L8c615dd2: if((*sr&1u))goto L8c615e26;goto L8c615dd4; /* bt 0x8c615e26 */
L8c615dd4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615dd6; /* fldi0 fr3 */
L8c615dd6: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c615dd8; /* fcmp/gt fr4,fr3 */
L8c615dd8: if((*sr&1u))goto L8c615e76;goto L8c615dda; /* bt 0x8c615e76 */
L8c615dda: r[5]=rd(m,bus,&io_status,0x8c616040u,4);if(io_status)return io_status;goto L8c615ddc; /* mov.l 0x8c616040,r5 */
L8c615ddc: r[6]=r[14];goto L8c615dde; /* mov r14,r6 */
L8c615dde: r[4]=r[11];goto L8c615de0; /* mov r11,r4 */
L8c615de0: r[14]+=4u;goto L8c615de2; /* add #0x4,r14 */
L8c615de2: r[8]+=16u;goto L8c615de4; /* add #0x10,r8 */
L8c615de4: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615de6; /* fschg */
L8c615de6: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615de8; /* fmov.s @r4+,fr0 */
L8c615de8: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615dea; /* fmov.s @r4+,fr2 */
L8c615dea: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615dec; /* fldi0 fr3 */
L8c615dec: fcopy(f,8u,1u);goto L8c615dee; /* fmov fr1,fr8 */
L8c615dee: fcopy(f,10u,3u);goto L8c615df0; /* fmov fr3,fr10 */
L8c615df0: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c615df2; /* fipr fv8,fv0 */
L8c615df2: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615df4; /* fmov.s @r4+,fr4 */
L8c615df4: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615df6; /* fmov.s @r4,fr6 */
L8c615df6: fcopy(f,8u,5u);goto L8c615df8; /* fmov fr5,fr8 */
L8c615df8: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615dfa; /* fschg */
L8c615dfa: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c615dfc; /* mov.w @r6+,r2 */
L8c615dfc: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c615dfe; /* mov.w @r6,r3 */
L8c615dfe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615e00; /* fmov.s fr6,@-r8 */
L8c615e00: *fpul=r[3];goto L8c615e02; /* lds r3,FPUL */
L8c615e02: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615e04; /* fmov.s fr5,@-r8 */
L8c615e04: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c615e06; /* fsub fr3,fr11 */
L8c615e06: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615e08; /* fmov.s fr4,@-r8 */
L8c615e08: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c615e0a; /* float FPUL,fr1 */
L8c615e0a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615e0c; /* fmul fr8,fr1 */
L8c615e0c: *fpul=r[2];goto L8c615e0e; /* lds r2,FPUL */
L8c615e0e: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615e10; /* mov.l r5,@-r8 */
L8c615e10: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c615e12; /* float FPUL,fr2 */
L8c615e12: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615e14; /* fmul fr8,fr2 */
L8c615e14: r[8]+=28u;goto L8c615e16; /* add #0x1c,r8 */
L8c615e16: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c615e18; /* fmov.s fr11,@-r8 */
L8c615e18: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c615e1a; /* fmov.s fr1,@-r8 */
L8c615e1a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c615e1c; /* fmov.s fr2,@-r8 */
L8c615e1c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615e1e; /* pref @r8 */
L8c615e1e: r[8]+=16u;goto L8c615e20; /* add #0x10,r8 */
L8c615e20: r[14]+=4294967292u;goto L8c615e22; /* add #-0x4,r14 */
L8c615e22: r[9]=r[8];goto L8c615ebc; /* bra 0x8c615ebc */
L8c615e26: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615e28; /* fldi0 fr3 */
L8c615e28: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c615e2a; /* fcmp/gt fr4,fr3 */
L8c615e2a: take=!(*sr&1u);r[14]+=4u;if(take)goto L8c615ebc;goto L8c615e2e; /* bf/s 0x8c615ebc */
L8c615e2e: r[14]+=4294967292u;goto L8c615e30; /* add #-0x4,r14 */
L8c615e30: r[5]=rd(m,bus,&io_status,0x8c616044u,4);if(io_status)return io_status;goto L8c615e32; /* mov.l 0x8c616044,r5 */
L8c615e32: r[6]=r[14];goto L8c615e34; /* mov r14,r6 */
L8c615e34: r[4]=r[10];goto L8c615e36; /* mov r10,r4 */
L8c615e36: r[14]+=4u;goto L8c615e38; /* add #0x4,r14 */
L8c615e38: r[8]+=16u;goto L8c615e3a; /* add #0x10,r8 */
L8c615e3a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615e3c; /* fschg */
L8c615e3c: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615e3e; /* fmov.s @r4+,fr0 */
L8c615e3e: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615e40; /* fmov.s @r4+,fr2 */
L8c615e40: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615e42; /* fldi0 fr3 */
L8c615e42: fcopy(f,8u,1u);goto L8c615e44; /* fmov fr1,fr8 */
L8c615e44: fcopy(f,10u,3u);goto L8c615e46; /* fmov fr3,fr10 */
L8c615e46: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c615e48; /* fipr fv8,fv0 */
L8c615e48: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615e4a; /* fmov.s @r4+,fr4 */
L8c615e4a: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615e4c; /* fmov.s @r4,fr6 */
L8c615e4c: fcopy(f,8u,5u);goto L8c615e4e; /* fmov fr5,fr8 */
L8c615e4e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615e50; /* fschg */
L8c615e50: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c615e52; /* mov.w @r6+,r2 */
L8c615e52: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c615e54; /* mov.w @r6,r3 */
L8c615e54: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615e56; /* fmov.s fr6,@-r8 */
L8c615e56: *fpul=r[3];goto L8c615e58; /* lds r3,FPUL */
L8c615e58: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615e5a; /* fmov.s fr5,@-r8 */
L8c615e5a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c615e5c; /* fsub fr3,fr11 */
L8c615e5c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615e5e; /* fmov.s fr4,@-r8 */
L8c615e5e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c615e60; /* float FPUL,fr1 */
L8c615e60: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615e62; /* fmul fr8,fr1 */
L8c615e62: *fpul=r[2];goto L8c615e64; /* lds r2,FPUL */
L8c615e64: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615e66; /* mov.l r5,@-r8 */
L8c615e66: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c615e68; /* float FPUL,fr2 */
L8c615e68: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615e6a; /* fmul fr8,fr2 */
L8c615e6a: r[8]+=28u;goto L8c615e6c; /* add #0x1c,r8 */
L8c615e6c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c615e6e; /* fmov.s fr11,@-r8 */
L8c615e6e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c615e70; /* fmov.s fr1,@-r8 */
L8c615e70: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c615e72; /* fmov.s fr2,@-r8 */
L8c615e72: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615e74; /* pref @r8 */
L8c615e74: r[8]+=16u;goto L8c615e76; /* add #0x10,r8 */
L8c615e76: r[5]=rd(m,bus,&io_status,0x8c616044u,4);if(io_status)return io_status;goto L8c615e78; /* mov.l 0x8c616044,r5 */
L8c615e78: r[6]=r[14];goto L8c615e7a; /* mov r14,r6 */
L8c615e7a: r[4]=r[11];goto L8c615e7c; /* mov r11,r4 */
L8c615e7c: r[14]+=4u;goto L8c615e7e; /* add #0x4,r14 */
L8c615e7e: r[8]+=16u;goto L8c615e80; /* add #0x10,r8 */
L8c615e80: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615e82; /* fschg */
L8c615e82: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615e84; /* fmov.s @r4+,fr0 */
L8c615e84: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615e86; /* fmov.s @r4+,fr2 */
L8c615e86: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615e88; /* fldi0 fr3 */
L8c615e88: fcopy(f,8u,1u);goto L8c615e8a; /* fmov fr1,fr8 */
L8c615e8a: fcopy(f,10u,3u);goto L8c615e8c; /* fmov fr3,fr10 */
L8c615e8c: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c615e8e; /* fipr fv8,fv0 */
L8c615e8e: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615e90; /* fmov.s @r4+,fr4 */
L8c615e90: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615e92; /* fmov.s @r4,fr6 */
L8c615e92: fcopy(f,8u,5u);goto L8c615e94; /* fmov fr5,fr8 */
L8c615e94: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615e96; /* fschg */
L8c615e96: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c615e98; /* mov.w @r6+,r2 */
L8c615e98: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c615e9a; /* mov.w @r6,r3 */
L8c615e9a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615e9c; /* fmov.s fr6,@-r8 */
L8c615e9c: *fpul=r[3];goto L8c615e9e; /* lds r3,FPUL */
L8c615e9e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615ea0; /* fmov.s fr5,@-r8 */
L8c615ea0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c615ea2; /* fsub fr3,fr11 */
L8c615ea2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615ea4; /* fmov.s fr4,@-r8 */
L8c615ea4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c615ea6; /* float FPUL,fr1 */
L8c615ea6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615ea8; /* fmul fr8,fr1 */
L8c615ea8: *fpul=r[2];goto L8c615eaa; /* lds r2,FPUL */
L8c615eaa: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615eac; /* mov.l r5,@-r8 */
L8c615eac: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c615eae; /* float FPUL,fr2 */
L8c615eae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615eb0; /* fmul fr8,fr2 */
L8c615eb0: r[8]+=28u;goto L8c615eb2; /* add #0x1c,r8 */
L8c615eb2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c615eb4; /* fmov.s fr11,@-r8 */
L8c615eb4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c615eb6; /* fmov.s fr1,@-r8 */
L8c615eb6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c615eb8; /* fmov.s fr2,@-r8 */
L8c615eb8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615eba; /* pref @r8 */
L8c615eba: r[8]+=16u;goto L8c615ebc; /* add #0x10,r8 */
L8c615ebc: r[2]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c615ebe; /* mov.l @(0x18,r15),r2 */
L8c615ebe: *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c615ec0; /* tst r2,r2 */
L8c615ec0: if((*sr&1u))goto L8c615f96;goto L8c615ec2; /* bt 0x8c615f96 */
L8c615ec2: r[0]=68u;goto L8c615ec4; /* mov #0x44,r0 */
L8c615ec4: r[4]=rd(m,bus,&io_status,0x8c616048u,4);if(io_status)return io_status;goto L8c615ec6; /* mov.l 0x8c616048,r4 */
L8c615ec6: r[3]=0u;goto L8c615ec8; /* mov #0x0,r3 */
L8c615ec8: r[9]=rd(m,bus,&io_status,0x8c61604cu,4);if(io_status)return io_status;goto L8c615eca; /* mov.l 0x8c61604c,r9 */
L8c615eca: wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c615ecc; /* mov.l r3,@r4 */
L8c615ecc: r[4]=r[15];goto L8c615ece; /* mov r15,r4 */
L8c615ece: fload(m,bus,&io_status,f,3u,r[11]);if(io_status)return io_status;goto L8c615ed0; /* fmov.s @r11,fr3 */
L8c615ed0: r[14]+=4294967292u;goto L8c615ed2; /* add #-0x4,r14 */
L8c615ed2: fload(m,bus,&io_status,f,2u,r[10]);if(io_status)return io_status;goto L8c615ed4; /* fmov.s @r10,fr2 */
L8c615ed4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615ed6; /* fadd fr3,fr2 */
L8c615ed6: fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c615ed8; /* fmov.s @r12,fr3 */
L8c615ed8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615eda; /* fadd fr3,fr2 */
L8c615eda: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615edc; /* fmov fr2,@(r0,r15) */
L8c615edc: r[0]=4u;goto L8c615ede; /* mov #0x4,r0 */
L8c615ede: fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c615ee0; /* fmov.s @(r0,r10),fr2 */
L8c615ee0: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c615ee2; /* fmov.s @(r0,r11),fr3 */
L8c615ee2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615ee4; /* fadd fr3,fr2 */
L8c615ee4: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c615ee6; /* fmov.s @(r0,r12),fr3 */
L8c615ee6: r[0]=72u;goto L8c615ee8; /* mov #0x48,r0 */
L8c615ee8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615eea; /* fadd fr3,fr2 */
L8c615eea: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615eec; /* fmov fr2,@(r0,r15) */
L8c615eec: r[0]=8u;goto L8c615eee; /* mov #0x8,r0 */
L8c615eee: fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c615ef0; /* fmov.s @(r0,r11),fr3 */
L8c615ef0: fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c615ef2; /* fmov.s @(r0,r10),fr2 */
L8c615ef2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615ef4; /* fadd fr3,fr2 */
L8c615ef4: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c615ef6; /* fmov.s @(r0,r12),fr3 */
L8c615ef6: r[0]=76u;goto L8c615ef8; /* mov #0x4c,r0 */
L8c615ef8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615efa; /* fadd fr3,fr2 */
L8c615efa: fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615efc; /* fmov fr2,@(r0,r15) */
L8c615efc: target=r[9];*pr=0x8c615f00u;r[4]+=68u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c615f00; /* jsr @r9 */
L8c615f00: r[4]=rd(m,bus,&io_status,0x8c616054u,4);if(io_status)return io_status;goto L8c615f02; /* mov.l 0x8c616054,r4 */
L8c615f02: r[0]=68u;goto L8c615f04; /* mov #0x44,r0 */
L8c615f04: fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c615f06; /* fmov.s @(r0,r15),fr3 */
L8c615f06: r[0]=72u;goto L8c615f08; /* mov #0x48,r0 */
L8c615f08: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c615f0a; /* fmov.s @r4,fr2 */
L8c615f0a: r[5]=rd(m,bus,&io_status,0x8c616050u,4);if(io_status)return io_status;goto L8c615f0c; /* mov.l 0x8c616050,r5 */
L8c615f0c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615f0e; /* fmul fr3,fr2 */
L8c615f0e: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c615f10; /* mov.w @r14,r3 */
L8c615f10: fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c615f12; /* fmov.s @r5,fr3 */
L8c615f12: *fpul=r[3];goto L8c615f14; /* lds r3,FPUL */
L8c615f14: r[9]=rd(m,bus,&io_status,0x8c616058u,4);if(io_status)return io_status;goto L8c615f16; /* mov.l 0x8c616058,r9 */
L8c615f16: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615f18; /* fsub fr2,fr3 */
L8c615f18: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615f1a; /* fmov.s @(r0,r15),fr2 */
L8c615f1a: r[0]=4u;goto L8c615f1c; /* mov #0x4,r0 */
L8c615f1c: fload(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c615f1e; /* fmov.s @(r0,r4),fr1 */
L8c615f1e: r[0]=76u;goto L8c615f20; /* mov #0x4c,r0 */
L8c615f20: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615f22; /* fmul fr2,fr1 */
L8c615f22: fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615f24; /* fmov.s @(r0,r15),fr2 */
L8c615f24: r[0]=8u;goto L8c615f26; /* mov #0x8,r0 */
L8c615f26: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615f28; /* fsub fr1,fr3 */
L8c615f28: fload(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c615f2a; /* fmov.s @(r0,r4),fr1 */
L8c615f2a: r[4]=r[9];goto L8c615f2c; /* mov r9,r4 */
L8c615f2c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615f2e; /* fmul fr2,fr1 */
L8c615f2e: fcopy(f,4u,3u);goto L8c615f30; /* fmov fr3,fr4 */
L8c615f30: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c615f32; /* float FPUL,fr3 */
L8c615f32: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c615f34; /* fsub fr1,fr4 */
L8c615f34: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c615f36; /* ftrc fr3,FPUL */
L8c615f36: r[2]=*fpul;goto L8c615f38; /* sts FPUL,r2 */
L8c615f38: wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c615f3a; /* mov.w r2,@r4 */
L8c615f3a: r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c615f3c; /* mov.w @(0x2,r14),r0 */
L8c615f3c: r[3]=r[0];goto L8c615f3e; /* mov r0,r3 */
L8c615f3e: *fpul=r[3];goto L8c615f40; /* lds r3,FPUL */
L8c615f40: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c615f42; /* float FPUL,fr3 */
L8c615f42: r[14]+=4u;goto L8c615f44; /* add #0x4,r14 */
L8c615f44: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c615f46; /* ftrc fr3,FPUL */
L8c615f46: r[0]=*fpul;goto L8c615f48; /* sts FPUL,r0 */
L8c615f48: wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c615f4a; /* mov.w r0,@(0x2,r4) */
L8c615f4a: r[4]+=4u;goto L8c615f4c; /* add #0x4,r4 */
L8c615f4c: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c615f4e; /* mov.w @r14,r3 */
L8c615f4e: *fpul=r[3];goto L8c615f50; /* lds r3,FPUL */
L8c615f50: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c615f52; /* float FPUL,fr3 */
L8c615f52: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c615f54; /* ftrc fr3,FPUL */
L8c615f54: r[2]=*fpul;goto L8c615f56; /* sts FPUL,r2 */
L8c615f56: wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c615f58; /* mov.w r2,@r4 */
L8c615f58: r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c615f5a; /* mov.w @(0x2,r14),r0 */
L8c615f5a: r[14]+=4u;goto L8c615f5c; /* add #0x4,r14 */
L8c615f5c: r[3]=r[0];goto L8c615f5e; /* mov r0,r3 */
L8c615f5e: *fpul=r[3];goto L8c615f60; /* lds r3,FPUL */
L8c615f60: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c615f62; /* float FPUL,fr3 */
L8c615f62: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c615f64; /* ftrc fr3,FPUL */
L8c615f64: r[0]=*fpul;goto L8c615f66; /* sts FPUL,r0 */
L8c615f66: wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c615f68; /* mov.w r0,@(0x2,r4) */
L8c615f68: r[4]=r[9];goto L8c615f6a; /* mov r9,r4 */
L8c615f6a: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c615f6c; /* mov.w @r14,r3 */
L8c615f6c: r[4]+=8u;goto L8c615f6e; /* add #0x8,r4 */
L8c615f6e: *fpul=r[3];goto L8c615f70; /* lds r3,FPUL */
L8c615f70: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c615f72; /* float FPUL,fr3 */
L8c615f72: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c615f74; /* ftrc fr3,FPUL */
L8c615f74: r[2]=*fpul;goto L8c615f76; /* sts FPUL,r2 */
L8c615f76: wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c615f78; /* mov.w r2,@r4 */
L8c615f78: r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c615f7a; /* mov.w @(0x2,r14),r0 */
L8c615f7a: r[3]=r[0];goto L8c615f7c; /* mov r0,r3 */
L8c615f7c: *fpul=r[3];goto L8c615f7e; /* lds r3,FPUL */
L8c615f7e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c615f80; /* float FPUL,fr3 */
L8c615f80: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c615f82; /* ftrc fr3,FPUL */
L8c615f82: r[3]=rd(m,bus,&io_status,0x8c61605cu,4);if(io_status)return io_status;goto L8c615f84; /* mov.l 0x8c61605c,r3 */
L8c615f84: r[5]=r[11];goto L8c615f86; /* mov r11,r5 */
L8c615f86: r[6]=r[12];goto L8c615f88; /* mov r12,r6 */
L8c615f88: r[7]=r[9];goto L8c615f8a; /* mov r9,r7 */
L8c615f8a: r[0]=*fpul;goto L8c615f8c; /* sts FPUL,r0 */
L8c615f8c: wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c615f8e; /* mov.w r0,@(0x2,r4) */
L8c615f8e: target=r[3];*pr=0x8c615f92u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c615f92; /* jsr @r3 */
L8c615f92: r[14]+=4294967292u;goto L8c615f94; /* add #-0x4,r14 */
L8c615f94: r[9]=r[8];goto L8c615f96; /* mov r8,r9 */
L8c615f96: fcopy(f,5u,13u);goto L8c615f98; /* fmov fr13,fr5 */
L8c615f98: r[0]=16u;goto L8c615f9a; /* mov #0x10,r0 */
L8c615f9a: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615f9c; /* mov.l @(0x8,r15),r3 */
L8c615f9c: r[10]=r[11];goto L8c615f9e; /* mov r11,r10 */
L8c615f9e: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c615fa0; /* fmov.s @(r0,r12),fr13 */
L8c615fa0: r[0]=20u;goto L8c615fa2; /* mov #0x14,r0 */
L8c615fa2: fcopy(f,6u,14u);goto L8c615fa4; /* fmov fr14,fr6 */
L8c615fa4: r[3]+=4294967295u;goto L8c615fa6; /* add #-0x1,r3 */
L8c615fa6: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c615fa8; /* fmov.s @(r0,r12),fr14 */
L8c615fa8: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c615faa; /* tst r3,r3 */
L8c615faa: r[11]=r[12];goto L8c615fac; /* mov r12,r11 */
L8c615fac: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c615fae; /* fneg fr15 */
L8c615fae: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c615fb6;goto L8c615fb2; /* bt/s 0x8c615fb6 */
L8c615fb2: goto L8c615c3e; /* bra 0x8c615c3e */
L8c615fb6: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c615fb8; /* cmp/hs r8,r9 */
L8c615fb8: take=(*sr&1u);r[14]+=8u;if(take)goto L8c616004;goto L8c615fbc; /* bt/s 0x8c616004 */
L8c615fbc: r[14]+=4294967288u;goto L8c615fbe; /* add #-0x8,r14 */
L8c615fbe: r[5]=rd(m,bus,&io_status,0x8c616040u,4);if(io_status)return io_status;goto L8c615fc0; /* mov.l 0x8c616040,r5 */
L8c615fc0: r[6]=r[14];goto L8c615fc2; /* mov r14,r6 */
L8c615fc2: r[4]=r[12];goto L8c615fc4; /* mov r12,r4 */
L8c615fc4: r[14]+=4u;goto L8c615fc6; /* add #0x4,r14 */
L8c615fc6: r[8]+=16u;goto L8c615fc8; /* add #0x10,r8 */
L8c615fc8: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615fca; /* fschg */
L8c615fca: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615fcc; /* fmov.s @r4+,fr0 */
L8c615fcc: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615fce; /* fmov.s @r4+,fr2 */
L8c615fce: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615fd0; /* fldi0 fr3 */
L8c615fd0: fcopy(f,8u,1u);goto L8c615fd2; /* fmov fr1,fr8 */
L8c615fd2: fcopy(f,10u,3u);goto L8c615fd4; /* fmov fr3,fr10 */
L8c615fd4: status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c615fd6; /* fipr fv8,fv0 */
L8c615fd6: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615fd8; /* fmov.s @r4+,fr4 */
L8c615fd8: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615fda; /* fmov.s @r4,fr6 */
L8c615fda: fcopy(f,8u,5u);goto L8c615fdc; /* fmov fr5,fr8 */
L8c615fdc: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615fde; /* fschg */
L8c615fde: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c615fe0; /* mov.w @r6+,r2 */
L8c615fe0: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c615fe2; /* mov.w @r6,r3 */
L8c615fe2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615fe4; /* fmov.s fr6,@-r8 */
L8c615fe4: *fpul=r[3];goto L8c615fe6; /* lds r3,FPUL */
L8c615fe6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615fe8; /* fmov.s fr5,@-r8 */
L8c615fe8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c615fea; /* fsub fr3,fr11 */
L8c615fea: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615fec; /* fmov.s fr4,@-r8 */
L8c615fec: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c615fee; /* float FPUL,fr1 */
L8c615fee: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615ff0; /* fmul fr8,fr1 */
L8c615ff0: *fpul=r[2];goto L8c615ff2; /* lds r2,FPUL */
L8c615ff2: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615ff4; /* mov.l r5,@-r8 */
L8c615ff4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c615ff6; /* float FPUL,fr2 */
L8c615ff6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615ff8; /* fmul fr8,fr2 */
L8c615ff8: r[8]+=28u;goto L8c615ffa; /* add #0x1c,r8 */
L8c615ffa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c615ffc; /* fmov.s fr11,@-r8 */
L8c615ffc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c615ffe; /* fmov.s fr1,@-r8 */
L8c615ffe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616000; /* fmov.s fr2,@-r8 */
L8c616000: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616002; /* pref @r8 */
L8c616002: r[8]+=16u;goto L8c616004; /* add #0x10,r8 */
L8c616004: r[3]=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;goto L8c616006; /* mov.l @(0x1c,r15),r3 */
L8c616006: r[3]+=4294967295u;goto L8c616008; /* add #-0x1,r3 */
L8c616008: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c61600a; /* tst r3,r3 */
L8c61600a: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x1cu,r[3],4);if(io_status)return io_status;if(take)goto L8c616012;goto L8c61600e; /* bt/s 0x8c616012 */
L8c61600e: goto L8c615bec; /* bra 0x8c615bec */
L8c616012: r[4]=rd(m,bus,&io_status,0x8c616060u,4);if(io_status)return io_status;goto L8c616014; /* mov.l 0x8c616060,r4 */
L8c616014: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c616016; /* mov.l @(0x10,r15),r3 */
L8c616016: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616018; /* mov.l @r4,r2 */
L8c616018: r[2]+=r[3];goto L8c61601a; /* add r3,r2 */
L8c61601a: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c61601c; /* mov.l r2,@r4 */
L8c61601c: r[4]=rd(m,bus,&io_status,0x8c616064u,4);if(io_status)return io_status;goto L8c61601e; /* mov.l 0x8c616064,r4 */
L8c61601e: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c616020; /* mov.l @(0x10,r15),r3 */
L8c616020: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616022; /* mov.l @r4,r2 */
L8c616022: r[2]+=r[3];goto L8c616024; /* add r3,r2 */
L8c616024: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c616026; /* mov.l r2,@r4 */
L8c616026: r[15]+=80u;goto L8c616028; /* add #0x50,r15 */
L8c616028: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61602a; /* lds.l @r15+,PR */
L8c61602a: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61602c; /* fmov.s @r15+,fr12 */
L8c61602c: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61602e; /* fmov.s @r15+,fr13 */
L8c61602e: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616030; /* fmov.s @r15+,fr14 */
L8c616030: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616032; /* fmov.s @r15+,fr15 */
L8c616032: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616034; /* mov.l @r15+,r9 */
L8c616034: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616036; /* mov.l @r15+,r10 */
L8c616036: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616038; /* mov.l @r15+,r11 */
L8c616038: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61603a; /* mov.l @r15+,r12 */
L8c61603a: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61603c; /* mov.l @r15+,r13 */
L8c61603c: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
