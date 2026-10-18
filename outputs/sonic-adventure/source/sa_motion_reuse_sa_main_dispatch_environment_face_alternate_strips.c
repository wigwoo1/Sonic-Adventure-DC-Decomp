#include "sa_draw_motion_stop.h"
/* Environment-face alternate strip emitters with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_environment_face_alternate_strip_8c614580(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c614580;
L8c614580: sa_motion_publish_stop(ctx,0x8c614580u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c614582; /* mov.l r14,@-r15 */
L8c614582: sa_motion_publish_stop(ctx,0x8c614582u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c614584; /* mov.l r13,@-r15 */
L8c614584: sa_motion_publish_stop(ctx,0x8c614584u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c614586; /* mov.l r12,@-r15 */
L8c614586: sa_motion_publish_stop(ctx,0x8c614586u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c614588; /* mov.l r11,@-r15 */
L8c614588: sa_motion_publish_stop(ctx,0x8c614588u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61458a; /* mov.l r10,@-r15 */
L8c61458a: sa_motion_publish_stop(ctx,0x8c61458au); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61458c; /* mov.l r9,@-r15 */
L8c61458c: sa_motion_publish_stop(ctx,0x8c61458cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61458e; /* fmov.s fr15,@-r15 */
L8c61458e: sa_motion_publish_stop(ctx,0x8c61458eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c614590; /* fmov.s fr14,@-r15 */
L8c614590: sa_motion_publish_stop(ctx,0x8c614590u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c614592; /* fmov.s fr13,@-r15 */
L8c614592: sa_motion_publish_stop(ctx,0x8c614592u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c614594; /* sts.l PR,@-r15 */
L8c614594: sa_motion_publish_stop(ctx,0x8c614594u); r[15]+=4294967280u;goto L8c614596; /* add #-0x10,r15 */
L8c614596: sa_motion_publish_stop(ctx,0x8c614596u); r[3]=rd(m,bus,&io_status,0x8c6145c8u,4);if(io_status)return io_status;goto L8c614598; /* mov.l 0x8c6145c8,r3 */
L8c614598: sa_motion_publish_stop(ctx,0x8c614598u); r[9]=rd(m,bus,&io_status,0x8c6145c4u,4);if(io_status)return io_status;goto L8c61459a; /* mov.l 0x8c6145c4,r9 */
L8c61459a: sa_motion_publish_stop(ctx,0x8c61459cu); target=r[3];*pr=0x8c61459eu;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61459e; /* jsr @r3 */
L8c61459e: sa_motion_publish_stop(ctx,0x8c61459eu); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6145a0; /* mov.l @(0x4,r15),r3 */
L8c6145a0: sa_motion_publish_stop(ctx,0x8c6145a0u); r[2]=0u;goto L8c6145a2; /* mov #0x0,r2 */
L8c6145a2: sa_motion_publish_stop(ctx,0x8c6145a2u); r[14]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6145a4; /* mov.l @(0x4,r15),r14 */
L8c6145a4: sa_motion_publish_stop(ctx,0x8c6145a4u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6145a6; /* mov.w @(0x2,r3),r0 */
L8c6145a6: sa_motion_publish_stop(ctx,0x8c6145a6u); r[3]=rd(m,bus,&io_status,0x8c6145ccu,4);if(io_status)return io_status;goto L8c6145a8; /* mov.l 0x8c6145cc,r3 */
L8c6145a8: sa_motion_publish_stop(ctx,0x8c6145a8u); r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c6145aa; /* mov.l @(0x4,r14),r14 */
L8c6145aa: sa_motion_publish_stop(ctx,0x8c6145aau); r[0]=r[0]&65535u;goto L8c6145ac; /* extu.w r0,r0 */
L8c6145ac: sa_motion_publish_stop(ctx,0x8c6145acu); wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c6145ae; /* mov.l r0,@(0xc,r15) */
L8c6145ae: sa_motion_publish_stop(ctx,0x8c6145aeu); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6145b0; /* mov.l @r3,r13 */
L8c6145b0: sa_motion_publish_stop(ctx,0x8c6145b0u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6145b2; /* mov.l r2,@(0x8,r15) */
L8c6145b2: sa_motion_publish_stop(ctx,0x8c6145b2u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6145b4; /* mov.l r2,@(0x4,r15) */
L8c6145b4: sa_motion_publish_stop(ctx,0x8c6145b6u); goto L8c6145d0; /* bra 0x8c6145d0 */
L8c6145d0: sa_motion_publish_stop(ctx,0x8c6145d0u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6145d2; /* mov.w @r14+,r3 */
L8c6145d2: sa_motion_publish_stop(ctx,0x8c6145d2u); r[4]=rd(m,bus,&io_status,0x8c6146a4u,4);if(io_status)return io_status;goto L8c6145d4; /* mov.l 0x8c6146a4,r4 */
L8c6145d4: sa_motion_publish_stop(ctx,0x8c6145d4u); r[3]=r[3]&65535u;goto L8c6145d6; /* extu.w r3,r3 */
L8c6145d6: sa_motion_publish_stop(ctx,0x8c6145d6u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6145d8; /* fldi1 fr4 */
L8c6145d8: sa_motion_publish_stop(ctx,0x8c6145d8u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6145da; /* tst r4,r3 */
L8c6145da: sa_motion_publish_stop(ctx,0x8c6145dau); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6145dc; /* mov.l r3,@r15 */
L8c6145dc: sa_motion_publish_stop(ctx,0x8c6145deu); take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c6145ee;goto L8c6145e0; /* bt/s 0x8c6145ee */
L8c6145e0: sa_motion_publish_stop(ctx,0x8c6145e0u); r[4]=sx(m,bus,&io_status,0x8c6146a0u,2);if(io_status)return io_status;goto L8c6145e2; /* mov.w 0x8c6146a0,r4 */
L8c6145e2: sa_motion_publish_stop(ctx,0x8c6145e2u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6145e4; /* mov.l @r15,r3 */
L8c6145e4: sa_motion_publish_stop(ctx,0x8c6145e4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6145e6; /* fldi1 fr4 */
L8c6145e6: sa_motion_publish_stop(ctx,0x8c6145e6u); r[3]&=r[4];goto L8c6145e8; /* and r4,r3 */
L8c6145e8: sa_motion_publish_stop(ctx,0x8c6145e8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c6145ea; /* fneg fr4 */
L8c6145ea: sa_motion_publish_stop(ctx,0x8c6145eau); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6145ec; /* mov.l r3,@r15 */
L8c6145ec: sa_motion_publish_stop(ctx,0x8c6145ecu); fcopy(f,15u,4u);goto L8c6145ee; /* fmov fr4,fr15 */
L8c6145ee: sa_motion_publish_stop(ctx,0x8c6145eeu); r[0]=16u;goto L8c6145f0; /* mov #0x10,r0 */
L8c6145f0: sa_motion_publish_stop(ctx,0x8c6145f0u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6145f2; /* mov.l @r15,r3 */
L8c6145f2: sa_motion_publish_stop(ctx,0x8c6145f2u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6145f4; /* mov.l @(0x4,r15),r2 */
L8c6145f4: sa_motion_publish_stop(ctx,0x8c6145f4u); r[10]=r[8];goto L8c6145f6; /* mov r8,r10 */
L8c6145f6: sa_motion_publish_stop(ctx,0x8c6145f6u); r[2]+=r[3];goto L8c6145f8; /* add r3,r2 */
L8c6145f8: sa_motion_publish_stop(ctx,0x8c6145f8u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c6145fa; /* mov.l r2,@(0x4,r15) */
L8c6145fa: sa_motion_publish_stop(ctx,0x8c6145fau); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6145fc; /* mov.l @r15,r1 */
L8c6145fc: sa_motion_publish_stop(ctx,0x8c6145fcu); r[1]+=4294967294u;goto L8c6145fe; /* add #-0x2,r1 */
L8c6145fe: sa_motion_publish_stop(ctx,0x8c6145feu); wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c614600; /* mov.l r1,@r15 */
L8c614600: sa_motion_publish_stop(ctx,0x8c614600u); r[4]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614602; /* mov.w @r14+,r4 */
L8c614602: sa_motion_publish_stop(ctx,0x8c614602u); r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614604; /* mov.w @r14+,r11 */
L8c614604: sa_motion_publish_stop(ctx,0x8c614604u); r[4]=r[4]&65535u;goto L8c614606; /* extu.w r4,r4 */
L8c614606: sa_motion_publish_stop(ctx,0x8c614606u); r[4]<<=2;goto L8c614608; /* shll2 r4 */
L8c614608: sa_motion_publish_stop(ctx,0x8c614608u); r[11]=r[11]&65535u;goto L8c61460a; /* extu.w r11,r11 */
L8c61460a: sa_motion_publish_stop(ctx,0x8c61460au); r[11]<<=2;goto L8c61460c; /* shll2 r11 */
L8c61460c: sa_motion_publish_stop(ctx,0x8c61460cu); r[4]<<=2;goto L8c61460e; /* shll2 r4 */
L8c61460e: sa_motion_publish_stop(ctx,0x8c61460eu); r[11]<<=2;goto L8c614610; /* shll2 r11 */
L8c614610: sa_motion_publish_stop(ctx,0x8c614610u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c614612; /* shll r4 */
L8c614612: sa_motion_publish_stop(ctx,0x8c614612u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c614614; /* shll r11 */
L8c614614: sa_motion_publish_stop(ctx,0x8c614614u); r[11]+=r[13];goto L8c614616; /* add r13,r11 */
L8c614616: sa_motion_publish_stop(ctx,0x8c614616u); r[4]+=r[13];goto L8c614618; /* add r13,r4 */
L8c614618: sa_motion_publish_stop(ctx,0x8c614618u); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c61461a; /* fmov.s @(r0,r11),fr14 */
L8c61461a: sa_motion_publish_stop(ctx,0x8c61461au); fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c61461c; /* fmov.s @(r0,r4),fr4 */
L8c61461c: sa_motion_publish_stop(ctx,0x8c61461cu); r[0]=20u;goto L8c61461e; /* mov #0x14,r0 */
L8c61461e: sa_motion_publish_stop(ctx,0x8c61461eu); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c614620; /* fmov.s @(r0,r11),fr13 */
L8c614620: sa_motion_publish_stop(ctx,0x8c614620u); fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c614622; /* fmov.s @(r0,r4),fr5 */
L8c614622: sa_motion_publish_stop(ctx,0x8c614622u); r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c614624; /* mov.w @r14+,r12 */
L8c614624: sa_motion_publish_stop(ctx,0x8c614624u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c614626; /* mov.w @r14,r3 */
L8c614626: sa_motion_publish_stop(ctx,0x8c614626u); r[12]=r[12]&65535u;goto L8c614628; /* extu.w r12,r12 */
L8c614628: sa_motion_publish_stop(ctx,0x8c614628u); r[12]<<=2;goto L8c61462a; /* shll2 r12 */
L8c61462a: sa_motion_publish_stop(ctx,0x8c61462au); r[3]=r[3]&65535u;goto L8c61462c; /* extu.w r3,r3 */
L8c61462c: sa_motion_publish_stop(ctx,0x8c61462cu); r[3]<<=2;goto L8c61462e; /* shll2 r3 */
L8c61462e: sa_motion_publish_stop(ctx,0x8c61462eu); r[12]<<=2;goto L8c614630; /* shll2 r12 */
L8c614630: sa_motion_publish_stop(ctx,0x8c614630u); r[3]<<=2;goto L8c614632; /* shll2 r3 */
L8c614632: sa_motion_publish_stop(ctx,0x8c614632u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c614634; /* shll r12 */
L8c614634: sa_motion_publish_stop(ctx,0x8c614634u); *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c614636; /* shll r3 */
L8c614636: sa_motion_publish_stop(ctx,0x8c614636u); r[3]+=r[13];goto L8c614638; /* add r13,r3 */
L8c614638: sa_motion_publish_stop(ctx,0x8c614638u); r[12]+=r[13];goto L8c61463a; /* add r13,r12 */
L8c61463a: sa_motion_publish_stop(ctx,0x8c61463au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c61463c; /* pref @r3 */
L8c61463c: sa_motion_publish_stop(ctx,0x8c61463cu); r[0]=20u;goto L8c61463e; /* mov #0x14,r0 */
L8c61463e: sa_motion_publish_stop(ctx,0x8c61463eu); fcopy(f,3u,14u);goto L8c614640; /* fmov fr14,fr3 */
L8c614640: sa_motion_publish_stop(ctx,0x8c614640u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c614642; /* fmov.s @(r0,r12),fr2 */
L8c614642: sa_motion_publish_stop(ctx,0x8c614642u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614644; /* fsub fr4,fr3 */
L8c614644: sa_motion_publish_stop(ctx,0x8c614644u); r[0]=16u;goto L8c614646; /* mov #0x10,r0 */
L8c614646: sa_motion_publish_stop(ctx,0x8c614646u); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c614648; /* cmp/hs r8,r10 */
L8c614648: sa_motion_publish_stop(ctx,0x8c614648u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61464a; /* fsub fr13,fr2 */
L8c61464a: sa_motion_publish_stop(ctx,0x8c61464au); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c61464c; /* fmov.s @(r0,r12),fr1 */
L8c61464c: sa_motion_publish_stop(ctx,0x8c61464cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61464e; /* fsub fr14,fr1 */
L8c61464e: sa_motion_publish_stop(ctx,0x8c61464eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c614650; /* fmul fr2,fr3 */
L8c614650: sa_motion_publish_stop(ctx,0x8c614650u); fcopy(f,2u,13u);goto L8c614652; /* fmov fr13,fr2 */
L8c614652: sa_motion_publish_stop(ctx,0x8c614652u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c614654; /* fsub fr5,fr2 */
L8c614654: sa_motion_publish_stop(ctx,0x8c614654u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c614656; /* fmul fr1,fr2 */
L8c614656: sa_motion_publish_stop(ctx,0x8c614656u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c614658; /* fsub fr2,fr3 */
L8c614658: sa_motion_publish_stop(ctx,0x8c614658u); fcopy(f,4u,3u);goto L8c61465a; /* fmov fr3,fr4 */
L8c61465a: sa_motion_publish_stop(ctx,0x8c61465cu); take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c6146a8;goto L8c61465e; /* bt/s 0x8c6146a8 */
L8c61465e: sa_motion_publish_stop(ctx,0x8c61465eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614660; /* fldi0 fr3 */
L8c614660: sa_motion_publish_stop(ctx,0x8c614660u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c614662; /* fcmp/gt fr4,fr3 */
L8c614662: sa_motion_publish_stop(ctx,0x8c614662u); if((*sr&1u))goto L8c6146d6;goto L8c614664; /* bt 0x8c6146d6 */
L8c614664: sa_motion_publish_stop(ctx,0x8c614664u); r[4]=r[11];goto L8c614666; /* mov r11,r4 */
L8c614666: sa_motion_publish_stop(ctx,0x8c614666u); r[5]=r[9];goto L8c614668; /* mov r9,r5 */
L8c614668: sa_motion_publish_stop(ctx,0x8c614668u); r[8]+=16u;goto L8c61466a; /* add #0x10,r8 */
L8c61466a: sa_motion_publish_stop(ctx,0x8c61466au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61466c; /* fschg */
L8c61466c: sa_motion_publish_stop(ctx,0x8c61466cu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61466e; /* fmov.s @r4+,fr0 */
L8c61466e: sa_motion_publish_stop(ctx,0x8c61466eu); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614670; /* fmov.s @r4+,fr2 */
L8c614670: sa_motion_publish_stop(ctx,0x8c614670u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c614672; /* fldi1 fr3 */
L8c614672: sa_motion_publish_stop(ctx,0x8c614672u); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c614674; /* ftrv xmtrx,fv0 */
L8c614674: sa_motion_publish_stop(ctx,0x8c614674u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614676; /* fmov.s @r4+,fr4 */
L8c614676: sa_motion_publish_stop(ctx,0x8c614676u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614678; /* fmov.s @r4,fr6 */
L8c614678: sa_motion_publish_stop(ctx,0x8c614678u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61467a; /* fschg */
L8c61467a: sa_motion_publish_stop(ctx,0x8c61467au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c61467c; /* fmov.s fr6,@-r8 */
L8c61467c: sa_motion_publish_stop(ctx,0x8c61467cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61467e; /* fmov.s fr5,@-r8 */
L8c61467e: sa_motion_publish_stop(ctx,0x8c61467eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c614680; /* fmov.s fr4,@-r8 */
L8c614680: sa_motion_publish_stop(ctx,0x8c614680u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614682; /* mov.l r5,@-r8 */
L8c614682: sa_motion_publish_stop(ctx,0x8c614682u); r[8]+=28u;goto L8c614684; /* add #0x1c,r8 */
L8c614684: sa_motion_publish_stop(ctx,0x8c614684u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614686; /* fmov.s fr2,@-r8 */
L8c614686: sa_motion_publish_stop(ctx,0x8c614686u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c614688; /* fmov.s fr1,@-r8 */
L8c614688: sa_motion_publish_stop(ctx,0x8c614688u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c61468a; /* fmov.s fr0,@-r8 */
L8c61468a: sa_motion_publish_stop(ctx,0x8c61468au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61468c; /* pref @r8 */
L8c61468c: sa_motion_publish_stop(ctx,0x8c61468cu); r[8]+=16u;goto L8c61468e; /* add #0x10,r8 */
L8c61468e: sa_motion_publish_stop(ctx,0x8c61468eu); r[3]=r[8];goto L8c614690; /* mov r8,r3 */
L8c614690: sa_motion_publish_stop(ctx,0x8c614690u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614692; /* mov.l @(0x8,r15),r2 */
L8c614692: sa_motion_publish_stop(ctx,0x8c614692u); r[3]-=r[10];goto L8c614694; /* sub r10,r3 */
L8c614694: sa_motion_publish_stop(ctx,0x8c614694u); r[3]+=32u;goto L8c614696; /* add #0x20,r3 */
L8c614696: sa_motion_publish_stop(ctx,0x8c614696u); r[2]+=r[3];goto L8c614698; /* add r3,r2 */
L8c614698: sa_motion_publish_stop(ctx,0x8c614698u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c61469a; /* mov.l r2,@(0x8,r15) */
L8c61469a: sa_motion_publish_stop(ctx,0x8c61469cu); r[10]=r[8];goto L8c614700; /* bra 0x8c614700 */
L8c6146a8: sa_motion_publish_stop(ctx,0x8c6146a8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6146aa; /* fldi0 fr3 */
L8c6146aa: sa_motion_publish_stop(ctx,0x8c6146aau); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6146ac; /* fcmp/gt fr4,fr3 */
L8c6146ac: sa_motion_publish_stop(ctx,0x8c6146acu); if(!(*sr&1u))goto L8c614700;goto L8c6146ae; /* bf 0x8c614700 */
L8c6146ae: sa_motion_publish_stop(ctx,0x8c6146aeu); r[5]=rd(m,bus,&io_status,0x8c6147d0u,4);if(io_status)return io_status;goto L8c6146b0; /* mov.l 0x8c6147d0,r5 */
L8c6146b0: sa_motion_publish_stop(ctx,0x8c6146b0u); r[8]+=16u;goto L8c6146b2; /* add #0x10,r8 */
L8c6146b2: sa_motion_publish_stop(ctx,0x8c6146b2u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6146b4; /* fschg */
L8c6146b4: sa_motion_publish_stop(ctx,0x8c6146b4u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6146b6; /* fmov.s @r4+,fr0 */
L8c6146b6: sa_motion_publish_stop(ctx,0x8c6146b6u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6146b8; /* fmov.s @r4+,fr2 */
L8c6146b8: sa_motion_publish_stop(ctx,0x8c6146b8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6146ba; /* fldi1 fr3 */
L8c6146ba: sa_motion_publish_stop(ctx,0x8c6146bau); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6146bc; /* ftrv xmtrx,fv0 */
L8c6146bc: sa_motion_publish_stop(ctx,0x8c6146bcu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6146be; /* fmov.s @r4+,fr4 */
L8c6146be: sa_motion_publish_stop(ctx,0x8c6146beu); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6146c0; /* fmov.s @r4,fr6 */
L8c6146c0: sa_motion_publish_stop(ctx,0x8c6146c0u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6146c2; /* fschg */
L8c6146c2: sa_motion_publish_stop(ctx,0x8c6146c2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6146c4; /* fmov.s fr6,@-r8 */
L8c6146c4: sa_motion_publish_stop(ctx,0x8c6146c4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6146c6; /* fmov.s fr5,@-r8 */
L8c6146c6: sa_motion_publish_stop(ctx,0x8c6146c6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6146c8; /* fmov.s fr4,@-r8 */
L8c6146c8: sa_motion_publish_stop(ctx,0x8c6146c8u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6146ca; /* mov.l r5,@-r8 */
L8c6146ca: sa_motion_publish_stop(ctx,0x8c6146cau); r[8]+=28u;goto L8c6146cc; /* add #0x1c,r8 */
L8c6146cc: sa_motion_publish_stop(ctx,0x8c6146ccu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6146ce; /* fmov.s fr2,@-r8 */
L8c6146ce: sa_motion_publish_stop(ctx,0x8c6146ceu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6146d0; /* fmov.s fr1,@-r8 */
L8c6146d0: sa_motion_publish_stop(ctx,0x8c6146d0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c6146d2; /* fmov.s fr0,@-r8 */
L8c6146d2: sa_motion_publish_stop(ctx,0x8c6146d2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6146d4; /* pref @r8 */
L8c6146d4: sa_motion_publish_stop(ctx,0x8c6146d4u); r[8]+=16u;goto L8c6146d6; /* add #0x10,r8 */
L8c6146d6: sa_motion_publish_stop(ctx,0x8c6146d6u); r[5]=rd(m,bus,&io_status,0x8c6147d0u,4);if(io_status)return io_status;goto L8c6146d8; /* mov.l 0x8c6147d0,r5 */
L8c6146d8: sa_motion_publish_stop(ctx,0x8c6146d8u); r[4]=r[11];goto L8c6146da; /* mov r11,r4 */
L8c6146da: sa_motion_publish_stop(ctx,0x8c6146dau); r[8]+=16u;goto L8c6146dc; /* add #0x10,r8 */
L8c6146dc: sa_motion_publish_stop(ctx,0x8c6146dcu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6146de; /* fschg */
L8c6146de: sa_motion_publish_stop(ctx,0x8c6146deu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6146e0; /* fmov.s @r4+,fr0 */
L8c6146e0: sa_motion_publish_stop(ctx,0x8c6146e0u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6146e2; /* fmov.s @r4+,fr2 */
L8c6146e2: sa_motion_publish_stop(ctx,0x8c6146e2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6146e4; /* fldi1 fr3 */
L8c6146e4: sa_motion_publish_stop(ctx,0x8c6146e4u); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6146e6; /* ftrv xmtrx,fv0 */
L8c6146e6: sa_motion_publish_stop(ctx,0x8c6146e6u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6146e8; /* fmov.s @r4+,fr4 */
L8c6146e8: sa_motion_publish_stop(ctx,0x8c6146e8u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6146ea; /* fmov.s @r4,fr6 */
L8c6146ea: sa_motion_publish_stop(ctx,0x8c6146eau); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6146ec; /* fschg */
L8c6146ec: sa_motion_publish_stop(ctx,0x8c6146ecu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6146ee; /* fmov.s fr6,@-r8 */
L8c6146ee: sa_motion_publish_stop(ctx,0x8c6146eeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6146f0; /* fmov.s fr5,@-r8 */
L8c6146f0: sa_motion_publish_stop(ctx,0x8c6146f0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6146f2; /* fmov.s fr4,@-r8 */
L8c6146f2: sa_motion_publish_stop(ctx,0x8c6146f2u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6146f4; /* mov.l r5,@-r8 */
L8c6146f4: sa_motion_publish_stop(ctx,0x8c6146f4u); r[8]+=28u;goto L8c6146f6; /* add #0x1c,r8 */
L8c6146f6: sa_motion_publish_stop(ctx,0x8c6146f6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6146f8; /* fmov.s fr2,@-r8 */
L8c6146f8: sa_motion_publish_stop(ctx,0x8c6146f8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6146fa; /* fmov.s fr1,@-r8 */
L8c6146fa: sa_motion_publish_stop(ctx,0x8c6146fau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c6146fc; /* fmov.s fr0,@-r8 */
L8c6146fc: sa_motion_publish_stop(ctx,0x8c6146fcu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6146fe; /* pref @r8 */
L8c6146fe: sa_motion_publish_stop(ctx,0x8c6146feu); r[8]+=16u;goto L8c614700; /* add #0x10,r8 */
L8c614700: sa_motion_publish_stop(ctx,0x8c614700u); fcopy(f,4u,14u);goto L8c614702; /* fmov fr14,fr4 */
L8c614702: sa_motion_publish_stop(ctx,0x8c614702u); r[0]=16u;goto L8c614704; /* mov #0x10,r0 */
L8c614704: sa_motion_publish_stop(ctx,0x8c614704u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614706; /* mov.l @r15,r3 */
L8c614706: sa_motion_publish_stop(ctx,0x8c614706u); r[4]=r[11];goto L8c614708; /* mov r11,r4 */
L8c614708: sa_motion_publish_stop(ctx,0x8c614708u); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c61470a; /* fmov.s @(r0,r12),fr14 */
L8c61470a: sa_motion_publish_stop(ctx,0x8c61470au); r[0]=20u;goto L8c61470c; /* mov #0x14,r0 */
L8c61470c: sa_motion_publish_stop(ctx,0x8c61470cu); fcopy(f,5u,13u);goto L8c61470e; /* fmov fr13,fr5 */
L8c61470e: sa_motion_publish_stop(ctx,0x8c61470eu); r[3]+=4294967295u;goto L8c614710; /* add #-0x1,r3 */
L8c614710: sa_motion_publish_stop(ctx,0x8c614710u); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c614712; /* fmov.s @(r0,r12),fr13 */
L8c614712: sa_motion_publish_stop(ctx,0x8c614712u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614714; /* tst r3,r3 */
L8c614714: sa_motion_publish_stop(ctx,0x8c614714u); r[11]=r[12];goto L8c614716; /* mov r12,r11 */
L8c614716: sa_motion_publish_stop(ctx,0x8c614716u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c614718; /* fneg fr15 */
L8c614718: sa_motion_publish_stop(ctx,0x8c61471au); take=!(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c614622;goto L8c61471c; /* bf/s 0x8c614622 */
L8c61471c: sa_motion_publish_stop(ctx,0x8c61471cu); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c61471e; /* cmp/hs r8,r10 */
L8c61471e: sa_motion_publish_stop(ctx,0x8c61471eu); if((*sr&1u))goto L8c614756;goto L8c614720; /* bt 0x8c614756 */
L8c614720: sa_motion_publish_stop(ctx,0x8c614720u); r[4]=r[12];goto L8c614722; /* mov r12,r4 */
L8c614722: sa_motion_publish_stop(ctx,0x8c614722u); r[5]=r[9];goto L8c614724; /* mov r9,r5 */
L8c614724: sa_motion_publish_stop(ctx,0x8c614724u); r[8]+=16u;goto L8c614726; /* add #0x10,r8 */
L8c614726: sa_motion_publish_stop(ctx,0x8c614726u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614728; /* fschg */
L8c614728: sa_motion_publish_stop(ctx,0x8c614728u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61472a; /* fmov.s @r4+,fr0 */
L8c61472a: sa_motion_publish_stop(ctx,0x8c61472au); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61472c; /* fmov.s @r4+,fr2 */
L8c61472c: sa_motion_publish_stop(ctx,0x8c61472cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c61472e; /* fldi1 fr3 */
L8c61472e: sa_motion_publish_stop(ctx,0x8c61472eu); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c614730; /* ftrv xmtrx,fv0 */
L8c614730: sa_motion_publish_stop(ctx,0x8c614730u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614732; /* fmov.s @r4+,fr4 */
L8c614732: sa_motion_publish_stop(ctx,0x8c614732u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c614734; /* fmov.s @r4,fr6 */
L8c614734: sa_motion_publish_stop(ctx,0x8c614734u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614736; /* fschg */
L8c614736: sa_motion_publish_stop(ctx,0x8c614736u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614738; /* fmov.s fr6,@-r8 */
L8c614738: sa_motion_publish_stop(ctx,0x8c614738u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61473a; /* fmov.s fr5,@-r8 */
L8c61473a: sa_motion_publish_stop(ctx,0x8c61473au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61473c; /* fmov.s fr4,@-r8 */
L8c61473c: sa_motion_publish_stop(ctx,0x8c61473cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61473e; /* mov.l r5,@-r8 */
L8c61473e: sa_motion_publish_stop(ctx,0x8c61473eu); r[8]+=28u;goto L8c614740; /* add #0x1c,r8 */
L8c614740: sa_motion_publish_stop(ctx,0x8c614740u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614742; /* fmov.s fr2,@-r8 */
L8c614742: sa_motion_publish_stop(ctx,0x8c614742u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c614744; /* fmov.s fr1,@-r8 */
L8c614744: sa_motion_publish_stop(ctx,0x8c614744u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c614746; /* fmov.s fr0,@-r8 */
L8c614746: sa_motion_publish_stop(ctx,0x8c614746u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614748; /* pref @r8 */
L8c614748: sa_motion_publish_stop(ctx,0x8c614748u); r[8]+=16u;goto L8c61474a; /* add #0x10,r8 */
L8c61474a: sa_motion_publish_stop(ctx,0x8c61474au); r[3]=r[8];goto L8c61474c; /* mov r8,r3 */
L8c61474c: sa_motion_publish_stop(ctx,0x8c61474cu); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61474e; /* mov.l @(0x8,r15),r2 */
L8c61474e: sa_motion_publish_stop(ctx,0x8c61474eu); r[3]-=r[10];goto L8c614750; /* sub r10,r3 */
L8c614750: sa_motion_publish_stop(ctx,0x8c614750u); r[3]+=32u;goto L8c614752; /* add #0x20,r3 */
L8c614752: sa_motion_publish_stop(ctx,0x8c614752u); r[2]+=r[3];goto L8c614754; /* add r3,r2 */
L8c614754: sa_motion_publish_stop(ctx,0x8c614754u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c614756; /* mov.l r2,@(0x8,r15) */
L8c614756: sa_motion_publish_stop(ctx,0x8c614756u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614758; /* mov.l @(0xc,r15),r3 */
L8c614758: sa_motion_publish_stop(ctx,0x8c614758u); r[3]+=4294967295u;goto L8c61475a; /* add #-0x1,r3 */
L8c61475a: sa_motion_publish_stop(ctx,0x8c61475au); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c61475c; /* tst r3,r3 */
L8c61475c: sa_motion_publish_stop(ctx,0x8c61475eu); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c614764;goto L8c614760; /* bt/s 0x8c614764 */
L8c614760: sa_motion_publish_stop(ctx,0x8c614762u); goto L8c6145d0; /* bra 0x8c6145d0 */
L8c614764: sa_motion_publish_stop(ctx,0x8c614764u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614766; /* mov.l @(0x8,r15),r3 */
L8c614766: sa_motion_publish_stop(ctx,0x8c614766u); r[4]=rd(m,bus,&io_status,0x8c6147d4u,4);if(io_status)return io_status;goto L8c614768; /* mov.l 0x8c6147d4,r4 */
L8c614768: sa_motion_publish_stop(ctx,0x8c614768u); r[3]>>=2;goto L8c61476a; /* shlr2 r3 */
L8c61476a: sa_motion_publish_stop(ctx,0x8c61476au); r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61476c; /* mov.l @r4,r1 */
L8c61476c: sa_motion_publish_stop(ctx,0x8c61476cu); r[3]>>=2;goto L8c61476e; /* shlr2 r3 */
L8c61476e: sa_motion_publish_stop(ctx,0x8c61476eu); *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c614770; /* shlr r3 */
L8c614770: sa_motion_publish_stop(ctx,0x8c614770u); r[1]+=r[3];goto L8c614772; /* add r3,r1 */
L8c614772: sa_motion_publish_stop(ctx,0x8c614772u); wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c614774; /* mov.l r1,@r4 */
L8c614774: sa_motion_publish_stop(ctx,0x8c614774u); r[4]=rd(m,bus,&io_status,0x8c6147d8u,4);if(io_status)return io_status;goto L8c614776; /* mov.l 0x8c6147d8,r4 */
L8c614776: sa_motion_publish_stop(ctx,0x8c614776u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614778; /* mov.l @(0x4,r15),r3 */
L8c614778: sa_motion_publish_stop(ctx,0x8c614778u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61477a; /* mov.l @r4,r2 */
L8c61477a: sa_motion_publish_stop(ctx,0x8c61477au); r[2]+=r[3];goto L8c61477c; /* add r3,r2 */
L8c61477c: sa_motion_publish_stop(ctx,0x8c61477cu); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c61477e; /* mov.l r2,@r4 */
L8c61477e: sa_motion_publish_stop(ctx,0x8c61477eu); r[15]+=16u;goto L8c614780; /* add #0x10,r15 */
L8c614780: sa_motion_publish_stop(ctx,0x8c614780u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614782; /* lds.l @r15+,PR */
L8c614782: sa_motion_publish_stop(ctx,0x8c614782u); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614784; /* fmov.s @r15+,fr13 */
L8c614784: sa_motion_publish_stop(ctx,0x8c614784u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614786; /* fmov.s @r15+,fr14 */
L8c614786: sa_motion_publish_stop(ctx,0x8c614786u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614788; /* fmov.s @r15+,fr15 */
L8c614788: sa_motion_publish_stop(ctx,0x8c614788u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61478a; /* mov.l @r15+,r9 */
L8c61478a: sa_motion_publish_stop(ctx,0x8c61478au); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61478c; /* mov.l @r15+,r10 */
L8c61478c: sa_motion_publish_stop(ctx,0x8c61478cu); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61478e; /* mov.l @r15+,r11 */
L8c61478e: sa_motion_publish_stop(ctx,0x8c61478eu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614790; /* mov.l @r15+,r12 */
L8c614790: sa_motion_publish_stop(ctx,0x8c614790u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614792; /* mov.l @r15+,r13 */
L8c614792: sa_motion_publish_stop(ctx,0x8c614794u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_environment_face_alternate_strip_clipped_8c616474(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c616474;
L8c616474: sa_motion_publish_stop(ctx,0x8c616474u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c616476; /* mov.l r14,@-r15 */
L8c616476: sa_motion_publish_stop(ctx,0x8c616476u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c616478; /* mov.l r13,@-r15 */
L8c616478: sa_motion_publish_stop(ctx,0x8c616478u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61647a; /* mov.l r12,@-r15 */
L8c61647a: sa_motion_publish_stop(ctx,0x8c61647au); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61647c; /* mov.l r11,@-r15 */
L8c61647c: sa_motion_publish_stop(ctx,0x8c61647cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61647e; /* mov.l r10,@-r15 */
L8c61647e: sa_motion_publish_stop(ctx,0x8c61647eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c616480; /* mov.l r9,@-r15 */
L8c616480: sa_motion_publish_stop(ctx,0x8c616480u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c616482; /* fmov.s fr15,@-r15 */
L8c616482: sa_motion_publish_stop(ctx,0x8c616482u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c616484; /* fmov.s fr14,@-r15 */
L8c616484: sa_motion_publish_stop(ctx,0x8c616484u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c616486; /* fmov.s fr13,@-r15 */
L8c616486: sa_motion_publish_stop(ctx,0x8c616486u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c616488; /* fmov.s fr12,@-r15 */
L8c616488: sa_motion_publish_stop(ctx,0x8c616488u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61648a; /* sts.l PR,@-r15 */
L8c61648a: sa_motion_publish_stop(ctx,0x8c61648au); r[15]+=4294967220u;goto L8c61648c; /* add #-0x4c,r15 */
L8c61648c: sa_motion_publish_stop(ctx,0x8c61648cu); r[3]=rd(m,bus,&io_status,0x8c6164c8u,4);if(io_status)return io_status;goto L8c61648e; /* mov.l 0x8c6164c8,r3 */
L8c61648e: sa_motion_publish_stop(ctx,0x8c61648eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c616490; /* fldi1 fr12 */
L8c616490: sa_motion_publish_stop(ctx,0x8c616492u); target=r[3];*pr=0x8c616494u;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c616494; /* jsr @r3 */
L8c616494: sa_motion_publish_stop(ctx,0x8c616494u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c616496; /* mov.l @(0x8,r15),r3 */
L8c616496: sa_motion_publish_stop(ctx,0x8c616496u); r[2]=0u;goto L8c616498; /* mov #0x0,r2 */
L8c616498: sa_motion_publish_stop(ctx,0x8c616498u); r[14]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61649a; /* mov.l @(0x8,r15),r14 */
L8c61649a: sa_motion_publish_stop(ctx,0x8c61649au); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61649c; /* mov.w @(0x2,r3),r0 */
L8c61649c: sa_motion_publish_stop(ctx,0x8c61649cu); r[3]=rd(m,bus,&io_status,0x8c6164ccu,4);if(io_status)return io_status;goto L8c61649e; /* mov.l 0x8c6164cc,r3 */
L8c61649e: sa_motion_publish_stop(ctx,0x8c61649eu); r[14]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c6164a0; /* mov.l @(0x4,r14),r14 */
L8c6164a0: sa_motion_publish_stop(ctx,0x8c6164a0u); r[0]=r[0]&65535u;goto L8c6164a2; /* extu.w r0,r0 */
L8c6164a2: sa_motion_publish_stop(ctx,0x8c6164a2u); wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c6164a4; /* mov.l r0,@(0x18,r15) */
L8c6164a4: sa_motion_publish_stop(ctx,0x8c6164a4u); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6164a6; /* mov.l @r3,r13 */
L8c6164a6: sa_motion_publish_stop(ctx,0x8c6164a6u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6164a8; /* mov.l r2,@(0x8,r15) */
L8c6164a8: sa_motion_publish_stop(ctx,0x8c6164aau); goto L8c6164d0; /* bra 0x8c6164d0 */
L8c6164d0: sa_motion_publish_stop(ctx,0x8c6164d0u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c6164d2; /* mov.w @r14+,r3 */
L8c6164d2: sa_motion_publish_stop(ctx,0x8c6164d2u); r[4]=rd(m,bus,&io_status,0x8c6165ecu,4);if(io_status)return io_status;goto L8c6164d4; /* mov.l 0x8c6165ec,r4 */
L8c6164d4: sa_motion_publish_stop(ctx,0x8c6164d4u); r[3]=r[3]&65535u;goto L8c6164d6; /* extu.w r3,r3 */
L8c6164d6: sa_motion_publish_stop(ctx,0x8c6164d6u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6164d8; /* tst r4,r3 */
L8c6164d8: sa_motion_publish_stop(ctx,0x8c6164d8u); wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6164da; /* mov.l r3,@(0xc,r15) */
L8c6164da: sa_motion_publish_stop(ctx,0x8c6164dcu); take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c6164ec;goto L8c6164de; /* bt/s 0x8c6164ec */
L8c6164de: sa_motion_publish_stop(ctx,0x8c6164deu); r[4]=sx(m,bus,&io_status,0x8c6165e8u,2);if(io_status)return io_status;goto L8c6164e0; /* mov.w 0x8c6165e8,r4 */
L8c6164e0: sa_motion_publish_stop(ctx,0x8c6164e0u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6164e2; /* mov.l @(0xc,r15),r3 */
L8c6164e2: sa_motion_publish_stop(ctx,0x8c6164e2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6164e4; /* fldi1 fr4 */
L8c6164e4: sa_motion_publish_stop(ctx,0x8c6164e4u); r[3]&=r[4];goto L8c6164e6; /* and r4,r3 */
L8c6164e6: sa_motion_publish_stop(ctx,0x8c6164e6u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c6164e8; /* fneg fr4 */
L8c6164e8: sa_motion_publish_stop(ctx,0x8c6164e8u); wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6164ea; /* mov.l r3,@(0xc,r15) */
L8c6164ea: sa_motion_publish_stop(ctx,0x8c6164eau); fcopy(f,15u,4u);goto L8c6164ec; /* fmov fr4,fr15 */
L8c6164ec: sa_motion_publish_stop(ctx,0x8c6164ecu); r[0]=16u;goto L8c6164ee; /* mov #0x10,r0 */
L8c6164ee: sa_motion_publish_stop(ctx,0x8c6164eeu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6164f0; /* mov.l @(0xc,r15),r3 */
L8c6164f0: sa_motion_publish_stop(ctx,0x8c6164f0u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6164f2; /* mov.l @(0x8,r15),r2 */
L8c6164f2: sa_motion_publish_stop(ctx,0x8c6164f2u); r[9]=r[8];goto L8c6164f4; /* mov r8,r9 */
L8c6164f4: sa_motion_publish_stop(ctx,0x8c6164f4u); r[2]+=r[3];goto L8c6164f6; /* add r3,r2 */
L8c6164f6: sa_motion_publish_stop(ctx,0x8c6164f6u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6164f8; /* mov.l r2,@(0x8,r15) */
L8c6164f8: sa_motion_publish_stop(ctx,0x8c6164f8u); r[1]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6164fa; /* mov.l @(0xc,r15),r1 */
L8c6164fa: sa_motion_publish_stop(ctx,0x8c6164fau); r[1]+=4294967294u;goto L8c6164fc; /* add #-0x2,r1 */
L8c6164fc: sa_motion_publish_stop(ctx,0x8c6164fcu); wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c6164fe; /* mov.l r1,@(0xc,r15) */
L8c6164fe: sa_motion_publish_stop(ctx,0x8c6164feu); r[10]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c616500; /* mov.w @r14+,r10 */
L8c616500: sa_motion_publish_stop(ctx,0x8c616500u); r[11]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c616502; /* mov.w @r14+,r11 */
L8c616502: sa_motion_publish_stop(ctx,0x8c616502u); r[10]=r[10]&65535u;goto L8c616504; /* extu.w r10,r10 */
L8c616504: sa_motion_publish_stop(ctx,0x8c616504u); r[10]<<=2;goto L8c616506; /* shll2 r10 */
L8c616506: sa_motion_publish_stop(ctx,0x8c616506u); r[11]=r[11]&65535u;goto L8c616508; /* extu.w r11,r11 */
L8c616508: sa_motion_publish_stop(ctx,0x8c616508u); r[11]<<=2;goto L8c61650a; /* shll2 r11 */
L8c61650a: sa_motion_publish_stop(ctx,0x8c61650au); r[10]<<=2;goto L8c61650c; /* shll2 r10 */
L8c61650c: sa_motion_publish_stop(ctx,0x8c61650cu); r[11]<<=2;goto L8c61650e; /* shll2 r11 */
L8c61650e: sa_motion_publish_stop(ctx,0x8c61650eu); *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c616510; /* shll r10 */
L8c616510: sa_motion_publish_stop(ctx,0x8c616510u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c616512; /* shll r11 */
L8c616512: sa_motion_publish_stop(ctx,0x8c616512u); r[11]+=r[13];goto L8c616514; /* add r13,r11 */
L8c616514: sa_motion_publish_stop(ctx,0x8c616514u); r[10]+=r[13];goto L8c616516; /* add r13,r10 */
L8c616516: sa_motion_publish_stop(ctx,0x8c616516u); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c616518; /* fmov.s @(r0,r11),fr13 */
L8c616518: sa_motion_publish_stop(ctx,0x8c616518u); fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c61651a; /* fmov.s @(r0,r10),fr6 */
L8c61651a: sa_motion_publish_stop(ctx,0x8c61651au); r[0]=20u;goto L8c61651c; /* mov #0x14,r0 */
L8c61651c: sa_motion_publish_stop(ctx,0x8c61651cu); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c61651e; /* fmov.s @(r0,r11),fr14 */
L8c61651e: sa_motion_publish_stop(ctx,0x8c61651eu); fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c616520; /* fmov.s @(r0,r10),fr5 */
L8c616520: sa_motion_publish_stop(ctx,0x8c616520u); r[12]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[14]+=2;goto L8c616522; /* mov.w @r14+,r12 */
L8c616522: sa_motion_publish_stop(ctx,0x8c616522u); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c616524; /* mov.w @r14,r3 */
L8c616524: sa_motion_publish_stop(ctx,0x8c616524u); r[12]=r[12]&65535u;goto L8c616526; /* extu.w r12,r12 */
L8c616526: sa_motion_publish_stop(ctx,0x8c616526u); r[12]<<=2;goto L8c616528; /* shll2 r12 */
L8c616528: sa_motion_publish_stop(ctx,0x8c616528u); r[3]=r[3]&65535u;goto L8c61652a; /* extu.w r3,r3 */
L8c61652a: sa_motion_publish_stop(ctx,0x8c61652au); r[3]<<=2;goto L8c61652c; /* shll2 r3 */
L8c61652c: sa_motion_publish_stop(ctx,0x8c61652cu); r[12]<<=2;goto L8c61652e; /* shll2 r12 */
L8c61652e: sa_motion_publish_stop(ctx,0x8c61652eu); r[3]<<=2;goto L8c616530; /* shll2 r3 */
L8c616530: sa_motion_publish_stop(ctx,0x8c616530u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c616532; /* shll r12 */
L8c616532: sa_motion_publish_stop(ctx,0x8c616532u); *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c616534; /* shll r3 */
L8c616534: sa_motion_publish_stop(ctx,0x8c616534u); r[3]+=r[13];goto L8c616536; /* add r13,r3 */
L8c616536: sa_motion_publish_stop(ctx,0x8c616536u); r[12]+=r[13];goto L8c616538; /* add r13,r12 */
L8c616538: sa_motion_publish_stop(ctx,0x8c616538u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c61653a; /* pref @r3 */
L8c61653a: sa_motion_publish_stop(ctx,0x8c61653au); r[2]=0u;goto L8c61653c; /* mov #0x0,r2 */
L8c61653c: sa_motion_publish_stop(ctx,0x8c61653cu); wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c61653e; /* mov.l r2,@(0x14,r15) */
L8c61653e: sa_motion_publish_stop(ctx,0x8c61653eu); r[4]=63u;goto L8c616540; /* mov #0x3f,r4 */
L8c616540: sa_motion_publish_stop(ctx,0x8c616540u); r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c616542; /* mov.l @(0xc,r10),r2 */
L8c616542: sa_motion_publish_stop(ctx,0x8c616542u); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c616544; /* mov.l @(0xc,r11),r3 */
L8c616544: sa_motion_publish_stop(ctx,0x8c616544u); r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c616546; /* mov.l @(0xc,r12),r1 */
L8c616546: sa_motion_publish_stop(ctx,0x8c616546u); r[2]&=r[3];goto L8c616548; /* and r3,r2 */
L8c616548: sa_motion_publish_stop(ctx,0x8c616548u); r[2]&=r[1];goto L8c61654a; /* and r1,r2 */
L8c61654a: sa_motion_publish_stop(ctx,0x8c61654au); r[4]&=r[2];goto L8c61654c; /* and r2,r4 */
L8c61654c: sa_motion_publish_stop(ctx,0x8c61654cu); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c61654e; /* tst r4,r4 */
L8c61654e: sa_motion_publish_stop(ctx,0x8c616550u); take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c616556;goto L8c616552; /* bt/s 0x8c616556 */
L8c616552: sa_motion_publish_stop(ctx,0x8c616554u); goto L8c6166b0; /* bra 0x8c6166b0 */
L8c616556: sa_motion_publish_stop(ctx,0x8c616556u); r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c616558; /* mov.l @(0xc,r10),r4 */
L8c616558: sa_motion_publish_stop(ctx,0x8c616558u); r[5]=1u;goto L8c61655a; /* mov #0x1,r5 */
L8c61655a: sa_motion_publish_stop(ctx,0x8c61655au); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61655c; /* mov.l @(0xc,r11),r3 */
L8c61655c: sa_motion_publish_stop(ctx,0x8c61655cu); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61655e; /* mov.l @(0xc,r12),r2 */
L8c61655e: sa_motion_publish_stop(ctx,0x8c61655eu); r[4]|=r[3];goto L8c616560; /* or r3,r4 */
L8c616560: sa_motion_publish_stop(ctx,0x8c616560u); r[4]|=r[2];goto L8c616562; /* or r2,r4 */
L8c616562: sa_motion_publish_stop(ctx,0x8c616562u); r[4]&=r[5];goto L8c616564; /* and r5,r4 */
L8c616564: sa_motion_publish_stop(ctx,0x8c616564u); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c616566; /* tst r4,r4 */
L8c616566: sa_motion_publish_stop(ctx,0x8c616566u); if(!(*sr&1u))goto L8c61656c;goto L8c616568; /* bf 0x8c61656c */
L8c616568: sa_motion_publish_stop(ctx,0x8c61656au); goto L8c616692; /* bra 0x8c616692 */
L8c61656c: sa_motion_publish_stop(ctx,0x8c61656cu); r[7]=r[15];goto L8c61656e; /* mov r15,r7 */
L8c61656e: sa_motion_publish_stop(ctx,0x8c61656eu); r[7]+=64u;goto L8c616570; /* add #0x40,r7 */
L8c616570: sa_motion_publish_stop(ctx,0x8c616570u); wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c616572; /* mov.l r10,@r7 */
L8c616572: sa_motion_publish_stop(ctx,0x8c616572u); wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c616574; /* mov.l r11,@(0x4,r7) */
L8c616574: sa_motion_publish_stop(ctx,0x8c616574u); wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c616576; /* mov.l r12,@(0x8,r7) */
L8c616576: sa_motion_publish_stop(ctx,0x8c616578u); r[6]=0u;goto L8c616624; /* bra 0x8c616624 */
L8c61657a: sa_motion_publish_stop(ctx,0x8c61657au); r[2]=r[6];goto L8c61657c; /* mov r6,r2 */
L8c61657c: sa_motion_publish_stop(ctx,0x8c61657cu); r[1]=r[6];goto L8c61657e; /* mov r6,r1 */
L8c61657e: sa_motion_publish_stop(ctx,0x8c61657eu); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c616580; /* shll r2 */
L8c616580: sa_motion_publish_stop(ctx,0x8c616580u); r[3]=r[6];goto L8c616582; /* mov r6,r3 */
L8c616582: sa_motion_publish_stop(ctx,0x8c616582u); r[3]<<=2;goto L8c616584; /* shll2 r3 */
L8c616584: sa_motion_publish_stop(ctx,0x8c616584u); r[2]+=r[1];goto L8c616586; /* add r1,r2 */
L8c616586: sa_motion_publish_stop(ctx,0x8c616586u); r[2]<<=2;goto L8c616588; /* shll2 r2 */
L8c616588: sa_motion_publish_stop(ctx,0x8c616588u); r[3]+=r[7];goto L8c61658a; /* add r7,r3 */
L8c61658a: sa_motion_publish_stop(ctx,0x8c61658au); r[2]+=r[4];goto L8c61658c; /* add r4,r2 */
L8c61658c: sa_motion_publish_stop(ctx,0x8c61658cu); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61658e; /* mov.l r3,@(0x4,r15) */
L8c61658e: sa_motion_publish_stop(ctx,0x8c61658eu); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c616590; /* mov.l r2,@r15 */
L8c616590: sa_motion_publish_stop(ctx,0x8c616590u); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c616592; /* mov.l @r3,r1 */
L8c616592: sa_motion_publish_stop(ctx,0x8c616592u); r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c616594; /* mov.l @(0xc,r1),r2 */
L8c616594: sa_motion_publish_stop(ctx,0x8c616594u); *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c616596; /* tst r5,r2 */
L8c616596: sa_motion_publish_stop(ctx,0x8c616598u); take=!(*sr&1u);r[6]+=1u;if(take)goto L8c6165fc;goto L8c61659a; /* bf/s 0x8c6165fc */
L8c61659a: sa_motion_publish_stop(ctx,0x8c61659au); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61659c; /* mov.l @(0x4,r15),r1 */
L8c61659c: sa_motion_publish_stop(ctx,0x8c61659cu); r[0]=24u;goto L8c61659e; /* mov #0x18,r0 */
L8c61659e: sa_motion_publish_stop(ctx,0x8c61659eu); fcopy(f,4u,12u);goto L8c6165a0; /* fmov fr12,fr4 */
L8c6165a0: sa_motion_publish_stop(ctx,0x8c6165a0u); r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c6165a2; /* mov.l @r1,r3 */
L8c6165a2: sa_motion_publish_stop(ctx,0x8c6165a2u); r[4]=rd(m,bus,&io_status,0x8c6165f0u,4);if(io_status)return io_status;goto L8c6165a4; /* mov.l 0x8c6165f0,r4 */
L8c6165a4: sa_motion_publish_stop(ctx,0x8c6165a4u); wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c6165a6; /* mov.l r3,@(0x10,r15) */
L8c6165a6: sa_motion_publish_stop(ctx,0x8c6165a6u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6165a8; /* fmov.s @(r0,r3),fr3 */
L8c6165a8: sa_motion_publish_stop(ctx,0x8c6165a8u); r[0]=12u;goto L8c6165aa; /* mov #0xc,r0 */
L8c6165aa: sa_motion_publish_stop(ctx,0x8c6165aau); r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c6165ac; /* mov.l @(0x10,r15),r2 */
L8c6165ac: sa_motion_publish_stop(ctx,0x8c6165acu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6165ae; /* fdiv fr3,fr4 */
L8c6165ae: sa_motion_publish_stop(ctx,0x8c6165aeu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6165b0; /* fmov.s @(r0,r4),fr3 */
L8c6165b0: sa_motion_publish_stop(ctx,0x8c6165b0u); r[0]=16u;goto L8c6165b2; /* mov #0x10,r0 */
L8c6165b2: sa_motion_publish_stop(ctx,0x8c6165b2u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6165b4; /* mov.l @r15,r3 */
L8c6165b4: sa_motion_publish_stop(ctx,0x8c6165b4u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6165b6; /* fmov.s @(r0,r2),fr2 */
L8c6165b6: sa_motion_publish_stop(ctx,0x8c6165b6u); r[0]=16u;goto L8c6165b8; /* mov #0x10,r0 */
L8c6165b8: sa_motion_publish_stop(ctx,0x8c6165b8u); r[2]=rd(m,bus,&io_status,0x8c6165f4u,4);if(io_status)return io_status;goto L8c6165ba; /* mov.l 0x8c6165f4,r2 */
L8c6165ba: sa_motion_publish_stop(ctx,0x8c6165bau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6165bc; /* fsub fr3,fr2 */
L8c6165bc: sa_motion_publish_stop(ctx,0x8c6165bcu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6165be; /* fmov.s @r2,fr3 */
L8c6165be: sa_motion_publish_stop(ctx,0x8c6165beu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6165c0; /* fmul fr4,fr2 */
L8c6165c0: sa_motion_publish_stop(ctx,0x8c6165c0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6165c2; /* fdiv fr3,fr2 */
L8c6165c2: sa_motion_publish_stop(ctx,0x8c6165c2u); fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6165c4; /* fmov.s fr2,@r3 */
L8c6165c4: sa_motion_publish_stop(ctx,0x8c6165c4u); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6165c6; /* mov.l @(0x4,r15),r1 */
L8c6165c6: sa_motion_publish_stop(ctx,0x8c6165c6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6165c8; /* fmov.s @(r0,r4),fr3 */
L8c6165c8: sa_motion_publish_stop(ctx,0x8c6165c8u); r[0]=20u;goto L8c6165ca; /* mov #0x14,r0 */
L8c6165ca: sa_motion_publish_stop(ctx,0x8c6165cau); r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c6165cc; /* mov.l @r1,r1 */
L8c6165cc: sa_motion_publish_stop(ctx,0x8c6165ccu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6165ce; /* mov.l @r15,r3 */
L8c6165ce: sa_motion_publish_stop(ctx,0x8c6165ceu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c6165d0; /* fmov.s @(r0,r1),fr2 */
L8c6165d0: sa_motion_publish_stop(ctx,0x8c6165d0u); r[1]=rd(m,bus,&io_status,0x8c6165f8u,4);if(io_status)return io_status;goto L8c6165d2; /* mov.l 0x8c6165f8,r1 */
L8c6165d2: sa_motion_publish_stop(ctx,0x8c6165d2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6165d4; /* fsub fr3,fr2 */
L8c6165d4: sa_motion_publish_stop(ctx,0x8c6165d4u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6165d6; /* fmov.s @r1,fr3 */
L8c6165d6: sa_motion_publish_stop(ctx,0x8c6165d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6165d8; /* fmul fr4,fr2 */
L8c6165d8: sa_motion_publish_stop(ctx,0x8c6165d8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6165da; /* fdiv fr3,fr2 */
L8c6165da: sa_motion_publish_stop(ctx,0x8c6165dau); r[0]=4u;goto L8c6165dc; /* mov #0x4,r0 */
L8c6165dc: sa_motion_publish_stop(ctx,0x8c6165dcu); fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c6165de; /* fmov fr2,@(r0,r3) */
L8c6165de: sa_motion_publish_stop(ctx,0x8c6165deu); r[0]=8u;goto L8c6165e0; /* mov #0x8,r0 */
L8c6165e0: sa_motion_publish_stop(ctx,0x8c6165e0u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6165e2; /* mov.l @r15,r3 */
L8c6165e2: sa_motion_publish_stop(ctx,0x8c6165e2u); fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c6165e4; /* fmov fr4,@(r0,r3) */
L8c6165e4: sa_motion_publish_stop(ctx,0x8c6165e6u); goto L8c616624; /* bra 0x8c616624 */
L8c6165fc: sa_motion_publish_stop(ctx,0x8c6165fcu); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6165fe; /* mov.l @(0x4,r15),r3 */
L8c6165fe: sa_motion_publish_stop(ctx,0x8c6165feu); r[0]=16u;goto L8c616600; /* mov #0x10,r0 */
L8c616600: sa_motion_publish_stop(ctx,0x8c616600u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616602; /* mov.l @r15,r2 */
L8c616602: sa_motion_publish_stop(ctx,0x8c616602u); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c616604; /* mov.l @r3,r1 */
L8c616604: sa_motion_publish_stop(ctx,0x8c616604u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c616606; /* fmov.s @(r0,r1),fr3 */
L8c616606: sa_motion_publish_stop(ctx,0x8c616606u); r[0]=20u;goto L8c616608; /* mov #0x14,r0 */
L8c616608: sa_motion_publish_stop(ctx,0x8c616608u); fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61660a; /* fmov.s fr3,@r2 */
L8c61660a: sa_motion_publish_stop(ctx,0x8c61660au); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61660c; /* mov.l @(0x4,r15),r2 */
L8c61660c: sa_motion_publish_stop(ctx,0x8c61660cu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61660e; /* mov.l @r15,r3 */
L8c61660e: sa_motion_publish_stop(ctx,0x8c61660eu); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c616610; /* mov.l @r2,r1 */
L8c616610: sa_motion_publish_stop(ctx,0x8c616610u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c616612; /* fmov.s @(r0,r1),fr3 */
L8c616612: sa_motion_publish_stop(ctx,0x8c616612u); r[0]=4u;goto L8c616614; /* mov #0x4,r0 */
L8c616614: sa_motion_publish_stop(ctx,0x8c616614u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616616; /* fmov fr3,@(r0,r3) */
L8c616616: sa_motion_publish_stop(ctx,0x8c616616u); r[0]=24u;goto L8c616618; /* mov #0x18,r0 */
L8c616618: sa_motion_publish_stop(ctx,0x8c616618u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61661a; /* mov.l @(0x4,r15),r2 */
L8c61661a: sa_motion_publish_stop(ctx,0x8c61661au); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61661c; /* mov.l @r15,r3 */
L8c61661c: sa_motion_publish_stop(ctx,0x8c61661cu); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61661e; /* mov.l @r2,r1 */
L8c61661e: sa_motion_publish_stop(ctx,0x8c61661eu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c616620; /* fmov.s @(r0,r1),fr3 */
L8c616620: sa_motion_publish_stop(ctx,0x8c616620u); r[0]=8u;goto L8c616622; /* mov #0x8,r0 */
L8c616622: sa_motion_publish_stop(ctx,0x8c616622u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616624; /* fmov fr3,@(r0,r3) */
L8c616624: sa_motion_publish_stop(ctx,0x8c616624u); r[3]=3u;goto L8c616626; /* mov #0x3,r3 */
L8c616626: sa_motion_publish_stop(ctx,0x8c616626u); r[4]=r[15];goto L8c616628; /* mov r15,r4 */
L8c616628: sa_motion_publish_stop(ctx,0x8c616628u); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c61662a; /* cmp/ge r3,r6 */
L8c61662a: sa_motion_publish_stop(ctx,0x8c61662cu); take=!(*sr&1u);r[4]+=28u;if(take)goto L8c61657a;goto L8c61662e; /* bf/s 0x8c61657a */
L8c61662e: sa_motion_publish_stop(ctx,0x8c61662eu); r[7]=r[4];goto L8c616630; /* mov r4,r7 */
L8c616630: sa_motion_publish_stop(ctx,0x8c616630u); r[0]=8u;goto L8c616632; /* mov #0x8,r0 */
L8c616632: sa_motion_publish_stop(ctx,0x8c616632u); r[3]=r[4];goto L8c616634; /* mov r4,r3 */
L8c616634: sa_motion_publish_stop(ctx,0x8c616634u); fload(m,bus,&io_status,f,6u,r[0]+r[7]);if(io_status)return io_status;goto L8c616636; /* fmov.s @(r0,r7),fr6 */
L8c616636: sa_motion_publish_stop(ctx,0x8c616636u); r[3]+=12u;goto L8c616638; /* add #0xc,r3 */
L8c616638: sa_motion_publish_stop(ctx,0x8c616638u); r[6]=r[4];goto L8c61663a; /* mov r4,r6 */
L8c61663a: sa_motion_publish_stop(ctx,0x8c61663au); r[0]=4u;goto L8c61663c; /* mov #0x4,r0 */
L8c61663c: sa_motion_publish_stop(ctx,0x8c61663cu); fcopy(f,0u,6u);goto L8c61663e; /* fmov fr6,fr0 */
L8c61663e: sa_motion_publish_stop(ctx,0x8c61663eu); r[6]+=24u;goto L8c616640; /* add #0x18,r6 */
L8c616640: sa_motion_publish_stop(ctx,0x8c616640u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c616642; /* mov.l r3,@r15 */
L8c616642: sa_motion_publish_stop(ctx,0x8c616642u); fload(m,bus,&io_status,f,5u,r[0]+r[7]);if(io_status)return io_status;goto L8c616644; /* fmov.s @(r0,r7),fr5 */
L8c616644: sa_motion_publish_stop(ctx,0x8c616644u); fload(m,bus,&io_status,f,7u,r[0]+r[3]);if(io_status)return io_status;goto L8c616646; /* fmov.s @(r0,r3),fr7 */
L8c616646: sa_motion_publish_stop(ctx,0x8c616646u); r[0]=8u;goto L8c616648; /* mov #0x8,r0 */
L8c616648: sa_motion_publish_stop(ctx,0x8c616648u); fload(m,bus,&io_status,f,8u,r[0]+r[3]);if(io_status)return io_status;goto L8c61664a; /* fmov.s @(r0,r3),fr8 */
L8c61664a: sa_motion_publish_stop(ctx,0x8c61664au); r[0]=4u;goto L8c61664c; /* mov #0x4,r0 */
L8c61664c: sa_motion_publish_stop(ctx,0x8c61664cu); fcopy(f,3u,5u);goto L8c61664e; /* fmov fr5,fr3 */
L8c61664e: sa_motion_publish_stop(ctx,0x8c61664eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[7],&f->fpscr,&fr[0]);if(status)return status;}goto L8c616650; /* fmul fr7,fr0 */
L8c616650: sa_motion_publish_stop(ctx,0x8c616650u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[8],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616652; /* fmul fr8,fr3 */
L8c616652: sa_motion_publish_stop(ctx,0x8c616652u); fload(m,bus,&io_status,f,4u,r[7]);if(io_status)return io_status;goto L8c616654; /* fmov.s @r7,fr4 */
L8c616654: sa_motion_publish_stop(ctx,0x8c616654u); fload(m,bus,&io_status,f,9u,r[3]);if(io_status)return io_status;goto L8c616656; /* fmov.s @r3,fr9 */
L8c616656: sa_motion_publish_stop(ctx,0x8c616656u); fcopy(f,2u,4u);goto L8c616658; /* fmov fr4,fr2 */
L8c616658: sa_motion_publish_stop(ctx,0x8c616658u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61665a; /* fmul fr8,fr2 */
L8c61665a: sa_motion_publish_stop(ctx,0x8c61665au); fcopy(f,1u,5u);goto L8c61665c; /* fmov fr5,fr1 */
L8c61665c: sa_motion_publish_stop(ctx,0x8c61665cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c61665e; /* fsub fr3,fr0 */
L8c61665e: sa_motion_publish_stop(ctx,0x8c61665eu); fcopy(f,3u,6u);goto L8c616660; /* fmov fr6,fr3 */
L8c616660: sa_motion_publish_stop(ctx,0x8c616660u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616662; /* fmul fr9,fr3 */
L8c616662: sa_motion_publish_stop(ctx,0x8c616662u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616664; /* fsub fr3,fr2 */
L8c616664: sa_motion_publish_stop(ctx,0x8c616664u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c616666; /* fmov.s @(r0,r6),fr3 */
L8c616666: sa_motion_publish_stop(ctx,0x8c616666u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616668; /* fmul fr2,fr3 */
L8c616668: sa_motion_publish_stop(ctx,0x8c616668u); fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c61666a; /* fmov.s @r6,fr2 */
L8c61666a: sa_motion_publish_stop(ctx,0x8c61666au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61666c; /* fmac fr0,fr2,fr3 */
L8c61666c: sa_motion_publish_stop(ctx,0x8c61666cu); fcopy(f,2u,3u);goto L8c61666e; /* fmov fr3,fr2 */
L8c61666e: sa_motion_publish_stop(ctx,0x8c61666eu); fcopy(f,3u,4u);goto L8c616670; /* fmov fr4,fr3 */
L8c616670: sa_motion_publish_stop(ctx,0x8c616670u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616672; /* fmul fr7,fr3 */
L8c616672: sa_motion_publish_stop(ctx,0x8c616672u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[9],&f->fpscr,&fr[1]);if(status)return status;}goto L8c616674; /* fmul fr9,fr1 */
L8c616674: sa_motion_publish_stop(ctx,0x8c616674u); r[0]=8u;goto L8c616676; /* mov #0x8,r0 */
L8c616676: sa_motion_publish_stop(ctx,0x8c616676u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c616678; /* fsub fr3,fr1 */
L8c616678: sa_motion_publish_stop(ctx,0x8c616678u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61667a; /* fmov.s @(r0,r6),fr3 */
L8c61667a: sa_motion_publish_stop(ctx,0x8c61667au); fcopy(f,0u,1u);goto L8c61667c; /* fmov fr1,fr0 */
L8c61667c: sa_motion_publish_stop(ctx,0x8c61667cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61667e; /* fmac fr0,fr3,fr2 */
L8c61667e: sa_motion_publish_stop(ctx,0x8c61667eu); fcopy(f,3u,15u);goto L8c616680; /* fmov fr15,fr3 */
L8c616680: sa_motion_publish_stop(ctx,0x8c616680u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c616682; /* fneg fr3 */
L8c616682: sa_motion_publish_stop(ctx,0x8c616682u); fcopy(f,4u,2u);goto L8c616684; /* fmov fr2,fr4 */
L8c616684: sa_motion_publish_stop(ctx,0x8c616684u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616686; /* fmul fr3,fr4 */
L8c616686: sa_motion_publish_stop(ctx,0x8c616686u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c616688; /* fldi0 fr2 */
L8c616688: sa_motion_publish_stop(ctx,0x8c616688u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c61668a; /* fcmp/gt fr2,fr4 */
L8c61668a: sa_motion_publish_stop(ctx,0x8c61668au); if((*sr&1u))goto L8c6166b0;goto L8c61668c; /* bt 0x8c6166b0 */
L8c61668c: sa_motion_publish_stop(ctx,0x8c61668cu); wr(m,bus,&io_status,r[15]+0x14u,r[5],4);if(io_status)return io_status;goto L8c61668e; /* mov.l r5,@(0x14,r15) */
L8c61668e: sa_motion_publish_stop(ctx,0x8c616690u); fcopy(f,4u,12u);goto L8c6166b0; /* bra 0x8c6166b0 */
L8c616692: sa_motion_publish_stop(ctx,0x8c616692u); r[0]=20u;goto L8c616694; /* mov #0x14,r0 */
L8c616694: sa_motion_publish_stop(ctx,0x8c616694u); fcopy(f,3u,13u);goto L8c616696; /* fmov fr13,fr3 */
L8c616696: sa_motion_publish_stop(ctx,0x8c616696u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c616698; /* fmov.s @(r0,r12),fr2 */
L8c616698: sa_motion_publish_stop(ctx,0x8c616698u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61669a; /* fsub fr6,fr3 */
L8c61669a: sa_motion_publish_stop(ctx,0x8c61669au); r[0]=16u;goto L8c61669c; /* mov #0x10,r0 */
L8c61669c: sa_motion_publish_stop(ctx,0x8c61669cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61669e; /* fsub fr14,fr2 */
L8c61669e: sa_motion_publish_stop(ctx,0x8c61669eu); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c6166a0; /* fmov.s @(r0,r12),fr1 */
L8c6166a0: sa_motion_publish_stop(ctx,0x8c6166a0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6166a2; /* fsub fr13,fr1 */
L8c6166a2: sa_motion_publish_stop(ctx,0x8c6166a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6166a4; /* fmul fr2,fr3 */
L8c6166a4: sa_motion_publish_stop(ctx,0x8c6166a4u); fcopy(f,2u,14u);goto L8c6166a6; /* fmov fr14,fr2 */
L8c6166a6: sa_motion_publish_stop(ctx,0x8c6166a6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6166a8; /* fsub fr5,fr2 */
L8c6166a8: sa_motion_publish_stop(ctx,0x8c6166a8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6166aa; /* fmul fr1,fr2 */
L8c6166aa: sa_motion_publish_stop(ctx,0x8c6166aau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6166ac; /* fsub fr2,fr3 */
L8c6166ac: sa_motion_publish_stop(ctx,0x8c6166acu); fcopy(f,4u,3u);goto L8c6166ae; /* fmov fr3,fr4 */
L8c6166ae: sa_motion_publish_stop(ctx,0x8c6166aeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6166b0; /* fmul fr15,fr4 */
L8c6166b0: sa_motion_publish_stop(ctx,0x8c6166b0u); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c6166b2; /* cmp/hs r8,r9 */
L8c6166b2: sa_motion_publish_stop(ctx,0x8c6166b2u); if((*sr&1u))goto L8c6166e8;goto L8c6166b4; /* bt 0x8c6166e8 */
L8c6166b4: sa_motion_publish_stop(ctx,0x8c6166b4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6166b6; /* fldi0 fr3 */
L8c6166b6: sa_motion_publish_stop(ctx,0x8c6166b6u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6166b8; /* fcmp/gt fr4,fr3 */
L8c6166b8: sa_motion_publish_stop(ctx,0x8c6166b8u); if((*sr&1u))goto L8c616718;goto L8c6166ba; /* bt 0x8c616718 */
L8c6166ba: sa_motion_publish_stop(ctx,0x8c6166bau); r[5]=rd(m,bus,&io_status,0x8c61687cu,4);if(io_status)return io_status;goto L8c6166bc; /* mov.l 0x8c61687c,r5 */
L8c6166bc: sa_motion_publish_stop(ctx,0x8c6166bcu); r[4]=r[11];goto L8c6166be; /* mov r11,r4 */
L8c6166be: sa_motion_publish_stop(ctx,0x8c6166beu); r[8]+=16u;goto L8c6166c0; /* add #0x10,r8 */
L8c6166c0: sa_motion_publish_stop(ctx,0x8c6166c0u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6166c2; /* fschg */
L8c6166c2: sa_motion_publish_stop(ctx,0x8c6166c2u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6166c4; /* fmov.s @r4+,fr0 */
L8c6166c4: sa_motion_publish_stop(ctx,0x8c6166c4u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6166c6; /* fmov.s @r4+,fr2 */
L8c6166c6: sa_motion_publish_stop(ctx,0x8c6166c6u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6166c8; /* fldi1 fr3 */
L8c6166c8: sa_motion_publish_stop(ctx,0x8c6166c8u); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6166ca; /* ftrv xmtrx,fv0 */
L8c6166ca: sa_motion_publish_stop(ctx,0x8c6166cau); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6166cc; /* fmov.s @r4+,fr4 */
L8c6166cc: sa_motion_publish_stop(ctx,0x8c6166ccu); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6166ce; /* fmov.s @r4,fr6 */
L8c6166ce: sa_motion_publish_stop(ctx,0x8c6166ceu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6166d0; /* fschg */
L8c6166d0: sa_motion_publish_stop(ctx,0x8c6166d0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6166d2; /* fmov.s fr6,@-r8 */
L8c6166d2: sa_motion_publish_stop(ctx,0x8c6166d2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6166d4; /* fmov.s fr5,@-r8 */
L8c6166d4: sa_motion_publish_stop(ctx,0x8c6166d4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6166d6; /* fmov.s fr4,@-r8 */
L8c6166d6: sa_motion_publish_stop(ctx,0x8c6166d6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6166d8; /* mov.l r5,@-r8 */
L8c6166d8: sa_motion_publish_stop(ctx,0x8c6166d8u); r[8]+=28u;goto L8c6166da; /* add #0x1c,r8 */
L8c6166da: sa_motion_publish_stop(ctx,0x8c6166dau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6166dc; /* fmov.s fr2,@-r8 */
L8c6166dc: sa_motion_publish_stop(ctx,0x8c6166dcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6166de; /* fmov.s fr1,@-r8 */
L8c6166de: sa_motion_publish_stop(ctx,0x8c6166deu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c6166e0; /* fmov.s fr0,@-r8 */
L8c6166e0: sa_motion_publish_stop(ctx,0x8c6166e0u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6166e2; /* pref @r8 */
L8c6166e2: sa_motion_publish_stop(ctx,0x8c6166e2u); r[8]+=16u;goto L8c6166e4; /* add #0x10,r8 */
L8c6166e4: sa_motion_publish_stop(ctx,0x8c6166e6u); r[9]=r[8];goto L8c616742; /* bra 0x8c616742 */
L8c6166e8: sa_motion_publish_stop(ctx,0x8c6166e8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6166ea; /* fldi0 fr3 */
L8c6166ea: sa_motion_publish_stop(ctx,0x8c6166eau); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6166ec; /* fcmp/gt fr4,fr3 */
L8c6166ec: sa_motion_publish_stop(ctx,0x8c6166ecu); if(!(*sr&1u))goto L8c616742;goto L8c6166ee; /* bf 0x8c616742 */
L8c6166ee: sa_motion_publish_stop(ctx,0x8c6166eeu); r[5]=rd(m,bus,&io_status,0x8c616880u,4);if(io_status)return io_status;goto L8c6166f0; /* mov.l 0x8c616880,r5 */
L8c6166f0: sa_motion_publish_stop(ctx,0x8c6166f0u); r[4]=r[10];goto L8c6166f2; /* mov r10,r4 */
L8c6166f2: sa_motion_publish_stop(ctx,0x8c6166f2u); r[8]+=16u;goto L8c6166f4; /* add #0x10,r8 */
L8c6166f4: sa_motion_publish_stop(ctx,0x8c6166f4u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6166f6; /* fschg */
L8c6166f6: sa_motion_publish_stop(ctx,0x8c6166f6u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6166f8; /* fmov.s @r4+,fr0 */
L8c6166f8: sa_motion_publish_stop(ctx,0x8c6166f8u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6166fa; /* fmov.s @r4+,fr2 */
L8c6166fa: sa_motion_publish_stop(ctx,0x8c6166fau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6166fc; /* fldi1 fr3 */
L8c6166fc: sa_motion_publish_stop(ctx,0x8c6166fcu); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6166fe; /* ftrv xmtrx,fv0 */
L8c6166fe: sa_motion_publish_stop(ctx,0x8c6166feu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616700; /* fmov.s @r4+,fr4 */
L8c616700: sa_motion_publish_stop(ctx,0x8c616700u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616702; /* fmov.s @r4,fr6 */
L8c616702: sa_motion_publish_stop(ctx,0x8c616702u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616704; /* fschg */
L8c616704: sa_motion_publish_stop(ctx,0x8c616704u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616706; /* fmov.s fr6,@-r8 */
L8c616706: sa_motion_publish_stop(ctx,0x8c616706u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616708; /* fmov.s fr5,@-r8 */
L8c616708: sa_motion_publish_stop(ctx,0x8c616708u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61670a; /* fmov.s fr4,@-r8 */
L8c61670a: sa_motion_publish_stop(ctx,0x8c61670au); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61670c; /* mov.l r5,@-r8 */
L8c61670c: sa_motion_publish_stop(ctx,0x8c61670cu); r[8]+=28u;goto L8c61670e; /* add #0x1c,r8 */
L8c61670e: sa_motion_publish_stop(ctx,0x8c61670eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616710; /* fmov.s fr2,@-r8 */
L8c616710: sa_motion_publish_stop(ctx,0x8c616710u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616712; /* fmov.s fr1,@-r8 */
L8c616712: sa_motion_publish_stop(ctx,0x8c616712u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616714; /* fmov.s fr0,@-r8 */
L8c616714: sa_motion_publish_stop(ctx,0x8c616714u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616716; /* pref @r8 */
L8c616716: sa_motion_publish_stop(ctx,0x8c616716u); r[8]+=16u;goto L8c616718; /* add #0x10,r8 */
L8c616718: sa_motion_publish_stop(ctx,0x8c616718u); r[5]=rd(m,bus,&io_status,0x8c616880u,4);if(io_status)return io_status;goto L8c61671a; /* mov.l 0x8c616880,r5 */
L8c61671a: sa_motion_publish_stop(ctx,0x8c61671au); r[4]=r[11];goto L8c61671c; /* mov r11,r4 */
L8c61671c: sa_motion_publish_stop(ctx,0x8c61671cu); r[8]+=16u;goto L8c61671e; /* add #0x10,r8 */
L8c61671e: sa_motion_publish_stop(ctx,0x8c61671eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616720; /* fschg */
L8c616720: sa_motion_publish_stop(ctx,0x8c616720u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616722; /* fmov.s @r4+,fr0 */
L8c616722: sa_motion_publish_stop(ctx,0x8c616722u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616724; /* fmov.s @r4+,fr2 */
L8c616724: sa_motion_publish_stop(ctx,0x8c616724u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c616726; /* fldi1 fr3 */
L8c616726: sa_motion_publish_stop(ctx,0x8c616726u); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c616728; /* ftrv xmtrx,fv0 */
L8c616728: sa_motion_publish_stop(ctx,0x8c616728u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61672a; /* fmov.s @r4+,fr4 */
L8c61672a: sa_motion_publish_stop(ctx,0x8c61672au); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61672c; /* fmov.s @r4,fr6 */
L8c61672c: sa_motion_publish_stop(ctx,0x8c61672cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61672e; /* fschg */
L8c61672e: sa_motion_publish_stop(ctx,0x8c61672eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616730; /* fmov.s fr6,@-r8 */
L8c616730: sa_motion_publish_stop(ctx,0x8c616730u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616732; /* fmov.s fr5,@-r8 */
L8c616732: sa_motion_publish_stop(ctx,0x8c616732u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616734; /* fmov.s fr4,@-r8 */
L8c616734: sa_motion_publish_stop(ctx,0x8c616734u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616736; /* mov.l r5,@-r8 */
L8c616736: sa_motion_publish_stop(ctx,0x8c616736u); r[8]+=28u;goto L8c616738; /* add #0x1c,r8 */
L8c616738: sa_motion_publish_stop(ctx,0x8c616738u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61673a; /* fmov.s fr2,@-r8 */
L8c61673a: sa_motion_publish_stop(ctx,0x8c61673au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61673c; /* fmov.s fr1,@-r8 */
L8c61673c: sa_motion_publish_stop(ctx,0x8c61673cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c61673e; /* fmov.s fr0,@-r8 */
L8c61673e: sa_motion_publish_stop(ctx,0x8c61673eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616740; /* pref @r8 */
L8c616740: sa_motion_publish_stop(ctx,0x8c616740u); r[8]+=16u;goto L8c616742; /* add #0x10,r8 */
L8c616742: sa_motion_publish_stop(ctx,0x8c616742u); r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c616744; /* mov.l @(0x14,r15),r2 */
L8c616744: sa_motion_publish_stop(ctx,0x8c616744u); *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c616746; /* tst r2,r2 */
L8c616746: sa_motion_publish_stop(ctx,0x8c616746u); if((*sr&1u))goto L8c6167ba;goto L8c616748; /* bt 0x8c6167ba */
L8c616748: sa_motion_publish_stop(ctx,0x8c616748u); r[0]=64u;goto L8c61674a; /* mov #0x40,r0 */
L8c61674a: sa_motion_publish_stop(ctx,0x8c61674au); r[4]=rd(m,bus,&io_status,0x8c616884u,4);if(io_status)return io_status;goto L8c61674c; /* mov.l 0x8c616884,r4 */
L8c61674c: sa_motion_publish_stop(ctx,0x8c61674cu); r[3]=0u;goto L8c61674e; /* mov #0x0,r3 */
L8c61674e: sa_motion_publish_stop(ctx,0x8c61674eu); r[9]=rd(m,bus,&io_status,0x8c616888u,4);if(io_status)return io_status;goto L8c616750; /* mov.l 0x8c616888,r9 */
L8c616750: sa_motion_publish_stop(ctx,0x8c616750u); wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c616752; /* mov.l r3,@r4 */
L8c616752: sa_motion_publish_stop(ctx,0x8c616752u); r[4]=r[15];goto L8c616754; /* mov r15,r4 */
L8c616754: sa_motion_publish_stop(ctx,0x8c616754u); fload(m,bus,&io_status,f,3u,r[11]);if(io_status)return io_status;goto L8c616756; /* fmov.s @r11,fr3 */
L8c616756: sa_motion_publish_stop(ctx,0x8c616756u); fload(m,bus,&io_status,f,2u,r[10]);if(io_status)return io_status;goto L8c616758; /* fmov.s @r10,fr2 */
L8c616758: sa_motion_publish_stop(ctx,0x8c616758u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61675a; /* fadd fr3,fr2 */
L8c61675a: sa_motion_publish_stop(ctx,0x8c61675au); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61675c; /* fmov.s @r12,fr3 */
L8c61675c: sa_motion_publish_stop(ctx,0x8c61675cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61675e; /* fadd fr3,fr2 */
L8c61675e: sa_motion_publish_stop(ctx,0x8c61675eu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c616760; /* fmov fr2,@(r0,r15) */
L8c616760: sa_motion_publish_stop(ctx,0x8c616760u); r[0]=4u;goto L8c616762; /* mov #0x4,r0 */
L8c616762: sa_motion_publish_stop(ctx,0x8c616762u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c616764; /* fmov.s @(r0,r11),fr3 */
L8c616764: sa_motion_publish_stop(ctx,0x8c616764u); fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c616766; /* fmov.s @(r0,r10),fr2 */
L8c616766: sa_motion_publish_stop(ctx,0x8c616766u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c616768; /* fadd fr3,fr2 */
L8c616768: sa_motion_publish_stop(ctx,0x8c616768u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61676a; /* fmov.s @(r0,r12),fr3 */
L8c61676a: sa_motion_publish_stop(ctx,0x8c61676au); r[0]=68u;goto L8c61676c; /* mov #0x44,r0 */
L8c61676c: sa_motion_publish_stop(ctx,0x8c61676cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61676e; /* fadd fr3,fr2 */
L8c61676e: sa_motion_publish_stop(ctx,0x8c61676eu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c616770; /* fmov fr2,@(r0,r15) */
L8c616770: sa_motion_publish_stop(ctx,0x8c616770u); r[0]=8u;goto L8c616772; /* mov #0x8,r0 */
L8c616772: sa_motion_publish_stop(ctx,0x8c616772u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c616774; /* fmov.s @(r0,r11),fr3 */
L8c616774: sa_motion_publish_stop(ctx,0x8c616774u); fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c616776; /* fmov.s @(r0,r10),fr2 */
L8c616776: sa_motion_publish_stop(ctx,0x8c616776u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c616778; /* fadd fr3,fr2 */
L8c616778: sa_motion_publish_stop(ctx,0x8c616778u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61677a; /* fmov.s @(r0,r12),fr3 */
L8c61677a: sa_motion_publish_stop(ctx,0x8c61677au); r[0]=72u;goto L8c61677c; /* mov #0x48,r0 */
L8c61677c: sa_motion_publish_stop(ctx,0x8c61677cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61677e; /* fadd fr3,fr2 */
L8c61677e: sa_motion_publish_stop(ctx,0x8c61677eu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c616780; /* fmov fr2,@(r0,r15) */
L8c616780: sa_motion_publish_stop(ctx,0x8c616782u); target=r[9];*pr=0x8c616784u;r[4]+=64u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c616784; /* jsr @r9 */
L8c616784: sa_motion_publish_stop(ctx,0x8c616784u); r[4]=rd(m,bus,&io_status,0x8c616890u,4);if(io_status)return io_status;goto L8c616786; /* mov.l 0x8c616890,r4 */
L8c616786: sa_motion_publish_stop(ctx,0x8c616786u); r[0]=64u;goto L8c616788; /* mov #0x40,r0 */
L8c616788: sa_motion_publish_stop(ctx,0x8c616788u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61678a; /* fmov.s @(r0,r15),fr3 */
L8c61678a: sa_motion_publish_stop(ctx,0x8c61678au); r[0]=68u;goto L8c61678c; /* mov #0x44,r0 */
L8c61678c: sa_motion_publish_stop(ctx,0x8c61678cu); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c61678e; /* fmov.s @r4,fr2 */
L8c61678e: sa_motion_publish_stop(ctx,0x8c61678eu); r[6]=r[12];goto L8c616790; /* mov r12,r6 */
L8c616790: sa_motion_publish_stop(ctx,0x8c616790u); r[5]=rd(m,bus,&io_status,0x8c61688cu,4);if(io_status)return io_status;goto L8c616792; /* mov.l 0x8c61688c,r5 */
L8c616792: sa_motion_publish_stop(ctx,0x8c616792u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616794; /* fmul fr3,fr2 */
L8c616794: sa_motion_publish_stop(ctx,0x8c616794u); r[3]=rd(m,bus,&io_status,0x8c616894u,4);if(io_status)return io_status;goto L8c616796; /* mov.l 0x8c616894,r3 */
L8c616796: sa_motion_publish_stop(ctx,0x8c616796u); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c616798; /* fmov.s @r5,fr3 */
L8c616798: sa_motion_publish_stop(ctx,0x8c616798u); r[5]=r[11];goto L8c61679a; /* mov r11,r5 */
L8c61679a: sa_motion_publish_stop(ctx,0x8c61679au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61679c; /* fsub fr2,fr3 */
L8c61679c: sa_motion_publish_stop(ctx,0x8c61679cu); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61679e; /* fmov.s @(r0,r15),fr2 */
L8c61679e: sa_motion_publish_stop(ctx,0x8c61679eu); r[0]=4u;goto L8c6167a0; /* mov #0x4,r0 */
L8c6167a0: sa_motion_publish_stop(ctx,0x8c6167a0u); fload(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c6167a2; /* fmov.s @(r0,r4),fr1 */
L8c6167a2: sa_motion_publish_stop(ctx,0x8c6167a2u); r[0]=72u;goto L8c6167a4; /* mov #0x48,r0 */
L8c6167a4: sa_motion_publish_stop(ctx,0x8c6167a4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6167a6; /* fmul fr2,fr1 */
L8c6167a6: sa_motion_publish_stop(ctx,0x8c6167a6u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c6167a8; /* fmov.s @(r0,r15),fr2 */
L8c6167a8: sa_motion_publish_stop(ctx,0x8c6167a8u); r[0]=8u;goto L8c6167aa; /* mov #0x8,r0 */
L8c6167aa: sa_motion_publish_stop(ctx,0x8c6167aau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6167ac; /* fsub fr1,fr3 */
L8c6167ac: sa_motion_publish_stop(ctx,0x8c6167acu); fload(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c6167ae; /* fmov.s @(r0,r4),fr1 */
L8c6167ae: sa_motion_publish_stop(ctx,0x8c6167aeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6167b0; /* fmul fr2,fr1 */
L8c6167b0: sa_motion_publish_stop(ctx,0x8c6167b0u); fcopy(f,4u,3u);goto L8c6167b2; /* fmov fr3,fr4 */
L8c6167b2: sa_motion_publish_stop(ctx,0x8c6167b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c6167b4; /* fsub fr1,fr4 */
L8c6167b4: sa_motion_publish_stop(ctx,0x8c6167b6u); target=r[3];*pr=0x8c6167b8u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6167b8; /* jsr @r3 */
L8c6167b8: sa_motion_publish_stop(ctx,0x8c6167b8u); r[9]=r[8];goto L8c6167ba; /* mov r8,r9 */
L8c6167ba: sa_motion_publish_stop(ctx,0x8c6167bau); fcopy(f,6u,13u);goto L8c6167bc; /* fmov fr13,fr6 */
L8c6167bc: sa_motion_publish_stop(ctx,0x8c6167bcu); r[0]=16u;goto L8c6167be; /* mov #0x10,r0 */
L8c6167be: sa_motion_publish_stop(ctx,0x8c6167beu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6167c0; /* mov.l @(0xc,r15),r3 */
L8c6167c0: sa_motion_publish_stop(ctx,0x8c6167c0u); r[10]=r[11];goto L8c6167c2; /* mov r11,r10 */
L8c6167c2: sa_motion_publish_stop(ctx,0x8c6167c2u); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c6167c4; /* fmov.s @(r0,r12),fr13 */
L8c6167c4: sa_motion_publish_stop(ctx,0x8c6167c4u); r[0]=20u;goto L8c6167c6; /* mov #0x14,r0 */
L8c6167c6: sa_motion_publish_stop(ctx,0x8c6167c6u); fcopy(f,5u,14u);goto L8c6167c8; /* fmov fr14,fr5 */
L8c6167c8: sa_motion_publish_stop(ctx,0x8c6167c8u); r[3]+=4294967295u;goto L8c6167ca; /* add #-0x1,r3 */
L8c6167ca: sa_motion_publish_stop(ctx,0x8c6167cau); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c6167cc; /* fmov.s @(r0,r12),fr14 */
L8c6167cc: sa_motion_publish_stop(ctx,0x8c6167ccu); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6167ce; /* tst r3,r3 */
L8c6167ce: sa_motion_publish_stop(ctx,0x8c6167ceu); r[11]=r[12];goto L8c6167d0; /* mov r12,r11 */
L8c6167d0: sa_motion_publish_stop(ctx,0x8c6167d0u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c6167d2; /* fneg fr15 */
L8c6167d2: sa_motion_publish_stop(ctx,0x8c6167d4u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;if(take)goto L8c6167da;goto L8c6167d6; /* bt/s 0x8c6167da */
L8c6167d6: sa_motion_publish_stop(ctx,0x8c6167d8u); goto L8c616520; /* bra 0x8c616520 */
L8c6167da: sa_motion_publish_stop(ctx,0x8c6167dau); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c6167dc; /* cmp/hs r8,r9 */
L8c6167dc: sa_motion_publish_stop(ctx,0x8c6167dcu); if((*sr&1u))goto L8c616808;goto L8c6167de; /* bt 0x8c616808 */
L8c6167de: sa_motion_publish_stop(ctx,0x8c6167deu); r[5]=rd(m,bus,&io_status,0x8c61687cu,4);if(io_status)return io_status;goto L8c6167e0; /* mov.l 0x8c61687c,r5 */
L8c6167e0: sa_motion_publish_stop(ctx,0x8c6167e0u); r[4]=r[12];goto L8c6167e2; /* mov r12,r4 */
L8c6167e2: sa_motion_publish_stop(ctx,0x8c6167e2u); r[8]+=16u;goto L8c6167e4; /* add #0x10,r8 */
L8c6167e4: sa_motion_publish_stop(ctx,0x8c6167e4u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6167e6; /* fschg */
L8c6167e6: sa_motion_publish_stop(ctx,0x8c6167e6u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6167e8; /* fmov.s @r4+,fr0 */
L8c6167e8: sa_motion_publish_stop(ctx,0x8c6167e8u); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6167ea; /* fmov.s @r4+,fr2 */
L8c6167ea: sa_motion_publish_stop(ctx,0x8c6167eau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6167ec; /* fldi1 fr3 */
L8c6167ec: sa_motion_publish_stop(ctx,0x8c6167ecu); status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6167ee; /* ftrv xmtrx,fv0 */
L8c6167ee: sa_motion_publish_stop(ctx,0x8c6167eeu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6167f0; /* fmov.s @r4+,fr4 */
L8c6167f0: sa_motion_publish_stop(ctx,0x8c6167f0u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6167f2; /* fmov.s @r4,fr6 */
L8c6167f2: sa_motion_publish_stop(ctx,0x8c6167f2u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6167f4; /* fschg */
L8c6167f4: sa_motion_publish_stop(ctx,0x8c6167f4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6167f6; /* fmov.s fr6,@-r8 */
L8c6167f6: sa_motion_publish_stop(ctx,0x8c6167f6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6167f8; /* fmov.s fr5,@-r8 */
L8c6167f8: sa_motion_publish_stop(ctx,0x8c6167f8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6167fa; /* fmov.s fr4,@-r8 */
L8c6167fa: sa_motion_publish_stop(ctx,0x8c6167fau); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6167fc; /* mov.l r5,@-r8 */
L8c6167fc: sa_motion_publish_stop(ctx,0x8c6167fcu); r[8]+=28u;goto L8c6167fe; /* add #0x1c,r8 */
L8c6167fe: sa_motion_publish_stop(ctx,0x8c6167feu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616800; /* fmov.s fr2,@-r8 */
L8c616800: sa_motion_publish_stop(ctx,0x8c616800u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616802; /* fmov.s fr1,@-r8 */
L8c616802: sa_motion_publish_stop(ctx,0x8c616802u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c616804; /* fmov.s fr0,@-r8 */
L8c616804: sa_motion_publish_stop(ctx,0x8c616804u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616806; /* pref @r8 */
L8c616806: sa_motion_publish_stop(ctx,0x8c616806u); r[8]+=16u;goto L8c616808; /* add #0x10,r8 */
L8c616808: sa_motion_publish_stop(ctx,0x8c616808u); r[3]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c61680a; /* mov.l @(0x18,r15),r3 */
L8c61680a: sa_motion_publish_stop(ctx,0x8c61680au); r[3]+=4294967295u;goto L8c61680c; /* add #-0x1,r3 */
L8c61680c: sa_motion_publish_stop(ctx,0x8c61680cu); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c61680e; /* tst r3,r3 */
L8c61680e: sa_motion_publish_stop(ctx,0x8c616810u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;if(take)goto L8c616816;goto L8c616812; /* bt/s 0x8c616816 */
L8c616812: sa_motion_publish_stop(ctx,0x8c616814u); goto L8c6164d0; /* bra 0x8c6164d0 */
L8c616816: sa_motion_publish_stop(ctx,0x8c616816u); r[4]=rd(m,bus,&io_status,0x8c616898u,4);if(io_status)return io_status;goto L8c616818; /* mov.l 0x8c616898,r4 */
L8c616818: sa_motion_publish_stop(ctx,0x8c616818u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61681a; /* mov.l @(0x8,r15),r3 */
L8c61681a: sa_motion_publish_stop(ctx,0x8c61681au); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61681c; /* mov.l @r4,r2 */
L8c61681c: sa_motion_publish_stop(ctx,0x8c61681cu); r[2]+=r[3];goto L8c61681e; /* add r3,r2 */
L8c61681e: sa_motion_publish_stop(ctx,0x8c61681eu); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c616820; /* mov.l r2,@r4 */
L8c616820: sa_motion_publish_stop(ctx,0x8c616820u); r[4]=rd(m,bus,&io_status,0x8c61689cu,4);if(io_status)return io_status;goto L8c616822; /* mov.l 0x8c61689c,r4 */
L8c616822: sa_motion_publish_stop(ctx,0x8c616822u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c616824; /* mov.l @(0x8,r15),r3 */
L8c616824: sa_motion_publish_stop(ctx,0x8c616824u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616826; /* mov.l @r4,r2 */
L8c616826: sa_motion_publish_stop(ctx,0x8c616826u); r[2]+=r[3];goto L8c616828; /* add r3,r2 */
L8c616828: sa_motion_publish_stop(ctx,0x8c616828u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c61682a; /* mov.l r2,@r4 */
L8c61682a: sa_motion_publish_stop(ctx,0x8c61682au); r[15]+=76u;goto L8c61682c; /* add #0x4c,r15 */
L8c61682c: sa_motion_publish_stop(ctx,0x8c61682cu); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61682e; /* lds.l @r15+,PR */
L8c61682e: sa_motion_publish_stop(ctx,0x8c61682eu); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616830; /* fmov.s @r15+,fr12 */
L8c616830: sa_motion_publish_stop(ctx,0x8c616830u); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616832; /* fmov.s @r15+,fr13 */
L8c616832: sa_motion_publish_stop(ctx,0x8c616832u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616834; /* fmov.s @r15+,fr14 */
L8c616834: sa_motion_publish_stop(ctx,0x8c616834u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616836; /* fmov.s @r15+,fr15 */
L8c616836: sa_motion_publish_stop(ctx,0x8c616836u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616838; /* mov.l @r15+,r9 */
L8c616838: sa_motion_publish_stop(ctx,0x8c616838u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61683a; /* mov.l @r15+,r10 */
L8c61683a: sa_motion_publish_stop(ctx,0x8c61683au); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61683c; /* mov.l @r15+,r11 */
L8c61683c: sa_motion_publish_stop(ctx,0x8c61683cu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61683e; /* mov.l @r15+,r12 */
L8c61683e: sa_motion_publish_stop(ctx,0x8c61683eu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616840; /* mov.l @r15+,r13 */
L8c616840: sa_motion_publish_stop(ctx,0x8c616842u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
