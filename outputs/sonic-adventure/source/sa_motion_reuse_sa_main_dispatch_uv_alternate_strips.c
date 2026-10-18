#include "sa_draw_motion_stop.h"
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
L8c61394a: sa_motion_publish_stop(ctx,0x8c61394au); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61394c; /* mov.l r14,@-r15 */
L8c61394c: sa_motion_publish_stop(ctx,0x8c61394cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61394e; /* mov.l r13,@-r15 */
L8c61394e: sa_motion_publish_stop(ctx,0x8c61394eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c613950; /* mov.l r12,@-r15 */
L8c613950: sa_motion_publish_stop(ctx,0x8c613950u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c613952; /* mov.l r11,@-r15 */
L8c613952: sa_motion_publish_stop(ctx,0x8c613952u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c613954; /* mov.l r10,@-r15 */
L8c613954: sa_motion_publish_stop(ctx,0x8c613954u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c613956; /* mov.l r9,@-r15 */
L8c613956: sa_motion_publish_stop(ctx,0x8c613956u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c613958; /* fmov.s fr15,@-r15 */
L8c613958: sa_motion_publish_stop(ctx,0x8c613958u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61395a; /* fmov.s fr14,@-r15 */
L8c61395a: sa_motion_publish_stop(ctx,0x8c61395au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61395c; /* fmov.s fr13,@-r15 */
L8c61395c: sa_motion_publish_stop(ctx,0x8c61395cu); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61395e; /* sts.l PR,@-r15 */
L8c61395e: sa_motion_publish_stop(ctx,0x8c61395eu); r[15]+=4294967276u;goto L8c613960; /* add #-0x14,r15 */
L8c613960: sa_motion_publish_stop(ctx,0x8c613960u); r[3]=rd(m,bus,&io_status,0x8c613994u,4);if(io_status)return io_status;goto L8c613962; /* mov.l 0x8c613994,r3 */
L8c613962: sa_motion_publish_stop(ctx,0x8c613962u); r[9]=rd(m,bus,&io_status,0x8c613990u,4);if(io_status)return io_status;goto L8c613964; /* mov.l 0x8c613990,r9 */
L8c613964: sa_motion_publish_stop(ctx,0x8c613966u); target=r[3];*pr=0x8c613968u;r[12]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c613968; /* jsr @r3 */
L8c613968: sa_motion_publish_stop(ctx,0x8c613968u); r[2]=rd(m,bus,&io_status,0x8c613998u,4);if(io_status)return io_status;goto L8c61396a; /* mov.l 0x8c613998,r2 */
L8c61396a: sa_motion_publish_stop(ctx,0x8c61396au); r[1]=0u;goto L8c61396c; /* mov #0x0,r1 */
L8c61396c: sa_motion_publish_stop(ctx,0x8c61396cu); r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c61396e; /* mov.w @(0x2,r12),r0 */
L8c61396e: sa_motion_publish_stop(ctx,0x8c61396eu); r[14]=rd(m,bus,&io_status,r[12]+0x14u,4);if(io_status)return io_status;goto L8c613970; /* mov.l @(0x14,r12),r14 */
L8c613970: sa_motion_publish_stop(ctx,0x8c613970u); r[13]=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;goto L8c613972; /* mov.l @(0x4,r12),r13 */
L8c613972: sa_motion_publish_stop(ctx,0x8c613972u); r[0]=r[0]&65535u;goto L8c613974; /* extu.w r0,r0 */
L8c613974: sa_motion_publish_stop(ctx,0x8c613974u); wr(m,bus,&io_status,r[15]+0x10u,r[0],4);if(io_status)return io_status;goto L8c613976; /* mov.l r0,@(0x10,r15) */
L8c613976: sa_motion_publish_stop(ctx,0x8c613976u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c613978; /* mov.l @r2,r3 */
L8c613978: sa_motion_publish_stop(ctx,0x8c613978u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61397a; /* mov.l r3,@(0x4,r15) */
L8c61397a: sa_motion_publish_stop(ctx,0x8c61397au); wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c61397c; /* mov.l r1,@(0x8,r15) */
L8c61397c: sa_motion_publish_stop(ctx,0x8c61397cu); wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c61397e; /* mov.l r1,@(0xc,r15) */
L8c61397e: sa_motion_publish_stop(ctx,0x8c613980u); goto L8c61399c; /* bra 0x8c61399c */
L8c61399c: sa_motion_publish_stop(ctx,0x8c61399cu); r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61399e; /* mov.w @r13+,r3 */
L8c61399e: sa_motion_publish_stop(ctx,0x8c61399eu); r[4]=rd(m,bus,&io_status,0x8c613a90u,4);if(io_status)return io_status;goto L8c6139a0; /* mov.l 0x8c613a90,r4 */
L8c6139a0: sa_motion_publish_stop(ctx,0x8c6139a0u); r[3]=r[3]&65535u;goto L8c6139a2; /* extu.w r3,r3 */
L8c6139a2: sa_motion_publish_stop(ctx,0x8c6139a2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6139a4; /* fldi1 fr4 */
L8c6139a4: sa_motion_publish_stop(ctx,0x8c6139a4u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6139a6; /* tst r4,r3 */
L8c6139a6: sa_motion_publish_stop(ctx,0x8c6139a6u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6139a8; /* mov.l r3,@r15 */
L8c6139a8: sa_motion_publish_stop(ctx,0x8c6139aau); take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c6139ba;goto L8c6139ac; /* bt/s 0x8c6139ba */
L8c6139ac: sa_motion_publish_stop(ctx,0x8c6139acu); r[4]=sx(m,bus,&io_status,0x8c613a8eu,2);if(io_status)return io_status;goto L8c6139ae; /* mov.w 0x8c613a8e,r4 */
L8c6139ae: sa_motion_publish_stop(ctx,0x8c6139aeu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6139b0; /* mov.l @r15,r3 */
L8c6139b0: sa_motion_publish_stop(ctx,0x8c6139b0u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6139b2; /* fldi1 fr4 */
L8c6139b2: sa_motion_publish_stop(ctx,0x8c6139b2u); r[3]&=r[4];goto L8c6139b4; /* and r4,r3 */
L8c6139b4: sa_motion_publish_stop(ctx,0x8c6139b4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c6139b6; /* fneg fr4 */
L8c6139b6: sa_motion_publish_stop(ctx,0x8c6139b6u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6139b8; /* mov.l r3,@r15 */
L8c6139b8: sa_motion_publish_stop(ctx,0x8c6139b8u); fcopy(f,15u,4u);goto L8c6139ba; /* fmov fr4,fr15 */
L8c6139ba: sa_motion_publish_stop(ctx,0x8c6139bau); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6139bc; /* mov.l @r15,r3 */
L8c6139bc: sa_motion_publish_stop(ctx,0x8c6139bcu); r[0]=16u;goto L8c6139be; /* mov #0x10,r0 */
L8c6139be: sa_motion_publish_stop(ctx,0x8c6139beu); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6139c0; /* mov.l @(0xc,r15),r2 */
L8c6139c0: sa_motion_publish_stop(ctx,0x8c6139c0u); r[10]=r[8];goto L8c6139c2; /* mov r8,r10 */
L8c6139c2: sa_motion_publish_stop(ctx,0x8c6139c2u); r[2]+=r[3];goto L8c6139c4; /* add r3,r2 */
L8c6139c4: sa_motion_publish_stop(ctx,0x8c6139c4u); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6139c6; /* mov.l r2,@(0xc,r15) */
L8c6139c6: sa_motion_publish_stop(ctx,0x8c6139c6u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6139c8; /* mov.l @r15,r1 */
L8c6139c8: sa_motion_publish_stop(ctx,0x8c6139c8u); r[1]+=4294967294u;goto L8c6139ca; /* add #-0x2,r1 */
L8c6139ca: sa_motion_publish_stop(ctx,0x8c6139cau); wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c6139cc; /* mov.l r1,@r15 */
L8c6139cc: sa_motion_publish_stop(ctx,0x8c6139ccu); r[4]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6139ce; /* mov.w @r13+,r4 */
L8c6139ce: sa_motion_publish_stop(ctx,0x8c6139ceu); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6139d0; /* mov.l @(0x4,r15),r3 */
L8c6139d0: sa_motion_publish_stop(ctx,0x8c6139d0u); r[11]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6139d2; /* mov.w @r13+,r11 */
L8c6139d2: sa_motion_publish_stop(ctx,0x8c6139d2u); r[4]=r[4]&65535u;goto L8c6139d4; /* extu.w r4,r4 */
L8c6139d4: sa_motion_publish_stop(ctx,0x8c6139d4u); r[4]<<=2;goto L8c6139d6; /* shll2 r4 */
L8c6139d6: sa_motion_publish_stop(ctx,0x8c6139d6u); r[11]=r[11]&65535u;goto L8c6139d8; /* extu.w r11,r11 */
L8c6139d8: sa_motion_publish_stop(ctx,0x8c6139d8u); r[11]<<=2;goto L8c6139da; /* shll2 r11 */
L8c6139da: sa_motion_publish_stop(ctx,0x8c6139dau); r[4]<<=2;goto L8c6139dc; /* shll2 r4 */
L8c6139dc: sa_motion_publish_stop(ctx,0x8c6139dcu); r[11]<<=2;goto L8c6139de; /* shll2 r11 */
L8c6139de: sa_motion_publish_stop(ctx,0x8c6139deu); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6139e0; /* shll r4 */
L8c6139e0: sa_motion_publish_stop(ctx,0x8c6139e0u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c6139e2; /* shll r11 */
L8c6139e2: sa_motion_publish_stop(ctx,0x8c6139e2u); r[11]+=r[3];goto L8c6139e4; /* add r3,r11 */
L8c6139e4: sa_motion_publish_stop(ctx,0x8c6139e4u); r[4]+=r[3];goto L8c6139e6; /* add r3,r4 */
L8c6139e6: sa_motion_publish_stop(ctx,0x8c6139e6u); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c6139e8; /* fmov.s @(r0,r11),fr14 */
L8c6139e8: sa_motion_publish_stop(ctx,0x8c6139e8u); fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c6139ea; /* fmov.s @(r0,r4),fr4 */
L8c6139ea: sa_motion_publish_stop(ctx,0x8c6139eau); r[0]=20u;goto L8c6139ec; /* mov #0x14,r0 */
L8c6139ec: sa_motion_publish_stop(ctx,0x8c6139ecu); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c6139ee; /* fmov.s @(r0,r11),fr13 */
L8c6139ee: sa_motion_publish_stop(ctx,0x8c6139eeu); fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c6139f0; /* fmov.s @(r0,r4),fr5 */
L8c6139f0: sa_motion_publish_stop(ctx,0x8c6139f0u); r[12]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6139f2; /* mov.w @r13+,r12 */
L8c6139f2: sa_motion_publish_stop(ctx,0x8c6139f2u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6139f4; /* mov.l @(0x4,r15),r3 */
L8c6139f4: sa_motion_publish_stop(ctx,0x8c6139f4u); r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c6139f6; /* mov.w @r13,r2 */
L8c6139f6: sa_motion_publish_stop(ctx,0x8c6139f6u); r[12]=r[12]&65535u;goto L8c6139f8; /* extu.w r12,r12 */
L8c6139f8: sa_motion_publish_stop(ctx,0x8c6139f8u); r[12]<<=2;goto L8c6139fa; /* shll2 r12 */
L8c6139fa: sa_motion_publish_stop(ctx,0x8c6139fau); r[2]=r[2]&65535u;goto L8c6139fc; /* extu.w r2,r2 */
L8c6139fc: sa_motion_publish_stop(ctx,0x8c6139fcu); r[2]<<=2;goto L8c6139fe; /* shll2 r2 */
L8c6139fe: sa_motion_publish_stop(ctx,0x8c6139feu); r[2]<<=2;goto L8c613a00; /* shll2 r2 */
L8c613a00: sa_motion_publish_stop(ctx,0x8c613a00u); r[12]<<=2;goto L8c613a02; /* shll2 r12 */
L8c613a02: sa_motion_publish_stop(ctx,0x8c613a02u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c613a04; /* shll r12 */
L8c613a04: sa_motion_publish_stop(ctx,0x8c613a04u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c613a06; /* shll r2 */
L8c613a06: sa_motion_publish_stop(ctx,0x8c613a06u); r[2]+=r[3];goto L8c613a08; /* add r3,r2 */
L8c613a08: sa_motion_publish_stop(ctx,0x8c613a08u); r[12]+=r[3];goto L8c613a0a; /* add r3,r12 */
L8c613a0a: sa_motion_publish_stop(ctx,0x8c613a0au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c613a0c; /* pref @r2 */
L8c613a0c: sa_motion_publish_stop(ctx,0x8c613a0cu); r[0]=20u;goto L8c613a0e; /* mov #0x14,r0 */
L8c613a0e: sa_motion_publish_stop(ctx,0x8c613a0eu); fcopy(f,3u,14u);goto L8c613a10; /* fmov fr14,fr3 */
L8c613a10: sa_motion_publish_stop(ctx,0x8c613a10u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c613a12; /* fmov.s @(r0,r12),fr2 */
L8c613a12: sa_motion_publish_stop(ctx,0x8c613a12u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613a14; /* fsub fr4,fr3 */
L8c613a14: sa_motion_publish_stop(ctx,0x8c613a14u); r[0]=16u;goto L8c613a16; /* mov #0x10,r0 */
L8c613a16: sa_motion_publish_stop(ctx,0x8c613a16u); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c613a18; /* cmp/hs r8,r10 */
L8c613a18: sa_motion_publish_stop(ctx,0x8c613a18u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613a1a; /* fsub fr13,fr2 */
L8c613a1a: sa_motion_publish_stop(ctx,0x8c613a1au); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c613a1c; /* fmov.s @(r0,r12),fr1 */
L8c613a1c: sa_motion_publish_stop(ctx,0x8c613a1cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c613a1e; /* fsub fr14,fr1 */
L8c613a1e: sa_motion_publish_stop(ctx,0x8c613a1eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c613a20; /* fmul fr2,fr3 */
L8c613a20: sa_motion_publish_stop(ctx,0x8c613a20u); fcopy(f,2u,13u);goto L8c613a22; /* fmov fr13,fr2 */
L8c613a22: sa_motion_publish_stop(ctx,0x8c613a22u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613a24; /* fsub fr5,fr2 */
L8c613a24: sa_motion_publish_stop(ctx,0x8c613a24u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613a26; /* fmul fr1,fr2 */
L8c613a26: sa_motion_publish_stop(ctx,0x8c613a26u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613a28; /* fsub fr2,fr3 */
L8c613a28: sa_motion_publish_stop(ctx,0x8c613a28u); fcopy(f,4u,3u);goto L8c613a2a; /* fmov fr3,fr4 */
L8c613a2a: sa_motion_publish_stop(ctx,0x8c613a2cu); take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c613a94;goto L8c613a2e; /* bt/s 0x8c613a94 */
L8c613a2e: sa_motion_publish_stop(ctx,0x8c613a2eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613a30; /* fldi0 fr3 */
L8c613a30: sa_motion_publish_stop(ctx,0x8c613a30u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c613a32; /* fcmp/gt fr4,fr3 */
L8c613a32: sa_motion_publish_stop(ctx,0x8c613a32u); if((*sr&1u))goto L8c613ae2;goto L8c613a34; /* bt 0x8c613ae2 */
L8c613a34: sa_motion_publish_stop(ctx,0x8c613a34u); r[6]=r[14];goto L8c613a36; /* mov r14,r6 */
L8c613a36: sa_motion_publish_stop(ctx,0x8c613a36u); r[4]=r[11];goto L8c613a38; /* mov r11,r4 */
L8c613a38: sa_motion_publish_stop(ctx,0x8c613a38u); r[5]=r[9];goto L8c613a3a; /* mov r9,r5 */
L8c613a3a: sa_motion_publish_stop(ctx,0x8c613a3au); r[14]+=4u;goto L8c613a3c; /* add #0x4,r14 */
L8c613a3c: sa_motion_publish_stop(ctx,0x8c613a3cu); r[8]+=16u;goto L8c613a3e; /* add #0x10,r8 */
L8c613a3e: sa_motion_publish_stop(ctx,0x8c613a3eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613a40; /* fschg */
L8c613a40: sa_motion_publish_stop(ctx,0x8c613a40u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613a42; /* fmov.s @r4+,fr0 */
L8c613a42: sa_motion_publish_stop(ctx,0x8c613a42u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613a44; /* fmov.s @r4+,fr2 */
L8c613a44: sa_motion_publish_stop(ctx,0x8c613a44u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613a46; /* fldi0 fr3 */
L8c613a46: sa_motion_publish_stop(ctx,0x8c613a46u); fcopy(f,8u,1u);goto L8c613a48; /* fmov fr1,fr8 */
L8c613a48: sa_motion_publish_stop(ctx,0x8c613a48u); fcopy(f,10u,3u);goto L8c613a4a; /* fmov fr3,fr10 */
L8c613a4a: sa_motion_publish_stop(ctx,0x8c613a4au); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c613a4c; /* fipr fv8,fv0 */
L8c613a4c: sa_motion_publish_stop(ctx,0x8c613a4cu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613a4e; /* fmov.s @r4+,fr4 */
L8c613a4e: sa_motion_publish_stop(ctx,0x8c613a4eu); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613a50; /* fmov.s @r4,fr6 */
L8c613a50: sa_motion_publish_stop(ctx,0x8c613a50u); fcopy(f,8u,5u);goto L8c613a52; /* fmov fr5,fr8 */
L8c613a52: sa_motion_publish_stop(ctx,0x8c613a52u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613a54; /* fschg */
L8c613a54: sa_motion_publish_stop(ctx,0x8c613a54u); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c613a56; /* mov.w @r6+,r2 */
L8c613a56: sa_motion_publish_stop(ctx,0x8c613a56u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c613a58; /* mov.w @r6,r3 */
L8c613a58: sa_motion_publish_stop(ctx,0x8c613a58u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613a5a; /* fmov.s fr6,@-r8 */
L8c613a5a: sa_motion_publish_stop(ctx,0x8c613a5au); *fpul=r[3];goto L8c613a5c; /* lds r3,FPUL */
L8c613a5c: sa_motion_publish_stop(ctx,0x8c613a5cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613a5e; /* fmov.s fr5,@-r8 */
L8c613a5e: sa_motion_publish_stop(ctx,0x8c613a5eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c613a60; /* fsub fr3,fr11 */
L8c613a60: sa_motion_publish_stop(ctx,0x8c613a60u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613a62; /* fmov.s fr4,@-r8 */
L8c613a62: sa_motion_publish_stop(ctx,0x8c613a62u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c613a64; /* float FPUL,fr1 */
L8c613a64: sa_motion_publish_stop(ctx,0x8c613a64u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c613a66; /* fmul fr8,fr1 */
L8c613a66: sa_motion_publish_stop(ctx,0x8c613a66u); *fpul=r[2];goto L8c613a68; /* lds r2,FPUL */
L8c613a68: sa_motion_publish_stop(ctx,0x8c613a68u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613a6a; /* mov.l r5,@-r8 */
L8c613a6a: sa_motion_publish_stop(ctx,0x8c613a6au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c613a6c; /* float FPUL,fr2 */
L8c613a6c: sa_motion_publish_stop(ctx,0x8c613a6cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613a6e; /* fmul fr8,fr2 */
L8c613a6e: sa_motion_publish_stop(ctx,0x8c613a6eu); r[8]+=28u;goto L8c613a70; /* add #0x1c,r8 */
L8c613a70: sa_motion_publish_stop(ctx,0x8c613a70u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c613a72; /* fmov.s fr11,@-r8 */
L8c613a72: sa_motion_publish_stop(ctx,0x8c613a72u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613a74; /* fmov.s fr1,@-r8 */
L8c613a74: sa_motion_publish_stop(ctx,0x8c613a74u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613a76; /* fmov.s fr2,@-r8 */
L8c613a76: sa_motion_publish_stop(ctx,0x8c613a76u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613a78; /* pref @r8 */
L8c613a78: sa_motion_publish_stop(ctx,0x8c613a78u); r[8]+=16u;goto L8c613a7a; /* add #0x10,r8 */
L8c613a7a: sa_motion_publish_stop(ctx,0x8c613a7au); r[3]=r[8];goto L8c613a7c; /* mov r8,r3 */
L8c613a7c: sa_motion_publish_stop(ctx,0x8c613a7cu); r[3]-=r[10];goto L8c613a7e; /* sub r10,r3 */
L8c613a7e: sa_motion_publish_stop(ctx,0x8c613a7eu); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613a80; /* mov.l @(0x8,r15),r2 */
L8c613a80: sa_motion_publish_stop(ctx,0x8c613a80u); r[3]+=32u;goto L8c613a82; /* add #0x20,r3 */
L8c613a82: sa_motion_publish_stop(ctx,0x8c613a82u); r[14]+=4294967292u;goto L8c613a84; /* add #-0x4,r14 */
L8c613a84: sa_motion_publish_stop(ctx,0x8c613a84u); r[2]+=r[3];goto L8c613a86; /* add r3,r2 */
L8c613a86: sa_motion_publish_stop(ctx,0x8c613a86u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613a88; /* mov.l r2,@(0x8,r15) */
L8c613a88: sa_motion_publish_stop(ctx,0x8c613a8au); r[10]=r[8];goto L8c613b28; /* bra 0x8c613b28 */
L8c613a94: sa_motion_publish_stop(ctx,0x8c613a94u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613a96; /* fldi0 fr3 */
L8c613a96: sa_motion_publish_stop(ctx,0x8c613a96u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c613a98; /* fcmp/gt fr4,fr3 */
L8c613a98: sa_motion_publish_stop(ctx,0x8c613a9au); take=!(*sr&1u);r[14]+=4u;if(take)goto L8c613b28;goto L8c613a9c; /* bf/s 0x8c613b28 */
L8c613a9c: sa_motion_publish_stop(ctx,0x8c613a9cu); r[14]+=4294967292u;goto L8c613a9e; /* add #-0x4,r14 */
L8c613a9e: sa_motion_publish_stop(ctx,0x8c613a9eu); r[5]=rd(m,bus,&io_status,0x8c613c1cu,4);if(io_status)return io_status;goto L8c613aa0; /* mov.l 0x8c613c1c,r5 */
L8c613aa0: sa_motion_publish_stop(ctx,0x8c613aa0u); r[6]=r[14];goto L8c613aa2; /* mov r14,r6 */
L8c613aa2: sa_motion_publish_stop(ctx,0x8c613aa2u); r[14]+=4u;goto L8c613aa4; /* add #0x4,r14 */
L8c613aa4: sa_motion_publish_stop(ctx,0x8c613aa4u); r[8]+=16u;goto L8c613aa6; /* add #0x10,r8 */
L8c613aa6: sa_motion_publish_stop(ctx,0x8c613aa6u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613aa8; /* fschg */
L8c613aa8: sa_motion_publish_stop(ctx,0x8c613aa8u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613aaa; /* fmov.s @r4+,fr0 */
L8c613aaa: sa_motion_publish_stop(ctx,0x8c613aaau); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613aac; /* fmov.s @r4+,fr2 */
L8c613aac: sa_motion_publish_stop(ctx,0x8c613aacu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613aae; /* fldi0 fr3 */
L8c613aae: sa_motion_publish_stop(ctx,0x8c613aaeu); fcopy(f,8u,1u);goto L8c613ab0; /* fmov fr1,fr8 */
L8c613ab0: sa_motion_publish_stop(ctx,0x8c613ab0u); fcopy(f,10u,3u);goto L8c613ab2; /* fmov fr3,fr10 */
L8c613ab2: sa_motion_publish_stop(ctx,0x8c613ab2u); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c613ab4; /* fipr fv8,fv0 */
L8c613ab4: sa_motion_publish_stop(ctx,0x8c613ab4u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613ab6; /* fmov.s @r4+,fr4 */
L8c613ab6: sa_motion_publish_stop(ctx,0x8c613ab6u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613ab8; /* fmov.s @r4,fr6 */
L8c613ab8: sa_motion_publish_stop(ctx,0x8c613ab8u); fcopy(f,8u,5u);goto L8c613aba; /* fmov fr5,fr8 */
L8c613aba: sa_motion_publish_stop(ctx,0x8c613abau); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613abc; /* fschg */
L8c613abc: sa_motion_publish_stop(ctx,0x8c613abcu); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c613abe; /* mov.w @r6+,r2 */
L8c613abe: sa_motion_publish_stop(ctx,0x8c613abeu); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c613ac0; /* mov.w @r6,r3 */
L8c613ac0: sa_motion_publish_stop(ctx,0x8c613ac0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613ac2; /* fmov.s fr6,@-r8 */
L8c613ac2: sa_motion_publish_stop(ctx,0x8c613ac2u); *fpul=r[3];goto L8c613ac4; /* lds r3,FPUL */
L8c613ac4: sa_motion_publish_stop(ctx,0x8c613ac4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613ac6; /* fmov.s fr5,@-r8 */
L8c613ac6: sa_motion_publish_stop(ctx,0x8c613ac6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c613ac8; /* fsub fr3,fr11 */
L8c613ac8: sa_motion_publish_stop(ctx,0x8c613ac8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613aca; /* fmov.s fr4,@-r8 */
L8c613aca: sa_motion_publish_stop(ctx,0x8c613acau); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c613acc; /* float FPUL,fr1 */
L8c613acc: sa_motion_publish_stop(ctx,0x8c613accu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c613ace; /* fmul fr8,fr1 */
L8c613ace: sa_motion_publish_stop(ctx,0x8c613aceu); *fpul=r[2];goto L8c613ad0; /* lds r2,FPUL */
L8c613ad0: sa_motion_publish_stop(ctx,0x8c613ad0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613ad2; /* mov.l r5,@-r8 */
L8c613ad2: sa_motion_publish_stop(ctx,0x8c613ad2u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c613ad4; /* float FPUL,fr2 */
L8c613ad4: sa_motion_publish_stop(ctx,0x8c613ad4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613ad6; /* fmul fr8,fr2 */
L8c613ad6: sa_motion_publish_stop(ctx,0x8c613ad6u); r[8]+=28u;goto L8c613ad8; /* add #0x1c,r8 */
L8c613ad8: sa_motion_publish_stop(ctx,0x8c613ad8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c613ada; /* fmov.s fr11,@-r8 */
L8c613ada: sa_motion_publish_stop(ctx,0x8c613adau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613adc; /* fmov.s fr1,@-r8 */
L8c613adc: sa_motion_publish_stop(ctx,0x8c613adcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613ade; /* fmov.s fr2,@-r8 */
L8c613ade: sa_motion_publish_stop(ctx,0x8c613adeu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613ae0; /* pref @r8 */
L8c613ae0: sa_motion_publish_stop(ctx,0x8c613ae0u); r[8]+=16u;goto L8c613ae2; /* add #0x10,r8 */
L8c613ae2: sa_motion_publish_stop(ctx,0x8c613ae2u); r[5]=rd(m,bus,&io_status,0x8c613c1cu,4);if(io_status)return io_status;goto L8c613ae4; /* mov.l 0x8c613c1c,r5 */
L8c613ae4: sa_motion_publish_stop(ctx,0x8c613ae4u); r[6]=r[14];goto L8c613ae6; /* mov r14,r6 */
L8c613ae6: sa_motion_publish_stop(ctx,0x8c613ae6u); r[4]=r[11];goto L8c613ae8; /* mov r11,r4 */
L8c613ae8: sa_motion_publish_stop(ctx,0x8c613ae8u); r[14]+=4u;goto L8c613aea; /* add #0x4,r14 */
L8c613aea: sa_motion_publish_stop(ctx,0x8c613aeau); r[8]+=16u;goto L8c613aec; /* add #0x10,r8 */
L8c613aec: sa_motion_publish_stop(ctx,0x8c613aecu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613aee; /* fschg */
L8c613aee: sa_motion_publish_stop(ctx,0x8c613aeeu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613af0; /* fmov.s @r4+,fr0 */
L8c613af0: sa_motion_publish_stop(ctx,0x8c613af0u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613af2; /* fmov.s @r4+,fr2 */
L8c613af2: sa_motion_publish_stop(ctx,0x8c613af2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613af4; /* fldi0 fr3 */
L8c613af4: sa_motion_publish_stop(ctx,0x8c613af4u); fcopy(f,8u,1u);goto L8c613af6; /* fmov fr1,fr8 */
L8c613af6: sa_motion_publish_stop(ctx,0x8c613af6u); fcopy(f,10u,3u);goto L8c613af8; /* fmov fr3,fr10 */
L8c613af8: sa_motion_publish_stop(ctx,0x8c613af8u); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c613afa; /* fipr fv8,fv0 */
L8c613afa: sa_motion_publish_stop(ctx,0x8c613afau); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613afc; /* fmov.s @r4+,fr4 */
L8c613afc: sa_motion_publish_stop(ctx,0x8c613afcu); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613afe; /* fmov.s @r4,fr6 */
L8c613afe: sa_motion_publish_stop(ctx,0x8c613afeu); fcopy(f,8u,5u);goto L8c613b00; /* fmov fr5,fr8 */
L8c613b00: sa_motion_publish_stop(ctx,0x8c613b00u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613b02; /* fschg */
L8c613b02: sa_motion_publish_stop(ctx,0x8c613b02u); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c613b04; /* mov.w @r6+,r2 */
L8c613b04: sa_motion_publish_stop(ctx,0x8c613b04u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c613b06; /* mov.w @r6,r3 */
L8c613b06: sa_motion_publish_stop(ctx,0x8c613b06u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613b08; /* fmov.s fr6,@-r8 */
L8c613b08: sa_motion_publish_stop(ctx,0x8c613b08u); *fpul=r[3];goto L8c613b0a; /* lds r3,FPUL */
L8c613b0a: sa_motion_publish_stop(ctx,0x8c613b0au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613b0c; /* fmov.s fr5,@-r8 */
L8c613b0c: sa_motion_publish_stop(ctx,0x8c613b0cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c613b0e; /* fsub fr3,fr11 */
L8c613b0e: sa_motion_publish_stop(ctx,0x8c613b0eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613b10; /* fmov.s fr4,@-r8 */
L8c613b10: sa_motion_publish_stop(ctx,0x8c613b10u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c613b12; /* float FPUL,fr1 */
L8c613b12: sa_motion_publish_stop(ctx,0x8c613b12u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c613b14; /* fmul fr8,fr1 */
L8c613b14: sa_motion_publish_stop(ctx,0x8c613b14u); *fpul=r[2];goto L8c613b16; /* lds r2,FPUL */
L8c613b16: sa_motion_publish_stop(ctx,0x8c613b16u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613b18; /* mov.l r5,@-r8 */
L8c613b18: sa_motion_publish_stop(ctx,0x8c613b18u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c613b1a; /* float FPUL,fr2 */
L8c613b1a: sa_motion_publish_stop(ctx,0x8c613b1au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613b1c; /* fmul fr8,fr2 */
L8c613b1c: sa_motion_publish_stop(ctx,0x8c613b1cu); r[8]+=28u;goto L8c613b1e; /* add #0x1c,r8 */
L8c613b1e: sa_motion_publish_stop(ctx,0x8c613b1eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c613b20; /* fmov.s fr11,@-r8 */
L8c613b20: sa_motion_publish_stop(ctx,0x8c613b20u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613b22; /* fmov.s fr1,@-r8 */
L8c613b22: sa_motion_publish_stop(ctx,0x8c613b22u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613b24; /* fmov.s fr2,@-r8 */
L8c613b24: sa_motion_publish_stop(ctx,0x8c613b24u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613b26; /* pref @r8 */
L8c613b26: sa_motion_publish_stop(ctx,0x8c613b26u); r[8]+=16u;goto L8c613b28; /* add #0x10,r8 */
L8c613b28: sa_motion_publish_stop(ctx,0x8c613b28u); fcopy(f,4u,14u);goto L8c613b2a; /* fmov fr14,fr4 */
L8c613b2a: sa_motion_publish_stop(ctx,0x8c613b2au); r[0]=16u;goto L8c613b2c; /* mov #0x10,r0 */
L8c613b2c: sa_motion_publish_stop(ctx,0x8c613b2cu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613b2e; /* mov.l @r15,r3 */
L8c613b2e: sa_motion_publish_stop(ctx,0x8c613b2eu); r[4]=r[11];goto L8c613b30; /* mov r11,r4 */
L8c613b30: sa_motion_publish_stop(ctx,0x8c613b30u); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c613b32; /* fmov.s @(r0,r12),fr14 */
L8c613b32: sa_motion_publish_stop(ctx,0x8c613b32u); r[0]=20u;goto L8c613b34; /* mov #0x14,r0 */
L8c613b34: sa_motion_publish_stop(ctx,0x8c613b34u); fcopy(f,5u,13u);goto L8c613b36; /* fmov fr13,fr5 */
L8c613b36: sa_motion_publish_stop(ctx,0x8c613b36u); r[3]+=4294967295u;goto L8c613b38; /* add #-0x1,r3 */
L8c613b38: sa_motion_publish_stop(ctx,0x8c613b38u); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c613b3a; /* fmov.s @(r0,r12),fr13 */
L8c613b3a: sa_motion_publish_stop(ctx,0x8c613b3au); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c613b3c; /* tst r3,r3 */
L8c613b3c: sa_motion_publish_stop(ctx,0x8c613b3cu); r[11]=r[12];goto L8c613b3e; /* mov r12,r11 */
L8c613b3e: sa_motion_publish_stop(ctx,0x8c613b3eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c613b40; /* fneg fr15 */
L8c613b40: sa_motion_publish_stop(ctx,0x8c613b42u); take=(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c613b48;goto L8c613b44; /* bt/s 0x8c613b48 */
L8c613b44: sa_motion_publish_stop(ctx,0x8c613b46u); goto L8c6139f0; /* bra 0x8c6139f0 */
L8c613b48: sa_motion_publish_stop(ctx,0x8c613b48u); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c613b4a; /* cmp/hs r8,r10 */
L8c613b4a: sa_motion_publish_stop(ctx,0x8c613b4cu); take=(*sr&1u);r[14]+=8u;if(take)goto L8c613ba2;goto L8c613b4e; /* bt/s 0x8c613ba2 */
L8c613b4e: sa_motion_publish_stop(ctx,0x8c613b4eu); r[14]+=4294967288u;goto L8c613b50; /* add #-0x8,r14 */
L8c613b50: sa_motion_publish_stop(ctx,0x8c613b50u); r[4]=r[12];goto L8c613b52; /* mov r12,r4 */
L8c613b52: sa_motion_publish_stop(ctx,0x8c613b52u); r[6]=r[14];goto L8c613b54; /* mov r14,r6 */
L8c613b54: sa_motion_publish_stop(ctx,0x8c613b54u); r[5]=r[9];goto L8c613b56; /* mov r9,r5 */
L8c613b56: sa_motion_publish_stop(ctx,0x8c613b56u); r[14]+=4u;goto L8c613b58; /* add #0x4,r14 */
L8c613b58: sa_motion_publish_stop(ctx,0x8c613b58u); r[8]+=16u;goto L8c613b5a; /* add #0x10,r8 */
L8c613b5a: sa_motion_publish_stop(ctx,0x8c613b5au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613b5c; /* fschg */
L8c613b5c: sa_motion_publish_stop(ctx,0x8c613b5cu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613b5e; /* fmov.s @r4+,fr0 */
L8c613b5e: sa_motion_publish_stop(ctx,0x8c613b5eu); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613b60; /* fmov.s @r4+,fr2 */
L8c613b60: sa_motion_publish_stop(ctx,0x8c613b60u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613b62; /* fldi0 fr3 */
L8c613b62: sa_motion_publish_stop(ctx,0x8c613b62u); fcopy(f,8u,1u);goto L8c613b64; /* fmov fr1,fr8 */
L8c613b64: sa_motion_publish_stop(ctx,0x8c613b64u); fcopy(f,10u,3u);goto L8c613b66; /* fmov fr3,fr10 */
L8c613b66: sa_motion_publish_stop(ctx,0x8c613b66u); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c613b68; /* fipr fv8,fv0 */
L8c613b68: sa_motion_publish_stop(ctx,0x8c613b68u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613b6a; /* fmov.s @r4+,fr4 */
L8c613b6a: sa_motion_publish_stop(ctx,0x8c613b6au); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c613b6c; /* fmov.s @r4,fr6 */
L8c613b6c: sa_motion_publish_stop(ctx,0x8c613b6cu); fcopy(f,8u,5u);goto L8c613b6e; /* fmov fr5,fr8 */
L8c613b6e: sa_motion_publish_stop(ctx,0x8c613b6eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c613b70; /* fschg */
L8c613b70: sa_motion_publish_stop(ctx,0x8c613b70u); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c613b72; /* mov.w @r6+,r2 */
L8c613b72: sa_motion_publish_stop(ctx,0x8c613b72u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c613b74; /* mov.w @r6,r3 */
L8c613b74: sa_motion_publish_stop(ctx,0x8c613b74u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c613b76; /* fmov.s fr6,@-r8 */
L8c613b76: sa_motion_publish_stop(ctx,0x8c613b76u); *fpul=r[3];goto L8c613b78; /* lds r3,FPUL */
L8c613b78: sa_motion_publish_stop(ctx,0x8c613b78u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c613b7a; /* fmov.s fr5,@-r8 */
L8c613b7a: sa_motion_publish_stop(ctx,0x8c613b7au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c613b7c; /* fsub fr3,fr11 */
L8c613b7c: sa_motion_publish_stop(ctx,0x8c613b7cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c613b7e; /* fmov.s fr4,@-r8 */
L8c613b7e: sa_motion_publish_stop(ctx,0x8c613b7eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c613b80; /* float FPUL,fr1 */
L8c613b80: sa_motion_publish_stop(ctx,0x8c613b80u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c613b82; /* fmul fr8,fr1 */
L8c613b82: sa_motion_publish_stop(ctx,0x8c613b82u); *fpul=r[2];goto L8c613b84; /* lds r2,FPUL */
L8c613b84: sa_motion_publish_stop(ctx,0x8c613b84u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c613b86; /* mov.l r5,@-r8 */
L8c613b86: sa_motion_publish_stop(ctx,0x8c613b86u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c613b88; /* float FPUL,fr2 */
L8c613b88: sa_motion_publish_stop(ctx,0x8c613b88u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613b8a; /* fmul fr8,fr2 */
L8c613b8a: sa_motion_publish_stop(ctx,0x8c613b8au); r[8]+=28u;goto L8c613b8c; /* add #0x1c,r8 */
L8c613b8c: sa_motion_publish_stop(ctx,0x8c613b8cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c613b8e; /* fmov.s fr11,@-r8 */
L8c613b8e: sa_motion_publish_stop(ctx,0x8c613b8eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c613b90; /* fmov.s fr1,@-r8 */
L8c613b90: sa_motion_publish_stop(ctx,0x8c613b90u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c613b92; /* fmov.s fr2,@-r8 */
L8c613b92: sa_motion_publish_stop(ctx,0x8c613b92u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613b94; /* pref @r8 */
L8c613b94: sa_motion_publish_stop(ctx,0x8c613b94u); r[8]+=16u;goto L8c613b96; /* add #0x10,r8 */
L8c613b96: sa_motion_publish_stop(ctx,0x8c613b96u); r[3]=r[8];goto L8c613b98; /* mov r8,r3 */
L8c613b98: sa_motion_publish_stop(ctx,0x8c613b98u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613b9a; /* mov.l @(0x8,r15),r2 */
L8c613b9a: sa_motion_publish_stop(ctx,0x8c613b9au); r[3]-=r[10];goto L8c613b9c; /* sub r10,r3 */
L8c613b9c: sa_motion_publish_stop(ctx,0x8c613b9cu); r[3]+=32u;goto L8c613b9e; /* add #0x20,r3 */
L8c613b9e: sa_motion_publish_stop(ctx,0x8c613b9eu); r[2]+=r[3];goto L8c613ba0; /* add r3,r2 */
L8c613ba0: sa_motion_publish_stop(ctx,0x8c613ba0u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c613ba2; /* mov.l r2,@(0x8,r15) */
L8c613ba2: sa_motion_publish_stop(ctx,0x8c613ba2u); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c613ba4; /* mov.l @(0x10,r15),r3 */
L8c613ba4: sa_motion_publish_stop(ctx,0x8c613ba4u); r[3]+=4294967295u;goto L8c613ba6; /* add #-0x1,r3 */
L8c613ba6: sa_motion_publish_stop(ctx,0x8c613ba6u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c613ba8; /* tst r3,r3 */
L8c613ba8: sa_motion_publish_stop(ctx,0x8c613baau); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;if(take)goto L8c613bb0;goto L8c613bac; /* bt/s 0x8c613bb0 */
L8c613bac: sa_motion_publish_stop(ctx,0x8c613baeu); goto L8c61399c; /* bra 0x8c61399c */
L8c613bb0: sa_motion_publish_stop(ctx,0x8c613bb0u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c613bb2; /* mov.l @(0x8,r15),r3 */
L8c613bb2: sa_motion_publish_stop(ctx,0x8c613bb2u); r[4]=rd(m,bus,&io_status,0x8c613c20u,4);if(io_status)return io_status;goto L8c613bb4; /* mov.l 0x8c613c20,r4 */
L8c613bb4: sa_motion_publish_stop(ctx,0x8c613bb4u); r[3]>>=2;goto L8c613bb6; /* shlr2 r3 */
L8c613bb6: sa_motion_publish_stop(ctx,0x8c613bb6u); r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c613bb8; /* mov.l @r4,r1 */
L8c613bb8: sa_motion_publish_stop(ctx,0x8c613bb8u); r[3]>>=2;goto L8c613bba; /* shlr2 r3 */
L8c613bba: sa_motion_publish_stop(ctx,0x8c613bbau); *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c613bbc; /* shlr r3 */
L8c613bbc: sa_motion_publish_stop(ctx,0x8c613bbcu); r[1]+=r[3];goto L8c613bbe; /* add r3,r1 */
L8c613bbe: sa_motion_publish_stop(ctx,0x8c613bbeu); wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c613bc0; /* mov.l r1,@r4 */
L8c613bc0: sa_motion_publish_stop(ctx,0x8c613bc0u); r[4]=rd(m,bus,&io_status,0x8c613c24u,4);if(io_status)return io_status;goto L8c613bc2; /* mov.l 0x8c613c24,r4 */
L8c613bc2: sa_motion_publish_stop(ctx,0x8c613bc2u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c613bc4; /* mov.l @(0xc,r15),r3 */
L8c613bc4: sa_motion_publish_stop(ctx,0x8c613bc4u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c613bc6; /* mov.l @r4,r2 */
L8c613bc6: sa_motion_publish_stop(ctx,0x8c613bc6u); r[2]+=r[3];goto L8c613bc8; /* add r3,r2 */
L8c613bc8: sa_motion_publish_stop(ctx,0x8c613bc8u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c613bca; /* mov.l r2,@r4 */
L8c613bca: sa_motion_publish_stop(ctx,0x8c613bcau); r[15]+=20u;goto L8c613bcc; /* add #0x14,r15 */
L8c613bcc: sa_motion_publish_stop(ctx,0x8c613bccu); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613bce; /* lds.l @r15+,PR */
L8c613bce: sa_motion_publish_stop(ctx,0x8c613bceu); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613bd0; /* fmov.s @r15+,fr13 */
L8c613bd0: sa_motion_publish_stop(ctx,0x8c613bd0u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613bd2; /* fmov.s @r15+,fr14 */
L8c613bd2: sa_motion_publish_stop(ctx,0x8c613bd2u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c613bd4; /* fmov.s @r15+,fr15 */
L8c613bd4: sa_motion_publish_stop(ctx,0x8c613bd4u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613bd6; /* mov.l @r15+,r9 */
L8c613bd6: sa_motion_publish_stop(ctx,0x8c613bd6u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613bd8; /* mov.l @r15+,r10 */
L8c613bd8: sa_motion_publish_stop(ctx,0x8c613bd8u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613bda; /* mov.l @r15+,r11 */
L8c613bda: sa_motion_publish_stop(ctx,0x8c613bdau); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613bdc; /* mov.l @r15+,r12 */
L8c613bdc: sa_motion_publish_stop(ctx,0x8c613bdcu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c613bde; /* mov.l @r15+,r13 */
L8c613bde: sa_motion_publish_stop(ctx,0x8c613be0u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_uv_alternate_strip_clipped_8c614f70(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c614f70;
L8c614f70: sa_motion_publish_stop(ctx,0x8c614f70u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c614f72; /* mov.l r14,@-r15 */
L8c614f72: sa_motion_publish_stop(ctx,0x8c614f72u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c614f74; /* mov.l r13,@-r15 */
L8c614f74: sa_motion_publish_stop(ctx,0x8c614f74u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c614f76; /* mov.l r12,@-r15 */
L8c614f76: sa_motion_publish_stop(ctx,0x8c614f76u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c614f78; /* mov.l r11,@-r15 */
L8c614f78: sa_motion_publish_stop(ctx,0x8c614f78u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c614f7a; /* mov.l r10,@-r15 */
L8c614f7a: sa_motion_publish_stop(ctx,0x8c614f7au); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c614f7c; /* mov.l r9,@-r15 */
L8c614f7c: sa_motion_publish_stop(ctx,0x8c614f7cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c614f7e; /* fmov.s fr15,@-r15 */
L8c614f7e: sa_motion_publish_stop(ctx,0x8c614f7eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c614f80; /* fmov.s fr14,@-r15 */
L8c614f80: sa_motion_publish_stop(ctx,0x8c614f80u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c614f82; /* fmov.s fr13,@-r15 */
L8c614f82: sa_motion_publish_stop(ctx,0x8c614f82u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c614f84; /* fmov.s fr12,@-r15 */
L8c614f84: sa_motion_publish_stop(ctx,0x8c614f84u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c614f86; /* sts.l PR,@-r15 */
L8c614f86: sa_motion_publish_stop(ctx,0x8c614f86u); r[15]+=4294967216u;goto L8c614f88; /* add #-0x50,r15 */
L8c614f88: sa_motion_publish_stop(ctx,0x8c614f88u); r[3]=rd(m,bus,&io_status,0x8c614fc0u,4);if(io_status)return io_status;goto L8c614f8a; /* mov.l 0x8c614fc0,r3 */
L8c614f8a: sa_motion_publish_stop(ctx,0x8c614f8au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c614f8c; /* fldi1 fr12 */
L8c614f8c: sa_motion_publish_stop(ctx,0x8c614f8eu); target=r[3];*pr=0x8c614f90u;r[12]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c614f90; /* jsr @r3 */
L8c614f90: sa_motion_publish_stop(ctx,0x8c614f90u); r[2]=rd(m,bus,&io_status,0x8c614fc4u,4);if(io_status)return io_status;goto L8c614f92; /* mov.l 0x8c614fc4,r2 */
L8c614f92: sa_motion_publish_stop(ctx,0x8c614f92u); r[1]=0u;goto L8c614f94; /* mov #0x0,r1 */
L8c614f94: sa_motion_publish_stop(ctx,0x8c614f94u); r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c614f96; /* mov.w @(0x2,r12),r0 */
L8c614f96: sa_motion_publish_stop(ctx,0x8c614f96u); r[13]=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;goto L8c614f98; /* mov.l @(0x4,r12),r13 */
L8c614f98: sa_motion_publish_stop(ctx,0x8c614f98u); r[14]=rd(m,bus,&io_status,r[12]+0x14u,4);if(io_status)return io_status;goto L8c614f9a; /* mov.l @(0x14,r12),r14 */
L8c614f9a: sa_motion_publish_stop(ctx,0x8c614f9au); r[0]=r[0]&65535u;goto L8c614f9c; /* extu.w r0,r0 */
L8c614f9c: sa_motion_publish_stop(ctx,0x8c614f9cu); wr(m,bus,&io_status,r[15]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c614f9e; /* mov.l r0,@(0x1c,r15) */
L8c614f9e: sa_motion_publish_stop(ctx,0x8c614f9eu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c614fa0; /* mov.l @r2,r3 */
L8c614fa0: sa_motion_publish_stop(ctx,0x8c614fa0u); wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c614fa2; /* mov.l r3,@(0xc,r15) */
L8c614fa2: sa_motion_publish_stop(ctx,0x8c614fa2u); wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c614fa4; /* mov.l r1,@(0x10,r15) */
L8c614fa4: sa_motion_publish_stop(ctx,0x8c614fa6u); goto L8c614fc8; /* bra 0x8c614fc8 */
L8c614fc8: sa_motion_publish_stop(ctx,0x8c614fc8u); r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c614fca; /* mov.w @r13+,r3 */
L8c614fca: sa_motion_publish_stop(ctx,0x8c614fcau); r[4]=rd(m,bus,&io_status,0x8c6150e8u,4);if(io_status)return io_status;goto L8c614fcc; /* mov.l 0x8c6150e8,r4 */
L8c614fcc: sa_motion_publish_stop(ctx,0x8c614fccu); r[3]=r[3]&65535u;goto L8c614fce; /* extu.w r3,r3 */
L8c614fce: sa_motion_publish_stop(ctx,0x8c614fceu); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c614fd0; /* tst r4,r3 */
L8c614fd0: sa_motion_publish_stop(ctx,0x8c614fd0u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c614fd2; /* mov.l r3,@(0x8,r15) */
L8c614fd2: sa_motion_publish_stop(ctx,0x8c614fd4u); take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c614fe4;goto L8c614fd6; /* bt/s 0x8c614fe4 */
L8c614fd6: sa_motion_publish_stop(ctx,0x8c614fd6u); r[4]=sx(m,bus,&io_status,0x8c6150e4u,2);if(io_status)return io_status;goto L8c614fd8; /* mov.w 0x8c6150e4,r4 */
L8c614fd8: sa_motion_publish_stop(ctx,0x8c614fd8u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614fda; /* mov.l @(0x8,r15),r3 */
L8c614fda: sa_motion_publish_stop(ctx,0x8c614fdau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c614fdc; /* fldi1 fr4 */
L8c614fdc: sa_motion_publish_stop(ctx,0x8c614fdcu); r[3]&=r[4];goto L8c614fde; /* and r4,r3 */
L8c614fde: sa_motion_publish_stop(ctx,0x8c614fdeu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c614fe0; /* fneg fr4 */
L8c614fe0: sa_motion_publish_stop(ctx,0x8c614fe0u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c614fe2; /* mov.l r3,@(0x8,r15) */
L8c614fe2: sa_motion_publish_stop(ctx,0x8c614fe2u); fcopy(f,15u,4u);goto L8c614fe4; /* fmov fr4,fr15 */
L8c614fe4: sa_motion_publish_stop(ctx,0x8c614fe4u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614fe6; /* mov.l @(0x8,r15),r3 */
L8c614fe6: sa_motion_publish_stop(ctx,0x8c614fe6u); r[0]=16u;goto L8c614fe8; /* mov #0x10,r0 */
L8c614fe8: sa_motion_publish_stop(ctx,0x8c614fe8u); r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c614fea; /* mov.l @(0x10,r15),r2 */
L8c614fea: sa_motion_publish_stop(ctx,0x8c614feau); r[9]=r[8];goto L8c614fec; /* mov r8,r9 */
L8c614fec: sa_motion_publish_stop(ctx,0x8c614fecu); r[2]+=r[3];goto L8c614fee; /* add r3,r2 */
L8c614fee: sa_motion_publish_stop(ctx,0x8c614feeu); wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c614ff0; /* mov.l r2,@(0x10,r15) */
L8c614ff0: sa_motion_publish_stop(ctx,0x8c614ff0u); r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614ff2; /* mov.l @(0x8,r15),r1 */
L8c614ff2: sa_motion_publish_stop(ctx,0x8c614ff2u); r[1]+=4294967294u;goto L8c614ff4; /* add #-0x2,r1 */
L8c614ff4: sa_motion_publish_stop(ctx,0x8c614ff4u); wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c614ff6; /* mov.l r1,@(0x8,r15) */
L8c614ff6: sa_motion_publish_stop(ctx,0x8c614ff6u); r[10]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c614ff8; /* mov.w @r13+,r10 */
L8c614ff8: sa_motion_publish_stop(ctx,0x8c614ff8u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614ffa; /* mov.l @(0xc,r15),r3 */
L8c614ffa: sa_motion_publish_stop(ctx,0x8c614ffau); r[11]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c614ffc; /* mov.w @r13+,r11 */
L8c614ffc: sa_motion_publish_stop(ctx,0x8c614ffcu); r[10]=r[10]&65535u;goto L8c614ffe; /* extu.w r10,r10 */
L8c614ffe: sa_motion_publish_stop(ctx,0x8c614ffeu); r[10]<<=2;goto L8c615000; /* shll2 r10 */
L8c615000: sa_motion_publish_stop(ctx,0x8c615000u); r[11]=r[11]&65535u;goto L8c615002; /* extu.w r11,r11 */
L8c615002: sa_motion_publish_stop(ctx,0x8c615002u); r[11]<<=2;goto L8c615004; /* shll2 r11 */
L8c615004: sa_motion_publish_stop(ctx,0x8c615004u); r[10]<<=2;goto L8c615006; /* shll2 r10 */
L8c615006: sa_motion_publish_stop(ctx,0x8c615006u); r[11]<<=2;goto L8c615008; /* shll2 r11 */
L8c615008: sa_motion_publish_stop(ctx,0x8c615008u); *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c61500a; /* shll r10 */
L8c61500a: sa_motion_publish_stop(ctx,0x8c61500au); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61500c; /* shll r11 */
L8c61500c: sa_motion_publish_stop(ctx,0x8c61500cu); r[11]+=r[3];goto L8c61500e; /* add r3,r11 */
L8c61500e: sa_motion_publish_stop(ctx,0x8c61500eu); r[10]+=r[3];goto L8c615010; /* add r3,r10 */
L8c615010: sa_motion_publish_stop(ctx,0x8c615010u); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c615012; /* fmov.s @(r0,r11),fr13 */
L8c615012: sa_motion_publish_stop(ctx,0x8c615012u); fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c615014; /* fmov.s @(r0,r10),fr5 */
L8c615014: sa_motion_publish_stop(ctx,0x8c615014u); r[0]=20u;goto L8c615016; /* mov #0x14,r0 */
L8c615016: sa_motion_publish_stop(ctx,0x8c615016u); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c615018; /* fmov.s @(r0,r11),fr14 */
L8c615018: sa_motion_publish_stop(ctx,0x8c615018u); fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c61501a; /* fmov.s @(r0,r10),fr6 */
L8c61501a: sa_motion_publish_stop(ctx,0x8c61501au); r[12]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61501c; /* mov.w @r13+,r12 */
L8c61501c: sa_motion_publish_stop(ctx,0x8c61501cu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c61501e; /* mov.l @(0xc,r15),r3 */
L8c61501e: sa_motion_publish_stop(ctx,0x8c61501eu); r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c615020; /* mov.w @r13,r2 */
L8c615020: sa_motion_publish_stop(ctx,0x8c615020u); r[12]=r[12]&65535u;goto L8c615022; /* extu.w r12,r12 */
L8c615022: sa_motion_publish_stop(ctx,0x8c615022u); r[12]<<=2;goto L8c615024; /* shll2 r12 */
L8c615024: sa_motion_publish_stop(ctx,0x8c615024u); r[2]=r[2]&65535u;goto L8c615026; /* extu.w r2,r2 */
L8c615026: sa_motion_publish_stop(ctx,0x8c615026u); r[2]<<=2;goto L8c615028; /* shll2 r2 */
L8c615028: sa_motion_publish_stop(ctx,0x8c615028u); r[2]<<=2;goto L8c61502a; /* shll2 r2 */
L8c61502a: sa_motion_publish_stop(ctx,0x8c61502au); r[12]<<=2;goto L8c61502c; /* shll2 r12 */
L8c61502c: sa_motion_publish_stop(ctx,0x8c61502cu); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61502e; /* shll r12 */
L8c61502e: sa_motion_publish_stop(ctx,0x8c61502eu); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c615030; /* shll r2 */
L8c615030: sa_motion_publish_stop(ctx,0x8c615030u); r[2]+=r[3];goto L8c615032; /* add r3,r2 */
L8c615032: sa_motion_publish_stop(ctx,0x8c615032u); r[12]+=r[3];goto L8c615034; /* add r3,r12 */
L8c615034: sa_motion_publish_stop(ctx,0x8c615034u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c615036; /* pref @r2 */
L8c615036: sa_motion_publish_stop(ctx,0x8c615036u); r[3]=0u;goto L8c615038; /* mov #0x0,r3 */
L8c615038: sa_motion_publish_stop(ctx,0x8c615038u); wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;goto L8c61503a; /* mov.l r3,@(0x18,r15) */
L8c61503a: sa_motion_publish_stop(ctx,0x8c61503au); r[4]=63u;goto L8c61503c; /* mov #0x3f,r4 */
L8c61503c: sa_motion_publish_stop(ctx,0x8c61503cu); r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c61503e; /* mov.l @(0xc,r10),r2 */
L8c61503e: sa_motion_publish_stop(ctx,0x8c61503eu); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c615040; /* mov.l @(0xc,r11),r3 */
L8c615040: sa_motion_publish_stop(ctx,0x8c615040u); r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c615042; /* mov.l @(0xc,r12),r1 */
L8c615042: sa_motion_publish_stop(ctx,0x8c615042u); r[2]&=r[3];goto L8c615044; /* and r3,r2 */
L8c615044: sa_motion_publish_stop(ctx,0x8c615044u); r[2]&=r[1];goto L8c615046; /* and r1,r2 */
L8c615046: sa_motion_publish_stop(ctx,0x8c615046u); r[4]&=r[2];goto L8c615048; /* and r2,r4 */
L8c615048: sa_motion_publish_stop(ctx,0x8c615048u); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c61504a; /* tst r4,r4 */
L8c61504a: sa_motion_publish_stop(ctx,0x8c61504cu); take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c615052;goto L8c61504e; /* bt/s 0x8c615052 */
L8c61504e: sa_motion_publish_stop(ctx,0x8c615050u); goto L8c6151ac; /* bra 0x8c6151ac */
L8c615052: sa_motion_publish_stop(ctx,0x8c615052u); r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c615054; /* mov.l @(0xc,r10),r4 */
L8c615054: sa_motion_publish_stop(ctx,0x8c615054u); r[5]=1u;goto L8c615056; /* mov #0x1,r5 */
L8c615056: sa_motion_publish_stop(ctx,0x8c615056u); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c615058; /* mov.l @(0xc,r11),r3 */
L8c615058: sa_motion_publish_stop(ctx,0x8c615058u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61505a; /* mov.l @(0xc,r12),r2 */
L8c61505a: sa_motion_publish_stop(ctx,0x8c61505au); r[4]|=r[3];goto L8c61505c; /* or r3,r4 */
L8c61505c: sa_motion_publish_stop(ctx,0x8c61505cu); r[4]|=r[2];goto L8c61505e; /* or r2,r4 */
L8c61505e: sa_motion_publish_stop(ctx,0x8c61505eu); r[4]&=r[5];goto L8c615060; /* and r5,r4 */
L8c615060: sa_motion_publish_stop(ctx,0x8c615060u); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c615062; /* tst r4,r4 */
L8c615062: sa_motion_publish_stop(ctx,0x8c615062u); if(!(*sr&1u))goto L8c615068;goto L8c615064; /* bf 0x8c615068 */
L8c615064: sa_motion_publish_stop(ctx,0x8c615066u); goto L8c61518e; /* bra 0x8c61518e */
L8c615068: sa_motion_publish_stop(ctx,0x8c615068u); r[7]=r[15];goto L8c61506a; /* mov r15,r7 */
L8c61506a: sa_motion_publish_stop(ctx,0x8c61506au); r[7]+=68u;goto L8c61506c; /* add #0x44,r7 */
L8c61506c: sa_motion_publish_stop(ctx,0x8c61506cu); wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c61506e; /* mov.l r10,@r7 */
L8c61506e: sa_motion_publish_stop(ctx,0x8c61506eu); wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c615070; /* mov.l r11,@(0x4,r7) */
L8c615070: sa_motion_publish_stop(ctx,0x8c615070u); wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c615072; /* mov.l r12,@(0x8,r7) */
L8c615072: sa_motion_publish_stop(ctx,0x8c615074u); r[6]=0u;goto L8c615120; /* bra 0x8c615120 */
L8c615076: sa_motion_publish_stop(ctx,0x8c615076u); r[2]=r[6];goto L8c615078; /* mov r6,r2 */
L8c615078: sa_motion_publish_stop(ctx,0x8c615078u); r[1]=r[6];goto L8c61507a; /* mov r6,r1 */
L8c61507a: sa_motion_publish_stop(ctx,0x8c61507au); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c61507c; /* shll r2 */
L8c61507c: sa_motion_publish_stop(ctx,0x8c61507cu); r[3]=r[6];goto L8c61507e; /* mov r6,r3 */
L8c61507e: sa_motion_publish_stop(ctx,0x8c61507eu); r[3]<<=2;goto L8c615080; /* shll2 r3 */
L8c615080: sa_motion_publish_stop(ctx,0x8c615080u); r[2]+=r[1];goto L8c615082; /* add r1,r2 */
L8c615082: sa_motion_publish_stop(ctx,0x8c615082u); r[2]<<=2;goto L8c615084; /* shll2 r2 */
L8c615084: sa_motion_publish_stop(ctx,0x8c615084u); r[3]+=r[7];goto L8c615086; /* add r7,r3 */
L8c615086: sa_motion_publish_stop(ctx,0x8c615086u); r[2]+=r[4];goto L8c615088; /* add r4,r2 */
L8c615088: sa_motion_publish_stop(ctx,0x8c615088u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61508a; /* mov.l r3,@(0x4,r15) */
L8c61508a: sa_motion_publish_stop(ctx,0x8c61508au); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61508c; /* mov.l r2,@r15 */
L8c61508c: sa_motion_publish_stop(ctx,0x8c61508cu); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61508e; /* mov.l @r3,r1 */
L8c61508e: sa_motion_publish_stop(ctx,0x8c61508eu); r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c615090; /* mov.l @(0xc,r1),r2 */
L8c615090: sa_motion_publish_stop(ctx,0x8c615090u); *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c615092; /* tst r5,r2 */
L8c615092: sa_motion_publish_stop(ctx,0x8c615094u); take=!(*sr&1u);r[6]+=1u;if(take)goto L8c6150f8;goto L8c615096; /* bf/s 0x8c6150f8 */
L8c615096: sa_motion_publish_stop(ctx,0x8c615096u); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615098; /* mov.l @(0x4,r15),r1 */
L8c615098: sa_motion_publish_stop(ctx,0x8c615098u); r[0]=24u;goto L8c61509a; /* mov #0x18,r0 */
L8c61509a: sa_motion_publish_stop(ctx,0x8c61509au); fcopy(f,4u,12u);goto L8c61509c; /* fmov fr12,fr4 */
L8c61509c: sa_motion_publish_stop(ctx,0x8c61509cu); r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61509e; /* mov.l @r1,r3 */
L8c61509e: sa_motion_publish_stop(ctx,0x8c61509eu); r[4]=rd(m,bus,&io_status,0x8c6150ecu,4);if(io_status)return io_status;goto L8c6150a0; /* mov.l 0x8c6150ec,r4 */
L8c6150a0: sa_motion_publish_stop(ctx,0x8c6150a0u); wr(m,bus,&io_status,r[15]+0x14u,r[3],4);if(io_status)return io_status;goto L8c6150a2; /* mov.l r3,@(0x14,r15) */
L8c6150a2: sa_motion_publish_stop(ctx,0x8c6150a2u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6150a4; /* fmov.s @(r0,r3),fr3 */
L8c6150a4: sa_motion_publish_stop(ctx,0x8c6150a4u); r[0]=12u;goto L8c6150a6; /* mov #0xc,r0 */
L8c6150a6: sa_motion_publish_stop(ctx,0x8c6150a6u); r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c6150a8; /* mov.l @(0x14,r15),r2 */
L8c6150a8: sa_motion_publish_stop(ctx,0x8c6150a8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6150aa; /* fdiv fr3,fr4 */
L8c6150aa: sa_motion_publish_stop(ctx,0x8c6150aau); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6150ac; /* fmov.s @(r0,r4),fr3 */
L8c6150ac: sa_motion_publish_stop(ctx,0x8c6150acu); r[0]=16u;goto L8c6150ae; /* mov #0x10,r0 */
L8c6150ae: sa_motion_publish_stop(ctx,0x8c6150aeu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6150b0; /* mov.l @r15,r3 */
L8c6150b0: sa_motion_publish_stop(ctx,0x8c6150b0u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6150b2; /* fmov.s @(r0,r2),fr2 */
L8c6150b2: sa_motion_publish_stop(ctx,0x8c6150b2u); r[0]=16u;goto L8c6150b4; /* mov #0x10,r0 */
L8c6150b4: sa_motion_publish_stop(ctx,0x8c6150b4u); r[2]=rd(m,bus,&io_status,0x8c6150f0u,4);if(io_status)return io_status;goto L8c6150b6; /* mov.l 0x8c6150f0,r2 */
L8c6150b6: sa_motion_publish_stop(ctx,0x8c6150b6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6150b8; /* fsub fr3,fr2 */
L8c6150b8: sa_motion_publish_stop(ctx,0x8c6150b8u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6150ba; /* fmov.s @r2,fr3 */
L8c6150ba: sa_motion_publish_stop(ctx,0x8c6150bau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6150bc; /* fmul fr4,fr2 */
L8c6150bc: sa_motion_publish_stop(ctx,0x8c6150bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6150be; /* fdiv fr3,fr2 */
L8c6150be: sa_motion_publish_stop(ctx,0x8c6150beu); fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6150c0; /* fmov.s fr2,@r3 */
L8c6150c0: sa_motion_publish_stop(ctx,0x8c6150c0u); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6150c2; /* mov.l @(0x4,r15),r1 */
L8c6150c2: sa_motion_publish_stop(ctx,0x8c6150c2u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6150c4; /* fmov.s @(r0,r4),fr3 */
L8c6150c4: sa_motion_publish_stop(ctx,0x8c6150c4u); r[0]=20u;goto L8c6150c6; /* mov #0x14,r0 */
L8c6150c6: sa_motion_publish_stop(ctx,0x8c6150c6u); r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c6150c8; /* mov.l @r1,r1 */
L8c6150c8: sa_motion_publish_stop(ctx,0x8c6150c8u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6150ca; /* mov.l @r15,r3 */
L8c6150ca: sa_motion_publish_stop(ctx,0x8c6150cau); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c6150cc; /* fmov.s @(r0,r1),fr2 */
L8c6150cc: sa_motion_publish_stop(ctx,0x8c6150ccu); r[1]=rd(m,bus,&io_status,0x8c6150f4u,4);if(io_status)return io_status;goto L8c6150ce; /* mov.l 0x8c6150f4,r1 */
L8c6150ce: sa_motion_publish_stop(ctx,0x8c6150ceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6150d0; /* fsub fr3,fr2 */
L8c6150d0: sa_motion_publish_stop(ctx,0x8c6150d0u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6150d2; /* fmov.s @r1,fr3 */
L8c6150d2: sa_motion_publish_stop(ctx,0x8c6150d2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6150d4; /* fmul fr4,fr2 */
L8c6150d4: sa_motion_publish_stop(ctx,0x8c6150d4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6150d6; /* fdiv fr3,fr2 */
L8c6150d6: sa_motion_publish_stop(ctx,0x8c6150d6u); r[0]=4u;goto L8c6150d8; /* mov #0x4,r0 */
L8c6150d8: sa_motion_publish_stop(ctx,0x8c6150d8u); fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c6150da; /* fmov fr2,@(r0,r3) */
L8c6150da: sa_motion_publish_stop(ctx,0x8c6150dau); r[0]=8u;goto L8c6150dc; /* mov #0x8,r0 */
L8c6150dc: sa_motion_publish_stop(ctx,0x8c6150dcu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6150de; /* mov.l @r15,r3 */
L8c6150de: sa_motion_publish_stop(ctx,0x8c6150deu); fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c6150e0; /* fmov fr4,@(r0,r3) */
L8c6150e0: sa_motion_publish_stop(ctx,0x8c6150e2u); goto L8c615120; /* bra 0x8c615120 */
L8c6150f8: sa_motion_publish_stop(ctx,0x8c6150f8u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6150fa; /* mov.l @(0x4,r15),r3 */
L8c6150fa: sa_motion_publish_stop(ctx,0x8c6150fau); r[0]=16u;goto L8c6150fc; /* mov #0x10,r0 */
L8c6150fc: sa_motion_publish_stop(ctx,0x8c6150fcu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6150fe; /* mov.l @r15,r2 */
L8c6150fe: sa_motion_publish_stop(ctx,0x8c6150feu); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c615100; /* mov.l @r3,r1 */
L8c615100: sa_motion_publish_stop(ctx,0x8c615100u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c615102; /* fmov.s @(r0,r1),fr3 */
L8c615102: sa_motion_publish_stop(ctx,0x8c615102u); r[0]=20u;goto L8c615104; /* mov #0x14,r0 */
L8c615104: sa_motion_publish_stop(ctx,0x8c615104u); fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c615106; /* fmov.s fr3,@r2 */
L8c615106: sa_motion_publish_stop(ctx,0x8c615106u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615108; /* mov.l @(0x4,r15),r2 */
L8c615108: sa_motion_publish_stop(ctx,0x8c615108u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61510a; /* mov.l @r15,r3 */
L8c61510a: sa_motion_publish_stop(ctx,0x8c61510au); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61510c; /* mov.l @r2,r1 */
L8c61510c: sa_motion_publish_stop(ctx,0x8c61510cu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61510e; /* fmov.s @(r0,r1),fr3 */
L8c61510e: sa_motion_publish_stop(ctx,0x8c61510eu); r[0]=4u;goto L8c615110; /* mov #0x4,r0 */
L8c615110: sa_motion_publish_stop(ctx,0x8c615110u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c615112; /* fmov fr3,@(r0,r3) */
L8c615112: sa_motion_publish_stop(ctx,0x8c615112u); r[0]=24u;goto L8c615114; /* mov #0x18,r0 */
L8c615114: sa_motion_publish_stop(ctx,0x8c615114u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615116; /* mov.l @(0x4,r15),r2 */
L8c615116: sa_motion_publish_stop(ctx,0x8c615116u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615118; /* mov.l @r15,r3 */
L8c615118: sa_motion_publish_stop(ctx,0x8c615118u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61511a; /* mov.l @r2,r1 */
L8c61511a: sa_motion_publish_stop(ctx,0x8c61511au); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61511c; /* fmov.s @(r0,r1),fr3 */
L8c61511c: sa_motion_publish_stop(ctx,0x8c61511cu); r[0]=8u;goto L8c61511e; /* mov #0x8,r0 */
L8c61511e: sa_motion_publish_stop(ctx,0x8c61511eu); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c615120; /* fmov fr3,@(r0,r3) */
L8c615120: sa_motion_publish_stop(ctx,0x8c615120u); r[3]=3u;goto L8c615122; /* mov #0x3,r3 */
L8c615122: sa_motion_publish_stop(ctx,0x8c615122u); r[4]=r[15];goto L8c615124; /* mov r15,r4 */
L8c615124: sa_motion_publish_stop(ctx,0x8c615124u); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c615126; /* cmp/ge r3,r6 */
L8c615126: sa_motion_publish_stop(ctx,0x8c615128u); take=!(*sr&1u);r[4]+=32u;if(take)goto L8c615076;goto L8c61512a; /* bf/s 0x8c615076 */
L8c61512a: sa_motion_publish_stop(ctx,0x8c61512au); r[7]=r[4];goto L8c61512c; /* mov r4,r7 */
L8c61512c: sa_motion_publish_stop(ctx,0x8c61512cu); r[0]=8u;goto L8c61512e; /* mov #0x8,r0 */
L8c61512e: sa_motion_publish_stop(ctx,0x8c61512eu); r[3]=r[4];goto L8c615130; /* mov r4,r3 */
L8c615130: sa_motion_publish_stop(ctx,0x8c615130u); fload(m,bus,&io_status,f,8u,r[0]+r[7]);if(io_status)return io_status;goto L8c615132; /* fmov.s @(r0,r7),fr8 */
L8c615132: sa_motion_publish_stop(ctx,0x8c615132u); r[3]+=12u;goto L8c615134; /* add #0xc,r3 */
L8c615134: sa_motion_publish_stop(ctx,0x8c615134u); r[6]=r[4];goto L8c615136; /* mov r4,r6 */
L8c615136: sa_motion_publish_stop(ctx,0x8c615136u); r[0]=4u;goto L8c615138; /* mov #0x4,r0 */
L8c615138: sa_motion_publish_stop(ctx,0x8c615138u); fcopy(f,0u,8u);goto L8c61513a; /* fmov fr8,fr0 */
L8c61513a: sa_motion_publish_stop(ctx,0x8c61513au); r[6]+=24u;goto L8c61513c; /* add #0x18,r6 */
L8c61513c: sa_motion_publish_stop(ctx,0x8c61513cu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61513e; /* mov.l r3,@r15 */
L8c61513e: sa_motion_publish_stop(ctx,0x8c61513eu); fload(m,bus,&io_status,f,9u,r[0]+r[7]);if(io_status)return io_status;goto L8c615140; /* fmov.s @(r0,r7),fr9 */
L8c615140: sa_motion_publish_stop(ctx,0x8c615140u); fload(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c615142; /* fmov.s @(r0,r3),fr4 */
L8c615142: sa_motion_publish_stop(ctx,0x8c615142u); r[0]=8u;goto L8c615144; /* mov #0x8,r0 */
L8c615144: sa_motion_publish_stop(ctx,0x8c615144u); fload(m,bus,&io_status,f,6u,r[0]+r[3]);if(io_status)return io_status;goto L8c615146; /* fmov.s @(r0,r3),fr6 */
L8c615146: sa_motion_publish_stop(ctx,0x8c615146u); r[0]=4u;goto L8c615148; /* mov #0x4,r0 */
L8c615148: sa_motion_publish_stop(ctx,0x8c615148u); fcopy(f,3u,9u);goto L8c61514a; /* fmov fr9,fr3 */
L8c61514a: sa_motion_publish_stop(ctx,0x8c61514au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61514c; /* fmul fr4,fr0 */
L8c61514c: sa_motion_publish_stop(ctx,0x8c61514cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61514e; /* fmul fr6,fr3 */
L8c61514e: sa_motion_publish_stop(ctx,0x8c61514eu); fload(m,bus,&io_status,f,7u,r[7]);if(io_status)return io_status;goto L8c615150; /* fmov.s @r7,fr7 */
L8c615150: sa_motion_publish_stop(ctx,0x8c615150u); fload(m,bus,&io_status,f,5u,r[3]);if(io_status)return io_status;goto L8c615152; /* fmov.s @r3,fr5 */
L8c615152: sa_motion_publish_stop(ctx,0x8c615152u); fcopy(f,2u,7u);goto L8c615154; /* fmov fr7,fr2 */
L8c615154: sa_motion_publish_stop(ctx,0x8c615154u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615156; /* fmul fr6,fr2 */
L8c615156: sa_motion_publish_stop(ctx,0x8c615156u); fcopy(f,1u,9u);goto L8c615158; /* fmov fr9,fr1 */
L8c615158: sa_motion_publish_stop(ctx,0x8c615158u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c61515a; /* fsub fr3,fr0 */
L8c61515a: sa_motion_publish_stop(ctx,0x8c61515au); fcopy(f,3u,8u);goto L8c61515c; /* fmov fr8,fr3 */
L8c61515c: sa_motion_publish_stop(ctx,0x8c61515cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61515e; /* fmul fr5,fr3 */
L8c61515e: sa_motion_publish_stop(ctx,0x8c61515eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615160; /* fsub fr3,fr2 */
L8c615160: sa_motion_publish_stop(ctx,0x8c615160u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c615162; /* fmov.s @(r0,r6),fr3 */
L8c615162: sa_motion_publish_stop(ctx,0x8c615162u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615164; /* fmul fr2,fr3 */
L8c615164: sa_motion_publish_stop(ctx,0x8c615164u); fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c615166; /* fmov.s @r6,fr2 */
L8c615166: sa_motion_publish_stop(ctx,0x8c615166u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615168; /* fmac fr0,fr2,fr3 */
L8c615168: sa_motion_publish_stop(ctx,0x8c615168u); fcopy(f,2u,3u);goto L8c61516a; /* fmov fr3,fr2 */
L8c61516a: sa_motion_publish_stop(ctx,0x8c61516au); fcopy(f,3u,7u);goto L8c61516c; /* fmov fr7,fr3 */
L8c61516c: sa_motion_publish_stop(ctx,0x8c61516cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61516e; /* fmul fr4,fr3 */
L8c61516e: sa_motion_publish_stop(ctx,0x8c61516eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[5],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615170; /* fmul fr5,fr1 */
L8c615170: sa_motion_publish_stop(ctx,0x8c615170u); r[0]=8u;goto L8c615172; /* mov #0x8,r0 */
L8c615172: sa_motion_publish_stop(ctx,0x8c615172u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c615174; /* fsub fr3,fr1 */
L8c615174: sa_motion_publish_stop(ctx,0x8c615174u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c615176; /* fmov.s @(r0,r6),fr3 */
L8c615176: sa_motion_publish_stop(ctx,0x8c615176u); fcopy(f,0u,1u);goto L8c615178; /* fmov fr1,fr0 */
L8c615178: sa_motion_publish_stop(ctx,0x8c615178u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61517a; /* fmac fr0,fr3,fr2 */
L8c61517a: sa_motion_publish_stop(ctx,0x8c61517au); fcopy(f,3u,15u);goto L8c61517c; /* fmov fr15,fr3 */
L8c61517c: sa_motion_publish_stop(ctx,0x8c61517cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c61517e; /* fneg fr3 */
L8c61517e: sa_motion_publish_stop(ctx,0x8c61517eu); fcopy(f,4u,2u);goto L8c615180; /* fmov fr2,fr4 */
L8c615180: sa_motion_publish_stop(ctx,0x8c615180u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c615182; /* fmul fr3,fr4 */
L8c615182: sa_motion_publish_stop(ctx,0x8c615182u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c615184; /* fldi0 fr2 */
L8c615184: sa_motion_publish_stop(ctx,0x8c615184u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c615186; /* fcmp/gt fr2,fr4 */
L8c615186: sa_motion_publish_stop(ctx,0x8c615186u); if((*sr&1u))goto L8c6151ac;goto L8c615188; /* bt 0x8c6151ac */
L8c615188: sa_motion_publish_stop(ctx,0x8c615188u); wr(m,bus,&io_status,r[15]+0x18u,r[5],4);if(io_status)return io_status;goto L8c61518a; /* mov.l r5,@(0x18,r15) */
L8c61518a: sa_motion_publish_stop(ctx,0x8c61518cu); fcopy(f,4u,12u);goto L8c6151ac; /* bra 0x8c6151ac */
L8c61518e: sa_motion_publish_stop(ctx,0x8c61518eu); r[0]=20u;goto L8c615190; /* mov #0x14,r0 */
L8c615190: sa_motion_publish_stop(ctx,0x8c615190u); fcopy(f,3u,13u);goto L8c615192; /* fmov fr13,fr3 */
L8c615192: sa_motion_publish_stop(ctx,0x8c615192u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c615194; /* fmov.s @(r0,r12),fr2 */
L8c615194: sa_motion_publish_stop(ctx,0x8c615194u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615196; /* fsub fr5,fr3 */
L8c615196: sa_motion_publish_stop(ctx,0x8c615196u); r[0]=16u;goto L8c615198; /* mov #0x10,r0 */
L8c615198: sa_motion_publish_stop(ctx,0x8c615198u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61519a; /* fsub fr14,fr2 */
L8c61519a: sa_motion_publish_stop(ctx,0x8c61519au); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c61519c; /* fmov.s @(r0,r12),fr1 */
L8c61519c: sa_motion_publish_stop(ctx,0x8c61519cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61519e; /* fsub fr13,fr1 */
L8c61519e: sa_motion_publish_stop(ctx,0x8c61519eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6151a0; /* fmul fr2,fr3 */
L8c6151a0: sa_motion_publish_stop(ctx,0x8c6151a0u); fcopy(f,2u,14u);goto L8c6151a2; /* fmov fr14,fr2 */
L8c6151a2: sa_motion_publish_stop(ctx,0x8c6151a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[6],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6151a4; /* fsub fr6,fr2 */
L8c6151a4: sa_motion_publish_stop(ctx,0x8c6151a4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6151a6; /* fmul fr1,fr2 */
L8c6151a6: sa_motion_publish_stop(ctx,0x8c6151a6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6151a8; /* fsub fr2,fr3 */
L8c6151a8: sa_motion_publish_stop(ctx,0x8c6151a8u); fcopy(f,4u,3u);goto L8c6151aa; /* fmov fr3,fr4 */
L8c6151aa: sa_motion_publish_stop(ctx,0x8c6151aau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6151ac; /* fmul fr15,fr4 */
L8c6151ac: sa_motion_publish_stop(ctx,0x8c6151acu); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c6151ae; /* cmp/hs r8,r9 */
L8c6151ae: sa_motion_publish_stop(ctx,0x8c6151aeu); if((*sr&1u))goto L8c615202;goto L8c6151b0; /* bt 0x8c615202 */
L8c6151b0: sa_motion_publish_stop(ctx,0x8c6151b0u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6151b2; /* fldi0 fr3 */
L8c6151b2: sa_motion_publish_stop(ctx,0x8c6151b2u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6151b4; /* fcmp/gt fr4,fr3 */
L8c6151b4: sa_motion_publish_stop(ctx,0x8c6151b4u); if((*sr&1u))goto L8c615252;goto L8c6151b6; /* bt 0x8c615252 */
L8c6151b6: sa_motion_publish_stop(ctx,0x8c6151b6u); r[5]=rd(m,bus,&io_status,0x8c6153f4u,4);if(io_status)return io_status;goto L8c6151b8; /* mov.l 0x8c6153f4,r5 */
L8c6151b8: sa_motion_publish_stop(ctx,0x8c6151b8u); r[6]=r[14];goto L8c6151ba; /* mov r14,r6 */
L8c6151ba: sa_motion_publish_stop(ctx,0x8c6151bau); r[4]=r[11];goto L8c6151bc; /* mov r11,r4 */
L8c6151bc: sa_motion_publish_stop(ctx,0x8c6151bcu); r[14]+=4u;goto L8c6151be; /* add #0x4,r14 */
L8c6151be: sa_motion_publish_stop(ctx,0x8c6151beu); r[8]+=16u;goto L8c6151c0; /* add #0x10,r8 */
L8c6151c0: sa_motion_publish_stop(ctx,0x8c6151c0u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6151c2; /* fschg */
L8c6151c2: sa_motion_publish_stop(ctx,0x8c6151c2u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6151c4; /* fmov.s @r4+,fr0 */
L8c6151c4: sa_motion_publish_stop(ctx,0x8c6151c4u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6151c6; /* fmov.s @r4+,fr2 */
L8c6151c6: sa_motion_publish_stop(ctx,0x8c6151c6u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6151c8; /* fldi0 fr3 */
L8c6151c8: sa_motion_publish_stop(ctx,0x8c6151c8u); fcopy(f,8u,1u);goto L8c6151ca; /* fmov fr1,fr8 */
L8c6151ca: sa_motion_publish_stop(ctx,0x8c6151cau); fcopy(f,10u,3u);goto L8c6151cc; /* fmov fr3,fr10 */
L8c6151cc: sa_motion_publish_stop(ctx,0x8c6151ccu); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c6151ce; /* fipr fv8,fv0 */
L8c6151ce: sa_motion_publish_stop(ctx,0x8c6151ceu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6151d0; /* fmov.s @r4+,fr4 */
L8c6151d0: sa_motion_publish_stop(ctx,0x8c6151d0u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6151d2; /* fmov.s @r4,fr6 */
L8c6151d2: sa_motion_publish_stop(ctx,0x8c6151d2u); fcopy(f,8u,5u);goto L8c6151d4; /* fmov fr5,fr8 */
L8c6151d4: sa_motion_publish_stop(ctx,0x8c6151d4u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6151d6; /* fschg */
L8c6151d6: sa_motion_publish_stop(ctx,0x8c6151d6u); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c6151d8; /* mov.w @r6+,r2 */
L8c6151d8: sa_motion_publish_stop(ctx,0x8c6151d8u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c6151da; /* mov.w @r6,r3 */
L8c6151da: sa_motion_publish_stop(ctx,0x8c6151dau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6151dc; /* fmov.s fr6,@-r8 */
L8c6151dc: sa_motion_publish_stop(ctx,0x8c6151dcu); *fpul=r[3];goto L8c6151de; /* lds r3,FPUL */
L8c6151de: sa_motion_publish_stop(ctx,0x8c6151deu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6151e0; /* fmov.s fr5,@-r8 */
L8c6151e0: sa_motion_publish_stop(ctx,0x8c6151e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c6151e2; /* fsub fr3,fr11 */
L8c6151e2: sa_motion_publish_stop(ctx,0x8c6151e2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6151e4; /* fmov.s fr4,@-r8 */
L8c6151e4: sa_motion_publish_stop(ctx,0x8c6151e4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c6151e6; /* float FPUL,fr1 */
L8c6151e6: sa_motion_publish_stop(ctx,0x8c6151e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6151e8; /* fmul fr8,fr1 */
L8c6151e8: sa_motion_publish_stop(ctx,0x8c6151e8u); *fpul=r[2];goto L8c6151ea; /* lds r2,FPUL */
L8c6151ea: sa_motion_publish_stop(ctx,0x8c6151eau); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6151ec; /* mov.l r5,@-r8 */
L8c6151ec: sa_motion_publish_stop(ctx,0x8c6151ecu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c6151ee; /* float FPUL,fr2 */
L8c6151ee: sa_motion_publish_stop(ctx,0x8c6151eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6151f0; /* fmul fr8,fr2 */
L8c6151f0: sa_motion_publish_stop(ctx,0x8c6151f0u); r[8]+=28u;goto L8c6151f2; /* add #0x1c,r8 */
L8c6151f2: sa_motion_publish_stop(ctx,0x8c6151f2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c6151f4; /* fmov.s fr11,@-r8 */
L8c6151f4: sa_motion_publish_stop(ctx,0x8c6151f4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6151f6; /* fmov.s fr1,@-r8 */
L8c6151f6: sa_motion_publish_stop(ctx,0x8c6151f6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6151f8; /* fmov.s fr2,@-r8 */
L8c6151f8: sa_motion_publish_stop(ctx,0x8c6151f8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6151fa; /* pref @r8 */
L8c6151fa: sa_motion_publish_stop(ctx,0x8c6151fau); r[8]+=16u;goto L8c6151fc; /* add #0x10,r8 */
L8c6151fc: sa_motion_publish_stop(ctx,0x8c6151fcu); r[14]+=4294967292u;goto L8c6151fe; /* add #-0x4,r14 */
L8c6151fe: sa_motion_publish_stop(ctx,0x8c615200u); r[9]=r[8];goto L8c615298; /* bra 0x8c615298 */
L8c615202: sa_motion_publish_stop(ctx,0x8c615202u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615204; /* fldi0 fr3 */
L8c615204: sa_motion_publish_stop(ctx,0x8c615204u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c615206; /* fcmp/gt fr4,fr3 */
L8c615206: sa_motion_publish_stop(ctx,0x8c615208u); take=!(*sr&1u);r[14]+=4u;if(take)goto L8c615298;goto L8c61520a; /* bf/s 0x8c615298 */
L8c61520a: sa_motion_publish_stop(ctx,0x8c61520au); r[14]+=4294967292u;goto L8c61520c; /* add #-0x4,r14 */
L8c61520c: sa_motion_publish_stop(ctx,0x8c61520cu); r[5]=rd(m,bus,&io_status,0x8c6153f8u,4);if(io_status)return io_status;goto L8c61520e; /* mov.l 0x8c6153f8,r5 */
L8c61520e: sa_motion_publish_stop(ctx,0x8c61520eu); r[6]=r[14];goto L8c615210; /* mov r14,r6 */
L8c615210: sa_motion_publish_stop(ctx,0x8c615210u); r[4]=r[10];goto L8c615212; /* mov r10,r4 */
L8c615212: sa_motion_publish_stop(ctx,0x8c615212u); r[14]+=4u;goto L8c615214; /* add #0x4,r14 */
L8c615214: sa_motion_publish_stop(ctx,0x8c615214u); r[8]+=16u;goto L8c615216; /* add #0x10,r8 */
L8c615216: sa_motion_publish_stop(ctx,0x8c615216u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615218; /* fschg */
L8c615218: sa_motion_publish_stop(ctx,0x8c615218u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61521a; /* fmov.s @r4+,fr0 */
L8c61521a: sa_motion_publish_stop(ctx,0x8c61521au); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61521c; /* fmov.s @r4+,fr2 */
L8c61521c: sa_motion_publish_stop(ctx,0x8c61521cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61521e; /* fldi0 fr3 */
L8c61521e: sa_motion_publish_stop(ctx,0x8c61521eu); fcopy(f,8u,1u);goto L8c615220; /* fmov fr1,fr8 */
L8c615220: sa_motion_publish_stop(ctx,0x8c615220u); fcopy(f,10u,3u);goto L8c615222; /* fmov fr3,fr10 */
L8c615222: sa_motion_publish_stop(ctx,0x8c615222u); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c615224; /* fipr fv8,fv0 */
L8c615224: sa_motion_publish_stop(ctx,0x8c615224u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615226; /* fmov.s @r4+,fr4 */
L8c615226: sa_motion_publish_stop(ctx,0x8c615226u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615228; /* fmov.s @r4,fr6 */
L8c615228: sa_motion_publish_stop(ctx,0x8c615228u); fcopy(f,8u,5u);goto L8c61522a; /* fmov fr5,fr8 */
L8c61522a: sa_motion_publish_stop(ctx,0x8c61522au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61522c; /* fschg */
L8c61522c: sa_motion_publish_stop(ctx,0x8c61522cu); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c61522e; /* mov.w @r6+,r2 */
L8c61522e: sa_motion_publish_stop(ctx,0x8c61522eu); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c615230; /* mov.w @r6,r3 */
L8c615230: sa_motion_publish_stop(ctx,0x8c615230u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615232; /* fmov.s fr6,@-r8 */
L8c615232: sa_motion_publish_stop(ctx,0x8c615232u); *fpul=r[3];goto L8c615234; /* lds r3,FPUL */
L8c615234: sa_motion_publish_stop(ctx,0x8c615234u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615236; /* fmov.s fr5,@-r8 */
L8c615236: sa_motion_publish_stop(ctx,0x8c615236u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c615238; /* fsub fr3,fr11 */
L8c615238: sa_motion_publish_stop(ctx,0x8c615238u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61523a; /* fmov.s fr4,@-r8 */
L8c61523a: sa_motion_publish_stop(ctx,0x8c61523au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c61523c; /* float FPUL,fr1 */
L8c61523c: sa_motion_publish_stop(ctx,0x8c61523cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61523e; /* fmul fr8,fr1 */
L8c61523e: sa_motion_publish_stop(ctx,0x8c61523eu); *fpul=r[2];goto L8c615240; /* lds r2,FPUL */
L8c615240: sa_motion_publish_stop(ctx,0x8c615240u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615242; /* mov.l r5,@-r8 */
L8c615242: sa_motion_publish_stop(ctx,0x8c615242u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c615244; /* float FPUL,fr2 */
L8c615244: sa_motion_publish_stop(ctx,0x8c615244u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615246; /* fmul fr8,fr2 */
L8c615246: sa_motion_publish_stop(ctx,0x8c615246u); r[8]+=28u;goto L8c615248; /* add #0x1c,r8 */
L8c615248: sa_motion_publish_stop(ctx,0x8c615248u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c61524a; /* fmov.s fr11,@-r8 */
L8c61524a: sa_motion_publish_stop(ctx,0x8c61524au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61524c; /* fmov.s fr1,@-r8 */
L8c61524c: sa_motion_publish_stop(ctx,0x8c61524cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61524e; /* fmov.s fr2,@-r8 */
L8c61524e: sa_motion_publish_stop(ctx,0x8c61524eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615250; /* pref @r8 */
L8c615250: sa_motion_publish_stop(ctx,0x8c615250u); r[8]+=16u;goto L8c615252; /* add #0x10,r8 */
L8c615252: sa_motion_publish_stop(ctx,0x8c615252u); r[5]=rd(m,bus,&io_status,0x8c6153f8u,4);if(io_status)return io_status;goto L8c615254; /* mov.l 0x8c6153f8,r5 */
L8c615254: sa_motion_publish_stop(ctx,0x8c615254u); r[6]=r[14];goto L8c615256; /* mov r14,r6 */
L8c615256: sa_motion_publish_stop(ctx,0x8c615256u); r[4]=r[11];goto L8c615258; /* mov r11,r4 */
L8c615258: sa_motion_publish_stop(ctx,0x8c615258u); r[14]+=4u;goto L8c61525a; /* add #0x4,r14 */
L8c61525a: sa_motion_publish_stop(ctx,0x8c61525au); r[8]+=16u;goto L8c61525c; /* add #0x10,r8 */
L8c61525c: sa_motion_publish_stop(ctx,0x8c61525cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61525e; /* fschg */
L8c61525e: sa_motion_publish_stop(ctx,0x8c61525eu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615260; /* fmov.s @r4+,fr0 */
L8c615260: sa_motion_publish_stop(ctx,0x8c615260u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615262; /* fmov.s @r4+,fr2 */
L8c615262: sa_motion_publish_stop(ctx,0x8c615262u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615264; /* fldi0 fr3 */
L8c615264: sa_motion_publish_stop(ctx,0x8c615264u); fcopy(f,8u,1u);goto L8c615266; /* fmov fr1,fr8 */
L8c615266: sa_motion_publish_stop(ctx,0x8c615266u); fcopy(f,10u,3u);goto L8c615268; /* fmov fr3,fr10 */
L8c615268: sa_motion_publish_stop(ctx,0x8c615268u); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c61526a; /* fipr fv8,fv0 */
L8c61526a: sa_motion_publish_stop(ctx,0x8c61526au); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61526c; /* fmov.s @r4+,fr4 */
L8c61526c: sa_motion_publish_stop(ctx,0x8c61526cu); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61526e; /* fmov.s @r4,fr6 */
L8c61526e: sa_motion_publish_stop(ctx,0x8c61526eu); fcopy(f,8u,5u);goto L8c615270; /* fmov fr5,fr8 */
L8c615270: sa_motion_publish_stop(ctx,0x8c615270u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615272; /* fschg */
L8c615272: sa_motion_publish_stop(ctx,0x8c615272u); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c615274; /* mov.w @r6+,r2 */
L8c615274: sa_motion_publish_stop(ctx,0x8c615274u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c615276; /* mov.w @r6,r3 */
L8c615276: sa_motion_publish_stop(ctx,0x8c615276u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615278; /* fmov.s fr6,@-r8 */
L8c615278: sa_motion_publish_stop(ctx,0x8c615278u); *fpul=r[3];goto L8c61527a; /* lds r3,FPUL */
L8c61527a: sa_motion_publish_stop(ctx,0x8c61527au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61527c; /* fmov.s fr5,@-r8 */
L8c61527c: sa_motion_publish_stop(ctx,0x8c61527cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c61527e; /* fsub fr3,fr11 */
L8c61527e: sa_motion_publish_stop(ctx,0x8c61527eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615280; /* fmov.s fr4,@-r8 */
L8c615280: sa_motion_publish_stop(ctx,0x8c615280u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c615282; /* float FPUL,fr1 */
L8c615282: sa_motion_publish_stop(ctx,0x8c615282u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615284; /* fmul fr8,fr1 */
L8c615284: sa_motion_publish_stop(ctx,0x8c615284u); *fpul=r[2];goto L8c615286; /* lds r2,FPUL */
L8c615286: sa_motion_publish_stop(ctx,0x8c615286u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615288; /* mov.l r5,@-r8 */
L8c615288: sa_motion_publish_stop(ctx,0x8c615288u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c61528a; /* float FPUL,fr2 */
L8c61528a: sa_motion_publish_stop(ctx,0x8c61528au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61528c; /* fmul fr8,fr2 */
L8c61528c: sa_motion_publish_stop(ctx,0x8c61528cu); r[8]+=28u;goto L8c61528e; /* add #0x1c,r8 */
L8c61528e: sa_motion_publish_stop(ctx,0x8c61528eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c615290; /* fmov.s fr11,@-r8 */
L8c615290: sa_motion_publish_stop(ctx,0x8c615290u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c615292; /* fmov.s fr1,@-r8 */
L8c615292: sa_motion_publish_stop(ctx,0x8c615292u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c615294; /* fmov.s fr2,@-r8 */
L8c615294: sa_motion_publish_stop(ctx,0x8c615294u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615296; /* pref @r8 */
L8c615296: sa_motion_publish_stop(ctx,0x8c615296u); r[8]+=16u;goto L8c615298; /* add #0x10,r8 */
L8c615298: sa_motion_publish_stop(ctx,0x8c615298u); r[2]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c61529a; /* mov.l @(0x18,r15),r2 */
L8c61529a: sa_motion_publish_stop(ctx,0x8c61529au); *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c61529c; /* tst r2,r2 */
L8c61529c: sa_motion_publish_stop(ctx,0x8c61529cu); if((*sr&1u))goto L8c615312;goto L8c61529e; /* bt 0x8c615312 */
L8c61529e: sa_motion_publish_stop(ctx,0x8c61529eu); r[4]=rd(m,bus,&io_status,0x8c6153fcu,4);if(io_status)return io_status;goto L8c6152a0; /* mov.l 0x8c6153fc,r4 */
L8c6152a0: sa_motion_publish_stop(ctx,0x8c6152a0u); r[3]=0u;goto L8c6152a2; /* mov #0x0,r3 */
L8c6152a2: sa_motion_publish_stop(ctx,0x8c6152a2u); r[14]+=4294967292u;goto L8c6152a4; /* add #-0x4,r14 */
L8c6152a4: sa_motion_publish_stop(ctx,0x8c6152a4u); r[9]=rd(m,bus,&io_status,0x8c615400u,4);if(io_status)return io_status;goto L8c6152a6; /* mov.l 0x8c615400,r9 */
L8c6152a6: sa_motion_publish_stop(ctx,0x8c6152a6u); wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c6152a8; /* mov.l r3,@r4 */
L8c6152a8: sa_motion_publish_stop(ctx,0x8c6152a8u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c6152aa; /* mov.w @r14,r3 */
L8c6152aa: sa_motion_publish_stop(ctx,0x8c6152aau); r[4]=r[9];goto L8c6152ac; /* mov r9,r4 */
L8c6152ac: sa_motion_publish_stop(ctx,0x8c6152acu); *fpul=r[3];goto L8c6152ae; /* lds r3,FPUL */
L8c6152ae: sa_motion_publish_stop(ctx,0x8c6152aeu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6152b0; /* float FPUL,fr3 */
L8c6152b0: sa_motion_publish_stop(ctx,0x8c6152b0u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6152b2; /* ftrc fr3,FPUL */
L8c6152b2: sa_motion_publish_stop(ctx,0x8c6152b2u); r[2]=*fpul;goto L8c6152b4; /* sts FPUL,r2 */
L8c6152b4: sa_motion_publish_stop(ctx,0x8c6152b4u); wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c6152b6; /* mov.w r2,@r4 */
L8c6152b6: sa_motion_publish_stop(ctx,0x8c6152b6u); r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c6152b8; /* mov.w @(0x2,r14),r0 */
L8c6152b8: sa_motion_publish_stop(ctx,0x8c6152b8u); r[14]+=4u;goto L8c6152ba; /* add #0x4,r14 */
L8c6152ba: sa_motion_publish_stop(ctx,0x8c6152bau); r[3]=r[0];goto L8c6152bc; /* mov r0,r3 */
L8c6152bc: sa_motion_publish_stop(ctx,0x8c6152bcu); *fpul=r[3];goto L8c6152be; /* lds r3,FPUL */
L8c6152be: sa_motion_publish_stop(ctx,0x8c6152beu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6152c0; /* float FPUL,fr3 */
L8c6152c0: sa_motion_publish_stop(ctx,0x8c6152c0u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6152c2; /* ftrc fr3,FPUL */
L8c6152c2: sa_motion_publish_stop(ctx,0x8c6152c2u); r[0]=*fpul;goto L8c6152c4; /* sts FPUL,r0 */
L8c6152c4: sa_motion_publish_stop(ctx,0x8c6152c4u); wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6152c6; /* mov.w r0,@(0x2,r4) */
L8c6152c6: sa_motion_publish_stop(ctx,0x8c6152c6u); r[4]+=4u;goto L8c6152c8; /* add #0x4,r4 */
L8c6152c8: sa_motion_publish_stop(ctx,0x8c6152c8u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c6152ca; /* mov.w @r14,r3 */
L8c6152ca: sa_motion_publish_stop(ctx,0x8c6152cau); *fpul=r[3];goto L8c6152cc; /* lds r3,FPUL */
L8c6152cc: sa_motion_publish_stop(ctx,0x8c6152ccu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6152ce; /* float FPUL,fr3 */
L8c6152ce: sa_motion_publish_stop(ctx,0x8c6152ceu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6152d0; /* ftrc fr3,FPUL */
L8c6152d0: sa_motion_publish_stop(ctx,0x8c6152d0u); r[2]=*fpul;goto L8c6152d2; /* sts FPUL,r2 */
L8c6152d2: sa_motion_publish_stop(ctx,0x8c6152d2u); wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c6152d4; /* mov.w r2,@r4 */
L8c6152d4: sa_motion_publish_stop(ctx,0x8c6152d4u); r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c6152d6; /* mov.w @(0x2,r14),r0 */
L8c6152d6: sa_motion_publish_stop(ctx,0x8c6152d6u); r[3]=r[0];goto L8c6152d8; /* mov r0,r3 */
L8c6152d8: sa_motion_publish_stop(ctx,0x8c6152d8u); *fpul=r[3];goto L8c6152da; /* lds r3,FPUL */
L8c6152da: sa_motion_publish_stop(ctx,0x8c6152dau); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6152dc; /* float FPUL,fr3 */
L8c6152dc: sa_motion_publish_stop(ctx,0x8c6152dcu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6152de; /* ftrc fr3,FPUL */
L8c6152de: sa_motion_publish_stop(ctx,0x8c6152deu); r[14]+=4u;goto L8c6152e0; /* add #0x4,r14 */
L8c6152e0: sa_motion_publish_stop(ctx,0x8c6152e0u); r[0]=*fpul;goto L8c6152e2; /* sts FPUL,r0 */
L8c6152e2: sa_motion_publish_stop(ctx,0x8c6152e2u); r[6]=r[12];goto L8c6152e4; /* mov r12,r6 */
L8c6152e4: sa_motion_publish_stop(ctx,0x8c6152e4u); r[5]=r[11];goto L8c6152e6; /* mov r11,r5 */
L8c6152e6: sa_motion_publish_stop(ctx,0x8c6152e6u); r[7]=r[9];goto L8c6152e8; /* mov r9,r7 */
L8c6152e8: sa_motion_publish_stop(ctx,0x8c6152e8u); wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6152ea; /* mov.w r0,@(0x2,r4) */
L8c6152ea: sa_motion_publish_stop(ctx,0x8c6152eau); r[4]=r[9];goto L8c6152ec; /* mov r9,r4 */
L8c6152ec: sa_motion_publish_stop(ctx,0x8c6152ecu); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c6152ee; /* mov.w @r14,r3 */
L8c6152ee: sa_motion_publish_stop(ctx,0x8c6152eeu); r[4]+=8u;goto L8c6152f0; /* add #0x8,r4 */
L8c6152f0: sa_motion_publish_stop(ctx,0x8c6152f0u); *fpul=r[3];goto L8c6152f2; /* lds r3,FPUL */
L8c6152f2: sa_motion_publish_stop(ctx,0x8c6152f2u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6152f4; /* float FPUL,fr3 */
L8c6152f4: sa_motion_publish_stop(ctx,0x8c6152f4u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6152f6; /* ftrc fr3,FPUL */
L8c6152f6: sa_motion_publish_stop(ctx,0x8c6152f6u); r[2]=*fpul;goto L8c6152f8; /* sts FPUL,r2 */
L8c6152f8: sa_motion_publish_stop(ctx,0x8c6152f8u); wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c6152fa; /* mov.w r2,@r4 */
L8c6152fa: sa_motion_publish_stop(ctx,0x8c6152fau); r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c6152fc; /* mov.w @(0x2,r14),r0 */
L8c6152fc: sa_motion_publish_stop(ctx,0x8c6152fcu); r[3]=r[0];goto L8c6152fe; /* mov r0,r3 */
L8c6152fe: sa_motion_publish_stop(ctx,0x8c6152feu); *fpul=r[3];goto L8c615300; /* lds r3,FPUL */
L8c615300: sa_motion_publish_stop(ctx,0x8c615300u); r[3]=rd(m,bus,&io_status,0x8c615404u,4);if(io_status)return io_status;goto L8c615302; /* mov.l 0x8c615404,r3 */
L8c615302: sa_motion_publish_stop(ctx,0x8c615302u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c615304; /* float FPUL,fr3 */
L8c615304: sa_motion_publish_stop(ctx,0x8c615304u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c615306; /* ftrc fr3,FPUL */
L8c615306: sa_motion_publish_stop(ctx,0x8c615306u); r[0]=*fpul;goto L8c615308; /* sts FPUL,r0 */
L8c615308: sa_motion_publish_stop(ctx,0x8c615308u); wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61530a; /* mov.w r0,@(0x2,r4) */
L8c61530a: sa_motion_publish_stop(ctx,0x8c61530cu); target=r[3];*pr=0x8c61530eu;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61530e; /* jsr @r3 */
L8c61530e: sa_motion_publish_stop(ctx,0x8c61530eu); r[9]=r[8];goto L8c615310; /* mov r8,r9 */
L8c615310: sa_motion_publish_stop(ctx,0x8c615310u); r[14]+=4294967292u;goto L8c615312; /* add #-0x4,r14 */
L8c615312: sa_motion_publish_stop(ctx,0x8c615312u); fcopy(f,5u,13u);goto L8c615314; /* fmov fr13,fr5 */
L8c615314: sa_motion_publish_stop(ctx,0x8c615314u); r[0]=16u;goto L8c615316; /* mov #0x10,r0 */
L8c615316: sa_motion_publish_stop(ctx,0x8c615316u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615318; /* mov.l @(0x8,r15),r3 */
L8c615318: sa_motion_publish_stop(ctx,0x8c615318u); r[10]=r[11];goto L8c61531a; /* mov r11,r10 */
L8c61531a: sa_motion_publish_stop(ctx,0x8c61531au); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c61531c; /* fmov.s @(r0,r12),fr13 */
L8c61531c: sa_motion_publish_stop(ctx,0x8c61531cu); r[0]=20u;goto L8c61531e; /* mov #0x14,r0 */
L8c61531e: sa_motion_publish_stop(ctx,0x8c61531eu); fcopy(f,6u,14u);goto L8c615320; /* fmov fr14,fr6 */
L8c615320: sa_motion_publish_stop(ctx,0x8c615320u); r[3]+=4294967295u;goto L8c615322; /* add #-0x1,r3 */
L8c615322: sa_motion_publish_stop(ctx,0x8c615322u); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c615324; /* fmov.s @(r0,r12),fr14 */
L8c615324: sa_motion_publish_stop(ctx,0x8c615324u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c615326; /* tst r3,r3 */
L8c615326: sa_motion_publish_stop(ctx,0x8c615326u); r[11]=r[12];goto L8c615328; /* mov r12,r11 */
L8c615328: sa_motion_publish_stop(ctx,0x8c615328u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c61532a; /* fneg fr15 */
L8c61532a: sa_motion_publish_stop(ctx,0x8c61532cu); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c615332;goto L8c61532e; /* bt/s 0x8c615332 */
L8c61532e: sa_motion_publish_stop(ctx,0x8c615330u); goto L8c61501a; /* bra 0x8c61501a */
L8c615332: sa_motion_publish_stop(ctx,0x8c615332u); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c615334; /* cmp/hs r8,r9 */
L8c615334: sa_motion_publish_stop(ctx,0x8c615336u); take=(*sr&1u);r[14]+=8u;if(take)goto L8c615380;goto L8c615338; /* bt/s 0x8c615380 */
L8c615338: sa_motion_publish_stop(ctx,0x8c615338u); r[14]+=4294967288u;goto L8c61533a; /* add #-0x8,r14 */
L8c61533a: sa_motion_publish_stop(ctx,0x8c61533au); r[5]=rd(m,bus,&io_status,0x8c6153f4u,4);if(io_status)return io_status;goto L8c61533c; /* mov.l 0x8c6153f4,r5 */
L8c61533c: sa_motion_publish_stop(ctx,0x8c61533cu); r[6]=r[14];goto L8c61533e; /* mov r14,r6 */
L8c61533e: sa_motion_publish_stop(ctx,0x8c61533eu); r[4]=r[12];goto L8c615340; /* mov r12,r4 */
L8c615340: sa_motion_publish_stop(ctx,0x8c615340u); r[14]+=4u;goto L8c615342; /* add #0x4,r14 */
L8c615342: sa_motion_publish_stop(ctx,0x8c615342u); r[8]+=16u;goto L8c615344; /* add #0x10,r8 */
L8c615344: sa_motion_publish_stop(ctx,0x8c615344u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615346; /* fschg */
L8c615346: sa_motion_publish_stop(ctx,0x8c615346u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615348; /* fmov.s @r4+,fr0 */
L8c615348: sa_motion_publish_stop(ctx,0x8c615348u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61534a; /* fmov.s @r4+,fr2 */
L8c61534a: sa_motion_publish_stop(ctx,0x8c61534au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61534c; /* fldi0 fr3 */
L8c61534c: sa_motion_publish_stop(ctx,0x8c61534cu); fcopy(f,8u,1u);goto L8c61534e; /* fmov fr1,fr8 */
L8c61534e: sa_motion_publish_stop(ctx,0x8c61534eu); fcopy(f,10u,3u);goto L8c615350; /* fmov fr3,fr10 */
L8c615350: sa_motion_publish_stop(ctx,0x8c615350u); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c615352; /* fipr fv8,fv0 */
L8c615352: sa_motion_publish_stop(ctx,0x8c615352u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615354; /* fmov.s @r4+,fr4 */
L8c615354: sa_motion_publish_stop(ctx,0x8c615354u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615356; /* fmov.s @r4,fr6 */
L8c615356: sa_motion_publish_stop(ctx,0x8c615356u); fcopy(f,8u,5u);goto L8c615358; /* fmov fr5,fr8 */
L8c615358: sa_motion_publish_stop(ctx,0x8c615358u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61535a; /* fschg */
L8c61535a: sa_motion_publish_stop(ctx,0x8c61535au); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c61535c; /* mov.w @r6+,r2 */
L8c61535c: sa_motion_publish_stop(ctx,0x8c61535cu); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c61535e; /* mov.w @r6,r3 */
L8c61535e: sa_motion_publish_stop(ctx,0x8c61535eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615360; /* fmov.s fr6,@-r8 */
L8c615360: sa_motion_publish_stop(ctx,0x8c615360u); *fpul=r[3];goto L8c615362; /* lds r3,FPUL */
L8c615362: sa_motion_publish_stop(ctx,0x8c615362u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615364; /* fmov.s fr5,@-r8 */
L8c615364: sa_motion_publish_stop(ctx,0x8c615364u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c615366; /* fsub fr3,fr11 */
L8c615366: sa_motion_publish_stop(ctx,0x8c615366u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615368; /* fmov.s fr4,@-r8 */
L8c615368: sa_motion_publish_stop(ctx,0x8c615368u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c61536a; /* float FPUL,fr1 */
L8c61536a: sa_motion_publish_stop(ctx,0x8c61536au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61536c; /* fmul fr8,fr1 */
L8c61536c: sa_motion_publish_stop(ctx,0x8c61536cu); *fpul=r[2];goto L8c61536e; /* lds r2,FPUL */
L8c61536e: sa_motion_publish_stop(ctx,0x8c61536eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615370; /* mov.l r5,@-r8 */
L8c615370: sa_motion_publish_stop(ctx,0x8c615370u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c615372; /* float FPUL,fr2 */
L8c615372: sa_motion_publish_stop(ctx,0x8c615372u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615374; /* fmul fr8,fr2 */
L8c615374: sa_motion_publish_stop(ctx,0x8c615374u); r[8]+=28u;goto L8c615376; /* add #0x1c,r8 */
L8c615376: sa_motion_publish_stop(ctx,0x8c615376u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c615378; /* fmov.s fr11,@-r8 */
L8c615378: sa_motion_publish_stop(ctx,0x8c615378u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61537a; /* fmov.s fr1,@-r8 */
L8c61537a: sa_motion_publish_stop(ctx,0x8c61537au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61537c; /* fmov.s fr2,@-r8 */
L8c61537c: sa_motion_publish_stop(ctx,0x8c61537cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61537e; /* pref @r8 */
L8c61537e: sa_motion_publish_stop(ctx,0x8c61537eu); r[8]+=16u;goto L8c615380; /* add #0x10,r8 */
L8c615380: sa_motion_publish_stop(ctx,0x8c615380u); r[3]=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;goto L8c615382; /* mov.l @(0x1c,r15),r3 */
L8c615382: sa_motion_publish_stop(ctx,0x8c615382u); r[3]+=4294967295u;goto L8c615384; /* add #-0x1,r3 */
L8c615384: sa_motion_publish_stop(ctx,0x8c615384u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c615386; /* tst r3,r3 */
L8c615386: sa_motion_publish_stop(ctx,0x8c615388u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x1cu,r[3],4);if(io_status)return io_status;if(take)goto L8c61538e;goto L8c61538a; /* bt/s 0x8c61538e */
L8c61538a: sa_motion_publish_stop(ctx,0x8c61538cu); goto L8c614fc8; /* bra 0x8c614fc8 */
L8c61538e: sa_motion_publish_stop(ctx,0x8c61538eu); r[4]=rd(m,bus,&io_status,0x8c615408u,4);if(io_status)return io_status;goto L8c615390; /* mov.l 0x8c615408,r4 */
L8c615390: sa_motion_publish_stop(ctx,0x8c615390u); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c615392; /* mov.l @(0x10,r15),r3 */
L8c615392: sa_motion_publish_stop(ctx,0x8c615392u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c615394; /* mov.l @r4,r2 */
L8c615394: sa_motion_publish_stop(ctx,0x8c615394u); r[2]+=r[3];goto L8c615396; /* add r3,r2 */
L8c615396: sa_motion_publish_stop(ctx,0x8c615396u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c615398; /* mov.l r2,@r4 */
L8c615398: sa_motion_publish_stop(ctx,0x8c615398u); r[4]=rd(m,bus,&io_status,0x8c61540cu,4);if(io_status)return io_status;goto L8c61539a; /* mov.l 0x8c61540c,r4 */
L8c61539a: sa_motion_publish_stop(ctx,0x8c61539au); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61539c; /* mov.l @(0x10,r15),r3 */
L8c61539c: sa_motion_publish_stop(ctx,0x8c61539cu); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61539e; /* mov.l @r4,r2 */
L8c61539e: sa_motion_publish_stop(ctx,0x8c61539eu); r[2]+=r[3];goto L8c6153a0; /* add r3,r2 */
L8c6153a0: sa_motion_publish_stop(ctx,0x8c6153a0u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c6153a2; /* mov.l r2,@r4 */
L8c6153a2: sa_motion_publish_stop(ctx,0x8c6153a2u); r[15]+=80u;goto L8c6153a4; /* add #0x50,r15 */
L8c6153a4: sa_motion_publish_stop(ctx,0x8c6153a4u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6153a6; /* lds.l @r15+,PR */
L8c6153a6: sa_motion_publish_stop(ctx,0x8c6153a6u); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6153a8; /* fmov.s @r15+,fr12 */
L8c6153a8: sa_motion_publish_stop(ctx,0x8c6153a8u); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6153aa; /* fmov.s @r15+,fr13 */
L8c6153aa: sa_motion_publish_stop(ctx,0x8c6153aau); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6153ac; /* fmov.s @r15+,fr14 */
L8c6153ac: sa_motion_publish_stop(ctx,0x8c6153acu); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6153ae; /* fmov.s @r15+,fr15 */
L8c6153ae: sa_motion_publish_stop(ctx,0x8c6153aeu); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6153b0; /* mov.l @r15+,r9 */
L8c6153b0: sa_motion_publish_stop(ctx,0x8c6153b0u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6153b2; /* mov.l @r15+,r10 */
L8c6153b2: sa_motion_publish_stop(ctx,0x8c6153b2u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6153b4; /* mov.l @r15+,r11 */
L8c6153b4: sa_motion_publish_stop(ctx,0x8c6153b4u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6153b6; /* mov.l @r15+,r12 */
L8c6153b6: sa_motion_publish_stop(ctx,0x8c6153b6u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6153b8; /* mov.l @r15+,r13 */
L8c6153b8: sa_motion_publish_stop(ctx,0x8c6153bau); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
