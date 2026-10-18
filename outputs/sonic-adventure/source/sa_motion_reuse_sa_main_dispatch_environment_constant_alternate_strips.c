#include "sa_draw_motion_stop.h"
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
L8c614796: sa_motion_publish_stop(ctx,0x8c614796u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c614798; /* mov.l r14,@-r15 */
L8c614798: sa_motion_publish_stop(ctx,0x8c614798u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61479a; /* mov.l r13,@-r15 */
L8c61479a: sa_motion_publish_stop(ctx,0x8c61479au); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61479c; /* mov.l r12,@-r15 */
L8c61479c: sa_motion_publish_stop(ctx,0x8c61479cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61479e; /* mov.l r11,@-r15 */
L8c61479e: sa_motion_publish_stop(ctx,0x8c61479eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6147a0; /* mov.l r10,@-r15 */
L8c6147a0: sa_motion_publish_stop(ctx,0x8c6147a0u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6147a2; /* mov.l r9,@-r15 */
L8c6147a2: sa_motion_publish_stop(ctx,0x8c6147a2u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c6147a4; /* fmov.s fr15,@-r15 */
L8c6147a4: sa_motion_publish_stop(ctx,0x8c6147a4u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c6147a6; /* fmov.s fr14,@-r15 */
L8c6147a6: sa_motion_publish_stop(ctx,0x8c6147a6u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c6147a8; /* fmov.s fr13,@-r15 */
L8c6147a8: sa_motion_publish_stop(ctx,0x8c6147a8u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6147aa; /* sts.l PR,@-r15 */
L8c6147aa: sa_motion_publish_stop(ctx,0x8c6147aau); r[15]+=4294967280u;goto L8c6147ac; /* add #-0x10,r15 */
L8c6147ac: sa_motion_publish_stop(ctx,0x8c6147acu); r[3]=rd(m,bus,&io_status,0x8c6147e0u,4);if(io_status)return io_status;goto L8c6147ae; /* mov.l 0x8c6147e0,r3 */
L8c6147ae: sa_motion_publish_stop(ctx,0x8c6147aeu); r[9]=rd(m,bus,&io_status,0x8c6147dcu,4);if(io_status)return io_status;goto L8c6147b0; /* mov.l 0x8c6147dc,r9 */
L8c6147b0: sa_motion_publish_stop(ctx,0x8c6147b2u); target=r[3];*pr=0x8c6147b4u;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6147b4; /* jsr @r3 */
L8c6147b4: sa_motion_publish_stop(ctx,0x8c6147b4u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6147b6; /* mov.l @(0x4,r15),r3 */
L8c6147b6: sa_motion_publish_stop(ctx,0x8c6147b6u); r[2]=0u;goto L8c6147b8; /* mov #0x0,r2 */
L8c6147b8: sa_motion_publish_stop(ctx,0x8c6147b8u); r[14]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6147ba; /* mov.l @(0x4,r15),r14 */
L8c6147ba: sa_motion_publish_stop(ctx,0x8c6147bau); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6147bc; /* mov.w @(0x2,r3),r0 */
L8c6147bc: sa_motion_publish_stop(ctx,0x8c6147bcu); r[3]=rd(m,bus,&io_status,0x8c6147e4u,4);if(io_status)return io_status;goto L8c6147be; /* mov.l 0x8c6147e4,r3 */
L8c6147be: sa_motion_publish_stop(ctx,0x8c6147beu); r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c6147c0; /* mov.l @(0x4,r14),r14 */
L8c6147c0: sa_motion_publish_stop(ctx,0x8c6147c0u); r[0]=r[0]&65535u;goto L8c6147c2; /* extu.w r0,r0 */
L8c6147c2: sa_motion_publish_stop(ctx,0x8c6147c2u); wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c6147c4; /* mov.l r0,@(0xc,r15) */
L8c6147c4: sa_motion_publish_stop(ctx,0x8c6147c4u); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6147c6; /* mov.l @r3,r13 */
L8c6147c6: sa_motion_publish_stop(ctx,0x8c6147c6u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6147c8; /* mov.l r2,@(0x8,r15) */
L8c6147c8: sa_motion_publish_stop(ctx,0x8c6147c8u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6147ca; /* mov.l r2,@(0x4,r15) */
L8c6147ca: sa_motion_publish_stop(ctx,0x8c6147ccu); goto L8c6147e8; /* bra 0x8c6147e8 */
L8c6147e8: sa_motion_publish_stop(ctx,0x8c6147e8u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6147ea; /* mov.w @r14+,r3 */
L8c6147ea: sa_motion_publish_stop(ctx,0x8c6147eau); r[4]=rd(m,bus,&io_status,0x8c6148bcu,4);if(io_status)return io_status;goto L8c6147ec; /* mov.l 0x8c6148bc,r4 */
L8c6147ec: sa_motion_publish_stop(ctx,0x8c6147ecu); r[3]=r[3]&65535u;goto L8c6147ee; /* extu.w r3,r3 */
L8c6147ee: sa_motion_publish_stop(ctx,0x8c6147eeu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6147f0; /* fldi1 fr4 */
L8c6147f0: sa_motion_publish_stop(ctx,0x8c6147f0u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6147f2; /* tst r4,r3 */
L8c6147f2: sa_motion_publish_stop(ctx,0x8c6147f2u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6147f4; /* mov.l r3,@r15 */
L8c6147f4: sa_motion_publish_stop(ctx,0x8c6147f6u); take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c614806;goto L8c6147f8; /* bt/s 0x8c614806 */
L8c6147f8: sa_motion_publish_stop(ctx,0x8c6147f8u); r[4]=sx(m,bus,&io_status,0x8c6148b8u,2);if(io_status)return io_status;goto L8c6147fa; /* mov.w 0x8c6148b8,r4 */
L8c6147fa: sa_motion_publish_stop(ctx,0x8c6147fau); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6147fc; /* mov.l @r15,r3 */
L8c6147fc: sa_motion_publish_stop(ctx,0x8c6147fcu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6147fe; /* fldi1 fr4 */
L8c6147fe: sa_motion_publish_stop(ctx,0x8c6147feu); r[3]&=r[4];goto L8c614800; /* and r4,r3 */
L8c614800: sa_motion_publish_stop(ctx,0x8c614800u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c614802; /* fneg fr4 */
L8c614802: sa_motion_publish_stop(ctx,0x8c614802u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c614804; /* mov.l r3,@r15 */
L8c614804: sa_motion_publish_stop(ctx,0x8c614804u); fcopy(f,15u,4u);goto L8c614806; /* fmov fr4,fr15 */
L8c614806: sa_motion_publish_stop(ctx,0x8c614806u); r[0]=16u;goto L8c614808; /* mov #0x10,r0 */
L8c614808: sa_motion_publish_stop(ctx,0x8c614808u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61480a; /* mov.l @r15,r3 */
L8c61480a: sa_motion_publish_stop(ctx,0x8c61480au); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61480c; /* mov.l @(0x4,r15),r2 */
L8c61480c: sa_motion_publish_stop(ctx,0x8c61480cu); r[10]=r[8];goto L8c61480e; /* mov r8,r10 */
L8c61480e: sa_motion_publish_stop(ctx,0x8c61480eu); r[2]+=r[3];goto L8c614810; /* add r3,r2 */
L8c614810: sa_motion_publish_stop(ctx,0x8c614810u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c614812; /* mov.l r2,@(0x4,r15) */
L8c614812: sa_motion_publish_stop(ctx,0x8c614812u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614814; /* mov.l @r15,r1 */
L8c614814: sa_motion_publish_stop(ctx,0x8c614814u); r[1]+=4294967294u;goto L8c614816; /* add #-0x2,r1 */
L8c614816: sa_motion_publish_stop(ctx,0x8c614816u); wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c614818; /* mov.l r1,@r15 */
L8c614818: sa_motion_publish_stop(ctx,0x8c614818u); r[4]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c61481a; /* mov.w @r14+,r4 */
L8c61481a: sa_motion_publish_stop(ctx,0x8c61481au); r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c61481c; /* mov.w @r14+,r11 */
L8c61481c: sa_motion_publish_stop(ctx,0x8c61481cu); r[4]=r[4]&65535u;goto L8c61481e; /* extu.w r4,r4 */
L8c61481e: sa_motion_publish_stop(ctx,0x8c61481eu); r[4]<<=2;goto L8c614820; /* shll2 r4 */
L8c614820: sa_motion_publish_stop(ctx,0x8c614820u); r[11]=r[11]&65535u;goto L8c614822; /* extu.w r11,r11 */
L8c614822: sa_motion_publish_stop(ctx,0x8c614822u); r[11]<<=2;goto L8c614824; /* shll2 r11 */
L8c614824: sa_motion_publish_stop(ctx,0x8c614824u); r[4]<<=2;goto L8c614826; /* shll2 r4 */
L8c614826: sa_motion_publish_stop(ctx,0x8c614826u); r[11]<<=2;goto L8c614828; /* shll2 r11 */
L8c614828: sa_motion_publish_stop(ctx,0x8c614828u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61482a; /* shll r4 */
L8c61482a: sa_motion_publish_stop(ctx,0x8c61482au); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61482c; /* shll r11 */
L8c61482c: sa_motion_publish_stop(ctx,0x8c61482cu); r[11]+=r[13];goto L8c61482e; /* add r13,r11 */
L8c61482e: sa_motion_publish_stop(ctx,0x8c61482eu); r[4]+=r[13];goto L8c614830; /* add r13,r4 */
L8c614830: sa_motion_publish_stop(ctx,0x8c614830u); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c614832; /* fmov.s @(r0,r11),fr14 */
L8c614832: sa_motion_publish_stop(ctx,0x8c614832u); fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c614834; /* fmov.s @(r0,r4),fr4 */
L8c614834: sa_motion_publish_stop(ctx,0x8c614834u); r[0]=20u;goto L8c614836; /* mov #0x14,r0 */
L8c614836: sa_motion_publish_stop(ctx,0x8c614836u); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c614838; /* fmov.s @(r0,r11),fr13 */
L8c614838: sa_motion_publish_stop(ctx,0x8c614838u); fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c61483a; /* fmov.s @(r0,r4),fr5 */
L8c61483a: sa_motion_publish_stop(ctx,0x8c61483au); r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c61483c; /* mov.w @r14+,r12 */
L8c61483c: sa_motion_publish_stop(ctx,0x8c61483cu); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c61483e; /* mov.w @r14,r3 */
L8c61483e: sa_motion_publish_stop(ctx,0x8c61483eu); r[12]=r[12]&65535u;goto L8c614840; /* extu.w r12,r12 */
L8c614840: sa_motion_publish_stop(ctx,0x8c614840u); r[12]<<=2;goto L8c614842; /* shll2 r12 */
L8c614842: sa_motion_publish_stop(ctx,0x8c614842u); r[3]=r[3]&65535u;goto L8c614844; /* extu.w r3,r3 */
L8c614844: sa_motion_publish_stop(ctx,0x8c614844u); r[3]<<=2;goto L8c614846; /* shll2 r3 */
L8c614846: sa_motion_publish_stop(ctx,0x8c614846u); r[12]<<=2;goto L8c614848; /* shll2 r12 */
L8c614848: sa_motion_publish_stop(ctx,0x8c614848u); r[3]<<=2;goto L8c61484a; /* shll2 r3 */
L8c61484a: sa_motion_publish_stop(ctx,0x8c61484au); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61484c; /* shll r12 */
L8c61484c: sa_motion_publish_stop(ctx,0x8c61484cu); *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c61484e; /* shll r3 */
L8c61484e: sa_motion_publish_stop(ctx,0x8c61484eu); r[3]+=r[13];goto L8c614850; /* add r13,r3 */
L8c614850: sa_motion_publish_stop(ctx,0x8c614850u); r[12]+=r[13];goto L8c614852; /* add r13,r12 */
L8c614852: sa_motion_publish_stop(ctx,0x8c614852u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c614854; /* pref @r3 */
L8c614854: sa_motion_publish_stop(ctx,0x8c614854u); r[0]=20u;goto L8c614856; /* mov #0x14,r0 */
L8c614856: sa_motion_publish_stop(ctx,0x8c614856u); fcopy(f,3u,14u);goto L8c614858; /* fmov fr14,fr3 */
L8c614858: sa_motion_publish_stop(ctx,0x8c614858u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61485a; /* fmov.s @(r0,r12),fr2 */
L8c61485a: sa_motion_publish_stop(ctx,0x8c61485au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61485c; /* fsub fr4,fr3 */
L8c61485c: sa_motion_publish_stop(ctx,0x8c61485cu); r[0]=16u;goto L8c61485e; /* mov #0x10,r0 */
L8c61485e: sa_motion_publish_stop(ctx,0x8c61485eu); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c614860; /* cmp/hs r8,r10 */
L8c614860: sa_motion_publish_stop(ctx,0x8c614860u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614862; /* fsub fr13,fr2 */
L8c614862: sa_motion_publish_stop(ctx,0x8c614862u); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c614864; /* fmov.s @(r0,r12),fr1 */
L8c614864: sa_motion_publish_stop(ctx,0x8c614864u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c614866; /* fsub fr14,fr1 */
L8c614866: sa_motion_publish_stop(ctx,0x8c614866u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614868; /* fmul fr2,fr3 */
L8c614868: sa_motion_publish_stop(ctx,0x8c614868u); fcopy(f,2u,13u);goto L8c61486a; /* fmov fr13,fr2 */
L8c61486a: sa_motion_publish_stop(ctx,0x8c61486au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61486c; /* fsub fr5,fr2 */
L8c61486c: sa_motion_publish_stop(ctx,0x8c61486cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61486e; /* fmul fr1,fr2 */
L8c61486e: sa_motion_publish_stop(ctx,0x8c61486eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614870; /* fsub fr2,fr3 */
L8c614870: sa_motion_publish_stop(ctx,0x8c614870u); fcopy(f,4u,3u);goto L8c614872; /* fmov fr3,fr4 */
L8c614872: sa_motion_publish_stop(ctx,0x8c614874u); take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c6148c0;goto L8c614876; /* bt/s 0x8c6148c0 */
L8c614876: sa_motion_publish_stop(ctx,0x8c614876u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614878; /* fldi0 fr3 */
L8c614878: sa_motion_publish_stop(ctx,0x8c614878u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c61487a; /* fcmp/gt fr4,fr3 */
L8c61487a: sa_motion_publish_stop(ctx,0x8c61487au); if((*sr&1u))goto L8c6148f0;goto L8c61487c; /* bt 0x8c6148f0 */
L8c61487c: sa_motion_publish_stop(ctx,0x8c61487cu); r[4]=r[11];goto L8c61487e; /* mov r11,r4 */
L8c61487e: sa_motion_publish_stop(ctx,0x8c61487eu); r[5]=r[9];goto L8c614880; /* mov r9,r5 */
L8c614880: sa_motion_publish_stop(ctx,0x8c614880u); r[8]+=16u;goto L8c614882; /* add #0x10,r8 */
L8c614882: sa_motion_publish_stop(ctx,0x8c614882u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614884; /* fschg */
L8c614884: sa_motion_publish_stop(ctx,0x8c614884u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614886; /* fmov.s @r4+,fr0 */
L8c614886: sa_motion_publish_stop(ctx,0x8c614886u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614888; /* fmov.s @r4+,fr2 */
L8c614888: sa_motion_publish_stop(ctx,0x8c614888u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c61488a; /* fldi1 fr3 */
L8c61488a: sa_motion_publish_stop(ctx,0x8c61488au); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c61488c; /* ftrv xmtrx,fv0 */
L8c61488c: sa_motion_publish_stop(ctx,0x8c61488cu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61488e; /* fmov.s @r4+,fr4 */
L8c61488e: sa_motion_publish_stop(ctx,0x8c61488eu); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614890; /* fmov.s @r4,fr6 */
L8c614890: sa_motion_publish_stop(ctx,0x8c614890u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614892; /* fschg */
L8c614892: sa_motion_publish_stop(ctx,0x8c614892u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614894; /* fmov.s fr6,@-r8 */
L8c614894: sa_motion_publish_stop(ctx,0x8c614894u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614896; /* fmov.s fr5,@-r8 */
L8c614896: sa_motion_publish_stop(ctx,0x8c614896u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614898; /* fmov.s fr4,@-r8 */
L8c614898: sa_motion_publish_stop(ctx,0x8c614898u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61489a; /* mov.l r5,@-r8 */
L8c61489a: sa_motion_publish_stop(ctx,0x8c61489au); r[8]+=28u;goto L8c61489c; /* add #0x1c,r8 */
L8c61489c: sa_motion_publish_stop(ctx,0x8c61489cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c61489e; /* fldi1 fr2 */
L8c61489e: sa_motion_publish_stop(ctx,0x8c61489eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6148a0; /* fmov.s fr2,@-r8 */
L8c6148a0: sa_motion_publish_stop(ctx,0x8c6148a0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6148a2; /* fmov.s fr1,@-r8 */
L8c6148a2: sa_motion_publish_stop(ctx,0x8c6148a2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c6148a4; /* fmov.s fr0,@-r8 */
L8c6148a4: sa_motion_publish_stop(ctx,0x8c6148a4u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6148a6; /* pref @r8 */
L8c6148a6: sa_motion_publish_stop(ctx,0x8c6148a6u); r[8]+=16u;goto L8c6148a8; /* add #0x10,r8 */
L8c6148a8: sa_motion_publish_stop(ctx,0x8c6148a8u); r[3]=r[8];goto L8c6148aa; /* mov r8,r3 */
L8c6148aa: sa_motion_publish_stop(ctx,0x8c6148aau); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6148ac; /* mov.l @(0x8,r15),r2 */
L8c6148ac: sa_motion_publish_stop(ctx,0x8c6148acu); r[3]-=r[10];goto L8c6148ae; /* sub r10,r3 */
L8c6148ae: sa_motion_publish_stop(ctx,0x8c6148aeu); r[3]+=32u;goto L8c6148b0; /* add #0x20,r3 */
L8c6148b0: sa_motion_publish_stop(ctx,0x8c6148b0u); r[2]+=r[3];goto L8c6148b2; /* add r3,r2 */
L8c6148b2: sa_motion_publish_stop(ctx,0x8c6148b2u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6148b4; /* mov.l r2,@(0x8,r15) */
L8c6148b4: sa_motion_publish_stop(ctx,0x8c6148b6u); r[10]=r[8];goto L8c61491c; /* bra 0x8c61491c */
L8c6148c0: sa_motion_publish_stop(ctx,0x8c6148c0u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6148c2; /* fldi0 fr3 */
L8c6148c2: sa_motion_publish_stop(ctx,0x8c6148c2u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6148c4; /* fcmp/gt fr4,fr3 */
L8c6148c4: sa_motion_publish_stop(ctx,0x8c6148c4u); if(!(*sr&1u))goto L8c61491c;goto L8c6148c6; /* bf 0x8c61491c */
L8c6148c6: sa_motion_publish_stop(ctx,0x8c6148c6u); r[5]=rd(m,bus,&io_status,0x8c6149ecu,4);if(io_status)return io_status;goto L8c6148c8; /* mov.l 0x8c6149ec,r5 */
L8c6148c8: sa_motion_publish_stop(ctx,0x8c6148c8u); r[8]+=16u;goto L8c6148ca; /* add #0x10,r8 */
L8c6148ca: sa_motion_publish_stop(ctx,0x8c6148cau); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6148cc; /* fschg */
L8c6148cc: sa_motion_publish_stop(ctx,0x8c6148ccu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6148ce; /* fmov.s @r4+,fr0 */
L8c6148ce: sa_motion_publish_stop(ctx,0x8c6148ceu); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6148d0; /* fmov.s @r4+,fr2 */
L8c6148d0: sa_motion_publish_stop(ctx,0x8c6148d0u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6148d2; /* fldi1 fr3 */
L8c6148d2: sa_motion_publish_stop(ctx,0x8c6148d2u); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6148d4; /* ftrv xmtrx,fv0 */
L8c6148d4: sa_motion_publish_stop(ctx,0x8c6148d4u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6148d6; /* fmov.s @r4+,fr4 */
L8c6148d6: sa_motion_publish_stop(ctx,0x8c6148d6u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6148d8; /* fmov.s @r4,fr6 */
L8c6148d8: sa_motion_publish_stop(ctx,0x8c6148d8u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6148da; /* fschg */
L8c6148da: sa_motion_publish_stop(ctx,0x8c6148dau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6148dc; /* fmov.s fr6,@-r8 */
L8c6148dc: sa_motion_publish_stop(ctx,0x8c6148dcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6148de; /* fmov.s fr5,@-r8 */
L8c6148de: sa_motion_publish_stop(ctx,0x8c6148deu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6148e0; /* fmov.s fr4,@-r8 */
L8c6148e0: sa_motion_publish_stop(ctx,0x8c6148e0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6148e2; /* mov.l r5,@-r8 */
L8c6148e2: sa_motion_publish_stop(ctx,0x8c6148e2u); r[8]+=28u;goto L8c6148e4; /* add #0x1c,r8 */
L8c6148e4: sa_motion_publish_stop(ctx,0x8c6148e4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c6148e6; /* fldi1 fr2 */
L8c6148e6: sa_motion_publish_stop(ctx,0x8c6148e6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6148e8; /* fmov.s fr2,@-r8 */
L8c6148e8: sa_motion_publish_stop(ctx,0x8c6148e8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6148ea; /* fmov.s fr1,@-r8 */
L8c6148ea: sa_motion_publish_stop(ctx,0x8c6148eau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c6148ec; /* fmov.s fr0,@-r8 */
L8c6148ec: sa_motion_publish_stop(ctx,0x8c6148ecu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6148ee; /* pref @r8 */
L8c6148ee: sa_motion_publish_stop(ctx,0x8c6148eeu); r[8]+=16u;goto L8c6148f0; /* add #0x10,r8 */
L8c6148f0: sa_motion_publish_stop(ctx,0x8c6148f0u); r[5]=rd(m,bus,&io_status,0x8c6149ecu,4);if(io_status)return io_status;goto L8c6148f2; /* mov.l 0x8c6149ec,r5 */
L8c6148f2: sa_motion_publish_stop(ctx,0x8c6148f2u); r[4]=r[11];goto L8c6148f4; /* mov r11,r4 */
L8c6148f4: sa_motion_publish_stop(ctx,0x8c6148f4u); r[8]+=16u;goto L8c6148f6; /* add #0x10,r8 */
L8c6148f6: sa_motion_publish_stop(ctx,0x8c6148f6u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6148f8; /* fschg */
L8c6148f8: sa_motion_publish_stop(ctx,0x8c6148f8u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6148fa; /* fmov.s @r4+,fr0 */
L8c6148fa: sa_motion_publish_stop(ctx,0x8c6148fau); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6148fc; /* fmov.s @r4+,fr2 */
L8c6148fc: sa_motion_publish_stop(ctx,0x8c6148fcu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6148fe; /* fldi1 fr3 */
L8c6148fe: sa_motion_publish_stop(ctx,0x8c6148feu); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c614900; /* ftrv xmtrx,fv0 */
L8c614900: sa_motion_publish_stop(ctx,0x8c614900u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614902; /* fmov.s @r4+,fr4 */
L8c614902: sa_motion_publish_stop(ctx,0x8c614902u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614904; /* fmov.s @r4,fr6 */
L8c614904: sa_motion_publish_stop(ctx,0x8c614904u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614906; /* fschg */
L8c614906: sa_motion_publish_stop(ctx,0x8c614906u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614908; /* fmov.s fr6,@-r8 */
L8c614908: sa_motion_publish_stop(ctx,0x8c614908u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61490a; /* fmov.s fr5,@-r8 */
L8c61490a: sa_motion_publish_stop(ctx,0x8c61490au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61490c; /* fmov.s fr4,@-r8 */
L8c61490c: sa_motion_publish_stop(ctx,0x8c61490cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61490e; /* mov.l r5,@-r8 */
L8c61490e: sa_motion_publish_stop(ctx,0x8c61490eu); r[8]+=28u;goto L8c614910; /* add #0x1c,r8 */
L8c614910: sa_motion_publish_stop(ctx,0x8c614910u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c614912; /* fldi1 fr2 */
L8c614912: sa_motion_publish_stop(ctx,0x8c614912u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614914; /* fmov.s fr2,@-r8 */
L8c614914: sa_motion_publish_stop(ctx,0x8c614914u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c614916; /* fmov.s fr1,@-r8 */
L8c614916: sa_motion_publish_stop(ctx,0x8c614916u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c614918; /* fmov.s fr0,@-r8 */
L8c614918: sa_motion_publish_stop(ctx,0x8c614918u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61491a; /* pref @r8 */
L8c61491a: sa_motion_publish_stop(ctx,0x8c61491au); r[8]+=16u;goto L8c61491c; /* add #0x10,r8 */
L8c61491c: sa_motion_publish_stop(ctx,0x8c61491cu); fcopy(f,4u,14u);goto L8c61491e; /* fmov fr14,fr4 */
L8c61491e: sa_motion_publish_stop(ctx,0x8c61491eu); r[0]=16u;goto L8c614920; /* mov #0x10,r0 */
L8c614920: sa_motion_publish_stop(ctx,0x8c614920u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614922; /* mov.l @r15,r3 */
L8c614922: sa_motion_publish_stop(ctx,0x8c614922u); r[4]=r[11];goto L8c614924; /* mov r11,r4 */
L8c614924: sa_motion_publish_stop(ctx,0x8c614924u); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c614926; /* fmov.s @(r0,r12),fr14 */
L8c614926: sa_motion_publish_stop(ctx,0x8c614926u); r[0]=20u;goto L8c614928; /* mov #0x14,r0 */
L8c614928: sa_motion_publish_stop(ctx,0x8c614928u); fcopy(f,5u,13u);goto L8c61492a; /* fmov fr13,fr5 */
L8c61492a: sa_motion_publish_stop(ctx,0x8c61492au); r[3]+=4294967295u;goto L8c61492c; /* add #-0x1,r3 */
L8c61492c: sa_motion_publish_stop(ctx,0x8c61492cu); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c61492e; /* fmov.s @(r0,r12),fr13 */
L8c61492e: sa_motion_publish_stop(ctx,0x8c61492eu); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614930; /* tst r3,r3 */
L8c614930: sa_motion_publish_stop(ctx,0x8c614930u); r[11]=r[12];goto L8c614932; /* mov r12,r11 */
L8c614932: sa_motion_publish_stop(ctx,0x8c614932u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c614934; /* fneg fr15 */
L8c614934: sa_motion_publish_stop(ctx,0x8c614936u); take=!(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c61483a;goto L8c614938; /* bf/s 0x8c61483a */
L8c614938: sa_motion_publish_stop(ctx,0x8c614938u); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c61493a; /* cmp/hs r8,r10 */
L8c61493a: sa_motion_publish_stop(ctx,0x8c61493au); if((*sr&1u))goto L8c614974;goto L8c61493c; /* bt 0x8c614974 */
L8c61493c: sa_motion_publish_stop(ctx,0x8c61493cu); r[4]=r[12];goto L8c61493e; /* mov r12,r4 */
L8c61493e: sa_motion_publish_stop(ctx,0x8c61493eu); r[5]=r[9];goto L8c614940; /* mov r9,r5 */
L8c614940: sa_motion_publish_stop(ctx,0x8c614940u); r[8]+=16u;goto L8c614942; /* add #0x10,r8 */
L8c614942: sa_motion_publish_stop(ctx,0x8c614942u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614944; /* fschg */
L8c614944: sa_motion_publish_stop(ctx,0x8c614944u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614946; /* fmov.s @r4+,fr0 */
L8c614946: sa_motion_publish_stop(ctx,0x8c614946u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614948; /* fmov.s @r4+,fr2 */
L8c614948: sa_motion_publish_stop(ctx,0x8c614948u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c61494a; /* fldi1 fr3 */
L8c61494a: sa_motion_publish_stop(ctx,0x8c61494au); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c61494c; /* ftrv xmtrx,fv0 */
L8c61494c: sa_motion_publish_stop(ctx,0x8c61494cu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61494e; /* fmov.s @r4+,fr4 */
L8c61494e: sa_motion_publish_stop(ctx,0x8c61494eu); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614950; /* fmov.s @r4,fr6 */
L8c614950: sa_motion_publish_stop(ctx,0x8c614950u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614952; /* fschg */
L8c614952: sa_motion_publish_stop(ctx,0x8c614952u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614954; /* fmov.s fr6,@-r8 */
L8c614954: sa_motion_publish_stop(ctx,0x8c614954u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614956; /* fmov.s fr5,@-r8 */
L8c614956: sa_motion_publish_stop(ctx,0x8c614956u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614958; /* fmov.s fr4,@-r8 */
L8c614958: sa_motion_publish_stop(ctx,0x8c614958u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61495a; /* mov.l r5,@-r8 */
L8c61495a: sa_motion_publish_stop(ctx,0x8c61495au); r[8]+=28u;goto L8c61495c; /* add #0x1c,r8 */
L8c61495c: sa_motion_publish_stop(ctx,0x8c61495cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c61495e; /* fldi1 fr2 */
L8c61495e: sa_motion_publish_stop(ctx,0x8c61495eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614960; /* fmov.s fr2,@-r8 */
L8c614960: sa_motion_publish_stop(ctx,0x8c614960u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c614962; /* fmov.s fr1,@-r8 */
L8c614962: sa_motion_publish_stop(ctx,0x8c614962u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c614964; /* fmov.s fr0,@-r8 */
L8c614964: sa_motion_publish_stop(ctx,0x8c614964u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614966; /* pref @r8 */
L8c614966: sa_motion_publish_stop(ctx,0x8c614966u); r[8]+=16u;goto L8c614968; /* add #0x10,r8 */
L8c614968: sa_motion_publish_stop(ctx,0x8c614968u); r[3]=r[8];goto L8c61496a; /* mov r8,r3 */
L8c61496a: sa_motion_publish_stop(ctx,0x8c61496au); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61496c; /* mov.l @(0x8,r15),r2 */
L8c61496c: sa_motion_publish_stop(ctx,0x8c61496cu); r[3]-=r[10];goto L8c61496e; /* sub r10,r3 */
L8c61496e: sa_motion_publish_stop(ctx,0x8c61496eu); r[3]+=32u;goto L8c614970; /* add #0x20,r3 */
L8c614970: sa_motion_publish_stop(ctx,0x8c614970u); r[2]+=r[3];goto L8c614972; /* add r3,r2 */
L8c614972: sa_motion_publish_stop(ctx,0x8c614972u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c614974; /* mov.l r2,@(0x8,r15) */
L8c614974: sa_motion_publish_stop(ctx,0x8c614974u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614976; /* mov.l @(0xc,r15),r3 */
L8c614976: sa_motion_publish_stop(ctx,0x8c614976u); r[3]+=4294967295u;goto L8c614978; /* add #-0x1,r3 */
L8c614978: sa_motion_publish_stop(ctx,0x8c614978u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c61497a; /* tst r3,r3 */
L8c61497a: sa_motion_publish_stop(ctx,0x8c61497cu); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c614982;goto L8c61497e; /* bt/s 0x8c614982 */
L8c61497e: sa_motion_publish_stop(ctx,0x8c614980u); goto L8c6147e8; /* bra 0x8c6147e8 */
L8c614982: sa_motion_publish_stop(ctx,0x8c614982u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614984; /* mov.l @(0x8,r15),r3 */
L8c614984: sa_motion_publish_stop(ctx,0x8c614984u); r[4]=rd(m,bus,&io_status,0x8c6149f0u,4);if(io_status)return io_status;goto L8c614986; /* mov.l 0x8c6149f0,r4 */
L8c614986: sa_motion_publish_stop(ctx,0x8c614986u); r[3]>>=2;goto L8c614988; /* shlr2 r3 */
L8c614988: sa_motion_publish_stop(ctx,0x8c614988u); r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61498a; /* mov.l @r4,r1 */
L8c61498a: sa_motion_publish_stop(ctx,0x8c61498au); r[3]>>=2;goto L8c61498c; /* shlr2 r3 */
L8c61498c: sa_motion_publish_stop(ctx,0x8c61498cu); *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c61498e; /* shlr r3 */
L8c61498e: sa_motion_publish_stop(ctx,0x8c61498eu); r[1]+=r[3];goto L8c614990; /* add r3,r1 */
L8c614990: sa_motion_publish_stop(ctx,0x8c614990u); wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c614992; /* mov.l r1,@r4 */
L8c614992: sa_motion_publish_stop(ctx,0x8c614992u); r[4]=rd(m,bus,&io_status,0x8c6149f4u,4);if(io_status)return io_status;goto L8c614994; /* mov.l 0x8c6149f4,r4 */
L8c614994: sa_motion_publish_stop(ctx,0x8c614994u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614996; /* mov.l @(0x4,r15),r3 */
L8c614996: sa_motion_publish_stop(ctx,0x8c614996u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c614998; /* mov.l @r4,r2 */
L8c614998: sa_motion_publish_stop(ctx,0x8c614998u); r[2]+=r[3];goto L8c61499a; /* add r3,r2 */
L8c61499a: sa_motion_publish_stop(ctx,0x8c61499au); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c61499c; /* mov.l r2,@r4 */
L8c61499c: sa_motion_publish_stop(ctx,0x8c61499cu); r[15]+=16u;goto L8c61499e; /* add #0x10,r15 */
L8c61499e: sa_motion_publish_stop(ctx,0x8c61499eu); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6149a0; /* lds.l @r15+,PR */
L8c6149a0: sa_motion_publish_stop(ctx,0x8c6149a0u); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6149a2; /* fmov.s @r15+,fr13 */
L8c6149a2: sa_motion_publish_stop(ctx,0x8c6149a2u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6149a4; /* fmov.s @r15+,fr14 */
L8c6149a4: sa_motion_publish_stop(ctx,0x8c6149a4u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6149a6; /* fmov.s @r15+,fr15 */
L8c6149a6: sa_motion_publish_stop(ctx,0x8c6149a6u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6149a8; /* mov.l @r15+,r9 */
L8c6149a8: sa_motion_publish_stop(ctx,0x8c6149a8u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6149aa; /* mov.l @r15+,r10 */
L8c6149aa: sa_motion_publish_stop(ctx,0x8c6149aau); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6149ac; /* mov.l @r15+,r11 */
L8c6149ac: sa_motion_publish_stop(ctx,0x8c6149acu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6149ae; /* mov.l @r15+,r12 */
L8c6149ae: sa_motion_publish_stop(ctx,0x8c6149aeu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6149b0; /* mov.l @r15+,r13 */
L8c6149b0: sa_motion_publish_stop(ctx,0x8c6149b2u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_environment_constant_alternate_strip_clipped_8c616844(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c616844;
L8c616844: sa_motion_publish_stop(ctx,0x8c616844u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c616846; /* mov.l r14,@-r15 */
L8c616846: sa_motion_publish_stop(ctx,0x8c616846u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c616848; /* mov.l r13,@-r15 */
L8c616848: sa_motion_publish_stop(ctx,0x8c616848u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61684a; /* mov.l r12,@-r15 */
L8c61684a: sa_motion_publish_stop(ctx,0x8c61684au); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61684c; /* mov.l r11,@-r15 */
L8c61684c: sa_motion_publish_stop(ctx,0x8c61684cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61684e; /* mov.l r10,@-r15 */
L8c61684e: sa_motion_publish_stop(ctx,0x8c61684eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c616850; /* mov.l r9,@-r15 */
L8c616850: sa_motion_publish_stop(ctx,0x8c616850u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c616852; /* fmov.s fr15,@-r15 */
L8c616852: sa_motion_publish_stop(ctx,0x8c616852u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c616854; /* fmov.s fr14,@-r15 */
L8c616854: sa_motion_publish_stop(ctx,0x8c616854u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c616856; /* fmov.s fr13,@-r15 */
L8c616856: sa_motion_publish_stop(ctx,0x8c616856u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c616858; /* fmov.s fr12,@-r15 */
L8c616858: sa_motion_publish_stop(ctx,0x8c616858u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61685a; /* sts.l PR,@-r15 */
L8c61685a: sa_motion_publish_stop(ctx,0x8c61685au); r[15]+=4294967220u;goto L8c61685c; /* add #-0x4c,r15 */
L8c61685c: sa_motion_publish_stop(ctx,0x8c61685cu); r[3]=rd(m,bus,&io_status,0x8c6168a0u,4);if(io_status)return io_status;goto L8c61685e; /* mov.l 0x8c6168a0,r3 */
L8c61685e: sa_motion_publish_stop(ctx,0x8c61685eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c616860; /* fldi1 fr12 */
L8c616860: sa_motion_publish_stop(ctx,0x8c616862u); target=r[3];*pr=0x8c616864u;wr(m,bus,&io_status,r[15]+0xcu,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c616864; /* jsr @r3 */
L8c616864: sa_motion_publish_stop(ctx,0x8c616864u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616866; /* mov.l @(0xc,r15),r3 */
L8c616866: sa_motion_publish_stop(ctx,0x8c616866u); r[2]=0u;goto L8c616868; /* mov #0x0,r2 */
L8c616868: sa_motion_publish_stop(ctx,0x8c616868u); r[14]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61686a; /* mov.l @(0xc,r15),r14 */
L8c61686a: sa_motion_publish_stop(ctx,0x8c61686au); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61686c; /* mov.w @(0x2,r3),r0 */
L8c61686c: sa_motion_publish_stop(ctx,0x8c61686cu); r[3]=rd(m,bus,&io_status,0x8c6168a4u,4);if(io_status)return io_status;goto L8c61686e; /* mov.l 0x8c6168a4,r3 */
L8c61686e: sa_motion_publish_stop(ctx,0x8c61686eu); r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c616870; /* mov.l @(0x4,r14),r14 */
L8c616870: sa_motion_publish_stop(ctx,0x8c616870u); r[0]=r[0]&65535u;goto L8c616872; /* extu.w r0,r0 */
L8c616872: sa_motion_publish_stop(ctx,0x8c616872u); wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c616874; /* mov.l r0,@(0x18,r15) */
L8c616874: sa_motion_publish_stop(ctx,0x8c616874u); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c616876; /* mov.l @r3,r13 */
L8c616876: sa_motion_publish_stop(ctx,0x8c616876u); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c616878; /* mov.l r2,@(0xc,r15) */
L8c616878: sa_motion_publish_stop(ctx,0x8c61687au); goto L8c6168a8; /* bra 0x8c6168a8 */
L8c6168a8: sa_motion_publish_stop(ctx,0x8c6168a8u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6168aa; /* mov.w @r14+,r3 */
L8c6168aa: sa_motion_publish_stop(ctx,0x8c6168aau); r[4]=rd(m,bus,&io_status,0x8c6169c4u,4);if(io_status)return io_status;goto L8c6168ac; /* mov.l 0x8c6169c4,r4 */
L8c6168ac: sa_motion_publish_stop(ctx,0x8c6168acu); r[3]=r[3]&65535u;goto L8c6168ae; /* extu.w r3,r3 */
L8c6168ae: sa_motion_publish_stop(ctx,0x8c6168aeu); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6168b0; /* tst r4,r3 */
L8c6168b0: sa_motion_publish_stop(ctx,0x8c6168b0u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6168b2; /* mov.l r3,@(0x8,r15) */
L8c6168b2: sa_motion_publish_stop(ctx,0x8c6168b4u); take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c6168c4;goto L8c6168b6; /* bt/s 0x8c6168c4 */
L8c6168b6: sa_motion_publish_stop(ctx,0x8c6168b6u); r[4]=sx(m,bus,&io_status,0x8c6169c0u,2);if(io_status)return io_status;goto L8c6168b8; /* mov.w 0x8c6169c0,r4 */
L8c6168b8: sa_motion_publish_stop(ctx,0x8c6168b8u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6168ba; /* mov.l @(0x8,r15),r3 */
L8c6168ba: sa_motion_publish_stop(ctx,0x8c6168bau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6168bc; /* fldi1 fr4 */
L8c6168bc: sa_motion_publish_stop(ctx,0x8c6168bcu); r[3]&=r[4];goto L8c6168be; /* and r4,r3 */
L8c6168be: sa_motion_publish_stop(ctx,0x8c6168beu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c6168c0; /* fneg fr4 */
L8c6168c0: sa_motion_publish_stop(ctx,0x8c6168c0u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6168c2; /* mov.l r3,@(0x8,r15) */
L8c6168c2: sa_motion_publish_stop(ctx,0x8c6168c2u); fcopy(f,15u,4u);goto L8c6168c4; /* fmov fr4,fr15 */
L8c6168c4: sa_motion_publish_stop(ctx,0x8c6168c4u); r[0]=16u;goto L8c6168c6; /* mov #0x10,r0 */
L8c6168c6: sa_motion_publish_stop(ctx,0x8c6168c6u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6168c8; /* mov.l @(0x8,r15),r3 */
L8c6168c8: sa_motion_publish_stop(ctx,0x8c6168c8u); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6168ca; /* mov.l @(0xc,r15),r2 */
L8c6168ca: sa_motion_publish_stop(ctx,0x8c6168cau); r[9]=r[8];goto L8c6168cc; /* mov r8,r9 */
L8c6168cc: sa_motion_publish_stop(ctx,0x8c6168ccu); r[2]+=r[3];goto L8c6168ce; /* add r3,r2 */
L8c6168ce: sa_motion_publish_stop(ctx,0x8c6168ceu); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6168d0; /* mov.l r2,@(0xc,r15) */
L8c6168d0: sa_motion_publish_stop(ctx,0x8c6168d0u); r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6168d2; /* mov.l @(0x8,r15),r1 */
L8c6168d2: sa_motion_publish_stop(ctx,0x8c6168d2u); r[1]+=4294967294u;goto L8c6168d4; /* add #-0x2,r1 */
L8c6168d4: sa_motion_publish_stop(ctx,0x8c6168d4u); wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c6168d6; /* mov.l r1,@(0x8,r15) */
L8c6168d6: sa_motion_publish_stop(ctx,0x8c6168d6u); r[10]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6168d8; /* mov.w @r14+,r10 */
L8c6168d8: sa_motion_publish_stop(ctx,0x8c6168d8u); r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6168da; /* mov.w @r14+,r11 */
L8c6168da: sa_motion_publish_stop(ctx,0x8c6168dau); r[10]=r[10]&65535u;goto L8c6168dc; /* extu.w r10,r10 */
L8c6168dc: sa_motion_publish_stop(ctx,0x8c6168dcu); r[10]<<=2;goto L8c6168de; /* shll2 r10 */
L8c6168de: sa_motion_publish_stop(ctx,0x8c6168deu); r[11]=r[11]&65535u;goto L8c6168e0; /* extu.w r11,r11 */
L8c6168e0: sa_motion_publish_stop(ctx,0x8c6168e0u); r[11]<<=2;goto L8c6168e2; /* shll2 r11 */
L8c6168e2: sa_motion_publish_stop(ctx,0x8c6168e2u); r[10]<<=2;goto L8c6168e4; /* shll2 r10 */
L8c6168e4: sa_motion_publish_stop(ctx,0x8c6168e4u); r[11]<<=2;goto L8c6168e6; /* shll2 r11 */
L8c6168e6: sa_motion_publish_stop(ctx,0x8c6168e6u); *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c6168e8; /* shll r10 */
L8c6168e8: sa_motion_publish_stop(ctx,0x8c6168e8u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c6168ea; /* shll r11 */
L8c6168ea: sa_motion_publish_stop(ctx,0x8c6168eau); r[11]+=r[13];goto L8c6168ec; /* add r13,r11 */
L8c6168ec: sa_motion_publish_stop(ctx,0x8c6168ecu); r[10]+=r[13];goto L8c6168ee; /* add r13,r10 */
L8c6168ee: sa_motion_publish_stop(ctx,0x8c6168eeu); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c6168f0; /* fmov.s @(r0,r11),fr13 */
L8c6168f0: sa_motion_publish_stop(ctx,0x8c6168f0u); fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c6168f2; /* fmov.s @(r0,r10),fr6 */
L8c6168f2: sa_motion_publish_stop(ctx,0x8c6168f2u); r[0]=20u;goto L8c6168f4; /* mov #0x14,r0 */
L8c6168f4: sa_motion_publish_stop(ctx,0x8c6168f4u); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c6168f6; /* fmov.s @(r0,r11),fr14 */
L8c6168f6: sa_motion_publish_stop(ctx,0x8c6168f6u); fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c6168f8; /* fmov.s @(r0,r10),fr5 */
L8c6168f8: sa_motion_publish_stop(ctx,0x8c6168f8u); r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6168fa; /* mov.w @r14+,r12 */
L8c6168fa: sa_motion_publish_stop(ctx,0x8c6168fau); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c6168fc; /* mov.w @r14,r3 */
L8c6168fc: sa_motion_publish_stop(ctx,0x8c6168fcu); r[12]=r[12]&65535u;goto L8c6168fe; /* extu.w r12,r12 */
L8c6168fe: sa_motion_publish_stop(ctx,0x8c6168feu); r[12]<<=2;goto L8c616900; /* shll2 r12 */
L8c616900: sa_motion_publish_stop(ctx,0x8c616900u); r[3]=r[3]&65535u;goto L8c616902; /* extu.w r3,r3 */
L8c616902: sa_motion_publish_stop(ctx,0x8c616902u); r[3]<<=2;goto L8c616904; /* shll2 r3 */
L8c616904: sa_motion_publish_stop(ctx,0x8c616904u); r[12]<<=2;goto L8c616906; /* shll2 r12 */
L8c616906: sa_motion_publish_stop(ctx,0x8c616906u); r[3]<<=2;goto L8c616908; /* shll2 r3 */
L8c616908: sa_motion_publish_stop(ctx,0x8c616908u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61690a; /* shll r12 */
L8c61690a: sa_motion_publish_stop(ctx,0x8c61690au); *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c61690c; /* shll r3 */
L8c61690c: sa_motion_publish_stop(ctx,0x8c61690cu); r[3]+=r[13];goto L8c61690e; /* add r13,r3 */
L8c61690e: sa_motion_publish_stop(ctx,0x8c61690eu); r[12]+=r[13];goto L8c616910; /* add r13,r12 */
L8c616910: sa_motion_publish_stop(ctx,0x8c616910u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c616912; /* pref @r3 */
L8c616912: sa_motion_publish_stop(ctx,0x8c616912u); r[2]=0u;goto L8c616914; /* mov #0x0,r2 */
L8c616914: sa_motion_publish_stop(ctx,0x8c616914u); wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c616916; /* mov.l r2,@(0x14,r15) */
L8c616916: sa_motion_publish_stop(ctx,0x8c616916u); r[4]=63u;goto L8c616918; /* mov #0x3f,r4 */
L8c616918: sa_motion_publish_stop(ctx,0x8c616918u); r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c61691a; /* mov.l @(0xc,r10),r2 */
L8c61691a: sa_motion_publish_stop(ctx,0x8c61691au); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61691c; /* mov.l @(0xc,r11),r3 */
L8c61691c: sa_motion_publish_stop(ctx,0x8c61691cu); r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61691e; /* mov.l @(0xc,r12),r1 */
L8c61691e: sa_motion_publish_stop(ctx,0x8c61691eu); r[2]&=r[3];goto L8c616920; /* and r3,r2 */
L8c616920: sa_motion_publish_stop(ctx,0x8c616920u); r[2]&=r[1];goto L8c616922; /* and r1,r2 */
L8c616922: sa_motion_publish_stop(ctx,0x8c616922u); r[4]&=r[2];goto L8c616924; /* and r2,r4 */
L8c616924: sa_motion_publish_stop(ctx,0x8c616924u); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c616926; /* tst r4,r4 */
L8c616926: sa_motion_publish_stop(ctx,0x8c616928u); take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c61692e;goto L8c61692a; /* bt/s 0x8c61692e */
L8c61692a: sa_motion_publish_stop(ctx,0x8c61692cu); goto L8c616a88; /* bra 0x8c616a88 */
L8c61692e: sa_motion_publish_stop(ctx,0x8c61692eu); r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c616930; /* mov.l @(0xc,r10),r4 */
L8c616930: sa_motion_publish_stop(ctx,0x8c616930u); r[5]=1u;goto L8c616932; /* mov #0x1,r5 */
L8c616932: sa_motion_publish_stop(ctx,0x8c616932u); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c616934; /* mov.l @(0xc,r11),r3 */
L8c616934: sa_motion_publish_stop(ctx,0x8c616934u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c616936; /* mov.l @(0xc,r12),r2 */
L8c616936: sa_motion_publish_stop(ctx,0x8c616936u); r[4]|=r[3];goto L8c616938; /* or r3,r4 */
L8c616938: sa_motion_publish_stop(ctx,0x8c616938u); r[4]|=r[2];goto L8c61693a; /* or r2,r4 */
L8c61693a: sa_motion_publish_stop(ctx,0x8c61693au); r[4]&=r[5];goto L8c61693c; /* and r5,r4 */
L8c61693c: sa_motion_publish_stop(ctx,0x8c61693cu); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c61693e; /* tst r4,r4 */
L8c61693e: sa_motion_publish_stop(ctx,0x8c61693eu); if(!(*sr&1u))goto L8c616944;goto L8c616940; /* bf 0x8c616944 */
L8c616940: sa_motion_publish_stop(ctx,0x8c616942u); goto L8c616a6a; /* bra 0x8c616a6a */
L8c616944: sa_motion_publish_stop(ctx,0x8c616944u); r[7]=r[15];goto L8c616946; /* mov r15,r7 */
L8c616946: sa_motion_publish_stop(ctx,0x8c616946u); r[7]+=64u;goto L8c616948; /* add #0x40,r7 */
L8c616948: sa_motion_publish_stop(ctx,0x8c616948u); wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c61694a; /* mov.l r10,@r7 */
L8c61694a: sa_motion_publish_stop(ctx,0x8c61694au); wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c61694c; /* mov.l r11,@(0x4,r7) */
L8c61694c: sa_motion_publish_stop(ctx,0x8c61694cu); wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c61694e; /* mov.l r12,@(0x8,r7) */
L8c61694e: sa_motion_publish_stop(ctx,0x8c616950u); r[6]=0u;goto L8c6169fc; /* bra 0x8c6169fc */
L8c616952: sa_motion_publish_stop(ctx,0x8c616952u); r[2]=r[6];goto L8c616954; /* mov r6,r2 */
L8c616954: sa_motion_publish_stop(ctx,0x8c616954u); r[1]=r[6];goto L8c616956; /* mov r6,r1 */
L8c616956: sa_motion_publish_stop(ctx,0x8c616956u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c616958; /* shll r2 */
L8c616958: sa_motion_publish_stop(ctx,0x8c616958u); r[3]=r[6];goto L8c61695a; /* mov r6,r3 */
L8c61695a: sa_motion_publish_stop(ctx,0x8c61695au); r[3]<<=2;goto L8c61695c; /* shll2 r3 */
L8c61695c: sa_motion_publish_stop(ctx,0x8c61695cu); r[2]+=r[1];goto L8c61695e; /* add r1,r2 */
L8c61695e: sa_motion_publish_stop(ctx,0x8c61695eu); r[2]<<=2;goto L8c616960; /* shll2 r2 */
L8c616960: sa_motion_publish_stop(ctx,0x8c616960u); r[3]+=r[7];goto L8c616962; /* add r7,r3 */
L8c616962: sa_motion_publish_stop(ctx,0x8c616962u); r[2]+=r[4];goto L8c616964; /* add r4,r2 */
L8c616964: sa_motion_publish_stop(ctx,0x8c616964u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c616966; /* mov.l r3,@(0x4,r15) */
L8c616966: sa_motion_publish_stop(ctx,0x8c616966u); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c616968; /* mov.l r2,@r15 */
L8c616968: sa_motion_publish_stop(ctx,0x8c616968u); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61696a; /* mov.l @r3,r1 */
L8c61696a: sa_motion_publish_stop(ctx,0x8c61696au); r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c61696c; /* mov.l @(0xc,r1),r2 */
L8c61696c: sa_motion_publish_stop(ctx,0x8c61696cu); *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c61696e; /* tst r5,r2 */
L8c61696e: sa_motion_publish_stop(ctx,0x8c616970u); take=!(*sr&1u);r[6]+=1u;if(take)goto L8c6169d4;goto L8c616972; /* bf/s 0x8c6169d4 */
L8c616972: sa_motion_publish_stop(ctx,0x8c616972u); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616974; /* mov.l @(0x4,r15),r1 */
L8c616974: sa_motion_publish_stop(ctx,0x8c616974u); r[0]=24u;goto L8c616976; /* mov #0x18,r0 */
L8c616976: sa_motion_publish_stop(ctx,0x8c616976u); fcopy(f,4u,12u);goto L8c616978; /* fmov fr12,fr4 */
L8c616978: sa_motion_publish_stop(ctx,0x8c616978u); r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61697a; /* mov.l @r1,r3 */
L8c61697a: sa_motion_publish_stop(ctx,0x8c61697au); r[4]=rd(m,bus,&io_status,0x8c6169c8u,4);if(io_status)return io_status;goto L8c61697c; /* mov.l 0x8c6169c8,r4 */
L8c61697c: sa_motion_publish_stop(ctx,0x8c61697cu); wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c61697e; /* mov.l r3,@(0x10,r15) */
L8c61697e: sa_motion_publish_stop(ctx,0x8c61697eu); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616980; /* fmov.s @(r0,r3),fr3 */
L8c616980: sa_motion_publish_stop(ctx,0x8c616980u); r[0]=12u;goto L8c616982; /* mov #0xc,r0 */
L8c616982: sa_motion_publish_stop(ctx,0x8c616982u); r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c616984; /* mov.l @(0x10,r15),r2 */
L8c616984: sa_motion_publish_stop(ctx,0x8c616984u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616986; /* fdiv fr3,fr4 */
L8c616986: sa_motion_publish_stop(ctx,0x8c616986u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c616988; /* fmov.s @(r0,r4),fr3 */
L8c616988: sa_motion_publish_stop(ctx,0x8c616988u); r[0]=16u;goto L8c61698a; /* mov #0x10,r0 */
L8c61698a: sa_motion_publish_stop(ctx,0x8c61698au); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61698c; /* mov.l @r15,r3 */
L8c61698c: sa_motion_publish_stop(ctx,0x8c61698cu); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61698e; /* fmov.s @(r0,r2),fr2 */
L8c61698e: sa_motion_publish_stop(ctx,0x8c61698eu); r[0]=16u;goto L8c616990; /* mov #0x10,r0 */
L8c616990: sa_motion_publish_stop(ctx,0x8c616990u); r[2]=rd(m,bus,&io_status,0x8c6169ccu,4);if(io_status)return io_status;goto L8c616992; /* mov.l 0x8c6169cc,r2 */
L8c616992: sa_motion_publish_stop(ctx,0x8c616992u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616994; /* fsub fr3,fr2 */
L8c616994: sa_motion_publish_stop(ctx,0x8c616994u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c616996; /* fmov.s @r2,fr3 */
L8c616996: sa_motion_publish_stop(ctx,0x8c616996u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616998; /* fmul fr4,fr2 */
L8c616998: sa_motion_publish_stop(ctx,0x8c616998u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61699a; /* fdiv fr3,fr2 */
L8c61699a: sa_motion_publish_stop(ctx,0x8c61699au); fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61699c; /* fmov.s fr2,@r3 */
L8c61699c: sa_motion_publish_stop(ctx,0x8c61699cu); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61699e; /* mov.l @(0x4,r15),r1 */
L8c61699e: sa_motion_publish_stop(ctx,0x8c61699eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6169a0; /* fmov.s @(r0,r4),fr3 */
L8c6169a0: sa_motion_publish_stop(ctx,0x8c6169a0u); r[0]=20u;goto L8c6169a2; /* mov #0x14,r0 */
L8c6169a2: sa_motion_publish_stop(ctx,0x8c6169a2u); r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c6169a4; /* mov.l @r1,r1 */
L8c6169a4: sa_motion_publish_stop(ctx,0x8c6169a4u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6169a6; /* mov.l @r15,r3 */
L8c6169a6: sa_motion_publish_stop(ctx,0x8c6169a6u); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c6169a8; /* fmov.s @(r0,r1),fr2 */
L8c6169a8: sa_motion_publish_stop(ctx,0x8c6169a8u); r[1]=rd(m,bus,&io_status,0x8c6169d0u,4);if(io_status)return io_status;goto L8c6169aa; /* mov.l 0x8c6169d0,r1 */
L8c6169aa: sa_motion_publish_stop(ctx,0x8c6169aau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6169ac; /* fsub fr3,fr2 */
L8c6169ac: sa_motion_publish_stop(ctx,0x8c6169acu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6169ae; /* fmov.s @r1,fr3 */
L8c6169ae: sa_motion_publish_stop(ctx,0x8c6169aeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6169b0; /* fmul fr4,fr2 */
L8c6169b0: sa_motion_publish_stop(ctx,0x8c6169b0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6169b2; /* fdiv fr3,fr2 */
L8c6169b2: sa_motion_publish_stop(ctx,0x8c6169b2u); r[0]=4u;goto L8c6169b4; /* mov #0x4,r0 */
L8c6169b4: sa_motion_publish_stop(ctx,0x8c6169b4u); fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c6169b6; /* fmov fr2,@(r0,r3) */
L8c6169b6: sa_motion_publish_stop(ctx,0x8c6169b6u); r[0]=8u;goto L8c6169b8; /* mov #0x8,r0 */
L8c6169b8: sa_motion_publish_stop(ctx,0x8c6169b8u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6169ba; /* mov.l @r15,r3 */
L8c6169ba: sa_motion_publish_stop(ctx,0x8c6169bau); fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c6169bc; /* fmov fr4,@(r0,r3) */
L8c6169bc: sa_motion_publish_stop(ctx,0x8c6169beu); goto L8c6169fc; /* bra 0x8c6169fc */
L8c6169d4: sa_motion_publish_stop(ctx,0x8c6169d4u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6169d6; /* mov.l @(0x4,r15),r3 */
L8c6169d6: sa_motion_publish_stop(ctx,0x8c6169d6u); r[0]=16u;goto L8c6169d8; /* mov #0x10,r0 */
L8c6169d8: sa_motion_publish_stop(ctx,0x8c6169d8u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6169da; /* mov.l @r15,r2 */
L8c6169da: sa_motion_publish_stop(ctx,0x8c6169dau); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6169dc; /* mov.l @r3,r1 */
L8c6169dc: sa_motion_publish_stop(ctx,0x8c6169dcu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6169de; /* fmov.s @(r0,r1),fr3 */
L8c6169de: sa_motion_publish_stop(ctx,0x8c6169deu); r[0]=20u;goto L8c6169e0; /* mov #0x14,r0 */
L8c6169e0: sa_motion_publish_stop(ctx,0x8c6169e0u); fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6169e2; /* fmov.s fr3,@r2 */
L8c6169e2: sa_motion_publish_stop(ctx,0x8c6169e2u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6169e4; /* mov.l @(0x4,r15),r2 */
L8c6169e4: sa_motion_publish_stop(ctx,0x8c6169e4u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6169e6; /* mov.l @r15,r3 */
L8c6169e6: sa_motion_publish_stop(ctx,0x8c6169e6u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6169e8; /* mov.l @r2,r1 */
L8c6169e8: sa_motion_publish_stop(ctx,0x8c6169e8u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6169ea; /* fmov.s @(r0,r1),fr3 */
L8c6169ea: sa_motion_publish_stop(ctx,0x8c6169eau); r[0]=4u;goto L8c6169ec; /* mov #0x4,r0 */
L8c6169ec: sa_motion_publish_stop(ctx,0x8c6169ecu); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6169ee; /* fmov fr3,@(r0,r3) */
L8c6169ee: sa_motion_publish_stop(ctx,0x8c6169eeu); r[0]=24u;goto L8c6169f0; /* mov #0x18,r0 */
L8c6169f0: sa_motion_publish_stop(ctx,0x8c6169f0u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6169f2; /* mov.l @(0x4,r15),r2 */
L8c6169f2: sa_motion_publish_stop(ctx,0x8c6169f2u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6169f4; /* mov.l @r15,r3 */
L8c6169f4: sa_motion_publish_stop(ctx,0x8c6169f4u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6169f6; /* mov.l @r2,r1 */
L8c6169f6: sa_motion_publish_stop(ctx,0x8c6169f6u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6169f8; /* fmov.s @(r0,r1),fr3 */
L8c6169f8: sa_motion_publish_stop(ctx,0x8c6169f8u); r[0]=8u;goto L8c6169fa; /* mov #0x8,r0 */
L8c6169fa: sa_motion_publish_stop(ctx,0x8c6169fau); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6169fc; /* fmov fr3,@(r0,r3) */
L8c6169fc: sa_motion_publish_stop(ctx,0x8c6169fcu); r[3]=3u;goto L8c6169fe; /* mov #0x3,r3 */
L8c6169fe: sa_motion_publish_stop(ctx,0x8c6169feu); r[4]=r[15];goto L8c616a00; /* mov r15,r4 */
L8c616a00: sa_motion_publish_stop(ctx,0x8c616a00u); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c616a02; /* cmp/ge r3,r6 */
L8c616a02: sa_motion_publish_stop(ctx,0x8c616a04u); take=!(*sr&1u);r[4]+=28u;if(take)goto L8c616952;goto L8c616a06; /* bf/s 0x8c616952 */
L8c616a06: sa_motion_publish_stop(ctx,0x8c616a06u); r[6]=r[4];goto L8c616a08; /* mov r4,r6 */
L8c616a08: sa_motion_publish_stop(ctx,0x8c616a08u); r[0]=8u;goto L8c616a0a; /* mov #0x8,r0 */
L8c616a0a: sa_motion_publish_stop(ctx,0x8c616a0au); r[3]=r[4];goto L8c616a0c; /* mov r4,r3 */
L8c616a0c: sa_motion_publish_stop(ctx,0x8c616a0cu); fload(m,bus,&io_status,f,6u,r[0]+r[6]);if(io_status)return io_status;goto L8c616a0e; /* fmov.s @(r0,r6),fr6 */
L8c616a0e: sa_motion_publish_stop(ctx,0x8c616a0eu); r[3]+=12u;goto L8c616a10; /* add #0xc,r3 */
L8c616a10: sa_motion_publish_stop(ctx,0x8c616a10u); r[7]=r[4];goto L8c616a12; /* mov r4,r7 */
L8c616a12: sa_motion_publish_stop(ctx,0x8c616a12u); r[0]=4u;goto L8c616a14; /* mov #0x4,r0 */
L8c616a14: sa_motion_publish_stop(ctx,0x8c616a14u); fcopy(f,0u,6u);goto L8c616a16; /* fmov fr6,fr0 */
L8c616a16: sa_motion_publish_stop(ctx,0x8c616a16u); r[7]+=24u;goto L8c616a18; /* add #0x18,r7 */
L8c616a18: sa_motion_publish_stop(ctx,0x8c616a18u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c616a1a; /* mov.l r3,@r15 */
L8c616a1a: sa_motion_publish_stop(ctx,0x8c616a1au); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c616a1c; /* fmov.s @(r0,r6),fr4 */
L8c616a1c: sa_motion_publish_stop(ctx,0x8c616a1cu); fload(m,bus,&io_status,f,5u,r[0]+r[3]);if(io_status)return io_status;goto L8c616a1e; /* fmov.s @(r0,r3),fr5 */
L8c616a1e: sa_motion_publish_stop(ctx,0x8c616a1eu); r[0]=8u;goto L8c616a20; /* mov #0x8,r0 */
L8c616a20: sa_motion_publish_stop(ctx,0x8c616a20u); fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c616a22; /* fmov.s @(r0,r3),fr9 */
L8c616a22: sa_motion_publish_stop(ctx,0x8c616a22u); r[0]=4u;goto L8c616a24; /* mov #0x4,r0 */
L8c616a24: sa_motion_publish_stop(ctx,0x8c616a24u); fcopy(f,3u,4u);goto L8c616a26; /* fmov fr4,fr3 */
L8c616a26: sa_motion_publish_stop(ctx,0x8c616a26u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c616a28; /* fmul fr5,fr0 */
L8c616a28: sa_motion_publish_stop(ctx,0x8c616a28u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616a2a; /* fmul fr9,fr3 */
L8c616a2a: sa_motion_publish_stop(ctx,0x8c616a2au); fload(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;goto L8c616a2c; /* fmov.s @r6,fr8 */
L8c616a2c: sa_motion_publish_stop(ctx,0x8c616a2cu); fload(m,bus,&io_status,f,7u,r[3]);if(io_status)return io_status;goto L8c616a2e; /* fmov.s @r3,fr7 */
L8c616a2e: sa_motion_publish_stop(ctx,0x8c616a2eu); fcopy(f,2u,8u);goto L8c616a30; /* fmov fr8,fr2 */
L8c616a30: sa_motion_publish_stop(ctx,0x8c616a30u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[9],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616a32; /* fmul fr9,fr2 */
L8c616a32: sa_motion_publish_stop(ctx,0x8c616a32u); fcopy(f,1u,4u);goto L8c616a34; /* fmov fr4,fr1 */
L8c616a34: sa_motion_publish_stop(ctx,0x8c616a34u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c616a36; /* fsub fr3,fr0 */
L8c616a36: sa_motion_publish_stop(ctx,0x8c616a36u); fcopy(f,3u,6u);goto L8c616a38; /* fmov fr6,fr3 */
L8c616a38: sa_motion_publish_stop(ctx,0x8c616a38u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616a3a; /* fmul fr7,fr3 */
L8c616a3a: sa_motion_publish_stop(ctx,0x8c616a3au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616a3c; /* fsub fr3,fr2 */
L8c616a3c: sa_motion_publish_stop(ctx,0x8c616a3cu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c616a3e; /* fmov.s @(r0,r7),fr3 */
L8c616a3e: sa_motion_publish_stop(ctx,0x8c616a3eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616a40; /* fmul fr2,fr3 */
L8c616a40: sa_motion_publish_stop(ctx,0x8c616a40u); fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c616a42; /* fmov.s @r7,fr2 */
L8c616a42: sa_motion_publish_stop(ctx,0x8c616a42u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616a44; /* fmac fr0,fr2,fr3 */
L8c616a44: sa_motion_publish_stop(ctx,0x8c616a44u); fcopy(f,2u,3u);goto L8c616a46; /* fmov fr3,fr2 */
L8c616a46: sa_motion_publish_stop(ctx,0x8c616a46u); fcopy(f,3u,8u);goto L8c616a48; /* fmov fr8,fr3 */
L8c616a48: sa_motion_publish_stop(ctx,0x8c616a48u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616a4a; /* fmul fr5,fr3 */
L8c616a4a: sa_motion_publish_stop(ctx,0x8c616a4au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c616a4c; /* fmul fr7,fr1 */
L8c616a4c: sa_motion_publish_stop(ctx,0x8c616a4cu); r[0]=8u;goto L8c616a4e; /* mov #0x8,r0 */
L8c616a4e: sa_motion_publish_stop(ctx,0x8c616a4eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c616a50; /* fsub fr3,fr1 */
L8c616a50: sa_motion_publish_stop(ctx,0x8c616a50u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c616a52; /* fmov.s @(r0,r7),fr3 */
L8c616a52: sa_motion_publish_stop(ctx,0x8c616a52u); fcopy(f,0u,1u);goto L8c616a54; /* fmov fr1,fr0 */
L8c616a54: sa_motion_publish_stop(ctx,0x8c616a54u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616a56; /* fmac fr0,fr3,fr2 */
L8c616a56: sa_motion_publish_stop(ctx,0x8c616a56u); fcopy(f,3u,15u);goto L8c616a58; /* fmov fr15,fr3 */
L8c616a58: sa_motion_publish_stop(ctx,0x8c616a58u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c616a5a; /* fneg fr3 */
L8c616a5a: sa_motion_publish_stop(ctx,0x8c616a5au); fcopy(f,4u,2u);goto L8c616a5c; /* fmov fr2,fr4 */
L8c616a5c: sa_motion_publish_stop(ctx,0x8c616a5cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616a5e; /* fmul fr3,fr4 */
L8c616a5e: sa_motion_publish_stop(ctx,0x8c616a5eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c616a60; /* fldi0 fr2 */
L8c616a60: sa_motion_publish_stop(ctx,0x8c616a60u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c616a62; /* fcmp/gt fr2,fr4 */
L8c616a62: sa_motion_publish_stop(ctx,0x8c616a62u); if((*sr&1u))goto L8c616a88;goto L8c616a64; /* bt 0x8c616a88 */
L8c616a64: sa_motion_publish_stop(ctx,0x8c616a64u); wr(m,bus,&io_status,r[15]+0x14u,r[5],4);if(io_status)return io_status;goto L8c616a66; /* mov.l r5,@(0x14,r15) */
L8c616a66: sa_motion_publish_stop(ctx,0x8c616a68u); fcopy(f,4u,12u);goto L8c616a88; /* bra 0x8c616a88 */
L8c616a6a: sa_motion_publish_stop(ctx,0x8c616a6au); r[0]=20u;goto L8c616a6c; /* mov #0x14,r0 */
L8c616a6c: sa_motion_publish_stop(ctx,0x8c616a6cu); fcopy(f,3u,13u);goto L8c616a6e; /* fmov fr13,fr3 */
L8c616a6e: sa_motion_publish_stop(ctx,0x8c616a6eu); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c616a70; /* fmov.s @(r0,r12),fr2 */
L8c616a70: sa_motion_publish_stop(ctx,0x8c616a70u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c616a72; /* fsub fr6,fr3 */
L8c616a72: sa_motion_publish_stop(ctx,0x8c616a72u); r[0]=16u;goto L8c616a74; /* mov #0x10,r0 */
L8c616a74: sa_motion_publish_stop(ctx,0x8c616a74u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616a76; /* fsub fr14,fr2 */
L8c616a76: sa_motion_publish_stop(ctx,0x8c616a76u); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c616a78; /* fmov.s @(r0,r12),fr1 */
L8c616a78: sa_motion_publish_stop(ctx,0x8c616a78u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c616a7a; /* fsub fr13,fr1 */
L8c616a7a: sa_motion_publish_stop(ctx,0x8c616a7au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616a7c; /* fmul fr2,fr3 */
L8c616a7c: sa_motion_publish_stop(ctx,0x8c616a7cu); fcopy(f,2u,14u);goto L8c616a7e; /* fmov fr14,fr2 */
L8c616a7e: sa_motion_publish_stop(ctx,0x8c616a7eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616a80; /* fsub fr5,fr2 */
L8c616a80: sa_motion_publish_stop(ctx,0x8c616a80u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616a82; /* fmul fr1,fr2 */
L8c616a82: sa_motion_publish_stop(ctx,0x8c616a82u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c616a84; /* fsub fr2,fr3 */
L8c616a84: sa_motion_publish_stop(ctx,0x8c616a84u); fcopy(f,4u,3u);goto L8c616a86; /* fmov fr3,fr4 */
L8c616a86: sa_motion_publish_stop(ctx,0x8c616a86u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616a88; /* fmul fr15,fr4 */
L8c616a88: sa_motion_publish_stop(ctx,0x8c616a88u); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c616a8a; /* cmp/hs r8,r9 */
L8c616a8a: sa_motion_publish_stop(ctx,0x8c616a8au); if((*sr&1u))goto L8c616ac2;goto L8c616a8c; /* bt 0x8c616ac2 */
L8c616a8c: sa_motion_publish_stop(ctx,0x8c616a8cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c616a8e; /* fldi0 fr3 */
L8c616a8e: sa_motion_publish_stop(ctx,0x8c616a8eu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c616a90; /* fcmp/gt fr4,fr3 */
L8c616a90: sa_motion_publish_stop(ctx,0x8c616a90u); if((*sr&1u))goto L8c616af4;goto L8c616a92; /* bt 0x8c616af4 */
L8c616a92: sa_motion_publish_stop(ctx,0x8c616a92u); r[5]=rd(m,bus,&io_status,0x8c616bfcu,4);if(io_status)return io_status;goto L8c616a94; /* mov.l 0x8c616bfc,r5 */
L8c616a94: sa_motion_publish_stop(ctx,0x8c616a94u); r[4]=r[11];goto L8c616a96; /* mov r11,r4 */
L8c616a96: sa_motion_publish_stop(ctx,0x8c616a96u); r[8]+=16u;goto L8c616a98; /* add #0x10,r8 */
L8c616a98: sa_motion_publish_stop(ctx,0x8c616a98u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616a9a; /* fschg */
L8c616a9a: sa_motion_publish_stop(ctx,0x8c616a9au); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616a9c; /* fmov.s @r4+,fr0 */
L8c616a9c: sa_motion_publish_stop(ctx,0x8c616a9cu); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616a9e; /* fmov.s @r4+,fr2 */
L8c616a9e: sa_motion_publish_stop(ctx,0x8c616a9eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c616aa0; /* fldi1 fr3 */
L8c616aa0: sa_motion_publish_stop(ctx,0x8c616aa0u); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c616aa2; /* ftrv xmtrx,fv0 */
L8c616aa2: sa_motion_publish_stop(ctx,0x8c616aa2u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616aa4; /* fmov.s @r4+,fr4 */
L8c616aa4: sa_motion_publish_stop(ctx,0x8c616aa4u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616aa6; /* fmov.s @r4,fr6 */
L8c616aa6: sa_motion_publish_stop(ctx,0x8c616aa6u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616aa8; /* fschg */
L8c616aa8: sa_motion_publish_stop(ctx,0x8c616aa8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616aaa; /* fmov.s fr6,@-r8 */
L8c616aaa: sa_motion_publish_stop(ctx,0x8c616aaau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616aac; /* fmov.s fr5,@-r8 */
L8c616aac: sa_motion_publish_stop(ctx,0x8c616aacu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616aae; /* fmov.s fr4,@-r8 */
L8c616aae: sa_motion_publish_stop(ctx,0x8c616aaeu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616ab0; /* mov.l r5,@-r8 */
L8c616ab0: sa_motion_publish_stop(ctx,0x8c616ab0u); r[8]+=28u;goto L8c616ab2; /* add #0x1c,r8 */
L8c616ab2: sa_motion_publish_stop(ctx,0x8c616ab2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c616ab4; /* fldi1 fr2 */
L8c616ab4: sa_motion_publish_stop(ctx,0x8c616ab4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616ab6; /* fmov.s fr2,@-r8 */
L8c616ab6: sa_motion_publish_stop(ctx,0x8c616ab6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616ab8; /* fmov.s fr1,@-r8 */
L8c616ab8: sa_motion_publish_stop(ctx,0x8c616ab8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616aba; /* fmov.s fr0,@-r8 */
L8c616aba: sa_motion_publish_stop(ctx,0x8c616abau); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616abc; /* pref @r8 */
L8c616abc: sa_motion_publish_stop(ctx,0x8c616abcu); r[8]+=16u;goto L8c616abe; /* add #0x10,r8 */
L8c616abe: sa_motion_publish_stop(ctx,0x8c616ac0u); r[9]=r[8];goto L8c616b20; /* bra 0x8c616b20 */
L8c616ac2: sa_motion_publish_stop(ctx,0x8c616ac2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c616ac4; /* fldi0 fr3 */
L8c616ac4: sa_motion_publish_stop(ctx,0x8c616ac4u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c616ac6; /* fcmp/gt fr4,fr3 */
L8c616ac6: sa_motion_publish_stop(ctx,0x8c616ac6u); if(!(*sr&1u))goto L8c616b20;goto L8c616ac8; /* bf 0x8c616b20 */
L8c616ac8: sa_motion_publish_stop(ctx,0x8c616ac8u); r[5]=rd(m,bus,&io_status,0x8c616c00u,4);if(io_status)return io_status;goto L8c616aca; /* mov.l 0x8c616c00,r5 */
L8c616aca: sa_motion_publish_stop(ctx,0x8c616acau); r[4]=r[10];goto L8c616acc; /* mov r10,r4 */
L8c616acc: sa_motion_publish_stop(ctx,0x8c616accu); r[8]+=16u;goto L8c616ace; /* add #0x10,r8 */
L8c616ace: sa_motion_publish_stop(ctx,0x8c616aceu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616ad0; /* fschg */
L8c616ad0: sa_motion_publish_stop(ctx,0x8c616ad0u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616ad2; /* fmov.s @r4+,fr0 */
L8c616ad2: sa_motion_publish_stop(ctx,0x8c616ad2u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616ad4; /* fmov.s @r4+,fr2 */
L8c616ad4: sa_motion_publish_stop(ctx,0x8c616ad4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c616ad6; /* fldi1 fr3 */
L8c616ad6: sa_motion_publish_stop(ctx,0x8c616ad6u); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c616ad8; /* ftrv xmtrx,fv0 */
L8c616ad8: sa_motion_publish_stop(ctx,0x8c616ad8u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616ada; /* fmov.s @r4+,fr4 */
L8c616ada: sa_motion_publish_stop(ctx,0x8c616adau); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616adc; /* fmov.s @r4,fr6 */
L8c616adc: sa_motion_publish_stop(ctx,0x8c616adcu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616ade; /* fschg */
L8c616ade: sa_motion_publish_stop(ctx,0x8c616adeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616ae0; /* fmov.s fr6,@-r8 */
L8c616ae0: sa_motion_publish_stop(ctx,0x8c616ae0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616ae2; /* fmov.s fr5,@-r8 */
L8c616ae2: sa_motion_publish_stop(ctx,0x8c616ae2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616ae4; /* fmov.s fr4,@-r8 */
L8c616ae4: sa_motion_publish_stop(ctx,0x8c616ae4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616ae6; /* mov.l r5,@-r8 */
L8c616ae6: sa_motion_publish_stop(ctx,0x8c616ae6u); r[8]+=28u;goto L8c616ae8; /* add #0x1c,r8 */
L8c616ae8: sa_motion_publish_stop(ctx,0x8c616ae8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c616aea; /* fldi1 fr2 */
L8c616aea: sa_motion_publish_stop(ctx,0x8c616aeau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616aec; /* fmov.s fr2,@-r8 */
L8c616aec: sa_motion_publish_stop(ctx,0x8c616aecu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616aee; /* fmov.s fr1,@-r8 */
L8c616aee: sa_motion_publish_stop(ctx,0x8c616aeeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616af0; /* fmov.s fr0,@-r8 */
L8c616af0: sa_motion_publish_stop(ctx,0x8c616af0u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616af2; /* pref @r8 */
L8c616af2: sa_motion_publish_stop(ctx,0x8c616af2u); r[8]+=16u;goto L8c616af4; /* add #0x10,r8 */
L8c616af4: sa_motion_publish_stop(ctx,0x8c616af4u); r[5]=rd(m,bus,&io_status,0x8c616c00u,4);if(io_status)return io_status;goto L8c616af6; /* mov.l 0x8c616c00,r5 */
L8c616af6: sa_motion_publish_stop(ctx,0x8c616af6u); r[4]=r[11];goto L8c616af8; /* mov r11,r4 */
L8c616af8: sa_motion_publish_stop(ctx,0x8c616af8u); r[8]+=16u;goto L8c616afa; /* add #0x10,r8 */
L8c616afa: sa_motion_publish_stop(ctx,0x8c616afau); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616afc; /* fschg */
L8c616afc: sa_motion_publish_stop(ctx,0x8c616afcu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616afe; /* fmov.s @r4+,fr0 */
L8c616afe: sa_motion_publish_stop(ctx,0x8c616afeu); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616b00; /* fmov.s @r4+,fr2 */
L8c616b00: sa_motion_publish_stop(ctx,0x8c616b00u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c616b02; /* fldi1 fr3 */
L8c616b02: sa_motion_publish_stop(ctx,0x8c616b02u); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c616b04; /* ftrv xmtrx,fv0 */
L8c616b04: sa_motion_publish_stop(ctx,0x8c616b04u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616b06; /* fmov.s @r4+,fr4 */
L8c616b06: sa_motion_publish_stop(ctx,0x8c616b06u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616b08; /* fmov.s @r4,fr6 */
L8c616b08: sa_motion_publish_stop(ctx,0x8c616b08u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616b0a; /* fschg */
L8c616b0a: sa_motion_publish_stop(ctx,0x8c616b0au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616b0c; /* fmov.s fr6,@-r8 */
L8c616b0c: sa_motion_publish_stop(ctx,0x8c616b0cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616b0e; /* fmov.s fr5,@-r8 */
L8c616b0e: sa_motion_publish_stop(ctx,0x8c616b0eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616b10; /* fmov.s fr4,@-r8 */
L8c616b10: sa_motion_publish_stop(ctx,0x8c616b10u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616b12; /* mov.l r5,@-r8 */
L8c616b12: sa_motion_publish_stop(ctx,0x8c616b12u); r[8]+=28u;goto L8c616b14; /* add #0x1c,r8 */
L8c616b14: sa_motion_publish_stop(ctx,0x8c616b14u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c616b16; /* fldi1 fr2 */
L8c616b16: sa_motion_publish_stop(ctx,0x8c616b16u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616b18; /* fmov.s fr2,@-r8 */
L8c616b18: sa_motion_publish_stop(ctx,0x8c616b18u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616b1a; /* fmov.s fr1,@-r8 */
L8c616b1a: sa_motion_publish_stop(ctx,0x8c616b1au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616b1c; /* fmov.s fr0,@-r8 */
L8c616b1c: sa_motion_publish_stop(ctx,0x8c616b1cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616b1e; /* pref @r8 */
L8c616b1e: sa_motion_publish_stop(ctx,0x8c616b1eu); r[8]+=16u;goto L8c616b20; /* add #0x10,r8 */
L8c616b20: sa_motion_publish_stop(ctx,0x8c616b20u); r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c616b22; /* mov.l @(0x14,r15),r2 */
L8c616b22: sa_motion_publish_stop(ctx,0x8c616b22u); *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c616b24; /* tst r2,r2 */
L8c616b24: sa_motion_publish_stop(ctx,0x8c616b24u); if((*sr&1u))goto L8c616b38;goto L8c616b26; /* bt 0x8c616b38 */
L8c616b26: sa_motion_publish_stop(ctx,0x8c616b26u); r[4]=rd(m,bus,&io_status,0x8c616c04u,4);if(io_status)return io_status;goto L8c616b28; /* mov.l 0x8c616c04,r4 */
L8c616b28: sa_motion_publish_stop(ctx,0x8c616b28u); r[3]=0u;goto L8c616b2a; /* mov #0x0,r3 */
L8c616b2a: sa_motion_publish_stop(ctx,0x8c616b2au); r[2]=rd(m,bus,&io_status,0x8c616c08u,4);if(io_status)return io_status;goto L8c616b2c; /* mov.l 0x8c616c08,r2 */
L8c616b2c: sa_motion_publish_stop(ctx,0x8c616b2cu); r[5]=r[11];goto L8c616b2e; /* mov r11,r5 */
L8c616b2e: sa_motion_publish_stop(ctx,0x8c616b2eu); wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c616b30; /* mov.l r3,@r4 */
L8c616b30: sa_motion_publish_stop(ctx,0x8c616b30u); r[6]=r[12];goto L8c616b32; /* mov r12,r6 */
L8c616b32: sa_motion_publish_stop(ctx,0x8c616b34u); target=r[2];*pr=0x8c616b36u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c616b36; /* jsr @r2 */
L8c616b36: sa_motion_publish_stop(ctx,0x8c616b36u); r[9]=r[8];goto L8c616b38; /* mov r8,r9 */
L8c616b38: sa_motion_publish_stop(ctx,0x8c616b38u); fcopy(f,6u,13u);goto L8c616b3a; /* fmov fr13,fr6 */
L8c616b3a: sa_motion_publish_stop(ctx,0x8c616b3au); r[0]=16u;goto L8c616b3c; /* mov #0x10,r0 */
L8c616b3c: sa_motion_publish_stop(ctx,0x8c616b3cu); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c616b3e; /* mov.l @(0x8,r15),r3 */
L8c616b3e: sa_motion_publish_stop(ctx,0x8c616b3eu); r[10]=r[11];goto L8c616b40; /* mov r11,r10 */
L8c616b40: sa_motion_publish_stop(ctx,0x8c616b40u); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c616b42; /* fmov.s @(r0,r12),fr13 */
L8c616b42: sa_motion_publish_stop(ctx,0x8c616b42u); r[0]=20u;goto L8c616b44; /* mov #0x14,r0 */
L8c616b44: sa_motion_publish_stop(ctx,0x8c616b44u); fcopy(f,5u,14u);goto L8c616b46; /* fmov fr14,fr5 */
L8c616b46: sa_motion_publish_stop(ctx,0x8c616b46u); r[3]+=4294967295u;goto L8c616b48; /* add #-0x1,r3 */
L8c616b48: sa_motion_publish_stop(ctx,0x8c616b48u); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c616b4a; /* fmov.s @(r0,r12),fr14 */
L8c616b4a: sa_motion_publish_stop(ctx,0x8c616b4au); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c616b4c; /* tst r3,r3 */
L8c616b4c: sa_motion_publish_stop(ctx,0x8c616b4cu); r[11]=r[12];goto L8c616b4e; /* mov r12,r11 */
L8c616b4e: sa_motion_publish_stop(ctx,0x8c616b4eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c616b50; /* fneg fr15 */
L8c616b50: sa_motion_publish_stop(ctx,0x8c616b52u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c616b58;goto L8c616b54; /* bt/s 0x8c616b58 */
L8c616b54: sa_motion_publish_stop(ctx,0x8c616b56u); goto L8c6168f8; /* bra 0x8c6168f8 */
L8c616b58: sa_motion_publish_stop(ctx,0x8c616b58u); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c616b5a; /* cmp/hs r8,r9 */
L8c616b5a: sa_motion_publish_stop(ctx,0x8c616b5au); if((*sr&1u))goto L8c616b88;goto L8c616b5c; /* bt 0x8c616b88 */
L8c616b5c: sa_motion_publish_stop(ctx,0x8c616b5cu); r[5]=rd(m,bus,&io_status,0x8c616bfcu,4);if(io_status)return io_status;goto L8c616b5e; /* mov.l 0x8c616bfc,r5 */
L8c616b5e: sa_motion_publish_stop(ctx,0x8c616b5eu); r[4]=r[12];goto L8c616b60; /* mov r12,r4 */
L8c616b60: sa_motion_publish_stop(ctx,0x8c616b60u); r[8]+=16u;goto L8c616b62; /* add #0x10,r8 */
L8c616b62: sa_motion_publish_stop(ctx,0x8c616b62u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616b64; /* fschg */
L8c616b64: sa_motion_publish_stop(ctx,0x8c616b64u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616b66; /* fmov.s @r4+,fr0 */
L8c616b66: sa_motion_publish_stop(ctx,0x8c616b66u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616b68; /* fmov.s @r4+,fr2 */
L8c616b68: sa_motion_publish_stop(ctx,0x8c616b68u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c616b6a; /* fldi1 fr3 */
L8c616b6a: sa_motion_publish_stop(ctx,0x8c616b6au); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c616b6c; /* ftrv xmtrx,fv0 */
L8c616b6c: sa_motion_publish_stop(ctx,0x8c616b6cu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616b6e; /* fmov.s @r4+,fr4 */
L8c616b6e: sa_motion_publish_stop(ctx,0x8c616b6eu); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616b70; /* fmov.s @r4,fr6 */
L8c616b70: sa_motion_publish_stop(ctx,0x8c616b70u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616b72; /* fschg */
L8c616b72: sa_motion_publish_stop(ctx,0x8c616b72u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616b74; /* fmov.s fr6,@-r8 */
L8c616b74: sa_motion_publish_stop(ctx,0x8c616b74u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616b76; /* fmov.s fr5,@-r8 */
L8c616b76: sa_motion_publish_stop(ctx,0x8c616b76u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616b78; /* fmov.s fr4,@-r8 */
L8c616b78: sa_motion_publish_stop(ctx,0x8c616b78u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616b7a; /* mov.l r5,@-r8 */
L8c616b7a: sa_motion_publish_stop(ctx,0x8c616b7au); r[8]+=28u;goto L8c616b7c; /* add #0x1c,r8 */
L8c616b7c: sa_motion_publish_stop(ctx,0x8c616b7cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c616b7e; /* fldi1 fr2 */
L8c616b7e: sa_motion_publish_stop(ctx,0x8c616b7eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616b80; /* fmov.s fr2,@-r8 */
L8c616b80: sa_motion_publish_stop(ctx,0x8c616b80u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616b82; /* fmov.s fr1,@-r8 */
L8c616b82: sa_motion_publish_stop(ctx,0x8c616b82u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616b84; /* fmov.s fr0,@-r8 */
L8c616b84: sa_motion_publish_stop(ctx,0x8c616b84u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616b86; /* pref @r8 */
L8c616b86: sa_motion_publish_stop(ctx,0x8c616b86u); r[8]+=16u;goto L8c616b88; /* add #0x10,r8 */
L8c616b88: sa_motion_publish_stop(ctx,0x8c616b88u); r[3]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c616b8a; /* mov.l @(0x18,r15),r3 */
L8c616b8a: sa_motion_publish_stop(ctx,0x8c616b8au); r[3]+=4294967295u;goto L8c616b8c; /* add #-0x1,r3 */
L8c616b8c: sa_motion_publish_stop(ctx,0x8c616b8cu); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c616b8e; /* tst r3,r3 */
L8c616b8e: sa_motion_publish_stop(ctx,0x8c616b90u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;if(take)goto L8c616b96;goto L8c616b92; /* bt/s 0x8c616b96 */
L8c616b92: sa_motion_publish_stop(ctx,0x8c616b94u); goto L8c6168a8; /* bra 0x8c6168a8 */
L8c616b96: sa_motion_publish_stop(ctx,0x8c616b96u); r[4]=rd(m,bus,&io_status,0x8c616c0cu,4);if(io_status)return io_status;goto L8c616b98; /* mov.l 0x8c616c0c,r4 */
L8c616b98: sa_motion_publish_stop(ctx,0x8c616b98u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616b9a; /* mov.l @(0xc,r15),r3 */
L8c616b9a: sa_motion_publish_stop(ctx,0x8c616b9au); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616b9c; /* mov.l @r4,r2 */
L8c616b9c: sa_motion_publish_stop(ctx,0x8c616b9cu); r[2]+=r[3];goto L8c616b9e; /* add r3,r2 */
L8c616b9e: sa_motion_publish_stop(ctx,0x8c616b9eu); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c616ba0; /* mov.l r2,@r4 */
L8c616ba0: sa_motion_publish_stop(ctx,0x8c616ba0u); r[4]=rd(m,bus,&io_status,0x8c616c10u,4);if(io_status)return io_status;goto L8c616ba2; /* mov.l 0x8c616c10,r4 */
L8c616ba2: sa_motion_publish_stop(ctx,0x8c616ba2u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c616ba4; /* mov.l @(0xc,r15),r3 */
L8c616ba4: sa_motion_publish_stop(ctx,0x8c616ba4u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616ba6; /* mov.l @r4,r2 */
L8c616ba6: sa_motion_publish_stop(ctx,0x8c616ba6u); r[2]+=r[3];goto L8c616ba8; /* add r3,r2 */
L8c616ba8: sa_motion_publish_stop(ctx,0x8c616ba8u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c616baa; /* mov.l r2,@r4 */
L8c616baa: sa_motion_publish_stop(ctx,0x8c616baau); r[15]+=76u;goto L8c616bac; /* add #0x4c,r15 */
L8c616bac: sa_motion_publish_stop(ctx,0x8c616bacu); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616bae; /* lds.l @r15+,PR */
L8c616bae: sa_motion_publish_stop(ctx,0x8c616baeu); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616bb0; /* fmov.s @r15+,fr12 */
L8c616bb0: sa_motion_publish_stop(ctx,0x8c616bb0u); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616bb2; /* fmov.s @r15+,fr13 */
L8c616bb2: sa_motion_publish_stop(ctx,0x8c616bb2u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616bb4; /* fmov.s @r15+,fr14 */
L8c616bb4: sa_motion_publish_stop(ctx,0x8c616bb4u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616bb6; /* fmov.s @r15+,fr15 */
L8c616bb6: sa_motion_publish_stop(ctx,0x8c616bb6u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616bb8; /* mov.l @r15+,r9 */
L8c616bb8: sa_motion_publish_stop(ctx,0x8c616bb8u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616bba; /* mov.l @r15+,r10 */
L8c616bba: sa_motion_publish_stop(ctx,0x8c616bbau); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616bbc; /* mov.l @r15+,r11 */
L8c616bbc: sa_motion_publish_stop(ctx,0x8c616bbcu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616bbe; /* mov.l @r15+,r12 */
L8c616bbe: sa_motion_publish_stop(ctx,0x8c616bbeu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616bc0; /* mov.l @r15+,r13 */
L8c616bc0: sa_motion_publish_stop(ctx,0x8c616bc2u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
