/* Environment-constant alternate strip emitters with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_environment_constant_alternate_strip_8c614796(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c614796;
L8c614796: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c614798; /* mov.l r14,@-r15 */
L8c614798: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61479a; /* mov.l r13,@-r15 */
L8c61479a: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61479c; /* mov.l r12,@-r15 */
L8c61479c: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61479e; /* mov.l r11,@-r15 */
L8c61479e: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6147a0; /* mov.l r10,@-r15 */
L8c6147a0: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6147a2; /* mov.l r9,@-r15 */
L8c6147a2: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c6147a4; /* fmov.s fr15,@-r15 */
L8c6147a4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c6147a6; /* fmov.s fr14,@-r15 */
L8c6147a6: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c6147a8; /* fmov.s fr13,@-r15 */
L8c6147a8: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6147aa; /* sts.l PR,@-r15 */
L8c6147aa: r[15]+=4294967280u;goto L8c6147ac; /* add #-0x10,r15 */
L8c6147ac: r[3]=rd(m,bus,&io_status,0x8c6147e0u,4);if(io_status)return io_status;goto L8c6147ae; /* mov.l 0x8c6147e0,r3 */
L8c6147ae: r[9]=rd(m,bus,&io_status,0x8c6147dcu,4);if(io_status)return io_status;goto L8c6147b0; /* mov.l 0x8c6147dc,r9 */
L8c6147b0: target=r[3];*pr=0x8c6147b4u;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6147b4; /* jsr @r3 */
L8c6147b4: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6147b6; /* mov.l @(0x4,r15),r3 */
L8c6147b6: r[2]=0u;goto L8c6147b8; /* mov #0x0,r2 */
L8c6147b8: r[14]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6147ba; /* mov.l @(0x4,r15),r14 */
L8c6147ba: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6147bc; /* mov.w @(0x2,r3),r0 */
L8c6147bc: r[3]=rd(m,bus,&io_status,0x8c6147e4u,4);if(io_status)return io_status;goto L8c6147be; /* mov.l 0x8c6147e4,r3 */
L8c6147be: r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c6147c0; /* mov.l @(0x4,r14),r14 */
L8c6147c0: r[0]=r[0]&65535u;goto L8c6147c2; /* extu.w r0,r0 */
L8c6147c2: wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c6147c4; /* mov.l r0,@(0xc,r15) */
L8c6147c4: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6147c6; /* mov.l @r3,r13 */
L8c6147c6: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6147c8; /* mov.l r2,@(0x8,r15) */
L8c6147c8: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6147ca; /* mov.l r2,@(0x4,r15) */
L8c6147ca: goto L8c6147e8; /* bra 0x8c6147e8 */
L8c6147e8: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6147ea; /* mov.w @r14+,r3 */
L8c6147ea: r[4]=rd(m,bus,&io_status,0x8c6148bcu,4);if(io_status)return io_status;goto L8c6147ec; /* mov.l 0x8c6148bc,r4 */
L8c6147ec: r[3]=r[3]&65535u;goto L8c6147ee; /* extu.w r3,r3 */
L8c6147ee: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6147f0; /* fldi1 fr4 */
L8c6147f0: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6147f2; /* tst r4,r3 */
L8c6147f2: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6147f4; /* mov.l r3,@r15 */
L8c6147f4: take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c614806;goto L8c6147f8; /* bt/s 0x8c614806 */
L8c6147f8: r[4]=sx(m,bus,&io_status,0x8c6148b8u,2);if(io_status)return io_status;goto L8c6147fa; /* mov.w 0x8c6148b8,r4 */
L8c6147fa: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6147fc; /* mov.l @r15,r3 */
L8c6147fc: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6147fe; /* fldi1 fr4 */
L8c6147fe: r[3]&=r[4];goto L8c614800; /* and r4,r3 */
L8c614800: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c614802; /* fneg fr4 */
L8c614802: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c614804; /* mov.l r3,@r15 */
L8c614804: fcopy(f,15u,4u);goto L8c614806; /* fmov fr4,fr15 */
L8c614806: r[0]=16u;goto L8c614808; /* mov #0x10,r0 */
L8c614808: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61480a; /* mov.l @r15,r3 */
L8c61480a: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61480c; /* mov.l @(0x4,r15),r2 */
L8c61480c: r[10]=r[8];goto L8c61480e; /* mov r8,r10 */
L8c61480e: r[2]+=r[3];goto L8c614810; /* add r3,r2 */
L8c614810: wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c614812; /* mov.l r2,@(0x4,r15) */
L8c614812: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614814; /* mov.l @r15,r1 */
L8c614814: r[1]+=4294967294u;goto L8c614816; /* add #-0x2,r1 */
L8c614816: wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c614818; /* mov.l r1,@r15 */
L8c614818: r[4]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c61481a; /* mov.w @r14+,r4 */
L8c61481a: r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c61481c; /* mov.w @r14+,r11 */
L8c61481c: r[4]=r[4]&65535u;goto L8c61481e; /* extu.w r4,r4 */
L8c61481e: r[4]<<=2;goto L8c614820; /* shll2 r4 */
L8c614820: r[11]=r[11]&65535u;goto L8c614822; /* extu.w r11,r11 */
L8c614822: r[11]<<=2;goto L8c614824; /* shll2 r11 */
L8c614824: r[4]<<=2;goto L8c614826; /* shll2 r4 */
L8c614826: r[11]<<=2;goto L8c614828; /* shll2 r11 */
L8c614828: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61482a; /* shll r4 */
L8c61482a: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61482c; /* shll r11 */
L8c61482c: r[11]+=r[13];goto L8c61482e; /* add r13,r11 */
L8c61482e: r[4]+=r[13];goto L8c614830; /* add r13,r4 */
L8c614830: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c614832; /* fmov.s @(r0,r11),fr14 */
L8c614832: fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c614834; /* fmov.s @(r0,r4),fr4 */
L8c614834: r[0]=20u;goto L8c614836; /* mov #0x14,r0 */
L8c614836: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c614838; /* fmov.s @(r0,r11),fr13 */
L8c614838: fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c61483a; /* fmov.s @(r0,r4),fr5 */
L8c61483a: r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c61483c; /* mov.w @r14+,r12 */
L8c61483c: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c61483e; /* mov.w @r14,r3 */
L8c61483e: r[12]=r[12]&65535u;goto L8c614840; /* extu.w r12,r12 */
L8c614840: r[12]<<=2;goto L8c614842; /* shll2 r12 */
L8c614842: r[3]=r[3]&65535u;goto L8c614844; /* extu.w r3,r3 */
L8c614844: r[3]<<=2;goto L8c614846; /* shll2 r3 */
L8c614846: r[12]<<=2;goto L8c614848; /* shll2 r12 */
L8c614848: r[3]<<=2;goto L8c61484a; /* shll2 r3 */
L8c61484a: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61484c; /* shll r12 */
L8c61484c: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c61484e; /* shll r3 */
L8c61484e: r[3]+=r[13];goto L8c614850; /* add r13,r3 */
L8c614850: r[12]+=r[13];goto L8c614852; /* add r13,r12 */
L8c614852: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c614854; /* pref @r3 */
L8c614854: r[0]=20u;goto L8c614856; /* mov #0x14,r0 */
L8c614856: fcopy(f,3u,14u);goto L8c614858; /* fmov fr14,fr3 */
L8c614858: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61485a; /* fmov.s @(r0,r12),fr2 */
L8c61485a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61485c; /* fsub fr4,fr3 */
L8c61485c: r[0]=16u;goto L8c61485e; /* mov #0x10,r0 */
L8c61485e: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c614860; /* cmp/hs r8,r10 */
L8c614860: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614862; /* fsub fr13,fr2 */
L8c614862: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c614864; /* fmov.s @(r0,r12),fr1 */
L8c614864: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c614866; /* fsub fr14,fr1 */
L8c614866: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614868; /* fmul fr2,fr3 */
L8c614868: fcopy(f,2u,13u);goto L8c61486a; /* fmov fr13,fr2 */
L8c61486a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61486c; /* fsub fr5,fr2 */
L8c61486c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61486e; /* fmul fr1,fr2 */
L8c61486e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614870; /* fsub fr2,fr3 */
L8c614870: fcopy(f,4u,3u);goto L8c614872; /* fmov fr3,fr4 */
L8c614872: take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c6148c0;goto L8c614876; /* bt/s 0x8c6148c0 */
L8c614876: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614878; /* fldi0 fr3 */
L8c614878: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c61487a; /* fcmp/gt fr4,fr3 */
L8c61487a: if((*sr&1u))goto L8c6148f0;goto L8c61487c; /* bt 0x8c6148f0 */
L8c61487c: r[4]=r[11];goto L8c61487e; /* mov r11,r4 */
L8c61487e: r[5]=r[9];goto L8c614880; /* mov r9,r5 */
L8c614880: r[8]+=16u;goto L8c614882; /* add #0x10,r8 */
L8c614882: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614884; /* fschg */
L8c614884: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614886; /* fmov.s @r4+,fr0 */
L8c614886: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614888; /* fmov.s @r4+,fr2 */
L8c614888: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c61488a; /* fldi1 fr3 */
L8c61488a: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c61488c; /* ftrv xmtrx,fv0 */
L8c61488c: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61488e; /* fmov.s @r4+,fr4 */
L8c61488e: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614890; /* fmov.s @r4,fr6 */
L8c614890: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614892; /* fschg */
L8c614892: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614894; /* fmov.s fr6,@-r8 */
L8c614894: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614896; /* fmov.s fr5,@-r8 */
L8c614896: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614898; /* fmov.s fr4,@-r8 */
L8c614898: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61489a; /* mov.l r5,@-r8 */
L8c61489a: r[8]+=28u;goto L8c61489c; /* add #0x1c,r8 */
L8c61489c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c61489e; /* fldi1 fr2 */
L8c61489e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6148a0; /* fmov.s fr2,@-r8 */
L8c6148a0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6148a2; /* fmov.s fr1,@-r8 */
L8c6148a2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c6148a4; /* fmov.s fr0,@-r8 */
L8c6148a4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6148a6; /* pref @r8 */
L8c6148a6: r[8]+=16u;goto L8c6148a8; /* add #0x10,r8 */
L8c6148a8: r[3]=r[8];goto L8c6148aa; /* mov r8,r3 */
L8c6148aa: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6148ac; /* mov.l @(0x8,r15),r2 */
L8c6148ac: r[3]-=r[10];goto L8c6148ae; /* sub r10,r3 */
L8c6148ae: r[3]+=32u;goto L8c6148b0; /* add #0x20,r3 */
L8c6148b0: r[2]+=r[3];goto L8c6148b2; /* add r3,r2 */
L8c6148b2: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6148b4; /* mov.l r2,@(0x8,r15) */
L8c6148b4: r[10]=r[8];goto L8c61491c; /* bra 0x8c61491c */
L8c6148c0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6148c2; /* fldi0 fr3 */
L8c6148c2: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6148c4; /* fcmp/gt fr4,fr3 */
L8c6148c4: if(!(*sr&1u))goto L8c61491c;goto L8c6148c6; /* bf 0x8c61491c */
L8c6148c6: r[5]=rd(m,bus,&io_status,0x8c6149ecu,4);if(io_status)return io_status;goto L8c6148c8; /* mov.l 0x8c6149ec,r5 */
L8c6148c8: r[8]+=16u;goto L8c6148ca; /* add #0x10,r8 */
L8c6148ca: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6148cc; /* fschg */
L8c6148cc: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6148ce; /* fmov.s @r4+,fr0 */
L8c6148ce: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6148d0; /* fmov.s @r4+,fr2 */
L8c6148d0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6148d2; /* fldi1 fr3 */
L8c6148d2: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6148d4; /* ftrv xmtrx,fv0 */
L8c6148d4: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6148d6; /* fmov.s @r4+,fr4 */
L8c6148d6: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6148d8; /* fmov.s @r4,fr6 */
L8c6148d8: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6148da; /* fschg */
L8c6148da: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6148dc; /* fmov.s fr6,@-r8 */
L8c6148dc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6148de; /* fmov.s fr5,@-r8 */
L8c6148de: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6148e0; /* fmov.s fr4,@-r8 */
L8c6148e0: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6148e2; /* mov.l r5,@-r8 */
L8c6148e2: r[8]+=28u;goto L8c6148e4; /* add #0x1c,r8 */
L8c6148e4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c6148e6; /* fldi1 fr2 */
L8c6148e6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6148e8; /* fmov.s fr2,@-r8 */
L8c6148e8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6148ea; /* fmov.s fr1,@-r8 */
L8c6148ea: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c6148ec; /* fmov.s fr0,@-r8 */
L8c6148ec: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6148ee; /* pref @r8 */
L8c6148ee: r[8]+=16u;goto L8c6148f0; /* add #0x10,r8 */
L8c6148f0: r[5]=rd(m,bus,&io_status,0x8c6149ecu,4);if(io_status)return io_status;goto L8c6148f2; /* mov.l 0x8c6149ec,r5 */
L8c6148f2: r[4]=r[11];goto L8c6148f4; /* mov r11,r4 */
L8c6148f4: r[8]+=16u;goto L8c6148f6; /* add #0x10,r8 */
L8c6148f6: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6148f8; /* fschg */
L8c6148f8: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6148fa; /* fmov.s @r4+,fr0 */
L8c6148fa: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6148fc; /* fmov.s @r4+,fr2 */
L8c6148fc: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6148fe; /* fldi1 fr3 */
L8c6148fe: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c614900; /* ftrv xmtrx,fv0 */
L8c614900: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614902; /* fmov.s @r4+,fr4 */
L8c614902: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614904; /* fmov.s @r4,fr6 */
L8c614904: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614906; /* fschg */
L8c614906: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614908; /* fmov.s fr6,@-r8 */
L8c614908: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61490a; /* fmov.s fr5,@-r8 */
L8c61490a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61490c; /* fmov.s fr4,@-r8 */
L8c61490c: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61490e; /* mov.l r5,@-r8 */
L8c61490e: r[8]+=28u;goto L8c614910; /* add #0x1c,r8 */
L8c614910: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c614912; /* fldi1 fr2 */
L8c614912: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614914; /* fmov.s fr2,@-r8 */
L8c614914: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c614916; /* fmov.s fr1,@-r8 */
L8c614916: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c614918; /* fmov.s fr0,@-r8 */
L8c614918: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61491a; /* pref @r8 */
L8c61491a: r[8]+=16u;goto L8c61491c; /* add #0x10,r8 */
L8c61491c: fcopy(f,4u,14u);goto L8c61491e; /* fmov fr14,fr4 */
L8c61491e: r[0]=16u;goto L8c614920; /* mov #0x10,r0 */
L8c614920: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614922; /* mov.l @r15,r3 */
L8c614922: r[4]=r[11];goto L8c614924; /* mov r11,r4 */
L8c614924: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c614926; /* fmov.s @(r0,r12),fr14 */
L8c614926: r[0]=20u;goto L8c614928; /* mov #0x14,r0 */
L8c614928: fcopy(f,5u,13u);goto L8c61492a; /* fmov fr13,fr5 */
L8c61492a: r[3]+=4294967295u;goto L8c61492c; /* add #-0x1,r3 */
L8c61492c: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c61492e; /* fmov.s @(r0,r12),fr13 */
L8c61492e: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614930; /* tst r3,r3 */
L8c614930: r[11]=r[12];goto L8c614932; /* mov r12,r11 */
L8c614932: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c614934; /* fneg fr15 */
L8c614934: take=!(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c61483a;goto L8c614938; /* bf/s 0x8c61483a */
L8c614938: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c61493a; /* cmp/hs r8,r10 */
L8c61493a: if((*sr&1u))goto L8c614974;goto L8c61493c; /* bt 0x8c614974 */
L8c61493c: r[4]=r[12];goto L8c61493e; /* mov r12,r4 */
L8c61493e: r[5]=r[9];goto L8c614940; /* mov r9,r5 */
L8c614940: r[8]+=16u;goto L8c614942; /* add #0x10,r8 */
L8c614942: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614944; /* fschg */
L8c614944: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614946; /* fmov.s @r4+,fr0 */
L8c614946: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614948; /* fmov.s @r4+,fr2 */
L8c614948: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c61494a; /* fldi1 fr3 */
L8c61494a: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c61494c; /* ftrv xmtrx,fv0 */
L8c61494c: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61494e; /* fmov.s @r4+,fr4 */
L8c61494e: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614950; /* fmov.s @r4,fr6 */
L8c614950: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614952; /* fschg */
L8c614952: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614954; /* fmov.s fr6,@-r8 */
L8c614954: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614956; /* fmov.s fr5,@-r8 */
L8c614956: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614958; /* fmov.s fr4,@-r8 */
L8c614958: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61495a; /* mov.l r5,@-r8 */
L8c61495a: r[8]+=28u;goto L8c61495c; /* add #0x1c,r8 */
L8c61495c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c61495e; /* fldi1 fr2 */
L8c61495e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614960; /* fmov.s fr2,@-r8 */
L8c614960: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c614962; /* fmov.s fr1,@-r8 */
L8c614962: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c614964; /* fmov.s fr0,@-r8 */
L8c614964: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614966; /* pref @r8 */
L8c614966: r[8]+=16u;goto L8c614968; /* add #0x10,r8 */
L8c614968: r[3]=r[8];goto L8c61496a; /* mov r8,r3 */
L8c61496a: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61496c; /* mov.l @(0x8,r15),r2 */
L8c61496c: r[3]-=r[10];goto L8c61496e; /* sub r10,r3 */
L8c61496e: r[3]+=32u;goto L8c614970; /* add #0x20,r3 */
L8c614970: r[2]+=r[3];goto L8c614972; /* add r3,r2 */
L8c614972: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c614974; /* mov.l r2,@(0x8,r15) */
L8c614974: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614976; /* mov.l @(0xc,r15),r3 */
L8c614976: r[3]+=4294967295u;goto L8c614978; /* add #-0x1,r3 */
L8c614978: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c61497a; /* tst r3,r3 */
L8c61497a: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c614982;goto L8c61497e; /* bt/s 0x8c614982 */
L8c61497e: goto L8c6147e8; /* bra 0x8c6147e8 */
L8c614982: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614984; /* mov.l @(0x8,r15),r3 */
L8c614984: r[4]=rd(m,bus,&io_status,0x8c6149f0u,4);if(io_status)return io_status;goto L8c614986; /* mov.l 0x8c6149f0,r4 */
L8c614986: r[3]>>=2;goto L8c614988; /* shlr2 r3 */
L8c614988: r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61498a; /* mov.l @r4,r1 */
L8c61498a: r[3]>>=2;goto L8c61498c; /* shlr2 r3 */
L8c61498c: *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c61498e; /* shlr r3 */
L8c61498e: r[1]+=r[3];goto L8c614990; /* add r3,r1 */
L8c614990: wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c614992; /* mov.l r1,@r4 */
L8c614992: r[4]=rd(m,bus,&io_status,0x8c6149f4u,4);if(io_status)return io_status;goto L8c614994; /* mov.l 0x8c6149f4,r4 */
L8c614994: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614996; /* mov.l @(0x4,r15),r3 */
L8c614996: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c614998; /* mov.l @r4,r2 */
L8c614998: r[2]+=r[3];goto L8c61499a; /* add r3,r2 */
L8c61499a: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c61499c; /* mov.l r2,@r4 */
L8c61499c: r[15]+=16u;goto L8c61499e; /* add #0x10,r15 */
L8c61499e: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6149a0; /* lds.l @r15+,PR */
L8c6149a0: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6149a2; /* fmov.s @r15+,fr13 */
L8c6149a2: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6149a4; /* fmov.s @r15+,fr14 */
L8c6149a4: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6149a6; /* fmov.s @r15+,fr15 */
L8c6149a6: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6149a8; /* mov.l @r15+,r9 */
L8c6149a8: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6149aa; /* mov.l @r15+,r10 */
L8c6149aa: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6149ac; /* mov.l @r15+,r11 */
L8c6149ac: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6149ae; /* mov.l @r15+,r12 */
L8c6149ae: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6149b0; /* mov.l @r15+,r13 */
L8c6149b0: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_environment_constant_alternate_strip_clipped_8c616844(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c616844;
L8c616844: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c616846; /* mov.l r14,@-r15 */
L8c616846: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c616848; /* mov.l r13,@-r15 */
L8c616848: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61684a; /* mov.l r12,@-r15 */
L8c61684a: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61684c; /* mov.l r11,@-r15 */
L8c61684c: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61684e; /* mov.l r10,@-r15 */
L8c61684e: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c616850; /* mov.l r9,@-r15 */
L8c616850: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c616852; /* fmov.s fr15,@-r15 */
L8c616852: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c616854; /* fmov.s fr14,@-r15 */
L8c616854: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c616856; /* fmov.s fr13,@-r15 */
L8c616856: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c616858; /* fmov.s fr12,@-r15 */
L8c616858: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61685a; /* sts.l PR,@-r15 */
L8c61685a: r[15]+=4294967220u;goto L8c61685c; /* add #-0x4c,r15 */
L8c61685c: r[3]=rd(m,bus,&io_status,0x8c6168a0u,4);if(io_status)return io_status;goto L8c61685e; /* mov.l 0x8c6168a0,r3 */
L8c61685e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c616860; /* fldi1 fr12 */
L8c616860: target=r[3];*pr=0x8c616864u;wr(m,bus,&io_status,r[15]+0xcu,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c616864; /* jsr @r3 */
L8c616864: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616866; /* mov.l @(0xc,r15),r3 */
L8c616866: r[2]=0u;goto L8c616868; /* mov #0x0,r2 */
L8c616868: r[14]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61686a; /* mov.l @(0xc,r15),r14 */
L8c61686a: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61686c; /* mov.w @(0x2,r3),r0 */
L8c61686c: r[3]=rd(m,bus,&io_status,0x8c6168a4u,4);if(io_status)return io_status;goto L8c61686e; /* mov.l 0x8c6168a4,r3 */
L8c61686e: r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c616870; /* mov.l @(0x4,r14),r14 */
L8c616870: r[0]=r[0]&65535u;goto L8c616872; /* extu.w r0,r0 */
L8c616872: wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c616874; /* mov.l r0,@(0x18,r15) */
L8c616874: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c616876; /* mov.l @r3,r13 */
L8c616876: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c616878; /* mov.l r2,@(0xc,r15) */
L8c616878: goto L8c6168a8; /* bra 0x8c6168a8 */
L8c6168a8: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6168aa; /* mov.w @r14+,r3 */
L8c6168aa: r[4]=rd(m,bus,&io_status,0x8c6169c4u,4);if(io_status)return io_status;goto L8c6168ac; /* mov.l 0x8c6169c4,r4 */
L8c6168ac: r[3]=r[3]&65535u;goto L8c6168ae; /* extu.w r3,r3 */
L8c6168ae: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6168b0; /* tst r4,r3 */
L8c6168b0: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6168b2; /* mov.l r3,@(0x8,r15) */
L8c6168b2: take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c6168c4;goto L8c6168b6; /* bt/s 0x8c6168c4 */
L8c6168b6: r[4]=sx(m,bus,&io_status,0x8c6169c0u,2);if(io_status)return io_status;goto L8c6168b8; /* mov.w 0x8c6169c0,r4 */
L8c6168b8: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6168ba; /* mov.l @(0x8,r15),r3 */
L8c6168ba: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6168bc; /* fldi1 fr4 */
L8c6168bc: r[3]&=r[4];goto L8c6168be; /* and r4,r3 */
L8c6168be: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c6168c0; /* fneg fr4 */
L8c6168c0: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6168c2; /* mov.l r3,@(0x8,r15) */
L8c6168c2: fcopy(f,15u,4u);goto L8c6168c4; /* fmov fr4,fr15 */
L8c6168c4: r[0]=16u;goto L8c6168c6; /* mov #0x10,r0 */
L8c6168c6: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6168c8; /* mov.l @(0x8,r15),r3 */
L8c6168c8: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6168ca; /* mov.l @(0xc,r15),r2 */
L8c6168ca: r[9]=r[8];goto L8c6168cc; /* mov r8,r9 */
L8c6168cc: r[2]+=r[3];goto L8c6168ce; /* add r3,r2 */
L8c6168ce: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6168d0; /* mov.l r2,@(0xc,r15) */
L8c6168d0: r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6168d2; /* mov.l @(0x8,r15),r1 */
L8c6168d2: r[1]+=4294967294u;goto L8c6168d4; /* add #-0x2,r1 */
L8c6168d4: wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c6168d6; /* mov.l r1,@(0x8,r15) */
L8c6168d6: r[10]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6168d8; /* mov.w @r14+,r10 */
L8c6168d8: r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6168da; /* mov.w @r14+,r11 */
L8c6168da: r[10]=r[10]&65535u;goto L8c6168dc; /* extu.w r10,r10 */
L8c6168dc: r[10]<<=2;goto L8c6168de; /* shll2 r10 */
L8c6168de: r[11]=r[11]&65535u;goto L8c6168e0; /* extu.w r11,r11 */
L8c6168e0: r[11]<<=2;goto L8c6168e2; /* shll2 r11 */
L8c6168e2: r[10]<<=2;goto L8c6168e4; /* shll2 r10 */
L8c6168e4: r[11]<<=2;goto L8c6168e6; /* shll2 r11 */
L8c6168e6: *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c6168e8; /* shll r10 */
L8c6168e8: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c6168ea; /* shll r11 */
L8c6168ea: r[11]+=r[13];goto L8c6168ec; /* add r13,r11 */
L8c6168ec: r[10]+=r[13];goto L8c6168ee; /* add r13,r10 */
L8c6168ee: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c6168f0; /* fmov.s @(r0,r11),fr13 */
L8c6168f0: fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c6168f2; /* fmov.s @(r0,r10),fr6 */
L8c6168f2: r[0]=20u;goto L8c6168f4; /* mov #0x14,r0 */
L8c6168f4: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c6168f6; /* fmov.s @(r0,r11),fr14 */
L8c6168f6: fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c6168f8; /* fmov.s @(r0,r10),fr5 */
L8c6168f8: r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6168fa; /* mov.w @r14+,r12 */
L8c6168fa: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c6168fc; /* mov.w @r14,r3 */
L8c6168fc: r[12]=r[12]&65535u;goto L8c6168fe; /* extu.w r12,r12 */
L8c6168fe: r[12]<<=2;goto L8c616900; /* shll2 r12 */
L8c616900: r[3]=r[3]&65535u;goto L8c616902; /* extu.w r3,r3 */
L8c616902: r[3]<<=2;goto L8c616904; /* shll2 r3 */
L8c616904: r[12]<<=2;goto L8c616906; /* shll2 r12 */
L8c616906: r[3]<<=2;goto L8c616908; /* shll2 r3 */
L8c616908: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61690a; /* shll r12 */
L8c61690a: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c61690c; /* shll r3 */
L8c61690c: r[3]+=r[13];goto L8c61690e; /* add r13,r3 */
L8c61690e: r[12]+=r[13];goto L8c616910; /* add r13,r12 */
L8c616910: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c616912; /* pref @r3 */
L8c616912: r[2]=0u;goto L8c616914; /* mov #0x0,r2 */
L8c616914: wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c616916; /* mov.l r2,@(0x14,r15) */
L8c616916: r[4]=63u;goto L8c616918; /* mov #0x3f,r4 */
L8c616918: r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c61691a; /* mov.l @(0xc,r10),r2 */
L8c61691a: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61691c; /* mov.l @(0xc,r11),r3 */
L8c61691c: r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61691e; /* mov.l @(0xc,r12),r1 */
L8c61691e: r[2]&=r[3];goto L8c616920; /* and r3,r2 */
L8c616920: r[2]&=r[1];goto L8c616922; /* and r1,r2 */
L8c616922: r[4]&=r[2];goto L8c616924; /* and r2,r4 */
L8c616924: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c616926; /* tst r4,r4 */
L8c616926: take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c61692e;goto L8c61692a; /* bt/s 0x8c61692e */
L8c61692a: goto L8c616a88; /* bra 0x8c616a88 */
L8c61692e: r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c616930; /* mov.l @(0xc,r10),r4 */
L8c616930: r[5]=1u;goto L8c616932; /* mov #0x1,r5 */
L8c616932: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c616934; /* mov.l @(0xc,r11),r3 */
L8c616934: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c616936; /* mov.l @(0xc,r12),r2 */
L8c616936: r[4]|=r[3];goto L8c616938; /* or r3,r4 */
L8c616938: r[4]|=r[2];goto L8c61693a; /* or r2,r4 */
L8c61693a: r[4]&=r[5];goto L8c61693c; /* and r5,r4 */
L8c61693c: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c61693e; /* tst r4,r4 */
L8c61693e: if(!(*sr&1u))goto L8c616944;goto L8c616940; /* bf 0x8c616944 */
L8c616940: goto L8c616a6a; /* bra 0x8c616a6a */
L8c616944: r[7]=r[15];goto L8c616946; /* mov r15,r7 */
L8c616946: r[7]+=64u;goto L8c616948; /* add #0x40,r7 */
L8c616948: wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c61694a; /* mov.l r10,@r7 */
L8c61694a: wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c61694c; /* mov.l r11,@(0x4,r7) */
L8c61694c: wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c61694e; /* mov.l r12,@(0x8,r7) */
L8c61694e: r[6]=0u;goto L8c6169fc; /* bra 0x8c6169fc */
L8c616952: r[2]=r[6];goto L8c616954; /* mov r6,r2 */
L8c616954: r[1]=r[6];goto L8c616956; /* mov r6,r1 */
L8c616956: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c616958; /* shll r2 */
L8c616958: r[3]=r[6];goto L8c61695a; /* mov r6,r3 */
L8c61695a: r[3]<<=2;goto L8c61695c; /* shll2 r3 */
L8c61695c: r[2]+=r[1];goto L8c61695e; /* add r1,r2 */
L8c61695e: r[2]<<=2;goto L8c616960; /* shll2 r2 */
L8c616960: r[3]+=r[7];goto L8c616962; /* add r7,r3 */
L8c616962: r[2]+=r[4];goto L8c616964; /* add r4,r2 */
L8c616964: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c616966; /* mov.l r3,@(0x4,r15) */
L8c616966: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c616968; /* mov.l r2,@r15 */
L8c616968: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61696a; /* mov.l @r3,r1 */
L8c61696a: r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c61696c; /* mov.l @(0xc,r1),r2 */
L8c61696c: *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c61696e; /* tst r5,r2 */
L8c61696e: take=!(*sr&1u);r[6]+=1u;if(take)goto L8c6169d4;goto L8c616972; /* bf/s 0x8c6169d4 */
L8c616972: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616974; /* mov.l @(0x4,r15),r1 */
L8c616974: r[0]=24u;goto L8c616976; /* mov #0x18,r0 */
L8c616976: fcopy(f,4u,12u);goto L8c616978; /* fmov fr12,fr4 */
L8c616978: r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61697a; /* mov.l @r1,r3 */
L8c61697a: r[4]=rd(m,bus,&io_status,0x8c6169c8u,4);if(io_status)return io_status;goto L8c61697c; /* mov.l 0x8c6169c8,r4 */
L8c61697c: wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c61697e; /* mov.l r3,@(0x10,r15) */
L8c61697e: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616980; /* fmov.s @(r0,r3),fr3 */
L8c616980: r[0]=12u;goto L8c616982; /* mov #0xc,r0 */
L8c616982: r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c616984; /* mov.l @(0x10,r15),r2 */
L8c616984: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616986; /* fdiv fr3,fr4 */
L8c616986: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c616988; /* fmov.s @(r0,r4),fr3 */
L8c616988: r[0]=16u;goto L8c61698a; /* mov #0x10,r0 */
L8c61698a: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61698c; /* mov.l @r15,r3 */
L8c61698c: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61698e; /* fmov.s @(r0,r2),fr2 */
L8c61698e: r[0]=16u;goto L8c616990; /* mov #0x10,r0 */
L8c616990: r[2]=rd(m,bus,&io_status,0x8c6169ccu,4);if(io_status)return io_status;goto L8c616992; /* mov.l 0x8c6169cc,r2 */
L8c616992: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616994; /* fsub fr3,fr2 */
L8c616994: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c616996; /* fmov.s @r2,fr3 */
L8c616996: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616998; /* fmul fr4,fr2 */
L8c616998: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61699a; /* fdiv fr3,fr2 */
L8c61699a: fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61699c; /* fmov.s fr2,@r3 */
L8c61699c: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61699e; /* mov.l @(0x4,r15),r1 */
L8c61699e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6169a0; /* fmov.s @(r0,r4),fr3 */
L8c6169a0: r[0]=20u;goto L8c6169a2; /* mov #0x14,r0 */
L8c6169a2: r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c6169a4; /* mov.l @r1,r1 */
L8c6169a4: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6169a6; /* mov.l @r15,r3 */
L8c6169a6: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c6169a8; /* fmov.s @(r0,r1),fr2 */
L8c6169a8: r[1]=rd(m,bus,&io_status,0x8c6169d0u,4);if(io_status)return io_status;goto L8c6169aa; /* mov.l 0x8c6169d0,r1 */
L8c6169aa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6169ac; /* fsub fr3,fr2 */
L8c6169ac: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6169ae; /* fmov.s @r1,fr3 */
L8c6169ae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6169b0; /* fmul fr4,fr2 */
L8c6169b0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6169b2; /* fdiv fr3,fr2 */
L8c6169b2: r[0]=4u;goto L8c6169b4; /* mov #0x4,r0 */
L8c6169b4: fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c6169b6; /* fmov fr2,@(r0,r3) */
L8c6169b6: r[0]=8u;goto L8c6169b8; /* mov #0x8,r0 */
L8c6169b8: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6169ba; /* mov.l @r15,r3 */
L8c6169ba: fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c6169bc; /* fmov fr4,@(r0,r3) */
L8c6169bc: goto L8c6169fc; /* bra 0x8c6169fc */
L8c6169d4: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6169d6; /* mov.l @(0x4,r15),r3 */
L8c6169d6: r[0]=16u;goto L8c6169d8; /* mov #0x10,r0 */
L8c6169d8: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6169da; /* mov.l @r15,r2 */
L8c6169da: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6169dc; /* mov.l @r3,r1 */
L8c6169dc: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6169de; /* fmov.s @(r0,r1),fr3 */
L8c6169de: r[0]=20u;goto L8c6169e0; /* mov #0x14,r0 */
L8c6169e0: fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6169e2; /* fmov.s fr3,@r2 */
L8c6169e2: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6169e4; /* mov.l @(0x4,r15),r2 */
L8c6169e4: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6169e6; /* mov.l @r15,r3 */
L8c6169e6: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6169e8; /* mov.l @r2,r1 */
L8c6169e8: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6169ea; /* fmov.s @(r0,r1),fr3 */
L8c6169ea: r[0]=4u;goto L8c6169ec; /* mov #0x4,r0 */
L8c6169ec: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6169ee; /* fmov fr3,@(r0,r3) */
L8c6169ee: r[0]=24u;goto L8c6169f0; /* mov #0x18,r0 */
L8c6169f0: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6169f2; /* mov.l @(0x4,r15),r2 */
L8c6169f2: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6169f4; /* mov.l @r15,r3 */
L8c6169f4: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6169f6; /* mov.l @r2,r1 */
L8c6169f6: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6169f8; /* fmov.s @(r0,r1),fr3 */
L8c6169f8: r[0]=8u;goto L8c6169fa; /* mov #0x8,r0 */
L8c6169fa: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6169fc; /* fmov fr3,@(r0,r3) */
L8c6169fc: r[3]=3u;goto L8c6169fe; /* mov #0x3,r3 */
L8c6169fe: r[4]=r[15];goto L8c616a00; /* mov r15,r4 */
L8c616a00: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c616a02; /* cmp/ge r3,r6 */
L8c616a02: take=!(*sr&1u);r[4]+=28u;if(take)goto L8c616952;goto L8c616a06; /* bf/s 0x8c616952 */
L8c616a06: r[6]=r[4];goto L8c616a08; /* mov r4,r6 */
L8c616a08: r[0]=8u;goto L8c616a0a; /* mov #0x8,r0 */
L8c616a0a: r[3]=r[4];goto L8c616a0c; /* mov r4,r3 */
L8c616a0c: fload(m,bus,&io_status,f,6u,r[0]+r[6]);if(io_status)return io_status;goto L8c616a0e; /* fmov.s @(r0,r6),fr6 */
L8c616a0e: r[3]+=12u;goto L8c616a10; /* add #0xc,r3 */
L8c616a10: r[7]=r[4];goto L8c616a12; /* mov r4,r7 */
L8c616a12: r[0]=4u;goto L8c616a14; /* mov #0x4,r0 */
L8c616a14: fcopy(f,0u,6u);goto L8c616a16; /* fmov fr6,fr0 */
L8c616a16: r[7]+=24u;goto L8c616a18; /* add #0x18,r7 */
L8c616a18: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c616a1a; /* mov.l r3,@r15 */
L8c616a1a: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c616a1c; /* fmov.s @(r0,r6),fr4 */
L8c616a1c: fload(m,bus,&io_status,f,5u,r[0]+r[3]);if(io_status)return io_status;goto L8c616a1e; /* fmov.s @(r0,r3),fr5 */
L8c616a1e: r[0]=8u;goto L8c616a20; /* mov #0x8,r0 */
L8c616a20: fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c616a22; /* fmov.s @(r0,r3),fr9 */
L8c616a22: r[0]=4u;goto L8c616a24; /* mov #0x4,r0 */
L8c616a24: fcopy(f,3u,4u);goto L8c616a26; /* fmov fr4,fr3 */
L8c616a26: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c616a28; /* fmul fr5,fr0 */
L8c616a28: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616a2a; /* fmul fr9,fr3 */
L8c616a2a: fload(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;goto L8c616a2c; /* fmov.s @r6,fr8 */
L8c616a2c: fload(m,bus,&io_status,f,7u,r[3]);if(io_status)return io_status;goto L8c616a2e; /* fmov.s @r3,fr7 */
L8c616a2e: fcopy(f,2u,8u);goto L8c616a30; /* fmov fr8,fr2 */
L8c616a30: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[9],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616a32; /* fmul fr9,fr2 */
L8c616a32: fcopy(f,1u,4u);goto L8c616a34; /* fmov fr4,fr1 */
L8c616a34: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c616a36; /* fsub fr3,fr0 */
L8c616a36: fcopy(f,3u,6u);goto L8c616a38; /* fmov fr6,fr3 */
L8c616a38: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616a3a; /* fmul fr7,fr3 */
L8c616a3a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616a3c; /* fsub fr3,fr2 */
L8c616a3c: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c616a3e; /* fmov.s @(r0,r7),fr3 */
L8c616a3e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616a40; /* fmul fr2,fr3 */
L8c616a40: fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c616a42; /* fmov.s @r7,fr2 */
L8c616a42: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616a44; /* fmac fr0,fr2,fr3 */
L8c616a44: fcopy(f,2u,3u);goto L8c616a46; /* fmov fr3,fr2 */
L8c616a46: fcopy(f,3u,8u);goto L8c616a48; /* fmov fr8,fr3 */
L8c616a48: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616a4a; /* fmul fr5,fr3 */
L8c616a4a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c616a4c; /* fmul fr7,fr1 */
L8c616a4c: r[0]=8u;goto L8c616a4e; /* mov #0x8,r0 */
L8c616a4e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c616a50; /* fsub fr3,fr1 */
L8c616a50: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c616a52; /* fmov.s @(r0,r7),fr3 */
L8c616a52: fcopy(f,0u,1u);goto L8c616a54; /* fmov fr1,fr0 */
L8c616a54: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616a56; /* fmac fr0,fr3,fr2 */
L8c616a56: fcopy(f,3u,15u);goto L8c616a58; /* fmov fr15,fr3 */
L8c616a58: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c616a5a; /* fneg fr3 */
L8c616a5a: fcopy(f,4u,2u);goto L8c616a5c; /* fmov fr2,fr4 */
L8c616a5c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616a5e; /* fmul fr3,fr4 */
L8c616a5e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c616a60; /* fldi0 fr2 */
L8c616a60: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c616a62; /* fcmp/gt fr2,fr4 */
L8c616a62: if((*sr&1u))goto L8c616a88;goto L8c616a64; /* bt 0x8c616a88 */
L8c616a64: wr(m,bus,&io_status,r[15]+0x14u,r[5],4);if(io_status)return io_status;goto L8c616a66; /* mov.l r5,@(0x14,r15) */
L8c616a66: fcopy(f,4u,12u);goto L8c616a88; /* bra 0x8c616a88 */
L8c616a6a: r[0]=20u;goto L8c616a6c; /* mov #0x14,r0 */
L8c616a6c: fcopy(f,3u,13u);goto L8c616a6e; /* fmov fr13,fr3 */
L8c616a6e: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c616a70; /* fmov.s @(r0,r12),fr2 */
L8c616a70: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c616a72; /* fsub fr6,fr3 */
L8c616a72: r[0]=16u;goto L8c616a74; /* mov #0x10,r0 */
L8c616a74: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616a76; /* fsub fr14,fr2 */
L8c616a76: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c616a78; /* fmov.s @(r0,r12),fr1 */
L8c616a78: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c616a7a; /* fsub fr13,fr1 */
L8c616a7a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616a7c; /* fmul fr2,fr3 */
L8c616a7c: fcopy(f,2u,14u);goto L8c616a7e; /* fmov fr14,fr2 */
L8c616a7e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616a80; /* fsub fr5,fr2 */
L8c616a80: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616a82; /* fmul fr1,fr2 */
L8c616a82: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c616a84; /* fsub fr2,fr3 */
L8c616a84: fcopy(f,4u,3u);goto L8c616a86; /* fmov fr3,fr4 */
L8c616a86: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616a88; /* fmul fr15,fr4 */
L8c616a88: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c616a8a; /* cmp/hs r8,r9 */
L8c616a8a: if((*sr&1u))goto L8c616ac2;goto L8c616a8c; /* bt 0x8c616ac2 */
L8c616a8c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c616a8e; /* fldi0 fr3 */
L8c616a8e: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c616a90; /* fcmp/gt fr4,fr3 */
L8c616a90: if((*sr&1u))goto L8c616af4;goto L8c616a92; /* bt 0x8c616af4 */
L8c616a92: r[5]=rd(m,bus,&io_status,0x8c616bfcu,4);if(io_status)return io_status;goto L8c616a94; /* mov.l 0x8c616bfc,r5 */
L8c616a94: r[4]=r[11];goto L8c616a96; /* mov r11,r4 */
L8c616a96: r[8]+=16u;goto L8c616a98; /* add #0x10,r8 */
L8c616a98: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616a9a; /* fschg */
L8c616a9a: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616a9c; /* fmov.s @r4+,fr0 */
L8c616a9c: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616a9e; /* fmov.s @r4+,fr2 */
L8c616a9e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c616aa0; /* fldi1 fr3 */
L8c616aa0: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c616aa2; /* ftrv xmtrx,fv0 */
L8c616aa2: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616aa4; /* fmov.s @r4+,fr4 */
L8c616aa4: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616aa6; /* fmov.s @r4,fr6 */
L8c616aa6: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616aa8; /* fschg */
L8c616aa8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616aaa; /* fmov.s fr6,@-r8 */
L8c616aaa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616aac; /* fmov.s fr5,@-r8 */
L8c616aac: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616aae; /* fmov.s fr4,@-r8 */
L8c616aae: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616ab0; /* mov.l r5,@-r8 */
L8c616ab0: r[8]+=28u;goto L8c616ab2; /* add #0x1c,r8 */
L8c616ab2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c616ab4; /* fldi1 fr2 */
L8c616ab4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616ab6; /* fmov.s fr2,@-r8 */
L8c616ab6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616ab8; /* fmov.s fr1,@-r8 */
L8c616ab8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616aba; /* fmov.s fr0,@-r8 */
L8c616aba: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616abc; /* pref @r8 */
L8c616abc: r[8]+=16u;goto L8c616abe; /* add #0x10,r8 */
L8c616abe: r[9]=r[8];goto L8c616b20; /* bra 0x8c616b20 */
L8c616ac2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c616ac4; /* fldi0 fr3 */
L8c616ac4: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c616ac6; /* fcmp/gt fr4,fr3 */
L8c616ac6: if(!(*sr&1u))goto L8c616b20;goto L8c616ac8; /* bf 0x8c616b20 */
L8c616ac8: r[5]=rd(m,bus,&io_status,0x8c616c00u,4);if(io_status)return io_status;goto L8c616aca; /* mov.l 0x8c616c00,r5 */
L8c616aca: r[4]=r[10];goto L8c616acc; /* mov r10,r4 */
L8c616acc: r[8]+=16u;goto L8c616ace; /* add #0x10,r8 */
L8c616ace: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616ad0; /* fschg */
L8c616ad0: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616ad2; /* fmov.s @r4+,fr0 */
L8c616ad2: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616ad4; /* fmov.s @r4+,fr2 */
L8c616ad4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c616ad6; /* fldi1 fr3 */
L8c616ad6: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c616ad8; /* ftrv xmtrx,fv0 */
L8c616ad8: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616ada; /* fmov.s @r4+,fr4 */
L8c616ada: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616adc; /* fmov.s @r4,fr6 */
L8c616adc: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616ade; /* fschg */
L8c616ade: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616ae0; /* fmov.s fr6,@-r8 */
L8c616ae0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616ae2; /* fmov.s fr5,@-r8 */
L8c616ae2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616ae4; /* fmov.s fr4,@-r8 */
L8c616ae4: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616ae6; /* mov.l r5,@-r8 */
L8c616ae6: r[8]+=28u;goto L8c616ae8; /* add #0x1c,r8 */
L8c616ae8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c616aea; /* fldi1 fr2 */
L8c616aea: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616aec; /* fmov.s fr2,@-r8 */
L8c616aec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616aee; /* fmov.s fr1,@-r8 */
L8c616aee: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616af0; /* fmov.s fr0,@-r8 */
L8c616af0: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616af2; /* pref @r8 */
L8c616af2: r[8]+=16u;goto L8c616af4; /* add #0x10,r8 */
L8c616af4: r[5]=rd(m,bus,&io_status,0x8c616c00u,4);if(io_status)return io_status;goto L8c616af6; /* mov.l 0x8c616c00,r5 */
L8c616af6: r[4]=r[11];goto L8c616af8; /* mov r11,r4 */
L8c616af8: r[8]+=16u;goto L8c616afa; /* add #0x10,r8 */
L8c616afa: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616afc; /* fschg */
L8c616afc: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616afe; /* fmov.s @r4+,fr0 */
L8c616afe: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616b00; /* fmov.s @r4+,fr2 */
L8c616b00: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c616b02; /* fldi1 fr3 */
L8c616b02: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c616b04; /* ftrv xmtrx,fv0 */
L8c616b04: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616b06; /* fmov.s @r4+,fr4 */
L8c616b06: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616b08; /* fmov.s @r4,fr6 */
L8c616b08: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616b0a; /* fschg */
L8c616b0a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616b0c; /* fmov.s fr6,@-r8 */
L8c616b0c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616b0e; /* fmov.s fr5,@-r8 */
L8c616b0e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616b10; /* fmov.s fr4,@-r8 */
L8c616b10: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616b12; /* mov.l r5,@-r8 */
L8c616b12: r[8]+=28u;goto L8c616b14; /* add #0x1c,r8 */
L8c616b14: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c616b16; /* fldi1 fr2 */
L8c616b16: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616b18; /* fmov.s fr2,@-r8 */
L8c616b18: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616b1a; /* fmov.s fr1,@-r8 */
L8c616b1a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616b1c; /* fmov.s fr0,@-r8 */
L8c616b1c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616b1e; /* pref @r8 */
L8c616b1e: r[8]+=16u;goto L8c616b20; /* add #0x10,r8 */
L8c616b20: r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c616b22; /* mov.l @(0x14,r15),r2 */
L8c616b22: *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c616b24; /* tst r2,r2 */
L8c616b24: if((*sr&1u))goto L8c616b38;goto L8c616b26; /* bt 0x8c616b38 */
L8c616b26: r[4]=rd(m,bus,&io_status,0x8c616c04u,4);if(io_status)return io_status;goto L8c616b28; /* mov.l 0x8c616c04,r4 */
L8c616b28: r[3]=0u;goto L8c616b2a; /* mov #0x0,r3 */
L8c616b2a: r[2]=rd(m,bus,&io_status,0x8c616c08u,4);if(io_status)return io_status;goto L8c616b2c; /* mov.l 0x8c616c08,r2 */
L8c616b2c: r[5]=r[11];goto L8c616b2e; /* mov r11,r5 */
L8c616b2e: wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c616b30; /* mov.l r3,@r4 */
L8c616b30: r[6]=r[12];goto L8c616b32; /* mov r12,r6 */
L8c616b32: target=r[2];*pr=0x8c616b36u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c616b36; /* jsr @r2 */
L8c616b36: r[9]=r[8];goto L8c616b38; /* mov r8,r9 */
L8c616b38: fcopy(f,6u,13u);goto L8c616b3a; /* fmov fr13,fr6 */
L8c616b3a: r[0]=16u;goto L8c616b3c; /* mov #0x10,r0 */
L8c616b3c: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c616b3e; /* mov.l @(0x8,r15),r3 */
L8c616b3e: r[10]=r[11];goto L8c616b40; /* mov r11,r10 */
L8c616b40: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c616b42; /* fmov.s @(r0,r12),fr13 */
L8c616b42: r[0]=20u;goto L8c616b44; /* mov #0x14,r0 */
L8c616b44: fcopy(f,5u,14u);goto L8c616b46; /* fmov fr14,fr5 */
L8c616b46: r[3]+=4294967295u;goto L8c616b48; /* add #-0x1,r3 */
L8c616b48: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c616b4a; /* fmov.s @(r0,r12),fr14 */
L8c616b4a: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c616b4c; /* tst r3,r3 */
L8c616b4c: r[11]=r[12];goto L8c616b4e; /* mov r12,r11 */
L8c616b4e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c616b50; /* fneg fr15 */
L8c616b50: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c616b58;goto L8c616b54; /* bt/s 0x8c616b58 */
L8c616b54: goto L8c6168f8; /* bra 0x8c6168f8 */
L8c616b58: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c616b5a; /* cmp/hs r8,r9 */
L8c616b5a: if((*sr&1u))goto L8c616b88;goto L8c616b5c; /* bt 0x8c616b88 */
L8c616b5c: r[5]=rd(m,bus,&io_status,0x8c616bfcu,4);if(io_status)return io_status;goto L8c616b5e; /* mov.l 0x8c616bfc,r5 */
L8c616b5e: r[4]=r[12];goto L8c616b60; /* mov r12,r4 */
L8c616b60: r[8]+=16u;goto L8c616b62; /* add #0x10,r8 */
L8c616b62: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616b64; /* fschg */
L8c616b64: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616b66; /* fmov.s @r4+,fr0 */
L8c616b66: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616b68; /* fmov.s @r4+,fr2 */
L8c616b68: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c616b6a; /* fldi1 fr3 */
L8c616b6a: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c616b6c; /* ftrv xmtrx,fv0 */
L8c616b6c: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616b6e; /* fmov.s @r4+,fr4 */
L8c616b6e: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616b70; /* fmov.s @r4,fr6 */
L8c616b70: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616b72; /* fschg */
L8c616b72: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616b74; /* fmov.s fr6,@-r8 */
L8c616b74: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616b76; /* fmov.s fr5,@-r8 */
L8c616b76: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616b78; /* fmov.s fr4,@-r8 */
L8c616b78: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616b7a; /* mov.l r5,@-r8 */
L8c616b7a: r[8]+=28u;goto L8c616b7c; /* add #0x1c,r8 */
L8c616b7c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c616b7e; /* fldi1 fr2 */
L8c616b7e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616b80; /* fmov.s fr2,@-r8 */
L8c616b80: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616b82; /* fmov.s fr1,@-r8 */
L8c616b82: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616b84; /* fmov.s fr0,@-r8 */
L8c616b84: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616b86; /* pref @r8 */
L8c616b86: r[8]+=16u;goto L8c616b88; /* add #0x10,r8 */
L8c616b88: r[3]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c616b8a; /* mov.l @(0x18,r15),r3 */
L8c616b8a: r[3]+=4294967295u;goto L8c616b8c; /* add #-0x1,r3 */
L8c616b8c: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c616b8e; /* tst r3,r3 */
L8c616b8e: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;if(take)goto L8c616b96;goto L8c616b92; /* bt/s 0x8c616b96 */
L8c616b92: goto L8c6168a8; /* bra 0x8c6168a8 */
L8c616b96: r[4]=rd(m,bus,&io_status,0x8c616c0cu,4);if(io_status)return io_status;goto L8c616b98; /* mov.l 0x8c616c0c,r4 */
L8c616b98: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616b9a; /* mov.l @(0xc,r15),r3 */
L8c616b9a: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616b9c; /* mov.l @r4,r2 */
L8c616b9c: r[2]+=r[3];goto L8c616b9e; /* add r3,r2 */
L8c616b9e: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c616ba0; /* mov.l r2,@r4 */
L8c616ba0: r[4]=rd(m,bus,&io_status,0x8c616c10u,4);if(io_status)return io_status;goto L8c616ba2; /* mov.l 0x8c616c10,r4 */
L8c616ba2: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616ba4; /* mov.l @(0xc,r15),r3 */
L8c616ba4: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616ba6; /* mov.l @r4,r2 */
L8c616ba6: r[2]+=r[3];goto L8c616ba8; /* add r3,r2 */
L8c616ba8: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c616baa; /* mov.l r2,@r4 */
L8c616baa: r[15]+=76u;goto L8c616bac; /* add #0x4c,r15 */
L8c616bac: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616bae; /* lds.l @r15+,PR */
L8c616bae: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616bb0; /* fmov.s @r15+,fr12 */
L8c616bb0: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616bb2; /* fmov.s @r15+,fr13 */
L8c616bb2: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616bb4; /* fmov.s @r15+,fr14 */
L8c616bb4: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616bb6; /* fmov.s @r15+,fr15 */
L8c616bb6: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616bb8; /* mov.l @r15+,r9 */
L8c616bb8: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616bba; /* mov.l @r15+,r10 */
L8c616bba: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616bbc; /* mov.l @r15+,r11 */
L8c616bbc: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616bbe; /* mov.l @r15+,r12 */
L8c616bbe: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616bc0; /* mov.l @r15+,r13 */
L8c616bc0: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
