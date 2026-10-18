#include "sa_draw_motion_stop.h"
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
L8c614076: sa_motion_publish_stop(ctx,0x8c614076u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c614078; /* mov.l r14,@-r15 */
L8c614078: sa_motion_publish_stop(ctx,0x8c614078u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61407a; /* mov.l r13,@-r15 */
L8c61407a: sa_motion_publish_stop(ctx,0x8c61407au); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61407c; /* mov.l r12,@-r15 */
L8c61407c: sa_motion_publish_stop(ctx,0x8c61407cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61407e; /* mov.l r11,@-r15 */
L8c61407e: sa_motion_publish_stop(ctx,0x8c61407eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c614080; /* mov.l r10,@-r15 */
L8c614080: sa_motion_publish_stop(ctx,0x8c614080u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c614082; /* mov.l r9,@-r15 */
L8c614082: sa_motion_publish_stop(ctx,0x8c614082u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c614084; /* fmov.s fr15,@-r15 */
L8c614084: sa_motion_publish_stop(ctx,0x8c614084u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c614086; /* fmov.s fr14,@-r15 */
L8c614086: sa_motion_publish_stop(ctx,0x8c614086u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c614088; /* fmov.s fr13,@-r15 */
L8c614088: sa_motion_publish_stop(ctx,0x8c614088u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61408a; /* sts.l PR,@-r15 */
L8c61408a: sa_motion_publish_stop(ctx,0x8c61408au); r[15]+=4294967276u;goto L8c61408c; /* add #-0x14,r15 */
L8c61408c: sa_motion_publish_stop(ctx,0x8c61408cu); r[3]=rd(m,bus,&io_status,0x8c6140c0u,4);if(io_status)return io_status;goto L8c61408e; /* mov.l 0x8c6140c0,r3 */
L8c61408e: sa_motion_publish_stop(ctx,0x8c61408eu); r[9]=rd(m,bus,&io_status,0x8c6140bcu,4);if(io_status)return io_status;goto L8c614090; /* mov.l 0x8c6140bc,r9 */
L8c614090: sa_motion_publish_stop(ctx,0x8c614092u); target=r[3];*pr=0x8c614094u;r[12]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c614094; /* jsr @r3 */
L8c614094: sa_motion_publish_stop(ctx,0x8c614094u); r[2]=rd(m,bus,&io_status,0x8c6140c4u,4);if(io_status)return io_status;goto L8c614096; /* mov.l 0x8c6140c4,r2 */
L8c614096: sa_motion_publish_stop(ctx,0x8c614096u); r[1]=0u;goto L8c614098; /* mov #0x0,r1 */
L8c614098: sa_motion_publish_stop(ctx,0x8c614098u); r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c61409a; /* mov.w @(0x2,r12),r0 */
L8c61409a: sa_motion_publish_stop(ctx,0x8c61409au); r[14]=rd(m,bus,&io_status,r[12]+0x14u,4);if(io_status)return io_status;goto L8c61409c; /* mov.l @(0x14,r12),r14 */
L8c61409c: sa_motion_publish_stop(ctx,0x8c61409cu); r[13]=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;goto L8c61409e; /* mov.l @(0x4,r12),r13 */
L8c61409e: sa_motion_publish_stop(ctx,0x8c61409eu); r[0]=r[0]&65535u;goto L8c6140a0; /* extu.w r0,r0 */
L8c6140a0: sa_motion_publish_stop(ctx,0x8c6140a0u); wr(m,bus,&io_status,r[15]+0x10u,r[0],4);if(io_status)return io_status;goto L8c6140a2; /* mov.l r0,@(0x10,r15) */
L8c6140a2: sa_motion_publish_stop(ctx,0x8c6140a2u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6140a4; /* mov.l @r2,r3 */
L8c6140a4: sa_motion_publish_stop(ctx,0x8c6140a4u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6140a6; /* mov.l r3,@(0x4,r15) */
L8c6140a6: sa_motion_publish_stop(ctx,0x8c6140a6u); wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c6140a8; /* mov.l r1,@(0x8,r15) */
L8c6140a8: sa_motion_publish_stop(ctx,0x8c6140a8u); wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c6140aa; /* mov.l r1,@(0xc,r15) */
L8c6140aa: sa_motion_publish_stop(ctx,0x8c6140acu); goto L8c6140c8; /* bra 0x8c6140c8 */
L8c6140c8: sa_motion_publish_stop(ctx,0x8c6140c8u); r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6140ca; /* mov.w @r13+,r3 */
L8c6140ca: sa_motion_publish_stop(ctx,0x8c6140cau); r[4]=rd(m,bus,&io_status,0x8c6141bcu,4);if(io_status)return io_status;goto L8c6140cc; /* mov.l 0x8c6141bc,r4 */
L8c6140cc: sa_motion_publish_stop(ctx,0x8c6140ccu); r[3]=r[3]&65535u;goto L8c6140ce; /* extu.w r3,r3 */
L8c6140ce: sa_motion_publish_stop(ctx,0x8c6140ceu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6140d0; /* fldi1 fr4 */
L8c6140d0: sa_motion_publish_stop(ctx,0x8c6140d0u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6140d2; /* tst r4,r3 */
L8c6140d2: sa_motion_publish_stop(ctx,0x8c6140d2u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6140d4; /* mov.l r3,@r15 */
L8c6140d4: sa_motion_publish_stop(ctx,0x8c6140d6u); take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c6140e6;goto L8c6140d8; /* bt/s 0x8c6140e6 */
L8c6140d8: sa_motion_publish_stop(ctx,0x8c6140d8u); r[4]=sx(m,bus,&io_status,0x8c6141bau,2);if(io_status)return io_status;goto L8c6140da; /* mov.w 0x8c6141ba,r4 */
L8c6140da: sa_motion_publish_stop(ctx,0x8c6140dau); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6140dc; /* mov.l @r15,r3 */
L8c6140dc: sa_motion_publish_stop(ctx,0x8c6140dcu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6140de; /* fldi1 fr4 */
L8c6140de: sa_motion_publish_stop(ctx,0x8c6140deu); r[3]&=r[4];goto L8c6140e0; /* and r4,r3 */
L8c6140e0: sa_motion_publish_stop(ctx,0x8c6140e0u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c6140e2; /* fneg fr4 */
L8c6140e2: sa_motion_publish_stop(ctx,0x8c6140e2u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6140e4; /* mov.l r3,@r15 */
L8c6140e4: sa_motion_publish_stop(ctx,0x8c6140e4u); fcopy(f,15u,4u);goto L8c6140e6; /* fmov fr4,fr15 */
L8c6140e6: sa_motion_publish_stop(ctx,0x8c6140e6u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6140e8; /* mov.l @r15,r3 */
L8c6140e8: sa_motion_publish_stop(ctx,0x8c6140e8u); r[0]=16u;goto L8c6140ea; /* mov #0x10,r0 */
L8c6140ea: sa_motion_publish_stop(ctx,0x8c6140eau); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6140ec; /* mov.l @(0xc,r15),r2 */
L8c6140ec: sa_motion_publish_stop(ctx,0x8c6140ecu); r[10]=r[8];goto L8c6140ee; /* mov r8,r10 */
L8c6140ee: sa_motion_publish_stop(ctx,0x8c6140eeu); r[2]+=r[3];goto L8c6140f0; /* add r3,r2 */
L8c6140f0: sa_motion_publish_stop(ctx,0x8c6140f0u); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c6140f2; /* mov.l r2,@(0xc,r15) */
L8c6140f2: sa_motion_publish_stop(ctx,0x8c6140f2u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6140f4; /* mov.l @r15,r1 */
L8c6140f4: sa_motion_publish_stop(ctx,0x8c6140f4u); r[1]+=4294967294u;goto L8c6140f6; /* add #-0x2,r1 */
L8c6140f6: sa_motion_publish_stop(ctx,0x8c6140f6u); wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c6140f8; /* mov.l r1,@r15 */
L8c6140f8: sa_motion_publish_stop(ctx,0x8c6140f8u); r[4]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6140fa; /* mov.w @r13+,r4 */
L8c6140fa: sa_motion_publish_stop(ctx,0x8c6140fau); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6140fc; /* mov.l @(0x4,r15),r3 */
L8c6140fc: sa_motion_publish_stop(ctx,0x8c6140fcu); r[11]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6140fe; /* mov.w @r13+,r11 */
L8c6140fe: sa_motion_publish_stop(ctx,0x8c6140feu); r[4]=r[4]&65535u;goto L8c614100; /* extu.w r4,r4 */
L8c614100: sa_motion_publish_stop(ctx,0x8c614100u); r[4]<<=2;goto L8c614102; /* shll2 r4 */
L8c614102: sa_motion_publish_stop(ctx,0x8c614102u); r[11]=r[11]&65535u;goto L8c614104; /* extu.w r11,r11 */
L8c614104: sa_motion_publish_stop(ctx,0x8c614104u); r[11]<<=2;goto L8c614106; /* shll2 r11 */
L8c614106: sa_motion_publish_stop(ctx,0x8c614106u); r[4]<<=2;goto L8c614108; /* shll2 r4 */
L8c614108: sa_motion_publish_stop(ctx,0x8c614108u); r[11]<<=2;goto L8c61410a; /* shll2 r11 */
L8c61410a: sa_motion_publish_stop(ctx,0x8c61410au); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61410c; /* shll r4 */
L8c61410c: sa_motion_publish_stop(ctx,0x8c61410cu); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61410e; /* shll r11 */
L8c61410e: sa_motion_publish_stop(ctx,0x8c61410eu); r[11]+=r[3];goto L8c614110; /* add r3,r11 */
L8c614110: sa_motion_publish_stop(ctx,0x8c614110u); r[4]+=r[3];goto L8c614112; /* add r3,r4 */
L8c614112: sa_motion_publish_stop(ctx,0x8c614112u); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c614114; /* fmov.s @(r0,r11),fr14 */
L8c614114: sa_motion_publish_stop(ctx,0x8c614114u); fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c614116; /* fmov.s @(r0,r4),fr4 */
L8c614116: sa_motion_publish_stop(ctx,0x8c614116u); r[0]=20u;goto L8c614118; /* mov #0x14,r0 */
L8c614118: sa_motion_publish_stop(ctx,0x8c614118u); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c61411a; /* fmov.s @(r0,r11),fr13 */
L8c61411a: sa_motion_publish_stop(ctx,0x8c61411au); fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c61411c; /* fmov.s @(r0,r4),fr5 */
L8c61411c: sa_motion_publish_stop(ctx,0x8c61411cu); r[12]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c61411e; /* mov.w @r13+,r12 */
L8c61411e: sa_motion_publish_stop(ctx,0x8c61411eu); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614120; /* mov.l @(0x4,r15),r3 */
L8c614120: sa_motion_publish_stop(ctx,0x8c614120u); r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c614122; /* mov.w @r13,r2 */
L8c614122: sa_motion_publish_stop(ctx,0x8c614122u); r[12]=r[12]&65535u;goto L8c614124; /* extu.w r12,r12 */
L8c614124: sa_motion_publish_stop(ctx,0x8c614124u); r[12]<<=2;goto L8c614126; /* shll2 r12 */
L8c614126: sa_motion_publish_stop(ctx,0x8c614126u); r[2]=r[2]&65535u;goto L8c614128; /* extu.w r2,r2 */
L8c614128: sa_motion_publish_stop(ctx,0x8c614128u); r[2]<<=2;goto L8c61412a; /* shll2 r2 */
L8c61412a: sa_motion_publish_stop(ctx,0x8c61412au); r[2]<<=2;goto L8c61412c; /* shll2 r2 */
L8c61412c: sa_motion_publish_stop(ctx,0x8c61412cu); r[12]<<=2;goto L8c61412e; /* shll2 r12 */
L8c61412e: sa_motion_publish_stop(ctx,0x8c61412eu); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c614130; /* shll r12 */
L8c614130: sa_motion_publish_stop(ctx,0x8c614130u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c614132; /* shll r2 */
L8c614132: sa_motion_publish_stop(ctx,0x8c614132u); r[2]+=r[3];goto L8c614134; /* add r3,r2 */
L8c614134: sa_motion_publish_stop(ctx,0x8c614134u); r[12]+=r[3];goto L8c614136; /* add r3,r12 */
L8c614136: sa_motion_publish_stop(ctx,0x8c614136u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c614138; /* pref @r2 */
L8c614138: sa_motion_publish_stop(ctx,0x8c614138u); r[0]=20u;goto L8c61413a; /* mov #0x14,r0 */
L8c61413a: sa_motion_publish_stop(ctx,0x8c61413au); fcopy(f,3u,14u);goto L8c61413c; /* fmov fr14,fr3 */
L8c61413c: sa_motion_publish_stop(ctx,0x8c61413cu); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61413e; /* fmov.s @(r0,r12),fr2 */
L8c61413e: sa_motion_publish_stop(ctx,0x8c61413eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614140; /* fsub fr4,fr3 */
L8c614140: sa_motion_publish_stop(ctx,0x8c614140u); r[0]=16u;goto L8c614142; /* mov #0x10,r0 */
L8c614142: sa_motion_publish_stop(ctx,0x8c614142u); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c614144; /* cmp/hs r8,r10 */
L8c614144: sa_motion_publish_stop(ctx,0x8c614144u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614146; /* fsub fr13,fr2 */
L8c614146: sa_motion_publish_stop(ctx,0x8c614146u); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c614148; /* fmov.s @(r0,r12),fr1 */
L8c614148: sa_motion_publish_stop(ctx,0x8c614148u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61414a; /* fsub fr14,fr1 */
L8c61414a: sa_motion_publish_stop(ctx,0x8c61414au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61414c; /* fmul fr2,fr3 */
L8c61414c: sa_motion_publish_stop(ctx,0x8c61414cu); fcopy(f,2u,13u);goto L8c61414e; /* fmov fr13,fr2 */
L8c61414e: sa_motion_publish_stop(ctx,0x8c61414eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614150; /* fsub fr5,fr2 */
L8c614150: sa_motion_publish_stop(ctx,0x8c614150u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614152; /* fmul fr1,fr2 */
L8c614152: sa_motion_publish_stop(ctx,0x8c614152u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614154; /* fsub fr2,fr3 */
L8c614154: sa_motion_publish_stop(ctx,0x8c614154u); fcopy(f,4u,3u);goto L8c614156; /* fmov fr3,fr4 */
L8c614156: sa_motion_publish_stop(ctx,0x8c614158u); take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c6141c0;goto L8c61415a; /* bt/s 0x8c6141c0 */
L8c61415a: sa_motion_publish_stop(ctx,0x8c61415au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61415c; /* fldi0 fr3 */
L8c61415c: sa_motion_publish_stop(ctx,0x8c61415cu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c61415e; /* fcmp/gt fr4,fr3 */
L8c61415e: sa_motion_publish_stop(ctx,0x8c61415eu); if((*sr&1u))goto L8c61420e;goto L8c614160; /* bt 0x8c61420e */
L8c614160: sa_motion_publish_stop(ctx,0x8c614160u); r[6]=r[14];goto L8c614162; /* mov r14,r6 */
L8c614162: sa_motion_publish_stop(ctx,0x8c614162u); r[4]=r[11];goto L8c614164; /* mov r11,r4 */
L8c614164: sa_motion_publish_stop(ctx,0x8c614164u); r[5]=r[9];goto L8c614166; /* mov r9,r5 */
L8c614166: sa_motion_publish_stop(ctx,0x8c614166u); r[14]+=4u;goto L8c614168; /* add #0x4,r14 */
L8c614168: sa_motion_publish_stop(ctx,0x8c614168u); r[8]+=16u;goto L8c61416a; /* add #0x10,r8 */
L8c61416a: sa_motion_publish_stop(ctx,0x8c61416au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61416c; /* fschg */
L8c61416c: sa_motion_publish_stop(ctx,0x8c61416cu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61416e; /* fmov.s @r4+,fr0 */
L8c61416e: sa_motion_publish_stop(ctx,0x8c61416eu); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614170; /* fmov.s @r4+,fr2 */
L8c614170: sa_motion_publish_stop(ctx,0x8c614170u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614172; /* fldi0 fr3 */
L8c614172: sa_motion_publish_stop(ctx,0x8c614172u); fcopy(f,8u,1u);goto L8c614174; /* fmov fr1,fr8 */
L8c614174: sa_motion_publish_stop(ctx,0x8c614174u); fcopy(f,10u,3u);goto L8c614176; /* fmov fr3,fr10 */
L8c614176: sa_motion_publish_stop(ctx,0x8c614176u); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c614178; /* fipr fv8,fv0 */
L8c614178: sa_motion_publish_stop(ctx,0x8c614178u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61417a; /* fmov.s @r4+,fr4 */
L8c61417a: sa_motion_publish_stop(ctx,0x8c61417au); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61417c; /* fmov.s @r4,fr6 */
L8c61417c: sa_motion_publish_stop(ctx,0x8c61417cu); fcopy(f,8u,5u);goto L8c61417e; /* fmov fr5,fr8 */
L8c61417e: sa_motion_publish_stop(ctx,0x8c61417eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614180; /* fschg */
L8c614180: sa_motion_publish_stop(ctx,0x8c614180u); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c614182; /* mov.w @r6+,r2 */
L8c614182: sa_motion_publish_stop(ctx,0x8c614182u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c614184; /* mov.w @r6,r3 */
L8c614184: sa_motion_publish_stop(ctx,0x8c614184u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614186; /* fmov.s fr6,@-r8 */
L8c614186: sa_motion_publish_stop(ctx,0x8c614186u); *fpul=r[3];goto L8c614188; /* lds r3,FPUL */
L8c614188: sa_motion_publish_stop(ctx,0x8c614188u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61418a; /* fmov.s fr5,@-r8 */
L8c61418a: sa_motion_publish_stop(ctx,0x8c61418au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c61418c; /* fsub fr3,fr11 */
L8c61418c: sa_motion_publish_stop(ctx,0x8c61418cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61418e; /* fmov.s fr4,@-r8 */
L8c61418e: sa_motion_publish_stop(ctx,0x8c61418eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c614190; /* float FPUL,fr1 */
L8c614190: sa_motion_publish_stop(ctx,0x8c614190u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c614192; /* fmul fr8,fr1 */
L8c614192: sa_motion_publish_stop(ctx,0x8c614192u); *fpul=r[2];goto L8c614194; /* lds r2,FPUL */
L8c614194: sa_motion_publish_stop(ctx,0x8c614194u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614196; /* mov.l r5,@-r8 */
L8c614196: sa_motion_publish_stop(ctx,0x8c614196u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c614198; /* float FPUL,fr2 */
L8c614198: sa_motion_publish_stop(ctx,0x8c614198u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61419a; /* fmul fr8,fr2 */
L8c61419a: sa_motion_publish_stop(ctx,0x8c61419au); r[8]+=28u;goto L8c61419c; /* add #0x1c,r8 */
L8c61419c: sa_motion_publish_stop(ctx,0x8c61419cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c61419e; /* fmov.s fr11,@-r8 */
L8c61419e: sa_motion_publish_stop(ctx,0x8c61419eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6141a0; /* fmov.s fr1,@-r8 */
L8c6141a0: sa_motion_publish_stop(ctx,0x8c6141a0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6141a2; /* fmov.s fr2,@-r8 */
L8c6141a2: sa_motion_publish_stop(ctx,0x8c6141a2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6141a4; /* pref @r8 */
L8c6141a4: sa_motion_publish_stop(ctx,0x8c6141a4u); r[8]+=16u;goto L8c6141a6; /* add #0x10,r8 */
L8c6141a6: sa_motion_publish_stop(ctx,0x8c6141a6u); r[3]=r[8];goto L8c6141a8; /* mov r8,r3 */
L8c6141a8: sa_motion_publish_stop(ctx,0x8c6141a8u); r[3]-=r[10];goto L8c6141aa; /* sub r10,r3 */
L8c6141aa: sa_motion_publish_stop(ctx,0x8c6141aau); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6141ac; /* mov.l @(0x8,r15),r2 */
L8c6141ac: sa_motion_publish_stop(ctx,0x8c6141acu); r[3]+=32u;goto L8c6141ae; /* add #0x20,r3 */
L8c6141ae: sa_motion_publish_stop(ctx,0x8c6141aeu); r[14]+=4294967292u;goto L8c6141b0; /* add #-0x4,r14 */
L8c6141b0: sa_motion_publish_stop(ctx,0x8c6141b0u); r[2]+=r[3];goto L8c6141b2; /* add r3,r2 */
L8c6141b2: sa_motion_publish_stop(ctx,0x8c6141b2u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6141b4; /* mov.l r2,@(0x8,r15) */
L8c6141b4: sa_motion_publish_stop(ctx,0x8c6141b6u); r[10]=r[8];goto L8c614254; /* bra 0x8c614254 */
L8c6141c0: sa_motion_publish_stop(ctx,0x8c6141c0u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6141c2; /* fldi0 fr3 */
L8c6141c2: sa_motion_publish_stop(ctx,0x8c6141c2u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6141c4; /* fcmp/gt fr4,fr3 */
L8c6141c4: sa_motion_publish_stop(ctx,0x8c6141c6u); take=!(*sr&1u);r[14]+=4u;if(take)goto L8c614254;goto L8c6141c8; /* bf/s 0x8c614254 */
L8c6141c8: sa_motion_publish_stop(ctx,0x8c6141c8u); r[14]+=4294967292u;goto L8c6141ca; /* add #-0x4,r14 */
L8c6141ca: sa_motion_publish_stop(ctx,0x8c6141cau); r[5]=rd(m,bus,&io_status,0x8c614348u,4);if(io_status)return io_status;goto L8c6141cc; /* mov.l 0x8c614348,r5 */
L8c6141cc: sa_motion_publish_stop(ctx,0x8c6141ccu); r[6]=r[14];goto L8c6141ce; /* mov r14,r6 */
L8c6141ce: sa_motion_publish_stop(ctx,0x8c6141ceu); r[14]+=4u;goto L8c6141d0; /* add #0x4,r14 */
L8c6141d0: sa_motion_publish_stop(ctx,0x8c6141d0u); r[8]+=16u;goto L8c6141d2; /* add #0x10,r8 */
L8c6141d2: sa_motion_publish_stop(ctx,0x8c6141d2u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6141d4; /* fschg */
L8c6141d4: sa_motion_publish_stop(ctx,0x8c6141d4u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6141d6; /* fmov.s @r4+,fr0 */
L8c6141d6: sa_motion_publish_stop(ctx,0x8c6141d6u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6141d8; /* fmov.s @r4+,fr2 */
L8c6141d8: sa_motion_publish_stop(ctx,0x8c6141d8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6141da; /* fldi0 fr3 */
L8c6141da: sa_motion_publish_stop(ctx,0x8c6141dau); fcopy(f,8u,1u);goto L8c6141dc; /* fmov fr1,fr8 */
L8c6141dc: sa_motion_publish_stop(ctx,0x8c6141dcu); fcopy(f,10u,3u);goto L8c6141de; /* fmov fr3,fr10 */
L8c6141de: sa_motion_publish_stop(ctx,0x8c6141deu); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c6141e0; /* fipr fv8,fv0 */
L8c6141e0: sa_motion_publish_stop(ctx,0x8c6141e0u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6141e2; /* fmov.s @r4+,fr4 */
L8c6141e2: sa_motion_publish_stop(ctx,0x8c6141e2u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6141e4; /* fmov.s @r4,fr6 */
L8c6141e4: sa_motion_publish_stop(ctx,0x8c6141e4u); fcopy(f,8u,5u);goto L8c6141e6; /* fmov fr5,fr8 */
L8c6141e6: sa_motion_publish_stop(ctx,0x8c6141e6u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6141e8; /* fschg */
L8c6141e8: sa_motion_publish_stop(ctx,0x8c6141e8u); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c6141ea; /* mov.w @r6+,r2 */
L8c6141ea: sa_motion_publish_stop(ctx,0x8c6141eau); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c6141ec; /* mov.w @r6,r3 */
L8c6141ec: sa_motion_publish_stop(ctx,0x8c6141ecu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6141ee; /* fmov.s fr6,@-r8 */
L8c6141ee: sa_motion_publish_stop(ctx,0x8c6141eeu); *fpul=r[3];goto L8c6141f0; /* lds r3,FPUL */
L8c6141f0: sa_motion_publish_stop(ctx,0x8c6141f0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6141f2; /* fmov.s fr5,@-r8 */
L8c6141f2: sa_motion_publish_stop(ctx,0x8c6141f2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c6141f4; /* fsub fr3,fr11 */
L8c6141f4: sa_motion_publish_stop(ctx,0x8c6141f4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6141f6; /* fmov.s fr4,@-r8 */
L8c6141f6: sa_motion_publish_stop(ctx,0x8c6141f6u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c6141f8; /* float FPUL,fr1 */
L8c6141f8: sa_motion_publish_stop(ctx,0x8c6141f8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6141fa; /* fmul fr8,fr1 */
L8c6141fa: sa_motion_publish_stop(ctx,0x8c6141fau); *fpul=r[2];goto L8c6141fc; /* lds r2,FPUL */
L8c6141fc: sa_motion_publish_stop(ctx,0x8c6141fcu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6141fe; /* mov.l r5,@-r8 */
L8c6141fe: sa_motion_publish_stop(ctx,0x8c6141feu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c614200; /* float FPUL,fr2 */
L8c614200: sa_motion_publish_stop(ctx,0x8c614200u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614202; /* fmul fr8,fr2 */
L8c614202: sa_motion_publish_stop(ctx,0x8c614202u); r[8]+=28u;goto L8c614204; /* add #0x1c,r8 */
L8c614204: sa_motion_publish_stop(ctx,0x8c614204u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c614206; /* fmov.s fr11,@-r8 */
L8c614206: sa_motion_publish_stop(ctx,0x8c614206u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c614208; /* fmov.s fr1,@-r8 */
L8c614208: sa_motion_publish_stop(ctx,0x8c614208u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61420a; /* fmov.s fr2,@-r8 */
L8c61420a: sa_motion_publish_stop(ctx,0x8c61420au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61420c; /* pref @r8 */
L8c61420c: sa_motion_publish_stop(ctx,0x8c61420cu); r[8]+=16u;goto L8c61420e; /* add #0x10,r8 */
L8c61420e: sa_motion_publish_stop(ctx,0x8c61420eu); r[5]=rd(m,bus,&io_status,0x8c614348u,4);if(io_status)return io_status;goto L8c614210; /* mov.l 0x8c614348,r5 */
L8c614210: sa_motion_publish_stop(ctx,0x8c614210u); r[6]=r[14];goto L8c614212; /* mov r14,r6 */
L8c614212: sa_motion_publish_stop(ctx,0x8c614212u); r[4]=r[11];goto L8c614214; /* mov r11,r4 */
L8c614214: sa_motion_publish_stop(ctx,0x8c614214u); r[14]+=4u;goto L8c614216; /* add #0x4,r14 */
L8c614216: sa_motion_publish_stop(ctx,0x8c614216u); r[8]+=16u;goto L8c614218; /* add #0x10,r8 */
L8c614218: sa_motion_publish_stop(ctx,0x8c614218u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61421a; /* fschg */
L8c61421a: sa_motion_publish_stop(ctx,0x8c61421au); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61421c; /* fmov.s @r4+,fr0 */
L8c61421c: sa_motion_publish_stop(ctx,0x8c61421cu); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61421e; /* fmov.s @r4+,fr2 */
L8c61421e: sa_motion_publish_stop(ctx,0x8c61421eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614220; /* fldi0 fr3 */
L8c614220: sa_motion_publish_stop(ctx,0x8c614220u); fcopy(f,8u,1u);goto L8c614222; /* fmov fr1,fr8 */
L8c614222: sa_motion_publish_stop(ctx,0x8c614222u); fcopy(f,10u,3u);goto L8c614224; /* fmov fr3,fr10 */
L8c614224: sa_motion_publish_stop(ctx,0x8c614224u); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c614226; /* fipr fv8,fv0 */
L8c614226: sa_motion_publish_stop(ctx,0x8c614226u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614228; /* fmov.s @r4+,fr4 */
L8c614228: sa_motion_publish_stop(ctx,0x8c614228u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61422a; /* fmov.s @r4,fr6 */
L8c61422a: sa_motion_publish_stop(ctx,0x8c61422au); fcopy(f,8u,5u);goto L8c61422c; /* fmov fr5,fr8 */
L8c61422c: sa_motion_publish_stop(ctx,0x8c61422cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61422e; /* fschg */
L8c61422e: sa_motion_publish_stop(ctx,0x8c61422eu); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c614230; /* mov.w @r6+,r2 */
L8c614230: sa_motion_publish_stop(ctx,0x8c614230u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c614232; /* mov.w @r6,r3 */
L8c614232: sa_motion_publish_stop(ctx,0x8c614232u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614234; /* fmov.s fr6,@-r8 */
L8c614234: sa_motion_publish_stop(ctx,0x8c614234u); *fpul=r[3];goto L8c614236; /* lds r3,FPUL */
L8c614236: sa_motion_publish_stop(ctx,0x8c614236u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614238; /* fmov.s fr5,@-r8 */
L8c614238: sa_motion_publish_stop(ctx,0x8c614238u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c61423a; /* fsub fr3,fr11 */
L8c61423a: sa_motion_publish_stop(ctx,0x8c61423au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61423c; /* fmov.s fr4,@-r8 */
L8c61423c: sa_motion_publish_stop(ctx,0x8c61423cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c61423e; /* float FPUL,fr1 */
L8c61423e: sa_motion_publish_stop(ctx,0x8c61423eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c614240; /* fmul fr8,fr1 */
L8c614240: sa_motion_publish_stop(ctx,0x8c614240u); *fpul=r[2];goto L8c614242; /* lds r2,FPUL */
L8c614242: sa_motion_publish_stop(ctx,0x8c614242u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614244; /* mov.l r5,@-r8 */
L8c614244: sa_motion_publish_stop(ctx,0x8c614244u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c614246; /* float FPUL,fr2 */
L8c614246: sa_motion_publish_stop(ctx,0x8c614246u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614248; /* fmul fr8,fr2 */
L8c614248: sa_motion_publish_stop(ctx,0x8c614248u); r[8]+=28u;goto L8c61424a; /* add #0x1c,r8 */
L8c61424a: sa_motion_publish_stop(ctx,0x8c61424au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c61424c; /* fmov.s fr11,@-r8 */
L8c61424c: sa_motion_publish_stop(ctx,0x8c61424cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61424e; /* fmov.s fr1,@-r8 */
L8c61424e: sa_motion_publish_stop(ctx,0x8c61424eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614250; /* fmov.s fr2,@-r8 */
L8c614250: sa_motion_publish_stop(ctx,0x8c614250u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614252; /* pref @r8 */
L8c614252: sa_motion_publish_stop(ctx,0x8c614252u); r[8]+=16u;goto L8c614254; /* add #0x10,r8 */
L8c614254: sa_motion_publish_stop(ctx,0x8c614254u); fcopy(f,4u,14u);goto L8c614256; /* fmov fr14,fr4 */
L8c614256: sa_motion_publish_stop(ctx,0x8c614256u); r[0]=16u;goto L8c614258; /* mov #0x10,r0 */
L8c614258: sa_motion_publish_stop(ctx,0x8c614258u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61425a; /* mov.l @r15,r3 */
L8c61425a: sa_motion_publish_stop(ctx,0x8c61425au); r[4]=r[11];goto L8c61425c; /* mov r11,r4 */
L8c61425c: sa_motion_publish_stop(ctx,0x8c61425cu); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c61425e; /* fmov.s @(r0,r12),fr14 */
L8c61425e: sa_motion_publish_stop(ctx,0x8c61425eu); r[0]=20u;goto L8c614260; /* mov #0x14,r0 */
L8c614260: sa_motion_publish_stop(ctx,0x8c614260u); fcopy(f,5u,13u);goto L8c614262; /* fmov fr13,fr5 */
L8c614262: sa_motion_publish_stop(ctx,0x8c614262u); r[3]+=4294967295u;goto L8c614264; /* add #-0x1,r3 */
L8c614264: sa_motion_publish_stop(ctx,0x8c614264u); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c614266; /* fmov.s @(r0,r12),fr13 */
L8c614266: sa_motion_publish_stop(ctx,0x8c614266u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614268; /* tst r3,r3 */
L8c614268: sa_motion_publish_stop(ctx,0x8c614268u); r[11]=r[12];goto L8c61426a; /* mov r12,r11 */
L8c61426a: sa_motion_publish_stop(ctx,0x8c61426au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c61426c; /* fneg fr15 */
L8c61426c: sa_motion_publish_stop(ctx,0x8c61426eu); take=(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c614274;goto L8c614270; /* bt/s 0x8c614274 */
L8c614270: sa_motion_publish_stop(ctx,0x8c614272u); goto L8c61411c; /* bra 0x8c61411c */
L8c614274: sa_motion_publish_stop(ctx,0x8c614274u); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c614276; /* cmp/hs r8,r10 */
L8c614276: sa_motion_publish_stop(ctx,0x8c614278u); take=(*sr&1u);r[14]+=8u;if(take)goto L8c6142ce;goto L8c61427a; /* bt/s 0x8c6142ce */
L8c61427a: sa_motion_publish_stop(ctx,0x8c61427au); r[14]+=4294967288u;goto L8c61427c; /* add #-0x8,r14 */
L8c61427c: sa_motion_publish_stop(ctx,0x8c61427cu); r[4]=r[12];goto L8c61427e; /* mov r12,r4 */
L8c61427e: sa_motion_publish_stop(ctx,0x8c61427eu); r[6]=r[14];goto L8c614280; /* mov r14,r6 */
L8c614280: sa_motion_publish_stop(ctx,0x8c614280u); r[5]=r[9];goto L8c614282; /* mov r9,r5 */
L8c614282: sa_motion_publish_stop(ctx,0x8c614282u); r[14]+=4u;goto L8c614284; /* add #0x4,r14 */
L8c614284: sa_motion_publish_stop(ctx,0x8c614284u); r[8]+=16u;goto L8c614286; /* add #0x10,r8 */
L8c614286: sa_motion_publish_stop(ctx,0x8c614286u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614288; /* fschg */
L8c614288: sa_motion_publish_stop(ctx,0x8c614288u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61428a; /* fmov.s @r4+,fr0 */
L8c61428a: sa_motion_publish_stop(ctx,0x8c61428au); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61428c; /* fmov.s @r4+,fr2 */
L8c61428c: sa_motion_publish_stop(ctx,0x8c61428cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61428e; /* fldi0 fr3 */
L8c61428e: sa_motion_publish_stop(ctx,0x8c61428eu); fcopy(f,8u,1u);goto L8c614290; /* fmov fr1,fr8 */
L8c614290: sa_motion_publish_stop(ctx,0x8c614290u); fcopy(f,10u,3u);goto L8c614292; /* fmov fr3,fr10 */
L8c614292: sa_motion_publish_stop(ctx,0x8c614292u); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c614294; /* fipr fv8,fv0 */
L8c614294: sa_motion_publish_stop(ctx,0x8c614294u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614296; /* fmov.s @r4+,fr4 */
L8c614296: sa_motion_publish_stop(ctx,0x8c614296u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614298; /* fmov.s @r4,fr6 */
L8c614298: sa_motion_publish_stop(ctx,0x8c614298u); fcopy(f,8u,5u);goto L8c61429a; /* fmov fr5,fr8 */
L8c61429a: sa_motion_publish_stop(ctx,0x8c61429au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61429c; /* fschg */
L8c61429c: sa_motion_publish_stop(ctx,0x8c61429cu); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c61429e; /* mov.w @r6+,r2 */
L8c61429e: sa_motion_publish_stop(ctx,0x8c61429eu); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c6142a0; /* mov.w @r6,r3 */
L8c6142a0: sa_motion_publish_stop(ctx,0x8c6142a0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6142a2; /* fmov.s fr6,@-r8 */
L8c6142a2: sa_motion_publish_stop(ctx,0x8c6142a2u); *fpul=r[3];goto L8c6142a4; /* lds r3,FPUL */
L8c6142a4: sa_motion_publish_stop(ctx,0x8c6142a4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6142a6; /* fmov.s fr5,@-r8 */
L8c6142a6: sa_motion_publish_stop(ctx,0x8c6142a6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c6142a8; /* fsub fr3,fr11 */
L8c6142a8: sa_motion_publish_stop(ctx,0x8c6142a8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6142aa; /* fmov.s fr4,@-r8 */
L8c6142aa: sa_motion_publish_stop(ctx,0x8c6142aau); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c6142ac; /* float FPUL,fr1 */
L8c6142ac: sa_motion_publish_stop(ctx,0x8c6142acu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6142ae; /* fmul fr8,fr1 */
L8c6142ae: sa_motion_publish_stop(ctx,0x8c6142aeu); *fpul=r[2];goto L8c6142b0; /* lds r2,FPUL */
L8c6142b0: sa_motion_publish_stop(ctx,0x8c6142b0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6142b2; /* mov.l r5,@-r8 */
L8c6142b2: sa_motion_publish_stop(ctx,0x8c6142b2u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c6142b4; /* float FPUL,fr2 */
L8c6142b4: sa_motion_publish_stop(ctx,0x8c6142b4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6142b6; /* fmul fr8,fr2 */
L8c6142b6: sa_motion_publish_stop(ctx,0x8c6142b6u); r[8]+=28u;goto L8c6142b8; /* add #0x1c,r8 */
L8c6142b8: sa_motion_publish_stop(ctx,0x8c6142b8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c6142ba; /* fmov.s fr11,@-r8 */
L8c6142ba: sa_motion_publish_stop(ctx,0x8c6142bau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6142bc; /* fmov.s fr1,@-r8 */
L8c6142bc: sa_motion_publish_stop(ctx,0x8c6142bcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6142be; /* fmov.s fr2,@-r8 */
L8c6142be: sa_motion_publish_stop(ctx,0x8c6142beu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6142c0; /* pref @r8 */
L8c6142c0: sa_motion_publish_stop(ctx,0x8c6142c0u); r[8]+=16u;goto L8c6142c2; /* add #0x10,r8 */
L8c6142c2: sa_motion_publish_stop(ctx,0x8c6142c2u); r[3]=r[8];goto L8c6142c4; /* mov r8,r3 */
L8c6142c4: sa_motion_publish_stop(ctx,0x8c6142c4u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6142c6; /* mov.l @(0x8,r15),r2 */
L8c6142c6: sa_motion_publish_stop(ctx,0x8c6142c6u); r[3]-=r[10];goto L8c6142c8; /* sub r10,r3 */
L8c6142c8: sa_motion_publish_stop(ctx,0x8c6142c8u); r[3]+=32u;goto L8c6142ca; /* add #0x20,r3 */
L8c6142ca: sa_motion_publish_stop(ctx,0x8c6142cau); r[2]+=r[3];goto L8c6142cc; /* add r3,r2 */
L8c6142cc: sa_motion_publish_stop(ctx,0x8c6142ccu); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6142ce; /* mov.l r2,@(0x8,r15) */
L8c6142ce: sa_motion_publish_stop(ctx,0x8c6142ceu); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c6142d0; /* mov.l @(0x10,r15),r3 */
L8c6142d0: sa_motion_publish_stop(ctx,0x8c6142d0u); r[3]+=4294967295u;goto L8c6142d2; /* add #-0x1,r3 */
L8c6142d2: sa_motion_publish_stop(ctx,0x8c6142d2u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6142d4; /* tst r3,r3 */
L8c6142d4: sa_motion_publish_stop(ctx,0x8c6142d6u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;if(take)goto L8c6142dc;goto L8c6142d8; /* bt/s 0x8c6142dc */
L8c6142d8: sa_motion_publish_stop(ctx,0x8c6142dau); goto L8c6140c8; /* bra 0x8c6140c8 */
L8c6142dc: sa_motion_publish_stop(ctx,0x8c6142dcu); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6142de; /* mov.l @(0x8,r15),r3 */
L8c6142de: sa_motion_publish_stop(ctx,0x8c6142deu); r[4]=rd(m,bus,&io_status,0x8c61434cu,4);if(io_status)return io_status;goto L8c6142e0; /* mov.l 0x8c61434c,r4 */
L8c6142e0: sa_motion_publish_stop(ctx,0x8c6142e0u); r[3]>>=2;goto L8c6142e2; /* shlr2 r3 */
L8c6142e2: sa_motion_publish_stop(ctx,0x8c6142e2u); r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6142e4; /* mov.l @r4,r1 */
L8c6142e4: sa_motion_publish_stop(ctx,0x8c6142e4u); r[3]>>=2;goto L8c6142e6; /* shlr2 r3 */
L8c6142e6: sa_motion_publish_stop(ctx,0x8c6142e6u); *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c6142e8; /* shlr r3 */
L8c6142e8: sa_motion_publish_stop(ctx,0x8c6142e8u); r[1]+=r[3];goto L8c6142ea; /* add r3,r1 */
L8c6142ea: sa_motion_publish_stop(ctx,0x8c6142eau); wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c6142ec; /* mov.l r1,@r4 */
L8c6142ec: sa_motion_publish_stop(ctx,0x8c6142ecu); r[4]=rd(m,bus,&io_status,0x8c614350u,4);if(io_status)return io_status;goto L8c6142ee; /* mov.l 0x8c614350,r4 */
L8c6142ee: sa_motion_publish_stop(ctx,0x8c6142eeu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6142f0; /* mov.l @(0xc,r15),r3 */
L8c6142f0: sa_motion_publish_stop(ctx,0x8c6142f0u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c6142f2; /* mov.l @r4,r2 */
L8c6142f2: sa_motion_publish_stop(ctx,0x8c6142f2u); r[2]+=r[3];goto L8c6142f4; /* add r3,r2 */
L8c6142f4: sa_motion_publish_stop(ctx,0x8c6142f4u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c6142f6; /* mov.l r2,@r4 */
L8c6142f6: sa_motion_publish_stop(ctx,0x8c6142f6u); r[15]+=20u;goto L8c6142f8; /* add #0x14,r15 */
L8c6142f8: sa_motion_publish_stop(ctx,0x8c6142f8u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6142fa; /* lds.l @r15+,PR */
L8c6142fa: sa_motion_publish_stop(ctx,0x8c6142fau); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6142fc; /* fmov.s @r15+,fr13 */
L8c6142fc: sa_motion_publish_stop(ctx,0x8c6142fcu); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6142fe; /* fmov.s @r15+,fr14 */
L8c6142fe: sa_motion_publish_stop(ctx,0x8c6142feu); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614300; /* fmov.s @r15+,fr15 */
L8c614300: sa_motion_publish_stop(ctx,0x8c614300u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614302; /* mov.l @r15+,r9 */
L8c614302: sa_motion_publish_stop(ctx,0x8c614302u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614304; /* mov.l @r15+,r10 */
L8c614304: sa_motion_publish_stop(ctx,0x8c614304u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614306; /* mov.l @r15+,r11 */
L8c614306: sa_motion_publish_stop(ctx,0x8c614306u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614308; /* mov.l @r15+,r12 */
L8c614308: sa_motion_publish_stop(ctx,0x8c614308u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61430a; /* mov.l @r15+,r13 */
L8c61430a: sa_motion_publish_stop(ctx,0x8c61430cu); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_uv_face_alternate_strip_clipped_8c615b84(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c615b84;
L8c615b84: sa_motion_publish_stop(ctx,0x8c615b84u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c615b86; /* mov.l r14,@-r15 */
L8c615b86: sa_motion_publish_stop(ctx,0x8c615b86u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c615b88; /* mov.l r13,@-r15 */
L8c615b88: sa_motion_publish_stop(ctx,0x8c615b88u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c615b8a; /* mov.l r12,@-r15 */
L8c615b8a: sa_motion_publish_stop(ctx,0x8c615b8au); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c615b8c; /* mov.l r11,@-r15 */
L8c615b8c: sa_motion_publish_stop(ctx,0x8c615b8cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c615b8e; /* mov.l r10,@-r15 */
L8c615b8e: sa_motion_publish_stop(ctx,0x8c615b8eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c615b90; /* mov.l r9,@-r15 */
L8c615b90: sa_motion_publish_stop(ctx,0x8c615b90u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c615b92; /* fmov.s fr15,@-r15 */
L8c615b92: sa_motion_publish_stop(ctx,0x8c615b92u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c615b94; /* fmov.s fr14,@-r15 */
L8c615b94: sa_motion_publish_stop(ctx,0x8c615b94u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c615b96; /* fmov.s fr13,@-r15 */
L8c615b96: sa_motion_publish_stop(ctx,0x8c615b96u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c615b98; /* fmov.s fr12,@-r15 */
L8c615b98: sa_motion_publish_stop(ctx,0x8c615b98u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c615b9a; /* sts.l PR,@-r15 */
L8c615b9a: sa_motion_publish_stop(ctx,0x8c615b9au); r[15]+=4294967216u;goto L8c615b9c; /* add #-0x50,r15 */
L8c615b9c: sa_motion_publish_stop(ctx,0x8c615b9cu); r[3]=rd(m,bus,&io_status,0x8c615be4u,4);if(io_status)return io_status;goto L8c615b9e; /* mov.l 0x8c615be4,r3 */
L8c615b9e: sa_motion_publish_stop(ctx,0x8c615b9eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c615ba0; /* fldi1 fr12 */
L8c615ba0: sa_motion_publish_stop(ctx,0x8c615ba2u); target=r[3];*pr=0x8c615ba4u;r[12]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c615ba4; /* jsr @r3 */
L8c615ba4: sa_motion_publish_stop(ctx,0x8c615ba4u); r[2]=rd(m,bus,&io_status,0x8c615be8u,4);if(io_status)return io_status;goto L8c615ba6; /* mov.l 0x8c615be8,r2 */
L8c615ba6: sa_motion_publish_stop(ctx,0x8c615ba6u); r[1]=0u;goto L8c615ba8; /* mov #0x0,r1 */
L8c615ba8: sa_motion_publish_stop(ctx,0x8c615ba8u); r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c615baa; /* mov.w @(0x2,r12),r0 */
L8c615baa: sa_motion_publish_stop(ctx,0x8c615baau); r[13]=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;goto L8c615bac; /* mov.l @(0x4,r12),r13 */
L8c615bac: sa_motion_publish_stop(ctx,0x8c615bacu); r[14]=rd(m,bus,&io_status,r[12]+0x14u,4);if(io_status)return io_status;goto L8c615bae; /* mov.l @(0x14,r12),r14 */
L8c615bae: sa_motion_publish_stop(ctx,0x8c615baeu); r[0]=r[0]&65535u;goto L8c615bb0; /* extu.w r0,r0 */
L8c615bb0: sa_motion_publish_stop(ctx,0x8c615bb0u); wr(m,bus,&io_status,r[15]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c615bb2; /* mov.l r0,@(0x1c,r15) */
L8c615bb2: sa_motion_publish_stop(ctx,0x8c615bb2u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c615bb4; /* mov.l @r2,r3 */
L8c615bb4: sa_motion_publish_stop(ctx,0x8c615bb4u); wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c615bb6; /* mov.l r3,@(0xc,r15) */
L8c615bb6: sa_motion_publish_stop(ctx,0x8c615bb6u); wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c615bb8; /* mov.l r1,@(0x10,r15) */
L8c615bb8: sa_motion_publish_stop(ctx,0x8c615bbau); goto L8c615bec; /* bra 0x8c615bec */
L8c615bec: sa_motion_publish_stop(ctx,0x8c615becu); r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c615bee; /* mov.w @r13+,r3 */
L8c615bee: sa_motion_publish_stop(ctx,0x8c615beeu); r[4]=rd(m,bus,&io_status,0x8c615d0cu,4);if(io_status)return io_status;goto L8c615bf0; /* mov.l 0x8c615d0c,r4 */
L8c615bf0: sa_motion_publish_stop(ctx,0x8c615bf0u); r[3]=r[3]&65535u;goto L8c615bf2; /* extu.w r3,r3 */
L8c615bf2: sa_motion_publish_stop(ctx,0x8c615bf2u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c615bf4; /* tst r4,r3 */
L8c615bf4: sa_motion_publish_stop(ctx,0x8c615bf4u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c615bf6; /* mov.l r3,@(0x8,r15) */
L8c615bf6: sa_motion_publish_stop(ctx,0x8c615bf8u); take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c615c08;goto L8c615bfa; /* bt/s 0x8c615c08 */
L8c615bfa: sa_motion_publish_stop(ctx,0x8c615bfau); r[4]=sx(m,bus,&io_status,0x8c615d08u,2);if(io_status)return io_status;goto L8c615bfc; /* mov.w 0x8c615d08,r4 */
L8c615bfc: sa_motion_publish_stop(ctx,0x8c615bfcu); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615bfe; /* mov.l @(0x8,r15),r3 */
L8c615bfe: sa_motion_publish_stop(ctx,0x8c615bfeu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c615c00; /* fldi1 fr4 */
L8c615c00: sa_motion_publish_stop(ctx,0x8c615c00u); r[3]&=r[4];goto L8c615c02; /* and r4,r3 */
L8c615c02: sa_motion_publish_stop(ctx,0x8c615c02u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c615c04; /* fneg fr4 */
L8c615c04: sa_motion_publish_stop(ctx,0x8c615c04u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c615c06; /* mov.l r3,@(0x8,r15) */
L8c615c06: sa_motion_publish_stop(ctx,0x8c615c06u); fcopy(f,15u,4u);goto L8c615c08; /* fmov fr4,fr15 */
L8c615c08: sa_motion_publish_stop(ctx,0x8c615c08u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615c0a; /* mov.l @(0x8,r15),r3 */
L8c615c0a: sa_motion_publish_stop(ctx,0x8c615c0au); r[0]=16u;goto L8c615c0c; /* mov #0x10,r0 */
L8c615c0c: sa_motion_publish_stop(ctx,0x8c615c0cu); r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c615c0e; /* mov.l @(0x10,r15),r2 */
L8c615c0e: sa_motion_publish_stop(ctx,0x8c615c0eu); r[9]=r[8];goto L8c615c10; /* mov r8,r9 */
L8c615c10: sa_motion_publish_stop(ctx,0x8c615c10u); r[2]+=r[3];goto L8c615c12; /* add r3,r2 */
L8c615c12: sa_motion_publish_stop(ctx,0x8c615c12u); wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c615c14; /* mov.l r2,@(0x10,r15) */
L8c615c14: sa_motion_publish_stop(ctx,0x8c615c14u); r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615c16; /* mov.l @(0x8,r15),r1 */
L8c615c16: sa_motion_publish_stop(ctx,0x8c615c16u); r[1]+=4294967294u;goto L8c615c18; /* add #-0x2,r1 */
L8c615c18: sa_motion_publish_stop(ctx,0x8c615c18u); wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c615c1a; /* mov.l r1,@(0x8,r15) */
L8c615c1a: sa_motion_publish_stop(ctx,0x8c615c1au); r[10]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c615c1c; /* mov.w @r13+,r10 */
L8c615c1c: sa_motion_publish_stop(ctx,0x8c615c1cu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c615c1e; /* mov.l @(0xc,r15),r3 */
L8c615c1e: sa_motion_publish_stop(ctx,0x8c615c1eu); r[11]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c615c20; /* mov.w @r13+,r11 */
L8c615c20: sa_motion_publish_stop(ctx,0x8c615c20u); r[10]=r[10]&65535u;goto L8c615c22; /* extu.w r10,r10 */
L8c615c22: sa_motion_publish_stop(ctx,0x8c615c22u); r[10]<<=2;goto L8c615c24; /* shll2 r10 */
L8c615c24: sa_motion_publish_stop(ctx,0x8c615c24u); r[11]=r[11]&65535u;goto L8c615c26; /* extu.w r11,r11 */
L8c615c26: sa_motion_publish_stop(ctx,0x8c615c26u); r[11]<<=2;goto L8c615c28; /* shll2 r11 */
L8c615c28: sa_motion_publish_stop(ctx,0x8c615c28u); r[10]<<=2;goto L8c615c2a; /* shll2 r10 */
L8c615c2a: sa_motion_publish_stop(ctx,0x8c615c2au); r[11]<<=2;goto L8c615c2c; /* shll2 r11 */
L8c615c2c: sa_motion_publish_stop(ctx,0x8c615c2cu); *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c615c2e; /* shll r10 */
L8c615c2e: sa_motion_publish_stop(ctx,0x8c615c2eu); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c615c30; /* shll r11 */
L8c615c30: sa_motion_publish_stop(ctx,0x8c615c30u); r[11]+=r[3];goto L8c615c32; /* add r3,r11 */
L8c615c32: sa_motion_publish_stop(ctx,0x8c615c32u); r[10]+=r[3];goto L8c615c34; /* add r3,r10 */
L8c615c34: sa_motion_publish_stop(ctx,0x8c615c34u); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c615c36; /* fmov.s @(r0,r11),fr13 */
L8c615c36: sa_motion_publish_stop(ctx,0x8c615c36u); fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c615c38; /* fmov.s @(r0,r10),fr5 */
L8c615c38: sa_motion_publish_stop(ctx,0x8c615c38u); r[0]=20u;goto L8c615c3a; /* mov #0x14,r0 */
L8c615c3a: sa_motion_publish_stop(ctx,0x8c615c3au); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c615c3c; /* fmov.s @(r0,r11),fr14 */
L8c615c3c: sa_motion_publish_stop(ctx,0x8c615c3cu); fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c615c3e; /* fmov.s @(r0,r10),fr6 */
L8c615c3e: sa_motion_publish_stop(ctx,0x8c615c3eu); r[12]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c615c40; /* mov.w @r13+,r12 */
L8c615c40: sa_motion_publish_stop(ctx,0x8c615c40u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c615c42; /* mov.l @(0xc,r15),r3 */
L8c615c42: sa_motion_publish_stop(ctx,0x8c615c42u); r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c615c44; /* mov.w @r13,r2 */
L8c615c44: sa_motion_publish_stop(ctx,0x8c615c44u); r[12]=r[12]&65535u;goto L8c615c46; /* extu.w r12,r12 */
L8c615c46: sa_motion_publish_stop(ctx,0x8c615c46u); r[12]<<=2;goto L8c615c48; /* shll2 r12 */
L8c615c48: sa_motion_publish_stop(ctx,0x8c615c48u); r[2]=r[2]&65535u;goto L8c615c4a; /* extu.w r2,r2 */
L8c615c4a: sa_motion_publish_stop(ctx,0x8c615c4au); r[2]<<=2;goto L8c615c4c; /* shll2 r2 */
L8c615c4c: sa_motion_publish_stop(ctx,0x8c615c4cu); r[2]<<=2;goto L8c615c4e; /* shll2 r2 */
L8c615c4e: sa_motion_publish_stop(ctx,0x8c615c4eu); r[12]<<=2;goto L8c615c50; /* shll2 r12 */
L8c615c50: sa_motion_publish_stop(ctx,0x8c615c50u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c615c52; /* shll r12 */
L8c615c52: sa_motion_publish_stop(ctx,0x8c615c52u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c615c54; /* shll r2 */
L8c615c54: sa_motion_publish_stop(ctx,0x8c615c54u); r[2]+=r[3];goto L8c615c56; /* add r3,r2 */
L8c615c56: sa_motion_publish_stop(ctx,0x8c615c56u); r[12]+=r[3];goto L8c615c58; /* add r3,r12 */
L8c615c58: sa_motion_publish_stop(ctx,0x8c615c58u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c615c5a; /* pref @r2 */
L8c615c5a: sa_motion_publish_stop(ctx,0x8c615c5au); r[3]=0u;goto L8c615c5c; /* mov #0x0,r3 */
L8c615c5c: sa_motion_publish_stop(ctx,0x8c615c5cu); wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;goto L8c615c5e; /* mov.l r3,@(0x18,r15) */
L8c615c5e: sa_motion_publish_stop(ctx,0x8c615c5eu); r[4]=63u;goto L8c615c60; /* mov #0x3f,r4 */
L8c615c60: sa_motion_publish_stop(ctx,0x8c615c60u); r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c615c62; /* mov.l @(0xc,r10),r2 */
L8c615c62: sa_motion_publish_stop(ctx,0x8c615c62u); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c615c64; /* mov.l @(0xc,r11),r3 */
L8c615c64: sa_motion_publish_stop(ctx,0x8c615c64u); r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c615c66; /* mov.l @(0xc,r12),r1 */
L8c615c66: sa_motion_publish_stop(ctx,0x8c615c66u); r[2]&=r[3];goto L8c615c68; /* and r3,r2 */
L8c615c68: sa_motion_publish_stop(ctx,0x8c615c68u); r[2]&=r[1];goto L8c615c6a; /* and r1,r2 */
L8c615c6a: sa_motion_publish_stop(ctx,0x8c615c6au); r[4]&=r[2];goto L8c615c6c; /* and r2,r4 */
L8c615c6c: sa_motion_publish_stop(ctx,0x8c615c6cu); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c615c6e; /* tst r4,r4 */
L8c615c6e: sa_motion_publish_stop(ctx,0x8c615c70u); take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c615c76;goto L8c615c72; /* bt/s 0x8c615c76 */
L8c615c72: sa_motion_publish_stop(ctx,0x8c615c74u); goto L8c615dd0; /* bra 0x8c615dd0 */
L8c615c76: sa_motion_publish_stop(ctx,0x8c615c76u); r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c615c78; /* mov.l @(0xc,r10),r4 */
L8c615c78: sa_motion_publish_stop(ctx,0x8c615c78u); r[5]=1u;goto L8c615c7a; /* mov #0x1,r5 */
L8c615c7a: sa_motion_publish_stop(ctx,0x8c615c7au); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c615c7c; /* mov.l @(0xc,r11),r3 */
L8c615c7c: sa_motion_publish_stop(ctx,0x8c615c7cu); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c615c7e; /* mov.l @(0xc,r12),r2 */
L8c615c7e: sa_motion_publish_stop(ctx,0x8c615c7eu); r[4]|=r[3];goto L8c615c80; /* or r3,r4 */
L8c615c80: sa_motion_publish_stop(ctx,0x8c615c80u); r[4]|=r[2];goto L8c615c82; /* or r2,r4 */
L8c615c82: sa_motion_publish_stop(ctx,0x8c615c82u); r[4]&=r[5];goto L8c615c84; /* and r5,r4 */
L8c615c84: sa_motion_publish_stop(ctx,0x8c615c84u); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c615c86; /* tst r4,r4 */
L8c615c86: sa_motion_publish_stop(ctx,0x8c615c86u); if(!(*sr&1u))goto L8c615c8c;goto L8c615c88; /* bf 0x8c615c8c */
L8c615c88: sa_motion_publish_stop(ctx,0x8c615c8au); goto L8c615db2; /* bra 0x8c615db2 */
L8c615c8c: sa_motion_publish_stop(ctx,0x8c615c8cu); r[7]=r[15];goto L8c615c8e; /* mov r15,r7 */
L8c615c8e: sa_motion_publish_stop(ctx,0x8c615c8eu); r[7]+=68u;goto L8c615c90; /* add #0x44,r7 */
L8c615c90: sa_motion_publish_stop(ctx,0x8c615c90u); wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c615c92; /* mov.l r10,@r7 */
L8c615c92: sa_motion_publish_stop(ctx,0x8c615c92u); wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c615c94; /* mov.l r11,@(0x4,r7) */
L8c615c94: sa_motion_publish_stop(ctx,0x8c615c94u); wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c615c96; /* mov.l r12,@(0x8,r7) */
L8c615c96: sa_motion_publish_stop(ctx,0x8c615c98u); r[6]=0u;goto L8c615d44; /* bra 0x8c615d44 */
L8c615c9a: sa_motion_publish_stop(ctx,0x8c615c9au); r[2]=r[6];goto L8c615c9c; /* mov r6,r2 */
L8c615c9c: sa_motion_publish_stop(ctx,0x8c615c9cu); r[1]=r[6];goto L8c615c9e; /* mov r6,r1 */
L8c615c9e: sa_motion_publish_stop(ctx,0x8c615c9eu); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c615ca0; /* shll r2 */
L8c615ca0: sa_motion_publish_stop(ctx,0x8c615ca0u); r[3]=r[6];goto L8c615ca2; /* mov r6,r3 */
L8c615ca2: sa_motion_publish_stop(ctx,0x8c615ca2u); r[3]<<=2;goto L8c615ca4; /* shll2 r3 */
L8c615ca4: sa_motion_publish_stop(ctx,0x8c615ca4u); r[2]+=r[1];goto L8c615ca6; /* add r1,r2 */
L8c615ca6: sa_motion_publish_stop(ctx,0x8c615ca6u); r[2]<<=2;goto L8c615ca8; /* shll2 r2 */
L8c615ca8: sa_motion_publish_stop(ctx,0x8c615ca8u); r[3]+=r[7];goto L8c615caa; /* add r7,r3 */
L8c615caa: sa_motion_publish_stop(ctx,0x8c615caau); r[2]+=r[4];goto L8c615cac; /* add r4,r2 */
L8c615cac: sa_motion_publish_stop(ctx,0x8c615cacu); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c615cae; /* mov.l r3,@(0x4,r15) */
L8c615cae: sa_motion_publish_stop(ctx,0x8c615caeu); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c615cb0; /* mov.l r2,@r15 */
L8c615cb0: sa_motion_publish_stop(ctx,0x8c615cb0u); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c615cb2; /* mov.l @r3,r1 */
L8c615cb2: sa_motion_publish_stop(ctx,0x8c615cb2u); r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c615cb4; /* mov.l @(0xc,r1),r2 */
L8c615cb4: sa_motion_publish_stop(ctx,0x8c615cb4u); *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c615cb6; /* tst r5,r2 */
L8c615cb6: sa_motion_publish_stop(ctx,0x8c615cb8u); take=!(*sr&1u);r[6]+=1u;if(take)goto L8c615d1c;goto L8c615cba; /* bf/s 0x8c615d1c */
L8c615cba: sa_motion_publish_stop(ctx,0x8c615cbau); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615cbc; /* mov.l @(0x4,r15),r1 */
L8c615cbc: sa_motion_publish_stop(ctx,0x8c615cbcu); r[0]=24u;goto L8c615cbe; /* mov #0x18,r0 */
L8c615cbe: sa_motion_publish_stop(ctx,0x8c615cbeu); fcopy(f,4u,12u);goto L8c615cc0; /* fmov fr12,fr4 */
L8c615cc0: sa_motion_publish_stop(ctx,0x8c615cc0u); r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c615cc2; /* mov.l @r1,r3 */
L8c615cc2: sa_motion_publish_stop(ctx,0x8c615cc2u); r[4]=rd(m,bus,&io_status,0x8c615d10u,4);if(io_status)return io_status;goto L8c615cc4; /* mov.l 0x8c615d10,r4 */
L8c615cc4: sa_motion_publish_stop(ctx,0x8c615cc4u); wr(m,bus,&io_status,r[15]+0x14u,r[3],4);if(io_status)return io_status;goto L8c615cc6; /* mov.l r3,@(0x14,r15) */
L8c615cc6: sa_motion_publish_stop(ctx,0x8c615cc6u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c615cc8; /* fmov.s @(r0,r3),fr3 */
L8c615cc8: sa_motion_publish_stop(ctx,0x8c615cc8u); r[0]=12u;goto L8c615cca; /* mov #0xc,r0 */
L8c615cca: sa_motion_publish_stop(ctx,0x8c615ccau); r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c615ccc; /* mov.l @(0x14,r15),r2 */
L8c615ccc: sa_motion_publish_stop(ctx,0x8c615cccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c615cce; /* fdiv fr3,fr4 */
L8c615cce: sa_motion_publish_stop(ctx,0x8c615cceu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615cd0; /* fmov.s @(r0,r4),fr3 */
L8c615cd0: sa_motion_publish_stop(ctx,0x8c615cd0u); r[0]=16u;goto L8c615cd2; /* mov #0x10,r0 */
L8c615cd2: sa_motion_publish_stop(ctx,0x8c615cd2u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615cd4; /* mov.l @r15,r3 */
L8c615cd4: sa_motion_publish_stop(ctx,0x8c615cd4u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c615cd6; /* fmov.s @(r0,r2),fr2 */
L8c615cd6: sa_motion_publish_stop(ctx,0x8c615cd6u); r[0]=16u;goto L8c615cd8; /* mov #0x10,r0 */
L8c615cd8: sa_motion_publish_stop(ctx,0x8c615cd8u); r[2]=rd(m,bus,&io_status,0x8c615d14u,4);if(io_status)return io_status;goto L8c615cda; /* mov.l 0x8c615d14,r2 */
L8c615cda: sa_motion_publish_stop(ctx,0x8c615cdau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615cdc; /* fsub fr3,fr2 */
L8c615cdc: sa_motion_publish_stop(ctx,0x8c615cdcu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c615cde; /* fmov.s @r2,fr3 */
L8c615cde: sa_motion_publish_stop(ctx,0x8c615cdeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615ce0; /* fmul fr4,fr2 */
L8c615ce0: sa_motion_publish_stop(ctx,0x8c615ce0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615ce2; /* fdiv fr3,fr2 */
L8c615ce2: sa_motion_publish_stop(ctx,0x8c615ce2u); fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c615ce4; /* fmov.s fr2,@r3 */
L8c615ce4: sa_motion_publish_stop(ctx,0x8c615ce4u); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615ce6; /* mov.l @(0x4,r15),r1 */
L8c615ce6: sa_motion_publish_stop(ctx,0x8c615ce6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c615ce8; /* fmov.s @(r0,r4),fr3 */
L8c615ce8: sa_motion_publish_stop(ctx,0x8c615ce8u); r[0]=20u;goto L8c615cea; /* mov #0x14,r0 */
L8c615cea: sa_motion_publish_stop(ctx,0x8c615ceau); r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c615cec; /* mov.l @r1,r1 */
L8c615cec: sa_motion_publish_stop(ctx,0x8c615cecu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615cee; /* mov.l @r15,r3 */
L8c615cee: sa_motion_publish_stop(ctx,0x8c615ceeu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c615cf0; /* fmov.s @(r0,r1),fr2 */
L8c615cf0: sa_motion_publish_stop(ctx,0x8c615cf0u); r[1]=rd(m,bus,&io_status,0x8c615d18u,4);if(io_status)return io_status;goto L8c615cf2; /* mov.l 0x8c615d18,r1 */
L8c615cf2: sa_motion_publish_stop(ctx,0x8c615cf2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615cf4; /* fsub fr3,fr2 */
L8c615cf4: sa_motion_publish_stop(ctx,0x8c615cf4u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c615cf6; /* fmov.s @r1,fr3 */
L8c615cf6: sa_motion_publish_stop(ctx,0x8c615cf6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615cf8; /* fmul fr4,fr2 */
L8c615cf8: sa_motion_publish_stop(ctx,0x8c615cf8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615cfa; /* fdiv fr3,fr2 */
L8c615cfa: sa_motion_publish_stop(ctx,0x8c615cfau); r[0]=4u;goto L8c615cfc; /* mov #0x4,r0 */
L8c615cfc: sa_motion_publish_stop(ctx,0x8c615cfcu); fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c615cfe; /* fmov fr2,@(r0,r3) */
L8c615cfe: sa_motion_publish_stop(ctx,0x8c615cfeu); r[0]=8u;goto L8c615d00; /* mov #0x8,r0 */
L8c615d00: sa_motion_publish_stop(ctx,0x8c615d00u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615d02; /* mov.l @r15,r3 */
L8c615d02: sa_motion_publish_stop(ctx,0x8c615d02u); fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c615d04; /* fmov fr4,@(r0,r3) */
L8c615d04: sa_motion_publish_stop(ctx,0x8c615d06u); goto L8c615d44; /* bra 0x8c615d44 */
L8c615d1c: sa_motion_publish_stop(ctx,0x8c615d1cu); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615d1e; /* mov.l @(0x4,r15),r3 */
L8c615d1e: sa_motion_publish_stop(ctx,0x8c615d1eu); r[0]=16u;goto L8c615d20; /* mov #0x10,r0 */
L8c615d20: sa_motion_publish_stop(ctx,0x8c615d20u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615d22; /* mov.l @r15,r2 */
L8c615d22: sa_motion_publish_stop(ctx,0x8c615d22u); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c615d24; /* mov.l @r3,r1 */
L8c615d24: sa_motion_publish_stop(ctx,0x8c615d24u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c615d26; /* fmov.s @(r0,r1),fr3 */
L8c615d26: sa_motion_publish_stop(ctx,0x8c615d26u); r[0]=20u;goto L8c615d28; /* mov #0x14,r0 */
L8c615d28: sa_motion_publish_stop(ctx,0x8c615d28u); fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c615d2a; /* fmov.s fr3,@r2 */
L8c615d2a: sa_motion_publish_stop(ctx,0x8c615d2au); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615d2c; /* mov.l @(0x4,r15),r2 */
L8c615d2c: sa_motion_publish_stop(ctx,0x8c615d2cu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615d2e; /* mov.l @r15,r3 */
L8c615d2e: sa_motion_publish_stop(ctx,0x8c615d2eu); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c615d30; /* mov.l @r2,r1 */
L8c615d30: sa_motion_publish_stop(ctx,0x8c615d30u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c615d32; /* fmov.s @(r0,r1),fr3 */
L8c615d32: sa_motion_publish_stop(ctx,0x8c615d32u); r[0]=4u;goto L8c615d34; /* mov #0x4,r0 */
L8c615d34: sa_motion_publish_stop(ctx,0x8c615d34u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c615d36; /* fmov fr3,@(r0,r3) */
L8c615d36: sa_motion_publish_stop(ctx,0x8c615d36u); r[0]=24u;goto L8c615d38; /* mov #0x18,r0 */
L8c615d38: sa_motion_publish_stop(ctx,0x8c615d38u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c615d3a; /* mov.l @(0x4,r15),r2 */
L8c615d3a: sa_motion_publish_stop(ctx,0x8c615d3au); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c615d3c; /* mov.l @r15,r3 */
L8c615d3c: sa_motion_publish_stop(ctx,0x8c615d3cu); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c615d3e; /* mov.l @r2,r1 */
L8c615d3e: sa_motion_publish_stop(ctx,0x8c615d3eu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c615d40; /* fmov.s @(r0,r1),fr3 */
L8c615d40: sa_motion_publish_stop(ctx,0x8c615d40u); r[0]=8u;goto L8c615d42; /* mov #0x8,r0 */
L8c615d42: sa_motion_publish_stop(ctx,0x8c615d42u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c615d44; /* fmov fr3,@(r0,r3) */
L8c615d44: sa_motion_publish_stop(ctx,0x8c615d44u); r[3]=3u;goto L8c615d46; /* mov #0x3,r3 */
L8c615d46: sa_motion_publish_stop(ctx,0x8c615d46u); r[4]=r[15];goto L8c615d48; /* mov r15,r4 */
L8c615d48: sa_motion_publish_stop(ctx,0x8c615d48u); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c615d4a; /* cmp/ge r3,r6 */
L8c615d4a: sa_motion_publish_stop(ctx,0x8c615d4cu); take=!(*sr&1u);r[4]+=32u;if(take)goto L8c615c9a;goto L8c615d4e; /* bf/s 0x8c615c9a */
L8c615d4e: sa_motion_publish_stop(ctx,0x8c615d4eu); r[7]=r[4];goto L8c615d50; /* mov r4,r7 */
L8c615d50: sa_motion_publish_stop(ctx,0x8c615d50u); r[0]=8u;goto L8c615d52; /* mov #0x8,r0 */
L8c615d52: sa_motion_publish_stop(ctx,0x8c615d52u); r[3]=r[4];goto L8c615d54; /* mov r4,r3 */
L8c615d54: sa_motion_publish_stop(ctx,0x8c615d54u); fload(m,bus,&io_status,f,9u,r[0]+r[7]);if(io_status)return io_status;goto L8c615d56; /* fmov.s @(r0,r7),fr9 */
L8c615d56: sa_motion_publish_stop(ctx,0x8c615d56u); r[3]+=12u;goto L8c615d58; /* add #0xc,r3 */
L8c615d58: sa_motion_publish_stop(ctx,0x8c615d58u); r[6]=r[4];goto L8c615d5a; /* mov r4,r6 */
L8c615d5a: sa_motion_publish_stop(ctx,0x8c615d5au); r[0]=4u;goto L8c615d5c; /* mov #0x4,r0 */
L8c615d5c: sa_motion_publish_stop(ctx,0x8c615d5cu); fcopy(f,0u,9u);goto L8c615d5e; /* fmov fr9,fr0 */
L8c615d5e: sa_motion_publish_stop(ctx,0x8c615d5eu); r[6]+=24u;goto L8c615d60; /* add #0x18,r6 */
L8c615d60: sa_motion_publish_stop(ctx,0x8c615d60u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c615d62; /* mov.l r3,@r15 */
L8c615d62: sa_motion_publish_stop(ctx,0x8c615d62u); fload(m,bus,&io_status,f,6u,r[0]+r[7]);if(io_status)return io_status;goto L8c615d64; /* fmov.s @(r0,r7),fr6 */
L8c615d64: sa_motion_publish_stop(ctx,0x8c615d64u); fload(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c615d66; /* fmov.s @(r0,r3),fr4 */
L8c615d66: sa_motion_publish_stop(ctx,0x8c615d66u); r[0]=8u;goto L8c615d68; /* mov #0x8,r0 */
L8c615d68: sa_motion_publish_stop(ctx,0x8c615d68u); fload(m,bus,&io_status,f,7u,r[0]+r[3]);if(io_status)return io_status;goto L8c615d6a; /* fmov.s @(r0,r3),fr7 */
L8c615d6a: sa_motion_publish_stop(ctx,0x8c615d6au); r[0]=4u;goto L8c615d6c; /* mov #0x4,r0 */
L8c615d6c: sa_motion_publish_stop(ctx,0x8c615d6cu); fcopy(f,3u,6u);goto L8c615d6e; /* fmov fr6,fr3 */
L8c615d6e: sa_motion_publish_stop(ctx,0x8c615d6eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c615d70; /* fmul fr4,fr0 */
L8c615d70: sa_motion_publish_stop(ctx,0x8c615d70u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615d72; /* fmul fr7,fr3 */
L8c615d72: sa_motion_publish_stop(ctx,0x8c615d72u); fload(m,bus,&io_status,f,8u,r[7]);if(io_status)return io_status;goto L8c615d74; /* fmov.s @r7,fr8 */
L8c615d74: sa_motion_publish_stop(ctx,0x8c615d74u); fload(m,bus,&io_status,f,5u,r[3]);if(io_status)return io_status;goto L8c615d76; /* fmov.s @r3,fr5 */
L8c615d76: sa_motion_publish_stop(ctx,0x8c615d76u); fcopy(f,2u,8u);goto L8c615d78; /* fmov fr8,fr2 */
L8c615d78: sa_motion_publish_stop(ctx,0x8c615d78u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615d7a; /* fmul fr7,fr2 */
L8c615d7a: sa_motion_publish_stop(ctx,0x8c615d7au); fcopy(f,1u,6u);goto L8c615d7c; /* fmov fr6,fr1 */
L8c615d7c: sa_motion_publish_stop(ctx,0x8c615d7cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c615d7e; /* fsub fr3,fr0 */
L8c615d7e: sa_motion_publish_stop(ctx,0x8c615d7eu); fcopy(f,3u,9u);goto L8c615d80; /* fmov fr9,fr3 */
L8c615d80: sa_motion_publish_stop(ctx,0x8c615d80u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615d82; /* fmul fr5,fr3 */
L8c615d82: sa_motion_publish_stop(ctx,0x8c615d82u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615d84; /* fsub fr3,fr2 */
L8c615d84: sa_motion_publish_stop(ctx,0x8c615d84u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c615d86; /* fmov.s @(r0,r6),fr3 */
L8c615d86: sa_motion_publish_stop(ctx,0x8c615d86u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615d88; /* fmul fr2,fr3 */
L8c615d88: sa_motion_publish_stop(ctx,0x8c615d88u); fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c615d8a; /* fmov.s @r6,fr2 */
L8c615d8a: sa_motion_publish_stop(ctx,0x8c615d8au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615d8c; /* fmac fr0,fr2,fr3 */
L8c615d8c: sa_motion_publish_stop(ctx,0x8c615d8cu); fcopy(f,2u,3u);goto L8c615d8e; /* fmov fr3,fr2 */
L8c615d8e: sa_motion_publish_stop(ctx,0x8c615d8eu); fcopy(f,3u,8u);goto L8c615d90; /* fmov fr8,fr3 */
L8c615d90: sa_motion_publish_stop(ctx,0x8c615d90u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615d92; /* fmul fr4,fr3 */
L8c615d92: sa_motion_publish_stop(ctx,0x8c615d92u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[5],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615d94; /* fmul fr5,fr1 */
L8c615d94: sa_motion_publish_stop(ctx,0x8c615d94u); r[0]=8u;goto L8c615d96; /* mov #0x8,r0 */
L8c615d96: sa_motion_publish_stop(ctx,0x8c615d96u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c615d98; /* fsub fr3,fr1 */
L8c615d98: sa_motion_publish_stop(ctx,0x8c615d98u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c615d9a; /* fmov.s @(r0,r6),fr3 */
L8c615d9a: sa_motion_publish_stop(ctx,0x8c615d9au); fcopy(f,0u,1u);goto L8c615d9c; /* fmov fr1,fr0 */
L8c615d9c: sa_motion_publish_stop(ctx,0x8c615d9cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615d9e; /* fmac fr0,fr3,fr2 */
L8c615d9e: sa_motion_publish_stop(ctx,0x8c615d9eu); fcopy(f,3u,15u);goto L8c615da0; /* fmov fr15,fr3 */
L8c615da0: sa_motion_publish_stop(ctx,0x8c615da0u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c615da2; /* fneg fr3 */
L8c615da2: sa_motion_publish_stop(ctx,0x8c615da2u); fcopy(f,4u,2u);goto L8c615da4; /* fmov fr2,fr4 */
L8c615da4: sa_motion_publish_stop(ctx,0x8c615da4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c615da6; /* fmul fr3,fr4 */
L8c615da6: sa_motion_publish_stop(ctx,0x8c615da6u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c615da8; /* fldi0 fr2 */
L8c615da8: sa_motion_publish_stop(ctx,0x8c615da8u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c615daa; /* fcmp/gt fr2,fr4 */
L8c615daa: sa_motion_publish_stop(ctx,0x8c615daau); if((*sr&1u))goto L8c615dd0;goto L8c615dac; /* bt 0x8c615dd0 */
L8c615dac: sa_motion_publish_stop(ctx,0x8c615dacu); wr(m,bus,&io_status,r[15]+0x18u,r[5],4);if(io_status)return io_status;goto L8c615dae; /* mov.l r5,@(0x18,r15) */
L8c615dae: sa_motion_publish_stop(ctx,0x8c615db0u); fcopy(f,4u,12u);goto L8c615dd0; /* bra 0x8c615dd0 */
L8c615db2: sa_motion_publish_stop(ctx,0x8c615db2u); r[0]=20u;goto L8c615db4; /* mov #0x14,r0 */
L8c615db4: sa_motion_publish_stop(ctx,0x8c615db4u); fcopy(f,3u,13u);goto L8c615db6; /* fmov fr13,fr3 */
L8c615db6: sa_motion_publish_stop(ctx,0x8c615db6u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c615db8; /* fmov.s @(r0,r12),fr2 */
L8c615db8: sa_motion_publish_stop(ctx,0x8c615db8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615dba; /* fsub fr5,fr3 */
L8c615dba: sa_motion_publish_stop(ctx,0x8c615dbau); r[0]=16u;goto L8c615dbc; /* mov #0x10,r0 */
L8c615dbc: sa_motion_publish_stop(ctx,0x8c615dbcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615dbe; /* fsub fr14,fr2 */
L8c615dbe: sa_motion_publish_stop(ctx,0x8c615dbeu); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c615dc0; /* fmov.s @(r0,r12),fr1 */
L8c615dc0: sa_motion_publish_stop(ctx,0x8c615dc0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c615dc2; /* fsub fr13,fr1 */
L8c615dc2: sa_motion_publish_stop(ctx,0x8c615dc2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c615dc4; /* fmul fr2,fr3 */
L8c615dc4: sa_motion_publish_stop(ctx,0x8c615dc4u); fcopy(f,2u,14u);goto L8c615dc6; /* fmov fr14,fr2 */
L8c615dc6: sa_motion_publish_stop(ctx,0x8c615dc6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[6],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c615dc8; /* fsub fr6,fr2 */
L8c615dc8: sa_motion_publish_stop(ctx,0x8c615dc8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615dca; /* fmul fr1,fr2 */
L8c615dca: sa_motion_publish_stop(ctx,0x8c615dcau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615dcc; /* fsub fr2,fr3 */
L8c615dcc: sa_motion_publish_stop(ctx,0x8c615dccu); fcopy(f,4u,3u);goto L8c615dce; /* fmov fr3,fr4 */
L8c615dce: sa_motion_publish_stop(ctx,0x8c615dceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c615dd0; /* fmul fr15,fr4 */
L8c615dd0: sa_motion_publish_stop(ctx,0x8c615dd0u); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c615dd2; /* cmp/hs r8,r9 */
L8c615dd2: sa_motion_publish_stop(ctx,0x8c615dd2u); if((*sr&1u))goto L8c615e26;goto L8c615dd4; /* bt 0x8c615e26 */
L8c615dd4: sa_motion_publish_stop(ctx,0x8c615dd4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615dd6; /* fldi0 fr3 */
L8c615dd6: sa_motion_publish_stop(ctx,0x8c615dd6u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c615dd8; /* fcmp/gt fr4,fr3 */
L8c615dd8: sa_motion_publish_stop(ctx,0x8c615dd8u); if((*sr&1u))goto L8c615e76;goto L8c615dda; /* bt 0x8c615e76 */
L8c615dda: sa_motion_publish_stop(ctx,0x8c615ddau); r[5]=rd(m,bus,&io_status,0x8c616040u,4);if(io_status)return io_status;goto L8c615ddc; /* mov.l 0x8c616040,r5 */
L8c615ddc: sa_motion_publish_stop(ctx,0x8c615ddcu); r[6]=r[14];goto L8c615dde; /* mov r14,r6 */
L8c615dde: sa_motion_publish_stop(ctx,0x8c615ddeu); r[4]=r[11];goto L8c615de0; /* mov r11,r4 */
L8c615de0: sa_motion_publish_stop(ctx,0x8c615de0u); r[14]+=4u;goto L8c615de2; /* add #0x4,r14 */
L8c615de2: sa_motion_publish_stop(ctx,0x8c615de2u); r[8]+=16u;goto L8c615de4; /* add #0x10,r8 */
L8c615de4: sa_motion_publish_stop(ctx,0x8c615de4u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615de6; /* fschg */
L8c615de6: sa_motion_publish_stop(ctx,0x8c615de6u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615de8; /* fmov.s @r4+,fr0 */
L8c615de8: sa_motion_publish_stop(ctx,0x8c615de8u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615dea; /* fmov.s @r4+,fr2 */
L8c615dea: sa_motion_publish_stop(ctx,0x8c615deau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615dec; /* fldi0 fr3 */
L8c615dec: sa_motion_publish_stop(ctx,0x8c615decu); fcopy(f,8u,1u);goto L8c615dee; /* fmov fr1,fr8 */
L8c615dee: sa_motion_publish_stop(ctx,0x8c615deeu); fcopy(f,10u,3u);goto L8c615df0; /* fmov fr3,fr10 */
L8c615df0: sa_motion_publish_stop(ctx,0x8c615df0u); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c615df2; /* fipr fv8,fv0 */
L8c615df2: sa_motion_publish_stop(ctx,0x8c615df2u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615df4; /* fmov.s @r4+,fr4 */
L8c615df4: sa_motion_publish_stop(ctx,0x8c615df4u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615df6; /* fmov.s @r4,fr6 */
L8c615df6: sa_motion_publish_stop(ctx,0x8c615df6u); fcopy(f,8u,5u);goto L8c615df8; /* fmov fr5,fr8 */
L8c615df8: sa_motion_publish_stop(ctx,0x8c615df8u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615dfa; /* fschg */
L8c615dfa: sa_motion_publish_stop(ctx,0x8c615dfau); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c615dfc; /* mov.w @r6+,r2 */
L8c615dfc: sa_motion_publish_stop(ctx,0x8c615dfcu); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c615dfe; /* mov.w @r6,r3 */
L8c615dfe: sa_motion_publish_stop(ctx,0x8c615dfeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615e00; /* fmov.s fr6,@-r8 */
L8c615e00: sa_motion_publish_stop(ctx,0x8c615e00u); *fpul=r[3];goto L8c615e02; /* lds r3,FPUL */
L8c615e02: sa_motion_publish_stop(ctx,0x8c615e02u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615e04; /* fmov.s fr5,@-r8 */
L8c615e04: sa_motion_publish_stop(ctx,0x8c615e04u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c615e06; /* fsub fr3,fr11 */
L8c615e06: sa_motion_publish_stop(ctx,0x8c615e06u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615e08; /* fmov.s fr4,@-r8 */
L8c615e08: sa_motion_publish_stop(ctx,0x8c615e08u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c615e0a; /* float FPUL,fr1 */
L8c615e0a: sa_motion_publish_stop(ctx,0x8c615e0au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615e0c; /* fmul fr8,fr1 */
L8c615e0c: sa_motion_publish_stop(ctx,0x8c615e0cu); *fpul=r[2];goto L8c615e0e; /* lds r2,FPUL */
L8c615e0e: sa_motion_publish_stop(ctx,0x8c615e0eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615e10; /* mov.l r5,@-r8 */
L8c615e10: sa_motion_publish_stop(ctx,0x8c615e10u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c615e12; /* float FPUL,fr2 */
L8c615e12: sa_motion_publish_stop(ctx,0x8c615e12u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615e14; /* fmul fr8,fr2 */
L8c615e14: sa_motion_publish_stop(ctx,0x8c615e14u); r[8]+=28u;goto L8c615e16; /* add #0x1c,r8 */
L8c615e16: sa_motion_publish_stop(ctx,0x8c615e16u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c615e18; /* fmov.s fr11,@-r8 */
L8c615e18: sa_motion_publish_stop(ctx,0x8c615e18u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c615e1a; /* fmov.s fr1,@-r8 */
L8c615e1a: sa_motion_publish_stop(ctx,0x8c615e1au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c615e1c; /* fmov.s fr2,@-r8 */
L8c615e1c: sa_motion_publish_stop(ctx,0x8c615e1cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615e1e; /* pref @r8 */
L8c615e1e: sa_motion_publish_stop(ctx,0x8c615e1eu); r[8]+=16u;goto L8c615e20; /* add #0x10,r8 */
L8c615e20: sa_motion_publish_stop(ctx,0x8c615e20u); r[14]+=4294967292u;goto L8c615e22; /* add #-0x4,r14 */
L8c615e22: sa_motion_publish_stop(ctx,0x8c615e24u); r[9]=r[8];goto L8c615ebc; /* bra 0x8c615ebc */
L8c615e26: sa_motion_publish_stop(ctx,0x8c615e26u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615e28; /* fldi0 fr3 */
L8c615e28: sa_motion_publish_stop(ctx,0x8c615e28u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c615e2a; /* fcmp/gt fr4,fr3 */
L8c615e2a: sa_motion_publish_stop(ctx,0x8c615e2cu); take=!(*sr&1u);r[14]+=4u;if(take)goto L8c615ebc;goto L8c615e2e; /* bf/s 0x8c615ebc */
L8c615e2e: sa_motion_publish_stop(ctx,0x8c615e2eu); r[14]+=4294967292u;goto L8c615e30; /* add #-0x4,r14 */
L8c615e30: sa_motion_publish_stop(ctx,0x8c615e30u); r[5]=rd(m,bus,&io_status,0x8c616044u,4);if(io_status)return io_status;goto L8c615e32; /* mov.l 0x8c616044,r5 */
L8c615e32: sa_motion_publish_stop(ctx,0x8c615e32u); r[6]=r[14];goto L8c615e34; /* mov r14,r6 */
L8c615e34: sa_motion_publish_stop(ctx,0x8c615e34u); r[4]=r[10];goto L8c615e36; /* mov r10,r4 */
L8c615e36: sa_motion_publish_stop(ctx,0x8c615e36u); r[14]+=4u;goto L8c615e38; /* add #0x4,r14 */
L8c615e38: sa_motion_publish_stop(ctx,0x8c615e38u); r[8]+=16u;goto L8c615e3a; /* add #0x10,r8 */
L8c615e3a: sa_motion_publish_stop(ctx,0x8c615e3au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615e3c; /* fschg */
L8c615e3c: sa_motion_publish_stop(ctx,0x8c615e3cu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615e3e; /* fmov.s @r4+,fr0 */
L8c615e3e: sa_motion_publish_stop(ctx,0x8c615e3eu); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615e40; /* fmov.s @r4+,fr2 */
L8c615e40: sa_motion_publish_stop(ctx,0x8c615e40u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615e42; /* fldi0 fr3 */
L8c615e42: sa_motion_publish_stop(ctx,0x8c615e42u); fcopy(f,8u,1u);goto L8c615e44; /* fmov fr1,fr8 */
L8c615e44: sa_motion_publish_stop(ctx,0x8c615e44u); fcopy(f,10u,3u);goto L8c615e46; /* fmov fr3,fr10 */
L8c615e46: sa_motion_publish_stop(ctx,0x8c615e46u); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c615e48; /* fipr fv8,fv0 */
L8c615e48: sa_motion_publish_stop(ctx,0x8c615e48u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615e4a; /* fmov.s @r4+,fr4 */
L8c615e4a: sa_motion_publish_stop(ctx,0x8c615e4au); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615e4c; /* fmov.s @r4,fr6 */
L8c615e4c: sa_motion_publish_stop(ctx,0x8c615e4cu); fcopy(f,8u,5u);goto L8c615e4e; /* fmov fr5,fr8 */
L8c615e4e: sa_motion_publish_stop(ctx,0x8c615e4eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615e50; /* fschg */
L8c615e50: sa_motion_publish_stop(ctx,0x8c615e50u); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c615e52; /* mov.w @r6+,r2 */
L8c615e52: sa_motion_publish_stop(ctx,0x8c615e52u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c615e54; /* mov.w @r6,r3 */
L8c615e54: sa_motion_publish_stop(ctx,0x8c615e54u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615e56; /* fmov.s fr6,@-r8 */
L8c615e56: sa_motion_publish_stop(ctx,0x8c615e56u); *fpul=r[3];goto L8c615e58; /* lds r3,FPUL */
L8c615e58: sa_motion_publish_stop(ctx,0x8c615e58u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615e5a; /* fmov.s fr5,@-r8 */
L8c615e5a: sa_motion_publish_stop(ctx,0x8c615e5au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c615e5c; /* fsub fr3,fr11 */
L8c615e5c: sa_motion_publish_stop(ctx,0x8c615e5cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615e5e; /* fmov.s fr4,@-r8 */
L8c615e5e: sa_motion_publish_stop(ctx,0x8c615e5eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c615e60; /* float FPUL,fr1 */
L8c615e60: sa_motion_publish_stop(ctx,0x8c615e60u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615e62; /* fmul fr8,fr1 */
L8c615e62: sa_motion_publish_stop(ctx,0x8c615e62u); *fpul=r[2];goto L8c615e64; /* lds r2,FPUL */
L8c615e64: sa_motion_publish_stop(ctx,0x8c615e64u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615e66; /* mov.l r5,@-r8 */
L8c615e66: sa_motion_publish_stop(ctx,0x8c615e66u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c615e68; /* float FPUL,fr2 */
L8c615e68: sa_motion_publish_stop(ctx,0x8c615e68u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615e6a; /* fmul fr8,fr2 */
L8c615e6a: sa_motion_publish_stop(ctx,0x8c615e6au); r[8]+=28u;goto L8c615e6c; /* add #0x1c,r8 */
L8c615e6c: sa_motion_publish_stop(ctx,0x8c615e6cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c615e6e; /* fmov.s fr11,@-r8 */
L8c615e6e: sa_motion_publish_stop(ctx,0x8c615e6eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c615e70; /* fmov.s fr1,@-r8 */
L8c615e70: sa_motion_publish_stop(ctx,0x8c615e70u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c615e72; /* fmov.s fr2,@-r8 */
L8c615e72: sa_motion_publish_stop(ctx,0x8c615e72u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615e74; /* pref @r8 */
L8c615e74: sa_motion_publish_stop(ctx,0x8c615e74u); r[8]+=16u;goto L8c615e76; /* add #0x10,r8 */
L8c615e76: sa_motion_publish_stop(ctx,0x8c615e76u); r[5]=rd(m,bus,&io_status,0x8c616044u,4);if(io_status)return io_status;goto L8c615e78; /* mov.l 0x8c616044,r5 */
L8c615e78: sa_motion_publish_stop(ctx,0x8c615e78u); r[6]=r[14];goto L8c615e7a; /* mov r14,r6 */
L8c615e7a: sa_motion_publish_stop(ctx,0x8c615e7au); r[4]=r[11];goto L8c615e7c; /* mov r11,r4 */
L8c615e7c: sa_motion_publish_stop(ctx,0x8c615e7cu); r[14]+=4u;goto L8c615e7e; /* add #0x4,r14 */
L8c615e7e: sa_motion_publish_stop(ctx,0x8c615e7eu); r[8]+=16u;goto L8c615e80; /* add #0x10,r8 */
L8c615e80: sa_motion_publish_stop(ctx,0x8c615e80u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615e82; /* fschg */
L8c615e82: sa_motion_publish_stop(ctx,0x8c615e82u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615e84; /* fmov.s @r4+,fr0 */
L8c615e84: sa_motion_publish_stop(ctx,0x8c615e84u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615e86; /* fmov.s @r4+,fr2 */
L8c615e86: sa_motion_publish_stop(ctx,0x8c615e86u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615e88; /* fldi0 fr3 */
L8c615e88: sa_motion_publish_stop(ctx,0x8c615e88u); fcopy(f,8u,1u);goto L8c615e8a; /* fmov fr1,fr8 */
L8c615e8a: sa_motion_publish_stop(ctx,0x8c615e8au); fcopy(f,10u,3u);goto L8c615e8c; /* fmov fr3,fr10 */
L8c615e8c: sa_motion_publish_stop(ctx,0x8c615e8cu); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c615e8e; /* fipr fv8,fv0 */
L8c615e8e: sa_motion_publish_stop(ctx,0x8c615e8eu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615e90; /* fmov.s @r4+,fr4 */
L8c615e90: sa_motion_publish_stop(ctx,0x8c615e90u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615e92; /* fmov.s @r4,fr6 */
L8c615e92: sa_motion_publish_stop(ctx,0x8c615e92u); fcopy(f,8u,5u);goto L8c615e94; /* fmov fr5,fr8 */
L8c615e94: sa_motion_publish_stop(ctx,0x8c615e94u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615e96; /* fschg */
L8c615e96: sa_motion_publish_stop(ctx,0x8c615e96u); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c615e98; /* mov.w @r6+,r2 */
L8c615e98: sa_motion_publish_stop(ctx,0x8c615e98u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c615e9a; /* mov.w @r6,r3 */
L8c615e9a: sa_motion_publish_stop(ctx,0x8c615e9au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615e9c; /* fmov.s fr6,@-r8 */
L8c615e9c: sa_motion_publish_stop(ctx,0x8c615e9cu); *fpul=r[3];goto L8c615e9e; /* lds r3,FPUL */
L8c615e9e: sa_motion_publish_stop(ctx,0x8c615e9eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615ea0; /* fmov.s fr5,@-r8 */
L8c615ea0: sa_motion_publish_stop(ctx,0x8c615ea0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c615ea2; /* fsub fr3,fr11 */
L8c615ea2: sa_motion_publish_stop(ctx,0x8c615ea2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615ea4; /* fmov.s fr4,@-r8 */
L8c615ea4: sa_motion_publish_stop(ctx,0x8c615ea4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c615ea6; /* float FPUL,fr1 */
L8c615ea6: sa_motion_publish_stop(ctx,0x8c615ea6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615ea8; /* fmul fr8,fr1 */
L8c615ea8: sa_motion_publish_stop(ctx,0x8c615ea8u); *fpul=r[2];goto L8c615eaa; /* lds r2,FPUL */
L8c615eaa: sa_motion_publish_stop(ctx,0x8c615eaau); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615eac; /* mov.l r5,@-r8 */
L8c615eac: sa_motion_publish_stop(ctx,0x8c615eacu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c615eae; /* float FPUL,fr2 */
L8c615eae: sa_motion_publish_stop(ctx,0x8c615eaeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615eb0; /* fmul fr8,fr2 */
L8c615eb0: sa_motion_publish_stop(ctx,0x8c615eb0u); r[8]+=28u;goto L8c615eb2; /* add #0x1c,r8 */
L8c615eb2: sa_motion_publish_stop(ctx,0x8c615eb2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c615eb4; /* fmov.s fr11,@-r8 */
L8c615eb4: sa_motion_publish_stop(ctx,0x8c615eb4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c615eb6; /* fmov.s fr1,@-r8 */
L8c615eb6: sa_motion_publish_stop(ctx,0x8c615eb6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c615eb8; /* fmov.s fr2,@-r8 */
L8c615eb8: sa_motion_publish_stop(ctx,0x8c615eb8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c615eba; /* pref @r8 */
L8c615eba: sa_motion_publish_stop(ctx,0x8c615ebau); r[8]+=16u;goto L8c615ebc; /* add #0x10,r8 */
L8c615ebc: sa_motion_publish_stop(ctx,0x8c615ebcu); r[2]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c615ebe; /* mov.l @(0x18,r15),r2 */
L8c615ebe: sa_motion_publish_stop(ctx,0x8c615ebeu); *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c615ec0; /* tst r2,r2 */
L8c615ec0: sa_motion_publish_stop(ctx,0x8c615ec0u); if((*sr&1u))goto L8c615f96;goto L8c615ec2; /* bt 0x8c615f96 */
L8c615ec2: sa_motion_publish_stop(ctx,0x8c615ec2u); r[0]=68u;goto L8c615ec4; /* mov #0x44,r0 */
L8c615ec4: sa_motion_publish_stop(ctx,0x8c615ec4u); r[4]=rd(m,bus,&io_status,0x8c616048u,4);if(io_status)return io_status;goto L8c615ec6; /* mov.l 0x8c616048,r4 */
L8c615ec6: sa_motion_publish_stop(ctx,0x8c615ec6u); r[3]=0u;goto L8c615ec8; /* mov #0x0,r3 */
L8c615ec8: sa_motion_publish_stop(ctx,0x8c615ec8u); r[9]=rd(m,bus,&io_status,0x8c61604cu,4);if(io_status)return io_status;goto L8c615eca; /* mov.l 0x8c61604c,r9 */
L8c615eca: sa_motion_publish_stop(ctx,0x8c615ecau); wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c615ecc; /* mov.l r3,@r4 */
L8c615ecc: sa_motion_publish_stop(ctx,0x8c615eccu); r[4]=r[15];goto L8c615ece; /* mov r15,r4 */
L8c615ece: sa_motion_publish_stop(ctx,0x8c615eceu); fload(m,bus,&io_status,f,3u,r[11]);if(io_status)return io_status;goto L8c615ed0; /* fmov.s @r11,fr3 */
L8c615ed0: sa_motion_publish_stop(ctx,0x8c615ed0u); r[14]+=4294967292u;goto L8c615ed2; /* add #-0x4,r14 */
L8c615ed2: sa_motion_publish_stop(ctx,0x8c615ed2u); fload(m,bus,&io_status,f,2u,r[10]);if(io_status)return io_status;goto L8c615ed4; /* fmov.s @r10,fr2 */
L8c615ed4: sa_motion_publish_stop(ctx,0x8c615ed4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615ed6; /* fadd fr3,fr2 */
L8c615ed6: sa_motion_publish_stop(ctx,0x8c615ed6u); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c615ed8; /* fmov.s @r12,fr3 */
L8c615ed8: sa_motion_publish_stop(ctx,0x8c615ed8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615eda; /* fadd fr3,fr2 */
L8c615eda: sa_motion_publish_stop(ctx,0x8c615edau); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615edc; /* fmov fr2,@(r0,r15) */
L8c615edc: sa_motion_publish_stop(ctx,0x8c615edcu); r[0]=4u;goto L8c615ede; /* mov #0x4,r0 */
L8c615ede: sa_motion_publish_stop(ctx,0x8c615edeu); fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c615ee0; /* fmov.s @(r0,r10),fr2 */
L8c615ee0: sa_motion_publish_stop(ctx,0x8c615ee0u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c615ee2; /* fmov.s @(r0,r11),fr3 */
L8c615ee2: sa_motion_publish_stop(ctx,0x8c615ee2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615ee4; /* fadd fr3,fr2 */
L8c615ee4: sa_motion_publish_stop(ctx,0x8c615ee4u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c615ee6; /* fmov.s @(r0,r12),fr3 */
L8c615ee6: sa_motion_publish_stop(ctx,0x8c615ee6u); r[0]=72u;goto L8c615ee8; /* mov #0x48,r0 */
L8c615ee8: sa_motion_publish_stop(ctx,0x8c615ee8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615eea; /* fadd fr3,fr2 */
L8c615eea: sa_motion_publish_stop(ctx,0x8c615eeau); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615eec; /* fmov fr2,@(r0,r15) */
L8c615eec: sa_motion_publish_stop(ctx,0x8c615eecu); r[0]=8u;goto L8c615eee; /* mov #0x8,r0 */
L8c615eee: sa_motion_publish_stop(ctx,0x8c615eeeu); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c615ef0; /* fmov.s @(r0,r11),fr3 */
L8c615ef0: sa_motion_publish_stop(ctx,0x8c615ef0u); fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c615ef2; /* fmov.s @(r0,r10),fr2 */
L8c615ef2: sa_motion_publish_stop(ctx,0x8c615ef2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615ef4; /* fadd fr3,fr2 */
L8c615ef4: sa_motion_publish_stop(ctx,0x8c615ef4u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c615ef6; /* fmov.s @(r0,r12),fr3 */
L8c615ef6: sa_motion_publish_stop(ctx,0x8c615ef6u); r[0]=76u;goto L8c615ef8; /* mov #0x4c,r0 */
L8c615ef8: sa_motion_publish_stop(ctx,0x8c615ef8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c615efa; /* fadd fr3,fr2 */
L8c615efa: sa_motion_publish_stop(ctx,0x8c615efau); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615efc; /* fmov fr2,@(r0,r15) */
L8c615efc: sa_motion_publish_stop(ctx,0x8c615efeu); target=r[9];*pr=0x8c615f00u;r[4]+=68u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c615f00; /* jsr @r9 */
L8c615f00: sa_motion_publish_stop(ctx,0x8c615f00u); r[4]=rd(m,bus,&io_status,0x8c616054u,4);if(io_status)return io_status;goto L8c615f02; /* mov.l 0x8c616054,r4 */
L8c615f02: sa_motion_publish_stop(ctx,0x8c615f02u); r[0]=68u;goto L8c615f04; /* mov #0x44,r0 */
L8c615f04: sa_motion_publish_stop(ctx,0x8c615f04u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c615f06; /* fmov.s @(r0,r15),fr3 */
L8c615f06: sa_motion_publish_stop(ctx,0x8c615f06u); r[0]=72u;goto L8c615f08; /* mov #0x48,r0 */
L8c615f08: sa_motion_publish_stop(ctx,0x8c615f08u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c615f0a; /* fmov.s @r4,fr2 */
L8c615f0a: sa_motion_publish_stop(ctx,0x8c615f0au); r[5]=rd(m,bus,&io_status,0x8c616050u,4);if(io_status)return io_status;goto L8c615f0c; /* mov.l 0x8c616050,r5 */
L8c615f0c: sa_motion_publish_stop(ctx,0x8c615f0cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615f0e; /* fmul fr3,fr2 */
L8c615f0e: sa_motion_publish_stop(ctx,0x8c615f0eu); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c615f10; /* mov.w @r14,r3 */
L8c615f10: sa_motion_publish_stop(ctx,0x8c615f10u); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c615f12; /* fmov.s @r5,fr3 */
L8c615f12: sa_motion_publish_stop(ctx,0x8c615f12u); *fpul=r[3];goto L8c615f14; /* lds r3,FPUL */
L8c615f14: sa_motion_publish_stop(ctx,0x8c615f14u); r[9]=rd(m,bus,&io_status,0x8c616058u,4);if(io_status)return io_status;goto L8c615f16; /* mov.l 0x8c616058,r9 */
L8c615f16: sa_motion_publish_stop(ctx,0x8c615f16u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615f18; /* fsub fr2,fr3 */
L8c615f18: sa_motion_publish_stop(ctx,0x8c615f18u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615f1a; /* fmov.s @(r0,r15),fr2 */
L8c615f1a: sa_motion_publish_stop(ctx,0x8c615f1au); r[0]=4u;goto L8c615f1c; /* mov #0x4,r0 */
L8c615f1c: sa_motion_publish_stop(ctx,0x8c615f1cu); fload(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c615f1e; /* fmov.s @(r0,r4),fr1 */
L8c615f1e: sa_motion_publish_stop(ctx,0x8c615f1eu); r[0]=76u;goto L8c615f20; /* mov #0x4c,r0 */
L8c615f20: sa_motion_publish_stop(ctx,0x8c615f20u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615f22; /* fmul fr2,fr1 */
L8c615f22: sa_motion_publish_stop(ctx,0x8c615f22u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c615f24; /* fmov.s @(r0,r15),fr2 */
L8c615f24: sa_motion_publish_stop(ctx,0x8c615f24u); r[0]=8u;goto L8c615f26; /* mov #0x8,r0 */
L8c615f26: sa_motion_publish_stop(ctx,0x8c615f26u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c615f28; /* fsub fr1,fr3 */
L8c615f28: sa_motion_publish_stop(ctx,0x8c615f28u); fload(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c615f2a; /* fmov.s @(r0,r4),fr1 */
L8c615f2a: sa_motion_publish_stop(ctx,0x8c615f2au); r[4]=r[9];goto L8c615f2c; /* mov r9,r4 */
L8c615f2c: sa_motion_publish_stop(ctx,0x8c615f2cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615f2e; /* fmul fr2,fr1 */
L8c615f2e: sa_motion_publish_stop(ctx,0x8c615f2eu); fcopy(f,4u,3u);goto L8c615f30; /* fmov fr3,fr4 */
L8c615f30: sa_motion_publish_stop(ctx,0x8c615f30u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c615f32; /* float FPUL,fr3 */
L8c615f32: sa_motion_publish_stop(ctx,0x8c615f32u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c615f34; /* fsub fr1,fr4 */
L8c615f34: sa_motion_publish_stop(ctx,0x8c615f34u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c615f36; /* ftrc fr3,FPUL */
L8c615f36: sa_motion_publish_stop(ctx,0x8c615f36u); r[2]=*fpul;goto L8c615f38; /* sts FPUL,r2 */
L8c615f38: sa_motion_publish_stop(ctx,0x8c615f38u); wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c615f3a; /* mov.w r2,@r4 */
L8c615f3a: sa_motion_publish_stop(ctx,0x8c615f3au); r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c615f3c; /* mov.w @(0x2,r14),r0 */
L8c615f3c: sa_motion_publish_stop(ctx,0x8c615f3cu); r[3]=r[0];goto L8c615f3e; /* mov r0,r3 */
L8c615f3e: sa_motion_publish_stop(ctx,0x8c615f3eu); *fpul=r[3];goto L8c615f40; /* lds r3,FPUL */
L8c615f40: sa_motion_publish_stop(ctx,0x8c615f40u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c615f42; /* float FPUL,fr3 */
L8c615f42: sa_motion_publish_stop(ctx,0x8c615f42u); r[14]+=4u;goto L8c615f44; /* add #0x4,r14 */
L8c615f44: sa_motion_publish_stop(ctx,0x8c615f44u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c615f46; /* ftrc fr3,FPUL */
L8c615f46: sa_motion_publish_stop(ctx,0x8c615f46u); r[0]=*fpul;goto L8c615f48; /* sts FPUL,r0 */
L8c615f48: sa_motion_publish_stop(ctx,0x8c615f48u); wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c615f4a; /* mov.w r0,@(0x2,r4) */
L8c615f4a: sa_motion_publish_stop(ctx,0x8c615f4au); r[4]+=4u;goto L8c615f4c; /* add #0x4,r4 */
L8c615f4c: sa_motion_publish_stop(ctx,0x8c615f4cu); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c615f4e; /* mov.w @r14,r3 */
L8c615f4e: sa_motion_publish_stop(ctx,0x8c615f4eu); *fpul=r[3];goto L8c615f50; /* lds r3,FPUL */
L8c615f50: sa_motion_publish_stop(ctx,0x8c615f50u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c615f52; /* float FPUL,fr3 */
L8c615f52: sa_motion_publish_stop(ctx,0x8c615f52u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c615f54; /* ftrc fr3,FPUL */
L8c615f54: sa_motion_publish_stop(ctx,0x8c615f54u); r[2]=*fpul;goto L8c615f56; /* sts FPUL,r2 */
L8c615f56: sa_motion_publish_stop(ctx,0x8c615f56u); wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c615f58; /* mov.w r2,@r4 */
L8c615f58: sa_motion_publish_stop(ctx,0x8c615f58u); r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c615f5a; /* mov.w @(0x2,r14),r0 */
L8c615f5a: sa_motion_publish_stop(ctx,0x8c615f5au); r[14]+=4u;goto L8c615f5c; /* add #0x4,r14 */
L8c615f5c: sa_motion_publish_stop(ctx,0x8c615f5cu); r[3]=r[0];goto L8c615f5e; /* mov r0,r3 */
L8c615f5e: sa_motion_publish_stop(ctx,0x8c615f5eu); *fpul=r[3];goto L8c615f60; /* lds r3,FPUL */
L8c615f60: sa_motion_publish_stop(ctx,0x8c615f60u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c615f62; /* float FPUL,fr3 */
L8c615f62: sa_motion_publish_stop(ctx,0x8c615f62u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c615f64; /* ftrc fr3,FPUL */
L8c615f64: sa_motion_publish_stop(ctx,0x8c615f64u); r[0]=*fpul;goto L8c615f66; /* sts FPUL,r0 */
L8c615f66: sa_motion_publish_stop(ctx,0x8c615f66u); wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c615f68; /* mov.w r0,@(0x2,r4) */
L8c615f68: sa_motion_publish_stop(ctx,0x8c615f68u); r[4]=r[9];goto L8c615f6a; /* mov r9,r4 */
L8c615f6a: sa_motion_publish_stop(ctx,0x8c615f6au); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c615f6c; /* mov.w @r14,r3 */
L8c615f6c: sa_motion_publish_stop(ctx,0x8c615f6cu); r[4]+=8u;goto L8c615f6e; /* add #0x8,r4 */
L8c615f6e: sa_motion_publish_stop(ctx,0x8c615f6eu); *fpul=r[3];goto L8c615f70; /* lds r3,FPUL */
L8c615f70: sa_motion_publish_stop(ctx,0x8c615f70u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c615f72; /* float FPUL,fr3 */
L8c615f72: sa_motion_publish_stop(ctx,0x8c615f72u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c615f74; /* ftrc fr3,FPUL */
L8c615f74: sa_motion_publish_stop(ctx,0x8c615f74u); r[2]=*fpul;goto L8c615f76; /* sts FPUL,r2 */
L8c615f76: sa_motion_publish_stop(ctx,0x8c615f76u); wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c615f78; /* mov.w r2,@r4 */
L8c615f78: sa_motion_publish_stop(ctx,0x8c615f78u); r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c615f7a; /* mov.w @(0x2,r14),r0 */
L8c615f7a: sa_motion_publish_stop(ctx,0x8c615f7au); r[3]=r[0];goto L8c615f7c; /* mov r0,r3 */
L8c615f7c: sa_motion_publish_stop(ctx,0x8c615f7cu); *fpul=r[3];goto L8c615f7e; /* lds r3,FPUL */
L8c615f7e: sa_motion_publish_stop(ctx,0x8c615f7eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c615f80; /* float FPUL,fr3 */
L8c615f80: sa_motion_publish_stop(ctx,0x8c615f80u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c615f82; /* ftrc fr3,FPUL */
L8c615f82: sa_motion_publish_stop(ctx,0x8c615f82u); r[3]=rd(m,bus,&io_status,0x8c61605cu,4);if(io_status)return io_status;goto L8c615f84; /* mov.l 0x8c61605c,r3 */
L8c615f84: sa_motion_publish_stop(ctx,0x8c615f84u); r[5]=r[11];goto L8c615f86; /* mov r11,r5 */
L8c615f86: sa_motion_publish_stop(ctx,0x8c615f86u); r[6]=r[12];goto L8c615f88; /* mov r12,r6 */
L8c615f88: sa_motion_publish_stop(ctx,0x8c615f88u); r[7]=r[9];goto L8c615f8a; /* mov r9,r7 */
L8c615f8a: sa_motion_publish_stop(ctx,0x8c615f8au); r[0]=*fpul;goto L8c615f8c; /* sts FPUL,r0 */
L8c615f8c: sa_motion_publish_stop(ctx,0x8c615f8cu); wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c615f8e; /* mov.w r0,@(0x2,r4) */
L8c615f8e: sa_motion_publish_stop(ctx,0x8c615f90u); target=r[3];*pr=0x8c615f92u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c615f92; /* jsr @r3 */
L8c615f92: sa_motion_publish_stop(ctx,0x8c615f92u); r[14]+=4294967292u;goto L8c615f94; /* add #-0x4,r14 */
L8c615f94: sa_motion_publish_stop(ctx,0x8c615f94u); r[9]=r[8];goto L8c615f96; /* mov r8,r9 */
L8c615f96: sa_motion_publish_stop(ctx,0x8c615f96u); fcopy(f,5u,13u);goto L8c615f98; /* fmov fr13,fr5 */
L8c615f98: sa_motion_publish_stop(ctx,0x8c615f98u); r[0]=16u;goto L8c615f9a; /* mov #0x10,r0 */
L8c615f9a: sa_motion_publish_stop(ctx,0x8c615f9au); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c615f9c; /* mov.l @(0x8,r15),r3 */
L8c615f9c: sa_motion_publish_stop(ctx,0x8c615f9cu); r[10]=r[11];goto L8c615f9e; /* mov r11,r10 */
L8c615f9e: sa_motion_publish_stop(ctx,0x8c615f9eu); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c615fa0; /* fmov.s @(r0,r12),fr13 */
L8c615fa0: sa_motion_publish_stop(ctx,0x8c615fa0u); r[0]=20u;goto L8c615fa2; /* mov #0x14,r0 */
L8c615fa2: sa_motion_publish_stop(ctx,0x8c615fa2u); fcopy(f,6u,14u);goto L8c615fa4; /* fmov fr14,fr6 */
L8c615fa4: sa_motion_publish_stop(ctx,0x8c615fa4u); r[3]+=4294967295u;goto L8c615fa6; /* add #-0x1,r3 */
L8c615fa6: sa_motion_publish_stop(ctx,0x8c615fa6u); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c615fa8; /* fmov.s @(r0,r12),fr14 */
L8c615fa8: sa_motion_publish_stop(ctx,0x8c615fa8u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c615faa; /* tst r3,r3 */
L8c615faa: sa_motion_publish_stop(ctx,0x8c615faau); r[11]=r[12];goto L8c615fac; /* mov r12,r11 */
L8c615fac: sa_motion_publish_stop(ctx,0x8c615facu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c615fae; /* fneg fr15 */
L8c615fae: sa_motion_publish_stop(ctx,0x8c615fb0u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c615fb6;goto L8c615fb2; /* bt/s 0x8c615fb6 */
L8c615fb2: sa_motion_publish_stop(ctx,0x8c615fb4u); goto L8c615c3e; /* bra 0x8c615c3e */
L8c615fb6: sa_motion_publish_stop(ctx,0x8c615fb6u); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c615fb8; /* cmp/hs r8,r9 */
L8c615fb8: sa_motion_publish_stop(ctx,0x8c615fbau); take=(*sr&1u);r[14]+=8u;if(take)goto L8c616004;goto L8c615fbc; /* bt/s 0x8c616004 */
L8c615fbc: sa_motion_publish_stop(ctx,0x8c615fbcu); r[14]+=4294967288u;goto L8c615fbe; /* add #-0x8,r14 */
L8c615fbe: sa_motion_publish_stop(ctx,0x8c615fbeu); r[5]=rd(m,bus,&io_status,0x8c616040u,4);if(io_status)return io_status;goto L8c615fc0; /* mov.l 0x8c616040,r5 */
L8c615fc0: sa_motion_publish_stop(ctx,0x8c615fc0u); r[6]=r[14];goto L8c615fc2; /* mov r14,r6 */
L8c615fc2: sa_motion_publish_stop(ctx,0x8c615fc2u); r[4]=r[12];goto L8c615fc4; /* mov r12,r4 */
L8c615fc4: sa_motion_publish_stop(ctx,0x8c615fc4u); r[14]+=4u;goto L8c615fc6; /* add #0x4,r14 */
L8c615fc6: sa_motion_publish_stop(ctx,0x8c615fc6u); r[8]+=16u;goto L8c615fc8; /* add #0x10,r8 */
L8c615fc8: sa_motion_publish_stop(ctx,0x8c615fc8u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615fca; /* fschg */
L8c615fca: sa_motion_publish_stop(ctx,0x8c615fcau); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615fcc; /* fmov.s @r4+,fr0 */
L8c615fcc: sa_motion_publish_stop(ctx,0x8c615fccu); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615fce; /* fmov.s @r4+,fr2 */
L8c615fce: sa_motion_publish_stop(ctx,0x8c615fceu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c615fd0; /* fldi0 fr3 */
L8c615fd0: sa_motion_publish_stop(ctx,0x8c615fd0u); fcopy(f,8u,1u);goto L8c615fd2; /* fmov fr1,fr8 */
L8c615fd2: sa_motion_publish_stop(ctx,0x8c615fd2u); fcopy(f,10u,3u);goto L8c615fd4; /* fmov fr3,fr10 */
L8c615fd4: sa_motion_publish_stop(ctx,0x8c615fd4u); status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c615fd6; /* fipr fv8,fv0 */
L8c615fd6: sa_motion_publish_stop(ctx,0x8c615fd6u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c615fd8; /* fmov.s @r4+,fr4 */
L8c615fd8: sa_motion_publish_stop(ctx,0x8c615fd8u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c615fda; /* fmov.s @r4,fr6 */
L8c615fda: sa_motion_publish_stop(ctx,0x8c615fdau); fcopy(f,8u,5u);goto L8c615fdc; /* fmov fr5,fr8 */
L8c615fdc: sa_motion_publish_stop(ctx,0x8c615fdcu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c615fde; /* fschg */
L8c615fde: sa_motion_publish_stop(ctx,0x8c615fdeu); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c615fe0; /* mov.w @r6+,r2 */
L8c615fe0: sa_motion_publish_stop(ctx,0x8c615fe0u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c615fe2; /* mov.w @r6,r3 */
L8c615fe2: sa_motion_publish_stop(ctx,0x8c615fe2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c615fe4; /* fmov.s fr6,@-r8 */
L8c615fe4: sa_motion_publish_stop(ctx,0x8c615fe4u); *fpul=r[3];goto L8c615fe6; /* lds r3,FPUL */
L8c615fe6: sa_motion_publish_stop(ctx,0x8c615fe6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c615fe8; /* fmov.s fr5,@-r8 */
L8c615fe8: sa_motion_publish_stop(ctx,0x8c615fe8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c615fea; /* fsub fr3,fr11 */
L8c615fea: sa_motion_publish_stop(ctx,0x8c615feau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c615fec; /* fmov.s fr4,@-r8 */
L8c615fec: sa_motion_publish_stop(ctx,0x8c615fecu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c615fee; /* float FPUL,fr1 */
L8c615fee: sa_motion_publish_stop(ctx,0x8c615feeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c615ff0; /* fmul fr8,fr1 */
L8c615ff0: sa_motion_publish_stop(ctx,0x8c615ff0u); *fpul=r[2];goto L8c615ff2; /* lds r2,FPUL */
L8c615ff2: sa_motion_publish_stop(ctx,0x8c615ff2u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c615ff4; /* mov.l r5,@-r8 */
L8c615ff4: sa_motion_publish_stop(ctx,0x8c615ff4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c615ff6; /* float FPUL,fr2 */
L8c615ff6: sa_motion_publish_stop(ctx,0x8c615ff6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c615ff8; /* fmul fr8,fr2 */
L8c615ff8: sa_motion_publish_stop(ctx,0x8c615ff8u); r[8]+=28u;goto L8c615ffa; /* add #0x1c,r8 */
L8c615ffa: sa_motion_publish_stop(ctx,0x8c615ffau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[8]);if(io_status)return io_status;goto L8c615ffc; /* fmov.s fr11,@-r8 */
L8c615ffc: sa_motion_publish_stop(ctx,0x8c615ffcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c615ffe; /* fmov.s fr1,@-r8 */
L8c615ffe: sa_motion_publish_stop(ctx,0x8c615ffeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616000; /* fmov.s fr2,@-r8 */
L8c616000: sa_motion_publish_stop(ctx,0x8c616000u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616002; /* pref @r8 */
L8c616002: sa_motion_publish_stop(ctx,0x8c616002u); r[8]+=16u;goto L8c616004; /* add #0x10,r8 */
L8c616004: sa_motion_publish_stop(ctx,0x8c616004u); r[3]=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;goto L8c616006; /* mov.l @(0x1c,r15),r3 */
L8c616006: sa_motion_publish_stop(ctx,0x8c616006u); r[3]+=4294967295u;goto L8c616008; /* add #-0x1,r3 */
L8c616008: sa_motion_publish_stop(ctx,0x8c616008u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c61600a; /* tst r3,r3 */
L8c61600a: sa_motion_publish_stop(ctx,0x8c61600cu); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x1cu,r[3],4);if(io_status)return io_status;if(take)goto L8c616012;goto L8c61600e; /* bt/s 0x8c616012 */
L8c61600e: sa_motion_publish_stop(ctx,0x8c616010u); goto L8c615bec; /* bra 0x8c615bec */
L8c616012: sa_motion_publish_stop(ctx,0x8c616012u); r[4]=rd(m,bus,&io_status,0x8c616060u,4);if(io_status)return io_status;goto L8c616014; /* mov.l 0x8c616060,r4 */
L8c616014: sa_motion_publish_stop(ctx,0x8c616014u); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c616016; /* mov.l @(0x10,r15),r3 */
L8c616016: sa_motion_publish_stop(ctx,0x8c616016u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616018; /* mov.l @r4,r2 */
L8c616018: sa_motion_publish_stop(ctx,0x8c616018u); r[2]+=r[3];goto L8c61601a; /* add r3,r2 */
L8c61601a: sa_motion_publish_stop(ctx,0x8c61601au); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c61601c; /* mov.l r2,@r4 */
L8c61601c: sa_motion_publish_stop(ctx,0x8c61601cu); r[4]=rd(m,bus,&io_status,0x8c616064u,4);if(io_status)return io_status;goto L8c61601e; /* mov.l 0x8c616064,r4 */
L8c61601e: sa_motion_publish_stop(ctx,0x8c61601eu); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c616020; /* mov.l @(0x10,r15),r3 */
L8c616020: sa_motion_publish_stop(ctx,0x8c616020u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616022; /* mov.l @r4,r2 */
L8c616022: sa_motion_publish_stop(ctx,0x8c616022u); r[2]+=r[3];goto L8c616024; /* add r3,r2 */
L8c616024: sa_motion_publish_stop(ctx,0x8c616024u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c616026; /* mov.l r2,@r4 */
L8c616026: sa_motion_publish_stop(ctx,0x8c616026u); r[15]+=80u;goto L8c616028; /* add #0x50,r15 */
L8c616028: sa_motion_publish_stop(ctx,0x8c616028u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61602a; /* lds.l @r15+,PR */
L8c61602a: sa_motion_publish_stop(ctx,0x8c61602au); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61602c; /* fmov.s @r15+,fr12 */
L8c61602c: sa_motion_publish_stop(ctx,0x8c61602cu); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61602e; /* fmov.s @r15+,fr13 */
L8c61602e: sa_motion_publish_stop(ctx,0x8c61602eu); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616030; /* fmov.s @r15+,fr14 */
L8c616030: sa_motion_publish_stop(ctx,0x8c616030u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616032; /* fmov.s @r15+,fr15 */
L8c616032: sa_motion_publish_stop(ctx,0x8c616032u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616034; /* mov.l @r15+,r9 */
L8c616034: sa_motion_publish_stop(ctx,0x8c616034u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616036; /* mov.l @r15+,r10 */
L8c616036: sa_motion_publish_stop(ctx,0x8c616036u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616038; /* mov.l @r15+,r11 */
L8c616038: sa_motion_publish_stop(ctx,0x8c616038u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61603a; /* mov.l @r15+,r12 */
L8c61603a: sa_motion_publish_stop(ctx,0x8c61603au); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61603c; /* mov.l @r15+,r13 */
L8c61603c: sa_motion_publish_stop(ctx,0x8c61603eu); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
